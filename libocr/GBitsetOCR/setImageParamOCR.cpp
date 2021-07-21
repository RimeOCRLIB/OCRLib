#include "GBitsetOCR.h"
#include "../GImageEditor/GImageEditor.h"


//функция выполнения определения параметров графической обработки страницы.
int GBitsetOCR::setImageParamOCR(GBitmap *pechaImg_,vector<float*>&param,OCRPointFloat &point){

    int print=0;

    int baseCount;
	unsigned int letterW,letterH;
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
    
    
    DR("//_________________________detect page image parameters by OCR"<<END);
    if(strArray[0].size()==0)return -1;


    baseCount=0;  letterW=0; letterH=0;
        
    if(!aliKali)return -1;
    if(aliKali->letterCount()<1)return -1;

    int OCRMode=OCR_NORMALISATION; //- Tibetan print text
    //int OCRMode=2; //- Tibetan woodblock text
        
    //float avarageScale=0;
    //int lineCount=0;

    //TIME_START

    //вычисляем номера строк в которых будет идти поиск
    //запоминаем номер строки в статическую переменную. При следующем вызове функции начинаем с этой строки
    static int lineIndex=(int)strArray[0].size()-(int)strArray[0].size()/4;
    // DR("lineIndex="<<lineIndex<<endl)
    
	//for (int index=15;index>=15; --index){  //for every string
	for (int index=lineIndex;index>=(int)strArray[0].size()/4; --index){  //for every string
        // DR(" index="<<index<<endl)
        matchLine.resize(0);
        //if(index!=21)continue;
        
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
        
        scale=param[0][0];  //оптимальный для распознавания размер основных букв шрифта вычислен на предыдущем этапе.
        int Base=param[0][2];
        if(Base==-1)Base=textLineSize*1.5;
        if(Base>32)Base=32;
        param[0][2]=Base;
        DR(" textLineSize1="<<textLineSize<<" scale="<<scale<<" Base="<<param[0][2]<<endl);


        //int correlation;
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
       //выполняем первое распознавание строки и определяем достаточно ли в этой строке букв для определения графических прараметоров.
       GBitmap *line =GBitmap::create(lineImg);
       adaptiveFiltration1(line,param[0],2);
       setMatrix=GBitsetMatrix::createRegion(line,
                                              0,
                                              0,
                                              line->columns(),
                                              line->rows(),
                                              1,1,
                                              IMGNOFLIP
                                              );
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(line,"/_Image2OCR/_1Draw_.jpg",0); //exit(0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageData((GBitmap*)&setMatrix->drawPoint,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/_1DrawRGB.jpg",0);
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);

        

	    DT2("m0_2");
		                //GImageEditor *editor=GImageEditor::create();
		                //editor->WriteImageData(lineScale,"/__XML__/1.jpg",0); //exit(0);
	    DT2("m1");

        setMatrix->letterCorrelation(matchLine,
                                 aliKali,
                                 index,
                                 LimY0*scale,
                                 (LimY0+textLineSize)*scale,
                                 0,   //no scale mode
                                 OCRMode,
                                 0);  //0 no print

		DT2("m3");
        setMatrix->destroy();
        line->destroy();
         DR(" matchLine.size="<<matchLine.size()<<endl)
        if(matchLine.size()<10){
            lineImg->destroy();
            continue;
        }else{
            lineIndex=index;  //запоминаем номер строки в статическую переменную. При следующем вызове функции начинаем с этой строки
            sort(matchLine.begin(),matchLine.end(),sortMatchC);
            int count=0;
            float avarageCorrellation=0;
            for(int i=0;i<matchLine.size()/4;i++){
                avarageCorrellation+=matchLine[i].correlation;
                // DR("id="<<matchLine[i].letterIndex<<" c="<<matchLine[i].correlation<<" n="<<matchLine[i].name<<endl)
                count++;
            }
            avarageCorrellation=(float)avarageCorrellation/count;
             DR("avarageCorrellation="<<avarageCorrellation<<" matchLine.size="<<matchLine.size()<<endl)
            
            ///для найденой строки проверяем все группы параметров графической обработки
            float maxCor=avarageCorrellation; int maxCorIndex=-1;
            
            for(int indexParam=0;indexParam<param.size();indexParam++){
                 DR("//__"<<indexParam<<endl)
                line=GBitmap::create(lineImg);
                matchLine.resize(0);
                if(param[indexParam][2]==-1)param[indexParam][2]=Base;

                adaptiveFiltration1(line,param[indexParam],2);
                setMatrix=GBitsetMatrix::createRegion(line,
                                                      0,
                                                      0,
                                                      line->columns(),
                                                      line->rows(),
                                                      1,1,
                                                      IMGNOFLIP
                                                      );
                //static string strPath="/_Image2OCR/_1Draw_"; strPath+="@";  string path=strPath+".jpg";
                //((GImageEditor*)inputData.imageEditor)->WriteImageData(line,path,0); //exit(0);
                //((GImageEditor*)inputData.imageEditor)->WriteImageData((GBitmap*)&setMatrix->drawPoint,"/_Image2OCR/_1Draw_.jpg",0); exit(0);
                //((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/_Image2OCR/_1DrawRGB.jpg",0);
                //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
                
                setMatrix->letterCorrelation(matchLine,
                                             aliKali,
                                             index,
                                             LimY0*scale,
                                             (LimY0+textLineSize)*scale,
                                             0,   //no scale mode
                                             OCRMode,
                                             0);  //0 no print
                
                DT2("m3");
                setMatrix->destroy();
                line->destroy();
                float avarageCorLine=0;
                DR("matchLine.size()="<<matchLine.size()<<endl)
                if(matchLine.size()<10){
                    continue;
                }else{
                    sort(matchLine.begin(),matchLine.end(),sortMatchC);
                    int count=0;
                    for(int i=0;i<matchLine.size()/4;i++){
                        avarageCorLine+=matchLine[i].correlation;
                        // DR("id="<<matchLine[i].letterIndex<<" c="<<matchLine[i].correlation<<" n="<<matchLine[i].name<<endl)
                        count++;
                    }
                    avarageCorLine=(float)avarageCorLine/count;
                     DR("avarageCorLine="<<avarageCorLine<<endl)
                }
                if(avarageCorLine>maxCor){
                    maxCor=avarageCorLine;
                    maxCorIndex=indexParam;
                    point.x=maxCor;  //передаем значение максимальной корреляции в вызывающие функции подбора параметоров графической обработки
                }
                
            }
            lineImg->destroy();
            return maxCorIndex;
        }
        
	}
    return -1;
    
	
 }////////////////////////////////////////////////////////////////////////////////




