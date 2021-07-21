//
//  setMatrix_Point_32_64C.cpp
//  Image2OCR

#include "GBitsetMatrix.h"


//  CPU универсальная ™С™ версия 32/64р  // базовая для отсчета быстродействия

// Вычисление коэффициента корреляции в точке xP, yP.
// универсальная ™С™ версия, работает в 32 и 64 разрядных режимах процессора, подсчитывая суммы единиц в 32х разрядных словах, самая не быстрая
// I1PD958471300.tif 2мин 32сек (по полному времени обсчета страници)

// -----------------------------------------------------------------------------
#ifdef CPU_OS32_64
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
    unsigned int m,p;
    //int sx0,sy0,sx1,sy1;
    int Sb,Si,SumBitOn,SumBitOff,deltaSum;
    int nr=nrows; //nc=ncolumns;  // SH(ncolumns);  SH(nrows);
    int mH=mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    
    
    ///DP(" TIME_START setMatrix_Point ON TEST mask32[0].mH="<<mask32[0].mH<<END);
    
    ///TIME_START
    
    ///mask32->printMask(c_out);         DP(END);DP(END);
    
    //////////// КЕШ  ///////////////
    int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
    //DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");
    
    ////	 	if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
    if ( correlation != 255) { *ch=1; return correlation; }
    
    // TEST
    ////Sb=0xFFFFFFFF;
    
    ///TIME_START
    ////for (int count = 0; count < 50000; count++) {  //проверка быстродействия
    
    // Вычисление коэффициента корреляции в точке xP, yP.
    p=yP+xP*nr;
    SumBitOn=SumBitOff=0;
    // Суммы маски mask32 по  Y
    for ( m=0; m < mH; m++ ) {
        Si=sInt[p+m];
        ////Si=maskCor[p+m];
        if(Si != 0) {
            // Умножение маски на транспонированный скан SInt
            Sb=Si & mask32[0].On[m];
            if(Sb != 0) {                                            //0xFFFFFFFF
                // Регистровые суммы маски mask32.On[m] (подсчет единиц) по  X
                Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
                Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
                Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
                Sb+=Sb >> 8;                               // 16-битовые
                Sb+=Sb >> 16;                              // 32-битовая ячейка
                SumBitOn+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
            }
			// Умножение маски на транспонированный скан SInt
            Sb=Si & mask32[0].Off[m];
            if(Sb != 0) {                                            //0xFFFFFFFF
                // Регистровые суммы маски mask32.Off[m] (подсчет единиц) по  X
                Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
                Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
                Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
                Sb+=Sb >> 8;                               // 16-битовые
                Sb+=Sb >> 16;                              // 32-битовая ячейка
                SumBitOff+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
            }
        } // if
    } // m
    
    ////} //проверка быстродействия
    
    // разность сумм по маскам
    deltaSum=SumBitOn-SumBitOff;
    if(deltaSum<0){deltaSum=0;}
    
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    //if(deltaSum<1||mask32[0].NMask<1)return 0;
    
    // коэффициент корреляции  // КЭШ  //
    //correlation=deltaSum*100/mask32[0].NMask;
    correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
#ifdef DEBUGLVL_DRAW
    if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
        drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
#endif
    
    ///if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
    ///drawDataRGB[0][yP][xP-32] - ((deltaSum)<<8)/(mask32[0].NMask+1); // - s*0.8-20  (SumBit/4)  SumBtMax
    //                                  +4
    
    
    ///TIME_PRINT
    //SH(time);
    
    ///DP("SumBitOn="<<SumBitOn<<" SumBitOff="<<SumBitOff<<" deltaSum"<<deltaSum<<END);
    ///DP(" deltaSum="<<deltaSum<<" mask32[0].NMask"<<mask32[0].NMask<<END);
    //SH(SumBtMaxOff); SH(" ");
    
    ///DM(END); DM(mH<<" mH ")DM(END);
    
    //DP("//__correlation="<<correlation<<END);
    ///DP(" TIME_PRINT setMatrix_Point TEST"); DP(END);
    
    return correlation;
    
    //	   for ( m=mH >> 1;  m > 0;  m-- ) {
    
}//_____________________________________________________________________________
#endif






//  CPU "C" only 64 // в 1,57 раз быстрее

// Вычисление коэффициента корреляции в точке xP, yP.
// ™С™ версия, работает только в 64 разрядном режиме процессора, подсчитывая суммы единиц в 64х разрядных словах,
// I1PD958471300.tif 1мин 37сек быстрее базовой универсальной 32/64р ™С™ версии в 1,57 раза (по полному времени обсчета страници)

// -----------------------------------------------------------------------------
#ifdef CPU_OS64
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
    unsigned int m,p;
    // unsigned long long *p1;
    //int sx0,sy0,sx1,sy1;
    unsigned long long Sb,Si;
    long long SumBitOn,SumBitOff;
    int deltaSum;
    unsigned int nr=nrows;          //nc=ncolumns;  // SH(ncolumns);  SH(nrows);
    unsigned int mH=mask32[0].mH; // mH=(8-128) высота и mW=32 ширина (габарит), битовой маски перефирийной области  "Off"
    
    // m+=2 в главном цикле, поскольку в 64 разрядном режиме процессора с памятью можно общаться только 64 разрядными словами,
    // число подсчитанных слайсов в маске перефирийной области  "Off" по вертикали, в массиве mask32[] округляется до меньшего
    // четного числа т.е. высота маски (прямоугольника поиска) м.б. только четным числом
    
    ///DP(" TIME_START setMatrix_Point ON TEST mask32[0].mH="<<mask32[0].mH<<END);
    
    ///TIME_START
    
    ///mask32->printMask(c_out);         DP(END);DP(END);
    
    //////////// КЕШ  ///////////////
    int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
    //DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");
    
    ////	 	if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
    if ( correlation != 255) { *ch=1; return correlation; }
    
    // TEST
    ////Sb=0xFFFFFFFF;
    
    ///TIME_START
    ////for (int count = 0; count < 50000; count++) {  //проверка быстродействия
    
    // Вычисление коэффициента корреляции в точке xP, yP.
    p=yP+xP*nr;
    SumBitOn=SumBitOff=0;
    // Суммы маски mask32 по  Y
    for ( m=0; m < mH; m+=2 ) {  // m++        // p1=(unsigned long long *)(sInt+p+m);   Si=*p1;
        // приведение типов int 32р к unsigned long long 64р
        Si=*(unsigned long long *)(sInt+p+m);  // Si=sInt[p+m];
        if(Si != 0) {
            // Умножение (&) маски On на транспонированный скан SInt, приведение типов
            Sb=Si & (*(unsigned long long *)(mask32[0].On+m));     // Sb=Si & mask32[0].On[m];
            if(Sb != 0) {                                          // 0xFFFFFFFF
                // Регистровые суммы маски On (подсчет единиц) по  X
                Sb-=(Sb >> 1) & 0x5555555555555555;                // 2-битовые ячейки
                Sb=(Sb & 0x3333333333333333) + ((Sb >> 2) & 0x3333333333333333); // 4-битовые
                Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F0F0F0F0F;                // 8-битоовые
                Sb+=Sb >> 8;                               // 16-битовые
                Sb+=Sb >> 16;                              // 32-битовая ячейка
                Sb+=Sb >> 32;                              // 64-битовая ячейка
                SumBitOn+=Sb & 0xFF;   // & 0xFF обнуляем старшие разряды, содержащие "мусор"
            }
			// Умножение (&) маски Off на транспонированный скан SInt, приведение типов
            Sb=Si & (*(unsigned long long *)(mask32[0].Off+m));    // Sb=Si & mask32[0].Off[m];
            if(Sb != 0) {                                          // 0xFFFFFFFF
                // Регистровые суммы маски Off (подсчет единиц) по  X
                Sb-=(Sb >> 1) & 0x5555555555555555;                // 2-битовые ячейки
                Sb=(Sb & 0x3333333333333333) + ((Sb >> 2) & 0x3333333333333333); // 4-битовые
                Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F0F0F0F0F;                // 8-битоовые
                Sb+=Sb >> 8;                               // 16-битовые
                Sb+=Sb >> 16;                              // 32-битовая ячейка
                Sb+=Sb >> 32;                              // 64-битовая ячейка
                SumBitOff+=Sb & 0xFF;  // & 0xFF обнуляем старшие разряды, содержащие "мусор"
            }
        } // if
    } // m
    
    ////} //проверка быстродействия
    
    // разность сумм по маскам
    deltaSum=SumBitOn-SumBitOff;
    if(deltaSum<0){deltaSum=0;}
    
    // если площадь центральной маски On очень мала (<1), то считать коэфф. корреляции не стоит
    //if(deltaSum<1||mask32[0].NMask<1)return 0;
    
    // коэффициент корреляции  // КЭШ  //
    //correlation=deltaSum*100/mask32[0].NMask;
    correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
#ifdef DEBUGLVL_DRAW
    if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
        drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
#endif
    
    ///if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
    ///drawDataRGB[0][yP][xP-32] - ((deltaSum)<<8)/(mask32[0].NMask+1); // - s*0.8-20  (SumBit/4)  SumBtMax
    //                                  +4
    
    
    ///TIME_PRINT
    //SH(time);
    
    ///DP("SumBitOn="<<SumBitOn<<" SumBitOff="<<SumBitOff<<" deltaSum"<<deltaSum<<END);
    ///DP(" deltaSum="<<deltaSum<<" mask32[0].NMask"<<mask32[0].NMask<<END);
    //SH(SumBtMaxOff); SH(" ");
    
    ///DM(END); DM(mH<<" mH ")DM(END);
    
    //DP("//__correlation="<<correlation<<END);
    ///DP(" TIME_PRINT setMatrix_Point TEST"); DP(END);
    
    return correlation;
    
    //	   for ( m=mH >> 1;  m > 0;  m-- ) {
    
}//_____________________________________________________________________________
#endif


