#include "GBitsetContour.h"

void GBitsetContour::baseFilter() {
	short resInt, resIntSum;
	//short angLength=ang.size();
       //	angInt.resize(angLength); angFilterBase.resize(angLength+20); angDiffSum.resize(angLength);
	
	resInt=0; resIntSum=0;

	short ind;  
	ind=10;
	for ( i = 0; i <angLength; i++ ) {//make new  differential Array for filtering
	   angFilterBase[ind]=angDiff[i]; //cout<<"angFilterBase["<<ind<<"]="<<(short)angFilterBase[ind]<<" angDiff["<<i<<"]="<<(short)angDiff[i]<<endl;
	   ind++;
	}
	//cout<<"#2";
	
	if(angLength>10){
		ind=0;
		for ( i = 0; i <10; i++ ) {//make new  differential Array for filtering
		angFilterBase[ind]=angDiff[angLength-10+i];ind++;}
		ind=angLength+10;
		for ( i = 0; i <10; i++ ) {//make new  differential Array for filtering
		angFilterBase[ind]=angDiff[i];ind++;}
	}else{
		ind=0;
		for ( i = 0; i <10; i++ ) {//make new  differential Array for filtering
		angFilterBase[ind]=0;ind++;}
		ind=angLength+10;
		for ( i = 0; i <10; i++ ) {//make new  differential Array for filtering
		angFilterBase[ind]=0;ind++;}
	}
	//_//_DC("#3");
	//for ( m = 0;m <1000; m++ ) {//test of performance
	impulseFilter(angFilterBase);
	//}
	//_//_DC("#4="<<angLength<<" ");
	   ind=10;
	//cout<<"#5 angLength="<<angLength<<endl;


	for ( i = 0; i <angLength; i++ ) { //cout<<"i="<<i<<endl;//3-point
		//if(ind<angFilterBase.size())angDiffSum.push_back(angFilterBase[ind]);  //cout<<"angDiffSum["<<i<<"]="<<(short)angDiffSum[i]<<endl;
		angDiffSum[i]=angFilterBase[ind];
		//_//_DC("angDiffSum["<<i<<"]="<<(short)angFilterBase[ind]<<END);
	    ind++;
	}

	//_//_DC("#6");
//	resInt=0;
//	for (i=0;i<angLength;i++){
//		resInt=resInt+angDiffSum[i];
//		angIntSum.push_back(resInt); //cout<<(short)angIntSum[i]<<" ";
//	}
	
}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



