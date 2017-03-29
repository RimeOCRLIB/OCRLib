//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
/*  Implemmentation  to GMemoryLib dynamic library  */

#include "GMemoryLib.h"
#include "config.h" 
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>

#include "php2stl.h"
#include "GMainEditor.h"
#include "GImageEditor.h"
#include "GFontEditor.h"
#include "GLogicProcessor.h"
#include "GMemory.h"
#include "GPDF.h"
#include <malloc/malloc.h>

#define DB_NAME "LIBRARY_CATALOG"
//#define DB_NAME "DHARMABOOK"

using namespace std;

#define EXPORT __attribute__((visibility("default")))
#define MAX_NUMBERS 99


#ifdef OCR_DYLIB
commandData inputData;
#endif
string stData="";

typedef struct catalogCategory{
    string name;
    string data;
}Category;


// Initializer.
__attribute__((constructor))
static void initializer(void) {                             // 2
    //printf("[%s] initializer()\n", __FILE__);
    inputData.init=0;
    
}

// Finalizer.
__attribute__((destructor))
static void finalizer(void) {                               // 3
    //printf("[%s] finalizer()\n", __FILE__);
}

#ifdef OCR_DYLIB
extern "C" {
#endif
    
    void initCommandData(){
        inputData.data["root"]="/_Image2OCR/";
        cout<<"init"<<inputData.init<<endl;
        readPreferences();
        GMemory *longMemory=GMemory::create();
        inputData.longMemory=longMemory;
        GLogicProcessor *logicProcessor=GLogicProcessor::create();
        //logicProcessor->readDictionary();
        inputData.logicProcessor=logicProcessor;
        inputData.init=1;
    }
    
    
    EXPORT
    const char* helloVarStr(char *str) {
        stData="www";
        return  stData.c_str();
    }


    EXPORT
    const char* www_helloVar(char *str) {
        ostringstream out_;
        out_<<"Hello_http:"<<1234567890;
        stData=out_.str();
        return  stData.c_str();
        
    }

    EXPORT
    const char* getFieldC(const char*db,int rec,int field){
        
        if(!inputData.init)initCommandData();
        string str=db;
        if(str=="")str="DHARMABOOK";
        GMemory *longMemory=(GMemory*)inputData.longMemory;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        cout<<v->size();
        if(rec>v->size())return "this record not exist";
        TString st;
        v->getTStr(rec,st);
        if(field>=st.size())return "this field not exist";
        stData=st[field];
        return stData.c_str();
    }
    
    EXPORT
    void getPageC(pageRec *page){
        
        if(!inputData.init)initCommandData();
        string str=page->db;
        if(str=="")str="DHARMABOOK";
        GMemory *longMemory=(GMemory*)inputData.longMemory;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        page->bookCount=(uint)v->size();
        if(page->index>v->size()){page->text="this record not exist"; return;}
        TString st;
        v->getTStr(page->index,st);
        if(page->field>=st.size()){page->text="this field not exist"; return;}
        str=st[page->field];

        page->pageCount=st.size();
        //if(str=="LIBRARY_CATALOG"){
        //    stData=renderCategory(stData.c_str(),"rus");
        //}
        str=str_replace("<br>","¶",str);
        str=str_replace("<br/>","¶",str);
        str=str_replace("¶¶¶¶","¶",str);
        str=str_replace("¶¶¶","¶",str);
        str=str_replace("¶¶","¶",str);
        str=str_replace("¶¶","¶",str);
        str=str_replace("¶","\n",str);
        
        stData=str;
        
        page->text=stData.c_str();

    }
    
    void getPageC_(pageRec *page){
        
        if(!inputData.init)initCommandData();
        
        string str=page->db;
        if(str=="")str="DHARMABOOK";
        GMemory *longMemory=(GMemory*)inputData.longMemory;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        page->bookCount=(uint)v->size();
        if(page->index>v->size()){page->text="this record not exist"; return;}
        
        TString st;
        v->getTStr(page->index,st);
        
        if(page->field>=st.size()){page->text="this field not exist"; return;}
        
        
        char *p=(char*)malloc(10240000);
        st.getStr(&p,page->field);
        stData=p;
        free(p);
        
        page->pageCount=st.size();

        stData=str_replace("<br>","¶",stData);
        stData=str_replace("<br/>","¶",stData);
        stData=str_replace("¶¶¶¶","¶",stData);
        stData=str_replace("¶¶¶","¶",stData);
        stData=str_replace("¶¶","¶",stData);
        stData=str_replace("¶¶","¶",stData);
        stData=str_replace("¶","\n",stData);

        page->text=stData.c_str();
        
    }
    
    
    EXPORT
    void getIntC(pageRec *page){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=page->db;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        page->bookCount=(uint)v->size();
        if(page->index>v->size()){cout<<"this record not exist"; return;}
        TString st;
        v->getTStr(page->index,st);
        if(page->index>=st.size()){cout<<"this field not exist"; return;}
        int c=st.getInt((uint)page->index);
        page->intData=c;
        page->pageCount=st.size();
        
    }
    
    EXPORT
    void splitOnPagesC(){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        
        string str=DB_NAME;
        //str="translationDictRus";
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        string ret="0"; ret[0]=11;
        cout<<v->size();
        for(int i=0;i<v->size();i++){
            //if(i%100==0)
            cout<<" "<<i;
            TString st;
            v->getTStr(i,st);
            if(st.size()>2)continue;
            TString st_;
            str=st[0];
            //re2::RE2::Replace(&str,".*\\[\\[ID:([\\d]+)\\]\\].*","\\1");
            //str="[[ID:"+str+"]]";
            st_.push_back(str);

            string key=st[1];
            int index=0;
            int pageSize=1024*9;
            
            while(index<key.size()){
                string page=substr(index,pageSize,key);
                //находим конец фразы
                if(index+pageSize<key.size()){
                    str=substr(index+pageSize,256, key);
                    if(str.find("\n")!=-1){
                        str=substr(0,(uint)str.find("\n")+1, str);
                    }else if(str.find(" ")!=-1){
                        str=substr(0,(uint)str.find(" ")+1, str);
                    }else if(str.find("།")!=-1){
                        str=substr(0,(uint)str.find("།")+3, str);
                    }else if(str.find("༔")!=-1){
                        str=substr(0,(uint)str.find("༔")+3, str);
                    }else if(str.find("༴")!=-1){
                        str=substr(0,(uint)str.find("༴")+3, str);
                    }else if(str.find("་")!=-1){
                        str=substr(0,(uint)str.find("་")+3, str);
                    }else if(str.find(".")!=-1){
                        str=substr(0,(uint)str.find(".")+1, str);
                    }
                    page+=str;
                }
                st_.push_back(page);
                index+=pageSize+str.size();
            }
            v->putTStr(i,st_);
        }
        v->close();
    }
    
    void dumpWiki(const char *path){
        string str=path;
        string text="";
        vector<string>v;
        readText(v,str);
        int start=0;
        string st;
        for(int i=0;i<v.size();i++){
            if(i%100000==0)cout<<i<<".";
            if(start){
                st+=v[i]+"\n";
                if(v[i].find("</page>")!=-1){
                    if(st.find("{{-pi-}}")!=-1){
                        text+=st;
                    }
                    st="";
                    start=0;
                }
            }
            if(v[i].find("<page>")!=-1){
                st+=v[i]+"\n";
                start=1;
            }
        }
        
        str+=".txt";
        writeText(text,str);
    
    }
    
    void exportDictFromTextDB(const char *db,const char *path){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        GVector *v;
        string db_=db;
        longMemory->loadTable(db_);
        v=longMemory->table[db].data;
        map<string,int> idMap;
        string str;
        map <string,string>rusMap;

        
        for(int i=0;i<v->size();i++){
            if(i%10000==0)cout<<" "<<i;
            TString st;
            v->getTStr(i,st);
            if(st.size()<10)continue;
            str=st[0];
            idMap[str]=i;
            str=st[3]; //ArchiveDate
            if(st[3]=="NULL"){
                //ищем русский перевод
                str=st[6];
                if(str.find("а")!=-1||
                   str.find("е")!=-1||
                   str.find("и")!=-1||
                   str.find("о")!=-1||
                   str.find("у")!=-1||
                   str.find("я")!=-1||
                   str.find("ю")!=-1
                   ){
                    rusMap[st[1]]=str;
                }
            }
        }
        cout<<endl<<"done build map"<<endl;
        string text="";
        
        for(int i=0;i<v->size();i++){
            
            if(i%10000==0)cout<<" "<<i;
            TString st;
            v->getTStr(i,st);
            if(st.size()<10)continue;
            
            if(st[1]=="NULL"){  //если у строки нет родительской строки
                //1/ проверяем является ли строка строкой Пали
                str=st[6];
                if(isPali(str)){
                    text+="[["+str+"\t@@@"+rusMap[st[0]]+"]]\n";
                }
            }
        }
        cout<<endl<<"done"<<endl;
        writeText(text,path);

    }
    
    
    //Dictionary functions
    
    EXPORT
    const char* translateLine(const char*line,const char*ln){
        //return "";
        //sleep(1);
        stData="";
        
        if(line==NULL||ln==NULL){
            cout<<"emptyLine"<<endl;
            return stData.c_str();
        }
        stData=line;
        if(stData=="")return stData.c_str();
        
        if(!inputData.init)initCommandData();
        GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
        inputData.data["ln"]=ln;
        stData=line;
        
        logicProcessor->lineTextTranslation(stData);
        //stData="";
        //cout<<"stData:"<<stData;
        return stData.c_str();
    
    }
    
    EXPORT
    const char* dictionaryReport(const char*line,const char*ln){
        //return "";
        if(line==NULL)return "";
        if(ln==NULL)ln="rus";
        if(!inputData.init)initCommandData();
        GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
        inputData.data["ln"]=ln;
        string text=line;
        re2::RE2::Replace(&text,"([\\p{Tibetan}]+)", "\\1་");
        text=str_replace("་་","་",text);
        logicProcessor->writeDictReportLocal(text);
        stData=text+"<br/>done";
        return stData.c_str();
    }
    
    const char* dictionaryReportTitle(const char*line,const char*ln){
        //return "";
        if(line==NULL)return "";
        if(ln==NULL)ln="rus";
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        inputData.data["ln"]=ln;
        string text=line;
        re2::RE2::Replace(&text,"([\\p{Tibetan}]+)", "\\1་");
        text=str_replace("་་","་",text);
        
        string dbTitle="DHARMABOOK_Title";
        string db="DHARMABOOK";
        longMemory->loadTable(db);
        GVector *v=longMemory->table[db].data;
        
        vector<TString>r;
        vector<int>f;
        f.push_back(0);
        longMemory->select(dbTitle, f, text, HASH_SEARCH,r,1);
        //cout<<r.size()<<endl;
        ostringstream out;
        vector<string>report;
        report.resize(20);
        
        if(r.size()){
            TString s=r[0];
            s.reloadPtr();
            vector<int>res;
            s.getIntVector(res, 1);
            //cout<<" ->> "<<s[0]<<" "<<str.size()<<" "<<res[0];
            
            for(int i=0;i<res.size();i++){
                TString st;
                v->getTStr(res[i],st);
                string t=st[1];
                t=str_replace("[[Category:Dictionary]]¶","",t);
                re2::RE2::Replace(&t,"\\[\\[Title:[^¶]*¶", "");
                re2::RE2::Replace(&t,"\\[\\[Category:Dictionary__\\[([^¶]*)\\]\\]¶", "[\\1\n");
                re2::RE2::Replace(&t,"\\[\\[Category:Dictionary__([^¶]*)\\]\\]¶", "[\\1]\n");
                t=str_replace("[[Category:Dictionary__GR]]","[GR]\n",t);
                t=str_replace("[[Category:Dictionary__GRM]]","[GRM]\n",t);
                t=str_replace("<br>","<br/>",t);
                string dict=substr(0, (uint)t.find("\n"), t);
                t=substr((uint)t.find("\n")+1,(uint)t.size(), t);
                if(dict.find("[TT]")!=-1||dict.find("[DK]")!=-1||dict.find("[DN]")!=-1){
                    t=translateLine(t.c_str(),ln);
                }
                
                
                //out<<renderPage(t.c_str());
                out<<t<<"<table width=\"100%\"><tr><td width=\"90%\"><hr/></td><td width=\"128px\"><div align=\"right\"><font color=\"#cccccc\">  "<<dict;
                out<<"</font><a href=\"/?field=1&index="<<res[i]<<"&ocrData=read\" target=\"_blank\"><font color=\"#cccccc\"> • edit</font></a></div></td></tr></table>\n";
                t=out.str();
                out.str("");
                t=str_replace("<br/><br/>","<br/>",t);
                
                if(dict.find("[MG]")!=-1){report[0]=t;continue;}
                if(dict.find("[HP]")!=-1){report[1]=t;continue;}
                if(dict.find("[TD]")!=-1){report[2]=t;continue;}
                if(dict.find("[MG]")!=-1){report[3]=t;continue;}
                if(dict.find("[MV]")!=-1){report[4]=t;continue;}
                if(dict.find("[VD]")!=-1){report[5]=t;continue;}
                if(dict.find("[ER]")!=-1){report[6]=t;continue;}
                if(dict.find("[RE]")!=-1){report[7]=t;continue;}
                if(dict.find("[SD]")!=-1){report[8]=t;continue;}
                if(dict.find("[IW]")!=-1){report[9]=t;continue;}
                if(dict.find("[JW]")!=-1){report[10]=t;continue;}
                if(dict.find("[TT]")!=-1){report[11]=t;continue;}
                if(dict.find("[DK]")!=-1){report[12]=t;continue;}
                if(dict.find("[BB]")!=-1){continue;}
                if(dict.find("[TS]")!=-1){continue;}
                if(dict.find("[TR]")!=-1){continue;}
                report.push_back(t);
                
            }
        }
        string delimenter="";
        text=implode(delimenter,report);

        
        
        stData=text;
        return stData.c_str();

    }
    
    
    EXPORT
    const char* reloadDictionary(const char*user_text, const char *ln){
        if(user_text==NULL||ln==NULL)return "";
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        inputData.data["ln"]=ln;
        string key=user_text;
        vector<string>newKey; newKey=explode("[",key);
        map<string,string>lineKey;
        //int flagLang=0;
        string db;
        string ln_=ln;
        string dictLn="tib";
        if(key.find("་")==-1)dictLn="skt";
        
        if(dictLn=="tib"){
            if(ln_=="rus")db="translationDictRus";
            if(ln_=="eng")db="translationDictEng";
            if(ln_=="")db="translationDictRus";
        }else{
            if(ln_=="rus")db="translationSktDictRus";
            if(ln_=="eng")db="translationSktDictEng";
            if(ln_=="")db="translationSktDictRus";

        }
        
        for(int i=0;i<newKey.size();i++){
            string line =str_replace("]", "", newKey[i]);
            line=str_replace("/","#",line);          //разделителем в текстовой словарной статье может быть знак # или /
            if(line=="*")line="";
            vector<string>keyLine=explode("#",line);
            if(keyLine.size()!=2)continue;
            string key=keyLine[0];
            key=str_replace(" ","",key);
            if(dictLn=="tib"&&key.find("་")!=-1){
                
                key+="་";
                key=str_replace("་་","་",key);
                key=str_replace("་་","་",key);
                key+="@"+key;
                key=str_replace("@་","",key);
                key=str_replace("@","",key);
                
            }
            lineKey[keyLine[0]]=keyLine[1]+"*";  //маркировка новой записи
        }
        
        int flagNewRecord=0;
        GVector *dk;
        GMap *d;
        
        longMemory->loadTable(db);
        dk=longMemory->table[db].data;
        indexRecord *indexRec=longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
        d=indexRec->mIndex;
        
        ostringstream out;
        map<string,string>::iterator it;
        for(it = lineKey.begin(); it!=lineKey.end(); it++) {
            string key=it->first;
            ulong index=d->getHKey(key,0);
            TString st;
            if(it->second=="*")key="";  //по запросу стираем запись
            st.push_back(key);
            out<<key<<"-->"<<it->second<<endl;
            st.push_back(it->second);
            if(index==0xffffffffffffffff){
                flagNewRecord=1;
                dk->push_back(st);
            }else{
                dk->putTStr(index, st);
            }
            
        }
        if(flagNewRecord)longMemory->reloadIndex(longMemory->table[db], 0, HASH_SEARCH);
        out<<"done save dictionary";
        stData=out.str();
        return stData.c_str();
    }
    
    void extractCategory(string &text,map<string,string>&pageMap){
        //cout<<"extract"<<endl;
        vector<string>page=explode("{{",text);
        string str;
        string report="";
        for(int i=0;i<page.size();i++){
            str=page[i];
            if(str.find("|")!=-1&&str.find("}}")!=-1){
                string line=substr(0, (uint)str.find("}}"), str);
                vector<string>v=explode("|",line);
                if(pageMap[v[0]]==""||pageMap[v[0]]==v[1]){
                    pageMap[v[0]]=v[1];
                }else{
                    //cout<<"not valid key "<<v[0]<<" in record "<<pageMap[v[0]]<<endl<<str<<endl;
                    continue;
                }
                report+=substr((uint)str.find("}}")+2, (uint)str.size(),str);
            }else{
                report+=str;
            }
        }
        for(map<string,string>::iterator it = pageMap.begin(); it != pageMap.end(); it++) {
            report+="{{"+it->first+"|"+it->second+"}}\n";
        }
        text=report;
    
    }
    
    void replaceInDB(const char* db){
        string str=db;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;

        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        string ret="0"; ret[0]=11;
        cout<<v->size();
        for(int i=0;i<v->size();i++){
            if(i%10000==0)
            cout<<" "<<i;
            TString st;
            v->getTStrData(i,st);
            string key=st[1];
            key=st[1];
            if(key.find("Category:Library catalog")==-1)continue;

            key=st[0];

            //key=str_replace("}","]",key);
            //key=str_replace("¶","\n",key);
            //re2::RE2::Replace(&key,"\\[\\[Path[^\\]].*\\]\\]","");
            //key=str_replace("\n","",key);
            
            TString st_;
            st_.push_back(key);
            for(int n=1;n<st.size();n++){
                key=st[n];
                //key=str_replace("Dharmadownload.net","Dharmadownload.org",key);
                //key=str_replace(" ¶།¶"," ¶།",key);
                if(n==1){
                    key=str_replace("\n","¶",key);
                    //re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\|([^\\]]*)\\]\\]", "[[Title:\\1|\\2]]¶[[Category:\\1]]¶[[Category:\\2]]");
                    re2::RE2::Replace(&key, "(.*)\\{\\{Path\\|([^\\}]+)\\}\\}(.*)", "{{Path|\\2}}\\1\\3");
                }

                /*
                if(n==1){
                    if(st.size()>3){
                        string s;
                        s=st[(uint)st.size()-2];
                        s=str_replace("¶","\n",s);
                        vector<string>lines=explode("\n",s);
                        for(int m=0;m<lines.size();m++){
                            if(lines[m].find("[[")==0){
                                key=lines[m]+"\n"+key;
                                lines[m]="";
                            }
                        }
                        s=implode("\n",lines);
                        st[(uint)st.size()-2]=s;
                    }
                }
               */
               // key=str_replace("\r","\n",key);
                //key=str_replace(" \n","\n",key);
                //key=str_replace("་ ","་",key);
                //key=str_replace("<","&lt;",key);
                //key=str_replace("<","&gt;",key);
                //key=str_replace("{{Title|","{{a_Title|",key);
                //key=str_replace("{{a_title","{{a_Title",key);
                //if(n==1)key=str_replace("¶","\n",key);

                //key=str_replace(ret.c_str(),"\n",key);
                //key=str_replace("}} & {{","}}\n{{",key);
                
                //key=str_replace("{{Q_COPY|{{copy","{{Q_COPY",key);
                //key=str_replace("{{P_TBRC_CODE|{{tcode","{{P_TBRC_CODE",key);
                //key=str_replace("{{ID:","{{ID|",key);
                //key=str_replace("{{M_VOLUME|{{volume","{{M_VOLUME",key);
                //key=str_replace("{{N_PAGES|{{pages","{{N_PAGES",key);
                //key=str_replace("{{a_title","{{a_Title",key);
                
                //key=str_replace("¶","\n",key);
                //key=str_replace("\n\n","\n",key);
                //key=str_replace("\n\n","\n",key);
                //re2::RE2::Replace(&key,"{{Tib-vol-id-end}","");
                //re2::RE2::Replace(&key,"\\{\\{Tib-vol-id-end(.*)","{{Tib-vol-id-end\\1}");
                //if(n==1)re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\]\\]", "{{a_Title|\\1}}");
                
                //if(key.find("{{A_CODE")!=-1){
                //    map<string,string>pageMap;
                    //extractCategory(key,pageMap);
                    //key=str_replace("\n\n","\n",key);
                    //key=str_replace("\n\n","\n",key);
                    //key=str_replace("\n\n","\n",key);
                    //key=str_replace("\n\n","\n",key);
               //     key=str_replace("\n\n","\n",key);
               //     key=str_replace("\n\n","\n",key);

               //     flag=1;
               // }else{
               //     if(n==1)break;
               // }

                
                //re2::RE2::Replace(&key,"་[ ]*\\n*[ །]*༄༅[ །]*","་");
                //key=str_replace("་\n","་",key);
                //re2::RE2::Replace(&key,"===[^=]+===\\n[ @]+\\n","");
                //key=str_replace("title=","",key);
                //key=str_replace("name=","",key);
                //key=str_replace("copy=","",key);

                ///int n=key.find("སམ་སིདྡྷི");
                //if(n!=-1){
                //    n-=100;
                //    if(n<0)n=0;
                //    string s=substr(n,256,key);
                //    cout<<" i:"<<i<<" key:"<<s<<endl;
                //}
                st_.push_back(key);
                
            }
            v->putTStr(i,st_);
            //break;
        }
        v->close();

    
    }
    
    void importOCRText(const char*path){
        string str=DB_NAME;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        v->resizeData(25000, 4100);
        vector<string>fileList;
        string fileData;
        string ext="txt";
        string path_=path;
        long time_=time(NULL);
        readDirectoryToArray(fileList,path_, ext);
        cout<<"fileList.size()="<<fileList.size()<<endl;
        for(int index=0;index<fileList.size();index++){
            cout<<index<<" ";
            readText(fileData, fileList[index]);
            fileData=str_replace("{page",":|:{page",fileData);
            vector<string>list=explode(":|:", fileData);
            TString st;
            ostringstream out;
            out<<"[[ID:"<<time_<<"]]";
            time_++;
            str=out.str();
            st.push_back(str);
            for(int i=0;i<list.size();i++){
                if(i==0){
                    list[i]="[[Title:"+fileName(fileList[index])+"]]\n"+list[i];
                }
                st.push_back(list[i]);
            }
            str="\n[[Category:Tibetan text]]\n[[Category:བཀའ་འགྱུར]]\n[[Path:"+fileName(fileList[index])+"]]";
            st.push_back(str);
            v->push_back(st);
                
        }
    }
    
    void getReportLine(string &line,int pos){
        int pos1;
        int pos2;
        int limit=512;
        string str=substr(pos,limit,line);
        string s;
        if(pos+limit<line.size()){
            s=substr(pos+limit,limit,line);
            pos1=(int)s.find(" ");
            pos2=(int)s.find("\n");
            if(pos2==-1)pos2=(int)s.find("་");
            if(pos1==-1)pos1=limit;
            
            if(pos1<pos2){
                str+=substr(0,pos1,s);
            }else{
                str+=substr(0,pos2,s);
            }
        }
        pos1=pos-limit;
        if(pos1<0){
            s=substr(0,pos,line);
        }else{
            s=substr(pos-limit,limit,line);
        }
        pos1=(int)s.find(" ");
        pos2=(int)s.find("\n");
        if(pos1==-1||pos2==-1){
            pos1=limit;
            pos2=(int)s.rfind("་");
        }
        if(pos2>0){
            if(pos1>pos2){
                str=substr(pos1,limit,s)+str;
            }else{
                str=substr(pos2,limit,s)+str;
            }
        }
        line=str;
    }
    
    EXPORT
    const char* searchDB(pageRec *page){
        //return "";
        //stData=searchDBTable(page,DB_NAME,FULL_TEXT_SEARCH);
        stData=searchDBTable(page,FULL_TEXT_SEARCH);
        //stData=searchDBTable(page,STRING_SEARCH);
        return stData.c_str();
    }
    
    const char* searchTextDB(pageRec *page){
        
        string db=page->db;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string key=page->text;
        page->bookCount=0;
        string text=key;
        string report;
        longMemory->loadTable(db);
        vector<TString>r;
        vector<int>f;
        f.push_back(page->field);
        longMemory->select(db, f, key, TEXT_SEARCH,r,0);
        for(int i=0;i<r.size();i++){
            TString s=r[i];
            s.reloadPtr();
            //cout<<i<<" ->> "<<s[0]<<" / "<<s.getInt(1)<<endl;
        }
        
        return stData.c_str();
    }
    
    EXPORT
    const char* searchDBTable(pageRec *page,int mode){
        string db=page->db;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string key=page->text;
        page->bookCount=0;
        string text=key;
        string report;
        longMemory->loadTable(db);
        TString st;
        string str;
        //v->getTStr(page->index,st);
        ostringstream out;
        if(mode==FULL_TEXT_SEARCH||mode==ID_SEARCH){
            setSearchText(key);
        }
        string destString;
        
        vector<TString>r;
        vector<int>f;
        f.push_back(page->field);
        longMemory->select(db, f, key, mode,r,0);
        //cout<<r.size()<<endl;
        for(int i=0;i<r.size();i++){
            TString s=r[i];
            s.reloadPtr();
            //cout<<i<<" ->> "<<s.getInt(0)<<" "<<s.getInt(1)<<" "<<s.getInt(2)<<endl;
            rLink *link;
            string str=s[0];
            link=(rLink*)&str[0];
            TString dataStr;
            longMemory->table[db].data->getTStr(link->index,dataStr);
            
            
            
            string st=dataStr[link->field];
            int offset=link->offset;
            map<string,string>pageMap;
            extractCategory(st, pageMap);
            getReportLine(st,offset);
            st+=renderCategorySearch(page,"rus");
            st=str_replace("\n<br/>\n", "", st);
            re2::RE2::Replace(&st,"Title:*","");
            st=str_replace("{","",st);
            st=str_replace("}","",st);

            if(pageMap.find("a_Title_bo")!=pageMap.end()){
                st="Title: "+pageMap["a_Title_bo"]+"<br/>"+st;
            }else if(pageMap.find("a_Title")!=pageMap.end()){
                st="Title: "+pageMap["a_Title"]+"<br/>"+st;
            }
            ostringstream out_;
            out_<<"<a href=\"http://www.buddism.ru:4000/?db="<<page->db<<"&index="<<link->index<<"&field="<<link->field<<"&ocrData=read\" target=\"_blank\">Book ID:"<<link->index<<" page:"<<link->field<<"<br/>..."<<st<<"....</a><hr/>\n";
            
            destString+=out_.str();
            page->bookCount++;
            if(i>100)break;
        }
        str="<font color=\"blue\">"+text+"</font>";
        destString=str_replace(text,str, destString);
        destString=str_replace("¶"," ", destString);
        
        out.str("");
        out<<"<h3>Search result:"<<page->bookCount<<"</h3><hr/>\n"<<endl;
        str=out.str();

        
        stData=str+destString;
        return stData.c_str();
    
    }
    
    EXPORT
    void joinFieldInTable(const char* dbName, int filedNameIndex){
        string db=dbName;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        longMemory->loadTable(db);
        GVector *v=longMemory->table[db].data;
        TString stFieldName;
        v->getTStr(filedNameIndex,stFieldName);
        
        
        for(int i=0;i<v->size();i++){
            cout<<i<<" ";
            TString st;
            v->getTStr(i,st);
            TString st_;
            string str;
            for(int n=0;n<st.size()&&n<stFieldName.size();n++){
                str+="==="+stFieldName[n]+"===\n"+st[n]+"\n";
            }
            st_.push_back("_");
            st_.push_back(str);
            v->putTStr(i,st_);
        }

    
    }
    
    void loadLibraryPathInCatalog(const char*db){
        string catalog=db;
        string path="LIBRARY_PATH";
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        longMemory->loadTable(catalog);
        longMemory->loadTable(path);
        GVector *vCatalog=longMemory->table[catalog].data;
        GVector *vPath=longMemory->table[path].data;
        string res;
        for(int i=0;i<vPath->size();i++){
            if(i%1000==0)cout<<" "<<i;
            string str;
            vPath->getStr(i,str);
            string name=fileName(str);
            string code;
            string file;
            if(name=="_mark"||name=="mark"||name=="_press"||name.find("_files")!=-1||name.find("_files")!=-1)continue;
            if(re2::RE2::PartialMatch(name,"R[\\d]{10}")){
                code=name;
                re2::RE2::Replace(&code,".*_R([\\d]{10})","\\1");
                re2::RE2::Replace(&name,"_R[\\d]{10}","");
                //cout<<name<<"->"<<code<<endl;
                //name="===FILE_NAME===\n"+name+"\n";
                //code="{{A_CODE|"+code+"}}";
                for(int n=0;n<vCatalog->size();n++){
                    TString st_;
                    TString st1;
                    vCatalog->getTStr(n,st_);
                    if(st_[1].find(code)!=-1){
                        st1.push_back(code);
                        name=st_[1]+"\n{{Path|"+str+"}}";
                        st1.push_back(name);
                        vCatalog->putTStr(n,st1);
                        break;
                    }
                
                }
                
                
                
            }
            //cout<<name<<endl;
            //res+=str+"\n";
        }
        //string s="/_Image2OCR/path.txt";
        //writeText(res, s);
    
    }
    
    void extractMap(string &text,map<string,string>pageMap){
        text=str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/","/___DHARMA___/___LIBRARY___/", text);
        
        string str;
        string report;
        string reportStr;
        //рендер шаблонов
        vector<string>page=explode("{{",text);
        for(int i=0;i<page.size();i++){
            str=page[i];
            if(str.find("|")!=-1&&str.find("}}")!=-1){
                string line=substr(0, (uint)str.find("}}"), str);
                vector<string>v=explode("|",line);
                if(pageMap.find(v[0])!=pageMap.end())continue;
                pageMap[v[0]]=v[1];
                str=substr((uint)str.find("}}")+2, (uint)str.size(),str);
                reportStr+=str_replace("\n","<br/>\n",str);
            }else{
                reportStr+=str_replace("\n","<br/>\n",str);
            }
        }
    
    }
    
    const char* renderCategoryText(const char *pageText, int index){
        
        string str=pageText;
        ostringstream out;
        ostringstream out_;
        string reportStr="";
        string report="";
        string render="";
        
        //рендер категорий
        out_<<"<hr/>";
        str=pageText;
        str=str_replace("[[","\n[[",str);
        str=str_replace("]]","]]\n",str);
        str=str_replace("¶","\n",str);
        vector <string>lineArray=explode("\n",str);
        for(int n=0;n<lineArray.size();n++){
            str=lineArray[n];
            if(lineArray[n].find("[[Category:")==0&&str.find("]]")!=-1){
                string line=substr(11, (uint)str.find("]]")-11, str);
                string link=line;
                encodeURL(link);
                out_<<"<a class=\"content\" href=\"/?category="<<link<<"&ocrData=readCategory\">"<<line<<"</a>|";
            }
        }
        out_<<"<hr/>";
        //рендер шаблонов
        string text=pageText;
        text=str_replace("[[","{{",text);
        text=str_replace("]]","}}",text);
        
        vector<string>page=explode("{{",text);
        map<string,string>pageMap;
        for(int i=0;i<page.size();i++){
            str=page[i];
            if(str.find("}}")!=-1&&(str.find("|")!=-1||str.find(":")!=-1)){
                string line=substr(0, (uint)str.find("}}"), str);
                if(str.find("|")!=-1){
                    vector<string>v=explode("|",line);
                    if(v.size()>1)pageMap[v[0]]+=v[1];
                }else{
                    vector<string>v=explode(":",line);
                    if(v.size()>1)pageMap[v[0]]+=v[1];
                }
                
                str=substr((uint)str.find("}}")+2, (uint)str.size(),str);
                reportStr+=str_replace("\n","<br/>",str);
            }else{
                reportStr+=str_replace("\n","<br/>",str);
            }
        }
        if(pageMap.find("Path")!=pageMap.end()){
            str=pageMap["Path"];
            if(str.find("/Volumes")!=-1){
                str=str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/","/___DHARMA___/___LIBRARY___/", str);
                str=str_replace("/Volumes/TERSAR_3TB/__LIBRARY_CANON_ALL/","/CANON/", str);
                
                report="<a href=\"http://www.buddism.ru/"+str+"\" target=\"_blank\" class=\"lib\">Open text in Library</a><br/>"+report;
            }
        }
        if(pageMap.find("Title")!=pageMap.end()){
            string s=pageMap["Title"];
            if(isPali(s)||s.find("་")!=-1){
                report+="<tb>Title: <tsh>"+pageMap["Title"]+"</tsh></tb><br/>";
            }else{
                report+="<tb>Title: "+pageMap["Title"]+"</tb><br/>";
            }
        }
        if(pageMap.find("a_Title_bo")!=pageMap.end()){
            report+="<tb>Title: "+pageMap["a_Title_bo"]+"</tb><br/>";
        }else if(pageMap.find("title_pali")!=pageMap.end()){
            report+="<tb>Title Pali: "+pageMap["title_pali"]+"</tb><br/>";
        }
        if(pageMap.find("title_eng")!=pageMap.end()){
            report+="<tb>Title: "+pageMap["title_eng"]+"</tb><br/>";
        }
        if(pageMap.find("title_rus")!=pageMap.end()){
            report+="<tb>Название: "+pageMap["title_rus"]+"</tb><br/>";
        }
        if(pageMap.find("info_eng")!=pageMap.end()){
            report+="<tb>"+pageMap["info_eng"]+"</tb><br/>";
        }
        if(pageMap.find("info_rus")!=pageMap.end()){
            report+="<tb>"+pageMap["info_rus"]+"</tb><br/>";
        }
        if(pageMap.find("chapter_code")!=pageMap.end()){
            report+="<code>Code: "+pageMap["chapter_code"]+"</code><br/>";
        }
        if(pageMap.find("info_rus")!=pageMap.end()){
            //report+="<td>"+pageMap["info_rus"]+"</td><br/>";
        }
        
        for(map<string,string>::iterator it = pageMap.begin(); it != pageMap.end(); it++) {
            // it->first = key
            
            if(it->second=="")continue;
            if(it->first=="OSBL")continue;
            if(it->first=="ID")continue;
            if(it->first=="title_pali")continue;
            if(it->first=="title_eng")continue;
            if(it->first=="title_rus")continue;
            if(it->first=="chapter_code")continue;
            if(it->first=="info_eng")continue;
            if(it->first=="info_rus")continue;
            if(it->first=="Title")continue;
            if(it->first=="Category")continue;
            
            if(it->first=="I_COMMENTARY_EN"){
                report+="<b>Commentary:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="M_VOLUME"){
                report+="<b>Volumes:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="NODE_LEVEL"){
                report+="<b>Publication type:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="P_TBRC_CODE"||it->first=="tbrc_code"){
                continue;
            }
            if(it->first=="creator"){
                report+="<b>Author:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="J_COMMENTARY_RU"){
                report+="<b>Комментарий:</b> "+it->second+"<br/>\n";
                continue;
            }
            
            if(it->first=="creatortype")continue;
            if(it->first=="creatorothers")continue;
            if(it->first=="disk")continue;
            if(it->first=="file_name")continue;
            if(it->first=="fileName")continue;
            if(it->first=="a_Title")continue;
            
            
            if(it->first=="Path")continue;
            if(it->first=="Q_COPY")continue;
            
            
            report+=it->first+": "+it->second+"<br/>\n";
        }
        if(pageMap["P_TBRC_CODE"]!=""){
            report+="<a href=\"https://www.tbrc.org/#!rid="+pageMap["P_TBRC_CODE"]+"\" target=\"_blank\" class=\"open_link\">Open this book in www.tbrc.org</a><br/>\n";
        }else if(pageMap["tbrc_code"]!=""){
            report+="<a href=\"https://www.tbrc.org/#!rid="+pageMap["tbrc_code"]+"\" target=\"_blank\" class=\"open_link\">Open this book in www.tbrc.org</a><br/>\n";
        }
        
        report+=out.str();
        report+=out_.str();
        //reportStr=str_replace(" ","</tsh> <tsh>",reportStr);
        reportStr=str_replace("/n","¶",reportStr);
        vector<string>v=explode(">",reportStr);
        for(int i=0;i<v.size();i++){
            if(isPali(v[i])||v[i].find("་")!=-1){
                string p1=substr(0,(uint)v[i].find("<"), v[i]);
                string p2=substr((uint)v[i].find("<"),(uint)v[i].size(), v[i]);
                p1=str_replace(" ","</tsh> <tsh>",p1);
                p1=str_replace("།","</tsh>།<tsh>",p1);
                p1="<tsh>"+p1+"</tsh>";
                v[i]=p1+p2;
            }
        }
        for(int i=0;i<v.size();i++){
            if(v[i].find("<")!=-1)v[i]+=">";
        }
        string delimeter ="";
        reportStr=implode(delimeter,v);
        render=reportStr+"</tsh>\n<hr/>\n"+report;
        render=str_replace("<br />", "<br/>", render);
        render=str_replace("<br>", "<br/>", render);
        re2::RE2::Replace(&stData,"(<br/>)+","<br/>");
        render=str_replace("</div><br/><div>", "</div><div>", render);
        render=str_replace(">>", ">", render);
        render=str_replace("<hr/>\n<hr/>", "<hr/>", render);
        render=str_replace("<hr/><hr/>", "<hr/>", render);
        render=str_replace("<br/><div>", "<div>", render);
        render=str_replace("<div><br/>", "<div>", render);
        stData=render;
    
        return stData.c_str();
    }
    
    
    EXPORT
    const char* renderCategory(pageRec *pageRec, const char *ln){
        string text=pageRec->text;
        string str;
        string report;
        string reportStr;
        ostringstream out;
        ostringstream out_;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        
        str=pageRec->db;
        if(str=="")str="DHARMABOOK";

        
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        if(pageRec->index>v->size()){pageRec->text="this record not exist"; return "";}
        TString st;
        v->getTStr(pageRec->index,st);
        if(pageRec->field>=st.size()){pageRec->text="this field not exist"; return "";}

        
        if(pageRec->field==1){
        
            //рендер содержания
            string pageText=st[1];
            str=pageText;
            
            if(str.find("===")!=-1){
                out<<"<a class=\"content\" href=\"/?field=1&index="<<pageRec->index<<"&ocrData=read\">Content • Содержание</a><br/>\n";
                for(int i=0;i<st.size();i++){
                    string s=st[i];
                    vector <string>lineArray=explode("¶",s);
                    for(int n=0;n<lineArray.size();n++){
                        if(lineArray[n].size()>3&&lineArray[n].find("===")==0){
                            out<<"<a class=\"content\" href=\"/?field="<<i<<"&index="<<pageRec->index<<"&ocrData=read\">"<<lineArray[n]<<"</a><br/>\n";
                        }
                    }
                }
            }
            str=str_replace("¶","\n",str);
            reportStr=out.str();
            reportStr+=renderCategoryText(str.c_str(),pageRec->index);
            //cout<<"reportStr:"<<reportStr<<"/"<<endl;
            //reportStr=st[1];
            
            
            

        
        }else{
            reportStr=st[pageRec->field];
        }
        
        //str=str_replace("<br/>","",reportStr);
        //str=str_replace("\n","",str);
        //str=str_replace("<hr/>","",str);
        //str=str_replace(" ","",str);
        //cout<<"@@@"<<str<<"/@@@"<<str.size()<<endl;
        //if(str!=""){
        //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
        //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
        //    reportStr=str_replace("<br/>\n<br/>\n", "<br/>\n", reportStr);
        
        //}else{
        //    stData=report;
        //}
        
        stData=reportStr;
        stData=str_replace("¶","<br/>",stData);
        re2::RE2::Replace(&stData,"(<br/>)+","<br/>");
        
        stData=str_replace("<tsh><br/></tsh>","<br/>",stData);
        stData=str_replace("<br/><br/>","<br/>",stData);
        stData=str_replace("<br/>\n<hr/>","<hr/>",stData);
        stData=str_replace("<hr/>\n<a href","<a href",stData);
        
        stData=str_replace("<br/>===","<br/><hr/><b>===",stData);
        stData=str_replace("===<br/>","===</b><br/>",stData);
        return stData.c_str();
    
    }
    EXPORT
    const char* renderCategorySearch(pageRec *pageRec, const char *ln){
        string text=pageRec->text;
        text=str_replace("/Volumes/TERSAR_3TB/_LIBRARY_/","/___DHARMA___/___LIBRARY___/", text);
        
        string str;
        string report;
        string reportStr;
        //рендер шаблонов
        vector<string>page=explode("{{",text);
        map<string,string>pageMap;
        for(int i=0;i<page.size();i++){
            str=page[i];
            if(str.find("|")!=-1&&str.find("}}")!=-1){
                string line=substr(0, (uint)str.find("}}"), str);
                vector<string>v=explode("|",line);
                if(pageMap.find(v[0])!=pageMap.end())continue;
                pageMap[v[0]]=v[1];
                str=substr((uint)str.find("}}")+2, (uint)str.size(),str);
                reportStr+=str_replace("\n","<br/>\n",str);
            }else{
                reportStr+=str_replace("\n","<br/>\n",str);
            }
        }
        for(map<string,string>::iterator it = pageMap.begin(); it != pageMap.end(); it++) {
            // it->first = key
            if(it->second=="")continue;
            if(it->first=="OSBL")continue;
            if(it->first=="ID")continue;
            if(it->first=="I_COMMENTARY_EN"){
                report+="<b>Commentary:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="M_VOLUME"){
                report+="<b>Volumes:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="NODE_LEVEL"){
                report+="<b>Publication type:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="P_TBRC_CODE"||it->first=="tbrc_code"){
                continue;
            }
            if(it->first=="creator"){
                report+="<b>Author:</b> "+it->second+"<br/>\n";
                continue;
            }
            if(it->first=="J_COMMENTARY_RU"){
                report+="<b>Комментарий:</b> "+it->second+"<br/>\n";
                continue;
            }
            
            if(it->first=="creatortype")continue;
            if(it->first=="creatorothers")continue;
            if(it->first=="disk")continue;
            if(it->first=="file_name")continue;
            if(it->first=="fileName")continue;
            if(it->first=="a_Title")continue;
            
            
            if(it->first=="Path")continue;
            if(it->first=="Q_COPY")continue;
            
            
            report+=it->first+": "+it->second+"<br/>\n";
        }
        stData=report+"<br/>\n"+reportStr;
        stData=str_replace("<br/>", " ",stData);
        stData=str_replace("\n", "",stData);
        stData=str_replace("@@@", "",stData);
        
        return stData.c_str();
        
    }

    const char* renderPage(const char *pageText){

        stData=pageText;
        stData=str_replace("¶","<br/>\n",stData);
        
        return stData.c_str();
    
    }
    
    const char* checkCode(pageRec *page){
        string str=page->db;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        string ret="0"; ret[0]=11;
        cout<<v->size();
        for(int i=0;i<v->size();i++){
            //if(i%100==0)
            cout<<" "<<i;
            TString st;
            v->getTStr(i,st);
            string text=st[0];
            
            //рендер шаблонов
            vector<string>page=explode("{{",text);
            map<string,string>pageMap;
            for(int i=0;i<page.size();i++){
                str=page[i];
                if(str.find("|")!=-1&&str.find("}}")!=-1){
                    string line=substr(0, (uint)str.find("}}"), str);
                    vector<string>v=explode("|",line);
                    pageMap[v[0]]=v[1];
                    //report+=substr((uint)str.find("}}")+2, (uint)str.size(),str);
                }else{
                    //report+=str;
                }
            }
            
        }
        return "";
    }
    
    static bool sortL(const string &d1, const string &d2){
        return d1.size() > d2.size();
    }

    
    void exportWordList(const char*path){
        if(path==NULL)return;
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        vector<string>dict;
        string str="mainDict";
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        for(int i=0;i<v->size();i++){
            TString st;
            v->getTStr(i,st);
            dict.push_back(st[0]);
        }
        str="translationDictRus";
        longMemory->loadTable(str);
        v=longMemory->table[str].data;
        for(int i=0;i<v->size();i++){
            TString st;
            v->getTStr(i,st);
            dict.push_back(st[0]);
        }
        sort(dict.begin(),dict.end(),sortL);
        str=path;
             writeText(dict, str);
    
    }
    
    void sortWordList(const char*path){
        if(path==NULL)return;
        vector<string>dict;
        string str=path;
        readText(dict, str);
        vector<string>dict_;
        string key;
        string report;
        
        for(int i=0;i<dict.size();i++){
            if(i%100000==0)cout<<" "<<i/100000;
            string s=dict[i];
            setSearchText(s);
            //vector<string>v=explode("\n",s);
            //dict_.insert(dict_.end(), v.begin(), v.end());
            //report+=s+"\n";
            //dict[i]=s;
            dict_.push_back(s);
        }
        cout<<"start sort"<<endl;
        sort(dict_.begin(),dict_.end());
        dict.resize(0);
        dict.push_back(dict_[0]);
        for(int i=1;i<dict_.size();i++){
            if(dict_[i]!=dict_[i-1])dict.push_back(dict_[i]);
        }
        sort(dict.begin(),dict.end(),sortL);
        str+="_.txt";
        writeText(dict, str);
        
    }
    
    void exportDBTable(const char *db){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string ext="txt";
        longMemory->exportTable(str,ext);
        
    
    }
    
    void importDBTable(const char *db,const char *path){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string path_=path;
        longMemory->importTable(str,path_);
    
    }
    
    void importOCR(const char *db,const char *path,const char *delimeter){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string path_=path;
        string delimeter_=delimeter;
        longMemory->importOCR(str,path_,delimeter_);
    }
    
    void importTextFolder(const char *db,const char *path,const char *category){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string path_=path;
        string category_=category;
        longMemory->importTextFolder(str,path_,category_);
    
    }
    
    void importText(const char *db,const char *path,int index){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string path_=path;
        longMemory->importText(str,path_,index);
    }
    
    void importDictTab(const char *db,const char *path,const char *category,int year){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        string path_=path;
        string category_=category;
        longMemory->importDictTab(str,path_,category_,year);
        
    }
    
    void importMainDictionary(){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string db="DHARMABOOK";
        string dbDict="mainDict";
        longMemory->loadTable(db);
        GVector *v=longMemory->table[db].data;
        longMemory->loadTable(dbDict);
        GVector *vDict=longMemory->table[dbDict].data;
        long t=time(NULL);
        t-=60*60*24*10;  //10 years ago;
        string str;
        
        for(int i=0;i<vDict->size();i++){
            if(i%10000==0)cout<<" "<<i;
            TString st;
            vDict->getTStr(i, st);
            if(st.size()<3){
                cout<<i<<" "<<st.size()<<st[3]<<endl;
                continue;
            }

            TString st_;
            ostringstream out;
            out<<"[[ID:"<<t<<"]]";
            str=out.str();
            st_.push_back(str);
            t++;
            
            if(st[2].size()>12){
                //cout<<"  "<<st.size()<<"    "<<st[0]<<"//// "<<st[1]<<"//// "<<st[2]<<"//// "<<st[3]<<endl;
                str="[[Title:"+st[0]+"]]¶";
                str+="[[Category:Dictionary]]¶";
                str+="[[Category:Dictionary__PL]]¶";
                for(int n=1;n<st.size();n++)str+=st[n];
                str=str_replace("]]]","] ]]", str);
                st_.push_back(str);
            }else{
                str="[[Title:"+st[0]+"]]¶";
                str+="[[Category:Dictionary]]¶";
                if(st[2]!=""){
                    str+="[[Category:Dictionary__"+st[2]+"]]¶";
                }else{
                    str+="[[Category:Dictionary__TR]]¶";
                }
                str+=st[1];
                str=str_replace("]]]","] ]]", str);
                st_.push_back(str);
            }
            v->push_back(st_);
        }
        
    
    }
    
    void checkDublicateID(const char *db){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        map<string,int>cMap;
        for(int i=0;i<v->size();i++){
            TString st;
            v->getTStr(i,st);
            str=st[0];
            if(cMap.find(str)!=cMap.end()){
                //cout<<str<<" i:"<<i<<" i_:"<<cMap[str]<<endl;
                /*
                int index1;
                int index2;
                if(i<cMap[str]){
                    index1=i;
                    index2=cMap[str];
                }else{
                    index2=i;
                    index1=cMap[str];
                }
                TString st1;
                v->getTStr(index1,st1);
                TString st2;
                v->getTStr(index2,st2);
                for(int n=1;n<st2.size();n++){
                    str=st2[n];
                    st1.push_back(str);
                }
                v->putTStr(index1,st1);
                TString st3;
                st3.push_back("");
                st3.push_back("");
                v->putTStr(index2,st3);
                */ 
                
            }else{
                cMap[str]=i;
            }
        }
    
    }
    
    
    EXPORT
    const char* savePageInDB(const char *db,const char *text,int index,int field){
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string str=db;
        longMemory->loadTable(str);
        GVector *v=longMemory->table[str].data;
        string text_=text;
        text_=str_replace("\n","¶",text_);
        //cout<<"save page:"<<index<<" field:"<<field<<" text:"<<text_<<endl;
        TString st;
        v->getTStr(index,st);
        TString st_;
        for(int i=0;i<st.size();i++){
            if(i==field){
                st_.push_back(text_);
            }else{
                str=st[i];
                st_.push_back(str);
            }
        }
        v->putTStr(index,st_);
        
        stData="done save page";
        return stData.c_str();
        
    }
    
    EXPORT
    void bookOCR(const char *path){
        cout<<path;
        inputData.data["ocrData"]="vectorOCR";
        //inputData.data["ocrData"]="batchOCR";
        inputData.fileList.push_back(path);
        GMainEditor *mainEditor=(GMainEditor *)inputData.mainEditor;
        mainEditor->readInputAction();
    }
    
    EXPORT
    void rebuildCategory(){
        string str;
        string report;
        string reportStr;
        ostringstream out;
        
        //рендер содержания
        str="DHARMABOOK";
        string path=inputData.data["root"]+"OCRData/DATA_DB/DHARMABOOK_Category/DHARMABOOK_Category.bin";
        remove(path.c_str());
        path=inputData.data["root"]+"OCRData/DATA_DB/DHARMABOOK_Category/DHARMABOOK_Category_0_1.index";
        remove(path.c_str());
        
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        GVector *v=longMemory->table[str].data;
        str="DHARMABOOK_Category";
        longMemory->loadTable(str);
        GVector *c=longMemory->table[str].data;
        string lt="[[Category:";
        uint l=(uint)lt.size();
        map<string,string >cMap;

        for(uint index=0;index<v->size();index++){
        
            TString st;
            v->getTStr(index,st);
            if(index%100000==0)cout<<" "<<index;
            
            str=st[1];
            str=str_replace("[[Category:","\n[[Category:",str);
            str=str_replace("]]","]]\n",str);
            vector <string>lineArray=explode("\n",str);
            for(int n=0;n<lineArray.size();n++){
                str=lineArray[n];
                if(lineArray[n].find("[[Category:")==0&&str.find("]]")!=-1){
                    string line=substr(l, (uint)str.find("]]")-l, str);
                    TString st;
                    //cout<<line<<" --> "<<index<<endl;
                    string s_;
                    s_.resize(4);
                    memcpy(&s_[0],&index,4);
                    cMap[line]+=s_;
                    
                }
            }
            
        }
        map<string,string >::iterator it;
        
        for(it=cMap.begin();it!=cMap.end();it++){
            TString st;
            string s=it->first;
            cout<<it->first<<" --> "<<it->second.size()<<endl;
            
            st.push_back(s);
            st.push_back((char*)&it->second[0],(uint)it->second.size());
            c->push_back(st);
            
        }
    }
    
    
    const char* searchByTag(const char *db_,const char *tag, const char *request, int mode){
        
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        
        ostringstream out;
        string key=request;

        GVector *v,*v_;
        GMap *d;
        string db;
        string dbIndex;
        string str,report;
        
        //load main db
        db=db_;
        longMemory->loadTable(db);
        v=longMemory->table[db].data;
        
        //load index db
        dbIndex=db_;
        dbIndex+="_";
        dbIndex+=tag;
        
        longMemory->loadTable(dbIndex);
        v_=longMemory->table[dbIndex].data;
        if(!v_->size()){   //build index
            string tagLine="[[";
            tagLine+=tag;
            tagLine+=":";
            uint l=(uint)tagLine.size();
            map<string,string >cMap;
            
            for(uint index=0;index<v->size();index++){
                
                TString st;
                v->getTStr(index,st);
                
                str=st[1];
                str=str_replace("[[","\n[[",str);
                str=str_replace("]]","]]\n",str);
                vector <string>lineArray=explode("\n",str);
                for(int n=0;n<lineArray.size();n++){
                    str=lineArray[n];
                    if(lineArray[n].find(tagLine)==0&&str.find("]]")!=-1){
                        string line=substr(l, (uint)str.find("]]")-l, str);
                        TString st;
                        //cout<<line<<" --> "<<index<<endl;
                        string s_;
                        s_.resize(4);
                        memcpy(&s_[0],&index,4);
                        cMap[line]+=s_;
                    }
                }
            }
            map<string,string >::iterator it;
            
            for(it=cMap.begin();it!=cMap.end();it++){
                TString st;
                string s=it->first;
                //cout<<it->first<<" --> "<<it->second.size()<<endl;
                st.push_back(s);
                st.push_back((char*)&it->second[0],(uint)it->second.size());
                v_->push_back(st);
            }
        }
        
        
        indexRecord *indexRec=longMemory->createIndex(longMemory->table[dbIndex], 0, mode);
        d=indexRec->mIndex;

        
        vector<TString>r;
        vector<int>f;
        f.push_back(0);
        longMemory->select(dbIndex, f, key, mode,r,1);
        //cout<<r.size()<<endl;
        
        if(mode==HASH_SEARCH){
            
            if(r.size()){
                TString s=r[0];
                s.reloadPtr();
                vector<int>res;
                s.getIntVector(res, 1);
                //cout<<" ->> "<<s[0]<<" "<<str.size()<<" "<<res[0];

                for(int i=0;i<res.size();i++){
                    TString st;
                    v->getTStr(res[i],st);
                    string t=st[1];
                    out<<renderPage(t.c_str());
                    out<<"<a href=\"/?field=1&index="<<res[i]<<"&ocrData=read\" target=\"_blank\"> • edit</a><hr/>";
                
                }
                
                //cout<<i<<" ->> "<<s.getInt(0)<<" "<<s.getInt(1)<<" "<<s.getInt(2)<<endl;
                //rLink *link;
                //string str=s[0];
                //link=(rLink*)&str[0];
                //TString dataStr;
                //v_->getTStr(link->index,dataStr);
                //string st=dataStr[1];
            }
        }
        
        stData=out.str();
        return stData.c_str();
    }
    
    
    //render DB report for request category
    const char* searchCategory(const char *request,int indexPage,int  *pageCount){
        //return "";

        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string key=request;
        GVector *dk,*dBook;
        GMap *d;
        string db="DHARMABOOK_Category";
        string str,report;
        
        longMemory->loadTable(db);
        dk=longMemory->table[db].data;
        indexRecord *indexRec=longMemory->createIndex(longMemory->table[db], 0, HASH_SEARCH);
        d=indexRec->mIndex;
        
        uint index=(uint)d->getHKey(key, 0);
        //cout<<index<<endl;
        ostringstream out;
        
        if(index!=-1){
            string titleRequest="CategoryPage_";
            titleRequest+=request;
            //search main DB for this category front page.
            if(indexPage==0){
                out<<searchByTag("DHARMABOOK","Title",titleRequest.c_str(),HASH_SEARCH);
            }
            TString st;
            dk->getTStr(index, st);
            vector<int>v;
            st.getIntVector(v, 1);
            db="DHARMABOOK";
            longMemory->loadTable(db);
            dBook=longMemory->table[db].data;
            *pageCount=(uint)v.size()/100+1;
            out<<"\n<tb>Category: "<<key<<" Total: "<<v.size()<<" records.</tb><hr/>";
            out<<"\n<div style=\"position:relative;top:40px:left:37px; width:80%\">";

            //cout<<"pageCount:"<<*pageCount<<" v.size():"<<v.size()<<endl;
            
            for(int i=indexPage;i<v.size()&&i<indexPage+100;i++){
                TString st;
                dBook->getTStr(v[i], st);
                if(st.size()>1){
                    str=st[1];
                    out<<"<a href=\"/?field=1&index="<<v[i]<<"&ocrData=read\" target=\"_blank\">";
                    out<<"Open BookID:"<<v[i]<<"<br/>";
                    string s=substr(0,(uint)str.find(" ",1024),str);
                    if(s.size()>2048)s=substr(0,(uint)str.find("་",1024),str);
                    if(s.size()>2048)s=substr(0,1024,str);
                    s=str_replace("<","{",s);
                    s=str_replace(">","}",s);
                    s=renderCategoryText(s.c_str(), 0);
                    s=str_replace("<hr/>"," ",s);
                    s=str_replace("<br/>"," ",s);
                    out<<s<<"</a><hr/>\n";
                }
            
            }
            
        }
        stData=out.str();
        stData=str_replace("¶"," ",stData);
        //cout<<stData;
        return stData.c_str();
    
    }
    
    void setCategory(const char *key_,const char *category_){

        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        string key=key_;
        string category=category_;
        GVector *dBook;
        string db="DHARMABOOK";
        string str,report;
        
        longMemory->loadTable(db);
        dBook=longMemory->table[db].data;
        for(int i=0;i<dBook->size();i++){
            if(i%100==0)cout<<" "<<i;
            TString st;
            TString st1;
            dBook->getTStr(i, st);
            if(st.size()>1){
                str=st[1];
                if(str.find(key)!=-1){
                    str="[[Category:"+category+"]]¶"+str;
                    string s=st[0];
                    st1.push_back(s);
                    st1.push_back(str);
                    for(int n=2;n<st.size();n++){
                        string s=st[n];
                        st1.push_back(s);
                    }
                    dBook->putTStr(i, st1);
                }
            }
            
        }
    }

    EXPORT
    const char* listCategory(){
        //return "";
        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        GVector *cV;
        string db="DHARMABOOK_Category";
        string str,report;
        longMemory->loadTable(db);
        cV=longMemory->table[db].data;
        ostringstream out;
        multimap <int,string>cMap;
        map <string,int>dMap;
        
        for(uint i=0;i<(uint)cV->size();i++){
            TString st;
            cV->getTStr(i,st);
            vector<int>v;
            st.getIntVector(v, 1);
            if(st[0].find("__")!=-1){
                string up=st[0];
                up=substr(0, (uint)up.find("__"), up);
                dMap[up]++;
                continue;
            }
            cMap.insert(pair<int,string>(v.size(),st[0]));
            
        }
        
        multimap <int,string>::iterator it;
        it=cMap.end();
        
        while(1){
            if(it->second.size()){
                string link=it->second;
                encodeURL(link);
            
                out<<"<a class=\"content\" href=\"/?category="<<link<<"&index=0&ocrData=readCategory\" target=\"_blank\">"<<it->second<<"</a> "<<it->first<<" records";
                if(dMap.find(it->second)!=dMap.end()){
                    out<<", "<<dMap[it->second]<<" category";
                }
                out<<"<br>\n";
            }
            if(it==cMap.begin())break;
            it--;
        }
        
        
        
        stData=out.str();
        return stData.c_str();
        
    
    }
    EXPORT
    const char* addRecord(){

        if(!inputData.init)initCommandData();
        GMemory* longMemory=(GMemory*)inputData.longMemory;
        ostringstream out;
        GVector *v;
        string db="DHARMABOOK";
        string str,report;
        longMemory->loadTable(db);
        v=longMemory->table[db].data;
        TString st;
        long time_=time(NULL);
        out<<"[[ID:"<<time_<<"]]";
        str=out.str();
        out.str("");
        st.push_back(str);
        st.push_back("");
        v->push_back(st);
        out<<v->size()-1;
        
        stData=out.str();
        return stData.c_str();
    }
    
    
    
    
#ifdef OCR_DYLIB
};
#endif




