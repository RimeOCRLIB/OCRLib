#include "GLogicProcessor.h"

//функция отображения массива результатов распознавания в виде
//HTML файла. Каждые 100-200 букв отображается в 
//виде отдельного "кадра" с сохранением местоположения буквы относительно
//высоты строки и габаритов изображения.
//Буквы с наибольшей корреляцией расположены в начальных кадрах страницы

static bool sortMatchC(const OCRMatch& d1, const OCRMatch& d2){
    return d1.correlation > d2.correlation;
}

void GLogicProcessor::drawGrapeLine(vector<OCRMatch>&line){
 
 string draw;
 string MAIN_HOST_="http://localhost/ocr/";

    int y0=y0Base;//((GStr<int>*)inputData.pref)->get(0);
    int y1=y1Base;//((GStr<int>*)inputData.pref)->get(1);
    int w=((GStr<int>*)inputData.pref)->get(2);
    int h=((GStr<int>*)inputData.pref)->get(3);
    
    //if(inputData.lineSize.size()){
    //    w=inputData.lineSize[2];
    //    w=inputData.lineSize[3];
    //}
    
    cout<<"grapeLine w="<<w<<" h="<<h<<" y0="<<y0<<" y1="<<y1<<" line.size()="<<line.size()<<endl;

    //sort(line.begin(),line.end(),sortMatchX);
 //int minY=10000;
 //int maxY=0;
 //int minX=10000;
 //int maxX=0;
    vector<OCRMatch>dLine;
    for(int i=0;i<line.size();i++){
         if(!line[i].correlation)continue;   //здесь нельзя проверять статус, он используется как счетчик при сборке стеков
         line[i].letterID=i;
         dLine.push_back(line[i]);
         dLine[dLine.size()-1].status=0;
         //cout<<"@"<<i<<" c="<<line[i].correlation<<" y0="<<line[i].y0<<" y1="<<line[i].y1<<" x0="<<line[i].x0
         //<<" x1="<<line[i].x1<<" "<<line[i].name<<" letterW="<<line[i].letterW<<" xCenter="<<line[i].xCenter<<" in="<<line[i].letterIndex<<endl;
    }
  
 ostringstream out;
 string  path=inputData.data["root"]+"/edit/headerPageLetter.xml";
 string headerPageHTML; readText(headerPageHTML,path);
 out<<headerPageHTML<<endl;
 out<<"</script>";
 out<<"</head><body topmargin=\"0px\" leftmargin=\"0px\">"<<endl;
 ostringstream out_;
 int rowCount=0;
 int index=0;
    
 path=inputData.data["inputFile"];  path=fileName(path);
 path=str_replace(".tif", ".jpg", path);

    
 while(1){
     
     //находим пару с наибольшей корреляцией
     int maxC=0,maxIndex=-1;
     for(int i=0;i<dLine.size();i++){
         if(dLine[i].status)continue;
         if(dLine[i].correlation>maxC){
             maxC=dLine[i].correlation;
             maxIndex=i;
         }
     }
     if(maxIndex<0)break; //разбор закончен
     
     //для найденной пары с наибольшей корреляцией строим «гроздья винограда»
     //находим все пары, центр которых находится внутри габарита найденной пары
     vector<OCRMatch>grapeLine;
     int dl=dLine[maxIndex].letterW*(float)0.15;
     int x0=dLine[maxIndex].x0+dl;
     int x1=dLine[maxIndex].x1-dl;
     
     //cout<<" x0="<<x0<<" x1="<<x1<<" dl="<<dl<<endl;
     //exit(0);
     for(int i=0;i<dLine.size();i++){
         if(dLine[i].xCenter>=x0&&dLine[i].xCenter<x1){
             if(dLine[i].status)continue;
             grapeLine.push_back(dLine[i]);             
             dLine[i].status=1;
         }
     }
     dLine[maxIndex].status=1;
     sort(grapeLine.begin(),grapeLine.end(),sortMatchC);
     //cout<<" maxC="<<maxC<<" n="<<dLine[maxIndex].name<<" grapeLine.size()="<<grapeLine.size()<<" rowCount="<<rowCount;
     
     
     
   if(rowCount<grapeLine.size())rowCount=(int)grapeLine.size();   //сохраняем значение максимального размера грозди
     
   for(int i=0;i<grapeLine.size();i++){
       int top=h*i+grapeLine[i].y0;
      
       out_<<"<div style=\"position:absolute;top:"<<grapeLine[i].s.yLimit0-MATRIX_BORDER+h*i<<"px; left:"<<grapeLine[i].x0<<"px;height:0px;width:"<<grapeLine[i].letterW<<"px;border:1px solid gray;\">"<<"</div>";
       
       out_<<"<div class=\"dE\" id=\"g"<<index<<"\" style=\"position:absolute; top:"<<top<<
       "px; left:"<<grapeLine[i].x0<<"px; border:1px solid red;width:"<<grapeLine[i].letterW<<"px;height:"
       <<grapeLine[i].letterH<<"px\">";
       
       //for(int n=0;n<grapeLine[i].mask32Vector.size();n++){
       //    int c=grapeLine[i].mask32Vector[n].correlation-75; if (c<0)c=0;
       //    out_<<"<div class=\"nM\" style=\"top:"<<h*i+grapeLine[i].mask32Vector[n].yMax-top<<
       //    "px; left:"<<grapeLine[i].mask32Vector[n].xMax-grapeLine[i].x0<<"px;width:"<<grapeLine[i].mask32Vector[n].imgW<<"px;height:"
       //    <<grapeLine[i].mask32Vector[n].mH<<"px; border:1px solid rgba(0, 255, 0, "<<0.4+0.02*c<<")\"></div>\n";
       //}
       out_<<"<div class=\"dM\" style=\"width:"<<grapeLine[i].letterW<<"px; height:6px;\"></div>";
       
       out_<<"<a href=\""<<MAIN_HOST_<<"ocr.php?xml=<ocrData>edit</ocrData><index>"<<grapeLine[i].letterIndex+1<<"</index>\"  target=\"_blank\" ><span class=\"nL\">"<<"*"<<"</span></a>"<<endl;
       out_<<"<span class=\"nS\">"<<grapeLine[i].correlation<<"/"<<grapeLine[i].letterID<<"</span>";
       out_<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:29px\" >"<<grapeLine[i].name<<grapeLine[i].delimeter
           <<"<br>";
       if(grapeLine[i].dIndex.size()){
           string st=UnicodeToYagpo(grapeLine[i].name);
           wstring sW=UTF_to_Unicode(st);
           for(int j=0;j<sW.size();j++){
               st=Unicode_to_UTF(sW[j]);
               st=YagpoToUni(st);
               if(grapeLine[i].dIndex[j]==1){
                   out_<<st<<"་";
               }else if(grapeLine[i].dIndex[j]==2){
                   out_<<st<<2;
               }else{
                   out_<<st;
               }
           }
           out_<<"<br>";
       }
       out_<<"<font size=\"4px\">";
       for (int n=0; n< grapeLine[i].letter.size(); n++) {
           out_<<grapeLine[i].letter[n].name<<""<<grapeLine[i].letter[n].delimeter<<" ";
       }
       out_<<"</font></div></div>"<<endl;
       //if(line[i+index].name=="མ")cout<<" line[i+index].y0="<<line[i+index].yCenter-line[i+index].letterH/2<<" y0="<<y0<<endl;
      //     exit(0);}
       index++;
        
   }   
 }
   
 for(int i=0;i<rowCount;i++){
    
      //рисуем графическую строку
      time_t t=time(NULL);
      out<<"<img style=\"position:absolute; top:"<<h*i<<"px;";//width:"<<w<<"px;height:"<<h<<"px\"
      out<<"\" src=\""<<path<<"?tm="<<t<<"\"/>"<<endl;
      out<<"<div style=\"position:absolute; top:"<<h*i<<"px; border:1px solid green; width:"<<w<<"px;height:"<<h<<"px\"></div>"<<endl;
      out<<"<div id=\"y0\" style=\"position:absolute; top:"<<h*i+y0<<"px; border:1px solid grey;width:"<<w<<"px;height:"<<y1-y0<<"px\"></div>"<<endl;
      

 }
    //cout_<<inputData.data["ocrData"];
    

    path=inputData.data["inputFile"];  path=str_replace(".tif", ".jpg", path);

   
    out_<<"<div style=\"position:absolute; top:0px; height:10px; owerflow:hidden\">";
	out_<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"\" name=\"main\">\n";
	out_<<"<input  type=\"button\" value=\"Ok\" onClick=\"setLetterInBase()\" style=\"position:absolute; top:2px\">";
	out_<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"20\" rows=\"1\" style=\"position:absolute; left:32px\"></textarea>\n";
	out_<<"<input  name=\"inputFile\" id=\"inputFile\" type=\"hidden\" value=\"";
    out_<<path<<"\"/>\n";
	out_<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/>\n";
	out_<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\"";
    out_<<inputData.data["inputFolder"]<<"\"/>\n";
	out_<<"</form>";
	out_<<"</div></body>";

    
    
    draw=out.str();     
    draw+=out_.str();
    string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";     cout<<" str="<<str<<endl;
    writeText(draw,str);
}
