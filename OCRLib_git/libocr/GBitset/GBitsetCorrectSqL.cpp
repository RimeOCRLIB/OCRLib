#include "GBitset.h"

// -----------------------------------------------------------------------------
// --- Page Correct SqrL --- GBitsetCorrectSqrL.h ---
// Подпрограмма  вертикальной коррекции координат букв (квадратиков).
// -----------------------------------------------------------------------------

void GBitset::CorrectSqL(){


TIME_START

        if( NStrok<0) { return; }


        // Запуск программы горизонтальной коррекции положения квадратиков
                      CorrectLetterL();
//pageStackDetector
        // Запуск подпрограммы вертикальной коррекции ниижней полки квадратиков
                      CorrectLetterLH();   //  (StatCorrectLetter)

/**/
        // откорректировали координаты квадратиков по горизонтали и вертикали.

if( GrafT[12][0] ) { DM(END);  DM(44444<<" CorrectSqrL ");  DM(END); }

TIME_PRINT


}//_____________________________________________________________________________


//  ----------------------------------------------------------------------------
// Запуск программы горизонтальной коррекции положения квадратиков
//  ----------------------------------------------------------------------------

void GBitset::CorrectLetterL(){

  int y,x,d,s0,s1,s2,s3,q1,q2,q3;
  int x0,x1,xp0,xp1,x0B,x1B; //,xp;
  int y0,y1,yp,y0B,y1B;      //
  int res,res1,NWord,tFg;

/// TIME_START

////////////////////////////////////////////////////////////////////////////////

   for ( Nstr=0; Nstr < strArray->size(); Nstr++ ){ // Цикл по количеству строк NStrok

        NWord = (uint)strArray[0][Nstr].wordArray.size();
        for ( Nwrd=0; Nwrd < NWord-1; Nwrd++ ){ // Цикл по количеству квадратиков вдоль строки NWord

           /// получение координат прямоугольников и точек из структуры по x по y
               // xp,yp координаты точки
               wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
               yp = wP->yp;
               // x0,x1, y0,y1 координаты квадрата вокруг буквы
               x0 = wP->x0;    x1 = wP->x1;    y0 = wP->y0;    y1 = wP->y1;
               // xp0,xp1 координаты прямоугольника вокруг точки
               xp0= wP->xp0;   xp1 = wP->xp1;
               // tFg флаг наличия точки в прямоугольнике
               tFg = wP->tsertoFlag;
               // x0B,x1B, y0B,y1B координаты квадрата вокруг буквы  Nwrd+1
               wP=&strArray[0][Nstr].wordArray[Nwrd+1];
               x0B= wP->x0;    x1B = wP->x1;   y0B = wP->y0;   y1B = wP->y1;
	     		
	    
			if(y0B>nrows-1)y0B=nrows-1; if(y0B<0)y0B=0;
			if(y0>nrows-1)y0=nrows-1; if(y0<0)y0=0;

               // подсчет вертикальных сумм в части квадрата вокруг буквы Nwrd
               s0=(xp0+xp1)/2;
               // смещение в низ для визуализации
               s2=y0-(y1-y0)/2;
               // проверка окончания (начала) строки
               s3=xp1;

           /// сдвиг граници квадрата вокруг буквы влево, относительно
           /// прямоугольника поиска точки
               // вычисление величины смещение границ полосы подсчета
               // вертикальных сумм в низ до s1, площадь точки не учитываем.
               s1=y1;
               if ( tFg ) {
                  q1=yp-(tFg+1)/2;  q2=y0+(y1-y0)/2;  q3=y0+(y1-y0)*2/5; // tFg+1; TMP
                  s1=(q1 < q2) ? q1: q2;                  // т.е. y = min(a, b).
                  s1=(s1 > q3) ? s1: q3;                  // т.е. y = max(a, b).
                  }
                  // подсчет вертикальных сумм в части квадрата вокруг буквы Nwrd
                  for ( x=s0; x>xp0 && x>16; x-- ) { // горизонтальный цикл
                      res1=res;  res=0;
                      // смещение границ полосы подсчета вертикальных сумм в низ до s1
                      for ( y=y0; y < s1 && y<nrows; y++ ) { // вертикальный цикл
                          d=y*ncolumns;
                          res+=bits_data[d + x];
#ifdef DEBUGLVL           // Визуализация
///                          drawData[0][y][x]*=0.7;
#endif
                      } // for y
#ifdef DEBUGLVL       // Визуализация
///                      for (p=s2; p<res+s2+1; p++) {drawData[0][p][x]*=0.7;}
///                      if ( res > res1 ) {drawData[0][s2-2][x]*=0.1;}
#endif                // и выход из цикла
                      if ( res > res1 ) {break;}
                  } // for x
                  // LineVertical(x1, 0x00AA0011);  // темно синий
                  strArray[0][Nstr].wordArray[Nwrd].x1 = x; // +1
/**/
           // сдвиг граници квадрата вокруг буквы вправо, относительно
           // прямоугольника поиска точки.
               // вычисление величины смещение границ полосы подсчета
               // вертикальных сумм в низ до s1, площадь точки не учитываем.
               s1=y1B;  q1=yp-(tFg+1)/2;  q2=y0B+(y1B-y0B)/2;  q3=y0B+(y1B-y0B)*2/5; // tFg+1; TMP
               if ( tFg ) {
               s1=(q1 < q2) ? q1: q2;                     // т.е. y = min(a, b).
               s1=(s1 > q3) ? s1: q3;                     // т.е. y = max(a, b).
               }    

                  // проверка окончания (начала) строки
                  if ( x0B > x1 + MeanSA*2/3 ) { s0=x0B-MeanSA/2; s3=(x0B+x1B)/2;} //s3=(x0B+x1B)/2;

                  // подсчет вертикальных сумм в части квадрата вокруг буквы Nwrd+1
	       if(s0<0)s0=0;		
                  for ( x=s0; x<s3 && x<ncolumns; x++ ) { // горизонтальный цикл
                      res1=res;  res=0;
                      // смещение границ полосы подсчета вертикальных сумм в низ до s1
                      for ( y=y0B; y < s1 && y<nrows; y++ ) { // вертикальный цикл
                          d=y*ncolumns;
                          res+=bits_data[d + x];
#ifdef DEBUGLVL           // Визуализация
///                          drawData[0][y][x]*=0.7;
#endif
                      } // for y
#ifdef DEBUGLVL       // Визуализация
///                      for (p=s2; p<res+s2+1; p++) {drawData[0][p][x]*=0.7;}
///                      if ( res > res1 ) {drawData[0][s2-2][x]*=0.1;}
#endif                // и выход из цикла
                      if ( res > res1 ) {break;}
                  } // for x
                 /// LineVertical(x, 0x0000AA00);  // зеленый
                  strArray[0][Nstr].wordArray[Nwrd+1].x0 = x;
/**/
        } // Nwrd   // Цикл по количеству квадратиков вдоль строки NWord

   } // for Nstr  // Цикл по количеству строк NStro

// OCRTypes.h
// x0,x1, y0,y1      координаты квадрата вокруг буквы
// xp0,xp1, yp0,yp1  координаты прямоугольника вокруг точки
// xp,yp             координаты точки
// xt0,xt1, yt0,yt1  координаты прямоугольника вокруг стека
// tsegFlag  флаг наличия точки в прямоугольнике
// spaceFlag флаг наличия пробела после точки
// stackFlag флаг наличия стека вместо коренной буквы.

#ifdef DEBUGLVL
// Визуализация скорректированных по горизонтали букв темно синими квадратиками //
///                   wordDraw(0x00AA0011);  // темно синий
#endif

/// TIME_PRINT

// if( NStrok<0) { return; }  page   строка line Line
// if (x<xp-tFg/2 || x>xp+tFg/2 || y<yp-tFg/2 || y>yp+tFg/2){

//  y = (a>b) ? a: b;  т.е. y = max(a, b).
//  abs(DeltaK);

//  int x0,x1,xp,xp0,xp1,x0B,x1B;
//  int y0,y1,yp,yp0,yp1,y0B,y1B;


}//_____________________________________________________________________________


                                 // KOR //

//  ----------------------------------------------------------------------------
// Подпрограмма вертикальной коррекции положения квадратиков по низу букв.
//  ----------------------------------------------------------------------------
void GBitset::CorrectLetterLH(){

  int y,x,p,d,m,s;
  int y0,y1,x0,x1,s0,s1,s3,s4;
  int DltNstrN,nrow;//DltNstrNKh
  int NWord,res,maxres; //minres
  int prg, Prg1, Prg2, prgY;//PGrad


       // для визуализации одной строки заремачить цикл по NStrok.
       Nstr=2;  // Nstr-глобальная.

       // вычисление максимальной высоты строки на странице hStrMax-глобальная.
       hStrMax=0;   //  hStrMax-глобальная.
       for ( Nstr=0; Nstr < strArray->size(); Nstr++ ) {
           s=strArray[0][Nstr].y1 - strArray[0][Nstr].y0;
       //    if ( s > hStrMax ) { hStrMax=s; }
           hStrMax=(s > hStrMax) ? s: hStrMax;  // т.е. y = max(a, b).
       } // for Nstr // Цикл по количеству строк

       // вычисление максимального размера вертикального массива BufW
       nrow=hStrMax*2+1;
       vector<int> BufW(nrow);


       // вычисление максимального колличества букв в строке по странице.
       int WordSMax=0, sizeWdS;   //  WordSMax - глобальная.
       for ( Nstr=0; Nstr < strArray->size(); Nstr++ ) {
           s=(uint)strArray[0][Nstr].wordArray.size();
       //    if ( s > WordSMax ) { WordSMax=s; }
           WordSMax=(s > WordSMax) ? s: WordSMax;  // т.е. y = max(a, b).
       } // for Nstr // Цикл по количеству строк

       // вычисление максимального размера массива букв WordS
       vector<int> WordS(WordSMax);


   for ( Nstr=0; Nstr < strArray->size(); Nstr++ ){ // Цикл по количеству строк NStrok
       // получение оптимального коэффициента вертикальной фильтрации для каждой строки
       DltNstrN = strArray[0][Nstr].y1 - strArray[0][Nstr].y0;

         WordS.assign(WordSMax,0);  sizeWdS=m=0;
         NWord = (int)strArray[0][Nstr].wordArray.size();
         for ( Nwrd=0; Nwrd < NWord; Nwrd++ ){ // Цикл по количеству квадратиков NWord

             /// получение координат прямоугольников и точек из структуры по x по y
             wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
             // x0,x1, y0,y1 координаты квадрата вокруг буквы
             x0 = wP->x0;    x1 = wP->x1;    y0 = wP->y0;    y1 = wP->y1;

             // получение области коррекции положения квадратиков по низу букв
             // если s0 меняем, то меняем синхронно с s0 в StatCorrectLetter()
             s0=y0-(y1-y0)/2;       s1=y0 + (y1-y0)/4;
             s3=x0+(x1-x0)/8;       s4=x1-(x1-x0)/8;

             // проверка выхода области коррекции квадратиков за граници массива
             if ( s0 <  1 ){ s0=s1=1; }  if ( s1 >= nrows ){ s0=s1=nrows-1; }
             if ( s3 <  1 ){ s3=s4=1; }  if ( s4 >= ncolumns ){ s3=s4=ncolumns-1; }

             /// заполнение массова коррекции BufW горизонтальными суммами
             p=maxres=0;  BufW.assign(nrow,0);
             for ( y=s0; y < s1; y++ ) {        // вертикальный цикл
                 res=0; d=(y*ncolumns);
                 for ( x=s3; x < s4; x++ ) {    // горизонтальный цикл
                       res+=bits_data[d + x];
// визуализация (серым) области поиска стеков
 ////                  drawData[0][y][x]*=0.8;  // визуализация (серым)
                      } // for x
                 BufW[p]=res<<6;   p++;         // <<6 *64 копирование в BufW
                 if ( p > nrow-2 ) { p=nrow-2; }
                 // получение максимума BufW[p]
                 if( res > maxres ) { maxres=res; } // maxres!=64
             } // for y

             /// первичный отсев прямоугольников в которых заведомо ничего нет
             // для улучшения быстродействия переходим к обработке следующей буквы
             Prg1=(s1-s0)*(s4-s3)/200 + 1;  // порог по нескольким пикселам // Prg1=(s1-s0)/12 + 1;
             Prg2=DeltaKh*(y1-y0)/(abs(x1-x0)+1)/10; // порог по прямоугольности области поиска
             if ( maxres < 1 || Prg2 > 15) { goto NwrdEND; }

             //  сглаживание BufW ФИЛЬТР НИЗКИХ ЧАСТОТ
             SP->filterHigh(BufW,DltNstrN/4);   //  /2 /4
/*
#ifdef DEBUGLVL
// функции рисования вертикальных сглаженных графиков BufW точками
for (y=0; y < p; y++) { drawData[0][y+s0][(BufW[y]/32+s3)]*=0.1; }
#endif
LineVertical((s4+s3)/2, 0x0000AA00);
*/
/*             // получение максимума и минимума сглаживанной функции BufW[у]
             maxres=0; minres=65535;
             for ( y=0; y < p; y++ ) {
                if( BufW[y] > maxres ) { maxres=BufW[y]; }
                if( BufW[y] < minres ) { minres=BufW[y]; }
             } // for y
*/

             // вычисление точки пересечения порога Prg1 с функцией BufW[у]
             prg=32*(s4-s3)*2/10 + 1;  // (s1-s0)*(s4-s3)*6/10 + 1;
             // prg порог по площади области коррекции
             prgY=y0-s0;
             for ( y=0; y < p; y++ ) {
                 if ( BufW[y] > prg) { prgY=y;  break; }  // y+s0;
             } // for y

// отбрасываем самое начало и конец области коррекции ниижней части квадратиков
             if ( (s1-s0)-(s1-s0)/8 > prgY && (s1-s0)/6 < prgY ) // (s1-s0)*7/8
             { WordS[m]=prgY;   m++; }   //  sizeWdS=m;
             // размер выходного массива
             sizeWdS=m;

/**/

/*
#ifdef DEBUGLVL
// функции рисования вертикальных графиков BufW точками  // y < p,nrow
for (y=0; y < p; y++) { drawData[0][y+s0][(BufW[y]/32+s3)]*=0.1; }

// функции рисования порога Prg1 вертикальной  чертой
for (y=0; y < p; y++) { drawData[0][y+s0][(prg/32+s3)]*=0.4; } // y+=2

// рисует горизонтальную черту через точку пересечения порога Prg1 с функцией BufS[у]
for (x=0; x <= s4-s3; x++) { drawData[0][prgY+s0][(x+s3)]*=0.4; }

// DM(END); DM(6666<<" CorrectLetterHL ");
 DM(END); DM(Prg1<<" Prg1 "); DM(Prg2<<" Prg2 "); DM(prg<<" prg "); DM(END);
#endif
*/

            // ппереход к обработке следующей буквы Nwrd  // {break;}
                                 NwrdEND:;

         } // for Nwrd  // Цикл по количеству букв Nwrd

        // Запуск подпрограммы статистики коррекции нижней полки квадратиков
        if ( sizeWdS>2 && sizeWdS<1000 && sizeWdS<=WordSMax ) {
           StatCorrectLetter(WordS, sizeWdS);
        }

   } // for Nstr // Цикл по количеству строк NStrok


/**/
}//_____________________________________________________________________________

                              ////

//  ----------------------------------------------------------------------------
// Запуск подпрограммы статистики коррекции нижней полки квадратиков
//  ----------------------------------------------------------------------------
void GBitset::StatCorrectLetter(vector<int> &WordS, int sizeWdS){


  int y,d,n; // ,p,m
  int s0,y0,y1;
  int NWord,sizeWdSR,DeltaH,Dit;
 // int size=(int)WordS.size();  // size



         // resise массива точно по колличеству данных
         vector<int> WordSR(sizeWdS);
         for ( y=0; y < sizeWdS;  y++ ){ WordSR[y]=WordS[y]; } // WordSR[y]=WordS[y];


                       // Сортировка вектора.
// сортируем наш вектор от начала и до конца. http://progs.biz/cpp/stl/lessons/005.aspx
                      sort(WordSR.begin(), WordSR.end());


         // вычисление среднего смещения нижней полки квадратиков для каждой строки
         // отбрасываем Dit штук самых больших и самых маленьких значений смещения
         Dit=sizeWdS/15+1;    // Dit=2;
         DeltaH=d=0;
         for ( y=Dit; y < sizeWdS-Dit; y++ ){ // Цикл по количеству букв NWord-Dit*2
             WordSR[d]=WordSR[y];  DeltaH+=WordSR[d];  d++;
         } // for y
         // размер выходного массива
         sizeWdSR=d;

         if ( sizeWdSR != 0 ) { DeltaH=10*DeltaH/sizeWdSR; }

         // величина смещения нижней полки квадратиков одинакова для всей строки
         DeltaH=DeltaH/10;

         // коррекция нижней полки квадратиков в структуре
         if ( DeltaH > 0 ) {
             NWord = (int)strArray[0][Nstr].wordArray.size();
             for ( n=0; n < NWord; n++ ){  // Цикл по количеству букв NWord
                 wordOCR *wP=&strArray[0][Nstr].wordArray[n];
                 y0 = wP->y0;  y1= wP->y1;     // чтение из структуры
                 s0=y0-(y1-y0)/2;
                 wP->y0 = s0 + DeltaH;         // запись в структуру
             } // for n
         } // if

    // Визуализация букв темно синими квадратиками из структур  // (вдвойне)
////         if( GrafT[6][6] ) { wordDraw(0x00AA0011); }  // темно синий

/**/
 /*
#ifdef DEBUGLVL
              // ВИЗУАЛИЗАЦИЯ цифровая WordS
              DM(END);
              for ( y=0; y < size;  y++ ){   //  NWord
                DM(WordS[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

              // ВИЗУАЛИЗАЦИЯ цифровая WordSR
              DM(END);
              for ( y=0; y < d;  y++ ){   //  NWord
                DM(WordSR[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);
#endif
*/

/*
#ifdef DEBUGLVL
DM(END); DM(7777<<" StatCorrectLetter ");
DM(END); DM(sizeWdS<<" sizeWdS ");  DM(size<<" size ");
DM(Dit<<" Dit "); DM(DeltaH<<" DeltaH "); DM(END);
#endif
*/

//  y = (a>b) ? a: b;  т.е. y = max(a, b).
//  abs(DeltaK); // {break;}
//  if( Nf==0) { return; }   // y+=2;
//  vector<int> WordS1=WordS;
//  if ( size < 3 ) { DeltaH=0; return; }
}//_____________________________________________________________________________

