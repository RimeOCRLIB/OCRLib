#include "GLogicProcessor.h"
/** @brief  функция сборки пар букв для корреляционного поиска.
 Пары собираются из стеков, полученных в результате функции collectStackLetter
 пара содержит две буквы Unicode. Пары формируются с перекрытием (см. associativeSignalProcessing)
 для каждого стека последняя пара букв содержит одну букву из соседней стековой буквы с проверкой по шрифтовой геометрии
 */

void GLogicProcessor::buildSearchStringNew(vector<OCRMatch>&line,
                                           vector<OCRMatch>&dLine,
                                           vector<uint>&letterX,
                                           string &result,
                                           GBitmap *lineImg32,
                                           GBitmap* letterAImg,
                                           GBitmap* letterBImg){
    
    
    uint size=(uint)line.size();
    if(!size)return;
    DR(" line.size()="<<line.size()<<endl)
    wstring letterStr;
    //drawGrapeLine(line); exit(0);
    for (int i=0;i<line.size();i++){
        OCRMatch &c=line[i];
        if(!c.correlation)continue;
        DR("line["<<i<<"].nameUni="<<c.nameUni<<" c.name="<<c.name<<" nUni="<<c.nameUni<<" d="<<c.delimeter<<endl)
        if(c.OCRIndex=='Z'||c.OCRIndex=='N'){    //записываем знаки препинания в разделители слогов
            c.delimeter=c.name+c.delimeter;
            c.name="";
            continue;
        }

        string nYagpo;
        if(c.OCRIndex!='S'){
            nYagpo=tibetanUTFToYagpo(c.nameUni,1);
        }else{
            nYagpo=UnicodeToYagpo(c.nameUni);
        }
        c.wName=UTF_to_Unicode(nYagpo);
        if(c.correlation)DR("/ nYagpo="<<nYagpo<<" line["<<i<<"].OCRIndex "<<(char)c.OCRIndex<<" name="<<c.name<<" c="<<c.correlation<<endl);
        //if(c.correlation)DR("/ nYagpo="<<nYagpo<<" > "<<c.name<<endl)
        
    }
    //drawGrapeLine(line); exit(0);
    
    sort(line.begin(),line.end(),sortMatchXCenter);
    
    //drawGrapeLine(line); exit(0);

    int dl=0; int letterCount=0;
   

    for (int i=0;i<line.size();i++){
        OCRMatch &c=line[i];
        if(!c.correlation)continue;
        if(c.OCRIndex!='Z'&&c.OCRIndex!='N'){
            dl+=c.letterW;
            letterCount++;
        }
    }
    
    
    //очень важное место. Тот же вопрос что и при разборе узора. Могут ли соседствовать две буквы.
    //считаем что максимальное расстояние между буквами равно не более чем двухкратной усредненной ширине буквы
    if(!letterCount)return;
    dl=(dl/letterCount)*2;
    
    //drawGrapeLine(line); exit(0);
    //int t=textCorpusGMap->testLetterP((ushort)line[843].wName[0],(ushort)line[950].wName[0]);
    //cout<<"t="<<t<<" w1="<<Unicode_to_UTF(line[843].wName)<<" w2="<<Unicode_to_UTF(line[950].wName);//exit(0);
    //print=1;
    for (int i=0;i<line.size()-1;i++){
        OCRMatch &c=line[i];
        if(!c.correlation)continue;
        
        if(c.OCRIndex=='Z'||c.OCRIndex=='N'||c.OCRIndex=='V'
           ||c.OCRIndex=='W'||c.OCRIndex=='X'||c.OCRIndex=='R'){  //пропускаем знаки препинания, номера и огласовки
            //c.delimeter+=line[n].delimeter;
            continue;
        }

        //print=0; if(i==5){print=1;}else{continue;}
        DR(" @@@3 "<<Unicode_to_UTF(c.wName)<<" ind:"<<c.letterIndex<<endl)
        wstring strW=c.wName;
        
        if(c.delimeter.find("•")!=-1){   //пара не может начинатся со знака препинания
            //cout<<"break name = "<<line[n].name<<endl;
            continue;
        }
        
        string t=Unicode_to_UTF(c.wName);
        DR("@@@@test /"<<t<<"/"<<endl)
        //letterStr+=L"\n";
        //letterStr+=c.wName;
        //letterStr+=L"\n";
        //затем собираем все пары из стека и следующего стека. Проверяем шрифтовую геометрию
        int x1=c.x1;
        
        if(isTibDigit(c.name))dl=20;
        int n=i+1;
        int xLimit=c.x1+c.letterW*3;
        int xCenterLimit=50000;
        
        while(n<line.size()){
            if(!line[n].correlation){n++;continue;}
            DR("@@_"<<n<<"_@@@ x1="<<x1<<" x0_="<<line[n].x0<<" dl="<<dl<<" /"<<c.name<<"+"<<line[n].name<<"/"<<endl)
            //if(n==21){cout<<"111";}
            
            if(line[n].OCRIndex=='Z'||line[n].OCRIndex=='N'||line[n].OCRIndex=='V'
               ||line[n].OCRIndex=='W'||line[n].OCRIndex=='X'||line[n].OCRIndex=='R'){  //пропускаем знаки препинания и огласовки
                //c.delimeter+=line[n].delimeter;
                n++;continue;
            }
            
            if(line[n].yCenter<c.y0||c.yCenter<line[n].y0){
                n++;continue;
            }
            
            if(line[n].xCenter>x1-24){//+line[n].letterW/4
                //if(!xLimit)xLimit=line[n].xCenter;
                
                if(line[n].x0>xLimit
                   ||line[n].x0-x1>dl
                   ||line[n].x0>xCenterLimit  //+dl/4
                   ){
                    xCenterLimit=50000;
                    break;
                }
                
                DR(Unicode_to_UTF(line[n].wName)<<"/"<<endl)
                //if(line[n].OCRIndex=='Z'||line[n].OCRIndex=='W'||line[n].OCRIndex=='R'||line[n].OCRIndex=='V')continue;   //убираем части букв
                
                //проверяем есть ли такая пара букв в словаре
                if(!textCorpusGMap->testLetterP((ushort)c.wName[0],(ushort)line[n].wName[0])){n++;continue;}
                DR(" line["<<i<<"].name    "<<c.name<<" line["<<n<<"]   "<<line[n].name<<endl)
                
                //необходимо проверить насколько буквы пар перекрывают друг друга.
                //пары сильно перекрывающихся букв убираем.
                OCRBox s;
                //проверяем есть ли пересечение букв.
                intArea(c.s,line[n].s,s);
                
                if(s.area){
                    letterArea(c,letterAImg); //подсчитываем количество пикселов в букве; рисуем ее в letterBImg
                    letterArea(line[n],letterBImg); //подсчитываем количество пикселов в букве; рисуем ее в letterBImg
                    int pArea=letterAImg->img32Union(letterBImg,&s);
                    letterAImg->fillColumns32V(0, &c.s);
                    letterBImg->fillColumns32V(0, &line[n].s);
                    if(pArea>c.pCount/3||pArea>line[n].pCount/3){
                        DR("intersection area too big"<<endl)
                        n++; continue;
                    }
                }
                
                DR(" x1="<<x1<<" x0_="<<line[n].x0<<" dl="<<dl<<" /"<<c.name<<line[n].name<<"/"<<endl)
                OCRMatch dLetter;
                dLetter.name=Unicode_to_UTF(c.wName)+Unicode_to_UTF(line[n].wName);
                DR(" dLetter0.name="<<dLetter.name<<endl)
                dLetter.name=YagpoToUni(dLetter.name);
                DR(" dLetter1.name="<<dLetter.name<<endl)
                
                dLetter.wName=c.wName+line[n].wName;
                dLetter.x0=c.x0;
                dLetter.x1=line[n].x1;
                dLetter.xLC=c.x1;
                dLetter.xL1C=line[n].xCenter;
                
                if(c.y0<line[n].y0){
                    dLetter.y0=c.y0;
                }else{
                    dLetter.y0=line[n].y0;
                }
                if(c.y1>line[n].y1){
                    dLetter.y1=c.y1;
                }else{
                    dLetter.y1=line[n].y1;
                }
                
                dLetter.correlation=(c.correlation+line[n].correlation)/2;
                dLetter.status=0;
                dLetter.letterID=(int)dLine.size();
                dLetter.setSize(1);
                dLetter.setSize(0);
                dLetter.line.push_back(i);  //нужно для сборки строки
                dLetter.line.push_back(n);  //нужно для сборки строки
                dLetter.dIndex.push_back(0);
                dLetter.dIndex.push_back(0);
                dLine.push_back(dLetter);
                xCenterLimit=line[n].xCenter; //устанавливаем ограничение на то, что следующие буквы в паре не должны быть дальше середины найденой второй буквы пары
            }
            
            n++;
        }
    }
    
    //drawGrapeLine(dLine); exit(0);
    
    //получившиеся пары необходимо проверить на «гроздья винограда»
    //на месте каждой пары находится до 10 пар, претендующих на это место
    //для словарного анализа необходимо сократить это количество до 3 пар на одно место
    size=(uint)dLine.size();
    while(1){
        //находим пару с наибольшей корреляцией
        int maxC=0,maxIndex=-1;
        for(int i=0;i<size;i++){
            if(dLine[i].status)continue;
            if(dLine[i].correlation>maxC){
                maxC=dLine[i].correlation;
                maxIndex=i;
            }
        }
        if(maxIndex<0)break; //разбор закончен
        //DR(" maxC="<<maxC<<" n="<<dLine[maxIndex].name;
        
        //для найденной пары с наибольшей корреляцией строим «гроздья винограда»
        //находим все пары, центр которых находится внутри габарита найденной пары
        vector<OCRMatch>grapeLine;
        int dl=dLine[maxIndex].letterW*(float)0.15;
        int x0=dLine[maxIndex].x0+dl;
        int x1=dLine[maxIndex].x1-dl;
        dLine[maxIndex].status=1;
        
        //DR(" x0="<<x0<<" x1="<<x1<<" dl="<<dl<<endl)
        //exit(0);
        int i=maxIndex+1;
        while(i<size){
            if(dLine[i].status){i++;continue;}
            if(dLine[i].xCenter>x0&&dLine[i].xCenter<x1){
                grapeLine.push_back(dLine[i]);
                dLine[i].status=1;
            }
            if(dLine[i].x0>dLine[maxIndex].x1+dl)break;
            i++;
        }
        i=maxIndex-1;
        while(i>-1){
            if(dLine[i].status){i--;continue;}
            if(dLine[i].xCenter>x0&&dLine[i].xCenter<x1){
                grapeLine.push_back(dLine[i]);
                dLine[i].status=1;
            }
            if(dLine[i].x1<dLine[maxIndex].x0-dl)break;
            i--;
        }
        sort(grapeLine.begin(),grapeLine.end(),sortMatchC);
        
        for(int i=0;i<grapeLine.size();i++){
            if(grapeLine[i].correlation>grapeLine[0].correlation*0.97||
               (i<5&&grapeLine[i].correlation>grapeLine[0].correlation*0.8)){
                
                DR((short)grapeLine[i].correlation<<" ")
            }else{
                dLine[grapeLine[i].letterID].correlation=0;
            }
        }
        DR(endl)
        
        
        
    }
    
    vector<OCRMatch>resultLine;
    
    //собираем пары в поисковую строку, пишем геометрические координаты
    for (int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation)continue;
        resultLine.push_back(dLine[i]);
        if(dLine[i].OCRIndex=='Z'||dLine[i].OCRIndex=='N')continue;
        letterStr+=dLine[i].wName;//+L" ";
        letterX.push_back(dLine[i].x0);
        letterX.push_back(dLine[i].x1);
    }
    
    dLine=resultLine;
    result=Unicode_to_UTF(letterStr);
    
    
    
    //cout<<result; //exit(0);
    //drawGrapeLine(dLine);  exit(0);
}





