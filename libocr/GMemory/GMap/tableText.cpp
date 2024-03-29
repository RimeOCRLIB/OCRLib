#include "GMap.h"

//#define REMARK
#ifdef REMARK
#define DN(x) cout<<x;
#else
#define DN(x)
#endif

using namespace std;

// функция плотной упаковки разреженного кодового пространства текстовых букв.
// осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
void GMap::tableText(string&text,uint mode){
    

    // плотная упаковка разреженного кодового пространства текстовых букв
    DN(endl<<endl<<"*****************************************************"<<
    "********************************************************* flagPSymbol="<<flagPSymbols<<endl);
    DN("Start program *table_Text*"<<endl)
    
    ulong x;   // ,n,y
    mSIZE ds,s,olds;
    wstring str;
    wstring strTextW;              
    string  strText;     
    
    
    
    /// перекодировка букв распознаваемого текста ///
    
    // в конце текста д.б. перевод каретки \n (Enter)
    // адреса переводов каретки Enter распознаваемого текста, после перекодировки будут хранятся в отдельном массиве BufET[size_BufET]    
        

    strTextW=UTF_to_Unicode(text);
    
    // размер массива распознаваемого текста
    text_size=strTextW.size();                           /// unsigned unt *text_data=(unsigned short*)&strW_[0];
    // создание массива распознаваемого текста           /// unsigned int *text_data;
    text_data_vector->reserve(text_size*sizeof(mSIZE)); // запрос памяти c обнулением
    text_data=text_data_vector->dataPtr();
    DN("text_size="<<text_size<<endl)
    
    text_size1=(uint)text_size;  // размер распознаваемого текста в буквах, без учета служебных символов
    uint size_BufEnterT=0;      // размер массива для хранения адресов перевода каретки текста
    //flagPSymbols=REMOVE_DELIMETERS или flagPSymbols=N)_REMOVE_DELIMETERS // "REMOVE_DELIMETERS" символы пробела, точки в словаре и тексте игнорируются
    //flagPSymbols должен быть задан в вызывающей функции
    
    /// вычисление размера массива BufET для хранения адресов перевода каретки Enter текста ///
    //  пропуск кодов: пробела или точки
    
    s=0;              // "s" тоже что и переменная "x", но без учета кодов пробела или точки 
    // resultText - строка результатов вычисления вероятности букв в фразе
    resultTextVector->reserve(text_size);
    resultText=resultTextVector->dataPtr();
    if ( flagPSymbols==REMOVE_DELIMETERS ) {   // "1" символы пробела, точки в словаре и тексте удаляются  
        for(x=0; x < text_size; x++) {
            ds=text_data[x]=(ushort)strTextW[x];                // копирование строки в массив
            resultText[x]=ds;                  // resultText[x] копия text_data[x] можно оптимизировать
            if ( ds==codeEnter ) { size_BufEnterT++; }  // подсчет количества переводов каретки текста
            if ( ds!=codeSpace && ds!=codePoint ) { text_data[s]=ds; resultText[s]=ds;  s++; }  // пропуск кодов: пробела или точки
        } // x
        text_size1=s;  // размер распознаваемого текста в буквах без учета кодов пробела или точки.
        resultTextVector->reserve(text_size1);
        resultText=resultTextVector->dataPtr();
    } else {
        for(x=0; x < text_size; x++) {
            ds=text_data[x]=(ushort)strTextW[x]; // копирование строки в массив
            resultText[x]=ds;
            if ( ds==codeEnter ) { size_BufEnterT++; }  // подсчет количества переводов каретки текста
        } // x
    } // else
    
#ifdef REMARK    
    DN("@@@@4> ")
    for(x=0; x < text_size1; x++) {
        wstring st; st=(wchar_t)resultText[x]; DN(resultText[x]<<"->"<<Unicode_to_UTF(st)<<"  ");
    }
#endif    
    
    resultDelimeterVector->reserve(text_size);
    resultDelimeter=resultDelimeterVector->dataPtr();
    
    size_BufEnterT++;     
    DN("размер массива для хранения адресов перевода каретки текста  size_BufEnterT="<<size_BufEnterT<<endl)     
    DN("text_size="<<text_size<<endl) 
    DN("размер распознаваемого текста в буквах без учета кодов пробела или точки  text_size1="<<text_size1<<endl)
    
    
    // размеры массивов                
    size_BufMpT=nRank*nRank;              // вычисление размера массива, size_BufMpT=size_BufMp=nRank*nRank;
    size_BufET=size_BufEnterT;
    size_BufUpT=(uint)nLetterP;  //=nLetterP;   // sizeof(unsigned short) // ALPHABET_SIZE
    size_BufUpT1=(uint)nLetterP; //=nLetterP;   // sizeof(unsigned short) // ALPHABET_SIZE
    
    BufMpT_vector->reserve(size_BufMpT);  // зеркальный массив пар букв BufMpT текста
    BufMpT=BufMpT_vector->dataPtr();
    
    BufET_vector->reserve(size_BufET);      // массив (int) для хранения адресов перевода каретки Enter текста
    BufET=BufET_vector->dataPtr();
    
    BufUpT_vector->reserve(size_BufUpT);  // массив для восстановления исходного кода первой буквы пары из упакованного кода пар букв текста  
    BufUpT=BufUpT_vector->dataPtr();

    BufUpT1_vector->reserve(size_BufUpT1); // тоже что и массив BufUpT но состоит из иэ char 0 или 255 (11111111) и служит для                                                              
    // вычисления Max длины совпавшего фрагмента текста с непрерывным фрагментом фразы словаря.
    BufUpT1=BufUpT1_vector->dataPtr();
    
#ifdef REMARK    
    // вывод на экран распознаваемого текста text_data в виде кода букв (short) 
    DN(endl<<"5 - вывод на экран распознаваемого текста text_data в виде кода букв (short)"<<endl)
    for(x=0; x < text_size1; x++) { DN(text_data[x]<<" "); }  DN(endl) // x++;  x+=2; // dict_size  table_size
    
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DN(endl<<"6 - вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)"<<endl)
    for(x=0; x < text_size1; x++){ str=(wchar_t)text_data[x]; DN(Unicode_to_UTF(str)); }  DN(endl)
    /**/
#endif
    
    
    s=0;                      // "s" тоже что и переменная "x", но без подсчета переводов каретки Enter
    olds=0;                   // "olds" тоже что и "s", но из предыдущего шага
    ulong nEnterT=0;   // количество переводов каретки Enter в распознаваемом тексте
  
    
    // используем готовый "словарный" массив перекодировки букв BufM    
    for(x=0; x < text_size1; x++) {
        ds=text_data[x];
        
        /*if(ds==0x0fb2){
            cout<<ds<<" BufM[ds]:"<<BufM[ds]<<endl;
        }
        if(ds==0x0f74){
            cout<<ds<<" BufM[ds]:"<<BufM[ds]<<endl;
        }*/

        
        // заполнение массива адресов переводов каретки Enter 
        if ( ds==codeEnter ) { BufET[nEnterT]=olds;  nEnterT++; }   // BufM[d]=n; // d!=codeEnter||codeпробел||code.  s-1;                                             
        // игнорирование (удаление) переводов каретки Enter из массива text_data
        //else  {
            text_data[s]=BufM[ds]; olds=s; s++;
        //}
        ////          DN(d<<" ";
    } // x
    ///DN(endl)
    
    //unsigned int text_size1=s;  // размер распознаваемого текста в буквах без переводов каретки Enter
    text_size1=s;  // размер распознаваемого текста в буквах без служебных символов
    
    
#ifdef REMARK    
    // вывод на экран распознаваемого текста text_data в виде упакованного кода букв (short) 
    DN(endl<<"распознаваемый текст text_data в виде упакованного кода букв (short)"<<endl)
    for(x=0; x < text_size1; x++) { DN(text_data[x]<<" ";) }   // x++;  x+=2;
    DN(endl) 
    /**/
    
    // вывод на экран распознаваемого текста text_data, шрифтом, UTF-16 (short)
    DN(endl<<"+++ BufU - распознаваемый текст text_data, шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1; x++){
        str=(wchar_t)BufU[text_data[x]]; DN(Unicode_to_UTF(str)); // DN(Unicode_to_UTF(dictionary_data[x])
        //DN(text_data[x]<<"  ";
        if ( x==BufET[s]){ DN(endl) s++; }  //x+1==  //// ( x==BufET[s])
    }
    DN(endl)
    /**/
    
    DN(endl<<"BufU[text_data[x]]"<<endl)
    for(x=0; x < text_size1; x++) { DN(BufU[text_data[x]]<<" "); }  DN(endl)  // x++;  x+=2;
    
    DN(endl<<"BufET"<<endl)
    for(x=0; x < size_BufET; x++) { DN(BufET[x]<<" "); }  DN(endl)
    
    
    // вывод на экран распознаваемого текста text_data, шрифтом, UTF-16 (short)
    DN(endl<<"BufU - распознаваемый текст text_data, шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1-1; x++){  // количество пар на 1 меньше количества букв
        str=(wchar_t)BufU[text_data[x]]; DN(Unicode_to_UTF(str)); // DN(Unicode_to_UTF(dictionary_data[x])
        DN(text_data[x]<<"  ")
        if ( x==BufET[s]){ DN(endl) s++; }  //x+1==  //// ( x==BufET[s])
    }
    DN(endl)
    /**/   
#endif    
    
    
    /// перекодировка пар букв распознаваемого текста ///
    
    // особенность //
    // Зеркальный массив BufMpT создается только при помощи пар букв распознаваемого текста.
    // Причем используется готовый "словарный" массив перекодировки пар букв BufMp,
    // в свою очередь массив BufMp создан только при помощи пар букв словаря.          
    
    
    
    DN(endl)
    nEnterT=0;
    olds=0; s=text_data[0];   // "olds" предыдущее значение  "s"
    mSIZE m,d;
    // перекодировка пар букв распознаваемого текста с использованием готового "словарного" массива.
    // (*) (mode==1) пары букв из текста идут вперекрышку на одну букву (чешуйками. Количество пар на 1 меньше количества букв
    // (*) (mode==2) в случае поиска по неупорядоченному массиву пар пары не перекрываются и количество пар равно половине длинны кодируемой строки
    if(mode==1){
        for(x=0; x < text_size1-1; x++) {
            
            
            // эквивалентно  d=text_data[x] + text_data[x+1]*nRank;
            olds=s;    s=text_data[x+1];     /// olds=text_data[x];
            
            //if(olds==107&&s==182){
            //    cout<<"t";
            //}
            
            // формирование адреса пары букв int
            // адрес пары букв состоит из адреса первой буквы и сдвинутого адреса второй буквы
            // из 16 бит адреса используются значимые биты числом равные rank
            // суммарный адрес получается много меньше полного значения int
            d=olds + (s<<rank); // d=olds+s*nRank; // d разреженный код пар букв распознаваемого текста.
           
            //wchar_t str=(wchar_t)BufU[s];
            //cout<<" olds="<<olds<<" s="<<s<< " sC="<<Unicode_to_UTF(str)<<" d="<<d<<endl;
            /**/
            m=BufMp[d];                      // m компактый код, взятый из "словарного" массива.
            //cout<<" olds="<<olds<<" s="<<s<< " sC="<<p2letter(m)<<" d="<<d<<endl;

            
            BufUpT[m]=ds=BufU[text_data[x]]; // запись в массив BufUpT исходного кода первой буквы из пары. /// BufUpT[m]=BufU[olds];
            
            if ( ds > 0 ) { BufUpT1[m]=255; }// запись в массив BufUpT1 маски 255 (1111111100000000...). /// if ( olds > 0 )
            ////        if ( ds > 0 ) { BufUpT1[m]=128; }    // запись в массив BufUpT1 маски 255 (0000000100000000...). 
            ////        if ( ds > 0 ) { BufUpT1[m]=1; } else { BufUpT1[m]=-2; } // запись в массив BufUpT1 1, -2. /// if ( olds > 0 )
            text_data[x]=BufMpT[d]=m;        // замена разреженных пар кодов букв распознаваемого текста (d) 
            // упакованными кодам буквы (m), взятым из "словарного" зеркального массива BufMp.    
    #ifdef REMARK
            // вывод на экран
            DN("/ m="<<m<<" d="<<d<<"   ");  DN(" c=")
            str=(wchar_t)BufUpT[m]; DN(Unicode_to_UTF(str))
             if ( x==BufET[nEnterT] ){ DN(endl);}
    #endif
            
            if ( x==BufET[nEnterT] )nEnterT++; 
            /**/
            //str=(wchar_t)BufUpT[text_data[x]]; DN(Unicode_to_UTF(str)  DN("___";
        } // x
    }else{
        uint indexX=0;
        for(x=0; x < text_size1; x+=2) {
            
            // формирование адреса пары букв int
            // адрес пары букв состоит из адреса первой буквы и сдвинутого адреса второй буквы
            // из 16 бит адреса используются значимые биты числом равные rank
            // суммарный адрес получается много меньше полного значения int
            d=text_data[x] + (text_data[x+1]<<rank); // d=olds+s*nRank; // d разреженный код пар букв распознаваемого текста.
            /**/
            
            m=BufMp[d];                      // m компактнй код, взятая из "словарного" массива.
            BufUpT[m]=ds=BufU[text_data[x]]; // запись в массив BufUpT исходного кода первой буквы из пары. /// BufUpT[m]=BufU[olds];
            
            if ( ds > 0 ) { BufUpT1[m]=255; }// запись в массив BufUpT1 маски 255 (1111111100000000...). /// if ( olds > 0 )
            ////        if ( ds > 0 ) { BufUpT1[m]=128; }    // запись в массив BufUpT1 маски 255 (0000000100000000...).
            ////        if ( ds > 0 ) { BufUpT1[m]=1; } else { BufUpT1[m]=-2; } // запись в массив BufUpT1 1, -2. /// if ( olds > 0 )
            text_data[indexX]=BufMpT[d]=m;        // замена разреженных пар кодов букв распознаваемого текста (d)
            indexX++;
            // упакованными кодам буквы (m), взятым из "словарного" зеркального массива BufMp.
            
            // вывод на экран
            ///        DR(" "<<m<<" "<<d<<"   ");  DR("+++  ")
            str=(wchar_t)BufUpT[m];
            DN(Unicode_to_UTF(str))
            if ( x==BufET[nEnterT] ){ DN(endl) nEnterT++; }
            /**/
            //str=(wchar_t)BufUpT[text_data[x]]; DR(Unicode_to_UTF(str)  DR("___";
        } // x
        DN(endl)
        text_size=indexX;
        text_size1=indexX-1;
    }
    
#ifdef REMARK    
    DN(endl)
    printInputEntry();
#endif
    
    /*
    // TMP
    for(x=BufE[1098] ; x < BufE[1099]-1; x++){
        str=(wchar_t)BufUp[dictionary_data[x]]; DN(dictionary_data[x]<<" "<<Unicode_to_UTF(str)
        // DN(Unicode_to_UTF(dictionary_data[x])  // str=(wchar_t)BufUpT[text_data[x]];
        //DN(" "<<text_data[x]<<" - "<<BufUpT[text_data[x]];
    }
    */

    
}//--------------------------------------------------------------------------------------------------------------------------

