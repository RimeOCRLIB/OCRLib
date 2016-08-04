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


#ifndef HEADER_OCRALGORITHM
#define HEADER_OCRALGORITHM

#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include <time.h>
#include <math.h>
#include <complex> // Комплексные числа. Быстрое преобразование Фурье. OCRAlgorihtmFFT.cpp
#include "OCRTypes.h"
#include "Vector.h"

#define PI 3.14159265358979   // 3.1415926535897932384626433832795

using namespace std;

namespace ocr {

class OCRFilter{
  protected:
         OCRFilter(void);
		 OCRFilter(int size);
  public:

		 virtual ~OCRFilter();
		 int ncnr;      ///<размерность буферных массивов фильтров
		 int szf;       ///< обычно в 32р системах sfz=4=32/8;  szf=sizeof(int);
		 int *ImBuf;    ///< запрос памяти с очисткой 0
		 int ImBf;      ///< текущий размер массива ImBuf
		 int *ReBuf;    ///< запрос памяти с очисткой 0
		 int ReBf;      ///< текущий размер массива ReBuf
		 int *BufIn;    ///< запрос памяти с очисткой 0
		 int BfIn;      ///< текущий размер массива ImBuf
		 int *BufOu;    ///< запрос памяти с очисткой 0
		 int BfOu;      ///< текущий размер массива ImBuf

 /**\brief Constructs a OCRFilter with #size# buffers handlers*/
 static OCRFilter* create(const int size){return new OCRFilter(size); }

 /// Фильтр низких частот.
 void filterHigh(vector<int> &In,int KHig);
 void filterHigh(int* In,uint size, int KHig);    

 /// Полосовой фильтр Гауссиан (Gaussian).

 void filterLaplasian(vector<int> &In, int KHig, int KLow);


 /// Полосовой фильтр Гауссиан (Gaussian), адаптивный бордюр. Входной буфер HBuf[y]

 void filterLaplasian(vector<int> &HBuf, vector<int> &GBuf, int KHig, int KLow);


 /// Полосовой фильтр Гауссиан (Gaussian), адапт бордюр. Входной ST буфер BufS[y]
 void filterLaplasian(int *Buf1, int *BufTL, int size,  int KHig, int KLow);
    
  
};
    
/**\brief функция подсчета количества единиц в 32 разрядном слове (без _mm_popcnt_u32) */
    int SumBit32(int Sb);
    
/** @brief функция определения площади пересечения двух OCRBox */
    void  intArea(OCRBox *a, OCRBox *b, OCRBox *dataBox);
    
  /// Функция быстрого преобразования Фурье
 // void mainFFT (vector<base> & a, bool invert);   //@@@  не проходит при компиляции обращение к комплексным числам
    
  /// Z-функция строки     
  vector<int>  ZFunction (wstring s);

  /// Sorting  сортировка
  void Sort(vector<int> &GBuf, int NSort);
    
  /// Округление числа до ближайшей степени двойки в большую сторону // power of two
  void power_Two(unsigned int* PowerTwo);
 
  bool isNearP(OCRPoint p1, OCRPoint p2,int d);

    
};

#endif