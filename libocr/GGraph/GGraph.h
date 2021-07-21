//
// GGraph.h
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
// C- from the Free Software Foundation at http:///<www.fsf.org.

#ifndef __OCRLib__GGraph__
#define __OCRLib__GGraph__

#include "../config.h"
#include "../GBitMask/GBitMask.h"
#include "../GBitmap/GBitmap.h"
#include "../GDraw/GDraw.h"
#include "../OCRAlgorithm/OCRAlgorithm.h"
#include "../OCRString/php2stl.h"
#include "../OCRTypes/OCRTypes.h"
#include <stdio.h>
///<#include "../OCRFontEditor/OCRLetter.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;
using namespace ocr;

namespace ocr {
/// Все что короче LINE_SIZE является прямыми линиями, а все что длинее более
/// сложными кривыми.
#define LINE_SIZE 6
#define LINE_SIZE_W 6
#define MAX_EXTREM 8
#define SERIF_LIMIT 1.9f
#define MAX_H 8         ///< лимит высоты точки фокальной линии относительно базового отрезка используется при
                        ///< выделении экстремума фокальной линии. Равен средней толщине штриха текста
#define MAX_CURVE 10    ///< лимит угла учета кривизны линии
    
#define MAX_CLUSTER_LETTER 128  ///< максимальное количество кластеров буквы
#define MAX_LETTER_GROUP 256    ///< максимальное количество букв базы похожих между собой (размер нейросети буквы)
#define MAX_LETTER_GROUP1 512  ///< максимальное количество букв базы похожих между собой (размер нейросети буквы)
#define MAX_CLUSTER 400000000   ///< максимальное количество кластеров в GGraph


typedef struct OCRRegisterStruct {
	uint64 reg0_0; ///<поисковый регистр углов лимитированных фокальных линий с учётом коротких линий (с неопределённым
				   ///<углом)
	uint64 reg0_1; ///<регистр маски лимитов углов  лимитированных фокальных линий с учётом коротких линий (с
				   ///<неопределённым углом)
	uint64 reg1_0; ///<поисковый регистр углов  лимитированных фокальных линий без учёта коротких линий (с
				   ///<неопределённым углом)
	uint64 reg1_1; ///<регистр маски лимитов углов  лимитированных фокальных линий без учёта коротких линий (с
				   ///<неопределённым углом)
	uint64 reg2_0; ///<поисковый регистр   углов опорных линий
	uint64 reg2_1; ///<регистр маски лимитов углов опорных линий
	
	uchar nReg0;  ///<количество единиц в регистре reg0_0
	uchar nReg1;  ///<количество единиц в регистре reg1_0
	uchar nReg2;  ///<количество единиц в регистре reg2_0
	
	uint ID;
	uchar type;
	char status;
}OCRRegister;

typedef struct OCRMaskStruct {
	int mask[32];
	int reg[32]; ///<массив поисковых регистров маски кластера OCRStar
	//необходимо создать маску аналогичную maskCluster с тем же масштабом и смещением для проверки середин линий кластера
	uchar regY[32]; ///<массив координат Y поисковых регистров маски кластера OCRStar
	uchar regCount; ///<количество поисковых регистров маски кластера OCRStar
	uint ID;
	uchar type;
	char status;
}OCRMask;

typedef struct OCRLineStruct {
	uchar type;         ///< тип линии - базовая или линия между точками экстремума или линия круга
	uchar extrem[MAX_EXTREM];
	uchar extremCount;  ///< количество экстремумов линии
	uchar status;       ///< статус линии
	uchar area;         ///< количество пикселов линии
	uchar nodeCount;    ///< количество производных линий после выделения экстремумов
	uchar cType;        ///< тип формы линии
	uchar p0_y;         ///< координаты проверки точек линии по маске 32х32
	uchar p1_y;         ///< координаты проверки точек линии по маске 32х32
	uchar p2_y;         ///< координаты проверки точек линии по маске 32х32
	uchar p3_y;         ///< координаты проверки точек линии по маске 32х32
	uchar p4_y;         ///< координаты проверки точек линии по маске 32х32
	uchar startIndex;   ///< индекс линии в стартовой  OCRStar
	uchar endIndex;     ///< индекс линии в конечной OCRStar
    char alphaMid;      ///< величина относительного угла между линией соединяющей концы OCRLine и линией от начала к середине (выгиб кривой)
    char alpha1;        ///< величина относительного угла линии от начала OCRLine к 1/3 OCRLine (выгиб кривой)
    char alpha2;        ///< величина относительного угла линии от конца OCRLine к 3/3 OCRLine (выгиб кривой)
	ushort node[MAX_EXTREM]; ///< id номера производных линий (необходимо для lookup4)
	ushort lenA;        ///< количество пикселов линии.
	ushort lenE0;       ///< длина линии от начала до первой точки экстремума
	ushort lenE1;       ///< длина линии от конца до последней точки экстремума
	ushort length;      ///< длина линии до конечной точки без учета экстремумов
	ushort corr;        ///< величина корреляции с наиболее соответсвующей линией
	short xMin;
	short xMax;
	short yMin;
	short yMax;
	uint startID;       ///< id номер стартовой OCRStar
	uint endID;         ///< id номер конечной OCRStar
	uint index;         ///< id number в векторе focalLine
	uint parentID;      ///< id номер родительской линии. В линии кластера parentID сохраняет индекс линии в графе
	uint pointIndex;    ///< индекс первой точки линии в массиве pointArray
	int match;          ///< id номер гипотезы линии в тексте
	int p0_r;           ///< регистр проверки точек линии по маске 32х32
	int p1_r;           ///< регистр проверки точек линии по маске 32х32
	int p2_r;           ///< регистр проверки точек линии по маске 32х32
	int p3_r;           ///< регистр проверки точек линии по маске 32х32
	int p4_r;           ///< регистр проверки точек линии по маске 32х32
	int SumBitOn;       ///< количество точек в маске 32х32 линии. Если SumBitOn ==0 линия не имеет маски (слишком короткая)
	OCRPoint pointC[5]; ///< координаты пяти проверочных точек линии
	OCRPoint start;     ///< координаты начала линии.
	OCRPoint end;       ///< координаты окончания линии в конечной точке или экстремуме
	OCRPoint start32;   ///< координаты начала линии в маске 32х32.
	OCRPoint end32;     ///< координаты окончания линии в маске 32х32
	float lineW;        ///< средняя ширина штриха линии
	float scale0;       ///< масштаб приведения габарита звездочки к maskLine 32х32
} OCRLine;
    
typedef struct OCRLineMatchStruct {
    uchar line;         ///< id номер линии буквы
    uchar match[10];    ///< id номера линий текста
    uchar matchCount;
    uchar corr;
}OCRLineMatch;
    
typedef struct OCRStarStruct {
	char type;
	char level;
	char status;
	uchar minLen;	 ///<длина самой короткой линии
	uchar lineCount; ///<количество линий в OCRStar круга или количество соответствий линий при корреляции
	uchar starCount;
	uchar pointCount;
	uchar baseCount;  ///< количество опорных линий
	uchar cluster;	  ///< флаг OCRStar как ядра кластера
	uchar matchCount; ///< количество гипотез OCRStar в тексте
	ushort sumLen;	  ///< сумма всех лимитированных длин линий
	ushort sumLen1;	  ///< сумма всех лимитированных длин линий и периметра
	ushort sumLenA;	  ///<сумма длин  линий
	ushort disp;	  ///<дисперсия длин линий
	ushort hash;
	short xMin;
	short xMax;
	short yMin;
	short yMax;
	ushort corr;  ///< корреляция OCRStar
	ushort corrC; ///< корреляция OCRStar в кластере
	float radius; ///< радиус
	float mLen;	  ///< средняя длина луча
	float lineW;  ///< средняя толщина фокальной линии
	float scale;
	float scale0;
	OCRPoint cPoint; ///<центр звездочки
	OCRPoint point[12]; ///<массив фокальных точек концов лучей и опорных линий.
	char dir[4]; ///<массив индикаторов направления фокальных линий OCRStar
	char len[4]; ///<массив индикаторов длин фокальных линий OCRStar
	char line_weight[4]; ///< массив относительных весов корреляции фокальной линии в общей корреляции OCRStar
	uchar cMatch[4];	 ///<массив корреляции OCRStar кластера
	uchar sMatch[4]; ///<порядок соответствия фокальных линий OCRStar текста и эталона
	uchar lineCorr[4];  ///<массив величин корреляций фокальных линий звездочки
	uint index; ///< ID номер OCRStar в GGraph или OCRCluster
	uint sIndex; ///< для OCRStar кластера ID номер исходной OCRStar в GGraph
    uint clusterIndex; ///< для OCRStar кластера ID номер кластера, в GGraph ID номер кластера для центральной OCRStar кластера
	uint matchStar;
	uint line[4];  ///<массив id номеров фокальных линий OCRStar
	uint star[12]; ///<массив id номеров OCRStar соединных с центром OCRStar фокальной или опорной линией
	short angle[12]; ///<массив углов фокальных и опорных линии относительно центра OCRStar
	short match[4];	 ///<массив id номеров гипотез OCRStar текста
	OCRRegister reg;  ///< структура кольцевых регистров для lookup1
} OCRStar;

typedef struct ClusterMatchStruct {
	short sIndex;           ///<номер центральной OCRStar кластера в букве в которой размещен кластер.
	short clusterIndex;     ///<номер OCRCluster в букве в которой размещен кластер.
	short lineCount;        ///<количество OCRLine кластера для которых найдено соответствие
	uint letterIndex;       ///<номер буквы из которой скорирован базовый кластер.
	uint matchIndex;        ///<номер кластера которому соответствует этот кластер. (номер базового кластера или номер кластера текста)
    uint textClusterID;     ///<номер кластера тексте которому соответствует этот кластер.
	uint baseClusterID;     ///<номер соответствующего базового кластера в массиве базовых кластеров
    uint letterClusterID;	///<номер кластера буквы в массиве кластеров
	uint clusterCount;      ///<количество кластеров буквы
    ushort sumLen1;         ///< сумма всех лимитированных длин линий и периметров всех OCRStar
    ushort sumLenA;         ///< сумма всех длин линий всех OCRStar
    ushort sumLenStar;     ///< сумма длин линий центральной OCRStar кластера
    OCRPoint cPoint;        ///< центр ClusterMatch в букве
    OCRPoint cPointText;    ///< центр OCRCluster в тексте
	uint corr;
	float scale;
    float radius; ///< радиус
	ushort line[16];	///<номера OCRLine провереряемого OCRCluster для которых найдено соответствие
	ushort len[16];	///<длины OCRLine провереряемого OCRCluster для которых найдено соответствие
} ClusterMatch;

typedef struct OCRClusterStruct {
	uchar	status;
	uchar   type;
	uchar	level;       ///<порядок распознавания OCRCluster в букве
	uchar	lineCount;   ///<количество линий в OCRStar круга или количество опорных линий
	uchar	starCount;
	uchar	lCount;
	uchar   pCount;
    uchar   matchIndex;   ///<номер OCRCluster в букве для которого найдено соответствие по базовым кластерам.
    uchar   indexInLetter;///<номер OCRCluster в букве
    ushort  sumLen1;     ///< сумма всех лимитированных длин линий и периметров всех OCRStar
    ushort  sumLenA;     ///< сумма всех длин линий всех OCRStar
	uint    corr;
	uint w;
	uint h;
	int xMax;
    int yMax;
    int xMin;
    int yMin;
    uint letterIndex;   ///< ID номер буквы
	uint index;         ///< ID номер кластера в массиве кластеров OCRFont
	uint sIndex;        ///< ID номер первой OCRStar кластера в тексте или в букве
    uint baseClusterID; ///<номер соответствующего базового кластера в массиве базовых кластеров
	uint matchID;       ///< ID номер кластера в тексте соответствующий наилучшей корреляции кластера
	OCRPoint cPoint;    ///< центр OCRCluster
	OCRPoint cPointText;///< центр OCRCluster в тексте или букве
	OCRStar star[5];	///<OCRStar в OCRCluster
	OCRLine line[16];	///< массив OCRLine
	//OCRMask maskC;      ///< маска кластера
	float	scale;
	float	scale32;
    float radius;       ///< радиус
} OCRCluster;

/*
///<структура описания буквы распознаваемого языка
typedef struct OCRLetterStruct {
	GString name;
	OCRBox s; ///<координаты прямоугольника буквы относительно центра буквы и площадь габарита
	///<буквы
	uint letterIndex;
	uint letterID;
	ushort lineCount;  ///<количество OCRLine в букве
	ushort starCount;  ///<количество OCRStar в букве
	ushort pointCount; ///<количество точек отцифровки линий буквы;
	ushort matchCount;
	short xMatch; ///< центр буквы на странице
	short yMatch;
	ushort status;
	ushort imgW;
	ushort imgH;
	ushort lineH;
	ushort letterW;
	ushort letterH;
	ushort correlation;
	ushort correlationNew;
	ushort sumLen; ///< сумма длин линий всех звездочек буквы
	short xCenter;
	short yCenter; ///<координаты центра буквы в изображении буквы
	ushort OCRIndex;
	ushort clusterCount; ///<количество кластеров OCRStar буквы
	float scale;

	OCRLine *line;	 ///<указатель на массив OCRLine буквы
	OCRPoint *point; ///<массив точек отцифровки линий буквы
	OCRStar *star;	 ///<указатель на массив OCRStar буквы
	uchar *img;		 ///<нормализованное изображение буквы
	uchar *graphImg; ///<изображение фокальных линий
	uchar initLine_flag; ///< флаг инициализации line_data
	OCRData *line_data; ///<диаграмма Воронова и данные атрибутов пикселов
	// описание в @fn GGraph::attributeLinePixels()
	uchar *cluster;	 ///< индексы OCRStar кластеров буквы
	uint *match;	 ///< все matches в гипотезе буквы

} OCRLetter;
*/

typedef struct OCRLetterMatchStruct {
    float kLine[MAX_CLUSTER_LETTER]; //регистр кооэффициентов весов корреляции линий тестовой буквы
    float scale;
    short xCenter;
    short yCenter;
    uchar lineCount;    ///< количество весов линий буквы
    uchar corr;
    uint matchIndex;    ///< индекс в массиве font.letterMatch
    uint matchIndexTest;    ///< индекс OCRLetterMatch тестовой буквы в массиве font.letterMatch
    uint baseIndex;     ///< индекс базовой буквы
    uint testIndex;     ///< индекс тестовой буквы
    uint OCRIndex;      ///< OCRIndex буквы указывает на совпадение Юникода проверяемой пары букв
}OCRLetterMatch;
    
    
///<структура описания буквы распознаваемого языка
typedef struct OCRLetterStruct {
	GString name;
	OCRBox s;               ///<координаты габарита буквы относительно центра буквы и площадь габарита
	uint index; 
	uint letterID;          ///< ID номер буквы
    uint group;             ///< номер группы
    ushort OCRStatus;       ///< участвует ли буква в распознавании текста
    ushort gIndex;          ///< номер буквы в группе
	ushort clusterCount;    ///<количество OCRCluster в букве
    ushort matchLineCount;  ///<количество коррелированных OCRLine в букве
	ushort lineCount;       ///<количество OCRLine в букве
    ushort lineOCRCount;    ///<количество OCRLine в букве участвующих в корреляции
	ushort matchCount;      ///< количество базовых кластеров в  букве
	short xMatch;           ///< центр буквы на странице
	short yMatch;
	ushort status;
	ushort lineH;
	ushort letterW;
	ushort letterH;
	ushort correlation;
	ushort correlationNew;
	ushort lineSum;         ///< сумма длин линий  буквы
    ushort matchLineSum;    ///< сумма длин коррелированных линий  буквы
	short xCenter;
	short yCenter;          ///< координаты центра буквы в изображении буквы
	uchar OCRIndex;         ///< разметка шрифтовой грамматики буквы
    uchar fontType;         ///< к какому виду шрифта принадлежит буква
    uchar lang;
    uchar OCRKey;
	float scale;
	uint cluster[MAX_CLUSTER_LETTER];       ///< массив ID номеров кластеров в базе данных
    float kLine[MAX_CLUSTER_LETTER];        ///< массив весов OCRLine буквы
    int lineMatch[MAX_CLUSTER_LETTER];      ///< массив ID номеров распознанных линий текста
	ClusterMatch match[MAX_CLUSTER_LETTER]; ///< массив соответствия кластеров буквы и базовых кластеров

} OCRLetter;
    
typedef struct OCRGroupDataStruct {
    uint index;
    uint size;
    uint letter[MAX_LETTER_GROUP];
    uint match[MAX_LETTER_GROUP];   ///< индекс OCRLetterMatch в массиве
}OCRGroupData;

typedef struct OCRGroupStruct {
    uint index;
    uint size;
    uint letter[MAX_LETTER_GROUP1];
    OCRIntData match[MAX_LETTER_GROUP1];
}OCRGroup;

typedef struct OCRGroup2DStruct {
    uint index;
    uint size;
    uint letter[MAX_LETTER_GROUP1];
    //uint match[MAX_LETTER_GROUP][MAX_LETTER_GROUP];
}OCRGroup2D;

class GGraph : public GBitmap {
  protected:
  public:
	/** @name Функции инициализации */
	/** @bref  Initialisation by dimention*/
	GGraph(int acolumns, int arows);
	/** @bref  Initialisation by dimention and mode*/
	GGraph(int acolumns, int arows, int mode);
	/** @bref  Initialisation by bitmap*/
	GGraph(const GBitmap *ref);
	GGraph(TString &st);

	void init(int acolumns, int arows, int mode);
	void initData();

	/** @bref  Destructor */
	~GGraph();

	OCRData *line_data;         ///< массив данных атрибутированных пикселов.
                                // описание в @fn attributeLinePixels()
	int initLine_flag;          ///< флаг инициализации массива line_data

	ushort *id_data;            ///< массив для поиска id номеров фокальных точек по заданным координатам
	int initID_flag;            ///< флаг инициализации массива id_data
	ushort *id_data2;           ///< уменьшеный массив для поиска id номеров фокальных точек по заданным координатам
	int initID2_flag;           ///< флаг инициализации массива id_data2
	uint *bites_data_32;        ///< транспонированный массив для поиска фокальных точек
	uint *bites_data_32_2;      ///< уменьшеный транспонированный массив для поиска фокальных точек
    ushort imgW;                ///< ширина исходного PNG

	GStr<OCRPoint> focalPoint;
	GStr<OCRPoint> pointArray;  //массив точек оцифровки всех фокальных линий
	GStr<OCRLine> focalLine;
	GStr<OCRStar> starArray;
	GStr<OCRPoint> corrPointsArray;  //массив точек корреляции GGraph
	GStr<OCRCluster> clusterArray;  ///< массив кластеров GGraph построенных относительно каждой OCRStar
	uint pCount;                ///<количество инициализированных точек в corrPointsArray

	ushort *hashTable;          ///<таблица упаковки хеша
	uchar *hashTableStar;       ///<таблица упаковки неупорядоченного хеша OCRStar
	ushort *hashTableOCRStar;   ///<таблица упаковки упорядоченного хеша OCRStar
	char hashTableReady;        ///<флаг готовности таблиц хеша

	uint *BufS;                 ///<массив адресов начала и конца слайсов (slice)
                                ///<предназначен для перепрыгивания белых областей.
	uint sizeBufSR;             ///<количество слайсов
	uint nRemovLayers;          ///<количество слоев эррозии изображения
	float *coord_fx;            ///<массив координат точек всех фокальныэх линий по "y", float.
	float *coord_fy;            ///<массив координат точек всех фокальныэх линий по "x", float.
	float *alpha_fl;            ///<массив углов (второй производной) вдоль фокальной линии, float.
	GDraw draw;                 ///< объект для вывода данных GGraph в SVG и stdout

	/** @name Функции распознавания фокальных линий */

	/** @bref Функция построения фокальных линий в исходном изображении путем
		 последовательной эррозии на основе таблиц пиксельных матриц 3х3 */
	void graphFocalLine(int n_RemovLayers);
	/** @bref Функция построения таблиц распознавания фокальных линий необходимых
		 для функции focalLineRecognition.*/
	void buildOCRLineTable();

	/** @bref Функция выделения фокальных точек на фокальных линиях (графемах) букв.
		 Ставит точки в "Т" узлах графа, в "Х" узлах и концах линий "L_End".
		 Определяется тип точки и записывается в ее статус.*/
	int focalPointXTL();
	/** @bref Функция выделения стабильных фокальных точек на фокальных линиях
		 (графемах) букв.*/
	int StablePoint();
	/** @bref Функция выделения стабильных фокальных точек на фокальных линиях
		 (графемах) букв. Версия с более подробным вычислением статуса фокальных
		 точек.*/
	int StablePointT();
	/** @bref Функция распознования фокальных линий (графемы) букв.*/
	int focalLineRecognition();
	/** @bref Функция создает для каждой фокальной линии структуру
		 хранения черных пикселов которые образуют данную фокальную линию.*/
	void attributeLinePixels();
	void attributeLinePixelsG(GBitmap *img);
	void attributeLinePixelsF(GBitmap *img);
	/** @bref Функция выделения экстремума (перегиба) фокальной линии */
	void findExtremPoints();

	/** @bref Функция выделения устойчивых острых экстремумов (перегибов) фокальной линии по углам.
		 Основная рабочая. _20_окт_2019 */
	void findExtremAlpha();
	/** @bref Функция сглаживания фокальной линии.*/
	void focalLineMid();
	/** @bref Функция для подсчета реальной длины сглаженной фокальной линии.*/
	void focalLineLengthMid();
	/** @bref Функция выделения точек перегиба и максимумов сглаженной фокальной линии по углам.
		 Основная рабочая. _26_окт_2019 */
	void findExtremAlphaFl();
	/** @bref формирование масок 32х32 необходимой
	для вычисления корреляций звездочек в @fn GGraphOCR::OCRStarCorrelationLookup2
	и @fn GGraphOCR::OCRStarCorrelationLookup3*/
	void buildFocalLineMask();
	/** @name Функции создания OCRStar */
	/** @bref  инициализация поискового массива ID номеров фокальных точек*/
	void initID();
	/** @bref  инициализация массива starArray */
	void initStarArray();
	/** @bref  запись данных в OCRStar на конце фокальной линии */
	void setEndStar(OCRStar &startStar, OCRLine &lineP, int endID, int nl, int id, int np);
	/** @bref запись данных в OCRStar на фокальной линии круга*/
	void setStarInCircle(int x);

	/** @bref  вычисление параметров звездочек */
	void buildOCRStars();
	/** @bref  Обьединение смежных T_POINT в одну X_POINT */
	void buildXPoint();
    /** @bref  Определение величины и знака выгиба кривой (вектора). */
    float CorrelationCurve(OCRPoint &p0, OCRPoint &p1, OCRPoint &p2);
    /** @brief  Определение величины и знака выгиба кривой (вектора) для всех OCRLine в GGraph */
    void setCorrelationCurve();
	/** выявление неустойчивых точек типов T_POINT X_POINT и создание OCRStars для каждой точки экстремума */
	void detectP_POINT();
	/** @bref распознавание кругов образованных фокальными линиями. */
	void detectCircle();
	/** вспомогательная функция для @fn detectCircle */
	void buildCircle(OCRStar &star);
	/** @bref  Функция вычисления хеша звездочки */
	void setOCRStarHash(OCRStar &star);
	/** @bref   создание маски фокальных линий*/
	void buildLineMask(OCRLine &line);
	/** @bref распознавание опорных линий для всех OCRStar*/
	void bearingLineRecognition(OCRStar &star, int mode);
	/** @bref Функция создания хеш регистра углов фокальных и опорных линий  OCRStar */
	void buildHashRegister(OCRStar &star, int mode);

	/** @bref Функция создания кластера OCRStar */
	void buildCluster(OCRStar &star);
	
	/** @name Функции классификации и распознавания OCRStar */
	/** @bref Функция тестирования хеш регистра на 64 разряда.
		 Основная рабочая. _12_окт_2019_ */
	void hash_register64();
	/** @bref Функция тестирования хеш регистра на 128 разрядов.
		 Основная рабочая. _16_окт_2019_ */
	void hash_register128();
	// void buildPontArea();
	/** @bref Функция поиска координат фокальных точек.
		 Функция получает на вход координаты и размер прямоугольника области поиска.
		 Возвращает координаты фокальных точек попавших в прямоугольник области
		 поиска их количество и ID номера*/
	ushort detectFocalPoint(short search_x, short search_y, ushort search_dx, 
													ushort search_dy, ushort w, ushort h,
													uint *data_32, OCRPoint *adrFocalPoint);
	/** @bref Функция вычисления корреляции фокальных линий  */
	void focalLineCorrelation(OCRLetter &letter);
	/** @bref Функция распознавания множеств (слогов, букв, протобукв и модификаторов)
		 OCRStar - созвездий  */
	void OCRStarRecognition();
	/** @bref Функция создает таблицу хеша для всех типов OCRStar.  */
	void buildHashTable();
	/** @bref Функция создания OCRLetter из данного графа  */
	void buildLetter(OCRLetter &letter);
	/** @bref Функция вычисления корреляции группы звездочек.
		 Исходный текст  */
	void letterCorrelation(OCRLetter &letter);

	/** @name Вспомогательные функции */
	/** упаковки данных в GGraph, при котором в одном int32 записано 32 пиксела черно-белого изображения.
		 данные транспонированы по вертикали и значения пикселов записаны как колонки по 32 пиксела
		 (значения битов в integer) */
	uchar *packImg32V_(int w, int h, uchar *bytes_data_);
	/** @bref  проверка угла образующих отрезков двух кривых Безъе которые проведеных
		 к точке экстремума фокальной линии. Такой угол равен также углу между двумя
		 касательными к двум кривым Безъе в этой точке */
	bool focalLineExtremLimit(OCRLine &line, int index0, int index1);
	
	void printPoint(int x, int y);
	void printPointS(int sl);
	void printPointP(unsigned char *p0);
	void saveToTStr(TString &st);
	
}; ///< namespace ocr

///< static bool sortLineXCenter(const OCRLine& d1, const OCRLine& d2){
///<	return d1.center.x < d2.center.x;
///<}

///main class for ctore all data about letters in base
class OCRFont{
    private:
    public:
        OCRFont();
        virtual ~OCRFont(); 
        void destroy(void);
        static OCRFont* create(){return new OCRFont();}
        
        GStr<OCRLetter>font;                ///< main array of OCR Letter
        GStr<OCRCluster> clusterArray;      ///< массив всех кластеров всех букв
        GStr<OCRCluster> baseClusterArray;  ///< массив всех базовых кластеров всех букв
        GStr<uint> indexArray;              ///< таблица соответствия letter.index - letterID
        GStr<GString> pathArray;            ///< array of letter images path
        GStr<pointLine> fontSortArray;      ///< массив порядка распознавания букв отсортированный по комплексности буквы
        GStr<OCRGroup> letterGroupArray;    ///< массив груп похожих букв (нейросеть распознавания графической разности и графической суммы букв)
        GStr<OCRGroup2D> letterGroup2D;     ///< массив груп похожих между собой букв (нейросеть распознавания графической разности и графической суммы букв)
        GStr<OCRGroupData> letterGroupData; ///< исходный массив груп похожих букв (используется для построения letterGroupArray)
        GStr<OCRLetterMatch> letterMatch;   ///< массив OCRLetterMatch для всех пар букв в группах
        GVector *alphabetLetters;           ///< массив GGraph базовых графем распознаваемого языка
       	GVector *fontRegisterTable;         ///< таблица кольцевых регистров углов
                                            /// базовых кластеров, строка таблицы заполняется информацией о базовых кластерах в которых совпадает кольцевой регистр углов базового кластера
        GStr<OCRRegister> fontRegisterGStr; ///< массив кольцевых регистров базовых кластеров
        GVector *fontMaskTable;             ///< таблица масок базовых кластеров,
                                            ///  строка таблицы заполняется информацией о базовых кластерах в которых совпадает маска базового кластера
        GStr<OCRMask> fontMaskGStr;         ///< массив масок базовых кластеров
        GVector *fontClusterTable;          ///< таблица базовых кластеров
                                            ///  строка таблицы заполняется информацией о буквах в которых найден базовый кластер этой строки
        GVector *letterClusterTable;        ///< таблица базовых кластеров буквы.
                                            ///< каждая строка заполняется набором базовых кластеров одной буквы.
        GDraw draw;                         ///< объект для вывода графики GGraph в SVG
    
        int textLineSize;
        string fontName;
        string fileName;
        uint maxID;
        uint letterReadCount;
        map<string,int>fontOCRIndex;        ///< разметка шрифтовой грамматики (граммтическая роль каждой буквы)
        map<int,int>fontID;                 ///< std::map перекодировки letterID -> index
        ulong *OCRLanguage;	///<язык распознавания
        
        int indexById(uint Id);
        //** @bref функция чтения данных буквы по ее ID номеру. */
        OCRLetter& letterByID(uint ID);
         //вызов из массива осуществляется по ID номеру буквы.
        void reloadPtr();	 	 	 	  //восстановление указателей на внутренние переменные после изменения размера или расположения в памяти
        void clear();   ///< erase all data
        void clearClusterTable();
        void clearLetterGroup();
        void clearLetterGroupData();
        void clearLetterGroup2D();
        ulong size(void){return font.size();}
        OCRLetter&	operator[](uint index);
        OCRLetter& getLetter(uint index);
        OCRLetter& getLetter_(uint index);
        void getAlphabetLetter(uint index,TString &st);
    
        void readOCRFontXML();                        ///чтение OCRFont из OCRData/OCRTables/fileName в память или импорт в базу данных
        void readOCRFontDB();                         ///чтение OCRFont из базы данных
        void writeOCRFontXML();                       ///запись OCRFont из OCRData/OCRTables/fileName      
        GBitmap* drawPict(int in);
        GBitmap* drawLetterPict(int in);
        void drawLetterSVG(string &dataStr, unsigned int in);
        void printLetterSVG(unsigned int in);
        void drawOCRBasePict(uint startRec,uint rowsNum, int mode);
        GBitmap *drawOCRBaseImage(uint startRec,uint rowsNum);
        GBitmap *drawOCRBasePict(vector<ulong>&searchResult,int mode);
        vector<uint>letterSet;                       ///массив индексов букв распознаваемого языка
        /**вывод в result HTML rowsNum записей начиная с startRec */
        void drawHTML(uint startRec,int rowsNum,string &result,int mode);
        void drawHTML(vector<ulong>&searchResult,string &result,int mode);
        void fontNormalisation();    //проверка ID номеров и создание IDTable
        /** @bref Функция создает OCRFont на основе обучающей выборки*/
        void buildOCRFont(cstr path);
        /** @bref Функция добавляет в OCRFont новые изображения обучающей выборки*/
        void updateOCRFont(cstr path);
        void buildSortArray();
            /** @bref Читает с диска папку с папками изображений букв в формате PNG
        и создает OCRletter для каждого файла. 
        Папки с изображениями букв  могут быть архивированы в формате ZIP.*/
        void buildLetters(cstr path, int mode);
        void buildLetter(cstr path, int mode);
        void joinLetters(OCRLetter &letterRoot, OCRLetter &letterMark,int mode);
    
        /** @name вспомогательные функции */
        void printGroup(OCRGroup &group);
        void printGroup(int index);
        void printGroupData(OCRGroupData &group);
        void printGroup2D(OCRGroup2D &group);
        void printGroup2D(int index);
        void cpLetter(OCRLetter &letter);
	
};

} // namespace ocr

#endif ///< defined(__OCRLib__GraphOCR__)
