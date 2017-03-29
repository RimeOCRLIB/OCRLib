#include "GLogicProcessor.h"
//компрессия. Все одинаковые буквы в пределах габарита буквы
//заменяются на одну букву с макcмимальной корреляцией
void GLogicProcessor::compressMatch(vector<OCRMatch>&letterLine){
    int print=0;
    
    for(int i=0;i<letterLine.size();i++){
        letterLine[i].status=0;
        //DR(" line[i]="<<line[i].name<<" c="<<line[i].correlation<<endl;
        
    }

    sort(letterLine.begin(),letterLine.end(),sortMatchXCenter);
    //drawGrapeLine(letterLine); exit(0);
    int count=(int)letterLine.size();
    //int step=0;
    
    for(int i=0;i<count;i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(line[i].name=="ྱ")print=1;
        //cout<<i<<" "<<letterLine[518].correlation<<endl;
        if(!letterLine[i].correlation)continue;
        DR(letterLine[i].name<<" c="<<letterLine[i].correlation<<" x="<<letterLine[i].xCenter<<" i="<<i<<endl)
        if(letterLine[i].status)continue;
        //проверяем, есть ли впереди гипотезы той же буквы. Если есть выбираем букву с наибольшей корреляцией,
        //и наибольшей общей площадью совпадающей с изображением на странице.
        //остальные затираем
        int dX=24;
        string matchStr=letterLine[i].name+letterLine[i].delimeter;
        int xLimit=letterLine[i].xCenter+dX; 
        int index=i+1;

        while(index<count&&letterLine[index].xCenter<xLimit){

            if(letterLine[index].status){index++;continue;}
            if(letterLine[index].name+letterLine[index].delimeter==matchStr&&
               letterLine[index].OCRIndex==letterLine[i].OCRIndex){
               //abs(letterLine[index].yCenter-letterLine[i].yCenter)<20&&
               //abs(letterLine[index].xCenter-letterLine[i].xCenter)<10){
                OCRBox s;
                //проверяем есть ли пересечение букв.
                intArea(letterLine[index].s,letterLine[i].s,s);
                if(s.area>letterLine[index].s.area*0.75||s.area>letterLine[i].s.area*0.75||(s.area&&letterLine[i].OCRIndex=='Z')){
                    float cL=(float)letterLine[i].correlation/letterLine[index].correlation;
                    float pL=1;
                    if(fabs(1-cL)<=0.03)pL=(float)letterLine[i].pCount/letterLine[index].pCount;
                    float sL=(float)(cL+pL)/2;
                    
                    if(sL>=1){
                        if(letterLine[index].correlation>letterLine[i].correlation){
                            letterLine[i].correlation=letterLine[index].correlation;
                        }
                        letterLine[index].correlation=0;
                        letterLine[index].status=1;
                        if(letterLine[index].Character!=100)letterLine[i].Character=0; //если заменяемая буква не блок OpenType то и заменяющая становится отдельной
                        letterLine[index].letterID=index;
                    }else{
                        //затираем букву, передвигаем лимит
                        letterLine[i].correlation=0;
                        if(letterLine[i].Character!=100)letterLine[index].Character=0;
                        letterLine[i].status=1;//
                        xLimit=letterLine[index].xCenter+dX;
                        letterLine[i].letterID=i;
                        break;
                    }
                }
            }
            index++;
        }
        DR(letterLine[i].name<<"/___ c="<<letterLine[i].correlation<<" x="<<letterLine[i].xCenter<<endl)
    }

}
//компрессия. Все одинаковые буквы в пределах габарита буквы
//заменяются на одну букву с макcмимальной корреляцией
void GLogicProcessor::compressMatchPrint(vector<OCRMatch>&letterLine){
    int print=0;
    
    for(int i=0;i<letterLine.size();i++){
        letterLine[i].status=0;
        //DR(" line[i]="<<line[i].name<<" c="<<line[i].correlation<<endl;
        
    }
    
    sort(letterLine.begin(),letterLine.end(),sortMatchX0);
    //drawGrapeLine(line); exit(0);
    int count=(int)letterLine.size();
    //int step=0;
    
    for(int i=0;i<letterLine.size();i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(line[i].name=="ྱ")print=1;
        DR(letterLine[i].name<<" c="<<letterLine[i].correlation<<" x="<<letterLine[i].xCenter<<" i="<<i<<endl)
        if(letterLine[i].status)continue;
        //проверяем, есть ли впереди гипотезы той же буквы. Если есть выбираем букву с наибольшей корреляцией,
        //остальные затираем
        int dX=24;
        string matchStr=letterLine[i].name;
        int xLimit=letterLine[i].xCenter+dX;
        int index=i+1;
        
        while(index<count&&letterLine[index].xCenter<xLimit){
            if(letterLine[index].status){index++;continue;}
            if(letterLine[index].name==matchStr&&abs(letterLine[index].yCenter-letterLine[i].yCenter)<10){
                if(letterLine[index].correlation<=letterLine[i].correlation){
                    letterLine[index].correlation=0;
                    letterLine[index].status=1;
                    if(letterLine[index].Character!=100)letterLine[i].Character=0; //если заменяемая буква не блок OpenType то и заменяющая становится отдельной
                }else{
                    //затираем букву, передвигаем лимит
                    letterLine[i].correlation=0;
                    if(letterLine[i].Character!=100)letterLine[index].Character=0;
                    letterLine[i].status=1;//
                    xLimit=letterLine[index].xCenter+dX;
                    break;
                }
            }
            index++;
        }
        DR(letterLine[i].name<<"/___ c="<<letterLine[i].correlation<<" x="<<letterLine[i].xCenter<<endl)
    }
    
}
