//  GBitmap.h
//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org

// if ( ImBuf !=NULL ) farfree(ImBuf);  // освобождение памяти
// q=(тип_q *)farmalloc(n_byte);        // запрос памяти без очистки 0
// q_new=farrealloc(q_old,n_byte);;     // изменение размера блока

#ifndef _GBITMAP_H_
#define _GBITMAP_H_
#include "../GMemory/GVector.h"
#include "../OCRAlgorithm/OCRAlgorithm.h"
#include "../OCRTypes/OCRTypes.h"
#include "../PNG/lodepng.h"
#include "../config.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

namespace ocr {
class GBitmap {
  protected:
    GBitmap();
	GBitmap(int ncolumns, int nrows);
	GBitmap(int ncolumns, int nrows, int mode);
	GBitmap(const GBitmap *ref);
	GBitmap(const GBitmap *ref, int x0, int y0, int w, int h);
	GBitmap(const GBitmap *ref, float scale, float rotation, int mode);
	GBitmap(const char *path);
	GBitmap(unsigned char *dataPNG, uint size);

  protected:
	// bitmap components
	int nrows;
	int ncolumns;
	int border;
	int bytes_per_row;

  public:
	unsigned char *bytes_data;  ///<буфер байтовых данных битмэпа (большой распакованный массив)
	unsigned char *bites_data;  ///<буфер битовых данных битмэпа (маленький запакованный массив)
	unsigned char *bytes_data_buf; ///<промежуточный буфер обработки битмэпа

	unsigned int data_size_p;   /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
	unsigned int data_size_p32; /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
	int colorMode;
	int colorModeOut;           /// флаг выходного формата изображения. По умолчанию равен colorMode
    ushort imgW;                ///< ширина исходного PNG

	unsigned int pack_flag;     ///< 1 массив bites_data запакован функцией packImg
	int data_size;
	// unsigned int h;
	// unsigned int w;
	unsigned int imgX0, imgX1, imgY0, imgY1; //габариты изображения
	unsigned int mask256[256];

	/// Массивы OCRLineTable///
	unsigned char table_1[256]; /// предпросчитаная таблица особых точек //
	unsigned int
		table_2[256];           ///< предпросчитаная таблица адресов смещения точек вдоль фокальной линии (упаковано по 2 short) //
	unsigned long
		table_3[256];           ///< предпросчитаная таблица адресов точек начала фокальных линий (упаковано по 4 short) //

	/// Массивы OCRAlphaTable. ///
	float tabAlpha_1[361]; // предпросчитаная таблица коэффициента корреляции уголов. //

	///__________________function declaration__________________//
	void destroy(void);
	/** @name Construction. */
	//@{

	/**\brief Constructs a GBitmap with #nrows# rows and #ncolumns# columns.  All
	  pixels are initialized to white. */
	static GBitmap *create(const int ncolumns, const int nrows) { return new GBitmap(ncolumns, nrows); }

	/**\brief Constructs a GBitmap with #nrows# rows and #ncolumns# columns in color mode.  All
	 pixels are initialized to white. */
	static GBitmap *create(const int ncolumns, const int nrows, const int mode) {
		return new GBitmap(ncolumns, nrows, mode);
	}

	/**\brief Copy constructor. Constructs a GBitmap by replicating the size, the
	  border and the contents of GBitmap #ref#. */
	static GBitmap *create(const GBitmap *ref) { return new GBitmap(ref); }

	/**\brief Constructs a GBitmap with region of GBitmap* ref */
	static GBitmap *createRegion(const GBitmap *ref, int x0, int y0, int w, int h) {
		return new GBitmap(ref, x0, y0, w, h);
	}

	//
	/**\brief Constructs a GBitmap by copying and scale the contents of GBitmap #ref#. */
	static GBitmap *createScale(const GBitmap *ref, const float scale) { return new GBitmap(ref, scale, 0, 0); }

	/**\brief Constructs a GBitmap by copying and scale the contents of GBitmap #ref#. */
	static GBitmap *createScale(const GBitmap *ref, const float scale, int mode) {
		return new GBitmap(ref, scale, 0, mode);
	}

	/**\brief Constructs a GBitmap by copying and rotate the contents of GBitmap #ref#.*/

	static GBitmap *createRotate(const GBitmap *ref, const float rotation) { return new GBitmap(ref, 0, rotation, 0); }
	//
	/**\brief Constructs a GBitmap by copying, scale and rotate the contents of GBitmap #ref#.*/
	static GBitmap *createScaleRotate(const GBitmap *ref, const float scale, const float rotation) {
		return new GBitmap(ref, scale, rotation, 0);
	}

	/**\brief Constructs a GBitmap by unpack PNG file*/
	static GBitmap *createFromPNG(string &inputData) { return new GBitmap(inputData.c_str()); }

	/**\brief Constructs a GBitmap by unpack PNG file*/
	static GBitmap *createFromPNG(const char *inputData) { return new GBitmap(inputData); }

	/**\brief Constructs a GBitmap by unpack PNG data*/
	static GBitmap *createFromPNGData(unsigned char *dataPNG, uint size) { return new GBitmap(dataPNG, size); }

	/**\brief Constructs a GBitmap by unpack PNG RGB data*/
	static GBitmap *create(const char *path) { return new GBitmap(path); }

	/** @name Initialization. */
	//@{
	/**\brief Resets this GBitmap size to #nrows# rows and #ncolumns# columns and sets and color mode
	  all pixels to white. */
	void init(int ncolumns, int nrows, int mode);
	/**\brief Initializes this GBitmap with the contents of the GBitmap #ref#.  */
	void init(const GBitmap *ref);
	/**\brief Initializes a GBitmap with region of GBitmap* ref */
	void initRegion(const GBitmap *ref, int x0, int y0, int w, int h);
	/**\brief Initializes this GBitmap with the scale contents of the GBitmap #ref#. */
	void init(const GBitmap *ref, float scale, float rotation, int mode);
	/**\brief Initializes this GBitmap with the contents of the file with Image Types mode */
	void init(const char *path, int mode, int invert);
	/**\brief Initializes this GBitmap with the contents of the string contained tiff file data */
	void init(string &inputData, int invert);
	/**\brief Initializes this GBitmap by ImageMagick library */
	void initImage(const char *path);
	/**\brief Initializes this GBitmap with the contents of the PNG file */
	void initPNG(const char *path);
	/**\brief Initializes this GBitmap with the contents of the PNG file */
	void initPNGData(unsigned char *dataPNG, uint size);

	/**\brief Save GBitmap contents into the PNG file accordind to color mode */
	void savePNG(const char *path);
	/**\brief Save GBitmap contents into the PNG file accordind to color mode */
	void savePNG(string &path) { return savePNG(path.c_str()); };
	/**\brief Save GBitmap PNG contents into the  string to color mode */
	void strPNG(string &data);

	/** copy all the GBitmap ref pixels to destination */
	GBitmap *operator=(const GBitmap *ref);
	/**\brief Initializes all the GBitmap pixels to value #value#. */
	void fill(unsigned char value);
	/**\brief Initializes all the GBitmap pixels to invert value */
	void invert();

	// универсальная функция размытия (blur) серого или битового массива bytes_data двухмерным Гауссианом ( Gaussian )
	void gaussian(int amplitude);
	// TMP test
	void gaussianT(int amplitude);
	// универсальная функция размытия (blur) серого или битового массива bytes_data двухмерным Гауссианом ( Gaussian )
	// Не используются боковые массивы.
	void gaussianF(int amplitude);
	// функция размытия (blur) серого или битового массива bytes_data усреднением скользящим квадратом размером
	// BASExBASE.
	void FilterIntegral(int BASE);

	// функция определения средней толщины штриха в буквах по всему графическому тексту (в пикселях). Не очень точная.
	int lineWidth();
	// Возвращает среднее значение толщины штриха line_w в буквах по всему графическому тексту (в пикселях).

	// быстрое автоопределение наличие серого или черно-белого в битмапе.
	// возвращает 1 если в битмапе встречается серое и надо применять адаптивный порог.
	// возвращает 0 если битмап уже бинаризован.
	int DetectGradations();
	// функция анализирует гистограмму изображения и нормализует изображение по гистограмме.
	void autoLevel();

	// функция определения пространственной ориентации Тибетского графического текста.
	void textOrientation(int *lineOrient, int *lineHeight);
	// возвращает среднею высоту строки lineHeight (без влияния рамок вокруг текста)
	// возвращает:
	// lineOrient=1 если текст нормально ориентирован.
	// lineOrient=-1 если текст повернут на 180.
	// lineOrient=0 если ориентация текста определена не достоверно.

	// Функция вертикального зеркального отражения графического текста. Меняет местами верх и низ
	// графического текста перестановкой строк по вертикали.
	void VerticalMirror();

	// Функция вертикального поворота Тибетского графического текста. Меняет местами верх и низ
	// графического текста. Работает на основе textOrientation.
	void textVerticalRotation(int Orient, int *lnOrient, int *lnHeight);
	// Orient=1 вертикальное зеркальное отражение графического текста, 2 поворот текста на +180°.
	// Возвращает параметр вертикальной ориентации lnOrient.
	// Возвращает оценку средней высоты строки lnHeight.

	// Функция определяет и ивертирует негативное изображение в сером графическом тексте.
	int textNegative();
	// Возвращает параметр negative=0 нормальное изображение, =1 негативное изображение.

	// Функция определения вертикального размера Тибетского графического текста по букве "B".

	float scaleTibetanB(int xB, int yB);

	// получает на вход координаты центра буквы "B" полученные с помощью мышки.
	// возвращает вертикальный размер Тибетского буквы "B" с pix.
	// Входной массив bytes_data должен быть бинаризован.

	// Функция определения вертикального размера Тибетского графического текста по букве "B".
	// То же что и scaleTibetanB, но выполнена не на указателях, а на массивах с [].
	float scaleTibetanBM(int xB, int yB);
	// получает на вход координаты центра буквы "B" полученные с помощью мышки.
	// возвращает вертикальный размер Тибетского буквы "B" с pix.
	// Входной массив bytes_data должен быть бинаризован.

	// Функция стирает черное с краев скана.
	void eraseBorder(uint dX, uint dY);
	// Функция заполняет значением "uint t" бордер с краев скана. Величина бордер "uint bord".
	void variationBorder(uint t, uint bord);
	// Функция для отработки фрагментов кода.
	// void test();

	//@}

	/** @name Accessing the pixels. */
	//@{
	/**\brief Returns the number of rows (the image height). */
	unsigned int rows() const;
	/** Returns the number of columns (the image width). */
	unsigned int columns() const;
	/**\brief Returns the size the border. */
	unsigned int borderSize() const;
	/**\brief Returns a constant pointer to the first byte of row #row#.
	  This pointer can be used as an array to read the row elements. */
	const unsigned char *operator[](int row) const;
	/**\brief Returns a pointer to the first byte of row #row#.
	  This pointer can be used as an array to read or write the row elements. */
	unsigned char *operator[](int row);
	/**\brief Returns the size of a row in memory (in pixels).  This number is equal
	  to the difference between pointers to pixels located in the same column
	  in consecutive rows.  This difference can be larger than the number of
	  columns in the image. */
	unsigned int rowsize() const;
	int color() { return colorMode; }
	void put(int x, int y, int color);
	int get(int x, int y);

	/**\brief// Тестирование распознования макета.*/
	// float TextDetect();

	/**\brief  // Вычисление средней толщины штриха по всему графическому тексту.*/
	float LineWidthImg();
	// Возвращает среднее значение толщины штриха в буквах (в пикселях и долях пиксела).

	/**\brief  // Вычисление для каждой точки графического текста средней (по квадрату или региону) толщины штриха.*/
	void LineWidthRegion(int BASE);
	// Возвращает массив в котором для каждого пиксела записано среднее значение толщины штриха по квадрату размером
	// BASExBASE).

	/**\brief  Выделение контуров при помощи простейшего пространственного Лапласиана. Тестовая функция. */
	void detectContourT();

	/**\brief  Функция определения габаритов изображения. mode=1 -
  Убирает белый фон вокруг изображения и возвращает новый GBitmap; mode=0 возвращяет габариты без создания нового
  изображения frame - отступ от края изображения (белый бордюр) в новом GBitmap.*/
	GBitmap *detectRegion(int frame, int *x0, int *x1, int *y0, int *y1, int mode);

	/**\// автоопределение количества градаций в битмапе с управляемой инверсией.
  Autodetect the number of gradations in the bitmap with a controlled inversion*/
	void AutodetectGradations(int invert);
	/**\brief  Упаковка большого битмапа в маленькй int массив. */
	void packImg32(int invert);
	/**\brief  Распаковка маленького битового массива int в большй битмап. */
	void unpackImg32(int invert);
	/**\brief Распаковка маленького битового массива int в большй битмап.
	 Используется для определения степени перекрытия распознаваемой буквы и исходного изображения в GBitsetMatrix.
	 GBitmap как и GBitsetMatrix в этом случае транспонирован по вертикали (x и y поменялись местами)
	 в отличие от GBitsetMatrix в GBitmap записано равное количество пикселов изображения и битов данных.
	 Транспонирование по вертикали позволяет копировать вертикальные участки изображения по 32 пиксела шириной
	 с помощью команды memcpy */
	void packImg32V(int invert);
	void unpackImg32V(int invert);

	/**\brief  Имитация полиграфического растискивания. */
	void dotGain(int gRateLefts, int gRateRight, int gRateHighs, int gRateLower, int dot_gain);
	/**\brief  dot_gain!=0 число добавленных (-dot_gain убавленных) однопиксельных слоёв вокруг символа. */

	////
	/**\brief   Scale GBitmap content
  масштабирование (Resample) серого (char) массива по методу "ближайших соседей" */
	/** маштабирование изменяет содержимое исходного изображения */
	void scaleFast(float scale);
	void scaleFast(const GBitmap *ref, float scale);

	// Билинейное масштабирование изображения
	/** маштабирование изменяет содержимое исходного изображения */
	void bilinearScale(float scale);
	void bilinearScale(GBitmap *ref, float scale);

	// Бикубическое (bicubic) масштабирование (Resample) серого (uchar) массива */
	void bicubicGrayScale(float scale);
	void bicubicGrayScale(const GBitmap *ref, float scale);

	// Бикубическое (bicubic) масштабирование (Resample) цветного RGB массива
	void bicubicScale(float scale);
	void bicubicScale(const GBitmap *img, float scale);

	// Функция транспонирования (transpose). // отладочная функция, готова
	void transpose();

	// отладочная функция, не готова
	void filterHighM(int KHig);
	////

	// Функция определения угла наклона текста с помощью быстрого преобразования Радона.
	// Выполняет адаптивную бинаризацию binarisationF и затем запускает функцию detectPageAngle
	float detectPageAngleB(int g, int Tr, int BASE);

	/**\brief функция определения угла наклона текста с помощью быстрого
	преобразования Радона аналог GBitsetRotation.h
	g параметр задающий максимально возможный определяемый угол наклона текста,
	по умолчанию g=8, с увеличением максимадьного угола наклона текста g,
	пропорционально падает быстродействие t.
	g=8 angle<7gr, t=0.125sec; g=4 angle<14gr t=0.29sec;  g=2 angle<26gr t=0.8sec.
	Геометрический смысл параметра g это сумма пикселей по горизонтальной
	прямоугольной области размером 1 на 8 пикселей или 1 на 4 или 1 на 2 или 1 на 1.
   */

	// Функция отимизация исходного массива bytes_data для определения угла наклона текста.
	// Выполняет уменьшение горизонтального размера исходной картинки в "g". 4 апреля 2019 г.
	void optimizationW(int g);

	float detectPageAngle(int g);

	/**\brief функция быстрого преобразования Радона.*/
	void toolsRadon(int sign, uint *sharpness, uint w2, int g);

	/**\ Отладочная функция. Отимизация исходного массива bytes_data для определения
  угла наклона текста.*/
	void optimizationSkew(unsigned int g);

	/**\brief  Вращение или (и) масштабирование изображения копировением.*/
	void rotateFast(const GBitmap *ref, float scale, float rotation);
	/**\brief  Вращение или (и) масштабирование изображения c заменой исходных данных.*/
	void rotateFast(float angle);

	/** Поворот битовой серой картинки с билинеарным сглаживанием. Bilinear Rotate.*/
	void BilinearGrayRotate(double angle);
	/** Рассчитывает размер повернутого изображения. Возвращает новый размер изображения.*/
	void CalculateRotatedSize(double angle, int *new_width, int *new_height);

	// функция коррекции освещённости предназначена для выравнивания неравномерностей освещённости страници. (адаптивный
	// яркость)
	void brightnessCor(int Wr, int BASE);

	// Функция быстрого преобразования серого массива в битовый массив по фиксированному порогу 128. 17 марта 2019
	// Бинаризует по 8 байт одновременно.
	void binarisation64();
	/**\brief  базовая функция преобразования серого массива в битовый массив (адаптивный порог)
	Авторы = "DerekBradley and GerhardRoth"// www.derekbradley.ca / AdaptiveThresholding / index.html. */
	void binarisation(int Tr, int BASE);
	/**\brief функция преобразования серого массива в битовый массив, тонкие перетяжки бинаризует хуже всех
	и в 2-2,5 раза медленние базовой функции binarisation. Нет особых достоинств.*/
	void binarisationG(int Tr, int BASE);
	/**\brief быстрая функция преобразования серого массива в битовый массив (адаптивный порог), аккуратная и
	быстрее базовой функции binarisation в 1,5-2 раза. Использует фиксированный border. 2 февраля 2019  */
	void binarisationF(int Tr, int BASE);
	/**\brief Тоже самое что и binarisationF(), но может обрабатывать графические
	картинки в несколько раз большего размера чем binarisationF. 29 марта 2019. */
	void binarisationL(int Tr, int BASE);
	/**\brief Многопороговая функция преобразования серого массива в битовый массив (адаптивный порог), самая
	аккуратная, но медленнее базовой функции binarisation в 2-2,5 раза. Использует фиксированный border. 22 февраля
	2019. */
	float binarisationM(int Tr1, int Tr2, int BASE);
	// Параметр Tr1 управляет качеством начертания буквы (как в классической функции binarisation). Обычно 92-98.
	// Оптимально 98. Параметр Tr2 управляет качеством фона, позволяя минимизировать шумовые артефакты фона (мусор).
	// Обычно 76-92. Оптимально 92. Параметр BASE это размер скользящий области усреднения яркости в pix. Оптимально 14.
	// Возвращает среднею толщину штриха по всей исходной картинке в пикселях и долях пикселя (float).

	/// Функция стандартизации начертания текста путем определения фокальной линии(графемы) букв.
	// n_RemovLayers - количество удаленных слоев (слой это периметр символа) // number of removed layers
	// Вычисление матрицы (3*3-1) выполняется по предпросчитанным таблицам, что в 2 раза быстрее.
	int focalLine(int n_RemovLayers);
	// TMP
	/// Версия где расчет матрицы происходит в теле цикла.
	int focalLineT(int n_RemovLayers);
	/// Версия где расчет матрицы происходит в теле цикла.
	int focalLineT1(int n_RemovLayers);

	/**\brief print GBitmap on stdout in pseudo graphic*/
	void printToScreen();
	/**\brief print GBitmap on stdout in pseudo graphic in binary*/
	void printToScreenBinary();
	/**\brief Save GBitmap as TIFF file with GroupFax4 compression*/
	void saveTIFF(string &path);
	/**\brief Save GBitmap as TIFF file with GroupFax4 compression*/
	void saveTIFF(const char *path);

	void drawImg(GBitmap *img, int x0, int y0);
	void drawImg(GBitmap *img, int x0, int y0, int mode);

	/** заполняет выбранную область в GBitmap32 нулями или единицами */
	void fill32V(bool color, OCRBox *s);
	/** заполняет в GBitmap32 выбранные колонки +1 с каждой стороны  нулями или единицами */
	void fillColumns32V(bool color, OCRBox *s);

	/**\brief отображает данные GBitMask32 в транспонированном и сжатом GBitmap. описание см. unpackImg32V */
	void drawMask32V(void *mask32, int x0, int y0, OCRBox *s, int mode);
	/**\brief функция подсчета количества белых пикселов в заданной области в формате GBitmap32 */
	int pixelCount(OCRBox *s);
	/**\brief функция подсчета количества совпадений пикселов двух изображений в заданной области в формате GBitmap32 */
	uint img32Union(GBitmap *bImg, OCRBox *s);
	/**\brief функция подсчета количества совпадений пикселов изображения буквы и изображения фокальных линий строки
	 * букв в заданной области в формате GBitmap32. результат пересечения заносится в bImg*/
	uint img32UnionLine(GBitmap *bImg, OCRBox *s);
	void imgIntegralSum(GBitmap *bImg, vector<uint> &sum, OCRBox *s);
	void rotate(int count);
	void clearRegion(int x0, int y0, int w, int h);
	// функция нормализации массива bytes_data по размеру ncolumns кратным 32
	void crop32();
	// получает новый массив bytes_data с новым размером по горизонтали ncolumns кратным 32 и новым
	// data_size=ncolumns*nrows;

	void packImg(int invert);	///< функция упаковки битмапа в битовый массив
	void unpackImg(int invert); ///< функция распаковки битового массива в битмап
	string drawToTIFFString(
		int invert); ///< представление битового контента как strokn в кодировке файла TIFF GroupFax4

	///______Bicubic_function________________________________
	double cubicInterpolate(double p[4], double x);
	char cubicInterpolate(char p[4], double x);
	double bicubicInterpolate(double p[4][4], double x, double y);
	char bicubicInterpolate(char p[4][4], double x, double y);
	double tricubicInterpolate(double p[4][4][4], double x, double y, double z);
	double nCubicInterpolate(int n, double *p, double coordinates[]);

	void testPopCnt();

	/**/
};
} // namespace ocr
#endif
