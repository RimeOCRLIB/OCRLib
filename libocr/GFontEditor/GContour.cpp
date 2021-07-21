#include "GContour.h"

namespace ocr {


//________GContour_________________
GContour::GContour(){
	direction=-1;
}
GContour::GContour(int length_,int dir){
    
};

GContour::GContour(int *angX,int *angY,int *andData,int length_,int dir){
    try
    {
        init(angX,angY,andData,length_,dir);
    }
    catch(int a)
    {
        destroy();
    }
}

GContour::~GContour(){}

void GContour::destroy(void){
    free(lineX);
    free(lineY);
}

void GContour::init(int length_,int dir){
    length=length_;
    direction=dir;
    lineX=(int*)malloc(length*4);
    lineY=(int*)malloc(length*4);
    lineD=(int*)malloc(length*4);
    direction=dir;
}

void GContour::init(int *angX,int *angY,int *andData,int length_,int dir){
    length=length_;
    direction=dir;
    border=32;
    if(angX==NULL||angY==NULL)return;
    init(length_,dir);
    int minX=100000,minY=100000;
    int maxX=0,maxY=0;
    
    for(int a=0;a<length;a++){
        if(angX[a]<minX)minX=angX[a];
        if(angY[a]<minY)minY=angY[a];
        if(angX[a]>maxX)maxX=angX[a];
        if(angY[a]>maxY)maxY=angY[a];
    }
    for(int a=0;a<length;a++){
        lineX[a]=angX[a]-minX;
        lineY[a]=angY[a]-minY;
        lineD[a]=andData[a];
    }
    w=maxX-minX+border*2+1;
    h=maxY-minY+border*2+1;
    dX=minX;
    dY=minY;
    img=GBitmap::create(w,h);
    
}

}

