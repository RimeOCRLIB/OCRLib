#include "GBitsetContour.h"

void
GBitsetContour::init(const GBitmap *ref, short stepW, short stepH, short flipV)
{
      border=2;
	  srcW=ref->columns()+border+2;
	  srcH=ref->rows()+border*2;
	  if(stepH<1)stepH=1;
      if(stepW<1)stepW=1;

	  nrows=ref->rows()/stepH+border+2;
	  ncolumns=ref->columns()/stepW+border+2;
	  init(ncolumns,nrows);
	  unsigned char *row=bytes_data;
      unsigned char *p;
      short h=nrows;
	  short w=ncolumns;
	  textLineSize=0;

#ifdef DEBUGLVL_GREY
      drawData=GBitmap::create(ncolumns,nrows);
#endif
	
	  line=GBitmap::create(w,h);
	  line->fill(0);

if(flipV){
	for (int n=border; n<nrows-border; n++){
		for(int m=border; m<ncolumns-border;m++){
			p=row+n*ncolumns+m;
			if(ref[0][h-n*stepH+border][m*stepW-border]<125){*p=1; //DP(1);
			}else{*p=0;  //DP(0);
			}
#ifdef DEBUGLVL_GREY
			drawData[0][n][m]=255+*p*120;
#endif
			line[0][h-n][m]=(short)*p;
						//DC((short)*p);

		}
		//DC(END)
	}
}else{
	for (int n=border; n<nrows-border; n++){
		for(int m=border; m<ncolumns-border;m++){
			p=row+n*ncolumns+m;
			if(ref[0][n*stepH-border][m*stepW-border]<125){*p=1; ;
			}else{*p=0;
			}
#ifdef DEBUGLVL_GREY
			drawData[0][n][m]=255+*p*120;
#endif
			line[0][n][m]=(short)*p;

		}
		//_//_DC(END);
		}

}
////////////////////////////////////////////////////////////////////////////////

}//__________________________________________

