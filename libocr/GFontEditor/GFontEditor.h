#ifndef HEADER_GFontEditor
#define HEADER_GFontEditor
//__OCR CLASSES_____________________________________
#include "../config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include "../GFontEditor/GFont.h"
#include "../GBitMask/GBitMask.h"
#include "../GBitset/GBitset.h"
#include "../GBitsetMatrix/GBitsetMatrix.h"
#include "../GImageEditor/GImageEditor.h"
#include "../OCRTypes/OCRTypes.h"
#include "../../pugixml/pugixml.hpp"

using namespace std;

namespace ocr {
        
    ///main class for manage GFonts
    class GFontEditor{
    private:
        GFontEditor(void);
    public:
        static GFontEditor* create(void){return new GFontEditor;}
        vector<GFont*> fontBase;
        GFont* aliKali;
        GFont* font;
        GVector *imageData;
        GVector *starData;
        int GFontCount;
        vector<GFont*> activeGFont;
        GFont*  operator[](int index);
        GLogicProcessor *logicProcessor;
        GBitmap *pechaImg;
        
        /// import OCR letter data from GLetter format
        void importLetter(GLetter *letter);
        int printImg;
	    ///чтение XML данных GFont из OCRData/OCRTables/...
		void readGFonts();
        ///чтение XML данных GFont в базу данных
		string importGFontsDB( string &path);
        ///запись данных базы данных GFont в XML
		string exportGFontsDB( string &path);
        ///запись данных базы данных GFont в PNG
        void exportGFontsImages(cstr path);
        ///переупаковка данных шрифта
        void reloadGFont();
        ///установка в шрифте флага языка распознавания
        void setLanguage();
        void setLettersProbability();       ///< set OCRFont letters probability
        
	    ///вывод окна редактирования буквы
	    string drawEditLetter(unsigned int idNumder);
        ///вывод окон редактирования новых букв в базе
        string drawNewLetters();
        
		///на основании координат буквы копирует ее изображение из текста, векторизует, определяет признаки буквы и создает запись в базе букв
	    void setLetter();
        ///сравнивает букву со всеми буквами в базе. Подробнее см. woodblockOCR.cpp
        void setMaskFromBase();
        //устанавливает масштаб для букв в базе
        void setScale();
        
        ///определяет с каким максимальным коеффициентом корреляции и в каком масштабе данная буква изображена в GBitmap. Данные о масштабе записываются в букву
	    int setLetterScale(GBitmap *img128, GLetter *glyph_);
	    ///один из вариантов определить что за буква для занесения в базу. Ищет похожую букву во всех сохраненных базах. 
		/// не проходит по памяти и вариабельности. (не находит себя в базах)
	    void setLetterFromTypesetting( GLetter *letter,GBitmap* img128); 
	    ///This function check correlation of test letter with all letters database and return array of letters with close correlation
	    ///проверяет есть ли похожие буквы в базе. 
        void testLetter(GLetter *letter, int mode);
        void testLetter(GLetter *letter,vector<OCRMatch>&letterLine,int mode,uint *startIndex);
        ///проверяет корреляцию буквы среди массива букв похожих на проверяемую
        void testLetterInLine(GLetter *letter,vector<OCRMatch>&letterLine,vector<OCRMatch>&letterLineNew);
        //Проверка на влияние маски в букве на корреляцию. Стираем маски которые не влияют на корреляцию
        void eraseNoCorrelationMask(GLetter *letter);
        /////  поиск главного признака в пределах буквы  /////
        void setLetterMainMask(GLetter *letter);
		///This function check correlation of every letter with all letters database and return array of letters with close correlation
		///проверяет есть ли похожие буквы в базе. Функцию нужно существенно расширить. Необходимо выявить в базе ВСЕ слишком похожие буквы.
		///В наилучшем результате между всеми буквами базы должно быть наибольшее кодовое расстояние.
		void testBase();
        ///функция проверяет всю базу на наличие частей букв аналогичных указанным частям тестовой буквы. Для найденных букв применяется расстановка масок указанных частей тестовой буквы
        void testMask();
        ///функция проверяет всю базу и оптимизирует расстановку масок. Убирает дубликаты, находит оптимальную позицию
        void rebuildMask();
        ///функция проверяет букву и оптимизирует расстановку масок. Убирает дубликаты, находит оптимальную позицию
        void rebuildMaskInLetter(GLetter *letter,int* pfds); //функция запускается в дочернем процессе и пересылает результат через pipe
        ///функция проверяет букву и оптимизирует расстановку масок на основе векторов.
        void rebuildMaskInLetterVector(GLetter *letter,int* pfds); //функция запускается в дочернем процессе
        ///функция оптимизирует расстановку масок в букве. Оптимальная позиция маски определяется по устойчивости к полиграфическим искажениям.
        void letterMaskOptimisation(GLetter *letter);
	    ///определение величины корреляции между изображением и буквой с заданным масштабом. function need copy of actual letter. It store OCR information 
	    int  setCorrelation(GBitmap *img, GLetter *glyph,float scale,int print);
        ///распознает стековую букву всеми простыми буквами базы
        void letterOCR(GLetter *letter, string &name, int *correlation,int mode);
        ///распознавание стековых букв базы всеми простыми буквами
        void letterBaseOCR(int startIndex);
        
	    ///внесение изменений в базу букв на основе редактирования буквы через интерфейс
		void setBase();
		/// This function build letter base from text file of alphabet
	    /// генерация базы букв на основании текстового файла алфавита. Функция платформо зависима.
	    void buildGFontFromFont();
        //переписывает базу в новый файл, убирает стертые и поврежденные буквы.
        void rebuildGFont();
        
        /// This function build letter base from RTF file of alphabet
        /// генерация базы букв на основании RTF файла алфавита. Функция платформо зависима.
        void buildGFontFromRTF();
        void buildTesseractData();  /// генерация фалов базы в формате Tesseract.

		void rebuildBaseIndex();
        void setOCRKey();
        void importOCRLetters();
        int maskCorrelation(GBitMask32 &mask1,GBitMask32&mask2);

    };

};

#endif
