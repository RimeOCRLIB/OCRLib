
/*GLetter* matchLine[(int)(inputBitmap->columns()*scale)+256];
 for(int m=0;m<matchLine.size();m++){
 if(matchLine[m].correlation){
 matchLine[m]=aliKali->getLetter(matchLine[m].letterIndex);
 }else{
 matchLine[m]=0;
 }
 }
 */
int ln=matchLine.size(); int maxW=0, indexMaxW;
yLimit1=(strArray[0][index].y0-strArray[0][index].LimY0)*scale;
for(int m=0;m<ln;m++){
    if(matchLine[m].correlation==0){
        matchLine[m].status=1;  //исключаем пустые значения
        continue;
    }
    //cout<<"m="<<m<<" c="<<matchLine[m].correlation<<" n="<<matchLine[m].name<<endl;
    if(matchLine[m].name=="་"){            //@@@
        //если тсерто не на месте мы его гасим
        DT2("/tserto/__yLimit1"<<yLimit1<<" maxY="<<matchLine[m].maxY<<" correlation="<<matchLine[m].correlation<<END);
        if(abs((int)(LimY0*scale)-matchLine[m].maxY)>
           matchLine[m].letterW*2){
            matchLine[m].correlation=5;
            matchLine[m].status=1;  //исключаем из рассмотрения
        }
    }
    
    if(matchLine[m].OCRIndex==3&&matchLine[m].name!="ུ"&&matchLine[m].name!="ཱ"&&matchLine[m].name!="ཱུ"&&matchLine[m].name!="་"&&matchLine[m].name[0]!='_'){
        //если огласовка не на месте мы её гасим
        if((int)(LimY0*scale)-matchLine[m].maxY>matchLine[m].letterH*3||  //1.5
           (int)(LimY0*scale)-matchLine[m].maxY<matchLine[m].letterH*0.3){
            matchLine[m].correlation=5;
            matchLine[m].status=1;  //исключаем из рассмотрения
        }
        DT2("/vowel/__LimY0="<<LimY0*scale<<" maxY="<<matchLine[m].maxY<<" correlation="<<
            matchLine[m].correlation<<" name="<<matchLine[m].name<<" letterH="<<matchLine[m].letterH<<endl);
        
    }
    
    
    //if(matchLine[m].Wylie=="."||matchLine[m].Wylie==","){
    //   //если точка не на месте мы её гасим
    //   DT2("/point/__yLimit1="<<yLimit1<<" maxY="<<matchLine[m].maxY<<" correlation="<<matchLine[m].correlation<<END);
    //   if(matchLine[m].maxY-yLimit1<<aliKali[0][matchLine[m].letterIndex].letterW){
    //	  matchLine[m].correlation=5;
    //	  matchLine[m].status=1;  //исключаем из рассмотрения
    //   }
    //}
    
    
    DT2("matchLine["<<m<<"].correlation="<<matchLine[m].correlation<<
        " name="<<matchLine[m].name.c_str()<<"     xMax="<<m-borderMatrix<<"   y="<<matchLine[m].maxY<<" w="<<matchLine[m].letterW<<" in="<<matchLine[m].OCRIndex<<END);
    
}

DT2("//__________"<<END<<END<<END<<END<<END<<"//__________"<<END<<END<<END<<END<<END);
DT2("m3_2");
while(1){  //продолжаем до полной разборки строки
    //находим максимальную неразобранную букву макимальной ширины
    maxW=0;
    for(int m=0;m<ln;m++){
        if (matchLine[m].status)continue;
        if (matchLine[m].letterW>maxW){
            maxW=matchLine[m].letterW;
            
            indexMaxW=m;
            
        }
    } if(maxW==0)break;
    DT2(END<<"//@@@/indexMaxW="<<indexMaxW<<"________________________________________________________________/check maxW="<<maxW<<"indexMaxW="<<indexMaxW<<" name"<<matchLine[indexMaxW].name<<" OCRIndex="<<matchLine[indexMaxW].OCRIndex<<" correlation="<<matchLine[indexMaxW].correlation<<END);
    //  теперь внутри этой буквы мы ищем вложенные буквы
    //  если эти буквы являются частью буквы мы их гасимю
    x0=indexMaxW-maxW/2-5; if(x0<0)x0=0;
        x1=indexMaxW+maxW/2+5; if(x1>ln)x1=ln;
            
            //отмечаем  саму тестовую букву как разобранную
            matchLine[indexMaxW].status=1;
            
            for (int xInd=x0;xInd<x1;xInd++){
                DT2("/0/________________check c="<<matchLine[xInd].status<<" xInd="<<xInd<<" name="<<matchLine[xInd].name<<END);
                //если буква, взятая для рассмотрения, не огласовка, то идем дальше
                //огласовки и церто рассматриваем до победного
                if ((matchLine[xInd].status)&&
                    (matchLine[xInd].OCRIndex!=3))continue;
                
                DT2(END<<"/0/______________________________check c="<<matchLine[xInd].status<<" xInd="<<xInd<<" name="<<matchLine[xInd].name<<
                    " correlation="<<matchLine[xInd].correlation<<" wy="<<matchLine[xInd].Wylie<<" OCRIndex="<<matchLine[xInd].OCRIndex<<END);
                
                
                //если внутри буквы мы находим саму себя c меньшей корреляцией мы ее гасим
                if(matchLine[xInd].name==matchLine[indexMaxW].name&&indexMaxW!=xInd){
                    if(matchLine[xInd].correlation<=matchLine[indexMaxW].correlation){
                        matchLine[xInd].correlation=5;
                        DT2("/5/");
                        continue;
                    }
                }
                DT2("/5_1/");
                //если внутри буквы стоит огласовка, и сама эта буква огласовка
                //то мы ее гасим в случае меньшей корреляции
                if(matchLine[indexMaxW].OCRIndex==3){
                    if(
                       matchLine[xInd].OCRIndex==3){
                        if(!(matchLine[indexMaxW].Wylie!="tsheg"&&
                             matchLine[xInd].Wylie=="tsheg")){ //огласовки не гасят церто
                            if(matchLine[xInd].correlation<matchLine[indexMaxW].correlation){
                                matchLine[xInd].correlation=19; DT2("/m31/"<<xInd);
                                //matchLine[xInd].status=1; //отмечаем букву как разобранную										 }
                            }
                        }
                    }
                    DT2("/31/");
                }
                DT2("/5_2/");
                //сочетания буквы и огласовки мы дальше не проверяем
                if(matchLine[indexMaxW].OCRIndex==3||matchLine[xInd].OCRIndex==3){
                    continue;
                }
                //matchLine[xInd].OCRIndex>matchLine[m].OCRIndex&&
                //если внутри буквы есть буквы с невысокой корреляцией мы их гасим
                if(matchLine[xInd].correlation<=matchLine[indexMaxW].correlation+2){
                    matchLine[xInd].correlation=32; DT2("/m32/"<<xInd);
                    matchLine[xInd].status=1; //отмечаем букву как разобранную										 }
                }else{
                    //если же эта часть имеет высокую корреляцию, то нужно что-то делать.
                    //вероятнее всего нужно погасить большую букву
                    if(matchLine[indexMaxW].OCRIndex!=3&&matchLine[xInd].name!="་"){
                        matchLine[indexMaxW].correlation=33; DT2("/m33_/"<<xInd);
                        matchLine[indexMaxW].status=1; //отмечаем букву как разобранную
                        //после этого нужно опять проверять все правила для этого места
                        break;
                    }
                }
            }
}

DT2("m3_3");
//повторяем цикл разборки стороки
//если огласовка стоит перед буквой, меняем их местами
int indexLetter=-1;
for(int m=0;m<matchLine.size();m++){
    if(matchLine[m].correlation<70)continue;
    
    if(matchLine[m].OCRIndex==3&&matchLine[m].name!="་"){
        DT2("//vowel found indexLetter="<<m<<END);
        indexLetter=m;
    }else{
        //проверяем предыдущую огласовку
        if(indexLetter<=0)continue; //начальные значения
        //проверяем, находится ли предыдущая огласовка внутри буквы
        if(m-matchLine[m].letterW/1.5<indexLetter){
            //если да, то ставим огласовку после буквы
            DT2("//start swap"<<" matchLine["<<indexLetter<<"]="<<matchLine[indexLetter].name<<" matchLine["<<m<<"]="<<matchLine[m].name<<END);
            
            
            //ищем куда вставить огласовку
            int position=m+1;
            while(position<ln){
                if(matchLine[position].correlation<70){
                    matchLine[position].Wylie=matchLine[indexLetter].Wylie;
                    matchLine[position].name=matchLine[indexLetter].name;
                    matchLine[position].letterIndex=matchLine[indexLetter].letterIndex;
                    matchLine[position].correlation=matchLine[indexLetter].correlation;
                    matchLine[position].OCRIndex=matchLine[indexLetter].OCRIndex;
                    matchLine[position].letterID=matchLine[indexLetter].letterID;
                    matchLine[position].letterIndex=matchLine[indexLetter].letterIndex;
                    matchLine[position].maxY=matchLine[indexLetter].maxY;
                    break;
                }
                position++;
            }
            matchLine[indexLetter].correlation=0;
            DT2("//end swap"<<" matchLine["<<indexLetter<<"]="<<matchLine[indexLetter].name<<" matchLine["<<m<<"]="<<matchLine[m].name<<END);
        }
    }
}

DT2("m3_4");
//распознаем рамку вокруг текста
strArray[0][index].stringFlag=1;
count=0; int countLetter=0;
for(int m=0;m<matchLine.size();m++){
    //горизонтальная рамка. помечаем строку как пустую
    if(matchLine[m].correlation<80)continue;
    if(matchLine[m].name=="---"||matchLine[m].name=="—"){count++;}else{
        countLetter++;
    }
    if(count>10||countLetter>10)break;
}
//проверяем имеем ли мы дело с таблицей или с рамкой вокруг текста
//cout<<"@@@frame count="<<count<<" countLetter="<<countLetter<<" this string is a frame. index="<<index<<" strArray[0].size()="<<strArray[0].size()<<endl;
if(count>countLetter/3){
    
    //убираем колонтитул
    if(index>strArray[0].size()-4&&strArray[0].size()>5){//если это верхняя рамка текста, убираем колонтитул
        for (int ind=index;ind<strArray[0].size();ind++){
            strArray[0][ind].wordArray.resize(0);
            strArray[0][ind].stringFlag=-1;
        }
    }
	
}

for(int m=0;m<matchLine.size();m++){
    //вертикальная рамка. Убираем шум вокруг рамки
    if(matchLine[m].name=="|"&&matchLine[m].correlation>80){
        if(m<matchLine.size()/2){
            //проверяем имеем ли мы дело с таблицей или с рамкой вокруг текста
            count=0;
            for(int t=0;t<=m;t++){
                if(matchLine[t].correlation>80&&matchLine[t].name!="|")count++;
            }
            if(count<3){
                for (int ind=0;ind<strArray[0][index].wordArray.size();ind++){
                    if(strArray[0][index].wordArray[ind].x0<m){
                        strArray[0][index].wordArray[ind].correlation=-1;
                        strArray[0][index].wordArray[ind].name="^";
                    }
                }
                for(int t=0;t<=m;t++) matchLine[t].correlation=0;
                    }
        }else{
            //проверяем имеем ли мы дело с таблицей или с рамкой вокруг текста
            count=0;
            for(int t=m;t<matchLine.size();t++){
                if(matchLine[t].correlation>80&&matchLine[t].name!="|")count++;
            }
            if(count<3){
                for (int ind=0;ind<strArray[0][index].wordArray.size();ind++){
                    if(strArray[0][index].wordArray[ind].x1>m){
                        strArray[0][index].wordArray[ind].correlation=-1;
                        strArray[0][index].wordArray[ind].name="^";
                    }
                }
                for(int t=m;t<matchLine.size();t++) matchLine[t].correlation=0;
                    }
        }
    }
}
//strArray[0][index].stringFlag=1;
if(strArray[0][index].stringFlag<0)continue;

DT2("m3_5_1 space=");
for(int m=0;m<matchLine.size();m++){
    if(matchLine[m].correlation==0)continue;
    DT2("matchLine["<<m<<"].correlation="<<matchLine[m].correlation<<
        " name="<<matchLine[m].name.c_str()<<"        w="<<matchLine[m].letterW<<" in="<<matchLine[m].OCRIndex<<END);
    
    if(matchLine[m].OCRIndex==3&&matchLine[m].correlation<85){continue;}
    
    wIndex=0;
    //if(matchLine[m].correlation>90)DT2("//_____________");  ///@@@
    if(matchLine[m].correlation>50){
        DT2("matchLine[m].letterIndex="<<matchLine[m].letterIndex<<" aliKali->base.size()="<<aliKali->letterCount());
        //matchLine[m].OCRStatus=1000; //помечаем букву как букву из этой книги
        count=0;
        //теперь внутри буквы мы убираем лишние квадратики
        //и устанавливаем верные габариты буквы
        //DT2("/@/_______m="<<m<<" ="<<matchLine[m].name<<" matchLine[m].maxY="<<matchLine[m].maxY
        //	<<" aliKali[0]["<<matchLine[m].letterIndex<<"]->ySum="<<matchLine[m].ySum
        //	<<" yLimit0="<<yLimit0<<END);
        
        //cout<<"m="<<m-borderMatrix<<" matchLine[m].xCenter="<<matchLine[m].xCenter-borderMatrix<<" name="<<matchLine[m].name<<endl;
        x0=(matchLine[m].xCenter-matchLine[m].letterW/2-borderMatrix)/scale;//+aliKali[0][matchLine[m].letterIndex]->dX;
        if(x0<0)x0=0;
            x1=(matchLine[m].xCenter+matchLine[m].letterW/2-borderMatrix)/scale;//+aliKali[0][matchLine[m].letterIndex]->dX;
            if(x1>ln)x1=ln;
                
                for (int in=0;in<strArray[0][index].wordArray.size();in++){
                    if(strArray[0][index].wordArray[in].center.x>x0&&
                       strArray[0][index].wordArray[in].center.x<x1&&
                       strArray[0][index].wordArray[in].name=="*"){
                        DT2("old"<<END);
                        if(count==0){
                            strArray[0][index].wordArray[in].name=matchLine[m].name;
                            strArray[0][index].wordArray[in].Wylie=matchLine[m].Wylie;
                            strArray[0][index].wordArray[in].correlation=matchLine[m].correlation;
                            strArray[0][index].wordArray[in].w=matchLine[m].letterW/scale;
                            strArray[0][index].wordArray[in].x0=x0;
                            strArray[0][index].wordArray[in].x1=x1;
                            
                            //if(w1>strArray[0][index].wordArray[in].w)strArray[0][index].wordArray[in].w=w1;
                            strArray[0][index].wordArray[in].lineIndex=index;
                            strArray[0][index].wordArray[in].letterID=matchLine[m].letterID;
                            strArray[0][index].wordArray[in].letterIndex=matchLine[m].letterIndex;
                            
                            //strArray[0][index].wordArray[in].center.x=(strArray[0][index].wordArray[in].x1-strArray[0][index].wordArray[in].x0)/2;
                            //strArray[0][index].wordArray[in].center.y=matchLine[m].maxY;
                            strArray[0][index].wordArray[in].OCRIndex=matchLine[m].OCRIndex;
                            strArray[0][index].wordArray[in].y0=strArray[0][index].y0;//-yLimit0
                            //+matchLine[m].maxY
                            //-aliKali[0][matchLine[m].letterIndex]->letterH/2
                            //-aliKali[0][matchLine[m].letterIndex]->ySum;
                            strArray[0][index].wordArray[in].y1=strArray[0][index].y1;//-yLimit0
                            //+matchLine[m].maxY
                            //+aliKali[0][matchLine[m].letterIndex]->letterH/2
                            //-aliKali[0][matchLine[m].letterIndex]->ySum;
                            wIndex=in;
                            strArray[0][index].wordArray[in].id=id_index; id_index++;
                            lineText.push_back(strArray[0][index].wordArray[in]);
                            DT2("x0="<<x0<<" x1="<<x1<<" center.x="<<strArray[0][index].wordArray[in].center.x<<END);
                        }else{
                            if(strArray[0][index].wordArray[in].name=="*"){
                                strArray[0][index].wordArray.erase(strArray[0][index].wordArray.begin()+in,strArray[0][index].wordArray.begin()+in+1);
                            }
                        }
                        
                        count++;
                    }
                    
                }
        if(count==0){ //не найдено подходящих квадратиков, нужно вставить в строку новый
            DT2("new"<<END);
            wordOCR wP;
            wP.center.x=(m-borderMatrix)/scale;
            wP.center.y=matchLine[m].maxY/scale;
            wP.name=matchLine[m].name;
            wP.Wylie=matchLine[m].Wylie;
            wP.w=matchLine[m].letterW/scale;
            wP.x0=x0;
            wP.x1=x1;
            wP.OCRIndex=matchLine[m].OCRIndex;
            wP.letterID=matchLine[m].letterID;
            wP.letterIndex=matchLine[m].letterIndex;
            wP.y0=strArray[0][index].y0;//-yLimit0
            //+matchLine[m].maxY
            //-aliKali[0][matchLine[m].letterIndex]->letterH/2
            //-aliKali[0][matchLine[m].letterIndex]->ySum;
            wP.y1=strArray[0][index].y1;//-yLimit0
            //+matchLine[m].maxY
            //+aliKali[0][matchLine[m].letterIndex]->letterH/2
            //-aliKali[0][matchLine[m].letterIndex]->ySum;
            wP.correlation=matchLine[m].correlation;
            wP.lineIndex=index;
            //strArray[0][index].wordArray.push_back(wP);
            strArray[0][index].wordArray.insert(strArray[0][index].wordArray.begin()+wIndex,wP);
            wIndex++;
            wP.id=id_index; id_index++;
            lineText.push_back(wP);
        }
        
        //сохраняем статистику использования буквы
        /*
         if(aliKali[0][matchLine[m].letterIndex]->cn==0){
         char key[0]; sprintf(key,"%d",matchLine[m].letterIndex);
         int ecode;
         if(!tcbdbput2(inputData.bookDataDB, key, "1")){
         ecode = tcbdbecode(inputData.bookDataDB);
         fprintf(stderr, "put error: %s\n", tcbdberrmsg(ecode));
         }
         }
         */
        //aliKali[0][matchLine[m].letterIndex]->crSum+=matchLine[m].correlation;
    }
}
//mainString+="\n";
//оцениваем результат распознавания строкию Если он не соответствует результату распознавания
//предыдущей строки, запускаем определение размера строки и распознаем строку с новым масштабом
correlation=0; int count=0;
for(int m=0;m<matchLine.size();m++){
    if(matchLine[m].correlation>70){
        if(matchLine[m].name=="་")continue;
        correlation+=matchLine[m].correlation;
        count++;
    }
}
if(count&&correlation){
    correlation/=count;
    cout<<"@_"<<index<<"_line///@@@@ main scale="<<scale<<" stepCount="<<stepCount<<" correlation="<<correlation<<endl;
    if(!lineCorrelation)lineCorrelation=correlation;
    }else{
        stepCount++;    cout<<"stepCount="<<stepCount<<"c=0 goto_ScaleDetector"<<endl;
        if(stepCount<2){
            needLineSize=0;
            if(scalePage==0)goto ScaleDetector;
        }
    }
    
    DT2(" correlation="<<correlation<<" bestCorrelation="<<bestCorrelation<<endl);
    if((correlation+correlation/10<bestCorrelation&&correlation<85)||correlation<70){
        stepCount++;   cout<<"stepCount="<<stepCount<<"c="<<correlation<<" best c="<<bestCorrelation<<" goto_ScaleDetector"<<endl;
        if(stepCount<2){
            needLineSize=0;
            if(scalePage==0)goto ScaleDetector;
        }
    }
    
    //space recognition
    //cout<<"@@@@space recognition MeanSA="<<strArray[0][index].MeanSA<<endl;
    int space=(strArray[0][index].MeanSA*10)/spaceIndex;
    
    int bRightOld=0,lw1,bLeft,space1,space0;
    if(lineText.size())bRightOld=lineText[0].x1;
    for(int wInd=1;wInd<lineText.size();wInd++){
        
        //cout<<"lineText["<<wInd<<"]="<<lineText[wInd].name<<endl;
        lw1=lineText[wInd].w;   //cout<<"lw1="<<lw1;
        bLeft=lineText[wInd].x0;
        space1=bLeft-bRightOld;   if(space1<0)space1=0;
        //cout<<"lineText["<<wInd<<"]="<<lineText[wInd].name<<" space1="<<space1<<" space="<<space<<" bRightOld="<<bRightOld<<" bLeft="<<bLeft<<endl;
        if(space1>space&&lineText[wInd].OCRIndex!=3){
            /*if(lineText.size()){
             if(lineText[lineText.size()-1].name!="་"&&matchLine[m].name!="་"){
             wordOCR st;
             st.name="";
             st.delimeter=" ";
             st.newLine="";
             DT2(" space found"<<END);
             st.id=id_index; id_index++;
             lineText.push_back(st);
             }
             }
             */
            wordOCR wP;
            
            wP.name="•";
            wP.Wylie="•";
            wP.w=space1;
            wP.x0=bRightOld/scale;
            wP.x1=lineText[wInd].x0/scale; if(wP.x1>wP.x0+50)wP.x1=wP.x0+50;
            wP.center.x=(wP.x1-wP.x0)/2+wP.x0;
            wP.y0=lineText[wInd].y0/scale;
            wP.y1=lineText[wInd].y1/scale;
            wP.center.y=(wP.y1-wP.y0)+wP.y0;
            wP.correlation=100;
            wP.lineIndex=index;
            wP.letterIndex=0;
            //for(int a=0;a<lineText.size();a++){
            //    if(lineText[a].x0>wP.center.x){
            //cout<<"insert space wP.x0="<<wP.x0<<" lineText[a].x0="<<lineText[wInd].x0<<endl;
            lineText.insert(lineText.begin()+wInd,wP);
            wInd++;
            //        break;
            //    }
            
            //}
            
        }
        //cout<<".3";
        space0=space1;
        if(lineText[wInd].x1>bRightOld)bRightOld=lineText[wInd].x1;
        
        count=0;
        //DT2("/@/_______m="<<m<<" ="<<matchLine[m].name<<" matchLine[m].maxY="<<matchLine[m].maxY
        //    <<" aliKali[0]["<<matchLine[m].letterIndex<<"]->ySum="<<aliKali[0][matchLine[m].letterIndex]->ySum
        //    <<" yLimit0="<<yLimit0<<END);
    }
    
    //cout<<"@@@ done space recognition"<<endl;
    
    if(correlation>=lineCorrelation&&lineCorrelation>0){
        //lineText.resize(0);
        //pageText.erase(pageText.begin()+pageTextSize0,pageText.begin()+pageTextSize1);
        //    stringArray[index]->wordArray->clearOCR();
    }
    
    //cout<<"lineText.size()="<<lineText.size()<<endl;
    
    strArray[0][index].wordArray.resize(0);
    for(int a=0;a<lineText.size();a++){
        strArray[0][index].wordArray.push_back(lineText[a]);
        //cout<<strArray[0][index].wordArray[a].name;
    }
    
    matchLine.resize(0);
    if(needScaleFlag)line->destroy();  //destroy text line bitmap
    
    //if(pageText.size())pageText[pageText.size()-1].newLine="\n";
    
    DT2("m4"<<endl;)
    
    /* //###ALERT need redevelop in GBitset (horizontal frame detector)  //уже сделано выше
     int countFrame=0;
     for(int t=0;t<strArray[0][index].wordArray.size();t++){
     if(strArray[0][index].wordArray[t].name.find("–")!=string::npos){countFrame++;}
     
     }
     //cout<<"countFrame="<<countFrame<<" index="<<index<<endl;
     if(countFrame>10&&strArray[0].size()-index<4)strArray[0][index].wordArray.resize(0);
     //###
     */

