#include "GLogicProcessor.h"

#ifdef COCOA
#import "Cocoa/Cocoa.h"

void GLogicProcessor::RTFtoYagpoConverter( string &mainString, string &path){

   
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	NSString *fileType;
	NSTextView *textHolder;
	NSString *pathSrc;
	NSAttributedString *fileContentString;
	//fileName=[[NSMutableString alloc] init];
	int print=0;
    //cout<<"path="<<path<<endl;
	
	pathSrc=[[NSString alloc] initWithCString:path.c_str() encoding:11];
	textHolder = [[NSTextView alloc] init];
	
	
	fileType=[pathSrc pathExtension];
	//cout<<"mainFileIndex="<<fileIndex<<endl;
	
	if ([fileType isEqualToString:@"rtf"]||[fileType isEqualToString:@"RTF"]||[fileType isEqualToString:@"doc"]||[fileType isEqualToString:@"DOC"]){
		NSLog(pathSrc);
		[textHolder readRTFDFromFile:pathSrc];
		//NSAttributedString *textAttrString =[textHolder attributedSubstringFromRange:NSMakeRange(0,[[textHolder string] length])];
		//NSAttributedString *fileContentString=[[NSString alloc]init];
		//fileContentString=[[textAttrString string] copy];
		fileContentString=[textHolder attributedSubstringFromRange:NSMakeRange(0,[[textHolder string] length])];
		DT("l1="<<[[fileContentString string]length]<<END);
	}
	
	//NSAttributedString *testLetter;
	//NSMutableAttributedString *mainString=[[NSMutableAttributedString alloc]init];
	//NSMutableAttributedString *resultString=[[NSMutableAttributedString alloc]init];
	//NSMutableAttributedString *formatLetter=[[NSMutableAttributedString alloc]init];
	
	NSString *fontName,*textString;
	int fileLength=[fileContentString length];
	int letterIndex=0;
	int YagpoWylieFontFlag;
		
	string str, pathMap, tibFontName;
    string letter;
	
	textString=[fileContentString string];
	string uniStack;
	wstring wstr;
	//DT("fileLength="<<fileLength/1000<<" kb"<<END;) 
	int step=0,index=0; 
	//ofstream c_out; c_out.open("/_out1.txt");
	
	while(letterIndex<fileLength){ 
		if(step==10000){cout <<index/1000<<". ";
		step=0;}step++;index++;
		//[progressInd setDoubleValue:letterIndex/k];   
		//[progressInd displayIfNeeded];  
		//testLetter=[textAttrString  attributedSubstringFromRange:NSMakeRange(letterIndex, 1)];		      	
		//testLetter=[fileContentString  attributedSubstringFromRange:NSMakeRange(letterIndex, 1)];
		fontName=[[fileContentString attribute:NSFontAttributeName  atIndex:letterIndex effectiveRange:nil]fontName] ;  //get new letter
		str=[fontName cStringUsingEncoding:11];   
		letter=Unicode_to_UTF([textString  characterAtIndex:letterIndex]);
		DR(" letterIndex="<<letterIndex<<" letter=/"<<letter<<"/ fontName="<<str<<" code="<<hex<<(unsigned short)[textString  characterAtIndex:letterIndex]<<END)
		
		if(letter=="\r"||letter=="\n"){
			if(uniStack.size()!=0){
				DR("YagpoWylieFontFlag"<<YagpoWylieFontFlag<<" uniStack="<<uniStack<<END)
				if(!YagpoWylieFontFlag){
                    uniStack=UniToTibetan(uniStack,0);
                }else{
                    uniStack=WylieToYagpoConverter(uniStack);
                }    
				//DP("uniStackOut="<<uniStack<<END);
				mainString+=uniStack;
				uniStack="";
			}
			mainString+=letter;
			letterIndex++;continue;
		}
		//DR("fontName="<<str<<END)
		if ( fontNameMap.find(str) != fontNameMap.end()) {
			//DR(str <<" is tib font"<<END)
			//________________START_convert Tibetan font_______________							
			//first check did we load the map of this font						
			if ( fontMap.find(str) != fontMap.end()) {
				//DR(str<<" Map exist"<<END)
				//flag=1;
			}else{
				//DT(str<<" Map not exist. Load map "<<str<<END);
				pathMap=inputData.data["tablePath"]+"/codePages/FontMap/"+str+".map";
				LoadFontMap(fontMap[str], pathMap);	
			}	
			//cout<<"fontMap["<<str<<"]"<<fontMap[str].size()<<END;	
			tibFontName=str; YagpoWylieFontFlag=0;
	    		
			//cout <<" font name="<<str<<" letter"<<letter<<END;
			
			if(str=="Yagpo!_Wylie"||str=="Yagpo!_Wylie_Callygraphy"){
				YagpoWylieFontFlag=1;
				//if(letter=="\n"||letter=="\r"){
				//if(uniStack.size()>0){
				//	uniStack=UniToTibetan(uniStack, 1);
				//	NSString *txt=[NSString stringWithCString:(const char*)uniStack.c_str() encoding:4];
				//	NSFont *font = [NSFont fontWithName:@"Yagpo" size:14.0];
				//	NSDictionary *attrsDictionary =	[NSDictionary dictionaryWithObject:font	forKey:NSFontAttributeName];							
				//	NSAttributedString *resultString =[[NSAttributedString alloc] initWithString:txt	attributes:attrsDictionary];
				//	[mainString appendAttributedString:resultString];
				//	[resultString release];
				//cout<<"ready1"<<endl;
				//	uniStack="";
				//	[mainString appendAttributedString:testLetter];
				//	[[mainString mutableString] appendString:@"\n"];
				//cout<<" letterIndex="<<letterIndex<<" lineIndex="<<lineIndex<<endl; lineIndex++;
				//}	
				//}else{
				//if (ASCIToUni.find(letter) != ASCIToUni.end()) {
				//	uniStack+=ASCIToUni[letter];
				//}				
				//}
				//cout<<"letter="<<letter;
				if (fontMap[str].find(letter) != fontMap[str].end()) {
                    uniStack+=letter;
				}else{
					letter="[{"+letter+"="+str+"}]"; //report+=letter;
                    uniStack+=letter;
				}
				//cout<<"l="<<letter<<END;
				
			}else{
				YagpoWylieFontFlag=0;
				DR("letter="<<letter<<"fontMap["<<str<<"]["<<letter<<"].OCRKey="<<fontMap[str][letter].OCRKey<<"/ "<<END)
				
                if (fontMap[str].find(letter) != fontMap[str].end()&&fontMap[str][letter].keyUTF.length()>0) {
						letter=fontMap[str][letter].keyUTF;
				}else{
                    ostringstream st;
                    wstring w;
                    w=UTF_to_Unicode(letter);
                    st<<"[{"<<letter<<"("<<hex<<(short)w[0]<<")"<<"="<<str<<"}]";
                    letter=st.str();  //report+=letter;
				}
				
                DR("l="<<letter<<END)
				if(letter!="•")mainString+=letter;
				
				//if(letter=="\n"||letter=="\r"){ cout<<"NEW"<<END;
				//	if(uniStack.size()>0){
				//		cout<<"uniStack size="<<uniStack.size();
				//		uniStack=UniToTibetan(uniStack,0);						   						    
				//NSString *txt=[NSString stringWithCString:(const char*)letter.c_str() encoding:4];
				//NSFont *font = [NSFont fontWithName:@"Yagpo" size:14.0];							
				//NSDictionary *attrsDictionary =	[NSDictionary dictionaryWithObject:font	forKey:NSFontAttributeName];							
				//NSAttributedString *resultString =[[NSAttributedString alloc] initWithString:txt	attributes:attrsDictionary];
				//[mainString appendAttributedString:resultString];
				//[resultString release];
				//cout<<"ready1"<<endl;
				//uniStack="";
				//[mainString appendAttributedString:testLetter];
				//[[mainString mutableString] appendString:@"\n"];
				//	}	
				//}else{
				//	cout<<fontMap[str][letter].keyUTF<<" uniStack.size()="<<uniStack.size()<<END;
				//}
			}
			
			//formatLetter=testLetter;
			
		}else{
			//DT(str<<" is Western font letter="<<letter<<END);
			if(uniStack.size()>0){
				uniStack=UniToTibetan(uniStack,YagpoWylieFontFlag);
				mainString+=uniStack;
				uniStack=" ";
			}
			mainString+=letter;
			
			//		//cout<<"uniStack size="<<uniStack.size();
			//	NSString *txt=[NSString stringWithCString:(const char*)uniStack.c_str() encoding:4];
			//	NSFont *font = [NSFont fontWithName:@"Yagpo" size:14.0];							
			//	NSDictionary *attrsDictionary =	[NSDictionary dictionaryWithObject:font	forKey:NSFontAttributeName];							
			//	NSAttributedString *resultString =[[NSAttributedString alloc] initWithString:txt	attributes:attrsDictionary];
			//	[mainString appendAttributedString:resultString];
			//	[resultString release];
			//	//cout<<"ready1"<<endl;
			//	uniStack="";
			//	[mainString appendAttributedString:testLetter];
			//}else{
			//[mainString appendAttributedString:testLetter];
			//}
		}
		letterIndex++;
		
	}//end while
	if(uniStack.size()>0){
		uniStack=UniToTibetan(uniStack,YagpoWylieFontFlag);	
		mainString+=uniStack;
		uniStack=" ";
		//cout<<"uniStack size="<<uniStack.size();
		//char *buff="123";
		//uniStackUTF="123";
		//NSString *txt=[NSString stringWithCString:(const char*)uniStack.c_str() encoding:4];		
		//NSFont *font = [NSFont fontWithName:@"Yagpo" size:14.0];		
		//NSDictionary *attrsDictionary =[NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];		
		//NSAttributedString *resultString =[[NSAttributedString alloc] initWithString:txt attributes:attrsDictionary];
		//[mainString appendAttributedString:resultString];
		//[resultString release];
		//uniStack="";
		//[mainString appendAttributedString:testLetter];
	}
	DT("done convert");
	//NSLog([mainString string]);
	//cout<<mainString;
	//[fileContentString setAttributedString:mainString];
	//c_out<<mainString; c_out<<report; c_out.close(); exit(1);
	[pool release];	



}////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif


/*
 NSString *fileType;
 NSTextView *textHolder;
 NSString *pathSrc,*pathDest;
 //fileName=[[NSMutableString alloc] init];
 
 pathSrc=[[NSString alloc] initWithCString:argVector[2].c_str() encoding:11];
 pathDest=[[NSString alloc] initWithCString:argVector[3].c_str() encoding:11];
 textHolder = [[NSTextView alloc] init];
 
 
 fileType=[pathSrc pathExtension];NSLog(pathSrc);
 //cout<<"mainFileIndex="<<fileIndex<<endl;
 
 if ([fileType isEqualToString:@"rtf"]||[fileType isEqualToString:@"RTF"]||[fileType isEqualToString:@"doc"]||[fileType isEqualToString:@"DOC"]){
 
 [textHolder readRTFDFromFile:pathSrc];
 //NSAttributedString *textAttrString =[textHolder attributedSubstringFromRange:NSMakeRange(0,[[textHolder string] length])];
 //NSAttributedString *fileContentString=[[NSString alloc]init];
 //fileContentString=[[textAttrString string] copy];
 NSAttributedString *fileContentString=[textHolder attributedSubstringFromRange:NSMakeRange(0,[[textHolder string] length])];
 //cout<<"l1="<<[[fileContentString string]length]<<endl;
 
 //________________________convertation___________________
 RTFtoYagpoConverter(fileContentString, fontNameMap,  uniTibTableUni,uniTibTableYagpo, ASCItoUni, fileName);
 //cout<<"l2="<<[[fileContentString string]length]<<endl;
 //________________________end convertation___________________
 
 NSTextStorage *textStorage;
 NSTextContainer *textContainer;
 NSLayoutManager *layoutManager;
 
 //[textViewField setHorizontallyResizable:NO];
 textStorage = [[NSTextStorage alloc] initWithAttributedString:fileContentString];
 layoutManager = [[NSLayoutManager alloc] init];
 textContainer = [[NSTextContainer alloc] init];
 [layoutManager addTextContainer:textContainer];
 //[textContainer setLineFragmentPadding:40];
 //[textContainer setWidthTracksTextView:YES];
 
 [textContainer release];
 [textStorage addLayoutManager:layoutManager];
 [layoutManager release];
 
 //fontName=[[string attribute:NSFontAttributeName  atIndex:0 effectiveRange:&effectiveRange] fontName];
 //[textStorage beginEditing];
 //[textStorage addAttribute:NSFontAttributeName
 //               value:[NSFont fontWithName:fontName size:72]
 //               range:NSMakeRange(0, 1)];
 //[textStorage addAttribute:NSBaselineOffsetAttributeName
 //               value:[NSNumber numberWithFloat:-30.0]
 //               range:NSMakeRange(0, 1)];
 
 //[textStorage endEditing];
 
 
 [textContainer setTextView:textHolder];
 [textHolder writeRTFDToFile:pathDest atomically:YES];
 
 */

