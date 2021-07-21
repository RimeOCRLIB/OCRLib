
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
	
/** @bref ЧЕТВЕРТЫЙ lookup проверки кореляции кластеров эталона и текста по масштабозависимым алгоритмам */
int GGraphOCR::OCRClusterCorrelationLookup4(OCRCluster &cluster, OCRCluster &clusterText,OCRLetter &letter, GGraph &graph, OCRData32 &lineCor) {
    /*
     Алгоритм май 2021
     для всех OCRCluster с невысокойй корреляцией применяем несколько алгоритмов.
     Первый алгогитм позволяет учесть влияние афинных преобразований
     и привести соответствующие фокальные линии эталона и текста  к одному масштабу, углу поворота и сдвигу.
     Эти афинные преобразования применяются в пределах лимитов.
     При вычислении корреляции совмещаем начало проверяемых линий в маске 32х32
     и проверяем лимиты афинных преобразовани необходимых для совмещения концов линий.
     Корреляцию формы совмещенных линий проверяем по маске 32х32 аналогично lookup2
     для линий которые не прошли проверку по аффинным преобразованиям проводим проверку по диаграмме Воронова.
     */
    /**
     МАСШТАБО ЗАВИСИМЫЙ lookup
     Алгоритм 1 октября 2019 года
     На этапе первого второго и третьего lookup проверяются Т и Х точки до первого экстремума.
     Все буквы базы данных которые не содержат таких точек на этом этапе не проверяются.
     Также при вычислении хеша Т и Х точек Т точки с одной короткой линией (засечками шрифта)
     приравниваются к точке экстремума кривой.
     Устойчивые Т и Х точки таким образом содержат достаточно короткие и устойчивые линии.
     Эти точки используются для получения гипотезы масштаба и передаются в функцию четвертого lookup.
     
     Для вычисления корреляции кривых и кругов которые содержат точки экстремума, L точки, Х и Т точки
     с короткими линиями (засечками шрифта) в четвертом lookup используется диаграмма Воронова.
     Для всех фокальных линий текста строится диаграмма Воронова, где каждый участок диаграммы
     маркирован ID номером фокальной линии.
     Для каждого набора соединенных фокальных линий выходящих из устойчивой Т и Х точки эталона
     выполяется последовательная проверка точек экстремума, L точек и неустойчивых Т точек.
     Проверка начинается с точки ближайшей к устойчивой Т или Х точке.
     Для кругов начало координат выбирается как центр круга.
     Проверяется попадание проверяемой точки в участок диаграммы Воронова.
     При наличии попадания вычисляется точка фокальной линии текста ближайшей к проверяемой точке эталона.
     Следующая точка эталона проверяется по отношению к координатам найденой точки фокальной линии текста.
     Такое последовательное применение сдвига начала координат проверяемых точкек эталона
     позволяет алгоритму не учитывать плавные изменения характера проверяемой кривой текста
     соответствующие афинным преобразованиям. Реализована синхронизация кривых по принципу кольчуги.
     
     
     Ретрит 2019 года
     Для вычисления корреляции звездочек фокальные линии которых
     имеют разрывы и шумовые фокальные точки выполняется
     определение масштаба участка текста в котором производится
     вычисление корреляции звездочек.
     Масштаб текста вычисляется в функции @fn detectScale
     Масштаб букв в базе данных вычисляется путем сравнения
     каждой буквы базы данных с изображением соответствующей буквы
     эталонного шрифта.
     Эталонная звездочка и изображение области текста с проверяемой звездочкой приводится к
     одному масштабу. Концы фокальных линий области текста с проверяемой звездочкой
     растискиваются в круглые области поиска диаметром в среднюю
     толщину штриха в первом тестовом изображении.
     Фокальные линии текста в масштабированном изображении
     растискиваются алгоритмом dotGain во втором тестовом изображении.
     Координаты центра эталонной звездочки совмещаются с координатами центра тестовой
     звездочки текста в тестовых изображениях путем вычисления смещения по X/Y. Вычисление
     корреляции концов фокальных линий производится проверкой наличия черного пиксела в
     первом тестовом изображении по координатам концов фокальных линий эталонной звездочки.
     Вычисление корреляции точек экстремума фокальных линий производится проверкой наличия
     черного пиксела во втором тестовом изображении по координатам точек экстремума эталонной
     звездочки. Также проверяются промежуточные точки оцифровки фокальной линии эталонной
     звездочки. Это необходимо для точного определения совмещения одной фокальной линии
     эталонной звездочки и неизвесного на момент корреляции числа фокальных линий текста.
     @img img3.jpg 256
     ДОДУМАТЬ:
     Также необходимо проверить совпадение направления фокальной линии эталонной и тестовой
     звездочки в точках экстремума.
     
     */
    
    int print = 0;
    if(printCorrelation)print=printCorrelation;
    DR("===   Lookup 4 ===" << endl);
    float clusterCorrelation = 0;
    lineCor.len=0;
    //int dataSize = graph.data_size - 1;
    countLookup4++;
    
    DR("   Lookup 4" << endl);
    //вычисляем корреляцию центральной OCRStar кластера
    int correlation1 = 0;
    //int flagCorrelation = 0; //флаг совпадения OCRStar буквы и текста по lookup 1,2,3
    
    //для центральной OCRStar кластера выполняем проверку гипотезы соответствия OCRStar текста по лимитам координат в соответствии с гипотезой масштаба.
    //int x = (letter.xMatch + cluster.cPoint.x * letter.scale);
    //int y = (letter.yMatch + cluster.cPoint.y * letter.scale);
    
    OCRStar &star=cluster.star[0]; //центральная OCRStar кластера буквы
    OCRStar &starT = graph.starArray[clusterText.sIndex]; //центральная OCRStar кластера текста
    
    //проверяем типы центральных OCRStar кластеров
    if(abs(star.type-starT.type)>1)return 0;
    
    cluster.cPointText=clusterText.cPoint; //записываем в кластер гипотезу центра кластера (для диаграмы Воронова)
    
    int limit = star.lineW * 2 * letter.scale;
    if (limit < LINE_SIZE * 2 * letter.scale) limit = LINE_SIZE * 2 * letter.scale;
    if(limit>star.sumLenA/4)limit=star.sumLenA/4;

    
    DR("   ___found star:" << starT.sIndex << endl);
    
    if (star.type==Z_POINT && starT.type == Z_POINT) return 100; //кластер из одной OCRStar без OCRLine  - точка текста
    //для OCRStar L типа проверяем является ли эта линия уже принадлежащей другой гипотезе кластера буквы
    //if (star.type == L_POINT) {
    //    if (letterLineReg[starT1.line[0]]) continue;
    //}
    
    //для центров кругов проверяем только совпадение типов
    if (star.type == O_POINT) {
        if (starT.type == O_POINT) {
            //записываем соответствующие линии текста в реестр
            for(int i=0;i<starT.lineCount;i++){
                lineCor.data[lineCor.len]=starT.line[i];
                lineCor.len++;
            }
            return 100;
        }else{
            //если в букве не найден круг ухудшаем корреляцию буквы.
            //это описательное правило и должно устанавливатся в матрице условных переходов
            return -20;
        }
    }else{
        //если соответствие еще не проверялось, проводим корреляцию по диаграмме Воронова.
        static int count11=0;
        count11++;
        if(count11==3){
            ;
        }
        
        correlation1 = voronovDiagramCorrelation(starT, star, graph, letter, cluster,lineCor);

    }
    clusterCorrelation = correlation1;
    if (clusterCorrelation < 70) return 0;
    return clusterCorrelation;
    
    /*
    //последовательно проверяем OCRStar лучей кластера
    
    int clusterStarCount = star.starCount; //количество OCRStar кластера буквы которые проверяются по диаграмме Воронова.
    int clusterStarCorrelation=0;
    int reg=0;
    char *regC=(char*)&reg; //в регистре отмечаем проверенные линии
    int w=graph.columns();
    
    for (int index = 0; index < star.starCount; index++) {
        OCRStar &star1 = cluster.star[star.star[index]];
        
        if (star1.type==L_POINT && star1.sumLenA < star.lineW * 1.5f) {
            //короткие лучи кластера с OCRStar типа LPoint не проверяем по диаграмме Воронова и не учитываем в корреляции.
            clusterStarCount--;
            continue;
        }
        
        //DR("star__:" << star.index << "  corr__star:" << star1.index << " starT:" << starT.star[star.sMatch[index]] << " c:" << correlation1 << endl);
        
        //вычисляем корреляцию всех фокальных линий OCRStar лучей кластера
        //вычисляем порядок соответствия OCRStar лучей кластера
        int min = 360;
        int minIndex = 0;
        int angle = star.angle[index];
        for (int m = 0; m < starT.starCount; m++) {
            int dl = abs(starT.angle[m] - angle);
            if (dl >= 325) dl = 360 - dl;
            if (dl < min && !regC[m]) {
                min = dl;
                minIndex = m;
            }
        }
        regC[minIndex]=1;
        star.sMatch[index] = minIndex; //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
        star1.match[0] = starT.star[minIndex];   //!// исключение составляют линии круга примыкающие к проверяемой starT
        
        OCRStar &starT1 = graph.starArray[star1.match[0]];
        
        //проверяем типы OCRStar кластеров
        //if(abs(star1.type-starT1.type)>1)return 0;
        
        //проверяем OCRStar лучей кластера star1 и starT1 по lookup3
        //correlation1 = OCRStarCorrelationLookup2(starT1, star1, graph.focalLine.dataPtr(), letter.line,NULL,NULL,0);
        correlation1=0;
        OCRData32 v;
        static int cout1_1=0; cout1_1++;
        if(cout1_1==27){
            cout<<1;
        }
        
        correlation1 = voronovDiagramCorrelation(starT1, star1, graph, letter, cluster,v);
        
        //for(int m=0;m<v.len;m++){
        //    if(letter.letterID==1569372096 && v.data[m]==3){
        //        cout<<1;
        //    }
        //}
        
        if (correlation1 > 70) {
            clusterStarCorrelation += correlation1;
            DR("   @V cluster correlation:" << correlation1 << " starT:" << starT1.index << endl << endl);
            //записываем соответствующие линии текста в реестр
            for(int i=0;i<v.len;i++){
                lineCor.data[lineCor.len]=v.data[i];
                lineCor.len++;
            }
        } else {
            //для этой OCRStar кластера не нашлось соответствующей OCRStar текста
            //проверяем все OCRStar текста в пределах лимита координат
            
            int x = (letter.xMatch + star1.cPoint.x * letter.scale);
            int y = (letter.yMatch + star1.cPoint.y * letter.scale);
            
            OCRPoint adrFocalPoint[64];
            
            //находим все фокальные точки по заданному лимиту координат
            int sizeAdr = graph.detectFocalPoint(x - limit / 2, y - limit / 2, limit, limit, graph.columns(),
                                                 graph.rows(), graph.bites_data_32, adrFocalPoint);
            DR("   sizeAdr:" << sizeAdr << endl);
            
            //находим ID номер фокальной точки и проверяем корреляцию
            int maxCorr = 0;
            int maxCorrIndex = -1;
            star1.matchCount = 1;
           OCRData32 lenCorr2D[sizeAdr];
            for (int i = 0; i < sizeAdr; i++) {
                OCRPoint &p = adrFocalPoint[i];
                //находим ID номер фокальной точки и ее исходные координаты
                int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
                //проверяем занятость точки текста
                //if (pointReg[id]) continue;
                OCRStar &starT1 = graph.starArray[id];
                DR("      ___detect star:" << starT1.index << endl);
                
                //вычисляем соответствие по lookup 3;
                // lookup 1 и 2 не выполняем, поскольку на этом этапе необходимо выяснить максимальную корреляцию
                // correlation1=OCRStarCorrelationLookup3(starT1, star1, letter.point, letter.line);
                // if(correlation1<95){
                //static int cont_2_1=0;cont_2_1++;
                //if(cont_2_1==1){
                //    cout<<1;
                //}
                
                OCRData32 &v=lenCorr2D[i];
                correlation1 = voronovDiagramCorrelation(starT1, star1, graph, letter, cluster,v);
                
                //for(int m=0;m<v.len;m++){
                //    if(letter.letterID==1569372096 && v.data[m]==3){
                //        cout<<1;
                //    }
                //}
                
                //}
                if (correlation1 > maxCorr) {
                    maxCorr = correlation1;
                    maxCorrIndex = i;
                    
                    //в проверяемом участке тексте может быть несколько гипотез OCRStar эталона.
                    //предполагаем что их не может быть больше 4
                    star1.match[star.matchCount] = id;
                    star1.matchCount++;
                    if (star.matchCount == 4) star1.matchCount = 3;
                }
            }
            //вычисляем корреляцию всех фокальных линий OCRStar кластера
            clusterStarCorrelation += maxCorr;
            star1.corrC = maxCorr;
            star1.match[0] = maxCorrIndex;
            if(maxCorrIndex!= -1){
                OCRData32 &v=lenCorr2D[maxCorrIndex];
                for(int i=0;i<v.len;i++){
                    lineCor.data[lineCor.len]=v.data[i];
                    lineCor.len++;
                }
            }

            DR("   @V cluster correlation:" << maxCorr << " starT:" << maxCorrIndex << endl << endl);
        }
    }
    
    if(clusterStarCount)clusterStarCorrelation = clusterStarCorrelation / clusterStarCount;
    clusterStarCorrelation = clusterCorrelation*0.5 + clusterStarCorrelation*0.5;
    
    DR(" //------ LookUp4 cluster:"<<cluster.index<<" star: " << star.index << " correlation:" << clusterStarCorrelation << endl);
   
    return clusterStarCorrelation;
    */

}
    
    
/** @bref ЧЕТВЕРТЫЙ lookup проверки кореляции кластеров эталона и текста по масштабозависимым алгоритмам */
int GGraphOCR::OCRLineCorrelationLookup4( OCRLetter &letterT,OCRLetter &letter) {
	/*
		Алгоритм август 2020
		для всех OCRLine с невысокойй корреляцией применяем
		несколько алгоритмов.
		Первый алгогитм позволяет учесть влияние афинных преобразований
		и привести соответствующие фокальные линии эталона и текста  к одному масштабу, углу поворота и сдвигу.
		Эти афинные преобразования применяются в пределах лимитов.
		При вычислении корреляции совмещаем начало проверяемых линий в маске 32х32
		и проверяем лимиты афинных преобразовани необходимых для совмещения концов линий.
		Корреляцию формы совмещенных линий проверяем по маске 32х32 аналогично lookup2
	*/
	
	/*
	
	int print = 1;
	DR("===   Lookup 4 ===" << endl);
	OCRStar *starsT=letterT.star;
	OCRLine *linesT=letterT.line;
	//OCRPoint *pointT=letterT.point;
	int starTCount=letterT.starCount;
	int letterCorrelation = 0;
	int flag = 1;
	int lineCount=letter.lineCount;
	//Для всех линий гипотезы буквы с невысокой корреляцией вычислям корреляцию
	//с учетом аффинных преобразований и вероятной разницы в расположении промежуточных OCRStar
	for (int index = 0; index < lineCount; index++) {
		
		OCRLine &line = letter.line[index];
		int lineCorr = line.corr;
		if(lineCorr>95)continue;
		DR("//_________ new line :" << index << " lineCorr:" << lineCorr << endl;)
		//находим соответствие одного из концов линии
		OCRStar &startStar=letter.star[line.startID];
		OCRStar &endStar=letter.star[line.endID];
		//проверяем есть ли вероятность найти в тексте соответствующую составную линию
		//cout<<"lenA:"<<line.lenA<<" lineW:"<<startStar.lineW<<endl;
		if((startStar.type==L_POINT || endStar.type==L_POINT) && line.lenA<startStar.lineW * 2){
			DR(" line "<<line.index<<" too short"<<endl;)
			continue;
		}
		//проверяем есть ли P_POINT между стартовой и конечной точкой линии в тексте
		if(startStar.type!=P_POINT && endStar.type != P_POINT && startStar.match[0]!=-1 && endStar.match[0] != -1){
				int matchID=endStar.match[0];
				OCRStar &starT=starsT[startStar.match[0]];
				int flag=0;
				for(int i=0;i<starT.starCount;i++){
					if(starT.star[i]==matchID){
						flag=1;
						line.match=starT.line[i];
						break;
					}
				}
				if(flag){
					DR("//_________ line :" << index << " have corr line:" << line.match << endl;)
					continue;
				}
		}
		cout<<"@1"<<endl;
		int maxCorr=0;
		int indexStar=-1;
		int indexLine=line.index;
		cout<<" startStar:"<<startStar.index<<" match:"<<startStar.match[0]<<" corr:"<<startStar.corr<<" corrC:"<<startStar.corrC<<endl;
		cout<<" endStar:"<<endStar.index<<" match:"<<endStar.match[0]<<" corr:"<<endStar.corr<<" corrC:"<<endStar.corrC<<endl;
		if(startStar.corrC>maxCorr){
			maxCorr=startStar.corrC;
			indexStar=startStar.index;
		}
		if(startStar.corr>maxCorr){
			maxCorr=startStar.corr;
			indexStar=startStar.index;
		}
		if(endStar.corrC>maxCorr){
			maxCorr=endStar.corrC;
			indexStar=endStar.index;
		}
		if(endStar.corr>maxCorr){
			maxCorr=endStar.corr;
			indexStar=endStar.index;
		}
		cout<<"@2 indexStar:"<<indexStar<<endl;
		if(indexStar==-1 || maxCorr<70){
			//проверяем является ли один или оба конца линии OCRStar типа P_POINT
			if(startStar.type==P_POINT){
				//проверяем есть ли соответствие у присоединенной к этой точке линии
				int i=0;
				int indexL=-1;
				int limit=endStar.lineW * SERIF_LIMIT;
				while(i<startStar.lineCount){
					if(startStar.line[i]==line.index || startStar.len[i]<limit){
						i++;
						continue;
					}else{
						indexL=startStar.line[i];
						break;
					}
				}
				int indexS=-1;
				if(indexL>-1){
					OCRLine &lineS=letter.line[indexL];
					//проверяем имеет ли соответствие OCRStar на другом конце найденой линии
					if(startStar.dir[i]==1){
						indexS=lineS.endID;
					}else{
						indexS=lineS.startID;
					}
				}
				if(indexS>-1){
				OCRStar startStar_=letter.star[indexS];
					if(startStar_.match[0]>-1 &&  startStar_.corrC>maxCorr){
							//найдена стартовая OCRStar для проверки
							indexStar=indexS;
							indexLine=indexL;
							cout<<"@ start star:"<<indexS<<" start line:"<<indexL<<endl;
					}
				}
			}
			if(endStar.type==P_POINT){
				//проверяем есть ли соответствие у присоединенной к этой точке линии
				int i=0;
				int indexL=-1;
				int limit=endStar.lineW * SERIF_LIMIT;
				while(i<endStar.lineCount){
					if(endStar.line[i]==line.index || endStar.len[i]<limit){
						i++;
						continue;
					}else{
						indexL=endStar.line[i];
						break;
					}
				}
				int indexS=-1;
				if(indexL>-1){
					OCRLine &lineS=letter.line[indexL];
					//проверяем имеет ли соответствие OCRStar на другом конце найденой линии
					if(endStar.dir[i]==1){
						indexS=lineS.endID;
					}else{
						indexS=lineS.startID;
					}
				}
				if(indexS>-1){
					OCRStar endStar_=letter.star[indexS];
					if(endStar_.match[0]>-1 && endStar_.corrC>maxCorr){
							//найдена стартовая OCRStar для проверки
							indexStar=indexS;
							indexLine=indexL;
							cout<<"@ end star:"<<indexS<<" end line:"<<indexL<<endl;
					}
				}
			}
		}
		cout<<"@4"<<endl;
		if(indexStar>-1 && maxCorr >70){
				cout<<"@@@ end star:"<<indexStar<<" start line:"<<indexLine<<endl;
				
				//выполняем корреляцию начиная с найденной линии
				//проверяем можно ли составить линию из нескольких линий
				OCRLineCorrelationVoronov(
													letterT, indexLine,  indexStar, letter);
		}else{
				cout<<" start star for correlation not found"<<endl;
				
		}

	}//for (int lineIndex = 0; lineIndex < lineCount; lineCount++) 
	letterCorrelation=0;
	for (int index = 0; index < lineCount; index++) {
		OCRLine &line=letter.line[index];
		DR("  ### check  line:" <<line.index << " lineT:"<< line.match << " corr:"<< line.corr<<endl;)
		letterCorrelation+=line.corr;
	}
	letterCorrelation/=lineCount;
	cout<< "@ lookup 4 letter correlation:"<<letterCorrelation<<endl;
	return letterCorrelation;
	*/
	return 0;
	
}


/** @bref функция ЧЕТВЕРТОГО lookup проверки кореляции OCRLine эталона и текста по масштабозависимым алгоритмам. Проверка совпадения OCRLine эталона  и текста методом проверки по массиву атрибутированных пикселов фокальных линий line_data (диаграмма Воронова)*/
void GGraphOCR::OCRLineCorrelationVoronov(
					OCRLetter &letterT, int lineIndex, int starIndex, OCRLetter &letter){
	/*																			
	int print=1;
	int correlation=0;
	OCRStar *starsT=letterT.star;
	OCRLine *linesT=letterT.line;
	//OCRPoint *pointT=letterT.point;
	OCRData *line_dataT=letterT.line_data;
	int starTCount=letterT.starCount;
	OCRLine *line=letter.line+lineIndex;
	OCRStar *star=letter.star+starIndex;
	int in=line->startIndex;
	if(line->endID==starIndex)in=line->endIndex;
	int dir=star->dir[in];
	//находим соответствующую линию в тексте
	OCRStar *starT=starsT+star->match[0];
	if(!dir)in=line->endIndex;
	OCRLine *lineT=linesT+starT->line[star->sMatch[in]];
	int dirT=starT->dir[star->sMatch[in]];
	uint idT = lineT->index;
	uint w = letterT.imgW;
	uint h = letterT.imgH;
	int dxT=letterT.xCenter;
	int dyT=letterT.yCenter;
	DR(endl<<"=== start Voronov correlation star:" <<starIndex <<" line:"<<lineIndex<<" dir:" << dir <<" starT:"<< starT->index << " lineT:"<<lineT->index<<endl;)


	int dataSize = w * h - 1;

	int lineCorrelation = 0;

	//для проверки совпадения OCRLine  эталона и текста
	//необходимо совместить центры соответствующих OCRStar.
	//для этого вычисляем разность координат  OCRStar эталона и OCRStar текста
	//координаты точек фокальных линий буквы отсчитываются от центра буквы, поэтому вычисляемсмещение относительно центра гипотезы буквы
	

	int dX = starT->cPoint.x - (letter.xMatch + star->cPoint.x * letter.scale) +dxT;
	int dY = starT->cPoint.y - (letter.yMatch + star->cPoint.y * letter.scale)+dyT;
	cout<<"letter.xMatch:"<<letter.xMatch<<" letter.yMatch:"<<letter.yMatch
		<<" dX:"<<dX<<" dY:"<<dY<<" dxT:"<<dxT<<" dyT:"<<dyT<<endl;


	int corrCount = 0;
	float scale=letter.scale;
	
	while(1){ // проверяем до выхода по условию
		OCRPoint *pointData=letter.point + line->pointIndex;

		//проверяем линию по всей длине по диаграмме Воронова
		// id номер линии в line_data это id номер линии текста увеличенный на единицу
		int lenA = line->lenA-1;
		
		//для  линий эалона проверяем на совпадение с OCRLine текста по точкам на расстоянии 4/scale, 1/2 , 1/3 и 2/3, lenA-4/scale от начала линии  
		//не проверяем точку 4/scale  линии эталона 
		//ближайшую к совпадающей OCRStar эталона и текста
		//заполняем массив адресов проверяемых точек
		int pIndex[5];
		if(dir){
			pIndex[0]=lenA / 5;
			pIndex[1]=lenA / 3;
			pIndex[2]=lenA / 2;
			pIndex[3]=lenA-lenA / 3;
			pIndex[4]=lenA-lenA / 5;
		}else{
			pIndex[0]=lenA-lenA / 5;
			pIndex[1]=lenA - lenA / 3;
			pIndex[2]=lenA / 2;
			pIndex[3]=lenA / 3;
			pIndex[4]=lenA / 5;
		}
		
		for(int i=0;i<5;i++){
			corrCount++;
			OCRPoint &pL =  pointData[pIndex[i]];
			int x = letter.xMatch + pL.x * letter.scale + dX;
			int y = letter.yMatch + pL.y * letter.scale + dY;
			int sl = y * w + x;
			if (sl > 0 && sl < dataSize) {
				OCRData &p = line_dataT[sl];
				int id=p.d0-1;
				cout<<" p__ x:"<<x<<" y:"<<y<< " id:"<<p.d0-1<<" pId:"<<(int)p.d1<<" cId:"<<(int)p.d2<<" idT:"<<idT<<endl;
				if (id == idT) {
					//точка совпала по диаграмме Воронова
					lineCorrelation += 100;
				} else {
					//проверяем является ли линия в тексте по этому ID номеру
					// продолжением проверяемой линии
					//сначала проверяем есть  ли у этих линий общая OCRStar
					int flag=0;
					if(id==-1){
						cout<<"not match"<<endl;
						continue;
					}			
					cout<<" need check new lineT:"<<id<<endl;
					OCRLine *lineT_=linesT+id;
					int indexT;
					if(dirT){
						indexT=lineT->endID;
					}else{
						indexT=lineT->startID;
					}
					if(indexT==lineT_->startID){
						lineT=lineT_;
						dirT=1;
						idT=id;
						lineCorrelation += 100;
						cout<<" valid new lineT:"<<id<<endl;
					}else if(indexT==lineT_->endID){
						lineT=lineT_;
						dirT=0;
						idT=id;
						lineCorrelation += 100;
						cout<<" valid new lineT:"<<id<<endl;
					}else{
						cout<<"not match in line "<<id<<endl;
					}
				}
			}
		}
		//проверка тестовой линии закончена.
		//записываем корреляцию линии
		line->corr=lineCorrelation / corrCount;
		lineCorrelation=0;
		corrCount=0;
		//проверяем существует ли линия эталона с невысокой
		//корреляцией, присоединенная к проверяемой линии
		int indexNext;
		if(dir){
			indexNext=line->endID;
		}else{
			indexNext=line->startID;
		}
		cout<<"indexNextStar:"<<indexNext<<endl;
		star=letter.star+indexNext;
		int flag=0;
		for(int n=0;n<star->lineCount;n++){
			if(star->line[n]==line->index){
				continue;
			}
			OCRLine *line_=letter.line+star->line[n];
			cout<<" line_:"<<line_->index<<endl;
			if(line_->corr>95 || line_->lenA<star->lineW * 2 )continue;
			flag=1;
			
			//начинаем проверку следующей линии эталона
			line=line_;
			cout<<"check new line::"<<line->index<<endl;
			dir=star->dir[n];
			break;
		}
		if(flag)continue;
		
		break;
		
	}
				
	DR("         V correlation:" << line->corr << endl);
	*/

}


int GGraphOCR::OCRStarCorrelationLookup4(OCRStar &star, GGraph &graph, OCRLetter &letter) {
	/**
	 МАСШТАБО ЗАВИСИМЫЙ lookup
	 Алгоритм 1 октября 2019 года
	 На этапе первого второго и третьего lookup проверяются Т и Х точки до первого экстремума.
	 Все буквы базы данных которые не содержат таких точек на этом этапе не проверяются.
	 Также при вычислении хеша Т и Х точек Т точки с одной короткой линией (засечками шрифта)
	 приравниваются к точке экстремума кривой.
	 Устойчивые Т и Х точки таким образом содержат достаточно короткие и устойчивые линии.
	 Эти точки используются для получения гипотезы масштаба и передаются в функцию четвертого lookup.

	 Для вычисления корреляции кривых и кругов которые содержат точки экстремума, L точки, Х и Т точки
	 с короткими линиями (засечками шрифта) в четвертом lookup используется диаграмма Воронова.
	 Для всех фокальных линий текста строится диаграмма Воронова, где каждый участок диаграммы
	 маркирован ID номером фокальной линии.
	 Для каждого набора соединенных фокальных линий выходящих из устойчивой Т и Х точки эталона
	 выполяется последовательная проверка точек экстремума, L точек и неустойчивых Т точек.
	 Проверка начинается с точки ближайшей к устойчивой Т или Х точке.
	 Для кругов начало координат выбирается как центр круга.
	 Проверяется попадание проверяемой точки в участок диаграммы Воронова.
	 При наличии попадания вычисляется точка фокальной линии текста ближайшей к проверяемой точке эталона.
	 Следующая точка эталона проверяется по отношению к координатам найденой точки фокальной линии текста.
	 Такое последовательное применение сдвига начала координат проверяемых точкек эталона
	 позволяет алгоритму не учитывать плавные изменения характера проверяемой кривой текста
	 соответствующие афинным преобразованиям. Реализована синхронизация кривых по принципу кольчуги.


	 Ретрит 2019 года
	 Для вычисления корреляции звездочек фокальные линии которых
	 имеют разрывы и шумовые фокальные точки выполняется
	 определение масштаба участка текста в котором производится
	 вычисление корреляции звездочек.
	 Масштаб текста вычисляется в функции @fn detectScale
	 Масштаб букв в базе данных вычисляется путем сравнения
	 каждой буквы базы данных с изображением соответствующей буквы
	 эталонного шрифта.
	 Эталонная звездочка и изображение области текста с проверяемой звездочкой приводится к
	 одному масштабу. Концы фокальных линий области текста с проверяемой звездочкой
	 растискиваются в круглые области поиска диаметром в среднюю
	 толщину штриха в первом тестовом изображении.
	 Фокальные линии текста в масштабированном изображении
	 растискиваются алгоритмом dotGain во втором тестовом изображении.
	 Координаты центра эталонной звездочки совмещаются с координатами центра тестовой
	 звездочки текста в тестовых изображениях путем вычисления смещения по X/Y. Вычисление
	 корреляции концов фокальных линий производится проверкой наличия черного пиксела в
	 первом тестовом изображении по координатам концов фокальных линий эталонной звездочки.
	 Вычисление корреляции точек экстремума фокальных линий производится проверкой наличия
	 черного пиксела во втором тестовом изображении по координатам точек экстремума эталонной
	 звездочки. Также проверяются промежуточные точки оцифровки фокальной линии эталонной
	 звездочки. Это необходимо для точного определения совмещения одной фокальной линии
	 эталонной звездочки и неизвесного на момент корреляции числа фокальных линий текста.
	 @img img3.jpg 256
	 ДОДУМАТЬ:
	 Также необходимо проверить совпадение направления фокальной линии эталонной и тестовой
	 звездочки в точках экстремума.

	 */
	 
	/* 
	int print = 1;
	float clusterCorrelation = 0;
	OCRStar *starArray = graph.starArray.dataPtr();
	int w = graph.columns();
	int dataSize = graph.data_size - 1;

	DR("   Lookup 4" << endl);
	//вычисляем корреляцию центральной OCRStar кластера
	int correlation1 = star.corrC;
	int flagCorrelation = 0; //флаг совпадения OCRStar буквы и текста по lookup 1,2,3

	//для центральной OCRStar кластера выполняем поиск соответствующей OCRStar текста по лимитам координат
	//в соответствии с гипотезой масштаба.
	int x = (letter.xMatch + star.cPoint.x * letter.scale);
	int y = (letter.yMatch + star.cPoint.y * letter.scale);

	OCRPoint adrFocalPoint[64];
	int limit = star.lineW * 4 * letter.scale;
	if (limit < LINE_SIZE * 4 * letter.scale) limit = LINE_SIZE * 4 * letter.scale;

	//находим все фокальные точки по заданному лимиту координат
	int sizeAdr = graph.detectFocalPoint(
		x - limit / 2, y - limit / 2, limit, limit, graph.columns(), graph.rows(), graph.bites_data_32, adrFocalPoint);
	//cout<<t<<" "<<sizeAdr<<" p:"<<(int)star.pointCount<<endl;

	//находим ID номер фокальной точки и проверяем корреляцию
	int maxCorr = 0;
	int maxCorrIndex = -1;
	star.matchCount = 1;

	//проверяем есть ли результате поиска OCRStar текста соответсвующие центральной OCRStar кластера
	//по масштабонезависимым lookup
	if (star.corrC > 95 && star.cluster == 0) {
		for (int i = 0; i < sizeAdr; i++) {
			OCRPoint &p = adrFocalPoint[i];
			//находим ID номер фокальной точки и ее исходные координаты
			int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
			//проверяем занятость точки текста
			if (pointReg[id]) continue;
			OCRStar &starT1 = graph.starArray[id];
			DR("   ___found star:" << starT1.index << endl);

			if (!star.lineCount || !starT1.lineCount) continue;
			//для OCRStar L типа проверяем является ли эта линия уже принадлежащей другой гипотезе кластера буквы
			if (star.type == L_POINT) {
				if (letterLineReg[starT1.line[0]]) continue;
			}

			//проверяем является ли найденая OCRStar текста уже проверенной гипотезой на этапе
			//применения lookup 1, 2 и 3 и лучи OCRStar не лимитированы
			if (id == star.match[0]) {
				if (star.corrC > 95 && star.cluster == 0 && starT1.cluster == 0) {
					maxCorr = star.corrC;
					maxCorrIndex = id;
					flagCorrelation = 1;
					break;
				}
			}
		}
	}

	//если соответствие не найдено, проверяем все OCRStar текста в результатах поиска
	if (maxCorrIndex < 0) {
		for (int i = 0; i < sizeAdr; i++) {
			OCRPoint &p = adrFocalPoint[i];
			//находим ID номер фокальной точки и ее исходные координаты
			int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
			//проверяем занятость точки текста
			if (pointReg[id]) continue;
			OCRStar &starT1 = graph.starArray[id];
			DR("   ___found star:" << starT1.index << endl);

			if (!star.lineCount || !starT1.lineCount) continue;
			//для OCRStar L типа проверяем является ли эта линия уже принадлежащей другой гипотезе кластера буквы
			if (star.type == L_POINT) {
				if (letterLineReg[starT1.line[0]]) continue;
			}

			//для центров кругов проверяем только совпадение типов
			if (star.type == O_POINT) {
				if (starT1.type == O_POINT) {
					maxCorr = 100;
					maxCorrIndex = id;
					break;
				}
				continue;
			}
			//если соответствие еще не проверялось, проводим корреляцию по диаграмме Воронова.
			correlation1 = voronovDiagramCorrelation(starT1, star, graph, letter);
			if (correlation1 > maxCorr) {
				maxCorr = correlation1;
				maxCorrIndex = id;
				//в проверяемом участке тексте может быть несколько гипотез OCRStar эталона.
				//предполагаем что их не может быть больше 4
				star.match[star.matchCount] = id;
				star.matchCount++;
				if (star.matchCount == 4) star.matchCount = 3;
			}
		}
	}
	clusterCorrelation += maxCorr;
	star.corrC = maxCorr;
	star.match[0] = maxCorrIndex;

	if (star.type == O_POINT) {
		if (star.corrC == 100) {
			return 100;
		} else {
			//если в букве не найден круг ухудшаем корреляцию буквы.
			//это описательное правило и должно устанавливатся в матрице условных переходов
			star.matchCount = 0;
			return -20;
		}
	}
	if (maxCorr < 70) return 0;

	//последовательно проверяем OCRStar лучей кластера

	OCRStar &starT =
		graph.starArray[maxCorrIndex]; // starT это OCRStar текста соответствующая OCRStar центра кластера буквы
	int clusterStarCount =
		star.starCount + 1; //количество OCRStar кластера буквы которые проверяются по диаграмме Воронова.

	for (int index = 0; index < star.starCount; index++) {
		OCRStar &star1 = letter.star[star.star[index]];

		if (star1.minLen <
			star.lineW *
				1.5f) { //короткие лучи кластера не проверяем по диаграмме Воронова и не учитываем в корреляции.
			clusterStarCount--;
			continue;
		}

		//если гипотеза соответствия кластеров star1 и starT1 уже проверялась на этапе
		//масштабонезависомого сравнения кластеров используем эту корреляцию.

		//если центральная OCRStar кластера буквы и OCRStar текста совпадают по lookup 1,2,3
		if (flagCorrelation && star.cluster == 0) {
			//вычисляем корреляцию OCRStar кластера
			correlation1 = star.cMatch[index];
			//если star1 является кластером и ее корреляция уже известна то копируем величину корреляции
			if (correlation1 > 95) {
				clusterCorrelation += correlation1;
				continue;
			}
		}

		DR("star__:" << star.index << "  corr__star:" << star1.index << " starT:" << starT.star[star.sMatch[index]]
					 << " c:" << correlation1 << endl);

		//вычисляем корреляцию всех фокальных линий OCRStar лучей кластера
		//вычисляем порядок соответствия OCRStar лучей кластера
		int min = 360;
		int minIndex = 0;
		int angle = star.angle[index];
		for (int m = 0; m < starT.starCount; m++) {
			int dl = abs(starT.angle[m] - angle);
			if (dl >= 325) dl = 360 - dl;
			if (dl < min) {
				min = dl;
				minIndex = m;
			}
		}
		star.sMatch[index] = minIndex; //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона
		star1.match[0] = starT.star[minIndex];

		OCRStar &starT1 = starArray[star1.match[0]];

		//проверяем OCRStar лучей кластера star1 и starT1 по lookup3
		correlation1 = OCRStarCorrelationLookup2(starT1, star1, graph.focalLine.dataPtr(), letter.line,NULL,NULL,0);

		if (correlation1 < 95) { correlation1 = voronovDiagramCorrelation(starT1, star1, graph, letter); }

		if (correlation1 > 70) {
			clusterCorrelation += correlation1;
			DR("   @V cluster correlation:" << correlation1 << " starT:" << starT1.index << endl << endl);
		} else {
			//для этой OCRStar кластера не нашлось соответствующей OCRStar текста
			//проверяем все OCRStar текста в пределах лимита координат

			int x = (letter.xMatch + star1.cPoint.x * letter.scale);
			int y = (letter.yMatch + star1.cPoint.y * letter.scale);

			OCRPoint adrFocalPoint[64];
			int limit = star1.lineW * 3 * letter.scale;
			if (limit < LINE_SIZE * 3 * letter.scale) limit = LINE_SIZE * 3 * letter.scale;

			//находим все фокальные точки по заданному лимиту координат
			int sizeAdr = graph.detectFocalPoint(x - limit / 2, y - limit / 2, limit, limit, graph.columns(),
				graph.rows(), graph.bites_data_32, adrFocalPoint);
			DR("   sizeAdr:" << sizeAdr << endl);

			//находим ID номер фокальной точки и проверяем корреляцию
			int maxCorr = 0;
			int maxCorrIndex = 0;
			star1.matchCount = 1;
			for (int i = 0; i < sizeAdr; i++) {
				OCRPoint &p = adrFocalPoint[i];
				//находим ID номер фокальной точки и ее исходные координаты
				int id = graph.id_data[p.y * w + p.x] - 1; // ID номера в id_data больше на 1
				//проверяем занятость точки текста
				if (pointReg[id]) continue;
				OCRStar &starT1 = graph.starArray[id];
				DR("      ___detect star:" << starT1.index << endl);

				//вычисляем соответствие по lookup 3;
				// lookup 1 и 2 не выполняем, поскольку на этом этапе необходимо выяснить максимальную корреляцию
				// correlation1=OCRStarCorrelationLookup3(starT1, star1, letter.point, letter.line);
				// if(correlation1<95){
				correlation1 = voronovDiagramCorrelation(starT1, star1, graph, letter);
				//}
				if (correlation1 > maxCorr) {
					maxCorr = correlation1;
					maxCorrIndex = id;

					//в проверяемом участке тексте может быть несколько гипотез OCRStar эталона.
					//предполагаем что их не может быть больше 4
					star1.match[star.matchCount] = id;
					star1.matchCount++;
					if (star.matchCount == 4) star1.matchCount = 3;
				}
			}
			//вычисляем корреляцию всех фокальных линий OCRStar кластера
			clusterCorrelation += maxCorr;
			star1.corrC = maxCorr;
			star1.match[0] = maxCorrIndex;
			DR("   @V cluster correlation:" << maxCorr << " starT:" << maxCorrIndex << endl << endl);
		}
	}
	clusterCorrelation = clusterCorrelation / clusterStarCount;

	//резервируем ОCRStar текста соответствующие найденому кластеру
	if (star.match[0] < pointReg.size()) {
		pointReg[star.match[0]] = 1;
		//все линии кластера текста отмечаем как проверенные
		OCRStar &starT = graph.starArray[star.match[0]];
		for (int index = 0; index < starT.lineCount; index++) { letterLineReg[starT.line[index]]++; }
	}
	star.corrC = clusterCorrelation;

	DR(" //------ LookUp4 star: " << star.index << " correlation:" << clusterCorrelation << endl);

	return clusterCorrelation;
	*/
	return 0;
	
} //_________________________________________________________

int GGraphOCR::voronovDiagramCorrelation(OCRStar &starT, OCRStar &star, GGraph &graph, OCRLetter &letter, OCRCluster &cluster, OCRData32 &lineCor) {
    /**
     Проверка корреляции фокальной линии по диаграме Воронова.
     */
    
    
    int print = 0;
    if (printCorrelation) print = printCorrelation;
    //int dataSize = graph.data_size - 1;
    
    OCRData *line_data = graph.line_data;
    
    int correlationCluster = 0;
    int starLineCount = star.lineCount;
    lineCor.len=0;
    countVoronov++;
    if(countVoronov==3){
        ;
    }
    
    //для проверки совпадения OCRStar эталона и OCRStar текста
    //необходимо совместить их центры.
    //для этого вычисляем разность координат OCRStar кластера эталона и OCRStar текста
    //это необходимо потому, что координаты точек фокальных линий кластера отсчитываются от центра кластера
    
    int clusterXMatch=cluster.cPointText.x;
    int clusterYMatch=cluster.cPointText.y;
    float scale=letter.scale;
    int x=star.cPoint.x;
    int y=star.cPoint.y;
    
    //DR(" star.cPoint.x * scale:"<< x * scale<<endl);
    
    int dX = starT.cPoint.x - (clusterXMatch + x * scale);
    int dY = starT.cPoint.y - (clusterYMatch + y * scale);  //?
    uint w = graph.columns();
    uint h = graph.rows();
    int reg=0;
    char *regC=(char*)&reg;  //в регистре отмечаем проверенные линии
    
    //для всех линий OCRStar эталона
    for (int index = 0; index < star.lineCount; index++) {
        OCRLine &line = cluster.line[star.line[index]];
        OCRLine *lineT;
        int correlation = 0;
        int corrCount = 0;
        
        
        //определяем ID номер соответствующей фокальной линии текста
        //для этого необходимо знать порядок соответствия линий OCRStar лучей кластера эталона и текста
        //находим соответствие лучей
        //#@ необходимо резервировать соответствие уже проверенных лучей кластера в тексте
        int min = 360;
        int minIndex = -1;
        int angle = star.angle[index];
        for (int m = 0; m < starT.lineCount; m++) {
            int dl = abs(starT.angle[m] - angle);
            if (dl >= 325) dl = 360 - dl;
            if (dl < min && !regC[m]) {
                min = dl;
                minIndex = m;
            }
        }
        
        // <-- эти правила игнорируют короткие линии и форму коротких линий
        // применение этих правил к тексту необходимо разрешать в корреляционных весах линий буквы.
        if(modeVoronov){

            if (min > 35 || minIndex < 0) {  //если одна OCRLine эталона не находит соответствие в тексте
                //если линия эталона слишком короткая не учитываем ее в корреляции
                //это правило нужно учесть в матрице условных переходов
                if(!line.cType){
                    starLineCount--;
                    continue;
                }
                //если в OCRStar эталона больше линий чем в OCRStar текста прекращаем проверку
                //это правило может быть установлено в матрице переходов
                return 0;
                //continue;
            }
            //создаем указатель на проверяемую линию текста
            lineT = &graph.focalLine[starT.line[minIndex]];
        
            //если одна из линий слишком короткая
            if(!line.cType){
                //если вторая линия также короткая
                if(lineT->lenA/2<line.lenA){
                    starLineCount--;
                    lineCor.data[lineCor.len] = lineT->index;
                    lineCor.len++;
                }continue;
                
            }
            if(!lineT->cType){
                //если первая линия длинная, а вторая линия короткая
                if(line.lenA/2<lineT->lenA){
                    lineCor.data[lineCor.len]=lineT->index;
                    lineCor.len++;
                    correlationCluster+=100;
                }continue;
            }

            if(line.cType == LINE && lineT->cType == LINE){
                //если обе линии прямые
                if(lineT->lenA/3<line.lenA){
                    lineCor.data[lineCor.len]=lineT->index;
                    lineCor.len++;
                    correlationCluster+=100;
                }continue;
            }
        // -->
        }else{
            if (min > 35 || minIndex < 0) {  //если одна OCRLine эталона не находит соответствие в тексте
                //если в OCRStar эталона больше линий чем в OCRStar текста прекращаем проверку
                return 0;
            }
            //создаем указатель на проверяемую линию текста
            lineT = &graph.focalLine[starT.line[minIndex]];
        }
        
        regC[minIndex]=1;
        // uint idT=lineT.parentID;
        uint idT = lineT->index;
        
        //проверяем линию по всей длине по диаграмме Воронова
        // id номер линии в line_data это id номер родительской линии увеличенный на единицу
        //проверяем точки 1/3 1/2 2/3 фокальной линии
        //определяем переменные относительного смещения точки линии относительно соответствующей точки фокальной линии текста
        int dX_extrem = 0;
        int dY_extrem = 0;
        //определяем направление линии
        int dir=star.dir[index];
        int letterLineLen=line.lenA * letter.scale;
        int chink=letterLineLen/5;
        int letterLineLenSum=0; //необходимо учитывать какая часть линии текста соответствует проверяемой линии буквы
        int dirText=-1; //направление проверяемой линии текста
        int pointIndexText=-1;
    
        for (int i = 0; i <5; i++) { //цикл по количеству проверочных точек линии буквы
            corrCount++;
            int shiftP=i;
            if(!dir)shiftP=4-i;

            OCRPoint &pL = line.pointC[shiftP];
            int x = clusterXMatch + pL.x * scale + dX + dX_extrem;
            int y = clusterYMatch + pL.y * scale + dY + dY_extrem;
            if (x < 0 || y < 0 || x >= w || y >= h) continue;
            
            OCRData &p = line_data[y * w + x];
            //cout<<" p__  "<<x<<":"<<y<< " id:"<<p.d0<<" pId:"<<(int)p.d1<<" cId:"<<(int)p.d2<<endl;
            
            //p.d0 - ID номер линии текста
            //p.d1 - ID номер точки линии (пиксельное расстояние до начала линии текста)
            //p.d2 - расстояние до ближайшей точки линии текста
            
            if (p.d0 - 1 == idT) {
                //точка совпала по диаграмме Воронова
                correlation+=100;
                letterLineLenSum += chink;
                if(pointIndexText>-1){
                    if(p.d1>pointIndexText){
                        dirText=1;
                    }else{
                        dirText=0;
                    }
                }
                pointIndexText=p.d1;
                //#@ пока поддергивание ухудшает совпадение
                //если расстояние от проверяемой точки до ближайшей фокальной линии текста больше 2 пикселов
                if (p.d2 > 2) {
                    //вычисляем смещение найденной точки относительно соответствующей точки
                    //фокальной линии текста
                    //для этого находим соответствующую фокальную линию текста.
                    //в переменной p.d0-1 записан id номер родительской линии.
                    OCRLine &lineT = graph.focalLine[p.d0 - 1];
                    int pointL = p.d1;
                    int shift = 0;
                    int n = 0;
                    int pointIndex = lineT.pointIndex;
                    
                    if (pointL > lineT.lenA) {
                        shift = lineT.lenA;
                        OCRLine &lineT1 = graph.focalLine[lineT.node[n]];
                        pointIndex = lineT1.pointIndex;
                        
                        while (pointL - shift > lineT.lenA) {
                            OCRLine &lineT1 = graph.focalLine[lineT.node[n]];
                            pointIndex = lineT1.pointIndex;
                            shift += lineT1.lenA;
                            n++;
                        }
                    }
                    OCRPoint &pT = graph.pointArray[pointIndex + pointL - shift];
                    dX_extrem = pT.x - x;
                    dY_extrem = pT.y - y;
                }
                
            } else {
                //проверка линии текста обнаружила новую линию текста котрую нужно проверить на соответствие проверяемоцй линии буквы
                //проверяем какая часть предыдущей проверяемой линии текста соответствует проверяемой линии эталона
                if(letterLineLenSum > lineT->lenA * 0.7){
                    lineCor.data[lineCor.len] = lineT->index;
                    lineCor.len++;
                    letterLineLenSum = 0;
                }

                
                //проверяем есть ли в ближайшем окружении линия-продолжение
                DR("                start search joined line to line:"<<idT<<" p.d0:"<<p.d0-1<<endl);
                if(!p.d0)continue;
                //вызываем линию по ее индексу, меняем указатель на проверяемую линию текста
                lineT = &graph.focalLine[p.d0 - 1];

                //на этом этапе ориентация линии определена по порядку совпадения точек проверяемой линии текста и линии эталона
                //поэтому мы проверяем OCRStar соответственно в начале или конце линии
                if(dirText!=-1){
                    if(dirText){
                        OCRStar &starT1=graph.starArray[lineT->endID];
                        for(int n=0;n<starT1.lineCount;n++){
                            if(starT1.line[n]==idT){
                                idT=p.d0 - 1;
                                correlation+=100;
                                break;
                            }
                        }
                    }else{
                        OCRStar &starT2=graph.starArray[lineT->startID];
                        for(int n=0; n<starT2.lineCount; n++){
                            if(starT2.line[n]==idT){
                                idT=p.d0 - 1;
                                correlation+=100;
                                break;
                            }
                        }
                    }
                }else{
                    //если направление линии текста не установлено,
                    //на этом этапе мы не знаем ориентацию линии (мы можем быть в середине составной линии)
                    //поэтому мы проверяем OCRStar в начале и конце линии
                    OCRStar &starT1=graph.starArray[lineT->startID];
                    int flag=1;
                    for(int n=0;n<starT1.lineCount;n++){
                        if(starT1.line[n]==idT){
                            idT=p.d0 - 1;
                            correlation+=100;
                            flag=0;
                            break;
                        }
                    }
                    OCRStar &starT2=graph.starArray[lineT->endID];
                    for(int n=0;n<starT2.lineCount && flag;n++){
                        if(starT2.line[n]==idT){
                            idT=p.d0 - 1;
                            correlation+=100;
                            flag=0;
                            break;
                        }
                    }
                }
                pointIndexText = -1;
                dirText=-1;
            }
        }
        //проверка линии буквы закончена
        //проверяем какая часть проверяемой линии текста соответствует проверяемой линии эталона
        if(letterLineLenSum > lineT->lenA * 0.5){
                lineCor.data[lineCor.len]=lineT->index;
                lineCor.len++;
        }

        if (corrCount) {
            correlation = correlation / corrCount;
            correlationCluster += correlation;
            
        }
        DR("         V correlation:" << correlation <<" correlationCluster:"<<correlationCluster<< endl);
    }
    if (!starLineCount) return 0;
    
    correlationCluster = correlationCluster / starLineCount;
    DR("      V star correlation:" << correlationCluster << endl);
    return correlationCluster;
}

    
    
};
