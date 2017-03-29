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

//if ( ImBuf !=NULL ) farfree(ImBuf);  //  освобождение памяти
//q=(тип_q *)farmalloc(n_byte);     // запрос памяти без очистки 0
//q_new=farrealloc(q_old,n_byte);;  // изменение размера блока

#ifndef _GMemory_H_
#define _GMemory_H_

#include "config.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>



#include "OCRTypes.h"
#include "GVector.h"
#include "GMap.h"
#include "GFontEditor.h"
#include "GLogicProcessor.h"
#include "GLetter.h"
#include "pugixml.hpp"
#include "OCRTypes.h"
#include "php2stl.h"


using namespace std;
using namespace pugi;

namespace ocr{
    
    class indexRecord{
        public:
            indexRecord(void);
            int status;
            int indexDataType; //type of the index data (GMap or GVector)
            int indexType;     //search type in index (b-tree in sorted vector, hash, associative full-text search, OCR and all another)
            int fieldNum;      //number of field in the table adressed by index
            int i;             //index of pointer to index in vIndex or mIndex vector according of indexDataType
            GVector* vIndex;   //pointer on index data
            GMap* mIndex;      //pointer on index data
    };


    class tableRecord{
        public:
            tableRecord(void);
            int status;
            string name;
            GVector *data;
            vector<indexRecord>index;
    };


//#define REMARK

    class  GMemory {
    protected:
        GMemory(void);
    public:
        void destroy(void);
        void init();
        static GMemory* create(void) {return new GMemory;}

    public:
        GVector *textCorpus;            ///vector with all tibetan OCR corpus text data
        GMap *textCorpusGMap;           ///map with all tibetan OCR corpus text data
        GVector *textCorpusLib;         ///vector with all tibetan library corpus text data
        GMap *textCorpusLibGMap;        ///map with all tibetan library corpus text data
        GVector *fontTable;             ///main font data table. Read from XML LETTERS_GRAMMAR.xml
        GMap *fontGMap;                 ///main font data map. Builded from fontTable. Keys it is unicode of letters.
        GMap *fontStackGMap;            ///main font data map. Builded from fontTable. Keys it is unicode of letters.

        
        GVector *vectorDict;            ///vector with text data of main dictionary
        GMap *dictionaryGMap;           ///map of main dictionary

        GVector *translationDictRus;       ///vector with text data of translation dictionary
        GMap *translationDictGMapRus;      ///main map of translation dictionary
        GVector *translationDictEng;       ///vector with text data of translation dictionary
        GMap *translationDictGMapEng;      ///main map of translation dictionary
        
        GVector *vectorCorpus;          //vector of all lines in corpus
        GVector *vectorLib;             //vector of all library catalog
        GVector *vectorLibPath;         //vector of all library data paths
        
        GFont *aliKali;                 //OCR main font
        
        map<string,tableRecord> table;  //tableRecord data by table name
        /*Format database description in DBVector:
         Every table record it is string stored as TString in GVector. 
         The first field in TString of the record it is name of the table
         in table string record contain data of all indexRecords for this table.
         Important.
         In moment of database start it create not inicialised indexRecords according the data in DBVector.
         Inicialisation of index executed in moment of first request to index.
         If index not exist on moment of request, it will created for requested field of table according the request type.
         Index can be created directly by createIndex(const char* tableName,int fieldNum, int indexType);
         */
         
         
        
        ///функции управления базой данных
        /**bref load all databases in mmap memory by request in inputData global structure*/
        void loadDB();
        /**bref load table in mmap memory by name and all table index GMap. If table not exist create it in default DATA_PATH*/
        void loadTable(string &name);
        indexRecord* createIndex(tableRecord &rec,int searchField,int searchMode);
        void reloadIndex(tableRecord &rec,int searchField,int searchMode);
        /**bref export table in default DATA_PATH*/
        void exportTable(string &name,string &ext);
        /**bref import table data from path*/
        void importTable(string &name,string &path);
        /**bref import OCR data from path*/
        void importOCR(string &name,string &path,string &delimeter);
        /**bref import txt data from path*/
        void importTextFolder(string &name, string &path,string &category);
        /**bref import txt data from path*/
        void importText(string &name, string &path,int index);
        /**bref import tab dictionary from path*/
        void importDictTab(string &name, string &path,string &category,int year);
        
        //memory request functions
        void select(string&tableName,vector<int> &field, string &query,int searchMode, vector<TString> &result,int limit);
        
        void exportAllRecords(string&tableName,string path);
        
        void memoryRequest(string &destString);
        void buildFileDatabase(string &path);
        void buildFileDatabase(const char*path_){string path=path_; buildFileDatabase(path);}
        void textTagger();

        
    };
};
// ---------------- IMPLEMENTATION
# endif

