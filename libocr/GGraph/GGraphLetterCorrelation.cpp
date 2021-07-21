//@__________GGraphLetterCorrelation.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org.

#include "GGraph.h"
#include "../OCRString/php2stl.h"

namespace ocr
{


/** @bref Функция вычисления корреляции группы звездочек.
				Исходный текст  */
void GGraph::letterCorrelation(OCRLetter &letter)
{
	int print = 1;
	if (print)
		cout << "letterCorrelation()" << endl;
	/*
	    for(int index=0;index<starArray.size()index++){
	    	cout<<index<<" "<<(int)starArray[index].type<<endl;
	    }*/

    //OCRStar &star = starArray[0];

	return;

    //OCRStar &lStar0 = starArray[0];

	// Определение коэффициента корреляции звездочки по относительным углам в % //
	// с вращением влево и вправро на одну позицию. //
    /*
	for (int index = 0; index < starArray.size(); index++)
	{
		OCRStar &star = starArray[index];
		if (lStar0.type == star.type)
		{
			int aCor = 360;
			int ord = 0;

			if (lStar0.type == T_POINT)
			{
				int aCor0 = 0;
				aCor0 += abs(star.mL[0] - lStar0.mL[0]);
				aCor0 += abs(star.mL[1] - lStar0.mL[1]);
				aCor0 += abs(star.mL[2] - lStar0.mL[2]);

				int aCor1 = 0;
				aCor1 += abs(star.mL[1] - lStar0.mL[0]);
				aCor1 += abs(star.mL[2] - lStar0.mL[1]);
				aCor1 += abs(star.mL[0] - lStar0.mL[2]);

				int aCor2 = 0;
				aCor2 += abs(star.mL[2] - lStar0.mL[0]);
				aCor2 += abs(star.mL[0] - lStar0.mL[1]);
				aCor2 += abs(star.mL[1] - lStar0.mL[2]);
                
				// выбор максимального коэффициента корреляции звездочки
				// из трех возможных вариантов
				if (aCor > aCor0)
				{
					aCor = aCor0;
				}
				if (aCor > aCor1)
				{
					aCor = aCor1;
					ord = 1;
				}
				if (aCor > aCor2)
				{
					aCor = aCor1;
					ord = 2;
				}
			}
			else if (lStar0.type == X_POINT)
			{
				int aCor0 = 0;
				aCor0 += abs(star.mL[0] - lStar0.mL[0]);
				aCor0 += abs(star.mL[1] - lStar0.mL[1]);
				aCor0 += abs(star.mL[2] - lStar0.mL[2]);
				aCor0 += abs(star.mL[3] - lStar0.mL[3]);

				int aCor1 = 0;
				aCor1 += abs(star.mL[1] - lStar0.mL[0]);
				aCor1 += abs(star.mL[2] - lStar0.mL[1]);
				aCor1 += abs(star.mL[3] - lStar0.mL[2]);
				aCor1 += abs(star.mL[0] - lStar0.mL[3]);

				int aCor2 = 0;
				aCor2 += abs(star.mL[3] - lStar0.mL[0]);
				aCor2 += abs(star.mL[0] - lStar0.mL[1]);
				aCor2 += abs(star.mL[1] - lStar0.mL[2]);
				aCor2 += abs(star.mL[2] - lStar0.mL[3]);

				// выбор максимального коэффициента корреляции звездочки
				// из трех возможных вариантов
				if (aCor > aCor0)
				{
					aCor = aCor0;
				}
				if (aCor > aCor1)
				{
					aCor = aCor1;
					ord = 1;
				}
				if (aCor > aCor2)
				{
					aCor = aCor1;
					ord = 2;
				}
			}

			// Коэффициент корреляции звездочки текста и эталона по относительным углам в %
			// (углам между лучами).
			aCor = 100 - (aCor * 100) / 360;
			cout << "aCor:" << aCor << endl;
			if (aCor < 50)
				break;

			// Расчет коэффициент корреляции звездочки текста и эталона по относительным длинам лучей в %. //
			int lCor = 0;
			if (lStar0.type == T_POINT)
			{
				if (ord == 0)
				{
					lCor += abs(star.lS[0] - lStar0.lS[0]);
					lCor += abs(star.lS[1] - lStar0.lS[1]);
					lCor += abs(star.lS[2] - lStar0.lS[2]);

					lCor += abs(star.lP[0] - lStar0.lP[0]);
					lCor += abs(star.lP[1] - lStar0.lP[1]);
					lCor += abs(star.lP[2] - lStar0.lP[2]);
				}
				else if (ord == 1)
				{
					lCor += abs(star.lS[1] - lStar0.lS[0]);
					lCor += abs(star.lS[2] - lStar0.lS[1]);
					lCor += abs(star.lS[0] - lStar0.lS[2]);

					lCor += abs(star.lP[1] - lStar0.lP[0]);
					lCor += abs(star.lP[2] - lStar0.lP[1]);
					lCor += abs(star.lP[0] - lStar0.lP[2]);
				}
				else if (ord == 2)
				{
					lCor += abs(star.lS[2] - lStar0.lS[0]);
					lCor += abs(star.lS[0] - lStar0.lS[1]);
					lCor += abs(star.lS[1] - lStar0.lS[2]);

					lCor += abs(star.lP[2] - lStar0.lP[0]);
					lCor += abs(star.lP[0] - lStar0.lP[1]);
					lCor += abs(star.lP[1] - lStar0.lP[2]);
				}
				// коэффициент корреляции звездочки "T" текста и эталона по относительным длинам лучей в %
				lCor = 100 - lCor / 6;
			}
			else if (lStar0.type == X_POINT)
			{
				if (ord == 0)
				{
					lCor += abs(star.lS[0] - lStar0.lS[0]);
					lCor += abs(star.lS[1] - lStar0.lS[1]);
					lCor += abs(star.lS[2] - lStar0.lS[2]);
					lCor += abs(star.lS[3] - lStar0.lS[3]);

					lCor += abs(star.lP[0] - lStar0.lP[0]);
					lCor += abs(star.lP[1] - lStar0.lP[1]);
					lCor += abs(star.lP[2] - lStar0.lP[2]);
					lCor += abs(star.lP[3] - lStar0.lP[3]);
				}
				else if (ord == 1)
				{
					lCor += abs(star.lS[1] - lStar0.lS[0]);
					lCor += abs(star.lS[2] - lStar0.lS[1]);
					lCor += abs(star.lS[3] - lStar0.lS[2]);
					lCor += abs(star.lS[0] - lStar0.lS[3]);

					lCor += abs(star.lP[1] - lStar0.lP[0]);
					lCor += abs(star.lP[2] - lStar0.lP[1]);
					lCor += abs(star.lP[3] - lStar0.lP[2]);
					lCor += abs(star.lP[0] - lStar0.lP[3]);
				}
				else if (ord == 2)
				{
					lCor += abs(star.lS[3] - lStar0.lS[0]);
					lCor += abs(star.lS[0] - lStar0.lS[1]);
					lCor += abs(star.lS[1] - lStar0.lS[2]);
					lCor += abs(star.lS[2] - lStar0.lS[3]);

					lCor += abs(star.lP[3] - lStar0.lP[0]);
					lCor += abs(star.lP[0] - lStar0.lP[1]);
					lCor += abs(star.lP[1] - lStar0.lP[2]);
					lCor += abs(star.lP[2] - lStar0.lP[3]);
				}
				// коэффициент корреляции звездочки "X" текста и эталона по относительным длинам лучей в %
				lCor = 100 - lCor / 8;
			}

			cout << "lCor:" << lCor << endl;
			if (lCor < 50)
				break;

			// Вычисление масштаба звездочки текста по сумме длин истинной (пиксельной) кривых образующих //
			// звездочку и периметр звездочки. //
			float scale = (float)lStar0.sumLen1 / star.sumLen1;
			cout << "scale:" << scale << " text sumLen:" << star.sumLen << " letter sumLen:" << lStar0.sumLen << endl;

			//расчитываем угол поворота звездочки текста отосительно эталона
			float deltaA = 0;
			if (lStar0.type == T_POINT)
			{
				if (ord == 0)
				{
					deltaA += star.mA[0] - lStar0.mA[0];
					deltaA += star.mA[1] - lStar0.mA[1];
					deltaA += star.mA[2] - lStar0.mA[2];
				}
				else if (ord == 1)
				{
					deltaA += star.mA[1] - lStar0.mA[0];
					deltaA += star.mA[2] - lStar0.mA[1];
					deltaA += star.mA[0] - lStar0.mA[2];
				}
				else if (ord == 2)
				{
					deltaA += star.mA[2] - lStar0.mA[0];
					deltaA += star.mA[0] - lStar0.mA[1];
					deltaA += star.mA[1] - lStar0.mA[2];
				}
				//угол поворота звездочки "T" текста отосительно эталона
				deltaA = (float)deltaA / 3;
			}
			else if (lStar0.type == X_POINT)
			{
				if (ord == 0)
				{
					deltaA += star.mA[0] - lStar0.mA[0];
					deltaA += star.mA[1] - lStar0.mA[1];
					deltaA += star.mA[2] - lStar0.mA[2];
					deltaA += star.mA[3] - lStar0.mA[3];
				}
				else if (ord == 1)
				{
					deltaA += star.mA[1] - lStar0.mA[0];
					deltaA += star.mA[2] - lStar0.mA[1];
					deltaA += star.mA[3] - lStar0.mA[2];
					deltaA += star.mA[0] - lStar0.mA[3];
				}
				else if (ord == 2)
				{
					deltaA += star.mA[3] - lStar0.mA[0];
					deltaA += star.mA[0] - lStar0.mA[1];
					deltaA += star.mA[1] - lStar0.mA[2];
					deltaA += star.mA[2] - lStar0.mA[3];
				}
				//угол поворота звездочки "X" текста отосительно эталона
				deltaA = (float)deltaA / 4;
			}
			cout << "deltaA:" << deltaA << endl;

			//------------------------------------------------------------------------------------------------------

			// Коэффициент корреляции звездочки текста и эталона по форме кривых образующих звездочку в %. //
			// Алгоритм масштабонезависим ! //
			// КОД вставить на это место, сам код с.м. ниже.

			//------------------------------------------------------------------------------------------------------

		} // if(lStar0.type==star.type){

	} // for(int index=0;index<starArray.size();index++);
     
    */ 
     
} //_________________________________________________________

} //namespace ocr
