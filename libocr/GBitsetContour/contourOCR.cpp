//main point for letter OCR implementation
//read letter from text, create all record for it
//join and separate letters part

#include "GBitsetContour.h"

void GBitsetContour::setData(vectorOCR *vectorBase_,vector<stringOCR>&strArray_,int iLeft_, int iTop_) {   //main point for GLetter Object. Order of all Letter OCR function
w=ncolumns;
h=nrows;  //read data from array
//recordName=(wchar_t)""; //we don't know name for this record
vectorBase=vectorBase_;
strArray=&strArray_;
iLeft=iLeft_;
iTop=iTop_;
DC("start OCR 2");
	angCount=0; lenCount=0;  contourCount=0; vectorCount=0;
vectorCount=0;
baseOCR();
DC("ALL ANGLE COUNT "<<angCount<<END);
DC("ALL CONTOUR COUNT "<<contourIndex<<END);
DC("ALL VECTOR COUNT "<<vectorCount<<END);
DC("complete");
//drawContour(); //рисование контруров в drawData
//vDraw(); //рисование контруров в drawData
}//___________________________________________________________________________________________________

///инициализация внутренних данных и создание контурной базы
void GBitsetContour::buildGFont(GFont *contourBase,vector<stringOCR>&strArray_){
	int print=1;
    
    w=ncolumns;
    h=nrows;  //read data from array
    //vectorBase=vectorBase_;
    strArray=&strArray_;
    DC("start OCR 2");
	angCount=0; lenCount=0;  contourCount=0; vectorCount=0;
    vectorCount=0;
    //first point of letter's, read its contour, implementation of all contour methods
	startDir=0;
	short  borderPoint0, borderPoint1, borderPoint2,borderPoint3,borderPoint4;
	char clockwise;
	indexFilter=0; contourIndex=0;
    set_sqrt_array();

	DT("start_OCR_contour"<<END);
	
	//   borderPoint
    //        1 
    //        | 
    //     3- 0 -4
    //        | 
    //        2 
    //
    
    int letterIndex=-1;
	
	for (int x_base=border;x_base<ncolumns-border;x_base++){  // цикл перебора всех точек печа       
		for(int y_base=border;y_base<nrows-border;y_base++){  
            
            //cout<<"x="<<x_base<<" y="<<y_base<<" ";
            //cout<<line[0][y_base][x_base]<<endl;
            
            borderPoint0=line[0][y_base][x_base];    //определение окружения точки
			borderPoint1=line[0][y_base-1][x_base];
			borderPoint2=line[0][y_base+1][x_base];
			borderPoint3=line[0][y_base][x_base-1];
			borderPoint4=line[0][y_base][x_base+1];
            
			clockwise=10; int signal=-1;
			if (borderPoint0>0){  DC("//___start1. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
				if(borderPoint0==1&&borderPoint1==0){clockwise=1;startDir=0;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;
					signal=0;
				} //нахождение граничного пиксела/found border pixel
                
				if(borderPoint0==1&&borderPoint2==0&&borderPoint1>0){clockwise=0; startDir=4;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;  DC("//___start2. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
					signal=1;
				} //нахождение граничного пиксела/found border pixel
                
				if(borderPoint0==11&&borderPoint2==0&&borderPoint1==0&&(borderPoint3!=0&&borderPoint4!=0)){clockwise=0; startDir=4; //cout<<"border point"<<contourCount<<endl;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;   DC("//___start3. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
					signal=2;
				} //нахождение граничного пиксела/found border pixel
				// if(borderPoint0==2000){clockwise=0;cout<<"start readInnerContour"<<endl;} //found border pixel
				char contour=0;
                
				while(endContour==0&&clockwise!=10){
					DT("clockwise_1="<<(short)clockwise<<" x_base="<<x_base<<" y_base"<<y_base<<" signal="<<signal<<END);
					if(clockwise==1){  DT("start readOuterContour"<<END);
						readOuterContour(10);contour=1;  //чтение внешнего контура
					}
					if(clockwise==0){  DT("start readInnerContour"<<END);
						readOuterContour(20);contour=0;   //чтение внутреннего контура
					}
                    
                    //setSize(); //test size of letter contour for OCR calibration
                    
                    DT(" contour complete"<<"angLen="<<angLength<<" contour="<<(short)contour<<END);
                    if(angLength>Radius*1.2&&angLength<3000){ //если контур соответствует норме
						 
                        GContour* letterContour=GContour::create(angX,angY,angData,angLength,contour);
                       
                         DT(" contour width="<<letterContour->width()<<" height="<<letterContour->height()<<END);
                        
                        if(letterContour->width()-letterContour->border*2>128||letterContour->height()-letterContour->border*2>128)continue;
                        
                        fillContour(letterContour);
                        if(letterIndex>=0){//проверяем находится ли контур внутри предыдущего контура
                            GLetter *letter=contourBase->getLetter(letterIndex);
                            
                            DT("dX="<<letterContour->dX<<" dX+w="<<letterContour->dX+letterContour->width()<<
                            "Max="<<letter->xMax<<
                            "dy="<<letterContour->dY<<" dY+h="<<letterContour->dY+letterContour->height()<<
                            "yMax="<<letter->yMax<<endl);
                            int dL=letter->letterW;
                            for(int a=0;a<contourBase->letterCount();a++){ //ищем внутри какой буквы находится этот контур
                                letter=contourBase->getLetter(a);
                                if(letterContour->dX>letter->xMin-dL/5&&
                                   letterContour->dX+letterContour->width()<letter->xMax+dL/5&&
                                   letterContour->dY>letter->yMin-dL&&
                                   letterContour->dY+letterContour->height()<letter->yMax){                           
                                      letter->contour[letter->contourCount]=letterContour;   
                                      letter->contourCount++;
                                      letter->drawContour();  
                                      goto exitSetLetter;
                                }
                            }
                                    
                        }
                       
                        
                        if(contour){
                            letterIndex=(int)contourBase->letterCount();
                            GLetter *letter=GLetter::create();
                            
                            letter->recordFlag=1;
                            letter->xMin=letterContour->dX;
                            letter->yMin=letterContour->dY;
                            letter->contour[letter->contourCount]=letterContour;
                            letter->contourCount++;
                            letter->xMax=letterContour->dX+letterContour->width();
                            letter->yMax=letterContour->dY+letterContour->height();
                            letter->letterW=letterContour->width()-letterContour->border*2;
                            letter->letterH=letterContour->height()-letterContour->border*2; 
                            letterContour->dX=0;letterContour->dY=0;
                            letter->drawContour();
                            
                            letter->name=":)";
                            letter->idNumber=letterIndex;
                            //contourBase+=letter; //####
                            
                             
                        }
                        exitSetLetter:;
                        contourIndex++;  
                        //DT("letterIndex="<<letterIndex<<" y0="<<contourBase[0][letterIndex]->yMin<<" x="<<contourBase[0][letterIndex]->xMin<<" name="<<contourBase[0][letterIndex]->name<<" h="<<contourBase[0][letterIndex]->letterH<<endl);  
                     
                    }
                    
				}//while(endContour==0)

			}//if (borderPoint0>70)
            
            

		}}//end for base x, y
    
	//for (n=0;n<filterIndex;n++)cout <<"filter"<< strArray[n]<<" filterCount="<<filterCount[n]<<endl; //print filter statistic

    DT("ALL CONTOUR COUNT "<<contourIndex<<END);
    DT("complete"<<endl);



}


// последовательность процесса векторизации
void GBitsetContour::baseOCR(){
	//first point of letter's, read its contour, implementation of all contour methods
	startDir=0;
	short  borderPoint0, borderPoint1, borderPoint2,borderPoint3,borderPoint4;
	char clockwise;
	indexFilter=0; contourIndex=0;
        set_sqrt_array();
	int print=1;
	DT("start_OCR_contour"<<END);
	
	//   borderPoint
    //        1 
    //        | 
    //     3- 0 -4
    //        | 
    //        2 
    //
	
	
	for (int x_base=3;x_base<w-3;x_base++){  // цикл перебора всех точек печа
		// if(x_base>position+step){position=x_base;[mainProgressBar setDoubleValue:(x_base*100)/w];[mainProgressBar displayIfNeeded];}

		for(int y_base=3;y_base<h-3 ;y_base++){
			borderPoint0=line[0][y_base][x_base];    //определение окружения точки
			borderPoint1=line[0][y_base-1][x_base];
			borderPoint2=line[0][y_base+1][x_base];
			borderPoint3=line[0][y_base][x_base-1];
			borderPoint4=line[0][y_base][x_base+1];

			clockwise=10; int signal=-1;
			if (borderPoint0>0){  DC("//___start1. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
				if(borderPoint0==1&&borderPoint1==0){clockwise=1;startDir=0;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;
					signal=0;
				} //нахождение граничного пиксела/found border pixel

				if(borderPoint0==1&&borderPoint2==0&&borderPoint1>0){clockwise=0; startDir=4;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;  DC("//___start2. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
					signal=1;
				} //нахождение граничного пиксела/found border pixel

				if(borderPoint0==11&&borderPoint2==0&&borderPoint1==0&&(borderPoint3!=0&&borderPoint4!=0)){clockwise=0; startDir=4; //cout<<"border point"<<contourCount<<endl;
					xL=x_base; yL=y_base; endContour=0;
					line[0][yL][xL]=150;   DC("//___start3. ("<<borderPoint0<<") x_base="<<x_base<<" y_base="<<y_base<<END);
					signal=2;
				} //нахождение граничного пиксела/found border pixel
				// if(borderPoint0==2000){clockwise=0;cout<<"start readInnerContour"<<endl;} //found border pixel
				char contour=0;

				while(endContour==0&&clockwise!=10){
					DT("clockwise_1="<<(short)clockwise<<" x_base="<<x_base<<" y_base"<<y_base<<" signal="<<signal<<END);
					if(clockwise==1){  DT("start readOuterContour"<<END);
						readOuterContour(10);contour=1;  //чтение внешнего контура
					}
					if(clockwise==0){  DT("start readInnerContour"<<END);
						readOuterContour(20);contour=0;   //чтение внутреннего контура
					}

						//setSize(); //test size of letter contour for OCR calibration

						DT(" contour complete"<<"angLen="<<angLength<<END);
						if(angLength>Radius*1.2&&angLength<19990){ //если контур соответствует норме
						angleDiff(); //забиваем одинаковые значения угла нулями

						baseFilter(); //выполняем импульсную фильтрацию
												//_//_DC(" done start record"<<END<<END);
							//if(vectorBase->size()==0){vectorBase->resize(5000);vectorBase[0].allVectorCount=0;vectorBasebase.size()=0;}
							//int size=vectorBase->size()-vectorBase[0].allVectorCount;
							//if(size<500){vectorBase->resize(vectorBase->size()+5000);
							//}
							//_//_DC("size="<<size<<" vectorBase"<<vectorBase->size()<<" vectorCount"<<vectorBase[0].allVectorCount<<END);

						setVector(contour);  //векторизация сигнала и запись векторов в базу
												//if(vectorBase[0].allVectorCount>0)
												//_//_DC(" done record"<<" vectorBase[0].allVectorCount"<<
												//vectorBase[0].allVectorCount<<" contourIndex="<<
												//vectorBase[vectorBase[0].allVectorCount-1].contourIndex<<
												//END);
							//_//_DC(" done Start filter"<<END);
						contourIndex++;
						}
				}//while(endContour==0)

			}//if (borderPoint0>70)
		}}//end for base x, y

	//for (n=0;n<filterIndex;n++)cout <<"filter"<< strArray[n]<<" filterCount="<<filterCount[n]<<endl; //print filter statistic

}//end method////////////////////////////////////////////////////////**

void GBitsetContour::setImageHandler(){
//short x,y;
/*
//srcPecha_->uncompress();
//DC("wPecha="<<ncolumns<<END);
//DC("hPecha="<<nrows<<END);
	vector<int>buf(nrows+5); //for(int m=0;m<buf.size();m++)buf[m]=0;
line.push_back(buf);
line.push_back(buf);
line.push_back(buf);
	bool *row=bits_data;
	bool *p;

for(x = 0; x < ncolumns; x++ ) {
	  vector<int>buf1(nrows+5); line.push_back(buf1);
		for(y = 0; y< nrows; y++){
		p=row+(h-y)*ncolumns+x;
        *p ? line[x+2][y+2]=1 : line[x+2][y+2]=0;
		//cout<<line[x+2][y+2];//draw on screen
        }
	//cout<<endl;
}


//for(y = 0; y< nrows; y++){
//	for(x = 0; x < ncolumns; x+=3) {
//		cout<<line[x][y];//draw on screen
//        }
//		cout<<endl;
//}

line.push_back(buf);
line.push_back(buf);
line.push_back(buf);

//cout<<"done";
*/
}//end method////////////////////////////////////////////////////////////////////////////////////


void GBitsetContour::drawContour(){
#ifdef DEBUGLVL_GREY
for (int i=0;i<ncolumns;i++){
for (int j=0;j<nrows;j++){

if(line[0][j][i]==1)drawData[0][j][i]=120;
if(line[0][j][i]>1)drawData[0][j][i]=0;

}}
#endif    

}//__________________________________

void GBitsetContour::setFontData(vectorOCR *vectorBase_,GLetter *glyph, string &recordName_, short wScale){
w=ncolumns;   //cout<<"wScale1="<<wScale<<endl;
h=nrows;  //read data from array
//recordName=recordName_; //we already know name for this record
    vectorBase=vectorBase_;
	int startInd=0;
	int print=1;
	//srcPecha_->uncompress();
	cout<<"wPecha="<<ncolumns<<endl;
	cout<<"hPecha="<<nrows<<endl;
	
	vectorCount=0;
	if(vectorBase[0].allVectorCount>0){vectorBase[0].allVectorCount++;
	startInd=vectorBase[0].allVectorCount;
	DT(" vectorBase->size()="<<vectorCount<<" vectorBasebase.size()"<<vectorBase[0].allVectorCount<<END);
	}
    set_sqrt_array();

    baseOCR();  // //printf ("present=%d\n",present);
	
	//calibrate center of letter

	int maxX=0, minX=10000;
	int maxY=0, minY=10000;
	for(unsigned int n=startInd;n<vectorBase[0].allVectorCount;n++){
		if(vectorBase[n].vStart.x<minX)minX=vectorBase[n].vStart.x;
		if(vectorBase[n].vEnd.x<minX)minX=vectorBase[n].vEnd.x;
		if(vectorBase[n].vStart.x>maxX)maxX=vectorBase[n].vStart.x;
		if(vectorBase[n].vEnd.x>maxX)maxX=vectorBase[n].vEnd.x;

		if(vectorBase[n].vStart.y<minY)minY=vectorBase[n].vStart.y;
		if(vectorBase[n].vEnd.y<minY)minY=vectorBase[n].vEnd.y;
		if(vectorBase[n].vStart.y>maxY)maxY=vectorBase[n].vStart.y;
		if(vectorBase[n].vEnd.y>maxY)maxY=vectorBase[n].vEnd.y;
		
	}
	int centerX=w/2;
	int centerY=h/2;
	
	DT("centerX="<<centerX<<" centerY"<<centerY<<END);
	glyph->xMin=minX-centerX;
	glyph->yMin=minY-centerY;
	glyph->xMax=maxX-centerX;
	glyph->yMax=maxY-centerY;
	
	glyph->letterH=maxY-minY;
	glyph->letterW=maxX-minX;
	glyph->dX=ncolumns/2+glyph->xMin;
	glyph->dY=nrows/2+glyph->yMin;

	
	cout<<"xMin="<<glyph->xMin<<" yMin="<<glyph->yMin<<" xMax="<<glyph->xMax<<" yMax="<<glyph->yMax<<" glyph->letterW="<<glyph->letterW<<" dX="<<glyph->dX<<" dY="<<glyph->dY<<endl;

	
	for(unsigned int n=startInd;n<vectorBase[0].allVectorCount;n++){
	    vectorBase[n].vStart.x=vectorBase[n].vStart.x-centerX;
	    vectorBase[n].vEnd.x=vectorBase[n].vEnd.x-centerX;
	    vectorBase[n].vCenter.x=vectorBase[n].vCenter.x-centerX;
		
		vectorBase[n].vStart.y=vectorBase[n].vStart.y-centerY;
	    vectorBase[n].vEnd.y=vectorBase[n].vEnd.y-centerY;
	    vectorBase[n].vCenter.y=vectorBase[n].vCenter.y-centerY;
		
		//vectorBase[n].name=recordName_;
	}
	
	
	//for(int i=0;i<vectorBase[0].allVectorCount;i++){
	//	glyph->v[i]=vectorBase[i];
	//}
	//glyph->vectorCount=vectorBase[0].allVectorCount;
	glyph->name=recordName_;
	
DT("ALL ANGLE COUNT "<<angCount<<END);
DT("ALL CONTOUR COUNT "<<contourIndex<<END);
DT("ALL VECTOR COUNT "<<vectorCount<<END);
//DT("allLetterCount"<<vectorBase->base.size()<<END<<END);

}//___________________________________________________________________________________________________



void GBitsetContour::setSize(){
        int minCenter=1000, wIndex=-1, sIndex=-1;
        int xCenter=xMin+(xMax-xMin)/2;
        int yCenter=yMin+(yMax-yMin)/2;
        if(strArray[0].size()==0){sizeH=0;return;}
	    int print=1;

	for (int index=0;index<strArray[0].size(); index++){
		for (int i=0;i<strArray[0][index].wordArray.size(); i++){  //word by word
                 if(abs(strArray[0][index].wordArray[i].center.x-xCenter)+abs(nrows-strArray[0][index].wordArray[i].center.y-yCenter)<(minCenter)){
                    minCenter=abs(strArray[0][index].wordArray[i].center.x-xCenter)+abs(nrows-strArray[0][index].wordArray[i].center.y-yCenter);
                    wIndex=i;
                    sIndex=index;

                 }
                 DT("xCenter="<<xCenter<<" yCenter="<<yCenter<<END);
                 DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.x"<<strArray[0][index].wordArray[i].center.x<<END);
                 DT("strArray[0]["<<index<<"].wordArray["<<i<<"].center.y"<<nrows-strArray[0][index].wordArray[i].center.y<<END);
        }}
        if(sIndex<0||wIndex<0){sizeH=0;return;}
        sizeH=strArray[0][sIndex].wordArray[wIndex].h;
        DT("minCenter="<<minCenter<<" sIndex="<<sIndex<<" wIndex="<<wIndex<<" sizeH="<<sizeH<<END);
}//___________________________________________________________________________________________________















