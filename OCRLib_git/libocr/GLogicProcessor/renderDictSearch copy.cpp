#include "GLogicProcessor.h"
#include "GMemory.h"
#include "php2stl.h"

void GLogicProcessor::renderDictSearch(GBitsetMatrix *setMatrix,
                                       map<vector<int>,ulong>&searchResult,
                                       vector<OCRMatch>&dLine,
                                       vector<OCRMatch>&originalMatch,
                                       vector<OCRMatch>&pageText){
    
    int print=1;
    DR(" searchResult.size()="<<searchResult.size()<<" dLine()="<<dLine.size())
    print=0;
    vector<OCRMatch>wordLine;
    
    //TIME_START
    
    //в dLine[] записаны пары букв из которых словарь собирал фразы
    //для окончательной сборки фразы нужны найденные с словаре фразы, составленные из оригинальных пар букв
    //также нужны сами пары букв для частей фразы на которые не найдены ответы словаря 
    //каждый OCRMatch содержит два массива, в которые записана информация о парах букв, составляющих фразу.
    // .line[] содержит индекс пар букв и возвращается заполненным в searchResult как результат работы словаря
    // .letter[] содержит копию пары букв по индексу line[] из исходного массива dLine[]
    for(int i=0;i<originalMatch.size();i++){
        originalMatch[i].setSize();
    }
    map<vector<int>,ulong>::iterator it;
    for (it = searchResult.begin(); it != searchResult.end(); ++it) {
        OCRMatch word;
        int in=abs(it->first[0]);   //знаком значения записано есть ли по мнению словаря разделитель слогов в этой паре
        DR(endl<<"in:"<<in<<endl);
        word.x0=dLine[in].x0;
        word.y0=dLine[in].y0;
        word.xL0C=originalMatch[dLine[in].line[0]].xCenter;
        int d; int sizeStr=0;
        while(sizeStr<128){    
            d=it->first[sizeStr];  DR(d<<" ")
            if(d==32767)break;  //32767 маркирует конец строки
            word.line.push_back(d);
            sizeStr++;
        }

        d=abs(word.line[word.line.size()-1]);
        word.x1=dLine[d].x1;
        word.y1=dLine[d].y1;
        word.xL1C=originalMatch[dLine[in].line[1]].xCenter;

        word.correlation=0;
        int n;
        for(int i=0;i<sizeStr;i++){
            //DR(searchResult[i].line[j]<<" c="<<line[searchResult[i].line[j]].correlation;
            //DR(" searchResult["<<i<<"].line.size()="<<searchResult[i].line.size()<<" ind="<<searchResult[i].line[j])
            n=abs(it->first[i]); 
            word.wName+=dLine[n].wName[0];
            word.correlation+=dLine[n].correlation;
        }
        word.wName+=dLine[n].wName[1];
        word.correlation=word.correlation/sizeStr+(float)(sizeStr*100)/50+(float)(it->second)/5000;   //учитываем длину и вероятность фразы
        //word.correlation+=it->second;   //прибавляем к кореляции количество ответов словаря
        //word.correlation=word.line.size();
        //if(word.correlation<70)continue;
        word.name=Unicode_to_UTF(word.wName);  DR("Yagpo= "<<word.name<<" "<<endl)
        word.name=YagpoToUni(word.name);       DR("xL0C="<<word.xL0C<<"xL1C="<<word.xL1C<<" n="<<word.name<<endl);
        DR(word.name<<"/"<<"c="<<word.correlation<<" x0="<<word.x0<<" x1="<<word.x1<<" y0="<<word.y0<<" y1="<<word.y1<<endl)
        wordLine.push_back(word);
    }
    //на этом этапе в результатах словаря около 70 результатов претендующих на один и тотже слог.
    //производим разбор результатов словаря по отношению к исходному тексту
    //сравниваем насколько точно каждая словарная гипотеза описывает данные текста и сравниваем гипотезы между собой.
    
    //testWordLine(setMatrix,wordLine,dLine,originalMatch);
    
    
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
        word.line=dLine[i].line;
        word.line.push_back(i);
        wordLine.push_back(word);
        
    }
    

    
    for(int i=0;i<wordLine.size();i++){
        wordLine[i].status=0;
        wordLine[i].allMatchCount=1;
        wordLine[i].setSize();
        //DR(" wordLine[i]="<<wordLine[i].name<<" w="<<wordLine[i].letterW)
        
    }
    
    sort(wordLine.begin(),wordLine.end(),sortMatchX0);
    compressMatch(wordLine);
    
    //drawGrapeLine(wordLine); exit(0);
    
    //int count=(int)wordLine.size();
    //int step=0;
    print=0;


    //проверяем ответы словаря на здравый смысл. Пары букв из которых составлен ответ словаря не должны отменять
    //уверенно распознанные буквы
    //также убираем пустые значения
    vector<OCRMatch>wLine;
    for(int i=0;i<wordLine.size();i++){
        if(!wordLine[i].correlation)continue;
        wordLine[i].status=0;
        DR("wordLine["<<i<<"].name="<<wordLine[i].name<<endl);
        if(wordLine[i].line.size()>1){
            //для полученных фраз удобнее хранить исходные пары букв внутри фразы
            for(int j=0;j<wordLine[i].line.size();j++){
                DR(" ind="<<wordLine[i].line[j]<<" size="<<wordLine[i].line.size())
                
                DR(" n="<<dLine[abs(wordLine[i].line[j])].name<<" d="<<dLine[abs(wordLine[i].line[j])].letter[0].delimeter<<endl)
                
                wordLine[i].letter.push_back(dLine[abs(wordLine[i].line[j])].letter[0]);
                //wordLine[i].letter.push_back(dLine[abs(wordLine[i].line[j])].letter[1]);
                if(wordLine[i].line[j]<0){   //если в паре по мнению словаря есть разделитель слога (минус маркирует такие пары) /@@@
                    wordLine[i].letter[wordLine[i].letter.size()-1].delimeter=dLine[abs(wordLine[i].line[j])].letter[0].delimeter;
                    if(wordLine[i].letter[wordLine[i].letter.size()-1].delimeter=="")wordLine[i].letter[wordLine[i].letter.size()-1].delimeter="་";
                }else{
                    ///если по мнению словаря в паре не должно быть разделителя слогов
                    wordLine[i].letter[wordLine[i].letter.size()-1].delimeter="";
                }
            }
            wordLine[i].letter.push_back(dLine[abs(wordLine[i].line[wordLine[i].line.size()-1])].letter[1]);
        }
        //if(wordLine[i].letter.size()==1){  //нормализуем одиночные пары букв
        //    wordLine[i].letter[0].delimeter=wordLine[i].delimeter;
       // }
        wLine.push_back(wordLine[i]);
    }
    //drawGrapeLine(wLine); exit(0);
    //полученные в результате подготовки в  renderDictSearch части фразы собираем в целые фразы
    //на этом этапе фразы собираются вместе ограничителями слогов, знаками препинания и примечаниями внутри фразы.
    sentenceConstructur(wLine);
    
    //drawGrapeLine(wLine); exit(0);

    //расставляем ограничители слогов (точки)
    print=0;
    string str;
    wstring delimeter;
    for(int n=0;n<originalMatch.size();n++){
        originalMatch[n].setSize();
    }
    print=0;
    for(int i=0;i<wLine.size();i++){
        if(!wLine[i].correlation)continue;
        DR(wLine[i].name<<endl)
        wLine[i].wName=L"";
        for(int n=0;n<wLine[i].letter.size();n++){
            str=wLine[i].letter[n].delimeter;  //cout<<"d="<<str;
            str=UnicodeToYagpo(str);
            delimeter=UTF_to_Unicode(str);
            wLine[i].wName+=wLine[i].letter[n].wName[0]+delimeter;
            DR(" n="<<wLine[i].letter[n].name<<"d="<<str<<"/"<<endl)
        }
        wLine[i].wName+=wLine[i].letter[wLine[i].letter.size()-1].wName[1];
        wLine[i].name=Unicode_to_UTF(wLine[i].wName);
        wLine[i].name=YagpoToUni(wLine[i].name);
        DR(" n="<<wLine[i].name<<endl)
        wLine[i].correlation+=3;  //создаем приоритет над парами без расставленных ограничителей слогов
        //добавляем результат к массиву исходных букв (результат распознавания включая все распознанные буквы и символы)
        //это позволит разобрать части фразы, не закрытые словарными ответами
        //x0=wLine[i].x0; x1=wLine[i].x1;
        //for(int n=0;n<dLine.size();n++){
        //    if(dLine[n].x0>=x0&&dLine[n].x1<=x1)dLine[n].correlation=0;
        //}
        //wLine[i].correlation=100;
        dLine.push_back(wLine[i]);
        //DR(wLine[i].name)
    }
    
    //добавляем все исходные графические элементы. Это нужно для распознования отдельно стоящих букв и знаков препинания
    for(int i=0;i<originalMatch.size();i++)dLine.push_back(originalMatch[i]);
    for(int i=0;i<dLine.size();i++)dLine[i].setSize();
    sort(dLine.begin(),dLine.end(),sortMatchXCenter);
    
    //drawGrapeLine(dLine); exit(0);
    
    
    print=0;
    //убираем фразы внутри стыкованной фразы
    int limit;
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        if(dLine[n].y0>y1Base)dLine[n].correlation=0;
        
        //print=0;if(n==13)print=1; if(!print)continue;
        DR("@@@@"<<n<<" Collect n="<<dLine[n].name<<" d="<<dLine[n].delimeter<<endl)
        
        for(int m=0;m<dLine.size();m++){
          if(!dLine[m].correlation)continue;
          //print=0;if(m==16)print=1;
          if(m==n)continue;
          limit=12; 
          
          if(dLine[n].OCRIndex!='N'&&dLine[m].OCRIndex=='N')limit=0;  
          if(dLine[n].OCRIndex!='Z'&&dLine[m].OCRIndex=='Z')limit=0;
          if(dLine[n].OCRIndex=='N'&&dLine[m].OCRIndex=='N')limit=0;
          if(dLine[n].OCRIndex=='Z'&&dLine[m].OCRIndex=='Z')limit=4;
          if(dLine[n].OCRIndex=='Z'&&dLine[m].OCRIndex!='Z')limit=0;
          if(dLine[n].OCRIndex=='S'&&dLine[m].OCRIndex=='S')limit=0;
          
          if(dLine[m].xCenter>dLine[n].x0-limit&&dLine[m].xCenter<dLine[n].x1+limit){
          
              DR("n"<<n<<"="<<dLine[n].name<<" d="<<dLine[n].delimeter<<" c="<<dLine[n].correlation<<" m"<<m<<"="<<dLine[m].name
                 <<" c="<<dLine[m].correlation<<" xmC="<<dLine[m].xCenter<<" xnC="<<dLine[n].xCenter<<" xnX0="<<dLine[n].x0<<" xnX1="<<dLine[n].x1<<" wM="<<dLine[m].letterW<<" wN="<<dLine[n].letterW<<endl);
              
                  if(dLine[n].correlation>dLine[m].correlation){
                      if(dLine[n].letterW>dLine[m].letterW-limit||(dLine[n].letterW/dLine[m].letterW)>1.3){ DR(100)
                          DR("REMOVE M "<<m<<" cM="<<dLine[m].correlation<<" wM="<<dLine[m].letterW<<" cN="<<dLine[n].correlation<<" wN="<<dLine[n].letterW<<endl)
                          dLine[m].correlation=0;
                      }else{ DR(200)
                          if(dLine[n].correlation-dLine[m].correlation>5){   //предпочтение отдаем более широким буквам
                              dLine[m].correlation=0;
                              DR("REMOVE M1 "<<m<<" cN="<<dLine[n].correlation<<endl)
                          }else{
                              DR("REMOVE N "<<m<<" cN="<<dLine[n].correlation<<endl)
                              dLine[n].correlation=0;
                              break;
                          }    
                      }    
                  }else{
                      if(dLine[m].letterW>dLine[n].letterW-limit||(dLine[n].letterW/dLine[m].letterW)<1.3){
                         dLine[n].correlation=0;
                         DR(" cM="<<dLine[m].correlation<<" cN="<<dLine[n].correlation<<endl)
                         break;
                      }else{
                         if(dLine[m].correlation-dLine[n].correlation>5&&(dLine[n].letterW/dLine[m].letterW)<1.3){
                             dLine[n].correlation=0;
                             DR(" cM="<<dLine[m].correlation<<" cN="<<dLine[n].correlation<<endl)
                             break;
                         }else{
                             dLine[m].correlation=0;
                         }    
                      }    
                  }
              DR(" cM="<<dLine[m].correlation<<" cN="<<dLine[n].correlation<<endl)
          }    
        }    
            
    }
    //drawGrapeLine(dLine); exit(0);
    //убираем лишние знаки препинания
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        
        if(dLine[n].delimeter!=""&&dLine[n].name==""){
            int m=n-1;
            while(m>0){
                if(!dLine[m].correlation){m--;continue;}
                if(dLine[m].letter.size()!=0){
                    if(dLine[m].letter[dLine[m].letter.size()-1].delimeter.find(dLine[n].delimeter)!=-1){
                        dLine[n].correlation=0;
                    }
                }
                break;
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
