#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64
#include <math.h>   // log

//#define REMARK

using namespace std;


void GMap::tableProcessingFontMatch(map<vector<int>,ulong>&searchResult,int mode){

    // Алгоритм предназначен для поиска совпадающих фрагментов фраз словаря с фрагментами текста.
    // В тексте не соблюдается линейная последовательность пар букв и он фактически представляет собой "ведро пар букв",
    // геометрическое положение пар букв записано в массиве letterX и проверяется в lookupProcessFontMatch3 в процессе поиска.

    
    // Описание алгоритма.
    // Массив словаря dictionary_data[x] состоит изнескольких миллионов фраз. Каждая фраза состоит из нескольких слов,
    // каждая буква компактно кодируется short. Массив словаря dictionary_data[x] линейно читается фраза за фразой буква за буквой.
    // По коду буквы с помощью таблицы "BufUpT" ( BufUpT[dictionary_data[x]]; ) определяется наличие этой буквы в тексте.
    // При совпадении 8 (constantPhrase=8) и более подряд идуших букв фразы словаря с буквами текста, происходит вызов
    // функции lookupProcess3 учитывающею шрифтовую (поространственную) геометрию текста.
    // Таблица "BufUpT" однократно предпросчитывается по тексту в функции tableText. Поиск может быть не четким,
    // если считать совпавший фрагмент кусочно непрерывным.
    // Для простоты здесь вместо "пар букв" используется просто "буква".

    
    int print=0;  // вывод отладочной информации 1 - выводим, 0 - нет
#ifdef REMARK
    print=1;
    mSIZE ds;
    printInputEntry();
#endif
    /// применение словаря к распознаваемому тексту ///  
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessingFontMatch*"<<endl)

    
    ulong nEnter_=nEnter;   // количество переводов каретки Enter в словаре
    uint LPhrase;          // текущая длина фразы словаря  Length Phrase
    uint LPhraseHits;    // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста
    uint LPhraseHits_3=0;
    //unsigned int NPhraseHits=0;    // количество совпавших фраз словаря с фразами распознаваемого текста
    //int xStart_Dict;               // координаты начала совпавшей с текстом части фразы словаря. // xMaxStartDict;
    //int xEnd_Dict;                 // координаты конца совпавшей с текстом части фразы словаря.  // xMaxEndDict;

    ulong x,n,w,w_1,oldw; // wm1,m,nc,s0,s1 // ,y,p //
    
    uint constantPhrase1=constantPhrase-1;
    //uint countIndex=0;  // countIndex/2 количество совпавших фрагментов фраз словаря с фрагментами распознаваемого текста
    wstring str;
    
    //unsigned int ng=7000000000;         // сколько графики выводим на экран  // 100000 // 100
    xMaxStartDict=1;
    
    ulong maxAnswer=0;
    ulong maxAnswerIndex=0;

#ifdef REMARK
    TIME_START
    printInputEntry();
#endif

    
    
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    //int start=50480;   //старт с нужной фразы словаря (на единицу меньше чем в словаре)
    int start=0;   //старт с нужной фразы словаря
    
    w=BufE[start];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз словаря
    for( n=start; n < nEnter_; n++ ) {  //DR(n<<" ";

        oldw=w;   w=BufE[n+1];
        //if(oldw>799343)cout<<oldw<<" ";
        ///if(oldw<28163)continue;   ///test
        //printInputEntry();
        //printInputEntry();
        //printDictEntry(n);
        //printDictTextLine(n);
        
        LPhrase=(uint)(w-oldw);       // текущая длина фразы Length Phrase
            
        LPhraseHits=0;    w_1=w-1;
        if(w_1-oldw>128)w_1=oldw+128;   // ограничение длины фразы
        // Цикл перебора пар букв фразы
        for(x=oldw;  x < w_1;  x++) {  // w_1=w-1;
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший
            // с фрагментами распознаваемого текста.
            // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // счетчик сбросывается, если не совпадет хоть одна буква (описание дано в конце цикла).
            LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]]; // Совпавшая буква текста найденная
            // по букве фразы словаря & 255(11111111) или НЕ совпавшая буква текста с буквой словаря & 0(00000000).
            
            //cout<<p2letter(dictionary_data[x])<<" ";
            
            //DR(LPhraseHits)
            if ( LPhraseHits > constantPhrase1  ) {
                // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с
                // непрерывным фрагментом текста уже с учетом точных координатах букв.
                //printDictEntry(n);
                LPhraseHits_3=lookupProcessFontMatch3(searchResult,
                                                      w-1,
                                                      x-LPhraseHits+1,
                                                      oldw,
                                                      n);
#ifdef REMARK
                printDictEntry(n);
                cout<<"L:"<<LPhraseHits_3<<endl;
#endif
                if(LPhraseHits_3>maxAnswer){
                    maxAnswer=LPhraseHits_3;
                    maxAnswerIndex=n;
                }
                // старт функции определения поространственной геометрию текста.
                ///       lookupProcessFont(n, oldw,w)
                // возвращаем их координаты xMaxStart, xMaxEnd
                //if ( LPhraseHits_3 > constantPhrase1  )
                // LPhraseHits_3 возвращает количество совпавших букв фразы LPhraseHits с буквами распознаваемого текста (Length Phrase Hits)
                break; // остановка подсчета LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
            }
            
        } // x
        //break;
        //exit(0);
        
        
    } // n
    // DR(endl<<endl)
}//--------------------------------------------------------------------------------------------------------------------------------------------
