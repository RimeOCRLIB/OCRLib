#include "GBitset.h"

////////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------//
// программа получения координат Eng букв в пределах страници (квадратики).   //
// ---------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


// Запуск обработки всех строк страници.
void GBitset::PageWordDetEng() {

  int y,x,m; // ,n
  int y0,y1,s0,s1;
  int res;          // unsigned long; // short  // 2147483647
  //int Lim=2147483647>>8-2;    // DM(Lim<<" Lim ");

TIME_START

   // вычисленное колличество строк в печа - NStrok=strArray.size();
                       NStrok=(uint)strArray->size();
                       if( NStrok<1) { return; }

   // номер строки на графику .
   // m=StrToInt(ImageProcessorMain->Edit5->Text);  //
   //m=36;
   m=2;
   if ( m > NStrok ) { m = NStrok; } // NStrok начинается с 1
   Nstr=m;


  int w=ncolumns,dw;
  bool d,*d1;  // ,*d2
  bool  *A=bits_data;

//  Mean = strArray[0][strInd].MeanSA;
//  dw=w*MeanSA/8;  if ( dw < 1 )  dw=1;



   // Цикл по колличеству строк NStrok
////   for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Nstr, NStrok - глобальные /////////
             // получение координат строк y0, y1 из структуры
             y0=strArray[0][Nstr].y0;   y1=strArray[0][Nstr].y1;
             // вычисленная ширина буквы DeltaNstrKH
             DeltaNstr = strArray[0][Nstr].y1-strArray[0][Nstr].y0;
             dw=w*(DeltaNstr/8+1);   //  DM(DeltaNstr/8+1<<" DeltaNstr/8+1 ");
             // DM(END); DM(Nstr<<" Nstr "); DM(y0<<" y0 "); DM(y1<<" y1 "); DM(END);
             // смещение границ полосы подсчета вертикальных сумм вверх и в низ
             s0=y0-(y1-y0)/3;    if ( s0 < 2 )        s0 = 1; //  /21 <<8; 1/3 <<9;  *2/3;
             s1=y1+(y1-y0)/3;    if ( s1 > nrows-2 )  s1 = nrows-1;
             // подсчет вертикальных сумм в пределах строки, вертикальный фильтр
             for (x=0; x<ncolumns; x++){ // -1
                 res=0;
                 for (y=s0; y<s1; y++){
                     d1=A+y*w+x;      //A[y][x]                      //      -1
                   //d2=A+y*w+x+w;    //A[y+1][x]                    //       1
                     d=*d1-*(d1+dw);
                   ///  d=*(A+y*w+x);  // d=*d1;
                     res+=d;
                     // визуализация частей букв в окошке в виде граници в один пиксел
#ifdef DEBUGLVL_GREY
if ( GrafT[10][1] ) { drawData[0][y][x]=255+d*196; }  // draw it   *(d+x)
#endif
                 }  // for y
                 LStr[x]=res<<9;    // res<<8  *256 копирование в BufS p++;
              }  // for x
// Запуск программы получения координат букв в пределах строки, получнние параметра MeanSA
///             LineBlockStart();
////    }// for Nstr // Цикл по колличеству строк NStrok ////////////////////////

/* */


#ifdef DEBUGLVL
if( GrafT[10][1] ) {
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
/*
  // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
  int M=64; static short delta=nrows*10/40;   delta=40; // 300
  for(x=0; x < ncolumns-1; x++) {
  for (y=delta; y<(LStr[x]/M)+delta; y++)drawData[0][y][x]*=0.1;} // 0.8*ncolumns;

              // вызов горизонтальной функции рисования
//            WDraw(LStr,20,0x00AA0011); // темно синий
*/
}
#endif

if( GrafT[10][0] ) { DM(END);  DM(10101010<<" PageWordDetEng ");  DM(END); }

// Визуализация прямоугольников в структурах для блоков букв
///if( GrafT[10][6] ) { blockDraw(0x000000FF); }  // цвет линии красный

// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
///if( GrafT[10][8] ) { columnDraw(0x00ff00); }  // цвет линии зеленый

TIME_PRINT

//LStr.assign(ncolumns,0);
//if ( res > Lim )  res = Lim;
/**/
}//_____________________________________________________________________________

