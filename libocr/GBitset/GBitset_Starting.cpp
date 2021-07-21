#include "GBitset.h"
// STARTING //
// ---------------------------------------------------------------------------//

// --- Page String Detector --- GBitset_Starting.h ---
// Подпрограмма управления последовательностью выполнения всех подпрограмм.
// Режим 1 - выделение строк
// Режим 2 - выделение строк,выделение границ текста
// Режим 3 - выделение строк, выделение границ текста и деление строки на буквы
// ---------------------------------------------------------------------------//

void GBitset::pageStringDetectorOCR(vector<OCRBox>&strArray_){
    
    vector<stringOCR>strArray1;
    pageStringDetector(strArray1,1);
    strArray_.resize(strArray1.size());
    
    for(int i=0;i<strArray1.size();i++){
        OCRBox &p=strArray_[i];
        stringOCR &s=strArray1[i];
        p.yLimit0=nrows - s.LimY0;
        p.yLimit0=nrows - s.LimY1;
        p.y0=nrows - s.y0;
        p.y1=nrows - s.y1;
    }
}


void GBitset::pageStringDetector(vector<stringOCR>&strArray_,int mode){

  int y,x;
  MeanCol=0;  //  MeanCol-глобальная. Средний по странице период букв
  #define B BitsBuf
  strArray=&strArray_;
  //int print=0;

  // resize(ncnr) векторных массивов сделан в Bitset_base.h
TIME_START

/////////                                                              /////////

			  // коэффициент формы страницы // *DeltaKh/100;
			  DeltaKh = 100*ncolumns/srcW*srcH/nrows;  // глобальная.


GrafT[0][0]=1;
if( GrafT[0][0] ) {
DM(END); DM(END); DM(END);  DM(0000<<" SUMM_pageStringDetector_SUMM " );
DM(END); DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows "); DM(ncnr<<" ncnr ");
						 DM(END);
}
			  // обнуление массива GrafT
			  for ( x=0; x < 32; x++ ){
				  for ( y=0; y < 16; y++ ) { GrafT[x][y]=0; }
			  } // x=0

 // ключ к запуску графики в подпрограммах
 // GrafT[M][N]=1;  M - номер подпрограммы с активной графикой  (0-31)
 //                 N - разновидность активной графики подпрограммы (0-15)
 //                 1, 0 - соответственно; выводится, не выводится на экран графика

				  GrafT[0][0]=0; // SUMM_pageStringDetector_SUMM

		   // Запуск подпрограммы для получения угла поворота текста.
				  GrafT[14][0]=0;
			  //   detectRotation(int NAverage);
				 // получили значение угла поворота текста в градусахи(Alpha)



		   // Подпрограмма чистки (border) входного однобитового массива. ВХОД
				  GrafT[15][0]=0; GrafT[15][1]=0;

				 //data_null_border();
				 // затирает мусор с краев скана + border
		   // Запуск подпрограммы для получения координат строк в структуре.   //
				   GrafT[1][0]=0; GrafT[1][4]=0;  GrafT[1][5]=0;  GrafT[1][6]=0;
DM("@4_2_1");
				 PrimaryStringDetector();
				 // получили координаты строк в структуре.

// вычисленное количество строк в печа  NStrok=strArray.size();
if( strArray->size()< 1 ) { return; }

		   // Подпрограмма получения координат блоков букв в пределах страници (квадратики).
				   GrafT[2][0]=0;  GrafT[2][2]=0;
///				   GrafT[2][1]=1;  GrafT[2][3]=1;  GrafT[2][4]=1;
///				   GrafT[2][5]=1;  GrafT[2][2]=1;  GrafT[2][7]=1;
///				   GrafT[2][6]=1;  GrafT[2][8]=1;
DM("@4_2_2");
	             if(mode>1)PageWordDet();
				 // получили координаты блоков букв в структуре.Флаги

		   // Подпрограмма для выделения рамки вокруг текста печа.
				   GrafT[15][0]=0; GrafT[15][1]=0; GrafT[15][2]=0; GrafT[15][3]=0; ////
DM("@4_2_3");
	             if(mode>1)GBitsetNet();
				 // получили координаты рамки в структуре.

		   // Подпрограмма для получения координат вертикальных колонок в пределах разворота.
				   GrafT[11][0]=0;  GrafT[11][3]=1;  GrafT[11][4]=1;  ////
DM("@4_2_4");
	              if(mode>1)pageSegmentationBitset();
				 // получили координаты вертикальных колонок в структуре.

		   // Подпрограмма получения координат Tib букв  в пределах страници (квадратики).
				   GrafT[3][0]=0;  GrafT[3][1]=0;    GrafT[3][5]=0;
DM("@4_2_5");
	              if(mode>1)PageWordDetTib();
				 // получили координаты букв в структуре.

		   // Подпрограмма получения координат Eng букв  в пределах страници (квадратики).
				   GrafT[10][0]=0;  GrafT[10][1]=0;
//                 PageWordDetEng();
				// получили координаты букв в структуре.

		   // Подпрограмма  вертикальной коррекции координат букв (квадратиков).
				   GrafT[4][0]=0;
DM("@4_2_6");
				 if(mode>1)CorrectSqH();
				 // откорректировали координаты квадратиков по вертикали.
/*
		   // Запуск подпрограммы выделения точек в пределах строки.
				   GrafT[5][0]=1;  GrafT[5][1]=0;  GrafT[5][2]=0;
				 pagePointDetector ();                // GBitsetPoint.h
*/				 // получили маркер (в структуре) о наличии точки после буквы.

		   // Подпрограмма  горизонтальной коррекции координат букв (квадратиков).
				   GrafT[12][0]=0;
DM("@4_2_7");
	            if(mode>1)CorrectSqL();
				 // откорректировали координаты квадратиков по горизонтали.

/*		   // Запуск подпрограммы выделения вертикальных стеков в пределах строки.
				   GrafT[6][0]=1;  GrafT[6][1]=0;  GrafT[6][2]=0;
				   GrafT[6][6]=0;  // Визуализация букв темно синими квадратиками
				 #ifdef pageStackDetector
//                 pageStackDetector ();
				 #endif
				 // получили маркер на наличие стека вместо коренной буквы.
				 // Визуализация букв темно синими квадратиками из структур GrafT[6][6]=1; //

		   // Подпрограмма распознавания верхних огласовок в пределах строки
				   GrafT[7][0]=1;  GrafT[7][1]=0;
				 //VowelDetector();
				 // получили маркер (в структуре) на наличие верхних огласовок.
*/
/*
		   // Подпрограмма структурализации букв.
				   GrafT[8][0]=1;  GrafT[8][1]=0;
				 StructureWorld();
				 // получили основу букв

		   // Набор подпрограмм тестирования алгоритмов.
				   startTest();
*/
DM("@4_2_8");
//****************************************************************************//
	   // Визуализация букв темно синими квадратиками из структур //
			  wordDraw(0x00AA0011);    // темно синий
	   //	 if( GrafT[6][6] ) { wordDraw(0x00AA0011); }  // темно синий
	   // получили маркер (в структуре) на наличие стека вместо коренной буквы.
//****************************************************************************//


///        // Запуск подпрограммы для выделения Sheg в тексте печа.
///                 startShegDetector();

/**/
/*
#ifdef DEBUGLVL
   // ВИЗУАЛИЗАЦИЯ аналоговая исходных горизонтальных сумм (черная), для HStr[y]>=0
   int M=16; static short delta=50;
   for(y=0; y < ncnr; y++) { // ncnr // drawData->rows()
   for(x=delta; x<(HStr[y]/M)+delta; x++)drawData[0][y][x]*=0.1;} // 0.8*ncolumns;
DM(" szf "<<szf);
#endif
*/
DM("@4_2_9");	

             if(mode>1)data_normalisation();  //нормализует координаты колонок на странице.
	         stringNormalisation();
    
/**/
DM("@4_2_10");
TIME_PRINT

}//_____________________________________________________________________________





// Подпрограмма чистки (из за border) входного однобитового массива. ВХОД
// -----------------------------------------------------------------------------
void GBitset::data_null_border() {

  int y,x;
  int brd1,brd2,bit;
  int w=ncolumns;
  bool *d;
  bool  *A=bits_data;


//----------------------------------------------------------------------------//
				 // ОБНУЛЕНИЕ //

  KFStrok=0;     // номер оптимального коэффициента фильтрации строк
  KHigS=0;       // оптимальный параметр фильтрации строк
  DeltaNstr=0;   // вычисленная высота строки
  DeltaNstrKH=0; // вычисленная высота строки с учетом коэффициентов
				 // масштаба размера печа, KH=srcH/nrows;  KW=srcW/ncolumns;
  NLet=0;        // вычисленное колличество букв в строке, умноженное на 4
  Nstr=0;        // номер текущей обрабатываемой строки
  Nwrd=0;        // номер текущей обрабатываемой буквы
//DeltaKh=0;     // коэффициент формы страници (100*ncolumns/srcW*srcH/nrows;)
  NStrok=0;      // вычисленное колличество строк в печа
  NBlockP=0;     // вычисленное колличество блоков букв в печа
  hStrMax=0;     // вычисленное значение максимальной высоты строки на странице
  WordSMax=0;    // вычисленное значение максимального колличества букв
				 // в строке по по всей странице

  MeanSA=0;      // вычисленный средний по строке период букв умноженный на 10
  MeanCol=0;     // вычисленный средний по странице период букв умноженный на 10
  VarianceSA=0;  // вычисленная дисперсия периода букв умноженныя на 10private:
//ncnr=0;        // наибольшее из ncolumns и nrows те из ширины и высоты печа
 /* */

//szf=0;       // обычно в 32р системах sfz=4=32/8;  szf=sizeof(int);
// *ImBuf=0;    // запрос памяти с очисткой 0
 ImBf=0;      // текущий размер массива ImBuf
// *ReBuf=0;    // запрос памяти с очисткой 0
 ReBf=0;      // текущий размер массива ReBuf
 m=0;         // ширина двухмерного массива (если массивы так используем)
// *BufIn=0;    // запрос памяти с очисткой 0
 BfIn=0;      // текущий размер массива ImBuf
// *BufOu=0;    // запрос памяти с очисткой 0
 BfOu=0;      // текущий размер массива ImBuf

 TurnX=0;     // координата серидины разворота книги (по X).
 Brd=0;

 // аналоги кнопок   ( 1 - кнопока нажата ).
 //AutoBorderBtn=0;  // обработка скана в виде ОДНОГО БЛОКА.
 //SplitPagesBtn=0;  // обработка скана (разворотов книги) с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦЫ.
 //SplitColumBtn=0;  // обработка скана с РАЗДЕЛЕНИЕМ на файлы - КОЛОНКИ.

/* */
//----------------------------------------------------------------------------//

			Brd=border;
			bit=0;

		   /// аналоги кнопок   ( 1 - кнопока нажата ).
		   //  AutoBorderBtn, SplitPagesBtn, SplitColumBtn  глобальные
#ifdef WIN32_DRAW
		   // обработка скана в виде ОДНОГО БЛОКА.
			//if(ImageProcessor->AutoRemoveBorderBtn->Checked==true){ AutoBorderBtn=1; }

			// обработка скана (разворотов книги) с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦИ.
			//if(ImageProcessor->SplitPagesBtn->Checked==true){ SplitPagesBtn=1; }

			// обработка скана с РАЗДЕЛЕНИЕМ на файлы - КОЛОНКИ.
			//if(ImageProcessor->SplitColumnsBtn->Checked==true){ SplitColumBtn=1; }
			//	ShowMessage((AnsiString)"AutoBorderBtn"+IntToStr(AutoBorderBtn));
#endif
	
// если нажата хоть одна кнопка то чистка border иначе return
if( AutoBorderBtn || SplitPagesBtn || SplitColumBtn ){ /*ShowMessage(" AAA ");*/ }
else { /*ShowMessage(" BBB ");*/  return; }

			/// чистка (из за border) входного однобитового массива.

			  brd1=border-1;  brd2=border*1.25;
			  // если надо обрабатывать скан в виде ОДНОГО БЛОКА
			  // увеличиваем толщину защитной полосы.
			  if( AutoBorderBtn==1 ){ brd2=border*1.6; }


	 if( ncnr>900 ){  // в маленьких картинках не убраем полосами мусор с краев скана
			  // стирание нолями двух вертикальных полос
			  for ( y=brd1; y < nrows-brd1; y++ ) {
				d=A+y*w;
				for ( x=brd1; x < brd2; x++ ) {
					*(d+x)=bit;        // стирание нолями (белым-0) полосы
#ifdef DEBUGLVL_GREY
if( GrafT[15][1] ) { drawData[0][y][x]*=0.8; }
#endif
				} // for x
				for ( x=ncolumns-brd2; x < ncolumns-brd1; x++ ) {
					*(d+x)=bit;
#ifdef DEBUGLVL_GREY
if( GrafT[15][1] ) { drawData[0][y][x]*=0.8; }
#endif
				} // for x
			  } // for y



			  // стирание нолями двух горизонтальных полос
			  for ( y=brd1; y < brd2; y++ ) {
				d=A+y*w;
				for ( x=brd1; x < ncolumns-brd1; x++ ) {
					*(d+x)=bit;        // стирание нолями (белым-255) полосы
#ifdef DEBUGLVL_GREY
if( GrafT[15][1] ) { drawData[0][y][x]*=0.8; }
#endif
				} // for x
			  } // for y

			  for ( y=nrows-brd2; y < nrows-brd1; y++ ) {
				d=A+y*w;
				for ( x=brd1; x < ncolumns-brd1; x++ ) {
					*(d+x)=bit;        // стирание нолями (белым-255) полосы
#ifdef DEBUGLVL_GREY
if( GrafT[15][1] ) { drawData[0][y][x]*=0.8; }
#endif
				} // for x
			  } // for y

	} // if( ncnr>120 )
/**/


			 // убрали полосами мусор с краев скана + border

// != 0
}//_____________________________________________________________________________




// Подпрограмма нормализации strArray с учетом величины бордюра border. ВЫХОД
// -----------------------------------------------------------------------------
void GBitset::data_normalisation() {

  int x0,x1,y1;
  int nClm,nColumn;
  int dX=ncolumns-border*2;
  int dY=nrows-border*2;
  //int brd=border*2;
  int nFrame = (int)frameArray.size();

 // компенсация добавления border, примененным для увеличения размера малых сканов.

 // TurnX - координата серидины разворота книги (по X), глобальная.
 if( TurnX > 0 ) { TurnX-=border; }  // нужно сохранить значение TurnX=0;

 // прямоугольники вертикальных колонк.
 // xf0,xf1, yf0,yf1  координаты прямоугольника вокруг многоугольника вертикальной колонки.
 // xfg0,xfg1,yfg0,yfg1  координаты прямоугольника вокруг колонок (с добавлением бордюра Border).
 // xc0,xc1, yc0,yc1  координаты блоков, вокруг которых описан прямоугольник вертикальной колонки.
 // X, X, Y, Y  тоже что и  x, x, y, y, но без добавленого border.
 for (int nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
/*
		frameArray[nFrm].xfg0-=border;  if(frameArray[nFrm].xfg0<0)frameArray[nFrm].xfg0=0;
		frameArray[nFrm].yfg0-=border;  if(frameArray[nFrm].yfg0<0)frameArray[nFrm].yfg0=0;
		frameArray[nFrm].xfg1-=border;  if(frameArray[nFrm].xfg1>dX)frameArray[nFrm].xfg1=dX;
		frameArray[nFrm].yfg1-=border;  if(frameArray[nFrm].yfg1>dY)frameArray[nFrm].yfg1=dY;

		frameArray[nFrm].xf0-=border;   if(frameArray[nFrm].xf0<0)frameArray[nFrm].xf0=0;
		frameArray[nFrm].yf0-=border;   if(frameArray[nFrm].yf0<0)frameArray[nFrm].yf0=0;
		frameArray[nFrm].xf1-=border;   if(frameArray[nFrm].xf1>dX)frameArray[nFrm].xf1=dX;
		frameArray[nFrm].yf1-=border;   if(frameArray[nFrm].yf1>dY)frameArray[nFrm].yf1=dY;

		nColumn=frameArray[nFrm].columnArray.size();
		for ( nClm=nColumn-1; nClm >= 0; nClm-- ){ // Цикл по количеству блоков колонок Clm
		  frameArray[nFrm].columnArray[nClm].xc0-=border; if(frameArray[nFrm].columnArray[nClm].xc0<0)frameArray[nFrm].columnArray[nClm].xc0=0;
		  frameArray[nFrm].columnArray[nClm].yc0-=border; if(frameArray[nFrm].columnArray[nClm].yc0<0)frameArray[nFrm].columnArray[nClm].yc0=0;
		  frameArray[nFrm].columnArray[nClm].xc1-=border; if(frameArray[nFrm].columnArray[nClm].xc1>dX)frameArray[nFrm].columnArray[nClm].xc1=dX;
		  frameArray[nFrm].columnArray[nClm].yc1-=border; if(frameArray[nFrm].columnArray[nClm].yc1>dY)frameArray[nFrm].columnArray[nClm].yc1=dY;
		} // Цикл по количеству блоков колонок nClm
*/

		frameOCR *wF=&frameArray[nFrm];
	           int y0=nrows-wF->yf0;
	           wF->yf0=nrows-wF->yf1;    wF->yf1=y0;
	 
		x0=wF->xf0;    x1=wF->xf1;    y0=wF->yf0;    y1=wF->yf1;
		wF->Xf0=wF->xf0=x0-=border;  if ( x0 < 0 ) { wF->xf0=0; }
		wF->Xf1=wF->xf1=x1-=border;  if ( x1 >dX ) { wF->xf1=dX;}
		wF->Yf0=wF->yf0=y0-=border;  if ( y0 < 0 ) { wF->yf0=0; }
		wF->Yf1=wF->yf1=y1+=border;  if ( y1 >dY ) { wF->yf1=dY;}

		wF->Xfg0=wF->xfg0=x0=wF->Xfg0-border;  if ( x0 < 0 ) { wF->xfg0=0; }
		wF->Xfg1=wF->xfg1=x1=wF->Xfg1-border;  if ( x1 >dX ) { wF->xfg1=dX;}
		wF->Yfg0=wF->yfg0=y0=wF->Yfg0-border;  if ( y0 < 0 ) { wF->yfg0=0; }
		wF->Yfg1=wF->yfg1=y1=wF->Yfg1+border;  if ( y1 >dY ) { wF->yfg1=dY;}

		/*
		wF->xfg0=x0=wF->Xfg0-border;  if ( x0 < 0 ) { wF->xfg0=0; }
		wF->xfg1=x1=wF->Xfg1-border;  if ( x1 >dX ) { wF->xfg1=dX;}
		wF->yfg0=y0=wF->Yfg0-border;  if ( y0 < 0 ) { wF->yfg0=0; }
		wF->yfg1=y1=wF->Yfg1-border;  if ( y1 >dY ) { wF->yfg1=dY;}

		wF->Xfg0-=border;
		wF->Xfg1-=border;
		wF->Yfg0-=border;
		wF->Yfg1-=border;
		*/


// DM(END); DM(wF->xfg0<<" xfg0 "); DM(wF->xfg1<<" xfg1 ");  DM(wF->yfg0<<" yfg0 "); DM(wF->yfg1<<" yfg1 "); DM(END);
// DM(END); DM(wF->Xfg0<<" Xfg0 "); DM(wF->Xfg1<<" Xfg1 ");  DM(wF->Yfg0<<" Yfg0 "); DM(wF->Yfg1<<" Yfg1 "); DM(END);

		 nColumn=(uint)frameArray[nFrm].columnArray.size();
		 //for ( nClm=nColumn-1; nClm >= 0; nClm-- ){ //  Цикл по количеству колонок nColumn
		 for ( nClm=0; nClm < nColumn; nClm++ ){
			columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
			x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;
			wC->Xc0=wC->xc0=x0-=border;  if ( x0 < 0 ) { wC->xc0=0; }//wC->Xc0;
			wC->Xc1=wC->xc1=x1-=border;  if ( x1 >dX ) { wC->xc1=dX; }//wC->Xc1;
			y0=nrows-y0;  y1=nrows-y1;
			wC->Yc0=wC->yc0=y0-=border;  if ( y0 < 0 ) { wC->yc0=0; } //wC->Yc0;
			wC->Yc1=wC->yc1=y1-=border;  if ( y1 >dY ) { wC->yc1=dY; }//wC->Yc1;
		 } // Цикл по количеству колонок nColumn
/**/
 } // Цикл по количеству колонок nFrame
/**/

}//_____________________________________________________________________________

////////////////////////////////////////////////////////////////////////////////

  //vector<int> HBuf(ncnr);      //nrows  max=6000    Imag  Real  ImBuf  ReBuf
  //vector<int> GBuf(ncnr);      //nrows              RBuf  SBuf  DBuf   FBuf


  ///vector<int> LStS(ncnr); // массив заполненный вертикальными дифференциальными суммами
  ///vector<int> HStS(ncnr); // массив заполненный горизонтальными дифференциальными суммами
  ///vector<int> HStr(ncnr); // массив заполненный горизонтальными суммами

/*
			  // в HStS кладем горизонтальные дифференциальные суммы
              // в HStr кладем горизонтальные дифференциальные суммы
              for ( y=0; y < nrows; y++ ) {      // read image make sum
                res=0; resD=0;  d1=A+y*w;        //  p=y*ncolumns;
                for ( x=0; x < ncolumns; x++ ) { // подсчет  горизонтальных сумм
                    //res+=bits_data[p + x];     ///  d=*(d1+x);
                    ///res+=pp=bits_data[p + x];
                    ///resD+=pp - bits_data[p + x +1];

                    d2=d1+x;        ///
                    d=*d2-*(d2+1);  ///
                    //d=*(d1+x);
                    resD+=d;
                    res+=*d2;

                }
                HStS[y]=resD<<5;  /// <<4 <<1   //
                HStr[y]=res;
                //HStr[y]=resD<<1;
//                drawData[0][y][x]=255+d*196;
              }
*/

///////////////
/*
  ImBuf=new int[ncnr+512];
  ReBuf=new int[ncnr+512];
  BufIn=new int[ncnr+512];
  BufOu=new int[ncnr+512];
*/
/*
q=new тип[n_el];  //запрос памяти под массив из n_el элементов; ImBuf=new int[ncnr+512];
q=new тип;        //запрос памяти под скалярную переменную;
delete q[n_el];   //освобождение памяти, занятой массивом;
delete q;         //освобождение памяти, занятой массивом или скалярной переменной; delete ImBuf;
q=NULL; // Поэтому к правилам "хорошего тона" в программировании относится и
        // сброс указателей после возврата динамически запрашивавшейся памяти:
*/
//           delete ImBuf;  delete ReBuf;  delete BufIn;  delete BufOu;
////////////////

/**/
// HStr(ncnr);   HBuf(ncnr);  GBuf(ncnr);  LStr(ncnr);   ncnr=max 6000
// for ( y=0; y < 64; y++ ) { GrafT[y]=0; }
// signed  char GrafT[32]={1,1,3,3,5,5,7,7};    Number functions    NFunct

// вычисленное колличество блоков букв во всем печа NBlockP
//if( NBlockP< 1 ) {  goto StartEND;  }
// завершение выполнения всех подпрограмм.  // {break;}
//                     StartEND:;
