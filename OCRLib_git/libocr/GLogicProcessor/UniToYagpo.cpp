#include "GLogicProcessor.h"

string GLogicProcessor::tibetanUTFToYagpo(string &uniStack,int mode){
	
	const int minUniRecord=1;
	const int maxUniRecord=27;
	int i,matchLen;
	string match, resultStr, str;
	short flag, flagExistInMap;
	//int LetterIndex=-1;
	int print=0;
	DR("mode= "<<mode<<" stack count="<<uniStack.size()<<" maxUniRecord"<<maxUniRecord<<" uniStack="<<uniStack<<"/ mainLetterTableUni.size()="<<mainLetterTableUni.size()<<END)

	
	if(uniStack.size()<minUniRecord)return uniStack;
	flagExistInMap=0;
	if(uniStack.size()<maxUniRecord){
		matchLen=uniStack.size();
	}else{
		matchLen=maxUniRecord;
	}	
	map<string, map<string,string> >::const_iterator it;
	while(matchLen>=0){  //from longest match till one letter
		DR(".1")
		if(uniStack.size()>=matchLen){  //if stack is long enouth
			match="";
			for(i=0;i<=matchLen;i++){
				match+=uniStack[i];//fill match string
			}
			DR("matchLen"<<matchLen<<" match=/"<<match<<"/ uniStack="<<uniStack<<"/"<<END) 
			flag=0;
			if(mode==1){it = mainLetterTableUni.find(match);if(it != mainLetterTableUni.end())flag=1;}
			if(mode==2){it = mainLetterTableKey.find(match);if(it != mainLetterTableKey.end())flag=1;}
            if(mode==3){it = mainLetterTableOCRKey.find(match);if(it != mainLetterTableKey.end())flag=1;}

			DR(".2"<<"flag="<<flag) 			
			
			if (flag) {
				flag=1;
				if(mode==2)str=mainLetterTableKey[match]["Wylie"];
                if(mode==3)str=mainLetterTableOCRKey[match]["Wylie"];
				DR("str="<<str<<" n="<<mainLetterTableUni[match]["name"]<<endl)
				if(str=="i"){resultStr+="";flag=0;}
				if(str=="o"){resultStr+="";flag=0;}
				if(str=="e"){resultStr+="";flag=0;}
				if(str=="u"){resultStr+="";flag=0;}
				if(str=="A"){resultStr+="";flag=0;}
				if(str=="I"){resultStr+="";flag=0;}
				if(str=="E"){resultStr+="";flag=0;}
				if(str=="U"){resultStr+="";flag=0;}
				if(str=="O"){resultStr+="";flag=0;}
				if(str=="space"){resultStr+="་";flag=0;}
				
				if(flag){
					if(mode==1)resultStr+=mainLetterTableUni[match]["name"];
					if(mode==2)resultStr+=mainLetterTableKey[match]["name"];
                    if(mode==3)resultStr+=mainLetterTableOCRKey[match]["name"];
				}
				if(mode==1)DR(" match="<<match<<"mainLetterTableUni[match].[\"name\"]="<<mainLetterTableUni[match]["name"]<<" "<<mainLetterTableUni[match]["Wylie"]<<END)
				if(mode==2)DR(" match="<<match<<"mainLetterTableKey[match].[\"name\"]="<<mainLetterTableKey[match]["name"]<<" "<<mainLetterTableKey[match]["Wylie"]<<END)
                if(mode==3)DR(" match="<<match<<"mainLetterTableKey[match].[\"name\"]="<<mainLetterTableKey[match]["name"]<<" "<<mainLetterTableKey[match]["Wylie"]<<END)
				uniStack.erase(0,matchLen+1);
				if(uniStack.size()==0){break;}
				if(uniStack.size()<=maxUniRecord){
					matchLen=(int)uniStack.size();
				}else{
					matchLen=maxUniRecord;
				}
				
			}else{
				if(matchLen==0){
					resultStr+=match;
					DR(" one letter match=/"<<match<<"/"<<END)
					uniStack.erase(0,matchLen+1);
					if(uniStack.size()==0){break;}
					if(uniStack.size()<=maxUniRecord){
						matchLen=(int)uniStack.size();
					}else{
						matchLen=maxUniRecord;
					}
				}	
			}
			DR(" matchLen="<<matchLen<<"/ resultStr=/"<<resultStr<<"/"<<END)
			DR(".3")
			
		}//if(uniStackIndex
		DR(".4")
		matchLen--;
    }//while(matchLen>=0)
	
	DR("resultStr=/"<<resultStr<<"/"<<END)
	return resultStr;
}//_______________________________________________________________________________________

string GLogicProcessor::SinhalaUniToYagpo(string &uniStack,int mode){
	
	const int minUniRecord=1;
	const int maxUniRecord=27;
	int matchLen;
	string match, resultStr, str;
	short flag, flagExistInMap;
	//int LetterIndex=-1;
	int print=0;
	DR("mode= "<<mode<<" stack count="<<uniStack.size()<<" maxUniRecord"<<maxUniRecord<<" uniStack="<<uniStack<<"/ mainLetterTableUni.size()="<<mainLetterTableUni.size()<<END)
	
	
	if(uniStack.size()<minUniRecord)return uniStack;
	flagExistInMap=0;
	if(uniStack.size()<maxUniRecord){
		matchLen=(int)uniStack.size();
	}else{
		matchLen=maxUniRecord;
	}	
	map<string, map<string,string> >::const_iterator it;
	while(matchLen>=0){  //from longest match till one letter
		DR(".1")
		if(uniStack.size()>=matchLen){  //if stack is long enouth
			match=uniStack.substr(0,matchLen+1);//fill match string
			DR("matchLen"<<matchLen<<" match=/"<<match<<"/ uniStack="<<uniStack<<"/ size="<<uniStack.size()<<END);   //sleep(1)
			flag=0;
			if(mode==1){it = mainLetterTableUni.find(match);if(it != mainLetterTableUni.end())flag=1;}
			if(mode==2){it = mainLetterTableKey.find(match);if(it != mainLetterTableKey.end())flag=1;}
			DR(".2"<<"flag="<<flag)
			if (flag) { 
				if(mode==1)resultStr+=mainLetterTableUni[match]["name"];
				if(mode==2)resultStr+=mainLetterTableKey[match]["name"];
				//if(mode==1)DR(" match="<<match<<"mainLetterTableUni[match].[\"name\"]="<<mainLetterTableUni[match]["name"]<<END)
				//if(mode==2)DR(" match="<<match<<"mainLetterTableKey[match].[\"name\"]="<<mainLetterTableKey[match]["name"]<<END)
				//cout<<"start erase matchLen="<<matchLen+1<<" uniStack.size()="<<uniStack.size()<<endl; 
				if(matchLen+1<uniStack.size()){
					uniStack=uniStack.substr(matchLen+1);
				}else { //cout<<" break ";
					break;
				}
				if(uniStack.size()<=maxUniRecord){
					matchLen=uniStack.size();
				}else{
					matchLen=maxUniRecord;
				}
				DR(".20"<<"flag="<<flag)  
				
			}else{   //cout<<" NO FOUND matchLen="<<matchLen;
				if(matchLen==0){
					resultStr+=match;
					DR(" one letter match=/"<<match<<"/"<<END)
					if(matchLen+1<uniStack.size()){
						uniStack=uniStack.substr(matchLen+1);
					}else {
						break;
					}
					if(uniStack.size()<=maxUniRecord){
						matchLen=uniStack.size();
					}else{
						matchLen=maxUniRecord;
					}
				}	
				DR(" _21"<<"flag="<<flag)   
			}
			DR(" matchLen="<<matchLen<<"/ resultStr=/"<<resultStr<<"/"<<END)
			DR(".3")
			
		}//if(uniStackIndex
		DR(".4")
		matchLen--;
    }//while(matchLen>=0)
	
	DR("resultStr=/"<<resultStr<<"/"<<END); //sleep(1)
	return resultStr;
}//_______________________________________________________________________________________


//deprecated
string GLogicProcessor::UniToTibetan(string &uniStack, int YagpoWylieFontFlag){
	
	const int minUniRecord=1;
	const int maxUniRecord=27;
	unsigned int i,matchFlag;
	int matchLen;
	//cout<<"stack count="<<uniStack.size()<<" maxUniRecord"<<maxUniRecord<<" uniStack[0]="<<(short)uniStack[0]<<END;
	string match, resultStr, wstr;
	string str;
	string test;
	short flagExistInMap;//maxLetterSize=0;
	//int LetterIndex=-1;
	map<string, uniRecord>::const_iterator it;
	map<string, string>::const_iterator index;
	
	//for(i=0;i<uniStack.size();i++)cout<<"uniStack["<<i<<"]="<<(short)uniStack[i]<<hex<<END;
	//cout<<" test="<<uniStack<<END;
	
	
	if(uniStack.size()<minUniRecord)return uniStack;
	flagExistInMap=0;
	matchLen=maxUniRecord;
	while(matchLen>=0){  //from longest match till one letter
		//cout<<".";
		if(uniStack.size()>=matchLen){  //if stack is long enouth
			match="";
			for(i=0;i<=matchLen;i++)match+=uniStack[i];//fill match string
			
			//for(int s=0;s<match.size();s++)cout<<"match["<<s<<"]="<<hex<<match[s]<<END;
			//cout<<"matchLen1="<<matchLen<<" match="<<match.size()<<" uniStack.size="<<uniStack.size()<<" match="<<match<<END;
			matchFlag=0;
			if(YagpoWylieFontFlag){
				if (uniTibTableYagpo.find(match) != uniTibTableYagpo.end()){matchFlag=1;
					resultStr+=uniTibTableYagpo[match].letterUTF;
					//cout<<"uniTibTableYagpo[match].letterUTF="<<uniTibTableYagpo[match].letterUTF<<END;
				}
			}else{
				if (uniTibTable.find(match) != uniTibTable.end()){matchFlag=1;
					resultStr+=uniTibTable[match].letterUTF;
					//cout<<"uniTibTable[match].letterUTF="<<uniTibTable[match].letterUTF<<END;
				}				
			}
			
			
			if(matchFlag){
				//str=uniTibTable[match].Wylie[0];
				//if(str=="i"){resultStr+="";flag=0;}	//if(str=="o"){resultStr+="";flag=0;}	//if(str=="e"){resultStr+="";flag=0;}	//if(str=="u"){resultStr+="";flag=0;}
				//if(str=="A"){resultStr+="";flag=0;}	//if(str=="I"){resultStr+="";flag=0;} //if(str=="E"){resultStr+="";flag=0;}	//if(str=="U"){resultStr+="";flag=0;}
				//if(str=="O"){resultStr+="";flag=0;}
				
				//cout<<" match="<<match.c_str()<<"uniTibTable[match].letterUni="<<Unicode_to_UTF(uniTibTable[match].letterUni)<<" "<<uniTibTable[match].Wylie[0]<<END;
				uniStack.erase(0,matchLen+1);
				if(uniStack.size()==0){break;}
				if(uniStack.size()<=maxUniRecord){
					matchLen=uniStack.size();
				}else{
					matchLen=maxUniRecord;
				}
				//cout<<" uniStack.size()="<<uniStack.size()<<" uniStack="<<uniStack<<END;
			}
		}//if(uniStackIndex
		matchLen--;
	}//while(matchLen>6)
	
	string txt=resultStr;
   	if(YagpoWylieFontFlag){
		txt=str_replace("","",txt);
		txt=str_replace("ོ","",txt);
		txt=str_replace("ེ","",txt);
		txt=str_replace("ི","",txt);
		txt=str_replace("ུ","",txt);
		txt=str_replace("ཱ","",txt);
		txt=str_replace("ཱི","",txt);
		txt=str_replace("཰","",txt);
		txt=str_replace("ཱུ","",txt);
		txt=str_replace("཯","",txt);
	txt=str_replace("࿎","ༀ",txt);	}
	
	//cout<<" resultStr.size="<<resultStr.size()<<" "<<resultStr<<END;
	return txt;
}//_______________________________________________________________________________________


string GLogicProcessor::YagpoToUni(string &srcStr){
	
	//const int minUniRecord=1;
	//const int maxUniRecord=27;
	unsigned int i;
	int print=0;
	//cout<<"srcStr count="<<srcStr.size()<<" maxUniRecord"<<maxUniRecord<<" srcStr="<<srcStr<<END;
	string match, resultStr, str;
	string letter,result,destStr,needConvertToUni;
	wstring srcLineUni,testChar;
	
	srcLineUni=UTF_to_Unicode(srcStr);
	result="";
	for(i=0;i<srcLineUni.size();i++){
		testChar=srcLineUni[i];
		letter=Unicode_to_UTF(testChar);
		//cout<<" letter="<<letter<<" testChar="<<hex<<testChar[0]<<END;
		if(testChar==L"\r"){result+="\r"; continue;}
		//if((int)srcLine[i]==-32||(int)yagpoStrVect[i][0]==-18){ //tibetan unicode range
        destStr=mainLetterTableYagpo[letter]["tibUniUTF"];
		//cout<<"letter= "<<letter<<" destString="<<destStr<<END;
		if(destStr!=""){
			result+=destStr;
		}else{
			//cout<<" needConvertToUni "<<hex<<testChar[0]<<endl;
			result+=Unicode_to_UTF(testChar);
			needConvertToUni+=Unicode_to_UTF(testChar)+"|\n";
		}
		
	}
	
	//cout<<result<<END;
	
	
	if(needConvertToUni!=""){
		DR("need Convert To Uni \n"<<needConvertToUni)
	}
	
	
	//cout<<"resultStr="<<resultStr<<END;
	return result;
}//_______________________________________________________________________________________

bool GLogicProcessor::isDelimeter(string &str){
    for(int i=0;i<delimeterTib.size();i++){
        if(str==delimeterTib[i]||str=="\n")return true;
    }
    return false;
}

//apply regular expression list and check tibetan text by OCR phrase dictionary probability /
string GLogicProcessor::lineTibetanCorrector(string &srcLine){
    vector<string>srcVect;
    textNormalisation(srcLine);   //применение регулярных выражений
    
    vector<OCRMatch>pageText;
    OCRMatch match;
    match.correlation=100;
    pageText.push_back(match);
    
    //проверка по словарю статистической устойчивости распознанного текста
    //cout<<"pageStr1="<<pageStr<<endl;
    //textNormalisation(pageStr);
    //cout<<"pageStr="<<pageStr<<endl;
    
    
    wstring strW=UTF_to_Unicode(srcLine);
    
    for(int n=0;n<strW.size();n++){
        string s=Unicode_to_UTF(strW[n]);
        if(isDelimeter(s)||s==" "){
            pageText[pageText.size()-1].delimeter+=s;
        }else{
            OCRMatch match;
            match.correlation=100;
            match.name=s;
            pageText.push_back(match);
        }
    }
    
    string result;
    textCorrector(pageText, result);
    result=YagpoToUni(result);
    //cout<<endl<<"@@@@result1="<<result;
    //result=str_replace("\n", "<br>\n", result);
    //writeText(result,"/_Image2OCR/11.html"); exit(0);

    return result;
}

//apply regular expression list and check tibetan text by OCR phrase dictionary probability /
void GLogicProcessor::TibetanCorrector(){
    
    string fileName;
    int print=0;
    
    ofstream c_out;  c_out.open(inputData.data["outFile"].c_str());
    if( !c_out ){
        cout<<"destination path "<<inputData.data["outFile"]<<" not exist"<<endl;
        exit(10);
    }
    vector<string>srcVect;
    string srcLine;
    readText(srcLine, inputData.data["inputFile"]);
    
    textNormalisation(srcLine);   //применение регулярных выражений
    
    srcVect=explode("{P:", srcLine);
    DR("stcVect.size="<<srcVect.size()<<endl)
    string mainString="";
    
    for(int i=0;i<srcVect.size();i++){
        //if(i>5)break;
        vector<OCRMatch>pageText;
        OCRMatch match;
        match.correlation=100;
        pageText.push_back(match);
        cout<<".";
        
        //проверка по словарю статистической устойчивости распознанного текста
            string pageStr="{P:"+srcVect[i];
            //cout<<"pageStr1="<<pageStr<<endl;
            //textNormalisation(pageStr);
            //cout<<"pageStr="<<pageStr<<endl;

        
            wstring strW=UTF_to_Unicode(pageStr);
        
            for(int n=0;n<strW.size();n++){
                string s=Unicode_to_UTF(strW[n]);
                if(isDelimeter(s)||s==" "){
                    pageText[pageText.size()-1].delimeter+=s;
                }else{
                    OCRMatch match;
                    match.correlation=100;
                    match.name=s;
                    pageText.push_back(match);
                }
            }
        
            string result;
            //textCorpusGMap->associativeSignalProcessing(result);
        
            textCorrector(pageText, result);
        
            result=YagpoToUni(result);

        
            //cout<<endl<<"@@@@result1="<<result;
            result=str_replace("\n", "<br>\n", result);
            //writeText(result,"/_Image2OCR/11.html"); exit(0);
            mainString+=result+"\n<br>";
        
    }
    mainString="<html><head><meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\"><title>www.dharmabook.ru</title>"+
        mainString+"</body></html>";
    c_out<<mainString;
    c_out.close();
    
}//_______________________________________________________________________________________

string GLogicProcessor::lineTranscription(string &srcLine){
    string line;
    string destStr;
    string result;
    string mainString;
    string needConvertToWylie;
    vector<string>srcVect=explode("\n",srcLine);

    for(int j=0;j<srcVect.size();j++){
        
        srcVect[j]=str_replace("\r","",srcVect[j]);
        string src=srcVect[j];
        mainString+=srcVect[j]+"\n";
        
        if(src.find("་")==-1){
            continue;
        }
        destStr="";
        result="";
        line=srcVect[j];
        
        line=str_replace("་"," ",line);
        line=str_replace("།"," ",line);
        line=str_replace("༽"," ",line);
        line=str_replace("༼"," ",line);
        line=str_replace("༏"," ",line);
        line=str_replace("༴"," ",line);
        line=str_replace("༄"," ",line);
        line=str_replace("༅"," ",line);
        line=str_replace("༈"," ",line);
        line=str_replace("༿"," ",line);
        line=str_replace("༾"," ",line);
        line=str_replace("༔"," ",line);
        line=str_replace("ཿ"," ",line);
        line=str_replace("["," ",line);
        line=str_replace("]"," ",line);
        line=str_replace("{"," ",line);
        line=str_replace(")"," ",line);
        line=str_replace("("," ",line);
        line=str_replace(")"," ",line);
        line=str_replace("<"," ",line);
        line=str_replace(">"," ",line);
        line=str_replace("."," ",line);
        line=str_replace(","," ",line);
        line=str_replace(";"," ",line);
        line=str_replace(":"," ",line);
        line=str_replace("#"," ",line);
        line=str_replace("|"," ",line);
        vector<string>UniStrVect=explode(" ",line);
        
        for(int i=0;i<UniStrVect.size();i++){
            if(!UniStrVect[i].size())continue;
            destStr=YagpoToWylieMap[UniStrVect[i]].translit;
            //DR(i<<"= "<<UniStrVect[i]<<" destString="<<destStr.c_str()<<endl)
            if(destStr!=""){
                result+=destStr+" ";
            }else{
                result+=UniStrVect[i]+" ";
                needConvertToWylie+=UniStrVect[i];
                needConvertToWylie+="|<br>\n";
            }
            //}
            
        }
        mainString+=result+"\n";
    }
    
    if(needConvertToWylie!="")mainString+=" Need convert to Wylie "+needConvertToWylie;
    
    return mainString;

}

//convert string to phonetic transtiption
void GLogicProcessor::Transcription(){
    
	string fileName;
	int i,j;
    int print=0;
	
	ofstream c_out;  c_out.open(inputData.data["outFile"].c_str());
	if( !c_out ){
		cout<<"destination path "<<inputData.data["outFile"]<<" not exist"<<endl;
		exit(10);
	}
	vector<string>srcVect;
	string srcLine;
    readText(srcLine, inputData.data["inputFile"]);
    srcVect=explode(" ", srcLine);
	DR("stcVect.size="<<srcVect.size()<<endl)
    
    vector<string>tibStr;
	vector<string>wylieStr;
	vector<string>wylieStrVect;
	vector<string>UniStrVect;
    
	loadTransliterationFile("TranslitTableUni_Wylie.xml");
	string destStr;
	string destLine;
	string delimeter;
	string needConvertToWylie="";
	//string::size_type pos = 0;
	string src;
	//wcout<<srcVect[0].c_str()<<L" delimeter="<<delimeter.c_str()<<endl;
    
    
	for(j=0;j<srcVect.size();j++){
		DR("j="<<j<<" srcVect[j]="<<srcVect[j]<<endl)
        
		srcVect[j]=str_replace("\r","",srcVect[j]);
		src=srcVect[j];
		srcLine=srcVect[j]; destLine="";
		srcLine=str_replace("་"," ",srcLine);
		srcLine=str_replace("།"," ",srcLine);
		srcLine=str_replace("༽"," ",srcLine);
		srcLine=str_replace("༼"," ",srcLine);
		srcLine=str_replace("༏"," ",srcLine);
		srcLine=str_replace("༴"," ",srcLine);
		srcLine=str_replace("༄"," ",srcLine);
		srcLine=str_replace("༅"," ",srcLine);
		srcLine=str_replace("༈"," ",srcLine);
		srcLine=str_replace("༿"," ",srcLine);
		srcLine=str_replace("༾"," ",srcLine);
		srcLine=str_replace("༔"," ",srcLine);
		srcLine=str_replace("ཿ"," ",srcLine);
		srcLine=str_replace("["," ",srcLine);
		srcLine=str_replace("]"," ",srcLine);
		srcLine=str_replace("{"," ",srcLine);
		srcLine=str_replace(")"," ",srcLine);
		srcLine=str_replace("("," ",srcLine);
		srcLine=str_replace(")"," ",srcLine);
		srcLine=str_replace("<"," ",srcLine);
		srcLine=str_replace(">"," ",srcLine);
		srcLine=str_replace("."," ",srcLine);
		srcLine=str_replace(","," ",srcLine);
		srcLine=str_replace(";"," ",srcLine);
		srcLine=str_replace(":"," ",srcLine);
		srcLine=str_replace("#"," ",srcLine);
		srcLine=str_replace("|"," ",srcLine);
        
		DR("SRC"<<srcLine<<endl)
        if(!srcLine.size())continue;
        
		UniStrVect=explode(" ",srcLine);
        
        DR("size="<<UniStrVect.size()<<endl)
        string result;
       
		for(i=0;i<UniStrVect.size();i++){
                if(!UniStrVect[i].size())continue;
				destStr=YagpoToWylieMap[UniStrVect[i]].translit;
				DR(i<<"= "<<UniStrVect[i]<<" destString="<<destStr.c_str()<<endl)
				if(destStr!=""){
					result+=destStr+" ";
				}else{
                    result+=UniStrVect[i]+" ";
					needConvertToWylie+=UniStrVect[i];
					needConvertToWylie+="|<br>\n";
				}
			//}
			DR("DEST____"<<result<<endl)
		}
        
		c_out<<src<<"<br>\n"<<result<<"<br>\n"<<endl;
		//cout<<"DEST"<<srcVect[j]<<endl;
        
	}
    
	if(needConvertToWylie!="")c_out<<"need Convert To Wylie <br>\n"<<needConvertToWylie;
    c_out.close();
    
}//_______________________________________________________________________________________


