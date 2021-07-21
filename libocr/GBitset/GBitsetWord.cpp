#include "GBitset.h"
                             // Block//
// ---------------------------------------------------------------------------//
// Подпрограмма получения координат блоков букв в пределах страници (квадратики).
// ---------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

// Запуск обработки всех строк страници.
void GBitset::PageWordDet() {

  int y,x,m; // ,n
  int y0,y1,s0,s1;
  int res,resD;          // unsigned long; // short  // 2147483647
  MeanSA=0;
  //int Lim=2147483647>>8-2;    // DM(Lim<<" Lim ");

TIME_START
     // вычисленное колличество блоков букв во всем печа
                       NBlockP=0;

   // номер строки на графику .
   // m=StrToInt(ImageProcessorMain->Edit5->Text);  //
   //m=36;
   m=2;
   if ( m > NStrok ) { m = NStrok; } // NStrok начинается с 1
   Nstr=m;

///////////////
/*
   // Цикл по колличеству строк NStrok
   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok - глобальные /////////
             // получение координат строк y0, y1 из структуры
             y0=strArray[0][Nstr].y0;   y1=strArray[0][Nstr].y1;
             // DM(END); DM(Nstr<<" Nstr "); DM(y0<<" y0 "); DM(y1<<" y1 "); DM(END);
             // подсчет вертикальных сумм в пределах строки
             for ( x=0; x < ncolumns-1; x++ ) {  // read image make sum    // ncolumns-1
                   res=0;
                   // смещение границ полосы подсчета вертикальных сумм вверх и в низ
                   s0=y0-(y1-y0)/21;   if ( s0 < 2 )        s0 = 1;  // *2/3;
                   s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
                   for ( y=s0; y < s1; y++ ) {
                        p=y*ncolumns;  res+=bits_data[p + x];
                       }  // for y
//                 if ( res > Lim )  res = Lim;
                   LStr[x]=res<<8; // ВАЖНО - сумма вдоль строк домножена на *256
                 }  //  for x
// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
             LineBlockStart();
       }// for Nstr // Цикл по колличеству строк NStrok ////////////////////////

*/
//////////////
  int w=ncolumns,dw;
  bool d,*d1;  // ,*d2
  bool  *A=bits_data;
//  MeanSA=DeltaNstrKH*6/5;
//  dw=w*MeanSA/8;  if ( dw < 1 )  dw=1;


////

   // в LStr кладем вертикальные суммы по каждой строке
   // в LStS кладем вертикальные суммы по всей странице в BlockSum()
   // в BufL кладем вертикальные дифференциальные суммы по каждой строке

   Nstr=2;

   // Цикл по колличеству строк NStrok
   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok - глобальные /////////
			 // получение координат строк y0, y1 из структуры
			 y0=strArray[0][Nstr].y0;   y1=strArray[0][Nstr].y1;
			 // вычисленная ширина буквы DeltaNstrKH
			 DeltaNstr = strArray[0][Nstr].y1-strArray[0][Nstr].y0;
			 dw=w+w; // dw=w*(DeltaNstr/8+1); // DM(DeltaNstr/8+1<<" DeltaNstr/8+1 ");
			 // DM(END); DM(Nstr<<" Nstr "); DM(y0<<" y0 "); DM(y1<<" y1 "); DM(END);
			 // смещение границ полосы подсчета вертикальных сумм вверх и в низ
             s0=y0-(y1-y0)/3;    if ( s0 < 2 )        s0 = 1; //  /21 <<8; 1/3 <<9;  *2/3;
             s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
             // подсчет вертикальных сумм в пределах строки, вертикальный фильтр
             for (x=0; x<ncolumns; x++){
				 res=resD=0; // old=A+s0*w+x; // стартовое значение  old
				 for (y=s0; y<s1; y++){
					 d1=A+y*w+x;      //A[y][x]         //      -1
				   //d2=A+y*w+x+w;    //A[y+1][x]       //       1     d2=d1+dw;
					 // подсчет вертикальных сумм
					 d=*d1;  res+=d;
					 // подсчет вертикальных дифференциальных сумм (не быстрее)
//					 d=*d1-*old;    old=d1;    resD+=d;
					 // подсчет вертикальных дифференциальных сумм без old с dw=w+w;
					 d=*d1-*(d1+dw); resD+=d;

  // способы получения  d
  //                    d=*d1;
  ////			   	    d=*(A+y*w+x);   // d для просто сумм без old
  ///                   d=*d1-*(d1+dw); // d для дифференциальных сумм через dw пикселей
// визуализация частей букв в окошке в виде граници в один пиксел
#ifdef DEBUGLVL_GREY
if ( GrafT[2][1] ) { drawData[0][y][x]=255+d*196; }  // draw it   *(d+x)
#endif
//drawData[0][y][x]=255+d*196;
				 }  // for y
				 LStr[x]=res<<8;    // res<<8  *256
				 BufL[x]=resD<<9;
			  }  // for x
// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
			 LineBlockStart();
	}// for Nstr // Цикл по колличеству строк NStrok ////////////////////////

////
/* */  
    
        // средний по всем строкам страници страници период букв MeanCol

        NStrok=(int)strArray->size();
        if ( NStrok > 0 ) { MeanCol=MeanCol/NStrok; }
        else  MeanCol=1;
        //DM(MeanCol<<" MeanCol ");

#ifdef DEBUGLVL_GREY
if( GrafT[2][1] ) {
/*          // ВИЗУАЛИЗАЦИЯ цифровая, координаты строк в структурах
              DM(END);
              for (int t=0; t<strArray.size(); t++ ){ // t<NStrok;
                  DM(
                  " x0="<<strArray[0][t].x0<<
                  " y0="<<strArray[0][t].y0<<
                  " x1="<<strArray[0][t].x1<<
                  " y1="<<strArray[0][t].y1<<
                  "\n\n");
              }// for t
*/

  // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
  int M=64; static short delta=nrows*10/40; // 300
  for(x=0; x < ncolumns-1; x++) {
  for (y=delta; y<(LStr[x]/M)+delta; y++)drawData[0][y][x]*=0.1;}// 0.8*ncolumns;

              // вызов горизонтальной функции рисования

//            WDraw(LStr,20,0x00AA0011); // темно синий
/**/
}
#endif

if( GrafT[2][0] ) { DM(END);  DM(22222<<" PageWordDet ");  DM(END); }

// Визуализация прямоугольников в структурах для блоков букв
///if( GrafT[2][6] ) { blockDraw(0x000000FF); }  // цвет линии красный

// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
//if( GrafT[2][8] ) { lineDraw(0x00ff00); }  // цвет линии зеленый

TIME_PRINT

//LStr.assign(ncolumns,0);
//if ( res > Lim )  res = Lim;
}//_____________________________________________________________________________


//                            ЗАПУСК Block

// Запуск программы получения координат блоков букв в пределах строки, параметр MeanSA.
// -----------------------------------------------------------------------------
void GBitset::LineBlockStart() {


             // вычисленная ширина буквы DeltaNstrKH с учетом коэффициентов
             // масштаба размера печа, KH=srcH/nrows;  KW=srcW/ncolumns;
             // y0, y1 - получены из структуры.
             DeltaNstr = strArray[0][Nstr].y1-strArray[0][Nstr].y0;
             DeltaNstrKH = DeltaNstr*ncolumns/srcW*srcH/nrows;

             // стартовое (по размеру строк) значение MeanSA.
             MeanSA=DeltaNstrKH*6/5;
             // DeltaNstr, DeltaNstrKH , MeanSA- глобальные.

/*
#ifdef DEBUGLVL
#ifdef	DRAW1
              // ввод козффициента для сглаживающего фильтра с экрана
              int Dm=atoi(ImageProcessorMain->Edit4->Text.c_str());
              if(Dm!=MeanSA){
                  if(Dm>0){MeanSA=Dm;};
                  ImageProcessorMain->Edit4->Text=IntToStr(MeanSA);
              }
#endif
#endif
*/

#ifdef DEBUGLVL
if( GrafT[2][3] ) {
/// DM(" srcW="<<srcW<<" srcH="<<srcH<<" nrows="<<nrows<<" ncolumns="<<ncolumns<<END);
}
if( GrafT[2][5] ) { DM(END); DM(" MeanSA стартовое значение "<<MeanSA); }
#endif


             // запуск подпрограмм получения координат букв в пределах
             // строки с стартовами, по размеру строк значениям MeanSA.

bit_vector  AMask(ncolumns);

// Подпрограмма обработки статистики блоков букв короткими импульсами.
                 ShortBurstNpBl(AMask);

// Нелинейная обработка статистики блоков букв в пределах строки, система масок.
                 SystemMasksNpBl(AMask);

// Получение среднего периода блоков букв в пределах строки. Поиск триадами.
                 PeriodBlock();
                 //// получение среднего периода букв MeanSA ////


// Подпрограмма для получения координат блоков букв в пределах строки.
                 BlockDetector();
                 // получили координаты блоков букв в структуре. Флаги

// Получение вертикальных сумм блоков букв по всей высоте страницы.
                 BlockSum();
                 // получили вертикальные суммы блоков букв.

// Подпрограмма для получения координат блоков слов в пределах колоноки.
////             LineDetector(HBuf,GBuf);
                 // получили координаты блоков слов в структуре. Флаги

}//_____________________________________________________________________________



//              Получение коротких импульсов Block.   I  I I  I  I
//
// Нелинейная обработка статистики букв короткими импульсами в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::ShortBurstNpBl(bit_vector  &AMask) {
  int y,p;


// получение битовой маски для умножнния вертикальных сумм букв на короткие импульсы
// NonlinearProcessing  нелинейная обработка,   short burst короткий импульс


// ВЫЧИСЛЕНИЕ ПРЕДВОРИТЕЛЬНОГО козффициента для сглаживающего фильтра SP->filterHigh
              // MeanSA=DeltaNstrKH*6/5;
			  KHig=MeanSA/3; // MeanSA/4;
			  if ( KHig>=31 ) KHig=31;
              KLow=KHig*2;  // KHig, KLow, MeanSA- глобальные.

              // выделение промежутков между буквами
              // ассиметричное преобразование ГИЛБЕРТА
              for ( y=4; y < ncolumns-4; y++ ){
                  HBuf[y]=abs( ( LStr[y-3]-LStr[y+2] )/3 +
                               ( LStr[y-2]-LStr[y+1] )/2 +
                                 LStr[y-1]-LStr[y]
                             );
			  }// for y

			  // сглаживание, полосовой фильтр
			  int KHg=(KHig+1)/2;
			  SP->filterLaplasian(HBuf,KHg,KHg*3);

              // преобразование ГИЛБЕРТА - HBuf
			  // KHig/16=PGilb - параметр (ширина) преобразования ГИЛБЕРТА 0,1,2,...
              // ncolumns=MGilb - размер массива преобразования ГИЛБЕРТА
			  filterGilbert(HBuf,GBuf, ncolumns,KHig/16);


        // Вычисление максимальнй амплитуды огибающей промежутков между букв.
        int GAmp;
        for ( y=8; y < ncolumns-8; y++ ){ // (8 пиксемей от края печа не используются)
           // вычисление координат середины GBuf по огибающей
            if( GBuf[y]<=0 && GBuf[y+1]>0 )
                // вычисление двух координат левого и правого края короткого импульса по GAmp
                {      GAmp=HBuf[y]*3/4;
                     for ( p=y;  HBuf[p]>GAmp && p>8;          p-- )
                     { AMask[p]=1; }
//                     LineVertical(p, 0x0000AA00);  // зеленый
                     for ( p=y;  HBuf[p]>GAmp && p<ncolumns-8; p++ )
                     { AMask[p]=1; }
//                     LineVertical(p, 0x0000AA00);  // зеленый
#ifdef DEBUGLVL
////if( GrafT[2][3] ) { LineVertical(y, 0x000000FF);}  // красный  ///
#endif
                } // GAmp=
        }// for y
/**/

// получили битовую маску для умножнния вертикальных сумм букв на короткие импульсы


#ifdef DEBUGLVL
if( GrafT[2][3] ) {
int SmGr=nrows/4;
                     // ВИЗУАЛИЗАЦИЯ цифровая
DM(END);  DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows ");  DM(END);

                    // ВИЗУАЛИЗАЦИЯ аналоговая переменной длинны
WDrawVar(HBuf,ncolumns,SmGr,32,0x00AA0011);  // вызов функции рисования ТЕМНО СИНИЙ
WDrawVar(GBuf,ncolumns,SmGr,32,0x0000AA00);  // вызов функции рисования зеленый
//                  LineVertical(p, 0x00AA0011);  // темно синий

#ifdef DEBUGLVL_GREY
 // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
 int buf, M=64; static short delta=SmGr/8; // 300
  for(int x=0; x < ncolumns; x++) {
      buf=LStr[x]*(1-AMask[x]);
      if( buf>=0 ){ for (y=delta; y<(buf/M)+delta; y++)drawData[0][y][x]*=0.1; }
      //else      { for (y=delta; y>(buf/M)+delta; y--)drawData[0][y][x]*=0.1; }

 } // x++  // 0.8*ncolumns;
#endif    
/**/
}
#endif
/**/

}//_____________________________________________________________________________


//                    Система масок Block.   00001010010001
//
// Нелинейная обработка статистики букв в пределах строки, система масок.
// -----------------------------------------------------------------------------
void GBitset::SystemMasksNpBl(bit_vector  &AMask) {

  int y,p,d,s=0;
  //int MGilb,PGilb;
  int SmGr=nrows/4;
  //vector<int> BufT(ncolumns);
  bit_vector  BMask(ncolumns);


                           HBuf=LStr;

              // фильтрация, сглаживание, полосовой фильтр
			  SP->filterLaplasian(HBuf,KHig,KLow);

if( GrafT[2][4] ){ WDrawVar(HBuf,ncolumns,SmGr*3,32, 0x00AA0011); } // вызов функции рисования ТЕМНО СИНИЙ

              // преобразование ГИЛБЕРТА
			  filterGilbert(HBuf,GBuf, ncolumns,KHig/16);

if( GrafT[2][4] ){ WDrawVar(GBuf,ncolumns,SmGr*3,32, 0x0000AA00); } // вызов функции рисования зеленый

   // Вычисление центра масс ширины центрально симметричных букв.
		d=0;  int GAmp=0, maxT=0, PGrad=0;
		PGrad=65000/(MeanSA*MeanSA+1); // градиент GBuf д.б. больше порога PGrad, 65535
        for ( y=8; y < ncolumns-8; y++ ){ // (8 пиксемей от края печа не используются)
           // вычисление координат середины GBuf по огибающей
            if( GBuf[y]<=0 && GBuf[y+1]>0 && (GBuf[y+1]-GBuf[y])>PGrad ) // 1600/(MeanSA+1)
                // вычисление координат левого и правого края центрально симметричных букв по GAmp
                {    d++; maxT=maxT+abs(HBuf[y]);// нахождение среднего abs(HBuf[y]);
                     GAmp=HBuf[y]*2/3;  // *3/4
                     for ( p=y;  HBuf[p]>GAmp && p>8;          p-- ){BMask[p]=1;}
//                     LineVertical(p, 0x0000AA00);  // зеленый
                     for ( p=y;  HBuf[p]>GAmp && p<ncolumns-8; p++ ){BMask[p]=1;}
#ifdef DEBUGLVL
if( GrafT[2][4] ) { LineVertical(y, 0x000000FF); }   // красный
#endif
//                     LineVertical(p, 0x0000AA00);  // зеленый
                } //
        }// for y

                // порог ограничения высоты букв
                maxT=maxT*4/(d+1);  // 5



#ifdef DEBUGLVL
if( GrafT[2][4] ) {
BufHelp.assign(ncnr,0);
for (  y=0; y < ncolumns; y++ ) { BufHelp[y]=8*BMask[y]; }
WDrawVar(BufHelp,ncolumns,SmGr*3,1, 0x000000FF);  // вызов функции рисования красный
// LineVertical(p, 0x00AA0011);  // темно синий
}
#endif
/**/

              // решение системы неравенств, а именно
              // отделение краев зеркально отраженных букв друг от друга,
              // защита середины центрально симметричных букв
              for (  y=0; y < ncolumns-1; y++ ){
                  if( AMask[y]==1 && BMask[y]==0 ) { s=0; }
                  else { s=LStr[y]; }
                  // выравнивание (ограничение) высоты букв
                  if (s > maxT) { s=maxT; }
                  HBuf[y]=s;
              }// for y
/**/



#ifdef DEBUGLVL
if( GrafT[2][4] ) {
/*
                     // ВИЗУАЛИЗАЦИЯ цифровая
                                DM(END);
DM(MeanSA<<" -MeanSA- "); DM(KHig<<" _KHig_ "); DM(DeltaNstr<<" DeltaNstr ");
DM(PGrad<<" PGrad "); DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows ");
                                DM(END);
*/

                    // ВИЗУАЛИЗАЦИЯ аналоговая
//WDrawVar(GBuf,ncolumns,SmGr*3,32, 0x0000AA00); // вызов функции рисования зеленый
WDrawVar(HBuf,ncolumns,SmGr/2,128,0x000000FF); // вызов функции рисования красный

/*
 // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
 int M=64; static short delta=SmGr/8; // 300
 for(x=0; x < ncolumns; x++) {
  if(LStr[x]>=0){ for (y=delta; y<(LStr[x]/M)+delta; y++)drawData[0][y][x]*=0.1; }
 } // x++       // 0.8*ncolumns;
*/
}
#endif
/**/

}//_____________________________________________________________________________

//                           Поиск триадами(ФАпЧ) ST

// Подпрограмма получения среднего периода букв в пределах строки.Поиск триадами
// -----------------------------------------------------------------------------
void GBitset::PeriodBlock() {

  int y,n,g,dt;
  int dtMemL,dtMemR,AmMAX,AdrAmMAX,LfAmSum=0,RgAmSum=0;
  float Lf,Rg,LfMem=0,RgMem=0,Delta;
  // автоматическое масштабирование размеров массивов для нахождение максимума спектра
  int d,size,M=1;
  M = (int)HBuf.size()/1250;
  if ( M<1 ) M=1;   if ( M>3 ) M=3;   //  M=2;
  size=ncolumns/M;

  // запрос памяти с очисткой 0,      szf=4;

	int *Buf1 =(int*)malloc((size+32)* szf);   // BufS
	int *Buf2 =(int*)malloc((size+32)* szf);   // BufR
	int *BufTL=(int*)malloc((size+32)* szf);
	int *BufTR=(int*)malloc((size+32)* szf);	

	
  // выход (0) в случае отсутствия свободной памяти запрашиваемого размера
    if ( !Buf1 || !Buf2 || !BufTL || !BufTR ) exit (1);

// локальный подсчет времени выполнения текущей функции PeriodBlock
#ifdef DEBUGLVL
if( GrafT[2][5] ) { DM(END); DM(" PeriodBlock start: "); DM(END); DM(END); }
static clock_t  tm_start1=clock(); clock_t  tm_end; double time;  // DM(tm<<" "<<t);
#endif



/*
// TEST гребенка
vector<int> BufQ(ncolumns);
int DnKH=25;  // 20
     for (  y=0; y < ncolumns-DnKH; y+=DnKH ){
          for (  p=0; p < DnKH/4*3; p++ ){ BufQ[p+y]=1024; }// for p   d+=2;
     }// for y
WDrawVar(BufQ,ncolumns,SmGr*2,32,0x000000FF);  // вызов функции рисования, красный
*/


// автоматическое масштабирование размеров массивов для нахождение максимума спектра
    d=2;
    if ( M<=1 ){ M=1;
      for ( y=2; y < size-2; y++ ){ Buf1[y]=HBuf[d]; d+=1; }
    }
    if ( M==2 ){ M=2;
      for ( y=2; y < size-2; y++ ){ Buf1[y]=(HBuf[d]+ HBuf[d+1])/2; d+=2; }
    }
    if ( M>=3 ){ M=3;
      for ( y=2; y < size-2; y++ ){ Buf1[y]=(HBuf[d-1]+ HBuf[d]+ HBuf[d+1])/3; d+=3; }
      }


// интерактивное нахождение максимума спектра (поиск триадами float).
// Функция состоит из: гребенчатого, полосового фильтров и амплитудного детектора.
// алгоритм находит период букв в пределах от DeltaNstrKH до 2*DeltaNstrKH

    dtMemL=0; dtMemR=0;  int N=0, NN=0;
    g=3;  // добротность гребенчатого фильтра = 3;  1 < g < 10
    Lf=DeltaNstrKH/M;   Rg=Lf+Lf;  //DM(END);
    Delta=Lf/12; // абсолютная точность итераций absolute Precision  /24;

    for ( n=0; n<10; n++ ){

         dt=LfMem=(Lf+Lf+Rg)/3;   // треть влево (Lf+Lf+Rg)/3;
         if ( dt/4 != dtMemL ) {  N++;   // DM("fL");
			 SP->filterLaplasian(Buf1, BufTL, size, dt/4, dt/2); // полосовой фильтр (dt/4)*2);
         }
             dtMemL=dt/4;  NN++;         // DM("FS");
             memcpy (Buf2, BufTL, size*szf);
             //for (x=0; x<size-1; x++){Buf2[x]=BufTL[x];}   //Buf2=BufTL;
             FunctionSpectrSt(Buf2, size, dt, LfAmSum);// гребенчатый фильтр, амплитудный детектор.

         dt=RgMem=(Lf+Rg+Rg)/3;   // треть вправо  (Lf+Rg+Rg)/3;
         if ( dt/4 != dtMemR ) {  N++;
             SP->filterLaplasian(Buf1, BufTR, size, dt/4, dt/2); // полосовой фильтр (dt/4)*2);
         }
             memcpy (Buf2, BufTR, size*szf);
             //for (x=0; x<size-1; x++){Buf2[x]=BufTR[x];}   //Buf2=BufTR;
             dtMemR=dt/4;  NN++;
             FunctionSpectrSt(Buf2, size, dt, RgAmSum);// гребенчатый фильтр, амплитудный детектор.
#ifdef DEBUGLVL
if( GrafT[2][5] ) {
DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="<<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
}
#endif
         if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
         if ( Rg-Lf<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)

   }// for n
   // filterLaplasianM при таком алгоритме запускается примерно в 2-3 раза мене
   // часто, а он ощутимо сложнее чем FunctionSpectrum
/**/

        //  максимум спектра, адрес максимума спектра
        AmMAX=(LfAmSum + RgAmSum)/2;   AdrAmMAX=M*(LfMem + RgMem)/2;


        // получение УТОЧНЕННЫХ параметров из AdrAmMAX
        // получение козффициента dt для гребенчатого фильтра
///     if ( AdrAmMAX<Lf ) AdrAmMAX=Lf;   if ( AdrAmMAX>Rg ) AdrAmMAX=Rg;
		MeanSA = AdrAmMAX;
#ifdef DEBUGLVL
if( GrafT[2][5] ) { DM(END); DM(" -MeanSA_1- "<<MeanSA); DM(END); }  /////
#endif
		// получение козффициентов KHig, KLow для полосового фильтра Gaussian
		KHig=(AdrAmMAX)/4; // ish KHigP=(MeanSA+3)/4;  // (MeanSA+0)/4;
		if ( KHig>=31 ) KHig=31;
		KLow=KHig*2;   // KHig, KLow, MeanSA - глобальные.


         // Заполнение структуры значениями MeanSA по строкам
         // MeanSA - средее, по всей строке, расстояние между центрами масс букв
         strArray[0][Nstr].MeanSA=MeanSA;   
         // средний по странице период букв

         MeanCol+=MeanSA;


//// DM("PeriodLetters_2@@")tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);

/*
                 // ПРИНЦИП РАБОТЫ поиска триадами
// это метод для поиска максимумов и минимумов функции, которая либо сначала
// строго возрастает, затем строго убывает, либо наоборот. Троичный поиск определяет,
// что минимум или максимум не может лежать либо в первой, либо в последней трети
// области, и затем повторяет поиск на оставшихся двух третях.

//  Функция поиска триада (F, влево, вправо, абсолютная точность)
      function ternarySearch(f, left, right, absolutePrecision)
// left and right are the current bounds; the maximum is between them
// слева и справа являются нынешние границы; максимум между ними
    if (right-left < absolutePrecision) return (left+right)/2
    leftThird := (left*2+right)/3   // Треть влево
    rightThird := (left+right*2)/3  // Треть вправо
    if (f(leftThird) < f(rightThird))
        return ternarySearch(f, leftThird, right, absolutePrecision)
    else
        return ternarySearch(f, left, rightThird, absolutePrecision)
end
*/

/*
  // компактное по коду и работоспособное решение той же задачи
  // ( нахождение максимума функции ) но медленние в 3-5 раз

      for ( y=0; y < size; y++ ){ BufS[y]=HBuf[d]; d+=M; }

            BufR=HBuf;
           dt=LfMem=(Lf+Lf+Rg)/3;              // треть влево (Lf+Lf+Rg)/3;
          filterLaplasian(BufR,dt/4,(dt/4)*2); // полосовой `фильтр
          CombFilter(BufR, dt, g);             // гребенчатый фильтр, резонансный фильтр
          AmplitudeS(BufR, 2, LfAmSum);        // получение средней амплитуды

           BufR=HBuf;
           dt=RgMem=(Lf+Rg+Rg)/3;              // треть вправо  (Lf+Rg+Rg)/3;
          filterLaplasian(BufR,dt/4,(dt/4)*2); // полосовой `фильтр
          CombFilter(BufR, dt, g);             // гребенчатый фильтр, резонансный фильтр
          AmplitudeS(BufR, 2, RgAmSum);        // получение средней амплитуды

           DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="
           <<LfAmSum<<"   RgAmSum="<<RgAmSum<<END); DM(Rg-Lf<<" Rg-Lf "); DM(END);

		  if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
          if ( fabs(Rg-Lf)<Delta || RgAmSum-LfAmSum==0 ) break;
*/

#ifdef DEBUGLVL
if( GrafT[2][5] ) {
                        // ВИЗУАЛИЗАЦИЯ цифровая
	  DM(END); DM("  N "<<N); DM("  NN "<<NN); DM("  M "<<M); DM(END);
DM("  Lf "<<Lf); DM("   Rg "<<Rg); DM("   Delta "<<Delta); DM(END); DM(END);
DM(END); DM(" MeanSA уточненое значение="<<MeanSA);  DM(" за n тактов="<<n+1);
DM(END);

/*
                     // ВИЗУАЛИЗАЦИЯ аналоговая
LineVertical(ncolumns/2, 0x001F4400);// Визуализация вертикальных линий, темно синий
LineVertical(AdrAmMAX, 0x0000AA00);  // Визуализация вертикальных линий, зеленый

              // вывод на экран уточненного козффициента MeanSA
              ImageProcessorMain->Edit4->Text=IntToStr(MeanSA);

*/                      // визуализация спектра
//                      ImagingSpectrum(HBuf);
}
#endif

// локальный подсчет времени выполнения текущей функции PeriodBlock
#ifdef DEBUGLVL
tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;
if( GrafT[2][5] ) { DM(" PeriodBlock end: "); DM(" local time = "<<time<<END); }
#endif

   //  освобождение памяти (1)

	if ( Buf1 )  free(Buf1);    if ( Buf2 )  free(Buf2);   // if (a)
	if ( BufTL ) free(BufTL);   if ( BufTR ) free(BufTR);  // if (a!=NULL)	


//----------------------------------------------------------------------------//
/*
//  if (!BufR) exit (1);      if (!BufTL) exit (1);    // if (!a) // if (a==NULL)
//  if (!BufS) exit (1);      if (!BufTR) exit (1);    // if (!a) // if (a==NULL)

//  if ( BufR || BufS || BufTL || BufTR ) exit (1);

  int size=(xp1-xp0)*(yp1-yp0);
  int *BufS=(int*)calloc(size+32,4);    // запрос памяти min с очисткой 0
  if (BufS==NULL) exit (1);

  if ( ImBuf !=NULL ) free(BufS);
*/
//----------------------------------------------------------------------------//

}//_____________________________________________________________________________




                   ///  ////////////////  ///////

//
// Подпрограмма для получения координат букв (блоков) в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::BlockDetector() {

  int y,x,d,p;
  int KHg,Prg,maxbft,Nblck,scope=8;
  //int s0,s1,s3,s4;
  int NLetter;  // вычисленное примерное количество букв в строке

                             GBuf=HBuf=LStr;
////////////////////////////////////////////////////////////////////////////////
		  // уточненое значение козффициента для сглаживающего фильтра SP->filterHigh
		  // 0.25 сответствует фильтрации буквы
		  KHg=MeanSA*0.25;       // *0.33 1/3       // KHig, KLow, MeanSA- глобальные.
if( GrafT[2][2] ) { DM(END); DM(MeanSA*0.33<<" KHg ") }
////////////////////////////////////////////////////////////////////////////////
		  // сглаживание блоков букв ( KHg  уже адаптивный )
		  SP->filterHigh(GBuf,KHg);

          // зеленый, вызов функции рисования переменной длинны
if( GrafT[2][2] ) { WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x0000AA00); }

          // сильное сглаживание для получение максимума HBuf
		  SP->filterHigh(HBuf,MeanSA*2);   //KHg=MeanSA*2;

          // красный, вызов функции рисования переменной длинны
if( GrafT[2][2] ) { WDrawVar(HBuf,ncolumns,nrows/16,scope*6,0x000000FF); }

          // получение максиииимума HBuf
          maxbft=0;
          for ( x=8; x < ncolumns-8; x+=4 ){  // 8 пикселей от края печа не используются.
                   if( HBuf[x] > maxbft ) { maxbft=HBuf[x]; }
		  }// for x

          HBuf.assign(ncnr,0);

          // получение порога по максимуму HBuf
          Prg=maxbft*0.25; p=d=0; // *0.25  1/4
          // получение координат блоков букв
          // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
          for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
                   if( GBuf[x] >= Prg ) { GBuf[x]=Prg; p++; }
                   else { GBuf[x]=0; }
                   if( GBuf[x]==Prg && GBuf[x-1]==0 ) { HBuf[d]=x; }
                   if( GBuf[x]==0 && GBuf[x-1]==Prg ) { HBuf[d+1]=x; d+=2;}
          }// for x
          Nblck=d;           // DM(END); DM(Nblck<<" Nblck ")

		  // примерная длина строки (оценка)  //, глобальная
		  NLetter=p;           // NLetter=p/MeanSA;  количество букв в строке

		  // Заполнение структуры длинной строки (для GBitsetNet.h)
		  strArray[0][Nstr].NLetter=NLetter;

          // вычисленное колличество блоков букв во всем печа, глобальная
          NBlockP=NBlockP + Nblck;  // DM(NBlockP<<" NBlockP "); DM(END);

		   // Заполнение структур координатами блоков букв
           int DMn=MeanSA*0.25;
		   for ( y=0; y < Nblck; y+=2 ){
                 blockOCR wt;
                 wt.xb0=HBuf[y];
                 wt.xb1=HBuf[y+1];
                 wt.yb0=strArray[0][Nstr].y0 - DMn; // - для красоты
                 wt.yb1=strArray[0][Nstr].y1 + DMn; // + для красоты
                 wt.langFlag=0;     // обнуление флага флаг языка langFlag
                 wt.blockFlag=0;    // обнуление флага принадлежности к колонке
                 strArray[0][Nstr].blockArray.push_back(wt);
//DM( " xb0="<<wt.xb0<< " yb0="<<wt.yb0<< " xb1="<<wt.xb1<< " yb1="<<wt.yb1<< "\n---\n");
           }// for y


           

#ifdef DEBUGLVL
if( GrafT[2][2] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны

					  // ВИЗУАЛИЗАЦИЯ цифровая
//		           DM(END); DM(NLetter<<" NLetter ");
//                 DM(END); DM(DMn<<" DMn "); DM(END);
/*
                      /// ТАБЛИЦА
            DM(END);
            for ( y=1; y < Nblck; y++  ){
            DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
              }//for y
            DM(END);
*/
}
#endif
/**/

}//_____________________________________________________________________________


                   //  // ///  ////  //
                   //  //// // //
                   //// /////// // / /
//
// Подпрограмма для получения вертикальных сумм блоков букв по всей высоте строницы.
// -----------------------------------------------------------------------------
void GBitset::BlockSum() {

  int x;
  int NBlock,Nblk;
  int s0,s1;  // ,s3,s4;
  // s0,s1     координаты квадрата вокруг блока букв

		  // вычисленное колличество блоков букв во всем печа
          if( NBlockP==0 ) { return; }

		  // получение вертикальных дифф. сумм блоков букв по всей высоте строницы.
           NBlock = (int)strArray[0][Nstr].blockArray.size();
           for ( Nblk=0; Nblk < NBlock; Nblk++ ){
               blockOCR *wt=&strArray[0][Nstr].blockArray[Nblk];
               s0=wt->xb0;   s1=wt->xb1;  // s3=wt->yb0;   s4=wt->yb1;
			   for ( x=s0; x < s1; x++ ){
//				   FFFF[x]+=LStr[x]>>3;  /8      // вертикальные суммы
				   LStS[x]+=BufL[x]>>3;          // вертикальные дифф. суммы
			   }// for x >>3  //  for ( x=0; x < ncolumns-1; x++ ){ LStS[x]+=LStr[x]>>3; }// for x
		   } // NWord

		  // получили вертикальные дифф. суммы блоков букв в массиве LStS.

#ifdef DEBUGLVL_GREY
if( GrafT[2][2] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
// красный, вызов функции рисования переменной длинны
//WDrawVar(LStS,ncolumns,nrows/16,scope*6,0x000000FF);// красный

// ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм (черная), для LStr[x]>=0
int M=64; static short delta=nrows*10/120; // 300
for(x=0; x < ncolumns-1; x++) {
for (int y=delta; y<(LStS[x]/M)+delta; y++)drawData[0][y][x]*=0.1;} // 0.8*ncolumns;
}
#endif
/**/

}//_____________________________________________________________________________



//                                             ***  **  ***
//
// Подпрограмма для получения координат блоков слов в пределах страницы.
// -----------------------------------------------------------------------------
void GBitset::LineDetector() {

  int x,d;
  int KHg,Prg,maxbft,Nblck,scope=8;
  //int s0,s1,s3,s4;

         // вычисленное колличество строк в печа - NStrok=strArray.size();
         // вычисленное колличество блоков букв во всем печа NBlockP
                       NStrok=(int)strArray->size();
                       if( NStrok<4 || NBlockP==0 ) { return; }

                             GBuf=LStr;

		  // уточненое значение козффициента для сглаживающего фильтра SP->filterHigh
		  KHg=MeanSA*1.5;      // *1.8;       // KHig, KLow, MeanSA- глобальные.

		  // сглаживание блоков слов - ширина колонок ( KHg  уже адаптивный )
		  SP->filterHigh(GBuf,KHg);

          // зеленый, вызов функции рисования переменной длинны
if( GrafT[2][7] ) { WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x0000AA00); }

                            HBuf=GBuf;

          // сильное сглаживание для получение максимума HBuf
		  SP->filterHigh(HBuf,MeanSA*4);  //  SP->filterHigh(HBuf,MeanSA*4);

          // красный, вызов функции рисования переменной длинны
if( GrafT[2][7] ) { WDrawVar(HBuf,ncolumns,nrows/16,scope*6,0x000000FF); }

          // получение максимума HBuf
          maxbft=0;
          for ( x=8; x < ncolumns-8; x+=4 ){  // 8 пикселей от края печа не используются.
                   if( HBuf[x] > maxbft ) { maxbft=HBuf[x]; }
          }// for x

          HBuf.assign(ncnr,0);

          // получение порога по максимуму HBuf
          Prg=maxbft*0.25; d=0; //  1/4
          // получение координат блоков слов на развороте
          // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
          for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
                   if( GBuf[x] >= Prg ) { GBuf[x]=Prg; }
                   else { GBuf[x]=0; }
                   if( GBuf[x]==Prg && GBuf[x-1]==0 ) { HBuf[d]=x; }
                   if( GBuf[x]==0 && GBuf[x-1]==Prg ) { HBuf[d+1]=x; d+=2;}
          }// for x
          Nblck=d;     // DM(END); DM(Nblck<<" Nblck ")


//  MeanSA - средее по всей строке расстояние между центрами масс букв
           // Заполнение структур координатами блоков слов
///           int DMn=MeanSA*0.4+2;
///           for ( y=0; y < Nblck; y+=2 ){
///                 lineOCR wt;
///                 wt.xL0=HBuf[y];   // - DMn/2
///                 wt.xL1=HBuf[y+1]; // + DMn/2
///                 wt.yL0=strArray[0][Nstr].y0 - DMn; // - для красоты
///                 wt.yL1=strArray[0][Nstr].y1 + DMn; // + для красоты
///                 wt.lineFlag=1;      // 1// установка флага блоков слов (колонок)
///                 strArray[0][Nstr].lineArray.push_back(wt);
//DM( " xL0="<<wt.xL0<< " yL0="<<wt.yL0<< " xL1="<<wt.xL1<< " yL="<<wt.yL1<< "\n---\n");
///           }// for y
/**/


#ifdef DEBUGLVL
if( GrafT[2][7] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны
/*
                      // ВИЗУАЛИЗАЦИЯ цифровая
                      /// ТАБЛИЦА
            DM(END);
            for ( y=1; y < Nblck; y++  ){
            DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
              }//for y
            DM(END);
*/
}
#endif
/**/

}//_____________________________________________________________________________



////////////////////////////////////////////////////////////////////////////////

/*
//////
   // Цикл по колличеству строк NStrok
///   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok - глобальные /////////
             // получение координат строк y0, y1 из структуры
             y0=strArray[0][Nstr].y0;   y1=strArray[0][Nstr].y1;
             // DM(END); DM(Nstr<<" Nstr "); DM(y0<<" y0 "); DM(y1<<" y1 "); DM(END);
             // смещение границ полосы подсчета вертикальных сумм вверх и в низ
             s0=y0-(y1-y0)/21;   if ( s0 < 2 )        s0 = 1;  // *2/3;
             s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
             // подсчет вертикальных сумм в пределах строки, вертикальный фильтр
             p=0;
             for (y=s0; y<s1; y++){
                 d1=A+y*w;        //A[y][x]                         //      -1
                 d2=d1+w;         //A[y+1][x]                       //       1
                 res=0;
                 for (x=0; x<ncolumns; x++){
                     //  выделение граници штриха буквы в один пиксел, независимо от толщины штриха буквы
                     ///d=*(d1+x)-*(d2+x);   //  -*(d3+x)-*(d4+x)-*(d5+x)
                     d=*(d1+x);
                     res+=d;
                     // визуализация частей букв в окошке в виде граници в один пиксел
#ifdef DEBUGLVL
if ( GrafT[6][2] ) { drawData[0][y][x]=255+d*196; }  // draw it   *(d+x)
#endif
                 }  // for x
                 LStr[p]=res<<8;   p++;     // res<<8  *256 копирование в BufS
//                 if ( p > w-2 ) { p=w-2; }
              }  // for y
// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
             LineBlockStart(LStr,HBuf,GBuf);
///       }// for Nstr // Цикл по колличеству строк NStrok ////////////////////////
//////
 */



////////////////////////////////////////////////////////////////////////////////

/*
			   KHig=MeanSA/3; // *2

			   SP->filterHigh(BufR,KHig);

               // преобразование ГИЛБЕРТА
			   filterGilbert(BufR,BufT, ncolumns,KHig/16);

               // преобразование ГИЛБЕРТА
			   filterGilbert(BufT,BufHelp, ncolumns,KHig/16);

      int bfh, bft;
      for ( y=8; y < ncolumns-8; y++ ){  // 8 пикселей от края печа не используются.
              bfh=BufHelp[y]*8/10; // *30/10
              bft=BufT[y];
              bfh=sqrt(bfh*bfh + bft*bft);
              BufHelp[y]=(bfh + BufR[y])/1;   //
      }// for y

               // фильтрация, сглаживание, полосовой фильтр
//               SP->filterHigh(BufHelp,KHig);
*/

/*
               KHig=MeanSA/4; // *2
               if ( KHig>=31 ) KHig=31;
			   KLow=KHig*2;  // KHig, KLow, MeanSA- глобальные.

			   BufT=BufR;

			   SP->filterHigh(BufR,KHig);

               // сглаживание, полосовой фильтр
               filterLaplasian(BufT,KHig,KLow);

      int bfh, bft;
      for ( y=8; y < ncolumns-8; y++ ){  // 8 пикселей от края печа не используются.
              BufHelp[y]=( abs(BufT[y]) + BufR[y] )/1;   //
      }// for y

               // фильтрация, сглаживание, полосовой фильтр
//               SP->filterHigh(BufHelp,KHig);
*/

/*
               // преобразование ГИЛБЕРТА PGilb==15
               for (  y=12; y < ncolumns-12; y++ ){
               BufT[y]=
                     ( + (BufR[y-7]-BufR[y+7])/7 + (BufR[y-6]-BufR[y+6])/6
                       + (BufR[y-5]-BufR[y+5])/5 + (BufR[y-4]-BufR[y+4])/4
                       + (BufR[y-3]-BufR[y+3])/3 + (BufR[y-2]-BufR[y+2])/2
                       +  BufR[y-1]-BufR[y+1] )/2;
                    }// for y

               // преобразование ГИЛБЕРТА PGilb==15
               for (  y=12; y < ncolumns-12; y++ ){
               BufHelp[y]=
                     ( + (BufR[y-7]+BufR[y+7])/7 + (BufR[y-6]+BufR[y+6])/6
                       + (BufR[y-5]+BufR[y+5])/5 + (BufR[y-4]+BufR[y+4])/4
                       + (BufR[y-3]+BufR[y+3])/3 + (BufR[y-2]+BufR[y+2])/2
                       +  BufR[y-1]+BufR[y+1] )/2 /8;
                    }// for y
*/
////////////////////////////////////////////////////////////////////////////////













