#include "GMemory.h"
#include "GVector.h"

void GMemory::textTagger(){
    /*
    string pathDict=inputData.data["dictPath"]+"TibetanGrammar.bin";  cout<<"pathDict="<<pathDict<<endl;
    GVector *vectorMainDict;
    GVector *vectorDict;
    GStr<uint>*keyDict;
    vectorMainDict=GVector::create(pathDict);// vectorMainDict->getName(strName);   //cout<<"getName1()="<<strName<<endl;
    vectorMainDict->resizeData(10,800);   //10 records in 500 mb
    //vectorMainDict->getName(strName);                                             //cout<<"getName2()="<<strName<<endl;
    vectorDict=GVector::create(vectorMainDict,"TibDict");
    
    //uint recordNum=vectorDict->size();
    keyDict=GStr<uint>::create(vectorMainDict,"keyDict");
    
    cout_<<"keyDict->size()="<<keyDict->size()<<"vectorDict->size()="<<vectorDict->size()<<endl;
    //keyDict->resize(0);
    if(!keyDict->size()||inputData.data["key"]=="reload"){
        keyDict->resize(2000000);
        uint sizeV=vectorDict->size();
        keyDict->resize(0);
        cout<<"start reload dictionary key "<<sizeV<<endl;
        vector<dictKey> vDict;
        for(uint a=0;a<sizeV;a++){
            string str;
            TString st;
            vectorDict->getTStr(a,&st);
            if(st[3].find("TR")==-1)continue;
            dictKey key;
            key.key=st[0];
            key.dict=st[2];
            key.lenght=(uint)key.key.size();
            key.id=a;
            vDict.push_back(key);
        }
        cout<<"start sort"<<endl;
        sort(vDict.begin(),vDict.end());
        cout<<"done keyDict->size()="<<keyDict->size()<<" sizeV="<<sizeV<<endl;
        for(int a=0;a<vDict.size();a++){
            cout<<vDict[a].key<<endl;
            keyDict->push_back(vDict[a].id);
        }
    }
    DIR *dir;
    if( (dir=opendir(inputData.data["fileList"].c_str()))!=NULL){
        readDirectoryToArray(inputData.fileList,inputData.data["fileList"],"txt");
    }else{
        if(!is_file(inputData.data["fileList"])){cout<<"no file "<<inputData.data["fileList"]; return;}
    }
    
    cout<<"inputData.fileList.size()="<<inputData.fileList.size();
    for(int i=0;i<inputData.fileList.size();i++){
        string destString;
        //cout<<"tagged text "<<inputData.fileList[i]<<endl;
        TString st;
        string textData=readText(inputData.fileList[i]);
        string delimeter;delimeter+=(char)0xC2; delimeter+=(char)0xA0; //no-break space
        textData=str_replace(delimeter.c_str()," ",textData);
        string reStr="(([\\p{Cyrillic}p{Latin}\\.,:;\\!\\-–]) ):|:\\1@:|:2";
        regExpReplace(textData,reStr);
        //cout<<" textData="<<textData<<endl;
    
        textData=str_replace("\n"," ",textData);
        textData=str_replace("\r"," ",textData);
        vector<string>textDataList=explode(" ",textData);
        //cout<<"text size="<<textDataList.size();
        int n=0; int step=0; int limit=(int)textDataList.size()/100; int count=0;
        for(int i=0;i<textDataList.size();i++){
            string line=clearText(textDataList[i]);  //cout<<" line="<<line<<endl;
            string rus=str_replace("@"," ",textDataList[i]);
            if(line.size()<6)continue;
            n++;
            if(rus==textDataList[i]){
                ((GLogicProcessor*)inputData.logicProcessor)->TibRusDictReport(vectorDict,keyDict,line);
                destString+=textDataList[i]+"\n"+line+"\n";
            }else{
                destString+=rus+"\n";
                
            }
            if(step>limit){cout<<count<<"% ";count++;step=0;}step++;
        }
        string path=inputData.fileList[i]+".out";
        writeText(destString, path);
    }
    */ 

}