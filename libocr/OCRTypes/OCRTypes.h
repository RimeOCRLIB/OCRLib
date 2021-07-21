#ifndef HEADER_OCRTYPES
#define HEADER_OCRTYPES
//__OCR CLASSES_____________________________________
#include "../config.h"
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;
namespace ocr {
    // ---------------------------------------------------------------------------------------------
    //  Basic data  // для функции определения характеристик процессора.
    // ---------------------------------------------------------------------------------------------
#define CB_64BIT
    
    typedef unsigned char uint8;
    typedef char int8;
    
    typedef unsigned short uint16;
    typedef short int16;
    
    typedef unsigned int uint32;
    typedef int int32;
    
    typedef unsigned long long uint64;
    typedef long long int64;
    
    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;
    typedef unsigned long ulong;
    typedef const char *cstr;
    typedef const char cchar;
    
#ifdef CB_64BIT
    typedef uint64 memint;
#else
    typedef uint32 memint;
#endif
    
    
enum SIMDType {
    SIMDUnknown = 0,
    NoSIMD,
    MMX,
    SSE,
    SSE2,
    SSE3,
    SSE41,
    SSE42,
    Altivec
};
enum CpuType { CpuTypeUnknown = 0, Intel, AMD };

struct SCpuInfo // добавлено CoresSysconf, Popcnt, LinuxKernel, KeySMP
{
    CpuType Type;  // !< Processor type
    SIMDType SIMD; // !< SIMD type supported by this procesor
    uint32
    Processors; // !< Number of physical processors, using the cpuid function
    uint32 Cores;   // !< Number of cores
    uint32 LogicalUnits; // !< e.g. hyper threading
    uint32 CoresSysconf; // !< Number of cores, using the sysconf function
    uint32 Popcnt; // !< поддержка процессором суммирования единиц в регистре
    string LinuxKernel; // !< версия ядра Linux в системе
    uint32 KeySMP; // описание смотри ниже по тексту
    
    SCpuInfo()
    : Type(CpuTypeUnknown), SIMD(SIMDUnknown), Processors(0), Cores(0),
    LogicalUnits(0), CoresSysconf(0), Popcnt(0), KeySMP(0)
    
    {}
};

// KeySMP //
// ключ (способ подсчета единиц в слайсах) для запуска функции setMatrix_Point в
// Matrix_Lion.h ASM // KeySMP=1 - запускаем функцию  ASM  Assembler MMX 64   в
// Matrix_Lion_Search.h (надо перенести в Matrix_Lion.h) KeySMP=2 - запускаем
// функцию  #ifdef ASM_OS64_SSE3   в Matrix_Lion.h   (надо назвать Assembler
// OS64_SSE3) KeySMP=3 - запускаем функцию  #ifdef ASM_OS64_SSE42_POPCNT   в
// Matrix_Lion.h  (надо назвать Assembler OS64_SSE42_POPCNT) CPU // KeySMP=4 -
// запускаем функцию  CPU универсальная «С» версия 32/64р   в Matrix_Lion.h
// KeySMP=5 - запускаем функцию  CPU "C" only 64   в Matrix_Lion.h
// KeySMP=6 - запускаем функцию  CPU "C" only 64 only POPCNT intrinsic   почти
// написана см main.cpp

// ---------------------------------------------------------------------------------------------

typedef struct OCRDataStruct {
    uchar d2;
    uchar d1;
    short d0;
} OCRData;

typedef struct OCRData32Struct {
    int data[32];
    short len;
} OCRData32;

typedef struct OCRIntDataStruct {
    int d1;
    int d0;
} OCRIntData;

typedef struct OCRPointStruct {
    short x;
    short y;
    short data;
    uchar status;
    uchar type;
} OCRPoint;
    
// -- A run of pixels with the same color
typedef struct pointLineStruct {
    int x;  // horizontal coordinate slice
    int y;  // vertical coordinate slice
    int x1; // first horizontal coordinate slice
    int x2; // last horizontal coordinate slice
}pointLine;

    
    
const short cLimit = 10;
const short deltaLimit = 13;
const short Radius = 15;
const short kShapeCassidy = 10;

class OCRPointFloat {
public:
    OCRPointFloat();
    OCRPointFloat(float x, float y);
    float x;
    float y;
    char status;
    char type;
    char stability;
    OCRPointFloat operator*(int n);
};



class drawDataRecord {
public:
    drawDataRecord();
    vector<int> color;
    vector<vector<OCRPointFloat> > data;
    short count;
};

class vectorOCR {
protected:
public:
    vectorOCR(void);
    virtual ~vectorOCR();
    void destroy(void);
    static vectorOCR *create(void) { return new vectorOCR; }
    OCRPointFloat vStart;
    OCRPointFloat vEnd;
    short length;
    string name;
    OCRPointFloat vCenter;
    // vector<OCRPointFloat>points;  //count of points in vector
    short matchSum;
    short vectorAngle;
    short vectorInd;
    short selectFlag;
    short centerDist;
    short vectorFlag;
    int contourIndex;
    int letterIndex;
    int objIndex;
    int pointCassidy;
    unsigned int allVectorCount;
    int allLetterCount;
};

class GString {
public:
    GString(void);
    GString(string &ref);
    char data[127];
    uchar len;
    void operator=(const GString ref);
    void operator=(const char *ref);
    void operator=(string &ref);
    bool operator==(const GString ref);
    bool operator==(const char *ref);
    bool operator==(string &ref);
    bool operator!=(const GString ref);
    bool operator!=(const char *ref);
    bool operator!=(string &ref);
    
    void operator+=(string &ref);
    void operator+(const char *ref);
    void operator+=(const char *ref);
    void operator+=(uint ref);
    void operator+=(int ref);
    
    int size() {
        return len;
    }
    const char *c_str() { return data; }
    char *operator[](int id);
    int find(const char *ref);
    int find(string &ref);
};

// ostream& operator << (ostream& os, GString& ref);

class byteMask {
public:
    byteMask();
    bool b1;
    bool b2;
    bool b3;
    bool b4;
    bool b5;
    bool b6;
    bool b7;
    bool b8;
    void setMask(char mask);
};

typedef struct OCRBoxStruct {
    short x0;
    short y0;
    short x1;
    short y1;
    short yLimit0; //координаты верхней полки буквы (верхняя точка буквы без
    //оглассовки)
    short yLimit1; //координаты нижней полки буквы
    ushort area;
    ushort status;
}OCRBox;

class OCRTextData {
public:
    OCRTextData();
    short middleH;
    short w;
    short h;
    short deltaX;
    short lineCount;
    short direction;
    short tLine[1024];
};

typedef struct dictKeyStruct {
    string key;
    string tibKey;
    string tib;
    string srcKey;
    string destKey;
    string dict;
    string ln;
    string grm;
    string context;
    int level; //релевантность ключа
    int flag;
    uint lenght;
    uint id;
} dictKey;

int operator==(const dictKey &left, const dictKey &right);
int operator<(const dictKey &left, const dictKey &right);

class OCRDictRecord {
public:
    OCRDictRecord();
    // char c[4048];
    // string tibKey;
    // string tibWord;
    // string engKey;
    //	string sncKey;
    // string rusKey;
    //	string plKey;
    //	string gmKey;
    // string chKey;
    // string esKey;
    string eentry;
    string rentry;
    string example;
    string exp;
    // string Wylie;
    // string rus_translit;
    string dict;
    string searchString;
    int recordFlag;
    map<string, dictKey> key;
    vector<dictKey> reportKey;
    int wordCount;
    int recID;
    int level; //релевантность записи
};

int operator==(const OCRDictRecord &left, const OCRDictRecord &right);
int operator<(const OCRDictRecord &left, const OCRDictRecord &right);

class GTextBufer {
public:
    GTextBufer();
    long size;
    char data;
};
ostream &operator<<(ostream &s, GTextBufer z);

/**
 main structure for all codepage conversion functions
 all data store in UTF-8 format
 */
typedef struct uniRecordStruct {
    int index;          ///< индекс буквы в OCRFont
    int letterID;       ///< ID номер буквы в OCRFont
    
    wstring uniYagpo;    // unicode Yagpo
    string uniTypingKey;     // letter typing order in the typing driver
    
    //string uniHexCode; // self Unicode in Hex in base Tibetan
    //string uniHexKey;  // unicode for typing in Hex
    string utfTibKey;          // UTF in the codepage of standart Uni driver
    //vector<string> uniHexTibetan; // unicode in Hex for Tibetan letter
    //vector<string> uniTibYagpo; // unicode in the codepage of standart Uni driver
    string OCRKey;             // self letter code in UTF
    //string OCRKeyHex;          // self letter code in hex
    string keyUTF;             // letter type order in not Unicode fonts
    string letterUniUTF;          // letter in Unicode codepage in UTF
    string letterYagpoUTF;        // letter in Yagpo codepage in UTF
    //string letterUniHex;
    string letterUTFLowerCase; // letter in UTF in lowerCase form
    string Wylie;
    string letterName;
    string translitRus;
    string translitEng;
    string lang;
    int stackFlag;
    
    int OCRCount;
    
} uniRecord;
    
//порядок полей в звписи uniRecordStruct в базе данных
enum iniRecordTString{
    REC_LANG=0,
    REC_LETTER_NAME=1,
    REC_YAGPO_UTF=2,
    REC_OCR_KEY=3,
    REC_KEY_UTF=4,
    REC_STACK_FLAG=5,
    REC_LETTER_UNI_UTF=6,
    REC_UNI_TYPE_KEY=7,
    REC_WYLIE=8,
    REC_OCR_COUNT=9,
    REC_TRANSLIT=10
};


typedef struct rootLetterOCRStruct {
    string letter;
    vector<short> lj;         // joined leter
    vector<short> lsj;        // subjoined leter
    vector<short> lssj;       // sub subjoined leter
    vector<short> lm;         // modificator
    vector<short> lbase;      // valid (2)letters
    vector<short> lbase1;     // valid (3)letters
    vector<short> lbase2;     // valid (4)letters
    vector<short> uniHexCode; // self Unicode in Hex
    vector<short> uniHexKey;  // unicode for typing in Hex
} rootLetterOCR;

typedef struct fontOCRMap {
    vector<uniRecord> fMap;
    string fontName;
} fontMap;

typedef struct stringMatch_ {
    int matchIndex;
    int startIndex;
    int endIndex;
    int endIndexSrc;
    int count;
    int lenSum;
} stringMatch;

typedef struct stringEntry_ {
    char *keyPtr;
    int keySize;
    char *strPtr;
    int strSize;
} stringEntry;

typedef struct hashRecord_ {
    unsigned int hash;
    unsigned int adress;
    bool operator<(const hashRecord_ c) const { return (this->hash < c.hash); }
    bool operator==(const hashRecord_ c) const { return (this->hash == c.hash); }
} hashRecord;

typedef struct pidIDStruct {
    int pid;
    int status;
} pidID;

typedef struct sData {
    int ID;
    int len;
    int memBase;
    string topic;
    int haveTopic;
    int inTopic;
    string path;
} sortData;
    

/*
 static bool sortSearchData(const sortData& d1, const sortData& d2){
 if(d1.haveTopic){
 if(d1.len > d2.len){
 if(d1.inTopic)return 1;
 }
 }
 return d1.len > d2.len;
 }
 */

enum stackFlag {
    VOWEL = 0,
    OCR_LETTER = 1,
    ROOT_LETTER = 1,
    ROOT_LETTER_VOWEL = 3,
    ROOT_LETTER_DOWN_VOWEL = 5,
    ROOT_LETTER_UP_DOWN_VOWEL = 6,
    STACK_2LETTER = 7,
    STACK_2LETTER_VOWEL = 9,
    STACK_2LETTER_DOWN_VOWEL = 11,
    STACK_2LETTER_UP_DOWN_VOWEL = 12,
    STACK_3LETTER = 13, //:)
    STACK_3LETTER_VOWEL = 15,
    STACK_3LETTER_DOWN_VOWEL = 17,
    STACK_3LETTER_UP_DOWN_VOWEL = 19,
    STACK_4LETTER = 21,
    STACK_4LETTER_VOWEL = 23,
    STACK_4LETTER_DOWN_VOWEL = 25,
    STACK_4LETTER_UP_DOWN_VOWEL = 27,
    letterKey = 0,
    
    LETTER_MANUSCRIPT=0,
    LETTER_TIB_MANUSCRIPT=100,
    LETTER_TIB = 101,
    LETTER_BOTTOM_VOWEL=103,
    LETTER_LEFT_PUNCTUATION=104,
    LETTER_PUNCTUATION=105,
    LETTER_RIGHT_PUNCTUATION=106,
    LETTER_UPPER_RIGHT_PUNCTUATION=107,
    LETTER_UPPER_VOWEL=109,
    LETTER_PHRASE_PUNCTUATION=110,
    LETTER_PUNCTUATION_ALONE=111,
    
    JOIN_LEFT=0,
    JOIN_RIGHT=1

    
};

//константы для функций обработки изображений
enum drawConst {
    IMGFLIP = 1,
    IMGNOFLIP = 0,
    IMGRGB = 1,
    IMGBW = 0,
    cRGB = 4,
    ALL_LETTER = 1,
    TEST_LETTER = 2,
    letterID = 3,
    GET_LETTERS = 4,
    VECTOR_OCR = 5,
    BIGMASK = 10,
    MIDDLEMASK = 9,
    SMALLMASK = 8,
    GIF = 1,
    JPEG = 2,
    LETTER_ADD = 1,
    DRAW_BASE = 0,
    DRAW_BLACK = 1,
    PLETTER = 0,
    WLETTER = 1,
    BITMAP_32 = 32,
    ADD_MODE = 1,
    XOR_MODE = 2,
    OR_MODE = 3,
    MATRIX_BORDER = 128,
};

//константы распознавания изображения
enum graphConst {
    
    Z_POINT = 0,        ///< точка фокальной линии нулевой длины
    L_POINT = 1,        ///< конечные точки линий
    P_POINT = 2,        ///< точки экстремума кривой и неустойчивые T и X точки
    T_POINT = 3,        ///< точки Т-образных пересечений линий
    X_POINT = 4,        ///< точки Х-образных пересечений линий
    E_POINT = 5,        ///< точки максимума кривизны кривой
    I_POINT = 6,        ///< точки перегиба кривой
    S_POINT = 7,        ///< точка фокальной линии лимитированной длины
    O_POINT = 10,       ///< круги
    
    //типы линий кластера
    BASE_LINE = 1,      ///< базовая линия в которой проводится поиск точек экстремума
    EXTREM_LINE = 2,    ///< кривая Bezier между точками экстремума
    NEW_LINE = 3,       ///< относительно прямая линия между точками экстремума
    CIRCLE_LINE = 4,    ///< линия принадлежит кругу
    CLUSTER_LINE = 9,    ///< линия присоеденина к переферийной OCRStar кластера
                        ///< и не имеет OCRStar на другом конце
    //типы формы линий
    SERIF = 0,          ///< короткая прямая
    LINE = 1,           ///< прямые
    L_CURVE = 2,        ///< линия выгнута влево
    R_CURVE = 3,        ///< линия выгнута вправо
    SL_CURVE = 4,       ///< S-кривая с первым выгибом влево
    SR_CURVE = 5,       ///< S-кривая с первым выгибом вправо
    N_CURVE = 6         ///< кривая с несколькими экстремумами
    
};

enum {
    ARC = 2,
    SPIRAL = 3,
    CIRCLE = 4,
};

//константы для функций передачи данных

enum dataConst {
    CLOSE_SOCKET = 0,
    INIT_SOCKET = 1,
    TIB_TO_ENG_UTF = 2,
    TIB_FROM_HASH = 3,
    REMAP_STRING_TIB = 4,
    HASH_FIND = 5,
    STRING_FIND = 6,
    LOCAL_MODE = 7,
    SOCKET_MODE = 8,
    IMPORT_TO_DB = 9,
    EXPORT_FROM_DB = 10,
    READ_FROM_DB = 11,
    READ_FROM_BIN = 12,
    READ_FROM_TAB = 13,
    READ_FROM_XML = 14,
    READ_FROM_TXT = 15,
    MEMORY_LOCATION = 16,
    MMAP_LOCATION = 17,
    CORPUS_HTML = 18,
    HTML = 19,
    LETTER_TABLE_HTML = 20,
    CHAT = 21,
    ADD_TO_GMAP = 22,
    REPLACE_IN_GMAP = 23,
    SINGLE_LINE = 24,
    MULTY_LINE = 25,
    TRANSLATE = 1,
    DICT_REPORT_TEXT = 2,
    DICT_REPORT_USER = 3,
    FULL_REPORT = 4,
    MAIN_REPORT = 1,
    TEXT_REPORT = 3,
    OCR_NORMALISATION = 100,
    OCR_IMAGE_PARAMETERS = 101,
    
};

///константы языков распознавания.
enum OCRConst {
    OCR_TIBETAN = 1,
    OCR_TIBETAN_ENG = 2,
    OCR_TIBETAN_RUS = 3,
    OCR_TIBETAN_SKT = 4,
    OCR_SANSKRIT = 5,
    OCR_SANSKRIT_ENG = 6,
    OCR_KANNADA = 7,
    OCR_SINHALA = 8,
    OCR_SINHALA_RUS = 9,
    OCR_SINHALA_ENG = 10,
    REMOVE_DELIMETERS = 11,
    NO_REMOVE_DELIMETERS = 12,
    OCR_DICT_HASH = 13,
    TRANSLATE_ENG = 14,
    TRANSLATE_RUS = 15,
    
    OCR_FONT_CORRELATION=101,
    OCR_TEXT_CORRELATION=103
    
    
    
};

/// search mode for database

enum searchModeConst {
    TEXT_SEARCH = 0,
    OCR_SEARCH = 2,
    HASH_SEARCH = 1,
    LIST_SEARCH = 5,
    FULL_TEXT_SEARCH = 6,
    STRING_SEARCH = 8,
    ID_SEARCH = 9,
    ID_SEARCH_DATA = 10,
    LETTER_SEARCH = 11,
    INDEX_COUNT = 10,
    ALL_FIELD = 1024,
    TEXT_DICT_SEARCH = 12
};
    
    
enum GUI_request {
    GET_DB_RECORD=100,
    GET_DB_RECORD_COUNT=101,
    RELEASE_DB_LOCK=108,
    SET_DB_LOCK = 109,
    
    RENDER_SEARCH=1,
    RENDER_TRANSLATE_RECORD=2,
    RENDER_READ=3,
    RENDER_DICT_REPORT=4,
    RENDER_DICT_SAVE=5,
    RENDER_TRANSLATE_PAGE=6,
    RENDER_TRANSLATE_LINE=7,
    RENDER_REPORT=8,
    RENDER_TEXT_SEARCH=9,
    RENDER_CATEGORY=10,
    RENDER_CATEGORY_LIST=11,
    RENDER_TITLE_SEARCH=12,
    RENDER_TEXT_SEARCH_NEW=13,
    RENDER_RECORD_CATEGORY=14,
    RENDER_SEARCH_DB_TABLE=15,
    
    //OCR
    RENDER_LETTER_BY_ID=200,
    RENDER_IMAGE_PROCESS=201,
    BATCH_OCR = 202,
    
    //type of request render
    RENDER_QUERY = 1024,
    RENDER_JSON = 1025,
    RENDER_DICT = 1026,
    
    //mode of execution
    READ_TEXT_WRITE_TEXT=1064,
    READ_PATH_WRITE_FILE=1065,
    
    //LogicProcessor function
    CONVERT_UNI_TO_YAGPO=2001,
    CONVERT_YAGPO_TO_UNI=2002,
    CONVERT_WYLIE_TO_UNI=2003,
    CONVERT_UNI_TO_WYLIE=2004,
    CONVERT_UNI_TO_TRANSCRIPTION_RUS=2005,
    CONVERT_UNI_TO_TRANSCRIPTION_ENG=2006
    
};
    


//////////////////////// ALGORITM ///////////////////////////
} // namespace ocr

#endif //#ifndef HEADER_OCRTYPES
