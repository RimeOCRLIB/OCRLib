#include "GLogicProcessor.h"
#include <regex>


string GLogicProcessor::WylieToUniConverter(string &inStr){
    
    string match,destStr,result;
    if(!ASCIToUni.size()){
        cout<<"need load font map"<<endl;
        return result;
    }
	//cout<<"in="<<line<<endl;
    locale::global(std::locale("en_US.UTF-8"));  //important
    
    //заменяем все одиночные гласные на q+гласная
    
    regex reg1("^([aoeiuOEAU])");
    inStr = regex_replace(inStr, reg1, "q$1");
    regex reg2("([^qwrtypsdghjkl'zxcvbnmNDT])([oeiuOEIU])");
    inStr = regex_replace(inStr, reg2, "$1q$2");
    
    //заменяем все гласные «а» в слогах на маскирующий символ •
    //маскирующий символ стираем в @fn tibetanUTFToYagpo
    //маскирующий символ необходим потому, что в кодировке Wylie слогам которые не имеют явной гласной
    //присваивается гласная а. Эта гласная в составных буквах с коренными согласными не обозначается в тибетском начертании
    //@fn tibetanUTFToYagpo при конвертации слога учитывает маскирующий символ как границу составной буквы.

    inStr=str_replace("a","•",inStr);
    
	for(int i=0;i<inStr.size();i++){
		match=inStr[i];
		if(ASCIToUni[match].size()){
		   destStr+=ASCIToUni[match];
		}else{
		   destStr+=inStr[i];
		}	
		//cout<<(short)inStr[i]<<" "<<inStr[i]<<" "<<destStr<<endl;
	}
    
    destStr=str_replace("+","྄",destStr);

    destStr=tibetanUTFToYagpo(destStr,2);
    
    destStr=str_replace("ག.","ག",destStr);
    destStr=str_replace("q","ཨ",destStr);
    
    return destStr;
}//_____________________________________________________________________


string GLogicProcessor::dWylieToYagpoConverter(string &inStr){
	string destStr;
	string match;
	string needConvertToWylie="";
	int print=0;
	string result;
	string startUTF="<";
	string endUTF=">";
	
	DR("DEST"<<inStr<<endl);
	
	short startTibetan, endTibetan;
    //inStr=str_replace("\r","",inStr);
    startTibetan=0, endTibetan=0;
    destStr="";		
    for(int i=0;i<inStr.size();i++){
        match=inStr[i]; 
        if(match==endUTF){
            startTibetan=0;
            endTibetan=1;  DR("destStr="<<destStr<<endl);
            result+=startUTF+tibetanUTFToYagpo(destStr,2)+endUTF;
            DR("end tibetan result"<<result<<endl);
            destStr="";
            
        }
        
        if(startTibetan==1){
            DR("startT1 i="<<i<<" match1="<<endl);
            destStr+=ASCIToUni[match];
            DR(" start/"<<destStr<<endl);
        }
        
        if(match==startUTF&&endTibetan==0){startTibetan=1;endTibetan=0;
            result+=destStr;
            DR("start i="<<i<<" result="<<destStr.size()<<endl);
            destStr="";
        }
        if(startTibetan==0){
            if(endTibetan!=1){
                result+=match;
            }else{endTibetan=0;}
        }
        DR("i="<<i<<" /"<<result<<endl);
        
        
    }
    DR("//__"<<destStr<<"="<<destStr.size()<<endl);
    result=YagpoToUni(result);
    //cout<<" result="<<result<<endl;
    //if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
    //result="";
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
}//_______________________________________________________________________________________



string GLogicProcessor::UnicodeToYagpo(string &srcStr){
	string destStr;
	string needConvertToWylie="";
	//string::size_type pos = 0;
	string result;
	int print=0;
	//cout<<"DEST"<<inStr<<endl;
    if(!mainLetterTableUni.size()){
        cout<<"need load font table"<<endl;
        return result;
    }
    
	short startTibetan, endTibetan;
	//inStr=str_replace("\r","",inStr);
	//c_out<<"res="<<Unicode_to_UTF(srcLineUni)<<endl;
	startTibetan=0, endTibetan=0;
	destStr="";		
	for(int i=0;i<srcStr.size();i++){
		DR("srcStr["<<i<<"]="<<(short)srcStr[i]<<"/"<<srcStr[i]<<endl);
		
		if((short)srcStr[i]>0&&startTibetan==1){
			startTibetan=0;
			endTibetan=1;  DR("destStr="<<destStr<<endl);
			result+=tibetanUTFToYagpo(destStr,1);
			result+=srcStr[i];
			DR("end tibetan result"<<result<<endl);
			destStr="";
			
		}
		if((short)srcStr[i]<0){endTibetan=0;
			//if((short)srcStr[i]==-96||(short)srcStr[i]==-62)continue;  //tabulation
			DR("start i="<<i<<" result="<<result<<endl);
			if(startTibetan==0){
				result+=destStr;
				destStr=srcStr[i];
				startTibetan=1;
			}else{
			    destStr+=srcStr[i];
			}	
			DR(" @tib/"<<destStr<<endl);
		}
		
		if(startTibetan==0){
			if(endTibetan!=1){
				destStr+=srcStr[i];
			}else{endTibetan=0;}
		}
        DR("endTibetan="<<endTibetan<<" startTibetan="<<startTibetan<<endl);
		DR("i="<<i<<"result=/"<<result<<"/ destStr=/"<<destStr<<"/"<<endl);
		
		
	}
	//cout<<"//__"<<destStr.size()<<endl;
	if(startTibetan){
		result+=tibetanUTFToYagpo(destStr,1);
	}else{
	    result+=destStr;
	}	
	DR(" result="<<result<<endl);
	//if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
	//result="";
	
	
    srcStr=result;
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
};


string GLogicProcessor::SinghalaASCIToYagpo(string &uniStack){
	///cout<<"uniStack="<<uniStack.size()<<endl; 
	string str=uniStack,strSrc,strDest;
	map<string, string>::iterator p; 
	if(SinhalaASCI.size()){
		for(p = SinhalaASCI.begin(); p != SinhalaASCI.end(); p++) {
			strSrc=p->first;
			strDest= p->second;  
			str=str_replace(strSrc,strDest, str);
            //cout<<"strSrc="<<strSrc<<" strDest="<<strDest<<" str="<<str<<endl;
		}
	}	
   	///cout<<"str="<<str<<endl;  
	return SinhalaUniToYagpo(str,1);;
}//_______________________________________________________________________________________



string GLogicProcessor::dSinhalaASCIToYagpo(string &inStr){
	string destStr,uniStack;
	string match,matchStr;
	string needConvertToWylie="";
	//string::size_type pos = 0;
	string result;
	string startUTF="༼";
	string endUTF="༽"; 
	
	short startTibetan, endTibetan;
	
	//inStr=str_replace("\r","",inStr);
	//cout<<"inStr="<<inStr<<endl;
	startTibetan=0, endTibetan=0;
	
	for(int i=0;i<inStr.size();i++){
		match=inStr[i];
		matchStr+=match;
		//cout<<matchStr<<endl;
		if(matchStr.find(endUTF)!= std::string::npos){
			startTibetan=0;
			endTibetan=1;
			matchStr=""; uniStack+=match;
			result+=SinhalaUniToYagpo(uniStack,1);
			//cout<<"end tibetan result"<<result<<endl;
		}
		
		if(startTibetan==1){
			//cout<<"startT1 i="<<i<<" match1="<<endl;
			uniStack+=match;
			//cout<<" /"<<Unicode_to_UTF(destStr)<<endl;
		}
		if(matchStr.find(startUTF)!= std::string::npos&&endTibetan==0){startTibetan=1;endTibetan=0;
			result+=match;
			//cout<<"result.size()="<<result.size()<<" result="<<result<<endl;
			matchStr=""; 
		}
		if(startTibetan==0){
			if(endTibetan!=1){
				result+=match;
			}else{endTibetan=0;}
		}
		//cout<<"i="<<i<<" /"<<result<<endl;
		
		
	}
	//cout<<"//__"<<destStr.size()<<endl;
	//result+=destStr;
	//c_out<<"result="<<result<<" // destStr="<<Unicode_to_UTF(destStr)<<endl;
	//destStr="";
	//c_out<<"destStr="<<Unicode_to_UTF(destStr)<<endl;
	//result+=tibetanUTFToYagpo(destStr, uniTibTableYagpo);
	
	//c_out<<"DEST____"<<result<<endl;
	//cout<<result<<endl;
	//if(srcVect.size()>150)c_out_html<<result<<"<br>"<<endl;
	//result="";
	//cout<<"DEST"<<srcVect[j]<<endl;
	
	
	//if(needConvertToWylie!="")c_out<<"need Convert To Wylie \n"<<needConvertToWylie;
	return result;	
	
}//_______________________________________________________________________________________


string GLogicProcessor::UniToWylieConverter(string &srcStr){
    string result;
    
    
    //srcStr="པ།།";
    //маскируем знак перевода каретки
    srcStr=str_replace("\n"," \n ",srcStr);
    
    
    srcStr=UnicodeToYagpo(srcStr);
    //cout<<"srcStr:"<<srcStr<<endl;
    wstring strW=UTF_to_Unicode(srcStr);
    size_t size=strW.size();
    string str,str_;
    
    for(int i=0;i<size;i++){
        str=Unicode_to_UTF(strW[i]);
        if(mainLetterTableYagpo.find(str)!=mainLetterTableYagpo.end()){
            str=mainLetterTableYagpo[str].letterUniUTF;
        }
        if(mainLetterTableUni.find(str)!=mainLetterTableUni.end()){
            str=mainLetterTableUni[str].Wylie;
        }
        result+=str;
    
    }
    
    locale::global(std::locale("en_US.UTF-8"));  //important
    
    //маскируем пробел
    result=str_replace("_", " _ ",result);
    
    vector<string>v;

    regex reg0("([\\d]+)");
    regex reg1("^([/\\|!:\\(\\[]*)([gdbm']a)*([wrtypsdfghjklzcvbnmŋ]+)([aeiou])([gŋdnbm'rls]a){0,1}([sd]a){0,1}([:\\|\\)\\]/=;]*)$");
    //regex reg1("^([^aeiou\\+]+)([aeiou])([gngdnbm'rls]a){0,1}([sd]a){0,1}$");
    regex reg2("^(.*[aoeiu].*)a$");
    regex reg3("^(.*)aØ(.*[aoeiu].*)$");
    regex reg4("^([wrtypsdfghjklzcvbnmŋ]+)([:\\|\\)\\]/=;]*)$");
    
    //отделяем цифры от текста
    result=regex_replace(result, reg0, "$1 ");
    //маскируем суффикс nga
    result=str_replace("ng", "ŋ",result);
    
    
    explode(" ", result,v);
    result="";
    size=v.size();
    for(int i=0;i<size;i++){
        str_=v[i];
        //проверяем является ли слог тибетским
        str = regex_replace(str_, reg1, "");
        if(str==""){
            //str = regex_replace(str_, reg1, "$1$2#2/$3/#3/$4%4/$5#5/$6#6/$7");  //debug
            str = regex_replace(str_, reg1, "$1$2Ø$3$4$5•$6•$7");   //work
 
            //стираем гласную а в суффиксах
            str=str_replace("a•", "",str);
            str=str_replace("•", "",str);
            //если в слоге есть гласная у коренной буквы, стираем гласную «a» в приставке
            str=regex_replace(str, reg3, "$1$2");
            str=str_replace("Ø", "",str);
            //добавляем гласную к одиночным буквам
            str=regex_replace(str, reg4, "$1a$2");
            result+=str+" ";
        }else{
            result+=v[i]+" ";
        }

    }
    //убираем маскирование пробела
    result=str_replace(" _ ", "_",result);
    //убираем маскирование новой строки
    result=str_replace("_\n_", "<br/>\n",result);
    //убираем маскирование суффикса nga
    result=str_replace("ŋ", "ng",result);
    //исключение буквы H в санскрите
    result=str_replace("aH", "H",result);
    
    //cout<<result<<endl;
    
    srcStr=result;
    return result;
}//_______________________________________________________________________________________


void GLogicProcessor::BonPDFToUni(string &srcStr){
    
    //cout<<" BonPDFToUni "<<srcStr<<endl;
    
    static vector<string>key;
    static vector<string>value;
    if(!key.size()){   cout<<"load dict"<<endl;
        string path=inputData.data["tablePath"]+"bon_code_page.txt";
        //cout<<"path="<<path<<endl;
        vector<string>dictStr; readText(dictStr,path);
        //cout<<"dictStr.size()="<<dictStr.size()<<endl;
        for(int i=0;i<dictStr.size();i++){
            vector<string> line;
            explode(":|:",dictStr[i], line);
            //cout<<"dictStr["<<i<<"]="<<dictStr[i]<<" size="<<line.size()<<endl;
            if(line.size()>1){
                key.push_back(line[0]);
                value.push_back(line[1]);
            }
        }
    }
    
    for(int i=0;i<key.size();i++){
        srcStr=str_replace(key[i], value[i], srcStr);
        //cout<<" key="<<key[i]<<" value="<<value[i]<<" srcStr="<<srcStr<<endl;
    }
    //cout<<" srcStr="<<srcStr<<endl;


}

