//
//  gWindow.m
//  OCRLib
//
//  Created by dharmabook on 16/07/15.
//
//

#import "gWindow.h"
#import "GLogicProcessor.h"
#import "GMainEditor.h"
#import "GMemory.h"
#import <iostream>
#import "php2stl.h"
#include <stdlib.h>

using namespace std;

@implementation gWindow

- (void)dealloc
{
    [super dealloc];
}

- (void)windowDidResize:(NSNotification *)notification;
{
    NSSize size = [window frame].size;
    NSRect viewRect=[window frame];
    viewRect.origin.x=0;
    viewRect.origin.y=0; //Apple bag
    viewRect.size.height-=56;
    
    /*
     buttonDict
     NSButton *buttonLeft;
     NSButton *buttonRight;
     NSButton *buttonSave;
     NSButton *buttonTranslate;
     
     */
    
    [buttonDict setFrame:NSMakeRect(size.width-64,size.height-56,64,34)];
    //[buttonDict setFrame: NSMakeRect(size.width-64,size.height-34,64,34)];
    [buttonLeft setFrame: NSMakeRect(0,size.height-56,64,34)];
    [pageNumField setFrame:NSMakeRect(64,size.height-56,64,34)];
    [buttonRight setFrame: NSMakeRect(128,size.height-56,64,34)];
    [buttonSave setFrame: NSMakeRect(192,size.height-56,64,34)];
    [buttonLanguage setFrame: NSMakeRect(256,size.height-56,64,34)];
    [buttonTranslate setFrame: NSMakeRect(size.width-128,size.height-56,64,34)];
    [searchTextField setFrame:NSMakeRect(size.width-384,size.height-56,256,34)];
    [lableFill setFrame:NSMakeRect(0,size.height-56,size.width,34)];
    
    [webView setFrame:viewRect];
    //NSLog(@"window width = %f, window height = %f", size.width,
    //      size.height);
}


/**brief Вызывается один раз при старте нового окна
 Перехватываем вызовы PHP и CGI
*/

- (void)webView:(WebView *)sender didStartProvisionalLoadForFrame:(WebFrame *)frame
{
    //cout<<"didStartProvisionalLoadForFrame"<<endl;
    string path;

    NSString * requestURL=[[[[frame provisionalDataSource] request] URL] absoluteString];
    //NSLog(@"load %@",requestURL);
    if([requestURL length])path=[requestURL UTF8String];
    int pos=(int)path.find("/cgi/");
    
    if(pos!=-1){  //перехватываем обращение к cgi
        path=substr((uint)path.find("?")+1,(uint)path.size(),path);
        if(path.find("port=gport")==-1){
            [[webView mainFrame] stopLoading];
            inputData.data["rootApp"]="/_Image2OCR/OCRLib.app/Contents/MacOS/OCRLib";
            string command=inputData.data["rootApp"]+" \""+path+"\"";
            writeText(command,"/_Image2OCR/command.sh");
            system("open -a /Applications/iTerm.app /_Image2OCR/command.sh");
        }else{
           [self startLetterBase:path];
           [[webView mainFrame] stopLoading];
        //[[webView mainFrame] loadHTMLString:[NSString stringWithUTF8String:destString.c_str()] baseURL:[NSURL fileURLWithPath:@"/1.html"]];
        }
    }
}

/* Called just before a webView attempts to load a resource.  Here, we look at the
 request and if it's destined for our special protocol handler we modify the request
 so that it contains an NSDictionary containing some information we want to share
 between the code in this file and the custom NSURLProtocol.  */
/* обращение к локальным файлам перенаправляется к локальным ресурсам программы.
 */
-(NSURLRequest *)webView:(WebView *)sender resource:(id)identifier
         willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponse
          fromDataSource:(WebDataSource *)dataSource {
    
    //print("load"<<endl);
    string path=[[[request URL] absoluteString] UTF8String];
        
   // NSLog(@"load window %@",[[request URL] absoluteString ] );
    //cout<<" init path="<<path<<endl;
    
    //далее обрабатываем запрос и если нужно перенаправляем
    int pos=(int)path.find(".php");

    if(pos!=-1){  //перехватываем обращение к php
        path=substr((int)path.find("?")+1,(int)path.size(),path);
        string cmd="php "+inputData.data["root"]+"edit/ocr.php \""+path+"\"";
        cout<<"cmd="<<cmd<<endl;
        string out=run (cmd);
        int pos1=(int)path.find("pict");
        if(pos1>0){ //перехватываем обращения к выдаче изображений из базы дынных
            path=inputData.data["root"]+"edit/1.jpg";
            writeText(out,path);
        }else{
            string phpPath=inputData.data["root"]+"edit/";
            [[webView mainFrame]
             loadHTMLString:[NSString stringWithUTF8String:out.c_str()]
             baseURL: [NSURL URLWithString:
                       [NSString stringWithUTF8String:phpPath.c_str()]]];
        }
        //if(is_dir(path))path+="/index.php";
        cout<<"path@="<<path<<endl;
        NSString *str=[NSString stringWithUTF8String:path.c_str()];
        NSURLRequest *newRequest=[NSURLRequest requestWithURL:[NSURL fileURLWithPath:str]];
        return newRequest;
    }
    return request;
}

-(IBAction)openLetter:(id)sender{
    NSString *text=[searchTextField stringValue];
    ulong index=[text intValue];
    ulong size=((GFontEditor*)inputData.fontEditor)->aliKali->size()-2;
    if(index>size)index=size;
    [gPortView openLetterByIndex:index];
    GLetter *letter=((GFontEditor*)inputData.fontEditor)->aliKali->getLetter(index);
    NSString *name=[NSString stringWithUTF8String:letter->name.c_str()];
    [letterName setStringValue:name];
    //((GStr<int>*)inputData.pref)->put(23,(uint)index);
}

-(IBAction)previousLetter:(id)sender{
    NSString *text=[searchTextField stringValue];
    int index=[text intValue];
    index--;
    if(index<0)index=0;
    [gPortView openLetterByIndex:index];
    [searchTextField setStringValue:[NSString stringWithFormat:@"%d",index]];
    GLetter *letter=((GFontEditor*)inputData.fontEditor)->aliKali->getLetter(index);
    NSString *name=[NSString stringWithUTF8String:letter->name.c_str()];
    [letterName setStringValue:name];
    //((GStr<int>*)inputData.pref)->put(23,index);
}

-(IBAction)nextLetter:(id)sender{
    NSString *text=[searchTextField stringValue];
    int index=[text intValue];
    index++;
    int size=((GFontEditor*)inputData.fontEditor)->aliKali->size()-2;
    if(index>size)index=size;
    [gPortView openLetterByIndex:index];
    [searchTextField setStringValue:[NSString stringWithFormat:@"%d",index]];
    GLetter *letter=((GFontEditor*)inputData.fontEditor)->aliKali->getLetter(index);
    NSString *name=[NSString stringWithUTF8String:letter->name.c_str()];
    [letterName setStringValue:name];
    //((GStr<int>*)inputData.pref)->put(23,index);
}

-(IBAction)lastLetter:(id)sender{
    int size=((GFontEditor*)inputData.fontEditor)->aliKali->size()-2;
    [gPortView openLetterByIndex:size];
    [searchTextField setStringValue:[NSString stringWithFormat:@"%d",size]];
    GLetter *letter=((GFontEditor*)inputData.fontEditor)->aliKali->getLetter(size);
    NSString *name=[NSString stringWithUTF8String:letter->name.c_str()];
    [letterName setStringValue:name];
    //((GStr<int>*)inputData.pref)->put(23,size);
}

-(IBAction)previousLetterBasePage:(id)sender{
    NSString *text=[pageNumField stringValue];
    int index=[text intValue];
    index--;
    if(index<0)index=0;
    [gPortView openLetterBasePage:index];
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",index]];
    //((GStr<int>*)inputData.pref)->put(24,index);

}

-(IBAction)nextLetterBasePage:(id)sender{
    NSString *text=[pageNumField stringValue];
    int index=[text intValue];
    index++;
    int size=(((GFontEditor*)inputData.fontEditor)->aliKali->size()-2)/300;
    if(index>size)index=size;
    [gPortView openLetterBasePage:index];
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",index]];
    //((GStr<int>*)inputData.pref)->put(24,index);
}
-(IBAction)openLetterBasePage:(id)sender{
    NSString *text=[pageNumField stringValue];
    int index=[text intValue];
    int size=(((GFontEditor*)inputData.fontEditor)->aliKali->size()-2)/300;
    if(index>size)index=size;
    [gPortView openLetterBasePage:index];
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",index]];
    //(GStr<int>*)inputData.pref)->put(24,index);
}
-(IBAction)lastLetterBasePage:(id)sender{
    int index=(((GFontEditor*)inputData.fontEditor)->aliKali->size()-2)/300;
    [gPortView openLetterBasePage:index];
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",index]];
    //((GStr<int>*)inputData.pref)->put(24,index);
}

-(IBAction)openLettersByName:(id)sender{
    NSString *str=[letterName stringValue];
    string name=[str UTF8String];
    if(name=="")return;
    vector<ulong>searchResult;
    
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;
    for(uint i=0;i<aliKali->size();i++){
        GLetter *letter=aliKali->getLetter(i);
        //cout<<"str="<<letter->name<<endl;
        if(letter->name!=name){letter->destroy();continue;}
        //cout_<<"str="<<letter->name<<endl;
        //index++;
        //if(pageNum>0&&index<pageNum*25-1)continue;
        searchResult.push_back(i);
        letter->destroy();
    }
    cout<<"searchResult="<<searchResult.size()<<endl;
    [gPortView drawLetterSearch:searchResult];

}

-(IBAction)exportLetterBase:(id)sender{
    GFontEditor *fontEditor =(GFontEditor*)inputData.fontEditor;
    string path=inputData.data["OCRTables"]+"GFont.xml";
    NSString *str=[self alertInput:@"Please confirm export database by path:" :[NSString stringWithUTF8String:path.c_str()] :@""];
    if(str!=nil){
        path=[str UTF8String];
        //cout<<"path="<<path<<endl;
        fontEditor->exportGFontsDB(path);
    }


}

-(IBAction)openDict:(id)sender {
    //read content of <div> by ID
    //cout<<"new window open"<<endl;
    NSString *text=[searchTextField stringValue];
    cout<<"[text length]="<<[text length]<<endl;
    if(![text length]){
        DOMDocument *myDOMDocument = [[webView mainFrame] DOMDocument];
        DOMElement *contentTitle = [myDOMDocument getElementById:@"searchText"];
        text=[[contentTitle firstChild] nodeValue];
        cout<<"text lengthJS="<<[text UTF8String]<<endl;
        if(![text length])return;
    }
    //NSLog(str);
    string str_=[text UTF8String];
    str_+="་";
    str_=str_replace("་་","་",str_);

    [[gWindow alloc]initDictionary:str_];
    //[titleText setStringValue:[[contentTitle firstChild] nodeValue]];
}

-(IBAction)openLeft:(id)sender{
    pageNum--; if(pageNum<0)pageNum=0;
    //((GStr<int>*)inputData.pref)->put(21,pageNum);
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d", pageNum]];
    [self dictTranslate:nil];
}
-(IBAction)openRight:(id)sender{
    pageNum++;
    //((GStr<int>*)inputData.pref)->put(21,pageNum);
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d", pageNum]];
    [self dictTranslate:nil];
}
-(IBAction)openPage:(id)sender{
    NSString *text = [pageNumField stringValue];
    pageNum=[text intValue];
    //((GStr<int>*)inputData.pref)->put(21,pageNum);
    [self dictTranslate:nil];
    cout<<"open page"<<pageNum<<endl;

}

-(IBAction)dictSave:(id)sender{
    //cout<<"saveToTranslationDictionary"<<endl;
    /*get all text
    WebFrame *frame = [webView mainFrame];
    WebDataSource *source = [frame dataSource];
    NSData *data = [source data];
    NSString *text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    */
    WebScriptObject *scriptObject = [webView windowScriptObject];
    NSString *text=[scriptObject evaluateWebScript:@"readText()"];
    if(![text length])return;
    string str=[text UTF8String];
    logicProcessor->saveToTranslationDictionary(str);
}

-(IBAction)exportDictionary:(id)sender{
    cout<<"export dictionary"<<endl;
    GMemory *longMemory=(GMemory*)inputData.longMemory;
    string str;
    if(translationMode==TRANSLATE_ENG){
        str="translationDictEng";
    }else{
        str="translationDictRus";
    }
    longMemory->loadTable(str);
    GVector *dk=longMemory->table[str].data;
    dk->exportTStr("/_Image2OCR/exportDictionary.txt");
}

-(IBAction)dictTranslate:(id)sender{
    if(textPath=="")[superview openFileForTranslation:nil];
    if(textPath=="")return;
    inputData.data["ocrData"]="";
    inputData.data["db"]="dict";
    //inputData.data["text"]="/KANGYUR_OCR_TAB/_W1PD96682-I1PD96822_ALL.html";
    inputData.data["text"]=textPath;
    cout<<" textPath="<<textPath<<endl;
    char buffer[10]; sprintf (buffer, "%d", pageNum);
    inputData.data["page"]=buffer;
    yOffset = [[webView stringByEvaluatingJavaScriptFromString:@"window.pageYOffset"] intValue];
    //cout<<"yOffset="<<yOffset<<endl;
    string report;
    if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary();
    logicProcessor->mainTextTranslation(report);
    string maket;
    readText(maket, "/_Image2OCR/edit/OSBL_Dictionary.html");
    maket=str_replace("@@@TEXT@@@", report, maket);
    writeText(maket,"/_Image2OCR/edit/_report.html");
    //[[webView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"/_Image2OCR/edit/OSBL_Dictionary.html"]]];
    [[webView mainFrame] loadHTMLString:[NSString stringWithUTF8String:maket.c_str()] baseURL:nil];
    [self runJS:self];
    //cout<<"reload pageNum="<<pageNum<<endl;
}

- (IBAction)runJS:(id)sender{
    // The user just chose a theme in the NSPopUpButton, so we replace the HTML
    // document's CSS file using JavaScript.
    
    cout<<"yOffset="<<yOffset<<endl;
    WebScriptObject *scriptObject = [webView windowScriptObject];
    //NSString *theme = [[themeChooser selectedItem] representedObject];
    NSString *script = [NSString stringWithFormat:@"window.scrollTo(0,%d);",yOffset];
    [scriptObject evaluateWebScript:script];
    /*
    DOMDocument *myDOMDocument = [[webView mainFrame] DOMDocument];
    DOMHTMLElement *contentTitle = (DOMHTMLElement *)[myDOMDocument getElementById:@"fileList"];
    [contentTitle setInnerText:@"321"];
    */
     
}


- (IBAction)setDrawVector:(id)sender{
    [gPortView setDrawMode:DRAW_LETTER];
    //((GStr<int>*)inputData.pref)->put(25,DRAW_LETTER);
}

- (void)openFileForOCR:(NSString *)message{
    //NSLog(message);
    // This method is called from the JavaScript "onClick" handler of the INPUT element
    // in the HTML. This shows you how to have HTML form elements call Cocoa methods.
    //NSRunAlertPanel(@"Message from JavaScript", message, nil, nil, nil);
    string filePath;
    
    NSOpenPanel * panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:YES];
    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:YES];
    
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton) {
            
            for (NSURL *fileURL in [panel URLs]) {
                NSString *str=[fileURL absoluteString];
                //cout<<"fileURL="<<[str UTF8String]<<endl;
                string filePath=[str UTF8String];
                filePath=str_replace("file://", "", filePath);
                DOMDocument *myDOMDocument = [[webView mainFrame] DOMDocument];
                DOMHTMLElement *contentTitle = (DOMHTMLElement *)[myDOMDocument getElementById:@"fileList"];
                [contentTitle setInnerText:[NSString stringWithUTF8String:filePath.c_str()]];
                //((GVector*)inputData.prefVector)->putStr(20,filePath);
                break;
                
            }
            //((GStr<int>*)inputData.pref)->put(20,0);
        }
    }];
}

- (void)bookOCR:(NSString *)message{
    //NSRunAlertPanel(@"Message from JavaScript bookOCR", message, nil, nil, nil);
    string fileList=[message UTF8String];
    fileList=str_replace("file://", "", fileList);
    inputData.fileList=explode("\n",fileList);
    inputData.data["ocrData"]="batchOCR";
    //((GVector*)inputData.prefVector)->putStr(20, textPath);
    ((GMainEditor*)inputData.mainEditor)->startOCRBatch();
    
    
}

- (void)showMessage:(NSString *)message{
    //NSLog(message);
    // This method is called from the JavaScript "onClick" handler of the INPUT element
    // in the HTML. This shows you how to have HTML form elements call Cocoa methods.
    NSRunAlertPanel(@"Message from JavaScript", message, nil, nil, nil);
    //run full dictionary report for this word
    //string str=[message UTF8String];
    //[[gWindow alloc]initDictionary:str];
    
}

/* This method is called by the WebView when it is deciding what
	methods on this object can be called by JavaScript.  The method
	should return NO the methods we would like to be able to call from
	JavaScript, and YES for all of the methods that cannot be called
	from JavaScript.
	*/
+ (BOOL)isSelectorExcludedFromWebScript:(SEL)selector {
    //NSLog(@"%@ received5 %@ for '%@'", self, NSStringFromSelector(_cmd), NSStringFromSelector(selector));
    if (selector == @selector(showMessage:)||
        selector == @selector(openFileForOCR:)||
        selector == @selector(bookOCR:)) {
        return NO;
    }
    return YES;
}

/* This method is called by the WebView to decide what instance
	variables should be shared with JavaScript.  The method should
	return NO for all of the instance variables that should be shared
	between JavaScript and Objective-C, and YES for all others.
	*/
+ (BOOL)isKeyExcludedFromWebScript:(const char *)property {
    //NSLog(@"%@ received6 %@ for '%s'", self, NSStringFromSelector(_cmd), property);
    if (strcmp(property, "sharedValue") == 0) {
        return NO;
    }
    return YES;
}



- (WebView *)webView:(WebView *)sender createWebViewWithRequest:(NSURLRequest *)request {
    //Apple обащает сделать здесь доступ к запросу
    NSLog(@"new window %@",[[request URL] absoluteString ] );
    //- (void)loadRequest:(NSURLRequest *)request
    //[self openWindow:self];
    gWindow *w=[[gWindow alloc]init];
    return w->webView;
}

/*
- (BOOL)webView:(WebView *)webView doCommandBySelector:(SEL)command
{
    if (command == @selector(copy:)) {
        NSString *markup = [[self selectedDOMRange] markupString];
        NSData *data = [markup dataUsingEncoding: NSUTF8StringEncoding];
        NSNumber *n = [NSNumber numberWithUnsignedInteger: NSUTF8StringEncoding];
        NSDictionary *options = [NSDictionary dictionaryWithObject:n forKey: NSCharacterEncodingDocumentOption];
        NSAttributedString *as = [[NSAttributedString alloc] initWithHTML:data options:options documentAttributes: NULL];
        NSString *selectedString = [as string];
        [as autorelease];
        
        NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
        [pasteboard clearContents];
        NSArray *objectsToCopy = [NSArray arrayWithObject: selectedString];
        [pasteboard writeObjects:objectsToCopy];
        return YES;
    }
    return NO;
}
*/

/*
DOMRange *ff = [ webView selectedDOMRange ];
NSString *marki = [ ff markupString];
NSData *data = [ marki dataUsingEncoding: NSUTF8StringEncoding ];
NSNumber *n = [ NSNumber numberWithUnsignedInteger: NSUTF8StringEncoding ];
NSDictionary *options = [ NSDictionary  dictionaryWithObject: n  forKey: NSCharacterEncodingDocumentOption ];
NSAttributedString *as =     [ [ NSAttributedString alloc ]     initWithHTML:         data
                                                                     options:    options
                                                          documentAttributes: NULL
                              ];
NSString *selectedString = [ as string ];
[ as release ];
*/

-(IBAction)closeWindow:(id)sender{}

-(void)initTable{
    //NSImageView *theImageView=[[NSImageView alloc] initWithFrame: NSMakeRect(0, 0, 1024, 1024)];

    NSImage *resizedImage = [[NSImage alloc] initWithSize: NSMakeSize(1024, 1024)];
    NSBitmapImageRep *bitmap =[[NSBitmapImageRep imageRepWithData:[resizedImage TIFFRepresentation]]retain];
    NSGraphicsContext *context=[NSGraphicsContext graphicsContextWithBitmapImageRep:bitmap];
    [NSGraphicsContext setCurrentContext:context];
    //[self lockFocus];
    //[sourceImage drawInRect: NSMakeRect(0, 0, 100, 100) fromRect: NSMakeRect(0, 0, originalSize.width, originalSize.height) operation: NSCompositeSourceOver fraction: 1.0];
    //[self unlockFocus];
    [NSGraphicsContext saveGraphicsState];
    [[resizedImage TIFFRepresentation] writeToFile:@"/_Image2OCR/507_1.tif" atomically:YES];
//[NSGraphicsContext restoreGraphicsState];

}

-(IBAction)setOCRFocalLine:(id)sender{
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    letterIndex=[searchTextField intValue];
    aliKali->setFocalLineInLetter(letterIndex);
    [gPortView setLetter:letterIndex];

}

-(IBAction)setLanguage:(id)sender{
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    //NSCenterTextAlignment  location:0 options:attrs];
    [attrs setObject:NSColorFromRGB(0xffffffff) forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    NSMutableParagraphStyle *st=[[NSParagraphStyle defaultParagraphStyle]mutableCopy];
    [st setAlignment:NSCenterTextAlignment];
    [attrs setObject:st forKey: NSParagraphStyleAttributeName];

    NSMutableAttributedString *str;
    if(language==TRANSLATE_ENG){
        language=TRANSLATE_RUS;
        str=[[NSMutableAttributedString alloc] initWithString:@"Rus" attributes:attrs];
        [buttonLanguage setAttributedTitle:str];
        //((GStr<int>*)inputData.pref)->put(22,language);
        [self dictTranslate:self];
    }else{
        language=TRANSLATE_ENG;
        str=[[NSMutableAttributedString alloc] initWithString:@"Eng" attributes:attrs];
        [buttonLanguage setAttributedTitle:str];
        //((GStr<int>*)inputData.pref)->put(22,language);
        [self dictTranslate:self];
    }
}

- (id)initView {
    cout<<"START VIEW"<<endl;
    
    NSRect windowRect = NSMakeRect(10.0f, 10.0f, 1024.0f, 1024.0f);
    
    window = [[NSWindow alloc]
              initWithContentRect:windowRect
              styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
              backing:NSBackingStoreBuffered defer:NO];
    
    superview = [window contentView];
    gPortView =[[gPort alloc] initWithFrame:windowRect];
    [gPortView setPechaImage];
    [superview addSubview:gPortView];
    //[window setContentView:gPortView];
    [window setReleasedWhenClosed:TRUE];
    
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    NSColor *strColor=[NSColor colorWithCalibratedRed:0.7 green:0.7 blue:0.7 alpha:1];
    //NSCenterTextAlignment  location:0 options:attrs];
    [attrs setObject:strColor forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Apple Color Emoji" size:31] forKey: NSFontAttributeName];
    NSMutableParagraphStyle *st=[[NSParagraphStyle defaultParagraphStyle]mutableCopy];
    //call_once[st setAlignment:2];
    [attrs setObject:st forKey: NSParagraphStyleAttributeName];
    NSMutableAttributedString *str;
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(0, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏪" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(moveLeft:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(64, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏩" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(moveRight:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(128, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏫" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(moveUp:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏬" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(moveDown:)];
    [superview addSubview:button1];
    [button1 release];
    
    [attrs setObject:[NSFont fontWithName:@"Apple Color Emoji" size:21] forKey: NSFontAttributeName];
    
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"🔍➕" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(zoomIn:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"🔍➖" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(zoomOut:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⧈" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(homeImg:)];
    [superview addSubview:button1];
    [button1 release];
    
    pageNumField=[[NSTextField alloc] initWithFrame:NSMakeRect(192, 0, 64, 34)];
    [pageNumField setAction: @selector(OCRLetterByIndex:)];
    [pageNumField setAlignment:NSCenterTextAlignment];
    [pageNumField setFont:[NSFont fontWithName:@"Arial Black" size:15]];
    //[pageNumField setBezeled:NO];
    int index=0;//((GStr<int>*)inputData.pref)->get(23);
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",index]];
    [superview addSubview:pageNumField];
    [pageNumField release];
    
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    [attrs setObject:NSColorFromRGB(0x000000) forKey: NSForegroundColorAttributeName];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192, 0, 64, 34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"OCR letter by index" attributes:attrs];
    [button1 setAttributedTitle:str];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(OCRLetterByIndex:)];
    [superview addSubview:button1];
    [button1 release];
    
    [window makeKeyAndOrderFront:self];
    
    return self;
}

-(id)initTranslation{
    pageNum=0;//((GStr<int>*)inputData.pref)->get(21);
    //((GVector*)inputData.prefVector)->getStr(20, textPath);
    language=0;//((GStr<int>*)inputData.pref)->get(22);
    textPath="/mDo 'dus sogs.txt";
    
    cout<<"open file="<<textPath<<" language="<<language<<endl;
    ((GLogicProcessor*)inputData.logicProcessor)->dictionaryReady=0;
    return [self initDictionary:""];
}

- (id)initDictionary:(string)word{
    cout<<"START Dictionary"<<endl;
    int dl=34;
    NSRect windowRect = NSMakeRect(0.0f, 0.0f, 1024.0f, 1024.0f);
    NSRect viewRect = NSMakeRect(0.0f, 0.0f, 1024.0f, 1024.0f-dl);
    
    window = [[NSWindow alloc]
              initWithContentRect:windowRect
              styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
              backing:NSBackingStoreBuffered defer:NO];
    
    [window setDelegate:(id)self];
    
    superview = [window contentView];
    
    webView =[[WebView alloc] initWithFrame:viewRect];
    
    // Set us up as the delegate of the webView for relevant events.
    [webView setUIDelegate:self];
    [webView setFrameLoadDelegate:self];
    [webView setResourceLoadDelegate:self];
    //[webView setEditingDelegate:self];
    
    [webView setGroupName:@"MyDocument"];
    [superview addSubview:webView];
    //[window setContentView:webView];
    [window setReleasedWhenClosed:TRUE];
    
    logicProcessor=(GLogicProcessor*)inputData.logicProcessor;
    pageNum=0;//((GStr<int>*)inputData.pref)->get(21);
    translationMode=0;//((GStr<int>*)inputData.pref)->get(22);
    if(pageNum<0)pageNum=0;
    if(translationMode<0)translationMode=TRANSLATE_RUS;
    
    if(word==""){
        [self dictTranslate:nil];
    }else{
        //cout<<"word="<<word<<"///"<<endl;
        inputData.data["ocrData"]="";
        inputData.data["db"]="dict";
        inputData.data["text"]=word;
        inputData.data["page"]="";
        if(!logicProcessor->dictionaryReady)logicProcessor->readDictionary();
        logicProcessor->writeDictReportLocal(word);
        //cout<<" report="<<word<<endl;
        string maket;
        readText(maket, "/_Image2OCR/edit/OSBL_Dictionary.html");
        maket=str_replace("@@@TEXT@@@", word, maket);
        writeText(maket,"/_Image2OCR/edit/_report.html");
        //[[webView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"/_Image2OCR/edit/OSBL_Dictionary.html"]]];
        [[webView mainFrame] loadHTMLString:[NSString stringWithUTF8String:maket.c_str()] baseURL:nil];
    }
    
    // Configure webView to let JavaScript talk to this object.
    [[webView windowScriptObject] setValue:self forKey:@"AppController"]; // can be any unique name you want
    /*
     Notes:
     1. In JavaScript, you can now talk to this object using "window.AppController".
     
     2. You must explicitly allow methods to be called from JavaScript;
     See the +isSelectorExcludedFromWebScript: method below for an example.
     
     3. The method on this class which we call from JavaScript is -showMessage:
     To call it from JavaScript, we use window.AppController.showMessage_()  <-- NOTE colon becomes underscore!
     For more on method-name translation, see:
     http://developer.apple.com/documentation/AppleApplications/Conceptual/SafariJSProgTopics/Tasks/ObjCFromJavaScript.html#
     */
    
    
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    //NSColor *strColor=[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1];
    //NSCenterTextAlignment  location:0 options:attrs];
    [attrs setObject:NSColorFromRGB(0xffffffff) forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    NSMutableParagraphStyle *st=[[NSParagraphStyle defaultParagraphStyle]mutableCopy];
    [st setAlignment:NSCenterTextAlignment];
    [attrs setObject:st forKey: NSParagraphStyleAttributeName];
    NSMutableAttributedString *str;
    
    lableFill=[[NSTextField alloc] initWithFrame:NSMakeRect(0,windowRect.size.height-34,windowRect.size.width,34)];
    [lableFill setEditable:NO];
    [lableFill setBezeled:NO];
    [lableFill setBackgroundColor:NSColorFromRGB(0x292929)];
    [superview addSubview:lableFill];
    
    buttonDict = [[NSButton alloc] initWithFrame: NSMakeRect(windowRect.size.width-64,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Dict" attributes:attrs];
    [buttonDict setAttributedTitle:str];
    [[buttonDict cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonDict setTarget: self];
    [buttonDict setAction: @selector(openDict:)];
    [superview addSubview:buttonDict];
    
    buttonLeft = [[NSButton alloc] initWithFrame: NSMakeRect(0,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"<<" attributes:attrs];
    [buttonLeft setAttributedTitle:str];
    [[buttonLeft cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonLeft setTarget: self];
    [buttonLeft setAction: @selector(openLeft:)];
    [superview addSubview:buttonLeft];
    
    pageNumField=[[NSTextField alloc] initWithFrame:NSMakeRect(64,windowRect.size.height-34,64,34)];
    [pageNumField setAction: @selector(openPage:)];
    [pageNumField setAlignment:NSCenterTextAlignment];
    [pageNumField setBackgroundColor:NSColorFromRGB(0x292929)];
    [pageNumField setFont:[NSFont fontWithName:@"Arial Black" size:15]];
    [pageNumField setBezeled:NO];
    [pageNumField setTextColor:NSColorFromRGB(0xffffffff)];
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",pageNum]];
    [superview addSubview:pageNumField];
    
    buttonRight = [[NSButton alloc] initWithFrame: NSMakeRect(128,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@">>" attributes:attrs];
    [buttonRight setAttributedTitle:str];
    [[buttonRight cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonRight setTarget: self];
    [buttonRight setAction: @selector(openRight:)];
    [superview addSubview:buttonRight];
    
    buttonSave = [[NSButton alloc] initWithFrame: NSMakeRect(192,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Save" attributes:attrs];
    [buttonSave setAttributedTitle:str];
    [[buttonSave cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonSave setTarget: self];
    [buttonSave setAction: @selector(dictSave:)];
    [superview addSubview:buttonSave];
    
    buttonLanguage = [[NSButton alloc] initWithFrame: NSMakeRect(256,windowRect.size.height-34,64,34)];
    if(language==TRANSLATE_ENG){
        str=[[NSMutableAttributedString alloc] initWithString:@"Rus" attributes:attrs];
    }else{
        str=[[NSMutableAttributedString alloc] initWithString:@"Eng" attributes:attrs];
    }
    [buttonLanguage setAttributedTitle:str];
    [[buttonLanguage cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonLanguage setTarget: self];
    [buttonLanguage setAction: @selector(setLanguage:)];
    [superview addSubview:buttonLanguage];
    
    buttonTranslate = [[NSButton alloc] initWithFrame: NSMakeRect(windowRect.size.width-128,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Translate" attributes:attrs];
    [buttonTranslate setAttributedTitle:str];
    [[buttonTranslate cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [buttonTranslate setTarget: self];
    [buttonTranslate setAction: @selector(dictTranslate:)];
    [superview addSubview:buttonTranslate];
    
    searchTextField=[[NSTextField alloc] initWithFrame:NSMakeRect(windowRect.size.width-384,windowRect.size.height-34,256,34)];
    [searchTextField setAction: @selector(openDict:)];
    [searchTextField setFont:[NSFont fontWithName:@"YagpoUni" size:15]];
    [superview addSubview:searchTextField];
    
    
    [window makeKeyAndOrderFront:nil];
    return self;
}

-(id)initLetter:(int)index{
    
    cout<<"START VIEW"<<endl;
    
    NSRect windowRect = NSMakeRect(10.0f, 10.0f, 987.0f, 987.0f);
    
    window = [[NSWindow alloc]
              initWithContentRect:windowRect
              styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
              backing:NSBackingStoreBuffered defer:NO];
    
    superview = [window contentView];
    gPortView =[[gPort alloc] initWithFrame:windowRect];
    letterIndex=index;
    [gPortView setLetter:index];
    [superview addSubview:gPortView];
    //[window setContentView:gPortView];
    [window setReleasedWhenClosed:TRUE];
    
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    //NSColor *strColor=[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1];
    //NSCenterTextAlignment  location:0 options:attrs];
    [attrs setObject:NSColorFromRGB(0x00000000) forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    NSMutableParagraphStyle *st=[[NSParagraphStyle defaultParagraphStyle]mutableCopy];
    [st setAlignment:NSCenterTextAlignment];
    [attrs setObject:st forKey: NSParagraphStyleAttributeName];
    NSMutableAttributedString *str;
    
    [attrs setObject:[NSFont fontWithName:@"Apple Color Emoji" size:31] forKey: NSFontAttributeName];

    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(0,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏩" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(lastLetter:)];
    [button1 setToolTip:@"Navigate ot last letter in letters database"];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(64,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"🔳" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(setDrawMask:)];
    [button1 setToolTip:@"Draw letter's features masks"];
    [superview addSubview:button1];
    
    [attrs setObject:[NSFont fontWithName:@"YagpoUni" size:24] forKey: NSFontAttributeName];

    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(128,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"༄" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(setDrawVector:)];
    [button1 setToolTip:@"Draw letter's focal lines"];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(192,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"🔲" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(setDrawNewMask:)];
    [button1 setToolTip:@"Draw letter's new OCR mask feature"];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(256,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"✖️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(removeMask:)];
    [button1 setToolTip:@"Remove last mask from letter"];
    [superview addSubview:button1];
    
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(320,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"💢" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(setLetterBox:)];
    [button1 setToolTip:@"Draw letter's OCR dimentions box"];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(384,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"🎼" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(setLetterLineBox:)];
    [button1 setToolTip:@"Draw letter's string position restangle"];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(448,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"✂️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(eraseInImage:)];
    [button1 setToolTip:@"Erase letter parts by restangle"];
    [superview addSubview:button1];
    
    
    letterName=[[NSTextField alloc] initWithFrame:NSMakeRect(PICT_SIZE*4,windowRect.size.height-192,220,192)];
    [letterName setAction: @selector(openLetter:)];
    [letterName setTarget: self];
    [letterName setAlignment:NSCenterTextAlignment];
    [letterName setFont:[NSFont fontWithName:@"YagpoUni" size:64]];
    GLetter *letter=((GFontEditor*)inputData.fontEditor)->aliKali->getLetter(index);
    NSString *name=[NSString stringWithUTF8String:letter->name.c_str()];
    [letterName setStringValue:name];
    [superview addSubview:letterName];
    
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(PICT_SIZE*4,windowRect.size.height-226,220,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Save letter" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(saveLetter:)];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(PICT_SIZE*4,windowRect.size.height-260,220,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Set OCR focal line in letter" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(setOCRFocalLine:)];
    [superview addSubview:button1];
    [button1 release];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(PICT_SIZE*4,windowRect.size.height-294,220,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Erase all masks" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: gPortView];
    [button1 setAction: @selector(eraseAllMask:)];
    [superview addSubview:button1];
    [button1 release];
    
    [attrs setObject:[NSFont fontWithName:@"Apple Color Emoji" size:31] forKey: NSFontAttributeName];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(PICT_SIZE*4,windowRect.size.height-328,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"◀️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(previousLetter:)];
    [superview addSubview:button1];
    
    
    searchTextField=[[NSTextField alloc] initWithFrame:NSMakeRect(PICT_SIZE*4+64,windowRect.size.height-328,94,34)];
    [searchTextField setAction: @selector(openLetter:)];
    [searchTextField setTarget: self];
    [searchTextField setAlignment:NSCenterTextAlignment];
    [searchTextField setFont:[NSFont fontWithName:@"Arial Black" size:15]];
    [searchTextField setStringValue:[NSString stringWithFormat:@"%d",index]];
    [superview addSubview:searchTextField];
    
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(PICT_SIZE*4+158,windowRect.size.height-328,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"▶️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(nextLetter:)];
    [superview addSubview:button1];

    
    [window makeKeyAndOrderFront:self];
    
    return self;
    
}

-(id)initLetterBase{
    string path="db=letter&page=0&port=gport";
    [self startLetterBase:path];
    return self;
    
}

- (id)init {
    //cout<<"START WEB"<<endl;
    
    NSRect windowRect = NSMakeRect(10.0f, 10.0f, 1024.0f, 1024.0f);
    
    window = [[NSWindow alloc]
              initWithContentRect:windowRect
              styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
              backing:NSBackingStoreBuffered defer:NO];
    
    [window setDelegate:(id)self];
    
    superview = [window contentView];
    
    webView =[[WebView alloc] initWithFrame:windowRect];
    
    // Set us up as the delegate of the webView for relevant events.
    [webView setUIDelegate:self];
    [webView setFrameLoadDelegate:self];
    [webView setResourceLoadDelegate:self];
    [[webView windowScriptObject] setValue:self forKey:@"AppController"];
    [superview addSubview:webView];
    [window setContentView:webView];
    //[window setReleasedWhenClosed:TRUE];
    //((GVector*)inputData.prefVector)->getStr(20, textPath);
    if(textPath.size()==0){
        [[webView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"/_Image2OCR/edit/OSBL.html"]]];
    }else{
        string pathText;
        //((GVector*)inputData.prefVector)->getStr(20,pathText);
        string path="/_Image2OCR/edit/OSBL.html";
        string html;
        readText(html,path);
        pathText="name=\"fileList\">"+pathText+"<";
        html=str_replace("name=\"fileList\"><",pathText,html);
        [[webView mainFrame] loadHTMLString:[NSString stringWithUTF8String:html.c_str()] baseURL:
         [NSURL URLWithString:@"/_Image2OCR/edit/OSBL.html"]];
        
    }
    //[[webView mainFrame] loadHTMLString:@"<html><head></head><body><h1>Hello</h1></body></html>" baseURL:nil];
    [window makeKeyAndOrderFront:nil];
    
    return self;
}
-(void)startLetterBase:(string)path{
    //cout<<"start port";
    NSRect windowRect = NSMakeRect(10.0f, 10.0f, 1490.0f, 1024.0f);
    
    window = [[NSWindow alloc]
              initWithContentRect:windowRect
              styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
              backing:NSBackingStoreBuffered defer:NO];
    
    superview = [window contentView];
    gPortView =[[gPort alloc] initWithFrame:windowRect];
    [superview addSubview:gPortView];
    
    [gPortView setLetterBase:path];
    
    
    //[window setContentView:gPortView];
    [window setReleasedWhenClosed:TRUE];
    NSMutableDictionary *attrs = [NSMutableDictionary dictionary];
    //NSColor *strColor=[NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:1];
    //NSCenterTextAlignment  location:0 options:attrs];
    [attrs setObject:NSColorFromRGB(0x00000000) forKey: NSForegroundColorAttributeName];
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    NSMutableParagraphStyle *st=[[NSParagraphStyle defaultParagraphStyle]mutableCopy];
    [st setAlignment:NSCenterTextAlignment];
    [attrs setObject:st forKey: NSParagraphStyleAttributeName];
    NSMutableAttributedString *str;
    
    [attrs setObject:[NSFont fontWithName:@"Apple Color Emoji" size:31] forKey: NSFontAttributeName];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(220,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"◀️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(previousLetterBasePage:)];
    [superview addSubview:button1];
    
    
    pageNumField=[[NSTextField alloc] initWithFrame:NSMakeRect(284,windowRect.size.height-34,92,34)];
    [pageNumField setAction: @selector(openLetterBasePage:)];
    [pageNumField setTarget: self];
    [pageNumField setAlignment:NSCenterTextAlignment];
    [pageNumField setFont:[NSFont fontWithName:@"Arial Black" size:15]];
    pageNum=0;//((GStr<int>*)inputData.pref)->get(24);
    [pageNumField setStringValue:[NSString stringWithFormat:@"%d",pageNum]];
    [superview addSubview:pageNumField];
    
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(376,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"▶️" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(nextLetterBasePage:)];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(440,windowRect.size.height-34,64,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"⏩" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(lastLetterBasePage:)];
    [superview addSubview:button1];
    
    letterName=[[NSTextField alloc] initWithFrame:NSMakeRect(0,windowRect.size.height-192,220,192)];
    [letterName setAction: @selector(openLetter:)];
    [letterName setTarget: self];
    [letterName setAlignment:NSCenterTextAlignment];
    [letterName setFont:[NSFont fontWithName:@"YagpoUni" size:64]];
    [letterName setStringValue:@""];
    [superview addSubview:letterName];
    
    [attrs setObject:[NSFont fontWithName:@"Arial" size:14] forKey: NSFontAttributeName];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(0,windowRect.size.height-226,220,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"View letter by name" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(openLettersByName:)];
    [superview addSubview:button1];
    
    button1 = [[NSButton alloc] initWithFrame: NSMakeRect(0,windowRect.size.height-260,220,34)];
    str=[[NSMutableAttributedString alloc] initWithString:@"Export all OCR letters in XML" attributes:attrs];
    [button1 setAttributedTitle:str];
    //[[button1 cell] setBackgroundColor:NSColorFromRGB(0x373737)];
    [button1 setTarget: self];
    [button1 setAction: @selector(exportLetterBase:)];
    [superview addSubview:button1];
    [button1 release];
    
    [window makeKeyAndOrderFront:self];
    
}

- (NSString *)alertInput: (NSString *)prompt
            :(NSString *)dValue
            :(NSString *)infoValue{
    NSAlert *alert = [NSAlert alertWithMessageText: prompt
                                     defaultButton:@"OK"
                                   alternateButton:@"Cancel"
                                       otherButton:nil
                         informativeTextWithFormat:infoValue];
    
    NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 600, 34)];
    [input setStringValue:dValue];
    [input autorelease];
    [alert setAccessoryView:input];
    NSInteger button = [alert runModal];
    if (button == NSAlertDefaultReturn) {
        [input validateEditing];
        return [input stringValue];
    } else if (button == NSAlertAlternateReturn) {
        return nil;
    } else {
        //NSAssert1(NO, @"Invalid input dialog button %d", button);
        return nil;
    }
}

@end
