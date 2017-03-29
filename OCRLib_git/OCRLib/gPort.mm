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
#include "GBitsetMatrix.h"


using namespace std;

@implementation gPort

- (void)drawRect:(NSRect)dirtyRect {
    
    if(drawMode==DRAW_VECTOR)[self drawVectorPort];
    if(drawMode==DRAW_OCR_LETTER)[self drawVectorPort];
    if(drawMode==DRAW_LETTERBASE)[self drawLetterBase];
    if(drawMode==DRAW_LETTER)[self drawLetter];
    if(drawMode==DRAW_MASK)[self drawLetter];
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
    //((GStr<int>*)inputData.pref)->put(11,dX);
    [self setNeedsDisplay:YES];

}
-(IBAction)moveRight:(id)sender{
    dX-=dl;
    //((GStr<int>*)inputData.pref)->put(11,dX);
    [self setNeedsDisplay:YES];
}
-(IBAction)moveUp:(id)sender{
    dY+=dl;
    //((GStr<int>*)inputData.pref)->put(12,dY);
    [self setNeedsDisplay:YES];
    
}
-(IBAction)moveDown:(id)sender{
    dY-=dl;
    //((GStr<int>*)inputData.pref)->put(12,dY);
    [self setNeedsDisplay:YES];
}

-(IBAction)zoomIn:(id)sender{
    int x=drawRect.size.width/2-dX;
    int w=(x/zoom)*(zoom+0.1)-x;
    int y=drawRect.size.height/2-dY;
    int h=(y/zoom)*(zoom+0.1)-y;
    zoom+=0.1;
    dX-=w;
    dY-=h;
    //((GStr<int>*)inputData.pref)->put(10,zoom*100);
    //((GStr<int>*)inputData.pref)->put(11,dX);
    //((GStr<int>*)inputData.pref)->put(12,dY);
    
    [self setNeedsDisplay:YES];
    
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
        [self setNeedsDisplay:YES];
    }
    //((GStr<int>*)inputData.pref)->put(10,zoom*100);
    //((GStr<int>*)inputData.pref)->put(11,dX);
    //((GStr<int>*)inputData.pref)->put(12,dY);
    
}

-(IBAction)homeImg:(id)sender{
    zoom=1;
    dX=0;
    dY=0;
    [self setNeedsDisplay:YES];
    
}

-(void)setLetterBase:(string)path{
    drawMode=DRAW_LETTERBASE;
    int pos=(int)path.find("?")+1;
    path=substr(pos,(int)path.size()-pos,path);
    destString="";
    inputData.data["QUERY_STRING"]=path;
    parseQuery();
    pageNum=0;//((GStr<int>*)inputData.pref)->get(24);
    if(inputData.data["db"]=="letter"){
        [self openLetterBasePage:pageNum];
    }
   
}

-(void)openLetterBasePage:(int)index{
    searchResult.resize(0);
    
    GImageEditor *imageEditor=(GImageEditor*)inputData.mainEditor;

    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    ulong size=aliKali->size();
    if(index*300<size){
        GBitmap *img=aliKali->drawOCRBaseImage(index*300+1,300);
        pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
        pechaSize=[pechaImage size];
        dX=0;
        dY=-250;
        scale=1;
        zoom=1;
        [self setNeedsDisplay:YES];
    }
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
        scale=4;
        zoom=1;
        for(int i=0;i<letter->focalLine.size();i++){
            OCRFocalLine &line=letter->focalLine[i];
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
            //cout<<" line.centerM.x="<<line.centerM.x<<endl;
            for(int j=0;j<line.data.size();j++){
                line.data[j].x+=PICT_SIZE/2;
                line.data[j].y+=PICT_SIZE/2;
            }
            for(int j=0;j<line.mData.size();j++){
                line.mData[j].x+=PICT_SIZE/2;
                line.mData[j].y+=PICT_SIZE/2;
            }
        }
        for(int i=0;i<letter->mask32Count();i++){
            letter->mask32[i].xMask+=PICT_SIZE/2;
            letter->mask32[i].yMask+=PICT_SIZE/2;
        }
        drawMode=0;//((GStr<int>*)inputData.pref)->get(25);
        if(drawMode!=DRAW_MASK&&drawMode!=DRAW_LETTER)drawMode=DRAW_LETTER;
        selectMask=-1;

    }else{
        destString="no letters";
    }
    pZoom=0.5; //пиксельное увеличение
    [self setNeedsDisplay:YES];
}

-(void)setOCRLetter{
    
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    int index=0;//((GStr<int>*)inputData.pref)->get(23);
    letter=aliKali->getLetter(index);
    pechaSize=[pechaImage size];
    //aliKali->drawHTML(0,100,result,mode);
    //lineVec.resize(0);
    index=selectLine[0];
    int x=lineVec[index].centerM.x-letter->focalLine[0].centerM.x;
    int y=lineVec[index].centerM.y-letter->focalLine[0].centerM.y;
    
    cout<<"selectLine="<<selectLine.size()<<" index="<<index<<" x="<<x<<endl;
    
    lineOCRVec.resize(0);
    
    for(int i=0;i<letter->focalLine.size();i++){
        
        OCRFocalLine line;
        line=letter->focalLine[i];
        line.start.x+=x;
        line.start.y+=y;
        line.end.x+=x;
        line.end.y+=y;
        line.centerM.x+=x*10;
        line.centerM.y+=y*10;
        line.center.x+=x;
        line.center.y+=y;
        line.p1.x+=x;
        line.p1.y+=y;
        line.p2.x+=x;
        line.p2.y+=y;
        
        for(int j=0;j<line.data.size();j++){
            line.data[j].x+=x;
            line.data[j].y+=y;
        }
        //cout<<" set OCRLetter"<<endl;
        for(int j=0;j<line.mData.size();j++){
            //cout<<"x="<<line.mData[j].x<<"y="<<line.mData[j].y<<endl;
            line.mData[j].x+=x*16;
            line.mData[j].y+=y*16;
        }
        lineOCRVec.push_back(line);
        
    }
    drawOCRLetter=1;
    selectOCR=-1;
    drawMode=DRAW_OCR_LETTER;
    [self drawLines:lineOCRVec :1];
}

-(void)setPechaImage{
    //NSString *filePath=[NSString stringWithUTF8String:"/BUDDISM.RU/_CHO_SPYOD/RIME_GYUNKER/BAI_RO_RGYUD_BUM/BAI_RO_RGYUD_BUM_W21519/W21519-0577-eBook_tif/page_0228.tif"];
    //pechaImage=[[NSImage alloc] initWithContentsOfFile:filePath]; //open Image;
    GImageEditor *imageEditor=(GImageEditor*)inputData.mainEditor;
    //string inputPath="/BUDDISM.RU/_CHO_SPYOD/RIME_GYUNKER/BAI_RO_RGYUD_BUM/BAI_RO_RGYUD_BUM_W21519/W21519-0577-eBook_tif/page_0228.tif";
    //string inputPath="/_TEST__MEDIA/W21519-0577-eBook_tif/page_0066.tif";
    string pathText;
    //((GVector*)inputData.prefVector)->getStr(20,pathText);
    GBitmap* pechaImg=imageEditor->LoadImageData(pathText,0);
    if(pechaImg->DetectGradations())pechaImg->binarisation(85,24);
    GGraph pechaGraph=GGraph(pechaImg);
    pechaGraph.focalLine(15);
    pechaGraph.focalPointXTL(strPoint);
    pechaGraph.focalLineRecognition(strPoint,lineVec);
    //VStr2D<int>pointMatrix;
    //VStr2D<int>lineMatrix;
    //pechaGraph.setLineMatrix(strPoint, lineVec,pointMatrix,lineMatrix);
    
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)&pechaGraph);
    pechaSize=[pechaImage size];

    dl=100;
    zoom=1;//((GStr<int>*)inputData.pref)->get(10);
    zoom/=100;
    if(zoom<0.2)zoom=1;
    pZoom=pechaSize.width/1024; //пиксельное увеличение
    
    dX=0;//((GStr<int>*)inputData.pref)->get(11);
    dY=0;//((GStr<int>*)inputData.pref)->get(12);
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
    if(drawMode==DRAW_VECTOR){
        [self drawLines:lineVec :0];
        [self drawPoints:lineVec];
        [self drawOCRLetter];
        [self drawSelected:lineVec];
    }
    if(drawMode==DRAW_OCR_LETTER){
        [self drawLines:lineVec :0];
        [self drawPoints:lineVec];
        [self drawLines:lineOCRVec :1];
        [self drawPoints:lineOCRVec];
        [self drawOCRLetter];
        [self drawSelected:lineOCRVec];
        
    }
    if(drawMode==DRAW_LETTER){
        [self drawLines:letter->focalLine :0];
        [self drawPoints:letter->focalLine];
        [self drawSelected:letter->focalLine];
    }
    //selectLine.push_back(3750);
    
    //draw selected lines
 }

-(void)setDrawMode:(int)mode{
    
    if(mode==DRAW_NEW_MASK){
        drawMode=DRAW_MASK;
        drawMaskMode=DRAW_NEW_MASK;
    }else{
     drawMode=mode;
     drawMaskMode=0;
    }
    
    [self setNeedsDisplay:YES];

}

-(IBAction)setLetterBox:(id)sender{
    drawMode=DRAW_MASK;
    drawMaskMode=DRAW_LETTER_BOX;
    letter->letterH=0;
    letter->letterW=0;
    letter->dX=0;
    letter->dY=0;
    [self setNeedsDisplay:YES];
}

-(IBAction)setLetterLineBox:(id)sender{
    drawMode=DRAW_MASK;
    drawMaskMode=DRAW_LETTER_LINE;
    letter->y0=0;
    letter->y1=2;
    [self setNeedsDisplay:YES];
}

- (IBAction)setDrawMask:(id)sender{
    drawMode=DRAW_MASK;
    drawMaskMode=0;
    newMask.resize(0);
    //((GStr<int>*)inputData.pref)->put(25,DRAW_MASK);
    [self setNeedsDisplay:YES];
}

-(IBAction)setDrawNewMask:(id)sender{
    drawMode=DRAW_MASK;
    drawMaskMode=DRAW_NEW_MASK;
    NSCursor *aCursor=[NSCursor crosshairCursor];
    [self addCursorRect:NSMakeRect(0,0,750,950) cursor:aCursor];
    [aCursor setOnMouseEntered:YES];
    //((GStr<int>*)inputData.pref)->put(25,DRAW_NEW_MASK);
}


-(void)drawLetterBase{
    drawRect=[[self superview]frame];   //size of Window
    [self setFrame:drawRect];
    NSTextField *textField;
    
    NSButton *button=[[self superview] subviews][1];
    [button setFrame:NSMakeRect(220,drawRect.size.height-34,64,34)];
    textField=[[self superview] subviews][2];
    [textField setFrame:NSMakeRect(284,drawRect.size.height-34,92,34)];
    button=[[self superview] subviews][3];
    [button setFrame:NSMakeRect(376,drawRect.size.height-34,64,34)];
    button=[[self superview] subviews][4];
    [button setFrame:NSMakeRect(440,drawRect.size.height-34,64,34)];
    textField=[[self superview] subviews][5];
    [textField setFrame:NSMakeRect(0,drawRect.size.height-192,220,192)];
    button=[[self superview] subviews][6];
    [button setFrame:NSMakeRect(0,drawRect.size.height-226,220,34)];
    button=[[self superview] subviews][7];
    [button setFrame:NSMakeRect(0,drawRect.size.height-260,220,34)];

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

-(void)drawLetterSearch:(vector<ulong>&)searchResult_{
    
    GImageEditor *imageEditor=(GImageEditor*)inputData.mainEditor;
    searchResult=searchResult_;
    
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    GBitmap *img=aliKali->drawOCRBasePict(searchResult, DRAW_BASE);
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
    pechaSize=[pechaImage size];
    dX=0;
    dY=-250;
    scale=1;
    zoom=1;
    [self setNeedsDisplay:YES];
    
}

-(void)drawLetter{
    drawRect=[[self superview]frame];   //size of Window
    [self setFrame:drawRect];
    [self drawLetterImage];
    //cout<<" points size="<<letter->focalLine.size();
    if(drawMode==DRAW_LETTER){
       [self drawVector];
    }
    if(drawMode==DRAW_MASK){
        if(drawMaskMode!=ERASE_IN_IMAGE){
            [self drawMask];
        }
        [self drawNewMask];
    }
}



-(void)drawPechaImage{
    
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
    
    hL=(short)pechaSize.height;
    wL=(short)pechaSize.width;
    
    //cout<<"drawRect.size.width="<<drawRect.size.width<<endl;
    
    if(pechaImage!=nil){
        //cout <<"hL="<<hL<<" wL="<<wL<<" zoom="<<zoom<<endl;
        pechaBounds=NSMakeRect(0,0,wL, hL);
        
        //scale=drawRect.size.height/hL;   //cout<<"scale="<<scale<<endl;
        pechaZoomBounds=NSMakeRect(220,drawRect.size.height-hL*scale*zoom-34,wL*scale*zoom, hL*scale*zoom); //
        
        //[self setFrame:pechaBounds];
        [pechaImage drawInRect:pechaZoomBounds fromRect:pechaBounds operation:NSCompositeSourceOver fraction:1.0];
        
    }
    
}//////////////////////////////////////////////////////////////

-(void)drawLetterImage{
    
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
    NSPoint t=theEvent.locationInWindow;
    OCRPoint p,startPoint,endPoint,centerM;
    p.x=t.x;
    p.y=t.y;

    
    if((drawMode==DRAW_VECTOR)&&drawMaskMode==0){
        int d=15;
        selectLine.resize(0);
        if(lineVec.size()){
            for (int i=0; i<lineVec.size();i++) {
                centerM.x=(lineVec[i].centerM.x+.5)*scale*zoom+dX;
                centerM.y=drawRect.size.height*zoom-(lineVec[i].centerM.y+.5)*scale*zoom+dY;
                if(isNearP(p,centerM,d)){
                    selectLine.push_back(i);
                    cout<<i<<" xS="<<centerM.x<<" yS="<<centerM.y<<endl;
                    break;

                }
            }
            [self setNeedsDisplay:YES];
            return;
            
        }
    }
    
    if((drawMode==DRAW_LETTER)&&drawMaskMode==0){
        int d=15;
        selectLine.resize(0);
        if(letter->focalLine.size()){
            for (int i=0; i<letter->focalLine.size();i++) {
                centerM.x=(letter->focalLine[i].centerM.x+.5)*scale*zoom+dX;
                centerM.y=drawRect.size.height*zoom-(letter->focalLine[i].centerM.y+.5)*scale*zoom+dY;
                if(isNearP(p,centerM,d)){
                    selectLine.push_back(i);
                    cout<<i<<" xS="<<centerM.x<<" yS="<<centerM.y<<endl;
                    break;
                    
                }
            }
            [self setNeedsDisplay:YES];
            return;
            
        }
    }
    
    if(drawMaskMode==DRAW_NEW_MASK){
        //newMask.resize(0);
        NSRect box=NSMakeRect(t.x, t.y, 2, 2);
        newMask.push_back(box);
        letter->letterH=0;
        letter->letterW=0;
        letter->dX=0;
        letter->dY=0;
        [self setNeedsDisplay:YES];
        return;
    }
    
    if(drawMode==DRAW_MASK&&
       drawMaskMode!=ERASE_IN_IMAGE&&
       drawMaskMode!=DRAW_NEW_MASK&&
       drawMaskMode!=DRAW_LETTER_BOX&&
       drawMaskMode!=DRAW_LETTER_LINE){
        //newMask.resize(0);
        int x0=t.x/scale;
        selectMask=-1;
        int y0=(drawRect.size.height-t.y)/scale;
        for (int i=0; i<letter->mask32Count();i++) {
            if(x0>letter->mask32[i].xMask&&
               x0<letter->mask32[i].xMask+letter->mask32[i].imgW&&
               y0>letter->mask32[i].yMask&&
               y0<letter->mask32[i].yMask+letter->mask32[i].mH){
                selectMask=i;
                break;
            }
        }
        mouseX=letter->mask32[selectMask].xMask-x0;
        mouseY=letter->mask32[selectMask].yMask-y0;  //запоминаем на каком расстоянии от угла маски находится курсор
        
        [self setNeedsDisplay:YES];
        //cout<<" x0="<<x0<<" y0="<<y0<<" selectMask="<<selectMask<<endl;
        return;
    }
    
    
    if(drawMaskMode==ERASE_IN_IMAGE){
        newMask.resize(0);
        NSRect box=NSMakeRect(t.x, t.y, 2, 2);
        newMask.push_back(box);
        [self setNeedsDisplay:YES];
        return;
    }
    
    if(drawMaskMode==DRAW_LETTER_BOX){
        letter->dX=t.x/scale-PICT_SIZE/2;
        letter->dY=(drawRect.size.height-t.y)/scale-PICT_SIZE/2;
        letter->letterH=2;
        letter->letterW=2;
        mouseX=t.x;
        mouseY=t.y;
        [self setNeedsDisplay:YES];
        return;
    }
    
    if(drawMaskMode==DRAW_LETTER_LINE){
        letter->y0=(drawRect.size.height-t.y)/scale-PICT_SIZE/2;
        mouseX=t.x;
        mouseY=t.y;
        [self setNeedsDisplay:YES];
        return;
    }
    
    if(drawMode==DRAW_OCR_LETTER){
        int d=15;
        selectLine.resize(0);
        if(lineVec.size()){
            for (int i=0; i<lineOCRVec.size();i++) {
                centerM.x=(lineOCRVec[i].centerM.x+.5)*scale*zoom+dX;
                centerM.y=drawRect.size.height*zoom-(lineOCRVec[i].centerM.y+.5)*scale*zoom+dY;
                if(isNearP(p,centerM,d)){
                    selectLine.push_back(i);
                    //cout<<i<<" xS="<<centerM.x<<" yS="<<centerM.y<<endl;
                    break;
                    
                }
            }
            [self setNeedsDisplay:YES];
            return;
            
        }
        
    }
     
    
    if(drawMode==DRAW_LETTERBASE){
       
        NSPoint t=theEvent.locationInWindow;
        //cout<<"mouse down y="<<(int)(drawRect.size.height-t.y-20)/64<<" ";
        //cout<<"mouse down x="<<(int)(t.x-10)/64<<endl;
        int index=((int)(drawRect.size.height-t.y-34)/64)*20+(t.x-230)/64+1;
        cout<<"index="<<index<<" pageNum="<<pageNum<<endl;
        if(!searchResult.size()){
            pageNum=0;//((GStr<int>*)inputData.pref)->get(24);
            index+=pageNum*300;
        }else{
            index--;
            if(index<searchResult.size()){
            index=searchResult[index];
            }else return;
        }
        
        [[gWindow alloc]initLetter:index];
    }
    //if(selectLine.size())cout <<"selectLine="<<selectLine[0]<<endl;
    
}

-(void)mouseDragged:(NSEvent *)theEvent{
    //cout<<"mouse drag x="<<theEvent.deltaX<<endl;
    NSPoint t=theEvent.locationInWindow;
    
    if(drawMaskMode==DRAW_NEW_MASK){
        if(newMask.size()){
            uint index=(uint)newMask.size()-1;
            NSRect &box=newMask[index];
            box.size.width=t.x-box.origin.x;
            if(box.size.width<-32*4)box.size.width=-32*4;
            if(box.size.width>32*4)box.size.width=32*4;
            box.size.height=t.y-box.origin.y;
            [self setNeedsDisplay:YES];
        }
        return;
    }
    if(drawMaskMode==ERASE_IN_IMAGE){
        if(newMask.size()){
            uint index=(uint)newMask.size()-1;
            NSRect &box=newMask[index];
            box.size.width=t.x-box.origin.x;
            box.size.height=t.y-box.origin.y;
            [self setNeedsDisplay:YES];
        }
        return;
    }
    if(drawMode==DRAW_MASK&&
       drawMaskMode!=DRAW_NEW_MASK&&
       drawMaskMode!=DRAW_LETTER_BOX&&
       drawMaskMode!=DRAW_LETTER_LINE){
        //theEvent.deltaX, deltaY дают неустойчивые показания и зависят от направления движения, поэтому используем координаты;
        int x=mouseX+t.x/scale;//-PICT_SIZE/2;
        int y=mouseY+(drawRect.size.height-t.y)/scale;//-PICT_SIZE/2;
        letter->mask32[selectMask].xMask=x;
        letter->mask32[selectMask].yMask=y;
        [self setNeedsDisplay:YES];
    }
    if(drawMode==DRAW_MASK&&
       drawMaskMode==DRAW_LETTER_BOX){
        //theEvent.deltaX, deltaY дают неустойчивые показания и зависят от направления движения, поэтому используем координаты;
        //letter->dX=t.x/scale-PICT_SIZE/2;
        //letter->dY=(drawRect.size.height-t.y)/scale-PICT_SIZE/2;
        letter->letterW=(t.x-mouseX)/scale;
        letter->letterH=(t.y-mouseY)/scale;
        if(letter->letterH<0)letter->letterH*=-1;
        if(letter->letterW<0)letter->letterW*=-1;
        letter->dX=mouseX/scale-PICT_SIZE/2+letter->letterW/2;
        letter->dY=(drawRect.size.height-mouseY)/scale-PICT_SIZE/2-letter->letterH/2;

        [self setNeedsDisplay:YES];
    }
    if(drawMode==DRAW_MASK&&
       drawMaskMode==DRAW_LETTER_LINE){
        letter->y1=(drawRect.size.height-t.y)/scale-PICT_SIZE/2;
        [self setNeedsDisplay:YES];
    }
}

-(void)mouseUp:(NSEvent *)theEvent{
    //cout<<" mouseUp"<<endl;
    NSPoint t=theEvent.locationInWindow;
    if(drawMaskMode==ERASE_IN_IMAGE){
        if(newMask.size()){
            uint index=(uint)newMask.size()-1;
            NSRect &box=newMask[index];
            box.size.width=t.x-box.origin.x;
            box.size.height=t.y-box.origin.y;
            [pechaImage lockFocus];
            
                [NSBezierPath setDefaultLineWidth:1*zoom];
                NSBezierPath* path=[NSBezierPath bezierPath];
                [path appendBezierPathWithRect:NSMakeRect(newMask[0].origin.x/scale,
                newMask[0].origin.y/scale-(246.75-PICT_SIZE),newMask[0].size.width/scale,newMask[0].size.height/scale)];
                [[NSColor whiteColor] set];
                [path fill];

            
            [pechaImage unlockFocus];
            newMask.resize(0);
            //[self drawLetterImage];
            [self setNeedsDisplay:YES];
        }
        return;
    }
    
    
}


-(IBAction)removeMask:(id)sender{
    if(newMask.size()){
       newMask.pop_back();
    }else if(letter->mask32Count()){
        letter->mask32Vector->resize(letter->mask32Vector->size()-1);
    }
    [self setNeedsDisplay:YES];

}

-(void)openLetterByIndex:(NSInteger)index{
    //cout<<"text1="<<index;
    [self setLetter:(int)index];
    [self setNeedsDisplay:YES];
}
-(IBAction)OCRLetterByIndex:(id)sender{
    cout<<" start OCR"<<endl;
    inputData.data["ocrData"]="batchOCR";
    string pathText;
    //((GVector*)inputData.prefVector)->getStr(20,pathText);
    NSTextField *textField=[[self superview] subviews][8];
    int index=(uint)[textField integerValue];
    
    //((GStr<int>*)inputData.pref)->put(23,(uint)index);
    inputData.fileList.push_back(pathText);
    string result;
    inputData.start=101;
    inputData.startIndex=(uint)index;
    processInput(result);
    cout<<"inputData.lineIndex="<<inputData.lineIndex.size()<<" index="<<index<<endl;
    [self setNeedsDisplay:YES];
    
}

-(IBAction)saveLetter:(id)sender{

    cout<<"save Letter "<<letter->letterIndex<<endl;
    GImageEditor *imageEditor=(GImageEditor*)inputData.imageEditor;
    GBitmap *img=imageEditor->LoadImageData(pechaImage);
    //imageEditor->WriteImageData(img,"/_Image2OCR/_2Draw_.jpg",0);
    
    //cout<<"letter->mask128.xMask="<<letter->mask128.xMask<<endl<<" mW="<<letter->mask128.mW;
    
    GBitmap *letterImg=GBitmap::createRegion(img, PICT_SIZE/2+letter->mask128.xMask,  PICT_SIZE/2+letter->mask128.yMask, letter->mask128.mW, letter->mask128.mH);
    letterImg->binarisation(85, 24);
    letter->mask128.packBitMask128(letterImg);
    for(int i=0;i<letter->mask32Count();i++){
        letter->mask32[i].xMask-=PICT_SIZE/2;
        letter->mask32[i].yMask-=PICT_SIZE/2;
    }
    NSTextField *letterName=[[self superview] subviews][9];
    //NSLog([letterName stringValue]);
    string name=[[letterName stringValue] UTF8String];
    letter->name=name;
    
    for(int i=0;i<newMask.size();i++){
        GBitMask32 mask;
        int x0=newMask[i].origin.x/scale-PICT_SIZE/2;
        int y0=(drawRect.size.height-newMask[i].origin.y)/scale-PICT_SIZE/2;
        int w=newMask[i].size.width/scale;
        int h=newMask[i].size.height/scale;
        if(h<0){h*=-1;}else{y0-=h;}
        if(w<0){w*=-1;x0-=w;}
        //cout<<"x0="<<x0<<" y0="<<y0<<" w="<<w<<" h="<<h<<endl;
        mask.xMask=x0;
        mask.yMask=y0;
        mask.imgW=w;
        mask.mH=h;
        mask.set();
        //cout<<"dlt0="<<mask.dlt0<<" dlt1="<<mask.dlt1<<" dlt2="<<mask.dlt2<<endl;
        letter->push_back(mask);
    }
    newMask.resize(0);
    //cout<<" letter->mask32Count()="<<letter->mask32Count()<<endl;
    if(drawMaskMode==ERASE_IN_IMAGE){
        letter->setLetterDimensions();
        letter->cropLetter();
        letter->setLetterDimensions();
    }else{
       letter->reloadMask();
    }
    //letter->setLetterDimensions();
    //letter->cropLetter();
    //letter->setLetterDimensions();
    //cout<<" letter->mask32Count()="<<letter->mask32Count()<<endl;
    //imageEditor->WriteImageData(letterImgCrop,"/_Image2OCR/_1Draw1_.jpg",0); exit(0);
    img->destroy();
    img=letter->drawLetterPict(1);
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
    pechaSize=[pechaImage size];
    drawMaskMode=0;
    drawMode=DRAW_MASK;
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;
    aliKali->saveLetter(letter);
    
    for(int i=0;i<letter->mask32Count();i++){
        letter->mask32[i].xMask+=PICT_SIZE/2;
        letter->mask32[i].yMask+=PICT_SIZE/2;
    }

    [self setNeedsDisplay:YES];

}

-(IBAction)eraseInImage:(id)sender{
    drawMaskMode=ERASE_IN_IMAGE;
    drawMode=DRAW_MASK;
    GImageEditor *imageEditor=(GImageEditor*)inputData.imageEditor;
    GBitmap *img=letter->drawLetterPict(3);
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
    pechaSize=[pechaImage size];
    newMask.resize(0);
    [self setNeedsDisplay:YES];
}

-(IBAction)eraseAllMask:(id)sender{
    cout<<"clearAllMAsks"<<endl;
    letter->eraseAllMask();
    drawMode=DRAW_MASK;
    drawMaskMode=DRAW_NEW_MASK;
    GImageEditor *imageEditor=(GImageEditor*)inputData.imageEditor;
    GBitmap *img=letter->drawLetterPict(1);
    pechaImage=(NSImage*)imageEditor->WriteNSImage((GBitmap*)img);
    pechaSize=[pechaImage size];
    newMask.resize(0);
    [self setNeedsDisplay:YES];
}


-(void)drawSelected:(vector<OCRFocalLine>&)lineV{
    
    float fSize=6*zoom/pZoom;
    
    [NSBezierPath setDefaultLineWidth:3*zoom];
    NSBezierPath* path=[NSBezierPath bezierPath];
    NSBezierPath* path2=[NSBezierPath bezierPath];
    int index;
    NSPoint centerPoint,startPoint,endPoint;
    for (int i=0; i<selectLine.size();i++){
        index=selectLine[i];
        centerPoint=NSMakePoint((lineV[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineV[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineV[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineV[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineV[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineV[index].end.y+.5)*scale*zoom+dY);
        
        if(lineV[index].corC<60){
            [path moveToPoint:startPoint];
            [path lineToPoint:endPoint];
        }else{
            //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
            //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
            int w=lineV[index].radius*scale*zoom;
            int r=lineV[index].radius*scale*zoom/2;
            //int w=2;
            [path2 appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                             centerPoint.y-r,w,w)];
            //cout<<"yc="<<lineVec[index].start.x<<endl;
            
        }
    }
    
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:fSize] forKey: NSFontAttributeName];
    [attrs setObject:NSColorFromRGB(0x0202020) forKey: NSForegroundColorAttributeName];
    
    [[NSColor redColor] set];
    [path stroke];
    //draw selected lines data
    
    for (int i=0; i<selectLine.size();i++){
        index=selectLine[i];
        centerPoint=NSMakePoint((lineV[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineV[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineV[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineV[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineV[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineV[index].end.y+.5)*scale*zoom+dY);
        [[NSColor whiteColor] set];
        NSPoint p;
        if(selectLine.size()>1){
            p=endPoint;
        }else{
            p=centerPoint;
        }
        
        NSString *report;
        
        if(inputData.start>0&&inputData.lineIndex.size()!=0){
            if(!drawOCRLetter){
                [self setOCRLetter];
            }else{
                cout<<"select= "<<selectLine[0]<<endl;
                [self setOCRDataForLine:selectLine[0]];
            }
            report=[NSString stringWithFormat:@"%@",[NSString stringWithUTF8String:inputData.data["report"].c_str()]];
        }else{
            ostringstream str;
            str<<"index="<<index //id number
            <<"\nalpha="<<lineV[index].alpha       // угол наклона линии в градусах (0 до 360° grade).
            <<"\nalpInt="<<lineV[index].alpInt       // целочисленное значение угола наклона линии в градусах (0 до 360° grade).
            <<"\nlenA="<<lineV[index].lenA           // количество пикселов линии.
            <<"\nlen="<<lineV[index].len         // длина прямой соединяющей концы линии.
            <<"\nlenM="<<lineV[index].lenM        // длина сглаженной фокальной линии.
            <<"\nlenP="<<lineV[index].lenP         // длина укороченной кривой Безье (как часть длины сглаженной фокальной линии).
            <<"\nlenB="<<lineV[index].lenB       // длина кривой Безье (вычислено по уравнению растеризации, медленно).
            <<"\nalphaP1="<<lineV[index].alphaP1      // угол наклона первого контрольного вектора (0 до 360° grade).
            <<"\nlenP1="<<lineV[index].lenP1     // длина первого контрольного вектора в pix.
            <<"\nalphaP2="<<lineV[index].alphaP2     // угол наклона второго контрольного вектора (0 до 360° grade).
            <<"\nlenP2="<<lineV[index].lenP2      // длина второго контрольного вектора в pix.
            <<"\nalphaP1P2="<<lineV[index].alphaP1P2    // угол наклона вектора от первой управляющей точки до второй управляющей точки.
            <<"\nlenP1P2="<<lineV[index].lenP1P2;      // расстояние вектора от первой управляющей точки до второй управляющей точки в pix.
            
            report=[NSString stringWithUTF8String:str.str().c_str()];
        
        }
            
        //cout<<"zoom="<<zoom<<endl;
        [NSBezierPath fillRect:NSMakeRect(p.x, p.y, fSize*zoom*12,  fSize*zoom*20 )];
        
        [[NSColor grayColor] set];
        [NSBezierPath setDefaultLineWidth:0.3*zoom];
        
        [NSBezierPath strokeRect:NSMakeRect(p.x, p.y, fSize*zoom*12,  fSize*zoom*20)];
        [report drawAtPoint:NSMakePoint(p.x+10*zoom,p.y+10*zoom) withAttributes: attrs];
        //if(zoom>1.5)[[NSString stringWithFormat:@"%d", lineVec[index].angl_t]
        //             drawAtPoint:centerPoint withAttributes: attrs];
        
    }
}


-(void)setOCRDataForLine:(int)index{

    int x=lineOCRVec[index].centerM.x;
    int y=lineOCRVec[index].centerM.y;
    int min=1000;
    int minIndex=-1;
    for(int i=0;i<lineVec.size();i++){
        int t=abs(lineVec[i].centerM.x-x)+abs(lineVec[i].centerM.y-y);
        if(t<min){
            min=t;
            minIndex=i;
        }
    }
    cout<<"selected line index"<<minIndex<<endl;
    OCRPoint center;
    center.x=lineVec[minIndex].centerM.x;
    center.y=lineVec[minIndex].centerM.y;
    inputData.start=100;
    
    //((GBitsetMatrix*)inputData.matrix)->lineCorrelation(lineVec[minIndex],
    //                                                     lineOCRVec[selectLine[0]],
    //                                                     center,
    //                                                     selectLine[0]);
    //cout<<"REPORT"<<inputData.data["report"]<<endl;

}


-(void)drawLines:(vector<OCRFocalLine>&)lineV :(int)mode{
    NSPoint startPoint,endPoint,centerPoint,centerM,cPoint1,cPoint2;
    
    float fSize; //font size
    if(zoom<2.3)fSize=9*zoom;
    if(zoom>=2.3)fSize=5*zoom;
    
    float pD=zoom/pZoom*4;  //points diameter
    float pR=pD/2;
    float sL=zoom/pZoom/4;

    
    if(drawMode==DRAW_LETTER||mode){
        sL=0.5;
        pD=8;
    }
    //if(mode){
    //    sL=0.7;
    //    pD=4;
    //}
    
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
    NSBezierPath *path4=[NSBezierPath bezierPath];
    NSBezierPath *path5=[NSBezierPath bezierPath];
    NSBezierPath *path7=[NSBezierPath bezierPath];    //Bezier handlers lines
    NSBezierPath *path8=[NSBezierPath bezierPath];    //Bezier handlers points
    NSBezierPath *path10=[NSBezierPath bezierPath];    //line center of mass
    
    
    
    NSColor *strColor=[NSColor colorWithCalibratedRed:0.7 green:0.7 blue:0.7 alpha:1];
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:fSize] forKey: NSFontAttributeName];
    
    int count=(uint)lineV.size();
    
    for (int index=0; index<count;index++) {
        
        centerPoint=NSMakePoint((lineV[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineV[index].center.y+.5)*scale*zoom+dY);
        
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        centerM=NSMakePoint((lineV[index].centerM.x+.5)*scale*zoom+dX,
                            drawRect.size.height*zoom-(lineV[index].centerM.y+.5)*scale*zoom+dY);
        startPoint=NSMakePoint((lineV[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineV[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineV[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineV[index].end.y+.5)*scale*zoom+dY);
        cPoint1=NSMakePoint((lineV[index].p1.x+.5)*scale*zoom+dX,
                            drawRect.size.height*zoom-(lineV[index].p1.y+.5)*scale*zoom+dY);
        cPoint2=NSMakePoint((lineV[index].p2.x+.5)*scale*zoom+dX,
                            drawRect.size.height*zoom-(lineV[index].p2.y+.5)*scale*zoom+dY);
        
        //centerPoint=NSMakePoint(lineVec[index].data[lineVec[index].data.size()/2].x*scale*zoom+dX,
        //    drawRect.size.height*zoom-lineVec[index].data[lineVec[index].data.size()/2].y*scale*zoom+dY);
        
        // Чем ближе коэффициент корреляции foat к 100, тем больше кривая похожа на прямую.
        // Тем меньше прямых на рисунке.
        //if(lineVec[index].corL>97.0){
        
        if(lineV[index].maxCor==LINE){
            [path moveToPoint:startPoint];
            [path lineToPoint:endPoint];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 lineToPoint:endPoint];
            }
        }else if(lineV[index].maxCor==ARC){
            [path3 moveToPoint:startPoint];
            [path3 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            }
        }else if(lineV[index].maxCor==SPIRAL){
            [path6 moveToPoint:startPoint];
            [path6 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            if(index==0){
                [path9 moveToPoint:startPoint];
                [path9 curveToPoint:endPoint controlPoint1:cPoint1 controlPoint2:cPoint2];
            }
            //draw Bezier handlers
        }else if(lineV[index].maxCor==CIRCLE){
            //cout<<"lineVec["<<index<<"].radius="<<lineVec[index].radius<<endl;
            //cout<<" x="<<lineVec[index].center.x<<" y="<<lineVec[index].center.y<<endl;
            int w=lineV[index].radius*scale*zoom;
            int r=lineV[index].radius*scale*zoom/2;
            //int w=2;
            [path2 appendBezierPathWithOvalInRect:NSMakeRect(centerPoint.x-r,
                                                             centerPoint.y-r,w,w)];
            //cout<<"yc="<<lineVec[index].start.x<<endl;
        }
        [path10 appendBezierPathWithOvalInRect:NSMakeRect(centerM.x-pD/2*zoom,
                                                          centerM.y-pD/2*zoom,pD*zoom,pD*zoom)];
        
        //if((zoom>4.5||drawMode==DRAW_OCR_LETTER)&&(lineV[index].maxCor==SPIRAL||lineV[index].maxCor==ARC)){
            [path7 moveToPoint:startPoint];
            [path7 lineToPoint:cPoint1];
            [path7 moveToPoint:endPoint];
            [path7 lineToPoint:cPoint2];
            [path8 appendBezierPathWithRect:NSMakeRect(cPoint1.x-pD/2*zoom,
                                                       cPoint1.y-pD/2*zoom,pD*zoom,pD*zoom)];
            [path8 appendBezierPathWithRect:NSMakeRect(cPoint2.x-pD/2*zoom,
                                                       cPoint2.y-pD/2*zoom,pD*zoom,pD*zoom)];
            
        //}
        
        
        //[[NSString stringWithFormat:@"%d:%d", lineVec[index].angl_t,lineVec[index].index]
        // drawAtPoint:centerPoint withAttributes: attrs];
        
        //if(zoom>1.5)[[NSString stringWithFormat:@"%d", lineVec[index].alpha]
        // drawAtPoint:centerPoint withAttributes: attrs];;
        
        [path4 moveToPoint:startPoint];
        for(int t=0;t<lineV[index].mData.size();t++){
            //cout<<"x="<<lineV[index].mData[t].x<<" y="<<lineV[index].mData[t].y<<endl;
            startPoint=NSMakePoint(((float)lineV[index].mData[t].x+.5)*scale*zoom+dX,
                                   drawRect.size.height*zoom-((float)lineV[index].mData[t].y+.5)*scale*zoom+dY);
            [path4 lineToPoint:startPoint];
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
    [path4 stroke];
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
    
}


-(void)drawPoints:(vector<OCRFocalLine>&)lineV{
    
    NSPoint startPoint,endPoint,centerPoint;
    float fSize;
    if(zoom<2.3)fSize=9*zoom;
    if(zoom>=2.3)fSize=5*zoom;
    float pD=zoom/pZoom*7;
    float sL=4;
    if(drawMode==DRAW_LETTER){
        sL=0.5;
        pD=8;
    }
    float pR=pD/2;
    int pR_=2*pR;

    
    [NSBezierPath setDefaultLineWidth:2*zoom/sL];
    NSBezierPath *path=[NSBezierPath bezierPath];
    
    int count=(uint)lineV.size();
    
    for (int index=0; index<count;index++) {
        centerPoint=NSMakePoint((lineV[index].center.x+.5)*scale*zoom+dX,
                                drawRect.size.height*zoom-(lineV[index].center.y+.5)*scale*zoom+dY);
        if(centerPoint.x<0 || centerPoint.x>drawRect.size.width||
           centerPoint.y<0 || centerPoint.y>drawRect.size.height) continue;
        
        startPoint=NSMakePoint((lineV[index].start.x+.5)*scale*zoom+dX,
                               drawRect.size.height*zoom-(lineV[index].start.y+.5)*scale*zoom+dY);
        endPoint=NSMakePoint((lineV[index].end.x+.5)*scale*zoom+dX,
                             drawRect.size.height*zoom-(lineV[index].end.y+.5)*scale*zoom+dY);
        
        //centerPoint=NSMakePoint(lineVec[index].data[lineVec[index].data.size()/2].x*scale*zoom+dX,
        //    drawRect.size.height*zoom-lineVec[index].data[lineVec[index].data.size()/2].y*scale*zoom+dY);
        
        if(lineV[index].start.type==C_POINT && lineV[index].end.type==C_POINT){
            [[NSColor redColor] set];
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(startPoint.x-pR_*zoom,startPoint.y-pR_*zoom,pR_*2*zoom,pR_*2*zoom)];
            [path fill];
            
        }else{
            
            [[NSColor greenColor] set];
            path=[NSBezierPath bezierPathWithOvalInRect: NSMakeRect(startPoint.x-pR*zoom,startPoint.y-pR*zoom,pD*zoom,pD*zoom)];
            [path fill];
            
        }
        
        
        if(lineV[index].start.type==C_POINT && lineV[index].end.type==C_POINT){
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
}


-(void)drawNewMask{
    //cout<<"drawNewMask newMask.size()="<<newMask.size()<<endl;
    if(drawMaskMode!=ERASE_IN_IMAGE){
        [NSBezierPath setDefaultLineWidth:3*zoom];
    }else{
        [NSBezierPath setDefaultLineWidth:0.3*zoom];
    }
    NSBezierPath* path=[NSBezierPath bezierPath];
    
    for(int i=0;i<newMask.size();i++){
        [path appendBezierPathWithRect:NSMakeRect(newMask[i].origin.x,
                                                  newMask[i].origin.y,newMask[i].size.width,newMask[i].size.height)];
    }
    if(drawMaskMode!=ERASE_IN_IMAGE){
        [[NSColor greenColor] set];
        [path stroke];
    }else{
        //cout<<"erase"<<" newMask.size()="<<newMask.size()<<endl;
        [[NSColor whiteColor] set];
        [path fill];
        [[NSColor grayColor] set];
        [path stroke];
        
    }
}


-(void)drawMask{
    
    NSPoint startPoint;
    float fSize;
    if(zoom<2.3)fSize=9*zoom;
    if(zoom>=2.3)fSize=5*zoom;
    float pD=2;
    //float pR=pD/2;
    float sL=2;
    if(drawMode==DRAW_LETTER){
        sL=0.5;
        pD=8;
    }
    int dYImage=PICT_SIZE/2*scale;
    
    [NSBezierPath setDefaultLineWidth:4*zoom/sL];
    NSBezierPath *path=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:6*zoom/sL];
    NSBezierPath *pathLetterBox=[NSBezierPath bezierPath];
    NSBezierPath *pathSelectedMask=[NSBezierPath bezierPath];
    [NSBezierPath setDefaultLineWidth:2*zoom/sL];
    NSBezierPath *path1=[NSBezierPath bezierPath];
    NSBezierPath *pathStr=[NSBezierPath bezierPath];
    //draw string
    [pathStr appendBezierPathWithRect:NSMakeRect(0,drawRect.size.height-(letter->y1*scale+dYImage),dYImage*2,(letter->y1-letter->y0)*scale)];
    [[NSColor grayColor] set];
    [pathStr stroke];
    
    
    [pathLetterBox appendBezierPathWithRect:NSMakeRect(dYImage-(letter->letterW/2-letter->dX)*scale,drawRect.size.height-dYImage-(letter->letterH/2+letter->dY)*scale,letter->letterW*scale,letter->letterH*scale)];
    [[NSColor blueColor] set];
    [pathLetterBox stroke];
    //cout<<"selectMask="<<selectMask<<endl;
    //cout<<"letter->letterH="<<letter->letterH<<" letter->letterW="<<letter->letterW<<endl;
    
    for(int i=0;i<letter->mask32Count();i++){
        GBitMask32 mask=letter->mask32[i];
        startPoint=NSMakePoint((mask.xMask)*scale*zoom+dX,
                               drawRect.size.height*zoom-(mask.yMask+mask.mH)*scale*zoom+dY);
        
        if(i!=selectMask){
            if(i==0){
                [path appendBezierPathWithRect:NSMakeRect(startPoint.x,
                                                       startPoint.y,mask.imgW*zoom*scale,mask.mH*zoom*scale)];
            }else{
                [path1 appendBezierPathWithRect:NSMakeRect(startPoint.x,
                                                          startPoint.y,mask.imgW*zoom*scale,mask.mH*zoom*scale)];
            }
        }else{
            [pathSelectedMask appendBezierPathWithRect:NSMakeRect(startPoint.x,
                                                       startPoint.y,mask.imgW*zoom*scale,mask.mH*zoom*scale)];
        }
    }
    [[NSColor redColor] set];
    [path stroke];
    [[NSColor blueColor] set];
    [path1 stroke];
    [[NSColor greenColor] set];
    [pathSelectedMask stroke];
}


-(void)drawOCRLetter{
    //draw recognition letter
    float pD=20;
    float sL=0.5;
    
    [NSBezierPath setDefaultLineWidth:2*zoom/sL];
    NSBezierPath *path=[NSBezierPath bezierPath];
    
    path=[NSBezierPath bezierPath];
    int pD_=4*pD;
    int index;
    for(int i=0;i<inputData.lineIndex.size();i++){
        index=inputData.lineIndex[i];
        NSPoint centerM=NSMakePoint((lineVec[index].centerM.x+.5)*scale*zoom+dX,
                                    drawRect.size.height*zoom-(lineVec[index].centerM.y+.5)*scale*zoom+dY);
        
        [path appendBezierPathWithOvalInRect:NSMakeRect(centerM.x-pD_/2*zoom,
                                                        centerM.y-pD_/2*zoom,pD_*zoom,pD_*zoom)];
    }
    [[NSColor blueColor] set];
    [path fill];
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
