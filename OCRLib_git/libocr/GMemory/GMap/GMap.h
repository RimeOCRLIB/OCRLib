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
//C- GNU General Public License for .more details.
//C-

//if ( ImBuf !=NULL ) farfree(ImBuf);  //  освобождение памяти
//q=(тип_q *)farmalloc(n_byte);     // запрос памяти без очистки 0
//q_new=farrealloc(q_old,n_byte);;  // изменение размера блока

#ifndef _GMap_H_
#define _GMap_H_

#include "config.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>



#include "OCRTypes.h"
#include "GVector.h"
#include "GLetter.h"
#include "pugixml.hpp"
#include "OCRTypes.h"
#include "php2stl.h"


using namespace std;
using namespace pugi;


//#define REMARK

namespace ocr{
    //Клас реализует сериализованное на диске хранилище ключ-значение
    //Поиск по ключу возвращает все значения, 
    //совпадающие с ключом по заданным параметрам
    //алгоритм поиска по кодированому наложением массиву 
    //на базе Марковских процессов
    //детально описан в файле associativeSignalProcessing.cpp
    
    //Наиболее эффективно искать в тексте все ключи одновременно
    //с выводом значений в упорядоченный по порядку ключей массив.
    //Применяется для поиска, разбора и перекодировок текстов.
    //при этом скорость разбора и поиска всех ключей одновременно
    //практически равна скорости поиска одного ключа при древовидном поиске
    //При поиске единичного ключа происходит перебор кодированного наложением словаря.
    //На практике около 1.1 гигабайт словаря при индексе в 700 мегабайт
    //значение по одному ключу выдает за секунду.
    //значения по 10 ключам за 1.003 секунды
    //для повышения производительности ключи нужно задавать в 
    //кодировках из одного или двух байтов
    //например кодировка Unicode little-Endian быстрее чем UTF8
    
    enum searchParam{
		FULL_MATCH=0,
		START_MATCH=1,
		ANY_MATCH=2};
	
    class  GMap{
    protected:
        GMap(string &path);
        GMap(string &path,GVector *data);
        void init(string &path);
        void init(string &path,GVector *data);
    public:
        void destroy(void);
        static GMap* create(string &path) {return new GMap(path);}
        static GMap* create(cstr path_) {string path=path_; return new GMap(path);}
        static GMap* create(string &path,GVector *data) {return new GMap(path,data);}
        
        void reloadPtr();	 	 	 	  ///восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
        uint size();
        ushort* dictData(){return dictionary_data;}
        ushort* BufUpData(){ return BufUp;}
        void clear();
        void freeTextVectors();
        void freeStackVectors();
        
        
        int testLetterP(ushort letter1, ushort letter2);
       
        //// ассоциативная обработка сигналов (стартовая программа)
        void associativeSignalProcessing(string&text);  
         
        //// функции создания словаря
        //наиболее быстрый способ это задать все ключи текстовым файлом
        //в формате ключ\tзначение
        
        //для ключей применяется функция перекодировки словаря 
        ///(плотной упаковки разреженного кодового пространства словарных букв).
        // Создает таблицы (массивы) для плотной паковки или распаковки кодов букв и пар букв.
        // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого текста используя таблицы (массивы) паковки словаря.
        // Данная функция выполняется однократно при создании объекта. 
       
        void addRecords(string &path,size_t mode);
        void addRecords(cstr path_,size_t mode){string path=path_; return addRecords(path, mode);}
        void addRecords(GVector *dataVector,int keyField,size_t mode);
        void addRecord(string &key);
        void addRecord(TString *st);
        
        //функции интерфейса доступа к данным GMap
        //get one record by key in mode
        uint getKey(string &key, int mode);
        //get vector of records by key in mode
        void getKey(string &key, vector<uint>&searchResult,int mode);
        //получение ключа по значению хеша
        uint getHKey(string & key,int keyField);
        //получение ключей по значению хеша
        uint getHKey(vector<uint>&searchResult,string & key,int keyField);
        
        void getOCRKey(string &key,vector<uint>&searchResult, int mode);
        void getOCRStackKey(string &key,vector<ushort>&letterX,map<vector<short>,int>&searchResult, int mode);
        
        //поиск по языковой модели
        void getAIML(string &key,vector<uint>&searchResult, int mode);
        
        //get unicode data of one record by index
        void getKeyStr(uint index,vector<ushort>&key);

        
        void readDataTXT(string &path);
        void readDataTAB(string &path,int keyField);
        void readDataVector(GVector *dataVector,int keyField);
        void readDataVectorHash(GVector *dataVector,int keyField);
        
        //void getTStr(string &key,TString *strT);
        
        void table_Dict();
        
        //// функция плотной упаковки разреженного кодового пространства текстовых букв
        // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого ключа используя таблицы (массивы) паковки словаря.
        void tableText(string&text);
        //// функция плотной упаковки разреженного кодового пространства текстовых букв
        // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого ключа используя таблицы (массивы) паковки словаря.
        //пары букв из текста идут подряд, без перекрытия и соблюдения порядка пар в тексте
        void tableTextStack(string&text);
        //// функция создания структуры для работы с реальными координатами пар букв в распознаваемом тексте
        void tableLetters();
        void tableLettersFontMatch();
        //// функция применения словаря к распознаваемому тексту
        // Базовая функция четкого или нечеткого поиска.
        void tableProcessing(int mode);
        // Базовая функция четкого поиска, возвращает все результаты
        void tableProcessingFullMatch(vector<uint>&searchResult);
        // Проверяется полное совпадение (четкое или нечеткое) слова с фразой словаря.
        int tableProcessingFullMatch(int mode);
        // Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
        int tableProcessingMatch(int mode);
        // Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
        void tableProcessingMatch(vector<uint>&searchResult, uint limit);
        // Проверяется совпадение (четкое или нечеткое) слова (стека) из текста, с фразой словаря.
        void tableProcessingOCRMatch(vector<uint>&searchResult,int mode);
        // Проверяется совпадение (четкое или нечеткое) последовательности из текста (ведра пар букв)
        // длинной более 8 символов, с фразой словаря.
        void tableProcessingStack(GStr<uint> *searchResult, int mode);
        // Проверяется совпадение (четкое или нечеткое) последовательности из текста (ведра пар букв)
        // длинной более 8 символов, с фразой словаря, с учетом реальной шрифтовой геометрии.
        void tableProcessingFontMatch(map<vector<short>,int>&searchResult,int mode);
        //Словарный разбор фразы текста
        void tableProcessingDict(int mode);
        //Словарный разбор запроса AIML
        void tableProcessingAIML(vector<uint>&searchResult,int mode);
        //// функция поиска совпавшего непрерывного фрагмента фразы словаря длинной больше constantPhrase1 с
        //// непрерывном фрагментом текста. Запускается в table_Processing()
        ushort lookupProcess3(uint w, uint oldw);
        ushort lookupProcessFontMatch3(map<vector<short>,int>&searchResult,
                                       unsigned int w,
                                       unsigned int oldw,
                                       unsigned int start,
                                       unsigned int nLine);
        
        ushort lookupProcess3_T(uint w, uint oldw);
        
        
        //// функция поиска наибольших не четких подвхождений фразы словаря т.е. оставляем самый крупный блок во фразе.
        //// Запускается в table_Processing()  
        bool lookupProcess4(uint n,uint oldw,uint w);
        
        // функция обработки результатов ассоциативного поиска 
        void renderResult(string &result);
        // функция обработки результатов ассоциативного поиска
        void renderResultStack();
        
        string p1letter(short code);   //декодирует компактный код первой буквы пары в строку
        string p2letter(short code);   //декодирует компактный код пары в пару букв в строке
        
        /// сохраняем глобальные переменные
        void save();
        
        GVector *valueData_vector;// массив значений словаря
        // флаг пропуска кодов символов пробела, точки в словаре и тексте
        size_t flagPSymbols;        // "1" символы пробела, точки в словаре и тексте удаляются, "0" все остается как есть
        GVector *hashData;        //двухмерный массив соответствия значения хеш-функции и индексов записей в векторе данных
        uint hashLimit;           //величина на которую делится полное значение хеша. Болшее значение делает хеш меньше но с большим колмчеством повторов

        uint hashError;
        uint maxHashError;
        uint hashCount;
        
        
    private:
        GVector *innerData;       //контейнер сохраняемых значений внутренних переменных и всех внутренних данных
        string dataPath;
        string selfPath;

        // кодировки символов словаря и проверяемого текста
        ushort codeEnter;         // исходная кодировка перевода каретки во всех словарях, обычно 10
        ushort codePoint;         // исходная кодировка точки во всех словарях, обычно 3851
        ushort codeSpace;         // исходная кодировка пробела во всех словарях, обычно 32

        ushort constantPhrase;    // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.(длинна ключа)
        uint delta;               // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)

        /// выходные переменные функции перекодировки букв словаря
        uint recordCount;      // количество записей
        uint dict_size1;       // размер словаря без переводов каретки Enter      
        ushort nLetter;        // количество разных букв в словаре
        uint nLetterP;         // количество разных пар букв в словаре
        uint nEnter;           // количество переводов каретки Enter в словаре
        uint nRank,rank;       // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank 
        
        GStr<uint>*vData_vector;   //массив внутренних переменных
        
        ushort  *dictionary_data; // массив словаря
        GStr<ushort>*dictionary_data_vector;
        uint dict_size;           // размер массива словаря
        uint BUFF_DICT_SIZE;      // количество пар букв в словаре. Вычисляется в момент создания словаря
    
        
        ushort  *text_data;       // массив распознаваемого текста
        GStr<ushort>*text_data_vector;
        uint text_size;           // размер массива распознаваемого текста
        GStr<ushort>*resultTextVector;               // строка результатов вычисления вероятности букв в фразе
        ushort* resultText;
        GStr<uint>*resultDelimeterVector;   // строка результатов вычисления вероятности разделителей в фразе
        uint *resultDelimeter;
        
        /// массивы функции перекодировки букв словаря 
        ushort *BufM;          // массив предназначен для получения упакованного кода из исходного кода буквы.
        GStr<ushort>*BufM_vector;
        ushort *BufU;          // массив для восстановления исходного кода буквы (разреженного кода)
        GStr<ushort>*BufU_vector;
        // из упакованного кода буквы
        uint *BufE;            // массив (int) для хранения адресов перевода каретки Enter словаря
        GStr<uint>*BufE_vector;
        uint size_BufE;
        /// массивы функции перекодировки пар букв словаря
        ushort *BufMp;         // массив BufMp упакованного кода пар букв 
        GStr<ushort>*BufMp_vector;
        uint size_BufMp;
        ushort *BufMpT;        // массив BufMpT пар букв текста. То же, что и BuffMp для словаря. 
        GStr<ushort>*BufMpT_vector;
        // Предназначен для получения упакованного кода пар букв из исходного кода буквы,  
        // адресом массива является исходный код буквы (разреженный код) - ответом
        // упакованный код пары буквы.
        uint size_BufMpT;
        
        ushort *BufUp;         // массив для восстановления исходного кода первой буквы пары 
        GStr<ushort>*BufUp_vector;
        ushort *BufUp2;         // массив для восстановления исходного кода второй буквы пары
        GStr<ushort>*BufUp2_vector;
        uchar  *BufD;         //массив адресов разделителей в строках словаря
        GStr<uchar>*BufD_vector;
        uint size_BufD;
        uint  *BufDR;         //массив адресов строк BuffD
        GStr<uint>*BufDR_vector;
        uint size_BufDR;
        // из упакованного (компактного) кода пар букв словаря 
        /**/
        
        /// массивы функции перекодировки пар текста
        //wstring strTextW;            //
        //string strText;              //
        uint *BufET;           // массив (int) для хранения адресов перевода каретки Enter текста
        GStr<uint>*BufET_vector;
        //ushort *BufMpT;      // зеркальный массив пар букв BufMpT текста
        ushort *BufUpT;        // массив для восстановления исходного кода первой буквы пары    
        GStr<ushort>*BufUpT_vector;
        // из упакованного (компактного) кода пар букв текста
        uchar *BufUpT1;        // тоже что и массив BufUpT но состоит из из char 0 или 255 и служит для вычисление Max длины 
        GStr<uchar>*BufUpT1_vector;
        // совпавшего фрагмента текста с непрерывном фрагментом фразы словаря.
        /// выходные переменные функции перекодировки пар текста
        uint text_size1;       // размер массива распознаваемого текста(ключей) без переводов каретки Enter

        uint size_BufET;
        uint size_BufUpT;
        uint size_BufUpT1;
        
        ushort  *BufLpT;
        GStr<ushort>*BufLpT_vector;
        uint size_BufLpT;
        
        uint nLetterPT;              
        uint clusters_size;    // размер массива цепочек адресов clusters_data
        
        int  *letters_data;       // Разреженный массив адресов пар букв встечающихся в распознаваемом тексте один раз (без учета одинаковых).
        GStr<int>*letters_data_vector;
        uint letters_size;
        // Адрес в массиве - компактный код пар букв, содержимое - позиция в распознаваемом тексте пары букв.     
        int  *clusters_data;      // Плотный массив цепочек адресов (позиций) ОДИНАКОВЫХ пар букв в распознаваемом тексте.
        GStr<int>*clusters_data_vector;
        // В тексте легко встечаются одинаковые пары букв.
        int  *clusters_copy;      // копия массива clusters_data, предназначена для восстановления счетчиков цепочек с помощью memcpy
        GStr<int>*clusters_copy_vector;
        
        uint size_BufTxt;      // размер массива распознаваемого текста (в байтах), в общем случае size_BufTxt=text_size;     
        ushort *BufTxt;        // восстановленная через словарь копия массива распознаваемого текста
        GStr<ushort>*BufTxt_vector;
        //  nev  //
        vector<short> BufDictStr;
        uint size_GravTxt;     // размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
        ushort *GravTxt;       // буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря.
        GStr<ushort>*GravTxt_vector;
        uint size_SumTxtP;     // полный размер статистической устойчивости распознаваемого текста, size_SumTxtP=text_size1*nLetterP; 
        ushort *SumTxtP;       // массив статистической устойчивости распознаваемого текста 
        // (массив частоты встречаемости пар букв)
        GStr<ushort>*SumTxtP_vector;
        ushort *letterX;      // массив координат пар букв в распознаваемом тексте
        
        int xMaxStart;                 // координаты начала нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
        int xMaxEnd;                   // координаты конца нечеткого кусочнонепрерывного фрагмента текста максимальной длинны
        int xMaxStartDict;             // координаты начала нечеткого кусочнонепрерывного фрагмента фразы словаря максимальной длинны
        int xMaxEndDict;               // координаты конца нечеткого кусочнонепрерывного фрагмента фразы словаря максимальной длинны
        
        void printDictEntry(uint n);
        void printInputEntry();
        
        //}	

        
    };
};
// ---------------- IMPLEMENTATION
# endif

/*
 ассоциативная обработка сигналов
     /// применение словаря к распознаваемому тексту ///  
    
    // поиск максимальных под вхождений фраз словаря в распознаваемом тексте     
    // задачи (зачем?):
    // - поиск максимальных под вхождений фраз словаря в распознаваемом тексте
    // - это д.б. не четкий поиск в аспекте расхождения соответствующих максимальных под вхождений фраз словаря с фразами 
    //   распознаваемоого текста на 1-2 символа в произвольном месте фразы (как "*" в поиске)  
    // - и одновременно это д.б. не четкий поиск в аспекте применения "молекулы" к распознаваемому тексту. Каждой букве распознаваемоого текста
    //   соответствует три наиболее подходящих по коэффициенту корреляции буквы, претендующие на данное место. Можно так-же использовать 
    //   подходящие буквы не только по коэффициенту корреляции, но и по грамматике (или по иному принципу).  
    //   (при задуманном подходе, с технической точки зрения, это просто эквивалентно увеличению длинны распознаваемоого текста в три раза) 
    // Замечания:
    // - если не искать под вхождения то минимальную длину под вхождения фразы словаря можно сделать равной самой длинне фразы словаря,
    //   что очень прилично улучшит быстродействие
    // - часто, для краткости вместо слов "пара букв" употребляется просто "буква"
    // - фраза "непрерывный фрагмент фразы словаря" в контексте не четкого поиска намекает на возможность применения очень быстрого 
    //   четкого поиска к этому фрагменту фразы 
    

 
 bool GMemory::table_Dict(vector<string>&dictionaryVector)
 
 Функция получает на вход 700мб файл словаряфраз (фактически в текстовом формате, буквы кодированны short, фразы отделены друг от друга переводами каретки) и записывается в массив dictionary_Dat. Полные требованиям к файлу смотри в функции associativeSignalProcessing ТРЕБОВАНИЯ К СЛОВАРЮ И ТЕКСТУ:
 Функция перекодировки словаря (плотной упаковки разреженного кодового пространства словарных букв):
 - создает таблицы (массивы) для плотной паковки или распаковки кодов букв и пар букв.
 - осуществляет запись на диск результатов перекодировки словаря, с использовании функция быстрого чтения больших файлов в память.
 Данная функция для данного словаря выполняется однократно с сохранением результатов на диск (HD). Время выполнения
 функции 8-10 сек
 После завершения работы функции, в том же массиве dictionary_Dat возвращается словарь, в котором уже пары буквы  кодированны short, но кодировка начинается с единици и заканчивается полным количеством пар букв в словаре. Плотно упаковывается разреженное кодовое пространство. Переводы каретки отсутстуют, адреса границ фраз лежат в массиве BuffE (массив int для хранения адресов перевода каретки Enter словаря). Пара букв это на самом деле чешуйка из пары букв с перекрытием в одну букву, поэтому число пар букв равно числу букв. Замена букв на пары букв фундаментально улучшает свойства текста, если разных букв в тексте несколько сотен, то пар букв несколько тысяч ( тиб 6000)
 
 
 unsigned short *dictionary_Dat;  // массив словаря
 unsigned int *BuffE;                 // массив (int) для хранения адресов перевода каретки Enter словаря
 
 /// массивы функции перекодировки букв словаря
 unsigned short BuffM[65536+32];  // зеркальный массив BufM 128k (фиксированного размера). Предназначен для получения   упакованного кода из исходного кода буквы, адресом массива является исходный код буквы (разреженный код) - ответом упакованный код букв. Внутри массива лежат коды плотно упакованных букв.
 
 unsigned short BuffU[65536+32];  // Массив восстановления исходного кода буквы (разреженного кода) из упакованного кода буквы. BuffU 128k (фиксированного размера). Адресом массива является упакованный код буквы - ответом исходный код буквы (разреженный код). По смыслу выполняет обратную задачу массива BufM. 
 
 // примеры прямой и обратной перекодировки букв словаря
 - упакованный код перевода каретки = BuffM[10] (исходный код перевода каретки 10, буквы и т.д. )
 - исходный код буквы = BuffU[dictionary_Dat[7] (упакованный код перевода каретки 7, буквы и т.д. )
 
 
 /// массивы функции перекодировки пар букв словаря
 По смыслу полностью соответствуют массивам BuffM и BuffU только вместо букв они кодируют - раскодируют пары букв
 unsigned short *BuffMp;              // зеркальный массив пар букв BufMp словаря. (переменного размера).  Предназначен для получения упакованного  кода пар букв из исходного кода буквы, адресом массива является исходный код буквы (разреженный код) ответом упакованный код буквы. Пары букв перекрываются как чешуйки т.е. считаются в перекрышку, а не в стык.
 unsigned short BuffUp[65536+32]; // массив для восстановления исходного кода первой буквы пары. 128k (фиксированного размера).
 
 // примеры прямой и обратной перекодировки пар букв словаря
 
 // вывод на экран упакованного словаря dictionary_Dat начертанием, как текст UTF-16 (short)
 s=0; 
 for(x=0; x < dict_sizeGr; x++){ 
 str=(wchar_t)BuffUp[dictionary_Dat[x]]; cout<<Unicode_to_UTF(str); //  cout<<Unicode_to_UTF(dictionary_Dat[x]);
 if ( x==BuffE[s] ) { cout<<endl; s++; }
 }    
 
 
 // массивы словаря разделителей (пробелов). 
 Это компактный способ хранения разделителей (пробелов).
 unsigned char *BuffD;  // массив словаря разделителей BuffD (delimeterVecor) т.е. массив строк адресов разделителей. char в строках указывают положение пробелов в исходном в массиве фраз словаря.
 unsigned int *BuffDR;  // массив индексов BuffDR (rowDelimeterVecor). Массив с адресами начала строк в словаре разделителей BuffD. Строки  соответствуют фразам в массиве словаря.  BuffDR - аналог массива перевода каретки словаря разделителей.
 
 
 
 
 ---------------------------------------------------------------------------------------------------------------------
 
 */



