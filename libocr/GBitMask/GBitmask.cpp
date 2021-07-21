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

#include "GBitMask.h"

namespace ocr {

//________GBitMask32_________________
GBitMask32::GBitMask32(){
status=0;             // статус маски
memset(imgOn,0,128);  // исходная область для получения масок
memset(On,0,128);     // центральная маска On
memset(Off,0,128);    // перефирийная маска Off
xMask=0; yMask=0;   // координаты крайне верхне-левого угла маски в исходном изображении
mW=32; mH=32; // габариты маски по ширине 32/64, габариты маски по высоте  1-128
NMask=0;      // площадь (количество единиц) центральной маски On
dlt0=8;
dlt1=1;
dlt2=3;
correlation=0;
maskType=1;
    // x0=0; y0=0; x1=0; y1=0;
}

void GBitMask32::set(){
    int size=imgW+mH;
    dlt0=size/4;
    if(size<30)dlt0=3;
    dlt1=1;
    if(size<20)dlt1=0;
    dlt2=(size)/20;
    maskType=1;
    if(size<20)maskType=0;
}
    
void GBitMask32::copy(GBitMask32* dest){
        
        
        
        
}
    
//________GBitMask64_________________
GBitMask128::GBitMask128(){
    status=0;              // статус маски
    //memset(imgOn,0,1024);   // исходная область для получения масок
    memset(On,0,1024);      // центральная маска On
    //memset(Off,0,1024);     // перефирийная маска Off
    xMask=0; yMask=0;   // координаты крайне верхне-левого угла маски в исходном изображении
    mW=128; mH=32; // габариты маски по ширине 32/64, габариты маски по высоте  1-128
    NMask=0;      // площадь (количество единиц) центральной маски On
    dlt0=8;
    dlt1=1;
    dlt2=2;
    maskType=1;
// x0=0; y0=0; x1=0; y1=0;
}

    

    
};
