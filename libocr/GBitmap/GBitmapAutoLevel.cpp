//  Copyright (c) Open Source Buddhism Library 2020 www.buddism.ru.
//****************************************************************************
//  NAMO TASSA BHAGAVATO ARAHATO SAMMA SAMBUDDHASSA
//  May Buddha Dharma bring benefit and peace in all the world.
//****************************************************************************
//
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .

#include "GBitmap.h"

void drawHist(int h[]){
  int max=0;
  for(int i=0;i<256;i++){
    if(h[i]>max)max=h[i];
  }
  float k=(float)100/max;
  for(int i=0;i<256;i++){
    int c=(int)((float)h[i]*k);
    cout<<i<<" "<<h[i];
    for(int n=0;n<c;n++)cout<<".";
    cout<<endl;
  }
  cout<<endl;
}
void drawHist(float h[]){
  float max=0;
  for(int i=0;i<256;i++){
    if(h[i]>max)max=h[i];
  }
  double k=(float)300/max;
  for(int i=0;i<256;i++){
    int c=(int)((float)h[i]*k);
    cout<<i<<" "<<h[i];
    for(int n=0;n<c&&n<300;n++)cout<<".";
    cout<<endl;
  }
  cout<<endl;
}

//*****************************************************************************
namespace ocr{
  
  // функция анализирует гистограмму изображения и нормализует изображение по гистограмме.
  void GBitmap::autoLevel(){
    
    unsigned long ctr=0;
    float eq_data[256];	// keep probability data
    memset(eq_data,0,256*4);	// initialize all cells with zeroes
    int pal[256];	// keep probability data
    memset(pal,0,256*4);	// initialize all cells with zeroes
    
    
    ctr=0;
    uchar *p=bytes_data;
    //заполняем гисограмму
    while(ctr<data_size) {
      eq_data[*(p+ctr)]++;	// inc cell, corresponding to file color-index-no, in 'eq_data' by 1
      ctr++;					// increment counter
    }
    
    for(ctr=0;ctr<256;ctr++)	eq_data[ctr] /= (float)ncolumns; // p.d.f. (part I) (Probability Density Function)
    for(ctr=0;ctr<256;ctr++)	eq_data[ctr] /= (float)nrows; 	 // p.d.f. (part II)
    for(ctr=0;ctr<256;ctr++)	eq_data[ctr] *= 255;			 // complete Transformation function
    for(ctr=1;ctr<256;ctr++)	eq_data[ctr] += eq_data[ctr-1];  // c.d.f. (Cummulative Density Function)
    for(ctr=0;ctr<256;ctr++)	pal[ctr]=(uchar)eq_data[ctr]>>2; // set palette_buffer to return new colors for old graylevel nos.
    
    //drawHist(pal);
    ctr=0;
    while(ctr<data_size){
      *(p+ctr)=(*(p+ctr)+pal[*(p+ctr)])/2;
      ctr++;
    }
  } //_____________________________________________________________________________
  
  
  /*
   
   int main()
   {
   // Load the image
   Mat image = imread("scene.jpg", CV_LOAD_IMAGE_GRAYSCALE);
   
   // Generate the histogram
   int histogram[256];
   imhist(image, histogram);
   
   // Caluculate the size of image
   int size = image.rows * image.cols;
   float alpha = 255.0/size;
   
   // Calculate the probability of each intensity
   float PrRk[256];
   for(int i = 0; i < 256; i++)
   {
   PrRk[i] = (double)histogram[i] / size;
   }
   
   // Generate cumulative frequency histogram
   int cumhistogram[256];
   cumhist(histogram,cumhistogram );
   
   // Scale the histogram
   int Sk[256];
   for(int i = 0; i < 256; i++)
   {
   Sk[i] = cvRound((double)cumhistogram[i] * alpha);
   }
   
   
   // Generate the equlized histogram
   float PsSk[256];
   for(int i = 0; i < 256; i++)
   {
   PsSk[i] = 0;
   }
   
   for(int i = 0; i < 256; i++)
   {
   PsSk[Sk[i]] += PrRk[i];
   }
   
   int final[256];
   for(int i = 0; i < 256; i++)
   final[i] = cvRound(PsSk[i]*255);
   
   
   // Generate the equlized image
   Mat new_image = image.clone();
   
   for(int y = 0; y < image.rows; y++)
   for(int x = 0; x < image.cols; x++)
   new_image.at<uchar>(y,x) = saturate_cast<uchar>(Sk[image.at<uchar>(y,x)]);
   
   
   */
  
}
