#include "GBitsetOCR.h"

// ----- constructor and destructor
// ----- constructor and destructor

void
GBitsetOCR::destroy(void)
{
 if ( data_size !=0 ){
#ifdef DEBUGLVL_GREY	
	 drawData->destroy();
#endif
	 data_size=0;
 }
    //delete this;

}

GBitsetOCR::GBitsetOCR(int nrows, int ncolumns, int border)
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
  try
  {
	init(nrows, ncolumns, border);
  }
  catch(int a)
  {
	destroy();
  }
}


GBitsetOCR::GBitsetOCR(const GBitsetOCR *ref)
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
  try
  {
	init(ref, ref->border);
  }
  catch(int a)
  {
	destroy();
  }
}

GBitsetOCR::GBitsetOCR(const GBitmap *ref, short stepW, short stepH)
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
  try
  {
	init(ref, stepW, stepH);
  }
  catch(int a)
  {
	destroy();
  }

}


GBitsetOCR::GBitsetOCR(const GBitsetOCR *ref, int border)
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
  try
  {
	init(ref, border);
  }
  catch(int a)
  {
	destroy();
  }
}



// ----- initialization

void
GBitsetOCR::init(int arows, int acolumns, int aborder)
{
 // c_out.open("c:/consoleOCR.html");
 // destroy();
  nrows = arows;
  ncolumns = acolumns;
  border = aborder;
  bits_per_row = ncolumns + border*2;
  //int nbits = (nrows * bits_per_row);
  //if (nbits > 0)
//	{
//		bits_data=(bool*)malloc(nbits);
//	}
  data_size=1;
}


void
GBitsetOCR::init(const GBitsetOCR *ref, int aborder)
{
   if (this != ref)
	{
	  init(ref->nrows, ref->ncolumns, aborder);
	  //bool *row = bits_data+border;
	  //bool *rowDest;
	  //for (int n=0; n<nrows; n++, row+=bits_per_row){
	//	rowDest=bits_data+bits_per_row*n+border*2*n+border;
	//	memcpy( (void*)row, (void*)rowDest,  ncolumns/8 );
	 // }
	}
  else if (aborder > border)
	{
	  //minborder(aborder);
	}
  data_size=1;
}


void
GBitsetOCR::init(const GBitmap *ref, short stepW, short stepH){
	srcW=ref->columns();
	srcH=ref->rows();
	nrows=ref->rows()/stepH;
	ncolumns=ref->columns()/stepW;
	init(nrows,ncolumns, 0);
	//bool *row=bits_data;
	
	//short h=nrows+6;
	//short w=ncolumns+6;
	inputBitmap=ref;
	
#ifdef DEBUGLVL_GREY
	bool p;
    drawData=GBitmap::create(ncolumns, nrows);
		for (int n=0; n<nrows; n++){
			for(int m=0; m<ncolumns;m++){
				//p=row+n*ncolumns+m;  
				if(ref[0][n*stepH][m*stepW]<125){p=1; ;
				}else{p=0;}
				drawData[0][n][m]=255+p*120;
			}
			//_//_DC(END);
		}
#endif
	data_size=1;
}



// Fills a bitmap with the given value
void
GBitsetOCR::fill(bool value)
{
  for(unsigned int y=0; y<rows(); y++)
	{
	  bool* bm_y = (*this)[y];
	  for(unsigned int x=0; x<columns(); x++)
		bm_y[x] = value;
	}
}


