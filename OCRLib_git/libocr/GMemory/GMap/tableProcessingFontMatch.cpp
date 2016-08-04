#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64
#include <math.h>   // log

//#define REMARK

using namespace std;


void GMap::tableProcessingFontMatch(map<vector<short>,int>&searchResult,int mode){

    // Алгоритм предназначен для поиска совпадающих фрагментов фраз словаря с фрагментами текста.
    // В тексте частично нарушена линейная последовательность пар букв и он фактически представляет собой "ведра пар букв",
    // с учетом пространственной геометрии.

    
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
    
    
    /// применение словаря к распознаваемому тексту ///  
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessingFontMatch*"<<endl)

    
    unsigned int nEnter_=nEnter;   // количество переводов каретки Enter в словаре
    unsigned int LPhrase;          // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits;    // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста
    unsigned short LPhraseHits_3=0;
    //unsigned int NPhraseHits=0;    // количество совпавших фраз словаря с фразами распознаваемого текста
    //int xStart_Dict;               // координаты начала совпавшей с текстом части фразы словаря. // xMaxStartDict;
    //int xEnd_Dict;                 // координаты конца совпавшей с текстом части фразы словаря.  // xMaxEndDict;

    unsigned int x,n,w,w_1,oldw; // wm1,m,nc,s0,s1 // ,y,p //
    //unsigned short ds;
    unsigned short constantPhrase1=constantPhrase-1;
    //uint countIndex=0;  // countIndex/2 количество совпавших фрагментов фраз словаря с фрагментами распознаваемого текста
    wstring str;
    
    //unsigned int ng=7000000000;         // сколько графики выводим на экран  // 100000 // 100
    xMaxStartDict=1;
    
    int maxAnswer=0;
    int maxAnswerIndex=0;
    
TIME_START
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    //int start=28163 ;   //старт с нужной фразы словаря (на единицу меньше чем в словаре)
    int start=0;   //старт с нужной фразы словаря
    
    w=BufE[start];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз словаря
    for( n=start; n < nEnter_; n++ ) {  //DR(n<<" ";

        oldw=w;   w=BufE[n+1];
        //if(oldw>799343)cout<<oldw<<" ";
        ///if(oldw<28163)continue;   ///test
        
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
            
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
            
            
            //DR(LPhraseHits)
            if ( LPhraseHits > constantPhrase1  ) {
                
                // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с
                // непрерывным фрагментом текста уже с учетом точных координатах букв.
                LPhraseHits_3=lookupProcessFontMatch3(searchResult,
                                                      w-1,
                                                      x-LPhraseHits+1,
                                                      oldw,
                                                      n);
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
        
        // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и
        // совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        ////if ( LPhraseHits_3 > constantPhrase1 ) {
        
        //if(n>100000)exit(0);

        
        
        
#ifdef REMARK
                // сколько "ng" и какой "xStart_Dict" графики выводим на экран.
                if (LPhraseHits_3 > constantPhrase1) { // ng=30000  && n < ng&&xMaxStartDict &&
                    DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl)
                    DR("LPhraseHits_3="<<LPhraseHits_3<<" constantPhrase1="<<constantPhrase1<<endl)
                    // вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста
                    DR("вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl)
                    DR("xMaxStartDict="<<xMaxStartDict<<"  xMaxEndDict="<<xMaxEndDict<<endl<<endl)
                    for(x=oldw;  x < w-1;  x++) {
                        ds=BufUpT[dictionary_data[x]];
                        if ( x == xMaxStartDict ) { DR("<";) }       if ( x == xMaxEndDict ) { DR(">") }
                        if ( ds > 0 ) { str=(wchar_t)ds; DR(Unicode_to_UTF(str)); } else { DR("-") }
                    }
                    DR(endl)
            
                    /*
                    // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                    DR("вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                    for(x=oldw;  x < w-1;  x++) {
                        //////s=x-oldw+1;
                        ds=BufUp[dictionary_data[x]];
                        if ( x == xMaxStartDict+oldw ) { DR("<";) }       if ( x == xMaxEndDict+oldw ) { DR(">") }
                        str=(wchar_t)ds; DR(Unicode_to_UTF(str))
        ///             str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
                    }
                    DR(endl)
                    */
                    
                    // вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                    DR("вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                    for(int x=xMaxStartDict;  x < xMaxEndDict;  x++) {  // x < w; // x < w-1;
                        ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; DR(Unicode_to_UTF(str))
                    } // x    
                    DR(endl<<endl<<endl)
                                
            
                    // вывод на экран координат начала и конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
                    DR("xMaxStartDict="<<xMaxStartDict<<"  xMaxEndDict="<<xMaxEndDict<<endl<<endl)
                                
                } // ( n < ng )       
#endif
/**/
            ///NPhraseHits++; ///
            
        ///} // if ( LPhraseHits_3 > constantPhrase1 ) // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        
        //exit(0);
        // обнуление массива восстановленного текста BufTxt
        //memset(BufTxt,0,(size_BufTxt+32)*2);
        
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

    
#ifdef REMARK
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    DR(endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl)
    s=0;
    for(x=0; x < text_size1/100; x++){
        str=(wchar_t)BufUpT[text_data[x]]; DR(Unicode_to_UTF(str))         //        DR(text_data[x]<<"  ";
        if ( x==BufET[s] ){ DR(endl) s++; }  // перевод каретки Enter текста
    }
    DR(endl)
    /**/
    
    DR("количество циклов по словарю   n="<<n<<endl)
    DR("количество переводов каретки Enter в словаре nEnter_="<<nEnter_<<endl)
    DR("минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную   constantPhrase="<<constantPhrase<<endl)
    //DR("количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   NPhraseHits="<<NPhraseHits<<endl)
    DR("количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   countIndex/2="<<countIndex/2<<endl)
 
    
    DR("maxAnswer="<<maxAnswer<<" maxAnswerIndex="<<maxAnswerIndex<<endl)
    oldw=BufE[maxAnswerIndex];   w=BufE[maxAnswerIndex+1];
    for(int x=oldw;  x < w;  x++) {  // w_1=w-1;
        str=(wchar_t)BufUp[dictionary_data[x]]; DR(Unicode_to_UTF(str))
    }
    DR(endl)
    if(print){TIME_PRINT_}
#endif
   
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
///DR("size_SumTxt=nLetter"<<size_SumTxt<<endl)
*/