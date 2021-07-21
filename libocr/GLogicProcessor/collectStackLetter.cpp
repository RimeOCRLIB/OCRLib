#include "GLogicProcessor.h"
#include "../GFontEditor/GFontEditor.h"
#include "../GMemory/GMemory.h"



//Функция сборки всех стековых букв, которые можно предположить в строке.
//морфемой может быть слог, слово или фраза - это определяется величиной словаря
//этот этап выполняется с на базе словаря стековых букв в формате GMap
//функция зависит от шрифтовой грамматики языка и выполняется по-разному
//для вертикальных и горизонтальных стеков

void GLogicProcessor::collectStackLetter(vector<stringOCR>&strArray,
                                         vector<OCRMatch>&letterLine,
                                         vector<OCRMatch>&line,
                                         GBitmap* lineImg32,
                                         GBitmap* letterAImg,
                                         GBitmap* letterBImg,
                                         int lineIndex){
       
    //формирование таблицы стеков
    //для каждой буквы Юникода нужен код варианта проверки геометрии
//TIME_START
    int print=0;
    int y0Base=strArray[lineIndex].y0;
    int y1Base=strArray[lineIndex].y1;
    int hLine=y1Base-y0Base;
    string draw;
    string xKey; xKey=(uchar)224; xKey+=(uchar)188; xKey+=(uchar)185; 
    
    //string tt; tt="ཾ";    vector<ulong> searchResult_;
    //fontStackGMap->getOCRKey(xKey,searchResult_, 1); ///
    //cout<<"searchResult_="<<searchResult_.size(); exit(0);
    
    //drawGrapeLine(line); exit(0);
    //letterLine.resize(0);
    //buildOpenType(line,letterLine);  //функция разборки стеков, нужна только если распознавание ведется эталонами стеков.

    //drawGrapeLine(letterLine); exit(0);
    //компрессия. Все одинаковые буквы в пределах габарита буквы
    //заменяются на одну букву с макcимальной корреляцией
    
 
    //drawGrapeLine(letterLine); exit(0);
    
    int stepIndex=0;
    int xLimit=hLine*4;
    int countStep=0;
    int nextStepIndex=0;
    line.resize(0);
    //int s_count=0;
    
//сборка стеков происходит последовательным применением алгоритма сборки к участку строки по ширине
//примерно равной двум-трем стекам
  //print=0;
//TIME_PRINT_
    
    
    while(stepIndex<letterLine.size()){  
    //собираем пары букв которые подходят по расположению
    string strW;
    int xStep=letterLine[stepIndex].x1;
    //int s, s_;
    //print=0;
    DR(" stepIndex="<<stepIndex<<" letterLine.size()="<<letterLine.size()<<" xStep="<<xStep<<" xLimit"<<xLimit<<endl)
    //print=0;
    
    for(int i=stepIndex;i<letterLine.size()&&letterLine[i].x0<xStep+xLimit;i++){ //cout<<"i="<<i<<endl;
        if(!letterLine[i].correlation)continue;

        for(int j=stepIndex;j<letterLine.size()&&letterLine[j].x0<xStep+xLimit;j++){
            
            //print=0; if(letterLine[i].name=="ག") {DR("@_"<<i<<" ") print=1;}//&letterLine[j].name=="ན"
           //print=0; if(i==95) {print=1;DR("@_"<<i<<"/"<<j)}
            
            DR("ind="<<letterLine[i].letterIndex<<" i"<<i<<"="<<letterLine[i].name<<" j"<<j<<"="<<letterLine[j].name<<" Iy0="<<letterLine[i].y0
               <<" Iy1="<<letterLine[i].y1<<" Ix0="<<letterLine[i].x1<<" Jy0_="<<letterLine[j].y0<<" Jy1_="<<letterLine[j].y1
               <<" Jx0="<<letterLine[j].x0<<endl)
            if(!letterLine[j].correlation)continue;

              if(((letterLine[j].OCRIndex!='V')&&(letterLine[j].y0<letterLine[i].y1+letterLine[i].letterW/2))||
                 (letterLine[j].OCRIndex=='X'&&(letterLine[j].y0<letterLine[i].y0+letterLine[i].letterW))||
                 (letterLine[i].name=="ར"&&(letterLine[j].y0>letterLine[i].y0))||
                 letterLine[j].OCRIndex=='V'||
                 letterLine[j].OCRIndex=='W'||
                 letterLine[j].OCRIndex=='R'||
                 letterLine[i].OCRIndex=='X'||
                 (letterLine[i].name=="ྭ")
                 ){
                  DR("# stepIndex="<<stepIndex<<endl)
                  strW+=letterLine[i].name+letterLine[j].name+"\n";//+letterLine[j].name+"\n";
              }

            
        }
        nextStepIndex++;
    }
   
    
    nextStepIndex=nextStepIndex-(nextStepIndex-stepIndex)/5;   //отступаем немного назад для перекрытия с предыдущими разбираемыми буквами
    if(nextStepIndex<=stepIndex)nextStepIndex++;
    //print=0;
    DR(" nextStepIndex="<<nextStepIndex<<endl)
    DR("@@@@@@@@ strW="<<strW.size()<<endl)  //
    vector<ulong> searchResult;
    DR("START SEARCH"<<endl)
    
    //writeText(strW,"/_Image2OCR/11.txt");
        
    //читаем из словаря стеков все стеки которые можно собрать
    fontStackGMap->getOCRKey(strW,searchResult, 1); ///
    //проверяем найденные стеки на соответствие шрифтовой геометрии
    //s_count+=searchResult.size();
    DR("searchResult="<<searchResult.size()<<endl)
    //if(stepIndex>670)print=1;
    //exit(0);
    
    if(print){
        string str;
        for(int i=0;i<searchResult.size();i++){   //для каждой найденной в базе буквы
            TString s; fontTable->getTStr(searchResult[i],s);
            int OCRCount=atoi(s[9].c_str());
            if(OCRCount<10)continue;
            str+=s[REC_KEY_UTF]+" "+s[REC_LETTER_UNI_UTF]+"/\n";
        }
        writeText(str,"/_Image2OCR/2.txt");
    }
    
    
    for(int i=0;i<searchResult.size();i++){   //для каждой найденной в базе буквы

        TString s; fontTable->getTStr(searchResult[i],s);
        int OCRCount=atoi(s[REC_OCR_COUNT].c_str());
        if(OCRCount<10)continue;
        //cout<<s[5]<<endl;
        //if(s[5]!="ཨོཾ")continue; print=1;
        //if(i!=113)continue;
        DR("start i="<<i<<" /"<<searchResult[i]<<" /"<<s[REC_OCR_KEY]<<"/"<<s[REC_LETTER_UNI_UTF]<<"/"<<s[REC_LANG]<<"/"<<s[REC_OCR_KEY]<<endl)
        if(s[REC_LANG]!="tib"&&s[REC_LANG]!="eng"&&s[REC_LANG]!="skt")continue;
        //print=0;
        
        string name=s[REC_KEY_UTF];
        string nameUni=s[REC_LETTER_UNI_UTF];
        string OCRIndex=s[REC_OCR_KEY];
        wstring wName=UTF_to_Unicode(name);
        wstring wNameStack=L"";
        
            for(int ind=stepIndex;ind<letterLine.size()&&letterLine[ind].x0<xStep+xLimit;ind++){  //в промежутка равном по ширине трем стекам
                //cout<<"ind="<<ind<<" ";
                //print=0; if(nameUni=="དེ")print=1; if(!print)continue;
                //print=0; if(nameUni=="རྤ")print=1; if(!print)continue;

                DR(" ind="<<ind<<"/"<<letterLine[ind].correlation<<" n="<<letterLine[ind].name<<endl)
                if(!letterLine[ind].correlation)continue;
                int firstLetterIndex=0;
                if(OCRIndex[0]=='a')firstLetterIndex=1;
                
                if(wName[firstLetterIndex]==letterLine[ind].uni){                  //находим первую подходящую в тексте
                    
                    //if(ind==6928)print=1; if(!print)continue;
                    DR("//@@@collect "<<ind<<" name="<<name<<" "<<ind<<" c="<<letterLine[ind].correlation
                       <<" x0="<<letterLine[ind].x0<<" x1="<<letterLine[ind].x1<<" xC="<<letterLine[ind].xCenter
                       <<" y0="<<letterLine[ind].y0<<" y1="<<letterLine[ind].y1<<" wName.size()="<<wName.size()<<endl);
                    vector<OCRMatch>stackResult;                    //для этой буквы создаем массив сборки стеков

                    if(wName.size()==1){                            //для одиночных букв
                        //if(letterLine[ind].status)continue;
                        if(letterLine[ind].OCRIndex=='A'){
                            //строительные блоки OpenType также записываем. Они нужны для анализа готовых стеков, претендующих на одно место
                            DR("@@@@ valid letter"<<name<<" s="<<line.size()<<endl)
                            letterLine[ind].nameUni=nameUni;
                            line.push_back(letterLine[ind]);            //заносим ее в результат
                        }
                        
                    }else{                                          //проверка стековых букв
                        int j=1;
                        int flagReverse=0; if(firstLetterIndex)flagReverse=-1;                          
                        //флаг отмечает что стек собирается снизу вверх от коренной буквы; так собираются стеки имеющие неустойчивые надписные буквы
                        
                        DR("i="<<i<<"/@@@j="<<j<<" test Stack "<<letterLine[ind].name<<" flagReverse="<<flagReverse<<endl)
                        
                        letterLine[ind].status=1;
                        stackResult.push_back(letterLine[ind]);      //найденную букву делаем первой буквой стека
                        stackResult[0].status=1;                     //отмечаем как пригодную к сборке
                        stackResult[0].letter.push_back(letterLine[ind]);
                        //для найденой буквы сохраняем все варианты сборки стека
                        int sizeName=(int)wName.size();
                        while(j<sizeName){              //для каждой составляющей стек буквы
                          int stackLineFlag=0;  
                          DR(endl<<"  @@@@"<<j+flagReverse<<"@@@"<<flagReverse<<" collect NEXT LETTER  "<<Unicode_to_UTF(wName[j+flagReverse])<<endl)
                          int sizeStackLine=(int)stackResult.size();  //сохраняем количество стеков найденных первоначально
    
                          for(int stInd=0;stInd<sizeStackLine;stInd++){  //для каждого найденного стека, начиная с первого найденного стека
                              DR(" @@@ stack stInd="<<stInd<<" sizeStackLine="<<sizeStackLine<<" status="<<stackResult[stInd].status<<" stepIndex="<<stepIndex<<endl)

                               if(!stackResult[stInd].status)continue;   //пропускаем несобравшиеся стеки
                               int stackFlag=0;                          //ставим флаг проверки сборки стека
                               OCRMatch stackCopy;
                               stackCopy=stackResult[stInd];             //сохраняем основу стека
                               //проверяем буквы начиная немного позади стека по X
                               int deltaInd=20; if(stepIndex<deltaInd)deltaInd=0;
                              
                               for(int n=stepIndex-deltaInd;n<letterLine.size()&&letterLine[n].x0<stackCopy.x1+xLimit;n++){     //проверяем буквы около стека на пригодность для сборки
                                   //print=0; if(n==93)print=1; if(!print)continue;
                                   //DR(n<<" "<<Unicode_to_UTF(b.uni)<<"/"<<wName[j]<<" ")
                                   if(!letterLine[n].correlation)continue;
                                   OCRMatch &b=letterLine[n];

                                   
                                   if(b.uni==wName[j+flagReverse]){     //для пригодных по коду букв
                                       
                                       //print=0; if(n==2&&ind==30)print=1; if(!print)continue;
                                       
                                       DR("@@@@@"<<flagReverse<<" проверка пары n="<<n<<"/ind="<<ind<<endl<<"stack->"<<"nS="<<stackCopy.name
                                          <<" xs0="<<stackCopy.x0<<" xs1="<<stackCopy.x1<<" xsC="<<stackCopy.xCenter
                                          <<" y0st="<<stackCopy.y0<<" y1st="<<stackCopy.y1<<" ysC="<<stackCopy.yCenter
                                          <<endl<<"letter n="<<b.name
                                          <<" yl0="<<b.y0<<" yl1="<<b.y1
                                          <<" xl0="<<b.x0<<" xl1="<<b.x1<<"xlC="<<b.xCenter<<"  "
                                          <<" ind="<<(char)b.OCRIndex<<" c="<<b.correlation<<endl)
                                       //проверяем геометрию сборки стека
                                       //проверка по горизонтали
                                       int dX1=abs(b.xCenter-stackCopy.xCenter);
                                       DR("dX1="<<dX1<<" stepIndex="<<stepIndex<<" xStep="<<xStep<<" xLimit="<<xLimit<<endl)
                                       if(b.OCRIndex!='X'){
                                         if(dX1>20){DR("too far X"<<endl) continue;}
                                       }else{
                                           if(b.x0>stackCopy.x1||
                                              b.x1<stackCopy.xCenter){DR("too far X"<<endl) continue;}
                                       }
                                       
                                       for(int ind1=stepIndex;ind1<letterLine.size()&&letterLine[ind1].x0<xStep+xLimit;ind1++){
                                           if(ind1==n||!letterLine[ind1].correlation||letterLine[ind1].OCRIndex!='A')continue;
                                           if(dX1>abs(b.xCenter-letterLine[ind1].xCenter)&&
                                              (letterLine[ind1].x0>stackCopy.xCenter||letterLine[ind1].x1<stackCopy.xCenter)&&
                                              letterLine[ind1].correlation-stackCopy.correlation>-2){
                                               DR("have better root letter by X ind="<<ind1<<" n="<<letterLine[ind1].name
                                                  <<endl<<"letter n="<<letterLine[ind1].name
                                                  <<" yl0="<<letterLine[ind1].y0<<" yl1="<<letterLine[ind1].y1
                                                  <<" xl0="<<letterLine[ind1].x0<<" xl1="<<letterLine[ind1].x1
                                                  <<" xlC="<<letterLine[ind1].xCenter<<" ylC="<<letterLine[ind1].yCenter
                                                  <<" ind="<<(char)letterLine[ind1].OCRIndex<<" c="<<letterLine[ind1].correlation<<endl;)
                                               dX1=-1;
                                               break;
                                           }
                                       }
                                       if(dX1==-1)continue;
                                       
                                       //проверка по вертикали
                                       int dY1=abs(b.yCenter-stackCopy.yCenter);
                                       DR("dY1="<<dY1<<" stepIndex="<<stepIndex<<" xStep="<<xStep<<" xLimit="<<xLimit<<endl)
                                       for(int ind1=stepIndex;ind1<letterLine.size()&&letterLine[ind1].x0<xStep+xLimit;ind1++){
                                           if(ind1==n||!letterLine[ind1].correlation||letterLine[ind1].OCRIndex!='A')continue;
                                           DR("/"<<ind1<<"/n="<<letterLine[ind1].name<<endl)
                                           if(dY1>abs(letterLine[ind1].yCenter-stackCopy.yCenter+7)&&
                                              (letterLine[ind1].y0>stackCopy.y1-5||
                                               letterLine[ind1].y1<stackCopy.y0+5)&&
                                              (letterLine[ind1].xCenter>stackCopy.x0-5&&
                                               letterLine[ind1].xCenter<stackCopy.x1+5)&&
                                              letterLine[ind1].correlation-stackCopy.correlation>-2&&
                                              letterLine[ind1].y0<stackCopy.y0-10
                                              ){
                                               DR("have better root letter by Y ind="<<ind1<<" n="<<letterLine[ind1].name
                                                  <<endl<<"letter n="<<letterLine[ind1].name
                                                  <<" yl0="<<letterLine[ind1].y0<<" yl1="<<letterLine[ind1].y1
                                                  <<" xl0="<<letterLine[ind1].x0<<" xl1="<<letterLine[ind1].x1
                                                  <<" xlC="<<letterLine[ind1].xCenter<<" ylC="<<letterLine[ind1].yCenter
                                                  <<" ind="<<(char)letterLine[ind1].OCRIndex<<" c="<<letterLine[ind1].correlation<<endl)
                                               dY1=-1;
                                               break;
                                           }
                                       }
                                       if(dY1==-1)continue;
                                       //проверяем на пересечение площадей.
                                       OCRBox s;
                                       //проверяем есть ли пересечение букв.
                                       intArea(stackCopy.s,b.s,s);
                                       if(s.area<stackCopy.area/5&&s.area<b.area/5){
                                           DR("no intersection"<<endl)
                                       }else if(b.OCRIndex!='X'){
                                          letterArea(stackCopy,letterAImg); //подсчитываем количество пикселов в букве; рисуем ее в letterBImg
                                          letterArea(b,letterBImg); //подсчитываем количество пикселов в букве; рисуем ее в letterBImg
                                          int pArea=letterAImg->img32Union(letterBImg,&s);
                                          letterAImg->fillColumns32V(0, &stackCopy.s);
                                          letterBImg->fillColumns32V(0, &b.s);
                                           if(b.name=="ར"&&flagReverse==-1){
                                               if(pArea>stackCopy.pCount/2||pArea>b.pCount*0.5){
                                                   DR("intersection area with Ra is too big "<<endl)
                                                   continue;
                                               }
                                           }else{
                                               if(pArea>stackCopy.pCount/3||pArea>b.pCount/3){
                                                   DR("intersection area too big"<<endl)
                                                   continue;
                                               }
                                           }
                                       }

                                       
                                       if(b.OCRIndex=='V'){
                                           //проверка по вертикали
                                           if(stackCopy.y0-b.y1>20){
                                               DR("too hight2"<<endl)
                                               continue;
                                           }
                                           //cout<<" stackCopy.name="<<stackCopy.name<<"y0="<<stackCopy.y0<<" y1="<<b.y1<<endl;
                                           if(stackCopy.name.find(xKey)!=-1){
                                               if(stackCopy.name.find("ར")==-1&&stackCopy.y0-b.y1>5){
                                                   DR("too hight21"<<endl)
                                                   continue;
                                               }
                                               if(stackCopy.name.find(xKey)!=-1&&b.y1>stackCopy.y0+25){
                                                    DR("too low_"<<endl)
                                                    continue;
                                               }
                                           }else{
                                               if(stackCopy.name.find("ར")==-1&&stackCopy.y0-b.y1>15){
                                                   DR("too hight22"<<endl)
                                                   continue;
                                               }
                                               if(b.y1>stackCopy.y0+16){
                                                   if(b.name=="ཾ"){
                                                       if(b.y0>stackCopy.y0+24){
                                                           DR("too low M"<<endl)
                                                           continue;
                                                       }
                                                   }else{
                                                       DR("too low V"<<endl)
                                                       continue;
                                                   }
                                               }

                                           }
                                           //проверка по горизонтали
                                           int vLeft=10;
                                           //cout<<"b.xCenter="<<b.xCenter<<" stackCopy.x0="<<stackCopy.x0<<endl;
                                           if(stackCopy.name.find(xKey)!=-1)vLeft=14;
                                           if(stackCopy.xCenter-b.xCenter>vLeft*1.6){
                                               DR("too left"<<endl)
                                               continue;
                                           }
                                           if(b.xCenter>stackCopy.x1){
                                               DR("too right"<<endl)
                                               continue;
                                           }
                                       }
                                       if(b.OCRIndex=='A'&&flagReverse<0){
                                           //проверка по вертикали
                                           if(stackCopy.y0-b.y1>10){
                                               DR("too hightR2"<<endl)
                                               continue;
                                           }
                                           //cout<<" stackCopy.name="<<stackCopy.name<<"y0="<<stackCopy.y0<<" y1="<<b.y1<<endl;
                                           if(stackCopy.name.find(xKey)!=-1&&stackCopy.name.find("ར")==-1&&stackCopy.y0-b.y1>5){ 
                                               DR("too hightR2"<<endl)
                                               continue;
                                           }
                                           int dl1=5;
                                           if(b.name=="ར"&&flagReverse==-1)dl1=b.letterH*0.8;
                                           if(b.name=="ད")dl1=15;
                                           if(b.y1>stackCopy.y0+dl1){
                                               DR("too lowR2 "<<b.name<<endl)
                                               continue;
                                           }    
                                           //проверка по горизонтали
                                           if(b.xCenter<stackCopy.x0){
                                               DR("too leftR2"<<endl)
                                               continue;
                                           }
                                           if(b.xCenter>stackCopy.x1){
                                               DR("too rightR2"<<endl)
                                               continue;
                                           }
                                           
                                       }
                                       
                                       if(b.OCRIndex=='X'){
                                           //проверка по вертикали
                                           if(stackCopy.y0-b.y1>10){
                                               DR("too hight1"<<endl)
                                               continue;
                                           }
                                           if(stackCopy.name.find("ར")!=-1){
                                               if(b.y0>stackCopy.y0+20){
                                                   DR("too low0"<<endl)
                                                   continue;
                                               }
                                           }else if(stackCopy.name.find("ཉ")!=-1||stackCopy.name.find("ས")!=-1){
                                               if(b.y0>stackCopy.y0+40){
                                                   DR("too low1"<<endl)
                                                   continue;
                                               }
                                           }else{
                                               if(b.y0>stackCopy.y0+5){
                                                   DR("too low2"<<endl)
                                                   continue;
                                               }
                                           }
                                           //проверка по горизонтали
                                           if(b.x0<stackCopy.xCenter){
                                               DR("too left"<<endl)
                                               continue;
                                           }
                                           if(b.x0>stackCopy.x1){
                                               DR("too right"<<endl)
                                               continue;
                                           }
                                       }
                                       if((b.OCRIndex=='W'||
                                          b.OCRIndex=='R')){
                                          //проверка по вертикали 
                                          if(b.OCRIndex=='R'&&b.y0-stackCopy.y1>20){
                                              DR("too low2"<<endl)
                                              continue;
                                          }
                                          if(b.OCRIndex=='W'&&b.y0-stackCopy.y1>20){
                                               DR("too low2"<<endl)
                                               continue;
                                          }
                                          if(b.name=="ྲ"||b.name=="ྭ"){
                                              if(b.x1<stackCopy.x1-(stackCopy.x1-stackCopy.x0)/3){
                                                  DR("too left"<<endl)
                                                  continue;
                                              }
                                              if(hasTail(stackCopy)||(lastOf(stackCopy.name)=="ྲ"&&b.name=="ྭ")){
                                                  if(stackCopy.yCenter>b.y1){
                                                      DR("too hight31"<<endl)
                                                      continue;
                                                  } 
                                              }else{
                                                  if(b.y0<stackCopy.yCenter){
                                                      DR("too hight30"<<endl)
                                                      continue;
                                                  }
                                              }
                                              
                                           }else{
                                               if(stackCopy.y1-b.y0>20){
                                                   if((b.OCRIndex=='W'||b.OCRIndex=='R')&&hasTail(stackCopy)){
                                                       if(b.y0<stackCopy.yCenter){
                                                           DR("too hight33"<<endl)
                                                           continue;
                                                       }
                                                   }else{
                                                      DR("too hight32"<<endl)
                                                      continue;
                                                   }    
                                               }
                                           }    

                                       }
                                       if(b.OCRIndex!='V'&&
                                          b.OCRIndex!='W'&&
                                          b.OCRIndex!='R'&&
                                          b.OCRIndex!='X'&&
                                          flagReverse!=-1){
                                          //проверка по вертикали 
                                          if(b.y0-stackCopy.y1>20){
                                               DR("too low3"<<endl)
                                               continue;
                                           }
                                           if(stackCopy.name=="ར"){
                                              if(stackCopy.y1-b.y0>stackCopy.letterH*0.75){
                                                  DR("too hight5"<<endl)
                                                  continue;
                                              }                                                 
                                                  
                                           }else{
                                               if(hasTail(stackCopy.letter[stackCopy.letter.size()-1])){
                                                   if(b.y0-20<stackCopy.y0){
                                                       DR("too hight6 tail"<<endl)
                                                       continue;
                                                   }
                                               }else if(stackCopy.y1-b.y0>16){
                                                   DR("too hight6"<<endl)
                                                   continue;
                                               }

                                           }
                                           //проверка по горизонтали
                                           if(stackCopy.xCenter-b.xCenter>16){
                                               DR("too left"<<endl)
                                               continue;
                                           }
               
                                           if(b.xCenter-stackCopy.xCenter>16){
                                               DR("too right"<<endl)
                                               continue;
                                           }
                                       }
                                       DR(endl<<"valid stack y0Base="<<y0Base<<" y1Base="<<y1Base<<endl<<
                                          " x0="<<stackCopy.x0<<" x1="<<stackCopy.x1<<
                                          " y0="<<stackCopy.y0<<" y1="<<stackCopy.y1<<
                                          " @@@   x0_="<<b.x0<<" x1_="<<b.x1<<
                                          " y0_="<<b.y0<<" y1_="<<b.y1<<
                                          " @@@ hLine="<<hLine<<" hLine*0.7="<<hLine*0.7<<"l.size()="<<
                                          stackCopy.letter.size()<<endl);

                                                                        //для пригодную букву присоединяем к стеку
                                         if(!stackFlag){                //для первого стека присоединяем букву к стеку
                                              stackResult[stInd].status++;
                                             
                                              if(b.OCRIndex=='V'||flagReverse==-1){
                                                   stackResult[stInd].y0=b.y0;
                                                   //if(b.x1>stackResult[stInd].x1){
                                                   //   stackResult[stInd].x1=b.x1;
                                                   //}
                                                   //if(b.x0<stackResult[stInd].x0){
                                                   //   stackResult[stInd].x0=b.x0;
                                                   //}
                                              }else{
                                                  if(b.OCRIndex=='X'){
                                                      if(stackResult[stInd].y0>b.y0){
                                                          stackResult[stInd].y0=b.y0;
                                                      }
                                                      if(b.x1>stackResult[stInd].x1){
                                                          stackResult[stInd].x1=b.x1;
                                                      }
                                                  }else{
                                                   stackResult[stInd].y1=b.y1;
                                                  }    
                                                  if(b.OCRIndex!='X'&&b.OCRIndex!='V'&&
                                                     b.OCRIndex!='W'&&b.OCRIndex!='R'){
                                                     if(b.x0<stackResult[stInd].x0)stackResult[stInd].x0=b.x0;
                                                     if(b.x1>stackResult[stInd].x1)stackResult[stInd].x1=b.x1;
                                                     //stackResult[stInd].xCenter=stackResult[stInd].x0+(stackResult[stInd].x1-stackResult[stInd].x0)/2;
                                                  }

                                              }
                                             
                                              DR(" stackResult[stInd].correlation="<<stackResult[stInd].correlation
                                                <<" b.correlation="<<b.correlation<<endl);
                                              stackResult[stInd].correlation+=b.correlation;
                                              stackResult[stInd].name+=b.name;
                                              stackResult[stInd].setCenter();
                                              stackResult[stInd].setSize(1);
                                              //stackResult[stInd].letter.push_back(stackResult[stInd]);
                                              stackResult[stInd].letter.push_back(b);
                                              //stackResult[stInd].addMask32Vector(b);
                                              stackFlag=1;              //помечаем стек как имеющий присоединенную букву
                                         }else{                         //если стек уже имеет присоединеееную букву 
                    
                                            //добавляем новый стек
                                            OCRMatch newMatch;
                                            newMatch=stackCopy;         //копируем исходный стек без присоединенной буквы
                                            if(b.OCRIndex=='V'||flagReverse==-1){ 
                                              newMatch.y0=b.y0;
                                              if(b.x0<newMatch.x0)newMatch.x0=b.x0;
                                              if(b.x1>newMatch.x1)newMatch.x1=b.x1;
                                            }else{
                                                if(b.OCRIndex=='X'){
                                                    if(newMatch.y0>b.y0){
                                                        newMatch.y0=b.y0;
                                                    }
                                                    if(newMatch.x1<b.x1)newMatch.x1=b.x1;
                                                }else{
                                                    newMatch.y1=b.y1;
                                                }
#ifdef OCR_woodblock
                                                if(b.x0<newMatch.x0)newMatch.x0=b.x0;
                                                if(b.x1>newMatch.x1)newMatch.x1=b.x1;
#else
                                                if(b.OCRIndex!='X'&&b.OCRIndex!='V'&&b.OCRIndex!='W'&&b.OCRIndex!='R'){
                                                    if(b.x0<newMatch.x0)newMatch.x0=b.x0;
                                                    if(b.x1>newMatch.x1)newMatch.x1=b.x1;
                                                }
#endif
                                            }
                                            newMatch.setCenter();
                                            newMatch.setSize(1);
                                            newMatch.s.yLimit0=newMatch.yCenter+MATRIX_BORDER+newMatch.s.yLimit0;
                                            newMatch.OCRIndex='S';   //stack letter
                                            newMatch.status=stackCopy.status+1;
                                                                                      
                                            DR(" corelation1="<<stackCopy.correlation<<" b.correlation="<<b.correlation<<endl)
                                            newMatch.correlation=stackCopy.correlation+b.correlation;
                                            newMatch.name=stackCopy.name+b.name;
                                            newMatch.letter=stackCopy.letter;
                                            newMatch.letter.push_back(b);
                                            //newMatch.addMask32Vector(b);
                                            //newMatch.addMask32Vector(stackCopy);
                                            stackResult.push_back(newMatch);

                                         }
                                         stackLineFlag=1;          //отмечаем что как минимум один стек имеет присоединенную букву
                                       
                                         DR("@@@@ valid letter"<<name<<" add="<<Unicode_to_UTF(b.uni)<<
                                           " stackResult="<<stackResult.size()<<" c="<<stackResult[stackResult.size()-1].correlation<<
                                           " status="<<stackResult[stackResult.size()-1].status<<endl);
                                   
                                   }//b.uni==wName[j]

                                
                               }// for(int n=0;n<letterLine.size();n++)
                              //если не нашлась подходящая буква, отмечаем стек как непригодный для сборки
                              if(!stackFlag){
                                  DR("NO STACK"<<endl)
                                  stackResult[stInd].status=0;
                              }

                          }//for(int stInd=0;stInd<stackResult.size();stInd++){
                                
                          if(!stackLineFlag)break; //не удалось собрать ни одного стека
                            
                          if(flagReverse==-1){j=2;flagReverse=0; continue;} //возвращаемся к прямому порядку сборки стека 
                          j++;    
                    
                        }//for(int j=1;j<wName.size();j++){
                    }//if(wName.size()==1)//проверка стековых букв
                                                                    //для всех собранных стеков 
                    int size=(int)wName.size();
                    DR("@@@collect stacks count="<<stackResult.size()-1<<" size="<<size<<endl)
                    for(int stInd=0;stInd<stackResult.size();stInd++){
                         DR("s="<<(short)stackResult[stInd].status)
                         if(!stackResult[stInd].status)continue;    //проверяем собрался ли стек
                         stackResult[stInd].name=name;
                         stackResult[stInd].nameUni=nameUni;
                         stackResult[stInd].correlation/=size;  //нормализуем корреляцию
                         if(name!="ལབ")stackResult[stInd].correlation+=3;     //увеличиваем приоритет стека над одиночной буквой
                         stackResult[stInd].Character=100;      //отмечаем стек как собраный
                         stackResult[stInd].OCRIndex='S';
                         line.push_back(stackResult[stInd]);
                         DR(endl<<"@@@line.size()="<<line.size()<<" name="<<name<<" c="<<stackResult[stInd].correlation<<" s="<<stackResult[stInd].status<<endl)
                        
                    }
                    DR(endl)
   
                    
            }//if(wName[0]==letterLine[i].uni){
                
        }//for(int ind=stepIndex;ind<letterLine
        //if(i>25)exit(0);
    }//for(int i=0;i<letterLine.size();i++)
    
    //DR("stepIndex="<<stepIndex<<" nextStepIndex="<<nextStepIndex<<endl;
    if(stepIndex==nextStepIndex)break;
    stepIndex=nextStepIndex;
    //DR(" new stepIndex="<<stepIndex<<endl;
    
    /*
    DR(" line.size()="<<line.size()<<endl)
    if(countStep==3){
        DR("draw line.size()="<<line.size()<<endl;
        drawMatchLine(line,draw, y0Base, y1Base);
        writeText(draw, "/1.html");
        exit(0);
    }
    */
    countStep++;
    
}

//TIME_PRINT_ADD
//TIME_PRINT_    

}
    




