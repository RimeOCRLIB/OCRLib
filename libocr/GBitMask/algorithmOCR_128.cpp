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

#ifndef _algorithmOCR_128_H
#define _algorithmOCR_128_H

# include "../config.h"
#include "GBitMask.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GBitmap/GBitmap.h"
#include "../OCRString/php2stl.h"
#include <string.h>
#include <fstream>


#include <valarray>


using namespace ocr;


						// ПСЕВДОГРАФИКА
// Вывод на экран битовых масок ON
// -----------------------------------------------------------------------------
void GBitMask128::printMask(){

int step=mW/32;
	cout<<"start print mW="<<mW<<"mH="<<mH<<" step="<<step<<endl;
	
for(int y=0;y<mW*mH/32;y+=step){
   for(int t=0;t<step;t++){
       binaryPrint(On[y+t],2);
   }
   cout<<endl;
}
cout<<"print DONE128"<<endl;
}//_____________________________________________________________________________


						// ГРАФИКА
// Вывод в битмап битовых масок ON и OFF упакованных в массив 64 long int.
// -----------------------------------------------------------------------------
GBitmap* GBitMask128::unPackMask(){ 

int y,x,d;
unsigned int s;
// входной массив - битовая маска из 64 long int, упакованная в массив "MaskInt"
unsigned int *MaskInt=On;
unsigned char c;
//DC("START printMask128 ON 1"<<END);
    int t=mH*mW/32;   //cout<<" mW="<<mW<<" mH="<<mH<<endl;
GBitmap* img=GBitmap::create(mW,mH);	
unsigned char *inputData=img[0].bytes_data;
			for( y=0; y < t ; y++ ){ 
				//binaryPrint(MaskInt[y],2);
				//cout<<MaskInt[y]<<" ";
				//if(f==2){f=0;cout<<endl;}f++;
				d=y*32+32;
				s=MaskInt[y];
				for( x=0; x < 32; x++ ){
//				  c=s & 1;      s=(s >> 1); // прямой порядок пикселей
				  c=s & 1; s=(s >> 1); // обратный порядок пикселей
                  
				  inputData[d-x-1]=255-255*c;     //d-x-1 восстанавливает прямой порядок пикселов на картинке
				} // x
			} // y

return img;

}//_____________________________________________________________________________

//упаковка битмапа в битовую маску
 void GBitMask128::packBitMask128(GBitmap *img){
  unsigned int reg;
  NMask=0;
  //int print=1;
  int mWImg=img->columns();    // произвольные размеры картинки по ширмне
  int mHImg=img->rows();       // произвольные размеры картинки по высоте
  if(mHImg>128)mHImg=128;
  mH=0;	 
  if(mWImg>0&&mWImg<=32)mW=32;
  if(mWImg>32&&mWImg<=64)mW=64;
  if(mWImg>64&&mWImg<=96)mW=96;
  if(mWImg>96&&mWImg<=128)mW=128;   // ширмна (фиксированная) прямоугольника маски
  mH=mHImg;                         // высота прямоугольника маски
				  // mW=img->columns();  // TMP  // произвольные размеры картинки
  int ncVnr=mW*mH, ncInr=mWImg*mH;
  unsigned int  *MaskInt=On;

  GBitmap *img128=GBitmap::create(mW,mH);  //создаем пустую картинку кратную 32
  img128->fill(0);
 unsigned char *inputData=img[0].bytes_data;

  // промежуточный массив >1024 char
  unsigned char *BufMask=(unsigned char*)malloc(ncVnr);
  // обнуление BufMask
  memset(BufMask,0,ncVnr);    // memset(BufMask,0,szf*(ncVnr));

  DC(END<<"START 128PACK 2 mWImg="<<mWImg<<END);

			 //автоопределение количества градаций
			 int max=0;
			 for (int x=0; x < ncInr; x++ ) {
			   if ( inputData[x]>max )max=inputData[x];
			 }


			 // бинаризация входного массива char
			 //DC("/1/___"<<END);

			 if(max>1){
					for (int y=0; y < mHImg; y++ ) {
						 for (int x=0; x < mWImg; x++ ) {
						   if ( img[0][y][x]>127 ){ img128[0][y][x]=0; }
						   else { img128[0][y][x]=1;}
							//if(img128[0][y][x]==1){DC("1");}else{DC("0")}
							//if ((x+1)%mWImg==0)DC(END);
						 }  // x
					}
			 }  // if

             inputData=img128[0].bytes_data;
/*
			  // Тест - сумма единиц по периметру X Y
			  for ( x=0; x < ncInr; x++ ) { BufMask[x]=0; }
			  BufMask[0]=BufMask[mWImg-1]=BufMask[ncInr-mWImg]=BufMask[ncInr-1]=1;
*/

			  // допустимое количество пикселей для срабатывания сторон маски пересечений,
			  // сравнивается раздельно для каждой стороны прямоугольной маски пересечений
			  //int porogS=1;

			  //							 X0      X1      Y0       Y1
			  //							 11		 00		 10		  O1
			  //							 00		 11		 10		  O1

			  // сумма единиц sumX0 по периметру X0 прямоугольной маски пересечений
			  //int sumX0=0, sumX1=0, sumY0=0, sumY1=0;
			  //for ( x=0; x < mWImg; x++ ) { sumX0+=inputData[x]; } // Тест BufMask[x]
			  // сумма единиц sumX1 по периметру X1 прямоугольной маски пересечений
			  //for ( x=ncInr-mWImg; x < ncInr; x++ ) { sumX1+=inputData[x]; }

			  // сумма единиц sumY0 по периметру Y0 прямоугольной маски пересечений
			  //for ( y=0; y < ncInr; y+=mWImg ) { sumY0+=inputData[y]; }
			  // сумма единиц sumY1 по периметру Y1 прямоугольной маски пересечений
			  //for ( y=mWImg-1; y < ncInr; y+=mWImg ) { sumY1+=inputData[y]; }

//***********

DC(END<<" суммы пересечений "<<sumX0<<" sumX0 "<<sumX1<<" sumX1 "<<sumY0<<" sumY0 "<<sumY1<<" sumY1 "<<END);

/*
				// Вывод на экран теста - сумма единиц по периметру X Y
				DC(END);
				for ( x=0; x < ncInr; x++ ) {
					if(BufMask[x]==1){DC("1");}else{DC(".")}
					if ((x+1)%mWImg==0) DC(END);
				} // x
				DC(END);
				memset(BufMask,0,ncVnr); // обнуление BufMask

			  // вычисление максимальных и минимальных габаритов произвольной
			  // входной области (mWImg*mH) < 32, выделенной мышкой
			  x0=128, y0=128, x1=0, y1=0;
			  for ( y=0; y < mH; y++ ) {
				p=y*mWImg;
				for ( x=0; x < mWImg; x++ ) {
					if( inputData[x+p] == 1 ) {
					  if( x > x1 ) { x1=x; }   if( x < x0 ) { x0=x; }
					  if( y > y1 ) { y1=y; }   if( y < y0 ) { y0=y; }
					}
				} // x
			  } // y

DC(END<<" маска пересечений "<<x0<<" x0 "<<x1<<" x1 "<<y0<<" y0 "<<y1<<" y1 "<<END);

			  // величина сдвига входной области в центр
			  int dx=(mW-x0-x1)/2,     dy=(mH-y0-y1)/2;
DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(mW<<" mW "); DC(mH<<" mH "); DC(END);

			  // x0,x1,y0,y1 габариты прямоугольной однопиксельной маски
			  // пересечений c учетом сдвига в центр
			  //x0st=0; x1st=0; y0st=0; y1st=0;
			  x0st=0; x1st=0; y0st=0; y1st=0;
			  if ( sumX0 >=porogS ) { x0st=1; }
			  if ( sumX1 >=porogS ) { x1st=1; }
			  if ( sumY0 >=porogS ) { y0st=1; }
			  if ( sumY1 >=porogS ) { y1st=1; }

			  x0+=dx;  x1+=dx;
			  y0+=dy;  y1+=dy;
/

			  // если входная область соприкасается с краеми то её не сдвигаем
			  if(sumX0>=porogS || sumX1>=porogS) dy=0;  if(sumY0>=porogS) dx=0;
			  // если входная область соприкасается с правым краем то её
			  // обязательно сдвигаем  вправо на mW-mWImg
			  if(sumY1>=porogS) dx=mW-mWImg;
			  if(sumY0>=sumY1+1) { dx=0; }
			  DC(END); DC(sumY0<<" sumY0 "); DC(sumY1<<" sumY1 "); DC(dx<<" dx "); DC(END);

			  DT("/@@@@/ yMask="<<yMask<<" dy="<<dy<<END);
			  DT("/@@@@/ xMask="<<xMask<<" dx="<<dx<<END);
			  xMask-=dx;        yMask-=dy;
			  //DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(END);

			  // сдвиг в центр по максимальным и минимальным габаритам входной
			  // области (mWImg*mH) и пересчет её размеров на полную маску (mW*mH)
			  dy=dy*mW;
			  for ( y=0; y < mH; y++ ) {
				p=y*mW;  q=y*mWImg;  i=0;
				for ( x=0; x < mWImg; x++ ) {
					 d=x+dx+p+dy;
					 //if( i < mWImg ) { i++; }
					 if( d > ncVnr || d < 0  ) break;
					 BufMask[d]=inputData[x+q];    // i+q
				} // x
			  } // y



			  // сдвиг в центр максимальных и минимальных габаритов центральной облласти "ON"
			  int dx=(mW-x0-x1)/2,  dy=(mH-y0-y1)/2;
			  mask64[0].x=x+dx;     mask64[0].y=y+dy;
			  //DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(END);
			  dy=dy*mW;
			  for ( y=0; y < mH; y++ ) {
				p=y*mW;
				for ( x=0; x < mW; x++ ) {
					 d=x+dx+p+dy;
					 if( d > ncVnr || d < 0  ) break;
					 BufMask[d]=inputData[x+p];
				} // x
			  } // y
*/
DC(END<<"/@@@@/ "<<" ncVnr="<<ncVnr<<" mH="<<mH<<" mW="<<mW<<" yMask="<<yMask<<END);

								  // ЗАПАКОВКА
			  // Запаковка однобитовой маски из массива mW*mH bool (1 байт)
			  // в массив mH 32 разрядных слов unsigned int (4 байта)
	          reg=0; 
			  for(int y=0; y < mH*mW/32; y++ ){
				 int d=y*32;
				 for (int x=0; x < 32; x++ ) {
					 reg=(reg << 1) + inputData[d+x]; // обратный порядок пикселей  // + BufMask[d + x]
///					 s=(BufMask[d + x])<<31; // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
///					 reg=s + (reg >> 1) ;               // прямой порядок пикселей
				 } // x
				MaskInt[y]=reg;
				//binaryPrint(reg,2); if(f==2){cout<<endl;f=0;}f++;
				//MaskInt1[y]=reg;
			  } // y


			  //DC("/2/___"<<END);
// Вывод на экран битовых масок ON и OFF упакованных в массив mH unsigned int(32p)
//printMask();

// TipMasc=1  размер и форма маски на высокое  разрешение ( 600 dpi )
// TipMasc=0  размер и форма маски на малое разрешение ( <300 dpi )

			  // Вывод на экран битовых масок ON и OFF упакованных в массив mH unsigned long long(64p)
			  //printMask();



/*
			  DC("packGBitMask входной массив"<<END);
			  DC(END);  // TABL
			  for ( x=0; x < mH; x++ ) { // ncVnr
				 //DC((int)inputData[x]);
//				 if(MaskInt0[x]==1){DC("1");}else{DC(".")}
//				 if ((x+1)%mW==0)DC(END);
				 DC((unsigned long long)MaskInt0[x]);  DC(END);
			  }
			  DC(END);
*/
if ( BufMask !=NULL ) free(BufMask);
DC("PACK DONE 2"<<END); DC(END);

////////////////////////////////////////////////////////////////////////////////

// (Sb >> 1) & 0x5555555555555555;   //& 0x1111111111111111 // | 0x0000000000000000
/*
				 // TEST  дельта функция
				 BufMask[ncVnr/2 + mW*4/50]=1;      //   1 << 7
				 BufMask[ncVnr/2 + mW*4/5 + mW + mW]=1;

			// unsigned long 4 // <<63 2147483648      <<31 2147483648     <<30 1073741824
			unsigned long long f=1<<63; // <<63 2147483648      <<31 2147483648     <<30 1073741824
			DC(END); DC(f<<"f "); DC(END);
*/
/*
//компиляторы стремятся совмещать счетчик цикла с указателем на обрабатываемые данные.
//Код вида "for (i = 0; I < n; i++) n+=a[i];"  превращается ими в
//for (p= a; p < &a[n]; p++) n+=*p; //gut

int a[10][20][30];
void example(void)
{
		int i, j, k;
		for (k = 0; k < 10; k++)
				for (j = 0; j < 10;j++)
						for (i = 0; i < 10; i++)
								a[i][j][k] = 1;
}
//Листинг 14. Неоптимизированный кандидат на регистровую ре-ассоциацию.

int a[10][20][30];
void example (void)
{
		int i, j, k;
		register int (*p)[20][30];
		for (k = 0; k < 10; k++)
				for (j = 0; j < 10; j++)
						for (p = (int (*)[20][30]) &a[0][j][k], i = 0; i < 10; i++)
		*(p++[0][0]) = 1;
}
//Листинг 15. Оптимизированный вариант - счетчик цикла совмещен с указателем на массив.
*/
///DC(END); DC(q<<" XXX ");      DC("xxx"<<END); DC(END);
////////////////////////////////////////////////////////////////////////////////

}//_____________________________________________________________________________

void GBitMask128::testVector(ofstream &c_out){

// vector<int> lt1(size+s6);
// int BufIn[10000]={0};
// bit_vector  AMask(ncolumns);
// vector <vector <vector <int> > > a;    //time=0.89
//valarray <int> a[10][20][30];

DC("AAAAAAAAA"<<END); DC(END);



/*
TIME_START
	  int a[10][20][30];
	  int i, j, k;
		register int (*p)[20][30];
for( int m=0; m < 100000	; m++ ){

		for (k = 0; k < 10; k++){   // time=0.828
		  for (j = 0; j < 10; j++){
			for (p = (int (*)[20][30]) &a[0][j][k], i = 0; i < 10; i++){
			   *(p++[0][0]) = 1;
//			   a[i][j][k] = 1;
			}
		  }
		}


		for (k = 0; k < 10; k++)   // 0.937
				for (j = 0; j < 10;j++)
						for (i = 0; i < 10; i++)
								a[i][j][k] = 1;


} // m
TIME_PRINT
*/
/**/
			int s=10000;
			// w 0,61
			//int a[10000];   // time=0.672
			//valarray <int> a[10000]; // 1.719
			//valarray <int> a(s); // time=1.235
			//vector <int> a(s);  //1.234

 int *a;
  a=(int*)malloc(s*4);        // time=0.688  0.672   time=0.672
  // выход в случае отсутствия свободной памяти запрашиваемого размера

  if (a==NULL) {DC("a"); exit (1);}
/**/
		   //int a[10000][10000];
TIME_START
/*		   //unsigned long long i,q=3,w,j;
		   int i,q=3,w,j;
		   //short i,q=3,w,m;
		   for( j=0; j < 10000; j++ ){
			  for( i=0; i < 10000; i++ ){
///				 w=(i*q+i+q+j+1234)*(i*q+i+q+j+1234)& 0x3333333333333333;
				 a[j]=(i*q+i+q+j+1234)*(i*q+i+q+j+1234)& 0x3333333333333333;
			  } // i
		   } // j
*/

		   int i,j;
		   for( j=0; j < 10000; j++ ){
			  for( i=0; i < 10000; i++ ){
				// a[i]=(i*q+i+q+j+1234)*(i*q+i+q+j+1234)& 0x3333333333333333;
			  } // i
		   } // j

TIME_PRINT
//	  int a[10][20][30];
//	  int i, j, k;
//		for (k = 0; k < 10; k++){   // time=0.828
//		  for (j = 0; j < 10; j++){
//			for (p = (int (*)[20][30]) &a[0][j][k], i = 0; i < 10; i++){
//			   *(p++[0][0]) = 1;
///			   a[i][j][k] = 1;
//			}
//		  }
//		}

  //  освобождение памяти
  if ( a !=NULL ) free(a);

/**/
}//_____________________________________________________________________________

int GBitMask128::hashValue(){
	string str;
	char buff[25];
 	for(int i=0;i<mH;i++){
		sprintf(buff,"%d", On[i]);
		str+=buff;
	}
	return strToHash(str.c_str(),(int)str.size());

};


#endif
