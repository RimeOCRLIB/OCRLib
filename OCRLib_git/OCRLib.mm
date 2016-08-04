    //
//  AppDelegate.m
//  OCRLib
//
//  Created by rimeocr on 11/1/12.
//  www.buddhism.ru may all be happy
//

#import "OCRLib.h"
#include "OCRTypes.h"
#include "php2stl.h"
#include "unzip.h"
#import "GPDF.h"
#include <string>

using namespace std;



@implementation MyDelegate
- (void) applicationDidFinishLaunching : (NSNotification *) aNotification;
{
    // TODO - Create the menu here.
}
@end


@implementation OCRLibAppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void) applicationDidFinishLaunching : (NSNotification *) aNotification;
{
    // TODO - Create the menu here.
    NSLog(@"knock-knock");
    //test PDF image extraction
    /*GPDF *pdf=GPDF::create("/__TEST/3.pdf");
     pdf->writeAllImages();
     pdf->destroy();
     exit(0);
     */
    
    //[[gWindow alloc]init];
    [[gWindow alloc] initView];
    
    // Create the main menu that will house each real menu
    // File, Edit, etc.
    id menubar = [[NSMenu new] autorelease];               //новое основное меню
    id appMenuItem = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItem];                         //добавляем первый элемент
    
    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    id openFile = [[[NSMenuItem alloc] initWithTitle:@"Open file"
                                              action:@selector(openConsole:) keyEquivalent:@"o"] autorelease];
    id closeWindow = [[[NSMenuItem alloc] initWithTitle:@"Close window"
                                                 action:@selector(closeWindow:) keyEquivalent:@"w"] autorelease];
    unichar arrowKey = NSLeftArrowFunctionKey;
    id previousPage = [[[NSMenuItem alloc] initWithTitle:@"Rrevious page"
                                                  action:@selector(backPage:) keyEquivalent:
                        [NSString stringWithCharacters:&arrowKey length:1]] autorelease];
    
    
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                                  action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
    
    targetBlank=0;
    
    [appMenu addItem:openFile];
    [appMenu addItem:previousPage];
    [appMenu addItem:closeWindow];
    [appMenu addItem:quitMenuItem];
    
    [appMenuItem setSubmenu:appMenu];
    
    
    [NSApp setMainMenu:menubar];
    
    // Create the file menu.

}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
    string path=[filename UTF8String];
    if(path.find(".zip")!=-1){
        //test zip
        chdir("/__TEST/");
        char *a[4];
        char report[1024*1024];
        memset(report,0,1024*1024);
        
        a[1]=(char *)"-l";
        a[2]=(char *)path.c_str();
        a[3]=report;
        string rezult; unzip(3,a);
        rezult="Zip path="+path+"\n"+report;
        rezult=str_replace("\n","\n<br>",rezult);
        //exit(0);
        NSString *str=[NSString stringWithUTF8String:rezult.c_str()];
        gWindow *g=[[gWindow alloc] initView];
        //%%%display string
        
    }
    if(path.find(".pdf")!=-1){
        GPDF *pdf=GPDF::create(path.c_str());
        pdf->writeAllImages();
        pdf->destroy();
    }
    
    //alert(str);
    //[self init];
    // This gets called when the user double-clicks a project file in the Finder.
    //[self openProject:filename];
    //[self setupProject:filename];
    return YES;
}

- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames
{
    // This gets called when the user opens multiple files from the Finder.
    if( [filenames count] > 0 ){
        for(int i=0;i<[filenames count];i++){
            [self application:sender openFile:[filenames objectAtIndex:i]];
        }
    
    }
}



- (id)init {
    
    return self;
}

- (BOOL)windowShouldClose:(id)sender{
    NSLog(@"window close");
return 1;
}


/////////////////////_____Console_______/////////////////////
-(IBAction)openConsole:(id)sender {
     /*
      cout<<"@console"<<endl;
     NSRect frame1 = NSMakeRect(0, 0, 500, 681);
     WebView *OCRWebView_ =[[WebView alloc] initWithFrame:frame1];
     NSRect windowRect = NSMakeRect(10.0f, 10.0f, 800.0f, 600.0f);
     NSWindow *OCRConsole_ = [[NSWindow alloc]
     initWithContentRect:windowRect
     styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask)
     backing:NSBackingStoreBuffered defer:NO];
     NSView *superview1 = [OCRConsole contentView];
     OCRWebView_ =[[WebView alloc] initWithFrame:frame1];
     //OCRWebView_ =[[WebView alloc] initWithFrame:frame1];
     [superview1 addSubview:OCRWebView_];
     [OCRConsole_ setContentView:OCRWebView_];
     
     [OCRWebView_ setUIDelegate:self];
     [OCRWebView_ setFrameLoadDelegate:self];
     [OCRWebView_ setResourceLoadDelegate:self];
     [OCRWebView_ setGroupName:@"MyDocument"];
    
     activeWebView=OCRWebView_;
     
     //if(! [OCRConsole isVisible] )
     [OCRConsole_ makeKeyAndOrderFront:sender];
    //if(! [OCRConsole isVisible] )
    //    [OCRConsole makeKeyAndOrderFront:sender];
    */
}
- (IBAction)closeWindow:(id)sender
{
    NSLog(@"closeWindow %@",[NSApp orderedWindows]);
    //[window performClose:(id)window];
    [[[NSApp orderedWindows] objectAtIndex:0] performClose:(id)[[NSApp orderedWindows] objectAtIndex:0]];    
}

- (void)openTerminal:(NSString *)argument
{
    NSString *s = [NSString stringWithFormat:
                   @"tell application \"Terminal\" to do script \"%@\"",argument];
    
    NSAppleScript *as = [[NSAppleScript alloc] initWithSource: s];
    [as executeAndReturnError:nil];

}


// This action start a unix task if we aren't already perform one,
// or stops the current task if one is already running
- (void)start:(id)sender{
    
    // Let's allocate memory for and initialize a new TaskWrapper object, passing
    // in ourselves as the controller for this TaskWrapper object, the path
    // to the command-line tool, and the contents of the text field that
    // displays what the user wants to search on
    //unixTask=[[TaskWrapper alloc] initWithController:self arguments:[NSArray arrayWithObjects:@"/cgi/yagpoOCR.cgi",nil]];
    // kick off the process asynchronously
    //[unixTask startProcess];
}

// This callback is implemented as part of conforming to the ProcessController protocol.
// It will be called whenever there is output from the TaskWrapper.
- (void)appendOutput:(NSString *)output
{
    // add the string (a chunk of the results from locate) to the NSTextView's
    // backing store, in the form of an attributed string
    //[[consoleTextView textStorage] appendAttributedString: [[[NSAttributedString alloc]
                                                             //initWithString: output] autorelease]];
    // setup a selector to be called the next time through the event loop to scroll
    // the view to the just pasted text.  We don't want to scroll right now,
    // because of a bug in Mac OS X version 10.1 that causes scrolling in the context
    // of a text storage update to starve the app of events
    [self performSelector:@selector(scrollToVisible:) withObject:nil afterDelay:0.0];
}

// This routine is called after adding new results to the text view's backing store.
// We now need to scroll the NSScrollView in which the NSTextView sits to the part
// that we just added at the end
- (void)scrollToVisible:(id)ignore {
    //[consoleTextView scrollRangeToVisible:NSMakeRange([[consoleTextView string] length], 0)];
}

@end
