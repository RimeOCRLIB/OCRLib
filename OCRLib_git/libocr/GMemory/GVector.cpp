#include "GVector.h"
#include "php2stl.h"

using namespace std;
using namespace ocr;

GVector::GVector(void){
	try
    {
        init();
    }
    catch(int a)
    {
        free();
    }
};
GVector::GVector(string &path){
    try
    {
        init(path);
    }
    catch(int a)
    {
        free();
    }
}

GVector::GVector(const char* path){
    string pathStr=path;
    try
    {
        init(pathStr);
    }
    catch(int a)
    {
        free();
    }
}

GVector::GVector(char* data_,int mode){
    try
    {
        init(data_,mode);
    }
    catch(int a)
    {
        free();
    }
}

GVector::GVector(GVector* parentVector, cstr name){
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
void GVector::reloadPtr(){
	poolSize		=&innerData[1];           //размер файла вектора
    recordCount		=&innerData[2];  	      //количество записей в векторе
    recordArraySize	=&innerData[3];           //размер файла учета записей
    dataSize		=&innerData[4];  	 	  //размер занимаемый актуальными данными
    lockFlag        =&innerData[5];           //флаг блокировки чтения-записи
    indexOffset		=&innerData[6];	 	 	  //адрес размещения массива индекса записей. отсчитывается от начала файла
    vectorID		=&innerData[7]; 	 	  //индех записи, в которой GVector размещен в родительском векторе
};

/**инициализация GVector с размещением в оперативной памяти*/
void GVector::GVector::init(){
    data=(char*)malloc(1024*24);
    dataLocation=MEMORY_LOCATION; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
    innerData=(uint*)(data);
    innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
    reloadPtr();
    *poolSize=1024*24;
    *recordArraySize=1024;
    *recordCount=0;
    *dataSize=(*recordArraySize)*4+64;
    *indexOffset=64;
    *vectorID=0xffffffff;
    *lockFlag=0;
    string indexName="name:|:data";
    index=(uint*)(data+*indexOffset);  //инициализация индекса
	push_back(indexName); 	 	 	 	 	    //инициализация индекса именных записей
}

void GVector::init(string &path){
    //подключаем или создаем файл данных
    dataMFile=MemoryFile::create(path.c_str(),MemoryFile::if_exists_keep_if_dont_exists_create);
    data=dataMFile->data();
    dataPath=path;
    dataLocation=MMAP_LOCATION; 	 		 	  //флаг размещения данных в оперативной  или  отображаемой памяти
    //cout<<" dataMFile->size()="<<dataMFile->size()<<endl;
    //проверяем является ли файл файлом GVector
    if(dataMFile->size()==0){//новый файл GVector
        dataMFile->resize(1024*24);
        data=dataMFile->data();
        innerData=(uint*)(data);
        innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
        reloadPtr();
        *poolSize=(uint)dataMFile->size();
        *recordArraySize=1024;
        *recordCount=0;
        *dataSize=(*recordArraySize)*4+64;
        *indexOffset=64;
        *vectorID=0xffffffff;
        *lockFlag=0;
        string indexName="name:|:data";
        index=(uint*)(data+*indexOffset);  //инициализация индекса
    	push_back(indexName); 	 	 	 	 	    //инициализация индекса именных записей
        *recordCount=1;
        
    }else{
        //проверяем является ли файл файлом GVector
        innerData=(uint*)(data);
        if(innerData[0]!=0xfffffffa){
            //cout<<"no valid GVector file "<<path; return;
        }
        reloadPtr();
        index=(uint*)(data+*indexOffset);  //инициализация индекса
        *poolSize=(uint)dataMFile->size();
        
    }
    
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
    dataLocation=mode; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
    innerData=(uint*)(data);
    innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
    reloadPtr();
    *lockFlag=0;
}

/**инициализация GVector копированием с размещением в оперативной памяти*/ /// ####
void GVector::GVector::init(GVector* ref){
    /*
     if(!ref->data_size())return;
     free();
     data=(char*)malloc(ref->size());
     dataLocation=MEMORY_LOCATION; 	 	 	 	//флаг размещения данных в оперативной  или  отображаемой памяти
     memcpy(data,ref->dataPtr(),ref->data_size());
     innerData=(uint*)(data);
     innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
     reloadPtr();
     */
    
}

void GVector::init(GVector* parentVector, cstr name){
    //запрашиваем у родительского GVector указатель на файл данных
    //если запись с таким именем уже существует, получаем на нее указатель
    uint vectorID_;
    parent=parentVector;
    data=parentVector->setVector(name,&vectorID_);  //cout_<<"#1vectorID_="<<vectorID_<<endl;
    //проверяем является ли файл файлом GVector
    innerData=(uint*)(data);
    if(innerData[0]!=0xfffffffa){//новый GVector
        //cout<<"new GVector"<<endl;
        innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
        reloadPtr();
        *poolSize=1024*24;
        *recordArraySize=1024;
        *recordCount=0;
        *dataSize=(*recordArraySize)*4+64;
        *indexOffset=64;
        *vectorID=vectorID_;
        *lockFlag=0;
        string indexName="name:|:data";
        index=(uint*)(data+*indexOffset);  //инициализация индекса
    	push_back(indexName); 	 	 	   //инициализация индекса именных записей
        
    }else{
        //проверяем является ли файл файлом GVector
        //cout<<"reload GVector"<<endl;
        innerData=(uint*)(data);
        if(innerData[0]!=0xfffffffa){
            //cout<<"no valid GVector file "; return;
        }
        reloadPtr();
        index=(uint*)(data+*indexOffset);  //инициализация индекса
        *vectorID=vectorID_;
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
      innerData[0]=0xfffffffa; 	 	 	 	 	//маркер наличия GVector
      reloadPtr();
      *vectorID=vectorID_;
      index=(uint*)(data+*indexOffset);  //инициализация индекса
      
  
}
*/

void GVector::addRecords(GVector *ref){
    //cout<<"ref->size()="<<ref->size()<<" path="<<ref->path()<<endl;
    //cout<<" self path="<<path()<<endl;
    int size=ref->size();
    for(int i=0;i<size;i++){ 
        TString str;
        ref->getTStr(i,&str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
        push_back(&str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
        getTStr(i,&str);
        //cout<<" @@@/"<<str[3]<<"/"<<i<<" s="<<ref->size()<<endl;
    }
    //cout<<"done"<<endl;
}

GVector::~GVector(){}

void GVector::free(void){
	if (dataLocation==MEMORY_LOCATION&&data!=NULL){
        std::free(data);
    }else{
        delete dataMFile;
    }
    delete this;
};

//добавление новой записи в GVector
void GVector::push_back(string&str){
    uint size=(uint)str.size();
    //data=dataMFile->data();
    
    //string dt; dt.resize(20);
    //memcpy(&dt[0],data,20);
    //cout<<" poolSize="<<*poolSize<<" dataSize="<<*dataSize<<endl;
    
    //проверяем достаточно ли места в массиве индекса
    if(*recordArraySize<*recordCount+1){
        //cout_<<"resize GVector index"<<" recordArraySize="<<*recordArraySize<<" recordCount="<<*recordCount<<" *poolSize="<<*poolSize<<endl;
        //увеличиваем массив индекса и записываем его как новую запись
        if((*recordArraySize)*INDEX_RESIZE+*dataSize>*poolSize){
            //cout<<"start resize for index. New size="<<*poolSize+(*recordArraySize)*INDEX_RESIZE+size*FILE_RESIZE_DELTA<<endl;
   	        uint newSize=*poolSize+(*recordArraySize)*INDEX_RESIZE+size*FILE_RESIZE_DELTA;
   	        uint vectorID_=*vectorID;     	   //cout_<<"#2vectorID_="<<vectorID_<<endl;
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
            
            innerData=(uint*)(data);
            reloadPtr();
            //cout<<"копируем индех";
            //копируем индех
            memcpy(data+*dataSize,data+*indexOffset,(*recordArraySize)*4);
            *recordArraySize=*recordArraySize*INDEX_RESIZE;
            *indexOffset=*dataSize;
            index=(uint*)(data+*indexOffset);
            *dataSize+=(*recordArraySize)*4;
            *poolSize=newSize;
            
        }else{
            //cout<<"копируем индех";
            //копируем индех
            memcpy(data+*dataSize,data+*indexOffset,(*recordArraySize)*4);
            *recordArraySize=*recordArraySize*INDEX_RESIZE;
            *indexOffset=*dataSize;
            index=(uint*)(data+*indexOffset);
            *dataSize+=(*recordArraySize)*4;
        }
        
    }
    //cout<<" poolSize1="<<*poolSize<<" dataSize="<<*dataSize<<endl;
    //проверяем достаточно ли места для новой записи
    int n=1000000;
    if(*poolSize<*dataSize+size+256){
        uint newSize=(*poolSize)*FILE_RESIZE;
        if(newSize<*dataSize+size+256)newSize=(*dataSize+size+256)*1.5;
        //cout<<"@resize GVector file new poolSize="<<newSize/n<<" Mb c3"<<endl;
   	    uint vectorID_=*vectorID;      //сохраняем vectorID от потери при перезаписи указателей
        //cout_<<"#3vectorID_="<<vectorID_<<endl;
   	    if(*vectorID==0xffffffff){
            if(dataLocation==MEMORY_LOCATION){
                data=(char*)realloc(data,newSize);
            }else{
                dataMFile->resize(newSize);
                data=dataMFile->data();
                newSize=(uint)dataMFile->size();
            }
        }else{
            void *mem=malloc(newSize);
            //cout<<"*poolSize="<<*poolSize/n<<" newSize="<<newSize/n<<endl;
            memcpy(mem,data,*poolSize);
            parent->putPtr(vectorID_,mem,newSize);
            std::free(mem);
            data=(char*)parent->getPtr(vectorID_,&newSize);
            //cout<<"@new size="<<newSize<<endl;
            
        }
        innerData=(uint*)(data);
        reloadPtr();
        *poolSize=newSize;
        index=(uint*)(data+*indexOffset);  //инициализация индекса
        
    }
    //записываем данные
    memcpy(data+*dataSize,&size,4); 	 	        //записываем длинну записи
    memcpy(data+*dataSize+4,&str[0],size);       //копируем запись
    index[*recordCount]=*dataSize;	    	    //записываем адрес записи в индекс
    *recordCount=*recordCount+1;
    *dataSize=*dataSize+size+4;
    
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
void  GVector::resizeData(uint recordSize, uint datasize){
    uint newSize=datasize*1024000;
    if(newSize<recordSize*6)newSize=recordSize;
    //cout<<"*poolSize="<<*poolSize/1000000<<" newSize="<<newSize/1000000<<" *dataSize="<<*dataSize/1000000<<" recordSize"<<recordSize*4/1000000<<endl;
    if(newSize<=*poolSize)return;
    //cout_<<"@resize GVector file new poolSize="<<newSize<<endl;
    uint vectorID_=*vectorID;      //сохраняем vectorID от потери при перезаписи указателей
    //cout_<<"#4vectorID_="<<vectorID_<<endl;
    if(vectorID_==0xffffffff){
        if(dataLocation==MEMORY_LOCATION){
            data=(char*)realloc(data,newSize);
        }else{
            dataMFile->resize(newSize);
            data=dataMFile->data();
            newSize=(uint)dataMFile->size();
        }
    }else if(newSize>*poolSize) {
        string str;
        str.resize(newSize);
        memcpy(&str[0],data,*poolSize);
        parent->putStr(vectorID_,str);
        data=(char*)parent->getPtr(vectorID_,&newSize);
        //cout_<<"@new size="<<newSize<<endl;
    }
    innerData=(uint*)(data);
    reloadPtr();
    *poolSize=newSize;
    index=(uint*)(data+*indexOffset);  //инициализация индекса
    if(*recordArraySize<recordSize){
        //cout_<<"копируем индех";
        //копируем индех
        memcpy(data+*dataSize,data+*indexOffset,(*recordArraySize)*4);
        *recordArraySize=recordSize*4;
        *indexOffset=*dataSize;
        index=(uint*)(data+*indexOffset);
        *dataSize+=*recordArraySize;
        //cout_<<" *dataSize="<<*dataSize<<" index="<<index[1]<<endl;
    }
    
};


/** resize GVector for new record count*/
void  GVector::resize(uint newRecordCount){
    //cout_<<"*poolSize="<<*poolSize<<" newSize="<<newSize<<endl;
    //cout_<<"@resize GVector file new poolSize="<<newSize<<endl;
    if(*recordCount<newRecordCount){
        unsigned long newSize=(newRecordCount*8+*poolSize);
        resizeData(newRecordCount, (uint)newSize);
        //cout<<" *dataSize="<<*dataSize<<" *poolSize="<<*poolSize<<endl;
        reloadPtr();
        //в начале каждой записи лежит значение длинны записи.
        //поэтому все новые записи инициализируем с нулевой длинной
        uint rStart=*recordCount;
        memset(data+*dataSize,0,(newRecordCount-*recordCount)*4);   //обнуляем записи
        for(uint i=rStart;i<newRecordCount;i++){
            index[i]=*dataSize;	    	       //записываем адрес записи в индекс
            *dataSize+=4;
        }

    }
    
    *recordArraySize=newRecordCount;
    *recordCount=newRecordCount;
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
void GVector::push_back(char*str, uint size){
    string str_=str;
    str_.resize(size);
    memcpy(&str[0],str,size);
    push_back(str_);
};
void GVector::push_back(string&str,cstr name){
    push_back(str);
    string strName;
    getName(strName);
    strName+=":|:"; strName+=name; strName+=":|:";
    strName.resize(strName.size()+4);
    uint n=*recordCount-1;
    memcpy(&strName[0]+strName.size()-4,&n,4);
    putName(strName);
    
};

void GVector::push_back(TString *st){
    string str;
    uint markTString=0xfffffffc;
    str.resize(st->sizeData+st->len*4+12);
    memcpy(&str[0],(char*)&markTString,4);
    memcpy(&str[4],(char*)&st->len,4);
    //cout<<" st->len="<<st->len<<" st->sizeData="<<st->sizeData<<endl;
    memcpy(&str[8],(char*)st->index,st->len*4+4);
    memcpy(&str[st->len*4+12],st->data,st->sizeData);
    push_back(str);
}


//получение данных из GVector
void* GVector::getPtr(uint indexRecord,uint *size){
    indexRecord++;  //пропускаем индекс именных записей
    if(indexRecord>*recordCount){*size=0;return NULL;}
    *size=*((uint*)(data+index[indexRecord]));
    if(*size==0){return NULL;}
    //cout<<"*size="<<*size<<endl;
    return (void*)(data+index[indexRecord]+4);
};
void GVector::getStr(uint indexRecord, string&str){
    indexRecord++;  //пропускаем индекс именных записей
    uint *size;
    //cout<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<" *dataSize="<<*dataSize<<endl;
    //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
    size=(uint*)(data+index[indexRecord]);
    //cout<<"size="<<*size<<endl;
    if(*size+index[indexRecord]+4>*dataSize){cout<<"size out of range"; return;}
    str.resize(*size);
    memcpy(&str[0],(data+index[indexRecord]+4),*size);
};

void GVector::getStr(uint indexRecord, vector<int>&str){
    indexRecord++;  //пропускаем индекс именных записей
    uint *size;
    //cout_<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<" *dataSize="<<*dataSize<<endl;
    //cout<<"index[indexRecord]="<<index[indexRecord]<<endl;
    size=(uint*)(data+index[indexRecord]);
    //cout<<"size="<<*size<<endl;
    str.resize(*size/4);
    memcpy(&str[0],(data+index[indexRecord]+4),*size);
};


void GVector::getStr(string&str,cstr name){
    uint *size;
    string name_=name;
    uint indexRecord=0;
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
    size=(uint*)(data+index[indexRecord]);
    if(*size==0){str="";return;}
    //cout<<"size="<<*size<<endl;
    str.resize(*size);
    memcpy(&str[0],(data+index[indexRecord]+4),*size);
    
};

int GVector::getInt(uint indexRecord){
    indexRecord++;  //пропускаем индекс именных записей
    int *size;
    if(indexRecord>*recordCount){*size=0;return NULL;}
    *size=*((uint*)(data+index[indexRecord]));
    if(*size==0){return NULL;}
    //cout<<"*size="<<*size<<endl;
    return *(int*)(data+index[indexRecord]+4);
};

/**получение указателей на запись в виде токенизированной строки ТОЛЬКО ДЛЯ ЧТЕИЯ! */
void GVector::getTStr(uint indexRecord,TString *str){
    indexRecord++;  //пропускаем индекс именных записей
    if(indexRecord>*recordCount){str->len=0;str->sizeData=0;return;}
    uint *ptr=(uint*)(data+index[indexRecord]);
    uint size_=*ptr;  //cout<<" size_="<<size_;
    if(*(ptr+1)!=0xfffffffc){str->len=0;str->sizeData=0;return;} //проверяем маркер
    str->len=*(ptr+2); //cout<<" str->len="<<str->len;
    str->index=ptr+3; //cout<<" str->index[0]="<<str->index[0]<<" str->index[1]="<<str->index[1];
    str->data=(char*)(ptr+str->len+4); //cout<<" str->data[0]="<<str->data[0]<<endl;
    str->sizeData=size_-str->len*4-12;
};

void GVector::getName(string&str){
    uint *size;
    size=(uint*)(data+index[0]);
    //cout<<"size="<<*size<<endl;
    str.resize(*size);
    memcpy(&str[0],(data+index[0]+4),*size);
};

void GVector::putPtr(uint indexRecord,void* ptr, uint size){
    indexRecord++;  //пропускаем индекс именных записей
    uint *sizeRecord;
    int print=0;
    //if(inputData.start){
    //    print=1;
    //}
    
    if(print)cout<<"index["<<indexRecord<<"]="<<index[indexRecord]<<" recordCount="<<*recordCount<<endl;
    if(indexRecord>*recordCount){return;}
    sizeRecord=(uint*)(data+index[indexRecord]);
    if(print)cout<<"sizeRecord="<<*sizeRecord<<"size="<<size<<" index["<<indexRecord<<"]="<<index[indexRecord]<<endl;
    if(*sizeRecord>=size){
        memcpy(data+index[indexRecord]+4,ptr,size);
        *sizeRecord=size;
    }else{
        *sizeRecord=0;
        //записываем на новое место
        //проверяем достаточно ли места для новой записи
        if(*poolSize-1024<*dataSize+size){
            unsigned int newSize=*poolSize+*dataSize+size;
            if(print)cout<<"resize newSize="<<newSize<<" *poolSize="<<*poolSize<<endl;
            if(newSize<*dataSize+size)newSize=(uint)4290*1000000;  //max integer
            if(newSize<*dataSize+size){cout<<"data exceed max integer capacity"; return;}
            uint vectorID_=*vectorID;      //сохраняем vectorID от потери при перезаписи указателей
            //cout_<<"#5vectorID_="<<vectorID_<<endl;
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
                if(print)cout<<"@new size="<<newSize<<endl;
            }
            
            innerData=(uint*)(data);
            reloadPtr();
            *poolSize=newSize;
            index=(uint*)(data+*indexOffset);     //инициализация индекса
        }
        if(print)cout<<"size="<<size<<" *dataSize="<<*dataSize<<" *poolSize="<<*poolSize<<endl;
        //записываем данные в конец блока
        memcpy(data+*dataSize,&size,4); 	 	              //записываем длинну записи
        memcpy(data+*dataSize+4,ptr,size);                 //копируем запись
        index[indexRecord]=*dataSize;	    	          //записываем адрес записи в индекс
        *dataSize+=size+4;
    }
    
};
void GVector::putTStr(uint indexRecord, TString *st){
    string str;
    uint markTString=0xfffffffc;
    str.resize(st->sizeData+st->len*4+12);
    memcpy(&str[0],(char*)&markTString,4);
    memcpy(&str[4],(char*)&st->len,4);
    memcpy(&str[8],(char*)st->index,st->len*4+4);
    memcpy(&str[st->len*4+12],st->data,st->sizeData);
    putPtr(indexRecord,&str[0],(uint)str.size());
};


void GVector::putName(string &str){
    uint *sizeRecord;
    uint size=(uint)str.size();
    void* ptr=&str[0];
    sizeRecord=(uint*)(data+index[0]);
    if(*sizeRecord >=size){
        memcpy(data+index[0]+4,ptr,size);
        *sizeRecord=size;
    }else{
        sizeRecord=0;
        //записываем на новое место
        //проверяем достаточно ли места для новой записи
        if(*poolSize-1024<*dataSize+size){
            //cout<<"resize GVector file new poolSize="<<(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA<<"c2"<<endl;
            
            uint newSize=(*poolSize)*FILE_RESIZE+size*FILE_RESIZE_DELTA;
            uint vectorID_=*vectorID;      //сохраняем vectorID от потери при перезаписи указателей
            //cout_<<"#7vectorID_="<<vectorID_<<endl;
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
                //cout<<"@new size="<<newSize<<endl;
                
            }
            
            innerData=(uint*)(data);
            reloadPtr();
            *poolSize=newSize;
            index=(uint*)(data+*indexOffset);     //инициализация индекса
        }
        //записываем данные
        memcpy(data+*dataSize,&size,4); 	 	              //записываем длинну записи
        memcpy(data+*dataSize+4,ptr,size);                 //копируем запись
        index[0]=*dataSize;	    	          //записываем адрес записи в индекс
        *dataSize+=size+4;
    }
    
};
void GVector::putStr(uint indexRecord, string&str){
 	putPtr(indexRecord,&str[0],(uint)str.size());
};
void GVector::putStr(uint indexRecord, vector<int>&str){
    putPtr(indexRecord,&str[0],(uint)str.size()*4);
};
void GVector::putCStr(uint indexRecord,const char* str){
 	putPtr(indexRecord,(void*)str,(uint)strlen(str));
};
void GVector::putInt(uint indexRecord,int data){
 	putPtr(indexRecord,(void*)&data,4);
};

char* GVector::setVector(cstr name,uint *vectorID){
    //проверяем, существует ли такая запись
    string name_=name;
    uint indexRecord=0xffffffff;
    string strName; getName(strName);
    //cout_<<"strName="<<strName<<endl;
    if(strName.size()>5){
        vector<string>indexName=explode(":|:",strName);
        //cout<<"indexName[a].size()="<<indexName.size()<<endl;
        for(int a=0; a<indexName.size()-1;a++){
            string st=indexName[a];
            if(indexName[a]==name_){
                //cout<<"name="<<indexName[a]<<"/";
                indexRecord=*((uint*)&indexName[a+1][0]);
                //cout_<<"indexRecord="<<indexRecord<<" *recordCount="<<*recordCount<<endl;
                break;
            }
        }
    }
    //cout_<<" indexRecord"<<indexRecord<<endl;
    if(indexRecord!=0xffffffff){
        string str;
        char *dataPointer=data+index[indexRecord+1]+4;
        *vectorID=indexRecord;
        return dataPointer;
    }else{
        string str;
        int size_=1024*24;
        str.resize(size_);
        push_back(str);
        *vectorID=*recordCount-2; //пропускаем индекс именных записей
        //сохраняем имя записи
        getName(str);
        str+=":|:"; str+=name; str+=":|:";
        str.resize(str.size()+4);
        memcpy(&str[0]+str.size()-4,vectorID,4);
        putName(str); //cout_<<"*vectorID="<<*vectorID<<endl;
        char *dataPointer=data+*dataSize-size_;
        return dataPointer;
    }
    return NULL;
};

/**возврат указателя на дочерний GStr или GVector по именной записи */
void* GVector::getVector(cstr name){
    //проверяем, существует ли такая запись
    string name_=name;
    uint indexRecord=0;
    string strName; getName(strName);
    //cout<<"strName="<<strName<<endl;
    if(strName.size()>5){
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
    }
    //cout<<" indexRecord"<<indexRecord<<endl;
    if(indexRecord!=0){
        char *dataPointer=data+index[indexRecord]+4;
        return (void*)dataPointer;
    }
    return NULL;
    
};

/**возврат указателя на дочерний GStr или GVector по ID */
void* GVector::getVector(uint indexRecord){
    //проверяем, существует ли такая запись
    indexRecord++;
    if(indexRecord!=0){
        char *dataPointer=data+index[indexRecord]+4;
        return (void*)dataPointer;
    }
    return NULL;
};


/**вывод в result HTML rowsNum записей начиная с startRec */
void GVector::drawHTML(uint startRec,int rowsNum,string &result,int mode){
    ostringstream str;
    int a=0;
    //cout_<< "startRec="<<startRec<<" rowsNum="<<rowsNum<<endl;
    for(uint index=startRec;index<startRec+rowsNum;index++){ //cout<<" index="<<index;
        TString strDict; getTStr(index,&strDict);
        //cout_<<"strDict.size="<<strDict.size;
        //for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        if(!strDict.size()){continue;}
        str<<"<tr id=\"r"<<a<<"\" class=\"dictRow\" onMouseOver=\"lightRow('r"<<a<<"')\" onMouseOut=\"showOver('r"<<a<<"')\">\n";
        str<<"<td title=\"Action\" class=\"dictCell\" id=\"s"<<a<<
        "c0\"><button onClick=\"saveRow('r"<<a<<"')\" \">s</button></td>\n";
        string dataStr;
        for(int cellNum=0;cellNum<strDict.len;cellNum++){
            string strXML;
            strXML=strDict[cellNum];
            strXML=XMLEncode(strXML);
            if(cellNum<4){
                if(cellNum==3&&mode==CORPUS_HTML){
                    str<<"<td title=\"Translation\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" style=\"overflow:visible;white-space: normal\">"<<strXML<<"</td>\n";
                }else{
                    str<<"<td class=\"dictCell"<<cellNum<<"\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" onClick=\"editCell('r"<<a<<"c"<<cellNum<<"')\">"<<strXML<<"</td>\n"; //onBlur=\"saveRow('r"<<a<<"')\"
                }
                
            }else{
                if(cellNum==4){str<<"<td title=\""<<index<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\">"<<index<<"</td>\n";
                }
            }
            
            
        }
        str<< "</tr>";
        a++;
    }
    result=str.str();
}
/**вывод в result HTML результатов поиска */
void GVector::drawHTML(vector<uint>&searchResult,string &result,int mode){
    ostringstream str;
    int a=0;
    //cout_<< "startRec="<<startRec<<" rowsNum="<<rowsNum<<endl;
    for(uint i=0;i<searchResult.size();i++){ 
        uint index=searchResult[i]; //cout<<" index="<<index;
        TString strDict; getTStr(index,&strDict);
        //cout_<<"@@@strDict.size="<<strDict.size;
        //for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        if(!strDict.size()){continue;}
        str<<"<tr id=\"r"<<a<<"\" tabindex=\"1\" class=\"dictRow\" onMouseOver=\"lightRow('r"<<a<<"')\" onMouseOut=\"showOver('r"<<a<<"')\">\n";
        str<<"<td title=\"Action\" class=\"dictCell\" id=\"s"<<a<<
        "c0\"><button onClick=\"saveRow('r"<<a<<"')\">s</button><button onClick=\"copyRow('r"<<a<<"')\">c</button></td>\n";
        string dataStr;
        for(int cellNum=0;cellNum<strDict.len;cellNum++){
            string strXML;
            strXML=strDict[cellNum];
            strXML=XMLEncode(strXML);
            if(cellNum!=5){
                if(cellNum==3&&mode==CORPUS_HTML){
                    str<<"<td title=\"Translation\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" style=\"overflow:visible;white-space: normal\">"<<strXML<<"</td>\n";
                }else{
                    str<<"<td class=\"dictCell"<<cellNum<<"\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" onClick=\"editCell('r"<<a<<"c"<<cellNum<<"')\">"<<strXML<<"</td>\n";
                }
            }else{
                if(cellNum==5){str<<"<td class=\"dictCell5\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\">"<<index<<"</td>\n";
                }
            }
        }
        str<< "</tr>";
        a++;
    }
    result=str.str();
}

/**вывод в result HTML результатов поиска по каталогу библиотеки */
void GVector::drawCatalogHTML(vector<uint>&searchResult,GVector*vectorLibPath,string &result,int mode){
    ostringstream str;
    int a=0; 
    string pathName;
    string pathLink;
    //cout_<< "startRec="<<startRec<<" rowsNum="<<rowsNum<<endl;
    for(uint i=0;i<searchResult.size();i++){ 
        uint index=searchResult[i]; //cout<<" index="<<index;
        TString strDict; getTStr(index,&strDict);
        //cout<<"@@@strDict.size="<<strDict.size();
        if(strDict.size()<9)continue;
        //cout<<strDict[3]<<endl;
        pathName=strDict[3];
        if(pathName=="")continue;
        uint size=vectorLibPath->size();
        int flag=0;
        
        
        for(int n=0;n<size;n++){
            vectorLibPath->getStr(n,pathLink);
            if(pathLink.find(pathName)==-1)continue;
            if(pathLink.find("_press")!=-1)continue;
            TString st;
            vectorLibPath->getTStr(n,&st);
            pathLink=st[0];
            //cout_<<"pathLink="<<pathLink<<" pathName="<<pathName<<endl;
            flag=1;
            break;
        }
        if(!flag)continue;
        //for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        if(mode==HTML){
            str<<"<tr id=\"r"<<a<<"\" tabindex=\"1\" class=\"dictRow\" onMouseOver=\"lightRow('r"<<a<<"')\" onMouseOut=\"showOver('r"<<a<<"')\">\n";
            str<<"<td title=\"Action\" class=\"dictCell\" id=\"s"<<a<<
            "c0\"><button onClick=\"saveRow('r"<<a<<"')\">s</button><button onClick=\"copyRow('r"<<a<<"')\">c</button></td>\n";
            string dataStr;
            for(int cellNum=1;cellNum<strDict.len;cellNum++){
                string strXML;
                strXML=strDict[cellNum];
                strXML=XMLEncode(strXML);
                str<<"<td class=\"dictCell\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<"\"><div class=\"lCell"<<cellNum<<"\"><a href=\""<<pathLink<<"\" target=\"_blank\">"<<strXML<<"</a></div></td>\n";
            }
            str<< "</tr>";
        } 
        if(mode==CHAT){
            pathLink=str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/","/_LIBRARY_/",pathLink);
            for(int cellNum=1;cellNum<strDict.len;cellNum++){
                if(cellNum!=2&&cellNum!=3&&cellNum!=6&&cellNum!=7)continue;
                string strXML;
                strXML=strDict[cellNum];
                //strXML=XMLEncode(strXML);
                str<<strXML<<"\n";
            }
            str<<"http://www.dharmabook.ru"<<pathLink;
        }    
        a++;
        
    }
    result=str.str();
}

/**вывод в result HTML GVector* strResult */
void GVector::drawHTML(uint startRec,int rowsNum,GVector* strResult,string &result,int mode){
    ostringstream str;int a=0;
    for(int index=startRec;index<startRec+rowsNum;index++){
        TString strDict; getTStr(index,&strDict);
        string dictResult; strResult->getStr(a,dictResult);
        if(dictResult=="")dictResult="no data";
        //cout_<<"strDict.size="<<strDict.size;
        //for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        if(!strDict.size()){continue;}
        str<<"<tr id=\"r"<<a<<"\" tabindex=\"1\" role=\"row\" class=\"ui-row-ltr\" onMouseOver=\"lightRow('r"<<a<<"')\" onMouseOut=\"showOver('r"<<a<<"')\">\n";
        str<<"<td title=\"Action\" id=\"s"<<a<<
        "c0\" onClick=\"editCell('r"<<a<<"c0')\"></td>\n";
        string dataStr;
        for(int cellNum=0;cellNum<strDict.len;cellNum++){
            string strXML;
            strXML=strDict[cellNum];
            strXML=XMLEncode(strXML);
            if(cellNum<4){
                if(cellNum==1&&mode==CORPUS_HTML){
                        str<<"<td title=\"Translation\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" style=\"overflow:visible;white-space: normal\"><a href=\"ocr.php?db=corpus&record=find&c5="<<index<<"\" target=\"_blank\">@</a>"<<dictResult<<"</td>\n";
                }else{
                    str<<"<td title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" onClick=\"editCell('r"<<a<<"c"<<cellNum<<"')\"\">"<<strXML<<"</td>\n";
                }
                
            }else{
                if(cellNum==4){str<<"<td class=\"dictCell5\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\">"<<index<<"</td>\n";
                }
            }
        }
        str<< "</tr>";
        a++;
    }
    result=str.str();
}

/**вывод в result HTML результатов поиска по корпусу текстов*/
void GVector::drawHTML(vector<uint>&searchResult,GVector* strResult, string &result,int mode){
    ostringstream str;int a=0;
    uint rowsNum=25;
    if (rowsNum>searchResult.size())rowsNum=searchResult.size();
    
    for(int index=0;index<rowsNum;index++){
        TString strDict; getTStr(searchResult[index],&strDict);
        string dictResult; strResult->getStr(a,dictResult);
        //cout_<<"strDict.size="<<strDict.size;
        //for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        if(!strDict.size()){continue;}
        str<<"<tr id=\"r"<<a<<"\" tabindex=\"1\" role=\"row\" class=\"ui-row-ltr\" onMouseOver=\"lightRow('r"<<a<<"')\" onMouseOut=\"showOver('r"<<a<<"')\">\n";
        str<<"<td title=\"Action\" id=\"s"<<a<<
        "c0\" onClick=\"editCell('r"<<a<<"c0')\" onBlur=\"saveRow('r"<<a<<"')\"></td>\n";
        string dataStr;
        for(int cellNum=0;cellNum<strDict.len;cellNum++){
            string strXML;
            strXML=strDict[cellNum];
            strXML=XMLEncode(strXML);
            if(cellNum<4){
                if(cellNum==1&&mode==CORPUS_HTML){
                    str<<"<td title=\"Translation\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" style=\"overflow:visible;white-space: normal\"><a href=\"ocr.php?db=corpus&record=find&c5="<<searchResult[index]<<"\" target=\"_blank\">@</a>"<<dictResult<<"</td>\n";
                }else{
                    str<<"<td title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\" onClick=\"editCell('r"<<a<<"c"<<cellNum<<"')\" onBlur=\"saveRow('r"<<a<<"')\">"<<strXML<<"</td>\n";
                }
                
            }else{
                if(cellNum==4){str<<"<td role=\"gridcell\" title=\""<<strXML<<"\" id=\"r"<<a<<"c"<<cellNum<<
                    "\">"<<searchResult[index]<<"</td>\n";
                }
            }
        }
        str<< "</tr>";
        a++;
    }
    result=str.str();
    
}

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
    long int sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    unsigned long int startIndex=0;
    unsigned long int ln, index=0;
    path=fileName(path);
    
    startIndexPtr=textBuffer;
    vector<string>record;
    string str;
    
    int step=0;
    int mode=0;
    //if(inputData.data["ln"]=="tib")mode=1;
    
    for(unsigned long int i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<hex<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout<<"index@@@";
            ln=i-startIndex;
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(step==100000){step=0;
                cout<<index<<" ";
            }step++;
            str.resize(ln);
            memcpy(&str[0],startIndexPtr,ln);
            startIndexPtr+=ln+1;
            startIndex=i+1;
            
            TString st;
            st+=str;
            push_back(&st);
        }
    }
};

/** импорт записей из TXT построчно по разделителю \n или \r с записью ключей в файл path1*/
void GVector::importDict(map<string,uint>keyMap,string &path,string &path1){
    char *textBuffer, *startIndexPtr;
    char c;
    long int sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    unsigned long int startIndex=0;
    unsigned long int ln, index=0;
    path=fileName(path);
    
    startIndexPtr=textBuffer;
    string keyIndex;
    string str;
    vector<string>record;
    
    int step=0;
    
    for(unsigned long int i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<hex<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout<<"index@@@";
            ln=i-startIndex;
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(step==100000){step=0;
                cout<<index<<" ";
            }step++;
            str.resize(ln);
            memcpy(&str[0],startIndexPtr,ln);
            startIndexPtr+=ln+1;
            startIndex=i+1;
            record=explode("\t",str);
            //if(record.size()<2)continue;
            TString st;
            for(int t=0;t<record.size();t++)st+=record[t];
            push_back(&st);
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
    long int sizeText=readInMemory(&textBuffer,path);
    //cout<<"sizeText="<<sizeText<<endl;
    unsigned long int startIndex=0;
    unsigned long int ln, index=0;
    
    startIndexPtr=textBuffer;
    vector<string>record;
    string str;
    int step=0;
    
    for(unsigned long int i=0;i<sizeText;i++){
        c=textBuffer[i];
        ////cout<<(short)c<<"-"; //cout<<c<<" ";
        if(c=='\r'||c=='\n'||i==sizeText-1){  //cout_<<i<<" ";
            ln=i-startIndex;
            //next we set all part of string in hash index
            //cout<<"ln="<<ln<<endl;
            if(ln==0)continue;
            index++;
            if(step==100000){step=0;cout<<index<<" ";
            }step++;
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
            push_back(&st);
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
	int i;
    vector<string> nameArray; nameArray.resize(100);
	cout<<"path="<<path<<END;
	
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;return;
	}
	
	letterSet = doc.child("FMPXMLRESULT");
    metadata = letterSet.child("METADATA");
	//from first we get map key name from field name 
	TString strT; i=0;
	for (field = metadata.child("FIELD"); field; field = field.next_sibling("FIELD")){
		strT.push_back(field.attribute("NAME").value());
        //cout<<"nameArray["<<i<<"]="<<field.attribute("NAME").value()<<END;
        i++;
	}
	push_back(&strT);
	//first record now contain database field names
	
	//в GVector в записи все поля записаны как TString
	//название поля кодируются порядком расположения в TString
 
	resultSet=letterSet.child("RESULTSET");
	for (xml_node row = resultSet.child("ROW"); row; row = row.next_sibling("ROW")){
		map<string,string>record; ///base record for all letter conversion function
		i=0;
		TString strT;
		for (Cell = row.child("COL"); Cell; Cell = Cell.next_sibling("COL")){
			Data=Cell.child("DATA");
			strT.push_back(Data.child_value());
            //cout<<" "<<Data.child_value()<<"/ ";
		}
		push_back(&strT);
        //cout<<endl;
		
	}
};

/** экспорт всех записей формата TString*/
void GVector::exportTStr(cstr path){
    string str,line;
    for(uint i=0;i<*recordCount;i++){
        TString st;
        getTStr(i,&st);
        line="";
        for(uint n=0;n<st.size();n++){
            line+=st[n]+"\t";
        }
        line+="\n";
        str+=line;
    }
    writeText(str, path);
}
/** экспорт всех записей формата TString*/
void GVector::exportTStr(string &path){
    exportTStr(path.c_str());
}

//экспорт в XML в формате FileMaker
void GVector::saveXML(string &path){

    //в GVector в записи все поля записаны как TString
	//первая запись содержит названия полей

        xml_document doc,docRecord;
        xml_node mainNode;
        xml_node book, data, key;
        mainNode=doc.append_child();
        mainNode.set_name("FMPXMLRESULT");
        TString strT;
        
        getTStr(0,&strT);
       
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
        int count=*recordCount;
        		
        for(int i=0;i<count;i++){
            data=book.append_child();
            data.set_name("ROW"); 
            TString str; getTStr(i,&str);
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


////////////////////////////////
TString::TString(void){
    len=0; sizeData=0;
    indexStr.push_back(0);
    data=&dataStr[0];
    index=(uint*)&indexStr[0];
};

TString::~TString(void){
};


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

void TString::push_back(TString *st){
    string str;
    uint markTString=0xfffffffc;
    str.resize(st->sizeData+st->len*4+12);
    memcpy(&str[0],(char*)&markTString,4);
    memcpy(&str[4],(char*)&st->len,4);
    //cout<<" st->len="<<st->len<<" st->sizeData="<<st->sizeData<<endl;
    memcpy(&str[8],(char*)st->index,st->len*4+4);
    memcpy(&str[st->len*4+12],st->data,st->sizeData);
    push_back(str);
}

void TString::push_back(string &str){
    push_back((char*)&str[0],(uint)str.size());
};

void TString::push_back(const char* str){
    push_back((char*)str,(uint)strlen(str));
};

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
    str.resize(size_);
    memcpy(&str[0],data+index[i],size_);
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

int TString::readInt(uint i){
   int dataInt;
   memcpy(&dataInt,data+index[i],4);
   return dataInt;
}

void TString::readStr(string &str, uint i){
    uint size_=index[i+1]-index[i]; 
    str.resize(size_);
    memcpy(&str[0],data+index[i],size_);
}

void TString::readIntVector(vector<int>&intVector,uint i){
    uint size_=index[i+1]-index[i];
    intVector.resize(size_/4);
    memcpy(&intVector[0],data+index[i],size_);
}

void TString::readCharVector(vector<char>&charVector,uint i){
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

