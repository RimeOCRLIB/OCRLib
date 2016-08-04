#include "GLogicProcessor.h"

int GLogicProcessor::hashMatch(string &str){
	
	//проверяем первый хеш
	unsigned int hash=strToHash(str.c_str(), str.length()); 
	int byteAdress=hash>>3; 
	int adress=hash-(byteAdress<<3);   //cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<endl;
	char *c=hashData0+byteAdress;
	//put bite on adress//*c = (1<<adress)|*c;
	//read bite from adress
	int data0 = (*c>>adress)&1;
	/*
	//проверяем второй хеш
	hash=RSHash(str.c_str(), str.length()); 
	byteAdress=hash>>3; 
	adress=hash-(byteAdress<<3);   //cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<endl;
	c=hashData1+byteAdress;
	//put bite on adress//*c = (1<<adress)|*c;
	//read bite from adress
	int data1 = (*c>>adress)&1;
	*/
    /*		if(adressInt<1073741824){
     memset(memBlock,0,4);
     fseeko(hFile1, adressInt*4 , SEEK_SET); //cout<<"@1";
     fread(memBlock, sizeof(char), 4, hFile1);
     memcpy(&offset,memBlock,4);
     }
     else if(adressInt<2147483648){
     memset(memBlock,0,4);
     fseeko(hFile2, (adressInt-1073741824)*4, SEEK_SET);
     fread(memBlock, sizeof(char), 4, hFile2);
     memcpy(&offset,memBlock,4);
     }else if(adressInt<3221225472){
     memset(memBlock,0,4);
     fseeko(hFile3, (adressInt-2147483648)*4, SEEK_SET);
     fread(memBlock, sizeof(char), 4, hFile3);
     memcpy(&offset,memBlock,4);
     }else{
     memset(memBlock,0,4);
     fseeko(hFile4, (adressInt-3221225472)*4, SEEK_SET);
     fread(memBlock, sizeof(char), 4, hFile4);
     memcpy(&offset,memBlock,4);
     }
     */
	return data0;

}


string GLogicProcessor::remapString(string &str_){
	//cout<<"///_______@@@ start remapString="<<str_<<endl;
	const char *strKey;
	int partStr;
	int srcL;
	int sL,maxLength;
	int print=0;
	vector<string>strV=explode(":|:",str_);
	vector<stringEntry>matchArray;
	string testStr,matchStr,hashStr;
	
	if(strV.size()<2)return ":|:";
    DT(" strIn="<<str_<<END);
	strKey=strV[0].c_str(); sL=(uint)strV[0].size(); if(sL<1)return ":|:"; //if(strV[0][sL-1]==' ')sL--;
	DT("sL="<<sL<<endl);
	
	maxLength=100; //it that we store how much delimeters will be in match
	partStr=0; //first we check whole string match
	//strLeft=strV[0].c_str();  
	srcL=(uint)strV[1].size();
	
	//проверяем первый хеш	
	unsigned int adressInt=strToHash(strKey, sL);   DT("d1");
	unsigned int offset;
	int byteAdress=adressInt>>3; 
	int adress=adressInt-(byteAdress<<3);   //cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<endl;
	char *c=hashData0+byteAdress;                    DT("d2");
	int flag=0,d0=0,d1=0;
	
	if(*c){   //если ноль то значения нет во всех восьми битах. Если больше нуля проверяем нужный бит 
		//read bite from adress
		d0 = (*c>>adress)&1;
		//cout<<"data="<<data<<endl;                  
        /*
			//проверяем второй хеш	
			adressInt=RSHash(strKey, sL);   DT("d1");
			byteAdress=adressInt>>3; 
			adress=adressInt-(byteAdress<<3);   //cout<<"byteAdress="<<byteAdress<<" adress="<<dec<<adress<<endl;
			c=hashData1+byteAdress;                    DT("d2");
		*/	
			if(*c){   //если ноль то значения нет во всех восьми битах. Если больше нуля проверяем нужный бит 
				//read bite from adress
				d1 = (*c>>adress)&1;
				//cout<<"data="<<data<<endl;    
				
			DT("d3");
					if(d0){
							DT("@@@ match adressInt="<<adressInt<<END);

					#ifdef HASH_DICTIONARY		
							int index=bsearch(hashVector,hashSize,adressInt);
							DT("index="<<index<<" hashVector size="<<hashSize<<END);
							if(index>=0){
								DT("hashVector["<<index<<"].hash="<<hashVector[index].hash<<END);
								offset=hashVector[index].adress;
								DT("offset="<<offset<<END);
								flag=1;
							}else{
								//cout<<"no valid hash adress "<<adressInt<<END;
								return ":|:";
							}
					#endif
					#ifdef STRING_DICTIONARY
							DT("input str="<<strKey);
							for(int i=0;i<textDictionaryKey.size();i++){
								if(textDictionaryKey[i].find(strKey)!=string::npos){
									offset=i; DT("@@@match "<<strKey<<endl);
									flag=1;
									break;
								}
							
							}
						DT("flag="<<flag);
						if(!flag){cout<<"@HASH_ERROR";}
					#endif		

					//cout<<"done read"<<endl;
					//cout<<" offset="<<offset<<END;
					if(flag)return findStringMatch(strKey,sL, offset);
					//if(matchArray.size()!=0)bestMatchString(matchArray,strKey,sL,strV[1],destStr);
				}
		  }
	}
	return ":|:";
	
	

}//__________________________________________________________________________

string GLogicProcessor::findStringMatch(const char *strKey, int sL, int offset){
	
	char *keyPtr,c,*p;
	//char data[2049];
	int ln,startIndex,endIndex,strIndex,indexSrc,match;
	int print=0;
	string srcStr,destStr;
	DT("offset="<<offset<<END);
	//cout<<"textKeyIndex.size()="<<textKeyIndex.size()<<" start find string ";) 
	//memprint(strKey,sL); cout<<" @/@ ";

	indexSrc=offset;match=0;
	startIndex=0;endIndex=0; //ln=0;
	strIndex=0; //maxLength_=0;  startIndexMax=0; endIndexMax=0; endIndexSrc=0;

	
#ifdef HASH_DICTIONARY	
	//if(offset==32077293)print=1;
		//start check match for every key entry			
		keyPtr=textDictTib;
		
		//memprint(keyPtr+offset,100);cout<<"/ "<<endl;
		//cout<<"key"<<keyIndex<<"=";memprint(textKeyIndex[keyIndex].keyPtr,textKeyIndex[keyIndex].keySize);cout<<endl;
	
		while(indexSrc-offset<2048){///for every letter in text base entry
			DT(" indexSrc="<<indexSrc<<" sL="<<sL<<" strIndex="<<strIndex<<END);
			if(strIndex==sL){break;} //if end test string
			while(strIndex<sL){ ///for every letter in match
				DT(" strIndex="<<strIndex<<endl);
				c=strKey[strIndex];
				DT("c="<<(short)c<<"/ "<<c<<" // indexSrc="<<indexSrc<<END);
				//if(indexSrc==dL){match=0; break;}
				if(keyPtr[indexSrc]==c){ //match letter found
					DT("indexSrc="<<indexSrc<<" startIndex="<<startIndex<<" endIndex="<<endIndex<<" strIndex="<<strIndex<<endl);
					//if it is first match letter
					if(strIndex==0){
						startIndex=indexSrc;endIndex=indexSrc; match=1;
					}else{ 
						endIndex++;
					}
					indexSrc++;
					
					if(keyPtr[indexSrc]==' '){ //ok we catch first syllable and continue
					indexSrc++; endIndex++;
						ln=endIndex-startIndex;
					//if(ln>maxLength_&&ln>3){
					//	
					//	maxLength_=ln; 
					////	startIndexMax=startIndex; 
					//	endIndexMax=endIndex;
					//	endIndexSrc=strIndex;
					//}
					//	strIndex=0; startIndex=0; endIndex=0; 
					//	indexSrc++;
					//	break;
					} 
					strIndex++;
				}else{
					if(strIndex==0){ //first letter no match it is hash table error 
						DT("hash table error"<<endl);
					   return ":|:";	
						
					}
					match=0;
					ln=endIndex-startIndex;
					DT("no match"<<endl);
					//if(ln>maxLength_){
					//	maxLength_=ln; 
					//	startIndexMax=startIndex; 
					//	endIndexMax=endIndex;
					//	endIndexSrc=strIndex;
					//}
					strIndex=0; startIndex=0; endIndex=0; indexSrc++;
					break;
				}	
				
			}
			DT("match="<<match<<endl);
			if(match){
				stringEntry st;
				st.keyPtr=keyPtr+startIndex;
				st.keySize=indexSrc-offset;
				//st.strPtr=keyPtr+startIndex;
				//st.strSize=textKeyIndex[keyIndex].strSize;
			    //matchArray.push_back(st);
			    DT("match startIndex="<<startIndex<<" st.keySize="<<st.keySize<<" indexSrc="<<indexSrc<<" offset="<<offset<<endl);//memprint(st.keyPtr,st.keySize);cout<<endl;
				
				//memset(data, 0, 2048); 
				destStr.resize(st.keySize); DT("1/");
				p=&destStr[0];
				memcpy(p, st.keyPtr, st.keySize); DT("2/");
				//destStr=data; 
				DT(" destStr"<<destStr<<endl);  DT("21/");
				destStr=str_replace(" ", "་", destStr); DT("3/");
				return destStr;
				
			}
			
		}
#endif
#ifdef STRING_DICTIONARY	
	
	DT("str="<<textDictionary[offset]<<endl);
	srcStr=textDictionary[offset];
    indexSrc=0;
	string key,report;
	
	
	while(indexSrc<srcStr.size()){///for every letter in text base entry
		DT(" indexSrc="<<indexSrc<<" sL="<<sL<<END);
		if(strIndex==sL){break;} //if end test string
		while(strIndex<sL){ ///for every letter in match
			DT(" strIndex="<<strIndex<<endl);
			c=strKey[strIndex];
			DT("c="<<(short)c<<"/ "<<c<<" // indexSrc="<<indexSrc<<END);
			if(srcStr[indexSrc]==c){ //match letter found
				DT("@match indexSrc="<<indexSrc<<" startIndex="<<startIndex<<" endIndex="<<endIndex<<" strIndex="<<strIndex<<" key="<<key<<" report="<<report<<endl);
				//if it is first match letter
				if(strIndex==0){
					startIndex=indexSrc;endIndex=indexSrc; match=1;
				}else{ 
					endIndex++;
				}
				report+=c;
				indexSrc++;
				
				if(srcStr[indexSrc]==' '){ //ok we catch first syllable and continue
					indexSrc++; endIndex++;
					ln=endIndex-startIndex;
					report+="་";
					//if(ln>maxLength_&&ln>3){
					//	
					//	maxLength_=ln; 
					////	startIndexMax=startIndex; 
					//	endIndexMax=endIndex;
					//	endIndexSrc=strIndex;
					//}
					//	strIndex=0; startIndex=0; endIndex=0; 
					//	indexSrc++;
					//	break;
				} 
				strIndex++;
				key+=c;
			}else{
				DT("@no match indexSrc="<<indexSrc<<" startIndex="<<startIndex<<" endIndex="<<endIndex<<" strIndex="<<strIndex<<" key="<<key<<" report="<<report<<endl);
				startIndex++;
				indexSrc=startIndex;
				key=""; report="";
				strIndex=0;
				if(indexSrc==srcStr.size())break;
				/*if(strIndex==0){ //first letter no match it is hash table error 
					//DT("hash table error"<<endl);
					//return ":|:";	

				}
				*/ 
			}	
			
		}
		DT("match="<<match<<endl);
	
		if(match){
			return report;
		}	
		/*	stringEntry st;
			st.keyPtr=keyPtr+startIndex;
			st.keySize=indexSrc-offset;
			//st.strPtr=keyPtr+startIndex;
			//st.strSize=textKeyIndex[keyIndex].strSize;
			//matchArray.push_back(st);
			DT("match startIndex="<<startIndex<<" st.keySize="<<st.keySize<<" indexSrc="<<indexSrc<<" offset="<<offset<<endl);//memprint(st.keyPtr,st.keySize);cout<<endl;
			
			//memset(data, 0, 2048); 
			destStr.resize(st.keySize); DT("1/");
			p=&destStr[0];
			memcpy(p, st.keyPtr, st.keySize); DT("2/");
			//destStr=data; 
			DT(" destStr"<<destStr<<endl);  DT("21/");
			destStr=str_replace(" ", "་", destStr); DT("3/");
			return destStr;
			
		}
		*/
	}
	
	
#endif	
        return ":|:";
	
}//______________________________________________________


void GLogicProcessor::bestMatchString(vector<stringEntry>&matchArray,const char *strKey_,int sL, string &textString,  string &destStr){
	
	string srcStr,keyStr,strKey,str,key,result;	
	strKey=ptrToStr(strKey_,sL);
	int print=0;
	DT("start string match analizematchArray.size()="<<matchArray.size()<<" strKey="<<strKey<<endl); 
	int p=0;
	vector<string>strItem;
	vector<string>strResult;
	for(int a=0;a<matchArray.size();a++){
	    
		keyStr=ptrToStr(matchArray[a].keyPtr,matchArray[a].keySize);
	    srcStr=ptrToStr(matchArray[a].strPtr,matchArray[a].strSize);
		DT(" srcStr="<<srcStr<<endl);
		strItem=explode(" ",srcStr);
		str=strKey; result="";
		for(int index=0;index<strItem.size();index++){
		    key=str_replace(grammarTib,"", strItem[index]);
			result+=strItem[index]+"་";
			if(key=="")continue;
			DT("key="<<key<<endl);
			p=str.find(key,0);
			if(p!=0&&str!=""){
				DT("no match p="<<p<<endl);
				str=strKey;	
			    result="";
			    continue;
			}
				
			str.erase(0,key.size());
			DT("p="<<p<<" str="<<str<<endl);
			if(str==""){
				DT("match. result="<<result<<endl);
				strResult.push_back(result);
				break;
			}
		}
		//p=srcStr

	
	}
	int max=10000;
	if(strResult.size()==0)return;
	for(int a=0;a<strResult.size();a++){
		if(strResult[a].size()<max){max=(uint)strResult.size();result=strResult[a];}
	}	
		
	
	destStr=result.substr(0, result.size()-3);
	
}



string  GLogicProcessor::spellCheck( string &str_){
    int recordFound, length,c;
	string srcString=str_+" ";
	string srcLine,str,testLine,request,answer,oldString;
	//unsigned int hash;
	//char buffer[5];
    vector<wordOCR> stringItemVector;
	vector<string>wordList;
	vector<string>strResult;
	string strCount;
	int print=0;
	//static int startSocket=0;
	int dL;//stringFlag=0,
	
	
	//normalise tibetan string strip punctuation mark
	//for(int m=1;m<delimeterTib.size();m++){
	//	srcString=str_replace(delimeterTib[m].c_str()," ",srcString);
	//	//<<"delimeter["<<m<<"]"<<delimeter[m]<<" srcLine="<<srcLine<<endl;
	//}
	//if(!startSocket){
	//	request="start";
	//	dataIP(request,INIT_SOCKET);
	//	DT(" str="<<srcString<<END);
	//}startSocket++;
	
	
	wordList=explode(" ",str_); strResult.resize(wordList.size());
	for(int index=0;index<wordList.size();index++){
		//testLine=wordList[index];
		//testLine=str_replace(grammarTib, " ", testLine);
		//testLine=str_replace(delimeterTib, " ",testLine );
		
		//cout<<"testLine="<<testLine<<endl;
		stringItemVector=explode(delimeterTib,wordList[index]); //stringItemVector now contain all syllabons from string
		DT("start report stringItemVector.size()="<<stringItemVector.size()<<" str="<<wordList[index]<<END);
	startSpellCheker:;		
		for (int entry=0; entry<stringItemVector.size();entry++){ //for every syllabon in List
			
			for (int k=(uint)stringItemVector.size();k>entry;k--){ //for every syllabon in List
				recordFound=0;
				srcLine=""; str=""; length=0; request="";
				for (c=entry;c<k;c++){
					cout<<"stringItemVector["<<c<<"].name="<<stringItemVector[c].name<<" stringItemVector["<<c<<"].delimeter="<<stringItemVector[c].delimeter<<endl;
					if(stringItemVector[c].name!=stringItemVector[c].delimeter){
						str+=stringItemVector[c].name+stringItemVector[c].delimeter;
						srcLine+=stringItemVector[c].name;   //all syllabons in this part of source string
						request+=stringItemVector[c].name+stringItemVector[c].delimeter;
						dL=(uint)stringItemVector[c].delimeter.size();
						
					}else{
						str+=stringItemVector[c].name;
					}	
					length++;
				}
				if(request.size()>dL)request.erase(request.size()-dL,dL);
				testLine=srcLine;
				if(testLine==""){
					testLine=str;
				}	
				testLine+=":|:";
				testLine+=str;
				
				//for(int m=0;m<grammarTib.size();m++){
				//testLine=str_replace(grammarTib,"",testLine);
				//}
				DT(" srcLine="<<str<<" testLine=/"<<testLine<<" request="<<request<<"/ length="<<length<<END);
				
				//DT("size="<<dictIndexTib.size()<<END);
				//hash=strToHash(testLine.c_str(), testLine.length()); 
				//memcpy(buffer,&hash,4);buffer[4]=0;  
				//testLine=buffer;  
				//DT("test hash="<<hash<<END);
				
				//answer=dataIP(testLine,TIB_FROM_HASH);  //send data by TCP/IP socket on main dictionary program
				//answer=dataIP(testLine,REMAP_STRING_TIB);
				
				
				if(answer!=":|:"){
					recordFound=1;
					//str=answer.substr(0,answer.size()-3);
					str=str_replace(request,answer,str);
					DT("match found srcLine="<<srcLine<<" answer="<<answer<<" str="<<str<<END);
					strCount=str_replace("་","", answer); length=((uint)answer.size()-(uint)strCount.size())/3+1;
					DT(" length="<<length<<" strCount="<<answer<<" answer="<<answer<<" s="<<answer.size()<<"-"<<strCount.size()<<endl);
					//DT("answer="<<str<<END);							
				}else{ DT(" not_found"<<" length="<<length<<END);
					if(length==1){
						if(stringItemVector[c-1].oldName==""){ cout<<"remap"<<endl;
							stringItemVector[c-1].sillablyCount=1;
							stringItemVector[c-1].oldName=stringItemVector[c-1].name;
							str=str_replace("ྫ","ྗ",stringItemVector[c-1].name);
							if(str==stringItemVector[c-1].name){
								stringItemVector[c-1].name=str_replace("ར","ན",stringItemVector[c-1].name);
							}else{
								stringItemVector[c-1].name=str;
							}	
							stringItemVector[c-1].name=str_replace("ལ","འ",stringItemVector[c-1].name);
							//cout<<"c="<<c-1<<" stringItemVector["<<c-1<<"].oldName="<<stringItemVector[c-1].oldName<<endl;
							strResult[index]="";
							goto startSpellCheker;
						}else{
							stringItemVector[c-1].name=stringItemVector[c-1].oldName;
							str=stringItemVector[c-1].name+stringItemVector[c-1].delimeter;
							
						}
						/*
						 DT("stringFlag="<<stringFlag<<" srcLine.size()="<<srcLine.size()<<" srcLine="<<srcLine<<" oldString="<<oldString<<endl);
						 if(stringFlag<55&&srcLine.size()>11&&srcLine!=oldString){
						 testLine=implode("",stringItemVector);
						 str_="";
						 if(entry>0)str_+=stringItemVector[entry-1].name;
						 str_+=" :|:";
						 if(entry<stringItemVector.size()-1)str_+=stringItemVector[entry+1].name;
						 str_+=" :|:"+srcLine;
						 DT(" send str="<<str_<<"/ str="<<srcLine<<":|:"<<END);
						 str_=dataIP(str_,REMAP_STRING_TIB);  //send data by TCP/IP socket on main dictionary program
						 DT(" remap answer="<<str_<<":|:"<<END);
						 if(str_!=":|:"){
						 //str_=str_;//+delimeterTib[0];
						 str=str.substr(0,str.size()-3);
						 wordList[index]=str_replace(str,str_,wordList[index]);
						 DT(" wordList["<<index<<"]="<<wordList[index]<<"/ str="<<str<<" str_="<<str_<<END);	
						 
						 }
						 strResult[index]=""; stringFlag++;
						 DT(" remap str="<<wordList[index]<<END);
						 oldString=srcLine;
						 goto startSpellCheker;
						 }else{
						 }	
						 stringFlag++;
						 */	
						strResult[index]+="<span class=\"c\" style=\"color:#FF0000\">"+str+"</span>";
						
					}
				}
				
				
				if(recordFound){entry=k-1;
					//cout<<"record found"<<k<<" e="<<entry<<END;
					string color;
					char buff[10];
					if(length==1)color="<span class=\"c\" style=\"color:#009900\">";
					if(length==2)color="<span class=\"c\" style=\"color:#005500\">";
					if(length==3)color="<span class=\"c\" style=\"color:#002288\">";
					if(length==4)color="<span class=\"c\" style=\"color:#001166\">";
					if(length==5)color="<span class=\"c\" style=\"color:#000555\">";
					if(length==6)color="<span class=\"c\" style=\"color:#000044\">";
					if(length==7)color="<span class=\"c\" style=\"color:#000033\">";
					if(length==8)color="<span class=\"c\" style=\"color:#000022\">";
					if(length==9)color="<span class=\"c\" style=\"color:#000011\">";
					if(length>9)color="<span class=\"c\" style=\"color:#000000\">";
					strResult[index]+=color+str+"</span>";
					break;
				} 			
			}//end for (int k=StringItemList->Count
		}//end for (int entry=0;
	}
	//request="stop";
	//dataIP(request,CLOSE_SOCKET);
	
    answer=implode(" ",strResult); answer+="<br>\n"; 
    return answer;
}//_______________________________________________________________________________









