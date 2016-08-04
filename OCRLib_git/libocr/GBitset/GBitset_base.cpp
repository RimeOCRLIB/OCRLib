#include "GBitset.h"
// ----- constructor and destructor

GBitset::~GBitset(){
}

void
GBitset::destroy(void){
	if ( data_size !=0 ) {   
		free(bits_data); 
#ifdef DEBUGLVL_GRAY        
	drawData->destroy();    
#endif
        
	data_size=0;
	//  освобождение памяти
			if ( ImBuf !=NULL ) free(ImBuf);
			if ( ReBuf !=NULL ) free(ReBuf);
			if ( BufIn !=NULL ) free(BufIn);
			if ( BufOu !=NULL ) free(BufOu);
		//// освобождение памяти // GBitset_base.h  // GBitset::destroy(void)
		if ( BitsBuf !=NULL ) free(BitsBuf);
		
	}
    //delete this;
}

GBitset::GBitset()
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
}

GBitset::GBitset(int nrows, int ncolumns, int border)
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


GBitset::GBitset(const GBitset *ref)
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

GBitset::GBitset(const GBitmap *ref, short stepW, short stepH, short flipV)
  : nrows(0), ncolumns(0), border(0),
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

GBitset::GBitset(const GBitmap *ref, const GBitmap *ref1)
  : nrows(0), ncolumns(0), border(0),
	bits_per_row(0),data_size(0)
{
  try
  {
	initLevel(ref);
  }
  catch(int a)
  {
	destroy();
  }
}


GBitset::GBitset(const GBitset *ref, int border)
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
GBitset::init(int arows, int acolumns, int aborder)
{
  //c_out.open("c:/consoleBit.txt");
  DM("start_bit"<<END);
  destroy();
  aborder=aborder+(ncolumns%32)/2; //выравнивание битсета по 32 увеличивает скорость обращения
  nrows = arows + aborder*2;
  ncolumns = acolumns + aborder*2;

  DM("nrows="<<nrows<<" ncolumns="<<ncolumns<<"arows="<<arows<<" acolumns="<<acolumns<<" aborder"<<aborder<<END);

  border = aborder;
  bits_per_row = ncolumns;
  int nbits = (nrows * bits_per_row);
  if (nbits > 0)
	{
	  bits_data=(bool*)malloc(nbits);
	  memset (bits_data,0,nbits);
	}

  // Размер массивов равен наибольшему из ncolumns и nrows те из ширины и высоты
  ncnr=ncolumns;   // ncnr глобальная
  if ( nrows>=ncolumns ) ncnr=nrows;
  HBuf.resize(ncnr+10);
  GBuf.resize(ncnr+10);

  BufS.resize(ncnr+10);
  BufT.resize(ncnr+10);
  BufR.resize(ncnr+10);
  BufL.resize(ncnr+10);
  BufHelp.resize(ncnr+10);

  HStr.resize(ncnr+10);
  HStS.resize(ncnr+10);
  LStr.resize(ncnr+10);
  LStS.resize(ncnr+10);
  data_size=ncolumns*nrows;

  SP=OCRFilter::create(ncnr);   //создание и инициализация блока фильтров

  ///////////////////////////////////////
  // запрос памяти с очисткой 0, 512(+16) это запас объема буферных массивов для применения фильтров 
  // szf=sizeof(int);         // level.h
  szf=sizeof(int);            // szf=4;
	
	ImBuf=(int*)malloc((ncnr+512+16)*szf);
	ReBuf=(int*)malloc((ncnr+512+16)*szf);
	BufIn=(int*)malloc((ncnr+512+16)*szf);
	BufOu=(int*)malloc((ncnr+512+16)*szf);
///////////////////////////////////////

  // выход в случае отсутствия свободной памяти запрашиваемого размера
  if (ImBuf==NULL) {DM("ImBuf");exit (1);}
  if (ReBuf==NULL) {DM("ReBuf");exit (1);}
  if (BufIn==NULL) {DM("BufIn");exit (1);}
  if (BufOu==NULL) {DM("BufOu");exit (1);}

}

void
GBitset::init(const GBitset *ref, int aborder)
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
	for (int x=0; x < data_size; x++ ) { BitsBuf[x]=bits_data[x]; }
  // #define B BitsBuf

}


// Fills a bitmap with the given value
void
GBitset::fill(bool value)
{
  for(unsigned int y=0; y<rows(); y++)
	{
	  bool* bm_y = (*this)[y];
	  for(unsigned int x=0; x<columns(); x++)
		bm_y[x] = value;
	}
}

GBitset*
GBitset::rotate(int count)
{
GBitset* newbitmap=this;
/*
  count = count & 3;
  if(count)
  {
	if( count & 0x01 )
	{
	  newbitmap = new GBitset(ncolumns, nrows);
	}else
	{
	  newbitmap = new GBitset(nrows, ncolumns);
	}
	GMonitorLock lock(monitor());
	GBitset &dbitmap = *newbitmap;
	switch(count)
	{
	case 3: // rotate 90 counter clockwise
	  {
		const int lastrow = dbitmap.rows()-1;
		for(int y=0; y<nrows; y++)
		{
		  const bool *r=operator[] (y);
		  for(int x=0,xnew=lastrow;xnew>=0; x++,xnew--)
		  {
			dbitmap[xnew][y] = r[x];
		  }
		}
	  }
	  break;
	case 2: // rotate 180 counter clockwise
	  {
		const int lastrow = dbitmap.rows()-1;
		const int lastcolumn = dbitmap.columns()-1;
		for(int y=0,ynew=lastrow;ynew>=0; y++,ynew--)
		{
		  const bool *r=operator[] (y);
		  bool *d=dbitmap[ynew];
		  for(int xnew=lastcolumn;xnew>=0; r++,--xnew)
		  {
			d[xnew] = *r;
		  }
		}
	  }
	  break;
	case 1: // rotate 270 counter clockwise
	  {
		const int lastcolumn = dbitmap.columns()-1;
		for(int y=0,ynew=lastcolumn;ynew>=0;y++,ynew--)
		{
		  const bool *r=operator[] (y);
		  for(int x=0; x<ncolumns; x++)
		  {
			dbitmap[x][ynew] = r[x];
		  }
		}
	  }
	  break;
	}
  }
*/
  return newbitmap;
}

#ifndef NDEBUG
void
GBitset::check_border() const
{int col ;
  if (bits_data)
	{
	  const bool *p = (*this)[-1];
	  for (col=-border; col<ncolumns+border; col++)
		if (p[col])exit(0);
	  for (int row=0; row<nrows; row++)
		{
		  p = (*this)[row];
		  for (col=-border; col<0; col++)
			if (p[col])exit(0);
		  for (col=ncolumns; col<ncolumns+border; col++)
			if (p[col])exit(0);
		}
	}
}


#endif

