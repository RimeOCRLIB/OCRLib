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

#ifndef _matrix_H
#define _matrix_H

# include "../config.h"
#include "GBitMask.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GBitmap/GBitmap.h"
#include "../GFontEditor/GLetter.h"
#include <string.h>
#include <fstream>


using namespace ocr;


namespace ocr {
	
	
	// Подпрограмма уменьшения маски On на величину dlt setOnOffMatrixT
	
	// -----------------------------------------------------------------------------
	void GBitMask32::MatrixDltOn(int dlt) {
		
		int y;
		int d=dlt;          // dlt толщина стенок перефирийной области  "Off"
		unsigned int Sb;
		//int x0,x1,y0,y1;    // габариты прямоугольной однопиксельной маски пересечений
		unsigned int s=0;
		unsigned int *BufOn=On;
		unsigned int BufOff[128];   memset(BufOff,0,128);    // обнуление
		/**/
        
		DC(" TIME_START MatrixT_ON/www/  d="<<d<<END);
		DC("/@@2@@@/маска пересечений"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
		
		//printMask(c_out );   // работает  ///////////////////////
		
		if ( d==0 ) return;
		
		// формирование двухсторонней переходной области между "On" и "Off"
		if ( d <1 ) { d=1; }    if ( d >2 ) { d=2; }
		
		// формирование левой и правой части переходной области между "On" и "Off"
		for ( y=0; y < mH; y++ ) {
			s=BufOn[y];
			BufOff[y]=((s >> d) ^ (s << d)); // для крупных букв (шмриной больше 3 пикселов)
			//BufOff[y]=((s >> d) | (s << d)); // для мелких букв (шмриной 1-2 пиксел)
			
		}
		
		// формирование верхней и нижней части переходной области между "On" и "Off"
		
		//			 for ( y=d;  y < mH-d;  y++ ) { BufOff[y]=BufOff[y] | (BufOn[y+d] ^ BufOn[y-d]); }
		
		// формирование верхней части переходной области между "On" и "Off"
		for ( y=0;  y < mH-d;  y++ ) {
			BufOff[y]=BufOff[y] | (BufOn[y+d] ^ BufOn[y]);
		}
		
		// формирование нижней части переходной области между "On" и "Off"
		for ( y=mH; y >= d; y-- ) {
			BufOff[y]=BufOff[y] | (BufOn[y-d] ^ BufOn[y]);
		}
		/**/
		
		DC("/WWW/");
		//printMask(BufOff);
        //printMask();
		
		// формирование центральной  области  "On"  без переходной области
		// формирование перефирийной области  "Off" без переходной области
        // Необходимо установить тип маски maskType. Для maskType=0 не выполняется эррозия
		
		for (int y=0; y < mH; y++ ) {
			s=~BufOff[y];
			Sb=On[y];
			///Off[y]=Off[y] & s;
			if (maskType==1) {On[y]=Sb=Sb & s; }
			// подсчет площади центральной маски "On"
			// Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
			Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
			Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
			Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
			Sb+=Sb >> 8;                               // 16-битовые
			Sb+=Sb >> 16;                              // 32-битовая ячейка
			NMask+=Sb & 0x3F;  // 31 Обнуляем старшие разряды, содержащие "мусор"
		}
		//printMask();
		
		//DC(END<<" x0st "<<x0st<<" x0st "<<x1st<<" x1st "<<y0st<<" y0st "<<y1st<<" y1st "<<END);
		/**/
		/**/
		
		
		DC(" TIME_PRINT MatrixT  "); DC(END);
		
		//							 X0      X1      Y0       Y1
		//							 11		 00		 10		  O1
		//							 00		 11		 10		  O1
		
		/**/
	}//_____________________________________________________________________________
	
	
	
	// Подпрограмма уменьшения маски Off на величину dlt для setOnOffMatrixT
	
	// -----------------------------------------------------------------------------
	void GBitMask32::MatrixDltOff(int dlt) {
		
		int y;
		int d=dlt;          // dlt толщина стенок перефирийной области  "Off"
		unsigned int s=0;
		unsigned int *BufOn=On;
		unsigned int BufOff[128];   memset(BufOff,0,128);    // обнуление
		/**/
		
		DC(" TIME_START MatrixT_ON/www/  d="<<d<<END);
		DC("/@@2@@@/маска пересечений"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
		
		//printMask(c_out );   // работает  ///////////////////////
		
		// формирование двухсторонней переходной области между "On" и "Off"
		if ( d <1 ) { d=1; }    if ( d >3 ) { d=3; }
		
		// формирование левой и правой части переходной области между "On" и "Off"
		for ( y=0; y < mH; y++ ) {
			s=BufOn[y];
			BufOff[y]=((s >> d) ^ (s << d)); // для крупных букв (шмриной больше 3 пикселов)
			//BufOff[y]=((s >> d) | (s << d)); // для мелких букв (шмриной 1-2 пиксел)
			
		}
		
		// формирование верхней и нижней части переходной области между "On" и "Off"
		
		//			 for ( y=d;  y < mH-d;  y++ ) { BufOff[y]=BufOff[y] | (BufOn[y+d] ^ BufOn[y-d]); }
		
		// формирование верхней части переходной области между "On" и "Off"
		for ( y=0;  y < mH-d;  y++ ) {
			BufOff[y]=BufOff[y] | (BufOn[y+d] ^ BufOn[y]);
		}
		
		// формирование нижней части переходной области между "On" и "Off"
		for ( y=mH; y >= d; y-- ) {
			BufOff[y]=BufOff[y] | (BufOn[y-d] ^ BufOn[y]);
		}
		/**/
		
		DC("/WWW/");
		// printMask(BufOff, c_out);
		
		// формирование центральной  области  "On"  без переходной области
		// формирование перефирийной области  "Off" без переходной области
		
		for (int y=0; y < mH; y++ ) {
			s=~BufOff[y];
			Off[y]=Off[y] & s;
			////				 if (maskType==1) {On[y]=Sb=Sb & s; }
		}
		
		
		//DC(END<<" x0st "<<x0st<<" x0st "<<x1st<<" x1st "<<y0st<<" y0st "<<y1st<<" y1st "<<END);
		/**/
		/**/
		
		
		DC(" TIME_PRINT MatrixT  "); DC(END);
		
		//							 X0      X1      Y0       Y1
		//							 11		 00		 10		  O1
		//							 00		 11		 10		  O1
		
		/**/
	}//_____________________________________________________________________________
	
	
	
	
	// Подпрограмма создания масок On и Off для setOnOffMatrixT
	// (обтекания перефирийной области  "Off" с переходной областью)
	// -----------------------------------------------------------------------------
	void GBitMask32::MatrixOff(int dlt) {
		
		// wr(8-96)=32  высота и wc (32) ширина (габарит) битовой
		// маски перефирийной области  "Off"
		// "ON" в маске  (центральная область)
		
		int y,n;
		int d=dlt;          // dlt толщина стенок перефирийной области  "Off"
		//int x0,x1,y0,y1;    // габариты прямоугольной однопиксельной маски пересечений
		unsigned int s=0;
		unsigned int *BufOn=On;
		unsigned int *BufOff=Off;
   		unsigned int BolS[128];  ///< вспомогательный массив  matriT,h
        
		/**/
		
		DC(" TIME_START MatrixT  d="<<d<<END);
		DC("/@@2@@@/маска пересечений"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
		
		//------------------------------------------------------------------------------
		
		DC(" d="<<d<<END);
		// BolS вспомогательный массив   GBitMask.h   // memset(BolS,0,128);
		
		// копирование
		//for ( y=0; y < mH; y++ ) { BolS[y]=BufOn[y]; }
		memcpy (BolS, BufOn, 128*4);        //  size*szf
		
		int m;
		for ( m=0; m < d; m++ ) {
			
			// формирование левой и правой части перефирийной области  "Off"
			for ( y=0; y < mH; y++ ) {
				s=BolS[y];
				BufOff[y]=~BufOn[y] & ((s >> 1) | (s << 1));
				//BufOff[y]=((s >> 1) | (s << 1));
			}
			/*
			 // формирование верхней и нижней части перефирийной области  "Off"
			 //for ( y=m+1;  y < mH-m-1;  y++ ) {
			 for ( y=1;  y < mH-1;  y++ ) { BufOff[y]=BufOff[y] | BolS[y+1] ^ BolS[y-1]; }
			 */
			// формирование верхней части перефирийной области  "Off"
			for ( y=1;  y < mH;  y++ ) { BufOff[y-1]=BufOff[y-1] | BolS[y];} //  BufOff[y];
			
			// формирование нижней части перефирийной области  "Off"
			for ( y=mH-1; y > 0; y-- ) { BufOff[y+1]=BufOff[y+1] | BolS[y];} // BufOff[y];
			
			// удаление центральной области  "On"
			for ( y=0; y < mH; y++ ) {
				BufOff[y]=~BufOn[y] & BufOff[y];
				BolS[y]=BufOff[y] | BolS[y];
			}
			
		} // m
		//printMask(c_out );   // работает  ///////////////////////
		/**/
		//if ( y > mH ) { y=mH; }   if ( y < 1 ) { y=1; }
		//------------------------------------------------------------------------------
		
		/*		 // формирование левой и правой части перефирийной области  "Off"
		 s=0;
		 for ( y=0; y < mH; y++ ) {
		 s=BufOn[y];
		 BufOff[y]=~s & ((s >> d) | (s << d));
		 }
		 
		 //			 for ( y=d;  y < mH-d;  y++ ) { BufOff[y]=BufOff[y] | BufOn[y+d] ^ BufOn[y-d]; }
		 
		 // формирование верхней части перефирийной области  "Off"
		 for ( y=d;  y < mH;  y++ ) { BufOff[y-d]=BufOff[y-d] | BufOn[y];} //  BufOff[y];
		 
		 // формирование нижней части перефирийной области  "Off"
		 for ( y=mH-d; y > 0; y-- ) { BufOff[y+d]=BufOff[y+d] | BufOn[y];} // BufOff[y];
		 
		 // удаление центральной области  "On"
		 for ( y=0; y < mH; y++ ) { BufOff[y]=~BufOn[y] & BufOff[y]; }
		 
		 printMask(c_out );   // работает  ///////////////////////
		 */
		
		// формирование прямоугольной маски пересечений
		// маскирование обтекающей перефирийной области  "Off"
		// габариты прямоугольной однопиксельной маски пересечений
		//int x0=mask[0].x0,  x1=mask[0].x1,  y0=mask[0].y0,   y1=mask[0].y1;
		
		y0=0;   y1=mH;      x0=0;   x1=imgW;
		/*
		 // маскирование верхней части перефирийной области  "Off"
		 if(x0st==1) {for ( y=0;  y < y0-1;  y++) { BufOff[y]=0; }}
		 
		 // маскирование нижней части перефирийной области  "Off"
		 if(x1st==1) {for ( y=mH; y > y1+1;  y--) { BufOff[y]=0; }}
		 
		 // маскирование левой части перефирийной области  "Off"
		 if( y0st==1 ) { s=0;   // 10000000000000000000000000000000
		 for ( n=0; n <= x0; n++ ) { s=(s >> 1) | 0x80000000; }  s=~s;
		 for ( y=0; y < mH;  y++ ) { BufOff[y]=BufOff[y] & s; }
		 binaryPrint(s, 2, c_out );  DC(END); DC(s<<"++s++"); DC(END);
		 } // y0st
		 */
		// маскирование правой части перефирийной области  "Off"
		//			 if( y1st==1 ) {  s=0;  // 00000000000000000000000000000001
		s=0;
		for ( n=mW; n >=x1; n-- ) {s=(s << 1) | 1; }
		s=~s;
		for ( y=0;  y < mH; y++ ) { BufOff[y]=BufOff[y] & s; }
		//	   binaryPrint(s, 2, c_out );  DC(END); DC(s<<"++s++"); DC(END);
		// 			 } // y1st                      // p=2147483648 ;   p=1<<31;
		
		DC("/@@3@@@/маска пересечений"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
		DC("x0st="<<(short)x0st<<" x1st="<<(short)x1st<<" y0st="<<(short)y0st<<" y1st="<<(short)y1st<<END);
		
		
		//DC(END<<" x0st "<<x0st<<" x0st "<<x1st<<" x1st "<<y0st<<" y0st "<<y1st<<" y1st "<<END);
		/**/
		
		
		DC(" TIME_PRINT MatrixT VVVVVVVVVVVVVVVVVV "); DC(END);
		
		///printMask(c_out ); //-------------------
		
		//							 X0      X1      Y0       Y1
		//							 11		 00		 10		  O1
		//							 00		 11		 10		  O1
		
	}//_____________________________________________________________________________
	
}

#endif
