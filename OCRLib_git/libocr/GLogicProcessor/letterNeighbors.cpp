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
void GLogicProcessor::letterNeighborsNew(vector<OCRMatch>&matchLine,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg){
    
    int print=0;
    
    
    DR("//****Start letterNeighbors ******* matchLine.size()="<<matchLine.size()<<endl)
    print=0;
    OCRBox s,sBox;
    
    for(int i=0;i<matchLine.size();i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(line[i].name=="ྱ")matchLine=1;
        //print=0; if(i==3167)print=1;   if(!print)continue;
        //cout<<"i="<<i<<" c="<<matchLine[4230].correlation<<" l="<<matchLine[4230].letterIndex<<endl;
        
        if(!matchLine[i].correlation)continue;  //||matchLine[i].OCRIndex=='X'||matchLine[i].OCRIndex=='S'||matchLine[i].OCRIndex=='W'||matchLine[i].OCRIndex=='R'||matchLine[i].OCRIndex=='V'||
        
        letterAImg->fillColumns32V(0, &matchLine[i].s);  //стираем букву
        letterLineArea(matchLine[i],lineImg32,letterAImg); //подсчитываем количество пикселов буквы совпадающих с фокальными линиями изображения; рисуем их в letterAImg
        DR("@@@"<<i<<" "<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" x0="<<matchLine[i].x0<<" x1="<<matchLine[i].x1
           <<" pCount="<<matchLine[i].pCount<<endl)
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
        
        
        for(int j=0;j<matchLine.size();j++){
            if(!matchLine[j].correlation||i==j)continue;
            int dC=matchLine[i].correlation-matchLine[j].correlation;
            
            
            //print=0; if(j==4230){print=1;}else{continue;}
            
            if(i==j||matchLine[j].OCRIndex=='X'||(matchLine[i].OCRIndex=='X'&&matchLine[j].name=="་"))continue;//||matchLine[j].OCRIndex=='S'
            
            //проверяем есть ли пересечение букв.
            intArea(&matchLine[i].s,&matchLine[j].s,&s);
            
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



// ******************************* Deprecated ********************


/** @brief для каждой гипотезы распознанной буквы проверяем две соседние буквы и буквы на месте самой буквы
 если буквы вложены друг в друга и меньшая буква имеет меньшую корреляцию оставляем большую букву
 Например в букве В есть две небольшие буквы O с меньшей корреляцией
 оставляем только те буквы, которые имеют хороших соседей и мирно с ними уживаются.*/
void GLogicProcessor::letterNeighbors(vector<OCRMatch>&matchLine){
    
    int print=0;
    
    DR("//****Start letterNeighbors ******* matchLine.size()="<<matchLine.size()<<endl)
    print=0;
    OCRBox s;
    
    for(int i=0;i<matchLine.size();i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(matchLine[i].name=="ང")print=1;
        //print=0; if(i==4733){print=1;}else{continue;}
        //cout<<"i="<<i<<" c="<<matchLine[25664].correlation<<endl;
        
        if(!matchLine[i].correlation||matchLine[i].OCRIndex=='W'||matchLine[i].OCRIndex=='R'||
           matchLine[i].OCRIndex=='V'||matchLine[i].OCRIndex=='X'||matchLine[i].name=="ར")continue;
        
        DR("@@@"<<i<<" "<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" x0="<<matchLine[i].x0<<" x1="<<matchLine[i].x1<<endl)
        
        for(int j=0;j<matchLine.size();j++){
            if(!matchLine[j].correlation)continue;
            //print=0; if(j==25664){print=1;}else{continue;}
            
            DR("I->"<<matchLine[i].name<<" c="<<matchLine[i].correlation<<" y0="<<matchLine[i].y0<<" y1="<<matchLine[i].y1<<" yC="<<matchLine[i].yCenter)
            DR("   J="<<j<<"->"<<matchLine[j].name<<" c="<<matchLine[j].correlation<<" y0="<<matchLine[j].y0<<" y1="<<matchLine[j].y1<<" yC="<<matchLine[j].yCenter<<endl)
            
            if(i==j||matchLine[j].OCRIndex=='W'||matchLine[j].OCRIndex=='R'||matchLine[j].OCRIndex=='V'||matchLine[j].OCRIndex=='X')continue;
            
            int dC=matchLine[j].correlation-matchLine[i].correlation;
            
            //проверяем есть ли пересечение букв.
            intArea(&matchLine[i].s,&matchLine[j].s,&s);
            if(s.area==0){
                DR("no intersection"<<endl)
                continue;
            }
            cout<<"s="<<s.area<<endl;
            
            
            //проверка соседей по горизонтали
            if(((matchLine[j].x0<matchLine[i].x1-10&&matchLine[j].xCenter>matchLine[i].x1-5)||
                (matchLine[j].x1>matchLine[i].x0+10&&matchLine[j].xCenter<matchLine[i].x0+5))&&
               (matchLine[j].yCenter<matchLine[i].y1-10&&matchLine[j].yCenter>matchLine[i].y0+10)
               ){
                if(dC>0){
                    DR(j<<"-x match nI="<<matchLine[i].name<<" nJ="<<matchLine[j].name<<" dC="<<dC<<endl)
                    matchLine[i].correlation=0;break;
                }else{
                    DR(i<<"-x matchJ nI="<<matchLine[i].name<<" nJ="<<matchLine[j].name<<" dC="<<dC<<endl)
                    matchLine[j].correlation=0;
                }
            }
            //проверка соседей по вертикали
            if((matchLine[j].xCenter>matchLine[i].x0&&matchLine[j].xCenter<matchLine[i].x1)&&
               (
                (matchLine[j].y0<matchLine[i].y1-10&&
                 matchLine[j].yCenter>matchLine[i].y1+5)||
                (matchLine[j].y1>matchLine[i].y0+10&&
                 matchLine[j].yCenter<matchLine[i].y0-5))
               ){
                
                if(dC>0){
                    DR(j<<"-y remove match I nI="<<matchLine[i].name<<" nJ="<<matchLine[j].name<<" dC="<<dC<<endl)
                    matchLine[i].correlation=0;break;
                }else{
                    DR(i<<"-y remove match J nI="<<matchLine[i].name<<" nJ="<<matchLine[j].name<<" dC="<<dC<<endl)
                    matchLine[j].correlation=0;
                } 
            }
            
             
            
            
            
            //for(int n=0;n<matchLine[i].mask32Vector.size();n++){
            //cout<<"<div style=\"position:absolute; top:"<<matchLine[i].mask32Vector[n].yMax<<
            //"px; left:"<<matchLine[i].mask32Vector[n].xMax<<"px; border:1px solid red;width:"<<matchLine[i].mask32Vector[n].imgW<<"px;height:"
            //<<matchLine[i].mask32Vector[n].mH<<"px\">\n";
            
            //}
        }
        
        
    }
    
    //exit(0);
    
    
    
}






