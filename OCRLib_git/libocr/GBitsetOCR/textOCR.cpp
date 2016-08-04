#include "GBitsetOCR.h"
#include "debug.h"

#define REPORT

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
	
    float scalePage=(float)atoi(inputData.data["scale"].c_str())/100;  //1 не выполняется масштабирование
    if(scalePage<0)scalePage=0; if(scalePage>5)scalePage=5;  //cout<<"scale="<<scalePage<<" / "<<inputData.data["scale"]<<endl; exit(0);
    GBitmap *lineImg;
    baseCount=0;  letterW=0; letterH=0;

int OCRMode=1; //- Tibetan print
//int OCRMode=2; //- Tibetan woodblock
    
int rewriteOCRCash;  
rewriteOCRCash=((GStr<int>*)inputData.pref)->get(5);
((GStr<int>*)inputData.pref)->put(5,0);

    rewriteOCRCash=1;
    
    
    int indexString=-2;  //14  строка для распознавания. //-2 full page //-1 debug mode
    
    inputData.lineSize.resize(5);
    
    if(inputData.data["ocrData"]=="lineOCR"){
        indexString=atoi(inputData.data["index"].c_str());
        inputData.data["inputFile"]=inputData.data["root"]+"/edit/OCR/_OCR/_line.jpg";
    }
    
  
    //for (int index=1;index>=1; --index){  //for every string
	for (int index=(int)strArray[0].size()-1;index>=0; --index){  //for every string
        cout<<". ";
        //cout<<"strArray[0]["<<index<<"].y0="<<strArray[0][index].y0<<" y1="<<strArray[0][index].y1<<endl;
        
        //if(index!=25)continue;  //отладочный режим
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
                        int scaleFlag=0;
startScale:;
        
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
                        int dL=5;
                        if(index>0&&strArray[0][index].y1+yLimit1>strArray[0][index-1].y0-dL){yLimit1=strArray[0][index-1].y0-strArray[0][index].y1-dL;}

                        int LimY0=yLimit0;
                        cout<<" textLineSize="<<textLineSize<<" scale="<<(float)32/textLineSize; //exit(0);

                        float scale=(float)32/textLineSize;
                        if(w*scale>15000)scale=1;
                        //if(scale>0.8)scale=1;
                        //if(scale<0.8)scale=0.68;
                        //if(scale<=1.1)scale=1;
                        //if(scale>1.1&&scale<1.3)scale=1.23;
                        //if(scale>=1.3)scale=1.53;
                        float scale1=1; //0.67;
                        float scale2=1;
                        if(scalePage!=1&&scalePage!=0){
                            scale=scalePage;
                        }else{
                            if(scale1!=1||scale2!=1){
                                if(scaleFlag==0){
                                    scale=scale1;
                                    scaleFlag++;
                                }else{
                                    scale=scale2;
                                    scaleFlag++;
                                }
                            }else{
                                scale=1;//1.28;//0.84;//1.68;////Tashi1.28; //0.98;//0.89; 1.45;   0.98; //Kangyur
                            }
                        }
                        textLineSize=32;
                        LimY0=LimY0*scale;
                        cout<<" textLineSize1="<<textLineSize<<" scale="<<scale<<" scaleFlag="<<scaleFlag<<endl;
        
                        strArray[0][index].LimY0=LimY0;
                        strArray[0][index].LimY1=LimY0+textLineSize;  //передаем координаты строки для шрифтовой грамматики

                        //DR(" textLineSize1="<<textLineSize)
                        //DR("w="<<w<<"h="<<h<<" scale="<<scale<<" y0="<< strArray[0][index].y0<<" y1="<< strArray[0][index].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<" LimY0="<<LimY0<<END);
                        //DR("@@Y0="<<strArray[0][index].y0-yLimit0<<endl)
                        //DR("@@H="<<strArray[0][index].y1-strArray[0][index].y0+yLimit0+yLimit1<<endl)
          //cout<<"scale="<<scale<<endl;
          //if(rewriteOCRCash){
                        //if(strArray[0][index].y0-LimY0-LimY0*2.5<5)continue;
                        //cout<<" y0="<<strArray[0][index].y0-LimY0<<" y1="<<LimY0*2.5<<endl; //exit(0);
                        int w32=inputBitmap->columns();
                        w32=(w32/32)*32;  //выравнивание по 32 нужно для графических функций
                        lineImg=GBitmap::createRegion (inputBitmap,
                                                    0, 
                                                    strArray[0][index].y0-yLimit0,
                                                    w32, 
                                                    yLimit0+textLineSize+yLimit1
                                                    );
        
                        if(inputData.data["ocrData"]=="lineOCR"){
                                string path=inputData.data["root"]+"/edit/OCR/_OCR/_line.tif";
                                ((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,path,0);
                        }

                       ((GImageEditor*)inputData.imageEditor)->adaptiveFiltration1(lineImg,scale,1);
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
                       //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
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
        
        
#ifdef REPORT
        //if(inputData.start>0){
        //    return;
        //}
        //print =0;if(in==16672)print=1;
#endif
       

#ifdef DEBUGLVL_DRAW         
             ((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/1Draw.jpg",0);
             ((GImageEditor*)inputData.imageEditor)->WriteImageData(setMatrix->drawPoint,"/_Image2OCR/1Draw_.jpg",0);
#endif
              if(indexString>-2||inputData.data["ocrData"]=="testOCRLine"||
                 inputData.data["ocrData"]=="OCRTraining"){  
                 ((GStr<int>*)inputData.pref)->put(0,LimY0*scale);
                  inputData.lineSize[0]=LimY0*scale;
                 ((GStr<int>*)inputData.pref)->put(1,(LimY0+textLineSize)*scale);
                  inputData.lineSize[1]=(LimY0+textLineSize)*scale;
                 ((GStr<int>*)inputData.pref)->put(2,lineImg->columns());
                  inputData.lineSize[2]=lineImg->columns();
                 ((GStr<int>*)inputData.pref)->put(3,lineImg->rows());
                  inputData.lineSize[2]=lineImg->rows();
             }    
            ///if(indexString>-1){
            //    logicProcessor->saveMatch(matchLine,"/2_1.match");
            //}
             
        //}else{     //rewriteOCRCach
              
            //if(indexString>-1){
            //    logicProcessor->readMatch(matchLine,"/2_1.match");
            //}
        
        //}

        
             string strW;
             vector<ushort>letterX;
             vector<OCRMatch>line;
             vector<OCRMatch>dLine;
             vector<OCRMatch>resultLine;
             map<vector<short>,int>searchResult;

             if(print){ cout<<"OCR "; TIME_PRINT_ }
             //logicProcessor->drawGrapeLine(matchLine); exit(0);
             inputData.log<<" start letter association"<<endl;
             logicProcessor->letterAssociation(strArray,
                                        matchLine,
                                        line,
                                        lineImg32,
                                        mainString,
                                        inputBitmap->columns()+256,
                                        index,
                                        2);
        
             string OCRresult;
             //cout<<"index="<<index<<endl;

             if(line.size()){
                    if(print){ cout<<"STACK "; TIME_PRINT_ }
                 if(OCRMode==1){
                     if(indexString==-2){
                             logicProcessor->buildSearchString(line,dLine,letterX,strW);
                             if(print){ cout<<"STRING "; TIME_PRINT_ }
                             //inputData.log<<" 1"<<endl;inputData.log.flush();
                             logicProcessor->textCorpusGMap->getOCRStackKey(strW,letterX,searchResult, ANY_MATCH);
                                if(print){ cout<<"SEARCH "; TIME_PRINT_ }
                             //inputData.log<<" 2"<<endl;inputData.log.flush();
                             inputData.log<<" start render"<<endl;
                             for (int i=0;i<line.size();i++){
                                 if(!line[i].correlation)continue;                        //cout<<line[i].name;
                                 line[i].name=Unicode_to_UTF(line[i].wName);              //cout<<" -- "<<line[i].name;
                                 line[i].name=logicProcessor->YagpoToUni(line[i].name);   //cout<<" -- "<<line[i].name<<endl;
                                 resultLine.push_back(line[i]);
                             }
                             //logicProcessor->drawGrapeLine(line); exit(0);
                             //inputData.log<<" 3"<<endl;inputData.log.flush();
                             logicProcessor->renderDictSearch(searchResult,dLine,resultLine,pageText);   //mainString+="<br>"+lineString+"original<br>";
                             if(print){ cout<<"RENDER "; TIME_PRINT_ }
                             strArray[0][index].line=dLine;   //сохраняем для вывода в HTML
                             //inputData.log<<" 4"<<endl;inputData.log.flush();
                     }else{
                         dLine=line;
                     }
                 }else{
                     
                         logicProcessor->compactResultOCR(line,OCRresult);
                         strArray[0][index].line=line;
                 
                 }
                 if(pageText.size())pageText[pageText.size()-1].delimeter+="\n<br>";
                 OCRresult="";
                 ostringstream out;
                 out<<"<line i=\""<<index<<"\" ocr=\"1\">"<<OCRresult<<"</line>\n";
                 xmlString+=out.str();
                  
             }
             if(indexString>-2){
                 logicProcessor->drawGrapeLine(dLine); 

                 //logicProcessor->grammarCorrector(mainString,xmlString,1);
                 logicProcessor->textCorrector(pageText, mainString);
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
    
    
    
    //проверка по словарю статистической устойчивости распознанного текста
    mainString="";
    for(int a=0;a<pageText.size();a++){
        if(!pageText[a].correlation)continue;
        mainString+=pageText[a].name;
        mainString+=pageText[a].delimeter;
    }
    //cout<<mainString;
    mainString=str_replace("•"," ",mainString);
    logicProcessor->textNormalisation(mainString);
        
    pageText.resize(0);
    OCRMatch match;
    match.correlation=100;
    pageText.push_back(match);
    
    //проверка по словарю статистической устойчивости распознанного текста
    
    wstring strW=UTF_to_Unicode(mainString);
    
    for(int n=0;n<strW.size();n++){
        string s=Unicode_to_UTF(strW[n]);
        if(logicProcessor->isDelimeter(s)||s==" "){
            pageText[pageText.size()-1].delimeter+=s;
        }else{
            OCRMatch match;
            match.correlation=100;
            match.name=s;
            pageText.push_back(match);
        }
    }
    mainString="";
    logicProcessor->textCorrector(pageText, mainString);
    
    
    xmlString=mainString;
    inputData.log<<" done page grammar"<<endl;inputData.log.flush();
#ifdef MAIN_MODE
        DR(" start grammar corrector")
        //cout<<mainString;
        //logicProcessor->grammarCorrector(strArray,correctionTable,mainString,xmlString,LOCAL_MODE);
        //logicProcessor->grammarCorrector(mainString,xmlString,1);
        //xmlString=mainString;
  
#endif
        DR_("done Grammar"<<endl);
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



