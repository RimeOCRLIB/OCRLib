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
            indexRecord *indRec=createIndex(rec,searchField,searchMode);
            //print(indRec.status);
            map<string,int>resultMap;
            GVector *data=indRec->vIndex;
            int len=(uint)data->size()-1;
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
            indexRecord *indRec=createIndex(rec,searchField,searchMode);

            if(limit==1){
                int indexInVector=(int)indRec->mIndex->getHKey(query,searchField);
                if(indexInVector>-1){
                    TString st;
                    rec.data->getTStr(indexInVector,st);
                    st.save();
                    result.push_back(st);
                }
            }else{
                vector<ulong>indexInVector;
                indRec->mIndex->getHKey(indexInVector,query,searchField);
                if(indexInVector.size()){
                    for(int i=0;i<indexInVector.size();i++){
                        TString st;
                        rec.data->getTStr(indexInVector[i],st);
                        st.save();
                        result.push_back(st);
                    }
                }
            }
            break;
        }
        case TEXT_SEARCH:{
            int searchField=field[0];
            //create index for requested field.
            indexRecord *indRec=createIndex(rec,searchField,searchMode);
            if(limit==1){
                ulong indexInVector=indRec->mIndex->getKey(query,ANY_MATCH);
                if(indexInVector>-1){
                    TString st;
                    rec.data->getTStr(indexInVector,st);
                    result.push_back(st);
                }
            }else{
                vector<ulong>indexInVector;
                indRec->mIndex->getKey(query,indexInVector,ANY_MATCH);
                if(indexInVector.size()){
                    for(int i=0;i<indexInVector.size();i++){
                        TString st;
                        string str;
                        rec.data->getStr(indexInVector[i],str);
                        st.push_back(str);
                        st.push_back((uint)indexInVector[i]);
                        result.push_back(st);
                    }
                }
            }
            break;
        }
        case FULL_TEXT_SEARCH:{
            int searchField=field[0];
            //create index for requested field.
            indexRecord *indRec=createIndex(rec,searchField,searchMode);
            
            TString st;
            //rec.data
            //indRec.mIndex->valueData_vector->getTStr(6,st);
            //cout<<6<<st[1]<<endl;
            
            if(limit==1){
                ulong indexInVector=indRec->mIndex->getKey(query,ANY_MATCH);
                if(indexInVector>-1){
                    rLink *st;
                    ulong s;
                    st=(rLink*)indRec->mIndex->valueData_vector->getPtr(indexInVector,&s);
                    TString sT;
                    sT.push_back((char*)st, sizeof(rLink));
                    result.push_back(sT);
                }
            }else{
                vector<ulong>indexInVector;                
                indRec->mIndex->getKey(query,indexInVector,ANY_MATCH);
                if(indexInVector.size()){
                    for(int i=0;i<indexInVector.size();i++){
                        rLink *st;
                        string s;
                        indRec->mIndex->valueData_vector->getStr(indexInVector[i],s);
                        st=(rLink*)&s[0];
                        //cout<<indexInVector[i]<<" i:"<<st->index<<" "<<(short)st->field<<" "<<(short)st->offset<<endl;
                        TString sT;
                        sT.push_back((char*)st, sizeof(rLink));
                        result.push_back(sT);
                    }
                }
            }
            break;
        }case STRING_SEARCH:{
            int searchField=field[0];
            GVector *v=rec.data;
            //cout<<"size:"<<v->size()<<endl;
            for(int i=0; i<v->size(); i++){
                TString st;
                v->getTStr(i, st);
                //cout<<"s:"<<st.size()<<" @"<<endl;
                int start,end;
                if(searchField==ALL_FIELD){
                    start=0;end=st.size();
                }else{
                    start=searchField;
                    end=searchField+1;
                }
                for(int n=start;n<end;n++){
                    //cout<<"n:"<<n<<" s:"<<st[n]<<endl;
                    int offset=(int)st[n].find(query);
                    if(st[n].size()&&offset!=-1){
                        TString stRes;
                        stRes.push_back(i);
                        stRes.push_back(n);
                        stRes.push_back(offset);
                        if(limit==1){
                            result.push_back(stRes);
                            return;
                        }else{
                            result.push_back(stRes);
                            //cout<<i<<" ->> "<<result[0].getInt(0)<<" "<<result[0].getInt(1)<<" "<<result[0].getInt(2)<<endl;
                        }
                    }
                }
            }
        break;
        }case ID_SEARCH:{
            int searchField=field[0];
            //create index for requested field.
            indexRecord *indRec=createIndex(rec,searchField,searchMode);
            TString st;
            vector<pLink>searchResult;
            //rec.data
            //indRec.mIndex->valueData_vector->getTStr(6,st);
            //cout<<6<<st[1]<<endl;
            
            string key=query;
            setSearchText(key);
            key+="\n";
            
            ((GLogicProcessor*)inputData.logicProcessor)->encodeByID(key);
            
            if(limit==1){
                indRec->mIndex->getKeyID(key,searchResult);
                if(searchResult.size()){
                    TString st;
                    //rec.data->getTStr(indexInVector,st);
                    result.push_back(st);
                }
            }else{
                
                indRec->mIndex->getKeyID(key,searchResult);
                if(searchResult.size()){
                    for(int i=0;i<searchResult.size();i++){
                        TString st;
                        //indRec->mIndex->valueData_vector->getTStrData(indexInVector[i],st);
                        //cout<<indexInVector[i]<<" "<<st.getInt(0)<<" "<<st.getInt(1)<<" "<<st.getInt(2)<<endl;
                        result.push_back(st);
                    }
                }
            }

            break;
        }

    }

}
