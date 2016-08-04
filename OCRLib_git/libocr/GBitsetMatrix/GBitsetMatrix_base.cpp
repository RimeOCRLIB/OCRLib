#include "GBitsetMatrix.h"

// ----- constructor and destructor

GBitsetMatrix::~GBitsetMatrix(){}

void
GBitsetMatrix::destroy(void)
{
if ( data_size !=0 ) {
     free(bits_data);
#ifdef DEBUGLVL_DRAW
drawDataRGB->destroy();
#endif

    //  освобождение памяти
	if ( srcIntFlag){
        free(srcInt);
	    free(sInt);
        srcIntFlag=0;
    }    

    delete this;
//// освобождение памяти // GBitsetMatrix_base.h  // GBitsetMatrix::destroy(void)

}
}//_____________________________________________________________________________

GBitsetMatrix::GBitsetMatrix()
  : nrows(0), ncolumns(0), border(0), mask32Count(0),
	bits_per_row(0),data_size(0)
{
}

GBitsetMatrix::GBitsetMatrix(int nrows, int ncolumns, int border)
  : nrows(0), ncolumns(0), border(0), mask32Count(0),
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


GBitsetMatrix::GBitsetMatrix(const GBitsetMatrix *ref)
  : nrows(0), ncolumns(0), border(0), mask32Count(0),
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

GBitsetMatrix::GBitsetMatrix(const GBitmap *ref, short stepW, short stepH, short flipV)
  : nrows(0), ncolumns(0), border(0), mask32Count(0),
	bits_per_row(0),data_size(0)
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


GBitsetMatrix::GBitsetMatrix(const GBitmap *ref, short x, short y, short w, short h, short stepW, short stepH, short flipV)
	: nrows(0), ncolumns(0), border(0), mask32Count(0),
	bits_per_row(0),data_size(0)
{
   try
  {
	init(ref,x,y,w,h, stepW, stepH, flipV);
  }
  catch(int a)
  {
	destroy();
  }

}

GBitsetMatrix::GBitsetMatrix(const GBitsetMatrix *ref, int border)
  : nrows(0), ncolumns(0), border(0), mask32Count(0),
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
GBitsetMatrix::init(int arows, int acolumns, int aborder)
{
  aborder=aborder+(ncolumns%32)/2; //выравнивание битсета по 32 увеличивает скорость обращения
  nrows = arows + aborder*2;
  ncolumns = acolumns + aborder*2;
  srcIntFlag=0;
	 DM("nrows="<<nrows<<" ncolumns="<<ncolumns<<"arows="<<arows<<" acolumns="<<acolumns<<" aborder"<<aborder<<END);

  border = aborder;
  bits_per_row = ncolumns;
  data_size = (nrows * bits_per_row);
  if (data_size > 0)
	{

		bits_data=(bool*)malloc(data_size);

	  memset (bits_data,0,data_size);
	}

  ///////////////////////////////////////
  // запрос памяти с очисткой 0,

	srcInt=(unsigned int*)malloc((data_size+64)*4);  // переупакованный в int массив пикселей
	sInt=(unsigned int*)malloc((data_size+64)*4);    // транспонированый массив
    

  // выход в случае отсутствия свободной памяти запрашиваемого размера
  if (srcInt==NULL) {DM("srcInt==NULL"); exit (1);}
  if (sInt==NULL) {DM("sInt==NULL"); exit (1);}
  srcIntFlag=1;

}

void
GBitsetMatrix::init(const GBitsetMatrix *ref, int aborder)
{
  if (this != ref)    //Учесть aborder
	{
	  init(ref->nrows, ref->ncolumns, aborder);
	  bool *row = bits_data;
	  bool *rowDest;
	  for (int n=0; n<nrows; n++, row+=bits_per_row){
		rowDest=bits_data+bits_per_row*n;
		memcpy( (void*)row, (void*)rowDest,  (ncolumns+border*2)/8 );
	  }
	}
  //else if (aborder > border)
  //	{
  //	  minborder(aborder);
  //	}
	data_size=(ncolumns+border*2)*(nrows+border*2);
 // #define B BitsBuf

}


void GBitsetMatrix::setMask(GBitMask32 *srcMask, int maskSize){
	//memcpy(mask32[mask32Count].On, srcMask[0].On,128); //128=32*4
	//mask32[mask32Count]->status=srcMask->status;
	mask32[mask32Count]=srcMask;
	//printMask(mask32[mask32Count],c_out);
}//_____________________________________________________________________________


