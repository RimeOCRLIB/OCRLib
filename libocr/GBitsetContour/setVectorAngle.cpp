#include "GBitsetContour.h"


short GBitsetContour::setVectorAngle(short x0, short y0, short x1, short y1){

int s=abs((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));

double cos=0;
if(s)cos=fabs((float)(x1-x0)/sqrtA(s));
short angle;
if(cos>-1&&cos<=1)angle=(short)(acos(cos)*57);
else angle=90;

// y=0     270
//         2   |  1
//  180---0---0
//         3   |  4
//y=1     90

if(x1>=x0){
	if(y1<=y0){// [1] sector
	}else{// [4] sector
	angle=360-angle;}
}else{
	if(y1>=y0){// [3] sector
	angle+=180;
	}else{// [2] sector
	angle=180-angle;
	}
}
return angle;
}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
