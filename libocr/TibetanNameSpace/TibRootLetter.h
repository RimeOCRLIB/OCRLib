void TibRootLetter(vector<rootLetterOCR>&rootLetter,
				   vector<rootLetterOCR>&vowelLetter,
                   vector<rootLetterOCR>&tagLetter
                   ){

short l;
rootLetterOCR record;
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter.push_back(record);

rootLetter[1].letter="k";
     short lb1[]={0,1,9,16,20,25,27,29,31,30,38};
     for(l=0;l<11;l++)rootLetter[1].lbase.push_back(lb1[l]);

    rootLetter[1].lbase1.push_back(0);
    rootLetter[1].lbase1.push_back(27);
    rootLetter[1].lbase1.push_back(37);
        rootLetter[1].lbase2.push_back(0);

     short lj1[]={0,1,2,3,4,9,10,11,13,14,15,16,17,18,19,20,25,27,30,31,32,34,35,37,38};
     for(l=0;l<25;l++)rootLetter[1].lj.push_back(lj1[l]);
           short lsj1[]={0,9,10,12,16,17,20,25,27,28,29};
           for(l=0;l<11;l++)rootLetter[1].lsj.push_back(lsj1[l]);
                     rootLetter[1].lsj.push_back(0);
                     rootLetter[1].lsj.push_back(20);
                     rootLetter[1].lssj.push_back(25);
                         rootLetter[1].lm.push_back(0);

//______________________________________________________________________________
rootLetter.push_back(record);

rootLetter[2].letter="kh";
rootLetter[2].lbase.push_back(0);
rootLetter[2].lbase.push_back(20);
rootLetter[2].lbase.push_back(25);
rootLetter[2].lbase.push_back(27);

    rootLetter[2].lbase1.push_back(0);
        rootLetter[2].lbase2.push_back(0);


     short lj2[]={0,2,12,20,25,27,29,31,32,38};
     for(l=0;l<10;l++)rootLetter[2].lj.push_back(lj2[l]);
           short lsj2[]={0,25};
           for(l=0;l<2;l++)rootLetter[2].lsj.push_back(lsj2[l]);
                    rootLetter[2].lssj.push_back(0);
                         rootLetter[2].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[3].letter="g";
     short lb3[]={0,3,20,25,27,29,32};
     for(l=0;l<7;l++)rootLetter[3].lbase.push_back(lb3[l]);
     rootLetter[3].lbase1.push_back(0);
     rootLetter[3].lbase1.push_back(12);
     rootLetter[3].lbase1.push_back(27);
     rootLetter[3].lbase2.push_back(0);


     short lj3[]={0,3,4,9,11,12,15,16,17,19,20,25,26,27,30,32,37};
     for(l=0;l<17;l++)rootLetter[3].lj.push_back(lj3[l]);
           short lsj3[]={0,20,25,27,32,31,37};
           for(l=0;l<7;l++)rootLetter[3].lsj.push_back(lsj3[l]);
                     rootLetter[3].lssj.push_back(0);
                     rootLetter[3].lssj.push_back(25);
                          rootLetter[3].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[4].letter="ng";
     short lb4[]={0,1,3,9,11,32};
     for(l=0;l<6;l++)rootLetter[4].lbase.push_back(lb4[l]);

    rootLetter[4].lbase1.push_back(0);
    rootLetter[4].lbase1.push_back(32);
    rootLetter[4].lbase2.push_back(0);

     short lj4[]={0,1,2,3,4,8,9,16,20,26,27,30,31,32,38};
     for(l=0;l<14;l++)rootLetter[4].lj.push_back(lj4[l]);
           short lsj4[]={0,12,16,20,25,27,29,32,38};
           for(l=0;l<9;l++)rootLetter[4].lsj.push_back(lsj4[l]);
                     rootLetter[4].lssj.push_back(0);
                          rootLetter[4].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[5].letter="c";
rootLetter[5].lbase.push_back(0);
rootLetter[5].lbase.push_back(17);
rootLetter[5].lbase.push_back(20);

    rootLetter[5].lbase1.push_back(0);
    rootLetter[5].lbase2.push_back(0);

     short lj5[]={0,3,4,20,25};
     for(l=0;l<5;l++)rootLetter[5].lj.push_back(lj5[l]);
             rootLetter[5].lsj.push_back(0);
                     rootLetter[5].lssj.push_back(0);
                           rootLetter[5].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[6].letter="ch";
rootLetter[6].lbase.push_back(0);
rootLetter[6].lbase.push_back(20);

    rootLetter[6].lbase1.push_back(0);
    rootLetter[6].lbase2.push_back(0);

     short lj6[]={0,3,20,27,31};
     for(l=0;l<5;l++)rootLetter[6].lj.push_back(lj6[l]);
             rootLetter[6].lsj.push_back(0);
                     rootLetter[6].lssj.push_back(0);
                          rootLetter[6].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[7].letter="j";
rootLetter[7].lbase.push_back(0);
rootLetter[7].lbase.push_back(20);
rootLetter[7].lbase.push_back(27);

    rootLetter[7].lbase1.push_back(0);
    rootLetter[7].lbase2.push_back(0);

     rootLetter[7].lsj.push_back(0);
     rootLetter[7].lsj.push_back(27);
             rootLetter[7].lsj.push_back(0);
                     rootLetter[7].lssj.push_back(0);
                            rootLetter[7].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[8].letter="ny";
    short lb8[]={0,7,17,19,20};
     for(l=0;l<5;l++)rootLetter[8].lbase.push_back(lb8[l]);
     rootLetter[8].lbase1.push_back(0);
             rootLetter[8].lbase2.push_back(0);

     short lj8[]={0,10,13,14,17,18,19,20,25,27,30,37,38};
     for(l=0;l<13;l++)rootLetter[8].lj.push_back(lj8[l]);
          short lsj8[]={0,12,16,17,19,25,27};
          for(l=0;l<7;l++)rootLetter[8].lsj.push_back(lsj8[l]);
                     rootLetter[8].lssj.push_back(0);
                     rootLetter[8].lssj.push_back(20);
                     rootLetter[8].lssj.push_back(25);
                     rootLetter[8].lssj.push_back(32);
                             rootLetter[8].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[9].letter="t";
     short lb9[]={0,3,9,12,13,16,20,25,27,31};
     for(l=0;l<10;l++)rootLetter[9].lbase.push_back(lb9[l]);
     rootLetter[9].lbase1.push_back(0);
     rootLetter[9].lbase1.push_back(27);
             rootLetter[9].lbase2.push_back(0);

     short lj9[]={0,1,2,3,8,9,10,11,12,13,14,15,16,18,19,20,25,26,27,28,29,31,32,34,35,38};
     for(l=0;l<26;l++)rootLetter[9].lj.push_back(lj9[l]);
           short lsj9[]={0,1,9,12,13,14,16,20,25,26,27,32,38};
           for(l=0;l<13;l++)rootLetter[9].lsj.push_back(lsj9[l]);
                     rootLetter[9].lssj.push_back(0);
                     rootLetter[9].lssj.push_back(12);
                     rootLetter[9].lssj.push_back(25);
                     rootLetter[9].lssj.push_back(27);
                             rootLetter[9].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[10].letter="th";
rootLetter[10].lbase.push_back(0);
rootLetter[10].lbase.push_back(20);
rootLetter[10].lbase.push_back(27);
     rootLetter[10].lbase1.push_back(0);
             rootLetter[10].lbase2.push_back(0);


     short lj10[]={0,10,20,27,29,32};
     for(l=0;l<6;l++)rootLetter[10].lj.push_back(lj10[l]);
          short lsj10[]={0,27,32};
          for(l=0;l<3;l++)rootLetter[10].lsj.push_back(lsj10[l]);
                     rootLetter[10].lssj.push_back(0);
                              rootLetter[10].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[11].letter="d";
     short lb11[]={0,3,11,16,20,25,27,32};
     for(l=0;l<8;l++)rootLetter[11].lbase.push_back(lb11[l]);
     rootLetter[11].lbase1.push_back(0);
     rootLetter[11].lbase1.push_back(20);
     rootLetter[11].lbase1.push_back(27);
     rootLetter[11].lbase1.push_back(32);
             rootLetter[11].lbase2.push_back(0);
             rootLetter[11].lbase2.push_back(25);

     short lj11[]={0,1,3,4,7,8,10,11,12,15,16,17,19,20,21,25,27,28,29,31,32};
     for(l=0;l<21;l++)rootLetter[11].lj.push_back(lj11[l]);
           short lsj11[]={0,11,12,16,20,25,27,28,29,32,36};
           for(l=0;l<11;l++)rootLetter[11].lsj.push_back(lsj11[l]);
                     short lssj11[]={0,12,20,25,27,32};
                     for(l=0;l<6;l++)rootLetter[11].lssj.push_back(lssj11[l]);
                               rootLetter[11].lm.push_back(0);
                               rootLetter[11].lm.push_back(20);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[12].letter="n";
     short lb12[]={0,9,11,12,20,25,27,32,36};
     for(l=0;l<9;l++)rootLetter[12].lbase.push_back(lb12[l]);
     rootLetter[12].lbase1.push_back(0);
     rootLetter[12].lbase1.push_back(3);
     rootLetter[12].lbase1.push_back(27);
     rootLetter[12].lbase1.push_back(32);
             rootLetter[12].lbase2.push_back(0);

     short lj12[]={0,1,2,4,9,10,11,12,13,14,15,16,17,19,20,25,26,27,29,30,31,32,34,36};
     for(l=0;l<24;l++)rootLetter[12].lj.push_back(lj12[l]);
           short lsj12[]={0,9,11,25,27,29,31,32,38};
           for(l=0;l<9;l++)rootLetter[12].lsj.push_back(lsj12[l]);
                     short lssj12[]={0,25,27,32};
                     for(l=0;l<4;l++)rootLetter[12].lssj.push_back(lssj12[l]);
                              rootLetter[12].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[13].letter="p";
     short lb13[]={0,9,11,13,20,25,27,32};
     for(l=0;l<8;l++)rootLetter[13].lbase.push_back(lb13[l]);
     rootLetter[13].lbase1.push_back(0);
             rootLetter[13].lbase2.push_back(0);

     short lj13[]={0,1,2,3,4,9,10,11,12,13,14,16,17,18,19,20,25,26,27,29,30,31,32,34,35,38};
     for(l=0;l<26;l++)rootLetter[13].lj.push_back(lj13[l]);
          short lsj13[]={0,12,20,25,27};
          for(l=0;l<5;l++)rootLetter[13].lsj.push_back(lsj13[l]);
                     short lssj13[]={0,20,25};
                     for(l=0;l<3;l++)rootLetter[13].lssj.push_back(lssj13[l]);
                                rootLetter[13].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[14].letter="ph";
     short lb14[]={0,20,25,27};
     for(l=0;l<4;l++)rootLetter[14].lbase.push_back(lb14[l]);
     rootLetter[14].lbase1.push_back(0);
     rootLetter[14].lbase1.push_back(20);
             rootLetter[14].lbase2.push_back(0);

     short lj14[]={0,1,2,3,4,9,10,11,12,13,14,16,17,18,19,20,25,26,27,29,30,31,32,34,35,38};
     for(l=0;l<26;l++)rootLetter[14].lj.push_back(lj14[l]);
           rootLetter[14].lsj.push_back(0);
                     rootLetter[14].lssj.push_back(0);
                             rootLetter[14].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[15].letter="b";
     short lb15[]={0,9,11,13,16,20,25,27,29,32};
     for(l=0;l<10;l++)rootLetter[15].lbase.push_back(lb15[l]);
     rootLetter[15].lbase1.push_back(0);
     rootLetter[15].lbase1.push_back(25);
     rootLetter[15].lbase1.push_back(27);
             rootLetter[15].lbase2.push_back(0);

     short lj15[]={0,1,9,10,11,14,15,16,19,20,25,26,27,32,37};
     for(l=0;l<15;l++)rootLetter[15].lj.push_back(lj15[l]);
           short lsj15[]={0,8,12,16,27,29,32,37};
           for(l=0;l<8;l++)rootLetter[15].lsj.push_back(lsj15[l]);
                     rootLetter[15].lssj.push_back(0);
                     rootLetter[15].lssj.push_back(25);
                     rootLetter[15].lssj.push_back(27);
                            rootLetter[15].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[16].letter="m";
     short lb16[]={0,15,16,20,25,27};
     for(l=0;l<6;l++)rootLetter[16].lbase.push_back(lb16[l]);
     rootLetter[16].lbase1.push_back(0);
     rootLetter[16].lbase1.push_back(32);
             rootLetter[16].lbase2.push_back(0);

     short lj16[]={0,3,9,8,11,12,13,14,15,16,20,21,25,27,29,30,31,32,37};
     for(l=0;l<19;l++)rootLetter[16].lj.push_back(lj16[l]);
           short lsj16[]={0,25,27,32};
           for(l=0;l<4;l++)rootLetter[16].lsj.push_back(lsj16[l]);
                     rootLetter[16].lssj.push_back(0);
                     rootLetter[16].lssj.push_back(27);
                              rootLetter[16].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[17].letter="ts";

     short lb17[]={0,17,18,20};
     for(l=0;l<4;l++)rootLetter[17].lbase.push_back(lb17[l]);
     rootLetter[17].lbase1.push_back(0);
             rootLetter[17].lbase2.push_back(0);

     short lj17[]={0,1,10,12,16,17,18,20,25,26,27,29,30,32};
     for(l=0;l<14;l++)rootLetter[17].lj.push_back(lj17[l]);
           short lsj17[]={0,20,25,27};
           for(l=0;l<4;l++)rootLetter[17].lsj.push_back(lsj17[l]);
                     rootLetter[17].lssj.push_back(0);
                               rootLetter[17].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[18].letter="tsh";
     short lb18[]={0,20};
     for(l=0;l<2;l++)rootLetter[18].lbase.push_back(lb18[l]);
     rootLetter[18].lbase1.push_back(0);
             rootLetter[18].lbase2.push_back(0);

     short lj18[]={0,18,20,27,29};
     for(l=0;l<5;l++)rootLetter[18].lj.push_back(lj18[l]);
            rootLetter[18].lsj.push_back(0);
                     rootLetter[18].lssj.push_back(0);
                               rootLetter[18].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[19].letter="dz";
     short lb19[]={0,8,20,25,27,32};
     for(l=0;l<6;l++)rootLetter[19].lbase.push_back(lb19[l]);
     rootLetter[19].lbase1.push_back(0);
             rootLetter[19].lbase2.push_back(0);

     short lj19[]={0,8,12,16,19,20,25,27,29,32};
     for(l=0;l<10;l++)rootLetter[19].lj.push_back(lj19[l]);
           short lsj19[]={0,8,19,20,25,27,29,32};
           for(l=0;l<8;l++)rootLetter[19].lsj.push_back(lsj19[l]);
                     rootLetter[19].lssj.push_back(0);
                     rootLetter[19].lssj.push_back(27);
                     rootLetter[19].lssj.push_back(32);
                               rootLetter[19].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[20].letter="w";        //vasur
rootLetter[20].lbase.push_back(0);
     rootLetter[20].lbase1.push_back(0);
             rootLetter[20].lbase2.push_back(0);

     short lj20[]={0,20,27};
     for(l=0;l<3;l++)rootLetter[20].lj.push_back(lj20[l]);
             rootLetter[20].lsj.push_back(0);
                     rootLetter[20].lssj.push_back(0);
                           rootLetter[20].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[21].letter="w";   //hard form Wa
rootLetter[21].lbase.push_back(0);
rootLetter[21].lbase.push_back(14);
rootLetter[21].lbase.push_back(20);
     rootLetter[21].lbase1.push_back(0);
             rootLetter[21].lbase2.push_back(0);

     short lj21[]={0,12,20,21,25,27,29};
     for(l=0;l<7;l++)rootLetter[21].lj.push_back(lj21[l]);
             rootLetter[21].lsj.push_back(0);
             rootLetter[21].lsj.push_back(27);
                     rootLetter[21].lssj.push_back(0);
                           rootLetter[21].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[22].letter="zh";
     short lb22[]={0,20,27};
     for(l=0;l<3;l++)rootLetter[22].lbase.push_back(lb22[l]);
     rootLetter[22].lbase1.push_back(0);
             rootLetter[22].lbase2.push_back(0);

     short lj22[]={0,20,27};
     for(l=0;l<2;l++)rootLetter[22].lj.push_back(lj22[l]);
             rootLetter[22].lsj.push_back(0);
                     rootLetter[22].lssj.push_back(0);
                           rootLetter[22].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[23].letter="z";
rootLetter[23].lbase.push_back(0);
rootLetter[23].lbase.push_back(20);
rootLetter[23].lbase.push_back(29);
     rootLetter[23].lbase1.push_back(0);
             rootLetter[23].lbase2.push_back(0);

     short lj23[]={0,20,27};
     for(l=0;l<3;l++)rootLetter[23].lj.push_back(lj23[l]);
             rootLetter[23].lsj.push_back(0);
                     rootLetter[23].lssj.push_back(0);
                           rootLetter[23].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[24].letter="a.chung";
rootLetter[24].lbase.push_back(0);
     rootLetter[24].lbase1.push_back(0);
             rootLetter[24].lbase2.push_back(0);

     rootLetter[24].lj.push_back(0);
     rootLetter[24].lj.push_back(20);
             rootLetter[24].lsj.push_back(0);
                     rootLetter[24].lssj.push_back(0);
                           rootLetter[24].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[25].letter="y";
rootLetter[25].lbase.push_back(0);
     rootLetter[25].lbase1.push_back(0);
             rootLetter[25].lbase2.push_back(0);

     rootLetter[25].lj.push_back(0);
     rootLetter[25].lj.push_back(20);
             rootLetter[25].lsj.push_back(0);
                     rootLetter[25].lssj.push_back(0);
                           rootLetter[25].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[26].letter="Y";
rootLetter[26].lbase.push_back(0);
     rootLetter[26].lbase1.push_back(0);
             rootLetter[26].lbase2.push_back(0);

     short lj26[]={0,4,9,12,15,16,20,25,26,27,29,31};
     for(l=0;l<12;l++)rootLetter[26].lj.push_back(lj26[l]);
             rootLetter[26].lsj.push_back(0);
                     rootLetter[26].lssj.push_back(0);
                           rootLetter[26].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[27].letter="r";
     short lb27[]={0,1,3,4,5,7,8,9,11,12,15,16,17,18,19,20,25,26,29,30,32,38};
     for(l=0;l<22;l++)rootLetter[27].lbase.push_back(lb27[l]);
     rootLetter[27].lbase1.push_back(0);
     rootLetter[27].lbase1.push_back(8);
     rootLetter[27].lbase1.push_back(20);
     rootLetter[27].lbase1.push_back(25);
     rootLetter[27].lbase1.push_back(27);
     rootLetter[27].lbase1.push_back(32);
             rootLetter[27].lbase2.push_back(0);

     short lj27[]={0,2,3,14,20,25,32,34,35,36,37,38};
     for(l=0;l<12;l++)rootLetter[27].lj.push_back(lj27[l]);
           short lsj27[]={0,1,8,9,11,13,15,16,17,18,19,20,25,27,31,32,38};
           for(l=0;l<17;l++)rootLetter[27].lsj.push_back(lsj27[l]);
                     short lssj27[]={0,12,16,25,27,32};
                     for(l=0;l<6;l++)rootLetter[27].lssj.push_back(lssj27[l]);
                            rootLetter[27].lm.push_back(0);
                            rootLetter[27].lm.push_back(25);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[28].letter="R";
rootLetter[28].lbase.push_back(0);
     rootLetter[28].lbase1.push_back(0);
             rootLetter[28].lbase2.push_back(0);

     short lj28[]={0,9,10,11,12,13,14,15,16,18,20,21,25,26,27,30,31,37,38};
     for(l=0;l<19;l++)rootLetter[28].lj.push_back(lj28[l]);
           short lsj28[]={0,16,20,25,26,27,32,34,37};
           for(l=0;l<9;l++)rootLetter[28].lsj.push_back(lsj28[l]);
                     rootLetter[28].lssj.push_back(0);
                     rootLetter[28].lssj.push_back(25);
                     rootLetter[28].lssj.push_back(26);
                            rootLetter[28].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[29].letter="l";
     short lb29[]={0,1,3,4,5,7,9,11,13,15,17,20,25,27,29,32};
     for(l=0;l<16;l++)rootLetter[29].lbase.push_back(lb29[l]);
     rootLetter[29].lbase1.push_back(0);
             rootLetter[29].lbase2.push_back(0);

    short lj29[]={0,10,14,16,20,25,26,27,29};
     for(l=0;l<9;l++)rootLetter[29].lj.push_back(lj29[l]);
           short lsj29[]={0,1,20,25,32};
           for(l=0;l<5;l++)rootLetter[29].lsj.push_back(lsj29[l]);
                     rootLetter[29].lssj.push_back(0);
                            rootLetter[29].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[30].letter="sh";
     short lb30[]={0,9,17,20,27};
     for(l=0;l<5;l++)rootLetter[30].lbase.push_back(lb30[l]);
     rootLetter[30].lbase1.push_back(0);
             rootLetter[30].lbase2.push_back(0);

     short lj30[]={0,2,3,4,9,12,13,14,15,16,17,18,20,25,26,27,28,30,34,37};
     for(l=0;l<20;l++)rootLetter[30].lj.push_back(lj30[l]);
           short lsj30[]={0,13,18,20,25,27};
           for(l=0;l<6;l++)rootLetter[30].lsj.push_back(lsj30[l]);
                     rootLetter[30].lssj.push_back(0);
                            rootLetter[30].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[31].letter="s";
     short lb31[]={0,1,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,20,23,25,27,29,30,32};
     for(l=0;l<24;l++)rootLetter[31].lbase.push_back(lb31[l]);
     rootLetter[31].lbase1.push_back(0);
     rootLetter[31].lbase1.push_back(20);
     rootLetter[31].lbase1.push_back(25);
     rootLetter[31].lbase1.push_back(27);
             rootLetter[31].lbase2.push_back(0);
             rootLetter[31].lbase2.push_back(20);

     short lj31[]={0,2,4,7,10,19,20,25,26,27,31,34,35,38};
     for(l=0;l<14;l++)rootLetter[31].lj.push_back(lj31[l]);
           short lsj31[]={0,20,25,27,29,31,32,38};
           for(l=0;l<8;l++)rootLetter[31].lsj.push_back(lsj31[l]);
                     rootLetter[31].lssj.push_back(0);
                     rootLetter[31].lssj.push_back(25);
                            rootLetter[31].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[32].letter="h";
     short lb32[]={0,9,12,20,25,27,37};
     for(l=0;l<7;l++)rootLetter[32].lbase.push_back(lb32[l]);
     rootLetter[32].lbase1.push_back(0);
             rootLetter[32].lbase2.push_back(0);

     short lj32[]={0,1,2,3,8,9,12,13,14,16,20,25,26,27,29,30,31,32,37,38};
     for(l=0;l<20;l++)rootLetter[32].lj.push_back(lj32[l]);
           short lsj32[]={0,20,25,27};
           for(l=0;l<4;l++)rootLetter[32].lsj.push_back(lsj32[l]);
                     rootLetter[32].lssj.push_back(0);
                             rootLetter[32].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[33].letter="a";
rootLetter[33].lbase.push_back(0);
     rootLetter[33].lbase1.push_back(0);
             rootLetter[33].lbase2.push_back(0);

     short lj33[]={0,10,25,27,33};
     for(l=0;l<5;l++)rootLetter[33].lj.push_back(lj33[l]);
             rootLetter[33].lsj.push_back(0);
             rootLetter[33].lsj.push_back(25);
                     rootLetter[33].lssj.push_back(0);
                             rootLetter[33].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[34].letter="T";
rootLetter[34].lbase.push_back(0);
rootLetter[34].lbase.push_back(20);
rootLetter[34].lbase.push_back(27);
rootLetter[34].lbase.push_back(35);
     rootLetter[34].lbase1.push_back(0);
             rootLetter[34].lbase2.push_back(0);

     short lj34[]={0,1,2,3,10,11,12,13,14,15,16,17,20,25,26,27,29,31,34,38};
     for(l=0;l<20;l++)rootLetter[34].lj.push_back(lj34[l]);
           short lsj34[]={0,1,2,9,12,16,20,25,27,29,32,38};
           for(l=0;l<12;l++)rootLetter[34].lsj.push_back(lsj34[l]);
                     rootLetter[34].lssj.push_back(0);
                             rootLetter[34].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[35].letter="Th";
rootLetter[35].lbase.push_back(0);
rootLetter[35].lbase.push_back(20);
     rootLetter[35].lbase1.push_back(0);
             rootLetter[35].lbase2.push_back(0);

     short lj35[]={0,1,3,20,25,27,29,35,36};
     for(l=0;l<9;l++)rootLetter[35].lj.push_back(lj35[l]);
             rootLetter[35].lsj.push_back(0);
             rootLetter[35].lsj.push_back(32);
                     rootLetter[35].lssj.push_back(0);
                     rootLetter[35].lssj.push_back(27);
                             rootLetter[35].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[36].letter="D";
rootLetter[36].lbase.push_back(0);
rootLetter[36].lbase.push_back(20);
rootLetter[36].lbase.push_back(32);
     rootLetter[36].lbase1.push_back(0);
             rootLetter[36].lbase2.push_back(0);
     short lj36[]={0,3,11,15,16,20,25,26,29,32,36,37};
     for(l=0;l<12;l++)rootLetter[36].lj.push_back(lj36[l]);
           short lsj36[]={0,20,27,32,36};
           for(l=0;l<5;l++)rootLetter[36].lsj.push_back(lsj36[l]);
                     rootLetter[36].lssj.push_back(0);
                     rootLetter[36].lssj.push_back(25);
                     rootLetter[36].lssj.push_back(32);
                             rootLetter[36].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[37].letter="N";
     short lb37[]={0,11,20,25,36};
     for(l=0;l<5;l++)rootLetter[37].lbase.push_back(lb37[l]);
     rootLetter[37].lbase1.push_back(0);
     rootLetter[37].lbase1.push_back(27);
             rootLetter[37].lbase2.push_back(0);

     short lj37[]={0,1,9,11,15,16,20,25,26,27,29,34,35,36,37,38};
     for(l=0;l<16;l++)rootLetter[37].lj.push_back(lj37[l]);
           short lsj37[]={0,18,25,26,27,32};
           for(l=0;l<6;l++)rootLetter[37].lsj.push_back(lsj37[l]);
                     rootLetter[37].lssj.push_back(0);
                     rootLetter[37].lssj.push_back(25);
                            rootLetter[37].lm.push_back(0);
//______________________________________________________________________________
rootLetter.push_back(record);
rootLetter[38].letter="Sh";
     short lb38[]={0,9,10,13,34,35,36,20,25,28,37};
     for(l=0;l<11;l++)rootLetter[38].lbase.push_back(lb38[l]);
     rootLetter[38].lbase1.push_back(0);
             rootLetter[38].lbase2.push_back(0);

     short lj38[]={0,1,9,10,13,14,15,16,17,20,25,27,29,31,34,35,36,38};
     for(l=0;l<18;l++)rootLetter[38].lj.push_back(lj38[l]);
           short lsj38[]={0,20,25,26,27,28};
           for(l=0;l<6;l++)rootLetter[38].lsj.push_back(lsj38[l]);
                     rootLetter[38].lssj.push_back(0);
                     rootLetter[38].lssj.push_back(25);
                            rootLetter[38].lm.push_back(0);
//______________________________________________________________________________


//set vowel and bindu
vowelLetter.push_back(record);
vowelLetter[0].letter="a";
vowelLetter.push_back(record);
vowelLetter[1].letter="i";
vowelLetter.push_back(record);
vowelLetter[2].letter="e";
vowelLetter.push_back(record);
vowelLetter[3].letter="u";
vowelLetter.push_back(record);
vowelLetter[4].letter="o";
vowelLetter.push_back(record);
vowelLetter[5].letter="A";
vowelLetter.push_back(record);
vowelLetter[6].letter="I";
vowelLetter.push_back(record);
vowelLetter[7].letter="E";
vowelLetter.push_back(record);
vowelLetter[8].letter="U";
vowelLetter.push_back(record);
vowelLetter[9].letter="O";

// tagLetter
tagLetter.push_back(record);
tagLetter[0].letter="";
tagLetter.push_back(record);
tagLetter[1].letter="r";
tagLetter.push_back(record);
tagLetter[2].letter="y";
tagLetter.push_back(record);
tagLetter[3].letter="w";



}//_____________________________________________________________________________
