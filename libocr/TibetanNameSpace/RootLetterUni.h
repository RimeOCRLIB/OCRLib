void BuildStandartTibetanUniTable(таблицы *inputData){

	map<wstring, uniRecord>uniTibTableUni;
	map<wstring, uniRecord>uniTibTableYagpo;
	vector<uniRecord>uniTibTable0F00;
	map<wstring,wstring>ASCItoUni;
	vector<string>tibStr;
	vector<string>wylieStr;
	vector<string>wylieStrVect;
	vector<string>yagpoStrVect;
	vector<string>TibetanAlphabet;
	vector<rootLetterOCR>rootLetter;
	vector<rootLetterOCR>vowelLetter;
	vector<rootLetterOCR>tagLetter;
	
	TibRootLetter(rootLetter,vowelLetter,tagLetter);
	
	LoadASCIToUniMap(inputData,ASCItoUni,uniTibTableUni,uniTibTableYagpo,uniTibTable0F00, TibetanAlphabet);
	
	//cout<<"uniTibTable0F00 size="<<uniTibTable0F00.size()<<endl;
	

string tmp,tmp1,tmpKeyCode;
string::size_type it;
vector<string> key;
string strWylie,glyphStr;	
wstring wStr;
unsigned int a,b,c,d,j,i,m,n,t;


int index=0xE000;
vector<string> TibLetter(3);
short flag;

	short buildName=0;
	short AliKali=0;
	short buildRootGlyph=0 ;
	short buildGlyph=1;
	short vowelCount=vowelLetter.size();
	string rootGlyph;

	if(buildRootGlyph)vowelCount=1;
	

//for(a=1;a<2;a++){ //for every root letter
	
for(a=1;a<rootLetter.size();a++){ //for every root letter
for(b=0;b<rootLetter[a].lbase.size();b++){  //for every valid joined letter
for(c=0;c<rootLetter[a].lbase1.size();c++){ //for every subjoined letter
for(d=0;d<rootLetter[a].lbase2.size();d++){ //for every subjoined letter
for(j=0;j<vowelCount;j++){//for every vowel
	
//if(j==0||j==3||j==5||j==8){
          //c_out<<"vowel"<<vowelLetter[j].letter<<" letter="<<rootLetter[a].letter<<endl;


              if(a==20||a==26||a==28)continue;   //vasur, hard form Ya hard form Ra
              if(c>0){  //for sub joined letters
                flag=0;
                for(m=0;m<rootLetter[rootLetter[a].lbase[b]].lbase.size();m++){   //check - does it valid pair root-join letter
                  if(rootLetter[rootLetter[a].lbase[b]].lbase[m]==rootLetter[a].lbase1[c]){//letter is valid
                  flag=1;break;}
                }

                if(rootLetter[a].lbase[b]==20)continue;
                if(rootLetter[a].lbase[b]==25)continue;
                if(rootLetter[a].lbase[b]==27&&rootLetter[a].lbase1[c]!=20)continue;

                if(!flag)continue;
              }

              if(d>0){  //for sub joined letters
                flag=0;
                for(m=0;m<rootLetter[rootLetter[a].lbase[b]].lbase.size();m++){   //check - does it valid pair root-join letter
                  if(rootLetter[rootLetter[a].lbase[b]].lbase[m]==rootLetter[a].lbase2[d]){//letter is valid
                  flag=1;break;}
                }

                if(rootLetter[a].lbase1[c]==20)continue;
                if(rootLetter[a].lbase1[c]==25)continue;
                if(rootLetter[a].lbase1[c]==27&&rootLetter[a].lbase2[d]!=20)continue;

                if(!flag)continue;
              }




                   TibLetter[1]=rootLetter[a].letter;
				   if(buildRootGlyph)glyphStr="tib."+rootLetter[a].letter+"a";
				                  
	               for(i=0;i<uniTibTable0F00.size();i++){
                     if(rootLetter[a].letter==uniTibTable0F00[i].Wylie){
					 TibLetter[0]=implode(";",uniTibTable0F00[i].uniHexCode);
					 TibLetter[2]=implode(";",uniTibTable0F00[i].uniHexKey);
                     break;
                     }
                   }
                   if(b>0){
                   tmp1="_";
                   tmp1+=rootLetter[rootLetter[a].lbase[b]].letter;
                   //cout<<"tmp1="<<tmp1<<endl;
                       for(n=0;n<uniTibTable0F00.size();n++){
						 if(uniTibTable0F00[n].Wylie==tmp1){
						 TibLetter[0]+=implode(";",uniTibTable0F00[n].uniHexCode);
						 TibLetter[2]+=implode(";",uniTibTable0F00[n].uniHexKey);
						 TibLetter[1]+=tmp1;
							 //glyphStr+="+tib."+tmp1+"a";
							 //	cout<<"v="<<TibLetter[2]<<endl;
                         break;
                         }
                       }
                   }
                  if(c>0){
                    tmp1="_";
                    tmp1+=rootLetter[rootLetter[a].lbase1[c]].letter;
					//cout<<"tmp2="<<tmp1<<endl;  
                       for(n=0;n<uniTibTable0F00.size();n++){
						 if(uniTibTable0F00[n].Wylie==tmp1){
						 TibLetter[0]+=implode(";",uniTibTable0F00[n].uniHexCode);
						 TibLetter[2]+=implode(";",uniTibTable0F00[n].uniHexKey);
                         TibLetter[1]+=tmp1;
                         glyphStr+="+tib."+tmp1;
							 //cout<<"v="<<TibLetter[2]<<endl;
                         break;
                         }
                       }
                   }
                  if(d>0){
                    tmp1="_";
                    tmp1+=rootLetter[rootLetter[a].lbase2[d]].letter;
                       for(n=0;n<uniTibTable0F00.size();n++){
						 if(uniTibTable0F00[n].Wylie==tmp1){
						 TibLetter[0]+=implode(";",uniTibTable0F00[n].uniHexCode);
						 TibLetter[2]+=implode(";",uniTibTable0F00[n].uniHexKey);
                         TibLetter[1]+=tmp1;
						 //glyphStr+="+tib."+tmp1+"a";	 
                         //c_out<<"v="<<TibLetter[2]<<endl;
                         break;
                         }
                       }
                   }



                  for(t=0;t<uniTibTable0F00.size();t++){
                    //if(uniTibTable0F00[t].Wylie=="O")
                    //c_out<<"uniTibTable0F00["<<t<<"].Wylie"<<uniTibTable0F00[t].Wylie<<endl;
                    if(vowelLetter[j].letter==uniTibTable0F00[t].Wylie){
					if(j!=0)TibLetter[0]+=implode(";",uniTibTable0F00[t].uniHexCode);
                    TibLetter[2]+=implode(";",uniTibTable0F00[t].uniHexKey);
                    break;
                    }
                  }
	
 	              

                  TibLetter[1]+=vowelLetter[j].letter;
	              if(j==0)rootGlyph=TibLetter[1];
	              glyphStr=rootGlyph+"+c."+vowelLetter[j].letter+"="+TibLetter[1];
                  
	
				  //c_out<<"match "<<TibLetter[1]<<endl;
                  //cout<<"TibLetter[1]"<<TibLetter[1]<<endl;
  	              //cout<<TibLetter[1];

	
              if(TibLetter[0].size()>0)TibLetter[0].resize(TibLetter[0].size()-1);
              tmpKeyCode=implode("|",TibLetter);
              tmpKeyCode.resize(tmpKeyCode.size()-2);


              strWylie=TibLetter[1].substr(0,TibLetter[1].size()-1);
              
              for(t=0;t<TibetanAlphabet.size();t++){
                   if(TibetanAlphabet[t]==strWylie){ //c_out<<"match"<<endl;
                      it=tmpKeyCode.find("0x0F84;",0);
                      while (it!=string::npos){
                      tmpKeyCode.erase(it,7);
                      it=tmpKeyCode.find("0x0F84;",0);
                      }
                   break;}
              }

              tmpKeyCode+="|0x";
              tmpKeyCode+=intToHex(index);


              //c_out<<TibLetter[0]<<endl;
              //cout<<tmpKeyCode;
              tmpKeyCode="0x";
              tmpKeyCode+=intToHex(index);
              tmpKeyCode+=" ";
              
			  tmpKeyCode+=TibLetter[1];
              
			  if(buildGlyph)cout<<glyphStr;
	
	          if(buildName){
                 if(AliKali){
                 wStr=index;
                 cout<<wStr.c_str();
                 }else{
                 cout<<tmpKeyCode;
                 }
              }else{
              //cout<<tmpKeyCode;
              }
         
			  index++;   cout<<endl;

}
}
}
}

//index+=8;
//
};
//ShowMessage(IntToStr(nameList->Count));
//if(AliKali)return;
//if(buildName){
//RichEdit1->Lines->Assign(nameList);
//RichEdit1->Lines->Add(IntToStr(index));
//}else{
//RichEdit1->Lines->Assign(uniList);
//RichEdit1->Lines->Add(IntToStr(index));
//}

//ShowMessage(IntToStr(nameList->Count));
}//_____________________________________________________________________________
