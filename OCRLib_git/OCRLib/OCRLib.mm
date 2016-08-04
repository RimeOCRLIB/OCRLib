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
    
    
    int flag=((GStr<int>*)inputData.pref)->get(20);
    //flag=1;
    
    if(flag==0){
        [[gWindow alloc]init];
    }else if(flag==1){
        [[[gWindow alloc]initView]autorelease];
    }else if(flag==2){
        ((GLogicProcessor*)inputData.logicProcessor)->dictionaryReady=0;
        [[gWindow alloc]initTranslation];
    }else if(flag==3){
        [[gWindow alloc]initLetterBase];
    }


    
    
    // Create the main menu that will house each real menu
    // File, Edit, etc.
    id menubar = [[NSMenu new] autorelease];               //новое основное меню
    NSMenuItem *appMenuItem = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItem];                         //добавляем первый элемент
    
    NSMenuItem *appMenuItemFile = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItemFile];                         //добавляем первый элемент
    
    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    id closeWindow = [[[NSMenuItem alloc] initWithTitle:@"Close window"
                                                 action:@selector(closeWindow:) keyEquivalent:@"w"] autorelease];
    //unichar arrowKey = NSLeftArrowFunctionKey;
    //id previousPage = [[[NSMenuItem alloc] initWithTitle:@"Rrevious page"
    //                                              action:@selector(backPage:) keyEquivalent:
    //                    [NSString stringWithCharacters:&arrowKey length:1]] autorelease];
    
    
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                                  action:@selector(terminate:) keyEquivalent:@"q"] autorelease];

    targetBlank=0;
    //[appMenu addItem:previousPage];
    [appMenu addItem:closeWindow];
    [appMenu addItem:quitMenuItem];
    
    [appMenuItem setSubmenu:appMenu];
    
    id appMenuFile = [[NSMenu new] autorelease];
    id openFileOCR = [[[NSMenuItem alloc]
                               initWithTitle:@"Open file for OCR"
                               action:@selector(openFileForOCR:)
                               keyEquivalent:@"o"] autorelease];
    id openFileTranslation = [[[NSMenuItem alloc]
                               initWithTitle:@"Open file for translation"
                               action:@selector(openFileForTranslation:)
                               keyEquivalent:@"o"] autorelease];
    id saveDict = [[[NSMenuItem alloc] initWithTitle:@"Save words in dictionary"
                                              action:@selector(dictSave:) keyEquivalent:@"s"] autorelease];
    [appMenuFile addItem:openFileOCR];
    [appMenuFile addItem:openFileTranslation];
    [appMenuFile addItem:saveDict];
    [appMenuItemFile setSubmenu:appMenuFile];
    
    ///********
    NSMenuItem *appMenuItemEidt = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItemEidt];                         //добавляем первый элемент

    id appMenuEdit = [[NSMenu new] autorelease];
    id menuCut = [[[NSMenuItem alloc] initWithTitle:@"Cut"
                                             action:@selector(copy:) keyEquivalent:@"x"] autorelease];
    id menuCopy = [[[NSMenuItem alloc] initWithTitle:@"Copy"
                                              action:@selector(copy:) keyEquivalent:@"c"] autorelease];
    id menuPaste = [[[NSMenuItem alloc] initWithTitle:@"Paste"
                                              action:@selector(paste:) keyEquivalent:@"v"] autorelease];
    id menuSelect = [[[NSMenuItem alloc] initWithTitle:@"Select All"
                                               action:@selector(selectAll:) keyEquivalent:@"a"] autorelease];
    id menuDict = [[[NSMenuItem alloc] initWithTitle:@"Open dictionary"
                                                action:@selector(openDict:) keyEquivalent:@"d"] autorelease];
    id menuExport = [[[NSMenuItem alloc] initWithTitle:@"Export translation dictionary"
                                                action:@selector(exportDictionary:) keyEquivalent:@"e"] autorelease];
    id menuJS = [[[NSMenuItem alloc] initWithTitle:@"Run JS script"
                                                action:@selector(runJS:) keyEquivalent:@"j"] autorelease];

    
    [appMenuEdit addItem:menuCut];    
    [appMenuEdit addItem:menuCopy];
    [appMenuEdit addItem:menuPaste];
    [appMenuEdit addItem:menuSelect];
    [appMenuEdit addItem:menuDict];
    [appMenuEdit addItem:menuExport];
    [appMenuEdit addItem:menuJS];
    
    [appMenuItemEidt setSubmenu:appMenuEdit];
    
    ///********
    NSMenuItem *appMenuItemView = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItemView];                         //добавляем первый элемент

    id appMenuView = [[NSMenu new] autorelease];
    id openPort = [[[NSMenuItem alloc] initWithTitle:@"Open OCR vector view"
                                              action:@selector(openPort:) keyEquivalent:@"p"] autorelease];
    id openMain = [[[NSMenuItem alloc] initWithTitle:@"Open main view"
                                              action:@selector(openMain:) keyEquivalent:@"m"] autorelease];
    id openDictionary = [[[NSMenuItem alloc] initWithTitle:@"Open Dictionary"
                                              action:@selector(openDictionary:) keyEquivalent:@"d"] autorelease];
    id openLetterBase = [[[NSMenuItem alloc] initWithTitle:@"Open OCR letters base"
                                                    action:@selector(openLetterBase:) keyEquivalent:@"b"] autorelease];
    id openLetter = [[[NSMenuItem alloc] initWithTitle:@"Open letter"
                                                   action:@selector(openLetterView:) keyEquivalent:@"l"] autorelease];



    [appMenuView addItem:openMain];
    [appMenuView addItem:openPort];
    [appMenuView addItem:openDictionary];
    [appMenuView addItem:openLetterBase];
    [appMenuView addItem:openLetter];

    [appMenuItemView setSubmenu:appMenuView];
    ///********
    NSMenuItem *appMenuItemOCR = [[NSMenuItem new] autorelease];       //appMenuItem первый элемент основного меню
    [menubar addItem:appMenuItemOCR];                         //добавляем первый элемент
    
    id appMenuOCR = [[NSMenu new] autorelease];
    id rebuildFocalLinesInBase = [[[NSMenuItem alloc] initWithTitle:@"Rebuild focal lines in letter base"
                                              action:@selector(rebuildFocalLinesInBase:) keyEquivalent:@""] autorelease];
    id setOCRLanguage = [[[NSMenuItem alloc] initWithTitle:@"Set OCR language"
                                                             action:@selector(setOCRLanguage:) keyEquivalent:@""] autorelease];
    
    [appMenuOCR addItem:rebuildFocalLinesInBase];
    [appMenuOCR addItem:setOCRLanguage];
    
    [appMenuItemOCR setSubmenu:appMenuOCR];
    ///********

    [NSApp setMainMenu:menubar];
    [[[[NSApp mainMenu] itemAtIndex:1] submenu] setTitle:@"File"];
    [[[[NSApp mainMenu] itemAtIndex:2] submenu] setTitle:@"Edit"];
    [[[[NSApp mainMenu] itemAtIndex:3] submenu] setTitle:@"View"];
    [[[[NSApp mainMenu] itemAtIndex:4] submenu] setTitle:@"OCR Functions"];
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
        
        [self openConsole:self];
        //[[activeWebView mainFrame] loadHTMLString:str baseURL:[NSURL fileURLWithPath:@"1.html"]];
    }
    if(path.find(".pdf")!=-1){
        string dirpath=substr(0,(int)path.size()-4,path)+"_tif";
        vector<string>v;
        readDirectoryToArray(v,dirpath,"img");
        //if(v.size()>4)return YES;
        
        int pid = fork();
        if (pid < 0){
            cout<<"ERROR on fork ";
            return 0;
        }
        if (pid == 0)  {
            GPDF *pdf=GPDF::create(path.c_str());
            pdf->writeAllImages();
            pdf->destroy();
            exit(0);
        }
        
        
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

-(IBAction)openMain:(id)sender{
    
    [[gWindow alloc]init];
    ((GStr<int>*)inputData.pref)->put(20,0);
}

-(IBAction)openPort:(id)sender{

    [[[gWindow alloc]initView]autorelease];
    ((GStr<int>*)inputData.pref)->put(20,1);
    
}

-(IBAction)openDictionary:(id)sender{
    [[gWindow alloc]initTranslation];
    ((GStr<int>*)inputData.pref)->put(20,2);
}

-(IBAction)openLetterBase:(id)sender{
    [[gWindow alloc]initLetterBase];
    ((GStr<int>*)inputData.pref)->put(20,3);
}

-(IBAction)openLetterView:(id)sender{
    int index=((GStr<int>*)inputData.pref)->get(23);
    [[gWindow alloc]initLetter:index];
    //((GStr<int>*)inputData.pref)->put(20,4);
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

-(IBAction)rebuildFocalLinesInBase:(id)sender{
    GFont *aliKali=((GFontEditor*)inputData.fontEditor)->aliKali;  //load font pointer from fontEditor
    aliKali->setFocalLine();
}

-(IBAction)setOCRLanguage:(id)sender{
    
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"Cancel"];
    [alert addButtonWithTitle:@"Tibetan-Russian OCR"];
    [alert addButtonWithTitle:@"Tibetan-English OCR"];
    [alert addButtonWithTitle:@"Tibetan-Sanskrit OCR"];
    [alert addButtonWithTitle:@"Tibetan OCR"];
    
    [alert setMessageText:@"Choose OCR language"];
    NSModalResponse responseTag = [alert runModal];
    [alert release];
    cout<<"responseTag="<<responseTag<<endl;

    if (responseTag==1001) {
        inputData.data["ocrLn"]="ocrTibetanRussian";
    }else if(responseTag==1002){
        inputData.data["ocrLn"]="ocrTibetanEnglish";
    }else if(responseTag==1003){
            inputData.data["ocrLn"]="ocrTibetanSanskrit";
    }else if(responseTag==1004){
        inputData.data["ocrLn"]="ocrTibetan";
    }
    if(responseTag>1000)((GFontEditor*)inputData.fontEditor)->setLanguage();
}

-(IBAction)openFileForOCR:(id)sender{
    //NSArray *fileTypes = [NSArray arrayWithObjects:@"txt",@"html",@"tab",nil];
    NSOpenPanel * panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:YES];
    [panel setCanChooseDirectories:YES];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:YES];
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton) {
            
            for (NSURL *fileURL in [panel URLs]) {
                NSString *str=[fileURL absoluteString];
                cout<<"fileURL="<<[str UTF8String]<<endl;
                string filePath=[str UTF8String];
                filePath=str_replace("file://", "", filePath);
                ((GVector*)inputData.prefVector)->putStr(20,filePath);
                break;
                
            }
            ((GStr<int>*)inputData.pref)->put(20,0);
            [[gWindow alloc]init];
        }
    }];
}

-(IBAction)openFileForTranslation:(id)sender{
    //NSArray *fileTypes = [NSArray arrayWithObjects:@"txt",@"html",@"tab",nil];
    NSOpenPanel * panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:YES];
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton) {
            for (NSURL *fileURL in [panel URLs]) {
                NSString *str=[fileURL absoluteString];
                cout<<"fileURL="<<[str UTF8String]<<endl;
                string textPath=[str UTF8String];
                textPath=str_replace("file://", "", textPath);
                ((GVector*)inputData.prefVector)->putStr(20,textPath);
                break;
            }
            ((GLogicProcessor*)inputData.logicProcessor)->dictionaryReady=0;
            [[gWindow alloc]initTranslation];
        }
    }];
}


@end
