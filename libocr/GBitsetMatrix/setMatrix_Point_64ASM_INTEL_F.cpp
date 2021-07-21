//
//  setMatrix_Point_32_64_SSE.c
//  Image2OCR
//
#include "GBitsetMatrix.h"

// Сама функция взята из setMatrix_Point_32_64_SSE.cpp и сделана setMatrix_Point_64ASM_INTEL_F.cpp
// Переназначены регистры popcnt и add. 12 апреля 2019.
// По утверждения авторов реальное быстродействие должно увеличится в 2 раза:
// https://stackoverflow.com/questions/25078285/replacing-a-32-bit-loop-counter-with-64-bit-introduces-crazy-performance-deviati
// На статичесих входных данных нет никакой разници.


// @@@ самая быстрая версия на 19 апреля 2011 (нотбуки и сервера USA)

//  Assembler only 64 only popcnt // в 2.76 раз быстрее

// Вычисление коэффициента корреляции в точке xP, yP.
// ™Assembler™ версия, работает только в 64 разрядном режиме процессора, используя команду popcnt подсчита сумм единиц в 64х разрядных словах,
// I1PD958471300.tif 55сек, быстрее базовой универсальной 32/64р ™С™ версии в 2,76 раза  (1.76 раза быстрее CPU "C" only 64)
// (по полному времени обсчета страници)
//
// -----------------------------------------------------------------------------
#ifdef ASM_OS64_SSE42_POPCNT_F
int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
                                   int xP,
                                   int yP,
                                   int sx0,
                                   int sy0,
                                   int w,
                                   int *ch,
                                   int print
                                   ){
    
    int correlation=0;
    unsigned long long p;
    long long deltaSum;    ///  int deltaSum;  long long SumBitOn,SumBitOff;
    int nr=nrows;  // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
    unsigned long long mH=(unsigned long long)mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    unsigned long long *sIntA=(unsigned long long *)sInt,   *maskOnA=(unsigned long long *)mask32[0].On,   *maskOffA=(unsigned long long *)mask32[0].Off;
    
    
    
    //////////// КЕШ  ///////////////
    int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
    //DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");
    
    ////	 	if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
    if ( correlation != 255) { *ch=1; return correlation; }
    
    
    ///TIME_START
    ////for (int count = 0; count < 50000; count++) {  //проверка быстродействия
    
    // Вычисление коэффициента корреляции в точке xP, yP.
    p=(unsigned long long) ((yP+xP*nr)*4);  // *4 (<<2) считаем в байтах  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;
    
    
    // переход в режим Assembler 64р
    __asm {
        
        // Установка накопительных 64p регистров  r8 и r9 и переменной цикла rdx в ноль или mov eax, 0
        xor             rdx, rdx;       // установка ноля  0x0000000000000000
        xor             r8,  r8;        // установка ноля  0x0000000000000000
        xor             r9,  r9;        // установка ноля  0x0000000000000000
        
        ////     mov             r11,  0xFFFFFFFFFFFFFFFF;         // установка ноля
        ////     mov             r12,  0xFFFFFFFFFFFFFFFF;         // установка ноля
        
        
        ////  начало цикла по переменной в регистре  rdx, шаг цикла 8, количество циклов mH/2
        //  8 число байт в 64 разрядном регистре, считаем 2 слайса за один цикл, количество циклов: mH*8/2 = mH*4 = mH<<2
        mov       rcx, mH;               //  загрузка количества циклов mH
        shl       rcx, 2;                //  умножение  rcx на *4 или lea  rcx,[rcx*4]; //  умножение  rcx на *4 + смещение на один (8) цикл
    loop_start:
        
        
        
        // вычисление адреса транспонированного скана sIntA
        mov       rax, sIntA;            //  вычисление адреса слайса скана
        add       rax, p;                //  добавление к адресу слайса смещения на переменную p
        lea       rax, [rax+rdx];        //  добавление к адресу маски переменной цикла   //add       rax, rdx;
        mov       r10, [rax];            //  загрузка слайса скана в регистр r10
        //                 mov             r10,  0xFFFFFFFFFFFFFFFF;
        
        // переход, если два подряд идущих слайса (64р) равны нолю (маски и единици не считаем)
        cmp       r10, 0;
      je          if_zero;               // je переход если  флаг ноля FZ=0     jz
        
        // вычисление адреса, умножение маски On на транспонированный скан sIntA
        mov       rax, maskOnA;          //  вычисление адреса маски maskOnA
        lea       rax, [rax+rdx];        //  добавление к адресу маски переменной цикла   //add       rax, rdx;
        mov       r11, [rax];            //  загрузка маски On в регистр r11
        //                 mov             r11,  0xFFFFFFFFFFFFFFFF;
        and       r11, r10;              //  применение & маски On к слайсу скана
        
        // вычисление адреса, умножение маски Off на транспонированный скан SInt
        mov       rax, maskOffA          //  вычисление адреса маски maskOffA
        lea       rax, [rax+rdx];        //  добавление к адресу маски переменной цикла   //add       rax, rdx;
        mov       r12, [rax];            //  загрузка маски maskOffA в регистр r12
        //                 mov             r12,  0xFFFFFFFFFFFFFFFF;
        and       r12, r10;              //  применение & маски maskOffA к слайсу скана
        
        
        
        // Регистровые суммы маски On (подсчет единиц) по  X и Y, инструкция popcnt была добавлена в "Nehalem" (SSE4.2).
        // Регистр rax свободен.
        popcnt    r11, r11;              // подсчет количества единиц скана попавших в маску On по горизонтали.
        add       r8,  r11;              // накопление единиц попавших в маску On в регистре r8
                                         // (подсчет маскированной площади всего признака).
        
        // Регистровые суммы маски Off (подсчет единиц) по X и Y
        popcnt    r12, r12;              // подсчет количества единиц скана попавших в маску Off по горизонтали.
        add       r9,  r12;              // накопление единиц попавших в маску Off в регистре r9
                                         // (подсчет маскированной площади всей защитной области).
        
    if_zero:
        
        
        ////  окончание цикла, шаг цикла 8       // add  +8   // sub -8
        add             rdx, 8;          // в rdx находится переменная цикла
        cmp             rdx, rcx;        // в rcx находится количество циклов mH*4
    jl                  loop_start;      // jl перейти, jle перейти, если меньше или равно
        
        
        // Регистровая разность площадей скана попавших маску Off и маску On
        sub       r8,  r9;               // вычитание количества единиц попавших в маску Off регистр r9,
                                         // из единиц попавших в маску On регистр r8.
        
        // заносим разности площадей On и Off в накопительную переменную  deltaSum
        mov       deltaSum, r8;          // сохранение разности площадей On и Off в переменной deltaSum
        
        // возврат из режима Assembler
    } // __asm
    
    
    
    ////} //проверка быстродействия
    
    // разность сумм по маскам
    ///    deltaSum=SumBitOn-SumBitOff;  // asm
    ///    if(deltaSum<0){deltaSum=0;}
    ////if(deltaSum<1) {cacheCor[q]=0; return 0;}    // return 0; return correlation;
    
    if(deltaSum<1) {return 0;}    // return 0; return correlation;
    
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    // проверка mask32[0].NMask<1 выполняется выше в функции setMatrix_Lion
    //if(deltaSum<1||mask32[0].NMask<1)return 0;
    
    // коэффициент корреляции  // КЭШ  //
    //correlation=deltaSum*100/mask32[0].NMask;
    correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
    
#ifdef DEBUGLVL_DRAW
    if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
        drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
#endif
    
    
    ///TIME_PRINT
    //SH(time);
    
    return correlation;
    
    // if(mask32[0].NMask<1)return 0; // вынести иэ зтого цикла
    // вместо if(deltaSum<0){deltaSum=0;} сделать if(deltaSum<1){deltaSum=0;} ???
    
}//_____________________________________________________________________________
#endif
