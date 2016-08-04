#include "GMap.h"

//#define REMARK

using namespace std;

// функция плотной упаковки разреженного кодового пространства текстовых букв.
// осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
void GMap::tableText(string&text){
    
    int print=0;

#ifdef REMARK
    TIME_START
#endif
    
    // плотная упаковка разреженного кодового пространства текстовых букв
    DR(endl<<endl<<"*****************************************************"<<
    "********************************************************* flagPSymbol="<<flagPSymbols<<endl);
    DR("Start program *table_Text*"<<endl)
    
    unsigned int x,s,olds;   // ,n,y
    unsigned short ds;
    wstring str;
    wstring strTextW;              
    string  strText;     
    
    
    
    /// перекодировка букв распознаваемого текста ///
    
    // в конце текста д.б. перевод каретки \n (Enter)
    // адреса переводов каретки Enter распознаваемого текста, после перекодировки будут хранятся в отдельном массиве BufET[size_BufET]    
        

    strTextW=UTF_to_Unicode(text);
    
    
    // размер массива распознаваемого текста
    text_size=(int)strTextW.size();                           /// unsigned short *text_data=(unsigned short*)&strW_[0];
    // создание массива распознаваемого текста           /// unsigned short *text_data;
    text_data_vector=GStr<ushort>::create();
    text_data_vector->resize(text_size+32);
    text_data=(ushort*)text_data_vector->dataPtr();   // запрос памяти c обнулением
    DR("text_size="<<text_size<<endl)
    
    
    letters_data_vector=GStr<int>::create(); // Разреженный массив адресов пар букв встечающихся в распознаваемом тексте один раз (без учета одинаковых).
    // Адрес в массиве - компактный код пар букв, содержимое - позиция в распознаваемом тексте пары букв.     
    clusters_data_vector=GStr<int>::create();// Плотный массив цепочек адресов (позиций) ОДИНАКОВЫХ пар букв в распознаваемом тексте.  
    // В тексте легко встечаются одинаковые пары букв.
    clusters_copy_vector=GStr<int>::create();// копия массива clusters_data, предназначена для восстановления счетчиков цепочек с помощью memcpy
    BufTxt_vector=GStr<ushort>::create();// восстановленная через словарь копия массива распознаваемого текста
    //  nev  //
    GravTxt_vector=GStr<ushort>::create();// буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря.
    SumTxtP_vector=GStr<ushort>::create();// массив статистической устойчивости распознаваемого текста 
    // (массив частоты встречаемости пар букв)    
    BufLpT_vector=GStr<ushort>::create();
    
    
    text_size1=text_size;  // размер распознаваемого текста в буквах, без учета кодов различных символов
    unsigned int size_BufEnterT=0;      // размер массива для хранения адресов перевода каретки текста
    //flagPSymbols=OCR_DICT_NO_DELIMETERS или flagPSymbols=OCR_DICT_WITH_DELIMETERS // "OCR_DICT_NO_DELIMETERS" символы пробела, точки в словаре и тексте игнорируются
    //flagPSymbols должен быть задан в вызывающей функции
    
    /// вычисление размера массива BufET для хранения адресов перевода каретки Enter текста ///
    //  пропуск кодов: пробела или точки
    
    s=0;              // "s" тоже что и переменная "x", но без учета кодов пробела или точки 
    // resultText - строка результатов вычисления вероятности букв в фразе
    resultTextVector=GStr<ushort>::create();
    resultTextVector->resize(text_size);
    resultText=(ushort*)resultTextVector->dataPtr();
    if ( flagPSymbols==OCR_DICT_NO_DELIMETERS ) {   // "1" символы пробела, точки в словаре и тексте удаляются  
        for(x=0; x < text_size; x++) {
            ds=text_data[x]=strTextW[x];                // копирование строки в массив
            resultText[x]=(wchar_t)ds;                  // resultText[x] копия text_data[x] можно оптимизировать
            if ( ds==codeEnter ) { size_BufEnterT++; }  // подсчет количества переводов каретки текста
            if ( ds!=codeSpace && ds!=codePoint ) { text_data[s]=ds; resultText[s]=(wchar_t)ds;  s++; }  // пропуск кодов: пробела или точки
        } // x
        text_size1=s;  // размер распознаваемого текста в буквах без учета кодов пробела или точки.
        resultTextVector->resize(text_size1);
        resultText=(ushort*)resultTextVector->dataPtr();
    } else {
        for(x=0; x < text_size; x++) {
            ds=text_data[x]=strTextW[x]; // копирование строки в массив
            resultText[x]=ds;
            if ( ds==codeEnter ) { size_BufEnterT++; }  // подсчет количества переводов каретки текста
        } // x
    } // else
#ifdef REMARK    
    DR("@@@@4> ")
    for(x=0; x < text_size1; x++) {
        wstring st; st=(wchar_t)resultText[x]; DR(resultText[x]<<" "<<Unicode_to_UTF(st));
    }
#endif    
    
    resultDelimeterVector=GStr<uint>::create();
    resultDelimeterVector->resize(text_size+32); 
    resultDelimeter=(uint*)resultDelimeterVector->dataPtr();
    
    size_BufEnterT++;     
    DR("размер массива для хранения адресов перевода каретки текста  size_BufEnterT="<<size_BufEnterT<<endl)     
    DR("text_size="<<text_size<<endl) 
    DR("размер распознаваемого текста в буквах без учета кодов пробела или точки  text_size1="<<text_size1<<endl)
    
    
    // размеры массивов                
    size_BufMpT=nRank*nRank;              // вычисление размера массива, size_BufMpT=size_BufMp=nRank*nRank;
    size_BufET=size_BufEnterT;
    size_BufUpT=nLetterP;  //=nLetterP;   // sizeof(unsigned short) // 65536
    size_BufUpT1=nLetterP; //=nLetterP;   // sizeof(unsigned short) // 65536
    
    BufMpT_vector=GStr<ushort>::create();       
    BufMpT_vector->resize(size_BufMpT+32);  // зеркальный массив пар букв BufMpT текста
    BufMpT=(ushort*)BufMpT_vector->dataPtr();
    
    BufET_vector=GStr<uint>::create();  
    BufET_vector->resize(size_BufET+32);      // массив (int) для хранения адресов перевода каретки Enter текста
    BufET=(uint*)BufET_vector->dataPtr();
    
    BufUpT_vector=GStr<ushort>::create();
    BufUpT_vector->resize(size_BufUpT+32);  // массив для восстановления исходного кода первой буквы пары из упакованного кода пар букв текста  
    BufUpT=(ushort*)BufUpT_vector->dataPtr();

    BufUpT1_vector=GStr<uchar>::create();
    BufUpT1_vector->resize(size_BufUpT1+32); // тоже что и массив BufUpT но состоит из иэ char 0 или 255 (11111111) и служит для                                                              
    // вычисления Max длины совпавшего фрагмента текста с непрерывным фрагментом фразы словаря.
    BufUpT1=(uchar*)BufUpT1_vector->dataPtr();
    
#ifdef REMARK    
    // вывод на экран распознаваемого текста text_data в виде кода букв (short) 
    DR(endl<<"5 - вывод на экран распознаваемого текста text_data в виде кода букв (short)"<<endl)
    for(x=0; x < text_size1; x++) { DR(text_data[x]<<" "); }  DR(endl) // x++;  x+=2; // dict_size  table_size
    
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DR(endl<<"6 - вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)"<<endl)
    for(x=0; x < text_size1; x++){ str=(wchar_t)text_data[x]; DR(Unicode_to_UTF(str)); }  DR(endl)
    /**/
#endif
    
    
    s=0;                      // "s" тоже что и переменная "x", но без подсчета переводов каретки Enter
    olds=0;                   // "olds" тоже что и "s", но из предыдущего шага
    unsigned int nEnterT=0;   // количество переводов каретки Enter в распознаваемом тексте
  
    
    // используем готовый "словарный" массив перекодировки букв BufM    
    for(x=0; x < text_size1; x++) { 
        ds=text_data[x];  
        // заполнение массива адресов переводов каретки Enter 
        if ( ds==codeEnter ) { BufET[nEnterT]=olds;  nEnterT++; }   // BufM[d]=n; // d!=codeEnter||codeпробел||code.  s-1;                                             
        // игнорирование (удаление) переводов каретки Enter из массива text_data
        //else  {
            text_data[s]=BufM[ds]; olds=s; s++;
        //}
        ////          DR(d<<" ";
    } // x
    ///DR(endl)
    
    //unsigned int text_size1=s;  // размер распознаваемого текста в буквах без переводов каретки Enter
    text_size1=s;  // размер распознаваемого текста в буквах без переводов каретки 
    
    
#ifdef REMARK    
    // вывод на экран распознаваемого текста text_data в виде упакованного кода букв (short) 
    DR(endl<<"распознаваемый текст text_data в виде упакованного кода букв (short)"<<endl)
    for(x=0; x < text_size1; x++) { DR(text_data[x]<<" ";) }   // x++;  x+=2;
    DR(endl) 
    /**/
    
    // вывод на экран распознаваемого текста text_data, шрифтом, UTF-16 (short)
    DR(endl<<"+++ BufU - распознаваемый текст text_data, шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1; x++){ 
        str=(wchar_t)BufU[text_data[x]]; DR(Unicode_to_UTF(str)); // DR(Unicode_to_UTF(dictionary_data[x])
        //DR(text_data[x]<<"  ";
        if ( x==BufET[s]){ DR(endl) s++; }  //x+1==  //// ( x==BufET[s])
    }
    DR(endl)
    /**/
    
    DR(endl<<"BufU[text_data[x]]"<<endl)
    for(x=0; x < text_size1; x++) { DR(BufU[text_data[x]]<<" "); }  DR(endl)  // x++;  x+=2; 
    
    DR(endl<<"BufET"<<endl)
    for(x=0; x < size_BufET; x++) { DR(BufET[x]<<" "); }  DR(endl) 
    
    
    // вывод на экран распознаваемого текста text_data, шрифтом, UTF-16 (short)
    DR(endl<<"BufU - распознаваемый текст text_data, шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1-1; x++){  // количество пар на 1 меньше количества букв
        str=(wchar_t)BufU[text_data[x]]; DR(Unicode_to_UTF(str)); // DR(Unicode_to_UTF(dictionary_data[x])
        DR(text_data[x]<<"  ")
        if ( x==BufET[s]){ DR(endl) s++; }  //x+1==  //// ( x==BufET[s])
    }
    DR(endl)
    /**/   
#endif    
    
    
    /// перекодировка пар букв распознаваемого текста ///
    
    // особенность //
    // Зеркальный массив BufMpT создается только при помощи пар букв распознаваемого текста.
    // Причем используется готовый "словарный" массив перекодировки пар букв BufMp,
    // в свою очередь массив BufMp создан только при помощи пар букв словаря.          
    
    
    
    DR(endl)
    unsigned short m;   
    nEnterT=0;              
    olds=0; s=text_data[0];   // "olds" предыдущее значение  "s"
    uint d;
    // перекодировка пар букв распознаваемого текста с использованием готового "словарного" массива.
    // пары букв из текста идут вперекрышку на одну букву (чешуйками).
    for(x=0; x < text_size1-1; x++) {   // количество пар на 1 меньше количества букв
        
        
        // эквивалентно  d=text_data[x] + text_data[x+1]*nRank;
        olds=s;    s=text_data[x+1];     /// olds=text_data[x];
        

        // формирование адреса пары букв int
        // адрес пары букв состоит из адреса первой буквы и сдвинутого адреса второй буквы
        // из 16 бит адреса используются значимые биты числом равные rank
        // суммарный адрес получается много меньше полного значения int
        d=olds + (s<<rank); // d=olds+s*nRank; // d разреженный код пар букв распознаваемого текста.
       
        wchar_t str=(wchar_t)BufU[s];
        //cout<<" olds="<<olds<<" s="<<s<< " sC="<<Unicode_to_UTF(str)<<" d="<<d<<endl;
        /**/
        m=BufMp[d];                      // m компактый код, взятый из "словарного" массива.
        BufUpT[m]=ds=BufU[text_data[x]]; // запись в массив BufUpT исходного кода первой буквы из пары. /// BufUpT[m]=BufU[olds];
        
        if ( ds > 0 ) { BufUpT1[m]=255; }// запись в массив BufUpT1 маски 255 (1111111100000000...). /// if ( olds > 0 )
        ////        if ( ds > 0 ) { BufUpT1[m]=128; }    // запись в массив BufUpT1 маски 255 (0000000100000000...). 
        ////        if ( ds > 0 ) { BufUpT1[m]=1; } else { BufUpT1[m]=-2; } // запись в массив BufUpT1 1, -2. /// if ( olds > 0 )
        text_data[x]=BufMpT[d]=m;        // замена разреженных пар кодов букв распознаваемого текста (d) 
        // упакованными кодам буквы (m), взятым из "словарного" зеркального массива BufMp.    
        
        // вывод на экран        
        DR("/ m="<<m<<" d="<<d<<"   ");  DR(" c=")
        str=(wchar_t)BufUpT[m]; DR(Unicode_to_UTF(str))
        if ( x==BufET[nEnterT] ){ DR(endl) nEnterT++; }
        /**/
        //str=(wchar_t)BufUpT[text_data[x]]; DR(Unicode_to_UTF(str)  DR("___";
    } // x
    DR(endl)
    
    
#ifdef REMARK    
    // вывод на экран распознаваемого текста text_data (упакованные пары букв), шрифтом, UTF-16 (short)
    DR(endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1; x++){
        str=(wchar_t)BufUpT[text_data[x]]; DR(text_data[x]<<"-"<<Unicode_to_UTF(str)<<"; ")
        //DR(Unicode_to_UTF(dictionary_data[x]);  // str=(wchar_t)BufUpT[text_data[x]];
                //DR(" "<<text_data[x]<<" - "<<BufUpT[text_data[x]];
        if ( x==BufET[s] ){ DR(endl) s++; }  // перевод каретки Enter текста
    }
    DR(endl)
    /**/
    TIME_PRINT
#endif
    
    /*
    // TMP
    for(x=BufE[1098] ; x < BufE[1099]-1; x++){
        str=(wchar_t)BufUp[dictionary_data[x]]; DR(dictionary_data[x]<<" "<<Unicode_to_UTF(str)
        // DR(Unicode_to_UTF(dictionary_data[x])  // str=(wchar_t)BufUpT[text_data[x]];
        //DR(" "<<text_data[x]<<" - "<<BufUpT[text_data[x]];
    }
    */
    
    //exit(0);
    
      
    
}//--------------------------------------------------------------------------------------------------------------------------------------------  



//-------------------------------------------------------------------------------------------------------//

/* 
 // словарь в 520мб textDictUni_Yagpo// 142 (14+19)
 text_size=142-1;
 unsigned short text_data[142-1]={
 3904, 32, 3904, 32, 3921, 32, 58335, 32, 3921, 3908, 32, 10,
 3904, 32, 3904, 32, 3921, 32, 58335, 32, 59148, 3939, 32, 3928, 32, 60076, 32, 10,
 3904, 32, 3904, 32, 3921, 32, 58335, 32, 3939, 32, 60078, 3906, 3942, 32, 3924, 59145, 32, 60714, 3923, 32, 59448, 32, 59085, 3908, 32, 3926, 59234, 3939, 32, 3926, 3938, 32, 58786, 3921, 32, 3924, 59145, 32, 59936, 32, 57567, 32, 60685, 3923, 32, 3924, 3938, 32, 58784, 59148, 32, 10,
 //3904, 32, 3904, 32, 3921, 32, 59165, 32, 3904, 32, 3938, 57864, 32, 10,
 //3904, 32, 3904, 32, 3921, 32, 59165, 32, 58335, 32, 58518, 32, 3919, 32, 57886, 32, 3939, 59148, 32, 10,
 3904, 32, 3904, 32, 3921, 3908, 32, 58335, 32, 3942, 32, 59866, 32, 60025, 3908, 32, 10,
 3904, 32, 3904, 32, 3923, 32, 57347, 32, 58935, 57914, 32, 57885, 61364, 32, 3923, 32, 3926, 3938, 32, 58077, 32, 3938, 3908, 32, 57565, 32, 57678, 32, 58648, 32, 3906, 3923, 3942, 32, 3924, 3942, 32, 60308, 3908, 32, 3924, 59148, 32, 10 
 };
 */

/*
 // словарь в 670мб textDictUni_Uni
 text_size=114-1;
 unsigned short text_data[114-1]={
 3904, 32, 3904, 32, 3921, 32, 3923, 3954, 32, 3939, 32, 3942, 3964, 3906, 3942, 32, 3924, 3936, 3954, 32, 3942, 4008, 3923, 32, 3938, 4003, 3964, 32, 3934, 3954, 3908, 32, 3926, 3938, 3986, 4017, 3939, 32, 3926, 3938, 32, 3926, 4017, 3962, 3921, 32, 3924, 3936, 3954, 32, 3939, 4001, 3962, 32, 3906, 3956, 32, 3942, 4006, 4017, 3954, 3923, 32, 3924, 3938, 32, 3926, 4017, 3936, 3964, 32, 10, 3904, 32, 3904, 32, 3921, 32, 3938, 3954, 32, 3904, 32, 3938, 3913, 4011, 32, 10, 3904, 32, 3904, 32, 3921, 32, 3938, 3954, 32, 3923, 3954, 32, 3924, 3964, 32, 3919, 32, 3919, 3962, 32, 3939, 3936, 3964, 32, 10
 };
 */ 

/*
 // словарь в 20кб
 text_size=46+8-1+26;
 // массив распознанного текста
 unsigned short text_data[46+8-1+26]={  //3923  // 57425,3851,10,
 3926,59354,3923,3851,57607,3938,3851,57755,3906,3851,10,
 //         58618,3906,3942,3851,3939,3942,3851,59444,3928,3851,3924,3938,3851,59234,3939,3851,58644,59145,3851,59234,3939,3851,3928,3930,3923,10,
 3942,3908,3942,3851,59234,3942,3851,10,57788,3942,3851,10,
 
 //    3926,59354,3923,3851,57607,3938,3851,57755,3906,3851,10,
 
 58047,3906,3942,3851,58074,3928,3851,10,
 58074,3908,3851,10,58978,3906,3942,3851,10,57425,3851,10,3928,57788,3906,3851,10,59444,3928,3942,3851,10,
 
 58618,3906,3942,3851,3939,3942,3851,59444,3928,3851,3924,3938,3851,59234,3939,3851,58644,59145,3851,59234,3939,3851,3928,3930,3923,10
 };      
 */

///////

/*
 // проверить
 // нормализация массива текста, совместить с подсчетом размера массива для хранения адресов перевода каретки
 //    if ( text_data[0]==codeEnter ) {  text_data[0]=0; } // 3851
 //    else {
 text_size=text_size+1;
 // сдвигаем весь массив текста на 1 т.к. первая буква текста обработается не корректно
 for(x=text_size-1; x > 0 ; x--) { text_data[x+1]=text_data[x]; } 
 text_data[1]=text_data[0];  // сдвигаем первую букву текста  
 text_data[0]=codeEnter;             // обнуляем место первой буквы текста // text_data[0]=0;
 //    } // else   
 */

/*
 // используем готовый "словарный" массив перекодировки букв BufM    
 for(x=0; x < text_size; x++) { 
 d=text_data[x];  
 // игнорирование (удаление) переводов каретки Enter из массива text_data 
 if ( d!=codeEnter ) {   text_data[s]=BufM[d]; olds=s; s++; }   // BufM[d]=n; // d!=codeEnter||codeпробел||code.                                       
 // заполнение массива адресов переводов каретки Enter
 else { BufET[nEnterT]=olds;  nEnterT++; } // olds;  s-1;
 ////          DR(d<<" ";
 } // x
 DR(endl)
 */

//olds=s;  s=text_data[x];   
//d=olds+s*nRank;           // эквивалентно  d=text_data[x] + text_data[x+1]*nRank;


//BufU[n]=d;                // запись в массив BufU исходного кода буквы (для восстановления исх. кода буквы)
//BufM[d]=n;                // запись в зеркальный массив BufM упакованного кода буквы (n) 


////   unsigned short *text_data; // массив букв распознаваемого текста
///    unsigned int *BufET;       // массив прямой перекодировки пар букв словаря
///    unsigned short *BufMpT;
///    unsigned short *BufUpT;


/*
 for(x=0; x < text_size; x++) {
 text_data[x]=strTextW[x];
 //if ( text_data[x]==32 ) { text_data[x]=3851; }  // замена пробела на точку
 DR(text_data[x]<<" ";
 }
 DR(endl)
 */ 

///////////////__________Примеры текстов для поиска_____________


// для словаря textDictUni_Yagpo.txt  520мб
//string strW="ལ  ན  ལ  \n";
//string strW="ལ  ན  ལ  \nལ  ན  ལ  \n";
//string strW="ར\nགས   མ པ མ   གས   དག དང ན ན མས ཅན མས ལ  བན གགས པ གས  ན  \nགས པ ཐ \n";
//string strW="གས   མ པ མ   གས   དག དང ན ན མས ཅན མས ལ  བན གགས པ གས  ན  \nགས པ ཐ \n";
//      strText="གས   མ པ མ   གས   དག དང ན ན མས ཅན མས ལ  བན གགས པ གས  ན  \n";

//    strText="གད   མ པ མ   གས   དག དང ན ན མས ཅན མས ལ  བན གགས པ གས  ན  \n";
///    strText="གས་་་མ་པ་མ་་་གས་་་དག་དང་ན་ན་མས་ཅན་མས་ལ་་བན་གགས་པ་གས་་ན་་\n";
////    strText="གད་་་མ་པ་མ་་་གས་་་དག་དང་ན་ན་མས་ཅན་མས་ལ་་བན་གགས་པ་གས་་ན་་\n";
//strTextW=UTF_to_Unicode(strText);


// для словаря textDictUni_Uni.txt  670мб
// тексты для тестирования брать либо из словаря или тщательно проверять кодировку
// сделать наглядную проверку кодировок текста и словаря. Иначе масса хлопот.
//string strW="རིགས ཀྱི བུ རྣམ པ སུམ ཅུ རྩ གཉིས པོ དེ དག དང ལྡན ན སེམས ཅན རྣམས ལ དེ བཞིན གཤེགས པའི ཐུགས རྗེ ཆེན པོ\n";  // чистый  //////////////
///      string strW="རིགས ཀྱི བུ རྣམ པ སུམ ཅུ རྩ གཉིས པོ དེ དག དང ལྡན ན སེམས ཅན རྣམས ལ དེ བཞིན སཤེགས པའི ཐུགས རྗེ ཆེན པོ\n";  // -ག>+ས
//////          string strW="རིགས ཀྱི བུ རྣམ པ སུམ ཅུ རྩ གཉིས པོ དེ དག དང ལྡན ན སེམས ཅན རྣམས ལ དེ སཞིན གཤེགས པའི ཐུགས རྗེ ཆེན པོ\n";  // -བ>+ས // ОСНОВНОЙ ТЕСТ

//string strW="ཕྱག ལན མཛད པ འདི ལྟར རོ རོ\n";
//string strW="ཕྱག ལན ལན\n";
///   string strW="མེ་དཔུང་འབར་རྩེ་གསུམ་རྣམས\n";  //////////////

///string strW="སྙིང་གའོ། །སྐུའིམཐའཡངརྐངཔའིམཐའསྟེབསྣུནཔགངལཕུརབུགནས\n";  //
///string strW="སྙིང་གའོ། །སྐུའིམཐའཡངརྐངཔའིམཐའསྟེབསྣུནཔགངསཕུརབུགནས\n";  //

//string strW="འདིར་སྦྱོར་བ་ནི་སྔར་བཤད་པའི་རྫས་ཀྱིས་དུར་ཁྲོད་ཀྱི་རས་ལ་རྡོ་རྗེ་འབར་བ་\n";
//string strW="འདིར་སྦྱོར་འ་ནི་སྔར་བཤད་པའི་རྫས་ཀྱིས་དུར་ཁྲོད་ཀྱི་རས་ལ་རྡོ་རྗེ་འབར་བ་\n";
//string strW="འདིར་སྦྱོཔ་བ་ནི་སྔར་བཤད་པའི་རྫས་ཀྱིས་དུར་ཁྲོད་ཀྱི་རས་ལ་རྡོ་རྗེ་འབར་བ་\n";


//string strW="དང་པོར་རྟག་པར་གོམས་པར་བྱ་བར་བསྟན་ཏོ། །སྤྱོད་ལམ་རྣམ་པ་བཞི་\n";
/*    
 string strW="དང་པོར་རྟག་པར་གོམས་པར་བྱ་བར་བསྟན་ཏོ། །སྤྱོད་ལམ་རྣམ་པ་བཞི་\
 ཀར་དེ་སེམས་ལ་གཞག་པར་བྱའོ། །དེའི་ཕྱིར། སྐད་ཅིག་མ་ལས་གཞན་མི་་་་\
 སེམས། །ཞེས་བྱ་བས་བསྟན་ཏོ། །སྔོན་དུ་རྣལ་འབྱོར་གྱི་སྤྱོད་པ་བྱ་བའི་ཕྱིར་\
 ཤེས་རབ་དག་པར་བྱ་བ་བསྟན་ཏོ། །ད་ནི་ཕྱག་རྒྱ་ཆེན་པོའི་དངོས་གྲུབ་གྲུབ་པར་\
 བྱ་བའི་ཕྱིར་ཚིག་འདི་བསྟན་པར་བྱ་སྟེ། དེ་ཉིད་ཐོབ་བྱས་མིག་ཡངས་མ། །ཞེས་\
 བྱ་བ་ལ་སོགས་པ་སྨོས་སོ། །སྔོན་དུ་རང་བཞིན་དགའ་བ་སྣ་ཚོགས་ཀྱི་རྣམ་པར་\
 བསྟན་ཏོ། །ད་ནི་རང་བཞིན་དགའ་བ་གཅིག་ཉིད་རྣམ་པ་ཀུན་གྱི་ལུས་ཡིན་པ་\
 དང་༑ དེ་ཉིད་དེ་བཞིན་གཤེགས་པ་ཡིན་པའོ། །དེ་ནི་དབྱེ་བསརྣམ་པ་ལྔར་\
 འགྱུར་རོ། །དེ་ཡང་རྒྱས་པར་ཕྱེ་བས་གངྒཱའི་བྱེ་མ་སྙེད་ཀྱི་གྲངས་དཔག་ཏུ་མེད་\
 པར་འགྱུར་རོ། །དེའི་དོན་གོ་བར་བྱ་བའི་ཕྱིར། དགའ་བ་ཆེན་པོ་གཅིག་པུ་ཞེས་\
 བྱ་བ་ལ་སོགས་པས་བསྟན་ཏོ། །པདྨའི་མྱུ་གུ་རྡོ་རྗེས་ཀྱེའི་རྡོ་རྗེའི་རྒྱུད་ཀྱི་རྟོག་པ་་\
 ེས་བྱ་བ་རིམ་པར་ཕྱེ་བ་ལེའུ་གཉིས་པའི་འགྲེལ་པ་མཛད་པ་རྫོགས་སོ།། །།\
 རིགས་ངན་པ་ལས་བཏོན་ནས་ཉན་ཐོས་ཀྱི་ས་ལ་སོགས་པ་ལས་སྡོམ་པར་\
 བྱེད་པས་ན་སྡོམ་པ་ཞེས་བྱའོ། །སྡོམ་པ་དེ་གང་ལ་གནས་ཤེ་ན། དེའི་ཕྱིར།\
 ཨེཝཾལ་ནི་རབ་ཏུ་གནས། །ཞེས་བྱ་བ་སྨོས་སོ། །དེ་ལ་ཨེ་ཝཾ་ཞེས་བྱབ་ནི་ཆོས་ཀྱི་\
 འབྱུང་གནས་སོ། །བཤད་མ་ཐག་པའི་སྙོམ་པ་དེ་ནི་བདེ་བ་ཆེན་པོའི་རང་བཞིན་\
 ནོ༑ །དེའི་ཐབས་ཀྱིས་དེ་གོ་བར་འགྱུར་རོ། །དགའ་བ་རྣམས་ནི་དེ་ལས་འབྱུང་།།\
 ཞེས་བྱ་བ་ནི་སྔོན་དུ་བསྟན་པའི་དགའ་བརྣམས་ནི་དེ་ལས་སྐྱེབར་འགྱུར་རོ། །\
 སྐད་ཅིག་མ་ཡིདབྱེ་བ་ཡིས། །ཞེས་བྱ་བ་ནི་སོ་སོར་མ་ལ་སོགས་པའོ། །དེ་བཞི་ནི་\n";  //
 */ 
/*
 string strW="པར་འགྱུར་རོ། །དེའི་དོན་གོ་བར་བྱ་བའི་ཕྱིར། དགའ་བ་ཆེན་པོ་གཅིག་པུ་ཞེས་\
 བྱ་བ་ལ་སོགས་པས་བསྟན་ཏོ། །པདྨའི་མྱུ་གུ་རྡོ་རྗེས་ཀྱེའི་རྡོ་རྗེའི་རྒྱུད་ཀྱི་རྟོག་པ་་\
 ེས་བྱ་བ་རིམ་པར་ཕྱེ་བ་ལེའུ་གཉིས་པའི་འགྲེལ་པ་མཛད་པ་རྫོགས་སོ།། །།\
 རིགས་ངན་པ་ལས་བཏོན་ནས་ཉན་ཐོས་ཀྱི་ས་ལ་སོགས་པ་ལས་སྡོམ་པར་\
 བྱེད་པས་ན་སྡོམ་པ་ཞེས་བྱའོ། །སྡོམ་པ་དེ་གང་ལ་གནས་ཤེ་ན། དེའི་ཕྱིར།\
 ཨེཝཾལ་ནི་རབ་ཏུ་གནས། །ཞེས་བྱ་བ་སྨོས་སོ། །དེ་ལ་ཨེ་ཝཾ་ཞེས་བྱབ་ནི་ཆོས་ཀྱི་\
 འབྱུང་གནས་སོ། །བཤད་མ་ཐག་པའི་སྙོམ་པ་དེ་ནི་བདེ་བ་ཆེན་པོའི་རང་བཞིན་\
 ནོ༑ །དེའི་ཐབས་ཀྱིས་དེ་གོ་བར་འགྱུར་རོ། །དགའ་བ་རྣམས་ནི་དེ་ལས་འབྱུང་།།\
 ཞེས་བྱ་བ་ནི་སྔོན་དུ་བསྟན་པའི་དགའ་བརྣམས་ནི་དེ་ལས་སྐྱེབར་འགྱུར་རོ། །\
 སྐད་ཅིག་མ་ཡིདབྱེ་བ་ཡིས། །ཞེས་བྱ་བ་ནི་སོ་སོར་མ་ལ་སོགས་པའོ། །དེ་བཞི་ནི་\n";  //
 */ 


//  //
//string strW="ཀ་ཀ་ཎ་སིངྒ་\n";    

// для маленького словаря в 20кб _Yagpo
// есть двойные буквы например Тибетские "б" བ и "д" ད
//    string strW="བན་ར་ག་\nསངས་ས་\nས་\nང་\nགས་\n་\nམག་་\n";




