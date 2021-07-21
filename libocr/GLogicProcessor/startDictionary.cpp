#include "GLogicProcessor.h"
#include "../GMemory/GMemory.h"

string GLogicProcessor::startDictionary(){

	string fileName;
	string srcLine, report,srcString;
	//int fullReport=0;
	string inputLine;
	//struct stat attrib;			// create a file attribute structure
	//int time=0,time1=0;
	//int step=0,i,j;

	int index=0;
	DIR *dir;
	int fileFlag;
	
	while(index<inputData.fileList.size()){
		if( ( dir=opendir(inputData.fileList[index].c_str()))!=NULL){
			if(inputData.data["ocrData"]=="RTFToUnicode"){
			    fileFlag=readDirectoryToArray(inputData.fileList, inputData.fileList[index],"rtf");
			}else{
			    fileFlag=readDirectoryToArray(inputData.fileList, inputData.fileList[index],"txt");
			}
			inputData.fileList.erase(inputData.fileList.begin()+index);
			continue;
		}
		index++;
	}
	
	
    string dictPath="";

	if(inputData.data["ocrData"]=="joinDict"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
		}
		cout<<"fileList.size()="<<inputData.fileList.size();
		strVector.resize(0); 
		mainDict.clear();
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"join "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			//strVector.resize(0);
			//readText(strVector,inputData.data["inputFile"]);			
			loadDictLevelFile();
			
			/*cout<<"strVector.size()="<<strVector.size()<<endl;
			 int index=0,step=0; 
			 for(int i = 0; i<strVector.size(); i++) {
			 strVector[i]=str_replace("\r", "", strVector[i]);
			 vector <string> stringItemVector;
			 stringItemVector=explode(":|:", strVector[i]);
			 if(stringItemVector.size()<2)continue;
			 
			 //cout<<"stringItemVector[1]="<<stringItemVector[1]<<" wordCount="<<
			 //mainDict[stringItemVector[1]].wordCount<<" strVector[0]="<<atoi(stringItemVector[0].c_str())<<endl;
			 
			 mainDict[stringItemVector[1]].wordCount+=atoi(stringItemVector[0].c_str());
			 if(step==100000){
			 cout<<index<<" mainDict.size()="<<mainDict.size()<<endl;
			 step=0;
			 }step++;index++;
			 }	
			 */	
			
		}
		inputData.data["inputFile"]=inputData.data["inputFolder"];
		cout<<"start save dictionary"<<endl;
		writeDictionaryTXT( mainDict);
		
	}
	if(inputData.data["ocrData"]=="buildDict"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
		}
		cout<<"fileList.size()="<<inputData.fileList.size();
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			strVector.resize(0); 
			mainDict.clear();
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<endl;
			buildDictionary(strVector);
		}
		
	}
    
    if(inputData.data["ocrData"]=="TibetanToEngTagger"){
       inputData.data["ocrData"]="TibetanToRusTagger";
       inputData.data["ln"]="eng";
    }
	
	if(inputData.data["ocrData"]=="TibetanToRusTagger"){
        if(!inputData.fileList.size()){
            readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
        }
        for(int i=0;i<inputData.fileList.size();i++){
            inputData.data["inputFile"]=inputData.fileList[i];
            cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
            string textData;
            readText(textData,inputData.data["inputFile"]);
            //int step=0;
            mainTextTranslation(textData);
            string maket;
            readText(maket, "/_Image2OCR/edit/OSBL_Dictionary.html");
            maket=str_replace("@@@TEXT@@@", textData, maket);
            string path=inputData.data["inputFile"]+"_out.html";
            writeText(maket,path);
        }        
        return "done translation";
	}

	
	if(inputData.data["ocrData"]=="buildDictFromText"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
		}
		cout<<"buildDictFromText  fileList.size()="<<inputData.fileList.size()<<endl;
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			strVector.resize(0); 
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<endl;
			//buildWordDictionary(strVector);
            inputData.data["ocrData"]="TibetanUTFToEng";
            buildTranslationDictionary(strVector);
			//cout<<"mainDict.size()="<<mainDict.size()<<endl;
		}
		//cout<<"start save dictionary"<<endl;
		//writeDictionaryTXT( mainDict);
		
	}
    if(inputData.data["ocrData"]=="reloadDict"){
  
    }
    
	if(inputData.data["ocrData"]=="buildHashDict"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
		}
		cout<<"buildDictionaryHashIndex  fileList.size()="<<inputData.fileList.size()<<endl;
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<endl;
			strVector.resize(0); 
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<endl;
			//buildDictionaryHashIndexUni(strVector);
			buildDictionaryHashIndex();
			cout<<"mainDict.size()="<<mainDict.size()<<endl;
		}
		cout<<"start save dictionary"<<endl;
		//writeDictionaryTXT( mainDict);
	}

	
	return "";
	
}//////////////////////////////////////////////////////////////////////////

