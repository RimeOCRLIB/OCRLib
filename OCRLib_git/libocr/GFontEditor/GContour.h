#ifndef HEADER_GContour
#define HEADER_GContour
#include "GBitmap.h"

namespace ocr {
    
class GContour {
protected:
    GContour(void);    
    GContour(int length_,int dir);
    GContour(int *angX,int *angY,int *andData,int length_,int dir);
    
    int length;
    int w;
    int h;

public:
    virtual ~GContour();
    void destroy(void);
    static GContour* create(void){return new GContour;}
    static GContour* create(int length_,int dir){return new GContour(length_,dir);}
    static GContour* create(int *angX,int *angY,int *andData, int length_,int dir){return new GContour(angX, angY, andData, length_, dir);}
    void init(int length_,int dir);
    void init(int *angX,int *angY,int *andData, int length_,int dir);
    int size(){return length;}
    int width(){return w;}
    int height(){return h;}
    int *lineX,*lineY,*lineD;
    int dX,dY;  //координаты левого верхнего угла контура на странице
    GBitmap* img;
    int direction;
    int border;
        
};  
    
}    

#endif

