void GenerateFullTibetanNameSpace(
      TObject *Sender){

TibRootLetter();



short l;
int index=19712+256,level;
AnsiString TibLetter;
short flag;
TStringList *nameList=new TStringList;
unsigned int a,b,c,d,j;

for(a=1;a<rootLetter.size();a++){ //for every root letter
for(b=0;b<rootLetter[a].lj.size();b++){  //for every joined letter
for(c=0;c<rootLetter[a].lsj.size();c++){ //for every subjoined letter
//for(int t=0;t<rootLetter[a].lsCount;t++){//for every 4 letter
//if(c>0){
//        if(rootLetter[a].ld[c]==27||
//        rootLetter[a].ld[c]==25||
//        rootLetter[a].ld[c]==20){level=2;}else{level=4;}
//}else{level=0;}
//first we need check - does this subjoined letter valid for joined letter?
if(c>0){
    flag=0;
    for(d=0;d<rootLetter[rootLetter[a].lj[b]].lj.size();d++){//for every lt in joined letter
        if(rootLetter[rootLetter[a].lj[b]].lj[d]==rootLetter[a].lsj[c]){
        flag=1; break;
        }
    }
}else{flag=1;}

        if(flag){

          for(j=0;j<vowelLetter.size();j++){//for every vowel
              //if(j==0||j==3||j==5||j==8){
              //TibLetter="0x"+IntToHex(index,4) + " " + rootLetter[a].letter;
              //if(b>0){TibLetter+=(AnsiString)"_"+rootLetter[rootLetter[a].lt[b]].letter;}
              //if(c>0){TibLetter+=(AnsiString)"_"+rootLetter[rootLetter[a].ld[c]].letter;}
              //if(t>0){TibLetter+=(AnsiString)"_"+rootLetter[rootLetter[a].ls[t]].letter;}
              //TibLetter+=(AnsiString)vowelLetter[j].letter;
              //nameList->Add(TibLetter);
              //index++;
              //};
          };
        };
};
};
};
index+=256;
//nameList->Add((AnsiString)a);
//nameList->Add((AnsiString)"_______________________________________");
};


//RichEdit1->Lines->Add((AnsiString)index);
//RichEdit1->Lines->Assign(nameList);

}//_____________________________________________________________________________
