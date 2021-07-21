#include "GLogicProcessor.h"

void  GLogicProcessor::LoadUniToTibetanMapUTF(){
	string path=inputData.data["tablePath"]+"codePages/FontMap/ASCIToUni.map";
	path+="/ASCIToUni.map";
	//cout_<<"TEST"<<path<<endl;
	
	//char tmp[4094];
	///load ASCIToUni.map/////////////////////////
	
};//____________________________________________________________________________


void GLogicProcessor::LoadUniToWylieMap(){
	string path=inputData.data["tablePath"];
	path+="codePages/TranslitTableUni_Wylie.txt";
	//cout_<<"TEST"<<path<<endl;
	
	
	//char tmp[4094];
	///load ASCIToUni.map/////////////////////////
	
	if( !is_file(path) ){cout_<<"no transliteration file"<<path<<endl;return;}
	
    // skip comment
    //streamsize maxLineSize =1024;

	vector<string> srcStrings;
	string tString;
	vector<string>value;
	//unsigned int i=0,d;
	uniRecord record;
	
	readText(srcStrings, path);
	for(int i=0;i<srcStrings.size();i++){
		//cout_<<tString.c_str()<<endl;
        vector<string>lineStrings;
		explode("|",srcStrings[i],lineStrings);
		if(lineStrings.size()>3) {
			record.Wylie=lineStrings[1];
			record.translitRus=lineStrings[2];
            record.translitEng=lineStrings[3];
			UniToWylieMap[lineStrings[0]]=record;
			//cout<<lineStrings[0]<<" "<<lineStrings[1]<<" "<<lineStrings[3]<<endl;
		}
		//i++;
	}
	
	//cout_<<"tibStr size1="<<tibStr.size()<<endl;
	//cout<<"UniToWylieMap.size()="<<UniToWylieMap.size();
	
	
	return;
};//____________________________________________________________________________


void  GLogicProcessor::LoadFontNameMap(){
	string path=inputData.data["tablePath"];
	path+="/codePages/FontMap/TibetanFontList.map";
	
	if(!is_file(path)){cout_<<"path "<<path<<" not open"<<endl;return;}
	
	vector<string> srcStrings;
	string tString;
	vector<string>lineStrings;
	vector<string>value;
	
	readText(lineStrings,path);
	
	
	for(int i=0;i<lineStrings.size();i++){
		fontNameMap[lineStrings[i]]=1;
	}

	return;
};//____________________________________________________________________________



void GLogicProcessor::LoadFontMap(map<string,uniRecord>&fMap,string &fileName){
	
	ifstream TibetanFontMap(fileName.c_str());
	if( !TibetanFontMap ){
		cout<<fileName<<" not found"<<endl;  return;
	}
	
	vector<string> srcStrings;
	string tString;
	//short i=0;
	uniRecord record;
	vector<string> value;
    //vector<string>::size_type d;
	wstring wstr;
    //unsigned short data;
	        //cout_<<"load map "<<fileName<<endl; 
	maxUniRecord=0;
	
	while (getline(TibetanFontMap, tString,'\n')){
        if(tString=="")continue;
		srcStrings.push_back(tString);    //cout<<"vector count="<<srcStrings.size()<<endl;
        vector<string>lineStrings;
		explode(":|:",tString,lineStrings);
        //for(int i=0;i<lineStrings.size();i++)cout<<i<<" "<<lineStrings[i]<<endl;
    
		wstr=L"";
        if(lineStrings.size()<2){cout<<"Wrong line "<<tString<<" in file"<<fileName<<endl;continue;}
        
        wstr+=UTF_to_Unicode(lineStrings[0]);
		//cout<<"lineStrings="<<tString<<" wstr.size()="<<wstr.size()<<" wstr="<<Unicode_to_UTF(wstr)<<endl;
		record.OCRKey=lineStrings[0];
		record.Wylie=lineStrings[1]; 
		if(lineStrings.size()>2){
			//cout_<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[2]<<endl;
			record.keyUTF=lineStrings[2];
			if(record.keyUTF.size()/3>maxUniRecord)maxUniRecord=(int)record.keyUTF.size()/3;
			//cout_<<" wstr="<<Unicode_to_UTF(wstr)<<" value.size()="<<value.size()<<endl;
 		    //cout_<<" str="<<record.keyUTF<<" hex="<<lineStrings[0]<<"key="<<record.OCRKey<<"//"<<endl;
		}
		if(lineStrings.size()>3){
			//cout_<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[3]<<"/"<<endl;
				record.letterUniUTF=lineStrings[3];
				//cout_<<" wstr="<<Unicode_to_UTF(wstr)<<endl;
		}		
		
		fMap[record.OCRKey]=record;
		//cout<<"fMap["<<record.OCRKey<<"]="<<fMap[record.OCRKey].keyUTF<<"//"<<endl; exit(0);
	}
	//cout_<<"fontMap[str][letter].OCRKey"<<fontMap["Dederis-a"][" "].keyUTF<<"//"<<endl;
	//cout_<< "fMap.size()="<<fMap.size()<<" maxUniRecord="<<maxUniRecord<<endl;
	return;
};//____________________________________________________________________________

