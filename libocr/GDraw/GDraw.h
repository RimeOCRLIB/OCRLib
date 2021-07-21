//
//  GDraw.h
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

#ifndef __OCRLib__GDraw__
#define __OCRLib__GDraw__

namespace ocr {

class GDraw {
  protected:
  	int border;
		float k;
		int r1,r2,r3,r4;
		int maskH;
		int maskBorder;
		int imgW;
		int imgH;
		int clusterCount;
		int graphW;
		int graphH;
		int shiftX;
		int shiftY;
		int shiftMaskX;
		int canvasImgH;
		int canvasImgW;
		int dH;
		float h;
		float k1;
		float dY;
		float dX;
		int pW;
		int xCenter, yCenter;
  public:
	/** @name Initialisation functions */
	/** @bref  Initialisation by constructor*/
	GDraw(void);
	void init();
	/** @bref  Destructor */
	~GDraw();
	/** @name Variables */

	int drawMode; 

	//inner functions, output to ostringstream
	void printHeadStyle(int mode);
	void printImgSVG(uchar *p, int mode);
	void printOCRStarSVG(void *lines_, void *star_);
	void printLineDirLable(void *lines_,void *star_, uchar mode);
	void printOCRStarBaseLine(void *star_);
	void printOCRLineLabel(void *line_, void *points_);
	void printOCRStarPoint(void *star_);
	void printOCRStarCluster(void *star_);
	void printReg(uint64 &reg, cstr text);
	void printOCRStarHash(void *star_);
	void printOCRLineMaskSVG(void *line_, int mode);
	void printClusterMaskSVG(void *cluster_);
	void printClusterSVG(void *cluster, int mode);
	void printStarSVG(void *star_,int mode);
	void printLineSVG(void *line_,void *points_,int mode);
	void printMidLineSVG(float *px_, float *py_, uint pointCount);

	
	//output to stdout 
	void printOCRMask(int *p);
	void printGraphStar(void *graph_, void *star_);
	void printClusterStar(void *cluster_, void *star_);
	void printStar(void *star_);
	void printGraphStars(void *graph_);
	void printLine(void *line_,void *points_);
	void printLine(void *line_);
	void printLines(void *graph_);
	void printCluster(void *cluster_);
	
	//output to string
	void drawCluster(void *cluster_);
	void drawGraphSVG(void *graph_, void *dataStr_);
	void drawLetterSVG(void *letter_, void *font_, void *dataStr_);
	void drawLetterCorrelationSVG();
	void drawOCRLineCorrelationSVG(void *lineT_, void *line_, void *dataStr_);
	void drawOCRLineAffineCorrelationSVG(void *lineT_, void *line_, void *dataStr_, int dir);
	int drawOCRStarCorrelation(void *starT_,void *star_,void *linesT_,void *lines_, cstr path);
	void drawLetterClustersSVG();
	void drawCorrelation();
    void calculateBezierCurveHandlers(OCRPoint &p0,OCRPoint &p1, OCRPoint &p2, OCRPoint &p3);
	
	//output to file path
	void printGraphSVG(void *graph_, cstr path);
	void printTextGraphSVG(void *graph_, cstr path,int mode);
	void printClusterSVG(void *cluster_, cstr path);
	void printGraphClustersSVG(void *clusterArray_, uint size, cstr path);
	void printLetterSVG(void *letter_, void *font_, cstr path);
	void printLetterImg();
    void printClusterMatch(void *letterMatch_,void *textMatch_,void *font_,void *graph_, cstr path);
    void printLetterMatch(void *letterMatch_,void *font_, cstr path);
    void printMatchLetters(void *matchLetter_,  void *graph, void *strArray, cstr imgPath, cstr path);
	
};

} // namespace ocr

#endif
