#include "GLogicProcessor.h"


void GLogicProcessor::letterConstruction(vector<OCRMatch>&letterLine){

    //проверка результатов распознавания на вероятность
    //убираем все огласовки которые находятся внутри коренных букв
    //если буква распознана с высокой корреляцией убираем другие гипотезы

    int s_;
    int dX_=10;
    int dX=32;
    int print=0;
    string xKey; xKey=(uchar)224; xKey+=(uchar)188; xKey+=(uchar)185;
    
    //drawGrapeLine(letterLine); exit(0);
    
    for(int i=0;i<letterLine.size();i++){  //cout<<"i="<<i<<" c="<<letterLine[277].correlation<<endl;
        if(!letterLine[i].correlation)continue;
        //cout<<i<<" name="<<letterLine[i].name<<" OCRIndex"<<(char)letterLine[i].OCRIndex<<endl;
        //cout<<"i="<<i<<" c="<<letterLine[i].correlation<<" y0Base="<<y0Base<<" y1Base="<<y1Base<<" y0="<<letterLine[i].y0<<endl;
        if(letterLine[i].OCRIndex=='S')continue;
        if(letterLine[i].name=="་"&&(letterLine[i].y0<y0Base-10||letterLine[i].y0>y0Base+10)){
            //letterLine[i].name=".";
            //letterLine[i].correlation=0;
            continue;
        }
        if((letterLine[i].name=="."||letterLine[i].name==",")&&(letterLine[i].y0<y1Base-10||letterLine[i].y1>y1Base+10)){letterLine[i].correlation=0;continue;}
        if(letterLine[i].name=="ར"&&letterLine[i].y0<y0Base-10){letterLine[i].correlation=0;continue;}
        if(letterLine[i].OCRIndex=='A'&&letterLine[i].y0<y0Base-20&&letterLine[i].name.find(xKey)==-1){letterLine[i].correlation=0;continue;}
        if(letterLine[i].OCRIndex=='Z'&&(letterLine[i].yCenter>y1Base+32||letterLine[i].yCenter<y0Base-32)){letterLine[i].correlation=0;continue;}
        if(letterLine[i].OCRIndex=='V'&&letterLine[i].yCenter>y0Base+dX_){letterLine[i].correlation=0;continue;}
        //if(letterLine[i].OCRIndex=='V'&&letterLine[i].y1<y0Base-letterLine[i].letterH*1.5){letterLine[i].OCRIndex='T';}
        if((letterLine[i].OCRIndex=='R'||letterLine[i].OCRIndex=='W')&&letterLine[i].y1<y1Base){letterLine[i].correlation=0;continue;}
        if(letterLine[i].OCRIndex!='V'&&letterLine[i].OCRIndex!='Z'&&letterLine[i].OCRIndex!='X'&&letterLine[i].OCRIndex!='N'&&letterLine[i].y0<y0Base-20){letterLine[i].correlation=0;continue;}
    }
    //drawGrapeLine(letterLine); exit(0);
    /*
    //если буква распознана с высокой корреляцией убираем другие гипотезы
    for(int i=0;i<letterLine.size();i++){
        if(letterLine[i].correlation>98&&
           letterLine[i].OCRIndex!='V'&&
           letterLine[i].OCRIndex!='Z'&&
           letterLine[i].OCRIndex!='X'&&
           letterLine[i].OCRIndex!='W'&&
           letterLine[i].OCRIndex!='R'&&
           letterLine[i].OCRIndex!='N'&&
           letterLine[i].name!="ར"&&
           letterLine[i].name!="ང"&&
           letterLine[i].name!="འ"){
            for(int j=0;j<letterLine.size();j++){
                if(letterLine[i].correlation>letterLine[j].correlation&&
                   letterLine[i].xCenter>letterLine[j].x0&&
                   letterLine[i].xCenter<letterLine[j].x1&&
                   letterLine[i].yCenter>letterLine[j].y0&&
                   letterLine[i].yCenter<letterLine[j].y1
                   ){letterLine[j].correlation=0;continue;}
            }
        }
    }
    */
    //drawGrapeLine(letterLine); exit(0);
    

    for(int i=0;i<letterLine.size();i++){  //cout<<"i="<<i<<" c="<<letterLine[123].correlation<<endl;
        if(!letterLine[i].correlation)continue;
        
        /*
        if(letterLine[i].OCRIndex=='V'||
           letterLine[i].OCRIndex=='X'||
           letterLine[i].OCRIndex=='W'||
           letterLine[i].OCRIndex=='R'||
           letterLine[i].OCRIndex=='Z'||
           letterLine[i].OCRIndex=='N'||
           letterLine[i].name=="ར"||
           letterLine[i].name=="བ"||
           letterLine[i].name=="པ"||
           letterLine[i].name=="ང"//||
           //letterLine[i].name=="འ"
           ){
        */
            //print=1;
            //
            //print=0; if(i==6)print=1; if(!print)continue;print
            //cout<<"i="<<i<<" c="<<letterLine[9].correlation<<" n="<<letterLine[9].name<<" "<<(char)letterLine[9].OCRIndex<<endl;
           
            //int raTagFlag=0;
            int spaceLeft=0;
            int spaceRight=0;

            //в пределах лимита проверяем есть ли коренная буква, которая находится на месте этой огласовки
            //int s=(letterLine[i].x1-letterLine[i].x0)*(letterLine[i].y1-letterLine[i].y0);
            /*print=0;
             if(i==268){
             DR(endl<<"@@@ letterLine[ind]="<<letterLine[i].name<<" c="<<letterLine[i].correlation<<endl; //exit(0)
             print=1;
             }
             */
            DR("@@@collect "<<i<<" name="<<letterLine[i].name<<"  c="<<letterLine[i].correlation
               <<" x0="<<letterLine[i].x0<<" x1="<<letterLine[i].x1<<" yC="<<letterLine[i].yCenter<<" xC="<<letterLine[i].xCenter
               <<" y0="<<letterLine[i].y0<<" y1="<<letterLine[i].y1<<endl);
            if(*aliKali->OCRLanguage==OCR_TIBETAN_SKT||letterLine[i].OCRIndex=='Z'||letterLine[i].OCRIndex=='N'||letterLine[i].name=="ག"){
                //подпрограмма поиска конца предложения
                int d=i+1;
                int xC=letterLine[i].xCenter;
                int dX=25;
                
                //if(letterLine[i].name!="་"&&letterLine[i].name!="།"){
                //  letterLine[i].delimeter=letterLine[i].name;
                //  letterLine[i].name="";
                //}
                
                if(!spaceRight){
                    while(1){
                        if(d>=letterLine.size()){spaceRight=1;break;}
                        if(!letterLine[d].correlation||letterLine[d].OCRIndex=='X'){d++;continue;}
                        if(letterLine[d].xCenter<letterLine[i].x1||letterLine[d].yCenter>y1Base+dX){d++;continue;}
                        if(letterLine[d].x0>letterLine[i].x1+dX)spaceRight=1;
                        break;
                    }
                }
                if(spaceRight){  DR("space right "<<endl)
                    letterLine[i].delimeter="•";
                }
                d=i-1;
                
                if(!spaceRight){
                    while(1){ 
                        if(d<0){spaceRight=1;break;}
                        if(!letterLine[d].correlation){d--;continue;}
                        if(abs(letterLine[d].xCenter-xC)<dX||letterLine[d].yCenter>y1Base+dX){d--;continue;}
                        if(letterLine[d].name!="ང"&&letterLine[d].name!="ད"&&letterLine[d].x1<letterLine[i].x0-dX)spaceLeft=1;
                        if((letterLine[d].name=="ང"||letterLine[d].name=="ང")&&letterLine[d].x1<letterLine[i].x0-dX*1.5)spaceLeft=1;
                         DR(d<<" n="<<letterLine[d].name<<" letterLine[d].x1="<<letterLine[d].x1<<endl)
                        break;
                    }
                    DR("n="<<letterLine[d].name<<"letterLine[i].x0="<<letterLine[i].x0<<" xC="<<xC<<endl)
                }

                if(spaceLeft){
                    DR("space left "<<endl;)
                }
            }
        /*
        if(letterLine[i].name=="ང"&&((float)letterLine[i].letterH)/((float)letterLine[i].letterW)>1.1){
            OCRMatch match=letterLine[i];
            match.name="ད";
            match.wName=UTF_to_Unicode(match.name);
            match.uni=match.wName[0];
            letterLine.insert(letterLine.begin()+i,match); i++;
        }
        if(letterLine[i].name=="ད"&&((float)letterLine[i].letterH)/((float)letterLine[i].letterW)<1){
            OCRMatch match=letterLine[i];
            match.name="ང";
            match.wName=UTF_to_Unicode(match.name);
            match.uni=match.wName[0];
            letterLine.insert(letterLine.begin()+i,match); i++;
        }
        */

    }
    
    //DR("@@@@@@4@";
}//____________________________________________________________



/*
 for(int j=0;j<letterLine.size();j++){  //if(i==33){cout<<letterLine[i].correlation<<endl;}
 
 if(i==j)continue;
 if(!letterLine[i].correlation)break;
 if(!letterLine[j].correlation)continue;
 if(letterLine[j].OCRIndex=='S')continue;
 
 if(!(letterLine[i].x1>letterLine[j].x0-5&&letterLine[i].x0<letterLine[j].x1+5))continue;
 
 if(letterLine[i].name==letterLine[j].name)continue;
 if(letterLine[i].name=="ེ"&&letterLine[j].name=="ྲ")continue;
 if(letterLine[i].name=="འ"&&letterLine[j].name=="༢")continue;
 if(letterLine[i].name=="ྲ"&&letterLine[j].name=="ེ")continue;
 if(letterLine[i].name=="ྱ"&&letterLine[j].name=="ྲ")continue;
 if(letterLine[i].name=="ྲ"&&letterLine[j].name=="ྭ")continue;
 if(letterLine[i].name=="བ"&&letterLine[j].name=="ང")continue;
 if(letterLine[i].name=="བ"&&letterLine[j].name=="པ")continue;
 //if(letterLine[i].name=="ར"&&letterLine[j].name=="ཇ")continue;
 if(letterLine[i].OCRIndex=='X'&&letterLine[j].OCRIndex=='X')continue;
 if(letterLine[i].OCRIndex=='R'&&letterLine[j].OCRIndex=='W')continue;
 
 DR(" letterLine["<<j<<"].name="<<letterLine[j].name<<" x0="<<letterLine[j].x0<<" x1="<<letterLine[j].x1
 <<" y0="<<letterLine[j].y0<<" y1="<<letterLine[j].y1
 <<" c="<<letterLine[j].correlation<<endl);
 
 
 
 int dC=letterLine[j].correlation-letterLine[i].correlation;
 
 if(letterLine[i].name=="་"&&letterLine[j].OCRIndex=='Z'&&dC>0){
 letterLine[i].correlation=0;
 DR("remove punctuation1"<<endl)
 continue;
 }
 if((letterLine[i].name=="ཾ"||letterLine[i].name=="ྂ"||letterLine[i].name=="ྃ"||letterLine[i].name=="༠")&&letterLine[j].name=="༔"&&dC>-10){
 letterLine[i].correlation=0;
 DR("remove punctuation2"<<endl)
 continue;
 }
 
 
 DR("+ letterLine["<<i<<"].name="<<letterLine[i].name<<" /"<<letterLine[j].OCRIndex<<" letterLine["<<j<<"].name="<<letterLine[j].name
 <<" DCY="<<abs(letterLine[j].yCenter-letterLine[i].yCenter)
 <<" y0="<<letterLine[j].y0<<" y1="<<letterLine[j].y1<<" yC="<<letterLine[j].yCenter<<" c="<<letterLine[j].correlation<<" dC="<<dC<<endl);
 
 if((letterLine[j].OCRIndex!='V'
 &&letterLine[j].OCRIndex!='Z'
 &&letterLine[j].OCRIndex!='X')||
 ((letterLine[i].name=="ེ"||letterLine[i].name=="ྲ"||letterLine[i].name=="ྂ"||letterLine[i].name=="ྃ"||letterLine[i].name=="ར"||letterLine[i].name=="ི")&&(letterLine[j].name=="ོ"||letterLine[j].name=="ི"||letterLine[j].name=="ི"||letterLine[j].name=="]"||letterLine[j].OCRIndex=='Z'))
 ||letterLine[i].name=="།"
 ||letterLine[i].name=="ཾ"
 ||letterLine[i].name=="་"
 ||letterLine[i].OCRIndex=='N'
 ){
 
 if(!raTagFlag&&letterLine[j].OCRIndex!='W'&&
 letterLine[j].OCRIndex!='V'
 &&letterLine[j].y1<letterLine[i].y0+10
 &&letterLine[j].y1>letterLine[i].y0-10
 &&abs(letterLine[j].yCenter-letterLine[i].yCenter)>15
 ){
 raTagFlag=1;
 DR("ra-tag=1"<<endl)
 }
 
 if(!raTagFlag&&letterLine[j].OCRIndex!='W'&&
 letterLine[j].OCRIndex!='V'
 &&hasTail(letterLine[j])
 &&letterLine[j].y1<letterLine[i].y0+20
 &&letterLine[j].y1>letterLine[i].y0-10
 &&abs(letterLine[j].yCenter-letterLine[i].yCenter)>15
 ){
 raTagFlag=1;
 DR("ra-tag_DU=1"<<endl)
 }
 
 if(dC<-10&&letterLine[i].OCRIndex!='N'&&letterLine[i].OCRIndex!='Z')continue; //применяем только устойчивые буквы
 
 if(letterLine[j].name=="ར"&&letterLine[i].name!="ྲ"&&letterLine[i].OCRIndex!='X')continue;
 
 if((letterLine[i].OCRIndex=='W'||letterLine[i].OCRIndex=='R')&&letterLine[j].OCRIndex!='W'){
 if(letterLine[j].y0>letterLine[i].y0&&dC>2){
 letterLine[i].correlation=0;
 DR("remove letter by down letter"<<endl)
 break;
 }
 }
 if(letterLine[i].name=="འ"){
 if(letterLine[j].y0<letterLine[i].y0&&letterLine[j].correlation>85
 &&letterLine[j].letterW>letterLine[i].letterW&&letterLine[i].y0>y1Base){
 letterLine[i].OCRIndex='W';
 DR("set 'a letter by up letter"<<endl)
 }
 }
 
 if(dC<-5&&letterLine[i].OCRIndex!='N'&&letterLine[i].OCRIndex!='Z')continue; //далее применяем только устойчивые буквы
 if((letterLine[i].OCRIndex=='A'||letterLine[i].OCRIndex=='W')
 &&letterLine[i].name!="ར"
 &&letterLine[i].name!="བ"
 &&letterLine[i].name!="པ"
 &&letterLine[i].name!="འ"
 &&letterLine[i].name!="ང"
 &&dC<2)continue;
 
 //проверяем есть ли пересечение букв и вычисляем ее площадь.
 OCRBox sBox;
 intArea(&letterLine[i].s,&letterLine[j].s,&sBox);
 s_=sBox.area;
 
 DR(" s_="<<s_<<" s="<<s<<" letterLine[i].name="<<letterLine[i].name<<" letterLine[j].name="<<letterLine[j].name<<endl)
 
 if(s_>0&&letterLine[j].name=="ར"&&letterLine[i].name=="ྲ"){
 letterLine[j].correlation+=5;
 letterLine[i].y1=letterLine[j].y1;
 letterLine[i].correlation=0;
 DR("@@@@ remove ra-tag by ra"<<endl)
 break;
 }
 
 
 if(letterLine[i].name=="་"){
 if(s_>s/4&&dC>-7&&letterLine[j].OCRIndex!='V'
 //&&letterLine[j].name!="ད"&&letterLine[j].name!="ང"
 //&&letterLine[j].name!="ར"&&letterLine[j].name!="པ"&&letterLine[j].name!="འ"
 ){
 //if(letterLine[i].xCenter>letterLine[j].xCenter||letterLine[i].xCenter<letterLine[j].x1){
 letterLine[i].correlation=0;
 DR("@@@@ remove tsag by root letter"<<endl)
 break;
 //}
 }continue;
 }
 
 if(s_>0&&letterLine[i].OCRIndex=='N'){
 if(dC>0){
 letterLine[i].correlation=0;
 DR("remove letter N_I"<<endl)
 break;
 }else{
 letterLine[j].correlation=0;
 DR("remove letter N_J"<<endl)
 }
 continue;
 }
 //if(s_>0&&letterLine[i].name!="་"&&letterLine[i].name!="།"&&letterLine[i].name!="•"&&letterLine[i].OCRIndex=='Z'){
 //        letterLine[i].correlation=0;
 //        DR("remove letter Z"<<endl)
 //        break;
 //}
 if(s_>s/2&&letterLine[i].OCRIndex=='Z'&&letterLine[i].name!="་"){
 if(dC<0){
 DR("remove j by intersection with i")
 letterLine[j].correlation=0;
 }else{
 if(letterLine[i].name=="༔"&&letterLine[j].name=="༠")continue;
 DR("remove2 / dC="<<dC<<endl)
 letterLine[i].correlation=0;
 break;
 }
 continue;
 }
 if(s_>0&&(dC<-12||letterLine[j].name=="1"||letterLine[j].name=="]"||letterLine[j].name=="/"||letterLine[j].name=="[")&&(letterLine[i].name=="•"||letterLine[i].name=="།")){
 if(dC<-5){
 letterLine[j].correlation=0;
 }else{
 DR("remove2 / dC="<<dC<<endl)
 letterLine[i].correlation=0;
 break;
 }
 continue;
 }
 
 
 
 if((s_>s/2&&letterLine[i].name=="ར"&&letterLine[j].OCRIndex=='A')||
 (s_>s/3&&letterLine[i].name=="ར"&&letterLine[j].OCRIndex=='A'&&
 (letterLine[i].x1>letterLine[j].x1+7||letterLine[i].x0<letterLine[j].x0-7))
 ){
 if(dC>-7&&letterLine[j].name!="ཇ"&&
 letterLine[j].name!="ཟ"&&
 letterLine[j].name!="ང"&&
 letterLine[j].name!="ད"){
 DR("@@@@ remove letterRa c0="<<letterLine[i].correlation<<" c1="<<letterLine[j].correlation<<endl)
 letterLine[i].correlation=0;
 break;
 }
 if((letterLine[j].name=="ཇ"||letterLine[j].name=="ཟ")&&dC>-1){
 DR("@@@@ remove letterRa Za c0="<<letterLine[i].correlation<<" c1="<<letterLine[j].correlation<<endl)
 letterLine[i].correlation=0;
 break;
 }
 
 }
 if(s_>s/4&&letterLine[i].name=="ར"&&letterLine[j].name=="ི"){
 if(dC>-5){
 DR("@@@@ remove letterRa by A c0="<<letterLine[i].correlation<<" c1="<<letterLine[j].correlation<<endl)
 letterLine[i].correlation=0;
 break;
 }
 
 }
 if(s_>s/2&&letterLine[i].name=="ེ"&&(letterLine[j].name=="ི"||letterLine[j].name=="ོ")){
 if(dC>-5){
 DR("@@@@ remove letterE by O c0="<<letterLine[i].correlation<<" c1="<<letterLine[j].correlation<<endl)
 letterLine[i].correlation=0;
 break;
 }
 
 }
 
 if(s_>s/3){  //&&letterLine[j].letterW>letterLine[i].letterW*0.75
 if(letterLine[i].name=="ར"&&
 (dC<-2||letterLine[j].y0>letterLine[i].y0-5)
 )continue;
 if(letterLine[i].name=="ང"){
 if((letterLine[j].name=="ཇ"||letterLine[j].name=="བ"||letterLine[j].name=="པ")&&dC>-2){
 DR("remove Nga by Za"<<endl)
 letterLine[i].correlation=0;
 break;
 }
 if(letterLine[j].name=="ད"&&letterLine[j].y1>letterLine[i].y1+5&&dC>-3){
 DR("remove Nga by Da"<<endl)
 letterLine[i].correlation=0;
 break;
 }
 continue;
 }
 if(letterLine[i].name=="བ"||letterLine[i].name=="པ"){
 if((letterLine[j].name=="ག"||letterLine[j].name=="ཁ"||
 letterLine[j].name=="ཐ"||letterLine[j].name=="ཏ"||letterLine[j].name=="ཝ")&&dC>-10){
 DR("remove Ba by Ga"<<endl)
 letterLine[i].correlation=0;
 break;
 }
 continue;
 }
 
 if(letterLine[i].name=="འ"){
 if((letterLine[j].name=="ལ")&&dC>-2){
 letterLine[i].correlation=0;
 break;
 }
 continue;
 }
 
 if(letterLine[i].OCRIndex=='X'){
 if(dC>5&&letterLine[j].name=="ར"){
 letterLine[i].correlation=0;
 DR("remove letter X"<<endl)
 break;
 }
 continue;
 }
 
 if(letterLine[i].name=="ྭ"&&
 (hasTail(letterLine[j])||
 letterLine[j].name.find("ཕ")!=-1||letterLine[j].name.find("ྲ")!=-1
 )
 )continue;
 if((letterLine[i].name=="ུ"||letterLine[i].name=="ྲ"||letterLine[i].name=="ྱ")&&hasTail(letterLine[j])
 &&letterLine[i].y1>letterLine[j].y0+letterLine[j].letterW
 )continue;
 if((letterLine[i].name=="ྲ"||letterLine[i].name=="ྱ")&&letterLine[j].name=="ུ"&&
 letterLine[j].y1>letterLine[i].y1+letterLine[j].letterH/2
 )continue;
 if(letterLine[i].name=="ྲ"&&(letterLine[j].name=="པ"||letterLine[j].name=="བ"||letterLine[j].name=="ཟ"||letterLine[j].name=="ཐ")&&dC<-3)continue;
 
 if(letterLine[i].name=="་"&&dC<=0)continue;
 if(letterLine[i].OCRIndex=='V'&&letterLine[j].y0<y0Base-5)continue;
 
 DR("@@@@ remove letter1 c0="<<letterLine[i].correlation<<" ci="<<letterLine[j].correlation<<endl)
 letterLine[i].correlation=0;
 break;
 }
 DR("no enough intersection"<<endl)
 
 }
 
 }
 
 if(!raTagFlag&&(letterLine[i].OCRIndex=='W'||letterLine[i].OCRIndex=='R')){
 DR("remove. hasn't root letter"<<endl)
 letterLine[i].correlation=0;
 }
 
 }
 */




