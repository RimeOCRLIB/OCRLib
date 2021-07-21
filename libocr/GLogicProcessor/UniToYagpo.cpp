
#include "GLogicProcessor.h"
#include "../GMemory/GMemory.h"

/** @brief encode all data in vData by words ID. All remaining letters will be encoded as uint*/
void GLogicProcessor::encodeByID(string &vData){

    string str="WORDLIST";
    GMemory *longMemory=((GMemory*)inputData.longMemory);
    longMemory->loadTable(str);
    indexRecord *indRec=longMemory->createIndex(longMemory->table[str],0,ID_SEARCH_DATA);
    indRec->mIndex->encodeID(vData);
}


string GLogicProcessor::tibetanUTFToYagpo(string &uniStack,int mode){
	
	const int minUniRecord=1;
	const int maxUniRecord=27;
	uint i,matchLen;
	string match, resultStr, str;
	short flag, flagExistInMap;
	//int LetterIndex=-1;
	int print=0;
	DR("mode= "<<mode<<" stack count="<<uniStack.size()<<" maxUniRecord"<<maxUniRecord<<" uniStack="<<uniStack<<"/ mainLetterTableUni.size()="<<mainLetterTableUni.size()<<END)
    if(!mainLetterTableUni.size()){
        cout<<"need load font map"<<endl;
        return resultStr;
    }
    
    
	if(uniStack.size()<minUniRecord)return uniStack;
	flagExistInMap=0;
	if(uniStack.size()<maxUniRecord){
		matchLen=(uint)uniStack.size();
	}else{
		matchLen=maxUniRecord;
	}	
	map<string, uniRecord >::const_iterator it;
	while(matchLen>0){  //from longest match till one letter
		DR(".1")
		if(uniStack.size()>=matchLen){  //if stack is long enouth
			match="";
			for(i=0;i<=matchLen;i++){
				match+=uniStack[i];//fill match string
			}
			DR("matchLen"<<matchLen<<" match=/"<<match<<"/ uniStack="<<uniStack<<"/"<<END) 
			flag=0;
            
            
            if(mode==1)if(mainLetterTableUni.find(match)!= mainLetterTableUni.end())flag=1;
            if(mode==2)if(mainLetterTableKey.find(match) != mainLetterTableKey.end())flag=1;
            if(mode==3)if(mainLetterTableOCRKey.find(match) != mainLetterTableKey.end())flag=1;
                
            if(mode==2&&flag==0&&match.size()>3){ //стираем последнюю гласную А
                //cout<<" match.size():"<<match.size()<<" "<<match.find("ཨ")<<endl;
                if(match.find("•")==match.size()-3){
                    match.resize(match.size()-3);
                    it = mainLetterTableKey.find(match);if(it != mainLetterTableKey.end())flag=1;
                }
            }
            //cout<<match<<" "<<flag<<endl;

			DR(".2"<<"flag="<<flag) 			
			
			if (flag) {

				if(flag){
					if(mode==1)resultStr+=mainLetterTableUni[match].letterYagpoUTF;
					if(mode==2)resultStr+=mainLetterTableKey[match].letterUniUTF;
                    if(mode==3)resultStr+=mainLetterTableOCRKey[match].letterUniUTF;
                }
				if(mode==1)DR(" match="<<match<<"mainLetterTableUni[match].letterYagpoUTF="<<mainLetterTableUni[match].letterYagpoUTF<<" "<<mainLetterTableUni[match].Wylie<<END)
				if(mode==2)DR(" match="<<match<<"mainLetterTableKey[match].letterYagpoUTF="<<mainLetterTableKey[match].letterYagpoUTF<<" "<<mainLetterTableKey[match].Wylie<<END)
                if(mode==3)DR(" match="<<match<<"mainLetterTableKey[match].letterYagpoUTF="<<mainLetterTableKey[match].letterYagpoUTF<<" "<<mainLetterTableKey[match].Wylie<<END)
				uniStack.erase(0,matchLen+1);
				if(uniStack.size()==0){break;}
				if(uniStack.size()<=maxUniRecord){
					matchLen=(int)uniStack.size();
				}else{
					matchLen=maxUniRecord;
				}
				
			}else{
				if(matchLen==1){
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
	map<string, uniRecord >::const_iterator it;
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
				if(mode==1)resultStr+=mainLetterTableUni[match].letterYagpoUTF;
				if(mode==2)resultStr+=mainLetterTableKey[match].letterYagpoUTF;
				//if(mode==1)DR(" match="<<match<<"mainLetterTableUni[match].[\"name\"]="<<mainLetterTableUni[match]["name"]<<END)
				//if(mode==2)DR(" match="<<match<<"mainLetterTableKey[match].[\"name\"]="<<mainLetterTableKey[match]["name"]<<END)
				//cout<<"start erase matchLen="<<matchLen+1<<" uniStack.size()="<<uniStack.size()<<endl; 
				if(matchLen+1<uniStack.size()){
					uniStack=uniStack.substr(matchLen+1);
				}else { //cout<<" break ";
					break;
				}
				if(uniStack.size()<=maxUniRecord){
					matchLen=(int)uniStack.size();
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
						matchLen=(int)uniStack.size();
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
	//cout<<"stack count="<<uniStack.size()<<" maxUniRecord"<<maxUniRecord<<" uniStack[0]="<<(short)uniStack[0]<<endl;
	string match, resultStr, wstr;
	string str;
	string test;
	short flagExistInMap;//maxLetterSize=0;
	//int LetterIndex=-1;
	map<string, uniRecord>::const_iterator it;
	map<string, string>::const_iterator index;
	
	//for(i=0;i<uniStack.size();i++)cout<<"uniStack["<<i<<"]="<<(short)uniStack[i]<<hex<<endl;
	//cout<<" test="<<uniStack<<endl;
	
	
	if(uniStack.size()<minUniRecord)return uniStack;
	flagExistInMap=0;
	matchLen=maxUniRecord;
	while(matchLen>=0){  //from longest match till one letter
		//cout<<".";
		if(uniStack.size()>=matchLen){  //if stack is long enouth
			match="";
			for(i=0;i<=matchLen;i++)match+=uniStack[i];//fill match string
			
			//for(int s=0;s<match.size();s++)cout<<"match["<<s<<"]="<<hex<<match[s]<<endl;
			//cout<<"matchLen1="<<matchLen<<" match="<<match.size()<<" uniStack.size="<<uniStack.size()<<" match="<<match<<endl;
			matchFlag=0;
			if(YagpoWylieFontFlag){
				if (mainLetterTableYagpo.find(match) != mainLetterTableYagpo.end()){matchFlag=1;
					resultStr+=mainLetterTableYagpo[match].letterYagpoUTF;
					//cout<<"mainLetterTableYagpo[match].letterUTF="<<mainLetterTableYagpo[match].letterUTF<<endl;
				}
			}else{
				if (mainLetterTableUni.find(match) != mainLetterTableUni.end()){matchFlag=1;
					resultStr+=mainLetterTableUni[match].letterYagpoUTF;
					//cout<<"mainLetterTableUni[match].letterUTF="<<mainLetterTableUni[match].letterUTF<<endl;
				}				
			}
			
			
			if(matchFlag){
				//str=mainLetterTableUni[match].Wylie[0];
				//if(str=="i"){resultStr+="";flag=0;}	//if(str=="o"){resultStr+="";flag=0;}	//if(str=="e"){resultStr+="";flag=0;}	//if(str=="u"){resultStr+="";flag=0;}
				//if(str=="A"){resultStr+="";flag=0;}	//if(str=="I"){resultStr+="";flag=0;} //if(str=="E"){resultStr+="";flag=0;}	//if(str=="U"){resultStr+="";flag=0;}
				//if(str=="O"){resultStr+="";flag=0;}
				
				//cout<<" match="<<match.c_str()<<"mainLetterTableUni[match].letterUni="<<Unicode_to_UTF(mainLetterTableUni[match].letterUni)<<" "<<mainLetterTableUni[match].Wylie[0]<<endl;
				uniStack.erase(0,matchLen+1);
				if(uniStack.size()==0){break;}
				if(uniStack.size()<=maxUniRecord){
					matchLen=(int)uniStack.size();
				}else{
					matchLen=maxUniRecord;
				}
				//cout<<" uniStack.size()="<<uniStack.size()<<" uniStack="<<uniStack<<endl;
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
	
	//cout<<" resultStr.size="<<resultStr.size()<<" "<<resultStr<<endl;
	return txt;
}//_______________________________________________________________________________________


string GLogicProcessor::YagpoToUni(string &srcStr){
	
	//const int minUniRecord=1;
	//const int maxUniRecord=27;
	unsigned int i;
	int print=0;
	//cout<<"srcStr count="<<srcStr.size()<<" maxUniRecord"<<maxUniRecord<<" srcStr="<<srcStr<<endl;
	string match, resultStr, str;
	string letter,result,destStr,needConvertToUni;
	wstring srcLineUni,testChar;
	
	srcLineUni=UTF_to_Unicode(srcStr);
	result="";
	for(i=0;i<srcLineUni.size();i++){
		testChar=srcLineUni[i];
		letter=Unicode_to_UTF(testChar);
		//cout<<" letter="<<letter<<" testChar="<<hex<<testChar[0]<<endl;
		if(testChar==L"\r"){result+="\r"; continue;}
		//if((int)srcLine[i]==-32||(int)yagpoStrVect[i][0]==-18){ //tibetan unicode range
        destStr=mainLetterTableYagpo[letter].letterUniUTF;
		//cout<<"letter= "<<letter<<" destString="<<destStr<<endl;
		if(destStr!=""){
			result+=destStr;
		}else{
			//cout<<" needConvertToUni "<<hex<<testChar[0]<<endl;
			result+=Unicode_to_UTF(testChar);
			needConvertToUni+=Unicode_to_UTF(testChar)+"|\n";
		}
		
	}
	
	//cout<<result<<endl;
	
	
	if(needConvertToUni!=""){
		DR("need Convert To Uni \n"<<needConvertToUni)
	}
    srcStr=result;
	
	//cout<<"resultStr="<<resultStr<<endl;
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
    srcLine=UnicodeToYagpo(srcLine);
    
    
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
    
    explode("{P:", srcLine, srcVect);
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

string GLogicProcessor::lineTranscription(string &srcLine, int mode){
    string line;
    string destStr;
    string result;
    string mainString;
    string needConvertToWylie;
    vector<string>srcVect;
    explode("\n",srcLine,srcVect);
    if(!UniToWylieMap.size())LoadUniToWylieMap();

    for(int i=0;i<srcVect.size();i++){
        
        string src=srcVect[i];
        src=str_replace("\r","",src);
        if(src.find("་")==-1){
            mainString+=src+"<br/>\n";
            continue;
        }
        
        vector<string>v;
        explode(" ",src,v);
        
        for(int j=0;j<v.size();j++){
            src=v[j];
            mainString+=src+"<br/>\n";
            
            destStr="";
            result="";
            line=src;
            
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
            line=str_replace("༑"," ",line);
            vector<string>UniStrVect;
            explode(" ",line, UniStrVect);
            
            if(mode){
                for(int i=0;i<UniStrVect.size();i++){
                    if(!UniStrVect[i].size())continue;
                        destStr=UniToWylieMap[UniStrVect[i]].translitEng;
                    //DR(i<<"= "<<UniStrVect[i]<<" destString="<<destStr.c_str()<<endl)
                    if(destStr!=""){
                        result+=destStr+" ";
                    }else{
                        result+=UniStrVect[i]+" ";
                        needConvertToWylie+=UniStrVect[i];
                        needConvertToWylie+="|<br/>\n";
                    }
                }
            }else{
                for(int i=0;i<UniStrVect.size();i++){
                    if(!UniStrVect[i].size())continue;
                    destStr=UniToWylieMap[UniStrVect[i]].translitRus;
                    //DR(i<<"= "<<UniStrVect[i]<<" destString="<<destStr.c_str()<<endl)
                    if(destStr!=""){
                        result+=destStr+" ";
                    }else{
                        result+=UniStrVect[i]+" ";
                        needConvertToWylie+=UniStrVect[i];
                        needConvertToWylie+="|<br/>\n";
                    }
                }
            }
            mainString+=result+"<br/>\n";
        }
    }
    
    if(needConvertToWylie!="")mainString+=" Need convert to Wylie <br/>\n"+needConvertToWylie;
    
    srcLine=mainString;
    return mainString;

}

