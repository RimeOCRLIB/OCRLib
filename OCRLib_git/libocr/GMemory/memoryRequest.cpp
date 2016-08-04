#include "GMemory.h"
#include "GVector.h"
#include "php2stl.h"


void GMemory::memoryRequest(string &destString){
 

    destString="";
    uint rowsNum=0;
    uint pageNum=0;
    string str;
    string searchString;
    rowsNum=atoi(inputData.data["rows"].c_str());
    if(rowsNum>1000||rowsNum<10)rowsNum=100;
    
    string db=inputData.data["db"];
    string ln=inputData.data["ln"];
    string mode=inputData.data["mode"];
    //cout<<"Database name="<<db<<" text="<<inputData.data["text"]<<" ln="<<ln<<endl; exit(0);
    string strName;

    if(mode=="LIST_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        vector<TString>result;
        //string query="Volumes";
        select(db,field, inputData.data["word"], LIST_SEARCH, result,25);
        for(int i=0;i<result.size();i++)
            cout<<result[i][0]<<":|:";
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    if(mode=="BOOK_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        vector<TString>result;
        string query=inputData.data["book"];
        //cout<<"book="<<query<<endl;
        
        select(db,field, query, TEXT_SEARCH, result,25);
        if(result.size()){
            int l=0;
            while(l<result.size()){
                string str=result[l][0];
                if(str.find(query)!=-1){
                    cout<<str; break;
                }
                l++;
            }
        }
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    if(mode=="CORPUS_ID_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        vector<TString>result;
        //cout<<" id="<<inputData.data["corpusID"]<<endl;
        select(db,field, inputData.data["corpusID"], HASH_SEARCH, result,25);
        //cout<<" res="<<result.size()<<endl;
        //if(result.size())cout<<result[0][0];
        TString &st=result[0];
        string res;
        for(int n=0;n<st.size();n++){
            string s=st[n];
            if(s.size()>9)res+=s+"\n";
        }
        cout<<res;
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    if(mode=="CORPUS_TEXT_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        vector<TString>result;
        //string query="Volumes";
        select(db,field, inputData.data["corpusID"], TEXT_SEARCH, result,25);
        cout<<"<table width=\"90%\" align=\"center\" border=\"1\">";
        for(int i=0;i<result.size();i++){
            cout<<"<tr><td width=\"200px\">";
            TString st=result[i];
            for(int n=0;n<st.size();n++){
                cout<<st[n]<<"</td><td>";
            }
            cout<<"</td></tr>";
        }
        cout<<"</table>";
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    if(mode=="HASH_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        vector<TString>result;
        //string query="Volumes";
        select(db,field, inputData.data["word"], HASH_SEARCH, result,25);
        cout<<"<table width=\"90%\" align=\"center\" border=\"1\">";
        for(int i=0;i<result.size();i++){
            cout<<"<tr><td width=\"200px\">";
            TString st=result[i];
            for(int n=0;n<st.size();n++){
                cout<<st[n]<<"</td><td>";
            }
            cout<<"</td></tr>";
        }
        cout<<"</table>";
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    
    if(mode=="TEXT_SEARCH"){
        int fieldIndex=atoi(inputData.data["field"].c_str());
        vector<int> field; field.push_back(fieldIndex);
        int limit=atoi(inputData.data["limit"].c_str());
        vector<TString>result;
        //string query="Volumes";
        select(db,field, inputData.data["word"], TEXT_SEARCH, result,limit);
        for(int i=0;i<result.size()&&i<limit;i++){
            TString st=result[i];
                cout<<st[fieldIndex]<<endl;
        }
        remove(inputData.data["log"].c_str());
        exit(0);
    }

    
    if(ln=="rus"&&db=="dict"&&mode=="list"){
        string word=inputData.data["word"];
        /*

        int len=dictWords->size()-1;
        string str;
        map<string,int>result;
        for(int n=len;n>-1;n--){
            dictWords->getStr(n,str);
            if(str.find(word)==0){
                result[str]++;
                if(result.size()>25)break;
            }
        }
        map<string,int>::iterator iter;
        for (iter = result.begin(); iter != result.end(); ++iter){
            cout<<iter->first<<":|:";
        }
        */
        remove(inputData.data["log"].c_str());
        exit(0);
        //cout<<keyDict->size();
        
        
    }
    

    if(inputData.data["act"]=="process"){  //запрос на обработку базы
        cout_<<"start process";
        uint recordNum=vectorDict->size();  
        
        uint index=0;
        while(index<recordNum){
            string strDict; vectorDict->getStr(index,strDict);
            if(!strDict.size()){continue;}            
            vector<string> vDict=explode(":|:",strDict);
            if(vDict.size()<5)continue;
            
            for(int cellNum=0;cellNum<5;cellNum++){  
            }
            index++;
        }
        cout_<<" end process";
        inputData.data["HTTPType"]="text/html";
        
    }
    if(inputData.data["record"]=="find"&&inputData.data["text"]=="" ){  //запрос на поиск данных в базе
        
        //cout_<<" start search"<<endl;
        string query;   
        //searchString="&record=find&c1="+inputData.data["c1"]+"&c2="+inputData.data["c2"]+"&c3="+inputData.data["c3"]+"&c4="+inputData.data["c4"]+"&c5="+inputData.data["c5"];
        
        if(inputData.data["c0"].size()>5&&substr(0,2,inputData.data["c0"])=="<<"){
            string str=substr(2,inputData.data["c0"]);    cout_<<" new c0="<<str<<endl;
            query=str;
        }else if(inputData.data["c0"].size()){
            query=inputData.data["c0"];
        }
        if(inputData.data["c1"].size()>5&&substr(0,2,inputData.data["c1"])=="<<"){
            string str=substr(2,inputData.data["c1"]);    cout_<<" new c1="<<str<<endl;
            query=str;    
        }else if(inputData.data["c1"].size()){
            query=inputData.data["c1"];      
        }
        if(inputData.data["c2"].size()>5&&substr(0,2,inputData.data["c2"])=="<<"){
            string str=substr(2,inputData.data["c2"]);    cout_<<" new c2="<<str<<endl;
            query=str;  
        }else if(inputData.data["c2"].size()){
            query=inputData.data["c2"];  
        }
        if(inputData.data["c3"].size()>5&&substr(0,2,inputData.data["c3"])=="<<"){
            string str=substr(2,inputData.data["c3"]);    cout_<<" new c3="<<str<<endl;
            query=str;  
        }else if(inputData.data["c3"].size()){
            query=inputData.data["c3"];    
        }
        if(inputData.data["c4"].size()>5&&substr(0,2,inputData.data["c4"])=="<<"){
            string str=substr(2,inputData.data["c4"]);    cout_<<" new c4="<<str<<endl;
            query=str;
        }else if(inputData.data["c4"].size()){
            query=inputData.data["c4"];  
        }
        if(inputData.data["c5"].size()>1){
            if(inputData.data["c5"][0]=='>'){
                string str=substr(1,inputData.data["c5"]);    cout_<<" new c5="<<str<<endl;
                query=str;  
            }else if(inputData.data["c5"][0]=='<'){
                string str=substr(1,inputData.data["c5"]);    cout_<<" new c5="<<str<<endl;
                query=str;  
            }else {
                query=inputData.data["c5"];
            }   
        }else if(inputData.data["c5"].size()){
            query=inputData.data["c5"];      
        }
        
        //       TIME_START
        //cout<<"start search query="<<query<<" db="<<db<<endl;
        string strRec;
        vector<uint>searchResult;
        
        if(db=="dict"){
            vector<int> field; field.push_back(0);
            vector<TString>result;
            //string query="Volumes";
            db="mainDict";
            select(db,field, query, HASH_SEARCH, result,25);
            cout<<"<table width=\"90%\" align=\"center\" border=\"1\">";
            for(int i=0;i<result.size();i++){
                cout<<"<tr><td width=\"200\">";
                TString st=result[i];
                for(int n=0;n<st.size();n++){
                    cout<<st[n]<<"</td><td>";
                }
                cout<<"</td></tr>";
            }
            cout<<"</table>";
            
            remove(inputData.data["log"].c_str());
            exit(0);

            
                for(uint i=0;i<vectorDict->size();i++){
                    vectorDict->getStr(i,strRec);
                    if(strRec.find(query)==-1)continue;
                    TString st; vectorDict->getTStr(i,&st);
                    //cout<<"@find="<<strRec.find(query)<<" strRec="<<st[0]<<endl;
                    strRec=st[0];
                    if(strRec!=""){
                        if(strRec.find(query)==0){
                            if(strRec.size()<query.size()+5){
                                searchResult.push_back(i);
                                //cout<<"@find="<<strRec.find(query)<<" strRec="<<st[0]<<" s1="<<strRec.size()<<" s2="<<query.size()<<endl;
                            }
                        }
                    }else{
                        searchResult.push_back(i);
                    }
                }
        }
        if(db=="corpus"){
            if(inputData.data["c5"]==""){
                uint index=0;
                for(uint i=0;i<vectorCorpus->size();i++){
                    vectorCorpus->getStr(i,strRec);
                    if(strRec.find(query)==-1)continue;
                    index++;
                    if(pageNum>0&&index<pageNum*25-1)continue; 
                    searchResult.push_back(i);
                    if(searchResult.size()>24)break;
                    
                    
                }
            }else{
                uint index=atoi(inputData.data["c5"].c_str());
                searchResult.push_back(index);
            } 
        }
        if(db=="letter"){
            uint index=atoi(inputData.data["c5"].c_str()); cout_<<"@@@index="<<index<<" query="<<query;
            if(index){
                searchResult.push_back(index);
            }else{
                for(uint i=0;i<aliKali->size();i++){
                    GLetter *letter=aliKali->getLetter(i);
                    if(letter->name!=query){letter->destroy();continue;}
                    //cout_<<"str="<<letter->name<<endl;
                    index++;
                    if(pageNum>0&&index<pageNum*25-1)continue;
                    searchResult.push_back(i);
                    letter->destroy();
                }
            }
        }
        
        if(db=="lib"){
            cout_<<" query="<<query<<" size="<<vectorLib->size()<<endl;
            for(uint i=0;i<vectorLib->size();i++){
                vectorLib->getStr(i,strRec); 
                if(strRec.find(query)==-1)continue;
                //TString st; vectorLib->getTStr(i,&st);
                //cout_<<"@find="<<strRec.find(query)<<" strRec="<<strRec<<endl;
                //strRec=st[0];
                searchResult.push_back(i);
            }
        }
        cout_<<"end search searchResult="<<searchResult.size();
        //TIME_PRINT_
        //exit(0);
        string result; 
        
        if(inputData.data["db"]=="corpus"){
            if(rowsNum>searchResult.size())rowsNum=(int)searchResult.size();
            if(rowsNum==0){
                result="<tr><td></td><td>no record found</td></tr>";
            }else{    
                GVector *strVector=GVector::create();
                int mode=CORPUS_HTML;
                uint index=0;
                TString st;
                while(index<rowsNum){
                    TString strDict; vectorCorpus->getTStr(searchResult[index],&strDict);
                    strVector->push_back(&strDict);
                    index++;
                }
                //((GLogicProcessor*)inputData.logicProcessor)->TibRusDictReport(vectorDict,keyDict,strVector);
                vectorCorpus->drawHTML(0,rowsNum,strVector,result,mode);
                vectorCorpus->drawHTML(searchResult,strVector,result,mode);
                strVector->free();  
            }     
        }
        if(inputData.data["db"]=="dict"){
            int mode=HTML;
            vectorDict->drawHTML(searchResult,result,mode);
        }
        if(inputData.data["db"]=="letter"){
            int mode=HTML;
            aliKali->drawOCRBasePict(searchResult,mode);
            aliKali->drawHTML(searchResult,result,mode);
        }
        if(inputData.data["db"]=="lib"){
            int mode;
            if(inputData.data["ID"]==""){
               mode=HTML;
            }else{
               mode=CHAT;
            }
            vectorLib->drawCatalogHTML(searchResult,vectorLibPath,result,mode);
        }
        destString=result;
        if(result=="")destString="no records found";
        inputData.data["HTTPType"]="text/html"; 
    }
    if(inputData.data["record"]=="findDuplicate"){
        if(db=="letter"){
            vector<uint>searchResult;
            
            for(uint i=0;i<aliKali->size();i++){
                GLetter *letter=aliKali->getLetter(i);
                uint index=0; int flag=0;
                
                while(letter->key[index].correlation>98&&letter->name!="*"){
                    GLetter *letter_=aliKali->getLetter(letter->key[index].letterIndex);
                    if(letter->name==letter_->name){
                        //searchResult->push_back(letter->key[index].letterIndex);
                        flag=1; break;
                    }
                    letter_->destroy();
                    index++;
                }
                if(flag)searchResult.push_back(i);
                
                //if(letter->keyCount()==0)searchResult->push_back(i);
                letter->destroy();
            }
            int mode=HTML;
            string result;
            
            //
            //for(itr = matchStat.begin(); itr != matchStat.end(); ++itr){
            //    if(matchStat[(*itr).first]>3)
            //       matchSort.insert(pair<int,string>((*itr).second, (*itr).first));
            //}
            
            
            multimap<string,int>list;
            for(uint i=0;i<searchResult.size();i++){
                GLetter *letter=aliKali->getLetter(searchResult[i]);
                list.insert(pair<string,int>(letter->name, searchResult[i]));
                letter->destroy();
            }
            searchResult.resize(0);
            map<string, int>::const_iterator itr;
            for(itr = list.begin(); itr != list.end(); ++itr){
                searchResult.push_back((*itr).second);
            }
            
            
            aliKali->drawOCRBasePict(searchResult,mode);
            aliKali->drawHTML(searchResult,result,mode);
            destString=result;
            if(result=="")destString="no records found";
            inputData.data["HTTPType"]="text/html";
        }
        
    }
    if(inputData.data["record"]=="findIncomplete"){
        if(db=="letter"){
            vector<uint>searchResult;
            GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
            cout<<"size map="<<logicProcessor->mainLetterTableUni.size();
            
            /*if(logicProcessor->mainLetterTableUni.find(letter->name)==logicProcessor->mainLetterTableUni.end()){
             cout<<"no record "<<letter->name<<endl;
             letter->name+="@";
             }
             */
            string re="[\\p{Tibetan}]+";
            
            for(uint i=0;i<aliKali->size();i++){
                GLetter *letter=aliKali->getLetter(i);
                if(letter->name=="*"){letter->destroy();continue;}
                //подсчитываем площадь буквы покрытой масками
                //letter->drawLetterPict(1);
                //aliKali->saveLetter(letter);
                //if(letter->onOffRate<50&&letter->name!="*")searchResult.push_back(i);
                /*
                 int flag=0;
                 for(uint n=0;n<letter->mask32Count();n++){
                 //cout<<"y0="<<letter->mask32[n].yMask<<" y01="<<letter->letterH/2<<endl;
                 if(letter->mask32[n].mH>letter->letterH){
                 letter->mask32[n].mH=letter->letterH+9;
                 flag=1;
                 }
                 if(letter->mask32[n].yMask<-letter->letterH/2-5){
                 letter->mask32[n].yMask=-letter->letterH/2-5;
                 flag=1;                        
                 }
                 if(letter->mask32[n].yMask+letter->mask32[n].mH>letter->letterH/2+5){
                 letter->mask32[n].yMask=letter->letterH/2+5-letter->mask32[n].mH;
                 flag=1;
                 }
                 }
                 if(flag){
                 searchResult.push_back(i);
                 letter->reloadMask();
                 aliKali->saveLetter(letter);
                 }
                 */
                letter->reloadMask();
                aliKali->saveLetter(letter);
                
                //if(letter->name.find("@")!=-1)searchResult.push_back(i);
                /*if(logicProcessor->mainLetterTableUni.find(letter->name)==logicProcessor->mainLetterTableUni.end()){
                 if(rexExpFind((char*)letter->name.c_str(),re)){
                 //cout<<"no record "<<letter->name<<endl;
                 //letter->name="*";
                 //aliKali->saveLetter(letter);
                 searchResult.push_back(i);
                 }
                 }
                 */ 
                
                letter->destroy();
            }
            int mode=HTML;
            string result;
            
            multimap<int,int>list;
            for(uint i=0;i<searchResult.size();i++){
                GLetter *letter=aliKali->getLetter(searchResult[i]);
                list.insert(pair<int,int>(letter->onOffRate, searchResult[i]));
                letter->destroy();
            }
            searchResult.resize(0);
            map<int, int>::const_iterator itr;
            for(itr = list.begin(); itr != list.end(); ++itr){
                searchResult.push_back((*itr).second);
            }
            
            /*
             multimap<string,int>list;
             for(uint i=0;i<searchResult.size();i++){
             GLetter *letter=aliKali->getLetter(searchResult[i]);
             list.insert(pair<string,int>(letter->name, searchResult[i]));
             letter->destroy();
             }
             searchResult.resize(0);
             map<string, int>::const_iterator itr;
             for(itr = list.begin(); itr != list.end(); ++itr){
             searchResult.push_back((*itr).second);
             }
             */
            
            aliKali->drawOCRBasePict(searchResult,mode);
            aliKali->drawHTML(searchResult,result,mode);
            destString=result;
            if(result=="")destString="no records found";
            inputData.data["HTTPType"]="text/html";
        }
        
    }
    if(inputData.data["record"]=="import"){  //запрос на импорт данных в базу
        cout_<<"memory record import request"<<endl;
        
        if(inputData.data["db"]=="corpus"){
            string path=inputData.data["import"];
            path="/_CORPUS/kannada_corpus_2011_08_16/Kannada_line_Uni.txt";
            string str;
            int mode=0;
            
            if(!is_file(path)){
                destString="no valid file path for import";
                cout_<<index<<endl; return;
            }
            
            if(fileExt(path)=="txt")mode=READ_FROM_TXT;
            if(fileExt(path)=="tab")mode=READ_FROM_TAB;
            if(fileExt(path)=="xml")mode=READ_FROM_XML;
            
            vectorCorpus->import(path,mode);
            cout_<<"vectorDict->size()="<<vectorDict->size()<<"vectorCorpus->size()="<<vectorCorpus->size()<<endl;
            return;
        }    
        if(inputData.data["db"]=="dict"){
            string path=inputData.data["import"];
            string str;
            int mode=0;
            
            if(!is_file(path)){
                destString="no valid file path for import";
                cout_<<index<<endl; return;
            }
            
            if(fileExt(path)=="txt")mode=READ_FROM_TXT;
            if(fileExt(path)=="tab")mode=READ_FROM_TAB;
            if(fileExt(path)=="xml")mode=READ_FROM_XML;
            
            vectorDict->import(path,mode);
            cout_<<"vectorDict->size()="<<vectorDict->size()<<endl;
            return;
        } 
        if(inputData.data["db"]=="letter"){
            string path=inputData.data["OCRTables"]+"GFont.xml";
            cout_<<"@path="<<path<<endl;
            if(!is_file(path)){
                destString="no valid file path for import";
                cout_<<index<<endl; return;
            }
            aliKali->readGFontXML();
            //string cmd="mv "+path +inputData.data["OCRTables"]+"GFont1.xml";
            //run (cmd);
            
            return;
        }             
    }
    if(inputData.data["record"]=="export"){  //запрос на экспорт данных из базы
        destString="database record export request";
        cout_<<"export "<<inputData.data["db"]<<endl;
        if(inputData.data["db"]=="letter"){
            aliKali->writeGFontXML();
            return;
        }    
        string path=inputData.data["dictPath"]+"/TibetanGrammar1.tab";
        
        string str;
        ofstream c_out; c_out.open(path.c_str());
        if(!c_out){
            destString="no valid file path for export";
            cout_<<destString<<" "<<path<<endl; return;
        }
        
        if(db=="dict"){
            string strRec;
            for(uint i=0;i<vectorDict->size();i++){
                TString st; vectorDict->getTStr(i,&st);
                strRec=st[0]+"\t"+st[1]+"\t"+st[2]+"\t"+st[3]+"\t"+st[4]+"\t";
                c_out<<str_replace("\n","•",strRec)<<"\n";
            }
        }
        c_out.close();
    }    
    if(inputData.data["record"]=="export_edited"){  //запрос на экспорт данных из базы
        destString="database record edited export request";
        string path=inputData.data["dictPath"]+"/TibetanGrammar_edited.tab";
        
        string str;
        ofstream c_out; c_out.open(path.c_str());
        if(!c_out){
            destString="no valid file path for export";
            cout_<<destString<<" "<<path<<endl; return;
        }
        
        if(inputData.data["db"]=="letter"){
            destString=((GFontEditor*)(inputData.fontEditor))->exportGFontsDB(path);
            return;
        }
        
        if(db=="dict"){
            string strRec;
            for(uint i=0;i<vectorDict->size();i++){
                TString st; vectorDict->getTStr(i,&st);
                if(st[3].find("@")==-1)continue;
                strRec=st[0]+"\t"+st[1]+"\t"+st[2]+"\t"+st[3]+"\t"+st[4]+"\t";
                c_out<<str_replace("\n","•",strRec)<<"\n";
            }
        }
        c_out.close();
    }
    if(inputData.data["record"]=="save"){  //запрос на сохранение записи
        destString="<script>location.href=\"ocr.php?db="+inputData.data["db"]+"&record=find&c1="+inputData.data["c0"]+"\"</script>";
        string dataStr;
        if(inputData.data["c5"]==""){
            destString="save error. no valid data";
            cout_<<destString;
            return;
        }else{
            dataStr="\tc0\t"+inputData.data["c0"]+"\tc1\t"+inputData.data["c1"]+"\tc2\t"+inputData.data["c2"]+"\tc3\t"+inputData.data["c3"]+"\tc4\t"+inputData.data["c4"]+"\tc5\t"+inputData.data["c5"];
            cout_<<"IndStr="<<inputData.data["c5"]<<"/ destStr="<<dataStr<<"/\n";             
            TString st;
            
            st.push_back(inputData.data["c0"]); st.push_back(inputData.data["c1"]); st.push_back(inputData.data["c2"]); 
            st.push_back(inputData.data["c3"]); st.push_back(inputData.data["c4"]); st.push_back(inputData.data["c5"]);
            uint index=atoi(inputData.data["c5"].c_str());
            if(inputData.data["db"]=="corpus"){
                vectorCorpus->putTStr(index,&st);
                return;
            }    
            if(inputData.data["db"]=="dict"){
                if(index>=vectorDict->size()){
                    vectorDict->push_back(&st);
                }else{
                    vectorDict->putTStr(index,&st);
                }
                return;
            } 
            
        }    
        
    }
    if(inputData.data["record"]=="copy"){  //запрос на сохранение записи
        destString="<script>location.href=\"ocr.php?"+inputData.data["db"]+"&record=find&c1="+inputData.data["c0"]+"\"</script>";
        string dataStr;
        if(inputData.data["c5"]==""){
            destString="save error. no valid data";
            cout_<<destString;
            return;
        }else{
            dataStr="\tc0\t"+inputData.data["c0"]+"\tc1\t"+inputData.data["c1"]+"\tc2\t"+inputData.data["c2"]+"\tc3\t"+inputData.data["c3"]+"\tc4\t"+inputData.data["c4"]+"\tc5\t"+inputData.data["c5"];
            cout_<<"IndStr="<<inputData.data["c5"]<<"/ destStr="<<dataStr<<"/\n";
            TString st;
            
            st.push_back(inputData.data["c0"]); st.push_back(inputData.data["c1"]); st.push_back(inputData.data["c2"]);
            st.push_back(inputData.data["c3"]); st.push_back(inputData.data["c4"]);
            vectorDict->push_back(&st);
            return;
        }
        
    }
    if(inputData.data["record"]=="add"){  //запрос на добавление записи
    }
    if(inputData.data["record"]=="delete"){  //запрос на удаление записи
        /*        
         if(inputData.data["id"]=="null"){
         int id=tctdbgenuid(tdb)-1;
         char buf[20];
         sprintf(buf,"%d",id);
         if(!tctdbout2(tdb, buf)){
         ecode = tctdbecode(tdb);
         fprintf(stderr, "put error: %s\n", tctdberrmsg(ecode));
         }else{   
         destString="record deleted";
         }    
         cout_<<"id="<<id;
         }else{
         
         if(!tctdbout2(tdb, inputData.data["id"].c_str())){
         ecode = tctdbecode(tdb);
         fprintf(stderr, "put error: %s\n", tctdberrmsg(ecode));
         }    
         destString="record deleted";
         }
         cout_<<destString<<tdb->hdb->rnum; 
         */
    }
    
    
}