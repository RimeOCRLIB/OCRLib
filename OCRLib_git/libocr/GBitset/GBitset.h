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
#ifndef _GBITSET_H_
#define _GBITSET_H_
#include "config.h"


#include <iostream>
using namespace std;

#include "GBitmap.h"
#include "OCRTypes.h"
#include "Gletter.h"

//Vic error in file naming
#include "OCRAlgorihtm.h"


#include <vector>
#include <bitset>
typedef std::vector<bool, std::allocator<bool> > bit_vector;

#include <string>
#include <algorithm>
#include <fstream>



using namespace std;
using namespace ocr;

namespace ocr {

/** \breif класс анализа макета страницы. В этом классе:
определяется наклон страницы, количество колонок и координыты , устраняется шум вокруг текста
определяются координаты строк и предварительные координаты букв*/
class GBitset{
protected:
  GBitset(void);
  GBitset(int nrows, int ncolumns, int border=0);
  GBitset(const GBitset *ref);
  GBitset(const GBitmap *ref, short stepW=1, short stepH=1,short flipV=0);
  GBitset(const GBitset *ref, int border);
  GBitset(const GBitmap *ref, const GBitmap *ref1);
public:
  virtual ~GBitset();
  void destroy(void);
  /** @name Construction. */
  //@{
  /** \brief Constructs an empty GBitset object.  The returned GBitset has zero rows and zero columns.  Use function \Ref{init} to change the size of the image. */
  static GBitset* create(void) {return new GBitset;}

  /** \brief Constructs a GBitset with #nrows# rows and #ncolumns# columns. 
	  All pixels are initialized to white. The optional argument #border#
	  specifies the size of the optional border of white pixels surrounding
	  the image.  The number of gray levels is initially set to #2#.  */
  static GBitset* create(const int nrows, const int ncolumns, const int border=0)
  {return new GBitset(nrows,ncolumns, border); }

  /** \brief Copy constructor. Constructs a GBitset by replicating the size, the
	  border and the contents of GBitset #ref#. */
  static GBitset* create(const GBitset *ref)
  {return new GBitset(ref);}

  /** \brief Copy constructor. Constructs a GBitset by replicating the size, the border and the contents of GBitmap #ref#. */
  static GBitset* createResize(const GBitmap *ref, short stepW, short stepH, short flipV)
  {return new GBitset(ref,stepW,stepH,flipV);}
  ///(deprecated)
  static GBitset* createLevel(const GBitmap *ref)  //@@@?
  {return new GBitset(ref, ref);}

  /**\brief Constructs a GBitset by copying the contents of GBitset #ref#.
	  Argument #border# specifies the width of the optional border. */
  static GBitset* create(const GBitset *ref, const int border)
  { return new GBitset(ref,border); }

  /** @name Initialization. */
  //@{
  /**\brief  Resets this GBitset size to #nrows# rows and #ncolumns# columns and sets all pixels to white.  
      The optional argument #border# specifies the size
	  of the optional border of white pixels surrounding the image.  
	  The number of gray levels is initialized to #2#. */
  void init(int nrows, int ncolumns, int border=0);
  /**\brief Initializes this GBitset with the contents of the GBitset #ref#.  
      The optional argument #border# specifies the size of the optional border of
	  white pixels surrounding the image. */
  void init(const GBitset *ref, int border=0);
  /**\brief Initializes this GBitset with the contents of the GBitset #ref#. stepH, stepW is step for reduce image size */
  void init(const GBitmap *ref, short stepW=1, short stepH=1, short flipV=0);
  /**\brief (deprecated) Initializes this GBitset with the contents of the GBitset #ref#. initLevel use adaptive treshould algorithm is*/
  void initLevel(const GBitmap *ref);
  /** Initializes this GBitset content of the GBitset #ref#.*/
  GBitset* operator=(const GBitset *ref);
  /**\brief Initializes all the GBitset pixels to value #value#. */
  void fill(bool value);
  //@}

  /** @name Accessing the pixels. */
  //@{
  /**\brief Returns the number of rows (the image height). */
  unsigned int rows() const;
  /** Returns the number of columns (the image width). */
  unsigned int columns() const;
 /**\brief Returns the size of border (border has different size for every image in integer aligment reason)*/
  unsigned int border_size() const;
  /**\brief Returns a constant pointer to the first byte of row #row#. This pointer can be used as an array to read the row elements. */
  const bool *operator[] (int row) const;
  /**\brief Returns a pointer to the first byte of row #row#.This pointer can be used as an array to read or write the row elements. */
  bool *operator[] (int row);
  /**\brief Returns the size of a row in memory (in pixels).*/
  unsigned int rowsize() const;
  ///<deprecated
  GBitset* rotate(int count=0);
  //@}

protected:
  // bitmap components
  unsigned short nrows;      ///<image width
  unsigned short ncolumns;   ///<image height
  unsigned short border;     ///<border size
  unsigned short bits_per_row;  ///<same as nrows
  short stepBit;
  short srcW;
  short srcH;

public:
  bool  *bits_data;          ///<main memory buffer of image data. All data store in bits so byte store 8 pixels
  //bitset<12000> BitVec;
  int data_size;             ///<size of main memory buffer of image data
#ifdef DEBUGLVL
  GBitmap *destData;         ///<debug graphic Bitmap
  ofstream c_out;            ///<debug file stream
  #ifdef MAC
  drawDataRecord drawGraph;  ///<debug graphic data
  #endif
#endif

//*****************************angle recognition********************************

public:
#ifndef NDEBUG
  void check_border() const;
#endif

//float Alpha;
  short deltaX;
  short lineCount;
#ifdef DEBUGLVL_GREY    
  GBitmap* drawData;
#endif   
#ifdef DEBUGLVL_DRAW   
  GBitmap* drawDataRGB;
#endif
//  GBitmap* line;
  short baseDirection;
  vector<stringOCR>*strArray;
  vector<frameOCR>frameArray;

  /** @name Text string  recognition */
  //@{	
// --------------------------------------------------------
// --- Page String Detector --- GBitset_Starting.h ---
// Подпрограмма управления последовательностью выполнения всех подпрограмм.

  /// Запуск подпрограммы управления выделения строк ПЕЧА
  void pageStringDetector(vector<stringOCR>&strArray_,int mode);

  /// Подпрограмма чистки (border) входного однобитового массива. ВХОД
  void data_null_border();
  // затирает мусор с краев скана + border

// --------------------------------------------------------
// --- Primary String Detector --- GBitset_Letter.h ---
// Подпрограмма выделения строк в пределах ПЕЧА ( РЕЛЬСЫ ).

  /// Подпрограмма предварительного выделения строк для сегментации текста печа.
  void PrimaryStringDetector();

  ///  Подпрограмма вычисления параметра фильтрации строк KHigS;
  void ParameterFilteringDetector(int* Pv1);

 /// Подпрограмма вычисления координат строк с оптимальным параметром фильтрации KHigS;
  void RepeatStringDetector();
				 // получили координаты строк в структуре.
 /// ་Нормализация координат строк
	void stringNormalisation();	
	
//}@

// -------------------------------------------------
  	
/**@name--- Page World Detector --- GBitsetWorld.h ---*/
/// Подпрограмма выделения блоков букв в пределах строки.
// @{
	
  /// Подпрограмма получения координат блоков букв  в пределах страници (квадратики).
  void PageWordDet();


  /// Запуск блока подпрограмм для получения координат блоков букв в пределах строки.
  void LineBlockStart();

  /// Подпрограмма обработки статистики блоков букв короткими импульсами.
  void ShortBurstNpBl(bit_vector  &AMask);

  /// Нелинейная обработка статистики блоков букв в пределах строки, система масок.
  void SystemMasksNpBl(bit_vector  &AMask);

  /// Получение среднего периода блоков букв в пределах строки. Поиск триадами.
  void PeriodBlock();
				 // получили средней период букв MeanSA

  /// запуск подпрограммы получения координат блоков букв в пределах строки.
  void BlockDetector();
				 // получили координаты блоков букв в структуре. Флаги

  /// получение вертикальных сумм блоков букв по всей высоте страницы.
  void BlockSum();
				 // получили вертикальные суммы блоков букв .

  /// Подпрограмма для получения координат блоков слов в пределах страницы.
  void LineDetector();
				 // получили координаты блоков слов в структуре. Флаги
//@}

// --------------------------------------------------------
// --- GBitset Net --- GBitsetNet.h ---

   /// Подпрограмма для выделения рамки вокруг текста печа.
   void GBitsetNet();


   /// Подпрограмма фильтрации строк, для выявления горизонтальных линий
   void DiffLineDetector();
				 // получили координаты строк с горизонтальными линиями.

   /// Подпрограмма для выявления коротких горизонтальных строк.
   void ShortStringDetector();
				 // получили флаг строк с горизонтальными линиями.

   ///  подпрограмма вычисления двойной амплитуды строк.
   void AmplitudeFilteringDetector(vector<int> &BufL,
								   int KHigS, int* Pv1, int* Pv2);


// --------------------------------------------------------
// --- Page Segmentation --- PageSegmentation.h ---

  /// Подпрограмма для получения координат вертикальных колонок в пределах разворота.
  void PageSegmentation();

  /// Базовая подпрограмма для получения координат колонок по горизонтали без учета вертикальных линий между колонками(страницами)
  void FrameDetectorLBase();
    
  /// Подпрограмма для получения координат колонок по горизонтали с учетом вертикальных линий между колонками(страницами)
  void FrameDetectorL();

  /// Программа для обработка скана (разворотов книги), с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦИ.
  void TurnBookDetector();
				 // в TurnX получили примерную координату по x серидины книги.

  /// Программа для обработка скана в виде ОДНОГО БЛОКА (н.создание DjVu файлов).
  void MonoBookDetector();
				 // получили прямоугольник описанный вокруг всех букв скана.


  /// Программы для обработка скана с РАЗДЕЛЕНИЕМ на файлы - КОЛОНКИ.
  /// Подпрограмма для получения координат Y прямоугольников вокруг вертикальных колонок
  void ColumnDetector();
				// получили min и max координаты по Y в колонке с отбрасыванием.

  /// Подпрограмма для получения координат X,Y многоугольников f,fg вокруг вертикальной колонки.
  void ColumnWorking();
				 // получили координаты многоугольников f,fg вокруг вертикальной колонки.

  /// Подпрограмма для получения разных типов бордюров между многоугольниками f,fg
  void BorderWorking();
				 // получили величину бордюра между многоугольниками f,fg вокруг вертикальной колонки.

  /// Подпрограмма для помещения колонок состоящих из блоков в структуру строк.
  void CopyBlock();
				 // получили координаты вертикальных колонок в структуре строк.

  // Подпрограмма для получения координат колонок по вертикали.
  // работает но не применили
  //void FrameDetectorH();

// --------------------------------------------------------
// --- Page World Tib Detector --- GBitsetWorldTib.h ---
// Подпрограмма выделения Tib букв в пределах строки.

  /// Запуск программы получения координат Tib букв в пределах страници (квадратики).
  void PageWordDetTib();


  /// Запуск программы получения координат букв в пределах строки, параметр MeanSA.
  void ParameterMeanDetector();

  /// Запуск блока подпрограмм для получения координат букв в пределах строки.
  void LineLetterStart();

  /// Нелинейная обработка статистики букв короткими импульсами в пределах строки.
  void ShortBurstNP(bit_vector  &AMask);

  /// Нелинейная обработка статистики букв в пределах строки, система масок.
  void SystemMasksNP(bit_vector  &AMask);

  /// Подпрограмма получения среднего периода букв в пределах строки,поиск триадами
  void PeriodLetters();

  /// Подпрограмма для получения координат букв в пределах строки.
  void RepeatLetterDetector();

  /// Подпрограмма для получения точных координат букв в пределах строки.
  void AccurateLetterDetector();

  /// Функция для получения точных координат букв (дотягивание по нолям).
  void AccurateLetFunction();

  // получили координаты букв в структуре.


// --------------------------------------------------------
// --- Page World Eng Detector --- GBitsetWorldEng.h ---
// Подпрограмма выделения Eng букв в пределах строки.

  /// Запуск программы получения координат Eng букв в пределах страници (квадратики).
  void PageWordDetEng();

  // получили координаты букв в структуре.



// --------------------------------------------------------
// --- Correct SqH --- GBitsetCorrectSqH.h ---
// Подпрограмма коррекции координат букв (квадратиков) в пределах строки.


  /// Подпрограмма  вертикальной коррекции координат букв (квадратиков).
  void CorrectSqH();

  // Запуск программы предвармтельной вертикальной коррекции квадратиков
  // не написана
//  void PredvCorrectLetH();

  /// Запуск программы вертикальной коррекции положения квадратиков по верху
  void CorrectLetterH();

  /// Запуск программы получения координат прямоугольников вокруг точек
  void SquarePointDetector();

  // откорректировали координаты квадратиков по вертикали.



// --------------------------------------------------------
// --- Page Point Detector --- GBitsetPoint.h ---
// Подпрограмма распознавания точек в пределах строки.
// Текущая работа.

  /// Подпрограмма  работы с точками в пределах строоооки.
  void pagePointDetector();


  /// Запуск подпрограммы распознавания точек в пределах строки.
  void RecognitionPoint();

  /// Запуск подпррограммы нахождения координат X,Y отдельно стоящей точки.
  void XYPointLocal( int xp0, int xp1, int yp0, int yp1 );

  /// Запуск подпрограммы нахождения центра отдельно стоящей точки.
  void CorePointLocal( int xp0, int xp1, int yp0, int yp1 );

  /// Запуск подпрограммы статистики размера точек в пределах строки.
  void StatPointLocal( int* WPnt, int* HPnt, int* MPoint, int* ms, int* ns );

  /// Запуск подпрограммы матрици распознавания точек в пределах прямоугольника.
  void MatrixPoint( int WPnt, int HPnt );

  /// Запуск подпрограммы A маски распознавания точек в пределах прямоугольника.
  void MatrixPointA( int MPoint, int ms, int ns );

  /// Запуск подпрограммы B маски распознавания точек в пределах прямоугольника.
  void MatrixPointB();

  // получили маркер (в структуре) на наличие точки после буквы.


// --------------------------------------------------------
// --- Correct SqL --- GBitsetCorrectSqL.h ---
// Подпрограмма коррекции координат букв (квадратиков) в пределах строки.

  /// Подпрограмма  вертикальной коррекции координат букв (квадратиков).
  void CorrectSqL();


  /// Запуск программы горизонтальной коррекции положения квадратиков
  void CorrectLetterL();

  /// Запуск подпрограммы вертикальной коррекции нижней полки квадратиков
  void CorrectLetterLH();

  /// Запуск подпрограммы статистики коррекции нижней полки квадратиков
  void StatCorrectLetter(vector<int> &WordS, int sizeWdS);

  // откорректировали координаты квадратиков по горизонтали.


// --------------------------------------------------------
// --- Page Stack Detector --- GBitsetStackDetector.h ---
// Подпрограмма распознавания вертикальных стеков в пределах строки.

//#define pageStackDetector   // блок выключен
#ifdef pageStackDetector
/*
  // Запуск подпрограммы выделения вертикальных стеков в пределах строки.

  void pageStackDetector();

  // Запуск подпрограммы предвармтельнного определения наличия стеков
  void PredvStackDetector();

  // Запуск подпрограммы определения параметров стеков
  void StackArgument(vector<int> &BufW, int DltNstrN,
					 int s0, int s1, int s3, int s4);

  // Запуск подпрограммы определения параметров букв
  void WorldArgument(vector<int> &BufW, int DltNstrN,
					 int x0, int x1, int y0, int y1 );

  // Запуск подпрограммы окончательного определения наличия стеков
  void FlagStackDetector();

  // получили маркер (в структуре) на наличие стека вместо коренной буквы.
*/
#endif


// --------------------------------------------------------
// --- Vowel Detector --- GBitsetVowelDetector.h ---
// Подпрограмма распознавания верхних огласовок в пределах строки.


  ///Подпрограмма определения наличия верхних огласовок в пределах строки.
  void VowelDetector();

  /// Подпрограмма предварительнного определения наличия верхних огласовок в пределах строки.
  void PredvVowelDetector();


// --------------------------------------------------------
// --- Algorithm --- GBitset_algorithm.h ---


  /// Фильтр низких частот.
  //@@@/ void filterHigh(vector<int> &In,int KHig);

  // Фильтр низких частот, адаптивный бордюр.
  //void filterHighM(vector<int> &In, int KHig);

  /// Полосовой фильтр Гауссиан (Gaussian).
  //void filterLaplasian(vector<int> &In, int KHig, int KLow);

  /// Полосовой фильтр Гауссиан (Gaussian), адаптивный бордюр.
  //void filterLaplasianM1(vector<int> &In, int KHig, int KLow);

  /// Полосовой фильтр Гауссиан (Gaussian), адаптивный бордюр. Входной буфер HBuf[y]
  //void filterLaplasianM(vector<int> &HBuf, vector<int> &GBuf, int KHig, int KLow);

  /// Полосовой фильтр Гауссиан (Gaussian), адапт бордюр. Входной ST буфер BufS[y]
  //void filterLaplasStM(int *Buf1, int *BufTL, int size,  int KHig, int KLow);


  /// вертикальные и горизонтальные локальные сумммы входного массива bits_data
  /// vertical and horizontal local summmy //
  void BitsSummmyWH(int NSumm);

  /// Подсчет единичных битов в числе
  //void bitsCountA( int Sbits );
  //void bitsCountB( int Sbits );
  //void bitsCountC( int Sbits );

  /// ВИЗУАЛИЗАЦИЯ АНАЛОГОВАЯ вертикальных графиков (GBitset)
  void drawNFase(vector<vector<int> > &INFase, int color);

  /// Преобразование ГИЛБЕРТА с параметром (ширина) PGilb
  void filterGilbert(vector<int> &HBuf, vector<int> &GBuf,
					 int MGilb, int PGilb );

  /// Визуализация аналоговая вертикальных графиков L
  void LDraw(vector<int> &HBuf, int sm, int color);
  /// Визуализация аналоговая вертикальных графиков H переменной длинны
  void LDrawVar(vector<int> &HBuf, int HDrawSize, int sm, int scope, int color);
  /// Визуализация аналоговая горизональных графиков W
  void WDraw(vector<int> &HBuf, int sm, int color);
  /// Визуализация аналоговая горизональных графиков W переменной длинны
  void WDrawVar(vector<int> &HBuf, int WDrawSize, int sm, int scope, int color);
  /// Визуализация аналоговая  прямоугольников в структурах для строк
  void stringDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для колонок
  void frameDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для блоков слов
  void lineDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для блоков букв
  void blockDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для букв
  void wordDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для точек
  void pointDraw(int color);
  /// Визуализация аналоговая  прямоугольников в структурах для огласовок
  void vowelDraw(int color);
  /// Рисование вертикальных линий
  void LineVertical(int sm, int color);
  /// Визуализация аналоговая результатов векторизации контуров //
  void vDraw();
  /// Polar coordinates  полярные координаты
  void PolarCoordinates(vector<int> &HBuf, vector<int> &GBuf, int LDrawSize,
						int smX,  int smY, int scope,int color);

  /// Подпрограмма расчета матожидания, дисперсии.
  void Statistics(vector<int> &HBuf, vector<int> &GBuf, int Porog, int Delta );

  /// Amplitude Frequency  амплитуда частота
  void AmplitudeFrequency(vector<int> &GBuf,int smX,int smY,int scope,int color);

  /// Подпрограмма гребенчатого фильтра Comb Filter с параметрами ( dt, g ).
  void CombFilter(vector<int> &GBuf, int dt, int g);

  /// Подпрограмма расчета средней амплитуды в массиве с параметром ( ms-сдвиг ).
  void AmplitudeS(vector<int> &GBuf, int ms, int &AmSum);

  /// Подпрограмма визуализация спектра imaging spectrum.
  void ImagingSpectrum(vector<int> &HBuf);

  /// Подпрограмма объединяет: гребенчатый фильтр, амплитудный детектор.
  void FunctionSpectrum(vector<int> &HBuf, int dt, int &AmSum);

  /// Подпрограмма объединяет: гребенчатый фильтр, амплитудный детектор. ST BufR.
  void FunctionSpectrSt(int *Buf2, int size,  int dt, int &AmSum);


// --------------------------------------------------------
// ---- Test ---- GBitset_Test.h ---

  /// start Test.
  void startTest();

  /// интегрирующий фильтр (integrating filter), адаптивный бордюр.Test
  void filterIntegrator(vector<int> &In, int KHig);

  /// Фильтр низких частот.Test
  void filterHighTest(vector<int> &In,int KHig);

  /// Полосовой фильтр Гауссиан (Gaussian), адаптивный бордюр.Test
  void filterLaplasianTest(vector<int> &In, int KHig, int KLow);

  /// тестирование массива BitsBuf
  void testBitsBuf();


// --------------------------------------------------------
  // Подпрограмма нормализации выходных данных с учетом величины бордюра
		void data_normalisation();
// --------------------------------------------------------
/* */

  int KFStrok;     ///< номер оптимального коэффициента фильтрации строк
  int KHigS;       ///< оптимальный параметр фильтрации строк
  int DeltaNstr;   ///< вычисленная высота строки
  int DeltaNstrKH; ///< вычисленная высота строки с учетом коэффициентов
			  ///< масштаба размера печа, KH=srcH/nrows;  KW=srcW/ncolumns;
  int NLet;        ///< вычисленное колличество букв в строке, умноженное на 4
  int Nstr;        ///< номер текущей обрабатываемой строки
  int Nwrd;        ///< номер текущей обрабатываемой буквы
  int DeltaKh;     ///< коэффициент формы страници (100*ncolumns/srcW*srcH/nrows;)
  int NStrok;      ///< вычисленное колличество строк в печа
  int NBlockP;     ///< вычисленное колличество блоков букв в печа
  int hStrMax;     ///< вычисленное значение максимальной высоты строки на странице
  int WordSMax;    ///< вычисленное значение максимального колличества букв
		            ///< в строке по по всей странице

  signed  char GrafT[32][16]; ///< массив управления графикой во всех подпрограммах

/* */
  int MeanSA;      ///< вычисленный средний по строке период букв умноженный на 10
  int MeanCol;     ///< вычисленный средний по странице период букв умноженный на 10
  int VarianceSA;  ///< вычисленная дисперсия периода букв умноженныя на 10private:
  int ncnr;        ///< наибольшее из ncolumns и nrows те из ширины и высоты печа

  vector<int> HBuf;   ///< базовый массив
  vector<int> GBuf;   ///< базовый массив

  vector<int> BufS;   ///< текущий массив
  vector<int> BufT;   ///< текущий массив
  vector<int> BufR;   ///< текущий массив
  vector<int> BufL;   ///< текущий массив
  vector<int> BufHelp;///< текущий массив

  vector<int> HStr;   ///< массив заполненный горизонтальными суммами
  vector<int> HStS;   /// массив заполненный горизонтальными дифференциальными суммами
  vector<int> LStr;   ///< массив заполненный вертикальными суммами
  vector<int> LStS;   ///< массив заполненный вертикальными суммами по всей странице
  vector<int> LineDetectArray; ///< массив вертикальных сумм в интервалах между колонками (не маскированный)
    
/* */

//  int *BufIn=(int*)farcalloc(10000);    // запрос памяти с очисткой

 OCRFilter *SP;     ///<объект блока фильтров сигналов и буферных массивов фильтрации

 int szf;       ///< обычно в 32р системах sfz=4=32/8;  szf=sizeof(int);
 int *ImBuf;    ///< запрос памяти с очисткой 0
 int ImBf;      ///< текущий размер массива ImBuf
 int *ReBuf;    ///< запрос памяти с очисткой 0
 int ReBf;      ///< текущий размер массива ReBuf
 int m;         ///< ширина двухмерного массива (если массивы так используем)
 int *BufIn;    ///< запрос памяти с очисткой 0
 int BfIn;      ///< текущий размер массива ImBuf
 int *BufOu;    ///< запрос памяти с очисткой 0
 int BfOu;      ///< текущий размер массива ImBuf

 bool *BitsBuf; ///< bool *B;

 int TurnX;     ///< координата серидины разворота книги (по X).
 int Brd;    ///< аналоги кнопок   ( 1 - кнопока нажата ).
 int AutoBorderBtn;  ///< обработка скана в виде ОДНОГО БЛОКА.
 int SplitPagesBtn;  ///< обработка скана (разворотов книги) с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦИ.
 int SplitColumBtn;  ///< обработка скана с РАЗДЕЛЕНИЕМ на файлы - КОЛОНКИ.


// --------------------------------------------------------
// --- Page Structure World --- GBitsetStructure.h ---
/// Подпрограмма структурализации букв.
// Текущая работа.
  void StructureWorld();


//*****************************angle recognition********************************

		int maxBitAdress;
		/// три запуска с разными параметрами программы pageStart
		float detectRotation();
		/// запуск программы вычисления угла наклона текста
		void pageStart(vector<vector<int> > &INFase,
					   vector<vector<int> > &INGilb,
					   vector<vector<int> > &Buf1,
					   vector<vector<int> > &Buf2,
					   vector<int> &LFase,
					   vector<int> &LGilb,
					   float &Alpha);

		///пространственныый Лапласиан, нормировка скана
		void contourRecognition();

		//void readHSum(vector<int> &H);

		/// ПОДСЧЕТ СТАТИЧЕСКИХ ЛОКАЛЬНЫХ СУММ
		void localSumStatic(vector<vector<int> > &INFase,
							vector<vector<int> > &INGilb,
							vector<vector<int> > &Buf1,
							vector<vector<int> > &Buf2);

		/// ПОДСЧЕТ ЛОКАЛЬНЫХ СУММ И ИХ ФИЛЬТРАЦИЯ
		void localSum(vector<vector<int> > &INFase,
					  vector<vector<int> > &INGilb,
					  vector<vector<int> > &Buf1,
					  vector<vector<int> > &Buf2);

		/// вычисление MAX амплитуд по всем срезам
		void CalculatAmplitude(vector<vector<int> > &INFase,vector<vector<int> > &INGilb);

		/// вычисление смещений между всеми срезами
		void deltaSlice(vector<vector<int> > &INFase,vector<vector<int> > &INGilb);

		/// преобразование массива срезов в один массив
		void matExpDispers(vector<vector<int> > &INFase,
						   vector<vector<int> > &INGilb,
						   vector<int> &LFase,
						   vector<int> &LGilb );

		/// Вычисление распределения вероятностей
		void ScheduleProbability(vector<int> &LFase,
								 vector<int> &LGilb,
								 int &Porog,
								 int &Delta );

		/// Подпрограмма расчета моментов распределения: матожидания, дисперсии
		void calculatemoments(vector<int> &LGilb,
							  float &mean,
							  float &variance,
							  float &Alpha);



		int nfasSt;  ///< количество фазовых сечений   1< nfas < 128;  32  базовое значение
		int nfas;    ///< количество фазовых сечений   1< nfas < 128;  16 или 32 вычисляемое значение
		int KHig;    ///< порядок высокочастотного фильтра  2< KHig =<63; вычисляемое значение 8 или 16
		int KLow;    ///< порядок низкочастотного фильтра  2< KLow =<63; вычисляемое значение 8 или 16
		int N0Fase;  ///< количество данных в нолевом слайсе массива INFase
		int N0Gilb;  ///< количество данных в нолевом слайсе массива INGilb
		int SumNAmp; ///< суммарное число хороших отсчетов по всем слайсам


//******************************************************************************

// --------------------------------------------------------
};




// --------------------------------------------------------

// ---------------- IMPLEMENTATION

inline unsigned int
GBitset::rows() const
{
  return nrows;
}

inline unsigned int
GBitset::columns() const
{
  return ncolumns;
}
inline unsigned int
GBitset::border_size() const
{
  return border;
}


inline unsigned int
GBitset::rowsize() const
{
  return bits_per_row;
}


inline bool*
GBitset::operator[](int row)
{
  if (row<0 || row>=nrows) {
	return  bits_data;
  }
  return &bits_data[row * bits_per_row + border];
}

inline const bool*
GBitset::operator[](int row) const
{
   if (row<0 || row>=nrows) {
	return bits_data;
  }
  return &bits_data[row * bits_per_row + border];
}

inline GBitset*
GBitset::operator=(const GBitset*ref)
{
  init(ref, ref->border);
  return this;
}
// ---------------- THE END

}//namespace

#endif


