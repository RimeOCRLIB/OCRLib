#include "GLogicProcessor.h"
#include "GImageEditor.h"
//функция расстановки знаков препинания в собранной фразе полученной в результате работы функции textWorldLine
//для тибетского языка также расставляются мезслоговые разделителе - знак tsheg

void GLogicProcessor::sentenceConstructurNew(vector<OCRMatch>&matchLine,
                                             vector<OCRMatch>&originalMatch,
                                             GBitmap* lineImg32,
                                             GBitmap* letterAImg,
                                             GBitmap* letterBImg){

    //drawGrapeLine(matchLine); exit(0);
    
    int print=0;
    
    DR("//****Start sentence constructor ******* matchLine.size()="<<matchLine.size()<<endl)
    print=0;
    OCRBox s,sBox;
    
    for(int i=0;i<matchLine.size();i++){
        
        if(!matchLine[i].correlation||matchLine[i].OCRIndex=='Z'||matchLine[i].OCRIndex=='Z')continue;
        for(int n=0;n<matchLine[i].line.size();n++){
            
            OCRMatch &letter=originalMatch[matchLine[i].line[n]];
            OCRMatch *nextLetter=0;
            int nextLetterFlag=0;
            if(n<matchLine[i].line.size()-1){
                nextLetter=&originalMatch[matchLine[i].line[n+1]];
                nextLetterFlag=1;
            }else{
                int minDistance=10000;
                for(int m=0;m<matchLine.size();m++){
                    if(!matchLine[m].correlation||matchLine[m].OCRIndex=='Z'||m==i)continue;
                    if(matchLine[m].x0>letter.x1-7){
                        if(matchLine[m].x0-letter.x1<minDistance){
                            minDistance=matchLine[m].x0-letter.x1;
                            nextLetter=&originalMatch[matchLine[m].line[0]];
                            nextLetterFlag=1;
                        }
                    }
                }
            }
            //теперь мы знаем габариты проверочной буквы и буквы следующей за ней. Знаки препинания должны находится следом за тестовой буквой и не перекрываться
            //следующей буквой. Проверяем все знаки препинания на соответствие.
            int flagDrawA=0;
            int flagDrawNext=0;
            for(int m=0;m<matchLine.size();m++){
                if(!matchLine[m].correlation||matchLine[m].OCRIndex!='Z')continue;
                if(matchLine[m].xCenter>=letter.xCenter){
                    if(nextLetterFlag&&matchLine[m].xCenter>nextLetter->xCenter)continue;
                    if(matchLine[m].name=="་"&&abs(matchLine[m].s.yLimit0-letter.s.yLimit0)>11)continue;
                }else{continue;}

                if(m==-1){
                   cout<<1;
                }
                //проверяем есть ли пересечение букв.
                intArea(letter.s,matchLine[m].s,s);
                if(s.area){
                    //DR("intersection with letter j="<<j<<endl)
                    if(!flagDrawA){
                        letterAImg->fillColumns32V(0, &letter.s);  //стираем букву A
                        //рисуем букву А в letterAImg
                        letter.drawPict32(letterAImg,0,0,XOR_MODE);
                        flagDrawA=1;
                    }
                    //рисуем букву B в letterBImg
                    matchLine[m].drawPict32(letterBImg,0,0,XOR_MODE);
                    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1LetterB32.jpg",0);
                    //подсчитываем количество точек буквы А закрытых буквой B
                    int pArea=letterAImg->img32Union(letterBImg,&s);
                    letterBImg->fillColumns32V(0, &matchLine[m].s); //стираем букву B
                    
                    DR("@@@"<<i<<" "<<letter.name<<" c="<<letter.correlation<<" x0="<<letter.x0<<" x1="<<letter.x1
                       <<" pCount="<<letter.pCount<<endl)
                    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
                    //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1LetterB32.jpg",0);
                    //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1LetterLine32_1.jpg",0);
                    if(pArea>matchLine[m].pCount){
                        DR("remove delimeter "<<m<<endl);
                        matchLine[m].correlation=0; continue;
                    }
                    if(matchLine[m].delimeter=="་"&&matchLine[m].pCount==0){
                        DR("remove delimeter1 "<<m<<endl);
                        matchLine[m].correlation=0; continue;
                    }
                }
                
                if(nextLetterFlag){
                    //проверяем есть ли пересечение букв.
                    intArea(nextLetter->s,matchLine[m].s,s);
                    if(s.area){
                        //DR("intersection with letter j="<<j<<endl)
                        if(!flagDrawNext){
                            letterAImg->fillColumns32V(0, &nextLetter->s);  //стираем букву nextLetter
                            //рисуем букву nextLetter в letterAImg
                            nextLetter->drawPict32(letterAImg,0,0,XOR_MODE);
                            flagDrawNext=1;
                        }
                        //рисуем букву B в letterBImg
                        matchLine[m].drawPict32(letterBImg,0,0,XOR_MODE);
                        //подсчитываем количество точек буквы А закрытых буквой B
                        int pArea=letterAImg->img32Union(letterBImg,&s);
                        letterBImg->fillColumns32V(0, &matchLine[m].s); //стираем букву B
                        
                        DR("@@@"<<i<<" "<<nextLetter->name<<" c="<<nextLetter->correlation<<" x0="<<nextLetter->x0<<" x1="<<nextLetter->x1
                           <<" pCount="<<nextLetter->pCount<<endl)
                        //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterAImg,"/_Image2OCR/_1LetterA32.jpg",0);
                        //((GImageEditor*)inputData.imageEditor)->WriteImageData(letterBImg,"/_Image2OCR/_1LetterB32.jpg",0);
                        //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1LetterLine32_1.jpg",0);
                        if(pArea>matchLine[m].pCount){
                            DR("remove delimenter "<<m<<endl);
                            matchLine[m].correlation=0; continue;
                        }
                    }
                    
                }

                DR("valid delimenter "<<m<<endl);
                //проверяем длинну участка уверенного ответа словаря
                int count=0;
                int ind=n;
                while(ind<matchLine[i].dIndex.size()&&matchLine[i].dIndex[ind]!=2){
                    count++;
                    ind++;
                }
                ind=n-1;
                while(ind>=0&&matchLine[i].dIndex[ind]!=2){
                    count++;
                    ind--;
                }
                //устанавливаем делиметер в случае уверенности словаря в его наличии или малой длинне ответа словаря
                if(matchLine[i].dIndex[n]==1||matchLine[i].dIndex[n]==2||count<5){
                    letter.delimeter+=matchLine[m].name+matchLine[m].delimeter;
                    matchLine[i].dIndex[n]=1;
                    matchLine[m].correlation=0;
                    if(matchLine[i].x1<matchLine[m].x1)matchLine[i].x1=matchLine[m].x1;
                    matchLine[i].setSize(1);
                }
            }
        }
    }
    //drawGrapeLine(matchLine); exit(0);

    
}


