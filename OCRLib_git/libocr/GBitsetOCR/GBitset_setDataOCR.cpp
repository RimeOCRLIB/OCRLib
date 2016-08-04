#include "GBitsetOCR.h"

void GBitsetOCR::setData(GFont *aliKali_,
						 vector<stringOCR>&strArray_,
					     vector<stringOCR>&correctionTable, //vector of all uncertan letters from OCR text for draw in correction table
						 GLogicProcessor *logicProcessor,   //dictionary controll object
 						 int iLeft_,
						 int iTop_,
						 int border,
						 int mode
						 ) {   //main point for GLetter Object. Order of all Letter OCR function

	aliKali=aliKali_;
	strArray=&strArray_;
	iLeft=iLeft_;
	iTop=iTop_;

		//bitsetBorder=border;   //size of border in string detector //NEED REWRITE
	//_DC("ALL vector COUNT "<<vectorBase[0][0].allVectorCount<<END);
	//DP("ALL string COUNT "<<strArray[0].size()<<END);
	
    //pageOCR(logicProcessor, mode);
    textOCR(correctionTable,logicProcessor, mode);


}//___________________________________________________________________________________________________
