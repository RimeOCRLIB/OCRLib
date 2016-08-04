//  OCRLib  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .


//13 11 2010
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//*****************************angle recognition********************************

// Функция определения угла наклона текста с помощью дискретного быстрого
// преобразования Радона (используется для обнаружения прямых линий на изображениях)
// аналог GBitsetRotation.h

// ИСТОЧНИКИ
// Программная реализация алгоритма Deskew
// http://www.djvu-soft.narod.ru/bookscanlib/008.htm
// и его прототип  http://pagetools.sourceforge.net/

#include "GBitmap.h"
namespace ocr{

float GBitmap::detectPageAngle( unsigned int g ) {
	// g параметр задающий максимально возможный определяемый угол наклона текста,
    // чем меньше g тем болше максимально возможный определяемый угол
	// по умолчанию g=8, с увеличением максимального угола наклона текста,
	// пропорционально падает быстродействие t.
	// g=8 angle<7gr, t=0.125sec; g=4 angle<14gr t=0.29sec;  g=2 angle<26gr t=0.8sec.
	// g=24 angle<14gr, t=0.125sec, но точность в два раза ниже чем g=4
	// геометрический смысл параметра g это сумма пикселей по горизонтальной
	// прямоугольной области размером 1 на 8 пикселей или 1 на 4 или 1 на 2 или 1 на 1
	// или 2 на 4 (g=24)
    
    int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    
    if (GrafT) { cout<<"Функция определения угла наклона текста   version V0    detectPageAngle(g)   g="<<g<<endl; }

	
	unsigned int h=nrows;      // высота массива   IMAGE_HEIGHT
	unsigned int w=ncolumns;   // ширина массива   IMAGE_WIDTH
	
	////unsigned int g0=g;   if (g==24) {h=h/2; g0=4;}   //***
	
	unsigned int w_g=w/g;      // w_8=w/8;    >>3 деление на 8  // w_8=(w+7)/8;  //***
	unsigned int i;
	
	TIME_START  //  time=0.516  w=2698 h= 4000
	
	// вычисление w2 как ближайшей степени двойки w, причем w2 >= w,
	// напрямер если w=1035 то w2=2048
	unsigned int w2=1;
	while( w2<w_g ){ w2<<=1; }  // <<1 умножение на 2
	
	
	
	// Size of sharpness table
	// Размер массива для суммы квадратов конечных разностей
	unsigned int ssize=2*w2-1;
	// создание массива для суммы квадратов конечных разностей
	unsigned int *sharpness=new unsigned int[ssize];
	
	// Вызовы функции быстрого преобразования Радона для определения угла alpha
	toolsRadon( 1, sharpness, w2, g);
	toolsRadon(-1, sharpness, w2, g);
	
	// освобождение массива bytes_data_buf
	//delete bytes_data_buf;
	
	// вычисляем максимум суммы квадратов разностей vmax и индекс imax,
	// соответствующий этому максимому. Индекс определяет угл наклона текста
	///cout_<<END;
	unsigned int s, imax=0, vmax=0;
	double sum=0.;  // unsigned int sum=0;
	for(i=0; i<ssize; i++) {
		s=sharpness[i];
		if( s>vmax ) { imax=i;  vmax=s; }
		
		
		sum+=s;
		/// cout_<<" "<<imax;  // cout_<<" "<<vmax/1000;
	} //i
	///cout_<<END<<END;
	
	// освобождение массива sharpness
	delete sharpness;
	
	// определение нулевого угла наклона текста  // Heuristics !!!
	//if( vmax<=((int)(3*sum/h)) ) { cout_<<END<<"angle=0; no detect"<<END; return 0; }
	if( vmax<=3*sum/h ) { if(GrafT)cout<<"angle=0; no detect"<<endl; return 0; }
	
	// определение произвольного угла наклона текста alpha, в диапазоне +-7 градусов
	// int iskew = (int)imax - int(w2) + 1;
	int iskew = imax - w2 + 1;
	
	
	// Convert radians to degrees and change the sign:
	// Вычисляем арктангенс, преобразуем радианы в градусы и изменяем знак:
	float F=iskew;
	/// float alpha = -57.29747*atan(F/(8*w2));
	float alpha = -57.29747*atan(F/(g*w2));                          // double  //***
	/**/
	
	if (GrafT) { cout<<"возвращает угол наклона текста    float alpha="<<alpha<<endl; }

	 /*
	 cout<<endl<<"w="<<w<<"  w2="<<w2<<"  h="<<h<<"  w*h="<<w*h<<endl;
	 cout<<"imax="<<imax<<"   vmax="<<vmax<<"   sum="<<(int)sum
	 <<"   3*sum/h="<<((int)(3*sum/h))<<endl;
	 cout<<"(int)imax="<<(int)imax<<"  int(w2)="<<int(w2)<<endl;
	 cout<<"iskew="<<iskew<<endl<<"alpha="<<alpha<<endl<<endl;
	 */
	
	if(GrafT){TIME_PRINT}
	
	return alpha;
	
}//_____________________________________________________________________________

// функция быстрого преобразования Радона

void
GBitmap::toolsRadon( int sign,
					unsigned int sharpness[],
					unsigned int w2,
					unsigned int g
					) {
	
///	cout<<endl<<"toolsRadon(int sign, unsigned int sharpness[]), version V0"<<endl;
///	cout<<"sign="<<sign<<endl;
	
	// TIME_START  // time=0.078*2   w=2698 h= 4000
	
	unsigned int h=nrows;      // высота массива   IMAGE_HEIGHT
	unsigned int w=ncolumns;   // ширина массива   IMAGE_WIDTH
	
	//// cout<<"h="<<h<<endl;   if (g==24) {h=h/2; g=4;}   cout<<"h="<<h<<endl; cout<<"g="<<g<<endl;  //***
	
	unsigned int w_g=w/g;      // w_8=w/8;  //***
	unsigned int x, y;
	
	unsigned int s=h*w2;
	unsigned short *p1, *p2;
	p1=new unsigned short [s];
	p2=new unsigned short [s];
	
	// обнуление массива p1
	memset(p1, 0, sizeof(unsigned short)*s);
	
	unsigned char *p0;     // unsigned char* input=bytes_data;
	unsigned int  n, d, sum;
	unsigned int  Psum=255*g;  //***
	
	// два варианта входного массива bytes_data:  серый или черное белый
	
	// первый вариант:
	// входной массив bytes_data серый (от 255 до 0), каждый байт char:
	// белое соответствует коду 255, черное 0
	// усреднение по серому восьми последовательных отсчетов (sum)
	// и преобразование в новое серое (от 0 до 8): белое кодируется 0  черное 8,
	// запись серого в массив p1
	
	// второй вариант:
	// входной массив bytes_data ч/б, каждый байт char: белое соответствует
	// коду 0, черное 1 (в каждом байте используется всего один разряд из восьми)
	// и подсчет числа единиц (sum) в восьми последовательных отсчетах,
	// запись суммы в массив p1
	
	
	
	//  универсальный; 1*8, 1*4, 1*2, 1*1
	for(y=0; y < h; y++) {       // ir
		p0 = bytes_data + y*w;
		for(x=0; x < w_g; x++) {  // ic
			// выборка из массива bytes_data
			// и суммирование g (восьми) последовательных отсчетов
			d=x*g; sum=0;
			if(sign>0) { for(n=0; n< g; n++) { sum+=*(w-1 - n - d + p0); } }
			else       { for(n=0; n< g; n++) { sum+=*(      n + d + p0); } }
			// первый вариант: преобразование серого (от 255 до 0) в усредненное
			// серое (от 0 до 8), запись в транспонированный массив p1
			///				p1[h*x + y]=(2040-sum)/255;  // /255   // >>10  /1024  28*8=1024
			p1[h*x + y]=(Psum-sum)/255; // >>10  /1024  128*8=1024  // 2040=255*8
			// второй вариант: для входного массива bytes_data ч/б
			// подсчет числа едениц в горизонтальном байте, запись в массив p1
			////			p1[h*x + y]=sum;
		} // x
	} // y
	/**/
	
	/*
	 // базовый алгоритм  1*8
	 unsigned int w_8=w_g;
	 for(y=0; y < h; y++) {       // ir
	 p0 = bytes_data + y*w;
	 for(x=0; x < w_8; x++) {  // ic
	 // выборка из массива bytes_data
	 // и суммирование восьми последовательных отсчетов
	 d=x*8; sum=0;
	 if(sign>0) { for(n=0; n< 8; n++) { sum+=*(w-1 - n - d + p0); } }
	 else       { for(n=0; n< 8; n++) { sum+=*(      n + d + p0); } }
	 // первый вариант: преобразование серого (от 255 до 0) в усредненное
	 // серое (от 0 до 8), запись в транспонированный массив p1
	 p1[h*x + y]=(2040-sum)/255;  // /255   // >>10  /1024  28*8=1024
	 // второй вариант: для входного массива bytes_data ч/б
	 // подсчет числа едениц в горизонтальном байте, запись в массив p1
	 ////			p1[h*x + y]=sum;
	 } // x
	 } // y
	 */
	
	
	// Iterate
	// быстрое преобразование Радона
	unsigned int i, j, m;
	unsigned short *s1, *s2, *t1, *t2;
	unsigned short *x1=p1, *x2=p2;  //
	unsigned short *aux;
	unsigned int step=1;
	
	while(1){
		for(i=0; i<w2; i+=2*step) {
			for(j=0; j<step; j++) {
				
				s1=x1+h*(i+j);
				s2=x1+h*(i+j+step);
				
				t1=x2+h*(i+2*j);
				t2=x2+h*(i+2*j+1);
				
				for(m=0; m<h; m++){
					t1[m]=s1[m];
					t2[m]=s1[m];
					if( m+j<h )   { t1[m]+=s2[m+j]; }
					if( m+j+1<h ) { t2[m]+=s2[m+j+1]; }
				} // m
			} // j
		} // i
		// Swap the tables:
		aux=x1;                // unsigned short *aux=x1;  aux=x1;
		x1=x2;
		x2=aux;
		// Increase the step:  // Увеличение шага:
		step*=2;
		if(step>=w2) break;
	} // while(1)
	/**/
	
	
	// Now, compute the sum of squared finite differences:
	// Теперь, вычисляем сумму квадратов конечных разностей:
	unsigned int acc;
	unsigned short int *d0;             //unsigned short int *col;
	int diff;
	for(x=0; x < w2; x++){
		d0=x1 + h*x;  acc=0;
		for(y=0; y< h-1; y++){          // for(y=0; y+1< h; y++)
			diff=*(d0 + y) - *(d0 + y+1);   // {diff=(int)(d0[ir])-(int)(d0[ir+1]);}
			acc+=diff*diff;
		} // y
		sharpness[w2-1 + sign*x] = acc;
		///		cout_<<" "<<acc;
	} // x
	///cout_<<END<<END;
	/**/
	
	
	// освобождение массивов p1, p2
	delete p1;     delete p2;
	
	///TIME_PRINT
	
	/*
	 // вывод псевдографики w*h    белый 255
	 for ( y=0;  y< h; y++ ) {
	 //scl=bytes_data + y*w;
	 for ( x=0; x < w; x++ ) {
	 // cout_<<(short)bytes_data[y*w + x]<<"";
	 if(bytes_data[y*(w)+x]>128) {cout_<<"1";} else{cout_<<".";}
	 //if ((x)%(w/8)==0) cout_<<END;
	 }
	 cout_<<END;
	 }
	 cout_<<END;
	 */
	/*
	 // вывод псевдографики  (w/8)*h  ++++
	 for ( x=0; x < w_8; x++ ){
	 for ( y=0;  y < h; y++ ){
	 cout_<<(short)p1[x*h + y]<<"";
	 // if(p1[x*h+y]>1) {cout_<<"1";} else{cout_<<".";}
	 // if ((x)%(w/8)==0) cout_<<END;     [y*(w)+x]
	 }
	 cout_<<END;
	 }
	 cout_<<END;
	 */
	
	//cout_<<"w="<<w<<"  w_8="<<w_8<<"  w2="<<w2<<"  h="<<h<<"  s="<<s<<END;
	
	/**/
}//_____________________________________________________________________________

//******************************************************************************

// Отимизация исходного массива bytes_data для определения угла наклона текста

void GBitmap::optimizationSkew( unsigned int g ) {
	
	cout_<<END<<"optimizationSkew(unsigned int g), version V0"<<END;
	
	// TIME_START  // time=0.078*2   w=2698 h= 4000
	
	unsigned int h=nrows;      // высота массива   IMAGE_HEIGHT
	unsigned int w=ncolumns;   // ширина массива   IMAGE_WIDTH
	
	cout_<<"h="<<h<<END;
	if (g==24) {h=h/2; g=4;}   cout_<<"h="<<h<<END; cout_<<"g="<<g<<END;
	
	unsigned int w_g=w/g;      // w_8=w/8;
	unsigned int x, y;
	
	unsigned int s=h*w_g;
	//unsigned short *p1;        // , *p2
	bytes_data_buf=new unsigned char [s];
	
	
	// обнуление массива bytes_data_buf          p1
	memset(bytes_data_buf, 0, sizeof(unsigned char)*s);
	
	unsigned char *d;     // unsigned char* input=bytes_data;
	unsigned int  n, m, sum;
	//unsigned int  Psum=255*g;
	
	// два варианта входного массива bytes_data:  серый или черное белый
	
	// первый вариант:
	// входной массив bytes_data серый (от 255 до 0), каждый байт char:
	// белое соответствует коду 255, черное 0
	// усреднение по серому восьми последовательных отсчетов (sum)
	// и преобразование в новое серое (от 0 до 8): белое кодируется 0  черное 8,
	// запись серого в массив p1
	
	// второй вариант:
	// входной массив bytes_data ч/б, каждый байт char: белое соответствует
	// коду 0, черное 1 (в каждом байте используется всего один разряд из восьми)
	// и подсчет числа единиц (sum) в восьми последовательных отсчетах,
	// запись суммы в массив p1
	
	
	
	//******************************************************************************
	
	//  универсальный; 1*8, 1*4, 1*2, 1*1
	m=0;
	for(y=0; y < h; y++) {       // ir
		d = bytes_data + y*w;   m++;  n=0;
		for(x=0; x < w-8; x++) {  // ic
			// выборка из массива bytes_data
			// и суммирование g (восьми) последовательных отсчетов
			n++;
			sum=*(d)+*(d+1)+*(d+2)+*(d+3)+*(d+4)+*(d+5)+*(d+6)+*(d+7);
			// sum+=*(d + p0); //sum+=*(n + d + p0);     *(d+x)
			// первый вариант: преобразование серого (от 255 до 0) в усредненное
			// серое (от 0 до 8), запись в транспонированный массив p1
			//bytes_data_buf[h*x + y]=(Psum-sum)/255; // >>10  /1024  128*8=1024  // 2040=255*8
			//				bytes_data_buf[h*x + y]=(Psum-sum)/255;
			
			// второй вариант: для входного массива bytes_data ч/б
			// подсчет числа едениц в горизонтальном байте, запись в массив p1
			////			p1[h*x + y]=sum;
		} // x
	} // y
	/**/
	
	
	//******************************************************************************
	
	//  2*4
	/*
	 // усреднение исходного массива bytes_data на два (2) пиксепя по вертикали
	 for(x=0; x < w; x++) {
	 p0 = bytes_data + x*nrows;
	 n=0;
	 for(y=0; y < nrows-1; y+=2) {  // X+=dx
	 //for(y=1; y < nrows; y+=2) {  // X+=dx
	 b0= p0 + y;  n++;
	 // *b0=*b0 + *(b0 - 1);
	 // *b0+=*(b0 - 1);
	 *(p0 + n)=*b0 + *(b0 + 1);
	 } // y
	 } // x
	 */
	/*
	 // усреднение исходного массива bytes_data на два (2) пиксепя по вертикали
	 for(y=1; y < nrows; y+=2) {  // X+=dx
	 p0 = bytes_data + y*w;
	 for(x=0; x < w; x++) {
	 b0= p0 + x;
	 // *b0=*b0 + *(b0 - w);
	 // *b0+=*(b0 - w);
	 *b0+=*(b0 - w);
	 } // x
	 } // y			 }
	 */
	/*
	 //  2*4
	 for(y=0; y < h; y++) {    // h   // ir   y++
	 p0 = bytes_data + y*w;
	 for(x=0; x < w_g; x++) {  // ic
	 // выборка из массива bytes_data
	 // и суммирование g (4) последовательных отсчетов
	 d=x*g; sum=0;
	 if(sign>0) { for(n=0; n< g; n++) { sum+=*(w-1 - n - d + p0); } }
	 else       { for(n=0; n< g; n++) { sum+=*(      n + d + p0); } }
	 // первый вариант: преобразование серого (от 255 до 0) в усредненное
	 // серое (от 0 до 8), запись в транспонированный массив p1
	 p1[h*x + y]=(Psum-sum)/255;  // /255   // >>10  /1024  28*8=1024
	 ///				p1[h*x + y]=(1020-sum)/255; // >>10  /1024  128*8=1024  // 2040=255*8
	 // второй вариант: для входного массива bytes_data ч/б
	 // подсчет числа едениц в горизонтальном байте, запись в массив p1
	 ////			p1[h*x + y]=sum;
	 } // x
	 } // y
	 */
	//******************************************************************************
	
	///TIME_PRINT
	
	/*
	 // вывод псевдографики w*h    белый 255
	 for ( y=0;  y< h; y++ ) {
	 //scl=bytes_data + y*w;
	 for ( x=0; x < w; x++ ) {
	 // cout_<<(short)bytes_data[y*w + x]<<"";
	 if(bytes_data[y*(w)+x]>128) {cout_<<"1";} else{cout_<<".";}
	 //if ((x)%(w/8)==0) cout_<<END;
	 }
	 cout_<<END;
	 }
	 cout_<<END;
	 */
	/*
	 // вывод псевдографики  (w/8)*h  ++++
	 for ( x=0; x < w_8; x++ ){
	 for ( y=0;  y < h; y++ ){
	 cout_<<(short)p1[x*h + y]<<"";
	 // if(p1[x*h+y]>1) {cout_<<"1";} else{cout_<<".";}
	 // if ((x)%(w/8)==0) cout_<<END;     [y*(w)+x]
	 }
	 cout_<<END;
	 }
	 cout_<<END;
	 */
	
	//cout_<<"w="<<w<<"  w_8="<<w_8<<"  w2="<<w2<<"  h="<<h<<"  s="<<s<<END;
	/**/
}//_____________________________________________________________________________



















/*
 // отлажена работает но не понадобилась //
 // Предобработка битовой картинки для функция определения угла наклона текста
 
 void GBitmap::detectContour() {
 
 // пространственный Лапласиан, убирает сплошные черные области и подчеркивает граници,
 // а ч.б. изображение превращает в контурное, с двухпиксельными контуром,
 // что благоприятно для алгоритмов определения угла наклона текста
 
 // Вызов функции предобработка битовой картинки
 ///	detectContour();
 
 int w=ncolumns, h=nrows;      // char
 unsigned int x, y;
 int w_h=w*h;
 //unsigned char *A;
 unsigned char* A=bytes_data;
 
 cout_<<END<<"detectContour(), version V0"<<END;
 
 TIME_START
 //
 //unsigned char *p0;
 for (x=0; x < w_h; x++){
 //bytes_data[x]=1 - bytes_data[x]/128;
 *(A +x )=1 - *(A +x )/128; // побитовая выборка из массива bytes_data
 //b=1-b/128; // преобразование серого в черно белый
 }
 //
 
 
 //bool *p,*d,*d1,*d2,*d3,*d4,*d5;
 char *p,*d,*d1,*d2,*d3,*d4,*d5;
 //A=bytes_data;
 //      -1
 //       0
 //  -1 0 4 0 -1
 for (int y=5; y<h-3; y++){                            //       0
 d =A+(y-4)*w-4;  //A[y-4][x-4]                     //      -1
 d1=A+y*w;        //A[y][x]
 d2=d1+2*w;       //A[y+2][x]
 d3=d1-2*w;       //A[y-2][x]
 d4=d1+2;         //A[y][x+2]
 d5=d1-2;         //A[y][x-2]
 for (int x=5; x<w-3; x++){
 //  выделение граници в 2 пиксела
 // A[y-4][x-4] =4*A[y][x]-A[y+2][x]-A[y-2][x]-A[y][x+2]-A[y][x-2]
 *(d+x)=*(d1+x)*4-*(d2+x)-*(d3+x)-*(d4+x)-*(d5+x);
 // *d0=255;
 ///		 *(bytes_data + y*w + x)=0;
 
 //#ifdef DEBUGLVL
 //	   drawData[0][y-5][x-5]=255+*(d+x)*196;  // draw it
 //#endif
 }}  // for x,y
 
 
 
 for (x=0; x < w_h; x++){
 bytes_data[x]=(1 - bytes_data[x])*255;
 // bytes_data[x]=bytes_data[x]/128;
 // b=*(p0 +x ); // побитовая выборка из массива bytes_data
 // b=1-b/128;   // преобразование серого в черно белый
 }
 
 
 TIME_PRINT
 
 // *(d+x)=*(d1+x)*2-*(d2+x)-*(d3+x); // -*(d4+x)-*(d5+x)
 //  (нормировка скана)
 }//_____________________________________________________________________________
 */


/**/
/*
 short w=ncolumns;
 short h=nrows;
 bool *p,*p1,*p2,*p3,*p4,*p5;
 
 for (int x=5; x<w-3; x++){
 for (int y=5; y<h-3; y++){                       //      -1
 //  выделение граници в 2 пиксела                //       0
 p=A+(y-4)*w+x-4; //A[y-4][x-4]                   //  -1 0 4 0 -1
 p1=A+y*w+x;      //A[y][x]                       //       0
 p2=A+(y+2)*w+x;  //A[y+2][x]                     //      -1
 p3=A+(y-2)*w+x;  //A[y-2][x]
 p4=A+y*w+x+2;    //A[y][x+2]
 p5=A+y*w+x-2;    //A[y][x-2]
 // A[y-4][x-4] =4*A[y][x]-A[y+2][x]-A[y-2][x]-A[y][x+2]-A[y][x-2]
 *p=*p1*4-*p2-*p3-*p4-*p5;
 
 #ifdef DEBUGLVL
 drawData[0][y-5][x-5]=255+*p*196;  //draw it
 #endif
 }}  // for x,y
 */

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

////    unsigned int w2=PageTools_Next_Pow2(FreeImage_GetPitch(dib));
// Получить ширина картинки в байта с округлением до следующего 32-разрядных границей,
// также известный как шаг или шаг или сканирования ширину. В FreeImage каждая
// строка начинается с 32-битной границе из соображений производительности.
// Это доступа к существенным при использовании функции нижних уровне пикселей
// манипуляции (см. также главу о функциях Pixel доступа).

// Возвращает:
// Возвращает ширину картинки в байта с округлением до следующего 32-разрядной границе,
// также известный как шаг или шаг или сканирования ширину.

// Я немного изменил алгоритм FindSkew, взятый из проекта PageTools - а именно,
// найденный угол перекоса умножается на глубину цвета картинки (в битах).
// Это позволило применять данный алгоритм не только к чёрно-белым изображениям
// (как в проекте PageTools), но также и к серым, и к цветным изображениям.


// http://www.radgs.com/docs/help/win32.graph.freeimage.html
// bw_=(w+7)/8;
//     unsigned char const *scanline(unsigned int ir)const{
//     return data_+ir*bw_; }
//


//unsigned int w2=(w/32)*32/8;   		//unsigned int w2=next_pow2(img.bytewidth());  ширина байта
// unsigned int w2=(w/32)*32/4;  //// /8   // w2 выровненное по 32 значение w  w2=(w/32+1)*32;
///unsigned int w2=(w+7)/8;


////	 unsigned char const *scl= FreeImage_GetScanLine(dib, ir);
//       nsigned char const *scl=img.scanline(ir);
//       unsigned int w2=next_pow2(img.bytewidth());  //  ширина байта


// FreeImage_GetHeight - Returns the height of the bitmap in pixel units.
// Возвращает высоту картинки в пикселях
//unsigned int h=FreeImage_GetHeight(dib);

//	 cout_<<"angle2="<<angle2<<END;
/// DLL_API BYTE *DLL_CALLCONV FreeImage_GetScanLine(FIBITMAP *dib, int);
// Returns a pointer to the start of the given scanline in the bitmap’s data-bits.
// It is up to you to interpret these bytes correctly, according to the results of
/// Это зависит от вас, чтобы интерпретировать эти байты правильно, в соответствии с результатами
// FreeImage_GetBPP and FreeImage_GetImageType

// Размер таблицы разнности (резкости, четкости?)
// , около 0 и 90 градусов

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*
 unsigned int w_8=(w+7)/8;   //  unsigned int data_size_p=data_size/8;
 //unsigned int w_8=data_size/8/h;
 
 // побайтовое получение данных
 for(y=0; y<h; y++) {
 
 scl=bites_data + y*w_8;
 
 for(x=0; x<w_8; x++) {  /////////// wwwwwww
 
 // bites_data[y*(w/8)+x]
 
 if(sign>0) { val = scl[w_8-1 - x];   p1[h*x + y]=sumbit[val];} //8-bitcount[val]; }  /////////// wwwwwww
 else	    { val = scl[x];           p1[h*x + y]=sumbit[val];} //8-bitcount[val]; }
 
 ///		 if(stp==8){ index++; stp=0; } stp++;      // x=index
 //		 if(sign>0) { p1[h*ic+ir] = bitcount[scl[w-1-ic]]; }
 //		 else	    { p1[h*ic+ir] = bitcount[scl[ic]]; }
 
 // *(bytes_data+ir*w+ic)=0;
 } // ic
 }   // ir
 
 
 // вывод псевдографики
 for ( y=0;  y < h; y++ ) {
 for ( x=0; x < w_8; x++ ) {
 //cout_<<(short)bites_data[y*(w/8)+x]<<"";
 if(bites_data[y*w_8 + x]>0) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;
 }
 cout_<<END;
 }
 cout_<<END;
 
 
 //reg1=0;   reg2=0;
 ///unsigned int index=0;
 ///unsigned int stp=0;
 */

//******************************************************************************

//float alpha = 57.29747*atan((double)iskew/(8*w2));           // *FreeImage_GetBPP(dib);
///	float alpha = 57.29747*atan((float)((iskew)/(8*w2)));


/*
 int d;
 unsigned int s;
 
 // s=bites_data[y];   d=y*8;
 for( x=0; x < 8; x++ ){
 ///				  (Buf[d + x])=s & 1;     s=(s >> 1); // прямой порядок пикселей
 Buf[d + 8-x-1]=255*(s & 1);  s=(s >> 1); // обратный порядок пикселей
 // уровни в битмапе  0 и 255
 }
 
 
 
 
 // вывод псевдографики
 for ( y=0;  y < h; y++ ){
 for ( x=0; x < w; x++ ){
 cout_<<(short)p1[y*w+x]<<"";
 //if ((x)%(w/8)==0) cout_<<END;     [y*(w)+x]
 }
 cout_<<END;
 }
 */



/*
 // вычисление w2 как ближайшей  степени двойки w причем w2>w, напрямер w=1035 а w2=2048
 unsigned int w_8=w/8;   //  w_8=(w+7)/8;
 //unsigned int w_8=(w+7)/8;
 unsigned int retval=1;
 while(retval<w_8){          // img.bytewidth()    // unsigned int w=img.bytewidth();  // байт
 retval<<=1;
 }
 unsigned int w2=retval;
 //w2=w2/8;
 */


//******************************************************************************


// FreeImage_GetPitch - Returns the width of the bitmap in bytes, rounded
// to the next 32-bit boundary, also known as pitch or stride or scan
// Возвращает ширину картинки в байта с округлением
// на следующий 32-разрядной границе, также известный как шаг или шаг или сканирование




//cout_<<"sizeof(int)="<<sizeof(int)<<END;   bites_data  указатель
/*
 unsigned char* Buf = new unsigned char [w*h];
 // обнуление массива new_data
 //memset(bytes_data,0,data_size);
 
 // вывод псевдографики
 for ( y=0;  y< h; y++ ) {
 for ( x=0; x < w; x++ ) {
 //cout_<<(short)bites_data[y*(w/8)+x]<<"";
 if(bytes_data[y*(w)+x]>0) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;
 }
 cout_<<END;
 }
 cout_<<END;
 */

/*
 unsigned int* Buf;
 // инициализация битового массива char размером  data_size_p=data_siz/8
 Buf=(unsigned char*)malloc(Buf);
 // обнуление bites_data
 memset(Buf,0,w*h);
 // выход в случае отсутствия свободной памяти запрашиваемого размера
 //if (bites_data==NULL) {DC("bites_data"); exit (1);}
 
 // обнуление массива new_data
 memset(Buf,0,w*h);
 */
/*
 // вывод псевдографики
 for ( y=0;  y< h; y++ ) {
 for ( x=0; x < w/8; x++ ) {
 //cout_<<(short)bites_data[y*(w/8)+x]<<"";
 if(bites_data[y*(w/8)+x]>0) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;
 }
 cout_<<END;
 }
 cout_<<END;
 
 // РАСПАКОВКА
 // Распаковка маленького битового массива (в одном байте 8 бит) в
 // большй стандартный байтовый массив битмап (в одном байте 1 бит).
 for( y=0; y < data_size_p; y++ ){  // >>3  деление на 8
 s=bites_data[y];   d=y*8;
 for( x=0; x < 8; x++ ){
 ///				  (Buf[d + x])=s & 1;     s=(s >> 1); // прямой порядок пикселей
 Buf[d + 8-x-1]=255*(s & 1);  s=(s >> 1); // обратный порядок пикселей
 // уровни в битмапе  0 и 255
 } // x
 } // y
 
 
 // вывод псевдографики
 for ( y=0;  y< h; y++ ) {
 for ( x=0; x < w; x++ ) {
 //cout_<<(short)bites_data[y*(w/8)+x]<<"";
 if(Buf[y*(w)+x]>0) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;
 }
 cout_<<END;
 }
 cout_<<END;
 */


/*	unsigned char *scl=bytes_data;    // unsigned char* input=bytes_data;
 
 for (x=0; x<w*h; x++) {
 if ( scl[x]  > 127 )  { scl[x]=1; }    // 255
 else { scl[x]=0; }
 } //x
 */


// освобождение маленького массива
///		if ( Buf !=NULL ) free(Buf);

//******************************************************************************

/*
 unsigned char* sumbit=new unsigned char[256];
 
 // создание таблицы для подсчета числа едениц в горизонтальном байте
 cout_<<END<<END;
 unsigned int i, j, ir, ic, cnt;
 for(i=0; i<256; i++) {
 j=i, cnt=0;
 do cnt+=j&1;  while(j>>=1);
 sumbit[i]=cnt;
 //cout_<<" "<<cnt;
 }
 cout_<<END<<END;
 */

//******************************************************************************

/*
 // побитовое получение данных
 // вход - белый 255  черный 0  (серый)
 // преобразование в белый 0  черный 1, упаковка в байты по 8 щтук
 // и подсчет, с помощью таблицы sumbit, числа едениц в байте
 for(y=0; y<h; y++) {   // ir
 
 scl=bytes_data + y*w;   // байт
 
 for(x=0; x<w; x++) {  // ic
 
 if(sign>0) {
 b=scl[w-1-x];
 //if (b > 128)  { b = 0; }  else { b = 1; }
 b=1-b/128;                                       //  >>7 деление на 128
 g=(g << 1) + b;
 val=g;    p1[h*(x/8) + y]=sumbit[val];           //  y*w + x
 }
 else	      {
 b=scl[x];
 //if (b > 128)  { b = 0; }  else { b = 1; }
 b=1-b/128;
 g=(g << 1) + b;            //    <<=*
 val=g;    p1[h*(x/8) + y]=sumbit[val];
 }
 //		   if(sign>0) { p1[h*ic+ir] = bitcount[scl[w-1-ic]]; }
 //		   else	    { p1[h*ic+ir] = bitcount[scl[ic]]; }
 } // x
 }   // y
 */


//******************************************************************************
//   рабочие варианты обработки входного массива  //
//------------------------------------------------------------------------------
/*
 // входной массив bytes_data серый (от 255 до 0), каждый байт char:
 // белое соответствует коду 255, черное 0
 // усреднение по серому восьми последовательных отсчетов (sum)
 // и преобразование в новое серое (от 0 до 8): белое кодируется 0  черное 8,
 // запись серого в массив p1
 
 // Fill in the first table   // Заполняем первую таблицу
 
 if(sign>0) {
 for(y=0; y < h; y++) {       // ir
 p0 = bytes_data + y*w;
 for(x=0; x < w_8; x++) {  // ic
 // побитовая выборка из массива bytes_data
 // и суммирование по серому восьми последовательных отсчетов
 d=x*8; sum=0;
 for(n=0; n < 8; n++) { sum+=*(p0 + w-1 - (d + n)); }
 // преобразование серого (от 255 до 0) в усредненное серое (от 0 до 8)
 // запись в транспонированный массив p1
 p1[h*x + y]=(2040-sum)/255;   // /255    //  >>10   /1024  128*8=1024
 } // x
 } // y
 } // if
 
 else {
 for(y=0; y < h; y++) {       // ir
 p0 = bytes_data + y*w;
 for(x=0; x < w_8; x++) {  // ic
 d=x*8; sum=0;
 for(n=0; n < 8; n++) { sum+=*(p0 + d + n); }
 p1[h*x + y]=(2040-sum)/255; // >>8
 ///				if(sum>1024) {cout_<<"1";} else{cout_<<".";}
 //				 cout_<<(short)((2040-sum)/255)<<"";    // (2040-sum)/256;
 } // x
 ///		cout_<<END;
 } // y
 } // else
 */

//------------------------------------------------------------------------------

/*
 // входной массив bytes_data ч/б, каждый байт char: белое соответствует
 // коду 0, черное 1 (в каждом байте используется всего один разряд из восьми)
 // и подсчет числа единиц (sum) в восьми последовательных отсчетах,
 // запись суммы в массив p1
 
 // Fill in the first table   // Заполняем первую таблицу
 
 unsigned char  b;
 if(sign>0) {
 for(y=0; y < h; y++) {       // ir
 p0 = bytes_data + y*w;
 for(x=0; x < w_8; x++) {  // ic
 // побитовая выборка из массива bytes_data
 // подсчет числа единиц (sum) в восьми последовательных отсчетах
 d=x*8; sum=0;
 for(n=0; n < 8; n++) { sum+=*(p0 + w-1 - (d + n)); }
 // тест если bytes_data не ч/б а серый (от 0 до 255)
 //for(n=0; n < 8; n++) { b=*(p0 + w-1 - (d + n)); sum+=1-b/128; }
 // запись в транспонированный массив p1
 p1[h*x + y]=sum;   // /255
 } // x
 } // y
 } // if
 
 else {
 for(y=0; y < h; y++) {       // ir
 p0 = bytes_data + y*w;
 for(x=0; x < w_8; x++) {  // ic
 d=x*8; sum=0;
 for(n=0; n < 8; n++) { sum+=*(p0 + d + n); }
 // тест если bytes_data не ч/б а серый (от 0 до 255)
 //for(n=0; n < 8; n++) { b=*(p0 + d + n); sum+=1-b/128; }
 p1[h*x + y]=sum;
 ///				if(sum>0) {cout_<<"1";} else{cout_<<".";}
 //				 cout_<<(short)sum<<"";
 } // x
 ///		cout_<<END;
 } // y
 } // else
 */

//------------------------------------------------------------------------------

// TIME_PRINT




//		   if(sign>0) { p1[h*ic+ir] = bitcount[scl[w-1-ic]]; }
//		   else	    { p1[h*ic+ir] = bitcount[scl[ic]]; }



/*                // TMP //
 // входной массив bytes_data серый каждый байт char:
 // белое соответствует 255, черное 0
 // преобразование в белый кодируется 0  черный 1,
 // и подсчет числа единиц в байте
 
 unsigned int  b;
 if(sign>0) {
 for(y=0; y<h; y++) {   // ir
 
 p0 = bytes_data + y*w;   // байт
 for(x=0; x<w_8; x++) {  // ic
 
 d=x*8; sum=0;
 for(n=0; n<8; n++) {
 b=*(p0 + w-1-(d + n)); // побитовая выборка из массива bytes_data
 b=1-b/128; // преобразование серого в черно белый
 sum+=b;    // и подсчет числа единиц в байте
 }// n
 p1[h*x + y]=sum;
 } // x
 } // y
 } // if
 
 else {
 for(y=0; y<h; y++) {   // ir
 
 p0 = bytes_data + y*w;   // байт
 for(x=0; x<w_8; x++) {  // ic   /8
 
 d=x*8; sum=0;
 for(n=0; n<8; n++) {
 b=*(p0 + d + n);
 //					 if(b>128) {cout_<<"1";} else{cout_<<".";}
 b=1-b/128;
 sum+=b;
 }// n
 p1[h*x + y]=sum;
 } // x
 //		cout_<<END;
 } // y
 } // else
 */

//------------------------------------------------------------------------------



/*
 
 // вывод псевдографики w*h    белый 255
 for ( y=0;  y< h; y++ ) {
 //scl=bytes_data + y*w;
 for ( x=0; x < w; x++ ) {
 // cout_<<(short)bytes_data[y*w + x]<<"";
 if(bytes_data[y*(w)+x]>128) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;
 }
 cout_<<END;
 }
 cout_<<END;
 */
/*
 // вывод псевдографики  (w/8)*h  ++++
 for ( x=0; x < w_8; x++ ){
 for ( y=0;  y < h; y++ ){
 cout_<<(short)p1[x*h + y]<<"";
 // if(p1[x*h+y]>1) {cout_<<"1";} else{cout_<<".";}
 //if ((x)%(w/8)==0) cout_<<END;     [y*(w)+x]
 }
 cout_<<END;
 }
 cout_<<END;
 */



//------------------------------------------------------------------------------


// TIME_PRINT




//		   if(sign>0) { p1[h*ic+ir] = bitcount[scl[w-1-ic]]; }
//		   else	    { p1[h*ic+ir] = bitcount[scl[ic]]; }



/*                // TMP //
 // входной массив bytes_data серый каждый байт char:
 // белое соответствует 255, черное 0
 // преобразование в белый кодируется 0  черный 1,
 // и подсчет числа единиц в байте
 
 unsigned int  b;
 if(sign>0) {
 for(y=0; y<h; y++) {   // ir
 
 p0 = bytes_data + y*w;   // байт
 for(x=0; x<w_8; x++) {  // ic
 
 d=x*8; sum=0;
 for(n=0; n<8; n++) {
 b=*(p0 + w-1-(d + n)); // побитовая выборка из массива bytes_data
 b=1-b/128; // преобразование серого в черно белый
 sum+=b;    // и подсчет числа единиц в байте
 }// n
 p1[h*x + y]=sum;
 } // x
 } // y
 } // if
 
 else {
 for(y=0; y<h; y++) {   // ir
 
 p0 = bytes_data + y*w;   // байт
 for(x=0; x<w_8; x++) {  // ic   /8
 
 d=x*8; sum=0;
 for(n=0; n<8; n++) {
 b=*(p0 + d + n);
 //					 if(b>128) {cout_<<"1";} else{cout_<<".";}
 b=1-b/128;
 sum+=b;
 }// n
 p1[h*x + y]=sum;
 } // x
 //		cout_<<END;
 } // y
 } // else
 */


//******************************************************************************

//	4			//p1[h*x + y]=(1020-sum)/255;

//------------------------------------------------------------------------------

/*
 unsigned int vmaxF;
 unsigned int *d0;
 d0 = sharpness + imax;     // p0 = bytes_data + y*w;
 if( imax >= 3 & imax <= ssize-3) {
 vmaxF=*d0*7 + (*(d0+1) + *(d0-1))*6 + (*(d0+2) + *(d0-2))*3 + (*(d0+3) + *(d0-3))*2;
 vmaxF=vmaxF/21;
 }
 cout_<<"vmaxF="<<vmaxF<<END;
 //double v=1<<31;  v=v*v;   cout_<<"v="<<v<<END;
 */

//------------------------------------------------------------------------------

//Alpha=57.29747*atan(F);
//angle = 57.29747*atan((float)(iskew)/(8*w2));

/*
 //------------------------------------------------------------------------------
 //// Используется если данные подаются в виде байт с упакованными битами ////
 
 // Упаковка большого битмапа в маленькй битовый массив.
 // Используется для уменьшения размера картинок букв при перетусовке их в памяти.
 ////	packImg();
 // cout_<<"data_size="<<data_sze<<END;
 
 // создание массива для подсчета числа едениц в горизонтальном байте
 unsigned char* sumbit=new unsigned char[256];
 
 // заполнение таблицы для подсчета числа едениц в горизонтальном байте
 // по адресу в массиве (байт) читаем число едениц в этом же байте
 cout_<<END<<END;
 unsigned int cnt;   //  ir, ic,
 for(i=0; i<256; i++) {
 j=i, cnt=0;
 do cnt+=j&1;  while(j>>=1);
 sumbit[i]=cnt;
 cout_<<" i="<<i<<" "<<cnt;
 }
 cout_<<END<<END;
 
 delete[] sumbit;
 //------------------------------------------------------------------------------
 */

/*
 for(ic=0; ic<w2; ic++){
 unsigned int acc=0;
 unsigned short int *col=x1+h*ic;
 for(ir=0; ir+1<h; ir++){
 int diff=(int)(col[ir])-(int)(col[ir+1]);
 acc+=diff*diff;
 } // ir
 sharpness[w2-1+sign*ic]=acc;
 ///		cout_<<" "<<acc;
 } // ic
 ///cout_<<END<<END;
 */

//------------------------------------------------------------------------------

/*
 // Iterate
 // быстрое преобразование Радона
 unsigned int i, j, ir, ic, m;
 unsigned short *s1, *s2, *t1, *t2;
 unsigned short *x1=p1, *x2=p2;  //
 unsigned int ds1, ds2, dt1, dt2;
 //unsigned int nPx=s;
 //  cout_<<"nPx="<<nPx<<END;
 unsigned short *aux;
 unsigned int step=1;
 while(1){
 for(i=0; i<w2; i+=2*step) {
 for(j=0; j<step; j++) {
 // Columns-sources:  // Столбцы-источники:
 //ds1=h*(i+j);       if(ds1>nPx){cout_<<"ds1="<<ds1; continue;} s1=x1+ds1; 	   // int
 //ds2=h*(i+j+step);  if(ds2>nPx){cout_<<"ds2="<<ds2; continue;} s2=x1+ds2;
 
 s1=x1+h*(i+j);
 s2=x1+h*(i+j+step);
 
 // Columns-targets:  // Столбцы-целей:
 //dt1=h*(i+2*j);     if(dt1>nPx){cout_<<"dt1="<<dt1; continue;} t1=x2+dt1;
 //dt2=h*(i+2*j+1);   if(dt2>nPx){cout_<<"dt2="<<dt2; continue;} t2=x2+dt2;
 
 t1=x2+h*(i+2*j);
 t2=x2+h*(i+2*j+1);
 
 for(m=0; m<h; m++){
 t1[m]=s1[m];
 t2[m]=s1[m];
 if( m+j<h )   { t1[m]+=s2[m+j]; }
 if( m+j+1<h ) { t2[m]+=s2[m+j+1]; }
 } // m
 } // j
 } // i
 // Swap the tables:
 aux=x1;                // unsigned short *aux=x1;  aux=x1;
 x1=x2;
 x2=aux;
 // Increase the step:  // Увеличение шага:
 step*=2;
 if(step>=w2) break;
 } // while(1)
 */

//------------------------------------------------------------------------------


//	// --наклона текста около 0 и 90 градусов  (+-6,5 градусов)
//	// --положительного или отрицательного угла

// unsigned char* newData = new unsigned char [new_w * new_h];

//------------------------------------------------------------------------------







//****************************************************************************//





/*

			// отлажена работает но не понадобилась //

// Предобработка битовой картинки для функция определения угла наклона текста



void GBitmap::detectContour() {



// пространственный Лапласиан, убирает сплошные черные области и подчеркивает граници,

// а ч.б. изображение превращает в контурное, с двухпиксельными контуром,

// что благоприятно для алгоритмов определения угла наклона текста



	// Вызов функции предобработка битовой картинки

///	detectContour();



	int w=ncolumns, h=nrows;      // char

	unsigned int x, y;

	int w_h=w*h;

	//unsigned char *A;

	unsigned char* A=bytes_data;



cout_<<END<<"detectContour(), version V0"<<END;



TIME_START

//

	//unsigned char *p0;

	for (x=0; x < w_h; x++){

		//bytes_data[x]=1 - bytes_data[x]/128;

		*(A +x )=1 - *(A +x )/128; // побитовая выборка из массива bytes_data

		//b=1-b/128; // преобразование серого в черно белый

	}

//





	//bool *p,*d,*d1,*d2,*d3,*d4,*d5;

	char *p,*d,*d1,*d2,*d3,*d4,*d5;

	//A=bytes_data;

														  //      -1

														  //       0

														  //  -1 0 4 0 -1

	for (int y=5; y<h-3; y++){                            //       0

	   d =A+(y-4)*w-4;  //A[y-4][x-4]                     //      -1

	   d1=A+y*w;        //A[y][x]

	   d2=d1+2*w;       //A[y+2][x]

	   d3=d1-2*w;       //A[y-2][x]

	   d4=d1+2;         //A[y][x+2]

	   d5=d1-2;         //A[y][x-2]

	   for (int x=5; x<w-3; x++){

		 //  выделение граници в 2 пиксела

		 // A[y-4][x-4] =4*A[y][x]-A[y+2][x]-A[y-2][x]-A[y][x+2]-A[y][x-2]

		 *(d+x)=*(d1+x)*4-*(d2+x)-*(d3+x)-*(d4+x)-*(d5+x);

		 // *d0=255;

///		 *(bytes_data + y*w + x)=0;



//#ifdef DEBUGLVL

//	   drawData[0][y-5][x-5]=255+*(d+x)*196;  // draw it

//#endif

	}}  // for x,y







	for (x=0; x < w_h; x++){

		bytes_data[x]=(1 - bytes_data[x])*255;

		// bytes_data[x]=bytes_data[x]/128;

		// b=*(p0 +x ); // побитовая выборка из массива bytes_data

		// b=1-b/128;   // преобразование серого в черно белый

	}





TIME_PRINT



// *(d+x)=*(d1+x)*2-*(d2+x)-*(d3+x); // -*(d4+x)-*(d5+x)

//  (нормировка скана)

}//_____________________________________________________________________________

*/

}