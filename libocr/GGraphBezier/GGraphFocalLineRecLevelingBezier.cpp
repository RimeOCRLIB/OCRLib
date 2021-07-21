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


namespace ocr{
    
    //*****************************************************************************
    
    // FocalLineRecLeveling //
    // Продолжение функции FocalLineRecognition.

    
    //*****************************************************************************
    
    // Функция для нормализации данных фокальной линии.
    
    void GGraphBezier::focalLineNorm( OCRFocalLineBezier &line){
  
        // Функция предназначена для получения массива адресов интегральных сумм фокальной линии по "x" и "y".

        // все массивы переменной длинны в длинну линии нужно перевести на массивы VString
        // или на массивы фиксированного размера 2(w+h), где размер рамки в периметр печа.
        // time=0.044
        
        float x0, y0, x3, y3;   // Декартовы координаты начала и конца сглаженной фокальной линии.
        int LengthLine=(int)line.data.size();   // Длина фокальной линии в pix.
        line.lenA=LengthLine;                   // Сохранение длины фокальной линии в pix.
        // После функции деления ломанной фокальной линии на составляющие линии, ее длина изменилась.

        /*
        // Нормализация по start end // сбивается графика т.к. не пересчитаны остальные точки фокальной линии.
        x0=line.start.x;      y0=line.start.y;
        x3=line.end.x;        y3=line.end.y;
        if(y3<y0)  { line.start.x=x3; line.start.y=y3;  line.end.x=x0; line.end.y=y0; }
        if(y3==y0) { if(x3<x0) { line.start.x=x3; line.start.y=y3;  line.end.x=x0; line.end.y=y0; } }
        // После функции деления ломанной фокальной линии на составляющие линии, start end  перепутаны
        */
        
 
        
        // Подсчет массива интегральных сумм. //
        // Используется в функции focalLineRecMiddle для сглаживания фокальной линии.

        OCRPointFloat s_line;                            // объявление s_line
      // time=0.039
        line.sData.resize(LengthLine);              // resize
      // time=0.053
        OCRPointFloat *data=(OCRPointFloat*)&line.data[0];    // получаем указатель на массив Data
        OCRPointFloat *sData=(OCRPointFloat*)&line.sData[0];  // получаем указатель на массив sData
        
        float centerX=0, centerY=0;     // Среднее по всем координатам "x" и "y"  линии. Центр масс линии.
        ///if (line.index==153)cout<<"     sData[0].y="<<sData[0].y<<"     sData[1].y="<<sData[1].y<<"     sData[2].y="<<sData[2].y<<endl;
        
        // Цикл массива интегральных сумм.
        for (int index=0; index < LengthLine; index++){
            // Вычисление суммы всех координат линии.
            centerX+=data[index].x;
            centerY+=data[index].y;
            // Заполнение массивов интегральных сумм адресов промежуточных точек по  "x" и "y" .
            s_line.x=centerX;
            s_line.y=centerY;
            sData[index]=s_line; // сохранение массива интегральных сумм адресов.
            //if(GrafT)cout<<"++++centerX="<<centerX<<"   centerY="<<centerY<<"   len="<<len<<endl;
            
        } // for (int

        ///if (line.index==153)cout<<"     sData[0].y="<<sData[0].y<<"     sData[1].y="<<sData[1].y<<"     sData[2].y="<<sData[2].y<<endl;
        
        // среднее по всем координатам линии (центр масс линии). //
        line.centerM.x=centerX/(float)LengthLine;
        line.centerM.y=centerY/(float)LengthLine;
     // time=0.055
        
        // Получаем координаты начальных и конечных точек фокальной линии.
        x0=line.start.x;    y0=line.start.y;    // координаты начала фокальной линии.
        x3=line.end.x;      y3=line.end.y;      // координаты конца фокальной линии.
        // Координаты начальных и конечных точек кривой Безье (в последствии, могут не совпадать с точками фокальной линии).
        line.p0.x=x0;       line.p0.y=y0;       // координаты начала кривой Безье, апроксимирующей фокальную линию.
        line.p3.x=x3;       line.p3.y=y3;       // координаты  конца кривой Безье, апроксимирующей фокальную линию.
        // вычисление середины линий.
        line.center.x=(x3 + x0)/2.0;
        line.center.y=(y3 + y0)/2.0;
        // вычисление координат начальных и конечных точек фокальной линии по отношению к ее середине.
        ///line.start_с.x=(x0-x3)/2.0;    line.start_с.y=(y0-y3)/2.0;
        ///line.end_с.x=(x3-x0)/2.0;      line.end_с.y=(y3-y0)/2.0;
        
        
        /// Отработка коротких линий ///
        // Углы и часть параметров нужны и для коротких линий.
        // Можно вынести в отдельную функцию.
        
        // все что длиннее LINE_SIZE рисуем кривыми Безье.
        line.maxCor=ARC;   // ARC светлый,  SPIRAL темный. 
        
        // Выходим из функции, если линия короткая.
        // "LINE_SIZE=12" в pix задаем в GGraphBezier.h    //   TRESHOLD_LINE    ithreshold_Line;
        if (LengthLine<LINE_SIZE) {
            
            // Вызов функции вычисления длины линии и угола между линией и горизонталью.
            float alpha;
            LineAngleLength(x3 - x0, y3 - y0, alpha, line.len); // , alpha, length
            ///cout<<"AlphaPoint0="<<line.alpha<<"     LengthPoint0="<<line.len<<endl;

            // Сохраняем угол между линией и горизонталью, а также ему равные, в случае коротких линий,
            // углы наклона контрольных векторов P1 и P2 (0 до 360° grade).
            line.alphaP1=line.alpha=alpha;
            alpha=alpha+180;
            if (alpha>=360) alpha=alpha-360;
            line.alphaP2=alpha;
            
            // Линии короче LINE_SIZE является прямыми линиями (=12).
            line.corL=100; line.maxC=100; line.maxCor=LINE;
            return;
        }
        // Возвращаем:
        ///line.maxCor=MaxCor;        // - MaxCor, прямая-1, дуга-2, спираль-3, круг-4
        ///line.maxC=(int)maxC;       // - maxC, максимальный коэффициент корреляции линии (0-100%).
        ///line.corL=(int)cor_L;      // - cor_L, коэффициент корреляции линии с прямой (0-100%).
        
        
        
        /// Запуск подпрограмм для подсчета характеристик фокальных линий ///
        
        // Функция для сглаживания фокальной линии.
        uint K_Mid2=4; // 3-4 для коротких линий // 8-12 для длинных линий
        if (LengthLine<=6) K_Mid2=2;    if (LengthLine>6 && LengthLine<=16) K_Mid2=3;
        if (LengthLine>16 && LengthLine<=32) K_Mid2=4;  if (LengthLine>32) K_Mid2=6;
        //focalLineMiddleA(line, K_Mid2);
        focalLineMiddleB(line, K_Mid2);
     // time=0.065
        
        // Функция для подсчета текущей длинны сглаженной фокальной линии,
        // на основе подсчета расстояний между точками сглаженной фокальной линии.
        ///focalLineLengthM(line);
     // time=0.076
        
        
        // Функция для создания таблицы определения координат по длине сглаженной фокальной линии.
        focalLineTabM(line);
        
        // Функция для подсчета реальных углов и длины сглаженной фокальной линии.
        ///focalLineAngleM(line);
     // time=0.08
        
        
        // Функция для сглаживания (интерполяции) несколько крайних pix фокальной линии. В разработке.
        float LengthLineM=line.lenM;  // длина сглаженной фокальной линии.
        if (LengthLineM>=LINE_SIZE) { // LengthLineM м.б. существенно меньше чем LengthLine (до двух раз).
            int strL, endL;
            endL=strL=(int)LengthLineM/5;      if (LengthLineM>16) endL=strL=2;      //endL=strL=0;
            ///if (LengthLine>6 && LengthLine<=16) endL=strL=4;     if (LengthLine>16) endL=strL=4;
            focalLineEnds(line, strL, endL);
        }
        /**/
       
        // Функция для вычисления параметров кривой Безье только по полной длине фокальной линии.
        focalLineBezierF(line);

        
        // Функция с возможностью вычисления параметров кривой Безье по части фокальной линии. // 0,0 по полной длине.
////        float LengthLineM=line.lenM;  // длина сглаженной фокальной линии. LINE_SIZE=6 в pix задаем в GGraphBezier.h
////        int str_L=0;    if (LengthLine>8) str_L=1;   if (LengthLine>24) str_L=2;    int end_L=LengthLineM - str_L;;
////        focalLineBezierP(line, str_L, end_L);
        /**/

     // time=0.08
        

        
        // Функция для вычисления длины кубической кривой Безье (прямое вычисление по уравнению).
        BezierLength(line);
      
        // Функция для очень быстрого приближенного вычисления длины кубической кривой Безье.
        BezierLengthApproximate(line);
     // time=0.092
        
        // пока не актуально //
        
        // Функция для распознования фокальных линиий с помощью центра масс: прямой, дуги, круга, и спирали.
        ///focalLineRecMass(line);
        
        // Вызов функции для подсчета характеристик фокальной линии: коэффициента корреляции линии
        // с кругом и диаметра круга.
        ///focalLineRecCircle2(line);
        
 /**/ // цифры time без углов и Pg0_pix=7;

        //K_Mid2=(int)sqrt((float)len);   if (K_Mid2<3) K_Mid2=3;    if (K_Mid2>10) K_Mid2=10;
        
    }//____________________________________________________________________________
    

    //*****************************************************************************
 
                            /// СРЕДНЕЕ ///
    // Функция для сглаживания фокальной линии. Новая версия. Не до конца отлажена.
    
    void GGraphBezier::focalLineMiddleA(OCRFocalLineBezier &line, uint K_Mid2){
        
        // Функция предназначена для получения усредненных координат точек фокальной линии по "x" и "y".
        // Подсчитывается скользящее среднее по координатам фокальной линии.
        // Без сглаживания фокальной линии подсчитать ее характеристики практически не возможно.
        // Среднее вычисляется по декартовым координатам "x", "y" (short) которые становятся float.
        // Для применения функции необходимо предварительно подсчитать массив интегральных сумм sData.
    
        // Параметры вызова функции //
        /// uint K_Mid2         // половина величины (ширины) усреднения pix.
        
        int index=0;    // Переменная цикла по количеству точек фокальной линий.
        //int k=0;        // копия K_Mid2
        //float sdx=0, sdy=0;
        //float x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;   // Декартовы координаты линии. //
        
        // длина фокальной линии (количество промежуточных точек вдоль фокальной линии).
        uint LengthLine=line.lenA;
        //cout<<"LengthLine="<<LengthLine<<"     K_Mid2="<<K_Mid2<<endl;
        
        // ограничение величины (ширины) усреднения pix.
        if (K_Mid2<1) K_Mid2=1;    if (K_Mid2>16) K_Mid2=16;
        ///cout<<"K_Mid2="<<k<<"    K_Mid2="<<endl;
        
        int GrafT=0 ;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)
        if (GrafT) {
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для сглаживания фокальной линии   focalLineMiddleA() "<<endl;
            cout<<"Половина величины (ширины) усреднения pix  K_Mid2="<<K_Mid2<<endl;
        }
        /**/


        // резервируем размер (resize).
        line.mData.resize(LengthLine);    // +1 нужна для алгоритмам сглаживания фокальной линии.
        
        // получаем прямые указатели на массивы координат точек фокальной линии.
        //OCRPointFloat *sData=(OCRPointFloat*)&line.sData[0];    // указатели на массивы интегральных сумм точек фокальной линии.
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];    // указатели на массивы адресов точек сглаженной фокальной линии.
        
        // Выходим из функции, если линия короткая, заполнив сглаженный массив исходными точками фокальной линии.
        ///if (LengthLine<4) {
        ///    for ( index=0; index<LengthLine; index++ ) { mData[index]=data[index]; }
        ///    return;
        ///}
        
        /*
         float str_x=line.start.x;       float str_y=line.start.y;    // координаты начала фокальной линии.
         float end_x=line.end.x;         float end_y=line.end.y;      // координаты конца фокальной линии.
         
         int LengthLine_=LengthLine-1;

        
         //K_Mid2=1;
        
        // Вычисление скользящего среднего переменной величины по координатам фокальной линии. //
        
        // добавляем первую точку.
        mData[0].x=str_x;            mData[0].y=str_y;        // координаты начала фокальной линии.
        
        //int LengthLine_=LengthLine-1;
        for ( index=1; index<LengthLine_; index++ ) {
            
            k=K_Mid2;
            // ограничение на величину усреднения в начале фокальной линии.
            if (index<K_Mid2) k=index;
            // ограничение на величину усреднения в конце фокальной линии.
            if ((LengthLine-index)<=K_Mid2) k=LengthLine-index-1;
            // востановление ограничения на величину усреднения в начале фокальной линии.
            // необходимо если длина фокальной линии LengthLine меньше полной величины усреднения 2*K_Mid2
            if (index<k) k=index;
            
            
            // отработка вылета за начало массива sData.
            int ik1=index-k-1;
            if (ik1>-1) { sdx=sData[ik1].x;   sdy=sData[ik1].y; } // >=0
            
            // Вычисление скользящего среднего для текущей точки с помощью массива интегральных сумм по координате "x".
            ///mData[index].x=(sData[index+k].x - sData[index-k-1].x)/(k+k+1);
            mData[index].x=(sData[index+k].x - sdx)/(k+k+1);

            // Вычисление скользящего среднего для текущей точки с помощью массива интегральных сумм по координате "y".
            ///mData[index].y=(sData[index+k].y - sData[index-k-1].y)/(k+k+1);
            mData[index].y=(sData[index+k].y - sdy)/(k+k+1);

            //if (GrafT)cout<<"index="<<index<<"  k="<<k<<"   x="<<data[index].x<<"   y="<<data[index].y<<"   mData="<<mData[index].y<<endl;

        } // for
        
        // добавляем последнею точку.
        mData[LengthLine_].x=end_x;    mData[LengthLine_].y=end_y;        // координаты конца фокальной линии.
        */
        
        OCRPointFloat *data=(OCRPointFloat*)&line.data[0];    // указатели на массивы интегральных сумм точек фокальной линии.
        for ( index=0; index<LengthLine; index++ ) {
            mData[index].x=data[index].x;   mData[index].y=data[index].y;
        } // for

        ///mData[0]=data[0];        // добавляем первую точку (оператор).
        ///mData[LengthLine_]=data[LengthLine_]; // (оператор) LengthLine-1
        
        //----------------------------------------------------------------------------------------------------------------
        
        
        /*
         float alpha=0.28; // чем меньшее значение, тем больше сглаживание 0.4    0.28
         int n=2; // 2
         float mx0, my0, mx1, my1;

        line.mTab.resize(LengthLine);                // резервируем размер resize.
        OCRPointFloat *mTab=(OCRPointFloat*)&line.mTab[0];    // получаем указатель mTab.
        //if(GrafT)cout<<"     line.mTab.size()="<<line.mTab.size()<<endl<<endl;
        
        // резервируем размер (resize).
        //line.data.resize(LengthLine);    // +1 нужна для алгоритмам сглаживания фокальной линии.
        //OCRPointFloat *data=(OCRPointFloat*)&line.data[0];    // получаем указатель на массив Data
 //       data[0].x=str_x;            data[0].y=str_y;        // координаты начала фокальной линии.
        // добавляем последнею точку.
 //       data[LengthLine_].x=end_x;    data[LengthLine_].y=end_y;        // координаты конца фокальной линии.

        // Дополнительное сглаживание всей фокальной линии простой рекурсией с дробным коэффициентом.//
        mx0=str_x;    my0=str_y;        // начальные условия
        mx0=mData[n-1].x;    my0=mData[n-1].y;        // начальные условия
        for ( index=1+n; index<LengthLine-n; index++ ) {  // - i - 1  // LengthLine-1
        // в конце линии не обработана одна точка LengthLine_-1   n=1
            
            x1=mData[index].x;              y1=mData[index].y;
            ///x1=data[index].x;               y1=data[index].y; // =data[index].x;
            
            // вычисление трехточки.
            // m[i] = m[i-1] + alpha * (x[i] - m[i-1])      // y[i] = y[i-1] + alpha * (x[i] - y[i-1])
            mData[index].x=mx1=mx0 + alpha*(x1-mx0);   mData[index].y=my1=my0 + alpha*(y1-my0);
            //mTab[index].x=mx1=mx0 + alpha*(x1-mx0);   mTab[index].y=my1=my0 + alpha*(y1-my0); // нужен другой массив m[i]=MData
       
            mx0=mx1;
            my0=my1;
            
        } // for
        
  
        //for ( index=0; index<LengthLine; index++ ) {
        //     mData[index].x=mTab[index].x;   mData[index].y=mTab[index].y;
        //} // for
       
        
        // добавляем первую точку.
        ///mData[0].x=str_x;               mData[0].y=str_y;           // координаты начала фокальной линии.
        // добавляем последнею точку.
        ///mData[LengthLine_].x=end_x;     mData[LengthLine_].y=end_y; // координаты конца фокальной линии.
        

        
        // Дополнительное реверсивное сглаживание всей фокальной линии.//
        // Компенсирует фазовые сдвиги.
        mx0=end_x;    my0=end_y;        // начальные условия
        mx0=mData[LengthLine_-(n-1)].x;    my0=mData[LengthLine_-(n-1)].y;        // начальные условия
        for ( index=LengthLine_-n; index>-1+n; index-- ) {  // LengthLine_=LengthLine-1;  //  index>-1+1;
        // в начале линии не обработана одна точка index=0; n=1
            
            x1=mData[index].x;              y1=mData[index].y;
            ///x1=data[index].x;               y1=data[index].y;
            
            // m[i] = m[i-1] + alpha * (x[i] - m[i-1])
            mData[index].x=mx1=mx0 + alpha*(x1-mx0);   mData[index].y=my1=my0 + alpha*(y1-my0);
            //mTab[index].x=mx1=mx0 + alpha*(x1-mx0);   mTab[index].y=my1=my0 + alpha*(y1-my0);
            
            mx0=mx1;
            my0=my1;
            
        } // for
        
        
       // for ( index=0; index<LengthLine; index++ ) {
       //     mData[index].x=mTab[index].x;   mData[index].y=mTab[index].y;
        //} // for
        
        
        // добавляем первую точку.
     //   mData[0].x=str_x;               mData[0].y=str_y;           // координаты начала фокальной линии.
        // добавляем последнею точку.
     //   mData[LengthLine_].x=end_x;     mData[LengthLine_].y=end_y; // координаты конца фокальной линии.
        
        //----------------------------------------------------------------------------------------------------------------
        
        //mData[LengthLine].x=end_x;     mData[LengthLine].y=end_y; // координаты конца фокальной линии.
        
        ///for ( index=0; index<LengthLine; index++ ) { // - i - 1
        
        ///line.mTab.resize(LengthLine);                // резервируем размер resize.
        ///OCRPointFloat *mTab=(OCRPointFloat*)&line.mTab[0];    // получаем указатель mTab.
        ///if(GrafT)cout<<"     line.mTab.size()="<<line.mTab.size()<<endl<<endl;
        
        
        // for ( index=0; index<LengthLine; index++ ) {
       //     mData[index].x=mTab[index].x;   mData[index].y=mTab[index].y;
       //  } // for
    */     
        
    }//__________________________________________________________________________
    
    
    //*****************************************************************************
    
    
                    /// СРЕДНЕЕ ///
    // Функция для сглаживания фокальной линии. Основная рабочая версия. Предыдущая версия.
    
    void GGraphBezier::focalLineMiddleB(OCRFocalLineBezier &line, uint K_Mid2){
        
        // Функция предназначена для получения усредненных координат точек фокальной линии по "x" и "y".
        // Подсчитывается скользящее среднее по координатам фокальной линии.
        // Без сглаживания фокальной линии подсчитать ее характеристики практически не возможно.
        // Среднее вычисляется по декартовым координатам "x", "y" (short) которые становятся float.
        // Для применения функции необходимо предварительно подсчитать массив интегральных сумм sData.
        
        // Параметры вызова функции //
        /// uint K_Mid2         // половина величины (ширины) усреднения pix.
        
        int index=0;    // Переменная цикла по количеству точек фокальной линий.
        int k=0;        // копия K_Mid2
        float sdx=0, sdy=0;
        float x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;   // Декартовы координаты линии. //
        
        // длина фокальной линии (количество промежуточных точек вдоль фокальной линии).
        uint LengthLine=line.lenA;
        //cout<<"LengthLine="<<LengthLine<<"     K_Mid2="<<K_Mid2<<endl;
        
        // ограничение величины (ширины) усреднения pix.
        if (K_Mid2<1) K_Mid2=1;    if (K_Mid2>16) K_Mid2=16;
        ///cout<<"K_Mid2="<<k<<"    K_Mid2="<<endl;
        
        int GrafT=0 ;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)
        if (GrafT) {
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для сглаживания фокальной линии   focalLineMiddleB() "<<endl;
            cout<<"Половина величины (ширины) усреднения pix  K_Mid2="<<K_Mid2<<endl;
        }
        /**/

        
        // резервируем размер (resize).
        line.mData.resize(LengthLine);
        
        // получаем прямые указатели на массивы координат точек фокальной линии.
        OCRPointFloat *sData=(OCRPointFloat*)&line.sData[0];    // указатели на массивы интегральных сумм точек фокальной линии.
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];    // указатели на массивы адресов точек сглаженной фокальной линии.
        
        // Выходим из функции, если линия короткая, заполнив сглаженный массив исходными точками фокальной линии.
        ///if (LengthLine<4) {
        ///    for ( index=0; index<LengthLine; index++ ) { mData[index]=data[index]; }
        ///    return;
        ///}
        
        float str_x=line.start.x;       float str_y=line.start.y;    // координаты начала фокальной линии.
        float end_x=line.end.x;         float end_y=line.end.y;      // координаты конца фокальной линии.
        
        
        
        // Вычисление скользящего среднего переменной величины по координатам фокальной линии. //
        
        // добавляем первую точку.
        mData[0].x=str_x;            mData[0].y=str_y;        // координаты начала фокальной линии.
        
        int LengthLine_=LengthLine-1;
        for ( index=1; index<LengthLine_; index++ ) {
            
            k=K_Mid2;
            // ограничение на величину усреднения в начале фокальной линии.
            if (index<K_Mid2) k=index;
            // ограничение на величину усреднения в конце фокальной линии.
            if ((LengthLine-index)<=K_Mid2) k=LengthLine-index-1;
            // востановление ограничения на величину усреднения в начале фокальной линии.
            // необходимо если длина фокальной линии LengthLine меньше полной величины усреднения 2*K_Mid2
            if (index<k) k=index;
            
            
            // отработка вылета за начало массива sData.
            int ik1=index-k-1;
            if (ik1>-1) { sdx=sData[ik1].x;   sdy=sData[ik1].y; } // >=0
            
            // Вычисление скользящего среднего для текущей точки с помощью массива интегральных сумм по координате "x".
            ///mData[index].x=(sData[index+k].x - sData[index-k-1].x)/(k+k+1);
            mData[index].x=(sData[index+k].x - sdx)/(k+k+1);
            
            // Вычисление скользящего среднего для текущей точки с помощью массива интегральных сумм по координате "y".
            ///mData[index].y=(sData[index+k].y - sData[index-k-1].y)/(k+k+1);
            mData[index].y=(sData[index+k].y - sdy)/(k+k+1);
            
            //if (GrafT)cout<<"index="<<index<<"  k="<<k<<"   x="<<data[index].x<<"   y="<<data[index].y<<"   mData="<<mData[index].y<<endl;
            
        } // for
        /**/
        // добавляем последнею точку.
        mData[LengthLine_].x=end_x;    mData[LengthLine_].y=end_y;        // координаты конца фокальной линии.
        
        
        
        
         // смещают конечные точки
         // Дополнительное сглаживание всей фокальной линии фиксированной скользящей трехточкой.//
         x0=str_x;   y0=str_y;   x1=mData[1].x;    y1=mData[1].y;        // начальные условия
         for ( index=2; index<LengthLine; index++ ) {
             //x0=line.mData[index-2].x;     y0=line.mData[index-2].y;
             //x1=line.mData[index-1].x;     y1=line.mData[index-1].y;
             //x2=line.mData[index].x;       y2=line.mData[index].y;
             x2=mData[index].x;              y2=mData[index].y;
         
             // вычисление трехточки.
             mData[index-1].x=(x0+x1+x1+x2)/4.0;   mData[index-1].y=(y0+y1+y1+y2)/4.0;
         
             x0=x1;   x1=x2;   // сдвиг влево двухразрядного регистра по "х"
             y0=y1;   y1=y2;   // сдвиг влево двухразрядного регистра по "y"
         
         } // for
         /**/
        
        
         // смещают конечные точки
         // Дополнительное сглаживание всей фокальной линии скользящей пятиточкой.//
         x0=str_x;        y0=str_y;        x1=mData[1].x;   y1=mData[1].y;
         x2=mData[2].x;   y2=mData[2].y;   x3=mData[3].x;   y3=mData[3].y;        // начальные условия
         for ( index=4; index<LengthLine; index++ ) {
         
             x4=mData[index].x;    y4=mData[index].y;
         
             // вычисление пятиточки.
             mData[index-2].x=(x0+x1+x1+x2+x2+x2+x3+x3+x4)/9.0;
             mData[index-2].y=(y0+y1+y1+y2+y2+y2+y3+y3+y4)/9.0;
         
             x0=x1;   x1=x2;    x2=x3;   x3=x4;    // сдвиг влево четырехразрядного регистра по "х"
             y0=y1;   y1=y2;    y2=y3;   y3=y4;    // сдвиг влево четырехразрядного регистра по "y"
         
         } // for
         /**/
        
        
         // Дополнительное сглаживание трехточкой начала и конца фокальной линии.//
         
         x0=str_x;                                   y0=str_y;       // координаты начала фокальной линии.
         x1=mData[1].x;                              y1=mData[1].y;
         x2=mData[2].x;                              y2=mData[2].y;
         x3=mData[3].x;                              y3=mData[3].y;
         // Сглаживание двух первых точек.
         mData[1].x=(x0+x1+x1+x2)/4.0;               mData[1].y=(y0+y1+y1+y2)/4.0;
         mData[2].x=(x1+x2+x2+x3)/4.0;               mData[2].y=(y1+y2+y2+y3)/4.0;
         
         x0=end_x;                                   y0=end_y;       // координаты конца фокальной линии.
         x1=mData[LengthLine-2].x;                   y1=mData[LengthLine-2].y;
         x2=mData[LengthLine-3].x;                   y2=mData[LengthLine-3].y;
         x3=mData[LengthLine-3].x;                   y3=mData[LengthLine-4].y;
         // Сглаживание двух последних точек.
         mData[LengthLine-2].x=(x0+x1+x1+x2)/4.0;    mData[LengthLine-2].y=(y0+y1+y1+y2)/4.0;
         mData[LengthLine-3].x=(x1+x2+x2+x3)/4.0;    mData[LengthLine-3].y=(y1+y2+y2+y3)/4.0;
         /**/
        
        ///mData[0]=data[0];        // добавляем первую точку (оператор).
        ///mData[LengthLine_]=data[LengthLine_]; // (оператор) LengthLine-1
        
        //----------------------------------------------------------------------------------------------------------------
        
        /*
         // прямоугольная весовая функция
         // y(n)= y(n-1)+x(n)-x(n-2k-1)
         // треугольная весовая функция
         // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)
         
         // Дополнительное сглаживание всей фокальной линии фиксированной скользящей трехточкой.//
         float x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;   // Декартовы координаты линии. //
         ///float mx0, my0, mx1, my1, mx2, my2;
         float mx0, my0, mx1, my1, mx2, my2, mx3, my3, mx4, my4;
         ///x0=str_x;   y0=str_y;   x1=mData[1].x;    y1=mData[1].y;        // начальные условия
         ///mx0=str_x;  my0=str_y;  mx1=mData[1].x;   my1=mData[1].y;        // начальные условия
         x0=str_x;        y0=str_y;        x1=mData[1].x;   y1=mData[1].y;
         x2=mData[2].x;   y2=mData[2].y;   x3=mData[3].x;   y3=mData[3].y;        // начальные условия
         mx0=str_x;        my0=str_y;        mx1=mData[1].x;   my1=mData[1].y;
         mx2=mData[2].x;   my2=mData[2].y;   mx3=mData[3].x;   my3=mData[3].y;        // начальные условия
         for ( index=0; index<LengthLine; index++ ) {
         //x0=line.mData[index-2].x;     y0=line.mData[index-2].y;
         //x1=line.mData[index-1].x;     y1=line.mData[index-1].y;
         //x2=line.mData[index].x;       y2=line.mData[index].y;
         x4=mData[index].x;              y4=mData[index].y;
         
         // треугольная весовая функция
         // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn // k=0
         // y(n)= -y(n-2)+2y(n-1)+x(n-4)-2x(n-2)+xn // k=1
         if (index>3) { mData[index-4].x=mx4=-mx2 + 2*mx3 + x0 - 2*x2 + x4;   mData[index-4].y=my4=-my2 + 2*my3 + y0 - 2*y2 + y4; }
         // нужен другой массив y(n)=MData
         
         //x0=x1;      x1=x2;   // сдвиг влево двухразрядного регистра по "х"
         //y0=y1;      y1=y2;   // сдвиг влево двухразрядного регистра по "y"
         //mx0=mx1;    mx1=mx2;
         // my0=my1;    my1=my2;
         x0=x1;   x1=x2;    x2=x3;   x3=x4;    // сдвиг влево четырехразрядного регистра по "х"
         y0=y1;   y1=y2;    y2=y3;   y3=y4;    // сдвиг влево четырехразрядного регистра по "y"
         mx0=x1;   mx1=x2;    mx2=x3;   mx3=mx4;    // сдвиг влево четырехразрядного регистра по "х"
         my0=y1;   my1=y2;    my2=y3;   my3=my4;    // сдвиг влево четырехразрядного регистра по "y"
         // прямоугольная весовая функция
         // y(n)= y(n-1)+xn-x(n-2k-1)
         // треугольная весовая функция
         // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn
         } // for
         */
        
    }//__________________________________________________________________________
    
    
    
    //*****************************************************************************
    
                            /// LineM ///
    // Функция для подсчета текущей длины сглаженной фокальной линии.
    
    void GGraphBezier::focalLineLengthM(OCRFocalLineBezier &line){
        
        // Подсчитываются реальные расстояния между точками сглаженной фокальной линии.
        // Результат сохраняется в массиве sLine, который сейчас заремачен (см. в конце функции).
        // Расстояние между точками сглаженной фокальной линии вычисляется по Теореме Пифагора.
        // Заполняется массив интегральных сумм расстояний между точками сглаженной фокальной линии sLine.
        // Подсчитывается реальная длина сглаженной фокальной линии LengthLineM, по сетке float.
        // Реальная длина сглаженной фокальной линии меньше пиксельной длины на 20-30%
        // В конце функции есть практически готовый и тестированнsq код для вычисления расстояния
        // между точками по предпросчитанной таблицt table_Sq, но только для целочисленных координат.
        // Реально хватает быстродействия обычного извлечения корня.
        
        int index;
        float x0,y0,x1,y1;      // Декартовы координаты соседних точек фокальной линии.
        float dx,dy;            // Переменные разностей координат.
        float rg_x1, rg_y1;     // Переменные двухразрядного регистров  "х" и "у".
        //float lineM=0;        // Реальное расстояние между пикселями сглаженной фокальной линии.
        float LengthLineM=0;    // Реальная длина сглаженной фокальной линии.
        
        
        //TIME_START
        
        // Длинна фокальной линии состоящая pix (количество промежуточных точек вдоль фокальной линии).
        int LengthLine=(int)line.mData.size();
        if(!LengthLine)return;
        ///int LengthLine=(uint)line.data.size();
        ///if(!line.mData.size())return;
        
        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }

        if (GrafT) {
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для подсчета текущей длинны сглаженной фокальной линии   focalLineLengthM() "<<endl;
            cout<<"Длинна фокальной линии  LengthLine="<<LengthLine<<endl;
        }

        // получаем указатель на массив сглаженных координат точек фокальной линии.
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];
        // получаем указатель на массив интегральных сумм.
        ////line.sLine.resize(LengthLine);          // resize
        ////float *sLine=(float*)&line.sLine[0];    // получаем указатель
        

        // Реальная длина сглаженной фокальной линии. //
        LengthLineM=0;    rg_x1=mData[0].x;   rg_y1=mData[0].y;    //// sLine[0]=0;
        for ( index=1; index<LengthLine; index++ ) {
            
            //x0=line.mData[index].x;   y0=line.mData[index].y;
            //x1=line.mData[index-1].x; y1=line.mData[index-1].y;
            x0=mData[index].x;          y0=mData[index].y;
            x1=rg_x1;                   y1=rg_y1;
            
            // Вычисление расстояния между соседними точками сглаженной фокальной линии.
            dx=x1-x0;  dy=y1-y0;
            LengthLineM+=sqrt(dx*dx+dy*dy); // Длина сглаженной фокальной линии.
            
            // Заполнение массива интегральных сумм расстояний между соседними точками сглаженной фокальной линии.
            ////sLine[index]=LengthLineM;
            ///if (GrafT) { cout<<"LengthLineM="<<LengthLineM<<"    sLine[index]="<<sLine[index]<<endl; }
            
            rg_x1=x0;   // сдвиг влево двухразрядного регистра "rg_x1, rg_x0"  по "х"
            rg_y1=y0;   // сдвиг влево двухразрядного регистра "rg_x1, rg_x0"  по "y"
            
        } // for ( index

        line.lenM=LengthLineM;
        line.lenP=LengthLineM;  // Сохранение начальной длины кривой Безье. В будущем это часть длины сглаженной фокальной линии.
        if (GrafT) { cout<<"длина сглаженной фокальной линии   LengthLineM="<<LengthLineM<<endl; }

        
        //TIME_PRINT_

        // Массив data (float, но по сути int) это упорядоченный набор координат "x" и "y" точек одной фокальной линии.
        // Точки находятся в узлах pix сетки, расстояния между точками одинаковы.
        
        // sData (float) это массив интегральных сумм по массиву data. Массив текущей длинны не сглаженной фокальной линии.
        
        // Массив mData (float) это упорядоченный набор координат "x" и "y" точек одной фокальной линии после их усреднения.
        // Точки находятся не в узлах pix сетки, расстояния между точками не одинаковы.
        
        // sLine это массив интегральных сумм по массиву mData. Массив текущей длинны сглаженной фокальной линии.
        // С помощью перебора этого массивы находим координаты по расстоянию вдоль фокальной линии от ее начала.
        // С линейной уточняющей интерполяцией между точками.
        
        // mTab разреженная таблица для получения координат (float) "x" и "y" по расстоянию вдоль фокальной линии от ее начала
        // С линейной уточняющей интерполяцией между точками. mTab это тот же sLine, только расстояния между точками
        // вдоль фокальной линии одинаковы.

        /**/
        
        
        //-----------------------------------------------------------------------------------------------------------
        
        // Практически готовая и тестированная функция //
        // Создание предпросчитанной таблицы table_Sq. Для целочисленной сетки *16
        // Но реально хватает быстродействия обычного извлечения корня.
        
        // Для улучшения быстродействия bb=(float)(dx*dx+dy*dy); можно считать по таблице uchar(4+4=256),
        // т.е. dx==4р dy==4р имеет по 16 градаций, что соответствует мелкой сетки.
        // dx,dy<=16 иногда встречаются dx,dy=24  (в 1/10 случаев =24 и никогда >24)
        
        /*
         float table_Sq[256]; // unsigned char
         unsigned char rg0;  // байтовый регистр для хранения отдельных 8 бит маски
         
         // цикл перебора всех возможных значений
         for (int index=0; index < 256; index++){
         rg0=index;
         table_Sq[index]=0; // заполнение таблицы нулями.
         
         // разборка байтового регистра на отдельные 4 битные блоки //
         d_x=rg0 & 0xF; // маской 00001111 выделяем младшие 4 разряда.
         d_y=rg0>>4;    // сдвигаем (/16) на 4 разряда, получаем старшие 4 разряда.
         //d_x=x_2-x_1;  d_y=y_2-y_1;
         table_Sq[index]=sqrt((float)(d_x*d_x + d_y*d_y));
         } // for
         
         if (GrafT) cout<<"значение байтового регистра для хранения отдельных 8 бит маски, в конце цикла   rg0="<<(unsigned short)rg0<<endl;
         // cout<<endl;
         
         // разборка байтового регистра на отдельные 4 битные блоки
         unsigned char rg0=0xEC;
         d_x=rg0 & 0xF; // маской 00001111 выделяем младшие 4 разряда.
         d_y=rg0>>4;    // сдвигаем (/16) на 4 разряда, получаем старшие 4 разряда.
         if(GrafT)cout<<"rg0="<<(short)rg0<<"  d_x="<<d_x<<"  d_y="<<d_y<<endl;
         
         // сборка байтового регистра из отдельные 4 битовых блоков находящихся в int  //
         d_x=d_x & 0xF; // маской ...000000001111 выделяем младшие 4 разряда.
         d_y=d_y & 0xF; // маской ...000000001111 выделяем младшие 4 разряда.
         d_y=d_y<<4;    // сдвигаем (*16) на 4 разряда, получаем старшие 4 разряда.
         rg0=d_x+d_y;   // сборка байтового регистра.
         if(GrafT)cout<<"rg0="<<(short)rg0<<"  d_x="<<d_x<<"  d_y="<<d_y<<endl;
         */

 //vector<float>sLine;    // вектор для массива интегральных сумм расстояний между соседними точками сглаженной фок.линии (NO Б.Д.).        
        // получаем указатель на массив интегральных сумм.
        ///line.sLine.resize(LengthLine);          // resize
        ///float *sLine=(float*)&line.sLine[0];    // получаем указатель
        ///sLine[0]=0;
        // Заполнение массива интегральных сумм расстояний между соседними точками сглаженной фокальной линии.
        ///          sLine[index]=LengthLineM;
        
        
    }//____________________________________________________________________________
    
    
    
    
    //*****************************************************************************
    
                                /// TabM ///
    // Функция для создания таблицы определения координат по длине сглаженной фокальной линии.
    
    void GGraphBezier::focalLineTabM(OCRFocalLineBezier &line){
        
        // Передискретизация //
        // Подсчитываются таблица для получения координат (float) "x" и "y" по расстоянию (float)
        // вдоль фокальной линии от ее начала. С линейной уточняющей интерполяцией между точками.
        // Без таблицы, для вычисления каждой координаты, приходится применять линейное чтение массива sLine.
        // Также с линейной уточняющей интерполяцией.

        int index;
        ///float x_0,y_0,x_1,y_1;  // Декартовы координаты соседних точек фокальной линии.
        float x0, y0, x1, y1;   // Декартовы координаты соседних точек фокальной линии
        float dx,dy;            // Переменные разностей координат.
        //float lineM=0;        // Реальное расстояние между пикселями сглаженной фокальной линии.
        float LengthLineM=0;    // Реальная длина сглаженной фокальной линии.

        
        // Длина фокальной линии состоящая из pix (количество промежуточных точек вдоль фокальной линии).
        int LengthLine=line.lenA;
        ///int LengthLine=(uint)line.data.size();
        ///if(!line.mData.size())return;
        
        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }1777 1851
        //if(line.index==1851){ // 153    1697 // 2611 2615 // 1689
        //    GrafT=1;
        //}else GrafT=0;
        
        if (GrafT) {
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для подсчета таблицы определения координат по длине фокальной линии.   focalLineTabM() "<<endl;
            cout<<"Длинна фокальной линии  LengthLine="<<LengthLine<<endl;
        }
        
        // получаем указатель на массив сглаженных координат точек фокальной линии.
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];  // указатели на массивы адресов точек сглаженной фокальной линии.

        int mTab_size=LengthLine*5; // обычно, после нормализации, массив почти не меняет свой размер и никогда не может превысить *3.
        line.mTab.resize(mTab_size);                // резервируем размер resize.
        OCRPointFloat *mTab=(OCRPointFloat*)&line.mTab[0];    // получаем указатель mTab.
        if(GrafT)cout<<"     line.mTab.size()="<<line.mTab.size()<<endl<<endl;
        
        memset((char*)mTab, 0, mTab_size*sizeof(OCRPointFloat));        //заполняем нулями весь массив.

        // полный ключ к получению точных координат точек по произвольному расстоянию вдоль длины фокальной линии.
        float lineL, lineL_old ;
        // целочисленный (округленный до целого) ключ для выборки из таблицы и его предыдущее значение.
        int  indexL=0, indexL_old=0;
        float k;    // коэффициент линейной интерполяции.

        
        
        // Реальная длина сглаженной фокальной линии. //
        LengthLineM=0;
        x0=mData[0].x;  y0=mData[0].y;  lineL_old=-1;    indexL_old=-1; // -1  // начальные условия регистра
        for ( index=0; index<LengthLine; index++ ) { // index=1;
            
            // Вычисление расстояния между соседними точками фокальной линии. //
            
            //x1=line.mData[index].x;   y1=line.mData[index].y;
            //x0=line.mData[index-1].x; y0=line.mData[index-1].y;
            x1=mData[index].x;          y1=mData[index].y;
            
            // Вычисление расстояния между соседними точками сглаженной фокальной линии.
            dx=x1-x0;  dy=y1-y0;
            LengthLineM+=sqrt(dx*dx+dy*dy); // Длина сглаженной фокальной линии.
            
            //if(GrafT)cout<<"     index="<<index<<"     LengthLineM="<<LengthLineM<<"     (int)LengthLineM="<<(int)LengthLineM<<endl;
            
            //---------------

            // Нормализация координат сглаженной фокальной линии по расстоянию вдоль длины фокальной линии. //
            
            // полный ключ к получению точных координат точек по произвольному расстоянию вдоль длины фокальной линии.
            // точные координаты по полному ключу получаются с помощью линейной интерполяцией между двумя целочисленными точками таблицы.
            lineL=LengthLineM;  // float
            // целочисленный (округленный до целого) ключ для заполнения и выборки из таблицы.
            indexL=(int)lineL;  // int
            
            // таблица //
            // Основное заполнение таблицы точными расстояниями вдоль длины фокальной линии от ее начала до всех точек линии.
            // Индексом таблицы является округленное до целого значение этого расстояния.
            OCRPointFloat & mTab_in0=mTab[indexL];       // указание компилятору на замену mTab[indexL] на mTab_in0, для увеличения скорости.
            mTab_in0.x=x1;      mTab_in0.y=y1;
            
            // Слияние двух ячеек в одну с помощью линейной интерполяции. Примерно 5-10% точек на линию.
            if (indexL==indexL_old) { // если целая часть двух соседних ячеек одинакова.
                // Заполнение одинаковых (по целой части) ячеек таблицы, средним из координат этих ячеек.
                OCRPointFloat & mTab_in1=mTab[indexL_old];
                mTab_in1.x=(x1+x0)/2.0;      mTab_in1.y=(y1+y0)/2.0;
                if (GrafT)cout<<"- отработка усреднением похожих ячейк таблицы   indexL="<<indexL<<"  indexL_old="<<indexL_old<<"  index="<<index<<endl;
            } // if (indexL==indL_old)
            
            // Заполнение пустой ячейки средним из координат соседних ячеек. Примерно 5-10% точек на линию.
            if (indexL>indexL_old+1) { // если индекс двух соседних ячеек отличается больше чем на единицу.
                // Заполнение пустой ячейки таблицы средним из координат соседних точек линии.
                OCRPointFloat & mTab_in2=mTab[indexL_old+1];
                mTab_in2.x=(x1+x0)/2.0;      mTab_in2.y=(y1+y0)/2.0;
                if (GrafT)cout<<"+ отработка пустой ячейки таблицы   indexL="<<indexL<<"  indexL_old="<<indexL_old<<"  index="<<index<<endl;
            } // if (indexL>indL_old+1)
            /**/
            
            ///if (GrafT)cout<<"   lineL="<<lineL<<"  lineL_old="<<lineL_old<<"  index="<<index<<endl;
            // основная линейная интерполяция, расставляющая точки в таблице вдоль длины фокальной линии,
            // на одинаковое расстояние в один pix.
            k=(indexL-lineL_old)/(lineL-lineL_old);
            mTab_in0.x=x0+k*(x1-x0);    mTab_in0.y=y0+k*(y1-y0);
            
            // запоминаем предидущие значения переменных, что бы не лазить в массивы лишний раз.
            x0=x1;              // сдвиг влево двухрехразрядного регистра "x0, x1" по "х"
            y0=y1;              // сдвиг влево двухрехразрядного регистра "y0, y1" по "y"
            lineL_old=lineL;    // сохранение предидущего значения полного ключа float.
            indexL_old=indexL;  // сохранение предидущего целочисленного (округленный до целого) значения ключа int.
            if(GrafT)cout<<"index="<<index<<"  lineL="<<lineL<<"   indexL="<<(int)indexL<<"  x1="<<x1<<"  y1="<<y1<<endl;
            /**/

        } // for ( index
        
        
        line.lenM=LengthLineM;  // Сохранение длины сглаженной фокальной линии
        line.lenP=LengthLineM;  // Сохранение начальной длины кривой Безье. В будущем это часть длины сглаженной фокальной линии.
        if (GrafT) { cout<<endl<<"длина сглаженной фокальной линии   LengthLineM="<<LengthLineM<<endl; }
        
        // Получаем координаты начальных и конечных точек фокальной линии. Тест
        if(GrafT) {
            x0=line.start.x;            y0=line.start.y;        // координаты начала фокальной линии при t2=0.
            float x3=line.end.x;        float y3=line.end.y;    // координаты конца фокальной линии при t2=L, где L=LengthLineM.
            cout<<"line.start.x="<<x0<<"  line.start.y="<<y0<<"   x3=line.end.x="<<x3<<"  line.end.y="<<y3<<endl;
        }
        /**/
        
        int nev_size=indexL+1; // новый размер таблицы mTab.
        
        // resize mData по новому размеру от заведомо большого LengthLine*3 до точного значения nev_size.
        line.mTab.resize(nev_size); // данные в массиве при resize сохраняются.
        if(GrafT)cout<<"новый размер mTab   line.mTab.size()="<<line.mTab.size()<<endl<<endl;
        // новый размер таблицы nev_size получился дискретным (1 pix).
        
        /*
        // Чтобы по длинам полученным из таблицы, можно было получить точную исходную длину фокальной линии LengthLineM,
        // все ячейки таблицы можно умножить на коэффициент k_LineT почти равной единице.
        float k_LineT=LengthLineM/nev_size;
        if(GrafT)cout<<"k_LineT="<<k_LineT<<endl;
        */
        
        
        // вывод графики в порт смотрим gPort.mm (например рисуем mData или mTab).
        // сейчас рисуем mTab.
        
        /// визуализация ///
        
        // Вычисляем еще раз реальную длину сглаженной фокальной линии по теореме Пифагора
        // и выводим в таблицу, где расстояние между соседними точками равно одному pix.
        if(GrafT) {
            if(GrafT)cout<<endl;
            LengthLineM=0;  x0=mTab[0].x; y0=mTab[0].y;
            for ( index=0; index<nev_size; index++ ) {
            
                OCRPointFloat & mTab_in0=mTab[index];
                x1=mTab_in0.x;             y1=mTab_in0.y;
                // Вычисление расстояния между соседними точками сглаженной фокальной линии.
                dx=x1-x0;  dy=y1-y0;
                LengthLineM+=sqrt(dx*dx+dy*dy); // Длина сглаженной фокальной линии.

                x0=x1;   // сдвиг влево двухразрядного регистра "x0, x1"  по "х"
                y0=y1;   // сдвиг влево двухразрядного регистра "x0, x1"  по "y"
            
                cout<<"!! index="<<index<<"   LengthLineM="<<LengthLineM<<"  x0="<<x0<<"  y0="<<y0<<endl;
            
            } // for ( index  ///
        } // if(GrafT)
        /**/

        
        //---------------
        
        // Таблица, описание //
        
        // Таблица позволяет найти координаты x,y по расстоянию вдоль фокальной линии.
        // Индекс это расстояние (округленное до целого), содержимое это координаты x,y.
        // По индексу находятся координата x,y соответствующая расстоянию точно кратному 1 pix (можно домножить).
        // mTab это тот же sLine, только расстояния между точками вдоль фокальной линии одинаковы.
        // Далее координаты x,y уточняются с помощью линейной интерполяции.
        
        // Создание таблицы //
        // Создание таблицы выполняется с помощью линейной интерполяции, пересчетом существующих расстояний и координат
        // в новые регулярные расстояния, кратные 1 pix и в новые соответсвующие координаты.
        
        //indexX        //длина линии.
        //index=1       1 pix
        //index=2       2 pix
        //index=3       3 pix
        //......................
        
        // Координаты длины линии x,y == mTab[indexX], где indexX длина линии.

        //---------------
        
        
        
        /*
                    // вычисление таблицы отдельно от вычисления длинны //
        
        x0=mData[0].x;  y0=mData[0].y;  lineL_old=-1;    indexL_old=-1; // -1  // начальные условия регистра
        for ( index=0; index<LengthLine; index++ ) {
        
        // Упорядочение координат сглаженной фокальной линии по расстоянию вдоль длины фокальной линии. //
        
        // полный ключ к получению точных координат точек по произвольному расстоянию вдоль длины фокальной линии.
        // точные координаты по полному ключу получаются с помощью линейной интерполяцией между двумя целочисленными точками таблицы.
        /// lineL=LengthLineM;
        lineL=sLine[index]; // float
        // целочисленный (округленный до целого) ключ для заполнения и выборки из таблицы.
        indexL=(int)lineL;
        
        // таблица //
        
        // Основное заполнение таблицы точными расстояниями вдоль длины фокальной линии от ее начала до всех точек линии.
        // Индексом таблицы является округленное до целого значение этого расстояния.
        OCRPointFloat & mTab_in0=mTab[indexL];       // указание компилятору на замену mTab[indexL] на mTab_in0, для увеличения скорости.
        ///mTab_in0.x=x1=mData[index].x;      mTab_in0.y=y1=mData[index].y;
        mTab_in0.x=x1=x_1;      mTab_in0.y=y1=y_1;
        ///if (GrafT)cout<<"   indexL="<<indexL<<"  indexL_old="<<indexL_old<<"  index="<<index<<endl;
        
        // Слияние двух ячеек в одну с помощью линейной интерполяции. Примерно 5-10% точек на линию.
        if (indexL==indexL_old) { // если целая часть двух соседних ячеек одинакова.
            // Заполнение одинаковых (по целой части) ячеек таблицы, средним из координат этих ячеек.
            OCRPointFloat & mTab_in1=mTab[indexL_old];
            mTab_in1.x=(x1+x0)/2.0;      mTab_in1.y=(y1+y0)/2.0;
            if (GrafT)cout<<"- отработка усреднением похожих ячейк таблицы   indexL="<<indexL<<"  indexL_old="<<indexL_old<<"  index="<<index<<endl;
        } // if (indexL==indL_old)
        
        // Заполнение пустой ячейки, средним из координат соседних ячеек. Примерно 5-10% точек на линию.
        if (indexL>indexL_old+1) { // если индекс двух соседних ячеек отличается больше чем на единицу.
            // Заполнение пустой ячейки таблицы средним из координат соседних точек линии.
            OCRPointFloat & mTab_in2=mTab[indexL_old+1];
            mTab_in2.x=(x1+x0)/2.0;      mTab_in2.y=(y1+y0)/2.0;
            if (GrafT)cout<<"+ отработка пустой ячейки таблицы   indexL="<<indexL<<"  indexL_old="<<indexL_old<<"  index="<<index<<endl;
        } // if (indexL>indL_old+1)
            
        
        ///if (GrafT)cout<<"   lineL="<<lineL<<"  lineL_old="<<lineL_old<<"  index="<<index<<endl;
        // основная линейная интерполяция, расставляющая точки в таблице вдоль длины фокальной линии,
        // на одинаковом расстоянии в один pix.
        k=(indexL-lineL_old)/(lineL-lineL_old);
        mTab_in0.x=x0+k*(x1-x0);    mTab_in0.y=y0+k*(y1-y0);
        
        // запоминаем предидущие значения переменных, что бы не лазить в массивы лишний раз.
        x0=x1;              // сдвиг влево двухрехразрядного регистра "x0, x1" по "х"
        y0=y1;              // сдвиг влево двухрехразрядного регистра "y0, y1" по "y"
        lineL_old=lineL;    // сохранение предидущего значения полного ключа float.
        indexL_old=indexL;  // сохранение предидущего целочисленного (округленный до целого) значения ключа int.
        
        if(GrafT)cout<<"index="<<index<<"  lineL="<<lineL<<"   indexL="<<(int)indexL<<"  x1="<<x1<<"  y1="<<y1<<endl;

    } // for ( index
    */
        
    //---------------

        
        // прямоугольная весовая функция
        // y(n)= y(n-1)+x(n)-x(n-2k-1)
        // треугольная весовая функция
        // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+x(n)
        /**/
        
    }//____________________________________________________________________________





    //*****************************************************************************
    
                                /// AngleM ///
    // Функция для подсчета реальных углов и длины сглаженной фокальной линии.
    
    void GGraphBezier::focalLineAngleM(OCRFocalLineBezier &line){
        
        // Подсчитываются реальные углы и длина по сглаженной фокальной линии LengthLineM.
        // Функционально это расширенная focalLineLineM.
        
        
        
        int index;
        float x0,y0,x1,y1,x2,y2;  // Декартовы координаты начала и конца линии.
        float dx,dy;
        
        // a,b,c стороны треугольника, "alpha" угол напротив стороны "a". Точка угола сканирует фокальную линию.
        float a, aa;   // расстояние между началом и концом угла и его квадрат.
        float b, bb;   // расстояние между началом и вершиной угла и его квадрат.
        float c=0, cc;   // расстояние между концом и вершиной угла и его квадрат.
        //float a_bc;    // отношение расстояния между концами угла и суммы длинны лучей угла и его копия.
        float alpha_bc;  // текущий угол в градусах вычесленный по приблизительной формуле alpha_bc=57.3*2*a/(b+c);
        float alpha_M0=0;  // средний угол в градусах вычесленный по всей кривой.
        float alpha_M1, alpha_M2;
        //float alpha_M1=0;  // средний угол в градусах вычесленный по ее половинкам.
        float rg_x0, rg_x1, rg_y0, rg_y1; // переменные двухразрядного регистров  "х" и "у"
        float rgAlp0, rgAlp1;
        
        float LengthLineM=0;    // Реальная длина сглаженной фокальной линии.
        
        
        //TIME_START
        
        // Длинна фокальной линии состоящая pix (количество промежуточных точек вдоль фокальной линии).
        int LengthLine=line.lenA;
        ///int LengthLine=(uint)line.data.size();
        ///if(!line.mData.size())return;
        
        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }  /// TIME=0.015 сек
        if(line.index==1446){ // 1132 4506 3100 не точная линия, 3187 (11pix)   4017 (14pix) точная линия,   4248 дуга,  3996 2706 1849 гигу.
            GrafT=0;
        }else GrafT=0;
        
        if (GrafT) {
            cout<<endl<<"Функция для подсчета реальных углов  и длины сглаженной фокальной линии   focalLineAngleM() "<<endl;
            cout<<"Длина фокальной линии  LengthLine="<<LengthLine<<endl;
        }
        
        // получаем прямой массив сглаженных координат точек фокальной линии (/16).
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];
        
        
        // Нахождение углов при движении по координатам сглаженной фокальной линии и ее длинны.
        
        LengthLineM=0;
        rg_x1=mData[0].x;  rg_x0=mData[1].x;  rg_y1=mData[0].y;  rg_y0=mData[1].y;
        rgAlp0=rgAlp1=0;   alpha_M0=alpha_M1=alpha_M2=0; //прописать реальные начальные условия
        
        for ( index=2; index<LengthLine; index++ ) {
            //for ( index=1; index<LengthLine-1; index++ ) {
            
            //x0=line.mData[index].x;        y0=line.mData[index].y;
            //x1=line.mData[index-1].x;      y1=line.mData[index-1].y;
            //x2=line.mData[index+1].x;      y2=line.mData[index+1].y;
            x2=mData[index].x;        y2=mData[index].y;
            x0=rg_x0;                       y0=rg_y0;
            x1=rg_x1;                       y1=rg_y1;
            
            
            // Вычисление кратчайшего расстояния между началом и концом угла по Теореме Пифагора.
            //dx=x2-x1;  dy=y2-y1;
            dx=x2-(x0+x0-x1);    dy=y2-(y0+y0-y1);
            aa=dx*dx+dy*dy;
            a=sqrt(aa);
            
            // Вычисление кратчайшего расстояния между началом и вершиной угла.
            dx=x1-x0;  dy=y1-y0;
            bb=dx*dx+dy*dy;
            b=sqrt(bb);
            //if (dx>23 || dy>23) { cout<<"dx="<<dx<<"     dy="<<dy<<endl;}
            
            // Вычисление кратчайшего расстояния между концом и вершиной угла.
            dx=x2-x0;  dy=y2-y0;
            cc=dx*dx+dy*dy;
            c=sqrt(cc);
            
            rg_x1=rg_x0;   rg_x0=x2;   // сдвиг влево двухразрядного регистра "rg_x1, rg_x0"  по "х"
            rg_y1=rg_y0;   rg_y0=y2;   // сдвиг влево двухразрядного регистра "rg_x1, rg_x0"  по "y"
            
            LengthLineM+=b; // Длинна сглаженной фокальной линии умноженной на 16.
            
            // для малых изменений угол в радианах это примерно отношение расстояния между концами угла и полусуммой длинны лучей угла.
            alpha_bc=114.59494*a/(b+c); // 57.29747*2=114.59494
            // точное вычисление угола по теорема косинусов.
            //￼￼￼ a*a=b*b + c*c - 2bc*cosA // A угол между bc.
            // alpha=arccos((b*b + c*c - a*a)/2bc);
            // alpha=57.29747*acos((bb + cc - aa)/(2*b*c));
            
            alpha_M0+=alpha_bc; // накопление текущего угола в градусах.
            
            alpha_M1+=alpha_bc - rgAlp1;  // a1-a0
            alpha_M2+=rgAlp0 - rgAlp1 - rgAlp1 + alpha_bc;  // a0-2*a1+a2 //
            rgAlp0=rgAlp1;   rgAlp1=alpha_bc;   // сдвиг влево двухразрядного регистра "rgAlp0, rgAlp1" alpha_bc
            
            ///if(GrafT)cout<<"x0="<<x0<<"     y0="<<y0<<"     alpha_bc="<<alpha_bc<<"     alpha_M1="<<alpha_M1<<endl;
            
        } // for ( index
        
        
        LengthLineM+=c; // добавляем к отрезкам "b+b+b+b+b+b+b+b+b..."самый последний отрезок "+c"
        
        // средний угол в градусах вычесленный по всей кривой.
        alpha_M0/=(LengthLine);////////////LengthLineM СГЛАЖЕННАЯ
        
        if (GrafT) { cout<<"alpha_M0="<<alpha_M0<<"     LengthLineM="<<LengthLineM<<" LengthLine="<<LengthLine<<endl;}
        /**/
        
        //TIME_PRINT_
        
        
        /**/
        //-----------------------------------------------------------------------------------------------------------
        
        //// a0-a1
        // a0,a1,a2,a3,a4,a5
        // a1-a0,a2-a1,a3-a2,a4-a3,a5-a4
        //// a0-2*a1+a2
        
    }//____________________________________________________________________________
  
    
  
    //*****************************************************************************
    
    
                /// Интерполяция крайних pix укороченной кривой Безье ///
    // Функция для сглаживания (интерполяции) несколько крайних pix фокальной линии. В разработке.
    void GGraphBezier::focalLineEnds(OCRFocalLineBezier &line, int strL, int endL){
    
    // strL и endL  отступ от начала и конца фокальной линии в pix.
    
    // Самыми не устойчивами (зашумленными) точками фокальной линии являются несколько крайних pix фокальной линии
    // около особых точек. Имеет смысл отбросить эти не устойчивые pix и исходя из общей формы кривой заменить
    // их на интерполированные по укороченной кривой Безье.
    // Для этого фокальная линия берется с отступом от краёв на несколько pix (strL, endL) и с помощью функции
    // focalLineBezierP (она умеет считать не учитывая края фокальной линии), вычисляются все необходимые параметры кривой Безье.
    // По направлениям крайних управляющих векторов кривой Безье, осуществляется линейная интерполяция краев фокальной линии.
    // По интерполированным краям фокальной линии с помощью функции focalLineBezierF вычисляются все необходимые параметры
    // новой кривой Безье..

        int GrafT=0;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if(line.index==-1) { //  || line.index==5095)
            GrafT=1;
        }else GrafT=0;
        
        if (GrafT) {
            cout<<endl<<"----------------------------------------------------------------------------"<<endl;
            cout<<endl<<"Функция для сглаживания (интерполяции) несколько крайних pix фокальной линии.   focalLineEnds() "<<endl;
            cout<<"index="<<line.index<<endl;
        }
        
//cout<<"index="<<line.index<<endl;
        
        float k;                // коэффициент линейной интерполяции.
        //strL=endL=4;            // отступ от краёв фокальной линии, pix.
        float LengthLineM=line.lenM;  // длина сглаженной фокальной линии.
        if(GrafT)cout<<"LengthLine="<<line.lenA<<"  LengthLineM="<<LengthLineM<<"  strL="<<strL<<"  endL="<<endL<<endl;
        
        if(GrafT)cout<<"+Px0="<<line.p0.x<<"  Py0="<<line.p0.y<<"  Px3="<<line.p3.x<<"  Py3="<<line.p3.y<<endl;
        
        int str_L=strL;      int end_L=LengthLineM-endL;
        // Вызов функции вычисления параметров кривой Безье по части фокальной линии.
        focalLineBezierP(line, str_L, end_L); // 0,0 по полной длине.
    
        // Получение координат управляющих точек кривой Безье.
        //float Px0=line.start.x;     float Py0=line.start.y;     // координаты “x,y” начала фокальной линии.
        float Px0=line.p0.x;      float Py0=line.p0.y;      // координаты “x,y” начала фокальной линии.
        float Px1=line.p1.x;        float Py1=line.p1.y;      // координаты “x,y” первой управляющей точки.
        float Px2=line.p2.x;        float Py2=line.p2.y;      // координаты “x,y” второй управляющей точки.
        float Px3=line.p3.x;      float Py3=line.p3.y;      // координаты “x,y” конца фокальной линии.
        //float Px3=line.end.x;       float Py3=line.end.y;       // координаты “x,y” конца фокальной линии.
        float lenP1=line.lenP1;   // длина первого контрольного вектора в pix. Проведен от точки P0 до точки P1.
        float lenP2=line.lenP2;   // длина второго контрольного вектора в pix. Проведен от точки P2 до точки P3.
        if(GrafT)cout<<"++Px0="<<Px0<<"  Py0="<<Py0<<"  Px3="<<Px3<<"  Py3="<<Py3<<endl;
    
        // линейная интерполяция конца кривой Безье. //
        k=endL/lenP2;
        if(GrafT)cout<<"    k="<<k<<endl;
        //Px3=Px3 + end_x;            Py3=Py3 + end_y;
        //Px3=Px3 + k*endL;           Py3=Py3 + k*endL;
        // Координаты интерполированного конца кривой Безье. 
        Px3=Px3 + k*(Px3-Px2);      Py3=Py3 + k*(Py3-Py2);
    
        // линейная интерполяция начала кривой Безье. //
        k=strL/lenP1;
        if(GrafT)cout<<"    k="<<k<<endl;
        // Координаты интерполированного начала кривой Безье.
        Px0=Px0 + k*(Px0-Px1);      Py0=Py0 + k*(Py0-Py1);
        if(GrafT)cout<<"+++Px0="<<Px0<<"  Py0="<<Py0<<"  Px3="<<Px3<<"  Py3="<<Py3<<endl;
        
 //       if(line.index==1726){ // <1727
        // Сохраняем координаты управляющих точек кривой Безье.
        //line.start.x=Px0;   line.start.y=Py0;   // координаты “x,y” начала фокальной линии.
        line.p0.x=Px0;      line.p0.y=Py0;      // координаты “x,y” начала фокальной линии.
        line.p3.x=Px3;      line.p3.y=Py3;      // координаты “x,y” конца фокальной линии.
        //line.end.x=Px3;     line.end.y=Py3;     // координаты “x,y” конца фокальной линии.
        
        /**/
        ///if(GrafT)cout<<"x2="<<x2<<"  y2="<<y2<<endl;
    
        // Вызов функции вычисления параметров кривой Безье по полной длине фокальной линии.
        ////focalLineBezierF(line);
//        }
        

        /**/
    
        
        // int strL - длина начального участока фокальной линии в pix, не участвующего в вычислении параметров кривой Безье.
        // int endL - длина конечного участока фокальной линии в pix, не участвующего в вычислении параметров кривой Безье.
    
        // Для замены краев фокальной линии истользуютя растеризованные концы кривой Безье у которой точки
        // при t=L*1/3, t=L*2/3 немного сдвинуты к краям.
    
        // Заново подсчитать кривую Безье по полной длинне фокальной линии. Сравнить с исходными длинными и принять решение
        // какую кривую Безье оставить.
    
    
    }//____________________________________________________________________________
    
    
    
    
    

/*
 // Функция для растискивания массива.
 void dotGainArray(OCRFocalLineBezier &line, uint K_Mid2);
 
/////////////////////////////////////////////////////////////////////////////////////

                                /// РАСТИСКИВАНИЕ /// не понадобилась
    // Функция для растискивания значений длинн строк в массиве счетчиков. Подробное описание в FocalLineMatrix.h
    void GGraphBezier::dotGainArray(OCRFocalLineBezier &line, uint K_Mid2){
    
        // Подсчитывается скользящея сумма по массиву и сохраняется.
        // Ширина скользящей суммы uint  2*K_Mid2  pix.
        // Для применения функции необходимо предварительно подсчитать массив интегральных сумм.
        // Массив может состоять из из любых чисел.
    
        int index;      // Переменная цикла по количеству точек массива.
        int k;          // копия K_Mid2
        // длина массива.
        uint LengthLine=line.sData.size();
        //cout<<"LengthLine="<<LengthLine<<"     K_Mid2="<<K_Mid2<<endl;
    
        int GrafT=0 ;  // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
        if (GrafT) {
            cout<<endl<<"Функция для сглаживания фокальной линии   dotGain() "<<endl;
            cout<<"Половина величины (ширины) усреднения pix  K_Mid2="<<K_Mid2<<endl;
        }
    
        // ограничение величины (ширины) скользящей суммы pix.
        if (K_Mid2<1) K_Mid2=1;    if (K_Mid2>16) K_Mid2=16;
        ///cout<<"K_Mid2="<<k<<"    K_Mid2="<<endl;
        // резервируем размер
        line.mData.resize(LengthLine);
    
        // получаем прямые массивы.
        OCRPointFloat *data=(OCRPointFloat*)&line.data[0];      // исходный массив.
        OCRPointFloat *sData=(OCRPointFloat*)&line.sData[0];    // массив интегральных сумм.
        OCRPointFloat *mData=(OCRPointFloat*)&line.mData[0];    // готовый растиснутый массив.
    
        // Пример создания массива интегральных сумм.//
        //-----------------------
        
         // Подсчет массива интегральных сумм.
         ///int sum=0;
         ///for (int index=0; index < LengthLine; index++){
         ///sum+=data[index].x; // Вычисление интегральных сумм.
         ///sData[index]=sum;   // Заполнение массива интегральных сумм.
         ///}
         
        //-----------------------
    
        mData[0]=data[0]; // добавляем первую точку (оператор).
    
        // Вычисление скользящей суммы по массиву.
        int LengthLine_=LengthLine-1;
        for ( index=1; index<LengthLine_; index++ ) {
            k=K_Mid2;
        
            // ограничение на величину скользящей суммы в начале массива.
            if (index<K_Mid2) k=index;
            // ограничение на величину скользящей суммы в конце массива.
            if ((LengthLine-index)<=K_Mid2) k=LengthLine-index-1;
            // востановление ограничения на величину скользящей суммы в начале массива,
            // необходимо если длина массива LengthLine меньше полной величины усреднения 2*K_Mid2.
            if (index<k) k=index;
        
            // Вычисление скользящей суммы текущей точки с помощью массива интегральных сумм.
            mData[index].x=sData[index+k].x - sData[index-k].x;  // /(k+k) для скользящего среднего
            ///    cout<<"index="<<index<<"     k="<<k<<"     x="<<data[index].x<<endl;
        
        } // for
    
        mData[LengthLine-1]=data[LengthLine-1]; // добавляем последнею точку (оператор).
    
        // Получили растиснутый массив.
        
    
    }//__________________________________________________________________________
*/

    
}; //____________________________________________________________________________




