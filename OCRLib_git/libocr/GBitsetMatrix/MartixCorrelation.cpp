#include "GBitsetMatrix.h"

/*

// Вычисление коэффициента корреляции по маске единичных битов.  Основная
// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrixCorrelation(BitMask32 *mask32,
										int xSrc0,
										int ySrc0,
										int xSrc1,
										int ySrc1,
										int dlt,
										ofstream &c_out){

  int correlation=0;
  // img - исходное изображение для примениения маски
  // mask32  - маска которая применяется по всей площади img

  //DP("xSrc="<<xSrc<<" ySrc"<<ySrc<<" dlt="<<dlt<<" border="<<border<<END);
   drawDataRGB[0][ySrc0+border][xSrc0+border]=0;

  int y,x,n,m,p;
  int sx0,sy0,sx1,sy1;
  int Sb,SumBitOn,SumBitOff,SumBtMax,deltaSum;
  int nc=ncolumns, nr=nrows;  // SH(ncolumns);  SH(nrows);
  int mH=32, mW=32;        // mH(8-96) высота и mW (32) ширина (габарит) битовой
						   // маски перефирийной области  "Off"
  //int ncVnr=mH*mW;
  //int sf=szf*8;     //  sf=32,  для int=32


DP(" TIME_START setMatrixCorrelation ON TEST ");
///TIME_START

		 //printMask(mask32,c_out);         DM(END);DM(END);

		 // Задание области скольжение маски по смещению dlt или по координатам x,y
		 if(dlt>0){
			 sx0=xSrc0 + border + 32,  sx1=xSrc0 + dlt + border + 32;
			 sy0=ySrc0 + border,       sy1=ySrc0 + dlt + border;
		 }else{  //  dlt=-dlt
			 sx0=xSrc0 + border + 32,  sx1=xSrc1 + border + 32;
			 sy0=ySrc0 + border,       sy1=ySrc1 + border;
//			 mH=sy1 - sy0;
		 }

		 // Скольжение маски по всей nc*nr площади скана.
		 SumBtMax=0;
		 for ( x=sx0; x < sx1; x++ ) {
		 p=x*nr;
		 for ( y=sy0; y < sy1; y++ ) {
			   SumBitOn=0; SumBitOff=0;
			   // Суммы маски mask32 по  Y
			   for ( m=0; m < mH; m++ ) {
					// Умножение маски на транспонированный скан SInt
					Sb=sInt[y+p+m] & mask32[0].On[m];
					// Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
					Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
					Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
					Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
					Sb+=Sb >> 8;                               // 16-битовые
					Sb+=Sb >> 16;                              // 32-битовая ячейка
					SumBitOn+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"

					// Умножение маски на транспонированный скан SInt
					Sb=sInt[y+p+m] & mask32[0].Off[m];
					// Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
					Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
					Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
					Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
					Sb+=Sb >> 8;                               // 16-битовые
					Sb+=Sb >> 16;                              // 32-битовая ячейка
					SumBitOff+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
			   } // m
			   // максимальная сумма по всей площади скана

			   deltaSum=SumBitOn-SumBitOff;
			   if(deltaSum<0){deltaSum=0;}
			   if( deltaSum >= SumBtMax ) { SumBtMax=deltaSum; }
			   //DP("SumBitOn="<<SumBitOn<<" SumBitOff="<<SumBitOff<<" SumBtMax"<<SumBtMax<<
			   //" sx0="<<sx0<<" sx1"<<sx1<<" sy0="<<sy0<<" sy1"<<sy1<<END);
#ifdef DEBUGLVL
//if( GrafT[2][1] ) {
   if(drawDataRGB[0][y][x-32])drawDataRGB[0][y][x-32]=drawDataRGB[0][y][x-32] - SumBitOn*0.7-20; // (SumBit/4)
//	}                                      //  drawDataRGB[0][y][x]*=0xff00ff;
#endif
			 } // y
		  } // x




//DP(" SumBtMax="<<SumBtMax<<" mask32[0].NMask"<<mask32[0].NMask<<END);
//SH(SumBtMax); SH(SumBtMaxOff); SH(" ");




			  // Индикаторы цифровые //
//if( GrafT[2][1] ) {
//			  DM(END);
//			  DM(END);  // TABL
//			  for ( x=0; x < ncVnr; x++ ) {
//				 if(bits_data[x]==1){DM("1");}else{DM(".")}
//				 if ((x+1)%mW==0)DM(END);
//			  }
//			  DM(END);
// }


///TIME_PRINT
DM(END); DM(mH<<" mH "); DM(END);
DM(" TIME_PRINT setMatrixCorrelation TEST"); DM(END);





if(SumBtMax<1||mask32[0].NMask<1)return 0;


correlation=SumBtMax*100/mask32[0].NMask;

//DP("//__correlation="<<correlation<<END);
return correlation;
// визуализация // 255 << 24; черный // 255 << 16; G  // 255 << 8; R  // 255; B //
}//_____________________________________________________________________________
*/



// Вычисление коэффициента корреляции по маске единичных битов  IEST
// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrixCorrelation(GBitMask32 *mask32,
										int xSrc0,
										int ySrc0,
										int xSrc1,
										int ySrc1,
										int dlt,
										int *maxX,
										int *maxY,
										ofstream &c_out){

  int correlation=0;
  // img - исходное изображение для примениения маски
  // mask32  - маска которая применяется по всей площади img

//DP("xSrc0="<<xSrc0<<" ySrc0"<<ySrc0<<" dlt="<<dlt<<" border="<<border<<END);
  if(ySrc0+border>nrows||xSrc0+border>ncolumns||xSrc0<0||ySrc0<0)return 0;
   //drawDataRGB[0][ySrc0+border][xSrc0+border]=0;

  int y,x,m,p;
  int sx0,sy0,sx1,sy1;
  int Sb,Si,SumBitOn,SumBitOff,SumBtMax,deltaSum;
  int nr=nrows;  // SH(ncolumns);  SH(nrows);
	int mH=mask32[0].mH;//mW=32;        // mH(8-96) высота и mW (32) ширина (габарит) битовой
						   // маски перефирийной области  "Off"
  //int ncVnr=mH*mW;
  //int sf=szf*8;     //  sf=32,  для int=32


//DP(" TIME_START setMatrixCorrelation ON TEST mask32[0].mH="<<mask32[0].mH<<END);
///TIME_START

		 //mask32->printMask(c_out);         DM(END);DM(END);

		 // Задание области скольжение маски по смещению dlt или по координатам x,y
		 if(dlt>0){
			 sx0=xSrc0 + border + 32,  sx1=xSrc0 + dlt + border + 32;
			 sy0=ySrc0 + border,       sy1=ySrc0 + dlt + border;
		 }else{  //  dlt=-dlt
			 sx0=xSrc0 + border + 32,  sx1=xSrc1 + border + 32;
			 sy0=ySrc0 + border,       sy1=ySrc1 + border;
//			 mH=sy1 - sy0;
		 }

  int wVh=(sx1 - sx0)*(sy1 - sy0);
  int d;
  unsigned short *BolS;

  // запрос памяти
	BolS=(unsigned short*)malloc((wVh+64)*2);
  // выход в случае отсутсттвия свободной памяти запрашиваемого размера
  if (BolS==NULL) {DC("BolS"); exit (1);}



		 // Скольжение маски по всей nc*nr площади скана.
		 SumBtMax=d=0;
		 for ( x=sx0; x < sx1; x++ ) {
		 p=x*nr;
		 for ( y=sy0; y < sy1; y++ ) {
			   SumBitOn=SumBitOff=0;
			   // Суммы маски mask32 по  Y
			   for ( m=0; m < mH; m++ ) {
				 // Умножение маски на транспонированный скан SInt
				 Si=sInt[y+p+m];
				 if(Si != 0) {
				   Sb=Si & mask32[0].On[m];
				   if(Sb != 0) {
					// Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
					Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
					Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
					Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
					Sb+=Sb >> 8;                               // 16-битовые
					Sb+=Sb >> 16;                              // 32-битовая ячейка
					SumBitOn+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
				   }
					// Умножение маски на транспонированный скан SInt
				   Sb=Si & mask32[0].Off[m];
				   if(Sb != 0) {
					// Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
					Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
					Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
					Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
					Sb+=Sb >> 8;                               // 16-битовые
					Sb+=Sb >> 16;                              // 32-битовая ячейка
					SumBitOff+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
				   }
				 } // if 
			   } // m
			   // максимальная сумма по всей площади скана

			   deltaSum=SumBitOn-SumBitOff;
			   if(deltaSum<0){deltaSum=0;}
			   if( deltaSum >= SumBtMax ) { SumBtMax=deltaSum; *maxX=x; *maxY=y; }
			   //DP("SumBitOn="<<SumBitOn<<" SumBitOff="<<SumBitOff<<" SumBtMax"<<SumBtMax<<
			   //" sx0="<<sx0<<" sx1"<<sx1<<" sy0="<<sy0<<" sy1"<<sy1<<END);
			   BolS[d]=deltaSum;  d++; // DM(SumBitOn<<" ";)    SumBitOn
#ifdef DEBUGLVL_DRAW
if(drawDataRGB[0][y][x-32])drawDataRGB[0][y][x-32]=
drawDataRGB[0][y][x-32] - ((SumBitOn+4)<<24); // - SumBitOn*0.7-20;  // (SumBit/4)  //  drawDataRGB[0][y][x]*=0xff00ff;
#endif
#ifdef DEBUGLVL_GREY1
			 if(drawData[0][y][x-32])drawData[0][y][x-32]=0;
				 //drawDataRGB[0][y][x-32] - ((SumBitOn+4)<<24); // - SumBitOn*0.7-20;  // (SumBit/4)  //  drawDataRGB[0][y][x]*=0xff00ff;
#endif
			 
		 
		 } // y
		 } // x

// число белых пикселей на вершине +4  //  454==SumBtMax;

//if( GrafT[2][1] ) {
/*		d=0;
		 for ( x=sx0; x < sx1; x++ ) {
			 p=x*nr;
			 for ( y=sy0; y < sy1; y++ ) {
			  if(drawDataRGB[0][y][x-32])drawDataRGB[0][y][x-32]=
			  drawDataRGB[0][y][x-32] - ((BolS[d]+4)<<8)/(mask32[0].NMask+1); // - s*0.8-20  (SumBit/4)  SumBtMax
			  d++;
				 } // y
		 } // x
//	}
*/

DP(" SumBtMax="<<SumBtMax<<" mask32[0].NMask"<<mask32[0].NMask<<END);
//SH(SumBtMax); SH(SumBtMaxOff); SH(" ");

			  // Индикаторы цифровые //
//if( GrafT[2][1] ) {
//			  DM(END);
//			  DM(END);  // TABL
//			  for ( x=0; x < ncVnr; x++ ) {
//				 if(bits_data[x]==1){DM("1");}else{DM(".")}
//				 if ((x+1)%mW==0)DM(END);
//			  }
//			  DM(END);
// }


///TIME_PRINT
//DM(END); DM(mH<<" mH "); DM(wVh<<" wVh "); DM(SumBtMax<<" SumBtMax "); DM(END);
//DM(" TIME_PRINT setMatrixCorrelation TEST"); DM(END);



  //  освобождение памяти
	if ( BolS !=NULL ) free(BolS);


if(SumBtMax<1||mask32[0].NMask<1)return 0;


			 maxX=maxX - border - 32;

			 maxY=maxY - border;

correlation=SumBtMax*100/mask32[0].NMask;

//DP("//__correlation="<<correlation<<END);
return correlation;
// визуализация // 255 << 24; черный // 255 << 16; G  // 255 << 8; R  // 255; B //
}//_____________________________________________________________________________

/**/
