#include "GBitsetContour.h"

void GBitsetContour::setVector(short contour){  //read angIntSum make Vector record. Here we recognise Vectors in letter

//_//_DC("angX[0]"<<angX[0]<<"angY[0]"<<angY[0]<<END);

    //vectorMarker.resize(0);
	vectorMarker[0]=0;
	int print=0;
	short vCount; //set vectorMarker read pozition on zero

	//vectorLimit=0;

	//vectorLimit=((xMax-xMin)*2+yMax-yMin)/2;
	//cout<<"xMax="<<xMax<<" xMin="<<xMin<<" yMax="<<yMax<<" yMin="<<yMin<<" vectorLimit="<<vectorLimit<<endl;
//if(vectorLimit<Radius*10)vectorLimit=Radius*10;
	//vectorMarker.resize(angDiffSum.size());
        vectorMarker[0]=0;  //start point
        vCount=1;

        vectorLimit=textLineSize;
	    if(textLineSize==0)vectorLimit=200;

///////////////////////////////////////////////////////////*main atrray
           for ( i = 0; i<angLength-1; i++ ) {
				    //_//_DC("angDiffSum["<<i<<"]"<<(short)angDiffSum[i]<<END);
					if (angDiffSum[i]!=0){
						//line[0][angY[i]][angX[i]]=125; 
						DT("i="<<i<<" xVectStart="<<angX[i]<<" yVectStart="<<angY[i]<<" vCount="<<vCount<<END);
						vectorMarker[vCount]=i;//start new vector
						//cout<<"set_vector"<<endl;
						setRecordInArray(contour, vCount); //we set contour vector
						vCount++;
					} 
           }//end for

vectorMarker[vCount]=angLength-1; //join contour
//DP("set end vector"<<END);
setRecordInArray(contour,vCount);  //we set end vector


}////////////////////////////////////////////////////////////////////*



