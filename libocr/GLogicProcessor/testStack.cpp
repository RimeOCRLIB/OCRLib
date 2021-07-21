#include "GLogicProcessor.h"
#include "../GImageEditor/GImageEditor.h"

/** @brief  функция проверки взаимного положения пары букв  */

void GLogicProcessor::testStackLetter(vector<OCRMatch>&line,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg){
    
    int print=0;
    int index;
    int dX=32;
    int dX_=dX/3;
    //int dCMax=12;
    string xKey; xKey=(char)0xE0; xKey+=(char)0xBC; xKey+=(char)0xB9;
    
    //стираем огласовки и приписные буквы
    for(int i=0;i<line.size();i++){
        if(line[i].OCRIndex=='V'||line[i].OCRIndex=='R'||line[i].OCRIndex=='X'){
            line[i].correlation=0;
        }
        if(line[i].OCRIndex=='W'&&line[i].name=="འ")line[i].OCRIndex='A';
        //else{
        //   line[i].correlation=line[i].correlationNew;
        //}
        //cout<<"i="<<i<<"letter->correlationNew="<<line[i].correlationNew<<endl;
    }
    
    
    int size=(int)line.size();
    
    
    //letterArea(line[54],letterAImg);
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
    
    //drawGrapeLine(line);exit(0);
    
    //    for(int i=0;i<size;i++){ //print=1;
    
    //        OCRMatch *letter=&line[i];
    //        if(!letter->correlation)continue;
    //if(letter->name!="རྕ")continue;
    //cout<<"letter.letter.size()="<<letter->letter.size()<<endl; exit(0);
    
    
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
    
    //        letterAImg->fillColumns32V(0, &line[i].s);  //стираем букву
    
    
    //    }
    
    
    for(int i=0;i<size;i++){
        
        OCRMatch *letter=&line[i];
        
        //cout<<i<<" "<<(char)letter->OCRIndex<<" "<<letter->correlation<<" "<<letter->name<<endl;
        
        if(!letter->correlation)continue;
        if(letter->OCRIndex=='V'||letter->OCRIndex=='W'||letter->OCRIndex=='R'||letter->OCRIndex=='X')continue; //пропускаем OpenType
        
        //проверяем, есть ли буквы более вероятные чем тестовая.
        //print=0; if(letter->name=="ལྷ")print=1;   if(!print)continue;
        //print=0; if(i==217)print=1;   if(!print)continue;
        //cout<<"i="<<i<<" c="<<line[117].correlation<<" n="<<line[117].name<<endl;
        //cout<<" i1="<<line[128].letter[0].letterIndex<<" i2="<<line[128].letter[1].letterIndex<<endl;
        
        letterAImg->fillColumns32V(0, &line[i].s);  //стираем букву
        //проверяем, есть ли буквы более вероятные чем тестовая.
        //inputData.start=111;
        
        line[i].drawPict32(letterAImg,0,0,XOR_MODE);
        
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
        
        //letterLineArea(line[i],lineImg32,letterAImg); //подсчитываем количество пикселов в букве; рисуем ее в letterAImg
    
        
        //сначала проверяем букву на соответствие графическому изображению на странице.
        //в пределах габарита буквы +-dY проверяем количество нераспознанных черных пикселов
        OCRBox s=line[i].s;
        s.y0=MATRIX_BORDER; s.y1=lineImg32->rows()-MATRIX_BORDER;
        //inputData.start=111;
        if(s.x1>lineImg32->columns()){
            cout<<"MEMORY ERROR0 s->x1:"<<s.x1;
            exit(0);
        }
        
        float pAreaPage=lineImg32->pixelCount(&s);
        
        line[i].pCount=lineImg32->img32UnionLine(letterAImg, &s);
        
        line[i].correlationNew=(pAreaPage+1)/(line[i].pCount+1);
        //количество нераспознанных пикселов (без нуля)
        DR(i<<" pAreaPage="<<pAreaPage<<" pCount="<<line[i].pCount<<" cN="<<line[i].correlationNew<<" n="<<line[i].name
           <<" x0="<<s.x0<<" x1="<<s.x1<<endl)
        
        
        if(letter->OCRIndex=='Z'){ dX_=10;}
        //int xLimit=letter->xCenter+dX;
        //int xLimit_=letter->xCenter-dX;
        int dir=1;
        //int flagReplace;
        int count=0;

        
        while(dir!=-1){
            //if(!letter->correlationNew)break;
            index=i+1;
            if(count==1){
                dir=-1;
                index=i-1;
            }
            if(dir>0) {DR(" test right"<<endl);} else {DR(" test left"<<endl)}
            
            while(1){
                if(index==-1||index==line.size())break;
                if(!line[index].correlation){ index+=dir; continue;}
                if(!letter->letter.size()&&!line[index].letter.size()){index+=dir; continue;} //не сравниваем одиночные буквы
                
                //проверяем есть ли пересечение букв.
                OCRBox s;
                intArea(letter->s,line[index].s,s);

                //if(index!=217){ index+=dir; continue;}
                //DR(" index="<<index<<" s.area="<<s.area<<" letter->area="<<letter->area<<" line[index].area="<<line[index].area<<endl);
                
                if(s.area<letter->area/5&&s.area<line[index].area/5){
                    //DR("no intersection"<<endl)
                    index+=dir;
                    continue;
                }
                //строим изображение тестовой буквы
                letterBImg->fillColumns32V(0, &line[index].s);  //стираем букву
                letterLineArea(line[index],lineImg32,letterBImg); //подсчитываем количество пикселов в букве; рисуем ее в letterBImg
                //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
                //проверяем есть ли пересечение двух букв по отношению к фокальным линиям изображения
                int pArea=letterAImg->img32Union(letterBImg,&s);
                if(!pArea&&letter->name!="ར"){ //(pArea<line[index].pCount/20&&pArea<line[i].pCount/20)
                    index+=dir;
                    continue;
                }
                float dC=(float)letter->correlation/line[index].correlation;
                float dP=((float)letter->pCount)/((float)line[index].pCount);
                float dPoint=letter->pCount-line[index].pCount;
                
                DR("//@@@*** Test_I letter A "<<letter->name<<" i="<<i<<" cI="<<letter->correlation<<" pCount="<<letter->pCount<<
                   " //  letter B "<<line[index].name<<" index="<<index
                   <<" cJ="<<line[index].correlation<<" pCount="<<line[index].pCount
                   <<"  //  dC="<<dC<<" dP="<<dP<<" dPoint/cN="<<dPoint/letter->pCount<<" pArea="<<pArea<<endl)
                
                if((dC>0.93&&dP>1.05)){  //неустойчивая проверка   dP>1.05  не менять
                    DR("remove J"<<endl);
                    line[index].correlation=0;
                    index+=dir;
                    continue;
                }
                if((dC>1&&dP>1)){  //неустойчивая проверка   dP>1.05  не менять
                    DR("remove J"<<endl);
                    line[index].correlation=0;
                    index+=dir;
                    continue;
                }
                if((dC>1.2&&dP>0.9)){
                    DR("remove J"<<endl);
                    line[index].correlation=0;
                    index+=dir;
                    continue;
                }
                if((dC>0.91&&dP>1.2)){
                    DR("remove J 1"<<endl);
                    line[index].correlation=0;
                    index+=dir;
                    continue;
                }
                if(dC<1.05&&dP<0.93){
                    DR("remove I _0"<<endl);
                    letter->correlation=0; dir=-1; break;
                }
                if(dC<0.95&&dP<1.05){
                    DR("remove I _1"<<endl);
                    letter->correlation=0; dir=-1; break;
                }
                if(dC<0.96&&dP<1){
                    DR("remove I _2"<<endl);
                    letter->correlation=0; dir=-1; break;
                }
                if(dC>0.98&&dP>0.98&&(letter->name.find(xKey)!=-1&&line[index].name.find(xKey)==-1)){
                    DR("remove J by X"<<endl);
                    line[index].correlation=0;
                    index+=dir;
                    continue;
                }
                if((letter->OCRIndex=='Z'||letter->OCRIndex=='N')&&(line[index].OCRIndex=='Z'||line[index].OCRIndex=='N')){
                    if(dC>1&&dP>0.95){
                        DR("remove J by Z"<<endl);
                        line[index].correlation=0;
                        index+=dir;
                        continue;
                    }else{
                        DR("remove I by Z"<<endl);
                        letter->correlation=0; dir=-1; break;
                    }
                }
                
                index+=dir;
            }
            count++;
        }
        //exit(0);
        //@@@@
        
        
    }
    
      
}
