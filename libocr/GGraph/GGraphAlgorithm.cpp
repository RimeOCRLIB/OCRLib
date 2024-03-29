//
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "../OCRString/php2stl.h"
#include "GGraph.h"

namespace ocr
{
    
/** @bref Функция создает таблицы хеша для всех типов DStar и OCRStar.  */
void GGraph::buildHashTable() {
    /**
     Для поиска DStar эталона в тексте используем значение хеша каждого DStar.
     Для расчета значения хеша используем кодировки типов двух центральных звездочек и
     кодировки типов звездочек расположенных на лучах их созвездий. Для кодировки DStar
     состоящего из двух созвездий, достаточно 12 бит. На практике используется short. Всего
     существует 801 вид DStar для всех видов узора. Такой хеш является кодом типа DStar и
     использование его для поиска позволяет использовать хеш таблицу без пропусков и повторов.
     
     Алгоритм вычисления хеша:
     Хеш вычисляется как последовательное вычисление двух шести разрядных хешей двух
     звездочек, объединение этих хешей в двенадцатиразрядное слово и получение порядкового
     номера типа DStar по заранее вычисленной таблице массива 30000 байт.
     
     Индекс хеша звездочки формируется как набор 12 битов поискогового регистра.
     Первые три бита это код центральной звездочки.
     Далее для каждого луча звездочки начиная с первого после луча который соединяет две
     звездочки DStar против часовой стрелки считываем коды типов звездочек концов лучей. Коды
     записываем как пары бит поискогового регистра. Нулевое значение пары заполняет оставшиеся
     биты поискогового регистра в том случае если звездочка имеет меньше чем четыре луча. По
     индексу поискогового регистра из заранее вычисленной таблицы получаем порядковай номер
     хеша звездочки. Порядок сборки поискового регистра Обнуляем значение регистра short.
     Записываем в первый байт код центральной звездочки
     X_POINT - 4 (100 в двоичной системе)
     T_POINT - 3 (011 в двоичной системе)
     P_POINT - 2 (010 в двоичной системе)
     L_POINT - 0 (010 в двоичной системе)
     прибавляем к поисковому регистру код звездочки первого луча
     сдвинутый на 3 бита в (умноженный на 8)
     прибавляем к поисковому регистру код следующей звездочки
     сдвинутый на 6 бит в (умноженный на 64)
     прибавляем к поисковому регистру код следующей звездочки
     сдвинутый на 9 бит в (умноженный на 512)
     прибавляем к поисковому регистру код следующей звездочки
     сдвинутый на 12 бит в (умноженный на 4096)
     Если центральная звездочка не имеет соответствующего луча
     прекращаем заполнение регистра.
     
     Два двенадцати разрядных хеша по таблице переводятся в два шести разрядных слова
     DStar при объединении в двенадцати разрядное слово дают 801 значений из 32000 возможных.
     
     По индексу двенадцатиразрядного хеша получаем порядковый номер типа DStar из заранее
     вычисленной таблицы массива 30000 байт.
     
     Вычисление таблицы компактного хеша звездочки выполняется
     путем вычисляния значение поискового регистра для каждого
     луча звездочки как стартового луча обхода лучей звездочки.
     Наибольшее значение поискового
     регистра используется для получения компактного хеша звездочки из таблицы.
     Всего 108 видов упорядоченных звездочек.
     
     */
    
    if(hashTableReady) return;
    
    char name[4]={'4', '3', '2', '1'};
    char S1;
    // char S2='0';
    char           s1[5];
    vector<string> hash;
    string         str;
    memset(s1, 0, 5);
    int   max=0;
    short hashIndex=1;
    int   size;
    //int print=1;
    
    //создание списка всех комбинаций типа центральной звездочки и типов звездочек на концах
    //лучей
    
    for(int n6=0; n6 < 4; n6++) {
        memset(s1, 48, 4);
        
        S1=name[n6];  //тип коренной звездочки
        int type1=S1;
        
        for(int n7=0; n7 < 4; n7++) {
            s1[0]=name[n7];
            if(type1=='1') {
                str=S1;
                str +=s1;
                hash.push_back(str);
                continue;
            }
            for(int n8=0; n8 < 4; n8++) {
                s1[1]=name[n8];
                if(type1=='2') {
                    str=S1;
                    str +=s1;
                    hash.push_back(str);
                    continue;
                }
                for(int n9=0; n9 < 4; n9++) {
                    s1[2]=name[n9];
                    if(type1=='3') {
                        str=S1;
                        str +=s1;
                        hash.push_back(str);
                        continue;
                    }
                    for(int n10=0; n10 < 3; n10++) {
                        s1[3]=name[n10];
                        str=S1;
                        str +=s1;
                        hash.push_back(str);
                    }
                }
            }
        }
    }
    
    //for(int n6=0; n6 < hash.size(); n6++) cout<<hash[n6]<<endl;
    
    //собираем хеш DStar из комбинации двух значений хеша звездочек
    size=(int)hash.size();
    hashTableStar=(uchar *)calloc(2, 20000);     //таблица неупорядоченного хеша OCRStar
                                                 //таблица упаковки хеша звездочки в DStar
    hashTableOCRStar=(ushort *)calloc(2, 20000); //таблица упорядоченного хеша OCRStar
    hashTable=(ushort *)calloc(2, 64000);         //таблица упорядоченного хеша DStar
    
    
    for(int n=0; n < size; n++) {
        // DR(hash[n]<<endl);
        //создаем таблицу упаковки неупорядоченного хеша звездочки
        char *p=&hash[n][0];
        short h=(*p - 48);
        p++;
        h +=(*p - 48) << 3;
        p++;
        h +=(*p - 48) << 6;
        p++;
        h +=(*p - 48) << 9;
        p++;
        h +=(*p - 48) << 12;
        if(h > max) max=h;
        hashTableStar[h]=n + 1;
    }
    // DR("max1:"<<max<<endl);
    max=0;
    
    
    //создаем таблицу комбинаций двух звездочек
    //первые лучи звездочек должны совпадать по типу
    //поскольку звездочки соединены фокальной линией
    for(int n=0; n < size; n++) {
        char c0=hash[n][0];  //тип первой звездочки DStar
        char c1=hash[n][1];  //тип второй звездочки DStar
        for(int m=0; m < size; m++) {
            //упаковываем первый хеш
            if(hash[m][1] !=c0 || hash[m][0] !=c1)
                continue;  //звездочки не сочетаются по типу звездочек на концах лучей
                           // DR(hash[n]<<"_"<<hash[m]<<"\n";
            char *p=&hash[n][0];
            short h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            p++;
            h +=(*p - 48) << 6;
            p++;
            h +=(*p - 48) << 9;
            p++;
            h +=(*p - 48) << 12;
            //получаем значение хеша первой звездочки
            short h1=hashTableStar[h];
            //DR(" "<<h1<<endl);
            //упаковываем второй хеш
            p=&hash[m][0];
            h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            p++;
            h +=(*p - 48) <<6;
            p++;
            h +=(*p - 48) << 9;
            p++;
            h +=(*p - 48) << 12;
            //получаем значение хеша второй звездочки
            short h2=hashTableStar[h];
            //DR(" "<<h2);
            //упаковываем хеш двух звездочек
            ushort h3=h2 << 12;
            h3 += h1;
            if(h3 > max) max=h3;
            //хеш прямого и обратного порядка звездочек имеет один и тот же hashIndex
            //поэтому мы увеличиваем hashIndex только в том случае
            //если такое сочетание звездочек уникально для прямого и обратного их расположения
            // hashTable[h3]=hashIndex;
            // hashIndex++;
            
            //вычисляем адрес хеша обратного порядка звездочек
            //заносим такое же значение hashIndex
            ushort h4=h1 << 12;
            h4 += h2;
            if(h4 > max) max=h4;
            if(hashTable[h3]==0 && hashTable[h4]==0) {
                hashIndex++;
                //cout<<h3<<"/"<<h4<<" ";
                            //continue;
                hashTable[h3]=hashIndex;
                hashTable[h4]=hashIndex;
                // DR("+"<<hashIndex<<endl);
            }
            
        }
    }

    // DR("max="<<max<<" hashIndex:"<<hashIndex<<endl);
    
    /**
     Вычисление таблицы упорядоченного компактного хеша OCRStar.
     Значение поискового регистра звездочки зависит от того какой луч выбирается как стартовый
     для обхода лучей звездочки. Поэтому вычисляется значение поискового регистра для каждого
     луча звездочки как стартового луча обхода лучей звездочки. Наибольшее значение поискового
     регистра используется для получения компактного хеша звездочки из таблицы.
     */
    
    //вычисление таблицы упаковки хеша
    size=(int)hash.size();
    hashIndex=1;
    max=0;
    // DR("2 hash___"<<endl);
    
    
    for(int n=0; n < size; n++) {
        str=hash[n];
        // DR(str<<endl);
        //читаем тип первой звездочки
        if(str[0]=='1') {
            //вычисляем поисковый регистр
            char *p=&hash[n][0];
            short h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            //записываем в таблицу
            hashTableOCRStar[h]=hashIndex;
            hashIndex++;
            if(h > max) max=h;
            // DR("+"<<str<<endl);
            //cout<<hash[n]<<endl;
        }
        if(str[0]=='2') {
            //вычисляем поисковый регистр
            char *p=&hash[n][0];
            short h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            p++;
            h +=(*p - 48) << 6;
            p++;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h]) continue;
            //проверяем второй вариант обхода лучей
            p=&hash[n][0];
            short h1=(*p - 48);
            h1 +=(*(p + 2) - 48) << 3;
            h1 +=(*(p + 1) - 48) << 6;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h1]) continue;
            
            //значение хеша еще не записано в таблицу
            //записываем значение hashIndex по всем трем адресам
            
            hashTableOCRStar[h]=hashIndex;
            hashTableOCRStar[h1]=hashIndex;
            hashIndex++;
            if(h > max) max=h;
            if(h1 > max) max=h1;
            // DR("+"<<str<<endl);
            //cout<<hash[n]<<endl;
        }
        if(str[0]=='3') {
            //вычисляем поисковый регистр
            char *p=&hash[n][0];
            short h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            p++;
            h +=(*p - 48) << 6;
            p++;
            h +=(*p - 48) << 9;
            p++;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h]) continue;
            //проверяем второй вариант обхода лучей
            p=&hash[n][0];
            short h1=(*p - 48);
            h1 +=(*(p + 2) - 48) << 3;
            h1 +=(*(p + 3) - 48) << 6;
            h1 +=(*(p + 1) - 48) << 9;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h1]) continue;
            //проверяем третий вариант обхода лучей
            p=&hash[n][0];
            short h2=(*p - 48);
            h2 +=(*(p + 3) - 48) << 3;
            h2 +=(*(p + 1) - 48) << 6;
            h2 +=(*(p + 2) - 48) << 9;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h2]) continue;
            
            //значение хеша еще не записано в таблицу
            //записываем значение hashIndex по всем трем адресам
            
            hashTableOCRStar[h]=hashIndex;
            hashTableOCRStar[h1]=hashIndex;
            hashTableOCRStar[h2]=hashIndex;
            hashIndex++;
            if(h > max) max=h;
            if(h1 > max) max=h1;
            if(h2 > max) max=h2;
            // DR("+"<<str<<endl);
            //cout<<hash[n]<<endl;
        }
        
        if(str[0]=='4') {
            //вычисляем поисковый регистр
            char *p=&hash[n][0];
            short h=(*p - 48);
            p++;
            h +=(*p - 48) << 3;
            p++;
            h +=(*p - 48) << 6;
            p++;
            h +=(*p - 48) << 9;
            p++;
            h +=(*p - 48) << 12;
            p++;
            //проверяем есть ли такое значение в таблице
            
            if(hashTableOCRStar[h]) continue;
            //проверяем второй вариант обхода лучей
            p=&hash[n][0];
            short h1=(*p - 48);
            h1 +=(*(p + 2) - 48) << 3;
            h1 +=(*(p + 3) - 48) << 6;
            h1 +=(*(p + 4) - 48) << 9;
            h1 +=(*(p + 1) - 48) << 12;
            
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h1]) continue;
            //проверяем третий вариант обхода лучей
            p=&hash[n][0];
            short h2=(*p - 48);
            h2 +=(*(p + 3) - 48) << 3;   //42323  42323
            h2 +=(*(p + 4) - 48) << 6;
            h2 +=(*(p + 1) - 48) << 9;
            h2 +=(*(p + 2) - 48) << 12;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h2]) continue;
            //проверяем четвертый вариант обхода лучей
            p=&hash[n][0];
            short h3=(*p - 48);
            h3 +=(*(p + 4) - 48) << 3;
            h3 +=(*(p + 1) - 48) << 6;
            h3 +=(*(p + 2) - 48) << 9;
            h3 +=(*(p + 3) - 48) << 12;
            //проверяем есть ли такое значение в таблице
            if(hashTableOCRStar[h3]) continue;
            
            //значение хеша еще не записано в таблицу
            //записываем значение hashIndex по всем четырем адресам
            
            hashTableOCRStar[h]=hashIndex;
            hashTableOCRStar[h1]=hashIndex;
            hashTableOCRStar[h2]=hashIndex;
            hashTableOCRStar[h3]=hashIndex;
            hashIndex++;
            if(h > max) max=h;
            if(h1 > max) max=h1;
            if(h2 > max) max=h2;
            if(h3 > max) max=h3;
            
            // DR("+"<<str<<endl);
            //cout<<hash[n]<<endl;
        }
    }
    hashTableReady=1;
    // cout<<"max="<<max<<" hashIndex:"<<hashIndex<<endl);
    
}  //________________________________________________________________

/** @bref  проверка угла образующих отрезков двух кривых Безъе которые проведеных
 к точке екстремума фокальной линии.
 Такой угол равен также углу между двумя касательными к двум кривым Безъе
 в этой точке */
    
bool GGraph::focalLineExtremLimit(OCRLine &line, int index0, int index1){

    int GrafT=1;
    // 1 - вывод отладочной информации (цифр, таблиц, комментариев и графиков)  if (GrafT) {  }
    if (GrafT) { DG("Функция выделения устойчивых экстремумов (перегибов) фокальной линии по углам"<<endl); }
    
    //*
    int x0=0, y0=0;     // координаты точки экстремума для которой будет вычисятся угол. Известны.
    int x1=10, y1=0;    // координаты концов хендлера (handler) первой кривой Безье. Подсчитаны по формуле.
    int x2=20, y2=20;   // координаты концов хендлера (handler) второй кривой Безье. Подсчитаны по формуле.
    if (GrafT) { cout<<"x0="<<x0<<" y0="<<y0<<"     x1="<<x1<<" y1="<<y1<<"     x2="<<x2<<" y2="<<y2<<endl; }
    
    int dx1=x1-x0;  int dy1=y1-y0;
    int dx2=x2-x0;  int dy2=y2-y0;
    
    // скалярное произведение векторов:
    float scalar = dx1*dx2 + dy1*dy2;
    
    // Вычисление модулей векторов (длинн хендлеров) по Теореме Пифагора (м.б. уже вычисленно).
    float mod1=sqrt(float(dx1*dx1 + dy1*dy1));
    float mod2=sqrt(float(dx2*dx2 + dy2*dy2));
    
    // cosAlpha в компактном виде
    //float cosAlpha = (dx1*dx2 + dy1*dy2)/(sqrt(float(dx1*dx1 + dy1*dy1)) * sqrt(float(dx2*dx2 + dy2*dy2)));
    
    float cosAlpha=scalar/(mod1*mod2);   // -1<=cosAlpha<=1      // cos(α)
    float Alpha=57.29747*acos(cosAlpha);    // -pi<=Alpha<=pi       // -pi<=cos(arccos(x))<=pi при -1<=x<=1
    if (GrafT) { cout<<"cosAlpha="<<cosAlpha<<"   Alpha="<<Alpha<<endl; }
    
    
    // Вычисление лимита угла в градусах.
    float lim_Alpha=54; // назначаемый лимит угла в градусах -pi<=lim_Alpha<=pi.
    float lim_cosAlpha=cos(lim_Alpha/57.29747); // вычисленный лимит угла в значениях косинуса -1<=lim_cosAlpha<=1.
    
    if(cosAlpha > lim_cosAlpha ) {
        if (GrafT){ cout<<"Точка <P> типа, cosAlpha="<<cosAlpha<<"   lim_cosAlpha="<<lim_cosAlpha<<endl; }
        return 0;
    }
    // угол больше, а косинус угла меньше.
    return 1;

}
    
}  // namespace ocr
