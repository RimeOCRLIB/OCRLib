#include "GMainEditor.h"

///диспетчер выполнения процесса распознавания страницы.
void GMainEditor::startOCR(GBitmap *pechaImg_){

    cout<<"Start#1 "<<inputData.data["inputFile"]<<endl;
    if(!pechaImg_){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
    pechaImg=pechaImg_;
    int border=0;
    string str;
    float param[11];
    param[0]=1;
    
    ((GLogicProcessor*)inputData.logicProcessor)->aliKali=aliKali;
    
    inputData.ocrCode["OCR_MATRIX_CORRELATION_LIMIT"]=77;
    
    /*
    if(inputData.data["ocrData"]=="batchOCR"||inputData.data["ocrData"]=="lineOCR"){
        //imageEditor->setImageByOCR(pechaImg,param,1);
        aliKali->letterSet.resize(0);
        //param[0]=1;
        if(param[0]!=-1){
            imageEditor->adaptiveFiltration1(pechaImg,param,1);
            if(strArray.size()){
                setBit->destroy();
                setOCR->destroy();
                strArray.resize(0);
            }
        }else{
            //если на странице не определен масштаб, то скорее всего это изображение
            //такие страницы могут занять большой объем в памяти прираспознавании.
            //перед распознаванием таких страниц их необходимо проверить детектором на наличие текста.
            cout<<"no detectabel text on page "<<inputData.data["inputFile"]<<endl;
            mainString="";
            drawOCRPage(strArray);
            pechaImg_->destroy();
            DR_("doneStartOCR_1"<<endl);
            pechaDataLoaded=0;
            return;
        }

    }
    */
    
    //((GImageEditor*)inputData.imageEditor)->WriteImageData(pechaImg,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
    
    //set OCRFont letters probability
    //((GFontEditor*)inputData.fontEditor)->setLettersProbability();

   
    if(!strArray.size()){
        setBit=GBitset::createResize(pechaImg_,1,1,1);
        setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        border=setBit->border_size();
        setBit->destroy();
        setOCR=GBitsetOCR::createResize(pechaImg_,1,1);
        //if(NewLetterOnlyBtn->Checked==true) {mode=letterID;}else{mode=ALL_LETTER;}
        str=fileName(inputData.data["inputFile"]); DT("@@@ file="<<str);
        
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


    mainString=setOCR->mainString;
    //string xmlString=setOCR->xmlString;
	
    
    //cout_<<"time="<<time<<" mainString"<<mainString<<endl;

    //DM("//______timeALL______="<<time1<<END);tm_start1=clock();

    if(setOCR!=NULL){
        
    str="/__1draw.jpg";
#ifdef DEBUGLVL_DRAW						
            //if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawDataRGB,str,0);
#endif
#ifdef DEBUGLVL_GREY1
            if(setOCR->setMatrix!=NULL)WriteImageData(setOCR->setMatrix->drawData,str,0);
#endif

        DT("start release setOCR  inputData.data[ocrData]="<< inputData.data["ocrData"]<<endl);
        setOCR->destroy();
        
        DT("draw0");
        
        int max=0; int maxIndex=-1;
        for(int n=0;n<strArray.size();n++){
            if(strArray[n].line.size()>max){
                max=(int)strArray[n].line.size();
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
    pechaImg_->destroy();

    
    DR_("doneStartOCR"<<endl);

    pechaDataLoaded=0;
    //cout<<"done "<<inputData.data["inputFile"]<<endl;
    
}//________________________________________________________________________________________________________________

void GMainEditor::startOCRSystem(){
    
	string strHeaderHTML,srcLine,str;
	string  path=inputData.data["tablePath"]+"/header.xml";
	DIR *dir;
	//int mode;
	int i=0;
    uint freeMemory;
    //читаем статистику использования букв книги
    //readLetterStat();

    int maxProcess=inputData.num_cores*0.85;
    pidID *pidIDArray;
    
    if(inputData.data["pathDB"]!=""){
        inputData.data["inputFile"]=inputData.fileList[0];
        pechaImg=GBitmap::create(inputData.data["inputFile"].c_str());
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

            if( is_dir(inputData.fileList[i])){
                i++; continue;
            }
            
            inputData.data["inputFile"]=inputData.fileList[i];
            string next="";
            if(i<inputData.fileList.size()-2){
                next=inputData.fileList[i+1];
                next=str_replace(".tif",".html",next);
                next=str_replace(".jpg",".html",next);
                next=str_replace(".png",".html",next);
            }
            
            string  path=inputData.data["inputFile"];    //проверяем есть ли такой распознаный файл
            path=substr(0,(int)path.rfind("."),path);
            string volume=path;
            string fileIndex=fileName(path);
            path+=".html";
            if(is_file(path)){i++; continue;}
            
            
            while(1){
                //проверяем загрузку системы
                str=run("vm_stat");
                vector<string>vm;
                explode("\n", str,vm);
                str=str_replace("Pages free:", "", vm[1]);
                str=str_replace(" ", "", str);
                str=str_replace(".", "", str);
                freeMemory=atoi(str.c_str());
                freeMemory=(freeMemory*4.096)/1000;
                //cout<<"@freeMemory:"<<freeMemory<<endl;
                
                if(freeMemory<10){
                    cout<<"critical memory loading error";
                    //run("killall OCRLib");
                    //exit(0);
                    sleep(100);
                }
                if(freeMemory>1000)break;
                cout<<"@ no free memory for process. freeMemory:"<<freeMemory<<endl;
                sleep(10);
            }

            cout<<"NEW file#1 "<<inputData.fileList[i]<<endl;
            ostringstream out;
            out<<inputData.data["rootApp"]<<" \"fileList="<<inputData.data["inputFile"]<<
            "&ocrData="<<inputData.data["ocrData"]<<"&ocrLn="<<inputData.data["ocrLn"]<<"&pathDB="<<pathDB<<
            "&ID="<<ID<<"&nextPage"<<next<<"&scale="<<inputData.data["scale"]<<"\"";
            string cmd=out.str();
            //cout<<cmd; exit(0);
            system(cmd.c_str());
            //sleep(1); //continue;
            
            
            i++;if(i==inputData.fileList.size())break;
            countProcess++;
            
            for(int index=0;index<maxProcess;index++)cout<<pidIDArray[index].status<<" ";
            cout<<endl;
            
            ID=100;
            //int status;
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
        i=0;
        while(i<inputData.fileList.size()){
            cout<<inputData.fileList[i]<<endl;
                if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
                    i++; continue;
                }
            inputData.data["inputFile"]=inputData.fileList[i];
            pechaImg=GBitmap::create(inputData.data["inputFile"].c_str());
            startOCR(pechaImg);
            i++;
        }
    }
}


void GMainEditor::startOCRBatch(){
    
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
    //int pid;
     pidID *pidIDArray;
     
    
    if(inputData.fileList.size()>2){
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
            path+=".html";
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
        pechaImg=GBitmap::create(inputData.data["inputFile"].c_str());
        startOCR(pechaImg);
    }
    
    
#else
	while(i<inputData.fileList.size()){
		if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
			readDirectoryToArray(inputData.fileList, inputData.fileList[i],"img");
			i++; continue;
		}	
		GBitmap* pechaImg;
		inputData.data["inputFile"]=inputData.fileList[i];

        if(!is_file(inputData.data["inputFile"]))continue;
        pechaImg=GBitmap::create(inputData.data["inputFile"].c_str()); 
        if(!pechaImg){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
        
        str=inputData.data["inputFile"];
		str=substr(0,str.rfind("."),str);
		str+=".html";   //cout_<<str<<endl;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr(inputData.data["siteRoot"].size(),inputData.data["inputFile"]);
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
    pechaImg=GBitmap::create(inputData.data["inputFile"].c_str());
    int pidID=fork();
    
	if (pidID < 0){
		error((char*)"ERROR on fork");
        return 0;
    }    
	if (pidID == 0)  {
		str=substr(0,(int)str.rfind("."),str);
		str+=".html";   cout_<<str<<endl;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr((int)inputData.data["siteRoot"].size(),inputData.data["inputFile"]);
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
		str+=".html";   cout_<<str<<endl;
		inputData.data["inputFileName"]=inputData.data["siteName"];
		inputData.data["inputFileName"]+=substr((int)inputData.data["siteRoot"].size(),inputData.data["inputFile"]);
        cout_<<"NEW START "<<inputData.data["inputFile"]<<endl;
		pechaDataLoaded=0;
		startOCR(pechaImgID_);
		//if(stat)writeLetterStat();
		exit(0);
	}
}


///диспетчер выполнения процесса создания  OCR  шрифта. Распознаем текст как контура, заносим в базу и распознаем.
void GMainEditor::buildOCRFont(){
#ifdef DEPRICATED
    // Автоматическое масштабирование размеров картинки
    //ShowMessage("1start");
	cout_<<"Start#1 "<<inputData.data["inputFile"]<<endl;
	if(aliKali==NULL){aliKali=GFont::create();}
	int border;
	string str;
	int draw=1;
	int print=1;
	
	
	for(int index=0;index<inputData.fileList.size();index++){		
        
        inputData.data["inputFile"]=inputData.fileList[index];
        if(!is_file(inputData.data["inputFile"]))continue;
        pechaImg=GBitmap::create(inputData.data["inputFile"].c_str());
        if(!pechaImg){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
        
        vector<stringOCR>strArray;  
        
        if(inputData.data["ocrData"]=="drawLetter"){
            draw=1;
            //inputData.c_out<<"<html><body><div style=\"position:absolute; left:0px; top:0px;\">";
            //inputData.c_out<<"<img src=\"__1draw.gif\"><br><div style=\"position:relative; left:20px;\">"<<endl;
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
        
        //cout_<<"strArray.size()="<<strArray.size()<<endl;
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
        //int maxID=0;
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
    
#endif
    
}



void GMainEditor::drawVector( vector<OCRPointFloat>&strPoint, vector<OCRFocalLineBezier>&lineVec){
    
    string path;
    int pageWide=0;
    uint drawW=0;
    uint imgW=pechaImg->columns();
    uint imgH=pechaImg->rows();
    if(imgW>imgH)pageWide=1;
    string fontName=aliKali->fontName;
    if(fontName=="")fontName="Yagpo Tibetan Uni";
    
    //cout_<<"@@@draw pageWide="<<pageWide<<" pechaImg->columns()="<<pechaImg->columns()<<" pechaImg->rows()="<<pechaImg->rows()<<endl;
    //cout_<<"inputData.data[\"inputFile\"]="<<inputData.data["inputFile"];
    
    ostringstream c_out1;
    
    if(pageWide){
        drawW=imgW;
        path=inputData.data["root"]+"/edit/headerPageLetter.xml";
    }else{
        drawW=950;
        path=inputData.data["root"]+"/edit/headerPage.xml";
    }
    
    string headerPageHTML; readText(headerPageHTML,path);
    c_out1<<headerPageHTML<<endl;
    c_out1<<"var pictW = "<<pechaImg[0].columns()<<";"<<endl;
    
    /*
     for ( strInd=strArray.size()-1; strInd >=0; strInd-- ){
     for ( m=0; m < strArray[strInd].wordArray.size(); m++ ){
     wordOCR *wP=&strArray[strInd].wordArray[m];
     if(wP->id<0)continue;
     x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
     c_out1<<"   frame["<<wP->id<<"]=new Array(4); ";
     c_out1<<"frame["<<wP->id<<"].x="<<x0<<"; ";
     c_out1<<"frame["<<wP->id<<"].y="<<y0<<"; ";
     c_out1<<"frame["<<wP->id<<"].w="<<x1-x0<<"; ";
     c_out1<<"frame["<<wP->id<<"].h="<<y1-y0<<";"<<endl;
     }
     }
     */
    c_out1<<"var pageLink=\""<<fileName(inputData.data["inputFile"])<<"\";\n";
    
    c_out1<<"</script>";
    c_out1<<"</head><body topmargin=\"0px\" leftmargin=\"0px\">"<<endl;
    c_out1<<"<img id=\"pechaImg\" src=\""<<fileName(inputData.data["inputFile"])<<"\" width=\""<<drawW<<"px\">"<<endl;
    
    c_out1<<"<canvas id=\"page_canvas\" width=\""<<drawW<<"px\" height=\""<<imgH<<"px\" style=\"position: absolute;top:0px;left:0px;\"></canvas>";
    c_out1<<"<script>\n";
    c_out1<<"var c = document.getElementById(\"page_canvas\");\n";
    c_out1<<"var ctx = c.getContext(\"2d\"); ctx.beginPath();\n";
    c_out1<<"ctx.strokeStyle=\"gray\";\n";
    c_out1<<"ctx.lineWidth=1;\n";

    //context.bezierCurveTo(cp1x,cp1y,cp2x,cp2y,x,y);
    //cp1x	The x-coordinate of the first Bézier control point
    //cp1y	The y-coordinate of the first Bézier control point
    //cp2x	The x-coordinate of the second Bézier control point
    //cp2y	The y-coordinate of the second Bézier control point
    //x	The x-coordinate of the ending point
    //y	The y-coordinate of the ending point
    
    cout<<"lineVec.size():"<<lineVec.size()<<endl;
    
    for(uint i=0;i<lineVec.size();i++){
        //cout<<"s.x:"<<lineVec[i].start.x<<" s.y:"<<lineVec[i].start.y<<" e.x:"<<lineVec[i].end.x<<" e.y:"<<lineVec[i].end.y<<" l:"<<lineVec[i].len<<endl;
        c_out1<<"ctx.moveTo("<<lineVec[i].start.x<<", "<<lineVec[i].start.y<<");\n";
        if(lineVec[i].len>5){
            c_out1<<"ctx.bezierCurveTo("<<lineVec[i].p1.x<<", "<<lineVec[i].p1.y<<", "
            <<lineVec[i].p2.x<<", "<<lineVec[i].p2.y<<", "<<lineVec[i].end.x<<", "<<lineVec[i].end.y<<");\n";
        }else{
             c_out1<<"ctx.lineTo("<<lineVec[i].end.x<<", "<<lineVec[i].end.y<<");\n";
        }
        c_out1<<"ctx.stroke();\n";
    }
    c_out1<<"</script>\n";
    
    for(uint i=0;i<strPoint.size();i++){
        //cout<<"y:"<<strPoint[i].y<<endl;
        c_out1<<"<div id=\"p"<<i<<"\" style=\"position: absolute;width:2px;height:2px;border:1px solid green;";
        if(strPoint[i].type==L_POINT){
            c_out1<<"background:green;";
        }else if(strPoint[i].type==T_POINT){
            c_out1<<"background:red;";
        }else if(strPoint[i].type==X_POINT){
            c_out1<<"background:blue;";
        }else if(strPoint[i].type==P_POINT){
            c_out1<<"background:yellow;";
        }else if(strPoint[i].type==O_POINT){
            c_out1<<"background:white;";
        }
        c_out1<<"left:"<<(uint)strPoint[i].x-1<<"px; top:"<<(uint)strPoint[i].y-1<<"px;\"></div>";
    }
    
    
    
    c_out1<<"<div id=\"page_form\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;top:900px;\">";
    c_out1<<"<br><a href=\"http://www.dharmabook.ru\" target=\"_blank\">tibetan OCR www.dharmabook.ru</a><br>";
    c_out1<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"/cgi/yagpoOCR.cgi\" name=\"main\">\n";
    c_out1<<"<input  name=\"inputFile\"  id=\"inputFile\" type=\"hidden\" value=\""; c_out1<<inputData.data["inputFile"];c_out1<<"\"/>\n";
    c_out1<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/>\n";
    c_out1<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; c_out1<<inputData.data["inputFolder"];c_out1<<"\"/>\n";
    //	c_out1<<"<button name=\"submit\" value=\"submit\" type=\"submit\">Set letter in base</button>";
    //	c_out1<<"<input  name=\"newLetter\"  type=\"checkbox\" value=\"true\" />Set as new letter<br>\n";
    c_out1<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"70\" rows=\"3\"></textarea>\n";
    c_out1<<"</form></div>";
    
    string strOut=c_out1.str();
    string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";
    writeText(strOut,str);

}










