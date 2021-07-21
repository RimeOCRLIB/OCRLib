// C-
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org .
// C-
// C- This program is distributed in the hope that it will be useful,
// C- but WITHOUT ANY WARRANTY; without even the implied warranty of
// C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// C- GNU General Public License for more details.
// C-
#include "GGraphOCR.h"

static bool sortLetterX(const OCRPoint &d1, const OCRPoint &d2) { return d1.x < d2.x; }
static bool sortC1_(OCRLetter &d0,OCRLetter &d1){
    return d0.correlationNew > d1.correlationNew;
}


/** @brief функция вычисления обратной корреляции кластеров области текста найденной гипотезами группы букв
    и каждой из гипотез букв этой области
 */
void GGraphOCR::backwardCorrelation(vector<OCRLetter> &matchLine, GGraph &graph) {
    
    /* 
     В том случае если буквы вложены друг в друга и меньшая буква имеет меньшую обратную корреляцию оставляем букву с большим количеством
     соответствующих кластеров. Например в букве В есть две небольшие буквы O с меньшей корреляцией.
     Для этого сравниваем каждую гипотезу буквы с совокупностью всех кластеров текста найденнной в этом месте текста 
     гипотезами букв.
     Для расчета величины обратной корреляции испоьзуем формулу k1 = (100*nOn/tCount + correlation)/2
     где tCount - суммарная длина линий области текста, nOn - длина найденых линий текста, correlation - корреляция буквы
     
     алгоритм 14.04.2021
     */
    
    //алгоритм 14.04.2021

    int print = 0;
    if (printCorrelation) { print = printCorrelation; }
	//int dX_ = 15;
    if(!matchLine.size())return;
    int tCount = matchLine[0].clusterCount * LINE_SIZE * 2; // - количество кластеров области текста,
    
	for(int i = 0;i<matchLine.size();i++){  //cout<<"i = "<<i<<" c = "<<matchLine[277].correlation<<endl;
		OCRLetter &letterTest = matchLine[i];
        int nOn = letterTest.matchLineSum; // - длина линий текста соответствующая найденным кластерам буквы,
        
        //int tCount = matchLine[0].clusterCount + 2; // - количество кластеров области текста,
        //int nOn = letterTest.matchLineCount; // - количество найдных кластеров буквы,
        //int nOff = letterTest.lineCount-nOn; // - количество не найденых кластеров буквы
        
        
        letterTest.correlationNew = (nOn*50/tCount + letterTest.correlation*1.5)/2;

		//if(!letterTest.correlation)continue;
        //DR(" @/sort/________________________ letter:"<<letterTest.name.data<<" id:"<<letterTest.letterID <<" nOn:"<<nOn<<" nOff:"<<nOff
        //   <<" tCount:"<<tCount<<" corr:"<<letterTest.correlation<<" correlationNew:"<<letterTest.correlationNew<<endl);
	}
    sort(matchLine.begin(), matchLine.end(), sortC1_);
    DR("//____all matchLine.size():"<<matchLine.size()<<endl;)
    
    if(print){
        for(int i = 0;i<matchLine.size();i++){  //cout<<"i = "<<i<<" c = "<<matchLine[277].correlation<<endl;
            OCRLetter &letterTest = matchLine[i];
            //int tCount = matchLine[0].clusterCount * LINE_SIZE * 4; // - количество кластеров области текста,
            int nOn = letterTest.matchLineSum; // - количество найдных кластеров буквы,
            //if(!letterTest.correlation)continue;
            DR("        @/sort/________________________ letter:"<<letterTest.name.data<<" id:"<<letterTest.letterID <<" nOn:"<<nOn
                  <<" tCount:"<<tCount<<" scale:"<<letterTest.scale<<" corr:"<<letterTest.correlation<<" correlationNew:"<<letterTest.correlationNew<<endl);
            if(i == 35)break;
        }
    }
    return;

	// drawGrapeLine(matchLine); exit(0);

}

void GGraphOCR::buildTextLine(vector<OCRLetter> &matchLetter, GGraph &graph, string &mainString) {

    int print = 0;
    if (printCorrelation) { print = printCorrelation; }
    
	vector<vector<int> > textLines;
	int h = graph.rows();
	textLines.resize(h+128);
	uint size = (uint)matchLetter.size();
    
    
    //for(int i=0;i<strArray.size();i++){
    //    OCRPoint &p=strArray[i];
    //    cout<<"y0:"<<p.x<<" y1:"<<p.y<<endl;
    //}
    //cout<<endl;
    
    //все буквы в пределах лимита выписываем в отдельный массив
    
    for(int stringIndex=0;stringIndex < strArray.size(); stringIndex++){
        
        OCRBox &strPoint=strArray[stringIndex];
        int yUp = strPoint.yLimit0;
        int yDown=strPoint.yLimit1;
    
        vector<OCRPoint> lineArray;
        for (int i = 0; i < size; i++) {
            OCRLetter &letter = matchLetter[i];
            if (!letter.status) continue;
            
            if (letter.yMatch < yDown && letter.yMatch > yUp) {
                OCRPoint d;
                d.data = i;
                d.x = letter.xMatch;
                lineArray.push_back(d);
                letter.status = 0;
            }
        }
        //сортируем результат
        sort(lineArray.begin(), lineArray.end(), sortLetterX);
        
        cout << " ";
        for (int i = 0; i < lineArray.size(); i++) {
            OCRLetter &letter = matchLetter[lineArray[i].data];
            mainString += letter.name.data;
            // cout<<letter.xMatch<<" letterID:"<<letter.letterID<<" c:"<<letter.correlation<<" name:"<<letter.name.data<<endl;
            // cout<<" ";
        }
       
        mainString += "\n";
    }
    
    mainString=str_replace("/", "", mainString);
    mainString=str_replace("/", "", mainString);
    mainString=str_replace("/", "", mainString);
    mainString=str_replace("#", "", mainString);
    mainString=str_replace("]", "", mainString);
    mainString=str_replace("］", "", mainString);
    mainString=str_replace("[", "", mainString);

/*
	for (int i = 0; i < size; i++) {
		OCRLetter &letter = matchLetter[i];
		if (!letter.correlation || letter.yMatch<0) continue;
        letter.status = 1;
		textLines[letter.yMatch].push_back(i);
	}
    
    
	vector<int> lines;
	lines.resize(h);

	vector<int> linesGauss;
	linesGauss.resize(h);

	for (int i = 0; i < h; i++) { lines[i] = (int)textLines[i].size(); }
	//усредняем значения
	for (int i = 10; i < h - 10; i++) {
		linesGauss[i] = lines[i - 1] + lines[i - 2] + lines[i - 3] + lines[i - 4] + lines[i - 5] + lines[i - 6] + lines[i - 7] + lines[i - 8] + lines[i - 9] + lines[i + 1] +
						lines[i + 2] + lines[i + 3] + lines[i + 4] + lines[i + 5] + lines[i + 6] + lines[i + 7] + lines[i + 8] + lines[i + 9];
	}

	int flag = 1;
	int y = 0;
	while (flag) {

		flag = 0;
		for (int i = 10; i < h - 10; i++) {
			// cout<<linesGauss[i]<<" ";
			if (!linesGauss[i]) continue;
			if (linesGauss[i] >= linesGauss[i + 3] && linesGauss[i] >= linesGauss[i - 3] && linesGauss[i] >= linesGauss[i + 7] && linesGauss[i] >= linesGauss[i - 7] &&
				linesGauss[i] >= linesGauss[i + 9] && linesGauss[i] >= linesGauss[i - 9]) {
				cout << "line y:" << i << " linesGauss[i]:" << linesGauss[i] << endl;
				flag = 1;
				y = i;
				break;
			}
		}

		if (!flag) break;

		//все буквы в пределах лимита выписываем в отдельный массив
		vector<OCRPoint> lineArray;
		for (int i = 0; i < size; i++) {
			OCRLetter &letter = matchLetter[i];
			if (!letter.status) continue;
			if (letter.yMatch > y - 30 && letter.yMatch < y + 30) {
				OCRPoint d;
				d.data = i;
				d.x = letter.xMatch;
				lineArray.push_back(d);
                letter.status = 0;
			}
		}
		//сортируем результат
		sort(lineArray.begin(), lineArray.end(), sortLetterX);

		cout << " ";
		for (int i = 0; i < lineArray.size(); i++) {
			OCRLetter &letter = matchLetter[lineArray[i].data];
			mainString += letter.name.data;
			// cout<<letter.xMatch<<" letterID:"<<letter.letterID<<" c:"<<letter.correlation<<" name:"<<letter.name.data<<endl;
			// cout<<" ";
		}
		for (int i = y - 50; i < y + 50 && i < h; i++) {
			if (i < 0) continue;
			linesGauss[i] = 0;
		}
		// break;
		mainString += "\n";
	}
 */
 
	cout << mainString << endl<<endl;
}


void GGraphOCR::checkFontGrammar(vector<OCRLetter> &textMatch,OCRLetter &letterTest){
    
    /*
     Проверка шрифтовой грамматики найденной буквы. 
     В том случае если буква коренная, записываем ее в результат распознавания. 
     Если найденная буква это добавление коренным буквам - например подписные и надписные буквы,
     или знаки препинания то проверяем соответствие добавочной буквы и коренной буквы по геометрии и шрифтовой грамматике.
     В случае если найденная буква соотвествует грамматике, присоединяем найденную букву к коренной букве.
     В результате работы функции в результат распознавания записываются только те малые формы которые которые находят соответствие.
     Такая проверка позволяет сократить количество эталонов OCR шрифта и снизить зашумление результата распознавания малыми графическими формами.
     */
    
    //if(letterTest.letterID==1619970670){
    //    cout<<1;
    //}
    //textMatch.push_back(letterTest); return;
    
    
    string name=letterTest.name.data;
    if(name=="།" || name=="］"){
        letterTest.OCRIndex=LETTER_PHRASE_PUNCTUATION;
    }
    
    //if(name=="༽"){
    //     cout<<1;
    //}
    
    int OCRIindex = letterTest.OCRIndex;
    int flag=0;
    switch (OCRIindex) {
        case LETTER_TIB_MANUSCRIPT:
            letterTest.status = 1;
            break;
        case LETTER_TIB:
            letterTest.status = 1;
            break;
        case LETTER_PUNCTUATION_ALONE:
            letterTest.status = 1;
            break;
        case LETTER_PHRASE_PUNCTUATION:{
            int minXY_left = 1000;
            int minXY_right = 1000;
            int minIndex_left = -1;
            int minIndex_right = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dX > 0 && dXY < minXY_left){
                    minXY_left = dXY;
                    minIndex_left = i;
                }
                if(dX < 0 && dXY < minXY_right){
                    minXY_right = dXY;
                    minIndex_right = i;
                }
            }
            if(minIndex_right >-1 && minXY_right < minXY_left){
                OCRLetter &letterRoot = textMatch[minIndex_right];
                if(minXY_right < letterRoot.letterW * 1.5){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                    flag=1;
                }
            }else if(minIndex_left >-1 && minXY_left < minXY_right){
                OCRLetter &letterRoot = textMatch[minIndex_left];
                if(minXY_left < letterRoot.letterW * 1.5){
                    //приписываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_LEFT);
                    if(name=="］"){
                        string rootName=letterRoot.name.data;
                        rootName = str_replace("］", "།", rootName);
                        letterRoot.name=rootName;
                    }
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_PUNCTUATION:{
            int minXY = 1000;
            int minIndex = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dX < 0 && dXY < minXY){
                    minXY = dXY;
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minXY < letterRoot.letterW * 2 && abs(letterRoot.yMatch - letterTest.yMatch) < letterRoot.letterH / 4){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_BOTTOM_VOWEL:{
            int minXY = 1000;
            int minIndex = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dY < 0 && dXY < minXY){
                    minXY = dXY;
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minXY < letterRoot.letterH){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_LEFT_PUNCTUATION:{
            int minXY = 1000;
            int minIndex = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dX > 0 && dXY < minXY){
                    minXY = dXY;
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minXY < letterRoot.letterW){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_LEFT);
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_RIGHT_PUNCTUATION:{
            int minXY = 1000;
            int minIndex = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dX < 0 && dXY < minXY){
                    minXY = dXY;
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minXY < letterRoot.letterW && abs(letterRoot.xMatch - letterTest.xMatch) <  letterRoot.letterW/4){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_UPPER_RIGHT_PUNCTUATION:{
            int minXY = 1000;
            int minX = 1000;
            int minIndex = -1;
            
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = letterTest.xMatch - textMatch[i].xMatch - textMatch[i].letterW /2;
                int dXY = abs(dY) + abs(dX);
                if(dXY < minXY){
                    minXY = dXY;
                    minX = abs(dX);
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minX < letterRoot.letterH * letterRoot.scale/2 &&
                   minXY < letterRoot.letterH * letterRoot.scale &&
                   letterRoot.yMatch > letterTest.yMatch){
                    //припысываем знак препинания к коренной букве
                    font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                    flag=1;
                }
            }
            letterTest.status = 0;
            break;
        }
        case LETTER_UPPER_VOWEL:{
            int minXY = 1000;
            int minIndex = -1;
            for(int i = 0; i<textMatch.size();i++){
                int dY = textMatch[i].yMatch - letterTest.yMatch;
                int dX = textMatch[i].xMatch - letterTest.xMatch;
                int dXY = abs(dY) + abs(dX);
                if(dY > 0 && dXY < minXY){
                    minXY = dXY;
                    minIndex = i;
                }
            }
            if(minIndex>-1){
                OCRLetter &letterRoot = textMatch[minIndex];
                if(minXY < letterRoot.letterH && abs(letterRoot.xMatch - letterTest.xMatch) <  letterRoot.letterW/4){
                    //проверяем шрифтовую грамматику тибетского языка
                    string nameRoot=letterRoot.name.data;
                    int flagMark=0;
                    if(nameRoot.find("ུ") != -1 || nameRoot.find("ཱ") != -1)flagMark=1;
                    if(nameRoot.find("ི")!=-1 && name=="ེ")flagMark=1;
                    if(nameRoot.find("ེ")!=-1 && name=="ི"){
                        flagMark=1;
                        nameRoot = str_replace("ེ", "ི", nameRoot);
                    }
                    if(nameRoot.find("ི")!=-1 && name=="ི")flagMark=1;
                    if(nameRoot.find("ེ")!=-1 && name=="ེ")flagMark=1;
                    
                    if(!flagMark){
                        //припысываем знак препинания к коренной букве
                        font.joinLetters(letterRoot, letterTest,JOIN_RIGHT);
                        flag=1;
                    }
                }
            }
            letterTest.status = 0;
            break;
        }
        default:
            break;
    }
    
    if(!letterTest.status){
        //textMatch[textMatch.size()-1].name+="/";
        //textMatch.push_back(letterTest);
    }else{
        //if(!flag)
        textMatch.push_back(letterTest);
    }
    if(!letterTest.status && flag) letterTest.status = 1; //сообщаем что эта буква нашла соответствие
    
    
    

}


void GGraphOCR::drawCorrelationImg(vector<OCRLetter> &matchLetter, GGraph &graphPage, GBitmap *imgPage) {

	//uchar *p = imgPage->bytes_data;
	uint w = imgPage->columns();
	uint h = imgPage->rows();
	int size = (int)matchLetter.size();

	ostringstream out;

	out << "<?xml version = \"1.0\" encoding = \"UTF-8\" standalone = \"no\"?>\n\
    <svg width = \""
		<< w << "\" height = \"" << h << "\" viewBox = \"0 0 " << w << " " << h << "\"\n\
    xmlns = \"http://www.w3.org/2000/svg\" xmlns:xlink = \"http://www.w3.org/1999/xlink\">\n\
    <style> .c{stroke:blue; stroke-width:1}\n\
    .s0{fill:white;stroke:rgba(100,100,100,0.5);stroke-width:1}\n\
    .s1{fill:rgba(23,203,0,0.7);}\n\
    .s2{fill:rgba(255,124,0,0.7);}\n\
    .s3{fill:rgba(0,100,225,0.7);}\n\
    .s4{fill:rgba(183,0,145,0.7);}\n\
    .s5{fill:rgba(228,0,16,0.7);}\n\
    .L{stroke:rgba(100,100,100,0.5);stroke-width:1}\n\
    .L0{stroke:white;stroke-width:3}\n\
    .L1{stroke:rgba(23,203,0,0.7);stroke-width:3}\n\
    .L2{stroke:rgba(255,124,0,0.7);stroke-width:3}\n\
    .L3{stroke:rgba(0,100,225,0.7);stroke-width:3}\n\
    .L4{stroke:rgba(183,0,145,0.7);stroke-width:3}\n\
    .L5{stroke:rgba(228,0,16,0.7);stroke-width:3}\n\
    .L11{fill:none;stroke:rgba(228,0,16,0.7);stroke-width:1}\n\
    .lp{fill:rgba(128,128,175,0.2);}\n\
    .c0{fill:none;stroke:black;stroke-width:2}\n\
    .c1{fill:white;stroke:black;stroke-width:2}\n\
    .c2{fill:none;stroke:rgba(255,255,255,0.7);stroke-width:5}\n\
    .p0{fill:rgba(60,60,60,0.5);}\n\
    .p1{fill:rgba(160,160,160,0.5);}\n\
    .p2{fill:rgba(228,0,16,0.7);}\n\
    .mask{fill:none;stroke:black;}\n\
    .t0{font-family:Courier;font-size:11px;fill:gray}\n\
    .t1{font-family:Yagpo Tibetan Uni;font-size:32px;fill:blue}\n\
    .t2{font-family:Courier;font-size:11px;fill:black}\n\
    </style>\n";

	out << "<image href = \"རྒྱས_.png\" />\n";

	for (int i = 0; i < size; i++) {
		OCRLetter &letter = matchLetter[i];
		if (!letter.correlation) continue;
		if (letter.correlation > 85) {
			out << "<rect class = \"p1\" x = \"" << letter.s.x0 << "\" y = \"" << letter.s.y0 << "\" width = \"" << letter.s.x1 - letter.s.x0 << "\" height = \"" << letter.s.y1 - letter.s.y0
				<< "\"/>" << endl;
			out << "<text class = \"t1\" x = \"" << letter.s.x0 + 2 << "\" y = \"" << letter.s.y1 - 2 << "\">" << letter.name.data << "</text>" << endl;

			/*  for(int y = letter.s.y0;y<letter.s.y1;y++){
				  for(int x = letter.s.x0;x<letter.s.x1;x++){
					  if(!p[y*w+x]){
						  p[y*w+x] = 128;
					  }else{
						  p[y*w+x]- = 30;
					  }
				  }
			  }
			 */
		}
	}
	out << "</svg>";
	string str = out.str();
	writeText(str, "/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/letter_result/mainString.svg");
}
