#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64
#include <math.h>   // log

//#define REMARK

using namespace std;


void GMap::tableProcessingStack(GStr<uint> *searchResult_, int mode){

    

    // Алгоритм предназначен для поиска совпадающих фрагментов фраз словаря с фрагментами текста.
    // В тексте частично нарушена линейная последовательность пар букв и он фактически представляет собой "ведра пар букв".
    
    // Описание алгоритма.
    // Массив словаря dictionary_data[x] состоит изнескольких миллионов фраз. Каждая фраза состоит из нескольких слов,
    // каждая буква компактно кодируется short. Массив словаря dictionary_data[x] линейно читается фраза за фразой буква за буквой.
    // По коду буквы с помощью таблицы "BufUpT" ( BufUpT[dictionary_data[x]]; ) определяется наличие этой буквы в тексте.
    // При совпадении 8 (constantPhrase=8) и более подряд идуших букв фразы словаря с буквами текста, подсчитывается начало и конец
    // совпавшего фрагмента словаря. Номер этой фразы словаря и координаты начала и конеца совпавшего фразы фрагмента словаря, 
    // для дальнейшей обработки, заносятся в массив searchResult[]. Таблица "BufUpT" однократно предпросчитывается по тексту
    // в функции tableText. Поиск может быть не четким, если считать совпавший фрагмент кусочно непрерывным.
    // Для простоты здесь вместо "пар букв" используется просто "буква".

    
    int print=1;  // вывод отладочной информации 1 - выводим, 0 - нет
    
    
    /// применение словаря к распознаваемому тексту ///  
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessing_Stack*"<<endl)

    
    unsigned int nEnter_=nEnter;   // количество переводов каретки Enter в словаре
    unsigned int LPhrase;          // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits;    // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста  // int
    int xStart_Dict;               // координаты начала совпавшей с текстом части фразы словаря. // xMaxStartDict;
    int xEnd_Dict;                 // координаты конца совпавшей с текстом части фразы словаря.  // xMaxEndDict;

    unsigned int x,n,s,w,w_1,oldw; // wm1,m,nc,s0,s1 // ,y,p //
    unsigned short ds;
    unsigned short constantPhrase1=constantPhrase-1;
    uint countIndex=0;  // countIndex/2 количество совпавших фрагментов фраз словаря с фрагментами распознаваемого текста    
    wstring str;
    uint *searchResult=(uint*)searchResult_->dataPtr();
    
    unsigned int ng=7000000;         // сколько графики выводим на экран  // 100000 // 100
    
TIME_START
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз словаря
    for( n=0; n < nEnter_; n++ ) {  //cout<<n<<" ";

        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
            
        LPhraseHits=0;    w_1=w-1;
        xStart_Dict=0;
        xEnd_Dict=w_1-1; // xEnd_Dict может оказаться < 0
        // Цикл перебора пар букв фразы
        for(x=oldw;  x < w_1;  x++) {  // w_1=w-1;
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший
            // с фрагментами распознаваемого текста.
            // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // счетчик сбросывается, если не совпадет хоть одна буква (описание дано в конце цикла).
            LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]]; // Совпавшая буква текста найденная
            // по букве фразы словаря & 255(11111111) или НЕ совпавшая буква текста с буквой словаря & 0(00000000).
                
            // Проверки определяющие начало и конец подряд идущих совпавших букв фразы словаря
            if ( LPhraseHits == constantPhrase1 ) {xStart_Dict=x-constantPhrase1+1;}
            if ( xStart_Dict && LPhraseHits==0 ) {
                xEnd_Dict=x;
                searchResult[countIndex]=xStart_Dict; countIndex++;
                searchResult[countIndex]=xEnd_Dict;   countIndex++;
                break; // если не ставить break, то сохранятся все фрагменты фразы словаря совпавшего с буквами текста
            }
        } // x
                      
                    
#ifdef REMARK                                 
                // сколько "ng" и какой "xStart_Dict" графики выводим на экран.
        if ( xStart_Dict && n < ng) { // ng=30000
            DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl)

            // вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста
            DR("вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl)
            DR("xStart_Dict="<<xStart_Dict<<"  xEnd_Dict="<<xEnd_Dict<<endl<<endl)
            for(x=oldw;  x < w-1;  x++) {
                ds=BufUpT[dictionary_data[x]];
                if ( x == xStart_Dict ) { cout<<"<"; }       if ( x == xEnd_Dict ) { cout<<">"; }
                if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }
            }
            DR(endl)
                        
            // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
            DR("вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
            for(x=oldw;  x < w-1;  x++) {
                //////s=x-oldw+1;
                ds=BufUp[dictionary_data[x]];
                if ( x == xStart_Dict+oldw ) { cout<<"<"; }       if ( x == xEnd_Dict+oldw ) { cout<<">"; }
                str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
///             str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
            }
            DR(endl)
            
            // вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
            DR("вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
            for(int x=xStart_Dict;  x < xEnd_Dict;  x++) {  // x < w; // x < w-1;
                ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
            } // x    
            DR(endl<<endl<<endl)
                        
    
            // вывод на экран координат начала и конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
            cout<<"xStart_Dict="<<xStart_Dict<<"  xEnd_Dict="<<xEnd_Dict<<endl<<endl;
                        
        } // ( n < ng )       
#endif 
/**/        
        
    } // n    
    // DR(endl<<endl)
    
    /// пояснение ///
    //  Понятный вариант.
    //  ds=BufUpT1[dictionary_data[x]]  ds>0 совпавшая, ds=0 НЕ совпавшая буква текста с буквой словаря
    /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }    
    //  Более быстрый вариант для четкого поиска (со сбросом, если не совпадет хоть одна буква)
    //  Совпавшая буква текста найденная по букве фразы словаря & 255(11111111)
    //  или НЕ совпавшая буква текста с буквой словаря & 0(00000000).
    /// LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];

    
    searchResult_->resize(countIndex);

#ifdef REMARK
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DR(endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1; x++){ 
        str=(wchar_t)BufUpT[text_data[x]]; cout<<Unicode_to_UTF(str);         //        cout<<text_data[x]<<"  ";
        if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter текста
    }
    DR(endl)
    /**/
    
    DR("количество циклов по словарю   n="<<n<<endl)
    DR("количество переводов каретки Enter в словаре nEnter_="<<nEnter_<<endl)
    DR("минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную   constantPhrase="<<constantPhrase<<endl)
    DR("количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   countIndex/2="<<countIndex/2<<endl)
#endif 
    
 TIME_PRINT
 DR("найдено фраз "<<searchResult_->size()/2<<endl)

    
   
}//--------------------------------------------------------------------------------------------------------------------------------------------

// Поиск (четкий или нечеткий) последовательности из текста (ведра пар букв) длинной более 8 символов,
// с частью фразы словаря. Словарь может быть фразовый (состоять из нескольких слов) или чисто словный.
// На этом этапе информация о точных координатах совпавших букв текста не используется но присутствует.


/*
// буферный массив текста ( восстановленная через словарь копия массива распознаваемого текста )
size_BufTxt=text_size1; // размер массива распознаваемого текста (в байтах), в общем случае size_BufTxt=text_size;
// text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки
// добавлено 4 III 2012

////unsigned short *BufTxt;
BufTxt=(unsigned short*)calloc(size_BufTxt+32,2);   // запрос памяти c обнулением // size_BufPhr+32


//  nev  //
// буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря ( GravityTxt, GravTxt, SumTxt, TotalTxt ).
size_GravTxt=text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
// text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки // добавлено 4 III 2012
////unsigned short *GravTxt;
GravTxt=(unsigned short*)calloc(size_GravTxt+32,2);   // запрос памяти c обнулением

/// массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв)
// где позиция буквы в тексте это номер строки массива частоты встречаемости пар букв
// адресом в строке является код пар букв (мб код буквы) содержимое - сколько раз данная пара букв была найдена
// во всех легитимных фрагментах фраз словаря.

size_SumTxtP=text_size1*nLetterP; // nLetterP // полный размер массива частоты встречаемости пар букв,
// где nLetterP равен количеству разных пар букв в словаре (примерно 6000), для букв nLetter (nLetter=214 unsigned short)
// text_size1 размер распознаваемого текста (в байтах) в буквах без учета кодов пробела или точки (в общем случае size_BufTxt=text_size;)
SumTxtP=(unsigned short*)calloc(size_SumTxtP+32,2);   // запрос памяти c обнулением
//SumTxt=(unsigned char*)calloc(size_SumTxt+32,1);    // запрос памяти c обнулением
///cout<<"size_SumTxt=nLetter"<<size_SumTxt<<endl;
*/