#include "GBitsetOCR.h"

float GBitsetOCR::lineOCRScaleDetector(GBitmap *line,
									   vector<OCRMatch>&matchLine, 
									   int yLimit0, 
									   int yLimit1, 
									   float scale,
									   int index,
									   int *betterCorrelation,
									   int limit){

//line size detector first pass
int print=0;
float step=scale/5;
int correlation; 
int maxCorrelation=0; 
float maxScale=0;
float scale_=1;

for(int s=-3;s<7;s++){
	scale_=scale+step*s;
	matchLine.resize(inputBitmap->columns()*scale_+256);
	correlation=lineOCR(line,matchLine, yLimit0, yLimit1, scale_,index,limit);
	if(correlation>maxCorrelation||correlation==maxCorrelation){
		maxCorrelation=correlation;
		maxScale=scale_;
	}	   
	DT("c="<<correlation<<" m="<<maxCorrelation<<" ms="<<maxScale<<" s="<<scale_<<" st="<<step<<endl);
}
	if(!maxScale){cout<<"no MAX_SCALE"<<endl; return 0;}
	//line size detector second pass
	DT("/2/______"<<endl);
	step=scale/10; float maxScale_=0;
	for(int s=0;s<2;s++){
		if(s==0)scale_=maxScale-step;
		if(s==1)scale_=maxScale+step;
		matchLine.resize(inputBitmap->columns()*scale_+256);
		correlation=lineOCR(line,matchLine, yLimit0, yLimit1, scale_,index,limit);
		if(correlation>maxCorrelation||correlation==maxCorrelation){
			maxCorrelation=correlation;
			maxScale_=scale_;
		}	
		DT("c="<<correlation<<" m="<<maxCorrelation<<" ms="<<maxScale_<<" s="<<scale_<<" st="<<step<<endl);
	}
	if(maxScale_>0){
		maxScale=maxScale_;
	}	
	//line size detector second pass
	DT("/3/______"<<endl);
	step=scale/20; maxScale_=0;
	for(int s=0;s<2;s++){
		if(s==0)scale_=maxScale-step;
		if(s==1)scale_=maxScale+step;
		matchLine.resize(inputBitmap->columns()*scale_+256);
		correlation=lineOCR(line,matchLine, yLimit0, yLimit1, scale_,index,limit);
		if(correlation>maxCorrelation||correlation==maxCorrelation){
			maxCorrelation=correlation;
			maxScale_=scale_;
		}	
		DT("c="<<correlation<<" m="<<maxCorrelation<<" ms="<<maxScale_<<" s="<<scale_<<" st="<<step<<endl);
	}
	if(maxScale_>0){
		maxScale=maxScale_;
	}
	scale=maxScale;
	*betterCorrelation=maxCorrelation;
	//
	DT("@@@ scale="<<scale<<endl);
	return scale;
	
};//___________________________________________________________________________________________	



int GBitsetOCR::lineOCR(GBitmap *line,vector<OCRMatch>&matchLine, int yLimit0, int yLimit1, float scale,int index,int scaleMode){

	int print=0;
	DT("m0_1 line.w="<<line->columns()<<"line.h="<<line->rows()<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<" scale="<<scale<<" index="<<index<<" scaleMode="<<scaleMode<<endl);
	GBitmap *lineScale=GBitmap::createScale(line,scale);
	DT("m0_2");
	//if(index==22){
    //    GImageEditor *editor=GImageEditor::create();
    //    editor->WriteImageData(lineScale,"/1.jpg",0);
    //    exit(0);
    //}    
	DT("m1");
    int OCRMode=1; //Tibetan
    
	setMatrix=GBitsetMatrix::createRegion(lineScale,
										  0,
										  0,
										  lineScale->columns(),
										  lineScale->rows(),
										  1,1,
										  IMGNOFLIP
										  );
	DT("m2");
	lineScale->destroy();
	
	setMatrix->letterCorrelation(
								 matchLine,
								 aliKali,
                                 index,
								 0, 
								 lineScale->rows(),
								 scaleMode,
                                 OCRMode,
								 0);
	DT("m3");
	setMatrix->destroy();

	//now we start check result of correlation and make decision about place of every letter

	int correlation=0; int count=0;
	for(int m=0;m<matchLine.size();m++){
		if(matchLine[m].correlation>80){
			correlation+=matchLine[m].correlation;
			count++;
		}
	}
	if(count&&correlation)correlation/=count;
    matchLine.resize(0);
	return correlation;

	
	return 0;
 
 }////////////////////////////////////////////////////////////////////////////////

