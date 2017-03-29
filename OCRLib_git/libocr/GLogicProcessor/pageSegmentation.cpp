#include "GLogicProcessor.h"
#include "GImageEditor.h"
#include "debug.h"

void GLogicProcessor::pageSegmentation(vector<OCRMatch>&matchLine,vector<OCRFocalLine>focalLine,int pageW,int dY){

    sort(matchLine.begin(),matchLine.end(),sortMatchX0);
    
    //drawGrapeLine(matchLine);exit(0);
    
    //cout<<"strArray[0].size()="<<strArray[0].size()<<" matchLine.size()="<<matchLine.size()<<endl;
    
    for(int i=0;i<matchLine.size();i++){
        matchLine[i].correlationNew=0;
        if(!matchLine[i].correlation)continue;
        matchLine[i].status=0;
    }

    if(!matchLine.size()||!focalLine.size())return;   //
    
    //распознавание вертикальных рамок
    int i=0;
    while(i<matchLine.size()){
    
        if(matchLine[i].name=="|"||matchLine[i].name=="།"){
            //проверяем есть ли вертикальный вектор достаточной длинны на месте этой буквы
            for(int j=0;j<focalLine.size();j++){
                OCRFocalLine &line=focalLine[j];
                if(line.len<256)break;
                if(abs(line.start.x-line.end.x)<32){
                    if(abs(line.start.x-matchLine[i].x0)<16){
                        if(line.start.x<pageW/2){
                            matchLine.erase(matchLine.begin(),matchLine.begin()+i+1);
                            i=-1;
                            break;
                        }else{
                            matchLine.erase(matchLine.begin()+i,matchLine.end());
                            break;
                        }
                    }
                }
            }
        }
        i++;
    }
    
    //распознавание горизонтальных рамок
    for(int j=0;j<focalLine.size();j++){
        OCRFocalLine &line=focalLine[j];
        if(line.len<128)break;
        if(abs(line.start.y-line.end.y)<32){
            for(int i=0;i<matchLine.size();i++){
                if(abs(line.start.y-dY-matchLine[i].yCenter)<16&&matchLine[i].xCenter>line.start.x-5&&matchLine[i].xCenter<line.end.x+5){
                    matchLine[i].correlation=0;
                }
            }
        }
    }

    //drawGrapeLine(matchLine);exit(0);
    
    


}



