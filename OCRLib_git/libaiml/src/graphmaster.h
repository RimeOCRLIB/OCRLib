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

#ifndef __LIBAIML_GRAPHMASTER_H__
#define __LIBAIML_GRAPHMASTER_H__

#include <string>
#include <list>
#include <set>
#include <fstream>
#include "AI.h"

using namespace std;

namespace aiml {
  
  class templData;
    
  class cCore;
  
  class AIMLentry {
    public:
      list<string>& getList(NodeType type);
    
      list<string> topic, that, patt;
      cWriteBuffer templ;
  };
    
  class Node;
  class gNode;
  typedef vector<Node> NodeVec;
  typedef list<AIMLentry> EntryList;
  
  class gNode {  //serialised Node in GVector
      protected:
      gNode();
      gNode(string &str);
      gNode(GVector *root_);
      gNode(GVector *root_,int ID_);
      gNode(GVector *root,gNode* parent_);
      void init();
      void init(string &str);
      void init(GVector *root_);
      void init(GVector *root_,int ID_);
      void init(GVector *root_,gNode* parent_);
      GVector *root;

      public:
      void destroy(void);
      void remove(void);
      static gNode* create() {return new gNode();}
      static gNode* create(string &str) {return new gNode(str);}
      static gNode* create(GVector *root) {return new gNode(root);}
      static gNode* create(GVector *root_,int ID_) {return new gNode(root_,ID_);}
      static gNode* create(GVector *root,gNode* parent) {return new gNode(root,parent);}
      
      int ID;
      int parentID;

      string key;
      NodeType type;
      vector<int> same_childs, diff_childs;
      cWriteBuffer templ;
      string topic;
      string that;
      string path;
      
      
      void save();
      void saveToStr(string &str);
      void removeChild(int ID);
      
  };
    
  class Node {
    public:
        string key;
        cWriteBuffer templ;
        NodeVec same_childs, diff_childs;
        NodeType type;
        
        bool operator< (const Node& other) const;
  };
  
  typedef list<string>::iterator StarIt;
    
  class StarsHolder {
    public:
      StarIt AddStar(const string& star, NodeType word_type);
      void DelStar(StarIt it, NodeType word_type);
          
      list<string> patt, that, topic;
  };
  
  class InputIterator {
    public:
      InputIterator(const list<string>& input) : current(input.begin()), end(input.end()) { }
      bool isDone(void) const { return (current == end); }
      const string& operator*(void) const { return *current; }
      void operator++(int) { ++current; }
  
    private:
      list<string>::const_iterator current;
      list<string>::const_iterator end;
  };
  
  class cUser;
    
  struct MatcherStruct {
        MatcherStruct(cUser& _user, list<aiml::cMatchLog>* _log) : user(_user), log(_log) { }
        MatcherStruct(cUser& _user) : user(_user) { }
      
        cWriteBuffer templ;
        cUser& user;
        StarsHolder sh;
        list<aiml::cMatchLog>* log;
        GVector *memVector;
        uint     memBase;
      
        list<string> pattern_log, that_log, topic_log;
        void logMatch(const string& match, NodeType type) {
            aiml::cMatchLog& match_log = log->back();
            _DBG_CODE(msg_dbg() << "LOG match: [" << match << "]" << endl);
            switch(type) {
                case NODE_PATT:   match_log.pattern.push_front(match);  break;
                case NODE_THAT:   match_log.that.push_front(match);     break;
                case NODE_TOPIC:  match_log.topic.push_front(match);    break;
            }
        }
  };
  
  class cGraphMaster {
    public:
      cGraphMaster(ofstream& gossip_stream, aiml::AIMLError& error_num, cCore& aiml_core);
      void initGMap(){dbOnDisk=1;}
      
      bool getAnswer(const string& input, string& output, list<aiml::cMatchLog>* log = NULL);
      bool getAnswerDB(string& input, string& output);
      bool parseSearchResult(string &input,
                             string& output,
                             vector<uint>&searchResult,
                             vector<sortData>&sort_,
                             MatcherStruct &ms);
      
      void addEntry(AIMLentry& entry, bool insert_ordered);
      void addEntryToGMap(GVector *memoryVector, AIMLentry& entry,bool insert_ordered);
      void remember(string &key, string &value);
      void reloadMemory(string &path);
      void sort_all(void);
      void testData();
  
      enum SubsType { SubsTypePerson, SubsTypePerson2, SubsTypeGender, SubsTypeGeneral };
      void addSubstitution(const string& from, const string& to, SubsType type, bool is_a_regex);
      void normalize(string input, vector<string>& out);
      void normalize_sentence(string& sentence) const;
  
      // to be used be CAIMLParser
      unsigned long& getSize(void);
      NodeVec& getRoot(void);
      int dbOnDisk;     
      cUser *avatar;
      cUser *person;
      
      string mainString; //main AIML report string
      uint mainStringSize; //size of main AIML report string
  
    private:
      cCore& aiml_core;
      ofstream& gossip_stream;
      unsigned long gm_size;
      NodeVec root;
      

  
      void toPerson(string& input);
      void toPerson2(string& input);
      void toGender(string& input);
      void toGossip(const string& input);
  
      void internal_sort(NodeVec& tree);
      void addNode(AIMLentry& entry, NodeType curr_type, NodeVec& tree, unsigned long rec, bool insert_ordered = false);
      void addGNode(GVector *memoryVector, AIMLentry& entry, NodeType curr_type,bool insert_ordered);
  
      void set_error(aiml::AIMLError num);
      aiml::AIMLError& last_error;
      void printBinTempl(size_t ID);  //print enum as text
      void printCond(size_t ID);  //print enum as text
      
      enum WILDCARD_TYPE { WILDCARD_AST, WILDCARD_LOWER };

    
      bool getMatch(InputIterator input, NodeType curr_type, const NodeVec& tree, MatcherStruct& ms, unsigned long rec);
      bool getMatchWildcard(const InputIterator& input, const NodeType& curr_type, const Node& tree_frontback, MatcherStruct& ms, const unsigned long& rec, const string& input_front);
      bool readTemplate(templData &d, string &output);
      bool templConditionBlock(templData &d,string &output,tPosition &t);
      bool templConditionMulty(templData &d,string &output,tPosition &t);
      bool templSrai(templData &d,string &output,tPosition &t);
      
      
      
      bool parseKeyAnswer(string &key,const string &input,MatcherStruct& ms);

      bool respordTempl(templData d,cUser &user);

      
      bool isMatch(InputIterator input, InputIterator pattern, unsigned long rec);
      bool isMatchWildcard(const InputIterator& input, const InputIterator& pattern, const unsigned long& rec);
  
      /** Substitution stuff **/
      struct SubstitutionPair {
        string from, to;
        bool regex;
      };
  
      typedef list<SubstitutionPair> SubstitutionList;
      SubstitutionList general_subs, person_subs, person2_subs, gender_subs;
  
      void do_substitutions(string& input, const SubstitutionList& subs_vec) const;
      string getStrListIdx(unsigned int idx, const list<string>& str_list);

  };
    
    class templData {
    public:
        templData();
        cReadBuffer *templ;
        const StarsHolder *sh;
        string templ_str;
        string input;
        string output;
        list<cMatchLog>* log;
        unsigned long rec;
    };
    
    
}



#endif
