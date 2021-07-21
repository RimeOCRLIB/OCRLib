#include "GFontEditor.h"
#include "../GImageEditorOCR/GImageEditorOCR.h"
//#include "../../re2/re2.h"

void GFontEditor::setScale(){
    int c=22186;
    for(int i=17226;i<c;i++){
        cout<<".";
        if(i%100==0)cout<<i<<" ";
        //int id[2];
        GLetter *letter=aliKali->getLetter(i);
        //if(RE2::PartialMatch(letter->name,"[\\p{Devanagari}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")){
           //aliKali->scaleLetter(letter,0.75);
           //rebuildMaskInLetterVector(letter,id);
            //aliKali->saveLetter(letter);
        //}
        letter->destroy();
    }
}


//_________________________________________________________________________________________________
void GFontEditor::rebuildMask(){
    
    //int step=0;
    string str;
    StartCpuInfo();
    cout<<"inputData.num_cores="<<inputData.num_cores<<endl;
    
    int index=14018;   //13890;
    
    //int forkCount=0;
    //int pfds[inputData.num_cores][2];
    //int pidID[inputData.num_cores],letterID[inputData.num_cores];
    //int pid,status;
    //int fIndex;
    
#ifdef FORK11
        //создаем изображения каждой буквы базы
        //изображения нужно хранить в базе. Для этого нужно сериализовать GBitmap
        
    //for(int in=0; in<aliKali->letterCount();in++){  //DT(" in="<<in<<" name"<<aliKali[0][in]->name.c_str());
    //    if(aliKali[0][in]->pict!=0)continue;
    //    aliKali[0][in]->pict=aliKali->drawPict(in);
    //}
        
        for(int n=0;n<inputData.num_cores;n++){
            pidID[n]=0;
        }
        
       
        
        while(index<aliKali->letterCount()){
            
            GLetter *letter=aliKali->getLetter(index);
            //for(int index=aliKali->letterCount-1;index>=0;index--){
			//cout<<index<<" index start"<<endl;
            //if(index<12400)path[n].c_str(){index++;continue;}
			cout<<index<<"@@@ aliKali[0]["<<index<<"]->OCRStatus="<<letter->OCRStatus<<" n="<<letter->name<<endl;
            
                if(letter->mask32Count()>0){index++;letter->destroy();cout<<"1";continue;}
                //if(letter->onOffRate>50){index++;letter->destroy();cout<<"2";continue;}
                //if(aliKali[0][index]->OCRStatus==100){index++;continue;}
                if(letter->name==":)"){index++;letter->destroy();cout<<"3";continue;}
                //if(aliKali[0][index]->name.find("*")!=-1){index++;continue;}
                if(letter->OCRIndex==3){index++;letter->destroy();cout<<"4";continue;}
                //if(aliKali[0][index]->mask32Count()<15){index++;continue;}
                //if(aliKali[0][index]->selfCorrelation>96&&aliKali[0][index]->codeSpace<96&&aliKali[0][index]->angLength>80){index++; continue;}
                //if(aliKali[0][index]->mask32Count()>2){index++;continue;}
                /*
                for(int a=0; a<aliKali[0][index]->mask32Count();a++){
                    w=aliKali[0][index]->mask32[a].imgW;
                    h=aliKali[0][index]->mask32[a].mH;
                    aliKali[0][index]->mask32[a].dlt0=(w+h)/3;
                    if(w+h<30)aliKali[0][index]->mask32[a].dlt0=3;
                    aliKali[0][index]->mask32[a].dlt1=1;if((w+h)<20)aliKali[0][index]->mask32[a].dlt1=0;
                    aliKali[0][index]->mask32[a].dlt2=(w+h)/20;  
                }   
                aliKali[0][index]->reloadMask();  //return;
                */
                //if(aliKali[0][index]->selfCorrelation!=0)continue;
                
                //if(forkCount<6);
                
                if(forkCount<inputData.num_cores&&index<aliKali->letterCount()){
                    //ищем свободный слот
                    for(int n=0;n<inputData.num_cores;n++){
                        if(pidID[n]==0){fIndex=n;letterID[n]=index;break;}
                    }    
                    forkCount++;
                    //cout<<"fIndex="<<fIndex<<" forkCount="<<forkCount<<" index="<<index<<endl;
                    //pipe(pfds[fIndex]);                    
                    
                    pidID[fIndex]= fork(); 
                    if (pidID[fIndex] < 0)
                        error((char*)"ERROR on fork");
                    if (pidID[fIndex] == 0)  {
                        GLetter *letter=aliKali->getLetter(index);
                        //#imageNormalisation((void*)&letter);
                        //rebuildMaskInLetter(letter,pfds[fIndex]);
                        aliKali->saveLetter(letter);
                        exit(0);
                    }
                    index++;
                    continue;
                }
            if(!forkCount)break;
            cout<<"start wait";
            pid=wait(&status);   cout<<" pid="<<pid<<endl;
            
            for(int n=0;n<inputData.num_cores;n++){
                if(pid==pidID[n]){
                    pidID[n]=0;  //освобождаем слот
                    forkCount--;
                    break;
                }
                
            }              
            
            //if(aliKali[0][index]->selfCorrelation<97&&aliKali[0][index]->codeSpace>95){   //если нужно оптимизируем букву еще раз
            //    rebuildMaskInLetter(index); 
            //}
            
            if(step==1000){
                step=0;
            }step++;
            
            //break;
        }   
#else
    index=0;
    cout<<"rebuild mask in base"<<endl;
    inputData.data["action"]="set focal points";
    
    //while(index<14978){
    while(index<aliKali->letterCount()){    
            GLetter *letter=aliKali->getLetter(index);
            //if(letter->mask32Count()>0){index++;letter->destroy();continue;}
            //if(letter->onOffRate>50){index++;letter->destroy();continue;}
            //if(aliKali[0][index]->OCRStatus==100){index++;continue;}
            if(letter->name=="*"){index++;letter->destroy();continue;}
            //if(aliKali[0][index]->name.find("*")!=-1){index++;continue;}
            if(letter->OCRIndex==3){index++;letter->destroy();continue;}
            //#imageNormalisation((void*)&letter);
            //rebuildMaskInLetter(letter,&pid);
            cout<<"@@@"<<index<<" letter="<<letter->name<<endl;
            aliKali->saveLetter(letter);
            letter->destroy();
            index++;
    }
    
#endif
    cout<<"done rebuild mask in base"<<endl;
    
    
}


///функция проверяет букву и оптимизирует расстановку масок. Убирает дубликаты, находит оптимальную позицию

void GFontEditor::rebuildMaskInLetterVector(GLetter *letter,int* pfds){
    vector<OCRMatch>letterLine;
    letter->setLetterDimensions();
    letter->eraseAllMask();
    aliKali->setFocalLineInLetter((uint)letter->letterIndex);
    
    for(int n=0;n<letter->fPointCount();n++){
        GBitMask32 mask;
        mask.xMask=letter->fPoint[n].x-16;
        mask.yMask=letter->fPoint[n].y-16;
        mask.imgW=24;
        mask.mH=24;
        mask.maskType=1;
        mask.dlt0=8;
        mask.dlt1=1;
        mask.dlt2=2;
        mask.status=1;
        letter->push_back(mask);
        letter->reloadMask(n);
    }
    for(int n=0;n<letter->focalLine.size();n++){
        GBitMask32 mask;
        mask.xMask=letter->focalLine[n].centerM.x-16;
        mask.yMask=letter->focalLine[n].centerM.y-16;
        mask.imgW=24;
        mask.mH=24;
        mask.maskType=1;
        mask.dlt0=8;
        mask.dlt1=1;
        mask.dlt2=2;
        mask.status=1;
        letter->push_back(mask);
    }
    letter->cropLetter();
    eraseNoCorrelationMask(letter);

    if(letter->onOffRate<85){
        GBitmap *outBitmap=letter->drawLetterPict(0);
        //локализуем области не закрытые масками
        //для этого закрашиваем области уже покрытые масками
        uchar *p=outBitmap->bytes_data;
        for(int x=0;x<outBitmap->data_size;x++){
            if(*p==0)*p=255;
            p++;
        }
        outBitmap->gaussian(3);
        outBitmap->binarisation(80, 24);
        //определяем фокальные точеки в незакрашенных областях.
        //В GBitsetMatrix все необходимые операции выполняются при создании матрицы
        GBitsetMatrix *matrix=GBitsetMatrix::createRegion(outBitmap,
                                                 0,
                                                 0,
                                                 outBitmap->columns(),
                                                 outBitmap->rows(),
                                                 1,
                                                 1,
                                                 IMGNOFLIP);
        
        int dltX=outBitmap->columns()/2+matrix->borderSize();
        int dltY=outBitmap->rows()/2+matrix->borderSize();
        
        for(int n=0;n<matrix->focalPoint.size();n++){
            //cout<<"x="<<matrix->focalPoint[n].x<<endl;
            
            GBitMask32 mask;
            mask.xMask=matrix->focalPoint[n].x-16-dltX;
            mask.yMask=matrix->focalPoint[n].y-16-dltY;
            mask.imgW=24;
            mask.mH=24;
            mask.maskType=1;
            mask.dlt0=8;
            mask.dlt1=1;
            mask.dlt2=2;
            mask.status=1;
            letter->push_back(mask);
            
        }
        letter->cropLetter();
        letter->reloadMask();
        eraseNoCorrelationMask(letter);
        matrix->destroy();
        //((GImageEditor*)inputData.imageEditor)->WriteImageData(outBitmap, "/_Image2OCR/__1.jpg", 0);
    }
    setLetterMainMask(letter);
   
    
    /////  поиск главного признака в пределах буквы  /////
    //
}

///функция проверяет букву и оптимизирует расстановку масок. Убирает дубликаты, находит оптимальную позицию

void GFontEditor::rebuildMaskInLetter(GLetter *letter,int* pfds){

    int print=1;
    vector<OCRMatch>letterLine;
    int w,h;
    letter->setLetterDimensions();
    
 	/*
	 for(int a=0; a<aliKali[0][index]->mask32Count();a++){
	 w=aliKali[0][index]->mask32[a].imgW;
	 h=aliKali[0][index]->mask32[a].mH;
	 aliKali[0][index]->mask32[a].dlt0=(w+h)/3;
	 if(w+h<30)aliKali[0][index]->mask32[a].dlt0=3;
	 aliKali[0][index]->mask32[a].dlt1=1;if((w+h)<20)aliKali[0][index]->mask32[a].dlt1=0;
	 aliKali[0][index]->mask32[a].dlt2=(w+h)/20;  
	 }   
	 aliKali[0][index]->reloadMask();  //return;
	*/
    
    int x0,x1,y0,y1,dlt0,dlt1,dlt2,c1,c2;
    x0=0;
	x1=0;
	y0=0;
	y1=0;
	dlt0=28;
    dlt1=1;
    dlt2=3;
	w=24;  
	h=24; 
    int delta=3;
    
    /*
	 ;
	 letter->cropLetter();
	 letter->eraseDublicateMask();
	 letter->reloadMask();
	 /////  поиск главного признака в пределах буквы  /////
	 setLetterMainMask(index);  
	 eraseNoCorrelationMask(index); 
	 */
    
    //testLetter(index,0);  
    
    //DR("@/___letter "<<index<<" "<<letter->name.c_str()<<" selfCorrelation="<<letter->selfCorrelation<<" codeSpace="<<letter->codeSpace<<endl)
	//return;

    letter->eraseAllMask();   //обязательно обнуляем маски. Алгоритм оптимизации расставляет маски лучше чем исходные. 
    
    
    ///// устанавливаем первую маску /////
    GBitMask32 mask;
    letter->push_back(mask);
    //cout<<"letter->mask32Count()="<<letter->mask32Count()<<endl;
    //cout<<" letter->letterW="<<letter->letterW<<" letter->letterH="<<letter->letterH<<endl;
    int maxN=0;
    int maxIndX=0,maxIndY=0;
        for(x0=0;x0<letter->letterW/2;x0+=5){
            for(y0=0;y0<letter->letterH/2;y0+=5){
                DR2("x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl);
                letter->mask32[0].xMask=x0;
                letter->mask32[0].yMask=y0;
                letter->mask32[0].imgW=w;
                letter->mask32[0].mH=h;     
                letter->mask32[0].maskType=1;
                letter->mask32[0].dlt0=dlt0;
                letter->mask32[0].dlt1=dlt1;
                letter->mask32[0].dlt2=dlt2;
                letter->mask32[0].status=1;    
                letter->reloadMask(0);
                if(letter->mask32[0].NMask<100){   //если маска неустойчивая, выбираем наилучшую
                    letter->mask32[0].status=0;
                    if(letter->mask32[0].NMask>maxN){
                        maxN=letter->mask32[0].NMask;
                        maxIndX=x0; maxIndY=y0;
                    }
                }else{
                    //letter->mask32[0].printMask();
                    goto exitFor;    
                }                
            }}   
        exitFor:;
    if(letter->mask32[0].status==0){   //если маска не найдена
        letter->mask32[0].xMask=maxIndX;
        letter->mask32[0].yMask=maxIndY;
        letter->mask32[0].status=1;
        letter->reloadMask(0);
    }
    aliKali->saveLetter(letter);
    
    //if(aliKali[0][index]->newLetter)
    //aliKali[0][index]->drawPict();    
    GBitmap *img=aliKali->drawPict((int)letter->letterIndex);
    //WriteImageData(img,"/1_.jpg",0);
    
    c2=setCorrelation(img,letter,1,0);                        //проверяем самокорреляцию

    int count=0;
    int minCorrelation, minX0=0,minY0=0,maskId=0;
    
    int letterCodeSpace=letter->codeSpace;  
    
    //int selfCorrelation=letter->selfCorrelation;
    int letterLineCount,letterLineCountNew,letterLineCountMin,letterMaskArea;
	int flag=0;
    int shape=0;
    letterMaskArea=0;
    uint startIndex=0;
    
    int x0_1=0,x0_2=0;
    int y0_1=0,y0_2=0;
    int step_1=0,step_2=0;
    
    while(1){
        DR("testLetter count="<<count<<" startIndex="<<startIndex<<endl)
        
        testLetter(letter,letterLine,1,&startIndex);
        
        count++; //testMaskCount++;
        
        letterLineCount=(uint)letterLine.size(); //запоминаем сколько было букв похожих на проверяемую
        
        if(count>25||letter->codeSpace==0||letter->mask32Count()>35)break;
        
        //находим ближайшую по кодовому расстоянию букву
        int max=0, maxIndex=-1;
        for(int a=0;a<letterLine.size();a++){
            if(letterLine[a].correlation>max){
                max=letterLine[a].correlation; maxIndex=a;
            }    
            
        } if(maxIndex==-1)break;
        
        DR("maxIndex="<<maxIndex<<" name="<<letterLine[maxIndex].name<<" letterLineCount="<<letterLineCount<<endl)
        
        //GLetter* letterTest=letterLine[maxIndex];
        //находим какая часть тестовой буквы наиболее влияет на кодовое расстояние с этой буквой
        
        //GBitmap *img128=letterTest->mask128.unPackMask();
        //if(letterLine[maxIndex]->newLetter)letterLine[maxIndex]->drawPict();  
        GBitmap *img128=aliKali->drawPict(letterLine[maxIndex].letterIndex);
        //WriteImageData(img128, "/1jpg", 0);        
        minCorrelation=100; letterCodeSpace=letter->codeSpace;
        
        DR(endl<<endl<<"add "<<letterLine[maxIndex].name<<" letter->mask32Count()="<<letter->mask32Count()<<" maskId="<<maskId<<" letterCodeSpace="<<letterCodeSpace<<
           " letterLine.size()="<<letterLine.size()<<" letter->letterH/2="<<letter->letterH/2<<endl);
        
        letterLineCountMin=letterLineCount;
        GBitMask32 maskNew;                                 //добавляем новую маску
        letter->push_back(mask);
        maskId=(int)letter->mask32Count()-1;
        GBitMask32 *mask=&letter->mask32[maskId];
        
        //начинаем поиск места для новой маски. Обходим букву по спирали начиная с центра
        
        while(y0_1>-(letter->letterH/2+20)&&y0_2<letter->letterH){
            if(step_2==1){step_2=0; y0=y0_1; 
            }else{y0=y0_2; step_2++;} x0_1=0; x0_2=0;
            				y0_1-=5,y0_2+=5;
            while(x0_1>-(letter->letterW/2+30)&&x0_2<letter->letterW-10){
                if(step_1==1){ step_1=0; x0=x0_1;
                }else{x0=x0_2;  step_1++;}
                x0_1-=5,x0_2+=5; 
                mask->xMask=x0;
                mask->yMask=y0;
                mask->imgW=w;
                mask->mH=h;     
                mask->maskType=1;
                mask->dlt0=dlt0;
                mask->dlt1=dlt1;
                mask->dlt2=dlt2;
                mask->status=1;  
                letter->reloadMask(maskId);                 
                DR("x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<" NMask"<<mask->NMask<<endl) 
                if(mask->NMask<100)continue;  //erase empty mask  
                //DR("letter->mask32["<<maskId<<"].NMask="<<mask->NMask<<" letter->mask32["<<maskId<<"].yMask="<<mask->yMask<<" status="<<mask->status<<END)
                //mask->printMask();
                
                c1=setCorrelation(img128,letter,1,0);                      //проверяем кодовое расстояние при этой маске   
                c2=setCorrelation(img,letter,1,0);                        //проверяем самокорреляцию
                vector<OCRMatch>letterLineNew;                            DR("c1="<<c1<<" c2="<<c2<<endl)
                testLetterInLine(letter,letterLine,letterLineNew);
                letterLineCountNew=(uint)letterLineNew.size();            DR("letterLineCountNew="<<letterLineCountNew<<endl)
                //if(shape==7&&x0==-5&&y0==-40)print=2;
                DR("/s"<<shape<<"@___correlation="<<c1<<" new selfCorrelation="<<c2<<" letterLineCountNew="<<
					letterLineCountNew<<" letterLineCountMin="<<letterLineCountMin<<" x0="<<x0<<" y0="<<y0<<" minX0="<<minX0<<" minY0="<<minY0<<" letter->onOffRate="<<letter->onOffRate<<" letterMaskArea="<<letterMaskArea<<endl);
                //if(shape==7&&x0==-5&&y0==-40)letter->mask32[letter->mask32Count()-1].printMask();
                //if(shape==7&&x0==-5&&y0==-40)setCorrelation(img,letter,1,1); //print=1;
                if(c2<95)continue;
                flag=0;
                
                if((c1<minCorrelation&&letterLineCountNew<=letterLineCountMin)||letter->onOffRate>letterMaskArea+delta){
                    //проверяем не совпадает ли маска с предыдущими
                    for(int a=0; a<letter->mask32Count()-1;a++){
                        if(letter->mask32[a].xMask==x0&&
                           letter->mask32[a].yMask==y0&&
                           letter->mask32[a].mH==h){ 
                            flag=1;
                            break;
                        }
                    }  
					if(!flag){ //если маска не повторяет координаты предыдущих
						minCorrelation=c1;
						letterLineCountMin=letterLineCountNew;
						minX0=mask->xMask;
						minY0=mask->yMask;
						DR("set mask new minX0="<<minX0<<" new minY0="<<minY0<<endl)
                        if(letter->onOffRate>letterMaskArea+delta)goto setMask;
						if(c1==0){
							//printImg=1;
							//c1=setCorrelation(img128,letter,1);
							//DT("/@___correlation="<<c1<<" maskId="<<maskId<<endl);
							//mask.printMask();
							//printImg=0;
							goto setMask;
						}
					}   
                }    
				
				
            }
        }    
        setMask:
        DR("minX0="<<minX0<<" minY0="<<minY0<<" minCorrelation="<<minCorrelation<<" letterCodeSpace="<<letterCodeSpace<<" letterLineCountMin"<<
			letterLineCountMin<<" letterLineCount="<<letterLineCount<<" flag="<<flag<<" w="<<w<<" h="<<h<<" letterMaskArea="<<letterMaskArea<<endl);
		
		if(print){
			letter->mask32[maskId].xMask=minX0;
			letter->mask32[maskId].yMask=minY0;
			letter->reloadMask(maskId);
			letter->mask32[maskId].printMask();
		}
        if(minCorrelation<letterCodeSpace||letterLineCountMin<letterLineCount||letter->onOffRate>letterMaskArea+delta){
			
            mask->xMask=minX0;
            mask->yMask=minY0;
            //letter->cropLetter();
            mask->status=1;
            DR(" letter->mask32Count()="<<letter->mask32Count()<<endl)
            letter->reloadMask(maskId);
             if(letter->onOffRate>letterMaskArea)letterMaskArea=letter->onOffRate;
            
            DR("shape="<<shape<<" letterCodeSpace="<<letterCodeSpace<<" new codeSpace="<<minCorrelation<<" codeSpace="<<letter->codeSpace<<
				" new selfCorrelation="<<letter->selfCorrelation<<" count="<<count<<" letter->mask32Count()="<<letter->mask32Count()<<endl);
            
            letter->codeSpace=minCorrelation;
            letterCodeSpace=minCorrelation;
			x0=0;
			x1=0;
			y0=0;
			y1=0;
			dlt0=28;
			w=24;  
			h=24;
            shape=0;
        }else{
            DR(" не удалось поставить различающую маску "<<shape<<" letter->mask32Count()="<<letter->mask32Count()-1<<endl)
			DR("minCorrelation="<<minCorrelation<<" letterCodeSpace="<<letterCodeSpace<<" letterLineCountMin="<<letterLineCountMin<<" letterLineCount="<<letterLineCount<<endl)
			//не удалось поставить различающую маску
			//пробуем поставить маску другой формы
            if(shape==7){ letter->pop_backMask(); break; }
            if(shape==6){
                dlt0=100;
                dlt1=1;
                dlt2=3;
                w=32;  
                h=85;
                shape++;
			}
            if(shape==5){
                dlt0=5;
                dlt1=1;
                dlt2=1;
                w=17;  
                h=16;
                shape++;
			}
            if(shape==4){
                dlt0=8;
                dlt1=1;
                dlt2=2;
                w=24;  
                h=24;
                shape++;
			}
            if(shape==3){
                dlt0=100;
                dlt1=1;
                dlt2=3;
                w=24;  
                h=64;
                shape++;
			}
			if(shape==2){
                dlt0=100;
                dlt1=1;
                dlt2=3;
                w=32;  
                h=24;
                shape++;
			}
            if(shape==1){
                dlt0=100;
                dlt1=1;
                dlt2=3;
                w=24;  
                h=48;
                shape++;
			}
			if(shape==0){
                dlt0=100;
                dlt1=1;
                dlt2=3;
                w=24;  
                h=32;
                shape++;
			}
            letter->pop_backMask();
        }
        //img128->destroy();
        
    }
    //img->destroy();
    //оптимизируем расстановку масок по устойчивости к искажениям буквы при печати
	//letterMaskOptimisation(index);
    DR("crop")
    letter->cropLetter();
    letter->reloadMask();
    /////  поиск главного признака в пределах буквы  /////
    DR("main")
    setLetterMainMask(letter);
    //eraseNoCorrelationMask(index); 
    
    testLetter(letter,0);
    
    DR("@/___letter  "<<letter->name.c_str()<<" selfCorrelation="<<letter->selfCorrelation<<" codeSpace="<<letter->codeSpace<<" maskCount="<<letter->mask32Count()<<" letter->onOffRate="<<letter->onOffRate<<endl)
    for(int t=0;t<letter->mask32Count();t++){
    
        cout<<"m="<<t<<" x="<<letter->mask32[t].xMask<<" imgW="<<letter->mask32[t].imgW<<endl;
    }
    
    //exit(0);
    return;
    
    
}//________________________________________________________________________________________________ 

void  GFontEditor::letterMaskOptimisation(GLetter *letter){
    int print=0;
    GBitmap *img=letter->mask128.unPackMask();
    //Типичный способ получения полиграфического растискивания двумя запусками dotGain
    //убираем со всех сторон по пикселю, сьедаем мелкие детали, которые не надо растискивать
    img->dotGain(-1,-1,-1,-1, 0);
    //добавляем со всех сторон символа по нескольку пикселей
    img->dotGain( 2, 2, 2, 2, 0);
    
    //WriteImageData(img,"_2.jpg",1); //exit(0);###
    GBitsetMatrix* setMatrix=GBitsetMatrix::createRegion(img,
                                                         0,
                                                         0,
                                                         img->columns(),
                                                         img->rows(),
                                                         1,1,
                                                         IMGNOFLIP);
    
    //str_="/_3.jpg";
    //WriteImageData(setMatrix->drawDataRGB,str_,1);
    GLetter *glyph=aliKali->getLetter(letter->letterIndex);

    
    for(int m=0;m<letter->mask32Count();m++){
        //glyph->mask32[0].printMask();
        
        int mC=0;
        int mCMax=0;
        int mCMaxNcount=0;
        int mCNcount=0;
        int xStart=0;
        int yStart=0;  
        int x,y;
        int xMax=glyph->mask32[0].xMask+3;
        int yMax=glyph->mask32[0].yMask+3;
        int xMin=glyph->mask32[0].xMask-2;
        int yMin=glyph->mask32[0].yMask-2;
        //cout<<"xMin="<<xMin<<" yMin="<<yMin<<endl;
        
        for(x=xMin;x<xMax;x+=2){
            for(y=yMin;y<yMax;y+=2){
                glyph->mask32[0].xMask=x; glyph->mask32[0].yMask=y;  glyph->mask32[0].status=1;
                glyph->reloadMask();
                if(glyph->mask32[0].NMask<10)continue;
                if(!glyph->mask32Count())continue;
                mC=setMatrix->letterToLetterCorrelation(glyph,
                                                        0,
                                                        img->rows(),
                                                        0,
                                                        0);
                //str_="/_31.jpg";
                //WriteImageData(setMatrix->drawDataRGB,str_,1); exit(0);
                
                mCNcount=mC+glyph->mask32[0].NMask/10;  //учитываем эффективную площадь маски
                //glyph->mask32[0].printMask();
                
                if(mCNcount>mCMaxNcount){
                    mCMax=mC;
                    mCMaxNcount=mCNcount;
                    xStart=x; yStart=y;
                }
                
                DT("correlation="<<mC<<" mCMax="<<mCMax<<" x="<<x<<" y="<<y<<" xStart="<<xStart<<" yStart="<<yStart
                   <<" NMask="<<glyph->mask32[0].NMask<<" mCNcount="<<mCNcount<<" mCMaxNcount="<<mCMaxNcount<<endl);
                
            }    
        }
        
        DT("/@@/______"<<m<<"_correlation="<<mC<<" mCMax="<<mCMax<<" x="<<xStart<<" y="<<yStart<<" NMask="<<glyph->mask32[0].NMask<<" mCNcount="<<mCNcount<<
           " mCMaxNcount="<<mCMaxNcount<<endl);
        letter->mask32[m].xMask=xStart-letter->dX;
        letter->mask32[m].yMask=yStart-letter->dY;
        letter->mask32[m].correlation=mCMax;
        
    }  
    glyph->destroy(); 
    setMatrix->destroy();
    img->destroy();
    DT("/@1@/______x="<<letter->mask32[0].xMask<<" y="<<letter->mask32[0].yMask<<endl);
    letter->reloadMask();
    //aliKali[0][index]->mask32[0].printMask();  
    cout<<"print DONE "<<endl;
}

///сравнивает букву со всеми буквами в базе.

void GFontEditor::setMaskFromBase(){
    int print=1;
    if(inputData.data["ocrData"]!="setBase")setBase();
    inputData.data["ocrData"]="setMask";
    int letterIndex=aliKali->testLetter;
    DT("letterIndex="<<letterIndex<<endl);

    GLetter *letter;
    vector<maskData_>letterDataLine;
    vector<int>letterIn; letterIn.resize(20000);
    //GBitmap *img128_=img128_=aliKali->drawPict(letterIndex);

    
    for(uint index=0;index<aliKali->size();index++){
    //for(uint index=0;index<85;index++){
        //if(index==letterIndex)continue;
        letter=aliKali->getLetter(letterIndex);
        GLetter* glyph=aliKali->getLetter(index);
        GBitmap *pict=glyph->drawPict();
        GBitsetMatrix* setMatrix=GBitsetMatrix::createRegion(pict,
                                                             0,
                                                             0,
                                                             pict->columns(),
                                                             pict->rows(),
                                                             1,1,
                                                             IMGNOFLIP);

        
        int correlation=setMatrix->letterToLetterCorrelation(letter,
                                                             0,
                                                             pict->rows(),
                                                             1,
                                                             0);
        
        cout<<index<<" correlation="<<correlation<<" count="<<letter[0].mask32Count()<<endl;
        //сохраняем результаты распознавания буквы в
        
        if(correlation>0){
            for(int a=0;a<letter[0].mask32Count();a++){
                if(letter[0].mask32[a].correlation<70)continue;
                maskData_ data;
                data.correlation=letter[0].mask32[a].correlation;
                data.id=a;
                data.letterIndex=index;
                letterDataLine.push_back(data);
                letterIn[index]++;
            }
        }
        setMatrix->destroy();
        letter->destroy();
        pict->destroy();
        glyph->destroy();
        
        //imageEditor->WriteImageData(pict,"/_1.tif",1);

    }
    cout<<"letterDataLine.size()="<<letterDataLine.size()<<endl;
    string dataStr; dataStr.resize(letterDataLine.size()*sizeof(maskData_));
    memcpy(&dataStr[0], &letterDataLine[0], letterDataLine.size()*sizeof(maskData_));
    writeText(dataStr, "/1.txt");
    int count=0;
    for(int a=0;a<letterIn.size();a++){
        if(letterIn[a]>0)count++;
    
    }
    cout<<" count="<<count<<endl;
    exit(0);
    
    
    /*
    
    
    for(int index=0;index<aliKali->letterCount();index++){  
        
        if(index==letterIndex)continue;
        //if(index==22)break;
        //    int s=1;
        //}
        //  GLetter *letter_=aliKali->getLetter(index);
        //if(letter_->mask128.mW>outBitmap->columns()*0.75){letter_->destroy();continue;}
        //cout<<"c2";
        GLetter *glyph=GLetter::create();
        
        for(int a=0;a<letter->mask32Count();a++){
            DT("aliKali[0]["<<index<<"].mask32["<<a<<"].status="<<letter->mask32[a].status)
            //if(aliKali[0][index]->mask32[a].status!=3)continue;   //проверяем только новые маски
            glyph=aliKali->getLetter(index);
            glyph->mask32[0]=letter->mask32[a];
            
            
            
            //DT("correlation="<<correlation<<" "<<letter->name.c_str()<<endl);
            //if(correlation<60)continue;
            
            ostringstream out;
            
            out<<"<glyph id=\""<<letter->idNumber<<"\" n=\""<<letter->name.c_str()<<"\" a=\"1\"/>";
            
            DT("@mask1 m="<<a<<" xMask="<<glyph->mask32[0].xMask<<" yMask="<<glyph->mask32[0].yMask<<endl);
            
            //int x0=glyph->mask32[0].xMask;
            //int x1=x0+glyph->mask32[0].imgW;
            //int y0=glyph->mask32[0].yMask;
            //int y1=y0+glyph->mask32[0].mH;
            //int dlt0=glyph->mask32[0].dlt0;
            
            
            DT(out.str());
            
            inputData.data["setBase"]=out.str();
            setBase();
            
            
            
        }
       // if(glyphMain->mask32Count()>50){
       //     glyph->destroy();
       //     break;
       // }
       // glyph->destroy();
        
        
        //WriteImageData(img,"/_1.jpg",1); //exit(0);
        //string str="/_RGB.jpg";
        //WriteImageData(setMatrix->drawDataRGB,str,0);
        
        
    }
    // for(int a=0;a<aliKali[0][letterIndex]->mask32Count();a++){
    //     aliKali[0][letterIndex]->mask32[a].status=1;
    // }
    //outBitmap->destroy();
    */ 
    
};


/////  поиск главного признака в пределах буквы  /////
void GFontEditor::setLetterMainMask(GLetter *letter){
	int xp,yp;  // координаты центра проверяемого признака, на предмет главного
	int r;  // расстояние между центром габаритного прямоугольника и проверяемого признака
	int n_key=0, r_key=(1 << 16);  // (1 << 16) два в 16 степени
	
	// главным признаком назначается тот центр которого находится ближе всего к центру габаритного прямоугольника
    // и обладающий устойчивой корреляцией
	// координаты центра габаритного прямоугольника 0,0,0,0
    GBitmap *pict=letter->drawPict();

    setCorrelation(pict,letter,1,0);
	
	// перебор признаков буквы
	for(int n=0; n<letter->mask32Count(); n++){
		// пропускаем заремаченные проверяемые признаки
		if(letter->mask32[n].status!=1||letter->mask32[n].correlation<97)continue;
		
		// координаты центра проверяемого признака
		xp=letter->mask32[n].xMask + (letter->mask32[n].imgW)/2;
		yp=letter->mask32[n].yMask + (letter->mask32[n].mH)/2;
		
		r=xp*xp + yp*yp;  // по теореме Пифагора вычисляем расстояние между центрами
		if ( r < r_key ) { r_key = r; n_key=n; }  // выбираем признак с наименьшим расстоянием r                            
		
		// cout<<r<<" r"<<endl;  cout<<r_key<<" r_key"<<endl;  cout<<n_key<<" n_key"<<endl;    
	} // n
	if(n_key<letter->mask32Count()){
		// замена признака на найденный главный признак
		GBitMask32 mask=letter->mask32[0];
		letter->mask32[0]=letter->mask32[n_key];
		letter->mask32[n_key]=mask;
	}
}; 

///// распознавание стековой буквы простыми буквами /////
void GFontEditor::letterOCR(GLetter *letter, string &name, int *correlation,int mode){
/*
    GBitmap *pict=letter->drawPict();
    if(!mode)((GImageEditor*)inputData.imageEditor)->WriteImageData(pict,"/_Image2OCR/edit/1.jpg",0); //exit(0);
    inputData.data["inputFile"]="/_Image2OCR/edit/1.jpg";
    if(!mode)logicProcessor->readDictionary();
    
    GBitsetMatrix *setMatrix=GBitsetMatrix::createRegion(pict,
                                          0,
                                          0,
                                          pict->columns(),
                                          pict->rows(),
                                          1,1,
                                          IMGNOFLIP
                                          );      
    vector<OCRMatch>matchLine;
        
    setMatrix->letterCorrelation(matchLine,
                                 aliKali,
                                 0,
                                 0, 
                                 pict->rows(),
                                 0,   //no scale mode
                                 3,   //OCRMode, 
                                 0);  //0 no print
#ifdef DEBUGLVL_DRAW         
    ((GImageEditor*)inputData.imageEditor)->WriteImageDataRGB(setMatrix->drawDataRGB,"/1Draw.jpg",0); 
#endif
    
    
    setMatrix->destroy();
    if(!mode){
        //((GStr<int>*)inputData.pref)->put(0,0);
        //((GStr<int>*)inputData.pref)->put(1,pict->rows());
        //((GStr<int>*)inputData.pref)->put(2,pict->columns());
        //((GStr<int>*)inputData.pref)->put(3,pict->rows());
    }    
    string strW;
    vector<OCRMatch>line;
    string mainString;
    vector<stringOCR>strArray;
    stringOCR strLine;  strLine.y0=0; strLine.y1=pict->rows();
    strLine.y0=0; strLine.y1=pict->rows();
    
    strLine.LimY0=pict->rows()/2+letter->y0;
    strLine.LimY1=pict->rows()/2+letter->y1;
    
    strArray.push_back(strLine);
    logicProcessor->letterAssociation(strArray,
                                      matchLine,
                                      line,
                                      pict,
                                      mainString,
                                      pict->columns(),
                                      0,
                                      2);
    *correlation=0;
    for(int i=0;i<line.size();i++){
        line[i].name=line[i].nameUni;
        if(line[i].correlation>*correlation){
            *correlation=line[i].correlation;
            name=line[i].nameUni;
        }
    }
    if(!mode)logicProcessor->drawGrapeLine(line);    
*/
}; 

///// распознавание стековой буквы простыми буквами /////
void GFontEditor::letterBaseOCR(int startIndex){
    //int print=1;
    logicProcessor->readDictionary();
    
    vector<string> OCRLog;
    string path=inputData.data["grammar"]+"/OCRLog.txt";
    readText(OCRLog,path);
    
    
    int count=0;
    for(int i=startIndex;i<aliKali->size();i++){  cout<<".";
    //for(int letterIndex=0;letterIndex<OCRLog.size();letterIndex++){
    
        //int i=atoi(OCRLog[letterIndex].c_str());
        
        GLetter *letter=aliKali->getLetter(i);
        string name=letter->name;   //cout<<" i="<<i<<" name="<<name<<endl;
        //if(name.find("ད")==-1&&name.find("ང")==-1&&name.find("ྡ")==-1&&name.find("ྔ")==-1){letter->destroy(); continue;}
        //imageEditor->imageNormalisation((void*)&letter);  
        
        int index=(int)logicProcessor->fontGMap->getKey(name, FULL_MATCH);
        if(index==-1){
            continue;
            //DR("NO LETTER"<<name<<endl)
        }else{
            TString strT; logicProcessor->fontTable->getTStr(index,strT);
            //DR("name="<<strT[8]<<" ln = "<<strT[1]<<" key="<<strT[4]<<endl)
            string OCRIndex=strT[4];
            if(strT[1]!="tib"||OCRIndex.size()==1){ continue; }
        }    
        
        name=""; 
        int correlation=0;
        
        letterOCR(letter,name, &correlation,1);
        letter->destroy();
        
        if(name==letter->name&&correlation>90){
            //cout<<"+"<<i;//<<"  name="<<letter->name<<" nameOCR="<<name<<" c="<<correlation<<endl;
        }else{
            cout<<i<<"@@@  name="<<letter->name<<" nameOCR="<<name<<" c="<<correlation<<endl;
            count++;
            //if(count>10)exit(0);
        } 
        
    }    
    exit(0);
}; 

//Проверка на влияние маски в букве на корреляцию. Стираем маски которые не влияют на корреляцию и количество похожих букв
void GFontEditor::eraseNoCorrelationMask(GLetter* letter){
    int print=0;
    uint startIndex=0;
    vector<OCRMatch>letterLine;
    
    for(int a=1; a<letter->mask32Count();a++)letter->mask32[a].status=1;
    
    startIndex=0;
    //testLetter(letter,letterLine,0,&startIndex);
    
    GBitmap *outBitmap=letter->drawLetterPict(0);
    //outBitmap->printToScreen();
    outBitmap->destroy();
    int onOffRate=letter->onOffRate;
    //cout<<"onOffRate="<<letter->onOffRate<<endl;
    
    
    for(int a=0; a<letter->mask32Count();a++){
        letter->mask32[a].status=0;
        GBitmap *outBitmap=letter->drawLetterPict(0);
        outBitmap->destroy();
        //cout<<"//____onOffRate="<<letter->onOffRate<<endl;
        if(letter->onOffRate<onOffRate-1)letter->mask32[a].status=1;  //восстанавливаем маску в правах
    }
    letter->eraseEmptyMask();
    letter->reloadMask();
    
    DT("new mask32Count="<<letter->mask32Count()<<endl);
};



