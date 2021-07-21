#include "GLogicProcessor.h"
#include "../GMemory/GMemory.h"
#include <regex>

static bool sortDictKeyL(const dictKey& d1, const dictKey& d2){
    return d1.lenght > d2.lenght;
}

struct textK{
    string key;
    vector<uint>id;
    int status;
};


inline string linkDict(string &line,int *id){
    string s="";
    ostringstream out;
    out<<"<tb id=\"s"<<*id<<"\" onClick=\"set(\'s"<<*id<<"\')\">"+line+"</tb>";
    *id+=1;
    s=out.str();
    return s;
}

inline string linkText(string &line,int *id){
    string s="";
    ostringstream out;
    out<<"<p class=\"tib\" id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\">"<<line<<"</p>";
    *id+=1;
    s=out.str();
    return s;
}
inline string linkOriginal(string &line,int *id){
    string s="";
    ostringstream out;
    out<<"<p class=\"tib\" id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\">"<<line<<"</p>";
    *id+=1;
    s=out.str();
    return s;
}

inline string linkEdit(string &line,int *id){
    string s="";
    ostringstream out;
    if(line.find("=_")==-1&&(line.find("@")!=-1||line.find("*")!=-1||line=="=g.p"||line=="=d.l."||line=="=")){
        if(line.find("_@")!=-1||line.find("__")!=-1||line.find("@@")!=-1){
            if(line.find("__")||line.find("@@")!=-1){
               out<<"<tt id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\" >"<<line<<"</tt>";
            }else{
               out<<"<t id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\">"<<line<<"</t>";
            }
        }else{
            out<<"<t id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\" >"<<line<<"</t>";
        }
    }else{
        out<<"<t id=\"s"<<*id<<"\" onClick=\"edit(\'s"<<*id<<"\')\" ><r>"<<line<<"<r></t>";
    }
    *id+=1;
    s=out.str();
    return s;
}
inline string linkDict(const char* line,int *id){
    string c=line;
    return linkDict(c,id);
}
inline string linkEdit(const char* line,int *id){
    string c=line;
    return linkEdit(c,id);
}

/** @brief read text file or text bufer and translate one page. Write result in destination string or disk file*/
void  GLogicProcessor::mainTextTranslation(string &textPage){
    
    
    TString st;
    //int rowsNum=100;
    if(inputData.data["page"]!="FULL_TEXT"){
        int pageNum=atoi(inputData.data["page"].c_str());
        //cout<<"pageNum="<<pageNum<<endl;
        
        string textData;
        int pageTextSize=24000;
        int pageCount=0;
        
        if(inputData.data["zip"]==""){
            textData=readText(inputData.data["text"]);
        }else{
            //извлекаем данные из Zip
            string text=inputData.data["text"];
            string cmd="unzip -p \""+inputData.data["zip"]+"\" \""+text+"\"";
            //cout<<cmd<<endl; exit(0);
            textData=run(cmd);
        }
        pageCount=(int)textData.size()/pageTextSize;
        cout<<pageCount<<endl;    //нужно для навигации на HTML странице
        if(pageNum>pageCount)pageNum=0;
        textPage=substr(pageNum*pageTextSize, pageTextSize, textData);
        int i=0; int in;
        
        //add start and end of page phrases/ 0x8D -start of any tibetan letter in UTF-8
        while(1){
            in=pageNum*pageTextSize+pageTextSize+i;
            if(in>=textData.size()-2||i>pageTextSize)break;
            if(textData[in]!='<'&&textData[in]!=' '&&textData[in+2]!=(char)0x8D){
                textPage+=textData[in];
                i++;
            }else{
                break;
            }
        }
        i=1;
        while(1){
            in=pageNum*pageTextSize-i;
            if(in<=0||i>pageTextSize)break;
            if(textData[in]!='<'&&textData[in]!=' '&&textData[in]!=(char)0x8D){
                textPage=textData[in]+textPage;
                i++;
            }else{
                break;
            }
        }
        //print(textPage); exit(0);
        //textPage=str_replace(" ", "\n",textPage);
        textPage=str_replace("\r", "\n",textPage);
        //TIME_START
        lineTextTranslation(textPage,0);
        //TIME_PRINT_
    }else{
        if(textPage==""){
            if(inputData.data["zip"]==""){
                textPage=readText(inputData.data["text"]);
            }else{
                //извлекаем данные из Zip
                string text=inputData.data["text"];
                string cmd="unzip -p \""+inputData.data["zip"]+"\" \""+text+"\"";
                //cout<<cmd<<endl; exit(0);
                textPage=run(cmd);
            }
        }
        textPage=str_replace(" ", "\n",textPage);
        textPage=str_replace("\r", "\n",textPage);
        lineTextTranslation(textPage,0);
    }
    
    //cout_<<" destString="<<destString;
    return;
    
}

void GLogicProcessor::writeDictReportLocal(string &srcStr){
    
    int id=1;
    //int print=1;
    string str,dL,line;
    vector<dictKey> keyArray;
    ostringstream out;
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    GMap *dict;
    GVector *dt;
    int translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    GVector *dk;
    GMap *d;
    string ln=inputData.data["ln"];
    if(ln=="")ln="rus";
    if(ln=="rus")translationMode=TRANSLATE_RUS;
    if(ln=="eng")translationMode=TRANSLATE_ENG;

    if(translationMode==TRANSLATE_ENG){
        str="translationDictEng";
    }else{
        str="translationDictRus";
    }
    longMemory->loadTable(str);
    dk=longMemory->table[str].data;
    indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    d=indexRec->mIndex;
    
    str="mainDict";
    longMemory->loadTable(str);
    indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    dt=longMemory->table[str].data;
    dict=indexRec->mIndex;
    
    vector<ulong>searchResult;
    //d->getKey(srcStr, searchResult,HASH_FIND);
    d->getHKey(searchResult,srcStr,0);
    for(int m=0;m<searchResult.size();m++) {
        TString st;
        dk->getTStr(searchResult[m], st);
        if(st.size()){
            str="="+st[1];
            str=str_replace(":|:YP","",str);
            out<<"["<<linkDict(st[0].c_str(),&id)<<linkEdit(str,&id)<<"]<hr>\n"<<endl;
        }
    }
    
    searchResult.resize(0);
    inputData.start=1;
    dict->getHKey(searchResult,srcStr,0);
    //dict->getKey(srcStr, searchResult,HASH_FIND);
    //cout<<"searchResult="<<searchResult.size()<<endl;
    vector<string>report;
    report.resize(35);
    int flagTib=0;
    
    for(int m=0;m<searchResult.size();m++) {
        TString st;
        dt->getTStr(searchResult[m], st);
        str=st[2];
        str=str_replace("-","",str);
        str=str_replace("+","",str);
        line=st[1];
        if(str.find("[GRM]")!=-1){
            report[0]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[MG]")!=-1){
            report[1]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[HP]")!=-1){
            report[2]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[TD]")!=-1){
            report[3]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[LD]")!=-1){
            report[4]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[MV]")!=-1){
            report[5]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[VD]")!=-1){
            report[6]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[EP]")!=-1){
            report[7]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[RE]")!=-1){
            report[8]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[SD]")!=-1){
            report[9]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[IW]")!=-1){
            report[10]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[JW]")!=-1){
            report[11]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        
        if(str.find("[TT]")!=-1){
            if(flagTib==0){
                lineTextTranslation(line,0);
                report[12]=line+"<div class=\"d_mark\">\n"+str+"</div>";
                flagTib=1;
            }
            continue;
        }
       if(str.find("[DK]")!=-1){
            lineTextTranslation(line,0);
            report[13]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        if(str.find("[DN]")!=-1){
            //lineTextTranslation(line,0);
            report[14]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            flagTib=1;
            continue;
        }
        if(str.find("[BB]")!=-1){
            if(flagTib==0){
                lineTextTranslation(line,0);
                report[15]=line+"<div class=\"d_mark\">\n"+str+"</div>";
                flagTib=1;
            }
            continue;
            
        }
        if(str.find("[TS]")!=-1){
            if(flagTib==0){
                lineTextTranslation(line,0);
                report[16]=line+"<div class=\"d_mark\">\n"+str+"</div>";
                flagTib=1;
            }
            continue;
        }
        if(str.find("[TR]")!=-1){
            report[17]=line+"<div class=\"d_mark\">\n"+str+"</div>";
            continue;
        }
        //cout<<"@@@"<<str<<endl;
        str=line+"<div class=\"d_mark\">\n"+str+"</div>";
        report.push_back(str);
        
    }
    for(int m=0;m<report.size();m++) {
        if(report[m].size())
            out<<report[m]<<"<hr>\n"<<endl;
    }
    
    inputData.start=0;
    srcStr=out.str();
    //print(srcStr);
    
}//_____________________________________________________________________________


/** @brief translate text lines and write result in destString*/
void GLogicProcessor::lineTextTranslation(string &destString,int mode_){
    //return;
    string textLn="tib";
    //test language
    if(destString.find("་")!=-1){
        textLn="tib";
    }else{
        
        wstring str=UTF_to_Unicode(destString);
        for(int i=0;i<25&&i<str.size();i++){
            //cout<<hex<<str[i]<<" ";
            if(str[i]>0x3400&&str[i]<0xA000){
                lineTextTranslationChn(destString);
                return;
            }
        }
                                   
        
        lineTextTranslationSkt(destString);
        return;
    }
    
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    int translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    string ln=inputData.data["ln"];
    if(inputData.data["ocrData"]=="TibetanUTFToRus")ln="rus";
    if(inputData.data["ocrData"]=="TibetanUTFToEng")ln="eng";
    if(ln!=""){
        if(ln=="rus")translationMode=TRANSLATE_RUS;
        if(ln=="eng")translationMode=TRANSLATE_ENG;
    }
    
    GVector *dk;
    GMap *d;
    string str;
    
    if(textLn=="tib"){
        if(translationMode==TRANSLATE_ENG){
            str="translationDictEng";
        }else{
            str="translationDictRus";
        }
    }else{
        if(translationMode==TRANSLATE_ENG){
            str="translationSktDictEng";
        }else{
            str="translationSktDictRus";
        }
    }
    
    longMemory->loadTable(str);
    dk=longMemory->table[str].data;
    indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    d=indexRec->mIndex;
    
    str="mainDict";
    longMemory->loadTable(str);
    indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    GMap *dictMap=indexRec->mIndex;
    
    
    str="TranslationDictTexts";
    longMemory->loadTable(str);
    indexRec=longMemory->createIndex(longMemory->table[str], 0, TEXT_DICT_SEARCH);
    GMap *textKey=indexRec->mIndex;
    
    string src,line,ld,l1,c,c1,c2,report,res,resD;
    
    int i,j,start,end,lng,index;
    int mode=inputData.OCRMode;
    
    vector<string>text;
    
    destString=str_replace(" ", "\n", destString);
    destString=str_replace(" ", "\n", destString);  //no brackeble space
    explode("\n",destString,text);
    
    int id=1;
    //text.resize(1); text[0]="གང་ཟག་ཏུ་ལྟ་(༦)བའམ།";
    //line="རྣམས་";
    //TString st;
    //dk->getTStr(109073,st);
    //cout<<"st="<<st.size()<<" st[0]="<<st[0]<<endl;
    //exit(0);
    //index=d->getHKey(line);
    //cout<<"index="<<index<<endl;
    //exit(0);
    //cout<<"text.size()="<<text.size()<<endl;
    
    //dictMap->hashError=0;
    //dictMap->maxHashError=0;
    //dictMap->hashCount=0;
    
    report="";
    //int step=0;
    
    
    for(int lineIndex=0;lineIndex<text.size();lineIndex++){
        //if(step==1000){print(lineIndex<<" from "<<text.size()); step=0;} step++;
        str=text[lineIndex];
        //cout<<"str="<<str<<endl;
        src=str;
        lng=(int)str.size();
        if(lng<2)continue;
        if(str.find("་")==-1){
            report+=src+"\n<br>";
            continue;
        }
        //continue;
        clearText(str);
        
        
        //str=str_replace("[ _\d\ "	\"\*\(\)\{\}\[\]@//\%\&༄༅༔༴༡༢༣༤༥༦༧༨༩༠༎།༑༈༌༐༼༽ऀ-ॿ]","་",str)
        str=str_replace("ཿ","་ཿ་",str);
        str=str_replace("￨","",str);
        str+="་";
        //str=str+"་།";
        str=str_replace("༌","་",str); //0F0C -> 0F0B
        str=str_replace("་་","་",str);
        //cout<<str;
        //str=str_replace("([^་])འོ་།","\1་(точка)་",str);
        //str=str_replace("([^་])འམ་།","\1་[འམ=или]་",str);
        /*
        str=str_replace("ག་གོ་།","ག་(¶)་",str);
        str=str_replace("ང་ངོ་།","ང་(¶)་",str);
        str=str_replace("ད་དོ་།","ད་(¶)་",str);
        str=str_replace("ན་ནོ་།","ན་(¶)་",str);
        str=str_replace("བ་བོ་།","བ་(¶)་",str);
        str=str_replace("མ་མོ་།","མ་(¶)་",str);
        str=str_replace("ར་རོ་།","ར་(¶)་",str);
        str=str_replace("ལ་ལོ་།","ལ་(¶)་",str);
        str=str_replace("ས་སོ་།","ས་(¶)་",str);
        str=str_replace("་ཏོ་།","་(¶)་",str);
        */
        //str=str_replace("་པའང་","་པ་[འང=уступ.]་",str);
        //str=str_replace("་བའང་","་བ་[འང=уступ.]་",str);
        
        ///проверяем все ответы текстового словаря по этой строке
        vector<dictKey>dictS;
        vector<textK>dictK;

        if(mode_==3){
            if(str.size()>10){
                string str_;
                textKey->getTextKey(str,dictS, ANY_MATCH);
                sort(dictS.begin(),dictS.end(),sortDictKeyL);
                str_="";
                for(int i=0;i<dictS.size();i++){
                    int s=(int)dictS[i].key.find("་")+3;
                    int e=(int)dictS[i].key.rfind("་");
                    dictS[i].key=dictS[i].key.substr(s,e-s);
                }
                
                for(int i=0;i<dictS.size();i++){
                    if(str_!=dictS[i].key){
                        //проверяем есть ли такой ключ
                        if(dictK.size()&&dictK[dictK.size()-1].key.find(dictS[i].key)!=-1)continue;
                        textK k;
                        str_=dictS[i].key;
                        k.key=str_;
                        k.status=1;
                        dictK.push_back(k);
                        dictK[dictK.size()-1].id.push_back(dictS[i].id);
                    }else{
                        dictK[dictK.size()-1].id.push_back(dictS[i].id);
                    }
                }
                //for(int i=0;i<dictK.size();i++){cout<<dictK[i].key<<" "<<dictK[i].id.size()<<endl; }
            }
        }
        //cout<<str;
        
        vector<string>l;
        explode("་",str,l);
        l.pop_back(); //remove last delimeter
        res="";
        resD="";
        
        lng=(int)l.size();
        start=0;
        end=lng-1;
        
        i=lng;
        
        //continue;
        
        while(start<lng){
            //make query string decrease end
            end=lng;
            while(end>-1){
                j=start;
                line="";
                //increase start
                while (j < end){
                    line+=l[j]+"་";
                    j++;
                }
                //count+=1
                //cout<<line<<" "<<start<<" "<<end<<endl;
                //if (count >120):
                //return
                index=(int)d->getHKey_(line,0);
                
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(line,&id)+linkEdit(c,&id)+"]་";
                    
                    if(mode_==3){
                        //проверяем есть ли такая фраза в текстовом словаре.
                        for(int i=0;i<dictK.size();i++){
                            if(!dictK[i].status)continue;
                            if(line.find(dictK[i].key)!=-1){
                                dictK[i].status=0;
                                //cout<<dictK[i].key<<" "<<dictK[i].id.size()<<endl;
                            }
                        }
                    }
                    
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }

                
                //next check big dictionary report
                index=(int)dictMap->getHKey_(line,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(line,&id)+linkEdit("=",&id)+"] ";
                }
                ln=line+"@";
                
                l1=str_replace("འི་@","་",ln);
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་["+linkDict("-འི་",&id)+linkEdit("=g.p",&id)+"]་";
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(l1,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                }
                
                l1=str_replace("ས་@","་",ln);
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་["+linkDict("-ས་",&id)+linkEdit("=i.p.",&id)+"]་";
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(l1,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                }
                
                l1=str_replace("ར་@","་",ln);
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་["+linkDict("-ར་",&id)+linkDict("=d.l.",&id)+"]་";
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(l1,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                }
                
                l1=str_replace("འོ་@","་",ln);
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་["+linkDict("-འོ་",&id)+linkEdit("=(точка)",&id)+"]་";
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(l1,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(line,&id)+linkEdit("=",&id)+"] ";
                }
                
                l1=str_replace("འམ་@","་",ln);
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་["+linkDict("-འམ་",&id)+linkEdit("=или",&id)+"]་";
                    if(c.find("__")!=-1||c.find("@@")!=-1||mode_==FULL_REPORT){
                        end-=1;
                        res+="<br>\n";
                        if(end==start){
                            break;
                        }
                        if(mode_==FULL_REPORT)continue;
                    }else{
                        start=end-1;
                        break;
                    }
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(l1,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                }
                
                
                end-=1;
                if(end==start){
                    res+=line;
                    break;
                }
            }
            start++;
        }
        res=str_replace(":|:YP","",res);
        res=str_replace("*","",res);
        res=str_replace("@","",res);
        if(mode_!=FULL_REPORT){
            report+=linkOriginal(src,&id)+"\n@<w>"+res+"</w>\n";
        }else{
            report+="@<w>"+res+"</w>\n";
        }
        
        if(resD.size()>10){
            report+="<w>"+resD+"</w><br>\n";
        }else{
            report+="<br>\n";
        }
        
        if(mode_==3){
            //добавляем все ответы текстового словаря
            ostringstream out;
            for(int i=0;i<dictK.size();i++){
                if(!dictK[i].status)continue;
                out<<dictK[i].key<<" "<<dictK[i].id.size()<<"<br/>"<<endl;
                
            }
            report+=out.str();
        }
    
        
        if(mode==FULL_REPORT){
            res="";
            for (int t=0;t<l.size();t++){
                line=l[t];
                l1=line+"་";
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་";
                }
            }
            res=str_replace(":|:YP","",res);
            report+="<w>"+res+"</w>\n<br>\n";
        }
        //break;
        
    }
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br> ", "<br>",report);
    report=str_replace("\n ", "\n",report);
    report=str_replace(" \n", "\n",report);
    report=str_replace("<br>\n།\n", "།<br>\n",report);
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br>\n<p", "\n<p",report);
    
    //print("@@@@"<<report);
    destString=report;
    
    //cout<<"    dictMap->hashError="<<dictMap->hashError<<" dictMap->maxHashError="<<dictMap->maxHashError<<" dictMap->hashCount="<<dictMap->hashCount<<endl;
    //cout<<"maxHashSize="<<(0xffffffff>>7)<<endl;
    //exit(0);
    
}


/** @brief translate text lines and write result in destString*/
void GLogicProcessor::lineTextTranslationChn(string &destString){
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    int translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    string ln=inputData.data["ln"];
    if(ln!=""){
        if(ln=="rus")translationMode=TRANSLATE_RUS;
        if(ln=="eng")translationMode=TRANSLATE_ENG;
    }
    GVector *dk;
    GMap *d;
    string str;
    
    if(translationMode==TRANSLATE_ENG){
        str="translationChnDictEng";
    }else{
        str="translationChnDictRus";
    }
    
    longMemory->loadTable(str);
    dk=longMemory->table[str].data;
    indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    d=indexRec->mIndex;
    
    str="mainDict";
    longMemory->loadTable(str);
    indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    GMap *dictMap=indexRec->mIndex;
    
    
    string src,line,ld,l1,c,c1,c2,report,res,resD;
    int i,j,start,end,lng,index;
    int mode=inputData.OCRMode;
    
    //clearTextSkt(destString);
    
    vector<string>text;
    explode("\n",destString,text);
    
    int id=1;
    
    report="";
    
    for(int lineIndex=0;lineIndex<text.size();lineIndex++){
        //if(lineIndex%1000==0){
        //    print(lineIndex<<" from "<<text.size());
        //}
        str=text[lineIndex];
        //cout<<"str="<<str<<endl;
        src=str;
        lng=(int)str.size();
        if(lng<2)continue;
        //if(str.find("а")!=-1||str.find("е")!=-1||str.find("и")!=-1||str.find("о")!=-1||str.find("у")!=-1){
        //    report+=src+"\n<br>";
        //    continue;
        //}
        str=str_replace(" ","",str);
        string l=str;
        res="";
        resD="";
        
        lng=(int)l.size();
        start=0;
        end=lng-1;
        
        i=lng;
        
        while(start<lng){
            //make query string decrease end
            end=lng;
            while(end>-1){
                j=start;
                line="";
                //increase start
                int index=0;
                while (j < end&&index<256){
                    line+=l[j];
                    j++;
                    index++;
                }
                //count+=1
                //cout<<line<<" "<<start<<" "<<end<<endl;
                //if (count >120):
                //return
                index=(int)d->getHKey_(line,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(line,&id)+linkEdit(c,&id)+"] ";
                    if(c.find("__")!=-1){
                        end-=1;
                        res+="<br>\n";
                        continue;
                    }
                    if(mode==FULL_REPORT && start==0 && end==lng-1){
                        end-=1;
                        continue;
                    }
                    //проверяем есть ли окончание этого слова
                    l1=line+"ṃ";
                    if(l.find(l1,start)==start){
                        start=end-1+3;
                        break;
                    }
                    
                    start=end-1;
                    break;
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(line,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(line,&id)+linkEdit("=",&id)+"] ";
                }
                
                if(line.size()>3){
                    l1=line+"a";
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
                        
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    l1=line+"u";
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
                        
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    
                    l1="a"+line;
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
                        
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    l1="aṃ"+line;
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
                        
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    
                }
                
                
                end-=1;
                if(end==start){
                    res+=line;
                    break;
                }
            }
            start++;
        }
        res=str_replace(":|:YP","",res);
        res=str_replace("*","",res);
        res=str_replace("@","",res);
        report+=linkOriginal(src,&id)+"\n@<w>"+res+"</w>\n";
        if(resD.size()>10){
            report+="<w>"+resD+"</w><br>\n";
        }else{
            report+="<br>\n";
        }
        
        if(mode==FULL_REPORT){
            res="";
            for (int t=0;t<l.size();t++){
                line=l[t];
                l1=line+" ";
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་";
                }
            }
            res=str_replace(":|:YP","",res);
            report+="<w>"+res+"</w>\n<br>\n";
        }
        //break;
        
    }
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br> ", "<br>",report);
    report=str_replace("\n ", "\n",report);
    report=str_replace(" \n", "\n",report);
    report=str_replace("<br>\n།\n", "།<br>\n",report);
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br>\n<p", "\n<p",report);
    //print("@@@@"<<report);
    destString=report;
    
    //cout<<"    dictMap->hashError="<<dictMap->hashError<<" dictMap->maxHashError="<<dictMap->maxHashError<<" dictMap->hashCount="<<dictKey->hashCount<<endl;
    //cout<<"maxHashSize="<<(0xffffffff>>7)<<endl;
    //exit(0);

}

void GLogicProcessor::lineTextTranslationSkt(string &destString){
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    int translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    string ln=inputData.data["ln"];
    if(ln!=""){
        if(ln=="rus")translationMode=TRANSLATE_RUS;
        if(ln=="eng")translationMode=TRANSLATE_ENG;
    }
    GVector *dk;
    GMap *d;
    string str;
    
    if(translationMode==TRANSLATE_ENG){
        str="translationSktDictEng";
    }else{
        str="translationSktDictRus";
    }

    longMemory->loadTable(str);
    dk=longMemory->table[str].data;
    indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    d=indexRec->mIndex;
    
    str="mainDict";
    longMemory->loadTable(str);
    indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
    GMap *dictMap=indexRec->mIndex;
    
    
    string src,line,ld,l1,c,c1,c2,report,res,resD;
    int i,j,start,end,lng,index;
    int mode=inputData.OCRMode;
    
    clearTextSkt(destString);
    
    vector<string>text;
    explode("\n",destString,text);
    
    int id=1;
    
    report="";
    
    for(int lineIndex=0;lineIndex<text.size();lineIndex++){
        //if(lineIndex%1000==0){
        //    print(lineIndex<<" from "<<text.size());
        //}
        str=text[lineIndex];
        //cout<<"str="<<str<<endl;
        src=str;
        lng=(int)str.size();
        if(lng<2)continue;
        if(str.find("а")!=-1||str.find("е")!=-1||str.find("и")!=-1||str.find("о")!=-1||str.find("у")!=-1){
            report+=src+"\n<br>";
            continue;
        }
        str=str_replace(" ","",str);
        string l=str;
        res="";
        resD="";
        
        lng=(int)l.size();
        start=0;
        end=lng-1;
        
        i=lng;
        
        while(start<lng){
            //make query string decrease end
            end=lng;
            while(end>-1){
                j=start;
                line="";
                //increase start
                int index=0;
                while (j < end&&index<256){
                    line+=l[j];
                    j++;
                    index++;
                }
                //count+=1
                //cout<<line<<" "<<start<<" "<<end<<endl;
                //if (count >120):
                //return
                index=(int)d->getHKey_(line,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(line,&id)+linkEdit(c,&id)+"] ";
                    if(c.find("__")!=-1){
                        end-=1;
                        res+="<br>\n";
                        continue;
                    }
                    if(mode==FULL_REPORT && start==0 && end==lng-1){
                        end-=1;
                        continue;
                    }
                    //проверяем есть ли окончание этого слова
                    l1=line+"ṃ";
                    if(l.find(l1,start)==start){
                        start=end-1+3;
                        break;
                    }
 
                    start=end-1;
                    break;
                }
                //next check big dictionary report
                index=(int)dictMap->getHKey_(line,0);
                if(line.size()>3 && index!=-1){
                    resD+="["+linkDict(line,&id)+linkEdit("=",&id)+"] ";
                }
                
                if(line.size()>3){
                    l1=line+"a";
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }

                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    l1=line+"u";
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
 
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    
                    l1="a"+line;
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }

                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }
                    l1="aṃ"+line;
                    index=(int)d->getHKey_(l1,0);
                    if(index!=-1){
                        TString st;
                        dk->getTStr(index,st);
                        c="="+st[1];
                        res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"] ";
                        //проверяем есть ли окончание этого слова
                        l1=line+"ṃ";
                        if(l.find(l1,start)==start){
                            start=end-1+3;
                            break;
                        }
   
                        start=end-1;
                        break;
                    }
                    //next check big dictionary report
                    index=(int)dictMap->getHKey_(l1,0);
                    if(line.size()>3 && index!=-1){
                        resD+="["+linkDict(l1,&id)+linkEdit("=",&id)+"] ";
                    }

                }

                
                end-=1;
                if(end==start){
                    res+=line;
                    break;
                }
            }
            start++;
        }
        res=str_replace(":|:YP","",res);
        res=str_replace("*","",res);
        res=str_replace("@","",res);
        report+=linkOriginal(src,&id)+"\n@<w>"+res+"</w>\n";
        if(resD.size()>10){
            report+="<w>"+resD+"</w><br>\n";
        }else{
            report+="<br>\n";
        }
        
        if(mode==FULL_REPORT){
            res="";
            for (int t=0;t<l.size();t++){
                line=l[t];
                l1=line+" ";
                index=(int)d->getHKey_(l1,0);
                if(index!=-1){
                    TString st;
                    dk->getTStr(index,st);
                    c="="+st[1];
                    res+="["+linkDict(l1,&id)+linkEdit(c,&id)+"]་";
                }
            }
            res=str_replace(":|:YP","",res);
            report+="<w>"+res+"</w>\n<br>\n";
        }
        //break;
        
    }
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br> ", "<br>",report);
    report=str_replace("\n ", "\n",report);
    report=str_replace(" \n", "\n",report);
    report=str_replace("<br>\n།\n", "།<br>\n",report);
    report=str_replace("<br>\n<br>", "<br>\n",report);
    report=str_replace("<br>\n<p", "\n<p",report);
    //print("@@@@"<<report);
    destString=report;
    
    //cout<<"    dictMap->hashError="<<dictMap->hashError<<" dictMap->maxHashError="<<dictMap->maxHashError<<" dictMap->hashCount="<<dictKey->hashCount<<endl;
    //cout<<"maxHashSize="<<(0xffffffff>>7)<<endl;
    //exit(0);
}


void GLogicProcessor::saveToTranslationDictionary(string &srcStr){
    vector<string>text;
    explode("[",srcStr,text);
    int index;
    string key,value;
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    int translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    GVector *dv;
    GMap *d;
    if(translationMode==TRANSLATE_ENG){
        dv=longMemory->translationDictEng;
        d=longMemory->translationDictGMapEng;
    }else{
        dv=longMemory->translationDictRus;
        d=longMemory->translationDictGMapRus;
    }
    //GMap *dictMap=longMemory->dictionaryGMap;
    
    for(int i=0;i<text.size();i++){
        string line=text[i];
        string reStr="<[^>]*>";
        std::regex key_regex(reStr);
        line = std::regex_replace(line, key_regex, "");
        reStr=".*=.*";
        std::regex key_regex_(reStr);
        line = std::regex_replace(line, key_regex_, "");
        if(line.size()<3)continue;
        index=(int)line.find("/");
        if(index<3)continue;
        index=(int)line.find("]");
        if(index<3)continue;
        line=substr(0, index, line);
        line=str_replace("/_", "/", line);
        vector<string>data;
        explode("/",line,data);
        if(data.size()<2)continue;
        key=data[0];
        value=data[1];
        key=key+"་";
        key=str_replace(" ","",key);
        key=str_replace("་་","་",key);
        value=str_replace("__","#",value);
        value=str_replace("_@","@",value);
        value=str_replace("#","__",value);
        value+="@";
        value=str_replace("@@","@",value);
        value=str_replace("{","༼",value);
        value=str_replace("«","༼",value);
        value=str_replace("}","༽",value);
        value=str_replace("»","༽",value);
        value=str_replace("༼༼","༼",value);
        value=str_replace("༽༽","༽",value);
        if(value.size()<3)continue;
        //cout<<"key="<<key<<" value="<<value<<endl;
        index=(uint)d->getHKey(key,0);
        //cout<<"index="<<index;
        TString st;
        if(value!="---"){
            st+=key;
            st+=value;
        }else{
            st+="";
            st+="";
        }
        if(index>-1){
            dv->putTStr(index, st);
        }else{
            d->addRecord(st);
        }
        
    }
    
}

