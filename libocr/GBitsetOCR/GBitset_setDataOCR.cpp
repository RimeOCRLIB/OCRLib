#include "GBitsetOCR.h"

void GBitsetOCR::setData(GFont *aliKali_,
						 vector<stringOCR>&strArray_,
					     vector<stringOCR>&correctionTable, //vector of all uncertan letters from OCR text for draw in correction table
						 GLogicProcessor *logicProcessor,   //dictionary control object
 						 int iLeft_,
						 int iTop_,
						 int border,
						 int mode
						 ) {   //main point for GLetter Object. Order of all Letter OCR function

	aliKali=aliKali_;
	strArray=&strArray_;
	iLeft=iLeft_;
	iTop=iTop_;

    textOCR(correctionTable,logicProcessor, mode);

}//___________________________________________________________________________________________________
