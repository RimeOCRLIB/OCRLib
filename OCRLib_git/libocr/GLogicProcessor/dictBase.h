/*
 map<string,string> data;
 char buf[10];
 
 
 for(int i=0;i<1000000;i++){        
 sprintf (buf,"%d",i);
 //cout<<"buf="<<buf<<endl;
 
 data[buf]="HelloWorld:|:MessagePack:|:";
 }
 //sort(data.begin(),data.end());
 TIME_START
 for(int i=0;i<1000000;i++){   
 sprintf (buf,"%d",i);
 string value=data[buf];
 //binary_search (data.begin(), data.end(), buf);
 
 }    
 
 TIME_PRINT_
 
 //newg.print_();
 
 //exit(0);
 
 
 // Example: simple search for a string:
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
