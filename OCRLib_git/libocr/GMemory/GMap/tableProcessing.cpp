#include "GMap.h"
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
    
    
    // unsigned int nEnter;              // количество переводов каретки Enter в словаре
    unsigned int LPhrase;                // текущая длина фразы словаря  Length Phrase
    unsigned short LPhraseHits;          // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста  // int
    unsigned short LPhraseHits_3=0;
    unsigned int NPhraseHits=0;          // количество совпавших фраз словаря с фразами распознаваемого текста
    unsigned int SPhraseHits=0;          // суммарная длинна всех совпавших фраз словаря с фразами распознаваемого текста
    
    unsigned short constantPhrase1=constantPhrase-1;

                                      ///  параметры для нечеткого поиска  ///
   TIME_START 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // буферный массив текста ( восстановленная через словарь копия массива распознаваемого текста )
    size_BufTxt=text_size1;        // размер массива распознаваемого текста (в байтах), в общем случаеsize_BufTxt=text_size;
                                   // text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки // добавлено 4 III 2012
    /*
    unsigned int sizeTwo_BufTxt=text_size+32;  
    power_Two(&sizeTwo_BufTxt);
    cout<<"sizeTwo_BufTxt="<<sizeTwo_BufTxt;
    cout<<"   округление числа sizeTwo_BufTxt до ближайшей степени двойки в большую сторону  power_Two(sizeTwo_BufTxt)="<<sizeTwo_BufTxt<<endl;
    */
    
    ////unsigned short *BufTxt;
    BufTxt=(unsigned short*)calloc(size_BufTxt+32,2);   // запрос памяти c обнулением // size_BufPhr+32 
    ////BufTxt=(unsigned short*)calloc(sizeTwo_BufTxt,2);   // запрос памяти c обнулением и округленным размером до ближайшей степени двойки    //memset(BufTxt,0,(size_BufTxt+32)*2);  // проверить *2    // обнуление массива Buf1  // 2==size_BufPhr*sizeof(unsigned short)
    //  nev  //
    // буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря ( GravityTxt, GravTxt, SumTxt, TotalTxt ).
    size_GravTxt=text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
    // text_size1 размер распознаваемого текста в буквах без учета кодов пробела или точки // добавлено 4 III 2012
    ////unsigned short *GravTxt;    
    GravTxt=(unsigned short*)calloc(size_GravTxt+32,2);   // запрос памяти c обнулением 

    
  
    unsigned int ng=100000;   // сколько графики выводим на экран  //// 100000 // 100     
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
    


    

    
    /// основной прогон нечеткого поиска ///
    
    unsigned int globalMaxPart=0;
    //unsigned int NLPhraseHits=0;
    //unsigned int MaxPartPhraseHit=0;
    //unsigned int NPartPhraseHit=0;        
//    unsigned short nCldF;              // количество непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
//    unsigned short nLPhrH;             // счетчик длины (ширина) совпавшего непрерывного фрагмента фразы.

    
    DR("список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl)
    
    
    unsigned int x,n,s,w,oldw,rg; // wm1,m,nc,s0,s1 // ,y,p //     
    //int blp,bl,bc,cnt;        
    unsigned short ds;         // ,bf
    wstring str;
    
    

int j=0;/////////////
    
    // Нечеткий поиск фраз словаря в распознаваемом тексте
    // Цикл перебора фраз
    oldw=w=0;   NPhraseHits=0;    SPhraseHits=0; 
    for( n=0; n < nEnter; n++ ) {  //cout<<n<<" ";  
       
        //oldw=w;   wm1=BufE[n];   w=wm1+1; // "oldw" предыдущее значение  "w"  // w=BufE[n]+1;  //  wm1=BufE[n];   w=wm1+1;
        oldw=w;   w=BufE[n+1];
        LPhrase=w-oldw;       // текущая длина фразы Length Phrase  
  
            // Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами распознаваемого текста.
            // На этом этапе не учитывается информация о точных координатах совпавших букв текста.
            // Цикл перебора пар букв фразы
            LPhraseHits=0;  rg=0;
            LPhraseHits_3=0;   ////////////////////
        //cout<<w<<"  "<<oldw<<endl;
        if ( w-oldw > 500  ) { cout<<n<<"слишком длинная фраза словаря ERROR "<<w-oldw;exit(0);}
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
                memset(BufTxt,0,(size_BufTxt+32)*2); 
            
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
    cout<<"количество фраз словаря удовлетворяющим ограничениям на длинну globalMaxPart="<<globalMaxPart<<endl;
    

//    cout<<"найдено фраз "<<resultReport.size()<<endl;
    ///cout<<"количество совпавших фраз словаря с фразами текста NPhraseHits="<<NPhraseHits<<endl;
    cout<<"суммарная длинна всех совпавших фраз словаря с фразами текста SPhraseHits="<<SPhraseHits<<endl;
    cout<<"количество переводов каретки Enter в словаре nEnter="<<nEnter<<endl;
    cout<<"количество циклов по словарю   n="<<n<<endl;
    
#endif        
  /**/ 
    
    TIME_PRINT
//cout<<j<<"j"<<endl;/////////////


         
 

    //  освобождение памяти
    if ( BufTxt !=NULL ) free(BufTxt);
    //if ( BufCldF !=NULL ) free(BufCldF);
    //if ( BufPhr !=NULL ) free(BufPhr);    

    
//      exit(0);  ////////  
    
    
}//--------------------------------------------------------------------------------------------------------------------------------------------  

// Базовая функция четкого или нечеткого поиска, возвращает все результаты
void GMap::tableProcessingFullMatch(vector<uint>&searchResult){
    
    // Поиск слова в словаре.
    // Проверяется полное совпадение слова с фразой словаря.
    
    
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
    /// основной прогон поиска ///
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
            
            str=BufUp[dictionary_data[x]]; DR(Unicode_to_UTF(str)<<" ");         //        DR(text_data[x]<<"  ")
            
            // счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста,
            // быстрый вариант для четкого поиска (со сбросом если не совпадет хоть одна буква)
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
                
                searchResult.push_back(n); // номер фразы в словаре совпавшей с словом текста
                NPhraseHits++;
            } // else
            
        } // x
    no_match:;
    } // n
    
}


// функция обработки результатов ассоциативного поиска
void GMap::renderResult(string &result){
    int size_GravTxt=text_size1;       // text_size размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
    //cout<<"RENDER";
    
    ostringstream out1;//out
    //out<<startSVG();
    string str;

    //out<<"stroke-width="2" fill="red"';
    //out<<"<polygon points=\"";
    
    string color,oldColor="";
    int mod=32;
    //cout<<"start render"<<endl;
    ostringstream out_;
    
    // заполнение и вывод на экран массива суммарных весов пар букв текста по всем фразам словаря (раскраска букв).
    for(int x=0; x < size_GravTxt-1; x++) {
        //str=(wchar_t)resultText[x];
        wstring t; t=(wchar_t)resultText[x];
        str=Unicode_to_UTF(t);
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
        if(resultDelimeter[x] >0){ /// GravTxt[x]/100){   // порог    
            int Dm=resultDelimeter[x]*256/mod;
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
        ///   out<<x*8<<","<<512-(unsigned int)(100*log10(GravTxt[x] +1))<<"  ";
        ///   cout<<x<<"."<<512-(unsigned int)(100*log10(GravTxt[x]+1))<<"  ";
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
    //writeText(str_,"/1.html");
    //str_=out1.str();
    //str_=((GLogicProcessor*)inputData.logicProcessor)->YagpoToUni(str_);
    //writeText(str_,"/_Image2OCR/2.html");
//#endif  

    if ( GravTxt !=NULL ) free(GravTxt);
    
    
}//--------------------------------------------------------------------------------------------------------------------------------------------  



//----------------------------------------------------------------------------    

/// подсчет вероятности буквы проверяемого текста по четкому поиску ///
// фактически это база грамматического корректора

// обработка массива частоты встречаемости пар букв после прогона всего словаря

/// Справка. Двухмерный массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв)
// представлен одномерным массивом, где позиция буквы в тексте - это номер строки массива частоты встречаемости пар букв, а
// адресом в строке является код пар букв (мб код буквы) содержимое - сколько раз данная пара букв была найдена 
// во всех легитимных фрагментах фраз словаря.

/*
 //unsigned short ds;
 
 // поиск максимума в каждой строке массива частоты встречаемости пар букв  
 for(int x=0;  x < size_SumTxtP;  x++) {
 // подсчет max
 // запоминание и сброс max в конце каждой строки
 //ds=dictionary_data[x]; // упакованный код пары букв
 SumTxtP[x]++; // тест
 } // x
 */

// если он в цикле на 7 млн фраз то
// очень долго 1мин    6000/200=30, 60сек/30=2сек реально по 200м буквам 5сек
// полная сортировка каждой строки еще дольше

//---------------------------------------------------------------------------- 

// алгоритм разбора распознаваемого текста четким поиском с использованием словаря слов
// словарь слов сортирован по длинне слов: 
// - при нахождении слова в распознаваемом тексте самое простое: строится маска найденных слов (массив char, а еще лучше как регистр из нескольких long int по 8мь char или в переспективе по 64е 0 и 1), которая заполняет 0 соответствующие места в распознаваемом тексте. Перед поиском словарного слова применяется логическое "и" между распознаваемым текстом и маской найденных слов. Следующее словарное слово ищется уже по более короткой фразе и т.д.
// - при достижении суммарной длинны найденных слов равной длинне прверяемого текста минус dltBalance (суммарную длинну накапливаем просто в переменной), начинаем проверять реальную заполненность маски найденных слов (подсчет единиц, intr popcht, годится и для char, но уже с учетом постранственного положения найденных слов), если маска прочти заполнена (числое единиц меньше dltBalance), то поиск по словарю останвливаем полностью и переходим к следующей распознаваемой фразе. Подсчет единиц ведем только в оставшимся окне маски. 
// где dltBalance априорный это остаток примерно в 3-8 букв.
// - если вычесленный остаток составляет например 10 букв, то все слова из словаря длиннее 10 букв можно не проверять

// Более сложный и красивый вариант: 0 заполняется уже существующая таблица наличия буквы в распознаваемом тексте, точнее часть таблицы соответствующая однократно встречаемым буквам и иногда и многократно встречаемым буквам (1/3), в общем таблицу проще воссоздать заново, а это долго 

//----------------------------------------------------------------------------    

// END 


//-------------------------------------------------------------------------------------------------------//


///olds=0; s=BufE[0]+1;  // "olds" предыдущее значение  "s"   

// эквивалентно  olds=s;  s=BufE[n]+1;
///olds=s;    s=BufE[n+1]+1; 

//-------------------------------------------------------------------------------------------------------//
/*
/// основной прогон нечеткого поиска ///

cout<<"список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl;
olds=s=0;   NPhraseHits=0;    SPhraseHits=0;  // time=2.7241
// нечеткий поиск фраз словаря в распознаваемом тексте
for( n=0; n < nEnter; n++ ) {  
    
    olds=s;  s=BufE[n]+1; // BufE[n]+1;
    LPhrase=s-olds;  // текущая длина фразы Length Phrase              
    // ограничение минимальной длина фразы для нечеткого поиска  min Length Phrase          
    if ( LPhrase > minLPhrase) { //|| LPhrase <= LPhrase //  && LPhrase <= LPhrase 
        
        /// подсчет совпавших букв фразы  ///  for(x=BufE[n]; x < BufE[n+1]; x++)
        // выгоднее прекратить подсчет совпавших букв фразы по достижении порогового коэффициента корреляции
        LPhraseHits=1; // число сравниваемых пар букв фразы LPhraseHits на 1 меньше длинны фразы LPhrase
        for(x=olds; x < s-1; x++) {  // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается                            
            if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; } // if !=
        } // x
        
        // проверка порогового коэффициента корреляции фразы
        if ( 100*LPhraseHits >= correlationPhrase*LPhrase ) {  // >=
#ifdef REMARK
            
            //s0=0;
            cout<<"    "<<LPhrase<<" ";     // просмотр текущий длинны фразы словаря
            // просмотр совпавших фраз целиком, кроме последней буквы
            for(x=olds; x < s-1; x++) {  // -1 не отображает последнюю букву фразы                     
                str=(wchar_t)BufUp[dictionary_data[x]]; cout<<Unicode_to_UTF(str);  //s0++;                
            } // x 
            //cout<<s0;//cout<<endl;
            cout<<"    "<<LPhraseHits<<" "; // просмотр числа совпавших букв во фразе
            // просмотр совпавших букв фраз, кроме последней буквы                   
            for(x=olds; x < s-1; x++) {  // -1 не отображает последнюю букву фразы                 
                if ( BufUpT[dictionary_data[x]] > 0 ) {
                    str=(wchar_t)BufUp[dictionary_data[x]]; cout<<Unicode_to_UTF(str);
                } // if
            } // x              
            cout<<endl;
               
            NPhraseHits++;
            SPhraseHits+=LPhrase;
            
#endif  
            
        } //་if ( 100 
        
    }  //་if ( LPhrase
    
} // n
cout<<endl;
 */

//-------------------------------------------------------------------------------------------------------//

/* 
 unsigned int NLPhraseHits=0;
 unsigned int MaxPartPhraseHit=0;
 unsigned int NPartPhraseHit=0;
 unsigned int globalMaxPart=0;
 
       // вычисление Max длины совпавшего непрерывного фрагмента фразы с непрерывным фрагментом фразы словаря // 107184
////         for(x=olds; x < s-1;  x++) { // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается
for(x=olds; x < s-1 && LPhraseHits < 9;  x++) { // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается 
    d=BufUpT[dictionary_data[x]]; // буква фразы найденная по букве словаря 
    if ( d > 0 ) {           // d > 0 совпавшая буква фразы с буквой словаря
        LPhraseHits++;       //счетчик длины совпавшего непрерывного фрагмента фразы. part
        ////if ( LPhraseHits > MaxPartPhraseHit ) { MaxPartPhraseHit=LPhraseHits; } // вычисление Max длины совпавшего непрерывного фрагмента фразы 
        ////                if ( LPhraseHits > 8 ) {  break; } // globalMaxPart++;
    } // if 
    else { LPhraseHits=0; }  // =0 НЕ совпавшая буква фразы с буквой словаря, сброс счетчика LPhraseHits++;
} // x
////if ( MaxPartPhraseHit > 8 ) { NPartPhraseHit++;  if ( MaxPartPhraseHit > globalMaxPart ) { globalMaxPart=MaxPartPhraseHit; } } 
*/


//p=0; s=LPhraseHits;
//if ( n < 10000 ) { if ( s > p ){ p=s; }  cout<<"  LPhraseHits="<<p;}  

//-------------------------------------------------------------------------------------------------------//

/// основной прогон нечеткого поиска ///
/*
cout<<"список найденных фраз словаря в распознаваемом тексте при нечетком поиске"<<endl;
olds=s=0;   NPhraseHits=0;    SPhraseHits=0;  // time=2.7241
unsigned int minLPhrase1=minLPhrase-1;

//unsigned int NLPhraseHits=0;
unsigned int MaxPartPhraseHit=0;
//unsigned int NPartPhraseHit=0;
unsigned int globalMaxPart=0;

// нечеткий поиск фраз словаря в распознаваемом тексте
for( n=0; n < nEnter; n++ ) {  //cout<<n<<" ";
    
    olds=s;  s=BufE[n]+1; // BufE[n]+1;
    LPhrase=s-olds;  // текущая длина фразы Length Phrase              
    // ограничение минимальной длина фразы словаря для нечеткого поиска  min Length Phrase          
    if ( LPhrase > minLPhrase) { 
        globalMaxPart++;
        ///          дополнительно уменьшает количество обрабатываемых фраз словаря в 2 раза 7362276/2
        ///          d=BufUpT[dictionary_data[x]];  LPhraseHits=1;
        ///          if ( d > 0 ) {           // d > 0 совпавшая буква фразы с буквой словаря
        ///              LPhraseHits++;       //счетчик длины совпавшего фрагмента фразы. 
        ///          } // if 
        ///          if ( LPhraseHits > 8 ) { NLPhraseHits++; }
        
        /// подсчет совпавших букв фразы  ///  for(x=BufE[n]; x < BufE[n+1]; x++)
        // выгоднее прекратить подсчет совпавших букв фразы по достижении порогового коэффициента корреляции
        LPhraseHits=0; // число сравниваемых пар букв фразы LPhraseHits на 1 меньше длинны фразы LPhrase
        /// memset(BufTxt,0,size_BufTxt*2);
        
        // вычисление Max длины совпавшего непрерывного фрагмента фразы с непрерывным фрагментом фразы словаря // 107184
        ////         for(x=olds; x < s-1;  x++) { // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается
        for(x=olds; x < s-1 && LPhraseHits < minLPhrase;  x++) { // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается // < 8 max=7
            d=BufUpT[dictionary_data[x]]; // буква фразы найденная по букве словаря 
            if ( d > 0 ) {           // d > 0 совпавшая буква фразы с буквой словаря
                LPhraseHits++;       //счетчик длины совпавшего непрерывного фрагмента фразы. part 
                ////            if ( LPhraseHits > 8 ) {  break; } // LPhraseHits=1;
            } // if 
            else { LPhraseHits=0; }  // =0 НЕ совпавшая буква фразы с буквой словаря, сброс счетчика LPhraseHits++;
        } // x
        
        
        
        //cout<<" LPhraseHits="<<LPhraseHits<<endl;
        // LPhraseHits++; // LPhraseHits=0 ;
        
        
        //for( p=0; p < LPhrase; p++ ) { BufPhr[p]=0; } // size_BufPhr/2
        //memset(BufPhr,0,size_BufPhr*2);  //memset(BufPhr,0,LPhrase);
        
        // проверка порогового коэффициента корреляции фразы
        if ( LPhraseHits == minLPhrase1 ) { // > minLPhrase1  > 7
            
            ////         if ( 100*LPhraseHits >= correlationPhrase*LPhrase ) { // >=
            ///if ( LPhraseHits > 10 && LPhraseHits < 256 ) {
*/            
            /*             
             //         #ifdef REMARK
             
             StringPhrase=L"";
             for(x=olds; x < s-1; x++) {  // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается 
             d=BufUpT[dictionary_data[x]];
             if ( d > 0 ) { 
             StringPhrase+=(wchar_t)d;
             } 
             else { StringPhrase+=L"."; } 
             } // x
             //StringPhrase+=L"";
             
             if ( n < 10000 ) { cout<<" StringPhrase= "<<Unicode_to_UTF(StringPhrase)<<endl; }  // break; 
             ////             cout<<" StringPhrase= "<<Unicode_to_UTF(StringPhrase)<<endl;
             //s0=0;
             //cout<<"    "<<LPhrase<<" ";     // просмотр текущий длинны фразы словаря
             // просмотр совпавших фраз целиком, кроме последней буквы
             
             
             StringDict=L"";  // обнуление
             for(x=olds; x < s-1; x++) {  // -1 не отображает последнюю букву фразы 
             StringDict+=(wchar_t)BufUp[dictionary_data[x]];  //s0++;                
             } // x 
             
             
             
             
             //void regExpReplace(string &src, string &exp)   
             //format exp  ---  regExp:|:replace:|:count
             //regExp -- (regExp(match1)(match2)...)
             //replace --- /1.../2...
             //count replace count
             
             // regExp --- (StringPhrase)
             //replace --- StringDict
             //count=1;
             
             //StringExp=Unicode_to_UTF(StringPhrase) + ":|:" + Unicode_to_UTF(StringDict) + ":|:1";
             StringExp=Unicode_to_UTF(StringPhrase);
             
             string str=strText;
             
             
             //int result=regExpReplace(str, StringExp);  
             int result=RE2::PartialMatch(str, StringExp); 
             
             if(result){
             resultReport.push_back(StringDict);
             //cout<<" StringDict="<<Unicode_to_UTF(StringDict)<<endl; 
             //cout<<"StringExp="<<StringExp<<endl;
             //cout<<"result="<<result<<" strText Re="<<str;  ///exit(0);             
             }
             */             
/*            // cout<<olds;//cout<<endl;
            // cout<<"    "<<LPhraseHits<<" "; // просмотр числа совпавших букв во фразе
            // просмотр совпавших букв фраз, кроме последней буквы 
            //       if ( n < 10000 ) {
            for(x=olds; x < s-1; x++) {  // -1 не отображает последнюю букву фразы
                d=BufUpT[dictionary_data[x]];
                if ( d == 0 ) { d=95; } 
                ///str=(wchar_t)d; cout<<Unicode_to_UTF(str);
                if ( n < 10000 ) { str=(wchar_t)d; cout<<Unicode_to_UTF(str); } // cout<<"  LPhraseHits="<<LPhraseHits;
            } // x              
            ///           cout<<endl;
            if ( n < 10000 ) { cout<<endl; }
            //       } // if
            //cout<<Unicode_to_UTF(StringPhrase)<<endl;  //cout<<endl;            
            
                
            NPhraseHits++; 
            SPhraseHits+=LPhrase;
            
            //         #endif  
            
        } //་if ( 100 
        
    }  //་if ( LPhrase
    
} // n
cout<<endl;
*/

/////    for(int i=0;i<resultReport.size();i++)cout<<i<<" "<<Unicode_to_UTF(resultReport[i])<<endl;

//-------------------------------------------------------------------------------------------------------

///          дополнительно уменьшает количество обрабатываемых фраз словаря в 2 раза 7362276/2
///          d=BufUpT[dictionary_data[x]];  LPhraseHits=1;
///          if ( d > 0 ) {           // d > 0 совпавшая буква фразы с буквой словаря
///              LPhraseHits++;       //счетчик длины совпавшего фрагмента фразы. 
///          } // if 
///          if ( LPhraseHits > 8 ) { NLPhraseHits++; }


/*
 // вычисление Max длины совпавшего фрагмента текста с непрерывном фрагментом фразы словаря. // 107184 // 101488 // 94785 (&& LPhrase < 128)
 // по достижении пороговой длины совпавших букв фразы словаря minLPhrase, останавливаем подсчет длины.
 LPhraseHits=0; // число сравниваемых пар букв фразы LPhraseHits на 1 меньше длинны фразы LPhrase, LPhraseHits=1;
 //for(x=BufE[n]; x < BufE[n+1]; x++)//
 for(x=olds;  x < s-1 && LPhraseHits < minLPhrase;  x++) { // s1=s-1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается // < 8 max=7
 // буква фразы найденная по букве словаря d=BufUpT[dictionary_data[x]]
 if ( BufUpT[dictionary_data[x]] > 0 ) { // d > 0 совпавшая буква фразы с буквой словаря
 LPhraseHits++;            // счетчик длины совпавшего непрерывного фрагмента фразы. part 
 } // if 
 else { LPhraseHits=0; }       // d = 0 НЕ совпавшая буква фразы с буквой словаря, сброс счетчика LPhraseHits++
 
 } // x
 */

//-------------------------------------------------------------------------------------------------------

//short d;
//unsigned short dlt;
//    short dlt;
//unsigned short delta=1;

/*
// Пороговый коэффициент корреляции фразы из распознаваемого текста и фразы из словаря (глубина нечеткости), 
// при нечетком поиске. Задается от 0 до 100 процентов. 100 процентов это четкий поиск.
unsigned short correlationPhrase=92; // 96            
if ( correlationPhrase > 100 ){ correlationPhrase=100; }     if ( correlationPhrase < 60 ){ correlationPhrase=60; }
float deltaK=(float)(100-correlationPhrase)/100;

// абсолютное значение количества несовпадающих букв (пар букв, чешуйки пар букв) в распознаваемом тексте и фразы из словаря, при нечетком поиске.

unsigned short delta;
delta=deltaK*LPhrase;
*/

//-------------------------------------------------------------------------------------------------------

/*       
 // Развернутый рабочий легкий эквивалент. x+=4  По тестам не быстрее, желательно уточнить         
 ///unsigned short d1;
 ///unsigned long long d0;
 ///unsigned long long *dictionary_data64;
 ///dictionary_data64=(unsigned long long*)dictionary_data;
 
 // массивы для развернутого цикла по  фразе словаря
 ///wstring StringPhrase;
 ///wstring StringDict;
 ///string StringExp;
 ///vector<wstring> resultReport;
 */

 /*
 LPhraseHits=0;
 for(x=olds;  x < s-1 && LPhraseHits < minLPhrase;  x+=4) { // x++  x+=4
 d0=dictionary_data64[x];  // d0=dictionary_data64[x];                           // s=0 s=1  // BufUpTmin[2>>14]=0,1         
 d1=d0;
 ///LPhraseHits=(LPhraseHits + 1) & BufUpT1[d1]; // & 0xFFFF000000000000  // s=BufUpT[s0 & 0xFFFF000000000000];  LPhraseHits=LPhraseHits*s + s
 LPhraseHits=(LPhraseHits + 1) & BufUpT1[(short)d0];
 d1=d0>>16;
 LPhraseHits=(LPhraseHits + 1) & BufUpT1[d1]; // & 0x0000FFFF00000000  // BufUpT1[(short)(d0>>16)];
 d1=d0>>16;
 LPhraseHits=(LPhraseHits + 1) & BufUpT1[d1]; // & 0x00000000FFFF0000
 d1=d0>>16;
 LPhraseHits=(LPhraseHits + 1) & BufUpT1[d1]; // & 0x000000000000FFFF
 
 // LPhraseHits=(LPhraseHits + 1) & BufUpT1[(short)(d0>>16)];
 // LPhraseHits=(LPhraseHits + 1) & BufUpT1[(short)(d0>>16)];
 // LPhraseHits=(LPhraseHits + 1) & BufUpT1[(short)(d0>>16)];
 
 } // x
 */

//-------------------------------------------------------------------------------------------------------

/// memset(BufTxt,0,size_BufTxt*2);
//for( p=0; p < LPhrase; p++ ) { BufPhr[p]=0; } // size_BufPhr/2
//memset(BufPhr,0,size_BufPhr*2);  //memset(BufPhr,0,LPhrase);
/**/

/*
// превращаем bl в положительнай адрес blp, т.к. число bl искуственно отрицательно,  +1 положение счетчика свободных адресов
blp=-bl+1; 
cnt=clusters_data[blp];   ////// заремачить                                                
// считывание первого, второго, третьего и т.д. адресов в цепочке ячеек кластера
bc=clusters_data[blp+1+cnt];   // bc аналог x, только извлеченный из массива цепочек адресов clusters_data[x]
///bc=clusters_data[blp+1+clusters_data[blp]];   // bc аналог x, только извлеченный из массива цепочек адресов clusters_data[x]
// увеличиваем счетчик свободных адресов в цепочке на единицу
cnt++;  ///cnt=clusters_data[blp]+=1;  // clusters_data[blp]=cnt+=1;                        
  // вывод на экран восстановленного распознаваемого текста через массив цепочек адресов
  cout<<bc;    str=(wchar_t)BufUpT[text_data[bc]]; cout<<Unicode_to_UTF(str)<<"  ";  // ds 
  if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter распознаваемого текста            
//// восстанавливаем значение счетчика адресов в цепочке (ячеек кластера) равное нолю // проверить!!!
if ( cnt==-clusters_data[blp-1] ) { cnt=0; }  // устраляет пеполнение счетчика, но дает лишние циклы считывания                        
clusters_data[blp]=cnt;                        
if ( bc==x ) m++; // проверка на совпадение восстановленного и исходного распознаваемого текста
*/

//-----------------------------------------------------------------------------------------------------------------------  

///dlt=deltaK*LPhrase;  // cout<<" deltaK="<<deltaK<<" "<<" LPhrase="<<LPhrase<<" ";
/* 
 delta=LPhrase*s1/s2;
 */

/*
//                cout<<" delta="<<delta<<" ";
// вычисление длинн непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
////// вычисление адресов (начало и конец) непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
// проекция осуществляется через таблицу (массив)
nLPhrH=nCldF=0;
for(x=oldw;  x < w-1;  x++) { // w-1 убирает последнюю пару букв фразы из сравнения, но последняя буква сравнивается 
    d=BufUpT[dictionary_data[x]]; // буква фразы найденная по букве словаря 
    if ( d > 0 ) {                // d > 0 совпавшая буква фразы с буквой словаря
        nLPhrH++;                 // счетчик длины совпавшего непрерывного фрагмента фразы.                         
    } // if  
    else {                        // d = 0 НЕ совпавшая буква фразы с буквой словаря, сброс счетчика LPhraseHits++; 
        ///if ( nLPhrH > minLPhrase1 ) {  // minLPhrase1=minLPhrase-1  
        if ( nLPhrH > constantPhrase1 ) {  // minLPhrase1=minLPhrase-1
            BufCldF[nCldF]=nLPhrH;  nCldF++; 
            ////////////////////////////////
        }                        
        nLPhrH=0; // w-oldw
    }       
} // x    
// nCldF количество непрерывных фрагментов фразы словаря в проекции на распознаваемый текст

// вывод на экран длинн непрерывных фрагментов фразы словаря в проекции на распознаваемый текст
for(x=0;  x < nCldF;  x++) { cout<<" "<<(short)BufCldF[x]<<" "; }  // <<" "<<d<<"   "  //  
    //cout<<endl;
     */
    
    
    
    /*
    // просмотр совпавших букв фраз в виде строки, кроме последней буквы. Не совпавшие буквы выводятся нижним прочерком                   
    for(x=oldw; x < w-1; x++) {  // -1 не отображает последнюю букву фразы
        d=BufUpT[dictionary_data[x]];
        if ( d == 0 ) { d=95; }  // 95 нижний прочерк
        str=(wchar_t)d;  cout<<Unicode_to_UTF(str);  // просмотр совпавших букв во фразе в виде строки
        ///if ( n < 10000 ) { str=(wchar_t)d; cout<<Unicode_to_UTF(str); } // cout<<"  LPhraseHits="<<LPhraseHits;
    } // x                 
    cout<<"  LPhraseHits="<<(short)LPhraseHits<<endl; // просмотр числа совпавших букв во фразе
    ///if ( n < 10000 ) { cout<<endl; }                 
     */ 

//----------------------------------------------------------------------------------------------------------------------- 

//Быстрая сортировка с дальнейшим бинарным поиском.
//Быстрая сортировка - qsort
//Бинарный поиск - bsearch

//

//-----------------------------------------------------------------------------------------------------------------------

// можно удалить
/*
/// код для вывода на экран ///
////            if ( n < 30000 ) {                

/// уточняющий алгоритм N3. ///

//  Описание алгоритма:
// вычисление Max длины совпавшего фрагмента текста с непрерывном фрагментом фразы словаря. 
// оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент совпавший с распознаваемым текстом
// поэтому, по достижении пороговой длины подряд идущих совпавших букв текста constantPhrase, останавливаем подсчет длины 
// и переходим к следующему уточняющему алгоритму (N3), пропуская кучу кода для вывода на экран. 
// информация о точных координатах совпавших букв текста не присутствует, за то быстро.
// Дает выйгрыш по быстродейсдвию примерно в три раза, что дает возможность применить уточняющий алгоритм N4.

///                cout<<endl; /////////////////

// 0  -3  3  2  56  63  -2  2  8  17  -2  2  9  42  -2  2  10  43  -2  2  23  57  -2  2  24  71  -2  2  26  47  -2  2  38  44
s=0;  nc=0;                
for(x=oldw; x < w-1; x++) {
    ds=dictionary_data[x];   // компактный код пары букв в распознаваемом тексте 
    bl=letters_data[ds];   // адреса (позиции) пар букв встечающихся в распознаваемом тексте один раз (без учета одинаковых).
    if ( bl != 0 ) {                       
        // заполнение массива адресами (позициями) пар букв в распознаваемом тексте, встречающихся однократно
        if ( bl > 0 ) { 
            BufTxt[bl-1]=ds;
            // вывод на экран восстановленного распознаваемого текста через разреженный массив 
            // Внимание! Адреса пар букв в тексте прописаны в массиве letters_data>0 и начинаются с единици поэтому bl-1
            ///                        cout<<bl-1;    str=(wchar_t)BufUpT[ds]; cout<<Unicode_to_UTF(str)<<"  ";  
            //        cout<<text_data[x]<<"   ";
            ///                        if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter текста
            //                    if ( bl-1==x-oldw ) nc++; // проверка на совпадение восстановленного и исходного распознаваемого текста
        } // ( bl > 0 )
        // заполнение массива адресами (позициями) пар букв в распознаваемом тексте, встречающихся более одного раза
        else { 
            
            blp=-bl; // превращаем bl в положительнай адрес blp, т.к. число bl искуственно отрицательно 
            cnt=clusters_data[blp+1];   // считываем значение счетчика адресов,  +1 положение счетчика свободных адресов
            // считывание первого, второго, третьего и т.д. адресов в цепочке ячеек кластера
            for(m=0; m < cnt; m++) {
                bc=clusters_data[blp+2+m];   // bc аналог x, только извлеченный из массива цепочек адресов clusters_data[x]
                BufTxt[bc]=ds;
                //                          if ( bc==x-oldw ) nc+=cnt;         // проверка на совпадение восстановленного и исходного распознаваемого текста
                // вывод на экран восстановленного распознаваемого текста
                ///                            cout<<bc;    str=(wchar_t)BufUpT[text_data[bc]]; cout<<Unicode_to_UTF(str)<<"  ";  // ds 
                ///        cout<<text_data[x]<<"   ";
                ///                            if ( x==BufET[s] ){ cout<<endl; s++; }  // перевод каретки Enter распознаваемого текста 
            } // for(m=0;
            // устанавливаем счетчик адресов в цепочке (вторая цифра в заголовке кластера) в ноль 
            // для исключения повторных обращений к данному кластеру.
            clusters_data[blp+1]=0;
            
            
        } // else
        
    } // if ( bl != 0 )                    
    ////                  else { cout<<0<<"  "; }
    
} // x
///                cout<<endl;
//cout<<"nc="<<nc<<"    "<<"LPhrase="<<LPhrase<<"   ";
//if ( nc==LPhrase ) cout<<"ОК"<<endl;

// после ...... обязательно восстанавливаем исходное значение счетчика адресов в цепочке (ячеек кластера) 
// равное количеству адресов в цепочке (вторая цифра в заголовке)
// text_size1≥clusters_size
for(x=0; x < clusters_size-1; x++) { if( clusters_data[x] < 0 ) { clusters_data[x+1]=-clusters_data[x]; }  } // x
// 0  -3  3  2  56  63  -2  2  8  17  -2  2  9  42  -2  2  10  43  -2  2  23  57  -2  2  24  71  -2  2  26  47  -2  2  38  44


//cout<<"size_BufTxt="<<size_BufTxt;


// вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short) 
///cout<<endl<<"вывод на экран восстановленного текста BufTxt начертанием, как текст UTF-16 (short)"<<endl;
///for(x=0; x < size_BufTxt; x++) { cout<<x;  str=(wchar_t)BufUpT[BufTxt[x]]; cout<<Unicode_to_UTF(str)<<"  "; }  cout<<endl;
//           for(x=0; x < size_BufTxt; x++) {
//               ds=BufUpT[BufTxt[x]];                     
//               if ( ds > 0 ) { str=(wchar_t)ds; cout<<x<<Unicode_to_UTF(str); } else { cout<<"_"; }
//           }  
//           cout<<endl; 


// Вычисление фраз словаря имеющих непрерывный фрагмент длинной больше constantPhrase совпавший с непрерывным фрагментом текста.
// На этом этапе присутствует информация о точных координатах совпавших букв текста. 
LPhraseHits=0;
for(x=0;  x < size_BufTxt && LPhraseHits < constantPhrase;  x++) { // s1=w-1 убирает последнюю пару букв фразы словаря из сравнения, но последняя буква сравнивается
    // счетчик длины непрерывного фрагмента фразы словаря совпавшего с непрерывным фрагментом текста, сброс счетчика  LPhraseHits++ .
    // d=BufUpT1[BufTxt[x]]  d>0 совпавшая, d=0 НЕ совпавшая буква текста с буквой словаря     
    /// if ( BufUpT[BufTxt[x]] > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }                 
    // Более быстрый эквивалент предыдущего кода. Совпавшая буква текста найденная по букве фразы словаря d==255(11111111) или 
    // НЕ совпавшая буква текста с буквой словаря 0(00000000).  
    LPhraseHits=(LPhraseHits + 1) & BufUpT1[BufTxt[x]];                
} // x

///////////
 */

//----------------------------------------------------------------------------------------------------------------------- 

/*
// Вычисление фраз словаря имеющие непрерывный фрагмент длинной больше constantPhrase совпавший с буквами распознаваемого текста.
// На этом этапе не учитывается информация о точных координатах совпавших букв текста.
LPhraseHits=0;
for(x=oldw;  x < wm1;  x++) { // wm1=w-1; убирает последнюю пару букв фразы словаря из сравнения, но последняя буква сравнивается
    // счетчик длины непрерывного фрагмента фразы словаря совпавшего с буквами текста, сброс счетчика  LPhraseHits++ .
    // d=BufUpT1[dictionary_data[x]]  d>0 совпавшая, d=0 НЕ совпавшая буква текста с буквой словаря     
    /// if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; }  else { LPhraseHits=0; }                 
    // Более быстрый эквивалент предыдущего кода. Совпавшая буква текста найденная по букве фразы словаря d==255(11111111) или 
    // НЕ совпавшая буква текста с буквой словаря 0(00000000).  
    LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];    
    //if ( LPhraseHits > constantPhrase1  ) { goto Leave1; } 
    if ( LPhraseHits > constantPhrase1  ) { LPhraseHits=lookup_Process3(w, oldw); break; }
} // x
*/

//----------------------------------------------------------------------------------------------------------------------- 

/*
// буферный массив адресов непрерывных фрагментов (облаков, кластеров) фразы словаря в проекции на распознаваемый текст
unsigned int size_BufCldF=maxLPhrase/constantPhrase+1;  // длина буферного массива адресов (в байтах) // /minLPhrase+1;
unsigned char *BufCldF;   // char        
BufCldF=(unsigned char*)calloc(size_BufCldF+32,1);      // запрос памяти c обнулением // size_BufPhr+32 

// буферный массив фразы
unsigned int size_BufPhr=maxLPhrase;    // максимальная длинна фразы LPhraseMax в словаре (в байтах) =128
unsigned short *BufPhr;                 // *BufPhrase; BufF
BufPhr=(unsigned short*)calloc(size_BufPhr+32,2);   // запрос памяти c обнулением // size_BufPhr+32  
//memset(BufPhr,0,(size_BufPhr+32)*2);  // проверить *2    // обнуление массива Buf1  // 2==size_BufPhr*sizeof(unsigned short)
*/ 

//----------------------------------------------------------------------------------------------------------------------- 

/*
 int xx = 9000;
 //double xx = 9000.0;
 double yy;    
 //yy = log( xx );  // Натуральный логарифм x
 yy = log10( xx );                 // Десятичный логарифм x
 cout<<"  xx="<<xx<<" yy="<<yy<<endl;
 */

//----------------------------------------------------------------------------------------------------------------------- 

// блок н1 в подвале
/*    
 // без учета под вхождений, рабочая версия //
 //unsigned long long deltaReggistr;
 unsigned short delta=1;
 LPhrase=64; //////
 
 /// края фраз словаря (в парах букв) в которых работает нечеткий поиска ///           
 unsigned short LPhraseMin=16; // минимальная длина фразы словаря (в парах букв) начиная с которой включается нечеткий поиска.
 unsigned short LPhraseMax=128;// максимальная длина фразы словаря (в парах букв) в которых работает нечеткий поиска.
 /// нечеткость (глубина нечеткости) соответствующая краям фраз словаря (в парах букв) в которых работает нечеткий поиска ///            
 unsigned short deltaMin=1;    // нечеткость (в парах букв) соответствующая LPhraseMin
 unsigned short deltaMax=8;    // нечеткость (в парах букв) соответствующая LPhraseMax  // 3   //////////////////////////////
 
 if ( LPhraseMin < 6 ) { LPhraseMin=6; };     if ( LPhraseMin > 31 ) { LPhraseMin=31; };
 if ( LPhraseMax < 32 ) { LPhraseMax=32; };   if ( LPhraseMax > 128 ) { LPhraseMax=128; };
 
 if ( deltaMin > 1 ) { deltaMin=1; };
 if ( deltaMax < 1 ) { deltaMax=1; };         if ( deltaMax > 16 ) { deltaMax=16; };
 
 // нечеткость (в парах букв) для промежуточных длин фраз словаря LPhrase
 //delta=LPhrase*(deltaMax-deltaMin)/(LPhraseMax-LPhraseMin);
 unsigned short s1=deltaMax-deltaMin;
 unsigned short s2=LPhraseMax-LPhraseMin;
 delta=LPhrase*s1/s2;
 
 
 // минимальная длина под вхождения фразы словаря в буквах, которую имеет смысл проверять    
 constantPhrase=(LPhrase-delta)/(delta+1); // непрерывный блок  constantPhrase
 
 cout<<"LPhrase="<<LPhrase<<"   delta="<<delta<<"    constantPhrase="<<constantPhrase<<endl; // <<" "    
 
 //LPhrase=LPhrase>>3;  // >>3  16 слов по 4 бита находятся в регистре deltaReggistr
 
 // если текущая длина проверяемой фразы словаря LPhrase<minLPhrase то не ищет вообще, LPhrase=minLPhrase ищет четким поиском, 
 // LPhrase>minLPhrase ищет не четким поиском в соответствии с критерием глубины нечеткости 
 */  

//----------------------------------------------------------------------------------------------------------------------- 

// блок н2 в подвале
/*
 /// предварительный прогон нечеткого поиска, для вычисления размера ///
 
 cout<<"предварительный прогон нечеткого поиска, для вычисления размера короткого словаря"<<endl;
 // размер большого словаря sizeText=670730860 мегабайт, поэтому проверяем через 16 // 670730860/8=80000000=80 мегабайт
 // в больших словарях считается только каждая 16 фраза
 unsigned int m=1;
 float reserve=1;    // коэффициент запаса
 if ( dict_size > 80000000 ) { m=16; reserve=1.3; }  // > 80 мегабайт
 
 // вычисление размера короткого словаря
 olds=s=0;    // time=0.216369  n+=16
 for( n=0; n < nEnter; n+=m ) {          
 olds=BufE[n]+1;  s=BufE[n+1]+1; // olds=s;
 LPhrase=s-olds;  // текущая длина фразы Length Phrase              
 // ограничение минимальной длина фразы для нечеткого поиска  min Length Phrase          
 if ( LPhrase > minLPhrase ) {            
 /// подсчет совпавших букв фразы  ///  for(x=BufE[n]; x < BufE[n+1]; x++)
 LPhraseHits=1;     // число сравниваемых пар букв фразы LPhraseHits на 1 меньше длинны фразы LPhrase
 for(x=olds; x < s-1; x++) {  // -1 убирает последнюю пару букв фразы из сравнения но последняя буква сравнивается                            
 if ( BufUpT[dictionary_data[x]] > 0 ) { LPhraseHits++; } // if !=
 } // x            
 // проверка порогового коэффициента корреляции фразы
 if ( 100*LPhraseHits >= correlationPhrase*LPhrase ) { SPhraseHits+=LPhrase;  NPhraseHits++; } //་if ( 100     
 }  //་if ( LPhrase        
 } // n
 cout<<endl;
 SPhraseHits=SPhraseHits*m*reserve;  // реальная разница Uni_Uni 1.02, Uni_Yagpo 1.07  коэффициент запаса 1.3
 
 ///TIME_PRINT_
 
 #ifdef REMARK
 cout<<"количество совпавших фраз словаря с фразами текста, предварительный прогон NPhraseHits="<<NPhraseHits<<endl;
 cout<<"суммарная длинна всех совпавших фраз, предварительный прогон SPhraseHits*m*reserve="<<SPhraseHits<<endl;
 cout<<"n="<<n<<endl;
 #endif 
 */

//----------------------------------------------------------------------------------------------------------------------- 

// блок н3 в подвале
/*               
 //               // можно поверить четверки букв (чешуйки со сдвигом на одну букву) 
 //               LPhraseHits=0;   short LPhraseHits1=0;   dlt=2 ;
 //               for(x=olds;  x < w-1 && LPhraseHits < constantPhrase;  x++) { // s1=w-1 убирает последнюю пару букв фразы словаря из сравнения
 //                   d=BufUpT1[dictionary_data[x]];
 //                   d=(dlt+1)*d/255-dlt;    
 //                   LPhraseHits1+=d;
 //                   if ( LPhraseHits1 < 0 ) { LPhraseHits1=0; }               
 //                   if ( LPhraseHits1 > 0 ) { LPhraseHits++; } else { LPhraseHits=0; }
 //                   
 //                   // d>0 совпавшая, d=0 НЕ совпавшая буква
 //                   // Совпавшая буква текста найденная по букве фразы словаря d==255(11111111) 
 //                   // или НЕ совпавшая буква текста с буквой словаря 0(00000000)
 //                   //LPhraseHits=(LPhraseHits + 1) & BufUpT1[dictionary_data[x]];                
 //               } // x
 */                 

//----------------------------------------------------------------------------------------------------------------------- 

/*
 int xx = 9000;
 //double xx = 9000.0;
 double yy;    
 //yy = log( xx );  // Натуральный логарифм x
 yy = log10( xx );                 // Десятичный логарифм x
 cout<<"  xx="<<xx<<" yy="<<yy<<endl;
 */

//----------------------------------------------------------------------------------------------------------------------- 

//-----------------------------------------------------------------------------------------------------------------------


 