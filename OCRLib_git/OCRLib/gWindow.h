//
//  gWindow.h
//  OCRLib
//
//  Created by dharmabook on 16/07/15.
//
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import "gPort.h"

#define RGBA(r,g,b,a) [NSColor colorWithCalibratedRed:r/255.f green:g/255.f blue:b/255.f alpha:a/255.f]
#define NSColorFromRGB(rgbValue) [NSColor colorWithCalibratedRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0]


@interface gWindow : NSObject{
    IBOutlet NSWindow *window;
    NSView *superview;
    WebView *webView;
    gPort   *gPortView;
    NSString *pathURL;
    NSButton *button1;
    NSButton *buttonDict;
    NSButton *buttonLeft;
    NSButton *buttonRight;
    NSButton *buttonSave;
    NSButton *buttonTranslate;
    NSButton *buttonLanguage;
    
    NSTextField *pageNumField;
    NSTextField *searchTextField;
    NSTextField *lableFill;
    NSTextField *letterName;
    int pageNum;
    int yOffset;
    GLogicProcessor *logicProcessor;
    string textPath;
    int translationMode;
    int letterIndex;
    int language;

}

-(id)initView;
-(id)initLetter:(int)index;
-(id)initDictionary:(string)word;
-(id)initTranslation;
-(id)initLetterBase;
-(IBAction)openDict:(id)sender;
-(IBAction)openLeft:(id)sender;
-(IBAction)openRight:(id)sender;
-(IBAction)openPage:(id)sender;
-(IBAction)previousLetter:(id)sender;
-(IBAction)nextLetter:(id)sender;
-(IBAction)lastLetter:(id)sender;
-(IBAction)previousLetterBasePage:(id)sender;
-(IBAction)nextLetterBasePage:(id)sender;
-(IBAction)lastLetterBasePage:(id)sender;
-(IBAction)openLetterBasePage:(id)sender;
-(IBAction)dictSave:(id)sender;
-(IBAction)dictTranslate:(id)sender;
-(IBAction)exportDictionary:(id)sender;
-(IBAction)setOCRFocalLine:(id)sender;
-(IBAction)setLanguage:(id)sender;
-(IBAction)runJS:(id)sender;
-(IBAction)openLetter:(id)sender;
-(IBAction)openLettersByName:(id)sender;
-(IBAction)exportLetterBase:(id)sender;
-(IBAction)closeWindow:(id)sender;


//-(void)showWindow;
-(void)startLetterBase:(string)path;
-(void)initTable;
//messages for JavaScript
-(void)bookOCR:(NSString *)message;
-(void)showMessage:(NSString *)message;
-(void)openFileForOCR:(NSString *)message;
-(void)imageProcess:(NSString *)message;
-(NSString *)alertInput: (NSString *)prompt :(NSString *)dValue :(NSString *)infoValue;

@end

#define alert(x)      NSAlert *alert = [[NSAlert alloc] init];\
[alert addButtonWithTitle:@"OK"];\
[alert setMessageText:x];\
[alert runModal];\
[alert release];\


