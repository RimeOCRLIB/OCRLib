#include "GLogicProcessor.h"

string GLogicProcessor::convertCXS_to_UTF(string &inStr){
          
	string str=inStr;
           
	for(int i=0;i<mettaPali.size();i++){
	
		str_replace(mettaPali[i].Wylie, mettaPali[i].letterUniUTF, str);
	
	}
	
	return str;
}

void GLogicProcessor::convertCXS_to_UTF_nocopy(string &inStr){
	
	for(int i=0;i<mettaPali.size();i++){
		
		str_replace(mettaPali[i].Wylie, mettaPali[i].letterUniUTF, inStr);
		
	}
}


void GLogicProcessor::lowerCase_nocopy(string &inStr){
	
	for(int i=0;i<mettaPali.size();i++){
		
		str_replace(mettaPali[i].letterUniUTF, mettaPali[i].letterUTFLowerCase, inStr);
		
	}
}
















