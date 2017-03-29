#include "GLogicProcessor.h"
#include "GMemory.h"


void GLogicProcessor::readDictionary(){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str,pathDict;
	vector<string>fileList;
	string path;
	//read source file with tibetan string punctuation mark
	//cout_<<" inputData.data[wordsDataPath]="<<inputData.data["wordsDataPath"]<<endl;
	
    //readGrammarDataXML(inputData.data["wordsDataPath"]); 
    
    //не выключать! иначе не будет конвертировать в Yagpo. Нужно переписать на GVector, как например в setBase()
    if(inputData.data["ocrData"]=="batchOCR"||
       inputData.data["ocrData"]=="lineOCR"||
       inputData.data["ocrData"]=="letterBlock"||
       inputData.data["ocrData"]=="OCRTrainingOne"||
       inputData.data["ocrData"]=="OCRTrainingOneFile"||
       inputData.data["ocrData"]=="testOCRLine"||
       inputData.data["ocrData"]=="RTFToYagpo"||
       inputData.data["ocrData"]=="dWylieToYagpo"||
       inputData.data["ocrData"]=="WylieToYagpo"||
       inputData.data["ocrData"]=="YagpoToWylie"||
       inputData.data["ocrData"]=="YagpoToUnicode"||
       inputData.data["ocrData"]=="UnicodeToYagpo"||
       inputData.data["ocrData"]=="tibTextCorrector"||
       inputData.data["ocrData"]=="OCRTraining"){

         LoadMapXML();  //cout_<<"load1";   //load main font inicode conversion table from XML
         readGrammarDataXML(inputData.data["wordsDataPath"]); 
	}
    
    //LoadYagpoToWylieMap();  //cout_<<"load21";
    //readFontDataXML(inputData.data["wordsDataPath"]);
    
    //cout<<"LOAD";
    inputData.data["loadMode"]="OCR";
    
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    longMemory->loadDB();
    
    textCorpus=longMemory->textCorpus;
    textCorpusGMap=longMemory->textCorpusGMap;
    fontTable=longMemory->fontTable;
    fontGMap=longMemory->fontGMap;
    fontStackGMap=longMemory->fontStackGMap;
    

    for(int n=0;n<65000;n++)UniBigLetters[n]=n;
    UniBigLetters[0x0F90]=0x0F40;
    UniBigLetters[0x0F91]=0x0F41;
    UniBigLetters[0x0F92]=0x0F42;
    UniBigLetters[0x0F93]=0x0F43;
    UniBigLetters[0x0F94]=0x0F44;
    UniBigLetters[0x0F95]=0x0F45;
    UniBigLetters[0x0F96]=0x0F46;
    UniBigLetters[0x0F97]=0x0F47;
    UniBigLetters[0x0F98]=0x0F48;
    UniBigLetters[0x0F99]=0x0F49;
    UniBigLetters[0x0F9A]=0x0F4A;
    UniBigLetters[0x0F9B]=0x0F4B;
    UniBigLetters[0x0F9C]=0x0F4C;
    UniBigLetters[0x0F9D]=0x0F4D;
    UniBigLetters[0x0F9E]=0x0F4E;
    UniBigLetters[0x0F9F]=0x0F4F;
    UniBigLetters[0x0FA0]=0x0F50;
    UniBigLetters[0x0FA1]=0x0F51;
    UniBigLetters[0x0FA2]=0x0F52;
    UniBigLetters[0x0FA3]=0x0F53;
    UniBigLetters[0x0FA4]=0x0F54;
    UniBigLetters[0x0FA5]=0x0F55;
    UniBigLetters[0x0FA6]=0x0F56;
    UniBigLetters[0x0FA7]=0x0F57;
    UniBigLetters[0x0FA8]=0x0F58;
    UniBigLetters[0x0FA9]=0x0F59;
    UniBigLetters[0x0FAA]=0x0F5A;
    UniBigLetters[0x0FAB]=0x0F5B;
    UniBigLetters[0x0FAC]=0x0F5C;
    //UniBigLetters[0x0FAD]=0x0F5D;
    UniBigLetters[0x0FAE]=0x0F5E;
    UniBigLetters[0x0FAF]=0x0F5F;
    UniBigLetters[0x0FB0]=0x0F71;
    //UniBigLetters[0x0FB1]=0x0F61;
    //UniBigLetters[0x0FB2]=0x0F62;
    UniBigLetters[0x0FB3]=0x0F63;
    UniBigLetters[0x0FB4]=0x0F64;
    UniBigLetters[0x0FB5]=0x0F65;
    UniBigLetters[0x0FB6]=0x0F66;
    UniBigLetters[0x0FB7]=0x0F67;
    UniBigLetters[0x0FB8]=0x0F68;
    UniBigLetters[0x0FB9]=0x0F69;
    UniBigLetters[0x0FBA]=0x0F6A;
    UniBigLetters[0x0FBB]=0x0F61;
    UniBigLetters[0x0FBC]=0x0F62;
    UniBigLetters[0x0F71]=0x0F60;

	dictionaryReady=1;
    //cout_ <<"dict loaded1"<<endl;
	
	
}//////////////////////////////////////////////////////////////////////////////////////////////////


void GLogicProcessor::buildDictionaryHashIndex(){
	
	cout_<<"start build hash"<<endl;
	//unsigned long size_, size_hash;
	unsigned int *hashBuffer; 
	vector<hashRecord> hash_Vector;
#define MEM_CHINK_INT 268435456
#define MEM_CHINK_CHAR 1073741824
	
	cout_<<"start build hash index from text"<<endl;
	
	FILE * hFile;
	string pathHash=inputData.data["root"]+HASH_PATH;
	string path=pathHash;	path+="/sortTextDictionary.txt";			
	hFile = fopen(path.c_str(), "r");
	unsigned long  sizeText;
	if (hFile == NULL){
		cout_<<"no read text dictionary. Need sortTextDictionary.txt";
		return; 
	}else{
		fseek(hFile, 0, SEEK_END);
		sizeText= ftell(hFile);   //cout_<<" size="<<size<<endl;   
		fseek(hFile, 0, SEEK_SET);
		textBuffer = (char *)malloc(sizeText+1);
		if(textBuffer==NULL)exit(0);
		if (sizeText != fread(textBuffer, sizeof(char), sizeText, hFile)){
			free(textBuffer);
			exit(0); 
		}
		fclose(hFile);
	}
	cout_<<"done read textBuffer="<<sizeText<<endl;
	
	
	char c;
	char *a;
	char *startIndexPtr=textBuffer;
	unsigned int startIndex,ln,offset, index=0;
	unsigned int byteAdress, adress,adressInt;
	char memBlock[4048];
	string srcStr,srcLine,str,testLine;
	vector<string>stringItemVector,keyIndex;
	int step=0,match;
	
	unsigned long hash_Size=536870912+512, hashCount=0;
	hashData0=(char*)malloc(hash_Size);
	hashData1=(char*)malloc(hash_Size);
	if(hashData0==NULL||hashData1==NULL){cout_<<"no memory for hash"<<endl; exit(0);} 
	memset(hashData0,0,hash_Size);
	memset(hashData1,0,hash_Size);
	startIndex=0;
	startIndexPtr=textBuffer;
	
	for(int i=0;i<sizeText;i++){
		c=textBuffer[i];
		//cout_<<"c="<<c<<endl;
		if(c=='\n'){  //if(index>837153)cout_<<index<<" ";
			ln=i-startIndex;
			//next we set all part of string in hash index
			//cout_<<"ln="<<ln<<endl;
			memset(memBlock,0,4048);                                
			if(ln>4044){
				cout_<<"too long line "<<index<<endl; 
				startIndex=i+1;
				index++;
				startIndexPtr=textBuffer+i+1;
				continue;
			}
			memcpy(memBlock,startIndexPtr,ln);
			//cout_<<"mem="<<memBlock<<"/ st.size="<<ln<<endl;
			srcStr=memBlock; 
			
			//if(index>837153)cout_<<" srcStr="<<srcStr<<endl;
			stringItemVector=explode(" ",srcStr);  if(stringItemVector.size()>200){cout_<<"too long line "<<index<<endl; continue;}
			
			srcStr=str_replace(" ", "", srcStr);  //remove all delimeters from key string
			//cout_<<" srcStr__="<<srcStr<<endl;   
			
			//заполняем первый хеш
			adressInt=strToHash(srcStr.c_str(),(int)srcStr.size());
			byteAdress=adressInt>>3; 
			adress=adressInt-(byteAdress<<3);  			
			a=hashData0+byteAdress;	       			
			*a=(1<<adress)|*a; //put bite on adress of hash
            hashCount++;
		
			//заполняем второй хеш
			adressInt=RSHash(srcStr.c_str(),(int)srcStr.size());
			byteAdress=adressInt>>3; 
			adress=adressInt-(byteAdress<<3);  			
			a=hashData1+byteAdress;	       			
			*a=(1<<adress)|*a; //put bite on adress of hash
			
			//put data in hash address table
			//cout_<<" adressInt="<<adressInt<<endl;	
			hashRecord rec;
			rec.hash=adressInt;
			rec.adress=startIndex;     //if(adressInt==4114664782)cout_<<"@@@match1 srcStr="<<srcStr<<"offset="<<startIndex<<endl;
			hash_Vector.push_back(rec);
			
			//cout_<<"start report stringItemVector.size()="<<stringItemVector.size()<<endl;
			
			for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
				
				for (int k=(int)stringItemVector.size();k>entry;k--){ //for every syllabon in List
					ln=0; testLine="";
					for (int c=entry;c<k;c++){
						testLine+=stringItemVector[c];   //all syllabons in this part of source string
						//ln++;
					}
					//cout_<<"testLine=/"<<testLine<<"/"<<endl; 
					//заполняем первый хеш
					adressInt=strToHash(testLine.c_str(), (int)testLine.size());
					byteAdress=adressInt>>3; 
					adress=adressInt-(byteAdress<<3);  
					a=hashData0+byteAdress;
					//read bite from adress
					match = (*a>>adress)&1;
					//cout_<<"data="<<data<<endl;
					if(match)continue;
					//put bite on adress of hash
					*a=(1<<adress)|*a;
					hashCount++;
					/*
					//заполняем второй хеш
					adressInt=RSHash(testLine.c_str(), testLine.size()); 
					byteAdress=adressInt>>3; 
					adress=adressInt-(byteAdress<<3);  
					a=hashData1+byteAdress;
					//read bite from adress
					match = (*a>>adress)&1;
					//cout_<<"data="<<data<<endl;
					if(match)continue;
					//put bite on adress of hash
					*a=(1<<adress)|*a;
					*/
					
					//put data in hash addres table
					offset=0;
					for (int c=0;c<entry;c++){
						offset+=stringItemVector[c].size()+1;   //all syllabons in this part of source string
					}
					offset+=startIndex;
					
					//cout_<<" adressInt="<<adressInt<<endl;	
					hashRecord rec;
					rec.hash=adressInt;
					rec.adress=offset;
					hash_Vector.push_back(rec);    //if(adressInt==4114664782)cout_<<"@@@match srcStr="<<srcStr<<"offset="<<offset<<endl;
				}//end for (int k=StringItemList->Count
			}//end for (int entry=0;
			
			//for(int a=0;a<ln;a++){cout_<<"memBlock["<<a<<"]="<<(short)memBlock[a]<<" str["<<a<<"]="<<(short)str[a]<<endl;}
			//cout_<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<" hash="<<adressInt<<endl;
			
			
			startIndex=i+1;
			index++;
			startIndexPtr=textBuffer+i+1;
			if(step==100000){
				cout_<<"."<<index<<" hashCount="<<hashCount<<endl;
			step=0;}step++;
		}
	}
	//dictRecordBase.resize(index);
	//fwrite(hashBuffer0, sizeof(char), size_hash, hFile1);
	
	free(textBuffer);
	cout_<<"done build index="<<index<<endl<<"start write on /hashIndex.txt"<<endl; 
	path=pathHash;	path+="/hashIndex0.txt";			
	writeText(hashData0,(int)hash_Size,path.c_str());
	free(hashData0);
	path=pathHash;	path+="/hashIndex1.txt";			
	writeText(hashData1,(int)hash_Size,path.c_str());
	free(hashData1);	
	
	//hashBuffer=(unsigned int *)malloc(MEM_CHINK_CHAR);
	cout_<<"strt sort hash ";
    sort(hash_Vector.begin(),hash_Vector.end());
	
	
    cout_<<"start write hash1 index on disk"<<endl;
	path=pathHash;	path+="/hashIndexRecord.txt";			
	hFile = fopen(path.c_str(), "w");
	hashBuffer=(unsigned int*)&hash_Vector[0];
	fwrite(hashBuffer, sizeof(hashRecord), hash_Vector.size(), hFile); 
	
	cout_<<"done build hash index"<<endl;

}//_______________________________________________________________________________________________

void GLogicProcessor::buildDictionaryHashIndexUni( vector<string>&strVector){
	
		
}//////////////////////////////////////////////////////////////////////////////////////////////////

string GLogicProcessor::sortKey(vector<dictKey>&keyArray,string &srcKey,int fullReport){
	string report="";
	string srcStr;
	int count;
	char cbuf[10];
	
	cout_<<" keyArray.size()="<<keyArray.size()<<" fullReport="<<fullReport<<endl;
	count=0;
	if(fullReport){
		if(fullReport==1){
			for(int i=0;i<keyArray.size();i++){
				//cout_<<"srcKey="<<srcKey<<endl;
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0;
				}
				//}
			}
		}
		if(fullReport==2){
			for(int i=0;i<keyArray.size();i++){
				//cout_<<"srcKey="<<srcKey<<endl;
				if(keyArray[i].ln=="pl")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0;
				}
				//}
			}
		}
		
	}else{
		
		for(int i=0;i<keyArray.size();i++){
			cout_<<"srcKey="<<srcKey<<" keyArray[i].dict="<<keyArray[i].dict<<endl;
			if(keyArray[i].dict=="YGP"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				//if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>3)break;
				}
				//}
			}
		}
		
		
		for(int i=0;i<keyArray.size();i++){
			//cout_<<"srcKey="<<srcKey<<endl;
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(srcKey==srcStr){
				if(keyArray[i].ln!="tb"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].dict=="MV"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				if(keyArray[i].ln=="sk"){
					report+=keyArray[i].key;
					report+="|&|";
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].dict=="YO"){
				if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
				if(keyArray[i].ln=="sk"){
					report+=keyArray[i].key;
					report+="|&|";
					report+=srcStr;
					report+="|&|";report+=keyArray[i].dict; report+="|&|";
					sprintf(cbuf,"%d",keyArray[i].id);
					report+=cbuf; report+=":|:";
					keyArray[i].flag=0; count++; if(count>2)break;
				}
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].ln=="sk"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0; count++; if(count>2)break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].ln=="pl"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;       count++; if(count>2)break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="RHD"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;	break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="TD"&&keyArray[i].flag){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0;	break;
			}
		}
		count=0;
		for(int i=0;i<keyArray.size();i++){
			//cout_<<"keyArray["<<i<<"].dict="<<keyArray[i].dict<<endl;
			if(keyArray[i].ln=="tb")srcStr=keyArray[i].key;
			if(keyArray[i].dict=="RYD"&&keyArray[i].flag&&keyArray[i].ln=="tb")srcStr=keyArray[i].srcKey;
			if(keyArray[i].dict=="RYD"&&keyArray[i].flag&&keyArray[i].ln=="ru"){
				report+=keyArray[i].key;
				report+="|&|";
				report+=srcStr;
				report+="|&|";report+=keyArray[i].dict; report+="|&|";
				sprintf(cbuf,"%d",keyArray[i].id);
				report+=cbuf; report+=":|:";
				keyArray[i].flag=0; count++; if(count>2)break;
			}
		}
	}
	//cout_<<"report="<<report<<endl;
	
	
	return report;
}//_________________________________________________________________________________________________




void GLogicProcessor::writeDictReportXML(vector <vector <OCRDictRecord> >&dictReportPage,string &binPath){
	xml_document doc;
	xml_node mainNode;
	xml_node book, data;
	//int wordID=0;
	//char cbuf[20];
	string str;
	/*
	 cout_<<"dictRecord size="<<dictReportPage.size();
	 
	 mainNode=doc.append_child();
	 mainNode.set_name("dictBase");
	 
	 for(int m=0;m<dictReportPage.size();m++) {
	 
	 
	 i=0;
	 while(i<dictReportPage[m].size()){
	 book=mainNode.append_child();
	 //sprintf(cbuf,"%d",i);
	 book.set_name("rec");
	 book.append_attribute("id")=dictReportPage[m][i].wordID=wordID;
	 book.append_attribute("tw")=dictReportPage[m][i].tibWord.c_str();
	 book.append_attribute("tk")=dictReportPage[m][i].tibKey.c_str();
	 book.append_attribute("ek")=dictReportPage[m][i].engKey.c_str();
	 book.append_attribute("rk")=dictReportPage[m][i].rusKey.c_str();
	 book.append_attribute("sk")=dictReportPage[m][i].sncKey.c_str();
	 book.append_attribute("w")=dictReportPage[m][i].Wylie.c_str();
	 book.append_attribute("dt")=dictReportPage[m][i].dict.c_str();
	 book.append_attribute("rt")=dictReportPage[m][i].rus_translit.c_str();
	 data=book.append_child();
	 data.set_name("r1");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].eentry.c_str());
	 data=book.append_child();
	 data.set_name("r2");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].rentry.c_str());
	 data=book.append_child();
	 data.set_name("r3");
	 data.append_child(node_pcdata).set_value(dictReportPage[m][i].example.c_str());
	 
	 wordID++; i++;
	 
	 }
	 
	 }
	 
	 */
	doc.save_file(binPath.c_str(),"\t", 1);
	
}//_____________________________________________________________________________

void GLogicProcessor::readDictionaryFileMakerXML(string &fileName){
	xml_node col,data,resultSet,dictRecordXML, st;
	xml_document doc;
	string str;
	int dInt=0;
	
	cout_<<fileName.c_str();
	
	/*
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="tibKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="engKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rusKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="sncKey" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="eentry" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rentry" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="example" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="dict" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="rus_translit" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="Wylie" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="ID" TYPE="NUMBER"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="in1" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="input" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="out1" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="output" TYPE="TEXT"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="search_count" TYPE="NUMBER"/>
	 <FIELD EMPTYOK="YES" MAXREPEAT="1" NAME="search_id" TYPE="NUMBER"/>
	 */
	/*
	 int step=0; int count=0;
	 if(!doc.load_file(fileName.c_str())){cout_<<fileName.c_str()<<" not loaded";exit(0);}
	 dictRecordXML = doc.child("FMPXMLRESULT");
	 // DM("xmlns " << dictRecordXML.attribute("xmlns").value()<<END);
	 //data = dictRecordXML.child("ERRORCODE");
	 //DM("ERRORCODE" << data.child_value()<<END);
	 resultSet = dictRecordXML.child("RESULTSET");
	 //DM("FOUND " <<resultSet.attribute("FOUND").value()<<END);
	 // Iterate through books
	 cout_<<"start parsing"<<endl;
	 //  cd ../../YagpoOCRProgram   yagpo_dict -dt tt t1
	 
	 int im=0;
	 for (xml_node row = resultSet.child("ROW"); row; row = row.next_sibling("ROW")){
	 //DM("RECORDID " << row.attribute("RECORDID").value()<<END);
	 OCRDictRecord record;
	 
	 col = row.child("COL");
	 data=col.child("DATA");
	 str=data.child_value();
	 //c_out<<"str1="<<str<<" -> ";
	 for(int m=1;m<delimeter.size();m++){
	 str=str_replace(delimeter[m].c_str()," ",str);
	 }
	 //cout_<<"  d  ";
	 for(int m=0;m<commonWords.size();m++){
	 str=str_replace(commonWords[m].c_str()," ",str);
	 }
	 ///c_out<<str<<endl; if(im>1000)exit(0);im++;
	 //dictIndex[str].push_back(dInt);
	 dictRecord[dInt]=record;
	 dictRecord[dInt].tibWord=data.child_value();
	 dictRecord[dInt].tibKey=str;
	 if(step>5000){cout_<<".";step=0;}step++;  count++;
	 //cout_<<record.engKey.c_str()<<endl; if(dInt>10)exit(0); dInt++;
	 
	 col = col.next_sibling("COL");//engKey
	 data=col.child("DATA");
	 //DM("engKey="<<data.child_value());
	 dictRecord[dInt].engKey=data.child_value();
	 col = col.next_sibling("COL");//rusKey
	 data=col.child("DATA");
	 //DM("rusKey="<<data.child_value());
	 dictRecord[dInt].rusKey=data.child_value();
	 col = col.next_sibling("COL");//sncKey
	 data=col.child("DATA");
	 //DM("sncKey="<<data.child_value());
	 dictRecord[dInt].sncKey=data.child_value();
	 col = col.next_sibling("COL");//eentry
	 data=col.child("DATA");
	 //DM("eentry="<<data.child_value());
	 dictRecord[dInt].eentry=data.child_value();
	 col = col.next_sibling("COL");//rentry
	 data=col.child("DATA");
	 //DM("rentry="<<data.child_value());
	 dictRecord[dInt].rentry=data.child_value();
	 col = col.next_sibling("COL");//example
	 data=col.child("DATA");
	 //DM("example="<<data.child_value());
	 dictRecord[dInt].example=data.child_value();
	 col = col.next_sibling("COL");//dict
	 data=col.child("DATA");
	 //DM("dict="<<data.child_value());
	 dictRecord[dInt].dict=data.child_value();
	 col = col.next_sibling("COL");//rus_translit
	 data=col.child("DATA");
	 //DM("rus_translit="<<data.child_value());
	 dictRecord[dInt].rus_translit=data.child_value();
	 col = col.next_sibling("COL");//Wylie
	 data=col.child("DATA");
	 //DM("Wylie="<<data.child_value());
	 dictRecord[dInt].Wylie=data.child_value();
	 col = col.next_sibling("COL");//ID
	 col = col.next_sibling("COL");//in1
	 dictRecord[dInt].recordFlag=1;   //validate this record
	 //col.remove_child("DATA");
	 data=col.child("DATA");
	 //str=":|:"+ str;
	 data.append_child(node_pcdata).set_value(str.c_str());
	 dInt++;
	 }
	 
	 //if(!dictIndex.size()){cout_<<fileName.c_str()<<" not loaded";exit(0);};
	 cout_<<"dict->size()="<<dictRecord.size()<<endl;
	 fileName+="1";
	 //doc.save_file(fileName.c_str(),"\t", 1);
	 writeDictionary(dictRecord,fileName);
	 cout_<<"Done";
	 */
}//////////////////////////////////////////////////////////////////////////////////////////////////


void GLogicProcessor::readGrammarDataXML(string &path){
	
	xml_node col,data,resultSet,GFontXML, st;
	xml_document doc;
	string str0,str1;
	//unsigned int dInt,in;
	//data=doc.append_child();
	//data.set_name("rec");
	//data.append_attribute("id")=">|<|\"|\'";
	//doc.save_file("/1.xml","\t", 1);
	
	if(!doc.load_file(path.c_str())){
		cout_<<path<<" not loaded"<<endl;exit(9);
	}
	
	resultSet = doc.child("GFont");
	// Iterate through books

	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		if(str1=="gT")grammarTib.push_back(str0);  //grammar partical
		if(str1=="dT")delimeterTib.push_back(str0);  //tibetan syllable delimeter
		if(str1=="iT")commentaryIndex.push_back(str0);     //commentary index in text
		if(str1=="gP")grammarPali.push_back(str0);
		if(str1=="dP")delimeterPali.push_back(str0);
	}
	
	//cout_<<"delimeterTib.size="<<delimeterTib.size()<<endl;
	//cout_<<"grammarTib.size="<<grammarTib.size()<<endl;
	//cout_<<"delimeterPali.size="<<delimeterPali.size()<<endl;
	//cout_<<"grammarPali.size="<<grammarPali.size()<<endl;
	
}//_____________________________________________________________________________


void GLogicProcessor::readFontDataXML(string &path){
	
	xml_node col,data,resultSet,GFontXML, st;
	xml_document doc;
	string str0,str1;
	//unsigned int dInt,in;
    
	if(!doc.load_file(path.c_str())){
		cout_<<path<<" not loaded"<<endl;exit(9);
	}
	
	resultSet = doc.child("GFont");
	// Iterate through books
	//int index=0;
	
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		if(str1=="lM")letterSign.push_back(str0);  //vowels
		if(str1=="sL")letterScale.push_back(str0);  //letters for scale detector
	}
	
	//cout_<<"delimeterTib.size="<<delimeterTib.size()<<endl;
	//cout_<<"grammarTib.size="<<grammarTib.size()<<endl;
	//cout_<<"delimeterPali.size="<<delimeterPali.size()<<endl;
	//cout_<<"grammarPali.size="<<grammarPali.size()<<endl;
	
}//_____________________________________________________________________________


/** @brief Text alanysis function. Buld matrix of syllable probability */
void  GLogicProcessor::buildSyllableStat(){
	string str;
    string path="/Volumes/2TB_N2/_CORPUS/CORPUS1_LINE";
    string ext="txt";
    vector<string>fileList;
    readDirectoryToArray(fileList, path, ext);
    path=inputData.data["grammar"]+"_allSillablesClean2012.txt";
    vector<string> morphemeList;
    readText(morphemeList, path);
    cout<<"morphemeList.size()="<<morphemeList.size()<<endl;
    // Declare size of two dimensional array and initialize.
    vector< vector<string> > morphemeVector(100, vector<string>());
    for(int i=0;i<morphemeList.size();i++){
        morphemeVector[morphemeList[i].size()].push_back(morphemeList[i]);
    }
    morphemeList.resize(0);
    for(int i=99;i>-1;i--){
        //cout<<"i="<<i<<" size="<<morphemeVector[i].size()<<endl;
        for(int n=0;n<morphemeVector[i].size();n++){
           morphemeList.push_back(morphemeVector[i][n]);
           //cout_<<morphemeVector[i][n]<<endl;
        }
    }
    //cout<<"size="<<morphemeList.size()<<endl;
    vector<int>morphemeStat;
    morphemeStat.resize(morphemeList.size());
    
    //TIME_START
    cout_<<"@@@textCount="<<fileList.size();
	for(int i=0;i<fileList.size();i++){
        cout<<"@ process"<<fileList[i];
        vector<string>text;
        readText(text,fileList[i]);
        cout<<" "<<text.size()<<" lines"<<endl;
        int step=0;
        for(int n=0;n<text.size();n++){
           if(step==1000){cout<<".";step=0;}step++;
           string line=text[n];
           //cout<<line<<endl;
           for(int t=0;t<morphemeList.size();t++){
               if(line.find(morphemeList[t])!=-1){
                   line=str_replace(morphemeList[t].c_str(),"", line);
                   morphemeStat[t]++;
               }
           }
        }
        ostringstream out;
        for(int n=0;n<morphemeList.size();n++){
            out<<morphemeStat[n]<<" "<<morphemeList[n]<<endl;
        }
        path="/_allSillablesClean2013.txt";
        string data=out.str();
        writeText(data, path);
	}
}


void GLogicProcessor::writeDictionaryTXT(  map<string, OCRDictRecord>&mainDict){
    
    ofstream srcout_put;
    string str=inputData.data["inputFile"];
    str+="_Dict.txt";
    srcout_put.open(str.c_str());
    int step=0,index=0,count=0;
    map<string, OCRDictRecord>::iterator p;
    string mainString;
    char buff[10];
    
    for(p = mainDict.begin(); p != mainDict.end(); p++) {
        if (step==100000){
            srcout_put<<mainString; mainString="";
            cout_<<"save line "<<index<<" word count "<<count<<endl;step=0;
        }
        step++;index++;
        if(p->second.wordCount>0){
            sprintf(buff,"%d",p->second.wordCount );
            mainString+=buff;
            mainString+=":|:";
            mainString+=p->first;
            mainString+="\n";
            count++;
            
        }
    }
    srcout_put<<mainString;
    srcout_put.close();
    cout_<<"dict save done."<<endl;
    
}//////////////////////////////////////////////////////////////////////////


void GLogicProcessor::loadDictLevelFile(){
    ifstream srcInput;
    string str=inputData.data["inputFile"];
    srcInput.open(str.c_str());
    string srcLine;
    int index=0,step=0,count=0;
    
    
    while(getline(srcInput, srcLine,'\n')){
        srcLine=str_replace("\r", "", srcLine);
        vector <string> stringItemVector;
        stringItemVector=explode(":|:", srcLine);
        if(stringItemVector.size()<2)continue;
        
        //cout_<<"stringItemVector[1]="<<stringItemVector[1]<<" wordCount="<<
        //mainDict[stringItemVector[1]].wordCount<<" strVector[0]="<<atoi(stringItemVector[0].c_str())<<endl;
        count=atoi(stringItemVector[0].c_str());   //cout_<<"count="<<count<<endl;
        if(count>20){
            mainDict[stringItemVector[1]].wordCount+=count;
            if(step==100000){
                cout_<<index<<" mainDict.size()="<<mainDict.size()<<endl;
                step=0;
            }
            step++;
        }index++;
    }
    srcInput.close();
}//________________________________________________

void GLogicProcessor::writeDictionary(map<int, OCRDictRecord> &dictRecord, string &filePath){
    int i, step=0, count=0;
    
    int wordID=0;
    //char cbuf[20];
    string str;
    int allDictSaved=0;
    //map<int, OCRDictRecord>::iterator in=dictRecord.begin();
    string dictFlag="";
    int recordFound;
    int p;
    cout_<<"start write dictionary. DictRecord size="<<dictRecord.size()<<endl;
    
    while(!allDictSaved){
        xml_document doc;
        xml_node mainNode;
        xml_node book, data, key;
        mainNode=doc.append_child();
        mainNode.set_name("dictBase");
        i=0; wordID=0;
        recordFound=0;
        
        while(i<dictRecord.size()){
            if(dictRecord[i].dict=="")dictRecord[i].dict="ND";
            if(dictRecord[i].dict.size()>5||dictRecord[i].dict.size()<2)dictRecord[i].dict="ND";
            
            if(!dictRecord[i].recordFlag){i++;continue;}
            if(dictFlag==""){
                dictFlag=dictRecord[i].dict;
                cout_<<"now save "<<dictFlag<<" dict"<<endl;
            }
            if(dictRecord[i].dict==dictFlag){
                book=mainNode.append_child();
                //sprintf(cbuf,"%d",i);
                book.set_name("rec");
                wordID=i;
                book.append_attribute("id")=(unsigned int)i;
                book.append_attribute("dt")=dictRecord[i].dict.c_str();
                book.append_attribute("lv")=dictRecord[i].level;
                
                for(p =0;p<dictRecord[i].reportKey.size(); p++) {
                    key=book.append_child();  key.set_name("key");
                    key.append_attribute("key")=dictRecord[i].reportKey[p].key.c_str();
                    key.append_attribute("ln")=dictRecord[i].reportKey[p].ln.c_str();
                    if(dictRecord[i].reportKey[p].context!="")
                        key.append_attribute("c")=dictRecord[i].reportKey[p].context.c_str();
                    if(dictRecord[i].reportKey[p].grm!="")
                        key.append_attribute("grm")=dictRecord[i].reportKey[p].grm.c_str();
                    if(dictRecord[i].reportKey[p].level!=0)
                        key.append_attribute("lv")=dictRecord[i].reportKey[p].level;
                }
                if(dictRecord[i].eentry.size()){
                    data=book.append_child();
                    data.set_name("r1");
                    data.append_child(node_pcdata).set_value(dictRecord[i].eentry.c_str());
                }
                if(dictRecord[i].rentry.size()){
                    data=book.append_child();
                    data.set_name("r2");
                    data.append_child(node_pcdata).set_value(dictRecord[i].rentry.c_str());
                }
                if(dictRecord[i].example.size()){
                    data=book.append_child();
                    data.set_name("r3");
                    data.append_child(node_pcdata).set_value(dictRecord[i].example.c_str());
                }
                if(dictRecord[i].example.size()){
                    data=book.append_child();
                    data.set_name("exp");
                    data.append_child(node_pcdata).set_value(dictRecord[i].exp.c_str());
                }
                dictRecord[i].recordFlag=0;
                //wordID++;
                if(step>5000){cout_<<"."<<wordID<<endl; step=0;}step++;  count++;
                recordFound=1;
            } i++;
        }
        if(recordFound){
            string path=filePath+"/"+dictFlag+".xml";
            cout_<<"start save"<<path<<endl;
            doc.save_file(path.c_str(),"\t", 1);
            cout_<<wordID<<"records in "<<path<<" saved"<<endl;
            dictFlag="";
        }else{allDictSaved=1;cout_<<"all done";};
    }
    
    
}//////////////////////////////////////////////////////////////////////////

