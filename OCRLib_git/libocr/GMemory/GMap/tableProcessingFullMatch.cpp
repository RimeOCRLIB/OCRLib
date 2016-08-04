#include "GMap.h"
//#include <smmintrin.h>  //_mm_popcnt_u64

#include <math.h>   // log

//#define REMARK

using namespace std;

int GMap::tableProcessingFullMatch(int mode){
       
    // Поиск (четкий или нечеткий) слова в словаре.
    // Проверяется полное совпадение (четкое или нечеткое) слова с фразой словаря.
    
    
    int print=0; // вывод отладочной информации 1 - выводим, 0 - нет
#ifdef REMARK
    print=1;
    TIME_START
#endif
    
    DR(endl<<endl<<"*****************************************************"<<
    "*********************************************************"<<endl);
    DR("Start program *tableProcessing_fullMatch*"<<endl)
        
    unsigned int LPhrase;       // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits; // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста    
    unsigned int NPhraseHits=0; // количество совпавших фраз словаря с фразами распознаваемого текста
  
    unsigned int x,n,w,w_1,oldw;
    unsigned short ds;         
    wstring str;
    int n_fullMatch=-1;
    
    //exit(0);
    
        
    /// основной прогон поиска ///    
    // Поиск (четкий или нечеткий) слова в словаре.
    // Проверяется полное совпадение (четкое или нечеткое) слова с фразой словаря.
    
    NPhraseHits=0;    
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    
    // Цикл перебора фраз словаря.
    for( n=0; n < nEnter; n++ ) {  //DR(n<<" ";  
       
        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
        // для уменьшения объема словаря, проверяем совпадение длинны фразы словаря с длинной слова текста.
        if(LPhrase!=text_size1)continue;
                        
            // Цикл перебора пар букв фразы
            LPhraseHits=0;   w_1=w-1; 
            for(x=oldw;  x < w_1;  x++) { 
                ds=BufUpT1[dictionary_data[x]];
                
                //str=BufUp[dictionary_data[x]]; DT(Unicode_to_UTF(str)<<" ");         //        DR(text_data[x]<<"  ")

                // счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
                LPhraseHits=(LPhraseHits + 1) & ds;
                // понятный вариант для нечеткого поиска
                /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }

                // По достижении пороговой длины text_size1-1 подряд идущих совпавших букв текста, останавливаем подсчет длины и переходим к следующей фразе.
                if(LPhraseHits!=text_size1-1)continue;
                else {
                    
                    //цикл окончательной проверки совпадения слова текста и фразы в словаря
                    for(int i=0;i<LPhrase-1;i++){
                        //DR("dictionary_data[i]="<<dictionary_data[i+oldw]<<" text_data[i]="<<text_data[i]<<endl)
                        if(dictionary_data[i+oldw]!=text_data[i])goto no_match;
                    }
                    
                    n_fullMatch=n; // номер фразы в словаре совпавшей с словом текста
                    
                    NPhraseHits++; 
                    // отладка и визуализация 
#ifdef REMARK
                    if(print){
                        DR(LPhraseHits<<" LPhraseHits++++++++++++++++++++++++++++++++++++++++++++++++")
                        DR("text_size1="<<text_size1<<endl)
                        //DR("n_fullMatch="<<n_fullMatch<<endl)
                        for(int j=oldw; j < w-1; j++){
                            str=(wchar_t)BufUpT[dictionary_data[j]];
                            DR("_"<<Unicode_to_UTF(str))         
                            DR(" "<<(short)BufUpT1[dictionary_data[j]]<<"#")
                        } // for j
                        DR(endl)
                    } // if(print)
#endif
                    return n_fullMatch;
                } // else

            } // x
            no_match:;
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
    int s=0;
    for(x=0; x < text_size1; x++){
        cout<<" text_data["<<x<<"]="<<text_data[x]<<endl;
        str=(wchar_t)BufUp[text_data[x]]; DR(Unicode_to_UTF(str));         //        DR(text_data[x]<<"  ")
        if ( x==BufET[s] ){ DR(endl) s++; }  // перевод каретки Enter текста
    }    
    /**/
         
    DR("количество совпавших фраз словаря с фразами текста NPhraseHits="<<NPhraseHits<<endl)
    DR("количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl)
    DR("количество циклов по словарю   n="<<n<<endl)
    DR("text_size1="<<text_size1<<endl)
    DR(endl)       
//TIME_PRINT
#endif
    
    return -1;
     
  /**/   
}//--------------------------------------------------------------------------------------------------------------------------------------------
// Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
int GMap::tableProcessingMatch(int mode){
    
    // Поиск (четкий или нечеткий) последовательности из текста (ведра пар букв) длинной более 8 символов,
    // с частью фразы словаря. Словарь может быть фразовый (состоять из нескольких слов) или чисто словный.
    
    
    /// Уточняющий алгоритм N2. ///
    // Алгоритм предназначен для существенного уменьшения объема словаря.
    // Для этого оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент совпавший с буквами распознаваемого текста.
    // По достижении пороговой длины constantPhrase подряд идущих совпавших букв текста, останавливаем подсчет длины.
    // и переходим к следующему уточняющему алгоритму N3,
    // На этом этапе информация о точных координатах совпавших букв текста не присутствует.
    
    
    int print=0;  // вывод отладочной информации 1 - выводим, 0 - нет
    int s,ds;
    
    /// применение словаря к распознаваемому тексту ///
    
    DR(endl<<endl<<"*****************************************************"<<
       "*********************************************************"<<endl);
    DR("Start program *tableProcessingAIML*"<<endl)
    
    
    // unsigned int nEnter;              // количество переводов каретки Enter в словаре
    unsigned int LPhrase;                // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits;          // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста  // int
    unsigned short constantPhrase_1;
    unsigned short LPhraseHits_3=0;
    unsigned int NPhraseHits=0;          // количество совпавших фраз словаря с фразами распознаваемого текста
    
    
    
#ifdef REMARK
    unsigned int ng=100000;   // сколько графики выводим на экран  // 100000 // 100
    print=1;
    TIME_START
#endif
    ///  параметры для нечеткого поиска  ///
    // Задаются как глобальные (перенесено в associativeSignalProcessing.cpp и дальше)
    /*
     // максимальное количество не четких пар букв в легитимной фразе словаря (dlt=0; четкий поиск).
     delta=0; // 2  // 4
     // минимальная длина части фразы словаря (в парах букв), для нечеткого поиска, которую имеет смысл проверять как непрерывную.
     constantPhrase=8;  //// фиксированного размера = 8
     */
    
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
    /**/
    
    /// основной прогон нечеткого поиска ///
    
    //unsigned int globalMaxPart=0;
    //unsigned int NLPhraseHits=0;
    //unsigned int MaxPartPhraseHit=0;
    //unsigned int NPartPhraseHit=0;
    //    unsigned short nCldF;              // количество непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
    //    unsigned short nLPhrH;             // счетчик длины (ширина) совпавшего непрерывного фрагмента фразы.
    
    
    DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске nEnter="<<nEnter<<endl)
    
    
    unsigned int x,n,w,w_1,oldw; // ,rg, wm1,m,nc,s0,s1 // ,y,p //
    //int blp,bl,bc,cnt;
    //unsigned short ds;         // ,bf
    wstring str;
    
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    
    NPhraseHits=0;    //SPhraseHits=0;
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз
    for( n=0; n < nEnter; n++ ) {
        
        //cout<<n<<" ";
        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
        
        // Цикл перебора пар букв фразы
        LPhraseHits=0;   w_1=w-1;
        LPhraseHits_3=0;   ////////////////////
        // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
        constantPhrase=LPhrase*0.3; // 4  // 8   //// фиксированного размера = 8
        constantPhrase_1=constantPhrase-1;   //локальная переменная

        
        //if(n<3)continue;
        for(x=oldw;  x < w_1;  x++) {  // w_1=w-1;
            
            //cout<<"dictionary_data["<<x<<"]="<<BufUpT[dictionary_data[x]]<<" BufUpT1[dictionary_data[x]="<<(short)BufUpT1[dictionary_data[x]]<<endl;
            
            /// уточняющий алгоритм N2. ///
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами
            // распознаваемого текста. Здессь не учитывается информация о точных координатах совпавших букв текста.
            // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // Быстрый вариант для четкого поиска (со сбросом, если не совпадет хоть одна буква)
            LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]]; // Совпавшая буква текста найденная
            // по букве фразы словаря & 255(11111111) или НЕ совпавшая буква текста с буквой словаря & 0(00000000).
            // Понятный вариант.
            /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
            
            // По достижении пороговой длины LPhrase-1 подряд идущих совпавших букв текста,
            // останавливаем подсчет длины и переходим в lookup_Process3.
            
            //if ( LPhraseHits > constantPhrase1  ) { goto Leave1; }  // break;
            if(LPhraseHits<text_size1-1)continue;
            else {
#ifdef REMARK
                cout<<"LPhraseHits="<<LPhraseHits<<" n="<<n<<" LPhrase="<<LPhrase<<endl;
                PR(printDictEntry(n););
                //PR(printInputEntry(););
#endif

                /// уточняющий алгоритм N3. ///
                // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с
                // непрерывным фрагментом текста уже с учетом точных координатах букв.
                LPhraseHits_3=lookupProcess3(w, oldw);
                //         LPhraseHits_3=8;
                //if ( LPhraseHits_3 > constantPhrase_1  ) { j++; } ///////////// служебный подсчет j
                // LPhraseHits_3 возвращает количество совпавших букв фразы LPhraseHits с буквами распознаваемого текста (Length Phrase Hits)
                break; // остановка подсчета LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
            }
        } // x
        //   Leave1:;
        /**/
        
        
        // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и
        // совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        if ( LPhraseHits_3 > text_size1-2) {
            
            
            /// уточняющий алгоритм N4. ///
            // старт функции поиска наибольших под вхождений фразы словаря ( т.е. оставляем самый длинный блок во фразе ).
            //lookupProcess4(n, oldw,w);  //ПРОВЕРИТЬ
            // nxx=n;
            // возвращаем их координаты xMaxStart, xMaxEnd (все с учетом нечеткости)
            
            
#ifdef REMARK
            // сколько "ng" графики выводим на экран.
            if ( n < ng ) { // 30000           ////////////////////////////////
                
                // вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)
                DR("вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)"<<endl)
                for(x=0; x < size_BufTxt; x++) {
                    if ( x == xMaxStart ) { cout<<"<"; }       if ( x == xMaxEnd ) { cout<<">"; }
                    //if ( x == xMaxStart ) { cout_<<"|"; }       if ( x == xMaxEnd ) { cout_<<"|"; }
                    if ( BufTxt[x] > 0 ) { str=(wchar_t)BufUpT[BufTxt[x]]; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }  // cout<<"..";
                    //if ( x == xMaxStart ) { cout<<"|"; }       if ( x == xMaxEnd ) { cout<<"|"; }
                }
                DR(endl)
                
                // вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста
                DR("вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl)
                DR("длинна совпавшего слова="<<xMaxEndDict-xMaxStartDict<<endl);
                for(x=oldw;  x < w-1;  x++) {
                    s=x-oldw+1;
                    ds=BufUpT[dictionary_data[x]];
                    if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
                    ///if ( x == xMaxStartDict+oldw ) { cout_<<"|"; }       if ( x == xMaxEndDict+oldw ) { cout_<<"|"; }
                    //if ( ds > 0 ) { str=(wchar_t)ds; cout<<s<<Unicode_to_UTF(str); } else { cout<<"-"; }
                    if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }
                }
                DR(endl)
                
                // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                DR("вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                for(x=oldw;  x < w-1;  x++) {
                    //////s=x-oldw+1;
                    ds=BufUp[dictionary_data[x]];
                    if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
                    str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
                    ///str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
                }
                DR(endl)
                
                // вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                DR("вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                for(int x=oldw + xMaxStartDict;  x < oldw + xMaxEndDict+1;  x++) {  // x < w; // x < w-1;
                    ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
                } // x
                DR(endl<<endl)
                
                //cout_<<endl<<"//__________________________________________________________"<<endl<<endl ;
                
                
                
                // вывод на экран координат начала и конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
                ///                 cout<<"xMaxStart="<<xMaxStart<<"  xMaxEnd="<<xMaxEnd<<endl;
                ///                 cout<<"xMaxStartDict="<<xMaxStartDict<<"  xMaxEndDict="<<xMaxEndDict<<endl<<endl;
                
            } // ( n < ng )       ////////////////////////////////
#endif
            /**/
            DR("match. n="<<n<<endl);
            if ( BufTxt !=NULL ) free(BufTxt);
            if ( SumTxtP !=NULL ) free(SumTxtP);
            if ( GravTxt !=NULL ) free(GravTxt);
            return n;
            
        } // if ( LPhraseHits_3 > constantPhrase1 ) // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        
        // обнуление массива восстановленного текста BufTxt
        memset(BufTxt,0,(size_BufTxt+32)*2);
/*
#ifdef REMARK
        cout<<"text_size1="<<text_size1<<"    "<<"clusters_size="<<clusters_size<<"   ";
        printInputEntry();
        // LPhraseHits[0]=LPhraseHits[0]-1   // длина первой найденной фразы дб на 1 меньше
        cout<<"минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную   constantPhrase="<<constantPhrase<<endl;
        cout<<"количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   NPhraseHits="<<NPhraseHits<<endl;
        //cout<<"Max длина совпавшего непрерывного фрагмента фразы MaxPartPhraseHit="<<MaxPartPhraseHit<<endl;
        //cout<<"Max длина совпавшего непрерывного фрагмента фразы за весь словарь LPhraseHits="<<LPhraseHits<<endl;
        //    cout<<"найдено фраз "<<resultReport.size()<<endl;
        cout<<"количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl;
        cout<<"количество циклов по словарю   n="<<n<<endl;
        TIME_PRINT
#endif
*/        /**/
        
    } // n
    
    //  освобождение памяти
     if ( BufTxt !=NULL ) free(BufTxt);
     if ( SumTxtP !=NULL ) free(SumTxtP);
     if ( GravTxt !=NULL ) free(GravTxt);
    return -1;

}//---------------------------------------------------------------------------------------------------------------------------------------


// Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
void GMap::tableProcessingMatch(vector<uint>&searchResult, uint limit){
    
    // Поиск (четкий или нечеткий) последовательности из текста (ведра пар букв) длинной более 8 символов,
    // с частью фразы словаря. Словарь может быть фразовый (состоять из нескольких слов) или чисто словный.
    
    
    /// Уточняющий алгоритм N2. ///
    // Алгоритм предназначен для существенного уменьшения объема словаря.
    // Для этого оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент совпавший с буквами распознаваемого текста.
    // По достижении пороговой длины constantPhrase подряд идущих совпавших букв текста, останавливаем подсчет длины.
    // и переходим к следующему уточняющему алгоритму N3,
    // На этом этапе информация о точных координатах совпавших букв текста не присутствует.
    
    
    int print=0;  // вывод отладочной информации 1 - выводим, 0 - нет
    int s,ds;
    
    /// применение словаря к распознаваемому тексту ///
    
    DR(endl<<endl<<"*****************************************************"<<
       "*********************************************************"<<endl);
    DR("Start program *tableProcessingAIML*"<<endl)
    
    
    // unsigned int nEnter;              // количество переводов каретки Enter в словаре
    unsigned int LPhrase;                // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits;          // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста  // int
    unsigned short constantPhrase_1;
    unsigned short LPhraseHits_3=0;
    unsigned int NPhraseHits=0;          // количество совпавших фраз словаря с фразами распознаваемого текста
    
    
    
#ifdef REMARK
    unsigned int ng=100000;   // сколько графики выводим на экран  // 100000 // 100
    print=1;
    TIME_START
#endif
    ///  параметры для нечеткого поиска  ///
    // Задаются как глобальные (перенесено в associativeSignalProcessing.cpp и дальше)
    /*
     // максимальное количество не четких пар букв в легитимной фразе словаря (dlt=0; четкий поиск).
     delta=0; // 2  // 4
     // минимальная длина части фразы словаря (в парах букв), для нечеткого поиска, которую имеет смысл проверять как непрерывную.
     constantPhrase=8;  //// фиксированного размера = 8
     */
    
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
    /**/
    
    /// основной прогон нечеткого поиска ///
    
    //unsigned int globalMaxPart=0;
    //unsigned int NLPhraseHits=0;
    //unsigned int MaxPartPhraseHit=0;
    //unsigned int NPartPhraseHit=0;
    //    unsigned short nCldF;              // количество непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
    //    unsigned short nLPhrH;             // счетчик длины (ширина) совпавшего непрерывного фрагмента фразы.
    
    
    DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске nEnter="<<nEnter<<endl)
    
    
    unsigned int x,n,w,w_1,oldw; // ,rg, wm1,m,nc,s0,s1 // ,y,p //
    //int blp,bl,bc,cnt;
    //unsigned short ds;         // ,bf
    wstring str;
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    
    NPhraseHits=0;    //SPhraseHits=0;
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    // Цикл перебора фраз
    for( n=0; n < nEnter; n++ ) {
        
        //cout<<n<<" ";
        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase
        
        // Цикл перебора пар букв фразы
        LPhraseHits=0;   w_1=w-1;
        LPhraseHits_3=0;   ////////////////////
        // минимальная длина части фразы словаря (в парах букв), для корреляционного поиска, которую имеет смысл проверять как непрерывную.
        constantPhrase=LPhrase*0.3; // 4  // 8   //// фиксированного размера = 8
        constantPhrase_1=constantPhrase-1;   //локальная переменная
        
        
        //if(n<3)continue;
        for(x=oldw;  x < w_1;  x++) {  // w_1=w-1;
            
            //cout<<"dictionary_data["<<x<<"]="<<BufUpT[dictionary_data[x]]<<" BufUpT1[dictionary_data[x]="<<(short)BufUpT1[dictionary_data[x]]<<endl;
            
            /// уточняющий алгоритм N2. ///
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами
            // распознаваемого текста. Здессь не учитывается информация о точных координатах совпавших букв текста.
            // Счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // Быстрый вариант для четкого поиска (со сбросом, если не совпадет хоть одна буква)
            LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]]; // Совпавшая буква текста найденная
            // по букве фразы словаря & 255(11111111) или НЕ совпавшая буква текста с буквой словаря & 0(00000000).
            // Понятный вариант.
            /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
            
            // По достижении пороговой длины LPhrase-1 подряд идущих совпавших букв текста,
            // останавливаем подсчет длины и переходим в lookup_Process3.
            
            //if ( LPhraseHits > constantPhrase1  ) { goto Leave1; }  // break;
            if(LPhraseHits<text_size1-1)continue;
            else {
#ifdef REMARK
                cout<<"LPhraseHits="<<LPhraseHits<<" n="<<n<<" LPhrase="<<LPhrase<<endl;
                PR(printDictEntry(n););
                //PR(printInputEntry(););
#endif
                
                /// уточняющий алгоритм N3. ///
                // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с
                // непрерывным фрагментом текста уже с учетом точных координатах букв.
                LPhraseHits_3=lookupProcess3(w, oldw);
                //         LPhraseHits_3=8;
                //if ( LPhraseHits_3 > constantPhrase_1  ) { j++; } ///////////// служебный подсчет j
                // LPhraseHits_3 возвращает количество совпавших букв фразы LPhraseHits с буквами распознаваемого текста (Length Phrase Hits)
                // остановка подсчета LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
                searchResult.push_back(n);
                if(searchResult.size()>limit){
                    //  освобождение памяти
                    if ( BufTxt !=NULL ) free(BufTxt);
                    if ( SumTxtP !=NULL ) free(SumTxtP);
                    if ( GravTxt !=NULL ) free(GravTxt);
                    return;
                }
                break;
            }
        } // x
        //   Leave1:;
        /**/
        
        
        // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и
        // совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        if ( LPhraseHits_3 > text_size1-2) {
            
            
            /// уточняющий алгоритм N4. ///
            // старт функции поиска наибольших под вхождений фразы словаря ( т.е. оставляем самый длинный блок во фразе ).
            //lookupProcess4(n, oldw,w);  //ПРОВЕРИТЬ
            // nxx=n;
            // возвращаем их координаты xMaxStart, xMaxEnd (все с учетом нечеткости)
            
            
#ifdef REMARK
            // сколько "ng" графики выводим на экран.
            if ( n < ng ) { // 30000           ////////////////////////////////
                
                // вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)
                DR("вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)"<<endl)
                for(x=0; x < size_BufTxt; x++) {
                    if ( x == xMaxStart ) { cout<<"<"; }       if ( x == xMaxEnd ) { cout<<">"; }
                    //if ( x == xMaxStart ) { cout_<<"|"; }       if ( x == xMaxEnd ) { cout_<<"|"; }
                    if ( BufTxt[x] > 0 ) { str=(wchar_t)BufUpT[BufTxt[x]]; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }  // cout<<"..";
                    //if ( x == xMaxStart ) { cout<<"|"; }       if ( x == xMaxEnd ) { cout<<"|"; }
                }
                DR(endl)
                
                // вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста
                DR("вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста"<<endl)
                DR("длинна совпавшего слова="<<xMaxEndDict-xMaxStartDict<<endl);
                for(x=oldw;  x < w-1;  x++) {
                    s=x-oldw+1;
                    ds=BufUpT[dictionary_data[x]];
                    if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
                    ///if ( x == xMaxStartDict+oldw ) { cout_<<"|"; }       if ( x == xMaxEndDict+oldw ) { cout_<<"|"; }
                    //if ( ds > 0 ) { str=(wchar_t)ds; cout<<s<<Unicode_to_UTF(str); } else { cout<<"-"; }
                    if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }
                }
                DR(endl)
                
                // вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                DR("вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                for(x=oldw;  x < w-1;  x++) {
                    //////s=x-oldw+1;
                    ds=BufUp[dictionary_data[x]];
                    if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw+1 ) { cout<<">"; }
                    str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
                    ///str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
                }
                DR(endl)
                
                // вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)
                DR("вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl)
                for(int x=oldw + xMaxStartDict;  x < oldw + xMaxEndDict+1;  x++) {  // x < w; // x < w-1;
                    ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
                } // x
                DR(endl<<endl)
                
                //cout_<<endl<<"//__________________________________________________________"<<endl<<endl ;
                
                
                
                // вывод на экран координат начала и конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
                ///                 cout<<"xMaxStart="<<xMaxStart<<"  xMaxEnd="<<xMaxEnd<<endl;
                ///                 cout<<"xMaxStartDict="<<xMaxStartDict<<"  xMaxEndDict="<<xMaxEndDict<<endl<<endl;
                
            } // ( n < ng )       ////////////////////////////////
#endif
            /**/
            
        } // if ( LPhraseHits_3 > constantPhrase1 ) // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
        
        // обнуление массива восстановленного текста BufTxt
        memset(BufTxt,0,(size_BufTxt+32)*2);
        /*
         #ifdef REMARK
         cout<<"text_size1="<<text_size1<<"    "<<"clusters_size="<<clusters_size<<"   ";
         printInputEntry();
         // LPhraseHits[0]=LPhraseHits[0]-1   // длина первой найденной фразы дб на 1 меньше
         cout<<"минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную   constantPhrase="<<constantPhrase<<endl;
         cout<<"количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   NPhraseHits="<<NPhraseHits<<endl;
         //cout<<"Max длина совпавшего непрерывного фрагмента фразы MaxPartPhraseHit="<<MaxPartPhraseHit<<endl;
         //cout<<"Max длина совпавшего непрерывного фрагмента фразы за весь словарь LPhraseHits="<<LPhraseHits<<endl;
         //    cout<<"найдено фраз "<<resultReport.size()<<endl;
         cout<<"количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl;
         cout<<"количество циклов по словарю   n="<<n<<endl;
         TIME_PRINT
         #endif
         */        /**/
        
    } // n
    
    //  освобождение памяти
    if ( BufTxt !=NULL ) free(BufTxt);
    if ( SumTxtP !=NULL ) free(SumTxtP);
    if ( GravTxt !=NULL ) free(GravTxt);
    
}//---------------------------------------------------------------------------------------------------------------------------------------
