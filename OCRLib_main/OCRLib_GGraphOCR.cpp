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
/*  Implemmentation  to GMemoryLib dynamic library  */

//#include "OCRLib_GGraphOCR.h"
#include "OCRLib.h"
#include "../libocr/config.h"
#include "../libocr/OCRTypes/OCRTypes.h"
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#ifdef MACOS
   #include <malloc/malloc.h>
#endif

#include "../libocr/GMemory/GMemory.h"
#include "../libocr/GGraphOCR/GGraphOCR.h"
#include "../libocr/OCRString/php2stl.h"

#ifdef ESP
   #include "esp_command.h"
#endif

//#define DB_NAME "LIBRARY_CATALOG"
//#define DB_NAME "DHARMABOOK"

using namespace std;


void initCommandDataGraph() {
  inputData.data["root"] = "/OSBL/";
  cout << "init" << inputData.init << endl;
  readPreferences();
  inputData.init = 1;
}
  
void OCRletterProcess(void *conn, char *buff, uint letterID) {
    /*
    GGraphOCR graph;
    string path=ROOT_PATH;
    path+="/OCRData/OCRData/FONT_OCR.bin";
    graph.openFontOCR(path.c_str());
    OCRLetter letter;
    graph.readLetterByID(letter, letterID);
    ///cout<<letterID<<" star.count:"<<(int)letter.starCount<<" name:"<<letter.name.data<<endl;
    //TString st;
    //graph.letterToStr(letter,st);
    //string dataStr;
    //st.exportHex(dataStr);
    string dataStr;
    graph.drawLetterSVG(letter,dataStr);
#ifdef ESP
    espRenderBlock((HttpConn *)conn, &dataStr[0], dataStr.size());
#endif
	*/
}//______________________________________________________________________________________

   
void letterSVG(void *conn, char *name, char *drawBuff, uint letterID){
	if(!inputData.init)initCommandDataGraph();
	GGraphOCR graph;
    cout<<"start font:"<<graph.font.size()<<endl;
    
    
    //graph.openFontOCR("/MainYagpoOCR/_OSBL/dist/root/FONT_OCR/FONT_OCR.bin");
    //string fontPath=ROOT_PATH;
    //fontPath+="/OCRData/OCRData/FONT_OCR.bin";
    //cout<<"@test"<<fontPath<<endl; return;
    //graph.openFontOCR(fontPath.c_str());
    
	OCRLetter letter=graph.font.letterByID(letterID);
	string dataStr, dataStr_;
	if(letter.index!=0xFFFFFFFF){
		//cout<<"read letter:"<<letterID<<endl;
		//cout<<"letterIndex:"<<letter.letterIndex<<endl;
		//cout<<"name:"<<letter.name.data<<endl;
	    graph.font.drawLetterSVG(dataStr,letter.index); 
		dataStr_=letter.name.data;
	}else{
		dataStr="Not this letter in OCR font. Rebuild OCR font";
		cout<<dataStr<<endl;
		dataStr_="undefined"; 
	}
	    //cout<<"done draw. size:"<<dataStr_.size()<<endl;
		memcpy(drawBuff,&dataStr[0],dataStr.size());
		memcpy(name,&dataStr_[0],dataStr_.size());
	
}    

    
void renderLetterByID(unsigned int letterID, void *drawStr_){
    string &drawStr=*(string*)drawStr_;
    GGraphOCR graphOCR;
    OCRLetter &letter=graphOCR.font.letterByID(letterID);
    if(letter.index!=0xFFFFFFFF){
        graphOCR.draw.printLetterSVG((void*)&letter, (void*)&graphOCR.font, "/OSBL/CORRELATION_2/");
    }else{
        drawStr="no letter by ID";
        return;
    }
    
    

}


void drawTwoLettersCorrelation(
				void *conn, char *drawBuff, uint index1, uint index2) {

//http://127.0.0.1:8080/OSBL_LetterCorrelation.tpl?index1=1576660935&index2=1577621820
//http://127.0.0.1:8080/OSBL_LetterCorrelation.tpl?index1=1576660879&index2=1577738066
    GGraphOCR graph;
 /*   
    OCRLetter letter;
    OCRLetter letterTest;
    string fontPath=ROOT_PATH;
    fontPath+="/OCRData/OCRData/FONT_OCR.bin";
    graph.openFontOCR(fontPath.c_str());
    graph.readLetterByID(letterTest, index1);
    graph.readLetterByID(letter, index2);
    graph.letterClusterCorrelation(letterTest,letter);
    string path=ROOT_PATH;
    path+="/CORRELATION/";
	graph.drawLetterCorrelationSVG(letterTest,letter, path.c_str());
   */ 
}//______________________________________________________________________________________
int rebuildOCRLetterFont (cstr fontPath){
	/*
	GGraphOCR graph;
	string path=ROOT_PATH;
	path+="/OCRData/OCRData/FONT_OCR.bin";
	unlink(path.c_str());
	graph.openFontOCR(path.c_str());
	graph.buildOCRFont(fontPath);
*/
	return 1;
}















