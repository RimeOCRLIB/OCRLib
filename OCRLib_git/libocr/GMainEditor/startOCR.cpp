#include "GMainEditor.h"
#include "php2stl.h"


///диспетчер выполнения процесса распознавания страницы.
void GMainEditor::startOCR(GBitmap *pechaImg_){

    cout<<"Start#1 "<<inputData.data["inputFile"]<<END; 
    if(!pechaImg_){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
    pechaImg=pechaImg_;
    
    imageEditor->adaptiveFiltration(pechaImg,1,0);
	
    vector<stringOCR>strArray;  
    int border;
    string str;
    int draw=1;
    int print=0;
    
	DT("@4_1");
    //vectorBase[0].allVectorCount=0;
	border=0; 
    setBit=GBitset::createResize(pechaImg_,1,1,1);
    DT("@4_2");
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
    DT("@4_3"); 
    border=setBit->border_size();
    DT("@4_4");      
    
    //fontEditor->setLanguage();

    GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg_,1,1);
    //if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
    TIME_START
    str=fileName(inputData.data["inputFile"]); DT("@@@ file="<<str);
    
    if(inputData.data["ocrData"]=="testOCRLine"||inputData.data["ocrData"]=="OCRTraining"){
        //находим тестовую строку для распознавания
        int bestIndex=0; int minDistance=1000;
        int t;
        for(int a=0;a<strArray.size();a++){
            strArray[a].selectFlag=0;
            t=abs((int)(pechaImg->rows()/2-strArray[a].y0));
            if(t<minDistance){minDistance=t;bestIndex=a;}
        }
        strArray[bestIndex].selectFlag=1;
        for(int a=0;a<strArray.size();a++){
           // strArray[a].selectFlag=1; //распознавать все строки теста
           if(!strArray[a].selectFlag)strArray[a].wordArray.resize(0); //распознавать центральную строку теста
        }
        cout_<<"bestIndex="<<bestIndex<<endl;
    }    
    
    
    setOCR->setData(
                    aliKali,
                    strArray,
                    correctionTable,
                    logicProcessor,
                    iLeft,
                    iTop,
                    border,
                    ALL_LETTER);
	
    TIME_PRINT
    mainString=setOCR->mainString;
    string xmlString=setOCR->xmlString;
	
    
    //cout_<<"time="<<time<<" mainString"<<mainString<<END;
    
    DT("@6");
    //DM("//______timeALL______="<<time1<<END);tm_start1=clock();
    if(setBit!=NULL){
        setBit->destroy();      
    }
    if(setOCR!=NULL){
        
        if(draw){
            str="/__1draw.jpg";
#ifdef DEBUGLVL_DRAW						
            //if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif
#ifdef DEBUGLVL_GREY1
            if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif
        }
        DT("start release setOCR  inputData.data[ocrData]="<< inputData.data["ocrData"]<<endl);
        setOCR->destroy();
        
        DT("draw0");
        
        int max=0; int maxIndex=-1;
        for(int n=0;n<strArray.size();n++){
            if(strArray[n].line.size()>max){
                max=strArray[n].line.size();
                maxIndex=n;
            }
        }
        
        if(inputData.data["ocrData"]=="batchOCR"||
           inputData.data["ocrData"]=="letterBlock"||
           inputData.data["ocrData"]=="OCRTrainingOne"||
           inputData.data["ocrData"]=="OCRTrainingOneFile"
           ){
            //logicProcessor->drawGrapeLine(strArray[maxIndex].line);
            drawOCRPage(strArray);
            //drawStrArrayDebug(strArray,border);
            writePageXML(xmlString);
            //cout<<"#";
            
        }    
        if(inputData.data["ocrData"]=="testOCRLine"||
           inputData.data["ocrData"]=="OCRTraining"){
            //drawStrArrayDebug(strArray,border);
            cout<<" maxIndex="<<maxIndex<<endl;
            if(maxIndex!=-1)logicProcessor->drawGrapeLine(strArray[maxIndex].line);
            
            
        }    
        DT("draw1");
        //if(inputData.data["ocrData"]=="ocrBatch")drawLettersInCorrectionPage(LETTER_ADD);	
        DT("draw2");
    }
    //strArray.resize(0);
    
    DR_("doneStartOCR"<<endl);

    pechaDataLoaded=0;
    //cout<<"done "<<inputData.data["inputFile"]<<END;
    
}//________________________________________________________________________________________________________________

void GMainEditor::startOCRSystem(){
    
	string strHeaderHTML,srcLine,str;
	string  path=inputData.data["tablePath"]+"/header.xml";
	DIR *dir;
	//int mode;
	int i=0;
    //читаем статистику использования букв книги
    //readLetterStat();

    int maxProcess=inputData.num_cores*0.5;
    pidID *pidIDArray;
    
    if(inputData.data["pathDB"]!=""){
        inputData.data["inputFile"]=inputData.fileList[0];
        pechaImg=LoadImageData(inputData.data["inputFile"],0);
        startOCR(pechaImg);

        MemoryFile *pidData_mf=MemoryFile::create(inputData.data["pathDB"].c_str(),
                                                  MemoryFile::if_exists_keep_if_dont_exists_create);
        int ID=atoi(inputData.data["ID"].c_str());
        pidIDArray=(pidID*)pidData_mf->data(); //array which can be share between processes.
        pidIDArray[ID].status=0;
        cout<<"done OCR";
        inputData.log<<" done OCR"<<endl;
        inputData.log.close();
        remove(inputData.data["log"].c_str());
        exit(0);
    }
    
    if(inputData.fileList.size()>1){
        int countProcess=0;
        MemoryFile *pidData_mf;  //main file for inter proccess communications
        //inputData.data["statPath"].c_str()
        time_t seconds;    seconds = time (NULL);
        ostringstream out;  out<<"/tmp/"<<seconds;
        string pathDB=out.str();
        pidData_mf=MemoryFile::create(pathDB.c_str(), MemoryFile::if_exists_keep_if_dont_exists_create);
        pidData_mf->resize(sizeof(pidID)*maxProcess);
        pidIDArray=(pidID*)pidData_mf->data(); //array which can be share between processes.
        
        
        for(int index=0;index<maxProcess;index++){
            //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
            pidIDArray[index].status=0;
        }
        
        int ID=0;
        
        while(i<inputData.fileList.size()){
            cout<<"NEW file#1 "<<inputData.fileList[i]<<endl;
            if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
                i++; continue;
            }
            
            inputData.data["inputFile"]=inputData.fileList[i];
            string next="";
            if(i<inputData.fileList.size()-2){
                next=inputData.fileList[i+1];
                next=str_replace(".tif",".html",next);
                next=str_replace(".jpg",".html",next);
            }
            
            string  path=inputData.data["inputFile"];    //проверяем есть ли такой распознаный файл
            path=substr(0,(int)path.rfind("."),path);
            string volume=path;
            string fileIndex=fileName(path);
            path+=".xml";
            if(is_file(path)){i++; continue;}
            ostringstream out;
            out<<inputData.data["rootApp"]<<" \"xml=<fileList>"<<inputData.data["inputFile"]<<
            "</fileList><ocrData>"<<inputData.data["ocrData"]<<"</ocrData>"<<
            "<ocrLn>"<<inputData.data["ocrLn"]<<"</ocrLn>"<<"<pathDB>"<<pathDB<<"</pathDB>"<<
            "<ID>"<<ID<<"</ID><nextPage>"<<next<<"</nextPage><scale>"<<inputData.data["scale"]<<"</scale>\" &";
            string cmd=out.str();
            //cout<<cmd; exit(0);
            system(cmd.c_str());
            //sleep(1); //continue;
            
            
            i++;if(i==inputData.fileList.size())break;
            countProcess++;
            
            for(int index=0;index<maxProcess;index++)cout<<pidIDArray[index].status<<" ";
            cout<<endl;
            
            ID=100; int status;
            while(ID==100){
                if(countProcess<=maxProcess){     //есть свободные слоты для новых процессов
                    for(int index=0;index<maxProcess;index++){  //маркируем слот как занятый
                        //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
                        if(pidIDArray[index].status==0){
                            ID=index;
                            pidIDArray[index].status=1;
                            break;
                        }
                    }
                }else{
                    int processStatusCount=0;
                    for(int index=0;index<maxProcess;index++){   //подсчитываем количество активных процессов
                        if(pidIDArray[index].status==1)processStatusCount++;
                    }
                    if(processStatusCount==countProcess){sleep(1); continue;};     //ждем завершения процесса
                    countProcess--;
                }
            }
        }
        
    }else{
        inputData.data["inputFile"]=inputData.fileList[0];
        pechaImg=LoadImageData(inputData.data["inputFile"],0);
        startOCR(pechaImg);
    }
}


void GMainEditor::startOCRBatch(){
    
    imageEditor=(GImageEditor*)inputData.imageEditor;
    fontEditor=(GFontEditor*)inputData.fontEditor;
    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
    if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary();
    aliKali=fontEditor->aliKali;

    
	string strHeaderHTML,srcLine,str;
	string  path=inputData.data["tablePath"]+"/header.xml";
	DIR *dir;
	//int mode;	
	int i=0;
    //читаем статистику использования букв книги
    //readLetterStat();
    //загружаем базу данных букв
    
#ifdef FORK
     int maxFork=inputData.num_cores*0.75;
     int pid;
     pidID *pidIDArray;
     
    
    if(inputData.fileList.size()>1){
        int countFork=0; 
        MemoryFile *pidData_mf;  //main file for conection with child process
        //inputData.data["statPath"].c_str()
        time_t seconds;    seconds = time (NULL);
        ostringstream out;  out<<"/tmp/"<<seconds;
        string path=out.str();
        pidData_mf=MemoryFile::create(path.c_str(), MemoryFile::if_exists_keep_if_dont_exists_create);
        pidData_mf->resize(sizeof(pidID)*maxFork);
        pidIDArray=(pidID*)pidData_mf->data(); //array which can be share between processes.
        
        
        for(int index=0;index<maxFork;index++){
            //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
            pidIDArray[index].status=0;
        }

        int ID=0;
        
        while(i<inputData.fileList.size()){
            cout<<"NEW file#1 "<<inputData.fileList[i]<<endl;
            if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
                i++; continue;
            }	
            
            inputData.data["inputFile"]=inputData.fileList[i];
            
            string  path=inputData.data["inputFile"];    //проверяем есть ли такой распознаный файл
            path=substr(0,(int)path.rfind("."),path);
            string volume=path;
            string fileIndex=fileName(path);
            path+=".xml";
            if(is_file(path)){i++; continue;}
            
            
            if(!forkProccesOCR_(pidIDArray,ID,maxFork)){
                cout<<"ERROR on fork return";
                sleep(1); continue; 
            };
            i++;if(i==inputData.fileList.size())break;
            countFork++;

            for(int index=0;index<maxFork;index++)cout<<pidIDArray[index].status<<" ";
            cout<<endl;
            
            ID=100; int status;
            while(ID==100){
                if(countFork<=maxFork){     //есть свободные слоты для новых процессов
                    for(int index=0;index<maxFork;index++){  //маркируем слот как занятый
                        //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
                        if(pidIDArray[index].status==0){
                          ID=index;
                          pidIDArray[index].status=1;
                          break;
                        }    
                    }
                }else{
                    int forkStatusCount=0;
                    for(int index=0;index<maxFork;index++){   //подсчитываем количество активных процессов
                        if(pidIDArray[index].status==1)forkStatusCount++;
                    }
                    if(forkStatusCount==countFork){sleep(1); continue;};     //ждем завершения процесса
                    wait(&status);                           //регистрируем с системе завершенный процесс
                    countFork--;
                }   
            }	
        }
            
    }else{
        inputData.data["inputFile"]=inputData.fileList[0];
        pechaImg=LoadImageData(inputData.data["inputFile"],0); 
        startOCR(pechaImg);
    }
    
    
#else
    cout<<"NO FORK";
	while(i<inputData.fileList.size()){
		if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
			readDirectoryToArray(inputData.fileList, inputData.fileList[i],"img");
			i++; continue;
		}	
		GBitmap* pechaImg;
		inputData.data["inputFile"]=inputData.fileList[i];

        if(!is_file(inputData.data["inputFile"]))continue;
        pechaImg=LoadImageData(inputData.data["inputFile"],0); 
        if(!pechaImg){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
        
        str=inputData.data["inputFile"];
		str=substr(0,str.rfind("."),str);
		str+=".html";   //cout_<<str<<END;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr(inputData.data["siteRoot"].size(),inputData.data["inputFile"]);

		//readPageHTML();
		inputData.c_out.open(str.c_str());
		pechaDataLoaded=0;
		startOCR(pechaImg);
        pechaImg->destroy();
		inputData.c_out.close();
		i++;
	}
	
#endif		
	//drawLettersInCorrectionTable(DRAW_BASE);
	
	cout_<<"COMPLETE"<<endl;
	
}//____________________________________________________________________________


bool GMainEditor::forkProccesOCR_(pidID *pidIDArray,int ID, int maxFork){
    string str;
    pechaImg=LoadImageData(inputData.data["inputFile"],0);
    int pidID=fork();
    
	if (pidID < 0){
		error((char*)"ERROR on fork");
        return 0;
    }    
	if (pidID == 0)  {
		str=substr(0,str.rfind("."),str);
		str+=".html";   cout_<<str<<END;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr(inputData.data["siteRoot"].size(),inputData.data["inputFile"]);
		//readPageHTML();
        cout<<"NEW START "<<inputData.data["inputFile"]<<endl;
		pechaDataLoaded=0;
		startOCR(pechaImg);
        pidIDArray[ID].status=0;
        cout<<"done "<<ID<<endl;
        remove(inputData.data["log"].c_str());
		exit(0);
	}
    pechaImg->destroy();
    return 1;
}



void GMainEditor::forkProccesOCR(int pidID,GBitmap *pechaImgID_){

	if (pidID < 0)
		error((char*)"ERROR on fork");
	if (pidID == 0)  {
		string str=inputData.data["inputFile"];
		str=substr(0,(int)str.rfind("."),str);
		str+=".html";   cout_<<str<<END;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr((int)inputData.data["siteRoot"].size(),inputData.data["inputFile"]);
		//readPageHTML();
        cout_<<"NEW START "<<inputData.data["inputFile"]<<endl;
		pechaDataLoaded=0;
		startOCR(pechaImgID_);
		//if(stat)writeLetterStat();
		exit(0);
	}
}


///диспетчер выполнения процесса создания  OCR  шрифта. Распознаем текст как контура, заносим в базу и распознаем.
void GMainEditor::buildOCRFont(){
    // Автоматическое масштабирование размеров картинки
    //ShowMessage("1start");
	cout_<<"Start#1 "<<inputData.data["inputFile"]<<END;
	if(aliKali==NULL){aliKali=GFont::create();}
	int border;
	string str;
	int draw=1;
	int print=1;
	
	
	for(int index=0;index<inputData.fileList.size();index++){		
        
        inputData.data["inputFile"]=inputData.fileList[index];
        if(!is_file(inputData.data["inputFile"]))continue;
        pechaImg=LoadImageData(inputData.data["inputFile"],0); 
        if(!pechaImg){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
        
        vector<stringOCR>strArray;  
        
        if(inputData.data["ocrData"]=="drawLetter"){
            draw=1;
            //inputData.c_out<<"<html><body><div style=\"position:absolute; left:0px; top:0px;\">";
            //inputData.c_out<<"<img src=\"__1draw.gif\"><br><div style=\"position:relative; left:20px;\">"<<END;
        }
        DT("@4_1");
        //vectorBase[0].allVectorCount=0;
        border=0; //clock_t  tm_start1, tm_end1; double time1;  tm_start1=clock();
        setBit=GBitset::createResize(pechaImg,1,1,1);
        DT("@4_2");
        setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        DT("@4_3");
        border=setBit->border_size();
        DT("@4_4");
        
        //cout_<<"strArray.size()="<<strArray.size()<<END;
        //cout_<<"<div style=\"position:absolute; left:0px; top:0px;\">";
        //cout_<<"<img src=\"http://localhost/"<<inputData.inputFileName<<"\">\n";
        ///str=inputData.data["outFolder"]+"/1_out.jpg";
        //WriteImageData(pechaImg,"/2.jpg",1);    return;
        
        GFont* contourBase=GFont::create();
        //for(int a=0;a<2000;a++){
        //    GLetter *letter=GLetter::create();
        //    contourBase->letterSet[a]=letter;
        //    contourBase->letterSet[a]->recordStatus=0;
        // }
        
        GBitsetContour* setContour=GBitsetContour::createResize(pechaImg,1,1,0);
        //setContour->drawContour();
        //WriteImageData(setContour->drawData,"/1.jpg",1);    return;
        
        setContour->buildGFont(contourBase,strArray);
        
        //int index=0;
        // while(1){
        
        //if(contourBase->letterSet[index]->recordStatus)contourBase->letterSet[index]->drawContour();
        //break;   
        
        //}
        int maxID=0;
        //for(int a=0;a<aliKali->letterCount();a++){
        //    if(aliKali->letterSet[a]->idNumber>maxID)maxID=aliKali->letterSet[a]->idNumber;
       // }
        contourBase->setLineFromArray(strArray);
        /*for(int a=0;a<contourBase->letterCount();a++){
         aliKali->letterSet[aliKali->letterCount()]=contourBase->letterSet[a];
         maxID++;
         aliKali->letterSet[aliKali->letterCount()]->idNumber=maxID;
         
         }
         */  
	    pechaImg->destroy();
		setContour->destroy();
		
    }
    fontEditor->aliKali=aliKali;  
    
    //WriteImageData(setContour->drawData,"/1.jpg",1);   
    
    
    
}


//test letter block from correction table and draw result
string GMainEditor::letterBlockCorrelation(unsigned int in){
   
	cout_<<"letterBlockCorrelation"<<endl;
	int print=1;
	
	int indexLetter=indexById(correctionTable,in);
	//if(pechaDataLoaded)pechaImg->destroy();
    //pechaImg=	
	vector<stringOCR>strArray;
	string str;
    GBitmap *letterImg=loadFromTiffString(correctionTable[indexLetter].img->TIFF_string,1);
	
	GBitset *setBit=GBitset::createResize(letterImg,1,1,1);
	cout_<<"@4_2";
	setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
	cout_<<"@4_3";
	if(strArray.size()>1)strArray[0].wordArray.resize(0);
	
	cout_<<" setBit->border_size()="<<setBit->border_size()<<endl;
	
	
	GBitsetOCR *setOCR=GBitsetOCR::createResize(letterImg,1,1);
	setOCR->setData(
					aliKali,
					strArray,
					correctionTable,
					logicProcessor,
					0,
					0,
					setBit->border_size(),
					ALL_LETTER);
	
	mainString=setOCR->mainString;
	cout_<<mainString<<"strArray="<<strArray[0].wordArray.size();
	
str="/__1draw.jpg";
#ifdef DEBUGLVL_DRAW						
	if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif
#ifdef DEBUGLVL_GREY1
	if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif
	
	
	ostringstream c_out1;
    int x0,y0,x1,y1,gY,index=0;
	
	// заполнение массива координат прямоугольника
	for (int strInd=(int)strArray.size()-1; strInd >=0; strInd-- ){
		x0=strArray[strInd].x0;   x1=strArray[strInd].x1;
		y0=strArray[strInd].y0;   y1=strArray[strInd].y1;
		c_out1<<"<div class=\"sT\" id=\"s"<<strInd<<"\" style=\"left:";
		c_out1<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
		c_out1<<y1-y0<<"px;\" ";
		c_out1<<"LimY0=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"LimY1=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"y0=\""<<strArray[strInd].y0<<"\" ";
		c_out1<<"y1=\""<<strArray[strInd].y1<<"\" ";
		c_out1<<"x1=\""<<strArray[strInd].x1<<"\" ></div>\n";
		
		for (int m=0; m < strArray[strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[strInd].wordArray[m];
			if(wP->correlation<0)continue;  //word marked as text frame etc.
			x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
			//gY=strArray[strInd].y1-y0+10;
			gY=0;
			//DM("y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" LimY0="<<strArray[strInd].LimY0<<" LimY1="<<strArray[strInd].LimY1<<END);
			if(x1-x0>0){
				c_out1<<"<div class=\"dE\" id=\"g"<<index<<"\" c=\""<<wP->correlation;
				c_out1<<"\" style=\"left:"<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
				c_out1<<y1-y0<<"px; ";
				if(wP->correlation<70){c_out1<<" border-color:#ff0000; border-width:3px;";}
				if(wP->correlation>70&&wP->correlation<80){c_out1<<" border-color:#cc0033; border-width:2px;";}
				
				c_out1<<"\">\n<div class=\"dM\" style=\"width: "<<x1-x0<<"px; height:3px;\"></div>\n";
				//c_out1<<wP->OCRIndex;
				c_out1<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:"<<gY<<"px\" onClick=\"edit(";
				c_out1<<index<<")\" onBlur=\"setText("<<index<<")\">"<<wP->name<<"</div></div>";
				index++;
			}
			DT("n="<<wP->name.c_str()<<" c="<<wP->correlation<<END);
			
		} // m
		
	} // strInd
	
	str=c_out1.str();
	
	
	string destStr=drawEditLetterBlock(str,in);
	return destStr;

}



//распознавание одной строки текста
void GMainEditor::lineOCR(){
    int print=1;
    /*
    pechaImg=LoadImageData(inputData.data["inputFile"],0);
    vector<stringOCR>strArray;
    
    int border=0; 
    setBit=GBitset::createResize(pechaImg,1,1,1);
    DT("@4_2");
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )  
    DT("@4_3");
    border=setBit->border_size();
    DT("@4_4");
    
    fontEditor->setLanguage();

    
    GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
    setOCR->setData(
                    aliKali,
                    strArray,
                    correctionTable,
                    logicProcessor,
                    iLeft,
                    iTop,
                    border,
                    ALL_LETTER);
    mainString=setOCR->mainString;
    setOCR->destroy();
    */
    
    
    cout<<"done "<<inputData.data["inputFile"]<<END;
    inputData.data["ocrData"]="regionOCR";
    
}//_______________________________________________________













