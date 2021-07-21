//
//  GGraph.h
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

#ifndef __OCRLib__GGraphOCR__
#define __OCRLib__GGraphOCR__

#include "../GGraph/GGraph.h"
#include "../GDraw/GDraw.h"
#include "../OCRString/php2stl.h"
#include "../config.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace ocr;

#define A_LIMIT 11 //лимит сдвига координат линии в маске 32x32
#define PV(x) cout << x << endl;

namespace ocr {

class GGraphOCR {
  protected:
  public:
	/** @name Функции инициализации */
	/** @bref  Initialisation by constructor*/
	GGraphOCR(void);

	/** @bref  Destructor */
	~GGraphOCR();

	OCRFont font;
	GDraw draw; ///< объект для вывода данных GGraphOCR в SVG и stdout

	vector<int> letterLineReg; ///< таблица учета количества проверок корреляции фокальных линий буквы (исползуется в lookup4)

	ushort *hashTable;		   ///< таблица упаковки хеша
	uchar *hashTableStar;	   ///< таблица упаковки неупорядоченного хеша OCRStar
	ushort *hashTableOCRStar;  ///< таблица упаковки упорядоченного хеша OCRStar
	char hashTableReady;	   ///< флаг готовности таблиц хеша
	uint printCorrelation;	   ///< глобальная переменная вывод отладки корреляции буквы
    
    //лимиты корреляции
    float correlationLimit;
    int letterCorrelationLimit;
    int letterCorrelationLimitGroup;
    int letterCorrelationLimitVoronov;
    //лимит корреляции совпадают с лимитом корреляции базовых кластеров, но меньше на единицу для устранения неоднозначности
    int limitBaseClusterCorrelation;
    int limitAlphaMid;          ///< лимит разницы углов лучей к концу и середине OCRLine
    int limitAlpha;             ///< лимит разницы углов лучей к концу и 1/3 OCRLine
    int pointSearchLimit;       ///< лимит поиска точки - limitCluster=(letter.letterH+letter.letterW)/pointSearchLimit*scale;

    //счетчики количества выполнения функций при распознавании GGraph
    int countStar;
    int countMask;
    int countLineMask;
    int countGeomentry;
    int countLetterCorrelation;
    int countLookup4;
    int countVoronov;
    int grammarMode;
    int grammarModeLetterID;
    vector<int>fontReg;
    vector<OCRBox>strArray;   ///< массив координат строк на странице
    //vector<vector< vector<pointLine> > >baseID2TextCluster; ///<реестр соответствия ID номера базового кластера и координат кластера текста
    
    string strTest;
    int letterPrint; //1569358799 a; //ga 1619655356; //1618920685 yo; //1619281359;//1619281354 tha;//1576660907 tha; //1576660907;   //1618962863 do;//1569374541 na //1569365687 rja;//1569372737 rda//1569370478//1577738079 //1569377825 do//1569364533 ma//1569369431 na //1569368608 //1569365026//1618705957//1569365140//1618345631 //1569368532

    
    int modeVoronov;
    int modeCorrelationResult;
    
    
    //лимиты точности корреляции
    int clusterCorrelationLimit; //лимит точности сравнения базовых кластеров с кластерами буквы и текста
    
	//** @name OCR font functions */

	/** @bref создает html файл изображения буквы */
	void drawLetter2Path(OCRLetter &letter, const char *path);
    //draw function
    void drawLetter(OCRLetter &letter);
    void drawLetterByID(int letterID);
    void drawGraph(GGraph &graph);
    void drawLetterMatch(OCRLetterMatch &letterMatch);
    
    void buildBaseOCRLetterGroup(vector<OCRLetter>&matchVector);
    void buildOCRLetterGroup(vector<OCRLetter>&matchVector);

	/** @bref запись в текстовом формате hex  TString которая содержит OSRLetter. Необходимо для передачи в функции Java Script */
	void letterToHex(OCRLetter &letter, string &str);
	//** @bref создает html файлы изображений букв OCR шрифта */
	void drawFont();
	//** @bref создает структуру весов параметров распознавания группы букв OCR шрифта*/
	void buildAlphabet();
    void buildletterGroup(OCRLetter &letter);
	//** @bref создает сортированную таблицу кластеров OCR шрифта */
	void buildOCRFontClusterTable(int mode);
	void clearLetter(OCRLetter &letter);
	void copyMatch(OCRLetter &letter, OCRLetter &matchL);
	
	
	
	

	/** @bref Детектор масштаба буквы относительно текста. */
	void detectScale(OCRStar *starsT, OCRLetter &letter);
    /** @bref Детектор масштаба буквы относительно текста на основе корреляции базовых кластеров. */
    void detectScaleCluster(OCRLetter &letter);
	/** @name Функции корреляции */
    
    /** @bref Функция поиска гипотез буквы в тексте
     Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
     с проверкой совпадения базовой OCRStar кластера и текста по lookup1-2-4 */
    void textLetterCorrelation(GGraph &graph, vector<OCRLetter>&mainMatchLetter);
    /** @bref Функция поиска гипотез буквы в тексте на основе групп похожих букв и весов OCRLine
     Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
     с проверкой совпадения базовой OCRStar кластера и текста по lookup1-2-4 */
    void textLetterGroupCorrelation(GGraph &graph, vector<OCRLetter>&textMatch);


	/** @bref Функция поиска гипотез буквы в тексте.
	 Выполняется проверка первого кластера буквы со всеми OCRStar текста */
	void textCorrelation(GGraph &graph, OCRLetter &letter);

	/** @bref Функция поиска гипотез буквы в тексте
	 Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
	 с проверкой совпадения хеша базовой OCRStar кластера и текста */
	void textCorrelationHash(GGraph &graph, vector<OCRLetter> &matchLetter, int correlationLimit);

    /** @bref Функция определяет величину корреляции каждой буквы OCR шрифта со всеми другими буквами */
	void buildOCRFontCorrelation(cstr path);

	/** @bref Функция тестовой корреляции буквы и изображения буквы текста */
	void testCorrelation(int letterID);
	/** @bref Функция вычисления корреляции и графической разности двух OCRLetter алфавита*/
	int letterAphabetCorrelation(GGraph &graph, OCRLetter &letterTest, int  mode);
	/** @bref Функция вычисления корреляции двух OCRLetter на основе кластеров и масштабонезависимых lookup */
    void letterClusterCorrelation(OCRLetter &letterTest, OCRLetter &letter);
	/** @bref Функция вычисления корреляции OCRLetter и GGraph на основе масштабонезависимых lookup */
	void correlationGraph(GGraph &graph, OCRLetter &letter);
	int correlationGraphArea(GGraph &graph, OCRLetter &letter, vector<int> &searchStar);
	/** @bref Функция вычисления корреляции OCRLetter и GGraph на основе гипотезы масштаба */
	void correlationOCR(GGraph &graph, OCRLetter &letter, vector<OCRCluster> &searchResult);

	int clusterCorrelation(OCRCluster &clusterT,OCRCluster &cluster);
    /** вычисление корреляции буквы и текста на основе найденных соответствий базовых кластеров в области поиска */
    int letterTextClusterCorrelation(GGraph &graph, OCRLetter &letter, int mode);
    
    /** вычисление корреляции группы похожих буквы и текста на основе найденных масштаба и центра буквы */
    int letterGroupClusterCorrelation(GGraph &graph, OCRLetter &letter, int mode);
    
	/** вычисление корреляции кластера и текста на основе найденных соответствий базовых кластеров в области поиска*/
	int letterCusterCorrelation(GGraph &graph, OCRLetter &letter, OCRStar &starT, OCRStar &star);
	/** @bref Функция вычисления корреляции двух букв OCR шрифта на основе OCRLetterMatch*/
	int letterMatchCorrelation(OCRLetter &letterBase, OCRLetter &letterTest, OCRLetterMatch &match);

	/** @bref Функция вычисления корреляции двух OCRStar */
	int OCRStarCorrelation(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines);
	
	/** @bref ПЕРВЫЙ lookup кореляции звездочек @fn OCRStarCorrelation
	 по совпадению углов фокальных и опорных линий OCRStar*/
	int OCRStarCorrelationLookup1(OCRRegister &regT, OCRRegister &reg);
	/** @bref ВТОРОЙ lookup кореляции кластеров @fn OCRStarCorrelation
	Сравнение кластеров по концам лучей в нормированной маске 32х32 */
	int clusterMaskCorrelation(OCRMask &maskT,OCRMask &mask);
	/** @bref Сравнение  двух OCRline по нормированной маске 32х32 */
	int OCRLineMaskCorrelation(OCRLine &lineT, OCRLine &line);
	/** @bref Сравнение  двух OCRline по нормированной маске 32х32 с учётом аффинных преобразований */
	int OCRLineMaskAffineCorrelation(OCRLine &lineT, OCRLine &line, int dir);

	/** @bref ВТОРОЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение звездочек по форме лучей в нормированных масках 32х32  */
	int OCRStarCorrelationLookup2(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines, uchar *lineCorr, uchar *sMatch, int mode);
	/** @bref ВТОРОЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение звездочек по форме лучей в нормированных масках 32х32 
	c учетом аффинных преобразований*/
	int OCRStarCorrelationAffineLookup2(
				OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines,uchar *lineCorr, uchar *sMatch);
	
	int OCRStarCorrelationLetterLookup3(OCRLetter &letterT, OCRLetter &letter);

    /** @bref ЧЕТВЕРТЫЙ lookup проверки кореляции кластеров эталона и текста по масштабозависимым алгоритмам */
    int OCRClusterCorrelationLookup4(OCRCluster &cluster, OCRCluster &clusterText, OCRLetter &letter, GGraph &graph, OCRData32 &lineCor);
    /** @bref ЧЕТВЕРТЫЙ lookup проверки кореляции кластеров эталона и текста по масштабо-зависимым алгоритмам */
    int OCRLineCorrelationLookup4(OCRLetter &letterT, OCRLetter &letter) ;

    /** @bref функция ЧЕТВЕРТОГО lookup проверки кореляции OCRLine эталона и текста по масштабозависимым алгоритмам. Проверка совпадения OCRLine эталона  и текста методом проверки по массиву атрибутированных пикселов фокальных линий line_data (диаграмма Воронова)*/
    void OCRLineCorrelationVoronov(OCRLetter &letterT,
														int lineIndex, 
														int starIndex,
														OCRLetter &letter);
	/** @bref ЧЕТВЕРТЫЙ lookup кореляции звездочек @fn OCRStarCorrelation
		Сравнение звездочек по лимитам координат концов лучей звездочек и точек экстремума */
	int OCRStarCorrelationLookup4(OCRStar &star, GGraph &graph, OCRLetter &letter);

	/** @bref функция ЧЕТВЕРТого lookup кореляции звездочек @fn OCRStarCorrelation
	 Сравнение звездочек по совпадениию точек фокальных линий эталона и текста по диаграмме Воронова */
	int voronovDiagramCorrelation(OCRStar &starT, OCRStar &star, GGraph &graph, OCRLetter &letter, OCRCluster &cluster,OCRData32 &lineCor);

	/** @bref ПЯТЫЙ lookup кореляции звездочек @fn OCRStarCorrelation
		Сравнение формы штрихов исходного изображения соответствущих фокальным линиям лучей звездочек. */
	int OCRStarCorrelationLookup5(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines);
    void lineOCR();
	/** @bref основная функция распознавания текста на основе графов фокальных линий */
	void textOCR();
	/** @bref функция построения сортированной таблицы корреляции эталона буквы и
	 обучающей выборки*/
	void buildLetterCorrelationTable(uint letterIndex, cstr path);
    void setBaseClasterInText(GGraph &graph,vector<vector<int> >&textCluster,vector<vector<int> >&text2Cluster,vector<vector< vector<pointLine> > >&baseID2TextCluster);
	void backwardCorrelation(vector<OCRLetter> &matchLine, GGraph &graph);
	void buildTextLine(vector<OCRLetter> &matchLetter, GGraph &graph, string &mainString);
    void checkFontGrammar(vector<OCRLetter> &textMatch,OCRLetter &letterTest);
	void drawCorrelationImg(vector<OCRLetter> &matchLetter, GGraph &graphPage, GBitmap *imgPage);

	/** @name вспомогательные функции */ 

	/** @bref Функция создает таблицу хеша для всех типов OCRStar.  */
	void buildHashTable();
	/** @bref Функция иницаализирует таблицу хеша в GGraph данными GGraphOCR */
	void initHashTable(GGraph &graph);
    
    
    //функция сортировки результатов поиска гипотезы буквы в тексте по базовым кластерам
    //для гипотез с похожей корреляцией отдаем предпочтение гипотезе с большим количеством кластеров
    static bool sortMatchCorrelation(const ClusterMatch &d1, const ClusterMatch &d2) { return d1.corr + d1.clusterIndex * 0.05 > d2.corr + d2.clusterIndex * 0.05; }
    
    //функция сортировки результатов поиска гипотезы буквы в тексте по базовым кластерам
    //для гипотез с похожей корреляцией отдаем предпочтение гипотезе с большим количеством кластеров
    static bool sortLetterC(const OCRLetter &d1, const OCRLetter &d2) { return d1.correlation > d2.correlation; }
    

    
    
};

} // namespace ocr

#endif
