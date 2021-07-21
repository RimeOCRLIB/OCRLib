//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-

#ifndef _algorithmOCR_32_H
#define _algorithmOCR_32_H

#include "../config.h"
#include "GBitMask.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GBitmap/GBitmap.h"
#include "../OCRString/php2stl.h"
#include <string.h>
#include <fstream>



using namespace ocr;


namespace ocr {
    //******************************************************************************
    // РАСПАКОВКА
    
    // Распаковка битовой маски из массива 32 int  в массив 1024 bool
    // -----------------------------------------------------------------------------
    void GBitMask32::unPackBitMask(unsigned int *MaskInt,
                                   unsigned char *MaskDest){
        
        // MaskInt входной массив - битовая маска из 32 int, упакованная в массив "MaskInt"
        // MaskDest выходной массив 1024 char для вывода на экран
        // "maskSize" размерность битовой маски (32,64)
        
        int y,x,d,s;
        
        DC("START UNPACK"<<" W="<<mW<<" H="<<mH<<END);
        //////////////////////////////// ////////////////////////////////////////////////
        
        /*
         // TEST,  квадрат 32*32   15 и 16  строки состоят из единиц и т.д.
         for( x=0; x < w; x++ ){ MaskInt[x]=0; }
         for( x=0; x < w*w; x++ ){ MaskDest[x]=0;}
         MaskInt[0]=0x000000FF;   MaskInt[1]=1<<1;   MaskInt[2]=2;
         MaskInt[15]=MaskInt[16]=0xFFFFFFFF;    MaskInt[31]=1<<31;
         
         
         // 11111111000000000000000000000000
         // 01000000000000000000000000000000
         // 01000000000000000000000000000000
         // ................................
         // 00000000000000000000000000000000
         // 11111111111111111111111111111111
         // 11111111111111111111111111111111
         // 00000000000000000000000000000000
         // 00000000000000000000000000000000
         // ................................
         // 00000000000000000000000000000001
         */
        
        // РАСПАКОВКА
        // перепаковка битовой маски упакованной в массив 32 int  в массив 1024 bool
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;     s=(s >> 1);  // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("unPackBitMask output array"<<END);
        // DC(END);  // TABL
        //for ( x=0; x < mW*mH; x++ ) {
        //DC((int)MaskDest[x]);
        //	 if(MaskDest[x]==1){DC("1");}else{DC(".")}
        //if ((x+1)%mW==0)DC(END);
        //  }
        //  DC(END);
        /**/
        ////////////////////////////////////////////////////////////////////////////////
        DC("UNPACK DONE "<<END); DC(END);
    }//_____________________________________________________________________________
    
    // ЗАПАКОВКА
    
    // Запаковка битовой маски в массиве 1024 bool в массив  32 int. Вход1
    // -----------------------------------------------------------------------------
    
    void GBitMask32::packBitMask(unsigned int *MaskInt,
                                 unsigned char *inputData){
        // inputData входной массив - битовая маска из 32 int, упакованная в массив "inputData"
        // MaskInt  выходной массив 1024 char для вывода на экран
        // "maskSize" размерность битовой маски (32,64)
        
        
        int y,x,d;
        unsigned int reg, s;
        int ncVnr=mW*mH;
        
        DC("START PACK 1"<<END);
        ////////////////////////////////////////////////////////////////////////////////
        
        //автоопределение количества градаций
        int max=0;
        for ( x=0; x < ncVnr; x++ ) {
            s=inputData[x];
            if ( s > max ) max=s;
        }
        
        // бинаризация входного массива char
        int NMask=0;
        if(max>1){
            for ( x=0; x < ncVnr; x++ ) {
                if ( inputData[x]>127 ){ inputData[x]=0; }
                else { inputData[x]=1; NMask++; } // количество единиц в маске
            }
        }
        
        // ЗАПАКОВКА
        // Запаковка битовой маски в массиве 1024 bool в массив  32 int
        reg=0;
        for( y=0; y < mH; y++ ){
            d=y*mW;
            for ( x=0; x < mW; x++ ) {
                reg=(reg << 1) + inputData[d + x];  // обратный порядок пикселей
                ///					 s=(inputData[d + x])<<31; // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
                ///					 reg=s + (reg >> 1) ;           // прямой порядок пикселей
            } // x
            MaskInt[y]=reg;
        } // y
        /**/
        
        DC(NMask <<" NMask count of true value in mask "<<END);
        DC("packBitMask output array"<<END);
        DC(END);  // TABL
        for ( x=0; x < ncVnr; x++ ) {
            //DC((int)inputData[x]);
            if(inputData[x]==1){DC("1");}else{DC(".")}
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);
        
        /*					// Контрольныя распоковка //
         ////////////////////////////////////////////////////////////////////////////////
         
         bool *BolS;
         // запрос памяти
         BolS=(bool*)farcalloc(ncVnr+64, 1);
         // выход в случае отсутствия свободной памяти запрашиваемого размера
         if (BolS==NULL) {DC("BolS"); exit (1);}
         
         // РАСПАКОВКА
         // перепаковка битовой маски упакованной в массив 32 int  в массив 1024 bool
         for( y=0; y < mH; y++ ){
         s=MaskInt[y];   d=y*mW;
         for( x=0; x < mW; x++ ){
         (BolS[d + x])=s & 1;    s=(s >> 1);
         } // x
         } // y
         
         
         DC("packBitMask упакованный массив TMP"<<END);
         DC("MaskInt"<<END);  // TABL
         for ( x=0; x < mH; x++ ) {
         DC(MaskInt[x]<<" "); if ((x+1)%mH==0)DC(END);
         }
         DC(END);
         
         DC("packBitMask выходной массив TMP"<<END);
         DC(END);  // TABL
         for ( x=0; x < ncVnr; x++ ) {
         DC(BolS[x]<<" "); if ((x+1)%mW==0)DC(END);
         }
         DC(END);
         
         //  освобождение памяти
         if ( BolS !=NULL ) farfree(BolS);
         */
        
        ////////////////////////////////////////////////////////////////////////////////
        DC("PACK DONE 1"<<END); DC(END);
        /**/
        
    }//_____________________________________________________________________________
    
    
    // ГРАФИКА
    // Вывод на экран битовых масок ON и OFF упакованных в массив 64 long int. Вход1
    // -----------------------------------------------------------------------------
    GBitmap* GBitMask32::unPackMask(){
        
        int y,x,d;
        unsigned int s;
        
        // входной массив - битовая маска из 64 long int, упакованная в массив "MaskInt"
        unsigned int *MaskInt=On;
        unsigned char c;
        GBitmap *img=GBitmap::create(mW,mH);	
        unsigned char *inputData=img[0].bytes_data;
        
        
        DC("START printMask64 ON 1"<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                c=s & 1; s=(s >> 1); // обратный порядок пикселей
                (inputData[d + mW-x-1])=255-255*c;
            } // x
        } // y
        
        return img;
        
    }//_____________________________________________________________________________
    
    // ГРАФИКА
    // Вывод на экран битовых масок ON и OFF упакованных в массив 64 long int. Вход1
    // -----------------------------------------------------------------------------
    GBitmap* GBitMask32::unPackMaskOff(){
        
        int y,x,d;
        unsigned int s;
        
        // входной массив - битовая маска из 64 long int, упакованная в массив "MaskInt"
        unsigned int *MaskInt=Off;
        unsigned char c;
        GBitmap *img=GBitmap::create(mW,mH);	
        unsigned char *inputData=img[0].bytes_data;
        
        DC("START printMask64 ON 1"<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                c=s & 1; s=(s >> 1); // обратный порядок пикселей
                (inputData[d + mW-x-1])=255-255*c;
            } // x
        } // y
        
        return img;
        
    }//_____________________________________________________________________________
    
    // OFF //
    //Формирование маски с прямоугольной маской Off dlt-ширина бордюра,
    //дополняющего маску On до прямоугольной маски Off
    void GBitMask32::setOnOffInMaskRect(int dlt) {
        // -----------------------------------------------------------------------------
        
        int y,x,p;
        int mx0,mx1,my0,my1;
        // mH(8-96) высота и mW (32) ширина (габарит) битовой маски перефирийной области  "Off"
        mW=32;                   // ширмна (фиксированная) прямоугольника маски
        unsigned int ncVnr=mW*mH;
        unsigned char *BolS;
        // запрос памяти
        BolS=(unsigned char*)malloc(ncVnr+64);
        // выход в случае отсутсттвия свободной памяти запрашиваемого размера
        if (BolS==NULL) {DC("BolS"); exit (1);}
        
        
        // Распаковка битовой маски из массива 32 int  в массив 1024 bool
        unPackBitMask(On, BolS);
        // mask32 - указатель на объект "маска"  в массиве mask32
        
        // вычисление максимальных и минимальных габаритов центральной области "ON"
        mx0=my0=128; mx1=my1=0;
        for ( y=0; y < mH; y++ ) {   // wr
            p=y*mW;
            for ( x=0; x < mW; x++ ) {
                if( BolS[x+p] == 1 ) {
                    if( x >= mx1 ) { mx1=x; }   if( x <= mx0 ) { mx0=x; }
                    if( y >= my1 ) { my1=y; }   if( y <= my0 ) { my0=y; }
                }
            } // x
        } // y
        
        // dlt=2;
        // dlt отступ от края центральной области "ON" влево, вправо, в верх, в низ.
        if( dlt < 1 ) { dlt=1; }  if( dlt > mW) { dlt=mW; }
        mx0=mx0-dlt;  mx1=mx1+dlt+1;  my0=my0-dlt;  my1=my1+dlt+1;
        
        // ограничения
        if( mx0 <= 0 ) { mx0=0; }    if( mx1 >= mW ) { mx1=mW; }
        if( my0 <= 0 ) { my0=0; }    if( my1 >= mH ) { my1=mH; }
        
        // формирование перефирийной области  "Off"
        for ( y=my0; y < my1; y++ ) {
            p=y*mW;
            for ( x=mx0; x < mx1; x++ ) {
                BolS[x+p]=1-BolS[x+p];
            } // x
        } // y
        /**/
        
        /*
         DC("setOnOffInMask выходной массив 1024"<<END);
         DC(END);  // TABL
         for ( x=0; x < ncVnr; x++ ) {
         DC((int)BolS[x]<<""); if ((x+1)%mW==0)DC(END);
         }
         DC(END);
         */
        
        // Запаковка битовой маски в массиве 1024 BolS в массив  32 bit,   wr !=32
        packBitMask(Off, BolS);
        
        
        //  освобождение памяти
        
        if ( BolS !=NULL ) free(BolS);
        /**/
    }//_____________________________________________________________________________
    
    // ПСЕВДОГРАФИКА
    // Вывод на экран битовых масок ON и OFF упакованных в массив 32 int. Вход1
    // -----------------------------------------------------------------------------
    void GBitMask32::printMask(ofstream &c_out){
        
        int y,x;
        int d,s;
        
        // выходной массив 1024 char для вывода на экран
#ifdef WIN32
        //unsigned char *MaskDest=(unsigned char*)farcalloc(mW*mH,1);  //4096=wr*wc=32*128;
        unsigned char MaskDest[128*32];
#else
        unsigned char *MaskDest=(unsigned char*)malloc(mW*mH);  //4096=wr*wc=32*128;
#endif
        // входной массив - битовая маска из 32 int, упакованная в массив "MaskInt"
        unsigned int *MaskInt=On;
        
        DC("START printMask ON 1 NMask="<<NMask<<" dlt0="<<dlt0<<" dlt1="<<dlt1<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("printMask output array ON"<<END);
        DC(END);  // TABL
        int count=0;
        for ( x=0; x < mW*mH; x++ ) {
            if(MaskDest[x]==1){DC("1");count++;}else{DC(".")}  // DC((int)MaskDest[x]<<"");
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);  DC("@@@ count="<<count<<END);
        
        MaskInt=Off;
        
        DC("START printMask OFF 1"<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("printMask output array OFF"<<END);
        DC(END);  // TABL
        for ( x=0; x < mW*mH; x++ ) {
            if(MaskDest[x]==1){DC("1");}else{DC(".")}
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);
        /**/
#ifdef WIN32
        //if ( MaskDest !=NULL ) farfree(MaskDest);
#else
        if ( MaskDest !=NULL ) free(MaskDest);
#endif
        DC("xMask="<<xMask<<" yMask="<<yMask<<END);
        DC("print DONE "<<END); DC(END);
    }//_____________________________________________________________________________
    
    //отличается от предыдущей только DT() нужна для вывода в базе при отключенной псевдографике
    // ПСЕВДОГРАФИКА
    // Вывод на экран битовых масок ON и OFF упакованных в массив 32 int. Вход1
    // -----------------------------------------------------------------------------
    void GBitMask32::printMask1(ofstream &c_out){
        
        int y,x;
        int d,s;
        
        // выходной массив 1024 char для вывода на экран
#ifdef WIN32
        unsigned char MaskDest[32*128];//=(unsigned char*)farcalloc(mW*mH,1);  //4096=wr*wc=32*128;
#else
        unsigned char *MaskDest=(unsigned char*)malloc(mW*mH);  //4096=wr*wc=32*128;
#endif
        // входной массив - битовая маска из 32 int, упакованная в массив "MaskInt"
        unsigned int *MaskInt=On;
        
        DC("START printMask ON 1 NMask="<<NMask<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("printMask output array ON"<<END);
        DC(END);  // TABL
        int count=0;
        for ( x=0; x < mW*mH; x++ ) {
            if(MaskDest[x]==1){DC("1");count++;}else{DC(".")}  // DC((int)MaskDest[x]<<"");
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);  DC("@@@ count="<<count<<END);
        
        MaskInt=Off;
        
        DC("START printMask OFF 1"<<END);
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("printMask output array OFF"<<END);
        DC(END);  // TABL
        for ( x=0; x < mW*mH; x++ ) {
            if(MaskDest[x]==1){DC("1");}else{DC(".")}
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);
        /**/
#ifdef WIN32
        //if ( MaskDest !=NULL ) farfree(MaskDest);
#else
        if ( MaskDest !=NULL ) free(MaskDest);
#endif
        
        DC("xMask="<<xMask<<" yMask="<<yMask<<END);
        DC("print DONE "<<END);
    }//_____________________________________________________________________________
    
    
    
    // ПСЕВДОГРАФИКА
    // Вывод на экран битовых масоки упакованных в массив 32 int. Вход2
    // -----------------------------------------------------------------------------
    void GBitMask32::printMask(unsigned int *MaskInt, ofstream &c_out){
        
        int y,x;
        int d,s;
        // выходной массив 1024 char для вывода на экран
#ifdef WIN32
        //unsigned char *MaskDest=(unsigned char*)farcalloc(mW*mH,1);
        unsigned char MaskDest[32*128];
#else
        unsigned char *MaskDest=(unsigned char*)malloc(mW*mH);
#endif
        // входной массив - битовая маска из 32 int, упакованная в массив "MaskInt"
        
        
        DC("START print 2"<<END);
        
        // перепаковка битовой маски упакованной в масссив 32 int  в массив 1024 bool
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DC("printMask output array"<<END);
        DC(END);  // TABL
        for ( x=0; x < mW*mH; x++ ) {
            //DC((int)MaskDest[x]<<"");
            if(MaskDest[x]==1){DC("1");}else{DC(".")}
            if ((x+1)%mW==0)DC(END);
        }
        DC(END);
        /**/
        DC("print DONE 2"<<END); DC(END);
#ifdef WIN32
        //if(MaskDest!=NULL)farfree(MaskDest);
#else
        if(MaskDest!=NULL)free(MaskDest);
#endif
    }//_____________________________________________________________________________
    
    // ПСЕВДОГРАФИКА
    // Вывод на экран битовых масоки упакованных в массив 32 int. Вход2
    // -----------------------------------------------------------------------------
    void GBitMask32::printMask(unsigned int *MaskInt){
        
        int y,x;
        int d,s;
        int print=1;
        // выходной массив 1024 char для вывода на экран
#ifdef WIN32
        //unsigned char *MaskDest=(unsigned char*)farcalloc(mW*mH,1);
        unsigned char MaskDest[32*128];
#else
        unsigned char *MaskDest=(unsigned char*)malloc(mW*mH);
#endif
        // входной массив - битовая маска из 32 int, упакованная в массив "MaskInt"
        
        
        DR("START print 2"<<END);
        
        // перепаковка битовой маски упакованной в масссив 32 int  в массив 1024 bool
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///(MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                (MaskDest[d + mW-x-1])=s & 1; s=(s >> 1); // обратный порядок пикселей
            } // x
        } // y
        
        DR("printMask output array"<<END);
        DR(endl);  // TABL
        for ( x=0; x < mW*mH; x++ ) {
            //DC((int)MaskDest[x]<<"");
            if(MaskDest[x]==1){DR("1");}else{DR(".")}
            if ((x+1)%mW==0)DR(endl);
        }
        DR(END);
        /**/
        DR("print DONE 2"<<END); DC(END);
#ifdef WIN32
        //if(MaskDest!=NULL)farfree(MaskDest);
#else
        if(MaskDest!=NULL)free(MaskDest);
#endif
    }//_____________________________________________________________________________
    
    
    void GBitMask32::printMask(){
        
        cout<<"START printMask ON 1 NMask="<<NMask<<endl;
        cout<<"xMask="<<xMask<<" yMask="<<yMask<<" mH="<<mH<<endl;
        
        cout<<"printMask output array ON"<<endl;
        for(int y=0; y < mH; y++ ){
            binaryPrint(On[y],2); cout<<endl;
        }
        
        cout<<"START printMask OFF 1"<<endl;
        for(int y=0; y < mH; y++ ){
            binaryPrint(Off[y],2); cout<<endl;
        }
        
        
        cout<<"print DONE "<<endl;
    }//_____________________________________________________________________________
    
    
    
    ////// НЕ ИСПОЛЬЗУЕМ  //////////
    // NormalT( mask32, c_out);
    
    // программа нормализации положения перефирийной области  "Off" и центральной
    // области  "On"  (сдвиг вверх). Подпрограмма для MatrixT.
    // -----------------------------------------------------------------------------
    void GBitMask32::NormalT() {
        int y,x,p;           //,d,p,q
        //unsigned int Sb;
        unsigned int xmin=128, xmax=0, ymin=128, ymax=0;
        int print=0;
        
        DC(END); DT(" TIME_START NormalT  ");
        
        /*  //////////// сдвиг по вертикали выключен
         
         // вычисление максимальных и минимальных габаритов по высоте
         // перефирийной области  "Off"
         for ( y=0; y < mH; y++ ) {
         Sb=BufOff[y];
         if( Sb > 0 ) {
         if( y > ymax ) { ymax=y;  }   if( y < ymin ) { ymin=y;  }
         } // if(
         } // y
         
         // ymin  минимальный размер по Y перефирийной области "Off"
         // ymax  максимальный размер по Y перефирийной области "Off"
         // ymax-ymin; новый минимальнай габарит по Y перефирийной области "Off"
         
         // Sbmin  слово с минимальной координатой по X перефирийной области "Off"
         // Sbmax  слово с максимальный координатой по X перефирийной области "Off"
         
         yMask+=ymin;                //  mask32[0].y-=y0;   =0
         
         //  сдвиг и вычисленние вертикального габарита по Y перефирийной области "Off"
         
         // сдвиг вверх на ymin, по Y перефирийной области  "Off"  и "On"
         for ( y=ymin; y < mH; y++ ) {
         BufOff[y-ymin]=BufOff[y];    BufOn[y-ymin]=BufOn[y];
         } // y
         
         // Обнуляем оставшиеся копии последних регистров
         for ( y=mH-ymin; y < mH; y++ ) { BufOff[y]=BufOn[y]=0; }
         
         // новый (минимальнай) габарит маски "Off" по высоте  1-128
         mH=ymax-ymin+1;
         
         */  //////////////  сдвиг по вертикали выключен
        
        DT(" min, max размер по Y перефирийной обласи Off "<<END);
        //DC("/@@@@/"<<ymin<<" ymin "<<" ymax="<<ymax<<" yMask="<<yMask<<END);
        
        // unsigned int ncVnr=32*mH;
        unsigned char BolS[4096];
        //BolS=(unsigned char*)malloc(ncVnr+64);
        // выход в случае отсутсттвия свободной памяти запрашиваемого размера
        //if (BolS==NULL) {DT("BolS==0"); exit (1);}
        
        // Распаковка битовой маски из массива 32 int  в массив char
        DT("mWOn="<<mWOn<<" mHOn="<<mHOn<<END);
        
        unPackBitMask(On, BolS);
        DT("@1");
        
        // вычисление максимальных и минимальных габаритов произвольной
        // входной области (mWImg*mH) < 32, выделенной мышкой
        xmin=128; xmax=0; ymin=128; xmax=0;    //
        for ( y=0; y < mH; y++ ) {
            p=y*mW;
            for ( x=0; x < 32; x++ ) {
                if( BolS[x+p] == 1 ) {
                    if( x > xmax ) { xmax=x; }   if( x < xmin ) { xmin=x; }
                    if( y > ymax ) { ymax=y; }   if( y < ymin ) { ymin=y; }
                }
            } // x
        } // y
        
        DT(" xmax "<<xmax<<" xmin="<<xmin<<" ymax "<<ymax<<" ymin="<<ymin<<endl);
        
        //  xmin  минимальная координата по X перефирийной области "Off"
        //  xmax  максимальный координата по X перефирийной области "Off"
        //  Xmax-Xmin;  габарит по X перефирийной области "Off"
        
        mWOn=xmax-xmin;
        mHOn=ymax-ymin;
        
        
        //  освобождение памяти
        //if ( BolS !=NULL ) free(BolS);
        
        DT("done NormalT"<<endl);
        //printMask(c_out);
        
        /**/
        
        /**/
        ////////////////////////////////////////////////////////////////////////////////
        /*
         // вычисление максимальных и минимальных габаритов, по Y
         // перефирийной области  "Off"
         int ymin=128, ymax=0, Sb;
         for ( y=0; y < mH; y++ ) {
         Sb=BufOff[y];
         // Регистровые суммы маски mask32 по  X    //0xFFFFFFFF
         Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
         Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
         Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
         Sb+=Sb >> 8;                               // 16-битовые
         Sb+=Sb >> 16;                              // 32-битовая ячейка
         Sb=Sb & 0x3F;  // 31 Обнуляем старшие разряды, содержащие "мусор"
         if( Sb > 0 ) { if( y > ymax ){ ymax=y; }  if( y < ymin ){ ymin=y; } }
         } // y
         */
        
        /*
         // сдвиг в центр максимальных и минимальных габаритов центральной области "ON"
         // и перефирийной области  "Off"
         int dx=(mW-x0-x1)/2,  dy=(mH-y0-y1)/2;
         mask32[0].x=x+dx;     mask32[0].y=y+dy;
         //DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(END);
         dy=dy*mW;
         for ( y=0; y < mH; y++ ) {
         p=y*mW;
         for ( x=0; x < mW; x++ ) {
         d=x+dx+p+dy;
         if( d > ncVnr || d < 0  ) break;
         BufMask[d]=inputData[x+p];
         } // x
         } // y
         */
        //DC("NormalT "<<x0<<" x0 "); DC(x1<<" x1 "); DC(y0<<" y0 "); DC(y1<<" y1 ");  DC(END);
        
        DC(" TIME_PRINT NormalT  "); DC(END);
        //	в левый верхний угол
        /**/
    }//_____________________________________________________________________________
    
    
    //Подсчет количестваа единиц в маске On
    int  GBitMask32::maskCount(){
        int y,x,d;
        unsigned int s;
        NMask=0;
        // входной массив - битовая маска из 64 long int, упакованная в массив "MaskInt"
        unsigned int *MaskInt=On;
        unsigned char c;
        for( y=0; y < mH; y++ ){
            s=MaskInt[y];   d=y*mW;
            for( x=0; x < mW; x++ ){
                ///				  (MaskDest[d + x])=s & 1;      s=(s >> 1); // прямой порядок пикселей
                c=s & 1; s=(s >> 1); // обратный порядок пикселей
                NMask+=c;
            } // x
        } // y
        
        return NMask;
        
    } ///_______________________________
    
    
    // формирование обтекающей перефирийной области  "Off" с переходной областью
    // -----------------------------------------------------------------------------
    void GBitMask32::setOnOffMatrixT(int stepH,
                                     int stepW){
        
        NMask=0;
        
        DC("/@@00@@@/cross mask"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
        
        //signed int BufOn[128];    memset(BufOn,0,128);     // обнуление
        
        
        
        // перефирийной области  "Off"      // переходной области
        //  1 <= dlt <=5          // -2 <= dlt <=-1
        // размер и форма маски на высокое  разрешение ( 600 dpi )
        //DT("maskType="<<maskType<<END);
        
        //if(maskType==10){maskType=1;    dlt0=3;                   dlt1=-2;}           // автоопреденение
        
        // maskType=0  размер и форма маски на малое разрешение ( <300 dpi )
        //if(maskType==9){maskType=0;    dlt0=3;                   dlt1=-1;}
        
        // maskType=0  размер и форма маски на малые объекты  ( точки )
        //if(maskType==8){maskType=0;    dlt0=5;                   dlt1=-2;}
        
        
        // TipMasc 0,1,2
        // TipMasc=1  размер и форма маски на высокое  разрешение ( 600 dpi )
        // с двухсторонней переходной областью между "On" и "Off"
        // TipMasc=0  размер и форма маски на малое разрешение ( <300 dpi )
        // с односторонней переходной областью между "On" и "Off"
        //TipMasc=0;
        //if (stepH > 1 || stepW > 1 ) { maskType=0; }
        DC("maskType="<<maskType<<" dlt0"<<dlt0<<" dlt1"<<dlt1<<END);
        
        // автоматическое опреденение размера маски (по габаритам буквы)
        ///int x0,x1,y0,y1;         // габариты буквы
        ///dlt=0.3*(x1-x0+y1-y0)/2; // часть среднего размера буквы
        
        // внешний размер маски Off на dlt пикселей больше маски On
        ///if (TipMasc==1) { dlt=3; }   //  1 <= dlt <=5          // автоопреденение
        ///if (TipMasc==0) { dlt=2; }   //  2 <= dlt <=5
        
        
        // формирование обтекающей перефирийной области  "Off"
        MatrixOff(dlt0);
        
        // формирование прямоугольной перефирийной области  "Off"
        ///			  setOnOffInMask(mask32[0], dlt, c_out);               // mask32[0]
        //  setOnOffInMask(&mask,2,c_out);
        //c_out<<"/_M1_/"<<endl;
        //printMask(c_out );
        
        // Ширина переходной области между "On" и "Off". Она не должна
        // съедать маску "Off" т.е. увеличивая толщину переходная облость
        // надо увеличивать толщину маски "Off" (автоопреденение -dlt=dlt/2;)
        ///if (TipMasc==1) { dlt=-1; }  // =abc(dlt+dlt) // -2 <= dlt <=-1
        ///if (TipMasc==0) { dlt=-1; }  // =abc(dlt)     // -2 <= dlt <=-1
        
        // получение маски переходной области между "On" и "Off"
        //MatrixOn(dlt1, c_out);
        
        // Подпрограмма уменьшения маски Off на величину dlt2
        MatrixDltOff(dlt2);
        
        // Подпрограмма уменьшения маски On на величину dlt1
        MatrixDltOn(dlt1);    // dlt2
        
        //c_out<<"/_M1_/"<<endl; 
        //printMask();   // работает  ///////////////////////
        
        // программа вычисления габаритов области центральной  области  "On"                                                           //
        NormalT(); 
        
        NMask_prim=maskCount();
        
        //NMask=NMask1;
        // printMask(c_out );   // работает  ///////////////////////
        
    }//_____________________________________________________________________________
    
    
    // формирование
    /*// -----------------------------------------------------------------------------
     void GBitMask32::setOnOffMatrixT(ofstream &c_out){
     
     setOnOffMatrixT(c_out,1,1);
     }//_____________________________________________________________________________
     */
    // Запаковка битовой маски из битмапа
    // -----------------------------------------------------------------------------
    void GBitMask32::packBitMask(GBitmap *img){
        
        int y,x,p,q,d,i;
        //int mx0,mx1,my0,my1;
        unsigned int reg;
        int mWImg=img->columns();    // произвольные размеры картинки по ширмне
        int mHImg=img->rows();       // произвольные размеры картинки по высоте
        mW=32;                   // ширмна (фиксированная) прямоугольника маски
        mH=mHImg;                // высота прямоугольника маски
        imgW=mWImg;
        int ncVnr=mW*mH, ncInr=mWImg*mH;
        unsigned int  *MaskInt0=imgOn;
        unsigned int  *MaskInt1=On;
        unsigned char *inputData=img[0].bytes_data;
        
        // промежуточный массив >1024 char
        //#ifdef WIN32
        unsigned char BufMask[4096];//=(unsigned char*)farcalloc(ncVnr,1);
        memset(BufMask,0,ncVnr);    // memset(BufMask,0,szf*(ncVnr));
        
        
        DC(END); DC(END<<"START 32PACK 2 mWImg="<<mWImg<<END);
        
        //автоопределение количества градаций
        int max=0;
        for ( x=0; x < ncInr; x++ ) {
            if ( inputData[x]>max )max=inputData[x];
        }
        
        // бинаризация входного массива char
        DC("/1/___"<<END);
        if(max>1){
            for ( x=0; x < ncInr; x++ ) {
                if ( inputData[x]>127 ){ inputData[x]=0; }
                else { inputData[x]=1;}
                if(inputData[x]==1){DC("1");}else{DC(".")}
                if ((x+1)%mWImg==0)DC(END);
            }  // x
        }  // if
        
        
        /*
         // Тест - сумма единиц по периметру X Y
         for ( x=0; x < ncInr; x++ ) { BufMask[x]=0; }
         BufMask[0]=BufMask[mWImg-1]=BufMask[ncInr-mWImg]=BufMask[ncInr-1]=1;
         */
        
        // допустимое количество пикселей для срабатывания сторон маски пересечений,
        // сравнивается раздельно для каждой стороны прямоугольной маски пересечений
        int porogS=1;
        
        //							 X0      X1      Y0       Y1
        //							 11		 00		 10		  O1
        //							 00		 11		 10		  O1
        
        // сумма единиц sumX0 по периметру X0 прямоугольной маски пересечений
        int sumX0=0, sumX1=0, sumY0=0, sumY1=0;
        for ( x=0; x < mWImg; x++ ) { sumX0+=inputData[x]; } // Тест BufMask[x]
        // сумма единиц sumX1 по периметру X1 прямоугольной маски пересечений
        for ( x=ncInr-mWImg; x < ncInr; x++ ) { sumX1+=inputData[x]; }
        
        // сумма единиц sumY0 по периметру Y0 прямоугольной маски пересечений
        for ( y=0; y < ncInr; y+=mWImg ) { sumY0+=inputData[y]; }
        // сумма единиц sumY1 по периметру Y1 прямоугольной маски пересечений
        for ( y=mWImg-1; y < ncInr; y+=mWImg ) { sumY1+=inputData[y]; }
        
        DC(END<<" суммы пересечений "<<sumX0<<" sumX0 "<<sumX1<<" sumX1 "<<sumY0<<" sumY0 "<<sumY1<<" sumY1 "<<END);
        
        /*
         // Вывод на экран теста - сумма единиц по периметру X Y
         DC(END);
         for ( x=0; x < ncInr; x++ ) {
         if(BufMask[x]==1){DC("1");}else{DC(".")}
         if ((x+1)%mWImg==0) DC(END);
         } // x
         DC(END);
         memset(BufMask,0,ncVnr); // обнуление BufMask
         */
        
        
        // вычисление максимальных и минимальных габаритов произвольной
        // входной области (mWImg*mH) < 32, выделенной мышкой
        x0=128; y0=128; x1=0; y1=0;
        for ( y=0; y < mH; y++ ) {
            p=y*mWImg;
            for ( x=0; x < mWImg; x++ ) {
                if( inputData[x+p] == 1 ) {
                    if( x > x1 ) { x1=x; }   if( x < x0 ) { x0=x; }
                    if( y > y1 ) { y1=y; }   if( y < y0 ) { y0=y; }
                }
            } // x
        } // y
        
        DC(END<<" маска пересечений "<<x0<<" x0 "<<x1<<" x1 "<<y0<<" y0 "<<y1<<" y1 "<<END);
        
        // величина сдвига входной области в центр
        //////////////int dx=(mW-x0-x1)/2,     dy=(mH-y0-y1)/2;
        
        int dx=0,     dy=0;  /////////////////////////////////////////////
        
        DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(mW<<" mW "); DC(mH<<" mH "); DC(END);
        
        // x0,x1,y0,y1 габариты прямоугольной однопиксельной маски
        // пересечений c учетом сдвига в центр
        x0st=0; x1st=0; y0st=0; y1st=0;
        if ( sumX0 >=porogS ) { x0st=1; }
        if ( sumX1 >=porogS ) { x1st=1; }
        if ( sumY0 >=porogS ) { y0st=1; }
        if ( sumY1 >=porogS ) { y1st=1; }
        
        DC("x0st="<<(short)x0st<<" x1st="<<(short)x1st<<" y0st="<<(short)y0st<<" y1st="<<(short)y1st<<END);
        
        x0+=dx;  x1+=dx;
        y0+=dy;  y1+=dy;
        DC("/@@_0_@@@/маска пересечений"<<"x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
        /**/
        /*
         // если входная область соприкасается с краеми то её не сдвигаем
         if(sumX0>=porogS || sumX1>=porogS) dy=0;  if(sumY0>=porogS) dx=0;
         // если входная область соприкасается с правым краем то её
         // обязательно сдвигаем  вправо на mW-mWImg
         if(sumY1>=porogS) dx=mW-mWImg;
         if(sumY0>=sumY1+1) { dx=0; }
         DC(END); DC(sumY0<<" sumY0 "); DC(sumY1<<" sumY1 "); DC(dx<<" dx "); DC(END);
         */
        DC("/@@@@/ yMask="<<yMask<<" dy="<<dy<<END);
        DC("/@@@@/ xMask="<<xMask<<" dx="<<dx<<END);
        xMask-=dx;        yMask-=dy;
        //DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(END);
        
        // сдвиг в центр по максимальным и минимальным габаритам входной
        // области (mWImg*mH) и пересчет её размеров на полную маску (mW*mH)
        dy=dy*mW;
        for ( y=0; y < mH; y++ ) {
            p=y*mW;  q=y*mWImg;  i=0;
            for ( x=0; x < mWImg; x++ ) {
                d=x+dx+p+dy;
                //if( i < mWImg ) { i++; }
                if( d > ncVnr || d < 0  ) break;
                BufMask[d]=inputData[x+q];    // i+q
            } // x
        } // y
        /**/
        
        /*
         // сдвиг в центр максимальных и минимальных габаритов центральной облласти "ON"
         int dx=(mW-x0-x1)/2,  dy=(mH-y0-y1)/2;
         mask32[0].x=x+dx;     mask32[0].y=y+dy;
         //DC(END); DC(dx<<" dx "); DC(dy<<" dy "); DC(END);
         dy=dy*mW;
         for ( y=0; y < mH; y++ ) {
         p=y*mW;
         for ( x=0; x < mW; x++ ) {
         d=x+dx+p+dy;
         if( d > ncVnr || d < 0  ) break;
         BufMask[d]=inputData[x+p];
         } // x
         } // y
         */
        DC(END<<"/@@@@/ M32 "<<dx<<" dx "<<dy<<" dy*mW "<<" ncVnr="<<ncVnr<<" mH="<<mH<<" mW="<<mW<<" yMask="<<yMask<<END);
        
        // ЗАПАКОВКА
        // Запаковка однобитовой маски из массива mW*mH bool (1 байт)
        // в массив mH 32 разрядных слов unsigned int (4 байта)
        reg=0;
        for( y=0; y < mH; y++ ){
            d=y*mW;
            for ( x=0; x < mW; x++ ) {
                reg=(reg << 1) + BufMask[d + x]; // обратный порядок пикселей  // + BufMask[d + x]
                ///					 s=(BufMask[d + x])<<31; // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
                ///					 reg=s + (reg >> 1) ;               // прямой порядок пикселей
            } // x
            MaskInt0[y]=reg;
            MaskInt1[y]=reg;
        } // y
        
        
        //DC("/2/___"<<END);
        // Вывод на экран битовых масок ON и OFF упакованных в массив mH unsigned int(32p)
        //printMask(mask32,c_out);
        
        // TipMasc=1  размер и форма маски на высокое  разрешение ( 600 dpi )
        // TipMasc=0  размер и форма маски на малое разрешение ( <300 dpi )
        DC("/@@_01_@@@/маска пересечений"<<"x0="<<x0 <<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<END);
        setOnOffMatrixT(1,1);  ///////////////////////
        
        /**/
        
        //DC("packBitMask входной массив"<<END);
        //DC(END);  // TABL
        //for ( x=0; x < ncVnr; x++ ) {
        //	 //DC((int)inputData[x]);
        //	 if(inputData[x]==1){DC("1");}else{DC(".")}
        //	 if ((x+1)%mW==0)DC(END);
        //}
        //DC(END);
        
        
        DC("PACK 32DONE 2"<<END); DC(END);
        DC("///@@@@ NMask="<<NMask<<"NMask_prim="<<NMask_prim<<END);
    }//_____________________________________________________________________________
    
    
    void GBitMask32::packBitMask(){
        
        unsigned int  *MaskInt0=imgOn;
        unsigned int  *MaskInt1=On;
        
        DC(END); DC(END<<"START 32PACK 2 "<<END);
        // Копирование
        // Копирование однобитовой маски из массива imgOn
        // в массив On
        for(int y=0; y < mH; y++ ){
            MaskInt1[y]=MaskInt0[y];
        } // y
        
        setOnOffMatrixT(1,1);  ///////////////////////
        
        DC("PACK 32DONE 2"<<END); DC(END);
    }//_____________________________________________________________________________
    
    // Запаковка битовой маски из массива 1024 (серый или чб) в массив  32 int.Вход2
    // вычисление  mW, mH
    // -----------------------------------------------------------------------------
    
    
    void  GBitMask32::resizeMask(int stepW,int stepH,ofstream &c_out){
        // 1/4 resize
        //Из поевоначальных данных в mask[0].imgOn  формируется маски On Off с
        //изменением габаитов и масштабированием маски в соответствии со значениями stepW, stepH
        
        
        
        
    }//_____________________________________________________________________________
    
    
}

#endif


/*
 // маска периметра
 for ( y=1; y < mH-1; y++ ) {
 p=y*mWImg;
 for ( x=1; x < mWImg-1; x++ ) {
 ///					inputData[x+p]=0;
 } // x
 } // y
 
 
 for ( y=0; y < mH; y++ ) {
 p=y*mWImg;
 for ( x=0; x < mWImg; x++ ) {
 BufMask[x+p]=inputData[x+p];
 } // x
 } // y
 */


/*
 // маска периметра
 for ( y=1; y < mH-1; y++ ) {
 p=y*mWImg;
 for ( x=1; x < mWImg-1; x++ ) {
 BufMask[x+p]=0;
 } // x
 } // y
 */
