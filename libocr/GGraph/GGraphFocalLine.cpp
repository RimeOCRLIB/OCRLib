//@___________OCRFocalLine.cpp

//  OCRLib  2015 http://www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org.

//*******************************************************************************

#include "GGraph.h"

// Функция стандартизации начертания текста путем определения фокальной линии (графемы) букв.
// Удобно использовать в паре с функцией определения толщины штриха lineWidth().
// Работает только с черно-белой картинкой.

// Матрица (3*3-1) перепрыгивает белые области и вычисляет матрицу только в черных областях,
// что в 2.0 раза быстрее, чем перебирать все пиксели подряд.

void  GGraph::graphFocalLine(int n_RemovLayers) {
    
    // n_RemovLayers это количество удаленных слоев (слой это периметр символов графического текста)
    // Обычно n_RemovLayers=7. // лучше =9  // number of removed layers
    //  * Идея *
    // Алгоритм, общая идеология и терминология взята из работ:
    // A Fast Parallel Algorithm for Thinning Digital Patterns. T. Y. ZHANG and C. Y. SUEN.
    // http://www-prima.inrialpes.fr/perso/Tran/Draft/gateway.cfm.pdf
    
    // Что нужно доделать (6 апреля 2019): ////////////////////////////////////////////////////////////////////
    // "Как только массив станет равным предыдущему массиву, процесс останавливается." эту проверку p0_64 лучше
    // заменить на проверку второго массива, которую все равно уже добавили для отработки исключений.
    
    
    //  * Особенности *
    // Функция потенциально не быстрая т.к. 14 раз пробегает весь исходный байтовый массив (при n_RemovLayers=7)
    // Поэтому:
    // - используются предпросчитанные таблицы, что улучшает общее быстродействие функции в 2 раза
    // - обрабатываются (подвергаются эррозии) только чернные пиксели в тексте (точки d1), перепрыгивая через белые области,
    // что улучшает общее быстродействие функции еще в 5-6 раз.
    // Можно пробегать сразу блоками по 64 pix, но это значительно более сложный код, примерно как в
    // функции "Полиграфическое растискивание dotGain()", зато на порядок быстрее.
    //  * Особенности применения *
    // Превышение количества удаленных слоев n_RemovLayers выше определенного значения не влияет на качество получаемой
    // фокальной линии (графемы) букв, а приводит только к замедлению работы программы.
    // Первый подход - априорное определение минимального количества удаленных слоев. Один удаленный слой
    // (слой это периметр символа т.е. 2 pix) и это 2 единици толщины штриха. Необходимое количество удаленных слоев
    // равно половине средней толщины штриха плюс еще одна половина средней толщины штриха, как компромисный запас
    // на особо толстые линии в изображении. Окончательное значение n_RemovLayers равно средней толщине штриха в pix.
    // Среднею толщину штриха по всему графическому тексту (изм. в десятых долях pix) можно получить с помощью
    // функции "Определение толщины штриха lineWidth()". Поэтому n_RemovLayers=line_wd/10;  где int line_wd=lineWidth()
    // Проще задать вручную при вызове программы.
    // Обычно компромисное значение количества удаленных слоев n_RemovLayers=7 (скорее 9).
    // Второй подход - определение минимального количества удаленных слоев в самом процессе работы этой программы.
    // При снятии слоя пикселей за слоем с какой-то момент графическая картинка (текст) перестанет изменятся.
    // Как только массив станет равным предыдущему массиву, процесс останавливается.
    // Практически удобно комплексировать оба метода. А именно, задаем максимальное разумное количество слоев,
    // (например 9), но если графическая картинка перестанет изменятся при снятии слоев, программа закончит работу
    // и не будет снимать последовательно все 9*2 слоев. Сейчас так и реализованно.
    
    int GrafT=0;
    // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT) { DG("Функция определения фокальных линий букв (графем)   focalLine()   n_RemovLayers="<<n_RemovLayers<<endl); }
    
    //TIME_START
    
    int x,y;
    int w,h;
    w=ncolumns;  // ширина массива  IMAGE WIDTH
    h=nrows;     // высота массива  IMAGE HEIGHT
    int w_h=w*h; // длина массива  IMAGE
    
    if (GrafT) { DG("w="<<w<<",  h="<<h<<"    w*h="<<w*h/1000000<<" млн. пикселей"<<"\n"); }
    // w*h=4 млн. пикселей time=0.26/2 // w*h=10 млн.  time=0.68/2  // 4.3/2  w*h=39 млн., n_RemovLayers=15
    
    /// управляющие переменные ///
    int n_Lay=n_RemovLayers; // количество удаленных слоев (слой это периметр символа) n_Lay=7;
    int min_RemovLayers=n_RemovLayers; // возвращаемое функцией фактическое количество удаленных слоев.
                                       // Вполне м.б. min_RemovLayers < n_RemovLayers
    uchar scaling=1; // Уменьшение изображения фокальной линии (графемы) букв в "scaling" раз (2,3,4,5,...) с
                             // сохранением площади черного. Серая подложка не выводится. При scaling=1 выводится изображение 1:1 с серой подложкой.
    
    if (GrafT){ DG("количество удаленных слоев   n_Lay="<<n_Lay<<"\n");}
    
    /// вспомогательные переменные
    int n=0;     // переменная по количеству удаленных слоев со всех сторон
                 //int m=0;
    uchar rg0=0;  // байтовый регистр для хранения отдельных 8 бит маски
    uchar d2,d3,d4,d5,d6,d7,d8,d9; // d1,  // переменные маски
    uchar a,b,c,d; // переменные условий
    int flag1=0, flag2=0; // флаги попиксельного равенства исходнного и подвергнутого эррозии массивов,
                          // if (flag1==0 && flag2==0) при этом условии все штрихи достигают пиксельной толщины.
    
    // вспомогательные переменные для предотвращения выхода за пределы массива
    int w_1=w+1; // смещение с угла в центр матрицы (маски)
    int w_h_=w*h-(w+1); // -(0)
    
    
    /// Массивы.///
    // указатели типа uchar
    uchar *p0=bytes_data, *pw;    // входной массив, 255-черный 0-белый ( с 1-черный 0-белый не работает)
    uchar *bytes_data_1, *p1;     // такой же как массив bytes_data. Оба массива работают по очереди.
    
    //////////////
    // выделение памяти под массивы.
    bytes_data_1 = (unsigned char*)malloc(w_h+64);
    
    uchar table_1[256];  // таблица для удаления граничных точек справа и внизу (снятие слоя точек справа и внизу) //
    uchar table_2[256];  // таблица для удаления граничных точек слева и сверху (снятие слоя точек слева и сверху) //
    
    // указатели на те же массивы типа ulong для быстрых циклов
    uint64 *bytes_data0_64=(uint64*)bytes_data;    // приведение к uint64
    uint64 *bytes_data1_64=(uint64*)bytes_data_1;  // приведение к uint64
    uint64 *p0_64, *p1_64;
    
    ///uint64 *p0_64=(uint64*)bytes_data;    // приведение к uint
    ///uint64 *p1_64=(uint64*)bytes_data_1;  // приведение к uint
    
    // предварительные данные для циклов определяющих момент остановки процесса удаления слоев,
    // по достижения фокальной линии пиксельной толщины.
    int sf_rg=sizeof(uint64);
    //int w_h_64=w_h/sf_rg;  // sizeof(uint64)=8
                           ///cout<<"w_h="<<w_h<<"  (w_h_32)*32="<<(w_h_32)*32<<endl;
    
    // отступы от краев (бордюр) x0, x1, y0, y1, блокируют влияние грязи с краев текста на процесс остановки эрозии
    // графической картинки по достижении однопиксельной толщины штриха. Возможный захват бордюром не большой части
    // текста в данном случае не критичен т.к.эрозия графической картинки происходит все равно по всей площади текста.
    // Но решение об остановке эрозии принмается только по центральной части картинки, без бордюров, поэтому лучше br=16
    int br=16;   // величина отступа от краев, как часть высоты и ширины изображения (1/8). Лучше 1/16
    int x0, x1, y0, y1; // отступы от краев (бордюр).
    x0=(w/br)/sf_rg;  x1=(w-w/br-1)/sf_rg;  y0=h/br;  y1=h-h/br-1;
    DG("sf_rg="<<sf_rg<<"  x0="<<x0<<"  x1="<<x1<<"  y0="<<y0<<"  y1="<<y1<<"\n");
    /**/
    
    
    
    ///TIME_START
    // Заполнение массива BufS адресами начала и конца черных слайсов (для перепрыгивания белых областей).
    // Эррозия работает только с черным в тексте, а площадь черного на порядок меньше площади
    // белого (в 30 и более раз), что уменьшает полное время выполнения функции с time=0.66 до time=0.33.
    pw=bytes_data + w_1;  // w_1=(w+1); смещение с угла в центр матрицы (маски)
                          // Вспомогательный массив для работы маски.
                          // массив uint размером в исходное изображение, где каждый pix фокальной линии заполнен id_num (ushort).
    line_data = (OCRData*)calloc(w_h+32,sizeof(uint)); // Каждый элемент инициализируется в 0.
                                                        // В начале работы алгоритма:
                                                        // массив сохраняет адреса начала и конца черных слайсов.
    initLine_flag=1;                                  // флаг инициализации массива line_data
    
    // Временное заполение массива bytes_data_32 адресами int начала и конца черных слайсов исходного изображения.
    // Необходимо для определения размера рабочего массива BufS (slice_address).
    uint g=0; // переменная адресов начала и конца черных слайсов исходного изображения.
    uint s;
    uint s_old=0 ;    //bytes_data[w]; // w_1-1=w+1-1=w    // 255-белый
    
    // Создание массива с адресами начала и конца черных слайсов.
    // массив слайсов создается для работы маски эррози которая смещена относительно
    // координат пиксала на  -(w+1). Поэтому в массив слайсов заносится координата
    // черного пиксела со смещением w+1
    // Цикл по всей исходной картинке.
    pw=bytes_data + w_1;  // w_1=(w+1); смещение с угла в центр матрицы (маски)
    uint *bytes_data32=(uint*)line_data;
    
    for (int x=0; x<w_h_; x++){
        s=pw[x];   // исходная картинка, 255-белый.
        // заполнение массива адресами начала и конца черных слайсов.
        if ((s-s_old)!=0) {
            bytes_data32[g]=x;
            g++;
        }
        s_old=s;
    } // x
    
    
    sizeBufSR=g; // Размер рабочего массива BufS.
    DG("Размер рабочего массива BufS в байтах sizeBufS="<<sizeBufSR<<endl);
    
    //uint *BufS; // указатели типа uint
    // массив адресов начала и конца слайсов (slice) и предназначен для перепрыгивания белых областей.
    
    BufS=(uint*)malloc(sizeBufSR*sizeof(uint)+32);
    // заполнение массива адресов начала и конца слайсов
    memcpy(BufS, line_data, sizeBufSR*sizeof(uint));
    
    // Приведение массива line_data в исходное состояние
    memset(line_data, 0, w_h*sizeof(uint)+32);     // заливка bytes_data_32 "0"
    
    
    //   n_Lay=14;  // test
    
    // матрица (маска) обсчета (3*3-1), где d1 центральной точка по которой принимается решение удалять - не удолять.
    // d2, d3, d4, d5, d6, d7, d8, d9  точки окружения, по которым работает алгоритм принятия решения.
    /*
     d9   d2   d3
     
     d8   d1   d4
     
     d7   d6   d5
     */
    
    // Создание предпросчитанных таблиц table_1 и table_2 по условию (a = 1) && (2 <= b <= 6) && (c = 0) && (d = 0)
    // В таблице table_2 по отношению к table_1 изменен только способ подсчета "c=d2*d4*d8" и "d=d2*d6*d8"
    
    // цикл перебора всех возможных значений матрицы (3*3-1) // time=0
    for (int index=0; index < 256; index++){
        rg0=index;
        table_1[index]=table_2[index]=255; // заполнение таблиц единицами // убрать 2 else { table_1[rg0]=255; }
        
        // разборка байтового регистра на отдельные 8 бит
        d9=rg0 & 1;    d2=rg0>>1 & 1; d3=rg0>>2 & 1; d8=rg0>>3 & 1;
        d4=rg0>>4 & 1; d7=rg0>>5 & 1; d6=rg0>>6 & 1; d5=rg0>>7 & 1;
        // АЛГОРИТМ //
        a=0; // "a" это число паттернов "01" в упорядоченном множестве d2, d3, d4, d5, d6, d7, d8, d9,(d2)
        if(d3-d2>0)a++; if(d4-d3>0)a++; if(d5-d4>0)a++; if(d6-d5>0)a++;
        if(d7-d6>0)a++; if(d8-d7>0)a++; if(d9-d8>0)a++; if(d2-d9>0)a++;    //  a = 1 - условие N1
                                                                           // "b" это число ненулевых соседей центральной точки d1, т.е, она имеет по крайней мере, 2 ненулевых соседа, но не более 6.
        b=d2+d3+d4+d5+d6+d7+d8+d9; //  2 <= b <= 6 - условие N2
                                   // По крайней мере, одна из точек d2, d4, d6 являются фоновой
        c=d2*d4*d6;  //  c = 0 - условие N3
                     // По крайней мере, одна из точек d4, d6, d8 являются фоновой
        d=d4*d6*d8;  //  d = 0 - условие N4
                     // заполнение таблицы первой половины итерации (subiteration)
        if ( b<=6 && b>=2 && a==1 && c==0 && d==0 ) table_1[index]=0;
        
        // По крайней мере, одна из точек d2, d4, d8 являются фоновой
        c=d2*d4*d8;  //  c = 0 - условие N3
                     // По крайней мере, одна из точек d2, d6, d8 являются фоновой
        d=d2*d6*d8;  //  d = 0 - условие N4
                     // заполнение таблицы второй половины итерации (subiteration)
        if ( b<=6 && b>=2 && a==1 && c==0 && d==0 ) table_2[index]=0;
        
        //cout<<" "<<(unsigned short)table_2[index];
    } // for
    
    ///исключения конечного пиксела 45 градусной линии, август 2019.
    table_1[192]=table_2[192]=255;
    table_1[144]=table_2[144]=255;
    table_1[96]=table_2[96]=255;
    table_1[40]=table_2[40]=255;
    table_1[20]=table_2[20]=255;
    table_1[9]=table_2[9]=255;
    table_1[6]=table_2[6]=255;
    table_1[3]=table_2[3]=255;
    
    if (GrafT) {DG("значение байтового регистра для хранения отдельных 8 бит маски, в конце цикла   rg0="<<(ushort)rg0<<"\n");}
    // cout<<endl;
    
    
    // создание массива и его заполнение для хранения и визуализвции серой подложки букв из исходного изображения
    uchar *bytes_data_2=NULL;
    if (GrafT) {
        bytes_data_2 = (uchar*)malloc(w_h);
        memcpy(bytes_data_2, bytes_data, w_h);  // копирование bytes_data в bytes_data_2
    } // if
    
    
    // инверсия уровней входного массива общепринятого стандарта 0-черный 255-белый, во внутренний формат программы 0-белый 255-черный
    //for ( x=0; x < w_h; x++ ) { if (bytes_data[x]) bytes_data[x]=0@;  else bytes_data[x]=255;} // по 1 байту  // else bytes_data[x]=1;
    //for ( x=0; x < w_h_64; x++ ) { bytes_data0_64[x]=~bytes_data0_64[x]; } // по 8 байт одновременно
    
    // Для нормальной работы алгоритма перед началом цикла по "x" в массивах bytes_data и bytes_data_1
    // должны быть одни и те же данные. В этом смысл всех memcpy() в данной программе.
    // копирование bytes_data в bytes_data_1  // time=0.003  w_h=w*h=10 млн. пикселей // time=0.003*14=0.042
    memcpy(bytes_data_1, bytes_data, w_h);
    
    
    /// Цикл по количеству удаленных слоев со всех сторон. /// time=0.24  w*h=10 млн. пикселей  n=8
    pw=bytes_data + w_1;    // w_1=(w+1); смещение с угла в центр матрицы (маски)
    p1=bytes_data_1 + w_1;
    
    //printToScreenBinary(); cout<<endl; //exit(0);
    
    //n_Lay=2;
    for ( n=0; n < n_Lay; n++ ) {
        
        // Удаление граничных точек справа и внизу (снятие одного слоя точек справа и внизу) //
        // В первой половине итерации (subiteration), точка контура d1 удаляется из изображения,
        // если она удовлетворяет следующим условиям: (a = 1) && (2 <= b <= 6) && (c = 0) && (d = 0)
        // условие реализованно в виде предросчитанной таблицы table_1[]
        
        // цикл обеспечивает обработку (эррозию) только черных пикселей в тексте (точки d1), перепрыгивая через белые области.
        for ( g=0; g < sizeBufSR; g+=2 ) { //  w_h_=w*h-(w+w+4);  // time=0.18/2  только цикл ( n_Lay=7, w*h=4 млн. пикселей )
                                           // цикл обеспечивает удаление точки контура d1 с помощью таблицы.
            for ( x=BufS[g]; x < BufS[g+1]; x++ ) {
                // если точка контура d1 равна нулю, то ее не зачем просчитывать и удалять.
                if (*(pw + x)!=0) {  // pw=bytes_data + w_1;   // w_1=(w+1);
                    p0=bytes_data+x;
                    
                    // сборка байтового регистра rg0 из отдельных 8 бит матрицы с помощью единичных масок
                    rg0=(*(p0)&1) + (*(p0+1)&2) + (*(p0+2)&4) + (*(p0+w)&8) + (*(p0+w+2)&16) + (*(p0+w+w)&32) + (*(p0+w+w+1)&64) + (*(p0+w+w+2)&128);
                    
                    if(rg0==11){ //проверка исключения (одиночный пиксел)
                        uchar *p2=bytes_data_1+x;
                        uchar rg1=(*(p2)&1) + (*(p2+1)&2) + (*(p2+2)&4) + (*(p2+w)&8) + (*(p2+w+2)&16) + (*(p2+w+w)&32) + (*(p2+w+w+1)&64) + (*(p2+w+w+2)&128);
                        //if( GrafT>4)pv("\n исключение rg1:",rg1);//printPointS(x);
                        if(rg1==0)continue;
                    }
                    
                    //if( GrafT>4&&rg0>0){pv("\niter1 rg0:",rg0);printPointS(x);}
                    if ( rg0>1 && table_1[rg0]==0 ) {
                        *(p1+x)=0;
                        //if( GrafT>4)DG("\@@@@erase ");
                    } // p1=bytes_data_1 + w_1;  // time=0.66/2
                    
                } // if
            } // x
        } // g
        
        /**/
        // bytes_data= bytes_data_1;
        //printToScreenBinary(); cout<<endl; //exit(0);
        
        // Как только удаление граничных точек справа и внизу перестанет изменять массив (все штрихи достигли пиксельной толщины),
        // устанавливаем  флаг в ноль.
        flag1=0;
        for (y=y0; y < y1; y++){  // time=0.0003 w*h=4 млн. пикселей
            s=(y*w)/sf_rg;
            p0_64=bytes_data0_64 + s;
            p1_64=bytes_data1_64 + s;
            for (x=x0; x < x1; x++){ // х управляет указателем на 64 (sf_rg)  // int x
                if ( p0_64[x]!=p1_64[x] ) { flag1++; goto break1; }  // goto break1;  break;
            } // x
        } // y
    break1:;
        if (GrafT){DG(" flag1="<<flag1);}
        /**/
        
        // в массивах bytes_data и bytes_data_1 должны быть одни и те же данные.
        memcpy(bytes_data, bytes_data_1, w_h);  // адрес буфера назначения, адрес источника, количество байт для копирования
        
        // Как только удаление граничных точек слева и сверху, справа и внизу перестанет изменять массив, выходим из общего цикла по "n".
        if (flag1==0 && flag2==0) {break;}
        
        
        // Удаление граничных точек слева и сверху (снятие одного слоя точек слева и сверху) //
        // Во второй половине итерации (subiteration), точка контура d1 удаляется из изображения,
        // если она удовлетворяет следующим условиям: (a = 1) && (2 <= b <= 6) && (c = 0) && (d = 0)
        // условие реализованно в виде предросчитанной таблицы table_2[]
        // Во второй половине итерации (subiteration), изменились только следующии условия относящиеся к "c" и "d":
        // d2 * d4 * d8 = 0     //  d2 * d6 * d8 = 0
        
        // цикл обеспечивает обработку (эррозию) только чернных пикселей в тексте (точки d1), перепрыгивая через белые области.
        
        for (int g=0; g < sizeBufSR; g+=2 ) {
            // цикл обеспечивает удаление точки контура d1 с помощью таблицы.
            for ( x=BufS[g]; x < BufS[g+1]; x++ ) {
                // если точка контура d1 равна нулю, то ее не зачем удалять.
                if (*(pw + x)!=0) {  // pw=bytes_data + w_1;   // w_1=(w+1);
                    p0=bytes_data+x;
                    /*
                     d9=*p0 & 1;         d2=*(p0+1) & 2;       d3=*(p0+2) & 4;
                     d8=*(p0+w) & 8;     d1=*(p0+w+1) & 1;     d4=*(p0+w+2) & 16;
                     d7=*(p0+w+w) & 32;  d6=*(p0+w+w+1) & 64;  d5=*(p0+w+w+2) & 128;
                     */
                    // сборка байтового регистра rg0 из отдельных 8 бит матрицы с помощью единичных масок
                    // rg0=d9 + d2 + d3 + d8 + d4 + d7 + d6 + d5;
                    rg0=(*(p0)&1) + (*(p0+1)&2) + (*(p0+2)&4) + (*(p0+w)&8) + (*(p0+w+2)&16) + (*(p0+w+w)&32) + (*(p0+w+w+1)&64) + (*(p0+w+w+2)&128);
                    //if(rg0>0){DG(" iter2 ");printPointS(x);}
                    
                    if ( rg0>1 && table_2[rg0]==0 ) { *(p1+x)=0; } // time=0.66/2   // if ( table_2[rg0]==0 ) { *(p1+w+1)=0; } // time=0.7/2
                } // if
            } // x
        } // g
        /**/
        //printToScreenBinary(); cout<<endl; //exit(0);
        // Как только удаление граничных точек слева и сверху перестанет изменять массив (все штрихи достигли пиксельной толщины),
        // устанавливаем  флаг в в ноль.
        flag2=0;
        for (y=y0; y < y1; y++){
            s=(y*w)/sf_rg;
            p0_64=bytes_data0_64 + s;
            p1_64=bytes_data1_64 + s;
            for (x=x0; x < x1; x++){ // х управляет указателем на 64 (sf_rg)
                if ( p0_64[x]!=p1_64[x] ) { flag2++; goto break2; } // goto break2; break;
            } // x
        } // y
    break2:;
        if (GrafT) {DG(" flag2="<<flag2);}
        /**/
        
        
        // в массивах bytes_data и bytes_data_1 должны быть одни и те же данные.
        memcpy(bytes_data, bytes_data_1, w_h);  // адрес буфера назначения, адрес источника, количество байт для копирования
        
        // Как только удаление граничных точек слева и сверху, справа и внизу перестанет изменять массив, выходим из общего цикла по "n".
        if (flag1==0 && flag2==0) {break;}
        
    } // n
    
    
    //после обработки массива отрабатываем исключение - эрозия четырех пикселов
    //образующих квадрат из которого исходят четыре линии (свастика)
    
    for ( g=0; g < sizeBufSR; g+=2 ) {
        // цикл обеспечивает удаление точки контура d1 с помощью таблицы.
        for ( x=BufS[g]; x < BufS[g+1]; x++ ) {
            // если точка контура d1 равна нулю, то ее не зачем удалять.
            if (*(pw + x)!=0) {  // pw=bytes_data + w_1;   // w_1=(w+1);
                p0=bytes_data+x;
                /*
                 d9=*p0 & 1;         d2=*(p0+1) & 2;       d3=*(p0+2) & 4;
                 d8=*(p0+w) & 8;     d1=*(p0+w+1) & 1;     d4=*(p0+w+2) & 16;
                 d7=*(p0+w+w) & 32;  d6=*(p0+w+w+1) & 64;  d5=*(p0+w+w+2) & 128;
                 */
                // сборка байтового регистра rg0 из отдельных 8 бит матрицы с помощью единичных масок
                // rg0=d9 + d2 + d3 + d8 + d4 + d7 + d6 + d5;
                rg0=(*(p0)&1) + (*(p0+1)&2) + (*(p0+w)&4) + (*(p0+w+1)&8);
                if(rg0==15){
                    //найдено исключение - квадрат четырех пикселов
                    //проверяем есть ли пиксел который отходит под прямым углом
                    //проверяем левый верхний пиксел квадрата
                    if(*(p0 - w) || *(p0 - 1)){
                        *p0=0;
                        break;
                    }
                    //проверяем правый верхний пиксел квадрата
                    if(*(p0 - w + 1) || *(p0 + 2)){
                        *(p0 + 1)=0;
                        break;
                    }
                    //проверяем левый нижний пиксел квадрата
                    if(*(p0 + w + w) || *(p0 + w - 1)){
                        *(p0 + w)=0;
                        break;
                    }
                    //проверяем правый нижний пиксел квадрата
                    if(*(p0 + w + w + 1) || *(p0 + w + 2)){
                        *(p0 + w + 1)=0;
                        break;
                    }
                    //в случае если все линии выходят из квадрата по диагоналям
                    //передвигаем нижний правый пиксел квадрата
                     *(p0+w+1)=0;
                     *(p0+w+2)=1;
                }
                
            } // if
        } // x
    } // g
    
    
    
    if (GrafT) { DG("\n"<<"Окончание цикла по количеству удаленных слоев   n="<<n<<"\n"); }
    
    //фактическое количество удаленных слоев.
    min_RemovLayers=n;
    
    // визуализация серым фокальных линий (по 8 байт одновременно). ( внутренний формат программы 255-черный, 0-белый ).
    ///for ( x=0; x < w_h_64; x++ ) { bytes_data0_64[x]=(bytes_data0_64[x]>>1)&0x7F7F7F7F7F7F7F7F; } // деление на 2 и прим. 64 р маски.
    // Черное (255) становится серым (127), белое (0) остается белым.
    // 0x7F7F7F7F7F7F7F7F  64 р маска для деление на 2, маска убирает передвинувшиеся (при сдвиге) из соседних байт разряды.
    
    
    // приведение массива к общепринятому стандарту, 0-черный 255-белый ( внутренний формат программы 0-белый 255-черный ).
    // for ( x=0; x < w_h; x++ ) { if (bytes_data[x]>0) bytes_data[x]=0;  else bytes_data[x]=255; } // по 1 байту   //time=0.03  w_h=w*h=10 млн. пикселей
    //for ( x=0; x < w_h_64; x++ ) { bytes_data0_64[x]=~bytes_data0_64[x]; } // по 8 байт одновременно  // time=0.005 w*h=10 млн. пикселей
    
    // создание серой подложки из исходного изображения для фокальной линии (графемы) букв. Не используем, переделано на 32
    //if (GrafT) {for ( x=0; x < w_h; x++ ){ bytes_data[x]=(bytes_data_2[x] + bytes_data[x])/2; }  }  // time=0.05 w*h=10 млн. пикселей
    
    // printToScreen(); exit(0);
    
    // Уменьшение изображения в "scaling" раз с сохранением площади черного. Если надо. Без серой подложки. // time=0.012  w*h=4 млн. пикселей
    // Изображение берем в bytes_data, сохраняем уменьшенное изображение в bytes_data_1 и копируем обратно в bytes_data.
    if (scaling>1) {
        memset(bytes_data_1, 255, w_h); // заливка bytes_data_1 белым
        scaling=2; // int y;
        for (y=0; y < h; y++){          // time=0.012  w*h=4 млн. пикселей
            p0=bytes_data + y*w;
            p1=bytes_data_1 + (y/scaling)*w;
            for (x=0; x < w; x++){
                if (*(p0+x)==0) *(p1+x/scaling)=0; // 0-черный для общепринятого стандарта
            } // x                                 // 255-черный для внутреннего формата
        } // y
        memcpy(bytes_data, bytes_data_1, w_h);     // адрес буфера назначения, адрес источника, количество байт для копирования
    } // if
    /**/
    
    /*
     // тест области сравнения массивов bytes_data0_64 и bytes_data1_64
     for (y=y0; y < y1; y++){
     s=(y*w)/sf_rg;
     p0_64=bytes_data0_64 + s;
     p1_64=bytes_data1_64 + s;
     for (int x=x0; x < x1; x++){
     p0_64[x]=0;
     // *(p0_64+x)=0;
     } // x
     } // y
     */
    
    /*
     // тест области работы алгоритма, эррозия работает только с черным в тексте.
     for ( g=0; g < sizeBufSR; g+=2 ) {
     for ( x=BufS[g]; x < BufS[g+1]; x++ ) { bytes_data[x]*=0.5; } // 0-черный
     } // for
     */
    
    
    // освобождение массивов
    if ( bytes_data_1 !=NULL ) free(bytes_data_1);
    if (GrafT) { if ( bytes_data_2 !=NULL ) free(bytes_data_2); }
    
    //TIME_PRINT_   cout<<"TIME_PRINT_   фокальных линий   focalLine"<<endl;
    
    // Возвращаемое функцией фактическое количество удаленных слоев как глобальная переменная.
    // Вполне м.б. что функция вычислит фокальной линии (графемы) букв
    // за меньшее число тактов min_RemovLayers, чем ее было предложено на входе n_RemovLayers.
    nRemovLayers=min_RemovLayers;
    
    //---------------------------------------------------------------------------------------------------------
    
    //for ( x=0; x < w_h_ && g < sizeBufS; x++ ) {  // time=0.07  w*h=10 млн. пикселей
    //////  int x0=(128*w)/sf_rg;     int x1=w_h_64-(128*w)/sf_rg-1;  // для цикла без отступов.
    ///     for ( x=x0; x < x1; x++ ) { if ( bytes_data0_64[x]!=bytes_data1_64[x] ) flag1++; }  cout<<" flag1="<<flag1;  // time=0.002  w*h=4 млн. pix
    ///     for ( x=x0; x < x1; x++ ) { if ( bytes_data0_64[x]!=bytes_data1_64[x] ) flag2++; }  cout<<" flag2="<<flag2;
    
    //---------------------------------------------------------------------------------------------------------
    
    
    
}//____________________________________________________________________________