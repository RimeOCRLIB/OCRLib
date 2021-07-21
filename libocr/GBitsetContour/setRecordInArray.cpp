#include "GBitsetContour.h"


//in this method we receive the data about vector, test it and if vector is valid, write it in base.

void GBitsetContour::setRecordInArray(char contour, short vCount){
	int vIndex=vectorBase[0].allVectorCount;
	//@DC("//________________new vector "<<vCount<<" contourIndex"<<contourIndex<<END);
	short vectXStart,vectXEnd,vectYStart,vectYEnd,vEndIndex;
	unsigned long length;
	short angLimit=0;
	unsigned int letterIndex=vectorBase[0].allVectorCount;
	
	
	vectXStart=angX[vectorMarker[vCount-1]];	//DP("vectXStart1="<<vectXStart<<END);
	vectYStart=angY[vectorMarker[vCount-1]];	//DP("vectYStart1="<<vectYStart<<END);
	vectXEnd=angX[vectorMarker[vCount]];		//DP("vectXEnd1="<<vectXEnd<<END);
	vectYEnd=angY[vectorMarker[vCount]];		//DP("vectYEnd1="<<vectYEnd<<END);
	vEndIndex=vectorMarker[vCount];			    //DP("vCount="<<vCount<<"/"<<vectorMarker[vCount-1]<<END);

	if(vectXStart>xMax)xMax=vectXStart;if(vectXStart<xMin)xMin=vectXStart;if(vectYStart>yMax)yMax=vectYStart;if(vectYStart<yMin)yMin=vectYStart;
        length=((vectXStart-vectXEnd)*(vectXStart-vectXEnd)+(vectYStart-vectYEnd)*(vectYStart-vectYEnd))*100;
        if(length>0)length=(short)sqrtA((int)length); //attention. length*10 increase Accuracy of short

	//DP(" length="<<length<<" vectorLimit="<<vectorLimit<<" vIndex="<<vIndex<<" angLength="<<angLength<<END);

	short angle=setVectorAngle(vectXStart, vectYStart, vectXEnd, vectYEnd); //@DC("angle="<< angle<<END);	
	
	if(vIndex>0){
		angLimit=abs(vectorBase[vIndex-1].vectorAngle-angle);
		if(angLimit>280)angLimit=360-angLimit;
	}
	if(vectorMarker[vCount]==angLength-1&&length<vectorLimit)angLimit=90; //if end vector is small
	if(vCount==1)angLimit=90;  //if first vector
	//DP(" angLimit="<<angLimit<<" vectorCount="<<vectorCount<<" vIndex="<<vIndex<<" vectorBase="<<vectorBase[0].allVectorCount<<END);
   	
 if(length>vectorLimit||vectorMarker[vCount]==angLength-1||vCount==1||angLength<sizeH){
		/////////////////////////now we start check angle between this vector and previous vector
					//vectorOCR vectBufer; //_DC(".");
                                        //vectorBase->push_back(vectBufer);   //_DC("|");
                                        //if(vectBufer==NULL)return;
					//vectorBase[vIndex]=vectBufer;
					//DP("set1");
					vectorBase[vIndex].vStart.x=vectXStart; //DT(" sX "<<vectXStart);
					vectorBase[vIndex].vStart.y=vectYStart; //DT(" sY "<<vectYStart);
					vectorBase[vIndex].vEnd.x=vectXEnd;     //DT(" eX "<<vectXEnd);
					vectorBase[vIndex].vEnd.y=vectYEnd;     //DT(" eY "<<vectYEnd);
					vectorBase[vIndex].length=length;
					vectorBase[vIndex].vectorFlag=0;
					vectorBase[vIndex].selectFlag=0;
					vectorBase[vIndex].matchSum=0;
					vectorBase[vIndex].vectorAngle=angle;   //DP("vectorBase["<<vIndex<<"].vectorAngle="<<vectorBase[vIndex].vectorAngle<<END);
					vectorBase[vIndex].vCenter.x=(vectXStart+vectXEnd)/2;  //_//_DC(" x="<<vectorBase[contourIndex].v[vIndex].vCenter.x;
					vectorBase[vIndex].vCenter.y=(vectYStart+vectYEnd)/2;  //_//_DC(" y="<<vectorBase[contourIndex].v[vIndex].vCenter.y<<END);
					vectorBase[vIndex].contourIndex=contourIndex;
					vectorBase[vIndex].letterIndex=letterIndex;
					vectorCount++;
					vectorBase[0].allVectorCount++;
 }else{//if(length>vectorLimitNormal||angLength<65||vectorMarker[vCount]==angLength-1){

	  if(angLimit>37){
		  //vectorOCR vectBufer; //_DC(".");
                  //vectorBase->push_back(vectBufer)//_DC("|");
		  //if(vectBufer==NULL)return;
		  //vectorBase[vIndex]=vectBufer;
		  //DP("set2");
		  vectorBase[vIndex].vStart.x=vectXStart; //DT(" sX "<<vectXStart);
		  vectorBase[vIndex].vStart.y=vectYStart; //DT(" sY "<<vectYStart);
		  vectorBase[vIndex].vEnd.x=vectXEnd;     //DT(" eX "<<vectXEnd);
		  vectorBase[vIndex].vEnd.y=vectYEnd;     //DT(" eY "<<vectYEnd);
		  vectorBase[vIndex].length=length;
		  vectorBase[vIndex].vectorFlag=0;
		  vectorBase[vIndex].selectFlag=0;
		  vectorBase[vIndex].matchSum=0;
		  vectorBase[vIndex].vectorAngle=angle;   //DP("vectorBase["<<vIndex<<"].vectorAngle="<<vectorBase[vIndex].vectorAngle<<END);
		  vectorBase[vIndex].vCenter.x=(vectXStart+vectXEnd)/2;  //_//_DC(" x="<<vectorBase[contourIndex].v[vIndex].vCenter.x;
		  vectorBase[vIndex].vCenter.y=(vectYStart+vectYEnd)/2;  //_//_DC(" y="<<vectorBase[contourIndex].v[vIndex].vCenter.y<<END);
		  vectorBase[vIndex].contourIndex=contourIndex;
		  vectorBase[vIndex].letterIndex=letterIndex;
	          vectorCount++;
		  vectorBase[0].allVectorCount++;
	  }else{   //DP("set3");
					vectXStart=vectorBase[vIndex-1].vStart.x;		//@DC("new vectXStart="<<vectXStart);
					vectYStart=vectorBase[vIndex-1].vStart.y;		                //@DC(" new vectYStart="<<vectYStart);
					length=((vectXStart-vectXEnd)*(vectXStart-vectXEnd)+(vectYStart-vectYEnd)*(vectYStart-vectYEnd))*100;
					if(length>0)length=(short)sqrtA((int)length);  //attention. length*100 increase Accuracy of short
					vectorBase[vIndex-1].vEnd.x=vectXEnd;  //DP(" eX "<<vectXEnd);
					vectorBase[vIndex-1].vEnd.y=vectYEnd;  //DP(" eY "<<vectYEnd);
					vectorBase[vIndex-1].length=length;
					vectorBase[vIndex-1].matchSum=0;
					vectorBase[vIndex-1].vectorFlag=0;
					vectorBase[vIndex].selectFlag=0;
					vectorBase[vIndex-1].vectorAngle=setVectorAngle(vectXStart, vectYStart, vectXEnd, vectYEnd);
					vectorBase[vIndex-1].vCenter.x=(vectXStart+vectXEnd)/2; //_//_DC(" x1="<<vectorBase[contourIndex].v[vIndex-1].vCenter.x;
	   				vectorBase[vIndex-1].vCenter.y=(vectYStart+vectYEnd)/2;  //_//_DC(" y1="<<vectorBase[contourIndex].v[vIndex-1].vCenter.y;
					vectorBase[vIndex-1].contourIndex=contourIndex;
		            vectorBase[vIndex].letterIndex=letterIndex;
	  }
 }
 //_//_DC("done");

	
	       //if(vectorBase[0].allVectorCount>0)
		   //_//_DC("vIndex"<<vIndex<<" "<<" vectorBase[0].allVectorCount"<<
		   //vectorBase[0].allVectorCount<<" letterIndex["<<vectorBasebase.size()-1<<"]="<<
		   //vectorBase[vectorBase[0].allVectorCount-1].letterIndex<<
		   //END);

}//end method//////////////////////////////////////////////////////////////////
