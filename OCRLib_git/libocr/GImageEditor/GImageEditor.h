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


#ifndef _GImageEditor_H_
#define _GImageEditor_H_

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "OCRTypes.h"
#include "GFont.h"
#include "pugixml.hpp"
#include "GBitmap.h"
#include "GBitset.h"
#include "GBitsetMatrix.h"
#include "GBitsetContour.h"
#include "GBitsetOCR.h"
#include "php2stl.h"

using namespace std;
using namespace pugi;

namespace ocr{
///Основной диспетчерский класс. Обеспечивает ввод и ввывод, логику создания и выполнения процессов обработки как изображения так и текста.
class  GImageEditor {
protected:
  GImageEditor(void);
  GImageEditor(int nrows, int ncolumns, int border=0);
  GImageEditor(const GImageEditor *ref);
  GImageEditor(const GImageEditor *ref, int border);
 public:
  virtual ~GImageEditor();
  void destroy(void);
  /** @name Initialization. */
  //@{
  void init();
protected:
public:
		GBitmap *pechaImg, *baseImg;              ///<global bitmap data 
		//GBitmap *pechaImgContour;
		//vectorOCR vectorBase[300];   //
		GFont* aliKali;                             ///<main letter OCR base
	    //GFontEditor* fontEditor;                 ///<Array of all GFont from letterStore
	    //vector<stringOCR>correctionTable; ///vector of all unsertan and hand-checked letters for draw in correction table
	    //GLogicProcessor *logicProcessor;
		//таблицы inputData;
		unsigned int vectorCount;
		GBitset *setBit;
		GBitsetContour *setContour;
		//GBitsetOCR *setOCR;
		int stringIndex, wordIndex, iLeft,iTop;
		int pechaDataLoaded;
		string mainString;
	    unsigned char hexCode[18991]; //byte to char string hex code
		short PechaLoaded;
		int print;
		//TmainOCRObject *newOCRObject;
		ofstream c_out;

		static GImageEditor* create(void) {return new GImageEditor;}

       ///На Маке запускает 8 потоков обработки изображений (proccessImage). На PC обрабатывает изображения в один поток. main multi praccessor image proccesing batch
		void imageProcess(); 
	    /// Выполнение функций обработки изображения. Авто поворот, чистка бордюров, деление на колонки, масштабирование.  image proccesing implementation
	    void proccessImage( GBitmap *pechaImg); 
        int forkImageProcess(pidID *pidIDArray,int maxFork,int ID);
        void adaptiveFiltration(GBitmap *pechaImg,float scale,int mode);
        void adaptiveFiltration1(GBitmap *pechaImg,float scale,int mode);
        void imageNormalisation(GLetter* letter);
        ///отладочная функция вывода графической информации процесса распознавания
		void drawString(int nStr,int border);
     	///draw image as JPEG data for HTTP and file operations
	    string drawToString(GBitmap *data); 
	    ///draw image as TIFF GroupFax4 data for HTTP and file operations
	    string drawToTiffString(GBitmap *data); 

 
	
		//void LoadImageFromPath(string &imgPath);
		/// Загрузка данных из графического файла. Проверка на пригодность данных.  Функция платформо зависима.
		GBitmap* LoadImageData(string &inputPath,bool invert);
        /// Загрузка данных из графического файла NSImage.  Функция платформо зависима
        GBitmap* LoadImageData(void* sourceImage);
	    /// Загрузка данных из TiffGroupFax4. Проверка на пригодность данных. Реализована на базе TiffLib.
	    GBitmap* loadFromTiffString(string &dataStr, bool invert);   
        ///Запись данных Bitmap в графический файл. Функция платформо зависима.
		void WriteImageData(GBitmap *data,string &path,bool flip, string &mode);
		///Запись данных Bitmap в графический файл. Функция платформо зависима. 
	    void WriteImageData(GBitmap *data,const char* path, bool invert);
   	    ///Запись данных Bitmap в графический файл. Функция платформо зависима.
	    void WriteImageData(GBitmap *data,string &path, bool invert);
        ///Запись данных Bitmap в графический файл NSIimage. Функция платформо зависима.
        void* WriteNSImage(GBitmap *data);
        ///Запись данных Bitmap в графический файл. Функция платформо зависима.
	    void WriteImageDataRGB(GBitmap *data,string &path, bool invert);
        void WriteImageDataRGB(GBitmap *data,const char *path,bool flip);
        ///Запись данных Bitmap в графический файл. Функция платформо зависима. 
	    void WriteImageData(GBitset *data,string &path, bool invert);
		///Запись данных Bitmap в графический файл. Функция платформо зависима.
	    void WriteImageData(GBitset *data,const char* path, bool invert);
		///Деление на колонки и запись в файл
		void WriteColumnsData(GBitmap *data, GBitset *set, int nFrm, string &filePath);
		///Деление на разворота на страницы и запись в файл
		void WriteSpreadData(GBitmap *data, GBitset *set, int nFrm, string &filePath);
		///Определение и чистка бордюров вокруг текста
		GBitmap* RemoveBorder(GBitmap *data, GBitset *set, int pageBorder);

	    void setHex(); //set conversion hex code
		
	
	    ///что-то из области фантастики. Читает и интерпретирует HTML файл результатов распознавания. 
	    void readPageHTML(vector<stringOCR>&strArray);
	           //Start detect scan page rotation. i - angle rotation limit
	           //void detectRotation(short i);

		//void LoadImage_mark();
		//void EncodeDjvuExe(AnsiString filePath);
		//void SavePDFAsDjVu();
		//void blur_aniso(short fileIndex);
		//void LoadDjvu();
		//void SaveDjvuExe(TStringList *fileList);
		//void EncodeDjvu(AnsiString filePath);
		//void collectDjvu();
		//void LoadDjvuExe();
};
};
// ---------------- IMPLEMENTATION
# endif

