//
//  setMatrix_Point_64_popcnt_C.c
//  Image2OCR

#include "GBitsetMatrix.h"

#ifdef CPU_OS64_popcnt
#include <smmintrin.h>

//  CPU "C" only 64 на popcnt intrinsic // в 1,хх раз быстрее // в 1,7 раза медленнее чем ASM  Assembler SSE3 128 only 64 bit

// Вычисление коэффициента корреляции в точке xP, yP.
// "С" версия, работает только в 64 разрядном режиме процессора, подсчитывая суммы единиц в 64х разрядных словах с помощью
// intrinsic-функции (интринсики) на popcnt. Компиляторы GCC, Q/T.
// I1PD958471300.tif 0мин 00сек быстрее базовой универсальной 32/64р ™С™ версии в 1,хх раза (по полному времени обсчета страници)

// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
                                   int xP,
                                   int yP,
                                   int sx0,
                                   int sy0,
                                   int w,
                                   int *ch,
                                   int print
                                   ){
    
    unsigned long long m,p;
    unsigned long long Si;       //,Sb1,Sb2;
    unsigned long long *p0,*p1,*p2;
    long long SumBitOn,SumBitOff;
    int deltaSum;
    unsigned int nr=nrows;        // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
    unsigned int mH=mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    unsigned int mH_=mH/2;
    // m+=2 в главном цикле, поскольку в 64 разрядном режиме процессора с памятью можно общаться
    // только 64 разрядными словами.
    // После приведения типов int 32р к unsigned long long 64р за циклом, m++ эквивалентно m+=2 при mH=mH/2;.
    // Число подсчитанных слайсов в маске перефирийной области  "Off" по вертикали, в массиве mask32[]
    // округляется до меньшего четного числа ( mH=(mH/2)*2 и выполняется в другой функции )
    // т.е. высота маски (прямоугольника поиска) м.б. только четным числом.
    
    
    //////////// КЕШ  ///////////////
    int q=(xP-sx0 + (yP-sy0)*w); 
    int correlation=cacheCor[q];  *ch=0;    
    // если в текущих координатах [q] коэффициент корреляции cacheCor[q] уже вычисляли,
    // то его значение берем из КЭШа и выполняем return correlation;
    if ( correlation != 255) { *ch=1; return correlation; }
    

    
    // Вычисление коэффициента корреляции в точке xP, yP.
    p=(unsigned int)(yP+xP*nr);
    if(p>ncolumns*nrows-mH_)return 0;
    SumBitOn=SumBitOff=0;
    
    // приведение типов int 32р к unsigned long long 64р за циклом
    p0=(unsigned long long *)(sInt+p);           
    p1=(unsigned long long *)mask32[0].On;
    p2=(unsigned long long *)mask32[0].Off;
    
    // Суммы маски mask32 по  X и Y
    for ( m=0; m < mH_; m++ ) { // mH д.б. четным
        // получение слайса (64 бита картинки) из транспонированного скана SInt
        Si=*(p0+m);
        if(Si > 0) {   
            // Умножение (&) маски On на транспонированный скан SInt
            //Sb1=Si & *(p1+m);
            // Умножение (&) маски Off на транспонированный скан SInt
            //Sb2=Si & *(p2+m);
            // Регистровые суммы маски On  (подсчет единиц) по  X
            SumBitOn+=_mm_popcnt_u64(Si & *(p1+m));
            // Регистровые суммы маски Off (подсчет единиц) по  X
            SumBitOff+=_mm_popcnt_u64(Si & *(p2+m));
        } // if
    } // m

    
    // разность сумм по маскам
    deltaSum=(int)SumBitOn-(int)SumBitOff;
    if(deltaSum<0){deltaSum=0;}
    
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    // проверка mask32[0].NMask<1 выполняется выше в функции setMatrix_Lion
    //if(deltaSum<1||mask32[0].NMask<1)return 0;
    ////if(deltaSum<1) {cacheCor[q]=0; return 0;}  // проверить на быстродействие
    
    // вычисление коэффициента корреляции и загрузка его значения в КЭШ  
    //correlation=deltaSum*100/mask32[0].NMask;      
    correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;

// рисовалка коэффициента корреляции  blue 
#ifdef DEBUGLVL_DRAW
    correlation=(int)deltaSum*100/mask32[0].NMask;
    if(correlation<0)correlation=0;
    drawDataRGB->put(xP-32,yP,0xffff0000+correlation);  // blue
#endif
    
    
   
    return correlation;
    
    
    // #include <immintrin.h>  // заголовочный файл immintrin.h с описанием соответствующих intrinsic-функций. // smmintrin.h // xmmintrin.h
    // Intrinsics — набор функций и типов данных, поддерживаемых компилятором, для предоставления высокоуровневого доступа к SSE инструкциям
    
    
}//_____________________________________________________________________________
#endif

