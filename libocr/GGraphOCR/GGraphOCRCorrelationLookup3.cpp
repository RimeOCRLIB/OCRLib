// ____GGraphOCRCorrelationlookup3.cpp
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
#include "GGraphOCR.h"

namespace ocr {
/** @bref ТРЕТИЙ lookup проверка сооответствия геометрии эталона и гипотезы буквы в тексте */
int GGraphOCR::OCRStarCorrelationLetterLookup3(
											OCRLetter &letterT,OCRLetter &letter) {
	/**
		Алгоритм сентябрь 2020
		проверка соответствия  взаимного расположения OCRStar и OCRLine гипотезы эталонной буквы
		В процессе выполнения алгоритма для каждой OCRStar и OCRLine эталонной буквы
		определяется наболее соответствующая по располодению и lookup 1,2 гипотеза в тексте
		поиск соответствия OCRLine эталона с низкой корреляцией продолжается в lookup4
		
		На данном этапе разработки сравнение OCRStar и OCRLine эталонной буквы в lookup 3 производится заново. Результаты сравнения OCRStar и OCRLine эталонной буквы и текста необходисо кешировать на основе
		массива кеширования. Записи информации о срвнении в данные 
		OCRStar и OCRLine затираются в результате перебора комбинаций
		соответствия кластеров эталона и текста.
	*/
	
	/*
	int print = 1;
	DR(endl<<"===   Lookup 3 ===" << endl);
	OCRStar *starsT=letterT.star;
	OCRLine *linesT=letterT.line;
	//OCRPoint *pointT=letterT.point;
	int starTCount=letterT.starCount;
	letter.xMatch = 0;
	letter.yMatch = 0;
	int clusterCorrelation = 0;
	uint clusterCount = letter.clusterCount;
	int flag = 1; //флаг вычисления центра гипотезы буквы по первому кластеру
	vector<int>reg;
	reg.resize(letter.clusterCount);
	uchar sMatch[4]; ///< порядок номеров линий OCRStar текста соответствующих кластеру OCRStar буквы
	uchar lineCorr[4]; ///<значения корреляции OCRLine текста соответствующих кластеру OCRStar буквы

	//создаем массив кеширования результатов корреляции
	char *registerStar=(char*)calloc(1,letter.starCount);
	
	while(1){
		//Находим первый кластер с высокой корреляцией и уровнем
		int maxCorr=0;
		int indexMax=-1;
		//Для всех кластеров гипотезы буквы
		for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
			OCRStar &star = letter.star[letter.cluster[clusterIndex]];
			if(registerStar[star.index])continue;
			if(star.match[0] < 0) continue;
			int corr=star.corrC+10/star.level;
			if(corr > maxCorr){
				maxCorr=corr;
				indexMax=clusterIndex;
			}
		}
		if(indexMax<0)break;
		OCRStar &star = letter.star[letter.cluster[indexMax]];
		registerStar[star.index]=1;
		DR(endl<<"//@@  start check star:" << star.index << " corrC:" << star.corrC << " corr:" << star.corr<<" match:"<< star.match[0]<< endl;)

		//вычисляем центр гипотезы буквы по первому кластеру имеющему высокую корреляцию 
		if (flag) {
			OCRStar &starT=starsT[star.match[0]];
			if(star.corr <97){
				int corr_=OCRStarCorrelationAffineLookup2(starT, star, linesT, letter.line,lineCorr,sMatch);
				DR("result Affine c:"<<corr_<<endl;)
				if(corr_>star.corr)star.corr=corr_;
			}
			letter.xMatch = starT.cPoint.x - star.cPoint.x * letter.scale;
			letter.yMatch = starT.cPoint.y - star.cPoint.y * letter.scale;
			DR(" letter.xMatch:"<<letter.xMatch<<" letter.yMatch:"<<letter.yMatch<<" starT.cPoint.x :"<<starT.cPoint.x <<" star.cPoint.x "<<star.cPoint.x<< endl;)
			flag = 0;
			clusterCorrelation += star.corrC;
			//исключаем из разбора OCRStar текста которые имеют кластерное и геометрическое соответствие
			starT.matchID=letter.letterID; 
			for(int in=0;in<star.lineCount;in++){
				if(star.sMatch[in]>4)continue; //линия не имеет соответствия
				OCRLine &line=letter.line[star.line[in]];
				line.corr=star.lineCorr[in];
				line.match=starT.line[star.sMatch[in]];
				DR("  ### write in first star:" <<star.index<< " starT:"<< starT.index 
				<<" line:" <<line.index << " lineT:"<< line.match << " corr:"<< line.corr
				<<endl;)
			}
			
			DR(" //------ LookUp3 star: " << star.index << " corrC:" << star.corrC << " corr:" << star.corr<<" match:"<< star.match[0] << endl);
			continue;
		}
		//для всех последущих кластеров
		//для центральной OCRStar кластера выполняем проверку соответствующей OCRStar текста по лимитам координат
		//в соответствии с гипотезой масштаба.
		int x = (letter.xMatch + star.cPoint.x * letter.scale);
		int y = (letter.yMatch + star.cPoint.y * letter.scale);
		int limit = star.lineW * 4 * letter.scale;

		//проверяем соответствие гипотезы соответствия OSRStar центра кластера  гипотезы буквы и текста
		//по масштабонезависимым lookup
		//один кластер может иметь несколько соответствий по геометрии и аффинному lookup2
		//в этом случае понижаем корреляцию обоих гипотез и передаем задачу 
		//выбора наилучшего соответствия для этой OCRStar в lookup 4 
		OCRStar &starT = starsT[star.match[0]];
		star.matchCount=0;
		DR("   ___ text star:" << starT.index << endl);
		//проверяем starT по лимитам координат
		int absX = abs(starT.cPoint.x - x);
		int absY = abs(starT.cPoint.y - y);
		//cout << "absX:" << absY << "absY:" << absX << endl;
		if (absX > limit || absY > limit) {
			cout<<"not match:"<<" absX:"<<absX<<" absY:"<<absY<<endl;
			//если кластер не соответствует лимитам координат
			//считаем кластер не распознанным
			//освобождаем соответствующую OCRStar текста
			OCRStar &starT=starsT[star.match[0]];
			if(starT.matchID==letter.letterID && star.matchStar==star.index){
				starT.matchID=-1;
				starT.corrC=0;
			}
			star.match[0] = -1; 
			star.corrC= 0; 
			
			if (starT.type == O_POINT) {
				//если в букве не найден круг ухудшаем корреляцию буквы.
				//это описательное правило и должно устанавливатся в матрице условных переходов
				clusterCorrelation -= 50;
			}
			continue;
		}
		//если OCRStar прошла проверку по взаимному расположению
		//переписываем в линии OCRStar номера и корреляцию соответствующих OCRLine текста 
		if (star.type == O_POINT) {
			//для центров кругов проверяем только совпадение типов
			clusterCorrelation += 100;
			//исключаем из разбора OCRStar текста которые имеют кластерное и геометрическое соответствие
			starT.matchID=letter.letterID; 
			starT.matchStar=star.index; 
			starT.corrC=star.corrC;
			for(int in=0;in<star.lineCount;in++){
				OCRLine &line=letter.line[star.line[in]];
				line.corr=100;
				line.type=CIRCLE_LINE;
			}
			continue;
		}
		int corr=star.corr;
		if(star.corr<95){
				//проверяем соответствие по второму lookup c учетом аффинных преобразований
				corr=OCRStarCorrelationAffineLookup2(starT, star, linesT, letter.line,lineCorr,sMatch);
				DR("result Affine c:"<<corr<<endl;)
				//НОБХОДИМО ПРОВЕРИТЬ чтобы аффинность для пары линий не считалась повторно
		}
		//в зтом месте алгоритма у кластера может быть несколько соответствий
		// предполагаем что кластеры с высокой корреляцией установят соответствие для большинства кластеров
		if(star.corrC>94 || (star.corrC>70 && corr > 70)){
			clusterCorrelation += star.corrC;
			//сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
			*(int*)star.sMatch =*(int*) sMatch; 
			*(int*)star.lineCorr = *(int*)lineCorr; 
			//исключаем из разбора OCRStar текста которые имеют кластерное и геометрическое соответствие
			starT.matchID=letter.letterID; 
			starT.matchStar=star.index; 
			starT.corrC=star.corrC;
			for(int in=0;in<star.lineCount;in++){
				cout<<"star:"<<star.index<<" sMatch:"<<(int)star.sMatch[in]<<endl;
				if(star.sMatch[in]>4)continue; //линия не имеет соответствия
				OCRLine &line=letter.line[star.line[in]];
				if(star.lineCorr[in]>line.corr){
					line.corr=star.lineCorr[in];
					line.match=starT.line[star.sMatch[in]];
				}
				DR("  ###1 write in star:" <<star.index<< " starT:"<< starT.index << " line:" <<line.index << " lineT:"<< line.match << " corr:"<< line.corr<<endl;)
			}
		}
		//на этом этапе не проверяем OCRStar лучей кластера
		DR(" //------ LookUp3 star: " << star.index << " correlation:" << star.corr << endl);
	}
	
	clusterCorrelation /= letter.clusterCount;
	DR("   Lookup 3 correlation:" << clusterCorrelation << " letter.clusterCount:" << letter.clusterCount
										 << endl);
	//if(clusterCorrelation<50)return clusterCorrelation;
	
	//для всех OCRstar эталона для которых не найдено соответствие, проверяем OCRStar текста
	//соответствующие по расположению.
	//соответствие проверяем по lookup2 без проверки по первому lookup
	DR(endl<<"=== Lookup 3 correlation start check all star" << endl);
	int starCount=letter.starCount;
	clusterCorrelation=0;
	for (int index = 0; index < starCount; index++) {
		OCRStar &star=letter.star[index];
		DR("		check star:"<<index << " match:"<<star.match[0]<<" corr:"<<star.corr<<" corrC:"<<star.corrC<<endl;)
		//пропускаем уже проверенные центры кластров, 
		//но проверяем концевые OCRStar кластеров с невысокой  корреляцией
		if(star.match[0]>-1 && star.corrC > 0){
			clusterCorrelation+=star.corrC;
			continue;
		}
		//находим соответствие по координатам
		//star.corr=0; //отменяем результат поиска по кластерам
		int x = (letter.xMatch + star.cPoint.x * letter.scale);
		int y = (letter.yMatch + star.cPoint.y * letter.scale);
		int limit = star.lineW * 3 * letter.scale;
		for (int indexT= 0; indexT < starTCount; indexT++) {
			OCRStar &starT_=starsT[indexT];	
			
			if(starT_.matchID==letter.letterID 
						&& star.matchStar != star.match[0]
						)continue;  //OCRStar текста распознана кластером этой буквы
			DR(">>>>>>>_____check star:"<<index<<" starT:"<<indexT<<endl;)
			//проверяем starT по лимитам координат
			int absX = abs(starT_.cPoint.x - x);
			int absY = abs(starT_.cPoint.y - y);
			cout << "		absX:" << absY << " absY:" << absX <<" limit:" << limit << endl;
			if (absX < limit && absY < limit) {
				//проверяем соответствие по второму lookup c учетом аффинных преобразований
				int corr=OCRStarCorrelationAffineLookup2(starT_, star, linesT, letter.line,lineCorr,sMatch);
				DR("result affine c:"<<corr<<endl;)
				if(corr>=star.corr){
					star.corr=corr;
					star.match[0]=indexT;
					//сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
					*(int*)star.sMatch =*(int*) sMatch; 
					*(int*)star.lineCorr = *(int*)lineCorr; 
					for(int in=0;in<star.lineCount;in++){
						if(star.sMatch[in]>4)continue; //линия не имеет соответствия
						OCRLine &line=letter.line[star.line[in]];
						if(star.lineCorr[in]>line.corr){
							line.corr=star.lineCorr[in]; 
							line.match=starT_.line[star.sMatch[in]];
						}
						DR("  ### write in star:" <<star.index<< " starT:"<< starT_.index << " line:" <<line.index << " lineT:"<< line.match << " corr:"<< line.corr<<endl;)
					}
				}
			}
		}
		clusterCorrelation+=star.corr;
	}
	DR("@@@ Lookup 3 clusterCorrelation:" << clusterCorrelation/starCount << " letter.clusterCount:" << letter.clusterCount
										 << endl<<endl);
	//Подсчитываем корреляцию OCRLine эталона и текста
	clusterCorrelation=0;
	for(int in=0;in<letter.lineCount;in++){
		OCRLine &line=letter.line[in];
		clusterCorrelation+=line.corr;
		DR("  ### check  line:" <<line.index << " lineT:"<< line.match << " corr:"<< line.corr<<endl;)
	}
	clusterCorrelation /= letter.lineCount;
	DR("@@@ Lookup 3 lineCorrelation:" << clusterCorrelation <<  endl<<endl);
										
	return clusterCorrelation;
	*/
	return 0;
	
} //_________________________________________________________

};