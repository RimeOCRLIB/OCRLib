#include "GMainEditor.h"


///Распознавание на основе фокальных точек и векторов
void GMainEditor::startVectorOCR(){
    
    
    inputData.data["inputFile"]=inputData.fileList[0];
    cout<<inputData.data["inputFile"]<<" = "<<endl;
    
    //GBitmap *pechaImg_=GBitmap::create(inputData.data["inputFile"].c_str());
    //pechaImg_->eraseBorder(2, 2);
    //if(pechaImg_->DetectGradations())pechaImg_->binarisation(85,24);
    GGraphOCR pechaGraph;
    
    //pechaGraph.textOCR();
    pechaGraph.lineOCR();
    return;
    /*
    // распознавание макета страницы
    //распознавание слов и фраз
    strArray.resize(0);
    GBitset *setBit=GBitset::createResize(pechaImg,1,1,1);
    setBit->pageStringDetector(strArray,1); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
    //uint border=setBit->border_size();
    setBit->destroy();
    cout<<"done";
    */
    
    
}//_______________________________________________________

//сегментация страницы, последовательное распознавание букв, слов, фраз и строк
void GMainEditor::vectorCorrelation( vector<OCRPointFloat>&strPoint, vector<OCRFocalLineBezier>&lineVec){
    
    
    
    
}


