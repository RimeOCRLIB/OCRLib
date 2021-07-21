//
//  loadDB.cpp
//  OCRLib
//
//  Created by dharmabook on 23/07/15.
//
//

#include <stdio.h>
#include "GMemory.h"

static bool sortKeyL(string &d1, string &d2) {
    return d1.size() > d2.size();
}

/**bref load table in mmap memory by name and all table index GMap. If table not exist create it in default data path */
void GMemory::loadTable(string &name) {
    if(table[name].status==0) {
        tableRecord rec;
        string      pathTable=inputData.data["DATA_DB"] + name;
        if(!is_dir(pathTable.c_str())) { mkdir(pathTable.c_str(), ACCESSPERMS); }
        string path=pathTable + "/" + name + ".bin";

        //cout<<" path:"<<path<<endl;
        rec.data=GVector::create(path);
        rec.status=1;
        rec.name=name;
        GVector *tableVector=rec.data;
        //cout<<" size:"<<tableVector->size()<<endl;
        // check data in tabel and import from default source if table is empty
        if(!tableVector->size()) {
            path=pathTable + "/" + name + ".tab";
            if(is_file(path)) {
                tableVector->importTAB(path);
                goto EXIT_IF;
            }else{
                path=pathTable + "/" + name + ".csv";
                if(is_file(path)) {
                    tableVector->importTAB(path);
                    goto EXIT_IF;
                }
            }
            path=pathTable + "/" + name + ".txt";
            if(is_file(path)) {
                tableVector->importTXT(path);
                goto EXIT_IF;
            }
            path=pathTable + "/" + name + ".xml";
            if(is_file(path)) {
                tableVector->importXML(path);
                goto EXIT_IF;
            }
        }
    EXIT_IF:;
        rec.status=1;
        table[name]=rec;
        
    }
}

/**bref export table in default DATA_PATH*/
void GMemory::exportTable(string &name, string &ext) {
    loadTable(name);
    GVector *v=table[name].data;

    if(ext=="txt") {
        string pathTable=inputData.data["DATA_DB"] + name + "/" + name + ".tab";
        v->exportTStr(pathTable);
        return;
    }
    if(ext=="xml") {
        string pathTable=inputData.data["DATA_DB"] + name + "/" + name + ".xml";
        v->exportXML(pathTable);
        return;
    }
}

/**bref import table data from path*/
void GMemory::importTable(string &name, string &path) {
    loadTable(name);
    GVector *v=table[name].data;
    string   ext=substr((uint)path.size() - 3, 3, path);

    if(ext=="txt") {
        v->importTXT(path);
        return;
    }
    if(ext=="tab") {
        v->importTAB(path);
        return;
    }
    if(ext=="xml") {
        v->importXML(path);
        return;
    }
}

/**bref import OCR data from path*/
void GMemory::importOCR(string &name, string &path, string &delimeter) {
    loadTable(name);
    GVector *      v=table[name].data;
    vector<string> fileList;
    string         text;
    string         ext="txt";
    string         str;
    time_t         timeStamp=time(nullptr);

    ostringstream out;
    out<<"test";

    readDirectoryToArray(fileList, path, ext);
    for(int i=0; i < fileList.size(); i++) {
        cout<<fileList[i]<<endl;
        text=readText(fileList[i]);
        text=str_replace("@", "", text);
        text=str_replace("\n", "¶", text);
        text=str_replace("\r", "¶", text);
        text=str_replace("¶¶", "¶", text);
        text=str_replace("¶¶", "¶", text);
        vector<string> t;
		explode(delimeter, text,t);
        TString        st;
        out.str("");
        out<<"[[ID:"<<timeStamp<<"]]";
        str=out.str();
        st.push_back(str);
        str="[[Category:" + fileName(path) + "]]¶[[Category:Tibetan text]]¶[[Category:bookOCR]]¶{{Path|" + path + "}}";
        st.push_back(str);

        for(int n=0; n < t.size(); n++) {
            if(t[n].size() < 3) continue;
            str=delimeter + t[n];
            st.push_back(str);
        }
        v->push_back(st);
        timeStamp++;
    }
}

/**bref import txt data from path*/
void GMemory::importTextFolder(string &name, string &path, string &category) {
    loadTable(name);
    GVector *      v=table[name].data;
    vector<string> fileList;
    string         text;
    string         ext="txt";
    string         str;
    time_t         timeStamp=time(nullptr);

    ostringstream out;
    out<<"test";

    readDirectoryToArray(fileList, path, ext);
    for(int i=0; i < fileList.size(); i++) {
        cout<<fileList[i]<<endl;
        text=readText(fileList[i]);
        text=str_replace("\n", "¶", text);
        text=str_replace("། ", "། ¶", text);
        text=str_replace("\r", "¶", text);
        text=str_replace("¶¶", "¶", text);
        text=str_replace("¶¶", "¶", text);
        // split on pages
        vector<string> lines;
		explode("¶", text,lines);
        vector<string> t;
        str="";
        int n=0;
        while(n < lines.size()) {
            while(str.size() < 2048 && n < lines.size()) {
                str +=lines[n] + "¶";
                n++;
            }
            t.push_back(str);
            str="";
            n++;
        }

        TString st;
        out.str("");
        out<<"[[ID:"<<timeStamp<<"]]";
        str=out.str();
        st.push_back(str);
        int flag=0;
        for(int n=0; n < t.size(); n++) {
            if(t[n].size() < 3) continue;
            flag++;
            str=t[n];
            // cout<<str;
            if(flag==1) {
                str="[[Category:" + fileName(path) + "]]¶[[Category:" + category + "]]¶{{Path|" + fileName(fileList[i]) + "}}¶" + str;
            }

            st.push_back(str);
        }
        v->push_back(st);
        timeStamp++;
    }
}

/**bref import txt data from path*/
void GMemory::importText(string &name, string &path, int index) {
    loadTable(name);
    GVector *      v=table[name].data;
    vector<string> fileList;
    string         text;
    string         ext="txt";
    string         str;
    time_t         timeStamp=time(nullptr);

    ostringstream out;

    text=readText(path);
    text=str_replace("\n", "¶", text);
    text=str_replace("། ", "། ¶", text);
    text=str_replace("\r", "¶", text);
    text=str_replace("¶¶", "¶", text);
    text=str_replace("¶¶", "¶", text);
    // split on pages
    vector<string> lines;
    int            flagOCR=0;
    if(text.find("{page_") !=-1) {
        explode("{page_", text,lines);
        flagOCR=1;
    } else {
        explode("¶", text,lines);
    }
    TString st;
    if(!flagOCR) {
        vector<string> t;
        str="";
        int n=0;
        while(n < lines.size()) {
            while(str.size() < 2048 * 5 && n < lines.size()) {
                str +=lines[n] + "¶";
                n++;
            }
            t.push_back(str);
            str="";
            n++;
        }

        out.str("");
        out<<"[[ID:"<<timeStamp<<"]]";
        str=out.str();
        st.push_back(str);
        int flag=0;
        for(int n=0; n < t.size(); n++) {
            if(t[n].size() < 3) continue;
            flag++;
            str=t[n];
            // cout<<str;
            if(flag==1) { str="{{Path|" + fileName(path) + "}}¶" + str; }

            st.push_back(str);
        }
    } else {
        out.str("");
        out<<"[[ID:"<<timeStamp<<"]]";
        str=out.str();
        st.push_back(str);
        for(int n=0; n < lines.size(); n++) {
            str="{page_" + lines[n];
            // cout<<str;
            if(n==0) { str="{{Path|" + fileName(path) + "}}¶" + str; }

            st.push_back(str);
        }
    }
    v->putTStr(index, st);
}

void GMemory::importDictTab(string &name, string &path, string &category, int year) {
    loadTable(name);
    GVector *      v=table[name].data;
    vector<string> fileList;
    string         text;
    string         ext="tab";
    string         str;
    time_t         timeStamp=time(nullptr);
    timeStamp -=60 * 60 * 24 * year;  // 12 years ago;

    ostringstream out;
    out<<"test";

    readDirectoryToArray(fileList, path, ext);
    for(int i=0; i < fileList.size(); i++) {
        cout<<fileList[i]<<endl;
        vector<string> lines;
        readText(lines, fileList[i]);
        str="";

        for(int n=0; n < lines.size(); n++) {
            if(n % 1000==0) cout<<"."<<n;
            TString st;
            out.str("");
            out<<"[[ID:"<<timeStamp<<"]]";
            str=out.str();
            st.push_back(str);

            vector<string> t;
			explode("\t", lines[n],t);
            if(lines[n].size() < 2) continue;
            str="[[Category:Dictionary]]¶[[Category:Dictionary__" + category + "]]¶[[Title:" + t[0] + "]]¶" + t[1];

            st.push_back(str);
            v->push_back(st);
            timeStamp++;
        }
    }
}

void GMemory::reloadIndex(tableRecord &rec, int searchField, int searchMode) {
    ostringstream out;
    out<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
    string path=out.str();
    remove(path.c_str());
    int indexAddress=searchField * INDEX_COUNT + searchMode;
    rec.index[indexAddress].status=0;
    createIndex(rec, searchField, searchMode);
}

void GMemory::exportAllRecords(string &tableName, string path) {
    path +="/";
    path=str_replace("//", "/", path);
    tableRecord &rec=table[tableName];
    if(!rec.status) loadTable(tableName);  // if table not exsist, create it
    GVector *dict=rec.data;
    for(int n=0; n < dict->size(); n++) {
        TString st;
        dict->getTStr(n, st);
        string res;
        string fileName=path + st[0] + ".txt";
        if(!st.size()) continue;
        for(int n=0; n < st.size(); n++) {
            string s=st[n];
            res +=s + "\n";
        }
        writeText(res, fileName.c_str());
    }
};

indexRecord *GMemory::createIndex(tableRecord &rec, int searchField, int searchMode) {
    int indexAddress=searchField * INDEX_COUNT + searchMode;

    if(rec.index.size() < indexAddress + 1) rec.index.resize(indexAddress + 1);  // one record for every type of index for every field
    // check index status. if index not exist, create it.
    if(!rec.index[indexAddress].status) {
        // cout<<"start create index for "<<searchField<<" field"<<endl;
        ostringstream out;
        out<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
        string path=out.str();
        // cout<<path<<endl;

        switch(searchMode) {
            case LIST_SEARCH: {
                GVector *dictIndex=GVector::create(path);
                GVector *dict=rec.data;
                rec.index[indexAddress].vIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictIndex->size()<<endl;
                if(!dictIndex->size()) {
                    uint sizeV=(uint)dict->size();
                    cout<<"start reload dictionary key "<<sizeV<<endl;

                    vector<string> vDict;
                    for(uint a=0; a < sizeV; a++) {
                        TString st;
                        dict->getTStr(a, st);
                        vDict.push_back(st[0]);
                    }
                    cout<<"start sort"<<endl;
                    sort(vDict.begin(), vDict.end(), sortKeyL);
                    for(int a=0; a < vDict.size(); a++) {
                        // cout<<vDict[a].key<<endl;
                        dictIndex->push_back(vDict[a]);
                    }
                    // cout<<"done dictWords->size()="<<dictIndex->size()<<" sizeV="<<sizeV<<endl;
                }
                break;
            }
            case HASH_SEARCH: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                if(dict->size() < 50000) {
                    dictIndex->hashLimit=10;
                } else if(dict->size() < 500000) {
                    dictIndex->hashLimit=9;
                } else {
                    dictIndex->hashLimit=8;
                }

                rec.index[indexAddress].mIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()) {
                    cout<<"start build index HASH "<<path<<endl;
                    // cout<<dictIndex->size()<<endl;
                    dictIndex->addRecords(dict, searchField, OCR_DICT_HASH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case TEXT_SEARCH: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                rec.index[indexAddress].mIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, searchField, TEXT_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case ID_SEARCH_DATA: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                rec.index[indexAddress].mIndex=dictIndex;
                cout<<"done dict->size()="<<dict->size()<<endl;
                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, searchField, ID_SEARCH_DATA);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case FULL_TEXT_SEARCH: {
                GVector *     dict=rec.data;
                ostringstream out_;
                string        selfPath;
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
                selfPath=out_.str();
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode
                    <<"_data.index";
                path=out_.str();
                GVector *dictTextData=GVector::create(path);
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode
                    <<"_dict.index";
                path=out_.str();
                GStr<mSIZE> *dictDictData=GStr<mSIZE>::create(path.c_str());

                GMap *dictIndex=GMap::create(selfPath, dictTextData, dictDictData);

                rec.index[indexAddress].mIndex=dictIndex;

                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, searchField, FULL_TEXT_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case TEXT_DICT_SEARCH: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                rec.index[indexAddress].mIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, 0, TEXT_DICT_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case LETTER_SEARCH: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                rec.index[indexAddress].mIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, searchField, LETTER_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case OCR_SEARCH: {
                GVector *dict=rec.data;
                GMap *   dictIndex=GMap::create(path, dict);
                rec.index[indexAddress].mIndex=dictIndex;
                // cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()) {
                    dictIndex->addRecords(dict, 0, OCR_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case ID_SEARCH: {
                GVector *     dict=rec.data;
                ostringstream out_;
                string        selfPath;
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
                selfPath=out_.str();
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode
                    <<"_data.index";
                path=out_.str();
                GVector *dictTextData=GVector::create(path);
                out_.str("");
                out_<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode
                    <<"_dict.index";
                path=out_.str();
                GStr<mSIZE> *dictDictData=GStr<mSIZE>::create(path.c_str());

                GMap *dictIndex=GMap::create(selfPath, dictTextData, dictDictData);
                rec.index[indexAddress].mIndex=dictIndex;

                if(!dictIndex->size()) {
                    string str="WORDLIST";
                    loadTable(str);
                    dictIndex->wordListCount=table[str].data->size();
                    dictIndex->addRecords(dict, searchField, ID_SEARCH);
                    dictIndex->save();
                    // cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
        }
        rec.index[indexAddress].status=1;
    }
    return &rec.index[indexAddress];
}

/**bref load all databases in mmap memory by request in inputData global structure*/
void GMemory::loadDB() {
    string db=inputData.data["db"];
    string ln=inputData.data["ln"];
    string mode=inputData.data["mode"];
    string pathCorpus;
    string path;

    if(inputData.data["loadMode"]=="OCR") {
        // aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
        // read font grammar data
        path="LETTERS_GRAMMAR";
        loadTable(path);
        fontTable=table[path].data;

        // cout<<" table->size()="<<fontTable->size()<<endl;
        indexRecord *indexRec=createIndex(table[path], REC_LETTER_UNI_UTF, HASH_SEARCH);
        fontGMap=indexRec->mIndex;

        indexRec=createIndex(table[path], REC_KEY_UTF, LETTER_SEARCH);
        fontStackGMap=indexRec->mIndex;

        path="OCR_DICTIONARY";
        loadTable(path);
        textCorpus=table[path].data;
        // cout<<"textCorpus->size()="<<textCorpus->size()<<endl;
        indexRec=createIndex(table[path], 0, OCR_SEARCH);
        textCorpusGMap=indexRec->mIndex;
        // path="LETTERS_CORRELATION";
        // loadTable(path);
        // aliKali->correlationVector=table[path].data;
        // cout<<"textCorpus->size()="<<textCorpus->size()<<endl;
        // indexRec=createIndex(table[path],0,HASH_SEARCH);
        // aliKali->correlationMap=indexRec.mIndex;
    }
}
