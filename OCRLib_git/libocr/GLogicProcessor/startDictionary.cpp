#include "GLogicProcessor.h"
#include "GMemory.h"

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
			if(inputData.data["ocrData"]=="RTFToYagpo"){
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
			cout<<"join "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<END;
			//strVector.resize(0);
			//readText(strVector,inputData.data["inputFile"]);			
			loadDictLevelFile();
			
			/*cout<<"strVector.size()="<<strVector.size()<<END;
			 int index=0,step=0; 
			 for(int i = 0; i<strVector.size(); i++) {
			 strVector[i]=str_replace("\r", "", strVector[i]);
			 vector <string> stringItemVector;
			 stringItemVector=explode(":|:", strVector[i]);
			 if(stringItemVector.size()<2)continue;
			 
			 //cout<<"stringItemVector[1]="<<stringItemVector[1]<<" wordCount="<<
			 //mainDict[stringItemVector[1]].wordCount<<" strVector[0]="<<atoi(stringItemVector[0].c_str())<<END;
			 
			 mainDict[stringItemVector[1]].wordCount+=atoi(stringItemVector[0].c_str());
			 if(step==100000){
			 cout<<index<<" mainDict.size()="<<mainDict.size()<<END;
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
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<END;
			strVector.resize(0); 
			mainDict.clear();
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<END;
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
            cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<END;
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
		cout<<"buildDictFromText  fileList.size()="<<inputData.fileList.size()<<END;
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<END;
			strVector.resize(0); 
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<END;
			//buildWordDictionary(strVector);
            inputData.data["ocrData"]="TibetanUTFToEng";
            buildTranslationDictionary(strVector);
			//cout<<"mainDict.size()="<<mainDict.size()<<END;
		}
		//cout<<"start save dictionary"<<END;
		//writeDictionaryTXT( mainDict);
		
	}
    if(inputData.data["ocrData"]=="reloadDict"){
        //cout<<inputData.data["user_text"]<<"<br>\n";
        
        vector<string>newKey; newKey=explode("[",inputData.data["user_text"]);
        map<string,string>lineKey;
        int flagLang=0;
        string db;
        string ln="tib";
        if(ln=="tib"){
            if(inputData.data["ln"]=="rus")db="translationDictRus";
            if(inputData.data["ln"]=="eng")db="translationDictEng";
        }

        //cout<<"data="<<data.size()<<" strREPath="<<strREPath; exit(0);

        
        for(int i=0;i<newKey.size();i++){
            string line =str_replace("]", "", newKey[i]);
            line=str_replace("/","#",line);          //разделителем в текстовой словарной статье может быть знак # или /
            line=str_replace("་\"","\"", line);
            line=str_replace("*","", line);
            if(line=="---")line="";
            vector<string>keyLine=explode("#",line);
            if(keyLine.size()!=2)continue;
            if(keyLine[0].find("་")!=-1)flagLang=1;
            lineKey[keyLine[0]]=keyLine[1]+"*";  //маркировка новой записи
        }

        
    
        int flagNewRecord=0;
        GVector *dk;
        GMap *d;
        GMemory *longMemory=(GMemory *)inputData.longMemory;
        
        longMemory->loadTable(db);
        dk=longMemory->table[db].data;
        indexRecord indexRec=longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
        d=indexRec.mIndex;
        
        
        map<string,string>::iterator it;
        for(it = lineKey.begin(); it!=lineKey.end(); it++) {
            string key=it->first;
            if(ln=="tib"){
                key+="་";
                key=str_replace("་་","་",key);
                cout<<"key="<<key<<" -> "<<it->second<<endl;
            }
            int index=d->getHKey(key,0);
            TString st;
            if(it->second=="*")key="";  //по запросу стираем запись
            st.push_back(key);
            st.push_back(it->second);
            if(index==-1){
                flagNewRecord=1;
                dk->push_back(&st);
            }else{
                dk->putTStr(index, &st);
            }
            
        }
        if(flagNewRecord)longMemory->reloadIndex(longMemory->table[db], 0, HASH_SEARCH);
        return "done save dictionary";
    }
    
	if(inputData.data["ocrData"]=="buildHashDict"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"txt");
		}
		cout<<"buildDictionaryHashIndex  fileList.size()="<<inputData.fileList.size()<<END;
		
		for(int i=0;i<inputData.fileList.size();i++){
			inputData.data["inputFile"]=inputData.fileList[i];
			cout<<"convert "<<i<<" from "<<inputData.fileList.size()<<" "<<inputData.fileList[i]<<END;
			strVector.resize(0); 
			readText(strVector,inputData.data["inputFile"]);
			//int step=0;
			cout<<"strVector.size()="<<strVector.size()<<END;
			//buildDictionaryHashIndexUni(strVector);
			buildDictionaryHashIndex();
			cout<<"mainDict.size()="<<mainDict.size()<<END;
		}
		cout<<"start save dictionary"<<END;
		//writeDictionaryTXT( mainDict);
	}

	
	return "";
	
}//////////////////////////////////////////////////////////////////////////

