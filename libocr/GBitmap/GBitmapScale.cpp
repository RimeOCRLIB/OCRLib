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
  
  
  ///______Scale_function______///
  
  /*
   - Функция бикубического (bicubic) масштабирования цветного RGB (3*uchar) массива
   Бикубический фильтр (bicubic, catrom в ImageMagick).
   Эта функция имеет импульсную характеристику в виде Гауссиана (шляпа).
   
   - Функция бикубического (bicubic) масштабирования (Resample) серого (uchar) массива.
   Бикубический фильтр (bicubic, catrom в ImageMagick).
   Эта функция имеет импульсную характеристику в виде Гауссиана (шляпа).
   
   - Функция билинейного масштабирования (Resample) серого (uchar) массива по методу "билинейной интерполяции".
   Билинейный фильтр (bilinear или triangle в ImageMagick).
   Эта функция имеет треугольную импульсную характеристику.
   
   - Функция масштабирования (Resample) серого (char) массива по методу "ближайших соседей"
   Эта функция имеет прямоугольную импульсную характеристику.
   
   Импульсная характеристика это реакция системы на единичный импульс, в данном случае на один pix.
   Маштабирование изменяет содержимое исходного изображения
   */
  //_______________________________________________________________________________
  
  
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
    
  }//_____________________________________
  
  // ГОТОВАЯ version
  
  /** масштабирование (Resample) серого (char) массива по методу "ближайших соседей" */
  void GBitmap::scaleFast(const GBitmap*ref, float scale){
    
    // Метод Ближайших соседей. Эта функция имеет прямоугольная импульсную характеристику.
    
    int x,y,yp;
    int w=ref->columns();    //cout_<<"w="<<w;  /// ширина массива
    int h=ref->rows();       //cout_<<" h="<<h; /// высота массива
    int new_p;               ///  пиксел, бижайший пиксел
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
      yp=((y<<8)/scaleP)*w;                            // *256
      d0=p0+y*ncolumns;
      for(x = 0; x < ncolumns; x++) {                  //p=y*new_w + x;
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
  
  
  /** маштабирование изменяет содержимое исходного изображения */
  
  void GBitmap::bilinearScale(float scale){
    if(scale==0 || scale==1 )return;
    GBitmap *ref=GBitmap::create(this);
    if(data_size){
      if(!pack_flag){
        free(bytes_data);
      }else{
        free(bites_data);
      }
    }
    bilinearScale(ref,scale);
    ref->destroy();
    
  } //_____________________________________
  
  // ГОТОВАЯ версия.
  
  /** Билинейное масштабирование (Resample) серого (uchar) массива по методу "билинейной интерполяции" */
  
  void GBitmap::bilinearScale(GBitmap *ref, float scale) {
    
    // Bilinear Image Scaling // Билинейное масштабирование изображения //
    // Эта функция имеет треугольную импульсную характеристику.
    // http://tech-algorithm.com/articles/bilinear-image-scaling/
    
    
    //cout<<"Билинейное масштабирование серого изображения bilinearScale()   scale="<<scale<<endl;
    
    
    // ограничения на величину масштаба
    if (scale<0.05) return;
    if (scale>5.0)scale=5.0;   //if (scale<0.05)scale=0.05;
    
    int x,y;
    int w=ref->columns();  //cout_<<"w="<<w;   // исходная ширина массива
    int h=ref->rows();     //cout_<<" h="<<h;  // исходная высота массива
    
    int w2=w*scale;        // новая ширина изображения
    int h2=h*scale;        // новая высота изображения
    init(w2,h2,ref->colorMode);
    unsigned char *p0=bytes_data;       // указатель на выходной массив пикселей масштабированного изображения
    unsigned char *p1=ref->bytes_data;  // указатель на исходный массив пикселей изображения, которое необходимо увеличить
    //cout<<"w2="<<w2<<" w="<<w<<" h2="<<h2<<" h="<<h<<endl;
    
    int A, B, C, D, index, gray;
    float x_ratio = 1/scale;  // ((float)(w-1))/w2;
    float y_ratio = 1/scale;  // ((float)(h-1))/h2;
    float x_diff, y_diff;
    int y_w;
    
    //TIME_START // time=1.00  scale=1.055  w=16224 × h=2464
    
    h2-=3;   // algorithm border
    
    for (int i=0; i < h2; i++) { // cout<<" y"<<i<<" y_="<<(int)(y_ratio * i)<<endl;
      
      y = (int)(y_ratio*i);
      y_diff = (y_ratio*i) - y;
      y_w = y*w;
      
      for (int j=0; j < w2; j++) {
        
        x = (int)(x_ratio*j);
        x_diff = (x_ratio*j) - x;
        index = y_w + x;
        
        // range is 0 to 255 thus bitwise AND with 0xff
        A = p1[index] & 0xff;
        B = p1[index+1] & 0xff;
        C = p1[index+w] & 0xff;
        D = p1[index+w+1] & 0xff;
        
        // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
        gray = (uchar)( A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                       C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)  );
        
        *p0=gray; p0++; // bytes_data[offset++] = gray;
        
      } // j
    } // i
    
    //TIME_PRINT_
    
  }; //_____________________________________________________________________________
  
  
  /** функция согласования динамического диапазона для bicubicScale() и bicubicGrayScale() */
  
  // согласование динамического диапазона float c диапазоном представления uchar.
  static inline char clampFloat(float val){
    // ограничение диапазона 0-255
    if (val < 0)   return 0;
    if (val > 255) return 255;
    return (uchar)val;
    ///return val < 0 ? 0 : val > 255 ? 255 : (uchar)val;
    
  } //_____________________________________________________________________________
  
  
  /** функция подготовки данных для bicubicGrayScale */
  void GBitmap::bicubicGrayScale(float scale){
    GBitmap *ref=GBitmap::create(this);
    if(data_size){
      if(!pack_flag){
        free(bytes_data);
      }else{
        free(bites_data);
      }
    }
    bicubicGrayScale(ref,scale);
    ref->destroy();
    
  } //_____________________________________
  
  // ГОТОВАЯ версия.
  
  /** Бикубическое (bicubic) масштабирование (Resample) серого (uchar) массива */
  void GBitmap::bicubicGrayScale(const GBitmap*ref, float scale){     // начало декабря 2018
    
    // - Бикубический фильтр (bicubic, catrom в ImageMagick). Геометрически импульсная характеристика выглядит как Гауссиан (шляпа).
    // - Билинейный фильтр (bilinear или triangle в ImageMagick). Геометрически это треугольная импульсная характеристика.
    // - Ближайший сосед. Геометрически это прямоугольная импульсная характеристика.
    // Импульсная характеристика это реакция системы на единичный импульс, в данном случае на один pix.
    // Данный алгоритм бикубического масштабирования имеет импульсную характеристику в виде Гауссиана
    // шириной в 8 pix (+-4 pix) для scale=4;
    // Функцию бикубического масштабирование можно использовать и для уменьшения, но только для квадратных изображений, когда w1=h1.
    
    // Код:
    // https://codereview.stackovernet.com/ru/q/34617
    
    
    // _Image2OCR/  618 × 880 pixels  при scale=4 увеличивается до 2472 × 3520 pixels,  размер  1 309 199 bytes.
    // bicubicGrayScale(4) time=0.135 + 0.14 = 0.26сек.
    // bicubicGrayScale(2) time=0.1сек.
    // gaussianT(4) time=0.36сек для увеличенного изображения.
    
    //TIME_START
    
    // ограничения на величину масштаба
    if (scale<=1) return;
    if (scale>5.0){scale=5.0;}   //if (scale<1.0){scale=1.0;}  // if (scale<0.2){scale=(float)0.2;}
    
    int w1=ref->columns();       // исходная ширина массива
    int h1=ref->rows();          // исходная высота массива
    int w2=w1*scale;             // новая ширина изображения после применения масштаба
    int h2=h1*scale;             // новая высота изображения после применения масштаба
    init(w2,h2,ref->colorMode);  // копия исходных данных
    // w1*h1 размер массива ref.
    // w2*h2 размер массива Buff и bytes_data *p0.
    ///cout<<"w1="<<w1<<",  h1="<<h1<<"    w1*h1="<<w1*h1<<" пикселей"<<endl;
    //cout<<"w2="<<w2<<",  h2="<<h2<<"    w2*h2="<<w2*h2<<" пикселей"<<endl;
    
    const uchar *p1=ref->bytes_data; // указатель на ИСХОДНЫЙ МАССИВ пикселей изображения
    uchar *p0=bytes_data; // указатель на ВЫХОДНОЙ МАССИВ пикселей модифицированного изображения
    
    // выходной массив для горизонтального и входной массив для вертикального масштабирования.
    uchar *Buff; // указатель на буфферный массив
    Buff = (uchar*)calloc(w2*h2, sizeof(uchar));    // инициализируем в 0.
    
    
    // Быстрое бикубическое масштабирование (bicubic). //
    
    // Горизонтальный масштаб (Horizontal Scale):
    
    float xScale = 1/scale;         // обратный масштаб по горизонтали
    int w1_1 = w1 - 1;              // Устраняет последний пиксель по X
    const uchar *inScan0 = p1;      // *p1   BufIn  входной массив для горизонтального масштабирования
    uchar *outScan0 = Buff;         // *p0   BufOut выходной массив для горизонтального масштабирования
    
    // Precalculate part of x position. Originally intX = (outX + 0.5f) * xRatio - 0.5f.
    // Removes one subtraction operation from the inner loop.
    // Предпросчитанная позиция по "x". Вывод из внутреннего цикла одной операции вычитания.
    float centerPreCalc = xScale * 0.5f - 0.5f;
    
    ///int n=0;  ////////////
    
    // цикл перебора строк
    for(int y=0; y<h1; y++){
      
      // начальный байт каждого ряда
      uchar* outRow = outScan0 + (y * w2);      ///n = y * w2;  //  n = 0;  ////////////
      const uchar* inRow = inScan0 + (y * w1);
      
      // Вывод из внутреннего цикла одной операции умножения (можно вывести и из внешнего цикла).
      float center = centerPreCalc - xScale; // nev
      
      // цикл по строкам (по выходному массиву)
      for (int outX=0; outX<w2; outX++){
        
        // float center = outX * xScale + centerPreCalc; // old
        center+=xScale; // nev
        
        int inX2 = (int)center;   // Center pixel
        int inX1 = inX2 - 1;
        int inX3 = inX2 + 1;
        int inX4 = inX2 + 2;
        
        float diff2 = center - inX2;
        float diff3 = inX3 - center;
        float diff4 = inX4 - center;
        float diff4Min2 = diff4 - 2;
        
        // Weights are using simplified catmull-rom formula.
        // Вычисление весов по упрощенной формуле Catmull-Rom.
        float weight2 = (1.5f * diff2 - 2.5f) * (diff2 * diff2) + 1;
        float weight3 = (1.5f * diff3 - 2.5f) * (diff3 * diff3) + 1;
        float weight4 = -0.5f * (diff4Min2 * diff4Min2) * (diff4 - 1);
        float weight1 = 1 - weight2 - weight3 - weight4;
        // weight1 + weight2 + weight3 + weight4 = 1; // Дополнение суммы всех весов до 1. Нормировка.
        // Because the weights will add up to 1.
        // Weight one is chosen to calculate this way as it is the most expensive
        // due to originally containing an if statement.
        // Вес равный 1 выбирается для расчета этого способа (таким образом),
        // поскольку он является самым дорогим из-за первоначально содержащего оператора if.
        
        int col1 = max(inX1, 0);
        int col2 = inX2;
        int col3 = min(inX3, w1_1);
        int col4 = min(inX4, w1_1);
        
        float inB1 = inRow[col1];
        float inB2 = inRow[col2];
        float inB3 = inRow[col3];
        float inB4 = inRow[col4];
        
        float outB = inB1 * weight1 + inB2 * weight2 + inB3 * weight3 + inB4 * weight4;
        
        // запаковка в выходной массив с ограничением диапазона float до 0-255
        //*outRow++ = clampFloat(outB);
        if (outB < 0)   outB=0;
        if (outB > 255) outB=255;
        ///n++;  ////////////
        *outRow++ = (uchar)outB;  ////////////
        /**/
      } // outX
      ///cout<<"n="<<n<<endl;
    } // y
    
    /**/
    ///cout<<"n="<<n<<endl;
    
    // Вертикальный масштаб (Vertical Scale):
    
    float yScale = xScale;  // обратный масштаб по вертикали.
    int h1_1 = h1 - 1;      // Высота входного массива без 1. Устраняет последний ряд пикселей по Y.
    inScan0  = Buff;        // In  входной массив для вертикального масштабирования, восстановление указателей.
    outScan0 = p0;          // Out выходной массив для вертикального масштабирования, восстановление указателей.
    
    
    // цикл перебора строк
    for(int outY=1; outY<h2; outY++){ // стартуем с 1 для устранения первого ряда пикселей по Y
      
      // начальный байт каждого ряда
      uchar* outRow = outScan0 + (outY * w2); /// byte* outRow = outBmpDataScan0 + (outY * outDataStride);    // w2_bpp
      
      float center = (outY + 0.5f) * yScale - 0.5f;
      
      int inY2 = (int)center;   //Center pixel
      int inY1 = inY2 - 1;
      int inY3 = inY2 + 1;
      int inY4 = inY2 + 2;
      
      
      const uchar* inRow1 = inScan0 + max(inY1, 0) * w2;
      const uchar* inRow2 = inScan0 + inY2 * w2;
      const uchar* inRow3 = inScan0 + min(inY3, h1_1) * w2; // h1_1 устраняет последний ряд пикселей по Y
      const uchar* inRow4 = inScan0 + min(inY4, h1_1) * w2; // h1_1 устраняет последний ряд пикселей по Y
      
      float diff2 = center - inY2;
      float diff3 = inY3 - center;
      float diff4 = inY4 - center;
      float diff4Min2 = diff4 - 2;
      
      // Weights are using simplified catmull-rom formulas.
      // Вычисление весов по упрощенной формуле Catmull-Rom.
      float weight2 = (1.5f * diff2 - 2.5f) * (diff2 * diff2) + 1;
      float weight3 = (1.5f * diff3 - 2.5f) * (diff3 * diff3) + 1;
      float weight4 = -0.5f * (diff4Min2 * diff4Min2) * (diff4 - 1);
      float weight1 = 1 - weight2 - weight3 - weight4;
      // weight1 + weight2 + weight3 + weight4 = 1; // Дополнение суммы всех весов до 1. Нормировка.
      
      // цикл по строкам
      for (int x=0; x<w2; x++){
        
        float inB1 = inRow1[x];
        float inB2 = inRow2[x];
        float inB3 = inRow3[x];
        float inB4 = inRow4[x];
        
        float outB = inB1 * weight1 + inB2 * weight2 + inB3 * weight3 + inB4 * weight4;
        
        // запаковка в выходной массив с ограничением диапазона float до 0-255
        //*outRow++ = clampFloat(outB);
        if (outB < 0)   outB=0;
        if (outB > 255) outB=255;
        *outRow++ = (uchar)outB;
        
      } // x
    } // outY
    
    /**/
    if ( Buff !=NULL ) free(Buff);
    
    //TIME_PRINT_
    
    // Сплайн Catmull-Rom является алгоритмом для интерполяции ряда точек (x, y, z).
    // Он будет генерировать кубическую полиномиальную кривую между каждыми двумя последовательными точками.
    
    
  } //_______________________________________________________________________________
  
  /** функция подготовки данных для bicubicScale */
  void GBitmap::bicubicScale(float scale){
    GBitmap *ref=GBitmap::create(this);
    if(data_size){
      if(!pack_flag){
        free(bytes_data);
      }else{
        free(bites_data);
      }
    }
    bicubicScale(ref,scale);
    ref->destroy();
    
  } //_____________________________________
  
  // ГОТОВАЯ версия.
  
  /** Бикубическое (bicubic) масштабирование (Resample) цветного RGB массива */
  void GBitmap::bicubicScale(const GBitmap*ref, float scale){     // начало декабря 2018
    
    // - Бикубический фильтр (bicubic, catrom в ImageMagick). Геометрически импульсная характеристика выглядит как Гауссиан (шляпа).
    // - Билинейный фильтр (bilinear или triangle в ImageMagick). Геометрически это треугольная импульсная характеристика.
    // - Ближайший сосед. Геометрически это прямоугольная импульсная характеристика.
    // Импульсная характеристика это реакция системы на единичный импульс, в данном случае на один pix.
    // Данный алгоритм бикубического масштабирования имеет импульсную характеристику в виде Гауссиана
    // шириной в 8 pix (+-4 pix) для scale=4;. Все 3 канала RGB обрабатываются одинаково.
    // Функцию бикубического масштабирование можно использовать и для уменьшения, но только для квадратных изображений, когда w1=h1.
    
    // Код:
    // https://codereview.stackovernet.com/ru/q/34617
    
    //TIME_START  // _Image2OCR/  618 × 880 pixels  при scale=4;  2472 × 3520 pixels   time=0.6сек   1 308 601 bytes
    
    // ограничения на величину масштаба
    if (scale>5.0){scale=5.0;}   if (scale<1.0){scale=1.0;}  // if (scale<0.2){scale=(float)0.2;}
    
    int w1=ref->columns();       // исходная ширина массива
    int h1=ref->rows();          // исходная высота массива
    int w2=w1*scale;             // новая ширина изображения после применения масштаба
    int h2=h1*scale;             // новая высота изображения после применения масштаба
    int bpp = 3;                 // количество цветовых каналов в RGB // int bpp = Bitmap.GetPixelFormatSize(PixelFormat.RGB) / 8;
    int w1_h1=w1*h1;             // размер массива ref и bytes_data.
    int w2_h2_bpp=w2*h2*bpp;     // размер входного () и выходного массива
    init(w2,h2,ref->colorMode);  // копия исходных данных
    
    
    const uchar *p1=ref->bytes_data; // указатель на ИСХОДНЫЙ МАССИВ пикселей изображения
    uchar *p0=bytes_data; // указатель на ВЫХОДНОЙ МАССИВ пикселей модифицированного изображения
    
    // RGB в обычных массивах упаковано последовательно по 3 байта "uchar"  (bpp=3)
    // Тестовые массивы BufIn и BufOut используются для создания синтетического RGB цветного изображения из серой картинки
    
    // входной RGB массив пикселей изображения.
    uchar *BufIn; // входной массив создаем с запасом в размер выходного и инициализируем в 0.
    BufIn = (uchar*)calloc(w2_h2_bpp+32, sizeof(uchar));
    //BufIn = (uchar*)calloc(w1_h1*bpp+32, sizeof(uchar));
    
    // выходной RGB массив пикселей изображения.
    uchar *BufOut;
    BufOut = (uchar*)calloc(w2_h2_bpp+32, sizeof(uchar));
    
    // заполнение каждого из трех RGB байтов входного цветного массива BufIn одним байтом серого исходного массива *p1 bytes_data.
    // для тестирования функции
    for (int x=0; x < w1_h1; x++ ) {
      int x_bpp=x*bpp;
      uchar px=*(p1+x);
      BufIn[x_bpp]  =px;
      BufIn[x_bpp+1]=px;
      BufIn[x_bpp+2]=px;
      //BufIn[x_bpp]=BufIn[x_bpp+1]=BufIn[x_bpp+2]=px;
    }
    // Теперь все RGB каналы одинаковые
    /**/
    
    
    // Быстрое бикубическое масштабирование (bicubic). //
    
    // Горизонтальный масштаб (Horizontal Scale):
    
    float xScale = 1/scale;         // обратный масштаб по горизонтали
    int w1_1 = w1 - 1;              // Устраняет последний пиксель по X
    const uchar *inScan0 = BufIn;   // *p1
    uchar *outScan0 = BufOut;       // *p0
    
    
    // Вывод из внутреннего цикла одной операции умножения.
    int w1_bpp = w1*bpp;
    int w2_bpp = w2*bpp;
    
    // Precalculate part of x position. Originally intX = (outX + 0.5f) * xRatio - 0.5f.
    // Removes one subtraction operation from the inner loop.
    // Предпросчитанная позиция по "x". Вывод из внутреннего цикла одной операции вычитания.
    float centerPreCalc = xScale * 0.5f - 0.5f;
    
    // цикл перебора строк
    for(int y=0; y<h1; y++){
      
      // начальный байт каждого ряда
      uchar* outRow = outScan0 + (y * w2_bpp);    /// byte* outRow = outScan0 + (y * outDataStride);
      const uchar* inRow = inScan0 + (y * w1_bpp);/// byte* inRow = inScan0 + (y * intDataStride);
      
      // Вывод из внутреннего цикла одной операции умножения.
      float center = centerPreCalc - xScale; // nev
      
      // цикл по строкам (по выходному массиву)
      for (int outX=0; outX<w2; outX++){
        
        // float center = outX * xScale + centerPreCalc; // old
        center+=xScale; // nev
        
        int inX2 = (int)center;   // Center pixel
        int inX1 = inX2 - 1;
        int inX3 = inX2 + 1;
        int inX4 = inX2 + 2;
        
        float diff2 = center - inX2;
        float diff3 = inX3 - center;
        float diff4 = inX4 - center;
        float diff4Min2 = diff4 - 2;
        
        // Weights are using simplified catmull-rom formula.
        // Вычисление весов по упрощенной формуле Catmull-Rom.
        float weight2 = (1.5f * diff2 - 2.5f) * (diff2 * diff2) + 1;
        float weight3 = (1.5f * diff3 - 2.5f) * (diff3 * diff3) + 1;
        float weight4 = -0.5f * (diff4Min2 * diff4Min2) * (diff4 - 1);
        float weight1 = 1 - weight2 - weight3 - weight4;
        // weight1 + weight2 + weight3 + weight4 = 1; // Дополнение суммы всех весов до 1. Нормировка.
        // Because the weights will add up to 1.
        // Weight one is chosen to calculate this way as it is the most expensive
        // due to originally containing an if statement.
        // Вес равный 1 выбирается для расчета этого способа (таким образом),
        // поскольку он является самым дорогим из-за первоначально содержащего оператора if.
        
        int col1 = max(inX1, 0) * bpp;
        int col2 = inX2 * bpp;
        int col3 = min(inX3, w1_1) * bpp;
        int col4 = min(inX4, w1_1) * bpp;
        
        float inB1 = inRow[col1];
        float inG1 = inRow[col1 + 1];
        float inR1 = inRow[col1 + 2];
        float inB2 = inRow[col2];
        float inG2 = inRow[col2 + 1];
        float inR2 = inRow[col2 + 2];
        float inB3 = inRow[col3];
        float inG3 = inRow[col3 + 1];
        float inR3 = inRow[col3 + 2];
        float inB4 = inRow[col4];
        float inG4 = inRow[col4 + 1];
        float inR4 = inRow[col4 + 2];
        
        float outB = inB1 * weight1 + inB2 * weight2 + inB3 * weight3 + inB4 * weight4;
        float outG = inG1 * weight1 + inG2 * weight2 + inG3 * weight3 + inG4 * weight4;
        float outR = inR1 * weight1 + inR2 * weight2 + inR3 * weight3 + inR4 * weight4;
        
        // запаковка в выходной массив с ограничением диапазона float до 0-255
        *outRow++ = clampFloat(outB);
        *outRow++ = clampFloat(outG);
        *outRow++ = clampFloat(outR);
        
      } // outX
    } // y
    
    /**/
    
    
    /*
     // заполнение байтами выходного массива *p0 bytes_data. Каждый байт это средее по трем RGB байтам массива BufOut.
     // *p0 bytes_data выводится в файл ___1Draw_bicubic.jpg.
     for (int x=0; x < w2*h2; x++ ) {
     int x_bpp=x*bpp;
     *(p0+x)=(BufOut[x_bpp] + BufOut[x_bpp+1] + BufOut[x_bpp+2])/3;
     } // for x
     */
    
    
    /*
     GBitmap *im=GBitmap::create(w1, h1);
     uchar *pp=im->bytes_data;
     for (int x=0; x < w1_h1; x++ ) {
     int x_bpp=x*bpp;
     *(pp+x)=(BufIn[x_bpp] + BufIn[x_bpp+1] + BufIn[x_bpp+2])/3; //
     } // for x
     im->printToScreen();
     */
    
    /*
     GBitmap *im=GBitmap::create(w2, h2);
     uchar *pp=im->bytes_data;
     for (int x=0; x < w1_h1; x++ ) {
     int x_bpp=x*bpp;
     *(pp+x)=(BufOut[x_bpp] + BufOut[x_bpp+1] + BufOut[x_bpp+2])/3; //
     } // for x
     im->printToScreen();
     */
    
    // free(BufIn);
    // BufIn = (uchar*)calloc(w2_h2_bpp+32, sizeof(uchar));
    // Копирование результатов горизонтального масштабирования во входной массив
    // memcpy делается для прозрачности кода, можно просто поменнять местами BufIn и BufOut
    memcpy(BufIn,BufOut,w2_h2_bpp+32);
    
    
    // Вертикальный масштаб (Vertical Scale):
    
    float yScale = xScale;  // обратный масштаб по вертикали  /// inBmp.Height / (float)outBmp.Height;
    int h1_1 = h1 - 1;      // Высота входного массива без 1. Устраняет последний ряд пикселей по Y./// int lastInPixelY=inBmp.Height-1;
    inScan0  = BufIn;       // входной массив для вертикального масштабирования. /// byte* inBmpDataScan0 = (byte*)inData.Scan0;
    outScan0 = BufOut;      // выходной массив для вертикального масштабирования./// byte* outBmpDataScan0 = (byte*)outData.Scan0;
    
    ///int outDataStride = w2_bpp;
    ///int intDataStride = w2_bpp;
    
    // цикл перебора строк
    for(int outY=1; outY<h2; outY++){ // стартуем с 1 для устранения первого ряда пикселей по Y
      
      // начальный байт каждого ряда
      uchar* outRow = outScan0 + (outY * w2_bpp); /// byte* outRow = outBmpDataScan0 + (outY * outDataStride);
      
      float center = (outY + 0.5f) * yScale - 0.5f;
      
      int inY2 = (int)center;   //Center pixel
      int inY1 = inY2 - 1;
      int inY3 = inY2 + 1;
      int inY4 = inY2 + 2;
      
      
      const uchar* inRow1 = inScan0 + max(inY1, 0) * w2_bpp;
      const uchar* inRow2 = inScan0 + inY2 * w2_bpp;
      const uchar* inRow3 = inScan0 + min(inY3, h1_1) * w2_bpp; // h1_1 устраняет последний ряд пикселей по Y // h2_1 или h1_1 ?
      const uchar* inRow4 = inScan0 + min(inY4, h1_1) * w2_bpp; // h1_1 устраняет последний ряд пикселей по Y
      //byte* inRow3 = inBmpDataScan0 + (Math.Min(inY3, lastInPixelY) * intDataStride);
      //byte* inRow4 = inBmpDataScan0 + (Math.Min(inY4, lastInPixelY) * intDataStride);
      
      float diff2 = center - inY2;
      float diff3 = inY3 - center;
      float diff4 = inY4 - center;
      float diff4Min2 = diff4 - 2;
      
      // Weights are using simplified catmull-rom formulas.
      // Вычисление весов по упрощенной формуле Catmull-Rom.
      float weight2 = (1.5f * diff2 - 2.5f) * (diff2 * diff2) + 1;
      float weight3 = (1.5f * diff3 - 2.5f) * (diff3 * diff3) + 1;
      float weight4 = -0.5f * (diff4Min2 * diff4Min2) * (diff4 - 1);
      float weight1 = 1 - weight2 - weight3 - weight4;
      // weight1 + weight2 + weight3 + weight4 = 1; // Дополнение суммы всех весов до 1.
      
      // цикл по строкам
      for (int x=0; x<w2; x++){
        
        int colB = x * bpp;
        int colG = colB + 1;
        int colR = colB + 2;
        
        float inB1 = inRow1[colB];
        float inG1 = inRow1[colG];
        float inR1 = inRow1[colR];
        float inB2 = inRow2[colB];
        float inG2 = inRow2[colG];
        float inR2 = inRow2[colR];
        float inB3 = inRow3[colB];
        float inG3 = inRow3[colG];
        float inR3 = inRow3[colR];
        float inB4 = inRow4[colB];
        float inG4 = inRow4[colG];
        float inR4 = inRow4[colR];
        
        float outB = inB1 * weight1 + inB2 * weight2 + inB3 * weight3 + inB4 * weight4;
        float outG = inG1 * weight1 + inG2 * weight2 + inG3 * weight3 + inG4 * weight4;
        float outR = inR1 * weight1 + inR2 * weight2 + inR3 * weight3 + inR4 * weight4;
        
        // запаковка в выходной массив с ограничением диапазона float до 0-255
        *outRow++ = clampFloat(outB);
        *outRow++ = clampFloat(outG);
        *outRow++ = clampFloat(outR);
        
      } // x
    } // outY
    
    
    //cout<<"scale="<<scale<<endl;
    /**/
    
    // заполнение байтами выходного массива *p0 bytes_data. Каждый байт это средее по трем RGB байтам массива BufOut.
    // для тестирования функции
    for (int x=0; x < w2*h2; x++ ) {
      int x_bpp=x*bpp;
      *(p0+x)=(BufOut[x_bpp] + BufOut[x_bpp+1] + BufOut[x_bpp+2])/3;
    } // for x
    /**/
    
    //GBitmap *im=GBitmap::create(w2*bpp, h2);
    //free(im->bytes_data);
    //im->bytes_data=BufOut;
    //im->printToScreen();
    //((GBitmap*)ref)->printToScreen();
    //printToScreen();
    /**/
    
    
    if ( BufIn !=NULL ) free(BufIn);
    if ( BufOut !=NULL ) free(BufOut);
    
    // TIME_PRINT_
    
    ///inScan0  = BufOut;      // BufOut выходной массив для вертикального масштабирования, является входным inScan0.
    ///outScan0 = BufIn;       // BufIn  входной массив для вертикального масштабирования, вляется выходным outScan0.
    
  } //_______________________________________________________________________________
  
  
  
  // ГОТОВАЯ версия.
  
  /**  Функция транспонирования (transpose). // Тестированная 4 дек 2018г */
  void GBitmap::transpose(){
    // void transpose(uchar *src, uchar *dst) {
    
    //  http://qaru.site/questions/2483/what-is-the-fastest-way-to-transpose-a-matrix-in-c
    
    //int w_w=w;		// размер исходного массива по горизонтали.
    //int h_h=h;		// размер исходного массива по вертикали.
    //cout<<endl<<"w="<<w<<"   h="<<h<<endl;
    int w0=5;		// размер исходного массива по горизонтали.
    int h0=3;		// размер исходного массива по вертикали.
    cout<<endl<<"w0="<<w0<<"   h0="<<h0<<endl;
    int w0_h0=w0*h0;    // размер исходного массива.
    
    
    // исходный массив
    uchar BufIn[15]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};    // uchar *src, uchar *dst;
    // транспонированный массив
    uchar BufOut[15]={0};                                      // массив инициализированный "0"
    
    
    cout<<endl<<"1,  2,  3,  4,  5"<<endl;
    cout<<endl<<"6,  7,  8,  9,  10"<<endl;
    cout<<endl<<"11, 12, 13, 14, 15"<<endl;
    cout<<endl<<" BufOut  [n]="<<endl;
    /**/
    // работает медленнее чем штатное вращение хотя нет прыгания по массиву через "w",
    // но больше вычислений в цикле
    // Поворот на -90°. Переупаковка (транспонирование) горизонтального массива строк в массив вертикальных строк.
    for(int n=0; n<w0_h0; n++) {
      
      //dst[n] = src[w0*(n%h0) + n/h0];      // int j=n%h0;  int i = n/h0;  dst[n]=src[w0*j + i];
      *(BufOut+n)=*(BufIn + w0*(n%h0) + n/h0);
      
      cout<<short(BufOut[n])<<"  ";
      
    } // n
    // 1  6  11  2  7  12  3  8  13  4  9  14  5  10  15
    
    for(int n = 0; n<w0_h0; n++) {cout<<short(BufOut[n])<<"  ";} // n
    /**/
    
    
    /*
     // штатное вращение, взято из Гауссиана, работает четко
     // Поворот на -90°. Переупаковка (транспонирование) горизонтального массива строк в массив вертикальных строк.
     //p0=bytes_data;   p1=bytes_data_1;   // index_=0;  p0==BufIn  p1==BufOut
     
     uchar *p0=BufIn;        uchar *p1=BufOut;
     //uchar *p0=bytes_data; // указатель на ВЫХОДНОЙ МАССИВ пикселей модифицированного изображения
     for (int x=0; x < w0; x++ ) {    // time=0.37
     /////      p0++; // +x
     for (int y=0; y < w0_h0; y+=w0 ){
     // *(p1 + index_)=*(p0 + x + y);  index_++;  // index_+=dlt; int dlt=1; // for ( y=0; y < h; y++ ){
     *p1=*(p0+y);   p1++; // p1++;  сквозной индекс в цикле переупаковки    // *p1=*p0;   p1++;  p0+=w;
     } // y
     p0++; // +x
     } // x
     // 1  6  11  2  7  12  3  8  13  4  9  14  5  10  15
     
     cout<<endl;
     p1=BufOut;
     for(int n = 0; n<w0_h0; n++) {cout<<short(*(p1+n))<<"  ";} // n
     */
    
    // 1  6  11  2  7  12  3  8  13  4  9  14  5  10  15
    
    /*
     // тестированно на Гауссиане и работает медленнее чем штатное вращение
     // Поворот (настоящий) на -90°. Переупаковка (транспонирование) горизонтального массива строк в массив вертикальных строк. 4 дек 2018г
     p0=bytes_data;   p1=bytes_data_1;
     for(int n=0; n<w_h; n++) {
     *(p1+n)=*(p0 + w*(n%h) + n/h);
     } // n
     ≈*/
    /**/
    ///TIME_PRINT_
    
  } //_______________________________________________________________________________
  
  
  // ++++++
  // ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian), попытка минимальной реализации.
  // Адаптивный F(KHig) нолевой бордюр вокруг массивов.
  // 2 массива
  // -----------------------------------------------------------------------------
  // треугольная весовая функция
  // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)
  
  /*
   void OCRFilter::filterHigh(vector<int> &In, int KHig){
   filterHigh(&In[0],(uint)In.size(),KHig);
   }//_____________________________________________________________________________
   
   void OCRFilter::filterHigh(int* In,uint size, int KHig){
   */
  
  // -----------------------------------------------------------------------------
  ////void  GBitmap::filterHighM(int KHig) {
  /*
   int y,Khg;
   //if(size>ncnr) return;
   
   //////////////////////////////////////////////  всего 2 массива
   
   if ( KHig<2 ) KHig=1;   if ( KHig>80 ) KHig=80;   // if ( KHig>63 ) KHig=63;
   // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
   //      129           257             385
   int  s2=KHig*2+1,  s4=KHig*4;//    s6=KHig*6+1;
   
   // vector<int> BufIn(size+s6);
   // vector<int> lt1(size+s6);
   // int BufIn[10000]={0};
   // int lt1[10000]={0};
   */
  
  /*
   // обнуление ImBuf,ReBuf ( где ImBuf, ReBuf глобальные )
   memset(ImBuf,0,4*(ncnr+512));      memset(ReBuf,0,4*(ncnr+512));
   //for ( y=0; y < size+s6; y++ ){BufIn[y]=lt1[y]=0;}
   
   // Входной буфер BufIn[y+s2], In[y]
   for ( y=0; y < size; y++ ){ImBuf[y+s2]=In[y];}
   Khg=KHig*KHig;
   for ( y=s2; y < size+s4; y++ ) { //треугольная весовая функция, прямая фильтрация
   ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+ImBuf[y-KHig-KHig]-2*ImBuf[y-KHig]+ImBuf[y]; //k=15+
   }
   for ( y=s2; y < size+s4; y++ ) { ReBuf[y]=ReBuf[y]/Khg;  ImBuf[y]=0; } // Norm // y=0; size+s6;
   // memset(ImBuf,0,4*(ncnr+512));
   for ( y=size+s4; y > s2; y-- ) { //треугольная весовая функция, инверсная фильтрация
   ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+ReBuf[y+KHig+KHig]-2*ReBuf[y+KHig]+ReBuf[y];//k=15-
   }
   for ( y=s2; y < size+s2; y++ ) { In[y-s2]=ImBuf[y]/Khg; }
   // Выходной буфер In[y-s2]
   */
  
  ////} // -----------------------------------------------------------------------------
  
  
  //////////////////////////////////////////////  применен адаптивный бордюр Khg8
  
  /*
   if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
   // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
   //   129             257             385              514
   int  Khg2=KHig*2+1,  Khg4=KHig*4+1,  Khg6=KHig*6+1,   Khg8=KHig*8+2;
   
   vector<int> BufIn(size+Khg8);
   vector<int> lt1(size+Khg8);
   vector<int> BufOut1(size+Khg8);
   
   // Входной буфер BufIn[y+Khg4], In[y]
   for ( y=0; y < size; y++ ){BufIn[y+Khg4]=In[y];}
   Khg=KHig*KHig;
   for ( y=Khg2; y < size+Khg6; y++ ) { //треугольная весовая функция, прямая фильтрация
   lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
   }
   for ( y=0; y < size+Khg8; y++ ) { lt1[y]=lt1[y]/Khg; }         //Norm
   for ( y=size+Khg6; y > Khg2; y--) { //треугольная весовая функция, инверсная фильтрация
   BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
   }
   for ( y=Khg4; y < size+Khg4; y++ ){ In[y-Khg4]=BufOut1[y]/Khg; }
   // Выходной буфер In[y-Khg4, BufOut1[y]
   */
  
  
} // namespace ocr
// -----------------------------------------------------------------------------
