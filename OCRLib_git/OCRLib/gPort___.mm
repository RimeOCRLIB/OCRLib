//
//  gPort.m
//  OCRLib
//
//  Created by dharmabook on 15/07/15.
//
//

#import "gPort.h"
#import "gWindow.h"
#include <iostream>
#include "OCRAlgorihtm.h"
#include "GFontEditor.h"
#include "VStr2D.h"


using namespace std;

@implementation gPort

- (void)drawRect:(NSRect)dirtyRect {
    
    if(drawMode==DRAW_VECTOR)[self drawVectorPort];
    if(drawMode==DRAW_LETTERBASE)[self drawLetterBase];
    if(drawMode==DRAW_LETTER)[self drawLetter];
    selectLine.resize(0);//
    selectLine.push_back(2349);//
    

}

-(void)drawVectorPort{
    //[super drawRect:dirtyRect];
    drawRect=[[self superview]frame];   //size of Window
    [self setFrame:drawRect];
    NSTextField *textField;
    
    NSButton *button=[[self superview] subviews][1];
    [button setFrame:NSMakeRect(0,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][2];
    [button setFrame:NSMakeRect(64,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][3];
    [button setFrame:NSMakeRect(128,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][4];
    [button setFrame:NSMakeRect(192,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][5];
    [button setFrame:NSMakeRect(256,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][6];
    [button setFrame:NSMakeRect(320,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][7];
    [button setFrame:NSMakeRect(384,drawRect.size.height-34,64,34)];
    textField=[[self superview] subviews][8];
    [textField setFrame:NSMakeRect(448,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][9];
    [button setFrame:NSMakeRect(512,drawRect.size.height-34,140,34)];

    
    
    if(zoom>0.6){
        [self drawPechaImage];
        [self drawVector];
    }
    //[self drawPoints];
    //cout<<"load image"<<endl;
    // Drawing code here.

}

-(IBAction)moveLeft:(id)sender{
    dX+=dl;
    ((GStr<int>*)inputData.pref)->put(11,dX);
    [self display];

}
-(IBAction)moveRight:(id)sender{
    dX-=dl;
    ((GStr<int>*)inputData.pref)->put(11,dX);
    [self display];
}
-(IBAction)moveUp:(id)sender{
    dY+=dl;
    ((GStr<int>*)inputData.pref)->put(12,dY);
    [self display];
    
}
-(IBAction)moveDown:(id)sender{
    dY-=dl;
    ((GStr<int>*)inputData.pref)->put(12,dY);
    [self display];
}

-(IBAction)zoomIn:(id)sender{
    int x=drawRect.size.width/2-dX;
    int w=(x/zoom)*(zoom+0.1)-x;
    int y=drawRect.size.height/2-dY;
    int h=(y/zoom)*(zoom+0.1)-y;
    zoom+=0.1;
    dX-=w;
    dY-=h;
    ((GStr<int>*)inputData.pref)->put(10,zoom*100);
    ((GStr<int>*)inputData.pref)->put(11,dX);
    ((GStr<int>*)inputData.pref)->put(12,dY);
    
    [self display];
    
}
-(IBAction)zoomOut:(id)sender{
    int x=drawRect.size.width/2-dX;
    int w=(x/zoom)*(zoom-0.1)-x;
    int y=drawRect.size.height/2-dY;
    int h=(y/zoom)*(zoom-0.1)-y;
    zoom-=0.1;
    if(zoom<0.2){
        zoom=0.2;
    }else{
        dX-=w;
        dY-=h;
        [self display];
    }
    ((GStr<int>*)inputData.pref)->put(10,zoom*100);
    ((GStr<int>*)inputData.pref)->put(11,dX);
    ((GStr<int>*)inputData.pref)->put(12,dY);
    
}

-(IBAction)homeImg:(id)sender{
    zoom=1;
    dX=0;
    dY=0;
    [self display];
    
}

-(void)setLetterBase:(string)path{
    drawMode=DRAW_LETTERBASE;
    GImageEditor *imageEditor=(GImageEditor*)inputData.mainEditor;
    int pos=(int)path.find("?")+1;
    path=substr(pos,(int)path.size()-pos,path);
    destString="";
    inputData.data["QUERY_STRING"]=path;
    parseQuery();
    if(inputData.data["db"]=="letter"){
        GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
        if(aliKali->size()){
            GBitmap *img=aliKali->drawOCRBaseImage(0,100);
            pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
            pechaSize=[pechaImage size];
            dX=0;
            dY=-250;
            scale=1;
            zoom=1;
            //aliKali->drawHTML(0,100,result,mode);
        }else{
            destString="no letters";
        }
    }
   
    
    [self display];
}

-(void)setLetter:(int)index{
    drawMode=DRAW_LETTER;
    GImageEditor *imageEditor=(GImageEditor*)inputData.imageEditor;
    
    destString="";
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    if(index>-1&&index<aliKali->size()){
        letter=aliKali->getLetter(index);
        GBitmap *img=letter->drawLetterPict(1);
        pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
        pechaSize=[pechaImage size];
        dX=0;
        dY=0;
        scale=6;
        zoom=1;
        //aliKali->drawHTML(0,100,result,mode);
        lineVec.resize(0);
        for(int i=0;i<letter->focalLine.size();i++){
            
            OCRFocalLine line;
            line=letter->focalLine[i];
            line.start.x+=PICT_SIZE/2;
            line.start.y+=PICT_SIZE/2;
            line.end.x+=PICT_SIZE/2;
            line.end.y+=PICT_SIZE/2;
            line.centerM.x+=PICT_SIZE/2;
            line.centerM.y+=PICT_SIZE/2;
            line.center.x+=PICT_SIZE/2;
            line.center.y+=PICT_SIZE/2;
            line.p1.x+=PICT_SIZE/2;
            line.p1.y+=PICT_SIZE/2;
            line.p2.x+=PICT_SIZE/2;
            line.p2.y+=PICT_SIZE/2;
            
            for(int j=0;j<line.data.size();j++){
                line.data[j].x+=PICT_SIZE/2;
                line.data[j].y+=PICT_SIZE/2;
            }
            for(int j=0;j<line.mData.size();j++){
                line.mData[j].x+=PICT_SIZE*8;
                line.mData[j].y+=PICT_SIZE*8;
            }
            lineVec.push_back(line);
        }
        
    }else{
        destString="no letters";
    }
    [self display];
}

-(void)setPechaImage{
    //NSString *filePath=[NSString stringWithUTF8String:"/BUDDISM.RU/_CHO_SPYOD/RIME_GYUNKER/BAI_RO_RGYUD_BUM/BAI_RO_RGYUD_BUM_W21519/W21519-0577-eBook_tif/page_0228.tif"];
    //pechaImage=[[NSImage alloc] initWithContentsOfFile:filePath]; //open Image;
    GImageEditor *imageEditor=(GImageEditor*)inputData.mainEditor;
    //string inputPath="/BUDDISM.RU/_CHO_SPYOD/RIME_GYUNKER/BAI_RO_RGYUD_BUM/BAI_RO_RGYUD_BUM_W21519/W21519-0577-eBook_tif/page_0228.tif";
    //string inputPath="/_TEST__MEDIA/W21519-0577-eBook_tif/page_0066.tif";
    string pathText;
    ((GVector*)inputData.prefVector)->getStr(20,pathText);
    if(!is_file(pathText))return;
    GBitmap* pechaImg=imageEditor->LoadImageData(pathText,0);
    //GBitmap* pechaImgOCR=GBitmap::create(pechaImg->columns()+64,pechaImg->rows()+64);
    //pechaImgOCR->drawImg(pechaImg, 32, 32);
    GGraph pechaGraph=GGraph(pechaImg);
    
    pechaGraph.focalLine(15);
    pechaGraph.focalPointXTL(strPoint);
    pechaGraph.focalLineRecognition(strPoint,lineVec);
    
    VStr2D<int>point2D;                ///<двухмерная матрица поиска индекса точки по значению координаты X
    VStr2D<int>line2D;                 ///<двухмерная матрица поиска индекса линии по значению координаты X ее центра массы
    pechaGraph.setLineMatrix(strPoint, lineVec, point2D, line2D,3);
    //pechaGraph.setLineCombination(strPoint, lineVec, point2D, line2D);

    //VStr2D<int>pointMatrix;
    //VStr2D<int>lineMatrix;
    //pechaGraph.setLineMatrix(strPoint, lineVec,pointMatrix,lineMatrix);
    
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)&pechaGraph);
    pechaSize=[pechaImage size];

    dl=100;
    zoom=((GStr<int>*)inputData.pref)->get(10);
    zoom/=100;
    if(zoom<0.2)zoom=1;
    dX=((GStr<int>*)inputData.pref)->get(11);
    dY=((GStr<int>*)inputData.pref)->get(12);
    drawMode=DRAW_VECTOR;

}

-(void)drawPoints{
    for (int i=0;i<strPoint.size();i++){
       [NSBezierPath setDefaultLineWidth:2*zoom];
       [[NSColor greenColor] set];
       [NSBezierPath strokeRect:
        NSMakeRect((strPoint[i].x+.5)*scale*zoom+dX,
                   drawRect.size.height*zoom-(strPoint[i].y+.5)*scale*zoom+dY,
                   1,1)];
    
    }

}

-(void)drawVector{
    //short index;
    NSPoint startPoint,endPoint,centerPoint,centerM,cPoint1,cPoint2;
    float fSize;
    if(zoom<2.3)fSize=9*zoom;
    if(zoom>=2.3)fSize=5*zoom;
    float pD=2;
    float pR=pD/2;
    float sL=2;
    
    if(drawMode==DRAW_LETTER){
        sL=0.5;
        pD=8;
    
    }
    
    [NSBezierPath setDefaultLineWidth:2*zoom/sL];
    NSBezierPath *path=[NSBezierPath bezierPath];
    NSBezierPath *path9=[NSBezierPath bezierPath];    //Bezier first line

    [NSBezierPath setDefaultLineWidth:1*zoom/sL];
    NSBezierPath *path1=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:3*zoom/sL];
    NSBezierPath *path2=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:3*zoom/sL];
    NSBezierPath *path3=[NSBezierPath bezierPath];
    NSBezierPath *path6=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:0.8*zoom/sL];
    NSBezierPath *path4=[NSBezierPath bezierPath];    //mData path
    NSBezierPath *path5=[NSBezierPath bezierPath];
    NSBezierPath *path7=[NSBezierPath bezierPath];    //Bezier handlers lines
    NSBezierPath *path8=[NSBezierPath bezierPath];    //Bezier handlers points
    NSBezierPath *path10=[NSBezierPath bezierPath];    //line center of mass
    NSBezierPath *path11=[NSBezierPath bezierPath];    //line center of mass
    
    
    NSColor *strColor=[NSColor colorWithCalibratedRed:0.7 green:0.7 blue:0.7 alpha:1];
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:fSize] forKey: NSFontAttributeName];
    
    int count=(uint)lineVec.size();
    
    //cout<<"@@@zoom="<<zoom<<endl;
    
    for (int index=0; index<count;index++) {
        
        centerPoint=NSMakePoint((lineVec[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineVec[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        centerM=NSMakePoint((lineVec[index].centerM.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineVec[index].centerM.y+.5)*scale*zoom+dY);
        startPoint=NSMakePoint((lineVec[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineVec[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineVec[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineVec[index].end.y+.5)*scale*zoom+dY);
        cPoint1=NSMakePoint((lineVec[index].p1.x+.5)*scale*zoom+dX,
                            drawRect.size.height*zoom-(lineVec[index].p1.y+.5)*scale*zoom+dY);
        cPoint2=NSMakePoint((lineVec[index].p2.x+.5)*scale*zoom+dX,
                            drawRect.size.height*zoom-(lineVec[index].p2.y+.5)*scale*zoom+dY);
        
        //centerPoint=NSMakePoint(lineVec[index].data[lineVec[index].data.size()/2].x*scale*zoom+dX,
        //    drawRect.size.height*zoom-lineVec[index].data[lineVec[index].data.size()/2].y*scale*zoom+dY);
        
        // Чем ближе коэффициент корреляции foat к 100, тем больше кривая похожа на прямую.
        // Тем меньше прямых на рисунке.
        //if(lineVec[index].corL>97.0){
        
        if(lineVec[index].maxCor==LINE){
            [path moveToPoint:startPoint];
            [path lineToPoint:endPoint];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 lineToPoint:endPoint];
            }
        }else if(lineVec[index].maxCor==ARC){
            [path3 moveToPoint:startPoint];
            [path3 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            }
        }else if(lineVec[index].maxCor==SPIRAL){
            [path6 moveToPoint:startPoint];
            [path6 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            }
            //draw Bezier handlers
        }else if(lineVec[index].maxCor==CIRCLE){
            //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
            //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
            int w=lineVec[index].radius*scale*zoom;
            int r=lineVec[index].radius*scale*zoom/2;
            //int w=2;
            [path2 appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                             centerPoint.y-r,w,w)];
            //cout<<"yc="<<lineVec[index].start.x<<endl;
        }
        [path10 appendBezierPathWithOvalInRect:NSMakeRect(centerM.x-pD/2*zoom,
                                                          centerM.y-pD/2*zoom,pD*zoom,pD*zoom)];

        if(zoom>4.5&&(lineVec[index].maxCor==SPIRAL||lineVec[index].maxCor==ARC)){
            [path7 moveToPoint:startPoint];
            [path7 lineToPoint:cPoint1];
            [path7 moveToPoint:endPoint];
            [path7 lineToPoint:cPoint2];
            [path8 appendBezierPathWithRect:NSMakeRect(cPoint1.x-pD/2*zoom,
                                                       cPoint1.y-pD/2*zoom,pD*zoom,pD*zoom)];
            [path8 appendBezierPathWithRect:NSMakeRect(cPoint2.x-pD/2*zoom,
                                                       cPoint2.y-pD/2*zoom,pD*zoom,pD*zoom)];
            
        }

        
        //[[NSString stringWithFormat:@"%d:%d", lineVec[index].angl_t,lineVec[index].index]
        // drawAtPoint:centerPoint withAttributes: attrs];
        
        //if(zoom>1.5)[[NSString stringWithFormat:@"%d", lineVec[index].alpha]
        // drawAtPoint:centerPoint withAttributes: attrs];;
        
        [path4 moveToPoint:startPoint];
        for(int t=0;t<lineVec[index].mData.size();t++){
            //cout<<"x="<<lineVec[index].data[t].x<<" y="<<lineVec[index].data[t].y<<endl;
            startPoint=NSMakePoint(((float)lineVec[index].mData[t].x+.5)*scale*zoom+dX,
                                   drawRect.size.height*zoom-((float)lineVec[index].mData[t].y+.5)*scale*zoom+dY);
            [path4 lineToPoint:startPoint];
        }
        

        for(int t=0;t<lineVec[index].test.size();t++){
            //cout<<"@@@@@@x="<<lineVec[index].test[t].x<<" y="<<lineVec[index].test[t].y<<endl;
            startPoint=NSMakePoint(((float)lineVec[index].test[t].x+.5)*scale*zoom+dX,
                                   drawRect.size.height*zoom-((float)lineVec[index].test[t].y+.5)*scale*zoom+dY);
            [path11 appendBezierPathWithRect:NSMakeRect(startPoint.x-pD/2*zoom,
                                                       startPoint.y-pD/2*zoom,pD*zoom,pD*zoom)];
        }

        
        /*for(int t=0;t<lineVec[index].data.size();t++){
         //cout<<"x="<<lineVec[index].data[t].x<<" y="<<lineVec[index].data[t].y<<endl;
         if(lineVec[index].data[t].type==C_POINT){
         startPoint=NSMakePoint((lineVec[index].data[t].x+.5)*scale*zoom+dX,
         drawRect.size.height*zoom-(lineVec[index].data[t].y+.5)*scale*zoom+dY);
         
         [path5 appendBezierPathWithOvalInRect: NSMakeRect(startPoint.x-2.5*zoom,startPoint.y-2.5*zoom,5*zoom,5*zoom)];
         
         }
         
         }
         */
        
    }
    [[NSColor cyanColor] set];
    [path11 fill];
    //[path fill];
    [[NSColor blueColor] set];
    [path stroke];
    [path1 stroke];
    [[NSColor magentaColor] set];
    [path3 stroke];
    [[NSColor purpleColor] set];
    [path6 stroke];
    [[NSColor orangeColor] set];
    [path2 stroke];
    [[NSColor redColor] set];
    [path5 fill];
    [path10 fill];
    [path9 stroke];
    [[NSColor blackColor] set];
    [path7 stroke];
    [path8 fill];
    [[NSColor cyanColor] set];
    [path4 stroke];
    
    
    for (int index=0; index<count;index++) {
        centerPoint=NSMakePoint((lineVec[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineVec[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineVec[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineVec[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineVec[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineVec[index].end.y+.5)*scale*zoom+dY);
        
        //centerPoint=NSMakePoint(lineVec[index].data[lineVec[index].data.size()/2].x*scale*zoom+dX,
        //    drawRect.size.height*zoom-lineVec[index].data[lineVec[index].data.size()/2].y*scale*zoom+dY);
        
        if(lineVec[index].start.type==C_POINT && lineVec[index].end.type==C_POINT){
            [[NSColor redColor] set];
            int pR_=2*pR;
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(startPoint.x-pR_*zoom,startPoint.y-pR_*zoom,pR_*2*zoom,pR_*2*zoom)];
            [path fill];
            
        }else{
            
            [[NSColor greenColor] set];
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(startPoint.x-pR*zoom,startPoint.y-pR*zoom,pD*zoom,pD*zoom)];
            [path fill];
            
        }
        
        
        if(lineVec[index].start.type==C_POINT && lineVec[index].end.type==C_POINT){
            [[NSColor redColor] set];
            int pR_=2*pR;
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(endPoint.x-pR_*zoom,endPoint.y-pR_*zoom,pR_*2*zoom,pR_*2*zoom)];
            [path fill];
            
        }else{
            
            [[NSColor magentaColor] set];
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(endPoint.x-pR*zoom,endPoint.y-pR*zoom,pD*zoom,pD*zoom)];
            [path fill];
            
        }
        
        
    }
    //выбираем линию вручную
    //selectLine.push_back(2174);
    
    
    //draw selected lines
    [NSBezierPath setDefaultLineWidth:3*zoom];
    path=[NSBezierPath bezierPath];
    int index;
    for (int i=0; i<selectLine.size();i++){
        index=selectLine[i];
        centerPoint=NSMakePoint((lineVec[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineVec[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineVec[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineVec[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineVec[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineVec[index].end.y+.5)*scale*zoom+dY);
        
        if(lineVec[index].corC<60){
            [path moveToPoint:startPoint];
            [path lineToPoint:endPoint];
        }else{
            //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
            //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
            int w=lineVec[index].radius*scale*zoom;
            int r=lineVec[index].radius*scale*zoom/2;
            //int w=2;
            [path2 appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                             centerPoint.y-r,w,w)];
            //cout<<"yc="<<lineVec[index].start.x<<endl;
            
        }
    }
    strColor=[NSColor colorWithCalibratedRed:0.2 green:0.2 blue:0.2 alpha:1];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    
    [[NSColor redColor] set];
    [path stroke];
    //draw selected lines data
    
    for (int i=0; i<selectLine.size();i++){
        index=selectLine[i];
        centerPoint=NSMakePoint((lineVec[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineVec[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineVec[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineVec[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineVec[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineVec[index].end.y+.5)*scale*zoom+dY);
        [[NSColor whiteColor] set];
        NSPoint p;
        if(selectLine.size()>1){
            p=endPoint;
        }else{
            p=centerPoint;
        }
        //cout<<"zoom="<<zoom<<endl;
        if(zoom<2.3){
            [NSBezierPath fillRect:NSMakeRect(p.x, p.y, 70*zoom,  120*zoom )];
        }else{
            [NSBezierPath fillRect:NSMakeRect(p.x, p.y, 50*zoom,  100*zoom )];
        }
        
        [[NSColor grayColor] set];
        [NSBezierPath setDefaultLineWidth:0.3*zoom];
        
        if(zoom<2.3){
            [NSBezierPath strokeRect:NSMakeRect(p.x, p.y, 70*zoom,  120*zoom )];
        }else{
            [NSBezierPath strokeRect:NSMakeRect(p.x, p.y, 50*zoom,  100*zoom )];
        }
        ostringstream out;
        OCRFocalLine &l=lineVec[index];
        
        //Вывод параметров линии
        // таблица в гр порту:
        // 1. len
        // 1. alpha°
        // 1. lenP1
        // 1. alphaP1°
        // 1. lenP2
        // 1. alphaP2°
        // 1. lenA
        // 1. lenM
        // 1. lenBf
        // 1. lenBff
        // 1. id number
        
        out<<"len="<<l.len<<"\nalpha°="<<l.alpha<<"\nlenP1="<<l.lenP1<<"\nalphaP1°="<<l.alphaP1<<"\nlenP2="<<l.lenP2<<"\nalphaP2°="<<l.alphaP2<<"\nlenA="<<l.lenA<<"\nlenM="<<l.lenM<<"\nlenB="<<l.lenB<<"\nlenBf="<<l.lenBf<<"\nlenBff="<<l.lenBff<<"\nid="<<l.index;
        /**/
        [[NSString stringWithUTF8String:out.str().c_str()]
         drawAtPoint:NSMakePoint(p.x+2*zoom,p.y+6*zoom) withAttributes: attrs];
        //if(zoom>1.5)[[NSString stringWithFormat:@"%d", lineVec[index].angl_t]
        //             drawAtPoint:centerPoint withAttributes: attrs];
    }
    
    
    
}




-(void)drawLetterBase{
    drawRect=[[self superview]frame];   //size of Window
    [self setFrame:drawRect];
    
    [NSBezierPath setDefaultLineWidth:2*zoom];
    [[NSColor greenColor] set];
    [NSBezierPath strokeRect:
     NSMakeRect(0,0,100,100)];
    
    NSColor *strColor=[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1];
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:13] forKey: NSFontAttributeName];
    [[NSString stringWithUTF8String:destString.c_str()]
     drawAtPoint:NSMakePoint(100,0) withAttributes: attrs];
    [self drawLetterBaseImage];
}

-(void)drawLetter{
    drawRect=[[self superview]frame];   //size of Window
    [self setFrame:drawRect];
    [self drawLetterBaseImage];
    
    [self drawVector];
    
    /*
    //cout<<" points size="<<letter->focalLine.size();

    hL=(short)pechaSize.height;
    NSPoint startPoint,endPoint,centerPoint,cP1,cP2,p;

    [NSBezierPath setDefaultLineWidth:4*zoom];
    NSBezierPath *pathL=[NSBezierPath bezierPath];
    NSBezierPath *pathA=[NSBezierPath bezierPath];
    NSBezierPath *pathS=[NSBezierPath bezierPath];
    NSBezierPath *pathC=[NSBezierPath bezierPath];
    NSBezierPath *pathStart=[NSBezierPath bezierPath];
    NSBezierPath *pathEnd=[NSBezierPath bezierPath];
    NSBezierPath *pathFirst=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:2];
    NSBezierPath *pathLine=[NSBezierPath bezierPath];
    NSBezierPath *pathCenter=[NSBezierPath bezierPath];
    
    //cout<<"letter->focalLine.size()="<<letter->focalLine.size()<<endl;
    
    int c=(int)letter->focalLine.size();
    for(int i=0;i<c;i++){
        OCRFocalLine line=letter->focalLine[i];
        //cout<<"x="<<line.start.x<<" y="<<line.start.y<<endl;
        
        startPoint=NSMakePoint((line.start.x+.5+PICT_SIZE/2)*scale*zoom+dX,
                               drawRect.size.height*zoom-(line.start.y+.5+PICT_SIZE/2)*scale*zoom+dY);
        endPoint=NSMakePoint((line.end.x+.5+PICT_SIZE/2)*scale*zoom+dX,
                             drawRect.size.height*zoom-(line.end.y+.5+PICT_SIZE/2)*scale*zoom+dY);
        centerPoint=NSMakePoint((line.centerM.x/10+.5+PICT_SIZE/2)*scale*zoom+dX,
                             drawRect.size.height*zoom-(line.centerM.y/10+.5+PICT_SIZE/2)*scale*zoom+dY);
        cP1=NSMakePoint((line.p1.x+.5+PICT_SIZE/2)*scale*zoom+dX,
                           drawRect.size.height*zoom-(line.p1.y+.5+PICT_SIZE/2)*scale*zoom+dY);
        cP2=NSMakePoint((line.p2.x+.5+PICT_SIZE/2)*scale*zoom+dX,
                        drawRect.size.height*zoom-(line.p2.y+.5+PICT_SIZE/2)*scale*zoom+dY);
            
        if(i>0){
            if(line.maxCor==LINE){
                [pathL moveToPoint:startPoint];
                [pathL lineToPoint:endPoint];
            }else if(line.maxCor==ARC){
                [pathA moveToPoint:startPoint];
                [pathA curveToPoint:endPoint controlPoint1:cP1 controlPoint2:cP2];
                
            }else if(line.maxCor==SPIRAL){
                [pathS moveToPoint:startPoint];
                [pathS curveToPoint:endPoint controlPoint1:cP1 controlPoint2:cP2];
                
            }else if(line.maxCor==CIRCLE){
                //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
                //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
                int w=letter->focalLine[i].radius*scale*zoom;
                int r=letter->focalLine[i].radius*scale*zoom/2;
                //int w=2;
                [pathC appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                                 centerPoint.y-r,w,w)];
                //cout<<"yc="<<lineVec[index].start.x<<endl;
            }
        }else{
            if(line.maxCor==LINE){
                [pathFirst moveToPoint:startPoint];
                [pathFirst lineToPoint:endPoint];
            }else if(line.maxCor==ARC){
                [pathFirst moveToPoint:startPoint];
                [pathFirst curveToPoint:endPoint controlPoint1:cP1 controlPoint2:cP2];
                
            }else if(line.maxCor==SPIRAL){
                [pathFirst moveToPoint:startPoint];
                [pathFirst curveToPoint:endPoint controlPoint1:cP1 controlPoint2:cP2];
                
            }else if(line.maxCor==CIRCLE){
                //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
                //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
                int w=letter->focalLine[i].radius*scale*zoom;
                int r=letter->focalLine[i].radius*scale*zoom/2;
                //int w=2;
                [pathFirst appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                                 centerPoint.y-r,w,w)];
                //cout<<"yc="<<lineVec[index].start.x<<endl;
            }

        
        }
        int l=line.size();
        //cout<<"l="<<l<<"len="<<line.len<<endl;
        p=NSMakePoint(((float)line.mData[0].x+.5+PICT_SIZE/2)*scale*zoom+dX,
                      drawRect.size.height*zoom-((float)line.mData[0].y+.5+PICT_SIZE/2)*scale*zoom+dY);
        [pathLine moveToPoint:p];
        
        for(int n=0;n<l;n++){
            p=NSMakePoint(((float)line.mData[n].x+.5+PICT_SIZE/2)*scale*zoom+dX,
                          drawRect.size.height*zoom-((float)line.mData[n].y+.5+PICT_SIZE/2)*scale*zoom+dY);
            [pathLine lineToPoint:p];
        
        }

        //[NSBezierPath fillRect:
        // NSMakeRect((PICT_SIZE*3-letter->fPoint[i].x-3)*scale*zoom+dX,
        //            drawRect.size.height*zoom-(PICT_SIZE*3-letter->fPoint[i].y-3)*scale*zoom+dY,
        //            6,6)];
        [pathStart appendBezierPathWithOvalInRect:NSMakeRect(startPoint.x-6,
                    startPoint.y-6,
                    12,12)];
        [pathEnd appendBezierPathWithOvalInRect:NSMakeRect(endPoint.x-6,
                                                         endPoint.y-6,
                                                         12,12)];
        [pathCenter appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-3,
                                                           centerPoint.y-3,
                                                           6,6)];
       
        //T_POINT=1,
        //X_POINT=2,
        //L_POINT=4,
        //P_POINT=8,
        //C_POINT=9
        
    }
    [[NSColor cyanColor] set];
    [pathLine stroke];
    [[NSColor blueColor] set];
    [pathL stroke];
    [[NSColor magentaColor] set];
    [pathA stroke];
    [[NSColor purpleColor] set];
    [pathS stroke];
    [[NSColor orangeColor] set];
    [pathC stroke];
    [[NSColor redColor] set];
    [pathFirst stroke];
    [pathCenter fill];
    
    [[NSColor greenColor] set];
    [pathStart fill];
    [[NSColor magentaColor] set];
    [pathEnd fill];

    //DRAW POINTS
    c=(int)letter->focalPoint->size();
    [NSBezierPath setDefaultLineWidth:4];
    
    for(int i=0;i<c;i++){
        OCRPoint p=letter->fPoint[i];
        //cout<<"line.start.type="<<(short)p.type<<" x="<<p.x<<" y="<<p.y<<endl;
        NSPoint pt=NSMakePoint((p.x+.5+PICT_SIZE/2)*scale*zoom+dX,
                      drawRect.size.height*zoom-(p.y+.5+PICT_SIZE/2)*scale*zoom+dY);
        if(p.type==T_POINT){
            NSBezierPath* path=[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(pt.x-7,pt.y-7,14,14)];
            [[NSColor yellowColor] set];
            [path stroke];
        }
        if(p.type==X_POINT){
            NSBezierPath* path=[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(pt.x-7,pt.y-7,14,14)];
            [[NSColor cyanColor] set];
            [path stroke];
        }
        if(p.type==L_POINT){
            NSBezierPath* path=[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(pt.x-7,pt.y-7,14,14)];
            [[NSColor blueColor] set];
            [path stroke];
        }
        if(p.type==P_POINT){
            NSBezierPath* path=[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(pt.x-7,pt.y-7,14,14)];
            [[NSColor redColor] set];
            [path stroke];
        }
        if(p.type==C_POINT){
            NSBezierPath* path=[NSBezierPath bezierPathWithOvalInRect:NSMakeRect(pt.x-7,pt.y-7,14,14)];
            [[NSColor orangeColor] set];
            [path stroke];
        }
    
    }
    
    [NSBezierPath setDefaultLineWidth:2*zoom];
    [[NSColor greenColor] set];
    [NSBezierPath strokeRect:
     NSMakeRect(0,0,100,100)];
    */
    /*
    NSColor *strColor=[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1];
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:13] forKey: NSFontAttributeName];
    [[NSString stringWithUTF8String:destString.c_str()]
     drawAtPoint:NSMakePoint(100,0) withAttributes: attrs];
    */
}



-(void)drawPechaImage{
    
    //[zoomSlider floatValue];

        //drawRect.size.height=drawRect.size.height-197;  //size of buttom space for button
    //drawRect.origin.y=197; //sfift y
    
    
    hL=(short)pechaSize.height;
    wL=(short)pechaSize.width;
    
    //cout<<"drawRect.size.width="<<drawRect.size.width<<endl;
    
    if(pechaImage!=nil){
        //cout <<"hL="<<hL<<" wL="<<wL<<" zoom="<<zoom<<endl;
        pechaBounds=NSMakeRect(0,0,wL, hL);
        
        scale=drawRect.size.height/hL;   //cout<<"scale="<<scale<<endl;
        pechaZoomBounds=NSMakeRect(dX,dY,wL*scale*zoom, hL*scale*zoom);

        //[self setFrame:pechaBounds];
        [pechaImage drawInRect:pechaZoomBounds fromRect:pechaBounds operation:NSCompositeSourceOver fraction:1.0];
        
    }
    
}//////////////////////////////////////////////////////////////

-(void)drawLetterBaseImage{
    
    //[zoomSlider floatValue];
    //drawRect.size.height=drawRect.size.height-197;  //size of buttom space for button
    //drawRect.origin.y=197; //sfift y
    hL=(short)pechaSize.height;
    wL=(short)pechaSize.width;
    
    //cout<<"drawRect.size.width="<<drawRect.size.width<<endl;
    
    if(pechaImage!=nil){
        //cout <<"hL="<<hL<<" wL="<<wL<<" zoom="<<zoom<<endl;
        pechaBounds=NSMakeRect(0,0,wL, hL);
        
        //scale=drawRect.size.height/hL;   //cout<<"scale="<<scale<<endl;
        pechaZoomBounds=NSMakeRect(0,drawRect.size.height-hL*scale*zoom,wL*scale*zoom, hL*scale*zoom); //
        
        //[self setFrame:pechaBounds];
        [pechaImage drawInRect:pechaZoomBounds fromRect:pechaBounds operation:NSCompositeSourceOver fraction:1.0];
        
    }
    
}//////////////////////////////////////////////////////////////

- (void)mouseDown:(NSEvent *)theEvent{
    //cout<<"mouse down x="<<theEvent.x<<endl;
    
    if(drawMode==DRAW_VECTOR||drawMode==DRAW_LETTER){
        
        NSPoint t=theEvent.locationInWindow;
        OCRPoint p,startPoint,endPoint,centerM;
        p.x=t.x;
        p.y=t.y;
        int d=15;
        selectLine.resize(0);
        if(lineVec.size()){
            for (int i=0; i<lineVec.size();i++) {
                centerM.x=(lineVec[i].centerM.x+.5)*scale*zoom+dX;
                centerM.y=drawRect.size.height*zoom-(lineVec[i].centerM.y+.5)*scale*zoom+dY;
                if(isNearP(p,centerM,d)){
                    selectLine.push_back(i);
                    //cout<<i<<" xS="<<centerM.x<<" yS="<<centerM.y<<endl;
                    break;

                }
            }
            [self display];
            return;
            
        }
        
    }
    if(drawMode==DRAW_LETTERBASE){
       
        NSPoint t=theEvent.locationInWindow;
        //cout<<"mouse down y="<<(int)(drawRect.size.height-t.y-20)/64<<" ";
        //cout<<"mouse down x="<<(int)(t.x-10)/64<<endl;
        
        int index=((int)(drawRect.size.height-t.y)/64)*20+(t.x-10)/64+1;
        //cout<<"index="<<index<<endl;
        [[gWindow alloc]initLetter:index];
    
    
    }
    //if(selectLine.size())cout <<"selectLine="<<selectLine[0]<<endl;
    [self display];
}

-(void)mouseDragged:(NSEvent *)theEvent{
    //cout<<"mouse drag x="<<theEvent.deltaX<<endl;
}

-(void)openLetterByIndex:(NSInteger)index{
    //cout<<"text1="<<index;
    [self setLetter:(int)index];
    [self display];
}
-(void)OCRLetterByIndex:(NSInteger)index{
    cout<<" start OCR"<<endl;
    inputData.data["ocrData"]="batchOCR";
    string pathText;
    ((GVector*)inputData.prefVector)->getStr(20,pathText);
    inputData.fileList.push_back(pathText);
    string result;
    inputData.start=100;
    processInput(result);
    
    
}



@end








//cout<<"lineVec[index].corL="<<lineVec[index].corL<<endl;
//NSRect rect=NSMakeRect(400+dX,400+dY,400,400);

// NSPoint topLeft = NSMakePoint(NSMinX(rect), NSMaxY(rect));
//NSPoint topRight = NSMakePoint(NSMaxX(rect), NSMaxY(rect));
//NSPoint bottomRight = NSMakePoint(NSMaxX(rect), NSMinY(rect));
//float radius=200;
//NSPoint origin=NSMakePoint(NSMidX(rect), NSMaxY(rect));

//[NSBezierPath strokeRect:NSMakeRect(startPoint.x+dX,startPoint.y+dY,10,10)];
//[NSBezierPath strokeRect:NSMakeRect(endPoint.x+dX,endPoint.y+dY,10,10)];
//[[NSColor blueColor] set];
///[NSBezierPath strokeRect:NSMakeRect(cPoint.x+dX,cPoint.y+dY,10,10)];
///[[NSColor greenColor] set];

//[path moveToPoint:startPoint];
//[path appendBezierPathWithArcFromPoint:startPoint     toPoint:endPoint radius:radius];

//[path moveToPoint:NSMakePoint(NSMidX(rect), NSMaxY(rect))];
//[path appendBezierPathWithArcFromPoint:topLeft     toPoint:rect.origin radius:radius];
//[path appendBezierPathWithArcFromPoint:rect.origin toPoint:bottomRight radius:clampedRadius];
//[path appendBezierPathWithArcFromPoint:bottomRight toPoint:topRight    radius:clampedRadius];
//[path appendBezierPathWithArcFromPoint:topRight    toPoint:topLeft     radius:clampedRadius];
//[path closePath];

//[path appendBezierPathWithArcFromPoint:startPoint toPoint:endPoint radius:600 ];
//[path appendBezierPathWithOvalInRect:NSMakeRect(600+dX,600+dY,400,400)];
//[path curveToPoint:endPoint controlPoint1:cPoint controlPoint2:cPoint];
//[path appendBezierPathWithArcWithCenter:startPoint radius:300 startAngle:25 endAngle:120];
//[path_ appendBezierPath:path];
//[NSBezierPath strokeLineFromPoint:startPoint toPoint:endPoint];
//  y_+=10;
//}
//[path closePath];
//NSRect f=[path bounds];
//cout<<" w="<<f.size.width<<" h"<<f.size.height<<" x"<<f.origin.x<<" y"<<f.origin.y<<" "<<endl;
//break;
