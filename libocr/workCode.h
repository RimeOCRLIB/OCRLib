
//inputData.data["action"] = "replace";


if (inputData.data["action"] == "replace") {
    
    if(1){
        pageRecLib page;
        inputData.data["inputFile"]="/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/seekfish@gmail.com/_out/__ALL_text.txt";
        HTML2UniConverter(page);
    }

    if(0){
        cout << sizeof(OCRData) << " " << sizeof(OCRBox) << " " << sizeof(OCRStar) << " " << sizeof(OCRLetter)
        << endl;
        GGraphOCR graph;
        graph.lineOCR();
        exit(0);
    }
    
    
    //отдладка запроса GUI
    //функция чтения и передачи данных в интерфейс Python на основе бибиотеки CTypevvbv+++
    //char* getPageStream(pageRec *page_)
    
    pageRecLib pageLib;
    pageLib.text="field=1&index=53749&ln=rus&ocrData=read";
    
    //pageLib.text="fileList=%2FVolumes%2FWORK_DHARMA_3TB%2F__UPLOAD%2F__OCR_UPLOAD__%2Fhung.firenado.hung%40gmail.com%2FPhangthangma&lineH=0&Tr1=99&Tr2=92&BASE=17&FilterIntegral1=0&FilterGauss1=1&gHor1=0&gVer1=0&d_gain1=1&gHor2=0&gVer2=0&d_gain2=-1&FilterIntegral2=0&FilterGauss2=3&ocrData=imgDirPreview";
    
    
    inputData.data["QUERY"] = pageLib.text;
    parseQuery();
    
    pageLib.command=inputData.ocrCode[inputData.data["ocrData"]];
    
    print_("ocrData:"<<inputData.data["ocrData"]<<endl;)
    print_("command:"<<pageLib.command<<endl;)
    pageLib.text = inputData.data["text"];
    pageLib.data = inputData.data["data"];
    pageLib.db = inputData.data["db"];
    pageLib.ln = inputData.data["ln"];
    pageLib.index = atoi(inputData.data["index"].c_str());
    pageLib.field = atoi(inputData.data["field"].c_str());
    pageLib.pageCount = atoi(inputData.data["pageCount"].c_str());
    pageLib.bookCount = atoi(inputData.data["bookCount"].c_str());
    pageLib.intData = atoi(inputData.data["intData"].c_str());
    pageLib.dataSize = 0;

    print_("pageLib.text:"<<pageLib.text<<endl;)
    
    processRequest(pageLib);
    cout<<pageLib.data<<endl;

    /*
    
    //отдадка запроса командной строки
    
    pageRecLib pageLib;
    
    pageLib.text="ocrData=dictionaryReport&db=mainDict&user_text=རི་བོ་&field=0&mode=HASH_SEARCH&limit=25";
    inputData.data["QUERY"] = pageLib.text;
    parseQuery();
    
    pageLib.command=inputData.ocrCode[inputData.data["ocrData"]];
    
    pageLib.text = inputData.data["text"];
    pageLib.data = inputData.data["data"];
    pageLib.db = inputData.data["db"];
    pageLib.ln = inputData.data["ln"];
    pageLib.index = atoi(inputData.data["index"].c_str());
    pageLib.field = atoi(inputData.data["field"].c_str());
    pageLib.pageCount = atoi(inputData.data["pageCount"].c_str());
    pageLib.bookCount = atoi(inputData.data["bookCount"].c_str());
    pageLib.intData = atoi(inputData.data["intData"].c_str());
    pageLib.dataSize = 0;

    
    print_("ocrData:"<<inputData.data["ocrData"]<<endl;)
    print_("command:"<<pageLib.command<<endl;)
    print_("user_text:"<<pageLib.text<<endl;)
    
    page.command=inputData.ocrCode[inputData.data["ocrData"]];

    string destString;
    processRequest(pageLib);
    destString=pageLib.data;
    print_(destString<<endl);
     
     
    */

    return 0;

    //imageProcess(page);


    //GBitmap *pechaImg=GBitmap::create("/OSBL/_1.png");
    //cout<<pechaImg->columns()<<endl;
    
    
    //exportGFontLetters("/OSBL/FONT_OCR/_FONT_SOURCE/GFont_02_05_2021");
    //return 0;

    //bookOCRGGraph("/OSBL/_1.png");
    
    
    //importOCRTextByIndex("DHARMABOOK","/MainYagpoOCR/TRANSLATIONS/Khadro_Thugtig.txt",1828655); exit(0);
    //importOCRTextByIndex("DHARMABOOK","/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/hq_ch@ukr.net/lha mo brgyad rtsis gza' 'bras dang bcas pa.txt",1828662); exit(0);
    
    //dictionaryReport("ཀརྨ་པཀྴི་ཆོས་ཀྱི་བླ་མ་","rus");
    //translateLine("ཀརྨ་པཀྴི་ཆོས་ཀྱི་བླ་མ་","rus",1);
    //dictionaryReport("འཇིག་རྟེན་","rus"); exit(0);
    
    
    //exportDBTable("translationDictRus");  exit(0);
    //exportDBTable("DHARMABOOK");
    //exit(0);
    
  if(1){
      pageRec page;
      string st="DHARMABOOK";
      page.db=(char*)st.c_str();
      page.field = ALL_FIELD;
      //page.field=1;
      string str="FULL_TEXT_SEARCH";
      //page.text=str.c_str();
      
      //pageRec rec1=searchDB(&page);
      //cout<<rec1.data;

      exit(0);
  }
  

    
  //string str__="རིགས་པ་";
  //logicProcessor->lineTextTranslation(str__,1);
    
  

  /*
  //создает ID номера в старой базу и папку с изображениями букв для новой базы
  string path = "/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_img/";
  uint timeT = time(0);
  for (int i = 0; i < fontEditor->aliKali->size(); i++) {
    GLetter *letter = fontEditor->aliKali->getLetter(i);
    // cout<<letter->letterID<<endl;
    //
    letter->letterID = timeT;
    ((GFontEditor*)inputData.fontEditor)->aliKali->saveLetter(letter);
    GString str_;
    str_ = path;
    str_ += "_";
    str_ += letter->name;
    str_ += "_";
    str_ += letter->letterID;
    //cout << str_.data << " " << endl;
    GBitmap *img=letter->drawPict();
    img->savePNG(str_.data);
    letter->destroy();
    timeT--;
    if (i >0 && i % 100 == 0) {
      cout << i << " ";
      //return (0);
    }
  }
  */
  //importOCRTextByIndex("DHARMABOOK","/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/yeshedeproject@gmail.com/__W1KG12986-I1KG12997-179-312-any_ALL.txt",1828652);
  //importOCRTextByIndex("DHARMABOOK","/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/T0101_zab mo nang don dang de'i 'grel pa zab don snang byed/T0101.txt",1828651);
  

  // cout<<(sizeof(OCRStar)*15+sizeof(OCRLetter))*300000/1000000<<endl;

  // importOCR("DHARMABOOK",
  // "/Volumes/TERSAR_3TB/__LIBRARY_CANON_ALL/_TENJUR_MAIN/bstan_'gyur_Beijing_OCR_Text","{P:index=");
  // importTextFolder("DHARMABOOK","/Volumes/LIBRARY_3TB/17_Tantr","17_Tantr");
  // importText("DHARMABOOK","/MainYagpoOCR/RETREAT/__ngagso_tib_27_06_2019_1.txt",1824026);

  // GBitmap *img=(GBitmap*)loadImage("/_Image2OCR/1_1.png",0);
  //GFontEditor *fontEditor = (GFontEditor *)inputData.fontEditor;
  //GLetter *letter_;

  // int sa=sizeof(GBitMask128);
  /*

      for(int i=0;i<fontEditor->aliKali->size();i++){
          letter=fontEditor->aliKali->getLetter(i);
          if(letter->dX==0&&letter->dY==0){
              cout<<" index:"<<i<<flush;
              letter->setLetterDimensions();
              ((GFontEditor*)inputData.fontEditor)->aliKali->saveLetter(letter);
          }
          letter->destroy();
      }
  */

  // imageProcess(NULL, "");
  // letter=fontEditor->aliKali->getLetter(0);

  //GBitmap *img = GBitmap::create("/_Image2OCR/__1.png");
  //img->binarisation64();

  // graph.printStars("/_Image2OCR/_2Draw_text.html",1);
  // graph.printStars("/_Image2OCR/_2Draw_text.html",2);
  // graph.printToScreenBinary();
  //OCRLetter letter;

  // graph.buildLetter(letter);

  // graph.printLetter(letter,"/_Image2OCR/_2Draw_letter.html",1);

  // graph.printLetterJS(letter,2);

  // letter->setLetterDimensions();
  // fontEditor->importLetter(letter);

  //exit(0);

  // strPNG

  // GBitmap
  // *im=GBitmap::create("/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/p4201.png");
  // GBitmap *im=GBitmap::create("/_Image2OCR/1_1.jpg");
  // im->colorModeOut=0;
  // im->savePNG("/_Image2OCR/1_1.png");

  // imageProcess("/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/p4201.png",
  //             "adaptiveFiltration","85", "21", "300", "3" , "savePNG");

  // int count;
  // const char* getCategoryPage(const char*request,const char*size,const
  // char*page,int *pageCount){

  // getCategoryPage("www.rigzod.org","35","0",&count);
  // 1454772694
  // char *report;
  // int pageCount;
  // getRecordByID(&report,"DHARMABOOK","1454771808","0", &pageCount);
  // cout<<report;

  // exit(0);

  // string str="pad+ma d+hara";
  // logicProcessor->readDictionary();
  // str=logicProcessor->WylieToUniConverter(str);
  // cout<<str<<endl;
  // str=logicProcessor->YagpoToUni(str);
  // cout<<str;
  // exit(0);
  // cout<<dictionaryReport("ཝ་ནག་","eng");

  //pageRec page;
  //page.db = "DHARMABOOK";

  //page.index = 46302;
  // page.field=ALL_FIELD;
  //page.field = 1;
  //page.db = "DHARMABOOK";
  // page.db="OCR_DICTIONARY";
  // page.db="LIBRARY_CATALOG";
  // page.text="нравственности";
  //page.text = "Tsewang Chime Gonpo";
  // page.text="Aratisuttaṃ";
  // page.text="zhal_gdams_snying_gi_bdud_rtsi";
  //page.text="དེ་ལྟར་བརྒྱ་";
  // str=page.db;

  // rebuildCategory();

  // char *c;
  // listCategory(&c);
  // cout<<c;
  // drawLetterBaseImg(0);

  // exit(0);

  // importMainDictionary();
  // searchCategory("ChoSpyod");
  // exportDBTable("DHARMABOOK");
  // setCategory("Его Святейшество Далай-Лама","HH Dalay-Lama XIV");
  // replaceInDB("DHARMABOOK");
  // cout<<translateLine("ཁྱབ་པའི་ཀློང༌།","rus");
  // cout<<listCategory();
  // reloadDictionary("[ཆོས་དང་ཡེ་ཤེས་/Дхарма и мудрость]","rus");
  //string str = "";

  // www_helloVar();
  // replaceInDB("DHARMABOOK");
  // rebuildCategory();
  // RTFToUniFile("/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/throma+phurpa/throma
  // thregchod.rtf");

  // GVector *v=GVector::create("/_Image2OCR/__1v.bin");
  // v->push_back("www");
  // string w;
  // v->getStr(0, w);
  // cout<<w<<" v->indexOffset:"<<*v->indexOffset<<" ind:"<<v->index[1]<<"
  // size:"<<*(ulong*)(v->data+v->index[1]); v->close();

  //str = "index=21";

  // letterProcess(NULL,str.c_str(),(int)str.size());

  // imageProcess(NULL,
  // "fileList=%2F_Image2OCR%2F__page_0011.png&ocrData=preview&Tr=85&Base=28&scale=200&blur=3&dotGain=1&erosion=1&mode=1&");

  //char *p;

  // cout<<translateLine(&p,"བསྟན་པའི་སྙིང་པོ་","rus",4);
  //exit(0);

  //initCommandData();
  // str="translationDictRus";
  // readText(str,"/_Image2OCR/11.txt");

  //GLogicProcessor *logicProcessor = (GLogicProcessor *)inputData.logicProcessor;
  inputData.data["ln"] = "rus";
  // deleteCategory("Tripitaka Otani Catalog");

  // importTextFolder("DHARMABOOK","/Volumes/WORK_DHARMA_3TB/__UPLOAD/__OCR_UPLOAD__/CBETA/__DB_TEXT","Tashio_Tripitaka");

  // dictionaryReport("གྲུབ་མཐའི་རྣམ་བཞག་","rus");
  // logicProcessor->lineTextTranslation(str,1);
  // cout<<logicProcessor->WylieToUnicode(str,0)<<endl;

  //exit(0);

  // logicProcessor->lineTibetanCorrector(str);
  //int rec;

  // str=getRecords("DHARMABOOK_Category","25","1",&rec);
  // str=getRecordsReport("DHARMABOOK","25","1",&rec);
  // str=listCategoryToTable();
  // str=getCategoryPage("ཀློང་ཆེན་མཛོད་བདུན","25","0",&rec);
  // str=getRecordByID("DHARMABOOK","1454773797","0", &rec);
  // writeText(str,"/_Image2OCR/1.txt");

  //cout << str;

  // cout<<str;
  //exit(0);
  /**/

  /*longMemory->loadTable(str);
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
   //indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0,
   HASH_SEARCH);
   //GMap *d=indexRec->mIndex;
   str="།གོམས་པ་ཆེ་བ་རྣམས་ལ་བེམ་རིག་བྲལ་ནས་ཆོས་ཉིད་དུ་ལམ་གྱིས་འོང༌།
   ཆུང་བ་ལ་དཀའ་བས་གདམས་པ་འདི་མེད་ཐབས་མེད་དོ། །གོམས་པ་ཆེ་ཆུང་དང༌།
   ངོ་འཕྲོད་མ་འཕྲོད་འཆི་ཁར་འཁྲུལ་མ་འཁྲུལ་མེད་པར། རིག་པ་ཕྱི་ཐས་མཐོང་བ་ཡིན་ནོ།
   །བར་དོ་རང་སྟོབས་ཀྱིས་མ་ཟིན་ཡང༌། གཞན་རྐྱེན་ལ་བརྟེན་ནས་ཟིན་ངེས་པས། ལམ་བློས་ཆོད་དུ་འགྲོ་ངེས་པ་ཡིན་ནོ།
   །བར་དོ་བློས་ཆོད་ཀྱི་ཞལ་གདམས་སོ། །བརྒྱུད་པ་གསང༌། གདམས་པ་གང༌། ཡིད་ལ་སྤོང༌། ཐེ་ཚོམ་ཤིག
   །བཀའ་རྒྱས་སྡོམས། བཀའ་མའམ། གཏེར་མའམ། ལུང་བསྟན་པ་སྙམ་དུ་ཡིད་ལ་མི་བྱ་བར་ཉམས་སུ་བླང་ངོ༌།།";
   //GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
   //inputData.data["ln"]="rus";
   //dictionaryReport("གྲུབ་མཐའི་རྣམ་བཞག་","rus");
   //logicProcessor->lineTextTranslation(str);


   }
   cout<<endl;
   }
   */

  // cout<<translateLine("།གོམས་པ་ཆེ་བ་རྣམས་ལ་བེམ་རིག་བྲལ་ནས་ཆོས་ཉིད་དུ་ལམ་གྱིས་འོང༌།
  // ཆུང་བ་ལ་དཀའ་བས་གདམས་པ་འདི་མེད་ཐབས་མེད་དོ། །གོམས་པ་ཆེ་ཆུང་དང༌།
  // ངོ་འཕྲོད་མ་འཕྲོད་འཆི་ཁར་འཁྲུལ་མ་འཁྲུལ་མེད་པར། རིག་པ་ཕྱི་ཐས་མཐོང་བ་ཡིན་ནོ།
  // །བར་དོ་རང་སྟོབས་ཀྱིས་མ་ཟིན་ཡང༌། གཞན་རྐྱེན་ལ་བརྟེན་ནས་ཟིན་ངེས་པས།
  // ལམ་བློས་ཆོད་དུ་འགྲོ་ངེས་པ་ཡིན་ནོ། །བར་དོ་བློས་ཆོད་ཀྱི་ཞལ་གདམས་སོ། །བརྒྱུད་པ་གསང༌། གདམས་པ་གང༌།
  // ཡིད་ལ་སྤོང༌། ཐེ་ཚོམ་ཤིག །བཀའ་རྒྱས་སྡོམས། བཀའ་མའམ། གཏེར་མའམ།
  // ལུང་བསྟན་པ་སྙམ་དུ་ཡིད་ལ་མི་བྱ་བར་ཉམས་སུ་བླང་ངོ༌།། །།༼༢༣༤༽ [[Path:08_14_08_gdams ngag
  // mdzod_nya.RTF]] [[Category:Tibetan text]]
  // [[Category:gdams ngag mdzod/08_gdams ngag mdzod_nya]]","rus");
  // exportDBTable("translationSktDictRus");
  // exportDBTable("translationDictRus");  exit(0);
  // exportDBTable("mainDict");
  // exportDBTable("DHARMABOOK");
  // dumpWiki("/Volumes/WORK_DHARMA_3TB/MainYagpoOCR/DICTIONARY/ruwiktionary-20160901-pages-articles-multistream.xml");
  // exportDictFromTextDB("paritta_text","/_Image2OCR/OCRData/DATA_DB/paritta_text/paritta_textDict.txt");
  // exportDictFromTextDB("paritta_text","/_Image2OCR/OCRData/DATA_DB/paritta_text/paritta_textDict.txt");
  // importDictTab("DHARMABOOK","/Volumes/LIBRARY_3TB/_LIBRARY/___DICTIONARIES/__DICTIONARIES/____DICTIONARIES_READY_TO_USE/DN.tab","DN",14);
  // exit(0);
  // checkDublicateID("DHARMABOOK");
  // cout<<searchByTag("DHARMABOOK","Title","ཀ་གཉིས་པ་",HASH_SEARCH);
  // int  pageCount;
  // cout<<searchCategory("Library catalog",0,&pageCount);
  // cout<<dictionaryReportTitle("yuvāvatthā","rus");
  // getPageC(&page);
  // cout<<renderCategory(&page,"rus");
  // cout<<listCategory();
  // importOCRText("DHARMABOOK","/_Image2OCR/__OCR_UPLOAD/roesiavo@gmail.com/ཕུརཔ.txt");
  // exit(0);
  // inputData.data["ocrData"]="batchOCR";
  // exportDBTable("DHARMABOOK");
  // importDBTable("DHARMABOOK","/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG.tab");
  // replaceInDB(page.db);
  // string
  // key="[[ergwerwerg:erwerwer]][[Title:wwww]]\n[[Path:ergrgwertgwergwerg]]";
  // re2::RE2::Replace(&key, "\\[\\[Title:([^\\]]*)\\]\\]", "{{a_Title|\\1}}");
  // cout<<key;


/**/

// string key="dffasfas་ \n ༄༅། །རྣམ་གྲོལ་ལག་བཅངས་";
// re2::RE2::Replace(&key,"/་[ །]*༄༅[ །]*/m","་#");

// cout<<key;
// splitOnPagesC();
// exit(1);

/*split on pages
 */
// splitOnPagesC();
// importOCRText("/CANON/BKA'_'GYUR/TRIPITAKA_TEXT/bka'_'gyur_Beijing_OCR_Text");
// importOCRText("/CANON/BKA'_'GYUR/TRIPITAKA_TEXT/bstan_'gyur_Beijing_OCR_Text");
// exit(0);

// splitOnPagesC();
// cout<<translateLine("","rus");
// cout<<dictionaryReport("མཆོག་ཏུ་འགྲོ་བ་","rus");
// replaceInDB();
// exit(0);

/*


 str="DHARMABOOK";
 //str="translationDictRus";
 exit(0);
 */

// str="1jlknl1lnlknlnl1lknlknl1lknlkn1";
// re2::RE2::Replace(&str,"1","2");
// print(str);

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
// inputData.data["ocrLn"]="ocrTibetanSanskrit"; fontEditor->setLanguage();
// exit(0); //17245 GFont *aliKali = fontEditor->aliKali; GLetter *l;
// l=aliKali->getLetter(5182);
// cout<<"H="<<l->letterH<<endl;

// int a=1000000*100;
// uint b=0xffffffff;
// b=b>>8;
// cout<<a<<endl<<b<<endl;;
// exit(0);

/*
 GFont *aliKali = fontEditor->aliKali;
 inputData.data["action"]="imageNormalisation";
 for(int n=0 ; n<aliKali->size();n++){
 cout<<" "<<n;
 GLetter *l;
 l=aliKali->getLetter(n);
 //if(RE2::PartialMatch(l->name, "[\\p{Devanagari}]"))l->OCRKey="S";
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
 out<<"<a href=\"./?index="<<index<<"&field="<<i<<"&ocrData=read\"
 target=\"_blank\">page:"<<i<<"<br>..."<<line<<"....</a><hr/>";
 page->bookCount++;

 }
 }
 report=out.str();
 str="<font color=\"blue\">"+key+"</font>";
 report=str_replace(key,str, report);
 */

/*
 //string inputPath="/_Image2OCR/_1Draw_text.jpg";
 //string inputPath="/_Image2OCR/_1Draw_text_scale.jpg";
 //string inputPath="/_Image2OCR/_1Draw_text_rotate.jpg";
 //string inputPath="/_Image2OCR/_2Draw_3_3.jpg";
 //string inputPath="/_Image2OCR/_2Draw_3_34.jpg";
 string inputPath="/_Image2OCR/_1Draw_letter.jpg";






 img->binarisation(85, 11);
 GGraph graph;
 graph.init(img);

 graph.graphFocalLine(15);

 //img->bytes_data=graph.bytes_data;
 //imageEditor->WriteImageData(img,"/_Image2OCR/_2Draw_focal_lines1.jpg",0);
 return 0;


 graph.focalPointXTL();
 //graph.printToScreenBinary();
 cout<<" focalPoint.size():"<<graph.focalPoint.size()<<endl;
 graph.focalLineRecognition();
 cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;
 graph.findExtremPoints();
 cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;

 graph.buildOCRStars();
 graph.printStars("/_Image2OCR/_2Draw_text.html",2);
 //graph.printToScreenBinary();
 return 0;

 OCRLetter letter;
 GStr<OCRLetter> *font=GStr<OCRLetter>::create("/_Image2OCR/_2Draw_font.bin");
 //graph.buildLetter(letter);
 //font->resize(1);
 //font[0][0]=letter;

 letter=font[0][0];
 graph.printLetter(letter, "/_Image2OCR/_2Draw_letter.html",  2);

 graph.focalLineCorrelation(letter);



 inputData.c_out.close();

 exit(0);


 */

/*class Base
 {
 public:
 Base(){
 cout << "Base Constructor Called\n";
 }
 virtual ~Base(){
 cout << "Base Destructor called\n";
 }
 };

 class Derived1: public Base
 {
 public:
 char *memblock = new char [1024];
 Derived1(){
 cout << "Derived constructor called\n";
 }
 ~Derived1(){
 cout << "Derived destructor called\n";
 delete[](memblock);
 cout << "Buffer deallocator called\n";
 }
 };

 Base *b = new Derived1;
 delete b;
 */

// exit(0);

// cout<<fontEditor->aliKali->size();

// fontEditor->aliKali->resize(21911);
// fontEditor->buildGFontFromFont();
// fontEditor->rebuildGFont(); exit(0);
// fontEditor->setScale(); exit(0);

// l="བདག་ཉིད་ཆེན་པོ་གཙང་པ་རྒྱ་རས་པའི་ཐུགས་དམ་དགེ་སྦྱོར་བདུན་པའི་གསུང་སྒྲོས་གྲུབ་པའི་དབང་ཕྱུག་འབྲས་མོ་བ་སངས་རྒྱས་འབུམ་ཞེས་གྲགས་པའི་དགེ་བའི་བཤེས་གཉེན་དེས་མཛད་པའི་གཞུང་འགྲེལ་རྫོགས་ས";
// string ret="0"; ret[0]=11;
// l=str_replace(ret.c_str(),"\n", l);
// cout<<l;

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

// find entry

// str="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG_1024_9_data.index";
// string str_="/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/DHARMABOOK.bin";
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

// exit(0);

// string
// str_="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG_1024_6_data.index";
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

// page.db="LIBRARY_CATALOG";
// page.db="LIBRARY_PATH";
// page.db="LIBRARY_PATH";

// str=page.db;
// longMemory->loadTable(str);
// exit(0);
// GVector *v=longMemory->table[str].data;
// v->resize(1743838);
// exit(0);

// path="/_Image2OCR/OCRData/DATA_DB/LIBRARY_CATALOG/LIBRARY_CATALOG.tab";
// path="/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/DHARMABOOK.tab";

// v->exportTStr(path);
// v->importTAB(path);
// v->close();
// exit(0);
// ulong data;
// cout<<v->size();
// TString st;
// replaceInDB(page.db);
// v->close();
// string str;
// cout<<"s:"<<v->size()<<" @-> "<<endl;
// exportWordList("/_Image2OCR/wordlist.txt");
// sortWordList("/_Image2OCR/OCRData/DATA_DB/WORDLIST/WORDLIST.txt");
// exit(0);

// TString st;
// for(int a=0;a<v->size();a++){
// joinFieldInTable("LIBRARY_CATALOG", v->size()-1);
// jloadLibraryPathInCatalog();

// v->getTStr(23000, st);
// cout<<"s:"<<st.size()<<" @"<<endl;
// for(int n=0;n<st.size()&&n<10;n++){
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
 indexRecord *indexRec=longMemory->createIndex(longMemory->table[str], 0,
 HASH_SEARCH); d=indexRec->mIndex; uint index=(uint)d->getHKey_(key, 0);

 exit(0);
 */

/*
 unsigned char ptr[4];
 *(unsigned int*)ptr=0xdab0a001;

 //unsigned char * ptr = (unsigned char*) 0xdab0a001;
 *(((unsigned short *) ptr) + 1) = 0xaa;

 // *(ptr + 1) = 0xaa;
 uint b=*((uint*)ptr);
 */
//}
// exit(0);

// logicProcessor->encodeByID(str);

// getPageC(&page);
// cout<<page.bookCount;
      
}


/*
 ///// проверка регулярных выражений.
 /// вывод - regex работает только с двухбайтовой частью utf-8
 
 #include <regex>
 
 vector<string>v;
 string path="/Volumes/WORK_DHARMA_3TB/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/WORK/xaa1.txt";
 string pathOut="/Volumes/WORK_DHARMA_3TB/_Image2OCR/OCRData/DATA_DB/DHARMABOOK/WORK/xaa2.txt";
 //readText(v, path);
 string res="";
 v.push_back("test སྠེཏྱཇདཕ༹གqཨེ་ཐ་ཇ་ཏ་ཀརྟ་གྭ་ཏ་ག་ཧེ་རྠв་к་е་п༓  цукепцке\\!\\@\\#\\$\\%\\^\\&\\*\\(\\)\\_\\+\\|\\{\\}\\[\\]\\'\\,\\.\\/\\<\\>\\?\\'\\±\\§(упцукЦВКПЦУЕКП укпацукп test");
 
 size_t size=v.size();
 //size=10000;
 string str, strOut;
 int step=(int)v.size()/1000;
 int progress=0;
 if(step==0)step=1;
 
 
 locale::global(std::locale("en_US.UTF-8"));  //important
 
 regex self_regex("REGULAR EXPRESSIONS", regex_constants::ECMAScript | regex_constants::icase);
 //regex reg("[а-я \\!\\@\\#\\%\\^\\&\\*\\\\\(\\)\\+\\|\\{\\}\\[\\]\\'\\,\\.\\/\\<\\>\\?\\'\\±\\§\\$]+");
 regex reg("[a-z ]+");
 
 for(int i=0;i<size;i++){
 
 if(i%step == 0){
 cout<<"."<<progress<<flush;
 progress++;
 }
 
 str=v[i];
 strOut = regex_replace(str, reg, "\n");
 //cout<<v[i]<<endl;
 if(strOut==""||strOut==" ")continue;
 res+=strOut+"\n";
 cout<<"res: "<<strOut<<endl;
 
 }
 
 writeText(res, pathOut);
 return 0;
 */
