#include "GLogicProcessor.h"
#include "GMemory.h"

#include <regex>

void  GLogicProcessor::mainDictReport(){
    //int reportLevel=0;
    string srcLine, testLine,delimiter, request, answer;
    short recordFound;
    //short spaceFlag=1;
    bool FullReport=false; if(inputData.data["dictReport"]=="report")FullReport=true;
    OCRDictRecord record;
    string str;
    int print=1;
    
    for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
        
        for (int k=stringItemVector.size();k>entry;k--){ //for every syllabon in List
            recordFound=0;
            srcLine=""; str="";
            for (int c=entry;c<k;c++){
                srcLine+=stringItemVector[c]+" ";   //all syllabons in this part of source string
                str+=stringItemVector[c]+delimeterTib[1];
            }
            testLine=srcLine;
            if(inputData.data["ocrData"]=="TibetanUTFToEng"){
                for(int m=0;m<grammarTib.size();m++){
                    testLine=str_replace(grammarTib[m].c_str(),"",testLine);  
                    //cout_<<"grammarTib[m]="<<grammarTib[m]<<" testLine="<<testLine<<endl;
                }
                testLine=str_replace("  "," ",testLine);
                testLine=str_replace("  "," ",testLine);
                DT(" srcLine="<<srcLine<<" testLine=/"<<testLine<<"/"<<END);
                
                //DT("size="<<dictIndexTib.size()<<END);
                //answer=dataIP(testLine,TIB_TO_ENG_UTF);  //send data by TCP/IP socket on main dictionary program
                if(dictIndexTib.find(testLine)!=dictIndexTib.end()){
                    cout_<<"match @@@ dictIndexTib[testLine].size()="<<dictIndexTib[testLine].size()<<endl;
                    for(int m=0;m<dictIndexTib[testLine].size();m++){ //all dictRecords for that word
                        //dictRecord[dictIndex[testLine][m]].searchString=str;
                        dictReport.push_back(dictRecord[dictIndexTib[testLine][m]]);
                        dictReport[dictReport.size()-1].searchString=str;
                        DT("found testLine "<<m<<" word index"<<dictRecord[dictIndexTib[testLine][m]].recID<<END);
                    }
					
                    recordFound=1;
                }else{ DT(" not_found");}
            }
            if(inputData.data["ocrData"]=="SanskritUTFToEng"||inputData.data["ocrData"]=="SinhalaUTFToEng"){
                //DT("size="<<dictIndexPali.size()<<END);
                DT("testLine0="<<testLine<<"/"<<endl;)
                for(int m=0;m<grammarPali.size();m++){
                    //cout_<<m<<"="<< grammarPali[m]<<"/"<<endl;
                    testLine=str_replace(grammarPali[m].c_str()," ",testLine);
                }
                //testLine.resize(testLine.size()-1); //remove delimeter in Pali
                DT("testLine0="<<testLine<<"/"<<endl;)
                if(dictIndexPali.find(testLine)!=dictIndexPali.end()){
                    
                    for(int m=0;m<dictIndexPali[testLine].size();m++){
                        //dictRecord[dictIndex[testLine][m]].searchString=str;
                        dictReport.push_back(dictRecord[dictIndexPali[testLine][m]]);
                        dictReport[dictReport.size()-1].searchString=str;
                        DT("found testLine "<<m<<" word index"<<dictRecord[dictIndexPali[testLine][m]].recID<<END);
                    }
                    recordFound=1;
                }
                
            }
            if(inputData.data["ocrData"]=="CheshDictReport"){
                //DT("size="<<dictIndexPali.size()<<END);
                DT("testLine0="<<testLine<<"/"<<endl;)
                //for(int m=0;m<grammarPali.size();m++){
                //cout_<<m<<"="<< grammarPali[m]<<"/"<<endl;
                //	testLine=str_replace(grammarPali[m].c_str()," ",testLine);
                //}
                //testLine.resize(testLine.size()-1); //remove delimeter in Pali
                DT("testLine0="<<testLine<<"/"<<endl;)
                if(dictIndexChesh.find(testLine)!=dictIndexChesh.end()){
                    
                    for(int m=0;m<dictIndexChesh[testLine].size();m++){
                        //dictRecord[dictIndex[testLine][m]].searchString=str;
                        dictReport.push_back(dictRecord[dictIndexChesh[testLine][m]]);
                        dictReport[dictReport.size()-1].searchString=str;
                        DT("found testLine "<<m<<" word index"<<dictRecord[dictIndexChesh[testLine][m]].recID<<END);
                    }
                    recordFound=1;
                }
                
            }
            
            if(recordFound){
                dictReportPage.push_back(dictReport);
                dictReport.resize(0);
                DT("record found"<<k<<" e="<<entry<<END);
                
                if(FullReport==false){
                    entry=k-1;
                    break;
                }
            }
            
        }//end for (int k=StringItemList->Count
    }//end for (int entry=0;
    
    
}//_________________________________________________________

void  GLogicProcessor::buildDictionary(vector<string> &strVector ){
    
	vector <string>srcVect;
	vector <string>srcVectKey;
	string srcLine,srcString,str,testLine,mainString;
	int step=0,recordFound;
	
	if(inputData.data["ocrData"]=="buildDict"){
		int index=0; 
		for(int i = 0; i<strVector.size(); i++) {
			strVector[i]=str_replace("\r", "", strVector[i]);
			
			vector <string> stringItemVector;
			stringItemVector=explode(" ", strVector[i]); //stringItemVector now contain all words from string
			for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
				for (int k=(int)stringItemVector.size();k>entry;k--){ //for every syllabon in List
					recordFound=0;
					srcLine=""; str="";
					for (int c=entry;c<k;c++){
						srcLine+=stringItemVector[c]+" ";   //all syllabons in this part of source string
					}
					//cout_<<" srcLine="<<srcLine<<endl;
					if(mainDict.find(srcLine)!=mainDict.end()){
						mainDict[srcLine].wordCount++; break;
					} 	mainDict[srcLine].wordCount++;
					
				}//end for (int k=StringItemList->Count
			}//end for (int entry=0;
			if (step==1000){cout_<<"translate line "<<index<<endl;step=0; }
			step++;index++;
            
		}
		cout_<<"mainDict.size()="<<mainDict.size()<<endl;
        cout_<<"start save dictionary"<<endl;
        writeDictionaryTXT( mainDict);
	}
    
}//_______________________________________________________________________________

void  GLogicProcessor::buildWordDictionary(vector<string> &strVector ){
	
	vector <string>srcVect;
	vector <string>srcVectKey;
	string srcLine,srcString,str,testLine,mainString;
	int step=0;
    
	
    int index=0; 
    for(int i = 0; i<strVector.size(); i++) {
        //strVector[i]=str_replace("\n", "", strVector[i]);
        vector <string> stringItemVector;
        stringItemVector=explode(" ", strVector[i]); //stringItemVector now contain all words from string
        //cout_<<"stringItemVector.size()"<<stringItemVector.size()<<endl;
        for (int entry=0; entry<stringItemVector.size();entry++){ //for every word in List
            mainDict[stringItemVector[entry] ].wordCount++; 
        }//end for (int entry=0;
        if (step==3000){cout_<<"translate line "<<index<<endl;step=0; }
        step++;index++;	
    }
    
    
	
}//_______________________________________________________________________________

void  GLogicProcessor::buildTranslationDictionary(vector<string> &strVector ){
    
    string srcLine, srcString;
    
    strVector.resize(0); 
    readText(strVector,inputData.data["inputFile"]);
    inputData.data["dictReport"]="main";
    ostringstream out;
    
    dictReport.resize(0); dictReportPage.resize(0);
    
    for(int i=0;i<strVector.size();i++){
        
        srcLine=strVector[i]; 
        srcLine=str_replace("\r","",srcLine);
        srcLine=str_replace("\n","",srcLine);
        cout_<<" start srcLine="<<srcLine<<endl;
        
        //if(inputData.data["cmd"]=="-dt")srcLine=translitWylieToYagpo(srcLine);
        srcString=srcLine;
        //normalise tibetan string strip punctuation mark
        for(int m=1;m<delimeterTib.size();m++){
            srcLine=str_replace(delimeterTib[m].c_str()," ",srcLine);
            //cout_<<"delimeter["<<m<<"]"<<delimeter[m]<<" srcLine="<<srcLine<<endl;
        }
        cout_<<" srcLine="<<srcLine<<endl;
        stringItemVector=explode(" ",srcLine); //stringItemVector now contain all syllabons from string
        cout_<<"start report stringItemVector.size()="<<stringItemVector.size()<<endl;
        mainDictReport();
        
        if(i>200)break;
    }
    out<<writeDictReportTranslation(srcString,1,0);
    
    string str=out.str();
    string path=inputData.data["inputFile"]+"_Dict.txt";
    writeText(str,path);
    
    
}

string GLogicProcessor::writeDictReport(string &srcStr,int reportLevel, int fullReport){
	
	int i;
	xml_document doc;
	xml_node mainNode;
	xml_node book, data;
	int print=1; 
	string str, dL;
	vector<dictKey> keyArray;
	//cout_<<"dictReportPage size="<<dictReportPage.size()<<endl;
	DT("<br>{/"<<srcStr<<"/}={} "<<END);
	if(dictReportPage.size()==0)return "";
	DT("dictRecord size="<<dictReportPage[0].size()<<END);
	ostringstream out;
    int reportCount=0;
    
    out<<"{\"data\": {\"mode\": \"dict\",\\\n \"recArray\": [\\\n";
    
	if(inputData.data["dictReport"]=="main"){  //main dict report
        
		for(int m=0;m<dictReportPage.size();m++) {
			DT("{"<<dictReportPage[m][0].searchString<<"}\t"<<END);
			
            sort (dictReportPage[m].begin(), dictReportPage[m].end() );
            
			for(i=0;i<dictReportPage[m].size();i++){
				//out<<"<div id=\"srcStr\">"; out<<dictReportPage[m][0].searchString; out<<"</div>";
                out<<"{\"rec\":\\\n{";
                out<<" \"recID\": \""<<dictReportPage[m][i].recID<<"\",\\\n";
                out<<" \"recKey\":\\\n[";
                
                sort (dictReportPage[m][i].reportKey.begin(), dictReportPage[m][i].reportKey.end() );
                
				for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
					out<<"{ \"ln\": \""<<dictReportPage[m][i].reportKey[j].ln<<"\",\\\n"; 
					out<<"\"key\": \""<<dictReportPage[m][i].reportKey[j].key<<"\",\\\n";
                    out<<"\"lv\": \""<<dictReportPage[m][i].reportKey[j].level<<"\",\\\n";
                    out<<"\"c\": \""<<dictReportPage[m][i].reportKey[j].context<<"\",\\\n";
                    out<<"\"grm\": \""<<dictReportPage[m][i].reportKey[j].grm<<"\",\\\n";
                    out<<"\"id\": \""<<dictReportPage[m][i].reportKey[j].id<<"\" }";
                    
                    if(j<dictReportPage[m][i].reportKey.size()-1)out<<",\\\n";
                    
				}
                out<<"\\\n],\\\n";
                out<<"\"lv\": \""<<dictReportPage[m][i].level<<"\",\\\n";
				out<<"\"eentry\": \""<<dictReportPage[m][i].eentry<<"\",\\\n";
				out<<"\"rentry\": \""<<dictReportPage[m][i].rentry<<"\",\\\n";
				out<<"\"example\": \""<<dictReportPage[m][i].example<<"\",\\\n";
				out<<"\"dict\": \""<<dictReportPage[m][i].dict+"\"\\\n";
                out<<"}\\\n";
                out<<"},\\\n";
                reportCount++;
				if(dictReportPage[m][i].dict=="ER"||
				   dictReportPage[m][i].dict=="RHD"||
				   dictReportPage[m][i].dict=="RE"||
				   dictReportPage[m][i].dict=="TD"){break;}
                
                //if(i<dictReportPage[m].size()-1)out<<",\n";
				
			}
            
			//sortKey(keyArray,dictReportPage[m][0].searchString,fullReport);
			//keyArray.resize(0);
			//cout_<<dL;
		}
	}else{  //all dict report
        for(int m=0;m<dictReportPage.size();m++) {
			DT("{"<<dictReportPage[m][0].searchString<<"}\t"<<END);
			
            sort (dictReportPage[m].begin(), dictReportPage[m].end() );
            
			for(i=0;i<dictReportPage[m].size();i++){
				//out<<"<div id=\"srcStr\">"; out<<dictReportPage[m][0].searchString; out<<"</div>";
                out<<"{\"rec\":\\\n{";
                out<<" \"recID\": \""<<dictReportPage[m][i].recID<<"\",\\\n";
                out<<" \"recKey\":\\\n[";
                sort (dictReportPage[m][i].reportKey.begin(), dictReportPage[m][i].reportKey.end() );
				for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
					out<<"{ \"ln\": \""<<dictReportPage[m][i].reportKey[j].ln<<"\",\\\n"; 
					out<<"\"key\": \""<<dictReportPage[m][i].reportKey[j].key<<"\",\\\n";
                    out<<"\"lv\": \""<<dictReportPage[m][i].reportKey[j].level<<"\",\\\n";
                    out<<"\"c\": \""<<dictReportPage[m][i].reportKey[j].context<<"\",\\\n";
                    out<<"\"grm\": \""<<dictReportPage[m][i].reportKey[j].grm<<"\",\\\n";
                    out<<"\"id\": \""<<dictReportPage[m][i].reportKey[j].id<<"\" }";
                    
                    if(j<dictReportPage[m][i].reportKey.size()-1)out<<",\\\n";
                    
				}
                out<<"\\\n],\\\n";
                out<<"\"lv\": \""<<dictReportPage[m][i].level<<"\",\\\n";
				out<<"\"eentry\": \""<<dictReportPage[m][i].eentry<<"\",\\\n";
				out<<"\"rentry\": \""<<dictReportPage[m][i].rentry<<"\",\\\n";
				out<<"\"example\": \""<<dictReportPage[m][i].example<<"\",\\\n";
				out<<"\"dict\": \""<<dictReportPage[m][i].dict+"\"\\\n";
                out<<"}\\\n";
                out<<"},\\\n";
				
			}
            
			//sortKey(keyArray,dictReportPage[m][0].searchString,fullReport);
			//keyArray.resize(0);
			//cout_<<dL;
		}
	}
    dL=out.str();dL.resize(dL.size()-3);
    dL+="]}\\\n }";
    dL=str_replace("'","ʼ",dL);
    
    return dL;
	
}//_____________________________________________________________________________

string GLogicProcessor::writeDictReportTranslation(string &srcStr,int reportLevel, int fullReport){
    
    int i;
    xml_document doc;
    xml_node mainNode;
    xml_node book, data;
    //int wordID=0;
    //char cbuf[20];
    int print=1;
    string str,dL;
    vector<dictKey> keyArray;
    //cout_<<"dictReportPage size="<<dictReportPage.size()<<endl;
    DT("<br>{/"<<srcStr<<"/}={} "<<END);
    if(dictReportPage.size()==0)return "";
    DT("dictRecord size="<<dictReportPage[0].size()<<END);
    static int index=0;
    ostringstream out;
    
    if(inputData.data["dictReport"]=="main"){  //main dict report
        
        for(int m=0;m<dictReportPage.size();m++) {
            DT("{"<<dictReportPage[m][0].searchString<<"}\t"<<END);
            
            for(i=0;i<dictReportPage[m].size();i++){
                //out<<"<div id=\"srcStr\">"; out<<dictReportPage[m][0].searchString; out<<"</div>";
                
                //проверяем был ли такой ключ в словаре
                
                for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
                    if(dictReportPage[m][i].reportKey[j].ln=="tb"){
                        str=dictReportPage[m][i].reportKey[j].key;
                        break;
                    }
                }
                
                //cout_<<"@@@str="<<str<<endl;
                print=0;
                //if(str=="ར་")print=1;
                
                int flag=0;
                for(int n=0;n<m;n++){
                    for(int a=0;a<dictReportPage[n].size();a++){
                        for(int j=0;j<dictReportPage[n][a].reportKey.size();j++){
                            DT("dictReportPage["<<n<<"]["<<a<<"].reportKey["<<j<<"].key="<<dictReportPage[n][a].reportKey[j].key<<endl);
                            if(dictReportPage[n][a].reportKey[j].ln=="tb"){
                                
                                if(str==dictReportPage[n][a].reportKey[j].key){
                                    flag=1;
                                    break;
                                }
                            }
                        }
                    }
                }
                print=0;
                //cout_<<"@@flag="<<flag<<endl;
                if(flag)continue;
                
                
                out<<"t["<<index<<"]=[\""; index++;
                for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
                    if(dictReportPage[m][i].reportKey[j].ln!="tb")continue;
                    out<<dictReportPage[m][i].reportKey[j].key;
                }
                out<<"\", \"\", \"";
                
                for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
                    if(dictReportPage[m][i].reportKey[j].ln!="en"&&
                       dictReportPage[m][i].reportKey[j].ln!="sn"&&
                       dictReportPage[m][i].reportKey[j].ln!="ru")continue;
                    out<<dictReportPage[m][i].reportKey[j].key;
                }
                
                out<<dictReportPage[m][i].eentry<<" ";
                out<<dictReportPage[m][i].rentry<<" ";
                out<<dictReportPage[m][i].example<<" (";
                out<<dictReportPage[m][i].dict<<")\"];\n";
                
                if(dictReportPage[m][i].dict=="ER"||
                   dictReportPage[m][i].dict=="RHD"||
                   dictReportPage[m][i].dict=="RE"||
                   dictReportPage[m][i].dict=="TD")break;
                
            }
            //sortKey(keyArray,dictReportPage[m][0].searchString,fullReport);
            //keyArray.resize(0);
            //cout_<<dL;
        }
    }else{  //all dict report
        for(int m=0;m<dictReportPage.size();m++) {
            DT("{"<<dictReportPage[m][0].searchString<<"}\t"<<END);
            
            for(i=0;i<dictReportPage[m].size();i++){
                //out<<"<div id=\"srcStr\">"; out<<dictReportPage[m][0].searchString; out<<"</div>";
                for(int j=0;j<dictReportPage[m][i].reportKey.size();j++){
                    out<<"<div class=\""; out<<dictReportPage[m][i].reportKey[j].ln+"\">";
                    out<<dictReportPage[m][i].reportKey[j].key;
                    out<<"</div>\n";
                    
                }
                out<<"<div class=\"eentry\">"; out<<dictReportPage[m][i].eentry+"</div>\n";
                out<<"<div class=\"rentry\">"; out<<dictReportPage[m][i].rentry+"</div>\n";
                out<<"<div class=\"example\">"; out<<dictReportPage[m][i].example+"</div>\n";
                out<<"<div class=\"dict\">"; out<<dictReportPage[m][i].dict+"</div>\n";
                
            }
            DT(dL<<endl);
        }
    }
    dL=out.str();
    return dL;
    
}//_____________________________________________________________________________


void GLogicProcessor::saveInDictionary( string &srcStr){
    int print=0;
    DT("srcStr="<<srcStr<<endl);
    vector<string> data=explode(":|:",srcStr); if(data.size()<3)return; 
    //for(int i=0;i<data.size();i++)cout_<<data[i]<<endl;
    
    int recID=atoi(data[0].c_str());
    
    if(data[1]=="dict")dictRecord[recID].dict=data[2];
    if(data[1]=="rentry")dictRecord[recID].rentry=data[2];
    if(data[1]=="eentry")dictRecord[recID].eentry=data[2];
    if(data[1]=="example")dictRecord[recID].example=data[2];
    if(data[1]=="level")dictRecord[recID].level=atoi(data[2].c_str());
    
    
    if(data[1]=="key"){
        int keyID=atoi(data[2].c_str());
        if(data[3]=="ln")dictRecord[recID].reportKey[keyID].ln=data[4];
        if(data[3]=="key")dictRecord[recID].reportKey[keyID].key=data[4];
        if(data[3]=="level")dictRecord[recID].reportKey[keyID].level=atoi(data[4].c_str());
        if(data[3]=="context")dictRecord[recID].reportKey[keyID].context=data[4];
        if(data[3]=="grm")dictRecord[recID].reportKey[keyID].grm=data[4];
    }
    
    if(data[1]=="save")writeDictionary(dictRecord, inputData.data["dictPath"]);
    
    DT(dictRecord[recID].reportKey[0].key);
}

void GLogicProcessor::TibRusDictReport(GVector*vectorDict, GStr<uint>*keyDict,GVector*strVector){

    string inStr,reStr;
    //cout<<"vectorDict.size()="<<vectorDict->size()<<endl;
    //cout<<"text size="<<strVector->size();
//TIME_START    
	for(int index=0;index<strVector->size();index++){//&&index<10
        
        TString st; strVector->getTStr(index,st);
        string textStr="@]་"+st[1];
        textStr=str_replace(" ","་",textStr);
        //cout<<"//////"<<" "<<textStr<<endl;

		uint recNum=keyDict->size();
        for(uint a=0;a<recNum;a++ ){
        	TString st; vectorDict->getTStr(keyDict[0][a],st);
            string keyStr=st[0]; 
            if(keyStr.size()<6)continue;
            keyStr.resize(keyStr.size()-3); //cout<<"key="<<keyStr<<endl;
            string valueStr=st[2];
            if(textStr.size()<keyStr.size())continue;
            if(textStr.find(keyStr)==-1)continue;
            //cout<<"key="<<keyStr<<endl;
            if(1){
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འི་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][འིa.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ར་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][རd.p]་\\2:|:3:|:"; 
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ས་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][སv.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འམ་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][འམ་a.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
            }else{
                /*
                //cout_<<" @//@ key="<<keyStr;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"་(.*)):|:@\\1["+keyStr+"་"+valueStr+"]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འི་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][འིa.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ར་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][རd.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);  //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ས་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][སv.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འམ་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][འམ་a.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                */
            }
           
	    }
        textStr=str_replace("@]་","",textStr);
	    strVector->putStr(index,textStr);
        //cout_<<"@@@@"<<" "<<textStr<<endl;
          
	}  
//TIME_PRINT_
    
};

void GLogicProcessor::TibRusDictReport(GVector*vectorDict, GStr<uint>*keyDict,string &textStr){
    
    string inStr,reStr;
    //cout<<"keyDict->size()="<<keyDict->size()<<endl;
    //TIME_START
    textStr="@]་"+textStr;
		uint recNum=keyDict->size();
        for(uint a=0;a<recNum;a++ ){
        	TString st; vectorDict->getTStr(keyDict[0][a],st);
            string keyStr=st[0];   //cout_<<"key="<<keyStr<<endl;
            if(keyStr.size()<6)continue;
            keyStr.resize(keyStr.size()-3); 
            string valueStr=st[2];
            if(textStr.size()<keyStr.size())continue;
            if(textStr.find(keyStr)==-1)continue;
            //cout<<"key="<<keyStr<<endl;
            
            if(1){
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འི་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][འིa.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ར་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][རd.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ས་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][སv.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འམ་(.*)):|:@\\1[<a href=\""+MAIN_HOST+"ocr.php?record=find&db=dict&c1="+keyStr
                +"་\" target=\"_blank\">"+keyStr+"་</a>"+valueStr+"][འམ་a.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);
                //cout<<textStr<<"<br>";
            }else{
                //cout_<<" @//@ key="<<keyStr;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"་(.*)):|:@\\1["+keyStr+"་"+valueStr+"]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འི་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][འིa.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ར་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][རd.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr);  //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"ས་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][སv.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                reStr="(@(.*\\][^\\[]*་)"+keyStr+"འམ་(.*)):|:@\\1["+keyStr+"་"+valueStr+"][འམ་a.p]་\\2:|:3:|:";
                regExpReplace(textStr,reStr); //cout_<<"textStr="<<textStr<<endl;
                
            }
            
	    }
    textStr=str_replace("@]་","@་",textStr);
    //TIME_PRINT_
    
};
void GLogicProcessor::PaliRusDictRE(vector<string>&text,int print){
    vector <string> dictRE,dictRESort;
    static string strREPath;
    static vector<dictKey>lineKey;
    static vector<string> lineSubstitution;
    
    if(!lineSubstitution.size()){
        lineSubstitution.resize(100);
        for(int i=0;i<100;i++){
            lineSubstitution[i]="{!-";lineSubstitution[i]+=(uchar)(i+48);lineSubstitution[i]+="-!}";
        }
        
    }
    
    
    //inputData.data["ln"]="eng";
    
    if(!lineKey.size()){
        strREPath=inputData.data["root"]+"edit/MEDIA/allDict_pali_"+inputData.data["ln"]+".js";
        vector<string>data;
        readText(data,strREPath);
        
        vector<string>grm;
        strREPath=inputData.data["root"]+"OCRData/Grammar/Pali_grammar.txt";
        readText(grm, strREPath);
        
        for(int i=1;i<data.size();i++){
            vector<string>line;
            line=explode("\"",data[i]);
            if(line.size()<6){
                //cout<<"i="<<i<<" "<<data[i]<<endl;
                continue;
            }
            dictKey key;
            key.lenght=(uint)line[1].size();
            key.tib=line[1];
            key.tibKey=line[1];
            key.destKey=line[5];
            lineKey.push_back(key);
            key.tibKey+="@";
            
            for(int t=0;t<grm.size();t++){
                string str=grm[t]+"@";
                string str_=str_replace(str.c_str(), "", key.tibKey);
                if(str_.size()<3)continue;
                //if(key.tibKey=="loka@"){cout<<"str="<<str<<" key.tibKey="<<key.tibKey<<" str_="<<str_<<endl;}
                if(str_!=key.tibKey){
                    key.tib=str_;
                    key.tibKey=str_;
                    key.destKey="~"+grm[t]+" "+line[5];
                    key.lenght=(uint)str_.size();
                    
                    lineKey.push_back(key); break;
                }
            }
        }
        sort(lineKey.begin(),lineKey.end());
        //cout<<"lineKey="<<lineKey.size()<<endl;
    }
    //for(int i=0;i<lineKey.size();i++)cout<<lineKey[i].tib<<endl;
    //exit(0);
    
    vector<string>textResult;
    string strText="";
    int l;
    string str;
    int stepLimit=(int)text.size()/10;
    int step=0;
    int progress=1;
    int size=(int)text.size();
    
    for(int i=0;i<size;i++){ //
        if(stepLimit>10000){
            if(step>stepLimit){
                cout_<<progress<<"0% size="<<size<<" i="<<i<<endl;
                progress++;
                step=0;
            }step++;
        }
        //cout<<"i="<<i<<text[i]<<endl;
        str=text[i];
        l=(int)str.size();
        int m=(int)str.find("а");
        if(l>3&&m==-1){
            //str=str_replace(")", ")་", str);
            //str=str_replace("(", "་(", str);
            //regExpReplace(str,"([༄༅༔༴།།༈༌༑༼༽]+):|:་:|:1");
            //regExpReplace(str,"([༄༅༔༴།།༈༌༑༼༽]+):|:་:|:1");
            //regExpReplace(str,"([༄༅༔༴༡༢༣༤༥༦༧༨༩༠།༈༌༑༼༽]+):|:་:|:1");
            //regExpReplace(str,"([༄༅༔༴༡༢༣༤༥༦༧༨༩༠།༈༌༑༼༽]+):|:་:|:1");
            //str=str_replace(" ", "་", str);
            //str=str_replace("ཿ", "ཿ་", str);
            
            //cout<<"////str="<<str<<endl;
            //str="་"+str+"་།";
            //regExpReplace(str,"(་[་]+):|:་:|:1");
            lowerCase(str);

            
            string re,re1;
            int countRes=0;
            vector<string> translationResult;
            
            for(int j=0;j<lineKey.size();j++){
                //cout<<"dictRE "<<t[j].tib<<" "<<t[j].tibKey<<" "<<t[j].destKey<<endl;
                //if(t[j][2]=="undefined")continue;
                if(lineKey[j].tib.size()>l)continue;
                if(str.find(lineKey[j].tib)==-1)continue;
                if(print){
                    //re="་"+lineKey[j].tib+"་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"]་";
                    //str=str_replace(re, re1, str);
                    
                }else{
                    
                    re=lineKey[j].tib;
                    re1=lineSubstitution[countRes]; countRes++;
                    translationResult.push_back("[<a href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"\" target=\"_blank\">"+lineKey[j].tibKey+"</a><span class=\"a2\" contenteditable=\"true\">="+lineKey[j].destKey+"</span>]");
                    str=str_replace(re, re1, str);
                    
                    //re="@"+lineKey[j].tib+"ṃ@"; re1="@[<a href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"\" target=\"_blank\">"+lineKey[j].tibKey+"</a><span contenteditable=\"true\">="+lineKey[j].destKey+"</span>][ṃ]@";
                    //str=str_replace(re, re1, str);
                }
                
                //cout<<"@@@str="<<str<<endl;
            }
            //cout<<"translationResult.size()="<<translationResult.size()<<endl;
            for(int t=0;t<translationResult.size();t++){
                //cout<<"@@@str="<<str<<" translationResult[t]="<<translationResult[t]<<endl;
                str=str_replace(lineSubstitution[t], translationResult[t], str);
                //cout<<"@@@str="<<str<<endl;
            }
            
            ostringstream out; out<<"i"<<i;
            string idLine=out.str();
            
            str=str_replace("@"," ",str);
            str=str_replace("@"," ",text[i])+"\n@<img src=\"/work_file/_img/line.gif\" onClick=\"editText(\""+idLine+"\")\"/><span id=\""+idLine+"\">"+str+"</span>\n";
            //+"<img src=\"/work_file/_img/lineEnd.gif\" onClick=\"editText(\""+idLine+"\")\">";
            textResult.push_back(str);
            //cout<<str;//exit(0);
        }else{
            str=str_replace("@"," ",text[i]);
            textResult.push_back(str);
        }
        
    }
    text=textResult;
    cout_<<"done translation";


}
void GLogicProcessor::SanskritRusDictRE(vector<string>&text,int print){
    vector <string> dictRE,dictRESort;
    static string strREPath;
    static vector<dictKey>lineKey;
    static vector<string> lineSubstitution;
    
    if(!lineSubstitution.size()){
        lineSubstitution.resize(100);
        for(int i=0;i<100;i++){
            lineSubstitution[i]="{!-";lineSubstitution[i]+=(uchar)(i+48);lineSubstitution[i]+="-!}";
        }
        
    }
    
    
    //inputData.data["ln"]="eng";
    
    if(!lineKey.size()){
        strREPath=inputData.data["root"]+"edit/MEDIA/allDict_sanskrit_"+inputData.data["ln"]+".js";
        vector<string>data;
        readText(data,strREPath);
        
        vector<string>grm;
        strREPath=inputData.data["root"]+"OCRData/Grammar/Sanskrit_grammar.txt";
        readText(grm, strREPath);
        
        for(int i=1;i<data.size();i++){
            vector<string>line;
            line=explode("\"",data[i]);
            if(line.size()<6){
                //cout<<"i="<<i<<" "<<data[i]<<endl;
                continue;
            }
            dictKey key;
            key.lenght=(uint)line[1].size();
            key.tib=line[1];
            key.tibKey=line[1];
            key.destKey=line[5];
            lineKey.push_back(key);
            key.tibKey+="@";
            
            for(int t=0;t<grm.size();t++){
                string str=grm[t]+"@";
                string str_=str_replace(str.c_str(), "", key.tibKey);
                if(str_.size()<3)continue;
                //if(key.tibKey=="loka@"){cout<<"str="<<str<<" key.tibKey="<<key.tibKey<<" str_="<<str_<<endl;}
                if(str_!=key.tibKey){
                    key.tib=str_;
                    key.tibKey=str_;
                    key.destKey="~"+grm[t]+" "+line[5];
                    key.lenght=(uint)str_.size();
                    
                    lineKey.push_back(key); break;
                }
            }
        }
        sort(lineKey.begin(),lineKey.end());
        //cout<<"lineKey="<<lineKey.size()<<endl;
    }
    //for(int i=0;i<lineKey.size();i++)cout<<lineKey[i].tib<<endl;
    //exit(0);
    
    vector<string>textResult;
    string strText="";
    int l;
    string str;
    int stepLimit=(int)text.size()/10;
    int step=0;
    int progress=1;
    int size=(int)text.size();
    
    for(int i=0;i<size;i++){ //
        if(stepLimit>10000){
            if(step>stepLimit){
                cout_<<progress<<"0% size="<<size<<" i="<<i<<endl;
                progress++;
                step=0;
            }step++;
        }
        //cout<<"i="<<i<<text[i]<<endl;
        str=text[i];
        l=(int)str.size();
        int m=(int)str.find("а");
        if(l>3&&m==-1){
            //cout<<"////str="<<str<<endl;
            //str="་"+str+"་།";
            //regExpReplace(str,"(་[་]+):|:་:|:1");
            lowerCase(str);
            
            
            string re,re1;
            int countRes=0;
            vector<string> translationResult;
            
            for(int j=0;j<lineKey.size();j++){
                //cout<<"dictRE "<<t[j].tib<<" "<<t[j].tibKey<<" "<<t[j].destKey<<endl;
                //if(t[j][2]=="undefined")continue;
                if(lineKey[j].tib.size()>l)continue;
                if(str.find(lineKey[j].tib)==-1)continue;
                if(print){
                    //re="་"+lineKey[j].tib+"་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"]་";
                    //str=str_replace(re, re1, str);
                    
                }else{
                    
                    re=lineKey[j].tib;
                    re1=lineSubstitution[countRes]; countRes++;
                    translationResult.push_back("[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>]");
                    str=str_replace(re, re1, str);
                    
                    //re="@"+lineKey[j].tib+"ṃ@"; re1="@[<a href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"\" target=\"_blank\">"+lineKey[j].tibKey+"</a><span contenteditable=\"true\">="+lineKey[j].destKey+"</span>][ṃ]@";
                    //str=str_replace(re, re1, str);
                }
                
                //cout<<"@@@str="<<str<<endl;
            }
            //cout<<"translationResult.size()="<<translationResult.size()<<endl;
            for(int t=0;t<translationResult.size();t++){
                //cout<<"@@@str="<<str<<" translationResult[t]="<<translationResult[t]<<endl;
                str=str_replace(lineSubstitution[t], translationResult[t], str);
                //cout<<"@@@str="<<str<<endl;
            }
            
            ostringstream out; out<<"i"<<i;
            string idLine=out.str();
            
            str=str_replace("@"," ",str);
            str=str_replace("@"," ",text[i])+"\n@<img src=\"/work_file/_img/line.gif\" onClick=\"editText(\""+idLine+"\")\"/><span id=\""+idLine+"\">"+str+"</span>\n";
            //+"<img src=\"/work_file/_img/lineEnd.gif\" onClick=\"editText(\""+idLine+"\")\">";
            textResult.push_back(str);
            //cout<<str;//exit(0);
        }else{
            str=str_replace("@"," ",text[i]);
            textResult.push_back(str);
        }
        
    }
    text=textResult;
    cout_<<"done translation";
    
    
}

void GLogicProcessor::TibRusDictRE(vector<string>&text,int print,int mode){
    

    int flagLang=0;
    for(int i=0;i<text.size();i++){
        if(RE2::PartialMatch(text[i],"[\\p{Tibetan}]")){flagLang=1; break;}
        if(RE2::PartialMatch(text[i],"[\\p{Devanagari}]")){flagLang=2; break;}
    }

    if(flagLang==0)return PaliRusDictRE(text,print);
    if(flagLang==2)return SanskritRusDictRE(text,print);
    
    vector <string> dictRE,dictRESort;
    static string strREPath;
    static vector<dictKey>lineKey;
    
    //inputData.data["ln"]="eng";"
    if(inputData.data["ln"]=="")inputData.data["ln"]="rus";
    if(inputData.data["ln"]=="report"){inputData.data["ln"]="rus"; mode=2;}
    
    if(!lineKey.size()){
        if(mode==1){
            strREPath=inputData.data["root"]+"edit/MEDIA/allDict_"+inputData.data["ln"]+".js";
        }
        if(mode==2){
            strREPath=inputData.data["root"]+"edit/MEDIA/allDict_main.js";
        }
        vector<string>data;
        readText(data,strREPath);
        
        
        /*    struct dictKey {
         string key;
         string tibKey;
         string tib;
         string srcKey;
         string destKey;
         string dict;
         string ln;
         string grm;
         string context;
         int level;  //релевантность ключа
         int flag;
         uint lenght;
         uint id;
         };
         */
        //cout<<"strREPath="<<strREPath<<""<<data.size();
        
        
        //int print=1;
        for(int i=1;i<data.size();i++){
            vector<string>line;
            line=explode("\"",data[i]);
            if(line.size()<6){
                DR("i="<<i<<" "<<line[1]<<endl)
                continue;
            }
            dictKey key;
            key.lenght=(uint)line[1].size();
            key.tib=line[1];
            line[1]=str_replace("་","^",line[1]);
            key.tibKey=line[1];
            key.destKey=str_replace("་","^",line[5]);
            lineKey.push_back(key);
        }
        sort(lineKey.begin(),lineKey.end());
        //cout<<"lineKey="<<lineKey.size();
    }
    //for(int i=0;i<lineKey.size();i++)cout_<<lineKey[i].tib<<endl;
    //exit(0);
    
    vector<string>textResult;
    string strText="";
    int l;
    string str,dictStr;
    int stepLimit=(int)text.size()/10;
    int step=0;
    int progress=1;
    int size=(int)text.size();
    
    for(int i=0;i<size;i++){ //
        if(stepLimit>10000){
            if(step>stepLimit){
                cout_<<progress<<"0% size="<<size<<" i="<<i<<endl;
                progress++;
                step=0;
            }step++;
        }    
        //cout<<"i="<<i<<text[i];
        str=text[i];
        dictStr="";
        if(str[0]=='@')continue;
        l=(int)str.size();
        int m=(int)str.find("་");
        if(l>3&&m!=-1){
            str=str_replace(")", ")་", str);
            str=str_replace("(", "་(", str);
            regExpReplace(str,"([༄༅༔༴།།༈༌༑༐༼༽]+):|:་:|:1");
            regExpReplace(str,"([༄༅༔༴།།༈༌༑༐༼༽]+):|:་:|:1");
            regExpReplace(str,"(\\[[༡༢༣༤༥༦༧༨༩༠]+\\]):|::|:1");
            //regExpReplace(str,"([༄༅༔༴༡༢༣༤༥༦༧༨༩༠།༈༌༑༼༽]+):|:་:|:1");
            //regExpReplace(str,"([༄༅༔༴༡༢༣༤༥༦༧༨༩༠།༈༌༑༼༽]+):|:་:|:1");
            str=str_replace(" ", "་", str);
            str=str_replace("ཿ", "ཿ་", str);
            
            //cout<<"////str="<<str<<endl;
            str="་"+str+"་།";
            regExpReplace(str,"(་[་]+):|:་:|:1");
            regExpReplace(str,"(ག་གོ་།):|:ག་(stop)་:|:1");
            regExpReplace(str,"(ང་ངོ་།):|:ང་(stop)་:|:1");
            regExpReplace(str,"(ད་དོ་།):|:ད་(stop)་:|:1");
            regExpReplace(str,"(ན་ནོ་།):|:ན་(stop)་:|:1");
            regExpReplace(str,"(བ་བོ་།):|:བ་(stop)་:|:1");
            regExpReplace(str,"(མ་མོ་།):|:མ་(stop)་:|:1");
            regExpReplace(str,"(([^་])འོ་།):|:\\1་(stop)་:|:2");
            regExpReplace(str,"(ར་རོ་།):|:ར་(stop)་:|:1");
            regExpReplace(str,"(ལ་ལོ་།):|:ལ་(stop)་:|:1");
            regExpReplace(str,"(ས་སོ་།):|:ས་(stop)་:|:1");
            regExpReplace(str,"(་ཏོ་།):|:་(stop)་:|:1");
            
	 	 	/*ཞིང་[zhing] I. ༼noun༽ Meaning a country, a land, or place. E. g. , a buddha-field; a universe or realm; a farmer"s field. II. ༼ཚིག་ཕྲད་ phrase connector༽ One of a group of three connectors ཅིང་, ཞིང་, and ཤིང་. Placement: They are ཕྲད་གཞན་དབང་ཅན་ dependent connectors. When one of them is required, this one is placed after names ending with ང་, ན་, མ་, འ་, ར་, ལ་, and after མཐའ་མེད་ words without endings.
             
             § 38. ཅིང་ཞིང་ཤིང་ — чистые формы несовершенного и совершенного деепричастия, судя по тому, на что указывает предшествующая форма времени. Они, касательно правописания, подлежат тем же законам конечных согласных, как и упомянутые в § 37 частицы; например: ཀློག་ཅིང་ — «читая»; ཀླགས་ཤིང་ — «прочитав»; སྨྲ་ཞིང་ — «говоря», «сказывая»; སྨྲས་ཤིང་ — «сказав».
             */
            /*
             regExpReplace(str,"(([ངནམའརལ])་ཞིང་):|:\1་(и;деепр.)་:|:2");
             regExpReplace(str,"(ས་ཤིང་):|:ས་(и;деепр.)་:|:1");
             regExpReplace(str,"(([ནརལས])་ཏེ་):|:\1་(н.деепр.)་:|:2");
             regExpReplace(str,"(ད་དེ་):|:ད་(н.деепр.)་:|:1");
             regExpReplace(str,"(([གངབམའ])་སྟེ་):|:\1་(н.деепр.):|:2");
             */
            
            regExpReplace(str,"(་པའང་):|:་པ་[འང=very]་:|:1");
            regExpReplace(str,"(་བའང་):|:་བ་[འང=very]་:|:1");
            regExpReplace(str,"(་པའམ་):|:་པ་[-འམ=q.p.;or]་:|:1");
            regExpReplace(str,"(་བའམ་):|:་བ་[-འམ=q.p.;or]་:|:1");
            regExpReplace(str,"(་པོའམ་):|:་པོ་[-འམ=q.p.;or]་:|:1");
            regExpReplace(str,"(་བོའམ་):|:་བོ་[-འམ=q.p.;or]་:|:1");
            
            
            string re,re1;
            for(int j=0;j<lineKey.size();j++){
                //cout<<"dictRE "<<t[j].tib<<" "<<t[j].tibKey<<" "<<t[j].destKey<<endl;
                //if(t[j][2]=="undefined")continue;
                if(lineKey[j].tib.size()>l)continue;
                if(mode==2&&lineKey[j].tib.size()<5)break;
                if(str.find(lineKey[j].tib)==-1)continue;
                if(mode==1){
                    if(print){
                        re="་"+lineKey[j].tib+"་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"འི་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"][g.l.]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"ས་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"][v.p.]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"ར་"; re1="་["+lineKey[j].tibKey+"="+lineKey[j].destKey+"][d.l.]་";
                        str=str_replace(re, re1, str);
                    }else{
                        
                        re="་"+lineKey[j].tib+"་"; re1="་[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"་\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"འི་"; re1="་[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"་\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>][g.l.]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"ས་"; re1="་[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"་\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>][v.p]་";
                        str=str_replace(re, re1, str);
                        
                        re="་"+lineKey[j].tib+"ར་"; re1="་[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"་\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>][d.l.]་";
                        str=str_replace(re, re1, str);
                        
                    }
                }
                if(mode==2){
                    dictStr+="་[<a class=\"a2\" href=\"ocr.php?db=dict&word="+lineKey[j].tibKey+"་\" target=\"_blank\">"+lineKey[j].tibKey+"</a><a1 contenteditable=\"true\">="+lineKey[j].destKey+"</a1>]་<br>";
                }
                
                //cout<<"@@@str="<<str<<endl;
            }
            if(mode==2)str=dictStr;
            
            
            /*§ 36. Частицы གོ་ངོ་དོ་ནོ་བོ་མོ་འོ་རོ་ལོ་སོ་ и ཏོ་ — все имеют тождественное значение. Эти частицы представляют утверждающую или определённую связку, наподобие того, как частицы под § 35 выражают вопрос или сомнение; они часто соединяются с настоящим, прошедшим и будущим временем; у может соединяться только с прошедшим после конечных букв ན་ར་ལ་ (вместо ནད་རད་ལད་) - Вот примеры для объяснения: འདུག་གོ — «есть», «есть здесь»; སོང་ངོ་ — «он пошёл»; རིང་ངོ་ — «он длинен»; ཡོད་དོ་ — «он здесь»; ཡིན་ནོ་ — «он есть»; ཐོབ་བོ་ — «ты нашёл»; ལམ་མོ་ — «это дорога»; མིའོ་ — «это человек»; ང་འགྲོའོ་ — «я иду», «я пойду», или «я идущий»; འགྱུར་རོ་ — «будет»; གསོལ་ལོ་ — «я прошу», «спрашиваю», «предлагаю»; ལེགས་སོ་ — «хорошо», «оно хорошо»; གསན་ཏོ་ (вместо གསནད་ཏོ) — «я слышал»; གྱུར་ཏོ་ (вместо གྱུརད་ཏོ་) — «он сделался»; གསོལ་ཏོ་ (вместо གསོལད་ཏོ་) — «он просил», «спросил» и пр. Они составляются повторением конечной буквы предстоящего слога и присоединением гласного знака «о», т. е. таким же образом, как и частицы, означенные под § 35.
             */
            
            
            /*§ 41. Частицы ཏེ་དེ་ и སྟེ་, присоединённые к причастию настоящего времени или к корню глагола, образуют, подобно упомянутым в § 38 частицам, деепричастие несовершенного вида; например: འདུག་སྟེ་ — «сидя»; таким же образом в соединении с причастием прошедшего времени образуют деепричастие совершенного вида, как например: སྨྲས་ཏེ་ — «сказав». Частица ཏེ следует за окончаниями на ན་ར་ལ་ས་, དེ་ — после ད་, а སྟེ་ — после ག་ང་བ་མ་འ་; например: གསན་ཏེ་ — «слыша»; འགུར་ཏེ་ — «ведя», «нося»; ཉལ་ཏེ་ — «лёжа», «спя»; སེམས་ཏེ་ — «обсуждая», «обдумывая»; ཡོད་དེ་ — «бывая»; རྒྱུག་སྟེ་ — «бегая»; འགྲེང་སྟེ་ — «остановившись»; སློབ་སྟེ་ — «уча»; སྒོམ་སྟེ་ — «представляя себе», «воображая», «упражняясь»; སྨྲ་སྟེ་ — «говоря». */
            
            ostringstream out; out<<"i"<<i;
            string idLine=out.str();
            
            str=str_replace("^","་",str);
            str="<a3>"+text[i]+"</a3>"+"\n@<img src=\"/work_file/_img/line.gif\" onClick=\"editText(\""+idLine+"\")\"/><span id=\""+idLine+"\">"+str+"</span>\n";
            //+"<img src=\"/work_file/_img/lineEnd.gif\" onClick=\"editText(\""+idLine+"\")\">";
            textResult.push_back(str);
        }else{
            str="<a3>"+text[i]+"</a3>";
            textResult.push_back(str);
        }
        
    }
    textResult.push_back("<style>body{color://909090;}</style>");
    text=textResult;
    cout_<<"done translation";
};


