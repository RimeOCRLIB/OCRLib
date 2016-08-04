#include "GBitset.h"
//-------------------------------АЛГОРИТМЫ------------------------------------//




//                                                     *
// Визуализация аналоговая вертикальных графиков L     *
// -----------------------------------------------------------------------------

void GBitset::LDraw(vector<int> &HBuf, int sm, int color){

#ifdef DEBUGLVL
#ifdef DRAW1
 int y,x,in,m;
 int size=HStr.size();
 TPoint ris[25000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
                   m=32;
           for (  y=0; y < nrows; y++ ) {
                   x=sm + HBuf[y]/m;
                   ris[y].x=x;

                   ris[y].y=nrows-y;  //заполнение массива координат линии

           } // for y
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=1,0; // 0,5
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,nrows-2);

		ImageProcessor->ImageEnView1->Update();
#endif
#endif
 // size=nrows;
}//_____________________________________________________________________________



//                                                                      *

//                                                                      *                                                       *
// Визуализация аналоговая вертикальных графиков H переменной длинны    *
// -----------------------------------------------------------------------------

void GBitset::LDrawVar(vector<int> &HBuf, int HDrawSize, int sm, int scope, int color){

#ifdef DEBUGLVL
#ifdef DRAW1
 int y,x,in,m;
 TPoint ris[25000];

 // WDrawSize - размер, sm - смещение по вертикали,   scope - масштаб=32, color - цвет
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           if(scope<=0) scope = 1;  //  m=32;
           for (  y=0; y < HDrawSize; y++ ) {
                   x=sm + HBuf[y]/scope;
                   ris[y].x=x;

                   ris[y].y=HDrawSize-y;  //заполнение массива координат линии

           } // for y
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,5; // 0,5
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,HDrawSize-2);

		ImageProcessor->ImageEnView1->Update();
#endif
#endif

}//_____________________________________________________________________________

//##

//                                                    |

//                                                    |

//                                                    |                                                        *

// Визуализация аналоговая вертикальных линий         |
// ---------------------------------------------------------------------------

void GBitset::LineVertical(int sm, int color){

#ifdef DEBUGLVL
#ifdef DRAW1
 int in;
 TPoint ris[4];   // 4

        //заполнение массива координат линии
        ris[0].x=sm;      ris[1].x=sm;    ris[2].x=sm;
        ris[0].y=nrows;   ris[1].y=0;     ris[2].y=0;
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,1;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,2);

		ImageProcessor->ImageEnView1->Update();

 #endif
#ifdef DEBUGLVL_GREY
    if(sm>0&&sm<ncolumns-4){
        for(int y=0;y<nrows;y++){
            drawData[0][y][sm]/=4;
            drawData[0][y][sm+1]/=4;
            drawData[0][y][sm+2]/=4;
            drawData[0][y][sm+3]/=4;
        }
    }
#endif
 #endif

}//_____________________________________________________________________________

//##


// Визуализация аналоговая горизонаальных графиков W  *******

// -----------------------------------------------------------------------------


void GBitset::WDraw(vector<int> &HBuf, int sm, int color){ //  LStr

#ifdef DEBUGLVL
#ifdef DRAW1
 int y,x,in,m,size;
 TPoint ris[50000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           //заполнение массива координат линии
                  m=16; sm=nrows-sm;
           for (  x=0; x < ncolumns; x++ ) {
                   y=sm - HBuf[x]/m;
                   ris[x].y=y;   // амплитуда

                   ris[x].x=x;   // горизонтальная разертка

           } // for x
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,ncolumns-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
 #endif
}//_____________________________________________________________________________

//##


// Визуализация аналоговая горизональных графиков W переменной длинны ******* **

// -----------------------------------------------------------------------------


void GBitset::WDrawVar(vector<int> &HBuf, int WDrawSize, int sm, int scope, int color){

#ifdef DEBUGLVL
#ifdef DRAW1

 // WDrawSize - размер, sm - смещение по вертикали,   scope - масштаб=8, color - цвет
 int y,x,in,size;
 TPoint ris[50000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           //заполнение массива координат линии
           if(scope<=0) scope = 1;
           sm=nrows-sm;
           for (  x=0; x < WDrawSize; x++ ) {
                   y=sm - HBuf[x]/scope;
                   ris[x].y=y;   // амплитуда

                   ris[x].x=x;   // горизонтальная разертка

           } // for x
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,WDrawSize-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
#ifdef DEBUGLVL_GREY
    if(scope<=0) scope = 1;
    sm=nrows-sm;
    int y;
    for (int  x=0; x < WDrawSize; x++ ) {
        y=sm - HBuf[x]/scope;
        drawData[0][y][x]=60;
        drawData[0][y+1][x]=60;
        drawData[0][y-1][x]=60;
        drawData[0][y-2][x]=60;
        drawData[0][y-3][x]=60;
    }
#endif
#endif
}//_____________________________________________________________________________

//##

//                                                       * * *

//                                                      *     *

//                                                      *     *

// Визуализация аналоговая полярные координаты           * * *

// -----------------------------------------------------------------------------


// Polar coordinates  полярные координаты
void GBitset::PolarCoordinates(vector<int> &HBuf,
                               vector<int> &GBuf,
                               int LDrawSize,
                               int smX,  int smY,
                               int scope,int color){

#ifdef DEBUGLVL
#ifdef DRAW1

 // WDrawSize - круговая длинна , smY - смещение по вертикали,   scope - масштаб=8, color - цвет
 int y,x,i,in,size;
 TPoint ris[50000];

           if(nrows<=ncolumns) { size = nrows; }
           else { size = ncolumns; }

           if(scope<=0) scope = 1;
           //smY=nrows-smY;
           //заполнение массива координат линии
           for (  i=0; i < size; i++ ) {
                   y=smY - GBuf[i]/scope;
                   x=smX - HBuf[i]/scope;
                   ris[i].y=y;   // вертикальная разертка

                   ris[i].x=x;   // горизонтальная разертка

           } // for i
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,size-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
 #endif
}//_____________________________________________________________________________


           //    ВОПРОС

//                                                       * 

//                                                      *  * * *

//                                                     *         *

// Визуализация аналоговая амплитуда частота

// -----------------------------------------------------------------------------


// AmplitudeFrequency  амплитуда частота
void GBitset::AmplitudeFrequency(vector<int> &GBuf,int smX,int smY,int scope,int color){

#ifdef DEBUGLVL
#ifdef DRAW1

 // WDrawSize - круговая длинна , smY - смещение по вертикали,   scope - масштаб=8, color - цвет
 int y,x,i,in,size;
 TPoint ris[50000];

           if(nrows<=ncolumns) { size = nrows; }
           else { size = ncolumns; }
 //          size = ncolumns;

           if(scope<=0) scope = 1;
           smY=nrows-smY;
           //заполнение массива координат линии
           for (  x=0; x < size; x=x+10 ) { // x++   LfMem=10*DeltaNstrKH;
        //     if(GBuf[x]>=0) { y=smY - GBuf[x]/scope; } // if
             y=smY - GBuf[x]/scope;
             ris[x].y=y;   // амплитуда
             ris[x].x=x;   // горизонтальная разертка
           } // for x

		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
        ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,size-2);

        ImageProcessor->ImageEnView1->Update();
/*   */
 #endif
 #endif
}//_____________________________________________________________________________





//                                                                   ***********

//                                                                   ***********

// Визуализация аналоговая  строк (прямоугольников) в структурах     ***********
// -----------------------------------------------------------------------------

void GBitset::stringDraw(int color){
#ifdef DEBUGLVL
#ifdef DRAW1
int in;
TPoint ris[9]; // 6

           // заполнение массива координат прямоугольника
		   for (int m=0; m < strArray->size(); m++ )
              {
                   ris[0].x=strArray[0][m].x0;
                   ris[0].y=nrows-strArray[0][m].y0;

                   ris[1].x=strArray[0][m].x0;

                   ris[1].y=nrows-strArray[0][m].y1;

                   ris[2].x=strArray[0][m].x1;

                   ris[2].y=nrows-strArray[0][m].y1;

                   ris[3].x=strArray[0][m].x1;

                   ris[3].y=nrows-strArray[0][m].y0;

                   ris[4].x=strArray[0][m].x0;

                   ris[4].y=nrows-strArray[0][m].y0;

                   //ris[5].x=strArray[0][m].x0;

                   //ris[5].y=nrows-strArray[0][m].y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();

                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color; // цвет линии
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,1;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); // 5
           } // m
           ImageProcessor->ImageEnView1->Update();
// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 коорддинаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
#endif
#endif
}//_____________________________________________________________________________



//                                             ***  **  ***

//                                             **   **  **
//                                             ***  **  ***
//                                             ***  *   ***
//                                             * *  **  ***
//                                             ***  **  ***

// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
// -----------------------------------------------------------------------------

void GBitset::frameDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int in;
int x0,x1,y0,y1;    // координаты прямоугольника вокруг колонок
int nFrm,nFrame, nClm,nColumn;            //, Mean

TPoint ris[9];     // количество координат для прямоугольника 10 (0,1,2...9)=9
//...(in,ris,4);   // количество узлов для прямоугольника      5 (0,1,2,3,4)=4



         nFrame = frameArray.size();
         for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
              frameOCR *wF=&frameArray[nFrm];

      ////
              // заполнение массива горизонтальными блоками описывающих
              // многоугольники колонок
              nColumn=frameArray[nFrm].columnArray.size();
              for ( nClm=0; nClm < nColumn; nClm++ ){ // Цикл по количеству блоков колонок Clm
				  columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
				  x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;

                   ris[0].x=x0;
	        ris[0].y=nrows-y0;
	        ris[1].x=x0;
     	        ris[1].y=nrows-y1;

				   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

				   ris[4].y=nrows-y0;

					in=ImageProcessor->ImageEnView1->AddNewObject();
					if(wF->frameFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
					}  // цвет линии светло синий
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/6; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
                  } // nClm
/**/  ////

//
              // заполнение массива нешними габаритами прямоугольника вокруг
              // многоугольника вертикальной колонки
              //x0=wF->xf0;   x1=wF->xf1;   y0=wF->yf0;   y1=wF->yf1;
              x0=wF->xfg0;   x1=wF->xfg1;   y0=wF->yfg0;   y1=wF->yfg1;
              
                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;


                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wF->frameFlag);
                    if(wF->frameFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffbb00;
					}  // цвет линии светло синий
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/6; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); //3
//DM("yfg0="<<y0<<" yfg1="<<y1<<" xfg0="<<x0<<" xfg1="<<x1<<END);
/**/


/*      ////
              nColumn=frameArray[nFrm].columnArray.size();
              for ( nClm=0; nClm < nColumn; nClm++ ){
                  columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
                  x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;

// заполнение массива координат векторов вокруг колонок
// работает, отображается на графике, но не дописана.
// TPoint ris[3];   // количество координат для вектора 4 (0,1,2,3)=3
// ...(in,ris,1);   // количество узлов для вектора     2     (0,1)=1
                   ris[0].x=x0;
                   ris[0].y=nrows-y0;
                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
                    if(wF->frameFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }  // цвет линии светло синий
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/3; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,1);
                  } // nClm
*/  ////
         } // nFrm
         ImageProcessor->ImageEnView1->Update();
/**/
// цвет линии красный  0x000000FF,  светло синий  0xff1400,  салатовый  0xffff00

// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).

// Визуализация прямоугольниками в структурах, универсальная.
// void GBitset::quadrateDraw(int color, int thickness){

#endif    //
#endif

}//_____________________________________________________________________________



//                                             ***  **  ***


// Визуализация прямоугольников вертикальных колонок в структурах строк в пределах разворота.
// структура заполненена горизонтальными блоками L описывающих многоугольники колонок
// -----------------------------------------------------------------------------

void GBitset::lineDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int strInd,m,in;
int x0,x1,y0,y1;        // координаты прямоугольника вокруг блоков слов (колонок)
int NString, Mean;
int MaxString=60; // 10 //MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];          // ( если больше графический порт вешается)



         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
           Mean = strArray[0][strInd].MeanSA;
		   for ( m=0; m < strArray[0][strInd].lineArray.size(); m++ ){
            lineOCR *wL=&strArray[0][strInd].lineArray[m];
                   x0=wL->xL0;   x1=wL->xL1;   y0=wL->yL0;   y1=wL->yL1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

//                   ris[5].x=x0;

//                   ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wL->lineFlag);
                    if(wL->lineFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00ff00;
                    }  // цвет линии зеленый
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/12; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
         ImageProcessor->ImageEnView1->Update();
/**/
// цвет линии красный  0x000000FF,  светло синий  0xff1400,  салатовый  0xffff00

// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif

// Подпрограмма для получения координат блоков слов в пределах страницы.

}//_____________________________________________________________________________



//                                                    ******************

//                                                    *                *

//                                                    ******************

// Визуализация аналоговая  прямоугольников в структурах для блоков букв
// -----------------------------------------------------------------------------

void GBitset::blockDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int strInd,m,in;
int x0,x1,y0,y1;          // координаты прямоугольника вокруг блоков букв
int NString, Mean;
int MaxString=60; // 10 //MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)



         // заполнение массива ккоординат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
           Mean = strArray[0][strInd].MeanSA;
		   for ( m=0; m < strArray[0][strInd].blockArray.size(); m++ ){
            blockOCR *wB=&strArray[0][strInd].blockArray[m];
                   x0=wB->xb0;   x1=wB->xb1;   y0=wB->yb0;   y1=wB->yb1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wB->langFlag);
/*
                    if(wB->langFlag==0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x000000FF;
                    }  // цвет линии красный
                    if(wB->langFlag==1){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00;
                    }  // цвет линии салатовый
                    if(wB->langFlag==2){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }  // цвет линии светло синий
*/
//                  DM(wB->blockFlag);
					if(wB->blockFlag==0){ // блоки вне колонок
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x000000FF;
					}  // цвет линии красный
					if(wB->blockFlag>0 && wB->blockFlag<1000 ){ // блоки внутри колонок
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00ff00;
					}  // цвет линии зеленый
/*
					if(wB->blockFlag>1000){ // пустые блоки
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00fff0;
					}  // цвет линии желтый
*/
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=Mean/8;;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________


//                                                           *******

//                                                           *     *

//                                                           *******

// Визуализация аналоговая  прямоугольников в структурах для букв
// -----------------------------------------------------------------------------

void GBitset::wordDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int strInd,m,in,strFlag;
int x,x0,x1,y0,y1;        // ,xt0,yt0,xt1,yt1
int NString,MaxString=60; // MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается) 6

// x0,x1, y0,y1     координаты квадрата вокруг буквы


         // заполнение массива координат прямоугольника
         NString = strArray->size();
		 if ( NString > MaxString ) NString=MaxString;

		 for ( strInd=0; strInd < NString; strInd++ ){
		 strFlag=strArray[0][strInd].stringFlag;
		   // если stringFlag==0 то строку не заполняем квадратиками букв (в строке линия)
		   for ( m=0; strFlag==1 && m < strArray[0][strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[0][strInd].wordArray[m];
				   x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;

				   ris[0].x=x0;
				   ris[0].y=nrows-y0;

				   ris[1].x=x0;

				   ris[1].y=nrows-y1;

				   ris[2].x=x1;

				   ris[2].y=nrows-y1;

				   ris[3].x=x1;

				   ris[3].y=nrows-y0;

				   ris[4].x=x0;

				   ris[4].y=nrows-y0;

				   //ris[5].x=x0;

				   //ris[5].y=nrows-y0;

					in=ImageProcessor->ImageEnView1->AddNewObject();
//                    DM(wP->stackFlag);
					if(wP->stackFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии салатовый
					}else{
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400; // цвет линии светло синий
					}
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=1;  // толщина линии
					ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
					ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); // 5
		   } // m
		  //} // if
		 } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________


//                                                           ******

//                                                           *    *

//                                                           ******

// Вииизуализация аналоговая  прямоугольников в структурах для точек
// -----------------------------------------------------------------------------

void GBitset::pointDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int strInd,in,m,Lp,Cp;
int x0,x1,y0,y1;
int NString,MaxString=60; // MaxString - ограничение на колличество выводдимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)
//// DM("NhIm");

         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
		   for ( m=0; m < strArray[0][strInd].wordArray.size(); m++ ){
                   wordOCR *wP=&strArray[0][strInd].wordArray[m];
                   x0=wP->xp0;   x1=wP->xp1;   y0=wP->yp0;   y1=wP->yp1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;
                    Lp=0.5; Cp=color;  // толщина линии, цвет
                    if ( strArray[0][strInd].wordArray[m].tsertoFlag ) // != 0
                    { Lp=1.5; Cp=0x0000AA00; } // зеленый
                    if ( strArray[0][strInd].wordArray[m].spaceFlag )
                    { Lp=2.5; Cp=0xAF00AA00; } // светло серый
                    in=ImageProcessor->ImageEnView1->AddNewObject();
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)Cp; // цвет линии
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=Lp; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
         ImageProcessor->ImageEnView1->Update();


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


/**/
#endif    //
#endif
}//_____________________________________________________________________________





//                                                           ******

//                                                           ******


// Визуализация аналоговая  прямоугольников в структурах для огласовок
// -----------------------------------------------------------------------------

void GBitset::vowelDraw(int color){

#ifdef DEBUGLVL
#ifdef DRAW1  //
int strInd,m,in;
int x0,x1,y0,y1;        // xv0,xv1, yv0,yv1
int NString,MaxString=60; // 10MaxString - ограничение на колличество выводимыых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)


// xv0,xv1, yv0,yv1; координаты прямоугольника вокруг огласовоки


         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
		   for ( m=0; m < strArray[0][strInd].wordArray.size(); m++ ){
            wordOCR *wP=&strArray[0][strInd].wordArray[m];
                   x0=wP->xv0;   x1=wP->xv1;   y0=wP->yv0;   y1=wP->yv1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wP->vowelFlag);
                    if(wP->vowelFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии
                    }else{
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=1;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
		 } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________


//
// вертикальные и горизонтальные локальные сумммы массива графики bits_data
// vertical and horizontal local summmy //
// -----------------------------------------------------------------------------

void GBitset::BitsSummmyWH(int NSumm){
  
  unsigned int res,resD,w=ncolumns; //
  bool d,*d1,*d2;
  bool  *A=bits_data;
  //LStS массив заполненный (в PageWordDet) вертикальными  суммами for PageWordDet
  //BufL массив заполненный (в PageWordDet) вертикальными дифференциальными суммами for PageWordDet
  //int size=HStr.size();
  //vector<int> HStr(ncnr); // массив заполненный горизонтальными суммами
  //vector<int> HStS(ncnr); // массив заполненный горизонтальными дифференциальными суммами

///TIME_START
cout_<<" s1";
			  HStr.assign(ncnr,0);   HStS.assign(ncnr,0);
cout_<<" s2";
	  if ( NSumm==1)  {                                                  // != 0
			  // в HStr кладём горизонтальные суммы
			  // в HStS кладём горизонтальные дифференциальные суммы
		      //cout_<<" nrows="<<nrows<<"HStr="<<HStr.size()<<" HStS="<<HStS.size()<<endl;
			  for (int y=0; y < nrows; y++ ) {      // read image make sum
				res=resD=0;  d1=A+y*w;
				for (int x=0; x < ncolumns-2; x++ ) { // подсчет  горизонтальных сумм
					d2=d1+x;       res+=*d2;
					d=*d2-*(d2+2); resD+=d;      // (d2+1)  //d=*(d1+x);
					//DM(*d2); ///
				}
				HStr[y]=res;
				HStS[y]=resD;
				  /// <<4 <<5
				//drawData[0][y][x]=255+d*196;
				//DM(" res "<<res); DM(END); ///
			  }
cout_<<" s3";
		  /**/
///DM(" горизонтальные суммы ");
///TIME_PRINT

/*
			  // в HStr кладем горизонтальные суммы
			  // в HStS кладем горизонтальные дифференциальные суммы
			  for ( y=0; y < nrows; y++ ) {      // read image make sum
				res=0; resD=0;  p=y*ncolumns;
				for ( x=0; x < ncolumns; x++ ) { // подсчет  горизонтальных сумм
					//res+=bits_data[p + x];
					res+=pp=bits_data[p + x];
					resD+=pp - bits_data[p + x +1];
				}
				HStS[y]=resD<<5;  /// <<4 <<1   //
				HStr[y]=res;
//              drawData[0][y][x]=255+d*196;
			  }
*/
		 } // if

//      else { HBuf=HStr; GBuf=HStS; }


/**/

// вертикальные и горизонтальные локальные сумммы

/*
#ifdef DEBUGLVL
   // ВИЗУАЛИЗАЦИЯ аналоговая горизонтальных сумм (черная)
   int M=8; static short delta=50;
   for(int y=0; y < drawData->rows(); y++) {
	if(HStr[y]>=0){ for (int x=delta; x<(HStr[y]/M)+delta; x++)drawData[0][y][x]*=0.1; }
	else          { for (int x=delta; x>(HStr[y]/M)+delta; x--)drawData[0][y][x]*=0.1; }

   } // y++  // 0.8*ncolumns;

#endif
*/

}//_____________________________________________________________________________


// Подпрограмма гребенчатого фильтра Comb Filter с параметрами ( dt, g ).
// -----------------------------------------------------------------------------
  void GBitset::CombFilter(vector<int> &GBuf, int dt, int g) {
  int x;
				   // dt - задержка delay
				   // добротность гребенчатого фильтра;  1 < g < 10
	 if( dt<=0 || dt>=ncolumns || g >= 10 || g <= 1) { return; }
	 for (x=dt+4; x<ncolumns-dt-4; x++){GBuf[x]=(GBuf[x]+ GBuf[x-dt]*g/10);}
	 for (x=ncolumns-dt-4; x>dt+4; x--){GBuf[x]=(GBuf[x]+ GBuf[x+dt]*g/10);}
}//_____________________________________________________________________________


//                        +++++++++++++++++++
// Подпрограмма расчета средней амплитуды в массиве с параметром ( ms-сдвиг ).
// -----------------------------------------------------------------------------
   void GBitset::AmplitudeS(vector<int> &GBuf, int ms, int &AmSum) {
   int x;
 // ms-сдвиг на ms позиций, те деление на степень двойки значений в исходном массиве.
		   AmSum=0;
		   for ( x=0; x < ncolumns; x+=1 ){ AmSum+=abs(GBuf[x]); }  // x++
		  AmSum=AmSum /(ncolumns>>ms);   // >>ms
 }//_____________________________________________________________________________

 //##
 // Преобразование ГИЛБЕРТА с параметром (ширина) PGilb
 // -----------------------------------------------------------------------------


 void GBitset::filterGilbert(vector<int> &HBuf,
                             vector<int> &GBuf,
                             int MGilb,
                             int PGilb){
  int y;

            // MGilb - размер массива преобразования ГИЛБЕРТА
            // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
                      //if( PGilb==0) { return; }

                      for ( y=0; y < 26; y++ ){ GBuf[y]=0; }
                      for ( y=MGilb-26; y < MGilb; y++ ){ GBuf[y]=0; }
 /*  */

         if ( PGilb==0 ) {
                      // преобразование ГИЛБЕРТА PGilb==7
               for (  y=8; y < MGilb-8; y++ ){
               GBuf[y]=(
                         (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1])/2;
                    }// for y
            }


         if ( PGilb==1 ) {
                     // преобразование ГИЛБЕРТА PGilb==15
               for (  y=12; y < MGilb-12; y++ ){
               GBuf[y]=(
       //                + (HBuf[y-11]-HBuf[y+11])/11 + (HBuf[y-10]-HBuf[y+10])/10
       //                + (HBuf[y-9]-HBuf[y+9])/9 + (HBuf[y-8]-HBuf[y+8])/8
                       + (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1] )/2;
                    }// for y
            }


         if ( PGilb==2 ) {
                     // преобразование ГИЛБЕРТА PGilb==15
               for (  y=16; y < MGilb-16; y++ ){
               GBuf[y]=(
       //                + (HBuf[y-15]-HBuf[y+15])/15 + (HBuf[y-14]-HBuf[y+14])/14
       //                + (HBuf[y-13]-HBuf[y+13])/13 + (HBuf[y-12]-HBuf[y+12])/12
                       + (HBuf[y-11]-HBuf[y+11])/11 + (HBuf[y-10]-HBuf[y+10])/10
                       + (HBuf[y-9]-HBuf[y+9])/9 + (HBuf[y-8]-HBuf[y+8])/8
                       + (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1] )/2;
                    }// for y
            }


         if ( PGilb==3 ) {
                     // преобразование ГИЛБЕРТА PGilb==23
               for (  y=20; y < MGilb-20; y++ ){
               GBuf[y]=(
       //                + (HBuf[y-23]-HBuf[y+23])/23 + (HBuf[y-22]-HBuf[y+22])/22
       //                + (HBuf[y-21]-HBuf[y+21])/21 + (HBuf[y-20]-HBuf[y+20])/20
       //                + (HBuf[y-19]-HBuf[y+19])/19 + (HBuf[y-18]-HBuf[y+18])/18
       //                + (HBuf[y-17]-HBuf[y+17])/17 + (HBuf[y-16]-HBuf[y+16])/16
                       + (HBuf[y-15]-HBuf[y+15])/15 + (HBuf[y-14]-HBuf[y+14])/14
                       + (HBuf[y-13]-HBuf[y+13])/13 + (HBuf[y-12]-HBuf[y+12])/12
                       + (HBuf[y-11]-HBuf[y+11])/11 + (HBuf[y-10]-HBuf[y+10])/10
                       + (HBuf[y-9]-HBuf[y+9])/9 + (HBuf[y-8]-HBuf[y+8])/8
                       + (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1] )/2;
                    }// for y
            }
 /* */

         if ( PGilb==4 ) {
                     // преобразование ГИЛБЕРТА PGilb==23
               for (  y=24; y < MGilb-24; y++ ){
               GBuf[y]=(
       //                + (HBuf[y-23]-HBuf[y+23])/23 + (HBuf[y-22]-HBuf[y+22])/22
       //                + (HBuf[y-21]-HBuf[y+21])/21 + (HBuf[y-20]-HBuf[y+20])/20
                       + (HBuf[y-19]-HBuf[y+19])/19 + (HBuf[y-18]-HBuf[y+18])/18
                       + (HBuf[y-17]-HBuf[y+17])/17 + (HBuf[y-16]-HBuf[y+16])/16
                       + (HBuf[y-15]-HBuf[y+15])/15 + (HBuf[y-14]-HBuf[y+14])/14
                       + (HBuf[y-13]-HBuf[y+13])/13 + (HBuf[y-12]-HBuf[y+12])/12
                       + (HBuf[y-11]-HBuf[y+11])/11 + (HBuf[y-10]-HBuf[y+10])/10
                       + (HBuf[y-9]-HBuf[y+9])/9 + (HBuf[y-8]-HBuf[y+8])/8
                       + (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1] )/2;
                    }// for y
            }
 /* */
         if ( PGilb==5 ) {
                    // преобразование ГИЛБЕРТА PGilb==31
               for (  y=32; y < MGilb-32; y++ ){
               GBuf[y]=(
                       + (HBuf[y-31]-HBuf[y+31])/31 + (HBuf[y-30]-HBuf[y+30])/30
                       + (HBuf[y-29]-HBuf[y+29])/29 + (HBuf[y-28]-HBuf[y+28])/28
                       + (HBuf[y-27]-HBuf[y+27])/27 + (HBuf[y-26]-HBuf[y+26])/26
                       + (HBuf[y-25]-HBuf[y+25])/25 + (HBuf[y-24]-HBuf[y+24])/24
                       + (HBuf[y-23]-HBuf[y+23])/23 + (HBuf[y-22]-HBuf[y+22])/22
                       + (HBuf[y-21]-HBuf[y+21])/21 + (HBuf[y-20]-HBuf[y+20])/20
                       + (HBuf[y-19]-HBuf[y+19])/19 + (HBuf[y-18]-HBuf[y+18])/18
                       + (HBuf[y-17]-HBuf[y+17])/17 + (HBuf[y-16]-HBuf[y+16])/16
                       + (HBuf[y-15]-HBuf[y+15])/15 + (HBuf[y-14]-HBuf[y+14])/14
                       + (HBuf[y-13]-HBuf[y+13])/13 + (HBuf[y-12]-HBuf[y+12])/12
                       + (HBuf[y-11]-HBuf[y+11])/11 + (HBuf[y-10]-HBuf[y+10])/10
                       + (HBuf[y-9]-HBuf[y+9])/9 + (HBuf[y-8]-HBuf[y+8])/8
                       + (HBuf[y-7]-HBuf[y+7])/7 + (HBuf[y-6]-HBuf[y+6])/6
                       + (HBuf[y-5]-HBuf[y+5])/5 + (HBuf[y-4]-HBuf[y+4])/4
                       + (HBuf[y-3]-HBuf[y+3])/3 + (HBuf[y-2]-HBuf[y+2])/2
                       +  HBuf[y-1]-HBuf[y+1] )/3;
                    }// for y
         }


         if ( PGilb==6 ) {
              // ассиметричное преобразование ГИЛБЕРТА
              for ( y=4; y < MGilb-4; y++ ){
                GBuf[y]=abs(
                             (HBuf[y-3]-HBuf[y+2])/3 +
                             (HBuf[y-2]-HBuf[y+1])/2 +
                              HBuf[y-1]-HBuf[y]
                           );
              }// for y
          }
/*   */

#ifdef DEBUGLVL
// DM(*MGilb<<" -*MGilb- ");DM(END);
#endif


  }//___________________________________________________________________________

/*

                               ////

                            //     //

                       /////        /////


// Подпрограмма визуализация спектра imaging spectrum .

// -----------------------------------------------------------------------------


  void GBitset::ImagingSpectrum(vector<int> &HBuf) {


  int y,x,n,p,d,g;

  int DnKH,LfAmSum;

  int size=DeltaNstrKH*8;    //  SmGr=nrows/4,scope=8,

  ///vector<int> BufR(ncolumns);

  vector<int> BufK(size);

  ///vector<int> BufHelp(ncolumns);

  // fabs


                       BufHelp.assign(ncolumns,0);

                     // ВИЗУАЛИЗАЦИЯ максимума сректра
// линейное нахождение максимума Функции ( максимум сректра )
// (средней резонансной пространственной частоты букв в строке).
// Функция состоит из: гребенчатого фильтра, полосового фильтра и амплитудного детектора.

    g=3; // добротность гребенчатого фильтра = 3;  1 < g < 1
    DnKH=(DeltaNstrKH*5/2-DeltaNstrKH/2)/16; // 16-число дискретных отсчетов // DeltaNstrKH/8;
    if ( DnKH<1 ) DnKH=1;
    d=0; p=0;
    for ( n=DeltaNstrKH/2; n<DeltaNstrKH*5/2 && n<size/2; n=n+DnKH ){ //12
           BufR=HBuf;
          filterLaplasian(BufR,n/4,(n/4)*2); // полосовой фильтр, (n/4)*2) это не ошибка
          CombFilter(BufR, n, g);            // гребенчатый фильтр, резонансный фильтр
          AmplitudeS(BufR, 2, LfAmSum);      // получение средней амплитуды
           BufK[n+size/2]=LfAmSum;
           p=p+DnKH;  d++;
////           DM(n<<"n  "); DM(LfAmSum<<"  ");
    }// for n
////    DM(END);


          //  нахождение min Функции maxRes
          int minResY, minRes=9999;
          for ( y=DeltaNstrKH/2; y<DeltaNstrKH*5/2; y=y+DnKH ){
              if( BufK[y+size/2] < minRes && BufK[y+size/2]!= 0 ) //
              { minRes=BufK[y+size/2]; minResY=y; }
          } // minResY  координаты min Функции maxRes


          // вычитание фона ( minResY ) из сректра

          for ( y=DeltaNstrKH/2; y<DeltaNstrKH*5/2; y=y+DnKH )

          { BufK[y+size/2]=(BufK[y+size/2]-minRes)*8; }

          // фильтрация максимума Функции
          // смещение на size/2 делает менее актуальным краевые эффекты при фильтрации
          int DnKHF=DeltaNstrKH/3;  // *3/2;
          if ( DnKHF<2 ) DnKHF=1;   if ( DnKHF>=31 ) DnKHF=31;
          filterLaplasian(BufK,DnKHF,DnKHF*2);  // полосовой фильтр  ,31,62

          //  нахождение максимума сректра ( максимума Функции BufS )
          int maxRes=0, maxResY, maxResYs;
          for ( y=DeltaNstrKH/2; y<DeltaNstrKH*5/2; y=y+1 ){
                 if( BufK[y+size/2] >= maxRes )
                 { maxRes=BufS[y+size/2]; maxResY=y+size/2; }
          } // maxResY  координаты максимума Функции maxRes


          //  максимум сректра

          maxResYs=maxResY-size/2;


//// DM(END); DM(maxResYs<<"-maxResYs-  ") DM(minRes<<"minRes  "); DM(maxResY<<"maxResY  "); ;
//// DM(maxRes<<"maxRes  "); DM(d<<"d  ");  DM(DnKH<<"DnKH  "); DM(END);

          for ( y=0; y < size; y++ ){  //
            BufHelp[y+ncolumns/2-size/2]=BufK[y]/8;
          }// for y


#ifdef DEBUGLVL
                       // ВИЗУАЛИЗАЦИЯ  параметров
//                                 DM(END);
//DM(KHig<<" KHig ") ;DM(DeltaNstr<<" DeltaNstr ");DM(DeltaNstrKH<<" DeltaNstrKH ");
//                                 DM(END);

                       // ВИЗУАЛИЗАЦИЯ цифровая, таблица
//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < size;  x++ ){      // x < d+1;
//                  DM(BufK[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//              DM(END);

                       // ВИЗУАЛИЗАЦИЯ аналоговая
 // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм в пределах строки (черная)
//  int M=1; static short delta=nrows/2-nrows/20; // 300
//  for(x=0; x < ncolumns; x++) {
//   if(BufHelp[x]>=0){ for (y=delta; y<(BufHelp[x]/M)+delta; y++)drawData[0][y][x]*=0.1; }
//   else             { for (y=delta; y>(BufHelp[x]/M)+delta; y--)drawData[0][y][x]*=0.1; }

//  } // x++  // 0.8*ncolumns;

#endif



}//_____________________________________________________________________________

*/


////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------
//                        FFFFFFFFFFFFFFFFFF
// Подпрограмма объединяет: гребенчатый фильтр, амплитудный детектор.
// -----------------------------------------------------------------------------

  void GBitset::FunctionSpectrum( vector<int> &HBuf, int dt, int &AmSum) {

  int x, ds=dt+4;
  int size=(int)HBuf.size();
//  int size=ncolumns;

     // Подпрограмма гребенчатого фильтра Comb Filter с параметрами ( dt, g ).

     // добротность гребенчатого фильтра;  1 < g < 10
     /// if( dt<=0 || dt>=size || g >= 10 || g <= 1) { return; }

     int g=3; // dt - задержка delay
     for (x=ds; x<size-ds; x++){HBuf[x]=(HBuf[x]+ HBuf[x-dt]*g/10);}
     for (x=size-ds; x>ds; x--){HBuf[x]=(HBuf[x]+ HBuf[x+dt]*g/10);}

     // Подпрограмма расчета средней амплитуды в массиве с параметром ( ms-сдвиг ).

     int ms=1;
     AmSum=0;
     for ( x=8; x < size-8; x+=2 ){ AmSum+=abs(HBuf[x]); } // x++; x+=1 //x+=2 ms=1;
     AmSum=AmSum /(size>>ms);   // >>ms
     // ms-сдвиг на ms позиций, те деление на степень двойки значений
     // в исходном массиве.

// DM(size<<" sizeF ");  DM(END);

}//_____________________________________________________________________________


// -----------------------------------------------------------------------------
//                        FFFFFFFFFFFFFFFFFF_ST
// Подпрограмма объединяет: гребенчатый фильтр, амплитудный детектор.

// -----------------------------------------------------------------------------


  void GBitset::FunctionSpectrSt( int *Buf2, int size,  int dt, int &AmSum) {



  int x, ds=dt+4;

//  int size=Buf2.size();

//  int size=ncolumns;




     // Подпрограмма гребенчатого фильтра Comb Filter с параметрами ( dt, g ).


     // добротность гребенчатого фильтра;  1 < g < 10

     /// if( dt<=0 || dt>=size || g >= 10 || g <= 1) { return; }


     int g=3; // dt - задержка delay

     for (x=ds; x<size-ds; x++){Buf2[x]=(Buf2[x]+ Buf2[x-dt]*g/10);}

     for (x=size-ds; x>ds; x--){Buf2[x]=(Buf2[x]+ Buf2[x+dt]*g/10);}



     // Подпрограмма расчета средней амплитуды в массиве с параметром ( ms-сдвиг ).


     int ms=1;

     AmSum=0;

     for ( x=8; x < size-8; x+=2 ){ AmSum+=abs(Buf2[x]); } // x++; x+=1 //x+=2 ms=1;
     AmSum=AmSum /(size>>ms);   // >>ms

     // ms-сдвиг на ms позиций, те деление на степень двойки значений

     // в исходном массиве.

// DM(size<<" sizeF ");  DM(END);


}//_____________________________________________________________________________

/**/





////////////////////////////////////////////////////////////////////////////////




/**/

 /*

          dt=LfMem=(Lf+Lf+Rg)/3;              // треть влево (Lf+Lf+Rg)/3;

         if ( dt/4 != dtMemL ) {
              filterLaplasianM(HBuf,GBuf,dt/4,(dt/4)*2); // полосовой `фильтр
              N++;
         }
           dtMemL=dt/4;  NN++;
           BufR=GBuf;
           FunctionSpectrum(BufR, dt, LfAmSum);

           dt=RgMem=(Lf+Rg+Rg)/3;              // треть вправо  (Lf+Rg+Rg)/3;
         if ( dt/4 != dtMemR ) {
              filterLaplasianM(HBuf,GBuf, dt/4,(dt/4)*2); // полосовой `фильтр
              N++;
         }
           BufR=GBuf;
           dtMemR=dt/4;  NN++;
           FunctionSpectrum(BufR, dt, RgAmSum);

           DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="
           <<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);

         if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
         if ( Rg-Lf<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)

 */









////////////////////////////////////////////////////////////////////////////////


 /*

http://window.edu.ru/window_catalog/pdf2txt?p_id=22254&p_page=3


Статические и динамические массивы

Статическим массивом называют набор данных, для хранения которого перед
началом функционирования программы выделяется фиксированное место в
памяти, освобождаемое после завершения работы программы.
В отличие от этого место для хранения динамических массивов выделяется и
освобождается в процессе выполнения программы. В одних случаях эти операции
осуществляются системой автоматически.
Например, когда отводится память для хранения локальных массивов в
процедуурах и функциях. В других случаях пользователю предоставляется
возможность запросить участок памяти нужного размера и в дальнейшем -
освободить его. Только таким способом в программах можно завести массив
переменного размера.
В Си для запроса и освобождения памяти используются следующие системные
функциии:
q=(тип_q *)calloc(n_el,s_el);        //запрос памяти с очисткой;
q=(тип_q *)farcalloc(n_el,s_el);     //запрос памяти с очисткой;
q=(тип_q *)malloc(n_byte);           //запрос памяти в ближней "куче"
q=(тип_q *)farmalloc(n_byte); //запрос памяти в дальней "куче"
q_new=realloc(q_old,n_byte); //изменение размера блока
q_new=farrealloc(q_old,n_byte);      //изменение размера блока
free(q);                             //освобождение памяти
farfree(q);                    //освобождение памяти
В приведенных выше обращениях q обозначает указатель на тип данных
элементов массива, заменяющий имя массива. Параметры n_el и s_el задают
соответственно количество элементов в массиве и длину каждого элемента в
байтах. Параметр n_byte определяет количчество запрашиваемых байтов.
Максимальный размер сегмента памяти, предоставляемого в ближней "куче",
равен 65521 байт. Добавка far означает, что программа использует дальние
указатели типа far или huge, которые позволяют адресоваться к дальней "куче" и
использовать сегменты размером более 64 кБ. Любая функция выделения памяти
возвращает начальный адрес или "нулевой" указатель (NULL) в случае отсутствия
свободной памяти запрашиваемого размера. Для того чтобы нормально работать с
предоставленным фрагментом памяти, возвращаемый адрес обязательно должен
быть приведен к типу указателя q.
Функция realloc (farrealloc) позволяет перераспределить ранее выделенную
память. При этом новый размер массива может быть как меньше предыдущего,
так и больше его. Если система выделит память в новом месте, то все предыдущие
значения, к которым программа обращалась по указателю q_old будут переписаны
на новое место автоматически.
В новых версиях Borland C++ появились две более удобные процедуры для
запроса и освобождения памяти, не нуждающиеся в дополнительном указании о
приведении типа возвращаемого адреса:
q=new тип[n_el];  //запрос памяти под массив из n_el элементов;
q=new тип;        //запрос памяти под скалярную переменную;

                                      26


delete q[n_el];            //освобождение памяти, занятой массивом;
delete q;                  //освобождение памяти, занятой массивом или
                           //скалярной переменной;
Процедура освобождения памяти не чистит указатель, "смотревший" на начало
возвращаемого сегмента. Запись по такому указателю после возврата памяти
приводит к трудно обнаруживаемым ошибкам. Поэтому к правилам "хорошего
тона" в программировании относится и сброс указателей после возврата
динамически ззапрашивавшейся памяти:
q=NULL;
Не менее хорошее правило заключается и в проверке, выделена ли запрашиваемая
память после обращения к соответствующей процедуре. Например, в Си, не
контролирующем запись по нулевому адресу, после стирания нескольких первых
элементов несуществующего массива происходит зависание операционной
системы.
При динамическом распределении памяти для массивов следует описать
соответствующий указатель и присваивать ему значение при помощи функции
calloc. Одномерный массив a[10] из элементов типа float можно создать
следующим образом
 float *a;
 a=(float*)(calloc(10,sizeof(float));// a=(float*)malloc(10*sizeof(float));
 if (!a) // if (a==NULL)
 {printf (“out of memory\n); //выход за пределы памяти
         exit (1);
         }
Аналогично может быть распределена память и для 2-мерного массива N?M
 a=(float*)(calloc(N*M,sizeof(float));// a=(float*)malloc(N*M*sizeof(float));
В этом случае он рассматривается как аналог одномерного массива из N?M
элементов.
Для создания по- настоящему двумерного массива вначале нужно распределить
память для массива указателей на одномерные массивы, а затем распределять
память для одномерных массивов Пусть, например, требуется создать массив

a[n][m], это можно сделать при помощи следующего фрагмента программы:


 
   main ()
       {  double **a;
          int n,m,i;
          scanf("%d %d",&n,&m);
          a=(double **)calloc(m,sizeof(double *));
          for (i=0; i<=m; i++) a[i]="(double" *)
          calloc(n,sizeof(double)); . . . . } 
Аналогичным образом можно распределить память и для трехмерного массива
размером n,m,l. Следует только помнить, что ненужную для дальнейшего выполнения
программы память следует освобождать при помощи функции free.


 
   main ()
       {  long ***a;
          int n,m,l,i,j;
          scanf("%d %d %d",&n,&m,&l);
//           --------  распределение памяти --------
          a=(long ***)calloc(m,sizeof(long **));
          for (i=0; i<=m; i++) 
          { a[i]="(long" **)calloc(n,sizeof(long *)); 
          for (j="0;" i<="l;" j++)
          a[i][j]="(long" *)calloc(l,sizeof(long)); } . . .  
//           освобождение памяти
          for (i="0;" i<="m;" i++) 
          { for (j="0;" j<="l;" j++) 
          free (a[i][j]); free (a[i]);} free (a);}

Рассмотрим еще один интересный пример, в котором память для массивов
распределяется в вызываемой функции, а используется в вызывающей. В таком
случае в вызываемую функцию требуется передавать указатели, которым будут
присвоены адреса выделяемой для массивов памяти.

Пример:


  
   main()
   {   int vvod(double ***, long **);
       double **a;    //  указатель для массива  a[n][m]
       long *b;       //   указатель для массива  b[n]
       vvod (&a,&b);
        ..   // в функцию vvod передаются адреса указателей,
        ..   // а не их значения
        ..
    }
    int vvod(double ***a, long **b)
    {   int n,m,i,j;
        scanf (" %d %d ",&n,&m);
        *a=(double **)calloc(n,sizeof(double *));
        *b=(long *)calloc(n,sizeof(long));
         for (i=0; i<=n; i++) 
        *a[i]="(double" *)calloc(m,sizeof(double)); ..... } 
Отметим также то обстоятельство, что указатель на массив не обязательно должен

показывать на начальный элемент некоторого массива. Он может быть сдвинут так,

что начальный элемент будет иметь индекс отличный от нуля, причем он может быть

как положительным так и отрицательным.




Динамическое размещение массивов

Пример:

    int main()
    {  float *q, **b;
       int i, j, k, n, m;
       scanf("%d %d",&n,&m);
       q=(float *)calloc(m,sizeof(float));
    // сейчас указатель q показывает на начало массива
       q[0]=22.3;
       q-=5;
    // теперь начальный элемент массива имеет индекс 5,
    // а конечный элемент индекс n-5
       q[5]=1.5;
    // сдвиг индекса не приводит к перераспределению
    // массива в памяти и изменится начальный элемент
       q[6]=2.5;   //  -  это второй элемент
       q[7]=3.5;   //  -  это третий элемент
       q+=5;
    // теперь начальный элемент вновь имеет индекс 0,
    // а значения элементов q[0], q[1], q[2] равны
    // соответственно 1.5, 2.5, 3.5
       q+=2;
    // теперь начальный элемент имеет индекс -2,
    // следующий -1, затем 0 и т.д. по порядку
       q[-2]=8.2;
       q[-1]=4.5;
       q-=2;
    // возвращаем начальную индексацию, три первых
    // элемента массива q[0], q[1], q[2], имеют
    // значения 8.2, 4.5, 3.5
       q--;
    // вновь изменим индексацию .
    // Для освобождения области памяти в которой размещен
    // массив q используется функция free(q), но поскольку
    // значение указателя q смещено, то выполнение
    // ф-я free(q) приведет к непредсказуемым последствиям
    // Для правильного выполнения этой функции
    // указатель q должен быть возвращен в первоначальное
    // положение
       free(++q);
    // Рассмотрим возможность изменения индексации и
    // освобождения памяти для двумерного массива
       b=(float **)calloc(m,sizeof(float *));
       for (i=0; i < m; i++)
           b[i]=(float *)calloc(n,sizeof(float));
    // После распределения памяти начальным элементом
    // массива будет элемент b[0][0]
    // Выполним сдвиг индексов так, чтобы начальным
    // элементом стал элемент b[1][1]
       for (i=0; i < m ; i++) --b[i];
       b--;
    // Теперь присвоим каждому элементу массива сумму его
    // индексов
       for (i=1; i<=m; i++) for (j="1;" j<="n;" j++) 
       b[i][j]="(float)(i+j);" 
    // Обратите внимание на начальные значенииия счетчиков
    // циклов i и j, он начинаются с 1 а не с 0
    // Возвратимся к прежней индексации
    for (i="1;" i<="m;" i++) ++b[i]; b++; 
    //Выполним освобождение памяти
    for (i="0;" i < m; i++) free(b[i]); free(b); ... 
    return 0; } 
В качестве последнего примера рассмотрим динамическое распределение памяти
для массива указателей на функции, имеющие один входной параметр типа double и
возвращающие значение типа double.

Пример:


    double cos(double);
    double sin(double);
    double tan(double);
   int main()
   { double (*(*masfun))(double);
     double x=0.5, y;
     int i;
     masfun=(double(*(*))(double))
              calloc(3,sizeof(double(*(*))(double)));
     masfun[0]=cos;
     masfun[1]=sin;
     masfun[2]=tan;
     for (i=0; i<3; i++); { y="masfun[i](x);" 
     printf("\n x="%g" y="%g",x,y);" } 
     return 0; } 



..............


*/

