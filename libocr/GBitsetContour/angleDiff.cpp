#include "GBitsetContour.h"

void GBitsetContour::angleDiff(){
	short x,y; //cout<<"@@@";

	x=ang[0];
	
	for (i=0;i<angLength-1;i++){ //cout<<"ang["<<i<<"]"<<(short)ang[i]<<" ";
		x=ang[i]; y=ang[i+1]; angDiff[i]=0;
		if(x==y)	angDiff[i]=0;   
		if(x==0){	if(y==4)angDiff[i]=4;		if(y==3)angDiff[i]=3;		if(y==2)angDiff[i]=2;		if(y==1)angDiff[i]=1;
		if(y==7)angDiff[i]=-1;	if(y==6)angDiff[i]=-2;	if(y==5)angDiff[i]=-3;}
		if(x==1){	if(y==5)angDiff[i]=4;		if(y==4)angDiff[i]=3;		if(y==3)angDiff[i]=2;		if(y==2)angDiff[i]=1;
		if(y==0)angDiff[i]=-1;	if(y==7)angDiff[i]=-2;	if(y==6)angDiff[i]=-3;}
		if(x==2){	if(y==6)angDiff[i]=4;		if(y==5)angDiff[i]=3;		if(y==4)angDiff[i]=2;		if(y==3)angDiff[i]=1;
		if(y==1)angDiff[i]=-1;	if(y==0)angDiff[i]=-2;	if(y==7)angDiff[i]=-3;}
		if(x==3){	if(y==7)angDiff[i]=4;		if(y==6)angDiff[i]=3;		if(y==5)angDiff[i]=2;		if(y==4)angDiff[i]=1;
		if(y==2)angDiff[i]=-1;	if(y==1)angDiff[i]=-2;	if(y==0)angDiff[i]=-3;}
		if(x==4){	if(y==0)angDiff[i]=4;		if(y==7)angDiff[i]=3;		if(y==6)angDiff[i]=2;		if(y==5)angDiff[i]=1;
		if(y==3)angDiff[i]=-1;	if(y==2)angDiff[i]=-2;	if(y==1)angDiff[i]=-3;}
		if(x==5){	if(y==1)angDiff[i]=4;		if(y==0)angDiff[i]=3;		if(y==7)angDiff[i]=2;		if(y==6)angDiff[i]=1;
		if(y==4)angDiff[i]=-1;	if(y==3)angDiff[i]=-2;	if(y==2)angDiff[i]=-3;}
		if(x==6){	if(y==2)angDiff[i]=4;		if(y==1)angDiff[i]=3;		if(y==0)angDiff[i]=2;		if(y==7)angDiff[i]=1;
		if(y==5)angDiff[i]=-1;	if(y==4)angDiff[i]=-2;	if(y==3)angDiff[i]=-3;}
		if(x==7){	if(y==3)angDiff[i]=4;		if(y==2)angDiff[i]=3;		if(y==1)angDiff[i]=2;		if(y==0)angDiff[i]=1;
		if(y==6)angDiff[i]=-1;	if(y==5)angDiff[i]=-2;	if(y==4)angDiff[i]=-3;}
		
	//cout<<"angDiff1["<<i<<"]"<<(short)angDiff[i]<<endl;
	}
	//now we set end point value
	i=angLength-1;
	x=ang[i]; y=ang[0]; angDiff[i]=0;
	if(x==y)	angDiff[i]=0;   
	if(x==0){	if(y==4)angDiff[i]=4;		if(y==3)angDiff[i]=3;		if(y==2)angDiff[i]=2;		if(y==1)angDiff[i]=1;
	if(y==7)angDiff[i]=-1;	if(y==6)angDiff[i]=-2;	if(y==5)angDiff[i]=-3;}
	if(x==1){	if(y==5)angDiff[i]=4;		if(y==4)angDiff[i]=3;		if(y==3)angDiff[i]=2;		if(y==2)angDiff[i]=1;
	if(y==0)angDiff[i]=-1;	if(y==7)angDiff[i]=-2;	if(y==6)angDiff[i]=-3;}
	if(x==2){	if(y==6)angDiff[i]=4;		if(y==5)angDiff[i]=3;		if(y==4)angDiff[i]=2;		if(y==3)angDiff[i]=1;
	if(y==1)angDiff[i]=-1;	if(y==0)angDiff[i]=-2;	if(y==7)angDiff[i]=-3;}
	if(x==3){	if(y==7)angDiff[i]=4;		if(y==6)angDiff[i]=3;		if(y==5)angDiff[i]=2;		if(y==4)angDiff[i]=1;
	if(y==2)angDiff[i]=-1;	if(y==1)angDiff[i]=-2;	if(y==0)angDiff[i]=-3;}
	if(x==4){	if(y==0)angDiff[i]=4;		if(y==7)angDiff[i]=3;		if(y==6)angDiff[i]=2;		if(y==5)angDiff[i]=1;
	if(y==3)angDiff[i]=-1;	if(y==2)angDiff[i]=-2;	if(y==1)angDiff[i]=-3;}
	if(x==5){	if(y==1)angDiff[i]=4;		if(y==0)angDiff[i]=3;		if(y==7)angDiff[i]=2;		if(y==6)angDiff[i]=1;
	if(y==4)angDiff[i]=-1;	if(y==3)angDiff[i]=-2;	if(y==2)angDiff[i]=-3;}
	if(x==6){	if(y==2)angDiff[i]=4;		if(y==1)angDiff[i]=3;		if(y==0)angDiff[i]=2;		if(y==7)angDiff[i]=1;
	if(y==5)angDiff[i]=-1;	if(y==4)angDiff[i]=-2;	if(y==3)angDiff[i]=-3;}
	if(x==7){	if(y==3)angDiff[i]=4;		if(y==2)angDiff[i]=3;		if(y==1)angDiff[i]=2;		if(y==0)angDiff[i]=1;
	if(y==6)angDiff[i]=-1;	if(y==5)angDiff[i]=-2;	if(y==4)angDiff[i]=-3;}
	//cout<<"angDiff1["<<i<<"]"<<(short)angDiff[i]<<endl;
	
	
	///////////////////////start make array with integral of Letter contour value (Important!)
	startDir=ang[0];  //save start Direction
	
	//cout<<endl<<"angLength"<<angLength;
}//end method///////////////////////////////////
