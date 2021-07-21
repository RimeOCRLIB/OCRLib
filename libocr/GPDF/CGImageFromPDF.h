//
//  CGImageFromPDF.h
//  OCRLib
//
//  Created by rimeocr on 11/10/12.
//

#ifndef OCRLib_CGImageFromPDF_h
#define OCRLib_CGImageFromPDF_h


//experiment
void directParsePDF(const char*path){
    cout_<<"@@@_//_PDF file "<<path<<" is DAMAGED. Try to extract all images"<<endl;
    vector<string>textArray;
    string path_=path;
    readText(textArray, path_);
    int index=0;
    cout_<<"textArray.size()="<<textArray.size()<<endl;
    while(index<textArray.size()){
        if(textArray[index]=="<</Subtype/Image"){
            cout_<<"image"<<endl;
            vector<string>imageInfo;
            while(textArray[index]!="stream"){
                imageInfo.push_back(textArray[index]);index++;
            }
            int width=0,height=0, bits=0;
            if(textArray[index]=="stream"){
                cout_<<"start stream";
                for(int i=0;i<imageInfo.size();i++){
                    int t=(int)imageInfo[i].find("Width ");
                    if(t!=-1){
                        string data=substr(t+6,(int)imageInfo[i].size()-t-6,imageInfo[i]);
                        width=atoi(data.c_str());
                    }
                    t=(int)imageInfo[i].find("Height ");
                    if(t!=-1){
                        string data=substr(t+7,(int)imageInfo[i].size()-t-7,imageInfo[i]);
                        height=atoi(data.c_str());
                    }
                    t=(int)imageInfo[i].find("BitsPerComponent ");
                    if(t!=-1){
                        string data=substr(t+17,(int)imageInfo[i].size()-t-17,imageInfo[i]);
                        bits=atoi(data.c_str());
                    }
                    cout_<<imageInfo[i]<<endl;
                }
                cout_<<"width="<<width<<" height="<<height<<" bits="<<bits<<endl;
                cout_<<"start extract stream data"<<endl;
                string imageData;
                index++;
                while(textArray[index]!="endstream"){
                    imageData+=textArray[index]; imageData+=0x0d;
                    index++;
                }
                cout_<<"imageData.size()="<<imageData.size();
                writeText(imageData,"/__TEST/1_test");
                exit(0);
            }
        }
        index++;
        
    }
    cout_<<"//@@@_done."<<endl;
    
}


NSMutableArray *aRefImgs;
void setRefImgs(NSMutableArray *ref){
    aRefImgs=ref;
}

NSMutableArray* ImgArrRef(){
    return aRefImgs;
}

CGPDFDocumentRef MyGetPDFDocumentRef (const char *filename) {
    CFStringRef path;
    CFURLRef url;
    CGPDFDocumentRef document;
    path = CFStringCreateWithCString (NULL, filename,kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path, kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    document = CGPDFDocumentCreateWithURL (url);// 2
    CFRelease(url);
    int count = CGPDFDocumentGetNumberOfPages (document);// 3
    if (count == 0) {
        printf("@@@ERROR `%s' needs at least one page!", filename);
        return NULL;
    }
    return document;
}


CGFloat *decodeValuesFromImageDictionary(CGPDFDictionaryRef dict, CGColorSpaceRef cgColorSpace, NSInteger bitsPerComponent) {
    CGFloat *decodeValues = NULL;
    CGPDFArrayRef decodeArray = NULL;
    
    if (CGPDFDictionaryGetArray(dict, "Decode", &decodeArray)) {
        size_t count = CGPDFArrayGetCount(decodeArray);
        decodeValues = malloc(sizeof(CGFloat) * count);
        CGPDFReal realValue;
        int i;
        for (i = 0; i < count; i++) {
            CGPDFArrayGetNumber(decodeArray, i, &realValue);
            decodeValues[i] = realValue;
        }
    } else {
        size_t n;
        switch (CGColorSpaceGetModel(cgColorSpace)) {
            case kCGColorSpaceModelMonochrome:
                decodeValues = malloc(sizeof(CGFloat) * 2);
                decodeValues[0] = 0.0;
                decodeValues[1] = 1.0;
                break;
            case kCGColorSpaceModelRGB:
                decodeValues = malloc(sizeof(CGFloat) * 6);
                for (int i = 0; i < 6; i++) {
                    decodeValues[i] = i % 2 == 0 ? 0 : 1;
                }
                break;
            case kCGColorSpaceModelCMYK:
                decodeValues = malloc(sizeof(CGFloat) * 8);
                for (int i = 0; i < 8; i++) {
                    decodeValues[i] = i % 2 == 0 ? 0.0 :
                    1.0;
                }
                break;
            case kCGColorSpaceModelLab:
                // ????
                break;
            case kCGColorSpaceModelDeviceN:
                n =
                CGColorSpaceGetNumberOfComponents(cgColorSpace) * 2;
                decodeValues = malloc(sizeof(CGFloat) * (n *
                                                         2));
                for (int i = 0; i < n; i++) {
                    decodeValues[i] = i % 2 == 0 ? 0.0 :
                    1.0;
                }
                break;
            case kCGColorSpaceModelIndexed:
                decodeValues = malloc(sizeof(CGFloat) * 2);
                decodeValues[0] = 0.0;
                decodeValues[1] = pow(2.0,
                                      (double)bitsPerComponent) - 1;
                break;
            default:
                break;
        }
    }
    
    return (CGFloat *)CFMakeCollectable(decodeValues);
}

NSImage *getImageRef(CGPDFStreamRef myStream) {
    CGPDFArrayRef colorSpaceArray = NULL;
    CGPDFStreamRef dataStream;
    CGPDFDataFormat format;
    CGPDFDictionaryRef dict;
    CGPDFInteger width, height, bps, spp;
    CGPDFBoolean interpolation = 0;
    //  NSString *colorSpace = nil;
    CGColorSpaceRef cgColorSpace;
    const char *name = NULL, *colorSpaceName = NULL, *renderingIntentName = NULL;
    CFDataRef imageDataPtr = NULL;
    CGImageRef cgImage;
    //maskImage = NULL,
    CGImageRef sourceImage = NULL;
    CGDataProviderRef dataProvider;
    CGColorRenderingIntent renderingIntent;
    CGFloat *decodeValues = NULL;
    NSImage *image;
    
    if (myStream == NULL)
        return nil;
    
    dataStream = myStream;
    dict = CGPDFStreamGetDictionary(dataStream);
    
    // obtain the basic image information
    if (!CGPDFDictionaryGetName(dict, "Subtype", &name))
        return nil;
    
    if (strcmp(name, "Image") != 0)
        return nil;
    
    if (!CGPDFDictionaryGetInteger(dict, "Width", &width))
        return nil;
    
    if (!CGPDFDictionaryGetInteger(dict, "Height", &height))
        return nil;
    
    if (!CGPDFDictionaryGetInteger(dict, "BitsPerComponent", &bps))
        return nil;
    
    if (!CGPDFDictionaryGetBoolean(dict, "Interpolate", &interpolation))
        interpolation = NO;
    
    if (!CGPDFDictionaryGetName(dict, "Intent", &renderingIntentName))
        renderingIntent = kCGRenderingIntentDefault;
    else{
        renderingIntent = kCGRenderingIntentDefault;
        //      renderingIntent = renderingIntentFromName(renderingIntentName);
    }
    
    imageDataPtr = CGPDFStreamCopyData(dataStream, &format);
    dataProvider = CGDataProviderCreateWithCFData(imageDataPtr);
    CFRelease(imageDataPtr);
    
    if (CGPDFDictionaryGetArray(dict, "ColorSpace", &colorSpaceArray)) {
        cgColorSpace = CGColorSpaceCreateDeviceRGB();
        //      cgColorSpace = colorSpaceFromPDFArray(colorSpaceArray);
        spp = CGColorSpaceGetNumberOfComponents(cgColorSpace);
    } else if (CGPDFDictionaryGetName(dict, "ColorSpace", &colorSpaceName)) {
        if (strcmp(colorSpaceName, "DeviceRGB") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceRGB();
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
            spp = 3;
        } else if (strcmp(colorSpaceName, "DeviceCMYK") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceCMYK();
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericCMYK);
            spp = 4;
        } else if (strcmp(colorSpaceName, "DeviceGray") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceGray();
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericGray);
            spp = 1;
        } else if (bps == 1) { // if there's no colorspace entry, there's still one we can infer from bps
            cgColorSpace = CGColorSpaceCreateDeviceGray();
            //          colorSpace = NSDeviceBlackColorSpace;
            spp = 1;
        }
    }
    
    decodeValues = decodeValuesFromImageDictionary(dict, cgColorSpace, bps);
    
    int rowBits = bps * spp * width;
    int rowBytes = rowBits / 8;
    // pdf image row lengths are padded to byte-alignment
    if (rowBits % 8 != 0)
        ++rowBytes;
    
    //  maskImage = SMaskImageFromImageDictionary(dict);
    
    if (format == CGPDFDataFormatRaw)
    {
        sourceImage = CGImageCreate(width, height, bps, bps * spp, rowBytes, cgColorSpace, 0, dataProvider, decodeValues, interpolation, renderingIntent);
        CGDataProviderRelease(dataProvider);
        cgImage = sourceImage;
        //      if (maskImage != NULL) {
        //          cgImage = CGImageCreateWithMask(sourceImage, maskImage);
        //          CGImageRelease(sourceImage);
        //          CGImageRelease(maskImage);
        //      } else {
        //          cgImage = sourceImage;
        //      }
    } else {
        if (format == CGPDFDataFormatJPEGEncoded){ // JPEG data requires a CGImage; AppKit can't decode it {
            sourceImage =
            CGImageCreateWithJPEGDataProvider(dataProvider,decodeValues,interpolation,renderingIntent);
            CGDataProviderRelease(dataProvider);
            cgImage = sourceImage;
            //          if (maskImage != NULL) {
            //              cgImage = CGImageCreateWithMask(sourceImage,maskImage);
            //              CGImageRelease(sourceImage);
            //              CGImageRelease(maskImage);
            //          } else {
            //              cgImage = sourceImage;
            //          }
        }
        // note that we could have handled JPEG with ImageIO as well
        else if (format == CGPDFDataFormatJPEG2000) { // JPEG2000 requires ImageIO {
            CFDictionaryRef dictionary = CFDictionaryCreate(NULL, NULL, NULL, 0, NULL, NULL);
            sourceImage=
            CGImageCreateWithJPEGDataProvider(dataProvider, decodeValues, interpolation, renderingIntent);
            
            
            //          CGImageSourceRef cgImageSource = CGImageSourceCreateWithDataProvider(dataProvider, dictionary);
            CGDataProviderRelease(dataProvider);
            
            cgImage=sourceImage;
            
            //          cgImage = CGImageSourceCreateImageAtIndex(cgImageSource, 0, dictionary);
            CFRelease(dictionary);
        } else // some format we don't know about or an error in the PDF
            return nil;
    }
    image=[NSImage imageWithCGImage:cgImage];
    return image;
}
/*
@implementation DashBoard

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    filePath=[[NSString alloc] initWithString:[[NSBundle mainBundle] pathForResource:@"per" ofType:@"pdf"]];
}



-(IBAction)btnTappedText:(id)sender{
    if(arrImgs!=nil && [arrImgs retainCount]>0 ) { [arrImgs release]; arrImgs=nil; }
    arrImgs=[[NSMutableArray alloc] init];
    
    setRefImgs(arrImgs);
    //  if(nxtTxtDtlVCtr!=nil && [nxtTxtDtlVCtr retainCount]>0) { [nxtTxtDtlVCtr release]; nxtTxtDtlVCtr=nil; }
    //  nxtTxtDtlVCtr=[[TxtDtlVCtr alloc] initWithNibName:@"TxtDtlVCtr" bundle:nil];
    //  nxtTxtDtlVCtr.str=StringRef();
    //  [self.navigationController pushViewController:nxtTxtDtlVCtr animated:YES];
    
    // 1. Open Document page
    CGPDFDocumentRef document = MyGetPDFDocumentRef ([filePath UTF8String]);
    
    int pgcnt = CGPDFDocumentGetNumberOfPages( document );
    
    for( int i1 = 0; i1 < pgcnt; ++i1 ) {
        
        CGPDFPageRef pg = CGPDFDocumentGetPage (document, i1+1);
        if( !pg ) {
            NSLog(@"Couldn't open page.");
        } else {
            
            // 2. get page dictionary
            CGPDFDictionaryRef dict = CGPDFPageGetDictionary( pg );
            if( !dict ) {
                NSLog(@"Couldn't open page dictionary.");
            } else {
                // 3. get page contents stream
                CGPDFStreamRef cont;
                if( !CGPDFDictionaryGetStream( dict, "Contents", &cont ) ) {
                    NSLog(@"Couldn't open page stream.");
                } else {
                    // 4. copy page contents steam
                    //  CFDataRef contdata = CGPDFStreamCopyData( cont, NULL );
                    
                    // 5. get the media array from stream
                    CGPDFArrayRef media;
                    if( !CGPDFDictionaryGetArray( dict, "MediaBox", &media ) ) {
                        NSLog(@"Couldn't open page Media.");
                    } else {
                        // 6. open media & get it's size
                        CGPDFInteger mediatop, medialeft;
                        CGPDFReal mediaright, mediabottom;
                        if( !CGPDFArrayGetInteger( media, 0, &mediatop ) || !CGPDFArrayGetInteger( media, 1, &medialeft ) || !CGPDFArrayGetNumber( media, 2, &mediaright ) || !CGPDFArrayGetNumber( media, 3, &mediabottom ) ) {
                            NSLog(@"Couldn't open page Media Box.");
                        } else {
                            // 7. set media size
                            //      double mediawidth = mediaright - medialeft, mediaheight = mediabottom - mediatop;
                            // 8. get media resources
                            CGPDFDictionaryRef res;
                            if( !CGPDFDictionaryGetDictionary( dict, "Resources", &res ) ) {
                                NSLog(@"Couldn't Open Page Media Reopsources.");
                            } else {
                                // 9. get xObject from media resources
                                CGPDFDictionaryRef xobj;
                                if( !CGPDFDictionaryGetDictionary( res, "XObject", &xobj ) ) {
                                    NSLog(@"Couldn't load page Xobjects.");
                                } else {
                                    CGPDFDictionaryApplyFunction(xobj, pdfDictionaryFunction, NULL);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    NSLog(@"Total images are - %i",[arrImgs count]);
    
    if(nxtImgVCtr!=nil && [nxtImgVCtr retainCount]>0 ) { [nxtImgVCtr release]; nxtImgVCtr=nil; }
    nxtImgVCtr=[[ImgVCtr alloc] initWithNibName:@"ImgVCtr" bundle:nil];
    nxtImgVCtr.arrImg=arrImgs;
    [self.navigationController pushViewController:nxtImgVCtr animated:YES];
}
*/

#endif
