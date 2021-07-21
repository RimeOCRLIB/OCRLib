//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org .

#include <stdio.h>
#include <string.h>
#include "../GBitMask/GBitMask.h"
#include "../OCRAlgorithm/OCRAlgorithm.h"
#include "../OCRString/php2stl.h"
#include "../PNG/lodepng.h"
#include "../config.h"
#include "GBitmap.h"

#ifdef ASM_OS64_SSE42_POPCNT_ATT
#    include <smmintrin.h>
#endif

#ifdef MAGIC
#    include <Magick++.h>
using namespace Magick;
#endif

///#ifdef CPU_OS64_popcnt
//#include <smmintrin.h>
///#endif

namespace ocr
{

GBitmap::GBitmap()
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0){
}

void GBitmap::destroy(void) {
    if(data_size) {
        if(!pack_flag) {
            free(bytes_data);
        } else {
            free(bites_data);
        }
    }
    delete this;
}


GBitmap::GBitmap(int ncolumns, int nrows)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try { 
        init(ncolumns, nrows, 1);
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(int ncolumns, int nrows, int mode)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
        init(ncolumns, nrows, mode);
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(const GBitmap *ref)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
        init(ref);
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(const GBitmap *ref, float scale, float rotation, int mode)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
        init(ref, scale, rotation, mode);
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(const GBitmap *ref, int x0, int y0, int w, int h)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
        initRegion(ref, x0, y0, w, h);
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(const char *path)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
#ifdef ARM
        initPNG(path);
#else
        initImage(path);
#endif
    } catch(int a) { destroy(); }
}

GBitmap::GBitmap(unsigned char *dataPNG, uint size)
    : nrows(0), ncolumns(0), border(0), bytes_per_row(0), data_size(0) {
    try {
        initPNGData(dataPNG, size);
    } catch(int a) { destroy(); }
}

//____________________________________________________________________________

// ----- initialization

void GBitmap::init(int acolumns, int arows, int mode) {
    if(mode==BITMAP_32) {
        nrows=arows;     //=(arows/32+1)*32;       // nrows=arows;
        ncolumns=acolumns;  //=(acolumns/32+1)*32;  // ncolumns=acolumns;
        data_size=ncolumns * nrows;
        data_size_p= data_size / 8;  /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
        data_size_p32= data_size / 32;  /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
        bytes_per_row=ncolumns/8;
        border=0;
        colorModeOut=0;
        colorMode=1;
        if(!data_size) return;
        // инициализация байтового массива char размером  data_size
        bites_data=(unsigned char *)malloc(data_size_p);  // запрос памяти с очисткой 0
        memset(bites_data, 0, data_size_p);
        pack_flag=BITMAP_32;
    } else {
        colorMode=mode;
        colorModeOut=0;
        // выравнивание bytes_data по 32
        // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
        // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
        nrows=arows;     // nrows=(arows/32+1)*32;       // nrows=arows;
        ncolumns=acolumns;  // ncolumns=(acolumns/32+1)*32;  // ncolumns=acolumns;

        data_size=ncolumns * nrows * colorMode;
        if(!data_size) return;
        data_size_p=0;
        bytes_per_row=ncolumns * colorMode;
        border=0;

        // cout<<" data_size="<<data_size<<endl;
        // инициализация байтового массива char размером  data_size
        bytes_data=(unsigned char *)malloc(data_size);  // запрос памяти с очисткой 0

        fill(255);
        pack_flag=0;
    }

}  //____________________________________________________________________________

void GBitmap::initPNG(const char *inputData) {
    unsigned       error;
    unsigned char *image=NULL;
    unsigned       width=0, height=0;
    unsigned char *png=0;
    size_t         pngsize;
    LodePNGState   state;

    lodepng_state_init(&state);
    /*optionally customize the state*/
    state.info_raw.colortype=LCT_GREY;
    state.info_raw.bitdepth=8;

    error=lodepng_load_file(&png, &pngsize, inputData);
    if(!error) error=lodepng_decode(&image, &width, &height, &state, png, pngsize);
    if(error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        exit(0);
    }

    free(png);

    // cout<<"w:"<<width<<" h:"<<height<<" pngsize:"<<pngsize<<endl;

    init(width, height, 1);
    memcpy(bytes_data, image, width * height);

    lodepng_state_cleanup(&state);
    free(image);

}  //_____________________________________________________________________________

void GBitmap::initPNGData(unsigned char *dataPNG, uint size) {
    unsigned       error;
    unsigned char *image=NULL;
    unsigned       width=0, height=0;
    
	error=lodepng_decode_memory(&image, &width, &height, dataPNG, size,LCT_GREY, 8);

    if(error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        exit(0);
    }

    // cout<<"w:"<<width<<" h:"<<height<<" pngsize:"<<pngsize<<endl;

    init(width, height, 1);
    memcpy(bytes_data, image, width * height);
    free(image);

}  //_____________________________________________________________________________

void GBitmap::init(const GBitmap *ref) {
    if(this !=ref) {
        border=0;
        if(ref->pack_flag !=BITMAP_32) {
            init(ref->ncolumns, ref->nrows, ref->colorMode);
            unsigned char *row=bytes_data;
            unsigned char *rowDest;
            for(int n=0; n < nrows; n++, row +=bytes_per_row) {
                rowDest=ref->bytes_data + bytes_per_row * n;
                memcpy((void *)row, (void *)rowDest, ncolumns);
            }
        } else {
            init(ref->ncolumns, ref->nrows, BITMAP_32);
            memcpy((void *)bites_data, (void *)ref->bites_data, ncolumns * nrows/8);
        }
    }

}  //____________________________________________________________________________

/** @fn void GBitmap::initRegion(const GBitmap* ref, int x0,int y0, int w, int h)
     @brief функция инициализации GBitmap регионом графического файла ref по заданным
   координатам
     */
void GBitmap::initRegion(const GBitmap *ref, int x0, int y0, int w, int h) {
    // cout_<<"start"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
    
    data_size=0;
    pack_flag=0;
    colorMode=1;
    colorModeOut=0;
    // normalisation
    if(x0 < 0) x0=0;
    if(y0 < 0) y0=0;
    if(x0 > ref->columns()) x0=0;
    if(y0 > ref->rows()) y0=0;
    if(w + x0 > ref->columns()) w=ref->columns() - x0;
    if(h + y0 > ref->rows()) h=ref->rows() - y0;
    unsigned char *p;
    if(this !=ref) {
        init(w, h, ref->colorMode);  //после инициализации битмап выровнен по 8
        // cout_<<"start1"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
        int            x, y;  // ( переменная d, 7 ноября 2010, быстрее классической).
        unsigned char *d;
        uint           refW=ref->columns();
        uchar *        refP=ref->bytes_data;

        for(y=0; y < h; y++) {
            d=bytes_data + y * ncolumns;
            for(x=0; x < ncolumns; x++) {
                p=d + x;
                *p=*(refP + (y + y0) * refW + x0 + x);
                // if(  pImg[0][y][x]>150){
                //   DS(0);
                //}else{
                //   DS(1);
                //};
            }  // DS(END);
        }
    }
    // cout_<<"done read"<<endl;
}  //_____________________________________________________________________________

void GBitmap::init(const GBitmap *ref, float scale, float rotation, int mode) {
    // cout<<"@@@_____scale="<<scale<<endl;
    if(this !=ref) {
        pack_flag=0;

        if(scale !=0 && rotation !=0) {
            rotateFast(ref, scale, rotation);
            return;
        }
        if(scale !=0 && mode==0) {
            scaleFast(ref, scale);
            return;
        }
        if(scale !=0 && mode==1) {
            bicubicScale(ref, scale);
            return;
        }
        if(rotation !=0) {
            rotateFast(ref, 1, rotation);
            return;
        }
        init(ref);
    }

}  //_____________________________________________________________________________
void GBitmap::initImage(const char *path) {
#ifdef MAGIC
    // InitializeMagick(*argv);

    // Construct the image object. Seperating image construction from the
    // the read operation ensures that a failure to read the image file
    // doesn't render the image object useless.
    Image image;
    try {
        // Read a file into image object
        image.read(path);
        int w=(int)image.columns();
        int h=(int)image.rows();
        image.type(GrayscaleType);
        Pixels view(image);
        // Crop the image to specified size (width, height, xOffset, yOffset)
        // image.crop( Geometry(100,100, 100, 100) );
        const Quantum *pixels=view.getConst(0, 0, w, h);

        // cout<<"Quantum:"<<sizeof(Quantum)<<" image.channels():"<<image.channels()<<"
        // QuantumRange:"<<QuantumRange<<endl;
        init(w, h, 1);
        int  index=0;
        uint step=(uint)image.channels();
        uint size=(uint)w * h * step;

        for(int i=0; i < size; i +=step) {
            uint c=pixels[i]/255;
            if(c > 255) c=255;
            bytes_data[index]=c;
            index++;
        }
    } catch(Exception &error_) {
        cout<<"Caught exception: "<<error_.what()<<endl;
        exit(0);
    }
#else
    initPNG(path);
#endif
}  //____________________________________________________________________________

// Fills a bitmap with the given value
// Заполняет растровое изображение заданным значением value

void GBitmap::fill(unsigned char value) {
    memset(bytes_data, value, data_size);

    //  for(unsigned int y=0; y<rows(); y++)
    //	{
    //	  unsigned char* bm_y=(*this)[y];
    //	  for(unsigned int x=0; x<columns(); x++)
    //		bm_y[x]=value;
    //	}

}  //_____________________________________________________________________________

// Рисование одного битмапа внутри другого по заданным координатам.

void GBitmap::drawImg(GBitmap *img, int x0, int y0) {
    int w=img->columns();
    int h=img->rows();
    // cout<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<"
    // nrows="<<nrows<<endl;
    int            x, y, d_;
    unsigned char *p;

    if(x0 > 0 && x0 + w <=ncolumns && y0 > 0 && y0 + h <=nrows) {  // рисование без проверки.

        unsigned char *d;  // (переменная d, 7 ноября 2010, быстрее классической).

        for(y=0; y < h; y++) {
            d_=(y0 + y) * ncolumns + x0;
            d=bytes_data + d_;
            for(x=0; x < w; x++) {
                p=d + x;
                *p=img[0][y][x];
            }
        }
        // cout_<<"+";
    } else {  // рисование с проверкой
        for(y=0; y < nrows; y++) {
            for(x=0; x < ncolumns; x++) {
                if(y - y0 < 0 || y - y0 > h || x - x0 < 0 || x - x0 > w) continue;
                d_=y * ncolumns + x;
                p=bytes_data + d_;
                *p=img[0][y - y0][x - x0];
                // cout_<<(short)*p/255;
            }  // cout_<<endl;
        }
        // cout_<<"-";
    }

    // cout_<<"draw done";
}  //___________________________________________________________________________

// Рисование одного битмапа внутри другого по заданным координатам и заданным режимом сложения
// значений пикселов.

void GBitmap::drawImg(GBitmap *img, int x0, int y0, int mode) {
    int w=img->columns();
    int h=img->rows();
    // cout_<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<"
    // nrows="<<nrows<<endl;

    int            x, y, d_;
    unsigned char *p;

    if(x0 > 0 && x0 + w <=ncolumns && y0 > 0 && y0 + h <=nrows) {  // рисование без проверки.

        unsigned char *d;  // (переменная d, 7 ноября 2010, быстрее классической).
        for(y=0; y < h; y++) {
            d_=(y0 + y) * ncolumns + x0;
            d=bytes_data + d_;
            for(x=0; x < w; x++) {
                p=d + x;
                if(mode==DRAW_BLACK) {
                    if(img[0][y][x]==0) *p=img[0][y][x];
                } else {
                    *p=img[0][y][x];
                }
            }
        }
        // cout_<<"+";
    } else {  // рисование с проверкой.
        for(y=0; y < nrows; y++) {
            for(x=0; x < ncolumns; x++) {
                if(y - y0 < 0 || y - y0 > h || x - x0 < 0 || x - x0 > w) continue;
                d_=y * ncolumns + x;
                p=bytes_data + d_;
                if(mode==DRAW_BLACK) {
                    if(img[0][y - y0][x - x0]==0) *p=img[0][y - y0][x - x0];
                } else {
                    *p=img[0][y - y0][x - x0];
                }
            }
        }
        // cout_<<"-";
    }
    // cout_<<"draw done";

}  //____________________________________________________________________________

void GBitmap::clearRegion(int x0, int y0, int w, int h) {
    // cout_<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" ncolumns="<<ncolumns<<"
    // nrows="<<nrows<<endl;

    if(x0 < 0) {
        w +=x0;
        x0=0;
    }
    if(x0 > ncolumns) return;
    if(y0 < 0) {
        h +=y0;
        y0=0;
    }
    if(y0 > nrows) return;

    if(x0 + w > ncolumns) w=ncolumns - x0;
    if(y0 + h > nrows) h=nrows - y0;

    if(w > ncolumns) w=ncolumns;
    if(h > nrows) h=nrows;

    int            x, y, d_;
    unsigned char *p, *d;  // (переменная d, 7 ноября 2010, быстрее классической).

    for(y=0; y < h; y++) {
        d_=(y0 + y) * ncolumns + x0;
        if(d_ > data_size || d_ < 0) continue;
        d=bytes_data + d_;
        for(x=0; x < w; x++) {
            p=d + x;
            *p=255;
        }
    }
    // cout_<<"draw done";

}  //____________________________________________________________________________

// Функция нормализации массива bytes_data по размеру ncolumns кратным 32.   // (29 ноября
// 2012) (26 февраля 2019).

void GBitmap::crop32() {
    // Получает новый массив bytes_data с новым размером по горизонтали ncolumns кратным 32 и
    // новым data_size=new_w*nrows; Массив с размером ncolumns кратным 32 необходим для работы
    // некоторых быстродействующих программ типа dotGain().

    //  * Особенности *
    // С правой стороны текста, в пределах 32 пикселей от края добавляется добавляется белая полоса
    // Если ncolumns является кратный 32, этого не происходит.

    // cout<<"Нормализация массива bytes_data по размеру ncolumns кратным 32 // Вызов функции
    // crop32()"<<endl;

    //*
    int w=ncolumns;       // прежний ncolumns не кратный 32.
    int new_w=(w/32) * 32;  // новый ncolumns кратный 32.

    if(new_w < w)new_w+=32; //расширяем изображение если исходное изображение обрезается
    imgW = w;

    // если ncolumns кратно 32 то сразу выходим из функции.
    if(new_w==w) return;

    uchar *newData=(uchar *)malloc(new_w * nrows);
    // Заполнение белым (255-белый), убирает черную полосу справа.
    memset(newData, 255, new_w * nrows);  // начало (указатель), значение, количество символов
                                          // в байтах (длина) unsigned.

    uchar *p0=bytes_data;
    uchar *p1=newData;
    uint nrows_=nrows-1;
    // -1 потому что memcpy использует буфер выровненный по int и последний ряд bytes_data
    // копируется не полностью

    for(int y=0; y < nrows_; y++) {
        memcpy(p1, p0, ncolumns);
        p0 +=w;
        p1 +=new_w;
    }

    free(bytes_data);
    // создание нового массива bytes_data с новым размером ncolumns и data_size.
    bytes_data=newData;
    ncolumns=new_w;
    data_size=new_w * nrows;
    bytes_per_row=new_w;

};  //____________________________________________________________________________

// Вывод на экран псевдографики массива битмап bytes_data.
void GBitmap::printToScreen() {
    int w=ncolumns;
    int h=nrows;
    cout<<"w="<<w<<" h="<<h<<endl;
    int            x, y;
    unsigned char *p, *d;  // (переменная d, 7 ноября 2010, быстрее классической).

    for(y=0; y < h; y++) {
        d=bytes_data + y * ncolumns;
        for(x=0; x < w; x++) {
            p=d + x;
            printf("%02x", *p);
            // if(*p>0){cout_<<"1";}else{cout_<<".";}
        }
        cout<<"<br>"<<endl;
    }

}  //____________________________________________________________________________

// Вывод на экран псевдографики массива битмап bytes_data.

void GBitmap::printToScreenBinary() {
    int w=ncolumns;
    int h=nrows;

    int            x, y;
    unsigned char *p, *d;  // (переменная d, 7 ноября 2010, быстрее классической).

    for(y=0; y < h; y++) {
        d=bytes_data + y * ncolumns;
        for(x=0; x < w; x++) {
            p=d + x;
            if(*p > 0) {
                cout<<"@";
            } else {
                cout<<".";
            }

            // if(*p>0){cout_<<"1";}else{cout_<<".";}
        }
        cout<<endl;
    }

}  //____________________________________________________________________________

// Инверсия входного массива битмап bytes_data с уровнями 0-255.

void GBitmap::invert() {
    // DM(END<<" TIME_START invert "<<END);
    // TIME_START

    // Новая версия. Инверсия входного массива битмап.
    // (изменено 7 ноября 2010, в 9 раз быстрее побайтовой классической).
    // (изменено 1 февраля 2018, 32р версия заменена на 64р версию с досчетом последних байт).

    // Инверсия уровней исходного распакованного массива черно-белого скана.
    // Например, инверсия общепринятого стандарта 0-черный 255-белый, во внутренний наглядный
    // формат программы 0-белый 255-черный. 255,255,255,255  0xFFFFFFFF      // 0,0,0,0
    // 0x00000000

    int w_h=data_size;  // размер массива   IMAGE_SIZE

    //*
    int w_h_64=w_h/sizeof(ulong);  // sizeof(ulong)=8
    ulong *bytes_data_64=(ulong *)bytes_data;  // приведение bytes_data к типу ulong

    // Инвертируем по 8 байт одновременно. Последний участок массива длинной от 0 до 8 байт не
    // инвертируется.
    for(int x=0; x < w_h_64; x++) {
        bytes_data_64[x]=~bytes_data_64[x];
    }
    // инверсия последнего участка массива длинной от 0 до 8 байт.
    int wh_=w_h_64 * sizeof(ulong);
    for(int x=wh_; x < w_h; x++) {
        bytes_data[x]=~bytes_data[x];
    }
    //*/

    /*
        uint* bytes_data_32=(uint*)bytes_data;      // приведение bytes_data к типу unsigned
       int. int w_h_32=w_h/sizeof(uint);                // sizeof(uint)=4

        // Инвертируем одновременно все 4 байта 32х разрядного слова.
        // Последний участок массива длинной от 0 до 4 байт не инвертируется.
        for ( int x=0; x < w_h_32; x++ ) {
            bytes_data_32[x]=~bytes_data_32[x]; //>>7 & 0x1010101 ^ invert  // где ^
       исключающее ИЛИ } // x
        //*/

    // Инвертируем по 1 байту.
    //// for (int x=0; x < w_h; x++){ bytes_data[x]=~bytes_data[x]; }  // 255=11111111

    // TIME_PRINT_

    /*
         // Инверсия уровней общепринятого стандарта 0-белый 255-черный, во внутренний формат
       программы 0-черный 1-белый.
         ///for ( x=0; x < w_h; x++ ){ bytes_data[x]=~bytes_data[x]; } // Инвертируем по 1
       байту медленно. int w_h_64=w_h/sizeof(ulong);  // sizeof(ulong)=8 ulong
       *bytes_data_64=(ulong*)bytes_data;    // приведение к ulong
         // Инвертируем и приводим к "1" одновременно по 8 байт. Последний участок массива
       длинной от 0 до 8 байт не инвертируется. for ( int x=0; x < w_h_64; x++ ) {
       bytes_data_64[x]=(~bytes_data_64[x]) & 0x101010101010101; } int
       wh_=w_h_64*sizeof(ulong);
         // обработка последнего участка массива длинной от 0 до 8 байт.
         for (int x=wh_; x<w_h; x++){ bytes_data[x]=(~bytes_data[x]) & 0x1; }
         //*/

}  //____________________________________________________________________________

void GBitmap::rotate(int rotation) {
}  //____________________________________________________________________________

// Функция стирает черное с краев скана на величину "dX" и "dY".
void GBitmap::eraseBorder(uint dX, uint dY) {
    // Протестированно на графике 23 марта 2019 г
    // проверить правый боковой бордюр, он меньше чем надо.
    if((dX + dY)==0) return;
    if(dX > ncolumns/2) return;
    if(dY > nrows/2) return;

    uint t=255;  // цвет заполнения бордера, 255 - белый.

    //стираем верхний бордюр
    memset(bytes_data, t, ncolumns * dY);
    //стираем нижний бордюр
    memset(bytes_data + (ncolumns * nrows - ncolumns * dY), t, ncolumns * dY);
    //стираем боковые бордюры
    int dX1=ncolumns - dX;
    dX *=2;
    int nrows_=nrows - dY;
    for(int i=0; i < nrows_; i++) {
        memset(bytes_data + ncolumns * i + dX1,
               t,
               dX);  // стирает сразу оба бордера: левый и правый.
        // long p=bytes_data+ncolumns*i+dX1 - bytes_data; cout<<"w*h="<<w*h<<",  p="<<p<<endl;
    }

}  //____________________________________________________________________________

// Функция заполняет значением "uint t" бордер с краев скана. Величина бордер "uint bord".
void GBitmap::variationBorder(uint t, uint bord) {
    //*
    // Протестированно на графике 23 марта 2019 г
    int w=ncolumns;  // ширина массива   IMAGE_WIDTH
    int h=nrows;     // высота массива   IMAGE_HEIGHT
    int w_h=w * h;     // размер массива   IMAGE_SIZE

    if(bord==0) return;
    if(bord > w/2) bord=w / 2;
    if(bord > h/2) bord=h / 2;

    // Заполнение border глобального массива bytes_data,
    // общепринятый формат массива 0-черный, 255-белый.
    uchar *p0=bytes_data;
    uchar *d0;
    uint   bord_w=bord * w;
    uint   bord_2=bord * 2;
    int    h_1=h - 1;

    // Заполнение верхнего горизонтального border.
    memset(bytes_data,
           t,
           bord_w);  // начало, значение, размер заполняемой части массива в байтах.

    // заполнение левого и правого вертикального border.
    p0=bytes_data - bord + w;     // + w начинаем со второй строки
    for(int y=0; y < h_1; y++) {  // h_1=h-1;
        d0=p0;  // сохраняем для выполнения цикла по "x"
        // цикл стирает сразу оба бордера: левый и правый.
        for(int x=0; x < bord_2; x++) {  // bord_2=bord*2;
            *d0=t;  // long p=d0-bytes_data; cout<<"w*h="<<w*h<<",  p="<<p<<endl;
            d0++;     // d0=bytes_data + y*w + x;
        }             // x
        p0 +=w;      // p0=bytes_data + y*w;
    }                 // y

    // заполнение нижнего горизонтального border
    memset(bytes_data + w_h - bord_w, t, bord_w);
    //*/

}  //_____________________________________________________________________________

// Упаковка большого битмапа в маленькй битовый массив.
// Используется для записи в формате TIFF.
// есть 32х битная реализация packImg32.

void GBitmap::packImg(int invert) {
    if(pack_flag) return;

    int          y, x, d;
    unsigned int s;
    unsigned int data_size_p=data_size/8;
    // data_size=ncolumns*nrows;  размер массива буквы (букв).
    // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв).

    // инициализация битового массива char размером  data_size_p=data_siz/8. Нужно применить
    // calloc
    bites_data=(unsigned char *)malloc(data_size_p);
    // обнуление bites_data
    memset(bites_data, 0, data_size_p);
    // выход в случае отсутствия свободной памяти запрашиваемого размера.
    // if (bites_data==NULL) {DC("bites_data"); exit (1);}

    //автоопределение количества градаций в битмапе.
    int max=0;
    for(x=0; x < data_size; x++) {
        s=bytes_data[x];
        if(s > max) max=s;
    }

    // бинаризация входного массива битмап. Нужно применить в 8 раз болеее буструю функцию
    // binarisation64().
    if(max > 1) {
        for(x=0; x < data_size; x++) {
            if(invert) {
                if(bytes_data[x] > 127) {
                    bytes_data[x]=0;
                }  // уровни в битмапе 0 и 255
                else {
                    bytes_data[x]=1;
                }  // уровни в битовом массиве 1 и 0
            } else {
                if(bytes_data[x] > 127) {
                    bytes_data[x]=1;
                }  // уровни в битмапе 0 и 255
                else {
                    bytes_data[x]=0;
                }  // уровни в битовом массиве 1 и 0
            }
        }
    }
    /**/

    // ЗАПАКОВКА.
    // Запаковка стандартного большого байтового массива битмап
    // (в одном байте 1 бит) в маленькй битовый массив (в одном байте 8 бит).
    unsigned int reg=0;
    for(y=0; y < data_size_p; y++) {  // >>3  деление на 8М
        d=y * 8;  // <<3  умножение на 2 в 3 степени=8.
        for(x=0; x < 8; x++) {
            reg=(reg<<1) + bytes_data[d + x];  // обратный порядок пикселейМ
            /// s=(bytes_data[d + x])<<7; // <<31 умножение на 2 в 31 степени, >> 1 деление
            /// на 2.
            ///	reg=s + (reg >> 1);       // прямой порядок пикселей.
        }                     // x
        bites_data[y]=reg;  // заполнение маленького выходной битового массива char.
    }                         // y
    /**/

    free(bytes_data);  // освобождение большого массива битмапМ

    // 1 запакован, каждый бит всех байтов маленького (char) массива заполнен битами.
    pack_flag=1;

}  //_____________________________________________________________________________

// Распаковка маленького битового массива в большй битмап.
// Используется для увеличения скорости доступа при обработке картинок букв.
// есть 32х битная реализация unpackImg32

void GBitmap::unpackImg(int invert) {
    // cout_<<"pack_flag"<<pack_flag<<" data_size="<<data_size<<endl;

    if(!pack_flag) return;
    if(pack_flag==BITMAP_32) {
        unpackImg32V(0);
        return;
    }

    int          y, x, d;
    unsigned int s;
    unsigned int data_size_p=data_size/8;

    // инициализация большого массива (битмап) bytes_data  полного размера data_siz.
    bytes_data=(unsigned char *)malloc(data_size);

    // РАСПАКОВКА.
    // Распаковка маленького битового массива (в одном байте 8 бит) в
    // большй стандартный байтовый массив битмап (в одном байте 1 бит).
    for(y=0; y < data_size_p; y++) {  // >>3  деление на 8.
        s=bites_data[y];
        d=y * 8;
        for(x=0; x < 8; x++) {
            //(bytes_data[d + x])=s & 1;     s=(s >> 1); // прямой порядок пикселей.
            if(invert) {
                bytes_data[d + 8 - x - 1]=255 - 255 * (s & 1);
                s=(s >> 1);  // обратный порядок пикселей.
            } else {
                bytes_data[d + 8 - x - 1]=255 * (s & 1);
                s=(s >> 1);  // обратный порядок пикселей.
            }
            // уровни в битмапе  0 и 255.
        }  // x
    }      // y

    free(bites_data);  // освобождение маленького массива.

    // распакован, один бит каждого байта большого массива (битмап) заполнен битами.
    pack_flag=0;

}  //_____________________________________________________________________________

void GBitmap::put(int x, int y, int color) {
    if(colorMode==4) {
        int *p=(int *)(bytes_data + (x + y * ncolumns) * 4);
        *p=color;
    }
    if(colorMode==1) { bytes_data[x + y * ncolumns]=color; }
};

int GBitmap::get(int x, int y) {
    if(colorMode==4) {
        int *p=(int *)(bytes_data + (x + y * ncolumns) * 4);
        return *p;
    }
    if(colorMode==1) {
        unsigned char *p=(unsigned char *)(bytes_data + x + y * ncolumns);
        return *p;
    }
    return 0;
};

//_____________________________________________________________________________

/**  Убирает белый фон вокруг изображения. */
GBitmap *GBitmap::detectRegion(int frame, int *x0, int *x1, int *y0, int *y1, int mode) {
    *x0=-1, *y0=-1, *x1=-1, *y1=-1;

    for(int x=0; x < ncolumns; x++) {
        for(int y=0; y < nrows; y++) {
            if(*(bytes_data + y * ncolumns + x) < 128) {
                *x0=x;
                break;
            }
        }
        if(*x0 > 0) break;
    }

    for(int x=ncolumns - 1; x > -1; x--) {
        for(int y=0; y < nrows; y++) {
            if(*(bytes_data + y * ncolumns + x) < 128) {
                *x1=x;
                break;
            }
        }
        if(*x1 > 0) break;
    }

    for(int y=0; y < nrows; y++) {
        for(int x=0; x < ncolumns; x++) {
            if(*(bytes_data + y * ncolumns + x) < 128) {
                *y0=y;
                break;
            }
        }
        if(*y0 > 0) break;
    }

    for(int y=nrows - 1; y > -1; y--) {
        for(int x=0; x < ncolumns; x++) {
            if(*(bytes_data + y * ncolumns + x) < 128) {
                *y1=y;
                break;
            }
        }
        if(*y1 > 0) break;
    }

    *x0 -=frame + 1;  // if(*x0<0)*x0=0;
    *y0 -=frame + 1;  // if(*y0<0)*y0=0;
    *x1 +=frame + 1;  // if(*x1>=ncolumns)*x1=ncolumns-1;
    *y1 +=frame + 1;  // if(*y1>=nrows)*y1=nrows-1;

    if(mode) {
        GBitmap *img=GBitmap::createRegion(this, *x0, *y0, *x1 - *x0, *y1 - *y0);
        return img;
    } else {
        return NULL;
    }

}  //_____________________________________________________________________________

  // ПОЛИГРАФИЧЕСКОЕ РАСТИСКИВАНИЕ. //
  
  //( изменено: 25 октября 2010, 13 декабря 2012, 27 февраля 2019 ).//
  
  /**\brief  Имитация полиграфического растискивания и зашумленности.
   НА ВХОДЕ В ФУНКЦИЮ ncolumns ДОЛЖЕН БЫТЬ КРАТНЕН 32.
   Запуск функции нормализации crop32(); должен происходить в самом начале блока функций графической предобработки.
   */
  void GBitmap::dotGain(int gRateLefts, int gRateRight,
                        int gRateHighs, int gRateLower,  int dot_gain){
    
    /**\ dot_gain!=0 число добавленных (-dot_gain убавленных) однопиксельных слоёв вокруг символа */
    
    /**\ Подпрограмма представляет собой пространственный фильтр с импульсной
     характеристикой (реакция пространственного фильтра на один pix) в виде квадрата
     размером gainRate x gainRate. */
    
    /**\ При инициализации массива bytes_data необходимо обязательно выровнить по 32
     nrows и ncolumns,   nrows = (arows/32+1)*32;   ncolumns =(acolumns/32+1)*32;.
     Например запустить функцию нормализации crop32();, которая делает массив bytes_data
     по горизонтали кратным 32 ( размер ncolumns кратен 32). */
    
    /**\ Два способа задания полиграфического растискивания:
     - dot_gain!=0  (например dot_gain=3;). Добавляет dot_gain однопиксельных слоёв вокруг штрихов
     (-dot_gain убавляет) и блокирует использование gRateLefts, gRateRight, gRateHighs, gRateLower.
     К каждой стороне штриха добавляется dot_gain/2 однопиксельных слоёв, плюс к одной стороне добавляется
     остаток dot_gain%2.
     dot_gain=0 блокирует свое собственное влияние и можно использовать gRateLefts, gRateRight, gRateHighs, gRateLower.
     - классический способ с помощью индивидуальной установки gRateLefts, gRateRight, gRateHighs, gRateLower.
     При этом способе слои можно добавлять ассиметроично по вертикали и горизонтали.
     Для этого устанавливаем dot_gain=0.
     
     Например, типичный способ получения полиграфического растискивания двумя запусками dotGain
     убираем со всех сторон по пикселю, съедаем мелкие детали, которые не надо растискивать.
     первый способ:  dotGain(0,0,0,0,-2);      второй способ:  dotGain(-1,-1,-1,-1, 0);
     добавляем со всех сторон символа по 2 пикселя (нескольку пикселей)
     первый способ:  dotGain(0,0,0,0, 4);      второй способ:  dotGain(2, 2, 2, 2, 0);
     Причем gRateLefts или gRateRight или gRateHighs или gRateLower == dot_gain/2
     Т.е. это половинки от dot_gain.
     Количество добавленных однопиксельных слоёв вокруг символа от 1 до 31, убавленных от -1 до -31, без изменений 0.*/
    
    //------------------------------------------------------------------------------
    
    // Если размер ncolumns не кратен 32 (остаток от деления на 32) > 0.
    if( ncolumns%32>0 ) {
      cout<<"НА ВХОДЕ В ФУНКЦИЮ dotGain() ncolumns ДОЛЖЕН БЫТЬ КРАТНЕН 32, ncolumns="<<ncolumns<<endl;
    }
    
    //------------------------------------------------------------------------------
    
    int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT) cout<<"Функция полиграфического растискивания "<<endl;
    if(GrafT) cout<<" ncolumns "<<ncolumns<<" nrows "<<nrows<<endl;
    
    
    if( dot_gain==0 && gRateLefts==0 && gRateRight==0 && gRateHighs==0 && gRateLower==0 ) return;
    
    TIME_START
    
    int x;
    unsigned int s0,s1;
    
    
    // dot_gain это число добавленных (-dot_gain убавленных) однопиксельных слоёв вокруг символа.
    // пересчет dot_gain на на левую, правую стороны и верх, низ символа.
    // dot_gain=0 блокирует свое влияние и можно использовать gRateLefts, gRateRight, gRateHighs, gRateLower.
    if (dot_gain !=0) {
      // ограничение на число добавленных (убавленных) однопиксельных слоёв
      if(dot_gain>63) dot_gain=63;   if(dot_gain<-63) dot_gain=-63;
      int dg=dot_gain/2;         ///cout<<"dot_gain="<<dot_gain<<"   dg="<<dg;  // division
      gRateLefts=dg;  gRateRight=dg;  gRateHighs=dg;  gRateLower=dg;
      dg=dot_gain%2;             ///cout<<"    dg="<<dg<<endl;                  // modulo
      gRateLefts+=dg;  gRateHighs+=dg;
    }
    ///cout<<" +dot_gain="<<dot_gain<<endl;
    ///cout<<"+gRateLefts="<<gRateLefts<<"  gRateRight="<<gRateRight<<"  gRateHighs="<<gRateHighs<<"  gRateLower="<<gRateLower<<endl;
    
    
    /// gRateLefts=2;  gRateRight=2;  gRateHighs=2;  gRateLower=2; // ТЕСТ
    
    
    //*
    // количество добавленных однопиксельных слоёв с левой стороны символа
    if(gRateLefts>31) gRateLefts=31;   if(gRateLefts<-31) gRateLefts=-31;
    // количество добавленных однопиксельных слоёв с правой стороны символа
    if(gRateRight>31) gRateRight=31;   if(gRateRight<-31) gRateRight=-31;
    // количество добавленных однопиксельных слоёв с верхней стороны символа
    if(gRateHighs>31) gRateHighs=31;   if(gRateHighs<-31) gRateHighs=-31;
    // количество добавленных однопиксельных слоёв с нижней стороны символа
    if(gRateLower>31) gRateLower=31;   if(gRateLower<-31) gRateLower=-31;
    //*/
    
    //cout<<"Полиграфическое растискивание dotGain()  g1, g2, g3, g4,  dot_gain="
    
    
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
    /// cout<<" gainRate "<<gainRate<<endl;
    
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
    
    
    
    // РАСПАКОВКА UNPACKING 32. //     time=0.01
    // вызов функции распаковки маленького битового массива int в большй битмап.
    // invert=0 без инверсии черного и белого, invert=1 с инверсией черного и белого.
    unpackImg32(0);
    //invert();
    
    // if(pack_flag)unpackImg32(0);
    
    
    //******************************************************************************
    
    // работающая исходная версия.
    // сдвиг влево, вправо всего массива data_size_p32, как единого регистра.
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
    
    
    if(GrafT) cout<<" gRateLefts "<<gRateLefts<<" gRateRight "<<gRateRight <<" gRateHighs "<<gRateHighs<<" gRateLower "<<gRateLower<<endl;
    
    if(GrafT) { TIME_PRINT_ }
    
    
  }//_____________________________________________________________________________
  

// УПАКОВКА  PACKING 32. //
/**\ Упаковка большого битмапа в маленькй int массив. Массив bites_data из 32х разрядных слов
   упакован битово. Используется для функции имитация полиграфического растискивания и
   зашумленности.*/

// При переносе в основную версию использовать эту функцию packImg32 вместо
// функции packImg.   САА 3 ноября 2010 года.

void GBitmap::packImg32(int invert) {
    if(pack_flag || !data_size) return;

    int y, x, d;
    // int print=0;

    // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв).
    // data_size=ncolumns*nrows;  размер большого битмапа.

    ////cout<<" TIME_START packImg32 /P32/ pack_flag="<<pack_flag<<"
    ///ncolumns="<<ncolumns<<endl;

    data_size_p=data_size /
                  8;  /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения.
    data_size_p32=
        data_size /
        32;  /// размер упакованного массива integer. 1 integer - 32 пиксела изображения.

    // инициализация битового массива char размером  data_size_p=data_siz/8 // НАДО ПРИМЕНИТЬ
    // calloc
    bites_data=
        (unsigned char *)malloc(data_size_p + 32);  // +ncolumns  // было +8 это ошибка.
    // обнуление bites_data                             // +ncolumns
    memset(bites_data, 0, data_size_p + 32);
    // выход в случае отсутствия свободной памяти запрашиваемого размера.
    // if (bites_data==NULL) {DC("bites_data"); exit (1);}

    // приведенный к unsigned int исходный упакованный массив черно-белого скана.
    unsigned int *bites_data_32=(unsigned int *)bites_data;
    /**/

    // автоопределение количества градаций в битмапе с управляемой инверсией.
    //  invert=0;  без инверсии (обычно белый фон)
    //  invert=1;  с инверсией (обычно черный фон)
    AutodetectGradations(invert);

    // Классический вариант. //
    // Запаковка стандартного большого байтового массива битмап.
    // (в одном байте 1 бит) в маленькй  массив int (в одном int 32 бита).

    unsigned int reg;  // unsigned int dirRev=31; // direct, reverse. dir_rev.
    for(y=0; y < data_size_p32; y++) {  // >>5  деление на 5  data_size/32.
        d=y * 32;
        reg=0;  // <<5  умножение на 2 в 5 степени=32.
        for(x=0; x < 32; x++) {
            ///	 ///  		 reg=(reg<<1) + bytes_data[d+x]; // обратный порядок пикселей.
            //////////////
            // reg=bytes_data[d+x] + (reg<<1); // обратный порядок пикселей.
            reg=(bytes_data[d + x]<<31) +
                  (reg >> 1);  // прямой порядок пикселей. ///////////
        }  // x      // <<31 умножение на 2 в 31 степени,  >> 1 деление на 2.
        bites_data_32[y]=reg;  // заполнение маленького выходного массива int.
    }                            // y

    /**/

    free(bytes_data);  // освобождение исходного большого массива битмап.

    // 1 если массив bites_data из 32х разрядных слов упакован побитово.
    pack_flag=BITMAP_32;
    /**/

}  //_____________________________________________________________________________

// РАСПАКОВКА UNPACKING 32. //
/**\ Распаковка маленького битового массива int в большй битмап.
     Используется для функции имитация полиграфического растискивания и зашумленности.*/

// При переносе в основную версию использовать эту функцию unpackImg32 вместо
// функции unpackImg.   САА 3 ноября 2010 года.

void GBitmap::unpackImg32(int invert) {
    if(pack_flag !=BITMAP_32) return;

    int          y, x, d;
    unsigned int s;
    // int print=0;

    int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)
                    // if (GrafT) {  }

    if(GrafT) { cout<<" TIME_START unpackImg32 /UP32/ 0="<<0<<endl; }

    if(GrafT)
        cout<<"  data_size="<<data_size<<"  data_size_p32="<<data_size_p32
            <<"  ncolumns="<<ncolumns<<endl;

    // инициализация большого массива (битмап) bytes_data полного размера data_siz. // НАДО
    // ПРИМЕНИТЬ calloc
    bytes_data=(unsigned char *)malloc(data_size);
    // обнуление bytes_data.
    memset(bytes_data, 0, data_size);
    // выход в случае отсутствия свободной памяти запрашиваемого размера.
    // if (bytes_data==NULL) {DC("bytes_data"); exit (1);}

    // приведенный к unsigned int упакованный битовый массив черно-белого скана.
    // маленький запакованный массив
    unsigned int *bites_data_32=(unsigned int *)bites_data;

    // приведенный к unsigned int распакованный байтовый массив черно-белого скана.
    // большой распакованный массив
    unsigned int *bytes_data_32=(unsigned int *)bytes_data;
    /**/

    // РАСПАКОВКА
    // Распаковка маленького массива int (в одном int 32 бита) в большй стандартный
    // байтовый массив битмап (в одном байте 1 бит) с управляемой инверсией.
    //*
    // Более быстрый вариант. //
    // (изменено 8 ноября 2010, в 1,3 раза быстрее классической).
    // invert=0; // без инверсии (белый фон), invert=1 с инверсией (обычно черный фон).
    if(invert < 1) { invert=0; }
    if(invert > 0) { invert=~0; }

    // По словная запись в массив bytes_data (по 4 байта одновременно).
    unsigned int s0;
    for(y=0; y < data_size_p32; y++) {  // >>5  деление на 32  // data_size/32
        s=bites_data_32[y];
        d=y * 8;                // 32/4
        for(x=0; x < 8; x++) {  // 32/4

            // прямой порядок пикселей     // bytes_data[d + x]=s & 1;  s=(s >> 1);
            s0=0x000000FF * (s & 1);
            s >>=1;  // заполнение 00000000 или 11111111 1ого байта 255*
            s0 |=0x0000FF00 * (s & 1);
            s >>=1;  // заполнение 00000000 или 11111111 2ого байта 255*<<8;
            s0 |=0x00FF0000 * (s & 1);
            s >>=1;  // заполнение 00000000 или 11111111 3его байта 255*<<16;
            // заполнение 00000000 или 11111111 4ого байта с управляемой инверсией. 255*<<24
            bytes_data_32[d + x]=(0xFF000000 * (s & 1) | s0) ^ invert;
            s >>=1;
            // запись в массив bytes_data одним 32р словом 4х байт одновременно

            // обратный порядок пикселей // bytes_data[d + 32-x-1]=255*(s & 1);  s>>=1;
            //			  s0=255*(s & 1)<<24;   s>>=1; // заполнение 00000000 или 11111111 4ого
            //байта 			  s0|=255*(s & 1)<<16;  s>>=1; // заполнение 00000000 или 11111111 3его байта
            //			  s0|=255*(s & 1)<<8;   s>>=1; // заполнение 00000000 или 11111111 2ого
            //байта
            // заполнение 00000000 или 11111111 1ого байта с управляемой инверсией.
            //			  bytes_data_32[d + 8-x-1]=(255*(s & 1) | s0) ^ invert;   s>>=1;
            // запись в массив bytes_data одним 32р словом 4х байт одновременно

        }  // x     // уровни в битмапе  0-чёрный и 255-белый.
    }      // y      // s>>=1; деление на 2 с присваиванием.
           //*/

    /*
         // Классический вариант. //
         //invert=0; // без инверсии (белый фон), invert=1 с инверсией (обычно черный фон).
         if(invert<1) { invert=0;}   if(invert>0) { invert=1;}

         // По байтная запись массив bytes_data. Классический вариант.
         for( y=0; y < data_size_p32; y++ ){  // >>5  деление на 32  // data_size/32
         s=bites_data_32[y];   d=y*32;
         for( x=0; x < 32; x++ ){
         //bytes_data[d + x]=s & 1;     s=(s >> 1);        // прямой порядок пикселей.
         //bytes_data[d + x]=255*(s & 1 ^ invert);   s=(s >> 1);  // прямой порядок пикселей
       ./////////// bytes_data[d + x]=255*(s & 1 ^ invert);   s>>=1;  // прямой порядок
       пикселей. ///////////
         //bytes_data[d + 32-x-1]=255*(s & 1); s=(s >> 1); // обратный порядок пикселей.
       /////////// } // x     // уровни в битмапе  0-чёрный и 255-белый. } // y         //
       s>>=1; деление на 2 с присваиванием.
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
         s0=(s & 1);       s>>=1;  // прямой порядок пикселей ///////////s0=s & 1;   s>>=1;  //
       прямой порядок пикселей /////////// s0|=(s & 1)<<8;   s>>=1;  // прямой порядок пикселей
       /////////// s0|=(s & 1)<<16;  s>>=1;  // прямой порядок пикселей /////////// s0|=(s &
       1)<<24;  s>>=1;  // 255*
         ///bytes_data_32[d + x]=(255*(s & 1)<<24 | s0) ^ invert;    s>>=1;  // прямой порядок
       пикселей ///////////
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
         //s0=255*(s & 1);       s>>=1;  // прямой порядок пикселей ///////////s0=s & 1;
       s>>=1;  // прямой порядок пикселей /////////// s0=(s & 1);      s>>=1; binaryPrint(s0,
       2);  DM(" "); DM(END); s0|=(s & 1)<<8;   s>>=1;  // прямой порядок пикселей ///////////
       255* binaryPrint(s0, 2);  DM(" "); DM(END); s0|=(s & 1)<<16;  s>>=1;  // прямой порядок
       пикселей ///////////  255* binaryPrint(s0, 2);  DM(" "); DM(END); s0|=(s & 1)<<24;
       s>>=1;  // прямой порядок пикселей ///////////   255* binaryPrint(s0, 2);  DM(" ");
       DM(END);
         //s0=s0 ^ invert; //255*
         // одновременное умножение 4х байт на 255 с управляемой инверсией.
         ///s0=((s0<<7)-s0+(s0<<7)) ^ invert; //255*
         s0=((s0<<8)-s0) ^ invert; //255*  +(s0<<7)
         binaryPrint(s0, 2);  DM(" "); DM(END);
         //bytes_data_32[d + x]=s0; //255*   ^ invert
         //*/
    //------------------------------------------------------------------------------

    /**/
    free(bites_data);  // освобождение маленького массива.
    bites_data=NULL;
    pack_flag=0;

    //		DM(END<<" data_size "<<data_size<<" ncolumns*nrows "<<ncolumns*nrows<<END);

}  //_____________________________________________________________________________

// АВТООПРЕДЕЛЕНИЕ ГРАДАЦИЙ В БИТМАПЕ. //
/**\ автоопределение количества градаций в битмапе с управляемой инверсией.
     Используется для приведения битмапа в состояние: младший бит байта 0-черный, 1-белый.
     Входной битмап м.б. 255-белый (напрямер 255-tif, 208-jpeg), 0-черный.*/

void GBitmap::AutodetectGradations(int invert) {
    unsigned int x, s;

    //  * Особенности *
    // Не работает если в черно-белом битмапе: байт 0-черный, байт 255-белый ( 11111111 ).
    // Ничего не возвращает.

    int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)
                    // if (GrafT) {  }

    if(GrafT) {
        cout<<"  TIME_START AutodetectGradations   invert="<<invert<<endl;
        cout<<"  data_size="<<data_size<<"  ncolumns*nrows="<<ncolumns * nrows<<endl;
    }

    TIME_START

    // TEST.//
    // for ( x=0; x < data_size; x++ ) { bytes_data[x]=1; }
    // 0xFEFEFEFE=~(1+(1<<8)+(1<<16)+(1<<24));  0xFEFEFEFE=~(16843009=0x1010101)
    // binaryPrint(0xFEFEFEFE, 2);  DM(" "); DM(END);

    /// быстрое автоопределение и бинаризация входного массива битмап ///
    // (изменино 31 октября 2010, в 6-9 раз быстрее классической).//
    //*
    // приведенный к unsigned int исходный распакованный массив черно-белого скана
    unsigned int *bytes_data_32=(unsigned int *)bytes_data;

    // автоопределение количества градаций в битмапе. // time=0.0042
    // алгоритм //
    // bytes_data[x]=0 или 1, то s всегда=0  битмап уже бинаризован.
    // bytes_data[x]>1,       то s всегда >0  битмап надо бинаризовать.
    // маской убираем младший бит во всех 4х байтах 32х разрядного слова в этих
    // младших битах находятся 0 или 1 возможно бинаризованного битмапа. Если в
    // массиве встречается серое (или чб) значит оставшиеся биты составляют число
    // больше ноля, то останавливаем цикл и далее выполняем бинаризацию битмапа.

    unsigned int b_data=data_size/4 - 8;
    s=0;
    for(x=8; s < 1 && x < b_data; x++) {  // эквивалентно if(s>0) {break;}
        s=bytes_data_32[x] & 0xFEFEFEFE;
    }  // x  // в бинаризации д.б. s>0;

    // int graduation=0;
    // if(s>0) graduation=1; // если в битмапе встречается серое надо применять адаптивный
    // порог. graduation=0;         // если битмап уже бинаризован. return graduation;

    /// быстрая инверсия входного массива битмап. ///
    //*
    // invert=1;  // TEST
    // invert=0 без инверсии (белый фон), invert=1 с инверсией (обычно черный фон).
    if(invert < 1) { invert=0; }
    if(invert > 0) { invert=~0; }  // ~0=1111111111111111

    // бинаризация входного массива битмап с управляемой инверсией параметром invert.
    // алгоритм.//
    // Маской выделяем младший бит во всех 4х байтах 32х разрядного слова
    // в этих младших битах находятся искомые 0 или 1 уже бинаризованного битмапа.

    b_data=data_size/4;
    if(s > 0) {  // s>4
        for(x=0; x < b_data; x++) {
            bytes_data_32[x]=((bytes_data_32[x] >> 7) & 0x1010101) ^
                               invert;  // >>7 деление на 128 // добавлены скобки 1 февр 2019
        }                               // x  // ^ исключающее ИЛИ
    }                                   // if
    //*/
    // Получили выходной битмап с уровнями:   0-черный    1-белый.

    if(GrafT) cout<<"  x="<<x<<"  s="<<s<<"   invert="<<invert<<endl;

    if(GrafT) { TIME_PRINT_ }

    //------------------------------------------------------------------------------

    /*
         // Классический вариант.//
         /// классическое автоопределение и бинаризация входного массива битмап. ///
         //автоопределение количества градаций в битмапе.
         int max=0;
         for ( x=0; x < data_size; x++ ) {
         s=bytes_data[x];
         if ( s > max ) max=s;
         }

         // бинаризация входного массива битмап.
         if(max>1){
         for ( x=0; x < data_size; x++ ) {
         if(invert){
         if ( bytes_data[x]>127 ){ bytes_data[x]=0; } // уровни в битмапе 0 и 255.
         else { bytes_data[x]=1; }  // уровни в битовом массиве 1 и 0.
         }else{
         if ( bytes_data[x]>127 ){ bytes_data[x]=1; } // уровни в битмапе 0 и 255.
         else { bytes_data[x]=0; }  // уровни в битовом массиве 1 и 0.
         }
         }
         }
         //*/

    //------------------------------------------------------------------------------

}  //_____________________________________________________________________________

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
    
    // уменьшение площади сканирования графического текста,
    // если площадь графического текста больше 1 млн.pix то прыгоем по пикселям через n.
    int n=data_size/1000000+1;         if(n>128) n=128;
    //cout<<"n="<<n<<endl;
    
    
    unsigned int x1=data_size/8;  unsigned int x2=data_size-data_size/8;
    for ( x=x1; gr<1 && x<x2; x+=n ) {  // эквивалентно if(s>0) {break;}
      s=bytes_data[x];   // s=*(p0+x);
      // проверка наличия серого в битмапе.
      if( s>0 && s<255 ) { gr=1;} // gr++;  примерное количество серых пикселей в тексте.
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

inline unsigned int GBitmap::rows() const {
    return nrows;
}

inline unsigned int GBitmap::columns() const {
    return ncolumns;
}

inline unsigned int GBitmap::borderSize() const {
    return border;
}

inline unsigned int GBitmap::rowsize() const {
    return bytes_per_row;
}

inline unsigned char *GBitmap::operator[](int row) {
    if(row < 0 || row >=nrows) { return bytes_data; }
    return &bytes_data[row * ncolumns];
}

inline const unsigned char *GBitmap::operator[](int row) const {
    if(row < 0 || row >=nrows) { return bytes_data; }
    return &bytes_data[row * ncolumns];
}

inline GBitmap *GBitmap::operator=(const GBitmap *ref) {
    init(ref);
    return this;
}

/**\brief Save GBitmap contents into the PNG file accordind to color mode and bit depth */
void GBitmap::savePNG(const char *path) {
    // packImg32V(0);

    unsigned       error;
    unsigned char *png;
    size_t         pngsize;
    LodePNGState   state;

    lodepng_state_init(&state);
    if(colorModeOut==BITMAP_32 || colorModeOut==0) {
        state.info_png.color.colortype=LCT_GREY;
        state.info_png.color.bitdepth=1;

        state.info_raw.colortype=LCT_GREY;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==1) {
        state.info_png.color.colortype=LCT_GREY;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_GREY;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==3) {
        state.info_png.color.colortype=LCT_RGB;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_RGB;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==4) {
        state.info_png.color.colortype=LCT_RGBA;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_RGBA;
        state.info_raw.bitdepth=8;
    }

    state.encoder.auto_convert=
        0;  // we specify ourselves exactly what output PNG color mode we want

    error=lodepng_encode(&png, &pngsize, bytes_data, ncolumns, nrows, &state);
    if(!error) lodepng_save_file(png, pngsize, path);

    /*if there's an error, display it*/
    if(error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        exit(0);
    }

    lodepng_state_cleanup(&state);
}

void GBitmap::strPNG(string &data) {
    unsigned       error;
    unsigned char *png;
    size_t         pngsize;
    LodePNGState   state;

    lodepng_state_init(&state);
    if(colorModeOut==BITMAP_32 || colorModeOut==0) {
        state.info_png.color.colortype=LCT_GREY;
        state.info_png.color.bitdepth=1;

        state.info_raw.colortype=LCT_GREY;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==1) {
        state.info_png.color.colortype=LCT_GREY;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_GREY;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==3) {
        state.info_png.color.colortype=LCT_RGB;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_RGB;
        state.info_raw.bitdepth=8;
    } else if(colorModeOut==4) {
        state.info_png.color.colortype=LCT_RGBA;
        state.info_png.color.bitdepth=8;

        state.info_raw.colortype=LCT_RGBA;
        state.info_raw.bitdepth=8;
    }

    state.encoder.auto_convert=
        0;  // we specify ourselves exactly what output PNG color mode we want.

    error=lodepng_encode(&png, &pngsize, bytes_data, ncolumns, nrows, &state);

    /*if there's an error, display it*/
    if(error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        exit(0);
    }

    data.resize(pngsize);
    memcpy(&data[0], png, pngsize);

    lodepng_state_cleanup(&state);
}

/**/
};  // namespace ocr
//******************************************************************************
