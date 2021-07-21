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

#include "GBitset.h"
#include <string.h>
#include <time.h>
#include <math.h>

namespace ocr {

//*****************************angle recognition********************************
//Rotate
float GBitset::detectRotation(){
//#undef DEBUGLVL   ////
maxBitAdress=nrows * ncolumns;    //1
//// vector<int> H;  H.resize(nrows);
//// vector<int> L;  L.resize(ncolumns);

int x;
float minVar, minVarP, maxSumAmp, mean, Alpha, variance;
nfas=32;   // TMP, количество фазовых сечений   1< nfas < 128    16 для крупных печа
nfasSt=32;
KHig=15+1; // TMP, порядок высокочастотного фильтра (k=15+1;) 2< KLow,KHig =<63  31 для крупных печа
float Varianc[3];
float Alph[3];
int SumAmp[3];

int NAverage;  // 0 <NAverage< 2   ДБ глобальной

vector<vector<int> >INFase(nfasSt);  //INFasa.resize(nfas);//массив частичных сумм
vector<vector<int> >INGilb(nfasSt);  //INFasa.resize(nfas);//массив преобразования ГИЛБЕРТА
vector<vector<int> >Buf1(nfasSt);
vector<vector<int> >Buf2(nfasSt);

vector<int> LFase;
vector<int> LGilb;



	   TIME_START

	   // когда появится возможность убирать черную засветку текста contourRecognition
	   // лучше отключить
	   contourRecognition();  DM("contourRecognition "); TIME_PRINT
	   localSumStatic(INFase,INGilb,Buf1,Buf2);DM("localSumStatic ");TIME_PRINT
							Alpha=0; variance=0; mean=0;

					// Усреднение по одному слайсу nfas=32; KHig=7+1;
	   nfas=32;   KHig=7+1;
	   pageStart(INFase,INGilb,Buf1,Buf2,LFase,LGilb,Alpha);DM("ИТОГО pageStart_n32/k8 ");TIME_PRINT
		  // Подпрограмма расчета моментов распределения: матожидания, дисперсии.
			   calculatemoments(LGilb, mean, variance, Alpha);
			   Varianc[0]=variance;  Alph[0]=Alpha;  SumAmp[0]=SumNAmp;

					// Усреднение по одному слайсу nfas=32;  KHig=15+1;
	   nfas=32;   KHig=15+1;
	   pageStart(INFase,INGilb,Buf1,Buf2,LFase,LGilb,Alpha);DM("ИТОГО pageStart_n32/k16 ");TIME_PRINT
		  // Подпрограмма расчета моментов распределения: матожидания, дисперсии.
			   calculatemoments(LGilb, mean, variance, Alpha);
			   Varianc[1]=variance;  Alph[1]=Alpha;  SumAmp[1]=SumNAmp;
/**/
					 // Усреднение по двум слайсам nfas=16; KHig=15+1;
	   nfas=16;   KHig=7+1;    // n16/k16
	   pageStart(INFase,INGilb,Buf1,Buf2,LFase,LGilb,Alpha);DM("ИТОГО pageStart_n16/k16 ");TIME_PRINT
		   // Подпрограмма расчета моментов распределения: матожидания, дисперсии.
			   calculatemoments(LGilb, mean, variance, Alpha);
			   Varianc[2]=variance;  Alph[2]=Alpha;  SumAmp[2]=SumNAmp;


	   // отработка ситуации с малым числом хороших отсчетов при
	   // коэффициентах фильтрации больше оптимального при 3 запусках
			 // вычисление max числа хороших отсчетов.
			 maxSumAmp=0;
			 for (  x=0; x < 3; x++ ) {
				 if( SumAmp[x]> maxSumAmp ) { maxSumAmp=SumAmp[x]; }
			 }   // SumNAmp суммарное число хороших отсчетов по всем слайсам
	   // результаты запусков с числом хороших отсчетов меньше 1/4 от лучшего не используем
			 for (  x=0; x < 3; x++ ) {
				 if(maxSumAmp/4 > SumAmp[x] ) { Varianc[x]=99; }
			 }
 /* */
	   // вычисление Alpha по min дисперсии.
	   // учитывается дисперсия < 6, дисперсия !=1, дисперсия !=0.
			 minVarP=6;  minVar=minVarP;
			 for (  x=0; x < 3; x++ ) {
				 if( Varianc[x]< minVar && Varianc[x]!=1 && Varianc[x]!=0)
				 { minVar=Varianc[x]; Alpha=Alph[x]; NAverage=x+1; }// вычисление min дисперсии.
			 }

				if( minVar>=minVarP ) {Alpha=0;};   if( minVar<=0 ) {Alpha=0;}
				if( Varianc[0]==1 && Varianc[1]==1 && Varianc[2]==1 ) Alpha=0;


#ifdef DEBUGLVL
 DM(END); DM("variance_n32/k8  "<<Varianc[0]<<"  "); DM("Alpha_32 "<<Alph[0]<<"  "); DM(END);
 DM(END); DM("variance_n32/k16 "<<Varianc[1]<<"  "); DM("Alpha_32 "<<Alph[1]<<"  "); DM(END);
 DM(END); DM("variance_n16/k16 "<<Varianc[2]<<"  "); DM("Alpha_16 "<<Alph[2]<<"  "); DM(END);
									DM(END);
 DM("minVar "<<minVar<<"  "); DM("Alpha "<<Alpha<<"  "); DM("NAverage "<<NAverage<<"  ");
								DM(END); DM(END);
#endif


return Alpha;

}//_____________________________________________________________________________



// TestProg1 ФАЗОВЫЙ ДЕТЕКТОР
//float GBitset::pageSegmentation(int* NAverage){

//return 0;

//}//_____________________________________________________________________________

void GBitset::pageStart(vector<vector<int> > &INFase,
						vector<vector<int> > &INGilb,
						vector<vector<int> > &Buf1,
						vector<vector<int> > &Buf2,
						vector<int> &LFase,
						vector<int> &LGilb,
						float &Alpha){


		   TIME_START

		   DM(END);

		   localSum(INFase,INGilb,Buf1,Buf2);DM("localSum "); TIME_PRINT

		   CalculatAmplitude(INFase,INGilb); DM("CalculatAmplitude "); TIME_PRINT
		   deltaSlice(INFase,INGilb); DM("deltaSlice "); TIME_PRINT
		   matExpDispers(INFase,INGilb,LFase,LGilb); DM("matExpDispers "); TIME_PRINT

}//_____________________________________________________________________________

 // ПРЕДОБРАБОТКА ТЕКСТА (пространственный Лапласиан, нормировка скана)

void GBitset::contourRecognition(){

/*

		short w=ncolumns;
		short h=nrows;
		bool *p,*p1,*p2,*p3,*p4,*p5;

		for (int x=5; x<w-3; x++){
		 for (int y=5; y<h-3; y++){                       //      -1
		 //  выделение граници в 2 пиксела                //       0
		 p=A+(y-4)*w+x-4; //A[y-4][x-4]                   //  -1 0 4 0 -1
		 p1=A+y*w+x;      //A[y][x]                       //       0
		 p2=A+(y+2)*w+x;  //A[y+2][x]                     //      -1
		 p3=A+(y-2)*w+x;  //A[y-2][x]
		 p4=A+y*w+x+2;    //A[y][x+2]
		 p5=A+y*w+x-2;    //A[y][x-2]
		 // A[y-4][x-4] =4*A[y][x]-A[y+2][x]-A[y-2][x]-A[y][x+2]-A[y][x-2]
		 *p=*p1*4-*p2-*p3-*p4-*p5;

#ifdef DEBUGLVL
		 drawData[0][y-5][x-5]=255+*p*196;  //draw it
#endif
		}}  // for x,y
*/

	int x,y;
	int w=ncolumns, h=nrows;
	bool *d,*d1,*d2,*d3,*d4,*d5;
    bool  *A=bits_data;

#ifdef DEBUGLVL
// локальный подсчет времени выполнения текущей функции contourRecognition
static clock_t  tm_start1=clock(); clock_t  tm_end; double time;  // DM(tm<<" "<<t);
#endif


	for (y=5; y<h-3; y++){                                //      -1
	   d =A+(y-4)*w-4;  //A[y-4][x-4]                     //       0
	   d1=A+y*w;        //A[y][x]                         //  -1 0 4 0 -1
	   d2=d1+2*w;       //A[y+2][x]                       //       0
	   d3=d1-2*w;       //A[y-2][x]                       //      -1
	   d4=d1+2;         //A[y][x+2]
	   d5=d1-2;         //A[y][x-2]
	   for (x=5; x<w-3; x++){
		 //  выделение граници в 2 пиксела
		 // A[y-4][x-4] =4*A[y][x]-A[y+2][x]-A[y-2][x]-A[y][x+2]-A[y][x-2]
		 *(d+x)=*(d1+x)*4-*(d2+x)-*(d3+x)-*(d4+x)-*(d5+x);
   //      *(d+x)=*(d1+x)*2-*(d2+x)-*(d3+x); // -*(d4+x)-*(d5+x)


#ifdef DEBUGLVL_GREY
	   drawData[0][y-5][x-5]=255+*(d+x)*196;  // draw it
#endif
	}}  // for x,y

#ifdef DEBUGLVL
tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;
DM(" пространственный Лапласиан "); DM(" local time = "<<time<<END);
#endif
/**/
}//_____________________________________________________________________________

   // ПОДСЧЕТ СТАТИЧЕСКИХ ЛОКАЛЬНЫХ СУММ

 void GBitset::localSumStatic(vector<vector<int> > &INFase,
							  vector<vector<int> > &INGilb,
							  vector<vector<int> > &Buf1,
							  vector<vector<int> > &Buf2
							  ){
   int y,x,n,d,d1,d2;
   int nfasrow=ncolumns/(nfasSt+1);   // размер массива частичных сумм
   short res;

		vector<int>bt;
		INFase.push_back(bt); INFase[0].resize(nrows);
		INGilb.push_back(bt); INGilb[0].resize(nrows);
		Buf1.push_back(bt);   Buf1[0].resize(nrows);
		Buf2.push_back(bt);   Buf2[0].resize(nrows);

	for ( n=1; n < nfasSt; n++ ) { /// nfasSt=32
		vector<int>bf;
		INFase.push_back(bf);  INFase[n].resize(nrows);
		INGilb.push_back(bf);  INGilb[n].resize(nrows);
		Buf1.push_back(bf);    Buf1[n].resize(nrows);
		Buf2.push_back(bf);    Buf2[n].resize(nrows);
		  for (  y=0; y < nrows; y++ ) {
		  res=0;   d=y*ncolumns;  d1=nfasrow*n; d2=nfasrow*(n+1); //
			// for ( x=nfasrow*n; x < nfasrow*(n+1); x++ ) {  // подсчет частичных сумм по x
			   for ( x=d1; x < d2; x++ ) {
					res+=bits_data[d + x];
			 }
			 Buf1[n][y]=res*16;   // *4
			 Buf1[0][y]+=res; // тоже что H[y]=res; - входной выходной массив фильтра Лапласиан
			 /*       #ifdef DEBUGLVL
					if(res>=0){
				   for(d =nfasrow*n; d < res/2+nfasrow*n&&d<ncolumns; d++ )drawData[0][y][d]*=0.4;
					 }else{

				   for(d =nfasrow*n; d > res/2+nfasrow*n&&d<ncolumns; d-- )drawData[0][y][d]*=0.4;

				   }

				 #endif
			 */
			}//for (y=0;y<nrows;y++)
		 }//for ( n=1; n < nfas-1; n++ )

#ifdef DEBUGLVL
	  //DM(END); DM(nfasSt<<" nfasSt  "); DM(END);
	  //drawNFase(Buf1,0x00AA0011);  // вызов функции рисования
      //INFase[16][0][nrows/2+1]=10000; INFase[16][0][nrows/2]=10000;
#endif

}//_____________________________________________________________________________

   // ПОДСЧЕТ ЛОКАЛЬНЫХ СУММ И ИХ ФИЛЬТРАЦИЯ

 void GBitset::localSum(vector<vector<int> > &INFase,
						vector<vector<int> > &INGilb,
						vector<vector<int> > &Buf1,
						vector<vector<int> > &Buf2
						){
   int y,n,d;

/*   // TEST
	 for ( n=1; n < nfasSt; n++) {
		  for (  y=0; y < nrows; y++ ) {
		   Buf1[n][y]=n*100;
			}//for (y=0;y<nrows;y++)
		 }//for ( n=1; n < nfasSt; n++ )

	 DM(END);
	 for ( n=1; n < nfasSt; n++  ){
		  DM(Buf1[n][50]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
	 }//for y
	 DM(END);
*/

	  // Усреднение по одному слайсу, копирование Buf1 >> INFase, ФИЛЬТР НИЗКИХ ЧАСТОТ

	   if ( nfas==32 ) {
			  for ( n=0; n < nfasSt; n++ ) {
					for (  y=0; y < nrows; y++ ){INFase[n][y]=0;}// for y
				  }// for n
			  for ( n=1; n < nfasSt; n++ ) {
					for (  y=0; y < nrows; y++ ){
						 INFase[n][y]=Buf1[n][y];
						}// for y
					 SP->filterHigh(INFase[n],KHig); // сглаживание ФИЛЬТР НИЗКИХ ЧАСТОТ
				   }// for n
		   }// if


	  // Усреднение по двум слайсам, копирование Buf1 >> INFase, ФИЛЬТР НИЗКИХ ЧАСТОТ

	  if ( nfas==16 ) {
			  for ( n=0; n < nfasSt; n++ ) {
					for (  y=0; y < nrows; y++ ){INFase[n][y]=0;}// for y
				  }// for n
			  d=0;
			  for ( n=1; n < nfasSt-1; n=n+2 ) {
					d++;
					for (  y=0; y < nrows; y++ ) {
						 INFase[d][y]=(Buf1[n][y]+Buf1[n+1][y])/2;
						}// for y
					SP->filterHigh(INFase[d],KHig); // сглаживание ФИЛЬТР НИЗКИХ ЧАСТОТ
				   }// for ( n=1; n < nfasSt; n++ )
		 }// if

/*  */
#ifdef DEBUGLVL
//drawNFase(Buf1,0x00AA0011);  // вызов функции рисования
/*
//
/// INFase[16][nrows/2+1]=10000; INFase[16][nrows/2]=10000;

							// ВИЗУАЛИЗАЦИЯ цифровая 0
				DM(END);
				for ( n=1; n < nfas; n++  ){
				DM(Buf1[n][50]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
				  }//for y
				DM(END);
*/
#endif
/*
	 for ( n=0; n < nfas; n++ ) {
		  for (  y=0; y < nrows; y++ ){
			   INGilb[n][y]=0; INFase[n][y]=0; Buf2[n][y]=0;
			  }// for y
		 }// for n
*/
}//_____________________________________________________________________________

	   // ВИЗУАЛИЗАЦИЯ АНАЛОГОВАЯ

void GBitset::drawNFase(vector<vector<int> > &INFase, int color){
#ifdef DEBUGLVL
#ifdef DRAW
int y,x,n,in;
//int level=0.5;
 TPoint ris[25000];
///   int nfas=INFase.size()    // количество фазовых сечений   1< nfas < 128  // n < nfas-1+1
   int nfasrow=ncolumns/nfas;   // размер массива частичных сумм  nfasrow=ncolumns/nfas
	for ( n=1; n < nfas; n++ ) {
		   for (  y=0; y < nrows; y++ ) {
					x=nfasrow*n+INFase[n][y]/8;       // /200;
				  // drawData[0][y][x]*=level;     //

				  // drawData[0][y][x+1]*=level;   //  рисование линией

				  // drawData[0][y+1][x]*=level;   //

                   ris[y].x=x;

				   ris[y].y=nrows-y;  //заполнение массива координат линии

		  }//for (y=0;y<nrows;y++)
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,nrows-2);
	}//for ( n=1; n < nfas; n++ )

	ImageProcessor->ImageEnView1->Update();
#endif
#endif
}//_____________________________________________________________________________

//      вычисление MAX амплитуд по всем срезам
// -----------------------------------------------------------------------------

void GBitset::CalculatAmplitude(vector<vector<int> > &INFase,vector<vector<int> > &INGilb){
   int y,n,d,maxA;
   long long maxAsr;
   //int sizeNcol=ncolumns;
//   int size=nrows;

   ////TIME_START

					   // преобразование ГИЛБЕРТА //
	 maxAsr=0; maxA=0;
	 for ( n=1; n < nfas; n++ ) {
			  maxAsr = maxAsr+ maxA;   // вычисление средней амплитуды по всем срезам
			   for (  y=4; y < nrows-4; y++ ){
			   if(INFase[n][y]>maxA) { maxA=INFase[n][y]; }// вычисление max амплитуды в пределах среза
			   INGilb[n][y]= ((INFase[n][y-3]-INFase[n][y+3])/3     // ГИЛБЕРТ
			   +(INFase[n][y-2]-INFase[n][y+2])/2+INFase[n][y-1]-INFase[n][y+1]);
				   }// for y
			   SP->filterHigh(INGilb[n],9+1); // сглаживание ФИЛЬТР НИЗКИХ ЧАСТОТ k=11-15
		  }// for n
 /*
#ifdef DEBUGLVL
drawNFase(INFase,0x000000FF);  // вызов функции рисования красный
//drawNFase(INGilb,0x0000AA00);  // вызов функции рисования зеленый
#endif
 */
		   // вычисление координат максимальных амплитуд в пределах среза //

	 maxAsr=maxAsr/(nfas-1);  // нормировка средней амплитуды по всем срезам
	 //  am=maxAsr; DM(am<<"maxAsr ");  DM(nfas<<"nfas ");
	 for ( n=1; n < nfas; n++ ) {
			  d=0;    INFase[n][0]=0;   INFase[n][1]=0;
			  for (  y=8; y < nrows-8; y++ ){ // максимальные амплитуды в пределах 8 пиксемей от края печа не используются
			  if(INFase[n][y]>maxAsr/2 && INGilb[n][y]<=0 && INGilb[n][y+1]>0
			  && INGilb[n][y-2]<INGilb[n][y] && INGilb[n][y]<INGilb[n][y+2])
			  {/*INGilb[n][y]=7000;*/ d++; INFase[n][d]=y;}
//                 {INGilb[n][y]=210; d++; INFase[n][d]=y;}
				   }// for y              // maxAsr/2 порог отсечки малых алплитуд
				   INFase[n][0]=d;  INGilb[n][0]=d;
		 }// for n

	//    if(INGilb[n][y]<=0 && INGilb[n][y+1]>0
	//    && INGilb[n][y-2]<INGilb[n][y]&&INGilb[n][y]<INGilb[n][y+2]&&INFase[n][y]>maxAsr/2)

   /*
#ifdef DEBUGLVL
//drawNFase(INFase,0x001F4400);  // вызов функции рисования //темно синий
drawNFase(INGilb,0x00AA0011);  // вызов функции рисования
#endif
  */
}//_____________________________________________________________________________

//      вычисление смещений между всеми срезами
//------------------------------------------------------------------------------
void GBitset::deltaSlice(vector<vector<int> > &INFase,vector<vector<int> > &INGilb){
   int y,n;
   int minY,ADR1minY=0,ADR2minY=0,minYabs;
   int N,NBad=0,B2,b,d,BAD=9999;
   //int sizeNcol=ncolumns;       // ГОРИЗОНТАЬНЫЙ РАЗМЕР ПЕЧА  w=ncolumns;
   int size=nrows;              // ВЕРТИКАЛЬНЫЙ РАЗМЕР ПЕЧА   h=nrows;

   ////TIME_START

					  // вычисление смещения срезов
		N=0; NBad=0;
		for ( n=1; n < nfas; n++ ) {
			d=0;
			for (  y=1; y < INFase[n][0]+1 && y<size-1; y++ ){
				  minYabs=size;
				  for ( b=0; b < INFase[n-1][0]+1 && b<size-1; b++ ){
				   B2=minYabs;
				   minY=(INFase[n][y]-INFase[n-1][b]);
				   minYabs = abs(minY);
				   if (minYabs<=B2) { ADR1minY=b; }
					   } // b++  // ADRminY координаты по y минимального значения разности
				  minYabs=size;
				  for ( b=0; b < INFase[n][0]+1 && b<size-1; b++ ){
				   B2=minYabs;
				   minY=(INFase[n][b]-INFase[n-1][ADR1minY]);
				   minYabs = abs(minY);
				   if (minYabs<=B2) { INGilb[n][y]=minY; ADR2minY=b; } // minY минимальное значение разности
					   } // b++  // ADRminY координаты по y минимального значения разности
				   if ( y!=ADR2minY ) { d++; NBad++; INGilb[n][y]=BAD; }  //***
				   else N++;
				}//for y
			}//for n

				   // ***  y!=ADR2minY  проверка совпадения исходных координат по y и
				   // координат полученных при отражении от соседнего слайса при поиске
				   // минимума во время поочередного движении вдоль обоих слайсов  ||


#ifdef DEBUGLVL
  /*           // ВИЗУАЛИЗАЦИЯ цифровая 1
		 DM(END);
		 DM(nfas<<" nfas_nfas ");
		 DM(N<<" N ");        //  N и SumNAmp суммарное число хороших отсчетов по всем слайсам
		 DM(NBad<<" NBad ");  //  NBad число битых отсчетов

		 for ( n=1; n < nfas; n++ ) {
			 DM(END<<" "<<n<<"\t"<<INFase[n][0]<<"  \t");/// ТАБЛИЦА
			  for ( y=1; y <INFase[n][0]+1;  y++ ){
				DM(INFase[n][y]<<" (");   /// ТАБЛИЦА
				DM(INGilb[n][y]<<")  \t");    /// ТАБЛИЦА
				  }//for y
			 }//for n
		  DM(END);
  */

//drawNFase(INFase,0x001F4400);  // вызов функции рисования //темно синий
//drawNFase(INGilb,0x00AA0011);  // вызов функции рисования
 /* */
#endif
/*  */
			   if (N<=2) N=3;   N0Fase = N+1;  // +2
//             DM(N<<" N ");


 }//___________________________________________________________________________

// преобразование массива срезов в один массив, запуск подпрограммы расчета
// распределения вероятностей
//  ---------------------------------------------------------------------------

  void GBitset::matExpDispers(vector<vector<int> > &INFase,
							  vector<vector<int> > &INGilb,
							  vector<int> &LFase,
							  vector<int> &LGilb
							  ){

   int y,n;
   int d,BAD=9999;
   int Porog,Delta;             // Delta число отбрасываемых отсчетов на хвостах
   //int sizeNcol=ncolumns;     // ГОРИЗОНТАЬНЫЙ РАЗМЕР ПЕЧА  w=ncolumns;
   int size=nrows;              // ВЕРТИКАЛЬНЫЙ РАЗМЕР ПЕЧА   h=nrows;


			LFase.resize(N0Fase);
			for ( y=0; y < N0Fase;  y++ ){ LFase[y]=BAD; }

			// преобразование массива срезов в один массив
			// фильтрация по порогу
	   Porog=nrows/8;     // расстояние между срезами в пикселях для угла <45гр
	   if ( Porog>ncolumns/(nfas+1)) Porog=ncolumns/(nfas+1);  // 67
	   d=0;  //  Porog=100;
	   for ( n=1; n < nfas; n++ ) {
			 for (  y=1; y < INGilb[n][0]+1 && y<size-1; y++ ){
				  if ( abs(INGilb[n][y])< Porog )
				  { LFase[d]=INGilb[n][y]; d++; }
				 }//for y
			}//for n

			//  SumNAmp суммарное число хороших отсчетов по всем слайсам
			SumNAmp=d;

 // Вызов подпрограммы расчета распределения вероятностей, фильтрация по порогу,
			// отбрасывание хвостов распределения по адаптивному порогу т.е.отбрасываем
			// Delta штук самых больших и самых маленьких отсчетов по всему массиву.
			Delta=SumNAmp/20+1;  // Delta=2;
			ScheduleProbability(LFase,LGilb, Porog, Delta);
/* */
#ifdef DEBUGLVL
/*
				   // ВИЗУАЛИЗАЦИЯ цифровая 1
			  DM(END);
		 //     Nf = INFase[0].size();
			  for ( y=0; y < N0Fase;  y++ ){      // y < d+1;
				DM(LFase[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
				  }//for y
			  DM(END);

					// ВИЗУАЛИЗАЦИЯ цифровая 2
			  DM(END);
			  Ng = INGilb[0].size();
			  for ( y=1; y < Ng;  y++ ){      // y < d+1;
				DM(INGilb[10][y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
				  }//for y
			  DM(END);

					// ВИЗУАЛИЗАЦИЯ  параметров
			DM(SumNAmp<<" SumNAmp ");  DM(Delta<<" Delta ");
	 DM(Porog<<" Porog "); DM(N0Fase<<" N0Fase "); DM(N0Gilb<<" N0Gilb "); DM(END);

*/
#endif

}//_____________________________________________________________________________

// Вычисление распределения вероятностей
// -----------------------------------------------------------------------------

void GBitset::ScheduleProbability( vector<int> &LFase, vector<int> &LGilb,
								   int &Porog,
								   int &Delta ) {

   int x;
   int Nf,Ng,p;
// int sizeNcol=ncolumns;       // ГОРИЗОНТАЬНЫЙ РАЗМЕР ПЕЧА  w=ncolumns;
// int size=nrows;              // ВЕРТИКАЛЬНЫЙ РАЗМЕР ПЕЧА   h=nrows;



							Nf=N0Fase;
						//  DM(Nf<<" Nf+ ");
							if( Nf==0) { return; }
	/*
			 // TMP
			  DM(END);  DM(END);
			  for ( x=0; x < N0Fase;  x++ ){      // x < d+1;
				DM(LFase[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				  }//for x
			  DM(END);
   */
						   // Сортировка вектора.
// сортируем наш вектор от начала и до конца. http://progs.biz/cpp/stl/lessons/005.aspx
						   sort(LFase.begin(), LFase.end());
				 // работает также как "сортировка Шейкера".

						// "сортировка Шейкера".
				 // http://www.abc-it.lv/index.php/id/610
//                              Nff=Nf-1;
//                          Sort(LFase, Nff);

/*
   int trash=0;
   bool f=true;
   int i,j;
							   // сортировка
	 for ( i=1; (i<=Nff) && (f=true) ; i++)
	   {
		  f=false;
		  for ( j=i; j<=Nff-i; j++) // проходим с лева на право
			 {
				if (LFase[j]>LFase [j+1]) // если число слева больше числа
			   {
				  trash=LFase[j]; // справа, то меняем местами
				  LFase[j]=LFase [j+1]; // справа собираются большие числа
				  LFase[j+1]=trash;
				  f=true;
			   }
			 }
			   for ( j=Nff-i-1; j>i ; j--)  // проходим с права на лево
				 {
				   if (LFase[j]<LFase[j-1])  // если число справа меньше числа
				   {
				   trash=LFase[j]; // слева, то меняем местами
				   LFase[j]=LFase[j-1]; // слева собираются меньшие числа
				   LFase[j-1]=trash;
				   f=true;
				 }
				 }
			 }
   */

	/*        // TMP
			  DM(END);            // size-1   N0Fase
			  for ( x=0; x < N0Fase;  x++ ){      // x < d+1;
				DM(LFase[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				  }//for x
			 DM(END);
   */

			//определение безопастного размера массива Ng NewSizeMassif
				Ng=0;
				for ( x=0; x <Nf-2; x++ ) {  // +1
					if ( abs(LFase[x])<Porog && abs(LFase[x+1])<Porog ){
					p=LFase[x+1]-LFase[x]; Ng=Ng+p;}
					}//for x
	 //             Ng=Ng+nrows/4+50; // Ng-ширина распределения вероятностей, nrows/8; (100)-max мат ожидание, 50-запас.
					Ng=Ng+2*ncolumns/(nfas+1)+50; // Ng-ширина распределения вероятностей, nrows/8; (100)-max мат ожидание, 50-запас.

						N0Gilb = Ng ;
						LGilb.resize(N0Gilb);
				for ( x=0; x < N0Gilb;  x++ ){ LGilb[x]=0; }
					//теперь LGilb массив размера (Ng) // LGilb.assign(N0Gilb,0);

					// вычисление распределения вероятностей
////                // фильтрация по порогу, отбрасывание хвостов распределения
				 // p=0; x <Nf-Delta-1
				 for ( x=Delta; x <Nf-Delta; x++ ) {
					if( abs(LFase[x])<Porog )
					   {
					   p=LFase[x]+ Ng/2;
					   if( p > 0 && p < Ng ){  // p выходящие за пределы массива Ng
						   LGilb[p]++; }       // отбрасываем (хотя таких не дб)
					   }
					 }//for x


#ifdef DEBUGLVL
 /*                        // ВИЗУАЛИЗАЦИЯ цифровая 0
			   DM(N0Gilb<<" N0Gilb ");
			   DM(END);
			   // INGilb[0][Ng/2]=9999;
				for ( x=0; x < N0Gilb;  x++ ){
				DM(LGilb[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				 }//for x

DM(END); DM(Porog<<" Porog calc "); DM(N0Fase<<" N0Fase calc "); DM(N0Gilb<<" N0Gilb calc ");
DM(END);
*/
#endif
/*
	 //    if ( abs(LFase[x])<Porog && p>=0 && p<Ng ){ LGilb[p]++; }
	 ///   if ( LFase[x]<Porog){ LGilb[p]++; }
*/
}//_____________________________________________________________________________




/*******************************************************************************
Подпрограмма расчета моментов распределения: матожидания, дисперсии.

Входные параметры:
	X       -   массив значений с нумерацией элементов [0..N-1]
	N       -   число значений в массиве

Выходные параметры
	Mean    -   математическое ожидание
	Variance-   дисперсия
*******************************************************************************/

void GBitset::calculatemoments(vector<int> &LGilb,
							   float &mean,
							   float &variance,
							   float &Alpha )  {

   int x;
   int Ng,B1,B2,c;
   float F,Bf1,cf;
   //int sizeNcol=ncolumns;        // ГОРИЗОНТАЬНЫЙ РАЗМЕР ПЕЧА  w=ncolumns;
   int size=nrows;                 // ВЕРТИКАЛЬНЫЙ РАЗМЕР ПЕЧА   h=nrows;

						 Ng=N0Gilb;
					if( Ng==0) { return; }

	// вычисление мат ожидания по распределению вероятностей
				  c = Ng/2; B1=0; B2=0;
				 for ( x=0; x < Ng; x++ ) {
				  if ( LGilb[x]!=0){
					 B1=B1+(x-c)*LGilb[x];
					 B2=B2+ LGilb[x]; }
					  }//for x

					// вычисление mean
				  if ( B2==0 ){Alpha=45;  }
				  else { mean=B1*100/B2;  F=(float)B1/B2;    //  DP(" F0="<<F);

					 // вычисление угла в градусах
					 //F=(float)(mean/100)/(srcW/(nfas+1))*(srcH/size);  ////  (nfas)
					 int s1=(nfas+1)*srcH, s2=size*srcW;
					 F=(float)s1*F;                          //   DP(" F1="<<F);
					 F=(float)F/s2;                          //   DP(" F2="<<F);
					 Alpha=57.29747*atan(F);            // 57.29747 = 180/3.1415
				  } // else

// srcW/nfas расстояние между срезами в пикселях, с учетом размера печа srcW=ref.columns();
// srcH/size вертикальный коэффициент масштаба с учетом размера печа  srcH=ref.rows();
// DM(END);DM(END); DM(srcW<<" srcW "); DM(srcH<<" srcH ");DM(END);DM(END);

			  // вычисление дисперсии (variance) float

			  Bf1=0; B2=0; cf = (Ng*50 + mean)/100;      // cf = Ng/2 + mean/100;
			  for ( x=0; x < Ng; x++ ) {
				   if ( LGilb[x]!=0) {
				   Bf1=Bf1+(x-cf)*(x-cf)*LGilb[x];
				   B2=B2 + LGilb[x]; }
				   } //for x
					// вычисление variance
				 if ( B2==0 ){ variance=100; }
				 else { variance = Bf1/B2; }
				 if ( variance<0 ){ variance=-variance; }
				 variance = sqrt(variance);
					// minYabs = abs(minY);

#ifdef DEBUGLVL
				// ВИЗУАЛИЗАЦИЯ  параметров
                   //  DM(Ng<<" NgNg ");
						   DM(END);
DM(mean<<" mean calc "); DM(variance<<" variance calc "); DM(Alpha<<" Alpha calc ");
						DM(END);DM(END);

/*
				 // ВИЗУАЛИЗАЦИЯ цифровая
		  //    LGilb[Ng/2]=10000+LGilb[Ng2];
			  DM(END);
			  for ( y=0; y < Ng;  y++ ){
				DM(LGilb[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
				  }//for y
			  DM(END);
		   //   LGilb[Ng/2]=LGilb[Ng/2]-10000;
*/
#endif
/*
// ---------------------------------
// Запись/чтение данных в регистр Data
// использована функця Out32 библиотеки inpout32.dll
int Address=888;
int data=2;
	Out32(Address, data);
// ---------------------------------
*/

}//_____________________________________________________________________________

}




