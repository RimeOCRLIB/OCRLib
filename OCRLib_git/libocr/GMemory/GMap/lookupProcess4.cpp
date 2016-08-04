#include "GMap.h"

//using namespace std; 


bool GMap::lookupProcess4(unsigned int n,unsigned int oldw,unsigned int w){   // int *xMaxStart, int *xMaxEnd
 
    
    // поиск наибольших не четких под вхождений фразы словаря ( т.е. оставляем самый длинный блок во фразе ).    
        
    unsigned int x;               // ,s0,s1 // ,y,p // n,
    //int bl,blp,bc;              // ,cnt,bc,bm,counter
    //unsigned short ds;          // ,bf    

   
    /// уточняющий алгоритм N4. /// не дописано
    
    //  Описание алгоритма:
    // Алгоритм работает только с фразами словаря прошедшими предыдущие этапы N1,N2 (в функции table_Processing) и 
    // N3 (в функции lookup_Process3).
    // Алгоритм предназначен для поиска наибольших под вхождений фразы словаря т.е. в итоге оставляем самый длинный блок  
    // в словарной фразе (с учетом нечеткости) и сохраняем его координаты как два short (int). Поиск может быть четким и не четким.
    // Поиск осуществляется по буквам расположенным по реальным координатам распознаваемого текста.
    // (буква это это на самом деле чешуйка из пары букв с перекрытием в одну букву)        
    // На этом этапе также явно присутствует информация о точных координатах совпавших букв текста.

                
    // ....2ག....5ྱ......9ར10ྣ..12པ..................22ི23ས..25ོ........30ད..........36ས37ེ38མ39ས40ཅ41ན42ར43ྣ44མ............51ི........56ག57ས..........63ག64ས..........70ན..........................................................

    unsigned int dlt=delta; // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск). table_Processing
    unsigned int dlt1=dlt+1;
    //unsigned int dltEnd=0;  // остаток от dlt для начала или конца кусочнонепрерывного фрагмента текста максимальной длинны
    int cnt0=0, cnt1=0;     // счетчики нулей (отсутствие буквы в данной позиции в тексте) и единиц (наличие буквы) в интервале поиска
    int cnt0Max, cnt1Max;   // максимальное значение счетчиков нулей и единиц  // длинна,
    int cntDlt, cntDltEnd;
    
    xMaxStart=0;            // координаты начала нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
    xMaxEnd=0;              // координаты конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
      
    unsigned int size_BufTxtD=size_BufTxt-dlt;
    unsigned int size_BufUpT=nLetterP;  //=nLetterP;   // sizeof(unsigned short) // 65536
    
    
//------------------------------------------------------------------
    

    // в обычном массиве BufTxt на каждой позиции находятся компактные коды пар букв узнанные словарной фразой
    // BufTxtDic массив в размер проверяемого текста, где на каждой позиции находятся адреса (в словарной фразе) соответствующих  
    // пар букв, узнанные словарной фразой. Вместо компактных кодов пар букв узнанных словарной фразой находятся адреса  
    // пар букв (в словарной фразе), узнанные словарной фразой.
 
//  BufTxtDic[адреса пар букв в тексте] = адреса пар букв в словарной фразе;   // заполнение массива адресами
//  адреса пар букв в словарной фразе   = BufTxtDic[адреса пар букв в тексте]; // чтение массива адресов
    
//------------------------------------------------------------------    

    // Вычисление максимального непрерывного фрагмента текста для текущей словарной фразы.
    
    if ( dlt == 0 ) { // ЧЕТКИЙ поиск.

        // Четкий поиск. Вычисление максимального непрерывного фрагмента текста для текущей словарной фразы.    
        cnt1=0;  cnt1Max=0;  
        for(x=0;  x < size_BufTxt;  x++) { 
            // счетчик длины непрерывного фрагмента фразы словаря совпавшего с непрерывным фрагментом текста.                 
            if ( BufTxt[x] > 0 ) { 
                cnt1++; 
            }  
            else { 
                // нахождение максимума количества единиц cnt1 в распознаваемом тексте
                if ( cnt1 > cnt1Max ) { cnt1Max=cnt1; xMaxEnd=x; }
                // сброс счетчика  cnt1 
                cnt1=0;
            } // else
        } // x
        if ( cnt1 > cnt1Max ) { cnt1Max=cnt1; xMaxEnd=x; }

        // координаты начала непрерывного фрагмента текста максимальной длинны
        xMaxStart=xMaxEnd-cnt1Max;
        
        // если фраза словаря не совпадает с проверяемымм текстом то прекращаем анализ словарной фразы 
        if ( xMaxEnd-xMaxStart < constantPhrase ) return 0; 
        
    }else { //  ( dlt == 1 ) // НЕ ЧЕТКИЙ поиск.
        
        // Не четкий поиск. Вычисление максимального не четкого фрагмента текста для текущей словарной фразы.        
        cnt0Max=0;  cnt1Max=0; 
        // цикл сдвига интервала поиска максимума количества букв (единиц) на единицу в распознаваемом тексте для текущей словарной фразы
        for(x=0; x < size_BufTxtD; x++) { // size_BufTxt-dlt;
            
            // поиск максимума количества единиц cnt1 в интервале соответствующиму количеству нулей равному dlt 
            cnt0=0, cnt1=0; cntDlt=0;
            for(int m=x; cnt0 < dlt1 && m < size_BufTxt; m++) { // dlt1=dlt+1 иначе не отрабатываются единичные дырки в не четком фрагменте текста
            ///for(m=x; cnt0 < dlt1; m++) {
                // счетчики нулей (отсутствие буквы в данной позиции в тексте) и единиц (наличие буквы) в интервале поиска
                if ( BufTxt[m] == 0 ) { cnt0++; }  else { cnt1++; }  // cnt1=m-x-cnt0;
                ///if ( BufTxt[m] == 0 ) { cnt0++; cntDlt++; }  else { cnt1++; cntDlt=0; }       
            } // m
            // нахождение максимума количества единиц cnt1 в распознаваемом тексте
            if ( cnt1 > cnt1Max ) { cntDltEnd=cntDlt; cnt1Max=cnt1; xMaxStart=x; }// > остаточный хвост нулей dltEnd в начале не четкого фрагмента текста
                                                                                  
///            cout<<x<<"."<<cnt1<<"  ";
            ///cout<<x<<"."<<cnt1<<"."<<cntDlt<<"  ";
            
        } // x
        // координаты конца нечеткого кусочно непрерывного фрагмента текста максимальной длинны
        xMaxEnd=xMaxStart + cnt1Max + dlt - 1; // +dlt-1  // -cnt0Max-2
        // координаты начала нечеткого кусочно непрерывного фрагмента текста максимальной длинны        
        xMaxStart--;        
        // если фраза словаря не совпадает с проверяемымм текстом то прекращаем анализ словарной фразы 
        if ( xMaxEnd-xMaxStart < constantPhrase ) return 0; 
             
    } //  else {  // окончание не четкого поиска. 
    
    ///cout<<endl<<"cnt0Max="<<cnt0Max<<"  cnt1Max="<<cnt1Max<<"  m-x="<<m-x<<"  xMaxStart="<<xMaxStart<<"  xMaxEnd="<<xMaxEnd<<endl;
    //cout<<endl<<"cntDltEnd="<<cntDltEnd<<"  cnt1Max="<<cnt1Max<<"  xMaxStart="<<xMaxStart<<"  xMaxEnd="<<xMaxEnd<<endl;
    //cout<<endl;   

//------------------------------------------------------------------

    /// функция синхронизации совпавших подстрок текста и словаря по непрерывному фрагменту текста ///
    // т.е. поиск начала и конца совпавшего фрагмента текста (xMaxStart и xMaxEnd) 
    // с фрагментом словаря (xMaxStartDict и xMaxEndDict) 
    
    // стартовая точка середины совпавшего фрагмента текста с фрагментом словаря 
    // середина между xMaxEnd и xMaxStart это это точка начала поиска блоком (unsigned long) в 4 подряд идущие буквы (short) 
    int  middleMatchText=(xMaxEnd + xMaxStart)/2-2; // сдвиг на -2 (половину unsigned long)
    int  middleMatchDict;
    unsigned long *test=(unsigned long*)(BufTxt+middleMatchText);
    xMaxStartDict=0;
    xMaxEndDict=0;
    
    if ( dlt == 0 ) { // ЧЕТКИЙ поиск.
        
        // цикл поиска блоком в 4 подряд идущие буквы (unsigned long)
        for(x=oldw;  x < w;  x++) { 
            /*wchar_t ds=BufUpT[dictionary_data[x]];
             wstring st; 
             if(ds==0){ds='-';} st+=ds;
             cout<<Unicode_to_UTF(st);
             //cout<<BufUpT[dictionary_data[x]]<<" ";
             */            
            if(*(unsigned long*)(dictionary_data+x)!=*test){  // проверка совпадения 4х подряд идущих букв 
                continue;
            }else{
                middleMatchDict=x;
                int x_=0;
                // проверка совпадения подряд идущих букв вправо. Цикл поиска по одной букве
                unsigned short ts=*(BufTxt + middleMatchText + x_);
                while(*(dictionary_data + x_ + middleMatchDict) == ts &&
                      BufUpT[dictionary_data[x_ + middleMatchDict]]> 0 && ts > 0
                      ){
                    x_++;
                    ts=*(BufTxt + middleMatchText + x_);
                }
                xMaxEnd=middleMatchText+x_;
                xMaxEndDict=middleMatchDict+x_-oldw;
                //cout<<" x_="<<x_<<" xMaxEndDict="<<xMaxEndDict<<endl;
                x_=0;
                // проверка совпадения подряд идущих букв влево. Цикл поиска по одной букве
                ts=*(BufTxt + middleMatchText + x_);
                while(middleMatchText + x_>0&&
                      *(dictionary_data + x_ + middleMatchDict) == ts &&
                      BufUpT[dictionary_data[x_ + middleMatchDict]] > 0 && ts > 0){
                    x_--;
                    ts=*(BufTxt + middleMatchText + x_);
                } // while
                xMaxStart=middleMatchText + x_+ 1; 
                xMaxStartDict=middleMatchDict + x_- oldw + 1;
                //cout<<" x_="<<x_<<" xMaxStartDict="<<xMaxStartDict<<endl;  
                break;
            } // if(*(unsigned long*)            
        } // for(x
        
        // если непрерывная фраза словаря короче минимальной длинны constantPhrase (8), то прекращаем анализ словарной фразы 
        if ( xMaxEndDict-xMaxStartDict < constantPhrase ) return 0; // с проверкой time=3.07342, если проверку заремачить то time=3.25472
    
    }else { //  ( dlt == 1 ) // НЕ ЧЕТКИЙ поиск.
            
       // Вычисление максимального непрерывного совпавшего фрагмента текущей словарной фразы. Еще не написано
            
    } //  else {  // окончание не четкого поиска.                    
 
    // скорректировали начало и конец совпавшего фрагмента текста (xMaxStart и xMaxEnd) 
    // вычислили за ново значение начала и конца фрагмента словаря (xMaxStartDict и xMaxEndDict) 
    
    
    //  вывод массава разделителей строк
    /*
    for(int x=0;x<dictData->size_BufDR-1;x++){        
        for(int y=dictData->BufDR[x];y<dictData->BufDR[x+1];y++){
            cout<<(short)*(dictData->BufD+y)<<" ";
        }cout<<endl;        
    }
    */
    
//------------------------------------------------------------------
    
    /// подсчет вероятности разделителя (пробела) распознаваемого текста по четкому поиску ///
    
    //cout<<"BufDR["<<n<<"]="<<BufDR[n]<<" size="<<BufDR[n+1]-BufDR[n]<<" LPhrase="<<w-oldw<<endl;
    //cout<<"xMaxStart="<<xMaxStart<<" xMaxStartDict="<<xMaxStartDict<<endl;
    //cout<<"xMaxEnd="<<xMaxEnd<<" xMaxEndDict="<<xMaxEndDict<<endl;
    
    int deltaDict=xMaxStart-xMaxStartDict-1; 
    // -1 синхронизирует словаь фраз и словарь пробелов, без нее разделители берутся из соседней строки
    int xMaxStartDict_=xMaxStartDict;
    int xMaxEndDict_=xMaxEndDict;
    unsigned int n_=n;  // n - переписали в локальную переменную nc
    
    
    /*wchar_t ds=BufUpT[dictionary_data[x]];
     wstring st; 
     if(ds==0){ds='-';} st+=ds;
     cout<<Unicode_to_UTF(st);
     //cout<<BufUpT[dictionary_data[x]]<<" ";
    */
    
    int start=0;
    int end=0;
 

    if ( dlt == 0 ) { // ЧЕТКИЙ поиск.
        
        // цикл выборки адресов разделителей из словаря разделителей      
        for(int x=BufDR[n_]; x < BufDR[n_+1]; x++){   
            int indexDict=(short)*(BufD + x);  //cout<<indexDict<<". ";
            //cout<<" "<<indexDict;
            if ( xMaxStartDict_ < indexDict && indexDict < xMaxEndDict_ ) { // && xMaxEndDict_- xMaxStartDict_> 6
                if(start==0) start=indexDict + deltaDict;
                end=indexDict + deltaDict;
                resultDelimeter[indexDict + deltaDict]++; 
                //cout<<indexDict<<"+ ";
            } // if   
        } // x
        //cout<<endl;
        // вывод на экран
        /*if(end-start){
            cout_<<" @@@@ -> ";
            for(int x=start; x < end; x++) { 
                string str;
                str=(wchar_t)resultText[x]; 
                str=Unicode_to_UTF(resultText[x]);
                cout_<<str;
                if(resultDelimeter[x])cout_<<"་";
            }
            cout_<<endl; 
        } // if(end-start) 
        */
    }else { //  ( dlt == 1 )
            
       /// подсчет вероятности разделителя (пробела) не четкому поиску. Не написан ///  
            
    } //  else {  // окончание не четкого поиска
    
//------------------------------------------------------------------   
    
    /// заполнение массива вероятности буквы проверяемого текста по четкому поиску ///

    
    int indexLetter=0; // индекс позиции буквы в тексте от xMaxStart до xMaxEnd и одновременно с xMaxStartDict_ до xMaxEndDict_ 
    // в совпавшем фрагменте текущей словарной фразы 
    unsigned short ds;
    unsigned int nLetterP_=nLetterP; // nLetter   nLetterP
    
    /// Двухмерный массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв) SumTxtP[x];
    // представлен одномерным массивом, где позиция буквы в тексте - это номер строки массива частоты встречаемости пар букв, а
    // адресом в строке является код пар букв (мб код буквы) содержимое - сколько раз данная пара букв была найдена 
    // во всех легитимных фрагментах фраз словаря.    
    // самая подходящая конструкция такого массива - графический w*h. 

    
    
    indexLetter=xMaxStart;  // индекс позиции буквы в тексте от xMaxStart
    // цикл заполнения массива частоты встречаемости пар букв
    for(int x=oldw + xMaxStartDict_;  x < oldw + xMaxEndDict_;  x++) {  
        indexLetter++;        
        ds=dictionary_data[x]; // упакованный код пары букв
        // нужен массив перекодировки; упакованный код пары букв dictionary_data[x] в упакованный код буквы. 
        // ds=BuffUUp[dictionary_data[x]] сейчас так  ds=BuffM[BufUp[dictionary_data[x]]];          
        // Зеркальный массив BuffM. Предназначен для получения упакованного кода из исходного кода буквы,
        /// адресом массива является исходный код буквы (разреженный код) - ответом упакованный код буквы.// BuffM не передан
        
        // сколько раз в словарной фразе встречалась пара букв с упакованным кодом ds на позиции в 
        // распознаваемом тексте indexLetter*nLetterP_
        SumTxtP[indexLetter*nLetterP_ + ds]++; 

    } // x
/**/ 
    




    
//------------------------------------------------------------------ 
    
    // заполнение массива суммарных весов пар букв по всем фразах словаря.
    // т.е. сколько пар букв из всех легитимных фраз словаря (в интервале от xMaxStart до xMaxEnd) попало на каждую позицию пар букв текста
    // просто  в интервале от xMaxStart до xMaxEnd к весам всех пар букв добавляем 1 т.к. совпадение букв уже проверено
    for(x=xMaxStart; x < xMaxEnd; x++) { GravTxt[x]++; } // +1 для полного совпадения с пред. версией     
/**/      
    
/*    
    // заполнение и вывод на экран массива суммарных весов пар букв текста во всех фразах словаря. предыд версия
    for(x=0; x < size_GravTxt; x++) { 
        //ds=BufTxt[x];
        if ( BufTxt[x] > 0 && xMaxStart < x && x < xMaxEnd ) { GravTxt[x]++;} // || //      
        ///if ( BufTxt[x] == 0 && xMaxStart < x && x < xMaxEnd ) { GravTxt[x]++; } // || // BufTxt[x] > 0  // BufUpT[BufTxt[x]]
        //cout<<x<<"."<<GravTxt[x]<<"  ";
///        cout<<"."<<GravTxt[x];
    } // x
*/ 
    
    
    return 1;
    

    
}//--------------------------------------------------------------------------------------------------------------------------------------------  

