#include "GLogicProcessor.h"
//полученные в результате подготовки в  renderDictSearch части фразы собираем в целые фразы по следующему алгоритму:
//1. Находим самую длинную фразу. Внутри длинной ищем фразы начинающиеся правее ее геометрического начала, начиная с фразы с наибольшей корреляцией
//2.Найденные фразы объединяем с исходной фразой по следующему правилу:
//  2.1. Если стыкуемая фраза совпадает по-буквенно с основной фразой, объединяем две фразы по-буквенно.
//       Расчитываем корреляцию результирующей фразы  как корреляцию фразы с наибольшей корреляцией
//       При стыковке учитываем длины двух фраз стыкуемых непосредственно, длина общей цепочки не учитывается
//  2.2. Если стыкуемые фразы не совпадают по-буквенно. Отдаем приоритет тем буквам, которые находятся в середине фразы.
//       Таким образом не учитываем первую и две последние буквы фразы. В случае расхождения в этих трех буквах
//       приоритет у букв в середине фразы.
//  2.3. Если не совпадают буквы в середине фразы. В этом случае приоритет у букв, находящихся в середине более длинной фразы.
//       При стыковке учитываем длины двух фраз стыкуемых непосредственно, длина общей цепочки не учитывается
//  2.4  Если не найдено подходящей стыкуемой фразы или достигнута граница текста, продолжаем разбор влево от основной фразы

void GLogicProcessor::sentenceConstructur(vector<OCRMatch>&wLine){

    //drawGrapeLine(wLine); exit(0);
    
    int print=0;

    while(1){
        //находим самую вероятную фразу (вероятность учитывает также и количество букв)
        DR(" wLine.size()="<<wLine.size()<<endl)
        int size=(int)wLine.size();
        int index;
        float maxP=0;
        int maxIndex=-1;
        int flagP=0;
        for(int i=0;i<size;i++){
            if(!wLine[i].correlation)continue;
            if(wLine[i].status)continue;
            if(wLine[i].correlation>maxP){
                maxP=wLine[i].correlation;
                maxIndex=i;
            }
        }
        if(maxIndex==-1){
            DR(" разбор строки закончен"<<endl)
            break;
        }
        DR(" сначала разбираем фразы справа от найденной фразы  "<<wLine[maxIndex].name<<" ")
        DR(" c="<<wLine[maxIndex].correlation<<" maxIndex="<<maxIndex<<endl)
        int countFlag=0;     //флаг указывает направление разбора фразы - сначала направо, затем налево
        while(1){
            
            float maxP=0; int maxLIndex=-1;
            
            if(!countFlag){
                index=maxIndex+1;
                DR("находим максимальную по вероятности стыкуемую фразу справа от основной фразы"<<endl)
                while(index<size){ //пока фразы имеют общую часть
                    //DR("index="<<index<<" c="<<wLine[index].correlation<<" wLine[index].x0="<<wLine[index].x0<<" wLine[maxIndex].x0="<<wLine[maxIndex].x0<<
                    //" wLine[index].x1="<<wLine[index].x1<<" wLine[maxIndex].x1="<<wLine[maxIndex].x1<<
                    //" wLine[index].xL0C="<<wLine[index].xL0C<<" wLine[maxIndex].xL0C="<<wLine[maxIndex].xL0C<<
                    //" wLine[index].xL1C="<<wLine[index].xL1C<<" wLine[maxIndex].xL1C="<<wLine[maxIndex].xL1C<< " n="<<wLine[index].name<<endl);
                    if(wLine[index].correlation&&
                       wLine[index].xL0C<wLine[maxIndex].x1&&
                       wLine[index].xL1C>wLine[maxIndex].x1){
                        //выбираем фразу с максимальным количеством букв и корреляцией
                        if(wLine[index].correlation>maxP){ 
                            maxP=wLine[index].correlation;
                            maxLIndex=index;
                        }
                    }    
                    index++;
                }
            }else{
                index=maxIndex-1;
                if(index<0)index=0;
                DR("находим максимальную по вероятности стыкуемую фразу слева от основной фразы"<<endl)
                while(index>-1){
                    DR("index="<<index<<" c="<<wLine[index].correlation<<" wLine[index].x0="<<wLine[index].x0<<" x1="<<wLine[index].x1<<" n="<<wLine[index].name<<" //wLine[maxIndex].x0="<<wLine[maxIndex].x0<<" x1="<<wLine[maxIndex].x1<<
                    " wLine[index].xL0C="<<wLine[index].xL0C<<" wLine[index].xL1C="<<wLine[index].xL1C<<endl);
                    if(wLine[index].correlation&&
                       wLine[index].xL1C>wLine[maxIndex].x0&&
                       wLine[maxIndex].x0>wLine[index].xL0C
                       ){
                       // DR("+"<<endl)
                        if(wLine[index].correlation>maxP){
                            maxP=wLine[index].correlation;
                            maxLIndex=index;
                            //DR("+1 maxLIndex="<<maxLIndex<<endl)
                        }
                        
                    }
                    index--;
                }
            }
            if(maxLIndex<0){
                if(countFlag==0){   //закончен разбор фразы справа
                    countFlag++;
                    continue;
                }else{
                    DR("слева не найдено стыкуемых фраз"<<endl)
                    break; //разбор фразы закончен
                }
            }
            DR(" maxLIndex="<<maxLIndex<<" n="<<wLine[maxLIndex].name<<endl)
            int bufIndex=0;
            if(countFlag){
                DR(" меняем местами фразы, чтобы сохранить направление стыковки"<<endl)
                bufIndex=maxIndex;
                maxIndex=maxLIndex;
                maxLIndex=bufIndex;
            }
            
            DR(" n0="<<wLine[maxIndex].name<<" n1="<<wLine[maxLIndex].name<<endl)
           
            if(index<0)index=0;
            
            //для полученной пары применяем правила сложения фраз
            //находим совпадает ли первая буква стыкуемой фразы
            int flag=0;
            for(int i=0;i<wLine[maxIndex].letter.size();i++){
                //DR(" N"<<i<<"="<<wLine[maxIndex].letter[i].name<<endl)
                if(wLine[maxIndex].letter[i].x0==wLine[maxLIndex].letter[0].x0&&
                   wLine[maxIndex].letter[i].name==wLine[maxLIndex].letter[0].name){
                    DR("MATCH"<<" N0="<<wLine[maxIndex].letter[i].name<<endl)
                    flag=1;
                    //найдено совпадение первой буквы.
                    //начиная с этой буквы начинаем прибавлять пары стыкуемой фразы, проверяя на совпадение.
                    for(int n=0;n<wLine[maxLIndex].letter.size();n++){
                        if(i+n>wLine[maxIndex].letter.size()-1){
                            DR(" end main phrase"<<endl)
                            DR(Unicode_to_UTF(wLine[maxLIndex].letter[n].wName[0]))
                            wLine[maxIndex].letter.push_back(wLine[maxLIndex].letter[n]);
                            wLine[maxIndex].line.push_back(wLine[maxLIndex].line[n]);
                            wLine[maxIndex].wName+=wLine[maxLIndex].letter[n].wName[0];
                        }else{
                            if(wLine[maxIndex].letter[i+n].name==wLine[maxLIndex].letter[n].name){
                                //пары букв совпадают, продолжаем разбор
                                DR("MATCH"<<" n"<<n<<"="<<wLine[maxLIndex].letter[n].name<<endl)
                            }else{
                                //пары букв не совпали
                                DR("BREAK"<<" n"<<n<<"="<<wLine[maxLIndex].letter[n].name<<endl)
                                //проверяем на какой позиции находится спорная буква
                                if(n>wLine[maxLIndex].letter.size()-2){
                                    DR(" //оставляем пару букв основной фразы"<<endl)
                                }else if(n+i>wLine[maxIndex].letter.size()-2){
                                    DR(" //оставляем пару букв стыкуемой фразы"<<endl)
                                    wLine[maxIndex].letter[n+i]=wLine[maxLIndex].letter[n];
                                    wLine[maxIndex].wName[n+i]=wLine[maxLIndex].letter[n].wName[0];
                                    wLine[maxIndex].wName[n+i+1]=wLine[maxLIndex].letter[n+1].wName[0];
                                }else{
                                    DR(" //обе буквы находятся в середине фразы"<<endl)
                                    DR(" //оставляем букву более длинной фразы"<<endl)
                                    if(wLine[index].letterW>wLine[maxIndex].letterW){
                                        DR(" //заменяем букву основной фразы"<<endl)
                                        wLine[maxIndex].letter[n+i]=wLine[maxLIndex].letter[n];
                                        wLine[maxIndex].wName[n+i]=wLine[maxLIndex].letter[n].wName[0];
                                    }
                                }
                            }
                        }
                        
                    }
                    wLine[maxIndex].name=Unicode_to_UTF(wLine[maxIndex].wName);
                    wLine[maxIndex].name=YagpoToUni(wLine[maxIndex].name);
                    wLine[maxIndex].status=1;
                    wLine[maxIndex].x1=wLine[maxLIndex].x1;
                    wLine[maxIndex].correlation+=(int)wLine[maxLIndex].letter.size()+15;
                    wLine[maxLIndex].correlation=0;
                    
                    flagP=1; 
                    DR("EXIT FULL MATCH3 name "<<wLine[maxIndex].name<<" wName="<<Unicode_to_UTF(wLine[maxIndex].wName)<<endl)
                    //drawGrapeLine(wLine); exit(0);
                    break; //фраза состыкована, прекращаем разбор, стыкуем следующую фразу
                }//if(wLine[maxIndex].letter[i].x0=
                
                if(flag)break;
            }//for(int i=0;i<wLine[maxIndex].letter.size()
            if(!flag){
                DR("no match in last letter countFlag="<<countFlag<<endl)
                if(!flag){
                    DR("no match in last letter countFlag="<<countFlag<<endl)
                    DR("проверяем можно ли состыковать фразы по предпоследней и второй букве"<<endl)
                    if(abs(wLine[maxIndex].letter[wLine[maxIndex].letter.size()-1].x0-wLine[maxLIndex].x0)<10){
                        DR("MATCH в расположении предпоследней и второй буквы"<<endl)
                        int lInd=(int)wLine[maxIndex].letter.size()-1;
                        if(wLine[maxIndex].letter[lInd].correlation<wLine[maxLIndex].letter[0].correlation-1){
                            DR("приоритет корреляции присоединяемой буквы"<<endl)                            
                            wLine[maxIndex].letter[lInd]=wLine[maxLIndex].letter[0];
                            wLine[maxIndex].line[lInd]=wLine[maxLIndex].line[0];
                            wLine[maxIndex].wName[lInd]=wLine[maxLIndex].wName[0];
                            wLine[maxIndex].wName[lInd+1]=wLine[maxLIndex].wName[1];
                            
                            if(wLine[maxLIndex].letter.size()>1){
                                for(int n=1;n<wLine[maxLIndex].letter.size();n++){
                                    DR(Unicode_to_UTF(wLine[maxLIndex].letter[n].wName[0]))
                                    wLine[maxIndex].letter.push_back(wLine[maxLIndex].letter[n]);
                                    wLine[maxIndex].line.push_back(wLine[maxLIndex].line[n]);
                                    wLine[maxIndex].wName+=wLine[maxLIndex].letter[n].wName[0];
                                }
                            }
                            wLine[maxIndex].name=Unicode_to_UTF(wLine[maxIndex].wName);
                            wLine[maxIndex].name=YagpoToUni(wLine[maxIndex].name);
                            wLine[maxIndex].x1=wLine[maxLIndex].x1;
                            wLine[maxIndex].correlation+=(int)wLine[maxLIndex].letter.size()+15;
                            wLine[maxLIndex].correlation=0; //стираем пристыкованную фразу
                            flagP=1;
                            DR("FULL MATCH1 name "<<wLine[maxIndex].name<<"/"<<wLine[maxIndex].letter.size()<<" xL1C="<<wLine[maxLIndex].xL1C<<endl)
                            continue;
                        }else{
                            DR("приоритет корреляции основной буквы"<<endl) 
                        }
                    }else{DR("no match"<<endl)}
                }
                if(!countFlag){   //учитываем прямой и обратный порядок стыковки
                    wLine[maxLIndex].correlation=0;
                    flagP=1;  
                }else{
                    DR("фраза не состыкована, возвращаем приоритет стыковки основной фразе bufIndex="<<bufIndex)
                    wLine[maxIndex].correlation=0;
                    maxIndex=bufIndex;
                    DR(wLine[maxIndex].name<<" maxIndex="<<maxIndex<<endl)
                    flagP=0; 
                }
            }
        }
        //если фраза полностью состыкована, убираем фразы внутри стыкованной фразы
        DR("flagP="<<flagP<<" n="<<wLine[maxIndex].name<<" c="<<wLine[maxIndex].correlation<<endl)
        if(flagP){  
            for(int n=0;n<wLine.size();n++){
                if(n==maxIndex)continue;
                if(wLine[n].xCenter>wLine[maxIndex].x0-24&&wLine[n].xCenter<wLine[maxIndex].x1+24)wLine[n].correlation=0;
            }
            
        }else{
            //оставляем пару или фразу с наибольшей корелляцией
            for(int n=0;n<wLine.size();n++){
                if(n==maxIndex)continue;
                if(wLine[n].xCenter>wLine[maxIndex].x0-20&&wLine[n].xCenter<wLine[maxIndex].x1+20){
                    if(wLine[n].correlation<wLine[maxIndex].correlation){
                        wLine[n].correlation=0;
                    }else{
                        //cout<<"wLine[n].correlation="<<wLine[n].correlation<<" <wLine[maxIndex].correlation="<<wLine[maxIndex].correlation<<endl;
                        wLine[maxIndex].correlation=0;
                        break;
                    }    
                }    
            }
            
        }   
        wLine[maxIndex].status=1;   ///отмечаем как разобранную
        //print++;
        //if(print>3){drawGrapeLine(wLine); exit(0);}
    }

//drawGrapeLine(wLine); exit(0);

    
}


