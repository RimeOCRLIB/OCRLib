//
//  tiffio_osx.cpp
//  OCRLib
//
//  Created by rimeocr on 11/6/12.
//
#include <Carbon/Carbon.h>
#include <iostream>

using namespace std;

int main_pdf (int argc, char * const argv[])
{
    if( argc != 3 )
    {
        cout << "pagesender2tiff Copyright 2006 by Scott Ribe" << endl
        << "usage: pagesender2tiff source.pdf dest.tiff" << endl <<
        flush;
        return 1;
    }
    
    CFStringRef path = CFStringCreateWithCString( NULL, argv[1],
                                                 kCFStringEncodingUTF8 );
    CFURLRef url = CFURLCreateWithFileSystemPath( NULL, path,
                                                 kCFURLPOSIXPathStyle, 0 );
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL( url );
    if( !doc )
    {
        cout << "could not open source pdf file" << endl << flush;
        return 1;
    }
    
    int pgcnt = (int)CGPDFDocumentGetNumberOfPages( doc );
    cout<<"pgcnt="<<pgcnt<<endl;
    if( pgcnt <= 0 )
    {
        cout << "source pdf file has no pages" << endl << flush;
        return 1;
    }
    
    //TIFF * tif = TIFFOpen( argv[2], "w" );
    //if( !tif )
   // {
   //     cout << "failed to open destination tiff file" << endl << flush;
   //     continue;
   // }  //####
    pgcnt=11;
    
    for( int i1 = 10; i1 < pgcnt; ++i1 )
    {
        CGPDFPageRef pg = CGPDFDocumentGetPage( doc, i1 + 1 );
        if( !pg )
        {
            cout << "failed to read page " << i1 + 1 << endl << flush;
            continue;
        }
        
        CGPDFDictionaryRef dict = CGPDFPageGetDictionary( pg );
        if( !dict )
        {
            cout << "failed to read dictionary for page " << i1 + 1 << endl
            << flush;
            continue;
        }
        
        CGPDFStreamRef cont;
        if( !CGPDFDictionaryGetStream( dict, "Contents", &cont ) )
        {
            cout << "failed to read contents stream for page " << i1 + 1 <<
            endl << flush;
            continue;
        }
        CFDataRef contdata = CGPDFStreamCopyData( cont, NULL );
        cout << "contents: " << (char *) CFDataGetBytePtr( contdata ) <<
        endl << flush;
        
        CGPDFArrayRef media;
        if( !CGPDFDictionaryGetArray( dict, "MediaBox", &media ) )
        {
            cout << "failed to read media box array for page " << i1 + 1 <<
            endl << flush;
            continue;
        }
        
        CGPDFInteger mediatop, medialeft;
        CGPDFReal mediaright, mediabottom;
        if( !CGPDFArrayGetInteger( media, 0, &mediatop ) ||
           !CGPDFArrayGetInteger( media, 1, &medialeft ) ||
           !CGPDFArrayGetNumber( media, 2, &mediaright ) ||
           !CGPDFArrayGetNumber( media, 3, &mediabottom ) )
        {
            cout << "failed to read media box values for page " << i1 + 1 <<
            endl << flush;
            continue;
        }
        double mediawidth = mediaright - medialeft, mediaheight =
        mediabottom - mediatop;
        cout<<"mediawidth="<<mediawidth<<" mediaright="<<mediaright<<" medialeft="<<medialeft<<" mediaheight="
        <<mediaheight<<" mediabottom="<<mediabottom<<" mediatop="<<mediatop<<endl;
        
        CGPDFDictionaryRef res;
        if( !CGPDFDictionaryGetDictionary( dict, "Resources", &res ) )
        {
            cout << "failed to read resources dictionary for page " << i1 +
            1 << endl << flush;
            continue;
        }
        
        CGPDFDictionaryRef xobj;
        if( !CGPDFDictionaryGetDictionary( res, "XObject", &xobj ) )
        {
            cout << "failed to read xobject dictionary for page " << i1 + 1
            << endl << flush;
            continue;
        }
        
        char *imagestr="I10";
        //sprintf( imagestr, "I%d", i1 + 1 );

        
        CGPDFStreamRef strm;
        if( !CGPDFDictionaryGetStream( xobj, imagestr, &strm ) )
        {
            cout << "failed to read " << imagestr <<" stream for page " <<
            i1 + 1 << endl << flush;
            continue;
        }
        
        CGPDFDictionaryRef strmdict = CGPDFStreamGetDictionary( strm );
        if( !strmdict )
        {
            cout << "failed to read dictionary of " << imagestr << " stream for page " << i1 + 1 << endl << flush;
                continue;
        }
        
        const char * type;
        if( !CGPDFDictionaryGetName( strmdict, "Type", &type )
           || strcmp(type, "XObject" ) )
        {
            cout << "failed to read Type:XObject of Im1 stream for page " <<
            i1 + 1 << endl << flush;
            continue;
        }
        
        const char * subtype;
        if( !CGPDFDictionaryGetName( strmdict, "Subtype", &subtype ) ||
           strcmp( subtype, "Image" ) )
        {
            cout << "failed to read Subtype:Image of " << imagestr << " stream for page " << i1 + 1 << endl << flush;
                continue;
        }
        
        //CGPDFInteger bitsper;
        //if( !CGPDFDictionaryGetInteger( strmdict, "BitsPerComponent",
        //                               &bitsper ) || bitsper != 1 )
        //{
        //    cout << "failed to read BitsPerComponent:1 of " << imagestr << " stream for page " << i1 + 1 << endl << flush;
        //        continue;
        //}
        
        const char * filter;
        if( !CGPDFDictionaryGetName( strmdict, "Filter", &filter ) ||
           strcmp( filter, "FlateDecode" ) )
        {
            cout << "failed to read Filter:FlateDecode of " << imagestr << " stream for page " << i1 + 1 << endl << flush;
                continue;
        }
        
        CGPDFInteger width, height;
        if( !CGPDFDictionaryGetInteger( strmdict, "Width", &width ) ||
           !CGPDFDictionaryGetInteger( strmdict, "Height", &height ) )
        {
            cout << "failed to read width or height of " << imagestr << " stream for page " << i1 + 1 << endl << flush;
                continue;
        }
        cout<<" width="<<width<<" height="<<height<<endl;
        CGPDFDataFormat fmt = CGPDFDataFormatRaw;
        CFDataRef data = CGPDFStreamCopyData( strm, &fmt );
        
        int32_t len = (int32_t)CFDataGetLength( data );
        const void *bytes = (const void *)CFDataGetBytePtr( data );
        
        cout << "image len: " << len <<" bytesPerPixel="<<len/(width*height)<< endl << flush;
        
        //int32_t rowbytes = (width + 7) / 8;
        //if( rowbytes * height != len )
        //{
        //    cout << "calculated rowbytes (" << rowbytes << ") x height (" <<
        //    height << ") does not match data length (" << len << ")" << endl << flush;
        //    continue;
        //}
        
        double xres = width / mediawidth * 72.0, yres = height / mediaheight
        * 72.0;
        xres = round( xres * 1000 ) / 1000;
        yres = round( yres * 1000 ) / 1000;
        cout << "xres: " << xres << ", yres: " << yres << endl << flush;
        
 /*       TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height );
        
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
        TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, xres);
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, yres);
        
        TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
        
        TIFFSetField( tif, TIFFTAG_SOFTWARE, "pagesender2tiff Copyright 2006
                     by Scott Ribe" );
                     
                     TIFFWriteEncodedStrip( tif, 0, (void *) bytes, rowbytes * height );
                     
                     TIFFWriteDirectory( tif );
 */                     
                  CFRelease( data );
                  CGPDFPageRelease( pg );
        }
                     
        //             TIFFClose( tif );
    
                     return 0;
}
                     
