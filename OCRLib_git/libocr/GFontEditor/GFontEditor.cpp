#include "GFontEditor.h"

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
    imageEditor=(GImageEditor*)inputData.imageEditor;
    aliKali=GFont::create();

    if(aliKali->size()==1){
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
 
}
//////////////////////////////////////////////////////////////////////////////////////////////////

///чтение XML данных GFont из path to database
string GFontEditor::importGFontsDB(string &path){

    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
    imageEditor=(GImageEditor*)inputData.imageEditor;
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
    imageEditor=(GImageEditor*)inputData.imageEditor;
	cout<<" fontEditor->exportGFonts"<<endl;
    inputData.data["GFontPath"]=path;
    inputData.argc=EXPORT_FROM_DB;
    aliKali->writeGFontXML();
    return "";
}
//////////////////////////////////////////////////////////////////////////////////////////////////

///переупаковка данных шрифта
void GFontEditor::reloadGFont(){
    string path=inputData.data["OCRTables"]+"GFont.xml";
    cout_<<"pathXML="<<path<<endl;
    GFont *aliKali_=GFont::create("/NewFont.bin");
    for(int i=0;i<FSIZE*FSIZE;i++){
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
                     if(!RE2::PartialMatch(glyph->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")
                        &&!RE2::PartialMatch(glyph->name,"[а-яА-ЯёЁ]+")){
                         glyph->recordStatus=0;
                     }else{
                         glyph->recordStatus=1;
                     }
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
                     if(RE2::PartialMatch(glyph->name,"[\\p{Tibetan}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")||RE2::PartialMatch(glyph->name,"[\\p{Devanagari}]+")){
                         glyph->recordStatus=1;
                         if(RE2::PartialMatch(glyph->name,"[\\p{Tibetan}]+")){
                             if(alphabet.find(glyph->name)!=-1){
                                 glyph->recordStatus=1;
                             }else{
                                 glyph->recordStatus=0;
                             }
                         }
                         if(RE2::PartialMatch(glyph->name,"[\\p{Devanagari}]+")){
                             glyph->OCRKey="S";
                         }
                         if(RE2::PartialMatch(glyph->name,"[\\p{Cyrillic}]+")){ //ошибка в RE
                            glyph->recordStatus=0;
                         }

                     }
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
                     if(!RE2::PartialMatch(glyph->name,"[\\p{Devanagari}\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")){
                         glyph->recordStatus=0;
                     }else{
                         glyph->recordStatus=1;
                     }
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
                     if(!RE2::PartialMatch(glyph->name,"[ಂ-೯\\d\\]\\[«»［］｛｝—⊕\\|\\(\\)\\-\\.,;:\\!\\?=IVX]+")){
                         glyph->recordStatus=0;
                     }else{
                         glyph->recordStatus=1;
                     }
                     aliKali->saveLetter(glyph);
                     glyph->destroy();
                     
                 }    
             //}
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
		GBitmap *letterImg=LoadImageData(fileList[i], 0);
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
    string path=inputData.data["root"]+"edit/OCRBase.jpg";
    ((GImageEditor*)inputData.imageEditor)->WriteImageData(tableImg,path,0);
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
             int index=logicProcessor->fontGMap->getKey(name, FULL_MATCH);
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

