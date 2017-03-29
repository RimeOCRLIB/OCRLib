#include "GMainEditor.h"

void GMainEditor::startOneFileOCR(){

    pechaImg=LoadImageData(inputData.data["inputFile"],0); 
    cout<<"Start#1 "<<inputData.data["inputFile"]<<endl; 
    if(!pechaImg){cout<<"no open file"<<inputData.data["inputFile"]<<endl; return;}
	
    inputData.data["ocrData"]="oneStringOCR";
    mainString="";
    
    vector<stringOCR>strArray;  
    int border;
    string str;
    string xmlString;
    int print=1;
    
	DT("@4_1");
    //vectorBase[0].allVectorCount=0;
	border=0; 
    setBit=GBitset::createResize(pechaImg,1,1,1);
    DT("@4_2");
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
    DT("@4_3");
    border=setBit->border_size();
    DT("@4_4");
    setBit->destroy();
    
    //получили координаты строк. Создаем новый процесс для каждой строки
    vector<int> pidID(inputData.num_cores);
    int status,pid;
    
    for(int index=0;index<pidID.size();index++){
        pidID[index]=0;
    }
    for(int a=0;a<strArray.size();a++)strArray[a].selectFlag=0;
    
    int i=strArray.size()-1;
    int processCount=0;
    while(i>=0){
        cout<<"NEW string# "<<i<<endl;
        for(int index=0;index<pidID.size();index++){
            if(pidID[index]==0){
                strArray[i].selectFlag=1;
                processCount++;
                pidID[index] = fork();
                if (pidID[index] < 0)
                    error((char*)"ERROR on fork");
                if (pidID[index] == 0)  {
                    
                    GBitsetOCR *setOCR=GBitsetOCR::createResize(pechaImg,1,1);
                    //if(NewLetterOnlyBtn->Checked==true) {mode=NEWLETTERINBOOK;}else{mode=ALL_LETTER;}
                    
                    setOCR->setData(
                                    aliKali,
                                    strArray,
                                    correctionTable,
                                    logicProcessor,
                                    iLeft,
                                    iTop,
                                    border,
                                    ALL_LETTER);
                    mainString=setOCR->mainString;
                    xmlString=setOCR->xmlString;
                    //cout<<"mainString="<<mainString<<endl;
                    ostringstream out;
                    out<<inputData.data["root"]<<"edit/OCR/_DATA/";
                    out.width(4);
                    out.fill('0');
                    out<<strArray.size()-i-1<<".html";
                    string path=out.str();
                    writeText(mainString,path);
                    out.str("");
                    out<<inputData.data["root"]<<"edit/OCR/_DATA/";
                    out.width(4);
                    out.fill('0');
                    out<<strArray.size()-i-1<<".xml";
                    path=out.str();
                    writeText(xmlString,path);
                    setOCR->destroy();
                    exit(0);
                }
                strArray[i].selectFlag=0;
                i--;if(i==-1)break;
            } 	
        }	
        
        pid=wait(&status); cout<<"new pecha";
        for(int index=0;index<pidID.size();index++){
            if(pid==pidID[index]){pidID[index]=0;processCount--;}
        }	
    }
    cout<<"start processCount="<<processCount<<endl;
    while(processCount){pid=wait(&status);
        if(pid>0){
         processCount--;
         cout<<"pid="<<pid<<" processCount="<<processCount<<endl;
        }
    }
    
    cout<<"collect all fork result in one file";
    
    vector<string>fileList;
    string path=inputData.data["root"]+"edit/OCR/_DATA/";
    int count=0;
    while(1){
       readDirectoryToArray(fileList, path,"html");
        if(fileList.size()!=strArray.size()){         
            cout<<"data not ready. has"<<fileList.size()<<" files. must be "<<strArray.size()<<" wait 2 sec."<<endl;
            count++; if(count==11)break;
             fileList.resize(0);
            sleep(2); 
        }else break;
    }
    
    for(int a=0;a<fileList.size();a++){
        string str; readText(str,fileList[a]);
        mainString+=str;
    }
    //cout<<"mainString="<<mainString<<endl;
    drawStrArray(strArray,border);
    xmlString="";  fileList.resize(0);
    readDirectoryToArray(fileList, path,"xml");
    for(int a=0;a<fileList.size();a++){
        string str; readText(str,fileList[a]);
        xmlString+=str;
    }
    //cout<<"xmlString="<<xmlString<<endl;
    writePageXML(xmlString);
    
    emptyFolder(path);

}//_________________________________
