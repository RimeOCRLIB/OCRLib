#include "GBitsetOCR.h"
#include "debug.h"

#define REPORT

void GBitsetOCR::pageOCR(GLogicProcessor *logicProcessor, int mode){
/*
	if(strArray[0].size()==0)return;
    if(!aliKali)return;    
    if(aliKali->letterCount()<1)return;

TIME_START
    int print=0;
    
	string letter;
	mainString="";
    vector<OCRMatch>pageText;
    
    
    int indexString=0;  //14  строка для распознавания. //-2 full page //-1 debug mode
    
    inputData.lineSize.resize(5);
    
    if(inputData.data["ocrData"]=="lineOCR"){
        indexString=atoi(inputData.data["index"].c_str());
        inputData.data["inputFile"]=inputData.data["root"]+"/edit/OCR/_OCR/_line.jpg";
    }
 
    vector<wordOCR>lineText;
    vector<OCRPoint>focalPoint;        //массив фокальных точек
    vector<OCRFocalLine>focalLine;     //массив фокальных линий
    
    string pathText;
    //((GVector*)inputData.prefVector)->getStr(20,pathText);
    GBitmap* pechaImg=((GImageEditor*)inputData.imageEditor)->LoadImageData(pathText,0);
    if(pechaImg->DetectGradations())pechaImg->binarisation(85,24);
    
    GGraph pechaGraph=GGraph(pechaImg);
    pechaGraph.focalLine(15);
    pechaGraph.focalPointXTL(focalPoint);
    pechaGraph.focalLineRecognition(focalPoint,focalLine);
    GStr2D<int>*point2D=GStr2D<int>::create();
    GStr2D<int>*line2D=GStr2D<int>::create();
    
    
    for(int lineIndex=0;lineIndex<focalLine.size();lineIndex++){
        cout<<"index="<<lineIndex<<" x="<<" alpha1="<<focalLine[lineIndex].alpha<<endl;
    }
    pechaGraph.setLineMatrix(focalPoint,focalLine,point2D,line2D,12);
    
    
    
    setMatrix=GBitsetMatrix::createRegion(inputBitmap,
                                         0,
                                         0,
                                         inputBitmap->columns(),
                                         inputBitmap->rows(),
                                         1,1,
                                         IMGNOFLIP
                                         );
   vector<OCRMatch>matchLine;
   pechaGraph.setLineCorrelation(focalPoint,focalLine,point2D,line2D,setMatrix,&matchLine);
   point2D->destroy();
   line2D->destroy();
   return;
    
    
   GBitmap *lineImg32=&setMatrix->drawLine;
   //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
   //((GImageEditor*)inputData.imageEditor)->WriteImageData((GBitmap*)&setMatrix->drawPoint,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
   //((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/_1DrawRGB.jpg",0);
   //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
   
#ifdef DEBUGLVL_DRAW         
             ((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/1Draw.jpg",0);
             ((GImageEditor*)inputData.imageEditor)->WriteImageData(setMatrix->drawPoint,"/_Image2OCR/1Draw_.jpg",0);
#endif
             string strW;
             vector<ushort>letterX;
             vector<OCRMatch>line;
             vector<OCRMatch>dLine;
             vector<OCRMatch>resultLine;
             map<vector<int>,ulong>searchResult;

             if(indexString>-2){ cout<<"OCR "; TIME_PRINT_ }
             //logicProcessor->drawGrapeLine(matchLine); exit(0);
             logicProcessor->letterAssociation(*strArray,
                                        matchLine,
                                        line,
                                        lineImg32,
                                        mainString,
                                        inputBitmap->columns()+256,
                                        0,
                                        2);
        
             string OCRresult;
             //cout<<"index="<<index<<endl;


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

    //проверка по словарю статистической устойчивости распознанного текста
    logicProcessor->textCorrector(pageText, mainString);
    xmlString=mainString;
    
#ifdef MAIN_MODE
        DR(" start grammar corrector")
        //cout<<mainString;
        //logicProcessor->grammarCorrector(strArray,correctionTable,mainString,xmlString,LOCAL_MODE);
        //logicProcessor->grammarCorrector(mainString,xmlString,1);
        //xmlString=mainString;
  
#endif
        DR_("done Grammar"<<endl);
 */
    
 }////////////////////////////////////////////////////////////////////////////////




