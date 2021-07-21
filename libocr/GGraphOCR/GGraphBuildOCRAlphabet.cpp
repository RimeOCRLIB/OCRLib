// _______GraphOCRBuildOCRTable.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "../OCRString/php2stl.h"
#include "GGraphOCR.h"
#include <unistd.h>

namespace ocr {
    
    
int testLetterInGroup(OCRGroup &newGroup){
    int flag=1;
    
    
    
    return flag;
}
    
void GGraphOCR::buildletterGroup(OCRLetter &letter){
    int print=1;
    DR(letter.index<<" Start group for letter: "<<letter.name.data <<" ID:" << letter.letterID;)
    letterCorrelationLimit=letterCorrelationLimitGroup;
    
    //создаем GGraph этой буквы
    //открываем изображение слога для распознавания
    string pathImg=font.pathArray[letter.index].data;
    GBitmap *img = GBitmap::create(pathImg.c_str());
    
    img->crop32();
    img->binarisation64();
    GGraph graph_1(img);
    //img->printToScreenBinary();
    graph_1.graphFocalLine(15);
    //graph_1.printToScreenBinary();
    graph_1.focalPointXTL();
    graph_1.focalLineRecognition();
    graph_1.buildOCRStars();
    
    strArray.resize(0);
    OCRBox p;
    p.yLimit0=0;
    p.yLimit1=img->rows();
    strArray.push_back(p);
    img->destroy();
    
    //сравниваем все буквы базы с этой буквой, создаем группы OCRGroupData
    letterAphabetCorrelation(graph_1, letter,1);
    DR(" size:"<<font.letterGroupData[letter.index].size<<endl;)
}
    
//** @bref функция создает реестр структур параметров распознавания групп букв OCR шрифта*/
void GGraphOCR::buildAlphabet(){
    /*
     OCR шрифт используется как обучающая выборка для создания таблицы структур
     весов параметров распознавания пар букв в группе похожих букв OCR шрифта.
     Такая таблица соответствует одному слою нейросети и позволяет учесть графические различия
     при распознавании группы похожих эталонных букв в тексте.
     
     При создании группы букв для каждой буквы OCR шрифта производится сравнение со всеми буквами.
     Группа составляется из букв базы OCR шрифта совпадающих по корреляции с точностью до 90%
     Каждая группа содержит таблицу прямой и обратной корреляции пары букв группы как таблицу структур типа OCRLetterMatch.
     Группы букв OCR шрифта записываются как массив структур OCRGroup.
     
     Создание OCRLetterMatch пары букв группы производится на основании графической разности и графической общности
     скелетных графов изображений каждой пары букв из группы похожих букв OCR шрифта.
     Каждой OCRLine графической разности двух букв присваивается корреляционный вес соответствующий частоте встречаемости этой линии
     для букв с одним значением Юникода.
     В OCRLetterMatch пары букв записываются веса линий тестовой буквы. Веса обратной корреляции сравнения базовой буквы с тестовой
     записываются в соответствующий OCRLetterMatch пары букв в которой тестовая буква является базовой. 
     Этот OCRLetterMatch записан в группе тестовой буквы.
     

     При создании группы в @fn buildOCRLetterGroup производится анализ группы. Такой анализ проводится в три этапа.
     
     На первом этапе для всех букв базы находим все буквы базы которые распознают изображение данной буквы.
     Все найденные соответствия записываем в группы букв. Для каждой базовой букве создается группа похожих букв.
     
     На втором этапе для всех эталонных шрифтовых букв вычисляются веса графической общности и 
     графической разности. Поскольку для такого вычисления критичным является синхронизация бвух букв по 
     кластерам и отсутствие шумовых векторов, такое вычисление производистя для шрифтовых эталонных букв.
     
     На этом этапе попарно сравниваются все шрифтовые буквы группы одинаковые по Юникоду.
     Для всех OCRLine имеющих высокую корреляцию по lookup2 и lookup4 увеличивается весовой коэффициент корреляции.
     Полученные весовые коэффициенты корреляции OCRLine нормируются по отношению к общей сумме весов корреляции всех
     OCRLine в OCRLetter.
     (kLine1 + kLine2 ... + kLineN) = 100%
     где kLine1, kLine2 ... это полученные ненормированные весовые коэффициенты всех OCRLine буквы
     
     На втором этапе попарно сравниваются все буквы группы различные по Юникоду.
     Для всех OCRLine имеющих нулевую или низкую корреляцию по lookup2 и lookup4 весовой коэффициент
     корреляции учитывается как отрицательный для буквы в которой нет соответствующей OCRLine.
     Величина отрицательного коэффициента нормируетя по сумме весовых коэффициентов всех OCRLine
     графической разности.
     (kLineNew1 + kLineNew2 ... + kLineNewN) = -100%
     где kLineNew1, kLineNew2 ... это полученные на первом этапе нормированные весовые коэффициенты
     всех OCRLine графической разности этой пары букв
     
     В результате для каждой буквы группы больший вес получают те OCRLine которые являются общими для букв с одинаковым
     значением Юникоду и OCRLine характеризующие отличия от букв группы с другим значением Юникод.
     Таким образом отличия в написании букв не влияющие на различение графем букв нивелируются, и выявляются
     характерные особенности скелетного графа каждой пары букв имеющие большое значение при распознавании.
     
     При распознавании все OCRLetter входящие в группу проверяются с одним масштабом в одном месте текста.
     Результатом распознавания OCRLetterStack в тексте является последовательность OCRLetter
     сортированная по величине корреляции с учетом графической разности.
     */

    //Сортируем буквы алфавита по количеству кластеров
    cout<<"font:"<<font.size()<<endl;
    uint fontSize = (uint)font.size();
    int print = 1;
    DR("Start build alphabet OCRLetterGroup"<<endl;)
    
    int mode=3;
    

    if(mode==1){
        //составление групп похожих букв для всех бкув шрифта как шрифтовых так и рукописных
        
        font.buildSortArray();
        font.clearLetterGroupData();
        font.letterGroupData.resize(fontSize);
        modeVoronov=1;  //при составлении груп используем режим общего сравнения пары букв без учета коротких линий
        //начиная с первой буквы
        
        //присваиваем OCRStatus для исключения пар букв одинаковых по значению корреляции
        //на этапе создания letterGroupData не исключаем повторы букв. Для того чтобы присвоить букве нулевой OCRStatus
        //необходимо проверить прямую и обратную корреляцию пары букв.
        for( int index=0;index < fontSize;index++){
            OCRLetter &letterTest = font[index];
            letterTest.OCRStatus=1;
        }
        for( int index=0;index < fontSize;index++){
            OCRLetter &letterTest = font[index];
            buildletterGroup(letterTest);
        }
        cout<<" done build letterGroupData groups"<<endl;
        int max=0;
        int averageSize;
        int size[MAX_LETTER_GROUP1];
        memset(size, 0, MAX_LETTER_GROUP1*4);
        uint letterGroupDataSize=(uint)font.letterGroupData.size();
        for( int index=0;index < letterGroupDataSize;index++){
            OCRGroupData &group=font.letterGroupData[index];
            if(group.size>max)max=group.size;
            size[group.size]++;
            averageSize+=group.size;
        }
        for( int i=0;i < 38;i++){
            for( int index=0;index < letterGroupDataSize;index++){
                OCRGroupData &group=font.letterGroupData[index];
                if(group.size==i){
                    OCRLetter &letter=font[group.letter[0]];
                    cout<<"group.size:"<<group.size<<" letter:"<<letter.name.data<<endl;
                    break;
                }
            }
        }
        cout<<"all group count:"<<font.letterGroupData.size()<<" maxSize:"<<max<<" averageSize:"<<averageSize/letterGroupDataSize<<endl;
        printArray(size, 37);
        
        //проверка одинаковых по Юникоду и корреляции букв.
        //если буквы не относятся к шрифтовому алфавиту, одинаковы по юникоду
        //и тестовая буква имеет с базовой буквой высокую корреляцию
        //считаем эти буквы одинаковыми и отмечаем что текстовая буква является повтором.
        
        cout<<"//проверка одинаковых по Юникоду и корреляции букв."<<endl;
        for( int index=0;index < fontSize;index++){
            OCRLetter &letterTest = font[index];
            if(letterTest.fontType)continue;
            //проверяем группу этой буквы
            OCRGroupData &group=font.letterGroupData[index];
            for( int i=1;i < group.size;i++){
                OCRLetterMatch &match=font.letterMatch[group.match[i]];
                if(!match.OCRIndex)continue;
                if(match.corr>97){
                    //проверяем обратную корреляцию буквы
                    OCRGroupData &group_=font.letterGroupData[group.letter[i]];
                    int flag=0;
                    for( int n=1;n < group_.size;n++){
                        OCRLetterMatch &match_=font.letterMatch[group_.match[n]];
                        if(match_.testIndex==index && match_.corr>97){
                            flag=1;
                            break;
                        }
                    }
                    if(flag){
                        letterTest.OCRStatus=0;
                        cout<<" index:"<<index<<" set letter:"<<letterTest.name.data<<endl;
                        break;
                    }
                    
                }
            }
        }
        //также заполняем индексы OCRLetterMatch тестовой буквы в массиве font.letterMatch
        //индекс OCRLetterMatch тестовой буквы используется при выполнении
        //вычисления обратной корреляции базовой и тестовой буквы

        cout<<"//заполняем индексы OCRLetterMatch тестовой буквы"<<endl;
        for( int index=0;index < fontSize;index++){
            OCRLetter &letterTest = font[index];
            if(!letterTest.OCRStatus)continue;
            //проверяем группу этой буквы
            OCRGroupData &group=font.letterGroupData[index];
            for( int i=1;i < group.size;i++){
                OCRLetterMatch &match=font.letterMatch[group.match[i]];
                //проверяем обратную корреляцию буквы
                OCRGroupData &group_=font.letterGroupData[group.letter[i]];
                for( int n=1;n < group_.size;n++){
                    OCRLetterMatch &match_=font.letterMatch[group_.match[n]];
                    if(match_.testIndex==index){
                        match.matchIndexTest=match_.matchIndex;
                        break;
                    }
                }
            }
        }


    }
    uint letterGroupDataSize=(uint)font.letterGroupData.size();
    if(!letterGroupDataSize){
        cout<<" need build letterGroupData first"<<endl;
        return;
    }
    //mode++;
    
 
    
    //if(letterTest.correlation > 97 && (!letterMain.fontType || !letterTest.fontType) && letterMain.name==letterTest.name){
    //    font[indexLetterT].OCRStatus=0;
    //    break;
    //}

    
    if(mode==2){
        font.clearLetterGroup();
        //Для букв каждого эталонного шрифта вычисляем общие линии пар букв с одним значением Юникод на основе точного сравнения букв.
        //Точное сравнение учитывает графическую разность букв с точностью до коротких линий и формы линий.
        //Общие линии пар букв с одним значением Юникод соооветствуют общеупотребительным особенностям букв.
        //присваеваем таким общим линиям больший корреляционный вес
        
        // вычисляем веса линий базовой буквы совпадающие с линиями всех шрифтовых букв группы с одним значением Юникод
        cout<<" start build same Unicode letters data"<<endl;
        modeVoronov=0;  //при проверке пары букв используем режим точного сравнения пары букв с учетом коротких линий
        for( int index=0;index < fontSize;index++){
            OCRLetter &letterBase=font[index];
            if(!letterBase.OCRStatus ||  !letterBase.fontType)continue;  //проверяем только шрифтовые буквы
            cout<<"i:"<<index<<" letter:"<<letterBase.name.data<<" OCRStatus:"<<letterBase.OCRStatus<<" f:"<<(int)letterBase.fontType<<endl;
            
            OCRGroupData &group=font.letterGroupData[index];
            OCRLetterMatch &letterMatchMain=font.letterMatch[group.match[0]];//первая буква в группе это основная буква группы
            letterBase.status=0;
            int lineCount=letterBase.lineCount; //массив для подсчета весов совпадающих линий
            uchar kLineSum[lineCount];
            memset(kLineSum,1,lineCount);
            int count=1;
            //drawLetterByID(letter.letterID);
            
            //создаем GGraph базовой буквы
            //открываем изображение буквы для распознавания
            string pathImg=font.pathArray[letterBase.index].data;
            GBitmap *img = GBitmap::create(pathImg.c_str());
            
            img->crop32();
            img->binarisation64();
            GGraph graph_1(img);
            //img->printToScreenBinary();
            graph_1.graphFocalLine(15);
            //graph_1.printToScreenBinary();
            graph_1.focalPointXTL();
            graph_1.focalLineRecognition();
            graph_1.buildOCRStars();
            
            strArray.resize(0);
            OCRBox p;
            p.yLimit0=0;
            p.yLimit1=img->rows();
            strArray.push_back(p);
            img->destroy();

            //сравниваем с базовой буквой все шрифтовые буквы группы, подсчитываем веса линий базовой буквы
            //совпадающих с линиями букв с одним значением Юникод
            for(int i=1;i < group.size; i++){
                OCRLetterMatch &match=font.letterMatch[group.match[i]];
                if(!match.OCRIndex)continue; //проверяем только буквы совпадающие по Юникод
                OCRLetter letterTest_=font[group.letter[i]];
                if(!letterTest_.fontType)continue; //проверяем только шрифтовые буквы
                cout<<letterTest_.name.data<<" ";
                
                //создаем копию тестовой буквы. В копию буквы будут записаны параметры корреляции
                OCRLetter letterTest=letterTest_;
                memset(letterTest.kLine,0,MAX_CLUSTER_LETTER * sizeof(float));
                letterTest.xMatch=match.xCenter;
                letterTest.yMatch=match.yCenter;
                letterTest.scale=match.scale;
                //сравниваем letterTest с графом буквы letterBase

                //проверяем точную корреляцию пары букв
                letterGroupClusterCorrelation(graph_1, letterTest, 1);
                
                //drawLetter(letterTest);
                //return;
                
                //в letterTest.lineMatch записаны номера линий базовой буквыы совпавших с тестовой буквой
                for(int n=0; n < letterTest.matchLineCount; n++){
                    kLineSum[letterTest.lineMatch[n]]++;
                }
                count++;
            }
            cout<<endl;
            printArray(kLineSum, lineCount);
            if(count>1){
                for(int i = 0; i < lineCount; i++){
                    letterMatchMain.kLine[i]=(float)kLineSum[i]/count;
                    letterBase.kLine[i]=letterMatchMain.kLine[i];
                }
            }
        }
    }
    
    if(mode==3){
        //Для букв каждого шрифта вычисляем графческую разность на основе точного сравнения букв
        //точное сравнение учитывает графическую разность букв с точностью до коротких линий и формы линий.
        //результаты графической разности двух букв записываем  в структуру OCRLetterMatch этой пары букв
        cout<<" start build same fonts  different Unicode letters data"<<endl;
        for( int fontIndex=1;fontIndex < 4;fontIndex++){
            for( int index=0;index < fontSize;index++){
                OCRGroupData &group=font.letterGroupData[index];
                OCRLetter &letterBase=font[group.letter[0]];
                if(letterBase.fontType!=fontIndex)continue;
                //if(letterBase.name!="ཆ")continue;
                cout<<index<<" "<<letterBase.name.data<<endl;
                
                //создаем GGraph базовой буквы
                //открываем изображение слога для распознавания
                string pathImg=font.pathArray[letterBase.index].data;
                GBitmap *img = GBitmap::create(pathImg.c_str());
                
                img->crop32();
                img->binarisation64();
                GGraph graph_1(img);
                //img->printToScreenBinary();
                graph_1.graphFocalLine(15);
                //graph_1.printToScreenBinary();
                graph_1.focalPointXTL();
                graph_1.focalLineRecognition();
                graph_1.buildOCRStars();
                //drawGraph(graph_1);
                
                strArray.resize(0);
                OCRBox p;
                p.yLimit0=0;
                p.yLimit1=img->rows();
                strArray.push_back(p);
                img->destroy();
                
                //font.printGroupData(group);
                
                for(int i=1;i < group.size; i++){
                    OCRLetterMatch match=font.letterMatch[group.match[i]];
                    if(match.OCRIndex)continue;  //сравниваем буквы с различным значением Юникод
                    OCRLetter &letterTest_=font[group.letter[i]];
                    if(letterTest_.fontType!=fontIndex)continue;
                    
                    //сравниваем две буквы одного шрифта с различным Юникодом
                    //cout<<"letter:"<<letterBase.name.data<<" letterTest:"<<letterTest_.name.data<<endl;
                
                    //drawLetter(letter);
                    //drawLetterMatch(match);
                    //printCorrelation=1;
                    //создаем копию тестовой буквы. В копию буквы будут записаны параметры корреляции
                    OCRLetter letterTest=letterTest_;
                    memset(letterTest.kLine,0,MAX_CLUSTER_LETTER * sizeof(float));
                    letterTest.xMatch=match.xCenter;
                    letterTest.yMatch=match.yCenter;
                    letterTest.scale=match.scale;
                    //сравниваем letterTest с графом буквы letterBase
                    
                    //проверяем точную корреляцию пары букв
                    letterGroupClusterCorrelation(graph_1, letterTest, 0);
                    
                    //присваиваем линиям найденной графической разности корелляционные веса этих линий из тестовой буквы
                    for (int n=0;n<letterTest.matchLineCount;n++){
                        if(letterTest.lineMatch[n]==0){
                            match.kLine[n]=letterTest.kLine[n];
                        }
                    }
                    match.lineCount=letterTest.lineCount;
                    
                    //drawLetter(letterTest);
                    //return;
                    
                }
                
                
            }
        }
        

    }
    if(mode==4){
        //найденные для эталонных шрифтовых букв корреляционные веса графической общности и графической разнасти
        //присваем всем буквам группы совпадающим по значению Юникод.
    
    }
    
    if(mode==5){
        // вычисляем веса линий графической разности с базовой буквой для букв не совпадающих с значением Юникод базовой буквы
        cout<<" start build different Unicode letters data"<<endl;
        for(int index = 0; index < fontSize; index++){
            OCRGroupData &group=font.letterGroupData[index];
            for(int i = 1; i < group.size; i++){
                if(group.match[i]==80416){
                    cout<<"";
                }
                OCRLetterMatch &letterGroupMatch=font.letterMatch[group.match[i]];
                OCRLetterMatch &letterTestMatch=font.letterMatch[font.letterGroupData[group.letter[i]].match[0]];
                if(letterGroupMatch.OCRIndex)continue;
                //drawLetterByID(letterTest.letterID);
                for(int n=0; n < letterGroupMatch.lineCount; n++){
                    //если линия не нашла соответствия
                    if(!letterGroupMatch.kLine[n]){
                        //считаем ее линию графической разностью и присваеваем ей вес этой линии в тестовой букве
                        letterGroupMatch.kLine[n]=letterTestMatch.kLine[n];
                    }else{
                        letterGroupMatch.kLine[n]=0;
                    }
                }
            }
            
        }

        //анализируем данные групп и фомируем группы в которых все буквы имеют корелляцию с базовой буквой не менее letterCorrelationLimit
        cout<<" start build groups "<<endl;
        font.letterGroupArray.resize(font.letterGroupData.size());
        for(int index = 0; index < fontSize; index++){
            cout<<index<<endl;
            OCRGroupData &groupData=font.letterGroupData[index];
            if(groupData.size>2){
                cout<<" ";
            }
            if(index==7069){
                cout<<"";
            }
            
            OCRGroup &newGroup=font.letterGroupArray[index];
            OCRLetterMatch &letterMainMatch=font.letterMatch[groupData.match[0]];
            letterMainMatch.corr=100;
            OCRLetter &letterBase=font[index];
            
            //записываем в группу базовую букву
            newGroup.letter[0]=index;
            newGroup.match[0].d0=letterMainMatch.matchIndex;
            newGroup.match[0].d1=letterMainMatch.matchIndex;
            
            letterBase.status=1;
            newGroup.size=groupData.size;
            
            //для каждого OCRLetterMatch (letterTest) группы groupData базовой буквы проверяем существует ли прямая и обратная корреляция
            //с базовой буквой
            for(int index1 = 1; index1 < groupData.size; index1++){
                int groupDataLetterIndex=groupData.letter[index1];
                
                //записываем прямую корреляцию
                newGroup.match[index1].d0=groupData.match[index1];
                newGroup.letter[index1]=groupDataLetterIndex;
                //проверяем есть ли обратная корреляция с этой тестовой буквой.
                OCRGroupData &groupTestData=font.letterGroupData[groupDataLetterIndex]; //группа OCRGroupData проверяемой буквы letterTest
                int flag=0;
                for(int index3 = 1; index3 < groupTestData.size; index3++){
                    //проверяем есть ли обратная корреляция letterTest <- letterBase
                    if(groupTestData.letter[index3] == letterMainMatch.baseIndex){
                        //записываем обратную корреляцию
                        newGroup.match[index1].d1=groupTestData.match[index3];  //???
                        flag=1;
                        break;
                    }
                }
                if(!flag)newGroup.match[index1].d1=-1;
            }
            
            //if(index==7069){
            //    font.printGroup(newGroup);
            //}
            
            //проверяем в базе letterGroupData есть ли обратная корреляция letterTest <- letterBase базовой буквы с какой либо буквой базы
            //которая не имеет прямой корреляции с базовой буквой и еще не попала в группу базовой буквы.
            for( int index4=0;index4 < fontSize;index4++){
                if(index==index4)continue;
                OCRGroupData &groupData=font.letterGroupData[index4];
                for(int index3 = 1; index3 < groupData.size; index3++){
                    //проверяем есть ли обратная корреляция letterTest <- letterBase
                    if(groupData.letter[index3] == index){
                        //проверяем есть ли такая буква в группе
                        int testLetterIndex=groupData.letter[0];
                        int flag=0;
                        for(int index5 = 0; index5 < newGroup.size; index5++){
                            if(newGroup.letter[index5]==testLetterIndex){
                                flag=1;
                                break;
                            }
                        }
                        if(flag)continue;
                        //записываем обратную корреляцию
                        //записываем в группу новую букву
                        newGroup.letter[newGroup.size]=testLetterIndex;
                        newGroup.match[newGroup.size].d0=-1;
                        newGroup.match[newGroup.size].d1=groupData.match[index3];
                        newGroup.size++;
                        if(newGroup.size>MAX_LETTER_GROUP1-1){
                            cout<<"need increase MAX_LETTER_GROUP1 2"<<endl;
                            //exit(0);
                            newGroup.size--;
                        }
                    }
                }
            }
        }
        cout<<"all group count:"<<font.letterGroupArray.size()<<endl;
        
        uint letterGroupArraySize=(uint)font.letterGroupArray.size();
        
        int max=0;
        int averageSize;
        int size[MAX_LETTER_GROUP1];
        memset(size, 0, MAX_LETTER_GROUP1*4);
        for( int index=0;index < letterGroupArraySize;index++){
            OCRGroup &group=font.letterGroupArray[index];
            if(group.size>max)max=group.size;
            size[group.size]++;
            averageSize+=group.size;
        }
        for( int i=0;i < 38;i++){
            for( int index=0;index < letterGroupArraySize;index++){
                OCRGroup &group=font.letterGroupArray[index];
                if(group.size==i){
                    OCRLetter &letter=font[group.letter[0]];
                    cout<<"group.size:"<<group.size<<" letter:"<<letter.name.data<<endl;
                    break;
                }
            }
        }
        cout<<"all group count:"<<font.letterGroupArray.size()<<" maxSize:"<<max<<" averageSize:"<<averageSize/letterGroupArraySize<<endl;
        printArray(size, 37);
        exit(0);
    
    }

    
    if(mode==6){
        font.clearLetterGroup2D();
        //анализируем данные групп и фомируем группы в которых все буквы имеют между собой корелляцию не менее letterCorrelationLimit
        cout<<" start build groups "<<endl;
        for(int index = 0; index < fontSize; index++){
            font[index].status=0;
        }
        for(int index = 0; index < fontSize; index++){
            cout<<index<<endl;
            OCRGroupData &groupData=font.letterGroupData[index];
            //if(groupData.size>2){
            //    cout<<1;
            //}
            //для каждой буквы группы проверяем входит ли она в уже существующую группу
            OCRLetterMatch &letterMainMatch=font.letterMatch[groupData.match[0]];
            OCRLetter &letterBase=font[letterMainMatch.baseIndex];
            uint groupIndex=0;
            if(!letterBase.status){
                //создаем новую группу
                OCRGroup2D newGroup_;
                newGroup_.size=0;
                groupIndex=(uint)font.letterGroup2D.size();
                font.letterGroup2D.push_back(newGroup_);
            }
            OCRGroup2D &newGroup=font.letterGroup2D[groupIndex];
            //if(groupIndex==4875){
            //    cout<<"";
            //}
            
            if(!letterBase.status){ //если базовая буква еще не записана в группу
                //записываем в группу базовую букву
                newGroup.letter[newGroup.size]=letterMainMatch.baseIndex;
                //newGroup.match[newGroup.size][newGroup.size]=letterMainMatch.index;

                letterBase.group=groupIndex;
                letterBase.gIndex=newGroup.size;
                letterBase.status=1;
                newGroup.size++;
                if(newGroup.size>MAX_LETTER_GROUP-1){
                    cout<<"need increase MAX_LETTER_GROUP"<<endl;
                    exit(0);
                }
            }
            
            //для каждого OCRLetterMatch (letterTest) группы groupData базовой буквы проверяем существует ли прямая и обратная корреляция
            //со всеми буквами группы newGroup (letterGroup)
            for(int index1 = 1; index1 < groupData.size; index1++){
                int groupDataLetterIndex=font.letterMatch[groupData.match[index1]].baseIndex;
                //OCRLetterMatch &letterTestMatch=font.letterMatch[groupData.match[index1]];
                
                //проверяем есть ли такая тестовая буква в группе
                int flag1=0; //флаг наличия буквы в группе newGroup
                int flag=1;  //флаг прямого и обратного соответствия letterTest и каждой из букв группы
                for(int index2 = 0; index2 < newGroup.size; index2++){
                    if(newGroup.letter[index2]==groupDataLetterIndex){
                        flag1=1;
                        break;
                    }
                    
                }
                if(!flag1){
                    //если такой буквы letterTest еще нет в группе newGroup
                    for(int index2 = 0; index2 < newGroup.size; index2++){
                        //для каждой буквы letterGroup группы newGroup
                        //проверяем существует ли прямая letterGroup <- letterTest  и обратная  letterTest <- letterGroup корреляция letterTest и letterGroup
                        int newGroupLetterIndex=newGroup.letter[index2];
                        //OCRLetter &letter=font[newGroupLetterIndex];
                        
                        //проверяем есть ли корреляция letterGroup <- letterTest
                        OCRGroupData &groupTestData=font.letterGroupData[newGroupLetterIndex]; //группа OCRGroupData проверяемой буквы letterGroup
                        flag=0;
                        for(int index3 = 0; index3 < groupTestData.size; index3++){
                            //проверяем есть ли letterGroup <- letterTest соответствие
                            if(font.letterMatch[groupTestData.match[index3]].baseIndex == groupDataLetterIndex){
                                flag=1;
                                break;
                            }
                        }
                        if(!flag){
                            //если не найдено соответствие letterGroup <- letterTest
                            //это значит что буква letterTest не может принадлежать к группе newGroup
                            break;
                        }else{
                            //если найдено соответствие letterGroup <- letterTest
                            //проверяем соответствие letterTest <- letterGroup
                            OCRGroupData &groupTestData=font.letterGroupData[groupDataLetterIndex]; //группа OCRGroupData проверяемой буквы letterTest
                            flag=0;
                            for(int index3 = 0; index3 < groupTestData.size; index3++){
                                //проверяем есть ли letterGroup <- letterTest соответствие
                                if(font.letterMatch[groupTestData.match[index3]].baseIndex == newGroupLetterIndex){
                                    flag=1;
                                    break;
                                }
                            }
                        }
                        
                    }
                    
                }
                if(flag){
                    //найдено прямое и обратное соответствие letterTest <-> letterGroup
                    //записываем letterTest в группу newGroup
                    OCRLetter &letterTest=font[groupDataLetterIndex];
                    letterTest.status=1;
                    letterTest.group=groupIndex;
                    letterTest.gIndex=newGroup.size;
                    newGroup.letter[newGroup.size]=groupDataLetterIndex;
                    newGroup.size++;
                    if(newGroup.size>MAX_LETTER_GROUP-1){
                        cout<<"need increase MAX_LETTER_GROUP"<<endl;
                        exit(0);
                    }
                    //записываем letterTestMatch в строку базовой буквы в таблице match в newGroup
                    //newGroup.match[letterBase.gIndex][groupDataLetterIndex]=letterTestMatch.matchIndex;
                }
            }
            
        }
        cout<<"all group count:"<<font.letterGroup2D.size()<<" all matchCount:"<<font.letterMatch.size()<<endl;
        
        uint letterGroup2DSize=(uint)font.letterGroup2D.size();
        
        int max=0;
        int averageSize;
        int size[64];
        memset(size, 0, 64*4);
        for( int index=0;index < letterGroup2DSize;index++){
            OCRGroup2D &group=font.letterGroup2D[index];
            if(group.size>max)max=group.size;
            size[group.size]++;
            averageSize+=group.size;
        }
        for( int i=0;i < 38;i++){
            for( int index=0;index < letterGroup2DSize;index++){
                OCRGroup2D &group=font.letterGroup2D[index];
                if(group.size==i){
                    OCRLetter &letter=font[group.letter[0]];
                    cout<<"group.size:"<<group.size<<" letter:"<<letter.name.data<<endl;
                    break;
                }
            }
        }
        cout<<"all group count:"<<letterGroup2DSize<<" maxSize:"<<max<<" averageSize:"<<averageSize/letterGroup2DSize<<endl;
        printArray(size, max);


    }
 
 
}//_______________________________________________________________________________________
    
    
/** @bref Функция создания OCRLetterStack для группы OCRLetter алфавита*/
int GGraphOCR::letterAphabetCorrelation(GGraph &graph, OCRLetter &letterMain, int mode) {
    /**
     Алгоритм май 2021 года
     */
    
    int print = 0;
    
    //if(letterMain.index==3){
    //    print=1;
        //drawGraph(graph);
    //}
    
    vector<OCRLetter> matchVector;
    matchVector.push_back(letterMain);
    
    //letterPrint=1622985229;
    
    //int w = graph.columns();
    //int h = graph.rows();
    uint fontSize = (uint)font.size();
    
    uint graphClusterArraySize = (uint)graph.clusterArray.size();
    OCRCluster *graphClusterArray = graph.clusterArray.dataPtr();
    //draw.printGraphSVG((void*)&graph, "/OSBL/CORRELATION_3"); exit(0);
    
    //таблица перекодировки номера OCRStar в GGraph в номер кластера.
    vector<uint>id2Index;
    id2Index.resize(graph.starArray.size());
    for(int i=0;i<graphClusterArraySize;i++){
        id2Index[graphClusterArray[i].sIndex]=i+1;
    }
    
    vector<vector<int> > textCluster; ///<таблица в которой каждая строка это список OCRCluster текста
    ///< которые соответствуют базовому кластеру по 1,2,3 lookup
    vector<vector<int> > text2Cluster; ///<таблица в которой каждая строка это список базовых кластеров для OCRCluster текста
    vector<vector< vector<pointLine> > >baseID2TextCluster;

    //размечаем весь текст кластерами базы данных, строим таблицу соответствия базовых кластеров базы и текста
    //и обратную таблицу соответствия OCRStar текста и базовых кластеров
    //для каждой строки текста в массиве baseID2TextCluster создаем реестр соответствия ID номера базового кластера и координат кластера текста
    setBaseClasterInText(graph,textCluster,text2Cluster,baseID2TextCluster);
    
    /*
    //отладочная версия регистров
    //Создаем регистр поиска. В регистре отмечаются кластеры текста которые распознаны одной буквой базы.
    //Проверка регистра позволяет не искать букву дважды на одном месте.
    vector<char> textRegLetter(graphClusterArraySize);
    //Создаем основной регистр поиска. В регистре отмечаются кластеры текста которые сответствуют
    //наилучшей гипотезе буквы всей базы распознаваемых букв.
    //Проверка регистра позволяет не искать дважды гипотезы букв базы на одном месте текста.
    vector<char> textReg(graphClusterArraySize);
    //регистр занятости линий текста
    vector<char> mainTextReg(graph.focalLine.size());
    //регистр соответствия линий буквы
    vector<short> letterLineReg(MAX_CLUSTER_LETTER);
    */
    //быстрая версия регистров
    
     //Создаем регистр поиска. В регистре отмечаются кластеры текста которые распознанны одной буквой базы.
     //Проверка регистра позволяет не искать букву дважды на одном месте.
     char textRegLetter[graphClusterArraySize];
     memset(textRegLetter, 0, graphClusterArraySize);
     
     //Создаем основной регистр поиска. В регистре отмечаются кластеры текста которые сответствуют
     //наилучшей гипотезе буквы всей базы распознаваемых букв.
     //Проверка регистра позволяет не искать дважды гипотезы букв базы на одном месте текста.
     char textReg[graphClusterArraySize];
     memset(textReg, 0, graphClusterArraySize);
     
     //регистр занятости линий текста
     char mainTextReg[graph.focalLine.size()];
     memset(mainTextReg, 0, graph.focalLine.size());
     //регистр соответствия линий буквы
     short letterLineReg[MAX_CLUSTER_LETTER];
    
    
    DR("Start text recognition"<<endl;)
    int letterCorrelationCount1=0;
    
    
    //начиная с буквы алфавита с наибольшим количеством кластеров
    for( int index=0;index < fontSize;index++){
        int indexLetterT = font.fontSortArray[index].x1;
        //копируем букву из шрифта. В букву записываем результаты сравнения с текстом
        OCRLetter letterTest = font[indexLetterT];
        if(letterTest.letterID==letterMain.letterID)continue;
        if(letterTest.OCRStatus==0)continue; //при построении групп исключаем повторы одинаковых букв
        
        //if(mode==1){
        //    if(letterMain.name != letterTest.name)continue;
        //}else{
        //    if(letterMain.name == letterTest.name)continue;
        //}

        letterTest.correlation=0;

        letterTest.matchCount=letterTest.clusterCount;
        int letterMatchCount=letterTest.matchCount;
        
        string str2=letterTest.name.data;
        
        if(letterPrint > 1 && letterTest.letterID!=letterPrint)continue;
        //if(index%500 == 0)cout<<" /"<<index<<flush;
        
        //список базовых кластеров этой буквы находится в letterTest.match
        //проверяем количество совпадений базовых кластеров эталонной буквы и кластеров для каждой гипотезы буквы
        //DR("start test "<<letterTest.name.data<< " cluster count:"<<(int)v[index].d1<<" id:"<<letterTest.letterID<<endl);
        //обнуляем регистр поиска
        memset((void*)&textRegLetter[0],0, graphClusterArraySize);
        int limitClusterSearch=letterTest.matchCount;
        if(limitClusterSearch>3)limitClusterSearch=3;
        
        
        for (int index3 = 0; index3 < limitClusterSearch; index3++) {
            //необходимо начать проверку с первого кластера как наиболее устойчивого
            //проверяем все базовые кластеры соответствующие первому кластеру буквы по геометрии
            
            ClusterMatch &letterMainMatch = letterTest.match[index3];
            //cout<<"match1.clusterID:"<<match1.clusterID<<endl;
            //print =0;
            string str1=letterTest.name.data;
            if( letterTest.letterID==letterPrint){
                print =1;
            }
            
            //проверяем есть ли такой кластер в массиве textCluster
            //этот массив содержит адреса всех кластеров текста соответствующих этому базовому кластеру
            //индекс базового кластера - match1.clusterID
            //начиная с каждого совпадения проверяем наличие остальных базовых кластеров в ближайшем окружении
            uint size1=(uint)textCluster[letterMainMatch.baseClusterID].size();
            
            if(size1){
                //если такой кластер есть в тексте, для каждого соответствующего кластера текста
                //необходимо проверить есть ли в окружении этого кластера другие кластеры буквы
                //для этого формируем список кластеров окружения
                //int correlationMatchStartSumLen1=letterTest.match[index3].sumLenStar;
                //print=0; printCorrelation=0;
                if(letterTest.letterID==letterPrint){
                    cout<<endl;
                    print=1; printCorrelation=1;
                }
                
                for (int index4 = 0; index4 < 1; index4++) {
                    //для каждого кластера текста совпадающего с базовым кластером проверяемого кластера буквы
                    //проверяем есть ли такой кластер в регистре проверенных кластеров
                    int index_1=textCluster[letterMainMatch.baseClusterID][index4];
                    if(textReg[index_1])continue; //кластер уже распознан
                    OCRCluster &clusterText=graphClusterArray[index_1];
                    if(textRegLetter[clusterText.index])continue; //кластер уже проверен на соответствие этой букве
                    //проверяем заняты ли линии текста этого кластера. Линии могут быть резервированны ранее распознанными буквами
                    int flagCluster=0;
                    for(int clusterLineIndex=0;clusterLineIndex<clusterText.lineCount;clusterLineIndex++){
                        if(mainTextReg[clusterText.line[clusterLineIndex].parentID]){
                            flagCluster=1;
                            break;
                        }
                    }
                    if(flagCluster)continue;
                    //записываем данные о соответствии кластера текст и кластера буквы
                    letterMainMatch.textClusterID = index_1;
                    clusterText.corr = 100;
                    clusterText.matchIndex = index3; //индекс в letter.match
                    //clusterText.matchID = match1.clusterIndex; // индекс кластера буквы
                    clusterText.baseClusterID = letterMainMatch.baseClusterID; // индекс базового кластера буквы
                    if(letterMainMatch.lineCount){
                        float scale1=(float)clusterText.sumLen1 / letterMainMatch.sumLen1;
                        float scale2=(float)clusterText.sumLenA / letterMainMatch.sumLenA;
                        letterMainMatch.scale = (float)(scale1 + scale2)/2;
                        
                    }else{
                        letterMainMatch.scale = 1;
                    }
                    
                    //обнуляем соответствия кластеров буквы и суммируем sumLen1 кластеров
                    letterTest.matchLineSum=0;
                    for (int letterMatchIndex = 0; letterMatchIndex < letterMatchCount; letterMatchIndex++) {
                        letterTest.match[letterMatchIndex].corr=0;
                    }
                    //записываем распознанные линии буквы в реестр буквы
                    memset(letterLineReg,0,MAX_CLUSTER_LETTER * 2);
                    for(int n=0;n<letterMainMatch.lineCount; n++){
                        letterLineReg[letterMainMatch.line[n]]=letterMainMatch.len[n];
                    }
                    letterMainMatch.corr=100; //записываем корреляцию стартового кластера
                    
                    if(letterTest.clusterCount>1){
                        letterTest.correlation=0;
                        //определяем к какой строке текста относится этот кластер текста
                        int lineIndex=0;
                        int yTextCluster=clusterText.cPoint.y;
                        for(int n=0;n<strArray.size();n++){
                            OCRBox &p=strArray[n];
                            if(yTextCluster > p.yLimit0 && yTextCluster < p.yLimit1){
                                lineIndex=n;
                                break;
                            }
                        }
                        //читаем массив соответствия ID номеров базовых кластеров и кластеров текста
                        vector<vector<pointLine> > &baseID2LineCluster=baseID2TextCluster[lineIndex];
                        
                        //находим все базовые кластеры буквы по заданному лимиту координат
                        int x_start=clusterText.cPoint.x - letterTest.letterW * letterMainMatch.scale;
                        int x_end=clusterText.cPoint.x + letterTest.letterW * letterMainMatch.scale;
                        if(x_start<0)x_start=0;
                        
                        //для каждого базового кластера буквы
                        for (int letterMatchIndex = 0; letterMatchIndex < letterTest.clusterCount; letterMatchIndex++) {
                            if(letterMatchIndex == index3) continue; // опорный кластер буквы уже учтен
                            ClusterMatch &match1 = letterTest.match[letterMatchIndex];
                            
                            uint allPointCount=(uint)baseID2LineCluster[match1.baseClusterID].size();
                            //DR("   letter sIndex:" << match1.sIndex << " allPointCount:" << allPointCount << endl);
                            
                            //находим ID номер фокальной точки и проверяем корреляцию
                            
                            for (int i = 0; i < allPointCount; i++) {
                                pointLine &p = baseID2LineCluster[match1.baseClusterID][i];
                                //проверяем лимит координат кластера текста
                                if(p.x<x_start || p.x > x_end) continue;
                                int idCluster=p.x1;
                                //проверяем есть ли такой кластер в регистре проверенных кластеров текста
                                if(textReg[idCluster])continue;
                                if(textRegLetter[idCluster])continue;
                                //проверяем есть ли свободные  линии этого кластера текста
                                OCRCluster &clusterText1=graphClusterArray[idCluster];
                                
                                //int flagCluster=0;
                                //for(int clusterLineIndex=0;clusterLineIndex<clusterText1.lineCount;clusterLineIndex++){
                                //    if(!mainTextReg[clusterText1.line[clusterLineIndex].parentID]){
                                //        flagCluster=1;
                                //        break;
                                //    }
                                //}
                                //if(!flagCluster)continue;
                                
                                //для найденного кластера текста
                                for(int n=0;n<match1.lineCount; n++){
                                    letterLineReg[match1.line[n]]=match1.len[n];
                                }
                                
                                //DR("_/_text:"<<clusterText1.sIndex<<" letter:"<<(int)match1.sIndex<<" /_base:"<<match1.baseClusterID<<endl);
                                if(print){
                                    //int corr=clusterCorrelation(clusterText1, font.baseClusterArray[match1.baseClusterID]);
                                    //int corr=clusterCorrelation(clusterText1, font.clusterArray[letterTest.cluster[letterMatchIndex]]);
                                    //if(print)draw.printClusterSVG((void*)&font.clusterArray[letterTest.cluster[letterMatchIndex]],"/OSBL/CORRELATION_0/");
                                    //if(print)draw.printClusterSVG((void*)&font.baseClusterArray[match1.baseClusterID],"/OSBL/CORRELATION_0/");
                                    //cout<<corr<<endl;
                                }
                                
                                //записываем данные о соответствии кластера текста  в соответствующий OCRMatch буквы
                                clusterText1.baseClusterID = match1.baseClusterID; // индекс базового кластера
                                if(match1.lineCount){
                                    float scale1=(float)clusterText1.sumLen1 / match1.sumLen1;
                                    float scale2=(float)clusterText1.sumLenA / match1.sumLenA;
                                    match1.scale = (float)(scale1 + scale2)/2;
                                }else{
                                    match1.scale=1;
                                }
                                
                                match1.corr=50;
                                //записываем соответствие кластера текста и буквы. На этом этапе точное соответствие еще не установлено.
                                //индекс matchIndex используем только для соответствия первого кластера и комплексных кластеров
                                match1.textClusterID=idCluster;
                            }
                        }
                        
                        int correlationSumLen1=0;
                        for(int n=0;n<letterTest.lineCount; n++){
                            correlationSumLen1+=letterLineReg[n];
                        }
                        
                        string str1=letterTest.name.data;
                        if(str1==strTest || letterTest.letterID==letterPrint)DR_("\ncorrelationSumLen1:"<<correlationSumLen1<<" letterTest.lineSum:"<<letterTest.lineSum<<" letterTest:"<<letterTest.name.data<<" ID:"<<letterTest.letterID<<endl);
                        
                        if(correlationSumLen1>letterTest.lineSum*correlationLimit){
                            //DR("index:"<<index<<" letterTest:"<<letterTest.name.data<<" id:"<<letterTest.letterID<<" index:"<<letterTest.index<<" lineCount:"<<letterTest.lineCount);
                            //DR(" clusterCount:"<<letterTest.clusterCount<<" matchCount:"<<letterTest.matchCount<<" countCorrelation:"<<countCorrelation<<endl);
                            printCorrelation=0;
                            if(letterPrint && letterTest.letterID == letterPrint){ //1569369846
                                print=1; printCorrelation=1;
                            }
                            letterCorrelationCount1++;
                            if(letterCorrelationCount1==11){
                                cout<<1;
                            }
                            //if(print && letterTest.letterID==1569371864){
                            //    cout<<"";
                            //    printCorrelation=1;
                            //    drawLetter(letterTest);
                            //}
                            int modeCorrelation=1;
                            if(letterMain.name!=letterTest.name){
                                modeCorrelation=0;
                            }
                            
                            
                            letterTextClusterCorrelation(graph, letterTest, modeCorrelation);
                            printCorrelation=0;
                            //if(str1=="ཇ")DR(" @/ /________________________ letter:"<<letterTest.name.data<<" corr:"<<letterTest.correlation<<endl);
                        }
                    }else{
                        letterTest.correlation = 100;
                        letterTest.xMatch=clusterText.cPoint.x;
                        letterTest.yMatch=clusterText.cPoint.y;
                        if(!letterTest.letterW)letterTest.letterW=10;
                        if(!letterTest.letterH)letterTest.letterH=10;
                        if(!letterTest.scale)letterTest.scale=1;
                        if(mode==1){
                            //записываем в букву совпадающие линии текста
                            int size1=clusterText.lineCount;
                            letterTest.matchLineCount=size1;
                            for(int i=0;i<size1;i++){
                                letterTest.lineMatch[i]=clusterText.line[i].index; //записываем в букву ID номера найденых линии текста
                            }
                        }else{
                            //записываем в букву совпадающие линии буквы
                            int size1=letterTest.lineCount;
                            for(int i=0;i<size1;i++){
                                letterTest.lineMatch[i] = i; //записываем в букву ID номера найденых линии буквы
                            }
                        }
                    }
                    //записываем гипотезу буквы
                    
                    
                    //if(letterTest.correlation>letterCorrelationLimit){
                    if(letterTest.correlation>letterCorrelationLimit){
                        DR(" @/+/________________________ letter:"<<letterTest.name.data<<" id:"<<letterTest.letterID<<" corr:"<<letterTest.correlation<<endl);
                        /*
                        //отмечаем что буква проверена на этом месте
                        for (int n = 0; n < letterMatchCount; n++) {
                            ClusterMatch &clusterMatch=letterTest.match[n];
                            //если буква найдена в тексте, исключаем соответствующие кластеры текста.
                            //регистр исключает повторный поиск этой буквы в этом месте
                            if(clusterMatch.corr==100){
                                textRegLetter[clusterMatch.textClusterID]=1;
                            }
                        }
                        */ 
                        //проверяем величину обратной корреляции
                        //описание обратной корреляции в функции backwardCorrelation
                        int tCount=letterMain.lineSum; // - количество кластеров области текста,
                        int nOn=letterTest.matchLineSum; // - длина линий текста соответствующая найденным кластерам буквы,
                        letterTest.correlationNew=(nOn*100/tCount + letterTest.correlation)/2;
 
                        if(letterTest.correlationNew>letterCorrelationLimit){
                            //добавляем новую букву в группу
                            DR("     add letter: "<<letterTest.name.data<<endl);
                            if(matchVector.size() > MAX_LETTER_GROUP - 1){
                                cout<<"matchVector.size() > MAX_LETTER_GROUP:"<<matchVector.size()<<"/"<<MAX_LETTER_GROUP<<endl;
                                exit(0);
                            }else{
                                matchVector.push_back(letterTest);
                            }
                            index3 = limitClusterSearch;
                            break;
                        }
                    }
                }//for (int index4 = 0;
            }//for (int index3 = 0;
            if(letterTest.lineCount<letterMain.lineCount * 0.45 || index == fontSize - 1){
                index = fontSize;
                break;
            }
        }//for (int index4 = 0; index4 < size1; index4++)
    }
    //cout<<"start build letter stack. matchVector.size():"<<matchVector.size()<<endl;
    //cout<<"matchVector.size():"<<matchVector.size()<<endl;
    
    if(mode==1){
        buildBaseOCRLetterGroup(matchVector);
    }else{
        if(matchVector.size()>1)buildOCRLetterGroup(matchVector);
    }

    
    return 0;
} //____________________________________________________________________________


    
    
    
//** @bref создает сортированную таблицу кластеров OCR шрифта */
void GGraphOCR::buildOCRFontClusterTable(int mode) {
	/**
	 В базе данных выполняется сортировка всех кластеров всех букв по частоте встречаемости
	 Каждый кластер каждой буквы сравнивается с каждым кластером каждой буквы по 0,1,2 lookup
	 для каждого кластера формируется таблица букв в которых он встречается.
	 В таблицу записывается кластер в котором есть следующая информация
		id номер буквы в которой размещен кластер,
		id номер кластера в букве в которой размещен кластер,
		список id номеров букв в которой найден кластер,
		id номера OCRStar соответствющих кластеру.
		размер буквы в которой найден данный кластер.
	 Каждая строка таблицы сортируется по признаку наибольшего габарита буквы в которой найден данный кластер.
	Для повышения быстродействия поиска базовых кластеров по этому же алгоритму, строится таблица проверки кольцевых регистров углов
	и таблица масок кластеров. Эти таблицы позволяют сократить количество проверок кластеров по 2 lookup
    если mode==3 таблицы кластеров записываются только новые буквы шрифта
	*/
	
	
	int print = 0;

	//стираем старые и создаем новые таблицы
	string str;
    if(mode!=3){
        font.clearClusterTable();
    }
	
	GStr<OCRCluster> &clusterArray = font.clusterArray;             // массив всех кластеров всех букв
	GStr<OCRCluster> &baseСlusterArray = font.baseClusterArray;     // массив всех базовых кластеров всех букв
    //GStr<uint> &indexArray = font.indexArray;                       // таблица соответствия letter.index - letterID
    GVector *fontRegisterTable = font.fontRegisterTable;            // таблица кольцевых регистров углов базовых кластеров,
                                                                    // строка таблицы заполняется информацией о базовых кластерах
                                                                    // в которых совпадает кольцевой регистр углов базового кластера
    GStr<OCRRegister> &fontRegisterGStr = font.fontRegisterGStr;    // массив кольцевых регистров базовых кластеров
    GVector *fontMaskTable = font.fontMaskTable;                    // таблица масок базовых кластеров, строка таблицы заполняется
                                                                    // информацией о базовых кластерах в которых совпадает маска базового кластера

	GStr<OCRMask> &fontMaskGStr = font.fontMaskGStr;                // массив масок базовых кластеров
    GVector *fontClusterTable = font.fontClusterTable;              // таблица базовых кластеров строка таблицы заполняется
                                                                    // информацией о буквах в которых найден базовый кластер этой строки

	//создаем таблицу проверенных кластеров
	int sizeFont = (int)font.size();
	//создаем индексы в памяти. После заполнения индексы компактно запишем на диск
	//массив регистрирующий проверку кластера букв шрифта
	uchar *clusterReady = (uchar *)calloc(sizeFont * MAX_CLUSTER_LETTER, sizeof(char));
	//создаем временный массив масок базовых кластеров.
	vector<OCRMask>maskArray;
	//создаем временный массив кольцевых регистров базовых кластеров.
	vector<OCRRegister>registerArray;
	
	int allClusterCount=0;
	for (uint index = 0; index < sizeFont; index++) {
		OCRLetter &letter=font[index];
        //if(letter.letterID==1569369434){
        //    cout<<1;
        //}
        letter.matchCount=letter.clusterCount;
        if(mode==3 && letter.status)continue;
        letter.status=1;
        
        if(mode==3){
            
            //в режиме добавления в таблицу кластеров новых букв проверяем каждый кластер буквы на соответствие базовым кластерам шрифта
            uint size12=(uint)baseСlusterArray.size();
             for(int baseClasterIndex=0;baseClasterIndex<size12;baseClasterIndex++){
                 OCRCluster &baseClaster=baseСlusterArray[baseClasterIndex];
                 
                 for (int clusterIndex = 0; clusterIndex < letter.clusterCount; clusterIndex++) { //для каждого кластера буквы
                     if(clusterReady[index * MAX_CLUSTER_LETTER + clusterIndex]) continue;
                     OCRCluster &cluster = clusterArray[letter.cluster[clusterIndex]];
                     
                     //выполняем сравнение двух кластеров типа O_POINT
                     if (cluster.type == O_POINT && baseClaster.type == O_POINT) { //?
                         //при высокой корреляции считаем эти кластеры одинаковыми
                         //и заносим соответствие в таблицу кластеров
                         ClusterMatch clusterM;
                         clusterM.letterIndex = index;
                         clusterM.matchIndex = index;  //номер буквы в которой найден базовый кластер
                         clusterM.clusterIndex = clusterIndex;  //индекс кластера в букве
                         clusterM.baseClusterID = baseClasterIndex; //индекс базового кластера в массиве базовых кластеров
                         //clusterM.letterClusterID = letterTest.cluster[n];  //индекс кластера буквы в массиве кластеров
                         clusterM.clusterCount = letter.clusterCount;
                         clusterM.sIndex = cluster.sIndex; //номер OCRStar в букве
                         clusterM.cPoint = cluster.cPoint;
                         //записываем номера линий кластера
                         for(int i = 0; i < cluster.lineCount; i++){
                             clusterM.line[i] = cluster.line[i].parentID; //в линии кластера parentID сохраняет индекс линии в графе
                             clusterM.len[i] = cluster.line[i].lenA;
                         }
                         clusterM.lineCount = cluster.lineCount;
                         //отмечаем кластер как проверенный в таблице проверенных кластеров
                         clusterReady[index * MAX_CLUSTER_LETTER + clusterIndex] = 255;
                         //clusterLine.push_back(index1);
                         //для последующего определения масштаба записываем сумму длин фокальных линий кластера буквы
                         clusterM.sumLen1 = cluster.sumLen1;
                         clusterM.sumLenA = cluster.sumLenA;
                         clusterM.sumLenStar = cluster.star[0].sumLenA;
                         
                         //записываем базовый кластер в букву
                         letter.match[clusterIndex] = clusterM;
                         continue;
                     }
                     //выполняем сравнение двух кластеров
                     int correlation = clusterCorrelation(baseClaster, cluster);
                     int correlation_ = clusterCorrelation(cluster, baseClaster);
                     if(correlation_<clusterCorrelationLimit || correlation < clusterCorrelationLimit) continue;
                     //при высокой корреляции считаем эти кластеры одинаковыми
                     //и заносим соответствие в таблицу кластеров
                     //при высокой корреляции считаем эти кластеры одинаковыми
                     //и заносим соответствие в таблицу кластеров
                     ClusterMatch clusterM;
                     clusterM.letterIndex = index;
                     clusterM.matchIndex = index;  //номер буквы в которой найден базовый кластер
                     clusterM.clusterIndex = clusterIndex;  //индекс кластера в букве
                     clusterM.baseClusterID = baseClasterIndex; //индекс базового кластера в массиве базовых кластеров
                     //clusterM.letterClusterID = letterTest.cluster[n];  //индекс кластера буквы в массиве кластеров
                     clusterM.clusterCount = letter.clusterCount;
                     clusterM.sIndex = cluster.sIndex; //номер OCRStar в букве
                     clusterM.cPoint = cluster.cPoint;
                     //записываем номера линий кластера
                     for(int i = 0; i < cluster.lineCount; i++){
                         clusterM.line[i] = cluster.line[i].parentID; //в линии кластера parentID сохраняет индекс линии в графе
                         clusterM.len[i] = cluster.line[i].lenA;
                     }
                     clusterM.lineCount = cluster.lineCount;
                     //отмечаем кластер как проверенный в таблице проверенных кластеров
                     clusterReady[index * MAX_CLUSTER_LETTER + clusterIndex] = 255;
                     //clusterLine.push_back(index1);
                     //для последующего определения масштаба записываем сумму длин фокальных линий кластера буквы
                     clusterM.sumLen1 = cluster.sumLen1;
                     clusterM.sumLenA = cluster.sumLenA;
                     clusterM.sumLenStar = cluster.star[0].sumLenA;
                     //записываем базовый кластер в букву
                     letter.match[clusterIndex] = clusterM;
                     continue;
                 }
            }
        }
        
        
		cout << index << "  clusters:" << letter.clusterCount << endl;
		for (int clusterIndex = 0; clusterIndex < letter.clusterCount; clusterIndex++) { //для каждого кластера буквы
			allClusterCount++;
			//if(allClusterCount==60000){
			//	index = sizeFont;
			//	break;
			//}
			if(clusterReady[index * MAX_CLUSTER_LETTER + clusterIndex])continue;
			OCRCluster cluster = clusterArray[letter.cluster[clusterIndex]];
			cluster.index = (uint)baseСlusterArray.size();
			baseСlusterArray.push_back(cluster);
			vector<uint> clusterLine;

			//записываем базовый кластер в таблицу базовых кластеров
			ClusterMatch baseCluster;
			baseCluster.letterIndex = index;
			baseCluster.matchIndex = index;
			baseCluster.baseClusterID = cluster.index;
			baseCluster.clusterIndex = clusterIndex; //записываем индекс OCRStar кластера в букве в базовый кластер
			baseCluster.sIndex = cluster.sIndex;
			baseCluster.clusterCount = letter.clusterCount;
			baseCluster.cPoint = cluster.cPoint;
			//записываем номера линий кластера
			for(int i = 0; i < cluster.lineCount; i++){
				baseCluster.line[i] = cluster.line[i].parentID; //в линии кластера parentID сохраняет индекс линии в графе
				baseCluster.len[i] = cluster.line[i].lenA;
			}
			baseCluster.lineCount = cluster.lineCount;
			clusterLine.push_back(index);
            //для последующего определения масштаба записываем сумму длин фокальных линий кластера буквы
            baseCluster.sumLen1 = cluster.sumLen1;
            baseCluster.sumLenA = cluster.sumLenA;
            baseCluster.sumLenStar = cluster.star[0].sumLenA;
			//записываем базовый кластер в букву
			letter.match[clusterIndex] = baseCluster;

						
			//резервируем кластер
			clusterReady[index * MAX_CLUSTER_LETTER + clusterIndex] = 255;

			// if(baseCluster.clusterID==1098){
			//    print=1;
			//}
			OCRMask mask;
			OCRRegister reg;
			
			//записываем маску кластера во временный массив масок
			//mask=cluster.maskC;
			//mask.status=1;
			//mask.ID=baseCluster.baseClusterID;
			//maskArray.push_back(mask);

			//записываем кольцевой регистр во временный массив  регистров
			reg=cluster.star[0].reg;
			reg.status=1;
			reg.ID=mask.ID;
			registerArray.push_back(reg);
		

			//для каждого еще не классифицированного кластера каждой буквы находим все буквы в которых распознается данный базовый кластер.
			//в таблице базовых кластеров по индексу базового кластера записывем список букв в которых есть данный базовый кластер
            //проверку начинаем с этой буквы шрифта

			for (int index1 = index; index1 < sizeFont; index1++) {
				//в букве один и тот же кластер может соответствовать 
				//нескольким базовым кластерам, поэтому проверяем все буквы на соответствие этому базовому кластеру, его маске и регистру
				OCRLetter &letterTest=font[index1];
				
				for (int n = 0; n < letterTest.clusterCount; n++) {
					if(clusterReady[index1 * MAX_CLUSTER_LETTER + n] )continue;
					OCRCluster &clusterT = clusterArray[letterTest.cluster[n]];

					//выполняем сравнение двух кластеров
					if (cluster.type == O_POINT && clusterT.type == O_POINT) { //?
						//при высокой корреляции считаем эти кластеры одинаковыми
						//и заносим соответствие в таблицу кластеров
						ClusterMatch clusterM;
						clusterM.letterIndex = index;
						clusterM.matchIndex = index1;  //номер буквы в которой найден базовый кластер
						clusterM.clusterIndex = n;  //индекс кластера в проверяемой букве
						clusterM.baseClusterID = baseCluster.baseClusterID; //индекс базового кластера в массиве базовых кластеров
                        clusterM.letterClusterID = letterTest.cluster[n];  //индекс кластера буквы в массиве кластеров
						clusterM.clusterCount = letterTest.clusterCount;
						clusterM.sIndex = clusterT.sIndex; //номер OCRStar в букве
						clusterM.cPoint = clusterT.cPoint;
						//записываем номера линий кластера
						for(int i = 0; i < clusterT.lineCount; i++){
							clusterM.line[i] = clusterT.line[i].parentID; //в линии кластера parentID сохраняет индекс линии в графе
							clusterM.len[i] = clusterT.line[i].lenA;
						}	
						clusterM.lineCount = clusterT.lineCount;
						//отмечаем кластер как проверенный в таблице проверенных кластеров
						clusterReady[index1 * MAX_CLUSTER_LETTER + n] = 255;
						clusterLine.push_back(index1);
                        //для последующего определения масштаба записываем сумму длин фокальных линий кластера буквы
                        clusterM.sumLen1 = clusterT.sumLen1;
                        clusterM.sumLenA = clusterT.sumLenA;
                        clusterM.sumLenStar = clusterT.star[0].sumLenA;
						//записываем базовый кластер в букву
						letterTest.match[n] = clusterM;
						continue;
					}
					//проверяем корреляцию кольцевого регистра по lookup1

                    //if(letterTest.letterID==1619940683 && baseCluster.baseClusterID==413 && clusterT.sIndex==2){
                    //    cout<<1;
                    //    draw.printClusterSVG((void*)&cluster,"/OSBL/CORRELATION_3/");
                    //    draw.printClusterSVG((void*)&clusterT,"/OSBL/CORRELATION_3/");
                    //}
					
					int correlation = clusterCorrelation(clusterT, cluster);
                    int correlation_ = clusterCorrelation(cluster, clusterT);
                    if(correlation_<clusterCorrelationLimit || correlation < clusterCorrelationLimit) continue;
                    //при высокой корреляции считаем эти кластеры одинаковыми
                    //и заносим соответствие в таблицу кластеров
                    ClusterMatch clusterM;
                    clusterM.letterIndex = index;
                    clusterM.matchIndex = index1;
                    clusterM.clusterIndex = n; //индекс кластера в проверяемой букве
                    clusterM.baseClusterID = baseCluster.baseClusterID; //индекс базового кластера в массиве базовых кластеров
                    clusterM.letterClusterID = letterTest.cluster[n];  //индекс кластера буквы в массиве кластеров
                    clusterM.clusterCount = letterTest.clusterCount;
                    clusterM.sIndex = clusterT.sIndex; //номер OCRStar в букве;
                    clusterM.cPoint = clusterT.cPoint;
                    //записываем номера линий кластера
                    for(int i = 0; i < clusterT.lineCount; i++){
                        clusterM.line[i] = clusterT.line[i].parentID;  //в линии кластера parentID сохраняет индекс линии в графе
                        clusterM.len[i] = clusterT.line[i].lenA;
                    }	
                    clusterM.lineCount = clusterT.lineCount;
                    //отмечаем кластер как проверенный в таблице проверенных кластеров
                    clusterReady[index1 * MAX_CLUSTER_LETTER + n] = 255;
                    clusterLine.push_back(index1);
                    //для последующего определения масштаба записываем сумму длин фокальных линий кластера буквы
                    clusterM.sumLen1 = clusterT.sumLen1;
                    clusterM.sumLenA = clusterT.sumLenA;
                    clusterM.sumLenStar = clusterT.star[0].sumLenA;
                    //записываем базовый кластер в букву
                    letterTest.match[n] = clusterM;
					
				}
			}
			if (print) {
				for (int n = 0; n < clusterLine.size(); n++) { cout << " match:" << clusterLine[n]; }
				cout << endl;
			}

			int cSize = (int)clusterLine.size();
			cout << "       " << cSize << " clusters" << endl;
			//записываем строку таблицы кластеров (индексы букв в которых есть этот базовый кластер)
			fontClusterTable->push_back((char *)&clusterLine[0], sizeof(int) * cSize);
		}
        ///cout << "index:" << index << "  clusters:" << letter.clusterCount << " match:" << letter.matchCount<< endl;
        //if(letter.clusterCount> letter.matchCount){
          //  cout<<1;
        //}

	}
	cout << "baseClusterCount:" << baseСlusterArray.size() << " allClusterCount:"<< allClusterCount << endl;
	
    if(mode!=3){
    
        //после создания таблицы базовых кластеров создаем поисковые индексы к этой таблице.
        //поисковые индексы позволяют примерно на два порядка сократить 
        //количество проверок кластеров по  lookup2 при распознавании базовых кластеров
        
        //создаем индекс который содержит ID номера всех базовых кластеров которые соответствуют данной маске.
        vector<vector<uint> > fontMaskTable_;
        fontMaskTable_.resize(fontClusterTable->size());
        uint maskCount=(uint)maskArray.size();
        uint baseMaskCount=0;
        
        for(int i=0;i<maskCount;i++){
            OCRMask &mask=maskArray[i];
            if(!mask.status)continue;
            //записываем базовую маску в массив
            fontMaskGStr.push_back(mask);
            //проверяем все маски базовых кластеров на совпадение
            for(int n=0;n<maskCount;n++){
                OCRMask &maskT=maskArray[n];
                if(!maskT.status)continue;
                //проверяем корреляцию масок
                int corr=clusterMaskCorrelation(maskT,mask);
                if(corr==100){
                    //записываем маску в таблицу индекса масок
                    fontMaskTable_[baseMaskCount].push_back(n);
                    maskT.status=0;
                }
            }
            baseMaskCount++;
        }
        cout<<" baseMaskCount:"<<baseMaskCount<<endl;
        
        //создаем индекс который содержит ID номера всех масок которые соответствуют данному регистру.
        vector<vector<uint> > fontRegisterTable_;
        fontRegisterTable_.resize(fontClusterTable->size());
        uint regCount=(uint)registerArray.size();
        uint baseRegisterCount=0;
        
        for(int i=0;i<regCount;i++){
            OCRRegister &reg=registerArray[i];
            if(!reg.status)continue;
            //cout<<i<<bitset<64>(reg.reg0_0)<<endl;
            //записываем базовую маску в массив
            fontRegisterGStr.push_back(reg);
            reg.status=0;
            //проверяем все регистры  массива на совпадение
            for(int n=i;n<regCount;n++){
                OCRRegister &regT=registerArray[n];
                if(!regT.status)continue;
                //проверяем корреляцию регистров
                int correlation = OCRStarCorrelationLookup1(regT, reg);
                if (correlation==100) {
                    //записываем регистр в таблицу идекса регистров
                    fontRegisterTable_[baseRegisterCount].push_back(n);
                    regT.status=0;
                    //cout<<"   "<<n<<"  "<<bitset<64>(regT.reg0_1)<<endl;
                    
                }
            }
            baseRegisterCount++;
        }
        cout<<" baseRegisterCount:"<<baseRegisterCount<<endl;
        cout<<" sizeFont:"<<sizeFont<<endl;
        //сохраняем индексы в базе данных
        for(int i=0;i<baseMaskCount;i++){
            vector<uint> &v=fontMaskTable_[i];
            fontMaskTable->push_back((char*)&v[0],v.size()*4);
        }
        for(int i=0;i<baseRegisterCount;i++){
            vector<uint> &v=fontRegisterTable_[i];
            fontRegisterTable->push_back((char*)&v[0],v.size()*4);
        }
    }

} //_____________________________________________________________________________________________	
	
/** @bref Функция определяет величину корреляции каждой буквы OCR шрифта со всеми другими буквами */
void GGraphOCR::buildOCRFontCorrelation(cstr path) {
    vector<string> fileList;
    readDirectoryToArray(fileList, path, "img");
    uint fontSize=(uint)font.size();
    grammarMode=OCR_FONT_CORRELATION;
    
    fontReg.resize(fontSize);
    
    int mode;
    
    //mode=1; // проверка наличия букв схожих по корелляции и совпадающих по Юникоду
    mode=2; // проверка наличия неверно распознанных букв (включая в эталоны саму эту букву)

    if(mode==2)run("echo \"\" > /OSBL/___1.txt");
    
    for(int i=0;i<fileList.size();i++){
        
        string str=fileList[i];
        string pathImg=str;
        str = substr((int)str.rfind("/") + 1, (int)str.size(), str);
        str = str_replace(".png", "", str);
        int letterTestIndex=0;
        vector<string> line;
        explode("_", str,line);
        if (line.size() < 2) return;
        string letterName = line[1];
        int letterID = atoi(line[2].c_str());
        if(mode==1)grammarModeLetterID=letterID;
        cout<<" letter:"<<letterName<<" ID:"<<letterID <<" pathImg:"<<pathImg<<endl;
        if(mode==2){
            OCRLetter &letterTest=font.letterByID(letterID);
            letterTestIndex=letterTest.index;
        }
        
        //открываем изображение слога для распознавания
        GBitmap *img = GBitmap::create(pathImg.c_str());

        img->crop32();
        img->binarisation64();
        GGraph graph_1(img);
        //img->printToScreenBinary();
        graph_1.graphFocalLine(15);
        //graph_1.printToScreenBinary();
        graph_1.focalPointXTL();
        graph_1.focalLineRecognition();
        graph_1.buildOCRStars();
        
        vector<OCRLetter>mainMatchLetter; //массив гипотез букв в проверяемом тексте
        strArray.resize(0);
        OCRBox p;
        p.yLimit0=0;
        p.yLimit1=img->rows();
        strArray.push_back(p);
        img->destroy();
        
        string mainString;
        textLetterCorrelation(graph_1, mainMatchLetter);
        if(mode==1)buildTextLine(mainMatchLetter,graph_1, mainString);
        if(mode==2 && mainMatchLetter.size())mainString=mainMatchLetter[0].name.data;
        
        OCRLetter letterTest = font.letterByID(letterID);
        cout<<"mainString:"<<mainString<<endl;
        mainString=str_replace("/", "", mainString);
        mainString=str_replace("\n", "", mainString);
        mainString=str_replace("་", "", mainString);
        letterName=str_replace("་", "", letterName);
        
        if(mode==1){
            if(mainString==letterName){
                cout<<"@@@ correlated letter letterName: |"<<letterName<<"| mainString:|"<<mainString<<"|"<<endl;
                ostringstream out;
                if(mode==1)out<<"rm "<<pathImg;
                string cmd=out.str();
                run(cmd.c_str());
                fontReg[letterTest.index]=1; //резервируем букву в регистре для того чтобы не участвовала в проверках корреляции шрифта
                
            }
        }
        if(mode==2){
            if(mainString!=letterName){
                cout<<"@@@not correlated letter:" << pathImg << " letterName: |"<<letterName<<"| mainString:|"<<mainString<<"|"<<endl;
                ostringstream out;
                if(mainMatchLetter.size()){
                    if(letterTestIndex > mainMatchLetter[0].index){
                        out<<"echo '"<<pathImg<< " :|: "<<mainMatchLetter[0].letterID<<" :|: " << mainString << "' >> /OSBL/___1.txt";
                    }else{
                            out<<"echo '    //no OCR// "<<pathImg<< " :|: "<<mainMatchLetter[0].letterID<<" :|: " << mainString << "' >> /OSBL/___1.txt";
                    }
                }else{
                    out<<"echo '"<<pathImg<< "' >> /OSBL/___1.txt";
                }
                string cmd=out.str();
                run(cmd.c_str());
            }
        }
        
        
	}

} //_________________________________________________________________________________________________________

/** @bref функция построения сортированной таблицы корреляции эталона буквы и
 обучающей выборки*/
void GGraphOCR::buildLetterCorrelationTable(uint letterIndex, cstr path) {
/*
	openFontOCR("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/FONT_OCR.bin");

	ostringstream out;
	ostringstream out1;
	vector<string> fileList;
	string str = path;

	readDirectoryToArray(fileList, str, "img");
	int size = (int)fileList.size();
	for (int i = 0; i < size; i++) {
		string path = fileList[i];
		cout << i << "   " << path << endl;
		str = path;
		str = substr((int)str.rfind("/") + 1, (int)str.size(), str);
		string fileName = str;
		vector<string> line;
		explode("_", str,line);
		if (line.size() < 2) return;
		string name = line[1].c_str();

		GBitmap *img = GBitmap::create(path.c_str());
		img->binarisation64();

		GGraph graph(img);

		initHashTable(graph);
		graph.graphFocalLine(15);
		graph.focalPointXTL();
		graph.focalLineRecognition(); 
		graph.buildOCRStars();

		OCRLetter letter;
		readLetterByID(letter, letterIndex);

		// TIME_PRINT_
		textCorrelation(graph, letter);

		int correlation = letter.correlation;

		str = letter.name.data;
		if (name == str && correlation < 90) { out << "<img src=\"" << fileName << "\"> correlation:" << correlation << "<br/>\n"; }
		if (name != str && correlation > 90) { out1 << "<img src=\"" << fileName << "\"> correlation:" << correlation << "<br/>\n"; }

		if (i == 1000) { cout << "1"; }
		img->destroy();
	}
	string pathStr = path;
	pathStr += "/__1.html";
	str = out.str();
	writeText(str, pathStr);
	pathStr = path;
	pathStr += "/__2.html";
	str = out1.str();
	writeText(str, pathStr);
*/
}

} // namespace ocr
