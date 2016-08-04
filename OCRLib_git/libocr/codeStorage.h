/*
 
 
 
 //выполнение ассинхронного запроса
 //int i=system ("php /_Image2OCR/edit/1.php &");
 //printf ("The value returned was: %d.\n",i);
 //exit(0);
 

 
 vector<string> vt,st;
 path="/Volumes/TERSAR_3TB/DICT_CONVERT/CORPUS_/_all_corpus.txt";
 
 
 readText(vt,path);
 cout<<"size="<<vt.size()<<endl;
 int step=0;
 
 for(int i=0;i<vt.size();i++){
 
 vt[i]=str_replace("་","\n",vt[i]);
 if(step==1000000){cout<<"."<<i;step=0;}step++;
 
 }
 writeText(vt,path);
 exit(0);
 
 
 //sort(vt.begin(),vt.end());
 int count=vt.size();
 int ind=0; int sCount;
 char *buff;
 //int step=0;
 
 ifstream srcInput;
 srcInput.open(path.c_str());
 string str,srcLine;
 
 ofstream  out; out.setf(ios::unitbuf); out.open("/1.txt");
 
 
 FILE * hFile;
 unsigned long  size;
 hFile = fopen( path.c_str(), "r");
 if (hFile == NULL){
 return 0; 
 }else{
 fseek(hFile, 0, SEEK_END);
 size= ftell(hFile);   //cout<<" size="<<size<<endl;
 fseek(hFile, 0, SEEK_SET);
 buff = (char *)malloc(size+1);
 if (size != fread(buff, sizeof(char), size, hFile)){
 free(buff);
 return 0;
 }
 fclose(hFile);
 }
 
 long indText=0;
 
 while(indText<size){
 if(buff[indText]==13||buff[indText]==0)buff[indText]=10;
 indText++;
 
 }
 
 path="/Volumes/TERSAR_3TB/DICT_CONVERT/CORPUS_/_all_corpus.txt";
 
 writeText(buff, size, path.c_str());
 
 
 cout<<"size="<<size<<endl;
 
 exit(0);
 
 indText=0;
 
 while(indText<size){
 srcLine="";
 while(indText<size&&buff[indText]!=10){
 srcLine+=buff[indText];
 indText++;
 }
 sCount=0;
 indText++;
 while(buff[indText]==10)indText++;
 cout<<"srcLine="<<srcLine<<" size="<<srcLine.size()<<"c="<<(short)buff[indText]<<endl;
 
 if(srcLine.size()==0){indText++; continue;}
 
 while(1){
 sCount++;
 str="";
 while(indText<size&&buff[indText]!=10){
 cout<<"c="<<(short)buff[indText];
 str+=buff[indText];
 indText++;
 }
 cout<<"str="<<str<<" str="<<str.size()<<endl;
 if(str!=srcLine){
 indText++;
 break;}    
 indText++;
 }
 cout<<sCount<<" "<<endl;       
 out<<sCount<<" "<<str<<endl;
 //sprintf(buf,"%d ",sCount);
 //str=buf+str;
 //st.push_back(str); 
 //if(step==100000){step=0;cout<<".";}step++;
 
 }
 srcInput.close();
 
 
 //writeText(st,"/Volumes/TERSAR_3TB/DICT_CONVERT/CORPUS_/_all_corpus_uniq.txt"); 
 cout<<"done"; exit(0);
 */





/*// Example: simple search for a string:
 string str,str_;
 
 //string contents="fo11owing\n fo11ow";
 string contents="ris = 5 ris = 6 ris = 7";
 
 //      string contents = ...;          // Fill string somehow
 StringPiece input(contents);    // Wrap a StringPiece around it
 //
 string match,match1,match2,match3,var;
 int value;
 
 // while (RE2::Consume(&input, "11")) {
 while (RE2::FindAndConsume(&input, "((\\w+) = (\\d+))",&match, &var, &value)){
 cout<<"match="<<match<<" var="<<var<<" value="<<value<<endl;
 }
 
 
 
 //int i=RE2::PartialMatch("fo11owing fo11ow", "(ri (bo[ bsang]*)mchod)", &str, &str_);
 //
 //cout<<" str="<<str<<" str_="<<str_<<" i="<<i;
 // Example: find first number in a string
 //      int number;
 //      RE2::PartialMatch("x*100 + 20", "(\\d+)", &number);
 //cout<<" number="<<number;
 
 
 RE2::PartialMatch("1","1");
 exit(0);
 */



/////Tokio Cabinet

/*
 
 leveldb::DB* db;
 leveldb::Options options;
 options.create_if_missing = true;
 leveldb::Status status = leveldb::DB::Open(options, "/tmp_/testdb", &db);
 assert(status.ok());
 */        
//char buf[10];
//char value_[256];
//string value_; 
//map<string,string> db_Map;

//cout<<"textDictTibSize="<<logicProcessor->textDictTibSize<<endl;

/* for(int a=0;a<logicProcessor->textDictTibSize;a+=256){
 sprintf (buf,"%d",a);
 memcpy(value,logicProcessor->textDictTib+a,256);
 db_Map[buf]=value;
 //cout<<a<<" ";
 //db->Put(leveldb::WriteOptions(), buf, value);
 }
 */

/*   
 TCBDB *bdb;
 TCHDB *hdb;
 BDBCUR *cur;
 int ecode;
 char *key, *value;
 
 hdb = tchdbnew();
 if(!tchdbopen(hdb, "/tmp_/casket.tch", HDBOWRITER | HDBOCREAT)){
 ecode = tchdbecode(hdb);
 fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
 }
 
 */ 


// create the object /
// bdb = tcbdbnew();

// open the database /
// if(!tcbdbopen(bdb, "/tmp_/casket.tcb", BDBOWRITER | BDBOCREAT)){
//     ecode = tcbdbecode(bdb);
//     fprintf(stderr, "open error: %s\n", tcbdberrmsg(ecode));
// }

// store records /
//if(!tcbdbput2(bdb, "foo", "hop") ||
//   !tcbdbput2(bdb, "bar", "step") ||
//   !tcbdbput2(bdb, "baz", "jump")){
//    ecode = tcbdbecode(bdb);
//    fprintf(stderr, "put error: %s\n", tcbdberrmsg(ecode));
//}



/* 
 for(int a=0;a<10000000;a++){//
 sprintf (buf,"%d",a);
 //cout<<"buf="<<buf<<endl;
 
 ///memcpy(value_,logicProcessor->textDictTib+a,256);
 
 if(!tchdbput2(hdb, buf, "HelloWorld:|:MessagePack:|:") ){
 ecode = tchdbecode(hdb);
 fprintf(stderr, "put error: %s\n", tchdberrmsg(ecode));
 }
 
 
 //if(!tcbdbput2(bdb, buf, "HelloWorld:|:MessagePack:|:")){
 //    ecode = tcbdbecode(bdb);
 //    fprintf(stderr, "put error: %s\n", tcbdberrmsg(ecode));
 //}
 
 }
 */ 
/* 
 for(int a=0;a<10000000;a++){//
 sprintf (buf,"%d",a);
 //cout<<"buf="<<buf<<endl;
 
 ///memcpy(value_,logicProcessor->textDictTib+a,256);
 
 value = tcbdbget2(bdb, buf);
 
 
 if(value){
 //cout<<"buf="<<buf<<endl;   
 //for(int i=0;i<256;i++){
 //    if(value[i]==32)i_++;
 //}
 free(value);
 } else {
 cout<<"buf="<<buf<<endl;
 ecode = tcbdbecode(bdb);
 fprintf(stderr, "get error: %s\n", tcbdberrmsg(ecode));
 //exit(0);
 }    
 
 
 }
 */

/* traverse records 
 cur = tcbdbcurnew(bdb);
 tcbdbcurfirst(cur);
 while((key = tcbdbcurkey2(cur)) != NULL){
 value = tcbdbcurval2(cur);
 if(value){
 //printf("%s:%s\n", key, value);
 free(value);
 }
 free(key);
 tcbdbcurnext(cur);
 }
 tcbdbcurdel(cur);
 */
/* traverse records 
 tchdbiterinit(hdb);
 while((key = tchdbiternext2(hdb)) != NULL){
 value = tchdbget2(hdb, key);
 if(value){
 //printf("%s:%s\n", key, value);
 free(value);
 }
 free(key);
 }
 
 
 */   
/*
 for(int a=0;a<logicProcessor->textDictTibSize;a+=256){//
 sprintf (buf,"%d",a);
 //cout<<"buf="<<buf<<endl;
 
 memcpy(value_,logicProcessor->textDictTib+a,256);
 
 if(!tcbdbput2(bdb, buf, value_)){
 ecode = tcbdbecode(bdb);
 fprintf(stderr, "put error: %s\n", tcbdberrmsg(ecode));
 }
 
 }
 */
/*
 
 int i_=0;
 cout<<"start fork"<<endl;
 
 for(int t=0;t<5;t++){    
 int pidID = fork();
 if (pidID < 0)
 error((char*)"ERROR on fork");
 if (pidID == 0)  {
 
 for(int a=0;a<logicProcessor->textDictTibSize;a+=256){
 sprintf (buf,"%d",a);
 
 
 //memcpy(value_,logicProcessor->textDictTib+a,256);
 value = tcbdbget2(bdb, buf);
 
 
 if(value){
 //cout<<"buf="<<buf<<endl;   
 for(int i=0;i<256;i++){
 if(value[i]==32)i_++;
 }
 free(value);
 } else {
 cout<<"buf="<<buf<<endl;
 ecode = tcbdbecode(bdb);
 fprintf(stderr, "get error: %s\n", tcbdberrmsg(ecode));
 //exit(0);
 }    
 }     
 cout<<"i_="<<i_<<endl;
 exit(0);
 }
 
 }   
 */

// if(!tcbdbclose(bdb)){
//     ecode = tcbdbecode(bdb);
//     fprintf(stderr, "close error: %s\n", tcbdberrmsg(ecode));
// }

// delete the object /
// tcbdbdel(bdb); 
// exit(0);
//

/* close the database
 if(!tchdbclose(hdb)){
 ecode = tchdbecode(hdb);
 fprintf(stderr, "close error: %s\n", tchdberrmsg(ecode));
 }
 */
/* delete the object */
//tchdbdel(hdb);


// exit(0);
/* retrieve records         value = tcbdbget2(bdb, "foo");
 if(value){
 printf("%s\n", value);
 free(value);
 } else {
 ecode = tcbdbecode(bdb);
 fprintf(stderr, "get error: %s\n", tcbdberrmsg(ecode));
 }
 
 */





/* for(int a=0;a<logicProcessor->textDictTibSize;a+=256){
 sprintf (buf,"%d",a);
 memcpy(value,logicProcessor->textDictTib+a,256);
 db->Put(leveldb::WriteOptions(), buf, value);
 }
 */

/* 
 
 //for(int a=0;a<logicProcessor->textDictTibSize;a+=256){
 for(int a=0;a<1000000;a++){
 
 sprintf (buf,"%d",a);
 //memcpy(value,logicProcessor->textDictTib+a,256);
 db->Get(leveldb::ReadOptions(), buf, &value);
 //cout<<"a="<<a<<value;  
 }
 */
/*     
 unsigned int i=0;
 unsigned int i_=0;
 unsigned int c=logicProcessor->textDictTibSize;
 
 cout<<"c="<<c<<endl;
 
 for(unsigned int a=0;a<c;a++){
 //for(int a=0;a<1000000;a++){    
 //sprintf (buf,"%d",a);
 if(logicProcessor->textDictTib[a]==32)i++;
 i_++;
 //memcpy(value,logicProcessor->textDictTib+a,256);
 //for(int i=0;i<256;i++)value[i]=0;
 //value_=db_Map[buf];
 //db->Get(leveldb::ReadOptions(), buf, &value);
 //cout<<"a="<<a<<value;  
 }
 cout<<"i="<<i<<" i_="<<i_<<" logicProcessor->textDictTibSize="<<c;
 */    





/* 
 TCBDB *bdb;
 TCHDB *hdb;
 BDBCUR *cur;
 int ecode;
 char *key, *value;
 
 hdb = tchdbnew();
 if(!tchdbopen(hdb, "/tmp_/casket.tch", HDBOWRITER | HDBOCREAT)){
 ecode = tchdbecode(hdb);
 fprintf(stderr, "open error: %s\n", tchdberrmsg(ecode));
 }
 
 */




cout<<endl<<endl<<"//_____Tokio cabinet"<<endl; 

/*    
 //Fixed lenght record 
 TCFDB *fdb;
 int ecode;
 char *key, *value;
 char buf[256];
 
 ///create the object 
 fdb = tcfdbnew();
 
 if(!tcfdbtune(fdb, 256, 5120000000)){
 ecode = tcfdbecode(fdb);
 fprintf(stderr, "tune error: %s\n", tcfdberrmsg(ecode));
 exit(0);
 }
 
 
 // open the database
 if(!tcfdbopen(fdb, "/_tmp/casket.tcf", FDBOWRITER | FDBOCREAT)){
 ecode = tcfdbecode(fdb);
 fprintf(stderr, "open error: %s\n", tcfdberrmsg(ecode));
 
 }
 fprintf(stderr, "tune fdb->limid=%d\n",fdb->limid);
 
 // store records 
 
 int a=1;
 for(a=1;a<10000000;a++){//
 
 ///memcpy(value_,logicProcessor->textDictTib+a,256);
 sprintf (buf,"HelloWorld Привет друзья བཀྲ་ཤིས་བདེ་ལེགས། Алоха земляне! %d",a);
 //cout<<"a="<<a<<endl;
 if(a==38760){
 int b=0; b++;
 //cout<<"a="<<a<<endl;
 }
 
 if(!tcfdbput(fdb,a, buf,1024)){
 cout<<"a="<<a<<endl;
 ecode = tcfdbecode(fdb);
 fprintf(stderr, "put error: %s\n", tcfdberrmsg(ecode));
 
 if(!tcfdbclose(fdb)){
 ecode = tcfdbecode(fdb);
 fprintf(stderr, "close error: %s\n", tcfdberrmsg(ecode));
 }
 exit(0);
 
 }
 
 //if(!tcfdbput(fdb, a, "HelloWorld:|:MessagePack:|:",255 ) ){
 ///    ecode = tcfdbecode(fdb);
 //    fprintf(stderr, "put error: %s\n", tcfdberrmsg(ecode));
 //    exit(0);
 //}
 }
 //memcpy(value_,logicProcessor->textDictTib+a,256);
 TIME_START
 
 
 cout<<"data stored"<<endl;
 
 /// traverse records /
 tcfdbiterinit(fdb);
 /*while((key = tcfdbiternext3(fdb)) != NULL){
 value = tcfdbget3(fdb, key);
 if(value){
 //printf("%s:%s\n", key, value);
 free(value);
 }
 free(key);
 }
 
 int sData[100];
 int b=0; cout<<"a="<<a<<endl;
 while(b<a){
 value =(char*) tcfdbget(fdb, a,sData);
 if(value){
 //printf("%s:%s\n", key, value);
 int c=strlen(value); if(!c){cout<<"no data"<<endl; break;}
 free(value);
 }
 b++;
 
 }
 
 /// close the database /
 if(!tcfdbclose(fdb)){
 ecode = tcfdbecode(fdb);
 fprintf(stderr, "close error: %s\n", tcfdberrmsg(ecode));
 }
 
 /// delete the object 
 tcfdbdel(fdb);
 TIME_PRINT_
 cout<<"data readed"<<endl;
 cout<<"complete"<<endl;
 */


//Table database

TCTDB *fdb;
int ecode, pksiz, i, rsiz;
char pkbuf[256];
const char *rbuf, *name;
TCMAP *cols;
TDBQRY *qry;
TCLIST *res;

///create the object 
fdb = tctdbnew();


TIME_START   


// open the database
if(!tctdbopen(fdb, "/_tmp/casket.tcf", TDBOWRITER | TDBOCREAT)){
    ecode = tctdbecode(fdb);
    fprintf(stderr, "open error: %s\n", tcfdberrmsg(ecode));
    
}

// store records 

int a=1;
for(a=1;a<1000000;a++){//
    pksiz = sprintf(pkbuf, "%ld", (long)tctdbgenuid(fdb));
    cols = tcmapnew3("HelloWorld", "HelloWorld", "HelloWorld", "HelloWorld", "lang", "ja,en,c", NULL);
    //cout<<"a="<<a<<endl;
    if(!tctdbput(fdb, pkbuf, pksiz, cols)){
        ecode = tctdbecode(fdb);
        fprintf(stderr, "put error: %s\n", tctdberrmsg(ecode));
    }
    tcmapdel(cols);
    
}         

//memcpy(value_,logicProcessor->textDictTib+a,256);



cout<<"data stored tctdbrnum()="<<tctdbrnum(fdb)<<endl;

/* search for records 
 qry = tctdbqrynew(fdb);
 tctdbqrysetorder(qry, "HelloWorld", TDBQOSTRASC);
 tctdbqrysetlimit(qry, 10, 0);
 res = tctdbqrysearch(qry);
 for(i = 0; i < tclistnum(res); i++){
 rbuf = tclistval(res, i, &rsiz);
 cols = tctdbget(tdb, rbuf, rsiz);
 if(cols){
 printf("%s", rbuf);
 tcmapiterinit(cols);
 while((name = tcmapiternext2(cols)) != NULL){
 printf("\t%s\t%s", name, tcmapget2(cols, name));
 }
 printf("\n");
 tcmapdel(cols);
 }
 }
 
 tclistdel(res);
 tctdbqrydel(qry);
 */




// Начало xml разметки
ostringstream str;
str<<"<?xml version='1.0' encoding='utf-8'?>";
str<<"<rows>";
str<<"<page>1</page>";
str<<"<total>4</total>";
str<<"<records>10</records>";
int a=0;
while(a<10) {
    str<< "<row id='"<<a<<"'>";
    str<< "<cell></cell>";
    str<< "<cell>"<<a<<"བཀྲ་ཤིས་བདེ་ལེགས"<<"</cell>";//str<< "<cell><![CDATA["<<a<<"_a1]]></cell>";
    str<< "<cell>"<<a<<"</cell>";
    str<< "<cell>"<<a<<"བཀྲ་ཤིས་བདེ་ལེགས"<<"</cell>";//str<< "<cell><![CDATA["<<a<<"_a3]]></cell>";
    str<< "<cell>"<<a<<"</cell>";
    str<< "<cell>"<<a<<"</cell>";
    //str<< "<cell>"<<a<<"</cell>";
    
    str<< "</row>";
    a++;
}
str<<"</rows>";
string res=str.str();
inputData.data["HTTPType"]="text/xml";
return res;





TIME_PRINT_

/// traverse records /
int cnt = 0;
if(!tctdbiterinit(fdb)){
    exit(1);
}

while((cols = tctdbiternext3(fdb)) != NULL){
    int pksiz;
    const char *pkbuf = (const char *)tcmapget(cols, "", 0, &pksiz);
    if(!pkbuf){ cout<<"no data"; break;
    }
    tcmapdel(cols);
}
/// close the database /

if(!tctdbclose(fdb)){
    ecode = tctdbecode(fdb);
    fprintf(stderr, "close error: %s\n", tctdberrmsg(ecode));
}

/* delete the object */
tctdbdel(fdb);



TIME_PRINT_
cout<<"data readed"<<endl;
cout<<"complete"<<endl;



exit(0);


///create the object 
fdb = tctdbnew();

//TIME_START   
// open the database
if(!tctdbopen(fdb, "/_Image2OCR/OCRData/_XML_Dict/TibetanGrammarDict.tcf", TDBOWRITER | TDBOCREAT)){
    ecode = tctdbecode(fdb);
    fprintf(stderr, "open error: %s\n", tcfdberrmsg(ecode));
    
}
/// traverse records /
int cnt = 0;
if(!tctdbiterinit(fdb)){
    cout<<"no valid database";  exit(1);
}

// Начало xml разметки
ostringstream str;
str<<"<?xml version='1.0' encoding='utf-8'?>";
str<<"<rows>";
str<<"<page>1</page>";
str<<"<total>4</total>";
str<<"<records>10</records>";

int a=0;
while((cols = tctdbiternext3(fdb)) != NULL&&a<10){
    int pksiz;
    const char *pkbuf = (const char *)tcmapget(cols, "", 0, &pksiz);
    if(!pkbuf){ cout<<"no data"; break;
    }
    if(cols){
        str<< "<row id='"<<a<<"'>";
        str<< "<cell></cell>";
        str<< "<cell>1</cell>";
        tcmapiterinit(cols);
        while((name = tcmapiternext2(cols)) != NULL){
            //printf("\t%s\t%s", name, tcmapget2(cols, name));
            // str<< "<cell>"<<tcmapget2(cols, name)<<"</cell>";
            str<< "<cell>"<<1<<"</cell>";
            
        }
        tcmapdel(cols);
        a++;
    }
    
}
//close XML
//str<< "<cell><![CDATA["<<a<<"_a3]]></cell>";
str<< "</row>";
str<<"</rows>";

/// close the database /

if(!tctdbclose(fdb)){
    ecode = tctdbecode(fdb);
    fprintf(stderr, "close error: %s\n", tctdberrmsg(ecode));
}

/* delete the object */
tctdbdel(fdb);
string res=str.str();

cout<<res;

inputData.data["HTTPType"]="text/xml";
return res;


// create the object /
// bdb = tcbdbnew();

// open the database /
// if(!tcbdbopen(bdb, "/tmp_/casket.tcb", BDBOWRITER | BDBOCREAT)){
//     ecode = tcbdbecode(bdb);
//     fprintf(stderr, "open error: %s\n", tcbdberrmsg(ecode));
// }

// store records /
//if(!tcbdbput2(bdb, "foo", "hop") ||
//   !tcbdbput2(bdb, "bar", "step") ||
//   !tcbdbput2(bdb, "baz", "jump")){
//    ecode = tcbdbecode(bdb);
//    fprintf(stderr, "put error: %s\n", tcbdberrmsg(ecode));
//}


printf("\n This is console");
fflush(stdout);
fgetpos(stdout, &pos);
fd = dup(fileno(stdout));
freopen(fname, "a+", stdout);   

printf("inside file op");  

fflush(stdout);
dup2(fd,fileno(stdout));
close(fd);
clearerr(stdout);
fsetpos(stdout, &pos);
printf("\nBack to Console");



/*
 The difference between the GET and POST is how the information from the form is sent to the CGI program, from the server.
 
 A GET will provide the the user's input to the CGI program as an environment variable called QUERY_STRING. The CGI program would read this environment variable (using the C getenv() function) and parse it to get the user's input. A GET method will also show the input data to the user in the URL area of the browser, showing a string like www.somewhere.com/CGIFavColor.CGI?FavColor=Black. The GET method is acceptable for small amounts of data. Also, GET is the default method, when a CGI program is run via a link
 
 A POST will provide the user's input to the CGI program, as if it were type at the keyboard, using the standard input device, or stdin. If POST is used, then an environment variable called CONTENT_LENGTH indicates how much data is being sent. You can read this data into a buffer, by doing something like:
 
 char Buffer[512]={0};
 int InputLength = atoi( getenv("INPUT_LENGTH") );
 InputLength = min( InputLength, sizeof(Buffer)-1 ); // Avoid buffer overflow /
 fread( Buffer, InputLength, 1, stdin );
 Your CGI program should inspect the REQUEST_METHOD environment variable to determine if the form was a GET or POST method, and take the appropriate action to retrieve the form.
 
 For a listing and working demonstration of other CGI Environment variables, click here.
 
 Variable Name	Description
 CONTENT_TYPE	The data type of the content. Used when the client is sending attached content to the server. For example file upload etc.
 CONTENT_LENGTH	The length of the query information. It's available only for POST requests
 HTTP_COOKIE	Return the set cookies in the form of key & value pair.
 HTTP_USER_AGENT	The User-Agent request-header field contains information about the user agent originating the request. Its name of the web browser.
 PATH_INFO	The path for the CGI script.
 QUERY_STRING	The URL-encoded information that is sent with GET method request.
 REMOTE_ADDR	The IP address of the remote host making the request. This can be useful for logging or for authentication purpose.
 REMOTE_HOST	The fully qualified name of the host making the request. If this information is not available then REMOTE_ADDR can be used to get IR address.
 REQUEST_METHOD	The method used to make the request. The most common methods are GET and POST.
 SCRIPT_FILENAME	The full path to the CGI script.
 SCRIPT_NAME	The name of the CGI script.
 SERVER_NAME	The server's hostname or IP Address
 SERVER_SOFTWARE	The name and version of the software the server is running.
 */

if(0){
    
    //GVector *vector=GVector::create("/1.txt"); 
    GVector *mainVector=GVector::create("/1.txt"); 
    mainVector->resizeData(10,300); 
    GVector *vector=GVector::create(mainVector,"dataVector"); 
    vector->resizeData(2000000,250);
    //string strW="བ་གཟུང་བ་ནི་རང་བཞིན་ཏེ་དེས་ན་གཉིས་སུ་མེཛ་པའི་བདེ་བ་ལ་སྤྱོད་པ་ན་བ/";
    GStr<uint>* keyDict=GStr<uint>::create(mainVector,"keyDict");
    keyDict->resize(2000000);    TIME_PRINT_
    
    string  strW="1234567890"; 
    TString st;
    st.push_back(strW);
    for(uint a=0;a<100;a++)vector->push_back(&st);
    cout_<<"sizeVector="<<vector->size();
    
    //    exit(0);
    
    //TString st;
    //string str="1\t2\t3\t4\t5\t6\t7\t8\t9\t0\t_";
    //st.push_TabStr(str);
    //st.push_back(str);
    
    for(int a=0;a<100;a++){
        //vector->push_back(&st);
        //vector->getTStr(a,&st1);
        vector->getStr(a,strW);
        cout_<<"strW="<<strW<<endl;
    }
    exit(0);
    //st.push_TabStr(str);
    //cout<<"size1="<<st.size<<" len1="<<st.len<<" vector->size()="<<vector->size()<<endl;
    //vector->push_back(&st);
    
    TString st1;
    
    for(int a=0;a<vector->size();a++){
        vector->getTStr(a,&st1);
        for(int a=0;a<st1.len;a++)cout<<" a="<<st1[a];
        cout<<endl;
    }
    //st.push_TabStr(str);
    
    cout<<"size="<<st.size<<" len="<<st.len;
    
    //for(int a=0;a<st1.indexStr->size();a++)cout<<" <"<<st1.indexStr[0][a]<<">";
    //cout<<endl;
    //for(int a=0;a<st1.dataStr->size();a++)cout<<" <"<<st1.dataStr[0][a]<<">";
    //cout<<endl;
    //for(int a=0;a<st.len;a++)cout<<" "<<st[a]<<endl;
    //cout<<endl;
    
    
    
    //strW="བ་གཟུང་བ་ནི་རང་བཞིན་ཏེ་དེས་ན་གཉིས་སུ་མེཛ་པའི་བདེ་བ་ལ་སྤྱོད་པ་ན་བ/བ་གཟུང་བ་ནི་རང་བཞིན་ཏེ་དེས་ན་གཉིས་སུ་མེཛ་པའི་བདེ་བ་ལ་སྤྱོད་པ་ན་བ/";
    //strW="123456789qwerty";
    //cout<<"push_back name";
    //vector->push_back(strW,"qwerty");
    //string str;
    //cout<<"get name";
    //vector->getStr(str,"qwerty");
    //cout<<"vector->size()="<<vector->size()<<" str="<<str<<endl;
    
    //cout<<"start vectorPtr"<<endl;
    ///GStr<int>*vectorPtr=GStr<int>::create();
    //string str="1234567890";
    /*
     TIME_START
     char *buff=(char*)malloc(10000100);
     vector<int>v;
     TString st;
     uint index[1000010];
     GStr<uint>*ind=GStr<uint>::create();
     GStr<char>*dat=GStr<char>::create();
     for(int a=0;a<1000000;a++){ //cout<<"a="<<a<<endl;
     //TString st;
     //string str="1234567890";
     //sprintf(buff,"%d",a+256);
     //str+="1234567890";
     //v.push_back(a);
     //ind->push_back(a);
     //dat->push_Ptr(buff,10);
     //memcpy(buff+a*10,"1234567890",10);
     //index[a]=a;
     st.push_back(str);
     //str=st[0];
     }
     
     TIME_PRINT_    
     */
    
    //cout<<"st.indexStr->size()"<<st.indexStr->size()<<" len="<<st.len<<endl;
    //cout<<"st.dataStr->size()"<<st.dataStr->size()<<endl;
    
    // for(int a=0;a<10;a++){ cout<<"a="<<a;
    //  cout<<" . "<<st[a]<<endl;
    //}
    cout<<"done";
    
    //vectorPtr->push_back(a);
    //cout<<"vectorPtr->size()="<<vectorPtr->size()<<endl;
    
    
    //int a1=vectorPtr[0][10];
    //cout<<endl<<"@a1="<<a1<<endl;
    
    
    /*for(int a=0;a<vectorPtr->size();a++){
     char *buff; buff=(char*)vectorPtr->getPtr(a,10);
     string st; st.resize(10);
     memcpy(&st[0],buff,10);
     cout<<st<<endl;
     }
     */ 
    //cout<<"vector->size()="<<vector->size()<<endl;
    
    //for(int a=0;a<vector->size()/100;a++){vector->getStr(a,str);cout<<"a="<<a<<" str="<<str<<endl;}
    //vector->free();
    exit(0); 
}    

