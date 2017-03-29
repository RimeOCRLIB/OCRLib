#include "config.h"

#include "OCRTypes.h"
#include "GFont.h"
#include "GLetter.h"
#include "GImageEditor.h"
#include "GLogicProcessor.h"

//#include "OCRString/php2stl.h"

namespace ocr {
    
    //#include "GFontIO.h"
    
    GFont::GFont(){
        string path;
        path=inputData.data["OCRTables"]+"/GFontVector.bin";
        try
        {
            init(path);
            if(!letterCount())readGFontXML();
            letterReadCount=0;
            *lockFlag=0;
            //cMatrix=GStr<uchar>::create(this,"cMatrix");
            //cMatrix->resize(FSIZE*FSIZE);
            OCRLanguage=&innerData[8];           //язык распознавания
            
            
        }
        catch(int a)
        {
            destroy();
        }
        //readGFontDB(); exit(0);
        //resizeData(20000,300);
    }
    
    GFont::GFont(const char *path){
        string path_=inputData.data["OCRTables"]+path;
        try
        {
            init(path_);
            letterReadCount=0;
            *lockFlag=0;
            //cMatrix=GStr<uchar>::create(this,"cMatrix");
            //cMatrix->resize(FSIZE*FSIZE);
            OCRLanguage     =&innerData[8];           //язык распознавания
        }
        catch(int a)
        {
            destroy();
        }
        //readGFontDB(); exit(0);
        //resizeData(20000,300);
    }

    
    void GFont::reloadPtr(){
        poolSize		=&innerData[1];           //размер файла вектора
        recordCount		=&innerData[2];  	      //количество записей в векторе
        recordArraySize	=&innerData[3];           //размер файла учета записей
        dataSize		=&innerData[4];  	 	  //размер занимаемый актуальными данными
        lockFlag        =&innerData[5];           //флаг блокировки чтения-записи
        indexOffset		=&innerData[6];	 	 	  //адрес размещения массива индекса записей. отсчитывается от начала файла
        vectorID		=&innerData[7]; 	 	  //индех записи, в которой GVector размещен в родительском векторе
        OCRLanguage     =&innerData[8];           //язык распознавания
        cMatrix->reload(this);

    };
    
    
    GFont::~GFont(){}

	void GFont::destroy(void){
		//for(int i=0;i<*recordCount-1;i++){
		//	letterSet[i]->destroy();
        //}
		delete this;
	}
    
    /*    GLetter*  GFont::operator[](uint index){  //deprecated too match memory consumption.
     if(letterSet[index]==NULL){
     TString dataStr; getTStr(index,dataStr);
     GLetter *letter=GLetter::create();
     letterSet[index]=letter;
     //cout<<"@@"<<dataStr.len;
     letter->readFromStr(&dataStr);
     }
     return letterSet[index];
     };
     */
    void  GFont::freeMemory(){
        string dataPath_=dataPath;
        munmap(data,data_size());
        init(dataPath_);
    }
    
    GLetter*  GFont::getLetter(ulong index){
        //cout<<"get letter"<<index<<" *lockFlag="<<*lockFlag<<endl;
        //while (*lockFlag!=0){usleep(10);}
        //*lockFlag=1;
        TString dataStr; getTStr(index,dataStr);
        GLetter *letter=GLetter::create();
        if(dataStr.size()>4){
            ///cout<<"dataStr.len="<<dataStr.size()<<" i="<<index<<endl;;
            letter->readFromStr(&dataStr);
            letterReadCount++;
            //if(letterReadCount==1000){
            //    freeMemory();
            //    letterReadCount=0;
            //}
            letter->letterIndex=(uint)index-1;
            letter->OCRIndex=letter->OCRKey[0];
        }else{ cout<<"error="<<index<<" "<<"size="<<dataStr.size(); return 0;}
        //*lockFlag=0;
        return letter;
    };
    
    //функция для вызова из базы алфавита языка распознавания
    GLetter*  GFont::getOCRLetter(ulong index){
        //cout<<"get letter"<<index<<" *lockFlag="<<*lockFlag<<endl;
        //index++;
        //while (*lockFlag!=0){usleep(10);}
        //*lockFlag=1;
        TString dataStr; getTStr(index,dataStr);
        string str=dataStr[0];
        int *d1=(int*)&str[0];
        if(d1[3]!=1)return 0;
        
        GLetter *letter=GLetter::create();
        if(dataStr.size()>4){
            ///cout<<"dataStr.len="<<dataStr.size()<<" i="<<index<<endl;;
            letter->readFromStr(&dataStr);
            letterReadCount++;
            //if(letterReadCount==1000){
            //    freeMemory();
            //    letterReadCount=0;
            //}
            letter->letterIndex=(uint)(index-1);
            letter->OCRIndex=letter->OCRKey[0];
        }else{ cout<<"error="<<index<<" "<<"size="<<dataStr.size();}
        //*lockFlag=0;
        return letter;
    };
    
    void  GFont::saveLetter(GLetter* letter){
        TString dataStr;
        letter->writeToStr(&dataStr);
        uint index=(uint)letter->letterIndex+1;
        while (*lockFlag!=0)usleep(10);
        *lockFlag=1;
        putTStr(index,dataStr);
        *lockFlag=0;
        
    }
    
    void GFont::push_back(GLetter *letter){
        TString dataStr;
        /*
         for(int i=0;i<letter->mask32Count();i++){
         if(letter->mask32[i].mW==0||letter->mask32[i].mW>1000||letter->mask32[i].mH==0||letter->mask32[i].mH>1000){cout<<"%%%%%"<<endl;cout<<"mW="<<letter->mask32[i].mW<<"mH="<<letter->mask32[i].mH<<endl;}
         }
         */
        letter->letterIndex=(uint)letterCount();  //первая запись это CMatrix
        letter->writeToStr(&dataStr);
        //cout<<"done push_back";
        while (*lockFlag!=0)usleep(10);
        *lockFlag=1;
        GVector::push_back(dataStr);
        *lockFlag=0;
        
    };
    
    
    
    void  GFont::operator+=(GLetter *letter){
        TString dataStr;
        letter->writeToStr(&dataStr);
        while (*lockFlag!=0)usleep(10);
        *lockFlag=1;
        GVector::push_back(dataStr);
        *lockFlag=0;
    }
    
    /*int GFont::indexById(unsigned int Id){	//находим нужную букву по idNumber
     int index=-1;
     for(int i=0;i<letterCount;i++){   ####
     //cout_<<i<<" idNumber="<<base[i].idNumber<<" Id="<<Id<<endl;
     if(letterSet[i]->idNumber==Id){
     index=i;
     break;
     }
     }
     
     return index;
     }
     */
    
    
    void GFont::fontNormalisation(){
        for(int index=0;index<*recordCount-1;index++){
            //if(letterSet[index]!=NULL)letterSet[index]->destroy();
            GLetter *letter=getLetter(index);
            letter->eraseEmptyMask();
            letter->maskNormalisation();
            saveLetter(letter);
            letter->destroy();
        }
        
        
    };
    
    /**чтение данных базы букв из OCRData/OCRTables/fileName или
     импорт XML данных GFont из path to database */
    void GFont::readGFontXML(){
        
        //GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
        
        //string fileName=binPath;
        xml_node col,data,resultSet,GFontXML, st;
        xml_document doc;
        string str;
        unsigned in;
        int print=0;
        
        DT("//_  load aliKali"<<endl);
        string vData;
        
        vector<string>dataArray;
        vector<string>vArray;
        
        int mainFont=0;
        string path=inputData.data["OCRTables"]+"GFont.xml";
        
		xml_parse_result stat=doc.load_file(path.c_str());
        
        if(!stat){
            cout<<path<<" not loaded"<<endl;
			cout<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
			ifstream file (path.c_str(), ios::in|ios::binary|ios::ate);
			//int lineIndex=0;
			ifstream::pos_type size;
			//long size_block = file.tellg();
			
			if (file.is_open())
			{
				int size = 1024;
				char *memblock = new char [size];
				//file.seekg (0, ios::beg);
				file.seekg (stat.offset, ios::beg);
				file.read (memblock, size);
				str=memblock;
				str=str_replace("<","{", str);
				str=str_replace(">","}", str);
				//file.read (memblock, size_block);
				file.close();
				cout_<<"line offset="<<str<<endl;
				//c_out<<"line offset="<<memblock<<endl;
				delete[] memblock;     //cout_<<"size_block="<<size_block<<endl;
			}
			exit(0);
		}
        
        
        
		resultSet = doc.child("GFont");
		// Iterate through books
		int indexLetter=0; textLineSize=0;
		//base.resize(500);
		fontName=resultSet.child("fontName").attribute("n").value();
		cout<<"fontName="<<fontName<<" letterCount="<<letterCount()<<endl;
        
        int count=0;
        
		for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){
            count++;
            if(count%1000==0)cout<<". "<<count;
            
            if(count==22258){
                cout<<"";
            }
            
            GLetter *letter=GLetter::create();
            
            letter->idNumber=indexLetter;//strtoul(row.attribute("id").value(),NULL,0);
            letter->letterIndex=indexLetter;indexLetter++;
            letter->vectorCount=(int)dataArray.size();
            letter->name=row.attribute("n").value();
            
            //@@@if(letter->name.find("*")!=-1){letter->name=":)";  cout_<<index<<" ";}
            //letter->name=logicProcessor->YagpoToUni(letter->name);
			DT("letter->name="<<letter->name<<endl);
            
            letter->OCRKey=row.attribute("wt").value();
            letter->letterH=atoi(row.attribute("h").value());
            letter->letterW=atoi(row.attribute("w").value());
            letter->dX=atoi(row.attribute("dX").value());
            letter->dY=atoi(row.attribute("dY").value());
            letter->lengthSum=atoi(row.attribute("s").value());
            letter->stackFlag=atoi(row.attribute("f").value());
            //letter->OCRIndex=atoi(row.attribute("in").value());
            letter->OCRStatus=0;
            if(mainFont)letter->OCRStatus=100;
            letter->baseLetter=atoi(row.attribute("l").value());
            letter->Character=atoi(row.attribute("c").value());
            letter->newLetterInBook=atoi(row.attribute("nb").value());
            letter->y0=atoi(row.attribute("y0").value());
            letter->y1=atoi(row.attribute("y1").value());
            letter->cn=atoi(row.attribute("cn").value());
            letter->cnRes=atoi(row.attribute("cnr").value());
            letter->crRes=atoi(row.attribute("cr").value());
            letter->selfCorrelation=atoi(row.attribute("sC").value());
            letter->codeSpace=atoi(row.attribute("cS").value());
            letter->onOffRate=atoi(row.attribute("al").value());
            letter->recordFlag=1;
            letter->newLetter=0;
            col=row.child("r1");
            vData=col.child_value();
            dataArray=explode("\n",vData);
            letter->vectorCount=(int)dataArray.size();
            textLineSize+=(letter->y1-letter->y0);
		    string str=letter->name;
            
            //for(int sIn=0;sIn<dataArray.size();sIn++){
            //DM("sIn="<<sIn<<" "<<dataArray[sIn].c_str()<<END);
            //vArray=explode(";",dataArray[sIn]);
            //vectorOCR vector;
            //SH(vArray.size());
            //letter->v[sIn].vStart.x=atoi(vArray[0].c_str());
            //letter->v[sIn].vStart.y=atoi(vArray[1].c_str());
            //letter->v[sIn].vEnd.x=atoi(vArray[2].c_str());
            //letter->v[sIn].vEnd.y=atoi(vArray[3].c_str());
            //letter->v[sIn].vCenter.x=atoi(vArray[4].c_str());
            //letter->v[sIn].vCenter.y=atoi(vArray[5].c_str());
            //letter->v[sIn].length=atoi(vArray[6].c_str());
            //letter->v[sIn].vectorAngle=atoi(vArray[7].c_str());
            //}
            letter->xSum=0;
            letter->ySum=0;
            
            
            for (col = row.child("mask32"); col; col = col.next_sibling("mask32")){
                GBitMask32 mask;
                mask.xMask=atoi(col.attribute("x").value());
                mask.yMask=atoi(col.attribute("y").value());
                if(mask.xMask>80||mask.xMask<-80)continue;
                if(mask.yMask>80||mask.yMask<-80)continue;
                mask.mW=32;
                mask.imgW=atoi(col.attribute("w").value()); //SH(mask.w);
                mask.mH=atoi(col.attribute("h").value());
                mask.mWOn=atoi(col.attribute("wR").value());
                mask.mHOn=atoi(col.attribute("hR").value());
                mask.NMask=atoi(col.attribute("s").value());
                mask.maskType=atoi(col.attribute("t").value());
                mask.dlt0=atoi(col.attribute("d0").value());
                mask.dlt1=atoi(col.attribute("d1").value());
                mask.dlt2=atoi(col.attribute("d2").value());
                mask.status=atoi(col.attribute("st").value());
                letter->xSum+=mask.xMask;
                //cout_<<"letter->mask32["<<letter->mask32Count()<<"].xMask="<<mask.xMask<<endl;
                letter->ySum+=mask.yMask;
                
                data=col.child("imgOn");
                vData=data.child_value();
                dataArray=explode(";",vData);
                for(int m=0;m<dataArray.size();m++){
                    sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
                    mask.imgOn[m]=in;
                }
                data=col.child("On");
                vData=data.child_value();
                dataArray=explode(";",vData);
                for(int m=0;m<dataArray.size();m++){
                    sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
                    mask.On[m]=in;
                }
                data=col.child("Off");
                vData=data.child_value();
                dataArray=explode(";",vData);
                for(int m=0;m<dataArray.size();m++){
                    sscanf(dataArray[m].c_str(),"%X",&in); //binaryPrint(in,2,c_out); DP(END);
                    mask.Off[m]=in;
                }
                //mask.printMask(c_out);
                //if(mask.mWReal<1||
                //	 mask.mWReal>32||
                //	 mask.mHReal<1||
                //	 mask.mHReal>64){
                //mask.NormalT(inputData.c_out);
                //}
                //if(base.size()==5)mask.printMask(inputData.c_out);
                mask.id=(uint)letter->mask32Count();
                /*
                 if(mask.mW==0||mask.mW>1000||mask.mH==0||mask.mH>1000){
                 cout<<"&&&&"<<endl;cout<<"mW="<<mask.mW<<"mH="<<mask.mH<<endl;
                 }
                 */
                letter->push_back(mask);
                //cout<<"letter->mask32Count()="<<letter->mask32Count()<<endl;
            }
            if(letter->mask32Count()>0){
                letter->xSum/=letter->mask32Count();
                letter->ySum/=letter->mask32Count();
            }
            /*
             for(int i=0;i<letter->mask32Count();i++){
             if(letter->mask32[i].mW==0||letter->mask32[i].mW>1000||letter->mask32[i].mH==0||letter->mask32[i].mH>1000){cout<<"^^^^"<<endl;cout<<"mW="<<letter->mask32[i].mW<<"mH="<<letter->mask32[i].mH<<endl; exit(0); }
             }
             */
            DT("letter->mask32Count()="<<letter->mask32Count()<<" letter->xSum"<<letter->xSum<<endl);
            col = row.child("mask128"); 
                letter->mask128.xMask=atoi(col.attribute("x").value());
                letter->mask128.yMask=atoi(col.attribute("y").value());
                letter->mask128.mW=atoi(col.attribute("w").value()); //SH(mask.w);
                letter->mask128.mH=atoi(col.attribute("h").value());
                letter->mask128.NMask=atoi(col.attribute("s").value());
                letter->mask128.status=atoi(col.attribute("st").value());
                
                data=col.child("On");
                vData=data.child_value();
                dataArray=explode(";",vData);
                for(int m=0;m<dataArray.size();m++){
                        sscanf(dataArray[m].c_str(),"%X",&in);
                        letter->mask128.On[m]=in;
                        //binaryPrint(in,2);
                }
            col = row.child("maskOriginal"); 
            letter->maskOriginal.xMask=atoi(col.attribute("x").value());
            letter->maskOriginal.yMask=atoi(col.attribute("y").value());
            letter->maskOriginal.mW=atoi(col.attribute("w").value()); //SH(mask.w);
            letter->maskOriginal.mH=atoi(col.attribute("h").value());
            letter->maskOriginal.NMask=atoi(col.attribute("s").value());
            letter->maskOriginal.status=atoi(col.attribute("st").value());
            
            data=col.child("On");
            vData=data.child_value();
            dataArray=explode(";",vData);
            for(int m=0;m<dataArray.size();m++){
                sscanf(dataArray[m].c_str(),"%X",&in);
                letter->maskOriginal.On[m]=in;
                //binaryPrint(in,2);
            }
            col = row.child("mask128"); 
            letter->mask128.xMask=atoi(col.attribute("x").value());
            letter->mask128.yMask=atoi(col.attribute("y").value());
            letter->mask128.mW=atoi(col.attribute("w").value()); //SH(mask.w);
            letter->mask128.mH=atoi(col.attribute("h").value());
            letter->mask128.NMask=atoi(col.attribute("s").value());
            letter->mask128.status=atoi(col.attribute("st").value());
            
            data=col.child("On");
            vData=data.child_value();
            dataArray=explode(";",vData);
            for(int m=0;m<dataArray.size();m++){
                sscanf(dataArray[m].c_str(),"%X",&in);
                letter->mask128.On[m]=in;
                //binaryPrint(in,2);
            }
            
#ifdef CMATRIX        
            col = row.child("cMatrix");
            vData=col.child_value();
            dataArray=explode(";",vData);       
            for(int m=0;m<dataArray.size();m+=2){
                 sscanf(dataArray[m].c_str(),"%X",&in);
                 *cMatrix[0][m]=in;
            }
#endif            
            
            //if(logicProcessor->uniTibTable.find(letter->name)!=logicProcessor->uniTibTable.end()){
            //    letter->baseLetter=logicProcessor->uniTibTable[letter->name].baseLetter;
            //}
            //cout_<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@ "<<letter->letterIndex<<endl;
            push_back(letter);
            //letter->destroy();
            /*
             string str_;
             getStr(letter->letterIndex,str_);
             if(str_.size()==0||str_.size()>100000)cout<<"s="<<str_.size()<<"i="<<letter->letterIndex<<endl;
             */
            
		}
		if (letterCount())textLineSize/=letterCount();
		cout<<"letterCount"<<letterCount()<<endl;
        
		//exit(0);
    }//////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    void GFont::writeGFontXML(){
        int i;
        string record;
        xml_document doc,docRecord;
        xml_node mainNode;
        xml_node book, data, key;
        mainNode=doc.append_child();
        mainNode.set_name("GFont");
        char cbuf[64];   //SH(base.size());
        
        //unsigned long long in64;
        unsigned int it;
        string str;
        int print=1;
        DR("base.size()="<<letterCount()<<endl)
        book=mainNode.append_child();
        book.set_name("fontName");
        book.append_attribute("n")=fontName.c_str();
        
		
        for(i=0;i<letterCount();i++){
            GLetter *letter=getLetter(i);
            
            DR("letter.name="<<letter->name<<" i:"<<i<<endl);
            //cout_<<"i="<<i<<endl;
            
            if(letter->OCRStatus==100)continue;
            
            if(letter->recordFlag==0||letter->name==":)"||letter->name=="*"){
                letter->destroy();continue;
            }
            book=mainNode.append_child();
            //sprintf(cbuf,"%d",i);
            DT("i="<<i<<" name="<<letter->name.c_str()<<"letter.stackFlag="<<letter->stackFlag<<END);
            
            book.set_name("rec");
            book.append_attribute("id")=(uint)letter->letterIndex;
            book.append_attribute("n")=letter->name.c_str();
            book.append_attribute("wt")=letter->OCRKey.c_str();
            book.append_attribute("f")=letter->stackFlag;
            book.append_attribute("h")=letter->letterH;
            book.append_attribute("w")=letter->letterW;
            book.append_attribute("dX")=letter->dX;
            book.append_attribute("dY")=letter->dY;
            book.append_attribute("s")=(unsigned int)letter->lengthSum;
            //book.append_attribute("k")=letter->OCRKey;
            book.append_attribute("in")=letter->OCRIndex;
            book.append_attribute("l")=letter->baseLetter;
            book.append_attribute("c")=letter->Character;
            book.append_attribute("nb")=letter->newLetterInBook;
            book.append_attribute("sC")=letter->selfCorrelation;
            book.append_attribute("cS")=letter->codeSpace;
            book.append_attribute("al")=letter->onOffRate;
            book.append_attribute("y0")=letter->y0;
            book.append_attribute("y1")=letter->y1;
            if(letter->crSum&&letter->cn){   //вычисляем статистику использования буквы;
                letter->crSum=letter->crSum/letter->cn;
                if(letter->crRes){
                    letter->crRes=(letter->crSum+letter->crRes)/2;
                }else{
                    letter->crRes=letter->crSum;
                }
                letter->crSum=letter->crSum=0;
            }
            book.append_attribute("cn")=letter->cn;
            book.append_attribute("cnr")=letter->cnRes;
            book.append_attribute("cr")=letter->crRes;
            
            for(int in=0; in<letter->mask32Count();in++){
                if(letter->mask32[in].status<1)continue;
                //if(letter.mask32[in].yMask>50||letter.mask32[in].yMask<-50)continue;
                data=book.append_child();
                data.set_name("mask32");
                data.append_attribute("x")=letter->mask32[in].xMask;
                data.append_attribute("y")=letter->mask32[in].yMask;
                data.append_attribute("w")=letter->mask32[in].imgW;
                data.append_attribute("wR")=letter->mask32[in].mWOn;
                data.append_attribute("h")=letter->mask32[in].mH;
                data.append_attribute("hR")=letter->mask32[in].mHOn;
                data.append_attribute("s")=letter->mask32[in].NMask;
                data.append_attribute("t")=letter->mask32[in].maskType;
                data.append_attribute("d0")=letter->mask32[in].dlt0;
                data.append_attribute("d1")=letter->mask32[in].dlt1;
                data.append_attribute("d2")=letter->mask32[in].dlt2;
                data.append_attribute("st")=letter->mask32[in].status;
                
                key=data.append_child();
                key.set_name("imgOn");
                record="";
                for(int m=0;m<letter->mask32[in].mH;m++) {
                    sprintf(cbuf,"%X",letter->mask32[in].imgOn[m]); record+=cbuf;record+=";";
                }key.append_child(node_pcdata).set_value(record.c_str());
                
                key=data.append_child();
                key.set_name("On");
                record="";
                for(int m=0;m<letter->mask32[in].mH;m++) {
                    sprintf(cbuf,"%X",letter->mask32[in].On[m]); record+=cbuf;record+=";";
                }key.append_child(node_pcdata).set_value(record.c_str());
                record="";
                
                key=data.append_child();
                key.set_name("Off");
                for(int m=0;m<letter->mask32[in].mH;m++) {
                    sprintf(cbuf,"%X",letter->mask32[in].Off[m]); record+=cbuf;record+=";";
                }key.append_child(node_pcdata).set_value(record.c_str());
            }
            
            data=book.append_child();
            data.set_name("mask128");
            data.append_attribute("x")=letter->mask128.xMask;
            data.append_attribute("y")=letter->mask128.yMask;
            data.append_attribute("w")=letter->mask128.mW;
            data.append_attribute("h")=letter->mask128.mH;
            data.append_attribute("s")=letter->mask128.NMask;
            data.append_attribute("st")=letter->mask128.status;
            key=data.append_child();
            key.set_name("On");
            record="";
            int t=letter->mask128.mH*letter->mask128.mW/32;
            for(int m=0;m<t;m++) {
                it=letter->mask128.On[m];
                sprintf(cbuf,"%X",it); record+=cbuf;record+=";";
            } key.append_child(node_pcdata).set_value(record.c_str());
            record="";
            
            data=book.append_child();
            data.set_name("maskOriginal");
            data.append_attribute("x")=letter->maskOriginal.xMask;
            data.append_attribute("y")=letter->maskOriginal.yMask;
            data.append_attribute("w")=letter->maskOriginal.mW;
            data.append_attribute("h")=letter->maskOriginal.mH;
            data.append_attribute("s")=letter->maskOriginal.NMask;
            data.append_attribute("st")=letter->maskOriginal.status;
            key=data.append_child();
            key.set_name("On");
            record="";
            t=letter->maskOriginal.mH*letter->maskOriginal.mW/32;
            for(int m=0;m<t;m++) {
                it=letter->maskOriginal.On[m];
                sprintf(cbuf,"%X",it); record+=cbuf;record+=";";
            } key.append_child(node_pcdata).set_value(record.c_str());
            record="";
            letter->destroy();
            
        }
#ifdef CMATRIX        
        book=mainNode.append_child();
        book.set_name("cMatrix");
        book.append_attribute("s")=FSIZE;
        uchar c;
        record="";
        for(int m=0;m<FSIZE*FSIZE;m++) {
            c=*cMatrix[0][m];
            sprintf(cbuf,"%X",c); record+=cbuf;record+=";";
        } data.append_child(node_pcdata).set_value(record.c_str());
        
#endif        
        string path;
        path=inputData.data["GFontPath"];
        if(path=="")path=inputData.data["OCRTables"]+"GFont.xml";
        cout<<"pathXML="<<path<<endl;
        bool exitCode=doc.save_file(path.c_str(),"\t", 1);
        if(exitCode){
            cout_<<"Done. save file "<<path<<endl;
        }else{
            cout_<<"Not saved "<<path<<endl;}
        
    }//////////////////////////////////////////////////////////////////////////
    
    void GFont::writeGFontDB(){
        
    }/////////////////////////////////////////////////////////////////
    
    
    void GFont::readGFontDB(){   //deprecated
        //int step=0;
        /*    for(uint i=0;i<*recordCount-1;i++){
         //if(step==1000){cout<<"."; step=0;} step++;
         //if(letterSet[i]!=NULL)continue;
         GLetter *letter1=GLetter::create();
         TString st;
         //cout<<" index="<<index[i+1]<<endl;
         getTStr(i,st);
         
         letter1->readFromStr(&st);
         
         for(int i=0;i<letter1->mask32Count();i++){
         if(letter1->mask32[i].mW==0||letter1->mask32[i].mW>1000||letter1->mask32[i].mH==0||letter1->mask32[i].mH>1000){cout<<"^^^^"<<endl;cout<<"mW="<<letter1->mask32[i].mW<<"mH="<<letter1->mask32[i].mH<<endl; exit(0); }
         }
         letterSet[i]=letter1;
         
         }
         */
        //cout<<"done";
    };/////////////////////////////////////////////////////////////////
    
    
    void GFont::writeGFontStat(){
        /*
         ostringstream out;
         cout_<<"Start write stat file"<<endl;
         
         for(int i=0;i<letterCount();i++){
         //cout_<<"0_letterSet["<<i<<"]->cn="<<letterSet[i]->cn<<" letterSet["<<i<<"]->crSum="<<letterSet[i]->crSum<<endl;
         if(letterSet[i]->cn&&letterSet[i]->crSum){   //вычисляем статистику использования буквы;
         if(letterSet[i]->crSum)letterSet[i]->crSum=letterSet[i]->crSum/letterSet[i]->cn;
         if(letterSet[i]->crRes){
         letterSet[i]->crRes=(letterSet[i]->crSum+letterSet[i]->crRes)/2;
         }else{
         letterSet[i]->crRes=letterSet[i]->crSum;
         }
         letterSet[i]->crSum=0;
         
         out<<"<rec id=\""<<letterSet[i]->idNumber<<"\" n=\""<<letterSet[i]->name<<"\" cn=\"";
         out<<letterSet[i]->cn<<"\" cnr=\""<<letterSet[i]->cnRes<<"\" cr=\""<<letterSet[i]->crRes<<"\" />\n";
         //cout_<<"1_letterSet["<<i<<"]->cn="<<letterSet[i]->cn<<" letterSet["<<i<<"]->crRes="<<letterSet[i]->crRes<<endl;
         
         }//else{cout_<<"letterSet["<<i<<"]->crSum no data\n";}
         }
         
         string str=out.str();
         writeToLog(str,"/1.txt","/_1.txt");
         cout_<<"Done";
         */
        
        
    };
    
    void GFont::setLineFromArray(vector<stringOCR>&strArray){
        //int print=0;
        /*
         for(int a=0;a<letterCount();a++){
         DT(endl<<"a="<<a<<" y0="<<letterSet[a]->yMin<<" x="<<letterSet[a]->xMin<<" name="<<letterSet[a]->name.c_str()<<" h="<<letterSet[a]->letterH<<endl);
         int y0=letterSet[a]->yMin;
         
         for(int b=strArray.size()-1;b>=0;b--){
         DT("b="<<b<<" strArray["<<b<<"].y0="<<strArray[b].y0<<" strArray["<<b<<"].y1="<<strArray[b].y1<<endl);
         DT("b="<<b-1<<" strArray["<<b-1<<"].y0="<<strArray[b-1].y0<<" strArray["<<b-1<<"].y1="<<strArray[b-1].y1<<" limit="<<strArray[b].y1+(strArray[b-1].y0-strArray[b].y1)/2<<endl);
         
         if(b==0){
         letterSet[a]->y0=strArray[b].y0-letterSet[a]->yMin-letterSet[a]->letterH/2;
         letterSet[a]->y1=strArray[b].y1-letterSet[a]->yMin-letterSet[a]->letterH/2;
         DT("match0 y0="<<letterSet[a]->y0<<" y1="<<letterSet[a]->y1<<endl);
         break;
         }
         if(y0<strArray[b].y1+(strArray[b-1].y0-strArray[b].y1)/2){
         letterSet[a]->y0=strArray[b].y0-letterSet[a]->yMin-letterSet[a]->letterH/2;
         letterSet[a]->y1=strArray[b].y1-letterSet[a]->yMin-letterSet[a]->letterH/2;
         DT("match1 y0="<<letterSet[a]->y0<<" y1="<<letterSet[a]->y1<<endl);
         break;
         }
         }
         
         
         }
         DT("done setLineFromArray"<<endl);
         */
        
    };
    
    
    GBitmap* GFont::drawPict(int in){
        //if(letterSet[in]->pictFlag==0){
        GLetter *letter=getLetter(in);
        GBitmap *img=letter->mask128.unPackMask();
        img->dotGain(-1,-1,-1,-1, 0);
        //добавляем со всех сторон символа по нескольку пикселей
        img->dotGain( 2, 2, 2, 2, 0);
        GBitmap* img_=GBitmap::create(img->columns()+60,img->rows()+120);
        img_->fill(255);
        img_->drawImg(img,30,60);
        img->destroy();
        letter->destroy();
        return img_;
    }
    
    void GFont::setStableFocalPoint(){
    
        for(int i=0;i<letterCount();i++){ if(i%500==0)cout<<i<<" ";
            vector<OCRPoint>str;
            GLetter *letter=getLetter(i);
            GBitmap *img=letter->mask128.unPackMask();
            GGraph outGraph=GGraph(PICT_SIZE,PICT_SIZE);
            int x0=letter->mask128.xMask+PICT_SIZE/2;
            int y0=letter->mask128.yMask+PICT_SIZE/2;
            outGraph.drawImg(img, x0,y0);
            //WriteImageData(outBitmap, "/_Image2OCR/_1.jpg", 0); exit(0);
            
            outGraph.StablePoint(str);
            
            //cout<<"new size="<<str->size()<<" x0="<<x0<<" y0="<<y0<<endl;
            letter->focalPoint->resize(0);
            for(int i=0;i<str.size();i++){
                str[i].x-=PICT_SIZE/2;
                str[i].y-=PICT_SIZE/2;
                letter->focalPoint->push_back(str[i]);
                //cout<<"x="<<letter->fPoint[i].x<<" y="<<letter->fPoint[i].y<<endl;
            }
            saveLetter(letter);
            letter->destroy();
            img->destroy();
        }

    }
    
    void GFont::setLetterStableFocalPoint(uint index){
        
        
            vector<OCRPoint>str;
            GLetter *letter=getLetter(index);
            GBitmap *img=letter->mask128.unPackMask();
            GGraph outGraph=GGraph(PICT_SIZE,PICT_SIZE);
            int x0=letter->mask128.xMask+PICT_SIZE/2;
            int y0=letter->mask128.yMask+PICT_SIZE/2;
            outGraph.drawImg(img, x0,y0);
            //WriteImageData(outBitmap, "/_Image2OCR/_1.jpg", 0); exit(0);
            
            outGraph.StablePoint(str);
            
            //cout<<"new size="<<str->size()<<" x0="<<x0<<" y0="<<y0<<endl;
            letter->focalPoint->resize(0);
            for(int i=0;i<str.size();i++){
                str[i].x-=PICT_SIZE/2;
                str[i].y-=PICT_SIZE/2;
                letter->focalPoint->push_back(str[i]);
                //cout<<"x="<<letter->fPoint[i].x<<" y="<<letter->fPoint[i].y<<endl;
            }
            saveLetter(letter);
            letter->destroy();
            img->destroy();
    }
    
    void GFont::setFocalLine(){
        int c=(int)letterCount();
        for(int i=0;i<c;i++){ if(i%500==0)cout<<i<<" ";
           setFocalLineInLetter(i);
        }
         cout<<"Done set OCRFocalLine in font database."<<endl;
        
    }

    static bool  sortFocalLine(const OCRFocalLine &a, const OCRFocalLine &b){
        int a_=a.len;
        int l=(a.len+b.len)*0.5;
        if(a.start.type==T_POINT||a.start.type==X_POINT)a_+=l;
        if(a.end.type==T_POINT||a.end.type==X_POINT)a_+=l;
        int b_=b.len;
        if(b.start.type==T_POINT||b.start.type==X_POINT)b_+=l;
        if(b.end.type==T_POINT||b.end.type==X_POINT)b_+=l;
        return a_>b_;
    }
    
    void GFont::scaleLetter(GLetter *glyph,float scale){
        GBitmap *img=glyph->mask128.unPackMask();
        img->scaleFast(scale);
        glyph->mask128.packBitMask128(img);
        glyph->eraseAllMask();
        glyph->mask128.xMask*=scale;
        glyph->mask128.yMask*=scale;
    }
    
    
    void GFont::setFocalLineInLetter(uint index){
            vector<OCRPoint>strPoint;
            vector<OCRFocalLine>focalLine;
            GLetter *letter=getLetter(index);
            GBitmap *img=letter->mask128.unPackMask();
            GGraph outGraph=GGraph(PICT_SIZE,PICT_SIZE);
            int x0=letter->mask128.xMask+PICT_SIZE/2;
            int y0=letter->mask128.yMask+PICT_SIZE/2;
            outGraph.drawImg(img, x0,y0);
            //img=GBitmap::create((GBitmap*)&outGraph);
            //((GImageEditor*)inputData.imageEditor)->WriteImageData(img, "/_Image2OCR/_1.jpg", 0); exit(0);
        
            //cout<<" setFocalLineInLetter "<<index<<endl;
            //outGraph.focalLine(15);
            //outGraph.focalPointXTL(strPoint);
            outGraph.StablePoint(strPoint);
            outGraph.drawImg(img, x0,y0);
            outGraph.focalLine(15);
            outGraph.focalLineRecognition(strPoint,focalLine);
        
        
            //cout<<"new size="<<str->size()<<" x0="<<x0<<" y0="<<y0<<endl;
            letter->focalPoint->resize(0);
            letter->focalLine.resize(0);
        
            for(int i=0;i<strPoint.size();i++){
                OCRPoint p=strPoint[i];
                p.x-=PICT_SIZE/2;
                p.y-=PICT_SIZE/2;
                letter->focalPoint->push_back(p);
                //cout<<"x="<<letter->fPoint[i].x<<" y="<<letter->fPoint[i].y<<endl;
            }

            int c=(int)focalLine.size();
            //cout<<index<<" c="<<c<<endl;
            //int maxSide=(max(letter->letterH,letter->letterW))/10;
            for(int i=0;i<c;i++){
                OCRFocalLine &line=focalLine[i];
                //if(line.len>maxSide){
                line.setDimension(PICT_SIZE/2,PICT_SIZE/2);
                
                letter->focalLine.push_back(line);
                ///}
                //cout<<"x="<<letter->focalLine[i].start.x<<" y="<<letter->focalLine[i].start.y<<endl;
                //cout<<"x="<<letter->focalLine[i].centerM.x<<" y="<<letter->focalLine[i].centerM.y<<endl;
            }
        
            //set OCRLine points type
            c=(uint)letter->focalLine.size();
            for(int i=0;i<c;i++){
                int x1=letter->focalLine[i].start.x;
                int x2=letter->focalLine[i].end.x;
                int y1=letter->focalLine[i].start.y;
                int y2=letter->focalLine[i].end.y;
                for(int n=0;n<strPoint.size();n++){
                    if(strPoint[n].x==x1&&strPoint[n].y==y1){
                        letter->focalLine[i].start.type=strPoint[n].type;
                    }
                    if(strPoint[n].x==x2&&strPoint[n].y==y2){
                        letter->focalLine[i].end.type=strPoint[n].type;
                    }
                }
            
            }
            //определяем линии примыкающие к стартовой точке
            /*
            for(int i=0;i<c;i++){
                int x1=focalLine[i].start.x;
                int x2=focalLine[i].end.x;
                int y1=focalLine[i].start.y;
                int y2=focalLine[i].end.y;
                for(int n=0;n<c;n++){
                    if(n==i)continue;
                    if(focalLine[n].start.x==x1&&focalLine[n].start.y==y1){
                        OCRLineRef ref(focalLine[n],focalLine[i].len);
                        letter->focalLine[i].startL.push_back(ref);
                    }
                    if(focalLine[n].end.x==x1&&focalLine[n].end.y==y1){
                        OCRLineRef ref(focalLine[n],focalLine[i].len);
                        letter->focalLine[i].startL.push_back(ref);
                    }
                    
                    if(focalLine[n].start.x==x2&&focalLine[n].start.y==y2){
                        OCRLineRef ref(focalLine[n],focalLine[i].len);
                        letter->focalLine[i].endL.push_back(ref);
                    }
                    if(focalLine[n].end.x==x2&&focalLine[n].end.y==y2){
                        OCRLineRef ref(focalLine[n],focalLine[i].len);
                        letter->focalLine[i].endL.push_back(ref);
                    }
                }
            }
            */ 
            sort(letter->focalLine.begin(),letter->focalLine.end(),sortFocalLine);
        
            //for(int i=0;i<letter->focalLine[0].mData.size();i++){
               //OCRLineRef ref=focalLine[0].startL[i];
               //cout<<"i="<<ref.index<<" a="<<ref.alpha<<" l="<<ref.len<<endl;
               //cout<<"x="<<letter->focalLine[0].mData[i].x<<" y="<<letter->focalLine[0].mData[i].y<<endl;
            //}
        
            //cout<<"count1="<<c<<" len="<<focalLine[0].len<<" s="<<focalLine[0].dataVector->size()<<endl;
            saveLetter(letter);
            letter->destroy();

            //letter=getLetter(index);
            //int count=(int)letter->focalLine.size();
            //cout<<"count="<<count<<" len="<<focalLine[0].len<<" s="<<focalLine[0].dataVector->size()<<endl;
             //for(int i=0;i<count;i++){
             //   cout<<"x="<<letter->focalLine[i].start.x<<" y="<<letter->focalLine[i].start.y<<endl;
             //}
             //
            img->destroy();
        
    }
    

    
};
