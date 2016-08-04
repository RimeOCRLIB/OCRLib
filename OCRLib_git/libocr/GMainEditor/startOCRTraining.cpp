#include "GMainEditor.h"

///диспетчер выполнения процесса распознавания страницы.
void GMainEditor::startOCRTraining(GBitmap *pechaImg_){
    // Автоматическое масштабирование размеров картинки
    //ShowMessage("1start");
    cout<<"Start#1 "<<inputData.data["inputFile"]<<END; 
    if(!pechaImg_){cout<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
    pechaImg=pechaImg_;
    correctionTable.resize(0);
	
    vector<stringOCR>strArray;  
    int border;
    string str;
    int print=1;
    int index=atoi(inputData.data["lineIndex"].c_str());
    int indexStart=index;
    int inputCorrectionCount=correctionTable.size();
    
    OCRTestImage:;
    
	DT("@4_1");
    //vectorBase[0].allVectorCount=0;
	border=0; clock_t  tm_start1, tm_end1; double time1;  tm_start1=clock();
    setBit=GBitset::createResize(pechaImg_,1,1,1);
    DT("@4_2");
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
    DT("@4_3");
    border=setBit->border_size();
    DT("@4_4 strArray.size()="<<strArray.size());
    
        //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
        if(index<0)index=strArray.size()-1;
        if(index>=strArray.size()){return;}
        index=strArray.size()-index-1;
        int yLimit0=(strArray[index].y1-strArray[index].y0)*3;	 
        int LimY0=yLimit0*0.8;  
        
        GBitmap *line=GBitmap::createRegion(pechaImg_, 
                                            0, 
                                            strArray[index].y0-LimY0, 
                                            pechaImg_->columns(), 
                                            yLimit0*2.3
                                            );

        pechaImg=line;
        inputData.data["inputFile"]=inputData.data["root"]+"edit/OCR/1.tif";
        string mode="tif";
        WriteImageData(pechaImg,inputData.data["inputFile"],0,mode);
        
        setBit->destroy();
        
        
        setBit=GBitset::createResize(pechaImg,1,1,1);
        DT("@4_2");
        setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        DT("@4_3");
        border=setBit->border_size();
        DT("@4_4"<<pechaImg->rows()/2);
        
        //находим тестовую строку для распознавания
        int bestIndex=0; int minDistance=1000;
        int t;
        for(int a=0;a<strArray.size();a++){
            strArray[a].selectFlag=0;
            t=abs((int)(pechaImg->rows()/2-strArray[a].y0));
            if(t<minDistance){minDistance=t;bestIndex=a;}
        }
        strArray[bestIndex].selectFlag=1;
        cout<<"bestIndex="<<bestIndex<<endl;
        
        

    GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
    //if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
    TIME_START
    
    setOCR->setData(aliKali,
                    strArray,
                    correctionTable,
                    logicProcessor,
                    iLeft,
                    iTop,
                    border,
                    ALL_LETTER);
	
    TIME_PRINT
    mainString=setOCR->mainString;
    
    if(inputData.data["ocrData"]=="nextOCRLetter"){
      cout<<" correctionTable.size()="<<correctionTable.size()<<endl;
        
        if(inputCorrectionCount==correctionTable.size()){  
            indexStart++; index=indexStart;
            inputData.startIndex=indexStart;
            cout<<"string is correct. Take new string for OCR"<<endl;
            pechaImg->destroy();
            goto OCRTestImage;
        }
    }
	pechaImg->destroy();
    
    //cout<<"time="<<time<<" mainString"<<mainString<<END;
    
    DT("@6");
    tm_end1=clock(); tm_end1-=tm_start1; time1=tm_end1/CLOCKS_PER_SEC;

    if(setBit!=NULL){
        setBit->destroy();      
    }
    if(setOCR!=NULL){
        
        DT("start release setOCR  inputData.data[ocrData]="<< inputData.data["ocrData"]<<endl);
        setOCR->destroy();
        
        DT("draw0");
        drawStrArrayDebug(strArray,border);
        DT("draw1");
        //if(inputData.data["ocrData"]=="ocrBatch")drawLettersInCorrectionPage(LETTER_ADD);	
        DT("draw2");
    }
    //strArray.resize(0);
    
    DT("done"<<endl);
    pechaDataLoaded=0;
    cout_<<"done "<<inputData.data["inputFile"]<<END;
    
    //}
}//________________________________________________________________________________________________________________


///формирование набора строк содержащих возможные ошибки распознавания
void GMainEditor::writeOCRTrainingData(){

    int mainCount=0;
    int indexLine;
    int print=0;
    
    for(int index=0;index<inputData.fileList.size();index++){
        int print=1;
        DR("load1 "<<inputData.fileList[index]<<endl;)
        string imgFile="";
       /*
        if(inputData.fileList[index].find(".xml")==-1)continue;
        xml_node page,line;
        xml_document doc;
        
                
        if(!doc.load_file(inputData.fileList[index].c_str())){
            cout<<inputData.fileList[index]<<" not loaded"<<endl;continue;
        }
        
        page = doc.child("text:text");
        vector<int>lineArray;
        for (line = page.child("line"); line; line = line.next_sibling("line")){
            indexLine=atoi(line.attribute("i").value());
            int lineOCR=atoi(line.attribute("ocr").value());
            
            if(lineOCR==1)lineArray.push_back(indexLine);
        } 
        if(lineArray.size()==0)continue;
        */
        string sourcePath=inputData.fileList[index];
        sourcePath=substr(0,(int)sourcePath.size()-3,sourcePath);
        sourcePath+="tif";
        DR("Start#1 "<<sourcePath<<endl;)
        if(!is_file(sourcePath))sourcePath+="f";
        if(!is_file(sourcePath)){
            sourcePath=substr(0,(int)sourcePath.size()-4,sourcePath);
            sourcePath+="jpg";
        }
        if(!is_file(sourcePath)){
            sourcePath=substr(0,(int)sourcePath.size()-3,sourcePath);
            sourcePath+="jpeg";
        }
        
        GBitmap* pechaImg; pechaImg=LoadImageData(sourcePath,0);
        imageEditor->adaptiveFiltration(pechaImg,1,0);
        
        if(!pechaImg){cout<<"no open file0"<<sourcePath<<endl; continue;}
        
        vector<stringOCR>strArray;  
        int border=0;
        setBit=GBitset::createResize(pechaImg,1,1,1);
        DR("@4_2")
        setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        DR("@4_3")
        border=setBit->border_size();
        DR("@4_4 strArray.size()="<<strArray.size())
        
        for(int a=0;a<strArray.size();a++){
            
            //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
            indexLine=a;//lineArray[a];
            int strH=strArray[indexLine].y1-strArray[indexLine].y0;
            int yLimit0=strH*4;
            if(indexLine>0&&yLimit0+strArray[indexLine].y1>strArray[indexLine-1].y0)yLimit0=strArray[indexLine-1].y0-strArray[indexLine].y1;
            int LimY0=strH*2;            
            
            GBitmap *lineImg=GBitmap::createRegion(pechaImg, 
                                                0, 
                                                strArray[indexLine].y0-LimY0, 
                                                pechaImg->columns(), 
                                                LimY0+strH+yLimit0
                                                );
            
            ostringstream out;
            out<<inputData.data["root"]<<"edit/OCR/_OCR/"<<imgFile<<"_";
            out.width(4);
            out.fill('0');
            out<<mainCount<<"_"<<indexLine<<".tif";
            string mode="tif";
            string path=out.str();
            WriteImageData(lineImg,path,0,mode);
            lineImg->destroy();
            mainCount++;if(mainCount>MAX_OCR_LINE)return;
        }    
            setBit->destroy();
            pechaImg->destroy();
    
    }
    



}

