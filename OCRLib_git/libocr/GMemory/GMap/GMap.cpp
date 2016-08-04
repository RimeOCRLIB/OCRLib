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


void GMap::destroy(void)
    {
        delete this;
    }


GMap::GMap(string &path){
    try
    {
        init(path);
    }
    catch(int a)
    {
        destroy();
    }
}
GMap::GMap(string &path,GVector *data){
    try
    {
        init(path,data);
    }
    catch(int a)
    {
        destroy();
    }
}
 
void GMap::init(string &path){
        //подключаем или создаем файл данных

    innerData=GVector::create(path);
    innerData->resizeData(15, 1);
    // ----- initialization
    dictionary_data_vector=GStr<ushort>::create(innerData,"Dict"); // массив словаря
    int newVector=0;
    valueData_vector=GVector::create(innerData,"Data");  //в случае если данные хранятся внутри GMap
    if(!valueData_vector->size())newVector=1;
    
    //двухмерный массив соответствия значения хеш-функции и индексов записей в векторе данных
    hashData=GVector::create(innerData,"Hash");

    /// массивы функции перекодировки букв словаря
    BufM_vector=GStr<ushort>::create(innerData,"BufM");          // зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    
    BufU_vector=GStr<ushort>::create(innerData,"BufU");          // массив для восстановления исходного кода буквы (разреженного кода)
    
    BufE_vector=GStr<uint>::create(innerData,"BufE");            // массив (int) для хранения адресов перевода каретки Enter словаря
    /// массивы функции перекодировки пар букв словаря
    BufMp_vector=GStr<ushort>::create(innerData,"BufMp");        // зеркальный массив предназначен для получения упакованного кода пар букв из исходного кода буквы
    BufUp_vector=GStr<ushort>::create(innerData,"BufUp");        // массив для восстановления исходного кода первой буквы пары
    BufUp2_vector=GStr<ushort>::create(innerData,"BufUp2");        // массив для восстановления исходного кода второй буквы пары

    
    BufD_vector=GStr<uchar>::create(innerData,"BufD");         //массив адресов разделителей в строках словаря
    BufDR_vector=GStr<uint>::create(innerData,"BufDR");        //массив адресов строк BufD
    
    vData_vector=GStr<uint>::create(innerData,"vData");   //массив внутренних переменных

    reloadPtr();
    
    codeEnter=vData_vector[0][0];            // исходная кодировка перевода каретки во всех словарях, обычно 10
    codePoint=vData_vector[0][1];            // исходная кодировка точки во всех словарях, обычно 3851
    codeSpace=vData_vector[0][2];            // исходная кодировка пробела во всех словарях, обычно 32
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    flagPSymbols=vData_vector[0][3];         // "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    constantPhrase=vData_vector[0][4];       // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.
    delta=vData_vector[0][5];                // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)
    /// выходные переменные функции перекодировки букв словаря
    dict_size1=vData_vector[0][6];           // размер словаря без переводов каретки Enter
    nLetter=vData_vector[0][7];              // количество разных букв в словаре
    nLetterP=vData_vector[0][8];             // количество разных пар букв в словаре
    nEnter=vData_vector[0][9];               // количество переводов каретки Enter в словаре
    nRank=vData_vector[0][10];
    rank=vData_vector[0][11];                 // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank
    recordCount=vData_vector[0][12];
    //cout<<"####@2 rank="<<rank<<" nRank="<<nRank<<" nLetter="<<nLetter<<" nLetterP="<<nLetterP<<endl;

    vData_vector->put(12,recordCount);         //количество записей
    
    
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
    
    // Задает пользователь.    
    // перед изменением проверить исходную кодировку пробелов и точек в тексте.
    
}

void GMap::init(string &path, GVector *data){
    //подключаем или создаем файл данных
    //cout<<" path="<<path;
    dataPath=data->path(); //save path to outer data
    innerData=GVector::create(path);
    innerData->resizeData(15, 1);
    // ----- initialization
    dictionary_data_vector=GStr<ushort>::create(innerData,"Dict"); // массив словаря
    int newVector=0;
    valueData_vector=data;  //в случае если данные хранятся во внешнем векторе
    
    //двухмерный массив соответствия значения хеш-функции и индексов записей в векторе данных
    hashData=GVector::create(innerData,"Hash");
    //при чтении данных из mmap данные необходимо однократно прочитать. В этом случае все последующие обращения к данным будут читатся из памяти.
    string str;
    uint size=*hashData->dataSize;
    str.resize(size);
    memcpy((void*)&str[0], hashData->data, size);
    //величина на которую делится полное значение хеша. Болшее значение делает хеш меньше но с большим колмчеством повторов
    hashLimit=8;
    
    //if(!valueData_vector->size())newVector=1;
    
    /// массивы функции перекодировки букв словаря
    BufM_vector=GStr<ushort>::create(innerData,"BufM");          // зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    
    BufU_vector=GStr<ushort>::create(innerData,"BufU");          // массив для восстановления исходного кода буквы (разреженного кода)
    
    BufE_vector=GStr<uint>::create(innerData,"BufE");            // массив (int) для хранения адресов перевода каретки Enter словаря
    /// массивы функции перекодировки пар букв словаря
    BufMp_vector=GStr<ushort>::create(innerData,"BufMp");        // зеркальный массив предназначен для получения упакованного кода пар букв из исходного кода буквы
    BufUp_vector=GStr<ushort>::create(innerData,"BufUp");        // массив для восстановления исходного кода первой буквы пары
    BufUp2_vector=GStr<ushort>::create(innerData,"BufUp2");        // массив для восстановления исходного кода второй буквы пары
    
    BufD_vector=GStr<uchar>::create(innerData,"BufD");         //массив адресов разделителей в строках словаря
    BufDR_vector=GStr<uint>::create(innerData,"BufDR");        //массив адресов строк BufD
    
    vData_vector=GStr<uint>::create(innerData,"vData");   //массив внутренних переменных
    
    reloadPtr();
    
    codeEnter=vData_vector[0][0];            // исходная кодировка перевода каретки во всех словарях, обычно 10
    codePoint=vData_vector[0][1];            // исходная кодировка точки во всех словарях, обычно 3851
    codeSpace=vData_vector[0][2];            // исходная кодировка пробела во всех словарях, обычно 32
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    flagPSymbols=vData_vector[0][3];         // "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    constantPhrase=vData_vector[0][4];       // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.
    delta=vData_vector[0][5];                // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)
    /// выходные переменные функции перекодировки букв словаря
    dict_size1=vData_vector[0][6];           // размер словаря без переводов каретки Enter
    nLetter=vData_vector[0][7];              // количество разных букв в словаре
    nLetterP=vData_vector[0][8];             // количество разных пар букв в словаре
    nEnter=vData_vector[0][9];               // количество переводов каретки Enter в словаре
    nRank=vData_vector[0][10];
    rank=vData_vector[0][11];                 // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank
    recordCount=vData_vector[0][12];
    //cout<<"####@2 rank="<<rank<<" nRank="<<nRank<<" nLetter="<<nLetter<<" nLetterP="<<nLetterP<<endl;
    
    // кодировки символов словаря и проверяемого текста
    codeEnter=10;      // исходныя кодировка перевода каретки во всех словарях, обычно 10
    codePoint=3851;    // исходныя кодировка точки во всех словарях, обычно 3851
    codeSpace=32;      // исходныя кодировка пробела во всех словарях, обычно 32
    ///DR("кодировка: codeEnter="<<codeEnter<<", codePoint="<<codePoint<<", codeCpace="<<codeCpace<<endl;
    
    // флаг пропуска кодов символов пробела, точки в словаре и тексте
    // Задает пользователь.
    // перед изменением проверить исходную кодировку пробелов и точек в тексте.
    //flagPSymbols// "1" символы пробела, точки в словаре и тексте игнорируются, "0" все остается как есть
    // Этот флаг для текста работает сразу. Для словаря сначала проверяется значение флага с которым он был скомпелирован,
    // если оно совпадает с значением заданным пользователем, словарь читается в память (mmap), если не совпадает, то присходит
    // перекомпиляция словаря (запуск только table_Dict(); и exit(0);, выполняется  примерно 10 секунд) с новым значением флага
    // и только за тем словарь читается в память (mmap).
    if(newVector){recordCount=0;dict_size1=0; nLetter=0; nEnter=0; nRank=0; rank=0; save();}
    reloadPtr();
    
    
}


    
void GMap::reloadPtr(){
    //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
    
    dictionary_data_vector->reload();
    valueData_vector->reloadPtr();
    hashData->reloadPtr();
    BufM_vector->reload();
    BufU_vector->reload();
    BufE_vector->reload();
    BufMp_vector->reload();
    BufUp_vector->reload();
    BufUp2_vector->reload();
    BufD_vector->reload();
    BufDR_vector->reload();
    vData_vector->reload();
    
    dictionary_data=(ushort*)dictionary_data_vector->dataPtr(); // массив словаря
    
    /// массивы функции перекодировки букв словаря 
    BufM=(ushort*)BufM_vector->dataPtr();                        // зеркальный массив предназначен для получения упакованного кода из исходного кода буквы. 
    BufU=(ushort*)BufU_vector->dataPtr();          // массив для восстановления исходного кода буквы (разреженного кода)
    BufE=(uint*)BufE_vector->dataPtr();            // массив (int) для хранения адресов перевода каретки Enter словаря
    /// массивы функции перекодировки пар букв словаря
    BufMp=(ushort*)BufMp_vector->dataPtr();         // зеркальный массив предназначен для получения упакованного кода пар букв из исходного кода буквы
    BufUp=(ushort*)BufUp_vector->dataPtr();         // массив для восстановления исходного кода первой буквы пары
    BufUp2=(ushort*)BufUp2_vector->dataPtr();       // массив для восстановления исходного кода второй буквы пары

    BufD=(uchar*)BufD_vector->dataPtr();         //массив адресов разделителей в строках словаря
    BufDR=(uint*)BufDR_vector->dataPtr();         //массив адресов строк BufD
}

uint GMap::size(){
    return recordCount;
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
    BufUp2_vector->resize(0);
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

    vector<short>keyVector;
    UTF_to_UnicodeVector(key,keyVector);
    delta=0;
    constantPhrase=keyVector.size();
    table_Text(textVector);
    table_Letters();
    tableProcessing(FULL_MATCH);

}
*/

void GMap::getKeyStr(uint index,vector<ushort>&key){
    uint oldw=BufE[index];   uint w=BufE[index+1];
    //DR("oldw="<<oldw<<" w="<<w<<endl;
    key.resize(w-oldw);
    memcpy(&key[0], dictionary_data+oldw,w-oldw);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// интерфейс вызова данных GMap
uint GMap::getKey(string & key_, int mode){
    int n;
    switch (mode){
        case ANY_MATCH:{
            tableText(key_);
            tableLetters();
            n=tableProcessingMatch(mode);
            break;
        }
        case FULL_MATCH:{
            string key=key_+"\n";  //в конце текста д.б. перевод каретки \n (Enter)
            tableText(key);
            n=tableProcessingFullMatch(mode);
            break;
        }
    }
    freeTextVectors();
    return n;
}

//get vector of records by key in mode
void GMap::getKey(string &key_, vector<uint>&searchResult,int mode){
    
    string key=key_;//+"\n";  //в конце текста д.б. перевод каретки \n (Enter)
    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
    constantPhrase=(int)key_.size(); // 4  // 8   //// фиксированного размера = 8
    // Не забыть включить оптимизацию в компиляторе, с оптимизацией работает в два раза быстрее, но без пошагового отображения переменных в отладчике.
    // LLVM GCC 4.2 Code Generation пункт Optimization  Level
    tableText(key);
    if(mode==FULL_MATCH){
        tableProcessingFullMatch(searchResult);
    }else{
        tableLetters();
        tableProcessingMatch(searchResult,100);
    }
    freeTextVectors();
}

//получение ключа по значению хеша
uint GMap::getHKey(string & key,int keyField){
    uint ln=(uint)key.size();
    uint hash=strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    //cout<<endl<<"key="<<key<<"hashLimit="<<hashLimit<<" "<<(int)key[0]<<" "<<(int)key[1]<<" "<<(int)key[2]<<endl;
    //cout<<"hash="<<hash<<endl;
    vector<int> v;
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
                valueData_vector->getTStr(index, &st);
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
    return -1;
}

//получение ключей по значению хеша
uint GMap::getHKey(vector<uint>&searchResult,string & key,int keyField){
    uint ln=(uint)key.size();
    uint hash=strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    //cout<<"hash="<<hash<<endl;
    vector<int> v;
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
                valueData_vector->getTStr(index, &st);
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
    return -1;

}

// интерфейс вызова данных GMap для проверки результатов распознавания
void GMap::getOCRKey(string &key,vector<uint>&searchResult, int mode){
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
    tableText(key);

    
    // функция применения словаря к распознаваемому тексту // word processing.
    ///table_Processing(ANY_MATCH);
    tableProcessingOCRMatch(searchResult,FULL_MATCH);
    DR("n="<<searchResult.size())

    for(int i=0;i<searchResult.size();i++){
        TString strT;
        valueData_vector->getTStr(searchResult[i],&strT);
        if(strT.size()){
            DR("i="<<i<<" s="<<strT.size()<<" /"<<strT[8]<<"/"<<" index="<<searchResult[i]<<endl)
        }else{DR("NOT FOUND")}
    }
    // функция обработки результатов ассоциативного поиска
    ///    renderResult();
    freeTextVectors();

}

#define DD(x) inputData.log<<x; inputData.log.flush();

// интерфейс вызова данных GMap для проверки результатов распознавания с учетом геометрии
void GMap::getOCRStackKey(string &key,vector<ushort>&letterX_,map<vector<short>,int>&searchResult, int mode){
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
    constantPhrase=8; //минимальное число пар в найденной фразе(пары чашуйками, см. associativeSignalProcessing.cpp). Число букв больше числа пар на одну букву. (шесть букв это 5 пар)
    DD(" tableTextStack"<<endl);
    // функция плотной упаковки разреженного кодового пространства текстовых букв.
    // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
    tableTextStack(key);
    
    // функция создания структуры для работы с реальными координатами пар букв в распознаваемом тексте
     DD(" tableLettersFontMatch"<<endl);
    tableLettersFontMatch();
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
    freeTextVectors();
     DD(" done"<<endl);

}


//поиск по языковой модели
void GMap::getAIML(string &key_,vector<uint>&searchResult, int mode){
    // полное описание находится в функции associativeSignalProcessing.cpp
    int print=0;
    string key=key_+"\n";  //в конце текста д.б. перевод каретки \n (Enter)

    // dlt максимальное количество не четких пар букв в легитимной фразе словаря
    delta=0; //0 // 2   // dlt=0; четкий поиск, dlt>0; не четкий поиск
    
    // сколько графики выводим на экран  // задается в table_Processing.cpp
    //unsigned int ng=100000;   //// 100000 // 100
        
    //TIME_START
    
    // Не забыть включить оптимизацию в компиляторе, с оптимизацией работает в два раза быстрее, но без пошагового отображения переменных в отладчике.
    // LLVM GCC 4.2 Code Generation пункт Optimization  Level
    
    /// отсюда стартуем для поиска по следующей странице ////
    
    // функция плотной упаковки разреженного кодового пространства текстовых букв.
    // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
    tableText(key);
    
    // функция создания структуры для работы с реальными координатами пар букв в распознаваемом тексте
    // выполняется один раз на весь распознаваемый текст не зависимо от размера словаря
    tableLetters();
    
    // функция применения словаря к распознаваемому тексту // word processing.
    ///table_Processing(ANY_MATCH);
    tableProcessingAIML(searchResult,ANY_MATCH);
    if(searchResult.size()){
        DR("n="<<searchResult[0])
        TString strT;
        valueData_vector->getTStr(searchResult[0],&strT);
        DR("size="<<searchResult.size()<<" searchResult[0]="<<searchResult[0]<<endl);
    }
    freeTextVectors();
}




int GMap::testLetterP(ushort letter1, ushort letter2){
    //BufM зеркальный массив предназначен для получения упакованного кода из исходного кода буквы.
    ushort l1=BufM[letter1];
    ushort l2=BufM[letter2];
    while( nRank < nLetter ){ nRank<<=1; rank++; } 
    //cout<<" letter1="<<letter1<<" letter2="<<letter2<<" l1="<<l1<<" l2="<<l2<<" nRank="<<nRank<<" nLetter="<<nLetter<<endl;
    //cout<<" nLetterP="<<nLetterP<<endl;
    
    // формирование адреса пары букв
    uint d=l1|(l2*nRank);
    
    //cout<<" BufMp[d]="<<BufMp[d]<<endl;
    // d разреженный код пар букв словаря.
    // BufMp массив упакованного кода пар букв
    if(!BufMp[d])return 0;
    return 1;
}
//декодирует компактный код первой буквы пары в строку
string GMap::p1letter(short code){
    string str;
    ushort l1=BufUp[code];
    str+=Unicode_to_UTF(l1);
    return str;
}

//декодирует компактный код пары в пару букв в строке
string GMap::p2letter(short code){
    string str;
    ushort l1=BufUp[code];
    ushort l2=BufUp2[code];
    
    str+=Unicode_to_UTF(l1);
    str+=Unicode_to_UTF(l2);
    return str;
}

void GMap::freeTextVectors(){
    
    text_data_vector->free();
    BufET_vector->free();
    BufUpT_vector->free();
    BufUpT1_vector->free();
    letters_data_vector->free();
    clusters_data_vector->free();
    clusters_copy_vector->free();
    BufTxt_vector->free();
    GravTxt_vector->free();
    SumTxtP_vector->free();
    BufLpT_vector->free();
    resultTextVector->free();
    resultDelimeterVector->free();
    BufMpT_vector->free();
    
}

void GMap::printDictEntry(uint n){
    string str;
    cout<<"вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl;
    cout<<"длинна совпавшего слова="<<xMaxEndDict-xMaxStartDict<<endl;
    uint oldw=BufE[n];
    uint w=BufE[n+1];
    uint s;
    short ds;
    
    for(uint x=oldw;  x < w-1;  x++) {
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


void GMap::printInputEntry(){
    string str;
    uint s;
    uint x;
    
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl;
    s=0;
    for(x=0; x < text_size1; x++){
        str=(wchar_t)BufUpT[text_data[x]]; cout<<Unicode_to_UTF(str);         //        cout<<text_data[x]<<"  ";
        if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter текста
    }
    cout<<endl;
    
}


