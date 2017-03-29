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

/* GBitmap32 это один из способов упаковки данных в GBitmap, при котором в одном int32 записано 32 пиксела черно-белого изображения.
 GBitmap32 Используется для определения степени перекрытия распознаваемой буквы и исходного изображения в GBitsetMatrix,
 GBitmap32 как и GBitsetMatrix в этом случае транспонирован по вертикали (x и y поменялись местами) и изображение записано в нем 
 вертикальными колонками шириной по 32 пиксела. В отличие от GBitsetMatrix в GBitmap записано равное количество пикселов изображения и битов данных.
 Транспонирование по вертикали позволяет копировать вертикальные участки изображения по 32 пиксела шириной с помощью команды memcpy и подсчитывать
 количество белых пикселов. В Запакованном  GBitmap32 в одном байте записано 8 пикселов и 4 байта образуют 32 последовательных пиксела изображения. 
 В следующих 32 битах записаны 32 пиксела изображения расположенные на один пиксел ниже предыдущих (транспонированный массив) */

// УПАКОВКА  PACKING 32V //
/** упаковки данных в GBitmap, при котором в одном int32 записано 32 пиксела черно-белого изображения.
 GBitmap32 как и GBitsetMatrix в этом случае транспонирован по вертикали (x и y поменялись местами) и изображение записано в нем
 вертикальными колонками шириной по 32 пиксела*/

void GBitmap::packImg32V(int invert){
    
    if(pack_flag||!data_size)return;
    
    int y,x,d;
    int print=0;
    
    // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв)
    //data_size=ncolumns*nrows;  //размер большого битмапа
    
    
    DR(END<<" TIME_START packImg32 /P32/ 0="<<0<<" ncolumns="<<ncolumns<<" nrows="<<nrows<<endl)
    
    data_size_p=data_size/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
    data_size_p32=data_size/32-32; /// размер упакованного массива integer. 1 integer - 32 пиксела изображенияc /@#@
    
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
    // при упаковке массив транспонируется, данные располагаются колонками, а не рядами
    
    unsigned int reg;
    uint stepX=0,stepY=0;
    for( y=0; y < data_size_p32; y++ ){
        reg=0;
        d=ncolumns*stepX+stepY;  stepX++; if(stepX==nrows){stepX=0;stepY+=32;}
        for ( x=32; x>0; x--) {
            ///	reg=(reg << 1) + bytes_data[d+x]; // обратный порядок пикселей ///////////
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
/**\ Распаковка GBitmap32 в GBitmap.*/

void GBitmap::unpackImg32V(int invert){
    //cout_<<"pack_flag32"<<pack_flag32<<" data_size="<<data_size<<endl;
    
    if(!pack_flag)return;
    int print=0;
    
    DR(END<<" TIME_START unpackImg32V /UP32V/ 0="<<0<<endl);
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
    uint s0,s,d;
    uint y=0,x,step=0,stepY=0;
    for( y=0; y < data_size_p32; y++ ){
        s=bites_data_32[y];   d=ncolumns/4*step+stepY;  step++; if(step==nrows){step=0;stepY+=8;}
        for( x=0; x < 8; x++ ){ // 32/4
            /*
             // прямой порядок пикселей     // bytes_data[d + x]=s & 1;  s=(s >> 1);
             s0 =0x000000FF*(s & 1); s>>=1; // заполнение 00000000 или 11111111 1ого байта 255*
             s0|=0x0000FF00*(s & 1); s>>=1; // заполнение 00000000 или 11111111 2ого байта 255*<<8;
             s0|=0x00FF0000*(s & 1); s>>=1; // заполнение 00000000 или 11111111 3его байта 255*<<16;
             // заполнение 00000000 или 11111111 4ого байта с управляемой инверсией.       255*<<24
             bytes_data_32[d + x]=(0xFF000000*(s & 1) | s0) ^ invert;   s>>=1;
             // запись в массив bytes_data одним 32р словом 4х байт одновременно
             */
            // обратный порядок пикселей // bytes_data[d + 32-x-1]=255*(s & 1);  s>>=1;
            s0=255*(s & 1)<<24;   s>>=1; // заполнение 00000000 или 11111111 4ого байта
            s0|=255*(s & 1)<<16;  s>>=1; // заполнение 00000000 или 11111111 3его байта
            s0|=255*(s & 1)<<8;   s>>=1; // заполнение 00000000 или 11111111 2ого байта
            // заполнение 00000000 или 11111111 1ого байта с управляемой инверсией.
            bytes_data_32[d + 8-x-1]=(255*(s & 1) | s0) ^ invert;   s>>=1;
            // запись в массив bytes_data одним 32р словом 4х байт одновременно
            
        } // x     // уровни в битмапе  0-чёрный и 255-белый
    } // y      // s>>=1; деление на 2 с присваиванием
    //*/
    free(bites_data);  // освобождение маленького массива
    pack_flag=0;
    
    //		DM(END<<" data_size "<<data_size<<" ncolumns*nrows "<<ncolumns*nrows<<END);
    
}//_____________________________________________________________________________


/** заполняет выбранную область в GBitmap32 нулями или единицами */
void GBitmap::fill32V(bool color, OCRBox *s){
    int shiftX0=(s->x0)/32;
    int shiftX1=(s->x1)/32;
    int dY=s->y0;
    int h=s->y1-s->y0;
    uint *p1;
    uint *bites_dataInt0=(uint*)bites_data;
    
    for(int index=shiftX0;index<=shiftX1;index++){//цикл перебора колонок
        p1=bites_dataInt0+nrows*index+dY;
        memset(p1,color,h*4);
    }
}

/** заполняет в GBitmap32 выбранные колонки +1 с каждой стороны  нулями или единицами */
void GBitmap::fillColumns32V(bool color, OCRBox *s){
    int shiftX0=(s->x0-32)/32-1; if(shiftX0<0)shiftX0=0;
    int shiftX1=(s->x1+32)/32+1; if(shiftX1>ncolumns/32)shiftX1=shiftX1>ncolumns/32;
    uint *p1;
    uint *bites_dataInt0=(uint*)bites_data;
    
    for(int index=shiftX0;index<=shiftX1;index++){//цикл перебора колонок
        p1=bites_dataInt0+nrows*index;
        memset(p1,color,nrows*4);
    }

}

// Отображение маски ON (в одном int 32 пикселя) по заданным координатам (сдвиг по x,y) в объект GBitmap
// в виде упакованного и транспонированного массива bites_data  (в одном int 32 пикселя)

void GBitmap::drawMask32V(void *mask32,int x0, int y0,OCRBox *s, int mode){
    GBitMask32 *mask=(GBitMask32*)mask32;
    int print=0;
    //if(inputData.start==111){
    //   print=1;
       //mask->printMask();
    //   print++;
    //}
    //  x0 величина полного сдвига по x
    // вычисление целочисленного смещения (по 32) по колонкам

    int shift32=x0/32;
    // вычисление пиксельного (битового) смещения по колонкам
    int dl=x0%32;               // величина регистрового сдвига (<32) по x
    int mH=mask->mH;            // актуальная высота маски
    
    DR("***DrawMask x0="<<x0<<" s.x0="<<s->x0<<" s.x1="<<s->x1<<" y0="<<y0<<" s.y0="<<s->y0<<" s.y1="<<s->y1<<endl)
    
    //проверяем выходит ли маска за габариты буквы. При этом маска устанавливается на изображении с некоторым
    //смещением относительно эталонной буквы. Это смещение соответствует лимиту поиска и его величина не более 4 пикселов.
    
    int dX1=s->x0-x0; if(dX1<0)dX1=0; DR(" dX1="<<dX1<<endl)
    if(dX1>32||x0>s->x1)return; //маска не в габарите буквы по X
    int dY1=s->y0-y0; if(dY1<0)dY1=0;  DR(" dY1="<<dY1<<" mH="<<mH<<endl)
    if(dY1>s->y1||mH-dY1<=0)return; //маска не в габарите буквы по Y
    
    if(y0+mH>s->y1)mH=s->y1-y0;  //проверяем высоту маски на соответствие габариту
    
    int dX2=0;
    if(x0+mask->imgW>s->x1)dX2=32-(s->x1-x0);
    if(dX2<0)dX2=0;

    DR(" dX1="<<dX1<<" dX2="<<dX2<<" mH1="<<mH<<endl);
    
    unsigned long long s0,s1;   // переменные 64 разряда
    memset(mask256,0,1024);     // обнуление массива
    uint *img32;
    uint maskSlice;
    if(mode==XOR_MODE){
        for ( int y=dY1;  y < mH;  y++ ) {
            maskSlice=mask->On[y];
            maskSlice=maskSlice>>dX2;  //формируем нужный участок изображения слайса маски
            maskSlice=maskSlice<<(dX1+dX2);
            maskSlice=maskSlice>>dX1;
            
            s0=maskSlice;
            s0=s0<<32; // сдвиг первого int во второй int (иначе пропадут данные при выполнении s1=s0>>dl;) // binaryPrint64(s0,32); cout<<"   -> source "<<endl;
            s1=s0>>dl; // сдвиг обоих int как одного 64р регистра на величину dl  // binaryPrint64(s1, 32); cout<<" s1"<<endl;
            // запись значений в первую и вторую колонку транспонированного массива mask256
            //mask256[x+128]=(uint)s1;
            img32=(uint*)(bites_data+(y+nrows*shift32+y0)*4); // указатель на актуальные данные в GBitmap
            uint a=*img32;
            *img32=(uint)(s1>>32)|(*img32); //побитовое сложение масок по "или"
            img32=(uint*)(bites_data+(y+nrows*shift32+nrows+y0)*4);
            *img32=(uint)s1|(*img32);
            
            
            //binaryPrint(mask256[x],32); cout<<" + "; binaryPrint(mask256[x+128],32); cout<<endl; //exit(0);
        } // x
    }
    if(mode==ADD_MODE){
        for ( int y=dY1;  y < mH;  y++ ) {
            maskSlice=mask->On[y];
            maskSlice=maskSlice>>dX2;  //формируем нужный участок изображения слайса маски
            maskSlice=maskSlice<<(dX1+dX2);
            maskSlice=maskSlice>>dX1;
            
            s0=maskSlice;
            s0=s0<<32; // сдвиг первого int во второй int (иначе пропадут данные при выполнении s1=s0>>dl;) // binaryPrint64(s0,32); cout<<"   -> source "<<endl;
            s1=s0>>dl; // сдвиг обоих int как одного 64р регистра на величину dl  // binaryPrint64(s1, 32); cout<<" s1"<<endl;
            // запись значений в первую и вторую колонку транспонированного массива mask256
            //mask256[x+128]=(uint)s1;
            img32=(uint*)(bites_data+(y+nrows*shift32+y0)*4); // указатель на актуальные данные в GBitmap
            *img32=(uint)(s1>>32)&(*img32); // сложение масок по "или"
            img32=(uint*)(bites_data+(y+nrows*shift32+nrows+y0)*4);
            *img32=(uint)s1&(*img32);
            //binaryPrint(mask256[x],32); cout<<" + "; binaryPrint(mask256[x+128],32); cout<<endl; //exit(0);
        } // x
    }
    y0=0;
    //memcpy(bites_data+4*y0, mask256, mH*4);
    //memcpy(bites_data+nrows*4, mask256+128, mask->mH*4);
    
    DR("@@@/*****Draw Mask32*****/  x0="<<x0<<" y0="<<y0<<" mH="<<mask->mH<<" shift32="<<shift32<<" dl="<<dl<<endl)
    //mask->printMask();
    //for(int i=0;i<mask->mH;i++){
    //    memcpy(bites_data+y0, mask->On, mask->mH*4);
    //}
    
}//_____________________________________________________________________________

/**\brief функция подсчета количества белых пикселов в заданной области в формате GBitmap32 */
int GBitmap::pixelCount(OCRBox *s){
    int print=0;
    //if(inputData.start==111)print=1;
    uint SumBit=0;
    uint mask=0xffffffff,maskP;
    uint pData;
    // вычисление целочисленного смещения (по 32) по колонкам
    int shiftX0=(s->x0)/32;
    int shiftX1=(s->x1)/32;

    ulong size=nrows*ncolumns/32;

    // вычисление пиксельного (битового) смещения по колонкам
    int dX1=(s->x0)%32;               // величина первого регистрового сдвига вправо (<32) по x
    
    int dX2=32-(s->x1)%32;               // величина второго регистрового сдвига влево (<32) по x
    
    int dY=s->y0;
    int h=s->y1-s->y0;
    uint *p1;
    uint *bites_dataInt0=(uint*)bites_data;
    
    DR("//**** pixelCount shiftX0="<<shiftX0<<" shiftX1="<<shiftX1
       <<" s->x0="<<s->x0<<" s->x1="<<s->x1<<endl)
    p1=bites_dataInt0+nrows*shiftX0+dY;
    
    if(shiftX0==shiftX1){  //***** область пересечения расположена в одной колонке (32 пиксела) изображения первой буквы
        
        // создание 32 разрядной маски обкусанной с двух сторон на величины dX1 и dX2
        mask=mask>>dX2;
        mask=mask<<(dX1+dX2);
        mask=mask>>dX1;
        
        p1=bites_dataInt0+nrows*shiftX0+dY;

        for(int i=0;i<h;i++){
            
            pData=mask&(*p1);  // пересечение маски с изображением первой буквы
            maskP=pData&mask; // пересечение изображения и маски с записью результата в пустую маску
            
            // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
            SumBit+=_mm_popcnt_u32(maskP); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
            SumBit+=SumBit32(maskP);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
            //int SumBit32(int Sb);
            //binaryPrint(*p1, 32); cout<<endl;
            p1++;
        }
    }else{   //***** область пересечения расположена в двух или более колонках (32 пиксела) изображения первой буквы
        
        for(int index=shiftX0;index<=shiftX1;index++){//цикл перебора колонок
            mask=0xFFFFFFFF;
            if(index==shiftX0){
                mask=mask<<dX1;
                mask=mask>>dX1;
            }
            if(index==shiftX1){
                mask=mask>>dX2;
                mask=mask<<dX2;
            }
            
            p1=bites_dataInt0+nrows*index+dY;
            
            for(int i=0;i<h;i++){
                pData=mask&(*p1);  // пересечение маски с изображением первой буквы
                maskP=pData&mask; // пересечение изображения и маски с записью результата в пустую маску
                // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
                SumBit+=_mm_popcnt_u32(maskP); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
                SumBit+=SumBit32(maskP);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
                //int SumBit32(int Sb);
                //binaryPrint(*p1, 32); cout<<endl;
                p1++;
            }
        }
    }

    DR("SumBit pixelCount="<<SumBit<<endl)
    return SumBit;
}

/**\brief функция подсчета количества совпадений пикселов двух изображений в заданной области в формате GBitmap32 */
uint GBitmap::img32Union(GBitmap *bImg,OCRBox *s){
    
    int print=0;
    uint mask=0xffffffff;
    uint SumBit=0;
    // вычисление целочисленного смещения (по 32) по колонкам
    int shiftX0=(s->x0)/32;
    int shiftX1=(s->x1)/32;
    // вычисление пиксельного (битового) смещения по колонкам
    int dX1=(s->x0)%32;               // величина первого регистрового сдвига вправо (<32) по x
    
    int dX2=32-(s->x1)%32;               // величина второго регистрового сдвига влево (<32) по x
    
    int dY=s->y0;
    int h=s->y1-s->y0;
    uint *p1,*p2,pData;
    uint *bites_dataInt0=(uint*)bites_data;
    uint *bites_dataInt1=(uint*)bImg->bites_data;
    
    DR("//**** img32Union dX1="<<dX1<<" dX2="<<dX2<<" shiftX0="<<shiftX0<<" shiftX1="<<shiftX1
       <<" s->x0="<<s->x0<<" s->x1="<<s->x1<<endl)
        
    if(shiftX0==shiftX1){  //***** область пересечения расположена в одной колонке (32 пиксела) изображения первой буквы
        
        // создание 32 разрядной маски обкусанной с двух сторон на величины dX1 и dX2
        mask=mask>>dX2;
        mask=mask<<(dX1+dX2);
        mask=mask>>dX1;
        
        p1=bites_dataInt0+nrows*shiftX0+dY;
        p2=bites_dataInt1+nrows*shiftX0+dY;
        
        for(int i=0;i<h;i++){
            
            pData=mask&(*p1);  // пересечение маски с изображением первой буквы
            *p2=mask&(*p2);  // пересечение маски с изображением второй буквы
            *p2=(pData)&(*p2); // пересечение изображений двух букв с записью в изображение второй буквы
            
            // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
            SumBit+=_mm_popcnt_u32(*p2); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
            SumBit+=SumBit32(*p2);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
            //int SumBit32(int Sb);
            //binaryPrint(*p1, 32); cout<<endl;
            p1++;
            p2++;
        }
    }else{   //***** область пересечения расположена в двух или более колонках (32 пиксела) изображения первой буквы
        
        for(int index=shiftX0;index<=shiftX1;index++){//цикл перебора колонок
            mask=0xFFFFFFFF;
            if(index==shiftX0){
                mask=mask<<dX1;
                mask=mask>>dX1;
            }
            if(index==shiftX1){
                mask=mask>>dX2;
                mask=mask<<dX2;
            }
            
            p1=bites_dataInt0+nrows*index+dY;
            p2=bites_dataInt1+nrows*index+dY;
            
            for(int i=0;i<h;i++){
                pData=mask&(*p1);  // пересечение маски с изображением первой буквы
                *p2=mask&(*p2);  // пересечение маски с изображением второй буквы
                *p2=(pData)&(*p2); // пересечение изображений двух букв с записью в изображение первой буквы
                // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
                SumBit+=_mm_popcnt_u32(*p2); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
                SumBit+=SumBit32(*p2);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
                //int SumBit32(int Sb);
                //binaryPrint(*p1, 32); cout<<endl;
                p1++;  p2++;
            }
        }
    }
    
    
    DR("SumBit="<<SumBit<<endl)
    return SumBit;
}//_____________________________________________________________________________

/**\brief функция подсчета количества совпадений пикселов изображения буквы и изображения фокальных линий строки букв в заданной области в формате GBitmap32. результат пересечения заносится в bImg*/
uint GBitmap::img32UnionLine(GBitmap *bImg,OCRBox *s){
    
    int x0_=s->x0;
    int x1_=s->x1;
    int y0_=s->y0;
    int y1_=s->y1;
    
    int print=0;
    uint mask=0xffffffff;
    uint SumBit=0;
    // вычисление целочисленного смещения (по 32) по колонкам
    int shiftX0=(x0_)/32;
    int shiftX1=(x1_)/32;
    // вычисление пиксельного (битового) смещения по колонкам
    int dX1=(x0_)%32;               // величина первого регистрового сдвига вправо (<32) по x
    
    int dX2=32-(x1_)%32;               // величина второго регистрового сдвига влево (<32) по x
    
    int dY=y0_;
    int h=y1_-y0_;
    uint *p1,*p2,pData;
    uint *bites_dataInt0=(uint*)bites_data;
    uint *bites_dataInt1=(uint*)bImg->bites_data;
    
    DR("//**** img32UnionLine dX1="<<dX1<<" dX2="<<dX2<<" shiftX0="<<shiftX0<<" shiftX1="<<shiftX1
       <<" s->x0="<<x0_<<" s->x1="<<x1_<<" dY="<<dY<<endl)
    
    
    if(shiftX0==shiftX1){  //***** область пересечения расположена в одной колонке (32 пиксела) изображения первой буквы
        
        // создание 32 разрядной маски заполненной нулями с двух сторон на величины dX1 и dX2
        mask=mask>>dX2;
        mask=mask<<(dX1+dX2);
        mask=mask>>dX1;
        
        p1=bites_dataInt0+nrows*shiftX0+dY;
        p2=bites_dataInt1+nrows*shiftX0+dY;
        
        for(int i=0;i<h;i++){
            
            pData=mask&(*p1);  // пересечение маски с изображением первой буквы
            *p2=mask&(*p2);  // пересечение маски с изображением второй буквы
            *p2=(pData)&(*p2); // пересечение изображений двух букв с записью в изображение первой буквы
            
            // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
            SumBit+=_mm_popcnt_u32(*p2); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
            SumBit+=SumBit32(*p2);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
            //int SumBit32(int Sb);
            //binaryPrint(*p1, 32); cout<<endl;
            p1++;
            p2++;
            
        }
        //заполняем нулями оставшеюся часть колонки
        p2=bites_dataInt1+nrows*shiftX0;
        memset(p2,0,dY*4);
        p2=bites_dataInt1+nrows*shiftX0+dY+h+1;
        memset(p2,0,(bImg->rows()-dY-h)*4);
    }else{   //***** область пересечения расположена в двух или более колонках (32 пиксела) изображения первой буквы
        
        for(int index=shiftX0;index<=shiftX1;index++){//цикл перебора колонок
            mask=0xFFFFFFFF;
            if(index==shiftX0){
                mask=mask<<dX1;
                mask=mask>>dX1;
            }
            if(index==shiftX1){
                mask=mask>>dX2;
                mask=mask<<dX2;
            }
            
            p1=bites_dataInt0+nrows*index+dY;
            p2=bites_dataInt1+nrows*index+dY;
            
            for(int i=0;i<h;i++){
                pData=mask&(*p1);  // пересечение маски с изображением первой буквы
                *p2=mask&(*p2);  // пересечение маски с изображением второй буквы
                *p2=(pData)&(*p2); // пересечение изображений двух букв с записью в изображение первой буквы
                // подсчет суммы пикселей пересечения изображений двух букв
#ifdef ASM_OS64_SSE42_POPCNT_ATT
                SumBit+=_mm_popcnt_u32(*p2); // подсчет единиц в 32 разрядном слове с помощью быстрой но новой _mm_popcnt_u32
#else
                SumBit+=SumBit32(*p2);       // подсчет единиц в 32 разрядном слове с помощью функции SumBit32 регистровыми сдвигами
#endif
                //int SumBit32(int Sb);
                //binaryPrint(*p1, 32); cout<<endl;
                p1++;  p2++;
            }
            //заполняем нулями оставшеюся часть колонки
            p2=bites_dataInt1+nrows*index;
            memset(p2,0,dY*4);
            p2=bites_dataInt1+nrows*index+dY+h;
            memset(p2,0,(bImg->rows()-dY-h)*4);
            
        }
    }

    
    DR("SumBit="<<SumBit<<endl)
    return SumBit;
}//_____________________________________________________________________________


void GBitmap::imgIntegralSum(GBitmap *bImg,vector<uint>&sum,OCRBox *s){
    uchar *p=bytes_data;
    uchar *p1=bImg->bytes_data;
    uint allSum=0;
    sum.resize(0);
    if(s->x0>ncolumns||s->x0<0){
        cout<<"match dimention error"<<endl;
        sum.push_back(0);
        return;
    }
    
    for(uint x=s->x0;x<s->x1;x++){
        uint lineSum=0;
        for(uint y=0;y<nrows;y++){
            if(*(p+y*ncolumns+x)&&*(p1+y*ncolumns+x))lineSum++;
        }
        allSum+=lineSum;
        sum.push_back(allSum);
    }
}

