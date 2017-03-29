#include "GMap.h"

//#define REMARK

#ifdef REMARK
    #define DD(x) cout<<x;
#else
    #define DD(x)
#endif
//#define DD(x) inputData.log<<x; inputData.log.flush();



uint GMap::lookupProcessFontMatch3(map<vector<int>,ulong>&searchResult,
                                             ulong w,
                                             ulong oldw,
                                             ulong start,
                                             ulong nLine
                                             ){
       
    //// функция поиска совпавшего непрерывного фрагмента фразы словаря длинной больше constantPhrase1 с
    //// непрерывным фрагментом текста. С учетом точных координатах совпавших букв текста .
    static uint coutStat=0;
    DD(coutStat<<" "); coutStat++;
    
    ulong x;           // ,s0,s1 // ,y,p // n,
    int bl,blp,cnt;          // ,bc,bm,counter
    mSIZE ds,m;          // ,bf
    uint LPhraseHits; // количество совпавших букв фразы Length Phrase Hits с буквами распознаваемого текста
    int indexX;                // адрес в массиве координат пар букв в тексте
    int x0=0,x0_=0,x1_=100000; // 100000 максимальная геометрическая ширина текста
    int constantPhrase1=constantPhrase-1;   //отсчет фраз начинается с нуля, а constantPhrase1 это количество пар
    int *BufDictLine=(int*)&BufDictStr[0];
#define D_(x) 
 
#ifdef REMARK
    // необходимы для вывода на экран
    int print=1;
    wstring str;
#endif
#ifdef REMARK

    //  Описание алгоритма:
    D_(" //******************lookupProcessFontMatch3****************"<<endl);
    
    for(ulong x=oldw;  x < w-1;  x++) {  // w_1=w-1;
        DD(p1letter(dictionary_data[x]));  //декодирует компактный код первой буквы пары в строку
    }
    DD(endl);
    
#endif
    // Функции lookupProcess3 учитывающею шрифтовую (поространственную) геометрию текста.
    // Для этого оставляем для дальнейшей обработки фразы словаря имеющие непрерывный фрагмент совпавший с непрерывным
    // фрагментом распознаваемого текста. По достижении пороговой длины constantPhrase1 подряд идущих букв текста
    // совпавших с непрерывным фрагментом распознаваемого текста, останавливаем подсчет длины и переходим к следующему
    // уточняющему алгоритму N4, который работает только с фразами словаря прошедшими предидущий этап N1,N2 и N3.
    // Таких фраз примерно примерно в три раза меньше чем количество фраз словаря поступающих на этот алгоритм.
    // Алгоритм дает выйгрыш по быстродейсдвию примерно в три раза.
    // Информация о точных координатах совпавших букв текста уже присутствует.
                
    // 0  -3  3  2  56  63  -2  2  8  17  -2  2  9  42  -2  2  10  43  -2  2  23  57  -2  2  24  71  -2  2  26  47  -2  2  38  44

    if(w-oldw>128)w=oldw+128;
    int flag=0;  //флаг начала цепочки
    long indexStart;
    int dl=11;  //новая пара должна начинатся отступив от края предыдущей
    
    //int count=0;
    //static int maxCount=0;
    int startClasterChain=0;
    
    for(x=oldw; x < w; x++) { // w-1;
      
      ds=dictionary_data[x];  // компактный код пары букв в словаре ]
#ifdef REMARK
      //str=(wchar_t)BufUp[ds]; cout<<endl<<Unicode_to_UTF(str);
      DD(p1letter(ds)); //декодирует компактный код пары букв в строке
#endif
      bl=letters_data[ds];    // координаты пар букв встечающихся в распознаваемом тексте один раз (без учета одинаковых).
      DD("/"<<bl<<"/"<<x-oldw<<"/"<<flag<<" indexX/2="<<indexX/2<<endl);
      if ( bl != 0 ) {
          
        // заполнение массива адресами (позициями) пар букв в распознаваемом тексте, встречающихся однократно (кодами пар букв по адресам)
        int flagNew=0;  //флаг соответствия новой пары
        if ( bl > 0 ) {
            indexX=(bl-1)*2;   //для извлечения двух short из массива координат indexX удваивается
            //первая пара цепочки встречается в строке один раз
            //проверяем одну цепочку, начиная с этой пары
            if(!flag){
                BufDictLine[flag]=indexX/2;  //записываем первую пару в цепочку 
                x0_=letterX[indexX];
                x1_=letterX[indexX+1];
                indexStart=x;
                DD("@1@@ start chain from letter "<<p1letter(ds)<<" flag="<<flag<<endl);
                flag++;
                //D_(" x0_="<<x0_<<" x1_="<<x1_<<endl);
                continue;                      //переходим ко второй паре
            }
#ifdef REMARK
            str=(wchar_t)BufUp[text_data[indexX/2]]; DD(" l="<<Unicode_to_UTF(str)<<endl);
#endif
            
            x0=letterX[indexX];
            //проверяем геометрию
            DD("          x0="<<x0<<" x0_="<<x0_<<" x1_="<<x1_);

            if(x0>x0_+dl&&x0<x1_){
               //пара подходит
                DD("[+]"<<" flag="<<flag<<endl);
                BufDictLine[flag]=indexX/2; 
                flag++;
                flagNew=1;
            }else{
              //пара не подходит
                DD("[-]"<<endl);
            }
            x0_=x0;
            x1_=letterX[indexX+1];
            
        } // ( bl > 0 )
           
        // заполнение массива адресами (позициями) пар букв в распознаваемом тексте, встречающихся более одного раза
        else {                                                 
            blp=-bl; // превращаем bl в положительнай адрес blp, т.к. число bl искуственно отрицательно
            
            //проверяем все пары кластера на возможность продолжения цепочки
            cnt=clusters_data[blp+1];   // считываем значение счетчика адресов,  +1 положение счетчика свободных адресов
            // считывание первого, второго, третьего и т.д. адресов в цепочке ячеек кластера
            for(m=0; m < cnt; m++) {
                indexX=(clusters_data[blp+2+m])*2;   // bc аналог x, только извлеченный из массива цепочек адресов clusters_data[x]
#ifdef REMARK 
                str=(wchar_t)BufUp[text_data[indexX/2]];
                DD(" c"<<startClasterChain<<"="<<Unicode_to_UTF(str)<<" cnt="<<cnt<<" startClasterChain="<<startClasterChain);
                DD("          x0="<<letterX[indexX]<<" x0_="<<x0_<<" x1_="<<x1_<<" flag="<<flag<<endl);
#endif
                
                if(!flag){
                    //в случае если первая буква в цепочке это буква кластера,
                    //необходимо построить все цепочки начиная с каждой буквы кластера
                    //и выбрать из них самую длинную
                    //начинаем с первой буквы кластера.
                    if(m<startClasterChain)continue;
                    BufDictLine[flag]=indexX/2;
                    x0_=letterX[indexX];
                    x1_=letterX[indexX+1];
                    indexStart=x;
#ifdef REMARK
                    str=(wchar_t)BufUp[text_data[indexX/2]]; DD(" c"<<startClasterChain<<"="<<Unicode_to_UTF(str)<<" ");
#endif
                    DD("@@"<<startClasterChain<<"@@"<<m<<" start chain from letter "<<Unicode_to_UTF(str)<<" ");
                    DD(" x0_="<<x0_<<" x1_="<<x1_<<" flag="<<flag<<endl);
                    flag++;
                    startClasterChain++;
                    goto exitFor;                      //переходим ко второй паре
                }
                
                x0=letterX[indexX];
                //проверяем геометрию
               
                DD("          x0="<<x0<<" x0_="<<x0_<<" x1_="<<x1_);
                if(x0>x0_+dl&&x0<x1_) {
                    //пара подходит
                    DD("[+]"<<" flag="<<flag<<" indexX="<<indexX/2<<endl);
                    BufDictLine[flag]=indexX/2; 
                     flag++;
                    x0_=x0;
                    x1_=letterX[indexX+1];
                    flagNew=1;
                    break;
                }else{
                    //пара не подходит
                    DD("[-]"<<endl);
                }
                
            } // for(m=0;

        } // else {
          
        if(!flagNew){  //пара не найдена
              if(flag>=constantPhrase1){
                  LPhraseHits=flag;
                  break;
              }
              //начинаем новую цепочку с точки обрыва
              //возвращаемся на пару с которой начиналась цепочка и начинаем новую цепочку
              //со следующей буквы
            
              //если первая буква цепочки была буквой кластера и мы перебрали все буквы кластера,
              //обнуляем счетчик кластера и берем следующую букву
              DD(" startClasterChain="<<startClasterChain<<" flag="<<flag<<endl);
              if(!flag&&startClasterChain){startClasterChain=0;indexStart++;}
            
              DD("Найдено "<<flag-1<<" пар. Новая цепочка с "<<indexStart-oldw+1<<" пары"<<endl);

              flag=0;
              flagNew=0;
              x=indexStart;
        }
          
      }else{
              //в процессе сборки цепочки пара не найдена в тексте.
              //обрываем цепочку, обнуляем счетчики цепочек
              if(flag>=constantPhrase1){

                      //если полученная цепочка достаточной длинны, заносим ее в массив результатов

                      //cout<<" x="<<x<<" w="<<w<<" oldw="<<oldw<<" start="<<start<<" nLine="<<nLine<<endl;
                      //for(int t=start;  t < w;  t++) cout<<p1letter(dictionary_data[t]);  //декодирует компактный код первой буквы пары в строку
                      //cout<<endl;
                      //cout<<"x="<<x<<" flag="<<flag<<" shift="<<x-start-flag<<endl;
                  

                      ulong shift=x-start-flag;         //смещение начала найденной фразы оносительно начала словарной фразы
                      ulong indexXD=BufDR[nLine];       //адрес начала стороки разделителей
                      ulong indexXD_=BufDR[nLine+1];    //адрес конца стороки разделителей
                      ulong indexDict;                  //значение адреса разделителя в строке словаря
                      ulong indexPoint;                 //адрес разделителя в найденной строке ответа
                  
                      //в результате нужно учесть что последняя пара состоит из двух букв
                      //на этом этапе записываем есть ли в конце пары разделитель слогов (точка)
                      //если после пары есть точка, то пару записываем как отрицательное число
                  
                      // цикл выборки адресов разделителей из словаря разделителей
                      //формат строки разделителей  1 3 6 8 10 11 12 15 17 19 22 23 26 28 31 32 34 35 36 37 39
                      //где каждый адрес соответствует позиции буквы фразы после которой есть разделитель
                  
                      for(ulong t=indexXD;  t <indexXD_;  t++) { // цикл выборки адресов разделителей из словаря разделителей   
                          indexDict=(short)*(BufD + t);
                          if(indexDict>shift){
                            indexPoint=indexDict-shift-1;
                            if(indexPoint>flag)break;
                            //cout<<" "<<indexDict<<" sh="<<indexPoint+1;
                            BufDictLine[indexPoint]*=-1;  //знаком записываем наличие разделителя в паре
                          }
                      }
                      DD("flag="<<flag<<" indexStart="<<indexStart<<endl);
                      
#ifdef REMARK
                      wstring str;
                      static int count_=0;
                      print=1;
                      //if(count_==3)print=1;
                      count_++;
                      if(print){
                          DD("flag="<<flag<<" indexStart="<<indexStart<<endl)
                          
                          for(int x=indexStart;  x < indexStart+flag+1;  x++) {  // w_1=w-1;
                              str=(wchar_t)BufUp[dictionary_data[x]]; DD(Unicode_to_UTF(str));
                          }
                          ostringstream out;
                          for(int x=0;  x < flag;  x++) {  // w_1=w-1;
                              out<<BufDictLine[x]<<" ";
                          }
                          DD("  ");
                          out<<endl;
                          DD(out.str());
                      }
#endif
                      BufDictLine[flag]=32767;  //маркируем конец строки
                      searchResult[BufDictStr]++;   //в searchResult формируются уникальные ответы словаря с указанием количества ответов
                      memset(BufDictLine,0,256); //обнуляем цепочку
                      //if(print)exit(0);

              }
              //начинаем новую цепочку со следующей пары в тексте
          
              startClasterChain=0;
              DD("chain break; startClasterChain1="<<startClasterChain<<" count="<<endl);
              flag=0;

      } // if ( bl != 0 )
      exitFor:;

    } // x<<
     DD(" mem clusters_size="<<clusters_size<<" clusters_data_vector="<<clusters_data_vector->size()<<"clusters_copy="<<clusters_copy_vector->size()<<endl);
    // восстановления массива clusters_data (счетчиков адресов цепочек) из массива clusters_copy, после обращения к массиву clusters_data
    memcpy(clusters_data, clusters_copy, clusters_size*4); 
    DD("exit lookup"<<endl);
    
    
    return flag;
    

}//--------------------------------------------------------------------------------------------------------------------------------------------  
