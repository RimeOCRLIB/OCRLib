#include "GBitset.h"

void GBitset::init(const GBitmap *ref, short stepW, short stepH, short flipV){
	  border=64;
// srcW/nfas расстояние между срезами в пикселях, с учетом размера печа srcW=ref.columns();
// srcH/size вертикальный коэффициент масштаба с учетом размера печа  srcH=ref.rows();
	
	if(!ref->columns()||!ref->rows())return;
	  srcW=ref->columns()+border*2;  //  необходимы для вычисление угла в градусах
	  srcH=ref->rows()+border*2;     //  необходимы для вычисление угла в градусах
	  if(stepH<1)stepH=1;
	  if(stepW<1)stepW=1;
	  nrows=ref->rows()/stepH;
	  ncolumns=ref->columns()/stepW;
	  init(nrows,ncolumns, border);
	  bool *row=bits_data;
	  bool *p,*p1;
	  int dH,dW;
	  int h=ref->rows();
#ifdef DEBUGLVL_GREY    
	  drawData=GBitmap::create(ncolumns,nrows);
#endif
#ifdef DEBUGLVL_DRAW
    drawDataRGB=GBitmap::create(ncolumns,nrows,4);
#endif

if(flipV){

	for (int n=border; n<nrows-border; n++){
		p1=row+n*ncolumns;  dH=h-(n-border)*stepH;
		for(int m=border; m<ncolumns-border;m++){
			p=p1+m;   dW=(m-border)*stepW;
			if(ref[0][dH][dW]<125){*p=1; //BitVec[row+n*ncolumns+m]=1;
#ifdef DEBUGLVL_GREY
			  drawData[0][n][m]=0;       //DM(1);  // 0-черное, 255-белое ?
#endif
#ifdef DEBUGLVL_DRAW
              drawDataRGB->put(m,n,0x000000FF);       //DM(1);  // 0-черное, 255-белое ?
#endif
                
			}
			else{*p=0;
#ifdef DEBUGLVL_GREY
			  drawData[0][n][m]=255;     //DM(0);
#endif
#ifdef DEBUGLVL_DRAW
              drawDataRGB->put(m,n,0xFFFFFFFF);       //DM(1);  // 0-черное, 255-белое ?
#endif
                
			}

		}
	 //DM(END);
	 }

/*
//	    -- draw picture in text graphic ---
	   bool *d1;
	   for (int y=0; y < nrows; y+=4 ) {
		  for(int x=0;x<ncolumns;x+=32){
		  d1=bits_data+y*ncolumns+x;
		  if(*d1){DM(1);}else{DM(0);
		  }
	   }DM(END);
	   }DM(" NEW"<<END<<END<<END);
*/

}else{
	for (int n=border; n<nrows-border; n++){
		p1=row+n*ncolumns; dH=(n-border)*stepH;
		for(int m=border; m<ncolumns-border;m++){
			p=p1+m;  dW=(m-border)*stepW;
			if(ref[0][dH][dW]<125){*p=1; //BitVec[row+n*ncolumns+m]=1;
#ifdef DEBUGLVL_GREY
			  drawData[0][n][m]=0;
#endif
#ifdef DEBUGLVL_DRAW
              drawDataRGB->put(m,n,0x000000FF);       //DM(1);  // 0-черное, 255-белое ?
#endif

			}else{*p=0;
#ifdef DEBUGLVL_GREY
			  drawData[0][n][m]=255;
#endif
#ifdef DEBUGLVL_DRAW
              drawDataRGB->put(m,n,0xFFFFFFFF);       //DM(1);  // 0-черное, 255-белое ?
#endif
                
			}
		}
		//_//_DM(END);
	 }

}
  // Временный массив для тестирования быстродействия (bool*)farcalloc(nbits+512,1)
  int nbits=nrows*ncolumns;

	BitsBuf=(bool*)malloc(nbits+512); // B=	

  if (BitsBuf==NULL) exit (1);
  for (int x=0; x < data_size; x++ ) { BitsBuf[x]=bits_data[x]; }
  // #define B BitsBuf

}//__________________________________________

void
GBitset::initLevel(const GBitmap *ref) {


	  srcW=ref->columns();

	  srcH=ref->rows();
	  nrows=ref->rows();
	  ncolumns=ref->columns();
	  init(nrows,ncolumns, 0);
	  bool *row=bits_data;
	  bool *p;

   
     GBitmap *draw=GBitmap::create(ncolumns,nrows);
    
      for (int n=0; n<nrows; n++){
        for(int m=0; m<ncolumns;m++){
        p=row+n*ncolumns+m;
		if(ref[0][n][m]<125){*p=1;
        }else{*p=0;}
        draw[0][n][m]=ref[0][n][m];
      }}


short BASE=128;


      // Check arguments and init variables
short IMAGE_WIDTH =ref->columns();
short IMAGE_HEIGHT =ref->rows();
short S=IMAGE_WIDTH/BASE;
float Tr=0.9f;

	unsigned long* integralImg = 0;
		unsigned char* input=ref->bytes_data;   
		unsigned char* output=draw->bytes_data;
	int i, j;
	long sum=0;
	int count=0;
	int index;
	int x1, y1, x2, y2;
	int s2 = S/2;
        //float k;

	// create the integral image
	integralImg = (unsigned long*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(unsigned long*));
        if(integralImg){
	for (i=0; i<IMAGE_WIDTH; i++)
	{
		// reset this column sum
		sum = 0;

		for (j=0; j<IMAGE_HEIGHT; j++)
		{
			index = j*IMAGE_WIDTH+i;

			sum += input[index];
			if (i==0)
				integralImg[index] = sum;
			else
				integralImg[index] = integralImg[index-1] + sum;
		}
	}

	// perform thresholding
	for (i=0; i<IMAGE_WIDTH; i++)
	{
		for (j=0; j<IMAGE_HEIGHT; j++)
		{
			index = j*IMAGE_WIDTH+i;

			// set the SxS region
			x1=i-s2; x2=i+s2;
			y1=j-s2; y2=j+s2;

			// check the border
			if (x1 < 0) x1 = 0;
			if (x2 >= IMAGE_WIDTH) x2 = IMAGE_WIDTH-1;
			if (y1 < 0) y1 = 0;
			if (y2 >= IMAGE_HEIGHT) y2 = IMAGE_HEIGHT-1;

			count = (x2-x1)*(y2-y1);

			// I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
			sum = integralImg[y2*IMAGE_WIDTH+x2] -
				  integralImg[y1*IMAGE_WIDTH+x2] -
				  integralImg[y2*IMAGE_WIDTH+x1] +
				  integralImg[y1*IMAGE_WIDTH+x1];

			//if ((long)(input[index]*count) < (long)(sum*(1.0-Tr)))
			//	input[index] = 0;
			//else
			//	input[index] = 255;

                        //k=((long)(input[index]*count))/((long)(sum));
                        if((long)(input[index]*count)>(long)(sum*Tr)){
                        output[index]=255;
                        }else{output[index]=0;}
		}
	}
	free (integralImg);
		}
   data_size= ncolumns*nrows;
     for (int x=0; x < ncolumns*nrows; x++ ) { BitsBuf[x]=bits_data[x]; }
  // #define B BitsBuf
    draw->destroy();

}//_____________________________________________________________________________



