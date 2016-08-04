bool LoadASCIToUniMap(){

// get full path of application


///load ASCIToUni.map/////////////////////////
string filePath=tmp.c_str();
replace(filePath.begin(),filePath.end(),'\\','/');
filePath.resize(filePath.rfind('/'));

/////////////////////
string fileName= filePath + "/TibUniDLL/Tibetan0x0F00.map";

        ifstream Tibetan0x0F00(fileName.c_str());
        if( !Tibetan0x0F00 ){
        ShowMessage("$(TibKeyYagpo)/TibUniDLL/Tibetan0x0F00.map \n not found");
        return false;
        }

    // skip comment
    streamsize maxLineSize =1024;
    while( Tibetan0x0F00.peek() == '%'||Tibetan0x0F00.peek() == '#' ||Tibetan0x0F00.peek() == '/')
    Tibetan0x0F00.ignore( maxLineSize , '\n' );

     vector<string> srcStrings;
     string tString;
     vector<string>lineStrings;
     short i=0;
     uniRecord record;
     vector<string> value;
     vector<string>::size_type d;

  while (getline(Tibetan0x0F00, tString,'\n')){
  srcStrings.push_back(tString);    //c_out<<"vector count="<<srcStrings.size()<<endl;
  lineStrings=explode(tString,"|");   //for(i=0;i<lineStrings.size();i++)c_out<<lineStrings[i]<<" ";
  //c_out<<"lineStrings="<<lineStrings.size();
  uniTibTable.push_back(record);

  value=explode(lineStrings[0],";");  //c_out<< value.size();
      for(d=0;d<value.size();d++){
      istringstream conv (value[d]);
      uniTibTable[i].uniCode.push_back(0);
      conv >>hex >>uniTibTable[i].uniCode[d];
      uniTibTable[i].uniHexCode.push_back("");
      uniTibTable[i].uniHexCode[d]=value[d];
      }
      uniTibTable[i].Wylie.push_back(lineStrings[1]);  //c_out<<uniTibTable[i].uniCode[0]<<" "<<uniTibTable[i].Wylie[0]<<endl;
      if(lineStrings.size()>2){
      //c_out<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[2]<<endl;

        value=explode(lineStrings[2],";");
        for(d=0;d<value.size();d++){
        istringstream conv1 (value[d]);
        uniTibTable[i].uniKey.push_back(0);
        conv1 >>hex >>uniTibTable[i].uniKey[d];
        uniTibTable[i].uniHexKey.push_back("");
        uniTibTable[i].uniHexKey[d]= value[d];
        //c_out<<"uniTibTable["<<i<<"].uniHexKey["<<d<<"]"<<uniTibTable[i].uniHexKey[d]<<endl;
        }
      }
 i++;
  }
//  c_out<< "uniTibTable[0].allCount="<<uniTibTable[0].allCount<<" maxUniRecord="<<maxUniRecord<<endl;
///////////////////////////////////////////////////////////////////

fileName= filePath + "/TibUniDLL/TibetanAlphabetGlyph.map";

        ifstream TibetanAlphabetGlyph(fileName.c_str());
        if( !TibetanAlphabetGlyph ){
        ShowMessage("$(TibKeyYagpo)/TibUniDLL/TibetanAlphabetGlyph.map \n not found");
        return false;
        }

    // skip comment

    while( TibetanAlphabetGlyph.peek() == '%'||TibetanAlphabetGlyph.peek() == '#' ||TibetanAlphabetGlyph.peek() == '/')
    TibetanAlphabetGlyph.ignore( maxLineSize , '\n' );
    i=0;

  while (getline(TibetanAlphabetGlyph, tString,'\n')){
  TibetanAlphabet.push_back(tString);    //c_out<<"vector count="<<srcStrings.size()<<endl;
  i++;
  }
/////////////////////////////////////////////////////////////////////////////////


fileName= filePath + "/TibUniDLL/UniToTibetanBig.map";
//c_out<<fileName<<endl;
        ifstream UniToTibetanBig(fileName.c_str());
        if( !UniToTibetanBig )return false;

    // skip comment
    while( UniToTibetanBig.peek() == '%'||UniToTibetanBig.peek() == '#' ||UniToTibetanBig.peek() == '/')
    UniToTibetanBig.ignore( maxLineSize , '\n' );

  srcStrings.clear(); //c_out<<"vector count11="<<srcStrings.size()<<endl;
  i=0;

   while (getline(UniToTibetanBig, tString,'\n')){
  srcStrings.push_back(tString);    //c_out<<"vector count="<<srcStrings.size()<<endl;
  lineStrings=explode(tString,"|");   //for(i=0;i<lineStrings.size();i++)c_out<<lineStrings[i]<<" ";
  //c_out<<"lineStrings="<<lineStrings.size();
  uniRecordTable.push_back(record);

  value=explode(lineStrings[0],";");  //c_out<< value.size();
      for(d=0;d<value.size();d++){
      istringstream conv (value[d]);
      uniRecordTable[i].uniCode.push_back(0);
      conv >>hex >>uniRecordTable[i].uniCode[d];
      uniRecordTable[i].uniHexCode.push_back("");
      uniRecordTable[i].uniHexCode[d]=value[d];
      }
      uniRecordTable[i].Wylie.push_back(lineStrings[1]);  //c_out<<uniTibTable[i].uniCode[0]<<" "<<uniTibTable[i].Wylie[0]<<endl;
      if(lineStrings.size()>2){
      //c_out<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[2]<<endl;

        value=explode(lineStrings[2],";");
        for(d=0;d<value.size();d++){
        istringstream conv1 (value[d]);
        uniRecordTable[i].uniKey.push_back(0);
        conv1 >>hex >>uniRecordTable[i].uniKey[d];
        uniRecordTable[i].uniHexKey.push_back("");
        uniRecordTable[i].uniHexKey[d]= value[d];
        //c_out<<"uniTibTable["<<i<<"].uniHexKey["<<d<<"]"<<uniTibTable[i].uniHexKey[d]<<endl;
        }
      }
      if(value.size()>maxUniRecord)maxUniRecord=value.size();


        if(lineStrings.size()>3){
      //c_out<<"lineStrings.size()="<<lineStrings.size()<<" v="<<lineStrings[2]<<endl;

        value=explode(lineStrings[3],";");
        for(d=0;d<value.size();d++){
        istringstream conv1 (value[d]);
        //c_out<<value[d]<<endl;
        uniRecordTable[i].uniTibetan.push_back(0xE000);
        conv1 >>hex >>uniRecordTable[i].uniTibetan[d];
        uniRecordTable[i].uniHexTibetan.push_back("");
        uniRecordTable[i].uniHexTibetan[d]= value[d];
        //c_out<<"uniTibTable["<<i<<"].uniHexKey["<<d<<"]"<<uniTibTable[i].uniHexKey[d]<<endl;
        }
      }

 i++;
  }
  //c_out<< "uniRecordTable[0].allCount="<<uniRecordTable.size()<<" maxUniRecord="<<maxUniRecord<<endl;


        return true;
};//____________________________________________________________________________
