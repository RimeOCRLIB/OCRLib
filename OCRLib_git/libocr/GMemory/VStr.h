#ifndef VSTR_H
#define VSTR_H
#include "config.h"
//#include "MmapFile.h"
#include "Vector.h"
#include <stddef.h> // for size_t
#include <stdlib.h>

using namespace std;
namespace ocr {
    
    /**Класс VStr одномерный вектор. На его основе работает построение
     таблиц и деревьев в Vector. VStr может располагатся как в памяти,
     так и в записи Vector. И в том и в другом случае VStr сериализован и расположен в непрерывном участке памяти по адресу data. При необходимости VStr запрашивает у родительского вектора расширения выделенного места. Для доступа к данным VStr предоставляет указатель на свои данные*/
    template<typename T>
    class VStr{
    protected:
        char *data;                     //указатель на буфер внутренних данных
        Vector *parent;
        uint *innerData;		  //массив размещенный в начале файла, в который записываются внутренние переменные
        /// внутренние переменные
        MemoryFile *dataMFile;
    public:
        VStr(void); 
        VStr(int sizePool);
        VStr(Vector &parentVector, uint childID);
        VStr(Vector &parentVector, cstr name);
        
        virtual ~VStr();
        void free();
        
        //VStr(cstr path);
        T *dataT;                       //указатель на буфер данных <typename T>
        uint *recordCount;              //количество записей в векторе
        uint *dataSize;                 //размер занимаемый актуальными данными
        uint *poolSize;                 //общий размер файла вектора
        uint *vectorID;                 //твердая копия индекса записи, в которой VStr размещен в родительском векторе
        uint dataLocation;
        uint vectorID_;                 //индех записи, в которой VStr размещен в родительском векторе
        
        void init(void* ptr);
        void reloadPtr();
        void resize(uint size);         //изменение размера массива с инициализацией объектов
        void resizePtr(uint size);      //изменение размера массива без инициализации объектов
        void reload();
        void read(cstr path);
        void save(cstr path);
        
        void push_back(T dat);
        void push_backT(T &dat);
        void push_Ptr(void *dat, uint size);
        void push_back(string &dat);
        
        T* getPtr(uint indexRecord, uint size);
        void put(uint indexRecord,T dat);
        void putT(uint indexRecord,T &dat);
        void putPtr(uint indexRecord, void *dat,  uint size);
        void pop_back();
        T& operator[] (uint id);
        T* operator() (uint id);
        void operator =(VStr<T> &ref);
        uint size(void){return *recordCount;};
        //получение указателя на актуальные данные
        char* dataPtr(){return data+24;};
    };
    
    template<typename T>
    VStr<T>::VStr(){
        //подключаем или создаем файл данных
        data=(char*)malloc(POOL_SIZE);
        dataT=(T*)(data+24);
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
    VStr<T>::VStr(int sizePool){
        //подключаем или создаем файл данных
        data=(char*)calloc(sizePool,1);
        dataT=(T*)(data+24);
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
    VStr<T>::VStr(Vector &parentVector, uint childID){
        //запрашиваем у родительского Vector указатель на файл данных
        parent=&parentVector;
        data=(char*)parentVector.getVector(childID);
        dataT=(T*)(data+24);
        //проверяем есть ли данные в VStr
        innerData=(uint*)(data);
        reloadPtr();
        if(innerData[0]!=0xffffffff){ 	 	 	 	//маркер наличия VStr в записи Vector
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
    VStr<T>::VStr(Vector &parentVector, cstr name){
        //запрашиваем у родительского Vector указатель на файл данных
        //если запись с таким именем уже существует, получаем на нее указатель
        parent=&parentVector;
        data=parentVector.setVector(name,&vectorID_);
        dataT=(T*)(data+24);
        //проверяем есть ли данные в VStr
        innerData=(uint*)(data);
        reloadPtr();
        if(innerData[0]!=0xffffffff){ 	 	 	 	//маркер наличия VStr в записи Vector
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
    void VStr<T>::VStr::init(void *ptr){
        data=(char*)ptr;
        innerData=(uint*)(data);
        reloadPtr();
    }
    
    template<typename T>
    VStr<T>::~VStr(){
    }
    
    template<typename T>
    void VStr<T>::free(){
        cout<<"*recordCount="<<*recordCount<<endl;
        if (dataLocation==MEMORY_LOCATION&&data!=NULL) std::free(data);
        //delete this;
    }
    
    /**восстановление указателей на внутренние переменные после изменения размера или расположения в памяти*/
    template<typename T>
    void VStr<T>::VStr::reloadPtr(){
        poolSize=&innerData[1];                     //размер файла вектора
        recordCount=&innerData[2];  	            //количество записей в векторе
        dataSize=&innerData[3];  	 	            //размер занимаемый актуальными данными
        vectorID=&innerData[4];
        dataT=(T*)(data+24);
    }
    /**восстановление указателей на внутренние переменные после изменения размера или расположения в памяти родительского вектора*/
    template<typename T>
    void VStr<T>::VStr::reload(){
        //запрашиваем у родительского Vector указатель на файл данных
        //если запись с таким именем уже существует, получаем на нее указатель
        data=(char*)parent->getVector(vectorID_);
        dataT=(T*)(data+24);
        //проверяем есть ли данные в VStr
        innerData=(uint*)(data);
        reloadPtr();
        if(innerData[0]!=0xffffffff){ 	 	 	 	//маркер наличия VStr в записи Vector
            // новый вектор
            innerData[0]=0xffffffff;
            *poolSize=POOL_SIZE;
            *recordCount=0;
            *dataSize=24;
            *vectorID=vectorID_;
        }
    }
    
    template<typename T>
    void VStr<T>::resize(uint newSize){
        if(*recordCount<newSize){
            uint count=newSize-*recordCount;
            T str;
            for(int i=0;i<count;i++){
                push_back(str);
            }
        }else{
            uint newSizeByte=(uint)(newSize*sizeof(T));
            *recordCount=newSize;
            *dataSize=newSizeByte+24;
        }
    }
    
    template<typename T>
    void VStr<T>::resizePtr(uint newSize){
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
    void VStr<T>::pop_back(){
        if(!*recordCount)return;
        *recordCount=*recordCount-1;
        *dataSize-=(int)sizeof(T);
    };
    
    
    template<typename T>
    void VStr<T>::save(cstr path){
        char *data_=data+24;
        dataMFile=MemoryFile::create(path,MemoryFile::if_exists_keep_if_dont_exists_create);
        dataMFile->resize(size()*sizeof(T));
        char *dataDest=dataMFile->data();
        memcpy(dataDest,data_,size()*sizeof(T));
        //cout<<" save1 dataMFile->size()="<<dataMFile->size()<<" size()="<<size()*sizeof(T)<<endl;
        
    };
    
    template<typename T>
    void VStr<T>::read(cstr path){
        //подключаем или создаем файл данных
        dataMFile=MemoryFile::create(path,MemoryFile::if_exists_keep_if_dont_exists_create);
        char *textBuffer=dataMFile->data();
        push_Ptr(textBuffer,dataMFile->size()-24);
        //cout<<" read dataMFile->size()="<<dataMFile->size()<<" size()="<<size()<<endl;
    };
    
    
    template<typename T>
    void VStr<T>::push_back(T dat){
        push_Ptr(&dat, sizeof(T));
    };
    template<typename T>
    void VStr<T>::push_backT(T &dat){
        push_Ptr(&dat, sizeof(T));
    };
    
    
    //получение данных из VStr
    template<typename T>
    T* VStr<T>::getPtr(uint indexRecord, uint size){
        return (T*)(data+indexRecord*size+24);
    };
    
    template<typename T>
    void VStr<T>::put(uint indexRecord,T dat){
        memcpy(data+indexRecord*sizeof(T)+24,&dat,sizeof(T));
    };
    
    template<typename T>
    void VStr<T>::putT(uint indexRecord,T &dat){
        memcpy(data+indexRecord*sizeof(T)+24,&dat,sizeof(T));
    };
    
    template<typename T>
    void VStr<T>::putPtr(uint indexRecord,void *dat,uint size){
        memcpy(data+indexRecord*size+24,&dat,size);
    };
    
    template<typename T>
    T& VStr<T>::operator[] (uint id){
        return dataT[id];
    };
    template<typename T>
    T* VStr<T>::operator() (uint id){
        return dataT+id;
    };
    
    template<typename T>
    void VStr<T>::operator =(VStr<T> &ref){
        resize(0);
        push_Ptr(ref.dataPtr(),ref.size()*sizeof(T));
    }
    
    //добавление новой записи в VStr
    template<typename T>
    void VStr<T>::push_Ptr(void *dat, uint size){
        //проверяем достаточно ли места для новой записи
        if(*poolSize<*dataSize+size){
            //cout<<"resize VStr. poolSize="<<*poolSize<<" new poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA<<" *vectorID="<<*vectorID<<endl;
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
        memcpy(data+*dataSize,dat,size); 	 	        //записываем данные
        *recordCount=(uint)(*recordCount+size/sizeof(T));
        *dataSize=*dataSize+size;
        //if(size==3)//cout<<"size="<<size<<" new dataSize="<<*dataSize<<" poolSize="<<*poolSize<<" recordCount="<<*recordCount<<endl;
        
    };
    
    template<typename T>
    void VStr<T>::push_back(string &dat){
        uint size=(uint)dat.size();
        //проверяем достаточно ли места для новой записи
        if(*poolSize<*dataSize+size){
            //cout<<"resize VStr. poolSize="<<*poolSize<<" new poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA<<" *vectorID="<<*vectorID<<endl;
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
        memcpy(data+*dataSize,(char*)&dat[0],size); 	 	        //записываем данные
        *recordCount=(uint)(*recordCount+size/sizeof(T));
        *dataSize=*dataSize+size;
        //if(size==3)//cout<<"size="<<size<<" new dataSize="<<*dataSize<<" poolSize="<<*poolSize<<" recordCount="<<*recordCount<<endl;
        
    }
    
    
    /**строка с размеченными подстроками используется как формат передачи данных
     в таблицах Vector. Записанную в Vector TStr можно только читать.
     в памяти стороку можно изменять и записывать в Vector новую стороку.
     Использует конструктор по умолчанию и не требует ручной деаллокации.*/
    
    class TStr{
    public:
        TStr();
        ~TStr();
        uint *index;   	   //указатель на индекс начальных позиций строк
        uint len;		   //длина индекса
        char *data;		   //указатель на данные
        uint sizeData;	 	   //размер данных
        VStr<char>dataStr;
        VStr<uint>indexStr;
        void push_back(char*ptr, uint size_);
        void push_back(TStr &str);
        void push_back(string &str);
        void push_back(const char* str);
        void push_TabStr(string &str);
        void setFromPtr(uint*index_,uint len_,char*data,uint dataSize);
        string operator [](uint i);
        void get(TStr &st, uint i);
        int readInt(uint i);
        void readStr(string &str, uint i);
        void readIntVector(vector<int>&intVector,uint i);
        void readCharVector(vector<char>&charVector,uint i);
        
        void operator +=(string &str);
        void operator +=(cstr str);
        void operator +=(uint i);
        uint size(){ return len;};
    };
    
}

#endif