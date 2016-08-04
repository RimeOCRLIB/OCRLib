#include "GBitset.h"
// Letter //
//  Подпрограмма работает с монотонными блоками строк, на данном этапе эти блоки
//  надо будет выделять мышкой  (печа достаточно регулярны по строкам и мышка не
//  понадобится), в дальнейшем надо писать сегментацию страници для выделения
//  этих самых монотонных блоков строк.
//  ----------------------------------------------------------------------------
//  Подпрограмма выделения строк ПЕЧА ( сегментация строк - направляющие РЕЛЬСЫ  )
//  ----------------------------------------------------------------------------


// Подпрограмма предворительного выделения строк для сегментации текста печа.
// -----------------------------------------------------------------------------

void GBitset::PrimaryStringDetector(){
  int n;
  int Pv1=0, max;


TIME_START
/*
  BufS.resize(nrows);
  BufT.resize(nrows);
  BufR.resize(nrows);
  BufL.resize(nrows);
  BufHelp.resize(nrows);
*/
   // заполненние массива HStr[y] горизонтальными суммами  и
   // массива HStS[y] горизонтальные дифференциальными суммами
   BitsSummmyWH(1);
   // несколько запусков (5) подпрограммы выделения строк для вычисления
   // оптимального параметра фильтрации KHigS, по max возвращенных значений.
            max=0;
            for ( n=0; n < 5; n++ ) {  //KHigS=n;  // KHigS Глобальная переменная  p=Pv1;
               Pv1=n;
               ParameterFilteringDetector(&Pv1);
               if( Pv1 > max ) { max=Pv1; KFStrok=n; } // KFStrok - max n
            } // for n

            // получили оптимальный параметр фильтрации KHigS.Глобальная переменная
            // KFStrok  номер оптимального коэффициента фильтрации строк
			KHigS=KFStrok*4+8;   //DM(END); DM(KHigS<<" KHigS_AFTER ");


// Подпрограмма фильтрации строк с оптимальным параметром KHigS, для выявления горизонтальных линий

////                  DiffStringDetector();
                 // получили предворительные координаты строк в структуре
                 // без строк, сформированными горизонтальными линиями.
// Подпрограмма вычисления координат строк с оптимальным параметром фильтрации KHigS;
                  RepeatStringDetector();
                 // получили координаты строк в структуре.
/**/

if (  GrafT[1][0] ) {  //!=0
#ifdef DEBUGLVL_GREY
/*           DM(KFStrok<<" KFStrok "); // DM(KHigS<<" KHigS "); DM(END);

			  // ВИЗУАЛИЗАЦИЯ цифровая, параметры фильтрации KHigS;
			  DP(END);
			  for ( y=0; y < 4;  y++ ){        // ТАБЛИЦА
				DP(KGAmpS[y]<<"\t"); if ((y+1)%10==0)DM(END);
				  }//for y
			  DP(END);
*/
			// ВИЗУАЛИЗАЦИЯ цифровая, координаты строк, медленный вывод
/*			 DP(END);
			  for ( y=0; y < NStrok*4;  y++ ){   // ТАБЛИЦА
				DP(HBuf[y]<<"\t"); if ((y+1)%10==0)DP(END);
				  }//for y
			  DP(END); DP(END);
 */
			// ВИЗУАЛИЗАЦИЯ аналоговая, координаты строк
		for(int y=2; y < nrows-2; y++){
		if(HBuf[y-1]==1) { for(int  x =0; x <ncolumns; x=x+4 ) drawData[0][HBuf[y]][x]*=0.1;
						   for(int  x =0; x <ncolumns; x=x+4 ) drawData[0][HBuf[y]-1][x]*=0.1;}
		if(HBuf[y-1]==2) { for(int  x =0; x <ncolumns; x=x+2 ) drawData[0][HBuf[y]][x]*=0.1;
						   for(int  x =0; x <ncolumns; x=x+2 ) drawData[0][HBuf[y]-1][x]*=0.1;}
	  } //for y       //0.8*ncolumns;

	 DM(END);  DM(1111<<" PrimaryStringDetector ");  DM(END);
#endif
}
/*
  BufS.resize(ncnr);
  BufT.resize(ncnr);
  BufR.resize(ncnr);
  BufL.resize(ncnr);
  BufHelp.resize(ncnr);
*/
TIME_PRINT

// BufT.assign(ncnr,0); BufR.assign(ncnr,0);   BufS.assign(ncnr,0);
//  int KFiltr[4]={8,12,16,24};  // параметры фильтрации KHigS;
}//_____________________________________________________________________________

//                             ПЯТЬ ЗАПУСКОВ
//  подпрограмма вычисления параметра фильтрации строк KHigS, (5 запусков)
// -----------------------------------------------------------------------------
void GBitset::ParameterFilteringDetector(int* Pv1) {
  int y,x,d,p,s;
  int MGilb,PGilb,KLowS;
  int LAmp,GAmp1,GAmp2,GAmpS,Amp1,Amp2;
  //vector<int> BufS(nrows);
              

               BufHelp.assign(ncnr,0);   //  PGilb=KHigS;

               PGilb=*Pv1;    // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
               ///PGilb=KHigS;       // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
               MGilb=nrows;      // размер массива преобразования ГИЛБЕРТА
               KHigS=PGilb*4+8;   // KHigS - параметр фильтрации

              // нормировка импульсной характеристики фильтра, т.е. импульсные
              // характеристики фильтров разных порядков должны иметь одинаковые амплитуды.
			  s=sqrt((float)100*KHigS) ;
              for ( y=0; y < nrows; y++ ){
                if( HStr[y]<8 ){d=0;}  // Суммы меньше 8 пикселов считаются равными нолю.
                else { d=HStr[y]*s/7;} // ВАЖНО - сумма вдоль текста HStr[y] домножена на 4  d=4*HStr[y]*s/28;
                BufS[y]=BufR[y]=d; 
              } // for y  //BufS[y]=4*HStr[y]*s/28;


              // адаптивная фильтрация  H[y]
			  SP->filterHigh(BufS,KHigS);

              // преобразование ГИЛБЕРТА 1 - BufT
              filterGilbert(BufS,BufT, MGilb,PGilb);

#ifdef DEBUGLVL
///           LDraw(BufT,200,0x0000AA00);  // вызов функции рисования зеленый
#endif

              // сглаживание, полосовой фильтр
              KLowS=KHigS*3;
              SP->filterLaplasian(BufR,KHigS,KLowS);

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


        // Вычисление оптимального параметра фильтрации строк KHigS;.
        // Вычисление максимальнй амплитуды (8 пиксемей от края печа не используются)
        NStrok=0; GAmpS=0; // d=0;
        for ( y=8; y < nrows-8; y++ ){
           // вычисление координат середины строки SAmp по максимому энергии
           if( BufT[y]<=0 && BufT[y+1]>0 &&
               BufT[y-2]< BufT[y]   && BufT[y+1]< BufT[y+3] &&
               BufT[y-4]< BufT[y-2] && BufT[y+3]< BufT[y+5]
			  )
				{ NStrok++;  LAmp=BufR[y]; // BufHelp[y]=800;
 // вычисление амплитуд сигнала GAmp1, GAmp2 пропущенного через полосовой фильтр
                  for ( p=y+8; BufT[p]>0 && p>8 && p < nrows-8; p++ ){ }
				   GAmp1=BufR[p];  // BufHelp[p]+=-100;
				  for ( p=y-8; BufT[p]<0 && p>8 && p < nrows-8; p-- ){ }
				   GAmp2=BufR[p];  // BufHelp[p]+=-200;
 // вычисление размаха сигнала Amp1, Amp2 пропущенного через полосовой фильтр
                  Amp1=(LAmp-GAmp1)/32; Amp2=(LAmp-GAmp2)/32;
#ifdef DEBUGLVL
   ////////       DM("--"<<Amp1<<"--"); DM("--"<<Amp2<<"--");     ////////
#endif
 // вычисление среднего размаха сигнала GAmpS (Amp1&Amp2<0)
                  GAmpS+=Amp1+Amp2;
				} // NStrok++;
			}// for y

           // нормировка среднего размаха сигнала GAmpS
                  if(NStrok<1) NStrok=1;
                  GAmpS=GAmpS/NStrok;

                     // возврат
                    *Pv1 = GAmpS ;


#ifdef DEBUGLVL
/*                //////// ВИЗУАЛИЗАЦИЯ цифровая ////////
                              DM(END);
DM(PGilb<<"  "); DM(GAmpS<<" GAmpS "); DM(NStrok<<" NStrok "); DM(KHigS<<" KHigS ");
                          DM(END); DM(END);
*/
/*				  // ВИЗУАЛИЗАЦИЯ аналоговая
LDraw(BufS,350,0x000000FF);    // вызов функции рисования красный
LDraw(BufHelp,100,0x000000FF); // вызов функции рисования красный
sm =300; // sm =(KHigS*2+300);
LDraw(BufR,sm,0x00AA0011);     // вызов функции рисования ТЕМНО СИНИЙ
	*/
#endif

//if(p==0) KHig=8;  if(p==1) KHig=12;  if(p==2) KHig=16;  if(p==3) KHig=20; if(p==4) KHig=24;
// BufT[y]=sqrt(abs(BufT[y]*BufS[y]));---
}//_____________________________________________________________________________


//                             ОДИН ЗАПУСК
// Подпрограмма фильтрации строк с вычисленным параметром фильтрации KHigS;
// -----------------------------------------------------------------------------
void GBitset::RepeatStringDetector() {
  int y,x,p,d,s;
  int MGilb,PGilb;
  int print=1;
  /// GAmp; амплитуда сигнала после 2х преобразований ГИЛБЕРТА.


			   BufHelp.assign(ncnr,0);

			   PGilb=KFStrok; // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
//               p=KFStrok;     // KHigS - параметр фильтрации

			   MGilb=nrows;   // размер массива преобразования ГИЛБЕРТА
			  // KHigS=PGilb*4+8;  // KHigS - параметр фильтрации

			  // нормировка импульсной характеристики фильтра, т.е. импульсные
			  // характеристики фильтров разных порядков должны иметь одинаковые амплитуды.
			  s=sqrt((float)100*KHigS) ;
			  for ( y=0; y < nrows; y++ ){
				if( HStr[y]<4 ){ BufS[y]=0; } // Суммы меньше 8 пикселов считаются равными нолю.
				// <8 (4) пикселей - строки определяются начиная с 2 (1) букв
				else { BufS[y]=HStr[y]*s/7; } // ВАЖНО - сумма вдоль текста HStr[y] домножена на 4  (BufS[y]=4*HStr[y]*s/28;)
				  } // for y                  // BufS[y]=24*sqrt(HStr[y]*s/28);

			  // адаптивная фильтрация  H[y]
			  SP->filterHigh(BufS,KHigS);

			  // преобразование ГИЛБЕРТА 1 - BufT
			  filterGilbert(BufS,BufT, MGilb,PGilb);

#ifdef DEBUGLVL
if ( GrafT[1][5] ) {LDraw(BufT,1200,0x0000AA00);} // вызов функции рисования зеленый
#endif


			  // преобразование ГИЛБЕРТА 2 - BufR сигнал без постоянной составляющей
			  filterGilbert(BufT,BufR, MGilb,PGilb);

//// PolarCoordinates( BufT, BufR, 50000, 500, 200, 16, 0x0000AA00);

			   // увеличение градиента преобразования ГИЛБЕРТА
			   // в окрестности максимума исходной кривой H[y]
			   for (  y=6; y < nrows-6; y++ ){
						x=BufT[y]*BufS[y];
						if(x>=0) BufT[y] = sqrt((float)x);
						else     BufT[y] =-sqrt((float)-x);
				  }// for y
#ifdef DEBUGLVL
if ( GrafT[1][5] ) { LDraw(BufT,1100,0x0000AA00); } // вызов функции рисования зеленый
#endif

		// предворительное вычисление трех направляющих строки.Вычисление максимальнй
		// амплитуды (8 пиксемей от края печа не используются)
		d=0; NStrok=0; // GAmp=0;
		for ( y=8; y < nrows-8; y++ ){
		   // вычисление координат середины строки SAmp по максимому энергии
		   if( BufT[y]<=0 && BufT[y+1]>0 &&
			   BufT[y-2]< BufT[y]   && BufT[y+1]< BufT[y+3] &&
			   BufT[y-4]< BufT[y-2] && BufT[y+3]< BufT[y+5]
			 )
			   {  NStrok++;  /// BufHelp[y]=800;
//                 GAmp=BufR[y]/2; SAmp=BufS[y];
				 // вычисление 2 координат верха и низа строки по уровню GAmp=BufR[y]/2;
				 for ( p=y-4; BufR[p]<BufR[y]/2 && p>8 && p < nrows-8; p-- ){ }
					 HBuf[d]=0;        HBuf[d+1]=p; d+=2;   /// BufHelp[p]+=400;
				 for ( p=y+4; BufR[p]<BufR[y]/2 && p>8 && p < nrows-8; p++ ){ }
					 HBuf[d]=ncolumns; HBuf[d+1]=p; d+=2;   /// BufHelp[p]+=300;
				} // BufHelp[y]=400;
		}// for y
		// NStrok вычисленное колличество строк в печа

DT(" NStrok="<<NStrok);DM(END);

/*        // смещение направляющих строк в вверх и в низ на долю от ширины строки
               for(y=0; y < NStrok*4 && y<nrows; y+=4){
                   // смещение направляющий строки в вверх на одну пятую
                   HBuf[y+3]=HBuf[y+3]+(HBuf[y+3]-HBuf[y+1])/5;
                   // смещение направляющий строки в низ на одну треть
                   HBuf[y+1]=HBuf[y+1]-(HBuf[y+3]-HBuf[y+1])/5;
               }// for y
*/

       // Заполнение структур координатами строк
       // OCRclases.h  // ImageProcessor.cpp
	   strArray->resize(NStrok);   d=0;
       if(NStrok){
         for ( y=0; y < NStrok*4 && y<nrows-4; y+=4 ){
                 // x0,y0,x1,y1 координаты строк
				 strArray[0][d].x0=HBuf[y];   strArray[0][d].y0=HBuf[y+1];
				 strArray[0][d].x1=HBuf[y+2]; strArray[0][d].y1=HBuf[y+3];
				 strArray[0][d].stringFlag=1; // флаг cтроки  =1 (=0), cтрока состоит из букв (линии).
				 strArray[0][d].NLetter=0;    // примерная длинна строки (оценка)
				 d++;
				 // DM( " x0="<<wt.x0<< " y0="<<wt.y0<< " x1="<<wt.x1<< "
                 // y1="<<wt.y1<< "\n---\n");
         }// for y
       }
/* */

// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
//cout<<" @@@@ NStrok="<<NStrok<<" strArray[0][ns].y0="<<strArray[0][0].y0<<" strArray[0][ns].y1="<<strArray[0][0].y1<<endl;

	

/// Дополнение структуры координатами середины между строк и производными от них
    int ns, LimY0, LimY1;
//  strArray->resize(NStrok);
    if( NStrok >=1 ){

         for ( d=0; d < NStrok && d<nrows-4; d++ ){
                 // h высота строки
                 strArray[0][d].h=(strArray[0][d].y1-strArray[0][d].y0);
                 // vLim минимальная длинна вектора
                 strArray[0][d].vLim=strArray[0][d].h*2; // *3/2 сами вектора домножены на *10
         }// for d

         for ( d=0; d < NStrok-1 && d<nrows-4; d++ ){ //
                 // LimY0  верхний лимит строки
                 strArray[0][d].LimY0=(strArray[0][d].y0 + strArray[0][d].y1
                    + strArray[0][d+1].y0 + strArray[0][d+1].y1)/4;
                 // LimY1  нижний лимит строки
                 strArray[0][d+1].LimY1=strArray[0][d].LimY0;
         }// for d
/**/
   } // if(NStrok)

   // отработка самой верхней и самой нижней строки
   // для получения лимитов строк (середины между строками)
   if( NStrok >=2 ){
        ns=NStrok-1;
          // самый нижний лимит
          LimY1=strArray[0][0].y0  + strArray[0][0].y1  - strArray[0][1].LimY1;
          // самый верхний лимит
          LimY0=strArray[0][ns].y1 + strArray[0][ns].y0 - strArray[0][ns-1].LimY0;
          // проверка выхода за граници изображения
          if ( LimY1 < 2 ) LimY1=1;      if ( LimY0 > nrows-2 ) LimY0=nrows-1;
          strArray[0][0].LimY1=LimY1;    strArray[0][ns].LimY0=LimY0;

        /// ТАБЛИЦА
        // DM(END); DM(strArray[0][d].y0<<"\t"); if (y%10==0)DM(END); DM(END);
   }
	
   if( NStrok ==1 ){
	   LimY0=strArray[0][0].y0 - (strArray[0][0].y0 - strArray[0][0].y1)*2;
	   // проверка выхода за граници изображения
	   if ( LimY0<0 ) LimY0=0;
	   strArray[0][0].LimY0=LimY0;
	   
   }
/**/



  // ВИЗУАЛИЗАЦИЯ лимитов строк (черная)
/*  int delta1=strArray[0][0].LimY1;     int delta0=strArray[0][ns].LimY0;
  for(x=0; x < ncolumns-1; x++) {
  for (y=delta1; y<12+delta1; y++)drawData[0][y][x]*=0.1; // 0.8*ncolumns;
  for (y=delta0; y<6 +delta0; y++)drawData[0][y][x]*=0.1; // 0.8*ncolumns;
  }
*/


#ifdef DEBUGLVL
if ( GrafT[1][5] ) {
/*					  // ВИЗУАЛИЗАЦИЯ цифровая
		DM(END);  DM(nrows<<" nrows "); DM(END); DM(END);
  DM(KFStrok<<" KFStrok "); DM(KHigS<<" KHigS "); DM(NStrok<<" NStrok ");
							 DM(END); DM(END);
*/

/*			// ВИЗУАЛИЗАЦИЯ цифровая, координаты строк, медленный вывод
			  DM(END);
			  for ( y=0; y < NStrok*4;  y++ ){   // ТАБЛИЦА
				DM(HBuf[y]<<"\t"); if ((y+1)%10==0)DM(END);
				  }//for y
			  DM(END); DM(END);
*/

					 // ВИЗУАЛИЗАЦИЯ аналоговая
LDraw(BufS,1350,0x000000FF);       // вызов функции рисования красный
LDraw(BufR,1200,0x000000FF);       // вызов функции рисования красный
//LDraw(BufHelp,1100,0x000000FF);  // вызов функции рисования красный
LineVertical(1200, 0x0000AA00);    // зеленый
//sm =(KHigS*8+1250);
//LDraw(BufR,sm,0x00AA0011);     // вызов функции рисования темно синий
/* */
/*
                      /// ТАБЛИЦА лимитов строк
            DM(END);
            for ( y=0; y < NStrok; y++  ){
DM(strArray[0][y].LimY0<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
              }//for y
            DM(END);

                      /// ТАБЛИЦА лимитов строк
            DM(END);
            for ( y=0; y < NStrok; y++  ){
DM(strArray[0][y].LimY1<<"\t"); if (y%10==0)DM(END);
              }//for y
            DM(END);
*/

					  // ВИЗУАЛИЗАЦИЯ аналоговая
// получение координат верхнего LimY0 и нижнего LimY1 лимита строки из структуры
/* for (int m=0; m <NStrok; m++ ){   //  m+=2
   for( int x =0; x <ncolumns; x++ ){
	  drawData[0][strArray[0][m].LimY0][x]*=0.1;
	  drawData[0][strArray[0][m].LimY0+1][x]*=0.1;
	  drawData[0][strArray[0][m].LimY1][x]*=0.1;
	  drawData[0][strArray[0][m].LimY1+1][x]*=0.1;
   }
  ////  DM(strArray[0][m].LimY1<<"LimY1  ");  DM(strArray[0][m].LimY0<<"LimY0  ");
 } // m
 ////   DM(END);
 */

} // if ( GrafT[1][5] )


 // Визуализация аналоговая  прямоугольников в структурах //
if ( GrafT[1][6] ) { stringDraw(0x0000AA00); } // зеленый
#endif


/**/
 //strArray[0][ns].LimY0 = 1000;
 //strArray[0][0].LimY1=2000;
}//_____________________________________________________________________________

/// normalisation the strArray   //In GBitset y coordinate start from bottom of image  :)
void GBitset::stringNormalisation(){
	int print=0;
	int iTop=0,iLeft=0; //for page layout coordinate
    
	for (int index=0;index<strArray[0].size(); index++){
		DR("//1_strArray[0]["<<index<<"].LimY0="<<strArray[0][index].LimY0-border<<" LimY1="<<strArray[0][index].LimY1-border<<" border="<<border
           <<" y0="<<strArray[0][index].y0-border<<" y1="<<strArray[0][index].y1-border<<END);
			int y0=nrows-strArray[0][index].LimY0-border;
			int y1=nrows-strArray[0][index].LimY1-border;
			DR("//1_LimY0="<<y0<<" LimY1="<<y1<<" nrows="<<nrows<<" border="<<border<<END);
			if(y0<0||y0>nrows)y0=0;
				if(y1<0||y1>nrows)y1=nrows;
					strArray[0][index].LimY0=y0;
					strArray[0][index].LimY1=y1;
					
					y0=nrows-strArray[0][index].y1-border;
					y1=nrows-strArray[0][index].y0-border;
					if(y0<0||y0>nrows)y0=0; if(y1<0||y1>nrows)y1=nrows;
						strArray[0][index].y0=y0;
						strArray[0][index].y1=y1;
						strArray[0][index].selectFlag=0;
						
						/*if(draw){
							//DT("//1_y0="<<y0<<" y1="<<y1<<" nrows="<<nrows<<" border="<<border<<END);
							cout<<"sttIn="<<index<<" y0_="<<inputData.y0<<" y1_="<<inputData.y1;
							DT(" LimY0="<<strArray[0][index].LimY0<<
							   " LimY1="<<strArray[0][index].LimY1<<" y0="<< strArray[0][index].y0<<" y1="<<strArray[0][index].y1<<END);
							if(strArray[0][index].LimY0<inputData.y0&&strArray[0][index].LimY1>inputData.y1){
								strArray[0][index].selectFlag=3;
								cout<<"find string "<<index<<END;
							}
							}
						*/
						 DR("//2_Y0="<<y0<<" Y1="<<y1<<" nrows="<<nrows<<" border="<<border<<END);
							
							for (int i=0;i<strArray[0][index].wordArray.size(); i++){  //word by word
								//if(strArray[0][index].wordArray[i].w){letterW+=strArray[0][index].wordArray[i].w;
								//	letterH+=strArray[0][index].wordArray[i].h;
								//	baseCount++;
								//}
								//if(mode==ALL_LETTER){
								    strArray[0][index].wordArray[i].y0=nrows-strArray[0][index].wordArray[i].y0+iTop-border;   
									strArray[0][index].wordArray[i].y1=nrows-strArray[0][index].wordArray[i].y1+iTop-border;
									int y0_=strArray[0][index].wordArray[i].y1;
								    strArray[0][index].wordArray[i].y1=strArray[0][index].wordArray[i].y0;
								    strArray[0][index].wordArray[i].y0=y0_;   //y0=y1 (important)
									strArray[0][index].wordArray[i].center.y=nrows-strArray[0][index].wordArray[i].center.y+iTop-border;
									strArray[0][index].wordArray[i].h=strArray[0][index].wordArray[i].y0-strArray[0][index].wordArray[i].y1;
									strArray[0][index].wordArray[i].x0=strArray[0][index].wordArray[i].x0+iLeft-border;
									strArray[0][index].wordArray[i].x1=strArray[0][index].wordArray[i].x1+iLeft-border;
									strArray[0][index].wordArray[i].w=strArray[0][index].wordArray[i].x1-strArray[0][index].wordArray[i].x0;
									strArray[0][index].wordArray[i].center.x=strArray[0][index].wordArray[i].center.x+iLeft-border;
									strArray[0][index].wordArray[i].blockIndex=-1;
									strArray[0][index].wordArray[i].yp0=nrows-strArray[0][index].wordArray[i].yp0+iTop-border;
									strArray[0][index].wordArray[i].yp1=nrows-strArray[0][index].wordArray[i].yp1+iTop-border;
									strArray[0][index].wordArray[i].xp0=strArray[0][index].wordArray[i].xp0+iLeft-border;
									strArray[0][index].wordArray[i].xp1=strArray[0][index].wordArray[i].xp1+iLeft-border;
									strArray[0][index].wordArray[i].name="*";
									strArray[0][index].wordArray[i].id=-1;
									
									
									/*if(draw){
										//cout<<"sttIn="<<i<<" y0="<<inData->y0<<" y1="<<inData->y1<<" x1="<<inData->x1<<" y1="<<inData->y1<<" c.x="<<strArray[0][index].wordArray[i].center.x<<" c.y="<<strArray[0][index].wordArray[i].center.y<<END;
										if(strArray[0][index].wordArray[i].center.x>inputData.x0&&
										   strArray[0][index].wordArray[i].center.x<inputData.x1&&
										   strArray[0][index].wordArray[i].center.y>inputData.y0&&
										   strArray[0][index].wordArray[i].center.y<inputData.y1){
											strArray[0][index].wordArray[i].selectFlag=3;
										}
										
									}
									*/
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].x0="<<strArray[0][index].wordArray[i].x0<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].y0="<<strArray[0][index].wordArray[i].y0<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.x="<<strArray[0][index].wordArray[i].center.x<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.y="<<strArray[0][index].wordArray[i].center.y<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].x0="<<strArray[0][index].wordArray[i].xp0<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].y0="<<strArray[0][index].wordArray[i].yp0<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.x="<<strArray[0][index].wordArray[i].center.x<<END);
									//DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.y="<<strArray[0][index].wordArray[i].center.y<<END);
								//}
								
								
							//}
							}
							
							
							//DT("//2_strArray[0]["<<index<<"].LimY0="<<strArray[0][index].LimY0<<" LimY1="<<strArray[0][index].LimY1<<END);
							
			}
    
    // Алгоритм обработки высоты строк
    
    // Задача:
    // разбиение строк текста на группы с одинаковой высотой и усреднение высот строк внутри группы.
    // Можно применять итерационно, несколько раз.
    
    int DeltaHStr=2;	// допуск высоты строки м.б. задан в pix или в %
    int NStrok=(int)strArray[0].size(); 		// количество строк в печа
    int m,n;		// переменные
    int p;			// переменная количества строк в группе
    int HStr; 		// переменная высоты строки
    
    // массив исходной высоты строк.
    vector<int>HStrIn; HStrIn.resize(strArray[0].size());
    //заполняем исходный массив
    for ( n=0; n < NStrok; n++ ){
        HStrIn[n]=strArray[0][n].y1-strArray[0][n].y0;  //cout<<HStrIn[n]<<" ";
    }
    //int HStrIn[16] = {0};
    // выходной массив усредненной высоты строк.
    vector<int>HStrOut;   HStrOut.resize(strArray[0].size());

    
    // проверка допуска высоты строки
    if(DeltaHStr<1) DeltaHStr=0;  if(DeltaHStr>16) DeltaHStr=16;
    
    // В двойном цикле сравниваем высоту каждой строки с высотой каждой строки в пределах
    // допуска и усредняем высоты тех строк, которые попали в допуск (каждая строка формирует
    // свою личную группу). Полученное среднее каждой строки - это и есть новая усредненная
    // высота строки.
    
    // цикл перебора строк
    //p=1;
    for ( n=0; n < NStrok; n++ ){
        HStr=0; p=0;
        // цикл сборки групп строк
        for ( m=0; m < NStrok; m++ ){
            // проверка допуска высоты строки
            if((HStrIn[m]<=HStrIn[n]+DeltaHStr) && (HStrIn[m]>=HStrIn[n]-DeltaHStr))
                // подсчет количества строк попавших в допуск
            {HStr=HStr+HStrIn[m];  p++;}
        } // for m
        // усреднение высоты строки
        if(p!=0) HStrOut[n]=HStr/p;
		///cout<<p<<" ";
    } // for n
    ///cout<<endl;
    
    // в выходном массив HStrOut[n] записана новая усредненная высота строки.    
    // записываем значение высоты строки в strArray
        for ( n=0; n < NStrok; n++ ){
           strArray[0][n].y1=strArray[0][n].y0+HStrOut[n];  //cout<<HStrOut[n]<<" ";
        }
        //exit(0);
}//_____________________________________________________________________________



////////////////////////////////////////////////////////////////////////////////

 //************ ПОДГОТОВКА нахождение середины между строками ****************

 // int NStr=strArray->size(); // вычисленное колличество строк в печа NStrok
 // int yMxMn=(yMax+yMin)/2;   // адрес центра масс контура по координате Y

 //***************************************************************************


/*
#ifdef DEBUGLVL
   // ВИЗУАЛИЗАЦИЯ аналоговая горизонтальных сумм (черная)
   int M=16; static short delta=50;
   for(y=0; y < drawData->rows(); y++) {
    if(HStr[y]>=0){ for (x=delta; x<(HStr[y]/M)+delta; x++)drawData[0][y][x]*=0.1; }
    else          { for (x=delta; x>(HStr[y]/M)+delta; x--)drawData[0][y][x]*=0.1; }
   } // y++  // 0.8*ncolumns;
#endif
*/

/*
                      // преобразование ГИЛБЕРТА 1
               for (  y=32; y < nrows-32; y++ ){
               BufT[y]=(
                       + (BufS[y-31]-BufS[y+31])/31 + (BufS[y-30]-BufS[y+30])/30
                       + (BufS[y-29]-BufS[y+29])/29 + (BufS[y-28]-BufS[y+28])/28
                       + (BufS[y-27]-BufS[y+27])/27 + (BufS[y-26]-BufS[y+26])/26
                       + (BufS[y-25]-BufS[y+25])/25 + (BufS[y-24]-BufS[y+24])/24
                       + (BufS[y-23]-BufS[y+23])/23 + (BufS[y-22]-BufS[y+22])/22
                       + (BufS[y-21]-BufS[y+21])/21 + (BufS[y-20]-BufS[y+20])/20
                       + (BufS[y-19]-BufS[y+19])/19 + (BufS[y-18]-BufS[y+18])/18
                       + (BufS[y-17]-BufS[y+17])/17 + (BufS[y-16]-BufS[y+16])/16
                       + (BufS[y-15]-BufS[y+15])/15 + (BufS[y-14]-BufS[y+14])/14
                       + (BufS[y-13]-BufS[y+13])/13 + (BufS[y-12]-BufS[y+12])/12
                       + (BufS[y-11]-BufS[y+11])/11 + (BufS[y-10]-BufS[y+10])/10
                       + (BufS[y-9]-BufS[y+9])/9 + (BufS[y-8]-BufS[y+8])/8
                       + (BufS[y-7]-BufS[y+7])/7 + (BufS[y-6]-BufS[y+6])/6
                       + (BufS[y-5]-BufS[y+5])/5 + (BufS[y-4]-BufS[y+4])/4
                       + (BufS[y-3]-BufS[y+3])/3 + (BufS[y-2]-BufS[y+2])/2
                       +  BufS[y-1]-BufS[y+1] )/2;
                    }// for y

               // сглаживание ФИЛЬТР НИЗКИХ ЧАСТОТ k=8
 ///              SP->filterHigh(BufT,8);

           LDraw(BufT,200,0x0000AA00);  // вызов функции рисования зеленый

                       // преобразование ГИЛБЕРТА 2
               for (  y=32; y < nrows-32; y++ ){
               BufR[y]=(
                       + (BufT[y-31]-BufT[y+31])/31 + (BufT[y-30]-BufT[y+30])/30
                       + (BufT[y-29]-BufT[y+29])/29 + (BufT[y-28]-BufT[y+28])/28
                       + (BufT[y-27]-BufT[y+27])/27 + (BufT[y-26]-BufT[y+26])/26
                       + (BufT[y-25]-BufT[y+25])/25 + (BufT[y-24]-BufT[y+24])/24
                       + (BufT[y-23]-BufT[y+23])/23 + (BufT[y-22]-BufT[y+22])/22
                       + (BufT[y-21]-BufT[y+21])/21 + (BufT[y-20]-BufT[y+20])/20
                       + (BufT[y-19]-BufT[y+19])/19 + (BufT[y-18]-BufT[y+18])/18
                       + (BufT[y-17]-BufT[y+17])/17 + (BufT[y-16]-BufT[y+16])/16
                       + (BufT[y-15]-BufT[y+15])/15 + (BufT[y-14]-BufT[y+14])/14
                       + (BufT[y-13]-BufT[y+13])/13 + (BufT[y-12]-BufT[y+12])/12
                       + (BufT[y-11]-BufT[y+11])/11 + (BufT[y-10]-BufT[y+10])/10
                       + (BufT[y-9]-BufT[y+9])/9 + (BufT[y-8]-BufT[y+8])/8
                       + (BufT[y-7]-BufT[y+7])/7 + (BufT[y-6]-BufT[y+6])/6
                       + (BufT[y-5]-BufT[y+5])/5 + (BufT[y-4]-BufT[y+4])/4
                       + (BufT[y-3]-BufT[y+3])/3 + (BufT[y-2]-BufT[y+2])/2
                       +  BufT[y-1]-BufT[y+1] )/2;
        ///        BufR[y]=BufR[y]*BufR[y]/128;
                    }// for y
 */


 /*     // int NAverage; стартовое значение величины усреднения 1 <NAverage< 3
      // получается в результате работы программы определния угла поворота печа

                      p=*NAverage;
                      KHig=8; // KHig; параметр фильтрации
      if( p==1 ) KHig=8;  if( p==2 ) KHig=12;  if( p==3 ) KHig=16;
                      Pv1=KHig;
      // запуск подпрограммы предворительного выделения строк
                RepeadStringDetector(HStr,HBuf, &Pv1,&Pv2);

*/

/*
             sm=20;
              //  сильная фильтрация (32) исходных горизонтальных сумм
              SP->filterHigh(HBuf,64);

#ifdef DEBUGLVL
LDraw(HBuf,sm,0x0000AA00);  // вызов функции рисования зеленый
#endif

              // вычисление максимума maxF сильно фильтрованных исходных сумм
              maxF=0; DLt=nrows/32; int HBf;
              for ( y=DLt; y < nrows-DLt; y=y+8 ) {
                HBf=HBuf[y];
                if( HBf > maxF )
                  { maxF= HBf; } // maxF - max HBuf[y]
              } // for y

              // бинариэация сильно фильтрованных исходных  сумм
              // порог надо брать из общей сегментации страници
              maxP=maxF/2;  //Porog=maxP*ncolumns/nrows*512/nrows;
              // Porog=maxP*700/nrows;
              if   ( Porog > 165 )   // 165, 190
              {
                for ( y=0; y < nrows; y++ ) {
                  HBf=HBuf[y] ;
                  if   ( HBf > maxP  )  { HBf = 0; }
                  else { HBf = maxF; }
                  HStr[y]=HStr[y] + HBf;
                } // for y
              }

#ifdef DEBUGLVL
   //           ShowMessage(Porog);
              DM(END); DM(maxF<<" maxF "); DM(END); DM(END);
LineVertical(maxF/64+sm, 0x00AA0011);  // ТЕМНО СИНИЙ
//LDraw(HBf,sm,0x000000FF);           // вызов функции рисования красный
#endif
 */

