//
//  realPDF.h
//  OCRLib
//  Created by rimeocr on 11/6/12.
//
#include "config.h"

#ifdef COCOA
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

#include <string>
#include <vector>
#include "GBitmap.h"
#include "php2stl.h"

using namespace std;

namespace ocr{
    
class  GPDF {
protected:
    GPDF(void);
    GPDF(const char *path);
    
    uint pageNum;
    CGPDFDocumentRef document;
    string selfPath;             //disk path to PDF document
    string imagePath;              //path to folder with PDF content images
    
///__________________function declaration__________________//
public: 
    virtual ~GPDF();
    void destroy(void);
    /** @name Construction. */
    //@{
    /** Constructs an empty GPDF object.  The returned GPDF has zero pages*/
    static GPDF* create(void) {return new GPDF;}
    
    /**\brief Constructs a GPDF from disk PDF file*/
    static GPDF* create(const char *path)
    {return new GPDF(path);}
    /** @name Initialization. */
    //@{
    /**\brief Initialize GPDF from disk PDF file*/
    void init(const char *path);
    //}

    void write(const char *path){};
    void write(string &path){};
    void writeAllImages();
    int writeImageFromPage(uint index);
    GBitmap* imageFromPage(uint pageNum){GBitmap*img=GBitmap::create(); return img;}
    
};

    
    
}//namespace ocr
#endif