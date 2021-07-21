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

#include "OCRLib.h"
#include "OCRLibC.h"
#include "../libocr/config.h"
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#ifdef MACOS
#include <malloc/malloc.h>
#endif

#include "../libocr/OCRTypes/OCRTypes.h"
#include "../libocr/GFontEditor/GFontEditor.h"
#include "../libocr/GImageEditor/GImageEditor.h"
#include "../libocr/GLogicProcessor/GLogicProcessor.h"
#include "../libocr/GMainEditor/GMainEditor.h"
#include "../libocr/GMemory/GMemory.h"
#include "../libocr/OCRString/php2stl.h"
#include "../nxjson/nxjson.h"

#ifdef ESP
#include "esp_command.h"
#endif

//#define DB_NAME "LIBRARY_CATALOG"
//#define DB_NAME "DHARMABOOK"

using namespace std;

//#define  __attribute__((visibility("default")))
#define MAX_NUMBERS 99

#ifdef OCR_DYLIB
commandData inputData;
#endif


typedef struct catalogCategory {
    string name;
    string data;
} Category;


void cpstr(pageRec &page,const char *str){
    size_t size=strlen(str);
    page.data=(char*)malloc(size+1);
    memcpy(page.data,&str[0],size);
    page.data[size]=0;
    page.dataSize=(int)size;
}

void cpstr(pageRec &page,string &str){
    size_t size=str.size();
    page.data=(char*)malloc(size+1);
    memcpy(page.data,&str[0],size);
    page.data[size]=0;
    page.dataSize=(int)size;
}//____________________________________________________

void cpstr(pageRecLib &page,string &str){
    page.data=str;
    page.dataSize=(int)str.size();
}//____________________________________________________

void cpstr(pageRecLib &page,cstr str){
    page.data=str;
    page.dataSize=(int)page.data.size();
}//____________________________________________________


void getDBSize(pageRecLib &page){
    //void *result_, char *buff, int *intBuff
    if (!inputData.init)
        initCommandData();
    string str=page.db;
    if (str == "") str = "DHARMABOOK";
    //запрашиваем ответ базы данных
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    page.bookCount = (uint)v->size();
}//_____________________________________________________


void processRequest(pageRecLib &page){
    
    
    //cout<<"page.command:"<<page.command<<endl;
    switch (page.command){
        case GET_DB_RECORD_COUNT:{ //101
            getDBSize(page);
        }case GET_DB_RECORD:{ //100
            getDBRecord(page);
            break;
        }case RENDER_CATEGORY_LIST: { //11
            listCategory(page);
            break;
        }case RENDER_CATEGORY:{ //10
            searchCategory(page);
            break;
        }case RENDER_RECORD_CATEGORY:{ //14
            renderCategoryText(page);
            break;
        }case RENDER_TRANSLATE_PAGE:{ //6
            page.mode=MAIN_REPORT;
            translateLine(page);
            break;
        }case RENDER_TRANSLATE_RECORD:{ //2
            page.mode=MAIN_REPORT;
            translateLine(page);
            break;
        }case RENDER_TRANSLATE_LINE:{ //7
            page.mode=MAIN_REPORT;
            translateLine(page);
            break;
        }case  RENDER_DICT_REPORT:{  //4
            dictionaryReportMain(page);
            break;
        }case  RENDER_DICT_SAVE:{ //5
            reloadDictionary(page);
            break;
        }case  RENDER_SEARCH_DB_TABLE:{ //15
            searchDBTable(page);
            break;
        }case RENDER_LETTER_BY_ID:{
            string str;
            //renderLetterByID(page.index,(void*)&str);
            cpstr(page,str);
            break;
        }case RENDER_IMAGE_PROCESS:{
            imageProcess(page);
            break;
        }case CONVERT_UNI_TO_YAGPO:{
            UnicodeToYagpo(page);
            break;
        }case CONVERT_YAGPO_TO_UNI:{
            YagpoToUnicode(page);
            break;
        }case CONVERT_WYLIE_TO_UNI:{
            WylieToUnicode(page);
            break;
        }case CONVERT_UNI_TO_TRANSCRIPTION_RUS:{
            RusTranscription(page);
            break;
        }case CONVERT_UNI_TO_TRANSCRIPTION_ENG:{
            EngTranscription(page);
            break;
        }case CONVERT_UNI_TO_WYLIE:{
            UnicodeToWylie(page);
            break;
        }case BATCH_OCR:{
            batchOCR(page);
            break;
        }
            
            
        default :{
            page.intData=0;
            page.dataSize=0;
        }
    }
    
}

void processCRequest(pageRec *page_){
    
}

//функция чтения и передачи данных в интерфейс Python на основе бибиотеки CTypevvbv+++
char* getPageStream(pageRec *page_) {
    pageRec &page=*page_;
    static int lock=0; //переменная запрещает прерывание процесса передачи данных
    static char *resultBuf=(char*)malloc(10240);
    static uint sizePool=10240;  //размер резервированной памяти буыера передачи данных resultBuf
    //print("page.command:"<<page.command<<" text:"<<page.text<<" index:"<<page.index<<" db:"<<page.db);
    //print_("page.command:"<<page.command)
    if(page.command==RELEASE_DB_LOCK){
        lock=0;
        return 0;
    }
    if(lock){
        page.intData=SET_DB_LOCK;
        return 0;
    }
    if (!inputData.init)
        initCommandData();
    if(page.command && lock){
        page.intData=100;
        return 0; //данные еще не прочитаны интерфейсом
    }
    
    
    pageRecLib pageLib;
    
    
    //переписываем данные из структуры C
    if(page.mode==RENDER_DICT){
        //cout<<"@@@@ init C page.text:"<<page.text<<endl;
        pageLib.text = page.text;
        pageLib.data = page.data;
        pageLib.db = page.db;
        pageLib.ln = page.ln;
        pageLib.index = page.index;;
        pageLib.field = page.field;
        pageLib.pageCount = page.pageCount;
        pageLib.bookCount = page.bookCount;
        pageLib.intData = page.intData;
        pageLib.command = page.command;;
        pageLib.dataSize = page.dataSize;
    }else{
        //парсим запрос от интерфейса
        if(page.mode==RENDER_QUERY){
            inputData.data["QUERY"] = page.text;
            parseQuery();
            pageLib.command=inputData.ocrCode[inputData.data["ocrData"]];
        }else if(page.mode==RENDER_JSON){
            inputData.data["QUERY"] = page.text;
            parseJson();
            pageLib.command=inputData.ocrCode[inputData.data["ocrData"]];
        }
        
        pageLib.text = inputData.data["text"];
        pageLib.data = inputData.data["data"];
        pageLib.db = inputData.data["db"];
        pageLib.ln = inputData.data["ln"];
        pageLib.index = atoi(inputData.data["index"].c_str());
        pageLib.field = atoi(inputData.data["field"].c_str());
        pageLib.pageCount = atoi(inputData.data["pageCount"].c_str());
        pageLib.bookCount = atoi(inputData.data["bookCount"].c_str());
        pageLib.intData = atoi(inputData.data["intData"].c_str());
        pageLib.dataSize = 0;
    }
    
    //cout<<"===@@@@=== init page command:"<<page.command<<endl;  // <<" pageLib.text:"<<pageLib.text<<" text:"<<inputData.data["text"]<<endl;
    //cout<<"===@@@@=== init pageLib.command:"<< pageLib.command<<"  pageLib.intData:"<<pageLib.intData<<" ln:"<<pageLib.ln<<" ocrData"<<inputData.data["ocrData"]<<endl; // <<" user_text:"<<pageLib.text<<endl<<endl<<" data:"<<pageLib.data<<endl;
    
    
    //заполняем буфер ответа функции C
    processRequest(pageLib);
    
    //cout<<"===@@@mode:"<<page.mode<<" data:"<<pageLib.data<<endl;
    
    uint size=pageLib.dataSize;
    page.dataSize=size;
    
    if(!size){
        page.intData=0;
        page.dataSize=0;
        return 0;
    }
    
    if(size>sizePool){
        free(resultBuf);
        resultBuf=(char*)malloc(size+1);
    }
    memcpy(resultBuf,&pageLib.data[0], size);
    resultBuf[size]=0;
    
    //возвращаем полученные бинарные значения
    page.pageCount=pageLib.pageCount;
    page.bookCount=pageLib.bookCount;
    
    //блокируем функцию до подтверждения передачи
    lock=1;
    return resultBuf;
    
}


void closePage(pageRec &page){
    if(page.text)free(page.text);
    if(page.data)free(page.data);
    if(page.db)free(page.db);
    if(page.ln)free(page.ln);
}//____________________________________________________

void initPage(pageRec &page){
    page.text=0;
    page.data=0;
    page.db=0;
    page.ln=0;
}//____________________________________________________


int test(){
    cout<<1<<endl;
    return 1;
}//____________________________________________________

void initCommandData() {
    inputData.data["root"] = "/OSBL/";
    cout << "init" << inputData.init << endl;
    readPreferences();
    inputData.data["DATA_DB"]="/_Image2OCR/OCRData/DATA_DB/";
    GMemory *longMemory = GMemory::create();
    inputData.longMemory = (void*)longMemory;
    GLogicProcessor *logicProcessor = GLogicProcessor::create();
    inputData.logicProcessor = (void*)logicProcessor;
    inputData.init = 1;
}

/*
 
 const char* helloVarStr(char *str) {
 stData="wwwке234е5 234н 346н3456н34 5ен345н345ре к5н3 4н6 3465н 3465н 34 5н
 2435н 3245 н2 45н345р436рн"; char *msg=(char*)calloc(stData.size()+10, 1);
 memcpy(msg,(char*)&stData[0],stData.size());
 msg[stData.size()]=0;
 //cout<<msg<<1<<endl;
 return msg;
 }
 */


void imageProcess(pageRecLib &page) {
    string request=inputData.data["QUERY"];
    
    //print ("@@@@ request:"<<request)
    string path=inputData.data["inputFile"];
    decodeURL(path);
    string ocrData=inputData.data["ocrData"];
    string pathImgParameters;
    
    
    if(is_dir(path.c_str())) {
        pathImgParameters=path + "/__img.txt";
        if(ocrData=="imgDirRequest") {
            readText(request, pathImgParameters);
            cpstr(page,request);
            return;
        }
        if(ocrData !="imgDir") {
            writeText(request, pathImgParameters);
        } else {
            readText(request, pathImgParameters);
            inputData.data["QUERY"]=request;
            parseQuery();
        }
    } else {
        if(is_file(path)) {
            
            pathImgParameters=substr(0, (int)path.rfind("/"), path);
            pathImgParameters +="/__img.txt";
            if(ocrData=="imgDirRequest") {
                readText(request, pathImgParameters);
                cpstr(page,request);
                return;
            }
            if(ocrData !="imgDir") {
                writeText(request, pathImgParameters);
            } else {
                readText(request, pathImgParameters);
                inputData.data["QUERY"]=request;
                parseQuery();
            }
        } else {
            return;
        }
    }
    
    vector<string> fileList;
    readDirectoryToArray(fileList, path, "img");
    if(!fileList.size()) return;
    
    // fileList,action,Tr,Base,scale,blur,dotGain,erosion
    
    int index=0;
    int flag=0;
    if(ocrData=="imgDirPreview") {
        index=(int)fileList.size()/2;
        flag=1;
    }
    
    //print_("@@imgDirPreview flag:"<<flag<<endl;)
    
    while(index < fileList.size()) {
        GBitmap *img=GBitmap::create(fileList[index].c_str());
        GBitmap *img_=(GBitmap *)imageAdaptiveFiltration((void *)img);
        img->destroy();
        
        if(flag) {
            vector<string>v;
            explode("&",request, v);
            for(int i=0; i < v.size(); i++){
                if(v[i].find("lineH") != -1){
                    v[i]="lineH="+inputData.data["lineH"];
                }
            }
            request=implode("&",v);
            writeText(request, pathImgParameters);
            drawGBitmap(page, img_);
            img_->destroy();
            return;
        } else {
            path=fileList[index];
            path=substr(0, (int)path.rfind("."), path) + ".png";
            img_->savePNG(path);
            img_->destroy();
            print_(fileList[index]<<endl);
        }
        index++;
    }
}//_________________________________________________________________________

//////*********** TEXT PROCESS FUNCTIONS **********
void textProcess(pageRecLib &page) {
    if (!inputData.init)
        initCommandData();
    if (inputData.init < 2) {
        //inputData.fontEditor = GFontEditor::create();
        inputData.init = 2;
    }
    string request = page.text;
    inputData.data["QUERY"] = page.text;
    parseQuery();
    string ocrData = inputData.data["ocrData"];
    
    if (ocrData == "TibetanToRusTagger") {
        inputData.data["ln"] = "rus";
        string dataStr;
        string path = inputData.data["filePath"];
        readText(dataStr, path);
        ((GLogicProcessor *)inputData.logicProcessor)
        ->lineTextTranslation(dataStr, 0);
        path = path + ".out";
        writeText(dataStr, path);
        cpstr(page,"done");
    }
}//____________________________________________________

void HTML2UniConverter(pageRecLib &page){

    if (!inputData.init)initCommandData();
    
    string path=inputData.data["inputFile"];
    string mainString;
    ((GLogicProcessor *)inputData.logicProcessor)->HTML2UniConverter(mainString, path);
    string path_=str_replace(".txt","_out.txt", path);
    writeText(mainString, path_);
}

void *loadImage(const char *path, int mode) {
    GBitmap *img = GBitmap::create(path);
    return (void *)img;
}//____________________________________________________

void getDBRecord(pageRecLib &page){
    
    //void *result_, char *buff, int *intBuff
    if (!inputData.init)initCommandData();
    string str;
    str=page.db;
    //if (!str.size())
    str = "DHARMABOOK";
    //запрашиваем ответ базы данных
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    page.bookCount = (uint)v->size();
    
    //cout<<" db:"<<str<<" "<<str.size()<<"@@@ init size:"<<page.bookCount<<endl;
    
    if (page.index < v->size()) {
        TString st;
        v->getTStr(page.index, st);
        if (page.field < st.size()) {
            str = st[page.field];
            page.pageCount = st.size();
        }else{
            cpstr(page,"this field not exist");
            return;
        }
    }else{
        cpstr(page,"this record not exist");
        return;
    }
    cpstr(page,str);
}//_____________________________________________________


void helloVarStr(pageRecLib &page) {
    string stData = "www static char *msg=(char*)mprAllocFast(stData.size());";
    cpstr(page,stData);
}//____________________________________________________

void getFieldC(pageRecLib &page) {
    //const char *db, int rec_, int field
    
    if (!inputData.init)
        initCommandData();
    string str = page.db;
    if (str == "")
        str = "DHARMABOOK";
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    //cout << v->size();
    if (page.index > v->size()) {
        cpstr(page,"this record not exist");
        return;
    }
    TString st;
    v->getTStr(page.index, st);
    if (page.field >= st.size()) {
        cpstr(page,"this field not exist");
        return;
    }
    str=st[page.field];
    cpstr(page,str);
}//____________________________________________________

void getPageC(pageRecLib &page) {
    
    if (!inputData.init)
        initCommandData();
    string str = page.db;
    if (str == "")
        str = "DHARMABOOK";
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    page.bookCount = (uint)v->size();
    if (page.index > v->size()) {
        cpstr(page,"this record not exist");
        page.intData = 0;
        return;
    }
    TString st;
    v->getTStr(page.index, st);
    if (page.field >= st.size()) {
        cpstr(page,"this field not exist");
        page.intData = 0;
        return;
    }
    str = st[page.field];
    
    page.pageCount = (uint)st.size();
    cpstr(page,str);
}//____________________________________________________

void getPageC_(pageRecLib &page) {
    
    if (!inputData.init)
        initCommandData();
    
    string str = page.db;
    if (str == "")
        str = "DHARMABOOK";
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    page.bookCount = (uint)v->size();
    if (page.index > v->size()) {
        cpstr(page,"this page not exist");
        page.intData = 0;
        return;
    }
    
    TString st;
    v->getTStr(page.index, st);
    
    if (page.field >= st.size()) {
        cpstr(page,"this field not exist");
        page.intData = 0;
        return;
    }
    str=st[page.field];
    page.pageCount = st.size();
    cpstr(page,str);
}//____________________________________________________

void getIntC(pageRecLib &page) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string db = page.db;
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    page.bookCount = (uint)v->size();
    if (page.index > v->size()) {
        cout << "this record not exist";
        page.intData = 0;
        return;
    }
    TString st;
    v->getTStr(page.index, st);
    if (page.field >= st.size()) {
        cpstr(page,"this field not exist");
        page.intData = 0;
        return;
    }
    int c = st.getInt((uint)page.field);
    page.intData = c;
    page.pageCount = (uint)st.size();
}//____________________________________________________

void splitOnPagesC(const char *db) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    
    string str = db;
    // str="translationDictRus";
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    string ret = "0";
    ret[0] = 11;
    //cout << v->size();
    for (int i = 0; i < v->size(); i++) {
        // if(i%100==0)
        //cout << " " << i;
        TString st;
        v->getTStr(i, st);
        if (st.size() > 2)
            continue;
        TString st_;
        str = st[0];
        // re2::RE2::Replace(&str,".*\\[\\[ID:([\\d]+)\\]\\].*","\\1");
        // str="[[ID:"+str+"]]";
        st_.push_back(str);
        
        string key = st[1];
        int index = 0;
        int pageSize = 1024 * 9;
        
        while (index < key.size()) {
            string page = substr(index, pageSize, key);
            //находим конец фразы
            if (index + pageSize < key.size()) {
                str = substr(index + pageSize, 256, key);
                if (str.find("\n") != -1) {
                    str = substr(0, (uint)str.find("\n") + 1, str);
                } else if (str.find(" ") != -1) {
                    str = substr(0, (uint)str.find(" ") + 1, str);
                } else if (str.find("།") != -1) {
                    str = substr(0, (uint)str.find("།") + 3, str);
                } else if (str.find("༔") != -1) {
                    str = substr(0, (uint)str.find("༔") + 3, str);
                } else if (str.find("༴") != -1) {
                    str = substr(0, (uint)str.find("༴") + 3, str);
                } else if (str.find("་") != -1) {
                    str = substr(0, (uint)str.find("་") + 3, str);
                } else if (str.find(".") != -1) {
                    str = substr(0, (uint)str.find(".") + 1, str);
                }
                page += str;
            }
            st_.push_back(page);
            index += pageSize + str.size();
        }
        v->putTStr(i, st_);
    }
    v->close();
}//____________________________________________________

void dumpWiki(const char *path) {
    string str = path;
    string text = "";
    vector<string> v;
    readText(v, str);
    int start = 0;
    string st;
    for (int i = 0; i < v.size(); i++) {
        if (i % 100000 == 0)
            //cout << i << ".";
        if (start) {
            st += v[i] + "\n";
            if (v[i].find("</page>") != -1) {
                if (st.find("{{-pi-}}") != -1) {
                    text += st;
                }
                st = "";
                start = 0;
            }
        }
        if (v[i].find("<page>") != -1) {
            st += v[i] + "\n";
            start = 1;
        }
    }
    
    str += ".txt";
    writeText(text, str);
}//____________________________________________________

void exportDictFromTextDB(const char *db, const char *path) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    GVector *v;
    string db_ = db;
    longMemory->loadTable(db_);
    v = longMemory->table[db].data;
    map<string, int> idMap;
    string str;
    map<string, string> rusMap;
    
    for (int i = 0; i < v->size(); i++) {
        if (i % 10000 == 0)
            cout << " " << i;
        TString st;
        v->getTStr(i, st);
        if (st.size() < 10)
            continue;
        str = st[0];
        idMap[str] = i;
        str = st[3]; // ArchiveDate
        if (st[3] == "NULL") {
            //ищем русский перевод
            str = st[6];
            if (str.find("а") != -1 || str.find("е") != -1 || str.find("и") != -1 ||
                str.find("о") != -1 || str.find("у") != -1 || str.find("я") != -1 ||
                str.find("ю") != -1) {
                rusMap[st[1]] = str;
            }
        }
    }
    cout << endl << "done build map" << endl;
    string text = "";
    
    for (int i = 0; i < v->size(); i++) {
        
        if (i % 10000 == 0)
            cout << " " << i;
        TString st;
        v->getTStr(i, st);
        if (st.size() < 10)
            continue;
        
        if (st[1] == "NULL") { //если у строки нет родительской строки
            // 1/ проверяем является ли строка строкой Пали
            str = st[6];
            if (isPali(str)) {
                text += "[[" + str + "\t@@@" + rusMap[st[0]] + "]]\n";
            }
        }
    }
    cout << endl << "done" << endl;
    writeText(text, path);
}

// Dictionary functions
void translateLine(pageRecLib &page) {
    //void *result_, const char *line, int *intBuff
    string stData = "";
    stData = page.text;
    //print_( stData );
    if (stData == "") {
        cpstr(page ,"emptyLine");
        return;
    }
    if (!inputData.init) initCommandData();
    inputData.data["ln"]=page.ln;
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    logicProcessor->lineTextTranslation(stData, page.mode);
    cpstr(page,stData);
}//____________________________________________________

void dictionaryReport(pageRecLib &page) {
    //void *result_, char *buff, int *intBuff
    if (!inputData.init)
        initCommandData();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    string str = page.text;
    logicProcessor->writeDictReportLocal(str);
    cpstr(page,str);
}

void dictionaryReportMain(pageRecLib &page) {
    page.mode=MAIN_REPORT;
    dictionaryReport(page);
    string str=page.data;
    page.data="";
    translateLine(page);
    str+="<hr/>";
    str+=page.data;
    page.data="";
    cpstr(page,str);
}

void dictionaryReportTitle(pageRecLib &page) {
    //const char *line, const char *ln
    // return "";
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    inputData.data["ln"] = page.ln;
    string text = page.text;
    
    string dbTitle = "DHARMABOOK_Title";
    string db = "DHARMABOOK";
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    
    vector<TString> r;
    vector<int> f;
    f.push_back(0);
    longMemory->select(dbTitle, f, text, HASH_SEARCH, r, 1);
    //cout<<r.size()<<endl;
    ostringstream out;
    vector<string> report;
    report.resize(20);
    int intBuff[10];
    intBuff[5]=-1;
    intBuff[6]=MAIN_REPORT;
    
    if (r.size()) {
        TString s = r[0];
        s.reloadPtr();
        vector<int> res;
        s.getIntVector(res, 1);
        //cout<<" ->> "<<s[0]<<" "<<str.size()<<" "<<res[0];
        
        for (int i = 0; i < res.size(); i++) {
            TString st;
            v->getTStr(res[i], st);
            string t = st[1];
            t = str_replace("[[Category:Dictionary]]¶", "", t);
            //re2::RE2::Replace(&t, "\\[\\[Title:[^¶]*¶", "");
            //re2::RE2::Replace(&t, "\\[\\[Category:Dictionary__\\[([^¶]*)\\]\\]¶","[\\1\n");
            //re2::RE2::Replace(&t, "\\[\\[Category:Dictionary__([^¶]*)\\]\\]¶","[\\1]\n");
            t = str_replace("[[Category:Dictionary__GR]]", "[GR]\n", t);
            t = str_replace("[[Category:Dictionary__GRM]]", "[GRM]\n", t);
            t = str_replace("<br>", "<br/>", t);
            string dict = substr(0, (uint)t.find("\n"), t);
            t = substr((uint)t.find("\n") + 1, (uint)t.size(), t);
            if (dict.find("[TT]") != -1 || dict.find("[DK]") != -1 ||
                dict.find("[DN]") != -1) {
                pageRecLib rec;
                rec.text=t;
                rec.ln=page.ln;
                translateLine(rec);
                t = rec.data;
                rec.data="";
            }
            
            // out<<renderPage(t.c_str());
            out << t
            << "<table width=\"100%\"><tr><td width=\"90%\"><hr/></td><td "
            "width=\"128px\"><div align=\"right\"><font color=\"#cccccc\">  "
            << dict;
            out << "</font><a href=\"/?field=1&index=" << res[i]
            << "&ocrData=read\" target=\"_blank\"><font color=\"#cccccc\"> • "
            "edit</font></a></div></td></tr></table>\n";
            t = out.str();
            out.str("");
            t = str_replace("<br/><br/>", "<br/>", t);
            
            if (dict.find("[MG]") != -1) {
                report[0] = t;
                continue;
            }
            if (dict.find("[HP]") != -1) {
                report[1] = t;
                continue;
            }
            if (dict.find("[TD]") != -1) {
                report[2] = t;
                continue;
            }
            if (dict.find("[MG]") != -1) {
                report[3] = t;
                continue;
            }
            if (dict.find("[MV]") != -1) {
                report[4] = t;
                continue;
            }
            if (dict.find("[VD]") != -1) {
                report[5] = t;
                continue;
            }
            if (dict.find("[ER]") != -1) {
                report[6] = t;
                continue;
            }
            if (dict.find("[RE]") != -1) {
                report[7] = t;
                continue;
            }
            if (dict.find("[SD]") != -1) {
                report[8] = t;
                continue;
            }
            if (dict.find("[IW]") != -1) {
                report[9] = t;
                continue;
            }
            if (dict.find("[JW]") != -1) {
                report[10] = t;
                continue;
            }
            if (dict.find("[TT]") != -1) {
                report[11] = t;
                continue;
            }
            if (dict.find("[DK]") != -1) {
                report[12] = t;
                continue;
            }
            if (dict.find("[BB]") != -1) {
                continue;
            }
            if (dict.find("[TS]") != -1) {
                continue;
            }
            if (dict.find("[TR]") != -1) {
                continue;
            }
            report.push_back(t);
        }
    }
    string delimenter = "";
    text = implode(delimenter, report);
    
    cpstr(page,text);
}//____________________________________________________

void reloadDictionary(pageRecLib &page) {
    //void *result_, const char *buff, int *intBuff
    string key = page.text;
    if (key == "") return;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    inputData.data["ln"] = page.ln;
    vector<string> newKey;
    explode("[", key, newKey);
    map<string, string> lineKey;
    // int flagLang=0;
    string db;
    string ln_ = page.ln;
    string dictLn = "tib";
    if (key.find("་") == -1)
        dictLn = "skt";
    
    if (dictLn == "tib") {
        if (ln_ == "rus")
            db = "translationDictRus";
        if (ln_ == "eng")
            db = "translationDictEng";
        if (ln_ == "")
            db = "translationDictRus";
    } else {
        if (ln_ == "rus")
            db = "translationSktDictRus";
        if (ln_ == "eng")
            db = "translationSktDictEng";
        if (ln_ == "")
            db = "translationSktDictRus";
    }
    
    for (int i = 0; i < newKey.size(); i++) {
        string line = str_replace("]", "", newKey[i]);
        line = str_replace("/", "#", line); //разделителем в текстовой словарной
        //статье может быть знак # или /
        if (line == "*")
            line = "";
        vector<string> keyLine;
        explode("#", line, keyLine);
        if (keyLine.size() != 2)
            continue;
        string key = keyLine[0];
        key = str_replace(" ", "", key);
        if (dictLn == "tib" && key.find("་") != -1) {
            
            key += "་";
            key = str_replace("་་", "་", key);
            key = str_replace("་་", "་", key);
            key += "@" + key;
            key = str_replace("@་", "", key);
            key = str_replace("@", "", key);
        }
        lineKey[keyLine[0]] = keyLine[1] + "@"; //маркировка новой записи
    }
    
    int flagNewRecord = 0;
    GVector *dk;
    GMap *d;
    
    
    
    longMemory->loadTable(db);
    dk = longMemory->table[db].data;
    indexRecord *indexRec =
    longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
    d = indexRec->mIndex;
    
    ostringstream out;
    map<string, string>::iterator it;
    for (it = lineKey.begin(); it != lineKey.end(); it++) {
        string key = it->first;
        ulong index = d->getHKey(key, 0);
        TString st;
        if (it->second == "@"){
            //по запросу стираем запись
            st.push_back("");
            st.push_back("");
        }else{
            st.push_back(key);
            out << key << "-->" << it->second << endl;
            st.push_back(it->second);
        }
        if (index == 0xffffffffffffffff) {
            flagNewRecord = 1;
            // dk->push_back(st);
            d->addRecord(st);
            
        } else {
            dk->putTStr(index, st);
        }
    }
    // if(flagNewRecord)longMemory->reloadIndex(longMemory->table[db], 0,
    // HASH_SEARCH);
    out << "done save dictionary";
    string result = out.str();
    print_(result <<" db:"<<db)
    cpstr(page,result);
}//____________________________________________________

void extractCategory(string &text, map<string, string> &pageMap) {
    //cout<<"extract"<<endl;
    vector<string> page;
    explode("{{", text, page);
    string str;
    string report = "";
    for (int i = 0; i < page.size(); i++) {
        str = page[i];
        if (str.find("|") != -1 && str.find("}}") != -1) {
            string line = substr(0, (uint)str.find("}}"), str);
            vector<string> v;
            explode("|", line, v);
            if (pageMap[v[0]] == "" || pageMap[v[0]] == v[1]) {
                pageMap[v[0]] = v[1];
            } else {
                //cout<<"not valid key "<<v[0]<<" in record
                // "<<pageMap[v[0]]<<endl<<str<<endl;
                continue;
            }
            report += substr((uint)str.find("}}") + 2, (uint)str.size(), str);
        } else {
            report += str;
        }
    }
    for (map<string, string>::iterator it = pageMap.begin(); it != pageMap.end();
         it++) {
        report += "{{" + it->first + "|" + it->second + "}}\n";
    }
    text = report;
}//____________________________________________________

void replaceInDB(const char *db) {
    string str = db;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    string ret = "0";
    ret[0] = 11;
    //cout << v->size();
    for (int i = 0; i < v->size(); i++) {
        if (i % 10000 == 0)
            cout << " " << i;
        TString st;
        v->getTStrData(i, st);
        string key = st[1];
        key = st[1];
        if (key.find("Category:Taishō") == -1)
            continue;
        
        key = st[0];
        
        // key=str_replace("}","]",key);
        // key=str_replace("¶","\n",key);
        // re2::RE2::Replace(&key,"\\[\\[Path[^\\]].*\\]\\]","");
        // key=str_replace("\n","",key);
        
        TString st_;
        st_.push_back(key);
        for (int n = 1; n < st.size(); n++) {
            key = st[n];
            // key=str_replace("Dharmadownload.net","Dharmadownload.org",key);
            // key=str_replace(" ¶།¶"," ¶།",key);
            if (n == 1) {
                // key=str_replace("\n","¶",key);
                // re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\|([^\\]]*)\\]\\]",
                // "[[Title:\\1|\\2]]¶[[Category:\\1]]¶[[Category:\\2]]");
                // re2::RE2::Replace(&key, "(.*)\\{\\{Path\\|([^\\}]+)\\}\\}(.*)",
                // "{{Path|\\2}}\\1\\3");
                key = str_replace("Category:creator_", "Category:creator__", key);
            }
            
            /*
             if(n==1){
             if(st.size()>3){
             string s;
             s=st[(uint)st.size()-2];
             s=str_replace("¶","\n",s);
             vector<string>lines=explode("\n",s);
             for(int m=0;m<lines.size();m++){
             if(lines[m].find("[[")==0){
             key=lines[m]+"\n"+key;
             lines[m]="";
             }
             }
             s=implode("\n",lines);
             st[(uint)st.size()-2]=s;
             }
             }
             */
            // key=str_replace("\r","\n",key);
            // key=str_replace(" \n","\n",key);
            // key=str_replace("་ ","་",key);
            // key=str_replace("<","&lt;",key);
            // key=str_replace("<","&gt;",key);
            // key=str_replace("{{Title|","{{a_Title|",key);
            // key=str_replace("{{a_title","{{a_Title",key);
            // if(n==1)key=str_replace("¶","\n",key);
            
            // key=str_replace(ret.c_str(),"\n",key);
            // key=str_replace("}} & {{","}}\n{{",key);
            
            // key=str_replace("{{Q_COPY|{{copy","{{Q_COPY",key);
            // key=str_replace("{{P_TBRC_CODE|{{tcode","{{P_TBRC_CODE",key);
            // key=str_replace("{{ID:","{{ID|",key);
            // key=str_replace("{{M_VOLUME|{{volume","{{M_VOLUME",key);
            // key=str_replace("{{N_PAGES|{{pages","{{N_PAGES",key);
            // key=str_replace("{{a_title","{{a_Title",key);
            
            // key=str_replace("¶","\n",key);
            // key=str_replace("\n\n","\n",key);
            // key=str_replace("\n\n","\n",key);
            // re2::RE2::Replace(&key,"{{Tib-vol-id-end}","");
            // re2::RE2::Replace(&key,"\\{\\{Tib-vol-id-end(.*)","{{Tib-vol-id-end\\1}");
            // if(n==1)re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\]\\]",
            // "{{a_Title|\\1}}");
            
            // if(key.find("{{A_CODE")!=-1){
            //    map<string,string>pageMap;
            // extractCategory(key,pageMap);
            // key=str_replace("\n\n","\n",key);
            // key=str_replace("\n\n","\n",key);
            // key=str_replace("\n\n","\n",key);
            // key=str_replace("\n\n","\n",key);
            //     key=str_replace("\n\n","\n",key);
            //     key=str_replace("\n\n","\n",key);
            
            //     flag=1;
            // }else{
            //     if(n==1)break;
            // }
            
            // re2::RE2::Replace(&key,"་[ ]*\\n*[ །]*༄༅[ །]*","་");
            // key=str_replace("་\n","་",key);
            // re2::RE2::Replace(&key,"===[^=]+===\\n[ @]+\\n","");
            // key=str_replace("title=","",key);
            // key=str_replace("name=","",key);
            // key=str_replace("copy=","",key);
            
            /// int n=key.find("སམ་སིདྡྷི");
            // if(n!=-1){
            //    n-=100;
            //    if(n<0)n=0;
            //    string s=substr(n,256,key);
            //    //cout<<" i:"<<i<<" key:"<<s<<endl;
            //}
            st_.push_back(key);
        }
        v->putTStr(i, st_);
        // break;
    }
    v->close();
}//____________________________________________________

void importOCRText(const char *db, const char *path) {
    string str = db;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    // v->resizeData(25000, 4100);
    vector<string> fileList;
    string fileData;
    string ext = "txt";
    string path_ = path;
    long time_ = time(NULL);
    readDirectoryToArray(fileList, path_, ext);
    //cout << "fileList.size()=" << fileList.size() << endl;
    for (int index = 0; index < fileList.size(); index++) {
        cout << index << " ";
        readText(fileData, fileList[index]);
        fileData = str_replace("{page", ":|:{page", fileData);
        vector<string> list;
        explode(":|:", fileData, list);
        TString st;
        ostringstream out;
        out << "[[ID:" << time_ << "]]";
        time_++;
        str = out.str();
        st.push_back(str);
        for (int i = 0; i < list.size(); i++) {
            if (i == 0) {
                list[i] = "[[Title:" + fileName(fileList[index]) + "]]\n" + list[i];
            }
            st.push_back(list[i]);
        }
        str = "\n[[Category:Tibetan text]]\n[[Category:tibetan_text]]\n[[Path:" +
        fileName(fileList[index]) + "]]";
        st.push_back(str);
        v->push_back(st);
    }
}//____________________________________________________

void importOCRTextByIndex(pageRecLib &page){
    //const char *db, const char *path, int index
    string str = page.db;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    string filePath=page.text;
    // v->resizeData(25000, 4100);
    string fileData;
    string ext = "txt";
    long time_ = time(NULL);
    cout << "path:" << filePath << endl;
    readText(fileData, filePath);
    fileData = str_replace("{page", ":|:{page", fileData);
    vector<string> list;
    explode(":|:", fileData, list);
    TString st;
    ostringstream out;
    out << "[[ID:" << time_ << "]]";
    time_++;
    str = out.str();
    st.push_back(str);
    for (int i = 0; i < list.size(); i++) {
        if (i == 0) {
            list[i] = "[[Title:" + fileName(filePath) + "]]\n" + list[i];
        }
        st.push_back(list[i]);
    }
    str = "\n[[Category:Tibetan text]]\n[[Category:tibetan_text]]\n[[Path:" +
    fileName(filePath) + "]]";
    st.push_back(str);
    v->putTStr(page.index, st);
}//____________________________________________________

void getReportLine(string &line, int pos) {
    int pos1;
    int pos2;
    int limit = 512;
    string str = substr(pos, limit, line);
    string s;
    if (pos + limit < line.size()) {
        s = substr(pos + limit, limit, line);
        pos1 = (int)s.find(" ");
        pos2 = (int)s.find("\n");
        if (pos2 == -1)
            pos2 = (int)s.find("་");
        if (pos1 == -1)
            pos1 = limit;
        
        if (pos1 < pos2) {
            str += substr(0, pos1, s);
        } else {
            str += substr(0, pos2, s);
        }
    }
    pos1 = pos - limit;
    if (pos1 < 0) {
        s = substr(0, pos, line);
    } else {
        s = substr(pos - limit, limit, line);
    }
    pos1 = (int)s.find(" ");
    pos2 = (int)s.find("\n");
    if (pos1 == -1 || pos2 == -1) {
        pos1 = limit;
        pos2 = (int)s.rfind("་");
    }
    if (pos2 > 0) {
        if (pos1 > pos2) {
            str = substr(pos1, limit, s) + str;
        } else {
            str = substr(pos2, limit, s) + str;
        }
    }
    line = str;
}

void searchDB(pageRecLib &page) {
    // return "";
    // stData=searchDBTable(page,DB_NAME,FULL_TEXT_SEARCH);
    page.mode=FULL_TEXT_SEARCH;
    searchDBTable(page);
}//______________________________________________________________

void searchDBTable(pageRecLib &page) {
    string db = page.db;
    if(db=="")db="DHARMABOOK";
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string key = page.text;
    string textKey=key;
    page.bookCount = 0;
    string text = key;
    string report;
    longMemory->loadTable(db);
    TString st;
    string str;
    // v->getTStr(page.index,st);
    
    //print_("###1@@@ intData:"<<page.intData<<" key:"<<key<<" db:"<<db<<endl;)
    
    ostringstream out;
    if (page.intData == FULL_TEXT_SEARCH || page.intData == ID_SEARCH) {
        setSearchText(key);
    }
    string destString;
    
    vector<TString> r;
    vector<int> f;
    f.push_back(page.field);
    longMemory->select(db, f, key, page.intData, r, 0);
    //cout<<r.size()<<endl;
    for (int i = 0; i < r.size(); i++) {
        TString s = r[i];
        s.reloadPtr();
        //cout<<i<<" ->> "<<s.getInt(0)<<" "<<s.getInt(1)<<" "<<s.getInt(2)<<endl;
        rLink *link;
        string str = s[0];
        link = (rLink *)&str[0];
        TString dataStr;
        longMemory->table[db].data->getTStr(link->index, dataStr);
        
        string st = dataStr[link->field];
        if(st.find(textKey)==-1)continue;
        int offset = link->offset;
        map<string, string> pageMap;
        extractCategory(st, pageMap);
        getReportLine(st, offset);
        renderCategorySearch(page);
        st += page.data;
        page.data="";
        
        st = str_replace("\n<br/>\n", "", st);
        //re2::RE2::Replace(&st, "Title:*", "");
        st = str_replace("{", "", st);
        st = str_replace("}", "", st);
        
        if (pageMap.find("a_Title_bo") != pageMap.end()) {
            st = "Title: " + pageMap["a_Title_bo"] + "<br/>" + st;
        } else if (pageMap.find("a_Title") != pageMap.end()) {
            st = "Title: " + pageMap["a_Title"] + "<br/>" + st;
        }
        ostringstream out_;
        out_ << "<a href=\"/?db=" << page.db
        << "&index=" << link->index << "&field=" << link->field
        << "&ocrData=read\" target=\"_blank\">Book ID:" << link->index
        << " page:" << link->field << "<br/>...<tsh>" << st << "</tsh>....</a><hr/>\n";
        
        destString += out_.str();
        page.bookCount++;
        if (i > 100)
            break;
    }
    text = str_replace("\n", "", text);
    text += ":|:";
    text = str_replace("་:|:", "", text);
    text = str_replace(":|:", "", text);
    str = "<font color=\"blue\">" + text + "</font>";
    destString = str_replace(text, str, destString);
    destString = str_replace("¶", " ", destString);
    
    out.str("");
    out << "<h3>Search result:" << page.bookCount
    << "</h3><hr/"
    ">\n<style>a{text-decoration:none;color:black;font-family:Yagpo Tibetan Uni}</"
    "style>\n"
    << endl;
    str = out.str();
    
    string stData = str + destString;
    stData+="\n<script type=\"text/javascript\" src=\"/_js/grammar.js\"></script>";
    cpstr(page,stData);
}//____________________________________________________

void joinFieldInTable(const char *dbName, int filedNameIndex) {
    string db = dbName;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    TString stFieldName;
    v->getTStr(filedNameIndex, stFieldName);
    
    for (int i = 0; i < v->size(); i++) {
        //cout << i << " ";
        TString st;
        v->getTStr(i, st);
        TString st_;
        string str;
        for (int n = 0; n < st.size() && n < stFieldName.size(); n++) {
            str += "===" + stFieldName[n] + "===\n" + st[n] + "\n";
        }
        st_.push_back("_");
        st_.push_back(str);
        v->putTStr(i, st_);
    }
}//____________________________________________________

void loadLibraryPathInCatalog(const char *db) {
    string catalog = db;
    string path = "LIBRARY_PATH";
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(catalog);
    longMemory->loadTable(path);
    //GVector *vCatalog = longMemory->table[catalog].data;
    GVector *vPath = longMemory->table[path].data;
    string res;
    for (int i = 0; i < vPath->size(); i++) {
        if (i % 1000 == 0)
            cout << " " << i;
        string str;
        vPath->getStr(i, str);
        string name = fileName(str);
        string code;
        string file;
        if (name == "_mark" || name == "mark" || name == "_press" ||
            name.find("_files") != -1 || name.find("_files") != -1)
            continue;
        //
        //    if (re2::RE2::PartialMatch(name, "R[\\d]{10}")) {
        //      code = name;
        //      re2::RE2::Replace(&code, ".*_R([\\d]{10})", "\\1");
        //     re2::RE2::Replace(&name, "_R[\\d]{10}", "");
        // //cout<<name<<"->"<<code<<endl;
        // name="===FILE_NAME===\n"+name+"\n";
        // code="{{A_CODE|"+code+"}}";
        //      for (int n = 0; n < vCatalog->size(); n++) {
        //        TString st_;
        //        TString st1;
        //        vCatalog->getTStr(n, st_);
        //        if (st_[1].find(code) != -1) {
        //          st1.push_back(code);
        //          name = st_[1] + "\n{{Path|" + str + "}}";
        //          st1.push_back(name);
        //          vCatalog->putTStr(n, st1);
        //         break;
        //       }
        //      }
        //  }
        // //cout<<name<<endl;
        // res+=str+"\n";
    }
    // string s="/_Image2OCR/path.txt";
    // writeText(res, s);
}//____________________________________________________

void extractMap(string &text, map<string, string> pageMap) {
    text = str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/",
                       "/___DHARMA___/___LIBRARY___/", text);
    
    string str;
    string report;
    string reportStr;
    //рендер шаблонов
    vector<string> page;
    explode("{{", text, page);
    for (int i = 0; i < page.size(); i++) {
        str = page[i];
        if (str.find("|") != -1 && str.find("}}") != -1) {
            string line = substr(0, (uint)str.find("}}"), str);
            vector<string> v;
            explode("|", line, v);
            if (pageMap.find(v[0]) != pageMap.end())
                continue;
            pageMap[v[0]] = v[1];
            str = substr((uint)str.find("}}") + 2, (uint)str.size(), str);
            reportStr += str_replace("\n", "<br/>\n", str);
        } else {
            reportStr += str_replace("\n", "<br/>\n", str);
        }
    }
}

static bool sortL(const string &d1, const string &d2) {
    return d1.size() > d2.size();
}//____________________________________________________

void exportWordList(const char *path) {
    if (path == NULL)
        return;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    vector<string> dict;
    string str = "mainDict";
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    for (int i = 0; i < v->size(); i++) {
        TString st;
        v->getTStr(i, st);
        dict.push_back(st[0]);
    }
    str = "translationDictRus";
    longMemory->loadTable(str);
    v = longMemory->table[str].data;
    for (int i = 0; i < v->size(); i++) {
        TString st;
        v->getTStr(i, st);
        dict.push_back(st[0]);
    }
    sort(dict.begin(), dict.end(), sortL);
    str = path;
    writeText(dict, str);
}//____________________________________________________

void sortWordList(const char *path) {
    if (path == NULL)
        return;
    vector<string> dict;
    string str = path;
    readText(dict, str);
    vector<string> dict_;
    string key;
    string report;
    
    for (int i = 0; i < dict.size(); i++) {
        if (i % 100000 == 0)
            cout << " " << i / 100000;
        string s = dict[i];
        setSearchText(s);
        // vector<string>v=explode("\n",s);
        // dict_.insert(dict_.end(), v.begin(), v.end());
        // report+=s+"\n";
        // dict[i]=s;
        dict_.push_back(s);
    }
    cout << "start sort" << endl;
    sort(dict_.begin(), dict_.end());
    dict.resize(0);
    dict.push_back(dict_[0]);
    for (int i = 1; i < dict_.size(); i++) {
        if (dict_[i] != dict_[i - 1])
            dict.push_back(dict_[i]);
    }
    sort(dict.begin(), dict.end(), sortL);
    str += "_.txt";
    writeText(dict, str);
}//____________________________________________________

void exportDBTable(const char *db) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = db;
    string ext = "txt";
    longMemory->exportTable(str, ext);
}//____________________________________________________

void importDBTable(const char *db, const char *path) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = db;
    string path_ = path;
    longMemory->importTable(str, path_);
}//____________________________________________________

void importOCR(const char *db, const char *path, const char *delimeter) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = db;
    string path_ = path;
    string delimeter_ = delimeter;
    longMemory->importOCR(str, path_, delimeter_);
}//____________________________________________________

void importTextFolder(const char *db, const char *path, const char *category) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = db;
    string path_ = path;
    string category_ = category;
    longMemory->importTextFolder(str, path_, category_);
}//____________________________________________________

void importText(pageRecLib &page) {
    //const char *db, const char *path, int index
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = page.db;
    string path = page.text;
    longMemory->importText(str, path, page.index);
}

void exportText(pageRecLib &page) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string db = page.db;
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    TString st;
    v->getTStr(page.index, st);
    string text;
    for (int i = 1; i < st.size(); i++) {
        text += st[i];
    }
    cpstr(page,text);
}//____________________________________________________

void importDictTab(pageRecLib &page) {
    //const char *db, const char *path, const char *category, int year
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = page.db;
    string path_ = page.text;
    string category_ = page.data;
    longMemory->importDictTab(str, path_, category_, page.intData);
}//____________________________________________________

void importMainDictionary() {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string db = "DHARMABOOK";
    string dbDict = "mainDict";
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    longMemory->loadTable(dbDict);
    GVector *vDict = longMemory->table[dbDict].data;
    long t = time(NULL);
    t -= 60 * 60 * 24 * 10; // 10 years ago;
    string str;
    
    for (int i = 0; i < vDict->size(); i++) {
        if (i % 10000 == 0)
            cout << " " << i;
        TString st;
        vDict->getTStr(i, st);
        if (st.size() < 3) {
            cout << i << " " << st.size() << st[3] << endl;
            continue;
        }
        
        TString st_;
        ostringstream out;
        out << "[[ID:" << t << "]]";
        str = out.str();
        st_.push_back(str);
        t++;
        
        if (st[2].size() > 12) {
            //cout<<"  "<<st.size()<<"    "<<st[0]<<"//// "<<st[1]<<"////
            // "<<st[2]<<"//// "<<st[3]<<endl;
            str = "[[Title:" + st[0] + "]]¶";
            str += "[[Category:Dictionary]]¶";
            str += "[[Category:Dictionary__PL]]¶";
            for (int n = 1; n < st.size(); n++)
                str += st[n];
            str = str_replace("]]]", "] ]]", str);
            st_.push_back(str);
        } else {
            str = "[[Title:" + st[0] + "]]¶";
            str += "[[Category:Dictionary]]¶";
            if (st[2] != "") {
                str += "[[Category:Dictionary__" + st[2] + "]]¶";
            } else {
                str += "[[Category:Dictionary__TR]]¶";
            }
            str += st[1];
            str = str_replace("]]]", "] ]]", str);
            st_.push_back(str);
        }
        v->push_back(st_);
    }
}//____________________________________________________

void checkDublicateID(const char *db) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string str = db;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    map<string, int> cMap;
    for (int i = 0; i < v->size(); i++) {
        TString st;
        v->getTStr(i, st);
        str = st[0];
        if (cMap.find(str) != cMap.end()) {
            //cout<<str<<" i:"<<i<<" i_:"<<cMap[str]<<endl;
            /*
             int index1;
             int index2;
             if(i<cMap[str]){
             index1=i;
             index2=cMap[str];
             }else{
             index2=i;
             index1=cMap[str];
             }
             TString st1;
             v->getTStr(index1,st1);
             TString st2;
             v->getTStr(index2,st2);
             for(int n=1;n<st2.size();n++){
             str=st2[n];
             st1.push_back(str);
             }
             v->putTStr(index1,st1);
             TString st3;
             st3.push_back("");
             st3.push_back("");
             v->putTStr(index2,st3);
             */
            
        } else {
            cMap[str] = i;
        }
    }
}//____________________________________________________

void savePageInDB(pageRecLib &page) {
    //const char *db, const char *text, int index, int field
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string db = page.db;
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    string text_ = page.text;
    //text_ = str_replace("\n", "¶", text_);
    //cout<<"save page:"<<index<<" field:"<<field<<" text:"<<text_<<endl;
    TString st;
    v->getTStr(page.index, st);
    TString st_;
    string str;
    for (int i = 0; i < st.size(); i++) {
        if (i == page.field) {
            st_.push_back(page.text);
        } else {
            str = st[i];
            st_.push_back(str);
        }
    }
    v->putTStr(page.index, st_);
    cpstr(page,"done save page");
}//____________________________________________________

void rebuildCategory() {
    string str;
    string report;
    string reportStr;
    ostringstream out;
    
    //рендер содержания
    str = "DHARMABOOK";
    string path = inputData.data["root"] +
    "OCRData/DATA_DB/DHARMABOOK_Category/DHARMABOOK_Category.bin";
    remove(path.c_str());
    path = inputData.data["root"] +
    "OCRData/DATA_DB/DHARMABOOK_Category/DHARMABOOK_Category_0_1.index";
    remove(path.c_str());
    
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    longMemory->loadTable(str);
    GVector *v = longMemory->table[str].data;
    str = "DHARMABOOK_Category";
    longMemory->loadTable(str);
    GVector *c = longMemory->table[str].data;
    string lt = "[[Category:";
    uint l = (uint)lt.size();
    map<string, string> cMap;
    ulong size = v->size();
    
    for (uint index = 0; index < size; index++) {
        
        TString st;
        v->getTStr(index, st);
        if (index % 100000 == 0)
            cout << " " << index;
        
        str = st[1];
        str = str_replace("[[Category:", "\n[[Category:", str);
        str = str_replace("]]", "]]\n", str);
        vector<string> lineArray;
        explode("\n", str, lineArray);
        for (int n = 0; n < lineArray.size(); n++) {
            str = lineArray[n];
            if (lineArray[n].find("[[Category:") == 0 && str.find("]]") != -1) {
                string line = substr(l, (uint)str.find("]]") - l, str);
                TString st;
                //cout<<line<<" --> "<<index<<endl;
                string s_;
                s_.resize(4);
                memcpy(&s_[0], &index, 4);
                cMap[line] += s_;
            }
        }
    }
    map<string, string>::iterator it;
    
    for (it = cMap.begin(); it != cMap.end(); it++) {
        TString st;
        string s = it->first;
        //cout << it->first << " --> " << it->second.size() << endl;
        
        st.push_back(s);
        st.push_back((char *)&it->second[0], (uint)it->second.size());
        c->push_back(st);
    }
}

void searchByTag(pageRecLib &page) {
    //const char *db_, const char *tag, const char *request, int mode
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    ostringstream out;
    string key = page.text;
    
    GVector *v, *v_;
    GMap *d;
    string db;
    string dbIndex;
    string str, report;
    string tag=page.ln;
    
    // load main db
    db = page.db;
    longMemory->loadTable(db);
    v = longMemory->table[db].data;
    
    // load index db
    dbIndex = page.db;
    dbIndex += "_";
    dbIndex += tag;
    
    longMemory->loadTable(dbIndex);
    v_ = longMemory->table[dbIndex].data;
    if (!v_->size()) { // build index
        string tagLine = "[[";
        tagLine += tag;
        tagLine += ":";
        uint l = (uint)tagLine.size();
        map<string, string> cMap;
        
        for (uint index = 0; index < v->size(); index++) {
            
            TString st;
            v->getTStr(index, st);
            
            str = st[1];
            str = str_replace("[[", "\n[[", str);
            str = str_replace("]]", "]]\n", str);
            vector<string> lineArray;
            explode("\n", str, lineArray);
            for (int n = 0; n < lineArray.size(); n++) {
                str = lineArray[n];
                if (lineArray[n].find(tagLine) == 0 && str.find("]]") != -1) {
                    string line = substr(l, (uint)str.find("]]") - l, str);
                    TString st;
                    //cout<<line<<" --> "<<index<<endl;
                    string s_;
                    s_.resize(4);
                    memcpy(&s_[0], &index, 4);
                    cMap[line] += s_;
                }
            }
        }
        map<string, string>::iterator it;
        
        for (it = cMap.begin(); it != cMap.end(); it++) {
            TString st;
            string s = it->first;
            //cout<<it->first<<" --> "<<it->second.size()<<endl;
            st.push_back(s);
            st.push_back((char *)&it->second[0], (uint)it->second.size());
            v_->push_back(st);
        }
    }
    
    indexRecord *indexRec =
    longMemory->createIndex(longMemory->table[dbIndex], 0, page.mode);
    d = indexRec->mIndex;
    
    vector<TString> r;
    vector<int> f;
    f.push_back(0);
    longMemory->select(dbIndex, f, key, page.mode, r, 1);
    //cout<<r.size()<<endl;
    
    if (page.mode == HASH_SEARCH) {
        
        if (r.size()) {
            TString s = r[0];
            s.reloadPtr();
            vector<int> res;
            s.getIntVector(res, 1);
            //cout<<" ->> "<<s[0]<<" "<<str.size()<<" "<<res[0];
            
            for (int i = 0; i < res.size(); i++) {
                TString st;
                v->getTStr(res[i], st);
                string t = st[1];
                t = str_replace("¶", "<br/>\n", t);
                out << t;
                out << "<a href=\"/?field=1&index=" << res[i]
                << "&ocrData=read\" target=\"_blank\"> • edit</a><hr/>";
            }
            
            //cout<<i<<" ->> "<<s.getInt(0)<<" "<<s.getInt(1)<<"
            // "<<s.getInt(2)<<endl; rLink *link; string str=s[0];
            // link=(rLink*)&str[0];
            // TString dataStr;
            // v_->getTStr(link->index,dataStr);
            // string st=dataStr[1];
        }
    }
    
    string stData = out.str();
    cpstr(page,stData);
}

// render DB report for request category
void searchCategory(pageRecLib &page) {
    //void *result_, char *request, int indexPage, int &pageCount
    
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string key = page.text;
    if(key=="")key=inputData.data["category"];
    GVector *dk, *dBook;
    GMap *d;
    string db = "DHARMABOOK_Category";
    string str, report;
    longMemory->loadTable(db);
    dk = longMemory->table[db].data;
    //print_("size:"<<dk->size()<<" key:"<<key<<endl;)
    
    indexRecord *indexRec = longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
    d = indexRec->mIndex;
    
    int index = (uint)d->getHKey(key, 0);
    //cout<<"index:"<<index<<endl;
    ostringstream out;
    
    if (index != -1) {
        string titleRequest = "CategoryPage_";
        titleRequest += page.text;
        // search main DB for this category front page.
        if (page.index == 0) {
            pageRecLib rec;
            string db="DHARMABOOK";
            rec.db=&db[0];
            string ln="Title";
            rec.ln=&ln[0];
            rec.text=&titleRequest[0];
            rec.mode=HASH_SEARCH;
            searchByTag(rec);
            out << rec.data;
            rec.data="";
        }
        
        TString st;
        dk->getTStr(index, st);
        vector<int> v;
        st.getIntVector(v, 1);
        //cout<<"@@@ init v.size():"<<v.size()<<endl;
        db = "DHARMABOOK";
        longMemory->loadTable(db);
        dBook = longMemory->table[db].data;
        page.pageCount = (uint)v.size() / 100 + 1;
        out << "\n<h3>Category: " << key << " Total: " << v.size()
        << " records.</h3><hr/>";
        
        //cout<<"@ init pageCount:"<<page.pageCount<<" v.size():"<<v.size()<<endl;
        
        for (int i = page.field*100; i < v.size() && i < page.field*100 + 100; i++) {
            TString st;
            dBook->getTStr(v[i], st);
            if (st.size() > 1) {
                str = st[1];
                out << "<a href=\"/?field=1&index=" << v[i]
                << "&ocrData=read\" target=\"_blank\">";
                out << "Open BookID:" << v[i] << "<br/>";
                string s = substr(0, (uint)str.find(" ", 1024), str);
                if (s.size() > 2048)
                    s = substr(0, (uint)str.find("་", 1024), str);
                if (s.size() > 2048)
                    s = substr(0, 1024, str);
                s = str_replace("<", "{", s);
                s = str_replace(">", "}", s);
                pageRecLib rec;
                rec.text=s;
                renderCategoryText(rec);
                s=rec.data;
                rec.data="";
                s = str_replace("<hr/>", " ", s);
                s = str_replace("<br/>", " ", s);
                out << s << "</a><hr/>\n";
            }
        }
    }
    str = out.str();
    //str = str_replace("¶", " ", str);
    cpstr(page,str);
}//____________________________________________________

void setCategory(const char *key_, const char *category_) {
    
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    string key = key_;
    string category = category_;
    GVector *dBook;
    string db = "DHARMABOOK";
    string str, report;
    
    longMemory->loadTable(db);
    dBook = longMemory->table[db].data;
    for (int i = 0; i < dBook->size(); i++) {
        if (i % 100 == 0)
            cout << " " << i;
        TString st;
        TString st1;
        dBook->getTStr(i, st);
        if (st.size() > 1) {
            str = st[1];
            if (str.find(key) != -1) {
                str = "[[Category:" + category + "]]¶" + str;
                string s = st[0];
                st1.push_back(s);
                st1.push_back(str);
                for (int n = 2; n < st.size(); n++) {
                    string s = st[n];
                    st1.push_back(s);
                }
                dBook->putTStr(i, st1);
            }
        }
    }
}//____________________________________________________

void listCategory(pageRecLib &page) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    GVector *cV;
    string db = "DHARMABOOK_Category";
    string str;
    longMemory->loadTable(db);
    
    cV = longMemory->table[db].data;
    ostringstream out;
    multimap<int, string> cMap;
    map<string, int> dMap;
    for (uint i = 0; i < (uint)cV->size(); i++) {
        TString st;
        cV->getTStr(i, st);
        vector<int> v;
        st.getIntVector(v, 1);
        if (st[0].find("__") != -1) { // if this enry is subcategory
            string up = st[0];
            up = substr(0, (uint)up.find("__"), up);
            dMap[up]++;
            continue;
        }
        cMap.insert(pair<int, string>(v.size(), st[0]));
    }
    
    multimap<int, string>::iterator it;
    it = cMap.end();
    
    while (1) {
        if (it->second.size()) {
            string link = it->second;
            encodeURL(link);
            //cout<<"link:"<<it->second<<endl;
            
            out << "<a class=\"content\" href=\"/?text=" << link
            << "&index=0&ocrData=readCategory\" target=\"_blank\">" << it->second
            << "</a> " << it->first << " records";
            if (dMap.find(it->second) != dMap.end()) {
                out << ", " << dMap[it->second] << " category";
            }
            out << "<br>\n";
        }
        if (it == cMap.begin())
            break;
        it--;
    }
    string result = out.str();
    
    cpstr(page,result);
}//____________________________________________________

void renderCategoryText(pageRecLib &page) {
    //const char *pageText, int index
    string str = page.text;
    ostringstream out;
    ostringstream out_;
    string reportStr = "";
    string report = "";
    string render = "";
    
    //рендер категорий
    out_ << "<hr/>";
    str = str_replace("[[", "\n[[", str);
    str = str_replace("]]", "]]\n", str);
    str = str_replace("¶", "\n", str);
    vector<string> lineArray;
    explode("\n", str, lineArray);
    for (int n = 0; n < lineArray.size(); n++) {
        str = lineArray[n];
        if (lineArray[n].find("[[Category:") == 0 && str.find("]]") != -1) {
            string line = substr(11, (uint)str.find("]]") - 11, str);
            string link = line;
            encodeURL(link);
            out_ << "<a class=\"content\" href=\"/?text=" << link
            << "&ocrData=readCategory\">" << line << "</a>|";
        }
    }
    out_ << "<hr/>";
    //рендер шаблонов
    string text = page.text;
    text = str_replace("[[", "{{", text);
    text = str_replace("]]", "}}", text);
    
    vector<string> pageV;
    explode("{{", text, pageV);
    map<string, string> pageMap;
    for (int i = 0; i < pageV.size(); i++) {
        str = pageV[i];
        if (str.find("}}") != -1 && (str.find("|") != -1 || str.find(":") != -1)) {
            string line = substr(0, (uint)str.find("}}"), str);
            if (str.find("|") != -1) {
                vector<string> v;
                explode("|", line, v);
                if (v.size() > 1)
                    pageMap[v[0]] += v[1];
            } else {
                vector<string> v;
                explode(":", line, v);
                if (v.size() > 1)
                    pageMap[v[0]] += v[1];
            }
            
            str = substr((uint)str.find("}}") + 2, (uint)str.size(), str);
            reportStr += str_replace("\n", "<br/>", str);
        } else {
            reportStr += str_replace("\n", "<br/>", str);
        }
    }
    if (pageMap.find("Path") != pageMap.end()) {
        str = pageMap["Path"];
        if (str.find("/Volumes") != -1) {
            str = str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/",
                              "/___DHARMA___/___LIBRARY___/", str);
            str = str_replace("/Volumes/TERSAR_3TB/__LIBRARY_CANON_ALL/", "/CANON/",
                              str);
            
            report =
            "<a href=\"https://www.buddism.ru/" + str +
            "\" target=\"_blank\" class=\"lib\">Open text in Library</a><br/>" +
            report;
        }
    }
    /*
     if(pageMap.find("Title")!=pageMap.end()){
     string s=pageMap["Title"];
     if(isPali(s)||s.find("་")!=-1){
     report+="<tb>Title: <tsh>"+pageMap["Title"]+"</tsh></tb><br/>";
     }else{
     report+="<tb>Title: "+pageMap["Title"]+"</tb><br/>";
     }
     }
     */
    if (pageMap.find("a_Title_bo") != pageMap.end()) {
        report += "<tb>Title: " + pageMap["a_Title_bo"] + "</tb><br/>";
    } else if (pageMap.find("title_pali") != pageMap.end()) {
        report += "<tb>Title Pali: " + pageMap["title_pali"] + "</tb><br/>";
    }
    if (pageMap.find("title_eng") != pageMap.end()) {
        report += "<tb>Title: " + pageMap["title_eng"] + "</tb><br/>";
    }
    if (pageMap.find("title_rus") != pageMap.end()) {
        report += "<tb>Название: " + pageMap["title_rus"] + "</tb><br/>";
    }
    if (pageMap.find("info_eng") != pageMap.end()) {
        report += "<tb>" + pageMap["info_eng"] + "</tb><br/>";
    }
    if (pageMap.find("info_rus") != pageMap.end()) {
        report += "<tb>" + pageMap["info_rus"] + "</tb><br/>";
    }
    if (pageMap.find("chapter_code") != pageMap.end()) {
        report += "<code>Code: " + pageMap["chapter_code"] + "</code><br/>";
    }
    if (pageMap.find("info_rus") != pageMap.end()) {
        // report+="<td>"+pageMap["info_rus"]+"</td><br/>";
    }
    
    for (map<string, string>::iterator it = pageMap.begin(); it != pageMap.end();
         it++) {
        // it->first = key
        
        if (it->second == "")
            continue;
        if (it->first == "OSBL")
            continue;
        if (it->first == "ID")
            continue;
        if (it->first == "title_pali")
            continue;
        if (it->first == "title_eng")
            continue;
        if (it->first == "title_rus")
            continue;
        if (it->first == "chapter_code")
            continue;
        if (it->first == "info_eng")
            continue;
        if (it->first == "info_rus")
            continue;
        if (it->first == "Title")
            continue;
        if (it->first == "Category")
            continue;
        
        if (it->first == "I_COMMENTARY_EN") {
            report += "<b>Commentary:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "M_VOLUME") {
            report += "<b>Volumes:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "NODE_LEVEL") {
            report += "<b>Publication type:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "P_TBRC_CODE" || it->first == "tbrc_code") {
            continue;
        }
        if (it->first == "creator") {
            report += "<b>Author:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "J_COMMENTARY_RU") {
            report += "<b>Комментарий:</b> " + it->second + "<br/>\n";
            continue;
        }
        
        if (it->first == "creatortype")
            continue;
        if (it->first == "creatorothers")
            continue;
        if (it->first == "disk")
            continue;
        if (it->first == "file_name")
            continue;
        if (it->first == "fileName")
            continue;
        if (it->first == "a_Title")
            continue;
        
        if (it->first == "Path")
            continue;
        if (it->first == "Q_COPY")
            continue;
        
        report += it->first + ": " + it->second + "<br/>\n";
    }
    if (pageMap["P_TBRC_CODE"] != "") {
        report += "<a href=\"https://www.tbrc.org/#!rid=" + pageMap["P_TBRC_CODE"] +
        "\" target=\"_blank\" class=\"open_link\">Open this book in "
        "www.tbrc.org</a><br/>\n";
    } else if (pageMap["tbrc_code"] != "") {
        report += "<a href=\"https://www.tbrc.org/#!rid=" + pageMap["tbrc_code"] +
        "\" target=\"_blank\" class=\"open_link\">Open this book in "
        "www.tbrc.org</a><br/>\n";
    }
    
    report += out.str();
    report += out_.str();
    // reportStr=str_replace(" ","</tsh> <tsh>",reportStr);
    reportStr = str_replace("/n", "¶", reportStr);
    vector<string> v;
    explode(">", reportStr, v);
    for (int i = 0; i < v.size(); i++) {
        if (isPali(v[i]) || v[i].find("་") != -1) {
            string p1 = substr(0, (uint)v[i].find("<"), v[i]);
            string p2 = substr((uint)v[i].find("<"), (uint)v[i].size(), v[i]);
            p1 = str_replace(" ", "</tsh> <tsh>", p1);
            p1 = str_replace("།", "</tsh>།<tsh>", p1);
            p1 = "<tsh>" + p1 + "</tsh>";
            v[i] = p1 + p2;
        }
    }
    for (int i = 0; i < v.size(); i++) {
        if (v[i].find("<") != -1)
            v[i] += ">";
    }
    string delimeter = "";
    reportStr = implode(delimeter, v);
    render = reportStr + "</tsh>\n<hr/>\n" + report;
    render = str_replace("<br />", "<br/>", render);
    render = str_replace("<br>", "<br/>", render);
    //re2::RE2::Replace(&render, "(<br/>)+", "<br/>");
    render = str_replace("</div><br/><div>", "</div><div>", render);
    render = str_replace(">>", ">", render);
    render = str_replace("<hr/>\n<hr/>", "<hr/>", render);
    render = str_replace("<hr/><hr/>", "<hr/>", render);
    render = str_replace("<br/><div>", "<div>", render);
    render = str_replace("<div><br/>", "<div>", render);
    render = str_replace("</h3><br/>", "</h3>", render);
    cpstr(page,render);
};

// database text record category parsing and render
void renderCategory(pageRecLib &page) {
    //void *result_, char *buff, int index, int field
    string text;
    string str;
    string report;
    string reportStr;
    ostringstream out;
    ostringstream out_;
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    
    string db = page.db;
    if (db == "")
        db = "DHARMABOOK";
    
    longMemory->loadTable(db);
    GVector *v = longMemory->table[db].data;
    if (page.index <0 || page.index > v->size()) {
        cpstr(page,"this record not exist");
        return;
    }
    TString st;
    v->getTStr(page.index, st);
    if (page.field >= st.size()) {
        cpstr(page,"this field not exist");
        return;
    }
    
    if (page.field == 1) {
        
        string pageText = st[1];
        str = pageText;
        
        // Title render
        
        if (str.find("[[Title:") != -1) {
            int start = (int)str.find("[[Title:") + 8;
            int end = (int)str.find("]]", start);
            string title = substr(start, end - start, str);
            out << "<span style=\"font-size:21px\">Title:" << title << "</span><hr/>";
        }
        // Content list render
        if (str.find("===") != -1) {
            out << "<a class=\"content\" href=\"/?field=1&index=" << page.index
            << "&ocrData=read\">Content • Содержание</a><br/>\n";
            for (int i = 0; i < st.size(); i++) {
                string s = st[i];
                vector<string> lineArray;
                explode("¶", s, lineArray);
                for (int n = 0; n < lineArray.size(); n++) {
                    if (lineArray[n].size() > 3 && lineArray[n].find("===") == 0) {
                        out << "<a class=\"content\" href=\"/?field=" << i
                        << "&index=" << page.index << "&ocrData=read\">"
                        << lineArray[n] << "</a><br/>\n";
                    }
                }
            }
        }
        str = str_replace("¶", "\n", str);
        reportStr = out.str();
        page.text= &str[0];
        renderCategoryText(page);
        reportStr += page.data;
        //cout<<"reportStr:"<<reportStr<<"/"<<endl;
        // reportStr=st[1];
        
    } else {
        reportStr = st[page.field];
    }
    
    //str=str_replace("<br/>","",reportStr);
    //str=str_replace("\n","",str);
    //str=str_replace("<hr/>","",str);
    //str=str_replace(" ","",str);
    //cout<<"@@@"<<str<<"/@@@"<<str.size()<<endl;
    // if(str!=""){
    //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
    //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
    //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
    
    //}else{
    //    stData=report;
    //}
    reportStr = str_replace("¶", "<br/>", reportStr);
    //re2::RE2::Replace(&stData, "(<br/>)+", "<br/>");
    
    reportStr = str_replace("<tsh><br/></tsh>", "<br/>", reportStr);
    reportStr = str_replace("<br/><br/>", "<br/>", reportStr);
    reportStr = str_replace("<br/>\n<hr/>", "<hr/>", reportStr);
    reportStr = str_replace("<hr/>\n<a href", "<a href", reportStr);
    reportStr = str_replace("<br/>===", "<br/><hr/><b>===", reportStr);
    reportStr = str_replace("===<br/>", "===</b><br/>", reportStr);
    cpstr(page,reportStr);
}

void renderCategorySearch(pageRecLib &page) {
    string text = page.text;
    text = str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/",
                       "/___DHARMA___/___LIBRARY___/", text);
    
    string str;
    string report;
    string reportStr;
    //рендер шаблонов
    vector<string> pageV;
    explode("{{", text, pageV);
    map<string, string> pageMap;
    for (int i = 0; i < pageV.size(); i++) {
        str = pageV[i];
        if (str.find("|") != -1 && str.find("}}") != -1) {
            string line = substr(0, (uint)str.find("}}"), str);
            vector<string> v;
            explode("|", line, v);
            if (pageMap.find(v[0]) != pageMap.end())
                continue;
            pageMap[v[0]] = v[1];
            str = substr((uint)str.find("}}") + 2, (uint)str.size(), str);
            reportStr += str_replace("\n", "<br/>\n", str);
        } else {
            reportStr += str_replace("\n", "<br/>\n", str);
        }
    }
    for (map<string, string>::iterator it = pageMap.begin(); it != pageMap.end();
         it++) {
        // it->first = key
        if (it->second == "")
            continue;
        if (it->first == "OSBL")
            continue;
        if (it->first == "ID")
            continue;
        if (it->first == "I_COMMENTARY_EN") {
            report += "<b>Commentary:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "M_VOLUME") {
            report += "<b>Volumes:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "NODE_LEVEL") {
            report += "<b>Publication type:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "P_TBRC_CODE" || it->first == "tbrc_code") {
            continue;
        }
        if (it->first == "creator") {
            report += "<b>Author:</b> " + it->second + "<br/>\n";
            continue;
        }
        if (it->first == "J_COMMENTARY_RU") {
            report += "<b>Комментарий:</b> " + it->second + "<br/>\n";
            continue;
        }
        
        if (it->first == "creatortype")
            continue;
        if (it->first == "creatorothers")
            continue;
        if (it->first == "disk")
            continue;
        if (it->first == "file_name")
            continue;
        if (it->first == "fileName")
            continue;
        if (it->first == "a_Title")
            continue;
        
        if (it->first == "Path")
            continue;
        if (it->first == "Q_COPY")
            continue;
        
        report += it->first + ": " + it->second + "<br/>\n";
    }
    string stData = report + "<br/>\n" + reportStr;
    stData = str_replace("<br/>", " ", stData);
    stData = str_replace("\n", "", stData);
    stData = str_replace("@@@", "", stData);
    
    cpstr(page,stData);
}//__________________________________________________________

//функция строит дерево категорий и передает для построения таблицы.
void listCategoryToTable(pageRecLib &page) {
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    GVector *cV;
    string db = "DHARMABOOK_Category";
    string str, report;
    longMemory->loadTable(db);
    cV = longMemory->table[db].data;
    ostringstream out;
    map<string, int> cMap;
    map<string, int> dMap;
    multimap<int, string> sortMap;
    
    for (uint i = 0; i < (uint)cV->size(); i++) {
        TString st;
        cV->getTStr(i, st);
        vector<int> v;
        st.getIntVector(v, 1);
        if (st[0].find("__") != -1) {
            string up = st[0];
            up = substr(0, (uint)up.find("__"), up);
            dMap[up]++;
            if (v.size())
                cMap[up] += v.size();
            continue;
        }
        if (v.size())
            cMap[st[0]] += v.size();
    }
    
    map<string, int>::iterator it0;
    it0 = cMap.end();
    it0--;
    while (1) {
        sortMap.insert(pair<int, string>(it0->second, it0->first));
        if (it0 == cMap.begin())
            break;
        it0--;
    }
    
    multimap<int, string>::iterator it;
    it = sortMap.end();
    it--;
    out << "var tableData =[";
    
    while (1) {
        if (it->second.size()) {
            
            out << "{p1:\"" << it->second << "\",p2:\"" << it->first << "\",p3:\"";
            if (dMap.find(it->second) != dMap.end()) {
                out << dMap[it->second];
            }
            out << "\"},\n";
        }
        if (it == sortMap.begin())
            break;
        it--;
    }
    out << "{}]";
    string stData = out.str();
    //cout<<stData;
    cpstr(page,stData);
}

void addRecord(pageRecLib &page) {
    
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    ostringstream out;
    GVector *v;
    string db = "DHARMABOOK";
    string str, report;
    longMemory->loadTable(db);
    v = longMemory->table[db].data;
    TString st;
    long time_ = time(NULL);
    out << "[[ID:" << time_ << "]]";
    str = out.str();
    out.str("");
    st.push_back(str);
    st.push_back("");
    v->push_back(st);
    out << v->size() - 1;
    
    string stData = out.str();
    cpstr(page,stData);
}


void addPage(pageRecLib &page) {
    //int index,int count
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    ostringstream out;
    GVector *v;
    string db = "DHARMABOOK";
    longMemory->loadTable(db);
    v = longMemory->table[db].data;
    TString st;
    //cout<<"index:"<<page.index<<" field:"<<page.field<<endl;
    
    v->getTStr(page.index, st);
    st.save();
    for(int i=0;i<page.field;i++)st.push_back("");
    
    v->putTStr(page.index, st);
    
    string stData = "done add page";
    cpstr(page,stData);
}//___________________________________________________


void WylieToUnicode(pageRecLib &page) {
    if (inputData.init<2)initOCR();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        srcStr=logicProcessor->WylieToUniConverter(srcStr);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        for(int i=0;i<inputData.fileList.size();i++){
            string path = inputData.fileList[i];
            if( is_dir(path)){
                i++; continue;
            }
            string srcStr;
            readText(srcStr,path);
            srcStr=logicProcessor->WylieToUniConverter(srcStr);
            path += ".out";
            writeText(srcStr, path);
        }
        string stData = " done";
        cpstr(page,stData);
    }
}//____________________________________________________


void UnicodeToWylie(pageRecLib &page) {
    if (inputData.init<2)initOCR();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        srcStr=logicProcessor->UniToWylieConverter(srcStr);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        for(int i=0;i<inputData.fileList.size();i++){
            string path = inputData.fileList[i];
            if( is_dir(path)){
                i++; continue;
            }
            string srcStr;
            readText(srcStr,path);
            srcStr=logicProcessor->UniToWylieConverter(srcStr);
            path += ".out";
            writeText(srcStr, path);
        }
        string stData = " done";
        cpstr(page,stData);
    }

}//____________________________________________________


void UnicodeToYagpo(pageRecLib &page) {
    if (inputData.init<2)initOCR();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        logicProcessor->UnicodeToYagpo(srcStr);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        for(int i=0;i<inputData.fileList.size();i++){
            string path = inputData.fileList[i];
            if( is_dir(path)){
                i++; continue;
            }
            string srcStr;
            readText(srcStr,path);
            logicProcessor->UnicodeToYagpo(srcStr);
            path += ".out";
            writeText(srcStr, path);
        }
        string stData = " done";
        cpstr(page,stData);
    }
}//____________________________________________________


void YagpoToUnicode(pageRecLib &page) {
    if (inputData.init<2)initOCR();
        initCommandData();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    string result;
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        logicProcessor->YagpoToUni(srcStr);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        string path = page.text;
        string srcStr;
        readText(srcStr,path);
        logicProcessor->YagpoToUni(srcStr);
        path += ".out";
        writeText(result, path);
        
        string stData = path + " done";
        cpstr(page,stData);
    }
}//____________________________________________________

void RusTranscription(pageRecLib &page){
    if (inputData.init<2)initOCR();
    initCommandData();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    string result;
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        logicProcessor->lineTranscription(srcStr,0);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        string path = page.text;
        string srcStr;
        readText(srcStr,path);
        logicProcessor->lineTranscription(srcStr,0);
        path += ".out";
        writeText(result, path);
        
        string stData = path + " done";
        cpstr(page,stData);
    }
}


void EngTranscription(pageRecLib &page){
    if (inputData.init<2)initOCR();
    initCommandData();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    string result;
    if(page.intData==READ_TEXT_WRITE_TEXT){
        string srcStr=page.text;
        logicProcessor->lineTranscription(srcStr,1);
        cpstr(page,srcStr);
    }else if(page.intData==READ_PATH_WRITE_FILE){
        string path = page.text;
        string srcStr;
        readText(srcStr,path);
        logicProcessor->lineTranscription(srcStr,1);
        path += ".out";
        writeText(result, path);
        
        string stData = path + " done";
        cpstr(page,stData);
    }
}

void writeText_(const char *text, const char *path) { writeText(text, path); }

//////*********** CONVERTERS ************************

void RTFToUniFile(pageRecLib &page) {
    if (inputData.init<2)initOCR();
        initCommandData();
    GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
    string result;
    string path = page.text;
    inputData.data["ocrData"] = "RTFToUnicode";
#ifdef COCOA
    logicProcessor->RTFtoUniConverter(result, path);
#endif
    path += ".out";
    writeText(result, path);
    
    string stData = path + " done";
    cpstr(page,stData);
}//_____________________________________________________

//////*********** DATABASE ************************
void deleteCategory(const char *category) {
    
    if (!inputData.init)
        initCommandData();
    GMemory *longMemory = (GMemory *)inputData.longMemory;
    GVector *dk, *dBook;
    GMap *d;
    string db = "DHARMABOOK_Category";
    string report;
    
    longMemory->loadTable(db);
    dk = longMemory->table[db].data;
    indexRecord *indexRec =
    longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
    d = indexRec->mIndex;
    string key = category;
    
    uint index = (uint)d->getHKey(key, 0);
    //cout<<index<<endl;
    TString st;
    
    if (index != -1) {
        
        dk->getTStr(index, st);
        vector<int> v;
        st.getIntVector(v, 1);
        db = "DHARMABOOK";
        longMemory->loadTable(db);
        dBook = longMemory->table[db].data;
        TString st_;
        //для каждой записи из вектора  v (списка страниц категории)
        for (int i = 0; i < v.size(); i++) {
            
            dBook->putTStr(v[i], st_);
        }
        db = "DHARMABOOK_Category";
        dk->putTStr(index, st_);
        longMemory->reloadIndex(longMemory->table[db], 0, HASH_SEARCH);
    }
}//______________________________________________________

void *getImg(const char *path) {
    GBitmap *img = GBitmap::create(path);
    return (void *)img;
}//______________________________________________________

void getFileList(pageRecLib &page) {
    vector<string> fileList;
    string path = page.text;
    readDirectoryToArray(fileList, path, "img");
    if (!fileList.size()){
        cpstr(page,"no image in path");
        return;
    }  
    string str = implode(":|:", fileList);
    cpstr(page,str);
}//____________________________________________________

void drawGBitmap(pageRecLib &page, void *img_) {
    GBitmap *img = (GBitmap *)img_;
    string str;
    img->strPNG(str);
    cpstr(page,str);
}//____________________________________________________

