#include "GLogicProcessor.h"


void GLogicProcessor::grammarCorrector(string &lineString,
                                       string &xmlString){

        //cout<<"@"<<mode<<" "<<lineString<<endl;
        textNormalisation(lineString);
        //cout<<"%"<<mode<<" "<<lineString<<endl;

}

void GLogicProcessor::textNormalisation( string &str){
    
    //cout<<"Start grammar correction "<<str<<endl;
    if(str.size()<3)return;
    vector<string>v=explode("\n",str);
   
    vector<string>rexExpSource;
    string path;
    if(inputData.data["ocrLn"]==""){
        ulong *p=aliKali->OCRLanguage;
        if(*p==OCR_TIBETAN)inputData.data["ocrLn"]="ocrTibetan";
        if(*p==OCR_TIBETAN_ENG)inputData.data["ocrLn"]="ocrTibetanEnglish";
        if(*p==OCR_TIBETAN_RUS)inputData.data["ocrLn"]="ocrTibetanRussian";
        if(*p==OCR_TIBETAN_SKT)inputData.data["ocrLn"]="ocrTibetanSanskrit";
        if(*p==OCR_SANSKRIT)inputData.data["ocrLn"]="ocrSanskrit";
        if(*p==OCR_KANNADA)inputData.data["ocrLn"]="ocrKannada";
    
    }
    
    if(inputData.data["ocrLn"]=="ocrTibetanEnglish"){path=inputData.data["root"]+"/OCRData/Grammar/regExp.txt";}
    if(inputData.data["ocrLn"]=="ocrTibetanRussian"){path=inputData.data["root"]+"/OCRData/Grammar/regExpRus.txt";}
    if(inputData.data["ocrLn"]=="ocrTibetan"){path=inputData.data["root"]+"/OCRData/Grammar/regExpTib.txt";}
    if(inputData.data["ocrLn"]=="ocrTibetanSanskrit"){path=inputData.data["root"]+"/OCRData/Grammar/regExpTibSkt.txt";}
    if(inputData.data["ocrLn"]=="ocrSanskrit"){path=inputData.data["root"]+"/OCRData/Grammar/regExpSkt.txt";}
    if(inputData.data["ocrLn"]=="ocrKannada"){path=inputData.data["root"]+"/OCRData/Grammar/regExpKannada.txt";}
    
    readText(rexExpSource,path);
    for(int n=0;n<v.size();n++){
        for(int i=0;i<rexExpSource.size();i++){
            //cout<<"rexExpSource="<<rexExpSource[i]<<endl;
            regExpReplace(v[n],rexExpSource[i]);
            //cout<<"str="<<str<<endl;
        }
    }
    
    str=implode("\n",v);
    rexExpSource.resize(0);
    path=str_replace(".txt","_str.txt",path);
    readText(rexExpSource,path);
    for(int i=0;i<rexExpSource.size();i++){
            //cout<<"rexExpSource="<<rexExpSource[i]<<endl;
            vector<string>line=explode(":|:",rexExpSource[i]);
            if(line.size()<2)continue;
            str=str_replace(line[0],line[1],str);
            //cout<<"str="<<str<<"    //"<<rexExpSource[i]<<endl;
    }
    str=str_replace("\\n","\n",str);
    str=str_replace("\n\n","\n",str);
    str=str_replace("\n\n","\n",str);
    str=str_replace("\n\n","\n",str);
}

//проверка текста по словарю на точное совпадение с учетом статистики
int GLogicProcessor::textCorrector(vector<OCRMatch>&dLine, string &mainString){
    
    int print=0;
    
    vector<OCRMatch>OCRStr;
    for(int i=0;i<dLine.size();i++){
        if(!dLine[i].correlation)continue;
        DR("dLine["<<i<<"].name="<<dLine[i].name<<" d="<<dLine[i].delimeter<<endl)
        
        if(dLine[i].OCRIndex=='Z'||dLine[i].OCRIndex=='N'){
            
            dLine[i].delimeter=dLine[i].name+dLine[i].delimeter; dLine[i].name="";
            OCRMatch match=dLine[i];
            OCRStr.push_back(match);
            
        }else{
            //vector<string> syllables;
            //разбираем строку на слоги. Разделители слогов, знаки препинания и незнакомые буквы заносим в разделители слогов
            string name=dLine[i].name;
            wstring wStr=UTF_to_Unicode(name);
            string delimeter;
            int startFlag,endFlag;
            int d=0;
            while(d<wStr.size()){
                OCRMatch match;
                OCRStr.push_back(match);
                OCRMatch &m=OCRStr[OCRStr.size()-1];
                startFlag=0;
                endFlag=0;
                //DR("w="<<Unicode_to_UTF(wStr[d])<<endl)
                while(d<wStr.size()){
                    //DR("w1="<<Unicode_to_UTF(wStr[d])<<endl)
                    string s=Unicode_to_UTF(wStr[d]);
                    if(!isDelimeter(s)){
                        if(endFlag){
                            d--;
                            endFlag=0;
                            break;
                        }else{
                            m.name+=Unicode_to_UTF(wStr[d]);
                        }
                        startFlag=1;
                        
                    }else{
                        m.delimeter+=s;
                        if(startFlag)endFlag=1;
                        startFlag=0;
                        
                    }
                    d++;
                }
                mainString+=m.name;
                if((m.delimeter.find(" ")!=-1||m.delimeter.find("•")!=-1)&&dLine[i].delimeter.find("\n")==-1){
                    mainString+="@";
                }
                d++;
            }
            if(OCRStr.size())OCRStr[OCRStr.size()-1].delimeter+=dLine[i].delimeter;
            //DR("OCRStr["<<OCRStr.size()-1<<"].name"<<OCRStr[OCRStr.size()-1].name<<" d="<<OCRStr[OCRStr.size()-1].delimeter<<endl)
        }
        
    }
    
    //for(int i=0; i<OCRStr.size();i++){
    //    cout<<"++OCRStr["<<i<<"].name="<<OCRStr[i].name<<"/ "<<OCRStr[i].delimeter<<endl;
    //}
    //cout<<"mainString="<<mainString;
    //exit(0);
    
    //drawGrapeLine(dLine); exit(0);
    //return;
    
    //Провка статистической устойчивости распознаваемого текста. Отмечаем количество совпадений последовательностей букв проверяемой строки и фраз словаря. Маркируем каждую букву в соответствии с количеством и длинной совпадений
    
    vector<string> lineText=explode("@", mainString);  //cout<<" mainString="<<mainString;
    int indexText=0;
    mainString="";
    
    for(int lineIndex=0;lineIndex<lineText.size();lineIndex++){
        string strW=lineText[lineIndex]+"\n";
        
        string result=strW;
        textCorpusGMap->associativeSignalProcessing(result);
        //result=UnicodeToYagpo(result);
        result=str_replace("་", "@", result);
        
        //cout<<endl<<"@@@@result1="<<result;
        //writeText(result,"/_Image2OCR/11.html"); //exit(0);
        
        
        print=0;
        int index=0; strW="";
        string str;
        //string test;
        while(indexText<OCRStr.size()&&index<result.size()){
            DR("OCRStr["<<indexText<<"].name="<<OCRStr[indexText].name<<"/"<<endl)
            for(int n=0;n<OCRStr[indexText].name.size();n++){
                DR("result["<<index<<"]="<<result[index]<<" n="<<n<<" "<<OCRStr[indexText].name[n]<<endl)
                while(index<result.size()&&result[index]=='<'){
                    while(index<result.size()&&result[index]!='>'){
                        str+=result[index];
                        index++;
                    }
                    index++;
                    strW+=str+">";
                    str="";
                }
                strW+=OCRStr[indexText].name[n];   DR("add "<<n<<" "<<OCRStr[indexText].name[n]<<endl)
                //test+=result[index];
                index++;
                while(index<result.size()&&result[index]=='<'){
                    while(index<result.size()&&result[index]!='>'){
                        str+=result[index];
                        index++;
                    }
                    index++;
                    strW+=str+">";
                    str="";
                }
                if(result[index]=='@'){
                    //if(n>0){
                    //    DR("add delimeter @"<<endl);
                        //strW+="@";  //словарь предполагает добавить разделитель
                    //}
                    index++;
                }
                DR(" strW="<<strW<<endl);  //" test:"<<test
            }
            DR("add delimeter "<<OCRStr[indexText].delimeter<<endl)
            strW+=OCRStr[indexText].delimeter;
            indexText++;
        }
        //cout<<endl<<"@@@@result="<<strW;
        strW=str_replace("@་", "་", strW);
        strW=str_replace("@", "་", strW);
        mainString+=str_replace("•", " ", strW);
        
        
        
        //writeText(mainString,"/_Image2OCR/2.html");
        //exit(0);
        
    }
    
    //cout<<mainString;
    mainString=YagpoToUni(mainString);
    mainString=str_replace("\n", "<br/>\n", mainString);
    
    //cout<<"\n\n\n\n"<<mainString;
    
    //writeText(mainString,"/_Image2OCR/2.html");
    
    return 1;

}

//проверка текста по словарю на точное совпадение без учета статистики

void GLogicProcessor::grammarCorrector(vector<stringOCR>&strArray,
									   vector<stringOCR> &correctionWordArray,
                                       string &mainString,
                                       string &xmlString){
    
    int recordFound, length,c;
	//string srcString=str_+" ";
	string srcLine,srcStr,str,testLine,request,answer,oldString;    
		str="";
	
	//unsigned int hash;
    vector<stringOCR> stringItemVector;
	vector<string>strResultArray, pageResultArray;
    vector<string>xmlResultArray;
    string xmlStartLine, xmlLine;

    
	stringOCR wordLine,testLineOCR,testLineOCR_revers;
	string strResult,strReport,strReportMem;
	string strCount;
	int print=0;
    int entryStart;
	int lineIndex=(int)strArray[0].size();
	vector<string>resultArray; resultArray.resize(lineIndex);
	string color; 
    int putInCorrectionTable=0;
    ostringstream out;
    int strReOCR;  //флаг указывает на возможную ошибку в распознавании строки.
    vector<wordOCR>lineText;
    vector<wordOCR>lineTextUp;
    vector<wordOCR>lineTextDown;
    int startFlag,endFlag;  //флаги наличия добавленных слов в начале и конце строки
    
    DT("Start grammar correction strArray[0].size()="<<strArray[0].size()<<" delimeterTib.size()="<<delimeterTib.size()<<" correctionWordArray.size()="<<correctionWordArray.size()<<endl);

    
    //применяем регулярные выражения к результату распознавания
    
	vector<string>delimeterString;
    if(delimeterTib.size()>2)for(int a=2;a<delimeterTib.size();a++)delimeterString.push_back(delimeterTib[a]);

    mainString="";
    
    for(int index=strArray[0].size()-1;index>=0;index--){   //основной цикл разбора страницы строка за строкой.
        
        
        //начинаем новую строку в XML
        out.str("");
        out<<"<line i=\""<<index<<"\" ";
        xmlStartLine=out.str();
        
        strReOCR=0;
        
        
        stringOCR *line=&strArray[index];   //in strArray line numeration is reversed
        stringOCR *lineUP; if(index+1<strArray[0].size()){lineUP=&strArray[index+1];}else{lineUP=0;}
        stringOCR *lineDown; if(index>0){lineDown=&strArray[index-1];}else{lineDown=0;}
        
        
        if(index==strArray[0].size()-1){ //the first string
            applyRE(line->wordArray,lineText);
            if(lineDown)applyRE(lineDown->wordArray,lineTextDown);
        }else{
            lineTextUp=lineText;
            lineText=lineTextDown;
            lineTextDown.resize(0);
            if(lineDown)applyRE(lineDown->wordArray,lineTextDown);
        }
        
        if(print){cout_<<"@string "; for(int a=0;a<lineText.size();a++){cout_<<lineText[a].name<<lineText[a].delimeter;} cout_<<endl;}
        
        if(!dictionaryReady){cout_<<"dictionary not ready";
            for(int t=0;t<lineText.size();t++){
                mainString+=lineText[t].name+lineText[t].delimeter+lineText[t].commentaryIndex;
            }
            mainString+="</br>\n";

        }else{
            startFlag=0; endFlag=0;
            
            //формируем строку для проверки по словарю. Добавляем содержание верхней и/или последующей строки
            for(int a=0;a<lineText.size();a++){   //для каждой буквы в разбираемой строке

                wordLine.push_back(lineText[a]); 	
                
                if(a==0&&lineTextUp.size()&&lineText.size()>10){  //добавляем начало слова или фразы
                    lineTextUp[lineTextUp.size()-1].newLine="<br/>\n";
                    for(int t=(int)lineTextUp.size()-1;t>=0;t--){
                        if(lineTextUp[t].testDelimeter(delimeterString))break;
                        wordLine.insert(0,lineTextUp[t]);
                        startFlag=1;
                    }
                }
                if(a==lineText.size()-1&&lineTextDown.size()>10&&!lineText[a].testDelimeter(delimeterString)){  //добавляем окончание слова или фразы
                    wordLine[wordLine.size()-1]->newLine="<br/>\n";
                    for(int t=0;t<lineTextDown.size()-1;t++){
                        if(lineTextDown[t].testDelimeter(delimeterString))break;
                        wordLine.push_back(lineTextDown[t]);
                        endFlag=1;
                    }
                }    
                
                DP(" lineText["<<a<<"].name="<<lineText[a].name<<" d="<<lineText[a].delimeter<<
                      " c="<<lineText[a].commentaryIndex<<" n="<<lineText[a].newLine<<" str="<<wordLine.str<<endl);
                //}
                if(lineText[a].testDelimeter(delimeterString)||a==lineText.size()-1){ 
                    //сформировали строку для проверки. Для этой строки применяем грамматический анализ
                                       
                    if(print)for(int i=0;i<wordLine.size();i++){
                        DT("str_=/"<<wordLine[i]->name<<"/ delimeter="<<wordLine[i]->delimeter<<" newLine="<<wordLine[i]->newLine);
                    }	
                    if(print){cout_<<"@stringW "; for(int a=0;a<wordLine.size();a++){cout_<<wordLine[a]->name<<wordLine[a]->delimeter<<wordLine[a]->newLine;} cout_<<endl;}
                    
                        DP("start report wordLine.size()="<<wordLine.size()<<" str="<<wordLine.str<<endl);
                        for (int entry=0; entry<wordLine.size();entry++){ //for every syllabon in List
                            for (int k=wordLine.size();k>entry;k--){ //for every syllabon in List
                                recordFound=0; 
                                srcLine=""; srcStr=""; length=0; testLineOCR.resize(0);
                                for (c=entry;c<k;c++){
                                    //DP("stringItemVector["<<c<<"].str=/"<<wordLine[c]->name<<"/ wordLine["<<c<<"].delimeter="<<wordLine[c]->delimeter<<endl);
                                    testLineOCR.push_back(*wordLine[c]);  //all syllabons in this part of source string
                                    //cout_<<"testLineOCR.x0="<<testLineOCR.x0<<" wordLine["<<c<<"].x0="<<wordLine[c]->x0<<endl;
                                    srcStr+=wordLine[c]->name+wordLine[c]->delimeter;
                                    length++;
                                }
                                DP(" testLineOCR.size()="<<testLineOCR.size()<<" testLineOCR.str=/"<<testLineOCR.str<<" testLineOCR.name=/"<<testLineOCR.name<<"/"<<endl); 
                                
                                testLine=testLineOCR.str; //all string syllables without delimeters

                                if(testLine!=""&&testLine!=" "&&testLine!=delimeterString[0]){
                                testLine+=":|:";
                                testLine+=srcStr;
                                    DP(" srcLine="<<srcStr<<" testLine=/"<<testLine<<"/ length="<<length<<endl);
                                       answer=remapString(testLine);
                                }else{
                                    answer=srcStr;
                                    testLineOCR.str=srcStr;
                                   
                                }							
                                
                                if(answer!=":|:"){
                                    recordFound=1;
                                    //str=answer;
                                    DP("match found testLineOCR.str="<<testLineOCR.str<<" testLineOCR.name="<<testLineOCR.name<<" answer="<<answer<<" srcStr="<<srcStr<<endl);
                                    //strReport=str_replace(testLineOCR.name,answer,srcStr);
                                    if(answer.size()<DICT_REPORT_SIZE){
                                        strReport=testLineOCR.name;								
                                    }else{
                                        strReport=answer;
                                    }

                                    //strCount=str_replace("་","", answer); length=(answer.size()-strCount.size())/3+1; 
                                    DT(" length="<<length<<" answer=/"<<answer<<"/ s="<<answer.size()<<"-"<<" strReport="<<strReport<<endl);
                                    //DP("answer="<<str<<endl);							
                                
                                }else{ DP(" not_found"<<" length="<<length<<endl);
                                       if(length==1)recordFound=1;
                                       strReport=testLineOCR.name;
                                }
                                
                                
                                if(recordFound){
                                    DP("record found "<<k<<" e="<<entry<<endl);
                                    if(srcStr=="།"&&(entry==0||entry==wordLine.size()-1))length=9;
                                    DT("input str="<<testLineOCR.name<<" strReport="<<strReport<<endl);
                                    if(srcStr!=" "&&srcStr!="@"){
                                        testLineOCR.set_id(strReport); 
                                        DT("output str="<<strReport<<endl);
                                            if(length<6){
                                                testLineOCR.path=inputData.data["inputFile"]; 
                                                testLineOCR.file=fileName(testLineOCR.path);
                                                testLineOCR.stringFlag=0;  //heed check;
                                            //если строка не найдена проверяем есть ли в словаре эта строка плюс предыдущий слог
                                            //(обратный поиск)
                                                
                                                int entry_revers=entry-1; int letterCount=0, length_revers=0;
                                                while(entry_revers>0&&wordLine[entry_revers]->name=="")entry_revers--; //пропускаем разделители слогов
                                                //string strReport1;
                                                while(entry_revers>0){ DT("revers search"<<endl);
                                                    testLineOCR_revers.resize(0); srcStr=""; length_revers=0;
                                                    for (c=entry_revers;c<k;c++){
                                                        DP("stringItemVector["<<c<<"].str=/"<<wordLine[c]->name<<"/ wordLine["<<c<<"].delimeter="<<wordLine[c]->delimeter<<endl);
                                                        testLineOCR_revers.push_back(*wordLine[c]);  //all syllabons in this part of source string
                                                        srcStr+=wordLine[c]->name+wordLine[c]->delimeter;
                                                        length_revers++;
                                                    }
                                                    
                                                    testLine=testLineOCR_revers.str; //all string syllables without delimeters
                                                    if(testLine!=""&&testLine!=" "&&testLine!=delimeterString[0]){
                                                        testLine+=":|:";
                                                        testLine+=srcStr;
                                                        DT(" srcLine="<<srcStr<<" testLine=/"<<testLine<<"/ length_revers="<<length_revers<<endl);

                                                        answer=remapString(testLine);
                                                        if(answer!=":|:"){  
                                                            //теперь нужно объединить результаты прямого и обратного поиска
                                                            //если ответ найден, то в текст пишем новую объединенную строку
                                                            DT("match="<<length_revers<<" entryStart="<<entryStart<<endl);
                                                            testLineOCR_revers.resize(0); length_revers=0; srcStr="";
                                                            for (c=entryStart;c<k;c++){
                                                                DP("wordLine["<<c<<"]->name=/"<<wordLine[c]->name<<"/ wordLine["<<c<<"].delimeter="<<wordLine[c]->delimeter<<endl);
                                                                testLineOCR_revers.push_back(*wordLine[c]);  //all syllabons in this part of source string
                                                                //cout_<<"testLineOCR.x0="<<testLineOCR.x0<<" wordLine["<<c<<"].x0="<<wordLine[c]->x0<<endl;
                                                                srcStr+=wordLine[c]->name+wordLine[c]->delimeter;
                                                                length_revers++;
                                                            }
                                                            DT("new str="<<testLineOCR_revers.name<<endl);
                                                            
                                                            strReport=remapTibString(testLineOCR_revers.name,answer); 
                                                            DT("strReport1="<<strReport<<endl);
                                                            
                                                            //cout_<<" putInCorrectionTable="<<putInCorrectionTable<<" correctionWordArray.size()="<<correctionWordArray.size()<<endl;
                                                            if(putInCorrectionTable){ //проверяем и стираем предыдущую запись в корректурной таблице
                                                                //cout_<<"test name="<<correctionWordArray[correctionWordArray.size()-1].name<<"/"<<endl;														
                                                                if(strReport.find(correctionWordArray[correctionWordArray.size()-1].name,0)!=string::npos){
                                                                    //cout_<<"POP_BACK match"<<endl;
                                                                    correctionWordArray.pop_back();  
                                                                    putInCorrectionTable=0;
                                                                }	
                                                            } 
                                                            DT("erase="<<strResultArray[strResultArray.size()-1]<<endl;)
                                                            strResultArray.pop_back();  //стираем предыдущую строку
                                                            xmlResultArray.pop_back();
                                                            
                                                            testLineOCR_revers.set_id(strReport); 
                                                            DT("full report="<<strReport<<endl);
                                                            length=length_revers;
                                                            break;
                                                        }
                                                        
                                                    }else{
                                                        entryStart=entry;
                                                        break;//если строка пустая то прекращаем обратный разбор строки												
                                                    }	
                                                    entry_revers--;
                                                    if(answer==":|:"){ //если строка не найдена то ищем еще 7 букв (размер слога)
                                                        letterCount++;
                                                        if(letterCount==7||entry_revers==0){
                                                            DP("no match entry="<<entry<<endl)
                                                            entryStart=entry;
                                                            break;        //если предыдущий слог не найден, то прекращаем обратный разбор строки
                                                        }	
                                                    }
                                                    
                                                }
                                            }
                                             
                                               DT("length="<<length<<endl);
                                                
                                               if(length<6){
                                                   if(lineText.size()>10){strReOCR=1;}else{length=6;}
                                                   //cout_<<"testLineOCR="<<testLineOCR.name<<"/"<<endl;
                                                }   
                                        
                                                if(length==0)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==1)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==2)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==3)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==4)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==5)color="<span class=\"c\" style=\"color:#FF0000\">";
                                                if(length==6)color="<span class=\"c\" style=\"color:#008800\">";
                                                if(length==7)color="<span class=\"c\" style=\"color:#007700\">";
                                                if(length==8)color="<span class=\"c\" style=\"color:#006600\">";
                                                if(length==9)color="<span class=\"c\" style=\"color:#006633\">";
                                                if(length==10)color="<span class=\"c\" style=\"color:#009900\">";
                                                if(length==11)color="<span class=\"c\" style=\"color:#009900\">";
                                                if(length==12)color="<span class=\"c\" style=\"color:#006633\">";
                                                if(length==13)color="<span class=\"c\" style=\"color:#006633\">";
                                                if(length==14)color="<span class=\"c\" style=\"color:#0033FF\">";
                                                if(length==15)color="<span class=\"c\" style=\"color:#0033FF\">";
                                                if(length==16)color="<span class=\"c\" style=\"color:#0066CC\">";
                                                if(length==16)color="<span class=\"c\" style=\"color:#0066CC\">";
                                                if(length==17)color="<span class=\"c\" style=\"color:#003399\">";
                                                if(length==18)color="<span class=\"c\" style=\"color:#003399\">";
                                                if(length==19)color="<span class=\"c\" style=\"color:#0000CC\">";
                                                if(length==20)color="<span class=\"c\" style=\"color:#0000CC\">";
                                                if(length==21)color="<span class=\"c\" style=\"color:#000099\">";
                                                if(length==22)color="<span class=\"c\" style=\"color:#000099\">";
                                                if(length==23)color="<span class=\"c\" style=\"color:#000066\">";
                                                if(length==24)color="<span class=\"c\" style=\"color:#000066\">";
                                                if(length==25)color="<span class=\"c\" style=\"color:#000033\">";
                                                if(length==26)color="<span class=\"c\" style=\"color:#000033\">";
                                                if(length>=27)color="<span class=\"c\" style=\"color:#000000\">";
                                                strResult=color+strReport+"</span>";
                                                //strResultArray.push_back("/#/<br>");
                                                //cout_<<"/@@@ strResult="<<strResult<<endl;
                                                strResultArray.push_back(strResult);
                                                xmlResultArray.push_back(strReport);
                                                //strResultArray.push_back("/@/<br>");								
                                                DT("strResult="<<strResult<<":|:"<<endl);
                                    }else{
                                      strResult=" ";
                                      strResultArray.push_back(strResult);
                                      xmlResultArray.push_back(strResult);
                                    }	
                                    strReportMem=answer;   //запоминаем результаты разбора строки для использования при обратном поиске
                                    if(!testLineOCR_revers.size())entryStart=entry;
                                    testLineOCR_revers.resize(0);
                                    entry=k-1;
                                    break;	
                                } 			
                            }//end for (int k=StringItemList->Count
                        }//end for (int entry=0;
                //request="stop";
                    wordLine.resize(0); //exit(0);
                } //wordLine analise
                
            } //lineText analise
            
        }
        out.str("");
        out<<"ocr=\""<<strReOCR<<"\">";
        xmlStartLine+=out.str(); 
        
        //при необходимости обрезаем начало и конец строки
        //сохраняем готовую строку в массив
        if(inputData.data["ocrData"]=="oneStringOCR"&&xmlResultArray.size()==0){
            xmlLine="";//в режиме многопотокового распознавания страницы сохраняем только одну строку
        }else{
            xmlLine=implode("",xmlResultArray); 
            vector<string>sX; sX=explode("<br/>\n",xmlLine);
            
            if(startFlag){
                if(sX.size()>1){xmlLine=sX[1];}else{cout_<<"count string error"; 
                    //exit(0);
                }
            }else if(endFlag){
                if(sX.size()>1){xmlLine=sX[0];}else{cout_<<"count string error"; 
                    //exit(0);
                }
            }
        }
        xmlLine=xmlStartLine+xmlLine+"</line>\n";
        xmlString+=xmlLine;
        xmlResultArray.resize(0);        
       
        //при необходимости обрезаем начало и конец строки
        //сохраняем готовую строку в массив
        //cout_<<"@@startFlag="<<startFlag<<endl;
        
        str=implode("",strResultArray);  
        //cout_<<"str="<<str<<endl;
        vector<string>sV; sV=explode("<br/>\n",str);

        if(startFlag){
            if(sV.size()>1){str=sV[1];}else{cout_<<"count string error"; //exit(0);
            }
        }else if(endFlag){
            if(sV.size()>1){str=sV[0];}else{cout_<<"count string error"; //exit(0);
            }
        }else{
            str=str;
        }
        if(inputData.data["ocrData"]!="oneStringOCR")str+="<br/>\n";
        pageResultArray.push_back(str);
        strResultArray.resize(0);
        
        
        //str=xmlResultArray[xmlResultArray.size()-1];
        //str=substr(str.find("<br/>\n")+5,str.size(),str);
        //xmlResultArray[xmlResultArray.size()-1]=str;
        //cout_<<"@XML="<<str<<endl;
        
    }//основной цикл разбора страницы строка за строкой.

        
    DP("%4");
    if(inputData.data["ocrData"]=="oneStringOCR"){
        str="<br/>\n";
        pageResultArray.push_back(str);
    }    

	strResult=implode("",pageResultArray);
	
   //cout_<<" strResult="<<strResult<<endl;
    
	mainString=strResult;     

}//__________________________________________________________________________


///функция объединяет два ответа словаря при прямом и обратном проходе строки
string GLogicProcessor::remapTibString(string &input0, string &input1){

	int print=GRAMMAR_LOG;
	DT("//@@@@@@@@@@@/ input0="<<input0<<" input1="<<input1<<" input1.size()="<<input1.size()<<endl);
	if(input0.size()<DICT_REPORT_SIZE||input1.size()<DICT_REPORT_SIZE)return input0;
	///в коротких ответах словаря не используем разбиение на слоги
	
	    //input1=str_replace("་","", input1);
		DT("//@@@@@@@@@@@/ input1="<<input1<<endl);

		
		string str=substr(0, 3, input1);

		int pos=input0.rfind(str);
		DT("str"<<str<<" pos="<<pos<<endl);
		  str=substr(0, pos, input0);
		  str+=input1;
		
		DT(" str="<<str<<endl);
		return str;
}

void GLogicProcessor::writeInCorrectionPage( vector<stringOCR>*strArray, vector<stringOCR> &correctionWordArray,stringOCR &testStr,string &srcStr){
	
	int lineIndex=-1;
	int flag;
	
	
	for(int index=0;index<testStr.size();index++){
		
		if(testStr.wordArray[index].lineIndex!=lineIndex){
			lineIndex=testStr.wordArray[index].lineIndex;		
			if(lineIndex>-1&&lineIndex<strArray[0].size()){
			  //cout_<<"@@@  write in file lineIndex="<<lineIndex<<" testStr.name="<<testStr.name<<endl;
		      stringOCR str;
			  str.file=testStr.file;
			  str.lineIndex=lineIndex;
			  str.x0=0;
			
			  str.x1=strArray[0][lineIndex].x1;
			  str.LimY0=strArray[0][lineIndex].LimY0;
			  str.LimY1=strArray[0][lineIndex].LimY1;
				
			  str.name=testStr.name;
			  flag=1;
			  for (int a=0;a<correctionWordArray.size();a++){
					if(correctionWordArray[a].lineIndex==lineIndex)flag=0;
              }
              //str.img=img;   //GBitmap::createRegion(); 
                              
			  if(flag)correctionWordArray.push_back(str);
			}	
		}
			
    }
	
}///_________________________________________________________________________




void GLogicProcessor::checkInCorrectionTable(vector<stringOCR> &correctionTable,stringOCR &testStr,string &srcStr){

	int x0,x1,y0,y1;
	int dl=10;
	cout_<<"//________________ testStr.str="<<testStr.str<<" testStr.file="<<testStr.file<<endl;
	int maxX=0, maxY=0,minX=90000,minY=90000;
	testStr.correlation=100;
	
	for(int index=0;index<testStr.size();index++){
		x0=testStr.wordArray[index].x0+dl;
		y0=testStr.wordArray[index].y0+dl;
		x1=testStr.wordArray[index].x1-dl;
		y1=testStr.wordArray[index].y1-dl;
		
		if(x0<minX)minX=x0;
		if(y0<minY)minY=y0;
		if(x1>maxX)maxX=x1;
		if(y1>maxY)maxY=y1;
		//dX=(x1-x0)/2;
		//dY=(y1-y0)/2;
		//testStr.wordArray[index].checkFlag=0;
		//cout_<<"testStr.wordArray["<<index<<"].correlation="<<testStr.wordArray[index].correlation<<endl;
		//if(testStr.wordArray[index].correlation<99){flag=0;}
		if(testStr.correlation>testStr.wordArray[index].correlation)testStr.correlation=testStr.wordArray[index].correlation;

	}
	//if(!flag){
		testStr.x0=minX; 
		testStr.y0=minY;
		testStr.x1=maxX;
		testStr.y1=maxY; 
		ostringstream out;
		out<<testStr.file<<minX<<minY<<maxX<<maxY<<testStr.correlation;
		string str=out.str();
		testStr.idNumber=strToHash(str.c_str(), str.size());
		//flag=1;
		//for(int a=0;a<correctionTable.size();a++){   //check dublicates
		//	if(testStr.idNumber==correctionTable[a].idNumber)flag=0;
		
		//}
		  correctionTable.push_back(testStr);

	//}
		/*
		for(int i=0;i<correctionWordArray.size();i++){
			//if(correctionTable[i].stringFlag!=1)continue;
			if(correctionWordArray[i].file!=testStr.file)continue;	

			
					//cout_<<"correctionTable["<<i<<"].stringFlag="<<correctionTable[i].stringFlag<<" str="<<correctionTable[i].str<<" correctionTable[i].file="<<correctionTable[i].file;
					
					dX_=(correctionWordArray[i].x1-correctionWordArray[i].x0)/2;
					dY_=(correctionWordArray[i].y1-correctionWordArray[i].y0)/2;
					
					//cout_<<"    x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<endl;
					//cout_<<"    x0_="<<correctionTable[i].x0<<" y0_="<<correctionTable[i].y0<<" x1_="<<correctionTable[i].x1<<" y1_="<<correctionTable[i].y1<<endl;
					if(dX-dX_>dl)continue;
					if(dX_-dX>dl)continue;
					if(dY-dY_>dl)continue;
					if(dY_-dY>dl)continue;
					
					if(x0<correctionWordArray[i].x0)continue;
					if(y0<correctionWordArray[i].y0)continue;
					if(x1>correctionWordArray[i].x1)continue;
					if(y1>correctionWordArray[i].y1)continue;
					testStr.wordArray[index].name=correctionWordArray[i].name;
					//cout_<<"### record found ### new name="<<correctionWordArray[i].name<<endl;
			
			flag=1;
			testStr.wordArray[index].checkFlag=1;
			break;
		}
		

		*/ 
		
	//}
    //cout_<<"correctionWordArray.size()="	<<correctionWordArray.size()<<endl;
 
	return;
	
	//exit(0);

}///_________________________________________________________________________

string GLogicProcessor::splitEngWord(string &word, int *result){
    
    int print=0;
    DT("split word="<<word<<endl);
    string match,match1,match_i,src;
    src=word;
    lowerCase(src);
    int result1=0,result2=0;
    
    int index=src.size()-1;
    while(1){
        match="";
        for(int i=0;i<index;i++)match+=src[i];
        match_i="\n"+match+"\n";
        DT(" match="<<match_i<<endl);
        if(rexExpFind(textDictEng,match_i)){DT("match=1");

            //первое слово прошло проверку. проверяем второе слово.
            match1=""; result1=1;
            for(int i=index;i<word.size();i++)match1+=word[i];
            
            match_i=match1;
            lowerCase(match_i);
            match_i=match_i="\n"+match_i+"\n";
            result2=rexExpFind(textDictEng,match_i);
            DT("match_i="<<match_i<<"result2="<<result2<<endl);
            if(result2)break;          

            
        }else{DT("match=0");}
        index--; 
        if(index==0)break;
    }
    cout_<<" result1="<<result1<<" result2="<<result2<<endl;
    if(!result1){*result=0;return word;}  //если первое слово не нашли в словаре, значит нам не удалось поделить исходное слово.
    if(!result2){*result=0;return word;}  //если второе слово не нашли в словаре, значит нам не удалось поделить исходное слово.
    //оба слова прошли проверку. Возвращаем два слова.
    match_i=match+" "+match1;
    *result=1;
    return match_i;
    
}


void GLogicProcessor::applyRE( vector<wordOCR> &wordArray,vector<wordOCR>&lineText){
    
    //grammar normalisation
    //здесь должны быть регулярные выражения.
    //для их применения используем библиотеку re2. Она работает со стандартным строковым типом. 
    //Приводим результат распознавания к этому виду.
    
    string mainString="";
    for(int a=0;a<wordArray.size();a++){
        if(wordArray[a].name=="*")continue;
        mainString+=wordArray[a].name;
        mainString+=wordArray[a].delimeter;
        mainString+=wordArray[a].newLine;
	}
   
    mainString=str_replace("•"," ",mainString);
    textNormalisation(mainString);

    if(inputData.data["languageOCR"]=="Kannada"){
        
        
        mainString=str_replace("_18_15", "_15",mainString);
        mainString=str_replace("_15_18", "_15",mainString);
        
        mainString=str_replace("_20", "್ಳ",mainString);
        mainString=str_replace("_21", "್ಯ",mainString);
        mainString=str_replace("_22", "್ಞೆ",mainString);
        mainString=str_replace("_23", "ೃ",mainString);
        mainString=str_replace("_24", "್ವ",mainString);
        mainString=str_replace("_25", "್ಷ",mainString);
        
        mainString=str_replace("_2", "್ನ",mainString);        
        mainString=str_replace("_3", "್ತ",mainString);
        mainString=str_replace("_4", "್ಲ",mainString);
        mainString=str_replace("_5", "್ದ",mainString);
        mainString=str_replace("_6", "್ಣ",mainString);
        mainString=str_replace("_8", "್ಥ",mainString);
        mainString=str_replace("_9", "್ಪ",mainString);
        mainString=str_replace("_10", "್ವ",mainString);
        mainString=str_replace("_11", "ೈ",mainString);
        mainString=str_replace("_12", "್ಮ",mainString);
        mainString=str_replace("_13", "್ಬ",mainString);
        mainString=str_replace("_14", "್ಕ",mainString);
        mainString=str_replace("_15", "್ಟೆ",mainString);
        mainString=str_replace("_16", "್ಚ",mainString);
        mainString=str_replace("_17", "್ರ",mainString);
        mainString=str_replace("_18", "್ಟ",mainString);
        mainString=str_replace("ಗ_19", "ರ್ಗ",mainString);
        mainString=str_replace("ಭ_19", "ರ್ಭ",mainString);
        mainString=str_replace("ಷ_19", "ರ್ಷ",mainString);
        mainString=str_replace("ಡ_19", "ರ್ಡ",mainString);
        mainString=str_replace("ಗು_19", "ರ್ಗು",mainString);
        mainString=str_replace("ಚ_19", "ರ್ಚ",mainString);
        
        
        mainString=str_replace("ೊ", "ೊ",mainString);
        mainString=str_replace("ು್ನ", "್ನು",mainString);
        mainString=str_replace("ೂ್ನ", "್ನೂ",mainString);
        mainString=str_replace("ಾ್ತ", "್ತಾ",mainString);
        mainString=str_replace("ಾ್ಲ", "್ಲಾ",mainString);
        mainString=str_replace("ು್ಣ", "್ಣು",mainString);
        mainString=str_replace("ಿ್ದ", "್ದಿ",mainString);
        mainString=str_replace("ೆ್ದ", "್ದೆ",mainString);
        mainString=str_replace("ೂ್ಲ", "್ಲೂ",mainString);
        mainString=str_replace("ಿ್ವಿ", "್ವಿ",mainString);
        mainString=str_replace("ಿ್ವ", "್ವಿ",mainString);
        mainString=str_replace("ೆ್ಡ", "್ಡೆ",mainString);
        
        
        mainString=str_replace("ಿ್ಬ", "್ಬಿ",mainString);
        mainString=str_replace("ಿ್ಕ", "್ಕಿ",mainString);
        mainString=str_replace("ೆ್ಕ", "್ಕೆ",mainString);
        mainString=str_replace("ಾ್ಚ","್ಚಾ",mainString);
        mainString=str_replace("ಿ್ಚ","್ಚಿ",mainString);
        mainString=str_replace("ಿ್ನ","್ನಿ",mainString);
        mainString=str_replace("ು್ಪ","್ಪು",mainString);
        mainString=str_replace("ಾ್ನ","್ನಾ",mainString);
        mainString=str_replace("ೆ್ತ","್ತೆ",mainString);
        mainString=str_replace("ಿ್ಷ","್ಷಿ",mainString);
        mainString=str_replace("ೆ್ರ","್ರೆ",mainString);
        mainString=str_replace("ೆ್ಯ","್ಯೆ",mainString);
        
        
        
        mainString=str_replace("ಿ್ಲ","್ಲಿ",mainString);
        mainString=str_replace("ೆ್ಲ","್ಲೆ",mainString);
        
        mainString=str_replace("ೆ್ಬ","್ಬೆ",mainString);
        mainString=str_replace("ು್ದ","್ದು",mainString);
        
        mainString=str_replace("ಿ್ಳ", "್ಳಿ",mainString);
        mainString=str_replace("ಿ್ಥ", "್ಥಿ",mainString);
        mainString=str_replace("ೆ್ಟ", "್ಟೆ",mainString);
        mainString=str_replace("ಿ್ತ", "್ತಿ",mainString);
        mainString=str_replace("ೆ್ಲೕ", "್ಲೂ",mainString);
        mainString=str_replace("ು್ಟ", "್ಟು",mainString);
        mainString=str_replace("ು್ಳ", "್ಳು",mainString);
        mainString=str_replace("ಾ್ದ", "್ದಾ",mainString);
        mainString=str_replace("ಾ್ದ", "್ದಾ",mainString);
        mainString=str_replace("ಾ್ಯ", "್ಯಾ",mainString);
        
        mainString=str_replace("ೆ್ದ", "್ದೆ",mainString);
        mainString=str_replace("ಾ್ರ", "್ರಾ",mainString); 
        mainString=str_replace("ಿ್ರ", "್ರಿ",mainString); 
        
        
        mainString=str_replace("ು್ಕ", "್ಕು",mainString);  
        
        mainString=str_replace("ವೊ", "ಮೊ",mainString);  //exeption
        mainString=str_replace("ವೀ", "ಮೀ",mainString);
        
        
        mainString=str_replace("ಿೈ", "ೈ",mainString);
        mainString=str_replace(" ್", "್",mainString);
        mainString=str_replace("ೆೈ", "ೈ",mainString);
        mainString=str_replace("ಿೖ", "ೈ",mainString);
        mainString=str_replace(" ೕ", "ೕ",mainString);
        
        
        
        mainString=str_replace("ೇ", "ೇ",mainString);
        
        mainString=str_replace("ೋ", "ೋ",mainString);
        
        
        mainString=str_replace(" .", ".",mainString);
        mainString=str_replace("  ", " ",mainString);
        mainString=str_replace("''", "\"",mainString);
        
        mainString=str_replace("ು್ತ", "್ತು",mainString);
    }


    //сохраняем ID номер буквы в новый массив. Делим слово на буквы и сохраняем координаты
   /* vector<wordOCR>wordArrayLetter;
    for(int a=0;a<wordArray.size();a++){
        wstring wstr=UTF_to_Unicode(wordArray[a].name);
        for(int t=0;t<wstr.size();t++){
            wordOCR w;
            w=wordArray[a];
            w.name=Unicode_to_UTF(wstr[t]);
            wordArrayLetter.push_back(w);
        }	
    }
    */

    
    //создаем новый массив с результатами применения регулярных выражений
    wstring wStr=UTF_to_Unicode(mainString);
    //cout_<<"mainString1="<<mainString<<endl;
    
    for(int a=0;a<wStr.size();a++){
        wordOCR w;
        w.name=Unicode_to_UTF(wStr[a]);
        lineText.push_back(w);
        //cout_<<w.name;
	}
    //cout_<<"line->wordArray.size()="<<line->wordArray.size()<<" line->wordArrayLetter.size()="<<line->wordArrayLetter.size()<<endl;
    
    //Применяем координаты первоначальной строки к результату грамматической коррекции.
    /*
    int indexStart=0;
    for(int a=0;a<line->wordArray.size();a++){
       
       for(int t=indexStart;t<line->wordArrayLetter.size();t++){
           if(line->wordArray[a].name==line->wordArrayLetter[t].name){
             line->wordArray[a]=line->wordArrayLetter[t];    cout_<<"line->wordArray["<<a<<"].lineIndex="<<line->wordArray[a].lineIndex<<" line->wordArrayLetter["<<t<<"].lineIndex="<<line->wordArrayLetter[t].lineIndex<<endl;
             indexStart++;
             break;
           }else{
             indexStart++;
             break;  
           }    
       }
    }
    */
    

    for(int a=0;a<lineText.size();a++){   //cout_<<"lineText"<<a<<"].lineIndex="<<lineTexta].lineIndex<<endl;
        if(!lineText[a].name.size())continue;
        if(lineText[a].name=="["){   //комментарии в Кангьюре
            if(a+3>lineText.size())continue;
            if(lineText[a+2].name=="]"){
                lineText[a].commentaryIndex="["+lineText[a+1].name+"]";
                lineText[a].name="";
                lineText[a+1].name="";
                lineText[a+2].name="";
            } 
            if(a+4>lineText.size())continue;
            if(lineText[a+3].name=="]"){  
                lineText[a].commentaryIndex="["+lineText[a+1].name+lineText[a+2].name+"]";
                lineText[a].name="";
                lineText[a+1].name="";
                lineText[a+2].name="";
                lineText[a+3].name="";
            
            }
        }else{	
            if(lineText[a]==delimeterTib){ 
                lineText[a].delimeter=lineText[a].name;
                lineText[a].name="";	
            }else{
                if(lineText[a].name=="\n"){  
                    lineText[a].newLine="<br/>\n";
                    lineText[a].name="";	
                }
            }
        }    
	}

}


