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

#define ALPHABET_SIZE  65536 // 262144  524288count of all leters pairs in search text data. Must be power of 2
#define mSIZE ushort

typedef struct pageRecordLink{
    uint index;
    uint field;
    ushort offset;
}rLink;

typedef struct pageLink{
    uint index;
    ushort field;
}pLink;


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
    //наилучшая эффективность поиска при кодировании текста словами или слогами
    
    enum searchParam{
		FULL_MATCH=0,
		START_MATCH=1,
		ANY_MATCH=2
    };
	
    class  GMap{
    protected:
        GMap(string &path);
        GMap(string &path,GVector *data);
        GMap(string &path,GVector *dataText,GStr<mSIZE> *dataDict);
        void init(string &path);
        void init(string &path,GVector *data);
        void init(string &path,GVector *dataText,GStr<mSIZE> *dataDict);
        void initVariables();
    public:
        void destroy(void);
        static GMap* create(string &path) {return new GMap(path);}
        static GMap* create(cstr path_) {string path=path_; return new GMap(path);}
        static GMap* create(string &path,GVector *data) {return new GMap(path,data);}
        static GMap* create(string &path,GVector *dataText,GStr<mSIZE> *dataDict) {return new GMap(path,dataText,dataDict);}
        
        void reloadPtr();	 	 	 	  ///восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
        ulong size();
        mSIZE* dictData(){return dictionary_data;}
        mSIZE* BufUpData(){ return BufUp;}
        void clear();
        void freeTextVectors();
        void freeStackVectors();
        void close();  //flush all data on disk;
        
        int testLetterP(uint letter1, uint letter2);
       
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
        void addRecord(TString &st);
        void processKey(string &key, uint index,uint field);
        void processKeyWordlist(string &key, uint index,uint field);
        
        //функции интерфейса доступа к данным GMap
        //get one record by key in mode
        ulong getKey(string &key, int mode);
        //get vector of records by key in mode
        void getKey(string &key, vector<ulong>&searchResult,int mode);
        //получение ключа по значению хеша
        ulong getHKey(string & key,int keyField);
        ulong getHKey_(string & key,int keyField);
        //получение ключей по значению хеша
        ulong getHKey(vector<ulong>&searchResult,string & key,int keyField);
        //интерфейс вызова данных GMap для проверки результатов распознавания
        void getOCRKey(string &key,vector<ulong>&searchResult, int mode);
        //проверка результатов распознования с учетом геометрии
        void getOCRStackKey(string &key,vector<uint>&letterX,map<vector<int>,ulong>&searchResult, int mode);
        //поиск по языковой модели
        void getAIML(string &key,vector<ulong>&searchResult, int mode);
        //get unicode data of one record by index
        void getKeyStr(ulong index,vector<uint>&key);
        //get index and field in search data vector by words in query by wordlist encoding
        void getKeyID(string &key,vector<pLink>&searchResult);
        //Кодирование фразы текста ID номерами слов
        void encodeID(string &vData);

        
        void readDataTXT(string &path);
        void readDataTAB(string &path,int keyField);
        void readDataVector(GVector *dataVector);
        void readDataVector(GVector *dataVector,int keyField);
        void readDataVectorHash(GVector *dataVector,int keyField);
        void readDataVectorText(GVector *dataVector,int keyField);
        void readDataVectorWordlist(GVector *dataVector,int keyField);

        //void getTStr(string &key,TString *strT);
        
        void table_Dict();
        
        //// функция плотной упаковки разреженного кодового пространства текстовых букв
        // осуществляет плотную паковку кодов кодов букв и пар букв проверяемого ключа используя таблицы (массивы) паковки словаря.
        void tableText(string&text,uint mode);
        //// функция создания структуры для работы с реальными координатами пар букв в распознаваемом тексте
        void tableLetters();
        //// функция применения словаря к распознаваемому тексту
        // Базовая функция четкого или нечеткого поиска.
        void tableProcessing(int mode);
        // Базовая функция четкого поиска, возвращает все результаты
        void tableProcessingFullMatch(vector<ulong>&searchResult);
        // Проверяется полное совпадение (четкое или нечеткое) слова с фразой словаря.
        ulong tableProcessingFullMatch(int mode);
        // Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
        ulong tableProcessingMatch(int mode);
        // Проверяется наличие совпадения (четкого или нечеткого) слова с фрагментом фразы словаря.
        void tableProcessingMatch(vector<ulong>&searchResult, uint limit);
        // Проверяется совпадение (четкое или нечеткое) слова (стека) из текста, с фразой словаря.
        void tableProcessingOCRMatch(vector<ulong>&searchResult,int mode);
        // Проверяется совпадение (четкое или нечеткое) последовательности из текста (ведра пар букв)
        // длинной более 8 символов, с фразой словаря.
        void tableProcessingStack(GStr<ulong> *searchResult, int mode);
        // Проверяется совпадение (четкое или нечеткое) последовательности из текста (ведра пар букв)
        // длинной более 8 символов, с фразой словаря, с учетом реальной шрифтовой геометрии.
        void tableProcessingFontMatch(map<vector<int>,ulong>&searchResult,int mode);
        //Кодирование фразы текста обработаной в tableText() ID номерами слов
        void tableProcessingID(GStr<uint>*result);
        //Словарный разбор фразы текста
        void tableProcessingDict(int mode);
        //Словарный разбор запроса AIML
        void tableProcessingAIML(vector<ulong>&searchResult,int mode);
        //// функция поиска совпавшего непрерывного фрагмента фразы словаря длинной больше constantPhrase1 с
        //// непрерывном фрагментом текста. Запускается в table_Processing()
        uint lookupProcess3(ulong w, ulong oldw);
        uint lookupProcessFontMatch3(map<vector<int>,ulong>&searchResult,
                                       ulong w,
                                       ulong oldw,
                                       ulong start,
                                       ulong nLine);
        
        uint lookupProcess3_T(ulong w, ulong oldw);
        
        
        //// функция поиска наибольших не четких подвхождений фразы словаря т.е. оставляем самый крупный блок во фразе.
        //// Запускается в table_Processing()  
        bool lookupProcess4(ulong n,ulong oldw,ulong w);
        
        // функция обработки результатов ассоциативного поиска 
        void renderResult(string &result);
        // функция обработки результатов ассоциативного поиска
        void renderResultStack();
        
        string p1letter(mSIZE code);   //декодирует компактный код первой буквы пары в строку
        string p2letter(mSIZE code);   //декодирует компактный код пары букв в строку
        
        /// сохраняем глобальные переменные
        void save();
        
        GVector *valueData_vector;// массив значений словаря
        // флаг пропуска кодов символов пробела, точки в словаре и тексте
        size_t flagPSymbols;        // "1" символы пробела, точки в словаре и тексте удаляются, "0" все остается как есть
        GVector *hashData;        //двухмерный массив соответствия значения хеш-функции и индексов записей в векторе данных
        uint hashLimit;           //величина на которую делится полное значение хеша. Болшее значение делает хеш меньше но с большим колмчеством повторов

        uint hashError;
        uint maxHashError;
        ulong hashCount;
        ulong wordListCount;
        
        
    private:
        GVector *innerData;       //контейнер сохраняемых значений внутренних переменных и всех внутренних данных
        string dataPath;
        string selfPath;

        // кодировки символов словаря и проверяемого текста
        mSIZE codeEnter;         // исходная кодировка перевода каретки во всех словарях, обычно 10
        mSIZE codePoint;         // исходная кодировка точки во всех словарях, обычно 3851
        mSIZE codeSpace;         // исходная кодировка пробела во всех словарях, обычно 32

        uint constantPhrase;    // минимальная длина части фразы словаря, которую имеет смысл проверять как непрерывную.(длинна ключа)
        ulong delta;               // максимальное количество не четких букв в легитимной фразе словаря (dlt=0; четкий поиск)

        /// выходные переменные функции перекодировки букв словаря
        ulong recordCount;      // количество записей
        ulong dict_size1;       // размер словаря без переводов каретки Enter
        ulong nLetter;          // количество разных букв в словаре
        ulong nLetterP;         // количество разных пар букв в словаре
        ulong nEnter;           // количество переводов каретки Enter в словаре
        ulong nRank,rank;       // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank
        
        GStr<ulong>*vData_vector;   //массив внутренних переменных
        
        mSIZE  *dictionary_data; // массив словаря
        GStr<mSIZE>*dictionary_data_vector;
        ulong dict_size;           // размер массива словаря
        ulong BUFF_DICT_SIZE;      // количество пар букв в словаре. Вычисляется в момент создания словаря
    
        
        mSIZE  *text_data;       // массив распознаваемого текста
        GStr<mSIZE>*text_data_vector;
        ulong text_size;           // размер массива распознаваемого текста
        GStr<mSIZE>*resultTextVector;         // строка результатов вычисления вероятности букв в фразе
        mSIZE* resultText;
        GStr<ulong>*resultDelimeterVector;   // строка результатов вычисления вероятности разделителей в фразе
        ulong *resultDelimeter;
        
        /// массивы функции перекодировки букв словаря 
        mSIZE *BufM;          // массив предназначен для получения упакованного кода из исходного кода буквы.
        GStr<mSIZE>*BufM_vector;
        mSIZE *BufU;          // массив для восстановления исходного кода буквы (разреженного кода)
        GStr<mSIZE>*BufU_vector;
        // из упакованного кода буквы
        ulong *BufE;            // массив (int) для хранения адресов перевода каретки Enter словаря
        GStr<ulong>*BufE_vector;
        ulong size_BufE;
        /// массивы функции перекодировки пар букв словаря
        mSIZE *BufMp;         // массив BufMp упакованного кода пар букв
        GStr<mSIZE>*BufMp_vector;
        ulong size_BufMp;
        mSIZE *BufMpT;        // массив BufMpT пар букв текста. То же, что и BuffMp для словаря.
        GStr<mSIZE>*BufMpT_vector;
        // Предназначен для получения упакованного кода пар букв из исходного кода буквы,  
        // адресом массива является исходный код буквы (разреженный код) - ответом
        // упакованный код пары буквы.
        ulong size_BufMpT;
        
        mSIZE *BufUp;         // массив для восстановления исходного кода первой буквы пары
        GStr<mSIZE>*BufUp_vector;
        //mSIZE *BufUp2;         // массив для восстановления исходного кода второй буквы пары
        //GStr<mSIZE>*BufUp2_vector;
        ulong  *BufD;         //массив адресов разделителей в строках словаря
        GStr<ulong>*BufD_vector;
        ulong size_BufD;
        ulong  *BufDR;         //массив адресов строк BuffD
        GStr<ulong>*BufDR_vector;
        ulong size_BufDR;
        // из упакованного (компактного) кода пар букв словаря 
        /**/
        
        /// массивы функции перекодировки пар текста
        //wstring strTextW;            //
        //string strText;              //
        ulong *BufET;           // массив (int) для хранения адресов перевода каретки Enter текста
        GStr<ulong>*BufET_vector;
        //uint *BufMpT;      // зеркальный массив пар букв BufMpT текста
        mSIZE *BufUpT;        // массив для восстановления исходного кода первой буквы пары
        GStr<mSIZE>*BufUpT_vector;
        // из упакованного (компактного) кода пар букв текста
        mSIZE *BufUpT1;        // тоже что и массив BufUpT но состоит из int и служит для вычисление Max длины
        GStr<mSIZE>*BufUpT1_vector;
        // совпавшего фрагмента текста с непрерывном фрагментом фразы словаря.
        /// выходные переменные функции перекодировки пар текста
        uint text_size1;       // размер массива распознаваемого текста(ключей) без переводов каретки Enter

        uint size_BufET;
        uint size_BufUpT;
        uint size_BufUpT1;
        
        mSIZE  *BufLpT;
        GStr<mSIZE>*BufLpT_vector;
        ulong size_BufLpT;
        
        ulong nLetterPT;
        ulong clusters_size;    // размер массива цепочек адресов clusters_data
        
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
        mSIZE *BufTxt;        // восстановленная через словарь копия массива распознаваемого текста
        GStr<mSIZE>*BufTxt_vector;
        //  nev  //
        vector<int> BufDictStr;
        uint size_GravTxt;     // размер массива суммарных весов (в байтах), в общем случае size_GravTxt=text_size;
        mSIZE *GravTxt;       // буферный массив суммарных весов пар букв текста во всех легитимных фрагментах фраз словаря.
        GStr<mSIZE>*GravTxt_vector;
        uint size_SumTxtP;     // полный размер статистической устойчивости распознаваемого текста, size_SumTxtP=text_size1*nLetterP; 
        mSIZE *SumTxtP;       // массив статистической устойчивости распознаваемого текста (массив частоты встречаемости пар букв)
        GStr<mSIZE>*SumTxtP_vector;
        uint *letterX;      // массив координат пар букв в распознаваемом тексте
        
        uint xMaxStart;                 // координаты начала нечеткого непрерывного фрагмента текста максимальной длинны
        uint xMaxEnd;                   // координаты конца нечеткого непрерывного фрагмента текста максимальной длинны
        ulong xMaxStartDict;             // координаты начала нечеткого непрерывного фрагмента фразы словаря максимальной длинны
        ulong xMaxEndDict;               // координаты конца нечеткого непрерывного фрагмента фразы словаря максимальной длинны
        
        void printDictEntry(ulong n);
        void printDictTextLine(ulong n);
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
 
 Функция получает на вход 700мб файл словаряфраз (фактически в текстовом формате, буквы кодированны uint, фразы отделены друг от друга переводами каретки) и записывается в массив dictionary_Dat. Полные требованиям к файлу смотри в функции associativeSignalProcessing ТРЕБОВАНИЯ К СЛОВАРЮ И ТЕКСТУ:
 Функция перекодировки словаря (плотной упаковки разреженного кодового пространства словарных букв):
 - создает таблицы (массивы) для плотной паковки или распаковки кодов букв и пар букв.
 - осуществляет запись на диск результатов перекодировки словаря, с использовании функция быстрого чтения больших файлов в память.
 Данная функция для данного словаря выполняется однократно с сохранением результатов на диск (HD). Время выполнения
 функции 8-10 сек
 После завершения работы функции, в том же массиве dictionary_Dat возвращается словарь, в котором уже пары буквы  кодированны uint, но кодировка начинается с единици и заканчивается полным количеством пар букв в словаре. Плотно упаковывается разреженное кодовое пространство. Переводы каретки отсутстуют, адреса границ фраз лежат в массиве BuffE (массив int для хранения адресов перевода каретки Enter словаря). Пара букв это на самом деле чешуйка из пары букв с перекрытием в одну букву, поэтому число пар букв равно числу букв. Замена букв на пары букв фундаментально улучшает свойства текста, если разных букв в тексте несколько сотен, то пар букв несколько тысяч ( тиб 6000)
 
 
 uint *dictionary_Dat;  // массив словаря
 ulong *BuffE;                 // массив (int) для хранения адресов перевода каретки Enter словаря
 
 /// массивы функции перекодировки букв словаря
 uint BuffM[ALPHABET_SIZE+32];  // зеркальный массив BufM 128k (фиксированного размера). Предназначен для получения   упакованного кода из исходного кода буквы, адресом массива является исходный код буквы (разреженный код) - ответом упакованный код букв. Внутри массива лежат коды плотно упакованных букв.
 
 uint BuffU[ALPHABET_SIZE+32];  // Массив восстановления исходного кода буквы (разреженного кода) из упакованного кода буквы. BuffU 128k (фиксированного размера). Адресом массива является упакованный код буквы - ответом исходный код буквы (разреженный код). По смыслу выполняет обратную задачу массива BufM.
 
 // примеры прямой и обратной перекодировки букв словаря
 - упакованный код перевода каретки = BuffM[10] (исходный код перевода каретки 10, буквы и т.д. )
 - исходный код буквы = BuffU[dictionary_Dat[7] (упакованный код перевода каретки 7, буквы и т.д. )
 
 
 /// массивы функции перекодировки пар букв словаря
 По смыслу полностью соответствуют массивам BuffM и BuffU только вместо букв они кодируют - раскодируют пары букв
 uint *BuffMp;              // зеркальный массив пар букв BufMp словаря. (переменного размера).  Предназначен для получения упакованного  кода пар букв из исходного кода буквы, адресом массива является исходный код буквы (разреженный код) ответом упакованный код буквы. Пары букв перекрываются как чешуйки т.е. считаются в перекрышку, а не в стык.
 uint BuffUp[ALPHABET_SIZE+32]; // массив для восстановления исходного кода первой буквы пары. 128k (фиксированного размера).
 
 // примеры прямой и обратной перекодировки пар букв словаря
 
 // вывод на экран упакованного словаря dictionary_Dat начертанием, как текст UTF-16 (short)
 s=0; 
 for(x=0; x < dict_sizeGr; x++){ 
 str=(wchar_t)BuffUp[dictionary_Dat[x]]; cout<<Unicode_to_UTF(str); //  cout<<Unicode_to_UTF(dictionary_Dat[x]);
 if ( x==BuffE[s] ) { cout<<endl; s++; }
 }    
 
 
 // массивы словаря разделителей (пробелов). 
 Это компактный способ хранения разделителей (пробелов).
uint *BuffD;  // массив словаря разделителей BuffD (delimeterVecor) т.е. массив строк адресов разделителей. char в строках указывают положение пробелов в исходном массиве фраз словаря.
 unsigned int *BuffDR;  // массив индексов BuffDR (rowDelimeterVecor). Массив с адресами начала строк в словаре разделителей BuffD. Строки  соответствуют фразам в массиве словаря.  BuffDR - аналог массива перевода каретки словаря разделителей.
 
 
 
 
 ---------------------------------------------------------------------------------------------------------------------
 
 */



