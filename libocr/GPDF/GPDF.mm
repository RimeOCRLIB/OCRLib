//
//  GPDF.m
//  OCRLib
//  Created by rimeocr on 11/6/12.
//

#include "config.h"


#ifdef COCOA

#include "GPDF.h"

namespace ocr{

GPDF::~GPDF(){
}

void GPDF::destroy(void)
{
	if(document){
        free(document);
    }
    delete this;
}

GPDF::GPDF()
{
}

GPDF::GPDF(const char *path)
{
    try
    {
        init(path);
    }
    catch(int a)
    {
        destroy();
    }
} 

//Initialize GPDF from disk PDF file
void GPDF::init(const char *path_){
    CFStringRef path;
    CFURLRef url;
    path = CFStringCreateWithCString (NULL, path_,
                                      kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path, // 1
                                         kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    document = CGPDFDocumentCreateWithURL (url);// 2
    CFRelease(url);
    pageNum = (uint)CGPDFDocumentGetNumberOfPages (document);// 3
    if (pageNum == 0) {
        printf("@@@ERROR  `%s' needs at least one page!", path_);
        cout_<<imagePath<<"@@@ERROR ///_______@export error @"<< path_<<endl;
        return;
    }
    selfPath=path_;
    imagePath=substr(0,(uint)selfPath.rfind("."),selfPath);
    imagePath+="_tif";
   }

}  //namespace ocr
#endif


