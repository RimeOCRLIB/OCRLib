    /***************************************************************************
 *   This file is part of "libaiml"                                        *
 *   Copyright (C) 2005 by V01D                                            *
 *                                                                         *
 *   "libaiml" is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   "libaiml" is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with "libaiml"; if not, write to the                            *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "std_util.h"
#include <ctime>
#include <cmath>
#include "config.h"
#include "core.h"
#include "php2stl.h"

using namespace std;
using namespace aiml;


bool cGraphMaster::readTemplate(templData &d,string &output) {
    int print=1;
    string tmp;
    
    DI("@@@@ d.templ_str="<<d.templ_str<<" topic"<<person->getVar("topic")<<endl;)
    d.templ_str="";
    output="";
    d.rec++;
    //if(d.rec==7){
    //    DI("test";
    //}
    DI( "[" << d.rec << "]" << endl);
    
    //for(int i=0;i<templ->data.size();i++){
    //     DI( "[" << i << "]" <<(short)templ->data[i]<< " d="<<templ->data[i]<<endl);
    //}
    while (!d.templ->at_end()) {
        DI( "[" << d.rec << "] loop" <<" topic"<<person->getVar("topic")<< endl);
        
        tPosition t;
        
        
        if (!(t.chars_read = d.templ->readNumber(t.len))) return false;
        t.chars_read_tag += t.chars_read;
        if (!(t.chars_read = d.templ->readNumber(t.int_type))) return false;
        t.chars_read_tag += t.chars_read;
        DI( "[" << d.rec << "] len: " << t.len << " type: " << t.int_type<<" chars_read="<<t.chars_read << endl);
        PR(printBinTempl(t.int_type);)
        
        
        switch(t.int_type) {
            case TEMPL_CHARACTERS:
                tmp="";
                // character data doesn't use the 'len' field, it has its own inside the string
                if (!(t.chars_read = d.templ->readString(tmp, true))) return false;
                //d.templ_str+=tmp;
                output+=tmp;
                mainString+=tmp;
                DI( "chardata text len: [" << t.chars_read - 4 << "] at " << (d.templ->tell() - t.chars_read) <<" output="<<output<<" templ_str="<<d.templ_str<<" mainString="<<mainString<<endl);
                break;
            case TEMPL_CONDITION:
            {
                
                
                if (!(t.chars_read = d.templ->readNumber(t.cond_type))) return false;
                t.chars_read_tag += t.chars_read;
                PR(printCond(t.cond_type);)
                
                switch(t.cond_type) {
                    case TEMPL_CONDITION_BLOCK:{
                      templConditionBlock(d,output,t);
                    }
                        break;
                    case TEMPL_CONDITION_SINGLE:
                    case TEMPL_CONDITION_MULTI:{
                        templConditionMulty(d,output,t);
                    }
                        break;
                    default:
                        _DBG_MARK();
                        return false;
                        break;
                }
            }
                break;
                
            case TEMPL_STAR:
            case TEMPL_TOPICSTAR:
            case TEMPL_THATSTAR:
            {
                size_t index1;
                if (!(t.chars_read = d.templ->readNumber(index1))) return false;
                
                const list<string>* curr_star = NULL;
                switch(t.int_type) {
                    case TEMPL_STAR:      curr_star = &d.sh->patt;   break;
                    case TEMPL_TOPICSTAR: curr_star = &d.sh->topic;  break;
                    case TEMPL_THATSTAR:  curr_star = &d.sh->that;   break;
                    default:
                        return false;
                        break;
                }
                tmp="";
                if (index1 >= 1 && (index1 - 1) < curr_star->size()) tmp += getStrListIdx((uint)index1 - 1, *curr_star);
                output+=tmp;
                mainString+=tmp;
                DI("star="<<tmp<<" output="<<output<<endl;)
            }
                break;
                
            case TEMPL_THAT:
            case TEMPL_INPUT:
            {
                size_t index1, index2;
                if (!(t.chars_read = d.templ->readNumber(index1))) return false;
                if (!(t.chars_read = d.templ->readNumber(index2))) return false;
                
                if (t.int_type == TEMPL_THAT) d.templ_str += person->getThat(false, (uint)index1, (uint)index2);
                else d.templ_str += person->getInput((uint)index1, (uint)index2);
            }
                break;
                
            case TEMPL_RANDOM:
            {
                vector<size_t> li_positions;
                
                size_t old_pos = d.templ->tell();
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                DI( "[" << d.rec << "] random" << endl);
                while(!d.templ->at_end()) {
                    DI( "[" << d.rec << "] li " << endl);
                    size_t chars_read_li = 0, chars_read;
                    size_t li_len, li_type;
                    li_positions.push_back(d.templ->tell());
                    
                    if (!(chars_read = d.templ->readNumber(li_len))) return false;
                    chars_read_li += chars_read;
                    if (!(chars_read = d.templ->readNumber(li_type))) return false;
                    chars_read_li += chars_read;
                    
                    DI( "li of len " << li_len << " about to seek to " << li_len - chars_read_li + d.templ->tell() << endl);
                    
                    d.templ->seek(li_len - chars_read_li, d.templ->tell());
                }
                d.templ->restoreSize(old_size);
                
                if (li_positions.empty()) return false;
                int dsize = (int)li_positions.size();
                size_t rand_elem = static_cast<size_t>(rand() % dsize);
                //DI(" dsize="<<dsize<<" rand_elem="<<rand_elem<<endl;
                d.templ->seek(li_positions[rand_elem]);
                
                size_t chars_read_li = 0;
                size_t li_len, li_type, li_subtype;
                if (!(t.chars_read = d.templ->readNumber(li_len))) return false;
                chars_read_li += t.chars_read;
                if (!(t.chars_read = d.templ->readNumber(li_type))) return false;
                chars_read_li += t.chars_read;
                if (!(t.chars_read = d.templ->readNumber(li_subtype))) return false;
                chars_read_li += t.chars_read;
                
                if (li_subtype != TEMPL_LI_DEFAULT) return false;
                
                // interpret the random one
                string li_content;
                old_size = d.templ->limitSize(li_len - chars_read_li);
                mainStringSize=(uint)mainString.size();
                if (!readTemplate(d,li_content)) return false;//rec+1
                mainString.resize(mainStringSize);
                d.templ->restoreSize(old_size);
                
                //d.templ_str += li_content;
                output+=li_content;
                mainString+=li_content;
                d.templ->seek(t.len - t.chars_read_tag, old_pos);
            }
                break;
                
            case TEMPL_GOSSIP:
            case TEMPL_THINK:   //The simple purpose of the <think> X </think> tag pair is to evaluate the AIML expression X, but "nullify" or hide the result from the client reply.
            {
                string inner;
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                size_t old_size_=mainString.size();
                if (!readTemplate(d,inner)) return false;  //rec+1
                mainString.resize(old_size_);
                d.templ->restoreSize(old_size);
                
                if (t.int_type == TEMPL_GOSSIP) toGossip(inner);
            }
                break;
            case TEMPL_LEARN:
            {
                string data;
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                mainStringSize=(uint)mainString.size();
                if (!readTemplate(d,data)) return false;//rec+1
                mainString.resize(mainStringSize);
                d.templ->restoreSize(old_size);
                vector<string>parseData=explode(":|:", data);
                if(parseData.size()>2&&parseData[0]=="#ENTRY"){
                    string str0=parseData[1];
                    string str1=parseData[2];
                    to_uppercase(str0);
                    str0=trim(str0);
                    remember(str0, str1);
                }
                if(parseData.size()>1&&parseData[0]=="#FILE"){
                    reloadMemory(parseData[1]);
                }
            }
                break;
                
            case TEMPL_SRAI:templSrai(d,output,t);  break;
                
                // scripted languages
            case TEMPL_SYSTEM:
            case TEMPL_JAVASCRIPT:
            {
                string chardata;
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                mainStringSize=(uint)mainString.size();
                if (!readTemplate(d,chardata)) return false;//rec+1
                mainString.resize(mainStringSize);
                d.templ->restoreSize(old_size);
                DI("chardata="<<chardata;)
                
                string ret;
                bool call_ret = (aiml_core.doSystemCall(chardata, ret) );
                mainString+=ret;
                output+=ret;
                //bool call_ret = (int_type == TEMPL_SYSTEM ? aiml_core.doSystemCall(chardata, ret) : aiml_core.doJavaScriptCall(chardata, ret));
                if (!call_ret) return false;
            }
                break;
                
            case TEMPL_DATE:
            {
                time_t curr_time;
                time(&curr_time);
                
                string format, time_str;
                d.templ->readString(format);
                string formatted_time;
                
                if (aiml_core.cfg_options.allow_dateformat && !format.empty()) {
                   
                    char formatted_time_buf[512];
                    size_t chars_written = strftime(formatted_time_buf, 512, format.c_str(), localtime(&curr_time));
                    if (chars_written < 512) formatted_time.assign(formatted_time_buf, chars_written);
                    d.templ_str += formatted_time;
                }
                else
                    formatted_time= std_util::strip(ctime(&curr_time), '\n', std_util::RIGHT);
                    //d.templ_str+=tmp;
                    output+=formatted_time;
                    mainString+=formatted_time;
            }
                break;
            case TEMPL_ID:
                //d.templ_str += person->name;
                output+=person->name;
                mainString+=person->name;
                break;
            case TEMPL_SIZE:
            {
                ostringstream osstr;
                osstr << gm_size;
                output+=osstr.str();
                mainString += osstr.str();
                
            }
                break;
            case TEMPL_VERSION:
                mainString += string("libaiml 1.1");
                output += string("libaiml 1.1");

                break;
                
            case TEMPL_PERSON:
            case TEMPL_PERSON2:
            case TEMPL_GENDER:
            case TEMPL_LOWERCASE:
            case TEMPL_UPPERCASE:
            case TEMPL_FORMAL:
            case TEMPL_SENTENCE:
            {
                string value;
                
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                mainStringSize=(uint)mainString.size();
                if (!readTemplate(d,value)) return false;//rec+1
                mainString.resize(mainStringSize);
                d.templ->restoreSize(old_size);
                
                switch(t.int_type) {
                    case TEMPL_PERSON:    toPerson(value);      break;
                    case TEMPL_PERSON2:   toPerson2(value);     break;
                    case TEMPL_GENDER:    toGender(value);      break;
                    case TEMPL_LOWERCASE: to_lowercase(value);  break;
                    case TEMPL_UPPERCASE: to_uppercase(value);  break;
                    case TEMPL_FORMAL:    to_formal(value, aiml_core.cfg_options.sentence_limit); break;
                    case TEMPL_SENTENCE:  to_sentence(value, aiml_core.cfg_options.sentence_limit); break;
                }
                //d.templ_str += value;
                mainString+=value;
                output+=value;
            }
                break;
                
            case TEMPL_SR:
            {
                if (d.sh->patt.empty()) break;
                vector<string> sentences;
                normalize(getStrListIdx(0, d.sh->patt), sentences);
                string single_response;
                for (vector<string>::const_iterator it = sentences.begin(); it != sentences.end(); ++it) {
                    if (getAnswer(*it, single_response, d.log)) output += single_response + " ";
                    else break;
                }
                
            }
                break;
            case TEMPL_PERSON_SHORT:
            case TEMPL_PERSON2_SHORT:
            case TEMPL_GENDER_SHORT:
            {
                if (d.sh->patt.empty()) break;
                string star(getStrListIdx(0, d.sh->patt));
                switch(t.int_type) {
                    case TEMPL_PERSON_SHORT:  toPerson(star);   break;
                    case TEMPL_PERSON2_SHORT: toPerson2(star);  break;
                    case TEMPL_GENDER_SHORT:  toGender(star);   break;
                }
                //d.templ_str += star;
                output+=star;
            }
                break;
                
            case TEMPL_SET:
            {
                string name, value;
                if (!(t.chars_read = d.templ->readString(name))) return false;
                t.chars_read_tag += t.chars_read;
                
                size_t old_size = d.templ->limitSize(t.len - t.chars_read_tag);
                //mainStringSize=(uint)mainString.size();// save mainString
                if (!readTemplate(d,value)) return false;//rec+1
                //mainString.resize(mainStringSize); //restore mainString
                d.templ->restoreSize(old_size);
                //d.templ_str += value;
                output+=value;
                string str=str_replace("@SELF@","", value);
                if(str!=value){
                    avatar->setVar(name,str);
                }else{
                    person->setVar(name, value);
                }
                DI("SET VAR name="<<name<<" value="<<value<<" templ_str="<<d.templ_str<<" avatar var="<<person->dumpVar()<<endl;)
            }
                break;
            case TEMPL_GET:
            case TEMPL_BOT:
            {
                string name,tmp;
                if (!(t.chars_read = d.templ->readString(name))) return false;
                t.chars_read_tag += t.chars_read;
                
                tmp= (t.int_type == TEMPL_GET ? person->getVar(name) : avatar->getBotVar(name));
                if(tmp=="")tmp="Oh I don't know yet...Let me think about that:)";
                mainString+=tmp;
                //d.templ_str +=tmp;
                output+=tmp;
            }
                break;
            case TEMPL_UNKNOWN:
                break;
            default:
                set_error(AIMLERR_TEMLP_UNKNOWN_TAG);
                return false;
                break;
        }
    }
    
    return true;
}




// Reduced version of the matching algorithm found on graphmaster.cpp, look there for a thorough explanation of the code
// This version doesn't have to handle with different node types. The pattern is just a list, not a tree, so it is much easier
bool cGraphMaster::isMatch(InputIterator input, InputIterator pattern, unsigned long rec) {
  string input_head(*input);
  string patt_head(*pattern);
  input++; pattern++;

  if (patt_head == "_" && isMatchWildcard(input, pattern, rec)) return true;
  
  if (patt_head == input_head) {
    if (input.isDone()) { if (pattern.isDone()) return true; else return false; }
    else {
      if (pattern.isDone()) return false;
      else {
        if (isMatch(input, pattern, rec+1)) return true;
        else return false;
      }
    }
  }

  if (patt_head == "*" && isMatchWildcard(input, pattern, rec)) return true;
  
  return false;
}

bool cGraphMaster::isMatchWildcard(const InputIterator& input, const InputIterator& pattern, const unsigned long& rec) {
  if (input.isDone()) { if (pattern.isDone()) return true; else return false; }
  else {
    if (pattern.isDone()) return true;
    else {
      InputIterator input_copy(input);
      do { if (isMatch(input_copy, pattern, rec+1)) return true; input_copy++; } while(!input_copy.isDone());
      return false;
    }
  }
  return false;
}

void cGraphMaster::printBinTempl(size_t ID){
    int print=1;
    switch(ID){
        case TEMPL_CONDITION:
            DI("TEMPL_CONDITION"<<endl;) break;
        case TEMPL_CHARACTERS:
            DI("TEMPL_CHARACTERS"<<endl;) break;
        case TEMPL_STAR:
            DI("TEMPL_STAR"<<endl;) break;
        case TEMPL_TOPICSTAR:
            DI("TEMPL_TOPICSTAR"<<endl;) break;
        case TEMPL_THATSTAR:
            DI("TEMPL_THATSTAR"<<endl;) break;
        case TEMPL_THAT:
            DI("TEMPL_THAT"<<endl;) break;
        case TEMPL_INPUT:
            DI("TEMPL_INPUT"<<endl;) break;
        case TEMPL_GET:
            DI("TEMPL_GET"<<endl;) break;
        case TEMPL_BOT:
            DI("TEMPL_BOT"<<endl;) break;
        case TEMPL_SET:
            DI("TEMPL_SET"<<endl;) break;
        case TEMPL_LI:
            DI("TEMPL_LI"<<endl;) break;
        case TEMPL_LOWERCASE:
            DI("TEMPL_LOWERCASE"<<endl;) break;
        case TEMPL_UPPERCASE:
            DI("TEMPL_UPPERCASE"<<endl;) break;
        case TEMPL_FORMAL:
            DI("TEMPL_FORMAL"<<endl;) break;
        case TEMPL_SENTENCE:
            DI("TEMPL_SENTENCE"<<endl;) break;
        case TEMPL_RANDOM:
            DI("TEMPL_RANDOM"<<endl;) break;
        case TEMPL_GOSSIP:
            DI("TEMPL_GOSSIP"<<endl;) break;
        case TEMPL_SRAI:
            DI("TEMPL_SRAI"<<endl;) break;
        case TEMPL_THINK:
            DI("TEMPL_THINK"<<endl;) break;
        case TEMPL_LEARN:
            DI("TEMPL_LEARN"<<endl;) break;
        case TEMPL_SYSTEM:
            DI("TEMPL_SYSTEM"<<endl;) break;
        case TEMPL_JAVASCRIPT:
            DI("TEMPL_JAVASCRIPT"<<endl;) break;
        case TEMPL_SR:
            DI("TEMPL_SR"<<endl;) break;
        case TEMPL_DATE:
            DI("TEMPL_DATE"<<endl;) break;
        case TEMPL_SIZE:
            DI("TEMPL_SIZE"<<endl;) break;
        case TEMPL_VERSION:
            DI("TEMPL_VERSION"<<endl;) break;
        case TEMPL_ID:
            DI("TEMPL_ID"<<endl;) break;
        case TEMPL_PERSON:
            DI("TEMPL_PERSON"<<endl;) break;
        case TEMPL_PERSON2:
            DI("TEMPL_PERSON2"<<endl;) break;
        case TEMPL_GENDER:
            DI("TEMPL_GENDER"<<endl;) break;
        case TEMPL_PERSON_SHORT:
            DI("TEMPL_PERSON_SHORT"<<endl;) break;
        case TEMPL_PERSON2_SHORT:
            DI("TEMPL_PERSON2_SHORT"<<endl;) break;
        case TEMPL_GENDER_SHORT:
            DI("TEMPL_GENDER_SHORT"<<endl;) break;
        case TEMPL_UNKNOWN:
            DI("TEMPL_UNKNOWN"<<endl;) break;
        break;
    }
               
            
}
void cGraphMaster::printCond(size_t ID){
    int print;
    switch(ID){
        case TEMPL_CONDITION_SINGLE:
            DI("TEMPL_CONDITION_SINGLE"<<endl;) break;
        case TEMPL_CONDITION_MULTI:
            DI("TEMPL_CONDITION_MULTI"<<endl;) break;
        case TEMPL_CONDITION_BLOCK:
            DI("TEMPL_CONDITION_BLOCK"<<endl;) break;
    }
    
}


/**
 N O T E: the to_lowercase() call is necessary because I do to_uppercase() with avatar input. I'll consider doing case-insensitive
 pattern matching
 Pc: i must be in bounds!!! [0,size())
 **/
string cGraphMaster::getStrListIdx(unsigned int idx, const list<string>& str_list) {
    list<string>::const_iterator it = str_list.begin();
    for (unsigned int i = 0; i < idx; ++it) ++i;
    string lowercased(*it);
    to_lowercase(lowercased);
    return lowercased;
}


