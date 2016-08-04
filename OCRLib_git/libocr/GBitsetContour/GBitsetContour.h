
#ifndef _GBitsetContour_H_
#define _GBitsetContour_H_
#include "config.h"

#include "GBitmap.h"
#include "OCRTypes.h"
#include "GLetter.h"
#include "GFont.h"
#include "GContour.h"

#ifdef PC
  #define DRAW1  /// аналоговое рисование
#endif

#include <vector>
#ifdef MAC
// #include <bvector.h>
#endif
#include <string>
#include <algorithm>
#include <fstream>
#include <math.h>


using namespace std;


namespace ocr {
	
/** \brief Класс распознавания контуров и их векторизации. 
Распознавание контуров использует "chain" алгоритм. 
Векторизация осуществляется сглаживанием контура импульсными фильтрами и выделением точек смены направления. 
Результирующие вектора также фильтруются по длинне и величине угла*/
class GBitsetContour{
protected:
  GBitsetContour(void);
  GBitsetContour(int ncolumns, int nrows);
  GBitsetContour(const GBitsetContour *ref);
  GBitsetContour(const GBitmap *ref, const GBitmap *ref1);
  GBitsetContour(const GBitmap *ref, short stepW=1, short stepH=1,short flipV=0);
public:
  virtual ~GBitsetContour();
  void destroy(void);
  /** @name Construction. */
  //@{
  /** \brief Constructs an empty GBitsetContour object.  The returned GBitsetContour has zero rows
	  and zero columns.  Use function \Ref{init} to change the size of the
	  image. */
  static GBitsetContour* create(void) {return new GBitsetContour;}

  /** \brief Constructs a GBitsetContour with #nrows# rows and #ncolumns# columns.  All
	  pixels are initialized to white. The number of gray levels is initially set to #2#.  */
  static GBitsetContour* create(const int nrows, const int ncolumns)
  {return new GBitsetContour(ncolumns,nrows);}

  /** \brief Copy constructor. Constructs a GBitsetContour by replicating the size and the contents of GBitsetContour #ref#. */
  static GBitsetContour* create(const GBitsetContour *ref)
  {return new GBitsetContour(ref);}

  /** Copy constructor. Constructs a GBitsetContour by replicating the size and the contents of every step pixel of GBitmap #ref#. stepH, stepW is step for reduce image size   */
  static GBitsetContour* createResize(const GBitmap *ref, short stepW, short stepH, short flipV)
  {return new GBitsetContour(ref,stepW,stepH,flipV);}

  /** \brief Resets this GBitsetContour size to #nrows# rows and #ncolumns# columns and sets all pixels to white. The  number of gray levels is initialized to #2#. */
  void init(int nrows, int ncolumns);
  /** \brief Initializes this GBitsetContour with the contents of the GBitsetContour #ref#. */
  void init(const GBitsetContour *ref);
  /** \brief Initializes this GBitsetContour with the contents every step pixel of the GBitsetContour #ref#. stepH, stepW is step for reduce image size */
  void init(const GBitmap *ref, short stepW=1, short stepH=1, short flipV=0);


  void fill(bool value);

  /** @name Accessing the pixels. */
  //@{
  /** \brief Returns the number of rows (the image height). */
  unsigned int rows() const;
  /** \brief Returns the number of columns (the image width). */
  unsigned int columns() const;
  /** \brief Returns a constant pointer to the first byte of row #row#.This pointer can be used as an array to read the columns elements in row. */
  const unsigned char *operator[] (int row) const;
  /** Returns a pointer to the first byte of row #row#. This pointer can be used as an array to read or write the row elements. */
  unsigned char *operator[] (int row);
  /** Returns the size of a row in memory (in pixels).*/
  unsigned int rowsize() const;

  GBitsetContour* rotate(int count=0);  ///(deprecated)
  //@}


protected:
  // bitmap components
  int nrows;             ///<высота битмэпа
  int ncolumns;          ///<ширина битмэпа
  int bytes_per_row;     ///<количество байтов в строке 
  int stepBit;
  int srcW;
  int srcH;
  int border;  
public:
  unsigned char  *bytes_data;      ///<основной байтовый буфер данных битмэпа
  int data_size;                   ///<размер основного байтового буфера данных битмэпа
  int textLineSize;	               ///<установочный размер строки текста. Используется для определения параметров векторизации. text line size for set the parameters of vectorisation	
  ofstream c_out;                  ///<отладочный вывод в файл
#ifdef DEBUGLVL
  GBitmap* destData;               ///<отладочный графический массив
  #ifdef UNIX
  drawDataRecord drawGraph;        ///<отладочный графический массив
  #endif
#endif
private:


public:
//float Alpha;
  int deltaX;
  int lineCount;                ///<количество строк в тексте
#ifdef DEBUGLVL_GREY
  GBitmap* drawData;              ///<отладочный графический массив
#endif
  GBitmap* line;                  ///<GBitmap в который записывается вся промежуточная информация в процессе выделения контуров. (Контур, заливка, концы контуров)
  char baseDirection;            ///<направление контура (по и против часовой стрелки)
  vector<stringOCR>*strArray;     ///<массив информации о координатах строк и букв. Используется для передачи результатов распознавания между классами
  int vectorCount;       ///<общее количество векторов в тексте

	/** @name _________________________OCR Function_____________________________________. */
	//@{
	///инициализация внутренних данных и запуск векторизации текста
	void setData(vectorOCR *vectorBase_,vector<stringOCR>&strArray_, int iLeft_, int iTop_); //set actual data in object
	///инициализация внутренних данных и запуск векторизации буквы на основе шрифта
	void setFontData(vectorOCR *vectorBase_, GLetter *glyph, string &recordName_,  short wScale);
    ///инициализация внутренних данных и создание контурной базы
	void buildGFont(GFont *contourBase,vector<stringOCR>&strArray_);
	///визуализация контурной информации в отладочных массивах
	void drawContour();
	//@}


private:
	short max_down, startLetter, startLine, contourCount, contourIndex, indexFilter;
	short res, xBegin, xEnd, xL,yL,hL, wL, dy_line, y_line, yLine, *ln,*lt;
	short xMax,xMin,yMin,yMax, XMCenter, YMCenter, startDir;
	short dlt_y, t, d, xl, yl, n,m;
	int iLeft,iTop;
	short *lineCoordinate, endContour;
	OCRTextData* textOCRData; //all OCR data about current pecha
	vectorOCR *vectorBase;
	short yUp, yDown;
	int i;
	short *p3;
	unsigned char *p1, *p2;
	int w,h,a, resX, resY;
	bool inBit[10];
	bool *tPoint;
	string recordName;

    short vectorLimit;                    ///<inner constan for set vectorisation parameters
    char ang[20000],angCalibrate[20000];
    int angData[20000],angX[20000],angY[20000];
    char angDiff[20000],angInt[20000],angDiffSum[20000],angDiffVector[20000],angFilterBase[20000];
    char angIntSum[20000], angIntVector[20000];
    int vectorMarker[20000];
    char OCRZone[20000];
    char *letterDataId;
    unsigned char *lineData ;
    int filterCount[300];
    short filterIndex;
    short angCount,lenCount,angLength;
    int sqrt_buff[16000];         ///<cash of sqrt function
    int sizeH;
	
	/** @name _________________________Contour detection_____________________________________. */
	//@{
	///found first point of letter's contour implemetnation of all method
	void baseOCR(); 
	///read pecha and set image handler set bitmap array from Bitmap
	void setImageHandler();  
	///read outer and inner contiur of letter from bitmap
	void readOuterContour(int vL);
	///fill contour
	void fillContour(GContour* contour);
    ///set center of letter and test vectors pair angle
	void calibrateAngle();
	///set andle difference array ablolute angle. Now we free from 0-360 border
	void angleDiff();  
	///filter angle difference array by impulse filter. remove all noise and small contour deformation - this create vectors in letter
	void baseFilter();  
	/// impulse filter implementation
	void impulseFilter(char *angFilter); 
	///(deptecated)
	void setOCRZone();
	int impFilter7(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 7 pixel contour filter
	int impFilter6(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 6 pixel contour filter
	int impFilter5(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 5 pixel contour filter
	int impFilter4(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 4 pixel contour filter
	int impFilter3(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 3 pixel contour filter
	int impFilter2(char *filterStr, char *aFt, short angLen);  ///< impFilter function for 2 pixel contour filter
	void rootFilter(char *aFt, short angLen);                  ///< impFilter function for 3 pixel contour filter
	//@}
	
	/** @name _________________________set vectors in letter and set letter records in base_____________________________________. */
	//@{
	//set vectors and write coordinates in two base - letters and vector
	void setVector(short  contour);   
	void setRecordInArray(char contour,short vCount);  //set record in vectorBase
	void setVectorRecord(); //set vectors in vector OCR base
	// create letter from user input. set all record for that new letter
	//void createFilter:(vector<GP<vectorOCR> >&vectorBase, (NSString *)srcString :(NSString *)filterString;
	short setVectorAngle(short x0, short y0, short x1, short y1);
	
	void readVSum(); //old one method
	void set_sqrt_array();
    int sqrtA(int data);
    void setSize();
    //@}
	
	///////////////////////**draw methods
	//void)drawGraph(unsigned char *graphData_);
	//-(NSImage *)drawMainOCRView:(vector<GP<GLetter> >&)vectorBase;
	//-(NSImage *)drawAllOCRLetter;
	OCRPoint CreatePoint(short x, short y);

};


// ---------------- IMPLEMENTATION

inline unsigned int
GBitsetContour::rows() const
{
  return nrows;
}

inline unsigned int
GBitsetContour::columns() const
{
  return ncolumns;
}



inline unsigned int
GBitsetContour::rowsize() const
{
  return bytes_per_row;
}


inline unsigned char *
GBitsetContour::operator[](int row)
{
  if (row<0 || row>=nrows) {
	return bytes_data;
  }
  return &bytes_data[row * bytes_per_row];
}

inline const unsigned char *
GBitsetContour::operator[](int row) const
{
  if (row<0 || row>=nrows) {
	return bytes_data;
  }
  return &bytes_data[row * bytes_per_row];
}

//inline GBitsetContour&
//GBitsetContour::operator=(const GBitsetContour &ref)
//{
//  init(ref, ref.border);
//  return *this;
//}

// ---------------- THE END

}

#endif


