#include "GLogicProcessor.h"

//функция отображения массива результатов распознавания в виде
//HTML файла. Каждые 5 букв отображается в 
//виде отдельного "кадра" с сохранением местоположения буквы относительно
//высоты строки и габаритов изображения

void GLogicProcessor::drawMatchLine(vector<OCRMatch>&line){
 
 string draw;

    int y0=y0Base;//((GStr<int>*)inputData.pref)->get(0);
    int y1=y1Base;//((GStr<int>*)inputData.pref)->get(1);
    int w=0;//((GStr<int>*)inputData.pref)->get(2);
    int h=0;//((GStr<int>*)inputData.pref)->get(3);
    
    cout<<"w="<<w<<" h="<<h<<" y0="<<y0<<" y1="<<y1<<endl;

    //sort(line.begin(),line.end(),sortMatchX);
 //int minY=10000;
 //int maxY=0;
 //int minX=10000;
 //int maxX=0;
    for(int i=0;i<line.size();i++){
         if(!line[i].correlation)continue;   //здесь нельзя проверять статус, он используется как счетчик при сборке стеков
         cout<<"y0="<<line[i].y0<<" y1="<<line[i].y1<<" x0="<<line[i].x0<<" x1="<<line[i].x1<<endl;
    }
 
  ostringstream out;
  
 
 string  path=inputData.data["root"]+"/edit/headerStack.xml";
 string headerPageHTML; readText(headerPageHTML,path);
 out<<headerPageHTML<<endl;
 out<<"</script>";
 out<<"</head><body topmargin=\"0px\" leftmargin=\"0px\">"<<endl;

 path=inputData.data["inputFile"];  path=fileName(path);    path=str_replace(".tif", ".jpg", path);


 int index=0; int rowCount=1;
 while(index<line.size()&&index<6000){
   out<<"<img style=\"position:absolute; top:"<<h*rowCount-h<<";width:"<<w<<";height:"<<h<<"\" src=\""<<path<<"\"/>"<<endl;
   out<<"<div style=\"position:absolute; top:"<<h*rowCount-h<<"; border:1px solid green; width:"<<w<<"px;height:"<<h<<"\"></div>"<<endl;
   out<<"<div style=\"position:absolute; top:"<<h*rowCount-h+y0<<"; border:1px solid grey;width:"<<w<<";height:"<<y1-y0<<"\"></div>"<<endl;
     
   int count=0;
   while(count<5&&index<line.size()){
     if(!line[index].correlation){index++;continue;}
     out<<"<div class=\"dE\" id=\"g"<<index<<"\" style=\"position:absolute; top:"<<h*rowCount-h+line[index].y0<<
       "; left:"<<line[index].x0<<"px; border:1px solid red;width:"<<line[index].letterW<<";height:"
       <<line[index].letterH<<"\">";
       out<<"<div class=\"dM\" style=\"width:"<<line[index].letterW<<"; height:3px;\"></div>"<<endl;
       out<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:39px\" >"<<line[index].name<<"/"
       <<line[index].correlation<<"/"<<index<<"</div></div>"<<endl;
       index++;count++;
       
       //if(line[index].name=="ཇ")cout<<"index="<<index<<" c="<<line[index].correlation<<endl;
      //     exit(0);}
        
   }
   rowCount++;
   
 }
    draw=out.str();     
    string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";   
    writeText(draw,str);
 
}
