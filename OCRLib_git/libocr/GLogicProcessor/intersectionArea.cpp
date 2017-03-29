////C-
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

#include "GLogicProcessor.h"
#include "GImageEditor.h"
#include "php2stl.h"

 /** @brief функция определения площади буквы по количеству пикселов. Рисует букву A в letterAImg*/
void  GLogicProcessor::letterArea(OCRMatch &a, GBitmap *letterAImg){
    int print=0;
    DR("//*** letterArea x0="<<a.x0<<" x1="<<a.x1<<" y0="<<a.y0<<" y1="<<a.y1<<" area="<<a.area<<endl);
    a.drawPict32(letterAImg,0,0,XOR_MODE);
    if(a.s.x1>6300){
        cout<<"MEMORY ERROR1 s->x1:"<<a.s.x1;
        exit(0);
    }
    a.pCount=letterAImg->pixelCount(&a.s);
}


/** @brief функция определения количества пикселов фокальных линий изображения находящихся внутри изображения буквы. Рисует букву A в letterAImg*/
void  GLogicProcessor::letterLineArea(OCRMatch &a,GBitmap *lineImg32, GBitmap *letterAImg){
    int print=0;
    DR("//*** letterLineArea x0="<<a.x0<<" x1="<<a.x1<<" y0="<<a.y0<<" y1="<<a.y1<<" area="<<a.area<<" n="<<a.name<<endl);
    a.drawPict32(letterAImg,0,0,XOR_MODE);
    a.pCount=lineImg32->img32UnionLine(letterAImg,&a.s);

    DR("a.pCount="<<a.pCount);
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
}


/** @brief функция проверки площади пересечения букв по количеству общих точек */
int  GLogicProcessor::intersectionArea(OCRBox *s, GBitmap *lineImg32,GBitmap *letterAImg,GBitmap *letterBImg){
    int print=0;
    int sArea=0;
    DR("//*** intersectionArea x0="<<s->x0<<" x1="<<s->x1<<" y0="<<s->y0<<" y1="<<s->y1<<" area="<<s->area<<endl);
    //if(inputData.start==111){
    //    ((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
    //}
    sArea=letterAImg->img32Union(letterBImg,s);        //вычисляем площадь пересечения
    
    return sArea;
}

/** @brief функция проверки площади пересечения букв по количеству общих точек */
int  GLogicProcessor::stackIntersectionArea(OCRMatch &a, OCRMatch &b,OCRBox *s, GBitmap *lineImg32,GBitmap *letterAImg,GBitmap *letterBImg){
    int print=0;
    int sArea=0;
    DR("//*** intersectionArea x0="<<s->x0<<" x1="<<s->x1<<" y0="<<s->y0<<" y1="<<s->y1<<" area="<<s->area<<endl);
    
    //a.drawPict32(letterAImg,MATRIX_BORDER, XOR_MODE);
    b.drawPict32(letterBImg, 0,0, XOR_MODE);  //рисуем только вторую букву, первая должна быть нарисована
    
    //if(inputData.start==111){
    //    ((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
    //}
    
    sArea=letterAImg->img32Union(letterBImg,s);        //вычисляем площадь пересечения
    
    letterBImg->fillColumns32V(0, &b.s);
    
    return sArea;
}





