
//__GGraphOCRTextCorrelation.cpp
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
    
    
    
/** @bref Функция поиска гипотез буквы в тексте на основе групп похожих букв и весов OCRLine
 Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
 с проверкой совпадения базовой OCRStar кластера и текста по lookup1-2-4 */
void GGraphOCR::textLetterGroupCorrelation(GGraph &graph, vector<OCRLetter>&textMatch){
    /* Алгоритм май 2021 */
    /* Алгоритм апрель 2021 */
    
    int print = 0;
    uint fontSize = (uint)font.size();
    modeVoronov=1;
    
    //strTest="ས";
    //letterPrint=1620279448;
    
    //font.printLetterSVG(11259); exit(0);
    
    uint graphClusterArraySize = (uint)graph.clusterArray.size();
    OCRCluster *graphClusterArray = graph.clusterArray.dataPtr();
    //OCRLine *graphLines = graph.focalLine.dataPtr();
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
    
    
    //также составляем реестр базовых кластеров всех букв алфавита
    //vector<vector<int> >baseClusterRegistr;
    //baseClusterRegistr.resize(fontSize);
    
    
    if(letterPrint>1){
        OCRLetter letterTest = font.letterByID(letterPrint);
        //if (print > 1) {
        //graph.savePNG("/OSBL/CORRELATION/__1.png");
        //рисуем диаграмму Воронова
        //uint w_h = graph.columns() * graph.rows();
        //for (int x = 0; x < w_h; x++) { graph.bytes_data[x] = graph.line_data[x].d0 * 32; }
        //graph.invert();
        //graph.colorModeOut = 1; // позволяет рисовать серую картинку
        //graph.savePNG("/_Image2OCR/___1Draw__1A_2.png");
        draw.printGraphSVG((void*)&graph, "/OSBL/CORRELATION/");
        draw.printLetterSVG((void*)&letterTest, (void*)&font, "/OSBL/CORRELATION_2/");
        
        //OCRCluster cLetter=font.clusterArray[letterTest.cluster[2]];
        //OCRCluster cText=graph.clusterArray[30];
        //int corr1=clusterCorrelation(cText,cLetter);
        
        for (int n = 0; n < letterTest.matchCount; n++) {
            cout<<n<<" cluster.sIndex: "<<(int)letterTest.match[n].sIndex<<" base cluster: "<<letterTest.match[n].baseClusterID<<endl;
        }
        //exit(0);
        //int indexBase=990;
        //draw.printClusterSVG((void*)&font.baseClusterArray[195], "/OSBL/CORRELATION_01/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[815], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[13], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[164], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&graph.clusterArray[43], "/OSBL/CORRELATION_01/");
        //OCRCluster &clusterText=graph.clusterArray[8];
        
        /*
         OCRCluster &cluster=graph.clusterArray[6];
         OCRCluster &cluster1=font.clusterArray[letterTest.cluster[2]];
         OCRCluster &OCRClasterBase=font.baseClusterArray[24923];

         draw.printClusterSVG((void*)&cluster, "/OSBL/CORRELATION_3/");
         draw.printClusterSVG((void*)&cluster1, "/OSBL/CORRELATION_3/");
         draw.printClusterSVG((void*)&OCRClasterBase, "/OSBL/CORRELATION_3/");
         int corr1=clusterCorrelation(cluster,OCRClasterBase);
         //int corr1=clusterCorrelation(cluster,cluster1);
         cout<<corr1<<endl;
         
         
         TIME_START
         int corr1;
         OCRData32 lineCorr;
         OCRStar starT=graph.starArray[2];
         OCRStar star=cluster1.star[0];
         cluster1.cPointText=starT.cPoint;
         cluster1.scale=0.9;
         
         //for(int i=0;i<10000000;i++){
         //    if(i%1000000==0)cout<<i<<endl;
         //corr1=clusterCorrelation(clusterLetter,clusterText);
         
         //int correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
         //if (correlation < 100) continue;
         //correlation = OCRStarCorrelationLookup2(starT, star, cluster.line, cluster1.line,NULL,NULL,0);
         corr1=voronovDiagramCorrelation(starT, star, graph, letterTest, cluster1, lineCorr);
         //cout<<correlation<<endl;
         //}
         TIME_PRINT_
         cout<<corr1<<endl;
         */
        
        //draw.printClusterSVG((void*)&font.clusterArray[letterTest.cluster[0]], "/OSBL/CORRELATION_01/");
        //int corr2=clusterCorrelation(font.clusterArray[letterTest.cluster[0]], font.baseClusterArray[indexBase]);
        //cout<<1;
        
    }
    
    DR_("Start text recognition"<<endl;)
    int letterCorrelationCount1=0;
    
    
    //начиная с буквы алфавита с наибольшим количеством кластеров
    for( int index=0;index < fontSize;index++){
        int indexLetterT = font.fontSortArray[index].x1;
        //копируем букву из шрифта. В букву записываем результаты сравнения с текстом
        OCRLetter letterTest = font[indexLetterT];
        letterTest.correlation=0;
        if(grammarMode==OCR_FONT_CORRELATION){
            if(letterTest.letterID==grammarModeLetterID){
                continue;
            }
            if(fontReg[indexLetterT]){
                continue;
            }
        }
        
        
        letterTest.matchCount=letterTest.clusterCount;
        int letterMatchCount=letterTest.matchCount;
        
        string str2=letterTest.name.data;
        
        if(letterPrint > 1 && letterTest.letterID!=letterPrint)continue;
        
        //if(str!="ཧཱུྃ")continue;
        if(index%500 == 0)cout<<" /"<<index<<flush;
        
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
            print =0;
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
                print=0; printCorrelation=0;
                if(letterTest.letterID==letterPrint){
                    cout<<endl;
                    print=1; printCorrelation=1;
                }
                
                for (int index4 = 0; index4 < size1; index4++) {
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
                    memset((char*)&letterLineReg[0],0,MAX_CLUSTER_LETTER * 2);
                    for(int n=0;n<letterMainMatch.lineCount; n++){
                        letterLineReg[letterMainMatch.line[n]]=letterMainMatch.len[n];
                    }
                    letterMainMatch.corr=100; //записываем корреляцию стартового кластера
                    

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
                        DR("   letter sIndex:" << match1.sIndex << " allPointCount:" << allPointCount << endl);
                        
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
                            //int flagCluster=0;
                            OCRCluster &clusterText1=graphClusterArray[idCluster];
                            
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
                            
                            DR("_/_text:"<<clusterText1.sIndex<<" letter:"<<(int)match1.sIndex<<" /_base:"<<match1.baseClusterID<<endl);
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
                        //if(letterCorrelationCount1==11){
                        //    cout<<1;
                        //}
                        letterTextClusterCorrelation(graph, letterTest, 1);
                        printCorrelation=0;
                        if(str1==strTest)DR_(" @/ /________________________ letter:"<<letterTest.name.data<<" corr:"<<letterTest.correlation<<endl);
                    }

                    
                    //записываем гипотезу буквы
                    //if(letterTest.correlation>letterCorrelationLimit){
                    if(letterTest.correlation>letterCorrelationLimit){
                        DR_(" @/+/________________________ letter:"<<letterTest.name.data<<" id:"<<letterTest.letterID<<" corr:"<<letterTest.correlation<<endl);
                        
                        if(letterTest.correlation < 98){
                            //проверяем на этом месте все буквы группы, выбираем наилучшую корреляцию.
                            int maxCorr=letterTest.correlation;
                            int maxIndex=0;
                            OCRGroup &group=font.letterGroupArray[letterTest.index];
                            GStr<OCRLetter>v;
                            OCRLetter &letterBase=font[letterTest.index];
                            
                            for(int index5=1; index5<group.size;index5++){
                                OCRLetter &letterGroup=font[group.letter[index5]];
                                letterGroup.correlation=0;
                                //int xCenter;
                                //int yCenter;
                                if(group.match[index5].d0>-1){
                                    OCRLetterMatch &match=font.letterMatch[group.match[index5].d0];
                                    letterGroup.scale=match.scale * letterTest.scale;
                                    letterGroup.xMatch=(match.xCenter - letterBase.xCenter) * letterGroup.scale + letterTest.xMatch;
                                    letterGroup.yMatch=(match.yCenter - letterBase.yCenter) * letterGroup.scale + letterTest.yMatch;
                                }else{
                                    //OCRLetterMatch &match=font.letterMatch[group.match[index5].d1];
                                    //letterGroup.scale=match.scale;
                                    //letterGroup.xCenter=match.xCenter;
                                    //letterGroup.yCenter=match.xCenter;
                                    continue;
                                }
                                letterGroupClusterCorrelation(graph, letterGroup, 3);
                                if(letterGroup.correlation > maxCorr){
                                    maxCorr = letterGroup.correlation;
                                    maxIndex = index5;
                                }
                                v.push_back(letterGroup);
                                DR_("corr:"<<letterGroup.correlation<<" "<<letterGroup.name.data<<" ID:"<<letterGroup.letterID<<endl;);
                            }
                            DR_("maxCorr:"<<maxCorr<<endl;)
                        
                        }
                        
                        
                        continue;
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
                        //проверяем величину обратной корреляции
                        //описание обратной корреляции в функции backwardCorrelation
                        //tCount=letterTest.matchLineSum; // - количество кластеров области текста,
                        //int nOn=letterTest.matchLineSum; // - длина линий текста соответствующая найденным кластерам буквы,
                        //letterTest.correlationNew=(nOn*100/tCount + letterTest.correlation)/2;
                        //if(letterTest.correlationNew>maxLetterCorrelation){
                        //    maxLetterCorrelation=letterTest.correlationNew;
                        //}
                        
                        if(grammarMode==OCR_FONT_CORRELATION && letterTest.letterID==grammarModeLetterID)continue;
                        
                        checkFontGrammar(textMatch,letterTest); //проверяем шрифтовую грамматику найденной буквы. Описание в @fn checkFontGrammar
                        if(!letterTest.status)continue;
                        
                        //исключаем все кластеры распознанные этой буквой из поиска
                        for (int n = 0; n < letterMatchCount; n++) {
                            ClusterMatch &clusterMatch=letterTest.match[n];
                            //если буква найдена в тексте, исключаем соответствующие кластеры текста.
                            //регистр исключает повторный поиск этой буквы в этом месте
                            if(clusterMatch.corr==100){
                                textReg[clusterMatch.textClusterID]=1;
                            }
                        }
                        
                        //исключаем все OCRLine текста распознанные кластерами буквы из регистра распознавания текста
                        for(int index9=0;index9 < letterTest.matchLineCount;index9++){
                            mainTextReg[letterTest.lineMatch[index9]]=1;
                            DR("   set line:"<<letterTest.lineMatch[index9]<<endl);
                            
                        }
                        */ 
                    }
                }//for (int index4 = 0; index4 < size1; index4++)
            }
        }
    }//for( int index=0;index < v.size();index++)
    cout<<endl;
    for( int index=0;index < textMatch.size();index++){
        cout<<" found letter:"<<textMatch[index].name.data<< " ID:"<<textMatch[index].letterID
        <<" corr:"<<textMatch[index].correlation << " index:"<<textMatch[index].index << " x2:" << font.fontSortArray[index].x2 <<endl;
    }
    cout<<"letterCorrelationCount1:"<<letterCorrelationCount1<<endl;
    
    
    return;
}

/** @bref Функция поиска гипотез буквы в тексте
 Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
 с проверкой совпадения базовой OCRStar кластера и текста по lookup1-2-4 */
void GGraphOCR::textLetterCorrelation(GGraph &graph, vector<OCRLetter>&textMatch) {
    
    /* Алгоритм апрель 2021 */

	int print = 0;
    //int w = graph.columns();
    //int h = graph.rows();
    uint fontSize = (uint)font.size();
    modeVoronov=1;
    
    //font.printLetterSVG(11259); exit(0);
    
	uint graphClusterArraySize = (uint)graph.clusterArray.size();
	OCRCluster *graphClusterArray = graph.clusterArray.dataPtr();
	//OCRLine *graphLines = graph.focalLine.dataPtr();
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
    setBaseClasterInText(graph,textCluster,text2Cluster, baseID2TextCluster);
    
    //отладка
    //Создаем регистр поиска. В регистре отмечаются кластеры текста которые распознанны одной буквой базы.
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

    //быстрая версия
    /*
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
    */
    
    
	
    
    //также составляем реестр базовых кластеров всех букв алфавита
    //vector<vector<int> >baseClusterRegistr;
    //baseClusterRegistr.resize(fontSize);
    
    
    if(letterPrint>1){
        OCRLetter letterTest = font.letterByID(letterPrint);
        //if (print > 1) {
        //graph.savePNG("/OSBL/CORRELATION/__1.png");
        //рисуем диаграмму Воронова
        //uint w_h = graph.columns() * graph.rows();
        //for (int x = 0; x < w_h; x++) { graph.bytes_data[x] = graph.line_data[x].d0 * 32; }
        //graph.invert();
        //graph.colorModeOut = 1; // позволяет рисовать серую картинку
        //graph.savePNG("/_Image2OCR/___1Draw__1A_2.png");
        draw.printGraphSVG((void*)&graph, "/OSBL/CORRELATION/");
        draw.printLetterSVG((void*)&letterTest, (void*)&font, "/OSBL/CORRELATION_2/");
        
        //OCRCluster cLetter=font.clusterArray[letterTest.cluster[2]];
        //OCRCluster cText=graph.clusterArray[30];
        //int corr1=clusterCorrelation(cText,cLetter);
        
        for (int n = 0; n < letterTest.matchCount; n++) {
            cout<<n<<" cluster.sIndex: "<<(int)letterTest.match[n].sIndex<<" base cluster: "<<letterTest.match[n].baseClusterID<<endl;
        }
        //exit(0);
        //int indexBase=990;
        //draw.printClusterSVG((void*)&font.baseClusterArray[195], "/OSBL/CORRELATION_01/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[815], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[13], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&font.baseClusterArray[164], "/OSBL/CORRELATION_3/");
        //draw.printClusterSVG((void*)&graph.clusterArray[43], "/OSBL/CORRELATION_01/");
        //OCRCluster &clusterText=graph.clusterArray[8];
       
        
  /*
        OCRCluster &cluster=graph.clusterArray[6];
        OCRCluster &cluster1=font.clusterArray[letterTest.cluster[2]];
        OCRCluster &OCRClasterBase=font.baseClusterArray[24923];
        draw.printClusterSVG((void*)&cluster, "/OSBL/CORRELATION_3/");
        draw.printClusterSVG((void*)&cluster1, "/OSBL/CORRELATION_3/");
        draw.printClusterSVG((void*)&OCRClasterBase, "/OSBL/CORRELATION_3/");
        int corr1=clusterCorrelation(cluster,OCRClasterBase);
        //int corr1=clusterCorrelation(cluster,cluster1);
        cout<<corr1<<endl;
   

         TIME_START
         int corr1;
         OCRData32 lineCorr;
         OCRStar starT=graph.starArray[2];
         OCRStar star=cluster1.star[0];
         cluster1.cPointText=starT.cPoint;
         cluster1.scale=0.9;
         
         //for(int i=0;i<10000000;i++){
         //    if(i%1000000==0)cout<<i<<endl;
             //corr1=clusterCorrelation(clusterLetter,clusterText);
             
             //int correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
             //if (correlation < 100) continue;
             //correlation = OCRStarCorrelationLookup2(starT, star, cluster.line, cluster1.line,NULL,NULL,0);
             corr1=voronovDiagramCorrelation(starT, star, graph, letterTest, cluster1, lineCorr);
             //cout<<correlation<<endl;
         //}
         TIME_PRINT_
         cout<<corr1<<endl;
        */
        
        //draw.printClusterSVG((void*)&font.clusterArray[letterTest.cluster[0]], "/OSBL/CORRELATION_01/");
        //int corr2=clusterCorrelation(font.clusterArray[letterTest.cluster[0]], font.baseClusterArray[indexBase]);
        //cout<<1;
        
    }

    DR_("Start text recognition"<<endl;)
    int letterCorrelationCount1=0;

    
    //начиная с буквы алфавита с наибольшим количеством кластеров
    for( int index=0;index < fontSize;index++){
        int indexLetterT = font.fontSortArray[index].x1;
        //копируем букву из шрифта. В букву записываем результаты сравнения с текстом
        OCRLetter letterTest = font[indexLetterT];
        letterTest.correlation=0;
        if(grammarMode==OCR_FONT_CORRELATION){
            if(letterTest.letterID==grammarModeLetterID){
                continue;
            }
            if(fontReg[indexLetterT]){
                continue;
            }
        }
        
        
        letterTest.matchCount=letterTest.clusterCount;
        int letterMatchCount=letterTest.matchCount;

        string str2=letterTest.name.data;
        
        if(letterPrint > 1 && letterTest.letterID!=letterPrint)continue;
        
        //if(str!="ཧཱུྃ")continue;
        if(index%500 == 0)cout<<" /"<<index<<flush;

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
            print =0;
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
                print=0; printCorrelation=0;
                if(letterTest.letterID==letterPrint){
                    cout<<endl;
                    print=1; printCorrelation=1;
                }
                
                for (int index4 = 0; index4 < size1; index4++) {
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
                    memset((char*)&letterLineReg[0],0,MAX_CLUSTER_LETTER * 2);
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
                            DR("   letter sIndex:" << match1.sIndex << " allPointCount:" << allPointCount << endl);
                            
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
                                //int flagCluster=0;
                                OCRCluster &clusterText1=graphClusterArray[idCluster];
                                
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

                                DR("_/_text:"<<clusterText1.sIndex<<" letter:"<<(int)match1.sIndex<<" /_base:"<<match1.baseClusterID<<endl);
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
                            //if(letterCorrelationCount1==11){
                            //    cout<<1;
                            //}
                            letterTextClusterCorrelation(graph, letterTest, 1);
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
                        int size1=clusterText.lineCount;
                        letterTest.matchLineCount=size1;
                        for(int i=0;i<size1;i++){
                            letterTest.lineMatch[i]=clusterText.line[i].index; //записываем в букву ID номер найденой линии текста
                        }
                        
                    }
                    
                    //записываем гипотезу буквы
                    //if(letterTest.correlation>letterCorrelationLimit){
                    if(letterTest.correlation>letterCorrelationLimit){
                        DR_(" @/+/________________________ letter:"<<letterTest.name.data<<" id:"<<letterTest.letterID<<" corr:"<<letterTest.correlation<<endl);
                        //отмечаем что буква проверена на этом месте
                        for (int n = 0; n < letterMatchCount; n++) {
                            ClusterMatch &clusterMatch=letterTest.match[n];
                            //если буква найдена в тексте, исключаем соответствующие кластеры текста.
                            //регистр исключает повторный поиск этой буквы в этом месте
                            if(clusterMatch.corr==100){
                                textRegLetter[clusterMatch.textClusterID]=1;
                            }
                        }
                        //проверяем величину обратной корреляции
                        //описание обратной корреляции в функции backwardCorrelation
                        //tCount=letterTest.matchLineSum; // - количество кластеров области текста,
                        //int nOn=letterTest.matchLineSum; // - длина линий текста соответствующая найденным кластерам буквы,
                        //letterTest.correlationNew=(nOn*100/tCount + letterTest.correlation)/2;
                        //if(letterTest.correlationNew>maxLetterCorrelation){
                        //    maxLetterCorrelation=letterTest.correlationNew;
                        //}
                        
                        if(grammarMode==OCR_FONT_CORRELATION && letterTest.letterID==grammarModeLetterID)continue;
                        
                        checkFontGrammar(textMatch,letterTest); //проверяем шрифтовую грамматику найденной буквы. Описание в @fn checkFontGrammar
                        if(!letterTest.status)continue;

                        //исключаем все кластеры распознанные этой буквой из поиска
                        for (int n = 0; n < letterMatchCount; n++) {
                            ClusterMatch &clusterMatch=letterTest.match[n];
                            //если буква найдена в тексте, исключаем соответствующие кластеры текста.
                            //регистр исключает повторный поиск этой буквы в этом месте
                            if(clusterMatch.corr==100){
                                textReg[clusterMatch.textClusterID]=1;
                            }
                        }
                        
                        //исключаем все OCRLine текста распознанные кластерами буквы из регистра распознавания текста
                        for(int index9=0;index9 < letterTest.matchLineCount;index9++){
                            mainTextReg[letterTest.lineMatch[index9]]=1;
                            DR("   set line:"<<letterTest.lineMatch[index9]<<endl);
                            
                        }
                    }
                }//for (int index4 = 0; index4 < size1; index4++)
            }
        }
    }//for( int index=0;index < v.size();index++)
    cout<<endl;
    for( int index=0;index < textMatch.size();index++){
        cout<<" found letter:"<<textMatch[index].name.data<< " ID:"<<textMatch[index].letterID
        <<" corr:"<<textMatch[index].correlation << " index:"<<textMatch[index].index << " x2:" << font.fontSortArray[index].x2 <<endl;
    }
    cout<<"letterCorrelationCount1:"<<letterCorrelationCount1<<endl;
    
    
	return;
}
    
void GGraphOCR::setBaseClasterInText(GGraph &graph,vector<vector<int> >&textCluster,vector<vector<int> >&text2Cluster, vector<vector< vector<pointLine> > >&baseID2TextCluster){

    //размечаем весь текст кластерами базы данных, строим таблицу соответствия базовых кластеров базы и текста
    //и обратную таблицу соответствия OCRStar текста и базовых кластеров
    GStr<OCRCluster> &baseСlusterArray = font.baseClusterArray;     // массив всех базовых кластеров всех букв
    uint baseСlusterArraySize=(uint)baseСlusterArray.size();
    uint graphClusterArraySize = (uint)graph.clusterArray.size();
    OCRCluster *graphClusterArray = graph.clusterArray.dataPtr();
    
    textCluster.resize(baseСlusterArraySize);   // таблица в которой каждая строка это список OCRCluster текста
    // котороые соответсвуют базовому кластеру по 1,2,3 lookup
    text2Cluster.resize(graphClusterArraySize); // таблица в которой каждая строка это список базовых кластеров для OCRCluster текста
    baseID2TextCluster.resize(0);
    
    
    //TIME_START
    
    //cout<<"set base clusters"<<endl;
    for (int i = 0; i < baseСlusterArraySize; i++) {
        OCRCluster &baseCluster = baseСlusterArray[i];
        //выполняем поиск этого кластера по OCRCluster текста
        for (int j = 0; j < graphClusterArraySize; j++) {
            OCRCluster &cluster = graphClusterArray[j];
            //if(i==444 && j==2){
            //    cout<<"> ";
            //    draw.printClusterSVG((void*)&baseCluster, "/OSBL/CORRELATION_0/");
            //    draw.printClusterSVG((void*)&cluster, "/OSBL/CORRELATION_0/");
            //}
            int correlation = clusterCorrelation(cluster, baseCluster);
            if(correlation>limitBaseClusterCorrelation){
                if(letterPrint>1)cout<<"base:"<<i<<"/ text:"<<cluster.sIndex<<"/"<<correlation<<endl;
                cluster.baseClusterID=i;
                textCluster[i].push_back(j);
                text2Cluster[j].push_back(i);
            }
        }
    }
    //cout<<"done set base clusters"<<endl;
    //TIME_PRINT_
    
    //таблица textCluster теперь содержит кластеры текста которые размещены
    //в строке по индексу соответствующего кластера базы данных
    //это позволяет быстро найти все кластеры текста соответствующие заданому.
    
    //для каждой строки текста в массиве baseID2TextCluster создаем реестр соответствия ID номера базового кластера и координат кластера текста
    
    uint size = (uint)strArray.size();
    uint sizeBaseClusterArray = (uint)font.baseClusterArray.size();
    baseID2TextCluster.resize(size);
    
    for(int i=0;i<size;i++){
        vector<vector<pointLine> >&v=baseID2TextCluster[i];
        v.resize(sizeBaseClusterArray);
        int limitUp = 0;
        if(i > 0){
            OCRBox &p = strArray[i - 1];
            limitUp = p.y1 + (p.y1 - p.y0)/2;
        }
        strArray[i].yLimit0 = limitUp;
        
        int limitDown = 0;
        if(i<size-1){
            OCRBox &p = strArray[i + 1];
            limitDown = p.y0 - (p.y1 - p.y0)/2;
        }else{
            limitDown = strArray[i].yLimit1;
        }
        strArray[i].yLimit1 = limitDown;
        
        for (int j = 0; j < graphClusterArraySize; j++) {
            OCRCluster &cluster = graphClusterArray[j];
            if(cluster.cPoint.y > limitUp && cluster.cPoint.y <= limitDown){
                pointLine line;
                line.x=cluster.cPoint.x;
                line.y=cluster.cPoint.y;
                line.x1=j;
                for(int n=0;n<text2Cluster[j].size();n++){
                    v[text2Cluster[j][n]].push_back(line);
                }
            }
        }
    }
    
}
    

/** @bref Функция тестовой корреляции буквы и изображения буквы текста */
void GGraphOCR::testCorrelation(int letterID) {
/*
	OCRLetter letter;
	openFontOCR("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/FONT_OCR.bin");
	string str = "/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_2/";
	readLetterByID(letter, letterID); // 1570636506 -zha  // 1569375393  1570637085-au 1570635965-k+sha //1570635946 - kya

	vector<string> fileList;

	readDirectoryToArray(fileList, str, "img");
	int size = (int)fileList.size();
	for (int i = 0; i < size; i++) {
		string path = fileList[i];
		cout << path << endl;
		// str=path;
		// str=substr((int)str.rfind("/") + 1, (int)str.size(), str);
		// vector<string> line;
		//explode("_", str,line);
		// if(line.size() < 2) return;
		// string name=line[1].c_str();

		GBitmap *img = GBitmap::create(path.c_str());
		img->binarisation64();

		GGraph graph(img);
		initHashTable(graph);
		graph.graphFocalLine(15);
		graph.focalPointXTL();
		graph.focalLineRecognition();
		graph.buildOCRStars();

		// TIME_PRINT_
		correlationGraph(graph, letter); //корреляция буквы и текста на основе масштабонезависимых lookup
		string dataStr_;
		drawGraphSVG(graph, dataStr_);
		writeText(dataStr_, "/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_2/__graph2.svg");
		exit(0);
		// detectScale(graph,letter);
		// correlationOCR(graph,letter);  //корреляция буквы и текста на основе гипотезы масштаба
	}
*/	
}

} // namespace ocr
