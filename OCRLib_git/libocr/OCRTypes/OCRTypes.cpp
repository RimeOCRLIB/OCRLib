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

#ifndef OCRTYPES_H
#define OCRTYPES_H

#include "config.h"

#include "OCRTypes.h"
#include <string.h>
#include <fstream>
#include "php2stl.h"


namespace ocr {

    int operator==(const dictKey &left, const dictKey &right){
        return  left.lenght==right.lenght;
    }
    
    int operator<(const dictKey &left, const dictKey &right){
        return  left.lenght>right.lenght;
    }
    
    //________byteMask________________
    byteMask::byteMask(){
        b1=b2=b3=b4=b5=b6=b7=b8=0;
    }
    
    void byteMask::setMask(char mask){
        
        // разборка байтового регистра на отдельные 8 бит //
        b1=mask & 1;    b2=mask>>1 & 1; b3=mask>>2 & 1; b4=mask>>3 & 1;
        b5=mask>>4 & 1; b6=mask>>5 & 1; b7=mask>>6 & 1; b8=mask>>7 & 1;

        
    }
    
    
    //________OCRPoint_________________
    OCRPoint::OCRPoint(){
        x=0;
        y=0;
        status=0;
        type=0;
        stability=0;
    }
    OCRPoint::OCRPoint(float x_,float y_){
        x=x_;
        y=y_;
        status=0;
        type=0;
        stability=0;
    }
    OCRPoint OCRPoint::operator*(int n){
        OCRPoint t;
        t.status=status;
        t.type=type;
        t.x=x*n;
        t.y=y*n;
        return t;
    }
    
    //________OCRBox_________________
    OCRBox::OCRBox(){
        x0=0;
        y0=0;
        x1=0;
        y1=0;
        area=0;
        status=0;
    }
    void OCRBox::setBorder(OCRBox *a,int border){
        x0=a->x0+border;
        y0=a->y0+border;
        x1=a->y1+border;
        y1=a->y1+border;
        area=a->area;
    }
    void OCRBox::printBox(){
        cout<<"//*** OCRBox x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<" area="<<area<<endl;
    }
  
//_______ OCRDictRecord
//OCRDictRecord::~OCRDictRecord(void){}

//void OCRDictRecord::destroy(void){}

OCRDictRecord::OCRDictRecord(){
	wordCount=0;
}
    
    int operator==(const OCRDictRecord &left, const OCRDictRecord &right){
        return  left.level==right.level;
    }
    
    int operator<(const OCRDictRecord &left, const OCRDictRecord &right){
        
        int levelLeft=0,levelRight=0; 
        
        if(left.level>right.level)return 1;
        if(left.level==right.level){
            if(left.dict=="ML")levelLeft+=10;
            if(right.dict=="ML")levelRight+=10;
            if(left.dict=="SH")levelLeft+=10;
            if(right.dict=="SH")levelRight+=10;
            if(left.dict=="ER")levelLeft+=10;
            if(right.dict=="ER")levelRight+=10;
            if(left.dict=="YO")levelLeft=0;
            if(right.dict=="YO")levelRight=0;
            if(left.dict=="IW")levelLeft+=8;
            if(right.dict=="IW")levelRight+=8;
            if(left.dict=="RHD")levelLeft+=9;
            if(right.dict=="RHD")levelRight+=9;
            if(left.dict=="JV")levelLeft+=6;
            if(right.dict=="JV")levelRight+=6;
            if(left.dict=="RB")levelLeft+=5;
            if(right.dict=="RB")levelRight+=5;
            if(left.dict=="TD")levelLeft+=4;
            if(right.dict=="TD")levelRight+=4;
            
            
        
        }
        
        return  levelLeft>levelRight;
    }
    
    
/*    
    void таблицы::copy(таблицы* src){
        fileList=src->fileList;
        folderList=src->folderList;		
        data=src->data;
        fileSet=src->fileSet;
        lineSize=src->lineSize;
        CoreProcessing=src->CoreProcessing;
        imgW=src->imgW;
        imgH=src->imgH;;
        start=src->start;
        idNumber=src->idNumber;
        x0=src->x0;
        y0=src->y0;
        x1=src->x1;
        y1=src->y1;
        socketFD=src->socketFD;  //socket file descriptor
        mainEditor=src->mainEditor;
        imageEditor=src->imageEditor;
        logicProcessor=src->logicProcessor;
        fontEditor=src->fontEditor;
        Core=src->Core;

    }
*/


}


#endif



