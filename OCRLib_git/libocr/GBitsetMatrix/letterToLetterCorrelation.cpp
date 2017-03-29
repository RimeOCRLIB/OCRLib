#include "GBitsetMatrix.h"

int GBitsetMatrix::letterToLetterCorrelation(GLetter *glyph,
											  int y0,
											  int y1,
                                              int OCRMode,
											  int print){
  //letter - указатель тестовую букву
  //BitMask32 *mask=&aliKali[0][1].mask32[0];  -- указатель на первую маску во второй букве базы
  //aliKali[0][1].mask32Count() - количество масок во второй букве базы
	//ofstream out;
	

  //GBitMask32 *mask;//=&aliKali[0][2].mask32[0];  //-- указатель на первую маску во второй букве базы
  //DM("/1/____aliKali[0].Wylie"<<aliKali[0].Wylie.c_str());
  //printMask(mask,c_out);
  //TIME_START

  int mC,mC0,dX,dY,mY,mX;
  int maxX,maxY,maxSumX,maxSumY,count;
  int letterY0, maskY0, maskY1,limitY;
    int correlation=0;
	
/* // константы для алгоритма подсчета единиц в 128 или в 64р словах
static const unsigned int constF[12]={              // 12  16
	0x33333333, 0x33333333, 0x33333333, 0x33333333,
	0x55555555, 0x55555555, 0x55555555, 0x55555555,
	0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f  // ,
//	0x00000000, 0x00000000, 0x00000000, 0x00000000  // TEST
};
*/ 

#if defined ASM_OS64_SSE3 || defined ASM_OS32_SSE2  
// переход в режим Assembler, загрузка констант
__asm {     //  movups (без требования выравнивания)
	/// Для выполнения в регистрах MMX
#ifdef MMX
	// загрузка констант в регистр 64p ММХ ( считаем в байтах - int*4)
	movq            mm5, qword ptr constF+16;   // 55 Пересылка данных (64 бит)
	movq            mm6, qword ptr constF;      // 33 из/в регистр ММХ  MOVDQU-128
	movq            mm7, qword ptr constF+32;   // 0f
#endif
	// загрузка констант в регистр 128p SSE
	//  movups без требования выравнивания  ( MOVAPS должны быть выровнены )
#ifdef WIN_VS
	//Vic movups          xmm5, dqword ptr constF+16;  // 55  Пересылка данных (128 бит)
	//Vicmovups          xmm6, dqword ptr constF;     // 33  + MOVDQU
	//Vicmovups          xmm7, dqword ptr constF+32;  // 0f
	//	movups          xmm2, dqword ptr constF+48;  // TEST
	
#endif
	
#ifdef WIN32
	movups          xmm5, dqword ptr constF+16;  // 55  Пересылка данных (128 бит)
	movups          xmm6, dqword ptr constF;     // 33  + MOVDQU
	movups          xmm7, dqword ptr constF+32;  // 0f
//	movups          xmm2, dqword ptr constF+48;  // TEST
#endif
#ifdef MACOS	
	movups          xmm5, [constF+16];  // 55  Пересылка данных (128 бит)
	movups          xmm6, [constF+0];     // 33  + MOVDQU
	movups          xmm7, [constF+32];  // 0f
	//	movups          xmm2, dqword ptr constF+48;  // TEST
#endif

       }; // __asm
//Инструкция movq выполняется за шесть тактов, тогда как для pshufw требуется только два
#endif
   
//print=0;
//#define DR(x) if(print)cout<<x;
/**/

//clock_t  tm_start, tm_end ,tm_0=0, tm_1=0; double time; // tm_start=clock();
//static clock_t tm0=0, tm1=0;
		 letterY0=y0;
	     int maskX[20];
	     int maskY[20];    //temporary arrays for store letter features coordinates
		 int bestX[20];
		 int bestY[20];    
	     int bestMask[20];  //temporary arrays for store letter features correlation
         int emptyMaskCount=0;
         int delta=glyph[0].letterH/12;  if(delta<4)delta=4; if(delta>8)delta=8;   //delta  размер области поиска признака
         if(OCRMode)delta=16;
    
         DR(" glyph[0].mask32Count()="<<glyph[0].mask32Count()<<" OCRMode="<<OCRMode<<endl)
    
		 if(glyph[0].mask32Count()>0){
			 dX=glyph[0].mask32[0].mWOn; if(dX<5)dX=5;
			 dY=glyph[0].mask32[0].mHOn; if(dY<5)dY=5; //if(dY>12)dY=12;
			 limitY=(y1-y0)/2;
			 maskY0=y0+border;
			 maskY1=y1+border;
			 
			 DR("y0="<<y0<<" y1="<<y1<<" maskY0="<<maskY0<<" maskY1="<<maskY1<<" border="<<border<<END)
             //int flag=1;
             
			 for(int y=maskY0;y<=maskY1;y+=dY){
                 for(int x=border+32;x<ncolumns-border+32;x+=dX){ 

                                mC=0; maxSumX=-1;maxSumY=0; count=0; emptyMaskCount=0;
								for(int m=0;m<glyph[0].mask32Count();m++){
	 								
                                    if(!glyph[0].mask32[m].status){
                                        emptyMaskCount++;
                                        continue; //при тестовых проверках буквы пропускаем маски с нулевым статусом
                                    }    
                                    
									maxX=0; maxY=0;

									if(m==0){
									   //tm_start=clock();
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
                                        DR(" mC0="<<mC0<<" mX="<<mX<<" mY="<<mY<<endl)                                                                                
                                        if(mC0<80&&!OCRMode)break;  //если первая маска найдена не устойчиво то букву дальше на проверяем
                                        if(mC0<40&&OCRMode)break;
                                        //if(flag){glyph[0].maskId0[glyph[0].maskId0Count]=glyph[0].newLetter;glyph[0].maskId0Count++; flag=0;}

										//tm_end=clock(); tm_end-=tm_start; tm0+=tm_end;
									}else{
										//tm_start=clock();
										 mC0=setMatrix_Lion(&glyph[0].mask32[m],
														   glyph[0].mask32[m].xMask+mX,
														   glyph[0].mask32[m].yMask+mY,
														   0,0,
														   delta,
														   &maxX,
														   &maxY,
														   0);

									//tm_end=clock(); tm_end-=tm_start; tm1+=tm_end;
									}

									#ifdef REPORT
									//if(print)glyph[0].mask32[m].printMask();
									DR("mask"<<m<<" mC0="<<mC0<<" xMask="<<glyph[0].mask32[m].xMask+mX<<" yMask="<<glyph[0].mask32[m].yMask+mY)
									DR("__________________MatrixCorrelation="<<mC0<<" glyph[0].name="<<glyph[0].name.c_str())
#ifdef DEBUGLVL_GREY1
                                    if(print&&mC0>80){
                                        glyph[0].mask32[m].printMask();
                                        GBitmap *img32=GBitmap::createRegion(drawData,
                                                                             glyph[0].mask32[m].xMask+mX-glyph[0].mask32[m].imgW,
                                                                             glyph[0].mask32[m].yMask+mY,
                                                                             glyph[0].mask32[m].imgW,
                                                                             glyph[0].mask32[m].mH);
                                        img32->printToScreenBinary();
                                        img32->destroy();
                                    
                                    }
#endif
									DR(" mask["<<m<<"].mHOn="<<glyph[0].mask32[m].mHOn)
									DR("   maxX="<<maxX<<" =maxY="<<maxY<<END)
									#endif
									if(mC0<50&&!OCRMode)break;
									mC+=mC0;
                                    maxX-=32;
									maxSumX+=maxX; DR("   maxSumX="<<maxSumX<<END)
									maxSumY+=maxY; DR("   maxSumY="<<maxSumY<<END)
									DR(" xMask="<<maxX<<" yMask="<<maxY<<endl)
									maskX[count]=maxX-border;   //coordinate from left upper corner of source image
									maskY[count]=maxY-border;
									count++;
									//SH(glyph[0].Wylie.c_str());

							   }
                               //if(glyph[0].mask32Count()-emptyMaskCount<1)print=1;
                               if(glyph[0].mask32Count())mC/=glyph[0].mask32Count();
                     
                               DR("mC="<<mC<<" glyph[0].mask32Count()="<<glyph[0].mask32Count()<<" emptyMaskCount="<<emptyMaskCount<<endl) 
							   

								 //SH(mC);
								 //ImageProcessor->WriteImageDTta(drawDTtaRGB,IMGNOFLIP);
						 
							 if((correlation<mC&&mC>80)||(OCRMode&&correlation<mC&&mC>40)){
								 DR("@@___maxSumX1="<<maxSumX<<" count="<<count<<END)
								 correlation=mC;
								 maxSumX=maxSumX/count-border;
								 maxSumY=maxSumY/count-border;
								 DR("mC="<<mC<<" maxSumX="<<maxSumX<<" nrows="<<nrows<<endl)
								 DR("glyph[0].xSum="<<glyph[0].xSum<<" maxSumX="<<maxSumX<<endl)
								 DR("glyph[0].ySum="<<glyph[0].ySum<<" maxSumY="<<maxSumY<<endl)
                                 DR("//@@___correlation="<<correlation<<endl)
								 for(int a=0;a<glyph[0].mask32Count();a++){
									 bestX[a]=maskX[a]; bestY[a]=maskY[a];  //store best rezult;
									 bestMask[a]=glyph[0].mask32[a].correlation;
								 }
								 //glyph[0].mask128.xMask=32+glyph[0].xSum-maxSumX-2;
								 //glyph[0].mask128.yMask=glyph[0].ySum-maxSumY;
							 }
					}//for(int x=border+32;
			 
			 }
		 }
        for(int a=0;a<glyph[0].mask32Count();a++){  //сохраняем в букве наилучшие значения корреляции маски с данным изображением. Используется при создании базы букв например testMask()
			glyph[0].mask32[a].xMax=bestX[a]; //coordinate from left upper corner of source image
			glyph[0].mask32[a].yMax=bestY[a];
			glyph[0].mask32[a].correlation=bestMask[a];
			DR("@mask m="<<a<<" xMask="<<glyph[0].mask32[a].xMax<<" yMask="<<glyph[0].mask32[a].yMax<<" c="<<glyph[0].mask32[a].correlation<<endl)
	    }
	
#ifdef MMX
__asm {
  emms;
}
#endif    
    
		
	 //cout<<"tm0="<<(double)tm0/CLOCKS_PER_SEC<<" tm1="<<(double)tm1/CLOCKS_PER_SEC<<" tm0/tm1="<<(double)tm0/tm1<<endl;
	// emms; команда обеспечивает переход процессора от исполнения MMX-команд
	// к исполнению обычных команд с плавающей запятой:
	// она устанавливает значение 1 во всех разрядах слова состояния.

	//DM("2@@")tm_end=clock(); tm_end-=tm_start; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);
	//DP("Matrix_Lion_2@@")tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;DP("time="<<time<<END<<END);
/**/

//#ifdef WIN32
	 //ImageProcessor->WriteImageDTta(drawDTtaRGB,IMGNOFLIP);  //SH(1);
//#endif
  // TIME_PRINT
DR("//ALL@@___correlation="<<correlation<<endl)

	return correlation;

}//_____________________________________________________________________________

