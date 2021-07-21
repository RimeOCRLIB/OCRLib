#include "GBitsetContour.h"

void rootFilter(char *aFt, short angLen){

short i, index, testIndex;
char sum, filterLen; 

filterLen=3;
//if(angLen<30)filterLen=3;

for ( i = 0; i <angLen; i=i+filterLen ) { //write important points to array
sum=0;
for (index=0;index<filterLen;index++){
		testIndex=i+index;
		if(testIndex>=angLen){testIndex=testIndex-angLen;} //now testIndex contain valid adress
		sum=sum+aFt[testIndex];
		aFt[testIndex]=0;

}
		testIndex=i+filterLen/2;
		if(testIndex>=angLen){testIndex=testIndex-angLen;} //now testIndex contain valid adress
		aFt[testIndex]=sum;  //cout<<"aFt["<<testIndex<<"]="<<(short)aFt[testIndex]<<" sum"<<(short)sum<<endl;
//cout<<(short)aFt[i]<<" ";

}//end for

//for ( i = 0; i <angLen; i++ ) //cout<<"aFtResult["<<i<<"]="<<(short)aFt[i]<<endl;

}////////////////////////////////////////////////////////**
