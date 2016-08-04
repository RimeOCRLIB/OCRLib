//
//  AppDelegate.h
//  OCRLib
//
//  Created by rimeocr on 11/1/12.
//  Copyright (c) 2012 www.buddism.ru. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import "SpecialProtocol.h"
#import "TaskWrapper.h"
#import "gWindow.h"
#include "config.h"

@interface MyDelegate : NSObject
- (void) applicationDidFinishLaunching : (NSNotification *) aNotification;
@end

@interface OCRLibAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {

    TaskWrapper *unixTask;
    int targetBlank;                 //new window open flag
}

//@property (assign) IBOutlet NSWindow *OCRConsole;
//@property (assign) IBOutlet NSWindow *window;
-(IBAction)openConsole:(id)sender;
-(IBAction)openMain:(id)sender;
-(IBAction)openPort:(id)sender;
-(IBAction)openDictionary:(id)sender;
-(IBAction)openLetterBase:(id)sender;
-(IBAction)openLetterView:(id)sender;
-(IBAction)rebuildFocalLinesInBase:(id)sender;
-(IBAction)setOCRLanguage:(id)sender;
//-(IBAction)backPage:(id)sender;
-(IBAction)closeWindow:(id)sender;
-(IBAction)openFileForOCR:(id)sender;
-(IBAction)openFileForTranslation:(id)sender;

@end

#define alert(x)      NSAlert *alert = [[NSAlert alloc] init];\
[alert addButtonWithTitle:@"OK"];\
[alert setMessageText:x];\
[alert runModal];\
[alert release];\


