#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64

#include <math.h>   // log

//#define REMARK

using namespace std;

void GMap::tableProcessingOCRMatch(vector<ulong>&searchResult,int mode){
       
    // Поиск (четкий или нечеткий) слова (стека) из текста, в словаре.
    // Проверяется совпадение (четкое или нечеткое) слова (стека) из текста, с фразой словаря.
    
    // Проверяемай текст должен состоять из пар букв и из тех же пар букв где вторая буква
    // заменена на перевод каретки (Enter). В словаре в конце каждой фразы добавлен перевод каретки.
    // В проверяемом тексте каждая пара м.б. последней.
    
    
    int print=0; // вывод отладочной информации 1 - выводим, 0 - нет
#ifdef REMAPK
    print=1;
#endif
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessing_OCRMatch*"<<endl)
        
    
    ulong LPhrase;       // текущая длина фразы словаря  Length Phrase
    uint LPhraseHits; // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста
    ulong NPhraseHits=0; // количество совпавших фраз словаря с фразами распознаваемого текста
  
    ulong x,n,w,w_1,oldw;
    mSIZE ds;
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
        //if(n!=523)continue;
        //printDictTextLine(n); cout<<"n:"<<n<<endl;
                        
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
#ifdef REMARK
                    DR("n:"<<n<<" L="<<LPhraseHits<<" /"<<LPhrase<<endl)
                    printDictTextLine(n);
#endif
                } // else

            } // x
            //printDictEntry(n);
        
    } // n
    
    // пояснение //
    // d=BufUpT1[dictionary_data[x]]  d>0 совпавшая, d=0 НЕ совпавшая буква текста с буквой словаря
    /// if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
    // Более быстрый эквивалент предыдущего кода. Совпавшая буква текста найденная по букве фразы словаря d==255(11111111)
    // или НЕ совпавшая буква текста с буквой словаря 0(00000000).
    //// LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
    
 /**/
    
#ifdef REMARK
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DR(endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl)
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

