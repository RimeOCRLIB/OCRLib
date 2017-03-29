#include "GVector.h"
#include "php2stl.h"
#include <malloc/malloc.h>

using namespace std;
using namespace ocr;

GVector::GVector(void){
	try{
        init();
    }catch(int a){
        destroy();
    }
};
GVector::GVector(string &path){
    try{
        init(path);
    }catch(int a){
        destroy();
    }
}

GVector::GVector(const char* path){
    string pathStr=path;
    try{
        init(pathStr);
    }catch(int a){
        destroy();
    }
}

GVector::GVector(char* data_,int mode){
    try{
        init(data_,mode);
    }catch(int a){
        destroy();
    }
}

GVector::GVector(GVector* parentVector, cstr name){
    try{
        init(parentVector,name);
    }catch(int a){
        destroy();
    }
}
/**восстановление указателей на внутренние переменные после изменения размера или расположения в памяти*/
void GVector::reloadPtr(){
	poolSize		=&innerData[1];           //размер файла вектора
    recordCount		=&innerData[2];  	      //количество записей в векторе
    recordArraySize	=&innerData[3];           //размер файла учета записей
    dataSize		=&innerData[4];  	 	  //размер занимаемый актуальными данными
    lockFlag        =&innerData[5];           //флаг блокировки чтения-записи
    indexOffset		=&innerData[6];	 	 	  //адрес размещения массива индекса записей. отсчитывается от начала файла
    vectorID		=&innerData[7]; 	 	  //индех записи, в которой GVector размещен в родительском векторе
    index=(ulong*)(data+*indexOffset);  //инициализация индекса
    
};

void GVector::reload(GVector *p){
    if(parent!=0){
        //запрашиваем у родительского GVector указатель на файл данных
        //если запись с таким именем уже существует, получаем на нее указатель
        parent=p;
        data=(char*)parent->getVector(vectorID_);
        //проверяем есть ли данные в GVector
        innerData=(ulong*)(data);
        if(innerData[0]!=0xfffffffffffffffa){
            cout<<"no valid GVector innerData[0]:"<<hex<<innerData[0]<<endl;
            return;
        }
    }
    reloadPtr();
}

/**инициализация GVector с размещением в оперативной памяти*/
void GVector::GVector::init(){
    data=new char[POOL_SIZE];
    dataLocation=MEMORY_LOCATION; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
    innerData=(ulong*)(data);
    innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
    reloadPtr();
    *poolSize=POOL_SIZE;
    *recordArraySize=1024;
    *recordCount=0;
    *dataSize=(*recordArraySize)*8+128;
    *indexOffset=128;
    *vectorID=0xffffffff;
     vectorID_=*vectorID;
    *lockFlag=0;
    string indexName="name:|:data";
    index=(ulong*)(data+*indexOffset);  //инициализация индекса
	push_back(indexName); 	 	 	 	 	    //инициализация индекса именных записей
}

void GVector::init(string &path){
    //подключаем или создаем файл данных
    dataMFile=MemoryFile::create(path.c_str(),MemoryFile::if_exists_keep_if_dont_exists_create);
    data=dataMFile->data();
    dataPath=path;
    dataLocation=MMAP_LOCATION; 	 		 	  //флаг размещения данных в оперативной  или  отображаемой памяти
    parent=0;
    //cout<<" dataMFile->size()="<<dataMFile->size()<<endl;
    //проверяем является ли файл файлом GVector
    if(dataMFile->size()==0){//новый файл GVector
        dataMFile->resize(POOL_SIZE);
        data=dataMFile->data();
        innerData=(ulong*)(data);
        innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
        reloadPtr();
        *poolSize=(ulong)dataMFile->size();
        *recordArraySize=1024;
        *recordCount=0;
        *dataSize=(*recordArraySize)*8+128;
        *indexOffset=128;
        *vectorID=0xfffffffffffffffa;
         vectorID_=*vectorID;
        *lockFlag=0;
        string indexName="name:|:data";
        index=(ulong*)(data+*indexOffset);  //инициализация индекса
    	push_back(indexName); 	 	 	 	 	    //инициализация индекса именных записей
        *recordCount=1;
        
    }else{
        //проверяем является ли файл файлом GVector
        innerData=(ulong*)(data);
        if(innerData[0]!=0xfffffffffffffffa){
            cout<<"no valid GVector file "<<hex<<innerData[0]<<path;
            return;
        }
        reloadPtr();
        index=(ulong*)(data+*indexOffset);  //инициализация индекса
        *poolSize=(ulong)dataMFile->size();
        
    }
    reloadPtr();
    vectorID_=*vectorID;
    
    //cout<<"innerData="<<*innerData<<endl;
    //cout<<"poolSize="<<*poolSize<<endl;
    //cout<<"recordCount="<<*recordCount<<endl;
    //cout<<"recordArraySize="<<*recordArraySize<<endl;
    //cout<<"dataSize="<<*dataSize<<endl;
    //cout<<"emptyRecordCount="<<*emptyRecordCount<<endl;
    //cout<<"indexOffset="<<*indexOffset<<endl;
}

/**инициализация GVector указателем на буфер данных GVector*/
void GVector::GVector::init(char* data_,int mode){
    data=data_;
    parent=0;
    dataLocation=mode; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
    innerData=(ulong*)(data);
    innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
    reloadPtr();
    *lockFlag=0;
    vectorID_=*vectorID;
}

/**инициализация GVector копированием с размещением в оперативной памяти*/ /// ####
void GVector::GVector::init(GVector* ref){
    /*
     if(!ref->data_size())return;
     destroy();
     data=new char[ref->size()];
     dataLocation=MEMORY_LOCATION; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
     memcpy(data,ref->dataPtr(),ref->data_size());
     innerData=(uint*)(data);
     innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
     reloadPtr();
     */
    
}

void GVector::init(GVector* parentVector, cstr name){
    //запрашиваем у родительского GVector указатель на файл данных
    //если запись с таким именем уже существует, получаем на нее указатель
    parent=parentVector;
    data=parentVector->setVector(name,vectorID_);  //cout<<"#1vectorID_="<<vectorID_<<endl;
    //проверяем является ли файл файлом GVector
    innerData=(ulong*)(data);
    if(innerData[0]!=0xfffffffffffffffa){//новый GVector
        //cout<<"new GVector"<<endl;
        innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
        reloadPtr();
        *poolSize=POOL_SIZE;
        *recordArraySize=1024;
        *recordCount=0;
        *dataSize=(*recordArraySize)*8+128;
        *indexOffset=128;
        *vectorID=vectorID_;
        *lockFlag=0;
        string indexName="name:|:data";
        index=(ulong*)(data+*indexOffset);  //инициализация индекса
    	push_back(indexName); 	 	 	   //инициализация индекса именных записей
        
    }else{
        //проверяем является ли файл файлом GVector
        //cout<<"reload GVector"<<endl;
        innerData=(ulong*)(data);
        if(innerData[0]!=0xfffffffffffffffa){
            cout<<"no valid GVector file "; return;
        }
        reloadPtr();
        index=(ulong*)(data+*indexOffset);  //инициализация индекса
        *lockFlag=0;
        
    }
    /*
    cout_<<"innerData="<<*innerData<<endl;
    cout_<<"poolSize="<<*poolSize<<endl;
    cout_<<"recordCount="<<*recordCount<<endl;
    cout_<<"recordArraySize="<<*recordArraySize<<endl;
    cout_<<"dataSize="<<*dataSize<<endl;
    cout_<<"lockFlag="<<*lockFlag<<endl;
    cout_<<"indexOffset="<<*indexOffset<<endl;
    */
    
}
/*
void GVector::operator = (GVector *ref){

      uint vectorID_=*vectorID;
      uint newSize=*ref->poolSize;
      if(*vectorID==0xffffffff){
                if(dataLocation==MEMORY_LOCATION){
                    data=(char*)realloc(data,newSize);
                }else{
                    dataMFile->resize(newSize);
                    data=dataMFile->data();
                    newSize=(uint)dataMFile->size();
                }
	        
	  }else{
                string str;
                str.resize(newSize);
                memcpy(&str[0],data,*poolSize);
                parent->putStr(vectorID_,str);
                data=(char*)parent->getPtr(vectorID_,&newSize);
                //cout_<<"@new size="<<newSize<<endl;
	  }
            
    
      memcpy(data,(char*)ref->data,*ref->poolSize);
      innerData=(uint*)(data);
      innerData[0]=0xfffffffffffffffa; 	 	 	 	 	//маркер наличия GVector
      reloadPtr();
      *vectorID=vectorID_;
      index=(uint*)(data+*indexOffset);  //инициализация индекса
      
  
}
*/

void GVector::addRecords(GVector *ref){
    //cout<<"ref->size()="<<ref->size()<<" path="<<ref->path()<<endl;
    //cout<<" self path="<<path()<<endl;
    ulong size=ref->size();
    for(ulong i=0;i<size;i++){
        TString str;
        ref->getTStr(i,str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
        push_back(str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
        getTStr(i,str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
    }
    //cout<<"done"<<endl;
}

GVector::~GVector(){}

void GVector::destroy(void){
	if (dataLocation==MEMORY_LOCATION&&data!=NULL){
        free(data);
    }else{
        delete dataMFile;
    }
    delete this;
};

//добавление новой записи в GVector
void GVector::push_back(string&str){
    ulong size=str.size();
    //data=dataMFile->data();
    //string dt; dt.resize(20);
    //memcpy(&dt[0],data,20);
    //cout<<" poolSize="<<*poolSize<<" dataSize="<<*dataSize<<endl;
    
    //проверяем достаточно ли места в массиве индекса
    if(*recordArraySize<*recordCount+1){
        cout<<"resize GVector index"<<" recordArraySize="<<*recordArraySize<<
        " recordCount="<<*recordCount<<" *poolSize="<<*poolSize/1000000<<endl;
        setSize((*recordArraySize)*INDEX_RESIZE, *poolSize);
    }
    //cout<<" poolSize1="<<*poolSize<<" dataSize="<<*dataSize<<endl;
    //проверяем достаточно ли места для новой записи
    
    if(*poolSize<*dataSize+size+256){
        ulong newSize=(*poolSize)*FILE_RESIZE;
        if(newSize<*dataSize+size+256)newSize=(*dataSize+size+256)*1.5;
        cout<<"@resize GVector file new poolSize="<<newSize/1000000<<" Mb c3"<<endl;
        setSize(*recordArraySize, newSize);
        
    }
    //записываем данные
    index[*recordCount]=*dataSize;	    	    //записываем адрес записи в индекс
    memcpy(data+*dataSize,&size,8); 	 	        //записываем длинну записи
    memcpy(data+*dataSize+8,&str[0],size);       //копируем запись
    //cout<<" *recordCount:"<<*recordCount<<" *dataSize:"<<*dataSize<<" *recordArraySize:"<<*recordArraySize<<endl;
    *recordCount=*recordCount+1;
    *dataSize=*dataSize+size+8;
    
    ///cout_<<"new data"<<endl;
    //cout_<<"innerData="<<*innerData<<endl;
    //cout_<<"innerData="<<*innerData<<endl;
    //cout_<<"poolSize="<<*poolSize<<endl;
    //cout_<<"recordCount="<<*recordCount<<endl;
    //cout_<<"recordArraySize="<<*recordArraySize<<endl;
    //cout_<<"dataSize="<<*dataSize<<endl;
    //cout_<<"emptyRecordCount="<<*emptyRecordCount<<endl;
    //cout_<<"indexOffset="<<*indexOffset<<endl;
    //cout_<<"index="<<index[247]<<index[248]<<index[249]<<index[250]<<index[251]<<endl;
    
    
};

/** resize GVector for new capacity*/
void  GVector::resizeData(ulong newRecordCount, ulong newSize){
    newSize=newSize*1024000;
    setSize(newRecordCount,newSize);
}

void GVector::setSize(ulong newRecordCount, ulong newSize){//datasize указывается в байтах.
    if(newSize<newRecordCount*8+*poolSize)newSize=newRecordCount*8+*poolSize;
    //cout<<"*poolSize="<<*poolSize/1000000<<" newSize="<<newSize/1000000<<" *dataSize="<<*dataSize/1000000<<" newRecordCount"<<newRecordCount*4/1000000<<endl;
    if(newSize<=*poolSize)return;
    //cout_<<"@resize GVector file new poolSize="<<newSize<<endl;
    //cout<<"#4vectorID_="<<hex<<vectorID_<<endl;
    if(vectorID_==0xfffffffffffffffa){
        if(dataLocation==MEMORY_LOCATION){
            data=(char*)realloc(data,newSize);
        }else{
            dataMFile->resize(newSize);
            data=dataMFile->data();
            newSize=(ulong)dataMFile->size();
        }
    }else if(newSize>*poolSize) {
        string str;   //нужно переписать для более прямого копирования данных в родительский вектор
        str.resize(newSize);
        memcpy(&str[0],data,*poolSize);
        parent->putStr(vectorID_,str);
        data=(char*)parent->getPtr(vectorID_,&newSize);
        //cout_<<"@new size="<<newSize<<endl;
    }
    innerData=(ulong*)(data);
    reloadPtr();
    *poolSize=newSize;
    if(newRecordCount>*recordCount){
        if(*recordArraySize<newRecordCount){
            //cout_<<"копируем индех";
            //копируем индех
            memcpy(data+*dataSize,data+*indexOffset,(*recordArraySize)*8);
            *recordArraySize=newRecordCount;
            *indexOffset=*dataSize;
            index=(ulong*)(data+*indexOffset);
            *dataSize+=*recordArraySize*8;
            //cout_<<" *dataSize="<<*dataSize<<" index="<<index[1]<<endl;
        }
    }
};


/** resize GVector for new record count*/
void  GVector::resize(uint newRecordCount){
    //cout<<"*poolSize="<<*poolSize<<" newRecordCount "<<newRecordCount<<endl;
    if(*recordCount<newRecordCount){
        ulong newSize=(newRecordCount*16+*poolSize);
        cout<<"@resize GVector file new poolSize Mb:"<<newSize/1000000<<endl;
        setSize(newRecordCount,newSize);
        ulong rStart=*recordCount;
        memset((char*)(index+rStart),0,(newRecordCount-rStart)*8);   //обнуляем индекс
        //cout<<" *dataSize="<<*dataSize<<" *poolSize="<<*poolSize<<endl;
    }
    *recordCount=newRecordCount+1;
    
};


/** clear GVector*/
void  GVector::clear(){    
        *recordCount=0;
        *dataSize=0;
};

void GVector::push_back(const char*str){
    string str_=str;
    push_back(str_);
};
void GVector::push_back(char*str, ulong size){
    string str_;
    str_.resize(size);
    memcpy(&str_[0],str,size);
    push_back(str_);
};
void GVector::push_back(string&str,cstr name){
    push_back(str);
    string strName;
    getName(strName);
    strName+=":|:"; strName+=name; strName+=":|:";
    strName.resize(strName.size()+8);
    ulong n=*recordCount-1;
    memcpy(&strName[0]+strName.size()-8,&n,8);
    putName(strName);
    
};

void GVector::push_back(TString &st){
    string str;
    str.resize(st.sizeData+st.len*4+8+st.sizeData*0.15);
    memcpy(&str[0],(char*)&st.len,4);
    //cout<<" st.len="<<st.len<<" st.sizeData="<<st.sizeData<<endl;
    memcpy(&str[4],(char*)st.index,st.len*4+4);
    memcpy(&str[st.len*4+8],st.data,st.sizeData);
    push_back(str);
}


//получение данных из GVector
void* GVector::getPtr(ulong indexRecord,ulong *size){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return NULL; //не инициализированая запись
    if(indexRecord>*recordCount){*size=0;return NULL;}
    *size=*((ulong*)(data+index[indexRecord]));
    if(*size==0){return NULL;}
    //cout<<"*size="<<*size<<endl;
    return (void*)(data+index[indexRecord]+8);
};

void GVector::getStr(ulong indexRecord, string&str){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return; //не инициализированая запись
    ulong *size;
    size=(ulong*)(data+index[indexRecord]);
    if(*size+index[indexRecord]+8>*dataSize){
        cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<" *dataSize="<<*dataSize<<endl;
        cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
        cout<<"size="<<*size<<endl;
        cout<<"size out of range"; return;
    }
    str.resize(*size);
    memcpy(&str[0],(data+index[indexRecord]+8),*size);
};

void GVector::getStr(ulong indexRecord, vector<uint>&str){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return; //не инициализированая запись
    ulong *size;
    //cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<" *dataSize="<<*dataSize<<endl;
    //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
    //if(indexRecord>20000000||indexRecord>*recordCount){cout<<indexRecord<<" size out of range1"<<endl; return;}
    //if(malloc_size(index)<indexRecord){cout<<malloc_size(index)<<" malloc error"<<endl; return;}
    //if(*dataSize==0){cout<<malloc_size(index)<<" malloc error"<<endl; return;}
    //if(index[indexRecord]>*dataSize){cout<<" indexRecord:"<<indexRecord<<" malloc error1"<<endl; return;}
    
    size=(ulong*)(data+index[indexRecord]);
    //if(*size+index[indexRecord]+8==0){cout<<" indexRecord:"<<indexRecord<<" *size:"<<*size<<" malloc error2"<<endl; return;}
    //cout<<"size="<<*size<<endl;
    
    //if(*size+index[indexRecord]+8>*dataSize){cout<<indexRecord<<" size out of range3"; return;}
    str.resize((*size)/4);
    memcpy(&str[0],(data+index[indexRecord]+8),*size);
    
};

void GVector::getStr(ulong indexRecord, uint **p, uint &size){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return; //не инициализированая запись
    //cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<" *dataSize="<<*dataSize<<endl;  
    //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
    //if(indexRecord>20000000||indexRecord>*recordCount){cout<<indexRecord<<" size out of range1"<<endl; return;}
    //if(malloc_size(index)<indexRecord){cout<<malloc_size(index)<<" malloc error"<<endl; return;}
    //if(*dataSize==0){cout<<malloc_size(index)<<" malloc error"<<endl; return;}
    if(index[indexRecord]>*dataSize){
        cout<<" indexRecord:"<<indexRecord<<" malloc error1"<<endl;
        return;
    }
    
    size=*((uint*)(data+index[indexRecord]));
    //if(*size+index[indexRecord]+8==0){cout<<" indexRecord:"<<indexRecord<<" *size:"<<*size<<" malloc error2"<<endl; return;}
    //cout<<"size="<<*size<<endl;
    
    //if(*size+index[indexRecord]+8>*dataSize){cout<<indexRecord<<" size out of range3"; return;}
    //str.resize((*size)/4);
    *p=(uint*)(data+index[indexRecord]+8);
    
};


void GVector::getStr(string&str,cstr name){
    ulong *size;
    string name_=name;
    ulong indexRecord=0;
    string strName; getName(strName);
    //cout<<"strName="<<strName<<endl;
    if(strName.size()<5){str="";return;}
    vector<string>indexName=explode(":|:",strName);
    //cout<<"indexName[a].size()="<<indexName.size()<<endl;
    
    for(int a=0; a<indexName.size()-1;a++){
        string st=indexName[a];
        if(indexName[a]==name_){
            //cout<<"name="<<indexName[a];
            memcpy(&indexRecord,&indexName[a+1][0],4);
            //cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<endl;
            //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
            break;
        }
    }
    if(indexRecord>*recordCount||indexRecord==0){str="";return;}
    size=(ulong*)(data+index[indexRecord]);
    if(*size==0){str="";return;}
    //cout<<"size="<<*size<<endl;
    str.resize(*size);
    memcpy(&str[0],(data+index[indexRecord]+4),*size);
    
};

int GVector::getInt(ulong indexRecord){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return 0; //не инициализированая запись
    ulong size;
    if(indexRecord>*recordCount){return NULL;}
    size=*(data+index[indexRecord]);
    if(size==0){return NULL;}
    //cout<<"*size="<<*size<<endl;
    return *(int*)(data+index[indexRecord]+8);
};

/**получение указателей на запись в виде токенизированной строки ТОЛЬКО ДЛЯ ЧТЕНИЯ! 
 нельзя записывать в вектор и перемещать данные */
void GVector::getTStr(ulong indexRecord,TString &str){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return; //не инициализированая запись
    if(indexRecord>*recordCount){
        cout<<" indexRecord:"<<indexRecord<<" *recordCount:"<<*recordCount<<endl;
        str.len=0;str.sizeData=0;return;
    }
    uint *ptr=(uint*)(data+index[indexRecord]);
    //cout<<"index["<<indexRecord<<"]:"<<index[indexRecord]<<endl;
    ptr+=2; //пропускаем длину записи записанную как ulong
    str.len=*(ptr); //cout<<" str.len="<<str.len;
    str.index=ptr+1; //cout<<" str->index[0]="<<str.index[0]<<" str.index[1]="<<str.index[1];
    str.data=(char*)(ptr+str.len+2); //cout<<" str.data[0]="<<str.data[0]<<endl;
    //str.sizeData=size_-str.len*4-12;
    str.sizeData=str.index[str.len];
};

/**получение копии записи в виде токенизированной строки.
данные можно перемещять и копировать. Перед чтением данных в случае их последующего
 копирования и перемещения после необходимо выполнить reloadPtr()*/
void GVector::getTStrData(ulong indexRecord,TString &str){
    indexRecord++;  //пропускаем индекс именных записей
    if(index[indexRecord]==0)return; //не инициализированая запись
    if(indexRecord>*recordCount){
        str.len=0;str.sizeData=0;
        cout<<" indexRecord:"<<indexRecord<<" *recordCount:"<<*recordCount<<endl;
        return;
    }
    uint *ptr=(uint*)(data+index[indexRecord]);
    ptr+=2;
    str.len=*(ptr); //cout<<" str->len="<<str->len;
    str.index=ptr+1; //cout<<" str->index[0]="<<str->index[0]<<" str->index[1]="<<str->index[1];
    str.indexStr.resize(str.len+1);
    memcpy((char*)&str.indexStr[0],(char*)str.index,str.len*4+4);
    str.data=(char*)(ptr+str.len+2); //cout<<" str->data[0]="<<str->data[0]<<endl;
    str.sizeData=str.index[str.len];
    str.dataStr.resize(str.sizeData);
    memcpy((char*)&str.dataStr[0],str.data,str.sizeData);
};


void GVector::getName(string&str){
    ulong *size;
    size=(ulong*)(data+index[0]);
    //cout<<"size="<<*size<<" index[0]="<<index[0]<<endl;
    str.resize(*size);
    memcpy(&str[0],(data+index[0]+8),*size);
};

void GVector::putPtr(ulong indexRecord,void* ptr, ulong size){
    indexRecord++;  //пропускаем индекс именных записей
    ulong *sizeRecord;
    int print=0;

    DT("index["<<indexRecord<<"]="<<index[indexRecord]<<" recordCount="<<*recordCount<<endl);
    if(indexRecord>*recordCount){return;}
    
    sizeRecord=(ulong*)(data+index[indexRecord]);
    DT("sizeRecord="<<*sizeRecord<<"size="<<size<<" index["<<indexRecord<<"]="<<index[indexRecord]<<endl);
    if(index[indexRecord]!=0){  //новая запись
        if(*sizeRecord>=size){
            memcpy(data+index[indexRecord]+8,ptr,size);
            *sizeRecord=size;
            return;
        }else{
            *sizeRecord=0;
        }
    }
    //записываем на новое место
    //проверяем достаточно ли места для новой записи
    if(*poolSize-1024<*dataSize+size){
        ulong newSize=(*poolSize)*FILE_RESIZE+size*FILE_RESIZE;
        DT("resize newSize="<<newSize<<" *poolSize="<<*poolSize<<endl);
        setSize(*recordArraySize,newSize);
    }
    DT("size="<<size<<" *dataSize="<<*dataSize<<" *poolSize="<<*poolSize<<endl);
    //записываем данные в конец блока
    memcpy(data+*dataSize,&size,8); 	 	              //записываем длинну записи
    memcpy(data+*dataSize+8,ptr,size);                 //копируем запись
    index[indexRecord]=*dataSize;	    	          //записываем адрес записи в индекс
    *dataSize+=size+8;
}

void GVector::putTStr(ulong indexRecord, TString &st){
    string str;
    getStr(indexRecord,str);
    if(str.size()<st.sizeData+st.len*4+8){
        str.resize((st.sizeData+st.len*4+8)*2);
    }
    memset(&str[0],0,str.size());
    memcpy(&str[0],(char*)&st.len,4);
    memcpy(&str[4],(char*)st.index,st.len*4+4);
    memcpy(&str[st.len*4+8],st.data,st.sizeData);
    putPtr(indexRecord,&str[0],str.size());
};


void GVector::putName(string &str){
    ulong *sizeRecord;
    ulong size=str.size();
    void* ptr=&str[0];
    sizeRecord=(ulong*)(data+index[0]);
    if(*sizeRecord >=size){
        memcpy(data+index[0]+8,ptr,size);
        *sizeRecord=size;
    }else{
        sizeRecord=0;
        //записываем на новое место
        //проверяем достаточно ли места для новой записи
        if(*poolSize-1024<*dataSize+size){
            //cout<<"resize GVector file new poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA<<"c2"<<endl;
            ulong newSize=(*poolSize)*FILE_RESIZE+size*FILE_RESIZE;
            setSize(*recordArraySize, newSize);
        }
        //записываем данные
        memcpy(data+*dataSize,&size,8); 	 	              //записываем длинну записи
        memcpy(data+*dataSize+8,ptr,size);                 //копируем запись
        index[0]=*dataSize;	    	          //записываем адрес записи в индекс
        *dataSize+=size+8;
    }
    
};
void GVector::putStr(ulong indexRecord, string&str){
 	putPtr(indexRecord,&str[0],str.size());
};
void GVector::putStr(ulong indexRecord, vector<uint>&str){
    putPtr(indexRecord,&str[0],str.size()*4);
};
void GVector::putCStr(ulong indexRecord,const char* str){
 	putPtr(indexRecord,(void*)str,strlen(str));
};
void GVector::putInt(ulong indexRecord,int data){
 	putPtr(indexRecord,(void*)&data,4);
};

char* GVector::setVector(cstr name,ulong &vectorIDChild){
    //проверяем, существует ли такая запись
    string name_=name;
    ulong indexRecord=0xfffffffffffffffa;
    string strName; getName(strName);
    //cout_<<"strName="<<strName<<endl;
    if(strName.size()>5){
        vector<string>indexName=explode(":|:",strName);
        //cout<<"indexName[a].size()="<<indexName.size()<<endl;
        for(int a=0; a<indexName.size()-1;a++){
            string st=indexName[a];
            if(indexName[a]==name_){
                //cout<<"name="<<indexName[a]<<"/";
                indexRecord=*((ulong*)&indexName[a+1][0]);
                //cout_<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<endl;
                break;
            }
        }
    }
    //cout<<" indexRecord="<<hex<<indexRecord<<endl;
    if(indexRecord!=0xfffffffffffffffa){
        string str;
        char *dataPointer=data+index[indexRecord+1]+8;
        vectorIDChild=indexRecord;
        return dataPointer;
    }else{
        string str;
        int size_=POOL_SIZE;
        str.resize(size_);
        push_back(str);
        vectorIDChild=*recordCount-2; //пропускаем индекс именных записей
        //сохраняем имя записи
        getName(str);
        str+=":|:"; str+=name; str+=":|:";
        str.resize(str.size()+8);
        memcpy(&str[0]+str.size()-8,&vectorIDChild,8);
        putName(str); //cout<<"vectorIDChild="<<vectorIDChild<<" offset="<<index[vectorIDChild+1]+4<<endl;
        char *dataPointer=data+index[vectorIDChild+1]+8;
        return dataPointer;
    }
    return NULL;
};

/**возврат указателя на дочерний GStr или GVector по именной записи */
void* GVector::getVectorByName(cstr name){
    //проверяем, существует ли такая запись
    string name_=name;
    ulong indexRecord=0;
    string strName; getName(strName);
    //cout<<"strName="<<strName<<endl;
    if(strName.size()>5){
        vector<string>indexName=explode(":|:",strName);
        //cout<<"indexName[a].size()="<<indexName.size()<<endl;
        for(int a=0; a<indexName.size()-1;a++){
            string st=indexName[a];
            if(indexName[a]==name_){
                //cout<<"name="<<indexName[a];
                memcpy(&indexRecord,&indexName[a+1][0],8);
                //cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<endl;
                //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
                break;
            }
        }
    }
    //cout<<" indexRecord"<<indexRecord<<endl;
    if(indexRecord!=0){
        char *dataPointer=data+index[indexRecord]+8;
        return (void*)dataPointer;
    }
    return NULL;
    
};

/**возврат указателя на дочерний GStr или GVector по ID */
void* GVector::getVector(ulong indexRecord){
    //проверяем, существует ли такая запись
    indexRecord++;
    char *dataPointer=data+index[indexRecord]+8;
    return (void*)dataPointer;
    return NULL;
};

/** импорт записей из XML, TXT, tab-separated TXT*/
void GVector::import(string &path, int mode){
    if(mode==READ_FROM_TXT)importTXT(path);
    if(mode==READ_FROM_TAB)importTAB(path);
    if(mode==READ_FROM_XML)importXML(path);
};

/** импорт записей из TXT построчно по разделителю \n или \r*/
void GVector::importTXT(string &path){
    char *textBuffer, *startIndexPtr;
    char c;
    ulong sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    ulong startIndex=0;
    ulong ln, index=0;
    path=fileName(path);
    
    startIndexPtr=textBuffer;
    vector<string>record;
    string str;
    
    for(ulong i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<hex<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout<<"index@@@";
            ln=i-startIndex;
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(index%100000==0)cout<<index<<" ";
            str.resize(ln);
            memcpy(&str[0],startIndexPtr,ln);
            startIndexPtr+=ln+1;
            startIndex=i+1;
            push_back(str);
        }
    }
};

/** импорт записей из TXT построчно по разделителю \n или \r с записью ключей в файл path1*/
void GVector::importDict(map<string,ulong>keyMap,string &path,string &path1){
    char *textBuffer, *startIndexPtr;
    char c;
    ulong sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    ulong startIndex=0;
    ulong ln, index=0;
    path=fileName(path);
    
    startIndexPtr=textBuffer;
    string keyIndex;
    string str;
    vector<string>record;
    
    for(ulong i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<hex<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout<<"index@@@";
            ln=i-startIndex;
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(index%100000==0)cout<<index<<" ";
            str.resize(ln);
            memcpy(&str[0],startIndexPtr,ln);
            startIndexPtr+=ln+1;
            startIndex=i+1;
            record=explode("\t",str);
            //if(record.size()<2)continue;
            TString st;
            for(int t=0;t<record.size();t++)st+=record[t];
            push_back(st);
            keyMap[record[0]]=*recordCount-1;
            keyIndex+=record[0]+"\n";
        }
    }
    writeText(keyIndex, path1);
    
};

/** импорт записей из tab-separated TXT построчно по разделителю \n или \r */
void GVector::importTAB(string &path){
    char *textBuffer, *startIndexPtr;
    char c;
    ulong sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    ulong startIndex=0;
    ulong ln, index=0;
    
    startIndexPtr=textBuffer;
    vector<string>record;
    string str;
    
    for(ulong i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout_<<i<<" ";
            ln=i-startIndex;
            //next we set all part of string in hash index
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(index%100000==0)cout<<index<<" ";
            str.resize(ln);
            memcpy(&str[0],startIndexPtr,ln);
            startIndexPtr+=ln+1;
            startIndex=i+1;
            //char pkbuf[256];
            //int pksiz=sprintf(pkbuf, "%u",*recordCount-1);
            TString st;
            
            st.push_TabStr(str);
            //str=" "; st.push_back(str); //corpus format
            //st.push_back(pkbuf,pksiz);
            push_back(st);
            ///cout_<<"size()="<<*recordCount<<endl;
        }
    }
    //cout_<<"done import. size()="<<*recordCount<<endl;
    
};
/** импорт записей из XML в формате FileMaker*/
void GVector::importXML(string &path){

    xml_node Cell,Data, letterSet,resultSet,table, metadata, field;
	xml_document doc;
	string str;
    vector<string> nameArray; nameArray.resize(100);
	cout<<"path="<<path<<endl;
	
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<endl;return;
	}
	
	letterSet = doc.child("FMPXMLRESULT");
    metadata = letterSet.child("METADATA");
	//from first we get map key name from field name 
	TString strT;
	for (field = metadata.child("FIELD"); field; field = field.next_sibling("FIELD")){
		strT.push_back(field.attribute("NAME").value());
        //cout<<"nameArray["<<i<<"]="<<field.attribute("NAME").value()<<endl;
	}
	push_back(strT);
	//first record now contain database field names
	
	//в GVector в записи все поля записаны как TString
	//название поля кодируются порядком расположения в TString
 
	resultSet=letterSet.child("RESULTSET");
    uint count=0;
	for (xml_node row = resultSet.child("ROW"); row; row = row.next_sibling("ROW")){
		map<string,string>record; ///base record for all letter conversion function
		TString strT;
		for (Cell = row.child("COL"); Cell; Cell = Cell.next_sibling("COL")){
			Data=Cell.child("DATA");
            string d=Data.child_value();
            if(d=="")d=" ";
			strT.push_back(d);
            
            //cout<<" "<<Data.child_value()<<"/ ";
		}
        count++;
		push_back(strT);
        //cout<<endl;
		
	}
};

/** экспорт всех записей формата TString*/
void GVector::exportTStr(cstr path){
    string str,line;
    string path_=path;
    for(uint i=0;i<*recordCount;i++){
        //
        if(i%100==0)cout<<" "<<i;
        TString st;
        getTStr(i,st);
        line="";
        string key;
        for(uint n=0;n<st.size();n++){
            key=st[n];
            key=str_replace("\t"," ",key);
            key=str_replace("\n","¶",key);
            key=str_replace("\r","¶",key);
            line+=key+"\t";
        }
        line+="\n";
        str+=line;
        if(i%100==0){
            writeFileAppend(str, path_);
            str="";
        }
        
    }
     writeFileAppend(str, path_);
}

/** экспорт всех записей формата TString*/
void GVector::exportTStr(string &path){
    exportTStr(path.c_str());
}

//экспорт в XML в формате FileMaker
void GVector::exportXML(string &path){

    //в GVector в записи все поля записаны как TString
	//первая запись содержит названия полей

        xml_document doc,docRecord;
        xml_node mainNode;
        xml_node book, data, key;
        mainNode=doc.append_child();
        mainNode.set_name("FMPXMLRESULT");
        TString strT;
        
        getTStr(0,strT);
       
        book=mainNode.append_child();
        book.set_name("METADATA");
        int size=strT.size();
          for(int i=0;i<size;i++){
             data=book.append_child();
             data.set_name("FIELD"); 
             ostringstream st; st<<"Field"<<i;
             data.append_attribute("NAME")=strT[i].c_str();
          }
        book=mainNode.append_child();
        book.set_name("RESULTSET");
        uint count=(uint)*recordCount;
        		
        for(int i=0;i<count;i++){
            data=book.append_child();
            data.set_name("ROW"); 
            TString str; getTStr(i,str);
            for(int n=0;n<str.size();n++){
               key=data.append_child();
               key.set_name("COL"); 
               key.append_child(node_pcdata).set_value(str[n].c_str());
            }
                
        } 
        cout<<"pathXML="<<path<<endl;
        bool exitCode=doc.save_file(path.c_str(),"\t", 1);
        if(exitCode){
            cout_<<"Done. save file "<<path<<endl;
        }else{
            cout_<<"Not saved "<<path<<endl;}
}

//зaкрывает обращение к GVector
void GVector::close(void){
    if(dataLocation==MMAP_LOCATION)dataMFile->flush();
}


////////////////////////////////
TString::TString(void){
    len=0; sizeData=0;
    indexStr.push_back(0);
    data=&dataStr[0];
    index=(uint*)&indexStr[0];
};

TString::~TString(void){
};

//запись данных из указателя на данные data во внутренний контейнер данных - dataStr. После этого TString можно копировать
void TString::save(){
    if(!dataStr.size()){
        dataStr.resize(sizeData);
        memcpy(&dataStr[0],data,sizeData);
        indexStr.resize(len*4+4);
        memcpy(&indexStr[0],index,len*4+4);
    }
}

//восстановление указателей на данные после записи в vector и другиз перемещений данных
void TString::reloadPtr(){
    
    data=(char *)&dataStr[0];
    index=(uint *)&indexStr[0];
    

}


void TString::push_back(char*ptr, uint size_){
    /*
    if(dataStr==NULL){
        dataStr=GStr<char>::create();
        indexStr=GStr<uint>::create();
        indexStr->push_back(0);
        if(sizeData!=0){ //копируем существующие данные   
      ?? в каком случае в строке есть данные а она не инициализирована?
            dataStr->push_Ptr(data,sizeData);
            indexStr->push_Ptr(index,len+4);
        }
    }
    */
    int l=(int)dataStr.size();
    dataStr.resize(l+size_);
    memcpy(&dataStr[l],ptr,size_);
    indexStr.push_back((int)dataStr.size());
    //cout<<"len="<<len<<" size_="<<indexStr->size()<<"lenInIndex="<<index[len+1]-index[len];
    //if(size_){cout<<" c="<<(short)ptr[0]<<endl;}
    //cout<<" data="<<dataStr->size()<<" size_="<<size_<<endl;
    data=&dataStr[0];
    index=(uint*)&indexStr[0];
    len+=1;
    sizeData+=size_;
};

void TString::push_back(TString &st){
    string str;
    uint markTString=0xfffffffc;
    str.resize(st.sizeData+st.len*4+12);
    memcpy(&str[0],(char*)&markTString,4);
    memcpy(&str[4],(char*)&st.len,4);
    //cout<<" st.len="<<st.len<<" st.sizeData="<<st.sizeData<<endl;
    memcpy(&str[8],(char*)st.index,st.len*4+4);
    memcpy(&str[st.len*4+12],st.data,st.sizeData);
    push_back(str);
}

void TString::push_back(string &str){
    push_back((char*)&str[0],(uint)str.size());
};

void TString::push_back(const char* str){
    push_back((char*)str,(uint)strlen(str));
};

void TString::push_back(uint intData){
     push_back((char*)&intData,4);
}

void TString::push_backL(ulong longData){
    push_back((char*)&longData,8);
}

void TString::push_TabStr(string &str){
	int index_=0; char*p=&str[0];
	for(int i=0;i<str.size();i++){
		if(str[i]=='\t'){
            push_back(p,i-index_);
            i++;
            p=(char*)&str[i];
            index_=i;
        }
	}
	push_back(p,(uint)str.size()-index_);
};

void TString::operator +=(string &str){
    push_back((char*)&str[0],(uint)str.size());
};

void TString::operator +=(cstr str){
    push_back((char*)str,(uint)strlen(str));
};

void TString:: operator +=(uint i){
    push_back((char*)&i,4);
}

string TString::operator [](uint i){
    string str;
    uint size_=index[i+1]-index[i]; //cout<<"index[i+1]="<<index[i+1]<<" index[i]="<<index[i]<<"size_="<<size_<<endl;
    if(size_&&size_<100000000){
        str.resize(size_);
        memcpy(&str[0],data+index[i],size_);
    }else{
        str="";
        if(size_!=0){
            cout<<endl<<" @i="<<i<<" index["<<i+1<<"]="<<index[i+1]<<" index["<<i<<"]="<<index[i]<<"size_="<<size_<<" l:"<<len<<endl;
        }

    }
    return str;
}

void TString::get(TString *st, uint i){
    uint size_=index[i+1]-index[i];
    uint *ptr=(uint*)(data+index[i]);
    if(*(ptr)!=0xfffffffc){st->len=0;st->sizeData=0; return;} //проверяем маркер
    st->len=*(ptr+1); //cout<<" st->len="<<st->len;
    st->index=ptr+2; //cout<<" st->index[0]="<<st->index[0]<<" st->index[1]="<<st->index[1];
    st->data=(char*)(ptr+st->len+3); //cout<<" st->data[0]="<<st->data[0]<<endl;
    st->sizeData=size_-st->len*4-12;
    
}

int TString::getInt(uint i){
   int dataInt;
   memcpy(&dataInt,data+index[i],4);
   return dataInt;
}

int TString::getLong(uint i){
    ulong dataLong;
    memcpy(&dataLong,data+index[i],8);
    return dataLong;
}

void TString::getStr(string &str, uint i){
    uint size_=index[i+1]-index[i]; 
    str.resize(size_);
    memcpy(&str[0],data+index[i],size_);
}

void TString::getStr(char **p, uint i){
    uint size_=index[i+1]-index[i];
    memcpy(*p,data+index[i],size_);
    (*p)[size_]=0;
}

void TString::getIntVector(vector<int>&intVector,uint i){
    uint size_=index[i+1]-index[i];
    intVector.resize(size_/4);
    memcpy(&intVector[0],data+index[i],size_);
}

void TString::getCharVector(vector<char>&charVector,uint i){
        uint size_=index[i+1]-index[i];
        charVector.resize(size_);
        memcpy(&charVector[0],data+index[i],size_);
}

void TString::setFromPtr(uint*index_,uint len_,char*data_,uint dataSize){
    index=index_;
    data=data_;
    len=len_;
    sizeData=dataSize;
};

