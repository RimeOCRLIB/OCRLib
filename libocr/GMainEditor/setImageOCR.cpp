#include "GMainEditor.h"


void GMainEditor::resetOCR(){
    if(strArray.size()){
        setBit->destroy();
        setOCR->destroy();
        strArray.resize(0);
    }
}


///диспетчер выполнения процесса определения масштаба страницы.
int GMainEditor::setImageOCR(GBitmap *pechaImg_,vector<float*>&param, OCRPointFloat &point){

    //cout<<"Start#1 "<<inputData.data["inputFile"]<<endl;
    if(!pechaImg_){cout_<<"no open file"<<inputData.data["inputFile"]<<endl; return 1;}
    pechaImg=pechaImg_;
    int border;
    string str;
    int print=0;
    
    if(!strArray.size()){
        setBit=GBitset::createResize(pechaImg_,1,1,1);
        DT("@4_2");
        setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        DT("@4_3");
        border=setBit->border_size();
        DT("@4_4");
        setOCR=GBitsetOCR::createResize(pechaImg_,1,1);
        //if(NewLetterOnlyBtn->Checked==true) {mode=letterID;}else{mode=ALL_LETTER;}
        str=fileName(inputData.data["inputFile"]); DT("@@@ file="<<str);
        
        setOCR->setData(
                        aliKali,
                        strArray,
                        correctionTable,
                        logicProcessor,
                        iLeft,
                        iTop,
                        border,
                        OCR_NORMALISATION);
    }
    
    int imageParameterIndex=setOCR->setImageParamOCR(pechaImg_,param, point);
    return imageParameterIndex;
    
    //cout<<"done "<<inputData.data["inputFile"]<<endl;
    
}//________________________________________________________________________________________________________________
