#include "GBitsetOCR.h"

//compare all words on pecha with letter in OCRBase. Main point for letter recognition

void GBitsetOCR::setOCRCorrelation(int stackFlag, int mode){
////////////////////////////////////////////set data/////////////////////////////////////////////////////////////////////*
/*
short record,testVectorCount;
OCRPoint letterCenter;
int maxCorrelationRecord, correlationByHollogram, correlation;
int maxCorrelation, deltaLen;
unsigned int vectorInd,letterInd;
int upLimit,downLimit,leftLimit,rightLimit,vCount;
	int print=0;
	int pr=0;

DP("//__________start correlation sIndex="<<sIndex<<" wIndex="<<wIndex<<" stackFlag"<<stackFlag<<END);

	wordSrc=&strArray[0][sIndex].wordArray[wIndex];
	letterCenter=wordSrc[0].center;
	//word.center=wordSrc[0].center;
	//word.x0=wordSrc[0].x0;
	//word.y0=wordSrc[0].y0;
	//word.x1=wordSrc[0].x1;
	//word.y1=wordSrc[0].y1;
	//word.h=wordSrc[0].h;
	//word.w=wordSrc[0].w;

	upLimit=wordSrc[0].y1+wordSrc[0].h*4;
	downLimit=wordSrc[0].y0-wordSrc[0].h*4;
	leftLimit=wordSrc[0].x0-wordSrc[0].w/2;
	rightLimit=wordSrc[0].x1+wordSrc[0].w/2;

	 DP("upLimit="<<upLimit<<" downLimit="<<downLimit<<" leftLimit="<<leftLimit<<" rightLimit="<<rightLimit<<" wordSrc.h="<<wordSrc[0].h<<" word.w"<<wordSrc[0].w<<END);
	 DP("letterCenter.x="<<letterCenter.x<<" letterCenter.y="<<letterCenter.y<<END);
	 DP("wordSrc.x0="<<wordSrc[0].x0<<" wordSrc.x1="<<wordSrc[0].x1<<" vectorBase[0].allVectorCount"<<vectorBase[0].allVectorCount<<END);

		glyphTest.vectorCount=0;
		glyphTest.lengthSum=0;
		vCount=0;
	for(vectorInd=0;vectorInd<vectorBase[0].allVectorCount;vectorInd++){
		if(
		   (vectorBase[vectorInd].vCenter.x>leftLimit&&vectorBase[vectorInd].vCenter.x<rightLimit)&&
		   (vectorBase[vectorInd].vCenter.y<upLimit&&vectorBase[vectorInd].vCenter.y>downLimit)
		   ){
			glyphTest.v[vCount]=vectorBase[vectorInd];
			glyphTest.lengthSum+=vectorBase[vectorInd].length;
			vCount++; if(vCount==300)break;
						//DC("match"<<END);
		}
			//DC("vStart.x="<<vectorBase[0][vectorInd].vStart.x<<" vStart.y="<<vectorBase[0][vectorInd].vStart.y<<END);
	}
	 DP("glyphTest size ="<<vCount<<END);
	 //DC("word.h="<<wordSrc[0].h<<END);
		glyphTest.vectorCount=vCount;
	if(vCount<3||vCount>200)return;


	///////////////////////////////////////// actual correlation////////////////////////////////////////////////////////////
	//letterInd=67;
//DC("GFont[0].base.size()="<<GFont[0].base.size()<<END);

		correlationByHollogram=0;
		if(stackFlag==1){
		  setOCRLetterCorrelation(1,0,mode);
		 }

		if(stackFlag==3){
		  setOCRLetterCorrelation(1,0,mode);
		  setOCRLetterCorrelation(0,1,mode);
		}

	   if(stackFlag==7){
		  setOCRLetterCorrelation(7,0,mode);
		}

		if(stackFlag==9){
		  setOCRLetterCorrelation(7,0,mode);
		  setOCRLetterCorrelation(0,1,mode);

		}

		if(stackFlag==100){
		  setOCRLetterCorrelation(100,0,mode);
		}

		//DP("strArray[0]["<<sIndex<<"].wordArray["<<wIndex<<"].match.size()="<<strArray[0][sIndex].wordArray[wIndex].match.size()<<END);
		//for(int t=0;t<strArray[0][sIndex].wordArray[wIndex].match.size();t++){
		//	 DP("strArray[0]["<<sIndex<<"].wordArray["<<wIndex<<"].match["<<t<<"]="<<strArray[0][sIndex].wordArray[wIndex].match[t].correlation<<END);
		//	 DP("name1="<<strArray[0][sIndex].wordArray[wIndex].match[t].name.c_str()<<END);
		//};
				//for(int t=0;t<strArray[0][sIndex].wordArray[wIndex].match1.size();t++){
				//DC("strArray[0]["<<sIndex<<"].wordArray["<<wIndex<<"].match1["<<t<<"]="<<strArray[0][sIndex].wordArray[wIndex].match1[t].correlation<<END);
				//DC("name1="<<strArray[0][sIndex].wordArray[wIndex].match1[t].name.c_str()<<END);
				//};
*/
	
}//end method//////////////////////////////////////////////////////**

int GBitsetOCR::setOCRLetterCorrelation(int letterFlag, int matchNum,int mode){
int correlation=0;
//int maxCorrelation=0;
int mC=0;
int maxX,maxY;
///wordSrc=&strArray[0][sIndex].wordArray[wIndex];

for(int in=0; in<aliKali[0].letterCount();in++){
  glyph=aliKali->getLetter(in);
  DP(" letterFlag="<<letterFlag<<" matchNum="<<matchNum<<"aliKali[0]["<<in<<"].stackFlag="<<glyph->stackFlag<<" name="<<glyph->name<<" OCRStatus="<<glyph->OCRStatus<<END);
	  //if(mode==NEWLETTER){       //коррелируем новую букву добавленную в базу со всеми буквами на страницы
	  // if(glyph[0].newLetter!=1)continue;
	  // }
	  //if(mode==NEWLETTERINBOOK){       //коррелируем новую букву добавленную в базу со всеми буквами на страницы
	  // if(glyph[0].newLetterInBook!=1)continue;
	  //}

   if(glyph[0].OCRIndex!=3){ //continue;
	   if(letterFlag!=100){
			  if(letterFlag==7||letterFlag==9||letterFlag==1){
				 if(glyph->stackFlag==0)continue;
			  }
			  if(letterFlag==0){
				 if(glyph[0].stackFlag!=letterFlag)continue;
			  }
	   }

	//if(letterInd>3)return;
		//short flag=0;
		//DP("glyph[0].letterW="<<glyph[0].letterW<<" word.w="<<word.w<<END);
		if(glyph->vectorCount<1){continue;}

       DP("setScaleRecord");
       //TIME_START
		setScaleRecord();
		//gSrc=glyphTest;

	DP("//__________________glyph[0].vectorCount"<<glyph[0].v[0].allVectorCount<<" in="<<in<<
	" glyph[0].lengthSum="<<glyph[0].lengthSum<<" glyph[0].name="<<glyph[0].name.c_str()<<
	" glyph[0].Character="<<glyph[0].Character<<
	" gSrc.lengthSum="<<gSrc.lengthSum<<" gSrc.vectorCount="<<gSrc.v[0].allVectorCount<<END);


		//if(gSrc.lengthSum<1||glyph->lengthSum<1)continue;

				//key=0;
				//DC("@@wIndex="<<wIndex<<END);
				//if(wIndex==1&&letterInd==35)key=1;
			//TIME_PRINT
///(deprecated)correlation=setHollogramCorrelation(0);
			//wordSrc=&strArray[0][sIndex].wordArray[wIndex];
			//gSrc.center.x=wordSrc[0].center.x;
			//gSrc.center.y=wordSrc[0].center.y;
			//correlation=70;
			//TIME_PRINT DP("setHollogramCorrelation"<<END);
			//(glyph[0].Wylie.c_str());
			DP(" correlation1="<<correlation<<" glyph[0].name="<<glyph[0].name.c_str()<<
			" glyph[0].mask32Count="<<glyph[0].mask32Count<<END);

		 if(correlation>50||glyph->Character==1){
		 //if correlation is good enouth we check matrix attributes in letter
			if(glyph[0].mask32Count()>0){
				mC=0;
				for(int m=0;m<glyph[0].mask32Count();m++){

				//DP("gSrc.center.x="<<gSrc.center.x<<END);
				//DP("glyph[0].mask32["<<m<<"].x="<<glyph[0].mask32[m].x<<END);
				//DP("glyph[0].mask32["<<m<<"].y="<<glyph[0].mask32[m].y<<END);
				//DP("strArray[0][sIndex].LimY0="<<strArray[0][sIndex].LimY0<<END);
				//DP("gSrc.center.y="<<gSrc.center.y<<END);
				//DP("glyph[0].mask32[m].y+gSrc.center.y-strArray[0][sIndex].LimY0-16="<<glyph[0].mask32[m].y+gSrc.center.y-strArray[0][sIndex].LimY0-16<<END);

				//mC+=setMatrix->setMatrixCorrelation(&glyph[0].mask32[m],
				//								   glyph[0].mask32[m].xMask-8+gSrc.center.x,
				//								   glyph[0].mask32[m].yMask+gSrc.center.y-strArray[0][sIndex].LimY0-8,
				//								   0,0,
				//								   16,
				//								   &maxX,
				//								   &maxY,
				//								   c_out
				//								   );
				//ImageProcessor->WriteImageData(setMatrix->drawDataRGB,IMGNOFLIP);
				//DP("MatrixCorrelation="<<mC<<" glyph[0].Wylie="<<glyph[0].Wylie.c_str()<<END);
				//SH(mC);SH(glyph[0].Wylie.c_str());

				}
			 correlation=(correlation+mC*2)/(1+glyph[0].mask32Count()*2);
			 //correlation=(mC*2)/(glyph[0].mask32Count*2);

			}
			if(correlation>50){
				OCRMatch match;
				match.correlation=correlation;
				match.letterIndex=in;
				match.name=glyph[0].name;
				match.Character=glyph->Character; //DP("glyph[0].Character="<<glyph[0].Character<<END);
					//if(matchNum==0)wordSrc[0].match.push_back(match);
					//if(matchNum==1)wordSrc[0].match1.push_back(match);
			}
		}
	}else{//tserto correlation
				mC=setMatrix->setMatrixCorrelation(&glyph[0].mask32[0],
												   strArray[0][sIndex].wordArray[wIndex].xp0-8,
												   strArray[0][sIndex].wordArray[wIndex].yp1-strArray[0][sIndex].LimY0-8,
												   0,0,
												   24,
												   &maxX,&maxY,
												   c_out
												   );
				strArray[0][sIndex].wordArray[wIndex].tsertoFlag=0;
				if(mC>85)strArray[0][sIndex].wordArray[wIndex].tsertoFlag=1;
				//ImageProcessor->WriteImageData(setMatrix->drawDataRGB,IMGNOFLIP);
				//DP("MatrixCorrelation="<<mC<<" glyph[0].Wylie="<<glyph[0].Wylie.c_str()<<END);
				//SH(mC);
				//SH(glyph[0].Wylie.c_str());

	}

	   DP(" correlation2="<<correlation<<" glyph[0].name="<<glyph[0].name.c_str()<<END);
       glyph->destroy();
}

return 1;
}//_____________________________________________________________________________



void GBitsetOCR::setTestOCRCorrelation(int sIndex, int wIndex, int mIndex){
////////////////////////////////////////////set data/////////////////////////////////////////////////////////////////////*
/*
	short record,testVectorCount;
OCRPoint letterCenter;
int maxCorrelationRecord, correlationByHollogram, correlation;
int maxCorrelation, deltaLen;
unsigned int vectorInd,letterInd;
int upLimit,downLimit,leftLimit,rightLimit,vCount;
int print=0;
int pr=0;
int maxX,maxY;

DP("//__________start test correlation sIndex="<<sIndex<<" wIndex="<<wIndex<<END);


						//Create GBitsetMatrix for matrix correlation
						//SH(inputData.borderSize());
						setMatrix=GBitsetMatrix::createRegion(inputBitmap,
															  inputBitmap->borderSize(),
															  strArray[0][sIndex].LimY0,
															  inputBitmap->columns()-inputBitmap->borderSize(),
															  strArray[0][sIndex].LimY1-strArray[0][sIndex].LimY0,
                                                              1,1,
															  IMGNOFLIP
															  );


	wordSrc=&strArray[0][sIndex].wordArray[wIndex];
		wordSrc[0].match.resize(0);
	letterCenter=wordSrc[0].center;
	//word.center=wordSrc[0].center;
	//word.x0=wordSrc[0].x0;
	//word.y0=wordSrc[0].y0;
	//word.x1=wordSrc[0].x1;
	//word.y1=wordSrc[0].y1;
	//word.h=wordSrc[0].h;
	//word.w=wordSrc[0].w;

	upLimit=wordSrc[0].y1+wordSrc[0].h*4;
	downLimit=wordSrc[0].y0-wordSrc[0].h*4;
	leftLimit=wordSrc[0].x0-wordSrc[0].w/2;
	rightLimit=wordSrc[0].x1+wordSrc[0].w/2;

	 //DC("upLimit="<<upLimit<<" downLimit="<<downLimit<<" leftLimit="<<leftLimit<<" rightLimit="<<rightLimit<<" word.h="<<word.h<<" word.w"<<word.w<<END);
	 DP("letterCenter.x="<<letterCenter.x<<" letterCenter.y="<<letterCenter.y<<END);
	 //DC("word.x0="<<word.x0<<" word.x1="<<word.x1<<" vectorBase[0][0].allVectorCount"<<vectorBase[0][0].allVectorCount<<END);

		glyphTest.vectorCount=0;
		glyphTest.lengthSum=0;
		vCount=0;
	for(vectorInd=0;vectorInd<vectorBase[0].allVectorCount;vectorInd++){
		if(
		   (vectorBase[vectorInd].vCenter.x>leftLimit&&vectorBase[vectorInd].vCenter.x<rightLimit)&&
		   (vectorBase[vectorInd].vCenter.y<upLimit&&vectorBase[vectorInd].vCenter.y>downLimit)
		   ){
			glyphTest.v[vCount]=vectorBase[vectorInd];
			glyphTest.lengthSum+=vectorBase[vectorInd].length;
			vCount++; if(vCount==300)break;
						//DC("match"<<END);
		}
			//DC("vStart.x="<<vectorBase[0][vectorInd].vStart.x<<" vStart.y="<<vectorBase[0][vectorInd].vStart.y<<END);
	}
	 DP("glyphTest size ="<<glyphTest.vectorCount<<END);
	 //DC("word.h="<<wordSrc[0].h<<END);
		glyphTest.vectorCount=vCount;
	if(vCount<3||vCount>200)return;


	///////////////////////////////////////// actual correlation////////////////////////////////////////////////////////////
	//letterInd=67;
//DC("GFont[0].base.size()="<<GFont[0].base.size()<<END);

		correlationByHollogram=0;
	//if(letterInd>3)return;
	short flag=0;
		glyph=&aliKali[0][mIndex];
		//DP("glyph[0].letterW="<<glyph[0].letterW<<" word.w="<<word.w<<END);
		if(glyph->vectorCount<1)return;
			glyph[0].v[0].allVectorCount=glyph->vectorCount;

	DP("//__________________glyph[0].vectorCount="<<glyph[0].vectorCount<<" glyph[0].lengthSum="<<glyph[0].lengthSum<<" glyph[0].name="<<glyph[0].name.c_str()<<" glyph[0].Character="<<glyph[0].Character<<END);
			//DP("setScaleRecord")TIME_START
		setScaleRecord();
		//gSrc=glyphTest;
		if(gSrc.lengthSum<1||glyph->lengthSum<1)return;
			vDest=glyph->v;
			vSrc=gSrc.v;
			int sT=vDest[0].allVectorCount;
			int dT=vSrc[0].allVectorCount;
				//key=0;
				//DC("@@wIndex="<<wIndex<<END);
				//if(wIndex==1&&letterInd==35)key=1;
			//TIME_PRINT
			correlation=setHollogramCorrelation(1);
			//TIME_PRINT DP("setHollogramCorrelation"<<END);
            DP(" correlation1="<<correlation<<" glyph[0].name="<<glyph[0].name.c_str()<<
			" glyph[0].mask32Count="<<glyph[0].mask32Count<<END);

		 if(correlation>50||glyph->Character==1){
			//if correlation is good enouth we check matrix attributes in letter
			if(glyph[0].mask32Count>0){
			   int mC=0;
			   for(int m=0;m<glyph[0].mask32Count;m++){

            	DP("gSrc.center.x="<<gSrc.center.x<<END);
				DP("glyph[0].mask32["<<m<<"].x="<<glyph[0].mask32[m].xMask<<END);
				DP("glyph[0].mask32["<<m<<"].y="<<glyph[0].mask32[m].yMask<<END);
				DP("strArray[0][sIndex].LimY0="<<strArray[0][sIndex].LimY0<<END);
				DP("gSrc.center.y="<<gSrc.center.y<<END);
				DP("glyph[0].mask32[m].y+gSrc.center.y-strArray[0][sIndex].LimY0-16="<<glyph[0].mask32[m].yMask+gSrc.center.y-strArray[0][sIndex].LimY0-16<<END);
				glyph[0].mask32[m].printMask(c_out);

					//mC=setMatrix->setMatrixCorrelation(&glyph[0].mask32[m],
					//								   glyph[0].mask32[m].xMask-8+gSrc.center.x,
					//								   glyph[0].mask32[m].yMask+gSrc.center.y-strArray[0][sIndex].LimY0-8,
					//								   0,0,
					//								   16,
					//								   &maxX,&maxY,
					//								   c_out
					//								   );
					//ImageProcessor->WriteImageData(setMatrix->drawDataRGB,IMGNOFLIP);
					//DP("MatrixCorrelation="<<mC<<" glyph[0].Wylie="<<glyph[0].Wylie.c_str()<<END);
					//SH(mC);SH(glyph[0].Wylie.c_str());
					correlation=(correlation+mC*2)/3;
			   }


			}
			OCRMatch match;
			match.correlation=correlation;
			match.letterIndex=mIndex;
			match.name=glyph[0].name;
				match.Character=glyph->Character; //DP("glyph[0].Character="<<glyph[0].Character<<END);
				wordSrc[0].match.push_back(match);
		 }
		 gSrc.correlation=correlation;

	   DP(" correlation="<<correlation<<" glyph[0].name="<<glyph[0].name.c_str()<<END);




        //DP(" correlation="<<correlation<<" glyph[0].name="<<glyph[0].name.c_str()<<END);
*/
}//end method//////////////////////////////////////////////////////**
