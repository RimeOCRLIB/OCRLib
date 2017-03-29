#ifndef HEADER_OCRTYPES
#define HEADER_OCRTYPES
//__OCR CLASSES_____________________________________
#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>

using namespace std;
namespace ocr {

    
    // ---------------------------------------------------------------------------------------------
    //  Basic data  // для функции определения характеристик процессора.
    // ---------------------------------------------------------------------------------------------
#define CB_64BIT
    
    typedef unsigned char		uint8;
    typedef char                int8;
    
    typedef unsigned short		uint16;
    typedef short               int16;
    
    typedef unsigned int		uint32;
    typedef int                 int32;

    typedef unsigned long long	uint64;
    typedef long long           int64;
    
    typedef unsigned char		uchar;
    typedef unsigned short		ushort;
    typedef unsigned int		uint;
    typedef unsigned long		ulong;
    typedef const char*			cstr;
    
#ifdef CB_64BIT
    typedef uint64			memint;
#else
    typedef uint32			memint;
#endif
    
    
    enum SIMDType		{ SIMDUnknown=0, NoSIMD, MMX, SSE, SSE2, SSE3, SSE41, SSE42, Altivec }; 
    enum CpuType		{ CpuTypeUnknown=0, Intel, AMD };
    
    struct SCpuInfo     // добавлено CoresSysconf, Popcnt, LinuxKernel, KeySMP
    {
        CpuType         Type;				// !< Processor type
        SIMDType        SIMD;				// !< SIMD type supported by this procesor
        uint32          Processors;			// !< Number of physical processors, using the cpuid function
        uint32          Cores;				// !< Number of cores
        uint32          LogicalUnits;		// !< e.g. hyper threading
        uint32          CoresSysconf;		// !< Number of cores, using the sysconf function
        uint32          Popcnt;             // !< поддержка процессором суммирования единиц в регистре
        string          LinuxKernel;        // !< версия ядра Linux в системе
        uint32          KeySMP;             // описание смотри ниже по тексту
        
        SCpuInfo()
        : Type(CpuTypeUnknown)
        , SIMD(SIMDUnknown)
        , Processors(0)
        , Cores(0)
        , LogicalUnits(0)
        , CoresSysconf(0)
        , Popcnt(0)
        , KeySMP(0)
        
        {}
    };
    
    // KeySMP //
    // ключ (способ подсчета единиц в слайсах) для запуска функции setMatrix_Point в  Matrix_Lion.h
    // ASM //
    // KeySMP=1 - запускаем функцию  ASM  Assembler MMX 64   в Matrix_Lion_Search.h (надо перенести в Matrix_Lion.h)
    // KeySMP=2 - запускаем функцию  #ifdef ASM_OS64_SSE3   в Matrix_Lion.h   (надо назвать Assembler OS64_SSE3)
    // KeySMP=3 - запускаем функцию  #ifdef ASM_OS64_SSE42_POPCNT   в Matrix_Lion.h  (надо назвать Assembler OS64_SSE42_POPCNT)
    // CPU //
    // KeySMP=4 - запускаем функцию  CPU универсальная ™С™ версия 32/64р   в Matrix_Lion.h
    // KeySMP=5 - запускаем функцию  CPU "C" only 64   в Matrix_Lion.h
    // KeySMP=6 - запускаем функцию  CPU "C" only 64 only POPCNT intrinsic   почти написана см main.cpp
    
    // ---------------------------------------------------------------------------------------------
    
    class byteMask{
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
    

	const short cLimit=10;
	const short deltaLimit=13;
	const short Radius=15;
	const short kShapeCassidy=10;
    
    class OCRPoint {
    public:
        OCRPoint();
        OCRPoint(float x,float y);
        float x;
        float y;
        char status;
        char type;
        char stability;
        OCRPoint operator*(int n);
    };

    
    // -- A run of pixels with the same color
    struct pointLine{
        int y;       // vertical coordinate slice
        int x1;      // first horizontal coordinate slice
        int x2;      // last horizontal coordinate slice
    };
    
    class OCRBox {
	public:
		OCRBox();
		short x0;
		short y0;
        short x1;
		short y1;
        short yLimit0;   //координаты верхней полки буквы (верхняя точка буквы без оглассовки)
        uint area;
		char status;
        void setBorder(OCRBox *a,int border);
        void printBox();
	};
   
    
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
	
	struct dictKey {
		string key;
		string tibKey;
		string tib;
		string srcKey;
		string destKey;
		string dict;
		string ln;
		string grm;
		string context;
		int level;  //релевантность ключа
		int flag;
		uint lenght;
		uint id;
	};
    
    int operator==(const dictKey &left, const dictKey &right);
    int operator<(const dictKey &left, const dictKey &right);


	class OCRDictRecord {
	public:
		OCRDictRecord();
		//char c[4048];
		//string tibKey;
		//string tibWord;
		//string engKey;
	   //	string sncKey;
		//string rusKey;
	   //	string plKey;
	  //	string gmKey;
	          //string chKey;
	          //string esKey;
		string eentry;
		string rentry;
		string example;
		string exp;
		//string Wylie;
		//string rus_translit;
		string dict;
		string searchString;
		int recordFlag;
		map<string,dictKey>key;
		vector <dictKey>reportKey;
		int wordCount;
		int recID;
        int level;  //релевантность записи
	};
    
    int operator==(const OCRDictRecord &left, const OCRDictRecord &right);
    int operator<(const OCRDictRecord &left, const OCRDictRecord &right);


	
class GTextBufer {
public:
		GTextBufer();
		long size;
		char data;
};
ostream&operator<<(ostream& s, GTextBufer z);
	
	/**
	 main structure for all codepage conversion functions
	 all data store in UTF-8 format
	 */
	typedef struct uniLetterRecord{
		map<string,string> key; //main map for store all string value
		vector<short> uniKey;  // unicode for typing
		vector<short> uniYagpo;  // unicode Yagpo
		vector<string> uniHexCode; //self Unicode in Hex in base Tibetan
		vector<string> uniHexKey;  // unicode for typing in Hex
		string utfTibKey;   //UTF in the codepage of standart Uni driver
		string utfYagpoTibKey;   //letter typing order in the Yagpo typing driver
		vector<string> uniHexTibetan;  // unicode in Hex for Tibetan letter
		vector<string> uniTibYagpo;   //unicode in the codepage of standart Uni driver
		string Wylie;
		string OCRKey; //self letter code in UTF
		string OCRKeyHex; //self letter code in hex
		string letterUTF;  //letter in Yagpo code in UTF
		string letterUTFLowerCase;  //letter in UTF in lowerCase form
		string keyUTF;  //letter in TibetanUni in UTF
		string translit;
		int stackFlag;
		int OCRIndex;
		int baseLetter;
		string lang;
	}uniRecord;

	typedef struct rootLetterOCRStruct{
		string letter;
		vector<short> lj;  //joined leter
		vector<short> lsj;  //subjoined leter
		vector<short> lssj;  //sub subjoined leter
		vector<short> lm;  //modificator
		vector<short> lbase;  //valid (2)letters
		vector<short> lbase1;  //valid (3)letters
		vector<short> lbase2;  //valid (4)letters
		vector<short> uniHexCode; //self Unicode in Hex
		vector<short> uniHexKey;  // unicode for typing in Hex
	}rootLetterOCR;
	
	
	typedef struct fontOCRMap{
		vector<uniRecord> fMap;
		string fontName;
	}fontMap;
	
	typedef struct stringMatch_{
		int matchIndex;
		int startIndex;
		int endIndex;
		int endIndexSrc;
		int count;
		int lenSum;
	}stringMatch;

	typedef struct stringEntry_{
		char *keyPtr;
		int  keySize;
		char *strPtr;
		int  strSize;
	}stringEntry;	
	
	typedef struct hashRecord_{
        unsigned int hash;
		unsigned int adress;
		bool operator < (const hashRecord_ c) const{      
			return (this->hash<c.hash);
		}
		bool operator == (const hashRecord_ c) const
		{      
			return (this->hash==c.hash);
		}
	}hashRecord;	
	    
    typedef struct pidIDStruct{
        int pid;
		int status;
	}pidID;
    
    typedef struct sData{
        int ID;
        int len;
        int memBase;
        string topic;
        int haveTopic;
        int inTopic;
        string path;
    }sortData;
    
    static bool sortSearchData(const sortData& d1, const sortData& d2){
        if(d1.haveTopic){
            if(d1.len > d2.len){
                if(d1.inTopic)return 1;
            }
        }
        return d1.len > d2.len;
    }

        enum  stackFlag{
                VOWEL = 0,
                ROOT_LETTER = 1,
                ROOT_LETTER_VOWEL = 3,
                ROOT_LETTER_DOWN_VOWEL = 5,
                ROOT_LETTER_UP_DOWN_VOWEL = 6,
                STACK_2LETTER = 7,
                STACK_2LETTER_VOWEL = 9,
                STACK_2LETTER_DOWN_VOWEL = 11,
                STACK_2LETTER_UP_DOWN_VOWEL = 12,
                STACK_3LETTER = 13,   //:)
                STACK_3LETTER_VOWEL = 15,
                STACK_3LETTER_DOWN_VOWEL = 17,
                STACK_3LETTER_UP_DOWN_VOWEL = 19,
                STACK_4LETTER = 21,
                STACK_4LETTER_VOWEL = 23,
                STACK_4LETTER_DOWN_VOWEL = 25,
				STACK_4LETTER_UP_DOWN_VOWEL = 27,
				letterKey = 0,

        };
 	
		//константы для функций обработки изображений
		enum drawConst{
				IMGFLIP=1,
				IMGNOFLIP=0,
				IMGRGB=1,
				IMGBW=0,
                cRGB=4,
				ALL_LETTER=1,
				TEST_LETTER=2,
				NEWLETTERINBOOK=3,
				GET_LETTERS=4,
                VECTOR_OCR=5,
				BIGMASK=10,
				MIDDLEMASK=9,
				SMALLMASK=8,
				GIF=1,
				JPEG=2,
				LETTER_ADD=1,
				DRAW_BASE=0,
                DRAW_BLACK=1,
                PLETTER=0,
                WLETTER=1,
                BITMAP_32=32,
                ADD_MODE=1,
                XOR_MODE=2,
                OR_MODE=3,
                MATRIX_BORDER=128,
                T_POINT=1,
                X_POINT=2,
                L_POINT=4,
                P_POINT=8,
                C_POINT=9
		};

		//константы для функций передачи данных

	enum dataConst{
		CLOSE_SOCKET=0,
		INIT_SOCKET=1,
		TIB_TO_ENG_UTF=2,
		TIB_FROM_HASH=3,
		REMAP_STRING_TIB=4,
		HASH_FIND=5,
		STRING_FIND=6,
		LOCAL_MODE=7,
		SOCKET_MODE=8,
        IMPORT_TO_DB=9,
        EXPORT_FROM_DB=10,
        READ_FROM_DB=11,
        READ_FROM_BIN=12,
        READ_FROM_TAB=13,
        READ_FROM_XML=14,
        READ_FROM_TXT=15,
        MEMORY_LOCATION=16,
        MMAP_LOCATION=17,
        CORPUS_HTML=18,
        HTML=19,
        LETTER_TABLE_HTML=20,
        CHAT=21,
        ADD_TO_GMAP=22,
        REPLACE_IN_GMAP=23,
        SINGLE_LINE=24,
        MULTY_LINE=25,
        TRANSLATE=1,
        DICT_REPORT_TEXT=2,
        DICT_REPORT_USER=3,
        FULL_REPORT=4,
        OCR_NORMALISATION=100,
        OCR_IMAGE_PARAMETERS=101,
        
	};
	
     ///константы языков распознавания.
    enum OCRConst{
		OCR_TIBETAN=1,
        OCR_TIBETAN_ENG=2,
        OCR_TIBETAN_RUS=3,
        OCR_TIBETAN_SKT=4,
        OCR_SANSKRIT=5,
        OCR_SANSKRIT_ENG=6,
        OCR_KANNADA=7,
        OCR_SINHALA=8,
        OCR_SINHALA_RUS=9,
        OCR_SINHALA_ENG=10,
        REMOVE_DELIMETERS=11,
        NO_REMOVE_DELIMETERS=12,
        OCR_DICT_HASH=13,
        TRANSLATE_ENG=14,
        TRANSLATE_RUS=15
        
	};
    	
    ///search mode for database
    
    enum searchModeConst{
        TEXT_SEARCH=0,
        OCR_SEARCH=2,
        HASH_SEARCH=1,
        LIST_SEARCH=5,
        FULL_TEXT_SEARCH=6,
        STRING_SEARCH=8,
        ID_SEARCH=9,
        ID_SEARCH_DATA=10,
        LETTER_SEARCH=11,
        INDEX_COUNT=10,
        ALL_FIELD=1024
    };
    
    
		//////////////////////// ALGORITM ///////////////////////////

	
}

#endif //#ifndef HEADER_OCRTYPES


