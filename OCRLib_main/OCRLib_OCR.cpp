// C-
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org .
// C-
// C- This program is distributed in the hope that it will be useful,
// C- but WITHOUT ANY WARRANTY; without even the implied warranty of
// C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// C- GNU General Public License for more details.
// C-
/*  Implemmentation  to GMemoryLib dynamic library  */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include "../libocr/config.h"
#include "OCRLib.h"
#ifdef MACOS
#  include <malloc/malloc.h>
#endif

#include "../libocr/GFontEditor/GFontEditor.h"
#include "../libocr/GGraphOCR/GGraphOCR.h"
#include "../libocr/GImageEditor/GImageEditor.h"
#include "../libocr/GLogicProcessor/GLogicProcessor.h"
#include "../libocr/GMainEditor/GMainEditor.h"
#include "../libocr/GMemory/GMemory.h"
#include "../libocr/OCRString/php2stl.h"
#include "../libocr/OCRTypes/OCRTypes.h"

#ifdef ESP
#  include "esp_command.h"
#endif

//#define DB_NAME "LIBRARY_CATALOG"
//#define DB_NAME "DHARMABOOK"

using namespace std;

/*
 void initCommandData() {
 inputData.data["root"] = "/_Image2OCR/";
 cout << "init" << inputData.init << endl;
 readPreferences();
 inputData.init = 1;
 }
 */


int test_fn(){
    cout<<endl<<"www@@@www"<<endl;
    return 2;
}

char* ctestPage(pageRec *page_){
    pageRec &page=*page_;
    cout<<"page.index:"<<(int)page.index<<" page.text:"<<page.text<<endl;
    //page.text=(char*)malloc(10);
    memcpy(page.text,"world",5);
    string str=".";
    page.index=(int)str.size();
    char *st=(char *)malloc(page.index+1);
    st[page.index]=0;
    memcpy(st,&str[0],page.index);
    
    return st;
}
void ctest1(int *i){
    *i=15;
}

//////________________ OCR____________________________

void initOCR() {
    if(!inputData.init) initCommandData();
    if(inputData.init < 2) {
        inputData.fontEditor = GFontEditor::create();
        inputData.mainEditor = GMainEditor::create();
        StartCpuInfo();
        inputData.init = 2;
    }
     GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    logicProcessor->readDictionary();
}  //______________________________________________________________________________________

void batchOCR(pageRecLib &page) {
    initOCR();
    GMainEditor *mainEditor = (GMainEditor *)inputData.mainEditor;
    mainEditor->readInputAction();
}  //______________________________________________________________________________________



void bookOCR(const char *path) {
    cout << path;
    inputData.data["ocrData"] = "vectorOCR";
    // inputData.data["ocrData"]="batchOCR";
    inputData.fileList.push_back(path);
    GMainEditor *mainEditor = (GMainEditor *)inputData.mainEditor;
    mainEditor->readInputAction();
}  //______________________________________________________________________________________

void bookOCRGGraph(const char *path) {
    cout << path;
    inputData.data["ocrData"] = "vectorOCR";
    inputData.fileList.push_back(path);
    GMainEditor *mainEditor = (GMainEditor *)inputData.mainEditor;
    readDirectoryToArray(inputData.fileList,inputData.fileList[0],"img");
    cout<<"inputData.fileList.size()="<<inputData.fileList.size()<<endl;
    
    //cout_<<" inputData.fileList="<<inputData.fileList.size()<<" first file="<<inputData.fileList[0]<<endl;
    mainEditor->startVectorOCR();
    
}  //______________________________________________________________________________________



void letterBaseProcess1(void *conn, const char *request_) {
    initOCR();
    string request = request_;
    inputData.data["QUERY"] = request_;
    parseQuery();
    string ocrData = inputData.data["ocrData"];
    if(ocrData == "drawBase") {
        string         result;
        int            page = atoi(inputData.data["index"].c_str());
        string         path = inputData.data["path"];
        vector<string> pathList;
        readDirectoryToArray(pathList, path, "img");
        for(int i = page * 400; i < (page + 1) * 400 && i < pathList.size(); i++) {
            result += str_replace(path.c_str(), "", pathList[i]);
            result += ":|:";
        }
#ifdef ESP
        espRenderBlock((HttpConn *)conn, result.c_str(), result.size());
#endif
        return;
    }
}  //______________________________________________________________________________________

//////________________ LETER BASE____________________________

void letterBaseProcess(void *conn, const char *request_) {
    initOCR();
    string request = request_;
    inputData.data["QUERY"] = request_;
    parseQuery();
    string ocrData = inputData.data["ocrData"];
    if(ocrData == "letterBase") {
        int      page = atoi(inputData.data["page"].c_str());
        GBitmap *img = ((GFontEditor *)inputData.fontEditor)->aliKali->drawOCRBaseImage(page, 20);
        //#drawGBitmap(conn, img);
        img->destroy();
        return;
    }
    if(ocrData == "letter") {
        int    index = atoi(inputData.data["letterIndex"].c_str());
        string dataStr = ((GFontEditor *)inputData.fontEditor)->drawEditLetter(index);
#ifdef ESP
        espRenderBlock((HttpConn *)conn, &dataStr[0], dataStr.size());
#endif
        return;
    }
    if(ocrData == "letterImg") {
        int      letterIndex = atoi(inputData.data["index"].c_str());
        GBitmap *img = ((GFontEditor *)inputData.fontEditor)->aliKali->drawLetter(letterIndex);
        img->colorModeOut = 1;
        //#drawGBitmap(conn, img);
        img->destroy();
        return;
    }
    if(ocrData == "letterCount") {
        int  letterCount = (int)((GFontEditor *)inputData.fontEditor)->aliKali->letterCount() / 20 / 20;
        char msg[64];
        sprintf(msg, "%d", letterCount);
#ifdef ESP
        espRenderBlock((HttpConn *)conn, msg, strlen(msg));
#endif
        return;
    }
    if(ocrData == "searchLetters") {
        int      page = atoi(inputData.data["page"].c_str());
        string   letter = inputData.data["text"];
        GBitmap *img = ((GFontEditor *)inputData.fontEditor)->aliKali->drawOCRBaseImage(page, 20);
        //#drawGBitmap(conn, img);
        img->destroy();
        return;
    }
    if(ocrData == "exportFontXML") {
        string path = inputData.data["root"] + "OCRData/OCRTables/GFont.xml";
        ((GFontEditor *)inputData.fontEditor)->exportGFontsDB(path);
        string msg = "done export";
#ifdef ESP
        espRenderBlock((HttpConn *)conn, msg.c_str(), msg.size());
#endif
        return;
    }
    if(ocrData == "drawBase") {
        string result;
        int    pageIndex = atoi(inputData.data["index"].c_str());
        ((GFontEditor *)inputData.fontEditor)->aliKali->drawHTML(pageIndex, 20, result, 0);
#ifdef ESP
        espRenderBlock((HttpConn *)conn, result.c_str(), result.size());
#endif
        return;
    }
}  //______________________________________________________________________________________

void letterProcess(void *conn, const char *request_, int len) {
    if(!inputData.init) initCommandData();
    if(inputData.init < 2) {
        inputData.fontEditor = GFontEditor::create();
        inputData.init = 2;
    }
    
    string request;
    request.resize(len);
    memcpy(&request[0], request_, len);
    vector<string> v;
    explode("&", request, v);
    string s = v[0];
    string index_ = substr((int)s.find("=") + 1, (int)s.size(), s);
    int    index = atoi(index_.c_str());
    
    GFont *aliKali = ((GFontEditor *)inputData.fontEditor)->aliKali;
    string dataStr;
    aliKali->getStr(index, dataStr);
    
    // cout<<"dataStr.size():"<<dataStr.size();
#ifdef ESP
    espRenderBlock((HttpConn *)conn, &dataStr[0], dataStr.size());
#endif
}  //______________________________________________________________________________________

void letterImg(void *conn, char *buff, unsigned int letterID) {
    /*
     if(!inputData.init) initCommandData();
     if(inputData.init < 2) {
     inputData.fontEditor=GFontEditor::create();
     inputData.init=2;
     }
     // GBitmap
     // *img=((GFontEditor*)inputData.fontEditor)->aliKali->drawLetter(index);
     GBitmap *img=((GFontEditor *)inputData.fontEditor)->aliKali->drawLetterPict(index);
     drawGBitmap(conn, img);
     img->destroy();
     */
    return;
}  //______________________________________________________________________________________

void fileDB(char *bufferDir, char *bufferFile) {
    if(!inputData.init) initCommandData();
    vector<string> fileList;
    vector<string> dirList;
    string         path = "/storage/emulated/0";
    readDirectory(fileList, dirList, path);
    string reportDir = "";
    join(dirList, "\n", reportDir);
    string reportFile = "";
    join(dirList, "\n", reportFile);
    memcpy(bufferDir, &reportDir[0], reportDir.size());
    memcpy(bufferFile, &reportFile[0], reportFile.size());
}  //______________________________________________________________________________________


void imageFileOCR(){
    GGraphOCR pechaGraph;
    pechaGraph.lineOCR();
    return;
}

void exportGFontLetters(const char *path){
    initOCR();
    GFontEditor *fontEditor = (GFontEditor *)inputData.fontEditor;
    fontEditor->exportGFontsImages(path);
}
    

