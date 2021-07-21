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


#ifndef _GImageEditorOCR_H_
#define _GImageEditorOCR_H_

#include "../config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "../OCRTypes/OCRTypes.h"
#include "../GBitmap/GBitmap.h"
#include "../GBitset/GBitset.h"

using namespace std;

namespace ocr{
///Функции создания и выполнения процессов обработки как изображения так и текста.
		/*
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
		//TmainOCRObject *newOCRObject;
		ofstream c_out;
		*/
		
	    /// Выполнение функций обработки изображения. Авто поворот, чистка бордюров, деление на колонки, масштабирование.  image proccesing implementation
	void proccessImageOCR(GBitmap *pechaImg, float *param);
	int forkImageProcess(pidID *pidIDArray,int maxFork,int ID);
	void adaptiveFiltration(GBitmap *pechaImg,float scale,int mode);
	void adaptiveFiltration1(GBitmap *pechaImg,float *param,int mode);
	void imageNormalisation(void* letter);
	///отладочная функция вывода графической информации процесса распознавания
	void drawString(int nStr,int border);
  
		
};
// ---------------- IMPLEMENTATION
# endif

