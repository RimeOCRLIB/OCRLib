/////////////////////////////////////////////////////////////////////////////
//Impuls filter is algorithm for filtering noise from signal. Filter is siquensce of  7,6...2 digits
//Algorithm compare filter with signal's string is Dhonald E. Knuth algorithm for seeking
//substriing in a string.
//http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/index.html
//www.buddhism.ru 2006
/////////////////////////////////////////////////////////////////////////////

#include "GBitsetContour.h"

int GBitsetContour::impFilter7(char *filterStr, char *aFt, short angLen){
short mainIndex;
char sum;
int filterCount=0;

for ( mainIndex = 3; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
		if(aFt[mainIndex]!=filterStr[0])continue;
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++; continue;}
		if(aFt[mainIndex+2]!=filterStr[2]){mainIndex=mainIndex+2; continue;}
		if(aFt[mainIndex+3]!=filterStr[3]){mainIndex=mainIndex+3; continue;}	
		if(aFt[mainIndex+4]!=filterStr[4]){mainIndex=mainIndex+4; continue;}	
		if(aFt[mainIndex+5]!=filterStr[5]){mainIndex=mainIndex+5; continue;}	
		if(aFt[mainIndex+6]!=filterStr[6]){mainIndex=mainIndex+6; continue;}	
                //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;

		sum=0; filterCount++;
				        sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
				        sum=sum+aFt[mainIndex+2]; aFt[mainIndex+2]=0; //actual filter action
				        sum=sum+aFt[mainIndex+3]; aFt[mainIndex+3]=0; //actual filter action
				        sum=sum+aFt[mainIndex+4]; aFt[mainIndex+4]=0; //actual filter action
				        sum=sum+aFt[mainIndex+5]; aFt[mainIndex+5]=0; //actual filter action
				        sum=sum+aFt[mainIndex+6]; aFt[mainIndex+6]=0; //actual filter actio
		aFt[mainIndex+3]=sum; //actual filter action
}//end main for
return filterCount;
}//////////////////////////////////////////////////////////////////////////**

///////////////////////////////////////////////////////////**filter 6//////*//////////////////////////////////////
int GBitsetContour::impFilter6(char *filterStr, char *aFt, short angLen){

short mainIndex;
char sum=0;
int filterCount=0;

for ( mainIndex = 4; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
		if(aFt[mainIndex]!=filterStr[0])continue;
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++; continue;}
		if(aFt[mainIndex+2]!=filterStr[2]){mainIndex=mainIndex+2; continue;}
		if(aFt[mainIndex+3]!=filterStr[3]){mainIndex=mainIndex+3; continue;}	
		if(aFt[mainIndex+4]!=filterStr[4]){mainIndex=mainIndex+4; continue;}	
		if(aFt[mainIndex+5]!=filterStr[5]){mainIndex=mainIndex+5; continue;}	
                //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;
					sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
				        sum=sum+aFt[mainIndex+2]; aFt[mainIndex+2]=0; //actual filter action
				        sum=sum+aFt[mainIndex+3]; aFt[mainIndex+3]=0; //actual filter action
				        sum=sum+aFt[mainIndex+4]; aFt[mainIndex+4]=0; //actual filter action
				        sum=sum+aFt[mainIndex+5]; aFt[mainIndex+5]=0; //actual filter action
		aFt[mainIndex+3]=sum; //actual filter action
}//end main for
return filterCount;
}//////////////////////////////////////////////////////////////////////////**

///////////////////////////////////////////////////////////**filter 5//////*//////////////////////////////////////
int GBitsetContour::impFilter5(char *filterStr, char *aFt, short angLen){

short mainIndex;
char sum;
int filterCount=0;

for ( mainIndex = 5; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
		if(aFt[mainIndex]!=filterStr[0])continue;
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++; continue;}
		if(aFt[mainIndex+2]!=filterStr[2]){mainIndex=mainIndex+2; continue;}
		if(aFt[mainIndex+3]!=filterStr[3]){mainIndex=mainIndex+3;continue;}	
		if(aFt[mainIndex+4]!=filterStr[4]){mainIndex=mainIndex+4;continue;}	
                //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;

		sum=0; 
				        sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
				        sum=sum+aFt[mainIndex+2]; aFt[mainIndex+2]=0; //actual filter action
				        sum=sum+aFt[mainIndex+3]; aFt[mainIndex+3]=0; //actual filter action
				        sum=sum+aFt[mainIndex+4]; aFt[mainIndex+4]=0; //actual filter action
		aFt[mainIndex+2]=sum; //actual filter action
}//end main for

return filterCount;
}//////////////////////////////////////////////////////////////////////////**

///////////////////////////////////////////////////////////**filter 4//////*//////////////////////////////////////
int GBitsetContour::impFilter4(char *filterStr, char *aFt, short angLen){

short mainIndex;
char sum;
int filterCount=0;

for ( mainIndex = 6; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
		if(aFt[mainIndex]!=filterStr[0])continue; 
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++; continue;}
		if(aFt[mainIndex+2]!=filterStr[2]){mainIndex=mainIndex+1; continue;}
		if(aFt[mainIndex+3]!=filterStr[3]){mainIndex=mainIndex+2; continue;}	
                //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;

		sum=0; 
				        sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
				        sum=sum+aFt[mainIndex+2]; aFt[mainIndex+2]=0; //actual filter action
				        sum=sum+aFt[mainIndex+3]; aFt[mainIndex+3]=0; //actual filter action
		aFt[mainIndex+2]=sum; //actual filter action
}//end main for
return filterCount;

}//////////////////////////////////////////////////////////////////////////**

///////////////////////////////////////////////////////////**filter 3//////*//////////////////////////////////////
int GBitsetContour::impFilter3(char *filterStr, char *aFt, short angLen){

short mainIndex;
char sum;
int filterCount=0;

for ( mainIndex = 7; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
		if(aFt[mainIndex]!=filterStr[0])continue;
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++;continue;}
		if(aFt[mainIndex+2]!=filterStr[2]){mainIndex=mainIndex+2;continue;}
                //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<"filterStr["<<filterIndex<<"]="<<(short)filterStr[filterIndex]<<" "<<(short)count<<endl;

		sum=0; 
					sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
				        sum=sum+aFt[mainIndex+2]; aFt[mainIndex+2]=0; //actual filter action
		aFt[mainIndex+1]=sum; //actual filter action
}//end main for
return filterCount;

}//////////////////////////////////////////////////////////////////////////**

///////////////////////////////////////////////////////////**filter 2//////*//////////////////////////////////////
int GBitsetContour::impFilter2(char *filterStr, char *aFt, short angLen){

short mainIndex;
char sum;
int filterCount=0;

for ( mainIndex = 8; mainIndex <angLen+10; mainIndex++ ) {//make new  differential Array for filtering

//for each array member we check filter
//cout<<"aFt["<<mainIndex<<"] "<<(short)aFt[mainIndex]<<endl;
		if(aFt[mainIndex]!=filterStr[0])continue;
		if(aFt[mainIndex+1]!=filterStr[1]){mainIndex++; continue;}

		sum=0;
				        sum=sum+aFt[mainIndex]; aFt[mainIndex]=0; //actual filter action
				        sum=sum+aFt[mainIndex+1]; aFt[mainIndex+1]=0; //actual filter action
		aFt[mainIndex+1]=sum; //actual filter action
}//end main for
return filterCount;
}//////////////////////////////////////////////////////////////////////////**









