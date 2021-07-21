//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-

#include "OCRAlgorithm.h"
#include "../OCRString/php2stl.h"
#include "popcntintrin.h" /////////////////
#include <smmintrin.h>

namespace ocr{

// ----- constructor and destructor
//________OCRFilter_________________
OCRFilter::~OCRFilter(){}

OCRFilter::OCRFilter(int size){
	szf=sizeof(int);            // szf=4;
	ncnr=size;     ///<создаем размерность буферных массивов

	ImBuf=(int*)malloc((ncnr+512+16)*szf);
	ReBuf=(int*)malloc((ncnr+512+16)*szf);
	BufIn=(int*)malloc((ncnr+512+16)*szf);
	BufOu=(int*)malloc((ncnr+512+16)*szf);
///////////////////////////////////////

  // выход в случае отсутствия свободной памяти запрашиваемого размера
  if (ImBuf==NULL) {DC("ImBuf");exit (1);}
  if (ReBuf==NULL) {DC("ReBuf");exit (1);}
  if (BufIn==NULL) {DC("BufIn");exit (1);}
  if (BufOu==NULL) {DC("BufOu");exit (1);}
}


//-------------------------------АЛГОРИТМЫ------------------------------------//
    
      // прямоугольная весовая функция
      // y(n)= y(n-1)+x(n)-x(n-2k-1)
      // треугольная весовая функция
      // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)

// -----------------------------------------------------------------------------
// ++++++
// ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian), попытка минимальной реализации.
// Адаптивный F(KHig) нолевой бордюр вокруг массивов.
// 2 массива
// -----------------------------------------------------------------------------
	// треугольная весовая функция
	// y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)

 void OCRFilter::filterHigh(vector<int> &In, int KHig){
     filterHigh(&In[0],(uint)In.size(),KHig);
}//_____________________________________________________________________________

void OCRFilter::filterHigh(int* In,uint size, int KHig){
    
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
    /**/
 
}
// -----------------------------------------------------------------------------

// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.
// Входной буфер &In. Адаптивный F(KHig) нолевой бордюр вокруг массивов
// 4 массива
// -----------------------------------------------------------------------------

 void OCRFilter::filterLaplasian( vector<int> &In, int KHig, int KLow){  // filterLaplasianM1
 int y,Khg,Klw,S;
 int size=(int)In.size();

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   SN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;

///////////////

// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
		memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512));
		memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));


	for ( y=0; y < size; y++ ){BufIn[y+S4]=In[y];} // Входной буфер Buf In[y]

	Khg=KHig*KHig; Klw=KLow*KLow;
	for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
		}
	for ( y=0; y < size+S8; y++ )  { // Norm
		BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
		}
	for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
		}
	for ( y=S4; y < size+S4; y++ ){ In[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
	// Выход In[y-S4]
/**/

KHig=KHigh; // заплатка - запрет на модификацию KHig

///////////////
/*
 vector<int> BufIn(size+S8);   // 514
 vector<int> BufOut1(size+S8);
 vector<int> BufOut2(size+S8);
 vector<int> lt1(size+S8);
 vector<int> lt2(size+S8);

	for ( y=0; y < size; y++ ){BufIn[y+S4]=In[y];} // Входной буфер Buf In[y]

	  Khg=KHig*KHig; Klw=KLow*KLow;
	  for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		 lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
		 lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
	  }
	  for ( y=0; y < size+S8; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
	  for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		 BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
		 BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
	  }
	  for ( y=S4; y < size+S4; y++ ){ In[y-S4]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
	  // Выход In[y-S4]
*/

// DM(size<<" sizeG ");  DM(END);

/*
	BufIn.assign(siz,0); BufOut1.assign(siz,0); BufOut2.assign(siz,0);
	lt1.assign(siz,0);   lt2.assign(siz,0);
*/
}//_____________________________________________________________________________



 // -----------------------------------------------------------------------------

// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.
// Входной буфер &HBuf, &GBuf. Адаптивный F(KHig) нолевой бордюр вокруг массивов
// 4 массива
// -----------------------------------------------------------------------------
 void OCRFilter::filterLaplasian(vector<int> &HBuf,
								 vector<int> &GBuf,
								 int KHig, int KLow){

 int y,Khg,Klw,S;
 int size=(int)HBuf.size()-10; //-10 компенсация только для GBitset

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   KhgN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;

 ///////////////

// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
		memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512));
		memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));
		GBuf.assign(ncnr,0);

//    for ( y=0; y < size+S8; y++ )  { BufIn[y]=BufOu[y]=ReBuf[y]=ImBuf[y]=0; }

	for ( y=0; y < size; y++ ){BufIn[y+S4]=HBuf[y];} // Входной буфер Buf HBuf[y]
	Khg=KHig*KHig; Klw=KLow*KLow;
	for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
		}
	for ( y=0; y < size+S8; y++ )  { // Norm
		BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
		}

	for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
		}
	for ( y=S4; y < size+S4; y++ ){ GBuf[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
	// Выход GBuf[y-S4]


 /**/

KHig=KHigh; // заплатка - запрет на модификацию KHig

 ///////////////

/*
 vector<int> BufIn(size+S8);   // 514
 vector<int> BufOut1(size+S8);
 vector<int> BufOut2(size+S8);
 vector<int> lt1(size+S8);
 vector<int> lt2(size+S8);


	for ( y=0; y < size; y++ ){BufIn[y+S4]=HBuf[y];} // Входной буфер HBuf[y]
													 // Входной буфер In[y]
	  Khg=KHig*KHig; Klw=KLow*KLow;
	  for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		 lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
		 lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
	  }
	  for ( y=0; y < size+S8; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
	  for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		 BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
		 BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
	  }
	  for ( y=S4; y < size+S4; y++ ){ GBuf[y-S4]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
	  // Выход GBuf   // Выход In[y-S4]
*/

//DM(size<<" sizeG ");  DM(END);
/*
 vector<int> BufIn(size+514);
 vector<int> BufOut1(size+514);
 vector<int> BufOut2(size+514);
 vector<int> lt1(size+514);
 vector<int> lt2(size+514);

   for ( y=0; y < size; y++ ){BufIn[y+257]=HBuf[y];} // Входной буфер BufIn[y]

	  Khg=KHig*KHig; Klw=KLow*KLow;
	  for ( y=129; y < size+385; y++ ) { //треугольная весовая функция, прямая фильтрация
		 lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
		 lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
	  }
	  for ( y=0; y < size+514; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
	  for ( y=size+385; y > 129; y--) { //треугольная весовая функция, инверсная фильтрация
		 BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
		 BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
	  }
	  for ( y=257; y < size+257; y++ ){ GBuf[y-257]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
	  // Выход In[y-257]
*/
/*
	BufIn.assign(siz,0); BufOut1.assign(siz,0); BufOut2.assign(siz,0);
	lt1.assign(siz,0);   lt2.assign(siz,0);
*/


}//_____________________________________________________________________________



// -------------------------------_ST_------------------------------------------

// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.
// Входной ST буфер *BufS, *BufTL. Адаптивный F(KHig) нолевой бордюр вокруг массивов
// 4 ST массива

// -----------------------------------------------------------------------------


 void OCRFilter::filterLaplasian( int *Buf1, int *BufTL, int size, int KHig, int KLow ){

 int y,Klw,Khg,S;

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   KhgN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;


// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
		memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512)); // szf=4;
		memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));
		memset(BufTL,0,4*(size+32 ));

	for ( y=0; y < size; y++ ){BufIn[y+S4]=Buf1[y];} // Входной буфер Buf Buf1[y]
	Khg=KHig*KHig; Klw=KLow*KLow;
	for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
		}
	for ( y=0; y < size+S8; y++ )  { // Norm
		BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
		}
	for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
		}
	for ( y=S4; y < size+S4; y++ ){ BufTL[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
	// Выход BufTL[y-S4]
/**/

KHig=KHigh; // заплатка - запрет на модификацию KHig


}//_____________________________________________________________________________


/**\brief функция подсчета количества единиц в 32 разрядном слове (без _mm_popcnt_u32) */
    int SumBit32(int Sb){
        
       // binaryPrint(Sb, 32); cout<<endl;
        if(Sb != 0) {                                           // 0xFFFFFFFF
            // Подсчет количество единиц в регистре
            Sb-=(Sb >> 1) & 0x55555555;                         // 2-битовые ячейки
            Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333);    // 4-битовые
            Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                   // 8-битоовые
            Sb+=Sb >> 8;                                        // 16-битовые
            Sb+=Sb >> 16;                                       // 32-битовая ячейка
            Sb=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
        } // if
        
        //cout<<"Sb="<<Sb<<endl;        
        return Sb; // возврощаем количество единиц в 32 разрядном слове
        
    }//_____________________________________________________________________________

    
    /** @brief функция определения площади пересечения двух OCRBox. */
    void  intArea(OCRBox &a, OCRBox &b,OCRBox &dataBox){
        int print=0;
        
        int y0_=0,y1_=0,x0_=0,x1_=0;
        int iy0=a.y0,iy1=a.y1,ix0=a.x0,ix1=a.x1;
        int jy0=b.y0,jy1=b.y1,jx0=b.x0,jx1=b.x1;
        
        if(iy0>=jy0 && iy0<=jy1) y0_=iy0;
        if(jy0>=iy0 && jy0<=iy1) y0_=jy0;
        
        if(iy1<=jy1 && iy1>=jy0) y1_=iy1;
        if(jy1<=iy1 && jy1>=iy0) y1_=jy1;
        
        if(ix0>=jx0 && ix0<=jx1) x0_=ix0;
        if(jx0>=ix0 && jx0<=ix1) x0_=jx0;
        
        if(ix1<=jx1 && ix1>=jx0) x1_=ix1;
        if(jx1<=ix1 && jx1>=ix0) x1_=jx1;
        
        DR(" y0_="<<y0_<<" y1_="<<y1_<<" x0_="<<x0_<<" x1_="<<x1_<<endl)
        DR(" iy0="<<iy0<<" iy1="<<iy1<<" ix0="<<ix0<<" ix1="<<ix1<<endl)
        DR(" jy0="<<jy0<<" jy1="<<jy1<<" jx0="<<jx0<<" jx1="<<jx1<<endl)
        
        if(y0_==0||y1_==0||x0_==0||x1_==0){
            DR("no intersection"<<endl)
            dataBox.area=0;
            return;
        }
        int s_=(x1_-x0_)*(y1_-y0_);
        
        dataBox.area=s_;
        dataBox.x0=x0_;
        dataBox.x1=x1_;
        dataBox.y0=y0_;
        dataBox.y1=y1_;
        
        return;
  }//__________________________________________________________________
    
    
    
    // 21? июня 2021. Проверено. Рабочий код.
    // Функция предназначена для растискивания единиц в 64 (32) разрядном регистре ulong (uint). dotGain - полная ширина области растискивания
    // Функция предвычисляется.
ulong dotGainReg(ulong rg0, int dotGain) {

    // Функция получает на вход регистр rg0 с единицами, которые растискиваем и ширину растискивания в битах dotGain.
    // Функция возвращает регистр rg1 в котором находится результат растискивания (растиснутые единици).
    // dotGain - полная ширина области растискивания
    // для симметрии ширину растискивания dotGain лучше выбирать не четной.
    // Четные числа растискивания dotGain округляются до ближайшего большего не четного числа,
    // т.е. вместо dotGain=8 растискивания будет выполнено на 9.

    int GrafT=1; // int
    // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT)  cout<<endl<<"// Функция для растискивания единиц в 64 разрядном регистре ulong."<<endl;
        
    // Вычисление //
    
    // Растискивание единиц в 64 разрядном регистре ulong.
    // регистр единиц который растискиваем. Правильная запись констант 1UL в регистр ulong.
    //ulong rg0=1UL<<10;          // поисковый регистр углов фокальных линий, тест 1<<10.
    // регистр в котором находится результат растискивания.
    ulong rg1=0;                // регистр маски лимитов углов фокальных линий
    /**/
    /*
     // Растискивание единиц в 32 разрядном регистре uint.
     // регистр единиц который растискиваем.
     uint rg0=1<<10;             // поисковый регистр углов фокальных линий, тест 1<<10.
     // регистр в котором находится результат растискивания.
     uint rg1=0;                 // регистр маски лимитов углов фокальных линий
     */
        
    int s_rg=8 * sizeof(rg0);  // длинна регистра в битах // 64 // uint64_t // ULL.
    ///cout<<"   s_rg="<<s_rg<<endl;
    rg1=rg0;
    
        
    /*
     // ТЕСТ
     // Растискивание на 5 бита вправо и 5 бита влево, с циклическим сдвигом. 11 подряд идущих единиц.
     rg1=rg1 | ((rg1<<1) | (rg1>>(s_rg-1)));     // Растискивание на 1 бит влево  (*). +1=2
     rg1=rg1 | ((rg1>>2) | (rg1<<(s_rg-2)));     // Растискивание на 2 бит вправо (/). +2=4
     rg1=rg1 | ((rg1<<4) | (rg1>>(s_rg-4)));     // Растискивание на 4 бит влево  (*). +4=8
     rg1=rg1 | ((rg1>>3) | (rg1<<(s_rg-3)));     // Растискивание на 3 бит вправо (/). +3=11
     */
        
    // Вычисление dotGain2 как ближайшей степени двойки dotGain, причем dotGain2 >= dotGain,
    // например если dotGain=1035 то dotGain2=2048. Источник Радон.
    //int dotGain=11;
    if (dotGain<2) dotGain=2; // ограничения на ширину растискивания.
    int dotGain2=1;    // ближайшея степень двойки числа dotGain, причем dotGain2 >= dotGain
    int dg=0;          // степень в которую надо возвести 2 чтобы получить числа dotGain2.
    
    while( dotGain2<dotGain ){ dotGain2<<=1; dg++; }  // <<1 умножение на 2.
    if (GrafT) {cout<<"   rg0="<<rg0<<"   dotGain="<<dotGain<<"  dotGain2="<<dotGain2<<"  dg="<<dg<<endl;}
    
    int gn=1;       // текущая величина растискивания в цикле.
    int dgSum=0;    // суммарная величина растискивания в цикле по степени двойки.
    
    // Цикл растискивания.
    for (int x=0; x<dg-2; x++){
        rg1=rg1 | ((rg1<<gn) | (rg1>>(s_rg-gn))); // Растискивание на gn бит влево  (*).
        ///cout<<"   gn="<<gn<<"   dgSum="<<dgSum<<endl;
        dgSum+=gn;
        // растискиваем по степеням двойки.
        gn<<=1; // текущая величина растискивания для dotGain=11   1, 2. // <<1 это *2.
    }
    
    // растискивания в предпоследним цикле (остаток gn=2, для dotGain=11).
    gn=dotGain/2-dgSum;
    rg1=rg1 | ((rg1<<gn) | (rg1>>(s_rg-gn))); // Растискивание на gn бит влево  (*).
    ///cout<<"   gn="<<gn<<endl;
    
    // окончательная величина растискивания в последним цикле (остаток gn=5, для dotGain=11).
    gn=dotGain/2;
    rg1=rg1 | ((rg1>>gn) | (rg1<<(s_rg-gn))); // Растискивание на gn бит вправо (/).
    ///cout<<"   gn="<<gn<<endl;
    /**/
        
        
    // Визуализация //
    
    if (GrafT) {
        // Вывод значения s_rg=64 р регистра rg0 в двоичном коде.
        // На 64 р програмисткий калькулятор не работает.
        ulong rg=rg0; // ulong, uint64_t
        for (int x=0; x<s_rg; x++){ // 64
            int r=rg%2; // остаток от деления на 2.
            rg>>=1;     // rg=rg/2;
            //cout<<"   n="<<x<<"  "<<r<<endl;  // вертикальный вывод.
            cout<<""<<r;  // горизонтальный вывод.
        }
        cout<<endl;
        // Вывод значения s_rg=64 р регистра rg1 в двоичном коде.
        rg=rg1; // ulong, uint64_t
        for (int x=0; x<s_rg; x++){ // rg<1
            int r=rg%2; // остаток от деления на 2.
            rg>>=1;     // rg=rg/2;
            //cout<<"   n="<<x<<"  "<<r<<endl;  // вертикальный вывод.
            cout<<""<<r;  // горизонтальный вывод.
        }
        // https://programforyou.ru/calculators/number-systems
        
        // подсчет единиц аппаратным алгоритмом popcnt для удобства отладки.
        uchar Sum_Bit;
        if (s_rg==64)
            Sum_Bit=_mm_popcnt_u64(rg1);
        else
            Sum_Bit=_mm_popcnt_u32((uint)rg1);
        cout<<endl<<"   сумма единиц  Sum_Bit="<<(int)Sum_Bit<<endl<<endl;
    }
    // Возвращаем регистр rg1 в котором находится результат растискивания (растиснутые единици).
    return rg1;
        
}//_____________________________________________________________________________
        


// Sorting  сортировка

// ---------------------------------------------------------------------------

void Sort(vector<int> &GBuf,int Nff){
 // сортировка
   int trash=0;
   bool f=true;
   int i,j;

   //  DM(NSort<<" NSort+ ");
   if( Nff==0) { return; }
  /*
#ifdef DEBUGLVL
              DM(END);  DM(END);
			  for ( x=0; x < NSort;  x++ ){      // x < d+1;
				DM(GBuf[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
                  }//for x
			  DM(END);
#endif
  */
				 // "сортировка Шейкера".
				 // http://www.abc-it.lv/index.php/id/610

	 for ( i=1; (i<=Nff) && (f=true) ; i++)
	   {
          f=false;
		  for ( j=i; j<=Nff-i; j++) // проходим с лева на право
             {
                if (GBuf[j]>GBuf [j+1]) // если число слева больше числа
			   {
                  trash=GBuf[j]; // справа, то меняем местами
                  GBuf[j]=GBuf [j+1]; // справа собираются большие числа
				  GBuf[j+1]=trash;
                  f=true;
               }
			 }
			   for ( j=Nff-i-1; j>i ; j--)  // проходим с права на лево
                 {
				   if (GBuf[j]<GBuf[j-1])  // если число справа меньше числа
                   {
                   trash=GBuf[j]; // слева, то меняем местами
				   GBuf[j]=GBuf[j-1]; // слева собираются меньшие числа
                   GBuf[j-1]=trash;
                   f=true;
				 }
				 }
			 }
/*
#ifdef DEBUGLVL
			  DM(END);  DM(END);
			  for ( x=0; x < NSort;  x++ ){      // x < d+1;
				DM(GBuf[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				  }//for x
			  DM(END);
#endif
  */
}//_____________________________________________________________________________

    
    
// округление числа до ближайшей степени двойки в большую сторону // power of two
    
// ---------------------------------------------------------------------------
    
void power_Two(unsigned int* PowerTwo){ 
        
    // округление числа находящегося по ссылке "PowerTwo" до ближайшей степени двойки в большую сторону // power of two
        
    unsigned int n;
    n=*PowerTwo; 
    
    // например если n=500 то n=512
    unsigned int w1=1;       // unsigned int it=0; // it - степень округленного числа n      
    while( w1<n ){ w1<<=1; } // it++;   // <<1 умножение на 2         
///    cout<<"n="<<n;           // при n=500
    n=w1;
///    cout<<"   округление числа n до ближайшей степени двойки в большую сторону  n="<<n<<endl; // n=512 // cout<<"it="<<it<<endl; // it=9      
  
    // возврат округленного числа находящегося по ссылке "PowerTwo"
    *PowerTwo = n;
    
    
    
    // пример вызова функции
    /*
    unsigned int text_size=500;
    power_Two(&text_size);
    cout<<"text_size="<<text_size;
    cout<<"   округление числа text_size до ближайшей степени двойки в большую сторону  power_Two(text_size)="<<text_size<<endl;
    */ 
    
}// --------------------------------------------------------------------------- 
/* */    

bool isNearP(OCRPointFloat p1, OCRPointFloat p2, int d){
    //if(p2.x==p1.x or p2.y==p1.y)return 0; //near but not the same
    if(p2.x>p1.x+d or p2.y>p1.y+d or p2.x<p1.x-d or p2.y<p1.y-d)return 0;
    return 1;
}
    
    // Тестовая функция с максимумом для Function_Lion()
    float  Cor(float x) {
        
        // парабола: перевернутая в низ рогами, сдвинутая в право на 50 и в верх на 100
        // форму кривой можно увидеть в программе Grapher, подставив ф-лу y=-(x-50)*(x-50)/25 + 100;
        x=-(x-50)*(x-50)/25 + 100; // на выходе получаем 100 при x=50 и 0 при x=0 и x=100
        
        
        return x;
        
    }//____________________________________________________________________________

    
// Функция для тестирования поиска максимума функции (Лев в пустыне).
void Function_Lion() {
    
    // Работоспособное решение задачи нахождение максимума функции Cor()
    // в пределах значения аргумента от Lf до Rg. Москва 8 авг.
    
    // Принцип работы поиска триадами.
    // Этот метод используют для поиска максимумов и минимумов функции, которая либо сначала
    // строго возрастает, затем строго убывает, либо наоборот. Троичный поиск определяет,
    // что минимум или максимум не может лежать либо в первой, либо в последней трети
    // области, и затем повторяет поиск на оставшихся двух третях.
    // Для Delta=1 и тестовой функции в виде параболы достаточно 9 итераций
    // MaxCor=99.9909  ArgCor=50.2007  n=9
    
    cout<<endl<<"Функция для тестирования поиска максимума функции (Лев в пустыне)"<<endl;
    
    int n;
    float Delta=5;      // абсолютная точность до которой происходят итерации absolute Precision.
    float Lf=0;         // Lf левая граница интервала поиска (начало интервала поиска).
    float Rg=100;       // Rg правая граница интервала поиска (конец интервала поиска).
    float Lf3=0, Rg3=0;     // точки разбиения интервала поиска на три равные части.
    float LfCor=0, RgCor=0; // значение функции Cor в точке Lf3, Rg3.
    
    // максимум функции Cor() находится между началом и конецом интервала поиска Lf и Rg.
    // при работе алгоритма интервал поиска разбивается на три равные части:
    // Lf - Lf3 - Rg3 - Rg
    
    
    for ( n=0; n<16; n+=5 ){ // n<16; ограничение на максимальное количество итераций. // n++
        
        Lf3=(Lf+Lf+Rg)/3;       // вычисление первой трети интервала поиска.
        LfCor=Cor(Lf3);         // получение значения функции в точке Lf3.
        
        Rg3=(Lf+Rg+Rg)/3;       // вычисление второй трети интервала поиска.
        RgCor=Cor(Rg3);         // получение значения функции в точке Rg3.
        
        cout<<"n="<<n<<endl;
        cout<<"Rg3-Lf3="<<Rg3-Lf3<<"    Lf3="<<Lf3<<"  Rg3="<<Rg3<<"  LfCor=" <<LfCor<<"  RgCor="<<RgCor<<endl;
        
        // условие завершения итераций.
        if ( fabsf(Rg3-Lf3)<Delta) break;
        
        // формирование следующих, меньших границ интервала поиска.
        if ( LfCor < RgCor) { Lf=Lf3; } else { Rg=Rg3; }
        
    } // n
    
    //  максимум функции Cor(), и ее аргумент.
    float MaxCor=(LfCor + RgCor)/2;   float ArgCor=(Lf3 + Rg3)/2;
    
    cout<<endl<<"MaxCor="<<MaxCor<<"  ArgCor="<<ArgCor<<"  n="<<n<<endl;
    // MaxCor=99.9984  ArgCor=50.0562  n=11
    // MaxCor=99.9984  ArgCor=49.8635  n=11
    // MaxCor=99.9909  ArgCor=50.2007  n=9
    
    //if ( fabsf(Rg-Lf)<Delta || fabsf(RgCor-LfCor)<1 ) break;
    ///if ( fabsf(Rg-Lf)<Delta || RgCor-LfCor==0 ) break;
    ///if ( fabsf(Rg-Lf)<Delta) break;
}//____________________________________________________________________________


void curveQubicAproximation(OCRPoint &p0, OCRPoint &p1, OCRPoint &p2, OCRPoint &p3) {
    /**
     Функция вычисления параметров кривой Безье по координатам четырех точек на кривой.
     p0 - начальная точка
     p1 - точка кривой  на расстоянии 1/3 от начала
     p2 - точка кривой  на расстоянии 1/4  от начала
     p3 - конечная точка
     В функции вычисляются координаты управляющих точек и константы параметрического уравнения
     общего вида (A, B, C, D). Две управляющие точки это те точки, которые таскаем мышкой при
     визуальном построении кривой (управляющие вектора). Параметрическое уравнение общего вида:
     x(t)=Ax*t*t*t + Bx*t*t + Cx*t + Dx, аналогично для “y”, где параметр t менятся в диапазоне
     от 0 до 1, от начала к концу кривой. По сути выполняется апроксимация сглаженных фокальных
     линий еще более гладкими кривыми Безье. Которые потенциально можно стыковать друг с другом.
     
     Функция используя координаты начальных и конечных точек исходной кривой вычисляет
     координаты узлов (главных точек) при t=L*1/3, t=L*2/3 по таблице mTab. По координатам
     главных точек вычисляются параметры (длина и угол) управляющих векторов кривой Безье.*/
    //float Px0=p0.x;
    //float  Py0=p0.y;
    float  Px1=(-5.0 * p0.x + 18.0 * p1.x - 9.0 * p2.x + 2.0 * p3.x)/6.0;
    // координата “x” первой управляющей точки.
    float  Py1=(-5.0 * p0.y + 18.0 * p1.y - 9.0 * p2.y + 2.0 * p3.y)/6.0;
    // координата “y” первой управляющей точки.
    float  Px2=(2.0 * p0.x - 9.0 * p1.x + 18.0 * p2.x - 5.0 * p3.x)/6.0;
    // координата “x” второй управляющей точки.
    float  Py2=(2.0 * p0.y - 9.0 * p1.y + 18.0 * p2.y - 5.0 * p3.y)/6.0;
    // координата “y” второй управляющей точки.
    //float  Px3=p3.x;  // Начальные и конечные управляющие точки кривой Безье совпадают
    //float  Py3=p3.y;  // с начальными и конечными точками (узлами) фокальной линии.
    
    /**
     Источники:
     Роджерс Д., Адамс Дж. Математические основы машинной графики. М.: Мир, 2001. 604 с.
     http://sernam.ru/book_mm3d.php?id=92 // это Адамс
     http://alex-black.ru/article.php?content=109
     http://fullref.ru/job_a517a7c403e9395d49dc5ea305453db0.html
     */
    
    //передаем результат вычисления
    p1.x=Px1;
    p1.y=Py1;
    p2.x=Px2;
    p2.y=Py2;
}

void curveQuadraticAproximation(OCRPoint &p0, OCRPoint &p1, OCRPoint &p3) {
    /**
     Функция вычисления параметров кривой Безье по координатам четырех точек на кривой.
     p0 - начальная точка
     p1 - точка кривой  на расстоянии 1/2 от начала. 
     Возвращается как координаты вычисляемой опорной точка квадратичной кривой
     p3 - конечная точка
     В функции вычисляются координаты управляющих точек и константы параметрического уравнения
     общего вида (A, B, C, D). Две управляющие точки это те точки, которые таскаем мышкой при
     визуальном построении кривой (управляющие вектора). Параметрическое уравнение общего вида:
     x(t)=Ax*t*t*t + Bx*t*t + Cx*t + Dx, аналогично для “y”, где параметр t менятся в диапазоне
     от 0 до 1, от начала к концу кривой. По сути выполняется апроксимация сглаженных фокальных
     линий еще более гладкими кривыми Безье. Которые потенциально можно стыковать друг с другом.
     
     Функция используя координаты начальных и конечных точек исходной кривой вычисляет
     координаты узлов (главных точек) при t=L*1/3, t=L*2/3 по таблице mTab. По координатам
     главных точек вычисляются параметры (длина и угол) управляющих векторов кривой Безье.*/
    //float Px0=p0.x;
    //float Py0=p0.y;
    float Px1=-0.5 * p0.x + 2.0 * p1.x - 0.5 * p3.x;
    // координата “x” первой управляющей точки.
    float Py1=-0.5 * p0.y + 2.0 * p1.y - 0.5 * p3.y;
    // координата “y” первой управляющей точки.
    
    // float Px1=-0.5*x0 + 2*x1 - 0.5*x2;  // координата “x” управляющей точки 1/2. ///////////
    // float Py1=-0.5*y0 + 2*y1 - 0.5*y2;  // координата “y” управляющей точки 1/2. ///////////
    
    //float Px3=p3.x;  // Начальные и конечные управляющие точки кривой Безье совпадают
    //float Py3=p3.y;  // с начальными и конечными точками (узлами) фокальной линии.
    
    /**
     Источники:
     Роджерс Д., Адамс Дж. Математические основы машинной графики. М.: Мир, 2001. 604 с.
     http://sernam.ru/book_mm3d.php?id=92 // это Адамс
     http://alex-black.ru/article.php?content=109
     http://fullref.ru/job_a517a7c403e9395d49dc5ea305453db0.html
     */
    
    //передаем результат вычисления
    p1.x=Px1;
    p1.y=Py1;
}


/** @bref  вычисление наименьшего угла образуемого двумя отрезками проведенными к точке p0  */
float pointsAngle(OCRPoint &p0, OCRPoint &p1, OCRPoint &p2){
    
    /**    // ТЕОРИЯ //
      В данный момент все фокальные линии описаны особыми точками, точками экстремума и кривыми Безье
      проведенными между этими точками. Для кривых Безье особые точки являются точками разрыва.
      Наиболее характерными (и более устойчивами) особыми точками фокальной линии являются точки с
      наибольней скоростью изменения угла. Т.е. Точки "P" типа с максимальной второй производной.
      Угол это первая производная.
     
      Практическая реализация.
      В каждой  особой точке фокальной линии вычисляем вторую производную.
      Эквивалентом второй производной в особой точке (точке разрыва кривых Безье) является угол между
      опорными векторами смежных кривых Безье. Чем меньше угол тем "характернее" донная точка экстремума
      Косинус угла между векторами a=(ax,ay) и b=(bx,by) равен скалярному произведению векторов a·b,
      поделенному на произведение модулей векторов |a|*|b|.
      cos(α) =a·b/|a|*|b|;
      https://ru.onlinemschool.com/math/library/vector/multiply/
      https://ru.onlinemschool.com/math/library/vector/angl/
     
      Развитие.
      Вычислять вторую производную по всей кривой Безье второго порядка нет смысла - кривая слишком гладкая.
      Если у кривой Безье начало и конец находятся близко (по отношению ко всей длине фокальной линии),
      то имеет смысл проверить угол между первым и вто
     рым опорным вектором этой кривой Безье проведенным к одной
      опорной точке.
   */
    
    int GrafT=0;
    // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT) { DG("Функция выделения устойчивых экстремумов (перегибов) фокальной линии по углам"<<endl); }
    
    
    // p0 координаты точки для которой будет вычисятся угол.
    // p1 координаты точки конца первого отрезка
    // p2 координаты точки конца первого отрезка
    if (GrafT) { cout<<"p0:"<<p0.x<<":"<<p0.y<<"  p1="<<p1.x<<":"<<p1.y<<" p2="<<p2.x<<":"<<p2.y<<endl; }
    
    int dx1=p1.x-p0.x;  int dy1=p1.y-p0.y;
    int dx2=p2.x-p0.x;  int dy2=p2.y-p0.y;
    
    // скалярное произведение векторов:
    float scalar = dx1*dx2 + dy1*dy2;
    
    // Вычисление модулей векторов (длинн хендлеров) по Теореме Пифагора (м.б. уже вычисленно).
    float mod1=sqrt(float(dx1*dx1 + dy1*dy1));
    float mod2=sqrt(float(dx2*dx2 + dy2*dy2));
    
    // cosAlpha в компактном виде
    //float cosAlpha = (dx1*dx2 + dy1*dy2)/(sqrt(float(dx1*dx1 + dy1*dy1)) * sqrt(float(dx2*dx2 + dy2*dy2)));
    
    float cosAlpha=scalar/(mod1*mod2);      // -1<=cosAlpha<=1      // cos(α)
    float Alpha=57.29747*acos(cosAlpha);    // -pi<=Alpha<=pi       // -pi<=cos(arccos(x))<=pi при -1<=x<=1
    if (GrafT) { cout<<"cosAlpha="<<cosAlpha<<"   Alpha="<<Alpha<<endl; }
    
    return Alpha;
    
    
    
    /*
    // Вычисление лимита угла в градусах.
    float lim_Alpha=54; // назначаемый лимит угла в градусах -pi<=lim_Alpha<=pi.
    float lim_cosAlpha=cos(lim_Alpha/57.29747); // вычисленный лимит угла в значениях косинуса -1<=lim_cosAlpha<=1.
    
    if(cosAlpha > lim_cosAlpha ) {
        if (GrafT){ cout<<"Точка <P> типа, cosAlpha="<<cosAlpha<<"   lim_cosAlpha="<<lim_cosAlpha<<endl; }
        return 0;
    }
    // угол больше, а косинус угла меньше.
    return 1;
    */
}

    
/*

// ЗАГОТОВКА Statistics Вычисление распределения вероятностей с параметром (Porog, Delta)
// Подпрограмма расчета моментов распределения: матожидания, дисперсии.
// -----------------------------------------------------------------------------
void GBitset::Statistics(vector<int> &HBuf, vector<int> &GBuf,
						 int Porog, int Delta ) {
  int x,y,d,p;
  ///vector<int> BufS(ncolumns);
  ///vector<int> BufR(ncolumns);

			  if( NLet<=0 || NLet>ncolumns) { return; }
								BufS=GBuf;
			  for (  y=0; y < NLet-1; y++ ){ BufR[y+1]= GBuf[y+1]-GBuf[y]; }
				   // TMP
//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < NLet;  x++ ){      // x < d+1;
//                DM(BufR[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//              DM(END);

				 // "сортировка Шейкера". // GBuf
				 // http://www.abc-it.lv/index.php/id/610
				 Sort(BufR,NLet-1);
			//             sort(BufR.begin(), BufR.end());

			//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < NLet;  x++ ){      // x < d+1;
//                DM(BufR[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//             DM(END);

			  // фильтрация по порогу, отбрасывание хвостов распределения
			  d=0;
			  for (  y=0; y < NLet; y++ ){
				 if ( BufR[y] > Porog/8 && BufR[y] < Porog )
					{ BufR[d]=BufR[y]; d++; }
			  }// for y
//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < d;  x++ ){      // x < d+1;
//                DM(BufR[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//              DM(END);

			  // отбрасывание хвостов распределения
			  // получение среднего по горизонту размера периода букв 10*mean
			  p=0; MeanSA=0;
			  for ( y=Delta; y <d-Delta && p < NLet; y++ ) {
				BufR[p]=BufR[y]; MeanSA+=BufR[p]; p++;
			  }// for y
			  // MeanSA=10*MeanSA/p; PLLLL
			  if( p > 0 && p <= NLet ){ MeanSA=10*MeanSA/p; }
			  else MeanSA=9999;// средний период букв умноженный на 10
			 // вычисленная дисперсия VarianceSA периода букв умноженныя на 10

#ifdef DEBUGLVL
			  // ВИЗУАЛИЗАЦИЯ цифровая
//     DM(MeanSA<<" -MeanSA*10- "); DM(p<<" p "); DM(Delta<<" Delta ");DM(END);
//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < NLet;  x++ ){      // x < d+1;
//                DM(BufS[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//              DM(END);

//              DM(END);            // size-1   N0Fase
//              for ( x=0; x < p;  x++ ){      // x < d+1;
//                DM(BufR[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
//                  }//for x
//              DM(END);

#endif
}//_____________________________________________________________________________
*/






////////////////////////////////////////////////////////////////////////



};
