//
//  GraphOCR.cpp
//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .


#include "GGraphBezier.h" 
namespace ocr{
    
    //*****************************************************************************
    
    
    // Функция создания поисковых индексов для массивов фокальных линий и точек. Запускается в gPort.mm
    void GGraphBezier::setLineMatrix(vector<OCRPointFloat>&focalPoint,
                               vector<OCRFocalLineBezier> &focalLine,
                               GStr2D<int>*pointMatrix,
                               GStr2D<int>*lineMatrix,
                               uint dltX){
 
        int GrafT=1;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if(GrafT){
            cout<<endl<<"-----------------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция создания поисковых индексов для массивов фокальных линий и точек.   setLineMatrix() "<<endl;
        }
TIME_START
        
        uint indexV[ncolumns];
        memset((char*)indexV,0,ncolumns*4);// весь блок заполнен нулями
        
        /*
        uint size=(uint)focalPoint.size();    // Количество линий на странице.
        
        for(int i=0;i<size;i++){
            indexV[(int)focalPoint[i].x]++;  // сколько
        }
        pointMatrix.setIndex(indexV,ncolumns);
        
        for(int i=0;i<size;i++){
            int x=focalPoint[i].x;
            int s=pointMatrix[x][0];
            s++;
            pointMatrix[x][0]=s;
            pointMatrix[x][s]=i;
        }
        memset((char*)indexV,0,ncolumns*4);
        */
        
        //int dltX=3; // на сколько pix растискиваем точки линии start.x и end.x в обе стороны по "x". дб одинаково с setLineCombination
        int dltX_=dltX-1; // иначе 2*dltX получается не симметричным на 1 (для dltX=3; фактически 2*dltX=-3+2; )
        uint size=(uint)focalLine.size();
        
        // Для каждой фокальной линии на странице определяем, сколько начальных и конечных точек линий
        // попадает на одни и те же координаты по "x".
        for(int i=0; i<size; i++){ // Цикл перебора линий по всей странице.
            // Получаем координаты начальных и конечных точек фокальной линии по "x".
            ///int x1=focalLine[i].start.x;
            ///int x2=focalLine[i].end.x;
            int x1=focalLine[i].p0.x;    // внимательно заменить line.start.x на line.p0.x и тд
            int x2=focalLine[i].p3.x;    // внимательно заменить line.end.x на line.p3.x и тд
            // цикл растискивания начальных и конечных точек линий.
            for(int d=-dltX_; d<dltX; d++){
                indexV[x1+d]++;
                indexV[x2+d]++;
            }
        }
        
        ///for(int i=0; i<ncolumns; i++){cout<<indexV[i]<<" ";}
       
        /**/
        
        //создаем двухмерную матрицу с длинами строк записанными в indexV и размером в ncolumns.
        lineMatrix->setIndex(indexV,ncolumns);
        int s,x1,x2;
        //заполняем созданную матрицу, записывая длинну строки в нулевой элемент строки.
        for(int i=0;i<size;i++){ // перебор линий.
            // Получаем координаты начальных и конечных точек фокальной линии по "x".
            ///x1=focalLine[i].start.x;
            ///x2=focalLine[i].end.x;
            x1=focalLine[i].p0.x;
            x2=focalLine[i].p3.x;
            //print=0;if(x==187)print=1;
            //DR("xStart="<<x<<" i="<<i<<endl);
            // цикл растискивания начальных и конечных точек линий.
            for(int d=-dltX_; d<dltX; d++){
                s=lineMatrix[0][x1+d][0];
                s++;                    // количество элементов в строке.
                lineMatrix[0][x1+d][s]=i;
                lineMatrix[0][x1+d][0]=s;  // записываем длинну строки.
                //print=0; if(x==187)print=1;
                //DR("xEnd="<<x<<" i="<<i<<endl);
                s=lineMatrix[0][x2+d][0];
                s++;
                lineMatrix[0][x2+d][s]=i;
                lineMatrix[0][x2+d][0]=s;  // записываем длинну строки.
            }

        }
        
        /*
        cout<<"focalLine[4136].center.x="<<focalLine[4136].center.x<<endl;
        cout<<"x1="<<indexV[862]<<endl;
        cout<<"x="<<lineMatrix[862][0]<<endl;
        size=lineMatrix[862][0];
        for(int i=0;i<size;i++){
           cout<<"   index="<<lineMatrix[862][i+1]<<endl;
        }
        */
        
        if(GrafT){TIME_PRINT_}

        
    }//____________________________________________________________________________
    

    //*****************************************************************************
    
                                /// LineCombination /// Запускается в gPort.mm ,  не до конца отлажена.
    // Функция анализирует все фокальные линии на возможность создания результирующей линии (цепочки) из двух (нескольких) сопряженных линий.
    void GGraphBezier::setLineCombination(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine, GStr2D<int>*point2D, GStr2D<int>*line2D){
        
  
        int GrafT=1;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if(GrafT){
            cout<<endl<<"-----------------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для сопряжения кривых Безье.   setLineCombination() "<<endl;
        }
TIME_START
        

        
        OCRPointFloat p(0,0);

        int NumberLine=(int)focalLine.size(); // Количество линий на странице. number of lines // number of lines
        if(GrafT)cout<<"NumberLine="<<NumberLine<<endl;
        // в нулевом элементе строки матрицы записано количество линий с координатой "x" вдоль "y", с учетом растискивания.
        int countLine=line2D[0][p.x][0];
        // начиная с первого элемента в строке матрицы записаны индексы линий по заданным координатам "x"с учетом растискивания.
        int indexLine=line2D[0][p.x][1];

        
        int nl, ny;
        int startX, startY, endX, endY;    // координата "y" начальной точки совпавшей линии.
        int x1, y1, x2, y2;
        int Y1_Mdlt, Y1_Pdlt, X1_Mdlt, X1_Pdlt;
        int Y2_Mdlt, Y2_Pdlt, X2_Mdlt, X2_Pdlt;
        //int LengthLine;
        //float line1_alphaP1;    // угол наклона первого контрольного вектора кривой Безье N1.
        //float line1_alphaP2;    // угол наклона второго контрольного вектора кривой Безье N1.
        //float line2_alphaP1;    // угол наклона первого контрольного вектора кривой Безье N2.
        //float line2_alphaP2;    // угол наклона второго контрольного вектора кривой Безье N2.
        //float deltaAlpha=0;     // абсолютное значение разности углов двух линий.
        float CorrBz=0;     // Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.
        //float CorAlphaP2; // Коэффициент корреляции углов между контрольными векторами P2 кривых Безье N1 и N2.
        //float nr111=100.0/90.0;
        
        // Таблица (массив) статуса фокальной линии (свободная 0, принадлежит цепочке 1). //  состояния
        uchar *bytes_line;
        bytes_line = (uchar*)calloc(NumberLine+16, sizeof(uchar)); // с заполнением нулями



        //OCRPointFloat *test=(OCRPointFloat*)&line.test[0]; // получение указателя.
        
        // При коэффициенте корреляции больше порога, принимаем решение, что две кривые Безье являются сопряженными.
        float dlt_max=75;   // Максимальный порог коэффициента корреляции уголов (от-100 до 100).
        int dlt=3;          // на сколько pix растискиваем точки линии start.y и end.y в обе стороны по "y". дб одинаково с setLineMatrix
        int status=1;       // статус вхождения в цепочку исходной линии (N1) и найденной линии (N2).
        int mirror=0;       // =0 для сравения двух кривых Безье у которых вектора расположены в обычном порядке.
        // =1 для сравения двух кривых у которых вектора расположены в противоположном порядке (зеркальные вектора).
        int ln=0, p1=1, p1p2=1, p2=1; // Базовые параметры кривых Безье.
        // Вектор учавствует в формировании коэффициенте корреляции "1", соответственно "0" если не учавствует.
        // "ln"     вектор, проведенный от начальной 0 до конечной 3 точки кривой Безье.
        // "p1"     первый контрольный вектор кривой Безье.
        // "p1p2"   вектор, проведенный между концами контрольных векторов.
        // "p2"     второй контрольный вектор кривой Безье.
        ///int pCor=1; // pCor=0 коэффициент корреляции не зависит от масштаба (зависит от поворота кривых Безье).
                    // pCor=1 коэффициент корреляции не зависит от масштаба и от поворота кривых Безье.
        
        
        int s=0, ss=0, sss=0;
        int m=0;
        
// Цикл перебора линий N1 по всей странице.
for (nl=0; nl<NumberLine; nl++){
       // 4379 4296
    
    OCRFocalLineBezier & line_nl=focalLine[nl];   // замена focalLine[nl] на line_nl, для увеличения скорости.
    if (line_nl.lenA<LINE_SIZE) continue;   // ограничение на длинну сравниваемой линии N1. LINE_SIZE=6 задаем в GGraphBezier.h
    
    ///x1=line_nl.start.x; y1=line_nl.start.y; // Координаты начальных точек фокальной линии nl по "x".
    x1=line_nl.p0.x;    y1=line_nl.p0.y;    // Координаты начальных точек фокальной линии nl по "x".
    Y1_Mdlt=y1-dlt;     Y1_Pdlt=y1+dlt;     // лимиты (растискивание) по "y1" начальных точек.
    X1_Mdlt=x1-dlt;     X1_Pdlt=x1+dlt;     // лимиты (растискивание) по "x1" начальных точек.
    
    ///x2=line_nl.end.x;   y2=line_nl.end.y;   // Координаты конечных точек фокальной линии nl по "x".
    x2=line_nl.p3.x;    y2=line_nl.p3.y;    // Координаты конечных точек фокальной линии nl по "x".
    Y2_Mdlt=y2-dlt;     Y2_Pdlt=y2+dlt;     // лимиты (растискивание) по "y1" конечных точек.
    X2_Mdlt=x2-dlt;     X2_Pdlt=x2+dlt;     // лимиты (растискивание) по "x1" конечных точекм

    
    ///line1_alphaP1=line_nl.alphaP1;  // угол наклона первого контрольного вектора кривой Безье N1.
    
    // Для каждой фокальной линии N1 на странице определяем, какие начальные и конечные точки других линий N2
    // попадает на одни и те же координаты по "x1", с учетом растискивания на dltX.
    countLine=line2D[0][x1][0];

    // перебор всех "y" других линий N2 с совпавшими координатами начальной точки "x1", с учетом растискивания на dltY.
    for (ny=1; ny<countLine+1; ny++){
        
        ///if(GrafT)cout<<"nl="<<nl<<" ny="<<ny<<" indexLine="<<indexLine<<" ***************"<<endl;
        
        indexLine=line2D[0][x1][ny]; // номера линий с одинаковыми начальными точками "x" и всеми возможными  "y".
        
        // Проверка линии: если "==nl"  линии совпали начальными точками сами с собой (иначе будет закрашено все),
        // или если эта линия уже просчитана (==1), то выходим из цикла (continue). // break;
        if (indexLine==nl) continue;
        ///if (indexLine==nl || bytes_line[indexLine]==3) continue; // на порядок
        
        OCRFocalLineBezier & line_in=focalLine[indexLine]; // замена focalLine[indexLine] на line_in, для увеличения скорости.
        if (line_in.lenA<LINE_SIZE) continue;        // ограничение на длинну сравниваемой линии N2
        
        ///startY=line_in.start.y;  // координата "y" начальной точки совпавшей линии N2.
        ///endY=line_in.end.y;      // координата "y" конечной точки совпавшей линии N2.
        startY=line_in.p0.y;        // координата "y" начальной точки совпавшей линии N2.
        endY=line_in.p3.y;          // координата "y" конечной точки совпавшей линии N2.
        s++;
        
        
        /// СТАРТОВАЯ точка линии N1. ///
        // Проверка стартовой точки линии N2 по "y" по отношению к лимитам стартовой точки линии N1. //
        if (Y1_Mdlt<startY && startY<Y1_Pdlt)  {
            ///startX=line_in.start.x;  // координата "x" начальной точки совпавшей линии
            startX=line_in.p0.x;        // координата "x" начальной точки совпавшей линии
            if (X1_Mdlt<startX && startX<X1_Pdlt)  { // здесь еще есть "x" линии N2 не соответствующие "start.x;"
            ss++;
            // Коэффициент корреляции кривых Безье N1 и N2. // mirror=1; ln=0, p1=1, p1p2=1, p2=1
            CorrBz=CorrelationBezierB(line_nl, line_in, mirror,  ln, p1, p1p2, p2);
            ///CorrBz=CorrelationBezierT(line_nl, line_in);
            // Нужно проверить в точке соотношение start и end линий N1 и N2 и принять решение надо ли выполнять mirror=1,
            // а в самой CorrelationBezierB возможно отключить дальние вектора.
                if (CorrBz > dlt_max) {
                    ///bytes_line[indexLine]+=1;    // Отмечаем в таблице (массиве) занятости новую фокальную линию (1).
                    line_nl.status=status;       line_in.status=status;   // статус исходной и найденной линии N1.
                    sss++;
                    // Запись в массив координат двух исходных точек сглаженной фокальной линии
                    //line_in.test.resize(2);  // resize на 2 пары.
                    //line_in.test[0].x=x1;    line_in.test[0].y=y1;
                    /**/
                } // if (CorrBz
            } // if (Y1_Mdlt<startY && startY<Y1_Pdlt)
        } // if (Y1_Mdlt<startY && startY<Y1_Pdlt)
        
        
        // Проверка конечной точки линии N2 по "y" по отношению к лимитам стартовой точки линии N1. //
        if (Y1_Mdlt<endY && endY<Y1_Pdlt)  {
            ///endX=line_in.end.x;  // координата "x" начальной точки совпавшей линии.
            endX=line_in.p3.x;      // координата "x" начальной точки совпавшей линии.
            if (X1_Mdlt<endX && endX<X1_Pdlt)  {
                ss++;
                // Коэффициент корреляции кривых Безье N1 и N2.
                CorrBz=CorrelationBezierB(line_nl, line_in, mirror,  ln, p1, p1p2, p2);
                ///CorrBz=CorrelationBezierT(line_nl, line_in);
                if (CorrBz > dlt_max) {
                    ///bytes_line[indexLine]+=1;    // Отмечаем в таблице (массиве) занятости новую фокальную линию (1).
                    line_nl.status=status;       line_in.status=status;   // статус исходной и найденной линии N1.
                    sss++;
//                  if(GrafT)cout<<"nl1="<<nl<<endl;
                } // if (CorrBz
            } // if (X1_Mdlt<endX && endX<X1_Pdlt)
        } // if (Y1_Mdlt<endY && endY<Y1_Pdlt)

        
        
        /// КОНЕЧНАЯ точка линии N1. ///
        // Проверка стартовой точки линии N2 по "y" по отношению к лимитам конечной точки линии N1. //
        if (Y2_Mdlt<startY && startY<Y2_Pdlt)  {
            ///startX=line_in.start.x;  // координата "x" начальной точки совпавшей линии
            startX=line_in.p0.x;        // координата "x" начальной точки совпавшей линии
            if (X2_Mdlt<startX && startX<X2_Pdlt)  { // здесь еще есть "x" линии N2 не соответствующие "start.x;"
                ss++;
                // Коэффициент корреляции кривых Безье N1 и N2.
                CorrBz=CorrelationBezierB(line_nl, line_in, mirror,  ln, p1, p1p2, p2);
                ///CorrBz=CorrelationBezierT(line_nl, line_in);
                if (CorrBz > dlt_max) {
                    ///bytes_line[indexLine]+=2;    // Отмечаем в таблице (массиве) занятости новую фокальную линию N1.
                    line_nl.status=status;       line_in.status=status;   // статус исходной и найденной линии N1.
                    sss++;
 //                   if(GrafT)cout<<"nl2="<<nl<<endl;
                    /**/
                } // if (CorrBz
            } // if (Y2_Mdlt<startY && startY<Y2_Pdlt)
        } // if (Y2_Mdlt<startY && startY<Y2_Pdlt)
        
        
        // Проверка конечной точки линии N2 по "y" по отношению к лимитам конечной точки линии N1. //
        if (Y2_Mdlt<endY && endY<Y2_Pdlt)  {
            ///endX=line_in.end.x;  // координата "x" начальной точки совпавшей линии.
            endX=line_in.p3.x;      // координата "x" начальной точки совпавшей линии.
            if (X2_Mdlt<endX && endX<X2_Pdlt)  {
                ss++;
                // Коэффициент корреляции кривых Безье N1 и N2.
                CorrBz=CorrelationBezierB(line_nl, line_in, mirror,  ln, p1, p1p2, p2);
                ///CorrBz=CorrelationBezierT(line_nl, line_in);
                if (CorrBz > dlt_max) {
                    ///bytes_line[indexLine]+=2;    // Отмечаем в таблице (массиве) занятости новую фокальную линию (1).
                    line_nl.status=status;       line_in.status=status;   // статус исходной и найденной линии N1.
                    sss++;
                } // if (CorrBz
            } // if (X2_Mdlt<endX && endX<X2_Pdlt)
        } // if (Y2_Mdlt<endY && endY<Y2_Pdlt)
        

    } // for (ny=1 ////
    
        
} // for (nl=0; ////

        
        if(GrafT)cout<<"     nl="<<nl<<"     s="<<s<<"     ss="<<ss<<"     sss="<<sss<<"     m="<<m<<endl;
        /**/
        
        // освобождение массивов
        if ( bytes_line !=NULL ) free(bytes_line);
        
        if(GrafT){TIME_PRINT_}
//TIME_PRINT_
        
        

    }//____________________________________________________________________________
    

    
    //*****************************************************************************
    
                        /// LineCombination2 промежуточная версия /// запускаем в gPort.mm
    // Функция анализирует все фокальные линии на возможность создания результирующей линии (цепочки) из двух (нескольких) сопряженных линий.
    void GGraphBezier::setLineCombination2(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine, GStr2D<int>*point2D, GStr2D<int>*line2D){
        
        
        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if(GrafT){
            cout<<endl<<"-----------------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для сопряжения кривых Безье.   setLineCombination() "<<endl;
        }
        TIME_START
        
        // При коэффициенте корреляции больше порога, принимаем решение, что две кривые Безье являются сопряженными.
        float dlt_max=85;   // Максимальный порог коэффициента корреляции уголов (от-100 до 100).
        int dlt=3; // на сколько pix растискиваем точки линии start.y и end.y в обе стороны по "y".
        
        OCRPointFloat p(0,0);
        
        int NumberLine=(int)focalLine.size(); // Количество линий на странице. number of lines // number of lines
        if(GrafT)cout<<"NumberLine="<<NumberLine<<endl;
        // в нулевом элементе строки матрицы записано количество линий с координатой "x" вдоль "y", с учетом растискивания.
        int countLine=line2D[0][p.x][0];
        // начиная с первого элемента в строке матрицы записаны индексы линий по заданным координатам "x"с учетом растискивания.
        int indexLine=line2D[0][p.x][1];
        
        
        int nl, ny;
        int startX, startY, endX, endY;    // координата "y" начальной точки совпавшей линии.
        int x1, y1;//, x2, y2;
        int dltY1, dltY2, dltX1, dltX2;
        //int LengthLine;
        float line1_alphaP1;    // угол наклона первого контрольного вектора кривой Безье N1.
        //float line1_alphaP2;    // угол наклона второго контрольного вектора кривой Безье N1.
        float line2_alphaP1;    // угол наклона первого контрольного вектора кривой Безье N2.
        float line2_alphaP2;    // угол наклона второго контрольного вектора кривой Безье N2.
        float deltaAlpha=0;     // абсолютное значение разности углов двух линий.
        float CorAlphaP1=0;     // Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.
        //float CorAlphaP2;     // Коэффициент корреляции углов между контрольными векторами P2 кривых Безье N1 и N2.
        float nr111=100.0/90.0;
        
        // Таблица (массив) статуса фокальной линии (свободная 0, принадлежит цепочке 1). //  состояния
        uchar *bytes_line;
        bytes_line = (uchar*)calloc(NumberLine+16, sizeof(uchar)); // с заполнением нулями
        
        
        
        //OCRPointFloat *test=(OCRPointFloat*)&line.test[0]; // получение указателя.
        
        
        int s=0, ss=0, sss=0;
        int m=0; //dlt=3;
        
        // Цикл перебора линий N1 по всей странице.
        for (nl=0; nl<NumberLine; nl++){
            ////nl=3617;  // 4379 4296
            
            OCRFocalLineBezier & line_nl=focalLine[nl];       // замена focalLine[nl] на line_nl, для увеличения скорости.
            if (line_nl.lenA<LINE_SIZE) continue;       // ограничение на длинну сравниваемой линии N1
            
            x1=line_nl.start.x;   y1=line_nl.start.y;   // Координаты начальных точек фокальной линии nl по "x".
            //x2=line_nl.end.x;     y2=line_nl.end.y    // Координаты конечных точек фокальной линии nl по "x".
            
            /*
             ////////////////
             // Запись в массив координат двух координат второй управляющей точки.
             float norma=0.00001, norma1=0.00001, norma2=0.00001;
             float Px0=line_nl.start.x;  float Px1=line_nl.p1.x;     float Px2=line_nl.p2.x;     float Px3=line_nl.end.x;
             float Py0=line_nl.start.y;  float Py1=line_nl.p1.y;     float Py2=line_nl.p2.y;     float Py3=line_nl.end.y;
             line_nl.test.resize(2);  // resize на 2 пары.
             //    line_nl.test[0].x=0;    line_nl.test[0].y=0;    line_nl.test[1].x=0;    line_nl.test[1].y=0;
             
             // Px2=Px1-4(Px2-Px3)   // коэффициенты слева и справа относятся к разным уравнениям L1 и L2
             int nl1=1627;
             int nl2=1648;
             if(nl==nl2 || nl==nl2) {
             norma2=line_nl.lenB;    norma1=line_nl.lenB;
             norma=norma1/norma2;
             cout<<"norma1="<<norma1<<"  norma2="<<norma2<<" norma="<<norma<<endl;
             }
             if(nl==nl1) {
             line_nl.test[0].x=Px1-4.0*(Px2-Px3);    line_nl.test[0].y=Py1-4.0*(Py2-Py3);
             ////line_nl.test[0].x=Px3-4.0*(Px2-Px1);    line_nl.test[0].y=Py3-4.0*(Py2-Py1); // ввверх влево
             //line_nl.test[0].x=Px2-4.0*(Px1-Px0);    line_nl.test[0].y=Py2-4.0*(Py1-Py0);
             //        line_nl.test[0].x=Px2;          line_nl.test[0].y=Py2;
             //        line_nl.test[1].x=Px3;          line_nl.test[1].y=Py3;
             cout<<"1 Px2="<<Px2<<endl;
             }
             if(nl==nl2) {
             
             Px2=norma*(Px2-Px3)+Px3;    Py2=norma*(Py2-Py3)+Py3;
             line_nl.test[1].x=Px2;          line_nl.test[1].y=Py2;
             cout<<"2 Px2="<<Px2<<endl;
             }
             
             ///cout<<"Px3-2.0*Px2+Px1="<<Px3-2.0*Px2+Px1<<"   Py3-2.0*Py2+Py1="<<Py3-2.0*Py2+Py1<<endl;
             ///if(nl==1599)
             ///cout<<"Px0-2.0*Px1+Px2="<<Px0-2.0*Px1+Px2<<"   Py0-2.0*Py1+Py2="<<Py0-2.0*Py1+Py2<<endl;
             
             // P''(0)=6(B0-2B1+B2) //
             // P''(1)=6(B3-2B2+B1)
             ////////////////
             */
            
            
            
            line1_alphaP1=line_nl.alphaP1;  // угол наклона первого контрольного вектора кривой Безье N1.
            
            dltY1=y1-dlt;   dltY2=y1+dlt;   // лимиты (растискивание) по "x1" по отношению к линии N1
            dltX1=x1-dlt;   dltX2=x1+dlt;   // лимиты (растискивание) по "y1" по отношению к линии N1
            
            // Для каждой фокальной линии N1 на странице определяем, какие начальные и конечные точки других линий N2
            // попадает на одни и те же координаты по "x1", с учетом растискивания на dltX.
            countLine=line2D[0][x1][0];
            
            // перебор всех "y" других линий N2 с совпавшими координатами начальной точки "x1", с учетом растискивания на dltY.
            for (ny=1; ny<countLine+1; ny++){
                
                ///if(GrafT)cout<<"nl="<<nl<<" ny="<<ny<<" indexLine="<<indexLine<<" ***************"<<endl;
                
                indexLine=line2D[0][x1][ny]; // номер линии с совпавшей координатой начальной точки "x" и переменной "y".
                
                // Проверка линии. Если эта линия уже просчитана (==1), то выходим из цикла (continue). // break;
                // или если "==nl"  линии совпали начальными точками сами с собой.
                if (indexLine==nl) continue; // иначе будет закрашено все
                //if (indexLine==nl || bytes_line[indexLine]==1) continue; // на порядок
                
                OCRFocalLineBezier & line_in=focalLine[indexLine]; // замена focalLine[indexLine] на line_in, для увеличения скорости.
                if (line_in.lenA<LINE_SIZE) continue;        // ограничение на длинну сравниваемой линии N2
                
                startY=line_in.start.y; // координата "y" начальной точки совпавшей линии N2.
                endY=line_in.end.y;     // координата "y" конечной точки совпавшей линии N2.
                s++;
                
                ///if ( (dltY1<startY && startY<dltY2 && dltX1<startX && startX<dltX2) || (dltY1<endY && endY<dltY2 && dltX1<endX && endX<dltX2) ) {
                if (dltY1<startY && startY<dltY2)  {
                    startX=line_in.start.x;    // координата "y" начальной точки совпавшей линии
                    //if ( (dltX1<startX && startX<dltX2) || (dltX1<endX && endX<dltX2) )  {
                    if (dltX1<startX && startX<dltX2)  {
                        ss++;
                        
                        ///            if(GrafT)cout<<endl<<"------------------------------------"<<endl;
                        ///            if(GrafT)cout<<"nl="<<nl<<" x1="<<x1<<" y1="<<y1<<"     indexLine="<<indexLine<<"   line1_alphaP1="<<line1_alphaP1<<"    line2_alphaP1="<<line2_alphaP1<<endl;
                        
                        line2_alphaP1=line_in.alphaP1;  // угол наклона первого контрольного вектора кривой Безье N2.
                        //if(GrafT)cout<<nl<<" line1_alphaP1="<<line1_alphaP1<<"   line2_alphaP1="<<line2_alphaP1<<endl;
                        
                        /// Вычисление разности между углами контрольных векторов P1 кривых Безье N1 и N2. ///
                        deltaAlpha=fabs(line2_alphaP1-line1_alphaP1); // абсолютное значение разности углов двух контрольных векторов.
                        ////if (deltaAlpha==0) { CorAlphaP1=100.11111111; continue; }   // и дальше не вычисляем; // очень много deltaAlphaБ<0.01
                        //   if(GrafT)cout<<"nl="<<nl<<" line1_alphaP1="<<line1_alphaP1<<" indexLine="<<indexLine<<"   line2_alphaP1="<<line2_alphaP1<<endl;
                        
                        // абсолютное значение разности углов контрольных векторов не м.б. больше 180°.
                        if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;
                        // абсолютное значение разности углов контрольных векторов не м.б. больше 90°
                        //  if(GrafT)cout<<"(P1) deltaAlpha="<<deltaAlpha<<endl;
                        ///if (deltaAlpha>90) { deltaAlpha=180-deltaAlpha; CorAlphaP1=-100+nr111*deltaAlpha; }//CorAlphaP1=100-100*deltaAlpha/90;
                        ///else CorAlphaP1=100-nr111*deltaAlpha;  // Коэффициент корреляции двух векторов по углам (0-100%).
                        if (deltaAlpha>90) deltaAlpha=180-deltaAlpha;
                        CorAlphaP1=100-nr111*deltaAlpha;
                        //    if(GrafT)cout<<"(P1) deltaAlpha="<<deltaAlpha<<"  CorAlphaP1="<<CorAlphaP1<<endl;
                        
                        CorAlphaP1=CorrelationBezierB(line_nl, line_in, 0, 0, 1, 1, 1);
                        // Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.
                        if (CorAlphaP1 > dlt_max) {
                            bytes_line[indexLine]=1;    // Отмечаем в таблице (массиве) занятости новую фокальную линию (1).
                            line_nl.status=1;       line_in.status=1;   // статус исходной линии N1.    // статус найденной линии N2.
                            
                            sss++;
                            
                            // Запись в массив координат двух исходных точек сглаженной фокальной линии
                            //              line_in.test.resize(2);  // resize на 2 пары.
                            //              line_in.test[0].x=x1;    line_in.test[0].y=y1;
                            //test[0]=line_in; // запись в массив точки L1=L/3.
                            //s_line.x=x2;    s_line.y=y2;
                            //test[1]=s_line; // запись в массив точки L2=L*2/3.
                            /**/
                            
                            //////                    if(GrafT)cout<<"nl="<<nl<<" indexLine="<<indexLine<<endl;
                            ///if(GrafT)cout<<"(P1) deltaAlpha="<<deltaAlpha<<"     Разность углов между векторами P1 кривых Безье N1 и N2"<<endl;
                            ///                if(GrafT)cout<<"(P1) Коэффициент корреляции углов между контрольными векторами P1 кривых Безье   CorAlphaP1="<<CorAlphaP1<<"%"<<endl<<endl;
                            
                        } // if (CorAlphaP1
                        
                    } // if (dltY1<endY && endY<dlt2
                    
                } // if (dlt1<startY && startY<dlt2)
                
                
                
                
                
                
                if (dltY1<endY && endY<dltY2)  {
                    endX=line_in.end.x;    // координата "y" начальной точки совпавшей линии.
                    if (dltX1<endX && endX<dltX2)  {
                        ss++;
                        
                        line2_alphaP2=line_in.alphaP2+180;  //P2+180 сравниваем с P1 // угол наклона первого контрольного вектора кривой Безье N2.
                        if (line2_alphaP2>=360) line2_alphaP2=line2_alphaP2-360; //alpha
                        //if(GrafT)cout<<nl<<" line1_alphaP1="<<line1_alphaP1<<"   line2_alphaP1="<<line2_alphaP1<<endl;
                        
                        //line1_alphaP1=line1_alphaP1+180;
                        
                        /// Вычисление разности между углами контрольных векторов P1 кривых Безье N1 и N2. ///
                        deltaAlpha=fabs(line2_alphaP2-line1_alphaP1); // абсолютное значение разности углов двух контрольных векторов.
                        //    if(GrafT)cout<<"(P1) deltaAlpha="<<deltaAlpha<<endl;
                        // абсолютное значение разности углов контрольных векторов не м.б. больше 180°.
                        if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;
                        // абсолютное значение разности углов контрольных векторов не м.б. больше 90°
                        ///if (deltaAlpha>90) { deltaAlpha=180-deltaAlpha; CorAlphaP1=-100+nr111*deltaAlpha; }//CorAlphaP1=100-100*deltaAlpha/90;
                        ///else CorAlphaP1=100-nr111*deltaAlpha;  // Коэффициент корреляции двух векторов по углам (0-100%).
                        if (deltaAlpha>90) deltaAlpha=180-deltaAlpha;
                        CorAlphaP1=100-nr111*deltaAlpha;
                        //     if(GrafT)cout<<"(P1) deltaAlpha="<<deltaAlpha<<"  CorAlphaP1="<<CorAlphaP1<<endl;
                        
                        CorAlphaP1=CorrelationBezierB(line_nl, line_in, 0, 0, 1, 1, 1);
                        // Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.
                        if (CorAlphaP1 > dlt_max) {
                            bytes_line[indexLine]=1;    // Отмечаем в таблице (массиве) занятости новую фокальную линию (1).
                            line_nl.status=1;       line_in.status=1;   // статус исходной линии N1.    // статус найденной линии N2.
                            sss++;
                        } // if (CorAlphaP1
                        
                    } // if (dltY1<endY && endY<dlt2
                    
                } // if (dlt1<startY && startY<dlt2)
                
                
                
                
                
                
                
                
            } // for (ny=1
            
            
        } // for (nl=0; ////
        
        
        if(GrafT)cout<<"     nl="<<nl<<"     s="<<s<<"     ss="<<ss<<"     sss="<<sss<<"     m="<<m<<endl;
        /**/
        
        ///if ( indexLine != nl && ((dlt1<y3 && y3<dlt2) || (dlt1<y4 && y4<dlt2)) ) { // != линии совпавшее сами с собой, не учитываем.
        
        // Для каждой фокальной линии на странице определяем, сколько КОНЕЧНЫХ точек линий
        // попадает на одни и те же координаты по "x", с учетом растискивания на dltX.
        ///countLine=line2D[x2][0];
        
        
        
        
        // освобождение массивов
        if ( bytes_line !=NULL ) free(bytes_line);
        
        if(GrafT){TIME_PRINT_}
        //TIME_PRINT_
        
        
        
        
        int t=1044;
        int count=line2D[0][focalLine[t].start.x][0];
        ///             cout<<"x="<<focalLine[t].start.x<<" line2D[focalLine[t].start][0]="<<count<<endl;
        for(int i=0; i<count;i++){
            ///                    cout<<" index line ="<<line2D[focalLine[t].start.x][i+1]<<endl;
        }
        
        
        
        
        /// Вычисление полного коэффициента корреляции контрольных векторов P1 кривых Безье N1 и N2.. ///
        // Суммы разности углов берутся с весами пропорциональными произведению длин соответствующих векторов.
        ///        float CorAlphaSUM=CorAlphaP1*(line1_lenP1*line2_lenP1);
        ///    if(GrafT)cout<<"Коэффициент корреляции кривых Безье (суммы разности углов с весами проп. длине вектора).    CorAlphaSUM="
        ///        <<CorAlphaSUM<<"%"<<endl<<endl;
        
        
        
        /*
         // При коэффициенте корреляции меньше порога, принимаем решение, линия N1 НЕ является сопряженной линии N2.
         if (CorAlphaP1 < dlt_max) {
         CorAlphaP1=0;
         if(GrafT)cout<<"линия N1 НЕ является сопряженной линии N2.   CorAlphaSUM="<<CorAlphaP1<<"%"<<endl<<endl;
         } else {
         if(GrafT)cout<<"линия N1 является сопряженной линии N2     CorAlphaSUM="<<CorAlphaP1<<"%"<<endl<<endl;
         }
         */
        
        
        ///uint *indexV;
        ///indexV=(uint*)calloc(ncolumns, sizeof(uint)); // весь блок заполнен нулями
        //if ( indexV !=NULL ) free(indexV);
        
        //uint bytes_line[NumberLine];
        // memset(Массив, Нулевое значение, Размерность массива)
        //memset((char*)bytes_line, 0, NumberLine*4); // Функция memset() копирует младший байт параметра 0 в первые NumberLine*4 символов массива, адресуемого параметром bytes_line.
        
        // uint indexV[100]={0};
        
        // Функция для склеивания двух кривых Безье.
        
    }//____________________________________________________________________________
    



}//____________________________________________________________________________
