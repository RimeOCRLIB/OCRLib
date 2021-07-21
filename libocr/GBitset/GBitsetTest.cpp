#include "GBitset.h"

//-------------------------------Test------------------------------------//

/*                     // СТАТИЧЕСКИЕ  массивы  //
  ImBuf=(int*)farcalloc(ncnr+512, 4);    // запрос памяти с очисткой 0
  if (ImBuf==NULL) exit (1);             // 4 длина каждого элемента в байтах
                                         // ncnr+512 количество элементов в массиве

  //       вычисления        //

  if ( ImBuf !=NULL ) farfree(ImBuf);  //  освобождение памяти
*/
//q=(тип_q *)farmalloc(n_byte);     // запрос памяти без очистки 0
//q_new=farrealloc(q_old,n_byte);;  // изменение размера блока

//int *p;
// p = (int *) malloc(100 * sizeof(int)); /* Выделение памяти для 100 целых чисел */


// -----------------------------------------------------------------------------




// -----------------------------------------------------------------------------
//                             start Test.
// Набор подпрограмм тестирования алгоритмов.
// -----------------------------------------------------------------------------
 void GBitset::startTest(){

///// vector<int> BufHelp(2000);

///                 filterIntegrator(BufHelp,10);
///                 filterHighTest(BufHelp, 10);
///                 filterLaplasianTest(BufHelp, 10, 20); // KHig=10;  KLow=20;
       //////           testBitsBuf();

}//_____________________________________________________________________________


// -----------------------------------------------------------------------------
// интегрирующий фильтр, integrating filter.
// -----------------------------------------------------------------------------
    // прямоугольная весовая функция
    // y(n)= y(n-1)+xn-x(n-2k-1)
    // треугольная весовая функция
    // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn

 void GBitset::filterIntegrator(vector<int> &In, int KHig){


//-----------------------------------------------//
/*              //  TEST на sqrt  //

TIME_START

//   int x,y,n,r,r0;   // ,p,d
   int Bf[1000];     // unsigned char   // short,  int,  unsigned char


      //  TEST
////      for ( y=0; y < 1000; y++ ) { Bf[y]=4 + y/30; }

      //  TABL
      signed char Sq[4095];
      int x,y,n,r;
      for ( y=0; y < 4095; y++ ) { Sq[y]=sqrt(y); }
//      for ( y=0; y < 4095; y++ ) { Sq[y]=0; }
//      for ( y=0; y < 64-1; y++ ) { Sq[y*y]=y; }  // *(y*y)=y;
//      for ( y=1; y < 4095; y++ ) { Sq[y]=Sq[y]+Sq[y-1]; }

      // 100 000 000
      for ( n=0; n < 100000; n++ ) { // 1000000
         for ( y=0; y < 990; y++ ) {
//          r0=sqrt(Bf[y]*Bf[y] + Bf[y+8]*Bf[y+8]);  //  TEST   // >>5 /32
////            r=Bf[y]*Bf[y] + Bf[y+8]*Bf[y+8];
            if ( r < 4095 ) r=Sq[r];
            else r=sqrt(r);
         }//for y
      }//for n

                DM(END);
                for ( y=0; y < 4095; y++  ){
                DM(Sq[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
                DM(END);

DM(END); DM(999999<<" sqrt "); DM(r<<" r "); DM(END);
TIME_PRINT
        //r=(float)(x0*x0 + y0*y0);
        //r=sqrt(r);
*/

//-----------------------------------------------//


/*
  // TEST и отработка
  int x,y,p,d;
  int delta;
  int size=In.size();  //  short
  //vector<int> BufIn(size);

//  int BufIn[200] = {0} ;  // < 3,6  // int m[2][3]={0}
  size=1000;

///  int *BufIn=(int*)malloc(4*size+32); // 4*size  *BufIn указатель, можно использовать в массиве  InLk[size];
///  memset(BufIn,0,4*size+32);  // 0000000
//// int *BufIn=(int*)calloc(size+32,4);    // запрос памяти с очисткой

////  int *BufIn=(int*)farcalloc(size+32,4); // запрос памяти с очисткой > 65521 байт.
////  if (BufIn==NULL) exit (1); // Максимальный размер сегмента памяти > 65521 байт.
  // free(BufIn);
  // return 0;


      // обнуление BufIn, lt1 ( где BufIn, lt1 глобальные )
      memset(ImBuf,0,4*(ncnr+512));   //   memset(lt1,0,4*(ncnr+512));


                             // TEST
        //BufIn.assign(size,0);
        //memset(ImBuf,0,4*size+32);
        for ( y=0; y < 10; y++ ) {
        ImBuf[2+y]=ImBuf[size/2+y]=ImBuf[size-2-y]=1000; // 100000;
        }

              // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size;  y++ ){
                DM(ImBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

///      for ( x=0; x < 1000000; x++ ) {// 1000000
      // Входной буфер  ImBuf[y]
      p=0;
      for ( y=0; y < size-1; y++ ) { // прямоугольная весовая функция, прямая фильтрация
          //p=(p+ImBuf[y])*ImBuf[y];
          delta=ImBuf[y+1] - ImBuf[y]; // delta=ImBuf[y]-delta;
          if ( delta > 0 )  { d=y; }
          if ( delta < 0 )  { ImBuf[p]=(y+d)>>1; ImBuf[p+1]=y-d; p+=2; } // ImBuf[p+1]=y-d; ImBuf[p]=(y+d)/2;
      }//for y
///      }//for x
      // p

              // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < p;  y++ ){
                DM(ImBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

//free(BufIn);
//farfree(BufIn);


*/


/*
    int st=10000;
    int *buffer = (int*) malloc (4*st+32);
    if (buffer==NULL) exit (1);
    for(int a=0;a<st;a++) buffer[a]=a;
    for(int a=0;a<st;a++)DM("buffer["<<a<<"]="<<buffer[a]<<END);
    free(buffer);
  //for ( int a=0; a<size; a++ )  InLk[a]=a;
  //for ( int a=0; a<size; a++ )  DM("InLk["<<a<<"]="<<InLk[a]<<END);
*/

  //unsigned size; количество байтов в захваченном блоке
  //int *InLk;   InLk=(int*)malloc(4*size+32);
  //int *pHeap = malloc(sizeof(pHeap));    free(pHeap);
  //void *malloc(size_t size);

////////////////////////////////////////////////////////////////////////////////

/*
 // TEST и отработка  Real  Imag
 int y,p;
 short size=In.size();

 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 int  sp=KHig/2,   s1=KHig+1,  s2=KHig*2+1;
 int  kp=KHig/2,   k1=KHig;

 vector<int> InLk(size+s1);
 vector<int> Real(size+s1);  // real part действительная часть
 vector<int> Imag(size+s1);  // imaginary part мнимая часть

                             // TEST
        InLk[s1+1]=InLk[(size+s1)/2]=InLk[size+s1-1]=1000; // 100000;

      // Входной буфер  InLk[y]
      for ( y=s1; y < size+s1; y++ ) { //прямоугольная весовая функция, прямая фильтрация
          Real[y+1] =Real[y]+InLk[y]-InLk[y-kp]; // Real[y-1+1]
          Imag[y-k1]=Imag[y-k1-1]+InLk[y-kp]-InLk[y-k1];
//          Imag[y-sp]=Imag[y-sp-1]+InLk[y]+InLk[y-k1]-2*InLk[y-kp];  // -sp
      }   // смещение импульсной характеристики фильта влево на sp=KHig/2;
      // Выходной буфер BufT[y-sp]

                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size+s1;  y++ ){
                DM(Real[y]-Imag[y]+InLk[y]*9<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size+s1;  y++ ){
                DM(Imag[y]+Real[y]+InLk[y]*9<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

// Real[y-sp]=Real[y-sp-1]+InLk[y]-InLk[y-k1];
// Imag[y-sp]=Imag[y-sp-1]+InLk[y]+InLk[y-k1]-2*InLk[y-kp];  // -sp
*/

////////////////////////////////////////////////////////////////////////////////
/*
 // TEST и отработка прямоугольной весовая функции

 ////////////////////////////////////////////////////////////
 //  прямоугольная весовая функция интегрирующего фильтра  //
 //  y(n)= y(n-1)+xn-x(n-2k-1)                             //
 ////////////////////////////////////////////////////////////

 int y,Khg;
 short size=In.size();

 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 int  sp=KHig/2,   s1=KHig+1,  s2=KHig*2+1,   s6=s1;

 vector<int> InLk(size+s6);
 vector<int> BufT(size+s6);


                             // TEST
        InLk[s1+1]=InLk[(size+s1)/2]=InLk[size+s1-1]=1000; // 100000;

      // Входной буфер  InLk[y]
      for ( y=s1; y < size+s1; y++ ) { //прямоугольная весовая функция, прямая фильтрация
          BufT[y-sp]=BufT[y-sp-1]+InLk[y]-InLk[y-KHig];
      }   // смещение импульсной характеристики фильта влево на sp=KHig/2;
      // Выходной буфер BufT[y-sp]

                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size+s6;  y++ ){
                DM(BufT[y]+InLk[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END); DM(END);
              for ( y=0; y < size;  y++ ){
                DM(BufT[y]+InLk[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);
*/

/*
// отработка прямоугольной весовая функции
      // Входной буфер  In[y]
      for ( y=s1; y < size+s1; y++ ) { //прямоугольная весовая функция, прямая фильтрация
          BufT[y]=BufT[y-1]+InLk[y]-InLk[y-KHig]; // -sp
      }
  ///    for ( y=s2; y < size+s4; y++ ) { lt1[y]=lt1[y]/Khg; }   // InLk[y]=0; Norm // y=0; size+s6;
  ///    for ( y=size+s4; y > s2; y--) { //прямоугольная весовая функция, инверсная фильтрация
         // BufT[y]=BufT[y+1]+lt1[y]-lt1[y+KHig];
  ///    }
  ///    for ( y=s1; y < size+s1; y++ ) { BufT[y]=BufT[y]; }  // BufT -s1 -sp
      // Выходной буфер BufT[y-sp], BufT[y]
*/

}//_____________________________________________________________________________




////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------




                     // Test //

// ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian) Test
// Адаптивный F(KHig) нолевой бордюр вокруг массивов.
// -----------------------------------------------------------------------------
    // прямоугольная весовая функция
    // y(n)= y(n-1)+xn-x(n-2k-1)
    // треугольная весовая функция
    // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn

 void GBitset::filterHighTest(vector<int> &In, int KHig){


// TEST и отработка ФИЛЬТРа НИЗКИХ ЧАСТОТ
 int y,Khg;
 int size=(int)In.size();


 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
 //      129           257             385
 int  s2=KHig*2+1,  s4=KHig*4,    s6=KHig*6+1;
/**/

// vector<int> BufIn(size+s6);
// vector<int> lt1(size+s6);

// int BufIn[10000]={0};
// int lt1[10000]={0};

// int *BufIn=(int*)farcalloc(size+s6+32, 4);    // запрос памяти с очисткой
// int *lt1=(int*)farcalloc(size+s6+32, 4);      // запрос памяти с очисткой
/**/

      // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
      memset(ImBuf,0,4*(ncnr+512));      memset(ReBuf,0,4*(ncnr+512));


//                          if (ImBuf==NULL) exit (1);
//                          if (ReBuf==NULL) exit (1);
                          In.assign(size,0);
                          In[0]=In[size-1]=10000; // 10000;

      // Входной буфер BufIn[y+s2], In[y]
      for ( y=0; y < size; y++ ){ImBuf[y+s2]=In[y];}
      Khg=KHig*KHig;
      for ( y=s2; y < size+s4; y++ ) { //прямоугольная весовая функция, прямая фильтрация //2 4
         ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+ImBuf[y-KHig-KHig]-2*ImBuf[y-KHig]+ImBuf[y]; //k=15+
      }
      for ( y=s2; y < size+s4; y++ ) { ReBuf[y]=ReBuf[y]/Khg;  ImBuf[y]=0; }   // Norm // y=0; size+s6;
      for ( y=size+s4; y > s2; y-- ) { //прямоугольная весовая функция, инверсная фильтрация //4 2
         ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+ReBuf[y+KHig+KHig]-2*ReBuf[y+KHig]+ReBuf[y];//k=15-
      }
      for ( y=s2; y < size+s2; y++ ) { In[y-s2]=ImBuf[y]/Khg; }
      // Выходной буфер In[y-s2]

/**/

              // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size+s6;  y++ ){
                DM(ReBuf[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

              // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END); DM(END);
              for ( y=0; y < size;  y++ ){
                DM(In[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

///farfree(ImBuf);
///farfree(ReBuf);
//farfree(InLk);
/**/

////////////////////////////////////////////////////////////////////////////////

// тестировались на текстах

//////////////////////////////////////////////  уменьшин адаптивный бордюр до s6

/*
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
 //   129             257             385
    int  s2=KHig*2+1,  s4=KHig*4,    s6=KHig*6+1;

 vector<int> BufIn(size+s6);
 vector<int> lt1(size+s6);
 vector<int> BufOut1(size+s6);


      // Входной буфер BufIn[y+s2], In[y]
      for ( y=0; y < size; y++ ) { BufIn[y+s2]=In[y]; }
      Khg=KHig*KHig;
      for ( y=s2; y < size+s4; y++ ) { //прямоугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
      }
      for ( y=s2; y < size+s4; y++ ) { lt1[y]=lt1[y]/Khg; }   // Norm // y=0; size+s6;
      for ( y=size+s4; y > s2; y-- ) { //прямоугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
      }
      for ( y=s2; y < size+s2; y++ ) { In[y-s2]=BufOut1[y]/Khg; }
      // Выходной буфер In[y-s2], BufOut1[y]


                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END);
              for ( y=0; y < size+s6;  y++ ){
                DM(lt1[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);

                 // ВИЗУАЛИЗАЦИЯ цифровая
              DM(END); DM(END);
              for ( y=0; y < size;  y++ ){
                DM(In[y]<<"\t"); if (y%10==0)DM(END);  /// ТАБЛИЦА
                  }//for y
              DM(END);
*/

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

}//_____________________________________________________________________________

                             // Test //

// -----------------------------------------------------------------------------

// Полосовой фильтр Гауссиан (ГАУССИАН), попытка минимальной реализации.Test
// Входной буфер &In. Адаптивный F(KHig) нолевой бордюр вокруг массивов
// -----------------------------------------------------------------------------

 void GBitset::filterLaplasianTest( vector<int> &In, int KHig, int KLow){ // filterLaplasianTest

 int y,Klw,Khg,S;
 int size=(int)In.size();

 //////////////////////////////////////////////  всего 4 массива

 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   SN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;
/*
 vector<int> BufIn(size+S8);   // 514
 vector<int> BufOut1(size+S8);
 vector<int> BufOut2(size+S8);
 vector<int> lt1(size+S8);
 vector<int> lt2(size+S8);
*/

// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
        memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512));
        memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));

//    for ( y=0; y < size+S8; y++ )  { BufIn[y]=BufOu[y]=ReBuf[y]=ImBuf[y]=0; }

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

// DM(size<<" sizeG ");  DM(END);

/*    // работает не точно  всего 3 массива
      // обнуление BufIn,ImBuf,ReBuf ( где BufIn,ImBuf,ReBuf глобальные )
memset(BufIn,0,4*(ncnr+512)); memset(ImBuf,0,4*(ncnr+512)); memset(ReBuf,0,4*(ncnr+512));

  for ( y=0; y < size; y++ ){BufIn[y+S4]=In[y];} // Входной буфер Buf In[y]

    Khg=KHig*KHig; Klw=KLow*KLow;
    for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
        ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
        }
    memset(BufIn,0,4*(ncnr+512));
    for ( y=0; y < size+S8; y++ ) {  //Norm
        BufIn[y]=ImBuf[y]/Khg - ReBuf[y]/Klw;   ImBuf[y]=ReBuf[y]=0;
        }
    for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
        ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufIn[y+KLow+KLow]-2*BufIn[y+KLow]+BufIn[y];
        }
    for ( y=S4; y < size+S4; y++ ){ In[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
    // Выход In[y-S4]
*/


}//_____________________________________________________________________________




    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
/*
//##

//  ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian)
//  --------------------------------------------------------------------------//
    // треугольная весовая функция
    // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn

 void GBitset::filterHighM(vector<int> &In, int KHig){
 //int KHig=11+1; //порядок высокочастотного фильтра (k=15) 2< KLow,KHig =<63

 int y,Khg;  // n,x,sum,Varu
 short size=In.size();
 //int size=nrows;
 vector<int> BufIn(size+514);
 vector<int> BufOut1(size+514);
 vector<int> lt1(size+514);


    if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
    for ( y=0; y < size; y++ ){BufIn[y+257]=In[y];} // Входной буфер BuIn[y]
    ///sum=0;
    for ( y=0; y < 257; y++ ){BufIn[y]=0; BufOut1[y]=0; lt1[y]=0;}             //0000
    for ( y=size+257; y < size+514; y++ ){BufIn[y]=0; BufOut1[y]=0; lt1[y]=0;} //0000
    //for ( y=0; y < size+514; y++ ){BufIn[y]=0;} BufIn[257+size/2]=1000;sum=0;//TEST
    //BufIn[257+32]=5000; BufIn[257+size/2]=5000; BufIn[size+257-32]=5000;     //TEST
      Khg=KHig*KHig;
      for ( y=129; y < size+385; y++ ) { //треугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
      }
      for ( y=0; y < size+514; y++ ) { lt1[y]=lt1[y]/Khg; }         //Norm
      for ( y=size+385; y > 129; y--) { //треугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
      }
      for ( y=257; y < size+257; y++ ){ BufOut1[y]=BufOut1[y]/Khg;  //Norm
       //  if(y<=size/2+257){Varu=y-257;} else {Varu=(size+257-y);}
       //  BufOut1[y-257]=(BufOut1[y]*1)*Varu/size;//временная регилировка усиления (вдоль y)
          In[y-257]=BufOut1[y];                    // Выходной буфер BufOut1
       /// BufOut1[y-257]=BufOut1[y]; // /1
       //// sum=sum+BufOut1[y]; //подсчет постоянной составляющей
        }

///DM(sum/size<<"summa "); //контроль постоянной составляющей
// DM("ГАУССИАН ");TIME_PRINT

#ifdef DEBUGLVL
//               static short delta=0.8*ncolumns;
//                for(y=0; y < drawData->rows(); y++) {
//              //DM(BufOut1[y]/200<<" ");
//                if(BufOut1[y]>=0){
//                  for(x =delta; x < (BufOut1[y])+delta; x++ )drawData[0][y][x]*=0.6;
//                 }else{
//                  for(x =delta; x > (BufOut1[y])+delta; x-- )drawData[0][y][x]*=0.6;
//                 }
//               }

#endif

}//_____________________________________________________________________________
*/




/*
Данная программа вычисляет синус, как сумму ряда вида x-x^3/3+x^5/5-...
с заданной точностью, а затем сравнивает полученный результат с результатом
вычисления синуса библиотечной функцией С++.
double CompareSin(double Eps, double X)
{
   double Fx = 0, Fxold = 0, PoxX = X;
   long i = 1;
   int minus = 1;
   do
	{
	   Fxold = Fx;
	   Fx += minus*PoxX/i;
	   PoxX *= X*X;
	   i+=2;
	   minus *= -1;
	}
   while((Fx-Fxold)>=Eps);
   return (Fx - sin(X));
}
*/
//------------------------------------------------------------------------------

