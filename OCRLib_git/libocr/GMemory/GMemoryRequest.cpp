#include "GMemory.h"
#include "GVector.h"
#include "php2stl.h"


//memory request functions
void GMemory::select(string&tableName,vector<int> &field, string &query, int searchMode,vector<TString> &result,int limit){
    string tName=tableName;
    tableRecord &rec=table[tName];
    if(!rec.status)loadTable(tableName);  //if table not exsist, create it
    //cout<<"status="<<rec.status<<" name="<<rec.name;
    switch(searchMode){
        case LIST_SEARCH:{   //get all words started from query
            //cout<<"word list search mode="<<endl;
            //First record in field it is field for search
            int searchField=field[0];
            //create index for requested field.
            indexRecord &indRec=createIndex(rec,searchField,searchMode);
            //print(indRec.status);
            map<string,int>resultMap;
            GVector *data=indRec.vIndex;
            int len=data->size()-1;
            string str;
            for(int n=len;n>-1;n--){
                data->getStr(n,str);
                if(str.find(query)==0){
                    resultMap[str]++;
                    if(resultMap.size()>limit)break;
                }
            }
            result.resize(resultMap.size());
            map<string,int>::iterator iter;
            int n=0;
            for (iter = resultMap.begin(); iter != resultMap.end(); ++iter){
                string st=iter->first;
                result[n].push_back(st);
                //print(iter->first);
                n++;
            }
            break;
        }
        case HASH_SEARCH:{
            int searchField=field[0];
            //create index for requested field.
            indexRecord &indRec=createIndex(rec,searchField,searchMode);

            if(limit==1){
                int indexInVector=indRec.mIndex->getHKey(query,searchField);
                if(indexInVector>-1){
                    TString st;
                    rec.data->getTStr(indexInVector,&st);
                    result.push_back(st);
                }
            }else{
                vector<uint>indexInVector;
                indRec.mIndex->getHKey(indexInVector,query,searchField);
                if(indexInVector.size()){
                    for(int i=0;i<indexInVector.size();i++){
                        TString st;
                        rec.data->getTStr(indexInVector[i],&st);
                        result.push_back(st);
                    }
                }
            }
            break;
        }
        case TEXT_SEARCH:{
            int searchField=field[0];
            //create index for requested field.
            indexRecord &indRec=createIndex(rec,searchField,searchMode);
            if(limit==1){
                int indexInVector=indRec.mIndex->getKey(query,ANY_MATCH);
                if(indexInVector>-1){
                    TString st;
                    rec.data->getTStr(indexInVector,&st);
                    result.push_back(st);
                }
            }else{
                vector<uint>indexInVector;
                indRec.mIndex->getKey(query,indexInVector,ANY_MATCH);
                if(indexInVector.size()){
                    for(int i=0;i<indexInVector.size();i++){
                        TString st;
                        rec.data->getTStr(indexInVector[i],&st);
                        result.push_back(st);
                    }
                }
            }
            break;
        }

    }
    
}




