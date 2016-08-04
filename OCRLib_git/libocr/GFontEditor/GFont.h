#ifndef HEADER_GFont
#define HEADER_GFont
//__OCR CLASSES_____________________________________
#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "GBitMask.h"
#include "GLetter.h"
#include "OCRTypes.h"
#include "GVector.h"
#include "GMap.h"
#include "MmapFile.h"
#include "pugixml.hpp"
#include "php2stl.h"


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
        //MemoryFile *fontData_mf;  //main file for store font letters data
        //int readByMmap;
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
        GStr<uchar>* cMatrix;                       ///матрица взаимной корреляции всех букв базы
        uint *OCRLanguage;                           ///язык распознавания
        //vector<GLetter*>letterLine; 
       
        //int indexById(unsigned int Id);
         //вызов из массива осуществляется по ID номеру буквы.
        //GLetter*  operator[](uint ID);
        void reloadPtr();	 	 	 	  //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
        uint size(void){return *recordCount-1;};
        //вызов из базы осуществляется по ID номеру буквы.
        GLetter*  getLetter(uint ID);
        //вызов из базы осуществляется по ID номеру буквы.Если буква не входит в набор распознаваемых букв возвращается 0
        GLetter*  getOCRLetter(uint index);
        void  saveLetter(GLetter* letter);  //сохраняет букву в базу по ее ID. Если ID нет в базе, сохраняет с новым ID
        //void  reloadLetter(uint ID);
        void  operator+=(GLetter *ref);  
        void readGFontXML();                        ///чтение GFont из OCRData/OCRTables/fileName в память или импорт в базу данных
        void readGFontDB();                         ///чтение GFont из базы данных
        void writeGFontXML();                       ///запись GFont из OCRData/OCRTables/fileName    
        void writeGFontDB();                        ///запись GFont из памяти в базу данных 
        void writeGFontStat();                      ///записывает в базу статистику использования буквы     
        void setLineFromArray(vector<stringOCR>&strArray);              //устанавливает координаты всех букв относительно строк текста
        GBitmap* drawPict(int in);
        void drawOCRBasePict(uint startRec,uint rowsNum, int mode);
        GBitmap *drawOCRBaseImage(uint startRec,uint rowsNum);
        GBitmap *drawOCRBasePict(vector<uint>&searchResult,int mode);
        GVector *correlationVector;
        GMap *correlationMap;
        
        uint letterCount(){return *recordCount-2;}
        void push_back(GLetter *letter);
        /**вывод в result HTML rowsNum записей начиная с startRec */
        void drawHTML(uint startRec,int rowsNum,string &result,int mode);
        void drawHTML(vector<uint>&searchResult,string &result,int mode);
        void fontNormalisation();    //проверка ID номеров и создание IDTable
        void setStableFocalPoint();  //создает фокальные точки в каждой букве шрифта
        void setLetterStableFocalPoint(int index);//создает фокальные точки в одной букве шрифта
        void setFocalLine();//создает фокальные линии в каждой букве шрифта
        void setFocalLineInLetter(int index); //создает фокальные линии в одной букве шрифта
        void setLettersCorrelation(); //построение матрицы весов признаков для всех пар букв близких по кодовому расстоянию.
        
    };


}; 


#endif
