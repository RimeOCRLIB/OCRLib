#include "GFontEditor.h"

void GFontEditor::setLetter(){
if(!aliKali)return;
int print=0;
pechaImg=LoadImageData(inputData.data["inputFile"],0);
DR("inputData.data[\"inputFile\"]="<<inputData.data["inputFile"]<<END)
if(!pechaDataLoaded){cout_<<"no image data"; return;}

xml_node col,data,resultSet,GFontXML, st;
xml_document doc;
int x0,x1,y0,y1,w,h;
    
((GStr<int>*)inputData.pref)->put(5,1);   //отметка что в базе новая буква

string str;
GLetter *letter;
char *cbuf=(char*)malloc(inputData.data["setLetter"].size()+1);
memcpy(cbuf,inputData.data["setLetter"].c_str(),inputData.data["setLetter"].size());
cbuf[inputData.data["setLetter"].size()]=0;
    str=cbuf;
	str=decodeURLString(str);
	str=str_replace("{","<",str);
	str=str_replace("}",">",str);
    str=str_replace("\\","",str);
    memcpy(cbuf,str.c_str(),str.size());
    DR("str="<<str<<endl)
    
if(!doc.parse(cbuf)){
		  cout<<"srcStr not loaded"<<END;return;
}
    
/*    float scale=1;//(float)atoi(inputData.data["scale"].c_str())/100;  //1 не выполняется масштабирование
if(scale<=0)scale=1; if(scale>5)scale=5;  cout_<<"scale="<<scale<<endl;
    if(scale!=1){
        GBitmap *scaleImg=GBitmap::createScale(pechaImg,scale);
        pechaImg->destroy();
        pechaImg=scaleImg;
        
    }    
*/
DR("setLetter="<<inputData.data["setLetter"]<<" cbuf="<<cbuf<<endl)
DR("inputData.data[\"newLetter\"]"<<inputData.data["newLetter"]<<END)
DR("aliKali->letterCount"<<aliKali->letterCount()<<END)

///set line coordinates in page	
    
    GBitset *setBit=GBitset::createResize(pechaImg,1,1,1);
        
	DR("@4_2")
	vector<stringOCR>strArray; 
	setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
	DR("@4_3 strArray.size()="<<strArray.size()<<endl)
	setBit->destroy();
	
	
int letterFlag,index,idNumber;
string name;
vector<GLetter>letterLine;
	

for (xml_node row = doc.child("glyph"); row; row = row.next_sibling("glyph")){

        index=-1;
        letterFlag=0;
        name=row.attribute("n").value();
        name=str_replace(" ", "",name);
        if(name=="*"||name=="")continue;
        //str=name.substr(0,name.find("/"));

		letter=GLetter::create();
        if(row.attribute("n").value()!=NULL)letter->name=row.attribute("n").value();
        DR("name1="<<letter->name.c_str()<<" fileName="<<aliKali->fileName<<" letterCount="<<aliKali->letterCount()<<endl);
        
		DR("letter->name="<<letter->name.c_str()<<"/"<<endl)
        letter->recordStatus=1;
    
    if(RE2::PartialMatch(letter->name,"[\\p{Devanagari}]+")){
        letter->OCRKey="S";
    }else{
    
        int indBase=logicProcessor->fontGMap->getHKey(name,8);
    
        TString strT; logicProcessor->fontTable->getTStr(indBase,&strT);
        if(strT.size()>7){
           DR("name="<<strT[8]<<" OCRIndex = "<<strT[4]<<endl)
           letter->OCRKey=strT[4];
        }else{
           cout<<"no letter in base";
           exit(0);
        }
    }     
		int dl=6; //JavaScript compensation 
		x0=atoi(row.attribute("x0").value());
		x1=atoi(row.attribute("x1").value())+dl;
		y0=atoi(row.attribute("y0").value());
		y1=atoi(row.attribute("y1").value())+dl;
 	    idNumber=(int)strtoul(row.attribute("l").value(),NULL,0); //cout_<<" wp.idNumber="<<wp.idNumber<<endl;
		int textLineH;

		//set text line scale and coordinate
        textLineH=34;

		w=x1-x0; if(w>128)w=128;
		h=y1-y0; if(h>128)h=128;
		DR("x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<" w="<<w<<" h="<<h<<endl)
		letter->letterH=h;  //SH(letter->letterH);
		letter->letterW=w;  //SH(letter->letterW);
		letter->letterIndex=aliKali->letterCount()-1;
        if(x0>pechaImg->columns()||y0>pechaImg->rows())return;

		int flag=0; letter->stackFlag=1;
        for(unsigned int d=0;d<logicProcessor->uniTibTable.size();d++){
		    //cout_<<"m11";
			if(logicProcessor->uniTibTable.find(letter->name)!=logicProcessor->uniTibTable.end()){
			   letter->OCRKey=logicProcessor->uniTibTable[letter->name].OCRKey;
			   letter->stackFlag=logicProcessor->uniTibTable[letter->name].stackFlag;
			   letter->OCRIndex=logicProcessor->uniTibTable[letter->name].OCRIndex;
			   letter->baseLetter=logicProcessor->uniTibTable[letter->name].baseLetter;
			   DR(" letter match ")	
			   flag=1; break;
			}
        }
		letter->Character=0;
		letter->recordFlag=1;
        letter->newLetter=1;

		cout<<"m2 pechaImg->rows()="<< pechaImg->rows();
		GBitmap *img;
        img=GBitmap::createRegion(pechaImg, x0, y0, w, h);
			//LoadImageData(img128,pechaImg, x0,y0,w,h);			 
		//}
		GBitmap *img128=GBitmap::createScale(img,1);
		//WriteImageData(img, "/1.jpg", 0);
        //string draw=letter->drawToSVG();
		//writeText(draw,"/1.svg");
		//img128->printToScreen();
		
		 
		DR("START_PACK img128.columns()="<<img128->columns()<<END)
			 letter->mask128.packBitMask128(img128);  
			 //letter->mask128.printMask();
             //aliKali[0][aliKali->letterCount-1]->mask128.printMask();
             //cout_<<"name4="<<aliKali[0][aliKali->letterCount-1]->name;
			 //exit(0);
			 letter->mask128.xMask=-(letter->mask128.mW/2);
		     
		     //normalisation on slice size
		     int s= img128->columns(); if(s>128)s=128;
			 dl=letter->mask128.mW-s;
		     letter->xMin=500;
			 letter->xMax=0;
		     for(int a=0;a<letter->mask32Count();a++){
			     letter->mask32[a].xMask-=dl/2;
				 if(letter->mask32[a].xMask<letter->xMin)letter->xMin=letter->mask32[a].xMask;
				 if(letter->mask32[a].xMask+letter->mask32[a].imgW>letter->xMax)letter->xMax=letter->mask32[a].xMask+letter->mask32[a].imgW;
			}
		     //cout_<<" letter->xMin="<<letter->xMin<<endl;
			 letter->dX=-(letter->mask128.mW/2-letter->letterW/2-(letter->mask128.mW/2+letter->xMin));
			 letter->dY=-(letter->mask128.mH/2-letter->letterH/2-(letter->mask128.mH/2+letter->yMin));

			 letter->mask128.yMask=-(letter->mask128.mH/2);
             letter->idNumber=0;//letter->mask128.hashValue();
             
             /*int IDflag=1;int cout_Time=0;
                while(IDflag){
                    for(int t=0;t<aliKali->letterCount-1;t++){ cout_Time++;
                        //cout_<<"aliKali->letterSet["<<t<<"]->idNumber="<<aliKali->letterSet[t]->idNumber<<endl;
                        if(letter->idNumber==aliKali->letterSet[t]->idNumber){
                            letter->idNumber+=(unsigned short)clock()+cout_Time;IDflag=2;
                            cout_<<"new letterSet["<<t<<"]->idNumber="<<letter->idNumber<<endl;
                        }
                    }
                    IDflag--;
                }
            */
            str=letter->name;
            for(unsigned int i=0;i<logicProcessor->letterSign.size();i++){
                if(logicProcessor->letterSign[i]==str)letter->OCRIndex=3;
            }	  
            for(unsigned int i=0;i<logicProcessor->letterScale.size();i++){
                if(logicProcessor->letterScale[i]==str)letter->OCRIndex=5;
            }
        
             
		
        //letter->reloadMask();
        //letter->mask128.printMask();
        //exit(0);
		
		     //set text line coordinate
		//cout_<<" y0="<<y0<<endl;
        for (unsigned int index=0;index<strArray.size(); index++){
			//cout_<<"strArray[index].LimY0="<<strArray[index].LimY0<<" strArray[index].LimY1="<<strArray[index].LimY1<<endl;
			if(y0>strArray[index].LimY0&&y0<strArray[index].LimY1){
				letter->y0=(strArray[index].y0-y0);
				letter->y0=-(letter->mask128.mH/2-(letter->y0));
				letter->y1=(strArray[index].y1-y0);
				letter->y1-=letter->mask128.mH/2;
				break;
			}
		
		}
    if(letter->y1-letter->y0<letter->letterH/4){
        letter->y0=-letter->letterH/2;
        letter->y1=letter->letterH/4;
    }
		//cout_<<"letter->mask128.xMask="<<letter->mask128.xMask<<endl;
        //cout_<<"if we don't find this letter in base we try to find it in GFont store"<<endl;
		//setLetterFromTypesetting(letter,img128); 
		img->destroy(); 
		img128->destroy();
        
		aliKali->testLetter=aliKali->letterCount()-1;
        //setMaskFromBase();
        letter->setLetterDimensions();
        letter->recordStatus=1;
        aliKali->push_back(letter);
        cout<<"1_aliKali->letterCount()"<<aliKali->letterCount()<<endl;
        cout<<aliKali->size();
        //exit(0);
        letter->destroy();
}
    
    
}//_____________________________________________________________________________


void GFontEditor::setBase(){

int print=0;
xml_node col,data,resultSet,GFontXML, st;
xml_document doc;
int x0,x1,y0,y1,w,h,dlt0,letter_y0,letter_y1,idMode=0;
string str;
unsigned int index=0;	
int indexFlag=0;	
GBitmap *outBitmap;
    
((GStr<int>*)inputData.pref)->put(5,1);   //отметка что в базе новая буква
    

//str=inputData.data["tablePath"]+"/OCRTables/OCRBase.jpg";
//LoadImageData(str,0);
	str=inputData.data["setBase"];
	
	DR("start set base mode="<<inputData.data["ocrData"]<<" str="<<str<<endl)
	str=decodeURLString(str);
	str=str_replace("{","<",str);
	str=str_replace("}",">",str);
    str=str_replace("\\","",str);
	
char *cbuf=(char*)malloc(str.size()+1);
memcpy(cbuf,str.c_str(),str.size());

cbuf[str.size()]=0;
DR("inputData.data[\"setBase\"]="<<cbuf)

if(!doc.parse(cbuf)){
		  DR("setBase str not loaded"<<END)return;
}

for (xml_node row = doc.child("glyph"); row; row = row.next_sibling("glyph")){
	
    index=(uint)strtoul(row.attribute("id").value(),NULL,0);
    string name=row.attribute("n").value();
    //cout<<" NAME="<<name;
    //for(int i=0;i<name.size();i++)cout<<" "<<(int)name[i];
    //cout<<"/"<<endl;

    GLetter *letter=aliKali->getLetter(index);
        indexFlag=1;
        aliKali->testLetter=index; //send data for testMask();
    letter->recordStatus=1;
    
    if(RE2::PartialMatch(name,"[\\p{Devanagari}]+")){
        letter->OCRKey="S";
    }else{
        int indBase=logicProcessor->fontGMap->getHKey(name,8);
        TString strT;
        logicProcessor->fontTable->getTStr(indBase,&strT);
        if(strT.size()<9){cout<<"no letter "<<name<<" in base"; exit(0);}
        DR("name="<<strT[8]<<"/ OCRIndex = "<<strT[4]<<endl)
        letter->OCRKey=strT[4];    
        DR("index="<<index<<" letterIndex="<<letter->letterIndex<<" letter->OCRKey="<<letter->OCRKey<<" name="<<name<<"/"<<endl) 
    }
        
    if(inputData.data["ocrData"]=="eraseAllMask"){
        letter->eraseAllMask();
        letter->setLetterDimensions();
        aliKali->saveLetter(letter);
        letter->destroy();
        return;
    }
    if(inputData.data["ocrData"]=="setMaskFromBase"){return;}
    if(inputData.data["ocrData"]=="rebuildMaskInLetter"){return;}
    if(inputData.data["ocrData"]=="deleteLetters"||name=="*"){
       letter->name="*";
        letter->recordFlag=0;  
        letter->recordStatus=0;
        letter->OCRKey="";
        DR("@@remove letter "<<index<<endl;)
        aliKali->saveLetter(letter);
        letter->destroy();
        return;
    }

		
		outBitmap=GBitmap::create(PICT_SIZE,PICT_SIZE);
		GBitmap *img=letter->mask128.unPackMask(); 
        //cout_<<"w="<<img->columns()<<" h="<<img->rows();
        for (unsigned int y = 0; y <img[0].rows() ;y++){
			//pSrc = (unsigned char*)letterImage->Canvas->ScanLine[y];
            for (unsigned int x = 0; x <img[0].columns(); x++){
				
                //if((int)img[0][y][x]){cout_<<"1";}else{cout_<<"0";}
                
                if(img[0][y][x]){
                    
                }else{
                    //if(outBitmap[0]
                    //   [y+aliKali[0][index]->mask128.yMask]
                    //   [x+aliKali[0][index]->mask128.xMask]==255){
                    outBitmap[0]
                    [y+letter->mask128.yMask+PICT_SIZE/2]
                    [x+letter->mask128.xMask+PICT_SIZE/2]=0;
                    //}
                }
            }//cout_<<END;
		}

		img->destroy();
        letter->newLetter=1;
    
	    if(name!="")letter->name=name;
	    int n=atoi(row.attribute("sC").value());  //записываем данные о selfCorrelation и кодовом расстоянии
	    if(n)letter->selfCorrelation=n;
		n=atoi(row.attribute("cS").value());
	    if(n)letter->codeSpace=n;
		DR("letter->name="<<letter->name.c_str()<<"/"<<endl)
		
        x0=atoi(row.attribute("x0").value()); 
		x1=atoi(row.attribute("x1").value());
		y0=atoi(row.attribute("y0").value()); 
		y1=atoi(row.attribute("y1").value());
		letter_y0=atoi(row.attribute("c0").value());
		letter_y1=atoi(row.attribute("c1").value()); 
        idMode=atoi(row.attribute("a").value());   DR(" idMode="<<idMode<<endl)
    
		if(letter_y0!=0||letter_y1!=0){
			letter->y0=letter_y0-PICT_SIZE/2-((float)(letter_y0/PICT_SIZE))*PICT_SIZE;
			letter->y1=letter_y1-PICT_SIZE/2-((float)(letter_y1/PICT_SIZE))*PICT_SIZE;
		}


		if(x0!=0&&x1!=0&&y0!=0&&y1!=0){
		   w=x1-x0;
		   h=y1-y0;
		   letter->letterH=h;  //SH(letter->letterH);
		   letter->letterW=w;  //SH(letter->letterW);
           //DR("@y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" letter->dX="<<letter->dX<<" letter->dY="<<letter->dY<<endl)
            letter->dX=x0-PICT_SIZE/2-((float)(x0/PICT_SIZE))*PICT_SIZE+w/2;
            letter->dY=y0-PICT_SIZE/2-((float)(y0/PICT_SIZE))*PICT_SIZE+h/2;
           //DR("y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" letter->dX="<<letter->dX<<" letter->dY="<<letter->dY<<endl)
		}
		letter->lengthSum=0;
		letter->newLetterInBook=1;
        letter->OCRIndex=0;
        letter->Character=0;
        letter->recordFlag=1;
        letter->recordStatus=1;
    
        aliKali->saveLetter(letter);
        letter->destroy();
}

  for (xml_node row = doc.child("mask"); row; row = row.next_sibling("mask")){
	    //if(!indexFlag){
			index=(int)strtoul(row.attribute("g").value(),NULL,0);
            aliKali->testLetter=index; //send data for testMask();
		//}
			//cout_<<"index="<<index<<END;
		
		GLetter *letter=aliKali->getLetter(index);
        letter->newLetter=1;
	    int maskId=atoi(row.attribute("id").value()); 
        GBitMask32 *mask;
        if(maskId>letter->mask32Count()){
            GBitMask32 mask_; letter->push_back(mask_);
            mask=&letter->mask32[letter->mask32Count()-1];
            maskId=letter->mask32Count()-1;
        }else{mask=&letter->mask32[maskId];}
	
	    //DR("add "<<letter->name.c_str()<<" letter->mask32Count()="<<letter->mask32Count()<<" maskId="<<maskId<<END)
		
		x0=atoi(row.attribute("x0").value());
		x1=atoi(row.attribute("x1").value());     
		y0=atoi(row.attribute("y0").value());
		y1=atoi(row.attribute("y1").value());
		dlt0=atoi(row.attribute("d").value());
        if(dlt0>28||dlt0<8)dlt0=28;
      
        DR("new letter->mask32Count()="<<letter->mask32Count()<<" maskId="<<maskId<<END)
		w=x1-x0;  
		h=y1-y0;  
        
      w=w/2; w=w*2+1;
      h=h/2; h=h*2;  // приводим к четным значениям
      if(inputData.data["ocrData"]!="eraseMaskContent"){
          if(w>32)w=32;  if(h>128)h=128;
       }
      
      DR("x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<" w="<<w<<" h="<<h<<" maskId="<<maskId<<END)
      mask->xMask=x0-PICT_SIZE/2;
      //cout_<<"index="<<index<<" index1="<<(index-((int)index/10)*PICT_SIZE)*PICT_SIZE<<END;
      mask->yMask=y0-PICT_SIZE/2;
      mask->imgW=w;
      mask->mH=h;
      
      if(inputData.data["ocrData"]=="eraseMaskContent"){
         //cout_<<"eraseMaskContent"<<endl;
         letter->eraseMaskContent(maskId);
         letter->setLetterDimensions();
         aliKali->saveLetter(letter);
         letter->destroy();
         return;
      }  
      
      GBitmap *img32=GBitmap::createRegion(outBitmap,x0,y0,w,h);
      
      
             
      //static int count=0;
      //string str="/1.jpg";
      //ostringstream out; out<<"/_1_"<<count<<".jpg"; count++;
      //str=out.str();
      //WriteImageData(img32,"/1.jpg",1); //exit(0);
     // WriteImageData(img32,str,1);

                  mask->maskType=1;
                  mask->dlt0=(w+h)/3;
                  if(w+h<30)dlt0=3;
                  mask->dlt0=dlt0;
                  mask->dlt1=1;
                  if((w+h)<20)mask->dlt1=0;
                  mask->dlt2=(w+h)/20;  
                  //mask->dlt2=2;
				  mask->packBitMask(img32);
				  if(mask->NMask==0){
                      DR("empty mask"<<endl); //exit(0)
                      letter->eraseMask(maskId); 
                  }  //erase empty mask
			 //mask->printMask();
			 mask->status=3;
      
             DR("letter->mask32["<<maskId<<"].NMask="<<mask->NMask<<" letter->mask32["<<maskId<<"].yMask="<<mask->yMask<<" status="<<mask->status<<endl)
	         
        img32->destroy();
			 //out.close();
			 //str=readText("mask.html");
			 //cout_<<"<div style=\"font-family:Courier;\">";
			 //cout_<<str<<"</div>";exit(0);
      aliKali->saveLetter(letter);
      letter->destroy();
      
	     
  }

	 
    if(inputData.data["ocrData"]=="setMask")return;
    
    //test self correlation
	
    GLetter *glyph=aliKali->getLetter(index);
    GLetter *letter=aliKali->getLetter(index);
    GBitmap *img=glyph->mask128.unPackMask();
    //Типичный способ получения полиграфического растискивания двумя запусками dotGain
    //убираем со всех сторон по пикселю, сьедаем мелкие детали, которые не надо растискивать
    img->dotGain(-1,-1,-1,-1, 0);
    //добавляем со всех сторон символа по нескольку пикселей
    img->dotGain( 2, 2, 2, 2, 0);
    //string str_="/_2.jpg";
    //WriteImageData(img,"/1.jpg",1); //exit(0);###
    GBitsetMatrix* setMatrix=GBitsetMatrix::createRegion(img,
                                                         0,
                                                         0,
                                                         img->columns(),
                                                         img->rows(),
                                                         1,1,
                                                         IMGNOFLIP);
    
    //str_="/_3.jpg";
    //WriteImageData(setMatrix->drawDataRGB,str_,1)
        
        letter->selfCorrelation=setMatrix->letterToLetterCorrelation(glyph,
                                                                    0,
                                                                    img->rows(),
                                                                    0,
                                                                    0);
                //str_="/_31.jpg";
                //WriteImageData(setMatrix->drawDataRGB,str_,1); exit(0);
                
    for(int a=0;a<glyph->mask32Count();a++){
		letter->mask32[a].correlation=glyph->mask32[a].correlation;
        cout_<<" letter->mask32["<<a<<"].correlation="<<letter->mask32[a].correlation<<endl;
        if(letter->mask32[a].correlation>100||letter->mask32[a].correlation<1)letter->mask32[a].correlation=0;
	}
	
	img->destroy();
    glyph->destroy();
	setMatrix->destroy();
    //подсчитываем площадь буквы покрытой масками
    letter->drawLetterPict(1);
    //letter->setLetterDimensions();
    aliKali->saveLetter(letter);
    letter->destroy();


}//_____________________________________________________________________________





