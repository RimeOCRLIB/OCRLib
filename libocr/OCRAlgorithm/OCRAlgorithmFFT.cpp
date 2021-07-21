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

namespace ocr{

    
// -----------------------------------------------------------------------------    
         
// Программа выполнения прямого и обратного БПФ (быстрого преобразования Фурье, FFT) 
// Maxim Ivanov   http://e-maxx.ru/algo/fft_multiply // 
    
// -----------------------------------------------------------------------------
/*
void mainFFT (vector<base> & a, bool invert){
    
cout<<"Start program *mainFFT*"<<endl;    
     
        int n = (int) a.size(); // n - число преобразуемых отсчетов
        
        for (int i=1, j=0; i<n; ++i) {
            int bit = n >> 1;
            for (; j>=bit; bit>>=1) // >>=1 деление на 2
                j -= bit;
            j += bit;
            if (i < j)
                swap (a[i], a[j]); // меняем местами значения
        }
        
        for (int len=2; len<=n; len<<=1) {
            double ang = 2*PI/len * (invert ? -1 : 1);
            base wlen (cos(ang), sin(ang));
            for (int i=0; i<n; i+=len) {
                base w (1);
                for (int j=0; j<len/2; ++j) {
                    base u = a[i+j],  v = a[i+j+len/2] * w;
                    a[i+j] = u + v;
                    a[i+j+len/2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (invert)   // invert=0 для прямого преобразования Фурье. invert=1 для обратного преобразования.
            for (int i=0; i<n; ++i)
                 a[i] /= n;  // нормировка для обратного преобразования Фурье.
     
        
}// -----------------------------------------------------------------------------  
*/    
    
// Z-функция строки // http://e-maxx.ru/algo/z_function
    
vector<int> ZFunction (wstring s) {
    //void  ZFunction (string s) {
    
cout<<"Start program *ZFunction*"<<endl;        
        
        int n = (int) s.length(); // длина string
        vector<int> z (n);
        for (int i=1, l=0, r=0; i<n; ++i) {
            if (i <= r)
                z[i] = min (r-i+1, z[i-l]);
            while (i+z[i] < n && s[z[i]] == s[i+z[i]])
                ++z[i];
            if (i+z[i]-1 > r)
                l = i,  r = i+z[i]-1;
        }
        
        return z;
        
}// ----------------------------------------------------------------------------- 
    
    
    
    
    
    // Литература //
    
    // Теория;
    // Радиотехнические цепи и сигналы. Радио и связь Москва, 1986. Гоноровский Иосиф Семенович. 
    // Radio Technical Circuits and Signals. Radio i sviaz, Moscow, 1986. Gonorovskij Iosif Semenovich
    // 12.13. Алгоритм цифроврй фильтрации во временной и частотной областях. Стр 385.
    // 13.3. Импульсная характеристика согласованного фильтра. Стр 402.        
    
    // Реализация;
    // Алгоритмические трюки для программистов. Издательский дом "Вильяме" Москва * Санкт-Петербург * Киев 2004. Генри Уоррен, мл.
    // Hacker's DelightHenry.  S. Warren Jr.
    // Листинг 5.1. Подсчет количества единичных битов в слове. Стр 76.
    
    /**/
    
////////////////////////////////////////////////////////////////////////    
    
    // 12.14. Быстрое преобразование Фурье. Стр 390.
    /*      
     // FORTRAN
     
     // ****************************************************************
     // Программа выполнения БПФ (быстрого преобразования фурье)
     // X - комплексный массив преобразуемых отсчетов
     // NN - число преобразуемых отсчетов, равное целой степени двойки, 
     // не превышающее 512
     // IN=0 для прямого преобразования
     // IN=1 для обратного преобразования
     // ****************************************************************
     
     0001        SUBROUTINE FFT (NN,X,IN)
     0002        COMPLEX X(512), W,T 
     0003        IT=0
     0004        IR=NN
     0005 10     IR=IR/2
     0006        IF (IR.EQ.1) GO TO 20
     0007        IT=IT+1
     0008        GO TO 10
     0009 20     CONTINUE
     0010        SI=-1.
     0011        IF (IN.EQ.1) SI=1.
     0012        NX2=NN
     0013        DO 50 ITT=1,IT
     0014        NX=NX2
     0015        NX2=NX/2
     0016        WP=3.141592/FLOAT(NX2)
     0017        DO 40 M=1,NX2
     0018        ARG=FLOAT(M-1)*WP
     0019        W=CMPLX(COS(ARG),SI*SIN(ARG))
     0020        DO 40 MX=NX,NN,NX
     0021        J1=MX-NX+M
     0022        J2=J1+NX2
     0023        T=X(J1)-X(J2)
     0024        X(J1)=X(J1)+X(J2)
     0025 40     X(J2)=T*W
     0026 50     CONTINUE 
     0027        N2=NN/2
     0028        N1=NN-1
     0029        J=1
     0030        DO 65 I=1,N1
     0031        IF (I.GE.J) GO TO 55
     0032        T=X(J)
     0033        X(J)=X(I)
     0034        X(I)=T
     0035 55     K=NN/2
     0036 60     IF (K.GE.J) GO TO 65
     0037        J=J-K
     0038        K=K/2
     0039        GO TO 60
     0040 65     J=J+K
     0041        IF (IN.EQ.1) GO TO 75
     0042        DO 70 I=1,NN
     0043 70     X(I)=X(I)/FLOAT(NN)
     0044 75     CONTINUE
     0045        RETURN
     0046        END        
     */ 
    
    /*        
     SUBROUTINE FFT (NN,X,IN)
     COMPLEX X(512), W,T 
     IT=0
     IR=NN
     10      IR=IR/2
     IF (IR.EQ.1) GO TO 20
     IT=IT+1
     GO TO 10
     20      CONTINUE
     SI=-1.
     IF (IN.EQ.1) SI=1.
     NX2=NN
     DO 50 ITT=1,IT
     NX=NX2
     NX2=NX/2
     WP=3.141592/FLOAT(NX2)
     DO 40 M=1,NX2
     ARG=FLOAT(M-1)*WP
     W=CMPLX(COS(ARG),SI*SIN(ARG))
     DO 40 MX=NX,NN,NX
     J1=MX-NX+M
     J2=J1+NX2
     T=X(J1)-X(J2)
     X(J1)=X(J1)+X(J2)
     40      X(J2)=T*W
     50      CONTINUE 
     N2=NN/2
     N1=NN-1
     J=1
     DO 65 I=1,N1
     IF (I.GE.J) GO TO 55
     T=X(J)
     X(J)=X(I)
     X(I)=T
     55      K=NN/2
     60      IF (K.GE.J) GO TO 65
     J=J-K
     K=K/2
     GO TO 60
     65      J=J+K
     IF (IN.EQ.1) GO TO 75
     DO 70 I=1,NN
     70      X(I)=X(I)/FLOAT(NN)
     75      CONTINUE
     RETURN
     END
     // в текстовом файле не забыть поставить перевод каретки (ENTER) после END
     */
    // DO - циклы с фиксированным числом повторений
    
    
    
    //______________________________________________________________________________________________ 
    
#ifdef FFT_FORTRAN   // блокировка кода      
    
/*  // блокировка кода 
 
    //// fft_(integer *nn, complex *x, integer *in);
    
    // Программа выполнения БПФ (быстрого преобразования фурье)
    // С версия компелится, но работает не правильно
 
 
    // Subroutine 
    ///         int fft_(integer *nn, complex *x, integer *in) {        
    
    //------------
    
    //   m1=m,  xx=x // замены       
    // struct { real r, i; } complex;
    
    integer *nn, nn1=8;  // NN - число преобразуемых отсчетов, равное целой степени двойки, не превышающее 512          
    //integer nn1=512;     // long int *nn;  // if (*nn < 4) { *nn=4; }    if (*nn > 512) { *nn=512; }
    if (nn1 < 4) { nn1=4; }    if (nn1 > 512) { nn1=512; }
    nn=&nn1;
    float fnn1=(float)nn1;
    
    complex xx[nn1+2]; // *2          // X - комплексная структура преобразуемых отсчетов  // комплексный массив
    //complex w[nn1+2];
    
    
    integer *in, in1;      // IN=0 для прямого преобразования Фурье. IN=1 для обратного преобразования.   
    //integer in1;     
    in=&in1;
    *in = 0;
    
    cout<<"*nn="<<*nn<<"  *in="<<*in<<endl;
    
    xx[0].r = 0, xx[0].i = 0;
    integer cnt;  // int cnt;
    for (cnt=1; cnt <= *nn+2; cnt++) {
        //xx[cnt].r = t1.r, xx[cnt].i = t1.i;
        //xx[cnt].r = 1.0, xx[cnt].i = 2.0;
        xx[cnt].r = 0, xx[cnt].i = 0;            
    }
    //xx[0].r=1.f; xx[1].r=-1.f; xx[2].r=1.f; xx[3].r=-1.f; xx[4].r=1.f; xx[5].r=-1.f; xx[6].r=1.f; xx[7].r=-1.f;
    //синус
    xx[1].r=1.f; xx[2].r=-1.f; xx[3].r=1.f; xx[4].r=-1.f; xx[5].r=1.f; xx[6].r=-1.f; xx[7].r=1.f; xx[8].r=-1.f;
    // постоянная составляющая
    //      xx[1].r=1.f; xx[2].r=1.f; xx[3].r=1.f; xx[4].r=1.f; xx[5].r=1.f; xx[6].r=1.f; xx[7].r=1.f; xx[8].r=1.f;
    
    cout<<"IN     xx[cnt].";
    for (cnt=1; cnt <= *nn; cnt++) {
        cout<<"r="<<xx[cnt].r<<" i="<<xx[cnt].i<<"    ";
    }
    cout<<endl;
    
    //------------
    
    // System generated locals //
    integer i__1, i__2, i__3, i__4, i__5, i__6, i__7;
    real r__1, r__2;
    complex q__1;
    
    // Builtin functions // Встроенные функции
    double sin(doublereal), cos(doublereal);
    
    // Local variables //
    static integer i__, j, k, m1;
    static complex t, w;  // , w
    static integer j1, j2, n1, n2, ir, it;
    static real si;
    static integer mx, nx;
    static real wp;
    static integer nx2;
    static real arg;
    static integer itt;
    
    // Parameter adjustments // настройки параметров
    //--xx;   // x=xx  // В С элементы массива нумеруются с нуля, в FORTRAN — с единицы.
    
    // Function Body // Тело функции
    
    // округление числа преобразуемых отсчетов *nn, до целой степени двойки /4, не более 512
    // ........,  256<=*nn<=511 it=7,  512<=*nn it=8,
    it = 0;
    ir = *nn;
L10:
    ir /= 2;
    if (ir == 1) {
        goto L20;
    }
    ++it;
    goto L10;
L20:
    //it = 3;
    cout<<"ir="<<ir<<"  it="<<it<<endl; //   cout<<"i__4="<<i__4<<endl;
    // выбор +- si
    si = -1.f;
    if (*in == 1) { //
        si = 1.f;
    }
    
    nx2 = *nn;  // 8
    i__1 = it;
    // разбиение исходной последовательности xx[] на подпоследовательности // 1,2
    for (itt = 1; itt <= i__1; ++itt) { // 1  <=2  ++1
        nx = nx2;
        nx2 = nx / 2;
        wp = 3.141592f / (real) nx2;
        i__2 = nx2;    cout<<"i__2="<<i__2<<endl;
        // вычисление коэффициентов в подпоследовательностях
        for (m1 = 1; m1 <= i__2; ++m1) { // 1  <=4;2;  ++1  // m1=m
            arg = (real) (m1 - 1) * wp;
            r__1 = cos(arg);
            r__2 = si * sin(arg);
            q__1.r = r__1, q__1.i = r__2;
            w.r = q__1.r, w.i = q__1.i;
            i__3 = *nn;
            i__4 = nx;
            cout<<"w.r="<<w.r<<"  w.i="<<w.i<<endl;
            //for (mx = nx;  i__4 < 0 ? mx >= i__3 : mx <= i__3;  mx += i__4) {
            //for (mx=nx;  mx<=*nn;  mx+=nx) {
            for (mx = nx;  mx <= i__3;  mx += i__4) {  // do i = 0, N, 2  // i__4=nx
                //cout<<"i__3="<<i__3<<"  i__4="<<i__4<<"  nx="<<nx<<"  nx2="<<nx2<<endl;
                j1 = mx - nx + m1;
                j2 = j1 + nx2;
                i__5 = j1;
                i__6 = j2;
                q__1.r = xx[i__5].r - xx[i__6].r, q__1.i = xx[i__5].i - xx[i__6].i;
                
                t.r = q__1.r, t.i = q__1.i;
                i__5 = j1;
                i__6 = j1;
                i__7 = j2;
                q__1.r = xx[i__6].r + xx[i__7].r, q__1.i = xx[i__6].i + xx[i__7].i;
                
                xx[i__5].r = q__1.r, xx[i__5].i = q__1.i;
                // L40: //
                i__5 = j2;
                q__1.r = t.r * w.r - t.i * w.i, q__1.i = t.r * w.i + t.i * w.r; 
                
                xx[i__5].r = q__1.r, xx[i__5].i = q__1.i;
            }
        }
        // L50: //
    }
    
    n2 = *nn / 2;  //////////// не используется
    n1 = *nn - 1;
    j = 1;
    i__1 = n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
        if (i__ >= j) {
            goto L55;
        }
        i__5 = j;
        t.r = xx[i__5].r, t.i = xx[i__5].i;
        i__5 = j;
        i__4 = i__;
        xx[i__5].r = xx[i__4].r, xx[i__5].i =xx[i__4].i;
        i__5 = i__;
        xx[i__5].r = t.r, xx[i__5].i = t.i;
    L55:
        k = *nn / 2;
    L60:
        if (k >= j) {
            goto L65;
        }
        j -= k;
        k /= 2;
        goto L60;
    L65:
        j += k;
    }
    
    // in=0 для выполнения нормировки прямого преобразования Фурье. in=1 для выполнения обратного преобразования.
    if (*in == 1) {
        goto L75;
    }
    i__1 = *nn;
    // нормировка прямого преобразования Фурье (обратногое преобразование не нормируем)
    for (i__ = 1; i__ <= i__1; ++i__) {
        // L70: //
        i__5 = i__;
        i__4 = i__;
        r__1 = (real) (*nn);
        q__1.r = xx[i__4].r / r__1, q__1.i = xx[i__4].i / r__1;
        xx[i__5].r = q__1.r, xx[i__5].i = q__1.i;
    }// for (i__ = 1;
L75:
            
    ///            return 0;
    ///        } // fft_ //
     
    
    cout<<"OUT xx[cnt].";
    for (cnt=1; cnt <= *nn; cnt++) {
        cout<<"r="<<xx[cnt].r<<" i="<<xx[cnt].i<<"    ";
    }
    cout<<endl;
    
    cout<<"ir="<<ir<<"  it="<<it<<endl;
    
*/ // блокировка кода    
#endif  // блокировка кода       
//______________________________________________________________________________________________
    
    
    // W8        
    // x(0)W 0 + x(1)W 4 + x(2)W 8	+ x(3)W 12 + x(4)W 16 + x(5)W 20 + x(6)W 24 + x(7)W 28 
    // W 0 - W 4 + W 8 - W 12 + W 16 - W 20 + W 24 - W 28;
    
//______________________________________________________________________________________________
    
    //Тест
    //data[0]=1,0; data[1]=-1,0; data[2]=1,0; data[3]=-1,0; data[4]=1,0; data[5]=-1,0; data[6]=1,0; data[7]=-1,0; // только первое число
    ///data[0]=complex<double>(1,0);  data[1]=complex<double>(-1,0); data[2]=complex<double>(1,0); data[3]=complex<double>(-1,0);
    ///data[4]=complex<double>(1,0); data[5]=complex<double>(-1,0);  data[6]=complex<double>(1,0); data[7]=complex<double>(-1,0);
    
//______________________________________________________________________________________________
    
    
////////////////////////////////////////////////////////////////////////



};
