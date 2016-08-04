/*
     File: SpecialProtocol.h
 Abstract: Our custom NSURLProtocol.
  Version: 1.1
  
 */

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>


	/* our custom NSURLProtocol is implemented as a subclass. */
@interface SpecialProtocol : NSURLProtocol {
}
+ (NSString*) specialProtocolScheme;
+ (NSString*) specialProtocolVarsKey;
+ (void) registerSpecialProtocol;
@end

	/* utility category on NSImage used for converting
	NSImage to jfif data.  */
@interface NSImage (JFIFConversionUtils)

	/* returns jpeg file interchange format encoded data for an NSImage regardless of the
	original NSImage encoding format.  compressionValue is between 0 and 1.  
	values 0.6 thru 0.7 are fine for most purposes.  */
- (NSData *)JFIFData:(float) compressionValue;

@end


/* the following categories are added to NSURLRequest and NSMutableURLRequest
for the purposes of sharing information between the various webView delegate
routines and the custom protocol implementation defined in this file.  Our
WebResourceLoadDelegate (WRLD) will intercept resource load requests before they are
handled and if a NSURLRequest is destined for our special protocol, then the
WRLD will copy the NSURLRequest to a NSMutableURLRequest and call setSpecialVars
to attach some data we want to share between the WRLD and our NSURLProtocol
object. Inside of our NSURLProtocol we can access this data.

In this example, we store a reference to our WRLD object in the dictionary inside
of our WRLD method and then we call a method on our WRLD object from inside of our
startLoading method on our NSURLProtocol object.
*/

@interface NSURLRequest (SpecialProtocol)
- (NSDictionary *)specialVars;
@end

@interface NSMutableURLRequest (SpecialProtocol)
- (void)setSpecialVars:(NSDictionary *)caller;
@end

