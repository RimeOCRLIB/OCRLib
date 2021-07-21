//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .

#include "GGraphBezier.h"
#include <math.h> 
#ifdef ASM_OS64_SSE42_POPCNT
#include <immintrin.h> // заголовочный файл immintrin.h с описанием соответствующих intrinsic-функций.
#endif


namespace ocr{

    
    //*****************************************************************************
    
                        /// Test ///
    
    // Функция для тестирования алгоритмов обработки кривых Безье.
    void GGraphBezier::Test(OCRFocalLineBezier &line1, OCRFocalLineBezier &line2){
        
#ifdef ASM_OS64_SSE42_POPCNT
        int GrafT=1;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if(GrafT){
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для тестирования алгоритмов обработки кривых Безье   Test() "<<endl;
        }



        
        //--------------------------------------------------------------------------------------------------------------------------
        
        // разрывы и лишние точки диагностируются смежными векторам
        
        // —————————————————
        
        // нейросети.
        // регистровая кодировка кривых Безье.
        // Кнутовский хеш составленный из двухбитовых хешей.
        
        // Из корпуса текстов можно и извлеч словарь фраз или статистику (вероятности) соседних слов.
        // Это называют Марковскими цепями. Словарь фраз перекрывает текст на 70 процентов,
        // а вероятности могут перекрыть на 100 процентов.
        
        
        // —————————————————

        //// ++ найти С функцию расстояния между точками.
        //// +заменить везде CorAlphaP1=100-100*deltaAlpha/90;        на      CorAlphaP1=100-1.11111111*deltaAlpha;
        // +влияние длины опорной линией на общий коэффициент корреляции углов лучше уменьшить в два раза. // wgL*0.25;
        // +вместо опорной линии лучше использовать вектор lenP1P2
        // +004 и 2006 не точно подсчитаны длинны опорных векторов
        // +разобраться с функцией масс  (последние несколько строк)
        
        // 0) Апроксимировать исходные линии более короткими (минус несколько pix для этого есть готовая функция) кривыми Безье,
        // по ним скорректировать несколько крайних pix фокальной линии, убрав не точности линий около особых точек.
        // Заново подсчитать кривую Безье по полной длинне фокальной линии. Сравнить с исходными длинными и принять решение
        // какую кривую Безье оставить.
        // 1) перебрать все линии (как в GGraphBezierFocalLineRecognition точки) и составить цепочки из кривых которые стыкуются.
        // 2) апроксимировать склеенные линии Безье, проверить качество апроксимации с помощью длины.
        //  Новое Безье использовать как гипотезу для проверки эталоном из БД.
        // 3) линии кругов стыкуются плохо, нужен более интегральный способ.
        // 4) Отдельно сравнить все длины линий и соответствующих кривых Безье, где есть расхождения
        //  более внимательно поискать углы, до исчезновения расхожденний.
        //  Углы при существующем алгоритме это 3 корня в каждой точке.
        
        // практически //
        // Устранение не точностей линий около особых точек в темпе лета. Совпадает с 1). Построить Безье короче на несколько pix,
        //  с обоих сторон, линейно апроксимировать конци этого Безье на те же несколько pix. Поставить новые точки начала и конца.
        //  Построить по этим точкам новое Безье и использовать его как обычно. И только за тем можно строить цепочки в отдельной функции.
        // + Переписать на таблицу сравнение большой линии и меньшей
        //  и потестировать.
        // + РЕВЕРС. начало и конец векторов определять двумя проверками каждого вектора, а не по всей кривой в целом.
        
        // + таблица  CorAlphaL=f(deltaAlpha);
        // + Линии на входе Функция для сравнения (корреляции) двух кривых Безье, можно вычислять по более простому алгоритму (1 вектор).
        
        // + добавить термин "узлы"
        
        
        
        // идеи //
        // На входе можно поставить проверку типов особых точек на концах проверяемое кривой, для ранней отсечки.
        //  Проверяем сумму типов особых точек.
        // На входе можно поставить проверку типов линий: прямая, кривая первого порядка,  кривая второго порядка, круг,.... ,
        //  но только для ранней и очевидной отсечки, и упрощения подсчета к.к. На сам к.к. тип линии не влияет.
        //  Используем тип линии и переходную область. Для далеких по типологии линий к.к. можно не считать к.к.=0.

        // Геометрический код описания слова.
  
        
        // —————————————————
        
        //  Условие непрерывности соседних кривых Безье "получаются путем приравнивания радиус-векторов и касательных векторов"
        // file:///__INFORM__/___NEV_INT_2015___/__кривые%20безье,%20центр%20масс__/Kursovaya_rabota_-_kopia.webarchive
        // file:///__INFORM__/___NEV_INT_2015___/__кривые%20безье,%20центр%20масс__/Кривые%20Безье%20для%20ваших%20игр:%20учебник%20-%203D-Orange.com.ua.webarchive
        
        //--------------------------------------------------------------------------------------------------------------------------
        
        //Условие непрерывности соседних кривых Безье:
        //если необходимо соблюдать непрерывность кривизны и наклона в точках соединения кривых, то требуется знать как первую, так и вторую производную кривой Безье.
        
        //
        //      B1  B2
        //  B0          B3C0        C2  C3
        //                      C1
        //
        
        // Пусть заданы вершины многоугольника Безье B0(1,1), B1(2,3), B2(4,5), B3(3,1). В наших обозначениях Px0,Py0 .....
        
        // Отсюда первые производные в концах будут (можно получить в любой промежуточной точке)
        // P'(0)=3(B1-B0)
        // P'(1)=3(B3-B2)
        
        // вторые производные в концах таковы:
        // P''(0)=6(B0-2B1+B2)
        // P''(1)=6(B3-2B2+B1)
        
        // направления касательных на стыке совпадают, если три вершины , ,  коллинеарны, т.е.  должна лежать на линии между
        // Если совпадают еще и величины касательных векторов, то  является серединой отрезка от  до
        // C1+B2=2C0=2B3
        
        // Условие непрерывности вторых производных между соседними кривыми:
        // 6(B0-2B1+B2)=6(C3-2C2+C1) // переменные слева и справа относятся к разным уравнениям // 6((B2-B1)-(B1-B0))=6((C3-C2)-(C2-C1))
        
        // Из условий  и  непрерывности в точке соединения получаем Положение третьей определяющей вершины второго многоугольника, выраженное через три последние вершины первого многоугольника, а именно:
        // C2=B1-4(B2-B3)
        
        // не забыть отнормировать по t (длинне кривой)
        
        // В наших обозначениях
        // Px2=Px1-4(Px2-Px3)   // переменные слева и справа относятся к разным уравнениям L1 и L2
        // Это означает, что вершины B1, B2, B3 = C0, C1, C2  должны либо составлять выпуклый многоугольник, либо лежать на одной прямой, чтобы сохранить  непрерывность на стыке. Несколько карандашных набросков на бумаге покажут, что данное требование существенно ограничивает множество кривых; поэтому на практике для соблюдения непрерывности вторых производных используются полиномиальные кривые более высокого порядка
        
        //  | 3(B3-B2)=3(C1-C0) |        B3-B2=C1-C0
        //  | 6(B3-2B2+B1)=6(C0-2C1+C2) |        B3-2B2+B1=C0-2C1+C2=-(C1-C0)-C1+C2=-(B3-B2)-C1+C2=-B3+B2-C1+C2
        //  | 2B3-3B2+B1=-C1+C2 |
        //  | B3=C0 |
        //  | C1+B2=2C0=2B3 |   -C1=B2-2B3         2B3-3B2+B1=B2-2B3+C2     2B3-3B2+B1-B2+2B3=C2    C2=+B1+4B3-4B2  C2=B1-4(B2-B3)
        //// C2=B1-4(B2-B3) ////
        
         // Дополнительную гибкость кривой можно также получить разбиением кривой Безье на две новых таким образом, чтобы они вместе совпадали с исходной кривой.
        
        //  стыковка двух кривых Безье //
        //  | B3-B2=C1-C0 |     C1+B2=B3+C0 и если B3=C0 то следует автоматически C1+B2=2C0=2B3 и наоборот
        //  | B3=C0 |
        //  | C1+B2=2C0=2B3 |
        
        // http://sernam.ru/book_mm3d.php?id=92
        // http://edu.dvgups.ru/METDOC/ENF/SAPR/S_KURS_V_GEOM/METOD/MOD_SPLAYN/Graf_9.htm
        
        
        //--------------------------------------------------------------------------------------------------------------------------
        
        
        //
        //http://spacelib.narod.ru/p_fnd.html
        
        // о трёхзначной логикие
        // У меня самого логика однозначная: я всегда хочу. Есть, спать, бездельничать, развлекаться, ну и всё такое.
        // А многозначную логику вредно иметь.
        // http://dxdy.ru/topic69059.html
        
        // —————————————————
        
        // терминология по кривой Безье.//
        // Давайте различать узлы и контрольные точки.
        // Узлы --- это те три точки P_0, P_1, P_2, через которые проходит Ваш профиль. На каждой ходре, P_0 P_1 и P_1 P_2, Вы дополнительно выбираете 4 контрольные точки; получаются контрольные полигоны P_0 A_0 B_0 C_0 D_0 P_1 и P_1 A_1 B_1 C_1 D_1 P_2. Кривая через дополнительные контрольные точки не проходит, с их помощью Вы управляете формой и, видимо, кривизной в месте сшивки.
        // http://dxdy.ru/topic16478-15.html
        
        
        
        // —————————————————
        

        
        //Поворот точки на плоскости.
        // xr=xcosθ−ysinθ, yr=xsinθ + ycosθ.
        
        // Андрэ Ламота "Программирование игр для Windows. Советы профессионала"
        
        // —————————————————
        
        // Полярная система координат
        // https://ru.wikipedia.org/wiki/ Полярная_система_координат
        
        // —————————————————
        
        // фильтры //
        
        //  «Low-pass filter».
        // a — текущее значение переменной, b — следующее значение, dt — время в миллисекундах между кадрами (промежуток времени),
        // RC — некий коэффициент (чем больше значение, тем больше сглаживание) например постоянная времени RC=0.85.
        
        // функция для сглаживания входных данных:
        /// def filter(a, b, dt, RC): // например RC=0.85.
        /// t = dt / (RC + dt)      // α = dt / (RC + dt)
        /// return a + t * (b - a)  // y[i] = y[i-1] + α * (x[i] - y[i-1])  /// y[i] = (y[i-1] + x[i])/2; при α=0.5;
        
        // http://blog2k.ru/archives/category/math
        // https://en.wikipedia.org/wiki/Low-pass_filter
        
        
        // фильтры более высокого порядка
        
        // Фильтруется сигнал по следующей формуле:
        /// y[n] = (b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2])/a0
        // Где
        // y[n] – выходной (отфильтрованный) сигнал;
        // x() – входной сигнал, который нужно отфильтровать;
        // b0, b1, b2 и a0, a1, a2 – коэффициенты (константы), которые рассчитываются для конкретного фильтра (LPF, HPF…).
        // Пояснение. x[n] – текущая выборка, x[n-1] – предыдущая выборка.
        
        /*
         param f0 - decision frequency. Value between 0.0 and sampling frequency divided by 2.
         param sr - sampling rate (frequency).
         param Q - quality factor (f0/bandwidth).
         LPF, HPF, BPF, BSF, NOTCH - use f0, sr, Q;
         
         TA, omega, sine, cosine, alpha, beta;
         m_ft = ft;
         
         A = pow(10, dBFilterGain /40);
         ////= sqrt(pow(10, dBFilterGain /20));
         omega = (T)(2 * M_PI * f0 /sr);     // #define M_PI 3.14159265358979323846
         sine = sin(omega);
         cosine = cos(omega); //
         alpha = sine/(2*Q);  //
         beta = sqrt(A + A);
         ////beta = sqrt(A)/Q;
         
         m_b0 = (1 - cosine)/2;
         m_b1 = 1 - cosine;
         m_b2 = (1 - cosine)/2;
         m_a0 = 1 + alpha;
         m_a1 = -2*cosine;
         m_a2 = 1 - alpha;
         */
        // this optimization is usefull for some king of compilers/processors
        ///  out = m_b0*(in + 2*m_x1 + m_x2) - m_a1*m_y1 - m_a2*m_y2; // LPF
        ///
        /// m_x2 = m_x1;    m_x1 = in;  // регистр
        ///
        /// m_y2 = m_y1;    m_y1 = out;
        
        
        // http://howtodoit.com.ua/tsifrovoy-bih-filtr-iir-filter/#codesyntax_5
        
        // —————————————————
        
        //P=(1-t)*P_0 + t*P_1
        
        
        // первая производная  кубической кривой Безье
        // mathbf{B}'(t) = 3(1-t)^2(\mathbf{P}_1 - \mathbf{P}_0) + 6(1-t)t(\mathbf{P}_2 - \mathbf{P}_1) + 3t^2(\mathbf{P}_3 - \mathbf{P}_2) \,.
        // https://en.wikipedia.org/wiki/Bézier_curve#Quadratic_B.C3.A9zier_curves
        
        //берем некий шаг Delta_t, вычисляем касательную для каждой t от нуля до единицы с шагом Delta_t, суммируем длину этих касательных, умноженную на Delta_t, получаем общую длину кривой Безье, точность которой зависит от Delta_t
        //Если я ничего не напутал, то формула для вычисления длины выглядит вот так:
        
        //  =sum{t=0 до 1}|f'(t)|*Delta_t
        
        //http://blog2k.ru/archives/3149
        
        /**/
        
        // —————————————————
        
        
        
        // формула для определения косинуса угла между векторами в координатной форме.
        // cosα=(x1*x2+y1*y2) / (sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2))
        
        // пересечение прямых вычисление наименьшего угла
        // между двумя прямыми два угла: a и 180°–a,        -tg(a)=tg(180°–a)
        // если alpha<=90; то alpha=alpha;
        // если alpha>90; то alpha=180-alpha;       cos(alpha)=-cos(180-alpha)
        // http://edu.dvgups.ru/METDOC/ENF/PRMATEM/V_MATEM/METOD/SULYANDZ2/1_6.htm
        
        // Условие равенства кривых Безье. //
        
        // —————————————————
        
        // Так одна прямая имеет отрицательный угловой коэффициент, а вторая положительный, то угол между этими прямыми будет равен сумме абсолютных величин этих углов. В случае же, когда угловые коэффициенты оба отрицательны или оба положительны, то угол находится путем вычитания из большего угла меньшего. В рассматриваемом примере получим, что угол между прямыми равен |34| + |-31| = 34 + 31 = 65 градусов.
        // http://www.kakprosto.ru/kak-126210-kak-opredelit-ugol-mezhdu-dvumya-pryamymi
        /*
        int alpha1=180;     int alpha2=10;       int d_alpha;       int CorAlpha;
        CorAlpha=fmax(alpha1, alpha2);
        ///int xx=alpha1^alpha2;
        ///if(GrafT)cout<<"alpha1*alpha2="<<alpha1*alpha2<<"     alpha1^alpha2="<<xx<<"     (alpha1^alpha2)="<<(alpha1^alpha2)<<endl;
        */
        /*
        // Универсальная машинка для сравнения углов всех типов: (-90° до +90° grade), (-180° до +180° grade) и (0° до +360° grade)
        // для целочисленных типов int  для контрольных векторов
        if ((alpha1^alpha2)<0) d_alpha=fabs(alpha1) + fabs(alpha2);      else d_alpha=fabs(alpha1-alpha2);
         // абсолютное значение разности углов двух контрольных векторов не м.б. больше 180°.
        if (d_alpha>180) d_alpha=360-d_alpha; // для входных углов (-180° до +180° grade) и (0° до +360° grade)
        */
        /*
        // Универсальная для float для контрольных векторов
        if ((alpha1*alpha2)<0) d_alpha=fabs(alpha1)+fabs(alpha2);     else d_alpha=fabs(alpha1-alpha2);
        if (d_alpha>180) d_alpha=360-d_alpha; // абсолютное значение разности углов двух контрольных векторов не м.б. больше 180°.
        */
        /*
        // Только для сравнения положительных углов контрольных векторов: (от 0° до +360° grade)
        d_alpha=fabs(alpha1-alpha2);
         if (d_alpha>180) d_alpha=360-d_alpha;// абсолютное значение разности углов двух контрольных векторов не м.б. больше 180°.
        */
        
        /*
        // Только для сравнения положительных углов контрольных векторов: (от 0° до +360° grade)
        d_alpha=fabs(alpha1-alpha2);
        // абсолютное значение разности углов двух линий не м.б. больше 180°grade.
        if (d_alpha>180) d_alpha=360-d_alpha;
        // абсолютное значение разности углов двух линий не м.б. больше 90° grade.
        if (d_alpha>90) { d_alpha=-180+d_alpha;    CorAlpha=-100-100*d_alpha/90;} // сдесь d_alpha<0
        else CorAlpha=100-100*d_alpha/90;  // Коэффициент корреляции двух векторов по углам (0-100%).
        */
        // обратная операция if (alpha<0) alpha+=360; выполняемая в функции LineAngleLength
 //       if(GrafT)cout<<"alpha1="<<alpha1<<"     alpha2="<<alpha2<<"     d_alpha="<<d_alpha<<"     CorAlpha="<<CorAlpha<<endl;
        
        
        
        
        
        // Алгоритм Осло. Повысить гибкость В-сплайна можно другим способом: вставить дополнительные узловые значения в определяющий узловой вектор. стр 355.
        
        // кодирование контуров Безье. На HDD "КОДИРОВАНИЕ КОНТУРОВ, ПРЕДСТАВЛЕННЫХ КРИВЫМИ БЕЗЬЕ, В ЗАДАЧАХ НЕЙРОСЕТЕВОЙ".
        
        // —————————————————
        
        // Источник:
        // http://rsdn.ru/article/multimedia/Bezier.xml#
        // Автор: Maxim Shemanarev.
        // Разобраться, ниже код компактнее:
        // Первоисточник:   http://www.devmag.org.za/downloads/bezier_curves/BezierPath.cs
        // Перевод первоисточника:   http://3d-orange.com.ua/bezier-curves-for-your-games-tutorial/
        // Хорошее объяснение метода в тексте: "Интерполяция сплайнами", на просторах интернета выглядет как "08 Сплайны.pdf"
        // Или что тоже самое:
        // http://www.intuit.ru/studies/courses/993/163/lecture/4497
        
        // —————————————————
        
        //Эта функция не определена стандартом ANSI С. Функция poly() вычисляет полином переменной x степени n с коэффициентами от c[0] до с[n] и возвращает значение полинома. Например, если n=3, то функция возвращает значение:
        //с[3]х3+ с[2]х2+с[1]х+ с[0]
        // http://www.c-cpp.ru/content/poly-polyl
        
        // Этот макрос не определен стандартом ANSI С.
        // Макрос cabs() возвращает абсолютную величину комплексного числа. Структура complex опре­делена следующим образом:
        // http://www.c-cpp.ru/content/cabs-cabsl
        
        // Функция hypot() возвращает длину гипотенузы прямоугольного треугольника по заданным длинам двух сторон.
        // Функция hypotl() представляет собой версию этой функции для данных типа long double.
        // hypot(2, 1);
        // http://www.c-cpp.ru/content/hypot-hypotl
        
        // Функция pow() возвращает значение base, возведенное в степень ехр, т. е. baseexp. pow (x, y)
        // Функция pow10() возвращает 10 в степени n.
        // http://www.c-cpp.ru/content/pow-powl
        
        // Функция fmax fmin()  Каждая функция семейства fmax() возвращает больший из аргументов а и b.
        // Каждая функция семейства fma() возвращает значение выражения а*b+с. Округление выполняется только один раз, после завершения всей операции.
        // http://c2p.ru/c/func/math?skip=35
        
        // —————————————————
        
        
        
        /*
        // обычный алгоритм выравнивания, его стоит запихать в конструктор сего класса, и забыть о нём
        class T4Matrix {
        private:
            float d[20];
            float *m;
            ...
        }
        
        void __fastcall T4Matrix::_init_mem(){
            this->m = (float *) (((unsigned long) & this->d[0] + 0x10) & ~0xf);
        }
        */
        
        
         // —————————————————
        
        // Intel Intrinsics Guide
        // https://software.intel.com/sites/landingpage/IntrinsicsGuide/
        // http://www.plantation-productions.com/Webster/www.artofasm.com/Windows/HTML/TheMMXInstructionSeta2.html
        
        // Computes the length of the hypotenuse of a right angled triangle with single precision.
        // Вычислить длину гипотенузы прямоугольного треугольника с одинарной точностью.
        //встроенные функции,
        float a=hypotf(1.0, 1.0);
///        if(GrafT)cout<<"+++++a="<<a<<endl;
        // https://software.intel.com/sites/default/files/a6/22/18072-347603.pdf
        
        
        
        // переход в режим Assembler, загрузка констант
        __asm {
        } // __asm
        // #include <immintrin.h>  // заголовочный файл immintrin.h с описанием соответствующих intrinsic-функций. // smmintrin.h // xmmintrin.h
        // Intrinsics — набор функций и типов данных, поддерживаемых компилятором, для предоставления высокоуровневого доступа к SSE инструкциям
        // Регистровые суммы маски On  (подсчет единиц) по  X
                    int SumBit=_mm_popcnt_u64(255);
        
                        //SumBit=_mm_rsqrt_ps(256); // _mm_crc32_u8
SumBit=_mm_popcnt_u32(256);
                    //double res;
                    float dot;
                    __m128 na,nb,res;
                    res=_mm_dp_ps(na,nb,0x7f);
                    _mm_store_ss(&dot,res);
                    //double v=DPPS xmm1, xmm2/m128, imm8
        ///if(GrafT)cout<<"SumBit="<<SumBit<<"     res="<<res<<endl;
        
        // Выравнивание данных является еще одним аспектом, за которым нужно следить при перемещении данных. Используйте атрибут GCC "aligned" для выравнивания исходных и целевых участков с данными в 16-битных границах для улучшения производительности. Например:
        float anarray[4] __attribute__((aligned(16))) = { 1.2, 3.5, 1.7, 2.8 };
        // http://rus-linux.net/MyLDP/algol/gcc-compiler-intrinsics-vector-processing-2.html

        
        //Для наибольшей эффективности элементы таких типов данных должны быть выровнены в памяти по соответствующей границе. Например, начало массива элементов типа __m64 выравнивается по 8 байтам, а массив элементов __m128 – по 16 байтам. Статические переменные и массивы компилятор выравнивает автоматически. Динамические данные компилятор обычно выравнивает по только величине 4 байта. Если данные векторных типов оказались невыровненными, то для работы с ними следует применять специальные команды невыровненного чтения и записи (они работают медленнее обычных – выровненных). Для выделения памяти с выравниванием используется функция: _mm_malloc

        float *Buf; // массив для обработки с помощью инструкций SSE
        //void *_mm_malloc(32, 16);
         Buf = (float*)_mm_malloc(32, 16); // (N*sizeof(float),16)
         _mm_free(Buf);
        // переписать на _mm_malloc   GBitsetMatrix_base.cpp
        // srcInt=(unsigned int*)malloc((data_size+64)*4);  // переупакованный в int массив пикселей
        // sInt=(unsigned int*)malloc((data_size+64)*4);    // транспонированый массив
        
        /*
        float *x;	// массив для обработки с помощью инструкций SSE
        x=(float)_mm_malloc(N*sizeof(float),16);
        // … здесь обработка …
        _mm_free(x);
        */
        /**/
         // —————————————————
        
        float data1[64];
        data1[1*4]=0; data1[2*4]=0;   data1[3*4]=0;
        data1[0]=16.0;
        //data1[0]=0;
        __m128 d0 = _mm_load_ps(&data1[0]); // i*4
        __m128 r0 = _mm_sqrt_ps(d0);
        //__m128 r0 = _mm_sqrt_ss(d0);
        _mm_store_ps(&data1[0],r0);        // i*4
///        if(GrafT)cout<<"+++++data1[0]="<<data1[0]<<endl;
        
        
        float   mat[3][4] = {{ 0.17f,0.22f,0.33f,0.44f},{0.55f,0.66f,0.77f,0.88f},{1.01f,1.02f,1.03f,1.04f}};
        float data[64];
        int sz=2;
        __m128 m0 = _mm_load_ps(mat[0]);
        __m128 m1 = _mm_load_ps(mat[1]);
        __m128 m2 = _mm_load_ps(mat[2]);
        
        
        for(int i=0;i<sz;i++)
        {
            __m128 d0 = _mm_load_ps(&data[i*4]);
            __m128 r0 = _mm_dp_ps(d0,m0,0xf1);
            __m128 t0 = _mm_dp_ps(d0,m1,0xff);
            __m128 t1 = _mm_dp_ps(d0,m2,0xff);
            r0 = _mm_blend_ps(r0,t0,2);
            r0 = _mm_blend_ps(r0,t1,4);
            _mm_store_ps(&data[i*4],r0);
        }
        /**/
        /**/
        // http://blog.lexa.ru/2011/09/01/o_kompilyatorakh_i_protsessorakh.html
        
        /*
        Скалярное умножение векторов[править | править вики-текст]
        DPPS xmm1, xmm2/m128, imm8 — (Dot Product of Packed Single Precision Floating-Point Values)
        DPPD xmm1, xmm2/m128, imm8 — (Dot Product of Packed Double Precision Floating-Point Values)
        Скалярное умножение векторов (dot product) 32-/64-битных полей. Посредством битовой маски в imm8 указывается, какие произведения полей должны суммироваться и что следует прописать в каждое поле результата: сумму указанных произведений или +0.0.
        */
        
        /* //???????????
        //  x86 Built-in Functions
        if (__builtin_cpu_supports ("popcnt"))
        {
            asm("popcnt %1,%0" : "=r"(count) : "rm"(n) : "cc");
        }
        else
        {
            count = generic_countbits (n); //generic implementation.
        }
        // https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html#x86-Built-in-Functions
        */
        
         // —————————————————
        /*
        int m=16; //длина регистра (хеш)
        float summ_P1_L_P2=(line1_lenP1 + line1_len + line1_lenP2); // сумма длин трех векторов: двух управляющих P1 и P2 и опорного вектора L,
        if(GrafT)cout<<"line1_lenP1="<<line1_lenP1<<"     line1_len="<<line1_len<<"     line1_lenP2="<<line1_lenP2<<endl;
        
        float ks=m/summ_P1_L_P2;
        if(GrafT)cout<<"m="<<m<<"     summ_P1_L_P2="<<summ_P1_L_P2<<"     ks="<<ks<<endl;
        int kP1=ks*line1_lenP1;     int kL=ks*line1_len;     int kP2=ks*line1_lenP2;
        if(GrafT)cout<<"kP1="<<kP1<<"     kL="<<kL<<"     kP2="<<kP2<<endl;
        
        kL=m-kP1-kP2;
        
        float sum=kP1 + kL + kP2;
        if(GrafT)cout<<"sum="<<sum<<"     kL="<<kL<<"     ks="<<ks<<endl;
        */
        // —————————————————
        
        float delta_x=-4, delta_y=-1;
        float alpha=57.29747*atan2f(delta_y,delta_x);    // угол в градусах в I,II и III,IIII квадрантах (-180° до +180° гр).
        // доопределение углов до значения от 0° до 360° градусов. Углы только положительные.
        if (alpha<0) alpha+=360;
///        cout<<"alpha="<<alpha<<endl;

        if (delta_x<0) alpha=180-alpha;
        if (alpha<0) alpha+=360;
        // alpha=180-alpha;    if (alpha<0) alpha+=360; // зеркало, где alpha приведенный угол
        
////        cout<<"alpha="<<alpha<<"     delta_x="<<delta_x<<"     delta_y="<<delta_y<<endl;
        // y>0     x=-x;     alpha=180-alpha;
        // y<0     x=-x;     alpha=180-alpha;
        
        ///deltaAlpha=fabs(line1_alpha-line2_alpha);
        // Находим наименьший угол (<90°) между опорными линиями кривых Безье N1 и N2.
        ///if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;
        ///if (deltaAlpha>90) deltaAlpha=180-deltaAlpha;
        
        //Линейная интерполяция между углами
        float a1, a2, t;
        if (a1 - a2 > 180) a1-= 360;
        if (a1 - a2 <-180) a1+= 360;
        float aa=a1 + t * (a2 - a1); //return
        // Если вам необходимо вычислить расстояние в градусах между углами, то можно переписать код так:
        if (a1 - a2 > 180) a1 -= 360;
        if (a1 - a2 <-180) a1 += 360;
        aa=abs(a2 - a1); // return
        //http://blog2k.ru/archives/3325
        
        // при зеркальных преобразованиях их соответствующая координата меняет свое значение на противоположное.
        //Масштабирование Поворот
        //
        //С геометрической точки зрения, производной кривой Безье будет другая кривая Безье, векторы управляющих точек которой определяются вычислением разностей векторов управляющих точек исходной кривой.
        
         //В работах Барского [5-16, 5-18] было показано, что любая кривая Безье может быть разбита с использованием произвольного параметра в диапазоне . Простейший случай - это средняя точка, т.е.  (см. [5-18]).
         //  5-16 Bartels, R. H., Beatty, J. C., and Barsky, B. A., Splines for Use in Computer Graphics & Geometric Modeling, Morgan Kaufman, Los Altos, CA, 1987.
         //   5-17 Barsky, B. A., «Arbitrary Subdivision of Bezier Curves», TR UCB/CSD 85/265, Соmp. Asi. Div., Univ. of Cal., Berkeley, November 1985.
         //   5-18 Clark, J. H., «А Fast Scanline Algorithm for Rendering Parametric Surfaces», Соmp. Graph., Vol. 14, pp. 7-12, 1979 [addendum to SIGGRAPH 79 Conf. Proc. - «papers to be published in the СACM»].
         // http://sernam.ru/book_mm3d.php?id=92

        // Брезенхэм предложил подход, позволяющий разрабатывать так называемые инкрементные алгоритмы растеризации.
        // http://pandia.ru/text/77/289/154-4.php
        
        
        // отлично разобрана рекурсия безье
        // _chirikov.pdf
        // —————————————————
        /*
        // Массивы для хранения таблиц
        float cos_look[360];
        float sin_look[360];
        
        float angDlt=3.14159/180;
        float theta=0;
        // Создаем таблицу
        for(int ang=0; ang < 360; ang++) {
            // Преобразуем значение угла в радианы
            theta+=angDlt;
            // Помещаем в таблицу очередной элемент
            cos_look[ang]=cos(theta);
            sin_look[ang]=sin(theta);
        } // for
        

        
        // Представление кривых второго порядка кривыми Безье. Квадратичная кривая
        // http://stu.sernam.ru/book_g_mod.php?id=25
        
        // Характеристика методов бинаризации
        // http://it-claim.ru/Library/Books/ITS/wwwbook/ist4b/its4/fyodorov.htm
        
        // Линейная интерполяция и кривая Безье
        // http://blog2k.ru/archives/3149
        
        // Кусочно-квадратичная интерполяция
        // http://aco.ifmo.ru/el_books/numerical_methods/lectures/glava3.html
        
        //BookCAT
        //Мощная программа-каталогизатор книг. Позволяет всегда за минимально короткое время выяснить, где находится та или иная книга. Поиск, составление отчетов и многие другие функции.
        

        
        /*
        //Допустим из двух значений нам нужно выбрать минимальное. В Си это можно сделать одним из следующих способов:
        x < y? x: y
        x <= y? x: y
        x > y? y: x
        x >= y? y: x
        //Часто компилятор считает их эквивалентными и всегда использует первый вариант, так как он выполняется за одну инструкцию процессора.
        */
        
        float A=-4.12; // 23.97
///        cout<<"A="<<A<<endl;
        int aInt=(((uint&)A) & 0x7FFFFFFF); // 0x80000000 // 0x7FFFFFFF
///        cout<<"aInt="<<aInt<<endl;
        //int aInt = *(int*)&A;
        //aInt=0x7FFFFFFF & aInt;
///        cout<<"aInt="<<aInt<<endl;
        //int aInt = (int)A;
        //int aInt = -1;
///        cout<<"A="<<A<<"     aInt="<<aInt<<endl;
        // Уберем знак в aInt, если есть
        if (aInt < 0) aInt=0x80000000 - aInt;
        // http://habrahabr.ru/post/112953/
////        cout<<"A="<<A<<"     aInt="<<aInt<<endl<<endl;
        /**/
        // —————————————————
        
        //Простыми словами о представлении форматов float32 и double64 в памяти компьютера
        //http://blog2k.ru/archives/3321
        
        /*
        Direct conversions
        I originally learned this one (and this way of thinking about the FPU) from Dan Wallach when we were both at Microsoft in the summer of 1995. He showed me how to implement a fast lookup table by direct manipulation of the mantissa of a floating point number in a certain range.
        First, you have to know a single precision float has three components:
        
        sign[1 bit]  exp[8 bits]  mantissa[23 bits]
        The value of the number is computed as:
        (-1)^sign * 1.mantissa * 2^(exp - 127)
        So what does that mean? Well, it has a nice effect:
        
        Between any power of two and the next (e.g., [2, 4) ), the exponent is constant. For certain cases, you can just mask out the mantissa, and use that as a fixed-point number. Let's say you know your input is between 0 and 1, not including 1. You can do this:
        */
        /**/
                                                      
        ///int FloatTo23Bits(float x)
        ///{
        float x=24.44999;
            float y = x + 1.f;
///        cout<<"x="<<x<<"     y="<<y<<endl;
            ///return ((unsigned long&)y) & 0x7FFFFF;	// last 23 bits
        uint yy=(((uint&)y) & 0x7FFFFF);	// last 23 bits
        //uint yy=(uint)y;	// last 23 bits
        ///}
///        cout<<"x="<<x<<"     y="<<y<<"     yy="<<yy<<endl;
        
        /*
        inline int fastftoi(float f)
        { return _mm_cvtss_si32(_mm_load_ss(&f)); }
        */
        
        _mm_cvtss_si32(_mm_load_ss(&x)); // http://forum.ixbt.com/topic.cgi?id=26:40327 // http://ccfit.nsu.ru/~kireev/lab4/lab4intr.htm
        //  смотрел на _mm_cvttss_si32 , который округляет вниз
        // В общем вывод: конструкция _mm_cvt_ss2si( _mm_load_ss(&x) ) переносима и работает быстрее всего.
        // Для округления вниз можно использовать обычный int(x) , т.к. выигрыш от _mm_cvtt_ss2si незначителен
////        cout<<"x="<<x<<"     y="<<y<<"     yy="<<yy<<endl;
        
        //The first line makes 'y' lie between 1 and 2, where the exponent is a constant 127. Reading the last 23 bits gives an exact 23 bit representation of the number between 0 and 1. Fast and easy.
        // http://stereopsis.com/FPU.html
        
        //Простыми словами о представлении форматов float32 и double64 в памяти компьютера
        // http://blog2k.ru/archives/3321
        
        // Практическое применение B-сплайна (B-Spline) в программировании
        // просто, понятно и реально, можно применять.
        // http://blog2k.ru/archives/3419
        // «Онлайн редактор уравнений»
        // http://www.codecogs.com/latex/eqneditor.php
        
        // —————————————————
        
        /**/
        
        //Для нахождения модуля полученного отрицательного числа нужно инвертировать биты и добавить единицу. Инвертируем:
        //10010110 - > 01101001
        //добавляем единицу:
        //01101001 + 0000001 = 01101010
        int alpha_1=-24;
////        cout<<"alpha_1="<<alpha_1<<endl;
        //alpha_1=(~alpha_1); //  & 0x7FFFFFFF // 0x80000000
        //alpha_1=(~alpha_1 & 0x7FFFFFFF)+1;
////        cout<<"alpha_1="<<alpha_1<<endl;
        
        //int a;
        //int sign=(a>0)-(a<0);   // Результат: 1 — положительное, 0 — нуль, -1 — отрицательное
        
        
        
        // Если заданы две прямые y=k1*x + b1; , y=k2*x + b2;, то острый угол между этими прямыми будет определяться как:
        // k1 и k2 подсчитаны
        
        //  tg(d_alpha)=fabs((k2-k1)/(1+k2*k1));
        
        //  k1=tg(alpha1);       k2=tg(alpha2);     d_alpha=alpha2-alpha1
        // Две прямые параллельны, если k1=k2. Две прямые перпендикулярны, если k1=-1/k2.
        // Прямые Ах + Ву + С=0 и А1х + В1у + С1=0 параллельны, когда пропорциональны коэффициенты А1=λА, В1=λВ.
        // Если еще и С1=λС, то прямые совпадают.
        // Для нахождения острого угла между прямыми выражения в правой части этих формул следует брать по модулю.
        //http://stu.sernam.ru/book_ehm.php?id=26
        //Подробнее: http://www.kakprosto.ru/kak-126210-kak-opredelit-ugol-mezhdu-dvumya-pryamymi#ixzz3mfoLdh86
        
        
        
        // https://valker.wordpress.com/2006/04/26/sign_function-2/
        // Относительная длина линий кривых Безье N1 и N3.
        
        //--------------------------------------------------------------------------------------------------------------------------

        // Две кривые Безье считаем равными, если совпадают начальные и конечные точки, а также их управляющие точки.
        // Визуализация кривых легко выполняется с помощью управляющих точек.
        // Кривые Безье удобно использовать для сравнения и визуализации.
        
        /*
        // Коэффициент корреляции двух линий по углам где: CorAlpha (0-100%),  Alpha1, Alpha2 (-90° до +90° grade).
        float deltaAlpha=fabs(line1_alpha-line2_alpha); // абсолютное значение разности углов двух линий. (0 до 360° grade) -=180
        if (deltaAlpha>90) deltaAlpha=180-deltaAlpha;  // устранение зависимости разности углов от "направления" линий.
        int CorAlpha=100-100*deltaAlpha/90;  // Коэффициент корреляции двух линий по углам (0-100%).
        if(GrafT)cout<<"Коэффициент корреляции углов линией проведенными между начальными и конечными точками кривых Безье CorAlpha="<<CorAlpha<<endl;
        */
        // Относительная длина линий (%).
        // int relLength1=100*LengthPoint1/LengthPoint0;
        // int relLength2=100*LengthPoint2/LengthPoint0;
        // Коэффициент корреляции двух линий по относительноцй длине (0-100%).
        // int relLength_=(relLength1+relLength2)/2;
        // int CorRelLength1=100*(relLength_-fabs(relLength1-relLength2))/relLength_;
        /// int CorRelLength1=100*(1-fabs2*(relLength1-relLength2)/(relLength1+relLength2)); /// можно подставлять абсолютные длины
        /**/
        
        /// Сравнение двух кривых Безье. ///
        /// Этого достаточно для полного совпадения двух кривых Безье.

        
        // gPort.mm  570стр
        // таблица в гр порту:
        // 1. id number
        // 1. len
        // 1. alpha°
        // 1. lenP1
        // 1. alphaP1°
        // 1. lenP2
        // 1. alphaP2°
        // 1. lenA
        // 1. lenM
        // 1. lenBf
        // 1. lenBff


#endif  //ASM_OS64_SSE42_POPCNT
        
    }//____________________________________________________________________________
    
    
    //*****************************************************************************
 
    
};  // namespace ocr{

/*
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине опорныых линий (0-100%).
float M1LenL=line2_len/line1_len;  //(line1_len+line2_len))
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P1
float M1LenP1=line1_lenP1/line1_len;
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P2
float M1LenP2=line1_lenP2/line1_len;
if(GrafT)cout<<"M1LenL="<<M1LenL<<"     M1LenP1="<<M1LenP1<<"     M1LenP2="<<M1LenP2<<endl;

// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P1
float M2LenP1=line2_lenP1/line2_len;
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P2
float M2LenP2=line2_lenP2/line2_len;
if(GrafT)cout<<"M1LenL="<<M1LenL<<"     M2LenP1="<<M2LenP1<<"     M2LenP2="<<M2LenP2<<endl;
*/

/*
/// Проверка совпадения относительных длин линий сравниваемых кривых Безье N1 и N3. ///
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине опорныых линий (0-100%).
float CorLenL=100*(1-2*fabs(line1_len-line2_len)/(line1_len+line2_len));  //(line1_len+line2_len))
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P1
float CorLenP1=100*(1-2*fabs(line1_lenP1-line2_lenP1)/(line1_lenP1+line2_lenP1));
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P2
float CorLenP2=100*(1-2*fabs(line1_lenP2-line2_lenP2)/(line1_lenP2+line2_lenP2));
if(GrafT)cout<<"CorLenL="<<CorLenL<<"     CorLenP1="<<CorLenP1<<"     CorLenP2="<<CorLenP2<<endl;
*/

//int L1x1=line1.start.x;  int L1y1=line1.start.y;
//int L1x2=line1.end.x;    int L1y2=line1.end.y;
//int L2x1=line2.start.x;  int L2y1=line2.start.y;
//int L2x2=line2.end.x;    int L2y2=line2.end.y;
//if(GrafT)cout<<"L1x1="<<L1x1<<" L1y1="<<L1y1<<" L1x2="<<L1x2<<" L1y2="<<L1y2<<endl;
//if(GrafT)cout<<"L2x1="<<L2x1<<" L2y1="<<L2y1<<" L2x2="<<L2x2<<" L2y2="<<L2y2<<endl;

// float wgP1, wgL, wgP2;    // weight
///float sum_P1_L_P2=CorLenP1 + CorLenL + CorLenP2;
///float wgP1=CorLenP1/sum_P1_L_P2;   float wgL=CorLenL/sum_P1_L_P2;   float wgP2=CorLenP2/sum_P1_L_P2;

/*
//допуски
float dlt_alpha=4.0;     // от 0 до 360° grade
///float dlt_len=5.0;       // pix

// сравниваем углы в допусках: если попадаем в допуск, то продолжаем если НЕТ, то return;
// сравние углов между линиями, проведенных от начальных до конечных точек кривых Безье N1 и N2.
if ( line1_alpha   > line2_alpha + dlt_alpha || line1_alpha   < line2_alpha - dlt_alpha) return;
// сравние углов первых контрольных векторов кривых Безье N1 и N2.
if ( line1_alphaP1 > line2_alphaP1+dlt_alpha || line1_alphaP1 < line2_alphaP1-dlt_alpha) return;
*/



/// Проверка совпадения уголов сравниваемых кривых Безье N1 и N2. ///
/*
// Коэффициент корреляции уголов линий между опорными линиями кривых Безье N1 и N2.
// Опорная линиям условно проведена между начальными 0 и конечными точками 3 кривой Безье.
deltaAlpha=fabs(line1_alpha-line2_alpha);       // абсолютное значение разности углов двух линий.
if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;  // абсолютное значение разности углов двух линий не м.б. больше 180°.
if (deltaAlpha>90) deltaAlpha=180-deltaAlpha;   // абсолютное значение разности углов двух линий не м.б. больше 90° grade.
CorAlphaL=100-100*deltaAlpha/90;  // Коэффициент корреляции двух линий по углам (0-100%).
if(GrafT)cout<<"line1_alpha="<<line1_alpha<<"     line2_alpha="<<line2_alpha<<endl;
if(GrafT)cout<<"+Коэффициент корреляции углов между опорными линиями кривых Безье N1 и N2   CorAlphaL="<<CorAlphaL<<"%"<<endl;




// Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.
deltaAlpha=fabs(line1_alphaP1-line2_alphaP1); // абсолютное значение разности углов двух контрольных векторов.
// абсолютное значение разности углов контрольных векторов не м.б. больше 180°.
if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;
///if (deltaAlpha>90) deltaAlpha=180-deltaAlpha; // абсолютное значение разности углов контрольных векторов не м.б. больше 90° grade.

// абсолютное значение разности углов контрольных векторов не м.б. больше 90°
if (deltaAlpha>90) { deltaAlpha=180-deltaAlpha;    CorAlphaP1=-100+100*deltaAlpha/90; }
else CorAlphaP1=100-100*deltaAlpha/90;  // Коэффициент корреляции двух векторов по углам (0-100%).

///CorAlphaP1=100-100*deltaAlpha/90;  // Коэффициент корреляции двух векторов по углам (0-100%). /////// int
if(GrafT)cout<<"line1_alphaP1="<<line1_alphaP1<<"     line2_alphaP1="<<line2_alphaP1<<endl;
if(GrafT)cout<<"+Коэффициент корреляции углов между контрольными векторами P1 кривых Безье N1 и N2.   CorAlphaP1="<<CorAlphaP1<<"%"<<endl;



// Коэффициент корреляции углов между контрольными векторами P2 кривых Безье N1 и N2.
deltaAlpha=fabs(line1_alphaP2-line2_alphaP2); // абсолютное значение разности углов двух контрольных векторов.
if (deltaAlpha>180) deltaAlpha=360-deltaAlpha;// абсолютное значение разности углов контрольных векторов не м.б. больше 180°.
///if (deltaAlpha>90) deltaAlpha=180-deltaAlpha; // абсолютное значение разности углов контрольных векторов не м.б. больше 90° grade.

// абсолютное значение разности углов контрольных векторов не м.б. больше 90°
if (deltaAlpha>90) { deltaAlpha=180-deltaAlpha;    CorAlphaP2=-100+100*deltaAlpha/90; }
else CorAlphaP2=100-100*deltaAlpha/90;  // Коэффициент корреляции двух векторов по углам (0-100%).

///CorAlphaP2=100-100*deltaAlpha/90;  // Коэффициент корреляции двух векторов по углам (0-100%).
if(GrafT)cout<<"line1_alphaP2="<<line1_alphaP2<<"     line2_alphaP2="<<line2_alphaP2<<endl;
if(GrafT)cout<<"+Коэффициент корреляции углов между контрольными векторами P2 кривых Безье N1 и N2.   CorAlphaP2="<<CorAlphaP2<<"%"<<endl;



/// Проверка совпадения относительных длин линий сравниваемых кривых Безье N1 и N3. ///

// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине опорныых линий (0-100%).
float CorLenL=100*(1-2*fabs(line1_len-line2_len)/(line1_len+line2_len));  //(line1_len+line2_len))
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P1
float CorLenP1=100*(1-2*fabs(line1_lenP1-line2_lenP1)/(line1_lenP1+line2_lenP1));
// Коэффициент корреляции кривых Безье N1 и N2 по относительной длине контрольных векторов P2
float CorLenP2=100*(1-2*fabs(line1_lenP2-line2_lenP2)/(line1_lenP2+line2_lenP2));
if(GrafT)cout<<"CorLenL="<<CorLenL<<"     CorLenP1="<<CorLenP1<<"     CorLenP2="<<CorLenP2<<endl;


//// дальнейше должно отрабатываться не здесь ////

float delta=60;   float dlt=100-delta;
if (CorAlphaL < dlt) CorAlphaL=0;   if (CorAlphaP1 < dlt) CorAlphaP1=0;   if (CorAlphaP2 < dlt) CorAlphaP2=0;
if (CorLenL < dlt) CorLenL=0;       if (CorLenP1 < dlt) CorLenP1=0;       if (CorLenP2 < dlt) CorLenP2=0;
if(GrafT)cout<<"000 CorAlphaL="<<CorAlphaL<<"     CorAlphaP1="<<CorAlphaP1<<"     CorAlphaP2="<<CorAlphaP2<<endl;
if(GrafT)cout<<"000 CorLenL="<<CorLenL<<"     CorLenP1="<<CorLenP1<<"     CorLenP2="<<CorLenP2<<endl;


// перейти на относительные углы точек P1 и P2
// wgline1P1*wgline2P1/norm,     wgL,    wgline1P2*wgline2P2/norm
/// if (alpha<0) alpha+=360;

// loat wgP1, wgL, wgP2;    // weight
float sum_P1_L_P2=CorLenP1 + CorLenL + CorLenP2;
float wgP1=CorLenP1/sum_P1_L_P2;   float wgL=CorLenL/sum_P1_L_P2;   float wgP2=CorLenP2/sum_P1_L_P2;
if(GrafT)cout<<"wgP1="<<wgP1<<"     wgL="<<wgL<<"     wgP2="<<wgP2<<endl;
//  Суммарный коэффициент корреляции кривых Безье N1 и N3 по разностям углов линий.
//float CorAlphaSUM=(CorAlphaP1 + CorAlphaL + CorAlphaP2)/3;
float CorAlphaSUM=wgP1*CorAlphaP1 + wgL*CorAlphaL + wgP2*CorAlphaP2;
if(GrafT)cout<<"CorAlphaL="<<CorAlphaL<<"     CorAlphaP1="<<CorAlphaP1<<"     CorAlphaP2="<<CorAlphaP2<<endl;
if(GrafT)cout<<"+++Суммарный коэффициент корреляции углов кривых Безье N1 и N2.   CorAlphaSUM="<<CorAlphaSUM<<"%"<<endl<<endl;

//  Суммарный коэффициент корреляции кривых Безье N1 и N2 по относительным длинам линий.
float CorLenSUM=(CorLenP1 + CorLenL + CorLenP2)/3;
if(GrafT)cout<<"---Суммарный коэффициент корреляции по относительным длинам линий кривых Безье N1 и N2.   CorLenSUM="<<CorLenSUM<<"%"<<endl;

//  Полный коэффициент корреляции кривых Безье N1 и N2
int CorBezierSUM=(CorAlphaSUM + CorLenSUM)/2;
//CorBezierSUM=sqrt(CorAlphaSUM*CorLenSUM);
if(GrafT)cout<<"***Полный коэффициент корреляции кривых Безье N1 и N2    CorBezierSUM="<<CorBezierSUM<<"%***"<<endl<<endl;
*/

/**/


