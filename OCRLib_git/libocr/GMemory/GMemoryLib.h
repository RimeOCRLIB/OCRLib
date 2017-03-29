//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for .more details.
//C-

/*  Interface  to GMemoryLib dynamic library  */


#ifndef _GMemoryLib_H_
#define _GMemoryLib_H_

#define RECORD_LIMIT 25


#ifdef OCR_DYLIB
extern "C" {
#endif
    
typedef
    struct pageRecord{
        const char *db;
        int index;
        int field;
        int pageCount;
        int bookCount;
        const char *text;
        int intData;
    }pageRec;
    
    void initCommandData();
    const char* helloVarStr(char *str);
    const char* www_helloVar(char *str);
    const char* getFieldC(const char*db,int rec,int field);
    void getPageC(pageRec *page);
    void getPageC_(pageRec *page);
    void getIntC(pageRec *page);
    void splitOnPagesC();
    void replaceInDB(const char*db);
    void importOCRText(const char*path);
    const char* searchDBTable(pageRec *page,int mode);
    const char* searchDB(pageRec *page);
    const char* searchTextDB(pageRec *page);
    void loadLibraryPathInCatalog(const char *db);
    void joinFieldInTable(const char* name, int filedNameIndex);
    void deleteCategory(const char* category);
    void exportDBTable(const char *db);
    void importDBTable(const char *db,const char *path);
    void importOCR(const char *db,const char *path,const char *delimeter);
    void importTextFolder(const char *db,const char *path,const char *category);
    void importText(const char *db,const char *path,int index);
    void importDictTab(const char *db,const char *path,const char *category,int year);
    void dumpWiki(const char *path);
    void exportDictFromTextDB(const char *db,const char *path);
    const char* savePageInDB(const char *db,const char *text,int index,int field);
    void checkDublicateID(const char *db);
    const char* searchByTag(const char *db_,const char *tag, const char *request, int mode);
    
    //Dictionary functions
    const char* translateLine(const char*line,const char*ln);
    const char* dictionaryReport(const char*line,const char*ln);
    const char* dictionaryReportTitle(const char*line,const char*ln);
    const char* reloadDictionary(const char*user_text, const char *ln);
    const char* renderCategory(pageRec *pageRec, const char *ln);
    const char* renderCategorySearch(pageRec *pageRec, const char *ln);
    const char* renderCategoryText(const char *pageText, int index);
    const char* renderPage(const char *pageText);
    const char* checkCode(pageRec *page);
    void exportWordList(const char*path);
    void sortWordList(const char*path);
    void rebuildCategory();
    const char* searchCategory(const char *request,int indexPage,int  *pageCount);
    const char* listCategory();
    void setCategory(const char *key,const char *category);
    const char* addRecord();
    void importMainDictionary();
    
    //OCR functions
    void bookOCR(const char *path);
    

#ifdef OCR_DYLIB
}
#endif

# endif

