#include "GLogicProcessor.h"

string GLogicProcessor::convertCXS_to_UTF(string &inStr){
          
	string str=inStr;
           
	for(int i=0;i<mettaPali.size();i++){
	
		str_replace(mettaPali[i].keyUTF, mettaPali[i].letterUTF, str);
	
	}
	
	return str;
}

void GLogicProcessor::convertCXS_to_UTF_nocopy(string &inStr){
	
	for(int i=0;i<mettaPali.size();i++){
		
		str_replace(mettaPali[i].keyUTF, mettaPali[i].letterUTF, inStr);
		
	}
}


void GLogicProcessor::lowerCase_nocopy(string &inStr){
	
	for(int i=0;i<mettaPali.size();i++){
		
		str_replace(mettaPali[i].letterUTF, mettaPali[i].letterUTFLowerCase, inStr);
		
	}
}
















