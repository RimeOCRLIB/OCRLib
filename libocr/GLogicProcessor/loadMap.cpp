#include "GLogicProcessor.h"

void GLogicProcessor::loadMapFilePali(const char *mapName){
//cout<<"Start inputData.data[\"inputFolder\"]"<<inputData.data["inputFolder"]<<endl;
string str;
xml_node resultSet;
xml_document doc;
	
	str="codePages/";
	str+=mapName;
	
	if(!doc.load_file(str.c_str())){
		cout<<str<<" not loaded"<<endl;
		return;
	}
	
	resultSet = doc.child("GFont");
	// Iterate through books
	//int index=0;
	
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){
		uniRecord letter;
		letter.letterUniUTF=row.attribute("n").value();
		letter.keyUTF=row.attribute("k").value();
		letter.letterUTFLowerCase=row.attribute("l").value();
		mettaPali.push_back(letter);
	}	
}//________________________________________________________________________________________________________________

