 //__GGraphOCRCorrelation.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
//
// C- This software is subject to§, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "../OCRString/php2stl.h"
#include "GGraphOCR.h"
#include <unistd.h>

namespace ocr {
    
/** вычисление корреляции группы похожих буквы и текста на основе найденных масштаба и центра буквы */
int GGraphOCR::letterGroupClusterCorrelation(GGraph &graph, OCRLetter &letter, int mode){
    /**
     на вход функции поступают найденные масштаб и центр буквы в тексте.
     функция выполняет тщательную проверку каждого кластера и каждой линии буквы
     включая короткие линии. Веса коротких линий и параметры сравнения линий определяются на основании 
     графической разности, записанной в OCRLetterMatch этой пары букв.
     
     алгоритм 31.05.2021
     */
    
    int print = 0;
    if (printCorrelation) { print = printCorrelation; }
    
    //вычисляем гипотезу масштаба буквы на основе корреляции базовых кластеров буквы и текста
    if(!letter.scale)detectScaleCluster(letter);
    float scale = letter.scale;
    letter.matchLineSum=0;
    string letterName=letter.name.data;
    
    
    if(print>1){
        //int indexBase=2539;
        //draw.printClusterSVG((void*)&font.baseClusterArray[indexBase], "/OSBL/CORRELATION_01/");
        //int corr1=clusterCorrelation(graph.clusterArray[3], font.baseClusterArray[indexBase]);
        //int corr2=clusterCorrelation(font.clusterArray[letter.cluster[10]], font.baseClusterArray[indexBase]);
        //cout<<corr1<<" "<<corr2<<endl;
    }
    
    
    //на этом этапе необходимо установить соответствие каждого кластера эталона с соответствующим кластером текста.
    //для того чтобы исключить нахождение нескольких кластеров эталона в одном кластере текста
    
    //отладка
    /*
    //создаем регистр кластеров найденых кластеров текста.
    vector<char> regLetterLine(letter.lineCount); //регистр корреляции линий буквы
    vector<char> regLetterCluster(letter.clusterCount); //регистр корреляции кластеров буквы
    regTextLine.resize(graph.focalLine.size()); // регистр занятости линий текста
    memset(&regTextLine[0],0,graph.focalLine.size());
    //создаем регистр занятости кластеров текста
    //такой реестр занятости кластера работает быстрее реестра занятости линий текста.
    vector<char> regTextCluster(graph.clusterArray.size());
    //временный массив для записи соответствия кластеров буквы и текста
    //массив matchArray содержит копии кластеров для которых найдено соответствие в тексте и необходим для вычисления обратной корреляции
    //ClusterMatch matchArray[MAX_CLUSTER_LETTER];
    //int matchArrayCount=0;
    */
    
    //быстрая версия
     //создаем регистр кластеров найденых кластеров текста.
     char regLetterLine[letter.lineCount]; //регистр корреляции линий буквы
     memset(regLetterLine, 0, letter.lineCount);
     char regLetterCluster[letter.clusterCount]; //регистр корреляции кластеров буквы
     memset(regLetterCluster, 0, letter.clusterCount);
     char regTextLine[graph.focalLine.size()]; // регистр занятости линий текста
     memset(regTextLine, 0, graph.focalLine.size());
     //создаем регистр занятости кластеров текста
     //такой реестр занятости кластера работает быстрее реестра занятости линий текста.
     char regTextCluster[graph.clusterArray.size()];
     memset(regTextCluster, 0, graph.clusterArray.size());
     //временный массив для записи соответствия кластеров буквы и текста
     //массив matchArray содержит копии кластеров для которых найдено соответствие в тексте и необходим для вычисления обратной корреляции
     //ClusterMatch matchArray[MAX_CLUSTER_LETTER];
     //int matchArrayCount=0;
    
    
    //проверяем корреляцию каждого кластера текста и находим первый кластер с высокой корреляцией
    //с наиболее устойчивым кластером буквы. Кластеры в букве должны быть отсортированы по уровню устойчивости
    
    /*
    int letterClusterIndex = -1;
    int minClasterIndex = MAX_CLUSTER;
    int letterMatchCount=letter.matchCount;

    for (int n = 0; n < letterMatchCount; n++) {
        ClusterMatch &match = letter.match[n];
        if(!match.corr)continue;
        DR("text_star:"<<match.textClusterID<<" letter_star:" << (int)match.sIndex <<" base:"<<match.baseClusterID<< " correlation:" << match.corr << endl);
        letterClusterIndex=n;
        break;
    }
    if(letterClusterIndex < 0)return 0;
    ClusterMatch letterMainMatch = letter.match[letterClusterIndex]; //копируем ClusterMatch буквы. Проверяем и записываем соответствие с текстом.
    */
    
    if(print){
        //draw.printMatch(letterMainMatch.letterClusterID,mainMatch.baseClusterID,(void*)&font,mainMatch.sIndex,(void*)&graph,"/OSBL/CORRELATION/");
        //draw.printMatch((void*)&letterMainMatch,(void*)&mainMatch,(void*)&font,(void*)&graph,"/OSBL/CORRELATION/");
        //cout<<1;
    }
    
    //определяем центр гипотезы буквы
    int xCenter = letter.xMatch;
    int yCenter = letter.yMatch;
    //int correlation=0;  //сумма длин коррелированных линий буквы
    int correlationText=0;  //сумма длин коррелированных линий текста
    //int limitCluster=(letter.letterH+letter.letterW)/pointSearchLimit*scale;
    OCRCluster &cluster=font.clusterArray[letter.cluster[0]];
    int limitCluster=(cluster.h+cluster.w)/2*scale;
    
    DR("scale:" << scale << " xMatch:" << xCenter  << " yMatch:" << yCenter << " limitCluster:" << limitCluster << endl);

    
    //если корреляция по базовым кластерам больше 50, центральные OCRStar кластеров проверяем по lookup4
    int correlationLineSum=0;   //переменная суммирует длину всех линий буквы для которых найдено соответствие
    int w = graph.columns();
    memset((char*)&regLetterLine[0], 0, letter.lineCount);
    
    if(print){
        cout<<"";
    }
    countLetterCorrelation++;
    
    for (int clusterIndex = 0; clusterIndex < letter.clusterCount; clusterIndex++) {
        //проверяем все кластер буквы по lookup4
        //опорный кластер буквы проверяем потому что лимиты диаграммы Вороновва более точные
        OCRCluster cluster=font.clusterArray[letter.cluster[clusterIndex]];
        ClusterMatch &match=letter.match[clusterIndex];
        match.corr=0;
        DR(clusterIndex<<" : sIndex:"<<cluster.sIndex<<" "<<(int)regLetterCluster[clusterIndex]<<endl);
        
        //проверяем содержит ли кластер непроверенные OCRLine
        int flag=0;
        for (int i = 0; i < cluster.lineCount; i++) {
            if(!regLetterLine[cluster.line[i].parentID]){  //в OCRLine кластера parentID это индекс OCRLine в букве
                flag=1;
                break;
            }
        }
        if(!flag)continue;
        //draw.printClusterSVG((void*)&cluster, "/OSBL/CORRELATION_0");
        //определяем предполагаемый центр кластера относительно центра гипотезы буквы
        int x=(cluster.cPoint.x - letter.xCenter)*scale + xCenter;
        int y=(cluster.cPoint.y - letter.yCenter)*scale + yCenter;
        //cout<<"x:"<<x<<" y:"<<y<<endl;
        //для всех кластеров текста которые находятся в зоне поиска гипотезы кластера
        OCRPoint adrFocalPoint[64];
        
        //находим все фокальные точки по заданному лимиту координат
        int sizeAdr = graph.detectFocalPoint(x - limitCluster / 2, y - limitCluster / 2, limitCluster, limitCluster, graph.columns(),
                                             graph.rows(), graph.bites_data_32, adrFocalPoint);
        DR("   sizeAdr:" << sizeAdr << endl);
        
        //находим ID номер фокальной точки и проверяем корреляцию
        int maxCorrelation = 0;
        int maxCorrelationIndex = -1;
        int maxCorrelationIndex1 = -1;
        
        OCRData32 lenCorr2D[sizeAdr];
        for (int i = 0; i < sizeAdr; i++) {
            OCRPoint &p = adrFocalPoint[i];
            //находим ID номер фокальной точки и ее исходные координаты
            int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
            //проверяем занятость точки текста
            if(regTextCluster[id])continue; //кластер текста уже имеет соответствие
            OCRCluster &clusterText=graph.clusterArray[id];
            countGeomentry++;
            DR(letter.letterID<<"// cluster letter:"<<cluster.sIndex<<"__clusterText:"<<clusterText.sIndex<<" clusterText.cPoint.x:"<<clusterText.cPoint.x<<" clusterText.cPoint.y"<<clusterText.cPoint.y<<endl);
            
            static int cout1_;
            cout1_++;
            if(cout1_==8){
                ;
            }
            
            
            //возможно необходимо проверять круги по диаграмме воронова. Одного соответствия по лимитам недостаточно.
            int corr=OCRClusterCorrelationLookup4(cluster,clusterText,letter, graph, lenCorr2D[i]);
            
            DR("corr:"<<corr<<endl);
            if(corr>maxCorrelation){
                maxCorrelation=corr;
                maxCorrelationIndex=id;
                maxCorrelationIndex1=i;
                if(corr==100)break;
            }
        }
        
        if(cluster.type==O_POINT && maxCorrelation<clusterCorrelationLimit){
            correlationLineSum-=letter.lineSum/5;  //если не найден круг ухудщаем корреляцию
        }
        if(maxCorrelation<clusterCorrelationLimit)continue;
        match.corr=100;
        OCRData32 &v=lenCorr2D[maxCorrelationIndex1];  //2-3
        //массив v содержит ID номера распознанных линий текста
        
        /* //проверем изменяет ли новый кластер количество распознанных линий текста
         //эта проверка исключает повторное нахождение кластера буквы в одном кластере текста
         int flagText=0;
         for(int i=0;i<v.len;i++){
         if(!regTextLine[v.data[i]]){
         flagText=1;
         break;
         }
         }
         //if(!flagText)continue; //линия буквы может перекрывать линию текста частично. Необходимо учесть соответствие частей OCRLine
         */
        
        regTextCluster[maxCorrelationIndex]=maxCorrelation;
        //в реестре корреляции кластеров буквы отмечаем что кластер имеет соответствие в тексте проверенное по lookup4
        regLetterCluster[clusterIndex]=maxCorrelation;
        for(int i=0;i<v.len;i++){
            regTextLine[v.data[i]]=1;  //в OCRLine кластера parentID это индекс OCRLine в букве
        }
        
        //в реестре корреляции OCRLine буквы отмечаем что OCRLine центральной OCRStar имеет соответствие в тексте
        //необходимо учитывать какие именно линии сработали в OCRStar.
        //Для некоторых букв одна OCRLine может отличать ее от другой буквы
        //Для таких пар букв отличающихся одним или несколькими небольшими векторами
        //применяем правила матрицы переходов.
        
        
        OCRStar &star=cluster.star[0];
        //записываем номера распознанных линий буквы и подсчитываем корреляцию кластера
        for(int i=0;i<star.lineCount;i++){
            int parentID=cluster.line[star.line[i]].parentID;
            if(!regLetterLine[parentID]){
                regLetterLine[parentID]=100;  //в OCRLine кластера parentID это индекс OCRLine в букве
                //cout<<">>> i:"<<(int)matchLetter.line[i]<<endl;
                correlationLineSum+=cluster.line[star.line[i]].lenA; //добавляем длину проверенной линии к величине корреляции
            }
        }
        
        
        DR("=======  NEW cluster correlation.  cluster:" << cluster.sIndex << " maxCorrelation:" << maxCorrelation<<endl);
    }
    
    
    //и вычисляем окончательную корреляцию кластеров гипотезы буквы и текста с учетом
    //геометрии расположения кластера относительно центра гипотезы буквы и проверки по lookup4
    //подсчитываем суммарную длину линий текста для которых найдено соответствие
    if(correlationLineSum<0){ //не найдено два круга буквы
        letter.correlation=0;
        return letter.correlation;
    }
    correlationText=0;
    letter.matchLineCount=0;
    uint size1=(uint)graph.focalLine.size();
    if(mode==1){
        for(int i=0;i<size1;i++){
            if(regTextLine[i]){
                correlationText+=graph.focalLine[i].lenA;
                letter.lineMatch[letter.matchLineCount]=i; //записываем в букву ID номер найденой линии текста
                letter.matchLineCount++;
            }
        }
    }else{
        for(int i=0;i<size1;i++){
            if(regTextLine[i]){
                correlationText+=graph.focalLine[i].lenA;
            }
        }
        size1=letter.lineCount;
        for(int i=0;i<size1;i++){
            if(regLetterLine[i]){
                letter.lineMatch[i]=1;
            }
        }
        letter.matchLineCount=size1;
    }
    letter.matchLineSum=correlationText;
    
    letter.correlation=round((float)correlationLineSum*100/letter.lineSum);
    DR("//====NEW letter.correlation::" << letter.correlation << " letter.matchLineSum:"<<letter.matchLineSum<< endl);
    
    
    //для каждой найденной буквы корреляция которой выше пороговой выполняем проверку геометрии и 1,2,3 lookup
    return letter.correlation;
}
    
/** вычисление корреляции буквы и текста на основе найденных соответствий базовых кластеров в области поиска*/
int GGraphOCR::letterTextClusterCorrelation(GGraph &graph, OCRLetter &letter, int mode) {
    /**
     на вход функции поступают результаты проверки соответствия базовых кластеров и OCRStar текста.
     также на основании работы @fn buildOCRFontClusterTable() известно какие OCRStar буквы соответствуют этим базовым
     кластерам. На основании таких соответствий мы можем утверждать, что если базовый кластер подобен OCRStar текста и
     OCRStar буквы то OCRStar текста и OCRStar буквы также подобны. Такое утверждение позволяет проводить вычисление
     корреляции только для совпадающих пар OCRStar текста и OCRStar буквы
     
     алгоритм 08.05.2021
     */
    
    int print = 0;
    if (printCorrelation) { print = printCorrelation; }


    //вычисляем гипотезу масштаба буквы на основе коррелляции базовых кластеров буквы и текста
    detectScaleCluster(letter);
    float scale = letter.scale;
    letter.matchLineSum=0;
    string letterName=letter.name.data;
    

    //проверяем соответствие по геометрии кластеров
    //необходимо проверить геометрию сработавших кластеров и применить lookup 4 ко всем кластерам корреляция которых ниже 95%
    
    if(print>1){
        //int indexBase=2539;
        //draw.printClusterSVG((void*)&font.baseClusterArray[indexBase], "/OSBL/CORRELATION_01/");
        //int corr1=clusterCorrelation(graph.clusterArray[3], font.baseClusterArray[indexBase]);
        //int corr2=clusterCorrelation(font.clusterArray[letter.cluster[10]], font.baseClusterArray[indexBase]);
        //cout<<corr1<<" "<<corr2<<endl;
    }
    
    
    //на этом этапе необходимо установить соответствие каждого кластера эталона с соответствующим кластером текста.
    //для того чтобы исключить нахождение нескольких кластеров эталона в одном кластере текста
    
    /*
    //отладка
    //создаем регистр кластеров найденых кластеров текста.
    vector<char> regLetterLine(letter.lineCount); //регистр корреляции линий буквы
    vector<char> regLetterCluster(letter.clusterCount); //регистр корреляции кластеров буквы
    regTextLine.resize(graph.focalLine.size()); // регистр занятости линий текста
    memset(&regTextLine[0],0,graph.focalLine.size());
    //создаем регистр занятости кластеров текста
    //такой реестр занятости кластера работает быстрее реестра занятости линий текста.
    vector<char> regTextCluster(graph.clusterArray.size());
    //временный массив для записи соответствия кластеров буквы и текста
    //массив matchArray содержит копии кластеров для которых найдено соответствие в тексте и необходим для вычисления обратной корреляции
    //ClusterMatch matchArray[MAX_CLUSTER_LETTER];
    //int matchArrayCount=0;
    */
    
    //быстрая версия

    //создаем регистр кластеров найденых кластеров текста.
    char regLetterLine[letter.lineCount]; //регистр корреляции линий буквы
    memset(regLetterLine, 0, letter.lineCount);
    char regLetterCluster[letter.clusterCount]; //регистр корреляции кластеров буквы
    memset(regLetterCluster, 0, letter.clusterCount);
    char regTextLine[graph.focalLine.size()]; // регистр занятости линий текста
    memset(regTextLine, 0, graph.focalLine.size());
    //создаем регистр занятости кластеров текста
    //такой реестр занятости кластера работает быстрее реестра занятости линий текста.
    char regTextCluster[graph.clusterArray.size()];
    memset(regTextCluster, 0, graph.clusterArray.size());
    //временный массив для записи соответствия кластеров буквы и текста
    //массив matchArray содержит копии кластеров для которых найдено соответствие в тексте и необходим для вычисления обратной корреляции
    //ClusterMatch matchArray[MAX_CLUSTER_LETTER];
    //int matchArrayCount=0;
    
    int correlation=0;  //сумма длин коррелированных линий буквы
    int correlationText=0;  //сумма длин коррелированных линий текста
    int xCenter;
    int yCenter;


    //проверяем корреляцию каждого кластера текста и находим первый кластер с высокой корреляцией
    //с наиболее устойчивым кластером буквы. Кластеры в букве должны быть отсортированы по уровню устойчивости
    int letterClusterIndex = -1;
    int minClasterIndex = MAX_CLUSTER;
    int letterMatchCount=letter.matchCount;
    
    for (int n = 0; n < letterMatchCount; n++) {
        ClusterMatch &match = letter.match[n];
        if(!match.corr)continue;
        DR("text_star:"<<match.textClusterID<<" letter_star:" << (int)match.sIndex <<" base:"<<match.baseClusterID<< " correlation:" << match.corr << endl);
        letterClusterIndex=n;
        break;
    }
    if(letterClusterIndex < 0)return 0;
    
    ClusterMatch letterMainMatch = letter.match[letterClusterIndex]; //копируем ClusterMatch буквы. Проверяем и записываем соответствие с текстом.
    OCRCluster &mainTextMatch = graph.clusterArray[letterMainMatch.textClusterID];
    OCRCluster &cluster=font.clusterArray[letter.cluster[letterClusterIndex]];
    int limitCluster=(cluster.h+cluster.w)/2*scale;
    
    
    if(print){
        //draw.printMatch(letterMainMatch.letterClusterID,mainMatch.baseClusterID,(void*)&font,mainMatch.sIndex,(void*)&graph,"/OSBL/CORRELATION/");
        //draw.printMatch((void*)&letterMainMatch,(void*)&mainMatch,(void*)&font,(void*)&graph,"/OSBL/CORRELATION/");
        //cout<<1;
    }
    
    //определяем центр гипотезы буквы
    xCenter = mainTextMatch.cPoint.x - (letterMainMatch.cPoint.x - letter.xCenter) * scale;
    yCenter = mainTextMatch.cPoint.y - (letterMainMatch.cPoint.y - letter.yCenter) * scale;
    //записываем координаты в гипотезу буквы
    letter.xMatch=xCenter;
    letter.yMatch=yCenter;
    DR("cluster:" << minClasterIndex << " xMatch:" << xCenter  << " yMatch:" << yCenter << endl);
    //проверяем гипотезу буквы относительно гипотезы найденного кластера
    //на первом этапе проверяем геометрическое положение кластеров буквы и текста совпавших по базовым кластерам
    //letterMainMatch считаем имеющим 100% корреляцию и записываем в реестр корреляции кластеров буквы

    for(int i=0;i<letterMainMatch.lineCount;i++){
        regLetterLine[letterMainMatch.line[i]]=100;
        correlation+=letterMainMatch.len[i];
        //cout<<">>> i:"<<(int)letterMainMatch.line[i]<<endl;
    }

    DR("cluster letter:" << (int)letterMainMatch.sIndex <<" text:"<<mainTextMatch.sIndex<< " x:" << mainTextMatch.cPoint.x << " xMatch:" << mainTextMatch.cPoint.x<<endl);
 
    
    //проверку соответствия кластеров буквы и текста начинаем с самого устойчивого кластера буквы.
    for (int letterMatchIndex = 0; letterMatchIndex < letterMatchCount; letterMatchIndex++) {
        if(letterMatchIndex == letterClusterIndex)continue; //опорный кластер буквы уже учтен в корреляции
        ClusterMatch matchLetter = letter.match[letterMatchIndex]; //копируем ClusterMatch буквы. Проверяем и записываем соответствие с текстом.
        DR(" matchLetter.sIndex:"<<matchLetter.sIndex<<" corr:"<<(int)matchLetter.corr<<endl;)
        if(!matchLetter.corr)continue;
        //вычисляем положение кластера относительно центра гипотезы буквы
        int x=(matchLetter.cPoint.x - letter.xCenter)*scale + xCenter;
        int y=(matchLetter.cPoint.y - letter.yCenter)*scale + yCenter;

        OCRCluster &matchText = graph.clusterArray[matchLetter.textClusterID];
        //cout<<">>test:"<<n<<" : "<<(int)match.matchIndex<<" base index:"<<match.baseClusterID<<" match.corr:"<<match.corr<<endl;
        //на этом этапе точное соответствие установленным можем предполагать только для комплексных кластеров.
        //проверяем положение найденного кластера текста относительно центра гипотезы буквы
        int xMatch=matchText.cPoint.x;
        if(abs(x-xMatch)>limitCluster)continue;
        int yMatch=matchText.cPoint.y;
        if(abs(y-yMatch)>limitCluster)continue;
        
        //для кластеров с найденых на этом этапе соответствие с кластером текста необходимо проверить по lookup4
        //добавляем длины линий таких кластеров к корреляции, но не резервируем их в тексте
        for(int i=0;i<matchLetter.lineCount;i++){
            if(!regLetterLine[matchLetter.line[i]]){
                correlation+=matchLetter.len[i];
                regLetterLine[matchLetter.line[i]]=1;
            }
        }
    }//for (int letterMatchIndex = 0;
    
    letter.correlation=correlation*100/letter.lineSum;
    DR(letter.name.data<<" letter.correlation:"<<letter.correlation<<endl;)
    if(letter.correlation<letterCorrelationLimitVoronov){
        letter.correlation=0;
        return 0;
    }

    //если корреляция по базовым кластерам больше 50, центральные OCRStar кластеров проверяем по lookup4
    int correlationLineSum=0;   //переменная суммирует длину всех линий буквы для которых найдено соответствие
    int w = graph.columns();
    memset((char*)&regLetterLine[0], 0, letter.lineCount);
    
    if(print){
        ;
    }
    countLetterCorrelation++;

    for (int clusterIndex = 0; clusterIndex < letter.clusterCount; clusterIndex++) {
        //проверяем все кластер буквы по lookup4
        //опорный кластер буквы проверяем потому что лимиты диаграммы Вороновва более точные
        OCRCluster cluster=font.clusterArray[letter.cluster[clusterIndex]];
        ClusterMatch &match=letter.match[clusterIndex];
        match.corr=0;
        DR(clusterIndex<<" : sIndex:"<<cluster.sIndex<<" "<<(int)regLetterCluster[clusterIndex]<<endl);

        //проверяем содержит ли кластер непроверенные OCRLine
        int flag=0;
        for (int i = 0; i < cluster.lineCount; i++) {
            if(!regLetterLine[cluster.line[i].parentID]){  //в OCRLine кластера parentID это индекс OCRLine в букве
                flag=1;
                break;
            }
        }
        if(!flag)continue;
        //draw.printClusterSVG((void*)&cluster, "/OSBL/CORRELATION_0");
        //определяем предполагаемый центр кластера относительно центра гипотезы буквы
        int x=(cluster.cPoint.x - letter.xCenter)*scale + xCenter;
        int y=(cluster.cPoint.y - letter.yCenter)*scale + yCenter;
        //cout<<"x:"<<x<<" y:"<<y<<endl;
        //для всех кластеров текста которые находятся в зоне поиска гипотезы кластера
        OCRPoint adrFocalPoint[64];
        
        //находим все фокальные точки по заданному лимиту координат
        int sizeAdr = graph.detectFocalPoint(x - limitCluster / 2, y - limitCluster / 2, limitCluster, limitCluster, graph.columns(),
                                             graph.rows(), graph.bites_data_32, adrFocalPoint);
        DR("   sizeAdr:" << sizeAdr << endl);
        
        //находим ID номер фокальной точки и проверяем корреляцию
        int maxCorrelation = 0;
        int maxCorrelationIndex = -1;
        int maxCorrelationIndex1 = -1;
        
        OCRData32 lenCorr2D[sizeAdr];
        for (int i = 0; i < sizeAdr; i++) {
            OCRPoint &p = adrFocalPoint[i];
            //находим ID номер фокальной точки и ее исходные координаты
            int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
            //проверяем занятость точки текста
            if(regTextCluster[id])continue; //кластер текста уже имеет соответствие
            OCRCluster &clusterText=graph.clusterArray[id];
            countGeomentry++;
            DR(letter.letterID<<"// cluster letter:"<<cluster.sIndex<<"__clusterText:"<<clusterText.sIndex<<" clusterText.cPoint.x:"<<clusterText.cPoint.x<<" clusterText.cPoint.y"<<clusterText.cPoint.y<<endl);

            static int cout1_;
            cout1_++;
            if(cout1_==3){
                cout<<" ";
            }
            
            
            //возможно необходимо проверять круги по диаграмме воронова. Одного соответствия по лимитам недостаточно.
            int corr=OCRClusterCorrelationLookup4(cluster,clusterText,letter, graph, lenCorr2D[i]);

            DR("corr:"<<corr<<endl);
            if(corr>maxCorrelation){
                maxCorrelation=corr;
                maxCorrelationIndex=id;
                maxCorrelationIndex1=i;
                if(corr==100)break;
            }
        }

        if(cluster.type==O_POINT && maxCorrelation<clusterCorrelationLimit){
            correlationLineSum-=letter.lineSum/5;  //если не найден круг ухудщаем корреляцию
        }
        if(maxCorrelation<clusterCorrelationLimit)continue;
        match.corr=100;
        OCRData32 &v=lenCorr2D[maxCorrelationIndex1];  //2-3
        //массив v содержит ID номера распознанных линий текста
        
        /* //проверем изменяет ли новый кластер количество распознанных линий текста
        //эта проверка исключает повторное нахождение кластера буквы в одном кластере текста
        int flagText=0;
        for(int i=0;i<v.len;i++){
            if(!regTextLine[v.data[i]]){
                flagText=1;
                break;
            }
        }
        //if(!flagText)continue; //линия буквы может перекрывать линию текста частично. Необходимо учесть соответствие частей OCRLine
        */
         
        regTextCluster[maxCorrelationIndex]=maxCorrelation;
        //в реестре корреляции кластеров буквы отмечаем что кластер имеет соответствие в тексте проверенное по lookup4
        regLetterCluster[clusterIndex]=maxCorrelation;
        for(int i=0;i<v.len;i++){
            regTextLine[v.data[i]]=1;  //в OCRLine кластера parentID это индекс OCRLine в букве
        }
        
        //в реестре корреляции OCRLine буквы отмечаем что OCRLine центральной OCRStar имеет соответствие в тексте
        //необходимо учитывать какие именно линии сработали в OCRStar. 
        //Для некоторых букв одна OCRLine может отличать ее от другой буквы
        //Для таких пар букв отличающихся одним или несколькими небольшими векторами
        //применяем правила матрицы переходов.
        
        
        OCRStar &star=cluster.star[0];
        //записываем номера распознанных линий буквы и подсчитываем корреляцию кластера
        for(int i=0;i<star.lineCount;i++){
            int parentID=cluster.line[star.line[i]].parentID;
            if(!regLetterLine[parentID]){
                regLetterLine[parentID]=100;  //в OCRLine кластера parentID это индекс OCRLine в букве
                //cout<<">>> i:"<<(int)matchLetter.line[i]<<endl;
                correlationLineSum+=cluster.line[star.line[i]].lenA; //добавляем длину проверенной линии к величине корреляции
            }
        }

        
        DR("=======  NEW cluster correlation.  cluster:" << cluster.sIndex << " maxCorrelation:" << maxCorrelation<<endl);
    }
        

    //и вычисляем окончательную корреляцию кластеров гипотезы буквы и текста с учетом
    //геометрии расположения кластера относительно центра гипотезы буквы и проверки по lookup4
    //подсчитываем суммарную длину линий текста для которых найдено соответствие
    if(correlationLineSum<0){ //не найдено два круга буквы
        letter.correlation=0;
        return letter.correlation;
    }
    correlationText=0;
    letter.matchLineCount=0;
    uint size1=(uint)graph.focalLine.size();
    if(mode==1){
        for(int i=0;i<size1;i++){
            if(regTextLine[i]){
                correlationText+=graph.focalLine[i].lenA;
                letter.lineMatch[letter.matchLineCount]=i; //записываем в букву ID номер найденых линии текста
                letter.matchLineCount++;
            }
        }
    }else{
        for(int i=0;i<size1;i++){
            if(regTextLine[i]){
                correlationText+=graph.focalLine[i].lenA;
            }
        }
        size1=letter.lineCount;
        for(int i=0;i<size1;i++){
            if(regLetterLine[i]){
                letter.lineMatch[i]=1;
            }
        }
        letter.matchLineCount=size1;
    }
    letter.matchLineSum=correlationText;
    
    letter.correlation=round((float)correlationLineSum*100/letter.lineSum);
    //correlation/=letter.lineCount;
    //копируем результат корреляции кластеров в гипотезу буквы
    //memcpy((void*)&letter.match, (const void*)&matchArray, sizeof(ClusterMatch)*matchArrayCount);
    
    DR("//====NEW letter.correlation::" << letter.correlation << " letter.matchLineSum:"<<letter.matchLineSum<< endl);
    
    //для каждой найденной буквы корреляция которой выше пороговой выполняем проверку геометрии и 1,2,3 lookup
    return letter.correlation;
 }
    
/** @bref Функция вычисления корреляции двух букв OCR шрифта на основе OCRLetterMatch*/
int GGraphOCR::letterMatchCorrelation(OCRLetter &letterBase, OCRLetter &letterTest_, OCRLetterMatch &match){
    //функция используется для отладки создания групп похожих букв и вычисления
    //графической разности двух букв
    //функция создает граф базовой буквы.
    //Это относительно долгая операция, поэтому функция отладочная.
    //в рабочей версии лучше использовать граф базовой буквы.
    
    //алгоритм 07.06.2021

    //создаем граф базовой буквы.
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
    
    strArray.resize(0);
    OCRBox p;
    p.yLimit0=0;
    p.yLimit1=img->rows();
    strArray.push_back(p);
    img->destroy();
    
    int modeCorrelation=1;
    if(letterBase.name!=letterTest_.name){
        modeCorrelation=0;
    }

    //создаем копию базовой буквы. В копию буквы будут записаны параметры корреляции
    OCRLetter letterTest=letterTest_;
    memset(letterTest.kLine,0,MAX_CLUSTER_LETTER * sizeof(float));
    letterTest.xMatch=match.xCenter;
    letterTest.yMatch=match.yCenter;
    letterTest.scale=match.scale;
    //сравниваем letterTest с графом буквы letterBase
    letterGroupClusterCorrelation(graph_1, letterTest, modeCorrelation);
    //копируем результаты корреляции в OCRLetterMatch
    match.lineCount=letterTest.lineCount;
    if(match.OCRIndex){
        for(int i=0;i<match.lineCount;i++){
            if(letterTest.lineMatch[i]){
                match.kLine[i]=1;
            }else{
                match.kLine[i]=0;
            }
        }
    }else{
        for(int i=0;i<match.lineCount;i++){
            if(!letterTest.lineMatch[i]){
                match.kLine[i]=1;
            }else{
                match.kLine[i]=0;
            }
        }
    }
    return letterTest.correlation;
}

    

/** @bref Функция поиска гипотез буквы в тексте.
	Выполняется проверка первого кластера буквы со всеми OCRStar текста*/
void GGraphOCR::textCorrelation(GGraph &graph, OCRLetter &letter) {

	/** функция выполняет поиск всех гипотез буквы в тексте
	 в процессе поиска выполняем поиск соответствия первого кластера эталона и
	 текста.
	 Для каждой найденой гипотезы совпадения первого кластера эталона и текста
	 расчитываем корреляцию совпадения гипотезы буквы эталона и текста
	 на этапе текста сохраняем только наилучшую гипотезу.
	 Для более экономичного распознавания используем предпросчет базовых кластеров
	 базы данных и функцию @fn textCorrelationHash
	 подробное описание стратегии распознавания в @fn textOCR

	*/
	//pointRegCluster.resize(graph.starArray.size());

	correlationGraph(graph, letter); //корреляция буквы и текста на основе масштабонезависимых lookup
	detectScale(graph.starArray.dataPtr(), letter);
	//correlationOCR(graph, letter); //корреляция буквы и текста на основе гипотезы масштаба
}

/** @bref Функция вычисления корреляции OCRLetter и GGraph
 на основе гипотезы масштаба и массива кластеров совпадающих по базовым кластерам  */
void GGraphOCR::correlationOCR(GGraph &graph, OCRLetter &letter, vector<OCRCluster> &searchResult) {
    
}

/** @bref Функция вычисления корреляции OCRLetter и GGraph
 на основе масштабонезависимых lookup */
void GGraphOCR::correlationGraph(GGraph &graph, OCRLetter &letter) {
/*
	int print = 1;
	if (printCorrelation) print = printCorrelation;

	int starCount = (int)graph.starArray.size();
	OCRStar *starsT = graph.starArray.dataPtr();
	OCRLine *graphLines = (OCRLine *)graph.focalLine.dataPtr();
	int correlation;
	int correlation1;
	int correlationStar;
	int clusterCorrelation = 0;
	int letterCorrelation = 0;
	uchar sMatch[4]; ///< id номера соответствующих OCRStar текста
	uchar corr[4];	 ///<значения корреляции соответствующих OCRStar текста
	//проверяем корреляцию каждого кластера
	for (int n = 0; n < letter.clusterCount; n++) {
		OCRStar &star = letter.star[letter.cluster[n]];
		star.corrC = 0;
		star.corr = 0;
		// printStar(letter,star);
		//ищем соответствие первой звезды кластера в тексте
		int maxCluster = 0;
		int maxClusterIndex = 0;
		clusterCorrelation = 0;
		star.matchCount = 0;
		for (int i = 0; i < starCount; i++) {
			OCRStar &starT = starsT[i];
			DT("cluster" << n << " star:" << star.index << " starT:" << i << endl);
			if (star.type == O_POINT) {
				if (starT.type == O_POINT) {
					letterCorrelation += 100;
					star.corrC = 100;
					star.match[star.matchCount] = i;
					star.matchCount++;
					if (star.matchCount == 4) star.matchCount = 3;
					DT("circle match" << endl;)
				} else {
					continue;
				}
			}

			//вычисляем масштабонезависимую корреляцию звездочек

			correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
			if (correlation < 100) continue;
			DT("correlation1:  star:" << star.index << " i:" << i << " c:" << correlation << endl);
			correlation = OCRStarCorrelationLookup2(starT, star, graphLines, letter.line,NULL,NULL,0);
			if (correlation < 70) continue;
			clusterCorrelation += correlation;
			//вычисляем корреляцию звезд кластера в тексте относительно найденой
			//первой звезды.
			for (int n1 = 0; n1 < star.starCount && n1 < starT.starCount; n1++) {
				//находим соответствие лучей
				int min = 360;
				int minIndex = 0;
				int angle = star.angle[n1];
				for (int m = 0; m < starT.starCount; m++) {
					int dl = abs(starT.angle[m] - angle);
					if (dl >= 325) dl = 360 - dl;
					if (dl < min) {
						min = dl;
						minIndex = m;
					}
				}
				sMatch[n1] = minIndex;
				corr[n1] = 0;
				OCRStar &starCluster = letter.star[star.star[n1]];
				OCRStar &starTCluster = starsT[starT.star[minIndex]];
				DT("____: t:" << starTCluster.index << " s:" << starCluster.index << endl);
				correlation = OCRStarCorrelationLookup2(starTCluster, starCluster, graphLines, letter.line,NULL,NULL,0);
				DT("correlation___2: t:" << starTCluster.index << " s:" << starCluster.index << " c:" << correlation
										 << endl);
				if (correlation < 70) continue;
				//накапливаем корреляцию звезд кластера
				clusterCorrelation += correlation;
				corr[n1] = correlation1;
			}
			//нормируем корреляцию кластера
			clusterCorrelation /= star.starCount + 1;
			//сохраняем максимальную корреляцию кластера
			if (clusterCorrelation > maxCluster) {
				maxCluster = clusterCorrelation;
				maxClusterIndex = i;
				star.corr = correlationStar;
				star.corrC = clusterCorrelation;
				star.match[0] = i; //сохраняем индекс звездочки с максимальной корреляцией
				star.matchCount = 1;
				memcpy(
					star.sMatch, sMatch, 4); //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
				memcpy(star.cMatch, corr, 4); //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
			}
		}
		letterCorrelation += maxCluster;
		DT("___letterCorrelation:" << letterCorrelation << endl);
		//проверить нужно ли в текте резервировать звезды соответствующие максимальной корреляции кластера
	}
	//нормируем корреляцию букваы
	letterCorrelation /= letter.clusterCount;

	DR("//==== GraphletterCorrelation:" << letterCorrelation << endl << endl);
*/

} //____________________________________________________________________________________________

/** @bref Функция вычисления корреляции OCRLetter и области GGraph
 на основе масштабонезависимых lookup */
int GGraphOCR::correlationGraphArea(GGraph &graph, OCRLetter &letter, vector<int> &searchStar) {

	int print = 0;
	if (printCorrelation) print = printCorrelation;

	//int starCount = (int)searchStar.size();
	//OCRStar *starsT = graph.starArray.dataPtr();
	// int      correlation;
	// int      correlation1;
	//int correlationStar;
	//int clusterCorrelation = 0;
	int letterCorrelation = 0;
/*
	//проверяем корреляцию каждого кластера
	for (int n = 0; n < letter.clusterCount; n++) {
		OCRStar &star = letter.star[letter.cluster[n]];
		star.corrC = 0;
		star.corr = 0;
		DT("star:" << star.index << endl);
		//ищем соответствие первой звезды кластера в тексте
		int maxCluster = 0;
		int maxClusterIndex = 0;
		clusterCorrelation = 0;
		star.matchCount = 0;
		for (int i = 0; i < starCount; i++) {
			OCRCluster &clusterT = starsT[searchStar[i]];
			//для вычисленеия корреляции создаем копию OCRStar буквы.
			//Это необходимо потому что в OCRStar будут записаны параметры корреляции кластера
			//в исходные данные OCRStar буквы переписываем копию с наибольшей корреляцией.
			//для lookup4 необходимо записать порядок соответствия и корреляцию OCRStar текста и эталона
			OCRStar star1 = star;

			clusterCorrelation = letterCusterCorrelation(graph, letter, starT, star1);
			DT("   starT:" << starT.index << " c:" << clusterCorrelation << endl);
			//сохраняем максимальную корреляцию кластера
			if (clusterCorrelation > maxCluster) {
				maxCluster = clusterCorrelation;
				maxClusterIndex = i;
				star = star1;
				star.corr = correlationStar;
				star.corrC = clusterCorrelation;
				star.match[0] = starT.index; //сохраняем индекс звездочки с максимальной корреляцией
				star.matchCount = 1;
			}
		}
		letterCorrelation += maxCluster;
	}
*/
	//нормируем корреляцию букваы
	letterCorrelation /= letter.clusterCount;
	DT("___letterCorrelation:" << letterCorrelation << endl);

	return letterCorrelation;
} //____________________________________________________________________________________________

/** вычисление корреляции кластера и текста на основе найденных соответствий базовых кластеров в области поиска*/
int GGraphOCR::letterCusterCorrelation(GGraph &graph, OCRLetter &letter, OCRStar &starT, OCRStar &star) {
/*
	int print = 0;
	if (star.type == O_POINT) {
		if (starT.type == O_POINT) { return 100; }
	}
	OCRLine *graphLines = (OCRLine *)graph.focalLine.dataPtr();

	//вычисляем корреляцию двух OCRStar по 1,2 и 3 lookup
	int correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
	if (correlation < 100) return 0;
	DT("correlation1:  star:" << star.index << " c:" << correlation << endl);
	correlation = OCRStarCorrelationLookup2(starT, star, graphLines, letter.line,NULL,NULL,0);
	if (correlation < 70) return 0;
	int clusterCorrelation = correlation;
	if (correlation > 70) {
		//вычисляем корреляцию звезд базового кластера в тексте относительно найденой
		//первой звезды.

		for (int n1 = 0; n1 < star.starCount && n1 < starT.starCount; n1++) {
			//находим соответствие лучей
			int min = 360;
			int minIndex = 0;
			int angle = star.angle[n1];
			for (int m = 0; m < starT.starCount; m++) {
				int dl = abs(starT.angle[m] - angle);
				if (dl >= 325) dl = 360 - dl;
				if (dl < min) {
					min = dl;
					minIndex = m;
				}
			}
			star.sMatch[n1] = minIndex; //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
			OCRStar &starCluster = letter.star[star.star[n1]];
			OCRStar &starTCluster = graph.starArray[starT.star[minIndex]];
			DT("____: t:" << starTCluster.index << " s:" << starCluster.index << endl);
			int correlation = OCRStarCorrelationLookup2(starTCluster, starCluster, graphLines, letter.line,NULL,NULL,0);
			DT("correlation___2: t:" << starTCluster.index << " s:" << starCluster.index << " c:" << correlation
									 << endl);
			if (correlation < 70) continue;
			//накапливаем корреляцию звезд кластера
			clusterCorrelation += correlation;
			star.cMatch[n1] =
				correlation; ////сохраняем порядок и значение корреляции фокальных линий  OCRStar текста и эталона
		}
		//нормируем корреляцию кластера
	}
	clusterCorrelation /= star.starCount + 1;
	return clusterCorrelation;
	*/
	return 0;
}


} // namespace ocr
