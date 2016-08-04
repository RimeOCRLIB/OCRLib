#ifndef HEADER_VLetter
#define HEADER_VLetter
//__OCR CLASSES_____________________________________
#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "GBitmap.h"
#include "GBitMask.h"
#include "GContour.h"
#include "GLetter.h"
#include "OCRTypes.h"
#include "GVector.h"
#include "GGraph.h"

using namespace std;

namespace ocr {
    
    class VLetter:public Vector{
    public:
        VLetter(Vector &v):
            letterIndex(((int *)v[0])[0]),
            recordFlag(((int *)v[0])[1]),
            recordStatus(((int *)v[0])[2]),
            onOffRate(((int *)v[0])[3]),
            dX(((int *)v[0])[4]),
            dY(((int *)v[0])[5]),
            OCRIndex(((int *)v[0])[6]),
            correlation(((int *)v[0])[7]),
            selfCorrelation(((int *)v[0])[8]),
            codeSpace(((int *)v[0])[9]),
            Character(((int *)v[0])[10]),
            vectorLimit(((int *)v[0])[11]),
            stackFlag(((int *)v[0])[12]),
            OCRStatus(((int *)v[0])[13]),
            letterW(((int *)v[0])[14]),
            letterH(((int *)v[0])[15]),
            xMin(((int *)v[0])[16]),
            xMax(((int *)v[0])[17]),
            yMax(((int *)v[0])[18]),
            yMin(((int *)v[0])[19]),
            xSum(((int *)v[0])[20]),
            ySum(((int *)v[0])[21]),
            y0(((int *)v[0])[22]),
            y1(((int *)v[0])[23]),
            textLineSize(((int *)v[0])[24]),
            mask128(*((GBitMask128*)v[1])),
            maskOriginal(*((GBitMask128*)v[2])),
            mask32(*((VStr<GBitMask32>*)v[3])),
            focalPoint(*((vector<OCRPoint>*)v[4])),
            focalLine(*((vector<OCRFocalLine>*)v[5])),
            openType(*((VStr<OCRBox>*)v[6])),
            name(*((VStr<char>*)v[7])),
            OCRKey(*((VStr<char>*)v[8])),
            dataVector(v)
            {
                
        }
        void init();
        int test(){return (int)focalPoint.size();};
        
        Vector &dataVector;
        GBitMask128 &mask128;            ///габаритная маска(изображение) буквы
        GBitMask128 &maskOriginal;       ///габаритная маска оригинального исходного изображения буквы
        VStr<GBitMask32>&mask32;         ///контейнер для масок буквы
        vector<OCRPoint>&focalPoint;       ///массив топологических ключевых точек буквы
        vector<OCRFocalLine>&focalLine;    ///массив линий соединяющих топологические ключевые точки буквы
        VStr<OCRBox>&openType;           ///массив габаритов всех составляющих букву элементов OpenType
        VStr<char>&name;
        VStr<char>&OCRKey;
        int& letterIndex;               ///letter index in base
        int& recordFlag;
        int& recordStatus;              ///флаг принадлежности к языку распознавания
        int& onOffRate;                 ///соотношение площади буквы покрытой и не покрытой признаками
        int& dX,dY;                     ///<смещение координат центра буквы относительно изображения буквы  (mask128)
        int& OCRIndex;
        int& correlation;               ///<value of correlation
        int& selfCorrelation;           ///<test value of self correlation
        int& codeSpace;                 ///<test value of test value of code space between this letter and all letters in letter base
        int& Character;
        int& vectorLimit;
        int& stackFlag;
        int& OCRStatus;
        int& letterW;                   ///габаритная ширина буквы в строке без учета огласовок и подписных букв
        int& letterH;                   ///высота буквы
        int& xMin;
        int& xMax;
        int& yMax;
        int& yMin;
        int& xSum;
        int& ySum;
        int& y0;                        //верхний габарит строки относительно центра изображения буквы  (mask128)
        int& y1;                        //нижний габарит строки относительно центра изображения буквы (mask128)
        int& textLineSize;              //size of text line in text in pixels
        
        
        
    };
    
};
#endif
