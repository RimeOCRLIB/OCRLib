
#include "GLogicProcessor.h"
//формирует массив букв OpenType с координатами соответсвующими позиции элемента стековой буквы
//относительно родительской буквы. Разбираем радительскую букву на OpenType
//функция применяется толшько в том случаее если распознавание ведется с участием эталонных стековых букв

void GLogicProcessor::buildOpenType(vector<OCRMatch>&line,vector<OCRMatch>&letterLine){

    //формируем массив букв OpenType с координатами соответсвующими позиции элемента стековой буквы
    //относительно родительской буквы
    int print=0;
        
    for(int i=0;i<line.size();i++){
        //cout<<"i="<<i<<" c="<<line[346].correlation<<endl;
        if(!line[i].correlation)continue;
        string name=line[i].name;
        //print=0; if(i==112)print=1; if(!print)continue;
        
        DR("line["<<i<<"].name="<<line[i].name<<"/ OCRIndex="<<line[i].OCRIndex<<endl)
        
        wstring wName=UTF_to_Unicode(line[i].name);
        if(line[i].OCRIndex=='Z'||line[i].OCRIndex=='S'){
            line[i].uni=wName[0];
            line[i].status=0;
            line[i].Character=0;
            letterLine.push_back(line[i]);
            continue;
        }
        
        ulong index=fontGMap->getHKey(name,REC_LETTER_UNI_UTF);
        if(index==0xffffffffffffffff){
            cout<<" not found letter /"<<name<<"/ letterIndex:"<<line[i].letterIndex<<endl;
            continue;
        }
        
        TString strT; fontTable->getTStr(index,strT);
        DR("name="<<strT[8]<<" OCRIndex = "<<strT[4]<<endl)
        if(strT.size()<5||(strT[1]!="tib"&&strT[1]!="eng"&&strT[1]!="skt")){continue; }
        string OCRIndex=strT[4];
        if(line[i].OCRIndex=='S')OCRIndex="S";
        if(wName.size()==1||OCRIndex=="Z"){
            //переводим все буквы в верхний регистр
            wName[0]=UniBigLetters[wName[0]];
            line[i].name=Unicode_to_UTF(wName);  DR("n="<<line[i].name<<" i="<<letterLine.size()<<endl)
            line[i].uni=wName[0];
            line[i].status=0;
            line[i].Character=0;
            line[i].OCRIndex=OCRIndex[0];
            letterLine.push_back(line[i]);
        }else{
        
            for(int j=0;j<wName.size();j++){
                OCRMatch match;
                wstring w;  w=wName[j];
                //переводим все буквы в верхний регистр
                w[0]=UniBigLetters[w[0]];
                DR("    OCRLetter  =  "<<Unicode_to_UTF(w)<<" / "<<OCRIndex[j])
                DR(" x0="<<line[i].x0<<" x1="<<line[i].x1<<" y0="<<line[i].y0<<" y1="<<line[i].y1<<endl)
                match.name=Unicode_to_UTF(w);
                match.correlation=line[i].correlation;
                match.letterIndex=line[i].letterIndex;
                match.OCRIndex=OCRIndex[j];
                match.uni=w[0];
                match.status=1;                     //помечаем букву как строительный элемент OpenType
                match.Character=100;                //помечаем букву как строительный элемент OpenType
                if(wName.size()==1){match.status=0; match.Character=0;} //помечаем букву как одиночную
                
                if(OCRIndex[j]=='A'&&OCRIndex.size()==1){
                    match.x0=line[i].xCenter-line[i].letterW/2;
                    match.x1=line[i].xCenter+line[i].letterW/2;
                    match.y0=line[i].yCenter-line[i].letterH/2;
                    match.y1=line[i].yCenter+line[i].letterH/2;
                    match.letterH=line[i].letterH;
                    match.letterW=line[i].letterW;
                }
                if(OCRIndex=="S"){   //режим распознавания в котором стеки не разбираем на OpenType
                    match.x0=line[i].x0;
                    match.x1=line[i].x1;
                    match.y0=line[i].y0;
                    match.y1=line[i].y1;
                    match.letterH=line[i].letterH;
                    match.letterW=line[i].letterW;
                    match.name=line[i].name;
                    match.wName=wName;
                    match.Character=0;
                    match.OCRIndex='S';
                    match.status=0;
                    for(int j=0;j<wName.size();j++){
                        wstring w;  w=wName[j];
                        //переводим все буквы в верхний регистр
                        w[0]=UniBigLetters[w[0]];
                        match.uni+=w[0];
                    }
                    match.setSize(0);
                    letterLine.push_back(match);
                    break;
                }
                if(OCRIndex[j]=='A'&&OCRIndex.size()>1){
                    match.x0=line[i].xCenter-line[i].letterW/2;
                    match.x1=line[i].xCenter+line[i].letterW/2;
                    if(OCRIndex[OCRIndex.size()-1]=='V'){
                        match.y0=line[i].yCenter-line[i].letterH/2+line[i].letterW*0.75;
                        match.y1=match.y0+line[i].letterH/(int)OCRIndex.size();
                    }else{
                        if((OCRIndex[1]=='W'||OCRIndex[1]=='R')&&OCRIndex.size()==2){
                            match.y0=line[i].yCenter-line[i].letterH/2;
                            match.y1=match.y0+line[i].letterH-20;
                        }else{
                            match.y0=line[i].yCenter-line[i].letterH/2;
                            match.y1=match.y0+line[i].letterH/(int)OCRIndex.size();                
                        }    
                    }
                    match.letterH=line[i].letterW;
                    match.letterW=line[i].letterW;
                }
                if(OCRIndex[j]=='B'||
                   OCRIndex[j]=='C'||
                   OCRIndex[j]=='D'||
                   OCRIndex[j]=='E'||
                   OCRIndex[j]=='F'||
                   OCRIndex[j]=='G'||
                   OCRIndex[j]=='H'){
                    
                    match.x0=line[i].xCenter-line[i].letterW/2;
                    match.x1=line[i].xCenter+line[i].letterW/2;
                    if(OCRIndex.size()>1){
                        match.y0=letterLine[letterLine.size()-1].y0+line[i].letterH/(int)OCRIndex.size();
                    }else{
                        match.y0=line[i].y0;
                    }
                    match.y1=match.y0+line[i].letterH/(int)OCRIndex.size();
                    
                    match.letterH=match.y1-match.y0;
                    match.letterW=line[i].letterW;
                }
                if(OCRIndex[j]=='R'){
                    
                    match.x0=line[i].xCenter-line[i].letterW/2;
                    match.x1=line[i].xCenter+line[i].letterW/2;
                    if(OCRIndex.size()>1){
                        if(OCRIndex[OCRIndex.size()-1]=='R'){
                            match.y0=line[i].y1-20;
                            match.y1=match.y0+20;
                        }else{
                            match.y0=letterLine[letterLine.size()-1].y1;
                            match.y1=match.y0+20;
                        }    
                    }else{
                        match.y0=line[i].y0;
                        match.y1=line[i].y1;
                        
                    }
                    match.letterH=match.y1-match.y0;
                    match.letterW=line[i].letterW;
                }
                if(OCRIndex[j]=='W'){
                    
                    match.x0=line[i].xCenter-line[i].letterW/2;
                    match.x1=line[i].xCenter+line[i].letterW/2;
                    if(OCRIndex.size()>1){
                        if(OCRIndex[OCRIndex.size()-1]=='W'){
                            match.y0=line[i].y1-20;
                            match.y1=match.y0+20;
                        }else{
                            match.y0=letterLine[letterLine.size()-1].y1;
                            match.y1=match.y0+20;
                        }    
                    }else{
                        match.y0=line[i].y0;
                        match.y1=line[i].y1;
                        
                    }
                    match.letterH=match.y1-match.y0;
                    match.letterW=line[i].letterW;
                }
                
                
                if(OCRIndex[j]=='V'||OCRIndex[j]=='Z'||OCRIndex[j]=='X'){
                    match.xCenter=line[i].xCenter;
                    match.yCenter=line[i].yCenter-line[i].letterH/2;
                    match.letterH=line[i].letterH;
                    match.letterW=line[i].letterW;
                    match.name=line[i].name;
                }
                DR(" x0="<<match.x0<<" x1="<<match.x1<<" y0="<<match.y0<<" y1="<<match.y1<<endl)
                DR(" letterLine.size()="<<letterLine.size()<<" @@@"<<match.name<<endl)
                match.setSize(0);
                letterLine.push_back(match);
                //дополняем массив похожими по начертанию буквами
                /* if(match.name=="ཞ"){
                 OCRMatch match_=match;
                 match_.name="ན";
                 match_.wName=UTF_to_Unicode("ན");
                 match_.uni=match_.wName[0];
                 match_.correlation-=5;
                 letterLine.push_back(match_);
                 }
                 */
            }
        }
    }
    
}
