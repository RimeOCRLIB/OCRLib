#include "GLogicProcessor.h"

string GLogicProcessor::WylieToYagpoConverter(string &inStr){
	string match,destStr,result;
	//cout<<"in="<<line<<endl;
	for(int i=0;i<inStr.size();i++){
		match=inStr[i];
		if(ASCIToUni[match].size()){
		   destStr+=ASCIToUni[match];
		}else{
		   destStr+=inStr[i];
		}	
		//cout<<(short)inStr[i]<<" "<<inStr[i]<<" "<<destStr<<endl;
		
	}
	return tibetanUTFToYagpo(destStr,2);
}//_____________________________________________________________________


string GLogicProcessor::dWylieToYagpoConverter(string &inStr){
	string destStr;
	string match;
	string needConvertToWylie="";
	int print=0;
	string result;
	string startUTF="{";
	string endUTF="}";
	
	DR("DEST"<<inStr<<endl);
	
	short startTibetan, endTibetan;
		//inStr=str_replace("\r","",inStr);
		startTibetan=0, endTibetan=0;
        destStr="";		
		for(int i=0;i<inStr.size();i++){
			match=inStr[i]; 
			if(match==endUTF){
				startTibetan=0;
				endTibetan=1;  DR("destStr="<<destStr<<endl);
				result+=startUTF+tibetanUTFToYagpo(destStr,2)+endUTF;
				DR("end tibetan result"<<result<<endl);
				destStr="";
				
			}
			
			if(startTibetan==1){
				DR("startT1 i="<<i<<" match1="<<endl);
				destStr+=ASCIToUni[match];
				DR(" start/"<<destStr<<endl);
			}
			
			if(match==startUTF&&endTibetan==0){startTibetan=1;endTibetan=0;
				result+=destStr;
				DR("start i="<<i<<" result="<<destStr.size()<<endl);
				destStr="";
			}
			if(startTibetan==0){
				if(endTibetan!=1){
					destStr+=match;
				}else{endTibetan=0;}
			}
			DR("i="<<i<<" /"<<result<<endl);
			
			
		}
		DR("//__"<<destStr<<"="<<destStr.size()<<endl);
		result+=destStr;
		//cout<<" result="<<result<<endl;
		//if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
		//result="";
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
}//_______________________________________________________________________________________



string GLogicProcessor::UnicodeToYagpo(string &srcStr){
	string destStr;
	string needConvertToWylie="";
	//string::size_type pos = 0;
	string result;
	int print=0;
	//cout<<"DEST"<<inStr<<endl;
    if(!mainLetterTableUni.size()){
        cout<<"load font table"<<endl;
        LoadMapXML();
    }
    
	short startTibetan, endTibetan;
	//inStr=str_replace("\r","",inStr);
	//c_out<<"res="<<Unicode_to_UTF(srcLineUni)<<endl;
	startTibetan=0, endTibetan=0;
	destStr="";		
	for(int i=0;i<srcStr.size();i++){
		DR("srcStr["<<i<<"]="<<(short)srcStr[i]<<"/"<<srcStr[i]<<endl);
		
		if((short)srcStr[i]>0&&startTibetan==1){
			startTibetan=0;
			endTibetan=1;  DR("destStr="<<destStr<<endl);
			result+=tibetanUTFToYagpo(destStr,1);
			result+=srcStr[i];
			DR("end tibetan result"<<result<<endl);
			destStr="";
			
		}
		if((short)srcStr[i]<0){endTibetan=0;
			//if((short)srcStr[i]==-96||(short)srcStr[i]==-62)continue;  //tabulation
			DR("start i="<<i<<" result="<<result<<endl);
			if(startTibetan==0){
				result+=destStr;
				destStr=srcStr[i];
				startTibetan=1;
			}else{
			    destStr+=srcStr[i];
			}	
			DR(" @tib/"<<destStr<<endl);
		}
		
		if(startTibetan==0){
			if(endTibetan!=1){
				destStr+=srcStr[i];
			}else{endTibetan=0;}
		}
        DR("endTibetan="<<endTibetan<<" startTibetan="<<startTibetan<<endl);
		DR("i="<<i<<"result=/"<<result<<"/ destStr=/"<<destStr<<"/"<<endl);
		
		
	}
	//cout<<"//__"<<destStr.size()<<endl;
	if(startTibetan){
		result+=tibetanUTFToYagpo(destStr,1);
	}else{
	    result+=destStr;
	}	
	DR(" result="<<result<<endl);
	//if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
	//result="";
	
	
	
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
};


string GLogicProcessor::SinghalaASCIToYagpo(string &uniStack){
	///cout<<"uniStack="<<uniStack.size()<<endl; 
	string str=uniStack,strSrc,strDest;
	map<string, string>::iterator p; 
	if(SinhalaASCI.size()){
		for(p = SinhalaASCI.begin(); p != SinhalaASCI.end(); p++) {
			strSrc=p->first;
			strDest= p->second;  
			str=str_replace(strSrc,strDest, str);
            //cout<<"strSrc="<<strSrc<<" strDest="<<strDest<<" str="<<str<<endl;
		}
	}	
   	///cout<<"str="<<str<<endl;  
	return SinhalaUniToYagpo(str,1);;
}//_______________________________________________________________________________________



string GLogicProcessor::dSinhalaASCIToYagpo(string &inStr){
	string destStr,uniStack;
	string match,matchStr;
	string needConvertToWylie="";
	//string::size_type pos = 0;
	string result;
	string startUTF="༼";
	string endUTF="༽"; 
	
	short startTibetan, endTibetan;
	
	//inStr=str_replace("\r","",inStr);
	//cout<<"inStr="<<inStr<<endl;
	startTibetan=0, endTibetan=0;
	
	for(int i=0;i<inStr.size();i++){
		match=inStr[i];
		matchStr+=match;
		//cout<<matchStr<<endl;
		if(matchStr.find(endUTF)!= std::string::npos){
			startTibetan=0;
			endTibetan=1;
			matchStr=""; uniStack+=match;
			result+=SinhalaUniToYagpo(uniStack,1);
			//cout<<"end tibetan result"<<result<<endl;
		}
		
		if(startTibetan==1){
			//cout<<"startT1 i="<<i<<" match1="<<endl;
			uniStack+=match;
			//cout<<" /"<<Unicode_to_UTF(destStr)<<endl;
		}
		if(matchStr.find(startUTF)!= std::string::npos&&endTibetan==0){startTibetan=1;endTibetan=0;
			result+=match;
			//cout<<"result.size()="<<result.size()<<" result="<<result<<endl;
			matchStr=""; 
		}
		if(startTibetan==0){
			if(endTibetan!=1){
				result+=match;
			}else{endTibetan=0;}
		}
		//cout<<"i="<<i<<" /"<<result<<endl;
		
		
	}
	//cout<<"//__"<<destStr.size()<<endl;
	//result+=destStr;
	//c_out<<"result="<<result<<" // destStr="<<Unicode_to_UTF(destStr)<<endl;
	//destStr="";
	//c_out<<"destStr="<<Unicode_to_UTF(destStr)<<endl;
	//result+=tibetanUTFToYagpo(destStr, uniTibTableYagpo);
	
	//c_out<<"DEST____"<<result<<endl;
	//cout<<result<<endl;
	//if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
	//result="";
	//cout<<"DEST"<<srcVect[j]<<endl;
	
	
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
}//_______________________________________________________________________________________


string GLogicProcessor::TibUniToWylie(string &srcStr,int mode){
	
	map<string,uniRecord>convertMap;
	vector<string>tibStr;
	vector<string>wylieStr;
	vector<string>wylieStrVect;
	vector<string>yagpoStrVect;

	string destStr;
	string destLine=srcStr;
	string delimeter;
	string srcLine=srcStr;
	string needConvertToWylie="";
	
		srcLine=str_replace("\r","",srcLine);
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
		
		cout<<"SRC="<<srcLine<<endl;
		
		yagpoStrVect=explode(" ",srcLine);
		
		for(int i=0;i<yagpoStrVect.size();i++){
			
			//if((int)yagpoStrVect[i][0]==-32||(int)yagpoStrVect[i][0]==-18){ //tibetan unicode range
				if(mode==1)destStr=UniToWylieMap[yagpoStrVect[i]].Wylie;
			    if(mode==2)destStr=YagpoToWylieMap[yagpoStrVect[i]].Wylie;

				cout<<i<<"= /"<<yagpoStrVect[i]<<"/ destString="<<destStr<<endl;
				if(destStr!=""){
					destLine=str_replace(yagpoStrVect[i],destStr,destLine,1);
				}else{
					needConvertToWylie+=yagpoStrVect[i];
					needConvertToWylie+="|\n";
				}
			//}
			//cout<<"DEST____"<<srcVect[j]<<endl;
		}
		destLine=str_replace("་"," ", destLine);
		destLine=str_replace("།","/", destLine);
		destLine=str_replace("༽", "}", destLine);
		destLine=str_replace("༼", "{", destLine);
		destLine=str_replace("༏", "|", destLine);
		destLine=str_replace("༴", "=", destLine);
		destLine=str_replace("༄", "@", destLine);
		destLine=str_replace("༅", "#", destLine);
		destLine=str_replace("༈", "!", destLine);
		destLine=str_replace("༿", "(", destLine);
		destLine=str_replace("༾", ")", destLine);
		destLine=str_replace("༔", ";", destLine);
		destLine=str_replace("ཿ", "H", destLine);
		
	    //cout<<destLine<<endl;
		cout<<"DEST"<<destLine<<endl;
		
    return destLine;
	if(needConvertToWylie!="")cout<<"need Convert To Wylie \n"<<needConvertToWylie;
	
}//_______________________________________________________________________________________


void GLogicProcessor::BonPDFToUni(string &srcStr){
    
    //cout<<" BonPDFToUni "<<srcStr<<endl;
    
    static vector<string>key;
    static vector<string>value;
    if(!key.size()){   cout<<"load dict"<<endl;
        string path=inputData.data["tablePath"]+"bon_code_page.txt";
        //cout<<"path="<<path<<endl;
        vector<string>dictStr; readText(dictStr,path);
        //cout<<"dictStr.size()="<<dictStr.size()<<endl;
        for(int i=0;i<dictStr.size();i++){
            vector<string> line=explode(":|:",dictStr[i]);
            //cout<<"dictStr["<<i<<"]="<<dictStr[i]<<" size="<<line.size()<<endl;
            if(line.size()>1){
                key.push_back(line[0]);
                value.push_back(line[1]);
            }
        }
    }
    
    for(int i=0;i<key.size();i++){
        srcStr=str_replace(key[i], value[i], srcStr);
        //cout<<" key="<<key[i]<<" value="<<value[i]<<" srcStr="<<srcStr<<endl;
    }
    //cout<<" srcStr="<<srcStr<<endl;


}

