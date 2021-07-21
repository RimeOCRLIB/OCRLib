// _______OCRFont.cpp
// Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru
// ****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
// ****************************************************************************
//
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org.

#include "GGraph.h"
#include "../OCRTypes/OCRTypes.h"
#include "../OCRString/php2stl.h"


namespace ocr {
    
    
bool sortStar_(OCRData &d0,OCRData &d1){
    return d0.d0 > d1.d0;
}
bool sortC_11(pointLine &p0,pointLine &p1){
    return p0.x2 > p1.x2;
}
    
	
OCRFont::OCRFont() {
    cout<<"init font"<<endl;
    string fontPath = inputData.data["OCR_FONT_PATH"];
    string  str = fontPath + "OCRFont.bin";
    cout<<"path:"<<str<<endl;
    font.init(str.c_str());
    //массив букв алфавита распознаваемого языка
    str = fontPath + "OCRFontAlphabetLetter.bin";
    alphabetLetters = GVector::create(str.c_str());
    str = fontPath + "OCRFontCluster.bin";
    clusterArray.init(str.c_str());
    str = fontPath + "OCRFontIndex.bin";
    indexArray.init(str.c_str());
    str = fontPath + "OCRFontPath.bin";
    pathArray.init(str.c_str());
    str = fontPath + "OCRFontSort.bin";
    fontSortArray.init(str.c_str());
    str = fontPath + "OCRLetterGroupData.bin";
    letterGroupData.init(str.c_str());
    str = fontPath + "OCRLetterGroup.bin";
    letterGroupArray.init(str.c_str());
    str = fontPath + "OCRLetterMatch.bin";
    letterMatch.init(str.c_str());
    str = fontPath + "OCRLetterGroup2D.bin";
    letterGroup2D.init(str.c_str());
    
    //инициализация таблиц кластеров
    //массив базовых регистров
    
    str = fontPath + "OCRFontBaseCluster.bin";
    baseClusterArray.init(str.c_str());
    //таблица кольцевых регистров углов
    str = fontPath + "_indexRegister.bin";
    fontRegisterTable = GVector::create(str.c_str());
    //массив кольцевых регистров базовых кластеров
    str = fontPath + "_OCRRegister.bin";
    fontRegisterGStr.init(str.c_str());
    //таблица масок базовых кластеров
    str = fontPath + "_indexMask.bin";
    fontMaskTable = GVector::create(str.c_str());
    //массив масок базовых кластеров
    str = fontPath + "_OCRMask.bin";
    fontMaskGStr.init(str.c_str());
    //таблица базовых кластеров
    str = fontPath + "_indexCluster.bin";
    fontClusterTable = GVector::create(str.c_str());
    //таблица базовых кластеров буквы
    str = fontPath + "_indexLetter.bin";
    letterClusterTable = GVector::create(str.c_str());
	
}//_________________________________________________

OCRFont::~OCRFont() {}

void OCRFont::clear(){
	string fontPath = inputData.data["OCR_FONT_PATH"];
	string str = fontPath+ "OCRFont.bin";
	unlink(str.c_str());
	font.init(str.c_str());
  str = fontPath+ "OCRFontCluster.bin";
	unlink(str.c_str());
  clusterArray.init(str.c_str());
  str = fontPath + "OCRFontIndex.bin";
	unlink(str.c_str());
  indexArray.init(str.c_str());
  str = fontPath + "OCRFontPath.bin";
	unlink(str.c_str());
  pathArray.init(str.c_str());
  str = fontPath + "OCRFontAlphabetLetter.bin";
	unlink(str.c_str());
	alphabetLetters = GVector::create(str.c_str());
    
}//_________________________________________________

void OCRFont::clearClusterTable(){
	//таблица кольцевых регистров углов OCRLine кластеров
	string fontPath = inputData.data["OCR_FONT_PATH"];

	//массив базовых регистров
	string  str = fontPath+ "OCRFontBaseCluster.bin";
	unlink(str.c_str());
    baseClusterArray.init(str.c_str());
	//таблица кольцевых регистров углов
	str = fontPath+ "_indexRegister.bin";
	unlink(str.c_str());
	fontRegisterTable = GVector::create(str.c_str());
	//массив кольцевых регистров базовых кластеров
	str = fontPath+ "_OCRRegister.bin";
	unlink(str.c_str());
	fontRegisterGStr.init(str.c_str());
	//таблица масок базовых кластеров
	str = fontPath + "_indexMask.bin";
	unlink(str.c_str());
	fontMaskTable = GVector::create(str.c_str());
	//массив масок базовых кластеров
	str = fontPath + "_OCRMask.bin";
	unlink(str.c_str());
	fontMaskGStr.init(str.c_str());
	//таблица базовых кластеров
	str = fontPath + "_indexCluster.bin";
	unlink(str.c_str());
	fontClusterTable = GVector::create(str.c_str());
	//таблица базовых кластеров буквы
	str = fontPath + "_indexLetter.bin";
	unlink(str.c_str());
	letterClusterTable = GVector::create(str.c_str());
    str = fontPath + "OCRLetterGroup.bin";
    unlink(str.c_str());
    letterGroupArray.init(str.c_str());
    str = fontPath + "OCRLetterGroupData.bin";
    unlink(str.c_str());
    letterGroupData.init(str.c_str());
    str = fontPath + "OCRLetterMatch.bin";
    unlink(str.c_str());
    letterMatch.init(str.c_str());
    str = fontPath + "OCRLetterGroup2D.bin";
    unlink(str.c_str());
    letterGroup2D.init(str.c_str());

    
}//_________________________________________________
    
    
void OCRFont::clearLetterGroup(){
    string fontPath = inputData.data["OCR_FONT_PATH"];
    string str = fontPath + "OCRLetterGroup.bin";
    unlink(str.c_str());
    letterGroupArray.init(str.c_str());
}
void OCRFont::clearLetterGroup2D(){
    string fontPath = inputData.data["OCR_FONT_PATH"];
    string str = fontPath + "OCRLetterGroup2D.bin";
    unlink(str.c_str());
    letterGroup2D.init(str.c_str());
}
    
void OCRFont::clearLetterGroupData(){
    string fontPath = inputData.data["OCR_FONT_PATH"];
    string str = fontPath + "OCRLetterGroupData.bin";
    unlink(str.c_str());
    letterGroupData.init(str.c_str());
    str = fontPath + "OCRLetterMatch.bin";
    unlink(str.c_str());
    letterMatch.init(str.c_str());
}


OCRLetter& OCRFont::operator[](uint index){
	return font[index];	
}//_________________________________________________

OCRLetter& OCRFont::getLetter(uint index) {
	return font[index];	
}//_________________________________________________

OCRLetter& OCRFont::getLetter_(uint index) {
	return font[index];	
}//_________________________________________________

//** @bref функция чтения данных буквы по ее ID номеру. */
OCRLetter& OCRFont::letterByID(uint ID){
    uint size_=(uint)font.size();
    uint sizeArray=(uint)fontID.size();
    if(!sizeArray){
        cout<<"start build font index"<<endl;
        for(int i=0;i<size_;i++){
            fontID[font[i].letterID]=i;
        }
    }
	if (fontID.count(ID) > 0){
			return font[fontID[ID]];
	}
	static OCRLetter letter;
	letter.index=0xFFFFFFFF;
	cout<<"no this letter ID in font:"<<ID<<endl;
	return (OCRLetter &)letter;
}//_________________________________________________

void OCRFont::getAlphabetLetter(uint index,TString &st){
	alphabetLetters->getTStr(index, st);
}

void OCRFont::fontNormalisation() {
}//_________________________________________________

/** чтение данных базы букв из OCR_FONT_PATH или
 импорт XML данных OCRFont из path to database */
void OCRFont::readOCRFontXML() {

}//_________________________________________________

void OCRFont::writeOCRFontXML() {
  

}//_________________________________________________

GBitmap* OCRFont::drawPict(int in) {
  //OCRLetter* letter = getLetter(in);
  GBitmap* img;
  return img;
}//_________________________________________________

GBitmap* OCRFont::drawLetterPict(int in) {
  //OCRLetter* letter = getLetter(in);
  GBitmap* img;
  return img;
}//_________________________________________________

void OCRFont::drawLetterSVG(string &dataStr, unsigned int in) {
  OCRLetter &letter = getLetter(in);
  draw.drawLetterSVG((void *)&letter, this, (void *)&dataStr);
}//_________________________________________________

void OCRFont::printLetterSVG(unsigned int in){
	string dataStr;
	drawLetterSVG(dataStr,in);
    string pathDraw = ROOT_PATH;
    pathDraw += "/CORRELATION/__letter.html";
    cout<<"@1";
    writeText(dataStr,pathDraw);
    cout<<"@2";
}

void OCRFont::buildLetter(cstr path, int mode) {
    //функция нормирует координаты элементов буквы и формирует данные для записи буквы в базу
    //данных Каждая буква имеет свои систему координат с центром буквы как точка отсчета
    //координат. По этим координатам находятся центры кластеров. Внутри кластеры имеют свою
    //систему координат с центром первой звездочки кластера как точка отсчета. 
    
    int print=0;
    DR("buildLetter"<<endl);
    OCRLetter letter;
    string path_= path;
    string str=path_;
    str = substr((int)str.rfind("/") + 1, (int)str.size(), str);
    str = str_replace(".png", "", str);
    vector<string> line;
    explode("_", str,line);
    if (line.size() < 2) return;
    letter.name = line[1];
    letter.letterID = atoi(line[2].c_str());
    letter.fontType=0;
    letter.OCRStatus=1;
    //if(letter.letterID!=1619205291)return;
    
    letter.OCRIndex = 0;
    if(path_.find("MANUSCRIPT")!=-1)letter.fontType=LETTER_MANUSCRIPT;
    if(path_.find("letter_font_1")!=-1)letter.fontType=1;
    if(path_.find("letter_font_2")!=-1)letter.fontType=2;
    if(path_.find("letter_font_3")!=-1)letter.fontType=3;
    if(path_.find("_TIB_")!=-1)letter.lang=OCR_TIBETAN;
    if(fontOCRIndex.count(line[1])>0){
        letter.OCRIndex=(uchar)fontOCRIndex[line[1]];
    }else{
        letter.OCRIndex=OCR_LETTER;
    }
    
    
    GBitmap *img = GBitmap::create(path);
		img->crop32();
		img->binarisation64();
		GGraph graph(img);
		img->destroy();
		graph.graphFocalLine(15);
		//graph.printToScreenBinary();
		graph.focalPointXTL();
		graph.focalLineRecognition();
		graph.buildOCRStars();
		//draw.printGraphSVG((void *)&graph,"/OSBL/CORRELATION");
		GStr<OCRCluster> &clusterArray_=graph.clusterArray;
		
    if(!clusterArray_.size()){cout<<"no content in graph.clusterArray\n"; return;}

    int clusterCount=(int)clusterArray_.size();

    int xMax = 0;
    int yMax = 0;
    int xMin = 10000;
    int yMin = 10000;

    for(int i=0; i < clusterCount; i++) {
        OCRCluster &cluster=clusterArray_[i];

        DR(" @0 new cluster check:"<<i<<" type:"<<(int)cluster.type <<" sIndex:"<<cluster.sIndex<<" starCount:"<<(int)cluster.starCount
           <<" x:"<<cluster.cPoint.x<<" y:"<<cluster.cPoint.y<<" xMin:"<<cluster.xMin<<endl);

        int limit=cluster.cPoint.x + cluster.xMax;
        if(limit > xMax)xMax = limit;
        limit=cluster.cPoint.x + cluster.xMin;
        if(limit < xMin)xMin = limit;
        limit=cluster.cPoint.y + cluster.yMax;
        if(limit > yMax)yMax = limit;
        limit=cluster.cPoint.y + cluster.yMin;
        if(limit < yMin)yMin = limit;
        
        
    }

    int xCenter=(xMax + xMin)/2;
    int yCenter=(yMax + yMin)/2;

    letter.xCenter=xCenter;
    letter.yCenter=yCenter;
    letter.letterW=xMax - xMin;
    letter.letterH=yMax - yMin;
    letter.s.area=letter.letterW * letter.letterH;

    
    //сортировка кластеров по стабильности
    
    int midSum=0;  //средняя для всей буквы сумма длин линий звездочки до экстремума
    int sumLen=0;

    // подсчет сумм длин лучей звездочек по букве
    for(int i=0; i < clusterCount; i++) {
        OCRCluster &cluster=clusterArray_[i];
        OCRStar &star=cluster.star[0];
        if(star.type==O_POINT) continue;
        //подсчитываем сумму длин всех линий звездочек
        sumLen +=star.sumLen1;
        DR(" @1 new star check:"<<i<<" type:"<<(int)star.type
           <<" lineCount:"<<(int)star.pointCount<<endl);
        midSum +=star.sumLen1;

    }
    //letter.sumLen=sumLen;

    // подсчет средних сумм длинн
    midSum /=clusterCount;
    if(midSum==0) midSum=1;

    vector<OCRData>starSortArray(clusterCount);
    // вычисляем коэффициент стабильности  OCRStar //
    for(int i=0; i < clusterCount; i++) {
        OCRCluster &cluster=clusterArray_[i];
        OCRStar &star=cluster.star[0];
        star.status=0;
        starSortArray[i].d1=i;
        starSortArray[i].d0=10000;

        if(star.type==O_POINT) continue;

        int mLine=(star.minLen * 100)/midSum;
        // наличие коротких линий уменьшает стабильность точки
        if(mLine < 18) mLine=0;
        // вычисляем самые стабильные точки по эмпирической формуле, где
        // disp - мера дисперсии в %, mLine - относительная длина минимального луча 
        //звездочки в % Чем больше "k" тем более стабильна особая точка
        int k=(100 - star.disp)*3 + mLine + star.lineCount * 200;  // +(letter.star[i].sumLenE/10+letter.star[i].minLen);
        //star.corr=k;
        starSortArray[i].d0=k;
        
        DR(i<<"\tdisp:"<<star.disp<<"\tsum:"<<star.sumLen<<"\tmLine:"<<mLine
            <<"\t   k:"<<(int)star.corr<<endl);

        // for(int j=0;j<letter.star[i].pointCount;j++){
        //    DR("\t\t\t"<<j<<"\tl:"<<letter.line[letter.star[i].line[j]].length<<endl);
        //}
    }
    
    //сортируем OCRStar по максимальному коэффициенту стабильности
    sort(starSortArray.begin(),starSortArray.end(),sortStar_);
    
    //записываем данные в букву
    
    letter.index=(int)size();
    if(clusterCount >=MAX_CLUSTER_LETTER) clusterCount = MAX_CLUSTER_LETTER -1;
    letter.clusterCount=clusterCount;
    
    for(int i=0; i < clusterCount; i++) {
        OCRCluster &cluster=clusterArray_[starSortArray[i].d1];
        cluster.index=(int)clusterArray.size();
        for(int n=0; n < cluster.starCount; n++){
            cluster.star[n].clusterIndex=i;
        }
        letter.cluster[i]=cluster.index;
        cluster.indexInLetter=i;
        clusterArray.push_back(cluster);
        //cout<<"letter:"<<letter.index<<" cluster:"<<cluster.index<<endl;
    }

    //if(mode){
    //    letter.OCRIndex = 255; // alphabet letter
    //    TString st;
    //    graph.saveToTStr(st);
    //    alphabetLetters->push_back(st);
    //}
    
    int lineSum = 0;
    int lineOCRCount = 0;
    uint lineCount=(uint)graph.focalLine.size();
    for(int i=0; i < lineCount; i++) {
        lineSum += graph.focalLine[i].lenA;
        if(graph.focalLine[i].cType)lineOCRCount++;
    }
    letter.lineSum = lineSum;
    letter.lineCount = lineCount;
    letter.lineOCRCount=lineOCRCount;
    font.push_back(letter);
    //draw.printLetterSVG((void *)&letter, this, "/OSBL/CORRELATION/"); exit(0);
    
}//_________________________________________________________

/** @bref Читает с диска папку с файлами изображений букв в формате  PNG и создает letterOCR для каждого файла. Папки с изображениями букв  могут быть архивированы в формате ZIP.
 * Результат записывается в текущий OCR шрифт.  */
void OCRFont::buildOCRFont(cstr path) {
	clear();
	clearClusterTable();
  //создаем алфавит
    string fontPath1 = inputData.data["OCR_ALPHABET_PATH"];
    string fontPath2 = inputData.data["OCR_LETTERS_PATH"];
    buildLetters(fontPath1.c_str(), 0);
	buildLetters(fontPath2.c_str(), 0);
    buildSortArray();
}//_________________________________________________________
    
void OCRFont::buildSortArray(){
    vector<pointLine>v;
    uint fontSize=(uint)size();
    int print = 1;
    DR("Start build alphabet"<<endl;)
    
    for( int index=0;index < fontSize;index++){
        OCRLetter &letterTest = font[index];
        //if(letterTest.OCRIndex != 255)break;  //not alphabet letter
        pointLine p;
        //int matchCount = letterTest.matchCount;
        p.x1 = index;
        p.x2 = letterTest.lineOCRCount;
        for(int i=0;i<letterTest.clusterCount;i++){
            if(letterTest.match[i].cPoint.type>1)p.x2++;
            if(letterTest.match[i].cPoint.type==T_POINT)p.x2+=1;
            if(letterTest.match[i].cPoint.type==X_POINT)p.x2+=2;
            if(letterTest.match[i].cPoint.type==O_POINT)p.x2+=2;
            
        }
        if(letterTest.OCRIndex==OCR_LETTER)p.x2+=100; //первыми распознаем коренные буквы
        v.push_back(p);
        //baseClusterRegistr[index].push_back(matchCount);//первый элемент строки реестра буквы
        
        //for (int indexС = 0; indexС < matchCount; indexС++) { //для каждого базового кластера буквы
        //    baseClusterRegistr[index].push_back(letterTest.match[indexС].clusterID);
        //}
    }

    sort(v.begin(),v.end(),sortC_11);
    
    fontSortArray.resize(fontSize);
    memcpy((char*)fontSortArray.dataPtr(),(char*)&v[0],fontSize*sizeof(pointLine));
}
    
/** @bref Функция добавляет в OCRFont новые изображения обучающей выборки*/
void OCRFont::updateOCRFont(cstr path){
    //string  fontPath1 = inputData.data["OCR_ALPHABET_PATH"];
    string fontPath2 = inputData.data["OCR_LETTERS_PATH "];
    //buildLetters(fontPath1.c_str(), 2);
    buildLetters(fontPath2.c_str(), 2);
    buildSortArray();

}
 
void OCRFont::buildLetters(cstr path, int mode) {
    
    //читаем текстовой файл алфавита
    if(!fontOCRIndex.size()){
        string text;
        string path_=inputData.data["grammar"] + "tib_alphabet.txt";
        readText(text, path_);
        vector<string>lines;
        vector<string>v;
        explode("\n", text, lines);
        for (int i = 0; i < lines.size(); i++) {
            string line=lines[i];
            if(line[0]!='_')continue;
            explode("_:_", line, v);
            if(v.size()<3)continue;
            fontOCRIndex[v[1]]=atoi(v[2].c_str());
        }
    }
    
	vector<string> fileList;
	string path_ = path;
	OCRLetter letter;
	string cmd;
	string pathZIP;
	if(path_.find(".zip")==-1){
		readDirectoryToArray(fileList, path_, "img");
		for (int i = 0; i < fileList.size(); i++) {
			string str = fileList[i];
			cout << i << " >>" << str << endl;
			if (str.find(".png") == -1) continue;
            //проверяем есть ли такой файл в OCR шрифте
            int flag=1;
            if(mode==2){
                for(int n=0;n<pathArray.size();n++){
                    if(pathArray[n].find(str)>-1){
                        flag=0;
                        break;
                    }
                }
            }
            if(flag){
                pathArray.push_back(GString(str));
                buildLetter(str.c_str(),mode);
            }
		  if(i%100==0)cout<<i<<" ";
		}
		fileList.resize(0);
		readDirectoryToArray(fileList, path_, "zip");
	}else{
		vector<string>list;
		explode(" ",path,list);
		for(int i=0;i<list.size();i++){
			string item=ROOT_PATH;
			item+="/OCRData/_FONT_SOURCE1/";
			item+=list[i];
			fileList.push_back(item);
		}
	}
	for (int i = 0; i < fileList.size(); i++) {
		string item = fileList[i];
		cout<<i<<" >>"<<item<<endl;
		//получаем список файлов в архиве ZIP		
		cmd = "python ";
		cmd+=ROOT_PATH;
		cmd += "src/zip.py -l ";
		cmd += item;
		cout<<cmd<<endl;
		string str = run(cmd);
		vector<string> list;
		explode("\n", str,list);
		for (int index; index < list.size(); index++) {
			string s = list[index];
			if (s.find(".png") == -1) continue;
			cmd = "python ";
			cmd += ROOT_PATH;
			cmd += "src/zip.py -e ";
			cmd += item + " ";
			cmd += s;
			pathArray.push_back(GString(cmd));
			cout<<cmd<<endl;
			string pathPNG = run(cmd);
			pathPNG.resize(pathPNG.size() - 1); // delete new line character
			cout << pathPNG << endl;
			buildLetter(pathPNG.c_str(),mode);
			unlink(pathPNG.c_str());
			if (i % 100 == 0) cout << i << " ";
			// break;
		}
	}

} //_____________________________________________________

void OCRFont::joinLetters(OCRLetter &letterRoot, OCRLetter &letterMark, int mode){
    
    string st=letterMark.name.data;
    if(st=="］"){
        cout<<1;
    }
    
    if(mode==JOIN_RIGHT){
        string str = letterRoot.name.data;
        str+= letterMark.name.data;
        str+="/";
        letterRoot.name = str;
    }else{
        string str = letterMark.name.data;
        str+= letterRoot.name.data;
        str+="/";
        letterRoot.name = str;
    }
    float scaleRoot=letterRoot.scale;
    float scaleMark=letterMark.scale;
    
    int x0=letterRoot.xMatch - letterRoot.letterW * scaleRoot/2;
    int x1=letterRoot.xMatch + letterRoot.letterW * scaleRoot/2;
    int y0=letterRoot.yMatch - letterRoot.letterH * scaleRoot/2;
    int y1=letterRoot.yMatch + letterRoot.letterH * scaleRoot/2;
    
    
    int x0_=letterMark.xMatch - letterMark.letterW * scaleMark/2;
    int x1_=letterMark.xMatch + letterMark.letterW * scaleMark/2;
    int y0_=letterMark.yMatch - letterMark.letterH * scaleMark/2;
    int y1_=letterMark.yMatch + letterMark.letterH * scaleMark/2;
    
    if(x0_ < x0)x0 = x0_;
    if(x1_ > x1)x1 = x1_;
    if(y0_ < y0)y0 = y0_;
    if(y1_ > y1)y1 = y1_;
    
    letterRoot.xMatch=x0 + (x1-x0)/2;
    letterRoot.yMatch=y0 + (y1-y0)/2;
    letterRoot.letterW = x1 - x0;
    letterRoot.letterH = y1 - y0;
    letterRoot.scale=1;
    
    
} //_____________________________________________________


void OCRFont::printGroup(OCRGroup &group){
    cout<<"//__print group size:"<<group.size<<" _____"<<endl;
    OCRLetter &letterBase=font[group.letter[0]];
    cout<<"  letterBase: "<<letterBase.name.data<<endl;
    
    for(int i=1;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        cout<<" "<<letter.name.data<<endl;
    }
    cout<<endl;
    
    //OCRLetterMatch match=letterMatch[group.match[0].d0];
    //printArray(match.kLine, match.lineCount);
    
    for(int i=0;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        if(group.match[i].d0>-1){
            OCRLetterMatch match1=letterMatch[group.match[i].d0];
            cout<<i<<"    letter: "<<letter.name.data<<" index:"<<letter.index<<" ID:"<<letter.letterID;
            cout<<"         xCenter:"<<match1.xCenter<<" yCenter:"<<match1.yCenter<<" scale:"<<match1.scale<<
            " lineCount:"<<letter.lineCount<<endl;

            cout<<"         d0 c:"<<(uint)match1.corr<<" ";
            printArray(match1.kLine, match1.lineCount);
            cout<<"         index0:"<<group.match[i].d0<<" index1:"<<match1.baseIndex<<endl;
        }
        if(group.match[i].d1>-1){
            OCRLetterMatch match1=letterMatch[group.match[i].d1];
            cout<<i<<"    letter: "<<letter.name.data<<" index:"<<letter.index<<" ID:"<<letter.letterID;
            cout<<"         xCenter:"<<match1.xCenter<<" yCenter:"<<match1.yCenter<<" scale:"<<match1.scale<<
            " lineCount:"<<letter.lineCount<<endl;

            cout<<"         d1 c:"<<(uint)match1.corr<<" ";
            printArray(match1.kLine, match1.lineCount);
            cout<<"         index0:"<<group.match[i].d1<<" index1:"<<match1.baseIndex<<endl;
        }
    
    }

}
    
void OCRFont::printGroup(int index){
    printGroup(letterGroupArray[index]);
}
    
    
void OCRFont::printGroupData(OCRGroupData &group){
    cout<<"//__print group size:"<<group.size<<" _____"<<endl;
    OCRLetter &letterBase=font[group.letter[0]];
    cout<<"  letterBase: "<<letterBase.name.data<<" f:"<<(int)letterBase.fontType<<" ID:"<<letterBase.letterID<<endl;
    //OCRLetterMatch match=letterMatch[group.match[0]];
    //printArray(match.kLine, match.lineCount);
    for(int i=0;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        cout<<i<<" "<<letter.name.data<<" f:"<<(int)letter.fontType<<" ID:"<<letter.letterID<<endl;
    }
    cout<<endl;
    
    return;
    
    for(int i=0;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        OCRLetterMatch match1=letterMatch[group.match[i]];
        cout<<i<<"    letter: "<<letter.name.data<<" index:"<<letter.index<<" ID:"<<letter.letterID;
        cout<<"         xCenter:"<<match1.xCenter<<" yCenter:"<<match1.yCenter<<" scale:"<<match1.scale<<
        " lineCount:"<<letter.lineCount<<endl;
        cout<<"         c:"<<(uint)match1.corr<<" ";
        printArray(match1.kLine, match1.lineCount);
        cout<<"         index0:"<<group.match[i]<<" index1:"<<match1.baseIndex<<endl;
    }
}
    
void OCRFont::printGroup2D(OCRGroup2D &group){
    cout<<"//__print group size:"<<group.size<<" _____"<<endl;
    OCRLetter &letterBase=font[group.letter[0]];
    for(int i=0;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        cout<<" "<<letter.name.data;
    }
    cout<<endl;
    /*
    OCRLetterMatch match=letterMatch[group.match[0].d0];
    printArray(match.kLine, match.lineCount);
    
    for(int i=0;i<group.size;i++){
        OCRLetter &letter=font[group.letter[i]];
        cout<<i<<"    letter: "<<letter.name.data<<" index:"<<letter.index<<" ID:"<<letter.letterID;
        cout<<"         xCenter:"<<match.xCenter<<" yCenter:"<<match.yCenter<<" scale:"<<match.scale<<
        " lineCount:"<<letter.lineCount<<endl;
        if(group.match[i].d0>-1){
            OCRLetterMatch match1=letterMatch[group.match[i].d0];
            cout<<"         d0 c:"<<(uint)match1.corr<<" ";
            printArray(match.kLine, match1.lineCount);
        }
        if(group.match[i].d1>-1){
            OCRLetterMatch match1=letterMatch[group.match[i].d1];
            cout<<"         d1 c:"<<(uint)match1.corr<<" ";
            printArray(match.kLine, match1.lineCount);
        }
        
    }
    */
}

void OCRFont::printGroup2D(int index){
    printGroup2D(letterGroup2D[index]);
}

void OCRFont::cpLetter(OCRLetter &letter){
    GString &path=pathArray[letter.index];
    ostringstream out;
    out<<"cp "<<path.c_str()<<" "<<inputData.data["root"]<<"_"<<letter.name.data<<"_"<<letter.letterID<<".png";
    string command=out.str();
    run(command);
}
    
}; // namespace ocr
