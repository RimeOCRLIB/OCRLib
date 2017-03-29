#include "GMap.h"
#include "GLogicProcessor.h"
#include "GMemory.h"

//#define REMARK
#ifdef REMARK
#define DN(x) cout<<x;
#else
#define DN(x)
#endif

using namespace std;

// Функции добавления записей в GMap.

void GMap::addRecords(string &path,size_t mode){
    flagPSymbols=mode;
    readDataTXT(path);
    table_Dict();
    save();   //сохранение глобальных переменных на диск
}

void GMap::addRecords(GVector *dataVector,int keyField, size_t mode){
    if(mode==OCR_DICT_HASH){
        readDataVectorHash(dataVector,keyField);
        recordCount=dataVector->size();
        reloadPtr();
        //cout<<"dataVector.size()="<<dataVector->size()<<" nEnter="<<nEnter<<endl;
        save();   //сохранение глобальных переменных на диск
        return;
    }
    if(mode==LETTER_SEARCH){
        readDataVector(dataVector,keyField);
        flagPSymbols=NO_REMOVE_DELIMETERS;
        table_Dict();
        reloadPtr();
        save();
        return;
    }
    if(mode==OCR_SEARCH){
        readDataVector(dataVector);
        flagPSymbols=REMOVE_DELIMETERS;
        table_Dict();
        reloadPtr();
        save();
        return;
    }
    if(mode==ID_SEARCH){
        TString st;
        cout<<"wordListCount:"<<wordListCount;
        valueData_vector->resize((uint)wordListCount);
        readDataVectorWordlist(dataVector,keyField);
        flagPSymbols=mode;
        nEnter=1;
        //table_Dict();
        reloadPtr();
        save();
        return;
    }
    if(mode==ID_SEARCH_DATA){
        readDataVector(dataVector);
        flagPSymbols=NO_REMOVE_DELIMETERS;
        table_Dict();
        reloadPtr();
        save();
        return;
    }
    if(mode==TEXT_SEARCH){
        readDataVector(dataVector,keyField);
        flagPSymbols=NO_REMOVE_DELIMETERS;
        table_Dict();
        reloadPtr();
        save();
        return;
    }
    if(mode==FULL_TEXT_SEARCH){
        readDataVectorText(dataVector,keyField);
        flagPSymbols=NO_REMOVE_DELIMETERS;
        table_Dict();
        reloadPtr();
        save();
        return;
    }
    
    flagPSymbols=mode;
    readDataVector(dataVector,keyField);
    table_Dict();
    reloadPtr();
    //cout<<"dataVector.size()="<<dataVector->size()<<" nEnter="<<nEnter<<endl;
    save();   //сохранение глобальных переменных на диск
}

void GMap::addRecord(TString &st){
    string key=st[0];
    uint ln=(uint)key.size();
    ulong hash=(uint)strToHash(key.c_str(), ln);
    hash=hash>>hashLimit;
    vector<uint> v;
    hashData->getStr(hash, v);
    v.push_back((uint)valueData_vector->size());     //сохраняем индекс строки в словаре
    v.push_back((uint)key.size());              //сохраняем длинну строки в словаре
    hashData->putStr(hash, v);
    valueData_vector->push_back(st);

}

void GMap::readDataTXT(string &path){
    
    int print=0;
    cout<<"Dictionary text file accepted in codepage Unicode-16 Little-Endian no BOM only"<<endl;
    cout<<"first line in dictionary must be code of ENTER - LF 0x0A;"<<endl;
    
    uint *data;
    DR("path="<<path<<endl)
    if(!is_file(path)){cout<<"no valid file"; return;}
    uint dict_size_=(uint)readInMemory((char**)&data,path);
    dict_size=dict_size_/sizeof(mSIZE);
    cout<<"dict_size_="<<dict_size_;
    dictionary_data_vector->resize(dict_size+1+128); // +32   ////////////////
    reloadPtr();
    cout<<"done resize";
    memcpy((char*)dictionary_data,data,dict_size_);
    
    DR("Done read dictionary_data, dict_size="<<dict_size<<endl)
    
    /**/
};

void GMap::readDataVector(GVector *dataVector){
    
    if(dataPath==""){
        valueData_vector->addRecords(dataVector);
        dataPath=dataVector->path();
        reloadPtr();
    }else{
        valueData_vector=dataVector;
    }
    //cout<<" valueData_vector->size()="<<valueData_vector->size()<<endl;
    
    vector<mSIZE>data;
    string key;
    ulong size=dataVector->size();
    cout<<"size="<<size;
    //плотная паковка ключей из записей GVector
    for(int i=0;i<size;i++){
        if(i%1000==0)cout<<i<<" ";
        string str;
        valueData_vector->getStr(i,str);
        //cout<<str<<endl;
        UTF_to_UnicodeVector(str,data);
        data.push_back(codeEnter);
    }
    dictionary_data_vector->resize((uint)data.size()+1+128); // +32   ////////////////
    reloadPtr();
    memcpy((char*)dictionary_data,&data[0],data.size()*sizeof(mSIZE));
    dict_size=(uint)data.size();
}

void GMap::readDataVector(GVector *dataVector,int keyField){
    
    if(dataPath==""){
        valueData_vector->addRecords(dataVector);
        dataPath=dataVector->path();
        reloadPtr();
    }else{
        valueData_vector=dataVector;
    }
    //cout<<" valueData_vector->size()="<<valueData_vector->size()<<endl;
    
    vector<mSIZE>data;
    string key;
    ulong size=dataVector->size();
    cout<<"size="<<size;
    //плотная паковка ключей из записей GVector
    for(int i=0;i<size;i++){
        if(i%10000==0)cout<<i<<" ";
        TString str;
        valueData_vector->getTStr(i,str);
        //cout<<"s="<<str.size();
        //cout<<" /"<<str[keyField]<<"/"<<i<<endl;
        key=str[keyField];
        UTF_to_UnicodeVector(key,data);
        //if(key.find("\n")!=-1){cout<<"ENTER IN THE KEY"; exit(0);}
        // Словарь должен быть закрыт переводом каретки (Enter).
        data.push_back(codeEnter);
    }
    dictionary_data_vector->resize((uint)data.size()+1+128); // +32   ////////////////
    reloadPtr();
    memcpy((char*)dictionary_data,&data[0],data.size()*sizeof(mSIZE));
    dict_size=(uint)data.size();
}


void GMap::processKeyWordlist(string &key,uint index, uint field){
    string tab="\t";
    string ret="0"; ret[0]=11;
    vector<mSIZE>data;
    vector<string>listLine;
    string name="WORDLIST";
    GVector *v=((GMemory*)inputData.longMemory)->table[name].data;
    setSearchText(key);
    if(key=="")return;
 
    pLink link;
    link.index=(uint)index;
    link.field=field;
    //кодируем все записи в key ID номерами слов. Слова и буквы не вошедшие в список слов игнорируются и заносятстя в WORDLIST_add.txt
    string text=key;
    string str=key;
    key+="\n";
    
    cout<<" "<<index<<":"<<field;
    
    ((GLogicProcessor *)inputData.logicProcessor)->encodeByID(key);
    //dictionary_data_vector->push_Ptr(&key[0],(uint)key.size());

    int n=0;
    uint id;
    uint *idPtr=(uint*)&key[0];
    uint size=sizeof(pLink);
    //cout<<"key.size():"<<key.size()<<endl;
    string line;
    string s1="";
    
    while (str.size()&&n<key.size()/4) {
        id=idPtr[n];
        if(id>wordListCount){
            cout<<"id:"<<id<<" text:"<<text<<endl;
            exit(0);
        }
        
        v->getStr(id,line);
        
        if(str.find(line)!=-1){
            TString st;
            valueData_vector->getTStrData(id, st);
            st.push_back((char*)&link, size);
            valueData_vector->putTStr(id,st);
            str=str_replace(line,s1, str);
        }
        n++;
        
    }


    
}


void GMap::processKey(string &key,uint index, uint field){
    string tab="\t";
    string ret="0"; ret[0]=11;
    ushort offset=0;
    vector<mSIZE>data;
    vector<string>listLine;
    int limit=10240;

    key=str_replace("\t"," ",key);
    key=str_replace(ret.c_str(),"\n",key);
    key=str_replace("\n","\n\t",key);
    key=str_replace("\r","\r\t",key);
    key=str_replace("།","།\t",key);
    key=str_replace("༔","༔\t",key);
    key=str_replace("༴","༴\t",key);
    key=str_replace(".",".\t",key);
    key=str_replace("!","!\t",key);
    key=str_replace("?","?\t",key);
    key=str_replace(";",";\t",key);
    key=str_replace("/","/\t",key);
    
    
    vector<string>list=explode(tab,key);
    //cout<<i<<" l:"<<list.size()<<" k:"<<key.size()<<endl;
    for(uint n=0;n<list.size();n++){
        
        if(list[n].size()<5){
            offset+=list[n].size();
            continue;
        }
        if(list[n].find("་")!=-1){
            list[n]=str_replace(" "," ",list[n]);
            listLine=explode(" ",list[n]);
            for(uint m=0;m<listLine.size();m++){
                key=listLine[m];
                if(key.size()==0){
                    offset++;
                    continue;
                }
                data.reserve(0);
                rLink st;
                //cout<<"n:"<<n<<" m:"<<m<<" "<<listLine[m]<<" o:"<<offset<<" l:"<<listLine[m].size()<<" t:"<<substr(offset,(uint)listLine[m].size(),text)<<endl;
                //st.push_back(listLine[m]);
                st.index=(uint)index;
                st.field=field;
                st.offset=offset;
                offset+=key.size()+1;
                valueData_vector->push_back((char*)&st,sizeof(rLink));
                
                setSearchText(key);
                data.resize(0);
                //key=((GLogicProcessor*)inputData.logicProcessor)->UnicodeToYagpo(key);
                UTF_to_UnicodeVector(key,data);
                if(data.size()>limit){
                    cout<<index<<" long line:"<<key<<endl;
                }
                // Словарь должен быть закрыт переводом каретки (Enter).
                data.push_back(codeEnter);
                dict_size+=data.size();
                dictionary_data_vector->push_Ptr(&data[0],(uint)data.size()*sizeof(mSIZE)); // +32   ////////////////
                //valueData_vector->reload(innerData);
                //cout<<valueData_vector->size()<<" /-> "<<dictionary_data_vector->size()<<endl;
                
            }
            offset--; //убираем последний пробел
        }else{
            data.resize(0);
            rLink st;
            //st.push_back(list[n]);
            //cout<<"n:"<<n<<" "<<list[n]<<" o:"<<offset<<" l:"<<list[n].size()<<" t:"<<substr(offset,(uint)list[n].size(),text)<<endl;
            st.index=(uint)index;
            st.field=field;
            st.offset=offset;
            offset+=list[n].size();
            valueData_vector->push_back((char*)&st,sizeof(rLink));
            string s=list[n];
            setSearchText(s);
            data.resize(0);
            UTF_to_UnicodeVector(s,data);
            if(data.size()>limit){
                cout<<index<<" long line:"<<list[n]<<endl;
            }
            // Словарь должен быть закрыт переводом каретки (Enter).
            data.push_back(codeEnter);
            dict_size+=data.size();
            dictionary_data_vector->push_Ptr(&data[0],(uint)data.size()*sizeof(mSIZE));
            //valueData_vector->reload(innerData);
            //cout<<valueData_vector->size()<<" -> "<<dictionary_data_vector->size()<<endl;
        }
    }

}


void GMap::readDataVectorText(GVector *dataVector,int keyField){
    //плотная паковка ключей из записей GVector
    //исходный текст из каждой записи разделяется на фразы, и фразы пишутся в valueData_vector с сохранением значения
    //отступа этой фразы в исходном тексте и ID номером текста
    //затем из фразы убираются знаки пунктуации и пробелы. Подготовленная для поиска фраза записывается в dictionary_data_vector

    string key;
    string r;
    dict_size=0;
    ulong size_=(ulong)dataVector->size();

    for(ulong i=0;i<size_;i++){
        if(i%100==0)
        //if(i>50)break;
        cout<<i<<endl;
        TString str;
        string text;
        dataVector->getTStr(i,str);
        key="";
        if(keyField==ALL_FIELD){
            for(int n=0;n<str.size();n++){
                key=str[n];
                processKey(key,(uint)i,n);
            }
        }else{
            key=str[keyField];
            processKey(key,(uint)i,keyField);
        }
        text=key;

        
    }
    reloadPtr();
    cout<<"valueData_vector size="<<valueData_vector->size()<<endl;
    
}

void GMap::readDataVectorHash(GVector *dataVector,int keyField){
    
    if(dataPath==""){
        valueData_vector->addRecords(dataVector);
        dataPath=dataVector->path();
        reloadPtr();
    }else{
        valueData_vector=dataVector;
    }
    //cout<<" valueData_vector->size()="<<valueData_vector->size()<<endl;
    
    string key;
    uint hash;
    hash=0xffffffff;
    hash=hash>>hashLimit;
    hash+=32;
    cout<<" hashData->size()="<<hash<<" hashLimit="<<hashLimit<<endl;
    hashData->resize(hash);
    reloadPtr();
    
    ulong size=dataVector->size();
    //cout<<"size="<<size;

    for(int i=0;i<size;i++){
        TString str;
        valueData_vector->getTStr(i,str);
        //cout<<"s="<<str.size();
        if(str.size()<keyField+1)continue;
        key=str[keyField];
        hash=strToHash(key.c_str(), (uint)key.size());
        hash=hash>>hashLimit;
        //cout<<" /"<<str[keyField]<<"/"<<i<<"/"<<hash<<endl;
        vector<uint> st;
        hashData->getStr(hash, st);
        st.push_back(i);               //сохраняем индекс строки в словаре
        st.push_back((int)key.size()); //сохраняем длинну строки в словаре
        hashData->putStr(hash, st);
    }
    //vector<int> st_;
    //hashData->getStr(testHash, st_);
    //for(int i=0;i<st_.size();i++)
    //    cout<<"@"<<i<<"="<<st_.size()<<" t1="<<st_[i]<<endl;

    //dictionary_data_vector->reserve((uint)data.size()+1+128); // +32   ////////////////
    //reloadPtr();
    //memcpy((char*)dictionary_data,&data[0],data.size()*sizeof(mSIZE));
    //dict_size=(uint)data.size();
    reloadPtr();
    save();
    //exit(0);
}

void GMap::readDataVectorWordlist(GVector *dataVector,int keyField){
    //плотная паковка ключей из записей GVector
    //исходный текст из каждой записи разделяется на фразы, и в valueData_vector записывается отступ этой фразы в исходном тексте и ID номером текста
    //затем из фразы убираются знаки пунктуации и пробелы и фраза записываается в промежуточный вектор.
    //все фразы из промежуточного вектора кодируются ID номерами слов. Подготовленные для поиска фразы записывается в dictionary_data_vector
    
    string key;
    string r;
    dict_size=0;
    ulong size_=(ulong)dataVector->size();
    
    for(ulong i=0;i<size_;i++){
        //if(i%100==0)
        //if(i<4000)continue;
        cout<<i<<endl;
        TString str;
        string text;
        dataVector->getTStr(i,str);
        key="";
        if(keyField==ALL_FIELD){
            for(int n=0;n<str.size();n++){
                key=str[n];
                processKeyWordlist(key,i,n);
            }
        }else{
            key=str[keyField];
            processKeyWordlist(key,i,keyField);
        }
        text=key;
        
        
    }
    reloadPtr();
    cout<<"valueData_vector size="<<valueData_vector->size()<<"dictionary_data_vector size="<<dictionary_data_vector->size()<<endl;
    
}

void GMap::table_Dict(){
    
    // Функция перекодирует все ключи в словарь с плотной упаковкой разреженного кодового пространства словарных букв:
    // -создает таблицы (массивы) для плотной паковки или распаковки кодов букв и пар букв.
    // -осуществляет запись на диск результатов перекодировки словаря, с использовании функция быстрого чтения больших файлов в память.
    // Данная функция для данного словаря выполняется однократно с сохранением результатов на диск (HD). Время выполнения
    // функции перекодировки словаря table_Dict составляет примерно в 4-5 раз больше чем все остальные вместе взятые функции.
    
    // Словарь должен начинаться с перевода каретки и д.б. закрыт переводом каретки (Enter).
    // Каждая запись в словаре отделена переводом каретки. Поиск записи состоящей из одиночной буквы
    // осуществляется как поиск пары иэ этой буквы и перевода каретки.
    
     dict_size=dictionary_data_vector->size();
    
    // плотная упаковка разреженного кодового пространства словарных букв
    int print=0;

    DR("Start program *table_Dict* dict_size:"<<dict_size<<endl)
    
    
    ulong x,s,olds;
    mSIZE ds,d,old_ds,n;
    wstring str;
    
    
    //TIME_START
    size_BufE=0;     // размер массива для хранения адресов перевода каретки Enter
    old_ds=dictionary_data[0];
    
    // словарь разделителей BufD (BufD) т.е. массив строк адресов разделителей
    // массив BufDR (BufDR) т.е. массив адресов начала строк в словаре разделителей BufD
    
    
    /// Цикл предварительной обработка словаря
    // Создание словаря словаря разделителей для нахождения пробелов в проверяемом тексте.
    // Время выполнения цикла: без вычисления адресов пробелов (разделителей) 2сек,
    // с вычислением адресов пробелов: статические массивы 2,6сек  динамические массивы 2,8сек (предел), вектор STL 3,6сек
    
    s=0;
    // массивы словаря разделителей (пробелов). Это компактный способ хранения разделителей (пробелов).
    GStr<ulong>*BufDRM_vector;  //аналог BufDR_vector в оперативной памяти
    BufDRM_vector=GStr<ulong>::create();
    BufDRM_vector->resize(dict_size/3);
    ulong *BufDRM=BufDRM_vector->dataPtr();
    
    GStr<ulong>*BufDM_vector;  //аналог BufD_vector в оперативной памяти
    BufDM_vector=GStr<ulong>::create();
    BufDM_vector->resize(dict_size/2);
    ulong *BufDM=BufDM_vector->dataPtr();
    
    BufM_vector->reload(innerData);
    BufM_vector->resize(ALPHABET_SIZE);
    BufU_vector->reload(innerData);
    BufU_vector->resize(ALPHABET_SIZE);
    reloadPtr();
    //BufDR_vector->reserve(dict_size/3); // массив индексов BufDR (BufDR).
    //reloadPtr();
    // Массив с адресами начала строк в словаре разделителей BufD. Строки  соответствуют фразам в массиве словаря.
    // BufDR - аналог массива перевода каретки словаря разделителей.
    BufDRM[0]=0;
    ulong indexDelimeter=1;
    ulong indexDPlace=0;
    // т.е. массив строк адресов разделителей. char в строках указывают положение пробелов в исходном в массиве фраз словаря.
    uint delimeterPlace=0;
    ulong delimeterCount=0;
    
    if ( flagPSymbols==REMOVE_DELIMETERS) {          // "1" точки и пробелы в словаре и тексте удаляются
        for(x=0; x < dict_size; x++) {
            ds=dictionary_data[x];
            
            
            //wstring st; st=(wchar_t)ds; cout<<"ds="<<Unicode_to_UTF(st)<<" ";
            
            if ( ds==codeEnter ) {
                size_BufE++; // подсчет количества переводов каретки словаря, необходим
                // для вычисление размера массива для хранения адресов перевода каретки Enter словаря
                //BufDR.push_back(delimeterCount); /// заполнение массива адресов начала строк в словаре разделителей
                BufDRM[indexDelimeter]=delimeterCount; indexDelimeter++;
                //cout<<"delimeterCount="<<(short)delimeterCount<<endl;
                delimeterPlace=0;
                //cout<<endl;
            }
            // пропуск кодов: пробела или точки. Сдвиг всего массива словаря на 1 т.к. первой буквой д.б. перевод каретки
            if ( ds!=codeSpace && ds!=codePoint ) {
                dictionary_data[s]=old_ds;  old_ds=ds;   s++;
                ///dictionary_data[s]=ds;  old_ds=ds;   s++;
                if ( ds!=codeEnter)delimeterPlace++; /// подсчет адресов разделителей в строках словаря
            }else{
                delimeterCount++; /// подсчет адресов начала строки разделителей в словаре разделителей
                //BufD.push_back(delimeterPlace); /// заполнение словаря разделителей адресами разделителей в исходной словарной статье
                BufDM[indexDPlace]=delimeterPlace; indexDPlace++;
                //  адрес разделителя слогов (делиметр) учитывается после после адреса последней буквы слова (адрес буквы + 1)
                //cout<<" "<<(short)delimeterPlace;
            }
            // пропуск кодов: пробела или точки. Без сдвига
            //if ( ds!=codeSpace && ds!=codePoint ) { dictionary_data[s]=ds;  s++; }
        } // x
        
        dict_size=s;  // размер словаря в буквах, без учета кодов различных символов (без учета кодов пробела или точки)
    } // if "1"
    else {  // "0" если символы пробела, точки в словаре отсутствуют
        for(x=0; x < dict_size; x++) {
            ds=dictionary_data[x];
            //cout<<Unicode_to_UTF((wchar_t)ds);//if(x<100)cout<<" en_"<<x;
            if ( ds==codeEnter ) {size_BufE++;}  // подсчет количества переводов каретки словаря
            dictionary_data[x]=old_ds;  old_ds=ds; // сдвиг всего массива словаря на 1 т.к. первой буквой д.б. перевод каретки
        } // x
    } // else "0"
    BufDRM[indexDelimeter]=delimeterCount; indexDelimeter++;
    // оформление начала словаря. На нулевой позиции д.б. перевод каретки Enter
    dictionary_data[0]=codeEnter;   // size_BufE++;
    dictionary_data[dict_size]=codeEnter;   // size_BufE++;
    dict_size++;

    
    size_BufD=indexDPlace; // получение размера вектора
    BufD_vector->reload(innerData);
    BufD_vector->resize(size_BufD);
    size_BufDR=(uint)indexDelimeter;// получение размера вектора
    BufDR_vector->reload(innerData);
    BufDR_vector->resize(size_BufDR);
    reloadPtr();
    
    memcpy(BufD,BufDM,size_BufD*8);
    memcpy(BufDR,BufDRM,size_BufDR*8);
    BufDRM_vector->destroy(); BufDM_vector->destroy();
    
    /////// первая фраза в словаре разделителей пока будет не верной, так как на нулевой позиции исходного словаря еще нет перевода каретки Enter
    /**/
    DR("количество разделителей в словаре разделителей "<<delimeterCount<<endl)
    DR("адрес разделителя в строке словаря "<<(short)delimeterPlace<<endl)
    
    //TIME_PRINT_
    
    size_BufE+=2;  // size_BufE++;
    DR("размер массива для хранения адресов перевода каретки x++  size_BufE="<<size_BufE<<endl)
    DR("dict_size (без учета кодов пробела или точки) = "<<dict_size<<endl)
    /**/
    //uint dict_size1=dict_size;  // размер словаря в буквах, без учета кодов различных символов
    
    DR("@1dict_size="<<dict_size<<endl)
    
    // ограничение размера словаря при выводе на экран
    ulong dict_sizeGr=100;     //dict_size; // размер словаря без переводов каретки с ограничением на вывод на экран
    ulong dict_sizeGrMax=100;
    if ( dict_sizeGr > dict_sizeGrMax ) { dict_sizeGr=dict_sizeGrMax; }
    //cout<<"*dict_sizeGr* "<<dict_sizeGr<<endl;
    
    
#ifdef REMARK
    // вывод на экран словаря dictionary_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"1a - вывод на экран словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl;
    for(x=0; x < dict_sizeGr; x++){ // dict_size  dict_sizeGr
        //cout<<"  "<<dictionary_data[x];                                /////////////////////////////////////////////////
        str=(wchar_t)dictionary_data[x]; cout<<Unicode_to_UTF(str);
    }
    cout<<endl;        // cout<<Unicode_to_UTF(dictionary_data[x]);
    
    /*
     ///cout<<endl<<"1b - вывод на экран хвоста словаря dictionary_data , как текст UTF-16 (short)"<<endl;
     ///for(x=dict_size-dict_sizeGr; x < dict_size; x++){ str=(wchar_t)dictionary_data[x]; cout<<Unicode_to_UTF(str); } cout<<endl;
     
     // вывод на экран словаря dictionary_data в виде кода букв (short)
     cout<<endl<<"2 - вывод на экран словаря dictionary_data в виде кода букв (short)"<<endl;
     for(x=0; x < dict_sizeGr; x++) { cout<<dictionary_data[x]<<" "; }  cout<<endl;  // dict_size  table_size   // x++;  x+=2;
     */
#endif
    
    
    
    
    /// перекодировка букв словаря ///   //----------------------------------------//
    
    // Фразы в словаре отделяются друг от друга переводом каретки (Enter),  \n
    // адреса переводов каретки Enter после перекодировки словаря будут хранятся в отдельном массиве BufE[nEnter]
    BufE_vector->reload(innerData);
    cout<<"size_BufE="<<size_BufE/1000000<<endl;
    BufE_vector->reserve(size_BufE+32);
    reloadPtr();
    
    n=0;                           // переменная упакованного кода буквы
    s=0;                           // "s" тоже что и переменная "x", но без подсчета переводов каретки Enter
    olds=0;                        // "olds" тоже что и "s", но из предыдущего шага
    nEnter=0;                      // количество переводов каретки Enter в словаре
    
    
    /// с переводами каретки Enter ///
    // заполняем массив dictionary_data (словарь), состоящий из букв с разреженными кодами, промежуточными
    // упакованными кодами букв, без удаления переводов каретки Enter
    n=0; ////////////
    for(x=0; x < dict_size; x++) {
        
        // нормализация начала массива словаря dictionary_data[x+p], где p сдвиг начала массива, в 20кб p=1
        //d=dictionary_data[x]=dictionary_data[x+1];
        d=dictionary_data[x];
        
        ////if ( d!=codeEnter ) {  // игнорирование (удаление) переводов каретки Enter из массива dictionary_data
        if (BufM[d]==0) {
            n++;                  // упакованный код буквы должен начинаться с 1 (не иметь 0) // if ( n > ALPHABET_SIZE )  { n=ALPHABET_SIZE-1; }
            BufU[n]=d;              // запись в массив BufU исходного кода буквы (для восстановления исх. кода буквы)
            BufM[d]=n;            // запись в зеркальный массив BufM упакованного кода буквы (n)
            dictionary_data[x]=n; // замена разреженного кода буквы (dictionary_data[x]) упакованным кодом буквы (n)
        }// if (BufM
        // замена разреженных кодов буквы словаря (d) упакованными кодам буквы (n) взятым из зеркального массива BufM
        else { dictionary_data[x]=BufM[d]; }
        ////} // if ( d!=
        // заполнение массива адресов переводов каретки Enter
        ////else { BufE[nEnter]=s;  nEnter++; } // olds;
        if ( d==codeEnter ) {BufE[nEnter]=x+1;  nEnter++; } // olds;
        ////          cout<<d<<" ";
    } // x
    /**/
    
    
    // добавление в массив адресов последнего перевода каретки Enter
    BufE[nEnter]=dict_size+1;  // nEnter++;
    recordCount=nEnter;
    
    nLetter=n;     // количество разных букв в словаре
    
    //размер словаря c переводами каретки или без переводов каретки Enter
    //в случае, если мы не убираем переводы каретки совпадает с размером исходного словаря
    dict_size1=dict_size;
    //dict_size1=s;  // размер словаря c переводами каретки или без переводов каретки Enter
    // в данной функции из словаря не удалены переводы каретки Enter, поэтому dict_size1=dict_size;
    
    // проверка размера массива адресов переводов каретки Enter
    // если предвычисленый размер массива < необходимого размера массива
    if ( size_BufE < nEnter ) {
        cout<<"НЕОБХОДИМО УВЕЛИЧИТЬ РАЗМЕР size_BufE  МАССИВА BufE"<<endl;
        cout<<"адрес функции:  libocr  GLogicProcessor  table_Dict.h    size_BufE=nPhrase*4*1.1;  "<<size_BufE<<endl;
        exit(0);
    }
    /**/
    
#ifdef REMARK
    cout<<"количество разных букв в словаре nLetter="<<nLetter<<endl;
    cout<<"количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl;
    //cout<<"размер словаря без переводов каретки dict_size1="<<dict_size1<<"   dict_size "<<dict_size<<endl;
    
    // примеры прямой и обратной перекодировки букв словаря
    // упакованный код перевода каретки = BufM[10] (исходный код перевода каретки 10, буквы и т.д. )
    cout<<"упакованный код перевода каретки BufM[10]="<<BufM[10]<<endl;
    // исходный код буквы BufU[dictionary_data[7] (упакованный код перевода каретки 7, буквы и т.д. )
    cout<<"исходный код перевода каретки BufU[7]="<<BufU[7]<<endl;
    
    // вывод на экран массива адресов переводов каретки Enter
    cout<<endl<<"вывод на экран массива адресов переводов каретки Enter"<<endl;
    for(x=0; x < dict_sizeGr; x++) { cout<<BufE[x]<<" "; }   cout<<endl; // x++;  x+=2;   // nEnter
    
    // вывод на экран словаря dictionary_data в виде упакованного кода букв (short)
    cout<<endl<<"вывод на экран словаря dictionary_data в виде упакованного кода букв (short)"<<endl;
    uint McodeEnter=BufM[codeEnter]; // упакованный код перевода каретки
    for(x=0; x < dict_sizeGr; x++) {
        d=dictionary_data[x];
        if ( d==McodeEnter ) { cout<<"<"<<d<<">"<<" "; }
        else { cout<<d<<" "; }
    }
    cout<<endl;
    
    //// Просмотр массива [ALPHABET_SIZE] компактного (упорядоченный) кода буквы. Адресом массива является шрифтовой код буквы
    //cout<<"Просмотр массива BufU [ALPHABET_SIZE]"<<endl;
    //for(x=0; x < nLetter+1; x++) { if (BufU[x]!=0) {cout<<BufU[x]<<" ";} else cout<<"."; }
    //cout<<endl;
    
    // вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"BufU - вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl;
    for(n=0; n < dict_sizeGr/100; n++){  // nEnter-1
        if ( BufE[n] < BufE[n+1] ) {
            for(x=BufE[n]; x < BufE[n+1]; x++){
                ////                str=(wchar_t)BufU[dictionary_data[x]]; cout<<dictionary_data[x]<<Unicode_to_UTF(str)<<"/";
                str=(wchar_t)BufU[dictionary_data[x]]; cout<<Unicode_to_UTF(str);
                //cout<<x<<" ";
            } // x
            cout<<endl;
        } // if
    } // n
    cout<<endl;
    /**/
#endif
    
    
    /// перекодировка пар букв словаря ///   //----------------------------------------//
    
    // округленное значение nLetter (количество разных букв в словаре) до ближайшей степени двойки (в большую сторону)
    // равно nRank, причем nRank>=nLetter, например если nLetter=294 то nRank=512. Сама степень двойки равна rank=9
    nRank=1;  rank=0;
    while( nRank < nLetter ){ nRank<<=1; rank++; }  // <<1 умножение на 2
    if ( nRank > ALPHABET_SIZE )  { nRank=ALPHABET_SIZE; }
    DR("nLetter="<<nLetter<<",   nRank как степень двойки="<<nRank<<endl)
    DR("rank степень двойки="<<rank<<",   nRank*nRank="<<nRank*nRank<<endl)
    
    size_BufMp=BUFF_DICT_SIZE=nRank*nRank;  // размер зеркального массива BufMp пар букв словаря
    BUFF_DICT_SIZE+=32;
    if(BUFF_DICT_SIZE>ALPHABET_SIZE)BUFF_DICT_SIZE=ALPHABET_SIZE;
    BufMp_vector->reload(innerData);
    BufMp_vector->reserve(size_BufMp);   // запрос памяти c обнулением sizeof(unsigned short)
    BufUp_vector->reload(innerData);
    BufUp_vector->reserve(BUFF_DICT_SIZE);
    //BufUp2_vector->reload(innerData);
    //BufUp2_vector->reserve(BUFF_DICT_SIZE);
    reloadPtr();
        
    ///TIME_START
    // заполняем массив dictionary_data, состоящий из букв с упакованными кодами, упакованными кодами пар букв
    // массив dictionary_data (словарь) уже без переводов каретки Enter
    uint m=0;    // количество разных пар букв в словаре // , nLetterP превышает max доп. значение для short
    //unsigned short m=0, nLetterP;
    olds=0; s=dictionary_data[0];  // "olds" предыдущее значение  "s"
    
   
    ///uint d1,d2;
    //int maxL=0; //для определения размерности буфра адресов пар BufMp и BufUp
    for(x=0; x < dict_size-1 ; x++) { // dict_size ИЗМЕНИЛСЯ // (x=1; x < dict_size1; x++)   //if ( s > ALPHABET_SIZE/2) { s=ALPHABET_SIZE/2; } // dict_size1-2
        
        olds=s;    s=dictionary_data[x+1];
        // формирование адреса пары букв
        d=olds|(s*nRank);    // d=olds+s*nRank; // d=olds + (s<<rank); // d разреженный код пар букв словаря.
        //if(d>maxL)maxL=d; //для определения размерности буфра адресов пар BufMp и BufUp
        
        // формирование адреса пары букв
        ///       d1=dictionary_data[x];   d2=dictionary_data[x+1];
        // эквивалентно  d=dictionary_data[x] + dictionary_data[x+1]*nRank;
        ///       d=d1|(d2*nRank);        // d разреженный код пар букв словаря. "|" поразрядное "ИЛИ"
        
        if (BufMp[d]==0) {
            m++;
            if (m > BUFF_DICT_SIZE){
                cout<<"exit !!! количество различных пар букв в словаре nLetterP=m="<<m<<" превышает max доп. значение для ALPHABET_SIZ("<<ALPHABET_SIZE<<")"<<endl; exit(0);
            }
            // запись в массив BufUp исходного кода первой буква из пары, для восстановления исх. кода пар букв
            BufUp[m]=BufU[dictionary_data[x]];  // исходный код первой буквы из пары  /// BufUp[m]=BufU[olds];
           
            BufMp[d]=m;           // запись в зеркальный массив BufMp упакованного кода пар букв (m)
            dictionary_data[x]=m;   // замена разреженного кода пар букв (dictionary_data) упакованным кодом пар буквы (m)
            
            ///                if(BufMp[d]==3){cout<<"@@@/// d="<<d<<" x="<<x<<" m="<<m<<" d1="<<d1<<" d2="<<d2<<" d_d[x+1]*nRank="<<dictionary_data[x+1]*nRank<<" BufUp="<<Unicode_to_UTF((wchar_t)BufUp[dictionary_data[x]])<<"  BufU[d1]="<<BufU[d1]<<"  BufU[d2]="<<BufU[d2]<<"  BufU[d1]="<<Unicode_to_UTF((wchar_t)BufU[d1])<<"  BufU[d2]="<<Unicode_to_UTF((wchar_t)BufU[d2])<<endl; } // continue;
        } // if (BufMp
        // замена разреженных пар кодов букв словаря (d) упакованными кодам буквы (m) взятым из зеркального массива BufMp
        else { dictionary_data[x]=BufMp[d]; }
        
        //if(x>dict_size-dict_size/100)cout<<"d="<<d<<" n="<<Unicode_to_UTF((wchar_t)BufUp[dictionary_data[x]])<<" BufMp[d]="<<BufMp[d]<<endl;
        ///          if(x<100)cout<<"@@@ d="<<d<<" m="<<m<<" BufUp="<<BufUp[m]<<"  dict_data[x]="<<dictionary_data[x]<<endl;

    } // x
    
    ///TIME_PRINT_
    nLetterP=m+1; // количество разных пар букв в словаре
    DR("количество различных пар букв в словаре nLetterP="<<nLetterP<<endl);
    /**/
    
#ifdef REMARK
    // пример перекодировки пар букв словаря
    // исходный код Тибетской буквы <К> (первой буквы из пары) 3904, упакованный код пары букв <К.> (m)=1
    cout<<"исходный код Тибетской буквы <К> (первой буквы из пары)  BufUp[1]="<<BufUp[1]<<endl;
    
    // вывод на экран словаря dictionary_data в виде упакованного кода пар букв (short)
    cout<<endl<<"вывод на экран словаря dictionary_data в виде упакованного кода пар букв (short)"<<endl;
    for(x=0; x < dict_sizeGr; x++) { cout<<dictionary_data[x]<<" "; }   // x++;  x+=2; // dict_size  table_size
    cout<<endl;
    
    // Просмотр массива BufUp [ALPHABET_SIZE] упакованного кода пар букв.
    ///cout<<"Просмотр массива BufUp [ALPHABET_SIZE]"<<endl;   ///
    ///for(x=0; x < nLetterP+1; x++) { if (BufUp[x]!=0) {cout<<BufUp[x]<<" ";} else cout<<"."; }  cout<<endl;  ///
    
    // Просмотр массива BufMp[nRank*nRank] компактного (упорядоченный) кода буквы. Адресом массива является шрифтовой код пары буквы
    ///cout<<"Просмотр массива BufMp[nRank*nRank]"<<endl;
    ///for(x=0; x < size_BufMp; x++) { if (BufMp[x]!=0) {cout<<BufMp[x]<<" ";}}  cout<<endl;  // else cout<<".";
    
    /*
     // вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)
     cout<<endl<<"BufUp - вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl;
     s=0; // 1
     for(x=0; x < dict_sizeGr; x++){
     str=(wchar_t)BufUp[dictionary_data[x]]; cout<<Unicode_to_UTF(str); //  cout<<Unicode_to_UTF(dictionary_data[x]);
     if ( x==BufE[s] ) { cout<<endl; s++; }
     }
     cout<<endl;
     */
    
    // вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"BufUp - вывод на экран упакованного словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl;
    ulong s1,s2;
    
    for(n=0; n < 100; n++){  // nEnter-1
        printDictTextLine(n);
        s1=BufE[n];  s2=BufE[n+1];
        //cout<<"s1="<<s1<<" s2="<<s2<<" c="<<dictionary_data[x]<<endl;
        if ( s1 < s2 ) {
            for(x=s1; x < s2; x++){
                ///                  cout<<"  "<<dictionary_data[x]; //////////////////////
                ///                  cout<<"  "<<BufUp[dictionary_data[x]]; //////////////////////
                //str=(wchar_t)BufUp[dictionary_data[x]]; cout<<Unicode_to_UTF(str); // cout<<Unicode_to_UTF(dictionary_data[x]);
                cout<<p2letter(dictionary_data[x])<<" ";
                //cout<<x<<" ";
            } // x
            //TString strT;
            //valueData_vector->getTStr(n,strT);
            //if(strT.size())cout<<" - "<<strT[8]<<"/";
            cout<<endl;
        } // if
    } // n
    cout<<endl;
    /**/
    //TIME_PRINT_
#endif
    //exit(0);
    reloadPtr();
    return ;
    
}//--------------------------------------------------------------------------------------------------------------------------------------------


