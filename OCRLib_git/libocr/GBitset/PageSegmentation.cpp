#include "GBitset.h"

////////////////////////////////////////////////////////////////////////////////

//  ----------------------------------------------------------------------------
// Подпрограмма для получения координат вертикальных колонок в пределах разворота.
//  ----------------------------------------------------------------------------
void GBitset::PageSegmentation() {

// LStS, HStS, HBuf, GBuf переменной длинны, max из nrows или ncolumns,
// в LStS кладем вертикальные дифференциальные суммамы
// в HStS лежат горизонтальные дифференциальные суммамы

//  vector<int> LStr(ncolumns);
  //int y,x;
/*
// Оператор сдвига влево очень удобен для вычисления значения битов позиции:
(1 << 15)   // Куда проще, чем помнить, что бит 15 равен 32768!
*/

	   //DM(NBlockP<<" NBlockP "); DM(END);
	   // вычисленное колличество строк в печа - NStrok=strArray.size();
       // вычисленное колличество блоков букв во всем печа NBlockP
                   NStrok=(uint)strArray->size();
				   if( NStrok<3 || NBlockP==0 ) { return; }
//TIME_START


	   // Подпрограмма для получения координат вертикальных колонок по горизонтали.
    
    /// Подпрограмма для получения координат колонок по горизонтали с учетом вертикальных линий между колонками(страницами)
                   FrameDetectorL();
    
    /// Базовая подпрограмма для получения координат колонок по горизонтали без учета вертикальных линий между колонками(страницами)
                   //FrameDetectorLBase();

	   /// Программа для обработка скана (разворотов книги, turn the book),
	   /// только с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦИ.
			   if(SplitPagesBtn==1){ TurnBookDetector(); }
				 // в TurnX получили примерную координату по x серидины книги.

/*
	   /// Программа для обработка скана в виде ОДНОГО БЛОКА (н.создание DjVu файлов).
///			    if(AutoBorderBtn==1){ MonoBookDetector(); }
				   MonoBookDetector();
				 // получили прямоугольник описанный вокруг всех букв скана.
*/

	   /// Программы для обработка скана с РАЗДЕЛЕНИЕМ на файлы - КОЛОНКИ.
	   /// или обработка скана в виде ОДНОГО БЛОКА.
	if(SplitColumBtn==1 || AutoBorderBtn==1 ){ //cout<<"start_column"<<endl;
	   // Подпрограмма для получения координат Y прямоугольников вокруг вертикальных колонок
				   ColumnDetector();
				 // получили min и max координаты по Y в колонке с отбрасыванием
				 // пустых строк и строк с линиями.

	   // Подпрограмма для получения координат X,Y многоугольников f,fg вокруг вертикальной колонки.
				   ColumnWorking();
				 // получили координаты многоугольников f,fg вокруг вертикальной колонки.

	   // Подпрограмма для получения разных типов бордюров между многоугольниками f,fg.
				   BorderWorking();
				 // получили величину бордюра между многоугольниками f,fg вокруг вертикальной колонки.

	   // Подпрограмма для помещения колонок состоящих из блоков в структуру строк.
				   CopyBlock();
				 // получили координаты вертикальных колонок в структуре строк.

				} // if(SplitColumBtn==1)

	   // Подпрограмма для получения координат вертикальных колонок по вертикали.
	   // работает но не применили. СДВИНУТА ДЛЯ УДОБСТВА.
	   //          FrameDetectorH();

/**/


/*
  // ВИЗУАЛИЗАЦИЯ аналоговая исходных вертикальных сумм (черная), для LStr[x]>=0
  int M=64; static short delta=nrows*10/60; // 300
  for(x=0; x < ncolumns-1; x++) {
  for (y=delta; y<(LStS[x]/M)+delta; y++)drawData[0][y][x]*=0.1;} // 0.8*ncolumns;

*/


if( GrafT[11][3] ) { frameDraw(0x00ff00); }  // цвет линии зеленый  0x00ff00


if( GrafT[11][3] ) { lineDraw (0x00ff00); }  // цвет линии зеленый  0x00ff00


if( GrafT[11][0] ) { DM(END);  DM(1111111111<<" PageSegmentation ");  DM(END); }


//TIME_PRINT


// Визуализация прямоугольников в структурах для блоков букв

//if( GrafT[11][3] ) { blockDraw(0x000000FF); }  // цвет линии красный


/**/

}//_____________________________________________________________________________

//                              X0, X1

//  ----------------------------------------------------------------------------
// Базовая подпрограмма для получения координат колонок по горизонтали без учета вертикальных линий между колонками(страницами)
//  ----------------------------------------------------------------------------
void GBitset::FrameDetectorLBase() {
    
    int y,x,n,d;
    int Prg,maxbft,Nblck,scope=8;
    
    
    // в LStS кладем вертикальные дифференциальные суммамы
    GBuf=LStS;  // LStS
    
    // импульсный фильтр, 3 последовательных запуска.
    for ( n=0; n < 3; n++ ){
        for ( y=8; y < nrows-8; y++ ){     // <<7
            d=4*abs( GBuf[y]-GBuf[y-1] )/( 1 + abs(GBuf[y]-GBuf[y+1]) );
            if( 1 < d && d < 16 ) { GBuf[y]=(GBuf[y-1]+GBuf[y+1])/2; }
        } // y
    } // n
    /**/
    /*
     // средний по всем строкам страници страници период букв MeanCol
     NStrok=strArray->size();
     if ( NStrok > 0 ) { MeanCol=MeanCol/NStrok; }
     else  MeanCol=1;
     DM(MeanCol<<" MeanCol ");
     */
    
    // уточненое значение козффициента сглаживания для  фильтра SP->filterHigh
    // KHigAdapt адаптивный по высоте печа (чем меньше высота тем сильнее сглаживание)
    int MeanMin=MeanCol*0.5, MeanMax=MeanSA*4; // KHig, KLow, MeanCol- глобальные.  MeanMin=MeanCol*0.5
    int NStr=(int)strArray->size();
    int KHigAdapt=MeanMax/(0.12*NStr*NStr+1)+MeanMin;
    if ( KHigAdapt > MeanMax ) { KHigAdapt=MeanMax; }
    
    //****************
    // если нет строк (печа расположены вертикально) то MeanMax задаем вручную При размере печа выше среднего (400дпи)  400
    // Максимально вожможный коэффициент сглаживания равен 80
    MeanMax=80;  // ручная установка MeanMax
    KHigAdapt=80; // ручная установка KHigAdapt
    //DR_(END); DR_(KHigAdapt<<" KHigAdapt "); DR_(MeanMax<<" MeanMax "); DR_(END);
    //****************

    
    /**/
    
    // сглаживание блоков слов - ширина колонок ( KHg  уже адаптивный )
    SP->filterHigh(GBuf,KHigAdapt);  // SP->filterHigh(GBuf,MeanCol*0.5);  *0.6 *1.8;
    
    // зеленый, вызов горизональной функции рисования переменной длинны
    if( GrafT[11][3] ) { WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x0000AA00); }
    
    HBuf=GBuf;
    
    // сильное сглаживание для получение максимума HBuf
    SP->filterHigh(HBuf,MeanMax);  //  SP->filterHigh(HBuf,MeanSA*4);
    
    // красный, вызов горизональной функции рисования переменной длинны
    if( GrafT[11][3] ) { WDrawVar(HBuf,ncolumns,nrows/16,scope*6,0x000000FF); }
    
    // получение максимума HBuf
    maxbft=0;
    for ( x=8; x < ncolumns-8; x+=4 ){  // 8 пикселей от края печа не используются.
        if( HBuf[x] > maxbft ) { maxbft=HBuf[x]; }
    }// for x
    
    // добавить ограничение на абсолютное значение величины порога
    // Prg=maxbft*0.2;   if( Prg > k*2(y1-y0) ) { Prg=k*2(y1-y0); }
    // может сработать на горизонтальных печа
    // получение порога по максимуму HBuf
    Prg=maxbft*0.2;  //  1/4
    if ( Prg < 4 ) {  return;  } /////////////////////////////////////////////////////////
    // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
    for ( x=2; x < ncolumns-2; x++ ){  // 2 пикселей от края печа не используются.
        if( GBuf[x] >= Prg ) { GBuf[x]=Prg; }
        else { GBuf[x]=0; }
    }// for x
    
    HBuf.assign(ncnr,0);
    // получение координат блоков слов (колонок) на развороте
    d=0;              
    for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
        if( GBuf[x]==Prg && GBuf[x-1]==0 ){ HBuf[d]=x; }
        if( GBuf[x]==0 && GBuf[x-1]==Prg ){ HBuf[d+1]=x; d+=2; }
    }// for x
    Nblck=d;
    
    // MeanCol - средее по всем строкам страници расстояние между центрами масс букв
    int DMn=MeanCol*0.4+2;
    
    // если надо обрабатывать скан в виде ОДНОГО БЛОКА
    // объединяем все блоки в один блок.
    if( AutoBorderBtn==1 ){
        HBuf[0]=HBuf[0] - DMn;  HBuf[1]=HBuf[Nblck-1] + DMn;  Nblck=1;
    }
    
    // Заполнение структур координатами блоков слов по размеру колонок
    d=1;
    for ( y=0; y < Nblck; y+=2 ){
        // ограничение на минимальную ширину колонок
        if( HBuf[y+1]-HBuf[y] > MeanCol*4 ) {  // *2 включаются маленькие колонки
            frameOCR wF;
            // xf0,xf1  левые и правые координаты прямоугольника вокруг
            // многоугольника вертикальной колонки
            // xfg0,xfg1  внешние габариты вокруг многоугольника вертикальной колонки
            wF.Xfg0=wF.xfg0=wF.Xf0=wF.xf0=HBuf[y];   // - DMn/2
            wF.Xfg1=wF.xfg1=wF.Xf1=wF.xf1=HBuf[y+1]; // + DMn/2
            // yf0,yf1  координаты низа и верха прямоугольника вокруг вертикальной колонки
            // yfg0,yfg1  внешние габариты вокруг многоугольника вертикальной колонки
            wF.Yfg0=wF.yfg0=wF.Yf0=wF.yf0=100;
            wF.Yfg1=wF.yfg1=wF.Yf1=wF.yf1=nrows-100;
            // xbr0,xbr1,ybr0,ybr1; зазор (бордюр) между многоугольниками f и fg вертикальной колонки.
            wF.xbr0=wF.xbr1=wF.ybr0=wF.ybr1=0;
            wF.frameFlag=d;  d++; // установка флага блоков слов (номер колоноки)
            frameArray.push_back(wF);
            DM( " xf0="<<wF.xf0<< " yf0="<<wF.yf0<< " xf1="<<wF.xf1<< " yf1="<<wF.yf1<< "\n---\n");
        }
    }// for y
    /**/
    
    //int nFrame = frameArray.size();
    DM(END); DM(nFrame<<" nFrame "); DM(Nblck<<" Nblck "); DM(END);
    
#ifdef DEBUGLVL
    if( GrafT[11][3] ) {
        // ВИЗУАЛИЗАЦИЯ аналоговая
        WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны
        
        // ВИЗУАЛИЗАЦИЯ цифровая
        DM(END); DM(KHigAdapt<<" KHigAdapt "); DM(MeanMin<<" MeanMin "); DM(END);
        DM(END); DM(MeanCol<<" MeanCol "); DM(END);
        DM(Nblck<<" Nblck ");  DM(maxbft<<" maxbft "); DM(Prg<<" Prg "); DM(END);
        
        
        /// ТАБЛИЦА
        DM(END);
        for ( y=1; y < Nblck; y++  ){
			DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
        }//for y
        DM(END);
        
#ifdef DEBUGLVL_GREY
        int M=64; static short delta=nrows*10/120; // 300
        for(x=0; x < ncolumns-1; x++) {
            for (y=delta; y<(LStS[x]/M)+delta; y++)drawData[0][y][x]*=0.1;
        } // 0.8*ncolumns;
        
#endif    
    }    
    
    
    // Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
    ////if( GrafT[11][3] ) { columnDraw(0x00ff00); }  // цвет линии зеленый
    
#endif
    
    /**/
    
    // Подпрограмма выделения рамки вокруг текста печа.
}//_____________________________________________________________________________





//                              X0, X1

//  ----------------------------------------------------------------------------
// Подпрограмма для получения координат колонок по горизонтали с учетом вертикальных линий между колонками(страницами)
//  ----------------------------------------------------------------------------
void GBitset::FrameDetectorL() {

  int y,x,d;
  int Prg,maxbft,Nblck,scope=8;
  LineDetectArray.resize(ncolumns);  // массив "чистых" вертикальных сумм (без масок строк) 
    
        // обнуление массива
        memset(&LineDetectArray[0],0,4*ncolumns);

  	    // массив LStS кладем вертикальные дифференциальные суммы с учетом масок 
		GBuf=LStS;
    

    // подсчет вертикальных сумм для увеличения влияния вертикальных линий на точность
    // разделения на колонки
///TIME_START // time=0.12    
    int sum=0;
    int x0=ncolumns/16+border;
    int x1=ncolumns-ncolumns/16-border;
    bool *p0=bits_data+x0;
    for (x=x0; x < x1; x++ ){        
        p0++;  sum=0;
        for (y=0; y < data_size; y+=ncolumns ){
            sum+=*(p0+y);
        } 
        LineDetectArray[x]=sum;
    } // x
///TIME_PRINT_
    
    // подсчет суммарной высоты строк
    int LimitH=0;
    for(int i=0;i<NStrok;i++)  LimitH+=(strArray[0][0].y1-strArray[0][0].y0);
    LimitH*=(float)2.2;
    DC("LimitH="<<LimitH<<endl);
    
    // добавляем результаты вертикальных сумм с обратным знаком, если они выше порога
    for ( x=x0; x < x1; x++ ){     // <<7
        if(LineDetectArray[x]>LimitH)GBuf[x]-=LineDetectArray[x]*256;   
    } // x
    
    //убираем шум по краю страницы.
    //Затираем начало и конец массива вертикальных дифференциальных сумм
    memset(&GBuf[0],0,x0*4);       memset(&GBuf[x1],0,(ncolumns-x1)*4);



		  // уточненое значение козффициента сглаживания для  фильтра SP->filterHigh
		  // KHigAdapt адаптивный по высоте печа (чем меньше высота тем сильнее сглаживание)
		  int MeanMin=MeanCol*0.5, MeanMax=MeanSA*4; // KHig, KLow, MeanCol- глобальные.  MeanMin=MeanCol*0.5
		  int NStr=(int)strArray->size();
          //////////////////////////////////// 
          int KHigAdapt=1.0*(MeanMax/(0.12*NStr*NStr+1)+MeanMin);  // *1.0 самый первый вариант - standart и если колонки очень близко к друг другу и текст плотный (декабрь 2014).
          //  *3.0 для более разреженного текста и колонок не в притык друг к другу, это для подходит для большинства текстов (ноябрь 2012).
		  if ( KHigAdapt > MeanMax ) { KHigAdapt=MeanMax; }
/**/
          // Особенность, при KHigAdapt=3.0* сглаживание KHigAdapt и 
          // сильное сглаживание KHigAdapt практически одинаковы  
		  // сглаживание блоков слов - ширина колонок ( KHg  уже адаптивный )
		  SP->filterHigh(GBuf,KHigAdapt);  // SP->filterHigh(GBuf,MeanCol*0.5);  *0.6 *1.8;

		  // зеленый, вызов горизональной функции рисования переменной длинны
if( GrafT[11][3] ) { WDrawVar(GBuf,ncolumns,nrows-nrows/16,scope*6,0x0000AA00); }

    
                            HBuf=GBuf;

          // сильное сглаживание для получение максимума HBuf
		  SP->filterHigh(HBuf,MeanMax);  //  SP->filterHigh(HBuf,MeanSA*4);

          // красный, вызов горизональной функции рисования переменной длинны
if( GrafT[11][3] ) { WDrawVar(HBuf,ncolumns,nrows/16,scope*6,0x000000FF); }

          // получение максимума HBuf
          maxbft=0;
          for ( x=x0; x < x1; x+=4 ){  // 8 пикселей от края печа не используются.
                   if( HBuf[x] > maxbft ) { maxbft=HBuf[x]; }
          }// for x

 // добавить ограничение на абсолютное значение величины порога
 // Prg=maxbft*0.2;   if( Prg > k*2(y1-y0) ) { Prg=k*2(y1-y0); }
 // может сработать на горизонтальных печа
		  // получение порога по максимуму HBuf
		  Prg=maxbft*0.2;  //  1/4
		  if ( Prg < 4 ) {  return;  } /////////////////////////////////////////////////////////
		  // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
		  for ( x=2; x < ncolumns-2; x++ ){  // 2 пикселей от края печа не используются.
                if( GBuf[x] >= Prg ) { GBuf[x]=Prg; }                       
                else { GBuf[x]=0; }
          }// for x

    if( GrafT[11][3] ) { WDrawVar(LineDetectArray,ncolumns,5,scope*6,0x0000AA00); }
    if( GrafT[11][3] ) { WDrawVar(GBuf,ncolumns,nrows/10,scope*6,0x0000AA00); }
    
          HBuf.assign(ncnr,0);
          // получение координат блоков слов (колонок) на развороте
          d=0; 
		  for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
                   if( GBuf[x]==Prg && GBuf[x-1]==0 ){ HBuf[d]=x; }
                   if( GBuf[x]==0 && GBuf[x-1]==Prg ){ HBuf[d+1]=x; d+=2; }
		  }// for x
		  Nblck=d;

  // MeanCol - средее по всем строкам страници расстояние между центрами масс букв
  int DMn=MeanCol*0.4+2;

		  // если надо обрабатывать скан в виде ОДНОГО БЛОКА
		  // объединяем все блоки в один блок.
		  if( AutoBorderBtn==1 ){
			HBuf[0]=HBuf[0] - DMn;  HBuf[1]=HBuf[Nblck-1] + DMn;  Nblck=1;
		  }

  // Заполнение структур координатами блоков слов по размеру колонок
  d=1;
  for ( y=0; y < Nblck; y+=2 ){

         frameOCR wF;
		 // xf0,xf1  левые и правые координаты прямоугольника вокруг
         // многоугольника вертикальной колонки
		 // xfg0,xfg1  внешние габариты вокруг многоугольника вертикальной колонки
		 wF.Xfg0=wF.xfg0=wF.Xf0=wF.xf0=HBuf[y];   // - DMn/2
		 wF.Xfg1=wF.xfg1=wF.Xf1=wF.xf1=HBuf[y+1]; // + DMn/2
		 // yf0,yf1  координаты низа и верха прямоугольника вокруг вертикальной колонки
         // yfg0,yfg1  внешние габариты вокруг многоугольника вертикальной колонки
		 wF.Yfg0=wF.yfg0=wF.Yf0=wF.yf0=100;
		 wF.Yfg1=wF.yfg1=wF.Yf1=wF.yf1=nrows-100;
		 // xbr0,xbr1,ybr0,ybr1; зазор (бордюр) между многоугольниками f и fg вертикальной колонки.
		 wF.xbr0=wF.xbr1=wF.ybr0=wF.ybr1=0;
		 wF.frameFlag=d;  d++; // установка флага блоков слов (номер колоноки)
		 frameArray.push_back(wF);
      DM( "/@/_y="<<y<<" xf0="<<wF.xf0-border<< " yf0="<<wF.yf0-border<< " xf1="<<wF.xf1-border<< " yf1="<<wF.yf1-border<<" brd="<<border<< "\n---\n");
   }// for y
/**/

DM(END); DM(frameArray.size()<<" nFrame "); DM(Nblck<<" Nblck "); DM(END);
///DR_("++++++ nFrame="<<frameArray.size()<<"\n---\n");
    
#ifdef DEBUGLVL
if( GrafT[11][3] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны

					  // ВИЗУАЛИЗАЦИЯ цифровая
 DM(END); DM(KHigAdapt<<" KHigAdapt "); DM(MeanMin<<" MeanMin "); DM(END);
				 DM(END); DM(MeanCol<<" MeanCol "); DM(END);
 DM(Nblck<<" Nblck ");  DM(maxbft<<" maxbft "); DM(Prg<<" Prg "); DM(END);


					  /// ТАБЛИЦА
			DM(END);
			for ( y=1; y < Nblck; y++  ){
			DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
			  }//for y
			DM(END);

#ifdef DEBUGLVL_GREY
    int M=64; static short delta=nrows*10/120; // 300
    for(x=0; x < ncolumns-1; x++) {
        for (y=delta; y<(LStS[x]/M)+delta; y++)drawData[0][y][x]*=0.1;
    } // 0.8*ncolumns;
    
#endif  
    
}    

    
// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
////if( GrafT[11][3] ) { columnDraw(0x00ff00); }  // цвет линии зеленый

#endif

/**/

// Подпрограмма выделения рамки вокруг текста печа.
}//_____________________________________________________________________________



//                              TurnX

// сделать доп. версию с суммами не по квадратикам  а по исходным вертикальным
// суммам, поверить на  _Test_str_Kochergena
// - вертикальные суммы считаем без квадратиков, но с подавлением вертикальный линий
// BufL[x]>>3; - вертикальные дифф. суммы
// - если выделенна всего одна колонка, то TurnX считаем по ближайшей к центру скана
// стороне колонки.

// программа определения разворота книги, для деления на колонки начиная с разворота.
//  ----------------------------------------------------------------------------
void GBitset::TurnBookDetector() {

  //int y,x;
  int xf0,xf1;           // координаты прямоугольника вокруг колонок.
  int nFrm,nFrame,widX,maXwx;    // ,nClm,nColumn ,TurnX

  nFrame = (int)frameArray.size();
  maXwx=0.4*MeanCol;// min расстояние между прямоугольников описанных вокруг колонок обычно *1, один раз было *2,
                    // если колонки очень близко и текст плотный, то *0.4.(декабрь 2014)  /////////////////////////
  //DR_("++++++ nFrame="<<frameArray.size()<<"\nn");
    
  if( nFrame > 1 ) {
     for ( nFrm=1; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
                                            // без первой колонки.
		 xf0=frameArray[nFrm].xf0;  xf1=frameArray[nFrm-1].xf1;      //  [0]
         widX=xf0 - xf1;
#ifdef DEBUGLVL
         if( GrafT[11][3] )LineVertical(xf0, 0xffff00);
         if( GrafT[11][3] )LineVertical(xf1, 0xffff00);
#endif
		 if( widX > maXwx ) { maXwx=widX; TurnX=(xf1 + xf0)/2; }
         //DR_("++++++ xf0="<<xf0<<" xf1="<<xf1<<"  widX="<<widX<<" MeanCol="<<MeanCol<<" TurnX="<<TurnX<<"\n---\n");

     } // Цикл по количеству колонок nFrame.
  } // if
  else  { TurnX=0; }

  // TurnX - координата серидины разворота книги (по X), глобальная.
  // TurnXL=set->TurnX;   

#ifdef DEBUGLVL
if( GrafT[11][3] ) {

// ВИЗУАЛИЗАЦИЯ аналоговая, вызов функции рисования вертикальной линии
                LineVertical(TurnX, 0xffff00);  // салатовый
                        // ВИЗУАЛИЗАЦИЯ цифровая
                DM(END); DM(TurnX<<" TurnX "); DM(END);
}
#endif
/**/
//DM("y="<<y<<END);
}//_____________________________________________________________________________

//              *****************************************
//              *                                       *
//              *                                       *
//              *                                       *
//              *****************************************

//                              MONO BLOK

/// Программа для обработка скана в виде ОДНОГО БЛОКА (н.создание DjVu файлов).
//  ----------------------------------------------------------------------------
void GBitset::MonoBookDetector() {

  int yf0,yf1;         // координаты прямоугольника вокруг вокруг всех букв скана.
  int xb0,xb1; // ,yb0,yb1     // координаты прямоугольника вокруг буквы (блока)
  int Nblc,Nblock;
  int xbMax,xbMin, ysMax,ysMin;


	 xbMax=8; xbMin=ncolumns-8;  ysMax=8; ysMin=nrows-8;
	 NStrok = (uint)strArray->size();
	 for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Цикл по количеству строк NStrok
		 stringOCR *wS=&strArray[0][Nstr];
		 // yf0,yf1 координаты лимитов (LimY0,LimY1) строк
		 yf0=wS->LimY0; 	yf1=wS->LimY1;
		 if ( yf0 < ysMin ) { ysMin=yf0; }  // т.е. max LimY0 во всем скане
		 if ( yf1 > ysMax ) { ysMax=yf1; }  // т.е. max LimY1 во всем скане

		  Nblock = (uint)strArray[0][Nstr].blockArray.size();
		  for ( Nblc=0; Nblc < Nblock; Nblc++ ){ // Цикл по количеству букв (блока)
			   blockOCR *wB=&strArray[0][Nstr].blockArray[Nblc];
			   // xb0,xb1, yb0,yb1 координаты прямоугольника вокруг буквы (блока)
			   xb0=wB->xb0;   xb1=wB->xb1;  // yb0=wB->yb0;   yb1=wB->yb1;
			   if ( xb0 < xbMin ) { xbMin=xb0; }  // т.е. min xb0 во всем скане
			   if ( xb1 > xbMax ) { xbMax=xb1; }  // т.е. max xb1 во всем скане
		  } // Цикл по количеству блоков букв Nblock

	 } // Цикл по количеству строк NStrok


////     nFrame = frameArray.size();
////	 frameArray[0].xf0=xbMin;    frameArray[nFrame-1].xf1=xbMax;


	 // получили прямоугольник описанный вокруг всех букв скана.




/*
#ifdef DEBUGLVL
if( GrafT[11][3] ) {

// ВИЗУАЛИЗАЦИЯ аналоговая, вызов функции рисования вертикальной линии
				   LineVertical(TurnX, 0xffff00);  // салатовый
						// ВИЗУАЛИЗАЦИЯ цифровая
//                 DM(END); DM(TurnX<<" TurnX "); DM(END);
}
#endif
*/

}//_____________________________________________________________________________


//         *********************************************************

//
// подпрограмма получения координат Y прямоугольников вокруг вертикальных колонок.
// -----------------------------------------------------------------------------
void GBitset::ColumnDetector() {

  int y0,y1;
  int xf0,xf1,yf0,yf1;        // координаты прямоугольника вокруг колонок
  int xb0,xb1,yb0,yb1;        // координаты прямоугольника вокруг букв (блоков)
  int nFrm,nFrame, Nblc,Nblock;
  int ybMaxC,ybMinC, xbMaxS,xbMinS;
  int Lxf1,Pxf0, stringFlag;



 // получение координат Y прямоугольников вокруг вертикальных колонок.
 // с отбрасыванием пустых строк в колонке и строк с линиями (stringFlag==0).
 nFrame = (uint)frameArray.size();
 for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
	 frameOCR *wF=&frameArray[nFrm];
	 xf0=wF->xf0;   xf1=wF->xf1;   yf0=wF->yf0;   yf1=wF->yf1;

	 ybMaxC=8; ybMinC=nrows-8;
	 NStrok = (uint)strArray->size();
	 for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Цикл по количеству строк NStrok

		  // stringFlag флаг наличия в строке линии, получено в ShortStringDetector()
		  stringFlag=strArray[0][Nstr].stringFlag;
//////// если нажата кнопка: обработка скана в виде ОДНОГО БЛОКА, то линии не стираются
		  if(AutoBorderBtn==1){ stringFlag=1; }

		  xbMaxS=8;  xbMinS=ncolumns-8;
		  Nblock = (uint)strArray[0][Nstr].blockArray.size();
		  for ( Nblc=0; Nblc < Nblock && stringFlag==1; Nblc++ ){ // Цикл по количеству блоков букв Nblock

			   blockOCR *wB=&strArray[0][Nstr].blockArray[Nblc];
			   // xb0,xb1, yb0,yb1 координаты прямоугольника вокруг блока букв wB
			   xb0=wB->xb0;   xb1=wB->xb1;   yb0=wB->yb0;   yb1=wB->yb1;

				 // отработка ситуации когда блок букв длиннее ширины колонки
				 // на величину (2*MeanCol + 2)
				 if ( nFrm != 0 ) { Lxf1=frameArray[nFrm-1].xf1; }
				 else { Lxf1=xf0 - 2*MeanCol + 2; }
				 if ( nFrm != nFrame-1 ) { Pxf0=frameArray[nFrm+1].xf0; }
				 else { Pxf0=xf1 + 2*MeanCol - 2; }
/// DM(END); DM(Lxf1<<" Lxf1 ");   DM(Pxf0<<" Pxf0 ");

				 // отработка ситуации когда блок букв ограничен только шириной скана
				 // и если нажата кнопка: обработка скана в виде ОДНОГО БЛОКА

				// xf0,xf1, yf0,yf1;  координаты прямоугольника вокруг вертикальной
				// колонки wF, касающийся краёв горизонтальных блоков букв
				if ( xb1 > xf0 && xb1 < Pxf0 && xb0 < xf1 && xb0 > Lxf1 ) {
				//if ( xb1 > xf0 && xb1 < Pxf0 && xb0 < xf1 && xb0 > Lxf1 ) {
				  //  xb0,xb1, yb0,yb1;  координаты горизонтальных блок букв wC
				  if ( yb1 > ybMaxC ) { ybMaxC=yb1; }  // т.е. max yb1 в колонке
				  if ( yb0 < ybMinC ) { ybMinC=yb0; }  // т.е. min yb0 в колонке
				  if ( xb1 > xbMaxS ) { xbMaxS=xb1; }  // т.е. max xb1 в строке*колонке
				  if ( xb0 < xbMinS ) { xbMinS=xb0; }  // т.е. min xb0 в строке*колонке
				} // if xb1
		  } // Цикл по количеству блоков букв Nblock

		// xr0,xr1  координаты всех блоков, вокруг которых описан прямоугольник вертикальной колонки
		recordOCR wR;
		wR.xr0=xbMinS;  wR.xr1=xbMaxS;
		frameArray[nFrm].recordArray.push_back(wR);

	 } // Цикл по количеству строк NStrok


	 // yf0,yf1 координаты низа и верха прямоугольника вокруг вертикальной колонки
	 wF->yf1=yf1=ybMaxC;    wF->yf0=yf0=ybMinC;   //wF->xf1=xbMaxS;    wF->xf0=xbMinS;


	 /// заполнение структуры горизонтальными блоками описывающих многоугольники колонок
	 /// создание пустых блоков wC вокруг которых описан прямоугольник вертикальной колонки
	 for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Цикл по количеству строк NStrok
		 stringOCR *wS=&strArray[0][Nstr];
		 // y0,y1 координаты прямоугольника попадающие в Max и Min Y вертикальной колонки
		 // yf0,yf1, xf0,xf1 координаты прямоугольника вокруг вертикальной колонки
		 y0=wS->y0;   y1=wS->y1;
		 if ( y0>yf0 && y1<yf1 ) {
			columnOCR wC;
			// увеличение высоты горизонтальных блоков до лимитов (LimY1-LimY0)
			// xс0,xс1 координаты прямоугольника по Max и Min X блоков букв в строке
			wC.yc1=wS->LimY0;
			wC.yc0=wS->LimY1;
			wC.xc0=frameArray[nFrm].recordArray[Nstr].xr0;
			wC.xc1=frameArray[nFrm].recordArray[Nstr].xr1;
			wC.columnFlag=nFrm+1;  // установка флага горизонтальных блоков
			frameArray[nFrm].columnArray.push_back(wC);
		 }  // if ( y0>yf0

	 } // Цикл по количеству строк NStrok

 } // Цикл по количеству колонок nFrame


// получили min и max координаты по Y в колонке с отбрасыванием пустых строк и строк с линиями.


#ifdef DEBUGLVL
if( GrafT[11][3] ) {
					  // ВИЗУАЛИЗАЦИЯ цифровая
///DM(END); DM(ybMaxC<<" ybMaxC "); DM(ybMinC<<" ybMinC "); DM(END);
}
#endif

// флаг принадлежности к колонке
}//_____________________________________________________________________________


//                            ColumnWorking

//                                             ***     **     ***
//                                             **      **     **
//                                             ***     **     ***
//                                             ***     *      ***
//                                             * *     **     ***
//                                             ***     **     ***
// Подпрограмма для получения координат X,Y многоугольников f,fg вокруг
// вертикальной колонки. Pабота внутри каждой колонки по очереди
// -----------------------------------------------------------------------------
void GBitset::ColumnWorking() {

    int xc0,yc0,xc1,yc1;  // координаты горизонтальных блоков описывающих многоугольники колонок
  int xf0,xf1,yf0,yf1;  // координаты прямоугольника вокруг колонок, касающийся краёв горизонтальных блок букв
  int nFrm,nFrame, nClm,nColumn;
  int xcMaxS,xcMinS, ycMaxC,ycMinC,ycMaxCN,ycMinCN;


  // адапдивный защитный интервал вокруг колонок
  // int Delta=MeanCol*0.3 + 2;  // *0.4
  int Delta=ncnr/384 + MeanCol*0.15 + 2 ;
//  DM(END); DM(ncnr/384<<" ncnr/384 "); DM(MeanCol*0.15<<" MeanCol*0.15 ")
//  DM(Delta<<" Delta "); DM(END);
/**/


 // Получение координат X,Y многоугольников f,fg вокруг вертикальной колонки.
 nFrame = (int)frameArray.size();
 for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
	 xcMaxS=0; xcMinS=ncolumns;   ycMaxC=0; ycMinC=nrows;
	 frameOCR *wF=&frameArray[nFrm];
	 xf0=wF->xf0;   xf1=wF->xf1;   yf0=wF->yf0;   yf1=wF->yf1;

	 nColumn=(int)frameArray[nFrm].columnArray.size();
	 for ( nClm=0; nClm < nColumn; nClm++ ){ //  Цикл по количеству колонок nColumn
		columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
		xc0=wC->xc0;   xc1=wC->xc1;   yc0=wC->yc0;   yc1=wC->yc1;

		// подтягивание координат X малых горизонтальных блоков до границы
		// прямоугольника описывающего колонку, добавление интервала.
		if ( xc1 < xf1 ){ xc1=xf1; } wC->xc1=xc1=xc1 + Delta;
		if ( xc0 > xf0 ){ xc0=xf0; } wC->xc0=xc0=xc0 - Delta;

		// xcMaxS,xcMinS  внешние габариты прямоугольника (fg, левые и правые)
		// вокруг многоугольника вертикальной колонки, соприкасающющегося
		// с краями горизонтальных блоков букв
		if ( xc1 > xcMaxS ) { xcMaxS=xc1; }  // т.е. max xc1 по всей колонке
		if ( xc0 < xcMinS ) { xcMinS=xc0; }  // т.е. min xc0 по всей колонке

		// получение MAX и MIN и их адресов для внешних габаритов (f,fg)
		// прямоугольника вокруг многоугольника вертикальной колонки
		if ( yc1 > ycMaxC ) { ycMaxC=yc1; ycMaxCN=nClm; }  // т.е. max yc1 в колонке
		if ( yc0 < ycMinC ) { ycMinC=yc0; ycMinCN=nClm; }  // т.е. min yc0 в колонке
#ifdef DEBUGLVL
if( GrafT[11][3] ) { DM(nClm<<" nClm "); DM(yc0<<" yc0 ") DM(yc1<<" yc1 ") DM(END); }
#endif
	 } // Цикл по количеству колонок nColumn
//DM(END);


   // БОЛЬШИЕ прямоугольники Y.
   // yf0, yf1 координаты прямоугольника вокруг вертикальной
   // колонки, касающийся краёв горизонтальных блок букв.
   // проверка выхода координат прямоугольника за граници массива.
	 yf1=yf1 + Delta*2.1;   if ( yf1 >= nrows ){ yf1=nrows-1; }
	 yf0=yf0 - Delta*2.1;   if ( yf0 <  1 ){ yf0=1; }
   // увеличение высоты самого первого и последнего прямоугольника
	 wF->yf1=frameArray[nFrm].columnArray[ycMaxCN].yc1=yf1;
	 wF->yf0=frameArray[nFrm].columnArray[ycMinCN].yc0=yf0;

   // БОЛЬШИЕ прямоугольники X.
   // проверка выхода за за граници картинки, прямоугольника вокруг вертикальной
   // колонки, касающийся краёв горизонтальных блок букв.
	 if ( xcMaxS >= ncolumns ){ xcMaxS=ncolumns-1; }
	 if ( xcMinS <  1 ){ xcMinS=1; }
	 wF->xf1=xcMaxS;
	 wF->xf0=xcMinS;

 } // Цикл по количеству колонок nFrame


/// получили координаты X,Y многоугольников f,fg вокруг вертикальной колонки.

/**/


// координаты прямоугольника вокруг колонок, не касающющегося краёв горизонтальных блоков букв
//  int yfg0,yfg1,xfg0,xfg1;

}//_____________________________________________________________________________


//                            BorderWorking

//                                             *************
//                                             *           *
//                                             *           *
//                                             *           *
//                                             *           *
//                                             *************
// Подпрограмма для получения разных типов бордюров между многоугольниками f,fg
// вокруг вертикальной колонки.
// -----------------------------------------------------------------------------
void GBitset::BorderWorking() {

  int xf0,xf1,yf0,yf1;    // координаты прямоугольника вокруг колонок, касающийся краёв горизонтальных блок букв
  int xfg0,xfg1,yfg0,yfg1;// координаты прямоугольника вокруг колонок, не касающющегося краёв горизонтальных блоков букв
  int xbr0,xbr1,ybr0,ybr1;// зазор (бордюр) между многоугольниками вертикальной колонки, индивидуальные бордюры по x,y
  int nFrm,nFrame;
  int Border;
  //int BorderMax=2000;     // значения Border не м.б. > BorderMax.


  // значение Border по умолчанию, когда во всех чекбоксах ноли но какая-то кнопка выбрана
  // адапдивный защитный интервал вокруг колонок (Delta)
  int Delta=ncnr/384 + MeanCol*0.15 + 2 ;
  xbr0=xbr1=ybr0=ybr1=Delta;  // 16  // Delta
  // но реально ниже (change) происходит центровка исходного скана

  // по размеру исходного скана, блок текста автоматически сцентрирован
  //frameOCR *wF=&frameArray[0];
  //xf0=wF->xf0;     xf1=wF->xf1;     yf0=wF->yf0;     yf1=wF->yf1;
  //xbr0=xbr1=(ncolumns-xf1+xf0)/2;   ybr0=ybr1=(nrows-yf1+yf0)/2;


	  // таблица значений сдвига (change) (выключки) блока текста;    //
	  // -100% <= ChangeX <= +100%;           -100% <= ChangeY <= +100%;
	  // ChangeX=+100% блок текста соприкасается с правой границей скана.
	  // ChangeX=-100% блок текста соприкасается с левой границей скана.
	  // ChangeY=+100% блок текста соприкасается с верхней границей скана.
	  // ChangeY=-100% блок текста соприкасается с нижней границей скана.
	  // ChangeX=ChangeY=0  блок текста автоматически перемещается в центр скана.
	  // Возможны все промежуточные комбинации ChangeX и ChangeY       //


 /// обработка скана в целом без разбиения на колонки и страници. AutoBorder.
 /// сдвиг (change) или бордюр, при сохранении исходного размера скана.
#ifdef WIN32_DRAW
  if (ImageProcessor->AutoRemoveBorderBtn->Checked==true){  // Change

	frameOCR *wF=&frameArray[0];
	 xf0=wF->xf0;     xf1=wF->xf1;     yf0=wF->yf0;     yf1=wF->yf1;

	// отработка сдвига (change) (выключки) блока текста.
	// ввод значения сдвига с экрана, по умолчанию 0.
#ifdef WIN32	  
	ChangeX=StrToInt(ImageProcessor->Edit6->Text);
	ChangeY=StrToInt(ImageProcessor->Edit7->Text);
#endif	  
	// Перевод значения Change в xbr0,xbr1,ybr0,ybr1.
	///if( ChangeX!=0 || ChangeY!=0 ){
	  if( ChangeX >100 ){ ChangeX=100; }    if( ChangeX <-100 ){ ChangeX=-100; }
	  if( ChangeY >100 ){ ChangeY=100; }    if( ChangeY <-100 ){ ChangeY=-100; }
	  xbr1=(100 - ChangeX)*(ncolumns-xf1+xf0)/200;
	  xbr0=(100 + ChangeX)*(ncolumns-xf1+xf0)/200;
	  ybr1=(100 - ChangeY)*(nrows-yf1+yf0)/200;
	  ybr0=(100 + ChangeY)*(nrows-yf1+yf0)/200;
	///}
	//ShowMessage(xbr0);  ShowMessage(xbr1);  ShowMessage(ChangeX  );  ShowMessage(ChangeY	);

	// отработка бордюра вокруг блока текста.
		#ifdef PC
		// ввод значения бордюра с экрана, по умолчанию 0.
		Border=StrToInt(ImageProcessor->Edit9->Text);
		#else
		Border=0;  // для MAC
		#endif
		// Перевод значения Border в xbr0,xbr1,ybr0,ybr1.
		if( Border > BorderMax ){ Border=BorderMax; }
		if( Border > 0 ){ xbr0=xbr1=ybr0=ybr1=Border;}
		//ShowMessage(Border);
  } // if (ImageProcessor
#endif
// DM(END); DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows "); DM(END);
// DM(END); DM(ChangeY<<" ChangeY ")DM(ybr1<<" ybr1 "); DM(ybr0<<" ybr0 "); DM(END);
// ChangeX ChangeY в виде кординат f,fg попадает в функцию RemoveBorder в LoadDjvu.cpp
/**/


 /// разделнние скана на колонки. SplitColumns
  // ввод значения технического бордюра с экрана (одинакого по всему перимнтру прямоугольника),
  // по умолчанию 16.
#ifdef WIN32_DRAW
  Border=StrToInt(ImageProcessor->Edit5->Text);
  #else
  Border=16;  // для MAC
  #endif
  // Перевод значения Border в xbr0,xbr1,ybr0,ybr1.
#ifdef WIN32_DRAW
  if(ImageProcessor->SplitColumnsBtn->Checked==true){ // индивидуальные бордюры по x,y
	if( Border > BorderMax ){ Border=BorderMax; }
	if( Border > 0 ){ xbr0=xbr1=ybr0=ybr1=Border;}
  }
#endif	
  //ShowMessage(Border);
  // Border в виде кординат f,fg попадает в функцию WriteColumnsData в LoadDjvu.cpp
/**/

 // значения xbr0,xbr1,ybr0,ybr1 не м.б. равным нолю.
 if( xbr0 < 1 ){ xbr0=1; }   if( xbr1 < 1 ){ xbr1=1; }
 if( ybr0 < 1 ){ ybr0=1; }   if( ybr1 < 1 ){ ybr1=1; }

 /// бордюр между многоугольниками f,fg вокруг вертикальной колонки или сдвиг (change).
 nFrame = (int)frameArray.size();
 for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
	 frameOCR *wF=&frameArray[nFrm];
	 xf0=wF->xf0;     xf1=wF->xf1;     yf0=wF->yf0;     yf1=wF->yf1;

   // БОЛЬШИЕ прямоугольники Y.
   // yfg0, yfg1  внешние габариты (верх и низ) прямоугольника вертикальной колонки,
   // вокруг многоугольника, не касающющегося краёв горизонтальных блоков букв
	 wF->Yfg1=yfg1=yf1 + ybr1;   if ( yfg1 >= nrows ){ yfg1=nrows-1; }  wF->yfg1=yfg1;  //
	 wF->Yfg0=yfg0=yf0 - ybr0;   if ( yfg0 <  1 ){ yfg0=1; }            wF->yfg0=yfg0;  //
   // БОЛЬШИЕ прямоугольники X.
   // xfg0,xfg1 внешние габаритты (левые и правые) прямоугольника вертикальной колонки,
   // вокруг многоугольника, не касающющегося (+Border) краёв горизонтальных блоков букв
	 wF->Xfg1=xfg1=xf1 + xbr1;   if ( xfg1 >= ncolumns ){ xfg1=ncolumns-1; } wF->xfg1=xfg1;
	 wF->Xfg0=xfg0=xf0 - xbr0;   if ( xfg0 <  1 ){ xfg0=1; }                 wF->xfg0=xfg0;
   // Xfg0,Xfg1,Yfg0,Yfg1 не попадает под ограничение размеров скана !

/* //// перенесено в функцию data_normalisation
   // переменные с БОЛЬШМИ X и Y используются для создания выходного файла
   // (jpg,tif) и не подлежат коррекции статическим бордюром (border),
   // примененным для увеличения размера малых сканов
	 // копирование координат БОЛЬШИХ прямоугольников
	 wF->Xf0=xf0;    wF->Xf1=xf1;    wF->Yf0=yf0;    wF->Yf1=yf1;
	 //wF->Xfg0=xfg0;  wF->Xfg1=xfg1;  wF->Yfg0=yfg0;  wF->Yfg1=yfg1;
	 // копирование координат X и Y малых горизонтальных блоков прямоугольника
	 // описывающего колонку.
	 nColumn=frameArray[nFrm].columnArray.size();
	 for ( nClm=0; nClm < nColumn; nClm++ ){ //  Цикл по количеству колонок nColumn
		columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
		wC->Xc0=wC->xc0;   wC->Xc1=wC->xc1;   wC->Yc0=wC->yc0;   wC->Yc1=wC->yc1;
	 } // Цикл по количеству колонок nColumn
*/   ////

// DM(END); DM(nFrm<<" nFrm "); DM(yfg1<<" yfg1 "); DM(yfg0<<" yfg0 "); DM(END);
// DM(END); DM(ncolumns<<" ncolumns "); DM(nrows<<" nrows "); DM(border<<" border "); DM(END);

 } // Цикл по количеству колонок nFrame
/**/


// получили величину бордюра между многоугольниками f,fg вокруг вертикальной колонки.

// xbr0,xbr1,ybr0,ybr1; зазор (бордюр) между многоугольниками вертикальной колонки.
// при сохранении исходного размера печа

// Xfg0,Xfg1,Yfg0,Yfg1, тоже, что и  xfg0,xfg1,yfg0,yfg1, только в дальнейшем не вычитается
// border (в функции data_normalisation) и не попадает под ограничение размеров скана.

#ifdef DEBUGLVL
if( GrafT[11][4] ) {

}
#endif
/**/

//xbr0=wF->xbr0;   xbr1=wF->xbr1;   ybr0=wF->ybr0;   ybr1=wF->ybr1;
//xfg0=wF->xfg0;   xfg1=wF->xfg1;   yfg0=wF->yfg0;   yfg1=wF->yfg1;
}//_____________________________________________________________________________


//                     ***  **  ***    Copy    ***  **  ***
//
// Подпрограмма для помещения колонок состоящих из блоков в структуру строк.
// -----------------------------------------------------------------------------
void GBitset::CopyBlock() {

  int xc0,yc0,xc1,yc1;
  int nFrm,nFrame, nClm,nColumn;
  //int Nstr,NStrok;


//DM(END)


// заполнение структуры L горизонтальными блоками описывающих многоугольники колонок
// колонки состоящие из блоков помещены в структуру строк (для глобальной структуры)

 NStrok = (int)strArray->size();
 for ( Nstr=0; Nstr < NStrok; Nstr++ ){ // Цикл по количеству строк NStrok

	 nFrame = (int)frameArray.size();
	 for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame

		nColumn=(int)frameArray[nFrm].columnArray.size();
		for ( nClm=0; nClm < nColumn; nClm++ ){ // Цикл по количеству блоков колонок Clm

			columnOCR *wC=&frameArray[nFrm].columnArray[nClm];  // nClm
			xc0=wC->xc0;   xc1=wC->xc1;   yc0=wC->yc0;   yc1=wC->yc1;
			// заполнение структуры L горизонтальными блоками описывающих многоугольники колонок
			if ( wC->columnFlag == Nstr ){
			  lineOCR wL;
              wL.xL0=wC->xc0;  wL.yL0=wC->yc0;
              wL.xL1=wC->xc1;  wL.yL1=wC->yc1;  
              wL.lineFlag=Nstr;  // установка флага горизонтальных блоков
              strArray[0][Nstr].lineArray.push_back(wL);
            }

        } // Цикл по количеству блоков колонок Clm

  //lineOCR *wL=&strArray[0][Nstr].lineArray[nFrm]; // Nstr nFrm

	 } // Цикл по количеству колонок nFrame

 } // Цикл по количеству строк NStrok
/**/

	 // получили координаты вертикальных колонок в структуре строк.

}//_____________________________________________________________________________




/*
//                      РАБОЧАЯ СДВИНУТА ДЛЯ УДОБСТВА
//                              Y0, Y1

//  ----------------------------------------------------------------------------
// Подпрограмма для получения координат колонок по вертикали.
// работает но не применили
//  ----------------------------------------------------------------------------
void GBitset::FrameDetectorH() {

  int y,x,d;
  int KHg,Prg,maxbft,Nblck,scope=8;
//int s0,s1,s3,s4;
// vector<int> BufR(ncnr);
// vector<int> BufT(ncnr);

//void LSum(vector<int> &L);  // заполняет массив L вертикальными суммами
//void HSum(vector<int> &H);  // заполняет массив H горизонтальными суммами


      // ПЕРЕИНОМЕНОВАТЬ   KHig >>  KHg

                        BufR=HStS;

               int PGilb=KFStrok; // PGilb - параметр (ширина) преобразования ГИЛБЕРТА
               int p=KFStrok;     // KHig - параметр фильтрации

               // MGilb=nrows;   // размер массива преобразования ГИЛБЕРТА
if(p==0) KHig=8;  if(p==1) KHig=12;  if(p==2) KHig=16;  if(p==3) KHig=20; if(p==4) KHig=24;

		  // уточненое значение козффициента для сглаживающего фильтра SP->filterHigh
          //KHig=MeanSA*0.9;   // MeanSA*0.9;  // KHig, KLow, MeanSA, MeanCol- глобальные.
               if ( KHig>=31 ) KHig=31;
               //KLow=KHig*3;
               KHig=KHig*1.7;
               KLow=KHig*3;

               // сглаживание, полосовой фильтр
               filterLaplasian(BufR,KHig,KLow);

               // зеленый, вызов функции рисования переменной длинны
//if( GrafT[11][4] ) { LDrawVar(BufR,nrows,ncolumns/16,scope*1,0x0000AA00); }

               // преобразование ГИЛБЕРТА
               filterGilbert(BufR,BufT, nrows,5);  // KHig/16   PGilb

          // красный, вызов функции рисования переменной длинны
if( GrafT[11][4] ) { LDrawVar(BufT,nrows,ncolumns/16,scope*1,0x000000FF); }


      int bfr, bft, bfa;
      for ( y=8; y < nrows-8; y++ ){  // 8 пикселей от края печа не используются.
              bfr=BufR[y]=BufR[y]*0.85;   // *0.8  2/ПИ
              bft=BufT[y];
              bfa=sqrt(bfr*bfr + bft*bft);
              GBuf[y]=bfa;   //
      }// for y


          // зеленый, вызов функции рисования переменной длинны
if( GrafT[11][4] ) { LDrawVar(BufR,nrows,ncolumns/16,scope*1,0x0000AA00); }

          // сглаживание блоков слов - ширина колонок ( KHig  уже адаптивный )
		  SP->filterHigh(GBuf,KHig);

          // красный, вызов функции рисования переменной длинны
if( GrafT[11][4] ) { LDrawVar(GBuf,nrows,ncolumns/16,scope*1,0x000000FF); }


                            HBuf=GBuf;

          // сильное сглаживание для получение максимума HBuf
		  SP->filterHigh(HBuf,MeanSA*4);  //  SP->filterHigh(HBuf,MeanSA*4);

          // красный, вызов функции рисования переменной длинны
if( GrafT[11][4] ) { LDrawVar(HBuf,nrows,ncolumns/16,scope*6,0x000000FF); }

          // получение максимума HBuf
          maxbft=0;
          for ( x=8; x < ncolumns-8; x+=4 ){  // 8 пикселей от края печа не используются.
                   if( HBuf[x] > maxbft ) { maxbft=HBuf[x]; }
          }// for x

                    HBuf.assign(ncnr,0);

          // получение порога по максимуму HBuf
          Prg=maxbft*0.2; d=0; //  1/4
          // получение координат блоков слов (колонок) на развороте
          // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
          for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
                   if( GBuf[x] >= Prg ) { GBuf[x]=Prg; }
                   else { GBuf[x]=0; }
                   if( GBuf[x]==Prg && GBuf[x-1]==0 ) { HBuf[d]=x; }
                   if( GBuf[x]==0 && GBuf[x-1]==Prg ) { HBuf[d+1]=x; d+=2;}
          }// for x
          Nblck=d;     // DM(END); DM(Nblck<<" Nblck ")


*/
/*

//  MeanCol - средее по всем строкам страници расстояние между центрами масс букв
           // Заполнение структур координатами блоков слов по размеру колонок
           int DMn=MeanCol*0.4+2;
  d=1;
  for ( y=0; y < Nblck; y+=2 ){
                 frameOCR wt;
                 wt.xf0=HBuf[y];   // - DMn/2
                 wt.xf1=HBuf[y+1]; // + DMn/2
//               wt.yf0=strArray[0][Nstr].y0 - DMn; // - для красоты
//               wt.yf1=strArray[0][Nstr].y1 + DMn; // + для красоты
                 wt.yf0=100;
                 wt.yf1=nrows-100;
                 wt.frameFlag=d;  d++; // установка флага блоков слов (номер колоноки)
                 frameArray.push_back(wt);
//DM( " xf0="<<wt.xf0<< " yf0="<<wt.yf0<< " xf1="<<wt.xf1<< " yf1="<<wt.yf1<< "\n---\n");
  }// for y
*/


/*
#ifdef DEBUGLVL
if( GrafT[11][4] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
LDrawVar(GBuf,nrows,ncolumns/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны

                      // ВИЗУАЛИЗАЦИЯ цифровая
//                 DM(END); DM(MeanCol<<" MeanCol "); DM(END);

                      /// ТАБЛИЦА
//            DM(END);
//            for ( y=1; y < Nblck; y++  ){
//            DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
//              }//for y
//            DM(END);

   // ВИЗУАЛИЗАЦИЯ аналоговая исходных горизонтальных сумм (черная), для HStr[y]>=0
   int M=16; static short delta=50;
   for(y=0; y < ncnr; y++) { // ncnr // drawData->rows()
   for(x=delta; x<(HStS[y]/M)+delta; x++)drawData[0][y][x]*=0.1;} // 0.8*ncolumns;
DM(" szf "<<szf);

}

// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
////if( GrafT[11][4] ) { columnDraw(0x00ff00); }  // цвет линии зеленый

#endif

}//_____________________________________________________________________________
*/


/**/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



/*
//                                             ***  **  ***
//                                             **   **  **
//                                             ***  **  ***
//                                             ***  *   ***
//                                             * *  **  ***
//                                             ***  **  ***
//
// Подпрограмма для получения координат блоков строк в  пределах колоноки.
// -----------------------------------------------------------------------------
void GBitset::ColumnDetector() {

  int y,x,d;
  int Prg,maxbft,Nblck,scope=8;
  int s0,s1,s3,s4;

         // вычисленнное колличество строк в печа - NStrok=strArray.size();
                       NStrok=strArray->size();
                       if( NStrok<4) { return; }

                             GBuf=LStS;

		  // уточненое значение козффициента для сглаживающего фильтра SP->filterHigh
          KHig=MeanSA*1.5;      // *1.8;       // KHig, KLow, MeanSA- глобальные.

          // сглаживание блоков слов - ширина колонок ( KHig  уже адаптивный )
		  SP->filterHigh(GBuf,KHig);

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
          // получение координат блоков слов (колонок) на развороте
          // для увеличения быстродействия можно  //x+=2 //GBuf[x-1]=  //GBuf[x-2]
          for ( x=8; x < ncolumns-8; x++ ){  // 8 пикселей от края печа не используются.
                   if( GBuf[x] >= Prg ) { GBuf[x]=Prg; }
                   else { GBuf[x]=0; }
                   if( GBuf[x]==Prg && GBuf[x-1]==0 ) { HBuf[d]=x; }
                   if( GBuf[x]==0 && GBuf[x-1]==Prg ) { HBuf[d+1]=x; d+=2;}
          }// for x
          Nblck=d;     // DM(END); DM(Nblck<<" Nblck ")

//  проверить выход за середину строк  //
//  MeanSA - средее по всей строке расстояние между центрами масс букв
           // Заполнение структур координатами блоков слов (колонок)
           int DMn=MeanSA*0.4+2;
           for ( y=0; y < Nblck; y+=2 ){
                 columnOCR wt;
                 wt.xc0=HBuf[y];   // - DMn/2
                 wt.xc1=HBuf[y+1]; // + DMn/2
                 wt.yc0=strArray[0][Nstr].y0 - DMn; // - для красоты
                 wt.yc1=strArray[0][Nstr].y1 + DMn; // + для красоты
                 wt.columnFlag=1;      // устаановка флага блоков слов (колонок)
                 strArray[0][Nstr].columnArray.push_back(wt);
//DM( " xc0="<<wt.xc0<< " yc0="<<wt.yc0<< " xc1="<<wt.xc1<< " yc1="<<wt.yc1<< "\n---\n");
           }// for y


#ifdef DEBUGLVL
if( GrafT[2][7] ) {
                      // ВИЗУАЛИЗАЦИЯ аналоговая
WDrawVar(GBuf,ncolumns,nrows/16,scope*6,0x000000FF);// красный, вызов функции рисования переменной длинны

                      // ВИЗУАЛИЗАЦИЯ цифровая
                      /// ТАБЛИЦА
//            DM(END);
//            for ( y=1; y < Nblck; y++  ){
//            DM(HBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
//              }//for y
//            DM(END);

}
#endif



}//_____________________________________________________________________________

*/

////////////////////////////////////////////////////////////////////////////////



/*
// http://ru.wikipedia.org/wiki/

// Применительно к массивам данных, поиск осуществляется по ключу, присвоенному
// каждому из элементов массива (в простейшем случае сам элемент является ключом).

// Двоичный (бинарный) поиск (также известен, как метод деления пополам и метод
// половинного деления) — алгоритм нахождения заданного значения монотонной
// (невозрастающей или неубывающей) функции. Поиск основывается на теореме о
// промежуточных значениях.


// В качестве примера можно рассмотреть поиск значения монотонной функции,
// записанной в массиве, заключающийся в сравнении срединного элемента массива
// с искомым значением, и повторением алгоритма для той или другой половины,
// в зависимости от результата сравнения.

// Пускай переменные Lb и Ub содержат, соответственно, левую и правую границы
// отрезка массива, где находится нужный нам элемент. Исследования начинаются
// со среднего элемента отрезка. Если искомое значение меньше среднего элемента,
// осуществляется переход к поиску в верхней половине отрезка, где все элементы
// меньше только что проверенного, то есть значением Ub становится M - 1 и на 
// следующей итерации исследуется только половина массива. Т.о., в результате 
// кажждой роверки область поиска сужается вдвое.

// Например, если длина массива равна 1023, после первого сравнения область
// сужается до 511 элементов, а после второй — до 255.Т.о. для поиска в массиве
// из 1023 элементов достаточно 10 сравнений.

////////////////////////////////////////////////////////////////////////////////
int function BinarySearch (Array A, int Lb, int Ub, int Key);
  begin
  do forever
    M = Lb + (Ub-Lb)/2;
    if (Key < A[M]) then
      Ub = M - 1;
    else if (Key > A[M]) then
      Lb = M + 1;
    else
      return M;
    if (Lb > Ub) then
    return -1;
  end;
////////////////////////////////////////////////////////////////////////////////
                    

////////////////////////////////////////////////////////////////////////////////
int Lb=0,Ub=ncolumns; // левая и правая граница отрезка массива
int Key=100, K; // Key - искомое значение, K - адрес искомого значения  в массиве
vector<int> A(ncolumns);
//map<int,int> AMap;  // AMap[x]=x;

                     //  TEST
         for ( x=0; x < ncolumns; x++ ) { A[x]=x; }  //  for x

DM(END);
//if (Lb > Ub);
for ( x=0; Key != A[K] && x < 16; x++ ) { //  else if (Key == A[K]) break;
     K = Lb + (Ub-Lb)/2;
     if (Key < A[K]) Ub = K - 1;
     else
     if (Key > A[K]) Lb = K + 1;
DM(K<<" K ");
     }  //  for x

        DM(END); DM(Key<<" Key "); DM(K<<" K "); DM(END);

////////////////////////////////////////////////////////////////////////////////
    //    if(AMap.find(100) != AMap.end()){
    //    DM("AMap[100]="<<AMap[100]<<END);  }

*/

/*
             // подсчет вертикальных сумм
             //int d;
             for ( x=0; x < ncolumns; x++ ) {  // read image make sum
                   res=0;
                   for ( y=0; y < nrows; y++ ) {
                        d=y*ncolumns;  res+=bits_data[d + x];
                       }  // for y
                 LStr[x]=res<<5; // ВАЖНО - сумма вдоль строк домножена на *256 res<<8;
           //        LStr[x]=res;
                 }  //  for x
*/
