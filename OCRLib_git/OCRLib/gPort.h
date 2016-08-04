//
//  gPort.h
//  OCRLib
//
//  Created by dharmabook on 15/07/15.
//
//

#import <Cocoa/Cocoa.h>
#import "GImageEditor.h"
#import "GFontEditor.h"

#define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:r/255.f green:g/255.f blue:b/255.f alpha:a/255.f]
#define NSColorFromRGB(rgbValue) [NSColor colorWithCalibratedRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0]


enum{
    DRAW_VECTOR,
    DRAW_LETTERBASE,
    DRAW_LETTER,
    DRAW_OCR_LETTER,
    DRAW_MASK,
    DRAW_NEW_MASK,
    DRAW_LETTER_BOX,
    DRAW_LETTER_LINE,
    ERASE_IN_IMAGE
};


@interface gPort : NSView
{
    NSImage *pechaImage;
    int drawPechaImage;
    NSRect pechaZoomBounds;
    NSRect pechaBounds;
    float zoomImage, scale;
    short hL,wL;
    float zoom;
    float pZoom; //пиксельное увеличение
    short redraw;
    NSSize pechaSize;
    NSRect drawRect;
    NSRect drawRectZoom;
    int dX;
    int dY;
    int dl;
    int pageNum;
    GLetter *letter;
    int drawOCRLetter;
    vector<OCRPoint>strPoint;        //массив фокальных точек
    vector<OCRFocalLine>lineVec;     //массив фокальных линий
    vector<OCRFocalLine>lineOCRVec;  //массив фокальных линий
    vector<NSRect>newMask;           //массив нарисованных масок
    vector<uint>selectLine;
    vector<uint>searchResult;
    int selectOCR;
    int selectMask;
    int drawMode;
    int drawMaskMode;
    float mouseX;
    float mouseY;
    string destString;
}

-(IBAction)moveLeft:(id)sender;
-(IBAction)moveRight:(id)sender;
-(IBAction)zoomIn:(id)sender;
-(IBAction)zoomOut:(id)sender;
-(IBAction)homeImg:(id)sender;
-(IBAction)OCRLetterByIndex:(id)sender;
-(IBAction)removeMask:(id)sender;
-(IBAction)saveLetter:(id)sender;
-(IBAction)eraseInImage:(id)sender;
-(IBAction)eraseAllMask:(id)sender;
-(IBAction)setLetterBox:(id)sender;
-(IBAction)setLetterLineBox:(id)sender;
-(IBAction)setDrawNewMask:(id)sender;
- (IBAction)setDrawMask:(id)sender;

-(void)drawVectorPort;
-(void)drawLetterBase;
-(void)drawLetter;
-(void)setPechaImage;
-(void)setLetterBase:(string)path;
-(void)openLetterBasePage:(int)index;
-(void)setLetter:(int)index;
-(void)setOCRLetter;
-(void)drawVector;
-(void)drawLines:(vector<OCRFocalLine>&)v :(int)mode;
-(void)drawPoints;
-(void)drawMask;
-(void)drawNewMask;
-(void)drawPechaImage;
-(void)drawLetterBaseImage;
-(void)drawLetterImage;
-(void)drawSelected:(vector<OCRFocalLine>&)lineV;
-(void)drawOCRLetter;
-(void)setDrawMode:(int)mode;
-(void)mouseDown:(NSEvent *)theEvent;
-(void)mouseDragged:(NSEvent *)theEvent;
-(void)openLetterByIndex:(NSInteger)index;
-(void)setOCRDataForLine:(int)index;
-(void)drawLetterSearch:(vector<uint>&)searchResult;


@end



