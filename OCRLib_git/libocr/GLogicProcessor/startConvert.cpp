#include "GLogicProcessor.h"

string GLogicProcessor::startConvert(){
	//cout<<"Start inputData.data[\"inputFolder\"]"<<inputData.data["inputFolder"]<<endl;
	string str,path;
	vector<string> strVector;     
	
	 int index=0;
     DIR *dir;
	int fileFlag;
    string ocrData=inputData.data["ocrData"];

	 while(index<inputData.fileList.size()){
		if( ( dir=opendir(inputData.fileList[index].c_str()))!=NULL){
			if(ocrData=="RTFToYagpo"){
			    fileFlag=readDirectoryToArray(inputData.fileList, inputData.fileList[index],"rtf");
			}else{
			    fileFlag=readDirectoryToArray(inputData.fileList, inputData.fileList[index],"txt");
			}
			inputData.fileList.erase(inputData.fileList.begin()+index);
			continue;
		}
		index++;
	}

    if(ocrData=="YagpoToWylie"){
        string mainString;
        if(inputData.data["InputMethod"]=="fileList"){
            cout<<"YagpoToWylieConverter inputData.fileList.size()="<<inputData.fileList.size()<<endl;
            for(int i=0;i<inputData.fileList.size();i++){
                strVector.resize(0);
                path=inputData.fileList[i]+"_out.txt";
                readText(strVector, inputData.fileList[i]);
                mainString="";
                for(int i=0;i<strVector.size();i++){
                    mainString+=TibUniToWylie(strVector[i],2);
                    mainString+="\n";
                }
                writeText(mainString, path);
                cout<<"done convert";
            }
        }else{
            mainString="";
            for(int i=0;i<inputData.fileList.size();i++){
                mainString+=TibUniToWylie(inputData.fileList[i],2);
                mainString+="\n";
            }
            return mainString;
        }
    }
    
	if(ocrData=="CXS_to_UTF"){
		loadMapFilePali("CXS_UTF_HTML.xml");
		cout<<"fileList.size()="<<inputData.fileList.size();
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			strVector.resize(0);
			readText(strVector,inputData.data["inputFile"]);
			for(int m=0;m<strVector.size();m++){
				//strVector[m]= regex_replace(  strVector[m], date, format );
				strVector[m]=Unicode_to_UTF(strVector[m]);
				convertCXS_to_UTF_nocopy(strVector[m]);
				//cout<<strVector[m];
			}
			writeText(strVector,inputData.data["inputFile"]);

		}

	}
	if(ocrData=="SinhalaUniToYagpo"){

		
	}
	if(ocrData=="ConcatenateFolder"){
		cout<<"fileList.size()="<<inputData.fileList.size();
		ofstream srcOutput;
		str=inputData.data["inputFolder"];
		str+="/allText.txt";
		srcOutput.open(str.c_str());
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			readText(str,inputData.data["inputFile"].c_str());
			cout<<"cat "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			srcOutput<<str<<endl;
		}
		srcOutput.close();
	}

	if(ocrData=="LowerCase"){
		loadMapFilePali("CXS_UTF_HTML.xml");
		cout<<"fileList.size()="<<inputData.fileList.size();
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			strVector.resize(0);
			readText(strVector,inputData.data["inputFile"]);
			int step=0;
			for(int m=0;m<strVector.size();m++){
				//strVector[m]= regex_replace(  strVector[m], date, format );
				//strVector[m]=Unicode_to_UTF(strVector[m]);
				lowerCase_nocopy(strVector[m]);
				if(step==1000){
					cout<<m<<"."<<strVector[m]<<endl;
				step=0;}step++;
			}
			writeText(strVector,inputData.data["inputFile"]);

		}

	}

	//cout<<"inputData.data[\"ocrData\"]="<<ocrData<<endl;

	if(ocrData=="PaliUTFToEng"){

		cout<<"PaliUTFToEng"<<endl;

	}
	if(ocrData=="RTFToYagpo"){
		ofstream c_out; c_out.open("/_out.txt");
		cout<<"RTFToYagpo inputData.fileList.size()="<<inputData.fileList.size()<<endl;
		string mainString;
		for(int i=0;i<inputData.fileList.size();i++){
			mainString="";
#ifdef COCOA
			RTFtoYagpoConverter(mainString,inputData.fileList[i]);
#endif
			string path=inputData.fileList[i]+"_out.txt";
			cout<<"path="<<path<<endl;
			writeText(mainString, path);
			c_out<<report;
		}
        return "done convert";
	}

	if(ocrData=="dWylieToYagpo"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"dWylieToYagpoConverter inputData.fileList.size()="<<inputData.fileList.size()<<endl;
            
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0); 
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
				int step=0;
				for(int i=0;i<strVector.size();i++){
					if(step==1000){cout<<i<<" ";step=0;}step++;
					mainString+=dWylieToYagpoConverter(strVector[i]);
                    mainString+="\n";
				}	
				writeText(mainString, path);
			}				
		}else{
			for(int i=0;i<inputData.fileList.size();i++){
			    mainString="";	
			    mainString+=dWylieToYagpoConverter(inputData.fileList[i])+"\n";   
			}
			return mainString;	
		}					
	}
	if(ocrData=="dSinhalaASCIToYagpo"){
		string mainString;
		string path=inputData.data["tablePath"]+"codePages/SinhalaASCI.xml";
		readMapXML(SinhalaASCI,path);
		cout<<"SinhalaASCI.size()="<<SinhalaASCI.size()<<endl;
		
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"dSinhalaASCIToYagpo inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			int step=0;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<endl;
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=dSinhalaASCIToYagpo(strVector[i])+"\n";
					if(step==1000){cout<<i<<" ";step=0;}step++;
				}
				writeText(mainString, path);
			}
		}else{
			int step=0;
			for(int i=0;i<inputData.fileList.size();i++){
				mainString="";
				mainString+=dSinhalaASCIToYagpo(inputData.fileList[i]);
				if(step==1000){cout<<".";step=0;}step++;
			}
			cout<<mainString<<endl;
		}
	}
	
	if(ocrData=="SinhalaUniToYagpo"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"SinhalaUniToYagpo inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			int step=0;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<endl;
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=SinhalaUniToYagpo(strVector[i],2)+"\n";
					if(step==1000){cout<<i<<" ";step=0;}step++;
				}
				writeText(mainString, path);
			}
		}else{
			int step=0;
			for(int i=0;i<inputData.fileList.size();i++){
				mainString="";
				mainString+=SinhalaUniToYagpo(inputData.fileList[i],2);
				if(step==1000){cout<<".";step=0;}step++;
			}
			return mainString;
		}
		
	}
	
	if(ocrData=="SinhalaMettaToYagpo"){
		string mainString;
		string path=inputData.data["tablePath"]+"codePages/SinhalaMetta.xml";
		readMapXML(SinhalaASCI,path);
		cout<<"SinhalaASCI.size()="<<SinhalaASCI.size()<<endl;
		
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"dSinhalaASCIToYagpo inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<endl;
				mainString=""; int step=0;
				for(int i=0;i<strVector.size();i++){  //cout <<"next string "<<i<<" ="<<strVector[i]<<endl;
					if(strVector[i].size()){
					   mainString+=SinghalaASCIToYagpo(strVector[i])+"\n";
					}else{mainString+="\n";}	
					if(step==1000){cout<<i<<" ";step=0;}step++;
				}
				writeText(mainString, path);
			}
		}else{   
			int step=0;
			for(int i=0;i<inputData.fileList.size();i++){
  				mainString="";
				mainString+=dSinhalaASCIToYagpo(inputData.fileList[i]);
				if(step==1000){cout<<".";step=0;}step++;
			}
			return mainString;
		}
		cout<<"DONE CONVERT";
	}
	

	if(ocrData=="WylieToYagpo"){
		string mainString; cout<<" @inputData.data[InputMethod]="<<inputData.data["InputMethod"]<<endl;
        loadTransliterationFile("TranslitTableUni_Wylie.xml");
        
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"WylieToYagpoConverter inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt"; cout<<" path="<<path;
				readText(strVector, inputData.fileList[i]);
                cout<<" strVector="<<strVector.size()<<endl;
				mainString="";
				int step=0;
                string str;
				for(int i=0;i<strVector.size();i++){ 
					if(step==100){cout<<i<<" ";step=0;}step++;
					str=WylieToYagpoConverter(strVector[i]);
                    mainString+=YagpoToUni(str);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=WylieToYagpoConverter(inputData.fileList[i])+"\n";
			}
			return mainString;
		}
	}

	if(ocrData=="TibUniToWylie"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"YagpoToWylieConverter inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=TibUniToWylie(strVector[i],1);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=TibUniToWylie(inputData.fileList[i],1);
				mainString+="\n";
			}
			return mainString;
		}
	}

	if(ocrData=="YagpoToWylie"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"YagpoToWylieConverter inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=TibUniToWylie(strVector[i],2);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=TibUniToWylie(inputData.fileList[i],2);
				mainString+="\n";
			}
			return mainString;
		}
	}

	if(ocrData=="YagpoToUnicode"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"YagpoToUnicode inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
                cout<<"strVector.size()="<<strVector.size()<<endl;
                int step=0;
				for(int n=0;n<strVector.size();n++){
                    if(step==strVector.size()/100){ step=0;cout<<".";}step++;
					mainString+=YagpoToUni(strVector[n]);
					mainString+="\n";
				}
				writeText(mainString, path);
                //return path;
			}
            return "done";
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=YagpoToUni(inputData.fileList[i]);
                //cout<<"mainString="<<mainString<<endl;
				mainString+="\n";
			}
			return mainString;
		}
	}

	if(ocrData=="UnicodeToYagpo"){
		string mainString;
			if(inputData.data["InputMethod"]=="fileList"){
			cout<<"YagpoToUnicode inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
                cout<<"convert "<<inputData.fileList[i]<<endl;
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
                int step=0;
				for(int n=0;n<strVector.size();n++){
					//mainString+=UnicodeToYagpo(strVector[n]);
                    if(step==100000){cout<<n<<" "; step=0;} step++;
                    mainString+=tibetanUTFToYagpo(strVector[n],1);
					mainString+="\n";
				}
				writeText(mainString, path);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=UnicodeToYagpo(inputData.fileList[i]);
				if(i)mainString+="\n";
			}
			return mainString;
		}
	}
    if(ocrData=="BonPDFToUni"){
		string mainString;
        if(inputData.data["InputMethod"]=="fileList"){
			cout<<"BonPDFToUni inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
                cout<<"convert "<<inputData.fileList[i]<<endl;
				path=inputData.fileList[i]+"_out.txt";
				readText(strVector, inputData.fileList[i]);
				mainString="";
				for(int n=0;n<strVector.size();n++){
					BonPDFToUni(strVector[n]);
                    mainString+=strVector[n];
					mainString+="\n";
				}
				writeText(mainString, path);
			}
            return "done";
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				BonPDFToUni(inputData.fileList[i]);
                mainString+=inputData.fileList[i];
				if(i)mainString+="\n";
			}
			return mainString;
		}
        
	}
    
    if(ocrData=="tibTextCorrector"){
        string mainString,path;
        readGrammarDataXML(inputData.data["wordsDataPath"]);
        if(inputData.data["mode"]!="text"){
            cout<<"TibetanCorrector inputData.fileList.size()="<<inputData.fileList.size()<<endl;
            cout<<" mode="<<inputData.data["mode"]<<endl;
            
            if(inputData.data["system"]=="process"){
                for(int i=0;i<inputData.fileList.size();i++){
                    cout<<"convert "<<inputData.fileList[i]<<endl;
                    inputData.data["fileName"]=inputData.fileList[i];
                    path=str_replace(".txt",".html",inputData.fileList[i]);
                    inputData.data["outFile"]=path;
                    TibetanCorrector();
                    string cmd="textutil -convert rtf \""+path+"\"";
                    cout<<cmd<<endl;
                    system(cmd.c_str());
                }
            }else{
                
                for(int i=0;i<inputData.fileList.size();i++){
                    //strVector.resize(0);
                    cout<<"convert "<<inputData.fileList[i]<<endl;
                    ostringstream out;
                    out<<inputData.data["rootApp"]<<" \"xml=<fileList>"<<inputData.fileList[i]<<
                    "</fileList><ocrData>"<<inputData.data["ocrData"]<<"</ocrData>"<<
                    "<ocrLn>"<<inputData.data["ocrLn"]<<"</ocrLn><system>process</system>\" &";
                    string cmd=out.str();
                    //cout<<cmd; exit(0);
                    system(cmd.c_str());
                }
            }
            return "done";
        }else{
            mainString=implode("\n",inputData.fileList);
            mainString=lineTibetanCorrector(mainString);
            return mainString;

        }
    }


    if(ocrData=="transcription"){
		string mainString,path;
        loadTransliterationFile("TranslitTableUni_Wylie.xml");
        if(inputData.data["InputMethod"]=="fileList"){
            cout<<"TranslitYagpoRus inputData.fileList.size()="<<inputData.fileList.size()<<endl;
            
            for(int i=0;i<inputData.fileList.size();i++){
                //strVector.resize(0);
                cout<<"convert "<<inputData.fileList[i]<<endl;
                inputData.data["fileName"]=inputData.fileList[i];
                path=inputData.fileList[i]+"_out.txt";
                inputData.data["outFile"]=path;
                Transcription();
            }
            return "done";
        }else{
            mainString="";
            for(int i=0;i<inputData.fileList.size();i++){
                mainString+=lineTranscription(inputData.fileList[i]);
                mainString+="\n";
            }
            return mainString;
        }
	}
    
    if(ocrData=="textNormalisation"){
		string mainString,path;
        
        if(inputData.data["InputMethod"]=="fileList"){
			cout<<"textNormalisation inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				//strVector.resize(0);
                cout<<"convert "<<inputData.fileList[i]<<endl;
                inputData.data["fileName"]=inputData.fileList[i];
                path=inputData.fileList[i]+"_out.txt";
                inputData.data["outFile"]=path;
                string srcStr;
                readText(srcStr,inputData.data["fileName"].c_str());
				//readText(strVector, inputData.fileList[i]);
				//mainString="";
				//for(int n=0;n<strVector.size();n++){
				//	mainString+=TranslitYagpo(strVector[n]);
                //		mainString+="\n";
				//}
				//writeText(mainString, path);
                textNormalisation(srcStr);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				//mainString+=TranslitYagpo();
				if(i)mainString+="\n";
			}
			return mainString;
		}
	}
    
	if(ocrData=="TXTtoXML"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"TXTtoXML inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i]+".xml";
				path=str_replace(".doc.txt", "" ,path);
				path=str_replace(".DOC.txt", "" ,path);
				readText(strVector, inputData.fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<endl;
				mainString="<text:text xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:text=\"http://www.tbrc.org/models/text#\" RID=\"\" lang=\"bo_ZH\" volume=\"\" num=\"\" fromWork=\"lam.dre\" fromVolume=\"\" start=\"0\" last=\"\">";
				for(int n=0;n<strVector.size();n++){
					if(strVector[n].find("FILE",0)==string::npos&&strVector[n].find("PAGE",0)==string::npos){
						//if(strVector[n].find("Corel",0)!=string::npos)cout<<inputData.fileList[i]<<endl;
					mainString+=strVector[n];
					mainString+="\n";
					}
				}
				mainString+="</text:text>";
				cout<<path<<endl;
				writeText(mainString, path);
			}
		}else{
			mainString="start\n";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=YagpoToUni(inputData.fileList[i]);
			}
			cout<<mainString<<endl;
		}
	}

	if(ocrData=="TXTtoHTML"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			cout<<"TXTtoHTML inputData.fileList.size()="<<inputData.fileList.size()<<endl;
			for(int i=0;i<inputData.fileList.size();i++){

				strVector.resize(0);
				path=inputData.fileList[i]+".xml";
				path=str_replace(".doc.txt", "" ,path);
				path=str_replace(".DOC.txt", "" ,path);
				readText(strVector, inputData.fileList[i]);
				cout<<"inputData.fileList[i]="<<inputData.fileList[i]<<" strVector.size()="<<strVector.size()<<endl;
				//continue;
				/*
				mainString="<text:text xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:text=\"http://www.tbrc.org/models/text#\" RID=\"\" lang=\"bo_ZH\" volume=\"\" num=\"\" fromWork=\"lam.dre\" fromVolume=\"\" start=\"0\" last=\"\">";
				for(int n=0;n<strVector.size();n++){
					if(strVector[n].find("FILE",0)==string::npos&&strVector[n].find("PAGE",0)==string::npos){
						//if(strVector[n].find("Corel",0)!=string::npos)cout<<inputData.fileList[i]<<endl;
						mainString+=strVector[n];
						mainString+="\n";
					}
				}
				mainString+="</text:text>";
				cout<<path<<endl;
				writeText(mainString, path);
				*/
			}
		}else{
			mainString="start\n";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=YagpoToUni(inputData.fileList[i]);
			}
			cout<<mainString<<endl;
		}
	}

	if(ocrData=="UTF8"){
		string mainString;
		if(inputData.data["InputMethod"]=="fileList"){
			DT("YagpoToUnicode inputData.fileList.size()="<<inputData.fileList.size()<<endl);
			for(int i=0;i<inputData.fileList.size();i++){
				strVector.resize(0);
				path=inputData.fileList[i];
				DT("convert path "<<path<<endl);
				readText(strVector, inputData.fileList[i]);
				mainString="";
				for(int n=0;n<strVector.size();n++){
					mainString+=Unicode_to_UTF(strVector[n]);
					mainString+="\n";
				}
				writeText(mainString, path);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData.fileList.size();i++){
				mainString+=YagpoToUni(inputData.fileList[i]);
				mainString+="\n";
			}
			return mainString;
		}
	}


return "";

}//________________________________________________________________________________________________________________

