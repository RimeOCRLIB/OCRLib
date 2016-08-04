/*
     File: SpecialProtocol.m 
 Abstract: Our custom NSURLProtocol. 
  Version: 1.1 
  
  
 */

#import <Foundation/NSError.h>

#import "SpecialProtocol.h"
#import "OCRLib.h"
#include <string>
#include <iostream>
using namespace std;



/* NSImage -> jfif utility category. */

@implementation NSImage (JFIFConversionUtils)

	/* returns jpeg file interchange format encoded data for an NSImage regardless of the
	original NSImage encoding format.  compressionValue is between 0 and 1.  
	values 0.6 thru 0.7 are fine for most purposes.  */
- (NSData *)JFIFData:(float) compressionValue {

	NSLog(@"%@ received %@", self, NSStringFromSelector(_cmd));

		/* convert the NSImage into a raster representation. */
	NSBitmapImageRep* myBitmapImageRep = [NSBitmapImageRep imageRepWithData: [self TIFFRepresentation]];
	
		/* convert the bitmap raster representation into a jfif data stream */
	NSDictionary* propertyDict =
		[NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:compressionValue]
			forKey: NSImageCompressionFactor];
	
		/* return the jfif encoded data */
	return [myBitmapImageRep representationUsingType: NSJPEGFileType properties:propertyDict];
}

@end




	/* data passing categories on NSURLRequest and NSMutableURLRequest.  see the
	header file for more info.  */
	
@implementation NSURLRequest (SpecialProtocol)

- (NSDictionary *)specialVars {
	NSLog(@"%@ received1 %@", self, NSStringFromSelector(_cmd));
	return [NSURLProtocol propertyForKey:[SpecialProtocol specialProtocolVarsKey] inRequest:self];
}

@end



@implementation NSMutableURLRequest (SpecialProtocol)

- (void)setSpecialVars:(NSDictionary *)specialVars {

	NSLog(@"%@ received21 %@", self, NSStringFromSelector(_cmd));
	
	NSDictionary *specialVarsCopy = [specialVars copy];
	
	[NSURLProtocol setProperty:specialVarsCopy
		forKey:[SpecialProtocol specialProtocolVarsKey] inRequest:self];
	[specialVarsCopy release];
}

@end




@implementation SpecialProtocol


	/* our own class method.  Here we return the NSString used to mark
	urls handled by our special protocol. */
+ (NSString*) specialProtocolScheme {
	return @"special";
}

	/* our own class method.  Here we return the NSString used to identify
	the property we add to the NSURLRequest object for passing around data. */
+ (NSString*) specialProtocolVarsKey {
	return @"specialVarsKey";
}



	/* our own class method.  We call this routine to handle registration
	of our special protocol.  You should call this routine BEFORE any urls
	specifying your special protocol scheme are presented to webkit. */
+ (void) registerSpecialProtocol {
	static BOOL inited = NO;
	if ( ! inited ) {
		[NSURLProtocol registerClass:[SpecialProtocol class]];
		inited = YES;
	}
}

 
	/* class method for protocol called by webview to determine if this
	protocol should be used to load the request. */
+ (BOOL)canInitWithRequest:(NSURLRequest *)theRequest {

	NSLog(@"%@ received2 %@ with url='%@' and scheme='%@'",
			self, NSStringFromSelector(_cmd),
			[[theRequest URL] absoluteString], [[theRequest URL] scheme]);
    //NSLog(@"@@@ %@",[[theRequest URL] absoluteString]);
	
		/* get the scheme from the URL */
	NSString *theScheme = [[theRequest URL] scheme];
    string path=[[[theRequest URL] absoluteString] UTF8String];
    
    if([theScheme isEqualToString:@"file"]&&path.find("/ocrlib?")!=-1){
        cout<<"@/";
        return true;
    }
	
		/* return true if it matches the scheme we're using for our protocol. */
	return ([theScheme caseInsensitiveCompare: [SpecialProtocol specialProtocolScheme]] == NSOrderedSame );
}


	/* if canInitWithRequest returns true, then webKit will call your
	canonicalRequestForRequest method so you have an opportunity to modify
	the NSURLRequest before processing the request */
+ (NSURLRequest *)canonicalRequestForRequest:(NSURLRequest *)request {

	NSLog(@"%@ received111 %@", self, NSStringFromSelector(_cmd));
	
	/* we don't do any special processing here, though we include this
	method because all subclasses must implement this method. */
	
    return request;
}


	/* our main loading routine.  This is where we do most of our processing
	for our class.  In this case, all we are doing is taking the path part
	of the url and rendering it in 36 point system font as a jpeg file.  The
	interesting part is that we create the jpeg entirely in memory and return
	it back for rendering in the webView.  */
- (void)startLoading {
	NSLog(@"%@ received %@ - start", self, NSStringFromSelector(_cmd));
	
		/* retrieve the current request. */
    //NSURLRequest *request = [self request];
	
		/* extract our special variables from the request.
	NSDictionary* specialVars = [request specialVars];
	if (specialVars) {
			// extract a reference to our WebResourceLoadDelegate object.
		AppDelegate* theCaller = (AppDelegate*) [specialVars objectForKey:[AppDelegate callerKey]];
		if (theCaller) {
			[theCaller callbackFromSpecialRequest: request];
		}
	}
    */
    //NSURLResponse *response;
    //NSData *data;
    
/*
    if(0){   //отработка
            /// Since the scheme is free to encode the url in any way it chooses, here
            we are using the url text to identify files names in our resources folder
            that we would like to display.
            
            // get the path component from the url
        NSString* theString = [[[request URL] path] substringFromIndex:1];
        
            /// set up some font attributes we'll use for rendering the path string to an NSImage
        NSDictionary* fontAttrs =
                [NSDictionary dictionaryWithObjectsAndKeys:
                    [NSColor redColor], NSForegroundColorAttributeName,
                    [NSFont systemFontOfSize:36], NSFontAttributeName,
                    nil];
                    
            /// calculate the size of the rendered string
        NSSize tsz = [theString sizeWithAttributes:fontAttrs];
        
            /// allocate an NSImage with large dimensions enough to draw the entire string.
        NSImage *myImage = [[[NSImage alloc] initWithSize: tsz] autorelease];
        
            /// draw the string into the NSImage
        [myImage lockFocus];
        [theString drawAtPoint:NSMakePoint(0,0) withAttributes:fontAttrs];
        [myImage unlockFocus];
            
            /// retrieve the jfif data for the image
        data = [myImage JFIFData: 0.75];

            /// create the response record, set the mime type to jpeg
        response = 
            [[NSURLResponse alloc] initWithURL:[request URL] 
                MIMEType:@"image/jpeg" 
                expectedContentLength:-1 
                textEncodingName:nil];
    
    }else{
        response =
        [[NSURLResponse alloc] initWithURL:[request URL]
                                  MIMEType:@"text/html"
                     expectedContentLength:-1
                          textEncodingName:nil];
        
        data=[@"Hello World" dataUsingEncoding:NSUTF8StringEncoding];
    
    }
	
		/// get a reference to the client so we can hand off the data
    id<NSURLProtocolClient> client = [self client];

		//// turn off caching for this response data
	[client URLProtocol:self didReceiveResponse:response
			cacheStoragePolicy:NSURLCacheStorageNotAllowed];
	
		/// set the data in the response to our jfif data
	[client URLProtocol:self didLoadData:data];
	
		/// notify that we completed loading
	[client URLProtocolDidFinishLoading:self];
	
		//// we can release our copy
	[response release];
		
		/// if an error occured during our load, here is the code we would
		execute to send that information back to webKit.  We're not using it here,
		but you will probably want to use this code for proper error handling.
	if (0) { /// in case of error 
        int resultCode;
        resultCode = NSURLErrorResourceUnavailable;
        [client URLProtocol:self didFailWithError:[NSError errorWithDomain:NSURLErrorDomain
			code:resultCode userInfo:nil]];
	}
*/
		/* added the extra log statement here so you can see that stopLoading is called
		by the underlying machinery before we leave this routine. */
	NSLog(@"%@ received %@ - end", self, NSStringFromSelector(_cmd));
}

		/* called to stop loading or to abort loading.  We don't do anything special
		here. */
- (void)stopLoading
{
	NSLog(@"%@ received %@", self, NSStringFromSelector(_cmd));
}


@end

