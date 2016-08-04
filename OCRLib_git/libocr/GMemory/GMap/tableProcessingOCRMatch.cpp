#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64

#include <math.h>   // log

//#define REMARK

using namespace std;

void GMap::tableProcessingOCRMatch(vector<uint>&searchResult,int mode){
       
    // Поиск (четкий или нечеткий) слова (стека) из текста, в словаре.
    // Проверяется совпадение (четкое или нечеткое) слова (стека) из текста, с фразой словаря.
    
    // Проверяемай текст должен состоять из пар букв и из тех же пар букв где вторая буква
    // заменена на перевод каретки (Enter). В словаре в конце каждой фразы добавлен перевод каретки.
    // В проверяемом тексте каждая пара м.б. последней.
    
    
    int print=0; // вывод отладочной информации 1 - выводим, 0 - нет
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessing_OCRMatch*"<<endl)
        
    
    unsigned int LPhrase;       // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits; // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста    
    unsigned int NPhraseHits=0; // количество совпавших фраз словаря с фразами распознаваемого текста
  
    unsigned int x,n,w,w_1,oldw;
    unsigned short ds;         
    wstring str;
    //int j=0;
    
    //exit(0);
    
TIME_START
        
    /// основной прогон поиска ///    
    // Поиск (четкий или нечеткий) слова (стека) из текста, в словаре.
    // Проверяется совпадение (четкое или нечеткое) слова (стека) из текста, с фразой словаря.
    
    NPhraseHits=0;    
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    
    // Цикл перебора фраз словаря.
    for( n=0; n < nEnter; n++ ) {  //DR(n<<" ";  //223015
       
        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
                        
            // Цикл перебора пар букв фразы
            LPhraseHits=0;   w_1=w-1; 
            for(x=oldw;  x < w_1;  x++) { 
                ds=BufUpT1[dictionary_data[x]]; //проверка, есть ли такая пара букв в тексте
                // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
                // Быстрый вариант для четкого поиска (со сбросом, если не совпадет хоть одна буква)
                LPhraseHits=(LPhraseHits + 1) & ds;
                // понятный вариант для нечеткого поиска
                /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
                
                // По достижении пороговой длины LPhrase-1 подряд идущих совпавших букв текста,
                // останавливаем подсчет длины и переходим к следующей фразе.
                if(LPhraseHits!=LPhrase-1)continue;
                else {                                        
                    searchResult.push_back(n); // номер фразы в словаре совпавшей с словом (стеком) текста
                    NPhraseHits++; 
                    // отладка и визуализация
                    DR("L="<<LPhraseHits<<" /"<<LPhrase<<endl)
#ifdef REMARK
                    if(print){
                        DR(LPhraseHits<<" LPhraseHits++++++++++++++++++++++++++++++++++++++++++++++++")
                        DR("text_size1="<<text_size1<<endl)
                        //DR("n_fullMatch="<<n_fullMatch<<endl)
                        for(j=oldw; j < w-1; j++){
                            str=(wchar_t)BufUpT[dictionary_data[j]];
                            DR("_"<<Unicode_to_UTF(str))         
                            DR(" "<<(short)BufUpT1[dictionary_data[j]]<<"#")
                        } // for j
                        DR(endl)
                    } // if(print)
#endif                                       
                } // else

            } // x
        
    } // n
    
    // пояснение //
    // d=BufUpT1[dictionary_data[x]]  d>0 совпавшая, d=0 НЕ совпавшая буква текста с буквой словаря
    /// if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
    // Более быстрый эквивалент предидущего кода. Совпавшая буква текста найденная по букве фразы словаря d==255(11111111)
    // или НЕ совпавшая буква текста с буквой словаря 0(00000000).
    //// LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
    
 /**/
    
#ifdef REMARK
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DR(endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1; x++){ 
        str=(wchar_t)BufUpT[text_data[x]]; DR(Unicode_to_UTF(str))         //        DR(text_data[x]<<"  ";
        //if ( x==BufET[s] ){ DR(endl) s++; }  // перевод каретки Enter текста
    }    
    /**/
         
    DR("количество совпавших фраз словаря с фразами текста NPhraseHits="<<NPhraseHits<<endl)
    DR("количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl)
    DR("количество циклов по словарю   n="<<n<<endl)
    DR("text_size1="<<text_size1<<endl)
    DR(endl)    
#endif
    
TIME_PRINT

     
  /**/   
}//--------------------------------------------------------------------------------------------------------------------------------------------


/*
 #ifdef REMARK
 /// код для вывода в log.txt или на экран  ///
 if ( n < ng ) { // 30000           ////////////////////////////////
 // вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)
 for(x=0; x < size_BufTxt; x++) {
 if ( x == xMaxStart ) { DR("<"; }       if ( x == xMaxEnd ) { DR(">"; }
 ///                         if ( x == xMaxStart ) { cout_<<"|"; }       if ( x == xMaxEnd ) { cout_<<"|"; }
 ds=BufUpT[BufTxt[x]];
 //if ( ds > 0 ) { str=(wchar_t)ds; DR(x<<Unicode_to_UTF(str) } else { DR("-"; } // DR("..";
 if ( ds > 0 ) { str=(wchar_t)ds; DR(Unicode_to_UTF(str) } else { DR("-"; }  // DR("..";
 //if ( x == xMaxStart ) { DR("|"; }       if ( x == xMaxEnd ) { DR("|"; }
 }
 DR(endl)
 
 // вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста
 for(x=oldw;  x < w-1;  x++) {
 s=x-oldw+1;
 ds=BufUpT[dictionary_data[x]];
 if ( x == xMaxStartDict+oldw ) { DR("<"; }       if ( x == xMaxEndDict+oldw ) { DR(">"; }
 ///                         if ( x == xMaxStartDict+oldw ) { cout_<<"|"; }       if ( x == xMaxEndDict+oldw ) { cout_<<"|"; }
 //if ( ds > 0 ) { str=(wchar_t)ds; DR(s<<Unicode_to_UTF(str) } else { DR("-"; }
 if ( ds > 0 ) { str=(wchar_t)ds; DR(Unicode_to_UTF(str) } else { DR("-"; }
 }
 DR(endl)
 
 // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
 for(x=oldw;  x < w-1;  x++) {
 //////s=x-oldw+1;
 ds=BufUp[dictionary_data[x]];
 if ( x == xMaxStartDict+oldw ) { DR("<"; }       if ( x == xMaxEndDict+oldw ) { DR(">"; }
 str=(wchar_t)ds; DR(Unicode_to_UTF(str)
 ///                         str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
 }
 
 DR(endl)
 // вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
 for(int x=oldw + xMaxStartDict;  x < oldw + xMaxEndDict;  x++) {  // x < w; // x < w-1;
 ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; DR(Unicode_to_UTF(str)
 } // x
 
 
 
 } // ( n < ng )       ////////////////////////////////
 #endif
 */
