//
//  realPDF.h
//  OCRLib
//  Created by rimeocr on 11/6/12.
//

#ifdef Cocoa

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

void CGImageWriteToFile(CGImageRef image, NSString *path) {
    CFURLRef url = (CFURLRef)[NSURL fileURLWithPath:path];
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypeJPEG, 1, NULL);
    CGImageDestinationAddImage(destination, image, nil);
    
    bool success = CGImageDestinationFinalize(destination);
    if (!success) {
        NSLog(@"Failed to write image to %@", path);
    }
    
    CFRelease(destination);
}

void MyDisplayPDFPage (CGContextRef myContext,
                       size_t pageNumber,
                       const char *filename)
{
    CGPDFDocumentRef document;
    CGPDFPageRef page;
    
    document = MyGetPDFDocumentRef (filename);// 1
    page = CGPDFDocumentGetPage (document, pageNumber);// 2
    CGContextDrawPDFPage (myContext, page);// 3
    CGPDFDocumentRelease (document);// 4
}

void MyDrawPDFPageInRect (CGContextRef context,
                          CGPDFPageRef page,
                          CGPDFBox box,
                          CGRect rect,
                          int rotation,
                          bool preserveAspectRatio)
{
    CGAffineTransform m;
    
    m = CGPDFPageGetDrawingTransform (page, box, rect, rotation, preserveAspectRatio); // 1
    CGContextSaveGState (context);// 2
    CGContextConcatCTM (context, m);// 3
    CGContextClipToRect (context,CGPDFPageGetBoxRect (page, box));// 4
    CGContextDrawPDFPage (context, page);// 5
    CGContextRestoreGState (context);// 6
}
/*Here’s what the code does:
 Creates an affine transform from the parameters supplied to the function.
 Saves the graphics state.
 Concatenates the CTM with the affine transform.
 Clips the graphics context to the rectangle specified by the box parameter. The function CGPDFPageGetBoxRect obtains the page bounding box (media, crop, bleed, trim, and art boxes) associated with the constant you supply—kCGPDFMediaBox, kCGPDFCropBox, kCGPDFBleedBox, kCGPDFTrimBox, or kCGPDFArtBox.
 Draws the PDF page to the transformed and clipped context.
 Restores the graphics state.*/

/* It’s as easy to create a PDF file using Quartz 2D as it is to draw to any graphics context. You specify a location for a PDF file, set up a PDF graphics context, and use the same drawing routine you’d use for any graphics context. The function MyCreatePDFFile, shown in Listing 13-4, shows all the tasks your code performs to create a PDF file. A detailed explanation for each numbered line of code appears following the listing.
 
 Note that the code delineates PDF pages by calling the functions CGPDFContextBeginPage and CGPDFContextEndPage. You can pass a CFDictionary object to specify page properties including the media, crop, bleed, trim, and art boxes. For a list of dictionary key constants and a more detailed description of each, see CGPDFContext Reference.
 */
void MyCreatePDFFile (CGRect pageRect, const char *filename)// 1
{
    CGContextRef pdfContext;
    CFStringRef path;
    CFURLRef url;
    CFDataRef boxData = NULL;
    CFMutableDictionaryRef myDictionary = NULL;
    CFMutableDictionaryRef pageDictionary = NULL;
    
    path = CFStringCreateWithCString (NULL, filename, // 2
                                      kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path, // 3
                                         kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    myDictionary = CFDictionaryCreateMutable(NULL, 0,
                                             &kCFTypeDictionaryKeyCallBacks,
                                             &kCFTypeDictionaryValueCallBacks); // 4
    CFDictionarySetValue(myDictionary, kCGPDFContextTitle, CFSTR("My PDF File"));
    CFDictionarySetValue(myDictionary, kCGPDFContextCreator, CFSTR("My Name"));
    pdfContext = CGPDFContextCreateWithURL (url, &pageRect, myDictionary); // 5
    CFRelease(myDictionary);
    CFRelease(url);
    pageDictionary = CFDictionaryCreateMutable(NULL, 0,
                                               &kCFTypeDictionaryKeyCallBacks,
                                               &kCFTypeDictionaryValueCallBacks); // 6
    boxData = CFDataCreate(NULL,(const UInt8 *)&pageRect, sizeof (CGRect));
    CFDictionarySetValue(pageDictionary, kCGPDFContextMediaBox, boxData);
    CGPDFContextBeginPage (pdfContext, &pageRect); // 7
    //###    myDrawContent (pdfContext);// 8
    CGPDFContextEndPage (pdfContext);// 9
    CGContextRelease (pdfContext);// 10
    CFRelease(pageDictionary); // 11
    CFRelease(boxData);
}
/* Here’s what the code does:
 Takes as parameters a rectangle that specifies the size of the PDF page and a string that specifies the filename.
 Creates a CFString object from a filename passed to the function MyCreatePDFFile.
 Creates a CFURL object from the CFString object.
 Creates an empty CFDictionary object to hold metadata. The next two lines add a title and creator. You can add as many key-value pairs as you’d like using the function CFDictionarySetValue. For more information on creating dictionaries, see CFDictionary Reference.
 Creates a PDF graphics context, passing three parameters:
 A CFURL object that specifies a location for the PDF data.
 A pointer to a rectangle that defines the default size and location of the PDF page. The origin of the rectangle is typically (0, 0). Quartz uses this rectangle as the default bounds of the page media box. If you pass NULL, Quartz uses a default page size of 8.5 by 11 inches (612 by 792 points).
 A CFDictionary object that contains PDF metadata. Pass NULL if you don’t have metadata to add.
 You can use the CFDictionary object to specify output intent options—intent subtype, condition, condition identifier, registry name, destination output profile, and a human-readable text string that contains additional information or comments about the intended target device or production condition. For more information about output intent options, see CGPDFContext Reference.
 
 Creates a CFDictionary object to hold the page boxes for the PDF page. This example sets the media box.
 Signals the start of a page. When you use a graphics context that supports multiple pages (such as PDF), you call the function CGPDFContextBeginPage together with CGPDFContextEndPage to delineate the page boundaries in the output. Each page must be bracketed by calls to CGPDFContextBeginPage and CGPDFContextEndPage. Quartz ignores all drawing operations performed outside a page boundary in a page-based context.
 Calls an application-defined function to draw content to the PDF context. You supply your drawing routine here.
 Signals the end of a page in a page-based graphics context.
 Releases the PDF context.
 Releases the page dictionary.*/

void MakePDF(){
    // lets try to make some pdf
    NSImage * theImage;
    NSString *outputFile;  // user selected file name
    NSString * theCommand = @"/System/Library/Printers/Libraries/quartzfilter";
    NSString * theFilter = [NSHomeDirectory() stringByAppendingString:@"/Library/Filters/ImageBrowser.qfilter"];
    
    NSString *tmpFile = [NSTemporaryDirectory() stringByAppendingString:
                         [[NSProcessInfo processInfo] globallyUniqueString]];
    //NSLog(@"tmpName: %@",tmpName);
    
    // get the filename for output
    NSSavePanel * panel= [NSSavePanel savePanel];
    [panel setRequiredFileType:@"pdf"];
    if( [panel runModal] ==NSFileHandlingPanelOKButton){
        outputFile =  [NSString stringWithString:[panel filename]]; //need to use "files"?
    }else return;
    
    NSMutableArray *args = [NSMutableArray array];
    [args addObject:tmpFile];
    [args addObject:theFilter];
    [args addObject:outputFile];
    
    NSBundle *thisBundle = [NSBundle bundleForClass:[self class]];
    NSString * emptypdf = [thisBundle pathForResource:@"Empty.pdf"
                                               ofType:nil] ;
    PDFDocument *myBook = [[PDFDocument alloc] initWithData:[NSData
                                                             dataWithContentsOfFile:emptypdf]];
    [myBook removePageAtIndex:0];
    
    NSArray *fileNameList; ///######  OpenPanel
    NSArray *filesLeft; ///######
    //Load the progress counter
    int cnt= [fileNameList count];
    //if(!thePanel) {
    //    [NSBundle loadNibNamed: @"FileCount" owner:self];
    //}
    //[thePanel orderFront:self];
    //[progressInd setUsesThreadedAnimation:YES];
    //[progressInd startAnimation:self];
    int fcnt = [fileNameList count];
    int i;
    for(i=0;i< cnt;i++) {
        theImage =[ [NSImage alloc] initWithContentsOfFile: [fileNameList
                                                             objectAtIndex:i]];
        NSImageView    *pdfView = [[NSImageView alloc]
                                   initWithFrame:NSMakeRect(0,0,
                                                            [theImage size].width,
                                                            [theImage size].height)];
        
        [pdfView setImage:theImage];
        NSData    *pdfData;
        pdfData = [pdfView dataWithPDFInsideRect:[pdfView bounds]];
        PDFDocument * tmpPage = [[PDFDocument alloc] initWithData: pdfData];
        [myBook insertPage:[ tmpPage pageAtIndex: 0] atIndex: i];
        [pdfView release];
        [tmpPage release];
        [theImage release];
        [filesLeft setIntValue: --fcnt];
        [filesLeft displayIfNeeded];
    }
    //[progressInd stopAnimation:self];
    //[thePanel orderOut:self];    // hide the panel
    
    //if(!theSavePanel) [NSBundle loadNibNamed:@"writePDF" owner: self];
    //[theSavePanel orderFront:self];
    //[saveInd setUsesThreadedAnimation:YES];
    //[saveInd startAnimation:self];
    
    BOOL writeOK;
    /* This won't work in the Universal SDK
     NSArray *filters = [QuartzFilterManager filtersInDomains:[NSArray
     arrayWithObject:@"QuartzFilterPDFWorkflowDomain"]];
     int search =  [self findSpecialFilter: filters byName:@"ImageBrowser"];
     if ( search != -1 ) {
     QuartzFilter *f = [filters objectAtIndex:search];
     NSDictionary * optionsDictWithQuartzFilter = [NSDictionary
     dictionaryWithObject: f forKey:@"QuartzFilter"];
     writeOK=[myBook writeToFile:outputFile
     withOptions:optionsDictWithQuartzFilter  ];
     }
     else {
     writeOK=[myBook writeToFile:outputFile];
     }
     */
    writeOK=[myBook writeToFile:tmpFile];
    // run special filter to compress jpgs and lighten images
    [self runCommand: theCommand withArguments:args];
    NSFileManager *manager = [NSFileManager defaultManager];
    if ([manager fileExistsAtPath:tmpFile ])
        [manager removeFileAtPath:tmpFile handler:nil];
    
    //[saveInd stopAnimation:self];
    //[theSavePanel orderOut:self];
    
    if(writeOK)
        NSRunInformationalAlertPanel(@"Write Complete.",outputFile,@"OK",nil,nil);
    else
        NSRunInformationalAlertPanel(@"Write Failed.",outputFile,@"OK",nil,nil);
    
    [myBook release];
}

void getImageFromPDF(CGPDFObjectRef obj,const char * key){
    int colorMode=1;
    CGPDFStreamRef strm;
    if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeStream, &strm
                            ) ){
        cout_<<"+"<<endl;
    }
    CGPDFDictionaryRef strmdict = CGPDFStreamGetDictionary( strm );
    if( !strmdict )
    {
        cout << "failed to read dictionary of " << key << " stream for page " << endl << flush;
    }
    const char * subtype;
    if( !CGPDFDictionaryGetName( strmdict, "Subtype", &subtype ) ||
       strcmp( subtype, "Image" ) )
    {
        cout << "failed to read Subtype:Image of " << key << " stream for page " << endl << flush;
    }
    CGPDFDictionaryGetName( strmdict, "ColorSpace", &subtype );
    if(strcmp( subtype, "DeviceRGB" )==0){colorMode=3;}
    
    CGPDFInteger bitsper;
    if( !CGPDFDictionaryGetInteger( strmdict, "BitsPerComponent",&bitsper ) )
    {
        cout << "failed to read BitsPerComponent:1 of " << key << " stream for page "<< endl << flush;
    }
    const char * filter;
    if( !CGPDFDictionaryGetName( strmdict, "Filter", &filter ))
    {
        cout << "failed to read Filter: of " << key << " stream for page "<< endl << flush;
    }
    cout_<<" filter="<<filter<<endl;
    
    CGPDFInteger width, height;
    if( !CGPDFDictionaryGetInteger( strmdict, "Width", &width ) ||
       !CGPDFDictionaryGetInteger( strmdict, "Height", &height ) )
    {
        cout << "failed to read width or height of " << key << " stream for page "<< endl << flush;
    }
    cout_<<" width="<<width<<" height="<<height<<endl;
    CGPDFDataFormat fmt = CGPDFDataFormatRaw;
    CFDataRef data = CGPDFStreamCopyData( strm, &fmt );
    
    int32_t len = (int32_t)CFDataGetLength( data );
    const void *bytes = (const void *)CFDataGetBytePtr( data );
    
    cout_<< "image len: " << len <<" bytesPerPixel="<<bitsper<< endl << flush;
    if(bitsper==1){
        GBitmap *img=GBitmap::create((int)width,(int)height);
        unsigned char* bites_data=(unsigned char*)bytes;
        int w=img->columns();
        int i=0;
        
		for(int y=0; y < height; y++ ){
            for(int x=0; x < width; x+=8){
                unsigned char s=bites_data[i]; i++;
                for(int d=0; d < 8; d++ ){
                    img->bytes_data[y*w+x+8-d]=255*(s & 1 ^ 1); s=(s >> 1);
                }
            }
		}
        imageArray.push_back(img);
    }
    
}



#endif