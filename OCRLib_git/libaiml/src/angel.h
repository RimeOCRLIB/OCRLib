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

#ifndef __LIBAIML_ANGEL_H__
#define __LIBAIML_ANGEL_H__

#include <string>
#include <list>
#include "errors.h"
#include "core.h"
using namespace std;

/**
 * Namespace that holds the libaiml public interface.
 */
namespace aiml {
  class cTestAppCallbacks : public cInterpreterCallbacks {
    public:
        void onAimlLoad(const std::string& filename) {
            //cout << "LoadedAIML " << filename << endl;
        }
    };
    
  class cAngel {
    private:
      cTestAppCallbacks myCallbacks;
      cCore *Core;
      cAngel(void);
      cAngel(string &avatar,string &person,int mode);
    public:  
      static cAngel* create(void){return new cAngel;}
      static cAngel* create(string &avatar,string &person,int mode){
          return new cAngel(avatar,person,mode);}
      string angel(string &line,string &avatar,string &person,int mode);
      string angel(const char* line,string &avatar,string &person);
      void angelReadFile (string &line,string &avatar);
  };    
    
}

#endif
