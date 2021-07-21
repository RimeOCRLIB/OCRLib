
#include "GLogicProcessor.h"


/** @brief check OCR hypotesis by font rules, grammar rules, dictionary, returns main string in HTML format */
void GLogicProcessor::grammarOCR(vector<OCRMatch>&pageText, string &mainString){

    int mode=inputData.OCRMode;
    if(mode==1){
        //проверка по словарю статистической устойчивости распознанного текста
        mainString="";
        for(int a=0;a<pageText.size();a++){
            if(!pageText[a].correlation)continue;
            mainString+=pageText[a].name;
            mainString+=pageText[a].delimeter;
        }
        //cout<<mainString;
        //mainString=str_replace("•"," ",mainString);
        //cout<<mainString<<endl;
    
    
    textNormalisation(mainString);
    //cout<<mainString;

        pageText.resize(0);
        OCRMatch match;
        match.correlation=100;
        pageText.push_back(match);
        
        //проверка по словарю статистической устойчивости распознанного текста
        //cout<<mainString<<endl;
        
        mainString=UnicodeToYagpo(mainString);
        wstring strW=UTF_to_Unicode(mainString);
        
        for(int n=0;n<strW.size();n++){
            string s=Unicode_to_UTF(strW[n]);
            if(isDelimeter(s)||s==" "){
                pageText[pageText.size()-1].delimeter+=s;
            }else{
                OCRMatch match;
                match.correlation=100;
                match.name=s;
                pageText.push_back(match);
            }
        }
        
        mainString="";
        textCorrector(pageText, mainString);
    }else{
        textNormalisation(mainString);
    }
    

}
