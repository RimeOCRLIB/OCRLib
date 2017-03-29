#include "GBitsetOCR.h"
#include "debug.h"

#define REPORT

static bool sortFocalLineL(const OCRFocalLine& d1, const OCRFocalLine& d2){
    return d1.len > d2.len;
}

void GBitsetOCR::textOCR(vector<stringOCR>&correctionTable,GLogicProcessor *logicProcessor, int mode){
	if(strArray[0].size()==0)return;
    if(!aliKali)return;    
    if(aliKali->letterCount()<1)return;

    int print=0;
    
    int baseCount;
	unsigned int letterW,letterH;
	int w,h,yLimit0,yLimit1,id_index;
	string letter;
	int indexW;
	mainString="";
	indexW=0; id_index=0;
    vector<OCRMatch>pageText;
	
    //float scalePage=(float)atoi(inputData.data["scale"].c_str())/100;  //1 не выполняется масштабирование
    //if(scalePage<0)scalePage=0; if(scalePage>5)scalePage=5;  //cout<<"scale="<<scalePage<<" / "<<inputData.data["scale"]<<endl; exit(0);
    GBitmap *lineImg;
    baseCount=0;  letterW=0; letterH=0;


//int OCRMode=1; //1- Tibetan  оптимальная высота строки 32
int OCRMode=1; //2- Sanskrit   оптимальная высота строки 39
inputData.OCRMode=OCRMode;
    
int rewriteOCRCash;  
rewriteOCRCash=0;((GStr<int>*)inputData.pref)->get(5);
((GStr<int>*)inputData.pref)->put(5,0);
    
    GGraph vectorGraph=GGraph(inputBitmap);
    vector<OCRPoint>focalPoint;
    vector<OCRFocalLine>focalLine;

    //for(int n=0;n<100;n++){
        //выделение фокальных линий страницы и распознавание векторов для предварительного распознавания букв по диаграмме Воронова
        //и распознавания макета страницы и таблиц.
        //cout<<n<<endl;
        vectorGraph.eraseBorder(10,10);  //необходимо для работы focalLine
        vectorGraph.focalLine(15);  // параметр функции - это максимально возможное разумное количество удаленных слоев,
        // если функция увидит что удаление слоев не меняет получаемую картинку (фокальная линия получена),
        // то она сама раньше закончит работу
        vectorGraph.focalPointXTL(focalPoint);
        vectorGraph.focalLineRecognition(focalPoint, focalLine);
        //focalPoint.resize(0);
        //focalLine.resize(0);
    //}
    sort(focalLine.begin(),focalLine.end(),sortFocalLineL);
    
    //exit(0);
    
#ifdef OCRLIB_STAT
    int indexString=0;  //14  строка для распознавания. //-2 full page //-1 debug mode
#else
    int indexString=-2;  //14  строка для распознавания. //-2 full page //-1 debug mode
#endif
    
    inputData.lineSize.resize(5);
    
    if(inputData.data["ocrData"]=="lineOCR"){
        indexString=atoi(inputData.data["index"].c_str());
        inputData.data["inputFile"]=inputData.data["root"]+"/edit/OCR/_OCR/_line.jpg";
    }
    
    //for (int index=1;index>=1; --index){  //for every string
	for (int index=(int)strArray[0].size()-1;index>=0; --index){  //for every string
        cout<<". "<<index;
        //cout<<"strArray[0]["<<index<<"].y0="<<strArray[0][index].y0<<" y1="<<strArray[0][index].y1<<endl;
        
        //if(index!=17&&index!=16&&index!=15)continue;  //отладочный режим
        
        //if(index!=16)continue;  //отладочный режим
        
        if(indexString>-1&&index!=indexString)continue;  //отладочный режим

        vector<wordOCR>lineText;

        
        /*if(print){
            int oldIndex=((GStr<int>*)inputData.pref)->get(4);
            if(oldIndex!=indexString){
                   ((GStr<int>*)inputData.pref)->put(4,indexString);
                   inputData.lineSize[4]=indexString;
                   rewriteOCRCash=1;
            }    
        }else{ rewriteOCRCash=1; }
        */
        rewriteOCRCash=1;
                        if((inputData.data["ocrData"]=="testOCRLine"||
                            inputData.data["ocrData"]=="OCRTraining")&&!strArray[0][index].selectFlag){continue;}
//#ifndef STACK_MODE 
        
TIME_START

                        //if(indexString>-2)cout_<<"@@@ line "<<index<<endl;
		    
						//Create GBitsetMatrix for matrix correlation
						w=inputBitmap->columns();
						h=inputBitmap->rows();
		
		                //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
                        int textLineSize=strArray[0][index].y1-strArray[0][index].y0;
                        yLimit0=textLineSize*1.6;
                        yLimit1=textLineSize*2.4; //2.4
                        //normalisation
        
                        //DR("w="<<w<<"h="<<h<<
                        //   "y0="<< strArray[0][index].y0<<" y1="<< strArray[0][index].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<" y0_1="<<strArray[0][index-1].y0<<END);

    
                        if(strArray[0][index].y0-yLimit0<0)yLimit0=strArray[0][index].y0;
                        if(strArray[0][index].y1+yLimit1>h)yLimit1=h-strArray[0][index].y1;
                        int dL=1;
                        if(index>0&&strArray[0][index].y1+yLimit1>strArray[0][index-1].y0-dL){yLimit1=strArray[0][index-1].y0-strArray[0][index].y1-dL;}

                        int LimY0=yLimit0;
                        //cout<<" textLineSize="<<textLineSize<<" scale="<<(float)32/textLineSize; //exit(0);

                        textLineSize=32;
        
                        strArray[0][index].LimY0=LimY0;
                        strArray[0][index].LimY1=LimY0+textLineSize;  //передаем координаты строки для шрифтовой грамматики

                        int w32=inputBitmap->columns();
                        w32=(w32/32)*32;  //выравнивание по 32 нужно для графических функций
                        lineImg=GBitmap::createRegion (inputBitmap,
                                                    0, 
                                                    strArray[0][index].y0-yLimit0,
                                                    w32, 
                                                    yLimit0+textLineSize+yLimit1
                                                    );
                        if(!lineImg->rows()){cout<<"NO ROWS"<<endl; continue;}
        
                        if(inputData.data["ocrData"]=="lineOCR"){
                                string path=inputData.data["root"]+"/edit/OCR/_OCR/_line.tif";
                                ((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,path,0);
                        }
                       //определяем поворот и при необходимости выравниваем строку
                       float p[]={1};
                       ((GImageEditor*)inputData.imageEditor)->adaptiveFiltration1(lineImg,p,1);
                       if(indexString>-2||inputData.data["ocrData"]=="testOCRLine"||inputData.data["ocrData"]=="OCRTraining"){
                           string path=inputData.data["inputFile"];  path=str_replace(".tif", ".jpg", path);
                           ((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,path,0); //exit(0);
                       }
        
                       setMatrix=GBitsetMatrix::createRegion(lineImg,
                                                     0,
                                                     0,
                                                     lineImg->columns(),
                                                     lineImg->rows(),
                                                     1,1,
                                                     IMGNOFLIP
                                                     );
                       vector<OCRMatch>matchLine;
                       GBitmap *lineImg32=&setMatrix->drawLine;

                       //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,"/_Image2OCR/_1Draw_.jpg",0); //exit(0);
                       //((GImageEditor*)inputData.imageEditor)->WriteImageData((GBitmap*)&setMatrix->drawPoint,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
                       //((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/_1DrawRGB.jpg",0);
                       //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
                        setMatrix->letterCorrelation(matchLine,
													 aliKali,
                                                     index,
													 LimY0, 
                                                     LimY0+textLineSize,
													 0,   //no scale mode
                                                     OCRMode,   //OCRMode, 
													 0);  //0 no print
        
        
       

#ifdef DEBUGLVL_DRAW
             ((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/1Draw.jpg",0);
             ((GImageEditor*)inputData.imageEditor)->WriteImageData(setMatrix->drawPoint,"/_Image2OCR/1Draw_.jpg",0);
#endif
             if(indexString>-2||inputData.data["ocrData"]=="testOCRLine"||
                 inputData.data["ocrData"]=="OCRTraining"){
                 ((GStr<int>*)inputData.pref)->put(0,LimY0);
                  inputData.lineSize[0]=LimY0;
                 ((GStr<int>*)inputData.pref)->put(1,(LimY0+textLineSize));
                  inputData.lineSize[1]=(LimY0+textLineSize);
                 ((GStr<int>*)inputData.pref)->put(2,lineImg->columns());
                  inputData.lineSize[2]=lineImg->columns();
                 ((GStr<int>*)inputData.pref)->put(3,lineImg->rows());
                  inputData.lineSize[2]=lineImg->rows();
             }

        
             string strW;
             vector<uint>letterX;
             vector<OCRMatch>line;
             vector<OCRMatch>dLine;
             vector<OCRMatch>resultLine;
             map<vector<int>,ulong>searchResult;
        
             GBitmap *letterAImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);
             GBitmap *letterBImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);

             if(print){ cout<<"OCR "; TIME_PRINT_ }
             //logicProcessor->drawGrapeLine(matchLine); //exit(0);
             inputData.log<<" start letter association"<<endl;
        
             logicProcessor->pageSegmentation(matchLine,focalLine,w,strArray[0][index].y0-yLimit0);

             logicProcessor->letterAssociation(*strArray,
                                        matchLine,
                                        line,
                                        lineImg32,
                                        letterAImg,
                                        letterBImg,
                                        mainString,
                                        inputBitmap->columns()+256,
                                        index);
        
             string OCRresult;
             //cout<<"index="<<index<<endl;
             //logicProcessor->drawGrapeLine(matchLine); exit(0);
        
             if(line.size()){
                    if(print){ cout<<"STACK "; TIME_PRINT_ }
                 if(OCRMode==1){
                     //if(indexString==-2){
                         for (int i=0;i<matchLine.size();i++){
                             if(!matchLine[i].correlation)continue;                        //cout<<line[i].name;
                             resultLine.push_back(matchLine[i]);
                         }
                     
                         logicProcessor->buildSearchStringNew(resultLine,dLine,letterX,strW,lineImg32,letterAImg,letterBImg);

                     //cout<<strW<<endl;
                         //string s=logicProcessor->YagpoToUni(strW);
                         //writeText( s,"/_Image2OCR/1.txt");
                         //logicProcessor->drawGrapeLine(dLine); exit(0);
                         logicProcessor->textCorpusGMap->getOCRStackKey(strW,letterX,searchResult, ANY_MATCH);
                         //inputData.log<<" 2"<<endl;inputData.log.flush();
                         //inputData.log<<" start render"<<endl;
                     
                     
                         for (int i=0;i<resultLine.size();i++){
                             resultLine[i].name=Unicode_to_UTF(resultLine[i].wName);              //cout<<" -- "<<line[i].name;
                             resultLine[i].name=logicProcessor->YagpoToUni(resultLine[i].name);   //cout<<" -- "<<line[i].name<<endl;
                         }
                     
                         //logicProcessor->drawGrapeLine(resultLine); exit(0);
                         //inputData.log<<" 3"<<endl;inputData.log.flush();
                     
                        logicProcessor->renderDictSearchNew(searchResult,dLine,resultLine,pageText,lineImg32,letterAImg,letterBImg);
                     
                        letterAImg->destroy();
                        letterBImg->destroy();
                     
                           //mainString+="<br>"+lineString+"original<br>";
                         if(print){ cout<<"RENDER "; TIME_PRINT_ }
                         strArray[0][index].line=dLine;   //сохраняем для вывода в HTML
                         //logicProcessor->drawGrapeLine(dLine); //exit(0);
                         //inputData.log<<" 4"<<endl;inputData.log.flush();
                     //}else{
                     //    dLine=line;
                     //}
                 }else{
                     
                     
                         logicProcessor->renderDictSearchSanskrit(searchResult,matchLine,lineImg32,letterAImg,letterBImg);
                         logicProcessor->compactResultOCR(matchLine,OCRresult);
                         //strArray[0][index].line=line;
                         mainString+=OCRresult+"<br/>\n";
                 }
                 if(pageText.size())pageText[pageText.size()-1].delimeter+="\n";
                 //OCRresult="";
                 //ostringstream out;
                 //out<<"<line i=\""<<index<<"\" ocr=\"1\">"<<OCRresult<<"</line>\n";
                 //xmlString+=out.str();
                  
             }
            
             if(indexString>-2){
                 
                 if(inputData.OCRMode==1){
                     logicProcessor->drawGrapeLine(dLine);
                 }
                 if(inputData.OCRMode==2){
                     logicProcessor->drawGrapeLine(matchLine);
                 }
                 logicProcessor->grammarOCR(pageText, mainString);

                 string str=inputData.data["inputFile"];
                 str=substr(0,(int)str.rfind("."),str);
                 str+=".html";
                 readText(xmlString, str);
                 mainString=xmlString+"<br><div style=\"position:relative;top:400px;font-size:32px\">"+mainString+"</div>";
                 writeText(mainString,str);
                 if(print){ cout<<"GRAMAR "; TIME_PRINT_ }
                 if(inputData.data["ocrData"]!="lineOCR"){
                     exit(0);
                 }else return;
        
             }    
             inputData.log<<" done OCR string"<<endl; inputData.log.flush();
             lineImg->destroy();

#ifndef REPORT_DRAW
    #ifndef REPORT
             setMatrix->destroy();
    #else
            if(inputData.start>0){
                inputData.matrix=(void*)setMatrix;
                return;
            }else{
               setMatrix->destroy();
            }
    #endif
#endif
             
    }//для каждой строки
    TIME_START
    if(print){ cout<<"DONE STRINGS ";  }
    logicProcessor->grammarOCR(pageText, mainString);
    inputData.log<<" done page grammar"<<endl;inputData.log.flush();
    if(print){ cout<<"DONE PAGE GRAMAR "; TIME_PRINT_ }

}////////////////////////////////////////////////////////////////////////////////




int GBitsetOCR::readFromCorrectionTable(vector<stringOCR>&correctionTable,vector<wordOCR>&pageText,int index){
	//cout<<"correctionTable.size()="<<correctionTable.size()<<" index="<<index<<endl;

	for(int i=0;i<correctionTable.size();i++){
		    //if(correctionTable[i].file==fileName)cout<<"f="<<correctionTable[i].file<<" l="<<correctionTable[i].lineIndex<<" c="<<correctionTable[i].correlation<<" i="<<index<<" f="<<fileName<<endl;
		    if(correctionTable[i].file==fileName&&correctionTable[i].lineIndex==index&&correctionTable[i].correlation==100){
				cout<<"@@@ CORRECTION match="<<correctionTable[i].name<<endl;
				wordOCR wp;
				wp.name=correctionTable[i].name;
				wp.lineIndex=index;
				wp.newLine="line";
				pageText.push_back(wp);
				return 1;
			}	

	}
	return 0;

}



