#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64

#include <math.h>   // log

//#define REMARK

#ifdef REMARK
#define DN(x) cout<<x;
#else
#define DN(x)
#endif


using namespace std;


void GMap::tableProcessingID(GStr<uint>*result){
    
    
    //Кодирование фразы текста ID номерами слов
    // замена в тексте всех найденых слов их ID номерами
    
    DN(endl<<endl<<"*****************************************************"<<
       "*********************************************************"<<endl);
    DN("Start program *tableProcessingID*"<<endl)
    
    
    uint LPhrase;            // текущая длина фразы словаря  Length Phrase
    uint LPhraseHits;        // количество букв фразы совпавших с буквами распознаваемого текста
    uint LPhraseHits_3=0;
    uint NPhraseHits=0;      // количество совпавших фраз словаря с фразами распознаваемого текста
    uint wLimit=5;           // минимальная длина слова используемая при кодировании.
                             //Неизвестные слова в строке заносятся в список новых слов.
    
    
    // буферный массив текста ( восстановленная через словарь копия массива распознаваемого текста )
    size_BufTxt=(uint)text_size1; // размер массива распознаваемого текста (в байтах), в общем случае size_BufTxt=text_size;
    // text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки
    // добавлено 4 III 2012
    
    ////uint *BufTxt;
    BufTxt_vector->reserve(size_BufTxt);
    BufTxt=BufTxt_vector->dataPtr();   // запрос памяти c обнулением // size_BufPhr+32
    
    // буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря ( GravityTxt, GravTxt, SumTxt, TotalTxt ).
    size_GravTxt=(uint)text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
    // text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки // добавлено 4 III 2012
    ////uint *GravTxt;
    GravTxt_vector->reserve(size_GravTxt);
    GravTxt=GravTxt_vector->dataPtr();   // запрос памяти c обнулением
    
    /// массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв)
    // где позиция буквы в тексте это номер строки массива частоты встречаемости пар букв
    // адресом в строке является код пар букв (мб код буквы) содержимое - сколько раз данная пара букв была найдена
    // во всех легитимных фрагментах фраз словаря.
    
    //size_SumTxtP=(uint)(text_size1*nLetterP); // nLetterP // полный размер массива частоты встречаемости пар букв,
    // где nLetterP равен количеству разных пар букв в словаре (примерно 6000), для букв nLetter (nLetter=214 uint)
    // text_size1 размер распознаваемого текста (в байтах) в буквах без учета кодов пробела или точки (в общем случае size_BufTxt=text_size;)
    //SumTxtP_vector->reserve(size_SumTxtP);
    //SumTxtP=SumTxtP_vector->dataPtr();   // запрос памяти c обнулением
    //SumTxt=(unsigned char*)calloc(size_SumTxt,1);    // запрос памяти c обнулением
    ///cout<<"size_SumTxt=nLetter"<<size_SumTxt<<endl;
    
    
    
    DN("список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl)
#ifdef REMARK
    printInputEntry();
#endif
    
    
    ulong x,n,w,w_1,oldw; // ,rg, wm1,m,nc,s0,s1 // ,y,p //
    wstring str;
    
    NPhraseHits=0;    //SPhraseHits=0;
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз
    for( n=0; n < nEnter; n++ ) {  //cout<<n<<" ";
        
        oldw=w;   w=BufE[n+1];
        LPhrase=(uint)(w-oldw);       // текущая длина фразы Length Phrase
        if(LPhrase<wLimit)break;
        if(LPhrase>text_size)continue;
        // Цикл перебора пар букв фразы
        LPhraseHits=0;   w_1=w-1;
        LPhraseHits_3=0;   ////////////////////
        constantPhrase=LPhrase-2;
        
        for(x=oldw;  x < w_1;  x++) {  // w_1=w-1;
            /// уточняющий алгоритм N2. ///
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами
            // распознаваемого текста. Здессь не учитывается информация о точных координатах совпавших букв текста.
            // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // Быстрый вариант для четкого поиска (со сбросом, если не совпадет хоть одна буква)
            if(!BufUpT1[dictionary_data[x]])break;
                        
            LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]]; // Совпавшая буква текста найденная
            /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
            
            // По достижении пороговой длины LPhrase-1 подряд идущих совпавших букв текста,
            // останавливаем подсчет длины и переходим в lookup_Process3.
            
            //if ( LPhraseHits > constantPhrase1  ) { goto Leave1; }  // break;
            //if ( LPhraseHits == LPhrase-2) {
            if ( LPhraseHits >LPhrase-3) {
#ifdef REMARK
                //cout<<"LPhraseHits="<<LPhraseHits<<" n="<<n<<" LPhrase="<<LPhrase<<endl;
                PR(printDictEntry((uint)n););
                //PR(printInputEntry(););
                PR(printDictTextLine((uint)n););
#endif
                
                
                /// уточняющий алгоритм N3. ///
                // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с
                // непрерывным фрагментом текста уже с учетом точных координатах букв.
                LPhraseHits_3=lookupProcess3(w, oldw);
                //         LPhraseHits_3=8;
                if ( LPhraseHits_3 == LPhrase-2) {
                    string key;
                    valueData_vector->getStr(n,key);
                    result->push_back((uint)n);
                } ///////////// служебный подсчет j
                // LPhraseHits_3 возвращает количество совпавших букв фразы LPhraseHits с буквами распознаваемого текста (Length Phrase Hits)
                break; // остановка подсчета LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
            }
        } // x
        //   Leave1:;
        // обнуление массива восстановленного текста BufTxt
        memset(BufTxt,0,(size_BufTxt)*sizeof(mSIZE));

        
    } // n

}//------------------------------------------------------------------------------------------------------------
