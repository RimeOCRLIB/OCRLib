//
//  setMatrix_Point_32_64_SSE.c
//  Image2OCR
//
#include "GBitsetMatrix.h"

/*
 #ifdef ASM_OS32_SSE2
 
 //  ASM  Assembler SSE2 128 32 bit  //
 
 // классическая 32 bit версия (чуть медленнее чем MAC с popcnt)
 
 
 // Вычисление коэффициента корреляции по маске единичных битов  IEST
 // -----------------------------------------------------------------------------
 
 
 int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
 int xP,
 int yP,
 int sx0,
 int sy0,
 int w,
 int *ch,
 int print){
 //int A=sInt;
 ///  static int pR=0; pR++;
 ///  cout<<"_"<<pR<<"/ ";
 ///  if(pR==85){pR++; //cout<<sIntA[0];}
 
 
 
 
 unsigned int p;
 //int y,x,n,m,p;
 int correlation=0;
 int SumBitOn,SumBitOff,deltaSum;
 int nr=nrows;  // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
 int mH=mask32[0].mH; //mW=32; // mH(8-96) высота и mW (32) ширина (габарит) битовой
 // маски перефирийной области  "Off"
 unsigned int *sIntA=sInt,   *maskOnA=mask32[0].On,   *maskOffA=mask32[0].Off;
 
 /// static int count=0;
 /// if(print){cout<<count<<" "; count++;}
 
 //////////// КЕШ  ///////////////
 int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
 //DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");
 
 //if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
 if ( correlation != 255) { *ch=1; return correlation; }
 
 
 //TIME_START
 ////for (int count = 0; count < 50000; count++) {  //проверка быстродействия
 
 //// Вычисление коэффициента корреляции в точке xP, yP. Assembler ////
 
 p=(yP+xP*nr)<<2;  // *4 считаем в байтах  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;
 ///for ( m=0; m < mH; m+=2 ) {
 #ifdef REPORT
 if(print){
 
 //   char str[128]; // char *string;   строка результата
 //   unsigned int test=sIntA[p];
 //   itoa(test,str,2); // двоичное представление числа   StrM[x]
 //   cout<<str<<endl;
 
 }
 #endif
 //------------------------------------------------------------------------------
 
 // переход в режим Assembler  // SSE2 128
 __asm {
 
 //// Установка накопительных регистров SSE2  mm3 и mm4  в ноль
 // mov eax, 0	 // xor eax, eax  	// pxor mm3, mm3;    // ecx, 0; ebx, 0;
 xor           edx,  edx;         // установка ноля   0x00000000
 pxor          xmm3, xmm3;        // установка ноля   0x00000000000000000000000000000000
 pxor          xmm4, xmm4;        // установка ноля   0x00000000000000000000000000000000
 //pcmpeqd       xmm2, xmm2         // установка единиц 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
 
 
 ////  начало цикла по переменной в регистре  edx=0, шаг цикла 16, количество циклов mH*4
 //  16 число байт в SSE128, 4 число байт в 32 разрядном слове, считаем 4 слайса за один цикл
 //  количество циклов:  mH*16/4 = mH*4 = mH<<2       // shl     ecx, 2;
 
 mov       ecx, mH;               //  загрузка количества циклов mH
 shl       ecx, 2;                //  умножение  ecx на *4    add eax,eax =*2
 //	lea       ecx,[ecx*4];           //  *4 + смещение на один (16) цикл
 loop_start:                               //  lea       ecx,[ecx*4+16];
 
 
 // По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU.
 //  Например, инструкция paddq выполняется за шесть тактов на регистрах XMM, но только за два такта - на MMX.
 // pshufw          mm1, mm0, 0xE4; // (два такта) быстрое копирование одного регистра в другой.
 // movq (шесть тактов)  // pshufw mm0, mm0, 0xE4 // pshufd xmm0, xmm0, 0xE4
 
 /// +++++ movdqu — перемещение невыровненных 128 бит из источника в приемник.( MOVDQA должны быть выровнены )
 // PADDQ        xmm3, xmm0;   // ++ packsswb  +xPADDQ      movlhps  MOVHPS  MOVHLPS   MOVLPS
 
 
 
 
 //------------------------------------------------------------------------------
 //  Регистры EAX,EBX,EDX,ECX называют рабочими регистрами               // neg
 //  кроме того в процессе выполнения этой программы служебно не модифицируются
 //  регистры  ebx  ecx  esp  esi  edi    кроме ebp
 
 // "Выражения asm должны сохранять регистры EDI, ESI, ESP, EBP и EBX, но могут
 // свободно изменять регистры EAX, ECX и EDX."
 
 
 // вычисление адреса транспонированного скана sIntA
 mov       eax, sIntA;              //  вычисление адреса слайса скана
 add       eax, p;                  //  добавление к адресу слайса смещения на переменную p
 lea       eax, [eax+edx];          //  добавление к адресу транспонированного скана sIntA переменной цикла
 //  добавление к адресу транспонированного скана sIntA переменной цикла
 LOAD_PTR    xmm1, [eax];  // movups ebx загрузка слайса скана в регистр xmm1
 
 
 // переход (единици не считаем), если четыре (128) подряд идущих слайса равны нолю
 pxor           xmm0, xmm0;         // загрузка ноля  0x0000000000000000 .....    2 1 MMX_ALU
 PCMPEQD        xmm0, xmm1;         // сравнение на равенство упакованных двойных слов. 2 1 MMX_ALU
 MOVMSKPS       eax,  xmm0;         // в младшую тетраду регистра eax заносится знаковая маска  6 2 FP_MISC
 cmp            eax,  0xF;          // je, сравнение eax и 00000000000000000000000000001111;
 //		  comiss        xmm0, xmm1;          //  ja, comiss - работает
 ///test            eax,  0xF;
 je        if_zero;                 // je (cmp) переход если  флаг ноля FZ=0     jz
 // jnz (test)	// JNE/JNZ – перейти, если меньше или равно.
 
 
 // вычисление адреса маски On добавление к адресу переменной цикла edx
 mov       eax, maskOnA;            // вычисление адреса маски On
 lea       eax, [eax+edx];
 LOAD_PTR    xmm0,  [eax];  //  esi загрузка маски maskOnA в регистр mxm0
 
 // вычисление адреса маски Off добавление к адресу переменной цикла edx
 mov       eax, maskOffA;           //  вычисление адреса маски maskOffA
 lea       eax, [eax+edx];
 LOAD_PTR    xmm2, [eax];  //  edi загрузка маски maskOffA в регистр xmm2
 
 //  умножение & маски On, Off на транспонированный скан sIntA (слайс скана)
 pand      xmm0, xmm1;              //  применение & маски к слайсу скана
 pand      xmm2, xmm1;              //  применение & маски к слайсу скана
 
 // forMAC  LDDQU . По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU. (LOAD_PTR)
 // forMAC  MOVDQA (MOVe Aligned Double Quadword) 6 1 FP_MOVE — перемещение выровненных 128 бит  ( MOVAPS )
 //------------------------------------------------------------------------------
 //MOVDQU
 // Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].On по  X
 MOV_PTR          xmm1, xmm0;         //  movups /// movss (без требования выравнивания)
 // pshufd          xmm1, xmm0, 0xE4;
 psrld           xmm0, 1;            // v >> 1          Логический сдвиг вправо, с заполнением старших бит нулями
 pand            xmm0, xmm5;         // (v >> 1) & 0x55555555
 psubd           xmm1, xmm0;         // w = v - ((v >> 1) & 0x55555555)  Вычитание упакованных байт (слов или двойных слов) без насыщения
 MOV_PTR          xmm0, xmm1;         // w               Пересылка данных (64 бит) из/в регистр ММХ
 psrld           xmm1, 2;            // w >> 2
 pand            xmm0, xmm6;         // w & 0x33333333  Побитовое логическое И над всеми битами источника и приемника. Результат помещается в  приемник.
 pand            xmm1, xmm6;         // (w >> 2)  & 0x33333333           Побитовое логическое И
 paddd           xmm0, xmm1;         // x = (w & 0x33333333) + ((w >> 2) & 0x33333333)  Сложение упакованных байт (слов или двойных слов) без насыщения
 MOV_PTR          xmm1, xmm0;         // x               Пересылка данных (64 бит) из/в регистр ММХ
 psrld           xmm0, 4;            // x >> 4          Логический сдвиг вправо
 paddd           xmm0, xmm1;         // x + (x >> 4)
 pand            xmm0, xmm7;         // y = (x + (x >> 4) & 0x0F0F0F0F)
 pxor            xmm1, xmm1;         // установка ноля  0x0000000000000000    Исключающее ИЛИ  ==1
 psadbw          xmm0, xmm1;         // сборка (разностями) и сложением вдоль регистра, старшие 3 слова из 4 слов обнуляем
 
 pshufd          xmm0, xmm0, 0xd8;   //11011000b=0xd8// исходное 11100100b = 0xE4 ,  инверсное 0x7F; unpckhps
 psadbw          xmm0, xmm1;         // сборка (разностями)
 paddq           xmm3, xmm0;         // — сложение учетверенных слов.   // 6 taktov
 
 
 
 // Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].Off по  X
 MOV_PTR          xmm0, xmm2;         //  копирование одного регистра в другой.
 MOV_PTR          xmm1, xmm2;         //  копирование одного регистра в другой.
 psrld           xmm0, 1;            //  PSLLDQ
 pand            xmm0, xmm5;
 psubd           xmm1, xmm0;
 MOV_PTR          xmm0, xmm1;
 psrld           xmm1, 2;
 pand            xmm0, xmm6;
 pand            xmm1, xmm6;
 paddd           xmm0, xmm1;
 MOV_PTR          xmm1, xmm0;
 psrld           xmm0, 4;
 paddd           xmm0, xmm1;
 pand            xmm0, xmm7;
 pxor            xmm1, xmm1;
 psadbw          xmm0, xmm1;         // раздельная (по старшей и младшей половинаи регистра xmm) сборка суммы единиц
 
 pshufd          xmm0, xmm0, 0xd8;    //11011000b;   // перемещение суммы единиц из старшей половины регистра в младшею
 psadbw          xmm0, xmm1;         // сборка (разностями) те сумма единиц старшей (перемещенной) и младшей половинам регистра
 paddq           xmm4, xmm0;         // суммирование единиц по всей маске  On
 
 
 
 if_zero:
 
 ////  окончание цикла, шаг цикла 16      // add  +16  // sub -16
 add             edx, 16;     // в edx находится переменная цикла
 cmp             edx, ecx;    // в ecx находится количество циклов mH*4
 jl             loop_start;   // jl перейти, jle перейти, если меньше или равно
 //------------------------------------------------------------------------------
 
 // заносим сумму единиц в накопительную переменную  SumBitOff , SumBitOn
 movd            SumBitOn,  xmm3;
 movd            SumBitOff, xmm4;
 
 // возврат из режима Assembler
 } // __asm
 
 //------------------------------------------------------------------------------
 
 
 // ниже подправить как в других функциях //
 
 // emms требуется только после работы с MMX, так как там регистры шарятся с FPU.
 // у SSE собственный пул регистров и emms уже не нужно.
 
 ////} //проверка быстродействия
 
 // разность сумм по маск
 deltaSum=SumBitOn-SumBitOff;
 if(deltaSum<0){deltaSum=0;}
 
 if(deltaSum<1||mask32[0].NMask<1)return 0;
 
 // коэффициент корреляции  // КЭШ  //
 ////correlation=deltaSum*100/mask32[0].NMask;
 correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
 //DP("//__correlation="<<correlation<<END);
 #ifdef DEBUGLVL_DRAW
 if(print){
 if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
 drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
 }
 #endif
 
 
 
 //TIME_PRINT
 //SH(time);
 
 return correlation;
 
 
 
 }//_____________________________________________________________________________
 
 #endif
 */





// @@  текущая версия на 10 апреля 2011 (самая быстрая для MAC PRO1 без popcnt)


//  ASM  Assembler SSE3 128 only 64 bit // в 2.45 раз быстрее

// Вычисление коэффициента корреляции по маске единичных битов  IEST
// ™Assembler™ версия, работает только в 64 разрядном режиме процессора,
// I1PD958471300.tif 62сек, быстрее базовой универсальной 32/64р ™С™ версии в 2,45 раза  ( 1,56 раза быстрее CPU "C" only 64)
// (по полному времени обсчета страници)

// при переходе с 32р в 64р режим компиляции  потребовалось изменить название регистров (напрямер edx в rdx) и переименовать переменные
// загружаемые в регистры из int в long long или (и) использовать приведение типов

// -----------------------------------------------------------------------------

#ifdef ASM_OS64_SSE3
int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
                                   int xP,
                                   int yP,
                                   int sx0,
                                   int sy0,
                                   int w,
                                   int *ch,
                                   int print){
    
    //int A=sInt;
    //  static int pR=0; pR++;
    //cout<<"_"<<pR<<"/ ";
    //if(pR==85){ pR++; //cout<<sIntA[0]; }
    
    
    unsigned long long p;  ////unsigned int p;      //int y,x,n,m,p;
    int correlation=255;  ////int correlation=0;
    int deltaSum=0;  //long long  deltaSum=0;  //long long SumBitOn,SumBitOff;
    int nr=nrows;    // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
    unsigned long long mH=(unsigned long long)mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    
    ////  unsigned int *sIntA=sInt,   *maskOnA=mask32[0].On,   *maskOffA=mask32[0].Off;           /////////////////////////////////
    unsigned long long *sIntA=(unsigned long long *)sInt,   *maskOnA=(unsigned long long *)mask32[0].On,   *maskOffA=(unsigned long long *)mask32[0].Off;
    
    
    
    //static int count=0;
    //if(print){ cout<<count<<" "; count++; }
    
    //////////// КЕШ  ///////////////
    int q=xP-sx0 + (yP-sy0)*w;
    correlation=cacheCor[q];
    *ch=0;
    //DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");
    
    //if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
    if ( correlation != 255) { *ch=1; return correlation; }
    
    
    //TIME_START
    ////for (int count = 0; count < 50000; count++) {  //проверка быстродействия
    
    //// Вычисление коэффициента корреляции в точке xP, yP. Assembler ////
    
    //// p=(yP+xP*nr)<<2;  // *4 считаем в байтах, 4 байта один int, прыгаем через int  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;
    p=(unsigned long long) ((yP+xP*nr)*4);
    
    
    ///for ( m=0; m < mH; m+=2 ) {
    
#ifdef REPORT
    if(print){
        //char str[128]; // char *string;   строка результата
        //unsigned int test=sIntA[p];
        //itoa(test,str,2); // двоичное представление числа   StrM[x]
        //cout<<str<<endl;
    }
#endif
    
    //------------------------------------------------------------------------------
    
    // переход в режим Assembler  // SSE2 128
    
    __asm {
        
        //// Установка накопительных регистров SSE2  mm3 и mm4  в ноль
        // mov eax, 0	 // xor eax, eax  	// pxor mm3, mm3;    // ecx, 0; ebx, 0;
        xor           rdx,  rdx;         // установка ноля   0x0000000000000000   // mov       rdx, 0;
        pxor          xmm3, xmm3;        // установка ноля   0x00000000000000000000000000000000
        pxor          xmm4, xmm4;        // установка ноля   0x00000000000000000000000000000000
        //pcmpeqd       xmm2, xmm2       // установка единиц 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        
        
        ////  начало цикла по переменной в регистре  edx=0, шаг цикла 16, количество циклов mH*4
        //  16 число байт в SSE128, 4 число байт в 32 разрядном слове, считаем 4 слайса за один цикл
        //  количество циклов:  mH*16/4 = mH*4 = mH<<2       // shl     ecx, 2;
        
        mov       rcx, mH;               //  загрузка количества циклов mH
        shl       rcx, 2;            //  умножение  rcx на *4    add eax,eax =*2
        //	lea       ecx,[ecx*4];       //  *4 + смещение на один (16) цикл (не понадобилось)
    loop_start:                          //  lea       ecx,[ecx*4+16];
        
        
        // По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU.
        //  Например, инструкция paddq выполняется за шесть тактов на регистрах XMM, но только за два такта - на MMX.
        // pshufw          mm1, mm0, 0xE4; // (два такта) быстрое копирование одного регистра в другой.
        // movq (шесть тактов)  // pshufw mm0, mm0, 0xE4 // pshufd xmm0, xmm0, 0xE4
        
        /// +++++ movdqu — перемещение невыровненных 128 бит из источника в приемник.( MOVDQA должны быть выровнены )
        // PADDQ        xmm3, xmm0;   // ++ packsswb  +xPADDQ      movlhps  MOVHPS  MOVHLPS   MOVLPS
        
        
        
        
        //------------------------------------------------------------------------------
        //  Регистры EAX,EBX,EDX,ECX называют рабочими регистрами               // neg
        //  кроме того в процессе выполнения этой программы служебно не модифицируются
        //  регистры  ebx  ecx  esp  esi  edi    кроме ebp
        
        // "Выражения asm должны сохранять регистры EDI, ESI, ESP, EBP и EBX, но могут
        // свободно изменять регистры EAX, ECX и EDX."
        
        
        // вычисление адреса транспонированного скана sIntA, в rdx находится переменная цикла, шаг цикла 16
        mov       rax, sIntA;              //  вычисление адреса слайса скана
        add       rax, p;                  //  добавление к адресу слайса смещения на переменную p
        lea       rax, [rax+rdx];          //  добавление к адресу транспонированного скана sIntA переменной цикла, шаг цикла 16
        LOAD_PTR    xmm1, [rax];           //  movups rax загрузка слайса скана в регистр xmm1       ///////// LDDQU   MOVDQU        // LOAD_PTR = MOVDQU
        
        
        //		  // переход (единици не считаем), если четыре (128) подряд идущих слайса равны нолю
        pxor           xmm0, xmm0;         // загрузка ноля  0x0000000000000000 .....    2 1 MMX_ALU
        PCMPEQD        xmm0, xmm1;         // сравнение на равенство упакованных двойных слов. 2 1 MMX_ALU
        MOVMSKPS       rax,  xmm0;         // в младшую тетраду регистра eax заносится знаковая маска  6 2 FP_MISC
        cmp            rax,  0xF;          // je, сравнение rax и 00000000000000000000000000001111;
        //		  comiss         xmm0, xmm1;         // ja, comiss - работает
        
        je        if_zero;                 // je (cmp) переход если  флаг ноля FZ=0     jz
        // jnz (test)	// JNE/JNZ – перейти, если меньше или равно.
        
        
        // вычисление адреса маски On добавление к адресу переменной цикла edx
        mov       rax, maskOnA;            // вычисление адреса маски On
        lea       rax, [rax+rdx];          // добавление к адресу маски On переменной цикла, шаг цикла 16
        LOAD_PTR    xmm0,  [rax];          // загрузка маски maskOnA в регистр mxm0
        pand      xmm0, xmm1;              // применение & маски On к слайсу скана
        
        // вычисление адреса маски Off добавление к адресу переменной цикла edx
        mov       rax, maskOffA;           // вычисление адреса маски maskOffA
        lea       rax, [rax+rdx];          // добавление к адресу маски Off переменной цикла, шаг цикла 16
        LOAD_PTR    xmm2, [rax];           // загрузка маски maskOffA в регистр xmm2
        pand      xmm2, xmm1;              // применение & маски Off к слайсу скана
        
        
        ///MOVNTDQA xmm1, m128 — (Load Double Quadword Non-Temporal Aligned Hint) 2011 май
        ///Операция чтения, позволяющая ускорить (до 7.5 раз) работу с write-combining областями памяти.
        // forMAC  LDDQU . По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU. (LOAD_PTR)
        // forMAC  MOVDQA (MOVe Aligned Double Quadword) 6 1 FP_MOVE — перемещение выровненных 128 бит  ( MOVAPS ) // А как Писать лучше минуя кэш? MOVNTQ, MOVNTPS.
        
        //------------------------------------------------------------------------------
        
        // Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].On по  X
        MOV_PTR          xmm1, xmm0;         //  movups /// movss (без требования выравнивания)   MOV_PTR=MOVDQU //-LDDQU
        //pshufd          xmm1, xmm0, 0xE4;
        psrld           xmm0, 1;            // v >> 1          Логический сдвиг вправо, с заполнением старших бит нулями
        pand            xmm0, xmm5;         // (v >> 1) & 0x55555555
        psubd           xmm1, xmm0;         // w = v - ((v >> 1) & 0x55555555)  Вычитание упакованных байт (слов или двойных слов) без насыщения
        MOV_PTR          xmm0, xmm1;         // w               Пересылка данных (64 бит) из/в регистр ММХ
        //pshufd          xmm0, xmm1, 0xE4;
        psrld           xmm1, 2;            // w >> 2
        pand            xmm0, xmm6;         // w & 0x33333333  Побитовое логическое И над всеми битами источника и приемника. Результат помещается в  приемник.
        pand            xmm1, xmm6;         // (w >> 2)  & 0x33333333           Побитовое логическое И
        paddd           xmm0, xmm1;         // x = (w & 0x33333333) + ((w >> 2) & 0x33333333)  Сложение упакованных байт (слов или двойных слов) без насыщения
        MOV_PTR          xmm1, xmm0;         // x               Пересылка данных (64 бит) из/в регистр ММХ
        //pshufd          xmm1, xmm0, 0xE4;
        psrld           xmm0, 4;            // x >> 4          Логический сдвиг вправо
        paddd           xmm0, xmm1;         // x + (x >> 4)
        pand            xmm0, xmm7;         // y = (x + (x >> 4) & 0x0F0F0F0F)
        psadbw          xmm0, xmm4;         // сборка (разностями) и сложением вдоль регистра, старшие 3 слова из 4 слов обнуляем
        
        pshufd          xmm0, xmm0, 0xd8;   // управление 11011000b=0xd8 // исходное 11100100b = 0xE4 ,  инверсное 0x7F; unpckhps
        psadbw          xmm0, xmm4;         // сборка (разностями)
        paddq           xmm3, xmm0;         // суммирование единиц по всей маске  On // — сложение учетверенных слов. 6 taktov
        
        
        // Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].Off по  X
        MOV_PTR          xmm0, xmm2;         //  копирование одного регистра в другой.
        MOV_PTR          xmm1, xmm2;         //  копирование одного регистра в другой.
        //pshufd          xmm0, xmm2, 0xE4;
        //pshufd          xmm1, xmm2, 0xE4;
        psrld           xmm0, 1;             //  PSLLDQ
        pand            xmm0, xmm5;
        psubd           xmm1, xmm0;
        MOV_PTR          xmm0, xmm1;
        //pshufd          xmm0, xmm1, 0xE4;
        psrld           xmm1, 2;
        pand            xmm0, xmm6;
        pand            xmm1, xmm6;
        paddd           xmm0, xmm1;
        MOV_PTR          xmm1, xmm0;
        //pshufd          xmm1, xmm0, 0xE4;
        psrld           xmm0, 4;
        paddd           xmm0, xmm1;
        pand            xmm0, xmm7;
        psadbw          xmm0, xmm4;         // раздельная (по старшей и младшей половинаи регистра xmm) сборка суммы единиц
        
        pshufd          xmm0, xmm0, 0xd8;   //11011000b;   // перемещение суммы единиц из старшей половины регистра в младшею
        psadbw          xmm0, xmm4;         // сборка (разностями) т.е. сумма единиц старшей (перемещенной) и младшей половинам регистра
        psubq           xmm3, xmm0;         // вычитание всех единиц маски Off из единиц маски On
        
        
    if_zero:
        
        ////  окончание цикла, шаг цикла 16      // add  +16  // sub -16
        add             rdx, 16;     // в rdx находится переменная цикла 16 что соответствует размеру одного регистра xmm, т.е. прыгаем через размер xmm
        cmp             rdx, rcx;    // в rcx находится количество циклов mH*4
        jl              loop_start;  // jl перейти, jle перейти, если меньше или равно
        //------------------------------------------------------------------------------
        
        
        // заносим разности площадей On и Off в накопительную переменную  deltaSum
        movd            deltaSum, xmm3;  // сохранение разности площадей On и Off в переменной deltaSum
        
        
        // возврат из режима Assembler
    } // __asm
    
    //------------------------------------------------------------------------------
    
    
    
    // emms требуется только после работы с MMX, так как там регистры шарятся с FPU.
    // у SSE собственный пул регистров и emms уже не нужно.
    
    ////} //проверка быстродействия
    
    // разность сумм по маск
    ///    deltaSum=SumBitOn-SumBitOff;
    if(deltaSum<1){deltaSum=0;}
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    // проверка mask32[0].NMask<1 выполняется выше в функции setMatrix_Lion
    ///if(deltaSum<1||mask32[0].NMask<1)return 0;
    
    // коэффициент корреляции  // КЭШ  //
    ////correlation=deltaSum*100/mask32[0].NMask;
    correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
    //DP("//__correlation="<<correlation<<END);
#ifdef DEBUGLVL_DRAW
    if(print){
        if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
            drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
    }
#endif
    
    
    //TIME_PRINT
    //SH(time);
    
    return correlation;
    
    
}//_____________________________________________________________________________
#endif
/* */


// @@@ самая быстрая версия на 19 апреля 2011 (нотбуки и сервера USA)

//  Assembler only 64 only popcnt // в 2.76 раз быстрее

// Вычисление коэффициента корреляции в точке xP, yP.
// ™Assembler™ версия, работает только в 64 разрядном режиме процессора, используя команду popcnt подсчита сумм единиц в 64х разрядных словах,
// I1PD958471300.tif 55сек, быстрее базовой универсальной 32/64р ™С™ версии в 2,76 раза  (1.76 раза быстрее CPU "C" only 64)
// (по полному времени обсчета страници)
//
// -----------------------------------------------------------------------------
#ifdef ASM_OS64_SSE42_POPCNT
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
        
        // Установка накопительных 64p регистров   r8 и r9 и переменной цикла rdx в ноль или mov eax, 0
        xor             rdx, rdx;        // установка ноля  0x0000000000000000
        xor             r8,  r8;         // установка ноля  0x0000000000000000
        
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
        
        
        
        // Регистровые суммы маски On (подсчет единиц) по  X и Y
        popcnt    rax, r11;              // подсчет единиц маскированного слайса скана по X, инструкция была добавлена ​​в "Nehalem" (SSE4.2)
        add       r8,  rax;              // суммирование единиц попавших в маску On в регистре r8 (подсчет маскированной площади признака)
        
        // Регистровые суммы маски Off (подсчет единиц) по X и Y
        popcnt    rax, r12;              // подсчет количества единиц маскированного слайса скана по X (подсчет горизонтальных сумм)
        sub       r8,  rax;              // вычитание единиц попавших в маску Off из регистра r8
        
    if_zero:
        
        
        ////  окончание цикла, шаг цикла 8       // add  +8   // sub -8
        add             rdx, 8;          // в rdx находится переменная цикла
        cmp             rdx, rcx;        // в rcx находится количество циклов mH*4
        jl                  loop_start;      // jl перейти, jle перейти, если меньше или равно
        
        
        
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
