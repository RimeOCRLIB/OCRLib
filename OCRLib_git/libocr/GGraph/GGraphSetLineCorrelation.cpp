//  OCRLib  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .

#include "GGraph.h"
#include "GBitsetMatrix.h"
#include "GFontEditor.h"
#include <math.h> 


namespace ocr{
    
//основная функция распознавания всех букв на странице.
void GGraph::setLineCorrelation(vector<OCRPoint>&focalPoint,
                                vector<OCRFocalLine> &focalLine,
                                GStr2D<int>*pointMatrix,
                                GStr2D<int>*lineMatrix,
                                void *matrix_,
                                void *matchLine_){
    
    //GBitsetMatrix *matrix=(GBitsetMatrix *)matrix_;
    //vector<OCRMatch>&matchLine=*(vector<OCRMatch>*)matchLine_;
    GFont* aliKali=((GFontEditor*)(inputData.fontEditor))->aliKali;
    
    uint fontLetterCount=aliKali->letterCount();
    OCRPoint *fPoint=&focalPoint[0];
    int print=0;
    int mC;
    
    for(int in=1; in<fontLetterCount;in++){
        
        if(in!=9544)continue; print=1;
        //print =0;if(in==16672)print=1;
        
        GLetter* glyph=aliKali->getLetter(in);
        
        if(glyph->mask32Count()==0||!glyph->recordStatus||glyph->focalLine.size()==0){glyph->destroy();continue;}
        
        cout<<"glyph.name="<<glyph->name<<endl;
        
        //если в букве есть фокальные линии то поиск ведем перебором фокальных линий на странице.
        int xLetter=glyph[0].focalLine[0].start.x;
        int yLetter=glyph[0].focalLine[0].start.y;
        
        
        for(int lineIndex=0;lineIndex<focalLine.size();lineIndex++){
            
            cout<<"index="<<lineIndex<<" x="<<
            " alpha1="<<focalLine[lineIndex].alpha<<" alpha2="<<glyph[0].focalLine[0].alpha<<
            " x="<<focalLine[lineIndex].start.x<<endl;
            
            mC=CorrelationBezierT(glyph->focalLine[0],focalLine[lineIndex]);
            cout<<" mC="<<mC<<endl;
            
            break;
            if(mC<70)continue;
            
            //если первая линия совпадает с линией на странице, проверяем все остальные линии буквы

            
            //int xCenter=fPoint[pointIndex].x-xPointLetter;
            //int yCenter=fPoint[pointIndex].y-yPointLetter;
        }
            ///cout<<"i="<<pointIndex<<" x="<<fPoint[pointIndex].x<<" y="<<fPoint[pointIndex].y<<endl;
            
            //int pointsCorrelation=0;
            //проверяем остальные точки.
            //if(glyph[0].fPointCount()>1){
            //    for(int pIndexLetter=1;pIndexLetter<glyph[0].fPointCount();pIndexLetter++){
                    /*
                    int x_=xCenter+glyph[0].fPoint[pIndexLetter].x;
                    int y_=yCenter+glyph[0].fPoint[pIndexLetter].y;
                    if(drawPoint[y_][x_]<255){
                        //drawPoint[0][y_][x_]=0;
                        //cout<<" Get it!";
                        pointsCorrelation++;
                        
                        byteMask m; //здесь нужно проверить тип точки
                        m.setMask(drawPoint[y_][x_]);
                        //cout<<"_j="<<pointIndex<<" x="<<x_<<" y="<<y_<<" t="<<(int)glyph[0].fPoint[pIndexLetter].type<<
                        //" b1="<<m.b1<<" b2="<<m.b2<<" b3="<<m.b3<<" b4="<<m.b4<<" b5="<<m.b5<<" b6="<<m.b6<<" b7="<<m.b7<<" b8="<<m.b8<<" p="<<(int)drawPoint[0][y_][x_]<<endl;
                        if(glyph[0].fPoint[pIndexLetter].type==T_POINT&&m.b1)pointsCorrelation+=1;
                        if(glyph[0].fPoint[pIndexLetter].type==X_POINT&&m.b2)pointsCorrelation+=1;
                        //if(glyph[0].fPoint[pIndexLetter].type==L_POINT&&mask.b3)pointsCorrelation+=1;
                        if(glyph[0].fPoint[pIndexLetter].type==P_POINT&&m.b4)pointsCorrelation+=1;
                    }
                     */
            //    }
            //}else{pointsCorrelation=1;}
    }
}
    
}; //____________________________________________________________________________




