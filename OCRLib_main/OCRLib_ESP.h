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

#ifndef _GMemoryLib_H_
#define _GMemoryLib_H_

#define RECORD_LIMIT 25


typedef struct pageRecord {
	char *text;
  char *data;
  char *db;
  char *ln;

  int index;
  int field;
  int pageCount;
  int bookCount;
  int intData;
  int command;
  int mode;
  int dataSize;
} pageRec;

void closePage(pageRec &page);
void initPage(pageRec &page);
void cpstr(pageRec &page,const char *str);

void initCommandData();
char *getPageStream(pageRec *page_);
void helloVarStr(pageRec &page);
void getDBRecord(pageRec &page);
void getFieldC(pageRec &page);
void getPageC(pageRec &page);
void getPageC_(pageRec &page);
void getIntC(pageRec &page);
void splitOnPagesC(const char *db);
void dumpWiki(const char *path);
void exportDictFromTextDB(const char *db, const char *path);

// Dictionary functions
void translateLine(pageRec &page);
void dictionaryReport(pageRec &page);
void dictionaryReportTitle(pageRec &page);
void reloadDictionary(pageRec &page);
void replaceInDB(const char *db);
void importOCRText(const char *db, const char *path);
void importOCRTextByIndex(pageRec &page);
void searchDB(pageRec &page);
void searchDBTable(pageRec &page);
void joinFieldInTable(const char *dbName, int filedNameIndex);
void loadLibraryPathInCatalog(const char *db);
void exportWordList(const char *path);
void sortWordList(const char *path);
void exportDBTable(const char *db);
void importDBTable(const char *db, const char *path);
void importOCR(const char *db, const char *path, const char *delimeter);
void importTextFolder(const char *db, const char *path, const char *category);
void importText(pageRec &page);
void exportText(pageRec &page);
void importDictTab(pageRec &page);
void importMainDictionary();
void checkDublicateID(const char *db);
void savePageInDB(pageRec &page);
void rebuildCategory();
void searchByTag(pageRec &page);
void searchCategory(pageRec &page);
void setCategory(const char *key_, const char *category_);
void listCategory(pageRec &page);
void renderCategoryText(pageRec &page);
void renderCategory(pageRec &page);
void renderCategorySearch(pageRec &page);
void listCategoryToTable(pageRec &page);
void addRecord(pageRec &page);
void addPage(pageRec &page);
void UnicodeToWylie(pageRec &page);
void writeText_(const char *text, const char *path);
void RTFToUniFile(pageRec &page);
void deleteCategory(const char *category);

/////// IMAGE FUNCTIONS ////////////
void *getImg(const char *path);
void *loadImage(const char *path, int mode);

void getFileList(pageRec &page);
void drawGBitmap(pageRec &page, void *img_);
void imageProcess(pageRec &page);

// Функция комплексной обработки графического текста.
void *imageAdaptiveFiltration(void *img_);
void drawPreview(void *conn, const char *path_);

/// TEXT FUNCTIONS
void textProcess(pageRec &page);

// OCR functions
void initOCR();
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
