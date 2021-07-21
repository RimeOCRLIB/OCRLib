#include "GFontEditor.h"

/** This function check correlation of test letter with all OCR font and return array of letters with close correlation
 */
void GFontEditor::testLetter(GLetter *letter, int mode){
    //letter->keyOCRVector->resize(0);
    //int print=0;
    vector<OCRMatch>letterLine;
    uint startIndex=0;
    testLetter(letter,letterLine,mode,&startIndex);
    
    string strStr,strDest;
    //strStr=letter->name;
    //strStr=logicProcessor->YagpoToUni(strStr);
    //letter->codeSpace=0;
    
    //cout<<"letterLine.size()="<<letterLine.size();
    if(letterLine.size()){
        ///string str=drawEditLetter(letterLine[0].idNumber);
        
        //sort(letterLine.begin(),letterLine.end(),sortMatchC);
        //letter->codeSpace=letterLine[0].correlation;
        //out<<"<script language=\"javaScript\">";
        int letterIndex=(int)letter->letterIndex;
        for(unsigned int i=0;i<letterLine.size()&&i<FSIZE;i++){
            
            //out<<"<a href=\"ocr.php?xml=<ocrData>edit</ocrData><index>"<<letterLine[i].letterIndex<<"</index>\", target=\"_blank\" ><font size=\"3\">"<<letterLine[i].name<<"</font>/"<<letterLine[i].correlation<<"</a>"<<endl;
            
            //keyOCR key;
            //key.letterIndex=letterLine[i].letterIndex;
            //key.correlation=letterLine[i].correlation;
            //letter->key[letter->keyCount_]=key;
            //letter->keyCount_++;
            //cout<<" c="<<(short)key.correlation<<" l="<<key.letterIndex;
            aliKali->cMatrix[0][FSIZE*letterIndex+letterLine[i].letterIndex]=(uchar)letterLine[i].correlation;
        }
    }
    
}//__________________________________________________________________________


//void GFontEditor::testLetter(GLetter *letter,int mode){
//    vector<OCRMatch>letterLine;
//    uint startIndex=0;
//    testLetter(letter,letterLine,mode,&startIndex);
//}
void GFontEditor::testLetter(GLetter *letter,vector<OCRMatch>&letterLine,int mode,uint *startIndex){
    
    int c1,c2;
    int print=0;
    //mode=0;  //print=2;
    
    string strStr,strDest;
    letter->codeSpace=0;
    vector<OCRMatch>letterLineNew;
    
    GBitmap *img128_letter;
    //if(aliKali[0][index]->newLetter)aliKali[0][index]->drawPict();img128_=aliKali[0][index]->pict;
    img128_letter=letter->drawPict();
    
    //imageEditor->WriteImageData(img128_,"/1.jpg",0);exit(0);
    
    GLetter *glyph_;
    GLetter *letter_copy;
    GBitmap *img128_glyph;
    
    
    if(letter->name==":)"||letter->name.find("*")!=-1){
        mode=3;
        //aliKali[0][index]->pict=aliKali[0][index]->mask128.unPackMask();
        //img128_=aliKali[0][index]->pict;
    }
    
    letter_copy=aliKali->getLetter(letter->letterIndex);
    if(mode!=3&&mode!=7&&mode!=9){
        c1=setCorrelation(img128_letter,letter_copy,1,0); letter->selfCorrelation=c1;
    }
    
    //int correlationCount=0;
    //first we check letter base -> letter corelation
    //когда мы тестируем новую букву то проверяем на какие буквы она похожа. Когда мы находим первую букву похожую на проверяемую
    //то все дальнейшие проверки мы проводим только с буквами, в которых первый признак похожей буквы присутствует с корреляцией более 80
    //в среднем это сокращает количество проверок вдвое
    //информация о буквах в которых присутствует первый признак, хранится в массиве maskID_0 каждой буквы
    //int maskID0_Flag=0;
    //int maskIDIndex=0;
    //int step=0;
    uint startIndex_=*startIndex;
    
    //cout<<"aliKali->letterCount()="<<aliKali->letterCount();
    
    for(unsigned int letterIndex=1;letterIndex<aliKali->letterCount();letterIndex++){
        
        if(letterIndex<*startIndex){                    //проверяем различение с прежде найденными буквами
            int flag=0;
            for(int i=0;i<letterLine.size();i++){
                if(letterIndex==letterLine[i].letterIndex){flag=1;break;}
            }
            if(flag==0)continue;
        }else{
            startIndex_=letterIndex;
        }
        //if(step==10){cout<<".";step=0;}step++;
        //if((letterIndex/100)*100==letterIndex)cout<<letterIndex<<" ";
        
        c1=0;
        //проверяем есть ли значение в таблице
        if(mode==9)c1=*(aliKali->cMatrix->dataPtr()+FSIZE*letterIndex+letter->letterIndex);
        c2=0;
        //проверяем есть ли значение в таблице
        if(mode==9)c2=*(aliKali->cMatrix->dataPtr()+FSIZE*letter->letterIndex+letterIndex);
        if(c1&&c2)continue;
        
        glyph_=aliKali->getLetter(letterIndex);
        DR2("name="<<glyph_->name<<" letterIndex="<<letterIndex<<" c1="<<c1<<" c2="<<c2<<endl);
        
        
        
        
        //if(!RE2::PartialMatch(glyph_->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-]+")||glyph_->OCRIndex==3){
        //    glyph_->destroy();continue;
        //}
        //if(!RE2::PartialMatch(glyph_->name,"[ಂ-೯\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-]+")){
        //    glyph_->destroy();continue;
        //}
        
        
        
        /*
         flag=0;	
         if(maskID0_Flag){//maskID0_Flag значит что мы нашли первую букву похожую на проверяемую. Дальше проверяет только похожие на нее буквы
         for(int a=0;a<aliKali[0][maskIDIndex]->maskId0Count;a++){
         if(aliKali[0][maskIDIndex]->maskId0[a]==aliKali[0][letterIndex]->idNumber){flag=1; break;}
         }
         }
         if(maskID0_Flag&&!flag)continue;
         */
        
        if(glyph_->name=="*"){glyph_->destroy();continue;}
        if(letterIndex==letter->letterIndex){glyph_->destroy();continue;}
        if(letter->name==glyph_->name&&(mode!=7&&mode!=9)){glyph_->destroy();continue;}
        
        
        //cout<<" letterIndex="<<letterIndex<<" newLetter="<<aliKali[0][letterIndex]->newLetter<<endl;
        //if(aliKali[0][letterIndex]->newLetter)
        //aliKali[0][letterIndex]->drawPict();img128=aliKali[0][letterIndex]->pict;
        //cout<<" l="<<letterIndex<<endl;
        img128_glyph=glyph_->drawPict();
        
        //cout<<"@1letterIndex="<<letterIndex<<" img128->columns()="<<img128->columns()<<endl;
        //cout<<" aliKali[0][index].mask32Count()="<<aliKali[0][index]->mask32Count()<<endl;
        
        letter_copy=aliKali->getLetter(letter->letterIndex);
        letter_copy->newLetter=(int)glyph_->letterIndex;
        //cout<<"m2 newLetter="<<glyph->newLetter;
        //cout<<" glyph[0].mask32Count()="<<glyph[0].mask32Count()<<endl;
        
        if(mode==1||mode==7||mode==9){  //для коренных букв считаем корреляцию тестовой буквы к букве в базе
            if(!c1){c1=setCorrelation(img128_glyph,letter_copy,1,0);}
            if(!c1)c1=1;
        }
        
        //if(c1>80){  ///@@key@@
        //    if(glyph_->maskId0Count>0){maskID0_Flag=1;maskIDIndex=letterIndex;
        //cout<<"maskIDIndex="<<maskIDIndex<<endl;
        //    }
        //}
        
        //cout<<" c1="<<c1<<endl;
        //if(glyph->mask32[0].correlation>80)correlationCount50++;
        //cout<<"c1="<<c1<<" glyph->maskID_0.size()="<<glyph->maskID_0.size()<<" id="<<glyph->maskID_0[glyph->maskID_0.size()-1]<<endl;
        
        //for(int a=0;a<glyph->maskId0Count;a++)aliKali[0][index]->maskId0[a]=glyph->maskId0[a];
        //aliKali[0][index]->maskId0Count=glyph->maskId0Count;
        
        if(mode){   //для тестовых и добавляемых в базу новых букв считаем корреляцию буквы базы к тестовой букве
                    //cout<<"m2";
            if(!c2)c2=setCorrelation(img128_letter,glyph_,1,0);
            if(!c2)c2=1;
            //if(c2>0)cout<<"@c2@="<<c2;
            if(c2>letter->correlation){
                letter->OCRKey=glyph_->letterIndex;
                DT2(" aliKali->OCRKey="<<glyph_->letterIndex<<endl);
                letter->correlation=c2;
                glyph_->correlation=c2;
            }
            DR2("c2="<<c2<<endl);
        }
        
        
        //cout<<"m3";
        
        if(c1>80&&mode<3){
            //glyph_->letterIndex=letterIndex;
            //cout<<"add "<<glyph_->name<<endl;
            OCRMatch match; match.copyData(glyph_);
            letterLineNew.push_back(match);
            if(c1>letter->codeSpace)letter->codeSpace=c1;
        }
        if(mode){
            if(mode==7||mode==9){  //для найденных букв считаем прямую корреляцию в режиме проверки базы
                aliKali->cMatrix[0][FSIZE*letterIndex+letter->letterIndex]=c1;
                aliKali->cMatrix[0][FSIZE*letter->letterIndex+letterIndex]=c2;
            }
            if(mode==5){
                OCRMatch match; match.copyData(glyph_);
                match.correlation=c2;
                letterLineNew.push_back(match);
                DR("add "<<glyph_->name<<glyph_->letterIndex<<" c2="<<c2<<endl)
            }
        }
        
        
        //cout<<"m4";
        DT2("name="<<letter_copy->name<<" correlation="<<c1<<" id="<<letter_copy->letterIndex<<" mC0="<<letter_copy->mask32[0].correlation<<endl);
        img128_glyph->destroy();
        letter_copy->destroy();
        glyph_->destroy();
        letter->lineCount=(uint)letterLineNew.size();
        if(letterLineNew.size()>20&&mode!=5&&mode!=7)break;
        
    }
    //вычисляем процент покрытия буквы масками признаков
    if(mode!=7&&mode!=9){
        GBitmap *outBitmap=letter->drawLetterPict(0);
        outBitmap->destroy();
    }
    img128_letter->destroy();
    
    letterLine.resize(0);
    cout<<"@"<<letter->letterIndex<<" "<<letter->name<<"@New letter line count"<<letterLineNew.size()<<" ";
    for(int i=0;i<letterLineNew.size();i++){
        letterLine.push_back(letterLineNew[i]);
        //cout<<letterLineNew[i].name<<" ";
    }cout<<endl;
    
    
    
    *startIndex=startIndex_;
    
    //aliKali->saveLetter(letter);
    
    //cout<<"//__name="<<aliKali[0][index]->name<<" codeSpace="<<aliKali[0][index]->codeSpace<<" selfCorrelation="<<aliKali[0][index]->selfCorrelation<<
    //" correlation="<<aliKali[0][index]->correlation<<" correlationCount50="<<aliKali[0][index]->maskId0Count<<" letterLine.size()="<<
    //letterLine.size()<<endl;
    
    //for(int i=0;i<aliKali[0][index]->maskID_0.size();i++)cout<<" aliKali[0][index]->maskID_0["<<i<<"]="<<aliKali[0][index]->maskID_0[i]<<endl;
    
    //for(int i=0;i<aliKali[0][index]->maskID_0.size();i++)cout<<" aliKali[0][index]->maskID_0["<<i<<"]="<<aliKali[0][index]->maskID_0[i]<<" aliKali[0]["<<aliKali->indexById(aliKali[0][index]->maskID_0[i])<<"].name="
    //        <<aliKali[0][aliKali->indexById(aliKali[0][index]->maskID_0[i])]->name<<endl;
    
    
}//__________________________________________________________________________

/** проверяет корреляцию буквы среди массива букв похожих на проверяемую */
void GFontEditor::testLetterInLine(GLetter *letter,vector<OCRMatch>&letterLine,vector<OCRMatch>&letterLineNew){
    
    int c1=0;
    int print=0;
    
    string strStr,strDest;
    letter->codeSpace=0;
    
    letterLineNew.resize(0);
    //cout<<"test name"<<letterLine.size()<<"=";
    //for(int letterIndex=0;letterIndex<letterLine.size();letterIndex++){
    //    cout<<letterLine[letterIndex]->name<<" ";
    //}
    
    //first we check letter base -> letter corelation
    for(unsigned int letterIndex=0;letterIndex<letterLine.size();letterIndex++){
        
        GBitmap *img128;
        //if(letterLine[letterIndex]->newLetter){letterLine[letterIndex]->drawPict();}img128=letterLine[letterIndex]->pict;
        img128=aliKali->drawPict(letterIndex);
        
        //cout<<"test name="<<letterLine[letterIndex]->name<<endl;
        
        //WriteImageData(img128, "/1.jpg", 0);
        //if(letterIndex!=index)continue;
        //cout<<"m1";
        GLetter *glyph=aliKali->getLetter(letter->letterIndex);
        GLetter *glyph_=aliKali->getLetter(letterIndex);
        
        //cout<<"m2";
        if(letter->name==glyph_->name)continue;
        
        c1=setCorrelation(img128,glyph,1,0); letterLine[letterIndex].correlation=c1;
        DR("m3 c1="<<c1<<" name="<<glyph_->name<<endl)
        if(c1>80){
            OCRMatch match;
            match.copyData(glyph_);
            letterLineNew.push_back(match);
            match.correlation=c1;
            if(c1>letter->codeSpace)letter->codeSpace=c1;
        }
        //cout<<"m4";
        DR("name="<<letter->name<<" correlation="<<c1<<" letterLineNew="<<letterLineNew.size()<<endl)
        img128->destroy();
        glyph->destroy();
    }
    
    //вычисляем процент покрытия буквы масками признаков
    GBitmap *outBitmap=letter->drawLetterPict(0);
    outBitmap->destroy();
    
    
    
    
}//__________________________________________________________________________

/** This function check correlation of all letters in OCR font with all OCR font and set corelation value in OCR font
 */
void GFontEditor::testBase(){
    
    memset(aliKali->cMatrix->dataPtr(),0,FSIZE*FSIZE);
    
#ifdef FORK
    StartCpuInfo();
    cout<<"inputData.num_cores="<<inputData.num_cores<<endl;
    int maxFork=inputData.num_cores;
    pidID *pidIDArray;
    
    
    MemoryFile *pidData_mf;  //main file for conection with child process
    pidData_mf=MemoryFile::create(inputData.data["statPath"].c_str(), MemoryFile::if_exists_keep_if_dont_exists_create);
    pidData_mf->resize(sizeof(pidID)*maxFork);
    pidIDArray=(pidID*)pidData_mf->data(); //array which can be share between processes.
    
    for(int index=0;index<maxFork;index++)pidIDArray[index].status=0;
    //for(int index=0;index<maxFork;index++)cout_<<pidIDArray[index].status;
    
    
    int ID=0;
    unsigned int i=1;
    int countFork=0;
    int status;
    
    while(i<aliKali->letterCount()){
        
        DR_("NEW START "<<i<<endl);
        GLetter *letter=aliKali->getLetter(i);
        
        //if(!RE2::PartialMatch(letter->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-]+")||letter->OCRIndex==3){
        //    letter->destroy();i++;continue;
        //}
        /*
         // if(!RE2::PartialMatch(letter->name,"[ಂ-೯\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-]+")||letter->OCRIndex==3){
         letter->destroy();i++;continue;
         }
         */
        int pid = fork();
        pidIDArray[ID].status=1;
        if (pid < 0){
            cout<<"ERROR on fork ";
        }
        if (pid == 0)  {
            testLetter(letter,9);
            aliKali->saveLetter(letter);
            pidIDArray[ID].status=0;
            exit(0);
        }
        letter->destroy();
        
        countFork++;
        i++;
        if(i==inputData.fileList.size())break;
        
        cout<<" pidIDArray["<<ID<<"].status= "<<pidIDArray[ID].status<<" >> ";
        for(int index=0;index<maxFork;index++)cout<<pidIDArray[index].status<<" ";
        cout<<endl;
        
        ID=100;
        while(ID==100){
            if(countFork<maxFork-1)break;             //есть свободные слоты для новых процессов
            int forkStatusCount=0;
            for(int index=0;index<maxFork;index++){   //подсчитываем количество активных процессов
                if(pidIDArray[index].status==1)forkStatusCount++;
            }
            if(forkStatusCount==countFork){sleep(1);continue;}     //нет завершенных процессов
            wait(&status);                           //регистрируем с системе завершенный процесс
            countFork--;
            for(int index=0;index<maxFork;index++){  //маркируем слот как свободный
                                                     //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
                if(pidIDArray[index].status==0){
                    
                    ID=index;
                    break;
                }
            }
            
        }
    }
#else
    unsigned int i=0;
    cout<<"aliKali->letterCount()="<<aliKali->letterCount()<<endl;
    while(i<aliKali->letterCount()){
        GLetter *letter=aliKali->getLetter(i);
        cout<<"NEW START "<<i<<" n="<<letter->name<<endl;
        
        letter->keyCount_=0;
        testLetter(letter,9);
        aliKali->saveLetter(letter);
        letter->destroy();
        i++;
        
    }
    
#endif
    
    
}//__________________________________________________________________________

void GFontEditor::rebuildBaseIndex(){
    
    //int letterIndex=0;
    // GFont *aliKali_=GFont::create();
    /*
     for(int index=0;index<aliKali_->letterCount();index++){
     aliKali_[0][index]->recordFlag=1;
     }
     
     for(int index=0;index<aliKali->letterCount();index++){
     
     if(aliKali[0][index]->name!=":)"&&aliKali[0][index]->name.find("*")==-1&&aliKali[0][index]->OCRIndex!=3){  //для коренных букв подбираем из базы все буквы похожие на коренную
     
     //cout<<" aliKali[0]["<<index<<"]->name="<<aliKali[0][index]->name<<" OCRKey="<<aliKali[0][index]->OCRKey<<" correlation="<<aliKali[0][index]->correlation<<" idNumber="<<aliKali[0][index]->idNumber<<endl;
     
     aliKali_->letterSet[letterIndex]=aliKali[0][index];
     aliKali[0][index]->recordFlag=0;
     letterIndex++;
     
     for(int a=0;a<aliKali->letterCount();a++){
     if(aliKali[0][a]->name!=":)"&&aliKali[0][a]->name.find("*")==-1)continue;
     //cout<<"______letterIndex="<<letterIndex<<" aliKali[0]["<<a<<"]="<<aliKali[0][a]->name<<" correlation="<<aliKali[0][a]->correlation<<" OCRKey="<<aliKali[0][a]->OCRKey<<endl;
     if(aliKali[0][a]->OCRKey==aliKali[0][index]->idNumber&&aliKali[0][a]->recordFlag&&aliKali[0][a]->correlation<196){
     
     aliKali[0][a]->name=aliKali[0][index]->name;
     aliKali[0][a]->name+="*";
     aliKali_->letterSet[letterIndex]=aliKali[0][a];
     aliKali[0][a]->recordFlag=0;
     letterIndex++;
     //cout<<"match";
     }
     }
     }
     
     
     }
     
     //добавляем неразобранные буквы с низкой корреляцией
     for(int index=0;index<aliKali->letterCount();index++){
     if(aliKali[0][index]->recordFlag&&aliKali[0][index]->correlation<96&&aliKali[0][index]->name==":)"){
     aliKali_->letterSet[letterIndex]=aliKali[0][index];
     aliKali[0][index]->recordFlag=0;
     letterIndex++;
     
     }
     }
     
     for(int index=0;index<aliKali_->letterCount();index++){
     aliKali_[0][index]->recordFlag=1;
     }
     //cout<<"//____aliKali_.cout="<<aliKali_->letterCount<<endl;
     aliKali=aliKali_;
     */
    
    
}


int GFontEditor::setCorrelation(GBitmap *img128, GLetter *glyph,float scale,int print){  //function need copy of actual letter. It store OCR information
    int correlation;
    GBitsetMatrix *setMatrix;
    //GBitmap *img,*img_;
    
    /*
     if(scale<0.1||scale>10)return 0;
     
     if(scale!=1){
     img=GBitmap::createScale(img128, scale);
     //img->printToScreen(); exit(0);
     img_=GBitmap::create(img->columns()+60,img->rows()+120);
     img_->fill(255);
     img_->drawImg(img,30,60);
     img->destroy();
     }else{
     img_=GBitmap::create(img128->columns()+60,img128->rows()+120);
     img_->fill(255);
     img_->drawImg(img128,30,60);
     }
     */
    
    if(!img128->columns()||!img128->rows()||
       img128->columns()<10||img128->columns()>600||
       img128->rows()<10||img128->rows()>600){
        cout<<"@@@_errror_img128->columns()="<<img128->columns()<<" img128->rows()="<<img128->rows()<<endl;
        return 0;
    }
    setMatrix=GBitsetMatrix::createRegion(img128,
                                          0,
                                          0,
                                          img128->columns(),
                                          img128->rows(),
                                          1,1,
                                          IMGNOFLIP);
    
    
    //cout<<"c2";
    
    correlation=setMatrix->letterToLetterCorrelation (glyph,
                                                      0,
                                                      img128->rows(),
                                                      1,
                                                      print);
    
    
    //if(print){
    //string str="/1.jpg";
    //WriteImageData(setMatrix->drawDataRGB,str,1);
    //}
    /* */  setMatrix->destroy();
    //img_->destroy();
    
    //cout<<"c3";
    return correlation;
}//____________________________________________________________________________





