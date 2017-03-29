#include "GBitsetMatrix.h"

static bool sortMatchC(const OCRMatch& d1, const OCRMatch& d2){
    return d1.correlation > d2.correlation;
}

void GBitsetMatrix::letterCorrelation(vector<OCRMatch>&matchLine,
									  GFont *aliKali,
                                      int lineIndex,
									  int y0,
									  int y1,
									  int scaleMode,
                                      int OCRMode,
									  int print){

  //aliKali - указатель на массив букв (база букв).
  //aliKali[0].alLetterCount - количество букв в базе
  //BitMask32 *mask=&aliKali[0][1].mask32[0];  -- указатель на первую маску во второй букве базы
  //aliKali[0][1].mask32Count - количество масок во второй букве базы
  //stringArray - массив строк как в CBitset
  //stringNum - номер строки в массиве

  // resize(ncnr) векторных массивов сделан в Bitset_base.h
#define  c_out_ cout
#define  c_out1_ inputData.c_out
    


#ifdef REPORT
DR("inputData.name"<<inputData.data["name"]<<" border="<<border<<END)
int mCMax=0; string name;
#endif

    //DR_("y0="<<y0<<" y1="<<y1<<" h="<<nrows<<endl);
//print=1;

  int mC,mC0,dX,dY,mY,mX;
  int maxX,maxY,maxSumX,maxSumY,count;
  int letterY0, maskY0, maskY1,limitY;    //, xCenter;
  GLetter *glyph;
  avarageLetterH=0;
  vector<OCRMatch>scaleVector;
  int maxLetterCount=60000;
    
/*  // константы для алгоритма подсчета единиц в 128 или в 64р словах
static const unsigned int constF[12]={              // 12  16
	0x33333333, 0x33333333, 0x33333333, 0x33333333,
	0x55555555, 0x55555555, 0x55555555, 0x55555555,
	0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f  // ,
//	0x00000000, 0x00000000, 0x00000000, 0x00000000  // TEST
};
*/
    
// переход в режим Assembler, загрузка констант
#if defined ASM_OS64_SSE3 || defined ASM_OS32_SSE2

__asm{     //  movups (без требования выравнивания)
	/// Для выполнения в регистрах MMX
#ifdef MMX	
	// загрузка констант в регистр 64p ММХ ( считаем в байтах - int*4)
	movq            mm5, qword ptr constF+16;   // 55 Пересылка данных (64 бит)
	movq            mm6, qword ptr constF;      // 33 из/в регистр ММХ  MOVDQU-128
	movq            mm7, qword ptr constF+32;   // 0f
#endif
	// загрузка констант в регистр 128p SSE
	//  movups без требования выравнивания  ( MOVAPS должны быть выровнены )
#ifdef WIN32
	//Vic movups          xmm5, dqword ptr constF+16;  // 55  Пересылка данных (128 бит)
	//Vic movups          xmm6, dqword ptr constF;     // 33  + MOVDQU
	//Vic movups          xmm7, dqword ptr constF+32;  // 0f
//	movups          xmm2, dqword ptr constF+48;  // TEST
#else
        movups          xmm5, [constF+16];  // 55  Пересылка данных (128 бит)
        movups          xmm6, [constF+0];     // 33  + MOVDQU
        movups          xmm7, [constF+32];  // 0f
	//movups          xmm2, dqword ptr constF+48;  // TEST
#endif

}; // __asm

#endif   
	
//Инструкция movq выполняется за шесть тактов, тогда как для pshufw требуется только два
/**/
    
//TIME_START
 
    
int deltaSearch;
int correlationLimit=70;

if(OCRMode==1)deltaSearch=8;
if(OCRMode==OCR_NORMALISATION)deltaSearch=8;
if(OCRMode==2)deltaSearch=8;
if(OCRMode==3)deltaSearch=8;
  

//unsigned long long  tm_start0, tm_end0; double time0=0;    
//unsigned long long  tm_start1, tm_end1; double time1=0;  
//unsigned long long  tm_start2, tm_end2; double time2=0; 
//pTicks tcs;
//int tCount=0;
    
    //установка лимита поиска по фокальным точкам
    limitY=(y1-y0);
    fPoint=&focalPoint[0];
    
    for(int pointIndex=0;pointIndex<focalPoint.size();pointIndex++){
        if(focalPoint[pointIndex].type==L_POINT||fPoint[pointIndex].y-border>y1+limitY*2||fPoint[pointIndex].y-border<y0-limitY){
           //drawPoint[0][fPoint[pointIndex].y][fPoint[pointIndex].x]=255;
           fPoint[pointIndex].status=0;
        }else{
           fPoint[pointIndex].status=1;
        }
            //cout<<" y="<<fPoint[pointIndex].y-border<<" y0="<<y0<<" y1="<<y1<<endl;
    }
 
    //TIME_START
    int contPoint=0;
    int contMask=0;
    
    ulong letterCount=0;
    ulong letterSetCount=aliKali->letterSet.size();

    if(!letterSetCount){
        letterCount=aliKali->letterCount();
    }else{
        letterCount=letterSetCount;
    }
    
    aliKali->letterSet.resize(0);
    letterSetCount=0;
    
	for(int index=1; index<letterCount;index++){
        //print=1;if(index!=22299 )continue;
    
        
        /*
        if(!letterSetCount){
            if(OCRMode!=OCR_NORMALISATION){
                glyph=aliKali->getOCRLetter(index);
                if(glyph==0)continue;
            }else{
                glyph=aliKali->getLetter(index);
                //if(index>22000)cout<<" index="<<index<<"  name"<<glyph->name<<endl;
                if(OCRMode==OCR_NORMALISATION&&glyph->name!="⊕"){glyph->destroy();continue;}
            }
            
            glyph[0].OCRIndex=glyph[0].OCRKey[0];
            //if(glyph->name!="ྱ"){glyph->destroy();continue;} print=1;
            //if(glyph->OCRKey[0]=='V'){glyph->destroy();continue;}
            //print=0; if(glyph->name=="ཧཱུ")print=1;
            
            if(glyph->mask32Count()==0){glyph->destroy();continue;}
            aliKali->letterSet.push_back(index);
        }else{
        
            letterIndex=aliKali->letterSet[index];
            glyph=aliKali->getLetter(letterIndex);
       }
        */
        
        if(OCRMode==2){
            glyph=aliKali->getLetter(index);
            if(!RE2::PartialMatch(glyph->name,"[\\p{Devanagari}\\d«»｛｝—⊕\\|\\(\\)\\.,\\!\\?IVX]+")||
               !glyph->mask32Count()){
                glyph->destroy();
                continue;
            }
        }else{
            glyph=aliKali->getOCRLetter(index);
            if(glyph==0)continue;
            if(glyph->mask32Count()==0){glyph->destroy();continue;}
        }
        
        
    //    print=0;
        //if(glyph->name!="ྲ"){glyph->destroy();continue;}
         //cout<<" index="<<index<<"  name"<<glyph->name<<" id="<<glyph->letterIndex<<endl;

         //if(scaleMode&&glyph->OCRIndex!=5){glyph->destroy();continue;}
		 
		 #ifdef REPORT
		 //if(inputData.data["name"]==glyph[0].name){print=1;}else{print=0;}
		 //cout<<"/"<<glyph[0].name<<"/check glyph="<<glyph[0].name<<" in="<<in<<" inputData.x1="<<inputData.x1<<" inputData.x0="<<inputData.x0<<endl;
		 //cout<<(short)glyph[0].name[0]<<endl;
		 //if((short)glyph[0].name[0]==58)print=1;
		 #endif

         //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start0=tcs.tx;

         
         int OCRFlag=0;
         

		 letterY0=-glyph[0].letterH/2;
         DR("glyph["<<index<<"].OCRKey="<<glyph[0].OCRKey<<" type="<<(int)glyph[0].fPoint[0].type<<endl)
        int letterMatrixMode=1;
        if(!glyph[0].fPointCount()||glyph[0].fPoint[0].type==L_POINT)letterMatrixMode=0;
        if(glyph[0].letterH*glyph[0].letterW<512)letterMatrixMode=0;
        
         if(letterMatrixMode){  //glyph[0].OCRKey[0]!='X'&&glyph[0].OCRKey[0]!='Z' //||OCRMode==2
             contPoint++;
             //если в букве есть фокальные точки то поиск ведем перебором фокальных точек на странице.
             //Положение первого признака отсчитываем от положения фокальной точки.
             int xPointLetter=glyph[0].fPoint[0].x;
             int yPointLetter=glyph[0].fPoint[0].y;
             
             for(int pointIndex=0;pointIndex<focalPoint.size();pointIndex++){

                 if(!fPoint[pointIndex].status)continue;
                 
                 int xCenter=fPoint[pointIndex].x-xPointLetter;
                 int yCenter=fPoint[pointIndex].y-yPointLetter;
                 
                 //drawData[0][yCenter][xCenter]=0;
                 ///cout<<"i="<<pointIndex<<" x="<<fPoint[pointIndex].x<<" y="<<fPoint[pointIndex].y<<endl;
                 
                 int pointsCorrelation=0;
                 //проверяем остальные точки.
                 if(glyph[0].fPointCount()>1){
                     for(int pIndexLetter=1;pIndexLetter<glyph[0].fPointCount();pIndexLetter++){
                         int x_=xCenter+glyph[0].fPoint[pIndexLetter].x;
                         int y_=yCenter+glyph[0].fPoint[pIndexLetter].y;
                         if(drawPoint[y_][x_]<255){
                             //drawPoint[0][y_][x_]=0;
                             //cout<<" Get it!";
                             pointsCorrelation++;
                             
                             byteMask m; //здесь нужно проверить тип точки
                             m.setMask(drawPoint[y_][x_]);
                             //cout<<"_j="<<pointIndex<<" x="<<x_<<" y="<<y_<<" t="<<(int)glyph[0].fPoint[pIndexLetter].type<<
                             //" b1="<<m.b1<<" b2="<<m.b2<<" b3="<<m.b3<<" b4="<<m.b4<<" b5="<<m.b5<<" b6="<<m.b6<<" b7="<<m.b7<<" b8="<<m.b8<<" p="<<(int)drawPoint[0][y_][x_]<<endl;
                             if(glyph[0].fPoint[pIndexLetter].type==T_POINT&&m.b1)pointsCorrelation+=1;
                             if(glyph[0].fPoint[pIndexLetter].type==X_POINT&&m.b2)pointsCorrelation+=1;
                             //if(glyph[0].fPoint[pIndexLetter].type==L_POINT&&mask.b3)pointsCorrelation+=1;
                             if(glyph[0].fPoint[pIndexLetter].type==P_POINT&&m.b4)pointsCorrelation+=1;
                         }
                     }
                 }else{pointsCorrelation=1;}
                     
                 if(pointsCorrelation){
                     //cout<<" pC="<<pointsCorrelation<<endl;
                     //cout<<" "<<glyph->name;
                 //if(flag+glyph[0].fPointCount()/2>=glyph[0].fPointCount()){
                     //одна или больше точек в этом месте сработали. 
                     //Проверяем корреляцию буквы относительно найденного центра буквы.
                     dX=glyph[0].mask32[0].mWOn; if(dX<10)dX=10; //if(dX>14)dX=16;
                     dY=glyph[0].mask32[0].mHOn; if(dY<10)dY=10; //if(dY>14)dY=16;
                     //if(dY>dX)dY=dX;  //выбираем наименьший шаг сетки. Необходимо для поиска небольших признаков
                     //if(dX>dY)dX=dY;
                     
                     mC=0; maxSumX=0;maxSumY=0; count=0;
                     
                     for(int m=0;m<glyph[0].mask32Count();m++){
                         maxX=0; maxY=0;
                         
                         if(m==0){  
                             
                             //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start1=tcs.tx;  //### 
                             
                             mC0=setMatrix_Lion(&glyph[0].mask32[m],
                                                glyph[0].mask32[0].xMask+xCenter-dX/4+32,
                                                glyph[0].mask32[0].yMask+yCenter-dY/4,
                                                glyph[0].mask32[0].xMask+xCenter+dX/4+32,
                                                glyph[0].mask32[0].yMask+yCenter+dY/4,
                                                0,
                                                &maxX,
                                                &maxY,
                                                0);
                             
                             
                             mX=xCenter+(maxX-glyph[0].mask32[0].xMask-xCenter);
                             mY=yCenter+(maxY-glyph[0].mask32[0].yMask-yCenter);
                             if(mC0>-1)DR("@mC0="<<mC0<<" x="<<maxX-32<<" y="<<maxY<<endl)
                            
#ifdef DEBUGLVL_DRAW        
                           if(mC0>0){
                             for(int t=0;t<dY;t++){
                                 for(int d=0;d<dX;d++){
                                     //drawData[0][maxY+t][glyph[0].mask32[0].xMask+xCenter-dX/2+d]/=(mC0+1);
                                     //drawDataRGB->put(glyph[0].mask32[0].xMask+xCenter-dX/2+d,maxY+t,0xFF000000);
                                 }        
                             }
                           }
#endif                       
                             //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end1=tcs.tx;
                             //tm_end1-=tm_start1; time1+=(double)tm_end1/3000000000;  //###
                             //tCount++;
                             
                             if(mC0<75)goto nextLetterPoint;
                         }else{
                             //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start2=tcs.tx;  //### 

                             mC0=setMatrix_Lion(&glyph[0].mask32[m],
                                                glyph[0].mask32[m].xMask+mX,
                                                glyph[0].mask32[m].yMask+mY,
                                                0,0,
                                                deltaSearch,
                                                &maxX,
                                                &maxY,
                                                0);
                             //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end2=tcs.tx;
                             //tm_end2-=tm_start2; time2+=(double)tm_end1/3000000000;  //###

                             //DR("t2")
                         }
                         
                         //#ifdef REPORT
                         //if(print)glyph[0].mask32[m].printMask();
                         //DR("mask"<<m<<" mC0="<<mC0<<" x="<<x<<" y="<<y<<" glyph y="<<glyph[0].mask32[m].yMask)
                         //DR("__________________MatrixCorrelation="<<mC0<<" glyph[0].name="<<glyph[0].name.c_str())
                         //DR(" mask["<<m<<"].mHOn="<<glyph[0].mask32[m].mHOn)
                         DR("   maxX="<<maxX<<" maxY="<<maxY<<" mC0="<<mC0<<END)
                         //#endif
                         //при распознавании печатного текста выходим из проверки буквы
                         //после второй нераспознанной маски 
                         if(mC0<75){
                             OCRFlag++;
                             if(OCRFlag==2){
                                 OCRFlag=0;
                                 goto nextLetterPoint; 
                             }                               
                         }
                         //if(mC0<50&&OCRMode==2){
                         //    OCRFlag++;
                         //    if(OCRFlag==2){
                         //        OCRFlag=0;
                         //        goto nextLetterPosition;
                         //    }
                         //}
                         
                         mC+=mC0;
                         maxSumX+=maxX-glyph[0].mask32[m].xMask;         //подсчет среднего положения буквы
                         maxSumY+=maxY-glyph[0].mask32[m].yMask;
                         glyph[0].mask32[m].xMax=maxX-32;              //сохраняем позиции найденных признаков
                         glyph[0].mask32[m].yMax=maxY;
                         count++;
                         
                     }//for(int m=0;m<glyph[0].mask32Count
                     //DR("t3")
                     mC/=glyph[0].mask32Count();
                     if(count){
#ifdef REPORT
DR("/_ALL MatrixCorrelation/_________________________mC="<<mC<<" maxSumX="<<maxSumX/count-32-border<<" maxSumY="<<maxSumY/count-border<<" count="<<count<<" name="<<glyph[0].name<<endl)
#endif
                         maxSumX=maxSumX/count-32;
                         maxSumY=maxSumY/count;
                         //if(maxSumX<0)maxSumX=0;
                         //if(maxSumX>matchLine.size())maxSumX=matchLine.size()-1;
                         //maxSumX=glyph[0].xSum+maxSumX-border+16-glyph[0].letterW/2;
                         //maxSumX=maxSumX-glyph[0].xSum; 
                         
                         DR("maxSumX="<<maxSumX<<" border="<<border<<" glyph[0].xSum="<<glyph[0].xSum)
                         
                         
                         
                     }
                     //SH(mC);
                     //ImageProcessor->WriteImageData(drawDataRGB,IMGNOFLIP);
#ifdef REPORT
                     if(mC>0)DR("ALL mC="<<mC<<" y="<<maxSumY<<" x="<<maxSumX<<" glyph[0].ySum="<<glyph[0].ySum<<END)
#endif
                     if(!maxSumX)continue;
                     //DR("t4 maxSumX="<<maxSumX)
                     if(mC>correlationLimit){
                         OCRMatch match;
                         DR("set in="<<(uint)glyph->letterIndex<<" i="<<matchLine.size()<<" c0="<<mC<<" maxSumX="<<maxSumX-border<<"name="<<glyph[0].name<<endl)
                         match.correlation=mC+pointsCorrelation;
                         match.status=0;
                         match.name=glyph[0].name;
                         match.OCRIndex=glyph[0].OCRKey[0];
                         match.letterIndex=(uint)glyph->letterIndex;
                         match.letterW=glyph->letterW;
                         match.letterH=glyph->letterH;
                         match.yCenter=maxSumY-border+glyph->dY;
                         match.xCenter=maxSumX-border+glyph->dX;
                         match.s.yLimit0=maxSumY+glyph->y0;
                         match.dX=glyph->dX; match.dY=glyph->dY;
                         match.lineH=glyph->y1-glyph->y0;
                         for(int n=0;n<glyph->mask32Count();n++)match.mask32Vector.push_back(glyph[0].mask32[n]);
                         match.lineIndex=lineIndex;
                         match.setSize(0);
                         matchLine.push_back(match);
                         if(matchLine.size()>maxLetterCount){
                             matchLine.resize(0);
                             cout<<" @@@@ memory error"<<endl;
                             return;
                         }  //memory protection protocol
                         
                     }//if(mC>50
                     //DR("t5")	
                     
                     
                 }
             nextLetterPoint:; 
             }
             
             //exit(0);
             
             
             
         }else{  
             //проверяем каждую возможную позицию первого признака. 
             contMask++;
             //Когда находим первый признак, ищем остальные признаки относительно гипотезы положения первого признака
             dX=glyph[0].mask32[0].mWOn*.75; if(dX<10)dX=10; //if(dX>10)dX=10;   //.75 необходимо для распознавания тонких штрихов
             dY=glyph[0].mask32[0].mHOn*.75; if(dY<10)dY=10; //if(dY>10)dY=10;
             //if(dY>dX)dY=dX;  //выбираем наименьший шаг сетки. Необходимо для поиска небольших признаков
             //if(dX>dY)dX=dY;
             maskY0=y0-letterY0+glyph[0].mask32[0].yMask+border-20; //лимит поиска строится от координат и высоты строки  y0 y1
             maskY1=y1-letterY0+glyph[0].mask32[0].yMask+border+40;  
             
             if(glyph[0].OCRIndex=='A'){ 
                 maskY0=y0-letterY0+glyph[0].mask32[0].yMask+border-15; //лимит поиска строится от координат и высоты строки  y0 y1
                 maskY1=nrows-border+letterY0+glyph[0].mask32[0].yMask;  
             }
             if(glyph[0].OCRIndex=='V'){ 
                 maskY0=y0-glyph[0].letterH*1.5+glyph[0].mask32[0].yMask+border-25; //лимит поиска строится от координат и высоты строки  y0 y1
                 if(maskY0<border)maskY0=border;
                 //maskY1=y0-glyph[0].letterH+glyph[0].mask32[0].yMask+border+25;
                 maskY1=nrows-border+letterY0+glyph[0].mask32[0].yMask;
             }
             if(glyph[0].OCRIndex=='Z'&&glyph[0].name!="|"){ 
                 maskY0=y0-letterY0+glyph[0].mask32[0].yMask+border-15; //лимит поиска строится от координат и высоты строки  y0 y1
                 maskY1=y1-letterY0+glyph[0].mask32[0].yMask+border+15;  
             }
             if(glyph[0].OCRIndex=='R'||glyph[0].OCRIndex=='W'){ 
                 maskY0=y1-letterY0+glyph[0].mask32[0].yMask+border-15; //лимит поиска строится от координат и высоты строки  y0 y1
                 maskY1=nrows-border+letterY0+glyph[0].mask32[0].yMask;  
             }
             if(glyph[0].OCRIndex=='X'){ 
                 maskY0=y0-letterY0+glyph[0].mask32[0].yMask+border-20; //лимит поиска строится от координат и высоты строки  y0 y1
                 maskY1=y1-letterY0+glyph[0].mask32[0].yMask+border+40;    
             }
             if(glyph[0].name=="|"){ 
                 maskY0=border; //лимит поиска строится от координат и высоты строки  y0 y1
                 maskY1=nrows-border+letterY0;    
             }

             
             DR("glyph[0].mask32[0].yMask="<<glyph[0].mask32[0].yMask<<"letterY0="<<letterY0<<" y0="<<y0+border<<" maskY0="<<maskY0<<" maskY1="<<maskY1<<" border="<<border<<" ncolumns="<<ncolumns<<" limitY="<<limitY<<" dX="<<dX<<" dY="<<dY<<" mW="<<glyph[0].mask32[0].mWOn<<END)
             int xSt=border+32,xEn=ncolumns-border+32; //выносим вычисления счетчиков за границы цикла
             
             for(int y=maskY0;y<=maskY1;y+=dY){  //DR("//@@@/_y="<<y<<endl)
                 for(int x=xSt;x<xEn;x+=dX){ //DR("//@@@/_x="<<x<<endl)
                
                    mC=0; maxSumX=0;maxSumY=0; count=0;
                    
                    for(int m=0;m<glyph[0].mask32Count();m++){
                            maxX=0; maxY=0;
                            
                            if(m==0){   
                               //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start1=tcs.tx;  //###    
                                
                               mC0=setMatrix_Lion(&glyph[0].mask32[m],
                                                   glyph[0].mask32[0].xMask+x,
                                                   y,
                                                   glyph[0].mask32[0].xMask+x+dX,
                                                   y+dY,
                                                   0,
                                                   &maxX,
                                                   &maxY,
                                                   0);


                                mX=x+(maxX-glyph[0].mask32[0].xMask-x);
                                mY=y+(maxY-glyph[0].mask32[0].yMask-y);
                                //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end1=tcs.tx;
                                //tm_end1-=tm_start1; time1+=(double)tm_end1/3000000000;  //###
                                //tCount++;

                                if(mC0>-1)DR("@mC0="<<mC0<<" x="<<maxX-32<<" y="<<maxY<<endl)
                                //if(mC0<correlationLimit)break;
                                //if(mC0<40&&OCRMode==2)break;
                                if(mC0<correlationLimit)goto nextLetterPosition;
                            }else{
                                //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start2=tcs.tx; //###
                                
                                 mC0=setMatrix_Lion(&glyph[0].mask32[m],
                                                   glyph[0].mask32[m].xMask+mX,
                                                   glyph[0].mask32[m].yMask+mY,
                                                   0,0,
                                                   deltaSearch,
                                                   &maxX,
                                                   &maxY,
                                                   0);
//DR("t2")
                                //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end2=tcs.tx;
                                //tm_end2-=tm_start2; time2+=(double)tm_end2/3000000000;  //###
                            }

                            //#ifdef REPORT
                            //if(print)glyph[0].mask32[m].printMask();
                            //DR("mask"<<m<<" mC0="<<mC0<<" x="<<x<<" y="<<y<<" glyph y="<<glyph[0].mask32[m].yMask)
                            //DR("__________________MatrixCorrelation="<<mC0<<" glyph[0].name="<<glyph[0].name.c_str())
                            //DR(" mask["<<m<<"].mHOn="<<glyph[0].mask32[m].mHOn)
                            DR("   maxX="<<maxX<<" maxY="<<maxY<<" mC0="<<mC0<<END)
                            //#endif
                        //при распознавании печатного текста выходим из проверки буквы
                        //после второй нераспознанной маски 
                        if(mC0<correlationLimit&&OCRMode==2){
                               OCRFlag++;
                               if(OCRFlag==2){
                                   OCRFlag=0;
                                   goto nextLetterPosition; 
                               }                               
                        }
                        //if(mC0<50&&OCRMode==2){
                        //    OCRFlag++;
                        //    if(OCRFlag==2){
                        //        OCRFlag=0;
                        //        goto nextLetterPosition;
                        //    }
                        //}
                        
                            mC+=mC0;
                            maxSumX+=maxX-glyph[0].mask32[m].xMask;    //подсчет среднего положения буквы
                            maxSumY+=maxY-glyph[0].mask32[m].yMask;
                            glyph[0].mask32[m].xMax=maxX-32;              //сохраняем позиции найденных признаков
                            glyph[0].mask32[m].yMax=maxY;
                            glyph[0].mask32[m].correlation=mC0;

                        count++;

                    }//for(int m=0;m<glyph[0].mask32Count
//DR("t3")
                         mC/=glyph[0].mask32Count();
                         if(count){
                            #ifdef REPORT
                            DR("/_ALL MatrixCorrelation/_______________________________mC1="<<mC<<" maxSumX="<<maxSumX/count-32-border
                               <<" maxSumY="<<maxSumY/count-border<<" count="<<count<<END);
                            #endif
                            maxSumX=maxSumX/count-32;
                            maxSumY=maxSumY/count;
                             //if(maxSumX<0)maxSumX=0;
                            //if(maxSumX>matchLine.size())maxSumX=matchLine.size()-1;
                            //maxSumX=glyph[0].xSum+maxSumX-border+16-glyph[0].letterW/2;
                            //maxSumX=maxSumX-glyph[0].xSum; 
                             
                             DR("maxSumX="<<maxSumX<<" border="<<border<<" glyph[0].xSum="<<glyph[0].xSum)
                             


                         }
                         //SH(mC);
                         //ImageProcessor->WriteImageData(drawDataRGB,IMGNOFLIP);
                         #ifdef REPORT
                         if(mC>0)DR("ALL mC="<<mC<<" y="<<maxSumY<<" x="<<maxSumX<<" glyph[0].ySum="<<glyph[0].ySum<<END)
                         #endif
                         if(!maxSumX)continue;
//DR("t4 maxSumX="<<maxSumX)
                            if(mC>correlationLimit){
                                     OCRMatch match;
                                     DR("set in="<<(uint)glyph->letterIndex<<" i="<<matchLine.size()<<" c0="<<mC<<"maxSumX="<<maxSumX-border<<"name="<<glyph[0].name<<endl)
                                     match.correlation=mC;
                                     match.status=0;
                                     match.name=glyph[0].name;
                                     match.OCRIndex=glyph[0].OCRKey[0];
                                     match.letterIndex=(uint)glyph->letterIndex;
                                     match.letterW=glyph->letterW;
                                     match.letterH=glyph->letterH;
                                     match.yCenter=maxSumY-border+glyph->dY;
                                     match.xCenter=maxSumX-border+glyph->dX;
                                     match.s.yLimit0=maxSumY+glyph->y0;
                                     match.dX=glyph->dX; match.dY=glyph->dY;
                                     match.lineH=glyph->y1-glyph->y0;
                                     match.lineIndex=lineIndex;
                                     for(int n=0;n<glyph->mask32Count();n++)match.mask32Vector.push_back(glyph[0].mask32[n]);
                                     match.setSize(0);
                                     matchLine.push_back(match);
                                     if(matchLine.size()>maxLetterCount){
                                         matchLine.resize(0);
                                         cout<<" @@@@ memory error"<<endl;
                                         return;
                                     }  //memory protection protocol
                                
                            }//if(mC>50
//DR("t5")	
            nextLetterPosition:;
                    
            }//for(int x=border+32 
DR("/t6/___"<<endl)
          }//for(int y=maskY0;
DR("/t7/___"<<endl)
     
       } 

         //asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end0=tcs.tx;
         //tm_end0-=tm_start0; time0+=(double)tm_end0/3000000000;
  
         glyph->destroy();

         //tm_end=clock(); tm_end-=tm_start; time=(float)tm_end/CLOCKS_PER_SEC;
         //if(time>maxTime){maxTime=time; maxTimeIndex=aliKali->letterSet[in];}
         //exit(0);
	 }//for(int in=0; in<aliKali->base.size();
    //cout<<"maxTime="<<maxTime<<" maxTimeIndex="<<maxTimeIndex<<endl;
    //cout<<"@@@@@time0/________________________time0="<<time0<<endl;
    //cout<<"@@@@@time1/________________________time1="<<time1<<endl;
    //cout<<"@@@@@time2/________________________time2="<<time2<<endl;
    //cout<<"@@@@@tCount/________________________tCount="<<tCount<<endl;
    //cout<<" ALL_OCR_LETTER"; TIME_PRINT_    
    //cout<<" contPoint="<<contPoint<<" contMask="<<contMask<<endl;
    
#ifdef MMX
	 __asm {emms;}
    // emms; команда обеспечивает переход процессора от исполнения MMX-команд
    // к исполнению обычных команд с плавающей запятой:
    // она устанавливает значение 1 во всех разрядах слова состояния.

#endif

    
//TIME_PRINT_

#undef c_out_


}//_____________________________________________________________________________

