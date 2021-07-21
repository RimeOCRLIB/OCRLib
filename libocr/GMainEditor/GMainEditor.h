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

//if ( ImBuf !=NULL ) farfree(ImBuf);  //  освобождение памяти
//q=(тип_q *)farmalloc(n_byte);     // запрос памяти без очистки 0
//q_new=farrealloc(q_old,n_byte);;  // изменение размера блока


#ifndef _GMainEditor_H_
#define _GMainEditor_H_
#include "../config.h"

#include "../GFontEditor/GFontEditor.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GGraphOCR/GGraphOCR.h"
#include "../OCRString/php2stl.h"

#include <stdlib.h>
#include <string.h>

using namespace std;

namespace ocr{
///Основной диспетчерский класс. Обеспечивает ввод и ввывод, логику создания и выполнения процессов обработки как изображения так и текста.
    class  GMainEditor{
    protected:
         GMainEditor(void);
    public:
         void destroy(void);
  /** @name Initialization. */
  //@{
       void init();
    public:
        GBitmap *pechaImg, *baseImg;              ///<global bitmap data
        vectorOCR vectorBase[300];   //
        GFont* aliKali;                             ///<main letter OCR base
        vector<stringOCR>correctionTable; ///vector of all unsertan and hand-checked letters for draw in correction table
        GLogicProcessor *logicProcessor;
        GFontEditor *fontEditor;
        
        unsigned int vectorCount;
		GBitset *setBit;
		GBitsetContour *setContour;
        vector<stringOCR>strArray;
        GBitsetOCR *setOCR;

        int stringIndex, wordIndex, iLeft,iTop;
		int pechaDataLoaded;
		string mainString;
                unsigned char hexCode[18991]; //byte to char string hex code
		short PechaLoaded;
        float avarageScale;
		int print;
		//TmainOCRObject *newOCRObject;

        static GMainEditor* create(void) {return new GMainEditor;}
        ///диспетчер выполнения процесса определения масштаба страницы.
        float setScaleOCR(GBitmap *pechaImg_);
        ///диспетчер выполнения процесса определения масштаба страницы.
        int setImageOCR(GBitmap *pechaImg_,vector<float*>&param,OCRPointFloat &point);
        //destroy all OCR objects;
        void resetOCR();
        ///диспетчер выполнения процесса распознавания страницы.
        void startOCR(GBitmap *pechaImg_);
        ///диспетчер выполнения процесса распознавания одной страницы в режиме одна строка - один процесс.
        void startOneFileOCR();
        ///диспетчер выполнения процесса распознавания страницы в тренировочном режиме
        void startOCRTraining(GBitmap *pechaImg_);
        
        ///диспетчер выполнения процесса создания  OCR  шрифта. Распознаем текст как контура, заносим в базу и распознаем.
        void buildOCRFont();
        ///На Маке запускает восемь потоков постраничного распознавания. На PC выполняет постраничное распознавание
        void startOCRBatch();
        ///Пакетное распознавание на основе асинхронного вызова system(command &)
        void startOCRSystem();
        ///Распознавание на основе фокальных точек и векторов
        void startVectorOCR();
        ///читает и интерпретирует команды интерфейса записанные в управляющий файл input.xml
        string readInputAction();

        ///диспетчер выполнения процесса распознавания одной строки на страницы.
        void lineOCR();
	    
        void forkProccesOCR(int pidID,GBitmap *pechaImgID_); ///OCR implementation in fork() child process 
        bool forkProccesOCR_(pidID *pidIDArray,int ID, int maxFork); ///OCR implementation in fork() child process 
        
        //сегментация страницы, последовательное распознавание букв, слов, фраз и строк 
        void vectorCorrelation( vector<OCRPointFloat>&strPoint, vector<OCRFocalLineBezier>&lineVec);
        ///распознавание фрагмента текста из корректурной таблицы
        string letterBlockCorrelation(unsigned int in);


        ///отладочная функция вывода графической информации процесса распознавания
        void drawString(int nStr,int border);
        ///вывод результатов распознавания в HTML файл постранично.
        void drawOCRPage(vector<stringOCR>&strArray);
        ///вывод результатов распознавания в HTML файл постранично с возможностью заненсения букв в базу.
        void drawStrArray(vector<stringOCR>&strArray,int border);
        ///вывод результатов распознавания в HTML файл постранично с сохранением графических обьектов строк и букв. Используется при ручном составлении и коррекции базы букв.
        void drawStrArrayDebug(vector<stringOCR>&strArray, int border);
        //вывод в HTML масивы векторов и фокальных точек изображения
        void drawVector( vector<OCRPointFloat>&strPoint, vector<OCRFocalLineBezier>&lineVec);
    
        ///Collect all HTML and XML result of OCT in folder
        void exportOCR();

        void setHex(); //set conversion hex code
		

};
// ---------------- IMPLEMENTATION
}
# endif

