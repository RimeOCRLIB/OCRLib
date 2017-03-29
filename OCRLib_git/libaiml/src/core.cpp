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
#include <fstream>
#include <glob.h>
#include "core.h"
#include "config.h"
#include "php2stl.h"




using namespace std;
using namespace aiml;
using std_util::gettok;
using std_util::operator<<;

cCore::cCore(void) :
          graphmaster(file_gossip_stream, last_error, *this),
          aiml_parser(graphmaster, last_error), caiml_parser(graphmaster, last_error),
          cfg_parser(*this),
          cfg(std_util::cConfig::ERRLEV_QUIET), user_manager(*this), initialized(false)
{ }

          
cCore::~cCore(void) { deinitialize(); }

bool cCore::initialize(const string& pathCfg) {
  if (initialized) { set_error(AIMLERR_ALREADY_INIT); return false; }
  if (!cfg_parser.load(pathCfg)) { set_error(AIMLERR_NO_CFGFILE); return false; }
  return applyConfigOptions(0);
}
bool cCore::initializeCAIML(const string& pathCfg){
    if (initialized) { set_error(AIMLERR_ALREADY_INIT); return false; }
    if (!cfg_parser.load(pathCfg)) { set_error(AIMLERR_NO_CFGFILE); return false; }
    return applyConfigOptions(1);
}
bool cCore::initializeGMap(){
    if (initialized) { set_error(AIMLERR_ALREADY_INIT); return false; }
    if (!cfg_parser.load(inputData.data["pathAIMLXML"])) { set_error(AIMLERR_NO_CFGFILE); return false; }

    ai.mainMemoryData=GVector::create(inputData.data["mainMemoryData"]);
    ai.mainMemory=GMap::create(inputData.data["mainMemoryMap"],ai.mainMemoryData);
    //cout<<"flagPSymbol="<<ai.mainMemory->flagPSymbols<<endl;
    
    ai.memoryData=GVector::create(inputData.data["memoryData"]);
    ai.memory=GMap::create(inputData.data["memoryMap"],ai.memoryData);
    
    ai.longMemoryData=GVector::create(inputData.data["longMemoryData"]);
    ai.longMemory=GMap::create(inputData.data["longMemoryMap"],ai.longMemoryData);
    
    graphmaster.initGMap();
    return applyConfigOptions(MMAP_LOCATION);
}


/*********************************** PRIVATE ******************************************/
bool cCore::applyConfigOptions(int mode) {
    //srand((uint)time(NULL));
    
    file_gossip_stream.open(cfg_options.file_gossip.c_str());
    if (!file_gossip_stream) { set_error(AIMLERR_OPEN_GOSSIP); return false; }
    
    if (!user_manager.load(cfg_options.user_file)) { set_error(AIMLERR_NO_USERLIST); return false; }
    
    if (cfg_options.allow_javascript) {
        if (!javascript_interpreter.init()) {
            if (last_error == AIMLERR_NO_ERR) set_error(AIMLERR_JAVASCRIPT_PROBLEM);
            return false;
        }
    }
    if(mode<2){
        if(!mode&&!load_aiml_files(cfg_options.memory_core,mode)) { return false; }
        graphmaster.sort_all();
    }
    if(mode==MMAP_LOCATION){
        aiml_parser.dbOnDisk=1;
        if(!ai.mainMemoryData->size()){
            aiml_parser.memoryVector=ai.mainMemoryData;
            if(!load_aiml_files(cfg_options.memory_core,ADD_TO_GMAP)) { return false; }
            ai.mainMemory->addRecords(ai.mainMemoryData, 3,REMOVE_DELIMETERS);
        }
        if(!ai.longMemoryData->size()){
            aiml_parser.memoryVector=ai.longMemoryData;
            if(!load_aiml_files(cfg_options.long_memory_core,ADD_TO_GMAP)) { return false; }
            ai.longMemory->addRecords(ai.longMemoryData, 3,REMOVE_DELIMETERS);
        }
    }
    initialized = true;
    
    return true;
}


bool cCore::initialize(const string& pathCfg, const cCoreOptions& opts) {
  if (initialized) { set_error(AIMLERR_ALREADY_INIT); return false; }
  if (!cfg_parser.load(pathCfg, true)) { set_error(AIMLERR_NO_CFGFILE); return false; }
  cfg_options = opts;
  return applyConfigOptions(0);
}

void cCore::deinitialize(void) {
  if (!initialized) return;
  
  if (file_gossip_stream.is_open()) file_gossip_stream.close();
  user_manager.save(cfg_options.user_file);
  initialized = false;
}

void cCore::registerCallbacks(cInterpreterCallbacks* _callbacks) {
  callbacks = _callbacks;
}

void cCore::unregisterUser(string& user_id) {
  UserMap::iterator it = user_map.find(user_id);
  if (it != user_map.end()) user_map.erase(it);
}

bool cCore::learnFile(string& filename) {
  if (!initialized) { set_error(AIMLERR_NOT_INIT); return false; }
  return learn_file(filename, true);
}

bool cCore::saveGraphmaster(string& file) {
#ifdef ENABLE_CAIML
  if (!initialized) { set_error(AIMLERR_NOT_INIT); return false; }
  return caiml_parser.save(file);
#else
  return false;
#endif
}

bool cCore::dumpMemory(string& path, GVector *memoryVector){
    if (!initialized) { set_error(AIMLERR_NOT_INIT); return false; }
    return aiml_parser.dumpMemory(path,memoryVector);

}


bool cCore::loadGraphmaster(string& file) {
#ifdef ENABLE_CAIML
  if (!initialized) { set_error(AIMLERR_NOT_INIT); return false; }
  return caiml_parser.load(file);
#else
  return false;
#endif
}

bool cCore::respond(string& input, string& avatar, string& person, string& output, list<cMatchLog>* log) {
  if (!initialized) { set_error(AIMLERR_NOT_INIT); return false; }
    
  vector<string> sentences;
  graphmaster.normalize(input, sentences);
  //cout<<"input="<<input<<" sentences="<<sentences[0]<<endl;
    
  if (sentences.empty()) { set_error(AIMLERR_EMPTY_INPUT); return false; }
 
  UserMap::const_iterator user_it;
  // user_it = user_map.find(avatar);
 // if (user_it == user_map.end()) {
 //   pair<string, cUser> user_entry(avatar, cUser(avatar, &last_error, &botvars_map, &graphmaster));
 //   user_map.insert(user_entry);
 // }
  user_it = user_map.find(person);
  if (user_it == user_map.end()) {
    pair<string, cUser> user_entry(person, cUser(person, &last_error, &botvars_map, &graphmaster));
    user_map.insert(user_entry);
  }
  user_it = user_map.find(inputData.data["avatar_uuid"]);
    if (user_it == user_map.end()) {
        pair<string, cUser> user_entry(inputData.data["avatar_uuid"], cUser(inputData.data["avatar_uuid"], &last_error, &botvars_map, &graphmaster));
        user_map.insert(user_entry);
  }
    

  /* CHANGE: if (getAnswer() == FALSE) ==> the user's input vector will be filled with unanswered inputs */
  //cUser& uuidAvatar=user_map[inputData.data["avatar_uuid"]];
  cUser& avatar_ = user_map[inputData.data["avatar_uuid"]];
  avatar_.addUserInput(sentences);
  cUser& person_ = user_map[person];
  person_.addUserInput(sentences);
    
    string timeStr=avatar_.getVar("timestamp");
    size_t oldTime=atoi(timeStr.c_str());
    time_t timer; time(&timer);
    ostringstream out;  out<<timer;
    string str=out.str();
    avatar_.setVar("timestamp", str);
    if(timer-oldTime<3){output="oh it was too fast for me:)";return 1;}
    //cout<<str<<" newTime="<<str<<" oldTime="<<oldTime<<" d="<<timer-oldTime;
    
  for(auto iter : avatar_.vars_map){   //update default settings of avatar with personal data
      //cout<<iter.first<<" -> "<<iter.second<<endl;
      //avatar_.setVar(iter.first, iter.second);
      botvars_map[iter.first]=iter.second;
  }

    
  
    string name=avatar_.getVar("name");
    if(name=="")avatar_.setVar("name", avatar);
    name=person_.getVar("name");
    if(name=="")person_.setVar("name", person);
    
  //    string topic="1", name="1";
  //    user.setVar(name, topic);

  string single_response;
  output.clear();
  if (log) log->clear();
  int flagAnswer=0;
  for (vector<string>::iterator it = sentences.begin(); it != sentences.end(); ++it) {
      if(graphmaster.dbOnDisk){
          graphmaster.mainStringSize=0;
          graphmaster.mainString="";
          graphmaster.avatar=&avatar_;
          graphmaster.person=&person_;

          if (!graphmaster.getAnswerDB(*it, single_response)) return false;
          //string str= user.dumpVar();
          //cout<<"str="<<str<<endl;
      }else{
          if (!graphmaster.getAnswer(*it, single_response, log)) return false;
      }
    output += graphmaster.mainString + " ";
    if(graphmaster.mainString!="")flagAnswer++;
  }
  if(!flagAnswer){output=" no idea about that";}
    
    output=str_replace("@href", "<a href", output);
    output=str_replace("href@", "</a>", output);
    output=str_replace("@SRC@", "", output);
  
  vector<string> split_response;
  do_split(output, split_response, cfg_options.sentence_limit, false);
  avatar_.addBotThat(split_response);
  person_.addBotThat(split_response);
    
  return true;
}

/**
 * Tell the aiml parser to feed the graphmaster
 */
bool cCore::load_aiml_files(string &path,int mode) {
    cout<<"mode="<<mode<<endl;
    vector<string>fileList;
    string ext="aiml";

    bool at_runtime=0;
    if(mode==REPLACE_IN_GMAP)at_runtime=1;
    
    readDirectoryToArray(fileList,path,ext);
    
    for (vector<string>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
        ai.path=fileName(*it); //save in global so we can use in parsing
        if (!learn_file(*it,mode,at_runtime)) return false;
        else { if (callbacks) callbacks->onAimlLoad(*it); }
    }
    return true;
}


/**
 * Loads AIML code from a file and feed it to the graphmaster
 */
bool cCore::learn_file(string& filename,int mode, bool at_runtime) {
  return aiml_parser.parse(filename, cfg_options.should_trim_blanks,mode, at_runtime);
}

bool cCore::doSystemCall(string& input, string& out) {
  if (!cfg_options.allow_system) {
    set_error(AIMLERR_SYSTEM_NOT_ALLOWED);
    return false;
  }
    string cmd=input;
    //cout<<"system call "<<cmd<<endl;
    if(cmd.find("@CALCULATE@")!=-1){
        cmd=str_replace("@CALCULATE@", "", cmd);
        cmd=str_replace("x", "*", cmd);
        cmd=str_replace("multiply", "*", cmd);
        cmd=str_replace("divide", "/", cmd);
        cmd=str_replace("X", "*", cmd);
        //cmd=str_replace(".", ",", cmd);
        stripLetters(cmd);
        
        cmd="php -r \"\\$x="+cmd+"; print \\$x;\"";
        //cout<<"cmd="<<cmd<<endl;
        out=run(cmd);
        if(out.find("error")!=-1){return 0;}
    }
    if(cmd.find("@DUMPMEMORY@")!=-1){
        string path=inputData.data["dumpPath"];
        if(!is_dir(path))mkdir(path.c_str(), S_IRWXU|S_IRWXO|S_IRWXG);
        path+="/memory.aiml";
        dumpMemory(path, ai.memoryData);
    }
  /* out.clear();
  FILE* file = popen(cmd.c_str(), "r");
  if (file) {
    char input[LIBAIML_POPEN_BUFFER_SIZE];
    while (!ferror(file) && !feof(file)) {
      size_t bytes_read = fread(input, 1, LIBAIML_POPEN_BUFFER_SIZE-1, file);
      if (bytes_read > 0) { input[bytes_read] = '\0'; out += input; }
    }
    pclose(file);
  }
  */
  return true;
}

bool cCore::doJavaScriptCall(string& cmd, string& ret) {
  if (!cfg_options.allow_javascript) {
    set_error(AIMLERR_JAVASCRIPT_NOT_ALLOWED);
    return false;
  }
  else {
    bool success = javascript_interpreter.eval(cmd, ret);
    if (!success) last_error = AIMLERR_JAVASCRIPT_PROBLEM;
    return success;
  }
}

/**
 * Debugging stuff
 */
#ifdef _DEBUG
#include <iostream>

ostream& aiml::msg_dbg(bool add_prefix) {
  if (add_prefix) return cout << "libaiml: [DEBUG] ";
  else return cout;
}

#endif
