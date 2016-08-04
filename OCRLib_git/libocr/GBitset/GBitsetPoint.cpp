#include "GBitset.h"
// -----------------------------------------------------------------------------
// --- Page Point Detector --- GBitsetPoint.h ---
// Подпрограмма  работы с точками в пределах строки.
// -----------------------------------------------------------------------------

void GBitset::pagePointDetector(){


TIME_START

        if( NStrok<0) { return; }


        // Запуск подпрограммы распознавания точек в пределах строки.
                    RecognitionPoint();



        // получили маркер (в структуре) на наличие точки после буквы.

/**/

if( GrafT[5][0] ) { DM(END);  DM(55555<<" pagePointDetector ");  DM(END); }

TIME_PRINT

}//_____________________________________________________________________________


//  ----------------------------------------------------------------------------
// Запуск подпрограммы распознавания точек в пределах строки.
//  ----------------------------------------------------------------------------

void GBitset::RecognitionPoint(){

  int yp0,yp1,xp0,xp1,sx,sy;
  int NWord,MPoint,ms,ns,WPnt,HPnt;

//TIME_START

//Nstr=2;

// распознавание точек внутри прямоугольников (описанных вокруг каждой точки).

   for ( Nstr=0; Nstr < strArray->size(); Nstr++ ){ // Цикл по количеству строк NStrok

         NWord = (int)strArray[0][Nstr].wordArray.size(); // Nstr, Nwrd - глобальные
         for ( Nwrd=0; Nwrd < NWord; Nwrd++ ){ // Цикл по количеству квадратиков вдоль строки NWord

             /// получение координат прямоугольников из структуры по x по y
             wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
             xp0= wP->xp0;   xp1 = wP->xp1;   yp0= wP->yp0;   yp1 = wP->yp1;

             // в прямоугольниках находящихся ближе чем высота/ширина
             // (или 16 пикселей) буквы к краям печа, точки не ищем
             sy=(yp1-yp0)/2;   if ( sy < 8 )  sy = 8;  // /1, 16,16
             sx=(xp1-xp0)/2;   if ( sx < 8 )  sx = 8;
             //sx=sy=DeltaNstr;

             // Вызов блока подпрограмм для распознавания точек в пределах прямоугольника.
             if ( wP->stackFlag == 0 &&
                  yp0 > sy && yp1 < nrows-sy && xp0 > sx && xp1 < ncolumns-sx 
                ){
                  // Вызов подпрограммы нахождения координат X,Y отдельно стоящей точки.
                  XYPointLocal(xp0, xp1, yp0, yp1);
                  // Вызов подпрограммы нахождения отдельно стоящей несклеенной точки.
                  CorePointLocal(xp0, xp1, yp0, yp1);
                } // yp0 >

         } // Nwrd   // Цикл по количеству квадратиков вдоль строки NWord


         // Запуск подпрограммы статистики размера точек в пределах строки.
                        StatPointLocal( &WPnt,&HPnt, &MPoint, &ms, &ns );
         // DM(END);  DM(MPoint<<" MPoint "); DM(ms<<" ms "); DM(ns<<" ns ");  DM(END);


         // Запуск подпрограммы A маски распознавания точек в пределах прямоугольника.
         NWord = (int)strArray[0][Nstr].wordArray.size(); // Nstr, Nwrd - глобальные
         for ( Nwrd=0; Nwrd < NWord; Nwrd++ ){ // Цикл по количеству квадратиков вдоль строки NWord

             /// получение координат прямоугольников из структуры по x по y
             wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
             xp0= wP->xp0;   xp1 = wP->xp1;   yp0= wP->yp0;   yp1 = wP->yp1;

             // в прямоугольниках находящихся ближе чем высота/ширина
             // (или 8 пикселей) буквы к краям печа, точки не ищем
             sy=(yp1-yp0)/2;   if ( sy < 8 )  sy = 8;
             sx=(xp1-xp0)/2;   if ( sx < 8 )  sx = 8;
             //sx=sy=DeltaNstr;
             // Вызов подпрограммы маски распознавания точек в пределах прямоугольника.
             if ( wP->stackFlag == 0 &&
                  yp0 > sy && yp1 < nrows-sy && xp0 > sx && xp1 < ncolumns-sx ) {
                  // Вызов подпрограммы A маски распознавания точек в пределах прямоугольника.
                  MatrixPointA( MPoint, ms, ns );
                  ///MatrixPoint( WPnt, HPnt );
             } // yp0 >

             /// проверка совпадения флага пробела по адресу (Nwrd) и точки (Nwrd+1)

         } // Nwrd   // Цикл по количеству квадратиков вдоль строки NWord

  } // for Nstr  // Цикл по количеству строк NStrok




/**/
        // Визуализация распознанных точек зелеными прямоугольниками
#ifdef DEBUGLVL
if( GrafT[5][1] ) { pointDraw(0x000000FF); }  // красный
#endif

//TIME_PRINT

/*
          /// проверка совпадения флага пробела по адресу (Nwrd) и точки (Nwrd+1)
          if ( Nwrd < NWord-1 ) { // NWord-1
            // tFg флаг наличия точки в прямоугольнике
            tFg = strArray[0][Nstr].wordArray[Nwrd+1].tsegFlag;
            // sFg флаг наличия пробела после буквы
            sFg = strArray[0][Nstr].wordArray[Nwrd].spaceFlag;
            if ( tFg!=0 && sFg!=0 ) {
             strArray[0][Nstr].wordArray[Nwrd].spaceFlag = 0;
             strArray[0][Nstr].wordArray[Nwrd+1].spaceFlag = sFg;
            } // tFg!=0 && sFg!=0
          } // Nwrd < NWord
*/

//                  yp0=0; yp1=0;  xp0=0; xp1=0;
         // запись области поиска точек - прямоугольников в структуру
//         wP->xp0 = xp0;   wP->xp1 = xp1;   wP->yp0 = yp0;   wP->yp1 = yp1;

}//__________________________________________________________________________

////////////////////////////////////////////////////////////////////////////////

                             //  X,Y  //

//  ----------------------------------------------------------------------------
// Запуск подпрограммы нахождения координат X,Y отдельно стоящей точки.
//  ----------------------------------------------------------------------------

void GBitset::XYPointLocal( int xp0, int xp1, int yp0, int yp1 ){

  int x,y,n,d,g,c; ///
  int wp,dx,mx,my,xIm;  //  ,yRe
  int w=ncolumns;
  bool *d1,*d2;
  bool  *A=bits_data;
   // нахождение X,Y отдельно стоящей точки XYPointLocal

   // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
   memset(ReBuf,0,4*(ncnr+512));      memset(ImBuf,0,4*(ncnr+512)); ///

   m=6; // max количество параметров на одну точку в массмве ReBuf

   // Оптимальная обработка ширины точки
   c=g=0; /// c=0;
   for (y=yp0; y<yp1; y++){       //+1  +1
       d1=A+y*w;                  //A[y][x]   // d1=A+y*w;
       d2=d1+1;                   //A[y][x+1]
       mx=xp0;
       for (x=xp0; x<xp1; x++){   // xp1-1
           d=*(d2+x)-*(d1+x);     // delta=ImBuf[x+1]-ImBuf[x];
           if ( d )  {  // !=0    // !=0  считаем только в точках изменения
              if ( d > 0 )  { mx=x; }
              // координаты середины горизонтальной средней полосы по y, по x1, x0
              else {
               ImBuf[g]=y;  ImBuf[g+1]=x;  ImBuf[g+2]=mx;   g+=m;
              // линейная координата "c" середины горизонтальной полосы по x
              /// ReBuf[c-(x-mx)/2]=1;
              } // d > 0
           } // !=0
#ifdef DEBUGLVL_GREY
if( GrafT[5][1] ) { drawData[0][y][x]*=0.8; } // визуализация плашки (серым) ///
#endif        
          /// c++;
       }  // for x
   }  // for y

   ImBf=g;

/**/
//DM(END); DM(g1<<" g1 ");  g1=



  // Оптимальная обработка высоты точки
  // вертикальные полосы проводим только через центры горизонтальных полос
  g=0; wp=xp1-xp0;
  for (n=0; n<ImBf; n+=m){
    xIm=ImBuf[n+1];  mx=ImBuf[n+2];
    dx=xIm - mx;                // dx >= 0 && // dx=ImBuf[n+3];
    if ( dx < (yp1-yp0)-4 )  {  // проверка длинны горизонтальных полосы по x
       x=(xIm + mx + 2)/2;      //yIm=ImBuf[n]; // x=ImBuf[n+1];   + 0
       d1=A+x;                  //A[y][x]
       my=yp0; // yp1
       for (y=yp0; y<yp1; y++){
           d2=d1+y*w;        //A[y+1][x]
           d=*(d2+w)-*d2;    // delta=ImBuf[y+w]-ImBuf[y];
           if ( d )  {       // !=0  считаем только в точках изменения
              if ( d > 0 )  { my=y; }  // my предидущая координата y
              else {
                  /// определение координаты пересечения вертикальной и
                  /// горизонтальной средней полосы по линейной координате "c" ( хеш )
                  ///c = (x-xp0) + (y-yp0-(y-my)/2)*wp;     //  BufS[c]=1;
                  ///BufS[c]=ReBuf[c];
                  //yRe=(y + my + 2)/2;  // >>1
                  if ( ImBuf[n]==(y + my + 2)/2 )  { // если совпадают игреки средней  + 0
                  ///if ( ReBuf[c] )  {  ///
                  // определение координаты пересечения вертикальной и
                  // горизонтальной средней полосы по координатам x, y,
                  ReBuf[g]=x;          ReBuf[g+1]=y+1;           ReBuf[g+2]=my;
                  ReBuf[g+3]=ImBuf[n]; ReBuf[g+4]=ImBuf[n+1]+1;  ReBuf[g+5]=ImBuf[n+2];
                  g+=m;
                 } // ImBuf[n]
             } // d > 0
           } // !=0
       }  // for y
    }  // x > 2
  }  // for n

  ReBf=g;
/**/

///   n+1 - верх y1,   n+2 - низ y0,   n+4 - право x1,    n+5 - лево x0   ///

#ifdef DEBUGLVL_GREY
if( GrafT[5][1] ) {
   for (x=0; x<ImBf; x+=m){  // y  x +1
   // визуализация вертикальной полосы в виде кривой в один пиксел
    drawData[0][ImBuf[x]][(ImBuf[x+1]+ImBuf[x+2])/2]*=0;   // *=.0-черная *= .9-серая
   }  // for x                                //  =1 -черная  =255-белая
   for (x=0; x<ReBf+1; x+=m){
   // визуализация горизонтальной полосы в виде кривой в один пиксел
    drawData[0][ReBuf[x+1]][ReBuf[x]]*=0;
   // визуализация центра распознаваемой точки в виде белого пикселя
///       drawData[0][ReBuf[x+1]][ReBuf[x]]=255;
   }  // for x
}
#endif
/**/

/*
#ifdef DEBUGLVL
// отработка доступа к массиву по разным системам координат
// визуализация вертикальной  полосы в виде кривой в один пиксел
   c=0;
   for (y=yp0; y<yp1; y++){
       for (x=xp0; x<xp1; x++){
       /// обращение к массиву по декартовым координатам X,Y
       c=(x-xp0) + (y-yp0)*wp;
       if (BufS[c]) {drawData[0][y][x]=255;} // drawData[0][y][x]*=1-BufS[c];
       /// обращение к массиву по линейной координате "c"
       // c++;
       }  // for x
   }  // for y
#endif
*/

}//_____________________________________________________________________________


                             //  CORE  //

//  ----------------------------------------------------------------------------
// Запуск подпрограммы нахождения отдельно стоящей несклеенной точки.
//  ----------------------------------------------------------------------------

void GBitset::CorePointLocal( int xp0, int xp1, int yp0, int yp1 ){

  int n,d;  // ,k ,g1,g2,mx,my

  //int size=(xp1-xp0)*(yp1-yp0);
  ////int *BufS=(int*)calloc(size+32,szf);    // запрос памяти min с очисткой 0
  ////if ( ImBuf !=NULL ) free(BufS);
  ////if (BufS==NULL) exit (1);



        // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
///   memset(ImBuf,0,szf*(ncnr+512));      memset(ReBuf,0,szf*(ncnr+512));

 ///   n+1 - верх y1,   n+2 - низ y0,   n+4 - право x1,    n+5 - лево x0   ///

   // проверка соприкосновения границ точки с прямоугольником поиска точки
   // и удаление блоков соприкасающихся с тем же прямоугольником
   d=0;
   for (n=0; n<ReBf; n+=m){
    if ( ReBuf[n+1]<yp1 && ReBuf[n+2]>yp0 && ReBuf[n+4]<xp1 && ReBuf[n+5]>xp0 )
       { ReBuf[d]=ReBuf[n]; ReBuf[d+1]=ReBuf[n+1]; ReBuf[d+2]=ReBuf[n+2];
         ReBuf[d+3]=ReBuf[n+3]; ReBuf[d+4]=ReBuf[n+4]; ReBuf[d+5]=ReBuf[n+5];  d+=m; }
   }  // for n

   for (n=d; n<ReBf+1; n+=m){
      ReBuf[n]=ReBuf[n+1]=ReBuf[n+2]=ReBuf[n+3]=ReBuf[n+4]=ReBuf[n+5]=0;
   }  // for n

   ReBf=d;


   wordOCR *wT=&strArray[0][Nstr].wordArray[Nwrd];
   for (n=0; n<ReBf; n+=m){
       TsertoOCR tp;
       // координаты середины горизонтальной средней полосы точки по y, по x1, x0
       tp.y=ReBuf[n+3];  tp.x1=ReBuf[n+4];  tp.x0=ReBuf[n+5];
       // координаты середины вертикальной средней полосы точки по x, по y1, y0
       tp.x=ReBuf[n];    tp.y1=ReBuf[n+1];  tp.y0=ReBuf[n+2];
       // ширина и высота точки ( габармты )
       tp.w=ReBuf[n+4]+1-ReBuf[n+5];  tp.h=ReBuf[n+1]-ReBuf[n+2]+1;
       wT->TLline.push_back(tp);
//     int a=wT->TLline[0].x0;    // int a=wT[0].TLline[0].x0;
//     int b=wT->TLline[0].w;
   }  // for n
/**/
   


#ifdef DEBUGLVL
/*
// блокирует через yp0,yp1,xp0,xp1 функцию MatrixPointA();
// запись координат области точки в структуру для рисования прямоугольника точки
         wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
         for (n=0; n<ReBf; n+=m){
              wP->xp0 = ReBuf[n+5];      wP->xp1 = ReBuf[n+4]+1;
              wP->yp0 = ReBuf[n+2]-1;    wP->yp1 = ReBuf[n+1];
///           DM(ReBuf[n+4]+1-ReBuf[n+5]<<" xp ");
///           DM(ReBuf[n+1]-ReBuf[n+2]+1<<" yp ");
         }  // for n
///      DM(END);
*/
/*
// визуализация области распознавания точки в виде 4 белох пикселей с помощью ReBuf
         for (n=0; n<ReBf; n+=m){
             drawData[0][ReBuf[n+3]][ReBuf[n+4]]=255;
             drawData[0][ReBuf[n+3]][ReBuf[n+5]]=255;
             drawData[0][ReBuf[n+1]][ReBuf[n]]=255;
             drawData[0][ReBuf[n+2]][ReBuf[n]]=255;
         }  // for n
*/
/*
// визуализация области распознавания точки в виде 4 белох пикселей с помощью TsertoOCR
         for (n=0; n<wT->TLline.size() n++){   // ReBf/m;
             y=wT->TLline[n].y;  x1=wT->TLline[n].x1;  x0=wT->TLline[n].x0;
             x=wT->TLline[n].x;  y1=wT->TLline[n].y1;  y0=wT->TLline[n].y0;
             drawData[0][y][x1]=255;    drawData[0][y][x0]=255;
             drawData[0][y1][x]=255;    drawData[0][y0][x]=255;
         }  // for n     int a=wT[0].TLline[0].x0;
*/
/*
// визуализация области распознавания точки в виде 4 белох пикселей с помощью TsertoOCR
         for (n=0; n<wT->TLline.size(); n++){
             drawData[0][wT->TLline[n].y][wT->TLline[n].x1]=255;
             drawData[0][wT->TLline[n].y][wT->TLline[n].x0]=255;
             drawData[0][wT->TLline[n].y1][wT->TLline[n].x]=255;
             drawData[0][wT->TLline[n].y0][wT->TLline[n].x]=255;
         }  // for n
*/
#endif

///   n+1 - верх y1,   n+2 - низ y0,   n+4 - право x1,    n+5 - лево x0   ///

///         vector<int> line(10);
///         int a=1,b=2;
///         //line.push_back(a);
///         //line.push_back(b);
///         line[0]=b;
///         line[1]=a;

/**/

////if ( ImBuf !=NULL ) free(BufS);

//  проверка соприкосновения левой граници точки с левым краем прямоугольника
//     if (ReBuf[n+5]<=xp0 || ReBuf[n+4]>=xp1) {ReBuf[n+4]=ReBuf[n+5]=0;}

/*
      /// получение координат прямоугольников из структуры по x по y
      wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
      xp0= wP->xp0;   xp1 = wP->xp1;   yp0= wP->yp0;   yp1 = wP->yp1;
*/

}//_____________________________________________________________________________


                             //  STAT  //

//  ----------------------------------------------------------------------------
// Запуск подпрограммы статистики размера точек в пределах строки.
//  ----------------------------------------------------------------------------
void GBitset::StatPointLocal( int* WPnt, int* HPnt, int* MPoint, int* ms, int* ns ){

  int x,y,n,d,p;
  int w,h,delta,WPoint,HPoint;  //
  int y0,y1,x0,x1;
  //int yp0,yp1,xp0,xp1;

      // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
   memset(ReBuf,0,szf*(ncnr+512));      memset(ImBuf,0,szf*(ncnr+512)); ///
   
///   DM(END);
   d=0;
   int NWord = (int)strArray[0][Nstr].wordArray.size(); // Nstr, Nwrd - глобальные
   for ( Nwrd=0; Nwrd < NWord; Nwrd++ ){ // Цикл по количеству квадратиков вдоль строки NWord
         wordOCR *wT=&strArray[0][Nstr].wordArray[Nwrd];
         for (n=0; n<wT->TLline.size(); n++){
             y=wT->TLline[n].y;  x1=wT->TLline[n].x1;  x0=wT->TLline[n].x0;
             x=wT->TLline[n].x;  y1=wT->TLline[n].y1;  y0=wT->TLline[n].y0;
             ReBuf[d]=w=wT->TLline[n].w;  ImBuf[d]=h=wT->TLline[n].h;  d++;
///         DM(w<<" w ");   DM(h<<" h ");

// визуализация области распознавания точки в виде 4 белох пикселей с помощью TsertoOCR
#ifdef DEBUGLVL_GREY
if ( GrafT[5][1] ) {
             drawData[0][y][x1]=255;    drawData[0][y][x0]=255;
             drawData[0][y1][x]=255;    drawData[0][y0][x]=255;
}
#endif
         }  // for n     int a=wT[0].TLline[0].x0;
   } // Nwrd   // Цикл по количеству квадратиков вдоль строки NWord
/**/

   vector<int> BufW(d);    vector<int> BufH(d);

   for (n=0; n<d; n++){
       BufW[n]=ReBuf[n];  BufH[n]=ImBuf[n];    //   BufW=ReBuf;     BufH=ImBuf;
   }  // for n
/**/
                           // Сортировка вектора.
// сортируем наш вектор от начала и до конца. http://progs.biz/cpp/stl/lessons/005.aspx
         sort(BufW.begin(), BufW.end());      sort(BufH.begin(), BufH.end());
                 // работает также как "сортировка Шейкера".

   delta=d/10+1;    if ( d<4 ) { delta=0; }  //
   WPoint=HPoint=p=0;
   for (n=delta; n<d-delta; n++){
       WPoint+=BufW[n];  HPoint+=BufH[n];   p++;
   }  // for n

   if ( p ) { WPoint=WPoint/p;  HPoint=HPoint/p; }   // != 0

if ( GrafT[5][1] ) {
DM(END); DM(WPoint<<" WPoint "); DM(HPoint<<" HPoint "); DM(p<<" p "); DM(END);
}

      // определение параметров  MPoint, ms, ns по высоте и ширине точки

 //              1   3   5   7
 signed char Tabl[8]={1,1,3,3,5,5,7,7};
 int MaxPnt,MsPnt,msp,nsp;

      MsPnt=0; msp=1; nsp=1;
      if ( WPoint<1 || HPoint<1 )  { goto StatPointEND; }

      MsPnt=7;
      if ( WPoint<8 && HPoint<8 ) {
         MaxPnt=HPoint;  if ( WPoint>HPoint ) MaxPnt=WPoint;
         MsPnt=Tabl[MaxPnt];
         }

      // ms, ns;  масштаб по высоте, ширине 1, 2, 3, 4
      if ( WPoint>7 ) nsp=2;     if ( WPoint>14 ) nsp=3;   if ( WPoint>21 ) nsp=4;
      if ( HPoint>7 ) msp=2;     if ( HPoint>14 ) msp=3;   if ( HPoint>21 ) msp=4;

/**/

                       // выход из подпрограммы
                             StatPointEND:;

   // возврат
   *WPnt = WPoint; *HPnt = HPoint;   *MPoint = MsPnt; *ms = msp; *ns = nsp;



/*
#ifdef DEBUGLVL
if ( GrafT[5][2] ) {
// визуализация области распознавания точки в виде 4 белох пикселей с помощью TsertoOCR
         for (n=0; n<wT->TLline.size(); n++){
             drawData[0][wT->TLline[n].y][wT->TLline[n].x1]=255;
             drawData[0][wT->TLline[n].y][wT->TLline[n].x0]=255;
             drawData[0][wT->TLline[n].y1][wT->TLline[n].x]=255;
             drawData[0][wT->TLline[n].y0][wT->TLline[n].x]=255;
         }  // for n
//  DM(END);  DM(MsPnt<<" MPnt ");  DM(msp<<" msp "); DM(nsp<<" nsp "); DM(END);
}
#endif
*/

 ///   n+1 - верх y1,   n+2 - низ y0,   n+4 - право x1,    n+5 - лево x0   ///

///         vector<int> line(10);
///         int a=1,b=2;
///         //line.push_back(a);
///         //line.push_back(b);
///         line[0]=b;
///         line[1]=a;

/**/


}//_____________________________________________________________________________

                      // MatrixPoint //

//  ----------------------------------------------------------------------------
// Запуск подпрограммы матрицы распознавания точек в пределах прямоугольника.
//  ----------------------------------------------------------------------------

void GBitset::MatrixPoint( int WPnt, int HPnt ){


  int x,y,n,d;
  //int wp,hp,mx,my;   // ,xIm,yRe
  int yp0,yp1,xp0,xp1;
  //int WPoint,HPoint;
  int w=ncolumns;
  bool *d1,*d2;
  bool  *A=bits_data;
  //HPnt=HPnt+1;
  //int *Hs1=(int*)malloc(4*HPnt+1);    // запрос памяти min без очистки 0
  signed char *Hs1=(signed char*)malloc(HPnt+1);      //* sizeof(signed char));  запрос памяти min без очистки 0
  if (Hs1==NULL) exit (1);


      for (x=0; x<HPnt; x++){ Hs1[x]=11; }  // for x


      /// получение координат прямоугольников из структуры по x по y
      wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
      xp0= wP->xp0;   xp1 = wP->xp1;   yp0= wP->yp0;   yp1 = wP->yp1;

   // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
//   memset(ReBuf,0,4*(ncnr+512));      memset(ImBuf,0,4*(ncnr+512)); ///

//DM(END);  DM(WPnt<<" WPnt ");  DM(HPnt<<" HPnt ");  DM(END);

   // Оптимальная обработка ширины точки
   // g=0;
for ( n=0; n < 1000; n++ ) { // 1000000
   for (y=yp0; y<yp1; y++){
       d1=A+y*w;                  //A[y][x]   // d1=A+y*w;
       d2=d1+1;                   //A[y][x+1]
            for (x=xp0; x<xp1; x++){
            d=*(d2+x)-*(d1+x);         // delta=ImBuf[x+1]-ImBuf[x];
/*
               for (z=0; z<HPnt; z++){
//                   Hs1[z]=11;
               }  // for z
*/
//        drawData[0][y][x]*=0.8; // визуализация плашки (серым) ///

       }  // for x
   }  // for y
}  // for n
   //ImBf=g;



//освобождение памяти
free(Hs1);
/*
которая могла бы по номеру бита выдавать равен ли он 0 или 1.
Т.к. тип Integer - 32-бита имеет в себе, то естесвенно что число любое - это набор 0 и 1 в пределах 32-х бит.

00000000000000000000000000000000 - это 0
00000000000000000000000000000001 - это 1
00000000000000000000000000000010 - это 2
....
00000000000000010000000000000000 - это 32768
*/
}//_____________________________________________________________________________



//  ----------------------------------------------------------------------------
// Запуск подпрограммы A маски распознавания точек в пределах прямоугольника.
//  ----------------------------------------------------------------------------

void GBitset::MatrixPointA( int MPoint, int ms, int ns ){


int y,x;
 int yp0,yp1,xp0,xp1;
 int w=ncolumns;    // ширина, высота  масштаб по ширине, высоте
 int Fg;   // tsegFlag флаг на на наличие точки после буквы
                            // Fg сумма внутри квадратика поиска точки,
                            // PorogFg порог различения точки
 bool *p;
 int b,b3,b5,b7,b9;         // суммы внутри квадратиков разного размера
 int qW,qH,qP,qL;
 int b5W,b5H,b5P,b5L;
 int b7W,b7H,b7P,b7L;
 int b9W,b9H,b9P,b9L;
 //int PrgS,PrgW,PrgH,PrgP,PrgL;    // пороги склеивания точки, соответственно с четырех сторон, в пикселях
 int m,m2,m3,m4,n,n2,n3,n4;       // ms,ns, масштаб по высоте ms, ширине ns
 //int MaxPt,KForm;
 bool  *A=bits_data;	
 //              1   3   5   7
 //char Tabl[8]={1,1,3,3,5,5,7,7};

 // int test=0xFFFFFF;

//TIME_START

                         //    -1-1-1-1-1
        //    -1-1-1     //    -1 1 2 1-1
        //    -1 8-1     //    -1 2 4 2-1                   16*18
        //    -1-1-1     //    -1 1 2 1-1
                         //    -1-1-1-1-1

      // Fg сумма внутри квадратика поиска точки, PorogFg порог различения точки

                         if( MPoint<2) { return; }

      /// получение координат прямоугольников из структуры по x по y
      wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
      xp0= wP->xp0;   xp1 = wP->xp1;   yp0= wP->yp0;   yp1 = wP->yp1;


/*
      // определение параметров  MPoint, ms, ns по высоте и ширине точки
      if ( WPoint<1 || HPoint<1 )  { goto MtxPointEND; }

      MPoint=7;
      if ( WPoint<8 && HPoint<8 ) {
         MaxPt=HPoint;  if ( WPoint>HPoint ) MaxPt=WPoint;
         MPoint=Tabl[MaxPt];
         }

      // ms, ns;  масштаб по высоте, ширине 1, 2, 3, 4
      ms=1; ns=1;
      if ( WPoint>7 ) ns=2;     if ( WPoint>14 ) ns=3;   if ( WPoint>21 ) ns=4;
      if ( HPoint>7 ) ms=2;     if ( HPoint>14 ) ms=3;   if ( HPoint>21 ) ms=4;
*/

      m=w*ms; m2=m*2; m3=m*3; m4=m*4;   // масштаб по высоте   ms=1;
      n=ns;   n2=n*2; n3=n*3; n4=n*4;   // масштаб по ширине   ns=1;

//   DM(END);  DM(MPoint<<" MPoint ");  DM(ms<<" ms "); DM(ns<<" ns "); DM(END);


// конкретные конфигурации
// ms=2; ns=1;  if ( Fg>20 && b9W<2 && b9H<2 && b9P<3 && b9L<3 // ms2; 0009_ris_test.jpg
// ms=1; ns=1;  if ( Fg>20 && b9W<2 && b9H<2 && b9P<4 && b9L<4 // ms2; 0006_NEW_POINT.tiff
//------------------------------------------------------------------------------

      // краевые эффекты для этих циклов учтены в функции RecognitionPoint
      for ( y=yp0; y < yp1; y++ ) {    // вертикальный цикл
        for ( x=xp0; x < xp1; x++ ) {  // горизонтальный цикл


       // набор блоков
       ///////////////////////////////
       if ( MPoint > 2 )
       {
         p=A+y*w+x;  // адрес A+y*w+x
         b=*p;   // b=*(A+y*w+x);
         b3=*(p+m)+*(p-m)+*(p+n)+*(p-n)+*(p+m+n)+*(p-m+n)+*(p+m-n)+*(p-m-n);// * значение по адресу


         // + поиск точки  1х1, (бордюр вокруг точки 3х3, b3 )
         Fg=b;     // Fg сумма внутри квадратика поиска точки
         if ( Fg==1 && b3==0 ) {
         strArray[0][Nstr].wordArray[Nwrd].tsertoFlag =1;
         // получили маркер (в структуре) на наличие точки после буквы.
         //goto MtxPointEND;
         }  // if
/**/
       }

       ///////////////////////////////
       if ( MPoint > 4 )
       {
         // количество слипшихся в блоках пикселей по ...
         b5W=*(p+m2)+*(p+m2+n)           +*(p+m2-n)+*(p+m2-n2);//вертикали сверху 1 (n)
         b5H=*(p-m2)+*(p-m2+n)+*(p-m2+n2)+*(p-m2-n);           //вертикали снизу  1 (n)
         b5P=*(p+n2)+*(p+m+n2)+*(p+m2+n2)+*(p-m+n2);           //горизонтали справа 2 (m)
         b5L=*(p-n2)+*(p+m-n2)           +*(p-m-n2)+*(p-m2-n2);//горизонтали слева  2 (m)
         b5 = b5W + b5H + b5P + b5P;
         // добавление в блоки ранее удаленных из углов квадрата двойных точек
         qW=*(p+m2+n2);  qH=*(p-m2-n2); qP=*(p-m2+n2); qL=*(p+m2-n2);
         b5W+=qW;  b5H+=qH;  b5P+=qP;  b5L+=qL;

         // + поиск точки 3х3,(бордюр 5х5 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3;   // Fg сумма внутри квадратика поиска точки
         if ( Fg>3 && b5W<2 && b5H<2 && b5P<2 && b5L<2 ) { // малый page07_point.jpg
         strArray[0][Nstr].wordArray[Nwrd].tsertoFlag = 3;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         // получили маркер (в структуре) на наличие точки после буквы.
         #ifdef DEBUGLVL_GREY
         drawData[0][y][x]=1;  // draw it
         #endif
         //goto MtxPointEND;
         }  // if
/**/
       }
       ///////////////////////////////
       if ( MPoint > 6 )
       {
         b7W=*(p+m3)+*(p+m3+n)+*(p+m3+n2)           +*(p+m3-n)+*(p+m3-n2)+*(p+m3-n3); //верх
         b7H=*(p-m3)+*(p-m3+n)+*(p-m3+n2)+*(p-m3+n3)+*(p-m3-n)+*(p-m3-n2);            //низ
         b7P=*(p+n3)+*(p+m+n3)+*(p+m2+n3)+*(p+m3+n3)+*(p-m+n3)+*(p-m2+n3);            //право
         b7L=*(p-n3)+*(p+m-n3)+*(p+m2-n3)           +*(p-m-n3)+*(p-m2-n3)+*(p-m3-n3); //лево
         b7 = b7W + b7H + b7P + b7P;
         qW=*(p+m3+n3);  qH=*(p-m3-n3); qP=*(p-m3+n3); qL=*(p+m3-n3);
         b7W+=qW;  b7H+=qH;  b7P+=qP;  b7L+=qL;

         // поиск точки 5х5,(бордюр 7х7 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3 + b5;     // Fg сумма внутри квадратика поиска точки
         if ( Fg>10 && b7W<2 && b7H<2 && b7P<3 && b7L<3  // средний 0022.jpg  9 2 3
         //if ( Fg>10 && b7W<3 && b7H<3 && b7P<5 && b7L<5    // ?? .jpg
                     ) {
         strArray[0][Nstr].wordArray[Nwrd].tsertoFlag = 5;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         #ifdef DEBUGLVL_GREY
         drawData[0][y][x]=1;  // draw it
         #endif
         //goto MtxPointEND;
         }  // if
/**/
       }
       ///////////////////////////////
       if ( MPoint > 8 )
       {
         // используем после автоопределения размеров и пропорции точки

         //ms=2;  m=w*ms; m2=m*2; m3=m*3; m4=m*4; //для запуска разреженных точек по вертикали  ms=2;
         //ns=1;  n=ns;   n2=n*2; n3=n*3; n4=n*4; //для запуска разреженных точек по горизонтали ns=2;

         b9W=*(p+m4)+*(p+m4+n)+*(p+m4+n2)+*(p+m4+n3)           +*(p+m4-n)+*(p+m4-n2)+*(p+m4-n3)+*(p+m4-n4); //верх
         b9H=*(p-m4)+*(p-m4+n)+*(p-m4+n2)+*(p-m4+n3)+*(p-m4+n4)+*(p-m4-n)+*(p-m4-n2)+*(p-m4-n3);            //низ
         b9P=*(p+n4)+*(p+m+n4)+*(p+m2+n4)+*(p+m3+n4)+*(p+m4+n4)+*(p-m+n4)+*(p-m2+n4)+*(p-m3+n4);            //право
         b9L=*(p-n4)+*(p+m-n4)+*(p+m2-n4)+*(p+m3-n4)           +*(p-m-n4)+*(p-m2-n4)+*(p-m3-n4)+*(p-m4-n4); //лево
         b9 = b9W + b9H + b9P + b9P;
         qW=*(p+m4+n4);  qH=*(p-m4-n4); qP=*(p-m4+n4); qL=*(p+m4-n4);
         b9W+=qW;  b9H+=qH;  b9P+=qP;  b9L+=qL;

         // поиск точки 7х7,(бордюр 9х9 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3 + b5 + b7;     // Fg сумма внутри квадратика поиска точки
    //   if ( Fg>24 && b9W<3 && b9H<3 && b9P<5 && b9L<5 // ns=2; 0037_DUDJOM_LINGPA_POINT.jpg
    //   if ( Fg>20 && b9W<4 && b9H<4 && b9P<5 && b9L<5 // ns=1; большой DzogchenNendro_Page_021.png
    ///  if ( Fg>20 && b9W<2 && b9H<2 && b9P<3 && b9L<3 // ms2; 009_.jpg
         if ( Fg>20 && b9W<2 && b9H<2 && b9P<4 && b9L<4 // ms2; 0009_ris_test.jpg
                     ) {
         strArray[0][Nstr].wordArray[Nwrd].tsertoFlag = 7;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         #ifdef DEBUGLVL_GREY
         drawData[0][y][x]=1;  // draw it
         #endif
         //goto MtxPointEND;
         }  // if
/**/
       }

         }  // for x
   } // for y

//------------------------------------------------------------------------------

/**/

#ifdef DEBUGLVL
/*     // визуализация //
 for(x=0; x < ncolumns; x++) {
 for ( y=(yp0+yp1)/2-8; y < BufS[x]+(yp0+yp1)/2-8; y++ ) drawData[0][x][y]*=0.8; }
 BufS.assign(ncolumns,0);
*/
// DM(END);  DM(WPoint<<" WPoint "); DM(HPoint<<" HPoint ");   DM(END);
#endif

//TIME_PRINT

                 // выход из подпрограммы распознавания точек
                               MtxPointEND:;


/*
         // поиск точки 3х3,(бордюр вокруг точки 5х5, b5 состоит из bW,bH,bP,bP)
         Fg=b + b3;     // Fg сумма внутри квадратика поиска точки
         if ( Fg>PorogFg && bW<=PrgW && bH<=PrgH && bP<=PrgP && bL<=PrgL ) {
         strArray[0][Nstr].wordArray[Nwrd].tsegFlag = 3;
         // получили маркер (в структуре) на наличие точки после буквы.
         #ifdef DEBUGLVL
         drawData[0][y][x]=1;  // draw it
         #endif
         goto MtxPointEND;
         }  // if
*/
         //ms=1;  m=w*ms; m2=m*2; m3=m*3; m4=m*4; //для запуска разреженных точек
         //ns=1;  n=ns;   n2=n*2; n3=n*3; n4=n*4; //для запуска разреженных точек

// int MtxPointEND;    // метка выхода из подпрограммы распознавания точек
}//_____________________________________________________________________________


//  ----------------------------------------------------------------------------
// Запуск подпрограммы B маски распознавания точек в пределах прямоугольника.
//  ----------------------------------------------------------------------------

void GBitset::MatrixPointB(){

 int y,x;
 int yp0,yp1,xp0,xp1;
 int w=ncolumns;    // ширина, высота  масштаб по ширине, высоте
 int Fg;   // tsegFlag флаг на на наличие точки после буквы
                            // Fg сумма внутри квадратика поиска точки,
                            // PorogFg порог различения точки
 bool *p;
 int b,b3,b5,b7,b9;         // суммы внутри квадратиков разного размера
 int qW,qH,qP,qL;
 int b5W,b5H,b5P,b5L;
 int b7W,b7H,b7P,b7L;
 int b9W,b9H,b9P,b9L;
 //int PrgS,PrgW,PrgH,PrgP,PrgL;    // пороги склеивания точки, соответственно с четырех сторон, в пикселях
 int ms,ns,m,m2,m3,m4,n,n2,n3,n4; // масштаб по высоте ms, ширине ns
 bool  *A=bits_data;
	// int MtxPointEND;              // метка выхода из подпрограммы распознавания точек
 // int test=0xFFFFFF;

//TIME_START

                         //    -1-1-1-1-1
        //    -1-1-1     //    -1 1 2 1-1
        //    -1 8-1     //    -1 2 4 2-1                   16*18
        //    -1-1-1     //    -1 1 2 1-1
                         //    -1-1-1-1-1

      // break, goto метка; , удалить лишние боковые точки в углах 5

      /// получение координат прямоугольников из структуры по x по y
             xp1 = strArray[0][Nstr].wordArray[Nwrd].xp1;
             xp0 = strArray[0][Nstr].wordArray[Nwrd].xp0;
             yp1 = strArray[0][Nstr].wordArray[Nwrd].yp1;
             yp0 = strArray[0][Nstr].wordArray[Nwrd].yp0;


      // Fg сумма внутри квадратика поиска точки, PorogFg порог различения точки

      // ms, ns;  масштаб по ширине, высоте 1, 2, 3,
      ms=2;  m=w*ms; m2=m*2; m3=m*3; m4=m*4;   // ms=2;   //  2
      ns=1;  n=ns;   n2=n*2; n3=n*3; n4=n*4;              //  3
      // краевые эффекты для этих циклов учтены в функции RecognitionPoint
      for ( y=yp0; y < yp1; y++ ) {    // вертикальный цикл
        for ( x=xp0; x < xp1; x++ ) {  // горизонтальный цикл

   // набор блоков
/*
         p=A+y*w+x;
         b=*p;   // pp=*(A+y*w+x);

         b3=*(p+m)+*(p-m)+*(p+n)+*(p-n)+*(p+m+n)+*(p-m+n)+*(p+m-n)+*(p-m-n);

         // количество слипшихся пикселей по ...
         b5W=*(p+m2)+*(p+m2+n)+*(p+m2+n2)+*(p+m2-n)+*(p+m2-n2);//вертикали сверху 1 (n)
         b5H=*(p-m2)+*(p-m2+n)+*(p-m2+n2)+*(p-m2-n)+*(p-m2-n2);//вертикали снизу  1 (n)
         b5P=*(p+n2)+*(p+m+n2)+*(p+m2+n2)+*(p-m+n2)+*(p-m2+n2);//горизонтали справа 2 (m)
         b5L=*(p-n2)+*(p+m-n2)+*(p+m2-n2)+*(p-m-n2)+*(p-m2-n2);//горизонтали слева  2 (m)
         b5 = b5W + b5H + b5P + b5P;

         b7W=*(p+m3)+*(p+m3+n)+*(p+m3+n2)+*(p+m3+n3)+*(p+m3-n)+*(p+m3-n2)+*(p+m3-n3); //верх
         b7H=*(p-m3)+*(p-m3+n)+*(p-m3+n2)+*(p-m3+n3)+*(p-m3-n)+*(p-m3-n2)+*(p-m3-n3); //низ
         b7P=*(p+n3)+*(p+m+n3)+*(p+m2+n3)+*(p+m3+n3)+*(p-m+n3)+*(p-m2+n3)+*(p-m3+n3); //право
         b7L=*(p-n3)+*(p+m-n3)+*(p+m2-n3)+*(p+m3-n3)+*(p-m-n3)+*(p-m2-n3)+*(p-m3-n3); //лево
         b7 = b7W + b7H + b7P + b7P;

         b9W=*(p+m4)+*(p+m4+n)+*(p+m4+n2)+*(p+m4+n3)+*(p+m4+n4)+*(p+m4-n)+*(p+m4-n2)+*(p+m4-n3)+*(p+m4-n4); //верх
         b9H=*(p-m4)+*(p-m4+n)+*(p-m4+n2)+*(p-m4+n3)+*(p-m4+n4)+*(p-m4-n)+*(p-m4-n2)+*(p-m4-n3)+*(p-m4-n4); //низ
         b9P=*(p+n4)+*(p+m+n4)+*(p+m2+n4)+*(p+m3+n4)+*(p+m4+n4)+*(p-m+n4)+*(p-m2+n4)+*(p-m3+n4)+*(p-m4+n4); //право
         b9L=*(p-n4)+*(p+m-n4)+*(p+m2-n4)+*(p+m3-n4)+*(p+m4-n4)+*(p-m-n4)+*(p-m2-n4)+*(p-m3-n4)+*(p-m4-n4); //лево
         b9 = b9W + b9H + b9P + b9P;
*/
//------------------------------------------------------------------------------

         //ms=1;  m=w*ms; m2=m*2; m3=m*3; m4=m*4; //для запуска разреженных точек
         //ns=1;  n=ns;   n2=n*2; n3=n*3; n4=n*4; //для запуска разреженных точек

         p=A+y*w+x;  // адрес A+y*w+x

         b=*p;   // b=*(A+y*w+x);
         b3=*(p+m)+*(p-m)+*(p+n)+*(p-n)+*(p+m+n)+*(p-m+n)+*(p+m-n)+*(p-m-n);// * значение по адресу

/*
         // + поиск точки  1х1, (бордюр вокруг точки 3х3, b3 )
         Fg=b;     // Fg сумма внутри квадратика поиска точки
         if ( Fg==1 && b3==0 ) {
         strArray[0][Nstr].wordArray[Nwrd].tsegFlag =1;
         // получили маркер (в структуре) на наличие точки после буквы.
         goto MtxPointEND;
         }  // if
*/

///////////////////////////////

         // количество слипшихся в блоках пикселей по ...
         b5W=*(p+m2)+*(p+m2+n)           +*(p+m2-n)+*(p+m2-n2);//вертикали сверху 1 (n)
         b5H=*(p-m2)+*(p-m2+n)+*(p-m2+n2)+*(p-m2-n);           //вертикали снизу  1 (n)
         b5P=*(p+n2)+*(p+m+n2)+*(p+m2+n2)+*(p-m+n2);           //горизонтали справа 2 (m)
         b5L=*(p-n2)+*(p+m-n2)           +*(p-m-n2)+*(p-m2-n2);//горизонтали слева  2 (m)
         b5 = b5W + b5H + b5P + b5P;
         // добавление в блоки ранее удаленных из углов квадрата двойных точек
         qW=*(p+m2+n2);  qH=*(p-m2-n2); qP=*(p-m2+n2); qL=*(p+m2-n2);
         b5W+=qW;  b5H+=qH;  b5P+=qP;  b5L+=qL;
/*
         // + поиск точки 3х3,(бордюр 5х5 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3;   // Fg сумма внутри квадратика поиска точки
         if ( Fg>3 && b5W<2 && b5H<2 && b5P<2 && b5L<2 ) { // малый page07_point.jpg
         strArray[0][Nstr].wordArray[Nwrd].tsegFlag = 3;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         // получили маркер (в структуре) на наличие точки после буквы.
         #ifdef DEBUGLVL
         drawData[0][y][x]=1;  // draw it
         #endif
         goto MtxPointEND;
         }  // if
*/

         b7W=*(p+m3)+*(p+m3+n)+*(p+m3+n2)           +*(p+m3-n)+*(p+m3-n2)+*(p+m3-n3); //верх
         b7H=*(p-m3)+*(p-m3+n)+*(p-m3+n2)+*(p-m3+n3)+*(p-m3-n)+*(p-m3-n2);            //низ
         b7P=*(p+n3)+*(p+m+n3)+*(p+m2+n3)+*(p+m3+n3)+*(p-m+n3)+*(p-m2+n3);            //право
         b7L=*(p-n3)+*(p+m-n3)+*(p+m2-n3)           +*(p-m-n3)+*(p-m2-n3)+*(p-m3-n3); //лево
         b7 = b7W + b7H + b7P + b7P;
         qW=*(p+m3+n3);  qH=*(p-m3-n3); qP=*(p-m3+n3); qL=*(p+m3-n3);
         b7W+=qW;  b7H+=qH;  b7P+=qP;  b7L+=qL;
/*
         // поиск точки 5х5,(бордюр 7х7 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3 + b5;     // Fg сумма внутри квадратика поиска точки
         if ( Fg>10 && b7W<2 && b7H<2 && b7P<3 && b7L<3  // средний 0022.jpg  9 2 3
         //if ( Fg>10 && b7W<3 && b7H<3 && b7P<5 && b7L<5    // ?? .jpg
                     ) {
         strArray[0][Nstr].wordArray[Nwrd].tsegFlag = 5;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         #ifdef DEBUGLVL
         drawData[0][y][x]=1;  // draw it
         #endif
         goto MtxPointEND;
         }  // if
*/

///////////////////////////////

         // используем после автоопределения размеров и пропорции точки

         //ms=2;  m=w*ms; m2=m*2; m3=m*3; m4=m*4; //для запуска разреженных точек по вертикали  ms=2;
         //ns=1;  n=ns;   n2=n*2; n3=n*3; n4=n*4; //для запуска разреженных точек по горизонтали ns=2;

         b9W=*(p+m4)+*(p+m4+n)+*(p+m4+n2)+*(p+m4+n3)           +*(p+m4-n)+*(p+m4-n2)+*(p+m4-n3)+*(p+m4-n4); //верх
         b9H=*(p-m4)+*(p-m4+n)+*(p-m4+n2)+*(p-m4+n3)+*(p-m4+n4)+*(p-m4-n)+*(p-m4-n2)+*(p-m4-n3);            //низ
         b9P=*(p+n4)+*(p+m+n4)+*(p+m2+n4)+*(p+m3+n4)+*(p+m4+n4)+*(p-m+n4)+*(p-m2+n4)+*(p-m3+n4);            //право
         b9L=*(p-n4)+*(p+m-n4)+*(p+m2-n4)+*(p+m3-n4)           +*(p-m-n4)+*(p-m2-n4)+*(p-m3-n4)+*(p-m4-n4); //лево
         b9 = b9W + b9H + b9P + b9P;
         qW=*(p+m4+n4);  qH=*(p-m4-n4); qP=*(p-m4+n4); qL=*(p+m4-n4);
         b9W+=qW;  b9H+=qH;  b9P+=qP;  b9L+=qL;

         // поиск точки 7х7,(бордюр 9х9 вокруг точки, b состоит из bW,bH,bP,bP)
         Fg = b + b3 + b5 + b7;     // Fg сумма внутри квадратика поиска точки
    //   if ( Fg>24 && b9W<3 && b9H<3 && b9P<5 && b9L<5 // ns=2; 0037_DUDJOM_LINGPA_POINT.jpg
    //   if ( Fg>20 && b9W<4 && b9H<4 && b9P<5 && b9L<5 // ns=1; большой DzogchenNendro_Page_021.png
         if ( Fg>20 && b9W<2 && b9H<2 && b9P<3 && b9L<3 // ms2; 009_.jpg
                     ) {
         strArray[0][Nstr].wordArray[Nwrd].tsertoFlag = 7;
         strArray[0][Nstr].wordArray[Nwrd].xp = x;
         strArray[0][Nstr].wordArray[Nwrd].yp = y;
         #ifdef DEBUGLVL_GREY
         drawData[0][y][x]=1;  // draw it
         #endif
         goto MtxPointEND;
         }  // if
/**/

///////////////////////////////

/*
         // поиск точки 3х3,(бордюр вокруг точки 5х5, b5 состоит из bW,bH,bP,bP)
         Fg=b + b3;     // Fg сумма внутри квадратика поиска точки
         if ( Fg>PorogFg && bW<=PrgW && bH<=PrgH && bP<=PrgP && bL<=PrgL ) {
         strArray[0][Nstr].wordArray[Nwrd].tsegFlag = 3;
         // получили маркер (в структуре) на наличие точки после буквы.
         #ifdef DEBUGLVL
         drawData[0][y][x]=1;  // draw it
         #endif
         goto MtxPointEND;
         }  // if
*/

         }  // for x
   } // for y

//------------------------------------------------------------------------------

/**/

#ifdef DEBUGLVL
// DM(END);  DM(4444<<" MatrixPointB "); DM(PorogFg<<" PorogFg "); DM(END);
#endif

//TIME_PRINT

                 // выход из подпрограммы распознавания точек
                               MtxPointEND:;

}//_____________________________________________________________________________





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*

//  ----------------------------------------------------------------------------
// Запуск программы горизонтальной коррекции положения квадратиков
//  ----------------------------------------------------------------------------

void GBitset::CorrectLetterW1(vector<int> &HBuf){


  int y,x,p,d,m,n,s0,s1;
  int x0,x1,xp,xp0,xp1,x0B,x1B;  //,xp1B
  int y0,y1,yp,yp0,yp1,y0B,y1B;  // xp,yp координаты точки
  int res,res1,NWord,tFg;


TIME_START

   for ( Nstr=0; Nstr < strArray->size(); Nstr++ ){ // Цикл по количеству строк NStrok

        NWord = strArray[0][Nstr].wordArray.size();
        for ( Nwrd=0; Nwrd < NWord-1; Nwrd++ ){ // Цикл по количеству квадратиков вдоль строки NWord

           tFg = strArray[0][Nstr].wordArray[Nwrd].tsegFlag;
           if ( tFg ) {  // != 0

           /// получение координат прямоугольников и точек из структуры по x по y
               // xp,yp координаты точки
               wordOCR *wP=&strArray[0][Nstr].wordArray[Nwrd];
               xp = wP->xp;    yp = wP->yp;
               // x0,x1, y0,y1 координаты квадрата вокруг буквы
               x0 = wP->x0;    x1 = wP->x1;    y0 = wP->y0;    y1 = wP->y1;
               // xp0,xp1 координаты прямоугольника вокруг точки
               xp0= wP->xp0;   xp1 = wP->xp1;
               // x0B,x1B, y0B,y1B координаты квадрата вокруг буквы  Nwrd+1
               wP=&strArray[0][Nstr].wordArray[Nwrd+1];
               x0B= wP->x0;    x1B = wP->x1;   y0B = wP->y0;   y1B = wP->y1;


               // проверка попадания координаты точки (xp) в квадрат вокруг буквы
               if ( xp > x0 && xp <= x1 ) {
                  // подсчет вертикальных сумм в части квадрата вокруг буквы Nwrd
                  for ( x=xp; x>=xp0 && x>16; x-- ) { // горизонтальный цикл
                      res=0;
                      // смещение границ полосы подсчета вертикальных сумм вверх и в низ
                      s0=y0;  s1=y1;    // s0=y0-(y1-y0)*2/3;  s1=y1+(y1-y0)/3;
                      for ( y=s0; y < s1 && y<nrows; y++ ) { // вертикальный цикл
                         d=y*ncolumns;
                         // площадь точки не учитываем
                         if (y<yp-tFg/2){
                          res+=bits_data[d + x];
                          drawData[0][y][x]*=0.7; } // Визуализация
                      } // for y
                     // Визуализация
                     s0=y0-(y1-y0)/2;
                     for (p=s0; d<res+s0+1; p++) {drawData[0][p][x]*=0.7;}
                     if ( res > res1 ) {drawData[0][s0-2][x]*=0.1; break;}
                     res1=res;


                  } // for x
//                      LineVertical(x1, 0x00AA0011);  // темно синий
                    strArray[0][Nstr].wordArray[Nwrd].x1 = x+1;
               } // xp > x0


               // проверка попадания координаты точки (xp) в квадрат вокруг буквы
               if ( xp >= x0B && xp < x1B ) {
                  // подсчет вертикальных сумм в части квадрата вокруг буквы Nwrd+1
                  for ( x=xp; x<=xp1 && x<ncolumns; x++ ) { // горизонтальный цикл
                      res=0;
                      // смещение границ полосы подсчета вертикальных сумм вверх и в низ
                      s0=y0B;  s1=y1B;    // s0=y0-(y1-y0)*2/3;  s1=y1+(y1-y0)/3;
                      for ( y=s0; y < s1 && y<nrows; y++ ) { // вертикальный цикл
                         d=y*ncolumns;
                         // площадь точки не учитываем
                         //if (x<xp-tFg/2 || x>xp+tFg/2 || y<yp-tFg/2 || y>yp+tFg/2){
                         if ( y<yp-tFg/2){
                          res+=bits_data[d + x];
                          drawData[0][y][x]*=0.7; } // Визуализация
                      } // for y
                     // Визуализация
                     s0=y0-(y1-y0)/2;  // s0=y0B-(y1B-y0B)/2;
                     for (p=s0; p<res+s0+1; p++) {drawData[0][p][x]*=0.7;}
                     if ( res > res1 ) {drawData[0][s0-2][x]*=0.1; break;}
                     res1=res;
                  } // for x
//                  LineVertical(x0B, 0x0000AA00);  // зеленый
//                  LineVertical(x, 0x0000AA00);  // зеленый
                    strArray[0][Nstr].wordArray[Nwrd+1].x0 = x-1;
               } // if xp >= x0B




           } // if tFg

        } // Nwrd   // Цикл по количеству квадратиков вдоль строки NWord

   } // for Nstr  // Цикл по количеству строк NStro


                 //   if( NStrok<0) { return; }  page   строка line Line

#ifdef DEBUGLVL

// Визуализация скорректированных по горизонтали букв темно синими квадратиками //
//                   wordDraw(0x00AA0011);  // темно синий

#endif

TIME_PRINT



}//__________________________________________________________________________
*/

