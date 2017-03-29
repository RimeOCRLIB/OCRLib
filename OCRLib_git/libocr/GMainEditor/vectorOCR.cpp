#include "GMainEditor.h"
#include "php2stl.h"


///Распознавание на основе фокальных точек и векторов
void GMainEditor::vectorOCR(){
    
    vector<OCRPoint>strPoint;        //массив фокальных точек
    vector<OCRFocalLine>lineVec;     //массив фокальных линий
    vector<OCRFocalLine>lineOCRVec;  //массив фокальных линий
    
    inputData.data["inputFile"]=inputData.fileList[0];
    pechaImg=LoadImageData(inputData.data["inputFile"],0);
    pechaImg->eraseBorder(2, 2);
    if(pechaImg->DetectGradations())pechaImg->binarisation(85,24);
    GGraph pechaGraph=GGraph(pechaImg);
    pechaGraph.focalLine(15);
    pechaGraph.focalPointXTL(strPoint);
    pechaGraph.focalLineRecognition(strPoint,lineVec);
    
    // распознавание макета страницы
    //распознавание слов и фраз
    strArray.resize(0);
    GBitset *setBit=GBitset::createResize(pechaImg,1,1,1);
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
    //uint border=setBit->border_size();
    setBit->destroy();
    
    DT("@4_4");
    GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
    //if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
    
    if(strArray.size()==0)return;
    if(!aliKali)return;
    if(aliKali->letterCount()<1)return;

    
    int baseCount;
    unsigned int letterW,letterH;
    int w,h,yLimit0,yLimit1,id_index;
    string letter,xmlString;
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
    rewriteOCRCash=0;//((GStr<int>*)inputData.pref)->get(5);
    //((GStr<int>*)inputData.pref)->put(5,0);
    
    rewriteOCRCash=1;
    
    
    int indexString=0;  //14  строка для распознавания. //-2 full page //-1 debug mode
    
    inputData.lineSize.resize(5);
    
    //for (int index=1;index>=1; --index){  //for every string
    for (int index=(int)strArray.size()-1;index>=0; --index){  //for every string
        cout<<". "<<index;
        //cout<<"strArray["<<index<<"].y0="<<strArray[index].y0<<" y1="<<strArray[index].y1<<endl;
        
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

        //Create GBitsetMatrix for matrix correlation
        w=pechaImg->columns();
        h=pechaImg->rows();
        
        //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
        int textLineSize=strArray[index].y1-strArray[index].y0;
        yLimit0=textLineSize*1.6;
        yLimit1=textLineSize*2.4; //2.4
        //normalisation
        
        //DR("w="<<w<<"h="<<h<<
        //   "y0="<< strArray[index].y0<<" y1="<< strArray[index].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<" y0_1="<<strArray[index-1].y0<<END);
        
        
        if(strArray[index].y0-yLimit0<0)yLimit0=strArray[index].y0;
        if(strArray[index].y1+yLimit1>h)yLimit1=h-strArray[index].y1;
        int dL=5;
        if(index>0&&strArray[index].y1+yLimit1>strArray[index-1].y0-dL){yLimit1=strArray[index-1].y0-strArray[index].y1-dL;}
        
        int LimY0=yLimit0;
        //cout<<" textLineSize="<<textLineSize<<" scale="<<(float)32/textLineSize; //exit(0);
        
        textLineSize=32;
        LimY0=LimY0;
        
        strArray[index].LimY0=LimY0;
        strArray[index].LimY1=LimY0+textLineSize;  //передаем координаты строки для шрифтовой грамматики
        
        int w32=pechaImg->columns();
        w32=(w32/32)*32;  //выравнивание по 32 нужно для графических функций
        lineImg=GBitmap::createRegion (pechaImg,
                                       0,
                                       strArray[index].y0-yLimit0,
                                       w32,
                                       yLimit0+textLineSize+yLimit1
                                       );
        
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
        
        GBitsetMatrix* setMatrix=GBitsetMatrix::createRegion(lineImg,
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
            //((GStr<int>*)inputData.pref)->put(0,LimY0);
            inputData.lineSize[0]=LimY0;
            //((GStr<int>*)inputData.pref)->put(1,(LimY0+textLineSize));
            inputData.lineSize[1]=(LimY0+textLineSize);
            //((GStr<int>*)inputData.pref)->put(2,lineImg->columns());
            inputData.lineSize[2]=lineImg->columns();
            //((GStr<int>*)inputData.pref)->put(3,lineImg->rows());
            inputData.lineSize[2]=lineImg->rows();
        }
        
        
        //logicProcessor->drawGrapeLine(matchLine); //exit(0);
        logicProcessor->classification(strArray,
                                          matchLine,
                                          lineImg32,
                                          mainString,
                                          pechaImg->columns()+256,
                                          index);
        
    }
    string mainString=setOCR->mainString;
    
    //vectorCorrelation(strPoint,lineVec);
    //drawVector(strPoint,lineVec);
    
    cout<<"done";
}//_______________________________________________________

//сегментация страницы, последовательное распознавание букв, слов, фраз и строк
void GMainEditor::vectorCorrelation( vector<OCRPoint>&strPoint, vector<OCRFocalLine>&lineVec){
    
    
    
    
}


