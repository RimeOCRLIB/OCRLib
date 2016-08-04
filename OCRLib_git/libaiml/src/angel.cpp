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
#include <iostream>
#include "aiml.h"
#include "angel.h"
#include "php2stl.h"
#include "config.h"

using namespace std;
using std_util::strip;
using namespace aiml;
using namespace ocr;

#define GMap


cAngel::cAngel(){
    Core=new cCore;
    Core->registerCallbacks(&myCallbacks);
    int ret = 0;
    string path=inputData.data["rootAIML"]+"1.graph";
    
    try {
                
        //cout << "Initializing Core..." << endl;
        if(is_file(path)){
        
           if (!Core->initializeCAIML(inputData.data["rootAIML"]+"libaiml.xml")) throw 1;
           if (!Core->loadGraphmaster(path)) throw 1;
            
            /** Uncomment this line out and you'll see that the bot will no longer remember user vars **/
            //Core->unregisterUser("localhost");
    
        }else{

            if (!Core->initialize(inputData.data["rootAIML"]+"libaiml.xml")) throw 1;
            Core->saveGraphmaster(path);
            
            //cout<<"done load";

        }    
        //
    
    }catch(int _ret) {
    cout << "ERROR: " << Core->getErrorStr(Core->getError()) << " (" << Core->getError() << ")" << endl;
    if (!Core->getRuntimeErrorStr().empty()) cout << "Runtime Error: " << Core->getRuntimeErrorStr() << endl;
    ret = _ret;
    }

}

cAngel::cAngel(string &avatar,string &person, int mode){
    Core=new cCore;
    Core->registerCallbacks(&myCallbacks);
    int ret = 0;
    inputData.data["mainMemoryData"]=inputData.data["rootAIML"]+"_mainMemData.bin";
    inputData.data["mainMemoryMap"]=inputData.data["rootAIML"]+"_mainMemMap.bin";
    inputData.data["memoryMap"]=inputData.data["rootAIML"]+"_memMap.bin";
    inputData.data["memoryData"]=inputData.data["rootAIML"]+"_memData.bin";
    inputData.data["longMemoryMap"]=inputData.data["rootAIML"]+"_longMemMap.bin";
    inputData.data["longMemoryData"]=inputData.data["rootAIML"]+"_lonMemData.bin";
    inputData.data["pathAIMLXML"]=inputData.data["rootAIML"]+"libaiml_"+avatar+".xml";
    inputData.data["dumpPath"]=inputData.data["rootAIML"]+"DUMP_MEMORY";

    string pathAiml=inputData.data["pathAIMLXML"];
    string path=inputData.data["rootAIML"]+"_library"+avatar+".bin";
    
    try {
        
        //cout << "Initializing Core..." << endl;
        if(is_file(pathAiml)){
            if(mode>0){
                if (!Core->initializeGMap()) throw 1;
                //Core->graphmaster.testData(); exit(0);
            }else{
               if (!Core->initializeCAIML(pathAiml.c_str())) throw 1;
               if (!Core->loadGraphmaster(path)) throw 1;
            }
            /** Uncomment this line out and you'll see that the bot will no longer remember user vars **/
            //interpreter->unregisterUser("localhost");
            
        }else{
            if(mode>0){
                if (!Core->initializeGMap()) throw 1;
                //Core->graphmaster.testData();  exit(0);
            }else{
                if (!Core->initialize(pathAiml.c_str())) throw 1;
                Core->saveGraphmaster(path);
            }
            //cout<<"done load";
            
        }
        //
        
    }catch(int _ret) {
        cout << "ERROR: " << Core->getErrorStr(Core->getError()) << " (" << Core->getError() << ")" << endl;
        if (!Core->getRuntimeErrorStr().empty()) cout << "Runtime Error: " << Core->getRuntimeErrorStr() << endl;
        ret = _ret;
        
    }
   
}
string cAngel::angel(const char*line,string &avatar,string &person){
    string str=line;
    return angel(str,avatar,person,SINGLE_LINE);
}

string cAngel::angel(string &line,string &avatar,string &person,int mode) {
  //static cInterpreter* interpreter = cInterpreter::newInterpreter();
   
    
#ifdef OCRLibAI_DEBUG
    //line="remember Bodhnath it is Great Buddhist Stupa in Kathmandu, Nepal";//"";//"@TOPICC2C2@";
    //line="do you KNOW ABOUT Bodhnath";//"";//"@TOPICC2C2@";
    //line="stonehenge";//"";//"@TOPICC2C2@";
    //line="remember Sunny is Sunny Topaz name";
    line="what is your name";
    
    //line="#RELOAD MEMORY";
    //line="#DUMP MEMORY";
    //line="remember mechanic it is a person who repairs and maintains machinery.";
    //line="remember that Stonehenge it is prehistoric monument located in Wiltshire, England, about 2 miles (3 km) west of Amesbury and 8 miles (13 km) north of Salisbury";
    //line="remember Indonesia it is officially the Republic of Indonesia, sovereign state in Southeast Asia.";//"";//"@TOPICC2C2@";
   //line="DO YOU KNOW ABOUT Krakatau";//"";//"@TOPICC2C2@";
   //line="remember mushroom are (or toadstool) is the fleshy, spore-bearing fruiting body of a fungus, typically produced above ground on soil or on its food source.";
    //line="remember thesaurus are book that lists words in groups of synonyms and related concepts.";
#endif
    
  int ret = 0;
  
  // exceptions are used because returning in the middle of the program wouldn't let 'interpreter' be freed
  try {
    string result;
    static std::list<cMatchLog> log;
      
      /** remove the last parameter to avoid logging the match **/
      string str;//="localhost";
      if (!Core->respond(line,avatar, person, result, &log)) throw 3;
      str=strip(result);
      if(str=="")str="no idea about that";
      if(mode==SINGLE_LINE)Core->deinitialize();
      return(str);
      
    /** Uncomment this line out and you'll see that the bot will no longer remember user vars **/
    //interpreter->unregisterUser("localhost");
  }
  catch(int _ret) {
    //cout << "ERROR: " << Core->getErrorStr(Core->getError()) << " (" << Core->getError() << ")" << endl;
    if (!Core->getRuntimeErrorStr().empty()) cout << "Runtime Error: " << Core->getRuntimeErrorStr() << endl;
    ret = _ret;
  }
  // the above is equivalent to cInterpreter::freeInterpreter(interpreter);
  return "no answer";
}

void cAngel::angelReadFile(string &path,string &avatar) {
    
    vector<string>fileList;
    vector<string>fileData;
    string ext="txt";
    string str;
    readDirectoryToArray(fileList,path, ext);
            cout<<"fileList.size()="<<fileList.size()<<endl;
            for(int index=0;index<fileList.size();index++){
                readText(fileData, fileList[index]);
                string output;
                for(int i=0;i<fileData.size();i++){
                    if(!fileData[i].size())continue;
                    if(fileData[i].size()>512)continue;
                    str=angel(fileData[i],avatar,avatar,MULTY_LINE);
                    output+=fileData[i]+"\n   --> "+str;
                }
                str=fileList[index]+"_out.txt";
                writeText(output, str);
            }
    // the above is equivalent to cInterpreter::freeInterpreter(interpreter);
}
