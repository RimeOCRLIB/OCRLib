//
//  loadDB.cpp
//  OCRLib
//
//  Created by dharmabook on 23/07/15.
//
//

#include <stdio.h>
#include "GMemory.h"

static bool sortKeyL(string& d1, string& d2){
    return d1.size() > d2.size();
}

/**bref load table in mmap memory by name and all table index GMap. If table not exist create it in default data path */
void GMemory::loadTable(string &name){
    string pathTable=inputData.data["DATA_DB"]+name;
    if(!is_dir(pathTable.c_str()) ){
        mkdir(pathTable.c_str(), ACCESSPERMS);
    }
    string path=pathTable+"/"+name+".bin";
    tableRecord &rec=table[name];
    rec.data=GVector::create(path);
    rec.status=1;
    rec.name=name;
    GVector *tableVector=rec.data;
    //check data in tabel and import from default source if table is empty
    if(!tableVector->size()){
        path=pathTable+"/"+name+".tab";
        if(is_file(path)){tableVector->importTAB(path); goto EXIT_IF;}
        path=pathTable+"/"+name+".txt";
        if(is_file(path)){tableVector->importTXT(path); goto EXIT_IF;}
        path=pathTable+"/"+name+".xml";
        if(is_file(path)){tableVector->importXML(path); goto EXIT_IF;}
    }
    EXIT_IF:;
}

void GMemory::reloadIndex(tableRecord &rec,int searchField,int searchMode){
    ostringstream out;
    out<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
    string path=out.str();
    remove(path.c_str());
    createIndex(rec, searchField,searchMode);

}

void GMemory::exportAllRecords(string&tableName,string path){
    path+="/";
    path=str_replace("//", "/", path);
    tableRecord &rec=table[tableName];
    if(!rec.status)loadTable(tableName);  //if table not exsist, create it
    GVector*dict=rec.data;
    for(int n=0;n<dict->size();n++){
        TString st;
        dict->getTStr(n,&st);
        string res;
        string fileName=path+st[0]+".txt";
        if(!st.size())continue;
        for(int n=0;n<st.size();n++){
            string s=st[n];
            res+=s+"\n";
        }
        writeText(res, fileName.c_str());
    }
    
};


indexRecord& GMemory::createIndex(tableRecord &rec, int searchField,int searchMode){
    int indexAddress=searchField*INDEX_COUNT+searchMode;
    
    rec.index.resize(indexAddress+1);  //one record for every type of index for every field
    //check index status. if index not exist, create it.
    if(!rec.index[indexAddress].status){
        //cout<<"start create index for "<<searchField<<" field"<<endl;
        ostringstream out;
        out<<inputData.data["DATA_DB"]<<rec.name<<"/"<<rec.name<<"_"<<searchField<<"_"<<searchMode<<".index";
        string path=out.str();
        
        switch(searchMode){
            case LIST_SEARCH:{
                GVector*dictIndex=GVector::create(path);
                GVector*dict=rec.data;
                rec.index[indexAddress].vIndex=dictIndex;
                //cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()){
                    uint sizeV=dict->size();
                    cout<<"start reload dictionary key "<<sizeV<<endl;
                    
                    vector<string> vDict;
                    for(uint a=0;a<sizeV;a++){
                        TString st;
                        dict->getTStr(a,&st);
                        vDict.push_back(st[0]);
                    }
                    cout<<"start sort"<<endl;
                    sort(vDict.begin(),vDict.end(),sortKeyL);
                    for(int a=0;a<vDict.size();a++){
                        //cout<<vDict[a].key<<endl;
                        dictIndex->push_back(vDict[a]);
                    }
                    cout<<"done dictWords->size()="<<dictIndex->size()<<" sizeV="<<sizeV<<endl;
                    
                }
                break;
            }
            case HASH_SEARCH:{
                GVector*dict=rec.data;
                GMap*dictIndex=GMap::create(path,dict);
                if(dict->size()>500000){
                    dictIndex->hashLimit=9;
                }else{
                    dictIndex->hashLimit=8;
                }
                rec.index[indexAddress].mIndex=dictIndex;
                //cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()){
                    dictIndex->addRecords(dict,searchField,OCR_DICT_HASH);
                    dictIndex->save();
                    //cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case TEXT_SEARCH:{
                GVector*dict=rec.data;
                GMap*dictIndex=GMap::create(path,dict);
                rec.index[indexAddress].mIndex=dictIndex;
                //cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()){
                    dictIndex->addRecords(dict,0,OCR_DICT_WITH_DELIMETERS);
                    dictIndex->save();
                    //cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }
            case OCR_SEARCH:{
                GVector*dict=rec.data;
                GMap*dictIndex=GMap::create(path,dict);
                rec.index[indexAddress].mIndex=dictIndex;
                //cout<<"done dictWords->size()="<<dictWords->size()<<endl;
                if(!dictIndex->size()){
                    dictIndex->addRecords(dict,searchField,OCR_DICT_NO_DELIMETERS);
                    dictIndex->save();
                    //cout<<"@@@dictionaryGMap->size()="<<dictIndex->size()<<endl;
                }
                break;
            }

        }
        rec.index[indexAddress].status=1;
        
    }
    return rec.index[indexAddress];

}



/**bref load all databases in mmap memory by request in inputData global structure*/
void GMemory::loadDB(){
    
    string db=inputData.data["db"];
    string ln=inputData.data["ln"];
    string mode=inputData.data["mode"];
    string pathCorpus;
    string path;
    
    aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor

    if(inputData.data["loadMode"]=="OCR"){
        //read font grammar data
        path="LETTERS_GRAMMAR";
        loadTable(path);
        fontTable=table[path].data;
        //cout<<" table->size()="<<fontTable->size()<<endl;
        indexRecord indexRec=createIndex(table[path],8,HASH_SEARCH);
        fontGMap=indexRec.mIndex;
        //string name="ྃ"; //3530339
        //int ind=indexRec.mIndex->getHKey(name,8);
        
        indexRec=createIndex(table[path],5,OCR_SEARCH);
        fontStackGMap=indexRec.mIndex;
        
        path="OCR_DICTIONARY";
        loadTable(path);
        textCorpus=table[path].data;
        //cout<<"textCorpus->size()="<<textCorpus->size()<<endl;
        indexRec=createIndex(table[path],0,OCR_SEARCH);
        textCorpusGMap=indexRec.mIndex;
        
        path="LETTERS_CORRELATION";
        loadTable(path);
        aliKali->correlationVector=table[path].data;
        //cout<<"textCorpus->size()="<<textCorpus->size()<<endl;
        indexRec=createIndex(table[path],0,HASH_SEARCH);
        aliKali->correlationMap=indexRec.mIndex;
        
    }
    
}