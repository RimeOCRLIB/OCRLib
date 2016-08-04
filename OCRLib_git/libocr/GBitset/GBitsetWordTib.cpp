#include "GBitset.h"

////////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------//
// программа получения координат Tib букв в пределах страници (квадратики).   //
// ---------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

// Запуск обработки всех строк страници.
void GBitset::PageWordDetTib() {


  int y,x,d,s0,s1;
  int y0,y1;
  int res;                   // unsigned long res int;   2147483647  // short
  //int print=1;


//DM("@@_2");
TIME_START

                 // Adaptive_Level  адаптивный уровень

                 //   if( NStrok<0) { return; }  page   строка line Line

// size()  Как видите Size показывает сколько сейчас лежит в векторе чисел.
// capacity() - сколько может храниться до изменения размера
// В то время как capacity возвращает инициализированный размер,
// то есть тот размер, до которого можно добавлять данные без инициализации.
// max_size() - максимальный размер обычно равен наиболее большому доступному блоку памяти
/*
        DM(" Size Vector "    <<BufR.size());
        DM(" Capacity Vector "<<BufR.capacity());
        DM(" Max_Size Vector "<<BufR.max_size()); DM(END); DM(END);
*/

//   int Lim=2147483647>>8-2;   // DM(Lim<<" Lim ");

///////////////
                            

   // Цикл по колличеству строк NStrok // strArray.size()= NStrok - вычисленное колличество строк в печа
   for ( Nstr=0; Nstr < strArray->size(); Nstr++ ){      // Nstr - глобальная
             // получение координат строк y0, y1 из структуры ( NStrok=strArray.size(); )
             y0=strArray[0][Nstr].y0;   y1=strArray[0][Nstr].y1;
             // DM(END); DM(Nstr<<" Nstr "); DM(y0<<" y0 "); DM(y1<<" y1 "); DM(END);
             // подсчет вертикальных сумм в пределах строки
             for ( x=0; x < ncolumns; x++ ) {  // read image make sum    // ncolumns-1
                   res=0;
                   // смещение границ полосы подсчета вертикальных сумм вверх и в низ
				   s0=y0-(y1-y0)/3;  if ( s0 < 2 )  s0 = 1;  // *2/3   *0.666
                   s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
				   for ( y=s0; y < s1; y++ ) {  // && y<nrows
						d=y*ncolumns;  res+=bits_data[d + x];
#ifdef DEBUGLVL_GRAY
if ( GrafT[3][1] ) { drawData[0][y][x]*=0.8; }  // визуализация (серым) ////
#endif
                       }  // for y
//                 if ( res > Lim )  res = Lim;
				   LStr[x]=res<<8; // ВАЖНО - сумма вдоль строк домножена на *256
                 }  //  for x
// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
	   ParameterMeanDetector();
   }// for Nstr // Цикл по колличеству строк NStrok
/**/
//////////////
/*
////
            // номер строки на графику .
             m=StrToInt(ImageProcessorMain->Edit5->Text);  //
             if ( m > NStrok ) { m = NStrok; } // NStrok начинается с 1

             // получение координат строк y0, y1 из структуры
             y0=strArray[0][m].y0;   y1=strArray[0][m].y1;

             // подсчет вертикальных сумм в пределах строки m
             for ( x=0; x < ncolumns; x++ ) {  // read image make sum
                   res=0;
                   // смещение границ полосы подсчета вертикальных сумм вверх и в низ
                   s0=y0-(y1-y0)*2/3;  if ( s0 < 2 )  s0 = 1;
                   s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
                   for ( y=s0; y < s1; y++ ) {
                         d=y*ncolumns;  res+=bits_data[d + x];
                       } // for y
                   if ( res > Lim )  res = Lim;
                   LStr[x]=res<<8; // =GBuf[x]
                   // ВАЖНО - сумма вдоль строк домножена на 256
                 } // for x
                Nstr=m;   // Nstr - глобальная
//                                       for ( y=y0; y < y1 && y<nrows; y++ )
////


// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
             ParameterMeanDetector();
 */

#ifdef DEBUGLVL
/*          // ВИЗУАЛИЗАЦИЯ цифровая, координаты строк массив HBuf
              DM(END);
              for ( y=0; y < NStrok*4;  y++ ){        // ТАБЛИЦА
                DM(HBuf[y]<<"\t"); if ((y+1)%10==0)DM(END);
                  }//for y
              DM(END);
*/
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

/*  // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
  int M=64; static short delta=nrows*10/40; // 300
  for(x=0; x < ncolumns; x++) {
  for (y=delta; y<(LStr[x]/M)+delta; y++)drawData[0][y][x]*=0.1;}// 0.8*ncolumns;
*/
#ifdef DEBUGLVL   
      if( GrafT[3][0] ) { DP(END); DP(33333<<" PageWordDetTib ");  DP(endl); }
#endif
    
              // вызов горизонтальной функции рисования
//            WDraw(LStr,20,0x00AA0011); // темно синий
#endif

TIME_PRINT

}//_____________________________________________________________________________

//                            ЗАПУСК

// Запуск программы получения координат букв в пределах строки, параметр MeanSA.
// -----------------------------------------------------------------------------
void GBitset::ParameterMeanDetector() {
    int print=0;
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
             // запуск подпрограммы получения координат букв в пределах
             // строки с стартовами, по размеру строк значениям MeanSA.
DT("s2_1");
	LineLetterStart();
DT("s2_2");
	// получение среднего периода букв MeanSA

#ifdef DEBUGLVL
/// DM(" srcW="<<srcW<<" srcH="<<srcH<<" nrows="<<nrows<<" ncolumns="<<ncolumns<<END);
#endif

}//_____________________________________________________________________________


// Запуск блока подпрограмм для получения координат букв в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::LineLetterStart() {

bit_vector  AMask(ncolumns);

//cout<<"w1";
// Подпрограмма обработки статистики букв короткими импульсами в пределах строки.
               ShortBurstNP(AMask);
//cout<<"w2";
// Нелинейная обработка статистики букв в пределах строки, система масок.
               SystemMasksNP(AMask);
//cout<<"w3";
// Подпрограмма получения среднего периода букв в пределах строки.Поиск триадами.
               PeriodLetters();
//cout<<"w4";
// запуск подпрограммы получения координат букв в пределах строки.
               RepeatLetterDetector();
//cout<<"w5";
// Подпрограмма для получения точных координат букв в пределах строки.
               AccurateLetterDetector();
//cout<<"w6";
/**/
}//_____________________________________________________________________________

//                 Получение коротких импульсов.   I  I I  I  I
//
// Нелинейная обработка статистики букв короткими импульсами в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::ShortBurstNP(bit_vector  &AMask) {

  int y,p; // ,d


// получение битовой маски для умножнния вертикальных сумм букв на короткие импульсы
//    NonlinearProcessing  нелинейная обработка,   short burst короткий импульс

//TIME_START
                            BufHelp=LStr;

// ВЫЧИСЛЕНИЕ ПРЕДВОРИТЕЛЬНОГО козффициента для сглаживающего фильтра SP->filterHigh
              // MeanSA=DeltaNstrKH*6/5;
              KHig=MeanSA/3; // MeanSA/4;
              if ( KHig>=31 ) KHig=31;
              KLow=KHig*2;  // KHig, KLow, MeanSA- глобальные.

              // выделение промежутков между буквами
              // ассиметричное преобразование ГИЛБЕРТА
              for ( y=4; y < ncolumns-4; y++ ){
                BufT[y]=abs( (LStr[y-3]-LStr[y+2])/3 +
                             (LStr[y-2]-LStr[y+1])/2 +
                              LStr[y-1]-LStr[y]       );
              }// for y

              // сглаживание, полосовой фильтр
              int KHg=(KHig+1)/2;
			  SP->filterLaplasian(BufT,KHg,KHg*3);

              // преобразование ГИЛБЕРТА - BufT
              // KHig/16=PGilb - параметр (ширина) преобразования ГИЛБЕРТА 0,1,2,...
              // ncolumns=MGilb - размер массива преобразования ГИЛБЕРТА
              filterGilbert(BufT,BufS, ncolumns,KHig/16);


        // Вычисление максимальнй амплитуды огибающей промежутков между букв.
        int GAmp; // d=0;
        for ( y=8; y < ncolumns-8; y++ ){ // (8 пиксемей от края печа не используются)
           // вычисление координат середины BufS по огибающей
            if( BufS[y]<=0 && BufS[y+1]>0 )
                // вычисление двух координат левого и правого края короткого импульса по GAmp
                {      GAmp=BufT[y]*3/4;
                     for ( p=y;  BufT[p]>GAmp && p>8;          p-- )
                     { AMask[p]=1; BufHelp[p]=0; }
//                     LineVertical(p, 0x0000AA00);  // зеленый
                     for ( p=y;  BufT[p]>GAmp && p<ncolumns-8; p++ )
                     { AMask[p]=1; BufHelp[p]=0; }
///                    LineVertical(y, 0x000000FF);  // красный
//                     LineVertical(p, 0x0000AA00);  // зеленый
                } // GAmp=
        }// for y

/**/

#ifdef DEBUGLVL
/*
int SmGr=nrows/4;
                     // ВИЗУАЛИЗАЦИЯ цифровая
DM(END);  DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows "); DM(END);

                    // ВИЗУАЛИЗАЦИЯ аналоговая переменной длинны
WDrawVar(BufT,ncolumns,SmGr,32,0x00AA0011);  // вызов функции рисования ТЕМНО СИНИЙ
WDrawVar(BufS,ncolumns,SmGr,32,0x0000AA00);  // вызов функции рисования зеленый
//                  LineVertical(p, 0x00AA0011);  // темно синий
*/
/*
 // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
 int M=64; static short delta=SmGr/8; // 300
  for(x=0; x < ncolumns; x++) {
  if(BufHelp[x]>=0){ for (y=delta; y<(BufHelp[x]/M)+delta; y++)drawData[0][y][x]*=0.1; }
  else             { for (y=delta; y>(BufHelp[x]/M)+delta; y--)drawData[0][y][x]*=0.1; }
 } // x++  // 0.8*ncolumns;
*/

#endif
//TIME_PRINT

}//_____________________________________________________________________________


//                    Система масок.   00001010010001
//
// Нелинейная обработка статистики букв в пределах строки, система масок.
// -----------------------------------------------------------------------------
void GBitset::SystemMasksNP(bit_vector  &AMask) {

  int y,p,d,s=0;
  //int MGilb,PGilb,scope=8;
  bit_vector  BMask(ncolumns);

//TIME_START

                           BufT=LStr;

              // фильтрация, сглаживание, полосовой фильтр
			  SP->filterLaplasian(BufT,KHig,KLow);

//WDrawVar(BufT,ncolumns,SmGr*3,32, 0x00AA0011);  // вызов функции рисования ТЕМНО СИНИЙ

              // преобразование ГИЛБЕРТА
              filterGilbert(BufT,BufS, ncolumns,KHig/16);

//WDrawVar(BufS,ncolumns,SmGr*3,32,0x0000AA00);  // вызов функции рисования зеленый

   // Вычисление центра масс ширины центрально симметричных букв.
        d=0;  int GAmp=0, maxT=0, PGrad=0;
        PGrad=65000/(MeanSA*MeanSA+1); // градиент BufS д.б. больше порога PGrad, 65535
        for ( y=8; y < ncolumns-8; y++ ){ // (8 пиксемей от края печа не используются)
           // вычисление координат середины BufS по огибающей
            if( BufS[y]<=0 && BufS[y+1]>0 && (BufS[y+1]-BufS[y])>PGrad ) // 1600/(MeanSA+1)
                // вычисление координат левого и правого края центрально симметричных букв по GAmp
                {    d++; maxT=maxT+abs(BufT[y]);// нахождение среднего abs(BufT[y]);
                     GAmp=BufT[y]*2/3;  // *3/4
                     for ( p=y;  BufT[p]>GAmp && p>8;          p-- ){BMask[p]=1;}
//                     LineVertical(p, 0x0000AA00);  // зеленый
                     for ( p=y;  BufT[p]>GAmp && p<ncolumns-8; p++ ){BMask[p]=1;}
////                   LineVertical(y, 0x000000FF);  // красный   //////
//                     LineVertical(p, 0x0000AA00);  // зеленый
                } //
        }// for y

                // порог ограничения высоты букв
                maxT=maxT*4/(d+1);  // 5

               
/*
#ifdef DEBUGLVL
        for (  y=0; y < ncolumns; y++ ){ BufHelp[y]=4*BMask[y]; }// for y
WDrawVar(BufHelp,ncolumns,SmGr*3,1, 0x000000FF);  // вызов функции рисования красный
//                    LineVertical(p, 0x00AA0011);  // темно синий
#endif
*/
              // решение системы неравенств, а именно
              // отделение краев зеркально отраженных букв друг от друга,
              // защита середины центрально симметричных букв
              for (  y=0; y < ncolumns; y++ ){
                  if( AMask[y]==1 && BMask[y]==0 ) { s=0; }  else { s=LStr[y]; }
                  // выравнивание (ограничение) высоты букв
                  if (s > maxT) { s=maxT; }
                  BufHelp[y]=HBuf[y]=s;
              }// for y
/**/

/* // заремачено 5_III_2009
//------------------------------------------------------------------------------

        d=0;  int AmpRamka, maxBufT=0;
        PGrad=6500/(MeanSA*MeanSA+1); // градиент BufS д.б. больше порога PGrad
        for ( y=32; y < ncolumns-32; y++ ){ // (32 пиксемей от края печа не используются)
              if( BufS[y]<=0 && BufS[y+1]>0 && (BufS[y+1]-BufS[y])>PGrad  )
              {
              d++; maxBufT=maxBufT+abs(BufT[y]);// нахождение среднего abs(BufT[y]);
////              LineVertical(y, 0x000000FF);  // красный   //////
              } //
        }// for y
                // AmpRamka=8*maxBufT/ncolumns*d/100;
                AmpRamka=maxBufT/d*128/ncolumns;
////                 DM(END); DM(AmpRamka<<" AmpRamka "); DM(d<<" d ");DM(END);

//------------------------------------------------------------------------------
*/  

#ifdef DEBUGLVL
/* ///
int SmGr=nrows/4;
                     // ВИЗУАЛИЗАЦИЯ цифровая
                                DM(END);
DM(MeanSA<<" -MeanSA- "); DM(KHig<<" _KHig_ "); DM(DeltaNstr<<" DeltaNstr ");
DM(PGrad<<" PGrad "); DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows ");
                                DM(END);
*/

                    // ВИЗУАЛИЗАЦИЯ аналоговая
///WDrawVar(BufT,ncolumns,SmGr*3,32, 0x00AA0011); // вызов функции рисования ТЕМНО СИНИЙ
///WDrawVar(BufS,ncolumns,SmGr*3,32,0x0000AA00);  // вызов функции рисования зеленый
// WDrawVar(HBuf,ncolumns,SmGr/2,128,0x000000FF); // вызов функции рисования красный

/* 
 // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
 int M=64; static short delta=SmGr/8; // 300
  for(x=0; x < ncolumns; x++) {
  if(BufHelp[x]>=0){ for (y=delta; y<(BufHelp[x]/M)+delta; y++)drawData[0][y][x]*=0.1; }
  else             { for (y=delta; y>(BufHelp[x]/M)+delta; y--)drawData[0][y][x]*=0.1; }
 } // x++  // 0.8*ncolumns;
*/
#endif

//TIME_PRINT

}//_____________________________________________________________________________

//                           Поиск триадами(ФАпЧ)

// Подпрограмма получения среднего периода букв в пределах строки.Поиск триадами
// -----------------------------------------------------------------------------
void GBitset::PeriodLetters() {

  int n,g,dt;
  int dtMemL,dtMemR,AmMAX,AdrAmMAX,LfAmSum,RgAmSum;
  float Lf,Rg,LfMem,RgMem,Delta;

////DM("PeriodLetters_2@@");
////static clock_t  tm_start1=clock();
////clock_t  tm_end; double time;  //DM(tm<<" "<<t);


//                    // TEST
//     int y, DnKH=25;  // 20
//     for (  y=0; y < ncolumns-DnKH; y+=DnKH ){
//          for (  p=0; p < DnKH/4*3; p++ ){ BufQ[p+y]=1024; }// for p   d+=2;
//     }// for y
//WDrawVar(BufQ,ncolumns,SmGr*2,32,0x000000FF);  // вызов функции рисования, красный


    // автоматическое масштабирование размеров массивов для нахождение максимума спектра
    int y,d,size,M=1;
    M = (int)HBuf.size()/1250;
    if ( M<1 ) M=1;   if ( M>3 ) M=3;   //  M=2;
    size=ncolumns/M;
    ///vector<int> BufR(size);
    ///vector<int> BufS(size);
    vector<int> BufTL(size);
    vector<int> BufTR(size);
    
    d=2;
    if ( M<=1 ){ M=1;
      for ( y=2; y < size-2; y++ ){ BufS[y]=HBuf[d]; d+=1; }
    }
    if ( M==2 ){ M=2;
      for ( y=2; y < size-2; y++ ){ BufS[y]=(HBuf[d]+ HBuf[d+1])/2; d+=2; }
    }
    if ( M>=3 ){ M=3;
      for ( y=2; y < size-2; y++ ){ BufS[y]=(HBuf[d-1]+ HBuf[d]+ HBuf[d+1])/3; d+=3; }
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
         if ( dt/4 != dtMemL ) {  N++;
             SP->filterLaplasian(BufS, BufTL, dt/4, dt/2); // полосовой фильтр (dt/4)*2
         }
             dtMemL=dt/4;  NN++;
             BufR=BufTL;
             FunctionSpectrum(BufR, dt,LfAmSum);// гребенчатый фильтр, амплитудный детектор.

         dt=RgMem=(Lf+Rg+Rg)/3;   // треть вправо  (Lf+Rg+Rg)/3;
         if ( dt/4 != dtMemR ) {  N++;
             SP->filterLaplasian(BufS, BufTR, dt/4, dt/2); // полосовой фильтр (dt/4)*2
         }
             BufR=BufTR;
             dtMemR=dt/4;  NN++;
             FunctionSpectrum(BufR, dt,RgAmSum);// гребенчатый фильтр, амплитудный детектор.
#ifdef DEBUGLVL
////             DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="
////             <<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
#endif
         if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
         if ( Rg-Lf<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)

   }// for n
   // filterLaplasianM при таком алгоритме запускается примерно в 2 раза мене
   // часто, а он ощутимо сложнее чем FunctionSpectrum
// N=-22;  N=N<<1;
// DM(N<<" N "); DM(END);
// HBuf.assign(size,0);

              //  максимум спектра, адрес максимума спектра
        AmMAX=(LfAmSum + RgAmSum)/2;   AdrAmMAX=M*(LfMem + RgMem)/2;


              // получение УТОЧНЕННЫХ параметров из AdrAmMAX
              // получение козффициента dt для гребенчатого фильтра
  ///           if ( AdrAmMAX<Lf ) AdrAmMAX=Lf;   if ( AdrAmMAX>Rg ) AdrAmMAX=Rg;
			  MeanSA = AdrAmMAX;

#ifdef DEBUGLVL
if ( GrafT[3][5] ) { DM(END); DM(" -MeanSA_2- "<<MeanSA); DM(END); }   ////
#endif
			  // получение козффициентов KHig, KLow для полосового фильтра Gaussian
              KHig=(AdrAmMAX)/4; // ish KHigP=(MeanSA+3)/4;  // (MeanSA+0)/4;
              if ( KHig>=31 ) KHig=31;
              KLow=KHig*2;   // KHig, KLow, MeanSA - глобальные.
/**/
//// DM("PeriodLetters_2@@")tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);


  /*            // ПРИНЦИП РАБОТЫ поиска триадами
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
if ( GrafT[3][5] ) {
////                      // ВИЗУАЛИЗАЦИЯ цифровая
////      DM(END); DM(N<<" N "); DM(NN<<" NN "); DM(M<<" M ");DM(END);
////DM(END); DM(END); DM(AdrAmMAX<<"-AdrAmMAX-  "); DM(n+1<<"-n- "); DM(Lf<<" Lf ");
////DM(Rg<<" Rg "); DM(Delta<<" Delta "); DM(END); DM(END);



/*                     // ВИЗУАЛИЗАЦИЯ аналоговая
LineVertical(ncolumns/2, 0x001F4400);// Визуализация вертикальных линий, темно синий
LineVertical(AdrAmMAX, 0x0000AA00);  // Визуализация вертикальных линий, зеленый

              // вывод на экран уточненного козффициента MeanSA
              ImageProcessorMain->Edit4->Text=IntToStr(MeanSA);

*/                      // визуализация спектра
//                      ImagingSpectrum(HBuf);
}
#endif
//DM("2@@")tm_end=clock(); tm_end-=tm_start; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);

}//_____________________________________________________________________________


                      ////////////////

//
// Подпрограмма для получения координат букв в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::RepeatLetterDetector() {

  int y,d; 
  //int Prg,scope=8;


/// TIME_START
                        BufR=HBuf;

               // фильтрация, сглаживание, полосовой фильтр
			   SP->filterLaplasian(BufR,KHig,KLow);

               //  гребенчатый фильтр, резонансный фильтр
               int dt = MeanSA, g=3;  // 1 < g < 10
               CombFilter(BufR, dt, g);

               // фильтрация, сглаживание
			   SP->filterLaplasian(BufR,KHig,KLow);

               // преобразование ГИЛБЕРТА
               filterGilbert(BufR,BufT, ncolumns,KHig/16);

                    HBuf.assign(ncolumns,0);

        // вычисление координат середины букв по максимумам sin.
///     int maxR=0;
        d=4;  // d=0;
        for ( y=8; y < ncolumns-8; y++ ){  // 8 пикселей от края печа не используются.
            if( BufT[y]<=0 && BufT[y+1]>0 ) {
              HBuf[d+2]=HBuf[d]=y;  d+=4;
///           maxR=maxR+abs(BufR[y]);     // нахождение среднего abs(maxR[y]);
//                  LineVertical(d, 0x0000AA00);  // зеленый
////                LineVertical(y, 0x00AA0011);  // темно синий  ////
            } // if( BufT[y]
		}// for y

        // NLet - колличество букв в строке, умноженное на 4.
		if ( d > ncolumns ) d=ncolumns;   if ( d < 0 ) d=0;
        NLet=d;


#ifdef DEBUGLVL
/*                      // ВИЗУАЛИЗАЦИЯ цифровая
//                 DM(END); DM(NLet<<" NLet "); DM(END);
 DM(END<<END<<" KFStrok="<<KFStrok<<" KHig="<<KHig<<" NStrok="<<NStrok<<END<<END);
                      // ВИЗУАЛИЗАЦИЯ аналоговая
WDrawVar(BufR,ncolumns,nrows/16,scope,0x000000FF);// вызов функции рисования переменной длинны, красный
WDrawVar(BufT,ncolumns,nrows/16,scope,0x0000AA00);// вызов функции рисования переменной длинны, зеленый
*/
#endif

/// TIME_PRINT

}//_____________________________________________________________________________


                      ////////////////

//
// Подпрограмма для получения точных координат букв в пределах строки.
// -----------------------------------------------------------------------------
void GBitset::AccurateLetterDetector() {

  int y,p,DtL,DtR;

/// TIME_START


             GBuf.assign(ncolumns,0);

        // нахождение середины между буквами, ограничение ширины квадратика
        p=MeanSA*2/3;      // *4/3/2  предельная ширин квадратика
        for ( y=4; y < NLet-4; y+=4 ){

             DtL=(HBuf[y]-HBuf[y-4])/2; // -4
             if ( DtL>p ) DtL=p;
			 GBuf[y]=HBuf[y]-DtL;

             DtR=(HBuf[y+4]-HBuf[y])/2; // -4
             if ( DtR>p ) DtR=p;
             GBuf[y+2]=HBuf[y+2]+DtR;

        }// for y
/**/
                           HBuf=GBuf;

 // Вызов функции для получения точных координат букв в пределах строки.
               AccurateLetFunction();
     // в массиве HBuf возвращаются букы, в GBuf возвращаются пробелы


/**/
               // Заполнение структур координатами букв без пустых квадратиков
               // OCRclases.h  // ImageProcessor.cpp
               // DM(END); DM(Nstr<<" Nstr ")

               for ( y=0; y < NLet-2; y+=4 ){  // NLet=NWord*4 // ( y=0; y < NWord*4; y+=4 )
 //                  if ( HBuf[y]!=0 && HBuf[y+2]!=0 ) {
                     wordOCR wt;
	                wt.x0=HBuf[y];
                     wt.y0=strArray[0][Nstr].y0; //
                     wt.x1=HBuf[y+2];
                     wt.y1=strArray[0][Nstr].y1; //
                     wt.center.x=(wt.x1-wt.x0)/2+wt.x0;
                     wt.center.y=(wt.y1-wt.y0)/2+wt.y0;
                     wt.w=wt.x1-wt.x0;
                     wt.h=wt.y1-wt.y0;
                     wt.correlation=0;
                     wt.tsertoFlag =0;        // обнуление флага точек tsegFlag
                     wt.spaceFlag=0;        // обнуление флага пробела spaceFlag  //wt.selectFlag=0;
                     wt.stackFlag=0;        // обнуление флага стек stackFlag
                     wt.vowelFlag=0;        // обнуление флага огласовок vowelFlag
                     wt.Param1=0;
                     wt.Param2=0;
                     wt.spaceFlag=GBuf[y];  // выделение пробелов spaceFlag
                     strArray[0][Nstr].wordArray.push_back(wt);
//                   } // if
//                   else {  LineVertical(HBuf[y], 0x0000AA00); // зеленый
//                   }
// DM( " x0="<<wt.x0<< " y0="<<wt.y0<< " x1="<<wt.x1<< " y1="<<wt.y1<< "\n---\n");
               }// for y


/*              // Подсчет среднего расстояния ( mean ) между буквами вдоль строки
              int  Porog, Delta;
              Porog=2*(MeanSA*5/6); // 3/2*
              Delta=1+ NLet/4/10; if ( NLet<7*4 ) Delta=0;
              Statistics( HBuf, GBuf, Porog, Delta );
*/



// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).

/**/
#ifdef DEBUGLVL
/*                      // ВИЗУАЛИЗАЦИЯ цифровая
                   DM(END); DM(Prg<<" Prg "); DM(END);
*/
#endif

/// TIME_PRINT

}//_____________________________________________________________________________


//                        AccurateFunction
// Функция для получения точных координат букв (дотягивание по нолям).
// -----------------------------------------------------------------------------

  void GBitset::AccurateLetFunction() {

  int y,x,n,p,d;
  int s, s1,s2,pcs, NLegit=ncolumns-2;
  int minSmYL, maxSmYR, minSm1, minSm2;
  vector<int> M(4);
  // 65535

    pcs=256*(DeltaNstrKH/16+1); // порог нолевых амплитуд- почти ноль, в пикселях
    // int prg=DeltaNstrKH/3;   // порог
     //int aa=NLet;

//----------------------------------------------------------------------------//

// смещение граници между буквами по нолевым амплитудам вертикальных сумм LStr
	for ( y=4; y < NLet-4; y+=4 ){


			 s1=(HBuf[y-2]-HBuf[y-4])*3/8; // интервал поиска на s1 - влево  *3/6
             s2=(HBuf[y+2]-HBuf[y])*3/8;   // интервал поиска на s2 - вправо *3/6

             /// отработка Левого смещения от середины расстояния между буквами
				 minSm1=minSm2=65535; // maxS1=0;
                 // поиск min (типа 1,2) и max в левом секторе
                 for ( p=HBuf[y]; p>HBuf[y]-s1 && p>0 && p<ncolumns; p-- ) {
                     if( LStr[p] <  minSm1 ) { minSm1=LStr[p]; M[0]=p; }
                     if( LStr[p] <= minSm2 ) { minSm2=LStr[p]; M[1]=p; }
///                  if( LStr[p] >  maxS1  ) { maxS1=LStr[p]; }
//                       LineVertical(p, 0x0000AA00);  // зеленый
                 } // if
//                       LineVertical(M[0]+1, 0x0000AA00);  // зеленый
//                       LineVertical(M[1]+1, 0x000000FF);  // красный
             // интервал поиска с не нолевым значением min вертикальных сумм не используем
                 if( minSm2>pcs ) { M[0]=NLegit; M[1]=NLegit; }


             /// отработка Правого смещения от середины расстояния между буквами
                 minSm1=minSm2=65535; // maxS2=0;
                 // поиск min (типа 1,2) и max в правом секторе
                 for ( p=HBuf[y]; p<HBuf[y]+s2 && p>0 && p<ncolumns; p++ ){
                     if( LStr[p] <  minSm1 ) { minSm1=LStr[p]; M[2]=p; }
                     if( LStr[p] <= minSm2 ) { minSm2=LStr[p]; M[3]=p; }
///                  if( LStr[p] >  maxS2  ) { maxS2=LStr[p]; }
//                       LineVertical(p, 0x0000AA00);  // зеленый
                 } // if
//                       LineVertical(M[2], 0x0000AA00);  // зеленый
//                       LineVertical(M[3], 0x000000FF);  // красный
             // интервал поиска с не нолевым значением min вертикальных сумм не используем
                 if( minSm2>pcs ) { M[2]=NLegit; M[3]=NLegit; }


             ///  выбор самых левых и самых правых концов нолевых отрезков
             minSmYL=65535; maxSmYR=0; d=0;
             for ( n=0; n < 4; n++ ){
               if( M[n] < minSmYL && M[n]!=NLegit ) {GBuf[y-2]=minSmYL=M[n]; d++;}
			   if( M[n] > maxSmYR && M[n]!=NLegit ) {GBuf[y]  =maxSmYR=M[n]; d++;}
			 }
			 // отработка случая когда не легитимны все 4 варианта концов
			 // нолевых отрезков
             if( d==0) { GBuf[y-2]=HBuf[y-2];  GBuf[y]=HBuf[y]; }

//                       LineVertical(HBuf[y+2], 0x00AA0011);  // темно синий
//                       LineVertical(HBuf[y],   0x00AA0011);  // темно синий

    }// for y

//----------------------------------------------------------------------------//

                            BufS=GBuf;
/**/

                        HBuf.assign(ncolumns,0);

		// вычисление и удаление из массива пустых квадратиков
		int Prg=MeanSA*5;   // (MeanSA*5/6)*2 порог для пустых квадратиков  1.66
        // Prg=0;
        d=0; p=0;
        for ( y=4; y < NLet-4; y+=4 ){
             // вычисление пустых квадратиков
             s=0; // s - сумма в пределах буквы
             for ( x=GBuf[y];  x<GBuf[y+2] && s<=Prg && x>0 && x<ncolumns;  x++ ){ // s < Prg
                s+=LStr[x]/256;    // Т.К. LStr[x]=res*256;
//                      LineVertical(x, 0x0000AA00);  // зеленый
             }// for x

             // удаление из массива пустых квадратиков
             if   ( s >= Prg ) {
                // корректированные по горизонтали граници между буквами
                HBuf[d]=GBuf[y];  HBuf[d+2]=GBuf[y+2];  d+=4; // GBuf[d]=

//              LineVertical(GBuf[y], 0x00AA0011);  // темно синий  ////
                if( d-4 < 0) { d=4; }   BufS[d-4]=0;
             } // if
             // выделение пробелов spaceFlag=1
//           // else { if( d-4 < 0) { d=4; }  GBuf[d-4]=1; } //  BufS[p+2]=1;  p+=4;
             else {   if( d-4 < 0) { d=4; }   BufS[d-4]=1;
///             LineVertical(BufS[y], 0x00AA0011);  // темно синий
              }
        }// for y

                            GBuf=BufS;

/**/
        // NLet - колличество букв в строке, умноженное на 4.
        // предидущее значение  NLet с пустыми квадратиками
        if ( d > NLet ) d=NLet;   if ( d < 0 ) d=0;
        // новое значение  NLet без пустых квадратиков
        NLet=d;


/// DM(END); DM(pcs<<" pcs "); DM(END);

/*                  // между minSmY1 и minSmY2  не должно быть большого maxSY1
                  maxS1=0;
                  for ( p=M[1];  p> M[0] && maxS1<prg && p>0 && p<ncolumns; p-- ) {
                  if( LStr[p] > maxS1 ) { maxS1=LStr[p]; M[1]=p; }
                  } // for y
                   // maxS1=pcs*5;  // Test
 */

 /*                  // между minSmY1 и minSmY2  не должно быть большого maxSY2
                  maxS1=0;
                  for ( p=M[2];  p< M[3] && maxS1<prg && p>0 && p<ncolumns; p++ ) {
               //   for ( p=M[2];  p< M[3] && maxS1<3*pcs && LStr[p]<4*pcs && p>0 && p<ncolumns; p++ ) {
                      if( LStr[p] > maxS1 ) { maxS1=LStr[p]; M[3]=p; }
                  } // for y
                   //  maxS1=pcs*5;  // Test
 */

}//_____________________________________________________________________________


   // в массиве BufS находятся исходные координаты центра букв,
   // не корректированные по горизонтали, с пустыми квадратиками
//                           BufS=HBuf;
/// не корректированные по горизонтали исходные координаты центра букв
///                GBuf[p]=BufS[y];    p+=4;  // GBuf[p+2]=BufS[y+2];  //


                // dt/2*2; это не ошибка, dt для fL д.б. только чётным
                // LfMem=dt=(Lf*(2-n)+Rg*(1+n))/3;  // n=0; треть влево (Lf+Lf+Rg)/3;
                // RgMem=dt=(Lf*(1+n)+Rg*(2-n))/3;  // n=0; треть вправо  (Lf+Rg+Rg)/3;


////////////////////////////////////////////////////////////////////////////////

