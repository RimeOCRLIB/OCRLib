//
//  AI.h
//  OCRLib
//
//  Created by dharmabook on 01/06/15.
//
//
#ifndef OCRLib_AI_h
#define OCRLib_AI_h

#include "GVector.h"
#include "GMap.h"

struct associativeDatabase
{
    
    GVector *mainMemoryData;
    GMap *mainMemory;
    
    GMap *memory;
    GVector *memoryData;
    
    GMap *longMemory;
    GVector *longMemoryData;
    
    string path;
    
    
};

extern associativeDatabase ai;



#endif
