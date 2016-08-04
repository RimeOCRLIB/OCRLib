#include "config.h"

#include "OCRTypes.h"
#include "GLetter.h"
#include <string.h>
#include <fstream>
#include "php2stl.h"
#include "GContour.h"
#include "GImageEditor.h"

//________vectorOCR_________________
vectorOCR::~vectorOCR(){}

void vectorOCR::destroy(void){}

vectorOCR::vectorOCR(){
	selectFlag=0;
}

//________wordOCR_________________
wordOCR::~wordOCR(){
}

void wordOCR::destroy(void){
    if(imgFlag){
        img->destroy();
    }
    
}
//________blockOCR_______________
blockOCR::~blockOCR(){}

void blockOCR::destroy(void){}

blockOCR::blockOCR(void){}
/**/
//________lineOCR_______________
lineOCR::~lineOCR(){}

void lineOCR::destroy(void){}

lineOCR::lineOCR(void){}
/**/

//________columnOCR_______________
columnOCR::~columnOCR(){}

void columnOCR::destroy(void){}

columnOCR::columnOCR(void){}
//________recordOCR_______________

recordOCR::~recordOCR(){}

void recordOCR::destroy(void){}

recordOCR::recordOCR(void){}
/**/
//________frameOCR_______________
frameOCR::~frameOCR(void){}
void frameOCR::destroy(void){}

frameOCR::frameOCR(){}

//_______ drawDataRecord__________
//drawDataRecord::~drawDataRecord(void){}

//void drawDataRecord::destroy(void){}

drawDataRecord::drawDataRecord(){}


//________TsertoOCR_________________
TsertoOCR::TsertoOCR(){};


wordOCR::wordOCR(void){
    delimeter="";
    name="";
    newLine="";
    commentaryIndex="";
    imgFlag=0;
}


bool  wordOCR::operator==(vector<string> &ref){
    for(int a=0;a<ref.size();a++){
        if(ref[a]==name)return true;
    }
    return false;
	
}

void  wordOCR::operator=(wordOCR *ref){
    x0=ref->x0;
    y0=ref->y0;
    x1=ref->x1;
    y1=ref->y1;
    xp0=ref->xp0;
    yp0=ref->yp0;
    xp1=ref->xp1;
    yp1=ref->yp1;
    xp=ref->xp;
    yp=ref->yp;
    xt0=ref->xt0;
    yt0=ref->yt0;
    xt1=ref->xt1;
    yt1=ref->yt1;
    w=ref->w;
    h=ref->h;
    xv0=ref->xv0;
    yv0=ref->yv0;
    xv1=ref->xv1;
    yv1=ref->yv1;
    center=ref->center;
    name=ref->name;
    Wylie=ref->Wylie;
    html=ref->html;
    delimeter=ref->delimeter;
    oldName=ref->oldName;
    newLine=ref->newLine;
    file=ref->file;
    commentaryIndex=ref->commentaryIndex;
    sillablyCount=ref->sillablyCount;
    letterIndex=ref->letterIndex;
    lineIndex=ref->lineIndex;
    letterID=ref->letterID;
    img=ref->img;
    correlation=ref->correlation;
    tsertoFlag=ref->tsertoFlag;
    spaceFlag=ref->spaceFlag;
    stackFlag=ref->stackFlag;
    vowelFlag=ref->vowelFlag;
    Param1=ref->Param1;
    Param2=ref->Param2;
    selectFlag=ref->selectFlag;
    blockIndex=ref->blockIndex;
    OCRIndex=ref->OCRIndex;
    fontSize=ref->fontSize;
    checkFlag=ref->checkFlag;
    imgFlag=ref->imgFlag;
    id=ref->id;
}

bool wordOCR::testDelimeter(vector<string> &ref){
    for(int a=0;a<ref.size();a++){
        if(ref[a]==delimeter||ref[a]==name)return true;
    }
    return false;
}


GString::GString(){
    len=0;
};

GString::~GString(){};

void GString::operator=(const GString ref){
    len=0;
    for(int a=0;a<ref.len&&a<128;a++){
        data[a]=ref.data[a];
        len++;
    }
    data[len]=0;
}
void GString::operator=(const char *ref){
    len=0;
    while(ref[len]&&len<128){
        data[len]=ref[len];
        len++;
    }
    data[len]=0;
}
void GString::operator=(string &ref){
    len=0;
    for(int a=0;a<ref.size()&&a<128;a++){
        data[a]=ref[a];
        len++;
    }
    data[len]=0;
}
bool GString::operator==(const GString ref){
    if(len!=ref.len)return 0;
    for(int a=0;a<ref.len&&a<128;a++){
        if(data[a]!=ref.data[a])return 0;
    }
    return 1;
    
}
bool GString::operator==(const char *ref){
    int len_=0;
    while(ref[len_]&&len_<128){
        if(data[len_]!=ref[len_])return 0;
        len_++;
    }
    if(len!=len_)return 0;
    return 1;
    
}
bool GString::operator==(string &ref){
    for(int a=0;a<ref.size()&&a<128;a++){
        if(data[a]!=ref[a])return 0;
    }
    if(len!=ref.size())return 0;
    return 1;
}

bool GString::operator!=(const GString ref){
    for(int a=0;a<ref.len&&a<128;a++){
        if(data[a]!=ref.data[a])return 1;
    }
    if(len!=ref.len)return 1;
    return 0;
    
}
bool GString::operator!=(const char *ref){
    int len_=0;
    while(ref[len_]&&len_<128){
        if(data[len_]!=ref[len_])return 1;
        len_++;
    }
    if(len!=len_)return 1;
    return 0;
    
}
bool GString::operator!=(string &ref){
    for(int a=0;a<ref.size()&&a<128;a++){
        if(data[a]!=ref[a])return 1;
    }
    if(len!=ref.size())return 1;
    return 0;
}

void GString::operator+(const char *ref){
    int len_=0;
    while(ref[len_]&&len<128){
        data[len]=ref[len_];
        len_++;
    }
    
};
void GString::operator+=(const char *ref){
    int len_=0;
    while(ref[len_]&&len<128){
        data[len]=ref[len_];
        len_++;
    }
    
};


inline char*  GString::operator[] (int id){
    if (id<0 || id>=len) {
        return  data;
    }
    return &data[id];
};

int GString::find(const char *ref){
    string str=data;
    return (int)str.find(ref);
    
};
int GString::find(string &ref){
    string str=data;
    return (int)str.find(ref);
};


//________stringOCR_______________
stringOCR::~stringOCR(void){
    //destroy();
}

void stringOCR::destroy(void){
    if(imgFlag){
        img->destroy();
    }
    wordArray.resize(0);
    
}

stringOCR::stringOCR(){
    x0=0;x1=0;y0=0;y1=0;
    count=0;
    imgFlag=0;
}

int stringOCR::length(){
    return count;
}
int stringOCR::size(){
    return count;
}//_______________________________________________

void stringOCR::push_back(const wordOCR &word){
    string str_=str_replace("/", "", word.name.c_str());
    name+=str_+word.delimeter+word.commentaryIndex+word.newLine; //cout<<" str_="<<str_;
    wordArray.push_back(word);
    if(str_!=word.name){wordArray[count].fontSize=6;}else{wordArray[count].fontSize=14;}
    wordArray[count].name=str_;
    str+=str_;
    strHTML+="<c id=\"";
    char d[20];
    memset(d,0,20);
    sprintf(d, "%d",word.id);
    strHTML+=d; strHTML+="\">"+str_+"</c>";   
    if(word.newLine!=""){x0=10000;y0=10000;x1=0;y1=0;}
    
    if(wordArray[count].x0>0&&wordArray[count].x0<x0)x0=wordArray[count].x0;
    if(wordArray[count].y0>0&&wordArray[count].y0<y0)y0=wordArray[count].y0;
    if(wordArray[count].x1>0&&wordArray[count].x1>x1)x1=wordArray[count].x1;
    if(wordArray[count].y1>0&&wordArray[count].y1>y1)y1=wordArray[count].y1;
    
    count++;
}//_______________________________________________

void stringOCR::insert(int index, wordOCR &word){
    string str_=str_replace("/", "", word.name.c_str());
    wordArray.insert(wordArray.begin()+index,word);
    count=wordArray.size();
    name="";
    for(int a=0;a<wordArray.size();a++){
        name+=wordArray[a].name+wordArray[a].delimeter+wordArray[a].commentaryIndex+wordArray[a].newLine;
    }
}

wordOCR*  stringOCR::operator[](int index){
    if (index<0 || index>=count) {
        return &wordArray[0];
    }
    return &wordArray[index];
}//_______________________________________________


void  stringOCR::operator+=(stringOCR& ref){
    for(int a=0;a<ref.size();a++){
        string str_=str_replace("/", "", ref[a]->name.c_str());
        name+=str_;
        wordArray.push_back(*ref[a]);
        if(str_!=ref[a]->name){wordArray[count].fontSize=6;}else{wordArray[count].fontSize=14;}
        wordArray[count].name=str_;
        
        str+=str_;
        strHTML+="<c id=\"";
        char d[10];
        memset(d,0,10);
        sprintf(d, "%d",ref[a]->id);
        strHTML+=d; strHTML+="\">"+str_+"</c>";
        
        if(wordArray[count].x0>0&&wordArray[count].x0<x0)x0=wordArray[count].x0;
        if(wordArray[count].y0>0&&wordArray[count].y0<y0)y0=wordArray[count].y0;
        if(wordArray[count].x1>0&&wordArray[count].x1>x1)x1=wordArray[count].x1;
        if(wordArray[count].y1>0&&wordArray[count].y1>y1)y1=wordArray[count].y1;
        
        count++;
        delimeter=ref.delimeter;
		
    }
	
}//_______________________________________________

void  stringOCR::operator+=(wordOCR ref){
    wordArray.push_back(ref);
}

int stringOCR::find(const string& match,int position){
    string str_;
    string matchStr=match;
    
    //cout_<<"match="<<match<<" position="<<position<<" count="<<count<<endl;
    
    for(int a=position;a<count;a++){
        str_=wordArray[a].name; //cout_<<"str_="<<str_<<" a="<<a<<endl;
        if(match.find(str_,0)!=0){
            //cout_<<"(-)";
        }else{
            //cout_<<"(+)";
            matchStr=match.substr(str_.length(),match.length());
            //cout_<<" matchStr="<<matchStr<<endl;
            if(matchStr.length()==0)return a;
        }
    }
    return -1;
}//_______________________________________________

stringOCR stringOCR::substr(int position,int length){
    stringOCR str_;
    for(int a=position;a<position+length&&a<count;a++){
        str_.push_back(wordArray[a]);
    }
    //cout_<<" str_="<<str_.str<<" position="<<position<<" length="<<length<<" count="<<count<<endl;
    return str_;
	
}//_______________________________________________

void stringOCR::resize(int length){
    wordArray.resize(length);
    count=length;
    if(length==0){
        str="";
        name="";
        strHTML="";
        x0=100000;y0=100000;x1=0;y1=0;
		
    }
}//_______________________________________________

void stringOCR::clearOCR(){
    for(int a=0;a<wordArray.size();a++){
        wordArray[a].name="*";
    }
}//_______________________________________________


void stringOCR::set_id(string &destStr){
    //на вход функции приходит строка ответа базы данных с расставленными слогами.
    //как выход функции мы получаем строку в которой на концах строки сохранена пунктуация исходной строки и расставлена пунктуация словаря
    
    string strReport=destStr,st; destStr="";
    int startIndex=0,indexSrc;
    //char d[20];
    int print=GRAMMAR_LOG;
    DT("///_________set id strReport_="<<strReport<<"/ str=/"<<str<<"/"<<" count="<<count<<END);
	
    //for(int a=0;a<wordArray.size();a++){
    //    cout_<<"w["<<a<<"]="<<wordArray[a].name<<" d="<<wordArray[a].delimeter<<" newLine="<<wordArray[a].newLine<<endl;
    //}
    
    
    while(startIndex<count){
        DT("wordArray["<<startIndex<<"].name="<<wordArray[startIndex].name<<" wordArray["<<startIndex<<"].delimeter="<<wordArray[startIndex].delimeter<<endl);
        if(wordArray[startIndex].delimeter==""){ DT("@/l/");
            indexSrc=(int)strReport.find(wordArray[startIndex].name,0);
            if(indexSrc!=string::npos){
                
                if(indexSrc!=0&&count>DICT_REPORT_SIZE)destStr+=strReport.substr(0,indexSrc); //добавляем разбиение на слоги по словарю только в длинных строках
                
                destStr+=wordArray[startIndex].name+wordArray[startIndex].commentaryIndex+wordArray[startIndex].newLine;//+"</c>";
                strReport=strReport.substr(indexSrc+wordArray[startIndex].name.size(),strReport.length());
                if(strReport.find(wordArray[startIndex].delimeter,0)==0)strReport=strReport.substr(wordArray[startIndex].delimeter.size(),strReport.size()-wordArray[startIndex].delimeter.size());
                DT("wordArray["<<startIndex<<"].commentaryIndex="<<wordArray[startIndex].commentaryIndex<<endl);
                if(wordArray[startIndex].commentaryIndex!="")if(strReport.find(wordArray[startIndex].commentaryIndex,0)==0){
                    DT("find commentary strReport1="<<strReport<<endl);
                    strReport=strReport.substr(wordArray[startIndex].commentaryIndex.size(),strReport.size()-wordArray[startIndex].commentaryIndex.size());
                    DT("find commentary strReport2="<<strReport<<endl);
                }
                if(wordArray[startIndex].newLine!="")if(strReport.find(wordArray[startIndex].newLine,0)==0){
                    DT("find commentary strReport1="<<strReport<<endl);
                    strReport=strReport.substr(wordArray[startIndex].newLine.size(),strReport.size()-wordArray[startIndex].newLine.size());
                    DT("find commentary strReport2="<<strReport<<endl);
                }
            }//else{destStr+="<b>ALERT</b>";}
        }else{ DT("@/cd/");
            
            
            //if(startIndex+1==count||count<=DICT_REPORT_SIZE)
            destStr+=wordArray[startIndex].delimeter;
            //if(count>DICT_REPORT_SIZE)destStr+=wordArray[startIndex].delimeter;
            
            //if(wordArray[startIndex].delimeter!="་"){
            if(strReport.find("་",0)==0){
                strReport=strReport.substr(3,strReport.size()-3);
                //if(startIndex+1!=count&&wordArray[startIndex].delimeter=="་")destStr+="་";
            }
            //}
            
            destStr+=wordArray[startIndex].commentaryIndex+wordArray[startIndex].newLine;//+"</c>";
            DT("wordArray["<<startIndex<<"].commentaryIndex="<<wordArray[startIndex].commentaryIndex<<endl);
            if(wordArray[startIndex].commentaryIndex!="")if(strReport.find(wordArray[startIndex].commentaryIndex,0)==0){
                DT("find commentary strReport1="<<strReport<<endl);
                strReport=strReport.substr(wordArray[startIndex].commentaryIndex.size(),strReport.size()-wordArray[startIndex].commentaryIndex.size());
                DT("find commentary strReport2="<<strReport<<endl);
            }
            if(wordArray[startIndex].newLine!="")if(strReport.find(wordArray[startIndex].newLine,0)==0){
                DT("find commentary strReport1="<<strReport<<endl);
                strReport=strReport.substr(wordArray[startIndex].newLine.size(),strReport.size()-wordArray[startIndex].newLine.size());
                DT("find commentary strReport2="<<strReport<<endl);
            }
            
            
        }
        //destStr+=wordArray[startIndex].newLine;
        DT("wordArray["<<startIndex<<"].name="<<wordArray[startIndex].name<<
           "wordArray["<<startIndex<<"].delimeter="<<wordArray[startIndex].delimeter<<
           "wordArray["<<startIndex<<"].newLine="<<wordArray[startIndex].newLine<<
           "wordArray["<<startIndex<<"].commentaryIndex="<<wordArray[startIndex].commentaryIndex<<endl<<
           " indexSrc="<<indexSrc<<"strReport="<<strReport<<" destStr="<<destStr<<endl;)
        
        startIndex++;
        
    }
    
    //destStr+=strReport; //rest of string can has newLine
}//_______________________________________________


// ----- constructor and destructor
//________GLetter_________________
GLetter::~GLetter(){};

void GLetter::destroy(){
    mask32Vector->free();
    focalPoint->free();
    delete this;
}

GLetter::GLetter(){
    vectorCount=0;
    contourCount=0;
    y0=0;
    y1=0;
    dX=0;
    dY=0;
    OCRIndex=0;
    recordFlag=1;
    newLetterInBook=1;
    newLetter=1;
    cn=0;
    cnRes=0;
    crSum=0;
    crRes=0;
    vectorCount=0;
    selfCorrelation=0;
    codeSpace=0;
    pictFlag=0;
    mask32Vector=GStr<GBitMask32>::create(24576);
    mask32=(GBitMask32*)mask32Vector->dataPtr();
    focalPoint=GStr<OCRPoint>::create(128);
    fPoint=(OCRPoint*)focalPoint->dataPtr();
    //focalLine=GStr<OCRFocalLine>::create(128);
    name="";
    OCRKey="";
}

int operator==(const GLetter &left, const GLetter &right){
	return  left.correlation==right.correlation;
}

int operator<(const GLetter &left, const GLetter &right){
	return  left.correlation>right.correlation;
}

void GLetter::eraseMask(int id){
    //cout_<<"need erase id="<<id<<" mask32Count="<<mask32Count<<endl;
    if(id>=mask32Count())return;
    if(id<0)return;
    mask32[id].status=-1;
    eraseEmptyMask();
    
}//_____________________________________________________________________________



void GLetter::copy(GLetter* letter){
    TString st;
	letter->writeToStr(&st);
    mask32Vector->resize(0);
    readFromStr(&st);
	
}//_____________________________________________________________________________


void GLetter::reloadMask(){
    int border=128;
    GBitmap *img=mask128.unPackMask();
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(img,"/3mask.jpeg",0); 
    
	GBitmap *destImg=GBitmap::create(mask128.mW+2*border,mask128.mH+2*border);
    int imgX0Point=mask128.mW/2+border;
    int imgY0Point=mask128.mH/2+border;
	destImg->drawImg(img, border, border);
    ((GImageEditor*)inputData.imageEditor)->WriteImageData(destImg,"/_Image2OCR/31mask.jpeg",0);
    
    int print=0;
	DR("@@mask32Count="<<mask32Count()<<endl)
	int x0,y0,w,h;

    
	for(int m=0; m<mask32Count();m++){

        DR("xMask="<<mask32[m].xMask<<"yMask="<<mask32[m].yMask<<endl);
        DR("mask128.xMask="<<mask128.xMask<<" mask128.mW/2="<<mask128.mW/2<<endl)
        
        //смещение возникает из-за того что mask128 установлена не по геометрическому центру буквы,
        //а по центру области распознавания буквы. Маски также отсчитываются от центра области распознавания буквы
        int dX_=-(mask128.xMask+mask128.mW/2);
        int dY_=-(mask128.yMask+mask128.mH/2);
        
		x0=mask32[m].xMask+dX_;
		y0=mask32[m].yMask+dY_;
        w=mask32[m].imgW;
		h=mask32[m].mH;
        
        if(w<4||h<4){
            DR("@@@No VALID MASK "<<m<<"in glyph"<<name<<endl) mask32[m].status=0;continue;
        }
        
        w=w/2; w=w*2; //if(w>32)w=32;  //приводим к чётным значениям. Нужно для функций ассемблера
        h=h/2; h=h*2;
        
        mask32[m].imgW=w;
		mask32[m].mH=h;
        
		DR("m="<<m<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" dlt1="<<mask32[m].dlt1<<" dX="<<dX<<" dY="<<dY<<" dX_="<<dX_<<endl)

        GBitmap *imgMask=GBitmap::createRegion(destImg,imgX0Point+x0,imgY0Point+y0,w,h);
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(imgMask,"/_Image2OCR/31111.jpeg",0);
        
        DR("m="<<m<<endl)
        imgMask->printToScreenBinary();
        DR("imgMask->columns()="<<imgMask->columns()<<" imgMask->rows()="<<imgMask->rows()<<" status="<<mask32[m].status);
        mask32[m].packBitMask(imgMask);
        DR(" mask32["<<m<<"].NMask="<<mask32[m].NMask<<endl)
        if(mask32[m].NMask<10){
            mask32[m].status=0;
        }else{
            mask32[m].status=1;
        }
        
        imgMask->destroy();
	}
	destImg->destroy();
    eraseEmptyMask();
    DR("done mask32Count()="<<mask32Count());
};


void GLetter::reloadMask(int m){
    //cout_<<"reload mask scale="<<scale<<endl;
	int border=128;
    GBitmap *img=mask128.unPackMask();
    
	GBitmap *destImg=GBitmap::create(mask128.mW+2*border,mask128.mH+2*border);
	destImg->drawImg(img, border, border);
	///img->printToScreen();
    int print=0;
	DT("mask32Count="<<mask32Count()<<" mask32["<<m<<"].imgW="<<mask32[m].imgW<<" =mH"<<mask32[m].mH<<" dX="<<dX<<" dY="<<dY<<endl);
	//destImg->printToScreen();
	int x0,y0,w,h;
	//cout_<<" done"<<endl;
	//int maskXMin=512,maskYMin=512;
    
    x0=mask32[m].xMask;
    y0=mask32[m].yMask;
    w=mask32[m].imgW;
    h=mask32[m].mH;

    if(w>4&&h>4){
        
        
        w=w/2; w=w*2; //if(w>32)w=32;  //приводим к чётным значениям. Нужно для функций ассемблера
        h=h/2; h=h*2;
        
        mask32[m].imgW=w;
        mask32[m].mH=h;
        
        DT("m="<<m<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl);
        if(destImg->columns()/2+x0<0||
           destImg->columns()/2+x0+w>destImg->columns()||
           w<0||
           destImg->rows()/2+y0<0||
           destImg->rows()/2+y0+h>destImg->rows()||
           h<0){
            DT("@No valig mask "<<m<<"in glyph"<<name<<endl;)
            mask32[m].status=0;
            DT("m="<<m<<"x0="<<x0<<" y0="<<y0<<" w="<<destImg->columns()<<" h="<<destImg->rows()<<endl);
            
        }else{
            //GBitmap *imgMask=GBitmap::create(destImg,destImg->columns()/2+x0+2,destImg->rows()/2+y0+1,w,h);
            GBitmap *imgMask=GBitmap::createRegion(destImg,destImg->columns()/2+x0+dX,destImg->rows()/2+y0+dY,w,h);
            DR("m="<<m<<endl)
            //imgMask->printToScreenBinary();
            DR("imgMask->columns()="<<imgMask->columns()<<" imgMask->rows()="<<imgMask->rows())
            mask32[m].packBitMask(imgMask);
            DR(" mask32["<<m<<"].NMask="<<mask32[m].NMask<<endl)
            if(mask32[m].NMask<10)mask32[m].status=0;
            
            imgMask->destroy();
        }
    }
	destImg->destroy();
	
	DT("done");
};


void GLetter::calibrateMask(GBitmap *img){
    
};


void GLetter::eraseEmptyMask(){
	//Erase empty mask;
	
	int index=0;
	for(int i=0;i<mask32Count();i++){
		if(mask32[i].status>0&&mask32[i].NMask>3&&mask32[i].NMask<2048){
			mask32[index]=mask32[i];
			mask32[index].status=mask32[i].status;
		    index++;
		}
	}
    mask32Vector->resize(index);
}
void GLetter::maskNormalisation(){
    int flag=0;
    for(int i=0;i<mask32Count();i++){
        int mH_=(mask32[i].mH/2)*2;  //проверка четности
        if(mask32[i].mH!=mH_)flag=1;
    }
    if(flag)reloadMask();
}



void GLetter::maskFromVector(){
    
	int index;
	int maxL;
    
	//cout_<<"@xMin="<<xMin<<" yMin="<<yMin<<" xMax="<<xMax<<" yMax="<<yMax<<" textLineSize="<<textLineSize<<endl;
	int sizeDL=textLineSize/15; ///sizeDL - we spread Mask box around vector
    
	for(int i=0;i<vectorCount;i++){
		maxL=0; index=-1;
		for(int j=0;j<vectorCount;j++){
			if(v[j].selectFlag)continue;
			if(v[j].length>maxL){maxL=v[j].length;index=j;}
		}
        GBitMask32 mask;
		//cout_<<"index="<<index<<endl;
		if(index==-1)break;
        v[index].selectFlag=1;
		
		int dL=v[index].length/10;
		if(dL>textLineSize/2)dL=textLineSize/2;
		//if(dL<textLineSize/5)dL=textLineSize/3;
		mask.xMask=v[index].vStart.x-dL;
		if(v[index].vStart.x<v[index].vEnd.x){
			mask.xMask=v[index].vStart.x-dL;
		}else{
			mask.xMask=v[index].vEnd.x-dL;
		}
		
        
		if(mask.xMask<xMin-sizeDL)mask.xMask=xMin-sizeDL;
		
		if(v[index].vStart.y<v[index].vEnd.y){
		    mask.yMask=v[index].vStart.y-dL;
		}else{
		    mask.yMask=v[index].vEnd.y-dL;
		}
		
		if(mask.yMask<yMin)mask.yMask=yMin;  ///cat Mask box in letterH
		
		mask.imgW=abs(v[index].vStart.x-v[index].vEnd.x)+dL*2;
		if(mask.imgW>32){
			mask.imgW=32;
			mask.xMask+=(mask.imgW-32)/2;
            ///cat Mask box in 32 (size of ASM slice)
		}
		
		if(mask.imgW>textLineSize*0.9){
			mask.imgW=textLineSize*0.9;
			mask.xMask+=(mask.imgW-textLineSize*0.9)/2;
            ///cat Mask box in textLineSize*0.7 (not too big in width	)
		}
		
		if(mask.xMask+mask.imgW>xMax+sizeDL)mask.imgW=xMax-mask.xMask+sizeDL; ///cat Mask box in letterW
		
		mask.mH=abs(v[index].vStart.y-v[index].vEnd.y)+dL*2;
        
		if(mask.mH>textLineSize){
			mask.yMask+=(mask.mH-textLineSize)/2;
			mask.mH=textLineSize;        ///cat Mask box in 0.7 text line size (not too long mask)
		}
		
		if(mask.mH>textLineSize*0.7&&mask.imgW>textLineSize*0.7){  //too big mask
			mask.mH*=0.75;
			mask.imgW*=0.75;
		}
		
		
		
		if(mask.yMask+mask.mH>yMax+sizeDL*2)mask.mH=yMax-mask.yMask+sizeDL*2;   //cut mask bottom
        
		push_back(mask);
	}
    
    //КОМПАКТИЗАЦИЯ ПРИЗНАКОВ  все признаки находящиеся внутри большого признака им поглощаются
    for(int i=0;i<mask32Count();i++)mask32[i].status=1;
	
    for(int i=0;i<mask32Count();i++){
        
        if(!mask32[i].status)continue;
        
        for(int j=0;j<mask32Count();j++){
            
            if(i==j||!mask32[j].status)continue;
            if(mask32[j].xMask+textLineSize/6>=mask32[i].xMask&&
               mask32[j].xMask+mask32[j].imgW<mask32[i].xMask+mask32[i].imgW+textLineSize/6&&
               
               mask32[j].yMask+textLineSize/6>=mask32[i].yMask&&
               mask32[j].yMask+mask32[j].mH<mask32[i].yMask+mask32[i].mH+textLineSize/6
               
               )  mask32[j].status=0;
        }
    }
    
    
    //Grow small mask
	for(int j=0;j<mask32Count();j++){
		
		if(!mask32[j].status)continue;
		if(mask32[j].imgW<textLineSize/4){
			mask32[j].imgW=textLineSize/4;
			if(mask32[j].xMask+mask32[j].imgW>xMax+sizeDL){
				mask32[j].xMask=xMax-mask32[j].xMask+sizeDL-mask32[j].imgW; ///cat Mask box in letterW
			}
		}
		if(mask32[j].mH<textLineSize/4){
			mask32[j].mH=textLineSize/4;
			if(mask32[j].yMask+mask32[j].mH>yMax+sizeDL){
				mask32[j].yMask=yMax-mask32[j].yMask+sizeDL-mask32[j].mH; ///cat Mask box in letterW
			}
		}
		
        
	}
    //КОМПАКТИЗАЦИЯ ПРИЗНАКОВ  все признаки находящиеся внутри большого признака им поглощаются
	for(int i=0;i<mask32Count();i++){
		
		if(!mask32[i].status)continue;
		
		for(int j=0;j<mask32Count();j++){
			
			if(i==j||!mask32[j].status)continue;
			if(mask32[j].xMask+textLineSize/6>=mask32[i].xMask&&
			   mask32[j].xMask+mask32[j].imgW<mask32[i].xMask+mask32[i].imgW+textLineSize/6&&
			   
			   mask32[j].yMask+textLineSize/6>=mask32[i].yMask&&
			   mask32[j].yMask+mask32[j].mH<mask32[i].yMask+mask32[i].mH+textLineSize/6
			   
			   )  mask32[j].status=0;
		}
	}
	
	
	
    //Erase empty mask;
	
	index=0;
	for(int i=0;i<mask32Count();i++){
		if(mask32[i].status){
			mask32[index]=mask32[i];
			mask32[index].maskType=1;
			mask32[index].status=1;
			mask32[index].dlt0=8;
			mask32[index].dlt1=1;
			mask32[index].dlt2=0;
			
		    index++;
		}
		
	}
    
};


void GLetter::drawContour(){
    int border0=contour[0]->border;
    int border_;
    contour[0]->height()>contour[0]->width()?border_=(128-contour[0]->height())/2+border0:border_=(128-contour[0]->width())/2+border0;
    //cout_<<"contour[0]->height()="<<contour[0]->height()<<" border_="<<border_<<" contour[0]->border="<<contour[0]->border<<" h="<<contour[0]->height()-border0*2+border_*2<<endl;
    
    if(contourCount==1){
        GBitmap *img=GBitmap::create(contour[0]->width()-border0*2+border_*2,contour[0]->height()-border0*2+border_*2);
        img->drawImg(contour[0]->img,border_-border0,border_-border0);
        //img->printToScreen();  //exit(0);
        mask128.packBitMask128(img);
        mask128.xMask=-(contour[0]->width()+border_*2)/2+border0;
        mask128.yMask=-(contour[0]->height()+border_*2)/2+border0;
    }else{
        GBitmap *img=mask128.unPackMask();
        //cout_<<"draw main image"<<endl; img->printToScreen(); //exit(0);
        
        //cout_<<"letter xMin="<<xMin<<" contour[a]->dX="<<contour[1]->dX<<endl;
        for(int a=1;a<contourCount;a++){
            int dX=contour[a]->dX-xMin+border_-border0;
            int dY=contour[a]->dY-yMin+border_-border0;
            
            img->drawImg(contour[a]->img,dX,dY,DRAW_BLACK);
            //cout_<<"draw contour"<<a<<endl; img->printToScreen(); //exit(0);
            mask128.packBitMask128(img);
        }
    }
    
}


string GLetter::drawToSVG(){
	
	int n=300/textLineSize;
	ostringstream out;
	int d=textLineSize;
	int color;
	out<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"<<endl;
	out<<"<svg version = \"1.1\""<<endl;
    out<<"baseProfile=\"full\""<<endl;
	out<<"xmlns = \"http://www.w3.org/2000/svg\""<<endl;
	out<<"xmlns:xlink = \"http://www.w3.org/1999/xlink\""<<endl;
	out<<"xmlns:ev = \"http://www.w3.org/2001/xml-events\""<<endl;
	out<<"height = \"800px\"  width = \"800px\">"<<endl;
	out<<"<rect x=\"0\" y=\"0\" width=\"800\" height=\"800\""<<endl;
	out<<"fill=\"none\" stroke=\"black\" stroke-width=\"5px\" stroke-opacity=\"0.5\"/>"<<endl;
	
	//out<<"<image x=\"0\" y=\"0\" width=\"800px\" height=\"800px\""<<endl;
    //out<<"xlink:href=\"_112.jpg\">"<<endl;
	//out<<"</image>"<<endl;
	for(int i=0;i<vectorCount;i++){
		out<<"<text x=\""<<v[i].vCenter.x*n+d*n+3<<"\" y=\""<<v[i].vCenter.y*n+d*n<<"\" font-family=\"Verdana\" font-size=\"9\" fill=\"black\" >"<<i<<"</text>"<<endl;
		out<<"<line x1=\""<<v[i].vStart.x*n+d*n<<"\" y1=\""<<v[i].vStart.y*n+d*n<<"\" x2=\""<<v[i].vEnd.x*n+d*n<<"\" y2=\""<<v[i].vEnd.y*n+d*n<<
		"\" stroke-width=\"5\" stroke=\"green\" stroke-opacity=\"0.3\"/>"<<endl;
	}
	
	//out<<"<g stroke=\"blue\" >"<<endl;
	srand ( (uint)time(NULL) );
	for(int i=0;i<mask32Count();i++){
		if(!mask32[i].status)continue;
        out<<"<rect x=\""<<mask32[i].xMask*n+d*n<<"\" y=\""<<mask32[i].yMask*n+d*n<<"\" width=\""<<mask32[i].imgW*n<<"\" height=\""<<mask32[i].mH*n<<"\" ";
		out<<"fill=\"none\" stroke-width=\"2px\" stroke=\"rgb(";
		color= rand() % 100 + 1;
		out<<color<<"%,";
		color= rand() % 100 + 1;
		out<<color<<"%,";
		color= rand() % 100 + 1;
		out<<color<<"%)\"/>"<<endl;
        
	}
	//out<<"</g>";
	//out<<"<g fill-opacity=\"0.7\" stroke=\"black\" stroke-width=\"0.5px\">"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"red\"   transform=\"translate(  0,-50)\" />"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"blue\"  transform=\"translate( 70, 50)\" />"<<endl;
	//out<<"<circle cx=\"200px\" cy=\"200px\" r=\"100px\" fill=\"green\" transform=\"translate(-70, 50)\" />"<<endl;
	//out<<"</g>";
	out<<"</svg>";
    
    
    return out.str();
}


void GLetter::cropLetter(){
    int print=0;
    for(int a=0;a<mask32Count();a++){
        DR("mask32["<<a<<"].dlt0="<<(int)mask32[a].dlt0<<endl);
        DR("mask32["<<a<<"].status="<<(int)mask32[a].status<<endl);
        DR("mask32["<<a<<"].xMask="<<(int)mask32[a].xMask<<endl);
        DR("letterW="<<(int)letterW<<endl);
        DR("dX="<<(int)dX<<endl);
        //if(mask32[a].dlt0!=8&&mask32[a].dlt0!=28)mask32[a].dlt0=8;
        //if(!mask32[a].status)continue;
        
        //проверка на выход маски за габариты.
        //левый край
        if(mask32[a].xMask<-(letterW/2+1)){
            //if(mask32[a].yMask>y0-10&&mask32[a].yMask<y1-10)
            mask32[a].xMask=-(letterW/2+1);
            DR("new x="<<mask32[a].xMask<<endl);
        }
        //правый край
        if(mask32[a].xMask+mask32[a].imgW>letterW/2+1){
            //if(mask32[a].yMask>y0-10&&mask32[a].yMask<y1-10)
            mask32[a].xMask=letterW/2+2-mask32[a].imgW;
            DR("new x="<<mask32[a].xMask<<endl);
        }
        //верхний край
        if(mask32[a].yMask<-(letterH/2+1)){
            //if(mask32[a].yMask>y0-10&&mask32[a].yMask<y1-10)
            mask32[a].yMask=-(letterH/2+1);
            DR("new x="<<mask32[a].xMask<<endl);
        }
        //нижний край
        if(mask32[a].yMask+mask32[a].mH>letterH/2+1){
            //if(mask32[a].yMask>y0-10&&mask32[a].yMask<y1-10)
            mask32[a].yMask=letterH/2+1-mask32[a].mH;
            DR("new x="<<mask32[a].xMask<<endl);
        }


    }
    //reloadMask();
};

//проверка на дубликаты масок по местоположению и габаритам. Если маска имеет дубликат, она стирается.
void GLetter::eraseDublicateMask(){
    int print=0;
    
    for(int a=0;a<mask32Count();a++){
        for(int b=0;b<mask32Count();b++){
            DT("mask32["<<b<<"].status="<<(int)mask32[b].status<<endl);
            if(a==b||!mask32[b].status)continue;
            int flag=1;
            if(abs(mask32[a].xMask-mask32[b].xMask)>abs(mask32[a].imgW)/2)continue;
            DT("    "<<mask32[a].xMask<<"-"<<mask32[b].xMask<<" f5"<<endl);
            if(abs(mask32[a].yMask-mask32[b].yMask)>abs(mask32[a].mH)/2)continue;
            DT("    "<<mask32[a].yMask<<"-"<<mask32[b].yMask<<" f7"<<endl);
            if(abs(mask32[a].imgW-mask32[b].imgW)>abs(mask32[a].imgW)/2)continue;
            DT("    "<<mask32[a].imgW<<"-"<<mask32[b].imgW<<" f9"<<endl);
            if(abs(mask32[a].mH-mask32[b].mH)>abs(mask32[a].mH)/2)continue;
            DT("    "<<mask32[a].mH<<"-"<<mask32[b].mH<<" f10"<<endl);
            DT("     /flag="<<flag<<endl);
            mask32[b].status=0;
            DT("     /__erase mask "<<b<<endl);
            
        }
    }
    
};

//стирает содержимое маски из изображения буквы
void GLetter::eraseMaskContent(int m){
    //cout_<<"reload mask scale="<<scale<<endl;
	//int border=128;
	GBitmap *img=mask128.unPackMask();
    
    
    int print=1;
	DR("mask32Count="<<mask32Count()<<endl)
	//img->printToScreen();
	int x0,y0,w,h;
	//cout_<<" done"<<endl;
	//int maskXMin=512,maskYMin=512;
    
    x0=letterW/2+mask32[m].xMask+dX;
    y0=img->rows()/2+mask32[m].yMask;
    w=mask32[m].imgW;
    h=mask32[m].mH;
        
    cout<<"dX="<<dX<<" m="<<m<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" mask32[m].xMask="<<mask32[m].xMask<<" img->columns()="<<img->columns()<<endl;
    //exit(0);
    img->clearRegion(x0,y0,w,h);
    //img->printToScreen(); exit(0);
    mask128.packBitMask128(img);
}

string GLetter::saveToString(){  ///save letter in string
    return "";
}


void GLetter::writeToStr(TString *st){
    int d1[64];                                        /// array of letter parameters
    d1[0]= letterIndex;                                ///letter index in base
    d1[1]= stringNum;
    d1[2]= recordFlag;
    d1[3]= recordStatus;
    d1[4]= onOffRate;
    d1[5]= keyCount_;
    d1[6]= contourCount;
    d1[7]= mask32Count();                                ///<количество признаков
    
    
    d1[11]= dX;
    d1[12]= dY;                                      ///<смещение координат центра буквы относительно изображения буквы  (mask128)
    d1[13]= 0;
    d1[14]= OCRIndex;
    d1[15]= correlation;                                ///<value of correlation
    d1[16]= selfCorrelation;                            ///<test value of self correlation
    d1[17]= codeSpace;                                  ///<test value of test value of code space between this letter and all letters in letter base
    d1[18]= Character;
    d1[19]= vectorLimit;
    d1[20]= stackFlag;
    d1[21]= OCRStatus;
    d1[22]= letterW;                                    ///габаритная ширина буквы в строке без учета огласовок и подписных букв
    d1[23]= letterH;                                    ///высота буквы
    d1[24]= baseLetter;
    d1[25]= xMin;
    d1[26]= xMax;
    d1[27]= yMax;
    d1[28]= yMin;
    d1[29]= xSum;
    d1[30]= ySum;
    d1[31]= cn;                                         //counter of this letter recognition
    d1[32]= y0;                                         //верхний габарит строки относительно центра изображения буквы  (mask128)
    d1[33]= y1;                                         //нижний габарит строки относительно центра изображения буквы (mask128)
    d1[34]= textLineSize;                               //size of text line in text in pixels
    d1[35]= newLetter;
    d1[36]= newLetterInBook;
    d1[37]= allLetterCount;
    d1[38]= GFontSize;
    d1[39]= lineCount;
    d1[40]= cnRes;                                       ///rezult counter of this letter recognition (cCount/10 000)
    d1[41]= crSum;                                       ///buffer for store result of correlation this letter in text
    d1[42]= crRes;                                       ///middle result of correlation
    d1[43]= idNumber;                                    ///<id number in base
    st->push_back((char*)d1,64*4);
    st->push_back(name);
    st->push_back(OCRKey);
    st->push_back((char*)mask32Vector->dataPtr(),(int)mask32Vector->size()*(int)sizeof(GBitMask32));
    st->push_back((char*)&mask128,sizeof(GBitMask128));
    st->push_back((char*)focalPoint->dataPtr(),(int)focalPoint->size()*(int)sizeof(OCRPoint));
    st->push_back((char*)&maskOriginal,sizeof(GBitMask128));
    TString s;
    for(int i=0;i<focalLine.size();i++){ //focalLine.size()
        TStr c;
        focalLine[i].writeToStr(c);
        s.push_back((TString*)&c);
    }
    st->push_back(&s);
    
}


//функция читает данные из базы данных и представляет из в виде даных класса GLetter
//Для получения данных из GLetter по прямой ссылке без копирования, возможно интерпретировать
//данные непосредственно из указателя на TString. например GBitMask32* getMask(int index,TString *st)
void GLetter::readFromStr(TString *st){
    string str=st[0][0];
    int *d1=(int*)&str[0];
    letterIndex         =d1[0];             ///letter index in base
    stringNum           =d1[1];
    recordFlag          =d1[2];
    recordStatus        =d1[3];
    onOffRate           =d1[4];
    keyCount_           =d1[5];
    contourCount        =d1[6];
    
    
    dX                  =d1[11];
    dY                  =d1[12];             ///<смещение координат центра буквы относительно изображения буквы  (mask128)
    //                  =d1[13];
    OCRIndex            =d1[14];
    correlation         =d1[15];             ///<value of correlation
    selfCorrelation     =d1[16];             ///<test value of self correlation
    codeSpace           =d1[17];             ///<test value of test value of code space between this letter and all letters in letter base
    Character           =d1[18];
    vectorLimit         =d1[19];
    stackFlag           =d1[20];
    OCRStatus           =d1[21];
    letterW             =d1[22];             ///габаритная ширина буквы в строке без учета огласовок и подписных букв
    letterH             =d1[23];             ///высота буквы
    baseLetter          =d1[24];
    xMin                =d1[25];
    xMax                =d1[26];
    yMax                =d1[27];
    yMin                =d1[28];
    xSum                =d1[29];
    ySum                =d1[30];
    cn                  =d1[31];             //counter of this letter recognition
    y0                  =d1[32];             //верхний габарит строки относительно центра изображения буквы  (mask128)
    y1                  =d1[33];             //нижний габарит строки относительно центра изображения буквы (mask128)
    textLineSize        =d1[34];             //size of text line in text in pixels
    newLetter           =d1[35];  
    newLetterInBook     =d1[36];
    allLetterCount      =d1[37];
    GFontSize           =d1[38];
    lineCount           =d1[39];
    cnRes               =(unsigned int)d1[40];                                       ///rezult counter of this letter recognition (cCount/10 000)
    crSum               =(unsigned int)d1[41];                                       ///buffer for store result of correlation this letter in text
    crRes               =(unsigned int)d1[42];                                       ///middle result of correlation
    idNumber            =(unsigned int)d1[43];  ///<id number in base
    
    if(1){
    name=st[0][1];
    OCRKey=st[0][2];   
    string maskStr=st[0][3];
    mask32Vector->push_Ptr( (GBitMask32*)&maskStr[0],(uint)maskStr.size());
    mask32=(GBitMask32*)mask32Vector->dataPtr();
    string mask128Str=st[0][4];
    memcpy(&mask128,&mask128Str[0],sizeof(GBitMask128));
    

    if(st->size()>5){
        string keyStr=st[0][5];
        focalPoint->push_Ptr( (OCRPoint*)&keyStr[0],(uint)keyStr.size());
        fPoint=(OCRPoint*)focalPoint->dataPtr();
    }
    if(st->size()>6){mask128Str=st[0][6];
         memcpy(&maskOriginal,&mask128Str[0],sizeof(GBitMask128));
    }
    if(st->size()>7){
        TString keyStr;
        st->get(&keyStr,7);
        //cout<<"keyStr="<<keyStr.size();
        for(int i=0;i<keyStr.size();i++){
            TString s_;
            TStr *s__=(TStr*)&s_;
            OCRFocalLine line;
            keyStr.get(&s_, i);
            line.readFromStr(*s__);
            focalLine.push_back(line);
        }
        fLine=(OCRFocalLine*)&focalLine[0];
    }
        
    
    }
        
};


void GLetter::push_back(GBitMask32 mask){
    mask32Vector->push_back(mask);
    mask32=(GBitMask32*)mask32Vector->dataPtr();
};

void GLetter::push_back(keyOCR mask){
    keyOCRVector->push_back(mask);
    key=(keyOCR*)keyOCRVector->dataPtr();
};

GBitmap* GLetter::drawPict(){
    GBitmap *img=mask128.unPackMask();
    //img->dotGain(-1,-1,-1,-1, 0);
    //добавляем со всех сторон символа по нескольку пикселей
    //img->dotGain( 2, 2, 2, 2, 0);
    GBitmap* img_=GBitmap::create(img->columns()+60,img->rows()+120);
    img_->fill(255);
    img_->drawImg(img,30,60);
    img->destroy();
    newLetter=0;
    return img_;
}

GBitmap* GLetter::drawPict32(){
    //cout_<<"draw letter"<<name<<endl;
	string str;
	int x0,y0,w,h;
    int mode=ADD_MODE;
	
    GBitmap *outBitmap32=GBitmap::create(PICT_SIZE,PICT_SIZE,BITMAP_32);

	for(int m=0; m<mask32Count();m++){
		//SH(mask32[m].w*zoomImage);  SH(mask32[m].h*zoomImage);
		x0=mask32[m].xMask+PICT_SIZE/2;
		y0=mask32[m].yMask+PICT_SIZE/2;
		w=mask32[m].imgW;
		h=mask32[m].mH;
        OCRBox s;
        s.x0=0; s.x1=1000;
        s.y0=0; s.y1=1000;
        
        outBitmap32->drawMask32V(&mask32[m],x0,y0,&s,mode); continue;
    }
   	return outBitmap32;
    
}


GBitmap* GLetter::drawLetterPict(int mode){
    
	//cout_<<"draw letter"<<name<<endl;
	string str;
	int x0,y0,w,h;
	//int color0=255;    //fill
	int color1=0;     //on mask
	int color2=200;   //off mask
	int color3=80;     //letter
    if(mode==3)color3=0;
	
	GBitmap *outBitmap=GBitmap::create(PICT_SIZE,PICT_SIZE);
	GBitmap *img=mask128.unPackMask();
    
    if(mask128.xMask==0)mask128.xMask=0-mask128.mW/2;

	for (int y = 0; y <img[0].rows() ;y++){
		//pSrc = (unsigned char*)letterImage->Canvas->ScanLine[y];
		for (int x = 0; x <img[0].columns(); x++){
			
			//if((int)img[0][y][x]){cout_<<"1";}else{cout_<<"0";}
			
			if(img[0][y][x]){
				
			}else{
				//if(outBitmap[0]
				//   [y+aliKali[0][index]->mask128.yMask]
				//   [x+aliKali[0][index]->mask128.xMask]==255){
                outBitmap[0]
                [y+mask128.yMask+PICT_SIZE/2]
                [x+mask128.xMask+PICT_SIZE/2]=color3;
				//}
			}
		}//cout_<<END;
	}
    if(mode!=3){
        for(int m=0; m<mask32Count();m++){
            if(!mask32[m].status)continue;
            //SH(mask32[m].w*zoomImage);  SH(mask32[m].h*zoomImage);
            x0=mask32[m].xMask+PICT_SIZE/2;
            y0=mask32[m].yMask+PICT_SIZE/2;
            w=mask32[m].imgW;
            h=mask32[m].mH;
            
            //if(w<=0||h<=0){cout<<"no valid mask";exit(0);}
            img=mask32[m].unPackMask();
            
            for (int y = 0; y <img[0].rows();y++){
                for (int x = 0; x <img[0].columns(); x++){
                    if(img[0][y][x]){
                    }else{
                        outBitmap[0]
                        [y+(mask32[m].yMask)+PICT_SIZE/2]
                        [x+(mask32[m].xMask)+PICT_SIZE/2]=color1;
                    }
                    
                }//DM(END);
            }
            img->destroy();
            if(mode==1){
                img=mask32[m].unPackMaskOff();
                for (int y = 0; y <img[0].rows();y++){
                    for (int x = 0; x <img[0].columns(); x++){
                        if(img[0][y][x]){
                        }else{
                            outBitmap[0]
                            [y+(mask32[m].yMask)+PICT_SIZE/2]
                            [x+(mask32[m].xMask)+PICT_SIZE/2]=color2;
                        }
                        
                    }//DM(END);
                }
                img->destroy();
            }
            
        }
        
        //подсчитываем площадь буквы не закрытой масками
        unsigned int onCount=0,offCount=0;
        //int color1=0;     //on mask
        //int color2=200;   //off mask
        //int color3=80;     //letter
        
        for (int y = 0; y <outBitmap[0].rows();y++){
            for (int x = 0; x <outBitmap[0].columns(); x++){
                if(outBitmap[0][y][x]==80){
                    offCount++;
                }else if(outBitmap[0][y][x]==0 ){
                    onCount++;
                }
                
            }//DM(END);
        }
        if(onCount){
            if(offCount==0){onOffRate=100;}else{
                onOffRate=(onCount*100)/(offCount+onCount);
            }
        }else{
            onOffRate=0;
        }
    }
	return outBitmap;
}



///Установление границ и координат центра буквы относительн строки
void GLetter::setLetterDimensions(){
    
	int print=0;
    letterW=mask128.mW;
    
    GBitmap *img=mask128.unPackMask();
    GBitmap *img_1,*img_2;
    ((GImageEditor*)inputData.imageEditor)->WriteImageData(img,"/_Image2OCR/1.jpg",0);
    DR("/aliKali[0]["<<index<<"]->y0="<<y0<<"aliKali[0]["<<index<<"]->y1="<<y1<<" letterW="<<letterW
       <<" letterH="<<letterH<<" yMask="<<mask128.yMask<<" xMask="<<mask128.xMask<<endl);
    
   
    if(letterH/2+y0<0){    //проверяем габариты строки
        y0=-letterH/2;
        y1=letterH/2;
    }
    DR(" aliKali[0]["<<index<<"]->y0="<<y0<<"aliKali[0]["<<index<<"]->y1="<<y1<<" letterW="<<letterW<<" letterH="<<letterH<<endl)   
    int x0,x1,y_0,y_1;
    
    int xOld=mask128.xMask;   //маска будет еще передвигатся относительно геометрического центра изображения буквы
    int yOld=mask128.yMask;

    img->detectRegion(0,&x0,&x1,&y_0,&y_1,0);
    DR("x0="<<x0<<" x1="<<x1<<" y0="<<y_0<<" y1="<<y_1<<endl)

    mask128.xMask=-(x1-x0)/2;   //для определения области распознавания буквы устанавливаем центр буквы в геометрическом центре габарита буквы
    mask128.yMask=-(y_1-y_0)/2;
    letterH=y_1-y_0;
    letterW=x1-x0;

    int dX_=xOld-mask128.xMask;
    int dY_=yOld-mask128.yMask;
    
    DR("dX_="<<dX_<<" dY_="<<dY_<<endl);

    for(int a=0;a<mask32Count();a++){
        mask32[a].xMask-=dX_+x0;
        mask32[a].yMask-=dY_+y_0;
    }
    
    
    xOld=mask128.xMask;
    yOld=mask128.yMask;

    img_1=GBitmap::createRegion(img,
                               x0,
                               y_0,
                               x1-x0,
                               y_1-y_0);
    mask128.packBitMask128(img_1);
    
    
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(img_1,"/_Image2OCR/2.jpg",0);//exit(0);
        
    img_2=GBitmap::createRegion(img_1,
                                0,
                                letterH/2+y0,
                                mask128.mW,
                                abs(y1-y0));
    
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(img_2,"/img_2.jpg",0);//exit(0);
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(img_,"/2.jpeg",0);
    img_2->detectRegion(0,&x0,&x1,&y_0,&y_1,0);
    DR("x0="<<x0<<" x1="<<x1<<"y0="<<y_0<<" y1="<<y_1<<endl)   
    letterW=x1-x0;

   
    
    //установление dX и dY необходимо потому, что в распознавании используется центр буквы установленный по габаритам средней части буквы.
    //он отличается от геометрическтго центра буквы. Также letterW letterH это габариты центральной оболасти буквы, без учета выносных элементов
    dX=x0+letterW/2-mask128.mW/2;
    mask128.xMask=-(mask128.mW/2+dX);  //устанавливаем букву по центру
    dX_=xOld-mask128.xMask;
    dY_=yOld-mask128.yMask;
    dY=0;
    
    //img_1->detectRegion(0,&x0,&x1,&y_0,&y_1,0);   //устанавливаем вертикальные габариты
    //letterH=y_1-y_0;
    //dY=y_0+letterH/2-mask128.mH/2;
    //mask128.yMask=-(mask128.mH/2+dY);  //устанавливаем букву по центру
    
    img->destroy();
    img_1->destroy();
    //img_2->destroy();
    
    DR("y_0="<<y_0<<" y_1="<<y_1<<" dY_="<<dY_<<" mask128.yMaskNew="<<mask128.yMask<<endl);
    DR(" x0="<<x0<<" letterW/2="<<letterW/2<<" mask128.mW/2="<<mask128.mW/2<<endl)
    DR(" y0="<<y0<<" letterH/2="<<letterH/2<<" mask128.mH/2="<<mask128.mH/2<<endl)
    DR("dX_="<<dX_<<" mask128.xMask="<<mask128.xMask<<" dX="<<dX<<" dY="<<dY<<endl)

    for(int a=0;a<mask32Count();a++){
            DR("/////   aliKali[0]["<<index<<"]->mask32["<<a<<"].xMask="<<mask32[a].xMask<<endl)
            mask32[a].xMask-=dX_;
            DR("/////   aliKali[0]["<<index<<"]->mask32["<<a<<"].yMask="<<mask32[a].yMask<<endl)
            mask32[a].yMask-=dY_;
    }

    reloadMask();
    dX=0;  //после того как буква установлена по центру, ее смещение равно нулю
    //exit(0);
};



OCRMatch::OCRMatch(){
    matchCount=0; //count for mutch points in structure
    centerDist=0;  //distance between centers of match vector
    vectorIndexModel=0; //index of match vector in base
    vectorIndexTest=0; //index of match vector in base
    allMatchCount=0;  //in [0] element we store count for all array of mutch
    matchSumTest=0; //calculated match
    status=0;
    letterIndex=0;
    correlation=0;
    Character=0;
    pCount=0;
    yCenter=0;
};

void OCRMatch::copyData(GLetter *letter){
    correlation=letter->correlation;
    status=0;
    name=letter->name;
    OCRIndex=letter->OCRIndex;
    letterIndex=letter->letterIndex;
    letterW=letter->letterW;
    letterH=letter->letterH;
};

void OCRMatch::operator+=(const OCRMatch &right){
    this->name=this->name+right.name;
    this->correlation=(this->correlation+right.correlation)/2;
    this->correlation+=right.name.size()*2;
    this->status=0;
    int letterW=right.xCenter+right.letterW/2-this->xCenter+this->letterW/2;
    if(this->letterH>right.letterH){
        this->letterH=this->letterH;
    }else{
        this->letterH=right.letterH;
    }
    this->xCenter=this->xCenter-this->letterW/2+letterW/2;
    this->letterW=letterW;
    this->yCenter=(this->yCenter+right.yCenter)/2;
}

void OCRMatch::operator = (OCRMatchConst &right){
start=right.start;
end=right.end;
matchCount=right.matchCount; //count for mutch points in structure
centerDist=right.centerDist;  //distance between centers of match vector
vectorIndexModel=right.vectorIndexModel; //index of match vector in base
vectorIndexTest=right.vectorIndexTest; //index of match vector in base
allMatchCount=right.allMatchCount;
matchSumTest=right.matchSumTest; //calculated match
xCenter=right.xCenter; //letter center
yCenter=right.yCenter;
status=right.status;
letterIndex=right.letterIndex;
lineIndex=right.lineIndex;
letterID=right.letterID;
letterW=right.letterW;
letterH=right.letterH;
maxY=right.maxY;
correlation=right.correlation;
correlationNew=right.correlationNew;
Character=right.Character;
x0=right.x0;
x1=right.x1;
name=right.name.c_str();
nameUni=right.nameUni.c_str();
OCRIndex=right.OCRIndex;
id=right.id;
}

void OCRMatch::setSize(){
    int n=100000;
    if(y0==0||y1==0||x0==0||x1==0||y0>n||y1>n||x0>n||x1>n){
        y0=yCenter-letterH/2;
        y1=yCenter+letterH/2;
        x0=xCenter-letterW/2;
        x1=xCenter+letterW/2;
    }else{
        yCenter=(y1-y0)/2+y0;
        xCenter=(x1-x0)/2+x0;
        letterH=y1-y0;
        letterW=x1-x0;
    }
    s.x0=x0+MATRIX_BORDER;
    s.x1=x1+MATRIX_BORDER;
    s.y0=y0+MATRIX_BORDER;
    s.y1=y1+MATRIX_BORDER;
    area=letterH*letterW;
    s.area=area;
    if(dX!=0||dY!=0){ //cout<<" dX="<<dX<<" dY="<<dY<<endl;
        for(int i=0;i<mask32Vector.size();i++){
            mask32Vector[i].xMask=mask32Vector[i].xMax;
            mask32Vector[i].yMask=mask32Vector[i].yMax;
        }
        dX=dY=0;
    }
    if(letter.size()){
        xL0C=letter[0].xCenter;
        xL1C=letter[letter.size()-1].xCenter;
    }
}

void OCRMatch::setCenter(){//устанавливает ценр буквы по габаритам и персчитывает координаты масок признаков
    
    int xCenterNew=(x1-x0)/2+x0;
    int yCenterNew=(y1-y0)/2+y0;
    int dX=xCenter-xCenterNew;
    int dY=yCenter-yCenterNew;
    xCenter=xCenterNew;
    yCenter=yCenterNew;
    
    for(int m=0; m<mask32Vector.size();m++){
		mask32Vector[m].xMask+=dX;
        mask32Vector[m].yMask+=dY;
    }
    
    letterH=y1-y0;
    letterW=x1-x0;

    s.x0=x0+MATRIX_BORDER;
    s.x1=x1+MATRIX_BORDER;
    s.y0=y0+MATRIX_BORDER;
    s.y1=y1+MATRIX_BORDER;
    area=letterH*letterW;
    s.area=area;

}

void OCRMatch::drawPict32(GBitmap* outBitmap32,int border, int mode){
    //if(inputData.start==111)cout<<"draw letter"<<name<<" mask32Vector.size()="<<mask32Vector.size()<<endl;
	string str;
	int x0,y0;
    //int print=1111;
    //if(inputData.start==111)print=1;
    
    if(letter.size()){
        for(int n=0;n<letter.size();n++){
            for(int m=0; m<letter[n].mask32Vector.size();m++){
                x0=letter[n].mask32Vector[m].xMax;
                y0=letter[n].mask32Vector[m].yMax;
                
                //if(print==1){
                //    cout<<"y0="<<y0<<" y0_="<<letter[n].s.y0<<" x0_="<<letter[n].s.x0
                //    <<" dY="<<letter[n].dY<<" n="<<letter[n].name<<endl;
                //}//exit(0);
                
                outBitmap32->drawMask32V(&letter[n].mask32Vector[m],x0,y0,&letter[n].s,mode);
            }
        }
    
    }else{
        for(int m=0; m<mask32Vector.size();m++){
            x0=mask32Vector[m].xMax;
            y0=mask32Vector[m].yMax;  
            outBitmap32->drawMask32V(&mask32Vector[m],x0,y0,&s,mode);
        }
    }

	
    
}
//добавляет новые маски в букву и пересчитывает их координаты
void OCRMatch::addMask32Vector(OCRMatch &a){
    int dX=xCenter-a.xCenter;
    int dY=yCenter-a.yCenter;   

    for(int i=0;i<a.mask32Vector.size();i++){
        GBitMask32 mask=a.mask32Vector[i];
        mask.xMask-=dX;
        mask.yMask-=dY;
        mask32Vector.push_back(mask);
    }
}


int operator==(const OCRMatch &left, const OCRMatch &right){
	return  left.correlation==right.correlation;
}

int operator<(const OCRMatch &left, const OCRMatch &right){
	return  left.correlation>right.correlation;
}


OCRMatchConst::OCRMatchConst(){};
OCRMatchConst::~OCRMatchConst(){}

void OCRMatchConst::operator = (const OCRMatch &right){
    start=right.start;
    end=right.end;
    matchCount=right.matchCount; //count for mutch points in structure
    centerDist=right.centerDist;  //distance between centers of match vector
    vectorIndexModel=right.vectorIndexModel; //index of match vector in base
    vectorIndexTest=right.vectorIndexTest; //index of match vector in base
    allMatchCount=right.allMatchCount;
    matchSumTest=right.matchSumTest; //calculated match
    xCenter=right.xCenter; //letter center
    yCenter=right.yCenter;
    status=right.status;
    letterIndex=right.letterIndex;
    lineIndex=right.lineIndex;
    letterID=right.letterID;
    letterW=right.letterW;
    letterH=right.letterH;
    maxY=right.maxY;
    correlation=right.correlation;
    correlationNew=right.correlationNew;
    Character=right.Character;
    x0=right.x0;
    x1=right.x1;
    const char* data=right.name.c_str();
    name=data;
    const char *data_=right.nameUni.c_str();
    nameUni=data_;
    OCRIndex=right.OCRIndex;
    id=right.id;
}



