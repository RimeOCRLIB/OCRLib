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

#include "GBitmap.h"
#include "GBitMask.h"
#include "config.h" 
#include <string.h>
#include <stdio.h>
#include "php2stl.h"
#include "OCRAlgorihtm.h"

#ifdef ASM_OS64_SSE42_POPCNT_ATT
    #include <smmintrin.h>
#endif

///#ifdef CPU_OS64_popcnt
//#include <smmintrin.h>
///#endif

namespace ocr{
    
    GBitmap::~GBitmap(){
    }
    
    void
    GBitmap::destroy(void)
    {
        if(data_size){  
            if(!pack_flag){
                free(bytes_data);
            }else{
                free(bites_data);
            }
        }	
        delete this;
    }
    
    GBitmap::GBitmap()
    : nrows(0), ncolumns(0), border(0),
	bytes_per_row(0),data_size(0)
    
    {
    }
    
    GBitmap::GBitmap(int ncolumns,int nrows)
    : nrows(0), ncolumns(0), border(0),
	bytes_per_row(0), data_size(0)
    {
        try
        {
            init(ncolumns,nrows,1);
        }
        catch(int a)
        {
            destroy();
        }
    }
    
    GBitmap::GBitmap(int ncolumns,int nrows, int mode)
    : nrows(0), ncolumns(0), border(0),
	bytes_per_row(0), data_size(0)
    {
        try
        {
            init(ncolumns,nrows,mode);
        }
        catch(int a)
        {
            destroy();
        }
    }    
    
    
    GBitmap::GBitmap(const GBitmap* ref)
    : nrows(0), ncolumns(0), border(0),
	bytes_per_row(0),data_size(0)
    {
        try
        {
            init(ref);
        }
        catch(int a)
        {
            destroy();
        }
    }
	
    GBitmap::GBitmap(const GBitmap* ref, float scale, float rotation,int mode)
	: nrows(0), ncolumns(0), border(0),
	bytes_per_row(0),data_size(0)
    {
		try
		{
			init(ref,scale,rotation,mode);
		}
		catch(int a)
		{
			destroy();
		}
    }
    
    GBitmap::GBitmap(const GBitmap* ref, int x0,int y0, int w, int h)
	: nrows(0), ncolumns(0), border(0),
	bytes_per_row(0),data_size(0)
    {
        try
        {
            initRegion(ref, x0, y0, w, h);
        }
        catch(int a)
        {
            destroy();
        }
    }
	
	
    GBitmap::GBitmap(const char *path, int mode, int invert)
    : nrows(0), ncolumns(0), border(0),
    bytes_per_row(0),data_size(0)
    {
        try
        {   
            init(path, mode, invert);
        }
        catch(int a)
        {
            destroy();
        }
    }
	
    GBitmap::GBitmap(string &inputData,int invert)
    : nrows(0), ncolumns(0), border(0),
    bytes_per_row(0),data_size(0)
    {
		try
		{   
			init(inputData,invert);
		}
		catch(int a)
		{
			destroy();
		}
    }
	
	
    //____________________________________________________________________________	
    
    // ----- initialization
    
    void
    GBitmap::init(int acolumns, int arows,int mode){
        if(mode==BITMAP_32){
            nrows = arows;// = (arows/32+1)*32;       // nrows = arows;
            ncolumns =acolumns;// =(acolumns/32+1)*32;  // ncolumns = acolumns;
            data_size=ncolumns*nrows;
            data_size_p=data_size/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
            data_size_p32=data_size/32; /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
            w=ncolumns;  h=nrows;  bytes_per_row=ncolumns/8;
            border=0;
            if(!data_size)return;
            // инициализация байтового массива char размером  data_size
            bites_data=(unsigned char*)malloc(data_size_p);  // запрос памяти с очисткой 0
            memset(bites_data,0,data_size_p);
            pack_flag=BITMAP_32;
        }else{
            colorMode = mode;
            // выравнивание bytes_data по 32
            // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
            // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
            nrows = arows; //nrows = (arows/32+1)*32;       // nrows = arows;
            ncolumns =acolumns; //ncolumns =(acolumns/32+1)*32;  // ncolumns = acolumns;
            
            data_size=ncolumns*nrows*colorMode;
            if(!data_size)return;
            data_size_p=0;
            w=ncolumns;  h=nrows;  bytes_per_row=ncolumns*colorMode;
            border=0;
            
            //cout<<" data_size="<<data_size<<endl;
            // инициализация байтового массива char размером  data_size
            bytes_data=(unsigned char*)malloc(data_size);  // запрос памяти с очисткой 0
            
            fill(255);
            pack_flag=0;
        }
        
    }//____________________________________________________________________________
    
    
    void
    GBitmap::init(const GBitmap* ref){
        if (this != ref)
        {	
            border=0;   
            if(ref->pack_flag!=BITMAP_32){
                init(ref->ncolumns,ref->nrows,ref->colorMode);
                unsigned char *row = bytes_data;
                unsigned char *rowDest;
                for (int n=0; n<nrows; n++, row+=bytes_per_row){
                    rowDest=ref->bytes_data+bytes_per_row*n;
                    memcpy( (void*)row, (void*)rowDest,  ncolumns );
                }
            }else{
                init(ref->ncolumns,ref->nrows,BITMAP_32);
                memcpy( (void*)bites_data, (void*)ref->bites_data,  ncolumns*nrows/8 );
            }
        }
        
    }//____________________________________________________________________________
    
    
	/** @fn void GBitmap::initRegion(const GBitmap* ref, int x0,int y0, int w, int h)
	 @brief функция инициализации GBitmap регионом графического файла ref по заданным  координатам 
	 */
    void GBitmap::initRegion(const GBitmap* ref, int x0,int y0, int w, int h){
        //cout_<<"start"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
        ref[0][1][1];  //ошибка компиллятора
        data_size=0;	
        pack_flag=0;
	    //normalisation
	    if(x0<0)x0=0;
		if(y0<0)y0=0;
	    if(x0>ref[0].columns())x0=0;
	    if(y0>ref[0].rows())y0=0;
		if(w+x0>ref[0].columns())w=ref[0].columns()-x0;
	    if(h+y0>ref[0].rows())h=ref[0].rows()-y0;
		unsigned char *p;
        if (this != ref)
        {	
            init(w,h,ref->colorMode); //после инициализации битмап выровнен по 8
            //cout_<<"start1"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
            int x,y;      // ( переменная d, 7 ноября 2010, быстрее классической).
            unsigned char *d;
            uint refW=ref->columns();
            uchar *refP=ref->bytes_data;
            
            for (y = 0; y < h; y++){
                d=bytes_data+y*ncolumns;
                for (x = 0; x < ncolumns; x++){
                    p=d+x;
                    *p=*(refP+(y+y0)*refW+x0+x);
                    //if(  pImg[0][y][x]>150){
                    //   DS(0);
                    //}else{
                    //   DS(1);
                    //};
                }//DS(END);
            }
        }
        //cout_<<"done read"<<endl;
    }//_____________________________________________________________________________
	
    
    void GBitmap::init(const GBitmap* ref, float scale, float rotation,int mode){
        //cout << "@@@_____scale="<<scale<<endl;
        if (this != ref){
            pack_flag=0;
            
            if(scale!=0&&rotation!=0){
                rotateFast(ref,scale,rotation);
                return;
            }
            if(scale!=0&&mode==0){
                scaleFast(ref,scale);
                return;
            }
            if(scale!=0&&mode==1){
                bicubicScale(ref,scale);
                return;
            }
            if(rotation!=0){
                
                rotateFast(ref,1,rotation);
                return;
            }
            init(ref);
            
        }
        
    }//_____________________________________________________________________________
	
    
    void GBitmap::init(const char *path, int mode, int invert_){
        //cout << "@@@_____init new GBitmap from file="<<path<<endl;
        /*
        int arows,acolumns;
        nrows=0;ncolumns=0;data_size=0;pack_flag=0;
        data_size_p=0;	
        //readTIFF(path,invert_,&bites_data,&acolumns,&arows);
        //if(!acolumns||!arows)return;
        nrows = arows;
        ncolumns = acolumns;
        border = 0;
        bytes_per_row = ncolumns;
        readText(TIFF_string,path);
        
        
         for(int i=0;i<nrows;i++){
         for(int j=0;j<ncolumns/8;j++){
         if(bites_data[i*(ncolumns/8)+j]){cout_<<".";}else{cout_<<"0";}
         }cout_<<endl;
         }
         
        
        data_size=nrows*ncolumns;
        pack_flag=1;
        unpackImg(1);
        if(invert_)invert();
        */
		
    }//_____________________________________________________________________________
	
	
    void GBitmap::init(string &inputData,int invert_){
        // cout << "@@@_____init new GBitmap from Tiff string="<<endl;
		
		//int arows,acolumns;
		nrows=0;ncolumns=0;data_size=0;pack_flag=0;
		
		//readTIFF(path,invert,&bites_data,&acolumns,&arows);
		//readTIFFFromString(invert_,&bites_data,&ncolumns,&nrows);
        
		if(!ncolumns||!nrows)return;
		border = 0;
		bytes_per_row = ncolumns;
		
		/*
		 for(int i=0;i<nrows;i++){
		 for(int j=0;j<ncolumns/8;j++){
		 if(bites_data[i*(ncolumns/8)+j]){cout_<<".";}else{cout_<<"0";}
		 }cout_<<endl;
		 }
		 */ 
		
		data_size=nrows*ncolumns;
        data_size_p=0;
		pack_flag=1;
		unpackImg(1);
	    if(invert_)invert();
		
    }//_____________________________________________________________________________
	
    /** маштабирование изменяет содержимое исходного изображения */  
    
    void GBitmap::scaleFast(float scale){
        if(scale==0 || scale==1 )return;
        GBitmap *ref=GBitmap::create(this);
        if(data_size){  
            if(!pack_flag){
                free(bytes_data);
            }else{
                free(bites_data);
            }
        }
        scaleFast(ref,scale);
        ref->destroy();
        
    }//_____________________________________________________________________________
	
    // ГОТОВАЯ version
    
    /** масштабирование (Resample) серого (char) массива по методу "ближайших соседей" */
    void GBitmap::scaleFast(const GBitmap*ref, float scale){
        
        int x,y,yp;
        int w=ref->columns();    //cout_<<"w="<<w;  /// ширина массива
        int h=ref->rows();       //cout_<<" h="<<h; /// высота массива
        int new_p;             ///  пиксел, бижайший пиксел
        unsigned char *d0;
        //scale=1.33;
        //TIME_START
        // float scale=0.536;   масштаб нового массива float, ипи double
        if (scale>5){scale=(float)5;}   if (scale<0.2){scale=(float)0.2;} // ограничения на масштаб
        int scaleP=256*scale;
        // инициализация нового (маштабированного) битового массива bytes_data
        // Get a new buffer to interpolate into bool Resample   http://www.cplusplus.com/forum/general/2615/
        init(w*scale,h*scale,ref->colorMode);
        
        //cout_<<"init(), Resample (scale), New version V0 new_w="<<new_w<<" new_h="<<new_h<<" w="<<w<<" h="<<h<<endl;
        
        unsigned char *p0=bytes_data;
        unsigned char *p1=ref->bytes_data;
        
        int w_h=w*h-1;  // int new_h_new_w=nrows*ncolumns-1; // ( переменнне, 7 ноября 2010, быстрее классической).
        for(y = 0; y < nrows; y++) { //cout_<<y<<" ";        // ( упрощена, 25 ноября 2010, не много быстрее ).
            yp=((y<<8)/scaleP)*w;                             // *256
            d0=p0+y*ncolumns;
            for(x = 0; x < ncolumns; x++) {                   //p=y*new_w + x;
                //new_p=( ((int)(y/scale)*w) + ((int)(x/scale)) ); // медленние
                new_p=(x<<8)/scaleP + yp;
                //  разный масштаб по разным координатам
                /// new_p=( ((int)(y/scale_h)*w) + ((int)(x/scale_w)) );
                /// if(new_p>=w*h){continue;} if(p + x>=new_h*new_w){continue;}
                if(new_p>w_h){break;}
                *(d0 + x)=*(p1+new_p);
                // вывод на экран
                ////d=new_data + p;     if(*d>127){cout_<<"1";}else{cout_<<".";}
            } ////cout_<<endl;
        }
        
        //TIME_PRINT_
        //cout_<<"done" ;
        /**/
    }//_______________________________________________________________________________
    
    /**\brief  Вращение или (и) масштабирование изображения c заменой исходных данных */    
    void GBitmap::rotateFast(float angle){
        if(angle==0||fabs(1-angle)<0.01)return;
        GBitmap *ref=GBitmap::create(this);
        if(data_size){
            if(!pack_flag){
                free(bytes_data);
            }else{
                free(bites_data);
            }
        }
        rotateFast(ref,1,angle);
        ref->destroy();
    }//_______________________________________________________________________________
    
    
    
	// ГОТОВАЯ version        // НЕ ЗАБЫТЬ ПЕРЕНЕСТИ В ОСНОВНУЮ ПРОГРАММУ //
    
	/**  Вращение или (и) масштабирование битовой картинки */
    /**  поворот изображения осуществляется на произвольный угол и дискретные углы  +180°, +90°, -90° */
    // добавлено быстое вращение на +180° 16 марта 2013 года
    void GBitmap::rotateFast(const GBitmap*src, float scale, float rotation){
		
		// the scaling factor  // Коэффициент масштабирования  (обратно прпорциональный масштаб)
		// float scale=1.0;
        double scale_=(double)scale;		
#define SCALE scale_* ///выполнение масштабирования при вращении
		
        
		// the rotation angle  // угол поворота в градусах    ///ang=0.03;
		double ang=rotation / 57.29747;  // ang - угол поворота в радианах
		// 57.29747 = 180/3.1415 = PI / 180;
        
		int w,h;
		int x,y;
		w=src->columns(); // ширина массива  IMAGE WIDTH
		h=src->rows();    // высота массива  IMAGE HEIGHT
		int w_1=w-1;      int h_1=h-1;
		int w_2=w/2;      int h_2=h/2;
		int w_h=w*h;
		int k=1<<16;      //  k=65536        // 0x10000=65536      0x8000=32768
		int d0, d1;
		
        
        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        
        if (GrafT) { cout<<"Вращение или (и) масштабирование битовой картинки  rotateFast(rotation)  rotation="<<rotation<<endl; }
		
        
        
        /// Дискретное вpащение изображения на углы  +90°, -90°
		if (rotation==90||rotation==-90) { 
            
			if (GrafT) { cout<<"rotateFast(), rotate +90°, -90°  New version V1"<<endl; }			
			//TIME_START    //  time=0.14  w=2591  h=3990  rotation=-90
            //  time=0.35  w=16224 × h=2464  rotation=-90
            //  time=0.36  w=16224 × h=2464  rotation=90
            
			// изменение размеров выходной картинки для углов вращения 90 или -90 градусов			
			init(h,w,src->colorMode);  // обратный порядок h,w
			unsigned char *p0=bytes_data;
			unsigned char *p1=src->bytes_data;
			
            // Дискретное вpащение изображения на углы +90° или -90°
			// Rotate one pixel at a time.
			// rotate by-90°, вращение против часовой стрелки,
			unsigned int index_=0;  int dlt=1; // было unsigned int dlt до 5 ноября 2010 года
			// rotate by+90°, вращение по часовой стрелки.
			if (rotation== 90) { index_=w_h, dlt=-1; }
			
			// переупаковка (транспонирование) всего  горизонтального
			// массива строк в массив вертикальные строк.
			for ( x=w_1; x >-1 ; x-- ) { // w_1=w-1;   // time=0.016
				for ( y=0; y < w_h; y+=w){
					*(p0 + index_)= *(p1 + x + y);
					index_+=dlt;
				} // y
			} // x
            /**/
            /*
             // заготовка
             // Поворот на -90°. Переупаковка (транспонирование) горизонтального массива строк в массив вертикальных строк.
             for ( x=0; x < w; x++ ) {    // time=0.0  
                for ( y=0; y < w_h; y+=w ){ /// sizeBuf   w_
                    // *(p0 + index_)=*(p1 + x + y);  index_++;  // index_+=dlt; int dlt=1;
                    *p0=*(p1+y);  p0++; // p0++;  сквозной индекс в цикле переупаковки
                    // *p0=*(p1+y); cout<<" "<<(unsigned short)*p0;  p0++;
                } // y
                p1++; // +x
             } // x
             */
			//TIME_PRINT_
			if (GrafT) { cout<<"rotation="<<rotation<<endl; }
			return;
			
		}  // if (rotation==90||rotation==-90) 
		/**/
        
		
		// Для вращение изображения на произвольный угол и на 180°, размеры выходного изображения не меняются
		init(w,h,src->colorMode);    // прямой порядок h,w
		unsigned char *p0=bytes_data;
		unsigned char *p1=src->bytes_data;
        
        
        /// Дискретное вpащение изображения на 180°
        if (rotation==180) {
            
            if (GrafT) { cout<<"rotateFast(), rotate +180°  New version V1"<<endl; }
            //TIME_START    //  time=0.11  16224 × 2464  rotation== +180°   
            
            // Поворот на +180°. 
            p1=p1+w_h;
            for ( y=0; y < h; y++ ){    
                for ( x=0; x < w; x++ ){ 
                    *p0=*(p1-x);  p0++; // p0++;  сквозной индекс в цикле переупаковки  
                    //*p0=*(p1+x); - flip horizontal (p1-=w; ставим перед циклом x)
                    //cout<<" "<<(unsigned short)*p0;  p0++;     
                } // x 
                p1-=w;
                // p1+=w;               
            } // y
            /**/           
            //TIME_PRINT_
            if (GrafT) { cout<<"rotation="<<rotation<<endl; }
            return;
            
        } // if (rotation==180)
        
        
        /// Вращение битовой картинки на произвольный угол (возможность поворота на 180° не используем)
        
        //TIME_START     //  time=0.156  w=2591,  h=3990,  rotation=-5
        if (GrafT) { cout<<"rotateFast(), rotate free  version V1"<<endl; }
        
		int x0,y0,x1,y1, xp,yp, xdx,xdy,ydx,ydy;
        
        //	SCALE=1.0/(1.0+0.2*cos(ang*4.0)); //коэффициент yвеличения каpтинки
        
		double SinA=sin(ang);       // float, ипи double
		double CosA=cos(ang);
        
		// какyю точкy каpтинки надо изобpажать в веpхней левой точке экpана?
		//x0 = (160.0 + scale*(-160.0*CosA+100.0*1.2*SinA))*65536.0;
		//y0 = (100.0 + scale*(-100.0*CosA-160.0*SinA/1.2))*65536.0;
        
		x0 = (w_2 + SCALE(-w_2*CosA+h_2*SinA))*k;
		y0 = (h_2 + SCALE(-h_2*CosA-w_2*SinA))*k;
        
		// на сколько надо сместиться по каpтинке пpи пеpемешении по экpанy
		// на пиксель влево
		xdx = SCALE(CosA*k);     //xdx = scale*CosA*65536.0;
		xdy = SCALE(SinA*k);     //xdy = scale*SinA*65536.0/1.2;
		// на пиксель вниз
		ydx =-SCALE(SinA*k);     //ydx =-scale*SinA*65536.0*1.2;
		ydy = SCALE(CosA*k);     //ydy = scale*CosA*65536.0;
		
		
		for (y=0; y<h; y++) {
			// x0, y0 - кооpдинаты на каpтинке начала текyщей стpоки сканиpования
			// x1, y1 - кооpдинаты на каpтинке текyщей pисyемой точки
			x1 = x0;    y1 = y0;
			
			d0 = y*w;
			for (x=0; x<w; x++) {
				// xp, yp - кооpдинаты на каpтинке текyщей pисyемой точки
				xp = x1 >> 16;  yp = y1 >> 16;   // /65536
				
				d1 = yp*w;
				if ( xp>=0 && xp<=w_1 && yp>=0 && yp<=h_1 ) {
					// dest[0][y][x] = src[0][yp][xp];
					*(p0 + x + d0) = *(p1 + xp + d1);
				}
                // { dest[0][y][x] = 255; }
				else { *(p0 + x + d0) = 255; }
				
				x1+=xdx;   y1+=xdy; // пеpедвижение вдоль стpоки сканиpования
			} // x
			x0+=ydx;   y0+=ydy;     // пеpеход к новой стpоке сканиpования
		} // y
		
		if (GrafT) { cout<<"rotation="<<rotation<<endl; }
        
		//TIME_PRINT
        
		// http://www.enlight.ru/demo/faq/smth.phtml?query=alg_bitmap_rotate
		// "DEMO.DESIGN FAQ, http://www.enlight.ru/demo/faq".
		// Вpащение pастpовой каpтинки
		// http://algolist.manual.ru/graphics/rotate.php
        
    }//_________________________________________________________________________
    
    
    
    // Fills a bitmap with the given value
    // Заполняет растровое изображение заданным значением value
    
    void  GBitmap::fill(unsigned char value){
		
		memset (bytes_data, value, data_size);
        
        //  for(unsigned int y=0; y<rows(); y++)
        //	{
        //	  unsigned char* bm_y = (*this)[y];
        //	  for(unsigned int x=0; x<columns(); x++)
        //		bm_y[x] = value;
        //	}
    }//_________________________________________________________________________
    
    //рисование одного битмапа внутри другого по заданным координатам
    
    void GBitmap::drawImg(GBitmap *img,int x0,int y0){
        int w=img->columns();
        int h=img->rows();
        //cout<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<" nrows="<<nrows<<endl;
        int x,y,d_;
        unsigned char *p; 
        
        if(x0>0&&x0+w<=ncolumns&&y0>0&&y0+h<=nrows){  //рисование без проверки
            
            unsigned char *d; // (переменная d, 7 ноября 2010, быстрее классической).
            
            for (y = 0; y < h; y++){
                d_=(y0+y)*ncolumns+x0;
                d=bytes_data+d_;
                for (x = 0; x < w; x++){
                    p=d+x;
                    *p=img[0][y][x];
                }
            }
            //cout_<<"+";
        }else{   //рисование с проверкой
            for (y = 0; y < nrows; y++){
                for (x = 0; x < ncolumns; x++){
                    if(y-y0<0||y-y0>h||x-x0<0||x-x0>w)continue;
                    d_=y*ncolumns+x;
                    p=bytes_data+d_;
                    *p=img[0][y-y0][x-x0];
                    //cout_<<(short)*p/255;
                }//cout_<<endl;
            }
            //cout_<<"-";
        }     
        
        
        //cout_<<"draw done";
    }//___________________________________________________________________________
    
    //рисование одного битмапа внутри другого по заданным координатам и заданным режимом сложения значений пикселов 
    
    void GBitmap::drawImg(GBitmap *img,int x0,int y0,int mode){
        int w=img->columns();
        int h=img->rows();
        //cout_<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<" nrows="<<nrows<<endl;
        
        int x,y,d_;
        unsigned char *p; 
        
        
        if(x0>0&&x0+w<=ncolumns&&y0>0&&y0+h<=nrows){  //рисование без проверки
            
            unsigned char *d; // (переменная d, 7 ноября 2010, быстрее классической).
            for (y = 0; y < h; y++){
                d_=(y0+y)*ncolumns+x0;
                d=bytes_data+d_;
                for (x = 0; x < w; x++){
                    p=d+x;
                    if(mode==DRAW_BLACK){
                        if(img[0][y][x]==0)*p=img[0][y][x];
                    }else{
                        *p=img[0][y][x];
                    }  
                }
            }
            //cout_<<"+";
        }else{   //рисование с проверкой
            for (y = 0; y < nrows; y++){
                for (x = 0; x < ncolumns; x++){
                    if(y-y0<0||y-y0>h||x-x0<0||x-x0>w)continue;
                    d_=y*ncolumns+x;
                    p=bytes_data+d_;
                    if(mode==DRAW_BLACK){
                        if(img[0][y-y0][x-x0]==0)*p=img[0][y-y0][x-x0];
                    }else{
                        *p=img[0][y-y0][x-x0];
                    }    
                }
            }
            //cout_<<"-";
        }     
        //cout_<<"draw done";
        
    }//____________________________________________________________________________
    
    
    void GBitmap::clearRegion(int x0,int y0,int w,int h){
        //cout_<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<" nrows="<<nrows<<endl;
        
        
        if(x0<0){
            w+=x0;  
            x0=0;  
        }
        if(x0>ncolumns)return;
        if(y0<0){
            h+=y0;
            y0=0;  
        }
        if(y0>nrows)return;
        
        if(x0+w>ncolumns)w=ncolumns-x0;
        if(y0+h>nrows)h=nrows-y0;

        if(w>ncolumns)w=ncolumns;
        if(h>nrows)h=nrows;
        
                
        int x,y,d_;
        unsigned char *p,*d; // (переменная d, 7 ноября 2010, быстрее классической).
        
        for (y = 0; y < h; y++){
            d_=(y0+y)*ncolumns+x0;
            if(d_>data_size||d_<0)continue;
            d=bytes_data+d_;
            for (x = 0; x < w; x++){
                p=d+x;
                *p=255;
            }
        }
        //cout_<<"draw done";
        
    }//____________________________________________________________________________
    
    
    // функция нормализации массива bytes_data по размеру ncolumns кратным 32    // (29 ноября 2012)
    
    void GBitmap::crop32(){
        
        // получает новый массив bytes_data с новым размером по горизонтали ncolumns кратным 32 и новым data_size=ncolumns*nrows;
        // массив с размером ncolumns кратным 32 необходим для работы некоторых быстродействующих программ типа dotGain()
        
        //  * Особенности *
        // С правой стороны текста, в пределах 32 пикселей от края добавляется черный случайный шум, который в дальнейшем 
        // легко убирается, например бордюром. Если ncolumns является кратный 32, то ничего подобного не происходит.
        
        
        /// cout<<"Нормализация массива bytes_data по размеру ncolumns кратным 32     crop32()"<<endl;
        
        int newW=(ncolumns/32)*32; // новый ncolumns кратный 32
        // если ncolumns кратно 32 то сразу выходим из функции
        if(newW==ncolumns)return;
        
        ///TIME_START  // w=16224 × h=2464  time=0.17 (пересчитано т.к. 16224/32=507 )
        newW+=32;
        unsigned char *p,*p1; 
        uchar *newData=(uchar*)malloc(newW*nrows);
        // Заполнение белым (255-белый), уменьшает черную полосу справа 
        memset(newData, 255, newW*nrows);  // начало (указатель), значение, количество символов в байтах (длинна) unsigned 
        
        
        for (int y=0; y < nrows; y++){
            p=bytes_data + y*ncolumns;            
            p1=newData + y*newW;
            for (int x=0; x < ncolumns; x++){                          
                *(p1+x)=*(p+x);  //*p1++=*p++;  
            } // x 
        } // y
        /**/
        
        /*
         unsigned char *d,*d1;
         p=bytes_data;  p1=newData;
         for (int y=0; y < nrows; y++){
         d=p+=ncolumns;   /// d=p=(p+ncolumns);   // d=p;
         d1=p1+=newW;     /// d1=p1=(p1+newW);    // d1=p1;
         for (int x=0; x < ncolumns; x++){               
         *d1++=*d++;
         } // x     
         } // y
         */
        
        free(bytes_data);
        // создание нового массива bytes_data с новым размером
        bytes_data=newData;
        ncolumns=newW;
        data_size=ncolumns*nrows;
        ///TIME_PRINT_
        
    }; //____________________________________________________________________________
    
    
    // вывод на экран псевдографики массива битмап bytes_data
    void GBitmap::printToScreen(){
        int w=ncolumns;
        int h=nrows;
        cout<<"w="<<w<<" h="<<h<<endl;
        int x,y;
        unsigned char *p,*d; // (переменная d, 7 ноября 2010, быстрее классической).
        
        for (y = 0; y < h; y++){
            d=bytes_data+y*ncolumns;
            for (x = 0; x < w; x++){
                p=d+x;
                printf("%02x", *p);
                //if(*p>0){cout_<<"1";}else{cout_<<".";}
            }cout<<"<br>"<<endl;
        }
        
    }//____________________________________________________________________________
    
    // вывод на экран псевдографики массива битмап bytes_data
    
    void GBitmap::printToScreenBinary(){
        int w=ncolumns;
        int h=nrows;
        
        int x,y;
        unsigned char *p,*d; // (переменная d, 7 ноября 2010, быстрее классической).
        
        for (y = 0; y < h; y++){
            d=bytes_data+y*ncolumns;
            for (x = 0; x < w; x++){
                p=d+x;
                if(*p>0){
                    cout_<<"1";
                }else{
                    cout_<<"0";
                }
                
                //if(*p>0){cout_<<"1";}else{cout_<<".";}
            }cout_<<endl;
        }
        
    }//____________________________________________________________________________
    
    
    // инверсия входного массива битмап bytes_data с уровнями 0-255.
    
    void GBitmap::invert(){
        
        
        //DM(END<<" TIME_START invert "<<END);
        //TIME_START
        
		//Новая версия. Инверсия входного массива битмап.
		// (изменено 7 ноября 2010, в 9 раз быстрее классической).
        //*
		// приведенный к unsigned int исходный распакованный массив черно-белого скана
		unsigned int* bytes_data_32=(unsigned int*)bytes_data;
        
		// инвертируем одновременно все 4 байта 32х разрядного слова. 255 11111111
		// 255,255,555,255 0xFFFFFFFF    0,0,0,0 0x00000000
		unsigned int b_data=data_size/4;
		for ( int x=0; x < b_data; x++ ) {
            bytes_data_32[x]=~bytes_data_32[x]; //>>7 & 0x1010101 ^ invert
		} // x                                 // ^ исключающее ИЛИ
        //*/
        
        /*
         //Старая версия. Инверсия входного массива битмап.
         int w=ncolumns;
         int h=nrows;
         int x,y;
         unsigned char *p;
         for (y = 0; y < h;y++){
         for (x = 0; x < w; x++){
         p=bytes_data+y*ncolumns+x;
         *p=255-*p;
         //if(*p>0){cout_<<"1";}else{cout_<<".";}
         }//cout_<<endl;
         }
         //*/
        //TIME_PRINT_
        
    }//____________________________________________________________________________
    
    
    void GBitmap::rotate(int rotation){
        
        
    }//____________________________________________________________________________
    
    void GBitmap::eraseBorder(uint dX,uint dY){
        if(dX>ncolumns/2)return;
        if(dY>nrows/2)return;
        
        //стираем верхний бордюр
        memset(bytes_data,255,ncolumns*dY);
        //стираем нижний бордюр
        memset(bytes_data+(ncolumns*nrows-ncolumns*dY),255,ncolumns*dY);
        //стираем боковые бордюры
        int dX1=ncolumns-dX;
        dX*=2;
        int nrows_=nrows-dY;
        for(int i=dY;i<nrows_;i++){
            memset(bytes_data+ncolumns*i+dX1,255,dX);
        }
        
    }//____________________________________________________________________________
    
	// ГОТОВАЯ НОВАЯ fast version
	
	//***********************Adaptive Threshold************************************
	// функция преобразования серого массива в битовый массив (адаптивный порог)
    
	// Авторы = "DerekBradley and GerhardRoth".
	// www.derekbradley.ca / AdaptiveThresholding / index.html.
	
	void  GBitmap::binarisation(int Tr, int BASE) {
		//***********************Adaptive Threshold************************************
        
		// BASE средний размер буквы или строки по всей странице. Напрямер Tenjur BASE=46pix.
		// скользящия область S*S вычисления значения средней яркости  в 3,5
		// больше размера буквы или строки S=BASE*3.5
		
		// Tr - устанавливает порог как число процентов, от вычисленного
		// адаптивного порога в виде значения средней яркости по области S*S.
		// Если значение яркости текущего пикселя на Tr процентов ниже, чем в среднея
		// яркость по области S*S то он установлевается в черное,
		// в противном случае он установлевается в белое. Обычно Tr=80.
		// Tr меньше - более тонкие линии (штрихи), Tr больше - более толстые линии.
        
        if(Tr==0 || BASE==0)return;   
		
		// Check arguments and init variables
		short w=ncolumns;   // ширина массива   IMAGE_WIDTH
		short h=nrows;      // высота массива   IMAGE_HEIGHT
		
		unsigned int* integralImg;
		unsigned int *d1;
		unsigned char* input=bytes_data;
		unsigned char *d0, *p0;
		
		unsigned int sum=0;
		unsigned int count=0;
		int x, y, x1, y1, x2, y2;
		// вспомогательные переменные для увеличения быстродействия
		unsigned int *p1, *p2;
		
		//cout_<<END<<"АdaptiveThreshold(), New version V0"<<endl;
        /// cout<<"Адаптивный порог binarisation()   Tr="<<Tr<<"  BASE="<<BASE<<endl;
		
		
		///TIME_START  //  time=0.297  w=2698 h= 4000
		
		
		integralImg = (unsigned int*)malloc(w*h*sizeof(unsigned int*));
		
		// выход в случае отсутствия свободной памяти запрашиваемого размера
		if (integralImg==NULL) {DC("integralImg"); return;}
		
		// обнуление массива new_data
		//memset(integralImg,0,w*h*sizeof(unsigned int*));
		
		
		// create the integral image
		// интегральная яркость integrated brightness
		
		for (y=0; y<h; y++)
		{
			// reset this column sum
			sum = 0;
			// вспомогательные переменные для увеличения быстродействия
			p0 = input + y*w;
			p1 = integralImg + y*w;
			
			for (x=0; x<w; x++)
			{
				d1 = p1 + x;             // d0 = (input + y*w + x);
				
				// подсчет сумм яркостей для обработки скользящего суммирующего квадрата S*S
				sum += *(p0 + x);        // sum += input[index];
				
				if ( y==0 )              // d0=(input + index);
					*d1 = sum;           // integralImg[index] = sum;
				else
					*d1 = *(d1-w) + sum; // integralImg[index]=integralImg[index-1] + sum;
			} //x
		} //y
		/* */
		
		// TIME_PRINT
		
		
		// ограничения на процент от порога  Tr
		if (Tr < 16) Tr = 16;   if (Tr > 99) Tr = 99;
		
		// позволяет использовать сдвиг <<7 вместо деления в цикле perform thresholding
		Tr=(Tr<<7)/100 + 1 ; //     // 80*128/100;   (80<<7)/100;
		
		// ограничения на размер буквы BASE в пикселях, где
		// BASE средний размер буквы или строки по всей странице, в Tenjur=46
		if (BASE < 4) BASE = 4;   if (BASE > 160) BASE = 160;
		int S=BASE*35/10;  // S в 3,5 больше размера буквы или строки  S=46 *3,5=160
		int s2=S/2;
		
		// вспомогательные переменные, для увеличения быстродействия
		int h_1=h-1;
		int w_1=w-1;
		int y2_y1;
		
		
		
		// perform thresholding
		
		for (y=0; y<h; y++){
			
			// set the SxS region
			y1 = y - s2;  y2 = y + s2;
			
			// check the border
			if (y1 < 0)   y1 = 0;
			if (y2 > h_1) y2 = h_1;         //   if (y2 >=h) y2 = h-1;
			
			// вспомогательные переменные, для увеличения быстродействия
			p0 = input + y*w;
			p1 = integralImg + y1*w;
			p2 = integralImg + y2*w;
			y2_y1 = y2 - y1;
			
			for (x=0; x<w; x++){
				
				// set the SxS region
				x1 = x - s2;  x2 = x + s2;
				
				// check the border
				if (x1 < 0)   x1 = 0;
				if (x2 > w_1) x2 = w_1;     //   if (x2 >=w) x2 = w-1;
				
				// площадь суммирующего квадрата S*S с учетом border
				count = y2_y1*(x2-x1);      // count = countX*(y2-y1);
				
				// сумма серого папавшая в площадь квадрата S*S
				// I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
				// sum = integralImg[y2*w + x2] - integralImg[y1*w + x2]
				//	    -integralImg[y2*w + x1] + integralImg[y1*w + x1];
				sum = *(p2 + x2) - *(p1 + x2) - *(p2 + x1) + *(p1 + x1);
				
				d0 = p0 + x;
				/// Бинаризация///
				// если яркость (0-255) пиксела больше средней по площади
				// яркости скользящего квадрата S*S,то считаем пиксель белым.
				// if (input[index]*count > (sum*Tr)>>7)  {input[index]=255;}
				if ( *d0*count > ((sum*Tr)>>7) )  { *d0=255; }  // 255 белый
				else { *d0=0; }   //else { input[index]=0; }    //  /128
				
			} // x
			
		}  // y
		/**/
		
		
		// освобождение массива integralImg
		if ( integralImg !=NULL ) free(integralImg);
        
		
		// cout_<<"w="<<w<<endl;    cout_<<"h="<<h<<endl;
		// cout_<<"Tr="<<Tr<<endl;
		// cout_<<"(Tr*100)>>7="<<((Tr*100)>>7)<<endl;
		// cout_<<"S="<<S<<endl;
		// cout_<<"sizeof(int)="<<sizeof(int)<<endl;
        
        
        //cout<<endl<<"adaptiveThreshold   ";
        ///TIME_PRINT_
        
	}//_____________________________________________________________________________
    
    
	// Упаковка большого битмапа в маленькй битовый массив.
	// Используется для записи в формате TIFF.
	// есть 32х битная реализация packImg32
    
	void GBitmap::packImg(int invert){
        
		if(pack_flag)return;
        
        int y,x,d;
        unsigned int s;
        unsigned int data_size_p=data_size/8;
        // data_size=ncolumns*nrows;  размер массива буквы (букв)
        // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв)
        
        // инициализация битового массива char размером  data_size_p=data_siz/8
        bites_data=(unsigned char*)malloc(data_size_p);
        // обнуление bites_data
        memset(bites_data,0,data_size_p);
        // выход в случае отсутствия свободной памяти запрашиваемого размера
        //if (bites_data==NULL) {DC("bites_data"); exit (1);}
        
        //автоопределение количества градаций в битмапе
        int max=0;
        for ( x=0; x < data_size; x++ ) {
            s=bytes_data[x];
            if ( s > max ) max=s;
        }
        
        // бинаризация входного массива битмап
        if(max>1){
            for ( x=0; x < data_size; x++ ) {
                if(invert){
                    if ( bytes_data[x]>127 ){ bytes_data[x]=0; } // уровни в битмапе 0 и 255
                    else { bytes_data[x]=1; }  // уровни в битовом массиве 1 и 0
                }else{
                    if ( bytes_data[x]>127 ){ bytes_data[x]=1; } // уровни в битмапе 0 и 255
                    else { bytes_data[x]=0; }  // уровни в битовом массиве 1 и 0
                }
                
            }
        }
        /**/
        
        
        // ЗАПАКОВКА
        // Запаковка стандартного большого байтового массива битмап
        // (в одном байте 1 бит) в маленькй битовый массив (в одном байте 8 бит).
        unsigned int reg=0;
        for( y=0; y < data_size_p; y++ ){  // >>3  деление на 8
            d=y*8;                          // <<3  умножение на 2 в 3 степени = 8
            for ( x=0; x < 8; x++ ) {
                reg=(reg << 1) + bytes_data[d+x]; // обратный порядок пикселей
                ///					 s=(bytes_data[d + x])<<7; // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
                ///					 reg=s + (reg >> 1) ;               // прямой порядок пикселей
            } // x
            bites_data[y]=reg;  // заполнение маленького выходной битового массива char
        } // y
        /**/
        
        free(bytes_data);  // освобождение большого массива битмап
        
        // 1 запакован, каждый бит всех байтов маленького (char) массива заполнен битами
        pack_flag=1;
        
    }//_____________________________________________________________________________
    
    // Распаковка маленького битового массива в большй битмап.
    // Используется для увеличения скорости доступа при обработке картинок букв.
    // есть 32х битная реализация unpackImg32
    
    void GBitmap::unpackImg(int invert){
        //cout_<<"pack_flag"<<pack_flag<<" data_size="<<data_size<<endl;
        
        if(!pack_flag)return;
        if(pack_flag==BITMAP_32){unpackImg32V(0); return;}
        
        if(TIFF_string.size()){
            //readTIFFFromString(TIFF_string,0, &bites_data, &ncolumns, &nrows);
            data_size=ncolumns*nrows;
            w=ncolumns; h=nrows; invert=1;
            //cout_<<"ncolumns="<<ncolumns<<" nrows="<<nrows<<endl;
        }
        int y,x,d;
        unsigned int s;
        unsigned int data_size_p=data_size/8;
        
        // инициализация большого массива (битмап) bytes_data  полного размера data_siz
        bytes_data=(unsigned char*)malloc(data_size);
        
        // РАСПАКОВКА
        // Распаковка маленького битового массива (в одном байте 8 бит) в
        // большй стандартный байтовый массив битмап (в одном байте 1 бит).
        for( y=0; y < data_size_p; y++ ){  // >>3  деление на 8
            s=bites_data[y];   d=y*8;
            for( x=0; x < 8; x++ ){
                //(bytes_data[d + x])=s & 1;     s=(s >> 1); // прямой порядок пикселей
                if(invert){
                    bytes_data[d + 8-x-1]=255-255*(s & 1); s=(s >> 1); // обратный порядок пикселей
                }else{
                    bytes_data[d + 8-x-1]=255*(s & 1); s=(s >> 1); // обратный порядок пикселей
                }
                // уровни в битмапе  0 и 255
            } // x
        } // y
        
        free(bites_data);  // освобождение маленького массива
        
        // распакован, один бит каждого байта большого массива (битмап) заполнен битами
        pack_flag=0;
        
    }//_____________________________________________________________________________
    
    
    void GBitmap::put(int x,int y,int color){
        if(colorMode==4){
            int *p=(int*)(bytes_data+(x+y*ncolumns)*4);
            *p=color;
        }
        if(colorMode==1){
            bytes_data[x+y*nrows]=color;
        }
    };
    
    int GBitmap::get(int x,int y){
        if(colorMode==4){
            int *p=(int*)(bytes_data+(x+y*ncolumns)*4);
            return *p;
        }
        if(colorMode==1){
            unsigned char *p=(unsigned char*)(bytes_data+x+y*ncolumns);
            return *p;
        }
        return 0;
        
    };
    
    
    // использует packImg  unpackImg, а не быстрые packImg32  unpackImg32
    string GBitmap::drawToTIFFString(int invert){
        
        int print=1;
        DT("draw TIFF_string_size="<<TIFF_string.size()<<" pack_flag="<<pack_flag<<endl);
        string data;
        if(TIFF_string.size()){
            return TIFF_string;
        }else if(data_size){
            DT(".1");
            if(!pack_flag){
                packImg(1);
            }
            DT(".2");
            //TIFF_string=dataToTIFF(bites_data,data_size/8,ncolumns, nrows);
        }
        DT(".3");
        unpackImg(0);
        return TIFF_string;
        
    };
    
    
    /** Save GBitmap as TIFF file with GroupFax4 compression*/
    void GBitmap::saveTIFF(string &path){
        string str=drawToTIFFString(0);
        writeText(str,path);
    }
	
    /** Save GBitmap as TIFF file with GroupFax4 compression*/
    void GBitmap::saveTIFF(const char* path){
        string str=drawToTIFFString(0);
        string file=path;
        writeText(str,file);
    }	
    
    //_____________________________________________________________________________
	
    /**  Убирает белый фон вокруг изображения */
    GBitmap* GBitmap::detectRegion(int frame, int *x0,int *x1,int *y0,int *y1,int mode){
        
        *x0=-1,*y0=-1,*x1=-1,*y1=-1;
        
        for(int x=0;x<ncolumns;x++){
            for(int y=0;y<nrows;y++){
                if(*(bytes_data+y*ncolumns+x)<128){*x0=x;break;}
            }
            if(*x0>0)break;
        }
        
        for(int x=ncolumns-1;x>-1;x--){
            for(int y=0;y<nrows;y++){
                if(*(bytes_data+y*ncolumns+x)<128){*x1=x;break;}
            }
            if(*x1>0)break;
        }
        
        for(int y=0;y<nrows;y++){
            for(int x=0;x<ncolumns;x++){
                if(*(bytes_data+y*ncolumns+x)<128){*y0=y;break;}
            }
            if(*y0>0)break;
        }
        
        for(int y=nrows-1;y>-1;y--){
            for(int x=0;x<ncolumns;x++){
                if(*(bytes_data+y*ncolumns+x)<128){*y1=y;break;}
            }
            if(*y1>0)break;
        }
        
        *x0-=frame+1; //if(*x0<0)*x0=0;
        *y0-=frame+1; //if(*y0<0)*y0=0;
        *x1+=frame+1; //if(*x1>=ncolumns)*x1=ncolumns-1;
        *y1+=frame+1; //if(*y1>=nrows)*y1=nrows-1;	
        
        if(mode){
            GBitmap *img=GBitmap::createRegion(this, *x0, *y0, *x1-*x0, *y1-*y0);
            return img;
        }else{
            return NULL;
        }    
        
    }//_____________________________________________________________________________
    
    // ПОЛИГРАФИЧЕСКОЕ РАСТИСКИВАНИЕ //
    //( изменено: 25 октября 2010, 13 декабря 2012, ).//
    
    /**\brief  Имитация полиграфического растискивания и зашумленности 
     
     НА ВХОДЕ В ФУНКЦИЮ NCOLUMNS ДОЛЖЕН БЫТЬ КРАТНЕН 32
     */
    void GBitmap::dotGain(int gRateLefts, int gRateRight,
                          int gRateHighs, int gRateLower,  int dot_gain){
        
        /**\ dot_gain!=0 число добавленных (-dot_gain убавленных) однопиксельных слоёв вокруг символа */ 
        
        /**\ Подпрограмма представляет собой пространственный фильтр с импульсной
         характеристикой (реакция пространственного фильтра на один pix) в виде квадрата
         размером gainRate X gainRate */
        
        /**\ При инициализации массива bytes_data необходимо обязательно выровнить по 32
         nrows и ncolumns,   nrows = (arows/32+1)*32;   ncolumns =(acolumns/32+1)*32;  */
        
        /**\ Два способа задания полиграфического растискивания:
         - с помощью dot_gain!=0  ( например dot_gain=3; ). Добавляет dot_gain однопиксельных слоёв вокруг символов (-dot_gain убавлет) и
         блокирует использование gRateLefts, gRateRight, gRateHighs, gRateLower.
         dot_gain=0 блокирует свое собственное влияние и можно использовать gRateLefts, gRateRight, gRateHighs, gRateLower.  
         - классический способ с помощью индивидуальной установки gRateLefts, gRateRight, gRateHighs, gRateLower. 
         Для этого устанавливаем dot_gain=0.
         
         Например, типичный способ получения полиграфического растискивания двумя запусками dotGain
         убираем со всех сторон по пикселю, сьедаем мелкие детали, которые не надо растискивать.
         первый способ:  dotGain(0,0,0,0,-2);      второй способ:  dotGain(-1,-1,-1,-1, 0);
         добавляем со всех сторон символа по 2 пикселя (нескольку пикселей)
         первый способ:  dotGain(0,0,0,0, 4);      второй способ:  dotGain(2, 2, 2, 2, 0);
         
         Количество добавленных однопиксельных слоёв вокруг символа от 1 до 31, убавленных от -1 до -31, без изменений 0.*/ 
        
        
        if( dot_gain==0 && gRateLefts==0 && gRateRight==0 && gRateHighs==0 && gRateLower==0 ) return;
        
        int x;
        unsigned int s0,s1;
        // bytes_data_buf  ///<промежуточный буфер обработки битмэпа
        
        
        // dot_gain число добавленных (-dot_gain убавленных) однопиксельных слоёв вокруг символа.    
        // пересчет dot_gain на на левую, правую стороны и верх, низ символа.
        // dot_gain=0 блокирует свое влияние и можно использовать gRateLefts, gRateRight, gRateHighs, gRateLower.
        if (dot_gain) {  
            // ограничение на число добавленных (убавленных) однопиксельных слоёв
            if(dot_gain>63) dot_gain=63;   if(dot_gain<-63) dot_gain=-63; 
            int dg=dot_gain/2;         ///cout<<"dot_gain="<<dot_gain<<"   dg="<<dg;  // division
            gRateLefts=dg;  gRateRight=dg;  gRateHighs=dg;  gRateLower=dg;
            dg=dot_gain%2;             ///cout<<"    dg="<<dg<<endl;                  // modulo
            gRateLefts+=dg;  gRateHighs+=dg;
        }   
        
        
        
        /// gRateLefts=2;  gRateRight=2;  gRateHighs=2;  gRateLower=2; // ТЕСТ
        
        //// ТЕСТ. Нормализациz массива bytes_data по размеру ncolumns кратным 32
        //// crop32();
        
        // количество добавленных однопиксельных слоёв с левой стороны символа
        if(gRateLefts>31) gRateLefts=31;   if(gRateLefts<-31) gRateLefts=-31;
        // количество добавленных однопиксельных слоёв с правой стороны символа
        if(gRateRight>31) gRateRight=31;   if(gRateRight<-31) gRateRight=-31;
        // количество добавленных однопиксельных слоёв с верхней стороны символа
        if(gRateHighs>31) gRateHighs=31;   if(gRateHighs<-31) gRateHighs=-31;
        // количество добавленных однопиксельных слоёв с нижней стороны символа
        if(gRateLower>31) gRateLower=31;   if(gRateLower<-31) gRateLower=-31;
        /**/
        
        //cout<<"Полиграфическое растискивание dotGain()  g1, g2, g3, g4,  dot_gain="
        //<<gRateLefts<<gRateRight<<gRateHighs<<gRateLower<<dot_gain<<endl;
        
        ///TIME_START
        
        
        /**/
        //------------------------------------------------------------------------------
        
        // УПАКОВКА  PACKING // 
        // time=0.017 здесь и далее w=4832,  h=828    w*h=4 млн. пикселей
        // вызов функции упаковки большого битмапа в маленькй int массив.
        // Массив bites_data из 32х разрядных слов упакован битово.
        // pack_flag32=0; // обнулили в функции GBitmap::init(int acolumns, int arows)
        // invert=0 без инверсии черного и белого, invert=1 с инверсией черного и белого.
        packImg32(0);
        
        // приведенный к unsigned int исходный упакованный массив черно-белого скана
        unsigned int* bites_data_32=(unsigned int*)bites_data;
        
        /**/
        //------------------------------------------------------------------------------
        
        // СДВИГИ SHIFTS //
        // сдвиг влево, вправо, вверх, вниз всего массива data_size_p32, как единого регистра
        
        int dl=1;  int dm=32-dl;  // dl - толщина одного слоя в pix по горизонтали
        int d0=dl*ncolumns/32;    // d0 - толщина одного слоя в pix по вертикали
        
        int dltLefts=abs(gRateLefts);  int dltRight=abs(gRateRight);
        int dltHighs=abs(gRateHighs);  int dltLower=abs(gRateLower);
        
        // нахождение максимума gainRate из dltLefts, dltRight, dltHighs, dltLower
        int gainRate=0; // количество однопиксельных слоёв
        if( dltLefts>gainRate ) { gainRate=dltLefts; }
        if( dltRight>dltLefts ) { gainRate=dltRight; }
        if( dltHighs>dltRight ) { gainRate=dltHighs; }
        if( dltLower>dltHighs ) { gainRate=dltLower; }
        DM(" gainRate "<<gainRate);
        
        //------------------------------------------------------------------------------
        
        // Цикл по количеству однопиксельных слоёв.  // time=0.0026
        int m;
        for ( m=0;  m < gainRate;  m++ ) {
            
            // СДВИГ ВЛЕВО всего массива bites_data_32, как единого регистра dltLefts раз
            // на величину dl, для прямого порядока пикселей.
            
            // Добавление пикселей (пикселя) с левой стороны символа.
            if(gRateLefts>0 && m<dltLefts) {
                s0=bites_data_32[0];  // input
                for ( x=0;  x < data_size_p32-1;  x++ ) { // ncolumns*nrows/32;
                    s1=bites_data_32[x+1];
                    // сдвиг двух 32р регистров s0 и s1 влево, как одного 64р регистра
                    bites_data_32[x]=s0&(s0>>dl|s1<<dm);   s0=s1;
                } // x
            } // if
            
            // убавление пикселей (пикселя) с левой стороны символа.
            if(gRateLefts<0 && m<dltLefts) {
                s0=bites_data_32[0];  // input
                for ( x=0;  x < data_size_p32-1;  x++ ) { // ncolumns*nrows/32;
                    s1=bites_data_32[x+1];
                    // сдвиг двух 32р регистров s0 и s1 влево, как одного 64р регистра
                    bites_data_32[x]=s0|(s0>>dl|s1<<dm);   s0=s1;
                } // x
            } // if
            
            //------------------------------------------------------------------------------
            
            // СДВИГ ВПРАВО всего массива bites_data_32, как единого регистра dltRight раз
            // на величину dl, для прямого порядока пикселей.
            
            // Добавление пикселей (пикселя) справой стороны символа.
            if(gRateRight>0 && m<dltRight) {
                s0=bites_data_32[data_size_p32-1];
                for ( x=data_size_p32-1;  x > 0 ;  x-- ) {  //  x=data_size_p32-1;  x > -1+1
                    s1=bites_data_32[x-1];
                    // сдвиг двух 32 разрядных регистров s0 и s1 вправо, как одного 64 разрядного регистра
                    bites_data_32[x]=s0&(s0<<dl|s1>>dm);   s0=s1;
                } // x      // уровни в битмапе  0-чёрный и 1-белый
            } // if
            
            // Убавление пикселей (пикселя) справой стороны символа.
            if(gRateRight<0 && m<dltRight) {
                s0=bites_data_32[data_size_p32-1];
                for ( x=data_size_p32-1;  x > 0 ;  x-- ) {  //  x=data_size_p32-1;  x > -1+1
                    s1=bites_data_32[x-1];
                    // сдвиг двух 32 разрядных регистров s0 и s1 вправо, как одного 64 разрядного регистра
                    bites_data_32[x]=s0|(s0<<dl|s1>>dm);   s0=s1;
                } // x      // уровни в битмапе  0-чёрный и 1-белый
            } // if
            
            //------------------------------------------------------------------------------
            
            
            // СДВИГ ВВЕРХ  всего массива bites_data_32, как единого регистра на d0 строк
            
            // Добавление пикселей с верхней стороны символа.
            if(gRateHighs>0 && m<dltHighs) { // количество добавленных однопиксельных слоёв с верхней стороны символа
                for ( x=0;  x < data_size_p32-d0;  x++ ) {
                    bites_data_32[x]=bites_data_32[x] & bites_data_32[x+d0];
                } // x
            } // if
            
            // Убавление пикселей с верхней стороны символа.
            if(gRateHighs<0 && m<dltHighs) { // количество убавленных однопиксельных слоёв с верхней стороны символа
                for ( x=0;  x < data_size_p32-d0;  x++ ) {
                    bites_data_32[x]=bites_data_32[x] | bites_data_32[x+d0];
                } // x
            } // if
            
            //------------------------------------------------------------------------------
            
            // СДВИГ ВНИЗ всего массива bites_data_32, как единого регистра на d0 строк
            // int dirRev=~0;  // ^ dirRev)
            // int dirRev=0;
            // Добавление пикселей с нижней стороны символа.
            if(gRateLower>0 && m<dltLower) { // количество добавленных однопиксельных слоёв с нижней стороны символа
                for ( x=data_size_p32-1;  x > d0;  x-- ) {
                    bites_data_32[x]=bites_data_32[x] & bites_data_32[x-d0];
                } // x
            } // if
            
            // Убавление пикселей с нижней стороны символа.
            if(gRateLower<0 && m<dltLower) { // количество убавленных однопиксельных слоёв с нижней стороны символа
                for ( x=data_size_p32-1;  x > d0;  x-- ) {   // d0
                    bites_data_32[x]=bites_data_32[x] | bites_data_32[x-d0];
                } // x
            } // if
            
            /**/
        } // m
        
        //------------------------------------------------------------------------------
        
        
        
        // РАСПАКОВКА UNPACKING 32 //     time=0.01
        // вызов функции распаковки маленького битового массива int в большй битмап.
        // invert=0 без инверсии черного и белого, invert=1 с инверсией черного и белого.
        unpackImg32(0);
        //invert();
        
        
        //******************************************************************************
        
        // работающая исходная версия
        // сдвиг влево, вправо всего массива data_size_p32, как единого регистра
        /*
         dl=1;  // 31
         if(dl<1 || dl>31) return;
         dm=32-dl;
         unsigned int data_size_p32_1=data_size_p32-1;
         */
        /*
         // сдвиг влево всего массива data_size_p32, как единого регистра
         // на величину dl, для прямого порядока пикселей
         s0=bites_data_32[0];
         for ( x=0;  x < data_size_p32_1;  x++ ) {   // ncolumns*nrows/32;
         s1=bites_data_32[x+1];
         // беззнаковый сдвиг двух 32 разрядных регистров s0 и s1 влево,
         // как одного 64 разрядного регистра, для прямого порядока пикселей
         ///bites_data_32[x]=y1=s1<<dl|s0>>dm;    // y0=s0<<dl;
         bites_data_32[x]=s0>>dl|s1<<dm;    s0=s1;
         } // x
         */
        /*
         // сдвиг вправо всего массива data_size_p32, как единого регистра
         // на величину dl, для прямого порядока пикселей
         s0=bites_data_32[data_size_p32-1];
         for ( x=data_size_p32-1;  x > 0 ;  x-- ) {  //  x=data_size_p32-1;  x > -1+1
         s1=bites_data_32[x-1];
         // беззнаковый сдвиг двух 32 разрядных регистров s0 и s1 вправо,
         // как одного 64 разрядного регистра, для прямого порядока пикселей
         bites_data_32[x]=s0<<dl|s1>>dm;    s0=s1;
         } // x
         */
        //******************************************************************************
        
        DM(" ncolumns "<<ncolumns<<" nrows "<<nrows<<END); // DM(" m "<<m<<END);
        
        DM(" gRateLefts "<<gRateLefts<<" gRateRight "<<gRateRight
           <<" gRateHighs "<<gRateHighs<<" gRateLower "<<gRateLower<<END);
        
        DM(" TIME_PRINT dotGain /GGG/ ");
        
        
        ///TIME_PRINT_ // DM(END);
        
        /**/	// y0=~0;   y1=~0;     // ^ |
        
        // END УПАКОВКА  PACKING //    
        
    }//_____________________________________________________________________________
    
    // УПАКОВКА  PACKING 32 //
	/**\ Упаковка большого битмапа в маленькй int массив. Массив bites_data
     из 32х разрядных слов упакован битово.
     Используется для функции имитация полиграфического растискивания и зашумленности.*/
    
	// При переносе в основную версию использовать эту функцию packImg32Б вместо
	// функции packImg.   САА 3 ноября 2010 года.
    
    void GBitmap::packImg32(int invert){
        
        if(pack_flag||!data_size)return;
        
        int y,x,d;
        int print=0;
        
        // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв)
        // data_size=ncolumns*nrows;  размер большого битмапа
        
        
        DR(END<<" TIME_START packImg32 /P32/ 0="<<0<<" ncolumns="<<ncolumns<<endl)
        
        data_size_p=data_size/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
        data_size_p32=data_size/32; /// размер упакованного массива integer. 1 integer - 32 пиксела изображения    
        
        // инициализация битового массива char размером  data_size_p=data_siz/8
        bites_data=(unsigned char*)malloc(data_size_p+32);         // +ncolumns  // было +8 это ошибка
        // обнуление bites_data                                   // +ncolumns
        memset(bites_data,0,data_size_p+32);
        // выход в случае отсутствия свободной памяти запрашиваемого размера
        //if (bites_data==NULL) {DC("bites_data"); exit (1);}
        
        // приведенный к unsigned int исходный упакованный массив черно-белого скана
        unsigned int* bites_data_32=(unsigned int*)bites_data;
        /**/
        
        // автоопределение количества градаций в битмапе с управляемой инверсией.
        //  invert=0;  без инверсии (обычно белый фон)
        //  invert=1;  с инверсией (обычно черный фон)
        AutodetectGradations(invert);
        
        // Классический вариант //
        // Запаковка стандартного большого байтового массива битмап
        // (в одном байте 1 бит) в маленькй  массив int (в одном int 32 бита).
        
        unsigned int reg;  // unsigned int dirRev=31; // direct, reverse. dir_rev
        for( y=0; y < data_size_p32; y++ ){ // >>5  деление на 5  data_size/32
            d=y*32;  reg=0;             // <<5  умножение на 2 в 5 степени = 32
            for ( x=0; x < 32; x++ ) {
                ///	 ///  		 reg=(reg << 1) + bytes_data[d+x]; // обратный порядок пикселей ///////////
                //reg=bytes_data[d+x] + (reg << 1); // обратный порядок пикселей
                reg=(bytes_data[d + x]<<31) + (reg >> 1); // прямой порядок пикселей ///////////
            } // x      // <<31 умножение на 2 в 31 степени,  >> 1 деление на 2
			bites_data_32[y]=reg;  // заполнение маленького выходного массива int
        } // y
        
        /**/
        
        free(bytes_data);  // освобождение исходного большого массива битмап
        
        // 1 если массив bites_data из 32х разрядных слов упакован побитово
        pack_flag=BITMAP_32;
        /**/
        
    }//_____________________________________________________________________________
    
    // РАСПАКОВКА UNPACKING 32 //
    /**\ Распаковка маленького битового массива int в большй битмап.
     Используется для функции имитация полиграфического растискивания и зашумленности.*/
    
    // При переносе в основную версию использовать эту функцию unpackImg32 вместо
    // функции unpackImg.   САА 3 ноября 2010 года.
    
    
    void GBitmap::unpackImg32(int invert){
        //cout_<<"pack_flag32"<<pack_flag32<<" data_size="<<data_size<<endl;
        
        if(pack_flag!=BITMAP_32)return;
        
        int y,x,d;
        unsigned int s;
        int print=0;
        
        DR(END<<" TIME_START unpackImg32 /UP32/ 0="<<0<<endl);
        DR("data_size="<<data_size<<" data_size_p32="<<data_size_p32<<" ncolumns="<<ncolumns<<endl)
        
        // инициализация большого массива (битмап) bytes_data полного размера data_siz
        bytes_data=(unsigned char*)malloc(data_size);
        // обнуление bytes_data
        memset(bytes_data,0,data_size);
        // выход в случае отсутствия свободной памяти запрашиваемого размера
        //if (bytes_data==NULL) {DC("bytes_data"); exit (1);}
        
        // приведенный к unsigned int упакованный битовый массив черно-белого скана
        // маленький запакованный массив
        unsigned int* bites_data_32=(unsigned int*)bites_data;
        
        // приведенный к unsigned int распакованный байтовый массив черно-белого скана
        // большой распакованный массив
        unsigned int* bytes_data_32=(unsigned int*)bytes_data;
        /**/
        
        // РАСПАКОВКА
		// Распаковка маленького массива int (в одном int 32 бита) в большй стандартный
		// байтовый массив битмап (в одном байте 1 бит) с управляемой инверсией.
        //*
        // Более быстрый вариант. //
		// (изменено 8 ноября 2010, в 1,3 раза быстрее классической).
		// invert=0; // без инверсии (белый фон), invert=1 с инверсией (обычно черный фон)
		if(invert<1) { invert=0;}   if(invert>0) { invert=~0;}
        
		// По словная запись в массив bytes_data (по 4 байта одновременно)
		unsigned int s0;
		for( y=0; y < data_size_p32; y++ ){  // >>5  деление на 32  // data_size/32
			s=bites_data_32[y];   d=y*8; // 32/4
			for( x=0; x < 8; x++ ){ // 32/4
                
                // прямой порядок пикселей     // bytes_data[d + x]=s & 1;  s=(s >> 1);
                s0 =0x000000FF*(s & 1); s>>=1; // заполнение 00000000 или 11111111 1ого байта 255*
                s0|=0x0000FF00*(s & 1); s>>=1; // заполнение 00000000 или 11111111 2ого байта 255*<<8;
                s0|=0x00FF0000*(s & 1); s>>=1; // заполнение 00000000 или 11111111 3его байта 255*<<16;
                // заполнение 00000000 или 11111111 4ого байта с управляемой инверсией.       255*<<24
                bytes_data_32[d + x]=(0xFF000000*(s & 1) | s0) ^ invert;   s>>=1;
                // запись в массив bytes_data одним 32р словом 4х байт одновременно
                
                // обратный порядок пикселей // bytes_data[d + 32-x-1]=255*(s & 1);  s>>=1;
                //			  s0=255*(s & 1)<<24;   s>>=1; // заполнение 00000000 или 11111111 4ого байта
                //			  s0|=255*(s & 1)<<16;  s>>=1; // заполнение 00000000 или 11111111 3его байта
                //			  s0|=255*(s & 1)<<8;   s>>=1; // заполнение 00000000 или 11111111 2ого байта
                // заполнение 00000000 или 11111111 1ого байта с управляемой инверсией.
                //			  bytes_data_32[d + 8-x-1]=(255*(s & 1) | s0) ^ invert;   s>>=1;
                // запись в массив bytes_data одним 32р словом 4х байт одновременно
                
			} // x     // уровни в битмапе  0-чёрный и 255-белый
		} // y      // s>>=1; деление на 2 с присваиванием
        //*/
        
        
        /*
         // Классический вариант //
         //invert=0; // без инверсии (белый фон), invert=1 с инверсией (обычно черный фон)
         if(invert<1) { invert=0;}   if(invert>0) { invert=1;}
         
         // По байтная запись массив bytes_data. Классический вариант
         for( y=0; y < data_size_p32; y++ ){  // >>5  деление на 32  // data_size/32
         s=bites_data_32[y];   d=y*32;
         for( x=0; x < 32; x++ ){
         //bytes_data[d + x]=s & 1;     s=(s >> 1);        // прямой порядок пикселей
         //bytes_data[d + x]= 255*(s & 1 ^ invert);   s=(s >> 1);  // прямой порядок пикселей ///////////
         bytes_data[d + x]= 255*(s & 1 ^ invert);   s>>=1;  // прямой порядок пикселей ///////////
         //bytes_data[d + 32-x-1]=255*(s & 1); s=(s >> 1); // обратный порядок пикселей       ///////////
         } // x     // уровни в битмапе  0-чёрный и 255-белый
         } // y         // s>>=1; деление на 2 с присваиванием
         //*/
        
        //------------------------------------------------------------------------------
        
        // TEST //
        
        /*            // 255   65280   16711680    4278190080
         binaryPrint(0x000000FF, 2);  DM(" "); DM(END);
         binaryPrint(0x0000FF00, 2);  DM(" "); DM(END);
         binaryPrint(0x00FF0000, 2);  DM(" "); DM(END);
         binaryPrint(0xFF000000, 2);  DM(" "); DM(END);
         
         // прямой порядок пикселей
         //bytes_data[d + x]=s & 1;     s=(s >> 1);        // прямой порядок пикселей
         s0=(s & 1);       s>>=1;  // прямой порядок пикселей ///////////s0=s & 1;   s>>=1;  // прямой порядок пикселей ///////////
         s0|=(s & 1)<<8;   s>>=1;  // прямой порядок пикселей ///////////
         s0|=(s & 1)<<16;  s>>=1;  // прямой порядок пикселей ///////////
         s0|=(s & 1)<<24;  s>>=1;  // 255*
         ///bytes_data_32[d + x]=(255*(s & 1)<<24 | s0) ^ invert;    s>>=1;  // прямой порядок пикселей ///////////
         // одновременное умножение 4х байт на 255 с управляемой инверсией.
         ///bytes_data_32[d + x]=s0 ^ invert; //255*
         bytes_data_32[d + x]=((s0<<8)-s0) ^ invert;   //  +(s0<<7)
         ///bytes_data_32[d + x]=((s0<<8)-s0) ^ invert;   //  +(s0<<7)
         */
        
        /*            // TEST
         invert=~0; // TEST
         invert=0;  // TEST
         s=0xFFFFFFFF-1;
         s=0xFFFFFF01;
         //s=0x00000000;
         //s=0xFFFFFF00;
         unsigned int s0;
         binaryPrint(s, 2);  DM(" "); DM(END);
         //s0 = 255*(s & 1);       s>>=1;  // прямой порядок пикселей ///////////s0=s & 1;   s>>=1;  // прямой порядок пикселей ///////////
         s0 =(s & 1);      s>>=1;
         binaryPrint(s0, 2);  DM(" "); DM(END);
         s0|=(s & 1)<<8;   s>>=1;  // прямой порядок пикселей /////////// 255*
         binaryPrint(s0, 2);  DM(" "); DM(END);
         s0|=(s & 1)<<16;  s>>=1;  // прямой порядок пикселей ///////////  255*
         binaryPrint(s0, 2);  DM(" "); DM(END);
         s0|=(s & 1)<<24;  s>>=1;  // прямой порядок пикселей ///////////   255*
         binaryPrint(s0, 2);  DM(" "); DM(END);
         //s0=s0 ^ invert; //255*
         // одновременное умножение 4х байт на 255 с управляемой инверсией.
         ///s0=((s0<<7)-s0+(s0<<7)) ^ invert; //255*
         s0=((s0<<8)-s0) ^ invert; //255*  +(s0<<7)
         binaryPrint(s0, 2);  DM(" "); DM(END);
         //bytes_data_32[d + x]=s0; //255*   ^ invert
         //*/
        //------------------------------------------------------------------------------
        
        /**/
		free(bites_data);  // освобождение маленького массива
		pack_flag=0;
        
        //		DM(END<<" data_size "<<data_size<<" ncolumns*nrows "<<ncolumns*nrows<<END);
        
    }//_____________________________________________________________________________
 

    
    
    // АВТООПРЕДЕЛЕНИЕ ГРАДАЦИЙ В БИТМАПЕ //
    /**\ автоопределение количества градаций в битмапе с управляемой инверсией.
     Используется для приведения битмапа в состояние: младший бит байта 0-черный, 1-белый.
     Входной битмап м.б. 255-белый (напрямер 255-tif, 208-jpeg), 0-черный.*/
    
    void GBitmap::AutodetectGradations(int invert){
        
        unsigned int x,s;
        
        //  * Особенности *
        // Не работает если в черно-белом битмапе: байт 0-черный, байт 255-белый ( 11111111 ).
        // Ничего не возвращает.
        
        DM(" TIME_START AutodetectGradations   invert="<<invert<<END);
        
        ///TIME_START
        
        // TEST //
        // for ( x=0; x < data_size; x++ ) { bytes_data[x]=1; }    
        // 0xFEFEFEFE=~(1+(1<<8)+(1<<16)+(1<<24));  0xFEFEFEFE=~(16843009=0x1010101)
        // binaryPrint(0xFEFEFEFE, 2);  DM(" "); DM(END);
        
        
        /// быстрое автоопределение и бинаризация входного массива битмап ///
        // (изменино 31 октября 2010, в 6-9 раз быстрее классической).//
        //*
        // приведенный к unsigned int исходный распакованный массив черно-белого скана
        unsigned int* bytes_data_32=(unsigned int*)bytes_data;
        
        // автоопределение количества градаций в битмапе. // time=0.0042    
        // алгоритм //
        // bytes_data[x]=0 или 1, то s всегда =0  битмап уже бинаризован.
        // bytes_data[x]>1,       то s всегда >0  битмап надо бинаризовать.
        // маской убираем младший бит во всех 4х байтах 32х разрядного слова в этих
        // младших битах находятся 0 или 1 возможно бинаризованного битмапа. Если в
        // массиве встречается серое (или чб) значит оставшиеся биты составляют число
        // больше ноля, то останавливаем цикл и далее выполняем бинаризацию битмапа.
        
        unsigned int b_data=data_size/4-8;   s=0;
        for ( x=8; s<1 && x<b_data; x++ ) {  // эквивалентно if(s>0) {break;}
            s=bytes_data_32[x] & 0xFEFEFEFE;
        } // x  // в бинаризации д.б. s>0;
        
        // int graduation=0;
        // if(s>0) graduation=1; // если в битмапе встречается серое надо применять адаптивный порог
        // graduation=0;         //  если битмап уже бинаризован.
        // return graduation;
        
        /// быстрая инверсия входного массива битмап ///    
        //*
        // invert=1;  // TEST
        // invert=0 без инверсии (белый фон), invert=1 с инверсией (обычно черный фон)
        if(invert<1) { invert=0;}   if(invert>0) { invert=~0;} // 1111111111111111
        
        // бинаризация входного массива битмап с управляемой инверсией параметром invert.
        // алгоритм //
        // маской выделяем младший бит во всех 4х байтах 32х разрядного слова
        // в этих младших битах находятся искомые 0 или 1 уже бинаризованного битмапа.
        
        b_data=data_size/4;
        if(s>0) {  // s>4
            for ( x=0; x < b_data; x++ ) {
                bytes_data_32[x]=bytes_data_32[x]>>7 & 0x1010101 ^ invert; // деление на 128
            } // x  // ^ исключающее ИЛИ
        } // if
        //*/
        // Получили выходной битмап с уровнями:   0-черный    1-белый
        
        DM(" s "<<s<<END);
        
        ///TIME_PRINT_    
        //*/
        
        //------------------------------------------------------------------------------
        
        /*
         // Классический вариант //
         /// классическое автоопределение и бинаризация входного массива битмап ///
		 //автоопределение количества градаций в битмапе
		 int max=0;
		 for ( x=0; x < data_size; x++ ) {
         s=bytes_data[x];
         if ( s > max ) max=s;
		 }
         
		 // бинаризация входного массива битмап
		 if(max>1){
         for ( x=0; x < data_size; x++ ) {
         if(invert){
         if ( bytes_data[x]>127 ){ bytes_data[x]=0; } // уровни в битмапе 0 и 255
         else { bytes_data[x]=1; }  // уровни в битовом массиве 1 и 0
         }else{
         if ( bytes_data[x]>127 ){ bytes_data[x]=1; } // уровни в битмапе 0 и 255
         else { bytes_data[x]=0; }  // уровни в битовом массиве 1 и 0
         }
         }
		 }
         //*/
        //------------------------------------------------------------------------------
        
        DM(" x "<<x<<" s "<<s<<" invert "<<invert<<END);
        // DM(END<<" data_size "<<data_size<<" ncolumns*nrows "<<ncolumns*nrows<<END);
        //DM(" s "<<s<<END);
        
        DM(" TIME_PRINT AutodetectGradations /AAA/ ");
        //TIME_PRINT_
        DM(END);
        
    }//_____________________________________________________________________________
    
    /// автоопределение наличия серого или черно-белого в битмапе /// 29 ноября 2012г
    
    int  GBitmap::DetectGradations() {
        
        // автоопределение наличия серого или черно-белого в битмапе. // time=0.006 для чб  
        // graduation=1; в битмапе встречается серое и надо применять адаптивный порог.
        // graduation=0; битмап уже бинаризован.
        
        // входной массив bytes_data это unsigned char и может принемать значения 
        // черно-белый битмап: младший бит байта 0-черный, 1-белый (или весь байт 00000000-черный, 255-11111111-белый).
        // серый битмап: байт принемает значения от 0-черный до 255-белый со всеми промежуточными значениями.
        
        // в AutodetectGradations алгоритм определения серого изящнее и быстрее, но работает только если в
        // черно-белом битмапе: младший бит байта 0-черный, 1-белый (белый 255 не годится)
        
        unsigned int x,s;
        unsigned short gr=0;
        int graduation;
        
        
        // уменьшение площади сканирования графического текста 
        // если площадь графического текста больше 1 млн.pix то прыгоем по пикселям через n 
        int n=data_size/1000000+1;         if(n>128) n=128;
        //cout<<"n="<<n<<endl;
        
        ///TIME_START
        unsigned int x1=data_size/8;  unsigned int x2=data_size-data_size/8;
        for ( x=x1; gr<1 && x<x2; x+=n ) {  // эквивалентно if(s>0) {break;}       
            s=bytes_data[x];   // s=*(p0+x);
            // проверка наличия серого в битмапе
            if( s>0 && s<255 ) { gr=1;}
            //cout<<"_"<<gr;        
        } // x  
        //cout<<endl;
        //cout<<"bytes_data[9]="<<(short)bytes_data[9]<<endl;
        
        graduation=gr; 
        
        /// cout<<"Автоопределение наличие серого (1) или ч/б (0) в битмапе DetectGradations()   graduation="<<graduation<<endl;
        
        ///TIME_PRINT_
        
        // возвращает 1 если в битмапе встречается серое.
        // возвращает 0 если битмап уже бинаризован.
        return graduation;
        
    }//_____________________________________________________________________________
    
    
    // ---------------- IMPLEMENTATION
    
    inline unsigned int
    GBitmap::rows() const
    {
        return nrows;
    }
    
    inline unsigned int
    GBitmap::columns() const
    {
        return ncolumns;
    }
    
    inline unsigned int
    GBitmap::borderSize() const
    {
        return border;
    }
    
    
    inline unsigned int
    GBitmap::rowsize() const
    {
        return bytes_per_row;
    }
    
    inline unsigned char *
    GBitmap::operator[](int row)
    {
        if (row<0 || row>=nrows) {
            return  bytes_data;
        }
        return &bytes_data[row * ncolumns];
    }
    
    inline const unsigned char *
    GBitmap::operator[](int row) const
    {
        if (row<0 || row>=nrows) {
            return bytes_data;
        }
        return &bytes_data[row * ncolumns];
    }
    
    
    inline GBitmap*
    GBitmap::operator=(const GBitmap*ref)
    {
        init(ref);
        return this;
    }

    
    /**/
};
//******************************************************************************

