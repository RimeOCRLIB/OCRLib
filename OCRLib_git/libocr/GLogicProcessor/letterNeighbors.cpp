//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#include "GLogicProcessor.h"
#include "GImageEditor.h"
#include "php2stl.h"


/** @brief для каждой гипотезы распознанной буквы проверяем две соседние буквы и буквы на месте самой буквы
 если буквы вложены друг в друга и меньшая буква имеет меньшую корреляцию оставляем большую букву
 Например в букве В есть две небольшие буквы O с меньшей корреляцией
 оставляем только те буквы, которые имеют хороших соседей и мирно с ними уживаются.*/
void GLogicProcessor::letterNeighbors(vector<OCRMatch>&matchLine,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg){
    
    int print=0;
    int dX_=15;

    string xKey; xKey=(uchar)224; xKey+=(uchar)188; xKey+=(uchar)185;

    
    
    for(int i=0;i<matchLine.size();i++){  //cout<<"i="<<i<<" c="<<matchLine[277].correlation<<endl;
        if(!matchLine[i].correlation)continue;
        //cout<<i<<" name="<<matchLine[i].name<<" OCRIndex"<<(char)matchLine[i].OCRIndex<<endl;
        //cout<<"i="<<i<<" c="<<matchLine[i].correlation<<" y0Base="<<y0Base<<" y1Base="<<y1Base<<" y0="<<matchLine[i].y0<<endl;
        if(matchLine[i].OCRIndex=='S')continue;
        //if(matchLine[i].name=="་"&&(matchLine[i].y0<y0Base-10||matchLine[i].y0>y0Base+10)){
            //matchLine[i].name=".";
            //matchLine[i].correlation=0;
        //    continue;
        //}
        if((matchLine[i].name=="."||matchLine[i].name==",")&&(matchLine[i].y0<y1Base-10||matchLine[i].y1>y1Base+10)){matchLine[i].correlation=0;continue;}
        if(matchLine[i].name=="ར"&&matchLine[i].y0<y0Base-10){matchLine[i].correlation=0;continue;}
        if(matchLine[i].OCRIndex=='A'&&matchLine[i].y0<y0Base-20&&matchLine[i].name.find(xKey)==-1){matchLine[i].correlation=0;continue;}
        if(matchLine[i].OCRIndex=='Z'&&(matchLine[i].yCenter>y1Base+32||matchLine[i].yCenter<y0Base-32)){matchLine[i].correlation=0;continue;}
        if(matchLine[i].OCRIndex=='V'&&matchLine[i].yCenter>y0Base+dX_){matchLine[i].correlation=0;continue;}
        //if(matchLine[i].OCRIndex=='V'&&matchLine[i].y1<y0Base-matchLine[i].letterH*1.5){matchLine[i].OCRIndex='T';}
        if((matchLine[i].OCRIndex=='R'||matchLine[i].OCRIndex=='W')&&matchLine[i].y1<y1Base){matchLine[i].correlation=0;continue;}
        if(matchLine[i].OCRIndex!='V'&&matchLine[i].OCRIndex!='Z'&&matchLine[i].OCRIndex!='X'&&matchLine[i].OCRIndex!='N'&&matchLine[i].y0<y0Base-20){matchLine[i].correlation=0;continue;}
    }
    
    
    
    DR("//****Start letterNeighbors ******* matchLine.size()="<<matchLine.size()<<endl)
    print=0;
    OCRBox s,sBox;
    
    for(int i=0;i<matchLine.size();i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(line[i].name=="ྱ")matchLine=1;
        //print=0; if(i==2193)print=1;   if(!print)continue;
        //cout<<"i="<<i<<" c="<<matchLine[4230].correlation<<" l="<<matchLine[4230].letterIndex<<endl;
        
        if(!matchLine[i].correlation||matchLine[i].OCRIndex=='X'||matchLine[i].name=="ྲ")continue;  //||matchLine[i].OCRIndex=='X'||matchLine[i].OCRIndex=='S'||matchLine[i].OCRIndex=='W'||matchLine[i].OCRIndex=='R'||matchLine[i].OCRIndex=='V'||
        
        letterAImg->fillColumns32V(0, &matchLine[i].s);  //стираем букву
        letterLineArea(matchLine[i],lineImg32,letterAImg); //подсчитываем количество пикселов буквы совпадающих с фокальными линиями изображения; рисуем их в letterAImg
        DR("@@@"<<i<<" "<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" x0="<<matchLine[i].x0<<" x1="<<matchLine[i].x1
           <<" pCount="<<matchLine[i].pCount<<endl)
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
        
        
        for(int j=0;j<matchLine.size();j++){
            if(!matchLine[j].correlation||i==j)continue;
            int dC=matchLine[i].correlation-matchLine[j].correlation;
            
            
            //print=0; if(j==4230){print=1;}else{continue;}
            
            if(i==j||matchLine[j].OCRIndex=='X'||matchLine[j].name=="ྲ"||(matchLine[i].OCRIndex=='X'&&matchLine[j].name=="་"))continue;//||matchLine[j].OCRIndex=='S'
            
            //проверяем есть ли пересечение букв.
            intArea(matchLine[i].s,matchLine[j].s,s);
            
            if(s.area<matchLine[i].area/5&&s.area<matchLine[j].area/5){
                //DR("no intersection j="<<j<<endl)
                continue;
            }
                        
            //if(1)inputData.start=111;
            //подсчитываем количество пикселов пересечения двух букв. Рисуем и стираем букву B (matchLine[j]) в letterBImg
            letterLineArea(matchLine[j],lineImg32,letterBImg); //подсчитываем количество пикселов фокальных линий попадающих в площадь буквы
            
            //if(j==-1){
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1LetterB32.jpg",0);
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1LetterLine32.jpg",0);
            //exit(0);
            //}
            //cout<<"matchLine[j].pCount="<<matchLine[j].pCount<<" matchLine[i].area="<<matchLine[j].area<<endl;
            
            if(matchLine[j].pCount==0&&matchLine[j].correlation<95){   //переделать распознавание круглых точек
                DR("remove J "<<j<<" no area"<<endl);
                matchLine[j].correlation=0; continue;
            }
            
            int pArea=letterAImg->img32Union(letterBImg,&s);
            letterBImg->fillColumns32V(0, &matchLine[j].s);
            
            DR("pArea="<<pArea<<endl);
            
            if(!pArea)continue;
            
            DR("I "<<i<<"->"<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" y0="<<matchLine[i].y0<<" y1="<<matchLine[i].y1<<" yC="<<matchLine[i].yCenter)
            DR("   J="<<j<<"->"<<matchLine[j].name<<" c="<<matchLine[j].correlation<<" y0="<<matchLine[j].y0<<" y1="<<matchLine[j].y1<<" yC="<<matchLine[j].yCenter<<" x0="<<matchLine[j].x0<<" x1="<<matchLine[j].x1<<" pCountJ="<<matchLine[j].pCount<<endl)
            
            //cout<<"pArea="<<pArea<<" s.area="<<s.area<<" i="<<i<<" j="<<j<<endl; //print=1;
            //DR("I"<<i<<"->"<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" y0="<<matchLine[i].y0<<" y1="<<matchLine[i].y1<<" yC="<<matchLine[i].yCenter)
            //DR("   J="<<j<<"->"<<matchLine[j].name<<" c="<<matchLine[j].correlation<<" y0="<<matchLine[j].y0<<" y1="<<matchLine[j].y1<<" yC="<<matchLine[j].yCenter<<" x0="<<matchLine[j].x0<<" x1="<<matchLine[j].x1<<endl)
            //continue;
            //}else{
            //    //cout<<"pArea="<<pArea<<endl;
            //}
            DR("pArea="<<pArea<<endl);
            
            if(pArea>matchLine[i].pCount/2){  //две буквы претендуют на одно и тоже место
                DR("//@@@*** Union_I letter I "<<matchLine[i].name<<" i="<<i<<" cI="<<matchLine[i].correlation<<" pCountI="<<matchLine[i].pCount<<" //  letter J "<<matchLine[j].name<<" j="<<j
                   <<" cJ="<<matchLine[j].correlation<<" pCountJ="<<matchLine[j].pCount<<endl)
                if(dC>5&&matchLine[i].pCount>matchLine[j].pCount*0.75){
                    //dC>5 неустойчиво, нужна дополнительная проверка
                    DR("remove J"<<endl);
                    matchLine[j].correlation=0; continue;
                    
                }
                if((dC<-3&&matchLine[i].pCount*0.75<matchLine[j].pCount)||
                   (dC<7&&matchLine[i].pCount<matchLine[j].pCount*0.5)){
                    DR("remove I"<<endl);
                    matchLine[i].correlation=0; break;
                }
            }
            if(pArea>matchLine[j].pCount/2){  //две буквы претендуют на одно и тоже место
                DR("//@@@*** Union_II"<<endl)
                if(dC>7||(matchLine[i].pCount>matchLine[j].pCount*1.1&&dC>0)||(matchLine[i].pCount>matchLine[j].pCount*1.2&&dC>-4)){
                    DR("remove J"<<endl);
                    matchLine[j].correlation=0; continue;
                }
                if(pArea>matchLine[i].pCount*0.7&&dC<-2&&(matchLine[j].pCount>matchLine[i].pCount*0.85)){
                    DR("remove I"<<endl);
                    matchLine[i].correlation=0; break;
                }
            }
            if(pArea>matchLine[j].pCount/3){
                DR("//@@@*** Union_III"<<endl)
                
                if(matchLine[i].pCount>matchLine[j].pCount*1.1&&dC>-4){
                    DR("remove J"<<endl);
                    matchLine[j].correlation=0; continue;
                }
                if(matchLine[j].pCount>matchLine[i].pCount*1.1&&dC<-4){
                    DR("remove I"<<endl);
                    matchLine[i].correlation=0; break;
                }
            }
            
            
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
            //exit(0);
            
        }
    }
    
    //exit(0);
    
    
    
}
