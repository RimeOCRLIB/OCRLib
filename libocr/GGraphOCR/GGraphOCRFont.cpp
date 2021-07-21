// _______GraphOCRFont.cpp
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

//** @bref создает html файлы изображений букв OCR шрифта */
void GGraphOCR::drawFont() {

} //_____________________________________________

//** @bref создает html файл изображения буквы */
void GGraphOCR::drawLetter2Path(OCRLetter &letter, const char *path) {
    OCRLetter &letter_ = font.letterByID(letter.letterID);
    draw.printLetterSVG((void*)&letter_, (void*)&font, path);
} //_________________________________________________________


//draw function
void GGraphOCR::drawLetter(OCRLetter &letter){
    OCRLetter &letter_ = font.letterByID(letter.letterID);
    draw.printLetterSVG((void*)&letter_, (void*)&font, "/OSBL/CORRELATION_2/");
}
void GGraphOCR::drawLetterByID(int letterID){
    OCRLetter &letter = font.letterByID(letterID);
    draw.printLetterSVG((void*)&letter, (void*)&font, "/OSBL/CORRELATION_2/");
}

void GGraphOCR::drawGraph(GGraph &graph){
    draw.printGraphSVG((void*)&graph, "/OSBL/CORRELATION/");
}
    
void GGraphOCR::drawLetterMatch(OCRLetterMatch &letterMatch){
    draw.printLetterMatch((void*)&letterMatch, (void*)&font, "/OSBL/CORRELATION_3/");
}
    
void GGraphOCR::buildBaseOCRLetterGroup(vector<OCRLetter>&matchVector){
    // алгоритм май 2021
    // Функция создания группы распознаваемых букв
    // Подробное описание принципов работы группы распознаваемых букв находится в @fn buildAlphabet()

    //сначала проверяем существует ли группа соответствующаяя найденным похожим буквам.
    //при необходимости создаем новую группу.
    

    OCRLetter &letter=matchVector[0];
    //OCRLetter &letterMain=font[letter.index];
    int groupIndex=0;
    //Создаем новую группу
    OCRGroupData &group=font.letterGroupData[letter.index];
    group.index=letter.index;
    groupIndex=letter.index;
    group.size=0;
    
    OCRLetterMatch letterMatch; 
    letterMatch.baseIndex=letter.index;
    letterMatch.testIndex=letter.index;
    letterMatch.matchIndex=(uint)font.letterMatch.size();
    letterMatch.xCenter=letter.xCenter;
    letterMatch.yCenter=letter.yCenter;
    letterMatch.scale=1;
    letterMatch.OCRIndex=1;
    int lineCount=letter.lineCount;
    letterMatch.lineCount=lineCount;
    for(int i=0;i < lineCount; i++){
        letterMatch.kLine[i]=1;
    }
    GString name=letter.name;
    
    //записываем OCRLetterMatch базовой буквы в группу базовой буквы
    group.letter[group.size]=letter.index;
    //записываем letterMatch в массив font.letterMatch

    group.match[group.size]=letterMatch.matchIndex;
    font.letterMatch.push_back(letterMatch);
    group.size++;
    
    //записываем все OCRLetterMatch найденных букв в группу базовой буквы
    
    for(int i=1;i < matchVector.size(); i++){
        OCRLetter &letterTest=matchVector[i];
        //создаем OCRLetterMatch этой буквы
        letterMatch.baseIndex=letter.index;
        letterMatch.testIndex=letterTest.index;
        letterMatch.matchIndex=(uint)font.letterMatch.size();
        letterMatch.xCenter=letterTest.xMatch; //устанавливаем центр как центр найденный проверяемой буквой в базовой букве 
        letterMatch.yCenter=letterTest.yMatch;
        letterMatch.scale=letterTest.scale;
        letterMatch.corr=letterTest.correlation;
        letterMatch.lineCount=letterTest.matchLineCount;
        memset(letterMatch.kLine,0,MAX_CLUSTER_LETTER * sizeof(float));
        for(int n=0;n < letterTest.matchLineCount; n++){
            letterMatch.kLine[letterTest.lineMatch[n]]=1;
        }
        if(letterTest.name==name){
            letterMatch.OCRIndex=1;
        }else{
            letterMatch.OCRIndex=0;
        }
        //записываем букву в группу
        group.letter[group.size]=letterTest.index;
        group.match[group.size]=letterMatch.matchIndex;
        font.letterMatch.push_back(letterMatch);
        group.size++;
        if(group.size>MAX_LETTER_GROUP-1){
            cout<<"need increase MAX_LETTER_GROUP 1"<<endl;
            exit(0);
        }
        
    }


    
}//____________________________________________________________________________________________________
    
void GGraphOCR::buildOCRLetterGroup(vector<OCRLetter>&matchVector){
/*
    //на втором этапе создания стека буквы используем веса линий базовых букв вычисленных в @fn buildBaseOCRLetterStack
    //для каждого OCRLetterMatch базовой буквы вычисляем веса каждой OCRLine и записываем в массив kLine
    
    OCRLetter &letter=font[matchVector[0].index];
    //переписываем в стек буквы данные из matchVector
    for(int i=1;i < matchVector.size(); i++){
        OCRLetter &letterTest=matchVector[i];
        OCRLetterMatch &letterMatch=letter.stack[letter.matchLineCount];
        letterMatch.OCRIndex=0; //буква не совпадает с базовой по Юникоду
        letter.matchLineCount++;

        letterMatch.index=letterTest.index;
        letterMatch.letterID=letterTest.letterID;
        letterMatch.xCenter=letterTest.xCenter;
        letterMatch.yCenter=letterTest.yCenter;
        letterMatch.scale=letterTest.scale;
        for(int n=0;n < letterTest.matchLineCount; n++){
            letterMatch.kLine[n]=letterTest.lineMatch[n];
        }
    }
    
    
    //далее анализируем данные записанные в стек базовой буквы
    for(int i=1;i < letter.matchLineCount; i++){
        OCRLetterMatch &match=letter.stack[i];
        if(match.OCRIndex){
            //для букв стека совпадающих с базовой буквой по значению Юникод
            //переписываем в OCRLetterMatch этой буквы значения весов линий из стека этой буквы
            
            OCRLetter &letterTest=font[match.index];
            memcpy((char*)match.kLine,(char*)letterTest.stack[0].kLine,sizeof(float) * letterTest.lineCount);
        }else{
            //для букв стека не совпадающих с базовой буквой по значению Юникод
            //вычисляем веса OCRLine графической разности.
            
            OCRLetter &letterTest=font[match.index];
            //читаем OCRLetterMatch этой буквы
            OCRLetterMatch &matchTestLetter = letterTest.stack[0];
            //записываем вес каждой линии этой буквы не совпадающей ни с одной из OCRLine базовой буквы
            for(int n=0;n < letterTest.lineCount; n++){
                if(!match.kLine[n]){
                    match.kLine[n]=matchTestLetter.kLine[n];
                }else{
                    match.kLine[n]=0;
                }
            }
        }
    }
    cout<<1;
*/


}//____________________________________________________________________________________________________
    
} // namespace ocr
