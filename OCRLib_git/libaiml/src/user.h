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

#ifndef __LIBAIML_USER_H__
#define __LIBAIML_USER_H__

#define LIBAIML_MAX_THAT_SIZE       8      // number of bot responses to keep
#define LIBAIML_MAX_INPUTS_SAVED    16     // the amount of clients input to store

using namespace std;

namespace aiml {

  class cGraphMaster;
  
  class cUser {
    public:
      cUser(void);
      cUser(const string& _name, aiml::AIMLError* last_error, const StringMAP* botvars, const cGraphMaster* gm);
          
      vector<string> that_array[LIBAIML_MAX_THAT_SIZE];
      vector<string> input_array[LIBAIML_MAX_INPUTS_SAVED];
  
      StringMAP vars_map;
      string name;
  
      const string& setVar(const string& key, const string& value);
      
      const string& getVar(const string& key) const;      /** ADD: default values **/
      const StringMAP& getAllVars(void) const;
      const string& getBotVar(const string& key) const;
  
      const string& getInput(unsigned int idx1 = 1, unsigned int idx2 = 1) const;
      const string& getThat(bool for_matching = true, unsigned int which = 1, unsigned int sentence = 1) const;
      const string& getTopic(void) const;
      const string dumpVar(void);
      
      const string& setMood(const string &key, const string &value);
  
      void addUserInput(vector<string> input);
      void addBotThat(const vector<string>& that);
  
      void getMatchList(NodeType type, list<string>& out) const;
  
      void set_error(aiml::AIMLError errnum) const;
              
    private:
      const StringMAP* botvars_map;
      const cGraphMaster* graphmaster;
      aiml::AIMLError* const last_error;
  };

}

#endif
