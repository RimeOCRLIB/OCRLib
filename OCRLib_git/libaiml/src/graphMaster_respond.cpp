
#include "std_util.h"
#include <ctime>
#include <cmath>
#include "config.h"
#include "core.h"
#include "php2stl.h"


using namespace std;
using namespace aiml;



bool cGraphMaster::respordTempl(templData d,cUser &user ) {



    
    return 1;
}

bool cGraphMaster::templConditionBlock(templData &d,string &output,tPosition &t){
    
    string name, value;
    if (!(t.chars_read = d.templ->readString(name))) return false;
    t.chars_read_tag += t.chars_read;
    if (!(t.chars_read = d.templ->readString(value))) return false;
    t.chars_read_tag += t.chars_read;
    
    bool matches = (value.empty() && person->getVar(name).empty());
    if (!matches && !value.empty()) {
        list<string> cond_list, value_list;
        tokenizeToList(person->getVar(name), cond_list, true, ".");
        tokenizeToList(value, value_list, true, "*");
        matches = isMatch(cond_list, value_list, 0);
    }
    if (matches) {
        string inner;
        size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
        mainStringSize=(uint)mainString.size();
        if (!readTemplate(d,inner)) return false;   //rec+1
        mainString.resize(mainStringSize);
        d.templ->restoreSize(old_size);
        //d.templ_str += inner;
        mainString+=inner;
    }
    else d.templ->seek(t.len - t.chars_read_tag, d.templ->tell());
    
    return 1;
}


bool cGraphMaster::templConditionMulty(templData &d,string &output,tPosition &t){
    
    int print=1;
    string name, value;
    if (t.cond_type == TEMPL_CONDITION_SINGLE) {
        if (!(t.chars_read = d.templ->readString(name))) return false;
        t.chars_read_tag += t.chars_read;
    }

    size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
    while(!d.templ->at_end()) {
        size_t chars_read_li = 0;
        
        // get header
        size_t li_len, li_type, li_subtype;
        if (!(t.chars_read = d.templ->readNumber(li_len))) return false;
        chars_read_li += t.chars_read;
        if (!(t.chars_read = d.templ->readNumber(li_type))) return false;
        chars_read_li += t.chars_read;
        
        DI( "li len: " << li_len << " type: " << li_type << endl);
        
        // skip any character data found between tags
        if (li_type != TEMPL_LI) return false;
        
        if (!(t.chars_read = d.templ->readNumber(li_subtype))) return false;
        chars_read_li += t.chars_read;
        
        bool matches = true;
        
        if ((t.cond_type == TEMPL_CONDITION_SINGLE && li_subtype == TEMPL_LI_VALUE) ||
            (t.cond_type == TEMPL_CONDITION_MULTI && li_subtype == TEMPL_LI_NAME_VALUE))
        {
            
            if (t.cond_type == TEMPL_CONDITION_MULTI) {
                if (!(t.chars_read = d.templ->readString(name))) return false;
                chars_read_li += t.chars_read;
            }
            if (!(t.chars_read = d.templ->readString(value))) return false;
            chars_read_li += t.chars_read;
            
            matches = (value.empty() && person->getVar(name).empty());     DI(" value="<<value<<endl);
            if (!matches && !value.empty()) {
                list<string> cond_list, value_list;
                tokenizeToList(person->getVar(name), cond_list, true, ".");  DI(" name="<<name<<" value="<<person->getVar(name)<<" map="<<person->dumpVar();)
                tokenizeToList(value, value_list, true, "*");
                matches = isMatch(cond_list, value_list, 0);
                DI( "matches [" << name << "] with [" << value << "]?: " << matches << endl);
            }
        }
        else if (li_subtype != TEMPL_LI_DEFAULT) return false;
        
        if (matches) {
            
            string inner;
            
            size_t old_size_li = d.templ->limitSize(li_len - chars_read_li);
            //mainStringSize=(uint)mainString.size();
            if (!readTemplate(d,inner) ) return false;  //rec+1)
            //mainString.resize(mainStringSize);
            d.templ->restoreSize(old_size_li);
            //d.templ_str += inner;
            //mainString+=inner;
            DI( "matched! mainString=" <<mainString<<endl);
            output+=inner;
            d.templ->to_end();
            break;
        }
        else d.templ->seek(li_len - chars_read_li, d.templ->tell());
    }
    d.templ->restoreSize(old_size);

    return 1;
}


bool cGraphMaster::templSrai(templData &d,
                             string &output,
                             tPosition &t){
    
    int print = 0;
    string inner;
    
    DI("@@@@ d.templ_str="<<d.templ_str<<endl;)
    
    size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
    mainStringSize=(uint)mainString.size();// save mainString
    if (!readTemplate(d,inner)) return false;//rec+1
    mainString.resize(mainStringSize); //restore mainString
    d.templ->restoreSize(old_size);
    
    
    vector<string> sentences;
    normalize(inner, sentences);   DI(d.templ_str);
    
    string tmp;
    for (vector<string>::iterator it = sentences.begin(); it != sentences.end(); ++it) {
        string single_response="";
        if(dbOnDisk){
            if (getAnswerDB(*it, single_response)) tmp += single_response + " ";
            else break;
        }else{
            if (getAnswer(*it, single_response, d.log)) tmp += single_response + " ";
            else break;
        }
        
    }
    //mainString+=tmp;
    output+=tmp;

    return 1;
}

bool cGraphMaster::parseKeyAnswer(string &key,const string &input,MatcherStruct& ms){
    
    int print=0;
    DI(" key="<<key<<endl;)
    DI(" input="<<input<<endl;)
    
    vector <string>keyVect=explode(" ", key);
    vector <string>inputVect=explode(" ", input);
    vector <string>starVect;
    string starText;
    int indexIn=0;
    int indexKey=0;
    
    PR(for(int i=0;i<keyVect.size();i++){DR(i<<" "<<keyVect[i]<<endl;)})
    
    while(indexIn<inputVect.size()&&indexKey<keyVect.size()){
        DR("keyVect["<<indexKey<<"]="<<keyVect[indexKey]<<endl);
        if(keyVect[indexKey]=="*"){
            indexKey++;
            if(indexKey<keyVect.size()){
                while(inputVect[indexIn].find(keyVect[indexKey])==-1&&
                    indexIn<inputVect.size()){
                    starText+=inputVect[indexIn]+" "; indexIn++;
                }
            }else{
                while(indexIn<inputVect.size()){
                    starText+=inputVect[indexIn]+" "; indexIn++;
                }
            }
            starVect.push_back(starText); starText="";
            //starText+=
        }
        
        while(indexIn<inputVect.size()&&
              indexKey<keyVect.size()){
            if(inputVect[indexIn].find(keyVect[indexKey])!=-1){
                indexIn++;
                indexKey++;
                continue;
            }else{
                if(keyVect[indexKey]=="*")break;
                indexIn++;
            }
        }
    
    }
    
    for(int i=0;i<starVect.size();i++){
        DR(i<<" "<<starVect[i]<<endl);
        ms.sh.AddStar(starVect[i],NODE_PATT);
    }


    return true;
}