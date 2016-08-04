#include "GFontEditor.h"
#include <Cocoa/Cocoa.h>

/** This function build letter base from text file of alphabet
*/ 
void GFontEditor::buildGFontFromFont(){
	
	//string data, path=inputData.data["root"]+"/OCRData/Grammar/AliKali_Kannada.txt";
    string data, path=inputData.data["root"]+"/OCRData/Grammar/__SANSKRIT_ALPHABET_SHORT.txt";
    
    readText(data,path);
	if(!data.size()){cout_<<"no data"; return;}
    //cout_<<"data="<<data<<endl;
    data=str_replace("\r","", data);
	vector<string>fontData=explode("\n",data);
	//string fontName="Kedage";
    string fontName="Siddhanta";
	cout<<"font="<<fontName;

#ifdef COCOA

	short index;
	//short wScale=445;
	short wScale=900;
	
	cout <<"Start font table building. wScale="<<wScale<<endl;
	//if(wScale<90)wScale=90;
	unsigned char *p1;
	int y0=0, y1=0,y0Main=0,y1Main=0;  //text line coordinate
	NSImage *stringOCRImage;
	fontName=str_replace("\r","", fontName); 
	NSString *fontNameStr=[[NSString alloc] initWithUTF8String:fontName.c_str()];
    //NSLog(fontNameStr);
	NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
	[attrs setObject:[NSColor blackColor] forKey: NSForegroundColorAttributeName];
    NSFont *font=[NSFont fontWithName:fontNameStr size:wScale/11];
	[attrs setObject:font forKey: NSFontAttributeName];
    //Сначала определяем габариты строки. Для этого создаем тестовую строку
	stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale*4, wScale)];
    fontData[0]="";
	string textLine;
	//textLine="བབྱང་ཆུབ་སེམས་པ";
	//textLine="ಕನ್ನಡ-ಗ್ರೀಟಿಂಗ್ಸ್ಕಾಂಕನ್ನಡ-ಗ್ರೀಟಿಂ";
    textLine="फ़लविशेषोऽपिहेतुविशेषाक्षिप्तवात्तस्य";
		
	[stringOCRImage lockFocus];
		[[NSColor whiteColor] set];
		[NSBezierPath fillRect:NSMakeRect(0, 0, wScale*4,wScale)];
		[[NSColor blackColor] set];
		NSString *str=[[NSString alloc] initWithUTF8String:textLine.c_str()];
		[str  drawAtPoint: NSMakePoint(wScale/3,wScale/4) withAttributes: attrs];	
	[stringOCRImage unlockFocus];
	
	NSBitmapImageRep *srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
	unsigned char *srcData = [srcImageRep bitmapData];
	
	GBitmap *textLineImg=GBitmap::create(wScale*4,wScale); 
	for(int x = 0; x <wScale*4; x++) {
		for(int y = 1; y< wScale; y++){
			p1=srcData+(y)*wScale*16+x*4;
			textLineImg[0][y][x]=*p1;
			//if(bSet[0][y][x]==255){cout_<<"0";}else{cout_<<"1";}//draw on screen
		} //cout_<<endl;
	}
    
	[srcImageRep release];
	[stringOCRImage release];
	[str release];
	//WriteImageData(textLineImg,"/_1.jpg",0); exit(0);
	///set line coordinates in page	
    
   
    GBitset *setBit=GBitset::createResize(textLineImg,1,1,1);
	cout_<<"@4_2";
	vector<stringOCR>strArray; 
	setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ )
	//int border=setBit->border_size();
	setBit->destroy();
	if(strArray.size()){
		y0Main=strArray[0].y0;
		y1Main=strArray[0].y1;   
	}	
    if(y1Main-y0Main<0){cout_<<"no string"; return;}
	
	cout<<"@4_3 strArray.size()="<<strArray.size()<<" y1Main="<<y1Main<<" y0Main="<<y0Main<<endl;
	
    //затем для каждой буквы создаем тестовое изображение
    
	stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale, wScale)];
	
	//GBitset *set;
	
	for(index=0;index<fontData.size();index++){   ///// for every string we build OCR Table Record
		
			cout<<"index="<<index<<" fontData[index]="<<fontData[index]<<endl;
			if(fontData[index]=="")continue;
			[stringOCRImage lockFocus];
			
			[[NSColor whiteColor] set];
			[NSBezierPath fillRect:NSMakeRect(0, 0, wScale,wScale)];
			[[NSColor blackColor] set];
			str=[[NSString alloc] initWithUTF8String:fontData[index].c_str()];
			[str  drawAtPoint: NSMakePoint(wScale/4,wScale/4) withAttributes: attrs];
			
			[stringOCRImage unlockFocus];
			
			srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
			unsigned char *srcData = [srcImageRep bitmapData];
			
			GBitmap *bMap=GBitmap::create(wScale,wScale); 
			for(int x = 0; x <wScale; x++) {
				for(int y = 1; y< wScale; y++){
					p1=srcData+(y)*wScale*4+x*4;
					bMap[0][y][x]=*p1;
					//if(bSet[0][y][x]==255){cout_<<"0";}else{cout_<<"1";}//draw on screen
				} //cout_<<endl;
			}
		    int x0_,x1_,y0_,y1_;

        //bMap->dotGain(-1,-1,-1,-1, 0);
        //добавляем со всех сторон символа по нескольку пикселей
        //bMap->dotGain( 0, 1, 0, 1, 0);
        
            GBitmap *img128=bMap->detectRegion(0,&x0_,&x1_,&y0_,&y1_,1);
			//WriteImageData(bMap,"/_Image2OCR/_11.jpg",0); exit(0);
            //img128->dotGain( 1, 1, 1, 1, 0);
            if(img128->columns()>128){
                img128->destroy();
                bMap->destroy();
                cout<<"TOO WIDE LETTER "<<endl;
                continue;
            }
	
			//short letterIndex;
			//GBitsetContour *vSet=GBitsetContour::createResize(img128,1,1,0);
            GLetter *letter=GLetter::create();
            letter->name=fontData[index];
			//vSet->textLineSize=(y1-y0);
			letter->textLineSize=(y1Main-y0Main);
			
		/*	vectorBase[0].allVectorCount=0;
			vSet->setFontData(vectorBase,letter,fontData[index],wScale);
			cout_<<"vectorBase[0].allVectorCount="<<vectorBase[0].allVectorCount<<endl;

			letter->maskFromVector();
			cout_<<"m1";
			string draw=letter->drawToSVG();
			writeText(draw,"/1.svg");
			
			//WriteImageData(vSet->drawData, "/_112.jpg",0);
			//_//_DM("DONE1");
			
		*/
        
			cout<<"START_PACK img128.columns()="<<img128->columns()<<END;
			letter->mask128.packBitMask128(img128);  
             
            //cout_<<"y0="<<y0<<" y1="<<y1<<"y0_="<<y0_<<" y1_="<<y1_<<" letter->mask128.mH="<<letter->mask128.mH<<" letter->y0="<<letter->y0<<" letter->y1="<<letter->y1<<endl;

        
			//glyphNew.mask128.printMask();
			//exit(0);
			letter->mask128.xMask=-(img128->columns()/2);
			//cout<<"m1_1";
			//normalisation on slice size
			int s= img128->columns(); if(s>128)s=128;
			//int dl=letter->mask128.mW-s;
			letter->xMin=500;
			letter->xMax=0;
			/*for(int a=0;a<letter->mask32Count();a++){
				letter->mask32[a].xMask-=dl/2;
				if(letter->mask32[a].xMask<letter->xMin)letter->xMin=letter->mask32[a].xMask;
				if(letter->mask32[a].xMask+letter->mask32[a].imgW>letter->xMax)letter->xMax=letter->mask32[a].xMask+letter->mask32[a].imgW;
			}
            */ 
			//cout_<<" letter->xMin="<<letter->xMin<<endl;
			//letter->dX=-(letter->mask128.mW/2-letter->letterW/2-(letter->mask128.mW/2+letter->xMin));
			//letter->dY=-(letter->mask128.mH/2-letter->letterH/2-(letter->mask128.mH/2+letter->yMin));
			
			letter->mask128.yMask=-(letter->mask128.mH/2);
			letter->idNumber=letter->mask128.hashValue();
			//cout<<"m1_2";
			//letter->reloadMask();
		    
            y0=y0Main-y0_;
            y1=y1Main-y0_;
      
			letter->y0=-(letter->mask128.mH/2-y0);
			letter->y1=y1-letter->mask128.mH/2;
        
            letter->letterH=img128->rows();
            letter->letterW=img128->columns();
		
            //cout<<"y0="<<y0<<" y1="<<y1<<"y0_="<<y0_<<" y1_="<<y1_<<" letter->mask128.mH="<<letter->mask128.mH<<" letter->y0="<<letter->y0<<" letter->y1="<<letter->y1<<endl;
        
		    //cout<<"m1_3";
		    //cout_<<" allVectorCount in letter "<<set->vectorCount<<endl;
			//vSet->destroy();
			bMap->destroy();
			img128->destroy();
		    [str release];
		    [srcImageRep release];
			//cout<<"m2";
            //letter->setLetterDimensions();
        
            aliKali->push_back(letter);
            letter->destroy();
			//if(index==0){
				//NSData *TIFFData = [stringOCRImage TIFFRepresentation];   //write file on disk
				//[[stringOCRImage TIFFRepresentation] writeToFile:@"/_1111.tiff" atomically:YES];
			//}
			//cout_<<"main index="<<index<<endl;
	}//for(index=0;index<[f
	
	
	//set textLineSize in base
	/*aliKali->textLineSize=0;
    if(aliKali->letterCount()){
        for(int i=0;i<aliKali->letterCount();i++){
            GLetter *letter=aliKali->getLetter(i);
            aliKali->textLineSize+=letter->y1-letter->y0;
            letter->destroy();
        }aliKali->textLineSize/=aliKali->letterCount();
	}
	*/
	cout_<<"m3";
	
	//[pool release];
	
	cout_<<"m4";
#endif
	
	
}//__________________________________________________________________________

/** This function build letter base from text file of alphabet
*/ 
void GFontEditor::buildGFontFromRTF(){
	
	string data, path=inputData.data["OCRTables"]+"/AliKali.rtf";
    int mode=1; // -- Kannada alphabet in form (Baraha font):|:(Unicode)
    string RTFFont="BRHKannada";

#ifdef MACOS
#ifdef COCOA
    
    NSString *fileType;
	NSTextView *textHolder;
	NSString *pathSrc;
	NSAttributedString *fileContentString;
	int print=1;
    //NSAutoreleasePool *pool=[[NSAutoreleasePool alloc]init];
	
	pathSrc=[[NSString alloc] initWithUTF8String:path.c_str()];
	textHolder = [[NSTextView alloc] init];
	
	
	fileType=[pathSrc pathExtension];
	//cout_<<"mainFileIndex="<<fileIndex<<endl;
	
	if ([fileType isEqualToString:@"rtf"]||[fileType isEqualToString:@"RTF"]||[fileType isEqualToString:@"doc"]||[fileType isEqualToString:@"DOC"]){
		
		[textHolder readRTFDFromFile:pathSrc];
		fileContentString=[textHolder attributedSubstringFromRange:NSMakeRange(0,[[textHolder string] length])];
		DT("l1="<<[[fileContentString string]length]<<END);
	}
	
    NSAttributedString *line;
	
	NSString *fontName,*textString;
	int fileLength=(int)[fileContentString length];
	int letterIndex=0;
	int YagpoWylieFontFlag;
	
	string str, pathMap, tibFontName;
    string letter;
	
	textString=[fileContentString string];
	string uniStack;
	wstring wstr;
	//DT("fileLength="<<fileLength/1000<<" kb"<<END;) 
	int step=0,index=0; 
    short wScale=200;
	//ofstream c_out; c_out.open("/_out1.txt");
    
    int firstLineFlag=0,drawLineFlag=0;
    unsigned char* p1;
    int indexStart, indexEnd;
    int y0,y1;
    NSImage *stringOCRImage;
    
    
    if(firstLineFlag){
       
        BuildLine:;
            //Сначала определяем габариты строки. Для этого создаем тестовую строку
            stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale*4, wScale)];
            string textLine;
            
            
            
            [stringOCRImage lockFocus];
            [[NSColor whiteColor] set];
            [NSBezierPath fillRect:NSMakeRect(0, 0, wScale*4,wScale)];
            [[NSColor blackColor] set];
            [line  drawAtPoint: NSMakePoint(wScale/3,wScale/4)];	
            [stringOCRImage unlockFocus];
            
            
            
            NSBitmapImageRep *srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
            unsigned char *srcData = [srcImageRep bitmapData];
            
            GBitmap *textLineImg=GBitmap::create(wScale*4,wScale); 
            for(int x = 0; x <wScale*4; x++) {
                for(int y = 1; y< wScale; y++){
                    p1=srcData+(y)*wScale*16+x*4;
                    textLineImg[0][y][x]=*p1;
                    //if(bSet[0][y][x]==255){cout_<<"0";}else{cout_<<"1";}//draw on screen
                } //cout_<<endl;
            }
            [srcImageRep release];
            [stringOCRImage release];

            WriteImageData(textLineImg,"/_1.jpg",0); //exit(0);
            ///set line coordinates in page	
            
            GBitset *setBit=GBitset::createResize(textLineImg,1,1,1);
            cout<<"@4_2";
            vector<stringOCR>strArray; 
            setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ )
            cout<<"@4_3 strArray.size()="<<strArray.size()<<endl;
            //int border=setBit->border_size();
            setBit->destroy();
            if(strArray.size()){
                y0=strArray[0].y0;
                y1=strArray[0].y1;
            }
            textLineImg->destroy();
            if(y1-y0<0){cout_<<"no string"; return;}
        
       goto ContinueConvert;     
    }
    
    
    ///функция строит изображение буквы и заносит ее в базу


 if(drawLineFlag){    

    BuildLetter:;
        cout<<"index="<<index<<" uniStack="<<uniStack<<endl;
        stringOCRImage = [[NSImage alloc]initWithSize:NSMakeSize(wScale, wScale)];
        [stringOCRImage lockFocus];
        
        [[NSColor whiteColor] set];
        [NSBezierPath fillRect:NSMakeRect(0, 0, wScale,wScale)];
        [[NSColor blackColor] set];
        
        [line  drawAtPoint: NSMakePoint(wScale/3,wScale/4)];
        
        [stringOCRImage unlockFocus];
        
        NSBitmapImageRep* srcImageRep = [NSBitmapImageRep imageRepWithData:[stringOCRImage TIFFRepresentation]]; 
        unsigned char *srcData = [srcImageRep bitmapData];
        
        GBitmap *bMap=GBitmap::create(wScale,wScale); 
        for(int x = 0; x <wScale; x++) {
            for(int y = 1; y< wScale; y++){
                p1=srcData+(y)*wScale*4+x*4;
                bMap[0][y][x]=*p1;
                //if(bSet[0][y][x]==255){cout_<<"0";}else{cout_<<"1";}//draw on screen
            } //cout_<<endl;
        }
        int x0_,x1_,y0_,y1_;
        string letterName;
     
        GBitmap *img128=bMap->detectRegion(5,&x0_,&x1_,&y0_,&y1_,1);
         if(mode==1){
            letterName="/_LETTERS/"+substr((int)uniStack.find(":|:")+3,(int)uniStack.size(),uniStack)+".jpg";
            WriteImageData(img128,letterName,0);
             WriteImageData(bMap,"/-3.jpg",0);
            cout<<"letterName="<<letterName<<endl;
         }
        cout<<"y0="<<y0<<" y1="<<y1<<"y0_="<<y0_<<" y1_="<<y1_<<" bMap->rows()="<<bMap->rows()<<endl;
        int LetterH=(y1_-y0_);
        y1_=y1-y0_-LetterH/2;
        y0_=y0-y0_-LetterH/2;
        cout<<"LetterH="<<LetterH<<" y0="<<y0<<" y1="<<y1<<"y0_="<<y0_<<" y1_="<<y1_<<" bMap->rows()="<<bMap->rows()<<endl;
        //short letterIndex;			//letterIndex=GFont[GFont.size()-1]->contourIndex;
        //GBitsetContour *vSet=GBitsetContour::createResize(img128,1,1,0);
        GLetter *glyph=GLetter::create();
        glyph->name=substr((int)uniStack.find(":|:")+3,(int)uniStack.size(),uniStack);
        //vSet->textLineSize=(y1-y0);
        glyph->textLineSize=(y1-y0);
        
        //cout_<<"START_PACK img128.columns()="<<img128->columns()<<END;
        glyph->mask128.packBitMask128(img128);  
        //glyphNew.mask128.printMask();
        //exit(0);
        glyph->mask128.xMask=-(glyph->mask128.mW/2);
        //cout_<<"m1_1";
        //normalisation on slice size
        int s= img128->columns(); if(s>128)s=128;
        //int dl=letter->mask128.mW-s;
        glyph->xMin=500;
        glyph->xMax=0;
        /*for(int a=0;a<letter->mask32Count();a++){
         letter->mask32[a].xMask-=dl/2;
         if(letter->mask32[a].xMask<letter->xMin)letter->xMin=letter->mask32[a].xMask;
         if(letter->mask32[a].xMask+letter->mask32[a].imgW>letter->xMax)letter->xMax=letter->mask32[a].xMask+letter->mask32[a].imgW;
         }
         */ 
        //cout_<<" letter->xMin="<<letter->xMin<<endl;
        glyph->dX=-(glyph->mask128.mW/2-glyph->letterW/2-(glyph->mask128.mW/2+glyph->xMin));
        glyph->dY=-(glyph->mask128.mH/2-glyph->letterH/2-(glyph->mask128.mH/2+glyph->yMin));
        
        glyph->mask128.yMask=-(glyph->mask128.mH/2);
        glyph->idNumber=glyph->mask128.hashValue();
        //cout_<<"m1_2";
        
        glyph->y0=y0_;
        //glyph->y0=-(glyph->mask128.mH/2-glyph->y0);
        glyph->y1=y1_;
        glyph->letterH=LetterH;
        glyph->letterW=img128->columns();
        //glyph->y1-=glyph->mask128.mH/2;
     
        glyph->setLetterDimensions();
        //cout_<<"m1_3";
        //cout_<<" allVectorCount in letter "<<set->vectorCount<<endl;
        //vSet->destroy();
        bMap->destroy();
        img128->destroy();
        [srcImageRep release];
        [stringOCRImage release];
        aliKali->push_back(glyph);
        glyph->destroy();
     
     
        //cout_<<"m2";
        //if(index==0){	
        //NSData *TIFFData = [stringOCRImage TIFFRepresentation];   //write file on disk
        //[[stringOCRImage TIFFRepresentation] writeToFile:@"/_1111.tiff" atomically:YES];
        //}
        //cout_<<"aliKali->base["<<aliKali->letterCount<<"]="<<aliKali->base[aliKali->letterCount-1]->name<<endl;
        firstLineFlag=0;
        goto ContinueConvert;
 }
    
    firstLineFlag=1;
    drawLineFlag=1;

	while(letterIndex<fileLength){ 
		if(step==10000){cout_ <<index/1000<<". ";
            step=0;}step++;index++;

		fontName=[[fileContentString attribute:NSFontAttributeName  atIndex:letterIndex effectiveRange:nil]fontName] ;  //get new letter
		str=[fontName cStringUsingEncoding:11]; 
		letter=Unicode_to_UTF([textString  characterAtIndex:letterIndex]);
        
		//cout<<" letterIndex="<<letterIndex<<" letter=/"<<letter<<"/ fontName="<<str<<" code="<<(unsigned short)letter[0]<<" uniStack="<<uniStack<<endl;
		
		if(letter[0]==10||letter=="\n"){
			if(uniStack.size()!=0){
				//DR("YagpoWylieFontFlag"<<YagpoWylieFontFlag<<" uniStack="<<uniStack<<END)
				if(firstLineFlag){
                    indexEnd=letterIndex;
                    indexStart=0;
                    DR("uniStackOut_1="<<uniStack<<" indexStart="<<indexStart<<" indexEnd="<<indexEnd<<END)
                    line=[fileContentString attributedSubstringFromRange:NSMakeRange(0,indexEnd)];
                    firstLineFlag=0;
                    goto BuildLine;
                    
                }else{
                    indexEnd=letterIndex;
                    if(mode==1){
                        string wLetter=substr(0,(int)uniStack.find(":|:"),uniStack);
                        wstring wL=UTF_to_Unicode(wLetter);
                        indexEnd=indexStart+(int)wL.size();
                    }
                    DR("uniStackOut_2="<<uniStack<<" indexStart="<<indexStart<<" indexEnd="<<indexEnd<<" l="<<[fileContentString length]<<END)
                    line=[fileContentString attributedSubstringFromRange:NSMakeRange(indexStart,indexEnd-indexStart)];
                    indexEnd=letterIndex;
                    goto BuildLetter;    //все говорят что goto это плохо. Но в Cocoa функции по километру и требуют лист параметров. Пусть уж будет такой вызов.
                }
                 ContinueConvert:;  // нарисовали букву и продолжаем разбор строки
                 indexStart=indexEnd+1;
                 [line release];
                
                mainString+=uniStack;
				uniStack="";
			}
			mainString+=letter;
			letterIndex++;continue;
		}
		//DT("fontName="<<str<<END);
		if ( logicProcessor->fontNameMap.find(str) != logicProcessor->fontNameMap.end()) {
            DR(str <<" is tib font"<<END)
			//________________START_convert Tibetan font_______________							
			//first check did we load the map of this font						
			if ( logicProcessor->fontMap.find(str) != logicProcessor->fontMap.end()) {
				DR(str<<" Map exist"<<END)
				//flag=1;
			}else{
				DT(str<<" Map not exist. Load map "<<str<<END);
				pathMap=inputData.data["tablePath"]+"/codePages/FontMap/"+str+".map";
				logicProcessor->LoadFontMap(logicProcessor->fontMap[str], pathMap);	
			}	
			DR("fontMap["<<str<<"]"<<logicProcessor->fontMap[str].size()<<endl)
			tibFontName=str; YagpoWylieFontFlag=0;
            
			//cout_ <<" font name="<<str<<" letter"<<letter<<END;
			
			if(str=="Yagpo!_Wylie"||str=="Yagpo!_Wylie_Callygraphy"){
				YagpoWylieFontFlag=1;
					if (logicProcessor->fontMap[str].find(letter) != logicProcessor->fontMap[str].end()) {
					letter=logicProcessor->fontMap[str][letter].keyUTF;	
				}else{
					letter="[{"+letter+"="+str+"}]"; //report+=letter;
				}
				DR("l="<<letter<<endl)
				uniStack+=letter;
				
			}else{
				YagpoWylieFontFlag=0;
				DR("letter="<<letter<<"/ fontMap[str][letter].OCRKey="<<logicProcessor->fontMap[str][letter].OCRKey<<" "<<str<<endl)
				if (logicProcessor->fontMap[str].find(letter) != logicProcessor->fontMap[str].end()) {
					if(logicProcessor->fontMap[str][letter].keyUTF.length()>0){
						letter=logicProcessor->fontMap[str][letter].keyUTF;	
					}else{
						letter="[{"+logicProcessor->fontMap[str][letter].OCRKeyHex+"="+str+"}]"; //report+=letter;
					}	 
				}else{
					letter="[{"+letter+"="+str+"}]" ;  //report+=letter;
				}
				DR("l="<<letter<<endl)
				if(letter!="•")mainString+=letter;
                uniStack+=letter;
			}

			
		}else{
			if(mode==1){
                uniStack+=letter;
            }else{
                DR(str<<" is Western font letter="<<letter<<END)
                if(uniStack.size()>0){
                    mainString+=uniStack;
                    uniStack=" ";
                }
                mainString+=letter;
            }
		}
		letterIndex++;
		
	}//end while
	if(uniStack.size()>0){
		mainString+=uniStack;
		uniStack=" ";
	}
	DT("done convert");


    

	
	//set textLineSize in base
	//aliKali->textLineSize=0;
	//for(int i=0;i<aliKali->letterCount();i++){
   //     GLetter *letter=aliKali->getLetter(i);
//		aliKali->textLineSize+=letter->y1-letter->y0;
  //      letter->destroy();
//	}if(aliKali->letterCount())aliKali->textLineSize/=aliKali->letterCount();
	
	
	cout_<<"m3";
	
	//[pool release];
	
	cout_<<"m4";
#endif
#endif	
	
}//__________________________________________________________________________



///функция проверяет всю базу на наличие частей букв аналогичных указанным частям тестовой буквы. Для найденных букв применяется расстановка масок указанных частей тестовой буквы
void GFontEditor::testMask(){
    
    int print=0;
    inputData.data["ocrData"]="setMask"; 
    
    setBase();
    
    int letterIndex=aliKali->testLetter;
    
    DT("letterIndex="<<letterIndex<<endl);
    GLetter *glyphMain; glyphMain=aliKali->getLetter(letterIndex);
  
    
    //for(int a=0;a<glyphMain->mask32Count();a++){
    //    DT("glyphMain.mask32["<<a<<"].status="<<glyphMain->mask32[a].status<<endl);
    //    DT("aliKali[0][letterIndex].mask32["<<a<<"].status="<<aliKali[0][letterIndex]->mask32[a].status<<endl);
   // 
   // }
    

    
    
    for(int index=0;index<aliKali->letterCount();index++){  
         
        if(index==letterIndex)continue;

        GBitmap *outBitmap=GBitmap::create(PICT_SIZE,PICT_SIZE);
        GLetter *letter=aliKali->getLetter(index);
		GBitmap *img=letter->mask128.unPackMask();
		for (int y = 0; y <img[0].rows() ;y++){
			//pSrc = (unsigned char*)letterImage->Canvas->ScanLine[y];
			for (int x = 0; x <img[0].columns(); x++){
				
				//if((int)img[0][y][x]){cout_<<"1";}else{cout_<<"0";}
				
				if(img[0][y][x]){
					
				}else{
					//if(outBitmap[0]
					//   [y+aliKali[0][index]->mask128.yMask]
					//   [x+aliKali[0][index]->mask128.xMask]==255){
					outBitmap[0]
					[y+letter->mask128.yMask+PICT_SIZE/2]
					[x+letter->mask128.xMask+PICT_SIZE/2]=0;
					//}
				}
			}//cout_<<END;
		}
        
        //string str_="/_2.jpg";
        //WriteImageData(outBitmap,str_,1); //exit(0);###
        //str_="/_3.jpg";
        //WriteImageData(img,str_,1); //exit(0);###
        img->destroy();
        
        GBitsetMatrix* setMatrix=GBitsetMatrix::createRegion(outBitmap,
                                              0,
                                              0,
                                              outBitmap->columns(),
                                              outBitmap->rows(),
                                              1,1,
                                              IMGNOFLIP);
        
        
        //cout_<<"c2";
        GLetter *glyph=GLetter::create();
        
        for(int a=0;a<glyphMain->mask32Count();a++){
            DT("glyphMain.mask32["<<a<<"].status="<<glyphMain->mask32[a].status)
            if(glyphMain->mask32[a].status!=3)continue;   //проверяем только новые маски
            glyph->copy(glyphMain);
            glyph->mask32[0]=glyphMain->mask32[a];
            
            int correlation=setMatrix->letterToLetterCorrelation(glyph,
                                                                 0,
                                                                 outBitmap->rows(),
                                                                 0,
                                                                 0);
            
            DT("correlation="<<correlation<<" "<<letter->name.c_str()<<endl);
            if(correlation<60)continue;
                  
            ostringstream out;
            
            out<<"<glyph id=\""<<letter->idNumber<<"\" n=\""<<letter->name.c_str()<<"\" a=\"1\"/>";
            
            DT("@mask1 m="<<a<<" xMask="<<glyph->mask32[0].xMax<<" yMask="<<glyph->mask32[0].yMax<<endl);
        
            int x0=glyph->mask32[0].xMax;
            int x1=x0+glyph->mask32[0].imgW;
            int y0=glyph->mask32[0].yMax;
            int y1=y0+glyph->mask32[0].mH;
            int dlt0=glyph->mask32[0].dlt0;
            
            out<<"<mask g=\""<<letter->idNumber<<"\"  x0=\""<<x0<<"\" x1=\""<<x1<<
            "\" y0=\""<<y0<<"\" y1=\""<<y1<<"\" d=\""<<dlt0<<"\"/>";
            
            
            inputData.data["setBase"]=out.str();
            
            setBase();

        

        }
        
        //WriteImageData(img,"/_1.jpg",1); //exit(0);
        //string str="/_RGB.jpg";
        //WriteImageData(setMatrix->drawDataRGB,str,0);
        setMatrix->destroy();
        outBitmap->destroy();
        glyph->destroy();
        letter->destroy();
        
    }
    
    for(int a=0;a<glyphMain->mask32Count();a++){
        glyphMain->mask32[a].status=1;
    }
    aliKali->saveLetter(glyphMain);
    glyphMain->destroy();

};



