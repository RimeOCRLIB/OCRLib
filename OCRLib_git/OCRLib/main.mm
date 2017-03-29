//  main.m
//  OCRLib  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .


#ifdef COCOA
#import <Cocoa/Cocoa.h>  
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#ifdef WIN32
#include "dos.h"
#include <dirent.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>


//#include "unzip.h"
#include <mach-o/dyld.h>	// _NSGetExecutablePath
#include "debug.h"
//#include "../libaiml/src/angel.h"

#ifdef OCRLib
   #include "ocrlib_.h"
   #import "OCRLib.h"
   #import "GPDF.h"
   #include "zlib.h"
#else
   #include "ocrlib_www.h"
#endif
#include "AI.h"
#include "GMemoryLib.h"

commandData inputData;
associativeDatabase ai;

using namespace std;
#ifdef OCRLib
using namespace ocr;
using namespace pugi;
using namespace re2;
#endif

//Программа имеет три основных режима старта.
//1. Запуск утилиты командной строки с параметрами запуска.
//2. Старт по запросу HTTP сервера в режиме CGI скрипта
//3. Запуск интерфейса приложения Cocoa
//Во всех трех режимах параметры управления программой описываются в
//виде ?key=value&key1=value1  и сответствуют параметрам вызова CGI-скрипта

#ifdef OCRLib
int NSApplicationMain (int argc, 
                       const char *argv[]
                       ){
    cout<<"Hello";
    NSAutoreleasePool *pool=[[NSAutoreleasePool alloc]init];
    NSApplication *application = [NSApplication sharedApplication];
    [application setActivationPolicy:NSApplicationActivationPolicyRegular];
    OCRLibAppDelegate *appDelegate = [[OCRLibAppDelegate alloc] init];
    [application setDelegate:appDelegate];
    [application run];
    [pool release];
    return 0;
}
#endif

int main(int argc, char *argv[]){
    /*
    cout<<"Content-type: text/html"<<endl<<endl;
    cout<<"<html><body>"<<endl;
    cout<<"<h1>Hello World!</h1>"<<endl;
    cout<<"This is HTML formatted Hello World C++ CGI"<<endl;
    cout<<"</body></html>"<<endl;
    exit(0);
    */
    char pathPWD[1024];
    uint32_t size = sizeof(pathPWD);
    _NSGetExecutablePath(pathPWD, &size);
    string path=pathPWD; path=substr(0, (int)path.rfind("/_Image2OCR"),path);
    path+="/_Image2OCR/";      path="/_Image2OCR/";
    inputData.data["root"]=path;
    inputData.data["rootApp"]=pathPWD;
    
    //for(int i=0;i<argc;i++)cout<<argv[i]<<endl;
    string argcStr;
    inputData.start=0;
    if(argc>1)argcStr=argv[1];
    inputData.data["QUERY"]=argcStr;
    
    time_t t=clock();
    //ostringstream out;
    //out<<"/_Image2OCR/log/__log_"<<t<<".txt";
    //inputData.data["log"]=out.str();
    //inputData.log.open(inputData.data["log"]); inputData.log.flush();
    //inputData.log<<argcStr;
    inputData.c_out.open("/_Image2OCR/log/__OCRLib__.txt",ios::out | ios::app);
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);
    inputData.c_out<<str<<" -> "<<argcStr<<endl;
    inputData.c_out.close();
    parseQuery();
    readPreferences();
    
#ifdef OCRLib_AI
    serverMonitor();
#endif
    string destString;

    
#ifdef OCRLib
    GMainEditor *mainEditor=GMainEditor::create();
    GImageEditor *imageEditor=GImageEditor::create();
    inputData.mainEditor=mainEditor;
    inputData.imageEditor=imageEditor;
    GFontEditor *fontEditor=GFontEditor::create();
    inputData.fontEditor=fontEditor;
    GLogicProcessor *logicProcessor=GLogicProcessor::create();
    inputData.logicProcessor=logicProcessor;
    fontEditor->logicProcessor=logicProcessor; //send data to imageEditor;
    mainEditor->logicProcessor=logicProcessor; //send data to imageEditor;
    GMemory *longMemory=GMemory::create();
    inputData.longMemory=longMemory;
 

#include "workCode.h"
    
    processInput(destString);
    
    
      
#else
    processRequest(destString);
#endif
    
    //cout<<"inputData.data["QUERY"]="<<inputData.data["QUERY_STRING"]<<endl;

#ifdef OCRLib_www
    serverMonitor();
#endif
    
    //cout<<"destString="<<destString.size()<<endl;

    //анализируем запрос и возвращаем запущенное приложение Cocoa
    if(!destString.size()){
#ifdef OCRLib
        
    return NSApplicationMain(argc, (const char **)argv);
#endif
    }
    
    
    cout<<destString;
    //cout_<<head<<destString;
    //remove(inputData.data["log"].c_str());
    
#ifdef OCRLib
    mainEditor->destroy();
    imageEditor->destroy();
    fontEditor->destroy();
    logicProcessor->destroy();
#endif
    
}

