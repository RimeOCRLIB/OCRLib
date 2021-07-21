#include "GBitset.h"

////////////////////////////////////////////////////////////////////////////////
//  ----------------------------------------------------------------------------
// Подпрограмма для выделения рамки вокруг текста печа.
//  ----------------------------------------------------------------------------
void GBitset::GBitsetNet() {

// LStS, HStS, HBuf, GBuf переменной длинны, max из nrows или ncolumns,
// в LStS кладем вертикальные дифференциальные суммамы
// в HStS лежат горизонтальные дифференциальные суммамы

  //int y,x,d;
  //int NLetter;

         // вычисленное колличество строк в печа - NStrok=strArray.size();
                   NStrok=(uint)strArray->size();
                   if( NStrok<3 ) { return; }

TIME_START

	   // Подпрограмма фильтрации строк, для выявления горизонтальных линий.
						 DiffLineDetector();
				 // получили координаты строк с горизонтальными линиями.

	   // Подпрограмма для выявления коротких горизонтальных строк.
						 ShortStringDetector();
			     // получили флаг строк с горизонтальными линиями.


				 // получили координаты рамки в структуре.

/**/

if( GrafT[15][0] ) { DM(END);  DM(1515151515<<" GBitsetNet ");  DM(END); }

TIME_PRINT

// GBitsetNet  net сеть  map card таблица
}//_____________________________________________________________________________


//						 --------------------------------

//                        ОДИН дифференциальный ЗАПУСК
// Подпрограмма фильтрации строк с вычисленным параметром фильтрации KHigS;
// -----------------------------------------------------------------------------
void GBitset::DiffLineDetector() {
  int y,p,d,n;  // ,s,m
  int KLine,Pv1,Pv2,dStrok,maxDS,maxSS;
  int SmGrL=ncolumns; //M=32,delta,
  vector<int> RBuf(ncnr);

  // BufS,BufT,BufR,BufL модифицируются при выполнении программы ParameterFilteringDetectorA

  //  HStS горизонтальные дифференциальные суммы
  Pv2=1; // импульсный фильтр включен Pv2=1;
  AmplitudeFilteringDetector(HStS, KHigS, &Pv1,&Pv2);  // Amplitude  AmplitudeFilteringDetector

  // в буффере GBuf находяться размах сигнала
  RBuf=GBuf;

  //  HStr горизонтальные суммы
  Pv2=0; // импульсный фильтр выключен Pv2=0;
  AmplitudeFilteringDetector(HStr, KHigS, &Pv1,&Pv2);
  dStrok=Pv2;

  // в буффере HBuf находяться интервалы (координаты) строки внутри которых
  // в итоге вычисляется амплитуды критерий похожести строки на линию

/*
			// ВИЗУАЛИЗАЦИЯ цифровая, координаты строк
			  DM(END);
			  for ( y=0; y < dStrok;  y++ ){   // ТАБЛИЦА
				DM(HBuf[y]<<"\t"); if ((y+1)%10==0)DM(END);
				  }//for y
			  DM(END); DM(END);
*/
	BufT.assign(ncnr,0);  BufS.assign(ncnr,0);  BufHelp.assign(ncnr,0);

  // критерий похожести строки на линию (KLine)
///DM(END);
  d=0;
  for ( n=0; n < dStrok; n+=2 ){  // цикл по количеству строк
	  maxSS=maxDS=p=0;
	  for ( y=HBuf[n+1]; y < HBuf[n]; y++ ){ // цикл по координатам строк
//		  if( SBuf[y]>0 ){ maxSS=SBuf[y]; maxDS=GBuf[y]; DM(SBuf[y]<<" SBuf "); }
		  if( GBuf[y]>maxSS ){ maxSS=GBuf[y]; p=y; }
		  if( RBuf[y]>maxDS ){ maxDS=RBuf[y]; }
		  } // y
		 KLine=10*maxSS/(maxDS+10);  if( KLine>ncolumns ){ KLine=ncolumns; } // 65535
		 // критерий похожести строки на линию KLine
		 BufT[d]=KLine;   BufHelp[p]=128*KLine;
		 // координата по y середины строки
////		 BufS[d]=(HBuf[n]+ HBuf[n+1])/2;

///DM(n<<" n "); DM(d<<" d "); DM(KLine<<" KLine "); DM(BufS[d]<<" BufS ");
///DM(HBuf[n]<<" HBuf "); DM(HBuf[n+1]<<" HBuf+1 ");
///DM(maxSS<<" maxSS "); DM(maxDS<<" maxDS "); DM(END); DM(END);
		 d++;
	  } // n
///DM(END);
/**/


if( GrafT[15][1] ) {
						 // ВИЗУАЛИЗАЦИЯ цифровая
///  DM(Pv2<<" NStrok "); DM(END);

						 // ВИЗУАЛИЗАЦИЯ аналоговая
//SP->filterHigh(BufHelp,KHigS);
//LDraw(BufHelp,SmGrL*0.4, 0x00AA0011);     // цвет линии темно синий
  SP->filterHigh(GBuf,KHigS);
  LDraw(GBuf,SmGrL*0.4, 0x000000FF);        // вызов функции рисования красный
  SP->filterHigh(RBuf,KHigS);
  LDraw(RBuf,SmGrL*0.4, 0x0000AA00);        // вызов функции рисования зеленый
}

  //for ( y=8; y < nrows-8; y++ ){ GBuf[y]=32*GBuf[y]; }
  //LDraw(GBuf,SmGrL*0.4,0x00fff0);         // вызов функции рисования желтый

}//_____________________________________________________________________________

//						 --
//						 -
//						 ---
//						 --------------------------------

// Подпрограмма для выявления коротких горизонтальных строк.
// -----------------------------------------------------------------------------
void GBitset::ShortStringDetector() {
  int n;
  int y0,y1;
  int NLetter,NLetMax,sy,dy;
  //int M=32, delta, SmGrL=ncolumns;

   // получение максимума длинны строки на странице NLetMax
   NLetMax=0;
   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok - глобальные
		 // получение длинны строки из структуры
		 NLetter=strArray[0][Nstr].NLetter;
		 if( NLetter > NLetMax ) { NLetMax=NLetter; }
	}// for Nstr // Цикл по колличеству строк NStrok
/**/

   // получение флага строки состоящий из линии
   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok, DeltaNstr - глобальные
	   // получение координат строки из структуры
	   stringOCR *wS=&strArray[0][Nstr];
	   y0=wS->y0;  y1=wS->y1;  sy=(y0+y1)/2;  dy=(y1-y0)*12 + 16;
	   if( NLetMax < dy ) { NLetMax = dy; }  // 12/3=4 буквы- минимальный порог NLetMax
	   NLetter=wS->NLetter;
		 // критерий похожести строки на линию >45 и строка не является короткой
		 if( BufT[Nstr] > 43 && NLetter > NLetMax/3 ) {
		 // СИНХРОНИЗАЦИЯ двух проходов по обработке строк
			 // когда основная строка попадает в середину строки с линией, то
			 if( HBuf[Nstr+Nstr+1] < sy && sy < HBuf[Nstr+Nstr] ) // && NLetter > NLetMax/3
			 { wS->stringFlag=0; }
			 // если нет, то ищем строку куда попадает основная строка
			 else {   // проверить на CDT / 0003.jpg
				 for ( n=0; n < NStrok; n++ ){
				 if( HBuf[n+n+1] < sy && sy < HBuf[n+n] ) // && strArray[0][Nstr].NLetter > NLetMax/3
				 { strArray[0][Nstr].stringFlag=0; }
				 } // n
			 } // else

		 } // if BufT

///DM(HBuf[Nstr+Nstr+1]<<" HBuf+1 "); DM(y0<<" y0 "); DM(sy<<" sy "); DM(y1<<" y1 "); DM(HBuf[Nstr+Nstr]<<" HBuf ");
///DM(dy<<" dy "); DM(NLetter<<" NLetter "); DM(wS->stringFlag<<" Flag "); DM(END); DM(END);

	}// for Nstr // Цикл по колличеству строк NStrok
/**/

///  DM(END); DM(NLetMax<<" NLetMax "); DM(NLetMax/3<<" NLetMax/3 "); DM(END);

		  // получили флаг строк stringFlag=0; с горизонтальными линиями.
/**/
}//_____________________________________________________________________________



// подпрограмма вычисления двойной амплитуды строк.
// -----------------------------------------------------------------------------
void GBitset::AmplitudeFilteringDetector( vector<int> &BufL,
										  int KHigS, int* Pv1, int* Pv2 ) {
  int y,x,n,d,p,s;
  int MGilb,PGilb; //KLowS
  int LAmp,GAmp1,GAmp2,GAmpS,Amp1,Amp2;
  //vector<int> RBuf(nrows);


			   BufHelp.assign(ncnr,0);   //  PGilb=KHigS;

////               PGilb=*Pv1;    // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
////               ///PGilb=KHigS;       // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
				   MGilb=nrows;      // размер массива преобразования ГИЛБЕРТА
////               KHigS=PGilb*4+8;   // KHigS - параметр фильтрации
				   PGilb=KHigS/4-2;    // PGilb - параметр (ширина) преобразования ГИЛБЕРТА  ////
				   if( PGilb<0 ) PGilb=0;

              // нормировка импульсной характеристики фильтра, т.е. импульсные
              // характеристики фильтров разных порядков должны иметь одинаковые амплитуды.
			  s=sqrt((float)100*KHigS) ;
              for ( y=0; y < nrows; y++ ){
				if( BufL[y]<4 ){d=0;}  // Суммы меньше 8 пикселов считаются равными нолю.
				else { d=BufL[y]*s/7;} // ВАЖНО - сумма вдоль текста HStr[y] домножена на 4  d=4*HStr[y]*s/28;
				BufS[y]=d; //  BufR[y]=
			  } // for y  //BufS[y]=4*HStr[y]*s/28;

			  // импульсный фильтр, 3 последовательных запуска по *Pv2==1.
				for ( n=0; *Pv2==1 && n < 3; n++ ){
				  for ( y=8; y < nrows-8; y++ ){     // <<7
					s=128*abs( BufS[y]-BufS[y-1] )/( 1 + abs(BufS[y]-BufS[y+1]) );
					if( 32 < s && s < 512 ) { BufS[y]=(BufS[y-1]+BufS[y+1])/2; }
					} // y
				} // n

/**/
			  // адаптивная фильтрация  H[y]
			  SP->filterHigh(BufS,KHigS);

              // преобразование ГИЛБЕРТА 1 - BufT
			  filterGilbert(BufS,BufT, MGilb,PGilb);

#ifdef DEBUGLVL
		   LDraw(BufT,200,0x0000AA00);  // вызов функции рисования зеленый ///
#endif

////////////////////////////////////////////////////////////////////////////////
			  // преобразование ГИЛБЕРТА 2 - BufR сигнал без постоянной составляющей
			  filterGilbert(BufT,BufR, MGilb,PGilb);
////////////////////////////////////////////////////////////////////////////////

               // увеличение градиента преобразования ГИЛБЕРТА
               // в окрестности максимума исходной кривой H[y]
               for (  y=6; y < nrows-6; y++ ){
                        x=BufT[y]*BufS[y];
						if(x>=0) BufT[y] = sqrt((float)x);
						else     BufT[y] =-sqrt((float)-x);
               }// for y

#ifdef DEBUGLVL
///            LDraw(BufT,100,0x0000AA00);  // вызов функции рисования зеленый
#endif
/**/

		HBuf.assign(ncnr,0);  GBuf.assign(ncnr,0);
        // Вычисление оптимального параметра фильтрации строк KHigS;.
        // Вычисление максимальнй амплитуды (8 пиксемей от края печа не используются)
		NStrok=d=0;  GAmpS=0;
        for ( y=8; y < nrows-8; y++ ){
		   // вычисление координат середины строки SAmp по максимому энергии
           if( BufT[y]<=0 && BufT[y+1]>0 &&
			   BufT[y-2]< BufT[y]   && BufT[y+1]< BufT[y+3] &&
               BufT[y-4]< BufT[y-2] && BufT[y+3]< BufT[y+5]
			  )
				{ NStrok++;  LAmp=BufS[y];  BufHelp[y]=800;
 // вычисление амплитуд сигнала GAmp1, GAmp2 пропущенного через полосовой фильтр
			   ///	  for ( p=y+8; BufT[p]>0 && p>8 && p < nrows-8; p++ ){ }
				   for ( p=y+4; BufR[p]<BufR[y]/2 && p>8 && p < nrows-8; p++ ){ }
				   GAmp1=BufS[p];  HBuf[d]=p; d++; BufHelp[p]+=-200;////  DM(p<<"p");
			   ///	  for ( p=y-8; BufT[p]<0 && p>8 && p < nrows-8; p-- ){ }
				   for ( p=y-4; BufR[p]<BufR[y]/2 && p>8 && p < nrows-8; p-- ){ }
				   GAmp2=BufS[p];  HBuf[d]=p; d++; BufHelp[p]+=-400;////  DM(p<<"p");
 // вычисление размаха сигнала Amp1, Amp2 пропущенного через полосовой фильтр
				  Amp1=(LAmp-GAmp1); Amp2=(LAmp-GAmp2);
				  GBuf[y]=32*(2*LAmp-GAmp1-GAmp2);
////				  DM(GBuf[y]<<" GBuf ");
#ifdef DEBUGLVL
   ////////       DM("--"<<Amp1<<"--"); DM("--"<<Amp2<<"--");     ////////
#endif
 // вычисление среднего размаха сигнала GAmpS (Amp1&Amp2<0)
				  GAmpS+=Amp1+Amp2;
				} // NStrok++;
			}// for y
///DM(END);


           // нормировка среднего размаха сигнала GAmpS
                  if(NStrok<1) NStrok=1;
				  GAmpS=GAmpS/NStrok;


                   /// возврат ///
					*Pv1 = GAmpS;  // не используется
					*Pv2 = d;
// в буффере HBuf находяться интервалы (координаты) строки внутри которых
// в итоге вычисляется амплитуды критерий похожести строки на линию

// в буффере GBuf находяться размах сигнала

#ifdef DEBUGLVL
if( GrafT[15][3] ) {
/*                //////// ВИЗУАЛИЗАЦИЯ цифровая ////////
//		    DM(END); DM(NStrok<<" NStrok "); DM(END);
							  DM(END);
DM(PGilb<<"  "); DM(GAmpS<<" GAmpS "); DM(NStrok<<" NStrok "); DM(KHigS<<" KHigS ");
						  DM(END); DM(END);
*/
					 // ВИЗУАЛИЗАЦИЯ аналоговая
LDraw(BufS,350,0x000000FF);    // вызов функции рисования красный
LineVertical(350, 0x000000FF);  // красный
LDraw(BufR,350,0x00fff0);         // вызов функции рисования желтый    SmGrL*0.4
LDraw(BufHelp,325,0x000000FF); // вызов функции рисования красный
sm =300; // sm =(KHigS*2+300);
///LDraw(GBuf,sm,0x00AA0011);     // вызов функции рисования ТЕМНО СИНИЙ
LineVertical(sm, 0x00AA0011);  // ТЕМНО СИНИЙ
}
#endif

/* */

}//_____________________________________________________________________________







