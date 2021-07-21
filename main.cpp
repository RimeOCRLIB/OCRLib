//  main.m
//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org .

//#include "OCRLib_main/OCRLib.h"
//#include "libocr/ocrlib_.h"
#include "libocr/OCRTypes/OCRTypes.h"
#include "libocr/GGraphOCR/GGraphOCR.h"
//#include "OCRLib_main/OCRLib_GGraphOCR.h"
#include "OCRLib_main/OCRLib.h"

commandData inputData;

using namespace std;
using namespace ocr;

//#define OCRLib_DLIB
//#include "OCRLib_main/OCRLib.h"

#define EXPORT __attribute__((visibility("default")))

// Initializer.
__attribute__((constructor)) static void initializer(void) { // 2
	// printf("[%s] initializer()\n", __FILE__);“”
	inputData.init = 0;
}

// Finalizer.
__attribute__((destructor)) static void finalizer(void) { // 3
														  // printf("[%s] finalizer()\n", __FILE__);
}


int main(int argc, char *argv[]) {

    string path = "";
	path = ROOT_PATH;
	inputData.data["root"] = path;
#ifdef ARM
	inputData.data["rootApp"] = path + "/storage/emulated/0/_Image2OCR/";
#else
	inputData.data["rootApp"] = path + "/OCRLib_ESP.app/Contents/MacOS/OCRLib_ESP";
#endif

	string argcStr;
	inputData.start = 0;
	readPreferences();
	if (argc > 1) {
		argcStr = argv[1];
		inputData.data["QUERY"] = argcStr;
		
	} else {
        print_("no input data"<<endl);
		//return 0;
	}

    pageRecLib pageLib;
    parseQuery(pageLib);
    
    //cout<<inputData.data["ocrData"]<<endl;
   


//#ifdef OCRLib_www
	//serverMonitor();
//#endif
	
#ifdef OCRLib
    string destString;
    
#include "libocr/workCode.h"

    processRequest(pageLib);
    if(pageLib.dataSize){
        destString=pageLib.data;
    }else{
        processInput(destString);
    }
    
	

	// if no command options, run ESP socket server loop
	if (destString == "") {
		cout<<"ocrlib done"<<endl;
	} else {
		cout << destString;
	}
	//((GMainEditor*)inputData.mainEditor)->destroy();
	//((GImageEditor *)inputData.imageEditor)->destroy();
	//((GFontEditor *) fontEditor)->destroy();
	//((GLogicProcessor *)inputData.logicProcessor)->destroy();
	//((GMemory *)inputData.longMemory)->destroy();

#endif

    return 0;
}
