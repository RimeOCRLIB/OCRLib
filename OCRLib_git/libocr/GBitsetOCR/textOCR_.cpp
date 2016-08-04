#include "GBitsetOCR.h"


void GBitsetOCR::textOCR_(vector<stringOCR>&correctionTable,GLogicProcessor *logicProcessor, int mode){
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
	
	static float bestScale=0;
	static int bestCorrelation=0;
	int needLineSize=0;
	int lineCorrelation=0;
    float scalePage=(float)atoi(inputData.data["scale"].c_str())/100;  //1 не выполняется масштабирование
    if(scalePage<0)scalePage=0; if(scalePage>5)scalePage=5;  //cout<<"scale="<<scalePage<<endl;
    GBitmap *line;
    
#define  c_out_ cout

int draw=0;
if(inputData.data["ocrData"]=="drawLetter"){
	draw=1; print=0;

c_out_<<"<span style=\"font-family:OCRUnicode;\">";
c_out_<<"inputData.ocrData="<<inputData.data["ocrData"]<<END;
}
	
DT("//_________________________print text OCR"<<END);
DT("//_________________________start correlation"<<END);
if(strArray[0].size()==0)return;


	

	baseCount=0;  letterW=0; letterH=0;
		//setOCRCorrelation(0,1,ROOT_LETTER); return;
if(!aliKali)return;    
if(aliKali->letterCount()<1)return;

//int OCRMode=1; //- Tibetan print text
int OCRMode=2; //- Tibetan woodblock text

TIME_START
    
	//for (int index=15;index>=15; --index){  //for every string
	for (int indexLine=(int)strArray[0].size()-1;indexLine>=0; --indexLine){  //for every string
        
        if((inputData.data["ocrData"]=="testOCRLine")&&!strArray[0][indexLine].selectFlag){continue;}

        //if(index!=21)continue;
        
	    lineCorrelation=0; lineText.resize(0);
		//if(readFromCorrectionTable(correctionTable,pageText,strArray[0].size()-3-index))continue;  //read string from human prof reading pages.
        //##if(readFromCorrectionTable(correctionTable,pageText,index))continue;  //read string from human prof reading pages.
		    
						//Create GBitsetMatrix for matrix correlation
						//SH(inputData.borderSize());
						w=inputBitmap->columns();
						h=inputBitmap->rows();
		
		                //*** ВЫЧИСЛЕНИЕ ГАБАРИТОВ ПОИСКА БУКВ В СТРОКЕ ***///
		                   yLimit0=(strArray[0][indexLine].y1-strArray[0][indexLine].y0)*3;	
		                   yLimit1=(strArray[0][indexLine].y1-strArray[0][indexLine].y0)*4; 
		                   int LimY0=yLimit0*0.8;  
		                  //normalisation
		

							DT2("w="<<w<<"h="<<h<<
							   "y0="<< strArray[0][indexLine].y0<<" y1="<< strArray[0][indexLine].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<END);
		
		                   if(strArray[0][indexLine].y0-yLimit0<0)yLimit0=strArray[0][indexLine].y0;
						   if(strArray[0][indexLine].y1+yLimit1>h)yLimit1=h-strArray[0][indexLine].y1;
											
		                   DT2("w="<<w<<"h="<<h<<
						   "y0="<< strArray[0][indexLine].y0<<" y1="<< strArray[0][indexLine].y1<<" yLimit0="<<yLimit0<<" yLimit1="<<yLimit1<<END);
		                   DT2("@@Y0="<<strArray[0][indexLine].y0-yLimit0<<endl);
		                   DT2("@@H="<<strArray[0][indexLine].y1-strArray[0][indexLine].y0+yLimit0+yLimit1);
		                   
		                int textLineSize=strArray[0][indexLine].y1-strArray[0][indexLine].y0;
		      
						DT2(" textLineSize1="<<textLineSize);
		                float scale=0,k;   
		                int needScaleFlag=1; //1 если нужно масштабировать строку
		               
		if(scalePage!=1){
                                line=GBitmap::createRegion(inputBitmap, 
															0, 
															strArray[0][indexLine].y0-LimY0, 
															inputBitmap->columns(), 
															yLimit0*2.5
		                                                    );
			                    if(scalePage!=0)needScaleFlag=0; 
		}                
		                 int correlation; 
		                 GBitmap *lineScale;
       
       //pageTextSize0=pageText.size();   //размер текста до распознавания строки
	   
       ScaleDetector:;   DT("needScaleFlag="<<needScaleFlag<<" needLineSize="<<needLineSize<<endl);
       
       strArray[0][indexLine].clearOCR();
       lineText.resize(0);
        
       //pageTextSize1=pageText.size(); //размер текста до распознавания строки или после первого распознавания с выбранным масштабом (после возврата по GOTO)
	   
        if(scalePage==1){
           scale=1;
           needScaleFlag=0; 
           
        }

       if(needScaleFlag){ 
                        if(needLineSize<3){
                            scale=1;
							scale=lineOCRScaleDetector(line,
													   matchLine, 
													   LimY0*scale, 
													   (LimY0+textLineSize)*scale, 
													   scale,
													   indexLine,
													   &correlation,
													   1);
							if(scale){
								k=bestScale/scale; DT("bestScale="<<bestScale<<" scale="<<scale<<" k="<<k<<endl);
							    if(k>0.8&&k<1.2){needLineSize++;}else{needLineSize=0;}
							}else{needLineSize=0;}	
							bestScale=scale;
							bestCorrelation=correlation;
							
						}else{
						    scale=bestScale;
						}
							
		                if(scale==0){
							scale=1;  //no rezult from scale detector
                            //continue;
							//stepCount++;
							//if(stepCount>2){needLineSize=0;continue;}
							//goto_ScaleDetector;
			            }
           //cout<<"@3index="<<index<<" scalePage="<<scalePage<<" needScaleFlag="<<needScaleFlag<<endl;

	 DT("m0_1");    
                      
                        ///scale=1.74;
                       lineScale=GBitmap::createScale(line,scale);
                       setMatrix=GBitsetMatrix::createRegion(lineScale,
                                                             0,
                                                             0,
                                                             lineScale->columns(),
                                                             lineScale->rows(),
                                                             1,1,
                                                             IMGNOFLIP
                                                             );      
                       lineScale->destroy();
	}else {

                        if(scalePage!=1){ 
                            scale=scalePage;
                            lineScale=GBitmap::createScale(line,scale);
                            setMatrix=GBitsetMatrix::createRegion(lineScale,
                                                                  0,
                                                                  0,
                                                                  lineScale->columns(),
                                                                  lineScale->rows(),
                                                                  1,1,
                                                                  IMGNOFLIP
                                                                  );      
                            lineScale->destroy();                      
                        }else{
                        
                        setMatrix=GBitsetMatrix::createRegion(inputBitmap, 
                                                              0, 
                                                              strArray[0][indexLine].y0-LimY0, 
                                                              inputBitmap->columns()/2,
                                                              yLimit0*2.5,
                                                              1,1,
                                                              IMGNOFLIP
                                                              );   
                         }    
		                
	}
	DT("m0_2");
		                //GImageEditor *editor=GImageEditor::create();
		                //editor->WriteImageData(lineScale,"/__XML__/1.jpg",0); //exit(0);
	DT("m1");

						setMatrix->letterCorrelation(matchLine,
													 aliKali,
                                                     indexLine,
													 LimY0*scale, 
													 (LimY0+textLineSize)*scale,
													 0,   //no scale mode
                                                     OCRMode,
													 0);  //0 no print

		DT2("m3");
						#ifndef REPORT_DRAW
		                setMatrix->destroy();
                        #endif
        


cout<<"new string"<<endl; sleep(10);

						//now we start check result of correlation and make decision about place of every letter
	   DT2("m3_1");					//разборка строки на буквы в соответствии с величиной корреляции и вложенностью букв. например О-С
        
	}
    TIME_PRINT
	    DT2("start grammar corrector"<<END);	
       // logicProcessor->grammarCorrector(strArray,correctionTable,mainString,xmlString,LOCAL_MODE);
        
        DT2("done Grammar"<<endl);
   
    
	
 }////////////////////////////////////////////////////////////////////////////////




