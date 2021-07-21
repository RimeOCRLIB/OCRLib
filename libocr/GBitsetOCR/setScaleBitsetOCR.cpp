#include "GBitsetOCR.h"
#include "../GImageEditor/GImageEditor.h"

float GBitsetOCR::setScaleBitsetOCR(GLogicProcessor *logicProcessor, int mode){
	int baseCount;
	unsigned int letterW,letterH;
	int print=0;
	int w,h,yLimit0,yLimit1,id_index;
	string letter;
	int indexW;
	mainString="";
	indexW=0; id_index=0;
	vector<wordOCR>lineText;
	vector<OCRMatch>matchLine;
    int textLineSize;
	
	int lineCorrelation=0;
    float scale;
    
    DR("//_________________________detect page scale by OCR strArray[0].size()="<<strArray[0].size()<<END);
    if(strArray[0].size()<5)return -1;


    baseCount=0;  letterW=0; letterH=0;
        
    if(!aliKali)return 1;
    if(aliKali->letterCount()<1)return 1;

    int OCRMode=OCR_NORMALISATION; //- Tibetan print text
    //int OCRMode=2; //- Tibetan woodblock text
        
    float avarageScale=0;
    int lineCount=0;

TIME_START

    //вычисляем номера строк в которых будет идти поиск
    int dLine=(int)strArray[0].size()/4;
    
	//for (int index=15;index>=15; --index){  //for every string
	for (int index=(int)strArray[0].size()-1-dLine;index>=dLine; --index){  //for every string
    

        //if(index!=21)continue;
        matchLine.resize(0);
        
	    lineCorrelation=0; lineText.resize(0);
		//if(readFromCorrectionTable(correctionTable,pageText,strArray[0].size()-3-index))continue;  //read string from human prof reading pages.
        //##if(readFromCorrectionTable(correctionTable,pageText,index))continue;  //read string from human prof reading pages.
		    
        //Create GBitsetMatrix for matrix correlation
        //SH(inputData.borderSize());
        w=inputBitmap->columns();
        h=inputBitmap->rows();

        //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
        yLimit0=(strArray[0][index].y1-strArray[0][index].y0)*3;
        yLimit1=(strArray[0][index].y1-strArray[0][index].y0)*4;
        int LimY0=yLimit0*0.8;
 
        //normalisation
        DT2("w="<<w<<"h="<<h<<
           "y0="<< strArray[0][index].y0<<" y1="<< strArray[0][index].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<END);

        if(strArray[0][index].y0-yLimit0<0)yLimit0=strArray[0][index].y0;
        if(strArray[0][index].y1+yLimit1>h)yLimit1=h-strArray[0][index].y1;
                        
        DT2("w="<<w<<"h="<<h<<
        "y0="<< strArray[0][index].y0<<" y1="<< strArray[0][index].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<END);
        DT2("@@Y0="<<strArray[0][index].y0-yLimit0<<endl);
        DT2("@@H="<<strArray[0][index].y1-strArray[0][index].y0+yLimit0+yLimit1);
           
        //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
        textLineSize=strArray[0][index].y1-strArray[0][index].y0;
        yLimit0=textLineSize*1.6;
        yLimit1=textLineSize*2.4; //2.4
        //normalisation
        
        scale=(float)28/textLineSize;
        int BaseTr=textLineSize;
        
        //32 это оптимальный для распознавания размер основных букв шрифта.
        //при распозновании габарита строк строка обычно уже чем размер основных букв
        //scale=0.7672;
        DR(" textLineSize1="<<textLineSize<<" scale="<<scale<<endl);

        //int correlation;
        //GBitmap *lineScale;
        GBitmap *lineImg;
        
        strArray[0][index].clearOCR();
        lineText.resize(0);
        int w32=inputBitmap->columns();
        w32=(w32/32)*32;  //выравнивание по 32 нужно для графических функций
        lineImg=GBitmap::createRegion (inputBitmap,
                                       0,
                                       strArray[0][index].y0-yLimit0,
                                       w32,
                                       yLimit0+textLineSize+yLimit1
                                       );
        float p[]={scale,85,(float)BaseTr,0,2,0,47,1,1,0,0};
        adaptiveFiltration1(lineImg,p,2);
        setMatrix=GBitsetMatrix::createRegion(lineImg,
                                              0,
                                              0,
                                              lineImg->columns(),
                                              lineImg->rows(),
                                              1,1,
                                              IMGNOFLIP
                                              );
        //static string strPath="/_Image2OCR/_1Draw_"; strPath+="@";  string path=strPath+".jpg";
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg,"/_Image2OCR/_1Draw_.jpg",0); //exit(0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageData((GBitmap*)&setMatrix->drawPoint,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/_1DrawRGB.jpg",0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);

		                //GImageEditor *editor=GImageEditor::create();
		                //editor->WriteImageData(lineScale,"/__XML__/1.jpg",0); //exit(0);

        setMatrix->letterCorrelation(matchLine,
                                 aliKali,
                                 index,
                                 LimY0*scale,
                                 (LimY0+textLineSize)*scale,
                                 0,   //no scale mode
                                 OCRMode,
                                 0);  //0 no print

        setMatrix->destroy();
        

        lineImg->destroy();
        
        int limitY=yLimit0*scale;
        
        for(int i=0;i<matchLine.size();i++){
            if(matchLine[i].y0>limitY+7||matchLine[i].y0<limitY-7)matchLine[i].correlation=0;
        }
         DR("matchLine.size()="<<matchLine.size()<<endl)
        if(matchLine.size()<10){
            continue;
        }
        
        sort(matchLine.begin(),matchLine.end(),sortMatchC);
        int count=0;
        float avarageLetterH=0;
        for(int i=0;i<matchLine.size()/5;i++){
            int h=matchLine[i].lineH;
            avarageLetterH+=h;
             DR("id="<<matchLine[i].letterIndex<<" y0="<<matchLine[i].y0<<" h="<<h<<" c="<<matchLine[i].correlation<<" n="<<matchLine[i].name<<endl)
            count++;
        }
        avarageLetterH=(float)avarageLetterH/count;
         DR("avarageLetterH="<<avarageLetterH <<" y0="<<limitY<<endl)

        
        

        //logicProcessor->drawGrapeLine(matchLine);
        //вычисляем масштаб строки с учетом полученных данных о средней высоте буквы
        scale=scale*((float)30/avarageLetterH);
        avarageScale+=scale;
        // DR("averageLetterH="<<setMatrix->avarageLetterH<<" scale="<<scale<<endl)
        lineCount++;
        
	}
    TIME_PRINT
    if(lineCount){
        scale=(float)avarageScale/lineCount;
         DR("scale="<<scale<<endl)
        return scale;
    }else{
        return -1;
    }
    
    
	
 }////////////////////////////////////////////////////////////////////////////////




