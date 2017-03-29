#include "GImageEditor.h"

#ifdef COCOA
#import <Cocoa/Cocoa.h>
#endif


/** \fn void GImageEditor::RemoveBorder(GBitmap *data,  GBitset *set,
 string &filePath)
 \brief Remove black border around scaned page  
 
 обработка скана в виде ОДНОГО БЛОКА
 Создание из скана одного блока текста (печа) не зависимо от того
 есть ли колонки в тексте или их нет в виде;
 файлов - (jpg) с добавлением Border, или
 файлов - (jpg) с добавлением сдвига (change) (выключки) блока текста.
 при сохранении исходного размера скана.
 */
GBitmap* GImageEditor::RemoveBorder(GBitmap *data,
								GBitset *set,
								int pageBorder){
	
	cout<<"@@@set->frameArray.size()="<<set->frameArray.size()<<endl;
    
	int xfg0,xfg1,yfg0,yfg1;    // координаты прямоугольника вокруг колонок
	int brd=0;//set->Brd;           // brd=border;  где Brd - глобальная /////////////
	//int hp=data->rows();        // -brd*2 //+64*2  border*2   // hp=nrows;
	
    cout<<"m0";
    
    int wp=data->columns();        // -brd*2 //+64*2  border*2   // hp=nrows;
	unsigned char *pSrc;
	int nFrm;
	
    cout<<"m1";
    
	///     // вычисленное количество строк в печа - NStrok=strArray.size();
	///            NStrok=strArray->size();
	///            if( NStrok<3 ) { return; }
	
	
	// получение координат x,y файлов - колонок (число колонок =0)
	//int nFrame=set->frameArray.size();  // nFrame - номера колонок 1,2,3...   1-печа
	//nFrm=nFrame-1;
	nFrm=0;   // nFrm - номера колонок 0,1,2,3...   0-печа
	if(!set->frameArray.size())return NULL;
	frameOCR *wF=&set->frameArray[nFrm];
    cout_<<"m2";
    
	//////  xfg0=wF->Xfg0;   xfg1=wF->Xfg1;   yfg0=wF->Yfg0;   yfg1=wF->Yfg1;
	xfg0=wF->Xfg0-brd;  xfg1=wF->Xfg1+brd;
	if( xfg0<0)xfg0=0; if( xfg1>wp)xfg1=wp;
	yfg0=wF->Yfg0-brd;  yfg1=wF->Yfg1+brd;
    cout_<<"m3";	
	// задание размеров выходного файла
	int w=xfg1-xfg0+pageBorder*2;
	int h=yfg1-yfg0+pageBorder*2;
	GBitmap *cropImg = GBitmap::create(w,h);
	
	cout<<" w="<<w<<" h="<<h<<" data->columns()="<<data->columns()<<endl;
	cout<<"yfg0="<<yfg0<<" yfg1="<<yfg1<<" xfg0="<<xfg0<<" xfg1="<<xfg1<<endl;
	cout<<nFrm<<" nFrm "<<yfg1<<" yfg1 "<<yfg0<<" yfg0 "<<endl;
	cout <<"nFrm="<<nFrm<<"yfg1"<<yfg1<<"yfg0"<<yfg0<<endl;
	//cout<<"strArray.size()="<<strArray.size()<<endl;
	
	int x,y,q=0,yInd;
	int nClm,nColumn;
	//int yf0;      // координаты прямоугольника вокруг вертикальной
	// колонки, касающийся краёв горизонтальных блок букв
	int x0,x1,y0,y1;  // координаты прямоугольника вокруг горизонтальных
	// блоков описывающих многоугольники колонок
	
	// Левый и правый бордюр, заполнение белым (255)
	nColumn=(int)set->frameArray[nFrm].columnArray.size();
	yInd=pageBorder; 
	for ( nClm=nColumn-1; nClm >= 0; nClm-- ){ // Цикл по количеству блоков строк Clm
		columnOCR *wC=&set->frameArray[nFrm].columnArray[nClm];
		x0=wC->Xc0;   x1=wC->Xc1;   y0=wC->Yc0;   y1=wC->Yc1;
		//cout_<<" xfg0="<<xfg0<<" xfg1="<<xfg1<<" x0="<<wC->Xc0<<"  x1="<<wC->Xc1<<"  y0="<<wC->Yc0<<"  y1="<<wC->Yc1<<endl;
		for (y = y1;  y < y0; y++){
			pSrc = cropImg->bytes_data;
			pSrc +=yInd*w+pageBorder; yInd++;
			q++;  // p++;  if(q>h){q=h;}
			for (x=xfg0; x < xfg1; x++){
				if( x > x0 && x < x1 ) {
					*pSrc=data[0][y][x];
				} // 254-белое
				else{ *pSrc=255; }  // 0-черное, 255-белое
				pSrc++;
			}
		}
		///DM(nClm<<" nClm "); DM(hp-y1<<" hp-y1 "); DM(hp-y0<<" hp-y0 "); DM(q<<" q2 "); DM(p<<" p ");  DM(END);
	} // Цикл по количеству блоков колонок nClm

	return cropImg;
    
    
	
}//_____________________________________________________________________________

/** @fn  void GImageEditor::WriteSpreadData(GBitmap *data,  GBitset *set,
 int nFrm,  string &filePath)
 @brief обработка скана (разворотов книги) с РАЗДЕЛЕНИЕМ  на файлы - СТРАНИЦЫ
 
 Исходная сканированная страница разворота книги разделяется на два файла в формате  jpg
 с именами исходного файла и добавлением суффикса номера страницы в развороте
 */
void GImageEditor::WriteSpreadData(GBitmap *data,  
								   GBitset *set,
								   int nFrm,  
								   string &filePath){
	
	
	int xfg0,xfg1,yfg0,yfg1;        // координаты прямоугольника вокруг колонок
	//int brd=set->Brd;               // brd=border;  где Brd - глобальная /////////
	int wp=data->columns(); //- brd*2; // w=  -64*2                      /////////
	int hp=data->rows();    //- brd*2; // h=  -64*2                      /////////
	unsigned char *pSrc;
	int print=0;
	
	///     // вычисленное колличество строк в печа - NStrok=strArray.size();
	///             NStrok=strArray->size();
	///            if( NStrok<3 ) { return; }
	
	
	// получение координат x,y файлов - страниц
	
    unsigned int TurnXL=set->TurnX;
    
    //cout<<" set->TurnX="<<set->TurnX<<" set->border_size()="<<set->border_size()<<" data->columns()="<<data->columns()<<endl;
    
    
	DR("TurnXL="<<TurnXL<<endl);
	
	if( TurnXL==0 || TurnXL>data->columns()) { return; } // НЕТ вертикальной линии разделения скана
	if( nFrm >-1 || nFrm <-2 ) { return; }
	
    int dL=0;
    
	// получение координат x,y файлов - страниц
	if( nFrm==-1 ) { xfg0=0;   xfg1=TurnXL+dL;  yfg0=0; yfg1=hp; } // левая  страница
	if( nFrm==-2 ) { xfg0=TurnXL;  xfg1=wp;  yfg0=0; yfg1=hp; } // правая страница
	
	int w=xfg1-xfg0+dL;
	int h=yfg1-yfg0;
	
    DR("yfg0="<<yfg0<<" yfg1="<<yfg1<<" xfg0="<<xfg0<<" xfg1="<<xfg1<<" w="<<w<<" h="<<h<<endl);
	GBitmap *cropImg = GBitmap::create(w,h);

	
	/// разделение на страницы. Создание файлов (jpg).
	// Создание файлов (jpg) без стирания изображения по периметру
	int x,y,p=0;
	for (y = 0; y < h; y++){
		pSrc = cropImg->bytes_data;
		pSrc+=y*w;  
		p=y + hp-yfg1;
		for (x=xfg0; x < xfg1; x++){
			*pSrc=data[0][p][x];  //  куда
			pSrc++;
		}}
	//  detectRotation(nFrm);
	//cout<<"filePath="<<filePath<<endl;
    string mode="jpg";
	WriteImageData(cropImg,filePath,0,mode);
	cropImg->destroy();

	
}//_____________________________________________________________________________

/** @fn void GImageEditor::WriteColumnsData(GBitmap *data, GBitset *set,  int nFrm,  string &filePath)
 @brief обработка скана с РАЗДЕЛЕНИЕМ НА КОЛОНКИ
 
 Исходная сканированная страница книги содержащая колонки текста разделяется на
 файлы по числу колонок текста. Для каждой колонки создается отдельный файл  (jpg)
 с именем исходной страницы и суффиксом номера колонки. Вокруг каждой колонки 
 добавляется свободное пространство шириной  Border
 */ 
void GImageEditor::WriteColumnsData(GBitmap *data,
                                    GBitset *set,
                                    int nFrm,
                                    string &filePath){
	/*
	 int xfg0,xfg1,yfg0,yfg1;    // координаты прямоугольника вокруг колонок
	 int hp=data->rows();        //+64*2 // border*2   // hp=nrows; ////////////////////////////
	 unsigned char *pSrc;
	 //short level=0;            //if(invert)level=255;
	 
	 
	 // получение координат x,y файлов - колонок
	 if( nFrm >= 0 ) {      // nFrm - номера колонок 0,1,2,3...   0-печа
	 frameOCR *wF=&set->frameArray[nFrm];
	 //xfg0=wF->xfg0;   xfg1=wF->xfg1;   yfg0=wF->yfg0;   yfg1=wF->yfg1;
	 xfg0=wF->Xfg0;   xfg1=wF->Xfg1;   yfg0=wF->Yfg0;   yfg1=wF->Yfg1;
	 //ShowMessage(nFrm);
	 }
	 else  { return; }
	 
	 // задание размеров выходного фвйла
	 ImageEnView1->RemoveAllObjects();
	 int w=xfg1-xfg0;
	 ImageEnView1->IEBitmap->Width=w;
	 int h=yfg1-yfg0;
	 ImageEnView1->IEBitmap->Height=h;
	 ImageEnView1->IEBitmap->PixelFormat=ie8g;
	 
	 //DM("yfg0="<<yfg0<<" yfg1="<<yfg1<<" xfg0="<<xfg0<<" xfg1="<<xfg1<<" w="<<w<<" h="<<h<<END);
	 //DM(END); DM(hp<<" hp "); DM(nFrm<<" nFrm "); DM(END);
	 //ShowMessage(nFrm);   ShowMessage(xfg1);   ShowMessage(xfg0);
	 
	 int x,y,p,q;
	 int nColumn,nClm,nColum0,nColumN;
	 int yf0,yf1;      // координаты прямоугольника вокруг вертикальной
	 // колонки, касающийся краёв горизонтальных блок букв
	 int x0,x1,y0,y1;  // координаты прямоугольника вокруг горизонтальных
	 // блоков описывающих многоугольники колонок
	 
	 */
	/********************************
	 /// разделение на колонки, заполнение белым только левого и правого бордюра.
	 // Создание файлов (jpg).
	 q=0; //pSrc=0;
	 nColumn=set->frameArray[nFrm].columnArray.size();
	 for ( nClm=nColumn-1; nClm >= 0; nClm-- ){ // Цикл по количеству блоков колонок Clm
	 columnOCR *wC=&set->frameArray[nFrm].columnArray[nClm];
	 x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;
	 for (y = hp-y1;  y < hp-y0; y++){  // (y = hp-y1;  y < hp-y0; y++)
	 pSrc = (unsigned char *)ImageEnView1->IEBitmap->ScanLine[q];
	 q++;   // p=y + hp-yfg1; yfg1
	 for (x=xfg0; x < xfg1; x++){
	 if( x > x0 && x < x1 ) { *pSrc=data[0][y][x]; }  // p куда
	 else{ *pSrc=255; }  // 0-черное, 255-белое
	 pSrc++;
	 }}
	 ///DM(nClm<<" nClm "); DM(END);
	 } // Цикл по количеству блоков колонок nClm
	 *****************/
	
	/*
	 /// заполнение бордюра белым (255) между прямоугольником вокруг горизонтальных
	 /// блоков и самими горизонтальными блоками
	 
	 // Верхний бордюр, заполнение белым (255)
	 yf1=set->frameArray[nFrm].Yf1;
	 q=0;  // p=0; // индикатор
	 for (y=hp-yfg1;  y < hp-yf1; y++){
	 pSrc = (unsigned char *)ImageEnView1->IEBitmap->ScanLine[q];
	 q++; // p++;
	 for (x=xfg0; x < xfg1; x++){
	 *pSrc=255;   // 0-черное, 255-белое
	 pSrc++;
	 }}
	 ///DM(END); DM(hp-yfg1<<" hp-yfg1 "); DM(hp-yf1<<" hp-yf1 "); DM(q<<" q1 "); DM(p<<" p "); DM(END); DM(END);
	 
	 // Левый и правый бордюр, заполнение белым (255)
	 nColumn=set->frameArray[nFrm].columnArray.size();
	 for ( nClm=nColumn-1; nClm >= 0; nClm-- ){ // Цикл по количеству блоков колонок Clm
	 columnOCR *wC=&set->frameArray[nFrm].columnArray[nClm];
	 x0=wC->Xc0;   x1=wC->Xc1;   y0=wC->Yc0;   y1=wC->Yc1;
	 for (y = hp-y1;  y < hp-y0; y++){
	 pSrc = (unsigned char *)ImageEnView1->IEBitmap->ScanLine[q];
	 q++;   // p++; if(q>h){q=h;}
	 for (x=xfg0; x < xfg1; x++){
	 if( x > x0 && x < x1 ) { *pSrc=data[0][y][x]; } // 254-белое
	 else{ *pSrc=255; }  // 0-черное, 255-белое
	 pSrc++;
	 }}
	 ///DM(nClm<<" nClm "); DM(hp-y1<<" hp-y1 "); DM(hp-y0<<" hp-y0 "); DM(q<<" q2 "); DM(p<<" p ");  DM(END);
	 } // Цикл по количеству блоков колонок nClm
	 DM(END);
	 // Нижний бордюр, заполнение белым (255)
	 yf0=set->frameArray[nFrm].Yf0;
	 for (y = hp-yf0;  y < hp-yfg0; y++){
	 pSrc = (unsigned char *)ImageEnView1->IEBitmap->ScanLine[q];
	 q++;   // p++; if(q>h){q=h;}
	 for (x=xfg0; x < xfg1; x++){
	 *pSrc=255;   // 0-черное, 255-белое
	 pSrc++;
	 }}
	 ///DM(END); DM(hp-yf0<<" hp-yf0 "); DM(hp-yfg0<<" hp-yfg0 "); DM(q<<" q3 "); DM(p<<" p "); DM(END); DM(END);
	 
	 //DM(END); DM(h<<" h "); DM(yf1-yf0<<" yf1-yf0 "); DM(END);
	 
	 
	 //DM("done"<<filePath.c_str());
	 ImageEnView1->Update(); Application->ProcessMessages();
	 pageFrameIndex=-1;  pechaDataLoaded=0;
	 detectRotation(nFrm);
	 //ShowMessage(filePath.c_str());
	 if(CheckBoxTIFF->Checked==true){
	 ImageEnView1->IO->Params->BitsPerSample=1;
	 ImageEnView1->IO->Params->SamplesPerPixel=1;
	 ImageEnView1->IO->Params->TIFF_Compression=ioTIFF_G4FAX;
	 ImageEnView1->IO->SaveToFileTIFF(filePath.c_str());
	 }else{
	 ImageEnView1->IO->SaveToFile(filePath.c_str());
	 }
	 */
	//x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;
	//int wp=data->columns();   // w=
	// int NStrok = sArray.size();
	// int NStrok=set->NStrok;
}//_____________________________________________________________________________


/** @fn void GImageEditor::WriteImageData(GBitmapInt *data,string &path,bool flip)    
 @brief отладочная функция записи в графический файл графического массива GBitmapInt
 
 выполнение этой функции платформо зависимо и реализуется на основе применяемой в основной программе 
 графических библиотек ввода-вывода
 реализован вариант для MACOS Cocoa и  PC HiComponent InImage
 */
void GImageEditor::WriteImageDataRGB(GBitmap *data,string &path,bool flip){
#ifdef COCOA
    //#define DP(x) cout_<<x
	int w=data->columns();
	int h=data->rows();
    
	//cout_<<"mode="<<mode<<endl;
	int print=0;
	DT("main w="<<w<<" h="<<h<<END);
    
	//static	NSApplication *app = [NSApplication sharedApplication];		
	//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	NSString *filePath=[[NSString alloc] initWithUTF8String:path.c_str()];
	//NSImage *sourceImage=[[NSImage alloc] initWithSize:NSMakeSize(w, h)]; //create Image;
	NSBitmapImageRep *srcImageRep;
    
    
    srcImageRep = [[NSBitmapImageRep alloc] 
                   initWithBitmapDataPlanes:NULL
                   pixelsWide:w 
                   pixelsHigh:h
                   bitsPerSample:8 
                   samplesPerPixel:4
                   hasAlpha:YES
                   isPlanar:NO
                   colorSpaceName:NSCalibratedRGBColorSpace
                   bytesPerRow:w*4
                   bitsPerPixel:32];
    
    
    DT("DRAW0_2");	
    //short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
    int wByte;
    w = (int)[srcImageRep  pixelsWide];
    h = (int)[srcImageRep pixelsHigh];
    wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
    
    DT("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
    
    //unsigned char lineData[wByte*h+w];
    unsigned char *srcData = [srcImageRep bitmapData];
    
    unsigned char *pSrc;
    int *p;
        int x,y;
    for (x = 0; x < w; x++){
        for (y = 0; y < h;y++){
            pSrc=srcData+y*wByte+x*4;
            p=(int*)pSrc;
            //cout<<hex<<data->get(x,y)<<endl;
            *p=data->get(x,y);
        }//cout_<<endl; //draw on screen
    }
   
    NSData *dataJpeg;
    dataJpeg = [srcImageRep representationUsingType: NSJPEGFileType properties:nil];
    [dataJpeg writeToFile:filePath atomically: NO];
    
    //DT("start save "<<path);
    //[[srcImageRep TIFFRepresentationUsingCompression:NSTIFFCompressionCCITTFAX4
    //                                          factor:0.0] writeToFile:filePath atomically:YES];
    
    
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
	//[srcImageRep release];
	//[pool release];
	DT(" done save RGB"<<END);
	
	
	//icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
    //         representationUsingType:NSGIFFileType properties:nil] retain];
	//icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
	//representationUsingType:NSGIFFileType properties:
	//		 [NSDictionary dictionaryWithObjectsAndKeys:
	//		 NSImageRGBColorTable, NULL]] retain];
#endif
	
	
	
	
}//_____________________________________________________________________________

void GImageEditor::WriteImageDataRGB(GBitmap *data,const char *path,bool flip){
    string path_=path;
    WriteImageDataRGB(data,path_,flip);
}

/** @fn void GImageEditor::WriteImageData(GBitmap *data,string &path,bool flip)   
 @brief функция записи в графический файл байтового графического массива GBitmap
 
 выполнение этой функции платформо зависимо и реализуется на основе применяемой в основной программе 
 графических библиотек ввода-вывода
 реализован вариант для MACOS Cocoa и  PC HiComponent InImage
 */

void GImageEditor::WriteImageData(GBitmap *data,string &path,bool flip, string &mode){

#ifdef MACOS
#ifdef COCOA
//#define DP(x) cout_<<x
    if(data->color()==cRGB){WriteImageDataRGB(data, path,0);    return;}
    if(data->pack_flag)data->unpackImg(0);
	short w=data->columns();
	short h=data->rows();
	int print=0;
	if(w==0||h==0){
       cout<<"no image"<<endl; 
       return;
    }    
	DR("mode="<<mode<<" w"<<w<<" h"<<h<<endl)

	//static	NSApplication *app = [NSApplication sharedApplication];
#ifndef OCR_DYLIB
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#endif
	NSString *filePath=[[NSString alloc] initWithUTF8String:path.c_str()];
	
    
    //NSImage *sourceImage=[[NSImage alloc] initWithSize:NSMakeSize(w, h)]; //create Image;
	NSBitmapImageRep *srcImageRep;

    
	if(mode=="jpg"||mode=="jpe"){
			                   srcImageRep = [[NSBitmapImageRep alloc]
											 initWithBitmapDataPlanes:NULL
											 pixelsWide:w 
											 pixelsHigh:h
											 bitsPerSample:8 
											 samplesPerPixel:1
											 hasAlpha:NO
											 isPlanar:NO
											 colorSpaceName:NSCalibratedWhiteColorSpace
											 bytesPerRow:w 
											 bitsPerPixel:8];
           
			
			DR("DRAW0_2")	
			//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha!
			int wByte;
			w = [srcImageRep  pixelsWide];	
			h = [srcImageRep pixelsHigh];
			wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
			
			DR("bytesPerRow="<<[srcImageRep bytesPerRow]<<END)		
			DR("main w="<<w<<" h="<<h<<END)		
			//unsigned char lineData[wByte*h+w];
			unsigned char *srcData = [srcImageRep bitmapData];
			
			unsigned char *pSrc;
			
			int x,y;
        if(flip){
			for (y = 1; y <h;y++){
				for (x = 0; x < wByte; x++){
					pSrc=srcData+y*wByte+x;
					*pSrc=data[0][h-y][x];
					//if(data[0][y][x]>120){cout_<<"1";}else{cout_<<"0";} //draw on screen
				}//cout_<<endl; //draw on screen
			}
        }else{
            for (y = 0; y < h;y++){
				for (x = 0; x < wByte; x++){
					pSrc=srcData+y*wByte+x;
					*pSrc=data[0][y][x];
					//if(data[0][y][x]>120){cout_<<"1";}else{cout_<<"0";} //draw on screen
				}//cout_<<endl; //draw on screen
			}
        }
		    DR("start save "<<path<<" mode="<<mode); //NSLog(filePath)
			NSData *dataJpeg;
			dataJpeg = [srcImageRep representationUsingType: NSJPEGFileType properties:nil];
		    [dataJpeg writeToFile:filePath atomically: NO];
	}
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if(mode=="tif"){  //сохранение изображения как Tiff GroupFax4
	   // cout_<<"DRAW0_tif";
        //При записи считаем что белые пиксели должны быть равны 0, а черные 255
        
		srcImageRep =[[NSBitmapImageRep alloc]
					 initWithBitmapDataPlanes:NULL
					 pixelsWide:w 
					 pixelsHigh:h
					 bitsPerSample:1 
					 samplesPerPixel:1
					 hasAlpha:NO
					 isPlanar:NO
					 colorSpaceName:NSCalibratedWhiteColorSpace
					 bytesPerRow:w 
					 bitsPerPixel:1];
			
		//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha!
		int wByte;
		w = [srcImageRep  pixelsWide];	
		h = [srcImageRep pixelsHigh];
		wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
		DR("bytesPerRow="<<[srcImageRep bytesPerRow]<<END)		
		DR("main w="<<w<<" h="<<h<<" wByte"<<wByte<<END)		
		//unsigned char lineData[wByte*h+w];
		unsigned char *srcData = [srcImageRep bitmapData];
		unsigned char *pSrc;	
		int x,y;
		
		pSrc=srcData;
		*pSrc=255;
		unsigned char reg=0;
		int d;
		for (y = 0; y < h;y+=1){
			for (x = 0; x < wByte/8; x++){
				pSrc=srcData+y*wByte+x;
				reg=0; d=x*8;
				for (int b=0; b < 8; b++ ) {
					if(data[0][y][d+b]>128){
						reg=(reg << 1) + 1; // обратный порядок пикселей
					}else{
						reg=(reg << 1); // обратный порядок пикселей
					}	
				}
				*pSrc=reg;
			}
		}
			
		DR("start save "<<path<<" mode="<<mode)
		[[srcImageRep TIFFRepresentationUsingCompression:NSTIFFCompressionCCITTFAX4  
												  factor:0.0] writeToFile:filePath atomically:YES];
		
	
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	}
#ifndef OCR_DYLIB
	[srcImageRep release]; //cout_<<"w1";
    [filePath release];
	[pool release];        //cout_<<"w2";
#endif
	DT(" doneSave"<<endl);
	
	
	//icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
    //         representationUsingType:NSGIFFileType properties:nil] retain];
	//icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
	//representationUsingType:NSGIFFileType properties:
	//		 [NSDictionary dictionaryWithObjectsAndKeys:
	//		 NSImageRGBColorTable, NULL]] retain];
#endif
#endif		
	
}//___________________________________________________________________________


void* GImageEditor::WriteNSImage(GBitmap *data){

#ifdef MACOS
#ifdef COCOA
    //#define DP(x) cout_<<x
    if(data->pack_flag)data->unpackImg(0);
    short w=data->columns();
    short h=data->rows();
    int print=0;
    if(w==0||h==0){
        cout<<"no image"<<endl;
        return nil;
    }
    DR(" w"<<w<<" h"<<h<<endl)
    
    //static	NSApplication *app = [NSApplication sharedApplication];
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    //NSImage *sourceImage=[[NSImage alloc] initWithSize:NSMakeSize(w, h)]; //create Image;
    NSBitmapImageRep *srcImageRep;
    
        srcImageRep = [[NSBitmapImageRep alloc]
                       initWithBitmapDataPlanes:NULL
                       pixelsWide:w
                       pixelsHigh:h
                       bitsPerSample:8
                       samplesPerPixel:1
                       hasAlpha:NO
                       isPlanar:NO
                       colorSpaceName:NSCalibratedWhiteColorSpace
                       bytesPerRow:w
                       bitsPerPixel:8];
        
        
        DR("DRAW0_2")
        //short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha!
        int wByte;
        w = [srcImageRep  pixelsWide];
        h = [srcImageRep pixelsHigh];
        wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
        
        DR("bytesPerRow="<<[srcImageRep bytesPerRow]<<END)
        DR("main w="<<w<<" h="<<h<<END)
        //unsigned char lineData[wByte*h+w];
        unsigned char *srcData = [srcImageRep bitmapData];
        
        unsigned char *pSrc;
        
        int x,y;
        for (y = 0; y < h;y++){
            for (x = 0; x < wByte; x++){
                pSrc=srcData+y*wByte+x;
                *pSrc=data[0][y][x];
                //if(data[0][y][x]>120){cout_<<"1";}else{cout_<<"0";} //draw on screen
            }//cout_<<endl; //draw on screen
        }
        
        NSData *dataJpeg;
        dataJpeg = [srcImageRep representationUsingType: NSJPEGFileType properties:nil];
    
    NSImage *img=[[NSImage alloc] initWithData:dataJpeg];

    
    //[srcImageRep release]; //cout_<<"w1";
    //[pool release];        //cout_<<"w2";
    DT(" doneSave"<<endl);
    return (__bridge void*)img;
    
    //icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
    //         representationUsingType:NSGIFFileType properties:nil] retain];
    //icon = [[[NSBitmapImageRep imageRepWithData:[newIcon TIFFRepresentation]] 
    //representationUsingType:NSGIFFileType properties:
    //		 [NSDictionary dictionaryWithObjectsAndKeys:
    //		 NSImageRGBColorTable, NULL]] retain];
#endif
#endif


}

void GImageEditor::WriteImageData(GBitmap *data,const char* path, bool invert){
	string str=path;
    string mode=substr((int)str.rfind(".")+1,3,str);
	WriteImageData(data,str,invert,mode);
	
}

void GImageEditor::WriteImageData(GBitmap *data,string &path, bool invert){
	string str=path;
    string mode=substr((int)str.rfind(".")+1,3,path);
	WriteImageData(data,str,invert,mode);
}

/** @fn void GImageEditor::WriteImageData(GBitset *data,string &path, bool flip) 
 @brief функция записи в графический файл битового графического массива GBitset
 
 выполнение этой функции платформо зависимо и реализуется на основе применяемой в основной программе 
 графических библиотек ввода-вывода
 реализован вариант для MACOS Cocoa и  PC HiComponent InImage
 */
void GImageEditor::WriteImageData(GBitset *data,string &path, bool flip){ 
#ifdef COCOA
	short w=data->columns();
	short h=data->rows();
	
	//static	NSApplication *app = [NSApplication sharedApplication];		
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	//NSString *filePath=[[NSString alloc]initWithCString:path.c_str() encoding:1];
	NSImage *sourceImage=[[NSImage alloc] initWithSize:NSMakeSize(w, h)]; //create Image;
	
	
	NSBitmapImageRep *srcImageRep =[NSBitmapImageRep imageRepWithData:[sourceImage TIFFRepresentation]];
	DP("DRAW0_1"<<END);	
	//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
	int wByte;
	w = [srcImageRep  pixelsWide];	
	h = [srcImageRep pixelsHigh];
	wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
	
	DP("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
	DP("_main wByte="<<wByte<<" w="<<w<<" h="<<h<<END);		
	//unsigned char lineData[wByte*h+w];
	DP("start save");
	unsigned char *srcData = [srcImageRep bitmapData];
	
	unsigned char *pSrc;
	
    int x,y;
    if(flip){ cout<<"@@@";
        for (y = 0; y < h;y++){
            for (x = 0; x < wByte; x++){
                pSrc=srcData+y*wByte+x;
                *pSrc=data[0][y][x];
                //if(*pSrc>120){cout_<<1;}else{cout_<<0;} //draw on screen
            }
            DP(END); //draw on screen
        }
    }else{
        for (y = 1; y < h;y++){
            for (x = 0; x < wByte; x++){
                pSrc=srcData+y*wByte+x;
                *pSrc=data[0][h-y][x];
                //if(*pSrc>120){cout_<<1;}else{cout_<<0;} //draw on screen
            }
            DP(END); //draw on screen
        }
        
    }
	
	///NSData *TIFFData = [sourceImage TIFFRepresentation];   //write file on disk
	[[sourceImage TIFFRepresentation] writeToFile:@"/1.jpg" atomically:YES];
	DP("save path="<<path<<END);	
	//[[NSBitmapImageRep representationOfImageRepsInArray:representations 
	///				   usingType:NSJPEGFileType properties:nil] writeToFile:filePath atomically:YES];
	/*
	 NSData *imageData;// = [stretchView TIFFRepresentation];
	 //NSBitmapImageRep *imageRep = [NSBitmapImageRep imageRepWithData:imageData];
	 //NSDictionary *imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:0.9] forKey:NSImageCompressionFactor];
	 NSDictionary *imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:0.9] forKey:NSImageCompressionFactor];
	 imageData = [sourceImage representationUsingType:NSJPEGFileType properties:imageProps];
	 [imageData writeToFile:filePath atomically:NO];
	 */
	
	
	//[pool release];
#endif
	
}//_____________________________________________________________________________

string GImageEditor::drawToString(GBitmap *data){
#ifdef WIN_BS
	short w=data->columns();
	short h=data->rows();
	TIEBitmap *bitmapImg=new TIEBitmap;
	bitmapImg->Width=w;
	bitmapImg->Height=h;

	//DM("w="<<w<<" h="<<h<<END);
	unsigned char *pSrc;
	unsigned char *pDest;
	unsigned char *p;

	//ShowMessage((AnsiString)(short)ImageEnView1->IEBitmap->IsGrayScale());
	bitmapImg->PixelFormat=ie8g;

	int x,y;

		for (y = 0; y < h;y++){
			pSrc = (unsigned char *)bitmapImg->ScanLine[y];
			for (x = 0; x < w; x++){
				*pSrc=data[0][y][x];
				pSrc++;
	    }}

	//DP("path="<<path<<END);

	//bitmapImg->Write("out.jpg");
	TImageEnView *viewIE = new TImageEnView((void*)this);
	//viewIE->IEBitmap=bitmapImg;
	//TImageEnIO *outIO=new TImageEnIO((void*)this);
	//outIO->Create();
	viewIE->IO->AttachedIEBitmap=bitmapImg;

	AnsiString Data;
	TMemoryStream *PostData = new TMemoryStream();

	  viewIE->IO->StreamHeaders=False;
	  viewIE->IO->SaveToStreamJpeg(PostData);
	cout_<<"PostData->Size="<<PostData->Size<<endl;
	PostData->Position=0;
	//BYTE *data = (BYTE *) malloc(PostData->Size);
	//memcpy(data,PostData->Memory,PostData->Size);
	string str; str.resize(PostData->Size);
	memcpy(&str[0],PostData->Memory,PostData->Size);
	//viewIE->IO->SaveToFile("D:/2.jpg");
	delete bitmapImg;
	delete viewIE;
	return str;

#endif	

#ifdef COCOA
	short w=data->columns();
	short h=data->rows();
	//static	NSApplication *app = [NSApplication sharedApplication];		
	//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSBitmapImageRep *srcImageRep;

	
		srcImageRep = [[NSBitmapImageRep alloc] 
					   initWithBitmapDataPlanes:NULL
					   pixelsWide:w 
					   pixelsHigh:h
					   bitsPerSample:8 
					   samplesPerPixel:1
					   hasAlpha:NO
					   isPlanar:NO
					   colorSpaceName:NSCalibratedWhiteColorSpace
					   bytesPerRow:w 
					   bitsPerPixel:8];
		
		DP("DRAW0_2");
		//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
		int wByte;
		w = [srcImageRep  pixelsWide];	
		h = [srcImageRep pixelsHigh];
		wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
		
		DP("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
		DP("main w="<<w<<" h="<<h<<END);		
		//unsigned char lineData[wByte*h+w];
		unsigned char *srcData = [srcImageRep bitmapData];
		
		unsigned char *pSrc;
		
		int x,y;
		for (y = 0; y < h;y++){
			for (x = 0; x < wByte; x++){
				pSrc=srcData+y*wByte+x;
				*pSrc=data[0][y][x];
				//if(*pSrc>120){DP(1);}else{DP(0);} //draw on screen
			}
			//DP(END); //draw on screen
		}
		NSData *dataJpeg;
		dataJpeg = [srcImageRep representationUsingType: NSJPEGFileType properties:nil];
		//[dataJpeg writeToFile:@"/1.jpg" atomically: NO];
	    string str; str.resize([dataJpeg length]);
	    char *p=&str[0];
	memcpy(p, [dataJpeg bytes], str.size());
    //[pool release];
	return str;
#endif

	return "dummy string";

}//_____________________________________________________________________________


string GImageEditor::drawToTiffString(GBitmap *data){
#ifdef WIN32
#endif	

#ifdef COCOA
int print=0;	
DT("@0");	
	short w=data->columns();
	short h=data->rows();
	//static	NSApplication *app = [NSApplication sharedApplication];		
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSBitmapImageRep *srcImageRep;
DT("@1");	
	srcImageRep = [[NSBitmapImageRep alloc] 
				   initWithBitmapDataPlanes:NULL
				   pixelsWide:w 
				   pixelsHigh:h
				   bitsPerSample:1 
				   samplesPerPixel:1
				   hasAlpha:NO
				   isPlanar:NO
				   colorSpaceName:NSCalibratedWhiteColorSpace
				   bytesPerRow:w 
				   bitsPerPixel:1];
	
	
	//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
	int wByte;
	w = [srcImageRep  pixelsWide];	
	h = [srcImageRep pixelsHigh];
	wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
	
	DT("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
	DT("main w="<<w<<" h="<<h<<" wByte"<<wByte<<END);		
	if(w==0||h==0)return "";
	//unsigned char lineData[wByte*h+w];
	unsigned char *srcData = [srcImageRep bitmapData];
	
	unsigned char *pSrc;
	
	int x,y;
	
	pSrc=srcData;
	*pSrc=255;
	unsigned char reg=0;
	int d;
	for (y = 0; y < h;y+=1){
		for (x = 0; x < wByte/8; x++){
			pSrc=srcData+y*wByte+x;
			reg=0; d=x*8;
			for (int b=0; b < 8; b++ ) {
				if(data[0][y][d+b]<128){
					reg=(reg << 1) + 1; // обратный порядок пикселей
				}else{
					reg=(reg << 1); // обратный порядок пикселей
				}	
			}
			*pSrc=reg;
		}
	}
		
DT("@2");	
	//[[srcImageRep TIFFRepresentationUsingCompression:NSTIFFCompressionCCITTFAX4  
	//										  factor:0.0] writeToFile:filePath atomically:YES];

	NSData *dataTiff;
	dataTiff =[srcImageRep TIFFRepresentationUsingCompression:NSTIFFCompressionCCITTFAX4  
													   factor:0.0];
DT("@3");
	//[dataJpeg writeToFile:@"/1.jpg" atomically: NO];
	string str; str.resize([dataTiff length]);
	char *p=&str[0];
	memcpy(p, [dataTiff bytes], str.size());
DT("@4");
	//[pool release];
	return str;
#endif

	return "dummy string";
}//_____________________________________________________________________________

GBitmap* GImageEditor::loadFromTiffString(string &dataStr, bool invert){

#ifdef COCOA
	if(!dataStr.size())return NULL;
	
    int print=0;

	DT("@0");
	string inputPath="/1.tiff";

	//writeText(dataStr,"/1.tiff");
	
	
	
	//static	NSApplication *app = [NSApplication sharedApplication];		
	//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	DT("@11");
	//NSString *filePath=[[NSString alloc] initWithCString:inputPath.c_str() encoding:1];
    //cout_<<"@12";
	//NSImage *sourceImage=[[NSImage alloc] initWithContentsOfFile:filePath]; //open Image;
	//[[sourceImage TIFFRepresentation] writeToFile:@"/_1112.tiff" atomically:YES];
	//cout_<<"@1";
	
   // NSBitmapImageRep *srcImageRep = [[NSBitmapImageRep alloc] initWithData:[NSData
	//																		dataWithContentsOfFile:filePath]];

	 NSBitmapImageRep *srcImageRep = [[NSBitmapImageRep alloc] initWithData:[NSData
																			 dataWithBytes:dataStr.c_str() length:dataStr.size()]];
    
	if(!srcImageRep){ 
		cout_<<"NO IMAGE";
		//[pool release];
		exit(0);
		return NULL;
	}	
	
	//[[NSBitmapImageRep alloc] initWithData:[self TIFFRepresentation]];
	//exit(0);
	//NSBitmapImageRep *srcImageRep =[NSBitmapImageRep imageRepWithData:[sourceImage TIFFRepresentation]];
	//NSBitmapImageRep *srcImageRep=[[NSBitmapImageRep alloc] initWithData:[sourceImage TIFFRepresentation]];
	
	DT("@2");	
	//short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
	int wByte;
	int w = (int)[srcImageRep  pixelsWide];
	int h = (int)[srcImageRep pixelsHigh];
	wByte=(int)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
	//cout_<<"@2";
	DT("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
	DT("main w="<<w<<" h="<<h<<" wByte="<<wByte<<END);	
	if(w==0||h==0||wByte==0){
	 cout_<<"NO IMAGE";
		//[srcImageRep  release];
		//[pool release];
		return NULL;
	}
	//unsigned char lineData[wByte*h+w];
	unsigned char *srcData = [srcImageRep bitmapData];
	DT("@31");
	unsigned char *pSrc;
	//cout_<<"@3";
	int samplesPerPixel = (int)[srcImageRep samplesPerPixel];  if(samplesPerPixel==3)samplesPerPixel=4; //???? WHY?
	int bytesPerSample = ([srcImageRep bitsPerPixel]/8)/samplesPerPixel;
	//int totalBytes = [srcImageRep pixelsHigh]*[srcImageRep pixelsWide]*samplesPerPixel*bytesPerSample;

	DT("samplesPerPixel="<<samplesPerPixel<<" bytesPerSample="<<bytesPerSample<<" w="<<wByte/samplesPerPixel<<" h="<<h<<endl);
	GBitmap* pechaImg=GBitmap::create(wByte/samplesPerPixel,h);
	
	
    if(bytesPerSample==1){
		if(samplesPerPixel>2){
			int o = 0;
			int i = 0;
			int background = 255;
			//for (ini=0; i<totalBytes; i+=samplesPerPixel){
			for (int y = 1; y < h;y++){
				o = 0;
				for (i = 0; i < wByte-samplesPerPixel; i+=samplesPerPixel){
					int R = (int)*(srcData+y*wByte+i);
					int G = (int)*(srcData+y*wByte+i+1);
					int B = (int)*(srcData+y*wByte+i+2);
					int grayValue = (int)((R+G+B)/3);
					if (samplesPerPixel==4)
					{
						int alpha = (int)*(srcData+y*wByte+i+3);  // this is the alpha value
						grayValue = (int) (    (grayValue*alpha + background*(255-alpha))    / 255);
					}
				    pechaImg[0][y][o] = grayValue;  //if(grayValue){cout_<<1;}else{cout_<<0;}
					o++;
				}}
		}else{
			int x,y;
			if(invert){
				for (y = 1; y < h;y++){ // cout_<<".";
					for (x = 0; x < wByte; x++){
						pSrc=srcData+y*wByte+x;
						pechaImg[0][y][x]=255-*pSrc;
						//if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
					}
					//_//_DM(END); //draw on screen
				}		  
			}else{
				for (y = 1; y < h;y++){ // cout_<<".";
					for (x = 0; x < wByte; x++){
						pSrc=srcData+y*wByte+x;
						pechaImg[0][y][x]=*pSrc;
						//if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
					}
					//_//_DM(END); //draw on screen
				}		  
			}	   
		}
    } else NSLog(@"Bytes per sample != 1 or less than 3 samples per pixel");
	DT("@3_2");
	
	//[sourceImage autorelease];
	//[srcImageRep release];
	//[pool release];
    DT("@4");
#endif
	//cout_<<"pechaImg1"<<pechaImg->columns()<<endl;
	//imgArray.push_back(pImg);
	///WriteImageData(pechaImg,"/1.jpg",0);
	pechaDataLoaded=1;
	return pechaImg;
	
}//_____________________________________________________________________________


void GImageEditor::WriteImageData(GBitset *data,const char* path, bool flip){
	string str=path;
	WriteImageData(data,str,flip);
}


/** @fn void GImageEditor::LoadImageData(string &inputPath)
 @brief функция загрузки из графического файла графического массива в формате GBitmap
 
 выполнение этой функции платформо зависимо и реализуется на основе применяемой в основной программе 
 графических библиотек ввода-вывода
 реализован вариант для MACOS Cocoa и  PC HiComponent InImage
 */
GBitmap* GImageEditor::LoadImageData(string &inputPath,bool invert){

	pechaDataLoaded=0;
	if(!is_file(inputPath)){cout<<"@@@!!!!@@@@ no file "<<inputPath<<"<<"<<endl; return 0;}
     GBitmap *pechaImg_;
#ifdef COCOA
	
    //cout<<" start load#1 "<<inputPath<<endl;
    int print=0;
	string mode;
	DT("@0"); 	
	string str=substr((uint)inputPath.rfind("."), 5, inputPath); 
	if(str==".jpg"||str==".JPG"||str==".jpeg"||str==".JPEG")mode="jpg";
	if((str==".tif"||str==".TIF"||str==".tiff"||str==".TIFF")){mode="tif"; invert=0;}
    //static	NSApplication *app = [NSApplication sharedApplication];
#ifndef OCR_DYLIB
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#endif
    NSString *filePath=[[NSString alloc] initWithUTF8String:inputPath.c_str()];
    //[[sourceImage TIFFRepresentation] writeToFile:@"/_1112.tiff" atomically:YES];
    
    NSBitmapImageRep *srcImageRep;
    
    float scale=1;
    
    if(scale!=1){
        //TIME_START
        NSImage *sourceImage=[[NSImage alloc] initWithContentsOfFile:filePath]; //open Image;
        NSBitmapImageRep *imRep =[NSBitmapImageRep imageRepWithData:[sourceImage TIFFRepresentation]];

        int height = [imRep pixelsHigh]*scale;
        cout<<" height="<<[sourceImage size].height<<" scaleHeight="<<height<<endl;
        int width = [imRep pixelsWide]*scale;
        NSImage *scaleImage = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
        [scaleImage lockFocus];
        [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
        [sourceImage setSize:NSMakeSize(width, height)];
        //[sourceImage drawAtPoint:NSZeroPoint fromRect:NULL operation:NSCompositeCopy];
        
        [sourceImage drawAtPoint:NSZeroPoint fromRect: NSMakeRect(0,0,width,height) operation: NSCompositeCopy fraction: 1.0];
        [scaleImage unlockFocus];
        //[[scaleImage TIFFRepresentation] writeToFile:@"/_1112.tiff" atomically:YES];
        srcImageRep =[NSBitmapImageRep imageRepWithData:[scaleImage TIFFRepresentation]];
        //cout<<"@scale"; TIME_PRINT_
        
    }else{
        srcImageRep = [[NSBitmapImageRep alloc] initWithData:[NSData dataWithContentsOfFile:filePath]];
    }
    
   /*[self resizedImage:newSize
                    transform:[self transformForOrientation:newSize]
               drawTransposed:drawTransposed
         interpolationQuality:quality];
    */
    
    //[[NSBitmapImageRep alloc] initWithData:[self TIFFRepresentation]];
    //exit(0); 
    //NSBitmapImageRep *srcImageRep =[NSBitmapImageRep imageRepWithData:[sourceImage TIFFRepresentation]];
    //NSBitmapImageRep *srcImageRep=[[NSBitmapImageRep alloc] initWithData:[sourceImage TIFFRepresentation]]
    //short delta=0;   //really a problem. in 600 dpi pecha pixelsWide != wigth of pecha! 
    int wByte;
    uint w = (uint)[srcImageRep  pixelsWide];
    uint h = (uint)[srcImageRep pixelsHigh];
    wByte=(uint)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
    DT("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);		
    DT("main w="<<w<<" h="<<h<<" wByte="<<wByte<<END);	
    
    if(w==0||h==0){cout<<"@@@@!!!!@@@@@ NO IMAGE"<<inputPath<<endl; return 0;}
    //unsigned char lineData[wByte*h+w];
    unsigned char *srcData = [srcImageRep bitmapData];
    unsigned char *pSrc;
    int samplesPerPixel = (uint)[srcImageRep samplesPerPixel];
    int bitsPerPixel=(uint)[srcImageRep bitsPerPixel];
    if(bitsPerPixel==32)samplesPerPixel=4; 
    if(bitsPerPixel==8)samplesPerPixel=1; 
    
    int bytesPerSample = (bitsPerPixel/8)/samplesPerPixel;
    //int totalBytes = [srcImageRep pixelsHigh]*[srcImageRep pixelsWide]*samplesPerPixel*bytesPerSample;
    DT("samplesPerPixel="<<samplesPerPixel<<" bytesPerSample="<<bytesPerSample<<" bitsPerPixel="<<bitsPerPixel<<endl);
    if(samplesPerPixel==1){
        if(bitsPerPixel>1){
            pechaImg_=GBitmap::create(wByte,h);
        }else{
            pechaImg_=GBitmap::create(w,h);
        }	
    }
    //if(samplesPerPixel==3&&mode=="jpg")samplesPerPixel=4; //?? загадка природы
    if(samplesPerPixel>1)pechaImg_=GBitmap::create(wByte/samplesPerPixel,h);
	
    //cout_<<" pechaImg_.w="<<pechaImg_->columns()<<" pechaImg_.h="<<pechaImg_->rows()<<endl;
    
    if(bytesPerSample==1){
        if(samplesPerPixel>2){
            int o = 0;
            int i = 0;
            int background = 255;
            //for (ini=0; i<totalBytes; i+=samplesPerPixel){
            for (int y = 1; y < h;y++){
                o = 0;
                for (i = 0; i < wByte-samplesPerPixel; i+=samplesPerPixel){
                    int R = (int)*(srcData+y*wByte+i);
                    int G = (int)*(srcData+y*wByte+i+1);
                    int B = (int)*(srcData+y*wByte+i+2);
                    int grayValue = (int)((R+G+B)/3);
                    if (samplesPerPixel==4)
                    {
                        int alpha = (int)*(srcData+y*wByte+i+3);  // this is the alpha value
                        grayValue = (int) (    (grayValue*alpha + background*(255-alpha))    / 255);
                    }
                    pechaImg_[0][y][o] = grayValue;  //if(grayValue){cout_<<1;}else{cout_<<0;}
                    o++;
                }}
        }else{
            int x,y;
            //Tiff packed in Tiff GroupFax Cocoa распаковывает как байтовый формат
            // для формата Tiff GroupFax не определен бит который задает негативное или позитивное изображение. Все программы читают его по-разному.
            // принимаем как допущение что белых пиксекей больше.
            //При записи считаем что белые пиксели должны быть равны 0, а черные 255
            
            uint sP=0;
            uint wT=w/10; 
            uint hT=h/10;    //проверяем 100 точек
            uchar *p;
            for (int y = 1; y < h;y+=hT){  //TiffGroupFax4 уже распакован
                p=srcData+wByte*y;
                for (int x = 0; x < wByte; x+=wT){
                    sP+=*p; //if(*p!=255)//cout<<(short)*p<<" ";
                    p+=wT;
                }//cout<<endl;//draw on screen
            } 
            
            if(sP/255<50&&mode=="tif"){
                //cout<<"черные пиксели равны 255"<<endl;
                for (y = 1; y < h;y++){ // cout_<<".";
                    for (x = 0; x < wByte; x++){
                        pSrc=srcData+y*wByte+x;
                        pechaImg_[0][y][x]=255-*pSrc;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }		  
            }else{
                //cout<<"черные пиксели равны 0"<<endl;
                for (y = 1; y < h;y++){ // cout_<<".";
                    for (x = 0; x < wByte; x++){
                        pSrc=srcData+y*wByte+x;
                        pechaImg_[0][y][x]=*pSrc;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }		  
            }	   
        }
    } else {
        if(bitsPerPixel==1){
            //Tiff packed in Tiff GroupFax
            // для этого формата не определен бит который задает негативное или позитивное изображение. Все программы читают его по-разному.
            // принимаем как допущение что белых пиксекей больше.
            //При записи считаем что белые пиксели должны быть равны 0, а черные 255
            uint sP=0;
            uint wT=w/10;
            uint hT=h/10;    //проверяем 100 точек
            unsigned long int d;
            uchar *p;
            for (int y = 1; y < h;y+=hT){  //TiffGroupFax4 уже распакован
                p=srcData+wByte*y;
                for (int x = 0; x < wByte; x+=wT){
                    sP+=*p; //if(*p!=255)//cout<<(short)*p<<" ";
                    p+=wT;
                }//cout<<endl;//draw on screen
            } 

            
            if(sP/255>50){
            DT("start unpack positive Tiff file wByte="<<wByte<<" h="<<h<<endl;)
                for (int y = 0; y < h;y++){ // cout_<<".";
                    p=srcData+wByte*y;
                    for (int x = 0; x < w; x++){
                        pechaImg_[0][y][x]=*p;
                        *p++;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }
            }else{
                DT("start unpack negative Tiff file wByte="<<wByte<<" h="<<h<<endl;)
                for (int y = 1; y < h;y++){ // cout_<<".";
                    p=srcData+wByte*y;
                    for (int x = 0; x < w; x++){
                        pechaImg_[0][y][x]=255-*p;
                        *p++;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen

                }
            }
                
                
            
            /*for(int  y=0; y < wByte*4; y++ ){ cout_<<"y="<<y; // >>3  деление на 8
             s=srcData[y];   d=y*8;
             cout_<<(short)s; if(step==wByte){cout_<<endl;step=0;}step++;
             for(int  x=0; x < 8; x++ ){
             //(bytes_data[d + x])=s & 1;     s=(s >> 1); // прямой порядок пикселей
             if(invert){
             pechaImg->bytes_data[d + 8-x-1]=255-255*(s & 1); s=(s >> 1); // обратный порядок пикселей
             }else{
             pechaImg->bytes_data[d + 8-x-1]=255*(s & 1); s=(s >> 1); // обратный порядок пикселей
             }
             // уровни в битмапе  0 и 255
             } // x
             } // y
             */

            
        }else{
            NSLog(@"Bytes per sample != 1 or less than 3 samples per pixel");
        }	
    }	
    DT("@3_2");
#ifndef OCR_DYLIB
    [filePath release];
    [srcImageRep release];
    [pool release];
#endif
    DT("@4");
	//}else if((str==".tif"||str==".TIF"||str==".tiff"||str==".TIFF")){
    //DT("start load");
    //pechaImg_=GBitmap::createFromTIFF(inputPath.c_str(),invert);
	//}else{
	//	return 0;
	//}
#endif

        pechaDataLoaded=1;
	return pechaImg_; 
	
}//_____________________________________________________________________________
/** @fn void GImageEditor::LoadImageData(string &inputPath)
 @brief функция загрузки из графического файла графического массива в формате GBitmap
 
 выполнение этой функции платформо зависимо и реализуется на основе применяемой в основной программе
 графических библиотек ввода-вывода
 реализован вариант для MACOS Cocoa и  PC HiComponent InImage
 */
GBitmap* GImageEditor::LoadImageData(void* sourceImage){
    
    pechaDataLoaded=0;
    GBitmap *pechaImg_;
#ifdef COCOA
    
    //cout<<" start load#1 "<<inputPath<<endl;
    int print=0;
    string mode="tif";
    DT("@0");
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    //int scale=1;
    
    
    NSBitmapImageRep *srcImageRep =[NSBitmapImageRep imageRepWithData:[(__bridge NSImage*)sourceImage TIFFRepresentation]];
        
    //int height = (uint)[srcImageRep pixelsHigh]*scale;
    //cout<<" height="<<[sourceImage size].height<<" scaleHeight="<<height<<endl;
        //NSImage *scaleImage = [[[NSImage alloc] initWithSize:NSMakeSize(width, height)] autorelease];
        //[scaleImage lockFocus];
        //[[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];
        //[sourceImage setSize:NSMakeSize(width, height)];
        //[sourceImage drawAtPoint:NSZeroPoint fromRect:NULL operation:NSCompositeCopy];
        //[sourceImage drawAtPoint:NSZeroPoint fromRect: NSMakeRect(0,0,width,height) operation: NSCompositeCopy fraction: 1.0];
        //[scaleImage unlockFocus];
        //[[scaleImage TIFFRepresentation] writeToFile:@"/_1112.tiff" atomically:YES];
       // srcImageRep =[NSBitmapImageRep imageRepWithData:[scaleImage TIFFRepresentation]];
        //cout<<"@scale"; TIME_PRINT_
    
    //[self resizedImage:newSize
    // transform:[self transformForOrientation:newSize]
    // drawTransposed:drawTransposed
    // interpolationQuality:quality];

    int wByte;
    uint w = (uint)[srcImageRep  pixelsWide];
    uint h = (uint)[srcImageRep pixelsHigh];
    wByte=(uint)[srcImageRep bytesPerRow];   //ATENTION on Tiger bytesPerRow aligned by word and bytesPerRow!=w
    DT("bytesPerRow="<<[srcImageRep bytesPerRow]<<END);
    DT("main w="<<w<<" h="<<h<<" wByte="<<wByte<<END);
    
    if(w==0||h==0){cout<<"@@@@!!!!@@@@@ NO IMAGE"<<endl; return 0;}
    //unsigned char lineData[wByte*h+w];
    unsigned char *srcData = [srcImageRep bitmapData];
    unsigned char *pSrc;
    int samplesPerPixel = (uint)[srcImageRep samplesPerPixel];
    int bitsPerPixel=(uint)[srcImageRep bitsPerPixel];
    if(bitsPerPixel==32)samplesPerPixel=4;
    if(bitsPerPixel==8)samplesPerPixel=1;
    
    int bytesPerSample = (bitsPerPixel/8)/samplesPerPixel;
    //int totalBytes = [srcImageRep pixelsHigh]*[srcImageRep pixelsWide]*samplesPerPixel*bytesPerSample;
    DT("samplesPerPixel="<<samplesPerPixel<<" bytesPerSample="<<bytesPerSample<<" bitsPerPixel="<<bitsPerPixel<<endl);
    if(samplesPerPixel==1){
        if(bitsPerPixel>1){
            pechaImg_=GBitmap::create(wByte,h);
        }else{
            pechaImg_=GBitmap::create(w,h);
        }
    }
    //if(samplesPerPixel==3&&mode=="jpg")samplesPerPixel=4; //?? загадка природы
    if(samplesPerPixel>1)pechaImg_=GBitmap::create(wByte/samplesPerPixel,h);
    
    //cout_<<" pechaImg_.w="<<pechaImg_->columns()<<" pechaImg_.h="<<pechaImg_->rows()<<endl;
    
    if(bytesPerSample==1){
        if(samplesPerPixel>2){
            int o = 0;
            int i = 0;
            int background = 255;
            //for (ini=0; i<totalBytes; i+=samplesPerPixel){
            for (int y = 1; y < h;y++){
                o = 0;
                for (i = 0; i < wByte-samplesPerPixel; i+=samplesPerPixel){
                    int R = (int)*(srcData+y*wByte+i);
                    int G = (int)*(srcData+y*wByte+i+1);
                    int B = (int)*(srcData+y*wByte+i+2);
                    int grayValue = (int)((R+G+B)/3);
                    if (samplesPerPixel==4)
                    {
                        int alpha = (int)*(srcData+y*wByte+i+3);  // this is the alpha value
                        grayValue = (int) (    (grayValue*alpha + background*(255-alpha))    / 255);
                    }
                    pechaImg_[0][y][o] = grayValue;  //if(grayValue){cout_<<1;}else{cout_<<0;}
                    o++;
                }}
        }else{
            int x,y;
            //Tiff packed in Tiff GroupFax Cocoa распаковывает как байтовый формат
            // для формата Tiff GroupFax не определен бит который задает негативное или позитивное изображение. Все программы читают его по-разному.
            // принимаем как допущение что белых пиксекей больше.
            //При записи считаем что белые пиксели должны быть равны 0, а черные 255
            
            uint sP=0;
            uint wT=w/10;
            uint hT=h/10;    //проверяем 100 точек
            uchar *p;
            for (int y = 1; y < h;y+=hT){  //TiffGroupFax4 уже распакован
                p=srcData+wByte*y;
                for (int x = 0; x < wByte; x+=wT){
                    sP+=*p; //if(*p!=255)//cout<<(short)*p<<" ";
                    p+=wT;
                }//cout<<endl;//draw on screen
            }
            
            if(sP/255<50&&mode=="tif"){
                //cout<<"черные пиксели равны 255"<<endl;
                for (y = 1; y < h;y++){ // cout_<<".";
                    for (x = 0; x < wByte; x++){
                        pSrc=srcData+y*wByte+x;
                        pechaImg_[0][y][x]=255-*pSrc;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }
            }else{
                //cout<<"черные пиксели равны 0"<<endl;
                for (y = 1; y < h;y++){ // cout_<<".";
                    for (x = 0; x < wByte; x++){
                        pSrc=srcData+y*wByte+x;
                        pechaImg_[0][y][x]=*pSrc;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }
            }
        }
    } else {
        if(bitsPerPixel==1){
            //Tiff packed in Tiff GroupFax
            // для этого формата не определен бит который задает негативное или позитивное изображение. Все программы читают его по-разному.
            // принимаем как допущение что белых пиксекей больше.
            //При записи считаем что белые пиксели должны быть равны 0, а черные 255
            uint sP=0;
            uint wT=w/10;
            uint hT=h/10;    //проверяем 100 точек
            //unsigned long int d;
            uchar *p;
            for (int y = 1; y < h;y+=hT){  //TiffGroupFax4 уже распакован
                p=srcData+wByte*y;
                for (int x = 0; x < wByte; x+=wT){
                    sP+=*p; //if(*p!=255)//cout<<(short)*p<<" ";
                    p+=wT;
                }//cout<<endl;//draw on screen
            }
            
            
            if(sP/255>50){
                DT("start unpack positive Tiff file wByte="<<wByte<<" h="<<h<<endl;)
                for (int y = 0; y < h;y++){ // cout_<<".";
                    p=srcData+wByte*y;
                    for (int x = 0; x < w; x++){
                        pechaImg_[0][y][x]=*p;
                        *p++;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                }
            }else{
                DT("start unpack negative Tiff file wByte="<<wByte<<" h="<<h<<endl;)
                for (int y = 1; y < h;y++){ // cout_<<".";
                    p=srcData+wByte*y;
                    for (int x = 0; x < w; x++){
                        pechaImg_[0][y][x]=255-*p;
                        *p++;
                        //if(pechaImg[0][h-y-1][x]>120)//_DM(1);}else//_DM(0);}; //draw on screen
                    }
                    //_//_DM(END); //draw on screen
                    
                }
            }
            
           }else{
            NSLog(@"Bytes per sample != 1 or less than 3 samples per pixel");
        }
    }
    DT("@3_2");
    
    //[pool release];
    DT("@4");
    //}else if((str==".tif"||str==".TIF"||str==".tiff"||str==".TIFF")){
    //DT("start load");
    //pechaImg_=GBitmap::createFromTIFF(inputPath.c_str(),invert);
    //}else{
    //	return 0;
    //}
#endif
    
    pechaDataLoaded=1;
    return pechaImg_;
    
}//_____________________________________________________________________________
#ifdef COCOA

void saveAsJpegWithName(NSImage *img, NSString* fileName){
    // Cache the reduced image
    NSData *imageData = [img TIFFRepresentation];
    NSBitmapImageRep *imageRep = [NSBitmapImageRep imageRepWithData:imageData];
    NSDictionary *imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:1.0] forKey:NSImageCompressionFactor];
    imageData = [imageRep representationUsingType:NSJPEGFileType properties:imageProps];
    [imageData writeToFile:fileName atomically:NO];
}

void imageDump(CGImageRef cgimage){
    
    size_t width  = CGImageGetWidth(cgimage);
    size_t height = CGImageGetHeight(cgimage);
    
    size_t bpr = CGImageGetBytesPerRow(cgimage);
    size_t bpp = CGImageGetBitsPerPixel(cgimage);
    size_t bpc = CGImageGetBitsPerComponent(cgimage);
    size_t bytes_per_pixel = bpp / bpc;
    
    CGBitmapInfo info = CGImageGetBitmapInfo(cgimage);
    
    NSLog(
          @"\n"
          "CGImageGetHeight: %d\n"
          "CGImageGetWidth:  %d\n"
          "CGImageGetColorSpace: %@\n"
          "CGImageGetBitsPerPixel:     %d\n"
          "CGImageGetBitsPerComponent: %d\n"
          "CGImageGetBytesPerRow:      %d\n"
          "CGImageGetBitmapInfo: 0x%.8X\n"
          "  kCGBitmapAlphaInfoMask     = %s\n"
          "  kCGBitmapFloatComponents   = %s\n"
          "  kCGBitmapByteOrderMask     = %s\n"
          "  kCGBitmapByteOrderDefault  = %s\n"
          "  kCGBitmapByteOrder16Little = %s\n"
          "  kCGBitmapByteOrder32Little = %s\n"
          "  kCGBitmapByteOrder16Big    = %s\n"
          "  kCGBitmapByteOrder32Big    = %s\n",
          (int)width,
          (int)height,
          CGImageGetColorSpace(cgimage),
          (int)bpp,
          (int)bpc,
          (int)bpr,
          (unsigned)info,
          (info & kCGBitmapAlphaInfoMask)     ? "YES" : "NO",
          (info & kCGBitmapFloatComponents)   ? "YES" : "NO",
          (info & kCGBitmapByteOrderMask)     ? "YES" : "NO",
          (info & kCGBitmapByteOrderDefault)  ? "YES" : "NO",
          (info & kCGBitmapByteOrder16Little) ? "YES" : "NO",
          (info & kCGBitmapByteOrder32Little) ? "YES" : "NO",
          (info & kCGBitmapByteOrder16Big)    ? "YES" : "NO",
          (info & kCGBitmapByteOrder32Big)    ? "YES" : "NO"
          );
    
    CGDataProviderRef provider = CGImageGetDataProvider(cgimage);
    NSData* data = (__bridge id)CGDataProviderCopyData(provider);
    //[data autorelease];
    const uint8_t* bytes = (uint8_t*)[data bytes];
    
    printf("Pixel Data:\n");
    for(size_t row = 0; row < height; row++)
    {
        for(size_t col = 0; col < width; col++)
        {
            const uint8_t* pixel =
            &bytes[row * bpr + col * bytes_per_pixel];
            
            printf("(");
            for(size_t x = 0; x < bytes_per_pixel; x++)
            {
                printf("%.2X", pixel[x]);
                if( x < bytes_per_pixel - 1 )
                    printf(",");
                    }
            
            printf(")");
            if( col < width - 1 )
                printf(", ");
                }
        
        printf("\n");
    }
}

#endif


/** @fn void GImageEditor::LoadImageData(GBitmap* pImg, int x0,int y0, int w, int h)
 @brief функция загрузки фрагмента графического файла по заданным  координатам 
 в глобальный байтый графический массив pechaImg
 */
/*void GImageEditor::LoadImageData(GBitmap* pImg,GBitmap* sImg, int x0,int y0, int w, int h){
	
	 ###### some error
	if(sImg==NULL){cout_<<"NO IMAGE"; return;}
	//cout_<<"start"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<"sImg[0].columns()="<<sImg[0].columns()<<" sImg[0].rows()="<<sImg[0].rows()<<endl;
	int print =0;
	if(x0<0||y0<0||w<0||h<=0)return;
	if(w+x0>sImg[0].columns())w=sImg[0].columns()-x0;
	if(h+y0>sImg[0].rows())h=sImg[0].rows()-y0;
	
	unsigned char *pSrc;
	unsigned char *pDest;
	unsigned char *p;
	
	int x,y;
	for (y = 0; y < h;y++){
		for (x = 0; x < w; x++){
			pImg[0][y][x]=sImg[0][y0+y][x0+x];
			//if(pImg[0][y][x]>150){DT(0);}else{DT(1);};
		}DT(END);
	}
	
	//cout_<<"done"<<endl;
}//_____________________________________________________________________________
*/

/** @fn void GImageEditor::drawString(int nStr, int border)
 @brief вывод в стандартный выходной поток HTML отображения
 одной строки из глобального массива результатов распознавания strArray
 */
void GImageEditor::drawString(int nStr, int border){
}//_____________________________________________________________________________




