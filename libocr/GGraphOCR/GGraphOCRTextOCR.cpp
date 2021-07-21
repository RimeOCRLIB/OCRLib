//__GGraphOCRTextOCR.cpp
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

#include "../OCRString/php2stl.h"
#include "GGraphOCR.h"
#include "../GBitset/GBitset.h"
#include "../GDraw/GDraw.h"
#include <unistd.h>

#include "OCRLib.h"

namespace ocr {
    
void GGraphOCR::lineOCR(){
    cout<<"startOCR"<<endl;
    
    int CREATE_NEW_FONT=0;
    int UPDATE_FONT=0;
    int BUILD_FONT_TABLE=0;
    
    int OCR_PECHA=0;

    
    GBitmap *img;
    strTest=" ";
    letterPrint=1;//1618936360   //1619784698   //1618936360;   //1569379709  1618936262 test! 1569363894; //1569379510 1569358799 a; //ga 1619655356; //1618920685 yo; //1619281359;//1619281354 tha;//1576660907 tha; //1576660907;   //1618962863 do;//1569374541 na //1569365687 rja;//1569372737 rda//1569370478//1577738079 //1569377825 do//1569364533 ma//1569369431 na //1569368608 //1569365026//1618705957//1569365140//1618345631 //1569368532

    /*
    int count=0;
    for(int i=0;i<font.size();i++){
        OCRLetter &letter=font[i];
        if(!letter.OCRStatus)continue;
        count++;
     
        //letter.fontType=0;
        //string path_=font.pathArray[i].data;
        //if(path_.find("MANUSCRIPT")!=-1)letter.fontType=LETTER_MANUSCRIPT;
        //if(path_.find("letter_font_1")!=-1)letter.fontType=1;
        //if(path_.find("letter_font_2")!=-1)letter.fontType=2;
        //if(path_.find("letter_font_3")!=-1)letter.fontType=3;
        //if(path_.find("_TIB_")!=-1)letter.lang=OCR_TIBETAN;
     
    }
    cout<<" count:"<<count<<" font.size():"<<font.size()<<endl;
    return;
    */
     
    //pageRec page;
    //page.text="inputFile=/OSBL/OCRData/LETTERS_ALPHABET/LETTERS_ALPHABET_TIB_&ocrData=imgDir";
    //imageProcess(page);
    
    /*
    OCRLetter &letterBase=font.letterByID(1622980427);
    OCRLetter &letter1=font.letterByID(1622985229);
    OCRLetterMatch match;
    match.scale=1;
    match.xCenter=letterBase.xCenter;
    match.yCenter=letterBase.yCenter;
    
    buildletterGroup(letterBase);
    
    
    //int corr=letterMatchCorrelation(letterBase, letter1, match);
    OCRGroupData &group=font.letterGroupData[letterBase.index];
    font.printGroupData(group);
    
    drawLetter(letter1);
    */
    
    //buildletterGroup(font[3]);
    buildAlphabet();
    
    
    /*
    vector<int>v;
    v.resize(font.font.size());
    
    for(int index5=1; index5<font.letterGroupArray.size();index5++){
    OCRGroupData &group=font.letterGroupData[index5];
        int letterIndex=group.letter[0];
        for(int index=1; index<group.size;index++){
            OCRLetterMatch &match=font.letterMatch[group.match[index]];
            if(match.OCRIndex && match.corr > 97){
                v[group.letter[index]]++;
            }
        }
    }
    int count=0;
    for(int index=0; index<v.size();index++){
        if(v[index])count++;
    }
    cout<<"count:"<<count<<endl;
    */
    
    //buildOCRFontCorrelation("/OSBL/___TEST");
    
    //OCRLetter &letter=font.letterByID(1569376733);
    //OCRLetter &letter=font.letterByID(1619205291); //test
    
    //OCRLetter &letter=font[674];
    //font.cpLetter(letter);
   
    //OCRGroup2D &group=font.letterGroup2D[4875];
    //font.printGroup(letter.index);
    //font.printGroup2D(letter.group);
    //cout<<letter.name.data<<endl;
    
    return;
    
/*
    if(UPDATE_FONT){
        font.updateOCRFont("/OSBL/LETTERS_NEW");
        buildOCRFontClusterTable(3); return;
        cout<<"font.size:"<<font.size()<<" countStar:"<<countStar<<endl;
    }
    
    
    if(CREATE_NEW_FONT){
        font.buildOCRFont("/OSBL/LETTERS");
        buildOCRFontClusterTable(1);
        cout<<"font.size:"<<font.size()<<" countStar:"<<countStar<<endl;
        return;
    //    //buildAlphabet();
    }
    if(BUILD_FONT_TABLE){
        buildOCRFontClusterTable(1);
        cout<<"font.size:"<<font.size()<<" countStar:"<<countStar<<endl;
        return;
        //    //buildAlphabet();
    }
    
    
    if(OCR_PECHA){
        letterPrint=1;
        strTest="";
        img = GBitmap::create("/OSBL/_1.png");
        GBitset *setBit=GBitset::createResize(img,1,1,0);
        setBit->pageStringDetectorOCR(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛЬСЫ  )
        int border=setBit->border_size();
        setBit->destroy();
    }else{
        img = GBitmap::create("/OSBL/_1_.png");
        OCRBox p;
        p.yLimit0=0;
        p.yLimit1=img->rows();
        strArray.push_back(p);
    }
    
    
    countStar=0;
    countMask=0;
    countLineMask=0;
    countGeomentry=0;
    countLetterCorrelation=0;
    countLookup4=0;
    countVoronov=0;


    img->crop32();
    img->binarisation64();
    GGraph graph_1(img);
    //img->printToScreenBinary();
    img->destroy();
    graph_1.graphFocalLine(15);
    //graph_1.printToScreenBinary();
    graph_1.focalPointXTL();
    graph_1.focalLineRecognition();
    graph_1.buildOCRStars();
    //draw.printGraphSVG((void *)&graph_1,"/OSBL/CORRELATION");
    vector<OCRLetter>mainMatchLetter; //массив гипотез букв в проверяемом тексте
    string mainString;
    
    TIME_START
    textLetterGroupCorrelation(graph_1, mainMatchLetter);
    buildTextLine(mainMatchLetter,graph_1, mainString);
    if(OCR_PECHA){
        draw.printMatchLetters((void*)&mainMatchLetter, (void*)&graph_1, (void*)&strArray, "_1.png","/OSBL/_1.html");
    }else{
        draw.printMatchLetters((void*)&mainMatchLetter, (void*)&graph_1, (void*)&strArray, "_1_.png","/OSBL/_1.html");
    }
    
    cout<<"// TEXT OCR time: ";
    TIME_PRINT_
    
    cout<<"countStar:"<<countStar<<endl;
    cout<<"countMask:"<<countMask<<endl;
    cout<<"countLineMask:"<<countLineMask<<endl;
    cout<<"countGeomentry:"<<countGeomentry<<endl;
    cout<<"countLetterCorrelation:"<<countLetterCorrelation<<endl;
    cout<<"countLookup4:"<<countLookup4<<endl;
    cout<<"countVoronov:"<<countVoronov<<endl;
    
    
    return;
    */


}

/** @bref основная функция распознавания текста на основе графов фокальных линий */
void GGraphOCR::textOCR() {

	/**
	 Функция определяющая порядок распознавания букв на странице.
	 1. Выполняется распознавание всех фокальных линий и OCRStar на странице.
	 2. На странице формируются двойные кластеры и создается таблица хешей двойных кластеров.
	 3. В базе данных выполняется сортировка всех кластеров всех букв по частоте встречаемости
	 для каждого кластера формируется список букв в которых он встречается. В кластер записывается размер наибольшей буквы
	 3. Начиная с начала сортированного по встречаемости списка кластеров выполняется поиск кластеров
	 на странице. Порядок поиска кластера эталона на странице:
		1. Из таблицы хешей двойных кластеров текста выбираются соответствующие координаты гипотез кластера эталона в тексте.
		2. Для этих гипотез выполняется проверка 1,2 и 3 lookup.
		3. В случае превышения порога корреляции первого кластера эталона выполняется поиск гипотез всех букв в которые входит данный кластер.
			Порядок поиска гипотез букв:
			1. Вычисляется размер наибольшего габарита гипотезы буквы относительно масштаба найденного кластера.
			2. В пределах наибольшего габарита гипотезы буквы выполняется поиск всех двойных кластеров текста.
			   из результатов поиска исключаются кластеры текста котрые были уверенно распознаны предыдущими эталонами букв.
			   проверка производится по таблице распознанных кластеров.
			3. Значения хеша найденных двойнах кластеров значение хеша и координаты заносятся в таблицу хешей длиной в 2048 байт как значения байтов.
			4. Проверка гипотез букв выпоняется начиная с букв с наибольшим габаритом.
			4. Для всех двойных кластеров гипотезы буквы выполняется поиск соответствия хеша по таблице хешей.
			5. В том случае если количество совпадений хешей выше порога выпоняется проверка всех двойных кластеров буквы с соответствующими по хешу
			   двойными кластерами текста по  1,2 и 3 lookup.
			6. При превышении порога корреляции выполняется вычисление масштаба и выполняестя вычисление корреляции по 4 lookup.
			7. При превышении порога корреляции гипотеза буквы заносится в массив гипотез букв.
			8. Все кластеры в пределах габарита распознанной буквы заносятся в таблицу распознанных кластеров.
			   В таблице учитывается id номер гипотезы буквы, корреляция и расстояние от центра кластера до центра гипотезы буквы.
			9. Все буквы в которых есть базовые кластеры в пределах габарита области поиска проверяем на наличие максимального
			   набора базовых кластеров. Буквы в которых есть большинство базовых кластеров области поиска проверяем по 1,2,3,4 lookup
	 4. Найденные гипотезы букв поступают на сборку пятерок.

	 При работе алгоритма большинство проверок выполняется по заранее вычисленным таблицам.
	 Испольемые таблицы:

	 1. GVector *fontClusterTable;
	 это таблица базовых кластеров, строка таблицы заполняется информацией о буквах в которых найден базовый кластер этойм строки.
	 Таблица заполняется в @fn buildOCRFontClusterTable()

	 2. ClusterMatch *clusterMatchArray
	 //таблица в которой записаны базовые кластеры сортированные по наибольшему габариту буквы в которой он найде
	 Таблица заполняется в @fn buildOCRFontClusterTable() и хранится в fontClusterTable

	 3. GVector *letterClusterTable;    ///< таблица базовых кластеров буквы.
	 каждая строка заполняется набором базовых кластеров одной буквы.
	 Таблица заполняется в @fn buildOCRFontClusterTable()

	 4. vector<vector<int> >hashArray;
	 таблица в которой по номеру хеша записаны все номера OCRStar текста имеющие такое значение хеша

	 5. vector<vector <int> > textCluster;
	 таблица в которой каждая строка это список OCRStar текста котороые соответсвуют базовому кластеру
	 по 1,2,3 lookup

	 6. vector<vector <int> > text2Cluster;
	 таблица в которой каждая строка это список базовых кластеров для одной OCRStar текста

	 7. vector<int> searchStar;
	 список номеров OCRStar текста которые находятся в области поиска наибольшего габарита гипотезы буквы


	 TO_DO list
	 Написать проверку геометрии буквы и применение 4 lookup
	 Написать интерфейс распознавания текста
	 Занесение новой буквы в базу
	 гроздья винограда гипотез букв текста
	 редактирование новой буквы

	 Написать техзадание матрицы условных переходов буквы.
	 - разработать формат хранения кластеров в матрице

	 */

	//int print = 0;

	if (1) {
		
	
		cout<<"start"<<endl;
		//font.buildOCRFont("/OSBL/LETTERS");
        //font.updateOCRFont("/OSBL/LETTERS");
		//font.buildOCRFont("FONT.zip");
		cout<<"font.size:"<<font.size()<<endl;
		buildOCRFontClusterTable(1);
		//buildAlphabet();
		return;
		
		
		//cout<<" font.baseClusterArray.size():"<<font.baseClusterArray.size()<<endl;
		//for(int i=0;i<font.baseClusterArray.size();i++){
		//	draw.printCluster((void *)&font.baseClusterArray[i]);
		//}return;                            
	
		//font.printLetterSVG(1);return;
	
		/*
		for(int i=0;i<font.clusterArray.size();i++){
			draw.printClusterSVG((void *)&font.clusterArray[i], "/OSBL/CORRELATION/");
		}
		OCRCluster &cluster= font.clusterArray[0];
		OCRCluster &clusterT= font.clusterArray[8];
		OCRStar &star = cluster.star[0];
		OCRStar &starT = clusterT.star[0];
		
		int c=OCRStarCorrelationLookup2(starT, star,clusterT.line, cluster.line,NULL,NULL,0);
		cout<<"corr:"<<c<<endl;
		exit(0);
		*/
		
		//unlink("/storage/emulated/0/_Image2OCR/OCRData/OCRData/FONT_OCR.bin");
		//openFontOCR("/storage/emulated/0/_Image2OCR/OCRData/OCRData/FONT_OCR.bin");
		
		
		// buildFont("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_test/");
		// buildFont("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_img_tib_vowel/");
		// buildFont("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_img_tib/");
		// buildFont("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_2/");
		// buildFont("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/_FONT_SOURCE/");
		
		
		buildOCRFontClusterTable(1); return;   //теперь у нас есть практически все данные для корреляции букв.
		
		
		//buildOCRFont("/storage/emulated/0/MainYagpoOCR/LETTER");
		//return;
		string dataStr_;
		OCRLetter letterTest=font[1];
		cout<<"letterTest:"<<letterTest.name.data<<endl;
		//drawLetterSVG(letterTest, dataStr_);
		//writeText(dataStr_, "/storage/emulated/0/_Image2OCR/___graph1.html");
		//return;
		// drawFont();
		// for(int i=0;i<10;i++) {
		OCRLetter letter=font[0];
		//readLetterByID(letter, 1576662852);]
		cout<<"letter:"<<letter.name.data<<endl;
		//drawLetterSVG(letter, dataStr_);
		//writeText(dataStr_, "/storage/emulated/0/_Image2OCR/___graph2.html");
		//OCRLetter letterTest;
		// readLetter(letter,107);
		const char *path="/storage/emulated/0/_Image2OCR/CORRELATION/" ;
		//inputData.data["ocrData"] = "starCorrelation";
		//inputData.data["star"]="0";
		//inputData.data["starT"]="2";
		
		if (inputData.data["ocrData"] == "starCorrelation") {
			int indexStar=atoi(inputData.data["star"].c_str());
			int indexStarT=atoi(inputData.data["starT"].c_str());
			cout<<" 1 text starCorrelation starT:" <<indexStarT<<" star:" <<indexStar << " path:"<<path<<endl;
			OCRCluster &cluster=font.clusterArray[letter.cluster[indexStar]];
			OCRCluster &clusterT=font.clusterArray[letterTest.cluster[indexStarT]];
			/*
			uint64 reg0 = star.reg0_0 & starT.reg0_1;
			//вычисляем поисковый регистр углов  лимитированных фокальных линий без учёта коротких линий (с неопределённым углом)
			uint64 reg1 = star.reg1_0 & starT.reg1_1;
			cout<<bitset<64>(reg0)<<endl;
			cout<<bitset<64>(reg1)<<endl;
			if (reg0 == star.reg0_0 || reg1 == star.reg1_0) cout<<"@match"<<endl;
			*/
			//return;
			
			//clusterMaskCorrelation(starT.maskC,star.maskC);
			//return;
			OCRStarCorrelationLookup2(clusterT.star[0], 
														cluster.star[0], 
														clusterT.line, 
														cluster.line,
														NULL,NULL,0);
			/*										    
			drawOCRStarCorrelation(clusterT.star[0], 
														cluster.star[0], 
														clusterT.line, 
														cluster.line ,
													  path);
     */													  
		}else{
			//letterClusterCorrelation(letterTest,letter);  
			//drawLetterCorrelationSVG(letterTest,letter, path);
			return;
		}
		
		/*
		GBitmap *img = GBitmap::create("/OSBL/LETTERS/_རྣ_1576664365.png");
		img->crop32();
		img->binarisation64();
		GGraph graph_1(img);
        //img->printToScreenBinary();
		img->destroy();
		graph_1.graphFocalLine(15);
		//graph_1.printToScreenBinary();
		graph_1.focalPointXTL();
		graph_1.focalLineRecognition();
		graph_1.buildOCRStars();
		
		
		TString st;
		graph_1.saveToTStr(st);
		GGraph graph_2(st);
		cout<<"graph_2.columns():"<<graph_2.columns()<<endl;
		GDraw draw_;
		draw.printGraphSVG((void *)&graph_2,"/OSBL/CORRELATION");
		cout<<"end draw"<<endl;
		return;
		*/
		
		
		
		//letterTest.status = 255;

		//string dataStr_;
		//drawLetterSVG(letter, dataStr_);
		//writeText(dataStr_, "/storage/emulated/0/_Image2OCR/___graph2.svg");

		//readLetterByID(letter1, 1575965320); // 1570636506 -zha  // 1569375393  1570637085-au 1570635965-k+sha //1570635946 - kya
		//drawLetterClustersSVG(letter1, "/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/_clusters.svg");
		//drawLetterCorrelationSVG(letter, letterTest, "/storage/emulated/0/_Image2OCR/CORRELATION/");
		cout << "done draw";
		cout << font.size();
		return;
	}


} //___________________________________________________________

} // namespace ocr
