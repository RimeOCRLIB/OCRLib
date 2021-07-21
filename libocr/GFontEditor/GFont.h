#ifndef HEADER_GFont
#define HEADER_GFont
//__OCR CLASSES_____________________________________
#include "../config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "../GBitMask/GBitMask.h"
#include "../GFontEditor/GLetter.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GMemory/GVector.h"
#include "../GMemory/GMap/GMap.h"
#include "../GMemory/MmapFile.h"
#include "../../pugixml/pugixml.hpp"
#include "../OCRString/php2stl.h"


#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;
using namespace pugi;

namespace ocr {
    

    ///main class for ctore all bata about letters in base
    class GFont: public GVector{
    private:
        GFont();
        GFont(const char* path);
    public:
        virtual ~GFont(); 
		void destroy(void);
        static GFont* create(void){return new GFont;}
        static GFont* create(const char* path){return new GFont(path);}
        void freeMemory();         //free allocated mmap pages and reinit
        //GLetter* letterSet[50000];
        
        int testLetter;
        int textLineSize;
        string fontName;
        string fileName;
        uint maxID;
        uint letterReadCount;
        map<string,int>fontStat;
        map<int,int>fontStatID;
        GStr<uchar> *cMatrix;                         ///<матрица взаимной корреляции всех букв базы
        ulong *OCRLanguage;                           ///<язык распознавания
        //vector<GLetter*>letterLine; 
       
        //int indexById(unsigned int Id);
         //вызов из массива осуществляется по ID номеру буквы.
        //GLetter*  operator[](uint ID);
        void reloadPtr();	 	 	 	  //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
        ulong size(void){return *recordCount-1;};
        //вызов из базы осуществляется по номеру буквы.
        GLetter*  getLetter(ulong ID);
        //вызов из базы осуществляется по номеру буквы.Если буква не входит в набор распознаваемых букв возвращается 0
        GLetter*  getOCRLetter(ulong index);
        void  saveLetter(GLetter* letter);  //сохраняет букву в базу по ее ID. Если ID нет в базе, сохраняет с новым ID
        //void  reloadLetter(uint ID);
        void  operator+=(GLetter *ref);  
        void readGFontXML();                        ///< чтение GFont из OCRData/OCRTables/fileName в память или импорт в базу данных
        void readGFontDB();                         ///< чтение GFont из базы данных
        void writeGFontXML();                       ///< запись GFont из OCRData/OCRTables/fileName
        void writeGFontDB();                        ///< запись GFont из памяти в базу данных
        void writeGFontStat();                      ///< записывает в базу статистику использования буквы
        void setLineFromArray(vector<stringOCR>&strArray);              //устанавливает координаты всех букв относительно строк текста
        GBitmap* drawPict(int in);
        GBitmap* drawLetterPict(int in);
        GBitmap* drawLetter(unsigned int in);
       
        void drawOCRBasePict(uint startRec,uint rowsNum, int mode);
        GBitmap *drawOCRBaseImage(uint startRec,uint rowsNum);
        GBitmap *drawOCRBasePict(vector<ulong>&searchResult,int mode);
        GVector *correlationVector;
        GMap *correlationMap;
        vector<ulong>letterSet;                       ///<массив индексов букв распознаваемого языка
        
        ulong letterCount(){return *recordCount-1;}
        void push_back(GLetter *letter);
        /**вывод в result HTML rowsNum записей начиная с startRec */
        void drawHTML(uint startRec,int rowsNum,string &result,int mode);
        void drawHTML(vector<ulong>&searchResult,string &result,int mode);
        void fontNormalisation();    //проверка ID номеров и создание IDTable
        void setStableFocalPoint();  //создает фокальные точки в каждой букве шрифта
        void setLetterStableFocalPoint(uint index);//создает фокальные точки в одной букве шрифта
        void setFocalLine();//создает фокальные линии в каждой букве шрифта
        void setFocalLineInLetter(uint index); //создает фокальные линии в одной букве шрифта
        void setLettersCorrelation(); //построение матрицы весов признаков для всех пар букв близких по кодовому расстоянию.
        void scaleLetter(GLetter *glyph,float scale);
    };


}; 


#endif
