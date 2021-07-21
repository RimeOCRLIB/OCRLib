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
// C- GNU General Public License for .more details.
// C-

/*  Interface  to GMemoryLib dynamic library  */

#ifndef _OCRLib_H_
#define _OCRLib_H_

#define RECORD_LIMIT 25

#include "OCRLibC.h"
#include "../libocr/config.h"
#include "../libocr/OCRTypes/OCRTypes.h"


typedef struct pageRecordLib {
    string text;        ///< input text data or URL query string
    string data;        ///< function respond data
    string db;          ///< request database name
    string ln;          ///< request language
    
    int index;          ///< database record index
    int field;          ///< database record field
    int pageCount;      ///< function report all field count in requested record
    int bookCount;      ///< function report all records count in request database
    int intData;        ///< additional request parameter
    int command;        ///< main request command as emum member
    int mode;           ///< mode of request executuion (in c++ library, in Python or shell session)
    int dataSize;       ///< text of data field size in bytes. Need for gateway between Python and C
} pageRecLib;

void closePage(pageRecLib &page);
void initPage(pageRecLib &page);


void initCommandData();
void processRequest(pageRecLib &page);
void helloVarStr(pageRecLib &page);
void getDBRecord(pageRecLib &page);
void getFieldC(pageRecLib &page);
void getPageC(pageRecLib &page);
void getPageC_(pageRecLib &page);
void getIntC(pageRecLib &page);
void splitOnPagesC(const char *db);
void dumpWiki(const char *path);
void exportDictFromTextDB(const char *db, const char *path);

// Dictionary functions
void translateLine(pageRecLib &page);
void dictionaryReport(pageRecLib &page);
void dictionaryReportMain(pageRecLib &page);
void dictionaryReportTitle(pageRecLib &page);
void reloadDictionary(pageRecLib &page);
void replaceInDB(const char *db);
void importOCRText(const char *db, const char *path);
void importOCRTextByIndex(pageRecLib &page);
void searchDB(pageRecLib &page);
void searchDBTable(pageRecLib &page);
void joinFieldInTable(const char *dbName, int filedNameIndex);
void loadLibraryPathInCatalog(const char *db);
void exportWordList(const char *path);
void sortWordList(const char *path);
void exportDBTable(const char *db);
void importDBTable(const char *db, const char *path);
void importOCR(const char *db, const char *path, const char *delimeter);
void importTextFolder(const char *db, const char *path, const char *category);
void importText(pageRecLib &page);
void exportText(pageRecLib &page);
void importDictTab(pageRecLib &page);
void importMainDictionary();
void checkDublicateID(const char *db);
void savePageInDB(pageRecLib &page);
void rebuildCategory();
void searchByTag(pageRecLib &page);
void searchCategory(pageRecLib &page);
void setCategory(const char *key_, const char *category_);
void listCategory(pageRecLib &page);
void renderCategoryText(pageRecLib &page);
void renderCategory(pageRecLib &page);
void renderCategorySearch(pageRecLib &page);
void listCategoryToTable(pageRecLib &page);
void addRecord(pageRecLib &page);
void addPage(pageRecLib &page);
void writeText_(const char *text, const char *path);
void deleteCategory(const char *category);

/////// LogicProcessor function/////
void UnicodeToYagpo(pageRecLib &page);
void YagpoToUnicode(pageRecLib &page);
void WylieToUnicode(pageRecLib &page);
void UnicodeToWylie(pageRecLib &page);
void RTFToUniFile(pageRecLib &page);
void RusTranscription(pageRecLib &page);
void EngTranscription(pageRecLib &page);


/////// IMAGE FUNCTIONS ////////////
void *getImg(const char *path);
void *loadImage(const char *path, int mode);

void getFileList(pageRecLib &page);
void drawGBitmap(pageRecLib &page, void *img_);
void imageProcess(pageRecLib &page);

// Функция комплексной обработки графического текста.
void *imageAdaptiveFiltration(void *img_);
void drawPreview(void *conn, const char *path_);

/// TEXT FUNCTIONS
void textProcess(pageRecLib &page);
void HTML2UniConverter(pageRecLib &page);

// OCR functions
void initOCR();
void batchOCR(pageRecLib &page);
void bookOCR(const char *path);
void bookOCRGGraph(const char *path);
void imageFileOCR();
    
void letterBaseProcess(void *conn, const char *request_);
void letterBaseProcess1(void *conn, const char *request_);
void letterProcess(void *conn, const char *request_, int len);
void OCRletterProcess(void *conn, char *buff, unsigned int letterID);
void letterImg(void *conn, char *buff,unsigned int letterID);
void letterSVG(void *conn, char *name, char *drawBuf, unsigned int letterID);
void drawTwoLettersCorrelation(void *conn, char *drawBuf, unsigned int index1, unsigned int index2);
int rebuildOCRLetterFont(const char  *fontPath);
void renderLetterByID(unsigned int letterID, void *drawStr);
void exportGFontLetters(const char *path);

    
    
// fileDB functions
void fileDB(char *bufferDir,char *bufferFile);


#endif
