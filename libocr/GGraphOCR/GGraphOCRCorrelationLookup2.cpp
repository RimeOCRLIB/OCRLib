// ____GGraphOCRCorrelationlookup2.cpp
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

#include "GGraphOCR.h"

namespace ocr {
	
/** @bref ВТОРОЙ lookup кореляции кластеров @fn OCRStarCorrelation
	Сравнение кластеров по концам лучей в нормированной маске 32х32 */
int GGraphOCR::clusterMaskCorrelation(OCRMask &maskT,OCRMask &mask){
    /*
	int correlation = 0;
	int regCount;
	//cout<<"mask.regCount:"<<(int)mask.regCount<<" maskT.regCount:"<<(int)maskT.regCount<<endl;
	
	if(mask.regCount>maskT.regCount){
		regCount=mask.regCount;
		for(int i=0;i<regCount;i++){
			int regT = maskT.mask[mask.regY[i]];
			//cout<<bitset<32>(regT)<<endl;
			int reg = mask.reg[i];
			//cout<<bitset<32>(reg)<<endl<<endl;
			if ((reg & regT) == reg) correlation += 100;
		}
	}else{
		regCount=maskT.regCount;
		for(int i=0;i<regCount;i++){
			int regT= mask.mask[maskT.regY[i]];
			//cout<<bitset<32>(regT)<<endl;
			int reg = maskT.reg[i];
			//cout<<bitset<32>(reg)<<endl<<endl;
			if ((reg & regT) == reg) correlation += 100;
		}
	}
	//cout<<"correlation:"<<correlation<<" "<<correlation/regCount<<endl;
    if(!regCount)return 0; //#
	return correlation/regCount;
    */
    return 0;
}

/** @bref Сравнение  двух OCRline по нормированной маске 32х32 */
int GGraphOCR::OCRLineMaskCorrelation(OCRLine &lineT, OCRLine &line) {
    /*
	int correlation = 0;
	if (line.SumBitOn == 0 && lineT.SumBitOn == 0) return 100;
	if (line.SumBitOn == 0 || lineT.SumBitOn == 0) {
		//если одна линия короткая то сравниваем относительные длины линий
		int dl = abs((lineT.lenA * 100) / lineT.lineW - (line.lenA * 100) / line.lineW);
		// cout<<"dl:"<<dl<<endl;
		if (dl < 100) return 70;
		if (dl < 200) return 50;
		if (dl < 300) return 10;  //необходимо проверить изгиб линий.
	}
	//определяем порядок сравнения
	if (line.SumBitOn >= lineT.SumBitOn) {
		int *lineMask = lineT.maskLine;
		int reg;
		reg = lineMask[line.p0_y];
		if ((reg | line.p0_r) == reg) correlation += 15;
		reg = lineMask[line.p1_y];
		if ((reg | line.p1_r) == reg) correlation += 20;
		reg = lineMask[line.p2_y];
		if ((reg | line.p2_r) == reg) correlation += 30;
		reg = lineMask[line.p3_y];
		if ((reg | line.p3_r) == reg) correlation += 20;
		reg = lineMask[line.p4_y];
		if ((reg | line.p4_r) == reg) correlation += 15;
	} else {
		int *lineMask = line.maskLine;
		int reg;
		reg = lineMask[lineT.p0_y];
		if ((reg | lineT.p0_r) == reg) correlation += 15;
		reg = lineMask[lineT.p1_y];
		if ((reg | lineT.p1_r) == reg) correlation += 20;
		reg = lineMask[lineT.p2_y];
		if ((reg | lineT.p2_r) == reg) correlation += 30;
		reg = lineMask[lineT.p3_y];
		if ((reg | lineT.p3_r) == reg) correlation += 20;
		reg = lineMask[lineT.p4_y];
		if ((reg | lineT.p4_r) == reg) correlation += 15;
	}
	return correlation;
    */
    return 0;
}

/** @bref ВТОРОЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение звездочек по форме лучей в нормированных масках 32х32  */
int GGraphOCR::OCRStarCorrelationLookup2(
			OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines,  uchar *lineCorr, uchar *sMatch, int mode) {
	/**
		МАСШТАБО НЕЗАВИСИМЫЙ lookup
		@img img2.jpg 256
		Алгоритм 3 октября 2019 года
		Алгоритм изменен в сентябре 2020 года

		Для выполнения масштабонезависимого сравнения обе звездочки  сравниваются по маскам составляющих
		их фокальных линий. Маски фокальных линий описаны в функции
		@fn GGraph::buildFocalLineMask()
		Масштабо-независимое сравнение звездочек по лимитированным концам лучей в функции
		@fn GGraphOCR::OCRStarCorrelationLookup1 и маскам фокальных линий эквивалентно
	   сравнению звездочек по форме и углам лучей. Также алгоритм выполняет синхронизацию
	   звездочек по фазе угла поворота. Сравнение звездочек осуществляется за 4*n (n - по
	   числу лучей) обращений к компактным массивам 32 int поэтому данный алгоритм достаточно быстрый.
		*/
		
	int correlation = 0;
    int corr = 0;
   
    if(star.lineCount == 1 && starT.lineCount == 1){
		OCRLine &line = lines[star.line[0]];
		OCRLine &lineT = linesT[starT.line[0]];
        int dir=star.dir[0];
        int dirT=starT.dir[0];
        /*
        //проверяем типы OCRStar концов линий
        if(dir){
            if(dirT){
                if(line.end.type != lineT.end.type)return 0;
            }else if(line.end.type != lineT.start.type) return 0;
        }else{
            if(dirT){
                if(line.start.type != lineT.end.type) return 0;
            }else if(line.start.type != lineT.start.type) return 0;
        }
        */
        
        //проверяем типы OCRStar концов линий
        if(dir){
            if(dirT){
                if(abs(line.end.type - lineT.end.type) > 1)return 0;
            }else if(abs(line.end.type - lineT.start.type)> 1) return 0;
        }else{
            if(dirT){
                if(abs(line.start.type - lineT.end.type) > 1) return 0;
            }else if(abs(line.start.type - lineT.start.type) > 1) return 0;
        }

        
        //если линия эталона слишком короткая и OCRStar на другом конце линии имеет тип L_POINT, не учитываем ее в корреляции
        //это правило нужно учесть в матрице условных переходов
        if(!line.cType ){
            //проверяем длину другой линии
            if(!lineT.cType){
                return 100;
            }else if((float)line.lenA/lineT.lenA < 0.35f) return 0;
        }else if(!lineT.cType){
            //проверяем длину другой линии
            if(!line.cType){
                return 100;
            }else if((float)lineT.lenA/line.lenA < 0.35f) return 0;
        }
        //если в OCRStar эталона больше линий чем в OCRStar текста
        
        int dl = abs(starT.angle[0] - star.angle[0]);
        if (dl >= 180) dl = 360 - dl;

        corr=100;
        if(abs(line.alphaMid - lineT.alphaMid) > limitAlphaMid){
            corr = 0;
        }else{
            if(dir==dirT){
                if(abs(line.alpha1 - lineT.alpha1) > limitAlpha){
                    corr = 0;
                }else if(abs(line.alpha2 - lineT.alpha2) > limitAlpha){
                    corr = 0;
                }
            }else{
                if(abs(line.alpha1 - lineT.alpha2) > limitAlpha){
                    corr = 0;
                }else if(abs(line.alpha2 - lineT.alpha1) > limitAlpha){
                    corr = 0;
                }
            }
            
        }
        return corr;
	}

	//вычисляем порядок соответствия лучей тестовой и эталонной OCRStar
	int min = 360;
	int minIndex = -1;
	int limit = star.lineCount;
	int limitT = starT.lineCount;

	
	//проверяем корреляции соответствующих лучей OCRStar
    int starLineCount;
    if(limit > limitT){
        starLineCount = limit;
    }else{
        starLineCount = limitT;
    }
    
    int reg=0;
    char *regC=(char*)&reg; //в регистре отмечаем проверенные линии
    
	for (int index = 0; index < limit; index++) {
        
        int dir=star.dir[index];
        int dirT=starT.dir[index];
        
        if(star.line[index]>1000000){
            cout<<" line error!:"<<star.line[index]<<endl;
            continue;
        }
        
        OCRLine &line = lines[star.line[index]];
		int angle = star.angle[index];
		min = 360;
		for (int m = 0; m < limitT; m++) {
			int dl = abs(starT.angle[m] - angle);
			if (dl >= 180) dl = 360 - dl;
			if (dl < min && !regC[m]) {
				min = dl;
				minIndex = m;
			}
		}
        if (min > 35 || minIndex < 0) {  //если одна OCRLine эталона не находит соответствие в тексте
            //если линия эталона слишком короткая и OCRStar на другом конце линии имеет тип L_POINT, не учитываем ее в корреляции
            //это правило нужно учесть в матрице условных переходов
            if(!line.cType){
                //проверяем тип OCRStar на другом конце линии
                if(dir){
                    if(line.end.type!=L_POINT){
                        continue;
                    }
                }else{
                    if(line.start.type!=L_POINT){
                        continue;
                    }
                }
                starLineCount--;
                continue;
            }
            //если в OCRStar эталона больше линий чем в OCRStar текста
            continue;
        }
        if(starT.line[minIndex]>1000000){
            cout<<" line error!:"<<starT.line[minIndex]<<endl;
            continue;
        }
        OCRLine &lineT = linesT[starT.line[minIndex]];
        
        
        //проверяем типы OCRStar концов линий
        if(dir){
            if(dirT){
                if(abs(line.end.type - lineT.end.type) > 1)return 0;
            }else if(abs(line.end.type - lineT.start.type) > 1) return 0;
        }else{
            if(dirT){
                if(abs(line.start.type - lineT.end.type) > 1) return 0;
            }else if(abs(line.start.type - lineT.start.type) > 1) return 0;
        }
        
        //проверяем типы линий
        int flag=1;
        if(!line.cType){
            if(!lineT.cType){
                starLineCount--;
                continue;
            }else{
                float k=(float)line.lenA/lineT.lenA;
                if(k < 0.35f){
                    flag=0;
                }
            }
        }else if(!lineT.cType){
            float k=(float)lineT.lenA/line.lenA;
            if(k < 0.35f){
                flag=0;
            }
        }
        if(!flag) continue;
        
        //regC[minIndex]=1;
		//if(mode)sMatch[index]=minIndex;
        //проверяем соответствие кривизны линий (паруса кривых)
        corr=100;
        if(abs(line.alphaMid - lineT.alphaMid) > limitAlphaMid){
            corr = 0;
        }else{
            if(dir==dirT){
                if(abs(line.alpha1 - lineT.alpha1) > limitAlpha){
                    corr = 0;
                }else if(abs(line.alpha2 - lineT.alpha2) > limitAlpha){
                    corr = 0;
                }
            }else{
                if(abs(line.alpha1 - lineT.alpha2) > limitAlpha){
                    corr = 0;
                }else if(abs(line.alpha2 - lineT.alpha1) > limitAlpha){
                    corr = 0;
                }
            }            
        }
        correlation+=corr;
        
		//if (line.SumBitOn > lineT.SumBitOn) {
		//	corr = OCRLineMaskCorrelation(lineT, line);
		//} else {
		//	corr = OCRLineMaskCorrelation(line, lineT);
		//}
		//if(mode)lineCorr[index]=corr; //сохраняем корреляцию луча
		//correlation += corr;
	}
	if(starLineCount==0){
		return 0;
	}
	correlation /= starLineCount;
	
	return correlation;
} //_________________________________________________________


/** @bref Сравнение  двух OCRline по нормированной маске 32х32 с учётом аффинных преобразований */
int GGraphOCR::OCRLineMaskAffineCorrelation(OCRLine &lineT, OCRLine &line, int dir) {
	/*
	//общая формула приведения масштаба и сдвига двух кривых
	int dxS; ///<расстояние между концами линий по X при совмещении координат начала линии
	int dyS; ///<расстояние между концами линий по Y при совмещении координат начала линии
	// int x= p.x + dxS * lenA / i;
	// lenA - длина линии i  - номер пиксела от начала линии
	// int y= p.y + dyS * lenA / i;

	int dX;
	int dY;
	int x, y;

	float scale = line.scale0;
	int correlation=0;
    
    if (dir) {
        //вычисляем смещение начала линий
        dX = lineT.start32.x - line.start32.x;
        dY = lineT.start32.y - line.start32.y;
        //вычисляем смещение концов линий при совмещении начала линий
        dxS = lineT.end32.x - line.end32.x - dX;
        dyS = lineT.end32.y - line.end32.y - dY;
    } else {
        //вычисляем смещение начала линий
        dX = lineT.start32.x - line.end32.x;
        dY = lineT.start32.y - line.end32.y;
        //вычисляем смещение концов линий при совмещении начала линий
        dxS = lineT.end32.x - line.start32.x - dX;
        dyS = lineT.end32.y - line.start32.y - dY;
    }
    int limit = A_LIMIT;
    //cout<<"line:"<<line.index<<" lineT:"<<lineT.index;
    cout<<" dX:"<<dX<<" dY:"<<dY<<" dxS:"<<dxS<<" dyS:"<<dyS<<" A_LIMIT:"<<A_LIMIT<<endl;
    if (abs(dxS) > limit) return 0;
    if (abs(dyS) > limit) return 0;

    
	//проверяем совпадение точек линии по маске проверяемой линии с приведеннием
	//масштаба  и сдвига по X Y и совмещением начала и конца линии

	float stepX=(float)dxS /(line.lenA -1);
	float stepY=(float)dyS /(line.lenA -1);
	int xShift;
	int yShift;
	int dX_=lineT.start32.x;
	int dY_=lineT.start32.y;
	if(dir){
		xShift=line.start.x;
		yShift=line.start.y;
	}else{
		xShift=line.end.x;
		yShift=line.end.y;
	}
	int *lineMask = lineT.maskLine;
	OCRPoint *points =line.pointC;
	int lenA = line.lenA;
	
	//cout<<"@ p0.x:"<<p0.x<<" p0.y:"<<y<<endl;
	//cout<<"lineT:"<<lineT.index<<" line:"<<line.index<<" dir:"<<dir<<endl;
	//cout<<" dX:"<<dX<<" dY:"<<dY<<" dxS:"<<dxS<<" dyS:"<<dyS<<endl;
		
	//cout<<" dl:"<<dl<<" stepX:"<< (int)(stepX * dl)<<" stepY:"<< (int)(stepY * dl)<<" dX_:"<<dX_<<" dY_:"<<dY_<<" xShift:"<<xShift<<" yShift:"<<yShift<<endl;
	//cout<<" point p.x_:"<<(int)((p0.x - xShift) * scale) <<" p.y_:"<<(int)((p0.y - yShift) * scale)<<endl;
		
	for(int index=0;index<5;index++){
		OCRPoint &p0 = points[index];
		int dl=p0.data;
		if (dir) {
			x = (p0.x - xShift) * scale + stepX * dl + dX_;
			y = (p0.y - yShift) * scale + stepY * dl + dY_;
		} else {
			x = (p0.x - xShift) * scale + stepX * (lenA - dl - 1) + dX_;
			y = (p0.y - yShift) * scale + stepY * (lenA - dl - 1) + dY_;
		}
		if ((lineMask[y] >> x) & 1) correlation +=p0.status;
		//cout<<"@ point0 x:"<<x<<" y:"<<y<<" c:"<<correlation<<endl;	
	}
	
	return correlation;
    */
    return 0;
}

/** @bref ВТОРОЙ lookup кореляции звездочек @fn OCRStarCorrelation
	Сравнение звездочек по форме лучей в нормированных масках 32х32 
	c учетом аффинных преобразований*/
int GGraphOCR::OCRStarCorrelationAffineLookup2(OCRStar &starT, OCRStar &star, OCRLine *linesT, OCRLine *lines,uchar *lineCorr, uchar *sMatch) {
	/**
		МАСШТАБО НЕЗАВИСИМЫЙ lookup
		Алгоритм сентябрь 2020 года

		Для выполнения масштабонезависимого сравнения обе звездочки  сравниваются по маскам составляющих
		их фокальных линий. Маски фокальных линий описаны в функции
		@fn GGraph::buildFocalLineMask()
		Функция анализирует уже выполненные проверки корреляции фокальных линий.
		Если проверка формы линий по маске 32x32 имеет не высокую корреляцию, проводится 
		проверка совпадения формы линии с учетом аффинных преобразований - масштаба и сдвига.
		Этот алгоритм описан в функции @fn OCRLineMaskAffineCorrelation 
		*/

	int correlation = 0;
	cout<<" === affine corr star:"<<star.index<<" starT:"<<starT.index<<" corr:"<<star.corr<<endl;
	if (star.type == O_POINT && starT.type == O_POINT) return 100;
	//проверяем корреляцию по lookup 2
	cout<<"start lookup 2"<<endl;
	correlation=OCRStarCorrelationLookup2(starT, star, linesT, lines,lineCorr, sMatch, 1);
	if(correlation>98)return correlation;
	correlation=0;
	
	//проверяем корреляции соответствующих лучей OCRStar
	int corr;
	int limit=star.lineCount;
	//int limitT = starT.starCount;
	int count=0;
	for (int index = 0; index < limit; index++) {
		int indexT = sMatch[index];
		if(indexT==254){
			continue; //линия не имеет соответствия и слишком короткая
		}
		count++;
		if(indexT==255)continue; //линия не имеет соответствия
		cout<<"star.line[index]:"<<star.line[index]<<" starT.line[indexT]:"<<starT.line[indexT]<<endl;
		OCRLine &line = lines[star.line[index]];
		OCRLine &lineT = linesT[starT.line[indexT]];
		int dir=1;
		if(star.dir[index]!=starT.dir[indexT]) dir = 0;
		if (line.SumBitOn > lineT.SumBitOn) {
			corr = OCRLineMaskAffineCorrelation(lineT, line,dir);
		} else {
			corr = OCRLineMaskAffineCorrelation(line, lineT,dir);
		}
		if(corr>lineCorr[index]){
			lineCorr[index]=corr; //сохраняем корреляцию луча
			correlation+=corr;
		}else{
			correlation+=lineCorr[index];
		}
		cout<<" line:"<<line.index<<" lineT:"<<lineT.index<<" corrA:"<<corr<<" corr:"<<(int)lineCorr[index]<<endl;
	}
	correlation /= count;

	// cout<<" correlation l2:"<<correlation<<endl;
	// star.sMatch[index] = minIndex; //сохраняем порядок соответствия фокальных линий  OCRStar текста и эталона

	return correlation;
} //_________________________________________________________


};
