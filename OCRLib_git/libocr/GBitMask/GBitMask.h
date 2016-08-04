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

#ifndef GBitMask_H
#define GBitMask_H



# include "GBitmap.h"
# include "config.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace ocr {


	class GBitMask32 {
	public:
		GBitMask32();
		unsigned int imgOn[128];   ///< исходная область для получения масок
		unsigned int On[128];      ///< центральная маска On
		unsigned int Off[128];     ///< переферийная маска Off
		//unsigned int BolS[128];  ///< вспомогательный массив  matriT,h
		int status;                ///< статус маски
		int correlation;           ///< величина корреляции маски
		int xMask,yMask;           ///< координаты крайне верхне-левого угла маски в исходном изображении
        int xMax,yMax;             ///< координаты крайне верхне-левого угла маски в исходном изображении при оптимальной корреляции
		int mW,mH;                 ///<габариты массивов маски (mHImg=mH)  // mW ширмна (фиксированная) прямоугольника маски
		int imgW;                  //<габариты выделенного признака по ширине (высота равнна mH)
		int mWOn,mHOn;             ///<реальные габариты зоны ON
		int x0,x1,y0,y1;           ///< габариты прямоугольной однопиксельной маски пересечений
		char x0st, x1st, y0st, y1st; ///< статус прямоугольной однопиксельной маски пересечений
		int NMask,NMask_prim;       ///< площадь (количество единиц) центральной маски On
		int dlt0;                  ///величина растискивания(дерривации) изображения маски OFF
        int dlt1;                  ///величина эрозии изображения в маске ON
        int dlt2;                  ///величина эрозии изображения в маске OFF
		int maskType;              ///<тип маски 1 -маска On-Off. Для других типов не выполняется эррозия!(маски малых объектов)
		int id;                    ///< номер маски в букве

  //******************************************************************************
//инициализация параметров маски dlt0, dlt1, dlt2 и типа маски maskType по ее габаритным размерам.
void set();
        
void copy(GBitMask32* dest);
        // РАСПАКОВКА

// Распаковка битовой маски из массива 32 int  в массив 1024 bool
// -----------------------------------------------------------------------------
void unPackBitMask(unsigned int *MaskInt,
							   unsigned char *MaskDest);						// ЗАПАКОВКА

// Запаковка битовой маски в массиве 1024 bool в массив  32 int. Вход1
// -----------------------------------------------------------------------------

void packBitMask(unsigned int *MaskInt,
							 unsigned char *inputData);						// ГРАФИКА
// Вывод на экран битовых масок ON и OFF упакованных в массив 64 long int. Вход1
// -----------------------------------------------------------------------------
GBitmap* unPackMask();
// Вывод на экран битовых масок ON и OFF упакованных в массив 64 long int. Вход1
// -----------------------------------------------------------------------------
GBitmap* unPackMaskOff();
						// ПСЕВДОГРАФИКА
// Вывод на экран битовых масок ON и OFF упакованных в массив 32 int. Вход1
// -----------------------------------------------------------------------------
void printMask(ofstream &c_out);
//отличается от предыдущей только DT() нужна для вывода в базе при отключенной псевдографике
						// ПСЕВДОГРАФИКА
// Вывод на экран битовых масок ON и OFF упакованных в массив 32 int. Вход1
// -----------------------------------------------------------------------------
void printMask1(ofstream &c_out);
void printMask();
						// ПСЕВДОГРАФИКА
// Вывод на экран битовых масоки упакованных в массив 32 int. Вход2
// -----------------------------------------------------------------------------
void printMask(unsigned int *MaskInt, ofstream &c_out);
void printMask(unsigned int *MaskInt);
				 // 00000000000000001111111111111111 //

// программа нормализации положения перефирийной области  "Off" и центральной
// области  "On"  (сдвиг вверх). Подпрограмма для MatrixT.
// -----------------------------------------------------------------------------
void NormalT();
// Подпрограмма для setOnOffMatrixT
// (обтекания перефирийной области  "Off" с переходной областью)
// -----------------------------------------------------------------------------
//void MatrixOn(int dlt, ofstream &c_out);

void MatrixDltOn(int dlt);
void MatrixDltOff(int dlt);

void MatrixOff(int dlt);
//Подсчет количестваа единиц в маске On
int  maskCount();

// формирование обтекающей перефирийной области  "Off" с переходной областью
// -----------------------------------------------------------------------------
//Формирование маски с прямоугольной маской Off dlt-ширина бордюра,
//дополняющего маску On до прямоугольной маски Off
void setOnOffInMaskRect(int dlt);


void setOnOffMatrixT(int stepH,
					 int stepW);

// Запаковка битовой маски из битмапа
// -----------------------------------------------------------------------------
void packBitMask(GBitmap *img);

void packBitMask();
// Запаковка битовой маски из массива 1024 (серый или чб) в массив  32 int.Вход2
// вычисление  mW, mH
// -----------------------------------------------------------------------------
void resizeMask(int stepW,int stepH,ofstream &c_out);

	};

class GBitMask128 {
	public:
		GBitMask128();
		//unsigned int imgOn[1024]; // исходная область для получения масок
		unsigned int On[512];    // центральная маска On
		//unsigned int Off[1024];   // перефирийная маска Off
		//unsigned int mHor[128];
		//unsigned int mVer[128];
		char status;     // статус маски
		int xMask,yMask; // координаты крайне верхне-левого угла маски относительно центра буквы
		int mW,mH;       // габариты маски: по ширине (фиксированный) 64, по высоте  1-128
		int x0,y0,x1,y1; // габариты прямоугольной однопиксельной маски пересечений
		char x0st, x1st, y0st, y1st; // статус прямоугольной однопиксельной маски пересечений
		int NMask;       // площадь (количество единиц) центральной маски On
		int dlt0,dlt1,dlt2; //характеристики маски
		int maskType;    //тип маски


							// ПСЕВДОГРАФИКА
// Вывод на консоль битовых масок ON и OFF упакованных в массив 64 long int. Вход1
// -----------------------------------------------------------------------------
void printMask();
						// ГРАФИКА
// Вывод на экран битовых масок ON и OFF упакованных в массив 64 long int. Вход1
// ----------------------------------------------------------------------------
GBitmap* unPackMask();
	
void packBitMask128(GBitmap *img);

void testVector(ofstream &c_out);

int hashValue();   ///функция необходима для создания уникального id-номера буквы 
};

};
#endif
