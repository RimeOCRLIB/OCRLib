#include "GBitsetMatrix.h"

// для использования
//                           Поиск триадами
// Подпрограмма получения максимума коэффициента корреляции по маске, в пределах
// прямоугольника поиска. Поиск триадами.

// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrix_Lion(GBitMask32 *mask32,
										int xSrc0,
										int ySrc0,
										int xSrc1,
										int ySrc1,
										int dlt,
										int *maxX,
										int *maxY,
										int print) {
/**/

	int sx0,sy0,sx1,sy1;
	int x,y,m,n,t;                 //  ,p,d
	int LfAmSum,RgAmSum;
	int Lf,Rg,LfMem,RgMem, Delta;  //float Lf,Rg,LfMem,RgMem, Delta;
	int LfxP, RgxP, LfyP, RgyP;
	int MaxCorX=0,MaxCorY=0,AdrMaxCorX=0,AdrMaxCorY=0;
	int ch=0,correlation=0;     //    ,wVh M,
	//отключение в config.h всех DP  -  #define DR(x) //c_out<<x;
    //print=1;
	int printP=0;


#ifdef REPORT
	//cout_<<"Matrix_Lion_1@@ dlt="<<dlt<<" xSrc0="<<xSrc0<<" xSrc1="<<xSrc1<<" ySrc0="<<ySrc0<<" ySrc1="<<ySrc1<<" mask32.mHOn="<<mask32->mHOn;
#endif

#ifdef REPORT
	int color;
	if(dlt==0)color=0xff4040ff;  ////red
#ifdef DEBUGLVL_DRAW	
    if(print)drawDataRGB->put(xSrc0-32,ySrc0,0xff4040ff);    ////red
#endif
#endif	


	//static clock_t  tm_start1=clock();
	//clock_t  tm_end; double time;  DR(END); //DR(tm<<" "<<t);


/*
// константы для алгоритма подсчета единиц в 128 или в 64р словах
static const unsigned int constF[12]={              // 12  16
	0x33333333, 0x33333333, 0x33333333, 0x33333333,
	0x55555555, 0x55555555, 0x55555555, 0x55555555,
	0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f  // ,
//	0x00000000, 0x00000000, 0x00000000, 0x00000000  // TEST
};

// переход в режим Assembler, загрузка констант
__asm {     //  movups (без требования выравнивания)
	// загрузка констант в регистр 64p ММХ ( считаем в байтах - int*4)
	movq            mm5, qword ptr constF+16;   // 55 Пересылка данных (64 бит)
	movq            mm6, qword ptr constF;      // 33 из/в регистр ММХ  MOVDQU-128
	movq            mm7, qword ptr constF+32;   // 0f

	// загрузка констант в регистр 128p SSE
	//  movups без требования выравнивания  ( MOVAPS должны быть выровнены )
#ifdef WIN32
	movups          xmm5, dqword ptr constF+16;  // 55  Пересылка данных (128 бит)
	movups          xmm6, dqword ptr constF;     // 33  + MOVDQU
	movups          xmm7, dqword ptr constF+32;  // 0f
//	movups          xmm2, dqword ptr constF+48;  // TEST
#else
	movups          xmm5, [constF+16];  // 55  Пересылка данных (128 бит)
	movups          xmm6, [constF];     // 33  + MOVDQU
	movups          xmm7, [constF+32];  // 0f
	//	movups          xmm2, dqword ptr constF+48;  // TEST
#endif

} // __asm
//Инструкция movq выполняется за шесть тактов, тогда как для pshufw требуется только два
*/

#ifdef MMX   
__asm {
		emms;
	} // emms; команда обеспечивает переход процессора от исполнения MMX-команд
	// к исполнению обычных команд с плавающей запятой:
	// она устанавливает значение 1 во всех разрядах слова состояния.
/**/
#endif


    
	////for ( int i=0; i<100000; i++ ){    //проверка быстродействия   3.5 sec
	
   	// заполнение массива кэша функции setMatrix_Point
	// в адесах где лежит 255 коэффициент корреляции (0-100) не занесен
	//unsigned char cacheCor[128*34];  // массив кэша коэффициента корреляции по области поиска максимума
	
    memset(cacheCor,255,4352);  //128*32

	
	// Задание области скольжение маски по смещению dlt или по координатам x,y
	if(dlt>0){
		sx0=xSrc0-dlt,    sx1=xSrc0+dlt;
		sy0=ySrc0-dlt,    sy1=ySrc0+dlt;
	}else{  //  dlt=-dlt
		sx0=xSrc0,        sx1=xSrc1;
		sy0=ySrc0,        sy1=ySrc1;
	    //			      mH=sy1 - sy0;
	} // if(dlt>0)
	
	AdrMaxCorX=sx0;
	AdrMaxCorY=sy0;
	
	int X,Y,X0,X1,Y0,Y1,Cor,maxCor,AdrCorX,AdrCorY;
	
	// выбор шага сетки первичного разбиения области поиска максимума коэффициента корреляции
	// высота h и ширина w области поиска максимума коэффициента корреляции
	int w=sx1-sx0;       int h=sy1-sy0;
	int maxwh=w;         if (h > w) { maxwh=h; }
	// stepwh=3 при maxwh<16 (сетка 4x4),  stepwh=4 при maxwh>=16 (сетка 5x5),  stepwh=5 maxwh>=32....          // (sx1-sx0)  (sy1-sy0)
	int stepwh=3 + maxwh/16;  // 4 + 16   // 4 + maxwh/16;
	int dx=64*w/stepwh;       int dy=64*h/stepwh;     // res<<8  *256
	//if (dx < 2) {dx=2;}      if (dy < 2) {dy=2;}
	if (dx < 128) {dx=128;}   if (dy < 128) {dy=128;}
	// int dx=20;             int dy=20;
	// количество узлов сетки stepwh по высоте и ширине одинакого, но величины шагов dx,dy, как правило разные
	DR("//maxwh="<<maxwh<<"  //stepwh="<<stepwh<<END); DR(END);
	//cout<<" "<<stepwh;
	
	// интервал поиска по сетке максимума коэффициента корреляции.
	int SX0=sx0*64;             int SX1=sx1*64;
	int SY0=sy0*64;             int SY1=sy1*64;
	
	// сдвиг периметра сетки к центру области поиска нам пол шага.
	int ddx=dx>>1;            int ddy=dy>>1; // /2
	X0=SX0 + ddx;             Y0=SY0 + ddy;
	X1=SX1 - ddx;             Y1=SY1 - ddy;
	
	DR(END);
	// формирование сетки (обычно 5x5), поиск максимума коэффициента корреляции в сетке
	maxCor=Cor=0;     t=0;
	for ( Y=Y0; Y <= Y1;  Y+=dy ) {   // || maxCor < 100
		//for ( Y=Y0+dy/2; Y <= Y1-dy/2;  Y+=dy ) {   // || maxCor < 100
		y=Y/64;   DR(END);            // p=y*w;  ///////////////////////////
		for ( X=X0; X <= X1; X+=dx ) {
		//for ( X=X0+dx/2; X <= X1-dx/2; X+=dx ) {
			x=X/64;                                        // res>>8  /256
			#ifdef REPORT
			  if(print){
				 /*
                 if(count_==134){int L=1; printP=1;}
                   else{printP=0;}
				 //cout<<"count="<<count_<<" Cor="<<Cor<<endl;
				 if(count_==135){int l=1;}				 				 
				 count_++;
				 */
			  }
			#endif
            
            ///cout_<<"Matrix_Lion_1@@q dlt="<<dlt<<" xSrc0="<<xSrc0<<" xSrc1="<<xSrc1<<" ySrc0="<<ySrc0<<" ySrc1="<<ySrc1<<" mask32.mHOn="<<mask32->mHOn<<endl;
            //cout_<<"Matrix_Lion_1@@q1 x="<<x<<" y="<<y<<" sx0="<<sx0<<" sy0="<<sy0<<" w="<<w<<endl;

			Cor=setMatrix_Point(mask32, x,y, sx0,sy0,w,&ch,printP); t+=1-ch;   //Cor=(Cor*2)/2;
  ///          Cor=(Cor+setMatrix_Point(mask32, x,y, sx0+1,sy0,w,&ch,printP))/2;   //Cor=(Cor+Cor1)/2;      //////////////////////////////
            //int Cor2=setMatrix_Point(mask32, x,y, sx0+2,sy0,w,&ch,printP);   Cor=(Cor+Cor1+Cor2)/3; //////////////////////////////
            
      ////      if ( Cor==-1 )  { correlation=0; AdrMaxCorX=x;  AdrMaxCorY=y; goto maxCorEND; }  /////////////////////////////////////////////////////////////////////////////////////////
            
			if ( Cor > maxCor ) { maxCor=Cor;     AdrCorX=x; AdrCorY=y; }
			if ( maxCor==100 ){correlation=maxCor; AdrMaxCorX=x;  AdrMaxCorY=y; goto maxCorEND;}
#ifdef DEBUGLVL_DRAW
            if(mask32->id==0){drawDataRGB->put(x-32,y,0xffcfcfc0);}
				else{drawDataRGB->put(x-32,y,0xf0ccffcc);}
#endif
#ifdef DEBUGLVL_GREY
				//if(mask32->id==0){drawData[0][y][x-32]/=2;}
				//else{drawData[0][y][x-32]*=0.9;}
#endif			
			DR(Cor<<" ,"<<x<<","<<y<<","<<"\t");
			///	DR(Cor<<","<<AdrCorX<<","<<AdrCorY<<"  "<<"\t");
		} // x
	} // y
	if ( maxCor<25 )  { correlation=1; goto maxCorEND; }   // correlation=maxCor;
	
	DR(END);
	DR("  maxCor="<<maxCor<<"   AdrCorX="<<AdrCorX<<"   AdrCorY="<<AdrCorY<<"   t="<<t<<END);
	
	
	
	// интервалы поиска X0-X1, Y0-Y1 для нахождения max коэф. корр. ( 2*dx на dy*2 ).
	X0=AdrCorX*64-dx;   if (X0<SX0) {X0=SX0;}    // if (X0<sx0) {X0=sx0;}
	X1=AdrCorX*64+dx;   if (X1>SX1) {X1=SX1;}    // if (X1>sx1) {X1=sx1;}
	Y0=AdrCorY*64-dy;   if (Y0<SY0) {Y0=SY0;}    // if (Y0<sy0) {Y0=sy0;}
	Y1=AdrCorY*64+dy;   if (Y1>SY1) {Y1=SY1;}    // if (Y1>sy1) {Y1=sy1;}
	/**/
	
	
	// интерактивное нахождение максимума функции коэффициента корреляции Lion
	// (поиск триадами float). Функция состоит из: блока перемножения масок
	// Off, ON и алгоритма подсчета единиц. Алгоритм находит максимум коэффициента
	// корреляции в прямоугольнмке с координатами sx0,sy0,sx1,sy1; 										int xSrc0,
	
	////////////////////////////////////////////////////////////////////////////////
	DR(END); DR(sx0<<" sx0 "); DR(sx1<<" sx1 "); DR(sy0<<" sy0 "); DR(sy1<<" sy1 "); DR(END);
	
	
	
	// алгоритм находит максимум коэффициента корреляции в пределах от sx0 ,sy0 до sx1,sy1
	
	// абсолютная точность итераций absolute Precision
	Delta=1;  // DR(END);DR(Delta<<" Delta ");DR(END);  // Delta=2-m;  if ( Delta< 1 ) Delta=1;
	// координаты старта поиска
	AdrMaxCorX=AdrCorX;  AdrMaxCorY=AdrCorY;   // t=0;
	
	// цикл чередования поиска по осям X и Y , максимума коэффициента корреляции
	for ( m=0; m<3; m++ ){
		
		// нахождение максимума коэффициента корреляции по оси X
		Lf=0;   Rg=X1-X0;   // интервал поиска X0-X1   // M*(X1-X0)/100;
		DR(END); DR(Lf<<" Lf "); DR(Rg<<" Rg "); DR(END);
		DR(END); DR("AdrMaxCorY="<<AdrMaxCorY<<END);
		for ( n=0; n<7; n++ ){  // 10
			
			LfMem=(Lf+Lf+Rg)/3;  LfxP=(LfMem+X0)/64;  // треть влево (Lf+Lf+Rg)/3; sx0
			// возврат коэффициента корреляции в точке LfxP, AdrMaxCorY.
			LfAmSum=setMatrix_Point(mask32, LfxP,AdrMaxCorY, sx0,sy0,w,&ch, printP); t+=1-ch;
			RgMem=(Lf+Rg+Rg)/3;  RgxP=(RgMem+X0)/64;  // треть вправо  (Lf+Rg+Rg)/3;
			// возврат коэффициента корреляции в точке RgxP, AdrMaxCorY.
			RgAmSum=setMatrix_Point(mask32, RgxP,AdrMaxCorY, sx0,sy0,w,&ch,printP); t+=1-ch;
			
			DR("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="<<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
			DR("  LfxP="<<LfxP<<"   RgxP="<<RgxP<<"   RgxP-LfxP="<<RgxP-LfxP<<"   t="<<t<<END);
			
			if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
			if ( abs(RgxP-LfxP)<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)
			///				 if ( abs(RgxP-LfxP)<Delta ) break;
			
		} // for n
		
		//  максимум коэффициента корреляции (correlation), его адрес
		MaxCorX=(LfAmSum + RgAmSum)/2;   AdrMaxCorX=(LfxP + RgxP)/2;
		DR("//MaxCorX="<<MaxCorX<<"  //AdrMaxCorX="<<AdrMaxCorX<<END); DR(END);
		
		///////	 if ( MaxCorY==MaxCorX || MaxCorY == 100 || MaxCorX==100 ) break;  // ОПОРНАЯ
		if ( MaxCorY==MaxCorX ) break;
		
		// нахождение максимума коэффициента корреляции по оси Y
		Lf=0;   Rg=Y1-Y0;   // интервал поиска Y0-Y1
		DR(END); DR("AdrMaxCorX="<<AdrMaxCorX<<END);
		for ( n=0; n<7; n++ ){ // 10
			
			LfMem=(Lf+Lf+Rg)/3;  LfyP=(LfMem+Y0)/64;  // треть влево (Lf+Lf+Rg)/3;
			// возврат коэффициента корреляции в точке AdrMaxCorX, LfyP.
			LfAmSum=setMatrix_Point(mask32, AdrMaxCorX,LfyP, sx0,sy0,w,&ch,printP); t+=1-ch;
			RgMem=(Lf+Rg+Rg)/3;  RgyP=(RgMem+Y0)/64;  // треть вправо  (Lf+Rg+Rg)/3;
			// возврат коэффициента корреляции в точке AdrMaxCorX, RgyP.
			RgAmSum=setMatrix_Point(mask32, AdrMaxCorX,RgyP, sx0,sy0,w,&ch,printP); t+=1-ch;
			
			DR("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="<<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
			DR("  LfyP="<<LfyP<<"   RgyP="<<RgyP<<"   RgyP-LfyP="<<RgyP-LfyP<<"   t="<<t<<END);
			// DR("//MaxCorX="<<MaxCorX<<"  //AdrMaxCorX="<<AdrMaxCorX<<END); DR(END);
			
			if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
			if ( abs(RgyP-LfyP)<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)
			///				 if ( abs(RgyP-LfyP)<Delta ) break;
			
		} // for n
		
		//  максимум коэффициента корреляции (correlation), его адрес
		MaxCorY=(LfAmSum + RgAmSum)/2;   AdrMaxCorY=(LfyP + RgyP)/2;
		DR("//MaxCorY="<<MaxCorY<<"  //AdrMaxCorY="<<AdrMaxCorY<<END); DR(END);
		
		//			 if ( RgxP-LfxP <= 1 && RgyP-LfyP<= 1 ) break;
		///////	     if ( MaxCorY==MaxCorX || MaxCorY == 100 || MaxCorX==100 ) break;  // ОПОРНАЯ
		if ( MaxCorY==MaxCorX ) break;
		
	} // for m
	
	/**/
	//int correlation=(MaxCorX+MaxCorY)/2;   ////// maxX=AdrAmMAX-border-32;   maxY=yP-border;
	correlation=MaxCorY;    if ( MaxCorX > MaxCorY ) { correlation=MaxCorX; }
	
	////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUGLVL_DRAW
	drawDataRGB->put(AdrMaxCorX-32,AdrMaxCorY,0xff00aa00);    ////red
    //cout_<<" AdrMaxCorX="<<AdrMaxCorX-32<<" AdrMaxCorY="<<AdrMaxCorY<<" MaxCor="<<MaxCorY<<endl;
#endif
	
   //  DT(" maxCorEND="<<maxCor);  DT(" DLT="<<correlation-maxCor);  DT(" correlation="<<correlation<<END);
   //  DT(" stepwh="<<stepwh);
	
maxCorEND:; // переход

	
	// ВИЗУАЛИЗАЦИЯ цифровая
	DR(" //__correlation="<<correlation<<END);
	DR(END<<"// w*h/(t+1)="<<w*h/(t+1)<<",  t="<<t<<END);
	////
	DR(" maxX="<<AdrMaxCorX<<END);
	
	DR(n+1<<"-n- "); DR(Lf<<" Lf ");
	DR(Rg<<" Rg "); DR(Delta<<" Delta "); DR(END);
	
	
	//____________
	/**/
	
	////} // i  //проверка быстродействия
/*
	__asm {
		emms;
	} // emms; команда обеспечивает переход процессора от исполнения MMX-команд
	// к исполнению обычных команд с плавающей запятой:
	// она устанавливает значение 1 во всех разрядах слова состояния.

	//DM("2@@")tm_end=clock(); tm_end-=tm_start; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);
	//DR("Matrix_Lion_2@@")tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;DR("time="<<time<<END<<END);
*/

	*maxX=AdrMaxCorX;
	*maxY=AdrMaxCorY;
	mask32->correlation=correlation;
	
	return correlation;
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	
	//			 maxX=maxX - border - 32;
	//			 maxY=maxY - border;
	/**/
	
	//------------------------------------------------------------------------------
	
/*
	 //		 int qx=100*w/9;      int qy=100*h/9;
//DR("//qx="<<qx<<"  //qy="<<qy<<END); DR(END);
DR(END);
		 maxCor=0;
		 for ( Y=Y0; Y <= Y1; Y+=qy ) {
		   y=Y/100;  DR(END);      //p=y*dx;
		   for ( X=X0; X <= X1; X+=qx ) {
			 x=X/100;
			 Cor=setMatrix_Point(mask32, x,y, c_out);   t++;
			 if ( Cor > maxCor ) { maxCor=Cor;     AdrCorX=x; AdrCorY=y; }
			 if ( maxCor==100 )  { MaxCorX=maxCor; goto maxCorEND; }
//			 drawDataRGB[0][y][x-32]=0;
			  DR(Cor<<","<<x<<","<<y<<"  "<<"\t");
///			 DR(Cor<<","<<AdrCorX<<","<<AdrCorY<<"  "<<"\t");
		   } // x
		 } // y
		 if ( maxCor<30 )  { goto maxCorEND; }
//		 maxCorEND:;
DR(END);
DR(END);DR("  maxCor="<<maxCor<<"   AdrCorX="<<AdrCorX<<"   AdrCorY="<<AdrCorY<<"   t="<<t<<END);
*/

//------------------------------------------------------------------------------
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
	  // работоспособное решение задачи нахождение максимума функции
	  Lf=0;   Rg=X0-X1;   // интервал поиска X0-X1
	 for ( n=0; n<10; n++ ){           d+=M;

		   dt=LfMem=(Lf+Lf+Rg)/3;               // треть влево (Lf+Lf+Rg)/3;
		   LfAmSum=FFFF( dt/4, 2 );             // получение отклика функции
												// в которой ищем максимум
		   dt=RgMem=(Lf+Rg+Rg)/3;               // треть вправо  (Lf+Rg+Rg)/3;
		   RgAmSum=FFFF( dt/4, 2 );             // получение отклика функции

		   DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="
		   <<LfAmSum<<"   RgAmSum="<<RgAmSum<<END); DM(Rg-Lf<<" Rg-Lf "); DM(END);

		  if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
		  if ( fabs(Rg-Lf)<Delta || RgAmSum-LfAmSum==0 ) break;

	 } // n
*/
//------------------------------------------------------------------------------


}//_____________________________________________________________________________

