#include "GFontEditor.h"

int GFontEditor::setLetterScale(GBitmap *img128, GLetter *glyph_){
   
	//int maxCorrelation=0;
	int c1, index, mC,mS,c2;
	float scale,step;
    
	
	cout<<"//so we has two letters. One is from base and one is from text we need found the best correlation"<<endl;
	GLetter *glyph=GLetter::create(); glyph->copy(glyph_);
	
	c1=setCorrelation(img128,glyph,1,0); 
	step=0.25; index=0; scale=1; mC=0;
	if(c1>0){  cout<<"two way search"<<endl;
		mC=c1; mS=1;
		while(index<4){	
			glyph->copy(glyph_);
			c2=setCorrelation(img128,glyph,scale,0); //scale UP
			if (c2>mC){ mC=c2;mS=scale;}
			scale+=step; index++;
		}
		index=0; scale=1;
		while(index<3){	
			glyph->copy(glyph_);
			c2=setCorrelation(img128,glyph,scale,0); //scale UP
			if (c2>mC){ mC=c2;mS=scale;}
			scale-=step; index++;
		}
		cout<<"second pass case1"<<endl;
		glyph->copy(glyph_);
		c2=setCorrelation(img128,glyph,mS-0.1,0); //scale UP
		if (c2>mC){ mC=c2;mS=mS-0.1;}
		glyph->copy(glyph_);
		c2=setCorrelation(img128,glyph,mS+0.2,0); //scale UP
		if (c2>mC){ mC=c2;mS=mS+0.2;}
		
		
	}else{
		cout<<"step by step search"<<endl;  scale=0.5;
		while(index<9){	
			glyph->copy(glyph_);
			c2=setCorrelation(img128,glyph,scale,0); //scale UP
			if (c2>mC){ mC=c2;mS=scale;}
			scale+=step; index++;
			cout<<"c2="<<c2<<"scale="<<scale<<endl;
		}
		if(mC>0){ cout<<"second pass case2 mS="<<mS<<" mC="<<mC<<endl;
			glyph->copy(glyph_);
			c2=setCorrelation(img128,glyph,mS-0.1,0); //scale UP
			if (c2>mC){ mC=c2;mS=mS-0.1;}
			glyph->copy(glyph_);
			c2=setCorrelation(img128,glyph,mS+0.2,0); //scale UP
			if (c2>mC){ mC=c2;mS=mS+0.2;}
			cout<<" done"<<endl;
		}
	}
	if(mC>0){
		cout<<" result mS="<<mS<<" mC="<<mC<<endl;	
		//glyph.copy(glyph_);
		mC=setCorrelation(img128,glyph_,mS,0);  //setCorrelation store actual information in glyph_
		glyph_->scale=mS;
        glyph->destroy();
		return mC;
	} 
    glyph->destroy();
	return 0;


	
}//_____________________________________________________________________________

