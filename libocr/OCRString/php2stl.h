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

#include "../config.h"
#include "../OCRTypes/OCRTypes.h"
#include "OCRLib.h"
#ifdef OCRLib_
	#include "../GFontEditor/GLetter.h"
#endif

#include "../../pugixml/pugixml.hpp"

using namespace std;
using namespace ocr;
using namespace pugi;

///processInput - основной перекресток взаимодействия программы с пользователем.
///состояние интерфейса анализируется и записывается в inputData в процессе обработки HTTP запроса браузера, командной строки или запроса приложения Cocoa.
///inputData также содержит указатели на глобальные обьекты.
///processInput возвращает строку результирующих данных, которая передается как ответ программы на запрос браузера или другого интерфейса.
///в Cocoa приложении результат отображается в Web-инерфейсе окна програмы в WebView

void readPreferences();
void processInput(string &destString);

int strLen(string &input);
int strLenW(wstring &input);
wstring UTF_to_Unicode(string &input);
wstring UTF_to_Unicode(cstr name);
void UTF_to_UnicodeVector(string &input, vector<ushort> &vt);
void UTF_to_UnicodeVector(string &input, vector<uint> &vt);
string Unicode_to_UTF(wstring &input);
string Unicode_to_UTF(wchar_t input);
string Unicode_to_UTF(string &input);

int explode(const string &delimiter, const string &input, vector<string> &res);
int explodeW(const wstring &delimiter, const wstring &input, vector<wstring> res);

string implode(const string &delimiter, const vector<string> &input);
wstring implodeW(const wstring &delimiter, const vector<wstring> &input);
string implode(string &delimiter, list<string> &input);
string implode(cstr delimiter, list<string> &input);
string implode(cstr delimiter, list<string> &input);

string substr(int inInd, int count, string &srcString);
string substr(int inInd, string &srcString);
string substrT(cstr buf, string &srcString);
string substrH(cstr buf, string &srcString);
cstr substr(int inInd, int count, cstr str1);
unsigned long readText(char *buff, cstr path);
void readText(vector<string> &strVector, string &inFile);
void readText(map<string, uint> &strMap, string &inFile);
void readText(string &str, ifstream &inFile);
void readText(string &str, cstr path);
void readText(string &str, string &inFile);
string readText(string &inFile);
unsigned long readInMemory(char **textBufer, string &path);

void writeText(vector<string> &strVector, string &outFile);
void writeText(vector<string> &strVector, cstr dest);
ulong writeText(string &outStr, string &outFile);
void writeText(char *buff, unsigned int size, cstr path);
void writeText(string &outStr, cstr path);
void writeText(cstr data, string &outFile);
void writeText(cstr data, cstr outFile);
ulong writeFileAppend(string &outStr, string &outFile);
void writeToLog(string &outStr, cstr path, cstr flagFile); ///few process can share log file. Used status of flag file for lock
bool is_file(string &path);
bool is_file(cstr path);
bool is_dir(string &path);
bool is_dir(cstr path);
bool fileOpen(string &path);
bool emptyFolder(string &path);

int bsearch(hashRecord *hashVector, unsigned size, int value); ///analog of c++ binary_search. Return index of element or -1 if not found

void readMapXML(map<string, string> &inputMap, string &path);

void readInput();
string run(cstr cmd); //execute external process
string run(string cmd);		 //execute external process

string str_replace(string &src, string &dest, string &srcString);
string str_replace(cstr srcChar, string &dest, string &srcString);
string str_replace(cstr srcChar, cstr destChar, string &srcString);
string str_replace(cstr srcChar, cstr destChar, cstr srcStringChar);
string str_replace(string &src, string &dest, string &srcString, int count);
string str_replace(cstr srcChar, cstr destChar, cstr srcStringChar, int count);
string str_replace(vector<string> &srcArray, string &dest, string &srcString);
string str_replace(vector<string> &srcArray, cstr destData, string &srcString);
void stripLetters(string &str); //remove all letters from str
void join(vector<string>&v,cstr delimeter,string &result);

// trim from start
//static inline std::string &ltrim(std::string &s)
//{
	//s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
//	return s;
//}

// trim from end
//static inline std::string &rtrim(std::string &s)
//{
	//s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
//	return s;
//}

// trim from both ends
//static inline std::string &trim(std::string &s)
//{
//	return ltrim(rtrim(s));
//}

void clearText(string &str); //форматирование текста для перевода
void clearTextSkt(string &str);
void setSearchText(string &key);				  //форматирование текста для поиска
bool regExpReplace(string &src, string &exp);	 //replace src by regular expression exp
bool regExpReplace(string &src, cstr exp); //replace src by regular expression exp
bool rexExpFind(char *textBuffer, string &exp);   //search textBuffer by regular expression exp
void lowerCase(string &src);
bool isRegExp(string &src);
string splitEngWord(string &word, cstr textDictEng, int *result);
bool isPali(string &str); //определает является ли строка строкой языка Пали (результат основан на употреблении букв ITRANS)

string fileName(string &path);
string fileExt(string &path);
string folderPath(string &path);
string ptrToStr(cstr st, int ln);
string HTMLtoTxt(string &input);
string XMLEncode(string &input);
string XMLEncode(cstr input);
void encodeURL(string &URLstr);
void decodeURL(string &URLstr);
cstr decodeURL_C(cstr URLstr);
void parseQuery(); ///Decode query string from HTTP request or command line into global structure inputData
void parseQuery(pageRecLib &pageLib); ///Decode query string from HTTP request or command line into global structure inputData
void parseJson(); ///Decode Json string from HTTP request or command line into global structure inputData
char translateHex(char hex);
unsigned int getID(string &data);

string intToHex(int input);
string charToHex(unsigned char input);
// Translate a single hex character; used by
// decodeURL():
char hex2char(char hex);
int readHexData(char *data, int size);
void readData2JPG(cstr file);
void stringToHex(string &str);
unsigned int strToHash(cstr str, unsigned int len);
unsigned int RSHash(cstr str, unsigned int len);
void convertFileToUTF8();
void convertDirectoryToUTF();
void copyFile(cstr in, cstr out);
int readDirectoryToArray(vector<string> &fileList, string &dirPath, string &ext);
int readDirectoryToArray(vector<string> &fileList, string &dirPath, cstr ext);
int readDirectoryToArray(vector<string> &fileList, cstr dirPath, cstr ext);
void readDirectory(vector<string> &fileList, vector<string> &folderList, string &dirPath);
void clearDirectory(cstr path, cstr ext);

void readInputData(cstr ext);

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
char *itoa(unsigned int value, char *result, int base);
char *itoa64(unsigned long long value, char *result, int base);
#endif

void binaryPrint(unsigned int input, int radix);
string binaryPrintStr(unsigned int input, int radix);
void binaryPrint64(unsigned long long input, int radix);
float round_2(float var);   //округление float до двух десятых

void printList(list<string> &list);
void printMap(map<string, string> &map);
string dampMap(map<string, string> &map);
void printArray(int *c, int len);
void printArray(unsigned *c, int len);
void printArray(short *c, int len);
void printArray(unsigned short *c, int len);
void printArray(char *c, int len);
void printArray(unsigned char *c, int len);
void printArray(float *c, int len);

void memprint(cstr ptr, int size);

void libCurl();
size_t write_data(void *ptr,
				  size_t size,
				  size_t nmemb,
				  FILE *stream);

void error(char *msg);

// функции определения характеристик процессора.
void StartCpuInfo();
uint32 GetNumPhysicalCpus();
void GetNumLinuxKernel();
const SCpuInfo &GetCpuInfo();

string startSVG();
string closeSVG();

string lastOf(string &name);

//сортировка строк по длинне
bool dictKey_sort(const dictKey d1, const dictKey d2);

int max(int &a, int &b);

//монитор состояния сервера
void serverMonitor();

#endif
