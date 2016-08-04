//
//  RDFParser.c
//  OCRLib
//
//  Created by rimeocr on 11/6/12.
//

#include <stdio.h>

/*--------------------------
And, just because I feel like earning some bonus points, if you don't know
how your source PDFs are actually structured and are wondering how to figure
out where your PDFs differ from mine, here's how to examine the structure of
a PDF file:
--------------------------
*/
#include "GPDF.h"

#ifdef COCOA
 
#import <Carbon/Carbon.h>
#import <Quartz/Quartz.h>

#include <iostream>
#include <map>
#include "config.h"
#include "php2stl.h"
#include "GBitmap.h"
#include "GImageEditor.h"


#define DP(x)  

using namespace std;
namespace ocr{
    
static uint level=1;
static const char *sPdfTypeNames[] = { "", "null", "boolean", "integer", "real", "name", "string", "array", "dictionary", "stream" };
static vector<GBitmap*>imageArray;  //all images from one page
static string color;

CGFloat* decodeValuesFromImageDictionary(CGPDFDictionaryRef dict, CGColorSpaceRef cgColorSpace, NSInteger bitsPerComponent) {
    CGFloat *decodeValues = NULL;
    CGPDFArrayRef decodeArray = NULL;
    
    if (CGPDFDictionaryGetArray(dict, "Decode", &decodeArray)) {
        size_t count = CGPDFArrayGetCount(decodeArray);
        decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * count);
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
                decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * 2);
                decodeValues[0] = 0.0;
                decodeValues[1] = 1.0;
                break;
            case kCGColorSpaceModelRGB:
                decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * 6);
                for (int i = 0; i < 6; i++) {
                    decodeValues[i] = i % 2 == 0 ? 0 : 1;
                }
                break;
            case kCGColorSpaceModelCMYK:
                decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * 8);
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
                decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * (n *
                                                         2));
                for (int i = 0; i < n; i++) {
                    decodeValues[i] = i % 2 == 0 ? 0.0 :
                    1.0;
                }
                break;
            case kCGColorSpaceModelIndexed:
                decodeValues = (CGFloat*)malloc(sizeof(CGFloat) * 2);
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

CGImage* getImageRef(CGPDFStreamRef myStream) {
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
        color="RGB";
        //      cgColorSpace = colorSpaceFromPDFArray(colorSpaceArray);
        spp = CGColorSpaceGetNumberOfComponents(cgColorSpace);
    } else if (CGPDFDictionaryGetName(dict, "ColorSpace", &colorSpaceName)) {
        if (strcmp(colorSpaceName, "DeviceRGB") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceRGB();
            color="RGB";
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
            spp = 3;
        } else if (strcmp(colorSpaceName, "DeviceCMYK") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceCMYK();
            color="CMYK";            
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericCMYK);
            spp = 4;
        } else if (strcmp(colorSpaceName, "DeviceGray") == 0) {
            cgColorSpace = CGColorSpaceCreateDeviceGray();
            color="GRAY";
            //          CGColorSpaceCreateWithName(kCGColorSpaceGenericGray);
            spp = 1;
        } else if (bps == 1) { // if there's no colorspace entry, there's still one we can infer from bps
            cgColorSpace = CGColorSpaceCreateDeviceGray();
            color="GRAY";
            //          colorSpace = NSDeviceBlackColorSpace;
            spp = 1;
        }
    }
    
    decodeValues = decodeValuesFromImageDictionary(dict, cgColorSpace, bps);
    
    int rowBits = (int)(bps * spp * width);
    int rowBytes = rowBits / 8;
    // pdf image row lengths are padded to byte-alignment
    if (rowBits % 8 != 0)
        ++rowBytes;
    
    //  maskImage = SMaskImageFromImageDictionary(dict);
    
    if (format == CGPDFDataFormatRaw)
    {    //cout<<"ROW";
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
        if (format == CGPDFDataFormatJPEGEncoded){ //cout<<"JPEG"; // JPEG data requires a CGImage; AppKit can't decode it {
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
        else if (format == CGPDFDataFormatJPEG2000) { //cout<<"JPEG2000";// JPEG2000 requires ImageIO {
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
    return cgImage;
}

void DumpObjectProperties( CGPDFObjectRef obj ){
        int cnt;
        
        CGPDFObjectType type = CGPDFObjectGetType( obj );
        switch( type )
        {
            case kCGPDFObjectTypeNull:{}
            case kCGPDFObjectTypeStream:{}
            case kCGPDFObjectTypeBoolean:
            {
                CGPDFBoolean pdfbool;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeBoolean, &pdfbool
                                        ) )
                {
                    DP(if( pdfbool )
                        cout<< " - " << true;
                    else
                        cout<< " - " << false;)
                }
            }
                break;
                
            case kCGPDFObjectTypeInteger:
            {
                CGPDFInteger pdfint;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeInteger, &pdfint )
                   ){}
                DP(cout<< " - " << pdfint;)
            }
                break;
                
            case kCGPDFObjectTypeReal:
            {
                CGPDFReal pdfreal;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeReal, &pdfreal ) ){}
                    DP(cout<< " - " << pdfreal;)
            }
                break;
                
            case kCGPDFObjectTypeName:
            {
                const char * name;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeName, &name ) ){}
                    DP(cout<< " - " << name;)
            }
                break;
                
            case kCGPDFObjectTypeString:
            {
                CGPDFStringRef pdfstr;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeString, &pdfstr )){}
                    DP(cout<< " - " << string( (char *) CGPDFStringGetBytePtr(pdfstr ), CGPDFStringGetLength( pdfstr ) );)
            }
                break;
                
            case kCGPDFObjectTypeArray:
            {
                CGPDFArrayRef array;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeArray, &array ) )
                {
                    DP(cnt = (int)CGPDFArrayGetCount( array );
                    cout<< " - " << "entries: " << cnt;)
                }
            }
                break;
                
            case kCGPDFObjectTypeDictionary:
            {
                CGPDFDictionaryRef dict;
                if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeDictionary, &dict))
                {
                    DP(cnt = (int)CGPDFDictionaryGetCount( dict );
                    cout<< " - " << "entries: " << cnt;)
                }
            }
                break;
        }
        DP(cout<< endl << flush;)
}

    
void DumpObject( const char * key, CGPDFObjectRef obj, void *pdf_){
    DP(for( int i = 0; i < level; ++i )
        cout<< "| ";)
    
    CGPDFObjectType type = CGPDFObjectGetType( obj );
    if( type >= 1 && type < sizeof( sPdfTypeNames ) / sizeof( char *) )
    {
        DP(cout<< key << ": " << sPdfTypeNames[type];)
        if(strncmp(sPdfTypeNames[type],"stream",6)==0&&(key[0]=='I'||key[0]=='i')){
            DP(cout<<endl<<"@@@__Image__";)
            CGPDFStreamRef strm;
            if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeStream, &strm
                                    ) ){
                DP(cout<<"+"<<endl;)
            }
            CGImage *cgimage=getImageRef(strm);
            NSData *data = (NSData *)CGDataProviderCopyData(CGImageGetDataProvider(cgimage));
            size_t bpp = CGImageGetBitsPerPixel(cgimage);
            DP(cout<<"bpp="<<bpp<<" color="<<color<<endl;)
            if(bpp==1){
                const char *bytes = (const char *)[data bytes];
                size_t width  = CGImageGetWidth(cgimage);
                size_t height = CGImageGetHeight(cgimage);
                GBitmap *img=GBitmap::create((int)width,(int)height);
                unsigned char* bites_data=(unsigned char*)bytes;
                DP(cout<<"width="<<width<<" height="<<height<<endl;)
                int w=img->columns();
                int i=0;
                for(int y=0; y < height; y++ ){
                    for(int x=0; x < width-8; x+=8){
                        unsigned char s=bites_data[i]; i++;
                        for(int d=0; d < 8; d++ ){
                            img->bytes_data[y*w+x+8-d]=255-255*(s & 1 ^ 1); s=(s >> 1);
                        } 
                    }i++;
                }
                imageArray.push_back(img);

            }
            if(bpp==8&&color=="GRAY"){
                const char *bytes = (const char *)[data bytes];
                size_t width  = CGImageGetWidth(cgimage);
                size_t height = CGImageGetHeight(cgimage);
                GBitmap *img=GBitmap::create((int)width,(int)height);
                unsigned char* bytes_data=(unsigned char*)bytes;
                int w=img->columns();
                int i=0;
                for(int y=0; y < height; y++ ){
                    for(int x=0; x < w; x++){
                        img->bytes_data[y*w+x]=bytes_data[i];
                        i++;
                    }
                }
                img->binarisation(80,24);
                imageArray.push_back(img);
                
            }
            if(bpp==8&&color=="RGB"){
                const char *bytes = (const char *)[data bytes];
                uint width  = (uint)CGImageGetWidth(cgimage);
                uint height = (uint)CGImageGetHeight(cgimage);
                GBitmap *img=GBitmap::create((int)width,(int)height,4);
                unsigned char* bytes_data=(unsigned char*)bytes;
                int w=(int)width*4;
                uint i=0;
                //uint length=width*height*3;
                //cout<<"width="<<width<<" height="<<height<<endl;
                
                for(int y=0; y < height; y++ ){
                    for(int x=0; x < w; x+=4){
                        img->bytes_data[y*w+x]=bytes_data[i];
                        img->bytes_data[y*w+x+1]=bytes_data[i+1];
                        img->bytes_data[y*w+x+2]=bytes_data[i+2];
                        img->bytes_data[y*w+x+3]=255;//bytes_data[i+3];
                        i+=3;
                    }
                }
                imageArray.push_back(img);
            }
            if(bpp==24){
                const char *bytes = (const char *)[data bytes];
                uint width  = (uint)CGImageGetWidth(cgimage);
                uint height = (uint)CGImageGetHeight(cgimage);
                GBitmap *img=GBitmap::create((int)width,(int)height,4);
                unsigned char* bytes_data=(unsigned char*)bytes;
                int w=(int)width*4;
                uint i=0;
                //uint length=width*height*3;
                //cout<<"width="<<width<<" height="<<height<<endl;
                
                for(int y=0; y < height; y++ ){
                    for(int x=0; x < w; x+=4){
                        img->bytes_data[y*w+x]=bytes_data[i];
                        img->bytes_data[y*w+x+1]=bytes_data[i+1];
                        img->bytes_data[y*w+x+2]=bytes_data[i+2];
                        img->bytes_data[y*w+x+3]=255;//bytes_data[i+3];
                        i+=3;
                    }
                }
                imageArray.push_back(img);
            } 
            if(bpp==32){
                const char *bytes = (const char *)[data bytes];
                uint width  = (uint)CGImageGetWidth(cgimage);
                uint height = (uint)CGImageGetHeight(cgimage);
                GBitmap *img=GBitmap::create((int)width,(int)height,4);
                unsigned char* bytes_data=(unsigned char*)bytes;
                int w=(int)width*4;
                //cout<<"width="<<width<<" height="<<height<<endl;
                uint i=0;
                //uint length=width*height*3;
                
                for(int y=0; y < height; y++ ){
                    for(int x=0; x < w; x+=4){
                        img->bytes_data[y*w+x]=bytes_data[i];
                        img->bytes_data[y*w+x+1]=bytes_data[i+1];
                        img->bytes_data[y*w+x+2]=bytes_data[i+2];
                        img->bytes_data[y*w+x+3]=255;
                        i+=4;
                    }
                }
                imageArray.push_back(img);
            }
            CFRelease(cgimage);
            [data release];
            
            
        }
        DumpObjectProperties( obj );
    }
    DP(else
        cout<< key << ": " << "unrecognized object type " << type << endl<< flush;)
    
    switch(type){
        case kCGPDFObjectTypeNull:{}
        case kCGPDFObjectTypeBoolean:{}
        case kCGPDFObjectTypeInteger:{}
        case kCGPDFObjectTypeReal:{}
        case kCGPDFObjectTypeName:{}
        case kCGPDFObjectTypeString:{}
        case kCGPDFObjectTypeDictionary:
        {
            if( strcmp( "Parent", key ) )
            {
                ++level;
                CGPDFDictionaryRef dict;
                if( CGPDFObjectGetValue(obj,
                                        kCGPDFObjectTypeDictionary,
                                        &dict )
                   ){
                    
                    CGPDFDictionaryApplyFunction( dict, DumpObject, NULL );
                }
                --level;
            }
        }
            break;
            
        case kCGPDFObjectTypeArray:
        {
            ++level;
            CGPDFArrayRef array;
            if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeArray, &array
                                    ) )
            {
                int arraycnt = (int)CGPDFArrayGetCount( array );
                for( int i = 0; i < arraycnt; ++i )
                {
                    CGPDFObjectRef aryobj;
                    if( CGPDFArrayGetObject( array, i, &aryobj ) )
                    {
                        char tmp[16];
                        sprintf( tmp, "%d", i );
                        DumpObject( tmp, aryobj, NULL );
                    }
                }
            }
            --level;
        }
            break;
            
        case kCGPDFObjectTypeStream:
        {
            ++level;
            CGPDFStreamRef strm;
            if( CGPDFObjectGetValue( obj, kCGPDFObjectTypeStream, &strm ) )
            {
                CGPDFDictionaryRef dict = CGPDFStreamGetDictionary( strm );
                if( dict )
                    CGPDFDictionaryApplyFunction( dict, DumpObject, NULL );
            }
            --level;
        }
            break;
    }
}
void GPDF::writeAllImages(){
    mkdir(imagePath.c_str(),0775);
    cout<<"@Start export images from "<<selfPath<<endl;
    for(int i=0;i<pageNum;i++){
        int imageExport=writeImageFromPage(i+1);
        if(!imageExport){
            cout<<imagePath<<"@@@ERROR! @export page "<<i<<" error<<endl";
        }
        
    }
    cout<<"@DONE writeAllImages"<<endl;
};
    

int GPDF::writeImageFromPage(uint index){

        CGPDFPageRef pg = CGPDFDocumentGetPage( document, index );
        if( !pg )
        {
            cout<< "@@@ERROR failed to read page " << index << " of source pdf file" << endl << flush;
            return 0;
        }
        
        CGPDFDictionaryRef dict = CGPDFPageGetDictionary( pg );
        if( !dict )
        {
            cout<< "@@@ERROR failed to read dictionary for page " << index << " of source pdf file" << endl << flush;
            return 0;
        }
        
        DP(cout<< "@@@//_____page: " << index << endl << flush;)
        CGPDFDictionaryApplyFunction( dict, DumpObject, NULL );
        if(imageArray.size()){
            //собираем все изображения в одно.
            int maxW=0; int h=0; int minH=1024;
            int startIndex=0;
            GBitmap *pecha;
            if(imageArray.size()>1){
                for(int i=0;i<imageArray.size();i++){
                    h+=imageArray[i]->rows();
                    if(imageArray[i]->rows()<minH){minH=imageArray[i]->rows(); startIndex=i;}
                    if(imageArray[i]->columns()>maxW)maxW=imageArray[i]->columns();
                }
                DP(cout<<" maxW="<<maxW<<" h="<<h<<endl;)
                DP(cout<<"imageArray.size()="<<imageArray.size()<<endl;)
          
                pecha=GBitmap::create(maxW,h);
                int y=0;
                for(int i=startIndex;i<imageArray.size();i++){
                    pecha->drawImg(imageArray[i], 0, y); y+=imageArray[i]->rows();
                    imageArray[i]->destroy();
                }
                for(int i=0;i<startIndex;i++){
                    pecha->drawImg(imageArray[i], 0, y); y+=imageArray[i]->rows();
                    imageArray[i]->destroy();
                }
            }else{
                pecha=imageArray[0];
            }
            
            string mode="tif";
            ostringstream out;
            //cout<<"pecha->colorMode="<<pecha->colorMode<<endl;
            if(pecha->colorMode==1){
                out<<imagePath<<"/page_";
                out.width(4);
                out.fill('0');
                out<<index;
                out<<"."<<mode;
                string path=out.str();
                GImageEditor *imageEditor=(GImageEditor *)inputData.imageEditor;
                imageEditor->WriteImageData(pecha,path,0,mode);
                pecha->destroy();
            }else if(pecha->colorMode==4){
                mode="jpg";
                out<<imagePath<<"/page_";
                out.width(4);
                out.fill('0');
                out<<index;
                out<<"."<<mode;
                string path=out.str(); 
                GImageEditor *imageEditor=(GImageEditor *)inputData.imageEditor;
                imageEditor->WriteImageDataRGB(pecha,path,0);
                pecha->destroy();
            }
            imageArray.resize(0);
            DP(cout<<"done page"<<index<<endl;)
        }
    
    
    return 1;
}
    
}

#endif