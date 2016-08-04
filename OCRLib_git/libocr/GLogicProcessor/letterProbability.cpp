#include "GLogicProcessor.h"
//расчет вероятности буквы на основании взаимной корреляции всех букв, найденных на месте тестовой буквы
void GLogicProcessor::letterProbability(vector<OCRMatch>&matchLine){

    //Находим в строке букву с наибольшим габаритом. Рассматриваем эту букву как тестовую.
    //Для всех букв внитри габарита высчитывем разницу между фактической корреляцией
    //и корреляцией этой буквы и буквы с тестовой буквой. Эта корреляция определена заранее в результате тестирования базы.
    //Если разница минимальна, это значит, что гипотеза тестовой буквы поддтверждается.
    //Для подсчета используем крадратичную сумму, извлекая корень из суммы крадратов разности корреляций.
    while(1){
        //Находим в строке букву с наибольшим габаритом.
        int maxW=0, maxH=0,  maxInd=-1;
        for(int i=0;i<matchLine.size();i++){
            if(!matchLine[i].correlation)continue;
            if(matchLine[i].status||matchLine[i].OCRIndex==3)continue;
            if(matchLine[i].letterH>maxH&&matchLine[i].letterW>maxW){
                maxW=matchLine[i].letterW;maxH=matchLine[i].letterH;maxInd=i;}
        }
        if(maxInd==-1)break; //разбор строки закончен
        
        //maxInd=451;//1574;//;
        //print=0;  if(maxInd==469)print=1;
        
        OCRMatch testLetter=matchLine[maxInd];
        
        DR("@@_______"<<matchLine[maxInd].name
           <<" c="<<matchLine[maxInd].correlation<<" x="<<matchLine[maxInd].xCenter<<" in=@"<<matchLine[maxInd].letterIndex);
        int x0=testLetter.xCenter-testLetter.letterW/2;
        int y0=testLetter.yCenter-testLetter.letterH/2;
        int x1=x0+testLetter.letterW;
        int y1=y0+testLetter.letterH;
        int S1=(x1-x0)*(y1-y0);  //площадь базовой буквы
        DR(" x0="<<x0<<" x1="<<x1<<" y0="<<y0<<" y1="<<y1<<endl)
        
        
        //Для каждой распознанной буквы подсчитываем среднюю коррелцию с учетом взаимной корреляции букв.
        //Для каждой пары букв распознаных на одном месте текста высчитывем разницу между фактической корреляцией
        //и корреляцией этой буквы и буквы с тестовой буквой подсчитанной заранее для всех букв базы.
        //Для определения пары букв используем как параметр площадь, на которые буквы
        //перекрывают друг друга. Таким побразом считаем парой две буквы распознанные на одном
        //месте текста. Значение разницы корреляции приводим к ста процентам и учитываем пропорционально площади пересечения габаритов букв.
        //Таким образом, если в А найдено Л то это поддтверждает гипотезу А
        
        vector<OCRMatch>cLine;
        int index=maxInd-1;
        int dX=testLetter.xCenter-32;
        //DR("dX="<<dX<<" StrH="<<StrH)
        while(index>=0){
            if(matchLine[index].OCRIndex==3||!matchLine[index].correlation){index--;continue;}
            if(matchLine[index].xCenter<dX)break;
            cLine.push_back(matchLine[index]); //DR("         x="<<testLetter.xCenter)
            //matchLine[index].status=1; //помечаем как разобранную
            index--;
        }
        index=maxInd+1;
        dX=testLetter.xCenter+32;
        //DR("dX="<<dX<<" s="<<line.size())
        while(index<matchLine.size()){
            if(matchLine[index].OCRIndex==3||!matchLine[index].correlation){index++;continue;}
            if(matchLine[index].xCenter>dX)break;
            cLine.push_back(matchLine[index]); //DR("         x="<<testLetter.xCenter)
            //matchLine[index].status=1; //помечаем как разобранную
            index++;
        }
        index=maxInd-1;
        dX=testLetter.xCenter-32;
        while(index>=0){
            if(matchLine[index].OCRIndex==3||!matchLine[index].correlation){index--;continue;}
            if(matchLine[index].xCenter<dX)break;
            cLine.push_back(matchLine[index]); //DR("         x="<<testLetter.xCenter)
            //matchLine[index].status=1; //помечаем как разобранную
            index--;
        }
        GLetter *letter=aliKali->getLetter(testLetter.letterIndex);
        int count=0; int cSum=0;
        sort(cLine.begin(),cLine.end(),sortMatchC);
        for(int i=0;i<cLine.size();i++){
            DR("   "<<cLine[i].name<<" c="<<cLine[i].correlation<<" x="<<cLine[i].xCenter<<" in="<<cLine[i].letterIndex)
            
            //определяем площадь пересечения.
            int x0_=cLine[i].xCenter-cLine[i].letterW/2;
            int y0_=cLine[i].yCenter-cLine[i].letterH/2;
            int x1_=x0_+cLine[i].letterW;
            int y1_=y0_+cLine[i].letterH;
            
            if(x0_>x1||x0>x1_||y0_>y1||y0>y1_){
                DR(" -s  x0_="<<x0_<<" x1_="<<x1_<<" y0_="<<y0_<<" y1_="<<y1_<<endl)
                continue; //буквы не пересекаются
            }
            
            int x0__; if(x0>x0_)x0__=x0; if(x0<=x0_)x0__=x0_;
            int x1__; if(x1<x1_)x1__=x1; if(x1>=x1_)x1__=x1_;
            
            int y0__; if(y0>y0_)y0__=y0; if(y0<=y0_)y0__=y0_;
            int y1__; if(y1<y1_)y1__=y1; if(y1>=y1_)y1__=y1_;
            
            //DR("x0__="<<x0__<<" x1__="<<x1__<<" y0__="<<y0__<<" y1__="<<y1__)
            int S0=(x1__-x0__)*(y1__-y0__);  //площадь пересечения габаритов букв
            int S2=(x1_-x0_)*(y1_-y0_);  //площадь второй проверяемой буквы
            
            if(S0<512){
                DR(" -r x0_="<<x0_<<" x1_="<<x1_<<" y0_="<<y0_<<" y1_="<<y1_)
                continue; //мала площадь пересечения
            }
            //S0_=(S0_*100)/S0;
            //DR("S0_="<<(S0_*100)/S0)
            
            int c1=aliKali->cMatrix[0][FSIZE*testLetter.letterIndex+cLine[i].letterIndex];
            DR(" c1="<<c1<<"  @@@   ")
            if(!c1){
                DR(" -l "<<endl)
                continue;
            }
            if(c1<40){
                DR(" -m "<<endl)
                continue;
            }
            
            int c0=cLine[i].correlation;
            int dC=abs(c0-c1);
            int dC1;
            
            if(S1/S2>=1){dC1=dC*S1/S2;} // S1/S2<1 было
            else{dC1=dC*S2/S1;}
            
            dC=(100-dC1);
            
            DR(" c1="<<c1<<" dC="<<dC<<endl)
            cSum+=dC;
            
            
            //line[i].correlation-=2;  //помечаем как разобранную
            count++;
        }
        testLetter.status=1; //помечаем как разобранную
        DR("n="<<testLetter.name<<" count="<<count<<endl)
        float kC=(float)0.25;
        //if(StrH*StrH*2>S1)kC=0;
        float oldC=testLetter.correlation; //*(1-kC)
        if(count){
            testLetter.correlationNew=testLetter.correlation-(testLetter.correlation-cSum/count)*kC;
            //c=c*(1-kC)+kC*(DL)
            
        }else{
            testLetter.correlationNew=testLetter.correlation;
        }
        if(count)DR("@@"<<maxInd<<"@@ new correlation="<<testLetter.correlationNew<<" DC="<<(oldC-cSum/count)<<" cSum/count="<<cSum/count<<endl<<endl<<endl)
        letter->destroy();
        matchLine[maxInd]=testLetter;
        //exit(0);
    }


}