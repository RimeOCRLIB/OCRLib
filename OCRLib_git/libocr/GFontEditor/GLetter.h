#ifndef HEADER_GLetter
#define HEADER_GLetter
//__OCR CLASSES_____________________________________
#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "GBitmap.h"
#include "GBitMask.h"
#include "GContour.h"
#include "OCRTypes.h"
#include "GVector.h"
#include "GGraph.h"

using namespace std;


namespace ocr {
    

    class drawDataRecord {
    public:
        drawDataRecord();
        vector<int>color;
        vector<vector<OCRPoint> >data;
        short count;
    };
    
    class vectorOCR {
	protected:
        
	public:
		vectorOCR(void);
		virtual ~vectorOCR();
		void destroy(void);
		static vectorOCR* create(void){return new vectorOCR;}
		OCRPoint vStart;
		OCRPoint vEnd;
		short length;
		string name;
		OCRPoint vCenter;
		//vector<OCRPoint>points;  //count of points in vector
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
        virtual ~GString();
        char data[128];
        int len;
        void operator=(const GString ref);
        void operator=(const char *ref);
        void operator=(string &ref);
        bool operator==(const GString ref);
        bool operator==(const char *ref);
        bool operator==(string &ref);
        bool operator!=(const GString ref);
        bool operator!=(const char *ref);
        bool operator!=(string &ref);
        
        void operator+(const char *ref);
        void operator+=(const char *ref);
        
        int size(){return len;}
        const char* c_str(){return data;}
        char* operator[] (int id);
        int find(const char *ref);
        int find(string &ref);
    };
    
    //ostream& operator << (ostream& os, GString& ref);
    
    ///класс содержит данные буквы языка и методы обращения к ее графическим особеностям
    //class GLetterPtr это виртуальный класс описывающий данные и функции GLetter.
    // доступ к данным предоставляется по ссылке на упакованные в TString данные, которые хранятся
    //в базе данных. Таким образом при использовании GLetterPtr не происходит копирования данных в память.
    //вызов конечных параметров GLetter происходит по inline функции, возвращающей указатель на параметр
    
    struct keyOCR{
        ushort letterIndex;
        uchar correlation;
    };
    
    
    ///класс содержит данные буквы языка и методы обращения к ее графическим особеностям
	class GLetter {
    private:
        GLetter(void);
        
	public:
		void destroy(void);
		static GLetter* create(void){return new GLetter;}
		ulong letterIndex;                                ///letter index in base
		unsigned short idNumber;                        ///<id number in base
		int stringNum;
		int recordFlag;
		int recordStatus;                       ///флаг принадлежности к языку распознавания
		ulong lengthSum;
		int onOffRate;                          ///соотношение площади буквы покрытой и не покрытой признаками
		vectorOCR *v;
		uint vectorCount;
        GContour *contour[10];
        uint contourCount;
		GBitMask32 *mask32;                      ///<<указатель на маски признаков
        GStr<GBitMask32>*mask32Vector;           ///контейнер для масок буквы
		GBitMask128 mask128;                     ///габаритная маска(изображение) буквы
        GBitMask128 maskOriginal;                ///габаритная маска оригинального исходного изображения буквы
        GStr<keyOCR>*keyOCRVector;               ///массив ближайших по корреляции букв
        keyOCR *key;                             ///указатель на массив ближайших по корреляции букв
        uint keyCount_;                     
        GBitmap *pict;                           ///изображение буквы
        GStr<OCRPoint>*focalPoint;               ///массив топологических ключевых точек буквы (фокальные точки)
        vector<OCRFocalLine>focalLine;            ///массив линий соединяющих топологические ключевые точки буквы (фокальные линии)
        GStr<OCRBox>*openType;                   ///массив габаритов всех составляющих букву элементов OpenType 
        OCRPoint *fPoint;                        ///указатель на массив топологических ключевых точек буквы
        OCRFocalLine *fLine;                     ///указатель на массив фокальных линий буквы
        int pictFlag;                            ///флаг наличия изображения в букве
		int dX,dY;                               ///<смещение координат центра буквы относительно изображения буквы  (mask128)
		string name;
        string OCRKey;
		int OCRIndex;
		int correlation;                         ///<value of correlation
		int selfCorrelation;                     ///<test value of self correlation
		int codeSpace;                           ///<test value of test value of code space between this letter and all letters in letter base
		int Character;
		int vectorLimit;
		int stackFlag;
		int OCRStatus;
		int letterW;                             ///габаритная ширина буквы в строке без учета огласовок и подписных букв
		int letterH;                             ///габаритная высота буквы
		int baseLetter;
		int xMin;
		int xMax;
		int yMax;
		int yMin;
		int xSum;
		int ySum;
		float scale;
		int cn;                                  //counter of this letter recognition
		uint cnRes;                      //rezult counter of this letter recognition (cCount/10 000)
		uint crSum;                      //buffer for store result of correlation this letter in text
		uint crRes;                      //middle result of correlation
		int y0;                                  //верхний габарит строки относительно центра изображения буквы  (mask128)
		int y1;                                  //нижний габарит строки относительно центра изображения буквы (mask128)
		int textLineSize;                        //size of text line in text in pixels
		int newLetter;
		int newLetterInBook;
		int allLetterCount;
		int GFontSize;
		int lineCount;
		
		void eraseMask(int id);
		void copy(GLetter* letter);
		void calibrateMask(GBitmap *img);
		void reloadMask();                       ///rebuild all mask from letter image
        void reloadMask(int m);                  ///rebuild mask by index from letter image
        void eraseEmptyMask();                   ///erase all empty mask and set mask32Count
		void maskFromVector();                   ///set masks from vectors in letter
        string drawToSVG();
        void cropLetter();                       ///set all mask in letter in region of letter
        void eraseDublicateMask();               ///set all symillar by coordinate masks in letter
        void maskNormalisation();                ///устанавливает высоту всех масок кратной 2 Необходимо для функций ASM
        void setLetterDimensions();              ///Установление границ и координат центра буквы относительно строки
        void eraseAllMask(){
            mask32Vector->resize(0);
            //focalPoint->resize(0);
        };                                       ///стирает все маски в букве
        void eraseMaskContent(int maskId);       ///стирает содержимое маски из изображения буквы
        void drawContour();                      ///создает mask128 содержащий изображения контуров буквы
        string saveToString();                   ///save letter in string as char buffer
        void saveAsBin(GLetter *glyph);          ///сериализация буквы
        void readFromBin(GLetter *glyph);        ///чтение сериализованной буквы
        GBitmap* drawPict();                     ///создает mask128 изображения буквы
        GBitmap* drawPict32();                   ///создает изображение буквы с признаками в сжатом по 32 бита GBitmap
        GBitmap* drawLetterPict(int mode);       ///создает изображение буквы с признаками
        void writeToStr(TString *st);            ///записывает структуру буквы в TString
        void readFromStr(TString *st);           ///читает букву из TString 
        void push_back(GBitMask32 &mask);              ///добавляет в букву новую маску
        void push_back(keyOCR &mask);             ///добавляет в букву новый keyOCR
        ulong mask32Count(){
            return mask32Vector->size();};       ///<количество признаков
        ulong fPointCount(){
            return focalPoint->size();};       ///<количество признаков
        void pop_backMask(){
            mask32Vector->pop_back();};          ///<стирает последнюю маску
        
	};
    
    int operator==(const GLetter &left, const GLetter &right);
    int operator<(const GLetter &left, const GLetter &right);

    class OCRMatchConst;
    
    class OCRMatch{
	public:
		OCRMatch(void);
		OCRPoint start;
		OCRPoint end;
		int matchCount; //count for mutch points in structure
		int centerDist;  //distance between centers of match vector
		//int contourIndexTest; //count for mutch points in structure
		//int contourIndexModel; //count for mutch points in structure
		int vectorIndexModel; //index of match vector in base
		int vectorIndexTest; //index of match vector in base
		int allMatchCount;
		int matchSumTest;                   //calculated match
        int xCenter;                        //letter center
        int yCenter;
        int xLC;                            //центр пары букв
        int xL0C;                           //центр первой буквы в паре
        int xL1C;                           //центр второй буквы в паре

		int status;
		int letterIndex;
        int lineIndex;
        int letterID;
        int letterW;
        int letterH;
        int lineH;
        int maxY;
		float correlation;
        float correlationNew;
		int Character;
        ushort uni;
        int x0,x1,y0,y1;                    //координаты буквы на странице
        int dX,dY;
        OCRBox s;                           //координаты прямоугольника буквы с учетом бордюра в GBitsetMatrix
        int area;                           //площадь габарита буквы
        int pCount;                         //количество пикселов буквы
		string name;
        string nameUni;
        wstring wName;
        string delimeter;
		int OCRIndex;
        vector<GBitMask32>mask32Vector;
        vector<int>line;                  // вектор индексов пар букв входящих в OCRMatch
        vector<int>dIndex;             // вектор индикатора наличия разделителя слогов после буквы гипотезы
        uint length;                       // длинна фразы найденой в словаре
        vector<OCRMatch>letter;            // вектор пар букв входящих в OCRMatch
        vector<uint>pSum;                  // массив интегральных сумм (описание в testWordLine.cpp)
        int id;
        void copyData(GLetter *letter);
        void operator+=(const OCRMatch &right);
        void operator = (OCRMatchConst &right);
        void setSize(int mode);//устанавливает все координаты прямоугольника буквы
        void setCenter();//устанавливает ценр буквы по габаритам и персчитывает координаты масок признаков
        void drawPict32(GBitmap* outBitmap32,int dx,int dy, int mode);    ///создает изображение буквы с признаками в сжатом по 32 бита GBitmap
        //добавляет новые маски в букву и пересчитывает их координаты
        void addMask32Vector(OCRMatch &a);


	};
    
    
    int operator==(const OCRMatch &left, const OCRMatch &right);
    int operator<(const OCRMatch &left, const OCRMatch &right);
    
    class OCRMatchConst{
	public:
		OCRMatchConst(void);
        virtual ~OCRMatchConst();
		OCRPoint start;
		OCRPoint end;
		int matchCount; //count for mutch points in structure
		int centerDist;  //distance between centers of match vector
		//int contourIndexTest; //count for mutch points in structure
		//int contourIndexModel; //count for mutch points in structure
		int vectorIndexModel; //index of match vector in base
		int vectorIndexTest; //index of match vector in base
		int allMatchCount;
		int matchSumTest; //calculated match
        int xCenter; //letter center
        int yCenter;
		int status;
		int letterIndex;
        int lineIndex;
        int letterID;
        int letterW;
        int letterH;
        int lineH;
        int maxY;
		float correlation;
        float correlationNew;
		int Character;
        int x0,x1;
		GString name;
        GString nameUni;
		int OCRIndex;
        int id;
        void operator = (const OCRMatch &right);
	};

    
    struct maskData{
        uint id;
        uint correlation;
        uint letterIndex;
        
    };
 	
    class TsertoOCR {
	public:
		TsertoOCR();
		int y;   // vertical coordinate point
		int x0;  // first horizontal coordinate point
		int x1;  // last horizontal coordinate point
        
		int x;   // horizontal coordinate point
		int y0;  // first vertical coordinate point
		int y1;  // last vertical coordinate point
        
		int w;
		int h;
        
		vector<pointLine> line;
        //		char status;
	};
	
	class blockOCR{
	public:
		blockOCR(void);
		virtual ~blockOCR();
		void destroy(void);
        
        int xb0,yb0,xb1,yb1;
        int blockFlag;
        int langFlag;
        
        // xb0,yb0, xb1,yb1  координаты прямоугольника вокруг блоков букв
        // blockFlag флаг принадлежности к колонке(значение флага-номер колонки)
        // langFlag  флаг языка 0 не определено, 1 tib, 2 en.
        
        
	};//_________________________________________________
    /**/
	class lineOCR{
	public:
        lineOCR(void);
       	virtual ~lineOCR();
		void destroy(void);
        int xL0,yL0,xL1,yL1;
        int lineFlag;
        
        // xL0,yL0, xL1,yL1  // координаты прямоугольников горизонтальных
        // блоков L, описывающих вертикальные колоноки в структурах строк.
        // lineFlag флаг принадлежности к колонке (значение флага-номер колонки)
        // флаг 0 не определено. 1,2,3,...  - номер колонки
        
	};//_________________________________________________
    /**/
    
	class columnOCR{
	public:
		columnOCR(void);
		virtual ~columnOCR();
		void destroy(void);
		//static columnOCR* create(void){return new columnOCR;}
        
        int xc0,yc0,xc1,yc1;
        int Xc0,Yc0,Xc1,Yc1;
        int columnFlag;
        // xc0,xc1, yc0,yc1  координаты блоков, вокруг которых описан
        // прямоугольник вертикальной колонки.
        // Xc0,Xc1, Yc0,Yc1  тоже что и  xc0,xc1, yc0,yc1 только
        // не вычитается border ( в функции data_normalisation ).
        // columnFlag флаг горизонтальных блоков
        // columnFlag флаг принадлежности к колонке (значение флага-номер колонки)
        // 0 не определено. 1, 2, 3,... номера вертикальных колонок на развороте.
        
	};//_________________________________________________
    /**/
    
	class recordOCR{
	public:
		recordOCR(void);
		virtual ~recordOCR();
		void destroy(void);
		//static recordOCR* create(void){return new recordOCR;}
        
        int xr0,xr1;
        
        // xc0,xc1  координаты блоков, вокруг которых описан
        // прямоугольник вертикальной колонки. Тоже что и vector <recordOCR>
        
	};//_________________________________________________
    /**/
    
    class frameOCR{
    public:
        frameOCR(void);
        virtual ~frameOCR();
        void destroy(void);
        static frameOCR* create(void){return new frameOCR;}
        
		int xf0,yf0,xf1,yf1;
		int Xf0,Yf0,Xf1,Yf1;
        
		int xfg0,yfg0,xfg1,yfg1;
		int Xfg0,Yfg0,Xfg1,Yfg1;
        
		int xbr0,xbr1,ybr0,ybr1;
		int frameFlag;
        
		vector <columnOCR>columnArray;
		vector <recordOCR>recordArray;
        
		// xf0,xf1, yf0,yf1;  координаты прямоугольника вокруг многоугольника
		// вертикальной колонки.
		// Xf0,Xf1, Yf0,Yf1;  тоже что и  xf0,xf1, yf0,yf1; только
		// не вычитается border ( в функции data_normalisation ).
        
		// xfg0,xfg1, yfg0,yfg1;  координаты прямоугольника (габариты) вокруг
		// многоугольника вертикальной колонки (с добавлением бордюра вокруг блока текста).
		// Xfg0,Xfg1, Yfg0,Yfg1;  тоже что и  xfg0,xfg1, yfg0,yfg1 только
		// не вычитается border ( в функции data_normalisation ).
        
		// xbr0,xbr1,ybr0,ybr1; зазор (бордюр) между многоугольниками вертикальной колонки.
		// frameFlag  флаг номера колонки (значение флага - номер колонки)
		// 0 не определено. 1, 2, 3,... номера вертикальных колонок на развороте.
    };//________________________________________________
    
    
    class wordOCR{
	public:
		wordOCR(void);
		virtual ~wordOCR();
		void destroy(void);
		static wordOCR* create(void){return new wordOCR;}
		// wordOCR(int _x,int _y,int _x1,int _y1){x0=_x;x1=_x1;y0=_y;y1=_y1;w=_x1-_x;h=_y1-_y;};
		int x0,y0,x1,y1,xp0,yp0,xp1,yp1,xp,yp,xt0,yt0,xt1,yt1,w,h;
		int xv0,yv0,xv1,yv1;
		OCRPoint center;
		string name;
		string Wylie;
		string html;
		string delimeter;
		string oldName;
		string  newLine;
		string file;
		string commentaryIndex;
		int sillablyCount;
		int letterIndex;
		int lineIndex;   /// index of line in page
        int letterID;
		vector<OCRMatch> match,match1;
		vector<TsertoOCR> TLline;
		GBitmap *img;  ///bitmap image from origimal text
		int correlation;
		int tsertoFlag;
		int spaceFlag;
		int stackFlag;
		int vowelFlag;
		int Param1,Param2;
		int selectFlag;
		int blockIndex;
		int OCRIndex;
		int fontSize;
		int checkFlag;   //flag of cheking by correction table
		int imgFlag;
		int id;
        
        // x0, x1,  y0, y1   координаты квадрата вокруг буквы
        // xp0,xp1, yp0,yp1  координаты прямоугольника вокруг точки
        // xt0,yt0, xt1,yt1  координаты прямоугольника вокруг стека
        // xp,yp             координаты точки
        // tsegFlag  флаг наличия точки в прямоугольнике
        // spaceFlag флаг наличия пробела после точки
        // stackFlag флаг наличия стека вместо коренной буквы.
        // vowelFlag флаг наличия огласовки над коренной буквой.
        
        //wordOCR& operator=(wordOCR &st){
        // x0=st.x0;x1=st.x1;y0=st.y0;y1=st.y1;w=st.w;h=st.h;
        //}
        //int operator==(wordOCR &wt){
        // if(x0==wt.x0&&x1==wt.x1&&y0==wt.y0&&y1==wt.y1&&name==wt.name) return 1;  //&&w==wt.w&&h==wt.h&&
        // return 0;
        //}
        
        bool operator<(wordOCR& right){
			return correlation < right.correlation;
            };
            bool  operator==(vector<string> &ref);
            void  operator=(wordOCR *ref);
            bool testDelimeter(vector<string> &ref);
            
            };//_________________________________________________
            
            struct wordOCR_sort {
                bool operator ()(wordOCR const& a, wordOCR const& b) const {
                    string s1,s2;
                    s1=a.name+a.delimeter+a.commentaryIndex;
                    s2=b.name+b.delimeter+b.commentaryIndex;
                    if (s1 < s2) return true;
                    if (s1 > s2) return false;
                    return false;
                }
            };
            
            class stringOCR{
            public:
                stringOCR(void);
                virtual ~stringOCR();
                void destroy(void);
                stringOCR(int _x,int _y,int _x1,int _y1){
                    x0=_x;x1=_x1;y0=_y;y1=_y1;w=_x1-_x;h=_y1-_y;
                };
                int x0,y0,x1,y1,w,h;
                int stringFlag;	/// флаг cтроки  =0 (=1), cтрока состоит из линии (букв).  //mark flag for pecha border and empty string
                int NLetter;    /// примерная длинна строки (оценка)   //letter count
                int LimY0;      /// верхний лимит строки               //upper y limit
                int LimY1;      /// нижний лимит строки                //doun y limit
                int MeanSA;     /// средее, по всей строке, расстояние между центрами масс букв  //middle letter space
                int vLim;       /// нижний лимит длинны вектора для данной строки //vector length limit for string
                int lineIndex;  /// индех строки в странице
                unsigned int idNumber; //uniq id number of string
                //vector<int> letter;
                short count;
                string name;
                string str;
                string strHTML;
                string delimeter;
                string oldName;
                string file;
                int selectFlag;
                int imgFlag;
                int syllableCount;
                int correlation;
                GBitmap *img;  ///bitmap image of string from origimal text
                string path; //original text filename
                
                int operator==(stringOCR &wt){
                    if(name==wt.name) return 1;  //&&w==wt.w&&h==wt.h&&
                    return 0;
                }
                vector <wordOCR>wordArray;     //main point. here is information about every letter correlation
                vector <blockOCR>blockArray;   //language segmentetion
                vector <lineOCR>lineArray;     //maket segmentation
                vector<OCRMatch>line;          //OCR result
                int top;
                
                int length();
                int size();
                void push_back(const wordOCR &word);
                void insert(int index, wordOCR &word);
                int find(const string& match,int position);
                void resize(int);
                stringOCR substr(int position,int length);
                void set_id(string &srcStr);  ////на вход функции приходит строка ответа базы данных с расставленными слогами. как выход функции мы получаем строку в которой сохранена пунктуация исходной строки с добавлением пунктуации словаря
                //string html_report(string &srcStr);  
                void clearOCR();  //стирает результаты распознаваниея в строке. Устанавливает имена в wordArray как (*)
                
                
                /** Return pointer on index element of wordArray
                 */ 
                wordOCR*  operator[](int index);
                void  operator+=(stringOCR& ref);
                void  operator+=(wordOCR ref);
                
                static bool stringOCR_sort(const stringOCR& d1, const stringOCR& d2){
                    return d1.name < d2.name;
                }
                
                
            };//________________________________________________
            
            
            };   
#endif
