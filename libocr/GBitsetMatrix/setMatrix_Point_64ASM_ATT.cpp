#include "GBitsetMatrix.h"


#ifdef ASM_OS64_SSE42_POPCNT_ATT
int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
                                   int xP,
                                   int yP,
                                   int sx0,
                                   int sy0,
                                   int w,
                                   int *ch,
                                   int print
                                   ){
    
    
    
    //////////// КЕШ  ///////////////
    int q=(xP-sx0 + (yP-sy0)*w);
    int correlation=cacheCor[q];  *ch=0;
    // если в текущих координатах [q] коэффициент корреляции cacheCor[q] уже вычисляли,
    // то его значение берем из КЭШа и выполняем return correlation;
    if ( correlation != 255) { *ch=1; return correlation; }

    
    
    unsigned long long p;
    int nr=nrows;  // nc=ncolumns;  // SH(ncolumns);  SH(nrows);    
    long long deltaSum;    /// значение может быть отрицательным.
    unsigned long long mH=(unsigned long long)mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    unsigned long long *maskOnA=(unsigned long long *)mask32[0].On;
    unsigned long long *maskOffA=(unsigned long long *)mask32[0].Off;
    
    // Вычисление коэффициента корреляции в точке xP, yP.
    p=(unsigned int) (yP+xP*nr);  // ((yP+xP*nr)*4) (<<2) считаем в байтах  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;

    
    //загрузка переменных в регистры
    //переменные заносятся непосредственно в функцию ассемблера.
    //приведение типа и вычисления производятся при входе в фунукцию;
    //значения sIntA_, maskOnA_, maskOffA_ загружаются в rex,rcx,rdx 
    //unsigned long long *sIntA_ =(unsigned long long *)(sInt+p);
    //unsigned long long *maskOnA_ =(unsigned long long *)maskOnA;
    //unsigned long long *maskOffA_=(unsigned long long *)maskOffA;

    asm (
         
         // сохранение переменных из именных регистров выделенных при загрузке функции ассемблера
         "mov        %%rax,  %%r13               \n" // сохраняем адрес слайса sIntA в регистре r13
         "mov        %%rbx,  %%r14               \n" // сохраняем адрес маски maskOnA_  в регистре r14
         "mov        %%rcx,  %%r15               \n" // сохраняем адрес маски maskOffA_ в регистре r15
         "mov        %%rdx,  %%r9                \n" // сохраняем адрес mH в регистре r9
         
         // Установка накопительных 64p регистров   %%r8 и r9 и переменной цикла %%rdx в ноль или mov eax, 0
         "xor        %%rdx,   %%rdx              \n" // установка ноля  0x0000000000000000
         "xor        %%r8,    %%r8               \n" // установка ноля  0x0000000000000000
         
         ////     mov             %%r11,  0xFFFFFFFFFFFFFFFF;         // установка ноля
         ////     mov             %%r12,  0xFFFFFFFFFFFFFFFF;         // установка ноля
         
         ////  начало цикла по переменной в регистре  %%rdx, шаг цикла 8, количество циклов mH/2
         //  8 число байт в 64 разрядном регистре, считаем 2 слайса за один цикл, количество циклов: mH*8/2 = mH*4 = mH<<2
         "mov        %%r9,    %%rcx              \n"//  загрузка количества циклов mH
         "shl        $2,      %%rcx              \n"//  умножение  %%rcx на *4 // умножение  rcx на *4 + смещение на один (8) цикл
         
         "lp_start:                                \n"
         
         // вычисление адреса транспонированного скана sIntA
         "mov       %%r13,    %%rax;             \n"//  вычисление адреса слайса скана
         
         //"add        %[p],    %%rax;             \n"//  добавление к адресу слайса смещения на переменную p
         
         "lea       (%%rax,%%rdx), %%rax         \n"//  добавление к адресу маски переменной цикла
         //  add     %%rdx,   %%rax ;
         
         "mov       (%%rax),  %%r10              \n"//  загрузка слайса скана в регистр %%r10
         //                 mov             %%r10,  0xFFFFFFFFFFFFFFFF;
         
         // переход, если два подряд идущих слайса (64р) равны нолю (маски и единици не считаем)
         "cmp       $0,       %%r10              \n"
         "je        if_zero                      \n"// je переход если  флаг ноля FZ=0     jz
         
         // вычисление адреса, умножение маски On на транспонированный скан sIntA
         "mov       %%r14,    %%rax              \n"//  вычисление адреса маски maskOnA
         "lea       (%%rax,%%rdx),  %%rax        \n"//  добавление к адресу маски переменной цикла   //add       %%rax, %%rdx;
         "mov       (%%rax),  %%r11              \n"//  загрузка маски On в регистр %%r11
         //                 mov             %%r11,  0xFFFFFFFFFFFFFFFF;
         "and       %%r10,    %%r11              \n"//  применение & маски On к слайсу скана
         
         // вычисление адреса, умножение маски Off на транспонированный скан SInt
         "mov       %%r15,    %%rax              \n"//  вычисление адреса маски maskOffA
         "lea       (%%rax,%%rdx),  %%rax        \n"//  добавление к адресу маски переменной цикла   //add       %%rax, %%rdx;
         "mov       (%%rax),  %%r12              \n"//  загрузка маски maskOffA в регистр r12
         //                 mov             %%r12,  0xFFFFFFFFFFFFFFFF;
         "and       %%r10,    %%r12              \n"//  применение & маски maskOffA к слайсу скана
         
         
         // Регистровые суммы маски On (подсчет единиц) по  X и Y
         "popcnt    %%r11,    %%rax              \n"// подсчет единиц маскированного слайса скана по X, инструкция была добавлена ​​в "Nehalem" (SSE4.2)
         "add       %%rax,    %%r8               \n"// суммирование единиц попавших в маску On в регистре %%r8 (подсчет маскированной площади признака)
         
         // Регистровые суммы маски Off (подсчет единиц) по X и Y
         "popcnt    %%r12,   %%rax               \n"// подсчет количества единиц маскированного слайса скана по X (подсчет горизонтальных сумм)
         "sub       %%rax,   %%r8               \n"// вычитание единиц попавших в маску Off из регистра %%r8
         "if_zero:                                  \n"
         
         
         ////  окончание цикла, шаг цикла 8       // add  +8   // sub -8
         "add             $8,    %%rdx          \n"// в %%rdx находится переменная цикла
         "cmp             %%rcx, %%rdx          \n"// в %%rcx находится количество циклов mH*4
         //"xor        %%r8,    %%r8               \n"   ///отладочная метка
         "jl                   lp_start          \n"// jl перейти, jle перейти, если меньше или равно. Выполнение начинается при нулевом значении счетчика, поэтому выход если меньше количества циклов (mH*4)
        
    
         // заносим разности площадей On и Off в накопительную переменную  deltaSum
         "mov      %%r8,  %[deltaSum]          \n"// сохранение разности площадей On и Off в переменной deltaSum
         "mov      %%r8,  %%r14                \n"// сохранение разности
         
         // метка для возвращаемого значения
         :[deltaSum] "=r"(deltaSum)
         // параметры вызова функции ассемблера с параметрами загрузки
         :[sIntA]"a"((unsigned long long *)(sInt+p)),
          [maskOnA]"b"((unsigned long long *)maskOnA),
          [maskOffA]"c"((unsigned long long *)maskOffA),
          [mH]"d"(mH)
         // зарезервированные регистры
         : "%rax","%rbx","%rcx","%rdx","%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15"
         
         // возврат из режима Assembler
         ); // __asm

    
  // рисовалка коэффициента корреляции  blue    
  #ifdef DEBUGLVL_DRAW
    correlation=(int)deltaSum*100/mask32[0].NMask;
    if(correlation<0)correlation=0;
    drawDataRGB->put(xP-32,yP,0xffff0000+correlation);  // blue
  #endif

    
    if(deltaSum<1) {return 0;}    // return 0; return correlation;
    
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    // проверка mask32[0].NMask<1 выполняется выше в функции setMatrix_Lion
    //if(deltaSum<1||mask32[0].NMask<1)return 0;

    // вычисление коэффициента корреляции и загрузка его значения в КЭШ
    correlation=cacheCor[q]=(int)deltaSum*100/mask32[0].NMask;
    //cacheCor[q]=correlation;
    

    
    return correlation;
        
}//_____________________________________________________________________________

#endif



