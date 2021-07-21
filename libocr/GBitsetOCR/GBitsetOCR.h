#ifndef HEADER_GBITSETOCR
#define HEADER_GBITSETOCR

#include "../config.h"
#include "../GBitmap/GBitmap.h"
#include "../GLogicProcessor/GLogicProcessor.h"
#include "../GBitsetMatrix/GBitsetMatrix.h"
#include "../OCRTypes/OCRTypes.h"
#include "../GFontEditor/GFont.h"
#include "../OCRString/php2stl.h"
#include "../GImageEditor/GImageEditor.h"



#ifdef WIN32_DRAW
#define DRAW1  //PC_DRAW
#endif

#include <vector>
#ifdef MAC
//#include <bvector.h>
#include <sys/time.h>
#endif
#include <string>
#include <list>
#include <algorithm>
#include <fstream>


using namespace std;

namespace ocr{
///класс битовой обработки битмэпа. Содержит базовые методы распознавания строк и макета страницы.
class GBitsetOCR{
protected:
    GBitsetOCR(int nrows, int ncolumns, int border=0);
    GBitsetOCR(const GBitsetOCR *ref);
    GBitsetOCR(const GBitmap *ref, short stepW=1, short stepH=1);
    GBitsetOCR(const GBitsetOCR *ref, int border);
public:
    void destroy(void);
    /** @name Construction. */
    //@{
    
    /** Constructs a GBitsetOCR with #nrows# rows and #ncolumns# columns.  All
     pixels are initialized to white. The optional argument #border#
     specifies the size of the optional border of white pixels surrounding
     the image.  The number of gray levels is initially set to #2#.  */
    static GBitsetOCR* create(const int nrows, const int ncolumns, const int border=0)
    {return new GBitsetOCR(nrows,ncolumns, border); }
    
    /** Copy constructor. Constructs a GBitsetOCR by replicating the size, the
     border and the contents of GBitsetOCR #ref#. */
    static GBitsetOCR* create(const GBitsetOCR *ref)
    {return new GBitsetOCR(ref);}
    
    /** Copy constructor. Constructs a GBitsetOCR by replicating the size, the
     border and the contents of GBitmap #ref#. */
    static GBitsetOCR* createResize(const GBitmap *ref, short stepW, short stepH)
    {return new GBitsetOCR(ref,stepW,stepH);}
    
    // static GBitsetOCR* createLevel(const GBitmap &ref)
    // {return new GBitsetOCR(ref, ref);}
    
    /** Constructs a GBitsetOCR by copying the contents of GBitsetOCR #ref#.
     Argument #border# specifies the width of the optional border. */
    static GBitsetOCR* create(const GBitsetOCR *ref, const int border)
    { return new GBitsetOCR(ref,border); }
    
    //@}
    
    /** @name Initialization. */
    //@{
    /** Resets this GBitsetOCR size to #nrows# rows and #ncolumns# columns and sets
     all pixels to white.  The optional argument #border# specifies the size
     of the optional border of white pixels surrounding the image.  The
     number of gray levels is initialized to #2#. */
    void init(int nrows, int ncolumns, int border=0);
    /** Initializes this GBitsetOCR with the contents of the GBitsetOCR #ref#.  The
     optional argument #border# specifies the size of the optional border of
     white pixels surrounding the image. */
    void init(const GBitsetOCR *ref, int border=0);
    /** Initializes this GBitsetOCR with the contents of the GBitmap #ref#. stepH, stepW
     is step for reduce image size */
    void init(const GBitmap *ref, short stepW=1, short stepH=1);
    /** Initializes this GBitsetOCR with the contents of the GBitsetOCR #ref#.
     initLevel use adaptive treshould algorithm is*/
    void initLevel(const GBitmap *ref);
    /** Initializes all the GBitsetOCR pixels to value #value#. */
    void fill(bool value);
    //@}
    
    /** @name Accessing the pixels. */
    //@{
    /** Returns the number of rows (the image height). */
    unsigned int rows() const;
    /** Returns the number of columns (the image width). */
    unsigned int columns() const;
    /** Returns a constant pointer to the first byte of row #row#.
     This pointer can be used as an array to read the row elements. */
    const bool *operator[] (int row) const;
    /** Returns a pointer to the first byte of row #row#.
     This pointer can be used as an array to read or write the row elements. */
    bool *operator[] (int row);
    /** Returns the size of a row in memory (in pixels).  This number is equal
     to the difference between pointers to pixels located in the same column
     in consecutive rows.  This difference can be larger than the number of
     columns in the image. */
    unsigned int rowsize() const;
    /** Makes sure that the border is at least #minimum# pixels large.  This
     function does nothing it the border width is already larger than
     #minimum#.  Otherwise it reorganizes the data in order to provide a
     border of #minimum# pixels. */
    void minborder(int minimum);
    //@}
    
    
    
protected:
    // bitmap components
    unsigned short nrows;
    unsigned short ncolumns;
    unsigned short border;
    unsigned short bits_per_row;
    bool  *bits;
    short stepBit;
    short srcW;
    short srcH;
    int data_size;
    ofstream c_out;
private:
    vectorOCR *vectorBase;
    GFont *aliKali;
    vector<stringOCR>*strArray;
    
public:
    bool  *bits_data;
    
#ifndef NDEBUG
    void check_border() const;
#endif
    /// float Alpha;
#ifdef DEBUGLVL_GREY
    GBitmap* drawData;           //bitmap for drawing
#endif
    const GBitmap  *inputBitmap, *GFontImg;               //actual bitmap pointer for input data
    GBitsetMatrix *setMatrix;     //matrix object with string bitmap data for string by string letter recognition
    int sIndex;   //current string
    int wIndex;   //current word
    short baseDirection;
    int iLeft, iTop, letterWsum, letterHsum,proportion;
    GLetter *glyph;
    vectorOCR *vDest,*vSrc; //pointer on array of vector in test and model letter;
    wordOCR *wordSrc;
    string mainString;
    string xmlString;
    string fileName;
    int bitsetBorder;
    vector<OCRMatch>textMatch;
    float pageScale;
    
    //_________________________OCR Function_____________________________________
    
    void setData(GFont *aliKali_,
                 vector<stringOCR>&strArray_,
                 vector<stringOCR>&correctionTable, //vector of all uncertan letters from OCR text for draw in correction table
                 GLogicProcessor *logicProcessor,   //dictionary controll object
                 int iLeft_,
                 int iTop_,
                 int border,
                 int mode); //set actual data in object
    void setTestOCRCorrelation(int sIndex_, int wIndex_, int mIndex); //set data for test correlation
    
    ///основная функция графического распознавания. К выделенной строке применяется поиск масштаба, графическое распознавание букв,
    ///применение правил шрифтовой грамматики. Результатом функции является массив pageText котрый содержит гипотезы букв для грамматического корректора.
    void pageOCR(GLogicProcessor *logicProcessor, int mode);
    void textOCR(vector<stringOCR>&correctionTable,GLogicProcessor *logicProcessor,int mode);
    
    void textOCR_letter(int mode);
    int readFromCorrectionTable(vector<stringOCR>&correctionTable,vector<wordOCR>&pageText,int index); ///read string from human prof reading pages.
    short setScaleRecord();
    int  lineOCR(GBitmap *line, vector<OCRMatch>&matchLine,int yLimit0, int yLimit1, float scale_,int index,int limit);
    
    //функция определнния масштаба страницы на основаннии распознавания тестовых букв
    float setScaleBitsetOCR(GLogicProcessor *logicProcessor,int mode);
    //функция выполнения определения параметров графической обработки страницы.
    int setImageParamOCR(GBitmap *pechaImg_,vector<float*>&param,OCRPointFloat &point);
    
    float lineOCRScaleDetector(GBitmap *line,
                               vector<OCRMatch>&matchLine,
                               int yLimit0,
                               int yLimit1,
                               float scale,
                               int index,
                               int *betterCorrelation,
                               int limit);
    
    //////////////////////////**letter recognition methods
    void setOCRCorrelation(int stackFlag,int mode); //SET CORRELATION FOR ALL RECORDS IN BASE
    
    void  setOCRRecordCorrelation ();  //SET CORRELATION FOR ONE WORD
    int   setOCRLetterCorrelation(int letterFlag, int matchNum,int mode); //SET CORRELATION FOR PART OF THE WORD
    //short setMatchCircle(short len, int L3, short x1, short y1, short x0,  short y0,  short x2,  short y2);
    //short setMatchCassidy(short len, int L3, short x1, short y1, short x3,  short y3, short x0, short y0, short x2, short y2);
    
    //short testCassidyMatch(OCRMatch *matchPoint,OCRMatch *matchOCR);
    //short setCassidyCorrelation (OCRMatch *matchOCR);
    
    static bool sortMatchXCenter(const OCRMatch& d1, const OCRMatch& d2){
        return d1.xCenter < d2.xCenter;
    }
    static bool sortMatchX0(const OCRMatch& d1, const OCRMatch& d2){
        return d1.xCenter-d1.letterW/2 < d2.xCenter-d2.letterW/2;
    }
    static bool sortKeyC(const keyOCR& d1, const keyOCR& d2){
        return d1.correlation > d2.correlation;
    }
    static bool sortMatchC(const OCRMatch& d1, const OCRMatch& d2){
        return d1.correlation > d2.correlation;
    }
    static bool sortMatchL(const OCRMatch& d1, const OCRMatch& d2){
        return d1.line.size() > d2.line.size();
    }
    static bool sortMatchW(const OCRMatch& d1, const OCRMatch& d2){
        return d1.letterW > d2.letterW;
    }
    
    
    //
};


// ---------------- IMPLEMENTATION

inline unsigned int
GBitsetOCR::rows() const
{
    return nrows;
}

inline unsigned int
GBitsetOCR::columns() const
{
    return ncolumns;
}

inline unsigned int
GBitsetOCR::rowsize() const
{
    return bits_per_row;
}

inline bool *
GBitsetOCR::operator[](int row)
{
    if (row<0 || row>=nrows) {
        return bits_data;
    }
    return &bits_data[row * bits_per_row + border];
}

// ---------------- THE END
}
using namespace ocr;


#endif //#ifndef HEADER_GBITSETOCR

