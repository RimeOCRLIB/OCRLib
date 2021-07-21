#include "GImageEditor.h"

namespace ocr{
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
GBitmap* RemoveBorder(GBitmap *data,
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
void WriteSpreadData(GBitmap *data,  
								   GBitset *set,
								   int nFrm,  
								   string &filePath){
	
	
	int xfg0,xfg1,yfg0,yfg1;        // координаты прямоугольника вокруг колонок
    xfg0=xfg1=yfg0=yfg1=0;
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
    
    cropImg->savePNG(filePath);
	cropImg->destroy();

	
}//_____________________________________________________________________________


};
