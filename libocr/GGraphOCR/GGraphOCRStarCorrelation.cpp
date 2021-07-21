// ____GGraphOCRStarCorrelation.cpp
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
	
/** @bref Функция вычисления масштабонезависмой корреляции двух кластеров*/
int GGraphOCR::clusterCorrelation(OCRCluster &clusterT,OCRCluster &cluster) {
	
	//совпадение центральной OCRStar кластера учитываем как 50 %
	//корреляции кластера. Совпадение OCRStar луча кластера
	//добавляет пропорциональную часть оставшихся 50% корреляции

	int print = 0;
	int clusterCorrelation = 0;
    if(cluster.type==Z_POINT || clusterT.type==Z_POINT){
        if(cluster.type==Z_POINT && clusterT.type==Z_POINT) return 100;
        return 0;
    }

    if(cluster.type==O_POINT || clusterT.type==O_POINT){
        if(cluster.type==O_POINT && clusterT.type==O_POINT){
            return 100;
        }else{
            return 0;
        }
    }
    
	OCRStar &star = cluster.star[0];
	OCRStar &starT = clusterT.star[0];
    if(abs(star.type - starT.type) > 1)return 0;
    
    countStar++;
	int correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
	if (correlation < 100) return 0;
	//DT("correlation1:  star:" << star.index << " c:" << correlation << endl);
    //countMask++;
	//correlation=clusterMaskCorrelation(clusterT.maskC,cluster.maskC);
	//if (correlation < 50) return 0;
	//DT("correlationMask_:  star:" << star.index << " c:" << correlation << endl);
	//cout<<"	//____cluster:"<<cluster.index <<" clusterT:"<<clusterT.index <<endl;
    
    countLineMask++;
	correlation = OCRStarCorrelationLookup2(starT, star, clusterT.line, cluster.line,NULL,NULL,0);
	DT("correlationLookup2:"<<correlation<<endl;)
	if (correlation < 70) return 0;
    //int correlationTest = OCRStarCorrelationLookup1(starT.reg, star.reg);
    //if ( correlationTest < 100) countStar++;
	clusterCorrelation += correlation;
	DT("correlation2:  clusterCorrelation:"<<clusterCorrelation<<" star:" << star.index <<" starCount:"<<(int)star.starCount<< " c:" << correlation << endl);
	
	//вычисляем корреляцию звезд кластера в тестовой буквы относительно найденой
	//первой звезды.
    int starClusterCorrelation=0;
    int reg=0;
    char *regC=(char*)&reg; //в регистре отмечаем проверенные линии
    
	for (int n1 = 0; n1 < star.starCount && n1 < starT.starCount; n1++) {
		//находим соответствие лучей
		int min = 360;
		int minIndex = -1;
		int angle = star.angle[n1];
		for (int m = 0; m < starT.starCount; m++) {
			int dl = abs(starT.angle[m] - angle);
			if (dl >= 325) dl = 360 - dl;
			if (dl < min && !regC[m]) {
				min = dl;
				minIndex = m;
			}
		}
        //if(minIndex<0 || min > 35)return 0; //эта проверка выполняется в lookup1 и lookup2
        regC[minIndex]=1;
		OCRStar &starCluster = cluster.star[star.star[n1]];
		OCRStar &starTCluster = clusterT.star[starT.star[minIndex]];
        if(abs(starCluster.type - starTCluster.type) > 1)return 0;
        
		DT("____: t:" << starTCluster.index << " s:" << starCluster.index << endl);
        countStar++;
        int correlation1 = OCRStarCorrelationLookup1(starTCluster.reg, starCluster.reg);  //в три раза увеличивает количество базовых кластеров
        if (correlation1 < 100) return 0;
        countLineMask++;
        correlation1 = OCRStarCorrelationLookup2(starTCluster, starCluster,clusterT.line, cluster.line,NULL,NULL,0);
		DT("correlation___2: t:" << starTCluster.index << " s:" << starCluster.index << " c:" << correlation1 << endl);

        if (correlation1 < 70) return 0;
        
        //int correlationTest = OCRStarCorrelationLookup1(starTCluster.reg, starCluster.reg);
        //if ( correlationTest < 100) countStar++;
        
		//накапливаем корреляцию звезд кластера
		starClusterCorrelation += correlation1;
	}
	//нормируем корреляцию кластера
	clusterCorrelation =clusterCorrelation/2 + starClusterCorrelation/(cluster.starCount-1)/2;
	DT("//___________cluster:"<<cluster.index<<" clusterT:"<<clusterT.index
		<<"				///// clusterCorrelation:"<<clusterCorrelation<<endl;)
	/*
    if(clusterCorrelation>90){
			int correlation_=clusterMaskCorrelation(starT.maskC,star.maskC);
			cout<<" @c:"<<correlation_<<endl;
			cout<<(int)star.lineCount<<" "<<(int)starT.lineCount<<endl;
			printOCRMask(starT.maskC.mask);
			cout<<endl;
			printOCRMask(star.maskC.mask);
			
			exit(0);
	}
	*/
	return clusterCorrelation;
}
	

	
/** @bref Функция вычисления корреляции двух OCRLetter на основе кластеров и масштабонезависимых lookup 1-2 */
void GGraphOCR::letterClusterCorrelation(OCRLetter &letterTest, OCRLetter &letter) {
	/**
	Алгоритм август 2020 года
	 Вычисление корреляции двух множеств OCRStar и OCRLine осуществляется последовательным применением
	 масштабо-зависимых и масштабо-независимых функций корреляции - lookup.
	 На первом этапе применяем масштабо-независую корреляцию по хешу типов OCRStar и lookup
	 соответствия углов фокальных и опорных линий звездочки - @fn OCRStarCorrelationLookup1

	 На втором этапе для для всех найденных соответствий OCRStar применем масштабо-незвисимые  lookup
	 @fn OCRStarCorrelationLookup2
	 и @fn OCRStarCorrelationLookup3 для нахождения соответствия устойчивых Т и Х типов OCRStar.
	 Устойчивыми считаем OCRStar Т, Х и C типов все лучи которых имеют длину больше двойной средней толщины линии буквы
	эталона.

	 При выполнеии корреляции второго этапа тип совпадение типа звездочки учитывается только в положительную сторону.
	 Все звездочки которые совпали по координатам передаются на проверку соответствия формы фокальных линий
	 в @fn OCRStarCorrelationLookup3.
	 Для выполнения корреляции второго этапа для всех точки экстремума фокальных линий эталона создаются OCRStar
	 типа P_POINT
	 При вычислении хеша неустойчивые T_POINT и X_POINT приравниваются к типу P_POINT
	 Поскольку точки экстремума и неустойчивые T_POINT и X_POINT имеют один тип, то при создании масок 32х32 маска
	 лучей OCRStar строится до первого экстремума.

	 Для того чтобы установить соответствие устойчивых OCRStar выполняем lookup 2 и 3 в таком порядке:
	 1. Для самой устойчивой OCRStar эталона находим в тексте соответствия. Для каждого соответствия произвоим проверку
	 корреляции OCRStar которые находятся на концах фокальных линий. Усредняем корреляцию полученного кластера.
	 Ищем следующий кластер относительно масштаба и геометрии первого найденного кластера.
	 Этот алгоритм не такой гибкий как алгоритм применения поисковых регистров углов.
	 применение принципа кластеров позволяет при распознавании рукописного текста учесть афинные преобразования.
	 Для этого при проверке корреляции второго этапа необходимо начинать проверку
	 с OCRStar текста которые находятися на минимальном расстоянии от устойчивых OCRStar эталона соответствие которых
	 в тексте было найдено на первом этапе. Проверку следующих OCRStar проводим от относительных координат найденных
	соответствий. Такое проследовательное применение сдвига координат позволяет учесть 5-10% афинные преобразования
	текста относительно эталона.

	 Буквы базы данных которые не имеют устойчивых  OCRStar Т, Х и С типов проверяем в соответствии с гипотезой масштаба
	соседних букв. Все буквы базы данных необходимо привести к одному масштабу.
	*/
	//int print = 1;
/*
	OCRStar *starsT = letterTest.star;
	int letterTStarCount = letterTest.starCount;
	uchar sMatch[4]; ///< порядок номеров линий OCRStar текста соответствующих кластеру OCRStar буквы
	uchar sMatch_[4]; ///< буферный массив
	uchar corr[4]; ///<значения корреляции OCRStar текста соответствующих кластеру OCRStar буквы
	uchar lineCorr[4]; ///<значения корреляции OCRLine текста соответствующих кластеру OCRStar буквы
	uchar lineCorr_[4]; ///< буферный массив
	int limit;
	int letterStarCount = letter.starCount;
	int letterLineCount = letter.lineCount;
	int letterID=letter.letterID;

	for (int index = 0; index < letterStarCount; index++) { //для каждой OCRStar буквы
		OCRStar &star = letter.star[index];
		star.corr = 0;
		star.corrC = 0;
		star.match[0] = -1;
	}
	for (int index = 0; index < letterLineCount; index++) { //для каждой OCRStar буквы
		OCRLine &line = letter.line[index];
		line.corr = 0;
		line.match= -1;
	}
	int clusterCount = letter.clusterCount;
	//совпадение центральной OCRStar кластера учитываем как 50 %
	//корреляции кластера. Совпадение OCRStar луча кластера
	//добавляет пропорциональную часть оставшихся 50% корреляции

	for (int index = 0; index < clusterCount; index++) { //для каждого кластера буквы

		cout <<endl<< "@@@//_____star:" << (int)letter.cluster[index] << " c:" << (int)letter.star[letter.cluster[index]].corr
			 << " index" << index << endl;
		// if(match.starTIndex!=4)continue;
		OCRStar &star = letter.star[letter.cluster[index]];
		int maxCluster = 0;
		int maxClusterIndex = 0;
		//среди кластеров тестовой буквы ищем соответствующий кластер
		for (int index1 = 0; index1 < letterTStarCount; index1++) { //для каждой OCRStar области поиска

			OCRStar &starT = starsT[index1];
			if(starT.matchID==letterID && star.corrC>90)continue;
			// OCRStar уже имеет соответствие
*/
/*
			OCRLine &line_ = letter.line[0];
			OCRLine &lineT_ = letterTest.line[0];
			string dataStr;
			drawOCRLineAffineCorrelationSVG(lineT_, line_, letterTest.point, letter.point, 0, dataStr);
			writeText(dataStr, "/storage/emulated/0/_Image2OCR/___graphLineAffineCorrelation.html");
			int rc = OCRLineMaskAffineCorrelation(lineT_, line_, 0);

			cout << "rc:" << rc << endl;
			exit(0);
*/
/*
			cout << "star:" << star.index << " starT:" << starT.index << endl;
			//вычисляем корреляцию двух кластеров
			int clusterCorrelation = 0;
			if (star.type == O_POINT) {
				//круги проверяем только на их количество в букве
				if (starT.type == O_POINT) {
					for (int index4 = 0; index4 < star.starCount; index4++) {
						letter.star[star.star[index4]].corr = 100;
					}
					star.corrC = 100;
					star.match[star.matchCount] = starT.index;
					star.matchCount++;
					if (star.matchCount == 4) star.matchCount = 3;
					DT("circle match" << endl;)
					clusterCorrelation = 100;
					break;
				} else {
					continue;
				}
			}
			//вычисляем масштабонезависимую корреляцию звездочек
			int correlation = OCRStarCorrelationLookup1(starT.reg, star.reg);
			if (correlation < 70) continue;
			DT("____correlation1:  star:" << star.index << " starT:" << index1 << " c:" << correlation << endl);
			correlation = OCRStarCorrelationLookup2(starT, star, letterTest.line, letter.line, lineCorr, sMatch,1);
			if (correlation < 70) continue;
			DT("____correlation2:  star:" << star.index << " starT:" << index1 << " c:" << correlation << endl);
			clusterCorrelation += correlation * 100;
			//вычисляем корреляцию звезд кластера в тексте относительно найденой
			//первой звезды.
			limit = star.starCount;
			if (limit > starT.starCount) limit = starT.starCount;
			for (int n1 = 0; n1 < limit; n1++) {
				if(sMatch[n1]>4)continue; //линия не имеет соответствия
				corr[n1] = 0;
				OCRStar &starCluster = letter.star[star.star[n1]];
				cout<<" star.star[n1]:"<<star.star[n1]<<" sMatch[n1]:"<<(int)sMatch[n1]<<endl;
				cout<<" starT.star[sMatch[n1]]:"<<(int)starT.star[sMatch[n1]]<<endl;
				OCRStar &starTCluster = starsT[starT.star[sMatch[n1]]];
				int correlation1 = OCRStarCorrelationLookup1(starTCluster.reg, starCluster.reg);
				DT("_______1_correlation___1: star:" << starCluster.index << " starT:" << starTCluster.index
													<< " c:" << correlation1 << endl);
				if (correlation1 < 70) continue;
				correlation1 = OCRStarCorrelationLookup2(starTCluster, starCluster, letterTest.line, letter.line, NULL,NULL,0);
				DT("_______1_correlation___2: star:" << starCluster.index << " starT:" << starTCluster.index
													<< " c:" << correlation1 << endl);
				if (correlation1 < 70) continue;
				//накапливаем корреляцию звезд кластера
				clusterCorrelation += correlation1 * star.line_weight[n1];
				corr[n1] = correlation1;
				//записываем максимальную корреляцию OCRStar кластера
				if (correlation1 > starCluster.corr) {
					starCluster.corr = correlation1;
					starCluster.match[0] = starTCluster.index;
				}
			}
			//нормируем корреляцию кластера
			//вес корреляции центра кластера  в общей корреляции
			//кластера равен 50%
			//суммарный вес корреляции лучей кластера  в общей корреляции
			//кластера равен 50%
			clusterCorrelation /= 200;
			cout << "@star:" << star.index << " clusterCorrelation:" << clusterCorrelation << endl;
			//сохраняем максимальную корреляцию кластера
			if (clusterCorrelation > maxCluster) {
				maxCluster = clusterCorrelation;
				maxClusterIndex = index1;
				star.corr = correlation;
				star.corrC = clusterCorrelation;
				star.match[0] = index1; //сохраняем индекс звездочки с максимальной корреляцией
				star.matchCount = 1;
				//сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
				*(int*)star.sMatch =*(int*) sMatch; 
				//сохраняем значения корреляции OCRStar на концах фокальных линий  OCRStar текста и эталона
				*(int*)star.cMatch = *(int*)corr; 
				//сохраняем значения корреляции OCRLine  OCRStar центра кластера эталона 
				//(используеься в масштабозависимых  lookup при взаимной корреляции OCRLine)
				*(int*)star.lineCorr = *(int*)lineCorr; 
			}
		}
		if(star.match[0]>-1){
			OCRStar &starT = starsT[star.match[0]];
			starT.matchID = letterID;
			starT.corrC = star.corrC;
			starT.matchStar=star.index;
			cout << "@@@ result: star.index:" << star.index << " corrC:" << star.corrC << " star.match[0]" << star.match[0]
				 << " level:" << (int)star.level << endl
				 << endl;
		}
	}

	//вычисляем результат корреляции буквы по масштабо-независимым lookup
	int letterCorrelation = 0;
	int mLen = 0;
	for (int index = 0; index < letterStarCount; index++) { //для каждой OCRStar буквы
		OCRStar &star = letter.star[index];
            //cout<<index<<" c:"<<star.corrC<<" c1:"<<star.corr<<endl;
		if (star.corrC) {
			letterCorrelation += star.corrC * star.mLen;
		} else {
			letterCorrelation += star.corr * star.mLen;
		}
		mLen += star.mLen;
	}
	letterCorrelation = letterCorrelation / mLen;
	if (letterCorrelation > 100) letterCorrelation = 100;

	DT("=== letter correlation:" << letterCorrelation << endl << endl);
	//return;
	//если буква имеет достаточно высокую корреляцию проверяем масштабо-зависимые lookup
	if (letterCorrelation > 50) {
		detectScale(starsT, letter);
		letterCorrelation = OCRStarCorrelationLetterLookup3(letterTest, letter);
		//если буква имеет корреляцию выше 96 % прекращаем проверку
		if (letterCorrelation > 96) {
			letter.correlation = letterCorrelation;
			return;
		}
		
			if (letter.correlation > 50) {
				letter.correlation = OCRLineCorrelationLookup4(letterTest,letter);
			}
	} else {
		letter.correlation = 0;
		return;
	}
*/
} //____________________________________________________________________________

/** @bref Функция вычисления корреляции двух OCRStar  */
int GGraphOCR::OCRStarCorrelation(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines) {
	/**

	МАСШТАБО НЕЗАВИСИМЫЕ lookup
	ПЕРВЫЙ lookup кореляции звездочек.
	На первом этапе звездочки сравниваются по хешу
	Описание алгоритма вычисления хеша в функции @fn setOCRStarHash

	ВТОРОЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение звездочек по концам лучей в нормированной маске 32х32.

	ТРЕТИЙ lookup кореляции звездочек.
	Сравнение звездочек по кривизне лучей на основе точек экстремума фокальных линий в
	нормированной маске 32х32
	 */
	cout << "hash:" << star.hash << " _ " << starT.hash << endl;
	if (star.hash != starT.hash) return 0;
	int correlation = OCRStarCorrelationLookup2(star, starT, linesT, lines,NULL,NULL,0);

	return correlation;
} //_________________________________________________________

/** @bref ПЕРВЫЙ lookup кореляции звездочек @fn OCRStarCorrelation
	по вычислению корреляции углов фокальных и опорных линий OCRStar */
int GGraphOCR::OCRStarCorrelationLookup1(OCRRegister &regT, OCRRegister &reg) {
	/**
	МАСШТАБО НЕЗАВИСИМЫЙ lookup
	В этом lookup сравниваются на совпадение углы множеств фокальных и опорных линий OCRStar.
	Углы фокальных линий звездочки вычисляем как углы лучей соединяющих центр OCRStar и
	точку конца лимитированной фокальной линии.
	Лимитирование длины фокальных линий при вычислении корреляции увеличивает вариабельность
	эталонов букв и сокращает влияние шумовых пересечений и разрывов фокальных линий.
	Для длинных кривых существует случаи когда две различные кривые исходящие из одной OCRStar
	имеют одинаковые углы или углы не совпадающие с углом под которым линия исходит из OCRStar 
    лимитирование нормирует углы исходящих из OCRStar кривых.
	При вычислении корреляции углов коротких линий в 1-5 пикселов учитываем неопределённость углов
	таких линий. Для вычисления корреляции используем два регистра построенных с учётом и без учёта
	коротких линий с неопределённым углом. В итоговой корреляции используем наилучщее значение регистра.
	Опорной линией считаем считаем луч проведенный из центра OCRStar до другой OCRStar с которой нет прямого
	соединения фокальной линиией. Опорные линии проводим до центров OCRStar которые находятся по 
    координатам в пределах 2 средних длин линии OCRStar.
	Для вычисления корреляции фокальных и опорных линий OCRStar все углы приводим к диапазону значений 0-64
	Поисковый регистр длинной 64 бита заполняем значениями углов фокальных и опорных линий приведенных к 
    диапазону значений 0-64. Для вычисления корреляции для всех OCRStar текста создаем базовые регистры
	фокальных и опорных линий OCRStar. В базовом регистре длинной 64 бита заполняем пределы лимита поиска угла
	фокальных и опорных линий OCRStar. Логическое сложение базового и поискового регистра позволяет
	одновременно вычислить количество совпадений всех углов фокальных и опорных линий OCRStar эталона и 
    текста в заданных лимитах значений. Логическое умножение двух базовых регистров позволяет вычислить 
    суммарную величину дисперсии расхождения значений углов фокальных и опорных линий OCRStar эталона и текста.
	Кольцевой сдвиг базового регистра позволяет вычислить относительный угол поворота звездочек.
	Для этого необходимо для каждого значения кольцевого сдвига регистра логическим умножением двух базовых регистров
	найти минимум суммарной величины дисперсии расхождения значении углов фокальных и опорных линий OCRStar эталона и
	текста. Значение кольцевого сдвига найденного минимума будет соответствовать относительному углу поворота OCRStar
	приведенному к диапазону 0-64 (точность 17%).
     
	При необходимости повысить точность можно использовать регистр большей длины.
	Алгоритм также позволяет произвести индивидуальную проверку совпадения лучей.

	При заполнении базового регистра лимиты углов в начале и конце регистра заполняются с кольцевым сдвигом значений
	регистра. При этом лимиты выходящие за предел конца регистра переносятся на противоположны конец регистра. Это
	позволяет выполнить проверку соответствия углов лимиты которых переходят за нулевое значение начала отсчета углов
	0-360.

	Для вычисления корреляции по углам фокальных и опорных линий OCRStar для проверки и построения поискового и
	базового регистра используем OCRStar типов X_POINT, T_POINT, P_POINT и O_POINT

     uint64 reg0_0;		///<поисковый регистр углов лимитированных фокальных линий с учётом коротких линий
     uint64 reg0_1;		///<регистр маски лимитов углов лимитированных фокальных линий с учётом коротких линий
     uint64 reg1_0;		///<поисковый регистр углов лимитированных фокальных линий без учёта коротких линий
     uint64 reg1_1;		///<регистр маски лимитов углов лимитированных фокальных линий без учёта коротких линий
     uint64 reg2_0;		///<поисковый регистр углов опорных линий uint64 reg2_1;		
                        ///<регистр маски лимитов углов опорных линий
	*/
	
    //Проверка типов обоих OCRStar на совпадение.//
    
	//Если не совпадает количество единиц в обоих регистрах, это значит что не совпадает
    //количество линий в проверяемых OCRStar, в этом случае регистры не проверяем.
	if(reg.nReg0 != regT.nReg0 && reg.nReg1 != regT.nReg1)return 0;
	
    //Проверка углов обоих OCRStar на совпадение.//
    
    //вычисляем поисковый регистр углов лимитированных фокальных линий с учётом коротких линий
	uint64 reg0 = regT.reg0_1 & reg.reg0_0;
	//вычисляем корреляцию Lookup1
    if (reg0 == reg.reg0_0) return 100;
	
    //вычисляем поисковый регистр углов лимитированных фокальных линий без учёта коротких линий
	uint64 reg1 = regT.reg1_1 & reg.reg1_0;
    //вычисляем корреляцию Lookup1
    if (reg1 == reg.reg1_0) return 100;
    
	//вычисляем поисковый регистр углов опорных линий
	//uint64 reg2 = regT.reg2_1 & reg.reg2_0;
    
	//вычисляем корреляцию Lookup1
	// if ((reg0 == star.reg0_0 || reg1 == star.reg1_0) && reg2 == star.reg2_0)return 100;
    ///	if (reg0 == reg.reg0_0 || reg1 == reg.reg1_0) return 100;

	return 0;

} //_________________________________________________________



/** @bref ПЯТЫЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение формы штрихов исходного изображения соответствущих фокальным линиям лучей
   звездочек. */
int GGraphOCR::OCRStarCorrelationLookup5(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines) {
	/**
		Вычисление корреляции совпадения формы штрихов выполняется как окончательная проверка
	   совпадения формы множеств черных пикселов текста и эталонной буквы. Вычисление множеств
	   черных пикселов фокальных линий производится в функци @fn attributeLinePixels Проверка
	   корреляции производится масштабированием и совмещением коррдинат центров фокальных
	   линий. Каждое множество пикселов масштабируется до размера битовой маски GBitMask32 и
	   создаются области On и Off каждой маски. Алгоритм создания такой же как при создании
	   маски признака буквы. Вычисление корреляции производится аналогично вычислению
	   корреляции признака буквы без вычисления оптимального взаимного положения масок.
		Подробное описание процесса корреляции масок в функции @fn
	   GBitsetMatrix::letterCorrelation

	*/
    
	return 0;
} //_________________________________________________________

/** @bref Детектор масштаба буквы относительно текста. */
void GGraphOCR::detectScale(OCRStar *starsT, OCRLetter &letter) {
	/** Расчет относительного масштаба эталона и текста на основе
	 среднего (мат. ожидания) и дисперсии между одинаковыми по коэффициенту корреляции
	 фокальными линиями текста и эталона. */

	int print = 1;
	float dispM = 0; // среднее (математическое ожидание) масштаба dispersion
	float avgM = 0;	 // дисперсия масштаба average
	float dispA = 0; // среднее (математическое ожидание) угла поворота dispersion
	float avgA = 0; // дисперсия угла поворота average

	GStr<float> xM;
	GStr<float> xA;
	GStr<float> xC;
	GStr<float> yC;

	// Массивы фиксированной длинны.
	// Если количество звездочек совпавших по коэффициенту корреляции больше размера массива,
	// то их просто не используем.
	int starN = 0; // количество распознанных звездочек буквы
	// short xM[]={17, 15, 23, 7, 9, 13};   // массив масштаба звездочек совпавших по коэффициенту корреляции
	// short xA[]={34, 30, 46, 14, 18, 26}; // массив угла поворота звездочек совпавших по коэффициенту корреляции
	int clusterCount = letter.clusterCount;
	xM.resize(clusterCount);
	xA.resize(clusterCount);
	xC.resize(clusterCount);
	yC.resize(clusterCount);

	for (int n = 0; n < clusterCount; n++) {
		OCRCluster &cluster = font.clusterArray[letter.cluster[n]];
		OCRStar &star = cluster.star[0];
		if (star.corrC > 70) {
			float scale;
			OCRStar &starT = starsT[star.match[0]];
			if (star.type == O_POINT) {
				scale = (float)starT.radius / star.radius;
				xM[star.index] = scale;
				star.scale = scale;
				starN++;
			} else {
				scale = (float)starT.sumLen1 / star.sumLen1;
				DR("starT:" << starT.index << " s:" << starT.sumLen << " star:" << star.index << " s:" << star.sumLen
							<< " scale" << scale << endl;)
				star.scale = scale;
				xM[star.index] = scale;
				starN++;
				/*
				//суммируем масштаб всех OCRStar кластера (ухудшает результат)
				for(int m=0; m < star.starCount; m++){
					OCRStar &star1=letter.star[star.star[m]];
					OCRStar &starT1=graph.starArray[starT.star[star.sMatch[m]]];
					float scale=(float)starT1.mLen/star1.mLen;
					xM[star1.index]=scale;
					DR(" s:"<<star1.index<<" t:"<<starT1.index<<" scale:"<<scale<<endl);
					starN++;
				}
				*/
			}
			xC[star.index] = starT.cPoint.x - star.cPoint.x * scale;
			yC[star.index] = starT.cPoint.y - star.cPoint.y * scale;
		}
	}
	if (print) {
		for (int i = 0; i < clusterCount; i++) cout << i << "__   " << xM[i] << " x:" << xC[i] << " y:" << yC[i] << endl;
	}
	// Вычисление среднего значения.
	// Можно получить в момент подсчета коэффициента корреляции и использовать здесь.
	float avg_M = 0;
	float avg_A = 0;
	for (int i = 0; i < clusterCount; i++) {
		avg_M += (float)xM[i];
		avg_A += xA[i];
	}

	if (starN < 2) starN = 2; // ниже делим на starN-1
	avgM = (float)avg_M / starN;
	avgA = (float)avg_A / starN;

	// Вычисление дисперсии.
	for (int i = 0; i < clusterCount; i++) {
		if (!xM[i]) continue;
		dispM += (xM[i] - avgM) * (xM[i] - avgM);
		dispA += (xA[i] - avgA) * (xA[i] - avgA);
	}

	dispM = sqrt(dispM / (starN - 1));
	dispA = sqrt(dispA / (starN - 1));

	// Правило шести сигм — практически все значения нормально распределённой случайной величины
	// лежат в интервале от avg-3*disp до avg+3*disp. Поэтому отбрасываем все что не попадает в этот интервал
	// и заново подсчитываем среднее.

	// Выполнение правила шести сигм.
	float dltM1 = avgM - 3 * dispM;
	float dltM2 = avgM + 3 * dispM;
	float dltA1 = avgA - 3 * dispA;
	float dltA2 = avgA + 3 * dispA;
	int starM = 0; // количество звездочек попадающих в интервал шести сигм по масштабу
	int starA = 0; // количество звездочек попадающих в интервал шести сигм по углу поворота

	// Вычисление нового среднего значения без самых больших и самых маленьких значений.
	avg_M = 0;
	avg_A = 0;
	int avg_X = 0;
	int avg_Y = 0;
	for (int i = 0; i < clusterCount; i++) {
		if (!xM[i]) continue;
		float m = xM[i];
		float a = xA[i];
		if (m >= dltM1 && m <= dltM2) {
			avg_M += m;
			avg_X += xC[i];
			avg_Y += yC[i];
			starM++;
		}
		if (a >= dltA1 && a <= dltA2) {
			avg_A += a;
			starA++;
		}
	}
	if (starM < 1) starM = 1;
	avgM = (float)avg_M / starM; // Средний масштаб всего текста
	if (starA < 1) starA = 1;
	avgA = (float)avg_A / starA; // Средний угол поворота всего текста

	// letter.xCenter = avg_X / starM;
	// letter.yCenter = avg_Y / starM;

	// avgA=(float)10/3;
	DR("avgM:" << avgM << " avgA:" << avgA << " dispM:" << dispM << " dispA:" << dispA << "  starM:" << starM
			   << " starA:" << starA << endl);
	letter.scale = avgM;

	//вычисляем усредненное значение центра гипотезы буквы

} //_________________________________________________________
    

/** @bref Детектор масштаба буквы относительно текста на основе корреляции базовых кластеров. */
void GGraphOCR::detectScaleCluster(OCRLetter &letter) {
    /** Расчет относительного масштаба эталона и текста на основе
     среднего (мат. ожидания) и дисперсии между кластерами текста и эталона 
     совпадающими по корреляции с базовым кластером */
    
    int print = 0;
    float dispM = 0; // среднее (математическое ожидание) масштаба dispersion
    float avgM = 0;	 // дисперсия масштаба average
    //float dispA = 0; // среднее (математическое ожидание) угла поворота dispersion
    //float avgA = 0; // дисперсия угла поворота average
    
    GStr<float> xM;
    GStr<float> xA;
    GStr<float> xC;
    GStr<float> yC;
    
    // Массивы фиксированной длинны.
    // Если количество звездочек совпавших по коэффициенту корреляции больше размера массива,
    // то их просто не используем.
    int starN = 0; // количество распознанных звездочек буквы
    // short xM[]={17, 15, 23, 7, 9, 13};   // массив масштаба звездочек совпавших по коэффициенту корреляции
    // short xA[]={34, 30, 46, 14, 18, 26}; // массив угла поворота звездочек совпавших по коэффициенту корреляции
    //int clusterCount = letter.clusterCount;
    uint clusterCount = (uint)letter.matchCount;
    xM.resize(clusterCount);
    xA.resize(clusterCount);
    xC.resize(clusterCount);
    yC.resize(clusterCount);

    for (int n = 0; n < clusterCount; n++) {
        ClusterMatch &clusterMatch = letter.match[n];
        if(clusterMatch.cPoint.type==Z_POINT)continue;
        if (clusterMatch.corr) {
            xM[starN] = clusterMatch.scale;
            starN++;
            //yC[n] = ;
        }
    }
    if (print) {
        for (int i = 0; i < starN; i++) cout << i << "__   " << xM[i] << endl;
    }
    // Вычисление среднего значения
    float avg_M = 0;
    //float avg_A = 0;
    for (int i = 0; i < starN; i++) {
        avg_M += (float)xM[i];
        //avg_A += xA[i];
    }
    
    if (starN < 2) starN = 2; // ниже делим на starN-1
    avgM = (float)avg_M / starN;
    //avgA = (float)avg_A / starN;
    
    // Вычисление дисперсии.
    for (int i = 0; i < clusterCount; i++) {
        if (!xM[i]) continue;
        dispM += (xM[i] - avgM) * (xM[i] - avgM);
       // dispA += (xA[i] - avgA) * (xA[i] - avgA);
    }
    
    dispM = sqrt(dispM / (starN - 1));
    //dispA = sqrt(dispA / (starN - 1));
    
    // Правило шести сигм — практически все значения нормально распределённой случайной величины
    // лежат в интервале от avg-3*disp до avg+3*disp. Поэтому отбрасываем все что не попадает в этот интервал
    // и заново подсчитываем среднее.
    
    // Выполнение правила шести сигм.
    float dltM1 = avgM - 3 * dispM;
    float dltM2 = avgM + 3 * dispM;
    //float dltA1 = avgA - 3 * dispA;
    //float dltA2 = avgA + 3 * dispA;
    int starM = 0; // количество звездочек попадающих в интервал шести сигм по масштабу
    //int starA = 0; // количество кластеров попадающих в интервал шести сигм по углу поворота
    
    // Вычисление нового среднего значения без самых больших и самых маленьких значений.
    avg_M = 0;
    //avg_A = 0;
    //int avg_X = 0;
    //int avg_Y = 0;
    for (int i = 0; i < clusterCount; i++) {
        if (!xM[i]) continue;
        float m = xM[i];
        //float a = xA[i];
        if (m >= dltM1 && m <= dltM2) {
            avg_M += m;
            //avg_X += xC[i];
            //avg_Y += yC[i];
            starM++;
        }
        //if (a >= dltA1 && a <= dltA2) {
        //    avg_A += a;
        //    starA++;
        //}
    }
    if (starM < 1) starM = 1;
    avgM = (float)avg_M / starM; // Средний масштаб всего текста
    //if (starA < 1) starA = 1;
    //avgA = (float)avg_A / starA; // Средний угол поворота всего текста
    
    // letter.xCenter = avg_X / starM;
    // letter.yCenter = avg_Y / starM;
    
    // avgA=(float)10/3;
    //DR("avgM:" << avgM << " avgA:" << avgA << " dispM:" << dispM << " dispA:" << dispA << "  starM:" << starM
    // << " starA:" << starA << endl);
    letter.scale = avgM;
    
    //вычисляем усредненное значение центра гипотезы буквы
    
} //_________________________________________________________


} // namespace ocr

/**

ПРАКТИЧЕСКАЯ РЕАЛИЗАЦИЯ. Исходный текст

	 1. ТИПЫ ЗВЕЗДОЧЕК:
	 Определение типов звездочек производится по типу центральной фокальной точки и вычислению
хеша в функции setOCRStarHash.

	 2. ОТНОСИТЕЛЬНЫЕ ДЛИННЫ ЛУЧЕЙ:
	 // Вычисление меры длинны линии по отношению к сумме длин линий и периметра звездочки по
концу линии или ближайшему экстремуму. //

	 3. ОТНОСИТЕЛЬНЫЕ УГЛЫ - инвариантны с скусыванию-раскусыванию. :
	 // Вычисление относительных углов между парами линий. //

	 4. ЩЕДРЫЕ ЛИМИТЫ:
	 Однако эти алгоритмы (п.п. 2 и 3) не инвариантны с скусыванию-раскусыванию.
	 Если после выполнения этих алгоритмов, по запросу (с известной гипотезой масштаба)
применить "щедрые лимиты", то решается и этот вопрос.
	 // Функция поиска координат фокальных точек. // в прямоугольнике

	 5. КОЭФФИЦИЕНТ КОРРЕЛЯЦИИ ПО ЛУЧАМ:
	 // Расчет коэффициент корреляции звездочки текста и эталона по относительным длинам лучей
в %. // Перекачивание между локальными коэффициентами корреляции при вычислении суммарного
коэффициента корреляции звездочки текста и эталона. Додумать и написать.

	 6. КОЭФФИЦИЕНТ КОРРЕЛЯЦИИ ПО УГЛАМ:
	 Подсчет коэффициента корреляции звездочек без учета кривизны выходящих линий (по выходящим
прямым линиям). По этим алгоритмам можно вычислять соответствующие коэффициенты корреляции.
	 // Определение коэффициента корреляции звездочки по относительным углам в % с вращением
влево и вправро на одну позицию. //

	 7. КОЭФФИЦИЕНТ КОРРЕЛЯЦИИ ПО КРИВЫМ:
	 Подсчет коэффициента корреляции звездочек с учетом кривизны выходящих линий (с учетом
звездочек-экстремумумов) сводится к алгоритмам вычисленич коэффициента корреляции локальных
созвездий. Для локальных созвездий "кольчугу" не применяем (для нее нужен масштаб).

	 8. МАСШТАБ.
	 Распознование по звездочкам (без учета кривизны или с ее учетом) позволяет сразу получить
масштаб буквы.
	 // Вычисление масштаба звездочки текста по сумме длин истинной (пиксельной) кривых
образующих звездочку и периметр звездочки.


*/
