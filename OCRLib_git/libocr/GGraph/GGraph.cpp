//
//  GraphOCR.cpp
//  OCRLib  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .


#include "GGraph.h" 
namespace ocr{

OCRLineRef::OCRLineRef(){
    index=-1;
    alpha=0;
    len=0;
}
    
    
OCRLineRef::OCRLineRef(OCRFocalLine &ref,uint len_){
    index=ref.index;
    alpha=ref.alpha;
    len=ref.len*100/len_;

}
    
    
//________OCRFocalLine_________________
    
OCRFocalLine::OCRFocalLine(){
    
    // характеристики line //
    maxCor=0;         // прямая-1, дуга-2, спираль-3, круг-4
    maxC=0;           // значение максимальной корреляции
    corL=0;           // коэффициент корреляции линии с прямой (0-100%)
    corA=0;           // коэффициент корреляции линии с дугой (0-100%)
    corS=0;           // коэффициент корреляции линии со спиралью (0-100%).
    dM=0;             // расстояние от центра масс до середины прямой соединяющей концы линии
    len=0;            // длина прямой соединяющей концы линии
    alpha=0;          // угол наклона линии в градусах (0 до 360° grade).
    angl_t=0;         // среднее значение тангенциального угла
    
    // характеристики circle //
    corC=0;         //коэффициент корреляции линии с окружностью (0-100%)
    radius=0;       //радиус круга в pix.    
    center.x=0;     //координаты центра окружности.
    center.y=0;     //координаты центра окружности.
    
    // характеристики кривой Безье //
    lenA=0;           // количество пикселов линии
    lenM=0;           // длина сглаженной фокальной линии.
    lenB=0;           // длина кривой Безье (вычислено по уравнению растеризации, медленно).
    lenBf=0;          // длина кривой Безье (вычислено рекурсиво и бысто).
    lenBff=0;         // приближенное значение длины кривой Безье (вычислено всего по 4 управляющим точкам).
    alphaP1=0;        // угол наклона первого контрольного вектора (0 до 360° grade).
    lenP1=0;          // длина первого контрольного вектора в pix
    alphaP2=0;        // угол наклона второго контрольного вектора (0 до 360° grade).
    lenP2=0;          // длина второго контрольного вектора в pix
    lenP1P2=0;        // расстояние от первой управляющей точки до второй управляющей точки в pix
    status=0;

    
}

//OCRFocalLine::~OCRFocalLine(){}
    
    
//Пересчет координат линии по отношению к центру буквы
void OCRFocalLine::setDimension(int dx,int dy){
    //data;  // для массивов адресов
    int c=(uint)data.size();
    for(int i=0;i<c;i++){
        data[i].x-=dx;
        data[i].y-=dy;
    }
    c=(uint)sData.size();
    //sData; // для массивов адресов интегральных сумм
    for(int i=0;i<c;i++){
        sData[i].x-=dx;
        sData[i].y-=dy;
    }
    //mData; // массив усредненных значений координат точек линии 
    c=(uint)mData.size();
    for(int i=0;i<c;i++){
        mData[i].x-=dx;
        mData[i].y-=dy;
    }
    
    start.x-=dx;       start.y-=dy;
    end.x-=dx;         end.y-=dy;
    center.x-=dx;      center.y-=dy;
    centerM.x-=dx;  centerM.y-=dy;
    p1.x-=dx;          p1.y-=dy;
    p2.x-=dx;          p2.y-=dy;
}

void OCRFocalLine::writeToStr(TString &st){
    int d1[4];         /// array of line int parameters
    float d2[11];         /// array of line parameters
    // характеристики line //
    d1[0]=index;        //id number
    d1[1]=alpInt;       // целочисленное значение угола наклона линии в градусах (0 до 360° grade).
    d1[2]=lenA;         // количество пикселов линии.
    d1[3]=maxCor;       // прямая-1, дуга-2, спираль-3, круг-4.

    // характеристики кривой Безье //
    d2[0]=len;          // длина прямой соединяющей концы линии.
    d2[1]=alpha;        // угол наклона линии в градусах (0 до 360° grade).
    d2[2]=lenM;         // длина сглаженной фокальной линии.
    d2[3]=lenP;         // длина укороченной кривой Безье (как часть длины сглаженной фокальной линии).
    d2[4]=lenB;         // длина кривой Безье (вычислено по уравнению растеризации, медленно).
    d2[5]=alphaP1;      // угол наклона первого контрольного вектора (0 до 360° grade).
    d2[6]=lenP1;        // длина первого контрольного вектора в pix.
    d2[7]=alphaP2;      // угол наклона второго контрольного вектора (0 до 360° grade).
    d2[8]=lenP2;        // длина второго контрольного вектора в pix.
    d2[9]=alphaP1P2;    // угол наклона вектора от первой управляющей точки до второй управляющей точки.
    d2[10]=lenP1P2;      // расстояние вектора от первой управляющей точки до второй управляющей точки в pix.
    
    string str;
    str.resize(8*sizeof(OCRPoint));
    OCRPoint *v=(OCRPoint*)&str[0];
    v[0]=start;
    v[1]=end;
    v[2]=center;
    v[3]=centerM;
    v[4]=p0;
    v[5]=p1;
    v[6]=p2;
    v[7]=p3;
    
    st.push_back((char*)d1,4*4);
    st.push_back((char*)d2,11*4);
    st.push_back(str);
    st.push_back((char*)&data[0],(uint)data.size()*(int)sizeof(OCRPoint));
    st.push_back((char*)&sData[0],(uint)sData.size()*(int)sizeof(OCRPoint));
    st.push_back((char*)&mData[0],(uint)mData.size()*(int)sizeof(OCRPoint));
    //st.push_back((char*)startL.dataPtr(),startL.size()*(int)sizeof(OCRLineRef));
    //st.push_back((char*)endL.dataPtr(),endL.size()*(int)sizeof(OCRLineRef));
    
}
void OCRFocalLine::readFromStr(TString &st){
    string str=st[0];
    int *d1=(int*)&str[0];
    string str1=st[1];
    float *d2=(float*)&str1[0];
    // характеристики line //
    index=d1[0];        //id number
    alpInt=d1[1];       // целочисленное значение угола наклона линии в градусах (0 до 360° grade).
    lenA=d1[2];         // количество пикселов линии.
    maxCor=d1[3];       // прямая-1, дуга-2, спираль-3, круг-4.
    
    // характеристики кривой Безье //
    len=d2[0];          // длина прямой соединяющей концы линии.
    alpha=d2[1];        // угол наклона линии в градусах (0 до 360° grade).
    lenM=d2[2];         // длина сглаженной фокальной линии.
    lenP=d2[3];         // длина укороченной кривой Безье (как часть длины сглаженной фокальной линии).
    lenB=d2[4];         // длина кривой Безье (вычислено по уравнению растеризации, медленно).
    alphaP1=d2[5];      // угол наклона первого контрольного вектора (0 до 360° grade).
    lenP1=d2[6];        // длина первого контрольного вектора в pix.
    alphaP2=d2[7];      // угол наклона второго контрольного вектора (0 до 360° grade).
    lenP2=d2[8];        // длина второго контрольного вектора в pix.
    alphaP1P2=d2[9];    // угол наклона вектора от первой управляющей точки до второй управляющей точки.
    lenP1P2=d2[10];      // расстояние вектора от первой управляющей точки до второй управляющей точки в pix.
    
    
    if(st.size()>2){
        string keyStr=st[2];
        OCRPoint *v=(OCRPoint*)&keyStr[0];
        start=v[0];
        end=v[1];
        center=v[2];
        centerM=v[3];
        p0=v[4];
        p1=v[5];
        p2=v[6];
        p3=v[7];
    }
    
    if(st.size()>3){
        string keyStr=st[3];
        data.resize(keyStr.size()/sizeof(OCRPoint));
        memcpy((char*)&data[0],(char*)&keyStr[0],keyStr.size());
        //data.push_back(keyStr);
    }
    if(st.size()>4){
        string keyStr=st[4];
        //sData.push_back(keyStr);
        sData.resize(keyStr.size()/sizeof(OCRPoint));
        memcpy((char*)&sData[0],(char*)&keyStr[0],keyStr.size());
    }
    if(st.size()>5){
        string keyStr=st[5];
        //mData.push_back(keyStr);
        mData.resize(keyStr.size()/sizeof(OCRPoint));
        memcpy((char*)&mData[0],(char*)&keyStr[0],keyStr.size());
    }
    /*
    if(st.size()>5){
        string keyStr=st[4];
        startL.push_back(keyStr);
    }
    if(st.size()>6){
        string keyStr=st[4];
        endL.push_back(keyStr);
    }
    */ 
    
}

void OCRFocalLine::printToScreen(){
    cout<<" index="<<index<<endl;
    cout<<" maxCor="<<maxCor<<endl;
    cout<<" maxC="<<maxC<<endl;
    cout<<" corL="<<corL<<endl;
    cout<<" corA="<<corA<<endl;
    cout<<" corS="<<corS<<endl;
    cout<<" dM="<<dM<<endl;
    cout<<" len="<<len<<endl;
    cout<<" alpha="<<alpha<<endl;
    cout<<" start.x="<<start.x<<" start.y="<<start.y<<endl;
    cout<<" end.x="<<end.x<<" end.y="<<end.y<<endl;
    cout<<" center.x="<<center.x<<" center.y="<<center.y<<endl;
    cout<<" p1.x="<<p1.x<<" p1.y="<<p1.y<<endl;
    cout<<" p2.x="<<p2.x<<" p2.y="<<p2.y<<endl;
    cout<<" data.size="<<data.size()<<endl;
    for(int i=0;i<data.size();i++)cout<<"("<<data[i].x<<","<<data[i].y<<")";
    cout<<endl<<"//________________________"<<endl<<endl;

}

GGraph::~GGraph(){
}


//____________________________________________________________________________

// ----- initialization
GGraph::GGraph(){
    colorMode = 1;
    nrows=0;
    ncolumns=0;
    data_size=0;
    pack_flag=0;
    border=0;
}
    
GGraph::GGraph(int acolumns, int arows){
    colorMode = 1;
    // выравнивание bytes_data по 32
    // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
    // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
    nrows = arows; //nrows = (arows/32+1)*32;       // nrows = arows;
    ncolumns =acolumns; //ncolumns =(acolumns/32+1)*32;  // ncolumns = acolumns;
    
    data_size=ncolumns*nrows*colorMode;
    if(!data_size)return;
    data_size_p=0;
    w=ncolumns;  h=nrows;  bytes_per_row=ncolumns*colorMode;
    border=0;
    
    //cout<<" data_size="<<data_size<<endl;
    // инициализация байтового массива char размером  data_size
    bytes_data=(unsigned char*)malloc(data_size);  // запрос памяти с очисткой 0
    
    fill(255);
    pack_flag=0;
}

GGraph::GGraph(int acolumns, int arows,int mode){
    if(mode==BITMAP_32){
        nrows = arows;// = (arows/32+1)*32;       // nrows = arows;
        ncolumns =acolumns;// =(acolumns/32+1)*32;  // ncolumns = acolumns;
        data_size=ncolumns*nrows;
        data_size_p=data_size/8;    /// размер упакованного массива байтов. 1 байт - 8 пикселов изображения
        data_size_p32=data_size/32; /// размер упакованного массива integer. 1 integer - 32 пиксела изображения
        w=ncolumns;  h=nrows;  bytes_per_row=ncolumns/8;
        border=0;
        if(!data_size)return;
        // инициализация байтового массива char размером  data_size
        bites_data=(unsigned char*)malloc(data_size_p);  // запрос памяти с очисткой 0
        memset(bites_data,0,data_size_p);
        pack_flag=BITMAP_32;
    }else{
        colorMode = mode;
        // выравнивание bytes_data по 32
        // добавляет к исходной картинке вправо и вниз бордюр размером в остаток
        // от деления сответственно: ncolumns на 32 (ncolumns%32),  nrows на 32 (nrows%32).
        nrows = arows; //nrows = (arows/32+1)*32;       // nrows = arows;
        ncolumns =acolumns; //ncolumns =(acolumns/32+1)*32;  // ncolumns = acolumns;
        
        data_size=ncolumns*nrows*colorMode;
        if(!data_size)return;
        data_size_p=0;
        w=ncolumns;  h=nrows;  bytes_per_row=ncolumns*colorMode;
        border=0;
        
        //cout<<" data_size="<<data_size<<endl;
        // инициализация байтового массива char размером  data_size
        bytes_data=(unsigned char*)malloc(data_size);  // запрос памяти с очисткой 0
        
        fill(255);
        pack_flag=0;
    }
    
}//____________________________________________________________________________


GGraph::GGraph(const GBitmap* ref){
    if (this != ref)
    {
        border=0;
        if(ref->pack_flag!=BITMAP_32){
            init(ref->columns(),ref->rows(),ref->colorMode);
            unsigned char *row = bytes_data;
            unsigned char *rowDest;
            for (int n=0; n<nrows; n++, row+=bytes_per_row){
                rowDest=ref->bytes_data+bytes_per_row*n;
                memcpy( (void*)row, (void*)rowDest,  ncolumns );
            }
        }else{
            init(ref->columns(),ref->rows(),BITMAP_32);
            memcpy( (void*)bites_data, (void*)ref->bites_data,  ncolumns*nrows/8 );
        }
    }
    
}//____________________________________________________________________________


/** @fn void GGraph::initRegion(const GBitmap* ref, int x0,int y0, int w, int h)
 @brief функция инициализации GBitmap регионом графического файла ref по заданным  координатам
 */
GGraph::GGraph(const GBitmap* ref, int x0,int y0, int w, int h){
    //cout_<<"start"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
    data_size=0;
    pack_flag=0;
    //normalisation
    if(x0<0)x0=0;
    if(y0<0)y0=0;
    if(x0>ref[0].columns())x0=0;
    if(y0>ref[0].rows())y0=0;
    if(w+x0>ref[0].columns())w=ref[0].columns()-x0;
    if(h+y0>ref[0].rows())h=ref[0].rows()-y0;
    unsigned char *p;
    if (this != ref)
    {
        init(w,h,ref->colorMode); //после инициализации битмап выровнен по 8
        //cout_<<"start1"<<" x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
        int x,y;      // ( переменная d, 7 ноября 2010, быстрее классической).
        unsigned char *d;
        for (y = 0; y < h; y++){
            d=bytes_data+y*ncolumns;
            for (x = 0; x < ncolumns; x++){
                p=d+x;
                *p=ref[0][y0+y][x0+x];
                //if(  pImg[0][y][x]>150){
                //   DS(0);
                //}else{
                //   DS(1);
                //};
            }//DS(END);
        }
    }
    //cout_<<"done read"<<endl;
}//_____________________________________________________________________________


GGraph::GGraph(const GBitmap* ref, float scale, float rotation,int mode){
    //cout << "@@@_____scale="<<scale<<endl;
    if (this != ref){
        pack_flag=0;
        
        if(scale!=0&&rotation!=0){
            rotateFast(ref,scale,rotation);
            return;
        }
        if(scale!=0&&mode==0){
            scaleFast(ref,scale);
            return;
        }
        if(scale!=0&&mode==1){
            bicubicScale(ref,scale);
            return;
        }
        if(rotation!=0){
            
            rotateFast(ref,1,rotation);
            return;
        }
        init(ref);
        
    }
    
}//_____________________________________________________________________________


GGraph::GGraph(string &inputData,int invert_){
    // cout << "@@@_____init new GBitmap from Tiff string="<<endl;
    
    //int arows,acolumns;
    nrows=0;ncolumns=0;data_size=0;pack_flag=0;
    
    //readTIFF(path,invert,&bites_data,&acolumns,&arows);
    //readTIFFFromString(invert_,&bites_data,&ncolumns,&nrows);
    
    if(!ncolumns||!nrows)return;
    border = 0;
    bytes_per_row = ncolumns;
    
    /*
     for(int i=0;i<nrows;i++){
     for(int j=0;j<ncolumns/8;j++){
     if(bites_data[i*(ncolumns/8)+j]){cout_<<".";}else{cout_<<"0";}
     }cout_<<endl;
     }
     */
    
    data_size=nrows*ncolumns;
    data_size_p=0;
    pack_flag=1;
    unpackImg(1);
    if(invert_)invert();
    
}//_____________________________________________________________________________


}
