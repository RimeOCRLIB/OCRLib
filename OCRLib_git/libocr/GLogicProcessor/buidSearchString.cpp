#include "GLogicProcessor.h"
/** @brief  функция сборки пар букв для корреляционного поиска.
 Пары собираются из стеков, полученных в результате функции collectStackLetter
 пара содержит две буквы Unicode. Пары формируются с перекрытием (см. associativeSignalProcessing)
 для каждого стека последняя пара букв содержит одну букву из соседней стековой буквы с проверкой по шрифтовой геометрии
 */
//#define REMARK
#ifdef REMARK
#define DN(x) cout<<x;
#else
#define DN(x)
#endif

void GLogicProcessor::buildSearchString(vector<OCRMatch>&line,
                                        vector<OCRMatch>&dLine,
                                        vector<uint>&letterX,
                                        string &result){
    
    uint size=(uint)line.size();
    DN(" line.size()="<<line.size()<<endl)
    wstring letterStr;
    //drawGrapeLine(line); exit(0);
    for (int i=0;i<line.size();i++){
        if(!line[i].correlation)continue;
        DN("line["<<i<<"].nameUni="<<line[i].nameUni<<" line[i].name="<<line[i].name<<" nUni="<<line[i].nameUni<<" d="<<line[i].delimeter<<endl)
        if(line[i].OCRIndex=='Z'||line[i].OCRIndex=='N'){    //записываем знаки препинания в разделители слогов
            line[i].delimeter=line[i].name+line[i].delimeter;
            line[i].name="";
            continue;
        }
        
        string nYagpo;
        if(line[i].OCRIndex!='S'){
            nYagpo=tibetanUTFToYagpo(line[i].nameUni,1);
        }else{
            nYagpo=UnicodeToYagpo(line[i].nameUni);
        }
        line[i].wName=UTF_to_Unicode(nYagpo);
        if(line[i].correlation)DN("/ nYagpo="<<nYagpo<<" line["<<i<<"].OCRIndex "<<(char)line[i].OCRIndex<<" name="<<line[i].name<<" c="<<line[i].correlation<<endl);
        //if(line[i].correlation)DN("/ nYagpo="<<nYagpo<<" > "<<line[i].name<<endl)
        
    }
    //drawGrapeLine(line); exit(0);
    
    sort(line.begin(),line.end(),sortMatchXCenter);
    
    int dl=0; int letterCount=0;
    
    //записываем знаки препинания в разделители слогов той буквы после которой они следуют
    for (int i=0;i<line.size()-1;i++){
        if(!line[i].correlation)continue;
        if(line[i].OCRIndex!='Z'&&line[i].OCRIndex!='N'){
            dl+=line[i].letterW;
            letterCount++;
            for (int n=i+1;n<line.size();n++){
                if(!line[n].correlation)continue;
                if(line[n].x0>line[i].xCenter){
                    if(line[n].OCRIndex=='Z'||line[n].OCRIndex=='N'){
                        line[i].delimeter+=line[n].delimeter;
                    }else break;                }
            }
        }
    }
    
    //очень важное место. Тот же вопрос что и при разборе узора. Могут ли соседствовать две буквы.
    //считаем что максимальное расстояние между буквами равно не более чем трехкратной усредненной ширине буквы
    if(!letterCount)return;
    dl=(dl/letterCount)*3;
    
    //drawGrapeLine(line); exit(0);
    //int t=textCorpusGMap->testLetterP((ushort)line[843].wName[0],(ushort)line[950].wName[0]);
    //cout<<"t="<<t<<" w1="<<Unicode_to_UTF(line[843].wName)<<" w2="<<Unicode_to_UTF(line[950].wName);
    //exit(0);
    print=0;
    for (int i=0;i<line.size()-1;i++){  //cout<<"line[123].correlation="<<line[123].correlation<<endl;
        if(!line[i].correlation)continue;
        
        //print=0; if(i==5){print=1;}else{continue;}
        DN(" @@@3 "<<Unicode_to_UTF(line[i].wName)<<endl)
        
        if(line[i].OCRIndex=='Z'||line[i].OCRIndex=='N')continue;   //пара не может начинатся со знака препинания
        wstring strW=line[i].wName;
        
        if(line[i].delimeter.find("•")!=-1){   //пара не может начинатся со знака препинания
            //cout<<"break name = "<<line[n].name<<endl;
            continue;
        }
        
        string t=Unicode_to_UTF(line[i].wName);
        DN("@@@@test /"<<t<<"/"<<endl)
        //letterStr+=L"\n";
        //letterStr+=line[i].wName;
        //letterStr+=L"\n";
        //затем собираем все пары из стека и следующего стека. Проверяем шрифтовую геометрию
        int x1=line[i].x1;
        
        if(isTibDigit(line[i].name))dl=20;
        int n=i+1;
        int xLimit=line[i].x1+line[i].letterW*3;
        int xCenterLimit=50000;
        
        while(n<line.size()){
            if(!line[n].correlation){n++;continue;}
            DN("@@_"<<n<<"_@@@ x1="<<x1<<" x0_="<<line[n].x0<<" dl="<<dl<<" /"<<line[i].name<<"+"<<line[n].name<<"/"<<endl)
            //if(n==21){cout<<"111";}
            
            if(line[n].OCRIndex=='Z'||line[n].OCRIndex=='N'){  //пропускаем знаки препинания
                //line[i].delimeter+=line[n].delimeter;
                n++;continue;
            }
            
            
            if(line[n].xCenter>x1){//+line[n].letterW/4
                //if(!xLimit)xLimit=line[n].xCenter;
                
                if(line[n].x0>xLimit
                   ||line[n].x0-x1>dl
                   ||line[n].x0>xCenterLimit+dl/4
                   ){
                    xCenterLimit=50000;
                    break;
                }
                
                DN(Unicode_to_UTF(line[n].wName)<<"/"<<endl)
                xCenterLimit=line[n].xCenter;
                //if(line[n].OCRIndex=='Z'||line[n].OCRIndex=='W'||line[n].OCRIndex=='R'||line[n].OCRIndex=='V')continue;   //убираем части букв
                
                //проверяем есть ли между буквами предполагаемой пары более подходящие буквы
                /*int flag=0;
                 for(int m=i+1;m<n;m++){
                 if(!line[m].correlation)continue;
                 if(line[m].OCRIndex=='Z'||line[m].OCRIndex=='N'){  //пропускаем знаки препинания
                 continue;
                 }
                 if(line[m].xCenter>x1&&line[m].xCenter<line[n].x0){
                 flag=1;
                 xCenterLimit=50000;
                 break;
                 }
                 }
                 if(flag)break;
                 */
                
                
                //проверяем есть ли такая пара букв в словаре
                if(!textCorpusGMap->testLetterP((ushort)line[i].wName[0],(ushort)line[n].wName[0])){n++;continue;}
                DN(" line[i].name    "<<line[i].name<<" line[n]   "<<line[n].name<<endl)
                
                DN(" x1="<<x1<<" x0_="<<line[n].x0<<" dl="<<dl<<" /"<<line[i].name<<line[n].name<<"/"<<endl)
                OCRMatch dLetter;
                dLetter.name=Unicode_to_UTF(line[i].wName)+Unicode_to_UTF(line[n].wName);
                DN(" dLetter0.name="<<dLetter.name<<endl)
                dLetter.name=YagpoToUni(dLetter.name);
                DN(" dLetter1.name="<<dLetter.name<<endl)
                
                dLetter.wName=line[i].wName+line[n].wName;
                dLetter.x0=line[i].x0;
                dLetter.x1=line[n].x1;
                dLetter.xLC=line[i].x1;
                dLetter.xL1C=line[n].xCenter;
                
                if(line[i].y0<line[n].y0){
                    dLetter.y0=line[i].y0;
                }else{
                    dLetter.y0=line[n].y0;
                }
                if(line[i].y1>line[n].y1){
                    dLetter.y1=line[i].y1;
                }else{
                    dLetter.y1=line[n].y1;
                }
                dLetter.correlation=(line[i].correlation+line[n].correlation)/2;
                dLetter.status=0;
                dLetter.letterID=(int)dLine.size();
                dLetter.setSize(0);
                dLetter.letter.push_back(line[i]);  //нужно для сборки строки
                dLetter.letter.push_back(line[n]);  //нужно для сборки строки
                dLine.push_back(dLetter);
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
        //DN(" maxC="<<maxC<<" n="<<dLine[maxIndex].name;
        
        //для найденной пары с наибольшей корреляцией строим «гроздья винограда»
        //находим все пары, центр которых находится внутри габарита найденной пары
        vector<OCRMatch>grapeLine;
        int dl=dLine[maxIndex].letterW*(float)0.15;
        int x0=dLine[maxIndex].x0+dl;
        int x1=dLine[maxIndex].x1-dl;
        dLine[maxIndex].status=1;
        
        //DN(" x0="<<x0<<" x1="<<x1<<" dl="<<dl<<endl)
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
                
                DN((short)grapeLine[i].correlation<<" ")
            }else{
                dLine[grapeLine[i].letterID].correlation=0;
            }
        }
        DN(endl)
        
        
        
    }
    
    vector<OCRMatch>resultLine;
    
    //собираем пары в поисковую строку, пишем геометрические координаты
    for (int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation)continue;
        letterStr+=dLine[i].wName;//+L" ";
        letterX.push_back(dLine[i].x0);
        letterX.push_back(dLine[i].x1);
        resultLine.push_back(dLine[i]);
    }
    
    dLine=resultLine;
    result=Unicode_to_UTF(letterStr);
    
    
    
    //cout<<result; //exit(0);
    //drawGrapeLine(dLine);  exit(0);
}
