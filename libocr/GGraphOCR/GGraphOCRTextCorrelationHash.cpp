//__GGraphOCRTextCorrelationHash.cpp
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
#include <unistd.h>

namespace ocr {

int testMatch(OCRLetter &letter, int x, int y) {

	if (letter.xMatch > x + letter.letterW / 2 || letter.xMatch < x - letter.letterW / 2 || letter.yMatch > y + letter.letterH / 2 || letter.yMatch < y - letter.letterH / 2)
		return 0;

	return 1;
}

/** @bref Функция поиска гипотез буквы в тексте
 Выполняется проверка базовых кластеров базы букв со всеми OCRStar текста
 с проверкой совпадения хеша базовой OCRStar кластера и текста
 */
void GGraphOCR::textCorrelationHash(GGraph &graph, vector<OCRLetter> &matchLetter, int correlationLimit) {

	//int print = 0;
/*
	//открываем таблицу кластеров записаную в последней записи fontClusterTable
	ulong size1;
	ClusterMatch *clusterMatchArray = (ClusterMatch *)fontClusterTable->getPtr(fontClusterTable->size() - 1, size1);
	uint matchLineSize = (uint)size1 / sizeof(ClusterMatch);

	//создаем таблицу корреляции фокальных линий буквы
	letterLineReg.resize(graph.focalLine.size());

	//выполняем построение первой хеш таблицы текста
	vector<vector<int> > hashArray;
	hashArray.resize(256);

	int sizeS = (int)graph.starArray.size();
	for (int i = 0; i < sizeS; i++) {
		OCRStar &star = graph.starArray[i];
		hashArray[star.hash].push_back(i);
	}
*/	
	/*
		//тест
	   //читаем все кластеры буквы
	   OCRLetter lt;
	   readLetterByID(lt, 1576660881);
	   ulong size21;
	   ClusterMatch *letterCluster=(ClusterMatch*)letterClusterTable->getPtr(lt.letterIndex, size21);
	   size21/=sizeof(ClusterMatch);
	   for(int index=0;index<size21;index++){
		   ClusterMatch &match=letterCluster[index];
		   cout<<(int)match.w<<" "<<(int)match.h<<" letterIndex:"<<match.letterIndex<<
		   " cluster:"<<(int)match.clusterID<<" starT:"<<(int)match.starTIndex<<" name:"<<lt.name.data<<endl;
	   }
   */
/*
	int count1 = 0;
	//размечаем весь текст кластерами базы данных, строим хеш таблицу соответствия кластеров базы и текста
	//и таблицу соответствия OCRStar текста и базовых кластеров
	textCluster.resize(matchLineSize);
	text2Cluster.resize(graph.starArray.size());
	for (int i = 0; i < matchLineSize; i++) {
		ClusterMatch &match = clusterMatchArray[i]; //поиск начинаем с базового кластера имеющего наибольший габарит буквы в которой он найден
		//выполняем поиск этого кластера по OCRStar текста соответствующих по хешу.
		//читаем из базы букву содержащую кластер
		OCRLetter letter;
		readOnlyLetter(letter, match.letterIndex);
		OCRStar &star = letter.star[match.starIndex];
		// cout<<(int)match.w<<" "<<(int)match.h<<" letterIndex:"<<letter.letterID<<
		//" cluster:"<<(int)match.clusterID<<" starT:"<<(int)match.starTIndex<<" name:"<<letter.name.data<<endl;
		// if(match.clusterID==8777){
		//    cout<<endl;
		//}
		vector<int> &line = hashArray[star.hash];
		printCorrelation = 0;
		for (int index = 0; index < line.size(); index++) {

			OCRStar &starT = graph.starArray[line[index]];
			int clusterCorrelation = letterCusterCorrelation(graph, letter, starT, star);
			count1++;
			if (clusterCorrelation > correlationLimit) {
				DR("match letterID:" << letter.letterID << " match:" << (int)match.clusterID << " starT:" << starT.index << " star:" << (int)star.index
									 << " c:" << clusterCorrelation << " name:" << letter.name.data << endl);
				//добавляем в строку таблицы размещенную по индексу кластера ID номер OCRStar текста
				textCluster[match.clusterID].push_back(starT.index);
				text2Cluster[starT.index].push_back(match.clusterID);
			}
		}
	}

	//таблица textCluster теперь содержит кластеры текста которые размещены
	//в строке по индексу соответствующего кластера базы данных
	//это позволяет быстро найти все кластеры текста соответствующие заданому.

	//еще раз читаем таблицу clusterMatchArray. это таблица кластеров базы данных сортированная по габариту наибольшей буквы
	//находим в тексте все гипотезы букв соответствующих базовым кластерам.
	//при нахождении устойчивой гипотезы буквы исключаем ее OCRStar из разбoра.
	int count = 0;

	vector<int> v;

	// TIME_START

	for (int i = 0; i < matchLineSize; i++) {
		ClusterMatch &match = clusterMatchArray[i];
		//выполняем поиск этого кластера по OCRStar текста соответствующих по индексу базового кластера.
		//читаем из базы букву содержащую кластер
		//поскольку clusterMatchArray сортирован по признаку наибольшего габарита буквы,
		//проверка корреляции букв начинаестя с самой большой гипотезы.

		vector<int> &line = textCluster[match.clusterID];
		int textClusterLineSize = (int)line.size();
		for (int index = 0; index < textClusterLineSize; index++) {
			OCRStar &starT = graph.starArray[line[index]];

			if (starT.corrC > 95) continue; // OCRStar уже участвует в устойчивой корреляции гипотезы буквы

			//поскольку текст уже размечен базовыми кластерами то starT и star имеют высокую корреляцию.
			//теперь нужно проверить на этом месте гипотезы всех букв которые содержат данный кластер.
			//для этого стоим на этом месте габарит наибольшей гипотезы буквы данного кластера и
			//находим все кластеры текста в пределах этого габарита.
			vector<int> searchStar;

			//читаем базовый кластер из таблицы кластеров
			ulong size1;
			ClusterMatch *matchLine = (ClusterMatch *)fontClusterTable->getPtr(match.clusterID, size1);
			ClusterMatch &match_ = matchLine[0];
			//читаем букву кластера из базы букв
			OCRLetter letterTest;
			readLetter(letterTest, match_.matchIndex);
			// cout<<" name:"<<letterTest.name.data<<" ID:"<<letterTest.letterID<<endl;
			OCRStar &star = letterTest.star[match.starTIndex];
			//вычисляем габарит буквы.
			//сначала вычисляем масштаб гипотезы буквы в тексте
			float scale;
			if (star.type == O_POINT) {
				scale = (float)starT.radius / star.radius;
			} else {
				scale = (float)starT.scale0 / star.scale0;
			}
			scale *= 2;
			//вычисляем центр гипотезы буквы в тексте
			//поскольку мы не можем утверждать что мы нашли соответствие гипотезы кластера в тексте,
			//то габарит поиска мы считаем равным двойному габариту буквы
			//размещеаем центр области поиска по координатам найденного кластера текста

			int search_x = starT.cPoint.x - (letterTest.letterW * scale) * 0.7f;
			int search_y = starT.cPoint.y - (letterTest.letterH * scale) * 0.7f;
			int search_dx = letterTest.letterW * scale * 1.4;
			int search_dy = letterTest.letterH * scale * 1.4;

			//выполняем поиск OCRStar в пределах габарита поиска

			for (int index1 = 0; index1 < graph.starArray.size(); index1++) {
				OCRStar &star = graph.starArray[index1];
				if (star.cPoint.x > search_x && star.cPoint.x < search_x + search_dx && star.cPoint.y > search_y && star.cPoint.y < search_y + search_dy) {
					searchStar.push_back(index1);
				}
			}
			//для найденного списка кластеров текста проводим проверку
			//всех букв к которых есть найденные кластеры.
			//находим в этой области поиска 3 буквы в которых есть базовый кластер,
			//и которые имеют наибольший габарит и количество кластеров с высокой корреляцией

			//для этого проверяем все строки таблицы кластеров которые соответствуют найденным кластерам.
			//также строим поисковый регистр всех индексов кластеров найденых в зоне поиска
			vector<int> searchClusterAll;
			vector<char> searchRegister;
			searchRegister.resize(fontClusterTable->size());
			int sizeSearchStar = (int)searchStar.size();
			//для того чтобы не проверять гипотезу буквы несколько раз на одном месте создаем регистр поиска
			vector<char> letterRegister;
			letterRegister.resize(font->size());

			for (int index1 = 0; index1 < sizeSearchStar; index1++) { //для каждой OCRStar в области поиска

				int size = (int)text2Cluster[searchStar[index1]].size(); //количество гипотез кластеров для OCRStar текста
				for (int index2 = 0; index2 < size; index2++) {
					//записываем индекс строки таблицы кластеров каждого базового кластера
					//который соответствует этой OCRStar в области поиска
					searchClusterAll.push_back(text2Cluster[searchStar[index1]][index2]);
					searchRegister[text2Cluster[searchStar[index1]][index2]] = 255;
					// cout<<"cluster:"<<text2Cluster[searchStar[index1]][index2]<<endl;
				}
			}

			//проверяем все буквы в которых есть найденные кластеры на наличие в области поиска большинства
			//базовых кластеров этой буквы.
			//записываем гипотезы букв в массив для сортировки
			vector<ClusterMatch> resultLetters;

			int searchClusterAllSize = (int)searchClusterAll.size();

			for (int index1 = 0; index1 < searchClusterAllSize; index1++) {

				ulong size2;
				ClusterMatch *line = (ClusterMatch *)fontClusterTable->getPtr(searchClusterAll[index1], size2);
				int size1 = (int)size2 / sizeof(ClusterMatch);

				for (int index2 = 0; index2 < size1; index2++) {
					ClusterMatch &match = line[index2];
					// cout<<"id:"<<match.clusterID<<" "<<match.matchIndex<<endl;
					// if(match.clusterID==229){
					//    cout<<" id:"<<match.clusterID<<" matchIndex:"<<match.matchIndex<<endl;
					//}
					if (letterRegister[match.matchIndex]) continue;
					letterRegister[match.matchIndex] = 255;

					// OCRLetter letter;
					// readOnlyLetter(letter, match.matchIndex);
					// if(letter.letterID==1576660881){
					//    1;
					//}

					//читаем список базовых кластеров этой буквы
					ulong size2;
					ClusterMatch *letterCluster = (ClusterMatch *)letterClusterTable->getPtr(match.matchIndex, size2);
					// OCRLetter letter;
					// readLetter(letter, match.matchIndex);
					// cout<<"letter:"<<letter.name.data<<endl;
					int clusterCount = (int)size2 / sizeof(ClusterMatch);
					//проверяем количество совпадений кластеров эталонной буквы и кластеров в области поиска
					//при подсчете кореляции учитываем количество OCRStar эталонной буквы которые входят в найденный базовый кластер.
					int countCorrelation = 0;
					for (int index3 = 0; index3 < clusterCount; index3++) {
						ClusterMatch &match1 = letterCluster[index3];
						//проверяем есть ли такой кластер в зоне поиска
						if (searchRegister[match1.clusterID]) { countCorrelation += 100; }
					}
					int correlation = countCorrelation / match.clusterCount;

					if (correlation > correlationLimit * 0.7f) {
						match.correlation = correlation;
						match.starIndex = clusterCount;
						resultLetters.push_back(match);
						// cout<<" letter:"<<letter.name.data <<" id:"<<letter.letterID<<" c:"<<correlation<<endl;
					}
				}
			}
			// return;

			//сортируем найденные буквы по габариту. Это позволит сначала проверить стековые буквы и затем простые
			DR(" resultLetters:" << resultLetters.size() << endl);
			sort(resultLetters.begin(), resultLetters.end(), sortMatchSize);

			//для первых трех гипотез букв выполняем проверку геометрии и 1,2,3,4 lookup
			int resultLettersSize = (int)resultLetters.size();
			int matchCount = 0;
			int maxCorrelation = 0;
			int maxCorrelationIndex = 0;
			for (int index2 = 0; index2 < resultLettersSize; index2++) {
				ClusterMatch &match = resultLetters[index2];

				OCRLetter letter;
				readOnlyLetter(letter, match.matchIndex);
				// readLetter(letter, match.matchIndex);

				//if (letter.letterID == 1569372575) { // 1576250752
					//cout << "x:" << search_x << " y:" << search_y << endl;
				//}

				match.correlation = correlationGraphArea(graph, letter, searchStar);

				// if(match.matchIndex==13108){
				// cout<<" id:"<<match.clusterID<<" c:"<<match.correlation<<" letterID:"<<letter.letterID<<" name:"<<letter.name.data<<endl;
				//    match.correlation=100;
				//}

				//для каждой найденной буквы корреляция которой выше пороговой выполняем проверку геометрии и 1,2,3 lookup
				if (match.correlation < correlationLimit) continue;
				count++;
				detectScale(graph.starArray.dataPtr(), letter);

				//if (count1 == 74482) { 1; }

				DR(" letter:" << letter.letterID << " scale:" << letter.scale << " correlation:" << match.correlation << " name:" << letter.name.data << endl);
				correlationOCR(graph, letter);

				//if (testMatch(letter, 2294, 305)) {
					// if(letter.name=="ག"){
					//1;
					//}
				//}

				if (letter.correlation < correlationLimit * 1.2f) continue;

				// cout<<letter.letterID<<" "<<letter.xMatch<<"/"<<letter.yMatch<<endl;

				if (match.correlation > maxCorrelation) {
					maxCorrelation = match.correlation;
					maxCorrelationIndex = (int)matchLetter.size();
				}

				// OCRStar текста в пределах центральной области буквы с высокой корреляцией
				//исключаем из дальнейшей проверки гипотез букв

				if (letter.correlation > 98) {
					int x1 = letter.xMatch - letter.letterW * letter.scale * 0.4;
					int x2 = letter.xMatch + letter.letterW * letter.scale * 0.4;
					int y1 = letter.yMatch - letter.letterH * letter.scale * 0.4;
					int y2 = letter.yMatch + letter.letterH * letter.scale * 0.4;

					for (int index4 = 0; index4 < searchStar.size(); index4++) {
						//исключаем OCRStar текста из поиска
						OCRStar &starT = graph.starArray[searchStar[index4]];
						if (starT.cPoint.x > x1 && starT.cPoint.x < x2 && starT.cPoint.y > y1 && starT.cPoint.y < y2) {
							if (match.correlation > starT.corrC) starT.corrC = match.correlation;
						}
					}
				}

				OCRLetter matchL;
				copyMatch(letter, matchL);
				//вычисляем габарит и координаты гипотезы буквы в тексте
				float k = matchL.scale / 2;
				matchL.s.x0 = matchL.xMatch - matchL.letterW * k;
				matchL.s.x1 = matchL.xMatch + matchL.letterW * k;
				matchL.s.y0 = matchL.yMatch - matchL.letterH * k;
				matchL.s.y1 = matchL.yMatch + matchL.letterH * k;
				matchL.s.area = (matchL.s.x1 - matchL.s.x0) * (matchL.s.y1 - matchL.s.y0);

				//на одном месте могут быть буквы с одинаковым именем и различной корреляцией.
				//оставляем только одну такую гипотезу буквы с наибольшей корреляцией.
				int flag = 0;

				for (int index4 = 0; index4 < matchLetter.size(); index4++) {
					OCRLetter &letterT = matchLetter[index4];
					if (letter.name == letterT.name) {
						if (letter.xMatch > letterT.s.x0 && letter.xMatch < letterT.s.x1 && letter.yMatch > letterT.s.y0 && letter.yMatch < letterT.s.y1) {
							flag = 1;
							if (letter.correlation > letterT.correlation) { letterT = matchL; }
						}
					}
				}

				// flag=0;
				if (!flag) {
					matchLetter.push_back(matchL);
					matchCount++;
					// if(matchCount==3)break;
				}
			}
			// TIME_PRINT_;
			if (!matchLetter.size()) continue;
			// OCRStar текста в пределах центральной области буквы с наибольшей корреляцией
			//исключаем из дальнейшей проверки гипотез букв
			OCRLetter &letter = matchLetter[maxCorrelationIndex];

			int x1 = letter.xMatch - letter.letterW * letter.scale * 0.4;
			int x2 = letter.xMatch + letter.letterW * letter.scale * 0.4;
			int y1 = letter.yMatch - letter.letterH * letter.scale * 0.4;
			int y2 = letter.yMatch + letter.letterH * letter.scale * 0.4;

			for (int index4 = 0; index4 < searchStar.size(); index4++) {
				OCRStar &starT = graph.starArray[searchStar[index4]];
				if (starT.cPoint.x > x1 && starT.cPoint.x < x2 && starT.cPoint.y > y1 && starT.cPoint.y < y2) {}
				starT.corrC = 100;
			}
		}
	}
	v.push_back(0);
	cout << "matchText:" << matchLetter.size() << endl;
	// sort(matchLetter.begin(),matchLetter.end(),sortLetterC);

	//нормируем габариты гипотезы буквы
	// if(print)
	// for(int i=0;i<matchLetter.size();i++){
	//    cout<<"letterID:"<<matchLetter[i].letterID<<" c:"<<matchLetter[i].correlation<<" x:"<<matchLetter[i].xMatch<<""" name:"<<matchLetter[i].name.data<<endl;
	//}
	cout << "count:" << count << endl;
*/
	return;
}

} // namespace ocr
