#ifndef PHP2STL_HEADER
#define PHP2STL_HEADER
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>

#include <dirent.h>
#ifdef UNIX
   #include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "config.h"
#include "OCRTypes.h"
#ifdef OCRLib
    #include "GLetter.h"
#endif

#include "pugixml.hpp"

#include "re2.h"

using namespace std;
using namespace ocr;
using namespace pugi;
using namespace re2;

///processInput - основной перекресток взаимодействия программы с пользователем.
///состояние интерфейса анализируется и записывается в inputData в процессе обработки HTTP запроса браузера, командной строки или запроса приложения Cocoa.
///inputData также содержит указатели на глобальные обьекты.
///processInput возвращает строку результирующих данных, которая передается как ответ программы на запрос браузера или другого интерфейса.
///в Cocoa приложении результат отображается в Web-инерфейсе окна програмы в WebView

void readPreferences();
void processInput(string &destString);
void processRequest(string &destString);


int strLen(string &input);
int strLenW(wstring &input);
wstring UTF_to_Unicode(string &input);
wstring UTF_to_Unicode(const char*name);
void UTF_to_UnicodeVector(string &input,vector<ushort>&vt);
void UTF_to_UnicodeVector(string &input,vector<uint>&vt);
string Unicode_to_UTF( wstring &input);
string Unicode_to_UTF( wchar_t input);
string Unicode_to_UTF(string &input);

vector<string> explode(const string& delimiter, const string& input);
vector<wstring> explodeW(const wstring& delimiter, const wstring& input);

#ifdef OCRLib
vector<wordOCR> explode(const vector<string>& delimiter, const string& input);
vector<stringOCR> explode(const vector<string>& delimiter, stringOCR& input);
string implode(const string& delimiter,  const vector<wordOCR>& input);
#endif

string implode(const string& delimiter,  const vector<string>& input);
wstring implodeW(const wstring& delimiter,  const vector<wstring>& input);
string implode(string& delimiter, list<string>& input);
string implode(const char *delimiter, list<string>& input);
string implode(const char *delimiter, list<string>& input);

string substr(int inInd,int count, string &srcString );
string substr(int inInd,string &srcString );
string substrT(const char *buf, string &srcString);
string substrH(const char *buf, string &srcString);
const char *substr(int inInd,int count, const char *str1);
unsigned long readText(char *buff, const char *path);
void readText(vector<string> &strVector, string &inFile);
void readText(map<string,uint> &strMap, string &inFile);
void readText(string &str,ifstream &inFile);
void readText(string &str,const char *path);
void readText(string &str,string &inFile);
string readText(string &inFile);
unsigned long readInMemory(char **textBufer,string &path);

void writeText(vector<string> &strVector, string &outFile);
void writeText(vector<string> &strVector, const char* dest);
ulong writeText(string &outStr, string &outFile);
void writeText(char *buff, unsigned int size, const char *path);
void writeText(string &outStr, const char *path);
void writeText(const char *data, string &outFile);
void writeText(const char *data, const char *outFile);
ulong writeFileAppend(string &outStr, string &outFile);
void writeToLog(string &outStr, const char *path, const char *flagFile);  ///few process can share log file. Used status of flag file for lock 
bool is_file(string &path);
bool is_file(const char* path);
bool is_dir(string &path);
bool is_dir(const char* path);
bool fileOpen(string &path);
bool emptyFolder(string &path);

int bsearch(hashRecord *hashVector,unsigned  size,int value);  ///analog of c++ binary_search. Return index of element or -1 if not found 

void readMapXML(map<string,string>&inputMap,string &path);

void readInput();
string run(const char* cmd);   //execute external process
string run(string cmd);   //execute external process


string str_replace(string &src, string &dest, string &srcString );
string str_replace(const char *srcChar, string &dest, string &srcString);
string str_replace(const char *srcChar, const char *destChar, string &srcString );
string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar );
string str_replace(string &src, string &dest, string &srcString , int count);
string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar, int count );
string str_replace(vector<string> &srcArray, string &dest, string &srcString );
string str_replace(vector<string> &srcArray, const char* destData, string &srcString );
void stripLetters(string &str);      //remove all letters from str

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

void clearText(string &str); //форматирование текста для перевода
void clearTextSkt(string &str);
void setSearchText(string &key); //форматирование текста для поиска
bool regExpReplace(string &src, string &exp);  //replace src by regular expression exp
bool regExpReplace(string &src, const char* exp);  //replace src by regular expression exp
bool rexExpFind(char* textBuffer, string &exp);  //search textBuffer by regular expression exp
void lowerCase(string&src);
bool isRegExp(string&src);
string splitEngWord(string &word,const char*textDictEng, int *result);
bool isPali(string &str);  //определает является ли строка строкой языка Пали (результат основан на употреблении букв ITRANS)


string fileName(string &path);
string fileExt(string &path);
string folderPath(string &path);
string ptrToStr(const char*st,int ln);
string HTMLtoTxt(string &input);
string XMLEncode(string &input);
string XMLEncode(const char *input);
void encodeURL(string &URLstr);
void decodeURL(string &URLstr);
const char *decodeURL_C(const char *URLstr);
void parseQuery();  ///Decode query string from HTTP request or command line into globel variable inputData
char translateHex(char hex);
unsigned int getID(string &data);

string intToHex(int input);
string charToHex(unsigned char input);
unsigned int strToHash(const char *str, unsigned int len);
unsigned int RSHash (const char *str, unsigned int len);
void convertFileToUTF8();
void convertDirectoryToUTF();
int readDirectoryToArray(vector<string>&fileList, string &dirPath, string &ext);
int readDirectoryToArray(vector<string>&fileList, string &dirPath, const char*ext);
void readDirectory(vector<string>&fileList,vector<string>&folderList,string &dirPath);
void readInputData(const char *ext);


// -----------------------------------------------------------------------------
/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string itoaSTL(unsigned long long value, int base);
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
#ifdef UNIX
char* itoa(unsigned int value, char* result, int base);
char* itoa64(unsigned long long value, char* result, int base);
#endif

void binaryPrint(unsigned int input, int radix);
void binaryPrint64(unsigned long long input, int radix);

void printList(list<string>&list);
void printMap(map<string,string>&map);
string dampMap(map<string,string>&map);

void memprint(const char* ptr, int size);

void libCurl();
size_t  write_data(void *ptr,
					 size_t size,
					 size_t nmemb,
					 FILE *stream);

void error(char *msg);


// функции определения характеристик процессора.
void StartCpuInfo();
uint32 GetNumPhysicalCpus();
void GetNumLinuxKernel();
const SCpuInfo& GetCpuInfo();

string startSVG();
string closeSVG();

//сортировка строк по длинне
bool dictKey_sort(const dictKey d1, const dictKey d2);

int max(int &a,int &b);


//строковые функции тибетского языка
#ifdef OCRLib
  int indexById(vector<stringOCR>&correctionTable,unsigned int in);
  bool hasTail(OCRMatch&match);
#endif
string lastOf(string &name);
bool isTibDigit(string &name);

//монитор состояния сервера
void serverMonitor();




#endif
