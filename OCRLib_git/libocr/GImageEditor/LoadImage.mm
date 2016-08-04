#include "GImageEditor.h"

 #import <Quartz/Quartz.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

    
    typedef struct tagRGBQUAD {
        unsigned char rgbBlue;
        unsigned char rgbGreen;
        unsigned char rgbRed;
        unsigned char rgbReserved;
    } RGBQUAD;
    
    RGBQUAD *makeLayer(size_t xSize, size_t ySize) {
        assert(ySize % 3 == 0);        // This code only works when ySize is a multiple of 3 /
        assert(sizeof(RGBQUAD) == 4);  // And when sizeof(RGBQUAD) is 4 /
        RGBQUAD *layer = (RGBQUAD*)malloc(sizeof(RGBQUAD) * xSize * ySize);
        const size_t rowSize = sizeof(RGBQUAD) * xSize;
        RGBQUAD *row = (RGBQUAD*)malloc(rowSize);
        const size_t bandSize = ySize / 3;
        size_t y = 0;
        // Make a band of blue /
        memset(row, 0, sizeof(RGBQUAD));
        row->rgbBlue = 255;
        memset_pattern4(row + 1, row, rowSize - sizeof(RGBQUAD));
        for (; y < bandSize; ++y) {
            memcpy(layer + (y * xSize), row, sizeof(RGBQUAD) * xSize);
        }
        // Make a band of green /
        memset(row, 0, sizeof(RGBQUAD));
        row->rgbGreen = 255;
        memset_pattern4(row + 1, row, rowSize - sizeof(RGBQUAD));
        for (; y < bandSize * 2; ++y) {
            memcpy(layer + (y * xSize), row, sizeof(RGBQUAD) * xSize);
        }
        // Make a band of red /
        memset(row, 0, sizeof(RGBQUAD));
        row->rgbRed = 255;
        memset_pattern4(row + 1, row, rowSize - sizeof(RGBQUAD));
        for (; y < ySize; ++y) {
            memcpy(layer + (y * xSize), row, sizeof(RGBQUAD) * xSize);
        }
        free(row);
        return layer;
    }
    
    CGImageRef makeImageFromLayer(RGBQUAD *layer, size_t xSize, size_t ySize) {
        CGDataProviderRef provider =
		CGDataProviderCreateWithData(NULL, layer, sizeof(RGBQUAD) * xSize * ySize, NULL);
        CGColorSpaceRef colorSpace =
		CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
        CGImageRef image =
		CGImageCreate(xSize, ySize, 8, 32, sizeof(RGBQUAD) * xSize,
                      colorSpace, kCGBitmapByteOrderDefault, provider,
                      NULL, FALSE, kCGRenderingIntentDefault);
        CFRelease(provider);
        CFRelease(colorSpace);
        return image;
    }
    
    void writeImageToPath(CGImageRef layerImage, const char* path) {
        CFStringRef pathString =
		CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
        CFURLRef url =
		CFURLCreateWithFileSystemPath(NULL, pathString, kCFURLPOSIXPathStyle, false);
        
        CFDictionaryRef options =
		CFDictionaryCreate(NULL, NULL, NULL, 0, NULL, NULL);
        
        CGImageDestinationRef imageDest =
		CGImageDestinationCreateWithURL(url, kUTTypeTIFF, 1, NULL);
        CGImageDestinationAddImage(imageDest, layerImage, options);
        CGImageDestinationFinalize(imageDest);
        
        CFRelease(imageDest);
        CFRelease(options);
        CFRelease(url);
        CFRelease(pathString);
    }
    

    int  GImageEditor::testJpg(){
        const size_t xSize = 96;
        const size_t ySize = 96;
        const char *outputPath = "/a.tiff";
        RGBQUAD *layer = makeLayer(xSize, ySize);
        CGImageRef layerImage = makeImageFromLayer(layer, xSize, ySize);
        writeImageToPath(layerImage, outputPath);
        CFRelease(layerImage);
        free(layer);
        return 0;
    }

