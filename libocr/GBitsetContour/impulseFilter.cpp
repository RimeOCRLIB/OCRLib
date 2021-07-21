#include "GBitsetContour.h"


void GBitsetContour::impulseFilter(char* angFilter){
	/////////*filter///////////////////////////*
	//short angLength=angFilter.size();
//ShowMessage(sizeH);	
if(sizeH>31){	
	//VERT filter (3 Pixel)
	char ft1[7]={-1,-1,0,4,0,-1,-1};	impFilter7(ft1, angFilter, angLength);
	//VERT filter (2 Pixel)
	char ft2[5]={-1,-1,4,-1,-1};		impFilter5(ft2, angFilter, angLength);
	
	//FREE FILTER//
	//char ft3[10]={-1,0,3,1,-1,-1,0,0,0,0};  impFilter10(ft3, angFilter, angLength);
	//char ft4[10]={0,0,0,0,-1,-1,1,3,0,-1};  impFilter10(ft4, angFilter, angLength);
	
	//////////////Meandr  
	char ft5[7]={-1,2,-2,2,-2,2,-1};	impFilter7(ft5, angFilter, angLength);
	char ft6[7]={1,-2,2,-2,2,-2,1};		impFilter7(ft6, angFilter, angLength);
	
	char ft7[6]={-1,2,-2,2,-2,1};	impFilter6(ft7, angFilter, angLength);
	char ft8[6]={1,-2,2,-2,2,-1};	impFilter6(ft8, angFilter, angLength);
	
	char ft9[5]={-1,2,-2,2,-1};	impFilter5(ft9, angFilter, angLength);
	char ft10[5]={1,-2,2,-2,1};	impFilter5(ft10, angFilter, angLength);
	
	char ft11[4]={-1,2,-2,1};		impFilter4(ft11, angFilter, angLength);
	char ft12[4]={1,-2,2,-1};		impFilter4(ft12, angFilter, angLength);
	
	char ft13[3]={-1,2,-1};		impFilter3(ft13, angFilter, angLength);
	char ft14[3]={1,-2,1};		impFilter3(ft14, angFilter, angLength);
	
	char ft15[4]={1,-1,1,-1};		impFilter4(ft15, angFilter, angLength);
	char ft16[4]={-1,1,-1,1};		impFilter4(ft16, angFilter, angLength);
	///////////////////////////
	
	char ft17[5]={1,0,-2,0,1};		impFilter5(ft17, angFilter, angLength);
	char ft18[5]={-1,0,2,0,-1};	impFilter5(ft18, angFilter, angLength);
	char ft19[5]={1,0,2,2,-1};		impFilter5(ft19, angFilter, angLength);
	char ft20[5]={-1,0,-2,-2,1};	impFilter5(ft20, angFilter, angLength);
	
	/////////angle 180*
	char ft21[5]={-1,2,2,0,1};		impFilter5(ft21, angFilter, angLength);
	char ft22[5]={1,-2,-2,0,-1};	impFilter5(ft22, angFilter, angLength);
	char ft23[5]={1,2,2,0,-1};		impFilter5(ft23, angFilter, angLength);
	char ft24[5]={-1,-2,-2,0,1};	impFilter5(ft24, angFilter, angLength);
	
	////////angle 90* 
	char ft25[5]={1,-1,3,-1,1};	impFilter5(ft25, angFilter, angLength);
	char ft26[5]={-1,1,-3,1,-1};	impFilter5(ft26, angFilter, angLength);
	
	char ft27[4]={0,-1,-2,1};		impFilter4(ft27, angFilter, angLength);
	char ft28[4]={0,1,2,-1};		impFilter4(ft28, angFilter, angLength);
	char ft29[4]={1,-2,-1,0};		impFilter4(ft29, angFilter, angLength);
	char ft30[4]={-1,2,1,0};		impFilter4(ft30, angFilter, angLength);
	
	// 90* angle
	char ft31[4]={1,-1,3-1};		impFilter4(ft31, angFilter, angLength);
	char ft32[4]={-1,1-3,1};		impFilter4(ft32, angFilter, angLength);
	char ft33[4]={-1,3-1,1};		impFilter4(ft33, angFilter, angLength);
	char ft34[4]={1,-3,1-1};		impFilter4(ft34, angFilter, angLength);
	
	char ft35[5]={-1,0,4,0,-1};	impFilter5(ft35, angFilter, angLength);
	char ft36[5]={1,0,-4,0,1};		impFilter5(ft36, angFilter, angLength);
	char ft37[3]={-1,4,-1};		impFilter3(ft37, angFilter, angLength);
	char ft38[3]={1,-4,1};		impFilter3(ft38, angFilter, angLength);
	
	char ft39[3]={1,3,-1};		impFilter3(ft39, angFilter, angLength);
	char ft40[3]={-1,-3,1};		impFilter3(ft40, angFilter, angLength);
	char ft41[3]={-1,3,1};		impFilter3(ft41, angFilter, angLength);
	char ft42[3]={1,-3,-1};		impFilter3(ft42, angFilter, angLength);
	
	char ft43[6]={2,1,-2,-1,-1,1};  impFilter6(ft43, angFilter, angLength);
	char ft44[6]={1,-1,-1,-2,1,2};  impFilter6(ft44, angFilter, angLength);
	
	//corner
	//sharp angle 135*
	char ft45[5]={1,-1,-2,0,-1};	impFilter5(ft45, angFilter, angLength);
	char ft46[5]={-1,1,2,0,1};		impFilter5(ft46, angFilter, angLength);
	char ft47[5]={-1,0,-2,-1,1};	impFilter5(ft47, angFilter, angLength);
	char ft48[5]={1,0,2,1,-1};		impFilter5(ft48, angFilter, angLength);
	
	char ft49[3]={-1,2,1};		impFilter3(ft49, angFilter, angLength);
	char ft50[3]={1,-2,-1};		impFilter3(ft50, angFilter, angLength);
	char ft51[3]={1,2,-1};		impFilter3(ft51, angFilter, angLength);
	char ft52[3]={-1,-2,1};		impFilter3(ft52, angFilter, angLength);
	
	char ft53[4]={1,0,2,-1};		impFilter4(ft53, angFilter, angLength);
	char ft54[4]={-1,0,-2,1};		impFilter4(ft54, angFilter, angLength);
	char ft55[4]={-1,2,0,1};		impFilter4(ft55, angFilter, angLength);
	char ft56[4]={1,-2,0,-1};		impFilter4(ft56, angFilter, angLength);
	
	char ft57[3]={4,0,-1};		impFilter3(ft57, angFilter, angLength);
	char ft58[3]={-4,0,1};		impFilter3(ft58, angFilter, angLength);
	char ft59[3]={-1,0,4};		impFilter3(ft59, angFilter, angLength);
	char ft60[3]={1,0,-4};		impFilter3(ft60, angFilter, angLength);
	char ft61[2]={-1,4};			impFilter2(ft61, angFilter, angLength);
	char ft62[2]={1,-4};			impFilter2(ft62, angFilter, angLength);
	char ft63[2]={4,-1};			impFilter2(ft63, angFilter, angLength);
	char ft64[2]={-4,1};			impFilter2(ft64, angFilter, angLength);
	
	//angle 180*
	char ft65[3]={-1,2,3};		impFilter3(ft65, angFilter, angLength);
	char ft66[3]={1,-2,-3};		impFilter3(ft66, angFilter, angLength);
	char ft67[3]={3,2,-1};		impFilter3(ft67, angFilter, angLength);
	char ft68[3]={-3,-2,1};		impFilter3(ft68, angFilter, angLength);
	
	//angl 90*
	char ft69[4]={-1,2,2,-1};		impFilter4(ft69, angFilter, angLength);
	char ft70[4]={1,-2,-2,1};		impFilter4(ft70, angFilter, angLength);
	
	char ft71[3]={1,-2,3};		impFilter3(ft71, angFilter, angLength);
	char ft72[3]={-1,2,-3};		impFilter3(ft72, angFilter, angLength);
	char ft73[3]={3,-2,1};		impFilter3(ft73, angFilter, angLength);
	char ft74[3]={-3,2,-1};		impFilter3(ft74, angFilter, angLength);
	
	//VERT filter (1Pixel)
	char ft75[3]={-1,2,-1};		impFilter3(ft75, angFilter, angLength);
	char ft76[3]={1,-2,1};		impFilter3(ft76, angFilter, angLength);
	
	char ft77[3]={-1,2,0};		impFilter3(ft77, angFilter, angLength);
	char ft78[3]={1,-2,0};		impFilter3(ft78, angFilter, angLength);
	char ft79[3]={0,2,-1};		impFilter3(ft79, angFilter, angLength);
	char ft80[3]={0,-2,1};		impFilter3(ft80, angFilter, angLength);
	
	char ft81[3]={-2,3,-1};		impFilter3(ft81, angFilter, angLength);
	char ft82[3]={2,-3,1};		impFilter3(ft82, angFilter, angLength);
	char ft83[3]={-1,3,-2};		impFilter3(ft83, angFilter, angLength);
	char ft84[3]={1,-3,2};		impFilter3(ft84, angFilter, angLength);
	
	////angle 135*
	char ft85[3]={2,2,-1};		impFilter3(ft85, angFilter, angLength);
	char ft86[3]={-2,-2,1};		impFilter3(ft86, angFilter, angLength);
	char ft87[3]={-1,2,2};		impFilter3(ft87, angFilter, angLength);
	char ft88[3]={1,-2,-2};		impFilter3(ft88, angFilter, angLength);
	
	// START filter ()
	char ft89[2]={1,-1};			impFilter2(ft89, angFilter, angLength);
	char ft90[2]={-1,1};			impFilter2(ft90, angFilter, angLength);
	
	// MEANDR 90gr filter (1Pixel)
	char ft91[2]={2,-2};			impFilter2(ft91, angFilter, angLength);
	char ft92[2]={-2,2};			impFilter2(ft92, angFilter, angLength);
	
	
	// STEP  filter (2*2Pixel)
	char ft93[6]={-1,0,1,1,0,-1};	impFilter6(ft93, angFilter, angLength);
	char ft94[6]={1,0,-1,-1,0,1};	impFilter6(ft94, angFilter, angLength);
	
	char ft95[6]={-1,-1,2,2,-1,-1};impFilter6(ft95, angFilter, angLength);
	char ft96[6]={1,1,-2,-2,1,1};	impFilter6(ft96, angFilter, angLength);
	
	char ft97[6]={-1,0,1,2,-1,-1};	impFilter6(ft97, angFilter, angLength);
	char ft98[6]={1,0,-1,-2,1,1};	impFilter6(ft98, angFilter, angLength);
	
	char ft99[6]={-1,-1,2,1,0,-1};	impFilter6(ft99, angFilter, angLength);
	char ft100[6]={1,1,-2,-1,0,1};	impFilter6(ft100, angFilter, angLength);
	
	
	//////Strong step //On GOOD text only
	//char ft101[3]={-1,0,1};		impFilter3(ft101, angFilter, angLength);
	//char ft102[3]={1,0,-1};		impFilter3(ft102, angFilter, angLength);
	////////////////////////////////////////////////
	
	//SHARP/// //On GOOD text only
	//char ft103[2]={1,1};			impFilter2(ft103, angFilter, angLength);
	//char ft104[2]={-1,-1};		impFilter2(ft104, angFilter, angLength);
	
	
	//On GOOD text only
	//char ft105[3]={1,0,1};		impFilter3(ft105, angFilter, angLength);
	//char ft106[3]={-1,0,-1};		impFilter3(ft106, angFilter, angLength);
	
	// MEANDR 90gr filter (1Pixel) REPEAT
	char ft107[2]={1,-1};		impFilter2(ft107, angFilter, angLength);
	char ft108[2]={-1,1};		impFilter2(ft108, angFilter, angLength);
	// MEANDR 90gr filter (1Pixel)REPEAT
	char ft109[2]={2,-2};		impFilter2(ft109, angFilter, angLength);
	char ft110[2]={-2,2};		impFilter5(ft110, angFilter, angLength);
	
	///////////angle 90*
	char ft111[3]={3,0,-1};		impFilter3(ft111, angFilter, angLength);
	char ft112[3]={-3,0,1};		impFilter3(ft112, angFilter, angLength);
	char ft113[3]={-1,0,3};		impFilter3(ft113, angFilter, angLength);
	char ft114[3]={1,0,-3};		impFilter3(ft114, angFilter, angLength);
	
	char ft115[2]={3,-1};		impFilter2(ft115, angFilter, angLength);
	char ft116[2]={-3,1};		impFilter2(ft116, angFilter, angLength);
	char ft117[2]={-1,3};		impFilter2(ft117, angFilter, angLength);
	char ft118[2]={1,-3};		impFilter2(ft118, angFilter, angLength);
	
	///////////////sharp angle 135*
	char ft119[2]={1,2};			impFilter2(ft119, angFilter, angLength);
	char ft120[2]={-1,-2};		impFilter2(ft120, angFilter, angLength);
	char ft121[2]={2,1};			impFilter2(ft121, angFilter, angLength);
	char ft122[2]={-2,-1};		impFilter2(ft122, angFilter, angLength);
	////////////////////////////////
	
	///////////////sharp angle 180*
	char ft123[2]={1,3};			impFilter2(ft123, angFilter, angLength);
	char ft124[2]={-1,-3};		impFilter2(ft124, angFilter, angLength);
	char ft125[2]={3,1};			impFilter2(ft125, angFilter, angLength);
	char ft126[2]={-3,-1};		impFilter2(ft126, angFilter, angLength);
	////////////////////////////////
	
	char ft127[3]={0,2,2};		impFilter3(ft127, angFilter, angLength);
	char ft128[3]={2,2,0};		impFilter3(ft128, angFilter, angLength);

	char ft129[2]={2,2};			impFilter2(ft129, angFilter, angLength);
	char ft130[2]={-2,-2};		impFilter2(ft130, angFilter, angLength);
}
	
	if (angLength>40){
		for(i=0;i<angLength+19;i++){
			if(angFilter[i]!=0&&angFilter[i+1]!=0){angFilter[i]=angFilter[i]+angFilter[i+1];angFilter[i+1]=0;}  //remove double point
		}
		//end point
		if(angFilter[10]==0&&angFilter[11]!=0){angFilter[10]=angFilter[11];angFilter[11]=0;}
		if(angFilter[angLength-12]!=0&&angFilter[angLength-11]==0){angFilter[angLength-11]=angFilter[angLength-12];angFilter[angLength-12]=0;}
	}
	
	
}/////////////////////////////////////////*

