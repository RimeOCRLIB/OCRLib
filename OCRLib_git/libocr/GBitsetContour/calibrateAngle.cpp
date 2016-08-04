#include "GBitsetContour.h"

void GBitsetContour::calibrateAngle(){  //main point for recognise Letter up corner and set center mass
resX=0; resY=0; 
short x,y;
//short upLimit=70;
//upCorner=10000;

	for(i=0;i<angLength;i++){
	x=angX[i]; //cout<<(short)angX[i]<<" ";
	y=angY[i];
	resX=resX+x; resY=resY+y;   
	}

	//cout<<endl;
	XMCenter=resX/angLength; //cout<<"XMCenter="<<XMCenter;  //set center mass of x
        YMCenter=resY/angLength; //cout<<"YMCenter="<<YMCenter<<endl;  //set center mass of y

}////////////////////////////////////////////////////////////////////////////////////////////**
