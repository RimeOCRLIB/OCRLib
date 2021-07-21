#include "GMainEditor.h"


///вывод результатов распознавания в HTML файл постранично.
void GMainEditor::drawOCRPage(vector<stringOCR>&strArray){
	int pageWide=0;  
	if(pechaImg->columns()>pechaImg->rows())pageWide=1;
    float scale;
	string fontName=aliKali->fontName;
	if(fontName=="")fontName="Yagpo Tibetan Uni";
    string path;
	//cout_<<"inputData.data[\"inputFile\"]="<<inputData.data["inputFile"];
	
    ostringstream out;
    path=inputData.data["root"]+"/edit/mainPage.html";
    
	string headerPageHTML; readText(headerPageHTML,path);
	if(pageWide){
		out<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: relative;";
		out<<"text-align: left; width: 1600px; left:20px; z-index:-1; top: 0px;\"><br />"<<endl;
		out<<"<img id=\"pechaImg\" src=\""<<fileName(inputData.data["inputFile"])<<"\" width=\"1600px\">"<<endl;
		out<<"</div><br>";
		out<<"<div id=\"textOCR\" onClick=\"editOCRText()\" style=\"font-family:"<<fontName<<",Himalaya; font-size:1.4em; width:1900px; position:relative; top:0px; left:50px;\"><!--OCR--><br>"<<endl;
		out<<mainString;
		out<<"</div><!--endOCR-->";
        scale=(float)1600/pechaImg->columns(); 
        for(int i=(int)strArray.size()-1;i>=0;i--){
            out<<"<div onClick=\"lineOCR("<<i<<")\" style=\"position:absolute; left:0px; top:"<<(int)strArray[i].y0*scale+20<<"px; width:10px; height:10px; background:grey\"></div>\n";
        }
        
		
	}else{
		out<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: relative;";
		out<<"text-align: left; width: 800px; left:0px; z-index:-1; top: 0px;\"><br />"<<endl;
		out<<"<img id=\"pechaImg\" src=\""<<fileName(inputData.data["inputFile"])<<"\" width=\"600px\">"<<endl;
		out<<"</div>";
        out<<"<div id=\"textOCR\" onClick=\"editOCRText()\" style=\"font-family:"<<fontName<<",Himalaya; font-size:1.4em; width: 1024px; position:absolute; top:0px; left:800px;\"><!--OCR--><br>"<<endl;
		out<<mainString;
		out<<"</div><!--endOCR-->";
        scale=(float)600/pechaImg->columns();
        for(int i=(int)strArray.size()-1;i>=0;i--){
            out<<"<div onClick=\"lineOCR("<<i<<")\" style=\"position:absolute; left:0px; top:"<<(int)strArray[i].y0*scale+20<<"px; width:10px; height:10px; background:grey\"></div>\n";
        }
		
	}
    out<<"<div id=\"nextPage\" style=\"position: absolute;top:0;left:250px\"><a href=\""<<inputData.data["nextPage"]<<"\">NextPage</a><script>nextPageURL=\""<<inputData.data["nextPage"]<<"\"</script></div>";
    string text=out.str();
    headerPageHTML=str_replace("@@Text@@",text,headerPageHTML);
    headerPageHTML=str_replace("@@fileName@@",inputData.data["inputFile"],headerPageHTML);
    
	string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";   
    writeText(headerPageHTML,str);
	

}



///вывод результатов распознавания в HTML файл постранично с возможностью заненсения букв в базу.
void GMainEditor::drawStrArray(vector<stringOCR>&strArray, int border){
	int pageWide=0;  
	if(pechaImg->columns()>pechaImg->rows())pageWide=1;
	string fontName=aliKali->fontName;
	if(fontName=="")fontName="Yagpo Tibetan Uni";
	
	//cout_<<"@@@draw pageWide="<<pageWide<<" pechaImg->columns()="<<pechaImg->columns()<<" pechaImg->rows()="<<pechaImg->rows()<<endl;
	
	string path=fileName(inputData.data["inputFile"]);
	//cout_<<"inputData.data[\"inputFile\"]="<<inputData.data["inputFile"];
	
    ostringstream c_out1;
	
    if(pageWide){
	    path=inputData.data["root"]+"/edit/headerPageLetter.xml";
    }else{
        path=inputData.data["root"]+"/edit/headerPage.xml";
    }
    
	string headerPageHTML; readText(headerPageHTML,path);
	cout_<<"path@@="<<path<<endl;
	
	c_out1<<headerPageHTML<<endl;
	c_out1<<"var pictW = "<<pechaImg[0].columns()<<";"<<endl;
	
	/*
     for ( strInd=strArray.size()-1; strInd >=0; strInd-- ){
     for ( m=0; m < strArray[strInd].wordArray.size(); m++ ){
     wordOCR *wP=&strArray[strInd].wordArray[m];
     if(wP->id<0)continue;
     x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
     c_out1<<"   frame["<<wP->id<<"]=new Array(4); ";
     c_out1<<"frame["<<wP->id<<"].x="<<x0<<"; ";
     c_out1<<"frame["<<wP->id<<"].y="<<y0<<"; ";
     c_out1<<"frame["<<wP->id<<"].w="<<x1-x0<<"; ";
     c_out1<<"frame["<<wP->id<<"].h="<<y1-y0<<";"<<endl;
     }
     }
     */
	//cout_<<"@@@"<<strInd<<endl;
	if(pageWide){
		c_out1<<"function a(index){"<<endl;
		c_out1<<"	//alert(document.getElementById(\"frame\"));"<<endl;
		c_out1<<"	var xFrame=frame[index].x*(1600/pictW*3)+20;"<<endl;
		c_out1<<"	var yFrame=frame[index].y*(1600/pictW*3)+20;"<<endl;
		//c_out1<<"	//alert(xFrame);"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.left=xFrame+\"px\";"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.top=yFrame+\"px\"; "<<endl;
		//c_out1<<"	document.getElementById('outputStr').innerHTML=frame[index].x+":|:"+frame[index].y;"<<endl;
		c_out1<<"}\n";
	}else{
		c_out1<<"function a(index){"<<endl;
		c_out1<<"	//alert(document.getElementById(\"frame\"));"<<endl;
		c_out1<<"	var xFrame=frame[index].x*(600/pictW)+640-10;"<<endl;
		c_out1<<"	var yFrame=frame[index].y*(600/pictW)-10;"<<endl;
		//c_out1<<"	//alert(xFrame);"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.left=xFrame+\"px\";"<<endl;
		c_out1<<"	document.getElementById(\"frame\").style.top=yFrame+\"px\"; "<<endl;
		//c_out1<<"	document.getElementById('outputStr').innerHTML=frame[index].x+":|:"+frame[index].y;"<<endl;
		c_out1<<"}\n";
	}	
	c_out1<<"var pageLink=\""<<fileName(inputData.data["inputFile"])<<"\";\n";
	
	c_out1<<"</script>";
	c_out1<<"</head><body topmargin=\"0px\" leftmargin=\"0px\" onLoad=\"link()\">"<<endl;
	c_out1<<"<div id=\"frame\" style=\"width:30px; height:30px; position:absolute; border:2px solid red\"></div>";
	
	if(pageWide){
		c_out1<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;";
		c_out1<<"text-align: left; width: 1600px; left:20px; z-index:-1; top: 20px;\"><br />"<<endl;
		c_out1<<"<img id=\"pechaImg\" src=\""<<fileName(inputData.data["inputFile"])<<"\" width=\"1600px\">"<<endl;
		c_out1<<"</div>";
		
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width:1600px; position:absolute; top:350px; left:250px;\"><br>"<<endl;
		c_out1<<mainString;
		c_out1<<"</div>";
		
	}else{
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:"<<(float)3500/pechaImg[0].columns()<<"em; width: 600px; position:absolute; top:20px; left:20px;\"><br>"<<endl;
		c_out1<<mainString;
		c_out1<<"</div>";
		c_out1<<"<div id=\"page_bitmap\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;";
		c_out1<<"text-align: left; width: 480px; left:640px; z-index:-1; top: -20px;\"><br />"<<endl;
		c_out1<<"<img id=\"pechaImg\" src=\""<<fileName(inputData.data["inputFile"])<<"\" width=\"600px\">"<<endl;
		c_out1<<"</div>";
		
	}	
    
	c_out1<<"<div id=\"page_form\" style=\"margin-left: 0px; margin-right: 0px;   position: absolute;top:900px;\">";
    c_out1<<"<br><a href=\"http://www.dharmabook.ru\" target=\"_blank\">tibetan OCR www.dharmabook.ru</a><br>";
	c_out1<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"/cgi/yagpoOCR.cgi\" name=\"main\">\n";
	c_out1<<"<input  name=\"inputFile\"  id=\"inputFile\" type=\"hidden\" value=\""; c_out1<<inputData.data["inputFile"];c_out1<<"\"/>\n";
	c_out1<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/>\n";
	c_out1<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; c_out1<<inputData.data["inputFolder"];c_out1<<"\"/>\n";
    //	c_out1<<"<button name=\"submit\" value=\"submit\" type=\"submit\">Set letter in base</button>";
    //	c_out1<<"<input  name=\"newLetter\"  type=\"checkbox\" value=\"true\" />Set as new letter<br>\n";
	c_out1<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"70\" rows=\"3\"></textarea>\n";
	c_out1<<"</form></div>";
	
	for(int i=0;i<25;i++){
		c_out1<<"<div class=\"dE\" id=\"g"<<i<<"\" c=\"83\" style=\"left:40px; top:0px; width:30px; height:30px; \">"<<endl;
		c_out1<<"<div class=\"dM\" style=\"width: 17px; height:3px;\"></div>"<<endl;
		c_out1<<"<div class=\"nG\" id=\""<<i<<"\" style=\" top:39px\" onClick=\"edit("<<i<<")\" onBlur=\"setText("<<i<<")\">*</div></div>"<<endl;
	}
	
	path=inputData.data["inputFolder"]+"/_all_text_html.html";
    //ofstream c_out; c_out.open(path.c_str(),ofstream::app);
    //c_out<<"{"<<inputData.data["inputFile"]<<"}"<<endl;
	//c_out<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width: 600px;\"><br>"<<endl;
    //c_out<<mainString;
	//c_out<<"</div>\n";
	//c_out.close();
	
	string strOut=c_out1.str();
    string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";
    writeText(strOut,str);
	
	// x0,y0,x1,y1
	
}//_____________________________________________________________________________


/** @fn void GMainEditor::drawString(int nStr, int border)
 @brief вывод в стандартный выходной поток HTML отображения
 глобального массива результатов распознавания strArray
 
 параметры вывода задаются в  управляющей структуре inputData
 */
void GMainEditor::drawStrArrayDebug(vector<stringOCR>&strArray, int border){
	
	int strInd,m,strFlag;
	int x0,x1,y0,y1,gY;        // ,xt0,yt0,xt1,yt1
	//char cbuf[10];
	// x0,x1, y0,y1     координаты квадрата вокруг буквы
	int index=0;
	int print=0;
    
    string fontName;
    ostringstream c_out1;
    if(aliKali)fontName=aliKali->fontName;
	if(fontName=="")fontName="Yagpo Tibetan Uni";
    
	string path=inputData.data["root"]+"edit/headerPageLetter.xml";
	replace(path.begin(),path.end(),'\\','/');
	cout_<<"pathDRAW="<<path<<endl;
	string headerPageHTML; readText(headerPageHTML,path);
    
	c_out1<<headerPageHTML<<endl;
    
    c_out1<<"			var pictW = "<<pechaImg[0].columns()<<";"<<endl;
    
	c_out1<<"</script>"<<endl;
	c_out1<<"<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">"<<endl;
	c_out1<<"</head>"<<endl;
	c_out1<<"<body topmargin=\"0px\" leftmargin=\"0px\">"<<endl;
	c_out1<<"<style>";
	c_out1<<".nG {font-family:"<<fontName<<"}"<<endl;
	c_out1<<".nT {font-family:"<<fontName<<"}"<<endl;
	c_out1<<".eG {font-family:"<<fontName<<"}"<<endl;
	c_out1<<"</style>";
    
	path=fileName(inputData.data["inputFile"]);
	cout_<<"path="<<inputData.data["inputFile"]<<endl;
	//cout_<<"inputData.data[\"inputFile\"]="<<inputData.data["inputFile"];
	
	c_out1<<"<div style=\"position:absolute; left:0px; top:0px;\">";
    time_t seconds;
    seconds = time (NULL);
	c_out1<<"<img class=\"srcImg\" src=\""<<path<<"\"/>\n";
    
	// заполнение массива координат прямоугольника
	for ( strInd=(int)strArray.size()-1; strInd >=0; strInd-- ){
		strFlag=strArray[strInd].stringFlag; 
		x0=strArray[strInd].x0;   x1=strArray[strInd].x1;
		y0=strArray[strInd].y0;   y1=strArray[strInd].y1;
		c_out1<<"<div class=\"sT\" id=\"s"<<strInd<<"\" style=\"left:";
		c_out1<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
		c_out1<<y1-y0<<"px;\" ";
		c_out1<<"LimY0=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"LimY1=\""<<strArray[strInd].LimY0<<"\" ";
		c_out1<<"y0=\""<<strArray[strInd].y0<<"\" ";
		c_out1<<"y1=\""<<strArray[strInd].y1<<"\" ";
		c_out1<<"x1=\""<<strArray[strInd].x1<<"\" ></div>\n";
		
		if(strFlag<1)continue; // если stringFlag==0 то строку не заполняем квадратиками букв (в строке линия)
		for ( m=0; m < strArray[strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[strInd].wordArray[m];
			//if(wP->correlation<0)continue;  //word marked as text frame etc.
            
			x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;
			gY=strArray[strInd].y1-y0; if(gY<0)gY=15;
			DT("y0="<<y0<<" y1="<<y1<<" x0="<<x0<<" x1="<<x1<<" LimY0="<<strArray[strInd].LimY0<<" LimY1="<<strArray[strInd].LimY1<<END);
						
            c_out1<<"<div class=\"dE\" id=\"g"<<index<<"\" c=\""<<wP->correlation;
            c_out1<<"\" style=\"left:"<<x0<<"px; top:"<<y0<<"px; width:"<<x1-x0<<"px; height:";
            c_out1<<y1-y0<<"px; ";
            if(wP->correlation<70){c_out1<<" border-color:#ff0000; border-width:3px;";}
            if(wP->correlation>70&&wP->correlation<80){c_out1<<" border-color:#cc0033; border-width:2px;";}
            
            c_out1<<"\">\n<div class=\"dM\" style=\"width: "<<x1-x0<<"px; height:3px;\"></div>\n";
            //c_out1<<wP->OCRIndex;
            c_out1<<"<a href=\"ocr.php?xml=<ocrData>edit<ocrData><index>"<<wP->letterIndex<<"</index>\", target=\"_blank\" ><font size=5>"<<"*"<<"</font></a>";
            
            c_out1<<"<div class=\"nG\" id=\""<<index<<"\" style=\" top:"<<gY<<"px\"";
            c_out1<<"onClick=\"edit("<<index<<")\" onBlur=\"setText("<<index<<")\">";
            GLetter *letter=aliKali->getLetter(wP->letterIndex);
            if(letter->OCRIndex==3){c_out1<<"_";}
            letter->destroy();
            c_out1<<wP->name
            // <<"<font size=\"4px\">/"<<wP->correlation<<"</font>"
            <<"</div></div>";
            //c_out1<<index<<")\" onBlur=\"setText("<<index<<")\">"<<wP->name<<"</div></div>";
            
            index++;
        
			//DT("n="<<wP->name.c_str()<<" c="<<wP->correlation<<END);
			//if(wP->stackFlag>0){
			//ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии салатовый
			//}else{
			//ImageEnView1->ObjPenColor[in]=(TColor)0xff1400; // цвет линии светло синий
			//}
			
		} // m
		
	} // strInd
    cout<<"@@1@";
    string exp="(<span class=\"c\" style=\"color:#FF0000\">([^<]*)</span><span class=\"c\" style=\"color:#FF0000\">([^<]*)</span>):|:<span class=\"c\" style=\"color:#FF0000\">\\1\\2</span>:|:3";
    string res=mainString;
    int i=0;
    while(1){
       regExpReplace(mainString, exp);
       if(res==mainString)break;
        i++; if(i>50)break;
       res=mainString;
    }
     

    
	c_out1<<"<br><br><br><font size=\"24\">"<<inputData.startIndex<<" "<<mainString<<"</font>"<<endl;
    
	c_out1<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"post\" action=\"/cgi/yagpoOCR.cgi\" name=\"main\">\n";
	c_out1<<"<input  name=\"inputFile\" id=\"inputFile\" type=\"hidden\" value=\""; c_out1<<inputData.data["inputFile"];c_out1<<"\"/><br/>\n";
	c_out1<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setLetter\"/><br/>\n";
	c_out1<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; c_out1<<inputData.data["inputFolder"];c_out1<<"\"/><br/>\n";
    //	c_out1<<"<button name=\"submit\" value=\"submit\" type=\"submit\">Set letter in base</button><br/>\n";
    //	c_out1<<"<input  name=\"newLetter\"  type=\"checkbox\" value=\"true\" /><p>Set as new letter</p>\n";
	c_out1<<"<textarea ID=\"outputStr\" name=\"setLetter\" cols=\"110\" rows=\"7\"></textarea>\n";
	c_out1<<"</form>";
	//string t;
	//t=str_replace("\n","</div><br/><div class=\"nT\">",mainString);
	//c_out1<<"<div class=\"nT\">"<<mainString<<"</div>";
	c_out1<<"</div></body>";
	
    
	string strOut=c_out1.str();
    
    string str=inputData.data["inputFile"];
    str=substr(0,(int)str.rfind("."),str);
    str+=".html";   cout_<<str<<endl;
    writeText(strOut,str);
    
    
    //print=0;
	// x0,y0,x1,y1
	// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
	// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
	
	
}//_____________________________________________________________________________



void GMainEditor::exportOCR(){
	
	cout<<"Start collect XML"<<endl;
    for(int fileIndex=0;fileIndex<inputData.fileList.size();fileIndex++){
        vector<string>fileListNew;
        readDirectoryToArray(fileListNew,inputData.fileList[fileIndex],"html");
        string strData,str;
        int pos;
        
        str=inputData.fileList[fileIndex];
        string fileNameStr=fileName(str);
        str+="/_"+fileNameStr+"_ALL.html";   //cout_<<str<<endl;
        ostringstream out;
        
        out<<"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"><html><head>"<<endl
        <<"<meta HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=utf-8\"/></head>"<<endl<<"<body>"<<endl;
        
        for(int i=0;i<fileListNew.size();i++){
            if(fileListNew[i].find("_ALL.html")!=string::npos)continue;
            readText(strData,fileListNew[i]);
            cout_<<".";
            string page;
            
            pos=(uint)strData.find("id=\"pechaImg\" src=");
            if(pos!=string::npos){
                page=strData.substr(pos+19, strData.size());
            }
            pos=(uint)page.find("\"");
            if(pos!=string::npos){

                page=page.substr(0,pos+1);
                page+="}</font><br>";
            }	
            page="<br><font color=\"#000000\">{"+page;
            //cout<<"page="<<page<<endl;
            
            pos=(uint)strData.find("<!--OCR-->");
            if(pos!=string::npos){
                strData=strData.substr(pos,strData.size());
            }
            pos=(uint)strData.find("<!--endOCR-->");
            if(pos!=string::npos){
                strData=strData.substr(0,pos);
            }
            
            strData=page+strData;
            strData=str_replace("<br><br>","", strData);
            string exp="(<span class=\"c\" style=\"color:#FF0000\">([^<]*)</span><span class=\"c\" style=\"color:#FF0000\">([^<]*)</span>):|:<span class=\"c\" style=\"color:#FF0000\">\\1\\2</span>:|:3";
            regExpReplace(strData, exp);
        
            out<<strData;	 
            
        }
        strData=out.str();
        writeText(strData, str);
        string cmd="textutil -convert rtf \""+str+"\"";
        system(cmd.c_str());
        
        fileListNew.resize(0);
        readDirectoryToArray(fileListNew,inputData.fileList[fileIndex],"xml");
        cout_<<endl<<"start write xml fileList.size="<<fileListNew.size();
        
        str=inputData.fileList[fileIndex];
        fileNameStr=fileName(str);
        str+="/_"+fileNameStr+"_ALL.txt";   cout_<<"XML PATH="<<str<<endl;
        
        ostringstream c_out;
        
        c_out<<"{BOOK INDEX:"<<str<<"}\n";
            
        int pageIndex=0;
        string st;
        for(int i=0;i<fileListNew.size();i++){
            cout_<<".";
            strData="";
            if(fileListNew[i].find("_ALL.txt")!=string::npos)continue;
            readText(strData,fileListNew[i]);
            strData=str_replace("<br>", "\n", strData);
            while(1){
                pos=(uint)strData.find("<");
                if(pos!=string::npos){
                    st=strData.substr(0, pos);
                    pos=(uint)strData.find(">",pos);
                    st+=strData.substr(pos+1,strData.size());
                    strData=st;
                }else break;
            }
            
            st=fileName(fileListNew[i]);
            c_out<<"{P:index=\""<<pageIndex<<"\" file=\""<<str_replace(".xml","",st)<<"\"}";
            c_out<<strData;	
            pageIndex++;
            
        }
        c_out<<"{GPL OCRLib; gomde@mail.ru}"<<endl;
        string rezult=c_out.str();
        writeText(rezult,str);
        //cout_<<"size="<<rezult.size()<<" path="<<str;
    }    
	cout<<"done export;"<<endl;
}




