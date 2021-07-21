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

#include "OCRLib_main/OCRLib.h"
#include "libocr/ocrlib_.h"

#ifdef ESP
#define ROOT_PATH "/_Image2OCR/";
#endif

#ifdef ARM
	#define ROOT_PATH "/storage/emulated/0/_Image2OCR"
	//#undef stderr
	//FILE *stderr = &__sF[2];
	#undef FORK
#endif

commandData inputData;

using namespace std;
using namespace ocr;

//#define OCR_DYLIB
#include "OCRLib_main/OCRLib.h"

#define EXPORT __attribute__((visibility("default")))

// Initializer.
__attribute__((constructor)) static void initializer(void) { // 2
	// printf("[%s] initializer()\n", __FILE__);
	inputData.init = 0;
}

// Finalizer.
__attribute__((destructor)) static void finalizer(void) { // 3
														  // printf("[%s] finalizer()\n", __FILE__);
}

int test2(){return 2;}

int main(int argc, char *argv[]) {
	// cout<<"test:"<<test()<<endl;
	string path = "";
	path = ROOT_PATH;

	inputData.data["root"] = path;
#ifdef ARM
	inputData.data["rootApp"] = path + "/storage/emulated/0/_Image2OCR/";
#else
	inputData.data["rootApp"] = path + "/OCRLib_ESP.app/Contents/MacOS/OCRLib_ESP";
#endif

	// for(int i=0;i<argc;i++)cout<<argv[i]<<endl;
	string argcStr;
	inputData.start = 0;
	if (argc > 1) {
		argcStr = argv[1];
		inputData.data["QUERY"] = argcStr;
	} else {
		GGraphOCR graph;
		graph.textOCR();
		return 0;
	}

	// ostringstream out;
	// out<<"/_Image2OCR/log/__log_"<<t<<".txt";
	// inputData.data["log"]=out.str();
	// inputData.log.open(inputData.data["log"]); inputData.log.flush();
	// inputData.log<<argcStr;
	parseQuery();

	//#ifdef OCRLib_www
	// serverMonitor();
	//#endif

#ifdef OCRLib_AI
	serverMonitor();
#endif
	string destString;
	readPreferences();
	/*
	string path_=ROOT_PATH;
	path_+="1.bin";

	GVector *v=GVector::create(path_);
	v->push_back(path_);
	string data;
	v->getStr(0,data);
	cout<<data;
	*/

#ifdef OCRLib

	GMemory *longMemory = GMemory::create();
	inputData.longMemory = longMemory;

	GMainEditor *mainEditor = GMainEditor::create();
	inputData.mainEditor = mainEditor;

	GImageEditor *imageEditor = GImageEditor::create();
	inputData.imageEditor = imageEditor;

	// GFontEditor *fontEditor = GFontEditor::create();
	// inputData.fontEditor = fontEditor;

	GLogicProcessor *logicProcessor = GLogicProcessor::create();
	inputData.logicProcessor = logicProcessor;

	// fontEditor->logicProcessor = logicProcessor; // send data to imageEditor;
	mainEditor->logicProcessor = logicProcessor; // send data to imageEditor;
	
	//#include "libocr/workCode.h"

	processInput(destString);

	// if no command options, run ESP socket server loop
	if (destString == "") {
		cout<<"ocrlib done"<<endl;
	} else {
		cout << destString;
	}

#endif

	// cout<<"inputData.data["QUERY"]="<<inputData.data["QUERY_STRING"]<<endl;
	// cout<<"destString="<<destString.size()<<endl;
	// remove(inputData.data["log"].c_str());

#ifdef OCRLib
	mainEditor->destroy();
	imageEditor->destroy();
	//  fontEditor->destroy();
	logicProcessor->destroy();
#endif

exit(0);
}