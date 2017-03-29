#include "GMap.h"
#include "GLogicProcessor.h"
//#include <smmintrin.h>  //_mm_popcnt_u64

#include <math.h>   // log

//#define REMARK

using namespace std;


void GMap::tableProcessing(int mode){
   
    
    // Базовая функция четкого или нечеткого поиска.
    // поиск максимальных под вхождений фраз словаря в распознаваемом тексте 

    int print=0;
    
    DR(endl<<endl<<"******************************************************************************************************************"<<endl)
    DR("Start program *table_Processing*"<<endl)

/**/     
   
///TIME_START      
    
    
    /// применение словаря к распознаваемому тексту ///  
    
    // поиск максимальных под вхождений фраз словаря в распознаваемом тексте 
    
    // задачи (зачем?):
    // - поиск максимальных под вхождений фраз словаря в распознаваемом тексте
    // - это д.б. не четкий поиск в аспекте расхождения соответствующих максимальных под вхождений фраз словаря с фразами 
    //   распознаваемоого текста на 1-2 символа в произвольном месте фразы (как "*" в поиске)  
    // - и одновременно это д.б. не четкий поиск в аспекте применения "молекулы" к распознаваемому тексту. Каждой букве распознаваемоого текста
    //   соответствует три наиболее подходящих по коэффициенту корреляции буквы, претендующие на данное место. Можно так-же использовать 
    //   подходящие буквы не только по коэффициенту корреляции, но и по грамматике (или по иному принципу).  
    //   (при задуманном подходе, с технической точки зрения, это просто эквивалентно увеличению длинны распознаваемоого текста в три раза) 
    // Замечания:
    // - если не искать под вхождения то минимальную длину под вхождения фразы словаря можно сделать равной самой длинне фразы словаря,
    //   что очень прилично улучшит быстродействие
    // - часто, для краткости вместо слов "пара букв" употребляется просто "буква"
    // - фраза "непрерывный фрагмент фразы словаря" в контексте не четкого поиска намекает на возможность применения очень быстрого 
    //   четкого поиска к этому фрагменту фразы 
    
    DR(endl<<"ПРИМЕНЕНИЕ СЛОВАРЯ К РАСПОЗНАННОМУ ТЕКСТУ"<<endl)
    
    
    // uint nEnter;              // количество переводов каретки Enter в словаре
    uint LPhrase;                // текущая длина фразы словаря  Length Phrase
    uint LPhraseHits;          // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста  // int
    uint LPhraseHits_3=0;
    uint NPhraseHits=0;          // количество совпавших фраз словаря с фразами распознаваемого текста
    uint SPhraseHits=0;          // суммарная длинна всех совпавших фраз словаря с фразами распознаваемого текста
    
    uint constantPhrase1=constantPhrase-1;

                                      ///  параметры для нечеткого поиска  ///
   TIME_START 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // буферный массив текста ( восстановленная через словарь копия массива распознаваемого текста )
    size_BufTxt=(uint)text_size1;        // размер массива распознаваемого текста (в байтах), в общем случаеsize_BufTxt=text_size;
    BufTxt_vector->reserve(size_BufTxt); // запрос памяти c обнулением // size_BufPhr
    BufTxt=BufTxt_vector->dataPtr();
    // буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря ( GravityTxt, GravTxt, SumTxt, TotalTxt ).
    size_GravTxt=(uint)text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
    // text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки // добавлено 4 III 2012
    GravTxt_vector->reserve(size_GravTxt);
    GravTxt=GravTxt_vector->dataPtr();   // запрос памяти c обнулением
  
    /// массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв)
    // где позиция буквы в тексте это номер строки массива частоты встречаемости пар букв
    // адресом в строке является код пар букв (мб код буквы) содержимое - сколько раз данная пара букв была найдена 
    // во всех легитимных фрагментах фраз словаря.
    
    //size_SumTxtP=(uint)(text_size1*nLetterP); // nLetterP // полный размер массива частоты встречаемости пар букв,
    // где nLetterP равен количеству разных пар букв в словаре (примерно 6000), для букв nLetter (nLetter=214 uint) 
    // text_size1 размер распознаваемого текста (в байтах) в буквах без учета кодов пробела или точки (в общем случае size_BufTxt=text_size;)
    //SumTxtP_vector->reserve(size_SumTxtP); // запрос памяти c обнулением
    //SumTxtP=SumTxtP_vector->dataPtr();

  
    /// основной прогон нечеткого поиска ///
    
    //uint globalMaxPart=0;
    //uint NLPhraseHits=0;
    //uint MaxPartPhraseHit=0;
    //uint NPartPhraseHit=0;        
//    uint nCldF;              // количество непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
//    uint nLPhrH;             // счетчик длины (ширина) совпавшего непрерывного фрагмента фразы.

#ifdef REMARK
    DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl)
    uint ng=100000;   // сколько графики выводим на экран  //// 100000 // 100
    ulong s;
    mSIZE ds;
#endif
    
    ulong x,n,w,oldw,rg; // wm1,m,nc,s0,s1 // ,y,p //
    //int blp,bl,bc,cnt;        
    wstring str;
    
    

int j=0;/////////////
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    // Цикл перебора фраз
    oldw=w=0;   NPhraseHits=0;    SPhraseHits=0; 
    for( n=0; n < nEnter; n++ ) {  //cout<<n<<" ";  
       
        //oldw=w;   wm1=BufE[n];   w=wm1+1; // "oldw" предыдущее значение  "w"  // w=BufE[n]+1;  //  wm1=BufE[n];   w=wm1+1;
        oldw=w;   w=BufE[n+1];
        LPhrase=(uint)(w-oldw);       // текущая длина фразы Length Phrase
  
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами распознаваемого текста.
            // На этом этапе не учитывается информация о точных координатах совпавших букв текста.
            // Цикл перебора пар букв фразы
            LPhraseHits=0;  rg=0;
            LPhraseHits_3=0;   ////////////////////
        //cout<<w<<"  "<<oldw<<endl;
        if ( w-oldw > 1500  ) {
            cout<<n<<"слишком длинная фраза словаря ERROR "<<w-oldw;exit(0);
        }
            for(x=oldw;  x < w;  x++) { // wm1=w-1; убирает последнюю пару букв фразы словаря из сравнения, но последняя буква сравнивается
                // счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста, сброс счетчика  LPhraseHits++ .
                // d=BufUpT1[dictionary_data[x]]  d>0 совпавшая, d=0 НЕ совпавшая буква текста с буквой словаря     
                /// if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }                 
                // Более быстрый эквивалент предыдущего кода. Совпавшая буква текста найденная по букве фразы словаря d==255(11111111) 
                // или НЕ совпавшая буква текста с буквой словаря 0(00000000).
                ///cout<<x<<"   ";
                LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
                ///rg=(rg|(BufUpT1[dictionary_data[x]]<<1))>>1;   LPhraseHits=_mm_popcnt_u32(rg); //<<1 умножение на 2 //rg=(rg|BufUpT1[dictionary_data[x]])>>1;
                //if ( LPhraseHits > constantPhrase1  ) { goto Leave1; }  // break;
                if ( LPhraseHits > constantPhrase1  ) {    
                    //cout<<LPhraseHits<<"   ";
                    /// уточняющий алгоритм N3. ///
                    // старт функции поиска совпавшего непрерывного фрагмента фразы словаря с 
                    // непрерывным фрагментом текста уже с учетом точных координатах букв.
                    LPhraseHits_3=lookupProcess3(w, oldw);
                    if ( LPhraseHits_3 > constantPhrase1  ) { j++; } ///////////// служебный подсчет j
                    // LPhraseHits_3 возвращает количество совпавших букв фразы LPhraseHits с буквами распознаваемого текста (Length Phrase Hits)
                    break; // остановка подсчета LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];
                }
            } // x
            //   Leave1:;
                // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и 
                // совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
                if ( LPhraseHits_3 > constantPhrase1 ) {
                     
                    
                    /// уточняющий алгоритм N4. ///
                    // старт функции поиска наибольших под вхождений фразы словаря ( т.е. оставляем самый длинный блок во фразе ).
                    lookupProcess4(n, oldw,w);
                    // nxx=n;
                    // возвращаем их координаты xMaxStart, xMaxEnd (все с учетом нечеткости)
          
                    
#ifdef REMARK                      
                    /// код для вывода в log.txt или на экран  ///
if ( n < ng ) { // 30000           ////////////////////////////////                    
                        cout<<"вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short) "<<endl;
                        /*
                        for(x=0; x < size_BufTxt; x++) {
                            if ( x == xMaxStart ) { cout<<"<"; }       if ( x == xMaxEnd ) { cout<<">"; }
///                         if ( x == xMaxStart ) { cout_<<"|"; }       if ( x == xMaxEnd ) { cout_<<"|"; }
                            ds=BufUpT[BufTxt[x]];                            
                            //if ( ds > 0 ) { str=(wchar_t)ds; cout<<x<<Unicode_to_UTF(str); } else { cout<<"-"; } // cout<<"..";
                            if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }  // cout<<"..";
                            //if ( x == xMaxStart ) { cout<<"|"; }       if ( x == xMaxEnd ) { cout<<"|"; }
                        }  
                        cout<<endl;
                        */
                        cout<<" вывод на экран фраз словаря dictionary_data начертанием, как текст UTF-16 (short), состоящих из букв текста "<<endl;
                        for(x=oldw;  x < w-1;  x++) {
                            s=x-oldw+1;
                            ds=BufUpT[dictionary_data[x]];
                            if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw ) { cout<<">"; }
///                         if ( x == xMaxStartDict+oldw ) { cout_<<"|"; }       if ( x == xMaxEndDict+oldw ) { cout_<<"|"; }
                            //if ( ds > 0 ) { str=(wchar_t)ds; cout<<s<<Unicode_to_UTF(str); } else { cout<<"-"; }
                            if ( ds > 0 ) { str=(wchar_t)ds; cout<<Unicode_to_UTF(str); } else { cout<<"-"; }
                        }  
                        cout<<endl;
                        
                        cout<<" вывод на экран полной фразы словаря dictionary_data начертанием, как текст UTF-16 (short)"<<endl;
                        for(x=oldw;  x < w-1;  x++) {
                            //////s=x-oldw+1;
                            ds=BufUp[dictionary_data[x]];
                            if ( x == xMaxStartDict+oldw ) { cout<<"<"; }       if ( x == xMaxEndDict+oldw ) { cout<<">"; }
                            str=(wchar_t)ds; cout<<Unicode_to_UTF(str);
///                         str=(wchar_t)ds; cout_<<Unicode_to_UTF(str);
                        }
    
                        cout<<endl;
                        cout<<" вывод на экран совпавшей с текстом части фразы словаря dictionary_data начертанием, как текст UTF-16 (short) "<<endl;
                        for(int x=oldw + xMaxStartDict;  x < oldw + xMaxEndDict;  x++) {  // x < w; // x < w-1;
                            ds=BufUp[dictionary_data[x]];  str=(wchar_t)ds; cout<<Unicode_to_UTF(str);                        
                        } // x    
    
    
                        cout<<endl<<endl;
                    //cout_<<endl<<"//__________________________________________________________"<<endl<<endl ;
                        
                        
                        // вывод на экран координат начала и конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны 
///                     cout<<"xMaxStart="<<xMaxStart<<"  xMaxEnd="<<xMaxEnd<<endl;
///                     cout<<"xMaxStartDict="<<xMaxStartDict<<"  xMaxEndDict="<<xMaxEndDict<<endl<<endl;
                        
                    } // ( n < ng )       ////////////////////////////////
#endif 
/**/
        
                    NPhraseHits++; ///
                    
                } // if ( LPhraseHits_3 > constantPhrase1 ) // оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент длинной больше constantPhrase и совпавший с непрерывным фрагментом текста с учетом точных координатах букв.
 

                // обнуление массива восстановленного текста BufTxt
                memset(BufTxt,0,(size_BufTxt)*sizeof(mSIZE));
            
    } // n
//    cout<<endl;

#ifdef REMARK    
 
     
cout<<"text_size1="<<text_size1<<"    "<<"clusters_size="<<clusters_size<<"   ";
  
    // вывод на экран распознаваемого текста text_data начертанием, как текст UTF-16 (short)
    cout<<endl<<"BufUpT - распознаваемый текст text_data (упакованные пары букв), шрифтом, UTF-16 (short)"<<endl;
    s=0;
    for(x=0; x < text_size1; x++){ 
        str=(wchar_t)BufUpT[text_data[x]]; cout<<Unicode_to_UTF(str);         //        cout<<text_data[x]<<"  ";
        if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter текста
    }
    cout<<endl;
    /**/
    
    
    // LPhraseHits[0]=LPhraseHits[0]-1   // длина первой найденной фразы дб на 1 меньше
    cout<<"минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную   constantPhrase="<<constantPhrase<<endl;
    cout<<"количество фраз словаря с длиной непрерывного совпавшего фрагмента фразы  > constantPhrase   NPhraseHits="<<NPhraseHits<<endl;
        //cout<<"Max длина совпавшего непрерывного фрагмента фразы MaxPartPhraseHit="<<MaxPartPhraseHit<<endl;
        //cout<<"Max длина совпавшего непрерывного фрагмента фразы за весь словарь LPhraseHits="<<LPhraseHits<<endl;
    //cout<<"количество фраз словаря удовлетворяющим ограничениям на длинну globalMaxPart="<<globalMaxPart<<endl;
    

//    cout<<"найдено фраз "<<resultReport.size()<<endl;
    ///cout<<"количество совпавших фраз словаря с фразами текста NPhraseHits="<<NPhraseHits<<endl;
    cout<<"суммарная длинна всех совпавших фраз словаря с фразами текста SPhraseHits="<<SPhraseHits<<endl;
    cout<<"количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl;
    cout<<"количество циклов по словарю   n="<<n<<endl;
    
#endif        
  /**/ 
    
    TIME_PRINT
//cout<<j<<"j"<<endl;/////////////

    
}//--------------------------------------------------------------------------------------------------------------------------------------------  

// Базовая функция четкого или нечеткого поиска, возвращает все результаты
void GMap::tableProcessingFullMatch(vector<ulong>&searchResult){
    
    // Поиск слова в словаре.
    // Проверяется полное совпадение слова с фразой словаря.
    
    
    int print=0; // вывод отладочной информации 1 - выводим, 0 - нет
#ifdef REMARK
    print=1;
    //TIME_START
    printInputEntry();
#endif
    
    DR(endl<<endl<<"*****************************************************"<<
       "*********************************************************"<<endl);
    DR("Start program *tableProcessing_fullMatch*"<<endl)
    
    uint LPhrase;       // текущая длина фразы словаря  Length Phrase
    uint LPhraseHits; // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста
    uint NPhraseHits=0; // количество совпавших фраз словаря с фразами распознаваемого текста
    
    ulong x,n,w,w_1,oldw;
    mSIZE ds;
    wstring str;
    /// основной прогон поиска ///
    NPhraseHits=0;
    w=BufE[0];  if( w<1)  w=1; // "w" не должно быть w<1, иначе w_1=w-1 будет отрицательным адресом массива.
    
    
    
    // Цикл перебора фраз словаря.
    for( n=0; n < nEnter; n++ ) {  //DR(n<<" ";
        
        oldw=w;   w=BufE[n+1];
        LPhrase=(uint)(w-oldw);       // текущая длина фразы Length Phrase
        // для уменьшения объема словаря, проверяем совпадение длинны фразы словаря с длинной слова текста.
        if(LPhrase!=text_size1)continue;
        
        //printDictTextLine(n);
        
        // Цикл перебора пар букв фразы
        LPhraseHits=0;   w_1=w-1;
        for(x=oldw;  x < w_1;  x++) {
            ds=BufUpT1[dictionary_data[x]];
            
            str=BufUp[dictionary_data[x]]; DR(Unicode_to_UTF(str)<<" ");         //        DR(text_data[x]<<"  ")
            
            // счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // быстрый вариант для четкого поиска (со сбросом если не совпадет хоть одна буква)
            LPhraseHits=(LPhraseHits + 1) & ds;
            // понятный вариант для нечеткого поиска
            /// if ( ds > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }
            
            // По достижении пороговой длины text_size1-1 подряд идущих совпавших букв текста, останавливаем подсчет длины и переходим к следующей фразе.
            if(LPhraseHits!=text_size1-3)continue;
            else {
                //цикл окончательной проверки совпадения слова текста и фразы в словаря
                for(int i=0;i<LPhrase-1;i++){
                    //DR("dictionary_data[i]="<<dictionary_data[i+oldw]<<" text_data[i]="<<text_data[i]<<endl)
                    if(dictionary_data[i+oldw]!=text_data[i])goto no_match;
                }
                
                searchResult.push_back(n); // номер фразы в словаре совпавшей с словом текста
                NPhraseHits++;
            } // else
            
        } // x
    no_match:;
    } // n
    
}


// функция обработки результатов ассоциативного поиска
void GMap::renderResult(string &result){
    ulong size_GravTxt=text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
    //cout<<"RENDER";
    
    ostringstream out1;//out
    //out<<startSVG();
    string str;

    //out<<"stroke-width="2" fill="red"';
    //out<<"<polygon points=\"";
    wstring sW=UTF_to_Unicode(result);
    
    string color,oldColor="";
    int mod=32;
    //cout<<"start render"<<endl;
    ostringstream out_;
    
    // заполнение и вывод на экран массива суммарных весов пар букв текста по всем фразам словаря (раскраска букв).
    for(int x=0; x < size_GravTxt-1; x++) {
        //str=(wchar_t)resultText[x];
        //wstring t; t=(wchar_t)resultText[x];
        wstring t; t=(wchar_t)sW[x];
        str=Unicode_to_UTF(t);
        //str=((GLogicProcessor*)inputData.logicProcessor)->YagpoToUni(str);
        //cout<<str<<resultDelimeter[x+1];
        //cout<<x<<".-"<<GravTxt[x];
        // масштабирование графика суммарных весов для вывада вывод в файл SVG (на экран)
        //out<<x*8<<","<<512-GravTxt[x]/64<<" ";
        
        // таблица раскраски текста
        int Dt=GravTxt[x];   //cout<<GravTxt[x]<<"/"<<Dt;
        if(GravTxt[x]==0){color="#CC0066"; 
        
        }else{
            if(Dt>15)Dt=15;
            out_<<"#00"<<hex<<7-Dt/2<<"00"<<15-Dt;
            color=out_.str();   //cout<<" "<<color<<endl;
            out_.str("");
        }    
        // объединение в стек букв одного цвета
        if(x==0){
           out1<<"<font color=\""<<color<<"\" c=\""<<GravTxt[x]<<"\">"<<str;    
        }else{
            if(oldColor!=color){
                out1<<"</font><font color=\""<<color<<"\" c=\""<<GravTxt[x]<<"\">"<<str; 
            }else{
                out1<<str;
            }
        } // }else{
        
        // вывод на экран раскрашенного массива суммарных весов разделителей по всем фразам словаря (раскраска разделителей).
        oldColor=color;  
        // если количество разделителей больше чем 1/20 от количества букв, то на данное место ставим разделитель
        // можно по абсолютному значению количества разделителей, если resultDelimeter[x]>3, то на данное место ставим разделитель
        // if(resultDelimeter[x] > 0 && resultDelimeter[x] > GravTxt[x]/20){   // избыточно  
        if(resultDelimeter[x] >GravTxt[x]/20){ /// GravTxt[x]/100){   // порог
            //uint Dm=(uint)resultDelimeter[x]*256/mod;
            // таблица раскраски разделителей (пробелов-точек)
            //if(resultDelimeter[x]>0)color="#00EA00";
            //if(GravTxt[x]==0)color="#FF0000"; 
            //if(Dm>32)Dm=32;
            //out_<<"#"<<hex<<64-Dm*2<<160-Dm*5<<255-Dm*7;
            //color=out_.str();
            //out_.str("");
            
            out1<<"</font><font color=\""<<color<<"\" c=\""<<resultDelimeter[x]<<"\">"<<"་"; //<<(float)resultDelimeter[x+1]/GravTxt[x]
        }
        //cout<<" resultDelimeter[x]="<<resultDelimeter[x]<<endl;
        //str=(wchar_t)text_data[x]; out1<<Unicode_to_UTF(str); // cout<<Unicode_to_UTF(dictionary_data[x]);
        //cout<<text_data[x]<<"  ";
        
        // график в логарифмической шкале
        ///   out<<x*8<<","<<512-(uint)(100*log10(GravTxt[x] +1))<<"  ";
        ///   cout<<x<<"."<<512-(uint)(100*log10(GravTxt[x]+1))<<"  ";
        ///cout<<x<<"."<<log10(GravTxt[x]+1)<<"  ";
        ///cout<<x<<"."<<int(sqrt(GravTxt[x]))<<"  ";
        
        //cout<<"."<<GravTxt[x];
    } // x
    out1<<"</font>";
    //out<<"\"/ style=\"stroke:width 2; stroke: red; fill:none \"/>";

    
    //cout<<endl<<endl;
    
//#ifdef REMARK    
    // вывод в файл SVG
    //out<<closeSVG();
    result=out1.str();
    //writeText(result,"/_Image2OCR/1.html");
    //str_=out1.str();
    //str_=((GLogicProcessor*)inputData.logicProcessor)->YagpoToUni(str_);
    //writeText(str_,"/_Image2OCR/2.html");
//#endif  
    
}//--------------------------------------------------------------------------------------------------------------------------------------------  

