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

#include "../OCRString/php2stl.h"
#include "GDraw.h"
#include "../GGraph/GGraph.h"
#include "../GGraphOCR/GGraphOCR.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace ocr;


namespace ocr {
	
GString color[15];
OCRPoint max, min;
ostringstream out;

enum graphColor {
	blue_ = 0,
	green_ = 1,
	orchid_ = 2,
	red_ = 3,
	violet_ = 4,
	grass_ = 5,
	oak_ = 6,
	orange_ = 7,
	black_ = 8,
	white_ = 9,
	light_ = 10,
	grey_ = 11,
	silver_ = 12,
	glow_ = 13
};
	
GDraw::GDraw(){
	init();
}	

GDraw::~GDraw(){}

void GDraw::init(){
	border = 27;
	maskH = 128;
	maskBorder = 14;
	shiftMaskX=0;
	h = 0.4;
	pW = 5;
	drawMode=1; //1-night mode; 0 - normal mode
	k = 7;
	r1=k*1.7;
	r2=k*1.6;
	r3=k*1.5;
	r4=k/3;
	dH = 1000;
}

void GDraw::printHeadStyle(int mode) {
	out.str("");
	
	color[blue_] = "rgba(0,100,225,0.7)";
	color[green_] = "rgba(23,203,0,0.7)";
	color[orchid_] = "rgba(180,17,126,0.7)";
	color[red_] = "rgba(228,0,16,0.7)";
	color[violet_] = "rgba(183,0,145,0.7)";
	color[grass_] = "rgba(236,11,0,0.7)";
	color[oak_] = "rgba(11,80,0,1)";
	color[orange_] = "rgba(255,124,0,0.7)";
	color[black_] = "black";
	color[white_] = "white";
	color[light_] = "rgba(175,175,175,0.5)";
	color[grey_] = "rgba(60,60,60,0.5)";
	color[silver_] = "rgba(128,128,175,0.2)";
	color[glow_] = "rgba(255,255,255,0.7)";

	out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
	if(drawMode){
		out << "<body style='background-color:black'>" << endl;
	}else{
		out << "<body style='background-color:white'>" << endl;
	}
	out << "<svg width=\"" << canvasImgW << "\" height=\"" << canvasImgH << "\" viewBox=\"0 0 " << canvasImgW << " "
		<< canvasImgH << "\" " << endl;
	if(drawMode){
		out<<"style=\"background-color:black;margin:-10px\" "<<endl;
	}else{
		out<<"style=\"background-color:white;margin:-10px\" "<<endl;
	}
	out << "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << endl;
	// out<<"<image xlink:href=\""<<imgPath<<"\" x=\"0\" y=\"0\" width=\""<<ncolumns<<"px\"
	// height=\""<<nrows<<"px\"/>"<<endl;
	out << "<style> .c{stroke:blue; stroke-width:1}" << endl;
	out << ".s0{fill:" << color[grey_].data << ";stroke:" << color[light_].data << ";stroke-width:1}" << endl;
	out << ".s1{fill:" << color[green_].data << ";}" << endl;
	out << ".s2{fill:" << color[orange_].data << ";}" << endl;
	out << ".s3{fill:" << color[blue_].data << ";}" << endl;
	out << ".s4{fill:" << color[violet_].data << ";}" << endl;
	out << ".s5{fill:" << color[red_].data << ";}" << endl;
	out << ".s9{fill:" << color[light_].data << ";}" << endl;
	out << ".s10{fill:" <<  color[grey_].data << ";stroke:black;stroke-width:2}" << endl;

	out << ".L{stroke:" << color[light_].data << ";stroke-width:1}" << endl;
	out << ".L0{stroke:" << color[white_].data << ";stroke-width:3}" << endl;
	out << ".L1{stroke:" << color[green_].data << ";stroke-width:3}" << endl;
	out << ".L2{stroke:" << color[orange_].data << ";stroke-width:3}" << endl;
	out << ".L3{stroke:" << color[blue_].data << ";stroke-width:3}" << endl;
	out << ".L4{stroke:" << color[violet_].data << ";stroke-width:3}" << endl;
	out << ".L5{stroke:" << color[red_].data << ";stroke-width:3}" << endl;
	out << ".L9{stroke:" << color[light_].data << ";stroke-width:3}" << endl;
	out << ".L11{fill:none;stroke:" << color[red_].data << ";stroke-width:1}" << endl;
	out << ".L12{fill:none;stroke:" << color[red_].data << ";stroke-width:0.5}" << endl;

	out << ".lp{fill:" << color[silver_].data << ";}" << endl;
	if(drawMode){
		out << ".c0{fill:none;stroke:white;stroke-width:2}" << endl;
		out << ".c1{fill:white;stroke:white;stroke-width:2}" << endl;
	}else{
		out << ".c0{fill:none;stroke:black;stroke-width:2}" << endl;
		out << ".c1{fill:black;stroke:black;stroke-width:2}" << endl;
	}
	out << ".c2{fill:none;stroke:" << color[glow_].data << ";stroke-width:3}" << endl;
	//out << ".c3{fill:"<< color[glow_].data <<";stroke:white;stroke-width:0.5}" << endl;
	out << ".c3{fill:"<< color[glow_].data <<"}" << endl;

	
	if(drawMode){
		out << ".p0{fill:" << color[light_].data << ";stroke:black;stroke-width:0.2}" << endl;	 // pixel dark
		out << ".p1{fill:" << color[silver_].data << ";stroke:silver;stroke-width:0.2}" << endl; // pixel light
		out << ".p2{fill:" << color[red_].data << ";stroke:gray;stroke-width:2}" << endl; // pixel light
		out << ".p4{fill:gray;stroke:white;stroke-width:0.2}" << endl;  // pixel white
		out << ".p3{fill:black;stroke:gray;stroke-width:0.2;}" << endl; // pixel black
	}else{
		out << ".p0{fill:" <<  color[grey_].data << ";stroke:black;stroke-width:0.2}" << endl; // pixel dark
		out << ".p1{fill:" << color[light_].data << ";stroke:gray;stroke-width:0.2}" << endl; // pixel light
		out << ".p2{fill:" << color[red_].data << ";stroke:gray;stroke-width:2}" << endl; // pixel light
		out << ".p3{fill:white;stroke:black;stroke-width:0.1}" << endl;  // pixel white
		out << ".p4{fill:black;stroke:black;stroke-width:0.1;}" << endl; // pixel black
	}
	out << ".p5{fill:green;stroke:white;stroke-width:0.5}" << endl;
	out << ".p6{fill:red;stroke:red;stroke-width:0.5;}" << endl;
	out << ".p7{fill:none;stroke:green;stroke-width:0.5;}" << endl;
    out << ".p8{fill:none;stroke:green;stroke-width:1.5;}" << endl;
    out << ".p9{fill:" << color[silver_].data << ";stroke:none;}" << endl;
   
	
	out << ".t0{font-family:Courier;font-size:11px;fill:black}" << endl;
	out << ".t1{font-family:Courier;font-size:14px;fill:black}" << endl;
	out << ".t2{font-size:11px;fill:black}" << endl;
	out << ".t2{font-size:14px;fill:black}" << endl;
	out << ".t3{font-size:9px;fill:black}" << endl;
	out << ".t4{font-size:9px;fill:white}" << endl;
	if(mode){
		out << ".cType{fill:none;stroke:white;}" << endl; // pixel light
		out << ".t5{font-family:Courier;font-size:11px;fill:white}" << endl;
		out << ".t6{font-family:Courier;font-size:14px;fill:white}" << endl;
	}else{
		out << ".cType{fill:none;stroke:black;}" << endl; // pixel light
		out << ".t5{font-family:Courier;font-size:11px;fill:black}" << endl;
		out << ".t6{font-family:Courier;font-size:14px;fill:black}" << endl;		
	}

	out << "</style>" << endl;
}

void GDraw::printImgSVG(uchar *p, int mode) {
	//___DRAW_IMAGE____
	// printLetterImg(letter);
	if(mode){
		for (int n = 0; n < imgH; n++) {
			for (int m = 0; m < imgW; m++) {
				if (p[n * imgW + m] > 128) {
					out << "<rect class=\"p1\" x=\"" << (m - xCenter + shiftX) * k - k / 2 << "\" y=\""
						<< (n - yCenter + shiftY) * k - k / 2 << "\" width=\"" << k << "\" height=\"" << k << "\" />"
						<< endl;
				}
			}
		}
	}else{
		for (int n = 0; n < imgH; n++) {
			for (int m = 0; m < imgW; m++) {
				if (p[n * imgW + m] < 128) {
					out << "<rect class=\"p1\" x=\"" << (m - xCenter + shiftX) * k - k / 2 << "\" y=\""
						<< (n - yCenter + shiftY) * k - k / 2 << "\" width=\"" << k << "\" height=\"" << k << "\" />"
						<< endl;
				}
			}
		}	
	}
}
void GDraw::printMidLineSVG(float *px, float *py, uint pointCount) {
	for (int n = 0; n < pointCount; n++) {
				out << "<circle class=\"lp\" cx=\"" << (px[n] - xCenter + shiftX) * k << "\" cy=\""
					<< (py[n] - yCenter + shiftY) * k << "\" r=\"2\" />"<< endl;
	}
}


void GDraw::printOCRStarSVG(void *lines_, void *star_){
	OCRLine *lineArray = (OCRLine*)lines_;
	OCRStar &star = *(OCRStar*)star_;
	if (star.type != O_POINT) {

		for (int n = 0; n < star.lineCount; n++) {
			OCRLine &line = lineArray[star.line[n]];
            if(!line.length)continue; //#
			//printLine((void *)&line);
			OCRPoint start = star.cPoint;
			OCRPoint end = star.point[n];
			end.x = start.x + end.x;
			end.y = start.y + end.y; //точки звездочки нормированы по центру звездочки

			k1 = h / line.length;
			dY = (end.x - start.x) * k1;
			dX = (start.y - end.y) * k1;
			dY *= -1;
			dX *= -1;
			if (end.type == S_POINT) {
				dY = 0;
				dX = 0;
			}
			//int len = line.lenA;
			if(line.type != CLUSTER_LINE){
				out << "<path class=\"L" << (int)star.type << "\" d=\"M " << (start.x + shiftX) * k + dX * k << " "
					<< (start.y + shiftY) * k + dY * k << " L " << (end.x + shiftX) * k + dX * k << " "
					<< (end.y + shiftY) * k + dY * k << "\"/>" << endl;
			}	else {
				out << "<path class=\"L" << (int)star.type << "\" d=\"M " << (start.x + shiftX) * k << " "
					<< (start.y + shiftY) * k << " L " << (end.x + shiftX) * k << " "
					<< (end.y + shiftY) * k << "\"/>" << endl;
			}
			
			//int Px1;
			//int Py1;
			
			if (line.cType) {
                /*
                //рисуем квадратичную апроксимацию
				OCRPoint p0=line.start;
				OCRPoint p1=line.pointC[2];
				OCRPoint p3=line.end;

				Px1 = -0.5 * p0.x + 2.0 * p1.x - 0.5 * p3.x;
				// координата “x” первой управляющей точки.
				Py1 = -0.5 * p0.y + 2.0 * p1.y - 0.5 * p3.y;

				out << "<path class=\"L11\" d=\"M " << (p0.x + shiftX) * k << " " << (p0.y + shiftY) * k << " Q "
					<< (Px1 + shiftX) * k << " " << (Py1 + shiftY) * k << " " << (int)((p3.x + shiftX) * k) << " "
					<< (int)((p3.y + shiftY) * k) << "\"/>" << endl;
                
                */
                //рисуем кубическую апроксимацию Безъе
                OCRPoint p0=line.start;
                OCRPoint p1=line.pointC[2];
                OCRPoint p2=line.pointC[3];
                OCRPoint p3=line.end;
                calculateBezierCurveHandlers(p0, p1,p2,p3);
                
                out << "<path class=\"L11\" d=\"M" << (p0.x + shiftX) * k << "," << (p0.y + shiftY) * k << " C"
                << (p1.x + shiftX) * k << "," << (p1.y + shiftY) * k << " "
                << (p2.x + shiftX) * k << "," << (p2.y + shiftY) * k << " "
                << (int)((p3.x + shiftX) * k) << ","
                << (int)((p3.y + shiftY) * k) << "\"/>" << endl;
                
                
			} else {
				OCRPoint p0=line.start;
				OCRPoint p4=line.end;
				out << "<path class=\"L11\" d=\"M " << (p0.x + shiftX) * k << " " << (p0.y + shiftY) * k << " L "
					<< (int)((p4.x + shiftX) * k) << " " << (int)((p4.y + shiftY) * k) << "\"/>" << endl;
			}
            
            
			// draw control point
			// out<<"<circle class=\"s3\" cx=\""<<(Px1 + shiftX) * k<<"\" cy=\""<<
			//(Py1 + shiftY) * k <<"\" r=\""<<r1<<"\" />"<<endl;

		}
	} else {
		out << "<circle class=\"c0\" cx=\"" << (star.cPoint.x + shiftX) * k << "\" cy=\""
			<< (star.cPoint.y + shiftY) * k << "\" r=\"" << (int)(star.radius * k) << "\"/>" << endl;
	}
}//______________________________________________________________

void GDraw::printLineDirLable(void *lines_,void *star_, uchar mode){

	OCRLine *lines = (OCRLine*)lines_;
	OCRStar &star = *(OCRStar*)star_;
	// if(i!=5)continue;
	//рисуем опорные линии
	for (int n = 0; n < star.lineCount; n++) {
		int h1 = k * 1.1;
		OCRLine &line=lines[star.line[n]];
		if(!star.dir[n] )continue; //&& line.type != CLUSTER_LINE
		//int lenA=line.lenA-1;
		int dir=star.dir[n];
		//int in2=k/3;
		//int in1=k * 0.8;


        OCRPoint start=line.start;
        OCRPoint end=line.pointC[2];
        if(line.lenA<line.lineW * SERIF_LIMIT*2)end=line.end;
        
        float l = sqrt((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y));
		k1 = h1 / l;
        
		dY = (end.x - start.x) * k1;
		dX = (start.y - end.y) * k1;
		dY *= -1;
		dX *= -1;
		int dl=k * 0.5;
		if(line.index > 9)dl=k * 0.9;
	
		if(dir){
			float kl1=(float)4.5/l; //коэффициент масштаба начала треугольника указателя по отношению к длине линии
			float kl2=(float)1.9/l; //коэффициент масштаба конца треугольника указателя по отношению к длине линии
			float kl3=(float)2.5/l; //коэффициент масштаба конца треугольника указателя по отношению к длине линии
			
			float t_end_x =start.x + (float)(end.x - start.x) * kl2;
			float t_end_y =start.y + (float)(end.y - start.y) * kl2;
			
			float t_start_x = start.x + (float)(end.x - start.x) * kl1;
			float t_start_y = start.y + (float)(end.y - start.y) * kl1;
			
			float t_text_x = start.x + (float)(end.x - start.x) * kl3;
			float t_text_y = start.y + (float)(end.y - start.y) * kl3;
		
			
			out << "<polygon class=\"c3\" points=\"" << (t_start_x + shiftX) * k << " " << (t_start_y + shiftY) * k << "  "
				<< (t_end_x + shiftX) * k + dX << " " << (t_end_y + shiftY) * k + dY << " " << (t_end_x + shiftX) * k - dX << " "
				<< (t_end_y + shiftY) * k - dY << "\"/>" << endl;
			
			//out << "<circle class=\"p5\" cx=\"" << (t_end_x + shiftX) * k  << "\" cy=\""
			//	<< (t_end_y + shiftY) * k  << "\" r=\"" << 1 << "\"/>" << endl;
			if(line.type == CLUSTER_LINE){	
				out << "<circle class=\"p5\" cx=\"" << (end.x + shiftX) * k  << "\" cy=\""
					<< (end.y + shiftY) * k  << "\" r=\"" << 2 << "\"/>" << endl;
			}
				
			string style="t3";
			if(mode==255){
				float fill=0.7-0.7* line.corr / 100;
				if(fill>0.5)style="t4";
				out << "<polygon style=\"fill:rgba(0,0,0,"<<fill<<")\" points=\"" << (start.x + shiftX) * k << " " << (start.y + shiftY) * k << "  "
				<< (end.x + shiftX) * k + dX << " " << (end.y + shiftY) * k + dY << " " << (end.x + shiftX) * k - dX << " "
				<< (end.y + shiftY) * k - dY << "\"/>" << endl;
			}
			float xf=(t_text_x + shiftX) * k - dl ;
			float yf=(t_text_y + shiftY) * k + 2.7;
            if(star.type != O_POINT){
                out << "<text class=\""<<style<<"\" x=\"" << xf<< "\" y=\""
                << yf << "\">" << line.index << "</text>" << endl;
            }else{
                out << "<text class=\""<<style<<"\" x=\"" << xf<< "\" y=\""
                << yf << "\">" << line.parentID << "</text>" << endl;
            }
            
            style="t4";
            t_text_x = line.pointC[2].x;
            t_text_y = line.pointC[2].y;
            xf=(t_text_x + shiftX) * k - dl*2 ;
            yf=(t_text_y + shiftY) * k + 2.7;
            //out << "<text class=\""<<style<<"\" x=\"" << xf<< "\" y=\""
            // << yf << "\">" << line.index <<"/ " << (int)line.lenA<< "</text>" << endl; // <<"/ " << (int)line.alphaMid
            
            if(line.scale0 < 5){
                //рисуем корреляционный вес линии //line.scale0 * 0.8

                //out << "<circle style=\"fill:rgba(255,255,225,"<<0.3<<")\" cx=\"" << xf  << "\" cy=\""
                // << yf  << "\" r=\"" << (int)7 * line.scale0 << "\"/>" << endl;
                if(line.scale0 > 0){
                    out <<" "<< "   <rect style=\"fill:rgba(255,255,225,"<<round_2(line.scale0 * 0.75)<<")\" x=\"" << xf  << "\" y=\""
                    << yf  << "\" width=\"" << round_2(line.scale0 * 13) << "\" height=\"" << round_2(line.scale0 * 13) << "\"/>" << endl;
                }
                out <<" "<< "  <rect style=\"fill:none;stroke:green\" x=\"" << xf  << "\" y=\""
                << yf  << "\" width=\"" << 13 << "\" height=\"" << 13 << "\"/>" << endl;

                
            }
            /*
            if(line.cType){
                out << "<text class=\""<<style<<"\" x=\"" << (line.pointC[1].x + shiftX) * k + 0.4 * k
                << "\" y=\"" << (line.pointC[1].y + shiftY) * k + 0.6 * k << "\">" << (int)line.alpha1 <<"</text>" << endl;
                
                out << "<text class=\""<<style<<"\" x=\"" << (line.pointC[3].x + shiftX) * k + 0.4 * k
                << "\" y=\"" << (line.pointC[3].y + shiftY) * k + 0.6 * k << "\">" << (int)line.alpha2 <<"</text>" << endl;
            }
            */
            
		}else{
			float kl1=(float)4.5/l; //коэффициент масштаба начала треугольника указателя по отношению к длине линии
			float kl2=(float)1.9/l; //коэффициент масштаба конца треугольника указателя по отношению к длине линии
			float kl3=(float)3.5/l; //коэффициент масштаба конца треугольника указателя по отношению к длине линии			
			
			float t_start_x =end.x + (float)(start.x - end.x) * kl2;
			float t_start_y =end.y + (float)(start.y - end.y) * kl2;
			
			float t_end_x = end.x + (float)(start.x - end.x) * kl1;
			float t_end_y = end.y + (float)(start.y - end.y) * kl1;
			
			float t_text_x = end.x + (float)(start.x - end.x) * kl3;
			float t_text_y = end.y + (float)(start.y - end.y) * kl3;
		
			
			out << "<polygon class=\"c3\" points=\"" << (t_start_x + shiftX) * k << " " << (t_start_y + shiftY) * k << "  "
				<< (t_end_x + shiftX) * k + dX << " " << (t_end_y + shiftY) * k + dY << " " << (t_end_x + shiftX) * k - dX << " "
				<< (t_end_y + shiftY) * k - dY << "\"/>" << endl;
			
			//out << "<circle class=\"p5\" cx=\"" << (t_end_x + shiftX) * k  << "\" cy=\""
			//	<< (t_end_y + shiftY) * k  << "\" r=\"" << 1 << "\"/>" << endl;
				
			out << "<circle class=\"p5\" cx=\"" << (start.x + shiftX) * k  << "\" cy=\""
				<< (start.y + shiftY) * k  << "\" r=\"" << 2 << "\"/>" << endl;
		
				
			string style="t3";
			if(mode==255){
				float fill=0.7-0.7* line.corr / 100;
				if(fill>0.5)style="t4";
				out << "<polygon style=\"fill:rgba(0,0,0,"<<fill<<")\" points=\"" << (start.x + shiftX) * k << " " << (start.y + shiftY) * k << "  "
				<< (end.x + shiftX) * k + dX << " " << (end.y + shiftY) * k + dY << " " << (end.x + shiftX) * k - dX << " "
				<< (end.y + shiftY) * k - dY << "\"/>" << endl;
			}
			float xf=(t_text_x + shiftX) * k - dl ;
			float yf=(t_text_y + shiftY) * k + 2.7;
            if(star.type != O_POINT){
                out << "<text class=\""<<style<<"\" x=\"" << xf<< "\" y=\""
                << yf << "\">" << line.index << "</text>" << endl;
                
                }else{
                out << "<text class=\""<<style<<"\" x=\"" << xf<< "\" y=\""
                << yf << "\">" << line.parentID << "</text>" << endl;
            }
		
		}

	}
}

void GDraw::printOCRStarBaseLine(void *star_) {
	
	OCRStar &star = *(OCRStar*)star_;
	//рисуем опорные линии
	for (int n = 0; n < star.baseCount; n++) {

		int h1 = 12;
		OCRPoint start = star.cPoint;
		OCRPoint end = star.point[star.pointCount + n];
		end.x = start.x + end.x;
		end.y = start.y + end.y; //точки звездочки нормированы по центру звездочки
		float l = sqrt((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y));

		k1 = h1 / l;
		dY = (end.x - start.x) * k1;
		dX = (start.y - end.y) * k1;
		dY *= -1;
		dX *= -1;

		out << "<polygon class=\"lp\" points=\"" << (start.x + shiftX) * k << " " << (start.y + shiftY) * k << "  "
			<< (end.x + shiftX) * k + dX << " " << (end.y + shiftY) * k + dY << " " << (end.x + shiftX) * k - dX << " "
			<< (end.y + shiftY) * k - dY << "\"/>" << endl;
	}
}

void GDraw::printOCRLineLabel(void *line_, void *points_){
	OCRLine &line = *(OCRLine*)line_;
	OCRPoint *points = (OCRPoint *)points_;
	out << "<text class=\"t3\" x=\"" << (points[line.pointIndex + line.lenA / 2].x + shiftX) * k + 0.4 * k
		<< "\" y=\"" << (points[line.pointIndex + line.lenA / 2].y + shiftY) * k + 0.6 * k << "\">" << line.index <<"</text>" << endl;
    
}
void GDraw::printOCRStarPoint(void *star_) {
	OCRStar &star = *(OCRStar*)star_;
	out << "<circle class=\"s" << (int)star.type << "\" cx=\"" << (star.cPoint.x + shiftX) * k << "\" cy=\""
		<< (star.cPoint.y + shiftY) * k << "\" r=\"" << r1 << "\"/>" << endl;
	int dl = 0.7 * k;
	if (star.index > 9) dl = 0.95 * k;
    out << "<text class=\"t6\" x=\"" << (star.cPoint.x + shiftX) * k - dl << "\" y=\""
    << (star.cPoint.y + shiftY) * k + 0.6 * k << "\">" << (int)star.index<< " / "<< (int)star.sumLenA << "</text>" << endl;

}

void GDraw::printOCRStarCluster(void *star_) {
	OCRStar &star = *(OCRStar*)star_;
	OCRPoint &point = star.cPoint;
    int dl=4;
	out << "<circle class=\"c2\" cx=\"" << (point.x + shiftX) * k << "\" cy=\"" << (point.y + shiftY) * k << "\" r=\""
		<< r2 << "\"/>" << endl;
	out << "<circle class=\"c0\" cx=\"" << (point.x + shiftX) * k << "\" cy=\"" << (point.y + shiftY) * k << "\" r=\""
		<< r3<< "\"/>" << endl;
    out << "<text class=\"t6\" x=\"" << (point.x + shiftX) * k - dl << "\" y=\""
    << (point.y + shiftY) * k + 0.6 * k << "\"> Cluster:" << star.clusterIndex<< "</text>" << endl;
}

void GDraw::printReg(uint64 &reg, cstr text){
	bitset<64> v = bitset<64>(reg);
	for (int i = 0; i < 64; i++) {
		if (v[i] > 0) {
			out << "<rect class=\"p4\" ";
		} else {
			out << "<rect class=\"p3\" ";
		}
		out << "x=\"" << border * k / 2 + i * pW << "\" y=\"" << dH + 8 << "\" width=\"" << pW << "\" height=\"" << pW
			<< "\" />" << endl;
	}
	out << "<text class=\"t0\" x=\"" << border * k/2 + pW * 65<< "\" y=\"" << dH + 8 << "\">"<< text << "</text>" << endl;
	dH += pW + 3;
}

void GDraw::printOCRStarHash(void *star_) {
	OCRStar &star = *(OCRStar*)star_;
	out << "<text class=\"t5\" x=\"" << border * k / 2 << "\" y=\"" << dH - 3 << "\">Star:" << star.index
		<< " hash:" << star.hash << " level:" << (int)star.level << "</text>" << endl;
	OCRRegister &reg=star.reg;
	printReg(reg.reg0_0, "reg0_0");
	printReg(reg.reg0_1, "reg0_1");
	dH += 3;
	printReg(reg.reg1_0, "reg1_0");
	printReg(reg.reg1_1, "reg1_1");
	dH += 3;
	printReg(reg.reg2_0, "reg2_0");
	printReg(reg.reg2_1, "reg2_1");
	dH += 32;
}

void GDraw::printOCRLineMaskSVG(void *line_, int mode) {
	/*
     OCRLine &line = *(OCRLine*)line_;
	k1 = (float)maskH / 32;
	out << "<text class=\"t5\" x=\"" << border * k  + shiftMaskX << "\" y=\"" << dH - 3 << "\">Line:" << line.index 
	<< " :" << line.startID << "/" << line.endID << "</text>" << endl;
	
	if (!line.cType) {
		dH += 12;
		return;
	}
	out << "<rect class=\"mask\" x=\"" << border * k + shiftMaskX<< "\" y=\"" << dH << "\" width=\"" << maskH << "\" height=\""
		<< maskH << "\" />" << endl;
	for (int y = 0; y < 32; y++) {
		int lineData = line.maskLine[y];
		for (int x = 0; x < 32; x++) {
			if ((lineData >> x) & 1) {
				out << "<rect class=\"p1\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" << y * k1 + dH << "\" width=\""
					<< k1 << "\" height=\"" << k1 << "\" />" << endl;
			}
		}
	}
	if(mode){
		
		// установка начальных значений x_old и y_old
		OCRPoint &p0=pointData[line.pointIndex];
			int x_old=(p0.x-line.xMin) * line.scale0 +4;	
			int y_old=(p0.y-line.yMin) * line.scale0 +4;
		for(int i=0;i<line.lenA;i++){
			OCRPoint &p=pointData[line.pointIndex+i];
			int x=(p.x-line.xMin) * line.scale0 +4;	
			int y=(p.y-line.yMin) * line.scale0 +4;		
			if(1){
			///////////
			/// закрытие пустот (дырок) после масштабирования (увеличения).
		    /// 26 sent 2020
		        int dlt_x=x-x_old;  // расстояние между соседними pix по оси x
		        int absdlt_x=abs(dlt_x);
		        int dlt_y=y-y_old;   // расстояние между соседними pix по оси y
		        int absdlt_y=abs(dlt_y);
				
		        // если  появилась пустота
				if(absdlt_x >1 || absdlt_y >1) {
				      // сохраняем x и y т.к. ниже они будут модифицированны
					  int x_tmp=x;
					  int y_tmp=y;
					  int step_sum=0; // накопителлная переменная
					// устанавливаем  в x и y  адреса начала дыреи
					  x=x_old; 
		              y=y_old;
					 // absdlt_x >= absdlt_y
					 if(absdlt_x >= absdlt_y){  
		                int step_x=0;
		                // вычисляем целый шаг по оси  x равнный +-1
		                if(dlt_x>0)step_x=1; else step_x=-1;         
		                // дробный шаг по оси y  умноженный на 16
	                    int step_y=16*dlt_y/absdlt_x; // *16 чтобы не вводить float
		                 // заполняем пустоту  
			             // цикл по количеству  pix заполняющих дырку
			             for (int n = 0; n < absdlt_x-1; n++) {
			             	// смещаемся на целый шаг по оси x равный +-1
			             	x+=step_x;
					         // смещаемся на дробный шаг по оси y умнженный на 16
					         step_sum+=step_y; // вычисляем дробное расстояние по оси  y от начала дырки умноженное на 16
					         y+=step_sum/16; // к стартовой точке прибавляем расстояние округленное д					о целого
							// рисуем pix в дырке
					 		out << "<rect class=\"p0\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				            << y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
					     } // for
					
			         // absdlt_x  < absdlt_y
			         }else{    
						// меняем местами  x и y, код тот же самый  
		                int step_y=0;
		                // вычисляем целый шаг по оси  x равнный +-1
		                if(dlt_y>0)step_y=1; else step_y=-1;         
		                // дробный шаг по оси y  умноженный на 16
	                    int step_x=16*dlt_x/absdlt_y; // *16 чтобы не вводить float
		                 // заполняем пустоту  
			             // цикл по количеству  pix заполняющих дырку
			             for (int n = 0; n < absdlt_y-1; n++) {
			             	// смещаемся на целый шаг по оси y равный +-1
			             	y+=step_y;
					         // смещаемся на дробный шаг по оси x умнженный на 16
					         step_sum+=step_x; // вычисляем дробное расстояние по оси  x от начала дырки умноженное на 16
					         x+=step_sum/16; // к стартовой точке прибавляем расстояние округленное до целого
							// рисуем pix в дырке
							out << "<rect class=\"p0\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
							<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		 
					     } // for
		             
		             } // else
					// восстанавливаем x и y
					x=x_tmp;
					y=y_tmp;
				} // если  появилась пустота
				
				x_old=x;
				y_old=y;
			
			/////////////
			}
			out << "<rect class=\"p0\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		}
		
	}
	dH += maskH + maskBorder;
    */
}

void GDraw::printClusterMaskSVG(void *cluster_){
    /*
	OCRCluster &cluster = *(OCRCluster*)cluster_;
	k1 = (float)maskH / 32;
	out << "<text class=\"t5\" x=\"" << border * k  + shiftMaskX << "\" y=\"" << dH - 3 << "\">Star:" << cluster.index <<" regCount:" << (int)cluster.cTypeC.regCount 
	<< "</text>" << endl;

	out << "<rect class=\"mask\" x=\"" << border * k + shiftMaskX<< "\" y=\"" << dH << "\" width=\"" << maskH << "\" height=\""
		<< maskH << "\" />" << endl;
	for (int y = 0; y < 32; y++) {
		int data = cluster.cTypeC.cType[y];
		for (int x = 0; x < 32; x++) {
			if ((data >> x) & 1) {
				out << "<rect class=\"p1\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" << y * k1 + dH << "\" width=\""
					<< k1 << "\" height=\"" << k1 << "\" />" << endl;
			}
		}
	}
	for (int y = 0; y < cluster.cTypeC.regCount; y++) {
		int y_=cluster.cTypeC.regY[y];
		int reg=cluster.cTypeC.reg[y];
		//cout<<"_y:"<<y_<<endl;
		//cout<<"reg:"<<bitset<32>(reg)<<endl;
		for (int x = 0; x < 32; x++) {
			if ((reg >> x) & 1) {
				out << "<rect class=\"p0\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" << y_ * k1 + dH << "\" width=\""
					<< k1 << "\" height=\"" << k1 << "\" />" << endl;
			}
		}
	}
    */ 
	/*
	//Записываем в массив mPoint координаты OCRStar кластера и координаты середин линий 
	OCRPoint mPoint[64];
	OCRLine mLine[64];
	int lCount=0;
	int pCount=0;
	mPoint[pCount]=star.cPoint;
	pCount++;
	
	for (int i = 0; i < star.lineCount; i++) {
		OCRLine &line=focalLine[star.line[i]];
		line.status=1;
		mLine[lCount]=line;
		lCount++;
		//mPoint[pCount]=pointArray[line.pointIndex+line.lenA/2];
		//pCount++; 
	}
	
	for (int i = 0; i < star.starCount; i++) {
		OCRStar &star_=starArray[star.star[i]];
		mPoint[pCount]=star_.cPoint;
		pCount++; 
		
		for (int n = 0; n < star_.lineCount; n++) {
			OCRLine &line=focalLine[star_.line[n]];
			//if(line.endID || line.startID == star.index) continue;
			line.status=0;
			mLine[lCount]=line;
			lCount++;
			//mPoint[pCount]=pointArray[line.pointIndex+line.lenA/2];
			//pCount++; 
		}
		
		for (int n = 0; n < star_.starCount; n++) {
			if(star_.star[n] == star.index) continue;
			OCRStar &star__=starArray[star_.star[n]];
			mPoint[pCount]=star__.cPoint;
			pCount++; 
		}
	}
	//Находим минимум и максимум
	int xMin = 100000;
	int xMax = -100000;
	int yMin = 100000;
	int yMax = -100000;
	for (int n = 0; n < pCount; n++) {
		OCRPoint &c = mPoint[n];
		//cout<<"x_:"<<c.x+letterP->xCenter<<" y_:"<<c.y+letterP->yCenter<<endl;
		if (c.x > xMax) xMax = c.x;
		if (c.x < xMin) xMin = c.x;
		if (c.y > yMax) yMax = c.y;
		if (c.y < yMin) yMin = c.y;
	}
	//размеры габарита кластера
	int wCluster = xMax - xMin;
	int hCluster = yMax - yMin;
	float scale;
	int maskW = 23;
	// maskW - нормированный размер габарита кластера меньше 32 для того чтобы
	// оставить место для растискивания
	if (wCluster > hCluster) {
		scale = (float)maskW / wCluster;
	} else {
		scale = (float)maskW / hCluster;
	}
	
	//рисуем кластер
	int x=(star.cPoint.x - xMin)*scale + 4;
	int y=(star.cPoint.y - yMin)*scale + 4;
	//cout<<"@@@ star:"<<star.index<<" scale:"<<scale<<" wCluster:"
	//<<wCluster<<" hCluster:"<<hCluster<<" pCount:"<<pCount<<endl;
	
	out << "<circle class=\"s" << (int)star.type << "\" cx=\"" << x * k1 + border * k + shiftMaskX + k1/2  << "\" cy=\""
		<< y * k1 + dH + k1/2 << "\" r=\"" << maskH / 64 << "\"/>" << endl;
	
	
	//рисуем квадратичную апроксимацию
	for (int n = 0; n < lCount; n++) {
			OCRLine &line=mLine[n];
			OCRPoint *lineData = &pointArray[line.pointIndex];

			OCRPoint &p0=line.start;
			OCRPoint &p1=lineData[line.lenA/2];
			OCRPoint &p3=line.end;
			int Px1;
			int Py1;
			int x0=((p0.x- xMin)*scale + 4 );
			int y0=((p0.y- yMin)*scale + 4 );
			int x1=((p1.x- xMin)*scale + 4 );
			int y1=((p1.y- yMin)*scale + 4 );
			int x3=((p3.x- xMin)*scale + 4 );
			int y3=((p3.y- yMin)*scale + 4 );
			
			Px1 = -0.5 * x0 + 2.0 * x1 - 0.5 * x3;
			// координата “x” первой управляющей точки.
			Py1 = -0.5 * y0 + 2.0 * y1 - 0.5 * y3;

			if(line.status){
				out << "<path class=\"L11\" d=\"M ";
			}else{
				out << "<path class=\"L12\" d=\"M ";
			}
			out<< x0 * k1 + border * k + shiftMaskX + k1/2<< " " 
			<< y0 * k1 + dH + k1/2  << " Q "
			<< Px1 * k1 + border * k + shiftMaskX + k1/2 << " " 
			<< Py1 * k1 + dH + k1/2  << " " 
			<< x3 * k1 + border * k + shiftMaskX + k1/2<< " "
			<< y3 * k1 + dH + k1/2  << "\"/>" << endl;
	}
	*/
					
	dH += maskH + maskBorder;

}

void GDraw::printClusterSVG(void *cluster_, int mode){
	OCRCluster &cluster = *(OCRCluster *)cluster_;
//_______DRAW_CLUSTER_LABLE________
    out << "<text class=\"p0\" x=\"" << shiftX * k + k << "\" y=\""
    << shiftY * k << "\"> Cluster_:" << cluster.sIndex << "</text>" << endl;
    
//_______DRAW_STAR_LINES________
	for (int i = 0; i < cluster.starCount; i++) {
		OCRStar &star = cluster.star[i];
		printOCRStarSVG((void*)cluster.line,(void*)&star);
		//printOCRStarBaseLine((void*)&star);
	}
	// out << "</svg>" << endl; dataStr = out.str(); return;
	
	//_______DRAW_LINE_LABEL____
	//for (int i = 0; i < letter.lineCount; i++) {
		//OCRLine &line = letter.line[i];
		//printOCRLineLabel(line,letter.point);
	//}

	//_______DRAW_POINTS________
	for (int i = 0; i < cluster.starCount; i++) {
		OCRStar &star = cluster.star[i];
		printOCRStarPoint((void*)&star);
		if(i == 0) printOCRStarCluster((void *)&star);
		/*
		if (letter.status == 255 ) {
			float fill=0.5-0.5*star.corr/100;
			out << "<circle style=\"fill:rgba(0,0,0,"<<fill<<")\" cx=\"" << (star.cPoint.x + shiftX) * k << "\" cy=\""
				<< (star.cPoint.y + shiftY) * k << "\" r=\"" << r1 << "\"/>" << endl;
		}
		*/
	}
	
	
	//_______DRAW_LINE_DIR AND_LABEL__________
	for (int i = 0; i < cluster.starCount; i++) {
		OCRStar &star = cluster.star[i];
		printLineDirLable((void*)cluster.line,(void*)&star,0);
	//_______DRAW_STAR_HASH__________
			if(mode)printOCRStarHash((void*)&star);
	}
	if(!mode) return;
	//_______DRAW_LINE_MASK_______
	dH += 32;
	int dH__=dH;
	shiftMaskX=0;
	for (int i = 0; i < cluster.lineCount; i++) {
		OCRLine &line = cluster.line[i];
		printOCRLineMaskSVG((void*)&line,1);
	}
	//_______DRAW_CLUSTER_MASK_______
	dH = dH__;
	shiftMaskX+=maskH + maskBorder;
	printClusterMaskSVG((void*)&cluster);

}//___________________________________________________________

void GDraw::printClusterSVG(void *cluster_, cstr path){
	  init();
	  OCRCluster &cluster = *(OCRCluster*)cluster_;
		imgW = cluster.w;
		imgH = cluster.h;
		graphW = imgW;
		graphH = imgH;
		shiftX = border - cluster.xMin;
		shiftY = border - cluster.yMin + 20;
		xCenter=cluster.cPoint.x;
		yCenter=cluster.cPoint.y;                 
		canvasImgH = (border * 2 + graphH) * k + maskH * cluster.lineCount + maskBorder * cluster.lineCount * 20 + 1000;
		canvasImgW = (border * 2 + graphW) * k;
		if(canvasImgW<800)canvasImgW=800;
        if(canvasImgH<800)canvasImgH=800;
		out.str("");
		printHeadStyle(drawMode);
		printClusterSVG(cluster_, 1);
		out << "</svg></body>" << endl;
  	string dataStr = out.str();
  	ostringstream out__;
  	out__<<path<<"/"<<"_cluster"<<(int)cluster.indexInLetter<<"_"<<(int)cluster.sIndex<<"_"<<cluster.index<<".html";
  	string pathSVG=out__.str();
  	cout<<pathSVG<<endl;
  	writeText(dataStr,pathSVG);
}


void GDraw::printGraphClustersSVG(void *clusterArray_, uint size, cstr path){
	OCRCluster *clusterArray = (OCRCluster *)clusterArray_;
  clearDirectory(path, "img");
  clearDirectory(path, "html");
  cout<<"Start draw size:"<<size<<endl;
  for(int i=0;i<size;i++){
  	OCRCluster &cluster=clusterArray[i];
  	//printCluster((void *)&cluster);
  	printClusterSVG((void *)&cluster,path);
  }
}

void GDraw::drawGraphSVG(void *graph_, void *dataStr_){
  init();
	GGraph &graph = *(GGraph *)graph_;
	string &dataStr = *(string *)dataStr_;
	imgW = graph.columns();
	imgH = graph.rows();
	int starCount = (int)graph.starArray.size();
	//int lineCount = (int)graph.focalLine.size();
	//int clusterCount = (int)graph.clusterArray.size();
	OCRStar *starArray=graph.starArray.dataPtr();
	OCRLine *lineArray=graph.focalLine.dataPtr();
	//OCRCluster *clusterArray = graph.clusterArray.dataPtr();
	xCenter = 0;
	yCenter = 0;
	
	//cout<<"imgW:"<<imgW<<endl; exit(0);
	//определяем габариты графа
	max.x = -10000;
	max.y = -10000;
	min.x = 10000;
	min.y = 10000;

	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		if (star.cPoint.x > max.x) max.x = star.cPoint.x;
		if (star.cPoint.y > max.y) max.y = star.cPoint.y;
		if (star.cPoint.x < min.x) min.x = star.cPoint.x;
		if (star.cPoint.y < min.y) min.y = star.cPoint.y;
	}
	graphW = max.x - min.x;
	graphH = max.y - min.y;
	shiftX = border - min.x;
	shiftY = border - min.y + 20;
	canvasImgH = (border * 2 + graphH) * k + maskH * starCount + maskBorder * starCount * 20;
	canvasImgW = (border * 2 + graphW) * k;
	printHeadStyle(drawMode);
	printImgSVG(graph.bytes_data,1);
	if(graph.coord_fx)printMidLineSVG(graph.coord_fx, graph.coord_fy, (uint)graph.pointArray.size());

	//_______DRAW_STAR_LINES________
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		printOCRStarSVG((void*)lineArray,(void*)&star);
		printOCRStarBaseLine((void*)&star);
	}
	// out << "</svg>" << endl; dataStr = out.str(); return;

	// return;

	//_______DRAW_LINE_LABEL____
	//for (int i = 0; i < letter.lineCount; i++) {
		//OCRLine &line = letter.line[i];
		//printOCRLineLabel(line,letter.point);
	//}

	//_______DRAW_POINTS________
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		printOCRStarPoint((void*)&star);
		//if (letter.status == 255) out<<"</a>" << endl;
		/*
		if (letter.status == 255 ) {
			float fill=0.5-0.5*star.corr/100;
			out << "<circle style=\"fill:rgba(0,0,0,"<<fill<<")\" cx=\"" << (star.cPoint.x + shiftX) * k << "\" cy=\""
				<< (star.cPoint.y + shiftY) * k << "\" r=\"" << r1 << "\"/>" << endl;
		}
		*/
	}

	//рисуем OCRStar ядер кластера
	/*
	for (int i = 0; i < letter.clusterCount; i++) {
		OCRStar &star = starArray[letter.cluster[i]];
		printOCRStarCluster(star);
	}
	*/

	//_______DRAW_LABEL________
	/*
	if (letter.status == 255)out << "<text class=\"t5\" x=\"" << border << "\" y=\""
			<< 15<< "\">Letter correlation:"<<letter.correlation<<"</text>"<<endl;
	int dH_=35;
	int dX_=0;
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		int dl = 0.5 * k;
		if (i > 9) dl = 0.9* k;

		if (letter.status == 255) {
			int flag=0;
			if(i%10==0 && i>0){
				dX_+=290;
				dH_=35;
			}
			for (int n = 0; n < letter.clusterCount; n++) {
				if(letter.cluster[n]==i){
					flag=1;
					 break;
				}
			}
			out<<"<a href=\"__star"<<star.index<<"_graph.html\" >";
			out << "<text class=\"t5\" x=\"" << border + dX_<< "\" y=\""
			<< dH_ << "\">";
			if(flag){
				out <<" (("<<star.index<<"))";
			}else{
				out <<" ("<<star.index<<")";
			}
			out<< " corr:" << (int)star.corr << " match:" << (int)star.match[0]
			<< " scale:" << star.scale <<" level:"<<(int)star.level
			<<"</text></a>" << endl;
			dH_+=15;
			out<<"<a href=\"__star"<<star.index<<"_graph.html\" >";
		}
		out << "<text class=\"t5\" x=\"" << (star.cPoint.x + shiftX) * k - dl << "\" y=\""
			<< (star.cPoint.y + shiftY) * k + 0.5 * k << "\">" << (int)star.index<< "</text>" << endl;
		if (letter.status == 255) out<<"</a>" << endl;
	}
	*/
	
	//_______DRAW_LINE_DIR AND_LABEL__________
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		printLineDirLable((void*)lineArray,(void*)&star,0);
	}
	//_______DRAW_STAR_HASH__________
    dH += 432;
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		printOCRStarHash((void*)&star);
	}
    
    /*
	//_______DRAW_LINE_MASK_______
	dH += 32;
	int dH__=dH;
	shiftMaskX=0;
	for (int i = 0; i < lineCount; i++) {
		OCRLine &line = lineArray[i];
		printOCRLineMaskSVG((void*)&line,1);
	}
	//_______DRAW_CLUSTER_MASK_______
	dH = dH__;
	shiftMaskX+=maskH + maskBorder;
	for (int i = 0; i < clusterCount; i++) {
		OCRCluster &cluster = clusterArray[i];
		printClusterMaskSVG((void*)&cluster);
	}
    */ 
	out << "</svg></body>" << endl;
	dataStr = out.str();

} //______________________________________________________________________________________


void GDraw::printGraphSVG(void *graph_, cstr path){
	GGraph &graph = *(GGraph *)graph_;
	printGraphClustersSVG((void *)graph.clusterArray.dataPtr(), (uint)graph.clusterArray.size(), path);
	string dataStr;
	drawGraphSVG(graph_,(void*)&dataStr);
	string pathSVG=path;
	pathSVG += "/__1_graph.html";
	writeText(dataStr,pathSVG);
}//______________________________________________________________________________________

void GDraw::printLetterSVG(void *letter_, void *font_, cstr path){
	string pathSVG=path;
    clearDirectory(path, "img");
    clearDirectory(path, "html");
	pathSVG += "/__1_letter.html";
	string dataStr;
	drawLetterSVG(letter_, font_, (void *)&dataStr);
    k=3;
    r1=k*2.7;
    r2=k*2.6;
    r3=k*2.5;
    OCRLetter &letter=*(OCRLetter *)letter_;
    OCRFont &font = *(OCRFont *)font_;
    OCRCluster *clusterArray = font.clusterArray.dataPtr();
    for(int i = 0; i < letter.clusterCount; i++){
        OCRCluster &cluster = clusterArray[letter.cluster[i]];
        printClusterSVG((void*)&cluster,path);
    }
    writeText(dataStr,pathSVG);
}

void GDraw::drawLetterSVG(void *letter_, void *font_, void *dataStr_){
    OCRFont &font = *(OCRFont *)font_;
    OCRLetter &letter = *(OCRLetter *)letter_;
    GGraph *graph_;
    
    if(letter.OCRIndex != 100){ //not alphabet letter
        GString &path_=font.pathArray[letter.index];
        string pathStr = path_.data;
        if(pathStr.find("python")!=-1){
            pathStr = run(pathStr);
        }
        //cout<<"path:"<<path<<endl;
        
        string pathPNG = ROOT_PATH;
        pathPNG += "/CORRELATION/__letter.png";
        copyFile(pathStr.c_str(),pathPNG.c_str());
        GBitmap *img = GBitmap::create(pathPNG.c_str());
        GGraph graph(img);
        graph.graphFocalLine(15);
        graph.focalPointXTL();
        graph.focalLineRecognition();
        graph.buildOCRStars();
        graph_ = &graph;
        for(int i=0;i<letter.lineCount && i < graph.focalLine.size();i++){
            OCRLine &line=graph.focalLine[i];
            if(!line.lenA)continue;
            line.scale0=letter.kLine[i];
        }
        drawGraphSVG((void*)graph_,dataStr_);
    }else{
        TString st;
        font.getAlphabetLetter(letter.index,st);
        GGraph graph(st);
        graph_ = &graph;
        for(int i=0;i<letter.lineCount && i < graph.focalLine.size();i++){
            OCRLine &line=graph.focalLine[i];
            if(!line.lenA)continue;
            line.scale0=letter.kLine[i];
        }
        drawGraphSVG((void*)graph_,dataStr_);
        
    }
    return;
}

void GDraw::drawOCRLineCorrelationSVG(void *lineT_, void *line_, void *dataStr_){
	OCRLine &line = *(OCRLine*)line_;
	OCRLine &lineT = *(OCRLine*)lineT_;
	string &dataStr = *(string *)dataStr_;
	
	out.str("");
	border=12;
	canvasImgH = border * 4 + maskH;
	canvasImgW =canvasImgH * 4;
	printHeadStyle(drawMode);
	border=3;
	dH=25;
	shiftMaskX=0;
	printOCRLineMaskSVG((void*)&lineT,0);
	dH=25;
	shiftMaskX+=maskH + maskBorder;
	printOCRLineMaskSVG((void*)&line,0);
	dH=25;
	shiftMaskX+=maskH + maskBorder;
	printOCRLineMaskSVG((void*)&lineT,0);
		
	dH=25;
	if(line.cType){
		//draw line pixel data in 32x32 mask
		shiftMaskX-=maskH + maskBorder;
		/*
		for(int i=0;i<line.lenA;i++){
			OCRPoint &p=pointData[line.pointIndex+i];
			int x=(p.x-line.xMin) * line.scale0 +4;	
			int y=(p.y-line.yMin) * line.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p4\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		}
		*/
		shiftMaskX+=maskH + maskBorder;
		/*			
		for(int i=0;i<line.lenA;i++){
			OCRPoint &p=pointData[line.pointIndex+i];
			int x=(p.x-line.xMin) * line.scale0 +4;	
			int y=(p.y-line.yMin) * line.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p0\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		}
		*/
		
		//draw line mask registers
        /*
		string style;
		float dp=0.7;
		int *lineMask=lineT.maskLine;
		for (int x = 0; x < 32; x++) {
			style="p6";
			if ((line.p0_r >> x) & 1) {
				if ((lineMask[line.p0_y] >> x) & 1) style="p5";
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< line.p0_y * k1 + dH << "\" width=\"" << k1+ dp<<"\" height=\"" << k1+ dp<< "\" />" << endl;
			}
			style="p6";
			if ((line.p1_r >> x) & 1) {
				if ((lineMask[line.p1_y] >> x) & 1) style="p5";
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< line.p1_y * k1 + dH << "\" width=\"" << k1+ dp<< "\" height=\"" << k1+ dp<< "\" />" << endl;
			}
			style="p6";
			if ((line.p2_r >> x) & 1) {
				if ((lineMask[line.p2_y] >> x) & 1) style="p5";
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< line.p2_y * k1 + dH << "\" width=\"" << k1+ dp<< "\" height=\"" << k1+ dp<< "\" />" << endl;
			}
			style="p6";
			if ((line.p3_r >> x) & 1) {
				if ((lineMask[line.p3_y] >> x) & 1) style="p5";
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< line.p3_y * k1 + dH << "\" width=\"" << k1+ dp<< "\" height=\"" << k1+ dp<< "\" />" << endl;
			}
			style="p6";
			if ((line.p4_r >> x) & 1) {
				if ((lineMask[line.p4_y] >> x) & 1) style="p5";
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< line.p4_y * k1 + dH << "\" width=\"" << k1+ dp<< "\" height=\"" << k1+ dp<< "\" />" << endl;
			}
		}
        */
	}//_______________________________________________________________
	
	//int correlation=OCRLineMaskCorrelation(lineT,line);
	//out << "<text class=\"t0\" x=\"" << border * k + shiftMaskX << "\" y=\"" << dH + maskH + 12 << "\">Correlation:" << correlation <<"</text>"<< endl;

	
	out << "</svg></body>" << endl;
	dataStr = out.str();

}//________________________________________________________________________________________________________________

void GDraw::drawOCRLineAffineCorrelationSVG(void *lineT_, void *line_, void *dataStr_, int dir){
	OCRLine &line = *(OCRLine*)line_;
	OCRLine &lineT = *(OCRLine*)lineT_;
	string &dataStr = *(string *)dataStr_;

	out.str("");
	border=12;
	canvasImgH = border * 4 + maskH;
	canvasImgW =canvasImgH * 4;
	printHeadStyle(drawMode);
	border=3;
	dH=25;
	shiftMaskX=0;
	printOCRLineMaskSVG((void*)&lineT,0);
	dH=25;
	shiftMaskX+=maskH + maskBorder;
	printOCRLineMaskSVG((void*)&line,0);
	dH=25;
	shiftMaskX+=maskH + maskBorder;
	printOCRLineMaskSVG((void*)&lineT,0);
	
	
		
	dH=25;
	int correlation=0;
	//draw correlation line pixel data in 32x32 mask
	//общая формула приведения масштаба и сдвига двух кривых
  	  int dxS; ///<расстояние между концами линий по X при совмещении координат начала линии
	int dyS; ///<расстояние между концами линий по Y при совмещении координат начала линии
	//int x= p.x + dxS * lenA / i;  
	// lenA - длина линии i  - номер пиксела от начала линии
	//int y= p.y + dyS * lenA / i; 

	int dX;   
	int dY;
	 int x,y;
	
	if(line.cType){
		
		cout<<" lineT.start32.x:"<<lineT.start32.x<<" lineT.end32.x:"<<lineT.end32.x<<endl;
		cout<<" lineT.start32.y:"<<lineT.start32.y<<" lineT.end32.y:"<<lineT.end32.y<<endl;
		cout<<" line.start32.x:"<<line.start32.x<<" line.end32.x:"<<line.end32.x<<endl;
		cout<<" line.start32.y:"<<line.start32.y<<" line.end32.y:"<<line.end32.y<<endl;
		
		if(dir){
			//вычисляем смешение начала линий
			dX=lineT.start32.x - line.start32.x;
			dY=lineT.start32.y - line.start32.y;
			//вычисляем смещение концов линий при совмещении начала линий
			dxS=lineT.end32.x - line.end32.x - dX;
			dyS=lineT.end32.y - line.end32.y - dY;
		}else{
			//вычисляем смешение начала линий
			dX=lineT.start32.x - line.end32.x;
			dY=lineT.start32.y - line.end32.y;
			//вычисляем смещение концов линий при совмещении начала линий
			dxS=lineT.end32.x - line.start32.x - dX;
			dyS=lineT.end32.y - line.start32.y - dY;
		}
		/*
		if(dxS> 7) dxS = return 0;
		if(dxS < -7) dxS = return 0;
		if(dyS> 7) dyS = return 0;
		if(dyS < -7) dyS = return 0;
		*/
		cout<<"lineT:"<<lineT.index<<" line:"<<line.index<<" dir:"<<dir<<endl;
		cout<<" dX:"<<dX<<" dY:"<<dY<<" dxS:"<<dxS<<" dyS:"<<dyS<<endl;
		
		//float scale=line.scale0;
		
		//рисуем тестовую маску
		shiftMaskX-=maskH * 2 + maskBorder *2;
		/*
		
		for(int i=0;i<lineT.lenA;i++){
			OCRPoint &p=pointDataT[lineT.pointIndex+i];
			x=(p.x-lineT.xMin) * lineT.scale0 +4;	
			y=(p.y-lineT.yMin) * lineT.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p4\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		}
		*/
		OCRPoint &p_1=lineT.start;
			x=(p_1.x-lineT.xMin) * lineT.scale0 +4;	
			y=(p_1.y-lineT.yMin) * lineT.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p7\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		
		OCRPoint &p_2=lineT.end;
			x=(p_2.x-lineT.xMin) * lineT.scale0 +4;	
			y=(p_2.y-lineT.yMin) * lineT.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p7\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		shiftMaskX+=maskH + maskBorder;

		//рисуем эталонную маску
		/*
		for(int i=0;i<line.lenA;i++){
			OCRPoint &p=pointData[line.pointIndex+i];
			x=(p.x-line.xMin) * line.scale0 +4;	
			y=(p.y-line.yMin) * line.scale0 +4;
			//cout<<"point x:"<<x<<" y:"<<y<<endl;	
			out << "<rect class=\"p4\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
		}
		*/
		shiftMaskX+=maskH + maskBorder;
					
		//проверяем совпадение точек линии по маске проверяемой линии с приведеннием
		//масштаба по X Y и совмещением начала и конца линии
		//рисуем эталонную линию поверх проверяемой линии
		float stepX=(float)dxS /(line.lenA -1);
		float stepY=(float)dyS /(line.lenA -1);
		int xShift;
		int yShift;
		//int dX_=lineT.start32.x;
		//int dY_=lineT.start32.y;
		if(dir){
			xShift=line.start.x;
			yShift=line.start.y;
		}else{
			xShift=line.end.x;
			yShift=line.end.y;
		}
		cout<<" stepX:"<< stepX << " stepY:" << stepY << endl;
		/*
		for(int i=0;i<line.lenA;i++){
			OCRPoint &p=pointData[line.pointIndex+i];
			if(dir){
				x= (p.x - xShift) * scale  //точка в координатах маски  без смещения
				+ stepX * i      // добавляем относительное смещение вдоль кривой
				+ dX_;  // передвигаем линию в начало проверяемой линии 
			
				y=(p.y - yShift) * scale+ stepY * i + dY_; 
			}else{
				x= (p.x - xShift) * scale + stepX * (line.lenA - i - 1) + dX_; 
				y= (p.y - yShift) * scale+ stepY * (line.lenA - i - 1) + dY_; 
			}
			out << "<rect class=\"p0\" x=\"" <<  x * k1 + border * k + shiftMaskX << "\" y=\"" 
			<< y * k1 + dH << "\" width=\"" << k1 << "\" height=\"" << k1 << "\" />" << endl;		
			
			//cout<<"i:"<<i<<" stepX:"<< (int)(stepX * i)<<" stepY:"<< (int)(stepY * i)<<" dX_:"<<dX_<<" dY_:"<<dY_;
			//cout<<"   point p.x_:"<<(int)((p.x - xShift) * scale) <<" p.y_:"<<(int)((p.y - yShift) * scale) <<" x:"<<x<<" y:"<<y<<endl;	
			
		}
		*/
        /*
		//draw line mask registers
		string style;
		float dp=0.7;
		int *lineMask=lineT.maslLine;
		int dl;
		
		
		for(int indexP=0;indexP<5;indexP++){
			style="p6";
			OCRPoint &p0=line.pointC[indexP];
			dl=p0.data;
			cout<<"@ p0.x:"<<p0.x<<" p0.y:"<<y<<endl;	
			cout<<"lineT:"<<lineT.index<<" line:"<<line.index<<" dir:"<<dir<<endl;
			cout<<" dX:"<<dX<<" dY:"<<dY<<" dxS:"<<dxS<<" dyS:"<<dyS<<endl;
			
			cout<<" dl:"<<dl<<" stepX:"<< (int)(stepX * dl)<<" stepY:"<< (int)(stepY * dl)<<" dX_:"<<dX_<<" dY_:"<<dY_<<" xShift:"<<xShift<<" yShift:"<<yShift<<endl;
			cout<<" point p.x_:"<<(int)((p0.x - xShift) * scale) <<" p.y_:"<<(int)((p0.y - yShift) * scale)<<endl;	
			
			
			if(dir){
				x= (p0.x - xShift) * scale  + stepX * dl + dX_;  
				y=(p0.y - yShift) * scale+ stepY * dl + dY_; 
			}else{
				x= (p0.x - xShift) * scale + stepX * (line.lenA - dl - 1) + dX_; 
				y= (p0.y - yShift) * scale+ stepY * (line.lenA - dl- 1) + dY_; 
			}
			if ((lineMask[y] >> x) & 1) {
				correlation += p0.status;
				style="p5";
			}
				out << "<rect class=\"" << style << "\" x=\"" << x * k1 + border * k + shiftMaskX << "\" y=\"" 
				<< y * k1 + dH << "\" width=\"" << k1+ dp<<"\" height=\"" << k1+ dp<< "\" />" << endl;
			cout<<"@ point0 x:"<<x<<" y:"<<y<<" c:"<<correlation<<endl;	
		}
		
		*/
	
	}
	out << "<text class=\"t0\" x=\"" << border * k + shiftMaskX - maskH<< "\" y=\"" << dH + maskH + 12 << "\">dxS:"<<dxS<<" dyS:"<<dyS<<" Correlation:" << correlation <<"</text>"<< endl;
	
	out << "</svg></body>" << endl;
	dataStr = out.str();

}




int GDraw::drawOCRStarCorrelation(void *starT_,void *star_,void *linesT_,void *lines_, cstr path){

	int correlation = 0;
	//if(starT.type != star.type) return 0;
	//if(star.type == O_POINT && starT.type == O_POINT) return 100;
/*	
	OCRStar &starT, 
	OCRStar &star, 
	OCRLine *linesT, 
	OCRLine *lines ,

	string dataStr,dataStr_;
	string path_;
	ostringstream out_;
	out.str("");
	k = 7;
	dH = border;
	graphW = 700;
	graphH = 300;
	canvasImgH = 300;
	canvasImgW = 700;
	printHeadStyle(drawMode);
	//выводим соответствие поисковых хеш-регистров uint64
	//тестовои и эталоннойOCRStar
	dH=10;
	out << "<text class=\"t5\" x=\"" << border * k / 2 << "\" y=\"" 
	<< dH << "\" >starT angle: ";
	for(int n=0;n<starT.lineCount;n++)out<<starT.angle[n]<<"  ";
	out<<"  star angle: ";
	for(int n=0;n<star.lineCount;n++)out<<star.angle[n]<<"  ";
	out<<"</text>"<<endl;
	dH=25;
	printOCRStarHash(starT);
	string style;
	bitset<64> v0=bitset<64>(star.reg.reg0_0);
	bitset<64> v0T=bitset<64>(starT.reg.reg0_1);
	dH=25;
	for(int i=0;i<64;i++){
		if(v0[i]){ 
			style="p5";
			if(!v0T[i])style="p6";
			out <<"<rect class=\""<<style<<"\"  x=\"" << border * k / 2 + i * pW << "\" y=\"" 
			<< dH + 8 << "\" width=\"" << pW << "\" height=\"" << pW<< "\" />" << endl;
		}
	}
	dH+=pW+14;
	bitset<64> v1=bitset<64>(star.reg.reg1_0);
	bitset<64> v1T=bitset<64>(starT.reg.reg1_1);
	for(int i=0;i<64;i++){
		if(v1[i]){ 
			style="p5";
			if(!v1T[i])style="p6";
			out <<"<rect class=\""<<style<<"\"  x=\"" << border * k / 2 + i * pW << "\" y=\"" 
			<< dH + 8 << "\" width=\"" << pW << "\" height=\"" << pW<< "\" />" << endl;
		}
	}
	dH+=pW+14;
	bitset<64> v2=bitset<64>(star.reg.reg2_0);
	bitset<64> v2T=bitset<64>(starT.reg.reg2_1);
	for(int i=0;i<64;i++){
		if(v2[i]){ 
			style="p5";
			if(!v2T[i])style="p6";
			out <<"<rect class=\""<<style<<"\"  x=\"" << border * k / 2 + i * pW << "\" y=\"" 
			<< dH + 8 << "\" width=\"" << pW << "\" height=\"" << pW<< "\" />" << endl;
		}
	}
	
	dataStr_=out.str();
	out_<<dataStr_<<"</svg></br>"<<endl;
	
	//вычисляем порядок соответствия лучей тестовой и эталонной OCRStar
	int min = 360;
	int minIndex = 0;
	int limit=star.starCount;
	int limitT=starT.starCount;		
	
	if(limit==limitT ){
		if(star.type != L_POINT){
			int angle = star.angle[0];
			for (int m = 0; m < limit; m++) {
				int dl = abs(starT.angle[m] - angle);
				if (dl >= 180) dl = 360 - dl;
				if (dl < min) {
					min = dl;
					minIndex = m;
				}
			}
		}
		//проверяем корреляции соответствующих лучей OCRStar
		for (int index = 0; index < limit; index++) {
			OCRLine &line = lines[star.line[index]];
			int indexT=minIndex+index;
			if(indexT>=limit)indexT-=limit;
			OCRLine &lineT = linesT[starT.line[indexT]];
			ostringstream out1;
			out1<<path<<"/_star_"<<star.index<<"_line_"<<index<<"_graph.html";
			path_=out1.str();	
			out_<<"<iframe src=\""<<path_<<"\" style=\"width:1024px;height:200px;"
			<<"position:absolute;top:"<<100+index*200<<"px;border:10x;\" ></iframe>\n";
			
			if(line.SumBitOn>lineT.SumBitOn){
				correlation+=OCRLineMaskCorrelation(lineT,line);
				drawOCRLineCorrelationSVG(lineT, line,pointData, dataStr);
			}else{
				correlation+=OCRLineMaskCorrelation(line,lineT);
				drawOCRLineCorrelationSVG(line, lineT,pointDataT, dataStr);
			}
			writeText(dataStr,path_);					
			}
		correlation/=limit;
	}else{
		//проверяем корреляции соответствующих лучей OCRStar
		int count=0;
		for (int index = 0; index < limit; index++) {
			OCRLine &line = lines[star.line[index]];
			int angle = star.angle[index];
			min=360;
			for(int m=0;m<limitT;m++){
				int dl = abs(starT.angle[m] - angle);
				if (dl >= 180) dl = 360 - dl;
				if (dl < min) {
					min = dl;
					minIndex = m;
				}	
			}
			if(min>35){
				if(line.lenA>line.lineW * SERIF_LIMIT){
					cout<<"serif limi:"<<index<<endl;
					count++;
				}
				continue;
			}
			ostringstream out1;
			OCRLine &lineT = linesT[starT.line[minIndex]];
			out1<<path<<"/_star_"<<star.index<<"_line_"<<index<<"_graph.html";
			path_=out1.str();	
			out_<<"<iframe src=\""<<path_<<"\" style=\"width:1024px;height:200px;"
			<<"position:absolute;top:"<<100+index*200<<"px;border:10x;\" ></iframe>\n";
			
			if(line.SumBitOn>lineT.SumBitOn){
				correlation+=OCRLineMaskCorrelation(lineT,line);
				drawOCRLineCorrelationSVG(lineT, line,pointData, dataStr);
			}else{
				correlation+=OCRLineMaskCorrelation(line,lineT);
				drawOCRLineCorrelationSVG(line, lineT,pointDataT, dataStr);
			}
			writeText(dataStr,path_);		
			count++;
		}
		correlation/=count;
	}	
	//cout<<" correlation l2:"<<correlation<<endl;
	
	out_<< "<a href=\"___graph2.html\">";
	out_<<"<div style=\"position:absolute;top:"<<limit*200+120<<"px;\" >Star: "<<star.index<< " starT:"<<starT.index<<" correlation:"<<correlation<<"<br/>Back ⏮</div></a></body>\n";
	dataStr=out_.str();
	out_.str("");
	out_<<path<<"/__star"<<star.index<<"_graph.html";
	path_=out_.str();
	writeText(dataStr,path_);
	if(drawMode){
		dataStr="<html><body style='background-color:black'>\n";
	}else{
		dataStr="<html><body style='background-color:white'>\n";
	}	
	dataStr+="<iframe src=\"___graph1.html\" border=1px style=\"\
	position:absolute;top:0px;left:0px; width:500px;height:1200px\"></iframe>\
	<iframe src=\"___graph2.html\" border=1px \
	style=\"position:absolute;top:0px;left:510px; width:500px;height:1200px\"/></iframe></body></html>";
	out_.str("");
	out_<<path<<"/___correlation_graph.html";
	path_=out_.str();
	writeText(dataStr,path_);	
*/
	//cout<<"star:"<<star.index << " starT:"<<starT.index<< " correlation:"<<correlation<<endl;
	return correlation;							
}


void GDraw::drawCorrelation() {
/*
	GGraph &graph, OCRLetter &letter
	// string str;
	// readText(str, "/_Image2OCR/edit/OCR/OCR.html");

	string dataStr;
	drawLetterSVG(letter, dataStr);
	writeText(dataStr, "/_Image2OCR/edit/OCR/letter.svg");

	drawGraphSVG(graph, dataStr);
	writeText(dataStr, "/_Image2OCR/edit/OCR/graph.svg");
*/
}

/** @bref вывод в SVG изображения всех ClusterMatch буквы */

void GDraw::drawLetterClustersSVG() {
	/*
	OCRLetter &letter, cstr path
	cout << "letter:" << letter.letterIndex << " name:" << letter.name.data << endl;
	ulong size2;
	ClusterMatch *letterCluster = (ClusterMatch *)letterClusterTable->getPtr(letter.letterIndex, size2);
	int clusterCount = (int)size2 / sizeof(ClusterMatch);
	//проверяем количество совпадений кластеров эталонной буквы и кластеров в области поиска
	int countCorrelation = 0;
	for (int index3 = 0; index3 < clusterCount; index3++) {
		ClusterMatch &match = letterCluster[index3];
		//проверяем есть ли такой кластер в зоне поиска
		OCRLetter letter_;
		readLetter(letter_, match.letterIndex);
		cout << "_match letterID:" << letter_.letterID << " match:" << match.clusterID
			 << " starT:" << (int)match.starTIndex << " star:" << (int)match.starIndex << " name:" << letter_.name.data
			 << endl;
	}
	cout << endl;
	*/
}//__________________
  
void GDraw::printClusterMatch(void *letterMatch_,void *textMatch_,void *font_,void *graph_, cstr path){
//void  GDraw::printMatch(int clusterID,int matchIndex,void *font_,int sIndex,void *graph_,cstr path){

    clearDirectory(path, "html");
    OCRFont &font = *(OCRFont *)font_;
    GGraph &graph = *(GGraph *)graph_;
    OCRCluster &textMatch = *(OCRCluster *)textMatch_;
    ClusterMatch &letterMatch = *(ClusterMatch*)letterMatch_;
    OCRCluster &letterCluster = font.clusterArray[letterMatch.letterClusterID];
    OCRCluster &baseCluster = font.baseClusterArray[textMatch.baseClusterID];
    OCRCluster &textCluster = graph.clusterArray[textMatch.index];
    cout<<"start draw letter star:"<<letterMatch.sIndex<<" letter cluster:"<<letterMatch.letterClusterID<<" base cluster:"<<(int)textMatch.baseClusterID<<" graph star:"<<textMatch.sIndex<<endl;

    printClusterSVG((void *)&letterCluster,path);
    printClusterSVG((void *)&baseCluster,path);
    printClusterSVG((void *)&textCluster,path);

}

void GDraw::printLetterMatch(void *letterMatch_,void *font_, cstr path){
    clearDirectory(path, "html");
    OCRFont &font = *(OCRFont *)font_;
    OCRLetterMatch &letterMatch = *(OCRLetterMatch*)letterMatch_;
    OCRLetter &letter=font[letterMatch.testIndex];
    
    memcpy(letter.kLine, letterMatch.kLine, letterMatch.lineCount*sizeof(float));
    
    printLetterSVG((void*)&letter, (void*)&font, path);

}
 
void GDraw::drawLetterCorrelationSVG() {
	/*
	OCRLetter &letterTest, OCRLetter &letter, cstr path
	vector<string> fileList;
	string path_ = path;
	readDirectoryToArray(fileList, path_, "html");
	for (int i = 0; i < fileList.size(); i++) { unlink(fileList[i].c_str()); }
	string dataStr_;
	string str = path_ + "/___graph1.html";
	drawLetterSVG(letterTest, dataStr_);
	writeText(dataStr_, str);
	str = path_ + "/___graph2.html";
	letter.status = 255;
	drawLetterSVG(letter, dataStr_);
	writeText(dataStr_, str.c_str());
	
	for (int i = 0; i < starCount; i++) {
		OCRStar &star = starArray[i];
		if(star.match[0]<0)continue;
		OCRStar &starT = letterTest.star[star.match[0]];
		string dataStr;
		drawOCRStarCorrelation(starT,star,letterTest.line, letter.line, letterTest.point,letter.point, path);
	}
	*/
}//__________________________________________________________________________	
	
void GDraw::calculateBezierCurveHandlers(OCRPoint &p0,OCRPoint &p1, OCRPoint &p2, OCRPoint &p3){

    // Функция на входе получает координаты четырех исходных точек, узлов (t=0, 1/3, 2/3, 1), всего 8 цифр.
    // Возвращает декартовы координаты вершин управляющего многоугольника Безье или что тоже, в полярных координатах
    // длинны и углы, управляющих точек (векторов) кривой Безье, всего 8 цифр.
    // Фактически переход в полярные координаты //
    // Для повышения точности вычисления можно производить с данными типа float
    // Вычисляем координаты управляющих точек кривой Безье (точек апроксимирующех линию)
    // по координатам исходных точек, расположенных на сглаженной фокальной линии mData:
    int x0=p0.x;
    int y0=p0.y;
    int x1=p1.x;
    int y1=p1.y;
    int x2=p2.x;
    int y2=p2.y;
    int x3=p3.x;
    int y3=p3.y;

    p1.x=(-5.0*x0 + 18.0*x1 - 9.0*x2 + 2.0*x3)/6.0;  // координата “x” первой управляющей точки.
    p1.y=(-5.0*y0 + 18.0*y1 - 9.0*y2 + 2.0*y3)/6.0;  // координата “y” первой управляющей точки.
    p2.x=( 2.0*x0 - 9.0*x1 + 18.0*x2 - 5.0*x3)/6.0;  // координата “x” второй управляющей точки.
    p2.y=( 2.0*y0 - 9.0*y1 + 18.0*y2 - 5.0*y3)/6.0;  // координата “y” второй управляющей точки.
    //float Px3=x3;   // Начальные и конечные управляющие точки кривой Безье совпадают
    //float Py3=y3;   // с начальными и конечными точками (узлами) фокальной линии.
    // Источники:
    // Роджерс Д., Адамс Дж. Математические основы машинной графики. М.: Мир, 2001. 604 с.
    // http://sernam.ru/book_mm3d.php?id=92 // это Адамс
    // http://alex-black.ru/article.php?content=109
    // http://fullref.ru/job_a517a7c403e9395d49dc5ea305453db0.html

}
    
    
} // namespace ocr











