#include "config.h"

#include "OCRTypes.h"
#include "VLetter.h"
#include <string.h>
#include <fstream>
#include "php2stl.h"
#include "GImageEditor.h"

//________VLetter_________________

void VLetter::init(){
        int a[30];
        memset((char*)a,0,120);
        dataVector.putPtr(0,(char*)a,120);
        GBitMask128 m;
        dataVector.putPtr(1,(char*)&m,sizeof(GBitMask128));
        dataVector.putPtr(2,(char*)&m,sizeof(GBitMask128));
        VStr<int> v;
        dataVector.putPtr(3,v.dataPtr(),*v.poolSize);
        dataVector.putPtr(4,v.dataPtr(),*v.poolSize);
        dataVector.putPtr(5,v.dataPtr(),*v.poolSize);
        dataVector.putPtr(6,v.dataPtr(),*v.poolSize);
        dataVector.putPtr(7,v.dataPtr(),*v.poolSize);
        dataVector.putPtr(8,v.dataPtr(),*v.poolSize);
    
        mask32.init(dataVector[3]);
        //focalPoint.init(dataVector[4]);
        //focalLine.init(dataVector[5]);
        openType.init(dataVector[6]);
        name.init(dataVector[7]);
        //OCRKey.init(dataVector[8]);
    
}