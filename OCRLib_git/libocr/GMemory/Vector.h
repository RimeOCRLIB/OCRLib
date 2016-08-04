#ifndef MMAP_VECTOR_H
#define MMAP_VECTOR_H
#include "config.h"
#include "MmapFile.h"
#include "OCRTypes.h"
#include <stddef.h> // for size_t
#include <stdlib.h>

using namespace std;
namespace ocr {
    
    
#define INDEX_RESIZE 4 //во сколько раз будет увеличиваться размер индекса
#define FILE_RESIZE 2 //во сколько раз будет увеличиваться размер файла
#define FILE_RESIZE_DELTA 100 //количество записей на которое увеличивается размер файла в дополнении к увеличению в FILE_RESIZE раз.(нужен при записи больших записей в небольшой файл)
#define POOL_SIZE   16384
    
    
    ////////////////////////////////////
    class Vector;                   //forward declaration
    class TStr;                    //forward declaration
    template<typename T>class VStr;  //forward declaration
    
    /**
     Класс Vector использует механизм файла подкачки (mmap) для организации массива данных переменной
     длинны в виде дискового файла с отображением в память. В отличии от GVector организован на адресах а не на указателях, поэтому память локальные переменные Vector выделяется програмно в момент создания экземпляра класса. Освобождение памяти также происходит в момент входа из функции cоздавшей экземпляр класса
     TEST 10 000 000 записей
     std::vector push_back  time=0.377144
     GStr        push_back  time=0.383936
     VStr        push_back  time=0.379484
     
     
     vector []     time=0.043199
     GStr   []     time=0.074694
     VStr   []     time=0.060237
     VStr   dataT  time=0.035373  //внутренний указатель на внутренние данные VStr_.dataT[1]++;
     *      []     time=0.027704  //прямой указатель на внутренние данные T* t=VStr_.dataT; t[1]++;
     
     */
    class Vector{
    protected:
        string dataPath;
        MemoryFile *dataMFile;
        Vector *parent;
    public:
        Vector(void);
        Vector(string &path);
        Vector(cstr  path);
        Vector(Vector& ref);
        Vector(char* data_,int mode);
        Vector(Vector& parentVector, cstr name);

        char *data;
        uint *index;
        uint *innerData;		  //массив размещенный в начале файла, в который записываются внутренние переменные
        /// внутренние переменные
        uint *recordCount;  	  //количество записей в векторе
        uint *recordArraySize;    //размер файла учета записей
        uint *dataSize;  	 	  //размер занимаемый актуальными данными
        uint *poolSize;  	 	  //общий размер файла вектора
        uint *lockFlag;           //флаг блокировки чтения-записи
        uint *indexOffset;	 	  //адрес размещения массива индекса записей. отсчитывается от начала файла
        uint *vectorID;
        int dataLocation;
        
        ~Vector();

        /** resize Vector for new capacity*/
        void  resizeData(uint recordCount, uint datasize);   //datasize указывается в мегабайтах.
        void  resize(uint recordCount);   //datasize указывается в мегабайтах.
        void  clear();

        void reloadPtr();	 	 	 	  //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
    
        void* operator[] (uint indexRecord);
        
        void push_back(char*ptr, uint size);
        void push_back(string&str);
        void push_back(cstr str);
        void push_back(string&str, cstr name);
        void push_back(TStr &str);
        void push_back(VStr<int> &v);
        void addRecords(Vector &ref);
        
        void *getPtr(uint indexRecord,uint *size);
        void getStr(uint indexRecord, string&str);
        void getStr(uint indexRecord, vector<int>&str);
        void getStr(string&str,cstr name);
        int getInt(uint indexRecord);
        /**получение указателей на запись в виде токенизированной строки */
        void getTStr(uint indexRecord,TStr &str);
        /**получение индекса именных записей*/
        void getName(string&str);
        
        void putPtr(uint indexRecord, void*ptr, uint size);
        void putStr(uint indexRecord, string&str);
        void putStr(uint indexRecord, vector<int>&str);
        void putCStr(uint indexRecord,cstr  str);
        void putTStr(uint indexRecord,TStr &str);
        void putInt(uint indexRecord,int data);
        /**запись индекса именных записей*/
        void putName(string&str);
        
   	 	/**размещение именной записи при создании дочернего VStr. */
        char* setVector(cstr name,uint *vectorID);
        /**возврат указателя на данные дочерний VStr или Vector по именной записи */
        void* getVector(cstr name);
        /**возврат указателя на данные дочерний VStr или Vector по ID */
        void* getVector(uint childID);        
        
        /**вывод в result HTML rowsNum записей начиная с startRec */
        void drawHTML(uint startRec,int rowsNum,string &result,int mode);
        /**вывод в result HTML Vector* strResult по корпусу текстов*/
        void drawHTML(uint startRec,int rowsNum,Vector& strResult,string &result,int mode);
        /**вывод в result HTML результатов поиска */
        void drawHTML(vector<uint>&searchResult,string &result,int mode);
        /**вывод в result HTML результатов поиска по корпусу текстов*/
        void drawHTML(vector<uint>&searchResult,Vector& strResult, string &result,int mode);
        /**вывод в result HTML результатов поиска по каталогу библиотеки */
        void drawCatalogHTML(vector<uint>&searchResult,Vector*vectorLibPath,string &result,int mode);
        
	 	/** импорт записей из XML, TXT, tab-separated TXT*/
	 	void import(string &path, int mode);
	 	/** импорт записей из TXT построчно по разделителю \n или \r*/
	 	void importTXT(string &path);
        /** импорт записей из TXT построчно по разделителю \n или \r с записью ключей в файл path1*/
	 	void importDict(map<string,uint>kayMap,string &path,string &path1);
	 	/** импорт записей из tab-separated TXT построчно по разделителю \n или \r*/
	 	void importTAB(string &path);
	 	/** импорт записей из XML */
	 	void importXML(string &path);
        /** экспорт всех записей формата TStr*/
        void exportTStr(cstr path);
        /** экспорт всех записей формата TStr*/
        void exportTStr(string &path);
	 	

        uint data_size(void){return *dataSize;};
        uint size(void){return *recordCount-1;};
        //переписывает данные, оптимизируя размещение
        bool save(void){return 0;};
        //записывает данные в дисковый файл, оптимизируя размещение
        bool save(string &path){return 0;};
        //записывает данные в дисковый файл, оптимизируя размещение
        bool save(cstr path){return 0;};
        //экспорт в XML с рекурсивным выводом именных дочерних записей
        void saveXML(string &path);
        string path(){string str=dataPath; return str;}
    };
     
}

#endif