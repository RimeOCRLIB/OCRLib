#include "GLogicProcessor.h"
#include "../GImageEditor/GImageEditor.h"

/*
 Основная функция графического распознавания ксиллографического текста. К выделенной строке применяется поиск масштаба, графическое распознавание букв, применение правил шрифтовой грамматики. Результатом функции является массив pageText котрый содержит гипотезы букв для грамматического корректора.
 
 1. Первый этап. Каждая буква базы записывается как результат корреляции всех букв базы с соответствующими частями буквы.
 Например буква А содержит части похожие на Л; М; И;  Корреляции соответствующих частей буквы записываются в формате
 структуры фиксированной длинны maskData_, в которую записывается указан номер признака, его геометрическое положение, вероятность корреляции с соответствующей буквой(частоста встречаемости и величина корреляции), юникод буквы. Юникод буквы кодируется как индекс буквы в базе (letterIndex).
 Таким образом каждая буква базы описана совокупностью признаков взаимной корреляции со всеми буквами базы.
 Все результаты корреляции заносятся в массив letterDataLine, и сортируются по координате X
 Массивы letterDataLine для одной буквы, создается, записывается в базу данных шрифта и заполняется в функции setMaskFromBase();
 Массивы letterDataLine для каждой буквы, создаются, записываются в базу данных шрифта и заполняются в функции setBaseCorrelation();
 
 2. Второй этап. При распознавании строки распознается каждый признак эталонной буквы базы. Распознавание начинается при срабатывании первого признака с вероятностью более 40%. Для каждого признака определяется величина корреляции и перимущественное геометрическое положениие в достаточно широких пределах поиска  - 2/3 строки.
 Результаты корреляции признаков заносятся в массив корреляциий признаков - matchMaskLine. Результаты заносятся в массив в виде структуры фиксированной длинны maskData_. Результаты matchMaskLine можно записывать и читать с диска для оптимизации скорости отладки.
 После занесения всех результатов в массив, массив сортируется по координате X для всех maskData_.
 Получившийся сортированный matchMaskLine поступает на ассоциативный поиск.
 
 3. Третий этап. Ассоциативный поиск в matchMaskLine совокупности признаков взаимной корреляции буквы базы.
 В получившийся в результате второго этапа распознавания совокупности признаков букв текста
 существуют характерные последовательности признаков. Например если мы в компктном месте строки, по размеру, соответстующему
 ширине буквы, нашли признаки букв А; Л; М; И; на соответстующих геометрических позициях и велицинах корреляции, то
 вероятность что в этом месте находится буква А значительно повышается, даже если кореляция А в этом месте первоначально была невысокая. Это позволяет укрепить гипотезу буквы за счет взаимной корреляции всех букв базы.
 Подсчет корреляции производится следующим образом.
 Находим в строке букву с наибольшим габаритом. Рассматриваем эту букву как тестовую. 
 Для всех букв внитри габарита высчитывем разницу между фактической корреляцией
 и корреляцией этой буквы и буквы с тестовой буквой. Эта корреляция определена заранее в результате тестирования базы.
 Если разница минимальна, это значит, что гипотеза тестовой буквы поддтверждается.
 Для подсчета используем крадратичную сумму, извлекая корень из суммы крадратов разности корреляций.
 

 Это гипотеза быстрого поиска (в разработке)
 Поизводится поиск последовательностей признаков букв в matchMaskLine соответствующих letterDataLine каждой буквы базы.
 Этот поиск проходит в три этапа.
 3.1 Первый этап поиска проводится по последовательности maskCode каждого признака в matchMaskLine. Определяются цепочки признаков, соответствующих letterDataLine каждой буквы базы  - matchChain.
 3.2 Цепочки признаков matchChain сортируются на основании геометрического совпадения и вероятностей записанных в letterDataLine буквы соответствующей matchChain.
*/ 

//новая версия грамматического анализатора.
//текст реконструируется на основе вероятностного анализа результатов распознавания
//с применением взаимной корреляции частей разных букв. Слоги и слова реконструируются
//на основе вероятносного анализа корпуса тибетских текстов и правил построения шрифта.
void GLogicProcessor::classification(vector<stringOCR>&strArray,
                                      vector<OCRMatch>&matchLine,
                                      GBitmap* lineImg32,
                                      string &mainString,
                                      int sizeLine,
                                      int lineIndex){

    int print=0;
    vector<OCRMatch>dLine;
    //TIME_START
    y0Base=strArray[lineIndex].LimY0;
    y1Base=strArray[lineIndex].LimY1;
    DR("@@@y0Base"<<y0Base<<" y1Base="<<y1Base<<" s="<<matchLine.size())
    
    GBitmap *letterAImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);
    GBitmap *letterBImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);

    
    
#ifdef MAIN_MODE
    
    //for(int i=0;i<matchLine.size();i++)if(matchLine[i].letterIndex==15650){cout<<"@@@@@";exit(0);}
    
    //drawGrapeLine(matchLine); exit(0);
    
    
    sort(matchLine.begin(),matchLine.end(),sortMatchX0);
    
    //cout<<"strArray.size()="<<strArray[0].size()<<" matchLine.size()="<<matchLine.size()<<endl;
    
    if(!matchLine.size()||!strArray.size())return;   //
    
    for(int i=0;i<matchLine.size();i++){
        matchLine[i].correlationNew=0;
        if(!matchLine[i].correlation)continue;
        matchLine[i].status=0;
    }
    //drawGrapeLine(matchLine); exit(0);

    compressMatch(matchLine);
    //drawGrapeLine(matchLine); exit(0);


    //на этом этапе в matchLine записано около 50 результатов на одну букву текста
    //заменяем одинаковые буквы на букву с наибольшей корреляцией и наибольшей общей площадью совпадающей с изображением на странице.
    //также для каждой гипотезы распознанной буквы проверяем перекрытие с соседними буквами
    //оставляем только те буквы, которые лучше описывают соответствующие площади буквы области изображения.
    letterNeighborsNew(matchLine,lineImg32,letterAImg,letterBImg);

    //drawGrapeLine(matchLine); exit(0);
    
    
#ifdef STACK_MODE    
    //saveMatch(matchLine,"/2_2.match");
#endif
    

    for(int i=0;i<matchLine.size();i++){
        if(matchLine[i].correlation){
          matchLine[i].status=0;
          if(matchLine[i].OCRIndex!=3&&matchLine[i].correlationNew)matchLine[i].correlation=matchLine[i].correlationNew;
          dLine.push_back(matchLine[i]);
        }
    }
    
    DR("done match processing line.size()="<<dLine.size())
    sort(dLine.begin(),dLine.end(),sortMatchX0);
    DR("@@@@@ SAVE MATCH")
#ifdef STACK_MODE    
    //saveMatch(dLine,"/2.match");
#endif    
#endif
    
#ifdef STACK_MODE    
    //readMatch(dLine,"/2.match");
#endif    
    
    //компрессия. Все одинаковые буквы в пределах габарита буквы
    //заменяются на одну букву с макcмимальной корреляцией
    compressMatch(dLine);

    //drawGrapeLine(dLine); exit(0);

    collectStackLetter(strArray,dLine, matchLine,lineImg32,letterAImg,letterBImg,lineIndex);
    //cout<<"COLLECT"; TIME_PRINT_
     //exit(0);
    //drawGrapeLine(dLine);exit(0);

    compressMatch(dLine);
    //drawGrapeLine(dLine);exit(0);
    //анализ готовых стеков.
    //проверяем есть ли над или под одиночной буквой коренные буквы или огласовки с высокой корреляцией.
    //если есть, то букву считаем частью стека и убираем как строительный блок OpenType.
    //testStackLetter(dLine,lineImg32,letterAImg,letterBImg);
    
    //drawGrapeLine(dLine); exit(0);
    
    letterAImg->destroy();
    letterBImg->destroy();
    
    string strW;
    vector<uint>letterX;
    vector<OCRMatch>line;
    vector<OCRMatch>resultLine;
    map<vector<int>,ulong>searchResult;
    
    buildSearchString(dLine,line,letterX,strW);
    cout<<strW<<endl<<endl;
    //drawGrapeLine(line); exit(0);
    textCorpusGMap->getOCRStackKey(strW,letterX,searchResult, ANY_MATCH);
    //inputData.log<<" 2"<<endl;inputData.log.flush();
    for (int i=0;i<line.size();i++){
        if(!line[i].correlation)continue;                        //cout<<line[i].name;
        line[i].name=Unicode_to_UTF(line[i].wName);              //cout<<" -- "<<line[i].name;
        line[i].name=YagpoToUni(line[i].name);   //cout<<" -- "<<line[i].name<<endl;
        resultLine.push_back(line[i]);
    }
    //logicProcessor->drawGrapeLine(line); //exit(0);
    //inputData.log<<" 3"<<endl;inputData.log.flush();
    //renderDictSearch(searchResult,line,resultLine,matchLine);   //mainString+="<br>"+lineString+"original<br>";
    //if(print){ cout<<"RENDER "; TIME_PRINT_ }
    strArray[lineIndex].line=dLine;   //сохраняем для вывода в HTML
    //logicProcessor->drawGrapeLine(dLine); //exit(0);
    //inputData.log<<" 4"<<endl;inputData.log.flush();

}
