#include "GBitsetContour.h"

// ----- constructor and destructor


GBitsetContour::~GBitsetContour()
{}

void
GBitsetContour::destroy(void)
{
 if ( data_size !=0 ){

 free(bytes_data);
#ifdef DEBUGLVL_GREY
 drawData->destroy();
#endif	 
 line->destroy();
 data_size=0;
 }
}

GBitsetContour::GBitsetContour()
  : nrows(0), ncolumns(0), 
	bytes_per_row(0),data_size(0)
{
	textLineSize=0;
}

GBitsetContour::GBitsetContour(int nrows, int ncolumns)
  : nrows(0), ncolumns(0), 
  bytes_per_row(0),data_size(0)
{
  try
  {
	init(ncolumns,nrows);
  }
  catch(int a)
  {
	destroy();
  }
}


GBitsetContour::GBitsetContour(const GBitsetContour *ref)
  : nrows(0), ncolumns(0), 
	bytes_per_row(0),data_size(0)
{
  try
  {
	init(ref);
  }
  catch(int a)
  {
	destroy();
  }
}

GBitsetContour::GBitsetContour(const GBitmap *ref, short stepW, short stepH, short flipV)
  : nrows(0), ncolumns(0), 
	bytes_per_row(0),data_size(0)
{
  try
  {
	init(ref, stepW, stepH, flipV);
  }
  catch(int a)
  {
	destroy();
  }

}


// ----- initialization

void
GBitsetContour::init(int acolumns, int arows)
{
  //c_out.open("c:/consoleBitContour.txt");
  destroy();
  nrows = arows;
  ncolumns = acolumns;
  textLineSize=0;
  bytes_per_row = ncolumns;
  int nbytes = (nrows * bytes_per_row);
  if (nbytes > 0)
	{

		bytes_data=(unsigned char*)malloc(nbytes);
		
	}
  data_size=1;
}


void GBitsetContour::init(const GBitsetContour *ref)
{
  if (this != ref)
	{
	  init(ref->ncolumns, ref->nrows);
	  unsigned char *row = bytes_data;
	  unsigned char *rowDest;
	  for (int n=0; n<nrows; n++, row+=bytes_per_row){
		  rowDest=bytes_data+bytes_per_row*n;
	    memcpy( (void*)row, (void*)rowDest,  ncolumns);
	  }
	}
  data_size=1;
  textLineSize=0;
}

// Fills a bitmap with the given value
void GBitsetContour::fill(bool value)
{
  for(unsigned int y=0; y<rows(); y++)
	{
	  unsigned char* bm_y = (*this)[y];
	  for(unsigned int x=0; x<columns(); x++)
		bm_y[x] = value;
	}
}


void GBitsetContour::set_sqrt_array(){
	
	for(int i=0;i<16000;i++){
		sqrt_buff[i]=(int)sqrt((float)i);
	}
}//______________________________

int GBitsetContour::sqrtA(int data){
	if(data<16000){return sqrt_buff[data];}else{return (int)sqrt((float)data);}
	
}//___________________________


