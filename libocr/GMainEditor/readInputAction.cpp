#include "GMainEditor.h"
#include "../GImageEditorOCR/GImageEditorOCR.h"

string GMainEditor::readInputAction(){
	string destLine;
    fontEditor=(GFontEditor*)inputData.fontEditor;
    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
    logicProcessor->readDictionary();
    aliKali=fontEditor->aliKali;
    destLine="Start";
    string ocrData=inputData.data["ocrData"];
    
	int print=1;
        
	DT("REZULT inputData.data[ocrData]="<<ocrData<<endl);
	
	if(ocrData=="setBase"){
		cout_<<"INPUT set base"<<inputData.data["setBase"]<<endl;
		fontEditor->setBase();
	}
    if(ocrData=="eraseMaskContent"){
		cout_<<"INPUT eraseMaskContent"<<endl;
		fontEditor->setBase();
	}
    if(ocrData=="eraseAllMask"){
		cout_<<"INPUT eraseMaskContent"<<endl;
		fontEditor->setBase();
	}
	if(ocrData=="setLetter"){
		cout_<<"INPUT set letter"<<inputData.data["setLetter"]<<endl;
		fontEditor->setLetter();
	}
    if(ocrData=="imageNormalisation"){
		cout_<<"INPUT imageNormalisation"<<endl;
        uint index=atoi(inputData.data["index"].c_str());
        GLetter *letter=aliKali->getLetter(index);
        //#imageNormalisation((void*)&letter);
        aliKali->saveLetter(letter);
        letter->destroy();
	}
	if(ocrData=="saveBase"){
		fontEditor->aliKali->writeGFontXML();
	}
    
	if(ocrData=="lineOCR"){
		//lineOCR();
        startOCRSystem();
        destLine="\n@@Out@@\n<script>location.href=\"http://localhost/ocr/OCR/_OCR/_line.html\";</script>";
	}
    
	if(ocrData=="batchOCR"){
        readDirectoryToArray(inputData.fileList,inputData.fileList[0],"img");     
        cout<<"inputData.fileList.size()="<<inputData.fileList.size()<<endl;

        //cout_<<" inputData.fileList="<<inputData.fileList.size()<<" first file="<<inputData.fileList[0]<<endl;
        //startOCRSystem();
        startOCRBatch();
        
		destLine="Start OCR";
	}
    if(ocrData=="vectorOCR"){
        readDirectoryToArray(inputData.fileList,inputData.fileList[0],"img");
        cout<<"inputData.fileList.size()="<<inputData.fileList.size()<<endl;
        
        //cout_<<" inputData.fileList="<<inputData.fileList.size()<<" first file="<<inputData.fileList[0]<<endl;
        vectorOCR();
        destLine="Start OCR";
    }
    if(ocrData=="buildOCRFont"){
		readDirectoryToArray(inputData.fileList,inputData.fileList[0],"img");       
		buildOCRFont();
		destLine="Start OCR";
	}
	if(ocrData=="letterBlock"){
        
		startOCRSystem();
		destLine="Start OCR";
	}
    
    if(ocrData=="nextOCRLetter"||
       ocrData=="previousOCRLetter"||
       ocrData=="currentOCRLine"||
       ocrData=="currentOCRLetter"){       
            string str=inputData.data["root"]+"edit/OCR/_OCR/";
            vector<string>fileList;
            readDirectoryToArray(fileList,str,"img");
            string path=inputData.data["root"]+"edit/OCR/input.txt";
            str="";
            readText(str,path);
            if(str=="")str="0";
            int startIndex=atoi(str.c_str());
            if(ocrData=="nextOCRLetter")startIndex++;
            if(ocrData=="previousOCRLetter")startIndex--;
            if(startIndex<0)startIndex=(uint)fileList.size()-1;
            if(startIndex>(uint)fileList.size()-1)startIndex=0;
            ostringstream out;
            out<<startIndex; str=out.str();
            writeText(str,path);
            cout_<<"@fileList[0]="<<fileList[startIndex]<<endl;
            str=substr((uint)fileList[0].rfind("/"),(uint)fileList[startIndex].size(),fileList[startIndex]);
            str=str_replace(".tif", ".html", str);
            destLine="\n@@Out@@\n/OCR/_OCR/"+str;
            return destLine;
	}
    
    if(ocrData=="testOCRLine"){
            string path=inputData.data["root"]+"edit/OCR/_OCR/";
            vector<string>fileList;
            readDirectoryToArray(fileList,path,"img");
            string path_=inputData.data["root"]+"edit/OCR/input.txt";
            string str="";
            readText(str,path_);
            if(str=="")str="0";
            int startIndex=atoi(str.c_str());
            if(startIndex>=fileList.size())startIndex=0;
            cout_<<"@1fileList[0]="<<fileList[startIndex]<<endl;
            str=substr((uint)fileList[0].rfind("/"),(uint)fileList[startIndex].size(),fileList[startIndex]);
            inputData.data["inputFile"]=fileList[startIndex];
            GBitmap *img=GBitmap::create(inputData.data["inputFile"].c_str());
            startOCR(img);
            img->destroy();
            fileList.resize(0);
            readDirectoryToArray(fileList,path,"img");
            str=substr((uint)fileList[0].rfind("/"),(uint)fileList[startIndex].size(),fileList[startIndex]);
            str=str_replace(".tif", ".html", str);
            destLine="\n@@Out@@\n/OCR/_OCR/"+str;
            return destLine;
    }

	if(ocrData=="exportOCR"){
		exportOCR();
		destLine="Start OCR";
	}
    
    if(ocrData=="testMask"){
		cout_<<"INPUT set base"<<inputData.data["setBase"]<<endl;
        fontEditor->testMask();
	}
    
    if(ocrData=="rebuildBase"){  //###
		//cout_<<"INPUT rebuildMask in base"<<endl;
		for(int index=0;index<aliKali->letterCount();index++){
			//aliKali[0][index]->selfCorrelation=0; aliKali[0][index]->codeSpace=0; 
			//aliKali[0][index]->lineCount=0; 
            //aliKali[0][index]->OCRStatus=1;
			//aliKali[0][index]->recordStatus=1;
		}
		fontEditor->rebuildMask();
		//cout_<<"test done"<<endl;
        
	}
    
    
    if(ocrData=="rebuildMaskInLetter"){
		//cout_<<"INPUT rebuildMaskInLetter In Letter"<<endl;
        fontEditor->setBase();
        int pfds[2];
        
        /*for(int in=0; in<fontEditor->aliKali->letterCount();in++){  //DT(" in="<<in<<" name"<<aliKali[0][in]->name.c_str());
            if(fontEditor->aliKali[0][in]->pict!=0||fontEditor->aliKali[0][in]->name=="*")continue;
            fontEditor->aliKali[0][in]->pict=fontEditor->aliKali->drawPict(in);
            cout<<"pictW="<<fontEditor->aliKali[0][in]->pict->columns()<<" pictH="<<fontEditor->aliKali[0][in]->pict->rows()<<" in="<<in<<endl;
        }*/ 
        

        
        int forkMode=0;        
        if(forkMode){
            print =1;
                        
            //cout_<<"start fork";
            int pidID = fork();
            
            if (pidID < 0)
                error((char*)"ERROR on fork");
            if (pidID == 0)  {
                GLetter *letter=aliKali->getLetter(aliKali->testLetter);
                fontEditor->rebuildMaskInLetterVector(letter,pfds);
                inputData.data["action"]="setFocalPoints";
                //#imageNormalisation((void*)&letter);
                aliKali->saveLetter(letter);
                letter->destroy();
                exit(0);
            }else{
            }
        }else{
            GLetter *letter=aliKali->getLetter(aliKali->testLetter);
            fontEditor->rebuildMaskInLetterVector(letter,pfds);
            inputData.data["action"]="setFocalPoints";
            //#imageNormalisation((void*)&letter);
            aliKali->saveLetter(letter);
            letter->destroy();
        }
        
        
        
    }

    if(ocrData=="setMaskFromBase"){
		cout_<<"INPUT setMaskFromBase in base"<<endl;
		fontEditor->setMaskFromBase();
	}
    
	if(ocrData=="deleteLetters"){
		DT("INPUT deleteLetters"<<inputData.data["setBase"]<<endl);
		fontEditor->setBase();
	}
	if(ocrData=="testBase"){
        //for(int index=0;index<aliKali->letterCount();index++){
            //aliKali[0][index]->selfCorrelation=0; 
            //aliKali[0][index]->codeSpace=0; 
            //aliKali[0][index]->lineCount=0; aliKali[0][index]->OCRKey="";  
        //    aliKali[0][index]->recordStatus=1;
       /// }
     	fontEditor->testBase();
		cout_<<"test done"<<endl;
        
	}
    
	return destLine;
	
}//___________________________________________________________________________

