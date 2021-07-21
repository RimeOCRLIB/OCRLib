#include "GLogicProcessor.h"

void GLogicProcessor::HTML2UniConverter( string &mainString, string &path){

    int print=0;
    string textCSV;
    readText(textCSV, path);
    
    vector<string>v;
    explode("\n", textCSV, v);
    string result="";
    uint size=(uint)v.size();
    string line;
    LoadFontNameMap();
    string uniStack="";
    int YagpoWylieFontFlag=0;
    readDictionary();
    
    
    //последовательно читаем все строки из текстового CSV файла
    for(int i=0;i<size;i++){
        line=v[i];
        //print_(line<<endl);
        if(line=="@@@ new line @@@"){
            //конвертируем набранный стек букв Юникода в последовательность Unicode Open Type
            //cout<<"uniStack:"<<uniStack<<endl;
            if(uniStack.size()!=0){
                //DR_("YagpoWylieFontFlag"<<YagpoWylieFontFlag<<" uniStack="<<uniStack<<END)
                if(YagpoWylieFontFlag){
                    uniStack=WylieToUniConverter(uniStack);
                }
                //DP("uniStackOut="<<uniStack<<END);x
                result+=uniStack;
                uniStack="";
            }
        result += "\n";
            continue;
        }
        if(line.find("@@@Page")!=-1){
            //конвертируем набранный стек букв Юникода в последовательность Unicode Open Type
            //cout<<"uniStack:"<<uniStack<<endl;
            if(uniStack.size()!=0){
                //DR_("YagpoWylieFontFlag"<<YagpoWylieFontFlag<<" uniStack="<<uniStack<<END)
                if(YagpoWylieFontFlag){
                    uniStack=WylieToUniConverter(uniStack);
                }
                //DP("uniStackOut="<<uniStack<<END);x
                result+=uniStack;
                uniStack="";
            }

            result += line + "\n";
            continue;
        }
        vector<string>v1;
        explode(":|:",line,v1);
        if(v1.size()<2)continue;
        string textLine="";
        string fontName=v1[0];
        string fontNameBase=fontName;
        string chink=v1[1];
        wstring chinkW=UTF_to_Unicode(chink);
        
        //заменяем шрифты с одинаковой кодовой страницей
        if(fontName.find("TibetanChogyal")!=-1){
            fontName=str_replace("TibetanChogyal", "TibetanCalligraphic", fontName);
            //fontName=str_replace("TibetanChogyal", "TibetanClassic", fontName);
        }
        
        
        int letterIndex=0;
        uint chinkSize=(uint)chinkW.size();
        string letter;
        
        while(letterIndex<chinkSize){
            string letter=Unicode_to_UTF(chinkW[letterIndex]);
            //DR_(" letterIndex="<<letterIndex<<" letter=/"<<letter<<"/ fontName="<<fontName<<" hex code="<<hex<<(unsigned short)chinkW[letterIndex]<<endl);
            //DR("fontName="<<str<<END)
            if ( fontNameMap.find(fontName) != fontNameMap.end() ) {
                 //DR_(fontName <<" is tib font"<<END)
                 //________________START_convert Tibetan font_______________
                 //first check did we load the map of this font
                 if ( fontMap.find(fontName) != fontMap.end()) {
                     //DR_(fontName<<" Map exist"<<END)
                     //flag=1;
                 }else{
                     //DR_(fontName<<" Map not exist. Load map "<<fontName<<END);
                     string pathMap=inputData.data["tablePath"]+"/codePages/FontMap/"+fontName+".map";
                     LoadFontMap(fontMap[fontName], pathMap);
                 }
                 //cout<<"fontMap["<<fontName<<"]:"<<fontMap[fontName].size()<<endl;
                 YagpoWylieFontFlag=0;
                 
                 //cout <<" font name="<<str<<" letter"<<letter<<endl;
                 
                 if(fontName=="Yagpo!_Wylie"||fontName=="Yagpo!_Wylie_Callygraphy"){
                     YagpoWylieFontFlag=1;
                     //if(letter=="\n"||letter=="\r"){
                     //if(uniStack.size()>0){
                     //	uniStack=UniToTibetan(uniStack, 1);
                     //	NSString *txt=[NSString stringWithCString:(const char*)uniStack.c_str() encoding:4];
                     //	NSFont *font = [NSFont fontWithName:@"Yagpo" size:14.0];
                     //	NSDictionary *attrsDictionary =	[NSDictionary dictionaryWithObject:font	forKey:NSFontAttributeName];
                     //	NSAttributedString *resultString =[[NSAttributedString alloc] initWithString:txt	attributes:attrsDictionary];
                     //	[mainString appendAttributedString:resultString];
                     //	[resultString release];
                     //cout<<"ready1"<<endl;
                     //	uniStack="";
                     //	[mainString appendAttributedString:testLetter];
                     //	[[mainString mutableString] appendString:@"\n"];
                     //cout<<" letterIndex="<<letterIndex<<" lineIndex="<<lineIndex<<endl; lineIndex++;
                     //}
                     //}else{
                     //if (ASCIToUni.find(letter) != ASCIToUni.end()) {
                     //	uniStack+=ASCIToUni[letter];
                     //}
                     //}
                     //cout<<"letter="<<letter;
                     if (fontMap[fontName].find(letter) != fontMap[fontName].end()) {
                         uniStack+=letter;
                     }else{
                         letter="[{"+letter+"="+fontName+"}]"; //report+=letter;
                         uniStack+=letter;
                     }
                     //cout<<"l="<<letter<<endl;
                 
                 }else{
                     YagpoWylieFontFlag=0;
                     //DR_("letter="<<letter<<"fontMap["<<fontName<<"]["<<letter<<"].OCRKey="<<fontMap[fontName][letter].OCRKey<<"/ "<<END)
                 
                     if (fontMap[fontName].find(letter) != fontMap[fontName].end()&&fontMap[fontName][letter].keyUTF.length()>0) {
                        letter=fontMap[fontName][letter].keyUTF;
                        result+=letter;
                     }else{
                         //uniStack=UniToTibetan(uniStack,YagpoWylieFontFlag);
                         result+=uniStack;
                         uniStack=" ";
                         ostringstream st;
                         wstring w;
                         w=UTF_to_Unicode(letter);
                         st<<"[{"<<letter<<"("<<hex<<(short)w[0]<<")"<<"="<<fontNameBase<<"}]";
                         letter=st.str();
                         result+=letter;
                     }
                 }
                 
                 //DR_("l="<<letter<<END)
                 
            }else{
                //DT(str<<" is Western font letter="<<letter<<END);
                if(uniStack.size()>0){
                    uniStack=UniToTibetan(uniStack,YagpoWylieFontFlag);
                    result+=uniStack;
                    uniStack=" ";
                }
                result+=letter;
            }
            letterIndex++;
            
        }//end while
    }
    if(uniStack.size()>0){
        //uniStack=UniToTibetan(uniStack,YagpoWylieFontFlag);
        result+=uniStack;
        uniStack=" ";
    }
    //print_(result<<endl);
    mainString=result;
    
    
}
