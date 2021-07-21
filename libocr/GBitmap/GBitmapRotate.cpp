//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .

#include "GBitmap.h"
#include <algorithm>

//*****************************************************************************
namespace ocr{
  
  
  ///______Rotate_function______///
  // это переспективы //
  
  /*
   - Функция бикубического (bicubic) вращения цветного RGB (3*uchar) массива
   Бикубический фильтр (bicubic, catrom в ImageMagick). ////////////??????
   Эта функция имеет импульсную характеристику в виде Гауссиана (шляпа).
   
   - Функция бикубического (bicubic) вращения серого (uchar) массива.
   Бикубический фильтр (bicubic, catrom в ImageMagick).
   Эта функция имеет импульсную характеристику в виде Гауссиана (шляпа).
   
   - Функция билинейного вращения цветного RGB (3*uchar) массива по методу "билинейной интерполяции".
   Билинейный фильтр (bilinear или triangle в ImageMagick).
   Эта функция имеет треугольную импульсную характеристику.
   
   + Функция билинейного вращения серого (uchar) массива по методу "билинейной интерполяции".
   Билинейный фильтр (bilinear или triangle в ImageMagick).
   Эта функция имеет треугольную импульсную характеристику
   
   - Функция вращения серого (char) массива по методу "ближайших соседей"
   Эта функция имеет прямоугольную импульсную характеристику.
   
   + Функция вращения или (и) масштабирование серого (char) массива по методу "сдвиго".
   
   Импульсная характеристика это реакция системы на единичный импульс, в данном случае на один pix.
   Маштабирование изменяет содержимое исходного изображения
   */
  //_______________________________________________________________________________
  
  
  /**\brief  Вращение или (и) масштабирование изображения c заменой исходных данных */
  void GBitmap::rotateFast(float angle){
    
    GBitmap *ref=GBitmap::create(this);
    rotateFast(ref,1,angle);
    ref->destroy();
    
  }//_______________________________________________________________________________
  
  
  
  // ГОТОВАЯ version
  
  /**  Вращение или (и) масштабирование битовой картинки сдвигом */
  /**  поворот изображения осуществляется на произвольный угол и дискретные углы  +180°, +90°, -90° */
  // добавлено быстое вращение на +180° 16 марта 2013 года
  void GBitmap::rotateFast(const GBitmap*src, float scale, float rotation){
    
    if(fabs(rotation)<0.05) return;
    
    // the scaling factor  // Коэффициент масштабирования  (обратно прпорциональный масштаб)
    // float scale=1.0;
    double scale_=(double)scale;
#define SCALE scale_* ///выполнение масштабирования при вращении
    
    
    // the rotation angle  // угол поворота в градусах    ///ang=0.03;
    double ang=rotation / 57.29747;  // ang - угол поворота в радианах
    // 57.29747 = 180/3.1415 = PI / 180;
    
    
    int x,y;
    int w=src->columns();   // ширина массива  IMAGE WIDTH
    int h=src->rows();      // высота массива  IMAGE HEIGHT
    int w_1=w-1;      int h_1=h-1;
    int w_2=w/2;      int h_2=h/2;
    int w_h=w*h;
    int k=1<<16;            //  k=65536        // 0x10000=65536      0x8000=32768
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
    // "DEMO.DESIGN FAQ,  http://www.enlight.ru/demo/faq ".
    // Вpащение pастpовой каpтинки
    // http://algolist.manual.ru/graphics/rotate.php
    
  }//_________________________________________________________________________
  
  
  // AForge Image Processing Library
  //
  // Copyright © Andrew Kirillov, 2005-2007
  // andrew.kirillov@gmail.com
  //
  // Rotate image using bilinear interpolation
  //
  // This algorithm was taken from the AForge.NET sourcecodes
  // and adopted for the FreeImage library
  // https://www.codeproject.com/Articles/9727/Image-Processing-Lab-in-C
  
  // Текст:
  // http://www.djvu-soft.narod.ru/bookscanlib/021.htm
  
  // Функции:
  // CalculateRotatedSize()
  // BilinearGrayRotate()
  
  //_________________________________________________________________________
  
  
  /** Функция рассчитывает новый размер повернутого изображения. Возвращает новый размер изображения.*/
  void GBitmap::CalculateRotatedSize(double angle, int *new_width, int *new_height) {
    
    double pi = 3.14159265359;
    
    // angle's sine and cosine. Косинус и синус угла angleRad.
    double angleRad = -angle * pi / 180;
    double angleCos = cos(angleRad);
    double angleSin = sin(angleRad);
    
    uint width=ncolumns;    // ширина массива (изображения),  IMAGE WIDTH.
    uint height=nrows;      // высота массива (изображения),  IMAGE HEIGHT.
    
    // calculate half size.
    // Вычисление центров вращения исходных и повёрнутых изображений, как половина ширины и высоты изображения.
    double halfWidth  = (double) width/2;
    double halfHeight = (double) height/2;
    
    // rotate corners. Поворот размеров изображения на угол angleRad.
    double cx1 = halfWidth * angleCos;
    double cy1 = halfWidth * angleSin;
    
    double cx2 = halfWidth * angleCos - halfHeight * angleSin;
    double cy2 = halfWidth * angleSin + halfHeight * angleCos;
    
    double cx3 = -halfHeight * angleSin;
    double cy3 =  halfHeight * angleCos;
    
    double cx4 = 0;
    double cy4 = 0;
    
    // recalculate image size. Пересчет размеров изображения с учетом угла поворота.
    halfWidth  = max( max(cx1, cx2), max(cx3, cx4) ) - min( min(cx1, cx2), min(cx3, cx4) );
    halfHeight = max( max(cy1, cy2), max(cy3, cy4) ) - min( min(cy1, cy2), min(cy3, cy4) );
    
    *new_width  = (int) ( halfWidth *2 + 0.5 );   // новая ширина изображения, new IMAGE WIDTH.
    *new_height = (int) ( halfHeight*2 + 0.5 );   // новая высота изображения, new IMAGE HEIGHT.
    
    
  } //_______________________________________________________________________________
  
  
  /** Поворот битовой серой картинки с билинеарным сглаживанием. Bilinear Rotate. */
  
  void GBitmap::BilinearGrayRotate(double angle){
    //FIBITMAP* ProcessFilter(FIBITMAP* src_dib, double angle) e
    
    // Angle - это угол поворота в градусах.
    // Функция работает с серыми или ч.б. изображениями "uchar".
    // На выходе функции получаем картинку в массиве с тем же названием,
    // но с измененными размерами и повернутую на угол Angle.
    // Алгоритм поворачивает именно серую, а не цветную картинку.
    
    if(fabs(angle)<0.05) return;
    
    uint width=ncolumns;    // ширина массива (изображения)  IMAGE WIDTH  // FreeImage_GetWidth(src_dib);
    uint height=nrows;      // высота массива (изображения)  IMAGE HEIGHT // FreeImage_GetWidth(src_dib);
    
    TIME_START
    
    int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT) { cout<<"Функция поворота битовой картинки с билинеарным сглаживанием.   ProcessFilter()   angle="<<angle<<endl; }
    // width=3300,  height=4800,   new_width=3706,  new_height=5069,   angle=5   time=0.5
    
    double pi = 3.14159265359;
    
    int new_width, new_height; // новая ширина и высота изображения.
    
    // Запуск функции вычисления нового размера повернутого изображения.
    CalculateRotatedSize(angle, &new_width, &new_height);
    //CalculateRotatedSize(src_dib, angle, new_width, new_height);
    if (GrafT) { cout<<"width="<<width<<",  height="<<height<<",   new_width="<<new_width<<",  new_height="<<new_height<<endl; }
    
    ///unsigned src_pitch = FreeImage_GetPitch(src_dib);  /// dst_dib, output
    uint src_pitch = width;         // исходная ширина изображения.
    ///unsigned dst_pitch = FreeImage_GetPitch(dst_dib);
    uint dst_pitch = new_width;     // новая ширина изображения.
    
    
    // Размер исходного серого изображения, в котором на один пиксель приходится один байт.
    ///uint src_dib=width*height;
    
    ////BYTE* src_bytes = (BYTE*)FreeImage_GetBits(src_dib); // The image raster
    uchar *src_bytes = (uchar*)bytes_data; // исходное изображение.
    
    // Размер выходного повернутого серого изображения, в котором на один пиксель приходится один байт.
    uint dst_dib = new_width*new_height;
    
    ///BYTE* dst_bytes = (BYTE*)FreeImage_GetBits(dst_dib); // The image raster
    uchar *dst_bytes = (uchar*)malloc(dst_dib*sizeof(uchar*)); // выходное повернутое изображение.
    
    uchar *lined; // просто указатель
    
    // переменная цикла "Заполнение целевого изображения значением белого."
    //uint d;
    
    // Получение половинного размера исходного изображения.
    double  halfWidth  = (double) width/2;
    double  halfHeight = (double) height/2;
    
    // get destination image size. Получение половинного размера целевого изображения.
    double  halfNewWidth  = (double) new_width/2;
    double  halfNewHeight = (double) new_height/2;
    
    // angle's sine and cosine. Синус и косинус угла angleRad.
    double angleRad = -angle*pi/180;
    double angleCos = cos( angleRad );
    double angleSin = sin( angleRad );
    
    // fill values. заполняем значением белого.
    uchar fill=255; // 0xFF white белый
    
    // destination pixel's coordinate relative to image center. Координаты конечного пикселя относительно центра изображения.
    double cx, cy;
    // coordinates of source points. Координаты исходных точек.
    double  ox, oy, dx1, dy1, dx2, dy2;
    int     ox1, oy1, ox2, oy2;
    // width and height decreased by 1. Ширина и высота уменьшиная на 1.
    int ymax = height-1;
    int xmax = width -1;
    
    uchar *p1, *p2; // temporary pointers. Указатели.
    
    
    
    cy = -halfNewHeight;
    
    for ( int y=0; y < new_height; y++ )
    {
      cx = -halfNewWidth;
      
      lined = dst_bytes + y * dst_pitch;  // для серого dst_pitch=ncolumns=new_width, btpp=1
      
      for ( int x=0; x < new_width; x++ )
      {
        // coordinates of source point. Координаты исходной точки.
        ox =  angleCos * cx + angleSin * cy + halfWidth;
        oy = -angleSin * cx + angleCos * cy + halfHeight;
        
        // top-left coordinate. Верхняя левая координата.
        ox1 = (int) ox;
        oy1 = (int) oy;
        
        // validate source pixel's coordinates. // Проверка координат исходного пикселя на выход за края исходного изображения.
        // Если исходный пиксель выходит за края исходного изображения (габарита) - записываем в пиксель цвет фоновой заливки (белый).
        if ( ( ox1 < 0 ) || ( oy1 < 0 ) || ( ox1 >= width ) || ( oy1 >= height ) )  // int ymax=height-1; int xmax=width-1;
        {
          ///for (d=0; d<btpp; d++)  lined[x * btpp + d] = fill;
          lined[x]=fill; // fill destination image with filler. Заполнение краев целевого изображения значением белого 255.
        }
        // Если исходная точка находится в пределах исходного изображения - вычисляем пропорциональный цвет и записываем его в пиксель назначения.
        else
        {
          // bottom-right coordinate. Нижняя правая координата.
          ox2 = ( ox1 == xmax ) ? ox1 : ox1 + 1;
          oy2 = ( oy1 == ymax ) ? oy1 : oy1 + 1;
          
          if ( ( dx1 = ox - (double) ox1 ) < 0 )  dx1 = 0;
          dx2 = 1.0 - dx1;
          
          if ( ( dy1 = oy - (double) oy1 ) < 0 )  dy1 = 0;
          dy2 = 1.0 - dy1;
          
          p1 = src_bytes + oy1 * src_pitch;
          p2 = src_bytes + oy2 * src_pitch;
          
          // interpolate using 4 points. Интерполяция по 4 точкам.
          ///for (d=0; d<btpp; d++)
          ///    lined[x * btpp + d] = (uchar) ( dy2 * ( dx2 * p1[ox1 * btpp + d] + dx1 * p1[ox2 * btpp + d] ) +
          ///                                    dy1 * ( dx2 * p2[ox1 * btpp + d] + dx1 * p2[ox2 * btpp + d] ) );
          lined[x] = (uchar)( dy2*(dx2*p1[ox1] + dx1*p1[ox2]) + dy1*(dx2*p2[ox1] + dx1*p2[ox2]) );
          //cout<<(int)lined[x]<<" ";
          
        }
        
        cx++;
      }
      //cout<<endl;
      cy++;
    }
    
    // Освобождение исходного массива bytes_data
    free(bytes_data);
    // Получение ссылки на место в памяти (на массив) где находится dst_bytes с выходным повернутым изображением.
    bytes_data=dst_bytes;
    // Замена исходных параметров на новые.
    ncolumns=new_width;
    nrows=new_height;
    data_size=new_width * new_height;
    bytes_per_row=new_width;        // количество байтов в строке
    
    
    if(GrafT) { TIME_PRINT_ }
    
    // для серого dst_pitch=ncolumns=new_width, btpp=1
    // double pi = 3.14159265358979323846264338327950288419716939937510;
    
    // для серого dst_pitch=ncolumns ////////////////////////////////////
    //uint btpp=1; // для серого btpp=1 //////////////////////////////////// определяет его формат bpp // uint btpp = bpp/8;
    //  if (!dib)  printf("%s%s%s\n","File \"", lpszPathName, "\" not found."); // не найдено  // <= 0
    //  if (dib) // bitmap is successfully loaded! // растровое изображение успешно загружено! //  > 0
    
  } //_______________________________________________________________________________
  
  
  
  //_______________________________________________________________________________
  
  
  // ЗАГОТОВКА //
  /** Поворот цветной RGB картинки с билинеарным сглаживанием. */
  // void GBitmap::BilinearRGBRotate(double angle){
  
  //_______________________________________________________________________________
  
  
} // namespace ocr


