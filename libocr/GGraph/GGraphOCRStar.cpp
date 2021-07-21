//___________GGraphOCRStar.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ***********************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "../OCRString/php2stl.h"
#include "GGraph.h"

namespace ocr {
/** @bref  вычисление параметров звездочек */
void GGraph::buildOCRStars() {
    int print = 0;
    int starCount = (int)starArray.size();
    if (!starCount) {
        cout << "no content in graph\n";
        return;
    }
    
    
    //создаем массив line_data для поиска фокальных линий и вычисляем толщину
    //линий
    attributeLinePixels();  ////TIME_PRINT_ cout<<"1"<<endl;
    //находим смежные T_POINT
    buildXPoint();  //TIME_PRINT_ cout<<"2"<<endl;
    //сглаживаем фокальные линии
    focalLineMid(); //TIME_PRINT_ cout<<"3"<<endl;
    //находим экстремумы
    findExtremAlpha();  //TIME_PRINT_ cout<<"4"<<endl;
    //findExtremPoints();
    // находим круги
    detectCircle(); //TIME_PRINT_ cout<<"5"<<endl;
    //переводим неустойчивые звездочки X_POINT T_POINT и точки экстремума в
    // P_POINT
    detectP_POINT(); //TIME_PRINT_ cout<<"6"<<endl;
    //определяем какие линии являются кривыми Bezier хотя и не имеют точек
    //экстремума
    focalLineLengthMid();   //TIME_PRINT_ cout<<"7"<<endl;
    //создаем массив line_data для поиска фокальных линий и вычисляем толщину
    //линий
    attributeLinePixels(); //TIME_PRINT_ cout<<"8"<<endl;
    //строим маски 32х32 для каждой фокальной линии
    //buildFocalLineMask(); //TIME_PRINT_ cout<<"9"<<endl;
    //вычисляем углы кривизны OCRLine
    setCorrelationCurve();
    //draw.printTextGraphSVG(this, "/OSBL/CORRELATION/__drawText.html", 1); exit(0);
    //draw.printGraphSVG(this, "/OSBL/CORRELATION"); exit(0);
    // printToScreenBinary();
    starCount = (int)starArray.size();
    //заполняем массивы point и вычисляем среднюю толщину штриха
    //draw.printGraphSVG(this, "/OSBL/CORRELATION_2"); exit(0);
    
    for (int n = 0; n < starCount; n++) {
        OCRStar &star = starArray[n];
        star.pointCount = star.starCount;
        star.sIndex=star.index;
        int lineWSum = 0;
        int lineWCount = 0;
        star.sumLenA = 0;
        
        for (int j = 0; j < star.starCount; j++) {
            
            OCRLine &line = focalLine[star.line[j]];
            OCRStar &star_ = starArray[star.star[j]];
            if (star.dir[j]) { //записываем соответстие OCRStar и  OCRLine
                line.startIndex = j;
            } else {
                line.endIndex = j;
            }
            star.point[j] = star_.cPoint;
            star.point[j].data = star.star[j];
            if (line.lineW < 3) {
                line.lineW = LINE_SIZE;
            } else if (line.lenA > line.lineW * 0.75) {
                lineWSum += line.lineW;
                lineWCount++;
            }
            star.sumLenA += line.lenA;
            //проверяем толщины линий соседних OCRStar
            for (int g = 0; g < star_.starCount; g++) {
                OCRLine &line = focalLine[star_.line[g]];
                if (line.lineW < 3) {
                    line.lineW = LINE_SIZE;
                } else if (line.lenA > line.lineW * 0.75) {
                    lineWSum += line.lineW;
                    lineWCount++;
                }
            }
        }
        if (!lineWCount) {
            star.lineW = LINE_SIZE;
        } else {
            star.lineW = (float)lineWSum / lineWCount;
        }
        //лимитируем длину лучей звездочки. Используется в @fn
        //GGraphOCR::OCRStarCorrelationLookup1 лимитирование длины увеличивает
        //вариабельность эталонов букв и сокращает влияние шумовых пересечений и
        //разрывов фокальных линий. Для длинных кривых существует случаи когда две
        //различные кривые исходящие из одной OCRStar имеют одинаковые углы или углы
        //не совпадающие с углом под которым линия исходит из OCRStar лимитирование
        //нормирует углы исходящих из OCRStar кривых
        // и устраняет вариабельность длин лучей звездочки.
        //Характеристики фокальных линий лучей звездочки распознаем
        //при сравнении фокальных линий двух звездочек.
        if (star.type == O_POINT)
            continue;
        star.cluster = 0;
        for (int j = 0; j < star.lineCount; j++) {
            OCRLine &line = focalLine[star.line[j]];
            int lw = line.lineW;
            // cout<<"star:"<<star.index<<" lw:"<<lw<<" line:"<<line.index<<"
            // lineW:"<<line.lineW<<endl;
            int l = line.lineW * 3.0f;
            if (line.lenA > l) {
                if(l > line.lenA/2) l = line.lenA/2;
                star.len[j] = l;
                // int dl = star.lineW;
                // if (l + dl >= line.lenA) { dl = line.lenA - dl - 1; }
                // cout << "line.lineW:" << (int)line.lineW << " dl:" << dl << endl;
                if (star.dir[j]) {
                    //вычисляем статистически устойчивую точку конца луча
                    int xSum = 0;
                    int ySum = 0;
                    int sInd = 0;
                    int i = l - lw;
                    while (i < l + lw && i < line.lenA) {
                        OCRPoint &p = pointArray[line.pointIndex + i];
                        xSum += p.x;
                        ySum += p.y;
                        sInd++;
                        i++;
                    }
                    star.point[j] = pointArray[line.pointIndex + l];
                    star.point[j].x = xSum / sInd;
                    star.point[j].y = ySum / sInd;
                } else {
                    //вычисляем статистически устойчивую точку конца луча
                    int xSum = 0;
                    int ySum = 0;
                    int sInd = 0;
                    int i = line.lenA - l + lw;
                    while (i > line.lenA - l - lw && i > 0) {
                        OCRPoint &p = pointArray[line.pointIndex + i];
                        xSum += p.x;
                        ySum += p.y;
                        sInd++;
                        i--;
                    }
                    star.point[j] = pointArray[line.pointIndex + line.lenA - l];
                    star.point[j].x = xSum / sInd;
                    star.point[j].y = ySum / sInd;
                }
                star.point[j].type = S_POINT;
                star.cluster = S_POINT;
            } else {
                star.len[j] = line.lenA - 1;
            }
            //нормируем координаты точек OCRStar по центру звездочки
            OCRPoint &p = star.point[j];
            p.x -= star.cPoint.x;
            p.y -= star.cPoint.y;
        }
    }
    
    //при распознавании фокальных линий в звездочеке в массив star
    //записаны  id номера всех  OCRStar
    //которые соединены линиями с этой звездочкой.
    //в массив lines записаны id номера линий которые начинаются
    //или заканчиваются по координатам ее центра
    
    // Вычисление меры длины линии по отношению к сумме длин линий и //
    // периметра звездочки по концу линии или ближайшему экстремуму. //
    // Вычисление относительных углов между парами линий. //
    for (int t = 0; t < starCount; t++) {
        OCRStar &star = starArray[t];
        if (star.type == O_POINT)
            continue;
        DR("t:" << t << " type:" << (int)star.type << " l:" << (int)star.level
           << " sumLen0:" << (int)star.sumLen << endl);
        
        // val коэффициент для перевода угла из радиан в градусы
        float rad = 57.2958; //  val=180.0/pi=57.2958; float pi=3.14159265;
        
        if (star.type == L_POINT) {
            star.level = 0;
            star.sumLen = star.len[0];
            star.sumLen1 = star.len[0];
            star.minLen = star.len[0];
            star.disp = 100;
        }
        
        //определяем  соотношение длины каждой фокальной линий и суммы
        //всех линий и периметра звездочки (понадобится для вычисления корреляции
        //звездочек)
        star.level = 0;
        star.disp = 0;
        star.sumLen = 0;
        star.sumLen1 = 0;
        int len;
        
        star.sumLen += star.len[0];
        star.sumLen1 += star.len[0];
        star.minLen = star.len[0];
        
        if (star.type != L_POINT && star.type != Z_POINT) {
            //вычисляем длину периметра звездочки и cумму длин линий
            for (int m = 1; m < star.starCount; m++) {
                OCRPoint &p0 = star.point[m - 1];
                OCRPoint &c1 = star.point[m];
                OCRLine &line = focalLine[star.line[m]];
                star.sumLen += star.len[m];
                star.sumLen1 += star.len[m];
                if (line.lenA < star.minLen)
                    star.minLen = line.lenA;
                len =
                sqrt((p0.x - c1.x) * (p0.x - c1.x) + (p0.y - c1.y) * (p0.y - c1.y));
                star.sumLen1 += len;
                // star.lP[m - 1]=len;
            }
            OCRPoint &p0 = star.point[0];
            OCRPoint &c1 = star.point[star.type - 1];
            len = sqrt((p0.x - c1.x) * (p0.x - c1.x) + (p0.y - c1.y) * (p0.y - c1.y));
            star.sumLen1 += len;
            // star.lP[star.type - 1]=len;
            
            //вычисляем среднюю лимитированную длину линии звездочки
            star.mLen = (float)star.sumLen / star.starCount;
        } else {
            star.mLen = star.minLen;
        }
        //вычисляем соотношение средней длины линии
        //к сумме всех длин линий и периметра
        
        //OCRPoint &p = star.cPoint;
        
        // цикл перебора лучей звездочки
        for (int i = 0; i < star.starCount; i++) {
            OCRLine &line = focalLine[star.line[i]];
            int l = line.lenA;
            //вычисление массива относительных весов корреляции фокальной линии в
            //общей корреляции OCRStar
            star.line_weight[i] = l * 100 / star.sumLenA;
            
            //вычисление меры длины линии по отношению
            //к сумме длин линий и периметра звездочки
            // star.lS[i]=(line.length * 100)/star.sumLen1;
            //вычисление относительной длины отрезка периметра
            // star.lP[i]=(star.lP[i] * 100)/star.sumLen1;
            
            //вычисляем сумму отклонений длин линий от среднего
            star.disp += abs(star.mLen - l);
            
            if (l == 0)
                continue;
            
            //вычисление углов
            //угол отсчитываем от горизонтальной оси против часовой стрелки
            //угол измеряется по лимитированой по длине фокальной линии
            OCRPoint &c1 = star.point[i];
            
            // sin(alpha)=dy/L; cos(alpha)=dx/L где L длина луча
            int dx = c1.x;
            int dy = -c1.y;
            
            //вычисление угла линии в градусах
            float mAf =
            rad * atan2f((float)dy, (float)dx); // стандартная область определнеия
            // atan2f  -pi<=atan2f<=pi (f-float)
            
            // получение области определнеия atan2f от 0 до 360 градусов (от 0 до
            // 2*pi)
            if (mAf < 0)
                mAf += 360.0; // переопределение atan2f для отрицательных углов
            // pi<=atan2f+2*pi<=2*pi
            star.angle[i] = (ushort)mAf;
            DR("star:" << star.index << "     l:" << l
               << "  star.mA:" << star.angle[i] << " dx:" << dx
               << " dy:" << dy << " line:" << star.line[i] << endl);
            
        } // конец цикла перебора лучей звездочки
        
        //вычисление меры дисперсии длин линий звездочки
        star.disp = (star.disp * 100) / (star.sumLen + 1);
        if (star.disp > 100)
            star.disp = 100;
        
        //вычисление габарита
        //проверяем концы лимитированных лучей звездочки
        //инициализируем параметры габарита звездочки
        int xMin = 24000;
        int yMin = 24000;
        int xMax = 0;
        int yMax = 0;
        
        //проверяем центр звездочки
        OCRPoint &c1 = star.cPoint;
        if (c1.x > xMax)
            xMax = c1.x;
        if (c1.y > yMax)
            yMax = c1.y;
        if (c1.x < xMin)
            xMin = c1.x;
        if (c1.y < yMin)
            yMin = c1.y;
        
        //проверяем концы лучей
        for (int index = 0; index < star.starCount; index++) {
            OCRPoint &c = star.point[index];
            if (c.x + c1.x > xMax)
                xMax = c.x + c1.x;
            if (c.y + c1.y > yMax)
                yMax = c.y + c1.y;
            if (c.x + c1.x < xMin)
                xMin = c.x + c1.x;
            if (c.y + c1.y < yMin)
                yMin = c.y + c1.y;
            
            //проверяем все точки линии
            OCRLine &line = focalLine[star.line[index]];
            OCRPoint *pData = (OCRPoint *)&pointArray[line.pointIndex];
            int len = star.len[index];
            int lenA = line.lenA - 1;
            for (int n1 = 0; n1 < len; n1++) {
                OCRPoint c2;
                star.dir[index] ? c2 = pData[n1] : c2 = pData[lenA - n1];
                if (c2.x > xMax)
                    xMax = c2.x;
                if (c2.y > yMax)
                    yMax = c2.y;
                if (c2.x < xMin)
                    xMin = c2.x;
                if (c2.y < yMin)
                    yMin = c2.y;
            }
        }
        
        //записываем значения в звездочку
        star.xMin = xMin;
        star.xMax = xMax;
        star.yMin = yMin;
        star.yMax = yMax;
        
        /*
         
         //вычисляем масштаб приведения звездочки к размеру маски 32х32 необходимой
         //для вычисления корреляций звездочек в @fn GGraphOCR::OCRStarCorrelationLookup2
         //и @fn GGraphOCR::OCRStarCorrelationLookup3
         int   wMask=star.xMax - star.xMin;
         int   hMask=star.yMax - star.yMin;
         float scale;
         int   starW=20;
         float scale1;
         int   starW1=25;
         // starW - нормированный размер звездочки меньше 32 для того чтобы оставить
         место для
         // кругов на концах лучей
         if(wMask > hMask) {
         scale=(float)starW/wMask;
         scale1=(float)starW1/wMask;
         } else {
         scale=(float)starW/hMask;
         scale1=(float)starW1/hMask;
         }
         star.scale0=scale;
         star.scale1=scale1;
         star.xC=c1.x * scale - xMin * scale + 6;
         star.yC=c1.y * scale - yMin * scale + 6;
         star.xC1=c1.x * scale1 - xMin * scale1 + 4;
         star.yC1=c1.y * scale1 - yMin * scale1 + 4;
         */
        
        // DR( t<<" x:"<<c1.x<<" y:"<<c1.y<<"  xMin:"<<xMin<<" yMin:"<<yMin <<
        // " xMax:"<<xMax<<" yMax:"<<yMax<<" w:"<<wMask<<" h:"<<hMask<<" scale:"
        // <<scale <<endl);
    } // for(int t=0;t<starCount;t++){
    
    //TIME_PRINT_ cout<<"10"<<endl;
    buildHashTable();
    //распознаем опорные линии OCRStar
    
    //создаем поисковый массив фокальных точек для @fn bearingLineRecognition
    // заполняем массив data уменьшенный в четыре раза по отношению к размеру
    // GGraph
    int w1 = ncolumns / 2;
    int h1 = nrows / 2;
    if (w1 % 32 != 0)
        w1 = (w1 / 32 + 1) *
        32; //расширяем изображение если уменьшенное изображение обрезается
    
    id_data2 = (ushort *)calloc(2, w1 * h1);
    uchar *data = (uchar *)calloc(1, w1 * h1);
    
    for (int n = 0; n < starCount; n++) {
        OCRPoint &p = starArray[n].cPoint;
        int x = (p.y / 2) * w1 + p.x / 2;
        if (data[x])
            x++;
        if (data[x])
            x++;
        data[x] = 255;
        id_data2[x] = n + 1;
        // cout<<"x:"<<p.x<<" y:"<<p.y<<endl;
        
        OCRStar &star = starArray[n];
        //создаем поисковые регистры
        //bearingLineRecognition(star, 1);
        buildHashRegister(star, 1);
        //bearingLineRecognition(star, 2);
        //buildHashRegister(star, 2);
        
    }
    //запаковываем bytes_data как вертикально транспонированный массив по 32 бита
    bites_data_32_2 = (uint *)packImg32V_(w1, h1, data);
    free(data);
    initID2_flag = 1;
    
    //draw.printTextGraphSVG((void*)this,"/_Image2OCR/_1.svg",0);
    //создание поисковых регистров, масок концов лучей и хеша OCRStar
    for (int n = 0; n < starCount; n++) {
        OCRStar &star = starArray[n];
        star.corrC = 0;
        star.corr = 0;
        star.level = 0;
        star.status = 0;
        // cout<<"@ -> "<<endl;
        
        if (star.type == O_POINT) {
            star.hash = 109;
            //continue;
        }
        
        if(star.sumLenA < star.lineW*SERIF_LIMIT){
            if(star.type==L_POINT ){
                if(star.point[0].type==L_POINT)star.type = Z_POINT;
            }
        }
        
        if (star.type == Z_POINT) {
            //Z_POINT это одиночные точки. Для точек создаем кластер в котором есть одна OCRStar но нет OCRLine.
            star.hash = 110;
        }
        
        if(star.type==Z_POINT && star.starCount){
            if(starArray[star.star[0]].type==Z_POINT)continue; //не создаем кластеры для конца линии в кластере Z_POINT
        }
        if(star.type==L_POINT && star.sumLenA < star.lineW*SERIF_LIMIT){
            //не создаем кластеры для OCRStar с одной короткой линией.
            //такие кластеры содержат слишком мало данных для правильной синхронизации кластера буквы и текста
            continue;
        
        }
        
        buildCluster(star);
        
        // setOCRStarHash(star); //требует отладки буква _ཀ_1576662795.png
    }
    //создаем поисковый массив фокальных точек для @fn
    // GGraphOCR::OCRStarCorrelationLookup4
    //draw.printGraphSVG(this, "/OSBL/CORRELATION_3"); exit(0);
    // заполняем массив bites_data_32
    memset(id_data, 0, ncolumns * nrows * 2);
    data = (uchar *)calloc(1, ncolumns * nrows);
    
    uint countCluster=(uint)clusterArray.size();
    for (int n = 0; n < countCluster; n++) {
        OCRPoint &p = clusterArray[n].cPoint;
        int x = p.y * ncolumns + p.x;
        data[x] = 255;
        id_data[x] = n + 1;
    }
    
    //запаковываем bytes_data как вертикально транспонированный массив по 32 бита
    bites_data_32 = (uint *)packImg32V_(ncolumns, nrows, data);
    free(data);
    
    // printSVG("/storage/emulated/0/_Image2OCR/CORRELATION/1.svg"); exit(0);
    // printStars("/_Image2OCR/_2Draw_text.html",1);
    //draw.printTextGraphSVG(this, "/OSBL/CORRELATION", 1); exit(0);
    
    //draw.printGraphSVG(this,"/OSBL/CORRELATION"); exit(0);
    //draw.printGraphClustersSVG((void *)clusterArray.dataPtr(), clusterArray.size(), "/OSBL/CORRELATION");
    DR("done build stars" << endl);
} //_____________________________________________________________

void GGraph::buildXPoint() {
    int print = 0;
    int starCount = (int)starArray.size();
    
    DR("\n === buildOCRStar() X_POINT starCount:" << starCount << endl);
    
    //сначала переводим двe рядом стоящие точки типа Т_POINT в одну X_POINT - это
    //описывает все перекрестья линий как точку типа X_POINT
    
    for (int t = 0; t < starCount;
         t++) { // массив starArray уменьшается в процессе обработки
        DR("//process star t:" << t << endl);
        OCRStar &star = starArray[t];
        if (star.type != T_POINT)
            continue;
        int minLen = star.lineW * SERIF_LIMIT; //минимальное расстояние на котором звездочки еще
        //не объединяются
        // DR("initial star content"<<endl);
        // if (print)
        // printStar(star);
        if (star.minLen < minLen) {
            DR("star need process" << endl);
            for (int indexMain = 0; indexMain < star.lineCount; indexMain++) {
                OCRLine &line = focalLine[star.line[indexMain]];
                if (line.lenA < minLen) {
                    if (starArray[star.star[indexMain]].type == T_POINT) {
                        DR(t << " star have connected T_POINT star by indexMain:"
                           << indexMain << endl);
                        DR("@base star content:" << endl);
                        //if (print){draw.printGraphStar((void*)this,(void*)&star);}
                        OCRStar newStar; //создаем новую звездочку вместо двух смежных
                        newStar.cPoint = star.cPoint;
                        newStar.cPoint.type = X_POINT;
                        newStar.type = X_POINT;
                        newStar.starCount = 0;
                        newStar.lineCount = 0;
                        newStar.index = star.index;
                        newStar.lineW = star.lineW;
                        newStar.sumLen = 0;
                        int pointCount = 0;
                        //переписываем часть звездочки до соединяющей линии в новую
                        //звездочку необходимо исправить ошибку когда еще одна линия кроме
                        //короткой приходит в обе звездочки
                        DR("//переписываем часть звездочки до соединяющей линии в новую "
                           "звездочку indexMain:"
                           << indexMain << endl);
                        for (int index1 = 0; index1 < indexMain; index1++) {
                            newStar.line[pointCount] = star.line[index1];
                            newStar.star[pointCount] = star.star[index1];
                            newStar.dir[pointCount] = star.dir[index1];
                            newStar.starCount++;
                            newStar.lineCount++;
                            DR("index1:" << index1 << " star:" << star.star[index1]
                               << " pointCount:" << pointCount << endl);
                            pointCount++;
                        }
                        DR("@ first result new star content:" << endl);
                        if (print){draw.printGraphStar((void*)this,(void*)&newStar);}
                        
                        //получаем ссылку смежной звездочки
                        DR("get link to near star" << endl);
                        
                        int dStarID = star.star[indexMain];
                        
                        DR("indexMain:" << indexMain << " dStarID:" << dStarID << endl);
                        OCRStar &dStar = starArray[dStarID];
                        DR("print dStar content" << endl);
                        if (print)
                            draw.printGraphStar((void*)this,(void*)&dStar);
                        
                        int index2;
                        int index3 = 4;
                        //ищем в смежной звездочке соединяющую линию и переписываем в новую
                        //звездочку линии смежной звездочки после соединения
                        DR("search dStar for main star connection" << endl);
                        for (index2 = 0; index2 < dStar.starCount; index2++) {
                            DR("	" << index2 << " t:" << t << " dStar.star[" << index2
                               << "]: " << dStar.star[index2] << " len"
                               << (int)focalLine[dStar.line[index2]].lenA << endl);
                            if (dStar.star[index2] == t &&
                                focalLine[dStar.line[index2]].lenA < minLen) {
                                index3 = index2;
                                DR("	@connection line index:" << index2 << endl);
                                break;
                            }
                        }
                        //добавляем данные соседней  звездочки после соединяющей линии
                        DR("add content from near star after connection line index:"
                           << index3 << endl;)
                        for (index2 = index3 + 1; index2 < dStar.starCount; index2++) {
                            //переписываем линии смежной звездочки после соединяющей линии в
                            //новую звездочку"
                            DR("add near star lines after conection line index.  pointCount:"
                               << pointCount << endl);
                            newStar.line[pointCount] = dStar.line[index2];
                            newStar.star[pointCount] = dStar.star[index2];
                            newStar.dir[pointCount] = dStar.dir[index2];
                            newStar.starCount++;
                            newStar.lineCount++;
                            pointCount++;
                        }
                        DR("@ first result new star content:" << endl);
                        if (print)
                            draw.printGraphStar((void*)this,(void*)&newStar);
                        
                        //переписываем линии смежной звездочки до соединяющей линии в новую
                        //звездочку
                        DR(" add near star content before conection line index" << endl);
                        DR("index3:" << index3 << " indexMain:" << indexMain << endl);
                        for (index2 = 0; index2 < index3; index2++) {
                            newStar.line[pointCount] = dStar.line[index2];
                            newStar.star[pointCount] = dStar.star[index2];
                            newStar.dir[pointCount] = dStar.dir[index2];
                            newStar.starCount++;
                            newStar.lineCount++;
                            pointCount++;
                        }
                        DR("print content of newStar" << endl);
                        if (print)
                            draw.printGraphStar((void*)this,(void*)&newStar);
                        DR("indexMain:" << indexMain << " type:" << (int)star.type
                           << " pointCount:" << pointCount << endl);
                        
                        //переписываем линии основной звездочки после соединяющей линии в
                        //новую звездочку
                        DR("add lines of main star after connection line index" << endl);
                        
                        for (index2 = indexMain + 1; index2 < star.starCount; index2++) {
                            newStar.line[pointCount] = star.line[index2];
                            newStar.star[pointCount] = star.star[index2];
                            newStar.dir[pointCount] = star.dir[index2];
                            newStar.starCount++;
                            newStar.lineCount++;
                            pointCount++;
                        }
                        
                        //переписываем в основную звездочку новые данные
                        star = newStar;
                        DR("copy new star data to main star" << endl);
                        if (print)
                            draw.printGraphStar((void*)this,(void*)&star);
                        
                        //сдвигаем данные основного массива на одну позицию, заменяя ссылки
                        //на исходную и стираемую звездочку
                        
                        //ссылки на starID перенаправляем на новую звездочку
                        //и обновляем концы линий В присоединенных звездочках обновляем
                        //данные концов линий
                        
                        //Во всем исходном массиве сдвигаем ссылки на ID номера больше
                        // dStarID на единицу
                        for (int i = 0; i < starArray.size(); i++) {
                            OCRStar &starTest = starArray[i];
                            for (int j = 0; j < starTest.starCount; j++) {
                                if (starTest.star[j] == dStarID) {
                                    starTest.star[j] = star.index;
                                }
                                if (starTest.star[j] > dStarID) {
                                    starTest.star[j]--;
                                }
                                // if(starTest.line[j] > line.index) { starTest.line[j]--; }
                            }
                            //сдвигаем ID номера звездочек
                            if (i > dStarID)
                                starTest.index--;
                        }
                        //стираем звездочку с индексом dStarID
                        starArray.erase(dStarID);
                        starCount--;
                        
                        //Во всем массиве линий сдвигаем на 1 ссылки на ID номера линий
                        //больше line.index перенаправляем все ссылки на OCRStar c ID
                        // dStarID на star.index все ссылки на OCRStar c ID номерами больше
                        // dStarID сдвигаем на единицу обновляем данные концов линий
                        
                        for (int i = 0; i < focalLine.size(); i++) {
                            OCRLine &lineTest = focalLine[i];
                            if (lineTest.startID == dStarID) {
                                lineTest.startID = star.index;
                                lineTest.start = star.cPoint;
                            }
                            if (lineTest.endID == dStarID) {
                                lineTest.endID = star.index;
                                lineTest.end = star.cPoint;
                            }
                            if (lineTest.startID > dStarID)
                                lineTest.startID--;
                            if (lineTest.endID > dStarID)
                                lineTest.endID--;
                            // if(lineTest.index > line.index) lineTest.index--;
                        }
                        //линию с индексом line.index устанавливаем как линию нулевой длины
                        focalLine[line.index].lenA = 0;
                        focalLine[line.index].end = focalLine[line.index].start;
                        
                        //получаем новый индекс исходной звездочки
                        int indexNew = t;
                        if (indexNew > dStarID) {
                            indexNew--;
                        }
                        t = indexNew; //устанавливаем индекс цикла перебора звздочек
                        break;        // not add T_POINT to X_POINT
                    }               // if (starArray[star.star[indexMain]].type==T_POINT)
                }                 // if (flag < minLen)
            } // for (int indexMain=0; indexMain < star.starCount; indexMain++)
        }   // if (star.minLen < minLen)
    }     // for (int t=0; t < starArray.size(); t++)
    //"Вычисление меры длины линии, углов и габарита"
    DR("done X_POINT" << endl);
}

/** вспомогательная функция для @fn detectCircle */
void GGraph::buildCircle(OCRStar &star) {
    int print = 0;
    DR("=== crcle check star:" << star.index << endl;)
    int cX = 0;
    int cY = 0;
    int sumLen = 0;
    star.lineW = 0;
    for (int n = 0; n < star.lineCount; n++) {
        DR(" index:" << star.line[n] << " focalLine:" << focalLine.size() << endl);
        OCRLine &line = focalLine[star.line[n]];
        int len = line.lenA;
        OCRPoint *pData = (OCRPoint *)&pointArray[line.pointIndex];
        for (int i = 0; i < len; i++) {
            cX += pData[i].x;
            cY += pData[i].y;
            sumLen++;
        }
        star.lineW += line.lineW;
    }
    star.lineW = (float)star.lineW / star.lineCount;
    cX /= sumLen;
    cY /= sumLen;
    star.type = O_POINT;
    star.cPoint.type = O_POINT;
    star.cPoint.x = cX;
    star.cPoint.y = cY;
    star.radius = (float)sumLen / 3.14151828 / 2; //сохраняем в этой переменной радиус круга
    star.minLen = star.radius;
    star.mLen = star.radius;
    star.cluster = 0;
    if (star.radius < star.lineW) {
        star.corr = 0;
        return;
    }
    star.level = 0;
    star.sumLen = sumLen;
    star.sumLen1 = sumLen;
    star.sumLenA = sumLen;
    star.corr = 100;
    star.baseCount = 0;
    int dist = 0;
    DR("done detect circle. radius:" << (int)star.radius << endl);
    //проверяем корреляцию с кругом
    int r_ = star.radius * 100;
    int dl = star.lineW * 100;
    if (dl < 200)
        dl = 200;
    
    for (int n = 0; n < star.lineCount; n++) {
        DR(" index:" << star.line[n] << " focalLine:" << focalLine.size() << endl);
        OCRLine &line = focalLine[star.line[n]];
        //проверяем концы линий
        OCRPoint &p = line.start;
        dist = sqrt((cX - p.x) * (cX - p.x) + (cY - p.y) * (cY - p.y)) * 100;
        if (abs(dist - r_) > dl) {
            star.corr = 0;
            return;
        }
        OCRPoint &p1 = line.end;
        dist = sqrt((cX - p1.x) * (cX - p1.x) + (cY - p1.y) * (cY - p1.y)) * 100;
        if (abs(dist - r_) > dl) {
            star.corr = 0;
            DR("dl too big:" << dl << endl;)
            return;
        }
        //проверяем точки экстремума
        OCRPoint *pData = &pointArray[line.pointIndex];
        for (int i = 0; i < line.extremCount; i++) {
            OCRPoint &p = pData[line.extrem[i]];
            int dist = sqrt((cX - p.x) * (cX - p.x) + (cY - p.y) * (cY - p.y)) * 100;
            if (abs(dist - r_) > dl) {
                star.corr = 0;
                DR("dl too big:" << dl << endl;)
                return;
            }
        }
        //устанавливаем в линии флаг принадлежности к кругу
        for (int n = 0; n < star.lineCount; n++) {
            OCRLine &line = focalLine[star.line[n]];
            line.type = CIRCLE_LINE;
            line.extremCount = 0;
        }
    }
    
} //_____________________________________________________________________

/* распознавание кругов образованных фокальными линиями.
 кругом будем считать одну или две соединенных фокальных линий
 концы которых находятся на расстоянии не превышающем лимит
 и точки экстремума которых находятся на расстоянии 2Pi*r от
 точки усредненных координат этой линии или линий. */
void GGraph::detectCircle() {
    int print = 0;
    int starCount = (int)starArray.size();
    DR(" detect circle. count:" << starCount << endl);
    int limit = 3;
    //static vector<OCRPoint> circleArray; //массив проверки наличия круга по координатам
    vector<int> circleLineArray(focalLine.size()); //массив проверки наличия круга по номерам OCRLine в GGraph
    // 106
    for (int n = 0; n < starCount; n++) {
        //проверяем OCRStar в соответствии с типом
        OCRStar &star = starArray[n];
        DR("@new star check n:" << n << " type" << (int)star.type << endl);
        // printStar(star);
        //для OCRStar есть три случая в которых фокальные линии образуют один или
        //несколько кругов. в первом случае это одна линия которая исходит и
        //приходит в OCRStar во втором случае из OCRStar исходят две линии концы
        //которых находятся на расстоянии меньше лимита в третьем случае из OCRStar
        //исходят две линии концы которых оба находятся близко от конца третьей
        //линии. Такие линии образуют два круга с одной общей линией.
        
        //if (print) draw.printGraphStar((void*)this,(void*)&star);
        for (int n1 = 0; n1 < star.starCount; n1++) {
            OCRLine &line = focalLine[star.line[n1]];
            DR("line index:" << line.index << " status" << (int)line.status << endl);
            DR("n1:" << n1 << endl);
            //первый случай аналогичен кругу исходящему из L_POINT
            if (abs(line.start.x - line.end.x) < limit &&
                abs(line.start.y - line.end.y) < limit &&
                line.lenA > line.length * 3) {
                DR(" detect1 line:" << n1 << endl);
                OCRStar newStar;
                newStar.line[0] = line.index;
                newStar.len[0] = line.lenA;
                newStar.lineCount =
                1; //сохраняем в этой переменной количество линий в круге
                newStar.star[0] = n;
                newStar.starCount = 1;
                newStar.point[0] = star.cPoint;
                newStar.pointCount = 1;
                newStar.corr = 0;
                buildCircle(newStar);
                if (!newStar.corr)continue;
                int flagLen = 0;
                int flagFree=0;
                for(int i=0;i<newStar.lineCount;i++){
                    if(circleLineArray[newStar.line[i]]){
                        //необходимо проверить длину резервированной линии текста
                        //по отношению к другим линиям круга
                        flagLen+=newStar.len[i];
                    }else{
                        flagFree+=newStar.len[i];
                    }
                }
                if ((!flagLen || flagFree*100/flagLen > 50) && newStar.radius > LINE_SIZE / 2) { //записываем кластер круга, резервируем линии круга
                    newStar.index = (int)starArray.size();
                    starArray.push_back(newStar);
                    //резервируем линии круга
                    for(int i=0;i<newStar.lineCount;i++){
                        circleLineArray[newStar.line[i]]=1;
                    }
                    
                }
                continue;
            }
            //второй случай - две линии исходят из T_POINT или X_POINT и соединяются
            OCRPoint p;
            //выбираем для проверки дальний конец проверяемой линии
            star.dir[n1] == 1 ? p = line.end : p = line.start;
            //проверяем все линии T_POINT и X_POINT
            for (int n2 = 0; n2 < star.starCount; n2++) {
                if (n1 == n2)
                    continue;
                OCRLine &line2 = focalLine[star.line[n2]];
                OCRPoint p2;
                star.dir[n2] ? p2 = line2.end : p2 = line2.start;
                
                DR(" p.x:" << p.x << " p.y:" << p.y << " -> p2.x:" << p2.x
                   << " p2.y:" << p2.y << endl);
                
                if (abs(p.x - p2.x) < limit && abs(p.y - p2.y) < limit &&
                    line.lenA + line2.lenA > line.length * 2 + limit) {
                    DR(" detect2 line:" << n1 << ":" << n2 << endl);
                    OCRStar newStar;
                    
                    //проверяем сколько линий соединяют начальную и конечную точку круга
                    //это проверка исключения, в случае которого в букве Ф находится три круга
                    int minLineLen=line2.lenA;
                    //int minIndex=n2;
                    newStar.line[1] = line2.index;
                    for (int n3 = 0; n3 < star.starCount; n3++){
                        if(n3==n2 || n3==n1)continue;
                        OCRLine &line3 = focalLine[star.line[n3]];
                        OCRPoint p3;
                        star.dir[n3] ? p3 = line3.end : p3 = line3.start;
                        if (abs(p.x - p3.x) < limit && abs(p.y - p3.y) < limit &&
                            line.lenA + line3.lenA > line.length * 2 + limit) {
                            if(line3.lenA<minLineLen){
                                minLineLen=line3.lenA;
                                //minIndex=n3;
                                newStar.line[1] = line3.index;
                                newStar.len[1] = line3.lenA;
                            }
                        }
                    }
                    newStar.line[0] = line.index;
                    newStar.len[0] = line.lenA;
                    newStar.dir[0] = 2; //сохраняем в этой переменной количество линий в круге
                    newStar.lineCount = 2;
                    newStar.star[0] = n;
                    newStar.star[1] = star.star[n1];
                    newStar.starCount = 2;
                    newStar.point[0] = star.cPoint;
                    newStar.point[1] = p2;
                    newStar.pointCount = 2;
                    newStar.corr = 0;
                    
                    //if()
                    buildCircle(newStar);
                    if (newStar.corr) {
                        int flagLen = 0;
                        int flagFree=0;
                        for(int i=0;i<newStar.lineCount;i++){
                            if(circleLineArray[newStar.line[i]]){
                                //необходимо проверить длину резервированной линии текста
                                //по отношению к другим линиям круга
                                flagLen+=newStar.len[i];
                            }else{
                                flagFree+=newStar.len[i];
                            }
                        }
                        if ((!flagLen || flagFree*100/flagLen > 50) && newStar.radius > LINE_SIZE / 2) { //записываем кластер круга, резервируем линии круга
                            newStar.index = (int)starArray.size();
                            starArray.push_back(newStar);
                            //резервируем линии круга
                            for(int i=0;i<newStar.lineCount;i++){
                                circleLineArray[newStar.line[i]]=1;
                            }
                        }
                    }
                }
                
                //в продолжении разбора отрабатывает третий случай -
                //две линии исходят из OCRStar и соединяются третьим лучом из другой
                // OCRStar
                // line - первая линия и p точка конца линии
                // line2 - вторая линия и p2 точка конца линии
                //для line2 находим OCRStar на другом конце
                int id;
                star.dir[n2] ? id = line2.endID : id = line2.startID;
                if(id==n)continue;
                OCRStar &star2 = starArray[id];
                for (int n3 = 0; n3 < star2.starCount; n3++) {
                    OCRLine line3 = focalLine[star2.line[n3]];
                    if (line2.index == line3.index || line.index == line3.index)
                        continue;
                    OCRPoint p3;
                    star2.dir[n3] ? p3 = line3.end : p3 = line3.start;
                    if (abs(p.x - p3.x) < limit && abs(p.y - p3.y) < limit &&
                        line3.lenA + line2.lenA > line.length / 2 + limit &&
                        line3.lenA + line.lenA > line2.length / 2 + limit) {
                        DR(" detect3 line:" << n3 << ":" << n3 << endl);
                        OCRStar newStar;
                        newStar.dir[0] = 3; //сохраняем в этой переменной количество линий в круге
                        newStar.line[0] = line.index;
                        newStar.line[1] = line2.index;
                        newStar.line[2] = line3.index;
                        newStar.len[0] = line.lenA;
                        newStar.len[1] = line2.lenA;
                        newStar.len[2] = line3.lenA;
                        newStar.lineCount = 3;
                        newStar.star[0] = n;
                        newStar.star[1] = star.star[n1];
                        newStar.star[2] = line.endID;
                        newStar.starCount = 2;
                        newStar.point[0] = star.cPoint;
                        newStar.point[1] = p2;
                        newStar.point[2] = p3;
                        newStar.pointCount = 3;
                        newStar.corr = 0;
                        buildCircle(newStar);
                        if (!newStar.corr)
                            continue;
                        int flagLen = 0;
                        int flagFree=0;
                        for(int i=0;i<newStar.lineCount;i++){
                            if(circleLineArray[newStar.line[i]]){
                                //необходимо проверить длину резервированной линии текста
                                //по отношению к другим линиям круга
                                flagLen+=newStar.len[i];
                            }else{
                                flagFree+=newStar.len[i];
                            }
                        }
                        if ((!flagLen || flagFree*100/flagLen > 50) && newStar.radius > LINE_SIZE / 2) { //записываем кластер круга, резервируем линии круга
                            newStar.index = (int)starArray.size();
                            starArray.push_back(newStar);
                            //резервируем линии круга
                            for(int i=0;i<newStar.lineCount;i++){
                                circleLineArray[newStar.line[i]]=1;
                            }
                        }
                    }
                }
            }
        }
    }
    
} //____________________________________________________________________
    

/** @bref формирование масок 32х32 необходимой
 для вычисления корреляций звездочек в @fn GGraphOCR::OCRStarCorrelationLookup2
 */
void GGraph::buildFocalLineMask() {
    /** @img img2.jpg 256
     Для выполнения масштабонезависимого сравнения обе фокальные линии приводятся к
     одному масштабу. Для этого вычисляем отношение габарита тестовой
     и эталонной звездочки к размеру 32х32 Scale0 float.
     При вычислении масштаба и координат необходимо учитывать
     радиус растискивания поисковой области 7 или 9 пикселов.
     Битовую маску храним в структуре OCRLine.
     При растискивание фокальной линии битовой маской
     необходимо учитывать что первый бит в integer расположен справа
     поэтому все маски рисуем в зеркальном отображении по отношению к
     пиксельному изображению
     1000000000000000 16 бит => 0x8000 2 байта в hex
     
     // 7 pixel  wide mask
     uint maskC[]={
     0b0000000000011100,
     0b0000000000111110,
     0b0000000001111111,
     0b0000000001111111,
     0b0000000001111111,
     0b0000000000111110,
     0b0000000000011100};
     
     
     // 9 pixel  wide mask
     uint maskC[]={
     0b0000000011111100,
     0b0000000111111110,
     0b0000001111111111,
     0b0000001111111111,
     0b0000001111111111,
     0b0000001111111111,
     0b0000001111111111,
     0b0000000111111110,
     0b0000000011111100};
     
     */
    /*
    // 9 pixel  wide mask
    uint maskC[] = {0b0000000001111100, 0b0000000011111110, 0b0000000111111111,
        0b0000000111111111, 0b0000000111111111, 0b0000000111111111,
        0b0000000111111111, 0b0000000011111110, 0b0000000001111100};
    int print = 0;
    uint size =(uint)focalLine.size(); // количество фокальных линий на странице или ID номеров.
    DT("Количество фокальных линий на странице size=" << size << endl);
    // цикл перебора фокальных ЛИНИЙ на странице.
    for (int m = 0; m < size; m++) { // size
        DT(endl << "цикл по количеству фокальных линий в букве  m=" << m << endl);
        OCRLine &line = focalLine[m];
        int len = line.lenA - 1;
        int lenA = line.lenA;
        line.mask = 0;
        line.SumBitOn = 0;
        if (len < line.lineW * SERIF_LIMIT)
            continue;
        line.mask = 1;
        //определяем минимум и максимум координат линии
        OCRPoint *pData = (OCRPoint *)&pointArray[line.pointIndex];
        int xMin = 100000;
        int xMax = 0;
        int yMin = 100000;
        int yMax = 0;
        for (int n = 0; n < lenA; n++) {
            OCRPoint &c = pData[n];
            // cout<<"x:"<<c.x<<" y:"<<c.y<<endl;
            if (c.x > xMax)
                xMax = c.x;
            if (c.x < xMin)
                xMin = c.x;
            if (c.y > yMax)
                yMax = c.y;
            if (c.y < yMin)
                yMin = c.y;
        }
        line.xMax = xMax;
        line.xMin = xMin;
        line.yMax = yMax;
        line.yMin = yMin;
        //размеры габарита линии
        int wLine = line.xMax - line.xMin;
        int hLine = line.yMax - line.yMin;
        float scale;
        int lineW = 23;
        int *p;
        int d = 1; //просчет промежуточных точек линии. 0 - не выполнять
        int xOld = 0;
        int yOld = 0;
        
        // lineW - нормированный размер габарита линии меньше 32 для того чтобы
        // оставить место для растискивания точек линии
        if (wLine > hLine) {
            scale = (float)lineW / wLine;
        } else {
            scale = (float)lineW / hLine;
        }
        line.scale0 = scale;
        // cout<<"len:"<<len<<" lineW:"<<wLine<<" hLine:"<<hLine<<"
        // scale:"<<scale<<endl;
        //строим маску 32x32
        // вывод тестовой графики
         cout<<"исходная фокальная линия"<<endl;
         p = line.maskLine;
         memset((char *)line.maskLine, 0, 32 * 4);
         
         for (int m = 0; m < lenA; m++) { //len
         OCRPoint &c = pData[m];
         int x = (int)((c.x - xMin)  * scale + 4.0) ; //
         int y = (int)((c.y - yMin)  * scale + 4.0);
         *(p + y) =*(p + y) | (1<<x);  //тестовое рисование линии (без
         растискивания)
         
         if (d && m > 0) {
         //рисуем промежуточную точку линии
         int x_ = (x + xOld) / 2;
         int y_ = (y + yOld) / 2;
         *(p + y_) =*(p + y_) | (1<<x_);  //тестовое рисование линии
         (без растискивания)
         }
         xOld = x;
         yOld = y;
         
         }
         printOCRMask(line.maskLine);
         cout<<endl<<"маска фокальной линии"<<endl;
     
        //<конец вывода графики
        
        p = line.maskLine;
        memset((char *)line.maskLine, 0, 32 * 4);
        
        d = 1; //просчет промежуточных точек линии. 0 - не выполнять
        xOld = 0;
        yOld = 0;
        for (int m = 0; m < lenA; m++) { // len
            OCRPoint &c = pData[m];
            int x = (c.x - xMin) * scale + 4; //
            int y = (c.y - yMin) * scale + 4;
            
            for (int i = 0; i < 9; i++) {
                *(p + y + i - 4) |= maskC[i] << (x - 4);
            }
            // *(p + y) =*(p + y) | (1<<x);  //тестовое рисование линии (без
            //растискивания)
            
            if (d && m > 0) {
                //рисуем промежуточную точку линии
                int x_ = (x + xOld) / 2;
                int y_ = (y + yOld) / 2;
                
                for (int i = 0; i < 9; i++) {
                    *(p + y_ + i - 4) |= maskC[i] << (x_ - 4);
                }
                // *(p + y_) =*(p + y_) | (1<<x_);  //тестовое рисование линии (без
                //растискивания)
            }
            xOld = x;
            yOld = y;
        }
        // iprintOCRMask(maskLine);
        
        //создаем регистры проверки точек линии по маске 32х32
        int dl = 4 / scale;
        OCRPoint &p0 = pData[0 + dl];
        line.p0_r = 1 << (int)((p0.x - xMin) * scale + 4);
        line.p0_y = (p0.y - yMin) * scale + 4;
        p0.data = 4 / scale; //сохраняем положение в линии
        p0.status = 15;
        line.pointC[0] = p0; //сохраняем исходные координаты конторольной точки
        OCRPoint &p1 = pData[len / 3];
        line.p1_r = 1 << (int)((p1.x - xMin) * scale + 4);
        line.p1_y = (p1.y - yMin) * scale + 4;
        p1.data = lenA / 3;
        p1.status = 20;
        line.pointC[1] = p1; //сохраняем исходные координаты конторольной точки
        OCRPoint &p2 = pData[len / 2];
        line.p2_r = 1 << (int)((p2.x - xMin) * scale + 4);
        line.p2_y = (p2.y - yMin) * scale + 4;
        p2.data = lenA / 2;
        p2.status = 30;
        line.pointC[2] = p2; //сохраняем исходные координаты конторольной точки
        OCRPoint &p3 = pData[len - len / 3];
        line.p3_r = 1 << (int)((p3.x - xMin) * scale + 4);
        line.p3_y = (p3.y - yMin) * scale + 4;
        p3.data = lenA - line.lenA / 3;
        p3.status = 20;
        line.pointC[3] = p3; //сохраняем исходные координаты конторольной точки
        OCRPoint &p4 = pData[len - dl];
        line.p4_r = 1 << (int)((p4.x - xMin) * scale + 4);
        line.p4_y = (p4.y - yMin) * scale + 4;
        p4.data = lenA - 4 / scale;
        p4.status = 15;
        line.pointC[4] = p4; //сохраняем исходные координаты конторольной точки
        //подсчитываем количество едениц в маске 32х32
        //этот параметр используется при вычисленнии корреляции  фокальных линий
        //в @fn OCRStarCorrelationLookup2
        int SumBitOn = 0;
        for (int m = 0; m < 32; m++) {
            int Sb = line.maskLine[m];
            if (Sb != 0) { // 0xFFFFFFFF
                // Регистровые суммы маски mask32.On[m] (подсчет единиц) по  X
                Sb -= (Sb >> 1) & 0x55555555; // 2-битовые ячейки
                Sb = (Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
                Sb = (Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
                Sb += Sb >> 8;                                     // 16-битовые
                Sb += Sb >> 16; // 32-битовая ячейка
                SumBitOn +=
                Sb & 0x3F; // 63 Обнуляем старшие разряды, содержащие "мусор"
            }
        }
        line.SumBitOn = SumBitOn;
        //вычисляем координаты концов линии в маске 32x32. Эти координаты
        //используются для вычисления афинных преобразований в @fn
        //OCRLineMaskAffineCorrelation
        line.start32.x = (line.start.x - line.xMin) * scale + 4;
        line.start32.y = (line.start.y - line.yMin) * scale + 4;
        line.end32.x = (line.end.x - line.xMin) * scale + 4;
        line.end32.y = (line.end.y - line.yMin) * scale + 4;
    }
    */
} //------------------------------------------------------------------------------------------------------

/** @bref  Функция вычисления хеша звездочки */
void GGraph::setOCRStarHash(OCRStar &star) {
    /** Вычисление хеша звездочки описано в функции @fn buildHashTable */
    char type = star.type; // 13524 //2323
    ushort h = type;
    int v[] = {3, 6, 9, 12};
    
    if (star.starCount == star.type) {
        for (int n = 0; n < star.starCount; n++) {
            type = starArray[star.star[n]].type;
            h += type << v[n];
        }
    } else {
        int index = 0;
        for (int n = 0; n < star.starCount; n++) {
            OCRLine &line = focalLine[star.line[n]];
            if (line.lenA < star.lineW * 1.5)
                continue;
            type = starArray[star.star[n]].type;
            h += type << v[index];
            index++;
        }
    }
    star.hash = hashTableOCRStar[h];
    
} //_________________________________________________________
/** выявление неустойчивых точек типов T_POINT X_POINT и создание OCRStars для
 * каждой точки экстремума */
void GGraph::detectP_POINT() {
    /**
     Для букв шрифта обычно существуют два варианта написания - Sanc и Serif.
     Эти два вида написания отличаются отсутствием или наличием штрихов-зачечек на
     концах линий и в точках пересечения штрихов. При выполнении распознавания
     фокальных линий такие штрихи-засечки образуют короткие фокальные линии которые
     присоединены к точкам пересечения фокальных линий. При распознавании фокальных
     линий шрифта без таких засечек в таких точках обычно находится точка экстремума
     кривой. Для того чтобы распознавать такие шрифты не увеличивая количество
     эталонов, приравниваем точки с штрихами-засечками к точкам экстремумов фокальных
     линий.
     
     Для распознавания сложных кривых создаем OCRStar для каждой точки перегиба
     фокальной линии. Точкой перегиба считаем такую точку экстремума в которой
     фокальная линия меняет свое направление на угол больший лимита. Переписываем
     данные OCRStar и OCRLine в которых находились точки перегиба фокальной линии.
     */
    
    uint focalLineSize = (uint)focalLine.size();
    uint starArraySize = (uint)starArray.size();
    
    // Производим классификацию Т и Х точек, для неустойчивых точек изменяем тип
    // точки на P_POINT
    for (int n = 0; n < starArraySize; n++) {
        OCRStar &star = starArray[n];
        if (star.type != T_POINT && star.type != X_POINT)
            continue;
        int flag = 0;
        for (int m = 0; m < star.starCount; m++) {
            if (starArray[star.star[m]].type != L_POINT)
                continue;
            // cout<<floor(star.lineW)<<" "<<focalLine[star.line[m]].lenA<<endl;
            if (focalLine[star.line[m]].lenA < star.lineW * SERIF_LIMIT) {
                star.type--;
                flag = 1;
            }
        }
        if (flag) {
            //исправляем тип точки в фокальной линии
            for (int j = 0; j < star.lineCount; j++) {
                OCRLine &line = focalLine[star.line[j]];
                if (star.dir[j]) {
                    line.start.type = star.type;
                } else {
                    line.end.type = star.type;
                }
            }
        }
    }
    
    
    //не добавляем P_POINT в точках экстремума кривой. Вгиб кривой учитываем как углы между опорной линией и тремя точками кривой
    //@fn setCorrelationCurve()
    //return;
    
    //Создаем OCRStar для каждой точки перегиба фокальной линии
    for (int n = 0; n < focalLineSize; n++) {
        OCRLine &line = focalLine[n];
        line.parentID = line.index;
        int extremCount = line.extremCount;
        if (!extremCount || line.type == CIRCLE_LINE) continue;
        int lenA = line.lenA;
        OCRPoint *lineData = (OCRPoint *)&pointArray[line.pointIndex];
        line.nodeCount = 0;
        
        /*
         uchar     extrem[MAX_EXTREM];
         OCRPoint  p0;    //точка начала квадратичной кривой
         OCRPoint  p1;    //точка середины квадратичной кривой
         OCRPoint  p2_1;  //опорная точка первой квадратичной кривой
         OCRPoint  p2_2;  //опорная точка второй квадратичной кривой
         OCRPoint  p3;    //точка конца квадратичной кривой
         
         
         //для всех точек экстремума проверяем длины линий образуемых экстремумом
         //и угол образуемый кривыми Безъе этих линий.
         for(int j=0; j < extremCount; j++) {
         OCRPoint &p=lineData[line.extrem[j]];
         //if(p.type!=E_POINT)continue;
         
         //проверяем расстояние до начала линии
         if(line.extrem[j] < line.lineW*1.25) continue;
         //проверяем расстояние до конца линии
         if(line.lenA - line.extrem[j] < line.lineW*1.25) continue;
         
         
         //проверяем длину первой линий образуемой экстремумом и вычисляем ее опорную
         точку int len1=0; if(j > 0) { p0=lineData[line.extrem[j - 1]];
         len1=line.extrem[j] - line.extrem[j - 1];
         if(len1 > line.lineW) {
         int x=line.extrem[j - 1] + len1/2;
         //усредняем координаты трех соседних точек в середине линии между
         точками экстремума p1.x=(lineData[x + 1].x + lineData[x].x + lineData[x -
         1].x)/3; p1.y=(lineData[x + 1].y + lineData[x].y + lineData[x - 1].y)/3;
         //вычисляем координаты опорной точки квадратичной кривой
         p2_1.x=-0.5 * p0.x + 2.0 * p1.x - 0.5 * p.x;
         // координата “x” первой управляющей точки.
         p2_1.y=-0.5 * p0.y + 2.0 * p1.y - 0.5 * p.y;
         } else {
         p2_1=p0;
         }
         
         } else {
         len1=line.extrem[j];
         p0=line.start;
         if(len1 > line.lineW) {
         int x=len1/2;
         //усредняем координаты трех соседних точек в середине линии между
         точками экстремума p1.x=(lineData[x + 1].x + lineData[x].x + lineData[x -
         1].x)/3; p1.y=(lineData[x + 1].y + lineData[x].y + lineData[x - 1].y)/3;
         //вычисляем координаты опорной точки квадратичной кривой
         p2_1.x=-0.5 * p0.x + 2.0 * p1.x - 0.5 * p.x;
         // координата “x” первой управляющей точки.
         p2_1.y=-0.5 * p0.y + 2.0 * p1.y - 0.5 * p.y;
         } else {
         p2_1=p0;
         }
         }
         //проверяем длину второй линий образуемой экстремумом и вычисляем ее опорную
         точку int len2=0; if(j < line.extremCount - 1) { len2=line.extrem[j + 1] -
         line.extrem[j]; p3=lineData[line.extrem[j + 1]]; if(len2 > line.lineW) { int
         x=line.extrem[j] + len2/2;
         //усредняем координаты трех соседних точек в середине линии между
         точками экстремума p1.x=(lineData[x + 1].x + lineData[x].x + lineData[x -
         1].x)/3; p1.y=(lineData[x + 1].y + lineData[x].y + lineData[x - 1].y)/3;
         //вычисляем координаты опорной точки квадратичной кривой
         p2_2.x=-0.5 * p.x + 2.0 * p1.x - 0.5 * p3.x;
         // координата “x” первой управляющей точки.
         p2_2.y=-0.5 * p.y + 2.0 * p1.y - 0.5 * p3.y;
         } else {
         p2_2=p3;
         }
         } else {
         len2=lenA - line.extrem[j];
         p3=line.end;
         if(len2 > line.lineW) {
         int x=line.extrem[j] + len2/2;
         //усредняем координаты трех соседних точек в середине линии между
         точками экстремума p1.x=(lineData[x + 1].x + lineData[x].x + lineData[x -
         1].x)/3; p1.y=(lineData[x + 1].y + lineData[x].y + lineData[x - 1].y)/3;
         //вычисляем координаты опорной точки квадратичной кривой
         p2_2.x=-0.5 * p.x + 2.0 * p1.x - 0.5 * p3.x;
         // координата “x” первой управляющей точки.
         p2_2.y=-0.5 * p.y + 2.0 * p1.y - 0.5 * p3.y;
         } else {
         p2_2=p3;
         }
         }
         
         //проверяем угол образующих отрезков двух кривых Безъе которые проведены к этой
         точке
         //такой угол равен также углу между двумя касательными к двум кривым Безъе
         //в этой точке
         float alpha=pointsAngle(p, p2_2, p2_1);
         // if(alpha>180) alpha=360-alpha;
         
         if(alpha < 120) {
         //проверяем расстояние до предыдущей точки перегиба
         if(extremCount && line.extrem[j]-extrem[extremCount-1] <line.lineW*1.25)
         continue; extrem[extremCount]=line.extrem[j]; extremCount++;
         }
         }
         */
        int startID = line.startID;
        int endID = line.endID;
        
        OCRStar &startStar = starArray[startID];
        OCRStar &endStar = starArray[endID];
        uchar *extrem = line.extrem;
        
        //делим линию в соответствии с найденными точками экстремума кривой
        for (int j = 0; j < extremCount; j++) {
            OCRPoint &p = lineData[extrem[j]];
            p.type=P_POINT;
            
            //создаем новую OCRStar;
            OCRStar newStar;
            newStar.cPoint = p;
            newStar.type=P_POINT;
            newStar.index = (int)starArray.size();
            newStar.starCount = 2;
            newStar.lineCount = 2;
            newStar.pointCount = 2;
            newStar.baseCount = 0;
            newStar.dir[0] = 0;
            newStar.dir[1] = 1;
            
            //проверяем длины линий образуемых экстремумом
            
            //для первой точки экстремума
            if (j == 0) {
                newStar.star[0] = line.startID;
                newStar.line[0] = n;
                // исправляем OCRStar начала линии
                for (int m = 0; m < startStar.lineCount; m++) {
                    if (startStar.line[m] == n) {
                        startStar.star[m] = newStar.index;
                        break;
                    }
                }
                
                // исправляем конец линии
                line.end = p;
                line.endID = newStar.index;
                
                // исправляем длину линии
                line.lenA = extrem[j] + 1;
                line.lenE0 = line.lenA;
                line.lenE1 = line.lenA;
                line.length =
                sqrt((line.start.x - line.end.x) * (line.start.x - line.end.x) +
                     (line.start.y - line.end.y) * (line.start.y - line.end.y));
                //переписываем точки экстремума линии из буферного массива
                line.extremCount = 0;
                for (int m = 0; m < extremCount; m++) {
                    if (line.extrem[m] < extrem[j]) {
                        line.extrem[line.extremCount] = line.extrem[m];
                        line.extremCount++;
                    } else
                        break;
                }
            }
            //создаем новую линию
            OCRLine newLine;
            newLine.start = p;
            newLine.startID = newStar.index;
            newLine.pointIndex = line.pointIndex + extrem[j];
            newLine.index = (uint)focalLine.size();
            newLine.extremCount = 0;
            newLine.parentID = line.index;
            int size = (int)starArray.size();
            newLine.type = NEW_LINE;
            
            //для промежуточных точек экстремума
            if (j < extremCount - 1) {
                if (j > 0) {
                    newStar.star[0] = size - 1;
                    newStar.star[1] = size + 1;
                    newStar.line[0] = newLine.index - 1;
                    newStar.line[1] = newLine.index;
                } else {
                    newStar.star[1] = size + 1;
                    newStar.line[1] = newLine.index;
                }
                
                OCRPoint &p1 = lineData[extrem[j + 1]];
                p1.type = P_POINT;
                
                // исправляем конец линии
                newLine.end = p1;
                newLine.endID = newStar.star[1];
                // исправляем длину линии
                newLine.lenA = extrem[j + 1] - extrem[j];
                newLine.lenE0 = newLine.lenA;
                newLine.lenE1 = newLine.lenA;
                newLine.length = sqrt((newLine.start.x - newLine.end.x) *
                                      (newLine.start.x - newLine.end.x) +
                                      (newLine.start.y - newLine.end.y) *
                                      (newLine.start.y - newLine.end.y));
                
                //переписываем точки экстремума линии
                for (int m = 0; m < extremCount; m++) {
                    if (line.extrem[m] < extrem[j + 1] && line.extrem[m] > extrem[j]) {
                        newLine.extrem[newLine.extremCount] = line.extrem[m] - extrem[j];
                        newLine.extremCount++;
                    }
                }
            }
            
            //для последней точки экстремума
            if (j == extremCount - 1) {
                // исправляем OCRStar конца линии
                //по неизвестной причине приходится еще раз брать ссылку на endStar для
                //записи (иначе в случае двух экстремумов линии запись не происходит)
                //для чтения ссылка предоставляет верные данные.
                OCRStar &endStar_ = starArray[endStar.index];
                
                for (int m = 0; m < endStar.lineCount; m++) {
                    if (endStar.line[m] == line.index) {
                        endStar_.star[m] = newStar.index;
                        endStar_.line[m] = newLine.index;
                        break;
                    }
                }
                
                // исправляем конец линии
                newLine.end = endStar.cPoint;
                newLine.endID = endID;
                // исправляем длину линии
                newLine.lenA = lenA - extrem[j];
                newLine.lenE0 = newLine.lenA;
                newLine.lenE1 = newLine.lenA;
                newLine.length = sqrt((newLine.start.x - newLine.end.x) *
                                      (newLine.start.x - newLine.end.x) +
                                      (newLine.start.y - newLine.end.y) *
                                      (newLine.start.y - newLine.end.y));
                
                if (extremCount > 1) {
                    newStar.star[0] = newStar.index - 1;
                    newStar.line[0] = newLine.index - 1;
                } else {
                    newStar.star[0] = startStar.index;
                    newStar.line[0] = line.index;
                }
                newStar.star[1] = endID;
                newStar.line[1] = newLine.index;
                
                //переписываем точки экстремума линии
                for (int m = 0; m < extremCount; m++) {
                    if (line.extrem[m] > extrem[j]) {
                        newLine.extrem[newLine.extremCount] = line.extrem[m] - extrem[j];
                        newLine.extremCount++;
                    }
                }
            }
            line.node[line.nodeCount] = newLine.index;
            line.nodeCount++;
            
            //записываем новую OCRLine
            focalLine.push_back(newLine);
            //записываем новую OCRStar
            starArray.push_back(newStar);
        }
    }
    
    // printSVG("/_Image2OCR/_2Draw_text_1.svg");
    
} //_________________________________________________________________________________

/** @bref Функция создания хеш регистра углов фокальных и опорных линий  OCRStar
 */
void GGraph::buildHashRegister(OCRStar &star, int mode) {
    /**
     Функция создает поисковый и базовый регистры для вычисления корреляции углов
     фокальных и опорных линий OCRStar. Детальное описание в @fn
     GGraphOCR::OCRStarCorrelationLookup1 в режиме 1 заполняем регистры фокальных
     линий и регистр базовых линий. в режиме 2 заполняем толлько поисковый регистр
     опорных линий. Это необходимо потому что в режиме 2 мы выполняем поиск опорных
     линий в меньшем габарите поиска. Это позволяет надежно находить соответствие
     двух OCRStar которые могут имет различную длину фокальных линий окружения.
     Поисковый и базовый регистры для вычисления корреляции углов
     фокальных линий создаются в двух режимах - с учётом и без учёта коротких линий.
     */
    OCRRegister &reg = star.reg;
    reg.type = star.type;
    uint64 &rg0_0 = reg.reg0_0; //поисковый регистр углов фокальных линий с учётом
    //коротких линий
    uint64 &rg0_1 = reg.reg0_1; //регистр маски лимитов углов фокальных линий с
    //учётом коротких линий
    uint64 &rg1_0 = reg.reg1_0; //поисковый регистр углов фокальных линий без
    //учёта коротких линий
    uint64 &rg1_1 = reg.reg1_1; //регистр маски лимитов углов фокальных линий без
    //учёта коротких линий
    //uint64 &rg2_0 = reg.reg2_0; //поисковый регистр углов опорных линий
    //uint64 &rg2_1 = reg.reg2_1; //регистр маски лимитов углов опорных линий
    
    int s_rg = 8 * sizeof(ulong); // длина регистра в битах // 64
    float k = (float)64 / 360;
    
    if (mode == 1) {
        //заполняем поисковый регистр фокальных линий с учётом коротких линий
        rg0_0 = 0;
        for (int i = 0; i < star.lineCount; i++) {
            rg0_0 |= ((uint64)1) << (int)(star.angle[i] * k);
        }
        reg.nReg0 = star.lineCount;
        // binaryPrint64(rg0_0, 2); cout<<endl;
        
        // Изготовление эталонного хеш регистра (маски) "rg0_1". Области проверки
        // получаем с помощью растискивания.
        
        // Растискивание на 3 бита вправо и 3 бита влево, с циклическим сдвигом. 5
        // подряд идущих единиц.
        rg0_1 = rg0_0;
        // rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1
        // бит влево (*). rg0_1=rg1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     //
        // Растискивание на 2 бит влево (*). rg0_1=rg0_1 | ((rg0_1>>2) |
        // (rg0_1<<(s_rg-2)));     // Растискивание на 3 бит вправо (/).
        
        // Растискивание на 3 бита вправо и 3 бита влево, с циклическим сдвигом. 7
        // подряд идущих единиц.
        // rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1
        // бит влево (*). rg0_1=rg0_1 | ((rg0_1<<2) | (rg0_1>>(s_rg-2)));     //
        // Растискивание на 2 бит влево (*). rg0_1=rg0_1 | ((rg0_1>>3) |
        // (rg0_1<<(s_rg-3)));     // Растискивание на 3 бит вправо (/).
        
        // Растискивание на 4 бита вправо и 4 бита влево, с циклическим сдвигом. 11
        // подряд идущих единиц.
        //rg0_1 = rg0_1 | ((rg0_1 << 1) | (rg0_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        //rg0_1 = rg0_1 | ((rg0_1 << 1) | (rg0_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        //rg0_1 = rg0_1 | ((rg0_1 << 3) | (rg0_1 >> (s_rg - 3))); // Растискивание на 3 бит влево (*).
        //rg0_1 = rg0_1 | ((rg0_1 >> 5) | (rg0_1 << (s_rg - 5))); // Растискивание на 5 бит вправо (/).
        
        if(star.lineCount>1){
            // Растискивание на 6 бита вправо и 6  бита влево, с циклическим сдвигом. 13 подряд идущих единиц.
            rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1<<2) | (rg0_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1<<3) | (rg0_1>>(s_rg-3)));     // Растискивание на 3 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1>>6) | (rg0_1<<(s_rg-6)));     // Растискивание на 6 бит вправо (/).
        }else{
            // Для  "L" точек;
            // Растискивание на 8 бит вправо и 8  бита влево, с циклическим сдвигом. 17 подряд идущих единиц. 360/64*17=95,6/2=47,8 +/- градусов
            rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1<<2) | (rg0_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1<<4) | (rg0_1>>(s_rg-4)));     // Растискивание на 4 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg0_1=rg0_1 | ((rg0_1>>8) | (rg0_1<<(s_rg-8)));     // Растискивание на 8 бит вправо (/).

            // Для  "L" точек;
            // Растискивание на 15 бит вправо и 15  бита влево, с циклическим сдвигом. 31 подряд идущих единиц. 360/64*17=95,6/2=47,8 +/- градусов
            //rg0_1=rg0_1 | ((rg0_1<<1) | (rg0_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            //rg0_1=rg0_1 | ((rg0_1<<2) | (rg0_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            //rg0_1=rg0_1 | ((rg0_1<<4) | (rg0_1>>(s_rg-4)));     // Растискивание на 4 бит влево (*).
            //rg0_1=rg0_1 | ((rg0_1<<8) | (rg0_1>>(s_rg-8)));     // Растискивание на 8 бит влево (*).
            //rg0_1=rg0_1 | ((rg0_1>>15) | (rg0_1<<(s_rg-15)));   // Растискивание на 15 бит вправо (/).
            
        }
        
        //binaryPrint64(rg0_1, 2); cout<<endl;
        
        //заполняем поисковый регистр фокальных линий без учёта коротких линий
        rg1_0 = 0;
        int count = 0;
        for (int i = 0; i < star.lineCount; i++) {
            if (focalLine[star.line[i]].lenA < star.lineW * SERIF_LIMIT)
                continue; //линия слишком короткая
            rg1_0 |= ((uint64)1) << (int)(star.angle[i] * k);
            count++;
        }
        reg.nReg1 = count;
        // binaryPrint64(rg1_0, 2); cout<<endl;
        rg1_1 = rg1_0;
        // Растискивание на 4 бита вправо и 4 бита влево, с циклическим сдвигом. 11
        // подряд идущих единиц.
        //rg1_1 = rg1_1 | ((rg1_1 << 1) | (rg1_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        //rg1_1 = rg1_1 | ((rg1_1 << 1) | (rg1_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        //rg1_1 = rg1_1 | ((rg1_1 << 3) | (rg1_1 >> (s_rg - 3))); // Растискивание на 3 бит влево (*).
        //rg1_1 = rg1_1 | ((rg1_1 >> 5) | (rg1_1 << (s_rg - 5))); // Растискивание на 5 бит вправо (/).
        
        if(count>1){
            // Растискивание на 6 бита вправо и 6  бита влево, с циклическим сдвигом. 13 подряд идущих единиц.
            rg1_1=rg1_1 | ((rg1_1<<1) | (rg1_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1<<2) | (rg1_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1<<3) | (rg1_1>>(s_rg-3)));     // Растискивание на 3 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1>>6) | (rg1_1<<(s_rg-6)));     // Растискивание на 6 бит вправо (/).
        
        }else{
            // Для  "L" точек;
            // Растискивание на 15 бит вправо и 15  бита влево, с циклическим сдвигом. 31 подряд идущих единиц. 360/64*17=95,6/2=47,8 +/- градусов
            //rg1_1=rg1_1 | ((rg1_1<<1) | (rg1_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            //rg1_1=rg1_1 | ((rg1_1<<2) | (rg1_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            //rg1_1=rg1_1 | ((rg1_1<<4) | (rg1_1>>(s_rg-4)));     // Растискивание на 4 бит влево (*).
            //rg1_1=rg1_1 | ((rg1_1<<8) | (rg1_1>>(s_rg-8)));     // Растискивание на 8 бит влево (*).
            //rg1_1=rg1_1 | ((rg1_1>>15) | (rg1_1<<(s_rg-15)));   // Растискивание на 15 бит вправо (/).

            // Для  "L" точек;
            // Растискивание на 8 бит вправо и 8  бита влево, с циклическим сдвигом. 17 подряд идущих единиц. 360/64*17=95,6/2=47,8 +/- градусов
            rg1_1=rg1_1 | ((rg1_1<<1) | (rg1_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1<<2) | (rg1_1>>(s_rg-2)));     // Растискивание на 2 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1<<4) | (rg1_1>>(s_rg-4)));     // Растискивание на 4 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1<<1) | (rg1_1>>(s_rg-1)));     // Растискивание на 1 бит влево (*).
            rg1_1=rg1_1 | ((rg1_1>>8) | (rg1_1<<(s_rg-8)));     // Растискивание на 8 бит вправо (/).
        }
         
        
    }
    
     //binaryPrint64(rg1_1, 2); cout<<endl;
    
    /*
    //заполняем поисковый регистр опорных линий
    rg2_0 = 0;
    for (int i = 0; i < star.baseCount; i++) {
        rg2_0 |= ((uint64)1) << (int)star.point[star.pointCount + i].type; //значение угла опорной линии записано в переменной  type
        // cout<<(int)star.point[star.pointCount + i].type<<endl;
    }
    reg.nReg2 = star.baseCount;
    
    // binaryPrint64(rg2, 2); cout<<endl;
    if (mode == 2 && star.baseCount) {
        // Изготовление эталонного хеш регистра (маски) "rg1". Области проверки
        // получаем с помощью растискивания. Растискивание на 2 бита вправо и 2 бита
        // влево, с циклическим сдвигом.
        rg2_1 = rg2_0;
        // Растискивание на 4 бита вправо и 4 бита влево, с циклическим сдвигом. 11
        // подряд идущих единиц.
        rg2_1 = rg2_1 | ((rg2_1 << 1) | (rg2_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        rg2_1 = rg2_1 | ((rg2_1 << 1) | (rg2_1 >> (s_rg - 1))); // Растискивание на 1 бит влево (*).
        rg2_1 = rg2_1 | ((rg2_1 << 3) | (rg2_1 >> (s_rg - 3))); // Растискивание на 3 бит влево (*).
        rg2_1 = rg2_1 | ((rg2_1 >> 5) | (rg2_1 << (s_rg - 5))); // Растискивание на 5 бит вправо (/)
    }
    */
    // binaryPrint64(rg3, 2); cout<<endl;
    
    // Проверка совпадения эталонного хеш регистра "rg0" и проверяемого хеш
    // регистра "rg0_0". // ulong old_rg0_0=rg0_0; Пересечение регистров
    // rg0_0=rg0_0 & rg0_1;
    
    // Вычисление коэффициент корреляции. //
    // Если после пересечение регистров значение проверяемого хеш регистра "rg0_1"
    // не изменилось, то коэффициент корреляции равен 100%.
    // int cor=0;
    // if(rg0_0==old_rg0_0) {
    //    cor=100;
    //    cout<<"rg0_0="<<bitset<64>(rg0_0)<<"
    //    old_rg0_0="<<bitset<64>(old_rg0_0)<<"   cor="<<cor<<endl;
    //}
    // else {} // Подсчитываем количество единиц в регистре "rg1"
    
    // Генри Уоррен, мл. - Алгоритмические трюки для программистов 2014.pdf
    // 2.15. Циклический сдвиг стр 59
    
} //____________________________________________________________

/** @bref распознавание опорных линий OCRStar*/
void GGraph::bearingLineRecognition(OCRStar &star, int mode) {
    /**
     Опорные линии это лучи проведенные от центра OCRStar до
     центров других OCRStar которые не соединены с данной OCRStar
     фокальными линиями. Опорные линии проводятся к OCRStar окружения
     на расстоянии не более 1.5 средней длины OCRStar.
     Опорные линии помогают сформировать устойчивые кластеры OCRStar
     и повышают индивидуальность OCRStar.
     Использование опорных линий для корреляции описано
     в @fn GGraphOCR::OCRStarCorrelationLookup1
     
     Для построения опорных линий необходимо найти все точки которые находятся
     на заданном расстоянии от искомой точки. Выполняем этот поиск по
     вертикально транспонированному массиву уменьшенному в два раза по сравнению с
     исходным. Это увеличивает быстродействие в 4 раза.
     */
    int print = 0;
    int w1 = ncolumns / 2;
    int h1 = nrows / 2;
    if (w1 % 32 != 0)
        w1 = (w1 / 32 + 1) *
        32; //расширяем изображение если уменьшенное изображение обрезается
    
    //создание поисковых регистров, масок концов лучей и хеша OCRStar
    // rad коэффициент для перевода угла из радиан в градусы
    float rad = 57.2958; //  rad=180.0/pi=57.2958; float pi=3.14159265;
    
    if (star.type == L_POINT)
        return;
    OCRPoint adrFocalPoint[64];
    OCRPoint &sP = star.cPoint;
    int index = star.index;
    float limitMode;
    if (mode == 1) {
        limitMode = 1.5;
    } else {
        limitMode = 3;
    }
    int limit = star.mLen * limitMode;
    if (limit < 2)
        return;
    //находим все фокальные точки в окружении OCRStar
    int sizeAdr =
    detectFocalPoint(sP.x / 2 - limit / 2, sP.y / 2 - limit / 2, limit, limit,
                     w1, h1, bites_data_32_2, adrFocalPoint);
    // cout<<t<<" "<<sizeAdr<<" p:"<<(int)star.pointCount<<endl;
    
    //записываем в найденные точки истинные значения координат
    for (int i = 0; i < sizeAdr; i++) {
        OCRPoint &p = adrFocalPoint[i];
        p.status = 1;
        p.type = 0;
        //находим ID номер фокальной точки и ее исходные координаты
        int id = id_data2[p.y * w1 + p.x] - 1; // ID номера в id_data2 больше на 1
        DR("mLen" << (int)star.mLen << "x:" << p.x << " y:" << p.y << " id:" << id
           << endl);
        
        OCRStar &dStar = starArray[id];
        p.x = dStar.cPoint.x -
        star.cPoint
        .x; //координаты в массиве star.point относительно центра OCRStar
        p.y = dStar.cPoint.y - star.cPoint.y;
        //записываем в параметр data индекс OCRStar на конце опорной линии
        p.data = id;
        
        if (p.x == sP.x && p.y == sP.y) {
            p.status = 0;
            continue;
        }
        //проверяем тип фокальной линии
        if (index == id || dStar.type == L_POINT) {
            p.status = 0;
            continue; // dStar.minLen<LINE_SIZE
        }
        
        //проверяем есть ли фокальная линия к этой точке
        int flag = 0;
        for (int n = 0; n < star.pointCount; n++) {
            OCRPoint &p1 = star.point[n];
            if (p.x == p1.x && p.y == p1.y) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            p.status = 0;
            continue;
        }
        //между двумя точками нет фокальной линии
        //вычисляем расстояние между точками
        // p.data=sqrt((float)((p.x - sP.x) * (p.x - sP.x) + (p.y - sP.y) * (p.y -
        // sP.y)));
    }
    // continue;
    
    star.baseCount = 0;
    for (int i = 0; i < sizeAdr; i++) {
        OCRPoint &p = adrFocalPoint[i];
        if (!p.status)
            continue;
        //создаем опорную линию
        OCRPoint &pLine = star.point[star.pointCount + star.baseCount];
        pLine = p;
        //в параметр type записываем угол опорной линии приведенный к диапазону 0-64
        //вычисление угла линии в градусах
        //угол отсчитываем от горизонтальной оси против часовой стрелки
        short dx = p.x;
        short dy = p.y; // координаты конечной точки относительно центра звездочки.
        
        float alpha =
        rad *
        atan2f(
               (float)dy,
               (float)
               dx); // стандартная область определнеия atan2f  -pi<=atan2f<=pi
        // (f-float) получение области определнеия atan2f от 0 до
        // 360 градусов (от 0 до 2*pi)
        if (alpha < 0)
            alpha += 360.0; // переопределение atan2f для отрицательных углов
        // pi<=atan2f+2*pi<=2*pi
        /// star.mA[i]=(ushort)mAf;
        
        //записываем значение угла
        pLine.type = (int)((alpha * 64) / 360.0);
        star.baseCount++;
        if (star.baseCount + star.pointCount > 11)
            break;
    }
    
} //_____________________________________________________________


/** @bref Функция создания кластера OCRStar */
void GGraph::buildCluster(OCRStar &star) {
    
    
    OCRCluster cluster;
    cluster.sIndex = star.index;
    cluster.cPoint = star.cPoint;
    cluster.cPointText = star.cPoint;
    cluster.level=0;
    cluster.status=0;
    cluster.corr=0;
    cluster.type=star.type;
    cluster.index = (uint)clusterArray.size();
    star.clusterIndex=cluster.index;
    
    if(star.type==Z_POINT){
        //cluster.maskC.type = 0;
        //cluster.maskC.ID = cluster.index;
        //cluster.maskC.regCount=0;
        cluster.starCount=1;
        cluster.lineCount=0;
        star.lineCount=0;
        cluster.star[0] = star;
        cluster.xMin=0;
        cluster.xMax=0;
        cluster.yMin=0;
        cluster.yMax=0;
        cluster.cPoint=star.cPoint;
        clusterArray.push_back(cluster);
        return;
    }
    
    //Записываем в массив mPoint координаты OCRStar кластера и координаты середин
    //линий
    OCRPoint mPoint[16];
    OCRPoint lPoint[64];
    OCRLine *cLine = cluster.line;
    uchar lCount = 0;
    uchar pCount = 0;
    int starCount=cluster.starCount;
    starCount=0;
    int lineCount=cluster.lineCount;
    lineCount=0;
    int xCenter=star.cPoint.x;
    int yCenter=star.cPoint.y;
    cluster.cPoint = star.cPoint;
    
    //записываем в кластер данные первой OCRStar
    mPoint[0] = star.cPoint;
    lPoint[0] = star.cPoint;
    cluster.star[0] = star;
    cluster.radius=star.radius;
    starCount++;
    pCount++;
    
    
    for (int n = 0; n < star.lineCount; n++) {
        OCRLine &line = focalLine[star.line[n]];
        line.status = 0;
        cLine[lineCount] = line;
        cLine[lineCount].parentID=star.line[n]; //в линии кластера parentID сохраняет индекс линии в графе
        if(line.cType){
            memcpy((char*)(lPoint+lCount),(char*)&line.pointC[1],4 * sizeof(OCRPoint));
            lCount+=4;
        }
        lineCount++;
    }
    if(star.type != O_POINT){
        //записываем данные присоединенных OCRStar
        for (int i = 0; i < star.starCount; i++) {
            cluster.star[starCount] = starArray[star.star[i]];
            OCRStar &star_ = cluster.star[starCount];
            star_.clusterIndex=cluster.index;
            mPoint[pCount] = star_.cPoint;
            
            starCount++;
            pCount++;
            //записываем линии присоединенных OCRStar
            for (int n = 0; n < star_.lineCount; n++) {
                OCRLine &line = focalLine[star_.line[n]];
                if(line.startID == star.index || line.endID == star.index) continue;
                line.status = 0;
                cLine[lineCount] = line;
                cLine[lineCount].parentID=star_.line[n]; //в линии кластера parentID сохраняет индекс линии в графе
                if(line.cType){
                    memcpy((char*)(lPoint+lCount),(char*)&line.pointC[1],4 * sizeof(OCRPoint));
                    lCount+=4;
                }
                //для тех лучей которые ведут к OCRStar вне кластера, меняем тип OCRLine
                cLine[lineCount].type = CLUSTER_LINE;
                lineCount++;
            }
            
            //записываем точки концов лучей присоединенных OCRStar
            for (int n = 0; n < star_.starCount; n++) {
                if (star_.star[n] == star.index){
                    star_.star[0] = star.index;
                    continue;
                }
                OCRStar &star__ = starArray[star_.star[n]];
                mPoint[pCount] = star__.cPoint;
                pCount++;
            }
            star_.starCount = 1;
        }
    }
    cluster.starCount = starCount;
    cluster.lineCount = lineCount;
    /*
     if(cluster.index==2){
     for (int n = 0; n < cluster.lineCount; n++) {
     OCRLine &line = cluster.line[n];
     cout<<" index:"<<line.index<<endl;
     }exit(0);
     }
     */
    
    //draw.printCluster((void*)&cluster);
    
    //Находим минимум и максимум
    int xMin = 100000;
    int xMax = -100000;
    int yMin = 100000;
    int yMax = -100000;
    for (int n = 0; n < pCount; n++) {
        OCRPoint &c = mPoint[n];
        //cout<<"x_:"<<c.x<<" y_:"<<c.y<<endl;
        if (c.x > xMax)
            xMax = c.x;
        if (c.x < xMin)
            xMin = c.x;
        if (c.y > yMax)
            yMax = c.y;
        if (c.y < yMin)
            yMin = c.y;
    }
    for (int n = 0; n < lCount; n++) {
        OCRPoint &c = lPoint[n];
        //cout<<"  x_:"<<c.x<<" y_:"<<c.y<<endl;
        if (c.x > xMax)
            xMax = c.x;
        if (c.x < xMin)
            xMin = c.x;
        if (c.y > yMax)
            yMax = c.y;
        if (c.y < yMin)
            yMin = c.y;
    }
    cluster.xMin=xMin;
    cluster.yMin=yMin;
    cluster.xMax=xMax;
    cluster.yMax=yMax;
    
    //размеры габарита кластера
    uint &wCluster = cluster.w;
    wCluster = xMax - xMin;
    uint &hCluster = cluster.h;
    hCluster = yMax - yMin;
    float &scale = cluster.scale32;
    int maskW = 23;
    // maskW - нормированный размер габарита кластера меньше 32 для того чтобы
    // оставить место для растискивания
    if (wCluster > hCluster) {
        if(wCluster > cluster.line[0].lineW){
            scale = (float)maskW / wCluster;
        } else {
            scale = 0;
        }
    } else {
        if(hCluster > cluster.line[0].lineW){
            scale = (float)maskW / hCluster;
        } else {
            scale = 0;
        }
    }
    /*
     if(cluster.index==2){
     draw.printCluster((void*)&cluster);
     //	exit(0);
     }
     */
    
    //переписываем все связи в кластере так, чтобы они адресовали элементы внутри кластера
    int dlt=MAX_CLUSTER; //временное смещение индексов для переадресации
    //переадресуем OCRStar кластера
    //также для последующего определения масштаба подсчитываем cluster.sumLen1 - сумма всех линий и переметра каждой OCRStar
    int indexStar=0;
    cluster.sumLen1=0;
    cluster.sumLenA=0;
    for (int i = 0; i < cluster.starCount; i++) {
        OCRStar &star_ = cluster.star[i];
        cluster.sumLen1 += star_.sumLen1;
        cluster.sumLenA +=star_.sumLenA;
        int sIndex=star_.index;
        star_.index = indexStar + dlt;
        //cout<<endl<<" sIndex:"<<sIndex<<" star_.type:"<<(int)star_.type<<" starCount:"<<(int)star_.starCount<<endl;
        
        for (int n = 0; n < cluster.starCount; n++) {
            if(n==i)continue;
            OCRStar &star__ = cluster.star[n];
            for (int m = 0; m < star__.starCount; m++) {
                //	cout<<"         star__.star["<<m<<"]:"<<(int)star__.star[m]<<endl;
                if(star__.star[m]==sIndex){
                    star__.star[m]=indexStar+dlt;
                }
            }
        }
        
        //переадресуем связи OCRLine к OCRStar кластера
        for (int n = 0; n < cluster.lineCount; n++) {
            OCRLine &line = cluster.line[n];
            if(line.startID==sIndex)line.startID = indexStar+dlt;
            if(line.endID==sIndex)line.endID = indexStar+dlt;
        }
        
        indexStar++;
    }
    //переадресуем связи OCRStar к OCRLine кластера
    uint indexLine=0;
    for (int i = 0; i < cluster.lineCount; i++) {
        OCRLine &line = cluster.line[i];
        int lIndex = line.index;
        line.match = lIndex; //записываем индекс исходной линии в GGraph
        line.index	= indexLine +dlt;
        //cout<<"//_____ lIndex:"<<lIndex<<endl;
        
        for (int n = 0; n < cluster.starCount; n++) {
            OCRStar &star_ = cluster.star[n];
            //cout<<"   //_____ n:"<<n <<" index:"<<star_.index<< " lineCount:"<<(int)star_.lineCount<<endl;
            
            for (int m = 0; m < star_.lineCount; m++) {
                //cout<<"           m:"<<m<<" line:"<<(int)star_.line[m]<<endl;
                if(star_.line[m]==lIndex){
                    star_.line[m]=indexLine+dlt;
                }
            }
        }
        indexLine++;
    }
    
    //вычитаем величину временного смещения из индексов
    for (int i = 0; i < cluster.starCount; i++) {
        OCRStar &star_ = cluster.star[i];
        star_.index -= dlt;
        for (int n = 0; n < star_.starCount; n++) {
            star_.star[n] -= dlt;
        }
        for (int n = 0; n < star_.lineCount; n++) {
            star_.line[n] -= dlt;
        }
    }
    
    for (int i = 0; i < cluster.lineCount; i++) {
        OCRLine &line = cluster.line[i];
        line.index -= dlt;
        line.startID -= dlt;
        line.endID -= dlt;
    }
    
    //перемещаем начало координат внутри кластера в центр первой OCRStar кластера
    cluster.xMin -= xCenter;
    cluster.xMax -= xCenter;
    cluster.yMin -= yCenter;
    cluster.yMax -= yCenter;
    
    for (int i = 0; i < cluster.starCount; i++) {
        OCRStar &star_ = cluster.star[i];
        star_.cPoint.x -= xCenter;
        star_.cPoint.y -= yCenter;
        star_.xMin -= xCenter;
        star_.xMax -= xCenter;
        star_.yMin -= yCenter;
        star_.yMax -= yCenter;
    }
    for (int i = 0; i < cluster.lineCount; i++) {
        OCRLine &line = cluster.line[i];
        line.start.x -= xCenter;
        line.start.y -= yCenter;
        line.end.x -= xCenter;
        line.end.y -= yCenter;
        for(int n=0;n<5;n++){
            line.pointC[n].x-=xCenter;
            line.pointC[n].y-=yCenter;
        }
    }
    /*
    //создаем маску и массив регистров кластера
    if(scale){
        cluster.maskC.type = cluster.type;
        cluster.maskC.ID = cluster.index;
        //if (cluster.type != O_POINT){
        if (0){
            // 9 pixel  wide mask
            uint maskC[] = {0b0000000001111100, 0b0000000011111110, 0b0000000111111111,
                0b0000000111111111, 0b0000000111111111, 0b0000000111111111,
                0b0000000111111111, 0b0000000011111110, 0b0000000001111100};
            
            // cout<<"@@@ star:"<<star.index<<" scale:"<<scale<<" wCluster:"<<wCluster<<"
            // hCluster:"<<hCluster<<" pCount:"<<pCount<<endl;
            OCRMask &maskC_=cluster.maskC;
            int *p = maskC_.mask;
            memset((char *)p, 0, 32 * 4);
            if (pCount > 32)
                pCount = 32;
            maskC_.regCount = pCount;
            for (int n = 0; n < pCount; n++) {
                //создаем область поиска в маске кластера
                OCRPoint &c = mPoint[n];
                int x = (c.x - xMin) * scale + 4; //
                int y = (c.y - yMin) * scale + 4;
                // cout<<"x:"<<x<<" y:"<<y<<endl;
                for (int i = 0; i < 9; i++) {
                    *(p + y + i - 4) |= maskC[i] << (x - 4);
                }
                //создаем поисковый регистр
                maskC_.reg[n] = 1 << x;
                maskC_.regY[n] = y;
            }
            //draw.printOCRMask(cluster.maskC.mask);
            
        }
    }else{
        cluster.maskC.type = 0;
        cluster.maskC.ID = cluster.index;
        cluster.maskC.regCount=0;
    }
    */
    
    //cluster test and draw
    //for (int i = 0; i < cluster.starCount; i++) {
    //OCRStar &star = cluster.star[i];
    //draw.printStar((void*)&star);
    //draw.printOCRMask(star.maskC.mask);
    //printOCRStarSVG((void*)cluster.line,(void*)&star);
    //printOCRStarBaseLine((void*)&star);
    //}
    //for (int i = 0; i < cluster.lineCount; i++) {
    //	draw.printLine((void *)&cluster.line[i]);
    //}
    /*
     for (int i = 0; i < cluster.starCount; i++) {
     OCRStar &star_ = cluster.star[i];
     draw.printClusterStar((void*)&cluster,(void*)&star_); 
     }
     */
    //draw.printCluster((void*)&cluster); exit(0);
    //draw.printClusterSVG((void *)&cluster, "/OSBL/CORRELATION/");
    //exit(0);

    clusterArray.push_back(cluster);
    //OCRCluster &cluster_=clusterArray[1];
    //cout<<1;
    
}//_________________________________________________________________________

/** @brief  Определение величины и знака выгиба кривой (вектора) для всех OCRLine в GGraph */
void GGraph::setCorrelationCurve() {
    uint size=(uint)focalLine.size();
    
        // цикл перебора фокальных ЛИНИЙ на странице.
    for (int m = 0; m < size; m++) { // size
        OCRLine &line = focalLine[m];
        int lenA = line.lenA;
        int len = lenA - 1;
        line.SumBitOn = 0;
        if (lenA < 3 || line.lineW < 1 || lenA < line.lineW * SERIF_LIMIT){
            line.cType = 0;
            continue;
        }
        line.cType = 1;
        //определяем минимум и максимум координат линии
        OCRPoint *pData = (OCRPoint *)&pointArray[line.pointIndex];
        int xMin = 100000;
        int xMax = 0;
        int yMin = 100000;
        int yMax = 0;
        for (int n = 0; n < lenA; n++) {
            OCRPoint &c = pData[n];
            // cout<<"x:"<<c.x<<" y:"<<c.y<<endl;
            if (c.x > xMax)
                xMax = c.x;
            if (c.x < xMin)
                xMin = c.x;
            if (c.y > yMax)
                yMax = c.y;
            if (c.y < yMin)
                yMin = c.y;
        }
        line.xMax = xMax;
        line.xMin = xMin;
        line.yMax = yMax;
        line.yMin = yMin;
        //размеры габарита линии
        int wLine = line.xMax - line.xMin;
        int hLine = line.yMax - line.yMin;
        float scale;
        int lineW = 23;
        
        // lineW - нормированный размер габарита линии меньше 32 для того чтобы
        // оставить место для растискивания точек линии
        if (wLine > hLine) {
            scale = (float)lineW / wLine;
        } else {
            scale = (float)lineW / hLine;
        }
        line.scale0 = scale;
        
        //создаем регистры проверки точек линии по маске 32х32
        int dl = 4 / scale;
        OCRPoint &p0 = pData[0 + dl];
        p0.status = 15;
        line.pointC[0] = p0; //сохраняем исходные координаты конторольной точки
        OCRPoint &p1 = pData[len / 3];
        p1.status = 20;
        line.pointC[1] = p1; //сохраняем исходные координаты конторольной точки
        OCRPoint &p2 = pData[len / 2];
        p2.status = 30;
        line.pointC[2] = p2; //сохраняем исходные координаты конторольной точки
        OCRPoint &p3 = pData[len - len / 3];
        p3.status = 20;
        line.pointC[3] = p3; //сохраняем исходные координаты конторольной точки
        OCRPoint &p4 = pData[len - dl];
        p4.status = 15;
        line.pointC[4] = p4; //сохраняем исходные координаты конторольной точки
    }

    
    
    // OCRPoint p0 	координаты общей точки начала векторов (центр звездочки).
    // OCRPoint p1 	координаты точки конца первого вектора (опорный вектор кривой).
    // OCRPoint p2 	координаты точки конца второго вектора (середина кривой).
    
    
    //определяем тип линии и устанавливаем значения углов контрольных точек
    for(int i=0;i<size;i++){
        OCRLine &line=focalLine[i];
        if (!line.cType){
            line.alphaMid=0;
            line.alpha1=0;
            line.alpha2=0;
            line.type=1;
            continue;
        }
       
        //проверяем выгиб середины OCRLine
        line.alphaMid=CorrelationCurve(line.start,line.pointC[4], line.pointC[2]);
        //проверяем выгиб первой трети OCRLine
        line.alpha1=CorrelationCurve(line.start,line.pointC[2], line.pointC[1]);
        //проверяем выгиб точки 2/3 относительно конца OCRLine
        line.alpha2=CorrelationCurve(line.pointC[2],line.pointC[4],line.pointC[3]);
        //определяем тип линии
        if(line.lenA < line.lineW * SERIF_LIMIT * 2){
            //короткие линии считаем прямыми
            line.cType = LINE;
        }else if(abs(line.alphaMid) < MAX_CURVE && abs(line.alpha1) < MAX_CURVE && abs(line.alpha2) < MAX_CURVE){
            line.cType = LINE;
        }else if(abs(line.alpha1) < MAX_CURVE * 1.5 && abs(line.alpha2) < MAX_CURVE * 1.5 && abs(line.alpha1+line.alpha2) < MAX_CURVE * 1.5 && line.alphaMid < MAX_CURVE){
            //также прямыми считаем S-кривые с небольшой кривизной
            line.cType = L_CURVE;
        }else if(abs(line.alphaMid) > MAX_CURVE && line.alphaMid < 0 && line.alpha1 < 0 && line.alpha2 < 0){
            line.cType = L_CURVE;
        }else if(line.alphaMid > MAX_CURVE && line.alphaMid > 0 && line.alpha1 > 0 && line.alpha2 > 0){
            line.cType = L_CURVE;
        }else if(line.alpha1 < 0 && line.alpha2 > 0 ){
            line.cType = SL_CURVE;
        }else if(line.alpha1 > 0 && line.alpha2 < 0 ){
            line.cType = SR_CURVE;
        }else{
            line.cType = N_CURVE;
        }

        
    }
}

/** @bref  Определение величины и знака выгиба кривой (вектора). */
float GGraph::CorrelationCurve(OCRPoint &p0, OCRPoint &p1, OCRPoint &p2) {
    // Основная рабочая. 24 апреля 2021.
    // Определение величины и знака выгиба кривой (вектора).
    // Выгиб кривой пропорционален углу между векторами проведенным от центра звездочки к концу кривой и к середине кривой.
    // Алгоритм выполняется предпросчетом и является масштаб-независимым.
    
    // Функция получает на вход:
    // int p0_x, p0_y 	координаты общей точки начала векторов (центр звездочки).
    // int p1_x, p1_y 	координаты точки конца первого вектора (опорный вектор кривой).
    // int p2_x, p2_y 	координаты точки конца второго вектора (середина кривой).
    // Функция возвращает:
    // float Alpha 		угол в градусах между векторами проведенным от центра звездочки к концу кривой и к середине кривой.
    // Модули векторов (длинны векторов a и б) mod1, mod2 и их проекции на оси dx1, dy1, dx2, dy2 м.б. уже вычисленны.
    
    /**
     Теория "Угол между векторами с учетом знака":
     Скалярное произведение не различает порядок векторов и дает только абсолютное значение угла.
     Векторное произведение дает абсолютное значение угла и его знак.
     Синус угла (a,b) между векторами a=(ax,ay) и b=(bx,by) равен векторному произведению векторов a·b,
     поделенному на произведение модулей векторов |a|*|b|.
     sin(a,b)=a·b/|a|*|b|;
     Угол как меру кривизны с учетом знака вычисляем с помощью asin(sin(a,b).
     Подробнее.
     https://gamedev.ru/code/forum/?id=63684
     Развитие:
     Проверку всех лучей звездочки по углам кривизны можно одновременно выполнять выполнять с
     помощью одного очень быстрого 64р регистра.
     
     В том случае если необходимо более точно сравнивать формы кривых, угол лучей к промедуточным точкам 
     необходимо отсчитывать от луча проведенного к промежуточной точке сильно усредненной кривой.
     Для тогго чтобы провести луч к промежуточной точке усредненной кривой достоаточно усреднить 
     окружение промежуточной точки исходной кривой.
     Проведение луча к усредненной кривой равносильно проведению средней линии между всеми 
     экстремумами кривой.
     
     
     */
    
    int GrafT=0;
    // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT)  cout<<"// Определение величины и знака выгиба кривой (вектора)"<<endl;
    
    //читаем координаты как параметр функции
    
    int p0_x=p0.x;
    int p0_y=p0.y;
    int p1_x=p1.x;
    int p1_y=p1.y;
    int p2_x=p2.x;
    int p2_y=p2.y;
    
    
    if (GrafT) cout<<"p0_x: "<<p0_x<<" p0_y: "<<p0_y<<" \np1_x: "<<p1_x<<" p1_y: "<<p1_y<<" \np2_x: "<<p2_x<<" p2_y: "<<p2_y<<endl;
    
    // векторное произведение векторов sin(a,b)=a·b/|a|*|b|; //
    
    ///int dx1=p1.x-p0.x;  int dy1=p1.y-p0.y;
    ///int dx2=p2.x-p0.x;  int dy2=p2.y-p0.y;
    
    // Приведение длин векторов к началу координат
    int dx1=p1_x-p0_x;  int dy1=p1_y-p0_y;
    int dx2=p2_x-p0_x;  int dy2=p2_y-p0_y;
    
    // векторное произведение векторов заданных координатами концов:
    ///float vector = dx1*dy2 - dy1*dx2;
    
    // Вычисление модулей векторов (длинн векторов a и б) по Теореме Пифагора (м.б. уже вычисленно?).
    ///float mod1=sqrt(float(dx1*dx1 + dy1*dy1));
    ///float mod2=sqrt(float(dx2*dx2 + dy2*dy2));
    
    ///float sinAlpha=vector/(mod1*mod2);
    
    // sinAlpha в компактном виде
    float sinAlpha = (dx1*dy2 - dy1*dx2)/(sqrt(float(dx1*dx1 + dy1*dy1) * float(dx2*dx2 + dy2*dy2))); // УБРАН 1 sqrt
    
    // Вычисляем угол как меру кривизны с учетом знака.  
    float Alpha=57.29747*asin(sinAlpha);		// of [-π/2, π/2].
    if (GrafT) { cout<<"sinAlpha="<<sinAlpha<<"   Alpha="<<Alpha<<endl; }  
    
    // Возвращаем угол в градусах между векторами.
    return Alpha;
    
}



} // namespace ocr

