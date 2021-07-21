#include "GFontEditor.h"

string GFontEditor::drawEditLetter(unsigned int letterIndex) {
    int    in=letterIndex;
    string maketStr, str;
    int    zoom=3;
    int    size=PICT_SIZE * zoom;
    if(in > aliKali[0].size()) return "";
    ostringstream out;
    GLetter *     letter=aliKali->getLetter(in);
    time_t        seconds;
    seconds=time(NULL);

    string path=inputData.data["root"] + "edit/mainEditLetter.html";
    readText(maketStr, path);

    int    indexMask=0;
    string name=letter->name;

    if(letter->y0 < -100 || letter->y0 > 100) {
        letter->y0=0;
        letter->y1=10;
    }

    out<<"./letterBaseProcess.esp?ocrData=letterImg&index="<<in<<"&tm="<<seconds;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@imageLink@-->", str, maketStr);
    maketStr=str_replace("<!-@letterName-->", letter->name, maketStr);

    vector<keyOCR> line;
    // int letterCount=aliKali->letterCount();
    /*
    for(unsigned int i=0;i<letterCount;i++){
        uchar c1=aliKali->cMatrix[0][FSIZE*letterIndex+i];
        keyOCR key;
        key.letterIndex=i;
        key.correlation=c1;
        line.push_back(key);
    }
    sort(line.begin(),line.end(),sortKeyC);
    */
    for(unsigned int i=0; i < line.size() && i < 25; i++) {
        GLetter *glyph=aliKali->getLetter(line[i].letterIndex);
        if(glyph->name !="*")
            out<<"<a href=\"/ocr/ocr.php?xml=<ocrData>edit</ocrData><index>"<<glyph->letterIndex
               <<"</index>\", target=\"_blank\" ><font size=\"3\" color=\"#"<<hex<<((short)line[i].correlation) * 2.5 + 0x000a0a
               <<"\">"<<glyph->name<<"</font>/"<<dec<<glyph->letterIndex<<"-"<<(short)line[i].correlation<<"</a>"<<endl;
        glyph->destroy();
    }
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@keyOCR@-->", str, maketStr);

    int leftInd=in - 1;
    if(leftInd < 0) leftInd=(int)aliKali->letterCount() - 1;
    int rightInd=in + 1;
    if(rightInd >=(int)aliKali->letterCount()) rightInd=0;

    out<<"/ocr/ocr.php?xml=<ocrData>edit</ocrData><window>self</window><index>"<<leftInd<<"</index>&tm="<<seconds;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@arrowLinkLeft@-->", str, maketStr);
    out<<"/ocr/ocr.php?xml=<ocrData>edit</ocrData><window>self</window><index>"<<rightInd<<"</index>&tm="<<seconds;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@arrowLinkRight@-->", str, maketStr);

    out<<"/ocr/ocr.php?xml=<ocrData>edit</ocrData><window>self</window><index>"<<aliKali->letterCount() - 1
       <<"</index>&tm="<<seconds;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@arrowLinkLast@-->", str, maketStr);

    out<<letter->mask32Count();
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@mask32Count@-->", str, maketStr);

    out<<letter->recordStatus;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@selfCorrelation@-->", str, maketStr);

    out<<letter->codeSpace;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@codeSpace@-->", str, maketStr);

    out<<letter->OCRIndex<<"/"<<letter->recordStatus;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@OCRIndex@-->", str, maketStr);

    maketStr=str_replace("<!--@OCRKey@-->", letter->OCRKey, maketStr);

    out<<letter->onOffRate;
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@onOffRate@-->", str, maketStr);

    out<<"<div class=\"dE dM\" id=\"s";
    out<<in<<"\" style=\"left:";
    out<<0<<"px; top:"<<size/2 + letter->y0 * zoom<<"px; width:"<<size;
    int h_=letter->y1 * zoom - letter->y0 * zoom;
    if(!h_) h_=10;
    out<<"px; height:"<<h_<<"px; border: 1px solid grey;\" ></div>\n";

    // cout<<"draw letter->letterH="<<letter->letterH<<in<<endl;
    int x0=PICT_SIZE/2 - letter->letterW / 2 + letter->dX;
    int y0=PICT_SIZE/2 - letter->letterH / 2 + letter->dY;
    int w=letter->letterW;
    int h=letter->letterH;
    out<<"<div class=\"dE dM\" id=\"g"<<in<<"\" style=\"left:"<<x0 * zoom<<"px; top:"<<y0 * zoom;
    out<<"px; width:"<<w * zoom<<"px; height:"<<h * zoom<<"px; border: 2px solid blue;\">\n";
    out<<"</div>\n";

    int color;
    srand((uint)time(NULL));
    // cout<<"aliKali[0][in]->mask32Count()="<<aliKali[0][in]->mask32Count()<<endl;

    for(int m=0; m < letter->mask32Count(); m++) {
        // cout<<" aliKali[0]["<<in<<"]->mask32["<<m<<"].status="<<aliKali[0][in]->mask32[m].status<<endl;
        if(letter->mask32[m].status < 1) continue;

        x0=letter->mask32[m].xMask + PICT_SIZE/2;
        y0=letter->mask32[m].yMask + PICT_SIZE/2;
        w=letter->mask32[m].imgW - 2;  //компенсация неточности JavaScript
        h=letter->mask32[m].mH;
        out<<"<div class=\"dE dM\" id=\"p";
        out<<in<<"_"<<m<<"_"<<indexMask<<"\" style=\"left:";
        out<<x0 * zoom<<"px; top:"<<y0 * zoom<<"px; width:"<<w * zoom<<"px; height:";
        out<<h * zoom<<"px;";
        if(letter->mask32[m].correlation > 95) {
            if(m==0) {
                out<<" border: 3px solid red;\">";
            } else {
                out<<" border: 1px solid rgb(";
                color=rand() % 100 + 1;
                out<<color<<"%,";
                color=rand() % 100 + 1;
                out<<color<<"%,";
                color=rand() % 100 + 1;
                out<<color<<"%)\"/>"<<endl;
            }
        } else {
            if(m==0) {
                out<<" border: 3px dotted red;\">";
            } else {
                out<<" border: 2px dotted rgb(";
                color=rand() % 100 + 1;
                out<<color<<"%,";
                color=rand() % 100 + 1;
                out<<color<<"%,";
                color=rand() % 100 + 1;
                out<<color<<"%)\"/>"<<endl;
            }
        }
        out<<"<div class=\"nP\" id=\"m"<<in<<"_"<<m<<"_"<<indexMask<<"\" onClick=\"edit1(\'m";
        out<<in<<"_"<<m<<"_"<<indexMask<<"\')\" onBlur=\"setText1(";
        out<<in<<")\">" <<
            // letter->mask32[m].dlt0;
            //(short)letter->mask32[m].NMask;
            (short)letter->mask32[m].correlation;
        // indexMask;
        out<<"</div></div>\n";
        indexMask++;

        // if(aliKali[0][in]->mask32[m].status<1)continue;
    }

    for(int m=(int)letter->mask32Count(); m <=21; m++) {
        // SH(aliKali[0][in]->mask32[m].w*zoomImage);  SH(aliKali[0][in]->mask32[m].h*zoomImage);

        x0=0;
        y0=0;
        w=24;
        h=24;
        out<<"<div class=\"dE dM\" id=\"p"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\" style=\"left:"<<x0 * zoom<<"px; top:"<<y0 * zoom<<"px; width:";
        out<<w * zoom<<"px; height:"<<h * zoom<<"px; border: 1px solid green;\">";
        out<<"<div class=\"nP\" id=\"m"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\" onClick=\"edit1(\'m"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\')\" onBlur=\"setText1("<<in<<")\">8</div></div>\n";
        indexMask++;
    }
    for(int m=12; m < 15; m++) {
        x0=10;
        y0=10;
        w=24;
        h=24;
        out<<"<div class=\"dE dM\" id=\"p"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\" style=\"left:"<<x0 * zoom<<"px; top:"<<y0 * zoom<<"px; width:";
        out<<w * zoom<<"px; height:"<<h * zoom<<"px; border: 1px solid green;\">";
        out<<"<div class=\"nP\" id=\"m"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\" onClick=\"edit1(\'m"<<in<<"_"<<m<<"_"<<indexMask;
        out<<"\')\" onBlur=\"setText1("<<in<<")\">28</div></div>\n";
        indexMask++;
    }
    // out<<"</td></tr><tr><td>";
    for(int m=0; m < letter->focalPoint->size(); m++) {
        x0=letter->focalPoint[0][m].x + PICT_SIZE/2;
        y0=letter->focalPoint[0][m].y + PICT_SIZE/2;
        // cout<<endl<<"x="<<x0<<" y="<<y0<<endl;
        w=2;
        h=2;
        out<<"<div class=\"point\"";
        out<<" style=\"position:absolute; left:"<<x0 * zoom<<"px; top:"<<y0 * zoom<<"px; width:";
        out<<w * zoom<<"px; height:"<<h * zoom<<"px; background:";
        int color=letter->focalPoint[0][m].type;
        if(color==T_POINT) out<<"#33FF33";
        if(color==X_POINT) out<<"#0033FF";
        if(color==L_POINT) out<<"#9900FF";
        if(color==P_POINT) out<<"#FF0099";
        // cout<<" m="<<m<<" status="<<(int)letter->focalPoint[0][m]->status<<" stability="<<(int)letter->focalPoint[0][m]->stability<<endl;
        if(!m && letter->focalPoint[0][m].type !=L_POINT) { out<<"; border:2px solid yellow"; }

        out<<"\"></div>\n";
    }
    // out<<"</form>";
    str=out.str();
    out.str("");
    maketStr=str_replace("<!--@mainLetter@-->", str, maketStr);
    out<<in;
    str=out.str();
    maketStr=str_replace("<!--@index-->", str, maketStr);

    return maketStr;
}

string GFontEditor::drawNewLetters() {
    ostringstream out;
    int           count=0;
    cout_<<"drawNewLetters aliKali->letterCount()="<<aliKali->letterCount()<<endl;
    for(size_t i=aliKali->letterCount() - 5; i < aliKali->letterCount(); i++) {
        cout_<<" "<<i;
        GLetter *letter=aliKali->getLetter(i);
        if(!letter->mask32Count()) {
            if(letter->name=="*") {
                letter->destroy();
                continue;
            }
            out<<"<script>location.href=\"/ocr/ocr.php?xml=<ocrData>edit</ocrData><index>"<<i
               <<"</index><window>self<window>\";</script>\n";
            count=1;
            letter->destroy();
            break;
        }
        letter->destroy();
    }
    if(count==0)
        out<<"<script>location.href=\"/ocr/ocr.php?xml=<ocrData>edit</ocrData><index>"<<aliKali->letterCount() - 1
           <<"</index><window>self<window>\";</script>\n";
    string str=out.str();
    cout_<<"str="<<str<<" count="<<count<<endl;
    return str;
}

/**вывод в result HTML результатов поиска */
void GFont::drawHTML(vector<ulong> &searchResult, string &result, int mode) {
    ostringstream str;
    int           a=0;
    // cout_<< "startRec="<<startRec<<" rowsNum="<<rowsNum<<endl;
    str<<searchResult.size()<<endl;
    for(uint i=0; i < searchResult.size(); i++) {  // cout<<" index="<<index;
        uint     index=(int)searchResult[i];
        GLetter *letter=getLetter(index);
        // if(letter->name=="*"){letter->destroy();continue;}
        // cout_<<"strDict.size="<<strDict.size;
        // for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        // str<<"index="<<index<<" name="<<letter->name;
        str<<"<div class=\"letterBlock\" title=\""<<letter->name<<"\" id=\""<<index<<"\" onClick=\"editLetterCell('"<<index
           <<"')\">"<<letter->name<<"<br>"<<index<<"</div>\n";
        a++;
        letter->destroy();
    }
    result=str.str();
}

/**вывод в result HTML rowsNum записей начиная с startRec */
void GFont::drawHTML(uint startRec, int rowsNum, string &result, int mode) {
    ostringstream str;
    // str<<size()/rowsNum<<endl;
    int a=0;
    int index_=startRec;
    // cout_<< "startRec="<<startRec<<" rowsNum="<<rowsNum<<endl;
    for(uint index=startRec; index_ < startRec + rowsNum * 20 && index < letterCount(); index++) {  // cout<<" index="<<index;
        if((index) % 20==0) str<<"<br/>\n";
        // cout<<index<<endl;
        GLetter *letter=getLetter(index);
        if(letter->name=="*") {
            letter->destroy();
            continue;
        }
        index_++;
        // cout_<<"strDict.size="<<strDict.size;
        // for(int i=0;i<strDict.len;i++)cout<<" "<<strDict[i]<<endl;  cout<<endl;
        // str<<"index="<<index<<" name="<<letter->name;
        // str<<"<div class=\"letterBlock\" title=\""<<letter->name<<"\" id=\""<<index<<"\"
        // onClick=\"editLetterCell('"<<index<<"')\">"<<index<<"</div>\n";

        str<<"<div class=\"letterCell\" title=\""<<letter->name<<"\" id=\"l"<<index
           <<"\"><a href=\"./OSBL_Letter.esp?index="<<index<<"\" target=\"_blank\">";
        str<<"<img class=\"lImg\" src=\"./create/letterImg.esp?index="<<index<<"\"><span class=\"lT\">"<<index
           <<"</span></a></div>";

        a++;
        letter->destroy();
    }
    result=str.str();
}

GBitmap *GFont::drawOCRBasePict(vector<ulong> &searchResult, int mode) {
    cout_<<"draw table mode="<<mode<<endl;
    string str;
    int    w=63;
    int    h=63;
    int    hMain=h * (int)searchResult.size()/20 + h;

    GBitmap *tableImg=GBitmap::create(w * 20, hMain);
    // cout_<<"w="<<tableImg->columns()<<" h="<<tableImg->rows()<<endl;
    // GBitmap *blank=GBitmap::create(w*5+40,h+120);
    // int in=-1;
    // cout_<<"input id="<<idNumber<<" correctionTable.size()="<<correctionTable.size()<<endl;
    //находим нужную букву по idNumber
    int step=0, y=0;
    // TIME_START
    // cout_<<"@@@letterCount="<<letterCount();
    for(int i=0; i < searchResult.size(); i++) {
        GLetter *letter=getLetter(searchResult[i]);
        // cout_<<"aliKali[0]["<<i<<"]->name="<<letter->name<<" OCRKey="<<letter->OCRKey<<endl;
        if(letter->name=="*" || letter->name=="") {
            letter->destroy();
            continue;
        }

        GBitmap *letterImg=letter->drawLetterPict(1);
        // GBitmap *letterImg=letter->mask128.unPackMask();
        GBitmap *letterImgScale=GBitmap::createScale(letterImg, 0.3);
        tableImg->drawImg(letterImgScale, w * step + 17, y + 15);
        step++;
        if(step==20) {
            // tableImg->drawImg(blank,w*step-40,y-90);
            y +=h;
            step=0;
        }
        letterImg->destroy();
        letterImgScale->destroy();
    }
    // TIME_PRINT_
    if(mode !=DRAW_BASE) {
        string path=inputData.data["root"] + "edit/OCRBase.jpg";
        tableImg->savePNG(path);
        tableImg->destroy();
        return 0;
    }
    return tableImg;
}

void GFont::drawOCRBasePict(uint startRec, uint rowsNum, int mode) {
    // cout_<<"draw table mode="<<mode<<endl;
    string str;
    int    w=63;
    int    h=63;
    int    letterPageCount=2000;
    if(mode) letterPageCount=20000;
    if(startRec==0) startRec++;
    GBitmap *tableImg=GBitmap::create(w * 20, h * 20 + 20);
    // cout_<<"w="<<tableImg->columns()<<" h="<<tableImg->rows()<<endl;
    // GBitmap *blank=GBitmap::create(w*5+40,h+120);
    // int in=-1;
    // cout_<<"input id="<<idNumber<<" correctionTable.size()="<<correctionTable.size()<<endl;
    //находим нужную букву по idNumber
    int step=0, y=0;
    // TIME_START
    // cout_<<"@@@letterCount="<<letterCount();
    for(int i=startRec; i < startRec + rowsNum && i < letterCount(); i++) {
        GLetter *letter=getLetter(i);
        // cout_<<"aliKali[0]["<<i<<"]->name="<<letter->name<<" OCRKey="<<letter->OCRKey<<endl;
        if(letter->name=="*") {
            letter->destroy();
            continue;
        }

        GBitmap *letterImg=letter->drawLetterPict(1);
        // GBitmap *letterImg=letter->mask128.unPackMask();
        GBitmap *letterImgScale=GBitmap::createScale(letterImg, 0.3);
        tableImg->drawImg(letterImgScale, w * step + 17, y + 15);
        step++;
        if(step==20) {
            // tableImg->drawImg(blank,w*step-40,y-90);
            y +=h;
            step=0;
        }
        letterImg->destroy();
        letterImgScale->destroy();
    }
    // TIME_PRINT_
    string path=inputData.data["root"] + "edit/OCRBase.png";
    tableImg->savePNG(path);
    tableImg->destroy();
}

GBitmap *GFont::drawOCRBaseImage(uint startRec, uint rowsNum) {
    string str;
    int    w=63;
    int    h=63;

    GBitmap *tableImg=GBitmap::create(w * 20, h * 20 + 20);
    int      step=0, y=0;
    // TIME_START
    // cout<<"@@@letterCount="<<letterCount()<<" rowsNum="<<rowsNum<<endl;
    for(int i=startRec; i < startRec + rowsNum * 20 && i < letterCount(); i++) {
        GLetter *letter=getLetter(i);
        // cout<<"aliKali[0]["<<i<<"]->name="<<letter->name<<" OCRKey="<<letter->OCRKey<<endl;
        if(letter->name=="*") {
            letter->destroy();
            continue;
        }

        GBitmap *letterImg=letter->drawLetterPict(1);
        // GBitmap *letterImg=letter->mask128.unPackMask();
        GBitmap *letterImgScale=GBitmap::createScale(letterImg, 0.3);
        tableImg->drawImg(letterImgScale, w * step + 17, y + 15);
        step++;
        if(step==20) {
            // tableImg->drawImg(blank,w*step-40,y-90);
            y +=h;
            step=0;
        }
        letterImg->destroy();
        letterImgScale->destroy();
    }
    return tableImg;
}
