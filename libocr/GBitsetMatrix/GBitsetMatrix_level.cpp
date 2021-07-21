#include "GBitsetMatrix.h"
#include "../OCRTypes/OCRTypes.h"


void GBitsetMatrix::init(GBitmap *ref, short stepW, short stepH, short flipV){
    border=128;
    srcW=ref->columns()+ border*2;
    srcH=ref->rows()+ border*2;
    if(stepH<1)stepH=1;
    if(stepW<1)stepW=1;
    nrows=ref->rows()/stepH;
    ncolumns=ref->columns()/stepW;
    init(nrows,ncolumns, border);
    bool *row=bits_data;
    bool *p,*p1;
    int dH,dW;
    int h=ref->rows();
    drawPoint=GGraphBezier(ncolumns,nrows);
    
#ifdef DEBUGLVL_DRAW
    drawDataRGB=GBitmap::create(nrows,ncolumns,cRGB);
#endif
    //line=GBitmap::create(w,h);
    //BitVec=new bitset<nrows*ncolumns>;
    if(flipV){
        
        for (int n=border; n<nrows-border; n++){
            p1=row+n*ncolumns;  dH=h-(n-border)*stepH;
            for(int m=border; m<ncolumns-border;m++){
                p=p1+m;   dW=(m-border)*stepW;
                if(ref->get(dW,dH)<125){*p=1;
                    drawPoint.put(m,n,0);       //DM(1);
                    
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xff000000);   //DM(drawDataRGB[0][n][m]);  // 0-черное, 255-белое
#endif
                }
                else{*p=0;
                    drawPoint.put(m,n,255);    //DM(0);
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xfff1f1f1); //DM(drawDataRGB[0][n][m]);  // 0-черное, 255-белое
#endif
                }
            }
            //DM(END);
        }
        
        /*
         //	    -- draw picture in text graphic ---
         bool *d1;
         for (int y=0; y < nrows; y+=4 ) {
         for(int x=0;x<ncolumns;x+=32){
         d1=bits_data+y*ncolumns+x;
         if(*d1){DM(1);}else{DM(0);
         }
         }DM(END);
         }DM(" NEW"<<END<<END<<END);
         */
        
    }else{
        for (int n=border; n<nrows-border; n++){
            p1=row+n*ncolumns; dH=(n-border)*stepH;
            for(int m=border; m<ncolumns-border;m++){
                p=p1+m;  dW=(m-border)*stepW;
                if(ref->get(dW,dH)<125){*p=1;
                    
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xff000000);
#endif
                }else{*p=0;
                    drawPoint.put(m,n,255);
                }
            }
            //_//_DM(END);
        }
        
    }
    
    // переупаковка сдвигом массива горизонтальных строк
    unsigned int nbits=nrows*ncolumns;
    unsigned int reg=0;           // обратный порядок пикселей
    for (unsigned int x=0; x < nbits; x++ ) { srcInt[x]=reg=(reg << 1)+ bits_data[x]; }
    //бордюр в 32 пиксела с краев каждого ряда содержит мусор
    //for ( x=0; x < ncVnr; x++ ) { SInt[x]=reg=(reg >> 1) + ((BolS[x])<<31); } // прямой порядок пикселей
    // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
    
    // переупаковка (транспонирование) всего  горизонтального
    // массива строк StrM (int) в массив вертикальные строк SInt (int)
    unsigned int index=0;
    for (int  x=0; x < ncolumns; x++ ) {
        for (int y=0; y < nbits; y+=ncolumns){
            sInt[index]=srcInt[x+y];
            //if(sInt[index]>0){drawDataRGB[0].int_data[x+y]=drawDataRGB[0].int_data[x+y]<<8;}
            index++;
        } // y
    } // x
    
    
}//__________________________________________

void GBitsetMatrix::init(GBitmap *ref,
                         short x,
                         short y,
                         short w,
                         short h,
                         short stepW,
                         short stepH,
                         short flipV){
    
    border=MATRIX_BORDER;
    if(stepH<1)stepH=1;
    if(stepW<1)stepW=1;
    //normalisation
    
    //cout<<"x="<<x<<" y="<<y<<" w="<<w<<" h="<<h<<" ref->columns()="<<ref->columns()<<" ref->rows()="<<ref->rows()<<" border="<<border<<endl;
    
    if(!w||!h)return;
    if(x<0)x=0; if(y<0)y=0;
    if(w>ref->columns())w=ref->columns();
    w=(w/64)*64;   //выравнивание по 64 (32) необходимо для работы графических алгоритмов
    if(x+w>ref->columns()){
        x=ref->columns()-w;
    }
    if(h>ref->rows())h=ref->rows();
    if(y+h>=ref->rows()){h=ref->rows()-y;}
    
    
    
    nrows=h/stepH;
    ncolumns=w/stepW;
    init(nrows,ncolumns,border);  //change nrows and ncolumns by border size
    bool *row=bits_data;
    bool *p,*p1;
    int dH,dW;
    int refH=ref->rows();
    drawPoint=GGraphBezier::create(ncolumns,nrows);
    uchar *bytes_dataP=drawPoint.bytes_data;
    
#ifdef DEBUGLVL_DRAW
    drawDataRGB=GBitmap::create(ncolumns,nrows,cRGB);
#endif
    if(flipV){
        
        for (int n=border; n<nrows-border; n++){
            p1=row+n*ncolumns;  dH=refH-y-(n-border)*stepH;
            for(int m=border; m<ncolumns-border;m++){
                p=p1+m;   dW=(m-border)*stepW+x;
                if(ref->get(dW,dH)<125){
                    *p=1;
                    bytes_dataP[m + n * ncolumns]=0;
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xff000000);   //DM(drawDataRGB[0][n][m]);  // 0-черное, 255-белое
#endif
                }
                else{
                    *p=0;
                    bytes_dataP[m + n * ncolumns]=255;
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xfff1f1f1); //DM(drawDataRGB[0][n][m]);  // 0-черное, 255-белое
#endif
                }
            }
            //DM(END);
        }
        
        
    }else{
        for (int n=border; n<nrows-border; n++){
            p1=row+n*ncolumns; dH=(n-border)*stepH+y;
            for(int m=border; m<ncolumns-border;m++){
                p=p1+m;  dW=(m-border)*stepW+x;
                if(ref->get(dW,dH)<125){
                    *p=1;
                    bytes_dataP[m + n * ncolumns]=0;
                    
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xff000000);
#endif
                }else{
                    *p=0;
                    bytes_dataP[m + n * ncolumns]=255;
#ifdef DEBUGLVL_DRAW
                    drawDataRGB->put(m,n,0xfff1f1f1);
#endif
                }
            }
            //_//_DM(END);
        }
        
        //set focal points
        drawPoint.focalLine(15);  // параметр функции - это максимально возможное разумное количество удаленных слоев,
        // если функция увидит что удаление слоев не меняет получаемую картинку (фокальная линия получена),
        // то она сама раньше закончит работу
        drawLine=GGraphBezier((GBitmap*)&drawPoint);
        drawLine.packImg32V(1);
        
        drawPoint.focalPointXTL(focalPoint);
        drawPoint.focalLineRecognition(focalPoint, focalLine);  //распознавание векторов для предварительного распознавания букв по диаграмме Воронова
        
        ulong size=ncolumns*nrows;
        uchar*p=drawPoint.bytes_data;
        
        
        for(int n=0;n<size;n++){
            if(!*(p+n)){*(p+n)=128;}  // точки скелета делаем серыми
        };
        uchar* t=0;
        //каждый бит в байте цвета обозначает наличие в этом пикселе соответсвуюшего вида фокальной точки
        //для вариабельности каждая фокальная точка занимает круговую область 32 пиксела вокруг начальных координат
        for(int n=0;n<focalPoint.size();n++){
            x=focalPoint[n].x;
            y=focalPoint[n].y;
            uchar color=focalPoint[n].type;
            //cout<<"color="<<(uint)color<<endl;
            
#define cP(x)  *(t x)=*(t x)+color
            t=p+y*ncolumns+x;
            cP(-5);cP( -4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4); cP(+5);
            t=p+(y-1)*ncolumns+x;
            cP(-5); cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4); cP(+5);
            t=p+(y-2)*ncolumns+x;
            cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4);
            t=p+(y-3)*ncolumns+x;
            cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4);
            t=p+(y-4)*ncolumns+x;
            cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3);
            t=p+(y-5)*ncolumns+x;
            cP(-1); cP(); cP(+1);
            
            t=p+(y+1)*ncolumns+x;
            cP(-5); cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4); cP(+5);
            t=p+(y+2)*ncolumns+x;
            cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4);
            t=p+(y+3)*ncolumns+x;
            cP(-4); cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3); cP(+4);
            t=p+(y+4)*ncolumns+x;
            cP(-3); cP(-2); cP(-1); cP(); cP(+1); cP(+2); cP(+3);
            t=p+(y+5)*ncolumns+x;
            cP(-1); cP(); cP(+1);
            
        }
        
        
    }
    
    
    // переупаковка сдвигом массива горизонтальных строк
    unsigned int nbits=nrows*ncolumns;
    unsigned int reg=0;           // обратный порядок пикселей
    for (unsigned int x=0; x < nbits; x++ ) { srcInt[x]=reg=(reg << 1)+ bits_data[x];}
    //бордюр в 32 пиксела с краев каждого ряда содержит мусор
    //for ( x=0; x < ncVnr; x++ ) { SInt[x]=reg=(reg >> 1) + ((BolS[x])<<31); } // прямой порядок пикселей
    // <<31 умножение на 2 в 31 степени, >> 1 деление на 2
    
    // переупаковка (транспонирование) всего  горизонтального
    // массива строк srcInt (int) в массив вертикальные строк sInt (int)
    unsigned int index=0;
    for (int  x=0; x < ncolumns; x++ ) {
        for (int y=0; y < nbits; y+=ncolumns){
            sInt[index]=srcInt[x+y];  //binaryPrint(sInt[index], 32); cout<<endl;
            //if(sInt[index]>0){drawDataRGB[0][y][x]=drawDataRGB[0][y][x]<<8;}
            index++;
        } // y
    } // x
    
}//__________________________________________

GBitmap* GBitsetMatrix::drawData32(){
    GBitmap *img=GBitmap::create(ncolumns,nrows,BITMAP_32);
    
    //cout<<"nrows="<<nrows<<" flag="<<img->pack_flag<<endl;
    
    for (int  x=0; x < ncolumns/32-1; x++ ) {
        memcpy(img->bites_data+x*nrows*4,sInt+32*nrows*(x+1),nrows*4);
        //binaryPrint(*(srcInt+x*nrows), 32); cout<<endl;
    }
    
    return img;
}
