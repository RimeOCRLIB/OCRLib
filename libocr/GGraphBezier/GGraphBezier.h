//
//  GraphOCR.h
//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .



#ifndef __OCRLib__GraphOCR__
#define __OCRLib__GraphOCR__

#include <stdio.h>
#include "../config.h"
#include "../OCRTypes/OCRTypes.h"
#include "../OCRAlgorithm/OCRAlgorithm.h"
//#include "GStr.h"
//#include "GStr2D.h"
#include "../GBitmap/GBitmap.h"
#include <stdlib.h>
#include <math.h>
#include <iostream> 

// Все что короче LINE_SIZE=12 является прямыми линиями, а все что длинее более сложными кривыми.
#define LINE_SIZE 6 // Ограничение на длину фокальной линии.12
//if (LINE_SIZE<4) LINE_SIZE=4;


using namespace std;


namespace ocr{
    
    class OCRFocalLineBezier;   //forward declaration
    
    //reference for OCRFocalLineBezier class
    class OCRLineRef{
    public:
        OCRLineRef();
        OCRLineRef(OCRFocalLineBezier &ref,uint len_);
        int index;
        int alpha;
        int len;    //относительная длина в процентах к длинне базовой линии.
    };

    class OCRFocalLineBezier{
    public:
        OCRFocalLineBezier();          // тип Point это float.
        vector<OCRPointFloat>data;    // вектор для массивов координат точек фокальной линии по "x" и "y".
        vector<OCRPointFloat>mTab;    // вектор для таблицы интегральных сумм сглаженных координат фокальной линии.
        // массивы ниже помеченные как (NO Б.Д.) нужны только для обработки текущей линии и их можно не сохранять в Б.Д.
        vector<OCRPointFloat>mData;   // - вектор для массива усредненных координат точек фокальной линии по "x" и "y"(NO Б.Д.).
        vector<OCRPointFloat>sData;   // - вектор для массива координат интегральных сумм фокальной линии по "x" и "y" (NO Б.Д.).
        //vector<float>sLine;    // - вектор для массива интегральных сумм расстояний между соседними точками сглаженной фок.линии (NO Б.Д.).
        vector<OCRPointFloat>test;    // - вектор для вывода на экран нескольких тестовых точек линии (NO Б.Д.).
        
        
        //GStr<OCRLineRef> startL;  // массив данных линий соединенных со стартовой точкой базовой линии.
        //GStr<OCRLineRef> endL;    // массив данных линий соединенных со конечной точкой базовой линии.
        //virtual ~OCRFocalLineBezier();
        
        OCRPointFloat start;     // координаты начала линии.
        //OCRPointFloat start_с;   // координаты начала линии приведенные к ее середине.
        OCRPointFloat end;       // координаты окончания линии.
        //OCRPointFloat end_с;     // координаты окончания линии приведенные к ее середине.
        OCRPointFloat center;    // координаты середины прямой соединяющей концы линии.
        OCRPointFloat centerM;   // координаты центра масс.(совпадает с центром окружности).
        OCRPointFloat p0;        // координаты начальной точки кривой Безье апроксимирующей фокальную линию (могут не совпадать с точкой start).
        OCRPointFloat p1;        // координаты первой контрольной точки кривой Безье апроксимирующей фокальную линию.
        OCRPointFloat p2;        // координаты второй контрольной точки кривой Безье апроксимирующей фокальную линию.
        OCRPointFloat p3;        // координаты конечной точки кривой Безье апроксимирующей фокальную линию (могут не совпадать с точкой end).
        OCRPointFloat p0_c;      // координаты начальной точки кривой Безье приведенные к её середине.
        OCRPointFloat p3_c;      // координаты конечной точки кривой Безье приведенные к ее середине.

        int index;          // id number
        int status;         // статус линии
        
        // характеристики line //
        int maxCor;         // прямая-1, дуга-2, спираль-3, круг-4.
        int maxC;           // - значение максимальной корреляции.
        int corL;           // - коэффициент корреляции линии с прямой (0-100%).
        int corA;           // - коэффициент корреляции линии с дугой (0-100%).
        int corS;           // - коэффициент корреляции линии со спиралью (0-100%).
        float dM;           // - расстояние от центра масс до середины прямой соединяющей концы линии.
        float len;          // длина прямой соединяющей концы линии.
        float alpha;        // угол наклона линии в градусах (0 до 360° grade).
        int   alpInt;       // целочисленное значение угола наклона линии в градусах (0 до 360° grade).
        int angl_t;         // - среднее значение тангенциального угла.
        // характеристики circle //
        int corC;           // - коэффициент корреляции линии с кругом (0-100%).
        int radius;         // - радиус круга в pix.
        // характеристики кривой Безье //
        int lenA;           // количество пикселов линии.
        float lenM;         // длина сглаженной фокальной линии.
        float lenP;         // длина укороченной кривой Безье (как часть длины сглаженной фокальной линии).
        float lenB;         // длина кривой Безье (вычислено по уравнению растеризации, медленно).
        float lenBf;        // - длина кривой Безье (вычислено рекурсиво и бысто).
        float lenBff;       // - длина кривой Безье, приближенное значение (очень бысто, вычислено всего по 4 векторам).
        float alphaP1;      // угол наклона первого контрольного вектора (0 до 360° grade).
        //int   alpIntP1;     // целочисленное значение угола наклона первого контрольного вектора (0 до 360° grade).
        float lenP1;        // длина первого контрольного вектора в pix.
        float alphaP2;      // угол наклона второго контрольного вектора (0 до 360° grade).
        //int   alpIntP2;     // целочисленное значение угола наклона второго контрольного вектора (0 до 360° grade).
        float lenP2;        // длина второго контрольного вектора в pix.
        float alphaP1P2;    // угол наклона вектора от первой управляющей точки до второй управляющей точки.
        //int   alpIntP1P2;   // целочисленное значение угола наклона вектора P1P2 (0 до 360° grade).
        float lenP1P2;      // расстояние вектора от первой управляющей точки до второй управляющей точки в pix.

        
        void writeToStr(TString &st);
        void readFromStr(TString &st);
        void setDimension(int dx,int dy); //Пересчет координат линии по отношению к центру буквы.
        int size(){return (uint)data.size();}
        void printToScreen();
        
    };

    class  GGraphBezier : public GBitmap{
    protected:
    public:
        
        ///__________________function declaration__________________//
        GGraphBezier(void);
        GGraphBezier(int ncolumns,int nrows);
        GGraphBezier(int ncolumns,int nrows, int mode);
        GGraphBezier(const GBitmap *ref);
        GGraphBezier(const GBitmap* ref, int x0,int y0, int w, int h);
        GGraphBezier(const GBitmap *ref, float scale, float rotation,int mode);
        GGraphBezier(string &inputData,int invert);
     
        int register1;  //  тестовая переменная для подсчета чего-нибудь. // count
       
        /** @name Initialization. */
        //Функции распознавания графа
 
        // функция стандартизации начертания текста путем определения фокальной линии (графемы) букв.
        // Просчет матрицы происходит в теле цикла.
        /// Функция выделения стабильных фокальных точек на фокальных линиях (графемах) букв.
        int  StablePoint(vector<OCRPointFloat>&focalPoint);
        // TMP
        // Версия с более подробным вычислением статуса фокальных точек.
        int  StablePointT(vector<OCRPointFloat>&focalPoint);

        /// Функция выделения фокальных точек на фокальных линиях (графемах) букв.
        // Ставит точки не только в "Т" стыках, но и в "Х" стыках (выбирается в самой функции).
        int graphFocalPoint(vector<OCRPointFloat>&focalPoint);
        /// Функция выделения фокальных точек на фокальных линиях (графемах) букв.
        // Ставит точки в "Т" стыках, в "Х" стыках и концах линий "L_End".
        // Определяется тип точки и записывается в ее статус.
        int  focalPointXTL(vector<OCRPointFloat>&focalPoint);
        /// Версия с предпросчетом адресов начала и конца черных слайсов (для перепрыгивания белых областей).
        // По факту не более быстрая версия, поэтому TMP.
        int focalPointT1();

        /// Функция распознования фокальных линий (графемы) букв.
        // Функция вычисляет координаты точек, из которых состоят фокальные линии букв.
        // Используется после применения функцией определения фокальной линии(графемы) букв и функции выделения фокальных точек.
        // сответственно GBitmapFocalLine.cpp  и  GBitmapfocalPointXTL.cpp .
        // Не забыть очистиь от черного 2х pix бордюр вызовом функции img->eraseBorder(2, 2).
        int focalLineRecognition(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine);
        // Функция для распознования углов и подсчета его величины alpha.
        void focalLineRecAngle(vector<OCRPointFloat>&data, GStr<int>*newPoints, uint deltaAngle);
        // Функция для деления ломанной фокальной линии на составляющие линии.
        void focalLineSplit(GStr<int>*newPoints, vector<OCRFocalLineBezier> &focalLine,vector<OCRPointFloat>&focalPoint, OCRFocalLineBezier &line);
        // Функция для нормализации данных фокальной линии.
        void focalLineNorm(OCRFocalLineBezier &line);
        // Функция для сглаживания фокальной линии. Новая версия. В разработке.
        void focalLineMiddleA(OCRFocalLineBezier &line, uint K_Mid2);
        // Функция для сглаживания фокальной линии. Предыдущая версия.
        void focalLineMiddleB(OCRFocalLineBezier &line, uint K_Mid2);
        // Функция для сглаживания (интерполяции) несколько крайних pix фокальной линии. В разработке.
        void focalLineEnds(OCRFocalLineBezier &line, int strL, int endL);
        // Функция с возможностью вычисления параметров кривой Безье по части фокальной линии.
        void focalLineBezierP(OCRFocalLineBezier &line, int str_L, int end_L);
        // Функция для вычисления параметров кривой Безье только по полной длине фокальной линии.
        void focalLineBezierF(OCRFocalLineBezier &line);
        // Функция для сравнения (корреляции) двух кривых Безье. Используется таблица для ранней отсечки.
        float CorrelationBezierT(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2);
        // Функция для сравнения (корреляции) двух кривых Безье.
        float CorrelationBezierA(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2, int pCor);
        // Функция для сравнения (корреляции) двух кривых Безье. С параметрами.
        float CorrelationBezierB(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2, int mirror, int ln, int p1, int p1p2, int p2);
        // Функция для сравнения (корреляции) одной кривой Безье с частью другой кривой Безье.
        float CorrelationBezierPart(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2, OCRFocalLineBezier &line3);
        // Функция для распознования фокальных линиий с помощью центра масс: прямой, дуги, круга, и спирали.
        void focalLineRecMass(OCRFocalLineBezier &line);
        // Функция для подсчета характеристик фокальной линии: коэффициента корреляции линии с кругом и
        // диаметра круга.
        void focalLineRecCircle2 (OCRFocalLineBezier &line);
        
        inline int lineCorrelation(OCRFocalLineBezier &textLine,OCRFocalLineBezier &baseLine,OCRPointFloat &center,uint &indexLine);
        
        // Функция построения таблиц распознавания фокальных линий.
        // Предназначена для построения таблиц, необходимых для функции распознования фокальных линий focalLineRecognition.
        void buildOCRLineTable();
        // Предназначена для построения таблиц, необходимых для функции сравнения двух кривых Безье CorrelationBezierA, (B).
        void buildOCRAlphaTable();  // использается для вычисления коэффициента корреляции уголов векторов.

        
        // Функция построения матрицы поиска фокальных линий по декартовым координатам концов.
        void setLineMatrix(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine, GStr2D<int>*pointMatrix, GStr2D<int>*lineMatrix,uint dltX);
        
        // Функция анализирует все фокальные линии на возможность создания результирующей линии из двух сопряженных линий.
        void setLineCombination(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine,GStr2D<int>*pointMatrix,GStr2D<int>*lineMatrix);
        // Функция анализирует все фокальные линии на возможность создания результирующей линии из двух сопряженных линий.
        // Промежуточная версия.
        void setLineCombination2(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine,GStr2D<int>*pointMatrix,GStr2D<int>*lineMatrix);
        
        // Функция вычисления длинны линии и угла между линией и горизонталью.
        void LineAngleLength(float delta_x, float delta_y, float &alpha, float &length );
        // Функция для подсчета реальной длины сглаженной фокальной линии.
        void focalLineLengthM(OCRFocalLineBezier &line);
        // Функция для подсчета реальных углов и длины сглаженной фокальной линии.
        void focalLineAngleM(OCRFocalLineBezier &line);
        // Функция для подсчета таблицы определения координат по длине сглаженной фокальной линии.
        void focalLineTabM(OCRFocalLineBezier &line);
        // Функция предназначена для растеризации и вычисления длины кубической кривой Безье (прямое вычисление по уравнению).
        void BezierLength(OCRFocalLineBezier &line);
        // Функция вызова подпрограммы быстрой растеризации кривой Безье (recursiveBezier).
        void BezierRasterisation(OCRFocalLineBezier &line,vector<OCRPointFloat>&lineBezierPoints);
        // Рекурсивная функция для быстрой растеризации и вычисления длины кубической кривой Безье.
        void recursiveBezier(vector<OCRPointFloat>&lineBezierPoints,
                            float x1, float y1,
                            float x2, float y2,
                            float x3, float y3,
                            float x4, float y4,
                            float &LengthLineBf );
        // Функция для очень быстрого приближенного вычисления длины кубической кривой Безье.
        void BezierLengthApproximate(OCRFocalLineBezier &line);
        // Функция для тестирования алгоритмов обработки кривых Безье.
        void Test(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2);
        
        void drawSVG(vector<OCRPointFloat>&focalPoint, vector<OCRFocalLineBezier> &focalLine);
        
};

    //static bool sortLineXCenter(const OCRFocalLineBezier& d1, const OCRFocalLineBezier& d2){
    //    return d1.center.x < d2.center.x;
    //}
    
    
}


#endif /* defined(__OCRLib__GraphOCR__) */
