#include "GBitsetOCR.h"

short GBitsetOCR::setScaleRecord(){
/*
short i,index,letterIndex, cX, cY, lengthSum=0;
short vectXStart,vectXEnd,vectYStart,vectYEnd;
int kH, kW;			
unsigned long length;
float k1,k2,k3;
int print=0;

//DP("glyph[0].letterH<<"<<glyph[0].letterH<<" wordSrc[0].h"<<wordSrc[0].h<<END);
kH=100;//5500/wordSrc[0].h;
kW=100;//glyph[0].letterW*100/wordSrc[0].w;

//DP("kH="<<kH<<" kW="<<kW<<END);
	k1=(float)wordSrc[0].w/wordSrc[0].h;
	k2=(float)glyph[0].letterW/glyph[0].letterH;
	k3=fabs(k1-k2);
//DP("kH="<<kH<<" kW="<<kW<<" k1="<<k1<<" k2="<<k2<<" k3="<<k3<<
//" glyph[0].letterW="<<glyph[0].letterW<<
//" glyph[0].letterH="<<glyph[0].letterH<<
//" glyph[0].lengthSum="<<glyph[0].lengthSum<<END);
//if(kH>1000||kH<10||kW>1000||kW<10) return 0;  //no match


		if(k3>0.6){gSrc.lengthSum=0;return 0;}

    gSrc.vectorCount=0;
    gSrc.lengthSum=0;
	for(int vectorInd=0;vectorInd<glyphTest.vectorCount;vectorInd++){
		gSrc.v[vectorInd]=glyphTest.v[vectorInd];

	}
        gSrc.vectorCount=glyphTest.vectorCount;
        gSrc.v[0].allVectorCount= gSrc.vectorCount;

	//DC("@1");
  	    //gSrc.center=wordSrc[0].center;
	    gSrc.letterW=wordSrc[0].w;
	    gSrc.letterH=wordSrc[0].h;
	    //gSrc.vectorLimit=(gSrc.letterW+gSrc.letterW+gSrc.letterH)/10;
		gSrc.recordStatus=1; //Flag for record validation
		gSrc.recordFlag=0; //Flag for grammar recognition


cX=0;  // *10 for pixel independed value
cY=0;  // *10 for pixel independed value



for (index=0;index<gSrc.vectorCount;index++){   //(vectXStart+vectXEnd)/2
vectXStart=((gSrc.v[index].vStart.x*10-wordSrc[0].center.x*10)*kW)/1000+cX;
vectYStart=((gSrc.v[index].vStart.y*10-wordSrc[0].center.y*10)*kH)/1000+cY;
vectXEnd=((gSrc.v[index].vEnd.x*10-wordSrc[0].center.x*10)*kW)/1000+cX;
vectYEnd=((gSrc.v[index].vEnd.y*10-wordSrc[0].center.y*10)*kH)/1000+cY;

length=((vectXStart-vectXEnd)*(vectXStart-vectXEnd)+(vectYStart-vectYEnd)*(vectYStart-vectYEnd))*100; //attention. length*10 increase Accuracy of short
//length=((vectXStart-vectXEnd)*(vectXStart-vectXEnd)+(vectYStart-vectYEnd)*(vectYStart-vectYEnd))*100;
	if(length>0)length=(short)sqrt((float)length);  //attention. length*100 increase Accuracy of short
	//DC(" length "<<index<<"="<<length<<"gSrc.v["<<index<<"].vStart.x="<<gSrc.v[index].vStart.x<<END);
			gSrc.v[index].vStart.x=vectXStart; ////DC(" x="<<vectXStart);
			gSrc.v[index].vStart.y=vectYStart; ////DC(" y="<<vectYStart<<END);
			gSrc.v[index].vEnd.x=vectXEnd;
			gSrc.v[index].vEnd.y=vectYEnd;
			gSrc.v[index].length=length;
			gSrc.v[index].vectorFlag=0;
			gSrc.v[index].matchSum=0;
			gSrc.v[index].vectorAngle=gSrc.v[index].vectorAngle;
			gSrc.v[index].vCenter.x=(gSrc.v[index].vStart.x+gSrc.v[index].vEnd.x)/2;
	                gSrc.v[index].vCenter.y=(gSrc.v[index].vStart.y+gSrc.v[index].vEnd.y)/2; //DC("BASE y="<<gSrc.v[index].vCenter.y<<END);
			gSrc.lengthSum+=length;

}//for (i=0;i<letterModelScale[record].letterSetCount;

//DP(" gSrc.lengthSum="<<gSrc.lengthSum<<END);

return gSrc.lengthSum;
*/ 
	 return 0;	 
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



















