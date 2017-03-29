#include "GLogicProcessor.h"
#include "GMemory.h"
#include "php2stl.h"

void GLogicProcessor::renderDictSearchSanskrit(map<vector<int>,ulong>&searchResult,
                                          vector<OCRMatch>&dLine,
                                          GBitmap *lineImg32,
                                          GBitmap* letterAImg,
                                          GBitmap* letterBImg){
    
    int print=0;
    
    //проверяем ответы словаря на максимальную корреляцию, выбираем наилучшие гипотезы
    sort(dLine.begin(),dLine.end(),sortMatchW);
    //drawGrapeLine(dLine); exit(0);
    
    
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        dLine[n].line.push_back(n);
        dLine[n].dIndex.push_back(2);
    }
    
    testStackLetterNew(dLine,dLine,lineImg32,letterAImg,letterBImg);
    //drawGrapeLine(dLine); exit(0);
    
    print=0;
    //убираем фразы и буквы внутри стыкованной фразы
    sort(dLine.begin(),dLine.end(),sortMatchX0);
    
    int limit;
    for(int n=0;n<dLine.size();n++){
        if(!dLine[n].correlation)continue;
        if(dLine[n].OCRIndex=='Z')continue;
        
        //print=0;if(n==13)print=1; if(!print)continue;
        DR("@@@@"<<n<<" Collect n="<<dLine[n].name<<" d="<<dLine[n].delimeter<<endl)
        
        for(int m=0;m<dLine.size();m++){
            if(!dLine[m].correlation)continue;
            if(dLine[m].OCRIndex=='Z')continue;
            //print=0;if(m==16)print=1;
            if(m==n)continue;
            if(dLine[m].xCenter>dLine[n].x0&&dLine[m].xCenter<dLine[n].x1){
                DR("n"<<n<<"="<<dLine[n].name<<" d="<<dLine[n].delimeter<<" c="<<dLine[n].correlation<<" m"<<m<<"="<<dLine[m].name
                   <<" c="<<dLine[m].correlation<<" xmC="<<dLine[m].xCenter<<" xnC="<<dLine[n].xCenter<<" xnX0="<<dLine[n].x0<<" xnX1="<<dLine[n].x1<<" wM="<<dLine[m].letterW<<" wN="<<dLine[n].letterW<<endl);
                
                if(dLine[n].letterW>dLine[m].letterW){
                    dLine[m].correlation=0;
                }else{
                    dLine[n].correlation=0;
                            break;
                }
            }
        }
    }

    
}//-------------------------------------------------------------------------------------------------------------
/*
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
 */
