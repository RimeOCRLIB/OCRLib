//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#include "GMap.h"


void GMap::destroy(void){
    freeTextVectors();
    delete this;
}


GMap::GMap(string &path){
    try{
        init(path);
    }catch(int a){
        destroy();
    }
}
GMap::GMap(string &path,GVector *data){
    try{
        init(path,data);
    }catch(int a){
        destroy();
    }
}
GMap::GMap(string &path,GVector *dataText,GStr<mSIZE> *dataDict){
    try{
        init(path,dataText,dataDict);
    }catch(int a){
        destroy();
    }
}

void GMap::initVariables(){
    
    int newVector=0;
    if(!valueData_vector->size()&&!dictionary_data_vector->size())newVector=1;
    
    //двухмерный массив соответствия значения хеш-функции и индексов записей в векторе данных
    hashData=GVector::create(innerData,"Hash");
    
    /// массивы функции перекодировки букв словаря
    BufM_vector=GStr<mSIZE>::create(innerData,"BufM");          // зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    
    BufU_vector=GStr<mSIZE>::create(innerData,"BufU");          // массив для восстановления исходного кода буквы (разреженного кода)
    
    BufE_vector=GStr<ulong>::create(innerData,"BufE");            // массив (int) для хранения адресов перевода каретки Enter словаря
    /// массивы функции перекодировки пар букв словаря
    BufMp_vector=GStr<mSIZE>::create(innerData,"BufMp");        // зеркальный массив предназначен для получения упакованного кода пар букв из исходного кода буквы
    BufUp_vector=GStr<mSIZE>::create(innerData,"BufUp");        // массив для восстановления исходного кода первой буквы пары
    //BufUp2_vector=GStr<mSIZE>::create(innerData,"BufUp2");        // массив для восстановления исходного кода второй буквы пары
    
    
    BufD_vector=GStr<ulong>::create(innerData,"BufD");         //массив адресов разделителей в строках словаря
    BufDR_vector=GStr<ulong>::create(innerData,"BufDR");        //массив адресов строк BufD
    
    vData_vector=GStr<ulong>::create(innerData,"vData");   //массив внутренних переменных
    
    reloadPtr();
    
    codeEnter=(mSIZE)vData_vector[0][0];            // исходная кодировка перевода каретки во всех словарях, обычно 10
    codePoint=(mSIZE)vData_vector[0][1];            // исходная кодировка точки во всех словарях, обычно 3851
    codeSpace=(mSIZE)vData_vector[0][2];            // исходная кодировка пробела во всех словарях, обычно 32
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    flagPSymbols=(uint)vData_vector[0][3];         // "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    constantPhrase=(uint)vData_vector[0][4];       // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.
    delta=vData_vector[0][5];                // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)
    /// выходные переменные функции перекодировки букв словаря
    dict_size1=vData_vector[0][6];           // размер словаря без переводов каретки Enter
    nLetter=vData_vector[0][7];              // количество разных букв в словаре
    nLetterP=vData_vector[0][8];             // количество разных пар букв в словаре
    nEnter=vData_vector[0][9];               // количество переводов каретки Enter в словаре
    nRank=vData_vector[0][10];
    rank=vData_vector[0][11];                 // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank
    recordCount=vData_vector[0][12];          //количество записей
    //cout<<"####@2 rank="<<rank<<" nRank="<<nRank<<" nLetter="<<nLetter<<" nLetterP="<<nLetterP<<endl;
    
    // кодировки символов словаря и проверяемого текста
    codeEnter=10;      // исходныя кодировка перевода каретки во всех словарях, обычно 10
    codePoint=3851;    // исходныя кодировка точки во всех словарях, обычно 3851
    codeSpace=32;      // исходныя кодировка пробела во всех словарях, обычно 32
    ///DR("кодировка: codeEnter="<<codeEnter<<", codePoint="<<codePoint<<", codeCpace="<<codeCpace<<endl;
    
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    //flagPSymbols= "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    // Этот флаг для текста работает сразу. Для словаря сначала проверяется значение флага с которым он был скомпелирован,
    // если оно совпадает с значением заданным пользователем, словарь читается в память (mmap), если не совпадает, то присходит
    // перекомпиляция словаря (запуск только table_Dict(); и exit(0);, выполняется  примерно 10 секунд) с новым значением флага
    // и только за тем словарь читается в память (mmap).
    
    if(newVector){recordCount=0;dict_size1=0; nLetter=0; nEnter=0; nRank=0; rank=0; save();}
    
    
    //создаем текстовые вектора
    text_data_vector=GStr<mSIZE>::create();
    letters_data_vector=GStr<int>::create(); // Разреженный массив адресов пар букв встечающихся в распознаваемом тексте один раз (без учета одинаковых).
    // Адрес в массиве - компактный код пар букв, содержимое - позиция в распознаваемом тексте пары букв.
    clusters_data_vector=GStr<int>::create();// Плотный массив цепочек адресов (позиций) ОДИНАКОВЫХ пар букв в распознаваемом тексте.
    // В тексте легко встечаются одинаковые пары букв.
    clusters_copy_vector=GStr<int>::create();// копия массива clusters_data, предназначена для восстановления счетчиков цепочек с помощью memcpy
    BufTxt_vector=GStr<mSIZE>::create();// восстановленная через словарь копия массива распознаваемого текста
    //  nev  //
    GravTxt_vector=GStr<mSIZE>::create();// буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря.
    SumTxtP_vector=GStr<mSIZE>::create();// массив статистической устойчивости распознаваемого текста
    // (массив частоты встречаемости пар букв)
    BufLpT_vector=GStr<mSIZE>::create();
    resultTextVector=GStr<mSIZE>::create();
    resultDelimeterVector=GStr<ulong>::create();
    BufMpT_vector=GStr<mSIZE>::create();
    BufET_vector=GStr<ulong>::create();
    BufUpT_vector=GStr<mSIZE>::create();
    BufUpT1_vector=GStr<mSIZE>::create();

    
    letters_data_vector=GStr<int>::create();
    BufLpT_vector=GStr<mSIZE>::create();



}

void GMap::init(string &path){
        //подключаем или создаем файл данных

    innerData=GVector::create(path);
    
    if(!innerData->size())innerData->resizeData(15, 1);
    // ----- initialization
    dictionary_data_vector=GStr<mSIZE>::create(innerData,"Dict"); // массив словаря
    valueData_vector=GVector::create(innerData,"Data");  //в случае если данные хранятся внутри GMap
    initVariables();
}

void GMap::init(string &path, GVector *data){
    //подключаем или создаем файл данных
    //cout<<" path="<<path;
    dataPath=data->path(); //save path to outer data
    innerData=GVector::create(path);
    innerData->resizeData(15, 1);
    // ----- initialization
    dictionary_data_vector=GStr<mSIZE>::create(innerData,"Dict"); // массив словаря
    valueData_vector=data;  //в случае если данные хранятся во внешнем векторе
    initVariables();
}

void GMap::init(string &path, GVector *dataText,GStr<mSIZE> *dataDict){
    //подключаем или создаем файл данных
    //cout<<" path="<<path;
    dataPath=dataText->path(); //save path to outer data
    innerData=GVector::create(path);
    innerData->resizeData(15, 1);
    // ----- initialization
    dictionary_data_vector=dataDict; // массив словаря
    valueData_vector=dataText;  //в случае если данные хранятся во внешнем векторе
    initVariables();
}

    
void GMap::reloadPtr(){
    //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
    
    dictionary_data_vector->reload(innerData);
    valueData_vector->reload(innerData);
    hashData->reload(innerData);
    BufM_vector->reload(innerData);
    BufU_vector->reload(innerData);
    BufE_vector->reload(innerData);
    BufMp_vector->reload(innerData);
    BufUp_vector->reload(innerData);
    //BufUp2_vector->reload(innerData);
    BufD_vector->reload(innerData);
    BufDR_vector->reload(innerData);
    vData_vector->reload(innerData);

    
    dictionary_data=dictionary_data_vector->dataPtr(); // массив словаря
    /// массивы функции перекодировки букв словаря 
    BufM=BufM_vector->dataPtr();                        // зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    BufU=BufU_vector->dataPtr();          // массив для восстановления исходного кода буквы (разреженного кода)
    BufE=BufE_vector->dataPtr();            // массив (int) для хранения адресов перевода каретки Enter словаря
    /// массивы функции перекодировки пар букв словаря
    BufMp=BufMp_vector->dataPtr();         // зеркальный массив предназначен для получения упакованного кода пар букв из исходного кода буквы
    BufUp=BufUp_vector->dataPtr();         // массив для восстановления исходного кода первой буквы пары
    //BufUp2=BufUp2_vector->dataPtr();       // массив для восстановления исходного кода второй буквы пары

    BufD=BufD_vector->dataPtr();         //массив адресов разделителей в строках словаря
    BufDR=BufDR_vector->dataPtr();         //массив адресов строк BufD
}

ulong GMap::size(){
    if(nEnter){
        return nEnter;
    }
    //if()
    if(hashData->size()){
        return hashData->size();
    }
    return 0;
}

void GMap::save(){
    vData_vector->put(0,codeEnter);            // исходная кодировка перевода каретки во всех словарях, обычно 10
    vData_vector->put(1,codePoint);            // исходная кодировка точки во всех словарях, обычно 3851
    vData_vector->put(2,codeSpace);            // исходная кодировка пробела во всех словарях, обычно 32
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    vData_vector->put(3,(uint)flagPSymbols);         // "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    vData_vector->put(4,constantPhrase);       // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.
    vData_vector->put(5,delta);                // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)
    
    /// выходные переменные функции перекодировки букв словаря
    vData_vector->put(6,dict_size1);           // размер словаря без переводов каретки Enter      
    vData_vector->put(7,nLetter);              // количество разных букв в словаре
    vData_vector->put(8,nLetterP);             // количество разных пар букв в словаре
    vData_vector->put(9,nEnter);               // количество переводов каретки Enter в словаре
    vData_vector->put(10,nRank); 
    vData_vector->put(11,rank);                // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank 
    
    vData_vector->put(12,recordCount);         //количество записей
    //cout<<"####@2 rank="<<rank<<" nRank="<<nRank<<" nLetter="<<nLetter<<" nLetterP="<<nLetterP<<endl;

    void close();
}

void GMap::close(){
    dictionary_data_vector->close();
    valueData_vector->close();
}


void GMap::clear(){
    dict_size=0;
    dictionary_data_vector->resize(0);
    
    if(dataPath=="")valueData_vector->clear();
    
    BufM_vector->resize(0);

    BufU_vector->resize(0);
    
    size_BufE=0;
    BufE_vector->resize(0);
    
    size_BufMp=0;
    BufMp_vector->resize(0);
    BufUp_vector->resize(0);
    //BufUp2_vector->resize(0);
    BufD_vector->resize(0);
    
    size_BufDR=0;
    BufDR_vector->resize(0);
    vData_vector->resize(0);
    
    dict_size1=0;       // размер словаря без переводов каретки Enter
    nLetter=0;        // количество разных букв в словаре
    nLetterP=0;         // количество разных пар букв в словаре
    nEnter=0;           // количество переводов каретки Enter в словаре
    dict_size=0;           // размер массива словаря
    BUFF_DICT_SIZE=0;      // количество пар букв в словаре. Вычисляется в момент создания словаря
    
     reloadPtr();
     save();
}


/*
void GMap::getTStr(string &key,TString *strT){

    vector<uint>keyVector;
    UTF_to_UnicodeVector(key,keyVector);
    delta=0;
    constantPhrase=keyVector.size();
    table_Text(textVector);
    table_Letters();
    tableProcessing(FULL_MATCH);

}
*/

void GMap::getKeyStr(ulong index,vector<uint>&key){
    ulong oldw=BufE[index];   ulong w=BufE[index+1];
    //DR("oldw="<<oldw<<" w="<<w<<endl;
    key.resize(w-oldw);
    memcpy(&key[0], dictionary_data+oldw,w-oldw);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// интерфейс вызова данных GMap
ulong GMap::getKey(string & key_, int mode){
    ulong n=0;
    switch (mode){
        case ANY_MATCH:{
            tableText(key_,1);
            tableLetters();
            n=tableProcessingMatch(mode);
            break;
        }
        case FULL_MATCH:{
            string key=key_+"\n";  //в конце текста д.б. перевод каретки \n (Enter)
            tableText(key,1);
            n=(uint)tableProcessingFullMatch(mode);
            break;
        }
    }
    return n;
}

//get index and field in search data vector by words ID in query
void GMap::getKeyID(string &key,vector<pLink>&searchResult){
    
    if(key.size()>4){
        uint *p=(uint*)&key[0];
        uint id=p[0];
        TString st;
        valueData_vector->getTStrData(id, st);
        string str=st[0];
        cout<<"st:"<<st.size()<<" str:"<<str.size()<<endl;
        pLink *link=(pLink*)&str[0];
        cout<<"index:"<<link->index<<" field:"<<(short)link->field<<endl;
        cout<<"";
        
    }


}

//Кодирование фразы текста ID номерами слов
void GMap::encodeID(string &vData){
    tableText(vData,1);
    tableLetters();
    GStr<uint>*res=GStr<uint>::create();
    GStr<uint>*result=GStr<uint>::create();
    tableProcessingID(res);

    
    string key;
    int i=0;
    string str=vData;
    str=str_replace("\n", "",str);
    string id;
    id.resize(4);
    string s="";
    string s1="@";
    while(str.size()&&i<res->size()){
        valueData_vector->getStr(res[0][i],key);
        //cout<<i<<" key:"<<key<<endl;
        str=str_replace(key,s,str);
        if(vData.find(key)!=-1){
            result->push_back(res[0][i]);
        }
        vData=str_replace(key,s1,vData);
        i++;
    }
    //cout<<"vData:"<<vData.size()<<" str:"<<str<<"/"<<endl;
    
    if(str.size()!=0){
        //cout<<"str:"<<str<<endl;
        vector<string>v;
		explode("@",vData,v);
        string path=inputData.data["root"]+"/OCRData/DATA_DB/WORDLIST/WORDLIST_add.txt";
        for(int i=0;i<v.size();i++){
            if(!v[i].size())continue;
            if(v[i].size()>5){
                //cout<<"<"<<v[i]<<">"<<endl;
                s=v[i]+"\n";
                wstring d=UTF_to_Unicode(s);
                if(d.size()>5){
                    writeFileAppend(s,path);
                }
            }
        }
    
    }
    uint c=codeEnter;
    vData.resize(result->size()*4+4);
    memcpy(&vData[0], result->dataPtr(), result->size()*4);
    memcpy(&vData[0]+result->size()*4, &c, 4);
    res->destroy();
    result->destroy();
    
}

//get vector of records by key in mode
void GMap::getKey(string &key_, vector<ulong>&searchResult,int mode){
    
    string key=key_;//+"\n";  //в конце текста д.б. перевод каретки \n (Enter)
    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
    constantPhrase=(int)key_.size(); // 4  // 8   //// фиксированного размера = 8
    // Не забыть включить оптимизацию в компиляторе, с оптимизацией работает в два раза быстрее, но без пошагового отображения переменных в отладчике.
    // LLVM GCC 4.2 Code Generation пункт Optimization  Level
    tableText(key,1);
    if(mode==FULL_MATCH){
        tableProcessingFullMatch(searchResult);
    }else{
        tableLetters();
        wstring keyW=UTF_to_Unicode(key);
        tableProcessingMatch(keyW,searchResult,200);
    }
}

//получение ключа по значению хеша
ulong GMap::getHKey_(string & key,int keyField){
    //return 0xffffffffffffffff;
    uint ln=(uint)key.size();
    uint hash=strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    //cout<<endl<<"key="<<key<<"hashLimit="<<hashLimit<<" "<<(int)key[0]<<" "<<(int)key[1]<<" "<<(int)key[2]<<endl;
    //cout<<"hash:"<<hash<<" hashLimit:"<<hashLimit<<" hashData->size():"<<hashData->size()<<endl;
    uint *p=0;
    uint size=0;
    
    if(hash>=hashData->size()){return 0xffffffffffffffff;}
    hashData->getStr(hash, &p, size);
    if(size>10000){cout<<"hash error size:"<<size<<"hash:"<<hash<<" key:"<<key<<endl; return 0xffffffffffffffff;}
    //return 1;
    if(size>1){
        //hashCount++;
        //if(s>2){
        //cout<<"hS="<<s<<" ";
        //hashError++; //cout<<hashError<<" ";
        //if(s>maxHashError)maxHashError=s;
        //}
        //проверяем записи с одинаковым хешем
        for(int i=0;i<size-1;i++){
            //получаем длинну записи
            uint l=p[i+1];
            //cout<<"l="<<l<<" ln="<<ln<<endl;
            if(l==ln){
                uint index =p[i];
                //cout<<"index="<<index<<endl;
                //проверяем значение записи
                TString st;
                valueData_vector->getTStr(index, st);
                if(st.size()){
                    if(key==st[keyField]){
                        return index;
                    }
                }
                //cout<<"key="<<st[0]<<endl;
                //for(int a=0;a<st.size();a++)cout<<a<<"="<<st[a]<<" ";
                //cout<<endl;
            }
            i++;
        }
        
    }
    //exit(0);
    return 0xffffffffffffffff;
}


//получение ключа по значению хеша
ulong GMap::getHKey(string & key,int keyField){
    uint ln=(uint)key.size();
    uint hash=strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    //cout<<endl<<"key="<<key<<"hashLimit="<<hashLimit<<" "<<(int)key[0]<<" "<<(int)key[1]<<" "<<(int)key[2]<<endl;
    //cout<<"hash="<<hash<<endl;
    vector<uint> v;
    hashData->getStr(hash, v);
    uint s=(uint)v.size();
    
    if(s>1){
        //hashCount++;
        //if(s>2){
            //cout<<"hS="<<s<<" ";
            //hashError++; //cout<<hashError<<" ";
            //if(s>maxHashError)maxHashError=s;
        //}
        //проверяем записи с одинаковым хешем
        for(int i=0;i<s;i++){
          //получаем длинну записи
            uint l=v[i+1];
            //cout<<"l="<<l<<" ln="<<ln<<endl;
            if(l==ln){
                uint index =v[i];
                //cout<<"index="<<index<<endl;
                //проверяем значение записи
                TString st;
                valueData_vector->getTStr(index, st);
                if(st.size()){ 
                    if(key==st[keyField]){
                        return index;
                    }
                }
                //cout<<"key="<<st[0]<<endl;
                //for(int a=0;a<st.size();a++)cout<<a<<"="<<st[a]<<" ";
                //cout<<endl;
            }
            i++;
        }
    
    }
    //exit(0);
    return 0xffffffffffffffff;
}

//получение ключей по значению хеша
ulong GMap::getHKey(vector<ulong>&searchResult,string & key,int keyField){
    uint ln=(uint)key.size();
    uint hash=strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    //cout<<endl<<"key="<<key<<"hashLimit="<<hashLimit<<" "<<(int)key[0]<<" "<<(int)key[1]<<" "<<(int)key[2]<<endl;
    //cout<<"hash="<<hash<<" hash size="<<hashData->size()<<endl;

    vector<uint> v;
    hashData->getStr(hash, v);
    uint s=(uint)v.size();
    
    if(s>1){
        //hashCount++;
        //if(s>2){
        //hashError++; //cout<<hashError<<" ";
        //if(s>maxHashError)maxHashError=s;
        //}
        //проверяем записи с одинаковым хешем
        for(int i=0;i<s;i++){
            //получаем длинну записи
            uint l=v[i+1];
            //if(print)cout<<"l="<<l<<" ln="<<ln<<endl;
            if(l==ln){
                uint index =v[i];
                //if(print)cout<<"index="<<index<<endl;
                //проверяем значение записи
                TString st;
                valueData_vector->getTStr(index, st);
                if(st.size()){
                    //if(print)cout<<"key="<<st[0]<<endl;
                    if(key==st[keyField]){
                        searchResult.push_back(index);
                    }
                }
                
                
            }
            i++;
        }
        
    }
    //exit(0);
    return 0xffffffffffffffff;

}

// интерфейс вызова данных GMap для проверки результатов распознавания
void GMap::getOCRKey(string &key,vector<ulong>&searchResult, int mode){
    // полное описание находится в не используемой функции associativeSignalProcessing.cpp
    int print=0;
    key+="\n";  //в конце текста д.б. перевод каретки \n (Enter)
    //каждая пара букв в тексте поиска должна быть в двух вариантах - закрыта переводом каретки
    //и без перевода каретки.
    
    
    // dlt максимальное количество не четких пар букв в легитимной фразе словаря
    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    
    // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
    constantPhrase=8; // 4  // 8   //// фиксированного размера = 8
    
    // функция плотной упаковки разреженного кодового пространства текстовых букв.
    // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
    tableText(key,1);
    
    // функция применения словаря к распознаваемому тексту // word processing.
    ///table_Processing(ANY_MATCH);
    tableProcessingOCRMatch(searchResult,FULL_MATCH);
    DR("n="<<searchResult.size())
    if(print){
        for(int i=0;i<searchResult.size();i++){
            TString strT;
            valueData_vector->getTStr(searchResult[i],strT);
            if(strT.size()){
                DR("i="<<i<<" s="<<strT.size()<<" /"<<strT[8]<<"/"<<" index="<<searchResult[i]<<endl)
            }else{DR("NOT FOUND")}
        }
    }
    // функция обработки результатов ассоциативного поиска
    ///    renderResult();
}


#define DD(x) inputData.log<<x; inputData.log.flush();

// интерфейс вызова данных GMap для проверки результатов распознавания с учетом геометрии
void GMap::getOCRStackKey(string &key,vector<uint>&letterX_,map<vector<int>,ulong>&searchResult, int mode){
    // полное описание находится в не используемой функции associativeSignalProcessing.cpp
    int print=0;
    if(key.size()<10)return;
    key+="\n";  //в конце текста д.б. перевод каретки \n (Enter)
    //каждая пара букв в тексте поиска должна быть в двух вариантах - закрыта переводом каретки
    //и без перевода каретки.
    letterX=&letterX_[0];
    BufDictStr.resize(128);
    memset(&BufDictStr[0],0,256);
    
    // dlt максимальное количество не четких пар букв в легитимной фразе словаря
    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    
    // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
#ifdef OCR_woodblock
    constantPhrase=3; //минимальное число пар в найденной фразе(пары , см. associativeSignalProcessing.cpp). Число букв больше числа пар на одну букву. (шесть букв это 5 пар)
#else 
    constantPhrase=5;
#endif
    DD(" tableTextStack"<<endl);
    // функция плотной упаковки разреженного кодового пространства текстовых букв.
    // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
    tableText(key,2);
    
    // функция создания структуры для работы с реальными координатами пар букв в распознаваемом тексте
     DD(" tableLetters"<<endl);
    tableLetters();
    DD(" tableProcessing"<<endl);
    
    // функция применения словаря к распознаваемому тексту // word processing.
    ///table_Processing(ANY_MATCH);
    tableProcessingFontMatch(searchResult,ANY_MATCH);
    DR("n="<<searchResult.size())
    DD(" tableProcessing done"<<endl);
    //for(int i=0;i<searchResult.size();i++){
    //}
    // функция обработки результатов ассоциативного поиска
    ///    renderResult();
     DD(" done"<<endl);

}

//интерфейс вызова данных GMap для текстового словаря
void GMap::getTextKey(string &key,vector<dictKey>&searchResult, int mode){

    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
    constantPhrase=(int)key.size(); // 4  // 8   //// фиксированного размера = 8
    if(constantPhrase>16)constantPhrase=16;

    tableText(key,1);
    tableLetters();
    wstring keyW=UTF_to_Unicode(key);
    
    tableProcessingText(keyW,searchResult,100);

}



int GMap::testLetterP(uint letter1, uint letter2){
    //BufM зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    mSIZE l1=BufM[letter1];
    mSIZE l2=BufM[letter2];
    while( nRank < nLetter ){ nRank<<=1; rank++; } 
    //cout<<" letter1="<<letter1<<" letter2="<<letter2<<" l1="<<l1<<" l2="<<l2<<" nRank="<<nRank<<" nLetter="<<nLetter<<endl;
    //cout<<" nLetterP="<<nLetterP<<endl;
    
    // формирование адреса пары букв
    mSIZE d=l1|(l2*nRank);
    
    //cout<<" BufMp[d]="<<BufMp[d]<<endl;
    // d разреженный код пар букв словаря.
    // BufMp массив упакованного кода пар букв
    if(!BufMp[d])return 0;
    return 1;
}
//декодирует компактный код первой буквы пары в строку
string GMap::p1letter(mSIZE code){
    string str;
    uint l1=BufUp[code];
    str+=Unicode_to_UTF(l1);
    return str;
}

//декодирует компактный код пары букв в строку
string GMap::p2letter(mSIZE code){
    string str;
    uint l1=BufUp[code];
    str+=Unicode_to_UTF(l1);
    //l1=BufUp2[code];
    //str+=Unicode_to_UTF(l1);
    return str;
}

void GMap::freeTextVectors(){
    
    text_data_vector->destroy();
    BufET_vector->destroy();
    BufUpT_vector->destroy();
    BufUpT1_vector->destroy();
    letters_data_vector->destroy();
    clusters_data_vector->destroy();
    clusters_copy_vector->destroy();
    BufTxt_vector->destroy();
    GravTxt_vector->destroy();
    SumTxtP_vector->destroy();
    BufLpT_vector->destroy();
    resultTextVector->destroy();
    resultDelimeterVector->destroy();
    BufMpT_vector->destroy();
        
}

void GMap::printDictEntry(ulong n){
    wstring str;
    cout<<"вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl;
    ulong oldw=BufE[n];
    ulong w=BufE[n+1];
    ulong s;
    mSIZE ds;
    
    for(ulong x=oldw;  x < w-1;  x++) {
        s=x-oldw+1;
        ds=BufUpT[dictionary_data[x]];
        //if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
        ///if ( x == xMaxStartDict+oldw ) { cout_<<"|"; }       if ( x == xMaxEndDict+oldw ) { cout_<<"|"; }
        //if ( ds > 0 ) { str=(wchar_t)ds; cout<<s<<Unicode_to_UTF(str); } else { cout<<"-"; }
        if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }
    }
    cout<<endl;
    cout<<endl;

}

void GMap::printDictTextLine(ulong n){
    // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
    cout<<"n:"<<n<<" вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-8     "<<endl;
    ulong oldw=BufE[n];
    ulong w=BufE[n+1];
    uint ds;
    wstring str;
    for(ulong x=oldw;  x < w-1;  x++) {
        //////s=x-oldw+1;
        ds=BufUp[dictionary_data[x]];
        if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
        str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
        //cout<<" "<<ds<<" "<<endl;
        ///str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
    }
    //cout<<"len:"<<w-oldw<<endl;
    if(w-1-oldw>0)cout<<endl;
}


void GMap::printInputEntry(){
    wstring str;
    ulong s;
    ulong x;
    
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl;
    s=0;
    for(x=0; x < text_size1; x++){
        if(text_data[x]>0){
        str=(wchar_t)BufUpT[text_data[x]]; cout<<Unicode_to_UTF(str);
        }else{
            cout<<"-";
        }
        //if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter текста
    }
    cout<<endl;
    
}


