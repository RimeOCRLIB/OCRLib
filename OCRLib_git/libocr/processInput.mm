///C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#include "php2stl.h"
#include "GMainEditor.h"
#include "GImageEditor.h"
#include "GFontEditor.h"
#include "GLogicProcessor/GLogicProcessor.h"
#include "GMemory.h"
#include "GPDF.h"

///основной перекресток управления программой через интерфейс
///состояние интерфейса записанное в inputData в процессе обработки HTTP запроса браузера или из командной строки интерпретируеся, 
///вызывая создание соответсвующих объектов и выполнение методов
///inputData также содержит указатели на глобальные обьекты.
///processInput возвращает строку результирующих данных которая передается как ответ программы на запрос браузера или другого интерфейса.

void processInput(string &destString){
	int print=1;
  //TIME_START
    inputData.data["HTTPType"]="text/html";
    string ocrData=inputData.data["ocrData"];

    
    if(ocrData=="imageFromPDF"){
#ifdef COCOA
        for(int i=0;i<inputData.fileList.size();i++){
            cout_<<"sart PDF from"<<inputData.fileList[i]<<endl;
            GPDF *pdf=GPDF::create(inputData.fileList[i].c_str());
            pdf->writeAllImages();
            pdf->destroy();
        }
        destString="done export Images from PDF";
        return;
#endif
    }
    
    
    
//********************Init main OCR Core*****************************
        GMainEditor *mainEditor=(GMainEditor*)inputData.mainEditor;
		GLogicProcessor *logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
        GFontEditor *fontEditor=(GFontEditor*)inputData.fontEditor;
        GMemory *longMemory=(GMemory*)inputData.longMemory;

        if(ocrData=="main"){
            string str;
            /*string path=inputData.data["siteData"]+"index.html";
            str=readText(path);
            inputData.data["db"]="lib";
            inputData.data["draw"]="head";
            longMemory->memoryRequest(destString);
            str=str_replace("@@2@@", destString, str);
            destString="";
            inputData.data["draw"]="menu";
            longMemory->memoryRequest(destString);
            str=str_replace("@@Menu@@", destString, str);
            cout<<str;
            */
            remove(inputData.data["log"].c_str());
            exit(0);
            
        }
        if(inputData.data["ocrData"]=="chat"){
            if(inputData.data["user_input"].find("bookID")!=-1){
                inputData.data["db"]="lib";
                inputData.data["record"]="find";
                string str=inputData.data["user_input"];
                str=str_replace(" ", "", str);
                inputData.data["c0"]=str;
                inputData.data["ID"]=str;
                
            }else{
                //aiml::cAngel *angel=aiml::cAngel::create();
                //destString=angel->angel(inputData.data["user_input"]);
                return;
            }
        }
    
        if(inputData.data["db"]!=""){
            if(inputData.data["mode"]=="TEXT"){  //запрос на словарный разбор
                destString=inputData.data["text"];
                logicProcessor->lineTextTranslation(destString);
                print(destString);
                remove(inputData.data["log"].c_str());
                exit(0);
            }
            if(inputData.data["text"].find("/Volumes")!=-1){  //запрос на словарный разбор текста
                destString=inputData.data["text"];
                logicProcessor->mainTextTranslation(destString);
                print(destString);
                remove(inputData.data["log"].c_str());
                exit(0);
            }

            longMemory->memoryRequest(destString);
            if(destString=="")destString="done";
            return;
        }
      
       
//*************************OCR enviroment***********************
    
    if(ocrData=="edit"){
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in)+"done";
        return; 
	}
	if(ocrData=="pict"){
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
        destString=fontEditor->drawLetter(in);
		inputData.data["HTTPType"]="image/jpeg";
        return; 
	}
	if(ocrData=="draw"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
	}  
    
	if(ocrData=="eraseAllMask"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
	}  
	if(ocrData=="eraseMaskContent"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
        destString=fontEditor->drawEditLetter(in);
        return; 
	} 
	if(ocrData=="rebuildMaskInLetter"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
	} 
    if(ocrData=="rebuildBase"){
        mainEditor->readInputAction();
        return; 
	} 
	if(ocrData=="setMaskFromBase"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
	} 
	if(ocrData=="setMasksInBase"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
	}
    if(ocrData=="imageNormalisation"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return;
	}
	if(ocrData=="testLetter"){
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
        GLetter *letter=fontEditor->aliKali->getLetter(in);
        fontEditor->testLetter(letter,7);
        cout<<"keySize1="<<letter->keyOCRVector->size()<<endl;
        fontEditor->aliKali->saveLetter(letter);
		destString=fontEditor->drawEditLetter(in);
        destString="1";
        letter->destroy();
        return; 
	}
    if(ocrData=="letterOCR"){
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
        GLetter *letter=fontEditor->aliKali->getLetter(in);
        string name; 
        int correlation;
        fontEditor->letterOCR(letter,name, &correlation,0);
        letter->destroy();
        destString="\n@@Out@@\n<script>location.href=\"1.html\";</script>";
        return; 
	}
    if(ocrData=="copyLetter"){
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
        GLetter *letter=fontEditor->aliKali->getLetter(in);
        fontEditor->aliKali->push_back(letter);
        letter->destroy();
        destString=fontEditor->drawEditLetter(fontEditor->aliKali->size()-2);
        return; 
	}
	
	if(ocrData=="saveBase"){
        mainEditor->readInputAction();
        inputData.data["HTTPType"]="text/html";
		destString="<script>alert(\"OCR Base saved\"); location.href=\"/?xml=<ocrData>edit<ocrData><index></index>\";</script>";
        return; 
	}
    if(ocrData=="deleteLetters"){
        mainEditor->readInputAction();
		destString="<script>location.href=\"/?xml=<ocrData>GFont<ocrData>\";</script>";
        return; 
	}
    
	if(ocrData=="openNewLetter"){
		destString=fontEditor->drawNewLetters();
        return; 
	}
	
	if(ocrData=="setLetter"){
		mainEditor->readInputAction();
        destString="<script>location.href=\"ocr.php?xml=<ocrData>openNewLetter</ocrData>\";</script>\n";
        return; 
	}
   
    if(ocrData=="setBase"){
        mainEditor->readInputAction();
		unsigned int in=(uint)strtoul (inputData.data["index"].c_str(),NULL,0);
		destString=fontEditor->drawEditLetter(in);
        return; 
    }
    
    // вызов функции определения характеристик процессора.
    if(inputData.num_cores==0)StartCpuInfo();
    
    if(ocrData=="OCRTraining"){
        if(inputData.data["fileList"]=="") {destString="Open file for OCR"; return;}
        DIR *dir;
        if( (dir=opendir(inputData.data["fileList"].c_str()))!=NULL){
             mainEditor->readInputAction();
             destString="\n@@Out@@\n";
        }else{
            if(is_file(inputData.data["fileList"])){
                string str_; readText(str_,inputData.data["fileList"]);
                string path=inputData.data["root"]+"edit/OCR/page.tif";
                writeText(str_,path);
                inputData.fileList[0]=path;
                ocrData="OCRTrainingOneFile";
                destString="\n@@Out@@\n";
                mainEditor->readInputAction();
            }else{
                destString="Open file for OCR";
            }    

        }
        return;
	}
    if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary();
    //cout<<" logicProcessor.dictionaryReady="<<logicProcessor->dictionaryReady<<" ocrData="<<ocrData<<endl;
    if(ocrData=="dataForOCRTraining"||
           ocrData=="batchOCR"||
           ocrData=="testOCRLine"||
           ocrData=="lineOCR"||
           ocrData=="buildOCRFont"||
           ocrData=="nextOCRLetter"||
           ocrData=="previousOCRLetter"||
           ocrData=="currentOCRLine"||
           ocrData=="currentOCRLetter"||
           ocrData=="exportOCR"||
           ocrData=="testBase"||
           ocrData=="imageProcess"){
           destString=mainEditor->readInputAction();
           return; 

    }
    
    if(ocrData=="UTF8"||
       ocrData=="RTFToYagpo"||
       ocrData=="WylieToYagpo"||  //Wylie transliteration to Yagpo
       ocrData=="dWylieToYagpo"||  //delimited ༼Wylie༽ transliteration converter
       ocrData=="dSinhalaASCIToYagpo"||  //delimited ༼Sinhala༽ transliteration converter
       ocrData=="SinhalaMettaToYagpo"||  //delimited ༼Sinhala༽ transliteration converter
       ocrData=="YagpoToUnicode"||  //Yagpo To Unicode converter
       ocrData=="UnicodeToYagpo"||  //Unicode To Yagpo converter
       ocrData=="YagpoToWylie"||   //YagpoToWylie converter
       ocrData=="transcription"||  //phonetic transcription of tibetan text
       ocrData=="tibTextCorrector"||
       ocrData=="textNormalisation"){
        if(inputData.fileList.size())inputData.data["inputFile"]=inputData.fileList[0];
        destString=logicProcessor->startConvert();
        return;
    }
    
    if(ocrData=="buildDict"||
       ocrData=="TibetanToRusTagger"||
       ocrData=="TibetanToEngTagger"||
       ocrData=="buildHashDict"||
       ocrData=="reloadDict"|| //UnicodeToYagpo converter
       ocrData=="buildDictFromText"){  //UnicodeToYagpo converter
        if(inputData.fileList.size())inputData.data["inputFile"]=inputData.fileList[0];
        inputData.data["InputMethod"]="fileList";
        destString=logicProcessor->startDictionary();
        return;
    }
    
    //разбор дискового файла словарным таггером
    if(ocrData=="TibetanToRusTagger"||ocrData=="TibetanToEngTagger"){
        inputData.data["db"]="dict";
        inputData.data["page"]="FULL_TEXT";
        if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary();
        destString=mainEditor->readInputAction();
        return;
    }

    if(ocrData=="TibetanDictSearch"){  //text editor functions
		ocrData="TibetanUTFToEng";
        inputData.data["dictReport"]="report";
        
        string str,path; 
        path=inputData.data["root"]+"edit/mainGUI_TextTranslation.html";
        readText(str,path);
        destString=logicProcessor->startDictionary();
        destString=str_replace("@@@",destString,str);
        //cout_<<"destString=(char*)"<<destString<<endl;
        return;
	}
   
   

}//_________________________________________________________________________________________________










