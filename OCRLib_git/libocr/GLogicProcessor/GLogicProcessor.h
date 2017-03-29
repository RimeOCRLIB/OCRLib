//  OCRLib  2015 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  Will Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .


#ifndef _GLogicProcessor_H_
#define _GLogicProcessor_H_

#include "config.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>


#include "OCRTypes.h"
#include "GVector.h"
#include "GBitsetMatrix.h"
#include "GMap.h"
#include "GFont.h"
#include "pugixml.hpp"
#include "php2stl.h"


using namespace std;
using namespace pugi;


namespace ocr{
    
    static bool sortMatchXCenter(const OCRMatch& d1, const OCRMatch& d2){
        return d1.xCenter < d2.xCenter;
    }
    static bool sortMatchX0(const OCRMatch& d1, const OCRMatch& d2){
     return d1.xCenter-d1.letterW/2 < d2.xCenter-d2.letterW/2;
     }
     static bool sortKeyC(const keyOCR& d1, const keyOCR& d2){
     return d1.correlation > d2.correlation;
     }
    static bool sortMatchC(const OCRMatch& d1, const OCRMatch& d2){
        return d1.correlation > d2.correlation;
    }
    static bool sortMatchL(const OCRMatch& d1, const OCRMatch& d2){
        return d1.line.size() > d2.line.size();
    }
    static bool sortMatchW(const OCRMatch& d1, const OCRMatch& d2){
        return d1.letterW > d2.letterW;
    }
    
    
///класс содержит данные по языковой модели,методы словарного и грамматического контроля
class  GLogicProcessor {
protected:
  GLogicProcessor(void);
 public:
  virtual ~GLogicProcessor();
  void destroy(void);
  void init();
public:
    
//-----------------------------------------------------------------------------------------
	int print;
		ofstream c_out;
    
    GFont *aliKali;                 //OCRFont of text recognition
    vector<stringOCR>*strArray;     //page data of text OCR
    
    map<string, OCRDictRecord>mainDict; ///main map of dictionary records.

		
    
    GVector *textCorpus;            ///vector with all tibetan corpus text data
    GMap *textCorpusGMap;           ///map with all tibetan corpus text data
    GVector *fontTable;             ///main font data table. Read from XML LETTERS_GRAMMAR.xml
    GMap *fontGMap;                 ///main font data map. Builded from fontTable. Keys it is unicode of letters.
    GMap *fontStackGMap;            ///main font data map. Builded from fontTable. Keys it is unicode of letters.
    GVector *mDict;
    GMap *mainDictIndex;
    GVector *translationDictRus;
    GMap *tDictRusIndex;
    GVector *translationDictEng;
    GMap *tDictEngIndex;
    
    
                                    //In stacks all letters transformed to capitals
	uint UniBigLetters[65000];      ///table for joined to capitals letter conversion
    
	map<string,vector <int> >dictIndexTib; ///< index map for all tibetan dictionary
	map<string,vector <int> >dictIndexPali; ///< index map for all pali dictionary
	map<string,vector <int> >dictIndexChesh; ///< index map for all Chesh dictionary
	map<int, OCRDictRecord> dictRecord; ///< main map for all dictionary
	vector<OCRDictRecord> dictReport; ///< vector for translation output
	vector<vector<OCRDictRecord> >dictReportPage;  ///<vector for multi-line translation output
	vector<string>tibStr;
	vector<string>wylieStr;
	vector<string>wylieStrVect;
	vector<string>yagpoStrVect;
	vector<string>strVector; //<<source text vector
	vector<string>delimeterTib; //<vector for all tibetan punctuation marks
	vector<string>grammarTib; //<common tibetan words. It is not used in search
	vector<string>delimeterPali; //<vector for all tibetan punctuation marks
	vector<string>grammarPali; //<common Plai grammar paricles
	vector<string>commentaryIndex; //<commentary index in text
    vector<uniLetterRecord>mettaPali;

    vector<string>letterSign;                       ///<Array of vowel letters
    vector<string>letterScale;                      ///<Array of scale letters
	
	vector<string>inputVect;
	vector <string> stringItemVector;
		

	map<string,int>fontNameMap;  ///<map of tibetan font names 
	map<string, uniRecord>uniTibTable; ///<map of tibetan letters conversion in Unicode and UTF
	map<string, uniRecord>uniTibTableYagpo;  ///<main map of tibetan letters in codepage Uni<->Yagpo
	map<string,string>ASCIToUni;    ///<map of tibetan letters in Unicode and ASCI keyboard layout
	map<string, uniRecord>YagpoToWylieMap; ///<map of Yagpo UTF To Wylie conversion
	map<string, uniRecord>UniToWylieMap; ///<map of Uni To Wylie conversion
	map<string,string> SinhalaASCI;  //map for Sinhala font convertion function  
	vector<uniRecord>uniTibTable0F00;
	vector<string>TibetanAlphabet;
	map<string,map<string,uniRecord> >fontMap;
	
	map<string,map<string,string> > mainLetterTableUni; ///base letter table for all letter conversion function in Uni key
	map<string,map<string,string> > mainLetterTableYagpo; ///base letter table for all letter conversion function in Yagpo key
	map<string,map<string,string> > mainLetterTableKey; ///base letter table for all letter conversion function in typing key
	map<string,map<string,string> > mainLetterTableOCRKey; ///base letter table for all letter conversion function in OCR
    
	char *hashData0,*hashData1;  ///hashed index for all words and phrases (about 600mb) this data it is all adress space of integer. Every integer it is uniq hash of string
	///integer it is index of one bit in hashData block of memory. Value of this bit it is exist this hash or not. 
    hashRecord *hashVector; ///second level hash index. Every adress in hashRecord it is offset in text buffer.
	unsigned long hashSize; //size of second level hash index. 
	vector<stringEntry> textKeyIndex; ///vector for all words and phrases key without grammar particals and words delimeters(about 600mb);
	
	char *textBuffer;     //main text buffer of grammar dictionary
    char *textDictTib;    //main text buffer of Tibetan grammar dictionary
    char *textDictRus;    //main text buffer of Russian grammar dictionary
    char *textDictEng;    //main text buffer of English grammar dictionary
    unsigned long textDictTibSize;
    unsigned long textDictRusSize;
    unsigned long textDictEngSize;
   
    int y0Base;           //координаты средней области распознаваемой строки текста относительно всей страницы.     
    int y1Base;
    
	vector<string> textDictionary; ///vector for all words and phrases;
	vector<string> textDictionaryKey; ///vector for all words and phrases key without words delimeters;	

	
	FILE *hFile1,*hFile2,*hFile3,*hFile4;  ///files of hash table for grammar coreector dictionary. Each chink is 4Gb and base on SSD drive
	
	int maxUniRecord;  ///<max length of tibetan staÏck letter in Unicode codepage
	string report;  //string for conversion errors output;
	
/*	map<string,string>fontMapLoaded;
		map<string, string>::const_iterator fontIndex;
	map<wstring, wstring>::const_iterator ASCIIndex;
	map<string,map<wstring,uniRecord> >::const_iterator fontIndexInMap;
	map<wstring,uniRecord>::const_iterator letterIndexInMap;
*/	
	int dictionaryReady;  //flag of dictionary loading
	
	/// @name Initialisation
//@{
	static GLogicProcessor* create(void) {return new GLogicProcessor;}
//}

//-----------------------------------------------------------------------------------------    
/// @name Language codepage conversion function
//@{
	/**  @brief load all conversion map from XML files */	void  LoadUniToTibetanMapUTF();
	/**  @brief load convrsion map for font from XML files */	void  LoadFontNameMap( map<string, string>&fontNameMap);
	/**  @brief main conversion function. all conversion parameter set in inputData structure */	 
	/** @brief load XML in to STL map	*/		
	void LoadMapXML();
	
	void loadTransliterationFile(const char *mapName);
    void readFontDataXML(string &path);
	
 	string  startConvert();
    
    /** @brief encode all data in vData by words ID. All remaining letters will be encoded as uint*/
    void encodeByID(string &vData);
    /** @brief apply regular expression list and check tibetan text by OCR phrase dictionary probability */
    string lineTibetanCorrector(string &srcLine);
    /** @brief apply regular expression list and check tibetan text file by OCR phrase dictionary probability */
    void TibetanCorrector();
    
    /** @brief convert file to phonetic transtiption */
	void Transcription();
    /** @brief convert string to phonetic transtiption */
    string lineTranscription(string &line);
	/** @brief convert string from Wylie transliteration to Yagpo UTF */
	string WylieToYagpoConverter(string &inStr);
	/** @brief convert string from delimeted Wylie transliteration to Yagpo UTF  (english {Wylie} english) */	
	string dWylieToYagpoConverter(string &inStr);
	/** @brief convert string from tibetan Unicode in UTF to Wylie transliteration */	string UniToWylieConverter(string &inStr);
	/** @brief convert string from delimeted Singhala ASCI font (FMAbhaya etc.) transliteration to Sinhala UTF  (english {Singhala ASCI} english) */	 
	string dSinhalaASCIToYagpo(string &inStr);
	/** @brief convert string from Singhala ASCI font (FMAbhaya etc.) transliteration to Sinhala UTF  (english {Singhala ASCI} english) */	 
	string SinghalaASCIToYagpo(string &uniStack);
	/** @brief convert string from Singhala Unicode font to Sinhala Yagpo precomposed Unicode codepage LETTERS_GRAMMAR.xml */
     string SinhalaUniToYagpo(string &uniStack,int mode);
	/** @brief convert string from CXS Extended Sanskrit codepage in Uni UTF8 string
	 this is fast conversion without copy string */
	 	
	void convertCXS_to_UTF_nocopy(string &inStr);
	/** @brief convert string from CXS Extended Sanskrit codepage in Uni UTF8 string
	 this is save conversion with copy string */
	 	
	string convertCXS_to_UTF(string &inStr);
	/** @brief convert string to lower case from Uni UTF8  Extended Sanskrit string
	 this is no save conversion with original string */
	 	
	void lowerCase_nocopy(string &inStr);
	
    void textNormalisation(string &dataStr);

	string TibUniToWylie(string &srcStr,int mode);
	
#ifdef COCOA
	void RTFtoYagpoConverter( string &mainString,string &path);
#endif	
	string UniToTibetan(string &uniStack, int YagpoWylieFontFlag);
	/** @brief convert mixed (Tibetan+other text)  string from Tibetan Unicode codepage in UTF code pages
	 in Yagpo precomposed Unicode */
    string UnicodeToYagpo(string &srcStr);
	/** @brief convert string 
	mode 1 - from Tibetan Unicode codepage in UTF code pages in Yagpo precomposed Unicode
	mode 2 - from Wylie keyboard typed key to  Yagpo precomposed Unicode */
	string tibetanUTFToYagpo(string &uniStack,int mode);
	string YagpoToUni(string &uniStack);
	string YagpoUniToTibetanBig(string &uniStack);
	void  LoadYagpoToWylieMap();
    void BonPDFToUni(string &srcStr);
	
	//}

//-----------------------------------------------------------------------------------------
/// @name Dictionary and text processing function
	//@{
    /** @brief check OCR hypotesis by font rules, grammar rules, dictionary, returns main string in HTML format */
    void grammarOCR(vector<OCRMatch>&pageText, string &mainString);
    
    
    /** @brief Returns main string in HTML format as result of grammar correction by dictionary base */
	void grammarCorrector(vector<stringOCR>&strArray,
                          vector<stringOCR> &correctionWordArray,
                          string &mainString,
                          string &xmlString);
    /** @brief Returns main string in HTML format as result of grammar correction by dictionary base */
	void grammarCorrector(string &lineString,
                          string &xmlString);
    
    /** @brief Returns main string in HTML format as result of grammar correction by dictionary base 
     проверка текста по словарю на точное совпадение с учетом статистики */
    int textCorrector(vector<OCRMatch>&pageText, string &mainString);
    bool isDelimeter(string &str);
    
    void pageSegmentation(vector<OCRMatch>&matchLine,vector<OCRFocalLine>focalLine,int pageW,int dY);
    
    void letterAssociation(vector<stringOCR>&strArray,
                         vector<OCRMatch>&matchLine,
                         vector<OCRMatch>&dLine,
                         GBitmap* lineImg32,
                         GBitmap* letterAImg,
                         GBitmap* letterBImg,
                         string &mainString,
                         int sizeLine,
                         int lineIndex);
    
    void classification(vector<stringOCR>&strArray,
                        vector<OCRMatch>&matchLine,
                        GBitmap* lineImg32,
                        string &mainString,
                        int sizeLine,
                        int lineIndex);

    /** @brief для каждой гипотезы распознанной буквы проверяем две соседние буквы
    оставляем только те буквы, которые имеют хороших соседей и мирно с ними уживаются.*/
    void letterNeighbors(vector<OCRMatch>&matchLine,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    void letterNeighborsNew(vector<OCRMatch>&matchLine,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    
    /** @brief функция определения площади буквы по количеству пикселов. Рисует букву A в letterAImg*/
    void  letterArea(OCRMatch &a,GBitmap* letterAImg);
    /** @brief функция определения количества пикселов фокальных линий изображения находящихся внутри изображения буквы. Рисует букву A в letterAImg*/
    void  letterLineArea(OCRMatch &a,GBitmap *lineImg32, GBitmap *letterAImg);
    /** @brief функция проверки площади пересечения двух букв по количеству общих точек. 
               Буква А должна быть нарисована в letterAImg, letterBImg затирается*/
    int  intersectionArea(OCRBox *dataBox, GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    /** @brief функция проверки площади пересечения двух букв по количеству общих точек. letterAImg, letterBImg затирается*/
    int  stackIntersectionArea(OCRMatch &a, OCRMatch &b,OCRBox *dataBox, GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    
    void letterConstruction(vector<OCRMatch>&letterLine,int OCRMode);
    void compactResultOCR(vector<OCRMatch>&letterLine,string&OCRresult);
    
    /** @brief Функция сборки всех стековых букв, которые можно предположить в строке.
    морфемой может быть слог, слово или фраза - это определяется величиной словаря
    этот этап выполняется с на базе словаря стековых букв в формате GMap
    функция зависит от шрифтовой грамматики языка и выполняется по-разному
    для вертикальных и горизонтальных стеков*/
    void collectStackLetter(vector<stringOCR>&strArray,
                            vector<OCRMatch>&letterLine,         //результаты распознавания с проверенным letterProbability
                            vector<OCRMatch>&line,               //результаты распознавания с проверенными сочетаниями букв (letterConstruction)
                            GBitmap* lineImg32,
                            GBitmap* letterAImg,
                            GBitmap* letterBImg,
                            int lineIndex);
    
    //компрессия. Все одинаковые буквы в пределах габарита буквы
    //заменяются на одну букву с макcмимальной корреляцией  
    void compressMatch(vector<OCRMatch>&letterLine);
    void compressMatchPrint(vector<OCRMatch>&letterLine);
    
    //расчет вероятности буквы на основании взаимной корреляции всех букв, найденных на месте тестовой буквы
    void letterProbability(vector<OCRMatch>&matchLine);

    //формирует массив букв OpenType с координатами соответсвующими позиции элемента стековой буквы
    //относительно родительской буквы
    void buildOpenType(vector<OCRMatch>&line,vector<OCRMatch>&letterLine);
    
    /** @brief  функция проверки взаимного положения пары букв  */
    void testStackLetter(vector<OCRMatch>&line,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    void testStackLetterNew(vector<OCRMatch>&line,vector<OCRMatch>&originalMatch,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg);
    /** @brief  функция сборки пар букв для корреляционного поиска.
     Пары собираются из стеков, полученных в результате функции collectStackLetter
     пара содержит две буквы Unicode. Пары формируются с перекрытием (см. associativeSignalProcessing)
     для каждого стека последняя пара букв содержит одну букву из соседней стековой буквы с проверкой по шрифтовой геометрии
    */ 
    void buildSearchString(vector<OCRMatch>&line,    //исходный вектор букв
                           vector<OCRMatch>&dLine,   //вектор пар букв
                           vector<uint>&letterX,   //массив геометрических координат пар букв
                           string &result);
    
    void buildSearchStringNew(vector<OCRMatch>&line,    //исходный вектор букв
                           vector<OCRMatch>&dLine,   //вектор пар букв
                           vector<uint>&letterX,   //массив геометрических координат пар букв
                           string &result,
                           GBitmap *lineImg32,
                           GBitmap* letterAImg,
                           GBitmap* letterBImg);
    
   /** @brief на этом этапе в результатах словаря около 70 результатов претендующих на один и тотже слог.
    //производим разбор результатов словаря по отношению к исходному тексту
    //сравниваем насколько точно каждая словарная гипотеза описывает данные текста и сравниваем гипотезы между собой.*/
    void  testWordLine(GBitmap *lineImg32,
                       vector<OCRMatch>&line,
                       vector<OCRMatch>&originalMatch);
    
    /** @brief  функция сборки пар букв вместе с ответами словаря. ответы словаря собираются из пар букв,
        затем ответы которые содержат общую часть собираются попарно. Ответы меньшие по длинне чем соединенные ответы
        исключаются из рассмотрения. Получившиеся фразы дополняются исходными парами букв из которых
        составляются фразы на тех участках основной фразы, которые не перекрыты ответами словаря*/
    void renderDictSearchNew(map<vector<int>,ulong>&searchResult,
                             vector<OCRMatch>&dLine,
                             vector<OCRMatch>&originalMatch,
                             vector<OCRMatch>&pageText,
                             GBitmap *lineImg32,
                             GBitmap* letterAImg,
                             GBitmap* letterBImg);
    
    void renderDictSearchSanskrit(map<vector<int>,ulong>&searchResult,
                             vector<OCRMatch>&originalMatch,
                             GBitmap *lineImg32,
                             GBitmap* letterAImg,
                             GBitmap* letterBImg);
    
    /** @brief  функция сборки пар букв вместе с ответами словаря. ответы словаря собираются из пар букв,
     затем ответы которые содержат общую часть собираются попарно. Ответы меньшие по длинне чем соединенные ответы
     исключаются из рассмотрения. Получившиеся фразы дополняются исходными парами букв из которых
     составляются фразы на тех участках основной фразы, которые не перекрыты ответами словаря*/
    void renderDictSearch(map<vector<int>,ulong>&searchResult,
                          vector<OCRMatch>&dLine,
                          vector<OCRMatch>&originalMatch,
                          vector<OCRMatch>&pageText);
    
    /** @brief  функция сборки фразы в  renderDictSearch*/
    void sentenceConstructur(vector<OCRMatch>&wLine);
    /** @brief  функция сборки фразы и знаков препинания в  renderDictSearch*/
    void sentenceConstructurNew(vector<OCRMatch>&matchLine,
                                vector<OCRMatch>&originalMatch,
                                GBitmap* lineImg32,
                                GBitmap* letterAImg,
                                GBitmap* letterBImg);
    
    /** @brief  //сборка пар букв вместе с ответами словаря. Часть функции renderDictSearch*/
    void collectTextLine(vector<OCRMatch>&originalMatch);
   
    
    void drawMatchLine(vector<OCRMatch>&line);       //вывод результатов графического распознавания в HTML
    void drawGrapeLine(vector<OCRMatch>&line);       //вывод результатов графического распознавания в HTML в форме «гроздьев винограда»
    
	/** @brief function for check spelling of tibetan text */
	string spellCheck( string &str_);
    
    void saveMatch(vector<OCRMatch>&line,const char *path);
    void readMatch(vector<OCRMatch>&line,const char *path);
    void printDIndex(OCRMatch &a);

//}
    
//-----------------------------------------------------------------------------------------
    /// @name Inner function of text processing
	//@{
    
	/** @brief function check misspeled string by dictionary and return corrected string.
	    as grammar criteria used length of regult correct string */
	string remapString(string &str_);
	/** @brief function check two string returned from dictionary and return one answer string,
	 where it is start from one string and end from second; */
	string remapTibString(string &input0, string &input1);

	
	/** @brief get match from hash index about query string */	int hashMatch(string &str);
    string findStringMatch(const char *strKey, int sL, int offset);
	void bestMatchString(vector<stringEntry>&matchArray,
                         const char *strKey_,
                         int sL,
                         string &textString,
                         string &destStr);
	
    /** @brief apply list of Regular Expression or current language for OCR result*/
    void applyRE( vector<wordOCR> &wordArray,vector<wordOCR>&lineText);
	
    /** @brief Add line it in correction page*/	
	void writeInCorrectionPage(vector<stringOCR>*strArray,
                               vector<stringOCR> &correctionWordArray,
                               stringOCR &testStr,
                               string &srcStr);
	
	/** @brief Returns check letter in correction Table and add it in correction Table if not found*/	
	void checkInCorrectionTable(vector<stringOCR> &correctionWordArray,
                                stringOCR &testStr,
                                string &srcStr);
	
	/** @brief function load dictionare data tree in memory and call all translation methods */	string startDictionary();
	/** @brief function load dictionare data tree in memory from XML files */
	void readDictionary();
	/** @brief function load dictionare data tree in memory from FileMaker XML files */
	void readDictionaryFileMakerXML(string &fileName);
	/** @brief function reload dictionare data tree in memory from XML files */
	void reloadDictionary();
	/** @brief function set new words in dictionary */
	void writeInDictionary();
    /** @brief function set new data in dictionary record */
	void saveInDictionary(string &srcStr);
    /** @brief Save all new words from text data in translation dictionary*/
    void saveToTranslationDictionary(string &srcStr);
	/** @brief function is used for build dictionary hash index from texts set. Function can be used for biuls full-text search index. */	
	void buildDictionaryHashIndex();
	
    void buildDictionaryHashIndexUni(vector<string>&strVector);
    
	/** @brief function read grammar particals and punctuation data from  XML file */	 	void readGrammarDataXML(string &path);
	
    /** @brief function translate one string and add dictionary report in report vector */
	void mainDictReport();
	
    /** @brief read text file or text bufer and translate one page. Write result in destination string or disk file*/
    void mainTextTranslation(string &destString);
    /** @brief translate one text line and write result in destString*/
    void lineTextTranslation(string &destString);
    void lineTextTranslationSkt(string &destString);
    
    
    void PaliRusDictRE(vector<string>&text,int print);
    void SanskritRusDictRE(vector<string>&text,int print);
    void TibRusDictRE(vector<string>&text,int print,int mode);
	
	/** @brief translation function. Apply dictionary Regular Expresssion list to text line-by-line */
    void TibRusDictReport(GVector*vectorDict, GStr<uint>*keyDict,GVector*strVector);
    void TibRusDictReport(GVector*vectorDict, GStr<uint>*keyDict,string &line);
	
	/** @brief function analise mainDict map and found match of all parts of every key in it.
	 this is used for build dictionary from texts set.
	 this is no save conversion with original mainDict map */
	void  buildDictionary( vector<string> &strVector);
    
    /** @brief Text alanysis function. Buld matrix of syllable probability */
	void  buildSyllableStat();
    
	/** @brief function is used for build dictionary from texts set. Every uniq word set in maindictioary */	
	void  buildWordDictionary(vector<string> &strVector );
    void  buildTranslationDictionary(vector<string> &strVector );
	/** @brief function format dictionary report as HTML */
	string writeDictReport(string &srcStr, int reportLevel, int fullReport);
    string writeDictReportTranslation(string &srcStr,int reportLevel, int fullReport);
    void writeDictReportLocal(string &srcStr);
	
    void writeDictReportXML(vector <vector <OCRDictRecord> >&dictReportPage,string &binPath);
	/** @brief function sort dictionary report vector and put more important report in top */
    string sortKey(vector<dictKey>&keyArray,string &srcKey,int fullReport);
	/** @brief save mainDict map in TXT file */
    void writeDictionaryTXT(  map<string, OCRDictRecord>&mainDict);
	/** @brief save mainDict map in XML file */
    void writeDictionary(map<int, OCRDictRecord> &dictRecord, string &filePath);
	/** @brief load font map xml file from disk */
	 	   
	void loadMapFilePali(const char *mapName);
	void LoadFontNameMap(); 
	void LoadFontMap(map<string,uniRecord>&fontMap,string &fileName);
	void loadDictLevelFile();
     
    void table_Dictionary();
    
    //RegExp functions
    string splitEngWord(string &word, int *result);
//-----------------------------------------------------------------------------------------
//}
    
/// @name Correlation search 
	//@{
//-----------------------------------------------------------------------------------------
    /// Тестовые функции разработки ассоциативного поиска ///
    
    unsigned short  *dictionary_data; // массив словаря
    unsigned int dict_size;           // размер массива словаря 
    
    ////unsigned short  *text_data;       // массив распознаваемого текста
    unsigned int text_size;           // размер массива распознаваемого текста
    
    unsigned short codeEnter;      // исходныя кодировка перевода каретки во всех словарях, обычно 10
    
    /// ассоциативная обработка сигналов
	void associativeSignalProcessing(); 
    
    
    /// функция плотной упаковки разреженного кодового пространства словарных букв
    void table_Dict();
    // массивы функции перекодировки букв словаря 
    unsigned short *BufM;          // зеркальный массив BufM 
    unsigned short *BufU;          // массив для восстановления исходного кода буквы (разреженного кода)
    // из упакованного кода буквы
    unsigned int *BufE;            // массив (int) для хранения адресов перевода каретки Enter словаря
    // массивы функции перекодировки пар букв словаря
    unsigned short *BufMp;         // зеркальный массив пар букв BufMp словаря
    unsigned short *BufUp;         // массив для восстановления исходного кода первой буквы пары    
    // из упакованного (компактного) кода пар букв словаря    
    // выходные переменные функции перекодировки букв словаря
    unsigned int dict_size1;       // размер словаря без переводов каретки Enter      
    unsigned short nLetter;        // количество разных букв в словаре
    unsigned int nEnter;           // количество переводов каретки Enter в словаре
    unsigned int nRank,rank;       // округленное значение nLetter до ближайшей степени двойки nRank. Сама степень двойки равна rank 
    
    
    
    
    /// функция плотной упаковки разреженного кодового пространства текстовых букв
    void table_Text();
    // массивы функции перекодировки пар текста
    wstring strTextW; //
    string strText;
    unsigned int *BufET;           // массив (int) для хранения адресов перевода каретки Enter текста
    unsigned short *BufMpT;        // зеркальный массив пар букв BufMpT текста
    unsigned short *BufUpT;        // массив для восстановления исходного кода первой буквы пары    
    // из упакованного (компактного) кода пар букв текста
       
//}	

};
};
// ---------------- IMPLEMENTATION
# endif

