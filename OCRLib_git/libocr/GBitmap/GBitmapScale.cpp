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

#include "GBitmap.h"



//*****************************************************************************
namespace ocr{
    
    
    ///______Bicubic_function________________________________

/** маштабирование изменяет содержимое исходного изображения */  
        
void GBitmap::bicubicScale(float scale){
        GBitmap *ref=GBitmap::create(this);
        if(data_size){  
            if(!pack_flag){
                free(bytes_data);
            }else{
                free(bites_data);
            }
        }
        bicubicScale(ref,scale);
        ref->destroy(); 
    
} //_____________________________________________________________________________
	
        
// Здесь должен быть бикубик на "С" c нормализацией по размеру ncolumns кратным 32 (на несколько процентов изменятся пропорции текста).
    
/** масштабирование (Resample) серого (char) массива по методу "ближайших соседей" */
void GBitmap::bicubicScale(const GBitmap*ref, float scale){
        
        int x,y,yp;
        int w=ref->columns();    //cout_<<"w="<<w;  /// ширина массива
        int h=ref->rows();       //cout_<<" h="<<h; /// высота массива
        int new_p;             ///  пиксел, бижайший пиксел
        unsigned char *d0;
        //scale=1.33;
        //TIME_START
        // float scale=0.536;   масштаб нового массива float, ипи double
        if (scale>5){scale=(float)5;}   if (scale<0.2){scale=(float)0.2;} // ограничения на масштаб
        int scaleP=256*scale;
        // инициализация нового (маштабированного) битового массива bytes_data
        // Get a new buffer to interpolate into bool Resample   http://www.cplusplus.com/forum/general/2615/
        init(w*scale,h*scale,ref->colorMode);
        
        //cout_<<"init(), Resample (scale), New version V0 new_w="<<new_w<<" new_h="<<new_h<<" w="<<w<<" h="<<h<<endl;
        
        unsigned char *p0=bytes_data;
        unsigned char *p1=ref->bytes_data;
        
        int w_h=w*h-1;  // int new_h_new_w=nrows*ncolumns-1; // ( переменнне, 7 ноября 2010, быстрее классической).
        for(y = 0; y < nrows; y++) { //cout_<<y<<" ";        // ( упрощена, 25 ноября 2010, не много быстрее ).
            yp=((y<<8)/scaleP)*w;                            // *256
            d0=p0+y*ncolumns;
            for(x = 0; x < ncolumns; x++) {                  //p=y*new_w + x;
                //new_p=( ((int)(y/scale)*w) + ((int)(x/scale)) ); // медленние
                new_p=(x<<8)/scaleP + yp;
                //  разный масштаб по разным координатам
                /// new_p=( ((int)(y/scale_h)*w) + ((int)(x/scale_w)) );
                /// if(new_p>=w*h){continue;} if(p + x>=new_h*new_w){continue;}
                if(new_p>w_h){break;}
                *(d0 + x)=*(p1+new_p);
                // вывод на экран
                ////d=new_data + p;     if(*d>127){cout_<<"1";}else{cout_<<".";}
            } ////cout_<<endl;
        }
        
        //TIME_PRINT_
        //cout_<<"done" ;
        /**/
} //_______________________________________________________________________________
    
    
/** маштабирование изменяет содержимое исходного изображения */ 
    
void GBitmap::bilinearScale(float scale){
    if(scale==0 || scale==1 )return;
    GBitmap *ref=GBitmap::create(this);
    if(data_size){  
        if(!pack_flag){
            free(bytes_data);
        }else{
            free(bites_data);
        }
    }
    bilinearScale(ref,scale);
    ref->destroy();
    
} //_____________________________________________________________________________
    
    // ГОТОВАЯ version
    
/** масштабирование (Resample) серого (uchar) массива по методу "билинейной интерполяции" */
    
void GBitmap::bilinearScale(GBitmap *ref, float scale) {
    
    // Bilinear Image Scaling // Билинейное масштабирование изображения //
    // http://tech-algorithm.com/articles/bilinear-image-scaling/
    
    //cout<<"Билинейное масштабирование серого изображения bicubicScale()   scale="<<scale<<endl;
    
    // ограничения на масштаб
    if (scale>5){scale=(float)5;}   if (scale<0.2){scale=(float)0.2;} 
    
    int x,y; 
    int w=ref->columns();  //cout_<<"w="<<w;   // исходная ширина массива
    int h=ref->rows();     //cout_<<" h="<<h;  // исходная высота массива
    
    int w2=w*scale;        // новая ширина изображения
    int h2=h*scale;        // новая высота изображения
    init(w2,h2,ref->colorMode);
    unsigned char *p0=bytes_data;       // указатель на массив пикселей изображения, которое необходимо увеличить
    unsigned char *p1=ref->bytes_data;  // указатель на массив пикселей масштабированного изображения
    //cout<<"w2="<<w2<<" w="<<w<<" h2="<<h2<<" h="<<h<<endl;
    
        int A, B, C, D, index, gray;
        float x_ratio = 1/scale;  // ((float)(w-1))/w2;
        float y_ratio = 1/scale;  // ((float)(h-1))/h2;
        float x_diff, y_diff;        
        int y_w;
    
//TIME_START // time=1.00  scale=1.055  w=16224 × h=2464
    
    h2-=3;   // algorithm border

    for (int i=0; i < h2; i++) { // cout<<" y"<<i<<" y_="<<(int)(y_ratio * i)<<endl;
             
             y = (int)(y_ratio*i);
             y_diff = (y_ratio*i) - y;
             y_w = y*w;
        
             for (int j=0; j < w2; j++) {
                 
                x = (int)(x_ratio*j);  
                x_diff = (x_ratio*j) - x;
                index = y_w + x;
                
                // range is 0 to 255 thus bitwise AND with 0xff
                A = p1[index] & 0xff;
                B = p1[index+1] & 0xff;
                C = p1[index+w] & 0xff;
                D = p1[index+w+1] & 0xff;
                
                // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
                gray = (uchar)( A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                                C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)  );                                                                          
                  
                *p0=gray; p0++; // bytes_data[offset++] = gray;
                 
             } // for j
    } // for i
    
//TIME_PRINT_

}; //_____________________________________________________________________________
    
    
    // ++++++
    // ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian), попытка минимальной реализации.
    // Адаптивный F(KHig) нолевой бордюр вокруг массивов.
    // 2 массива
    // -----------------------------------------------------------------------------
	// треугольная весовая функция
	// y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)
    
    /*
    void OCRFilter::filterHigh(vector<int> &In, int KHig){
        filterHigh(&In[0],(uint)In.size(),KHig);
    }//_____________________________________________________________________________
    
    void OCRFilter::filterHigh(int* In,uint size, int KHig){
    */  
    
 // -----------------------------------------------------------------------------        
////void  GBitmap::filterHighM(int KHig) {
/*        
        int y,Khg;
        //if(size>ncnr) return;
        
        //////////////////////////////////////////////  всего 2 массива
        
        if ( KHig<2 ) KHig=1;   if ( KHig>80 ) KHig=80;   // if ( KHig>63 ) KHig=63;
        // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
        //      129           257             385
        int  s2=KHig*2+1,  s4=KHig*4;//    s6=KHig*6+1;
        
        // vector<int> BufIn(size+s6);
        // vector<int> lt1(size+s6);
        // int BufIn[10000]={0};
        // int lt1[10000]={0};
*/
    
        /*
        // обнуление ImBuf,ReBuf ( где ImBuf, ReBuf глобальные )
        memset(ImBuf,0,4*(ncnr+512));      memset(ReBuf,0,4*(ncnr+512));
        //for ( y=0; y < size+s6; y++ ){BufIn[y]=lt1[y]=0;}
        
        // Входной буфер BufIn[y+s2], In[y]
        for ( y=0; y < size; y++ ){ImBuf[y+s2]=In[y];}
        Khg=KHig*KHig;
        for ( y=s2; y < size+s4; y++ ) { //треугольная весовая функция, прямая фильтрация
            ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+ImBuf[y-KHig-KHig]-2*ImBuf[y-KHig]+ImBuf[y]; //k=15+
        }
        for ( y=s2; y < size+s4; y++ ) { ReBuf[y]=ReBuf[y]/Khg;  ImBuf[y]=0; } // Norm // y=0; size+s6;
        // memset(ImBuf,0,4*(ncnr+512));
        for ( y=size+s4; y > s2; y-- ) { //треугольная весовая функция, инверсная фильтрация
            ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+ReBuf[y+KHig+KHig]-2*ReBuf[y+KHig]+ReBuf[y];//k=15-
        }
        for ( y=s2; y < size+s2; y++ ) { In[y-s2]=ImBuf[y]/Khg; }
        // Выходной буфер In[y-s2]
        */
        
////} // -----------------------------------------------------------------------------
    
    
    //////////////////////////////////////////////  применен адаптивный бордюр Khg8
    
    /*
    if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
        // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
        //   129             257             385              514
        int  Khg2=KHig*2+1,  Khg4=KHig*4+1,  Khg6=KHig*6+1,   Khg8=KHig*8+2;
        
        vector<int> BufIn(size+Khg8);
        vector<int> lt1(size+Khg8);
        vector<int> BufOut1(size+Khg8);
        
        // Входной буфер BufIn[y+Khg4], In[y]
        for ( y=0; y < size; y++ ){BufIn[y+Khg4]=In[y];}
    Khg=KHig*KHig;
    for ( y=Khg2; y < size+Khg6; y++ ) { //треугольная весовая функция, прямая фильтрация
        lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
    }
    for ( y=0; y < size+Khg8; y++ ) { lt1[y]=lt1[y]/Khg; }         //Norm
    for ( y=size+Khg6; y > Khg2; y--) { //треугольная весовая функция, инверсная фильтрация
        BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
    }
    for ( y=Khg4; y < size+Khg4; y++ ){ In[y-Khg4]=BufOut1[y]/Khg; }
    // Выходной буфер In[y-Khg4, BufOut1[y]
    */
    
    
} // namespace ocr 
// -----------------------------------------------------------------------------
