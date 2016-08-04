#include "GLogicProcessor.h"

void  GLogicProcessor::LoadUniToTibetanMapUTF(){
	string path=inputData.data["tablePath"]+"codePages/FontMap/ASCIToUni.map";
	path+="/ASCIToUni.map";
	//cout_<<"TEST"<<path<<endl;
	
	//char tmp[4094];
	///load ASCIToUni.map/////////////////////////
	
};//____________________________________________________________________________


void GLogicProcessor::LoadYagpoToWylieMap(){
	string path=inputData.data["tablePath"];
	path+="/codePages/TranslitTableUni_Wylie.xml";
	//cout_<<"TEST"<<path<<endl;
	
	
	//char tmp[4094];
	///load ASCIToUni.map/////////////////////////
	
	if( !is_file(path) ){cout_<<"no open font database "<<path<<endl;return;}
	
    // skip comment
    //streamsize maxLineSize =1024;

	vector<string> srcStrings;
	string tString;
	vector<string>lineStrings;
	vector<string>value;
	//unsigned int i=0,d;
	uniRecord record;
	
	readText(srcStrings, path);
	for(int i=0;i<srcStrings.size();i++){
		//cout_<<tString.c_str()<<endl;
		lineStrings=explode("|",srcStrings[i]);
		if(lineStrings.size()==4) {
			record.Wylie=lineStrings[1];
			record.translit=lineStrings[2];
			YagpoToWylieMap[lineStrings[0]]=record;
			UniToWylieMap[lineStrings[3]]=record;
			//cout_<<lineStrings[0]<<" "<<lineStrings[1]<<" "<<lineStrings[3]<<endl;
		}
		//i++;
	}
	
	//cout_<<"tibStr size1="<<tibStr.size()<<endl;
	//cout<<"YagpoToWylieMap.size()="<<YagpoToWylieMap.size();
	
	
	return;
};//____________________________________________________________________________


void  GLogicProcessor::LoadFontNameMap(){
	string path=inputData.data["tablePath"];
	path+="/codePages/FontMap/TibetanFontList.map";
	
	if(!is_file(path)){cout_<<"path "<<path<<" not open"<<END;return;}
	
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

void GLogicProcessor::LoadMapXML(){
	
	xml_node Cell,Data, letterSet,resultSet,table, metadata, field;
	xml_document doc;
	string str,path;
	int i;
    vector<string> nameArray; nameArray.resize(100);
	
	LoadFontNameMap();
	//cout_<<"tibFontName.size()="<<fontNameMap.size()<<END;
	
	
	path=inputData.data["uniTablePath"];
	
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;return;
	}
	
	letterSet = doc.child("FMPXMLRESULT");
    metadata = letterSet.child("METADATA");
	i=0; //from first we get map key name from field name 
	for (field = metadata.child("FIELD"); field; field = field.next_sibling("FIELD")){
		nameArray[i]=field.attribute("NAME").value(); //cout_<<"nameArray["<<i<<"]="<<field.attribute("NAME").value()<<END;
		i++;  			
	}//nameArray now contain database field name
	
	resultSet=letterSet.child("RESULTSET");
	for (xml_node row = resultSet.child("ROW"); row; row = row.next_sibling("ROW")){
		map<string,string>record; ///base record for all letter conversion function
		i=0;
		for (Cell = row.child("COL"); Cell; Cell = Cell.next_sibling("COL")){
			Data=Cell.child("DATA");
			record[nameArray[i]]=Data.child_value();  //cout_<<"record["<<nameArray[i]<<"]="<<record[nameArray[i]]<<END;
			i++;  			
		}//nameArray now contain database field name
		
		if(record["Wylie"]=="space"){ //exeption for XML parser
			record["name"]=" ";
			record["uniHexKey"]=" ";
			record["tibUniUTF"]=" ";
		}

            mainLetterTableYagpo[record["name"]]=record;
			mainLetterTableUni[record["tibUniUTF"]]=record;
			mainLetterTableKey[record["uniKey"]]=record;
            mainLetterTableOCRKey[record["OCRKey"]]=record;
            ASCIToUni[record["Wylie"]]=record["tibUniUTF"];
        
       //cout<<"k="<<record["name"]<<" v="<<record["OCRKey"]<<" u="<<record["tibUniUTF"]<<" size="<<mainLetterTableUni.size()<<END;
	}
    
	ASCIToUni[" "]="་";  //exeption;
    ASCIToUni["_"]=" ";  //exeption;
	//cout_<<"mainLetterTableUni.size()="<<mainLetterTableUni.size()<<END;
	
	
};//____________________________________________________________________________


void GLogicProcessor::LoadFontMap(map<string,uniRecord>&fMap,string &fileName){
	
	ifstream TibetanFontMap(fileName.c_str());
	if( !TibetanFontMap ){
		cout<<fileName<<" not found"<<END;  return;
	}
	
	vector<string> srcStrings;
	string tString;
	vector<string>lineStrings;
	//short i=0;
	uniRecord record;
	vector<string> value;
	vector<string>::size_type d;
	wstring wstr;
	unsigned short data;
	        //cout_<<"load map "<<fileName<<END; 
	maxUniRecord=0;
	
	while (getline(TibetanFontMap, tString,'\n')){
		srcStrings.push_back(tString);    //cout<<"vector count="<<srcStrings.size()<<endl;
		lineStrings=explode(":|:",tString); //for(int i=0;i<lineStrings.size();i++)cout<<lineStrings[i]<<" ";
		wstr=L"";
		
        wstr+=UTF_to_Unicode(lineStrings[0]);
		//cout<<"lineStrings="<<tString<<" wstr.size()="<<wstr.size()<<" wstr="<<Unicode_to_UTF(wstr)<<END;
		record.OCRKey=lineStrings[0];
		record.Wylie=lineStrings[1]; 
		if(lineStrings.size()>2){
			//cout_<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[2]<<endl;
			record.keyUTF=lineStrings[2];
			if(record.keyUTF.size()/3>maxUniRecord)maxUniRecord=(int)record.keyUTF.size()/3;
			//cout_<<" wstr="<<Unicode_to_UTF(wstr)<<" value.size()="<<value.size()<<END;
 		    //cout_<<" str="<<record.keyUTF<<" hex="<<lineStrings[0]<<"key="<<record.OCRKey<<"//"<<END;
		}
		if(lineStrings.size()>3){
			//cout_<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[3]<<"/"<<endl;
				record.letterUTF=lineStrings[3];
				//cout_<<" wstr="<<Unicode_to_UTF(wstr)<<END;
		}		
		
		fMap[record.OCRKey]=record;
		//cout<<"fMap["<<record.OCRKey<<"]="<<fMap[record.OCRKey].keyUTF<<"//"<<END; exit(0);
	}
	//cout_<<"fontMap[str][letter].OCRKey"<<fontMap["Dederis-a"][" "].keyUTF<<"//"<<END;
	//cout_<< "fMap.size()="<<fMap.size()<<" maxUniRecord="<<maxUniRecord<<endl;
	return;
};//____________________________________________________________________________

