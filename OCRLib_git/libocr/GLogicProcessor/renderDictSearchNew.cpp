#include "GLogicProcessor.h"
#include "GMemory.h"
#include "php2stl.h"

void GLogicProcessor::renderDictSearchNew(map<vector<int>,ulong>&searchResult,
                                          vector<OCRMatch>&dLine,
                                          vector<OCRMatch>&originalMatch,
                                          vector<OCRMatch>&pageText,
                                          GBitmap *lineImg32,
                                          GBitmap* letterAImg,
                                          GBitmap* letterBImg){
    
    int print=0;
    DR(" searchResult.size()="<<searchResult.size()<<" dLine()="<<dLine.size())
    vector<OCRMatch>wordLine;
    
    //TIME_START
    
    //в dLine[] записаны пары букв из которых словарь собирал фразы
    //для окончательной сборки фразы нужны найденные с словаре фразы, составленные из оригинальных букв хранящихся в originalMatch
    //также нужны пары букв для частей фразы на которые не найдены ответы словаря
    //каждый OCRMatch содержит массив line[], в которые записан индекс о парах букв, составляющих фразу.
    // line[] содержит индекс пар букв в массиве dLine и возвращается заполненным в searchResult как результат работы словаря

    map<vector<int>,ulong>::iterator it;
    for (it = searchResult.begin(); it != searchResult.end(); ++it) {
        OCRMatch word;
        int in=abs(it->first[0]);   //знаком значения записано есть ли по мнению словаря разделитель слогов в этой паре
        DR(endl<<"in:"<<in<<endl);
        word.x0=dLine[in].x0;
        word.y0=dLine[in].y0;
        word.xL0C=originalMatch[abs(dLine[in].line[0])].xCenter;
        int d,d_; int sizeStr=0;

        while(sizeStr<128){
            d=abs(it->first[sizeStr]);  DR(d<<" ")
            if(d==32767)break;  //32767 маркирует конец строки
            d_=d;
            word.line.push_back(abs(dLine[d].line[0]));
            if(it->first[sizeStr]<0){  //записываем наличие в словарной фразе разделителя после этой буквы
                word.dIndex.push_back(1);
            }else{
                word.dIndex.push_back(0);
            }
            ///word.line.push_back(d);
            sizeStr++;
        }
        word.line.push_back(abs(dLine[d_].line[1]));
        word.dIndex.push_back(0);
        
        d=word.line[word.line.size()-1];
        word.x1=originalMatch[d].x1;
        word.y1=0;
        for(int i=0;i<word.line.size();i++)if(originalMatch[word.line[i]].y1>word.y1)word.y1=originalMatch[word.line[i]].y1;
        word.xL1C=originalMatch[d].xCenter;
        word.setSize(1);
        word.setSize(0);
        word.correlation=0;
        word.allMatchCount=(uint)it->second;
        
        int n;
        for(int i=0;i<sizeStr;i++){
            //DR(searchResult[i].line[j]<<" c="<<line[searchResult[i].line[j]].correlation;
            //DR(" searchResult["<<i<<"].line.size()="<<searchResult[i].line.size()<<" ind="<<searchResult[i].line[j])
            n=abs(it->first[i]);
            word.wName+=dLine[n].wName[0];
            word.correlation+=dLine[n].correlation;
        }
        word.wName+=dLine[n].wName[1];
        //word.correlation=word.correlation/sizeStr+(float)(sizeStr*100)/50+(float)(it->second)/5000;   //учитываем длину и вероятность фразы
        word.correlation=word.correlation/sizeStr;
        //word.correlation+=it->second;   //прибавляем к кореляции количество ответов словаря
        //word.correlation=word.line.size();
        //if(word.correlation<70)continue;
        word.name=Unicode_to_UTF(word.wName);  DR("Yagpo= "<<word.name<<" "<<endl)
        word.name=YagpoToUni(word.name);       DR("xL0C="<<word.xL0C<<"xL1C="<<word.xL1C<<" n="<<word.name<<endl);
        DR(word.name<<"/"<<"c="<<word.correlation<<" x0="<<word.x0<<" x1="<<word.x1<<" y0="<<word.y0<<" y1="<<word.y1<<endl)
        wordLine.push_back(word);
    }
    //drawGrapeLine(wordLine); exit(0);        //результаты словаря с записаными в разделители букв знаками препинания и расставленными слогами
    //drawGrapeLine(originalMatch); exit(0);   //результаты распознавания
    //drawGrapeLine(dLine); exit(0);           //пары букв с записанными в разделители букв знаками препинания
    
    //print=1;
    //добавляем распознанные пары букв
    for(int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation)continue; //значение уже записано
        OCRMatch word=dLine[i];
        word.name=Unicode_to_UTF(word.wName);  //DR("Yagpo= "<<word.name<<" ")
        word.name=YagpoToUni(word.name);
        word.allMatchCount=1;
        word.line=dLine[i].line;
        word.dIndex=dLine[i].dIndex;
        wordLine.push_back(word);
    }
    
    for(int i=0;i<wordLine.size();i++){
        wordLine[i].status=0;
        wordLine[i].length=(uint)wordLine[i].line.size();
        //DR(" wordLine[i]="<<wordLine[i].name<<" w="<<wordLine[i].letterW)
        
    }
    
    compressMatch(wordLine);
    
    //проверяем ответы словаря на максимальную корреляцию, выбираем наилучшие гипотезы
    //drawGrapeLine(wordLine); exit(0);

    testWordLine(lineImg32,wordLine,originalMatch);
    //drawGrapeLine(wordLine); exit(0);
    
    ///добавляем все исходные графические элементы. Это нужно для распознования отдельно стоящих букв и знаков препинания
    vector<OCRMatch>line;
    for(int i=0;i<originalMatch.size();i++){
        if(!originalMatch[i].correlation)continue;
        originalMatch[i].line.push_back(i);
        originalMatch[i].dIndex.push_back(2);
        line.push_back(originalMatch[i]);
    }
    
    for(int i=0;i<line.size();i++){
        if(!line[i].correlation)continue;
        if(line[i].OCRIndex!='A'&&line[i].OCRIndex!='S'&&line[i].OCRIndex!='Z'&&line[i].OCRIndex!='N')continue;
        if(line[i].x0<-5||line[i].x0>10000){
            cout<<"wrong match dimention";
        }
        wordLine.push_back(line[i]);
    }
    sort(wordLine.begin(),wordLine.end(),sortMatchX0);
    
    //drawGrapeLine(wordLine); exit(0);
    
    //int count=(int)wordLine.size();
    //int step=0;
    print=0;

    //drawGrapeLine(wordLine); exit(0);
    
    
  
    dLine.resize(0);
    for(int i=0;i<wordLine.size();i++){
        if(!wordLine[i].correlation)continue;
        dLine.push_back(wordLine[i]);
    }
    
    //drawGrapeLine(dLine); exit(0);
    
    
    print=0;
    //убираем фразы и буквы внутри стыкованной фразы
    sort(dLine.begin(),dLine.end(),sortMatchX0);
    
    int limit;
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        if(dLine[n].OCRIndex=='Z')continue;
        
        //print=0;if(n==13)print=1; if(!print)continue;
        DR("@@@@"<<n<<" Collect n="<<dLine[n].name<<" d="<<dLine[n].delimeter<<endl)
        
        for(int m=0;m<dLine.size();m++){
            if(!dLine[m].correlation)continue;
            if(dLine[m].OCRIndex=='Z')continue;
            //print=0;if(m==16)print=1;
            if(m==n)continue;
            if(dLine[m].xCenter>dLine[n].x0&&dLine[m].xCenter<dLine[n].x1){
                DR("n"<<n<<"="<<dLine[n].name<<" d="<<dLine[n].delimeter<<" c="<<dLine[n].correlation<<" m"<<m<<"="<<dLine[m].name
                   <<" c="<<dLine[m].correlation<<" xmC="<<dLine[m].xCenter<<" xnC="<<dLine[n].xCenter<<" xnX0="<<dLine[n].x0<<" xnX1="<<dLine[n].x1<<" wM="<<dLine[m].letterW<<" wN="<<dLine[n].letterW<<endl);
                
                if(dLine[n].letterW>dLine[m].letterW*2){
                    dLine[m].correlation=0;
                }
                if(dLine[m].letterW>dLine[n].letterW*2){
                    dLine[n].correlation=0;
                            break;
                }
            }
        }
    }


    
    compressMatch(dLine);

    
    //drawGrapeLine(dLine); exit(0);
    
    testStackLetterNew(dLine,originalMatch,lineImg32,letterAImg,letterBImg);
    
    //drawGrapeLine(dLine); exit(0);
    //расставляем знаки препинания
    sentenceConstructurNew(dLine,originalMatch,lineImg32,letterAImg,letterBImg);
    
    //drawGrapeLine(dLine); exit(0);
    
    //убираем буквы и знаки препинания внутри стыкованной фразы
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        if(dLine[n].OCRIndex=='Z')continue;
        for(int m=0;m<dLine.size();m++){
            if(!dLine[m].correlation)continue;
            if(m==n)continue;
            if(dLine[m].xCenter>dLine[n].x0&&dLine[m].xCenter<dLine[n].x1){
                if(dLine[n].letterW>dLine[m].letterW){
                    dLine[m].correlation=0;
                    if(originalMatch[dLine[m].line[0]].delimeter.find("•")!=-1)originalMatch[dLine[n].line[0]].delimeter+="•";
                }else{
                    dLine[n].correlation=0;
                    if(originalMatch[dLine[n].line[0]].delimeter.find("•")!=-1)originalMatch[dLine[m].line[0]].delimeter+="•";
                    break;
                }
            }
        }
    }

    
    for(int n=0;n<dLine.size();n++){
        OCRMatch &c=dLine[n];
        if(!c.correlation)continue;
        c.name="";
        for(int m=0;m<c.line.size();m++){
            c.name+=originalMatch[c.line[m]].name;
            if(c.dIndex[m]==1){ //if dictionary have delimenter in this match
                if(originalMatch[c.line[m]].delimeter==""){
                    c.name+="་";
                }else{
                    c.name+=originalMatch[c.line[m]].delimeter;
                }
            }else if(c.dIndex[m]==0&&originalMatch[c.line[m]].delimeter!="་"){ //
                c.name+=originalMatch[c.line[m]].delimeter;
            }else if(c.dIndex[m]==2){
                c.name+=originalMatch[c.line[m]].delimeter;
            }
        }
    }
    
    
    //drawGrapeLine(dLine); exit(0);
    
    //сохраняем результат
    
    for(int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation)continue;
        pageText.push_back(dLine[i]);
    }
    
    
}//-------------------------------------------------------------------------------------------------------------
/*
 //----------------------------------------------------------------------------
 
 // алгоритм разбора распознаваемого текста четким поиском с использованием словаря слов
 // словарь слов сортирован по длинне слов:
 // - при нахождении слова в распознаваемом тексте самое простое: строится маска найденных слов (массив char, а еще лучше как регистр из нескольких long int по 8мь char или в переспективе по 64е 0 и 1), которая заполняет 0 соответствующие места в распознаваемом тексте. Перед поиском словарного слова применяется логическое "и" между распознаваемым текстом и маской найденных слов. Следующее словарное слово ищется уже по более короткой фразе и т.д.
 // - при достижении суммарной длинны найденных слов равной длинне прверяемого текста минус dltBalance (суммарную длинну накапливаем просто в переменной), начинаем проверять реальную заполненность маски найденных слов (подсчет единиц, intr popcht, годится и для char, но уже с учетом постранственного положения найденных слов), если маска прочти заполнена (числое единиц меньше dltBalance), то поиск по словарю останвливаем полностью и переходим к следующей распознаваемой фразе. Подсчет единиц ведем только в оставшимся окне маски.
 // где dltBalance априорный это остаток примерно в 3-8 букв.
 // - если вычесленный остаток составляет например 10 букв, то все слова из словаря длиннее 10 букв можно не проверять
 
 // Более сложный и красивый вариант: 0 заполняется уже существующая таблица наличия буквы в распознаваемом тексте, точнее часть таблицы соответствующая однократно встречаемым буквам и иногда и многократно встречаемым буквам (1/3), в общем таблицу проще воссоздать заново, а это долго
 
 //----------------------------------------------------------------------------
 
 // END
 */
