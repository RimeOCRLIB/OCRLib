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
//на основе вероятносного анализа корпуса тибетских текстов и правил построения тибетского шрифта.
void GLogicProcessor::letterAssociation(vector<stringOCR>&strArray,
                                      vector<OCRMatch>&matchLine,
                                      vector<OCRMatch>&dLine,
                                      GBitmap* lineImg32,
                                      GBitmap* letterAImg,
                                      GBitmap* letterBImg,
                                      string &mainString,
                                      int sizeLine,
                                      int lineIndex){

    
    int print=0;
    int OCRMode=inputData.OCRMode;
    //TIME_START
    y0Base=strArray[lineIndex].LimY0;
    y1Base=strArray[lineIndex].LimY1;
    DR("@@@y0Base"<<y0Base<<" y1Base="<<y1Base<<" s="<<matchLine.size())
    
    //for(int i=0;i<matchLine.size();i++)if(matchLine[i].letterIndex==15650){cout<<"@@@@@";exit(0);}
    
    //drawGrapeLine(matchLine); exit(0);
    if(!matchLine.size()||!strArray.size())return;   //
 
    //для каждой буквы определяем количесво точек фокальных линий текста, совпадающих с областью ON всех масок буквы.
    //таким образом определем настолько область ON этой буквы совпадает с текстом.
    //OCRBox s;
    for (int i=0;i<matchLine.size();i++){
        if(!matchLine[i].correlation)continue;
        letterAImg->fillColumns32V(0, &matchLine[i].s);  //стираем букву
        matchLine[i].drawPict32(letterAImg,0,0,XOR_MODE);
        matchLine[i].pCount=lineImg32->img32UnionLine(letterAImg, &matchLine[i].s);
        ///if(i==518)
            //cout<<i<<" "<< matchLine[i].name<<" pCount="<<matchLine[i].pCount<<" c:"<<matchLine[i].correlation<<endl;
    }
    
    compressMatch(matchLine);
    //drawGrapeLine(matchLine); exit(0);
    
    
    letterConstruction(matchLine,OCRMode);
    
    //drawGrapeLine(matchLine); exit(0);

//#ifndef OCR_woodblock
    
    //на этом этапе в matchLine записано около 50 результатов на одну букву текста
    //заменяем одинаковые буквы на букву с наибольшей корреляцией и наибольшей общей площадью совпадающей с изображением на странице.
    //также для каждой гипотезы распознанной буквы проверяем перекрытие с соседними буквами
    //оставляем только те буквы, которые лучше описывают соответствующие площади буквы области изображения.
//    letterNeighborsNew(matchLine,lineImg32,letterAImg,letterBImg);
//#else
    if(OCRMode!=2){
        letterNeighborsNew(matchLine,lineImg32,letterAImg,letterBImg);
    }else{
        letterNeighborsSanskrit(matchLine,lineImg32,letterAImg,letterBImg);
    }
//#endif
    
    //drawGrapeLine(matchLine); exit(0);

    for(int i=0;i<matchLine.size();i++){
        if(matchLine[i].correlation){
          matchLine[i].status=0;
          dLine.push_back(matchLine[i]);
        }
    }

    //компрессия. Все одинаковые буквы в пределах габарита буквы
    //заменяются на одну букву с макcмимальной корреляцией
    compressMatch(dLine);
    
    for(int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation||!dLine[i].name.size())continue;
        wstring w;  w=UTF_to_Unicode(dLine[i].name);
        //переводим все буквы в верхний регистр
        w[0]=UniBigLetters[w[0]];
        dLine[i].uni=w[0];
    }


    //drawGrapeLine(dLine); exit(0);
    
    if(print){
        //TIME_PRINT_
        DR("STACK")};

    
    
    //drawGrapeLine(dLine); exit(0);
    
    if(OCRMode==1){
        collectStackLetter(strArray,dLine, matchLine,lineImg32,letterAImg,letterBImg,lineIndex);

        //cout<<"COLLECT"; TIME_PRINT_
        //exit(0);
        //drawGrapeLine(matchLine);exit(0);
        compressMatch(matchLine);
    }else{
        matchLine=dLine;
        return;
    }
    
    //drawGrapeLine(matchLine);exit(0);

//#ifndef OCR_woodblock
    //анализ готовых стеков.
    //проверяем есть ли над или под одиночной буквой коренные буквы или огласовки с высокой корреляцией.
    //если есть, то букву считаем частью стека и убираем как строительный блок OpenType.
//    testStackLetter(matchLine,lineImg32,letterAImg,letterBImg);
//#endif
    
    for(int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation||(dLine[i].OCRIndex!='Z'&&dLine[i].OCRIndex!='N'))continue;
        matchLine.push_back(dLine[i]);
        
    }
    //после сборки необходимо стереть отдельно стоящие и не вошедшие в стек надписные буквы ra
    for(int i=0;i<matchLine.size();i++){
        if(matchLine[i].name!="ར")continue;
        if(matchLine[i].letterW>matchLine[i].letterH*1.2)matchLine[i].correlation=0;
    }

    
    //drawGrapeLine(matchLine); exit(0);
    
    //if(print){TIME_PRINT_}
    return;
 }


void GLogicProcessor::saveMatch(vector<OCRMatch>&line,const char *path){
    OCRMatchConst *data;
    uint size=0;
    data=(OCRMatchConst*)malloc(line.size()*sizeof(OCRMatchConst));
    for(int i=0;i<line.size();i++){
        if(!line[i].correlation)continue;
        data[size]=line[i];
        size++;
    }
    writeText((char*)data,size*(uint)sizeof(OCRMatchConst), path);
}


void GLogicProcessor::readMatch(vector<OCRMatch>&line,const char *path){
    OCRMatchConst *data;
    string str; readText(str,path);
    line.resize(str.size()/sizeof(OCRMatchConst));
    data=(OCRMatchConst*)&str[0];
    for(int i=0;i<line.size();i++){
        //DR(data[i].name.c_str()<<"////"<<i)
        line[i]=data[i];
    }

}

void GLogicProcessor::compactResultOCR(vector<OCRMatch>&letterLine,string&OCRresult){
   
    vector<OCRMatch>matchLine; matchLine.resize(letterLine.size());

    for(int i=0;i<letterLine.size();i++){
      for(int j=0;j<letterLine.size();j++){
          if(letterLine[j].correlation>letterLine[i].correlation&&
             letterLine[j].xCenter>letterLine[i].x0&&
             letterLine[j].xCenter<letterLine[i].x1&&
             letterLine[j].name!="་"
             ){letterLine[i].correlation=0;break;}
      }
    }
    string xKey; xKey=(char)0xE0; xKey+=(char)0xBC; xKey+=(char)0xB9;
    
    for(int i=0;i<letterLine.size();i++){
        if(letterLine[i].correlation==0)continue;
        OCRresult+=letterLine[i].name;
        OCRresult+=letterLine[i].delimeter;
    }
    string str="ཅ"+xKey;
    OCRresult=str_replace(str.c_str(), "ཙ", OCRresult);
    str="ཆ"+xKey;
    OCRresult=str_replace(str.c_str(), "ཚ", OCRresult);
    str="ཇ"+xKey;
    OCRresult=str_replace(str.c_str(), "ཛ", OCRresult);

}





/*
 //frame detector
 
 int upFrame=0;
 int downFrame=1000;
 int countFrame=0;
 int countUpFrame=0;
 int countDownFrame=0;
 int countLetter=0;
 
 
 
 int frameFlag=0;
 //рамка внутри строки или ниже
 if(matchLine[i].name=="="){
 countFrame++;
 }
 if(countFrame==3)frameFlag=1;
 
 if(frameFlag&&!count&&matchLine[i].name=="="){
 for(int n=0;n<matchLine.size();n++){
 if(!matchLine[n].correlation)continue;
 
 //print=0;if(n==320)print=1;
 if(matchLine[n].name=="="){  //cout<<"@@@"<<matchLine[n].name;
 if(matchLine[n].yCenter>y0Base-20&&matchLine[n].yCenter<y1Base+20){
 matchLine[n].correlation=0;
 count++;
 }
 if(matchLine[n].yCenter<y0Base-20){
 countUpFrame++;
 if(countUpFrame>3)upFrame=matchLine[n].yCenter+10;
 matchLine[n].correlation=0;
 }
 if(matchLine[n].yCenter>y1Base+20){
 countDownFrame++;
 if(countDownFrame>3)downFrame=matchLine[n].yCenter-10;
 matchLine[n].correlation=0;
 }
 //
 DR(n<<" yCenter="<<matchLine[n].yCenter<<" upFrame="<<upFrame<<" downFrame="<<downFrame<<" count="<<count<<endl;)
 
 }else if(matchLine[n].name!="ར"&&matchLine[n].name!="་"&&matchLine[n].name!="ི"){
 countLetter++;
 }
 if(matchLine[n].yCenter<upFrame||matchLine[n].yCenter>downFrame){
 matchLine[n].correlation=0;
 }
 
 
 }
 }
 
 DR(" count="<<count<<" countLetter="<<countLetter<<endl)
 if(count>countLetter/3){
 DR("remove frame line"<<endl)
 matchLine.resize(0);
 return;
 }
 
 //print=0; if(i==815)print=1;   //верхняя рамка распознанная совместно с первой строкой
 //DR("matchLine[i].yCenter="<<matchLine[i].yCenter<<" y0Base="<<y0Base<<" frameFlag="<<frameFlag<<" upFrame="<<upFrame<<endl)
 
 if(matchLine[i].name=="|"&&matchLine[i].correlation>90){
 //print=1;
 int xC=matchLine[i].xCenter;
 DR(endl<<"i="<<i<<" s="<<matchLine.size()<<" c="<<matchLine[i].correlation<<" xC="<<xC<<endl)
 if(i>matchLine.size()/2){
 DR("test right\n")
 count=0;
 int rightLimit=0;
 for(int n=i;n<matchLine.size();n++){
 if(matchLine[n].OCRIndex=='A'&&
 rightLimit<matchLine[n].xCenter){
 count++;
 rightLimit=matchLine[n].x1;
 }
 if(count>10)break;
 }
 if(count>10){matchLine[i].name="།"; DR("have root letters\n") continue;}
 matchLine.resize(i);   DR("count="<<count<< "erase >>"<<endl)
 for(int n=(int)matchLine.size()-20;n<matchLine.size()&&n>-1;n++){
 if((matchLine[n].name=="།"||matchLine[n].name=="|"||matchLine[n].name=="1")&&xC-matchLine[n].xCenter<3){
 //DR("n="<<n<<" matchLine[n].xCenter="<<matchLine[n].xCenter<<endl;)
 matchLine[n].correlation=0;
 }
 }
 DR("erase done"<<endl)
 }
 if(i<matchLine.size()/2){
 DR("test left\n")
 count=0;
 int leftLimit=0;
 for(int n=0;n<i;n++){
 if(matchLine[n].OCRIndex=='A'&&
 leftLimit<matchLine[n].xCenter){
 count++;
 leftLimit=matchLine[n].x1;
 }
 if(count>10)break;
 }
 if(count>10){matchLine[i].name="།"; DR("have root letters\n") continue;}
 DR("count="<<count<< "erase <<"<<endl)
 for(int n=i+20;n>=0&&n<matchLine.size();n--){
 if(matchLine[n].xCenter-xC<3){matchLine[n].correlation=0;continue;}
 DR("n="<<n<<" matchLine[n].xCenter="<<matchLine[n].xCenter<<" n="<<matchLine[n].name<<endl)
 if((matchLine[n].name=="།"||matchLine[n].name=="|"||matchLine[n].name=="1")&&matchLine[n].xCenter-xC<3){
 matchLine[n].correlation=0;
 DR("+")
 }
 
 }
 DR("erase done"<<endl)
 
 }
 print=0;
 
 }
 
 }
 */


