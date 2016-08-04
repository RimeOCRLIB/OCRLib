#ifndef MMAP_GVECTOR_H
#define MMAP_GVECTOR_H
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
    class GVector;                   //forward declaration
    class TString;                   //forward declaration
    template<typename T>class GStr;  //forward declaration
    
    /**
     Класс GVector использует механизм файла подкачки (mmap) для организации массива данных переменной
     длинны в виде дискового файла с отображением в память.
     */
    class GVector{
    protected:
        GVector(void);
        GVector(string &path);
        GVector(cstr  path);
        GVector(GVector* ref);
        GVector(char* data_,int mode);
        GVector(GVector* parentVector, cstr name);
        void init();
        void init(string &path);
        void init(GVector* ref);
        void init(char* data_, int mode);
        void init(GVector *parentVector, cstr name);
        string dataPath;
        MemoryFile *dataMFile;
        GVector *parent;
    public:
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
        
        virtual ~GVector();
        void free(void);
        static GVector* create(){return new GVector();};
        static GVector* create(string&path){return new GVector(path);};
        static GVector* create(GVector* ref){return new GVector(ref);};
        static GVector* create(GVector* parentVector, cstr name){return new GVector(parentVector,name);};
        static GVector* create(char* data_,int mode){return new GVector(data_,mode);};
        static GVector* create(cstr path){return new GVector(path);};
        /** resize GVector for new capacity*/
        void  resizeData(uint recordCount, uint datasize);   //datasize указывается в мегабайтах.
        void  resize(uint recordCount); //recordCount - количество записей
        void  clear();

        void reloadPtr();	 	 	 	  //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
    
        
        void push_back(string&str);
        void push_back(cstr str);
        void push_back(char*str, uint size);
        void push_back(string&str, cstr name);
        void push_back(TString *str);
        void addRecords(GVector *ref);
        
        void *getPtr(uint indexRecord,uint *size);
        void getStr(uint indexRecord, string&str);
        void getStr(uint indexRecord, vector<int>&str);
        void getStr(string&str,cstr name);
        int getInt(uint indexRecord);
        /**получение указателей на запись в виде токенизированной строки */
        void getTStr(uint indexRecord,TString *str);
        /**получение индекса именных записей*/
        void getName(string&str);
        
        void putPtr(uint indexRecord, void*ptr, uint size);
        void putStr(uint indexRecord, string&str);
        void putStr(uint indexRecord, vector<int>&str);
        void putCStr(uint indexRecord,cstr  str);
        void putTStr(uint indexRecord,TString *str);
        void putInt(uint indexRecord,int data);
        /**запись индекса именных записей*/
        void putName(string&str);
        
   	 	/**размещение именной записи при создании дочернего GStr. */
        char* setVector(cstr name,uint *vectorID);
        /**возврат указателя на данные дочерний GStr или GVector по именной записи */
        void* getVector(cstr name);
        /**возврат указателя на данные дочерний GStr или GVector по ID */
        void* getVector(uint childID);
        
        
        /**вывод в result HTML rowsNum записей начиная с startRec */
        void drawHTML(uint startRec,int rowsNum,string &result,int mode);
        /**вывод в result HTML GVector* strResult по корпусу текстов*/
        void drawHTML(uint startRec,int rowsNum,GVector* strResult,string &result,int mode);
        /**вывод в result HTML результатов поиска */
        void drawHTML(vector<uint>&searchResult,string &result,int mode);
        /**вывод в result HTML результатов поиска по корпусу текстов*/
        void drawHTML(vector<uint>&searchResult,GVector* strResult, string &result,int mode);
        /**вывод в result HTML результатов поиска по каталогу библиотеки */
        void drawCatalogHTML(vector<uint>&searchResult,GVector*vectorLibPath,string &result,int mode);
        
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
        /** экспорт всех записей формата TString*/
        void exportTStr(cstr path);
        /** экспорт всех записей формата TString*/
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
    
    /**Класс GStr одномерный вектор. На его основе работает построение
     таблиц и деревьев в GVector. GStr может располагатся как в памяти,
     так и в записи GVector. И в том и в другом случае GStr сериализован и расположен в непрерывном участке памяти по адресу data. При необходимости GStr запрашивает у родительского вектора расширения выделенного места. Для доступа к данным GStr предоставляет указатель на свои данные*/
    template<typename T>
    class GStr{
    protected:
        GStr(void);
        GStr(int sizePool);
        GStr(GVector *parentVector, cstr name);
        //GStr(cstr path);
        void init(void);
        void init(int sizePool);
        void init(GVector *parentVector, cstr name);
        //void init(cstr path);
        char *data;
        GVector *parent;
        uint *innerData;		  //массив размещенный в начале файла, в который записываются внутренние переменные
        /// внутренние переменные
        MemoryFile *dataMFile;
    public:
        uint *recordCount;  	  //количество записей в векторе
        uint *dataSize;  	 	  //размер занимаемый актуальными данными
        uint *poolSize;  	 	  //общий размер файла вектора
        uint *vectorID;	 	      //твердая копия индекса записи, в которой GStr размещен в родительском векторе
        uint dataLocation;
        uint vectorID_;           //индех записи, в которой GStr размещен в родительском векторе
        
        virtual ~GStr();
        void free(void);
        static GStr* create(){return new GStr();};
        static GStr* create(int sizePool){return new GStr(sizePool);};
        static GStr* create(GVector *parentVector, cstr name){return new GStr(parentVector, name);};
        //static GStr* create(cstr path){return new GStr(path);}
        void reloadPtr();
        void resize(uint size);
        void reload();
        void read(cstr path);
        void save(cstr path);
        
        void push_back(T dat);
        void push_Ptr(void *dat, uint size);
        
        T* getPtr(uint indexRecord, uint size);
        void put(uint indexRecord,T dat);
        void putPtr(uint indexRecord, void *dat,  uint size);
        void pop_back();
        T get(uint indexRecord);
        T& operator[] (uint id);
        void operator =(GStr<T> *ref);
        uint size(void){return *recordCount;};
        //получение указателя на актуальные данные
        char* dataPtr(){return data+24;};
    };
    template<typename T>
    GStr<T>::GStr(){
        try
        {
            init();
        }
        catch(int a)
        {
            free();
        }
    }
    template<typename T>
    GStr<T>::GStr(int sizePool){
        try
        {
            init(sizePool);
        }
        catch(int a)
        {
            free();
        }
    }
    template<typename T>
    GStr<T>::GStr(GVector *parentVector, cstr name){
        try
        {
            init(parentVector,name);
        }
        catch(int a)
        {
            free();
        }
    }
    
    /**восстановление указателей на внутренние переменные после изменения размера или расположения в памяти*/
    template<typename T>
    void GStr<T>::GStr::reloadPtr(){
        
        
        poolSize=&innerData[1];                     //размер файла вектора
        recordCount=&innerData[2];  	            //количество записей в векторе
        dataSize=&innerData[3];  	 	            //размер занимаемый актуальными данными
        vectorID=&innerData[4];
        
        
    }
    /**восстановление указателей на внутренние переменные после изменения размера или расположения в памяти родительского вектора*/
    template<typename T>
    void GStr<T>::GStr::reload(){
        //запрашиваем у родительского GVector указатель на файл данных
        //если запись с таким именем уже существует, получаем на нее указатель
        data=(char*)parent->getVector(vectorID_);
        //проверяем есть ли данные в GStr
        innerData=(uint*)(data);
        reloadPtr();
        if(innerData[0]!=0xffffffff){ 	 	 	 	//маркер наличия GStr в записи GVector
            // новый вектор
            innerData[0]=0xffffffff;
            *poolSize=POOL_SIZE;
            *recordCount=0;
            *dataSize=24;
            *vectorID=vectorID_;
        }
    }
    
    template<typename T>
    void GStr<T>::GStr::init(){
        //подключаем или создаем файл данных
        data=(char*)calloc(POOL_SIZE,1);
        innerData=(uint*)(data);
        innerData[0]=0xffffffff;
        reloadPtr();
        *poolSize=POOL_SIZE;
        *recordCount=0;
        *dataSize=24;
        *vectorID=0xffffffff;
        vectorID_=*vectorID;
        dataLocation=MEMORY_LOCATION;
    }
    template<typename T>
    void GStr<T>::GStr::init(int sizePool){
        //подключаем или создаем файл данных
        data=(char*)calloc(sizePool,1);
        innerData=(uint*)(data);
        innerData[0]=0xffffffff;
        reloadPtr();
        *poolSize=sizePool;
        *recordCount=0;
        *dataSize=24;
        *vectorID=0xffffffff;
        vectorID_=*vectorID;
        dataLocation=MEMORY_LOCATION;
    }

    
    template<typename T>
    void GStr<T>::init(GVector *parentVector, cstr name){
        //запрашиваем у родительского GVector указатель на файл данных
        //если запись с таким именем уже существует, получаем на нее указатель
        parent=parentVector;
        data=parentVector->setVector(name,&vectorID_);
        //проверяем есть ли данные в GStr
        innerData=(uint*)(data);
        reloadPtr();
        if(innerData[0]!=0xffffffff){ 	 	 	 	//маркер наличия GStr в записи GVector
            // новый вектор
            innerData[0]=0xffffffff;
            *poolSize=POOL_SIZE;
            *recordCount=0;
            *dataSize=24;
        }
        *vectorID=vectorID_;
        dataLocation=MMAP_LOCATION;
        //cout<<"magicNum="<<innerData[0]<<endl;
        //cout<<"poolSize="<<*poolSize<<endl;
        //cout<<"recordCount="<<*recordCount<<endl;
        //cout<<"dataSize="<<*dataSize<<endl;
        //cout<<"vectorID="<<*vectorID<<endl;
        
    }
    template<typename T>
    void GStr<T>::resize(uint newSize){
        uint newSizeByte=(uint)(newSize*sizeof(T));
        uint poolSize_=*poolSize;
        if(*poolSize<newSizeByte+24){
            if(vectorID_==0xffffffff){
                char *data_=(char*)calloc(newSizeByte+24,1);
                memcpy(data_,data,*dataSize);
                std::free(data);
                data=data_;
                poolSize_=newSizeByte+24;
            }else{
                string str;
                str.resize(newSizeByte+24);
                memcpy(&str[0],data,*poolSize);
                parent->putStr(vectorID_,str);
                data=(char*)parent->getPtr(vectorID_,&newSizeByte);
                poolSize_=newSizeByte;
                cout<<"@new size="<<newSize<<endl;
            }
            innerData=(uint*)(data);
            reloadPtr();
            *poolSize=poolSize_;
            *vectorID=vectorID_;
        }
        *recordCount=newSize;
        *dataSize=newSizeByte+24;
        
        //cout<<"poolSize="<<*poolSize<<endl;
        //cout<<"recordCount="<<*recordCount<<endl;
        //cout<<"dataSize="<<*dataSize<<endl;
        //cout<<"vectorID="<<*vectorID<<endl;
    }

    template<typename T>
    void GStr<T>::pop_back(){
        if(!*recordCount)return;
        *recordCount=*recordCount-1;
        *dataSize-=(int)sizeof(T);
    };
    
    
    template<typename T>
    GStr<T>::~GStr(){}
    
    template<typename T>
    void GStr<T>::free(void){
        if (dataLocation==MEMORY_LOCATION) std::free(data);
        delete this;
    };
    
    template<typename T>
    void GStr<T>::save(cstr path){
        char *data_=data+24;
        dataMFile=MemoryFile::create(path,MemoryFile::if_exists_keep_if_dont_exists_create);
        dataMFile->resize(size()*sizeof(T));
        char *dataDest=dataMFile->data();
        memcpy(dataDest,data_,size()*sizeof(T));
        //cout<<" save1 dataMFile->size()="<<dataMFile->size()<<" size()="<<size()*sizeof(T)<<endl;

    };

    
    template<typename T>
    void GStr<T>::read(cstr path){
        //подключаем или создаем файл данных
        dataMFile=MemoryFile::create(path,MemoryFile::if_exists_keep_if_dont_exists_create);
        char *textBuffer=dataMFile->data();
        push_Ptr(textBuffer,dataMFile->size()-24);
        //cout<<" read dataMFile->size()="<<dataMFile->size()<<" size()="<<size()<<endl;
    };
   
    
    template<typename T>
    void GStr<T>::push_back(T dat){
        push_Ptr(&dat, sizeof(T));
    };
    
    
    //получение данных из GStr
    template<typename T>
    T* GStr<T>::getPtr(uint indexRecord, uint size){
        return (T*)(data+indexRecord*size+24);
    };
    
    template<typename T>
    void GStr<T>::put(uint indexRecord,T dat){
        memcpy(data+indexRecord*sizeof(T)+24,&dat,sizeof(T));
    };
    
    template<typename T>
    void GStr<T>::putPtr(uint indexRecord,void *dat,uint size){
        memcpy(data+indexRecord*size+24,&dat,size);
    };
    
    template<typename T>
    T& GStr<T>::operator[] (uint id){
        return (T&)*(data+id*sizeof(T)+24);
    };
    
    template<typename T>
    T GStr<T>::get(uint id){
       return *(T*)(data+id*sizeof(T)+24);
    }
    
    template<typename T>
    void GStr<T>::operator =(GStr<T> *ref){
        free();
        init();
        push_Ptr(ref->dataPtr(),ref->size()*sizeof(T));
    }
    
    //добавление новой записи в GStr
    template<typename T>
    void GStr<T>::push_Ptr(void *dat, uint size){
        //проверяем достаточно ли места для новой записи
        if(*poolSize<*dataSize+size){
            //cout<<"resize GStr. poolSize="<<*poolSize<<" new poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA<<" *vectorID="<<*vectorID<<endl;
            uint newSize=(*poolSize)*FILE_RESIZE+FILE_RESIZE_DELTA*size;
            if(*vectorID==0xffffffff){
                char *data_=(char*)calloc(newSize+24,1);
                memcpy(data_,data,*dataSize);
                std::free(data);
                data=data_;
            }else{
                string str;
                str.resize(newSize);
                memcpy(&str[0],data,*poolSize);
                parent->putStr(vectorID_,str);
                data=(char*)parent->getPtr(vectorID_,&newSize);
                //cout<<"@new size="<<newSize<<endl;
            }
            innerData=(uint*)(data);
            reloadPtr();
            *poolSize=newSize;
            
        }
        //записываем данные
        memcpy(data+*dataSize,dat,size); 	 	        //записываем длинну записи
        *recordCount=(uint)(*recordCount+size/sizeof(T));
        *dataSize=*dataSize+size;
        //if(size==3)//cout<<"size="<<size<<" new dataSize="<<*dataSize<<" poolSize="<<*poolSize<<" recordCount="<<*recordCount<<endl;
        
    };
    
    
    /**строка с размеченными подстроками используется как формат передачи данных
     в таблицах GVector. Записанную в GVector TString можно только читать.
     в памяти стороку можно изменять и записывать в GVector новую стороку.
     Использует конструктор по умолчанию и не требует ручной деаллокации.*/
	
	class TString{
	public:
        TString();
        ~TString();
        uint *index;   	   //указатель на индекс начальных позиций строк
        uint len;		   //длина индекса
        char *data;		   //указатель на данные
        uint sizeData;	 	   //размер данных
        vector<char>dataStr;
        vector<uint>indexStr;
        void push_back(char*ptr, uint size_);
        void push_back(TString *str);
        void push_back(string &str);
        void push_back(const char* str);
        void push_TabStr(string &str);
        void setFromPtr(uint*index_,uint len_,char*data,uint dataSize);
        string operator [](uint i);
        void get(TString *st, uint i);
        int readInt(uint i);
        void readStr(string &str, uint i);
        void readIntVector(vector<int>&intVector,uint i);
        void readCharVector(vector<char>&charVector,uint i);
        void operator +=(string &str);
        void operator +=(cstr str);
        void operator +=(uint i);
        uint size(){ return len;};
	};
    
    ////////////////////////////////////
    
    
}

#endif