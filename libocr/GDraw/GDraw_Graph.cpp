//
// _______GDraw.cpp
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../GGraph/GGraph.h"
#include "../GGraphOCR/GGraphOCR.h"
#include "../OCRString/php2stl.h"
#include "GDraw.h"

using namespace std;
using namespace ocr;

namespace ocr
{
ostringstream out_;

void GDraw::printGraphStars(void *graph_) {
  GGraph &graph = *(GGraph *)graph_;
  uint    size = (uint)graph.starArray.size();
  for(int i = 0; i < size; i++)
    printGraphStar(graph_, (void *)&graph.starArray[i]);
}  //____________________________________________________________

void GDraw::printOCRMask(int *p) {
  for(int index = 0; index < 32; index++) {
    binaryPrint(p[index], 2);
    cout << endl;
  }
}

void GDraw::printLine(void *line_) {
  OCRLine &line = *(OCRLine *)line_;
  cout << "        @line index: " << line.index << " lenA:" << (int)line.lenA << " length:" << (int)line.length
       << " mask:" << (int)line.cType << " type:" << (int)line.type << endl;
  cout << "             start x:y " << line.start.x << ":" << line.start.y << "  end " << line.end.x << ":" << line.end.y << endl;
  cout << "             pointIndex:" << line.pointIndex;
  if(line.extremCount) {
    cout << " extremCount:" << (int)line.extremCount << endl;
    cout << "             extrem:  ";
    cout << "    _____" << endl;
  } else {
    cout << endl;
  }
  if(line.cType) {
    cout << "             pointC:" << endl;
    for(int i = 0; i < 5; i++)
      cout << "                " << line.pointC[i].x << ":" << line.pointC[i].y << endl;
  }

}  //____________________________________________________________

void GDraw::printLine(void *line_, void *points_) {
  OCRLine & line = *(OCRLine *)line_;
  OCRPoint *pointArray = (OCRPoint *)points_;
  cout << "        @line index: " << line.index << " lenA:" << (int)line.lenA << " type:" << (int)line.type
       << " length:" << (int)line.length << endl;
  cout << "             start x:y " << line.start.x << ":" << line.start.y << "  end " << line.end.x << ":" << line.end.y << endl;
  cout << "             pointIndex:" << line.pointIndex << " extremCount:" << (int)line.extremCount << endl;
  cout << "             extrem:  ";
  for(int i = 0; i < line.extremCount; i++) {
    OCRPoint &p = pointArray[line.pointIndex + line.lenA + i];
    cout << "                i:" << i << " x:y " << p.x << ":" << p.y << " type:" << (int)p.type << "  data:" << p.data << endl;
  }
  cout << "    _____" << endl;

}  //____________________________________________________________

void GDraw::printLines(void *graph_) {
  GGraph &       graph = *(GGraph *)graph_;
  GStr<OCRLine> &lineArray = graph.focalLine;
  OCRPoint *     points = graph.pointArray.dataPtr();

  for(int i = 0; i < lineArray.size(); i++) {
    cout << i;
    printLine((void *)&lineArray[i], points);
  }
}  //____________________________________________________________

void GDraw::printStar(void *star_) {
  OCRStar &star = *(OCRStar *)star_;
  cout << "    star.index:" << star.index << " star_.type:" << (int)star.type << " x:y " << star.cPoint.x << ":" << star.cPoint.y
       << " starCount:" << (int)star.starCount;
  for(int i = 0; i < star.starCount; i++)
    cout << " " << star.star[i];
  cout << " ____ lineCount:" << (int)star.lineCount;
  for(int i = 0; i < star.lineCount; i++)
    cout << " " << star.line[i];
  cout << endl;
}  //____________________________________________________________

void GDraw::printClusterStar(void *cluster_, void *star_) {
  OCRCluster &cluster = *(OCRCluster *)cluster_;
  OCRStar *   starArray = cluster.star;
  //OCRLine *   focalLine = cluster.line;
  OCRStar &   star = *(OCRStar *)star_;
  cout << "//____@star.index:" << star.index << " type:" << (int)star.type << " hash:" << (int)star.hash << " star x:y " << star.cPoint.x
       << ":" << star.cPoint.y << " starCount:" << (int)star.starCount << " box(x:y): " << star.xMin << ":" << star.yMin << " / "
       << star.xMax << ":" << star.yMax << endl;
  for(int n = 0; n < star.starCount; n++) {
    cout << "	   @star[" << n << "]:" << star.star[n] << " type:" << (int)starArray[star.star[n]].type << " "
         << starArray[star.star[n]].cPoint.x << ":" << starArray[star.star[n]].cPoint.y << endl;
    cout << "	   point[" << n << "]: type:" << (int)star.point[n].type << " " << star.point[n].x << ":" << star.point[n].y << endl;

    // printLine((void*)&focalLine[star.line[n]],(void *)graph.pointArray.dataPtr());
  }
}

void GDraw::printGraphStar(void *graph_, void *star_) {
  GGraph & graph = *(GGraph *)graph_;
  OCRStar *starArray = graph.starArray.dataPtr();
  OCRLine *focalLine = graph.focalLine.dataPtr();
  OCRStar &star = *(OCRStar *)star_;

  cout << "//____@star.index:" << star.index << " type:" << (int)star.type << " hash:" << (int)star.hash << " star x:y " << star.cPoint.x
       << ":" << star.cPoint.y << " starCount:" << (int)star.starCount << " box(x:y): " << star.xMin << ":" << star.yMin << " / "
       << star.xMax << ":" << star.yMax << endl;
  for(int n = 0; n < star.starCount; n++) {
    cout << "	   @star[" << n << "]:" << star.star[n] << " type:" << (int)starArray[star.star[n]].type << " "
         << starArray[star.star[n]].cPoint.x << ":" << starArray[star.star[n]].cPoint.y << endl;
    cout << "	   point[" << n << "]: type:" << (int)star.point[n].type << " " << star.point[n].x << ":" << star.point[n].y << endl;

    printLine((void *)&focalLine[star.line[n]], (void *)graph.pointArray.dataPtr());
  }
}  //____________________________________________________________________

void GDraw::printCluster(void *cluster_) {
  OCRCluster &cluster = *(OCRCluster *)cluster_;
  cout << "cluster:" << cluster.index;
  cout << "status:" << (int)cluster.status << endl;
  cout << "type:" << (int)cluster.type << endl;
  cout << "level:" << (int)cluster.level << endl;
  cout << "lineCount:" << (int)cluster.lineCount << endl;
  cout << "starCount:" << (int)cluster.starCount << endl;
  cout << "lCount:" << (int)cluster.lCount << endl;
  cout << "pCount:" << (int)cluster.pCount << endl;
  cout << "corr:" << cluster.corr << endl;
  cout << "w:" << cluster.w << endl;
  cout << "h:" << cluster.h << endl;
  cout << "xMax:" << cluster.xMax << endl;
  cout << "yMax:" << cluster.yMax << endl;
  cout << "xMin:" << cluster.xMin << endl;
  cout << "yMin:" << cluster.yMin << endl;
  cout << "sIndex:" << cluster.sIndex << endl;
  cout << "matchID:" << cluster.matchID << endl;
  cout << "cPoint.x:" << cluster.cPoint.x << endl;
  cout << "cPoint.y:" << cluster.cPoint.y << endl;
  cout << "scale:" << cluster.scale << endl;
  cout << "scale32:" << cluster.scale32 << endl;

  for(int i = 0; i < cluster.starCount; i++) {
    OCRStar &star_ = cluster.star[i];
    printStar((void *)&star_);
  }
  //printOCRMask(cluster.maskC.mask);  ///< маска кластера
}  //______________________________________________________________________________________

/*
void GDraw::printGraphSVG(void *graph_, cstr path) {
    GGraph &graph = *(GGraph *)graph_;
    string dataStr;
    drawGraphSVG((void*)&graph, (void*)&dataStr);
    writeText(dataStr, path);
} //______________________________________________________________________________________
*/

void GDraw::printTextGraphSVG(void *graph_, cstr path, int mode) {
  GGraph &graph = *(GGraph *)graph_;
  clearDirectory(path, "img");
  clearDirectory(path, "html");
  string path_svg = path;
  string path_img = path;
  if(path_img.find(".svg")!=-1){
  	path_img = str_replace(".svg",".png",path_img);
  } else if(path_img.find(".html")!=-1){
  	path_img = str_replace(".html",".png",path_img);
  } else { 
  	path_img += "/_draw_.png";
  	path_svg += "/_draw_.html";
  }
  
  graph.colorMode = 1;
  graph.savePNG(path_img);
  string str;
  out_.str("");

  out_ << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
  out_ << "<svg width=\"" << graph.columns() << "\" height=\"" << graph.rows() << "\" viewBox=\"0 0 " << graph.columns() << " "
       << graph.rows() << "\" " << endl;
  out_ << "xmlns=\"http://www.w3.org/2000/svg\" "
          "xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
       << endl;
  out_ << "<image xlink:href=\"" << path_img << "\" x=\"0\" y=\"0\" width=\"" << graph.columns() << "px\" height=\"" << graph.rows()
       << "px\"/>" << endl;
  out_ << "<style> .c{fill:none; stroke:blue; stroke-width:1}" << endl;
  out_ << ".d{fill:none; stroke:red; stroke-width:1}" << endl;
  out_ << ".s0{fill:white;}" << endl;
  out_ << ".s1{fill:green;}" << endl;
  out_ << ".s2{fill:orange;}" << endl;
  out_ << ".s3{fill:blue;}" << endl;
  out_ << ".s4{fill:magenta;}" << endl;
  out_ << ".s5{fill:#cc0033;}" << endl;
  out_ << ".c0{fill:none;stroke:white;stroke-width:0.5}" << endl;
  out_ << ".c1{fill:white;stroke:none}" << endl;
  out_ << ".t0{font-family:Courier;font-size:6px;fill:white}" << endl;

  out_ << "</style>" << endl;
  // draw focal lines
  cout << "//" << mode << endl;
  for(int i = 0; i < graph.focalLine.size(); i++) {
    printLineSVG((void *)&graph.focalLine[i], (void *)graph.pointArray.dataPtr(), mode);
    out_ << str;
  }
  for(int i = 0; i < graph.starArray.size(); i++) {
    printStarSVG((void *)&graph.starArray[i], 1 - mode);
    // printGraphStar((void *)&graph,(void *)&graph.starArray[i]);
  }

  out_ << "</svg>" << endl;

  str = out_.str();
  writeText(str, path_svg);
  cout << "done" << endl;

}  //______________________________________________________________________
void GDraw::printStarSVG(void *star_, int mode) {
  OCRStar &star = *(OCRStar *)star_;
  if(star.type != 0) {
    out_ << "<rect class=\"s" << (int)star.type << "\" x=\"" << star.cPoint.x - 1 << "\" y=\"" << star.cPoint.y - 1
         << "\" width=\"3\" height=\"3\"/>" << endl;
  } else {
    out_ << "<circle class=\"c0\" cx=\"" << star.cPoint.x << "\" cy=\"" << star.cPoint.y << "\" r=\"" << (int)star.radius << "\"/>" << endl;
    out_ << "<circle class=\"c1\" cx=\"" << star.cPoint.x << "\" cy=\"" << star.cPoint.y << "\" r=\"1.5\"/>" << endl;
  }
  if(mode) {
    out_ << "<text class=\"t0\" x=\"" << star.cPoint.x + 2 << "\" y=\"" << star.cPoint.y - 1 << "\">" << star.index << "</text>" << endl;
  }
}  //______________________________________________________________________

void GDraw::printLineSVG(void *line_, void *points_, int mode) {
  OCRLine & line = *(OCRLine *)line_;
  OCRPoint *pointArray = (OCRPoint *)points_;
  char      style = 'c';
  if(mode && !line.status) style = 'd';
  int extremCount = line.extremCount;
  if(extremCount) {
   	//cout<<" line:"<<line.index<<" extremCount:"<<extremCount<<" line.pointIndex:"<<line.pointIndex<<endl;
    OCRPoint *lineData = &pointArray[line.pointIndex];
    
    //for(int m = 0; m <line.lenA; m++) cout<<"    p0: "<<lineData[m].x<<"/"<<lineData[m].y<<endl;
    
    //рисуем квадратичную апроксимацию
    for(int m = 0; m <= extremCount; m++) {
      OCRPoint p0;
      OCRPoint p1;
      OCRPoint p3;
      int      Px1;
      int      Py1;
      int      len = 0;
      //cout<<" line.extrem["<<m<<"]:"<<(int)line.extrem[m]<<endl;
      if(m > 0) {
        p0 = lineData[line.extrem[m - 1]];
        len = line.extrem[m] - line.extrem[m - 1];
        p1 = lineData[line.extrem[m - 1] + len / 2];

      } else {
        p0 = line.start;
        len = line.extrem[m];
        p1 = lineData[len / 2];
      }

      if(m < line.extremCount) {
        p3 = lineData[line.extrem[m]];
      } else {
        p3 = line.end;
      }
      if(m == line.extremCount) {
        p0 = lineData[line.extrem[m - 1]];
        len = line.lenA - line.extrem[m - 1];
        p1 = lineData[line.extrem[m - 1] + len / 2];
      }
      
      //cout<<"p0: "<<p0.x<<"/"<<p0.y<<"  p3:"<<p3.x<<"/"<<p3.y<<endl;

      if(len > LINE_SIZE) {
        Px1 = -0.5 * p0.x + 2.0 * p1.x - 0.5 * p3.x;
        // координата “x” первой управляющей точки.
        Py1 = -0.5 * p0.y + 2.0 * p1.y - 0.5 * p3.y;

        out_ << "<path class=\"" << style << "\" d=\"M " << p0.x << " " << p0.y << " Q " << Px1 << " " << Py1 << " " << p3.x << " " << p3.y
             << "\"/>" << endl;
      } else {
      
        out_ << "<path class=\"" << style << "\" d=\"M " << p0.x << " " << p0.y << " L" << p3.x << " " << p3.y << "\"/>" << endl;
      }

      // draw control point
      // out<<"<circle class=\"s3\" cx=\""<<(Px1 + shiftX) * k<<"\" cy=\""<<
      //(Py1 + shiftY) * k <<"\" r=\""<<k/2<<"\" />"<<endl;

      // out<<"<text class=\"t1\" x=\""<<(Px1+ shiftX) * k + 0.4 * k
      // <<"\" y=\""<<(Py1 + shiftY) * k + 0.6 *
      // k<<"\">"<<Px1<<":"<<Py1<<"</text>"<<endl;

      if(m < line.extremCount) {
        int       r = 1;
        OCRPoint &p = lineData[line.extrem[m]];
        // if(p.type==P_POINT)r=2*k;
        out_ << "<circle class=\"s5\" cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"" << r << "\" />" << endl;

        // out<<"<text class=\"t1\" x=\""<<p.x + 1
        // <<"\" y=\""<<p.y + shiftY) * k + 0.6 * k<<"\">"<<p.x<<":"<<p.y
        // <<" -> "<<(int)p.type<<"</text>"<<endl;
      }
    }
  } else if(line.cType) {
  	OCRPoint p0;
		OCRPoint p1;
		OCRPoint p3;
		int      Px1;
		int      Py1;
		//int      len = 0;
		p0 = line.start;
		p1 = line.pointC[2];
		p3 = line.end;

		Px1 = -0.5 * p0.x + 2.0 * p1.x - 0.5 * p3.x;
		// координата “x” первой управляющей точки.
		Py1 = -0.5 * p0.y + 2.0 * p1.y - 0.5 * p3.y;

		out_ << "<path class=\"" << style << "\" d=\"M " << p0.x << " " << p0.y << " Q " << Px1 << " " << Py1 << " " << p3.x << " " << p3.y
					 << "\"/>" << endl;
					 
		if(mode) {
      out_ << "<text class=\"t0\" x=\"" << p1.x + 2 << "\" y=\""
           << p1.y - 1 << "\">" << line.index << "</text>" << endl;
    }
		
  	
  } else {
    out_ << "<path class=\"" << style << "\" d=\"M " << line.start.x << " " << line.start.y << " L " << line.end.x << " " << line.end.y
         << "\"/>" << endl;
    if(mode) {
      out_ << "<text class=\"t0\" x=\"" << line.start.x + (line.end.x - line.start.x) / 2 + 2 << "\" y=\""
           << line.start.y + (line.end.y - line.start.y) / 2 - 1 << "\">" << line.index << "</text>" << endl;
    }
  }

}  //______________________________________________________________________

void GDraw::printMatchLetters(void *matchLetter_, void *graph_, void *strArray_, cstr imgPath, cstr path){
    
    vector<OCRLetter> &matchLetter=*(vector<OCRLetter>*)matchLetter_;
    ostringstream out;
    GGraph &graph = *(GGraph*)graph_;
    int h = graph.rows();
    int w = graph.imgW;
    vector<OCRPoint> &strArray=*(vector<OCRPoint>*)strArray_;
    
    
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
    out << "<svg width=\"" << w << "\" height=\"" << h << "\" viewBox=\"0 0 " << graph.columns() + 200 << " "
    << h + 200 << "\" " << endl;
    out << "xmlns=\"http://www.w3.org/2000/svg\" "
    "xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
    << endl;
    out << "<image xlink:href=\"" << imgPath << "\" x=\"0\" y=\"0\" width=\"" << w << "px\" height=\"" << h
    << "px\"/>" << endl;
    out << "<style>\n.s0{fill:none; stroke:green; stroke-width:3; cursor: move;}" << endl;
    out << ".t0{font-family:Yagpo Tibetan Uni;font-size:21px;fill:green; paint-order: stroke; stroke: white; stroke-width: 3px;}" << endl;
    out << ".s1{stroke: silver; stroke-width:1x;}" << endl;
    out<< "</style>"<<endl;
    
    for(int i=0;i<strArray.size();i++){
        out<<" <line  class=\"s1\" x1=\"0\" y1=\""<< h - strArray[i].y <<"\" x2=\""<< w <<"\" y2=\""<< h - strArray[i].y <<"\"/>"<<endl;
    }
    
    for(int i=0;i<matchLetter.size();i++){
        OCRLetter &letter=matchLetter[i];
        //cout<<letter.name.data<<endl;
        
        int x=letter.xMatch - letter.letterW/2 * letter.scale;
        int y=letter.yMatch - letter.letterH/2 * letter.scale;
        out << "<rect class=\"s0\" \" x=\"" << x << "\" y=\"" << y
        << "\" width=\""<<letter.letterW * letter.scale<<"\" height=\""<<letter.letterH * letter.scale<<"\"/>" << endl;
        out << "<text class=\"t0\" x=\"" << x + 5 << "\" y=\"" << y + letter.letterH * letter.scale + 20 << "\">" << letter.name.data << "</text>" << endl;
        
    }
    
    
    string dataStr=out.str();
    writeText(dataStr,path);
}

}  // namespace ocr
