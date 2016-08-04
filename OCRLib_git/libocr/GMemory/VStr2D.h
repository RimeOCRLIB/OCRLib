#ifndef VSTR2D_H
#define VSTR2D_H
#include "config.h"
//#include "MmapFile.h"
#include "Vector.h"
#include <stddef.h> // for size_t
#include <stdlib.h>

using namespace std;
namespace ocr {
    
    ////////////////////////////////////
    template<typename T>
    class VStr2D :public VStr<T>{
    public:
        T* dataT;
        uint* index;
        void setIndex(uint*indexV,uint size);
        uint size(uint i){return index[*(VStr<T>::recordCount)+i];};
        T* operator [](uint index_);
    };
    
    
    template<typename T>
    void VStr2D<T>::setIndex(uint*indexV,uint size){
        uint sizeData=0;
        for(int i=0;i<size;i++){
            sizeData+=indexV[i];
        }
        VStr<T>::resize(sizeData+size*2/(sizeof(T)/4));
        index=(uint*)(VStr<T>::data+24);
        dataT=(T*)(VStr<T>::data+24+(*VStr<T>::recordCount)*4);
        
        *VStr<T>::recordCount=size;
        uint dataOffset=0;
        dataOffset=0;
        for(int i=0;i<size;i++){
            index[i]=dataOffset;
            dataT[dataOffset]=0; //записываем длинну строки
            dataOffset+=indexV[i]+1;
        }
        
    }

    template<typename T>
        T* VStr2D<T>::operator [](uint index_){
            return (T*)(dataT+index[index_]);
    }

    
}

#endif