#include "GBitsetContour.h"

int impFilter(std::string *srcStr, char *aFt, short angLen){

string str=*srcStr;
char filterLength=str.length();
char filterLen=0;
char filterStr[filterLength];
char strLetter;
short indexLetter=0;
short index=0;
short mainIndex, testIndex, filterIndex;
char count=0;
char sum;
int filterCount=0;
char sign=1;

//convert filter record in array
//for(index=0;index<filterLength;index++)cout<<str[index];
//cout<<endl;
//index=0;

while(index<filterLength){
strLetter=str[index]; //cout<<(short)strLetter-48<<" ";
if(strLetter=='-'){sign=-1;}
if(strLetter!=' '&&strLetter!='-'){filterStr[indexLetter]=(strLetter-48)*sign; sign=1;filterLen++;indexLetter++;}
index++;
}
//cout<<"filterLen"<<(short)filterLen<<endl;
//for(index=0;index<filterLen;index++)  //cout<<(short)filterStr[index]<<" ";
//cout<<endl;
//index=0;

//cout<<"angLen="<<(short)angLen<<endl;
for ( mainIndex = -filterLen; mainIndex <angLen+filterLen; mainIndex++ ) {//make new  differential Array for filtering
count=0; index=0;
//for each array member we check filter
		for (filterIndex= 0; filterIndex <filterLen; filterIndex++ ){
		testIndex=mainIndex+filterIndex;
		if(testIndex<0){testIndex=angLen+testIndex;}
		if(testIndex>=angLen){testIndex=testIndex-angLen;} //now testIndex contain valid adress
		if(aFt[testIndex]==filterStr[filterIndex]){count++;//cout<<(short)count<<endl;	
		} 
		//cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;
		} 
	
			if(count==filterLen){ //cout<<"signal found "<<(short)mainIndex<<endl;  //signal found
			sum=0; filterCount++;
				for (index=mainIndex;index<mainIndex+filterLen;index++){
                                testIndex=index;
					if(testIndex<0){testIndex=angLen+testIndex;}
					if(testIndex>=angLen){testIndex=testIndex-angLen;} //now testIndex contain valid adress
				sum=sum+aFt[testIndex];			
				aFt[testIndex]=0; //actual filter action
				}
				testIndex=mainIndex+filterLen/2;
					if(testIndex<0){testIndex=angLen+testIndex;}
					if(testIndex>=angLen){testIndex=testIndex-angLen;} //now testIndex contain valid adress
				aFt[testIndex]=sum; //actual filter action
				
			}//end if

}//end main for

//for (index=0;index<angLen;index++)cout<<(short)aFt[index]<<" ";

return filterCount;
}////////////////////////////////////////////////////////////////////










