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
    int dX_=15;

    string xKey; xKey=(uchar)224; xKey+=(uchar)188; xKey+=(uchar)185;

    
    
    for(int i=0;i<matchLine.size();i++){  //cout<<"i="<<i<<" c="<<matchLine[277].correlation<<endl;
        OCRMatch &a=matchLine[i];
        if(!a.correlation)continue;
        //cout<<i<<" name="<<a.name<<" OCRIndex"<<(char)a.OCRIndex<<endl;
        //cout<<"i="<<i<<" c="<<a.correlation<<" y0Base="<<y0Base<<" y1Base="<<y1Base<<" y0="<<a.y0<<endl;
        if(a.OCRIndex=='S')continue;
        //if(a.name=="་"&&(a.y0<y0Base-10||a.y0>y0Base+10)){
            //a.name=".";
            //a.correlation=0;
        //    continue;
        //}
        if((a.name=="."||a.name==",")&&(a.y0<y1Base-10||a.y1>y1Base+10)){a.correlation=0;continue;}
        if(a.name=="ར"&&a.y0<y0Base-10){a.correlation=0;continue;}
        if(a.OCRIndex=='A'&&a.y0<y0Base-20&&a.name.find(xKey)==-1){a.correlation=0;continue;}
        if(a.OCRIndex=='Z'&&(a.yCenter>y1Base+32||a.yCenter<y0Base-32)){a.correlation=0;continue;}
        if(a.OCRIndex=='V'&&a.yCenter>y0Base+dX_){a.correlation=0;continue;}
        //if(a.OCRIndex=='V'&&a.y1<y0Base-a.letterH*1.5){a.OCRIndex='T';}
        if((a.OCRIndex=='R'||a.OCRIndex=='W')&&a.y1<y1Base){a.correlation=0;continue;}
        if(a.OCRIndex!='V'&&a.OCRIndex!='Z'&&a.OCRIndex!='X'&&a.OCRIndex!='N'&&a.y0<y0Base-20){a.correlation=0;continue;}
    }
    
    //drawGrapeLine(matchLine); exit(0);
    
    
    DR("//****Start letterNeighbors ******* matchLine.size()="<<matchLine.size()<<endl)
    print=0;
    OCRBox s,sBox;
    
    for(int i=0;i<matchLine.size();i++){ //if(step>1000){DR(", "<<i; step=0;}step++;
        //print=0; if(a.name=="र")print=1;   if(!print)continue;
        //print=0; if(i==8838)print=1;   if(!print)continue;
        //cout<<"i="<<i<<" c="<<matchLine[1089].correlation<<" l="<<matchLine[1089].letterIndex<<endl;
        OCRMatch &a=matchLine[i];
        
        if(!a.correlation||
           a.name=="་"||
           //(a.name=="།"&&a.delimeter.find("•")==-1)||
           a.OCRIndex=='X'||
           a.name=="ྲ"||
           a.name=="ः"||
           a.name=="र"
           )continue;  //||a.OCRIndex=='X'||a.OCRIndex=='S'||a.OCRIndex=='W'||a.OCRIndex=='R'||a.OCRIndex=='V'||
        
        letterAImg->fillColumns32V(0, &a.s);  //стираем букву
        letterLineArea(a,lineImg32,letterAImg); //подсчитываем количество пикселов буквы совпадающих с фокальными линиями изображения; рисуем их в letterAImg
        DR("@@@"<<i<<" "<<a.name<<" c="<<a.correlation<<" x0="<<a.x0<<" x1="<<a.x1
           <<" pCount="<<a.pCount<<endl)
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
        
        
        for(int j=0;j<matchLine.size();j++){
            if(!matchLine[j].correlation||i==j)continue;
            OCRMatch &b=matchLine[j];
            int dC=a.correlation-b.correlation;
            
            
            //print=0; if(j==8883){print=1;}else{continue;}
            
            if(i==j||
               b.OCRIndex=='X'||
               b.name=="ྲ"||
               a.OCRIndex=='X'||
               (a.name=="ད"&&b.name=="ང")||
               (b.name=="ད"&&a.name=="ང")||
               //(b.name=="།"&&b.delimeter.find("•")==-1)||
               b.name=="ः"||
               b.name=="र"
               )continue;//||b.OCRIndex=='S'
            
            //проверяем есть ли пересечение букв.
            intArea(a.s,b.s,s);
            
            if(s.area<a.area/4&&s.area<b.area/4&&!(a.name=="།"&&a.delimeter.find("•")!=-1)){
                //DR("no intersection j="<<j<<endl)
                continue;
            }
                        
            //if(1)inputData.start=111;
            //подсчитываем количество пикселов фокальных линий в области пересечения двух букв. Рисуем и стираем букву B (b) в letterBImg
            
            letterLineArea(b,lineImg32,letterBImg); //подсчитываем количество пикселов фокальных линий попадающих в площадь буквы B
            
            //if(j==-1){
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1LetterB32.jpg",0);
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1LetterLine32.jpg",0);
            //exit(0);
            //}
            //cout<<"b.pCount="<<b.pCount<<" a.area="<<b.area<<endl;
            
            //if(b.pCount==0&&b.correlation<95){   //переделать распознавание круглых точек
            //    DR("remove J "<<j<<" no area"<<endl);
            //    b.correlation=0; continue;
            //}
            
            int pArea=letterAImg->img32Union(letterBImg,&s);  //подсчитываем количество пикселов фокальных линий перекрытых областями ON буквы B и буквы А
            letterBImg->fillColumns32V(0, &b.s);
            
            DR("pArea="<<pArea<<endl);
            
            if(!pArea)continue;
            
            DR("I "<<i<<"->"<<a.name<<" c="<<a.correlation<<" y0="<<a.y0<<" y1="<<a.y1<<" yC="<<a.yCenter)
            DR("   J="<<j<<"->"<<b.name<<" c="<<b.correlation<<" y0="<<b.y0<<" y1="<<b.y1<<" yC="<<b.yCenter<<" x0="<<b.x0<<" x1="<<b.x1<<" pCountJ="<<b.pCount<<endl)
            
            //cout<<"pArea="<<pArea<<" s.area="<<s.area<<" i="<<i<<" j="<<j<<endl; //print=1;
            //DR("I"<<i<<"->"<<a.name<<" c="<<a.correlation<<" y0="<<a.y0<<" y1="<<a.y1<<" yC="<<a.yCenter)
            //DR("   J="<<j<<"->"<<b.name<<" c="<<b.correlation<<" y0="<<b.y0<<" y1="<<b.y1<<" yC="<<b.yCenter<<" x0="<<b.x0<<" x1="<<b.x1<<endl)
            //continue;
            //}else{
            //    //cout<<"pArea="<<pArea<<endl;
            //}
            DR("pArea="<<pArea<<endl);
            
            if(pArea>a.pCount/3){  //две буквы претендуют на одно и тоже место
                DR("//@@@*** Union_I letter I "<<a.name<<" i="<<i<<" cI="<<a.correlation<<" pCountI="<<a.pCount<<" //  letter J "<<b.name<<" j="<<j
                   <<" cJ="<<b.correlation<<" pCountJ="<<b.pCount<<endl)
                if(dC>5&&a.pCount>b.pCount*0.75){
                    //dC>5 неустойчиво, нужна дополнительная проверка
                    DR("remove J"<<endl);
                    b.correlation=0;
                    if(b.delimeter.find("•")!=-1&&a.delimeter.find("•")==-1)a.delimeter+="•";
                    continue;
                    
                }
                if((dC<-3&&a.pCount*0.75<b.pCount)||
                   (dC<7&&a.pCount<b.pCount*0.5)){
                    DR("remove I"<<endl);
                    a.correlation=0;
                    if(a.delimeter.find("•")!=-1&&b.delimeter.find("•")==-1)b.delimeter+="•";
                    break;
                }
            }
            if(pArea>b.pCount/3){  //две буквы претендуют на одно и тоже место
                DR("//@@@*** Union_II"<<endl)
                if(dC>7||(a.pCount>b.pCount*1.1&&dC>0)||(a.pCount>b.pCount*1.2&&dC>-4)){
                    DR("remove J"<<endl);
                    b.correlation=0;
                    if(b.delimeter.find("•")!=-1&&a.delimeter.find("•")==-1)a.delimeter+="•";
                    continue;
                }
                if(pArea>a.pCount*0.7&&dC<-2&&(b.pCount>a.pCount*0.85)){
                    DR("remove I"<<endl);
                    a.correlation=0;
                    if(a.delimeter.find("•")!=-1&&b.delimeter.find("•")==-1)b.delimeter+="•";
                    break;
                }
            }
            /*
            if(pArea>b.pCount/3){
                DR("//@@@*** Union_III"<<endl)
                
                if(a.pCount>b.pCount*1.1&&dC>-4){
                    DR("remove J"<<endl);
                    b.correlation=0; continue;
                }
                if(b.pCount>a.pCount*1.1&&dC<-4){
                    DR("remove I"<<endl);
                    a.correlation=0; break;
                }
            }
            */
            
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
            //exit(0);
            
        }
    }
    
    //exit(0);
    
    
    
}


