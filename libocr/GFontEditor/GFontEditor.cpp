#include "GFontEditor.h"
#include "../GMemory/GMemory.h"
#include "../GLogicProcessor/GLogicProcessor.h"
#include "../GGraph/GGraph.h"
//#include "../../re2/re2.h"

//________________GFontEditor____________________
GFontEditor::GFontEditor(void){
    GFontCount=0;
    readGFonts();
}


GFont*  GFontEditor::operator[](int index){
	if (index<0 || index>=GFontCount) {
		return fontBase[0];
	}
	return fontBase[index];
}//_______________________________________________	


void GFontEditor::readGFonts(){ 
	///////////////////////////////////*read letter data
    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
    aliKali=GFont::create();
    if(aliKali->size()<2){
        vector<string>fileList;
        readDirectoryToArray(fileList, inputData.data["OCRTables"], "xml");  
        cout<<" fontEditor->readGFonts"<<endl;        
        cout<<"fileList.size()="<<fileList.size()<<" "<<endl;
        
        for(int fileIndex=0;fileIndex<fileList.size();fileIndex++){
            string file=fileName(fileList[fileIndex]); 
            cout<<"load GFont - "<<file<<endl;
            inputData.data["GFontPath"]=fileList[fileIndex];
            aliKali->readGFontXML();
        }    
    }
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    string str="OCR_FONT";
    longMemory->loadTable(str);
    font=(GFont*)longMemory->table[str].data;
    str="OCR_IMAGE";
    longMemory->loadTable(str);
    imageData=longMemory->table[str].data;
    str="OCR_STAR";
    longMemory->loadTable(str);
    starData=longMemory->table[str].data;

    
 
}

int GFontEditor::maskCorrelation(GBitMask32 &mask1,GBitMask32&mask2){
    int correlation=0;
    
    
    return correlation;
}

/// import OCR letter data from GLetter format
void GFontEditor::importLetter(GLetter *letter){
    
    OCRLetter newLetter;
    GBitmap *img,*imgMask;
    TString st;
    string data;
    
    img=GBitmap::create(letter->mask128.mH,letter->mask128.mH+16);
/*
    //original image

    GBitmap *imgMask=letter->maskOriginal.unPackMask();
    img->drawImg(imgMask, 8+letter->dX, 8+letter->dY);
    imgMask->destroy();
    st.push_back(letter->name);
    img->strPNG(data);
    //st.push_back(data);
*/
    //OCR image
    img->fill(255);
    imgMask=letter->mask128.unPackMask();
    img->drawImg(imgMask, 8+letter->dX, 8+letter->dY);
    imgMask->destroy();
    st.push_back("ocr");
    img->strPNG(data);
    st.push_back(data);
    //newLetter.imgIndex=(uint)imageData->size();

    //imageData->push_back(st);
    
    //build OCRStar
    
    GGraph graph(img);
    
    graph.graphFocalLine(15);
    
    //img->bytes_data=graph.bytes_data;
    //img->savePNG("/_Image2OCR/__1.png");

    graph.focalPointXTL();
    //graph.printToScreenBinary();
    //cout<<" focalPoint.size():"<<graph.focalPoint.size()<<endl;
    graph.focalLineRecognition();
    //cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;
    graph.findExtremPoints();
    //cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;
    
    graph.buildOCRStars();
    //graph.printStars("/_Image2OCR/__2Draw_text.html",2);
    //graph.printToScreenBinary();

    
    
    OCRLetter letterNew;
    graph.buildLetter(letterNew);
    
    //graph.printLetter(letterNew, "/_Image2OCR/_2Draw_letter.html",  2);
    
    //graph.focalLineCorrelation(letter);
    

    

}


//import OCR letters from aliKali to OCRFont
void GFontEditor::importOCRLetters(){
    for(int i=0;i<aliKali->size();i++){
        
        TString st;
        aliKali->getTStr(i,st);
        //if(l->mask32Count()>0&&l->name!="*"){
        //    //OCRFont->push_back(l);
        //}else{
        //    cout<<" no valid letter "<<i;
        //    l->destroy(); continue;
        //}
        //l->destroy();
        //cout<<" "<<i;

    
    }


}

///чтение XML данных GFont из path to database
string GFontEditor::importGFontsDB(string &path){

    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
	cout<<" fontEditor->readGFonts XML"<<endl;
   
        aliKali=GFont::create();
        cout<<"load GFont - "<<path<<endl;
        inputData.data["GFontPath"]=path;
        aliKali->readGFontXML();
return "";
}
//////////////////////////////////////////////////////////////////////////////////////////////////


///запись данных базы данных GFont в XML
string  GFontEditor::exportGFontsDB(string &path){
    
    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
	cout<<" fontEditor->exportGFonts"<<endl;
    inputData.data["GFontPath"]=path;
    inputData.argc=EXPORT_FROM_DB;
    aliKali->writeGFontXML();
    return "";
}
//////////////////////////////////////////////////////////////////////////////////////////////////


///запись данных базы данных GFont в PNG
void GFontEditor::exportGFontsImages(cstr path){
    
    string date=run("date \"+%s\"");
    int tm_start=atoi(date.c_str());                                                                                          \
    string str;
    
    for(int i=0;i<aliKali->size();i++){
        GLetter *letter = aliKali->getLetter(i);
        GBitmap *letterImg=letter->mask128.unPackMask();
        letterImg->colorModeOut=1;
        ostringstream out;
        out<<path<<"/_"<<letter->name<<"_"<<tm_start<<".png";
        str=out.str();
        cout<<i<<" -> "<<str<<endl;
        letterImg->savePNG(str);
        letterImg->destroy();
        letter->destroy();
        tm_start++;
    }
}

///переупаковка данных шрифта
void GFontEditor::reloadGFont(){
    string path=inputData.data["OCRTables"]+"GFont.xml";
    cout_<<"pathXML="<<path<<endl;
    GFont *aliKali_=GFont::create("/NewFont.bin");
    for(ulong i=0;i<FSIZE*FSIZE;i++){
        aliKali_->cMatrix->put(i,aliKali->cMatrix[0][i]);
    }
    cout<<"done matrix"<<endl;
    for(int i=0;i<aliKali->letterCount()-1;i++){
        GLetter *letter;
        letter=aliKali->getLetter(i);
        if(letter==0)continue;
        if(letter->name=="*")continue;
        aliKali_->push_back(letter);
    }
    cout<<"done reload font";

}

///установка в шрифте флага языка распознавания
void GFontEditor::setLanguage(){
    ulong *p=aliKali->OCRLanguage;   //cout<<inputData.data["ocrLn"]<<"*p="<<*p<<" OCR_TIBETAN="<<OCR_TIBETAN<<endl;
    //inputData.data["ocrLn"]="ocrTibetan";
    string path=inputData.data["grammar"]+"_alphabetTibetan.txt";
    string alphabet;
    readText(alphabet,path);
    if(inputData.data["ocrLn"]=="ocrTibetan"){ //cout<<"tibetan";
         cout<<"set tibetan in OCR"<<endl;
         *p=OCR_TIBETAN;
         int count=0;
         //TIME_START
        
        cout<<"aliKali->letterCount():"<<aliKali->letterCount()<<endl;
         for(int i=1;i<aliKali->letterCount();i++){
               GLetter *glyph=aliKali->getLetter(i);   //cout<<glyph->name;
               if(alphabet.find(glyph->name)!=-1){
                   glyph->recordStatus=1;
                   count++;
               }else{
                   glyph->recordStatus=0;
               }
               aliKali->saveLetter(glyph);
               glyph->destroy();
         }
         //TIME_PRINT_
         cout<<"count="<<count<<endl;
     }

     if(inputData.data["ocrLn"]=="ocrTibetanEnglish"){
         //if(*p!=OCR_TIBETAN_ENG){  cout<<"no OCR_TIBETAN_ENG";
             *p=OCR_TIBETAN_ENG;
             string path=inputData.data["grammar"]+"_alphabetTibetanEnglish.txt";
             string alphabet;
             readText(alphabet,path);
             
             for(int i=1;i<aliKali->letterCount();i++){
                 GLetter *glyph=aliKali->getLetter(i);   //cout<<glyph->name;
                 if(alphabet.find(glyph->name)!=-1){
                     glyph->recordStatus=1;
                 }else{
                     glyph->recordStatus=0;
                 }
                 aliKali->saveLetter(glyph);
                 glyph->destroy();
                 
             }
        // }
     }
     if(inputData.data["ocrLn"]=="ocrTibetanRussian"){
             //if(*p!=OCR_TIBETAN_RUS){
                 *p=OCR_TIBETAN_RUS;
                 for(int i=0;i<aliKali->letterCount();i++){
                     GLetter *glyph=aliKali->getLetter(i);
                     //if(!RE2::PartialMatch(glyph->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")
                     //   &&!RE2::PartialMatch(glyph->name,"[а-яА-ЯёЁ]+")){
                     //    glyph->recordStatus=0;
                     //}else{
                         glyph->recordStatus=1;
                     //}
                     aliKali->saveLetter(glyph);
                     glyph->destroy();
                     
                 }    
             //}
     } 
     if(inputData.data["ocrLn"]=="ocrTibetanSanskrit"){
             //if(*p!=OCR_TIBETAN_SKT){
                 *p=OCR_TIBETAN_SKT;
                 for(int i=0;i<aliKali->letterCount();i++){
                     GLetter *glyph=aliKali->getLetter(i);
                    // if(RE2::PartialMatch(glyph->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")||RE2::PartialMatch(glyph->name,"[\\p{Devanagari}]+")){
                         glyph->recordStatus=1;
                      //   if(RE2::PartialMatch(glyph->name,"[\\p{Tibetan}]+")){
                             if(alphabet.find(glyph->name)!=-1){
                                 glyph->recordStatus=1;
                             }else{
                                 glyph->recordStatus=0;
                             }
                         //}
                         //if(RE2::PartialMatch(glyph->name,"[\\p{Devanagari}]+")){
                         //    glyph->OCRKey="S";
                        // }
                         //if(RE2::PartialMatch(glyph->name,"[\\p{Cyrillic}]+")){ //ошибка в RE
                         //   glyph->recordStatus=0;
                         //}

                     //}
                         if(glyph->recordStatus!=0)cout<<" "<<glyph->name;

                         aliKali->saveLetter(glyph);
                         glyph->destroy();
                     
                 }    
             //}
     }    
     if(inputData.data["ocrLn"]=="ocrSanskrit"){
            // if(*p!=OCR_SANSKRIT){
            cout<<"set Sanskrit OCR";
                 *p=OCR_SANSKRIT;
                 for(int i=0;i<aliKali->letterCount();i++){
                     GLetter *glyph=aliKali->getLetter(i);
                     //if(!RE2::PartialMatch(glyph->name,"[\\p{Devanagari}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")){
                     //    glyph->recordStatus=0;
                     //}else{
                     //    glyph->recordStatus=1;
                    // }
                     aliKali->saveLetter(glyph);
                     glyph->destroy();
                     
                 }    
            // }
     }    
     if(inputData.data["ocrLn"]=="ocrKannada"){
             //if(*p!=OCR_KANNADA){
                 *p=OCR_KANNADA;
                 for(int i=0;i<aliKali->letterCount();i++){
                     GLetter *glyph=aliKali->getLetter(i);
                     //if(!RE2::PartialMatch(glyph->name,"[ಂ-೯\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")){
                     //    glyph->recordStatus=0;
                     //}else{
                     //    glyph->recordStatus=1;
                     //}
                     aliKali->saveLetter(glyph);
                     glyph->destroy();
                     
                 }    
             //}
     }    

}

void GFontEditor::setLettersProbability(){       ///< set OCRFont letters probability
    
    map<string, uniRecord> &mainLetterTableUni=((GLogicProcessor*)inputData.logicProcessor)->mainLetterTableUni;
    for(int i=0;i<aliKali->letterCount();i++){
        GLetter *letter=aliKali->getLetter(i);
        string str;
        if(mainLetterTableUni.find(letter->name)!=mainLetterTableUni.end()){
            str=mainLetterTableUni[letter->name].lang;
            if(str!="tib"&&str!="skt"){
                letter->allLetterCount=0;
                continue;
            }
            letter->allLetterCount=mainLetterTableUni[letter->name].OCRCount;
        }else{
            letter->allLetterCount=0;
        }
        aliKali->saveLetter(letter);
        letter->destroy();
        
    }
}

/// генерация фалов базы в формате Tesseract.
void GFontEditor::buildTesseractData(){
	string str;
	int w=180;
	int h=100;
    int tH=h*40+20;
    int tW=w*20;
    GBitmap *tableImg=GBitmap::create(tW,tH);
    ostringstream out;
    ostringstream box;
    str="<html><body  style=\"padding-left:0px; padding-top:0px\">";
    str+="<img src=\"OCRBase.jpg\" style=\"position:absolute;top:0;left:0\">";
    out<<str;
   
    string pathLetters="/_LETTERS";
    string ext="img";
    vector<string>fileList;
    readDirectoryToArray(fileList, pathLetters, ext);
    
	int step=0,y=0;
    //TIME_START
    cout_<<"@@@letterCount="<<fileList.size();
	for(int i=0;i<fileList.size();i++){
        string name=str_replace("/_LETTERS/","",fileList[i]);
        name=str_replace(".jpg","",name);
        
        cout<<"path="<<fileList[i]<<" i="<<i<<endl;
		GBitmap *letterImg=GBitmap::create(fileList[i].c_str());
        //GBitmap *letterImg=letter->mask128.unPackMask();
		tableImg->drawImg(letterImg,w*step+17,y+15);
        out<<"<div style=\"position:absolute; top:"<<y+18<<";left:"<<w*step+20
        <<"; width:"<<letterImg->columns()-6<<"; height:"<<letterImg->rows()-6<<"; border:1px solid blue\">"<<name<<"</div>";
        //left bottom right top 0.0 left bottom
        box<<name<<" "<<w*step+20<<" "<<tH-(y+18)-(letterImg->rows()-6)<<" ";
        box<<w*step+20+letterImg->columns()-6<<" "<<tH-(y+18)<<"\n";
		step++;
		if(step==20){
			//tableImg->drawImg(blank,w*step-40,y-90);
			y+=h;step=0;
		}
		letterImg->destroy();
	}
    string path=inputData.data["root"]+"edit/OCRBase.png";
    tableImg->savePNG(path);
    tableImg->destroy();
    path=inputData.data["root"]+"edit/OCRBase.html";
    string data=out.str();
    writeText(data, path);
    path=inputData.data["root"]+"edit/OCRBase.txt";
    data=box.str();
    writeText(data, path);
}


void GFontEditor::setOCRKey(){
    
     //inputData.data["action"]="set focal points";
     //cout<<sizeof(GLetter); exit(0);
     logicProcessor->readDictionary();
     
     string name;
     int print =0;
     for(int i=1;i<aliKali->size()-1;i++){ //cout<<i<<".";
             GLetter *letter=aliKali->getLetter(i);
             //GLetter *letter=GLetter::create();
             name=letter->name;
             //if(letter->OCRKey=="X"){ letter->name="*"; letter->recordStatus=0; aliKali->saveLetter(letter); letter->destroy(); continue;}
             //imageEditor->imageNormalisation(letter);   
             uint index=(uint)logicProcessor->fontGMap->getKey(name, FULL_MATCH);
             ///DR("line["<<i<<"].name="<<name<<"/ index="<<index<<endl)
             //if(letter->letterW>0){ letter->destroy(); continue;}
             //cout<<" i="<<i<<" "<<letter->name<<" "<<letter->OCRKey<<" "<<letter->OCRIndex<<endl;
             //letter->name="*"; aliKali->saveLetter(letter);
             
             //letter->mask128.printMask();
             //break;
             if(index==-1){
                 DR("NO LETTER"<<name<<endl)letter->OCRKey="";
             }else{
                 TString strT; logicProcessor->fontTable->getTStr(index,strT);
                 DR("name="<<strT[8]<<" OCRIndex = "<<strT[4]<<endl)
                 letter->OCRKey=strT[4];
             }    
             aliKali->saveLetter(letter);
             letter->destroy();
     }
     cout<<"done";

}

//переписывает базу в новый файл, убирает стертые и поврежденные буквы.
void GFontEditor::rebuildGFont(){
    string path="_newFont.bin";
    
    GFont *newFont=GFont::create(path.c_str());
    for(int i=0;i<aliKali->size();i++){
        GLetter *l;
        l=aliKali->getLetter(i);
        if(l->mask32Count()>0&&l->name!="*"){
            newFont->push_back(l);
        }else{
            cout<<" no valid letter "<<i;
            l->destroy(); continue;
        }
        l->destroy();
        cout<<" "<<i;
    }
    

}

