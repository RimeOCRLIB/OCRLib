
//
//  graphmasterDB.cpp
//  OCRLib
//
//  Created by dharmabook on 24/05/15.
//
//

#include "std_util.h"
#include <list>
#include <stack>
#include <regex>
#include "config.h"
#include "core.h"
#include "levenshteinDistance.h"
//#include "DemerayLevinshteinDistance.h"


using namespace std;
using namespace aiml;
using namespace aiml;
using std_util::gettok;
using std_util::strip;

bool cGraphMaster::getAnswerDB(string& input, string& output) {
    
    if(input.size()<4)return false;
    //int print=1;
    DI("input str="<<input<<" size="<<input.size()<<endl);
    MatcherStruct ms(*avatar);
    static int startCount=0;
    if(startCount>10){
         if(output=="")output="no idea about that";
         return 1;
    }
    startCount++;

    
    if(dbOnDisk){
        //cout<<"dbOnDisk; input str="<<input<<endl;

        string str=input;
        string output;
        vector<uint>searchResult;
        vector<sortData>sort_;
        
        TString strT;
        //test in long memory
        ai.longMemory->getAIML(str,searchResult,1);
        DI("searchResult="<<searchResult.size()<<endl);
        ms.memBase=0; ms.memVector=ai.longMemoryData;
        parseSearchResult(input,output,searchResult,sort_,ms);
        searchResult.resize(0);

        //test in main memory
        ai.mainMemory->getAIML(str,searchResult,1);
        DI("searchResult="<<searchResult.size()<<endl);
        ms.memBase=1; ms.memVector=ai.mainMemoryData;
        parseSearchResult(input,output,searchResult,sort_,ms);
        searchResult.resize(0);
        
        //test in short memory
        ai.memory->getAIML(str,searchResult,1);
        DI("searchResult1="<<searchResult.size();)
        if(searchResult.size())DI(" searchResult[0]="<<searchResult[0]<<endl);
        ms.memBase=2; ms.memVector=ai.memoryData;
        parseSearchResult(input,output,searchResult,sort_,ms);
        ms.memBase=100; searchResult.resize(0);
        parseSearchResult(input,output,searchResult,sort_,ms);

        
        //анализ топика
        string topic=avatar->getVar("topic");
        
               //output.clear();
        DI("output="<<output<<endl);
        
        //output="Hello World!";
    }
    return 1;
}


bool cGraphMaster::parseSearchResult(string &input,
                                     string& output,
                                     vector<uint>&searchResult,
                                     vector<sortData>&sort_,
                                     MatcherStruct &ms){
    
    static map<int,int>testID;

    int print=1;
    GVector *memData;

    
    if(ms.memBase<100){
        memData=ms.memVector;
        for(int i=0;i<searchResult.size();i++){
            gNode *node=gNode::create(memData,searchResult[i]);
            if(node->key.size()>input.size()+10){node->destroy(); continue;}
            //DI("key="<<node->key<<endl;)
            if(node->type==NODE_PATT){
                sortData s; s.ID=searchResult[i];
                s.len=(int)node->key.size(); s.topic=node->topic; s.memBase=ms.memBase, s.path=node->path;
                sort_.push_back(s);
            }
            node->destroy();
        }
        return 1;
    }
    GVector *memVect[3];
    memVect[0]=ai.longMemoryData;
    memVect[1]=ai.mainMemoryData;
    memVect[2]=ai.memoryData;
    

    sort(sort_.begin(),sort_.end(),sortSearchData);
    
    DI("SEARCH sort.siz()="<<sort_.size()<<endl;)
    
    for(int i=0;i<sort_.size();i++){

        gNode *node=gNode::create(memVect[sort_[i].memBase],sort_[i].ID);
        DI(i<<" key="<<node->key<<" -> "<<node->path<<endl);
        string key=str_replace("*",".*", node->key);
        key=str_replace("_",".*",key);
        //key=str_replace(" .",".",key);
        //key=str_replace("* ","*",key);
        
        std::regex key_regex(key);
        string new_s = std::regex_replace(input, key_regex, "@");
        
        if(new_s.length()!=input.length()){
            DI("@choice  sort_["<<i<<"]."<<sort_[i].ID<<"="<<key<<" ->"<<sort_[i].path<<endl);
            
            if(key.find("*")!=-1){
                //we need split answer according to * in pattern and strore in
                //MatcherStruct
                parseKeyAnswer(node->key,input,ms);
            }
            ms.templ=node->templ;
            DI(" size="<<node->templ.data.size()<<endl);
            
            if(print){
                string data_; data_.resize(node->templ.data.size());
                memcpy(&data_[0],&node->templ.data[0],node->templ.data.size());
                DI(" templ="<<data_<<endl);
            }
            
            ms.templ.seek(0);
            cReadBuffer read_buf(ms.templ);
            
            //remove loop in answers
            if(testID.find(sort_[i].ID)!=testID.end()){
                DI("match"<<endl);
                continue;
            }
            testID[sort_[i].ID]=1;
            /////
            templData d;
            d.sh=&ms.sh;
            d.templ_str=input;
            d.templ=&read_buf;
            d.rec=0;
            if(node->topic!="")avatar->setVar("topic",node->topic);
            DI("topic="<<avatar->getVar("topic")<<endl);
            if (!readTemplate(d,output)){   //read_buf, ms.sh, ms.user, output, log
                continue;
            } //return 0;
            node->destroy();
            return 1;
        }
        node->destroy();
    }
    
    return 0;

}




/*
 if(!answerFound){
 //DamerauLevensteinMetric dist(512);
 int minDist=10000;
 int minDistID=-1;
 for(int i=0;i<sort_.size();i++){
 gNode *node=gNode::create(rootData,sort_[i].ID);
 
 int d=(int)levenshteinDistance(node->key,input);
 //int d=(int)dist.GetDistance(node->key,input,512);
 if(d<minDist){
 minDist=d; minDistID=i;
 }
 DI("key="<<node->key<<" d="<<d<<endl);
 
 }
 if(minDistID>=-1){
 gNode *node=gNode::create(rootData,searchResult[minDistID]);
 ms.templ=node->templ;
 //DI(" size="<<node->templ.data.size()<<endl);
 DI(endl<<" choice="<<node->key<<endl);
 string data_; data_.resize(node->templ.data.size());
 memcpy(&data_[0],&node->templ.data[0],node->templ.data.size());
 //cout<<" templ="<<data_<<endl;
 ms.templ.seek(0);
 cReadBuffer read_buf(ms.templ);
 templData d;
 d.sh=&ms.sh;
 d.templ_str=input;
 d.templ=&read_buf;
 d.rec=0;
 if(node->topic!="")user.setVar("topic",node->topic);
 DI("topic="<<user.getVar("topic")<<endl);
 
 if (!readTemplate(d,output,user)){
 DI(" no read template");
 } //return 0;
 }
 
 }
 */
