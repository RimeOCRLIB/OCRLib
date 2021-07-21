//
// GraphOCR.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,                                
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "GGraph.h"
namespace ocr{
	
GGraph::~GGraph() {
    if(hashTableReady==1) {
        free(hashTable);
        free(hashTableOCRStar);
        free(hashTableStar);
    }

    if(initID_flag){
        free(id_data);
        if(bites_data_32!=0)free(bites_data_32);
    }
    if(initID2_flag){
        free(id_data2);
        if(bites_data_32_2!=0)free(bites_data_32_2);
    }
    if(ncolumns) {
        if(pack_flag !=BITMAP_32) {
            free(bytes_data);
        }
    }
    if(initLine_flag){
        free(line_data);
        if(BufS !=0) free(BufS); 
    }
    // массив координат точек всех фокальныэх линий по "y", float.
    if(coord_fx !=0) free(coord_fx);
    // массив координат точек всех фокальныэх линий по "x", float
    if(coord_fy !=0) free(coord_fy);

}  //____________________________________________________________

// ----- initialization
void GGraph::initData(){
    colorMode=1;
    nrows=0;
    ncolumns=0;
    data_size=0;
    pack_flag=0;
    border=0;
    initID_flag=0;
    initID2_flag=0;
    hashTableReady=0;
    initLine_flag=0;
    BufS = 0;
    bites_data_32=0;
    bites_data_32_2=0;
    coord_fx=0;
    coord_fy=0;
}


GGraph::GGraph(int acolumns, int arows) {
    initData();
    
    // выравнивание bytes_data по 32
    // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
    // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
    nrows=arows;                     // nrows=(arows/32+1)*32;       // nrows=arows;
    ncolumns=(acolumns/32 + 1) * 32;  // ncolumns=acolumns;

    data_size=ncolumns * nrows * colorMode;
    if(!data_size) return;
    data_size_p=0;
    bytes_per_row=ncolumns * colorMode;
    border=0;

    // cout<<" data_size="<<data_size<<endl;
    // инициализация байтового массива char размером  data_size
    bytes_data=(unsigned char *)malloc(data_size);  // запрос памяти с очисткой 0

    fill(0);
    pack_flag=0;
}  //____________________________________________________________

GGraph::GGraph(int acolumns, int arows, int mode) {
    initData();
    
    init(acolumns, arows, mode);
}  //______________________________________________________________

GGraph::GGraph(const GBitmap *ref) {
    if(this !=ref) {
        initData();
        imgW = ref->imgW;
        if(ref->pack_flag !=BITMAP_32) {
            init(ref->columns(), ref->rows(), ref->colorMode);
            uchar *row=bytes_data + bytes_per_row * 3;
            uchar *rowDest;
            uint w=ref->columns();
            
            for(int n=3; n < nrows - 6; n++, row +=bytes_per_row) {
                rowDest=ref->bytes_data + w * n + 3;
                memcpy(row + 3, rowDest, w - 6); //копируем без бордюра в 3 пиксела
            }
            // инверсия уровней входного массива общепринятого стандарта 0-черный 255-белый, во внутренний формат GGraph 0-белый 255-черный
            int sf_rg=sizeof(unsigned long);
            uint w_h_64=data_size/sf_rg;  // sizeof(unsigned long)= 64
            uint64 *bytes_data0_64=(uint64*)bytes_data;    // приведение к uint64
            for (int  x=0; x < w_h_64; x++ ) { bytes_data0_64[x]=~bytes_data0_64[x]; } // по 8 байт одновременно
            
        } else {
            init(ref->columns(), ref->rows(), BITMAP_32);
            memcpy((void *)bites_data, (void *)ref->bites_data, ncolumns * nrows/8);
        }
    }

}  //______________________________________________________________
    
GGraph::GGraph(TString &st){
	initData();
	if(st.size() < 8)return;
	//копируем данные GGraph из TString
	char *p;
	int size_;
	int data[10];
	st.getPtr(&p,0,&size_);
	memcpy((char *)data,p,size_);
	ncolumns = data[0];
	nrows = data[1];
	data_size = data[2];
	data_size_p32 = data[3];
	pack_flag = data[4];
	colorMode = data[5];
	border = data[6];
	bytes_per_row = data[7];
	

	st.getPtr(&p,1,&size_);
	focalPoint.init();
	focalPoint.resize(size_/sizeof(OCRPoint));
	memcpy((char*)focalPoint.dataPtr(),p,size_);
	
	st.getPtr(&p,2,&size_);
	pointArray.init();
	pointArray.resize(size_/sizeof(OCRPoint));
	memcpy((char*)pointArray.dataPtr(),p,size_);

	st.getPtr(&p,3,&size_);
	focalLine.init();
	focalLine.resize(size_/sizeof(OCRLine));
	memcpy((char*)focalLine.dataPtr(),p,size_);

	st.getPtr(&p,4,&size_);
	starArray.init();
	starArray.resize(size_/sizeof(OCRStar));
	memcpy((char*)starArray.dataPtr(),p,size_);

	st.getPtr(&p,5,&size_);
	corrPointsArray.init();
	corrPointsArray.resize(size_/sizeof(OCRPoint));
	memcpy((char*)corrPointsArray.dataPtr(),p,size_);
	
	st.getPtr(&p,6,&size_);
	clusterArray.init();
	clusterArray.resize(size_/sizeof(OCRCluster));
	memcpy((char*)clusterArray.dataPtr(),p,size_);
	
	st.getPtr(&p,7,&size_);
	bites_data = (uchar *)malloc(size_);
	memcpy((char*)bites_data,p,size_);
	
	if(initLine_flag && st.size()>8){
		st.getPtr(&p,8,&size_);
		line_data = (OCRData *)malloc(size_);
		memcpy((char*)line_data,p,size_);
	}
	unpackImg32V(0);
	 
	hashTableReady = 0;
  initID_flag = 0;
  initID2_flag = 0;
  coord_fx = 0;
  coord_fy = 0;	
	BufS = 0;
	
}//___________________________________________________________________
    
void GGraph::init(int acolumns, int arows, int mode) {
    
    if(mode==BITMAP_32) {
        nrows=arows;                     //=(arows/32+1)*32;       // nrows=arows;
        if(acolumns%32==0){
            ncolumns=acolumns;
        }else{
            ncolumns=(acolumns/32 + 1) * 32;  // ncolumns=acolumns;
        }
        data_size=ncolumns * nrows;
        data_size_p=data_size/8;  /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
        data_size_p32=data_size/32;  /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
        bytes_per_row=ncolumns/8;
        border=0;
        if(!data_size) return;
        // инициализация байтового массива char размером  data_size
        bites_data=(unsigned char *)malloc(data_size_p);  // запрос памяти с очисткой 0
        memset(bites_data, 0, data_size_p);
        pack_flag=BITMAP_32;
    } else {
        colorMode=mode;
        // выравнивание bytes_data по 32
        // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
        // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
        nrows=arows;                                // nrows=(arows/32+1)*32;       // nrows=arows;
        if(acolumns%32==0){
            ncolumns=acolumns;
        }else{
            ncolumns=(acolumns/32 + 1) * 32;  // ncolumns=acolumns;
        }
        data_size=ncolumns * nrows * colorMode;
        if(!data_size) return;
        data_size_p=0;
        bytes_per_row=ncolumns * colorMode;
        border=0;
        
        // cout<<" data_size="<<data_size<<endl;
        // инициализация байтового массива char размером  data_size
        bytes_data=(unsigned char *)malloc(data_size);
        memset(bytes_data,255,data_size);
        pack_flag=0;
    }
    
}  //______________________________________________________________


void GGraph::printPoint(int x, int y) {
    DR_("\nprint point matrix x:"<<x<<" y:"<<y);
    uint w=ncolumns;
    unsigned char *p0=bytes_data + y * w + x;
    DR_("\n"<<*(p0)/255);
    DR_("   "<<*(p0 + 1)/255);
    DR_("   "<<*(p0 + 2)/255);
    DR_("\n"<<*(p0 + w)/255);
    DR_("   "<<*(p0 + w + 1)/255);
    DR_("   "<<*(p0 + w + 2)/255);
    DR_("\n"<<*(p0 + w + w)/255);
    DR_("   "<<*(p0 + w + w + 1)/255);
    DR_("   "<<*(p0 + w + w + 2)/255);
    DR_("\n________\n\n");

}  //____________________________________________________________

void GGraph::printPointS(int sl) {
    uint w=ncolumns;
    DR_("\nprint point matrix x:"<<sl % w<<" y:"<<sl/w);
    unsigned char *p0=bytes_data + sl;
    DR_("\n"<<*(p0)/255);
    DR_("   "<<*(p0 + 1)/255);
    DR_("   "<<*(p0 + 2)/255);
    DR_("\n"<<*(p0 + w)/255);
    DR_("   "<<*(p0 + w + 1)/255);
    DR_("   "<<*(p0 + w + 2)/255);
    DR_("\n"<<*(p0 + w + w)/255);
    DR_("   "<<*(p0 + w + w + 1)/255);
    DR_("   "<<*(p0 + w + w + 2)/255);
    DR_("\n________\n\n");

}  //____________________________________________________________

void GGraph::printPointP(unsigned char *p0) {
    uint w=ncolumns;
    DR_("\nprint point matrix");
    DR_("\n"<<*(p0)/255);
    DR_("   "<<*(p0 + 1)/255);
    DR_("   "<<*(p0 + 2)/255);
    DR_("\n"<<*(p0 + w)/255);
    DR_("   "<<*(p0 + w + 1)/255);
    DR_("   "<<*(p0 + w + 2)/255);
    DR_("\n"<<*(p0 + w + w)/255);
    DR_("   "<<*(p0 + w + w + 1)/255);
    DR_("   "<<*(p0 + w + w + 2)/255);
    DR_("\n________\n\n");

}  //____________________________________________________________
    
// УПАКОВКА  PACKING 32V //
/** упаковки данных в GBitmap, при котором в одном int32 записано 32 пиксела черно-белого изображения.
 GBitmap32 как и GBitsetMatrix в этом случае транспонирован по вертикали (x и y поменялись местами) и изображение записано в нем
 вертикальными колонками шириной по 32 пиксела*/

uchar* GGraph::packImg32V_(int w, int h, uchar *bytes_data_){
    
    
    int y,x,d;
    int print=0;
    
    // ncolumns ширина массива буквы (букв), nrows высота массива буквы (букв)
    //data_size=ncolumns*nrows;  //размер большого битмапа
    
    
    DR(END<<" TIME_START packImg32 /P32/ 0="<<0<<" w="<<w<<" h="<<h<<endl)
    
    uint w_h=(w * h);
    uint data_size_p_=w_h/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
    uint data_size_p32_=w_h/32-1; /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
    //минус единица потому что каждый слайс обрабатывается циклом от 32 до 0
    
    // инициализация битового массива char размером  data_size_p=data_siz/8
    uchar *bites_data_=(unsigned char*)malloc(data_size_p_+32);         // +ncolumns  // было +8 это ошибка
    // обнуление bites_data                                   // +ncolumns
    memset(bites_data_,0,data_size_p_+32);
    // выход в случае отсутствия свободной памяти запрашиваемого размера
    //if (bites_data==NULL) {DC("bites_data"); exit (1);}
    
    // приведенный к unsigned int исходный упакованный массив черно-белого скана
    unsigned int* bites_data_32=(unsigned int*)bites_data_;

    // Классический вариант //
    // Запаковка стандартного большого байтового массива битмап
    // (в одном байте 1 бит) в маленькй  массив int (в одном int 32 бита).
    // при упаковке массив транспонируется, данные располагаются колонками, а не рядами
    
    uint reg;
    uint stepX=0,stepY=0;
    int regLimit=31;
    if(w<31)regLimit=w;
    
    for( y=0; y < data_size_p32_; y++ ){
        reg=0;
        d=w*stepX+stepY;
        stepX++;
        if(stepX==h){stepX=0;stepY+=32;}
        for ( x=regLimit; x>=0; x--) {
            ///	reg=(reg << 1) + bytes_data[d+x]; // обратный порядок пикселей ///////////
            //reg=bytes_data[d+x] + (reg << 1); // обратный порядок пикселей
            reg=(bytes_data_[d + x]<<31) + (reg >> 1); // прямой порядок пикселей ///////////
            //reg=bytes_data[d + x] * 0x80000000 + (reg >> 1);  //прямой порядок пикселов iOs
        } // x      // <<31 умножение на 2 в 31 степени,  >> 1 деление на 2
        //binaryPrint(reg, 2); cout<<endl;
        bites_data_32[y]=reg;  // заполнение маленького выходного массива int
        
    } // y
    
    return bites_data_;
}//_____________________________________________________________________________

void GGraph::saveToTStr(TString &st){
	packImg32V(0);
	int data[10];
	data[0] = ncolumns;
	data[1] = nrows;
	data[2] = data_size;
	data[3] = data_size_p32;
	data[4] = pack_flag;
	data[5] = colorMode;
	data[6] = border;
	data[7] = bytes_per_row;
	st.push_back((char*)data,40);  //0
	st.push_back((char*)focalPoint.dataPtr(),(int)focalPoint.size() * sizeof(OCRPoint)); //1
	st.push_back((char*)pointArray.dataPtr(),(int)pointArray.size() * sizeof(OCRPoint)); //2
	st.push_back((char*)focalLine.dataPtr(),(int)focalLine.size() * sizeof(OCRLine)); //3
	st.push_back((char*)starArray.dataPtr(),(int)starArray.size() * sizeof(OCRStar));  //4
	st.push_back((char*)corrPointsArray.dataPtr(),(int)corrPointsArray.size() * sizeof(OCRPoint)); //5
	st.push_back((char*)clusterArray.dataPtr(),(int)clusterArray.size() * sizeof(OCRCluster)); //6
	uint data_size_p=data_size/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
  st.push_back((char*)bites_data,data_size_p+32); //7
  if(initLine_flag){
		st.push_back((char*)line_data,data_size * sizeof(uint)); //8
	}
	
}//______________________________________________________________________________

}  // namespace ocr
