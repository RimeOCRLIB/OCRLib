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

#include "../config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "../OCRTypes/OCRTypes.h"
#include "../GFontEditor/GFont.h"
#include "../GBitmap/GBitmap.h"
#include "../GBitset/GBitset.h"
#include "../GBitsetMatrix/GBitsetMatrix.h"
#include "../GBitsetContour/GBitsetContour.h"
#include "../GBitsetOCR/GBitsetOCR.h"
#include "../OCRString/php2stl.h"
#include "../../pugixml/pugixml.hpp"

using namespace std;
using namespace pugi;

namespace ocr{
///Функции создания и выполнения процессов обработки как изображения так и текста.
	
	    /// Выполнение функций обработки изображения. Авто поворот, чистка бордюров, деление на колонки, масштабирование.  image proccesing implementation
        int forkImageProcess(pidID *pidIDArray,int maxFork,int ID);
        void adaptiveFiltration(GBitmap *pechaImg,float scale,int mode);
        void adaptiveFiltration1(GBitmap *pechaImg,float *param,int mode);
  
		///Деление на колонки и запись в файл
		void WriteColumnsData(GBitmap *data, GBitset *set, int nFrm, string &filePath);
		///Деление на разворота на страницы и запись в файл
		void WriteSpreadData(GBitmap *data, GBitset *set, int nFrm, string &filePath);
		///Определение и чистка бордюров вокруг текста
		GBitmap* RemoveBorder(GBitmap *data, GBitset *set, int pageBorder);
		
};
// ---------------- IMPLEMENTATION
# endif

