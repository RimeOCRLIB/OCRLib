#include "php2stl.h"
#include <unistd.h>
#include "GVector.h"
#include "GMap.h"



void readPreferences(){

//inputData.argc=READ_FROM_XML;
inputData.argc=READ_FROM_DB;
inputData.data["HOSTNAME"]=MAIN_HOST;
inputData.data["DATA_DB"]=inputData.data["root"]+"OCRData/DATA_DB/";
inputData.data["OCRTables"]=inputData.data["root"]+"OCRData/OCRTables/";
inputData.data["uniTablePath"]=inputData.data["root"]+"OCRData/DATA_DB/LETTERS_GRAMMAR/LETTERS_GRAMMAR.xml";
inputData.data["wordsDataPath"]=inputData.data["root"]+"OCRData/Grammar/dictData.xml";
inputData.data["tablePath"]=inputData.data["root"]+"OCRData/CodeTables/";
inputData.data["inputPath"]=inputData.data["root"]+"input.xml";
inputData.data["statPath"]=inputData.data["root"]+"OCRData/OCRStat1.xml";  //синхронизация форков
inputData.data["prefPath"]=inputData.data["root"]+"OCRData/preferences.bin";  //синхронизация форков
inputData.data["OCRBasePath"]=inputData.data["root"]+"OCRData/OCRTables/OCRLetterBase.xml";
inputData.data["dictPath"]=inputData.data["root"]+"OCRData/_XML_Dict/";
inputData.data["grammar"]=inputData.data["root"]+"OCRData/Grammar/";
inputData.data["rootAIML"]=inputData.data["root"]+"edit/AIML_CORE/";
    
inputData.data["dictionary"]="1";
//_________________site_enviroment____________________________________________
inputData.data["siteRoot"]="/BUDDISM.RU/";
inputData.data["siteData"]="/BUDDISM.RU/MaketZen/Site Folder/";
inputData.data["HTTPType"]="text/xml";
cout_<<"@get message="<<inputData.data["QUERY_STRING"]<<inputData.data["POST"]<<"//"<<endl;

//create preferences data
inputData.startIndex=0;
GVector *prefVector=GVector::create(inputData.data["prefPath"]);
prefVector->resize(100);
GStr<int>*pref=GStr<int>::create(prefVector, "PREF");
pref->resize(100);

inputData.prefVector=(void*)prefVector;
inputData.pref=(void*)pref;
inputData.num_cores=0;

}

int strLen(string &input){return (int)input.size();}
int strLenW(wstring &input){return (int)input.size();}

unsigned int strToHash(const char *str, unsigned int len){
	unsigned int hash = 0;
	unsigned int i = 0;
	for (i = 0; i < len; str++, i++) {
		hash = (hash * 1664525) + (unsigned char)*str + 1013904223;
	}
	return hash;
}

unsigned int RSHash (const char *str, unsigned int len)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;
	unsigned int i = 0;
	
	for (i = 0; i < len; str++, i++) {
		hash = hash * a + (unsigned char)(*str);
		a = a * b;
	}
	return hash;
}

// заменить >= на > с уменьшенмем на единицу проверяемого числа
wstring UTF_to_Unicode(string &input) {
        if(input.size()==0)return L"";
//cout<<"input="<<input<<endl;
	unsigned char ints;
	int bits=0;
	string val;
	wstring value;
	//val   = array();
	unsigned char z,y,x,w,v,u,c,b;
	int i;
	int len=strLen(input);
	int bit1,bit2,bit3,bit4,bit5,bit6;


	for(i=0; i<len; i++){
		c=input[i]; //cout<<"c="<<(short)c<<endl;
		if(c > 128){
			if((c >= 254||c < 192)){value=(wchar_t*)input.c_str(); return value;}
			if(c >= 192) bits=2;
			if(c >= 224) bits=3;
			if(c >= 240) bits=4;
			if(c >= 248) bits=5;
			if(c >= 252) bits=6;
			if((i+bits) > len) {value=(wchar_t*)input.c_str(); return value;}
			//cout<<"i="<<i<<" bits="<<bits<<" len="<<len<<endl;

			while(bits > 1){
				i++;
				b=input[i];
				if(b < 128 || b > 191){value=(wchar_t*)input.c_str(); return value;}
				bits--;
			}
		}
	}

	//cout<<"input1="<<input<<endl;


 	//if(!is_utf8(input)return input;

	//bit1  = pow(64, 0);   //php code
	//bit2  = pow(64, 1);
	//bit3  = pow(64, 2);
	//bit4  = pow(64, 3);
	//bit5  = pow(64, 4);
	//bit6  = pow(64, 5);

	bit1 = 1;
	bit2 = 64;
	bit3 = 4096;
	bit4 = 262144;
	bit5 = 16777216;
	bit6 = 1073741824;
    //char buff[33];

	for(i=0; i< strLen( input ); i++){

		ints = (unsigned char)input[i];

		z     = (unsigned char) input[i];
		y     = (unsigned char) input[i+1] - 128;
		x     = (unsigned char) input[i+2] - 128;
		w     = (unsigned char) input[i+3] - 128;
		v     = (unsigned char) input[i+4] - 128;
		u     = (unsigned char) input[i+5]  - 128;

		if( ints <= 127 ){
			// 1 bit
			//sprintf(buff,"%d",(z * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)(z*bit1);
			//val[]  = value;
		}

		if( ints >= 192 && ints <= 223 ){
			// 2 bit
			//sprintf(buff,"%d",((z-192) * bit2 + y * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
		    value += (wchar_t)((z-192) * bit2 + y * bit1);
			//val[]  = value;
		}

		if( ints >= 224 && ints <= 239 ){
			// 3 bit
			//sprintf(buff,"%d",((z-224) * bit3 + y * bit2 + x * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value += (wchar_t)((z-224) * bit3 + y * bit2 + x * bit1);
			//val[]  = value;
		}
		if( ints >= 240 && ints <= 247 ){
			// 4 bit
			//sprintf(buff,"%d",((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1);
			//val[]  = value;
		}
		if( ints >= 248 && ints <= 251 ){
			// 5 bit

			//sprintf(buff,"%d",((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1);
			//val[]  = value;
		}
		if( ints == 252 && ints == 253 ){
			// 6 bit
			//sprintf(buff,"%d",((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1);
			//val[]  = value;
		}
		if( ints == 254 || ints == 255 ){
			cout<<"Wrong Result!"<<endl;
		}


	}

	//cout << "val="<<val<<endl;

	//if( array === False ){
	return value;
	//}
	//if(array === True ){
	//	val     = str_replace('&#', '', value);
	//	val     = explode(';', val);
	//	len = count(val);
	//	unset(val[len-1]);

	//	return unicode = val;
	//}

}//____________________________________________________________________

wstring UTF_to_Unicode(const char*name){string str=name; return UTF_to_Unicode(str); };

void UTF_to_UnicodeVector(string &input,vector<short>&vt) {
    if(input.size()==0)return;
    //cout<<"input="<<input<<endl;
	unsigned char ints;
	//int bits=0;
	string val;
	string value;
	//val   = array();
	unsigned char z,y,x,w,v,u;
	int i;
	//int len=strLen(input);
	int bit1,bit2,bit3,bit4,bit5,bit6;
    
    /*
    // сделать флаг проверки (в тексте) на совместимость с Unicode
    // заменить >= на > с уменьшенмем на единицу проверяемого числа
	for(i=0; i<len; i++){
		c=input[i]; //cout<<"c="<<(short)c<<endl;
		if(c > 128){
			if((c >= 254||c < 192)){return vt.resize(input.size()/2+2); memcpy((char*)&vt[0],(char*)&input[0],input.size());}
			if(c >= 192) bits=2;
			if(c >= 224) bits=3;
			if(c >= 240) bits=4;
			if(c >= 248) bits=5;
			if(c >= 252) bits=6;
			if((i+bits) > len) {return vt.resize(input.size()/2+2); memcpy((char*)&vt[0],(char*)&input[0],input.size());}
			//cout<<"i="<<i<<" bits="<<bits<<" len="<<len<<endl;
            
			while(bits > 1){
				i++;
				b=input[i];
				if(b < 128 || b > 191){return vt.resize(input.size()/2+2); memcpy((char*)&vt[0],(char*)&input[0],input.size());}
				bits--;
			}
		}
	}
    
	cout<<"input1="<<input<<endl;
    */
    
 	//if(!is_utf8(input)return input;
    
	//bit1  = pow(64, 0);   //php code
	//bit2  = pow(64, 1);
	//bit3  = pow(64, 2);
	//bit4  = pow(64, 3);
	//bit5  = pow(64, 4);
	//bit6  = pow(64, 5);
    
	bit1 = 1;
	bit2 = 64;
	bit3 = 4096;
	bit4 = 262144;
	bit5 = 16777216;
	bit6 = 1073741824;
    //char buff[33];
    
	for(i=0; i< strLen( input ); i++){
        
		ints = (unsigned char)input[i];
        
		z     = (unsigned char) input[i];
		y     = (unsigned char) input[i+1] - 128;
		x     = (unsigned char) input[i+2] - 128;
		w     = (unsigned char) input[i+3] - 128;
		v     = (unsigned char) input[i+4] - 128;
		u     = (unsigned char) input[i+5]  - 128;
        
		if( ints <= 127 ){
			// 1 bit
			//sprintf(buff,"%d",(z * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
            vt.push_back(z*bit1);
            //val[]  = value;
		}
        
		if( ints >= 192 && ints <= 223 ){
			// 2 bit
			//sprintf(buff,"%d",((z-192) * bit2 + y * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
            vt.push_back((z-192) * bit2 + y * bit1);
			//val[]  = value;
		}
        
		if( ints >= 224 && ints <= 239 ){ 
			// 3 bit
			//sprintf(buff,"%d",((z-224) * bit3 + y * bit2 + x * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			vt.push_back((z-224) * bit3 + y * bit2 + x * bit1);
			//val[]  = value;
		}
		if( ints >= 240 && ints <= 247 ){
			// 4 bit
			//sprintf(buff,"%d",((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			vt.push_back((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1);
			//val[]  = value;
		}
		if( ints >= 248 && ints <= 251 ){
			// 5 bit
            
			//sprintf(buff,"%d",((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			vt.push_back((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1);
			//val[]  = value;
		}
		if( ints == 252 && ints == 253 ){
			// 6 bit
			//sprintf(buff,"%d",((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			vt.push_back((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1);
			//val[]  = value;
		}
		if( ints == 254 || ints == 255 ){
			cout<<"Wrong Result!"<<endl;
		}
        
        
	}
    
	//cout << "val="<<val<<endl;
    
	//if( array === False ){
	return;
	//}
	//if(array === True ){
	//	val     = str_replace('&#', '', value);
	//	val     = explode(';', val);
	//	len = count(val);
	//	unset(val[len-1]);
    
	//	return unicode = val;
	//}
    
}//____________________________________________________________________


string Unicode_to_UTF( wstring &input){

	int c=0, b=0, flag=0, i;
	int bits=0;
	int len=strLenW(input); //cout<<"len="<<len<<endl; 
	if(!len)return "";
	int byte1,byte2,byte3,byte4,byte5,byte6;
	string utf="";


	for(i=0; i<len; i++){
		c=input[i];
		// test BOM and remove it
	   //	if(i==0&&c==0xEF)continue;
	   //	if(i==1&&c==0xBB)continue;
	   //	if(i==2&&c==0xBF)continue;
		//

		utf+=(char)c;  //cout<<"c="<<(short)c<<endl; 
		if(c > 128){
			if(c >= 252) bits=6;
			if(c >= 248) bits=5;
			if(c >= 240) bits=4;
			if(c >= 224) bits=3;
			if(c >= 192) bits=2;
			flag=1; i=len;
		}
		if((i+bits) > len) {flag=1; i=len;}//need convert
		while(bits > 1){
			i++;
			b=input[i];
			if(b < 128 || b > 191) {flag=1; i=len;}//need convert
			bits--;
		}
	}
	//cout<<"flag="<<flag<<END;
	if(!flag)return utf;
	utf = "";
	//if(!is_array(input)){
	//input     = str_replace('&#', '', input);
	//input     = explode(';', input);
	//len = count(input);
	//unset(input[len-1]);
	//for(i=0; i < len; i++)inputArray.push_back(input[i]);

	//}else{count=count(input);}

	for(i=0; i < len; i++){

	    // test BOM and remove it
	   //	if(i==0&&c==0xEF)continue;
	   //	if(i==1&&c==0xBB)continue;
	   //	if(i==2&&c==0xBF)continue;
		//print input[i];
		//print "__i<br>";
		if ( input[i] <128 ){
			byte1 = input[i];
			utf  += (char)byte1;
		}

///@@@@///	//__set_win_1251 charset___
			//if(input[i]>191&&input[i]<256)input[i]+=848;
			//if(input[i]==184)input[i]=0x0451;
			//if(input[i]==168)input[i]=0x0401;


		if ( input[i] >=128 && input[i] <=2047 ){
			byte1 = 192 + (int)(input[i] / 64);
			byte2 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;



		}
		if ( input[i] >=2048 && input[i] <=65535){
			byte1 = 224 + (int)(input[i] / 4096);
			byte2 = 128 + ((int)(input[i] / 64) % 64);
			byte3 = 128 + (input[i] % 64);

			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			//print utf;
			//print "___<br>";
		}
		if ( input[i] >=65536 && input[i] <=2097151){

			byte1 = 240 + (int)(input[i] / 262144);
			byte2 = 128 + ((int)(input[i] / 4096) % 64);
			byte3 = 128 + ((int)(input[i] / 64) % 64);
			byte4 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
		}
		if ( input[i] >=2097152 && input[i] <=67108863){

			byte1 = 248 + (int)(input[i] / 16777216);
			byte2 = 128 + ((int)(input[i] / 262144) % 64);
			byte3 = 128 + ((int)(input[i] / 4096) % 64);
			byte4 = 128 + ((int)(input[i] / 64) % 64);
			byte5 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
			utf  += (char)byte5;
		}
		if ( input[i] >=67108864 && input[i] <=2147483647){

			byte1 = 252 + (input[i] / 1073741824);
			byte2 = 128 + ((input[i] / 16777216) % 64);
			byte3 = 128 + ((input[i] / 262144) % 64);
			byte4 = 128 + ((input[i] / 4096) % 64);
			byte5 = 128 + ((input[i] / 64) % 64);
			byte6 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
			utf  += (char)byte5;
			utf  += (char)byte6;
		}
	}

	//cout<<"utf="<<utf<<END;
	return utf;
}//_____________________________________________________________________


string Unicode_to_UTF( wchar_t input){
wstring in=L"";
in+=input;
return Unicode_to_UTF(in);

}//_____________________________________________________________________

string Unicode_to_UTF(string &input){
	
	wstring strW;
	for(int i=0;i<input.size();i++){
	strW+=(wchar_t)(unsigned char)input[i];
		//cout<<input[i]<<"="<<(short)strW[i]<<END;	
	}
	//cout<<"strW.size()="<<strW.size();
	return Unicode_to_UTF(strW);
	
}//_____________________________________________________________________

string substr(int inInd,int count, string &srcString ){
    if(inInd+count<srcString.size()){
        return srcString.substr(inInd,count);
    }else{
        return srcString.substr(inInd,srcString.size()-inInd);
    }
   return "";
}

string substr(int inInd,string &srcString ){
return srcString.substr(inInd,srcString.size()-inInd);
}

string substrT(const char *buf, string &srcString){
return srcString.substr(srcString.rfind(buf),srcString.size()-srcString.rfind(buf));
}

string substrH(const char *buf, string &srcString){
return srcString.substr(0,srcString.rfind(buf));
}const char *substr(int inInd,int count, const char *str1){
    char *dest = (char*) malloc(count+1);
    strncpy(dest, str1+inInd, count);
    dest[count]=0;
    return dest;
}//_____________________________________________________________________________



vector<string> explode(const string& delimiter,const string& input){

	vector<string> result;
	int start;
	string data=input; 
	start=1;
    //cout<<" delimiter="<<delimiter<<" input="<<input<<endl;
    //for(int a=0;a<input.size();a++)cout<<"a="<<a<<" c="<<(short)input[a]<<" "<<input[a]<<endl;
	while ( start != -1){
		start = (int)data.find(delimiter); //cout<<"start="<<start<<" input="<<input.size()<<endl;
		if(start>-1){
		  result.push_back(substr(0,start,data));
		  data=substr(start+(uint)delimiter.size(),data);
		}else{
		  result.push_back(data);
		}
	}
    return result;
}//_____________________________________________________________________________

vector<wstring> explodeW(const wstring& delimiter,
						 const wstring& input){

	vector<wstring> result;
	int n = (uint)input.length();
	int start, stop;
	// найти первую букву, не разделитель
	start = (uint)input.find_first_not_of(delimiter);
	while ( (start >= 0) && (start < n) ){
		// найти конец текущего слова
		stop = (uint)input.find_first_of(delimiter, start);
		if ( (stop < 0) || (stop > n) ) stop = n;
		// добавить слово к списку слов
		result.push_back(input.substr(start, stop-start));
		// найти начало следующего слова
		start = (uint)input.find_first_not_of(delimiter,stop+1);
	}
    return result;
}//_____________________________________________________________________________

#ifdef OCRLib
vector<wordOCR> explode(const vector<string>& delimiter, const string& input){
	
	vector<wordOCR> result;
	int n = (uint)input.length();
	int start, stop, minStop,dlt;
	string delimeter;
	start = 0;
	int count =(uint)delimiter.size();
	
	
	while (start <n){
		wordOCR strItem;
		minStop=1000; dlt=start;
		for(int i=0;i<count;i++){
			    stop = (uint)input.find(delimiter[i], start);
			    //cout<<" stop="<<stop<<" delimiter[i]="<<delimiter[i] <<END;
				if(stop<minStop&&stop>-1){minStop=stop; strItem.delimeter=delimiter[i];}
			   
		} 
		//start-=1;
		//if(start<0)start=0;	
		//cout<<" start="<<start<<" minStop="<<minStop<<END;
	   strItem.name=input.substr(start,minStop-start);	
		if(strItem.name=="")strItem.name=strItem.delimeter;
	    //cout<<"n="<<strItem.name<<"/ d="<< strItem.delimeter<<" start="<<start<<" minStop="<<minStop<<" s="<<strItem.delimeter.size()<<END;
	   start=minStop+(uint)strItem.delimeter.size();
	   result.push_back(strItem);
	}
    return result;
	

}


vector<stringOCR> explode(const vector<string>& delimiter, stringOCR &input){
	
	vector<stringOCR> result;
	int n = input.length();
	int start, stop, minStop,dlt;
	string delimeter_;
	start = 0;
	int count =(uint)delimiter.size();
	
	//cout<<"input.str=/"<<input.str<<"/"<<endl;
	
	while (start <n){
		stringOCR strItem;
		minStop=1000; dlt=start;
		for(int i=0;i<count;i++){
			stop = input.find(delimiter[i], start);
			//cout<<" stop="<<stop<<" delimiter[i]="<<delimiter[i] <<END;
			if(stop<minStop&&stop>-1){minStop=stop; delimeter_=delimiter[i];}
			
		} 
		//start-=1;
		//if(start<0)start=0;	
		strItem=input.substr(start,minStop-start);	//cout<<"strItem.str=/"<<strItem.str<<"/"<<endl;
		strItem.delimeter=delimeter_;
		//cout<<" start="<<start<<" minStop="<<minStop<<"strItem="<<strItem.str<<" strItem.size()="<<strItem.size()<<END; 
		if(strItem.str=="")strItem.str=strItem.delimeter;
	    //cout<<"n="<<strItem.str<<"/ d="<< strItem.delimeter<<" start="<<start<<" minStop="<<minStop<<" s="<<strItem.delimeter.size()<<END;
		start=minStop+1;
		if(strItem.str!=" ")result.push_back(strItem);
	}
    return result;
	
	
}//___________________________________________________________________________


string implode(const string& delimiter,  const vector<wordOCR>& input){
    string result;
    unsigned int i;
    for(i=0;i<input.size();i++){
        result+=input[i].name;
        result+=delimiter;
    }
    return result;
}//_____________________________________________________________________________


#endif 

string implode(const string& delimiter,
			   const vector<string>& input){
	string result;
	unsigned int i;
	for(i=0;i<input.size();i++){
		result+=input[i];
		result+=delimiter;
	}

	return result;
}//_____________________________________________________________________________

string implode(const char *delimiter,
               const vector<string>& input){
    string result;
    unsigned int i;
    for(i=0;i<input.size();i++){
        result+=input[i];
        result+=delimiter;
    }
    
    return result;
}//_____________________________________________________________________________


wstring implodeW(const wstring& delimiter,
				 const vector<wstring>& input){
	wstring result;
	unsigned int i;
	for(i=0;i<input.size();i++){
		result+=input[i];
		result+=delimiter;
	}
	return result;
}//_____________________________________________________________________________

string implode(const string& delimiter, list<string>& input){
    string result;
    std::list<string>::iterator it;
    for (it = input.begin(); it != input.end(); it++){
        result+=*it;
        result+=delimiter;
    }
    return result;

}

string implode(const char *delimiter, list<string>& input){
    string result;
    std::list<string>::iterator it;
    for (it = input.begin(); it != input.end(); it++){
        result+=*it;
        result+=delimiter;
    }
    return result;
    
}

//fast file reading in memory
unsigned long readText(char *buff, const char *path){
	FILE * hFile;
    unsigned long  size;
	hFile = fopen( path, "r");
	if (hFile == NULL){
	return 0; 
	}else{
		fseek(hFile, 0, SEEK_END);
		size= ftell(hFile);   //cout<<" size="<<size<<endl;
		fseek(hFile, 0, SEEK_SET);
		buff = (char *)malloc(size+1);
		if (size != fread(buff, sizeof(char), size, hFile)){
			free(buff);
			return 0;
		}
		fclose(hFile);
	}
    return size;
	//cout<<"done read"<<endl;
}

//fast file reading in memory
void readText(string &str,const char *path){

	//cout<<"start read "<<path;
    str="";
	FILE * hFile;
	hFile = fopen( path, "rb");
	//char *buff;
	unsigned long  size;
	if (hFile == NULL){
		cout_<<"no open file "<<path;
		return;
	}else{
		fseek(hFile, 0, SEEK_END);
		size= ftell(hFile);   //cout<<" size="<<size<<endl;
		fseek(hFile, 0, SEEK_SET);
		str.resize(size);
		unsigned long readDataSize=fread(&str[0], sizeof(char), size, hFile);
		if (readDataSize != size){
			str="";
			cout_<<"no read file "<<path<<" readDataSize="<<readDataSize<<endl;
			return ;
		}
		fclose(hFile);
	}
	//cout<<"done read size="<<size<<endl;
	return;


}


//file reading in vector
void readText(vector<string> &strVector, string &inFile){
	ifstream srcInput;
	srcInput.open(inFile.c_str());
	string str,srcLine;
	
	while(getline(srcInput, srcLine,'\n')){
		strVector.push_back(srcLine);
	}
	srcInput.close();
	
}

void readText(map<string,uint> &strMap, string &inFile){
    ifstream srcInput;
    srcInput.open(inFile.c_str());
    string str,srcLine;
    int i=0;
    while(getline(srcInput, srcLine,'\n')){
        strMap[srcLine]=i;
        i++;
    }
    srcInput.close();
}

void readText(string& str,string &inFile){
	readText(str,inFile.c_str());
}

string readText(string &inFile){
    string str;
	readText(str,inFile.c_str());
    return str;
}

unsigned long readInMemory(char **textBuffer,string &path){
    FILE* hFile = fopen(path.c_str(), "r");
    fseek(hFile, 0, SEEK_END);
    unsigned long size= ftell(hFile);   //cout<<" path="<<path<<" size="<<size<<endl;   
    fseek(hFile, 0, SEEK_SET);
    fclose(hFile);
    int fd = -1;
    fd = open(path.c_str(), O_RDWR);
    
    *textBuffer=(char*)mmap(0,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(textBuffer == MAP_FAILED){cout<<"mmap error";
        int myerr = errno;
        printf("ERROR (child): mmap failed (errno %d %s)\n", myerr,strerror(myerr));
        exit(0);}
    fclose(hFile);
    return size;

}

void readMapXML(map<string,string>&inputMap,string &path){

	xml_node col,data,resultSet,GFontXML, st;
	xml_document doc;
	string str0,str1;
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;return;
	}
	
	resultSet = doc.child("GFont");
	// Iterate through books
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		//cout<<"str1="<<str1<<" str0="<<str0<<END;
		inputMap[str1]=str0;		
	}	
		
}//_____________________________________________________________________________

void  writeText(char *buff, unsigned int size, const char *path){
	FILE * hFile;
	hFile = fopen( path, "w");
	if (hFile == NULL){
		return; 
	}else{
		fwrite(buff, sizeof(char), size,hFile);
	}
	fclose(hFile);
}//_____________________________________________________________________________________

void writeText(const char *data, const char *path){
	FILE * hFile;
	hFile = fopen( path, "w");
	if (hFile == NULL){
		return; 
	}else{
		fwrite(data, sizeof(char), strlen(data), hFile);
	}
	fclose(hFile);
}

//file write from vector
void writeText(vector<string> &strVector, string &outFile){
	ofstream srcOutput;
	srcOutput.open(outFile.c_str());
	for(int i=0;i<strVector.size();i++){
		srcOutput<<strVector[i]<<endl;
	}
	srcOutput.close();	
}

//file write from vector
void writeText(vector<string> &strVector, const char* dest){
	string outFile=dest;
	writeText(strVector,outFile);

}

struct flock* file_lock(short type, short whence) {
    static struct flock ret ;
    ret.l_type = type ;
    ret.l_start = 0 ;
    ret.l_whence = whence ;
    ret.l_len = 0 ;
    ret.l_pid = getpid() ;
    return &ret ;
}

int writeText(string &outStr, string &outFile){
	FILE * hFile;
	hFile = fopen( outFile.c_str(), "wb");
    
    unsigned int size;
	if (hFile == NULL){
		cout<<outFile<<" not open"<<END;
		return 0;
	}else{
		size=fwrite(outStr.c_str(), sizeof(char), outStr.size(),hFile);
        if(outStr.size()!=size){
            fclose(hFile);
            return 0;
        }    
	}
    
	fclose(hFile);
    
    return size;
}

void writeText(string &outStr, const char *path){
	string outFile=path;
    writeText(outStr,outFile);
}

void writeText(const char *data, string &outFile){
	string outStr=data;
    writeText(outStr,outFile);
}

void writeToLog(string &outStr, const char *path,const char *flagFile){
	string str="1";
	while(str!=""){
     	readText(str,flagFile);
		//cout<<"WAIT IN LINE"<<endl;
#ifdef UNIX
		sleep(1);
#else
		Sleep(1000);
#endif
	}
	str="1";
	writeText(str,flagFile);
	
	//FILE * hFile;
	//hFile = fopen( path, "a");
	//fwrite(outStr.c_str(), sizeof(char), outStr.size(), hFile);
	//fclose(hFile);

	ofstream srcOutput;
	srcOutput.open(path,fstream::out|fstream::app);
	if(!srcOutput){cout_<<path<<" not open"<<END; exit(0);}
	srcOutput<<outStr;
	srcOutput.close();	
	
	str="";
	writeText(str,flagFile);
}

bool is_file(string &path){
	///cout<<"start check "<<path;
    if(!path.size())return 0;
	FILE * hFile;
	hFile = fopen( path.c_str(), "r");
    if(!hFile)return 0;
	fseek(hFile, 0, SEEK_END);
	unsigned long  size= ftell(hFile);   //cout<<" size="<<size<<endl;   
	if(size<4)return 0; 
	
	fclose(hFile);
    
    DIR* dir;
    if( ( dir=opendir(path.c_str()))!=NULL)return 0;
    
	//cout<<"_exist";
	return 1;
	
}

bool is_file(const char *path){
	///cout<<"start check "<<path;
    if(path==NULL)return 0;
	FILE * hFile;
	hFile = fopen( path, "r");
    if(!hFile)return 0;
	fseek(hFile, 0, SEEK_END);
	unsigned long  size= ftell(hFile);   //cout<<" size="<<size<<endl;   
	if(size<4)return 0; 
	
	fclose(hFile);
    
    DIR* dir;
    if( ( dir=opendir(path))!=NULL)return 0;
    
	//cout<<"_exist";
	return 1;
	
}

bool is_dir(string &path){
    DIR* dir;
    if( ( dir=opendir(path.c_str()))!=NULL)return 1;
    return 0;
}
bool is_dir(const char* path){
    DIR* dir;
    if( ( dir=opendir(path))!=NULL)return 1;
    return 0;
}

bool fileOpen(string &path){
	cout<<"start check "<<path;
	
	FILE * hFile;
	hFile = fopen( path.c_str(), "r");
	char buff[5];
	string str;
	unsigned int size;
	if (hFile == NULL){
		return 1; 
	}else{
		fseek(hFile, 0, SEEK_END);
		unsigned long  size= ftell(hFile);   cout<<" size="<<size<<endl;   
		if(size<4)return 0; 
		
		fseek(hFile, size-4, SEEK_SET);
		fread(buff, sizeof(char), 4, hFile);
		fclose(hFile);
	}
	buff[4]=0;
		cout<<"done read "<<buff<<endl;
	str=buff;
	if(str=="<r/>"){
		fseek(hFile, size-4, SEEK_SET);
		memset(buff,0,4);
		fwrite(buff, sizeof(char), 4, hFile);
		return 1;
	}	
	return 0;
}


bool emptyFolder(string &dirPath){
    DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	//int result;
	string str;
	if(dirPath=="")return 0;
    string path=dirPath;
	vector<string>fileType;
    int print=0;
	
	if( ( dir=opendir(path.c_str()))!=NULL){
		while((entry=readdir(dir))!=NULL){
			path=dirPath;
			path+="/";
            str=(char*)entry->d_name; DT("str="<<str<<endl);
			if(str[0]=='.')continue;
			path+=str;
            DT("path="<<path<<endl;)
			if( stat( path.c_str(), &attribut ) == -1 ){
			}else{
				if(remove(path.c_str())){
                  DT("no remove path="<<path<<endl;)
                };    
			}
		}
        closedir( dir );
	}else  return 0;
	return 1;


}


struct data_traverser: public xml_tree_walker{virtual bool for_each(xml_node& n)	{
		static string name;
		//for (int i = 0; i < depth(); ++i) cout << "  "; // indentation

		if (n.type() == node_element){
		   //cout <<"name="<<n.name() << END;
		   name=n.name();
		}else{
		   inputData.data[name]=n.value();
			//cout<<"inputData.data["<<name<<"]="<< inputData.data[name]<<endl;
		    //cout <<"name="<<name<<" value="<<n.value() << END;
		}
		return true; // continue traversal
	}

};

string str_replace(const char *srcChar, const char *destChar, string &srcString ){
    //cout<<" srcString="<<srcChar<<endl;
	string src=srcChar;
	string dest=destChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	string result=srcString;
		while ( (pos = result.find(src, pos)) != string::npos ) {
			result.erase(pos, src.size());
			result.insert(pos,dest);
			//cout<<"s="<<srcString<<endl;
			pos+=dest.size();
		}
	return result;

}//_____________________________________________________________________________

string str_replace(const char *srcChar, string &dest, string &srcString ){

	string src=srcChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	string result=srcString;
    while ( (pos = result.find(src, pos)) != string::npos ) {
        result.erase(pos, src.size());
        result.insert(pos,dest);
        //cout<<"s="<<srcString<<endl;
        pos+=dest.size();
    }
	return result;

}//_____________________________________________________________________________


string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar ){

	string src=srcChar;
	string dest=destChar;
	string srcString=srcStringChar;
	if(src==dest)return srcString;

	string::size_type pos = 0;
	string result=srcString;
    while ( (pos = result.find(src, pos)) != string::npos ) {
        result.erase(pos, src.size());
        result.insert(pos,dest);
        //cout<<"s="<<srcString<<endl;
        pos+=dest.size();
    }
	return result;

}//_____________________________________________________________________________

string str_replace(string &src, string &dest, string &srcString , int count){
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int index=0;
	//int foundFlag=1;
	string result=srcString;
	while ( (pos = result.find(src, pos)) != string::npos&&index<count ) {
		result.erase(pos, src.size());
		result.insert(pos,dest);
		pos+=dest.size();index++;
	}
	return result;

}//_____________________________________________________________________________

string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar, int count ){

	string src=srcChar;
	string dest=destChar;
	string srcString=srcStringChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int index=0;
	string result=srcString;
	while ( (pos = result.find(src, pos)) != string::npos&&index<count ) {
		result.erase(pos, src.size());
		result.insert(pos,dest);
		pos+=dest.size();index++;
	}
	return result;

}//_____________________________________________________________________________


string str_replace(string &src, string &dest, string &srcString ){
    
    //cout <<"start src="<<src<<"/ dest="<<dest<<"/ srcString="<<srcString<<endl;
    if(src==dest)return srcString;
	string::size_type pos = 0;
	int foundFlag=1;
	string result=srcString;
		while ( (pos = result.find(src, pos)) != string::npos ) {  //cout<<"pos1="<<pos<<endl;
			result.erase(pos, src.size());
			result.insert(pos,dest);
			foundFlag=1;
			pos+=dest.size(); //cout<<"pos2="<<pos<<endl;
		}
    
    //cout <<"done result="<<result<<endl;
	return result;
	
}//_____________________________________________________________________________


string str_replace(vector<string> &srcArray, const char* destData, string &srcString ){
	//cout <<"start"<<endl;
	string::size_type pos = 0;
	string dest=destData;
	int foundFlag;
	string result=srcString;
	
	for(int i=0;i<srcArray.size();i++){
			if(srcArray[i]==result)continue;
            pos = 0;
			while ( (pos = result.find(srcArray[i], pos)) != string::npos ) {
				result.erase(pos, srcArray[i].size());
				result.insert(pos,dest);
				foundFlag=1;
				pos+=dest.size();
			}
	}	
	//cout <<"done"<<endl;
	return result;
	
}//_____________________________________________________________________________

void stripLetters(string &str){     //remove all letters from str
    string result;
    for(int i=0;i<str.size();i++){
        if(str[i]>0x0040&&str[i]<0x005B)continue;
        if(str[i]>0x0060&&str[i]<0x007B)continue;
        result+=str[i];
    }
    str=result;
    
}
    
string ptrToStr(const char*st,int ln){

	string str;
	str.resize(ln);
	memcpy(&str[0], st,ln);
    return str;
}


string fileName(string &path){
	string str=path;
	return str.erase(0,str.rfind("/")+1);
}
string fileExt(string &path){
	string str=path;
	return str.erase(0,str.rfind(".")+1);
}

string folderPath(string &path){
	string str=path;
    size_t found=str.rfind("/")+1;
	str.erase(found,str.size()-found);
    return str;
}


string intToHex(int input){
	string result;
	char buffer [10];

	sprintf (buffer, "%X", input);
	result=buffer;

	return result;
}
string charToHex(unsigned char input){
	string result;
	char buffer1 [3];
	char buffer2 [3];
	
	
	sprintf (buffer1, "%X", input);
	if(buffer1[1]==0){
		buffer2[0]='0';
		buffer2[1]=buffer1[0];
		buffer2[2]=0;
		result=buffer2;
	}else{
		result=buffer1;		
	}
	
	return result;
}

string HTMLtoTxt(string &input){
	string destStr;
	int flag=0;
	
	for(int a=0;a<input.size();a++){
		if(input[a]=='<')flag=1;
		if(input[a]=='>'){flag=0;continue;}
	    if(!flag)destStr+=input[a];
	}
	return destStr;
}



void convertFileToUTF8(const char* path){
/*	
	string srcLine;
	string destString;
	ifstream textData(path);
	if( !textData ){
		cout<<"Source path "<<textData<<" not exist"<<endl;
	}else{
		
		while (getline(textData, srcLine,'\n')){
			
			destString+=string_to_UTF(srcLine,"windows-1251")+"\n";
			
			
		}
		
	} textData.close();
	ofstream c_out;  c_out.open(path);
	c_out<<destString;
	c_out.close();
*/	
}//_____________________________________________________________________________


void convertDirectoryToUTF(){
#ifdef PC
	char apath[1024];
	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	int result;

	if( ( dir=opendir(inputData.data["folderPath"]))!=NULL){

		while((entry=readdir(dir))!=NULL){

			memset(apath,'\0',sizeof(apath));
			strcpy(apath,path);
			strcat(apath,"/");
			strcat(apath,(char*)entry->d_name);

			if(strcmp((char*)entry->d_name,".")!= 0&&strcmp((char*)entry->d_name,"..")!= 0){
				printf("%s\n",apath);
			}
			if( stat( apath, &attribut ) == -1 )
				printf( "stat(%s) failed\n", apath );
			else{
				if((attribut.st_mode & S_IFDIR)&&strstr(apath,".")==NULL){
					convertDirectoryToUTF( apath );
				}else{
					char pch[4];
					strncpy (pch,apath+strlen(apath)-3,3);
					cout<<"pch=" <<pch<<endl;
					if(pch=="php"){
						
						cout<<"need convert"<<apath<<endl;
						convertFileToUTF8(apath);
						
					}
				}
			}
		}
	}
	else
		//printf("opendir(%s) failed\n", path);

	closedir( dir );
#endif

}//_____________________________________________________________________________


void readInputData(const char *ext){

	if(inputData.data["InputMethod"]=="fileList"){
		if(!inputData.fileList.size()){
			readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],ext);
		}
	}else{
	    ofstream c_out; c_out.open("/_out_text.txt");
		for(int i=0;i<inputData.fileList.size();i++){
			c_out<<inputData.fileList[i]<<endl;
			//cout<<inputData.fileList[i]<<i<<endl;
		}
		c_out.flush();
		c_out.close();
	    inputData.fileList.resize(0);
		inputData.fileList.push_back("/_out_text.txt");
	}	
}


int readDirectoryToArray(vector<string>&fileList, string dirPath, string ext){

	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	//int result;
	string str;
    int print=0;
	DT("dirPath="<<dirPath<<END);
	if(dirPath=="")return 0;
    if(is_file(dirPath))return 0;
	string path=dirPath;
	
	vector<string>fileType;
	if(ext=="img"){
		fileType.push_back(".jpg");
		fileType.push_back(".JPG");
		fileType.push_back(".jpeg");
		fileType.push_back(".JPEG");
		fileType.push_back(".tif");
		fileType.push_back(".TIF");
		fileType.push_back(".tiff");
		fileType.push_back(".TIFF");
		fileType.push_back(".gif");
		fileType.push_back(".GIF");
		fileType.push_back(".png");
		fileType.push_back(".PNG");
        fileType.push_back(".pdf");
		fileType.push_back(".PDF");
	}else if(ext=="txt"){
		fileType.push_back(".txt");
		fileType.push_back(".TXT");
		fileType.push_back(".h");
		fileType.push_back(".cpp");
        fileType.push_back(".mm");
        fileType.push_back(".c");
	}else if(ext=="html"){
		fileType.push_back(".php");
		fileType.push_back(".PHP");
		fileType.push_back(".htm");
		fileType.push_back(".HTM");
		fileType.push_back(".html");
		fileType.push_back(".HTML");
		fileType.push_back(".phtml");
	}else if(ext=="rtf"){
	    fileType.push_back(".rtf");
		fileType.push_back(".doc");
		fileType.push_back(".RTF");
		fileType.push_back(".DOC");
	}else if(ext=="xml"){	
		fileType.push_back(".xml");
		fileType.push_back(".XML");
	}else {
		fileType.push_back(ext);
	}
	int fileFlag=0;
	
	if( ( dir=opendir(path.c_str()))!=NULL){
		while((entry=readdir(dir))!=NULL){
			//memset(apath,'\0',sizeof(apath));
			//strcpy(apath,path.c_str());
			//strcat(apath,"/");
			path=dirPath;
			path+="/";
			str=(char*)entry->d_name; DT("str="<<str<<endl);
			if(str[0]=='.')continue;
			if(str.find("_mark")!=-1)continue;
			if(str.find("_press")!=-1)continue;
			path+=str;

			DT("path="<<path<<endl);
			
			
			if( stat( path.c_str(), &attribut ) == -1 ){
				DT("stat "<<path <<" failed"<<endl);
			}else{
				if((attribut.st_mode & S_IFDIR)){
					DT("recursion read "<<path<<endl);
					fileFlag=readDirectoryToArray(fileList,path, ext);
				}else{
					int count=0;
					for(int i=0;i<fileType.size();i++){
						if(path.rfind(fileType[i])==path.size()-fileType[i].size()){
							count++;
							fileFlag=1;  
						    break;
						}
					}          
					if(count==0)continue;
					DT(path<<endl);
					fileList.push_back(path);				}
			}
		}
	closedir( dir );
    }if(is_file(dirPath)){
        for(int i=0;i<fileType.size();i++){
            if(path.rfind(fileType[i])==path.size()-fileType[i].size()){
                fileList.push_back(path);	
                break;
            }
        }
    }
    //printf("opendir(%s) failed\n", dirPath.c_str());
	//cout<<" fileFlag="<<fileFlag;
	return fileFlag;
	
}//_____________________________________________________________________________


void readDirectory(vector<string>&fileList,vector<string>&folderList,string dirPath){
	
	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	//int result;
	string str;   dirPath=str_replace("%20", " ", dirPath);
	//cout<<"dirPath="<<dirPath<<END;
	if(dirPath=="")exit(0);
	string path=dirPath;
	return;
	if( ( dir=opendir(path.c_str()))!=NULL){
		while((entry=readdir(dir))!=NULL){
			//memset(apath,'\0',sizeof(apath));
			//strcpy(apath,path.c_str());
			//strcat(apath,"/");
			path=dirPath;
			path+="/";
			str=(char*)entry->d_name;
			if(str[0]=='.')continue;
			path+=str;
			//cout<<"path="<<path<<END;
			
			
			if( stat( path.c_str(), &attribut ) == -1 ){
				//cout<<"stat "<<path <<" failed"<<END;
			}else{
				if((attribut.st_mode & S_IFDIR)){
					//cout<<"recurcion read "<<path<<END;
					//readDirectory(fileList,path);
					path=str_replace("//", "/", path);
					folderList.push_back(path);
				}else{
					//cout<<path<<END;
				path=str_replace("//", "/", path);
				fileList.push_back(path);				}
			}
		}
	}else  //printf("opendir(%s) failed\n", dirPath.c_str());
	
	closedir( dir );
	
	
}//_____________________________________________________________________________



size_t  write_data(void *ptr,
					 size_t size,
					 size_t nmemb,
					 FILE *stream){

	static int first_time=1;
	char outfilename[FILENAME_MAX] = "body.out";
	static FILE *outfile;
	size_t written;
	if (first_time) {
		first_time = 0;
		outfile = fopen(outfilename,"w");
		if (outfile == NULL) {
			return -1;
		}
		fprintf(stderr,"The body is <%s>\n",outfilename);
	}
	written = fwrite(ptr,size,nmemb,outfile);
	return written;

}//_____________________________________________________________________________________


		//char cbuf[12];
		//for(int in=5744;in<70000;in++){
		//string str="http://www.buddhistdoor.com/glossary/word_eng.php?word_id=";
		//sprintf(cbuf,"%d",in);
		//str+=cbuf;
		//cout<<"yagpo_dict -curl "<<str<<endl<<"move body.out body"<<cbuf<<".out"<<endl;
		//}

		//if(argVector.size()<2)usage(argVector);

		//if(argVector[1]=="-curl"){
		//	libCurl(argVector);
		//}

void libCurl(vector<string>&argVector){
#ifdef PC

HINSTANCE cl = NULL;

  if((cl = LoadLibrary( "libcurl.dll" ))==NULL ){
  cout<<"I can't load libcurl";
  FreeLibrary(cl); exit(0);
  }
  CURL*(__stdcall *curl_easy_init)();
  CURLcode(__stdcall *curl_easy_setopt )
   (CURL *curl, CURLoption option, ...);
  CURLcode (__stdcall *curl_easy_perform )
   (CURL *curl);
  CURLcode(__stdcall *curl_easy_getinfo )
   (CURL *curl, CURLINFO info, ...);
  void (__stdcall *curl_easy_cleanup )
   (CURL *curl);
  struct curl_slist *(__stdcall
   *curl_slist_append )
  (struct curl_slist *list,const char *string);
  void (__stdcall *curl_slist_free_all)
  (struct curl_slist * list);

// присваиваем указателям функций  соответствующие адреса функции DLL
  curl_easy_init =(CURL*(__stdcall*)())GetProcAddress(cl,"curl_easy_init" );
  curl_easy_setopt =(CURLcode(__stdcall *)(CURL *curl, CURLoption option, ...))
  GetProcAddress(cl, "curl_easy_setopt");
  curl_easy_perform =(CURLcode(__stdcall *)(CURL *curl))
  GetProcAddress(cl, "curl_easy_perform");
  curl_easy_cleanup =(void (__stdcall *)(CURL *curl))
  GetProcAddress(cl, "curl_easy_cleanup");
  curl_easy_getinfo=(CURLcode(__stdcall *)
  (CURL *curl, CURLINFO info, ...))GetProcAddress(cl, "curl_easy_getinfo");
  curl_slist_append=(curl_slist*(__stdcall *)
  (struct curl_slist *list,const char *string))GetProcAddress(cl,"curl_slist_append");
  curl_slist_free_all=(void(__stdcall *)
	 (struct curl_slist *list))
	 GetProcAddress (cl,"curl_slist_free_all");

	 char cbuf[10];   int i=0;

	//for(int in=5744;in<70000;in++){
		CURL *curl_handle;
		char headerfilename[FILENAME_MAX] = "head.out";
		FILE *headerfile;
		int rc=0;
		curl_handle = curl_easy_init();
		//string str="http://www.buddhistdoor.com/glossary/word_eng.php?word_id=";
		//sprintf(cbuf,"%d",id);
		//str+=cbuf;
		string str=argVector[2];
		cout<<"start download "<<str<<endl;
		curl_easy_setopt(curl_handle,   CURLOPT_URL, str.c_str());
		curl_easy_setopt(curl_handle,   CURLOPT_NOPROGRESS  ,1);
		//curl_easy_setopt(curl_handle,   CURLOPT_MUTE        ,1);
		curl_easy_setopt(curl_handle,   CURLOPT_WRITEFUNCTION,&write_data);
		headerfile = fopen(headerfilename,"w");
		if (headerfile == NULL) {
			curl_easy_cleanup(curl_handle);
			exit(0);
		}
		curl_easy_setopt(curl_handle,   CURLOPT_WRITEHEADER ,headerfile);
		curl_easy_perform(curl_handle);
		printf("The head is <%s>\n",headerfilename);
		fclose(headerfile);
		curl_easy_cleanup(curl_handle);
   //}
   exit(10);

#endif
}//_____________________________________________________________________________

#ifdef UNIX
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(unsigned int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

char* itoa64(unsigned long long value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
    
	char* ptr = result, *ptr1 = result, tmp_char;
	unsigned long long tmp_value;
	
	do {
		tmp_value = value;
		value /= base; //cout<<" tmp_value="<<tmp_value<<" base="<<base<<" value="<<value<<endl;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}


#endif

/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string itoaSTL(unsigned long long value, int base) {

	std::string buf;
#ifdef UNIX
	// check that the base if valid
	if (base < 2 || base > 16) return buf;

	enum { kMaxDigits = 65 };
	buf.reserve( kMaxDigits ); // Pre-allocate enough space.

	unsigned long long quotient = value;

	// Translating number to string with base:
	do {
		buf += "0123456789abcdef"[ quotient % base ];
		quotient /= base;
	} while ( quotient );
	
	// Append the negative sign
	if ( value < 0) buf += '-';
	
	reverse( buf.begin(), buf.end() );
 #endif
	return buf;
}




// РАСПАКОВКА на экран, представления целого числа int
// -----------------------------------------------------------------------------
void binaryPrint(unsigned int input, int radix){
	
	//cout<<"int ="<<input<<" radix="<<radix<<endl;
	
	char c[33];
	// int radix  - где radix = 2 для двоичного представления целого числа (binary number);
	//			        radix = 8 для восьмеричного... и т.д.
	char cbuf[33]; // char *string;   строка результата
	char *p;
	//DC("START binaryNumber"<<END);
	
	// Вывод на экран числа в двоичнном виде  00000000000000001111111111111111
	memset(cbuf,48,32);
	memset(c,0,33);
	cbuf[32]=0;
	//c[31]=0;
	
	itoa(input,c,2);
	
	p=cbuf+(32-strlen(c));
	//DC("//__"<<str<<END);
	memcpy(p,c,strlen(c));
	cout<<cbuf;
	//DC("print binaryNumber "<<END); DC(END);
}//_____________________________________________________________________________

void binaryPrint64(unsigned long long input, int radix){
    // функция отлажена ( октябрь 2012 )
	//cout_<<"@@@int ="<<input<<" radix="<<radix<<endl;
	
	char c[33];
	// int radix  - где radix = 2 для двоичного представления целого числа (binary number);
	//			        radix = 8 для восьмеричного... и т.д.
	char cbuf[33]; // char *string;   строка результата
	char *p;
	
	// Вывод на экран числа в двоичнном виде  00000000000000001111111111111111
	memset(cbuf,48,32);
	memset(c,0,33);
	cbuf[32]=0;
	
    // выборка по 32 разряда  из 64 на сдвигах, работает тоже
    //unsigned int input1=(unsigned int)input&0x00000000FFFFFFFF;
    //printf("%qx",input1);  cout<<endl;
    //unsigned long long input2_=input&0xFFFFFFFF00000000; input2_=input2_>>16; input2_=input2_>>16;
    //unsigned int input2 = (unsigned int)input2_;
    
    
    // выборка по 32 разряда  из 64 на указателях
    unsigned int *p1=(unsigned int *)&input;
    unsigned int *p2=p1+1;   
    
    // печать первых 32 разрядов
	itoa(*p1,c,2);

	p=cbuf+(32-strlen(c));
	memcpy(p,c,strlen(c));
	cout<<cbuf<<" + ";
    
    // печать вторых 32 разрядов
    itoa(*p2,c,2);
    
    memset(cbuf,48,32); ///////////
	p=cbuf+(32-strlen(c));
	memcpy(p,c,strlen(c));
	cout<<cbuf;

}//_____________________________________________________________________________

/**/

void memprint(const char* ptr, int size){
	char *memBlock=new char(size+1);
	memcpy(memBlock, ptr, size);
	memBlock[size]=0;
	cout<<memBlock;
	delete memBlock;
}

void error(char *msg){
    perror(msg);
    exit(1);
}

int bsearch(hashRecord *hashVector,unsigned  size,int value){
    int low = 0;
	int mid;
	int N=size;
    int high = N;
	while (low < high) {
		mid = (low + high) / 2;
		if (hashVector[mid].hash < value)
			low = mid + 1; 
		else
			//can't be high = mid-1: here A[mid] >= value,
			//so high can't be < mid if A[mid] == value
			high = mid; 
	}
	// high == low, using high or low depends on taste 
	if ((low < N) && (hashVector[low].hash == value)){
		return low ;// found
	}else{
		return -1; // not found        
    }
}//________________________________________________	

string decodeURLString(string URLstr) {
    const int len = (int)URLstr.length();
    string result;
    for(int i = 0; i < len; i++) {
		//if(URLstr[i] == '+')result += ' ';}else{   //? зачем менять плюс
        if(URLstr[i] == '%') {
			result +=
			translateHex(URLstr[i + 1]) * 16 +
			translateHex(URLstr[i + 2]);
			i += 2; // Move past hex code
		} else // An ordinary character
			result += URLstr[i];
    }
    
    return result;
}
//Decode query string from HTTP request or command line into globel variable inputData
void parseQuery(){
    string data=inputData.data["QUERY"];
    //стираем прежние данные
    inputData.data["db"]="";
    inputData.data["ocrData"]=""; 
    inputData.data["xml"]="";
    inputData.data["window"]="";
    inputData.data["ln"]="";
    if(data.find("=")==-1)return;
    
    vector <string>request=explode("&",data);
    //cout_<<"request size="<<request.size()<<endl;
    for(int a=0;a<request.size();a++){  
        if(request[a].size()>2){
            string dataStr=request[a];
            dataStr=decodeURLString(dataStr);
            string name=substr(0,(int)dataStr.find("="),dataStr);
            inputData.data[name]=dataStr;
            dataStr=substr((uint)name.size()+1,(uint)dataStr.size(),dataStr);
            //cout<<"name1="<<name<<" data="<<dataStr<<endl;
            inputData.data[name]=dataStr;
        }
    }
    if(inputData.data["xml"]!=""){
        readInput();
	}
    if(inputData.data["inputFolder"]!="")inputData.data["inputFolder"]=str_replace("\\","/",inputData.data["inputFolder"]);
    if(inputData.data["fileList"]!="")inputData.data["fileList"]=str_replace("\\","/",inputData.data["fileList"]);
    if(inputData.data["fileList"]!="")inputData.data["fileList"]=str_replace("/ "," ",inputData.data["fileList"]);
    
    if(inputData.data["fileList"]!=""){
        inputData.fileList=explode(":|:",inputData.data["fileList"]);
        //#ifdef MACOS
        //		if(inputData.data["InputMethod"]=="fileList"){
        //			for(int i=0;i<inputData.fileList.size();i++){
        //				if(inputData.fileList[i][0]!='/')inputData.fileList[i]=inputData.data["inputFolder"]+"/"+inputData.fileList[i];
        //				   //cout<<"read inputData.fileList[i]"<<inputData.fileList[i]<<END;
        //			}
        //		}
        //#endif
        inputData.data["inputFile"]=inputData.fileList[0];
        
    }
    
    //cout<<" read inputData.data[inputFile]="<<inputData.data["inputFile"]<<endl;
    
    if(inputData.data["outFolder"]=="")inputData.data["outFolder"]=inputData.data["inputFolder"];
    
    if(inputData.data["setBase"]!=""){
        inputData.data["setBase"]=str_replace("{","<",inputData.data["setBase"]);
        inputData.data["setBase"]=str_replace("}",">",inputData.data["setBase"]);
    }
    if(inputData.data["setLetter"]!=""){
        inputData.data["setLetter"]=str_replace("{","<",inputData.data["setLetter"]);
        inputData.data["setLetter"]=str_replace("}",">",inputData.data["setLetter"]);
    }
    if(inputData.data["region"]!=""){
        inputData.data["region"]=str_replace("{","<",inputData.data["region"]);
        inputData.data["region"]=str_replace("}",">",inputData.data["region"]);
    }
    
}
 // You can also traverse the whole tree (or a subtree)
 //data_traverser t;
 //doc.traverse(t);
 //or   doc.first_child().traverse(t);
void readInput(){
    const char *inputStr=inputData.data["xml"].c_str();
    
    xml_document doc,strDoc;
    xml_node mainNode;
    xml_node book, data, key;
    string str,srcLine,str_;
    inputData.start=0;
    
    if(strlen(inputStr)){
        doc.parse((char*)inputStr);
        data_traverser t;
        doc.traverse(t);
        
        
    }else{
        if(doc.load_file(inputData.data["inputPath"].c_str())){
            data_traverser t;
            doc.traverse(t);
        }else{
            cout<<inputData.data["inputPath"]<<" not loaded"<<END;
        }
    }
    //cout<<"read inputData.data[drawData]="<<inputData.data["drawData"]<<endl;
    //str_=""; writeText(str_,"input.xml");
    
}//_____________________________________________________________________________

//execute external process and read binary or text output
string run(const char* cmd){
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[262144];
    string data;
    string result;
    int dist=0;
    int size;
    //TIME_START
    while(!feof(pipe)) {
        size=(int)fread(buffer,1,262144, pipe); //cout<<buffer<<" size="<<size<<endl;
        data.resize(data.size()+size);
        memcpy(&data[dist],buffer,size);
        dist+=size;
    }
    //TIME_PRINT_
    pclose(pipe);
    return data;
}
string run(string cmd){return run(cmd.c_str());}   //execute external process

// Translate a single hex character; used by
// decodeURLString():
char translateHex(char hex) {
    if(hex >= 'A')
		return (hex & 0xdf) - 'A' + 10;
    else
		return hex - '0';
}

const char *decodeURLString(const char *URLstr){
    const int len = (uint)strlen(URLstr);
    string result;
    for(int i = 0; i < len; i++) {
		//if(URLstr[i] == '+')result += ' ';}else{   //? зачем менять плюс
        if(URLstr[i] == '%') {
			result +=
			translateHex(URLstr[i + 1]) * 16 +
			translateHex(URLstr[i + 2]);
			i += 2; // Move past hex code
		} else // An ordinary character
			result += URLstr[i];
    }
    return result.c_str();
}

string XMLEncode(string &input){
    string str=str_replace("&", "࿍", input);
    str=str_replace("<", "&lt;", str);
	str=str_replace(">", "&gt;", str);
	str=str_replace("\"", "&quot;", str);
	str=str_replace("\'", "&apos;", str);
    
    str=str_replace("", " ", str);
    str=str_replace("\n", " ", str);
    return str;

}

string XMLEncode(const char *input){
	string str=input;
	return XMLEncode(str);
}

unsigned int getID(string data){ //extract ID from xml data
    string str=decodeURLString(data);
	int pos=(int)str.find("glyph id=\""); //cout<<"pos="<<pos<<endl;
	int idNumber;
	if(pos!=-1){
		str=substr(pos+10, (int)str.find("\"",pos+11)-pos ,str);
		//cout<<"str="<<str<<endl;
	    idNumber=(uint)strtoul (str.c_str(),NULL,0);
		return idNumber;
	}
	if(pos==-1){
		int pos=(int)str.find("mask g=\""); //cout<<"pos="<<pos<<endl;
		int idNumber;
		if(pos!=-1){
			str=substr(pos+8, (int)str.find("\"",pos+9)-pos ,str);
			//cout<<"str="<<str<<endl;
			idNumber=(uint)strtoul (str.c_str(),NULL,0);
			return idNumber;
		}
	}
	return 0;

}

void printList(list<string>&list){
    std::list<string>::iterator it;
    for (it = list.begin(); it != list.end(); it++)
        cout << ", " << *it;
    cout << endl;
}

void printMap(map<string,string>&map){
    std::map<string,string>::iterator it;
    for (it = map.begin(); it != map.end(); it++)
        cout<< it->first<<" --> "<<it->second<<endl;
    cout << endl;
}
string dampMap(map<string,string>&map){
    string result;
    std::map<string,string>::iterator it;
    for (it = map.begin(); it != map.end(); it++)
        result+=it->first+":|:"+it->second+"\n";
    return result;
}


bool regExpReplace(string &src, string &exp){

    // Example: precompile pattern for faster matching:
    //    RE2 pattern("h.*o");
    //    while (ReadLine(&str)) {
    //      if (RE2::FullMatch(str, pattern)) ...;
    //    }
    
    // Example: simple search for a string:
    //      CHECK(RE2::PartialMatch("hello", "ell"));
    //
    // Example: find first number in a string
    //      int number;
    //      CHECK(RE2::PartialMatch("x*100 + 20", "(\\d+)", &number));
    //      CHECK_EQ(number, 100);
    
    // Example: read lines of the form "var = value" from a string.
    //      string contents = ...;          // Fill string somehow
    //      StringPiece input(contents);    // Wrap a StringPiece around it
    //
    //      string var;
    //      int value;
    //      while (RE2::Consume(&input, "(\\w+) = (\\d+)\n", &var, &value)) {
    //        ...;
    //      }
    //
    // Example:
    //   const RE2::Arg* args[10];
    //   int n;
    //   // ... populate args with pointers to RE2::Arg values ...
    //   // ... set n to the number of RE2::Arg objects ...
    //   bool match = RE2::FullMatchN(input, pattern, args, n);
    //
    // The last statement is equivalent to
    //
    //   bool match = RE2::FullMatch(input, pattern,
    //                               *args[0], *args[1], ..., *args[n - 1]);
    
    int print=0;
    
    vector<string>expArray=explode(":|:",exp);
    string regExp=expArray[0];
    string replaceStr;
    if(expArray.size()>1){replaceStr=expArray[1];}else{replaceStr="";}
    int n;
    if(expArray.size()>2){n=atoi(expArray[2].c_str());}else{ n=1;}
    
    StringPiece input(src);    // Wrap a StringPiece around str
    string match,match1,match2,match3,match4,match5,match6,match7,result,replaceStr1;
    
    DT(" regExp="<<regExp<<" replaceStr="<<replaceStr<<" n="<<n<<endl);
    
    //inline bool RE2::Arg::Parse(const char* str, int n) const {
    //    return (*parser_)(str, n, arg_);
    //}
    
    //bool RE2::Arg::parse_string(const char* str, int n, void* dest) {
    //    if (dest == NULL) return true;
    //    reinterpret_cast<string*>(dest)->assign(str, n);
    //    return true;
    //}
    int res=0;
    
    if(n==1){
        while(RE2::FindAndConsume(&input, regExp,&match)){  res=1; 
            DT(" match="<<match<<"/ replaceStr="<<replaceStr<<endl);
            if(match==replaceStr)continue;
            src=str_replace(match,replaceStr,src); ///break;
            //input.set(&src[5], src.size());
        };
    }    
    if(n==2){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            DT(" match="<<match<<" match1="<<match1<<"/ replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src); 
            input.set(src.c_str(), src.size());
        };
    } 
    if(n==3){        int a=0;
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2)&&a<5){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<" regExp="<<regExp<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src); 
            input.set(src.c_str(), src.size()); a++;
        };
    } 
    if(n==4){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2,&match3)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            replaceStr1=str_replace("\\3",match3,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src);
            input.set(src.c_str(), src.size());
        };
    } 
    if(n==5){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2,&match3,&match4)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            replaceStr1=str_replace("\\3",match3,replaceStr1);
            replaceStr1=str_replace("\\4",match4,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src);
            input.set(src.c_str(), src.size());
        };
    } 
    if(n==6){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2,&match3,&match4,&match5)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            replaceStr1=str_replace("\\3",match3,replaceStr1);
            replaceStr1=str_replace("\\4",match4,replaceStr1);
            replaceStr1=str_replace("\\5",match5,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src);
            input.set(src.c_str(), src.size());
        };
    } 
    if(n==7){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2,&match3,&match4,&match5,&match6)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            replaceStr1=str_replace("\\3",match3,replaceStr1);
            replaceStr1=str_replace("\\4",match4,replaceStr1);
            replaceStr1=str_replace("\\5",match5,replaceStr1);
            replaceStr1=str_replace("\\6",match6,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src);
            input.set(src.c_str(), src.size());
        };
    }
    if(n==8){        
        while(RE2::FindAndConsume(&input, regExp,&match,&match1,&match2,&match3,&match4,&match5,&match6,&match7)){ res=1;
            replaceStr1=str_replace("\\1",match1,replaceStr);
            replaceStr1=str_replace("\\2",match2,replaceStr1);
            replaceStr1=str_replace("\\3",match3,replaceStr1);
            replaceStr1=str_replace("\\4",match4,replaceStr1);
            replaceStr1=str_replace("\\5",match5,replaceStr1);
            replaceStr1=str_replace("\\6",match6,replaceStr1);
            replaceStr1=str_replace("\\6",match7,replaceStr1);
            DT(" match="<<match<<" match1="<<match1<<" match2="<<match2<<" replaceStr1="<<replaceStr1<<endl);
            if(match==replaceStr1)continue;
            src=str_replace(match,replaceStr1,src);
            input.set(src.c_str(), src.size());
        };
    }
    DT("src="<<src<<endl);
    return res;

}
bool regExpReplace(string &src, const char *exp){
    string exp_=exp;
    return regExpReplace(src,exp_);
}

bool rexExpFind(char* textBuffer, string &exp){  //search textBuffer by regular expression exp
    return RE2::PartialMatch(textBuffer, exp);
}    

string lowerCase(string&src){
    //ABCDEFGHIJKLMNOPQRSTUVWXYZ

    string srcStr=src;
    srcStr=str_replace("A","a",srcStr);
    srcStr=str_replace("B","b",srcStr);
    srcStr=str_replace("C","c",srcStr);
    srcStr=str_replace("D","d",srcStr);
    srcStr=str_replace("E","e",srcStr);
    srcStr=str_replace("F","f",srcStr);
    srcStr=str_replace("G","g",srcStr);
    srcStr=str_replace("H","a",srcStr);
    srcStr=str_replace("I","i",srcStr);
    srcStr=str_replace("J","j",srcStr);
    srcStr=str_replace("K","k",srcStr);
    srcStr=str_replace("L","l",srcStr);
    srcStr=str_replace("M","m",srcStr);
    srcStr=str_replace("N","n",srcStr);
    srcStr=str_replace("O","o",srcStr);
    srcStr=str_replace("P","p",srcStr);
    srcStr=str_replace("Q","q",srcStr);
    srcStr=str_replace("R","r",srcStr);
    srcStr=str_replace("S","s",srcStr);
    srcStr=str_replace("T","t",srcStr);
    srcStr=str_replace("U","u",srcStr);
    srcStr=str_replace("V","v",srcStr);
    srcStr=str_replace("W","w",srcStr);
    srcStr=str_replace("X","x",srcStr);
    srcStr=str_replace("Y","y",srcStr);
    srcStr=str_replace("Ā","ā",srcStr);
    srcStr=str_replace("Ē","ē",srcStr);
    srcStr=str_replace("Ī","ī",srcStr);
    srcStr=str_replace("Ō","ō",srcStr);
    srcStr=str_replace("Ū","ū",srcStr);
    srcStr=str_replace("Ñ","ñ",srcStr);
 
    return srcStr;
}

bool isRegExp(string&src){
    
    if(src.find('"')!=-1)return 1;
    //if(RE2::PartialMatch(src,"[ ŋ\\-<>\\[\\]\\(\\)\\{\\}\\.\\d\\?\\!\\=\\t\\n\\r«»áäéóöúüēāĀĒĪŌōŪūĨĩùÙÀàñÑīĪèɛəɔƆüöǖǜȫȫì_~̲̀,']"))return 1; 
    if(RE2::PartialMatch(src,"[^a-zA-Z]"))return 1; 
    return 0;

}

string startSVG(){
    
    string str="<html>\
    <body>\
    <h1>My first SVG</h1>\
    <svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
    return str;
}
string closeSVG(){
    
    string str="</svg></body></html>";
    return str;
}

//сортировка строк по длинне
bool dictKey_sort(const dictKey d1, const dictKey d2){
    return d1.key.size() > d2.key.size();
}


string clearText(string str){
    string str_=str;
    string reStr;
    //cout<<str<<endl;
    /*reStr="([^\\p{Tibetan}]):|:་:|:1";
    //reStr="([\\p{Latin}\\p{Cyrillic}\\d\\-\\=\\[\\]\\!\\#\\$\\%\\^\\&\\*\\(\\)\\_\\+]+):|:་:|:1";
    regExpReplace(str_,reStr);
    //reStr="([༡༢༣༤༥༦༧༨༩༠༼༽༐༒༴༌།༑༈༏༔༈༄༅]):|:་:|:1";
    regExpReplace(str_,reStr);
    */
    //reStr="(([༡༢༣༤༥༦༧༨༩༠༼༽༐༒༴༑༈༏༔༈༄༅])):|:\\1་:|:2";
    //regExpReplace(str_,reStr);
    string delimeter;delimeter+=(char)0xC2; delimeter+=(char)0xA0; //no-break space
    str_=str_replace(delimeter.c_str()," ",str_);
    str_=str_replace("  ", " ",str_);
    str_=str_replace("་ ", "་",str_);
    str_=str_replace("\r", "\n",str_);
    str_=str_replace("\n\n", "\n",str_);
    str_=str_replace("་\n", "་",str_);
    str_=str_replace("།", "། ",str_);
    str_=str_replace("\n།", "།",str_);
    str_=str_replace("  ", " ",str_);
    str_=str_replace("། ། ", "། །",str_);
    str_=str_replace("\n།\n", "།\n",str_);
    str_=str_replace(" །\n", "།\n",str_);
    str_=str_replace("ག ། ", "ག །",str_);
    str_=str_replace(" ེ", "ེ",str_);
    str_=str_replace(" ོ", "ོ",str_);
    str_=str_replace("] ", "]",str_);

    //textData=str_replace(" ། ", "། ",textData);
    reStr="(([\\p{Cyrillic}\\p{Latin}\\d\\.,:;\\!\\-–]) ):|:\\1@:|:2";
    regExpReplace(str_,reStr);
    
    reStr="<[^>]*>";
    std::regex key_regex(reStr);
    str_ = std::regex_replace(str_, key_regex, "");
    reStr="[ _\\d	\\*\(\\)\\{\\}\\[\\]@//\\%\\&]";
    std::regex key_regex_(reStr);
    str_ = std::regex_replace(str_, key_regex_, "་");

    str_=str_replace("\n"," ",str_);
    str_=str_replace("\r"," ",str_);
    str_=str_replace(",","@",str_);
    str_=str_replace(".","@",str_);
    str_=str_replace("/","@",str_);
    str_=str_replace("′","",str_);
    str_=str_replace("ō","o",str_);
    str_=str_replace("ē","e",str_);
    str_="✦"+str_;
    str_=str_replace("✦། ","།",str_);
    str_=str_replace("✦","",str_);
    str_=str_replace("༡", "་",str_);
    str_=str_replace("༢", "་",str_);
    str_=str_replace("༣", "་",str_);
    str_=str_replace("༤", "་",str_);
    str_=str_replace("༥", "་",str_);
    str_=str_replace("༦", "་",str_);
    str_=str_replace("༧", "་",str_);
    str_=str_replace("༨", "་",str_);
    str_=str_replace("༩", "་",str_);
    str_=str_replace("༠", "་",str_);
    str_=str_replace("༔", "་",str_);
    str_=str_replace("༴", "་",str_);
    str_=str_replace("།", "་",str_);
    str_=str_replace("༑", "་",str_);
    str_=str_replace("༐", "་",str_);
    str_=str_replace("་་", "་",str_);
    str_=str_replace("་་", "་",str_);
    str_=str_replace("་་", "་",str_);

    //cout<<" @@@@"<<str_<<endl;
     
    return str_;
};

int max(int &a,int &b){if(a>b)return a; return b;}

#ifdef OCRLib
int indexById(vector<stringOCR>&correctionTable,unsigned int in){
    for(int i=0;i<correctionTable.size();i++){
        if(correctionTable[i].idNumber==in)return i;
    }
    return -1;
    
};


//строковые функции тибетского языка
bool hasTail(OCRMatch&match){
    if( match.name=="ད"||match.name=="ན"||match.name=="ག"||
        match.name=="ཀ"||match.name=="ཁ"||match.name=="ཞ"||
        match.name=="ཧ"||match.name=="ཏ"||match.name=="ཤ"||
        match.name=="ཊ"||match.name=="ཌ"||match.name=="ཎ"||
        match.name=="ཥ"||match.name=="༡"

       )return 1; 
    
       //нга нельзя добавлять, иначе не убирает ра-таги

    return 0;
}
#endif

string lastOf(string &name){
    if(name.size()<3)return "";
    string str=substr((int)name.size()-3, 3, name);
    //cout<<"lastOf="<<str<<endl;
    return str;
}

bool isTibDigit(string &name){
    
     if(name=="༠"||
        name=="༡"||
        name=="༢"||
        name=="༣"||
        name=="༤"||
        name=="༥"||
        name=="༦"||
        name=="༧"||
        name=="༨"||
        name=="༩"
        )return 1;

    return 0;
}



