





    //cout<<fontEditor->aliKali->size();
    
    //fontEditor->aliKali->resize(21911);
    //fontEditor->buildGFontFromFont();
    //fontEditor->rebuildGFont(); exit(0);
    //fontEditor->setScale(); exit(0);


    //l="བདག་ཉིད་ཆེན་པོ་གཙང་པ་རྒྱ་རས་པའི་ཐུགས་དམ་དགེ་སྦྱོར་བདུན་པའི་གསུང་སྒྲོས་གྲུབ་པའི་དབང་ཕྱུག་འབྲས་མོ་བ་སངས་རྒྱས་འབུམ་ཞེས་གྲགས་པའི་དགེ་བའི་བཤེས་གཉེན་དེས་མཛད་པའི་གཞུང་འགྲེལ་རྫོགས་ས";
    //string ret="0"; ret[0]=11;
    //l=str_replace(ret.c_str(),"\n", l);
    //cout<<l;


/*find entry
 
 str="DHARMABOOK";
 //str="translationDictRus";
 longMemory->loadTable(str);
 GVector *v=longMemory->table[str].data;
 string ret="0"; ret[0]=11;
 cout<<v->size();
 for(int i=0;i<v->size();i++){
     if(i%100==0)cout<<" "<<i;
     TString st;
     v->getTStr(i,st);
     string key=st[1];
     int n=key.find("སམ་སིདྡྷི");
     if(n!=-1){
         n-=100;
         if(n<0)n=0;
         string s=substr(n,256,key);
         cout<<" i:"<<i<<" key:"<<s<<endl;
     }
 }
 exit(0);
*/


//find entry
 
//str="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG_1024_9_data.index";
//string str_="/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/DHARMABOOK.bin";
/*
GVector *v_=GVector::create(str_);

TString st;

v_->getTStrData(19969,st);

cout<<st[0]<<" "<<st[1]<<endl;

str=st[1];

if(str.find("Tenjur_Derge")!=-1){
    //cout<<"Match !"<<str.find("Tenjur_Derge");
    str="[[Category:Tengyur_Derge]]\n"+str;
    cout<<str;
}else{
    cout<<"Not Match!";
}
*/



//exit(0);


//string str_="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG_1024_6_data.index";
/*
rLink l;
l.index=128;
l.field=512;
l.offset=1024;

string s;
s.resize(sizeof(rLink));
memcpy(&s[0],&l,sizeof(rLink));

rLink *rr=(rLink*)&s[0];
cout<<" s:"<<s.size()<<" i:"<<rr->index<<" "<<rr->field<<" "<<rr->offset<<endl;

cout<<"";
//string str__="";
 //str="translationDictRus";
//longMemory->loadTable(str);
//GVector *v=GVector::create(str);



for(int i=0;i<100;i++){
    
    v_->push_back((char*)rr,sizeof(rLink));
    string st;
    v_->getStr(i,st);
    rLink *l=(rLink*)&st[0];
    cout<<i<<"s:"<<st.size()<<" i:"<<l->index<<" "<<l->field<<" "<<l->offset<<endl;
    
    
}
    exit(0);

string s="@";
string d;
v_->resize(10000);
 for(int i=0;i<10000;i++){
     cout<<i<<endl;
     for(int n=0;n<1000;n++){
         TString st;
         v_->getTStrData(i,st);
         st.push_back(s);
         v_->putTStr(i,st);
         
         v_->getTStr(i,st);
         for(int m=0;m<st.size();m++)d=st[m];

     }
 
 }
exit(0);
*/
/*
for(int i=0;i<v->size()&&i<4667519;i++){
     if(i%100000==0)cout<<" "<<i;
     TString st;
     TString st_;
     v->getTStr(i,st);
     //if(st.size()>100){
         for(int n=0;n<st.size();n++){
             string s=st[n];
             st_.push_back(s);
         }
         v_->push_back(st_);
         continue;
     //}
     //v_->push_back(st);
}
 exit(0);
*/



pageRec page;
page.db="DHARMABOOK";
//page.db="LIBRARY_CATALOG";
//page.db="LIBRARY_PATH";
//page.db="LIBRARY_PATH";

//str=page.db;
//longMemory->loadTable(str);
//exit(0);
//GVector *v=longMemory->table[str].data;
//v->resize(1743838);
//exit(0);

//path="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG.tab";
//path="/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/DHARMABOOK.tab";

//v->exportTStr(path);
//v->importTAB(path);
//v->close();
//exit(0);
//ulong data;
//cout<<v->size();
//TString st;
//replaceInDB(page.db);
//v->close();
//string str;
//cout<<"s:"<<v->size()<<" @-> "<<endl;
//exportWordList("/_Image2OCR/wordlist.txt");
//sortWordList("/_Image2OCR/OCRData/DATA_DB/WORDLIST/WORDLIST.txt");
//exit(0);

//TString st;
//for(int a=0;a<v->size();a++){
//joinFieldInTable("LIBRARY_CATALOG", v->size()-1);
//jloadLibraryPathInCatalog();

//v->getTStr(23000, st);
//cout<<"s:"<<st.size()<<" @"<<endl;
//for(int n=0;n<st.size()&&n<10;n++){
//    cout<<" n:"<<st[n]<<" "<<endl;
//}

/*
for(int i=0;i<10000;i++){
    GStr<uint> *v=GStr<uint>::create();
    v->resize(1000000);
    for(int n=0;n<1000;n++)v->push_back(n);
    v->destroy();

}
*/

/*
string ln=inputData.data["ln"];
GVector *dk;
GMap *d;
str="mainDict";
string key="aṃ?pànaüdassàvãatthima¤jeññhikàn";

longMemory->loadTable(str);
dk=longMemory->table[str].data;
indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
d=indexRec->mIndex;
uint index=(uint)d->getHKey_(key, 0);

exit(0);
*/



//}
//exit(0);
page.index=46302;
//page.field=ALL_FIELD;
page.field=1;
page.db="DHARMABOOK";
//page.db="OCR_DICTIONARY";
//page.db="LIBRARY_CATALOG";
//page.text="нравственности";
page.text="Tsewang Chime Gonpo";
//page.text="Aratisuttaṃ";
//page.text="zhal_gdams_snying_gi_bdud_rtsi";
//page.text="wwwབརྩནརྒོདརངབཞིནR12122123";
str=page.db;


//logicProcessor->readDictionary();
//str=logicProcessor->UnicodeToYagpo(str);
//cout<<str;
//cout<<dictionaryReport("ཝ་ནག་","eng");
//logicProcessor->encodeByID(str);

//getPageC(&page);
//cout<<page.bookCount;

if(inputData.data["action"]=="replace"){
    
    //rebuildCategory();
    //importMainDictionary();
    //searchCategory("ChoSpyod");
    //exportDBTable("DHARMABOOK");
    //setCategory("Его Святейшество Далай-Лама","HH Dalay-Lama XIV");
    //replaceInDB("DHARMABOOK");
    //importOCR("DHARMABOOK", "/Volumes/TERSAR_3TB/__LIBRARY_CANON_ALL/_TENJUR_MAIN/bstan_'gyur_Beijing_OCR_Text","{P:index=");
    importTextFolder("DHARMABOOK","/Volumes/TERSAR_3TB/_LIBRARY_/__Letter_RA/rin_chen_gter_mdzod_chen_mo_one_R1450783391/rin_chen_gter_mdzod_chen_mo_R1450783391_v053_OCR","Mahadeva");
    //importTextFolder("DHARMABOOK","/_Image2OCR/__OCR_UPLOAD/kenggiapsoh@gmail.com/Mahadeva","Mahadeva");
    //translateLine("sucaritaṃcare","rus");
    exit(0);
    
    /*
    initCommandData();
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    str="translationDictRus";
    longMemory->loadTable(str);
    for(int n=0;n<100;n++){
    for(int i=70684;i<100000+70684;i++){
        //if(i%10000==0)cout<<i<<" ";
        page.index=i;
        page.field=1;
        getPageC_(&page);
        //page.field=(int)page.pageCount/2;
        //getPageC(&page);
        //str=page.text;
        //GVector *dk=longMemory->table[str].data;
        //indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0, HASH_SEARCH);
        //GMap *d=indexRec->mIndex;
        //str="།གོམས་པ་ཆེ་བ་རྣམས་ལ་བེམ་རིག་བྲལ་ནས་ཆོས་ཉིད་དུ་ལམ་གྱིས་འོང༌། ཆུང་བ་ལ་དཀའ་བས་གདམས་པ་འདི་མེད་ཐབས་མེད་དོ། །གོམས་པ་ཆེ་ཆུང་དང༌། ངོ་འཕྲོད་མ་འཕྲོད་འཆི་ཁར་འཁྲུལ་མ་འཁྲུལ་མེད་པར། རིག་པ་ཕྱི་ཐས་མཐོང་བ་ཡིན་ནོ། །བར་དོ་རང་སྟོབས་ཀྱིས་མ་ཟིན་ཡང༌། གཞན་རྐྱེན་ལ་བརྟེན་ནས་ཟིན་ངེས་པས། ལམ་བློས་ཆོད་དུ་འགྲོ་ངེས་པ་ཡིན་ནོ། །བར་དོ་བློས་ཆོད་ཀྱི་ཞལ་གདམས་སོ། །བརྒྱུད་པ་གསང༌། གདམས་པ་གང༌། ཡིད་ལ་སྤོང༌། ཐེ་ཚོམ་ཤིག །བཀའ་རྒྱས་སྡོམས། བཀའ་མའམ། གཏེར་མའམ། ལུང་བསྟན་པ་སྙམ་དུ་ཡིད་ལ་མི་བྱ་བར་ཉམས་སུ་བླང་ངོ༌།།";
        //GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
        //inputData.data["ln"]="rus";
        //dictionaryReport("གྲུབ་མཐའི་རྣམ་བཞག་","rus");
        //logicProcessor->lineTextTranslation(str);

    
    }
            cout<<endl;
    }
    */
    
    //cout<<translateLine("།གོམས་པ་ཆེ་བ་རྣམས་ལ་བེམ་རིག་བྲལ་ནས་ཆོས་ཉིད་དུ་ལམ་གྱིས་འོང༌། ཆུང་བ་ལ་དཀའ་བས་གདམས་པ་འདི་མེད་ཐབས་མེད་དོ། །གོམས་པ་ཆེ་ཆུང་དང༌། ངོ་འཕྲོད་མ་འཕྲོད་འཆི་ཁར་འཁྲུལ་མ་འཁྲུལ་མེད་པར། རིག་པ་ཕྱི་ཐས་མཐོང་བ་ཡིན་ནོ། །བར་དོ་རང་སྟོབས་ཀྱིས་མ་ཟིན་ཡང༌། གཞན་རྐྱེན་ལ་བརྟེན་ནས་ཟིན་ངེས་པས། ལམ་བློས་ཆོད་དུ་འགྲོ་ངེས་པ་ཡིན་ནོ། །བར་དོ་བློས་ཆོད་ཀྱི་ཞལ་གདམས་སོ། །བརྒྱུད་པ་གསང༌། གདམས་པ་གང༌། ཡིད་ལ་སྤོང༌། ཐེ་ཚོམ་ཤིག །བཀའ་རྒྱས་སྡོམས། བཀའ་མའམ། གཏེར་མའམ། ལུང་བསྟན་པ་སྙམ་དུ་ཡིད་ལ་མི་བྱ་བར་ཉམས་སུ་བླང་ངོ༌།། །།༼༢༣༤༽ [[Path:08_14_08_gdams ngag mdzod_nya.RTF]] [[Category:Tibetan text]] [[Category:gdams ngag mdzod/08_gdams ngag mdzod_nya]]","rus");
    //exportDBTable("translationSktDictRus");
    //exportDBTable("translationDictRus");  exit(0);
    //exportDBTable("mainDict");
    //exportDBTable("DHARMABOOK");
    //dumpWiki("/Volumes/WORK_DHARMA_3TB/MainYagpoOCR/DICTIONARY/ruwiktionary-20160901-pages-articles-multistream.xml");
    //exportDictFromTextDB("paritta_text","/_Image2OCR/OCRData/DATA_DB/paritta_text/paritta_textDict.txt");
    //exportDictFromTextDB("paritta_text","/_Image2OCR/OCRData/DATA_DB/paritta_text/paritta_textDict.txt");
    //importDictTab("DHARMABOOK","/Volumes/LIBRARY_3TB/_LIBRARY/___DICTIONARIES/__DICTIONARIES/____DICTIONARIES_READY_TO_USE/DN.tab","DN",12);
    //checkDublicateID("DHARMABOOK");
    //cout<<searchByTag("DHARMABOOK","Title","༄༅།།དཔལ་ཡེ་ཤེས་ཀྱི་མགོན་པོ་ཕྱག་བཞི་པ་ལྷ་བཅུ་གསུམ་གྱི་མངོན་རྟོགས་སྒྲུབ་བརྒྱུད་ཀརྨ་ཀཾ་ཚང་གི་ལུགས་སངས་རྒྱས་མཉན་པ་ཆེན་པོའི་ཞལ་ནས་བརྒྱུད་པའོ།།",HASH_SEARCH);
    //int  pageCount;
    //cout<<searchCategory("Library catalog",0,&pageCount);
    //cout<<dictionaryReportTitle("ཨུཏྤལ་སྔོན་པོ་འདྲ་","rus");
    //getPageC(&page);
    //cout<<renderCategory(&page,"rus");
    //cout<<listCategory();
    
    //inputData.data["ocrData"]="batchOCR";
    //exportDBTable("DHARMABOOK");
    //importDBTable("DHARMABOOK","/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG.tab");
    //replaceInDB(page.db);
    //string key="[[ergwerwerg:erwerwer]][[Title:wwww]]\n[[Path:ergrgwertgwergwerg]]";
    //re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\]\\]", "{{a_Title|\\1}}");
    //cout<<key;
    //page.index=1177;
    ///page.field=1;
    //getPageC(&page);
    //bookOCR("/Volumes/WORK_DHARMA_3TB/_TEST/1.tif");
    
    //bookOCR("/_Image2OCR/__OCR_UPLOAD/page_0248.tif");
    //str="ཀྱི་གཞུང་";
    //str="གཞུང་ཆེན་";
    //str=logicProcessor->UnicodeToYagpo(str);
    //page.text=str.c_str();

    //cout<<"str:"<<str<<endl;
    cout<<searchDB(&page)<<endl;
    

//cout<<searchDBTable(&page,HASH_SEARCH);
//cout<<searchDBTable(&page,FULL_TEXT_SEARCH);
//cout<<searchDBTable(&page,ID_SEARCH);
//v->close();
//loadLibraryPathInCatalog("LIBRARY_CATALOG");
exit(0);
    
    
}

 /**/

//string key="dffasfas་ \n ༄༅། །རྣམ་གྲོལ་ལག་བཅངས་";
//re2::RE2::Replace(&key,"/་[ །]*༄༅[ །]*/m","་#");

//cout<<key;
//splitOnPagesC();
//exit(1);


/*split on pages
*/
//splitOnPagesC();
//importOCRText("/CANON/BKA'_'GYUR/TRIPITAKA_TEXT/bka'_'gyur_Beijing_OCR_Text");
//importOCRText("/CANON/BKA'_'GYUR/TRIPITAKA_TEXT/bstan_'gyur_Beijing_OCR_Text");
//exit(0);



//splitOnPagesC();
//cout<<translateLine("","rus");
//cout<<dictionaryReport("མཆོག་ཏུ་འགྲོ་བ་","rus");
//replaceInDB();
//exit(0);

/*


    str="DHARMABOOK";
     //str="translationDictRus";
exit(0);
*/

//str="1jlknl1lnlknlnl1lknlknl1lknlkn1";
//re2::RE2::Replace(&str,"1","2");
//print(str);

/*

exit(0);


     vector<TString>r;
     vector<int>f;
     f.push_back(1);
     longMemory->select(str, f, l, FULL_TEXT_SEARCH,r,0);
     //longMemory->select(str, f, l, HASH_SEARCH,r,0);
     cout<<r.size()<<endl;
     for(int i=0;i<r.size();i++){
     cout<<i<<" ->> "<<r[i].getInt(0)<<" "<<r[i].getInt(1)<<endl;
     TString s;
     
     longMemory->table["DHARMABOOK"].data->getTStr(r[i].getInt(0),s);
     string st=s[1];
     int offset=r[i].getInt(1);
     
     st=substr(offset,1000,st);
     cout<<endl<<endl<<st<<"<<<<<"<<endl;
     if(i>100)break;
     }
 
     */
    
    /*str="DHARMABOOK";
     path="/_Image2OCR/EXPORT/";
     longMemory->exportAllRecords(str, path);
     exit(0);
     */
    //inputData.data["ocrLn"]="ocrTibetanSanskrit"; fontEditor->setLanguage(); exit(0); //17245
    //GFont *aliKali = fontEditor->aliKali;
    //GLetter *l;
    //l=aliKali->getLetter(5182);
    //cout<<"H="<<l->letterH<<endl;
    
    //int a=1000000*100;
    //uint b=0xffffffff;
    //b=b>>8;
    //cout<<a<<endl<<b<<endl;;
    //exit(0);
    
    /*
     GFont *aliKali = fontEditor->aliKali;
     inputData.data["action"]="imageNormalisation";
     for(int n=0 ; n<aliKali->size();n++){
     cout<<" "<<n;
     GLetter *l;
     l=aliKali->getLetter(n);
     if(RE2::PartialMatch(l->name, "[\\p{Devanagari}]"))l->OCRKey="S";
     //fontEditor->imageNormalisation(l);
     //fontEditor->rebuildMaskInLetterVector(l,0);
     //l->y0-=1;
     //l->y1-=1;
     //l->reloadMask();
     aliKali->saveLetter(l);
     l->destroy();
     //break;
     //aliKali->setFocalLineInLetter(n);
     }
     exit(0);
     */

/*
 string db="DHARMABOOK";
 inputData.data["root"]="/_Image2OCR/";
 readPreferences();
 GMemory *longMemory=GMemory::create();
 inputData.longMemory=longMemory;
 string key=page->text;
 page->bookCount=0;
 string text=key;
 string report;
 int index=page->index;
 string s;
 longMemory->loadTable(db);
 GVector *v=longMemory->table[db].data;
 TString st;
 string str;
 v->getTStr(page->index,st);
 ostringstream out;
 for(int i=1;i<st.size();i++){
 string line=st[i];
 int pos=(int)line.find(key);
 if(pos!=-1){
 getReportLine(line,pos);
 out<<"<a href=\"./?index="<<index<<"&field="<<i<<"&ocrData=read\" target=\"_blank\">page:"<<i<<"<br>..."<<line<<"....</a><hr/>";
 page->bookCount++;
 
 }
 }
 report=out.str();
 str="<font color=\"blue\">"+key+"</font>";
 report=str_replace(key,str, report);
*/









