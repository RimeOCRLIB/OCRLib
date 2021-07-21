#ifndef MMAP_GVECTOR_H
#define MMAP_GVECTOR_H
#include <stddef.h>  // for size_t
#include <stdlib.h>
#include "../OCRTypes/OCRTypes.h"
#include "../config.h"
#include "MmapFile.h"

using namespace std;
namespace ocr
{
#define INDEX_RESIZE 4  //во сколько раз будет увеличиваться размер индекса
#define FILE_RESIZE 2  //во сколько раз будет увеличиваться размер файла
#define POOL_SIZE 16384
////////////////////////////////////
class GVector;  // forward declaration
class TString;  // forward declaration
template<typename T>
class GStr;  // forward declaration

/**
 Класс GVector использует механизм файла виртуальной памяти (mmap) для организации массива данных
 переменной длинны в виде дискового файла с отображением в память.
 */
class GVector
{
protected:
    GVector(void);
    GVector(string &path);
    GVector(cstr path);
    GVector(GVector *ref);
    GVector(char *data_, int mode);
    GVector(GVector *parentVector, cstr name);
    void        init();
    void        init(string &path);
    void        init(GVector *ref);
    void        init(char *data_, int mode);
    void        init(GVector *parentVector, cstr name);
    string      dataPath;
    MemoryFile *dataMFile;
    GVector *   parent;

public:
    char * data;
    ulong *index;
    ulong *innerData;  //массив размещенный в начале файла, в который записываются внутренние
                       //переменные
                       /// внутренние переменные
    ulong *recordCount;      //количество записей в векторе
    ulong *recordArraySize;  //размер файла учета записей
    ulong *dataSize;  //размер занимаемый актуальными данными
    ulong *poolSize;  //общий размер файла вектора
    ulong *lockFlag;  //флаг блокировки чтения-записи
    ulong *
           indexOffset;  //адрес размещения массива индекса записей. отсчитывается от начала файла
    ulong *vectorID;
    ulong  vectorID_;
    int    dataLocation;

    virtual ~GVector();
    void            destroy(void);
    static GVector *create() {
        return new GVector();
    };
    static GVector *create(string &path) {
        return new GVector(path);
    };
    static GVector *create(GVector *ref) {
        return new GVector(ref);
    };
    static GVector *create(GVector *parentVector, cstr name) {
        return new GVector(parentVector, name);
    };
    static GVector *create(char *data_, int mode) {
        return new GVector(data_, mode);
    };
    static GVector *create(cstr path) {
        return new GVector(path);
    };
    /** resize GVector for new capacity*/
    void setSize(ulong newRecordCount, ulong datasize);  // datasize указывается в байтах.
    void resizeData(ulong newRecordCount,
                    ulong datasize);  // datasize указывается в мегабайтах.
    void resize(uint newRecordCount);  // recordCount - количество записей
    void clear();

    void reloadPtr();  //восстановление указателей на внутренние переменные после изменения
                       //размера или расположения в памяти
    void reload(GVector *p);  //восстановление указателей на внутренние переменные при
                              //изменении родительского вектора

    void push_back(string &str);
    void push_back(cstr str);
    void push_back(char *str, ulong size);
    void push_back(string &str, cstr name);
    void push_back(TString &str);
    void addRecords(GVector *ref);

    void *getPtr(ulong indexRecord, ulong &size);
    void  getStr(ulong indexRecord, string &str);
    void  getStr(ulong indexRecord, vector<uint> &str);
    void  getStr(ulong indexRecord, uint **p, uint &size);
    void  getStr(string &str, cstr name);
    int   getInt(ulong indexRecord);
    /**получение указателей на запись в виде токенизированной строки только для чтения */
    void getTStr(ulong indexRecord, TString &str);
    /**получение копии записи в виде копии токенизированной строки.
     данные можно перемещять и копировать. Перед чтением данных в случае их последующего
     копирования и перемещения после необходимо выполнить TString::reloadPtr() */
    void getTStrData(ulong indexRecord, TString &str);

    /**получение индекса именных записей*/
    void getName(string &str);

    void putPtr(ulong indexRecord, void *ptr, ulong size);
    void putStr(ulong indexRecord, string &str);
    void putStr(ulong indexRecord, vector<uint> &str);
    void putCStr(ulong indexRecord, cstr str);
    void putTStr(ulong indexRecord, TString &str);
    void putInt(ulong indexRecord, int data);
    /**запись индекса именных записей*/
    void putName(string &str);

    /**размещение именной записи при создании дочернего GStr. */
    char *setVector(cstr name, ulong &vectorID);
    /**возврат указателя на данные дочерний GStr или GVector по именной записи */
    void *getVectorByName(cstr name);
    /**возврат указателя на данные дочерний GStr или GVector по ID */
    void *getVector(ulong childID);
    /**возврат указателя на Ptr, дочерний GStr или GVector по имени */
    void *operator[](const char *name);

    /** импорт записей из XML, TXT, tab-separated TXT*/
    void import(string &path, int mode);
    /** импорт записей из TXT построчно по разделителю \n или \r*/
    void importTXT(string &path);
    /** импорт записей из TXT построчно по разделителю \n или \r с записью ключей в файл
     * path1*/
    void importDict(map<string, ulong> keyMap, string &path, string &path1);
    /** импорт записей из tab-separated TXT построчно по разделителю \n или \r*/
    void importTAB(string &path);
    /** импорт записей из XML */
    void importXML(string &path);
    /** экспорт всех записей формата TString*/
    void exportTStr(cstr path);
    /** экспорт всех записей формата TString*/
    void exportTStr(string &path);

    ulong data_size(void) {
        return *dataSize;
    };
    ulong size(void) {
        return *recordCount - 1;
    };
    //зaкрывает обращение к GVector
    void close(void);
    //переписывает данные, оптимизируя размещение
    bool save(void) {
        return 0;
    };
    //записывает данные в дисковый файл, оптимизируя размещение
    bool save(string &path) {
        return 0;
    };
    //записывает данные в дисковый файл, оптимизируя размещение
    bool save(cstr path) {
        return 0;
    };
    //экспорт в XML с рекурсивным выводом именных дочерних записей
    void   exportXML(string &path);
    string path() {
        string str=dataPath;
        return str;
    }
};

/**Класс GStr одномерный вектор. На его основе работает построение
 таблиц и деревьев в GVector. GStr может располагатся как в памяти,
 так и в записи GVector. И в том и в другом случае GStr сериализован и расположен в непрерывном
 участке памяти по адресу data. При необходимости GStr запрашивает у родительского вектора
 расширения выделенного места. Для доступа к данным GStr предоставляет указатель на свои
 данные*/
template<typename T>
class GStr
{
protected:
public:
    GStr(void);
    GStr(int sizePool);
    GStr(GVector *parentVector, cstr name);
    GStr(cstr path);
    void     init(void);
    void     init(int sizePool);
    void     init(GVector *parentVector, cstr name);
    void     init(cstr path);
    char    *data;
    GVector *parent;
    ulong   *innerData;  ///<массив размещенный в начале файла, в который записываются внутренние
                         ///<переменные
    MemoryFile *dataMFile;

    ulong *recordCount;  ///<количество записей в векторе
    ulong *dataSize;  ///<размер занимаемый актуальными данными
    ulong *poolSize;  ///<общий размер файла вектора
    ulong *vectorID;  ///<твердая копия индекса записи, в которой GStr размещен в родительском векторе
    ulong dataLocation;
    ulong vectorID_;  //индех записи, в которой GStr размещен в родительском векторе

    virtual ~GStr();
    void    destroy(void);
    void clear();   ///<erase all data
    static  GStr *create() {
        return new GStr();
    };
    static GStr *create(int sizePool) {
        return new GStr(sizePool);
    };
    static GStr *create(GVector *parentVector, cstr name) {
        return new GStr(parentVector, name);
    };
    static GStr *create(cstr path) {
        return new GStr(path);
    }
    void reloadPtr();
    void resize(ulong size);  // set new size, fill new data with zero, keep existing data
                              // below new size limit
    void reserve(ulong newSize);  // set new size, fill all data with zero
    void reload(GVector *p);
    void read(cstr path);
    void save(cstr path);
    void close();

    void push_back(T dat);
    void push_Ptr(void *dat, ulong size);

    T *   getPtr(ulong indexRecord, ulong size);
    void  put(ulong indexRecord, T dat);
    void  putPtr(ulong indexRecord, void *dat, ulong size);
    void  pop_back();
    T     get(ulong indexRecord);
    T &   operator[](ulong id);
    ulong size(void) {
        return *recordCount;
    };
    //получение указателя на актуальные данные
    T *dataPtr() {
        return (T *)(data + 48);
    };
    void erase(ulong id);
    void fill(T value);
};
template<typename T>
GStr<T>::GStr() {
    try {
        init();
    } catch(int a) { destroy(); }
}
template<typename T>
GStr<T>::GStr(int sizePool) {
    try {
        init(sizePool);
    } catch(int a) { destroy(); }
}
template<typename T>
GStr<T>::GStr(GVector *parentVector, cstr name) {
    try {
        init(parentVector, name);
    } catch(int a) { destroy(); }
}
template<typename T>
GStr<T>::GStr(cstr path) {
    try {
        init(path);
    } catch(int a) { destroy(); }
}

/**восстановление указателей на внутренние переменные после изменения размера или расположения
 * в памяти*/
template<typename T>
void GStr<T>::GStr::reloadPtr() {
    poolSize=&innerData[1];  //размер файла вектора
    recordCount=&innerData[2];  //количество записей в векторе
    dataSize=&innerData[3];  //размер занимаемый актуальными данными
    vectorID=&innerData[4];
}
/**восстановление указателей на внутренние переменные после изменения размера или расположения
 * в памяти родительского вектора*/
template<typename T>
void GStr<T>::GStr::reload(GVector *p) {
    //запрашиваем у родительского GVector указатель на файл данных
    //если запись с таким именем уже существует, получаем на нее указатель
    if(parent) {
        parent=p;
        data=(char *)parent->getVector(vectorID_);
        //проверяем есть ли данные в GStr
        innerData=(ulong *)(data);
        if(innerData[0] !=0xffffffffffffffff) {  //маркер наличия GStr в записи GVector
            cout<<"not valid GStr"<<endl;
            return;
            // новый вектор
            // innerData[0]=0xffffffff;
            //*poolSize=POOL_SIZE;
            //*recordCount=0;
            //*dataSize=48;
            //*vectorID=vectorID_;
        }
    }
    reloadPtr();
}

template<typename T>
void GStr<T>::GStr::init() {
    //подключаем или создаем файл данных
    data=(char *)calloc(POOL_SIZE, 1);
    innerData=(ulong *)(data);
    innerData[0]=0xffffffffffffffff;
    reloadPtr();
    *poolSize=POOL_SIZE;
    *recordCount=0;
    *dataSize=48;
    *vectorID=0xffffffffffffffff;
    vectorID_=*vectorID;
    dataLocation=MEMORY_LOCATION;
}
template<typename T>
void GStr<T>::GStr::init(int sizePool) {
    //подключаем или создаем файл данных
    data=(char *)calloc(POOL_SIZE, 1);
    innerData=(ulong *)(data);
    innerData[0]=0xffffffffffffffff;
    reloadPtr();
    *poolSize=sizePool;
    *recordCount=0;
    *dataSize=48;
    *vectorID=0xffffffffffffffff;
    vectorID_=*vectorID;
    dataLocation=MEMORY_LOCATION;
}

template<typename T>
void GStr<T>::init(GVector *parentVector, cstr name) {
    //запрашиваем у родительского GVector указатель на файл данных
    //если запись с таким именем уже существует, получаем на нее указатель
    parent=parentVector;
    data=parentVector->setVector(name, vectorID_);
    //проверяем есть ли данные в GStr
    innerData=(ulong *)(data);
    reloadPtr();
    if(innerData[0] !=0xffffffffffffffff) {  //маркер наличия GStr в записи GVector
                                              // новый вектор
        innerData[0]=0xffffffffffffffff;
        *poolSize=POOL_SIZE;
        *recordCount=0;
        *dataSize=48;
    }
    *vectorID=vectorID_;
    dataLocation=parentVector->dataLocation;
    // cout<<"magicNum="<<innerData[0]<<endl;
    // cout<<"poolSize="<<*poolSize<<endl;
    // cout<<"recordCount="<<*recordCount<<endl;
    // cout<<"dataSize="<<*dataSize<<endl;
    // cout<<"vectorID="<<*vectorID<<endl;
}

template<typename T>
void GStr<T>::init(cstr path) {
    parent=0;
    //подключаем или создаем файл данных
    dataMFile=MemoryFile::create(path, MemoryFile::if_exists_keep_if_dont_exists_create);
    if(dataMFile->size()==0) {  //новый файл GVector
        dataMFile->resize(POOL_SIZE);
        data=dataMFile->data();
        innerData=(ulong *)(data);
        reloadPtr();
        innerData[0]=0xffffffffffffffff;
        *poolSize=POOL_SIZE;
        *recordCount=0;
        *dataSize=48;
    } else {
        data=dataMFile->data();
        //проверяем есть ли данные в GStr
        innerData=(ulong *)(data);
        reloadPtr();
        if(innerData[0] !=0xffffffffffffffff) {  //маркер наличия GStr в записи GVector
                                                  // новый вектор
            innerData[0]=0xffffffffffffffff;
            *poolSize=POOL_SIZE;
            *recordCount=0;
            *dataSize=48;
        }
    }
    *vectorID=vectorID_=0xffffffffffffffff;
    dataLocation=MMAP_LOCATION;
    // cout<<"magicNum="<<innerData[0]<<endl;
    // cout<<"poolSize="<<*poolSize<<endl;
    // cout<<"recordCount="<<*recordCount<<endl;
    // cout<<"dataSize="<<*dataSize<<endl;
    // cout<<"vectorID="<<*vectorID<<endl;
}

template<typename T>
void GStr<T>::close() {
    if(dataLocation==MMAP_LOCATION) dataMFile->flush();
}

template<typename T>
void GStr<T>::resize(ulong newSize) {
    ulong poolSize_=*poolSize;
    ulong newSizeByte=(ulong)(newSize * sizeof(T)) + 48;
    if(poolSize_ < newSizeByte) {
        newSizeByte = (sizeof(T) * newSize + poolSize_) * 1.5;
        if(vectorID_==0xffffffffffffffff) {
            if(dataLocation==MMAP_LOCATION) {
                dataMFile->resize(newSizeByte);
                data=dataMFile->data();
                poolSize_=newSizeByte;
            } else {
                char *data_=(char *)calloc(newSizeByte, 1);
                memcpy(data_, data, *dataSize);
                free(data);
                data=data_;
                poolSize_=newSizeByte;
            }
        } else {
            string str;
            str.resize(newSizeByte);
            memcpy(&str[0], data, *poolSize);
            parent->putStr(vectorID_, str);
            data=(char *)parent->getPtr(vectorID_, newSizeByte);
            poolSize_=newSizeByte;
            // cout<<"1@new size="<<newSize<<endl;
        }
        innerData=(ulong *)(data);
        reloadPtr();
        *poolSize=poolSize_;
        *vectorID=vectorID_;
    }
    //если нужен resize() без изменения числа записей лучше написать его отдельной функцией
    *recordCount=newSize;
    *dataSize=newSize * sizeof(T) + 48;
    // cout<<"1@new recordCount="<<*recordCount<<endl;

    // cout<<"poolSize="<<*poolSize<<endl;
    // cout<<"recordCount="<<*recordCount<<endl;
    // cout<<"dataSize="<<*dataSize<<endl;
    // cout<<"vectorID="<<*vectorID<<endl;
}

template<typename T>
void GStr<T>::reserve(ulong newSize) {
    resize(newSize);
    memset(data + 48, 0, *dataSize - 48);
}

template<typename T>
void GStr<T>::pop_back() {
    if(!*recordCount) return;
    *recordCount=*recordCount - 1;
    *dataSize -=(int)sizeof(T);
};

template<typename T>
GStr<T>::~GStr() {
    if(dataLocation==MEMORY_LOCATION) free(data);
}

template<typename T>
void GStr<T>::destroy(void) {
    if(dataLocation==MEMORY_LOCATION) free(data);
    dataLocation=0;
    delete this;
};

template<typename T>
void GStr<T>::clear(void) {
	*recordCount = 0;
	*dataSize = 0;
}

template<typename T>
void GStr<T>::save(cstr path) {
    char *data_=data + 48;
    dataMFile=MemoryFile::create(path, MemoryFile::if_exists_keep_if_dont_exists_create);
    dataMFile->resize(size() * sizeof(T));
    char *dataDest=dataMFile->data();
    memcpy(dataDest, data_, size() * sizeof(T));
    // cout<<" save1 dataMFile->size()="<<dataMFile->size()<<"
    // size()="<<size()*sizeof(T)<<endl;
};

template<typename T>
void GStr<T>::read(cstr path) {
    //подключаем или создаем файл данных
    dataMFile=MemoryFile::create(path, MemoryFile::if_exists_keep_if_dont_exists_create);
    char *textBuffer=dataMFile->data();
    push_Ptr(textBuffer, dataMFile->size() - 48);
    // cout<<" read dataMFile->size()="<<dataMFile->size()<<" size()="<<size()<<endl;
};

template<typename T>
void GStr<T>::push_back(T dat) {
    ulong size=*recordCount;
    resize(size + 1);
    T* p=(T*)(data + 48);
    p[size]=dat;
    *recordCount=size + 1;
};

//получение данных из GStr
template<typename T>
T *GStr<T>::getPtr(ulong indexRecord, ulong size) {
    return (T *)(data + indexRecord * size + 48);
};

template<typename T>
void GStr<T>::put(ulong indexRecord, T dat) {
    memcpy(data + indexRecord * sizeof(T) + 48, &dat, sizeof(T));
};

template<typename T>
void GStr<T>::putPtr(ulong indexRecord, void *dat, ulong size) {
    memcpy(data + indexRecord * size + 48, &dat, size);
};

template<typename T>
T &GStr<T>::operator[](ulong id) {
    T *p=(T*)(data + 48);
    return (T &)*(p + id);
};

template<typename T>
T GStr<T>::get(ulong id) {
    T *p=(T*)(data + 48);
    return (T &)*(p + id);
}

//добавление новой записи в GStr
template<typename T>
void GStr<T>::push_Ptr(void *dat, ulong size) {
    //проверяем достаточно ли места для новой записи
    if(*poolSize < *dataSize + size) {
        // cout<<"resize GStr. poolSize="<<*poolSize<<" new
        // poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE<<" *vectorID="<<*vectorID<<"
        // count:"<<*recordCount<<endl;
        ulong newSize=(*poolSize) * FILE_RESIZE + size * FILE_RESIZE;
        if(*vectorID==0xffffffffffffffff) {
            if(dataLocation==MMAP_LOCATION) {
                dataMFile->resize(newSize + 48);
                data=dataMFile->data();
            } else {
                char *data_=(char *)calloc(newSize + 48, 1);
                // cout<<"new size:"<<newSize<<" dataSize"<<*dataSize<<endl;
                memcpy(data_, data, *dataSize);
                free(data);
                data=data_;
            }
        } else {
            string str;
            str.resize(newSize);
            memcpy(&str[0], data, *poolSize);
            parent->putStr(vectorID_, str);
            data=(char *)parent->getPtr(vectorID_, newSize);
            // cout<<"@new size GStr:"<<newSize<<endl;
        }
        innerData=(ulong *)(data);
        reloadPtr();
        *poolSize=newSize;
    }
    //записываем данные
    memcpy(data + *dataSize, dat, size);
    *recordCount=(ulong)(*recordCount + size/sizeof(T));
    *dataSize +=size;
    // if(size==3)//cout<<"size="<<size<<" new dataSize="<<*dataSize<<"
    //poolSize="<<*poolSize<<" recordCount="<<*recordCount<<endl;
};

template<typename T>
void GStr<T>::erase(ulong id) {
    size_t ind=*recordCount - 1;
    *recordCount=ind;
    size_t  s_=sizeof(T);
    char *p=data + 48;
    //size_t size_=(*recordCount-id)*s_;
    //char *pp=(char*)malloc(size_);
    //memcpy(pp,(p+id*s_+s_),(*recordCount-id-1)*s_);
    //memcpy((p+id*s_),pp,(*recordCount-id-1)*s_);
    //free(pp);
    for(size_t i=id; i < ind; i++) {
        *((T*)(p + i * s_))=*((T*)(p + (i + 1) * s_));
    }
}

template<typename T>
void GStr<T>::fill(T value){
    int sizeT=sizeof(T);
    char *data_=data + 48;
    int recordCount_=*recordCount;
    for(int i=0;i<recordCount_;i++){
        *data_=value;
        data_+=sizeT;
    }
}

////////////////////////////////////
/////////////GStr2D/////////////////
////////////////////////////////////
    
template<typename T>
class GStr2D: public GStr<T>
{
public:
    T *   dataT;
    uint *index;

    GStr2D(void);
    void           destroy(void);
    static GStr2D *create() {
        return new GStr2D();
    };
    void init();
    void setIndex(uint *indexV, uint size);
    uint size(uint i) {
        return index[*(GStr<T>::recordCount) + i];
    };
    T *operator[](uint index_);
};

template<typename T>
GStr2D<T>::GStr2D() {
    try {
        init();
    } catch(int a) { destroy(); }
}

template<typename T>
void GStr2D<T>::GStr2D::init() {
    //подключаем или создаем файл данных
    GStr<T>::init();
}

template<typename T>
void GStr2D<T>::setIndex(uint *indexV, uint size) {
    uint sizeData=0;
    for(int i=0; i < size; i++) {
        sizeData +=indexV[i];
    }
    int sizeT=sizeof(T);
    sizeT==0 ? size=1 : 0;
    GStr<T>::resize(sizeData + size * 2/(sizeT / 8));
    index=(uint *)(GStr<T>::data + 48);
    dataT=(T *)(GStr<T>::data + 48 + (*GStr<T>::recordCount) * 8);

    *GStr<T>::recordCount=size;
    uint dataOffset=0;
    dataOffset=0;
    for(int i=0; i < size; i++) {
        index[i]=dataOffset;
        dataT[dataOffset]=0;  //записываем длину строки
        dataOffset +=indexV[i] + 1;
    }
}

template<typename T>
T *GStr2D<T>::operator[](uint index_) {
    return (T *)(dataT + index[index_]);
}

template<typename T>
void GStr2D<T>::destroy(void) {
    GStr<T>::destroy();
    delete this;
};

/**строка с размеченными подстроками используется как формат передачи данных
 в таблицах GVector. Записанную в GVector TString можно только читать.
 в памяти стороку можно изменять и записывать в GVector новую стороку.
 Использует конструктор по умолчанию и не требует ручной деаллокации.*/

class TString
{
public:
    TString();
    ~TString();
    uint *index;  //указатель на индекс начальных позиций строк
    uint         len;       //длина индекса
    char *       data;      //указатель на данные
    uint         sizeData;  //размер данных
    vector<char> dataStr;
    vector<uint> indexStr;
    void         push_back(char *ptr, uint size_);
    void         push_back(TString &str);
    void         push_back(string &str);
    void         push_back(const char *str);
    void         push_back(uint intData);
    void         push_backL(ulong intData);
    void         push_TabStr(string &str);
    void         setFromPtr(uint *index_, uint len_, char *data, uint dataSize);
    string       operator[](uint i);
    void         getPtr(char **p, uint i, int *size);
    void         get(TString *st, uint i);
    int          getInt(uint i);
    ulong        getLong(uint i);
    void         getStr(string &str, uint i);
    void         getStr(char **p, uint i);
    void         getIntVector(vector<int> &intVector, uint i);
    void         getCharVector(vector<char> &charVector, uint i);
    void         operator+=(string &str);
    void         operator+=(cstr str);
    void         operator+=(uint i);
    uint         size() { return len; }
    void save();  //запись данных из указателя на данные data во внутренний контейнер данных -
                  //dataStr. После этого TString можно копировать
    void reloadPtr();  //восстановление указателей на данные после записи в vector и других
                       //перемещений данных
    void exportHex(string &str);
};

////////////////////////////////////
}  // namespace ocr

#endif
