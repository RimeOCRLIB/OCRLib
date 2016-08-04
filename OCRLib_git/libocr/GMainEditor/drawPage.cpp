#include "GMainEditor.h"


///вывод результатов распознавания в HTML файл постранично.
void GMainEditor::drawOCRPage(vector<stringOCR>&strArray){
	int pageWide=0;  
	if(pechaImg->columns()>pechaImg->rows())pageWide=1;
    float scale;
	string fontName=aliKali->fontName;
	if(fontName=="")fontName="Kailasa";
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
		out<<"<div id=\"textOCR\" onClick=\"editOCRText()\" style=\"font-family:"<<fontName<<",Himalaya; font-size:1.6em; width:1600px; position:relative; top:0px; left:50px;\"><!--OCR--><br>"<<END;
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
        out<<"<div id=\"textOCR\" onClick=\"editOCRText()\" style=\"font-family:"<<fontName<<",Himalaya; font-size:1.6em; width: 1024px; position:absolute; top:0px; left:800px;\"><!--OCR--><br>"<<END;
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
	if(fontName=="")fontName="Kailasa";
	
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
	cout_<<"path@@="<<path<<END;
	
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
		
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width:1600px; position:absolute; top:350px; left:250px;\"><br>"<<END;
		c_out1<<mainString;
		c_out1<<"</div>";
		
	}else{
		c_out1<<"<div style=\"font-family:"<<fontName<<"; font-size:"<<(float)3500/pechaImg[0].columns()<<"em; width: 600px; position:absolute; top:20px; left:20px;\"><br>"<<END;
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
	//c_out<<"<div style=\"font-family:"<<fontName<<"; font-size:1.6em; width: 600px;\"><br>"<<END;
    //c_out<<mainString;
	//c_out<<"</div>\n";
	//c_out.close();
	
	string strOut=c_out1.str();
    string str=inputData.data["inputFile"];
    str=substr(0,str.rfind("."),str);
    str+=".html";   cout_<<str<<END;
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
	if(fontName=="")fontName="Kailasa";
    
	string path=inputData.data["root"]+"edit/headerPageLetter.xml";
	replace(path.begin(),path.end(),'\\','/');
	cout_<<"pathDRAW="<<path<<END;
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
	cout_<<"path="<<inputData.data["inputFile"]<<END;
	//cout_<<"inputData.data[\"inputFile\"]="<<inputData.data["inputFile"];
	
	c_out1<<"<div style=\"position:absolute; left:0px; top:0px;\">";
    time_t seconds;
    seconds = time (NULL);
	c_out1<<"<img class=\"srcImg\" src=\""<<path<<"\"/>\n";
    
	// заполнение массива координат прямоугольника
	for ( strInd=strArray.size()-1; strInd >=0; strInd-- ){  
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
            //<<"<font size=\"4px\">/"<<wP->correlation<<"</font>"
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
    str=substr(0,str.rfind("."),str);
    str+=".html";   cout_<<str<<END;
    writeText(strOut,str);
    
    
    //print=0;
	// x0,y0,x1,y1
	// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
	// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
	
	
}//_____________________________________________________________________________



void GMainEditor::writePageXML(string &xmlString){
    int print=0;
	
	DT("start save XML file"<<END);
	string  path=inputData.data["inputFile"];
	path=substr(0,(int)path.rfind("."),path);
	string volume=path;
	string fileIndex=fileName(path);
	path+=".xml";
	DT("path="<<path<<END);
	
	
	volume=volume.erase(volume.rfind("/"),volume.size());
	volume=substr((int)volume.rfind("/")+1,(int)volume.size(),volume);   cout_<<"volume="<<volume<<END;
	
	ostringstream out;
    out<<"<?xml version=\"1.0\"?>"<<endl;
    out<<"<text:text xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:text=\"http://www.tbrc.org/models/text#\" RID=\""<<"";
    out<<"\" lang=\"bo_ZH\" volume=\""<<volume<<"\" num=\""<<fileIndex<<"\" fromWork=\"\" fromVolume=\""<<volume<<"\" start=\"0\" last=\"0\">"<<endl;
    out<<xmlString<<endl;
    out<<"</text:text>";
    string str=out.str();
    writeText(str,path);
}

void GMainEditor::readLetterStat(){
/*	
	string str,vData;
	str=inputData.fileList[0];
    DIR *dir;
    BDBCUR *cur;
    char *key, *value;
    int id;
    
    //обнуляем статистку использования букв
    for(size_t i;i<aliKali[0].letterCount;i++)aliKali[0][i]->cn=0;
    
    
    if(( dir=opendir(str.c_str()))==NULL){
        str=substr(0,str.rfind("/"),str);
    }
	cout_<<"Start read inputData.bookDataDB"<<str<<END;
	
    int ecode;
    //create the object /
    inputData.bookDataDB = tcbdbnew();
    string dictPath=str+"/_book.tcb";    
    
    //open the database /
    if(!tcbdbopen(inputData.bdb, dictPath.c_str(), BDBOWRITER | BDBOCREAT)){
        ecode = tcbdbecode(inputData.bdb);
        fprintf(stderr, "open error: %s\n", tcbdberrmsg(ecode));
    }    
    cout_<<" load records "<<inputData.bookDataDB->rnum<<endl;
    
    //read stat data
    cur = tcbdbcurnew(inputData.bookDataDB);
    tcbdbcurfirst(cur);
    while((key = tcbdbcurkey2(cur)) != NULL){
        value = tcbdbcurval2(cur);
        if(value){
            //printf("%s:%s\n", key, value);
            id=atoi(key);
            aliKali[0][id]->cn=1;
            free(value);
        }
        free(key);
        tcbdbcurnext(cur);
    }
    tcbdbcurdel(cur);
*/    

}


void GMainEditor::readCorrectionTable(){
    
	cout_<<"Start read inputData.data[\"correctionTable\"]"<<inputData.data["correctionTable"]<<END;
	string str,vData;
	vector<string>dataArray;
	xml_node data;
	xml_document doc;
	int w,h;
	char *p;
	setHex(); //set hex code for char string to byte conversion
	
	xml_parse_result stat;
	stat=doc.load_file(inputData.data["correctionTable"].c_str());
	
	if(!stat){cout_<<"correctionTable not loaded";
		if(stat.offset>0){
			cout_<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
			ifstream file (inputData.data["correctionTable"].c_str(), ios::in|ios::binary|ios::ate);
			//int lineIndex=0;
			ifstream::pos_type size;
			//long size_block = file.tellg();
			
			if (file.is_open()){
				int size = 1024;
				char *memblock = new char [size];
				//file.seekg (0, ios::beg);
				file.seekg (stat.offset-512, ios::beg);
				file.read (memblock, size);
				str=memblock;
				//str=str_replace("<","{", str);
				//str=str_replace(">","}", str);
				//file.read (memblock, size_block);
				file.close();
				cout_<<"line offset="<<str<<endl;
				//c_out<<"line offset="<<memblock<<endl;
				delete memblock;     //cout_<<"size_block="<<size_block<<endl;
			}
			exit(0);
		}	
	}
	
	
	correctionTable.resize(0);
	
	for (xml_node row = doc.child("rec"); row; row = row.next_sibling("rec")){
		stringOCR wp;
		wp.file=row.attribute("f").value();
		wp.name=row.attribute("n").value(); //if(wp.name.find("{br}",0)!=string::npos)continue;
		wp.idNumber=strtoul(row.attribute("id").value(),NULL,0); //cout_<<" wp.idNumber="<<wp.idNumber<<endl;
		wp.correlation=atoi(row.attribute("c").value());
		data=row.child("img");
		w=atoi(data.attribute("w").value());
		h=atoi(data.attribute("h").value());  //cout_<<"open w="<<w<<" h="<<h<<endl;
		vData=data.child_value();
		wp.img=GBitmap::create();
		wp.img->h=h;
		wp.img->w=w;
		
		char *cBuff=(char*)malloc(vData.size()/2);
		int i=0; unsigned short *s;
        for(int m=0;m<vData.size();m+=2){
            s=(unsigned short*)&vData[m];
            cBuff[i]=hexCode[(*s)]; 
            //cout_<<buff[0]<<buff[1]<<" "<<*s<<" cBuff["<<i<<"]="<<(short)cBuff[i]<<endl;
            i++;
        }
		wp.img->TIFF_string.resize(vData.size()/2);
		p=&wp.img->TIFF_string[0];
		memcpy(p, cBuff, vData.size()/2); free(cBuff);
		wp.img->pack_flag=1;
        
		correctionTable.push_back(wp);
        //cout_<<"correctionTable.image="<<correctionTable[correctionTable.size()-1].img[0].TIFF_string.size()<<endl;
		
	}	
	
	
	if(!is_file(inputData.data["correctionPages"]))return;
	stat=doc.load_file(inputData.data["correctionPages"].c_str());
	
	if(!stat){cout_<<"correctionPage not loaded";
		if(stat.offset==0)return;
		cout_<<stat.description()<<" offset="<<stat.offset<<" line="<<stat.line<<endl;
		ifstream file (inputData.data["correctionPages"].c_str(), ios::in|ios::binary|ios::ate);
		//int lineIndex=0;
		ifstream::pos_type size;
		//long size_block = file.tellg();
		
		if (file.is_open()){
			int size = 1024;
			char *memblock = new char [size];
			//file.seekg (0, ios::beg);
			file.seekg (stat.offset-512, ios::beg);
			file.read (memblock, size);
			str=memblock;
			//str=str_replace("<","{", str);
			//str=str_replace(">","}", str);
			//file.read (memblock, size_block);
			file.close();
			cout_<<"line offset="<<str<<endl;
			//c_out<<"line offset="<<memblock<<endl;
			delete memblock;     //cout_<<"size_block="<<size_block<<endl;
		}
		exit(0);
	}
	
    
	
    
	for (xml_node row = doc.child("rec"); row; row = row.next_sibling("rec")){
		stringOCR wp;
		wp.file=row.attribute("f").value();
		wp.name=row.attribute("n").value(); //if(wp.name.find("{br}",0)!=string::npos)continue;
		wp.lineIndex=atoi(row.attribute("l").value());
		wp.idNumber=strtoul(row.attribute("id").value(),NULL,0); //cout_<<" wp.idNumber="<<wp.idNumber<<endl;
		wp.correlation=100;
		int flag=1;  //cout_<<wp.file<<"/"<<wp.lineIndex<<endl;
		for(int i=0;i<correctionTable.size();i++){
			if(correctionTable[i].file==wp.file&&correctionTable[i].lineIndex==wp.lineIndex){
				correctionTable[i].name=wp.name; 
				correctionTable[i].correlation=wp.correlation;
				flag=0; //cout_<<"  +"<<correctionTable[i].file<<"/"<<wp.lineIndex<<endl;
				break;
			}	
            
		}
		if(flag)correctionTable.push_back(wp);
		
	}	
	
	cout_<<"done read correctionTable.size()="<<correctionTable.size()<<endl;
	
	
	//stringOCR d;
	//sort(correctionTable.begin(),correctionTable.end(),stringOCR::stringOCR_sort);
    
	//WriteImageData(correctionTable[0].img,"/1.jpg",0);
    
}


void GMainEditor::exportOCR(){
	
	cout_<<"Start collect XML"<<END;
    for(int fileIndex=0;fileIndex<inputData.fileList.size();fileIndex++){
        vector<string>fileListNew;
        readDirectoryToArray(fileListNew,inputData.fileList[fileIndex],"html");
        string strData,str;
        int pos;
        
        str=inputData.fileList[fileIndex];
        string fileNameStr=fileName(str);
        str+="/_"+fileNameStr+"_ALL.html";   cout_<<str<<END;
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
        str+="/_"+fileNameStr+"_ALL.txt";   cout_<<"XML PATH="<<str<<END;
        
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


void GMainEditor::drawLettersInCorrectionTable(int mode){
	///GBitmap *img;
	static int x0,y0,x1,y1;
	//static int baseIndex=0;
	char cbuf[3];
	string path=fileName(inputData.data["inputFile"]);
	int w,h,print=0;
	cbuf[2]=0;
	int c;
	string str;
	
	if(mode==LETTER_ADD){
		for(int index=0;index<correctionTable.size();index++){
			//cout_<<" path="<<path<<" correctionTable["<<index<<"].file="<<correctionTable[index].file<<" "<<END;
			//if(correctionTable[index].delimeter==" "&&correctionTable[index].name=="")continue;
			if(correctionTable[index].file!=path)continue;
			x0=correctionTable[index].x0-100;
			y0=correctionTable[index].y0-50;
			x1=correctionTable[index].x1+100;
			y1=correctionTable[index].y1+120;
			w=x1-x0; 
            if(w%8<4){
                w-=w%8; //выравнивание битмапа по 8 увеличивает скорость обращения и необходимо для компрессии
            }else{
                w-=w%8; 
            }
            
			h=y1-y0;   
			DT("x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<endl);
			
			GBitmap *img=GBitmap::createRegion(pechaImg,x0,y0, w, h);  DT(" img->packFlag1="<<img->pack_flag);
			//LoadImageData(img,pechaImg,x0,y0, w, h); cout_<<" img->packFlag2="<<img->pack_flag;
			correctionTable[index].img=GBitmap::create(img);
			correctionTable[index].imgFlag=1;
			img->destroy();
		}
        
        ostringstream out;
        string record;
        for(int index=0;index<correctionTable.size();index++){
            if(!correctionTable[index].imgFlag){cout_<<"NO IMAGE"; continue;}
            out<<"<rec id=\""<<correctionTable[index].idNumber<<"\" f=\""<<correctionTable[index].file<<"\" ";
            out<<"n=\""<<correctionTable[index].name<<"\" c=\""<<correctionTable[index].correlation<<"\">";
            //int t=correctionTable[index].img->columns()*correctionTable[index].img->rows()/8;
            record="";
            str=drawToTiffString(correctionTable[index].img);
            //str="34123514352346245634563456y345673567u4567u34567u";
            for(int m=0;m<str.size();m++) {
                c=(unsigned char)str[m];
                record+=charToHex(c);
                //record+="/";
                
                //*p=(unsigned int*)correctionTable[index].img->bytes_data+m;
                //in=*p;
                //sprintf(buff,"%X",in); record+=buff;record+=";";
                //if(correctionTable[index].img->bytes_data[m]>128){
                //   record+="0";
                //}else{
                //   record+="1";
                //}
                
            } 
            out<<"<img w=\""<<correctionTable[index].img->columns()<<"\" h=\""<<correctionTable[index].img->rows()<<"\">"<<record<<"</img></rec>\n";
            //break;
        }	
        
        string str=out.str();
        cout_<<"START WRITE CORRECTION"<<endl;
        writeToLog(str,inputData.data["correctionTable"].c_str(),"_1.txt");
        cout_<<"DONE WRITE CORRECTION"<<endl;
	}
}//_____________________________________________________________________________

void GMainEditor::drawLettersInCorrectionPage(int mode){
	///GBitmap *img;
	static int x0,y0,x1,y1;
	//static int baseIndex=0;
	char cbuf[3];
	string path=fileName(inputData.data["inputFile"]);
	int w,h,print=1;
	cbuf[2]=0;
	int c,lineIndex=-1;
	string str,strIndex;
	
	//cout_<<"drawLettersInCorrectionPage"<<endl;
	
	if(mode==LETTER_ADD){
		
		//cout_<<"mainString="<<mainString;
		vector<string> lineArray=explode("<",mainString);
		
		cout <<"start write in correction table";
		
		
		//for(int in=0;in<lineArray.size();in++)cout_<<"i="<<in<<" "<<lineArray[in]<<endl;
        
		
		for(int index=0;index<correctionTable.size();index++){
			//cout_<<" path="<<path<<" correctionTable["<<index<<"].file="<<correctionTable[index].file<<" "<<END;
            
			//if(correctionTable[index].delimeter==" "&&correctionTable[index].name=="")continue;
			if(correctionTable[index].file!=path)continue;
			if(lineIndex!=correctionTable[index].lineIndex){
				lineIndex=correctionTable[index].lineIndex;
				x0=0;
				y0=correctionTable[index].LimY0;
				x1=correctionTable[index].x1;
				y1=correctionTable[index].LimY1;
				w=x1-x0; 
				if(w%8<4){
					w-=w%8; //выравнивание битмапа по 8 увеличивает скорость обращения и необходимо для компрессии
				}else{
					w-=w%8; 
				}
				
				h=y1-y0;   
				
				ostringstream out1_;
				out1_<<"l=\""<<lineIndex<<"\"";
				string indexStr=out1_.str(); //cout_<<"indexString="<<indexStr<<endl;
				str="";
				int startFlag=0;
				for(int i=0;i<lineArray.size();i++){
				    //cout_<<" lineArray["<<i<<"]="<<lineArray[i]<<" str0="<<str<<endl;
					if(lineArray[i].find(indexStr)!=string::npos){
						if(startFlag==0){  ///начало строки
							if(i>0){
								if(lineArray[i-1].find("style=")!=string::npos){
								    str+="<"+lineArray[i-1]; 
									str+="<"+lineArray[i]; //cout_<<"style"<<i<<" "<<" lineArray[i]="<<lineArray[i]<<" str_s="<<str<<endl<<endl;
								}else{
									str+="<"+lineArray[i];    //cout_<<"str_1"<<i<<" "<<" lineArray[i]="<<lineArray[i]<<" str="<<str<<endl<<endl;
								}	
							}else{
								str+="<"+lineArray[i];       //cout_<<"str_2"<<i<<" "<<" lineArray[i]="<<lineArray[i]<<" str="<<str<<endl<<endl;
                                
							}
							startFlag++;
						}else{
						    str+="<"+lineArray[i]; 
						} 					
					}else{
						if(lineArray[i]=="/c>"&&startFlag){str+="</c>";continue;}
						if(startFlag>0){ ///окончание строки
                            if(i<lineArray.size()&&lineArray[i].find("/span")!=string::npos){	
                                str+="<"+lineArray[i];
                                //cout_<<"span"<<i<<" "<<" lineArray[i]="<<lineArray[i]<<" str_3="<<str<<endl<<endl;
                                startFlag=0;
                            }
						}
                        
					}
                    
				}
				//cout_<<"result str="<<str<<endl;
				
				DT("	h="<<	h<<" x0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<" correctionTable[index].lineIndex="<<correctionTable[index].lineIndex<<endl);
				
				GBitmap *img=GBitmap::createRegion(pechaImg,x0,y0, w, h);  //DT(" img->packFlag1="<<img->pack_flag);
				//WriteImageData(img,"/1.jpg", 0);
				//LoadImageData(img,pechaImg,x0,y0, w, h); cout_<<" img->packFlag2="<<img->pack_flag;
				correctionTable[index].img=GBitmap::create(img);
				correctionTable[index].imgFlag=1;
				ostringstream out_;
				out_<<XMLEncode(str)<<XMLEncode("<font size=\"2\">{")<<
				//correctionTable[index].file<<"|"<<lineArray.size()-correctionTable[index].lineIndex-1<<XMLEncode("}</font>");  ///old table
				correctionTable[index].file<<"|"<<correctionTable[index].lineIndex<<XMLEncode("}</font>");
				
				correctionTable[index].name=out_.str();
				correctionTable[index].idNumber=0;
				correctionTable[index].correlation=0;
				img->destroy();
			}	
		}
		
		ostringstream out;
		string record;
		for(int index=0;index<correctionTable.size();index++){
			if(!correctionTable[index].imgFlag){continue;} //cout_<<"NO IMAGE";
			out<<"<rec id=\""<<correctionTable[index].idNumber<<"\" f=\""<<correctionTable[index].file<<"\" ";
			out<<"n=\""<<correctionTable[index].name<<"\" c=\""<<correctionTable[index].correlation<<"\">";
			//int t=correctionTable[index].img->columns()*correctionTable[index].img->rows()/8;
			record="";
			str=drawToTiffString(correctionTable[index].img);
			
			//str=correctionTable[index].img->drawToTIFFString(0);
			
			//str="34123514352346245634563456y345673567u4567u34567u";
			for(int m=0;m<str.size();m++) {
				c=(unsigned char)str[m];
				record+=charToHex(c);
				//record+="/";
				
				//*p=(unsigned int*)correctionTable[index].img->bytes_data+m;
				//in=*p;
				//sprintf(buff,"%X",in); record+=buff;record+=";";
				//if(correctionTable[index].img->bytes_data[m]>128){
				//   record+="0";
				//}else{
				//   record+="1";
				//}
				
			} 
			out<<"<img w=\""<<correctionTable[index].img->columns()<<"\" h=\""<<correctionTable[index].img->rows()<<"\">"<<record<<"</img></rec>\n";
			//break;
		}	
        
		str=out.str();
		cout_<<"START WRITE CORRECTION"<<endl;
		writeToLog(str,inputData.data["correctionTable"].c_str(),"_1.txt");
		cout_<<"DONE WRITE CORRECTION"<<endl;
		
	}
}//_____________________________________________________________________________


string GMainEditor::drawCorrectionTable(string data){
	if(correctionTable.size()==0)readCorrectionTable();
	drawCorrectionTablePict();
    
	string fontName=aliKali->fontName,str;
	if(fontName=="")fontName="Kailasa";
    
	ostringstream out;
	int limit=atoi(data.c_str());
	cout_<<"limit="<<limit<<" correctionTable.size()="<<correctionTable.size()<<endl;
    
	string path=inputData.data["root"]+"edit/headerCorrectionTable.xml";
	readText(str,path);
    out<<str;
    
	
	out<<".b  {background-color:#990000;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b0 {background-color:#990000;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b30{background-color:#CC0000;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b40{background-color:#ff3333;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b50{background-color:#ff6666;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b70{background-color:#FF9999;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b80{background-color:#FFFF99;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b90{background-color:#FFFFCC;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	out<<".b95{background-color:#CCFFCC;position:absoute; width:105px; height:30px; overflow:hidden; text-align:center; font-size:24px; font-family:"<<fontName<<"}\n";
	
	out<<".c{background-color:#ffcc99;position:absoute; text-align:center; font-family:"<<fontName<<"}</style>\n";
	//out+="data=";་out+=data;
	out<<"<table><tr>";
	int step=1;
	//char buff[15];
	int w,h;
	int c;
	
    for(int i=limit-1;i<correctionTable.size()&&i<limit+500;i++){
		
		//if(data!=""){
		//	if(correctionTable[i].name.find(data)==-1)continue;
		//}
		if(!correctionTable[i].img)continue;
		w=correctionTable[i].img->w/3;
		h=correctionTable[i].img->h/3;
		
		//if(w>180){h=h*180/w;w=180;}
		//if(h>100){w=w*100/h;h=100;}
		out<<"<td><div class=\"gL_\" id=\""<<correctionTable[i].idNumber<<"\" onClick=\"editCell(";
		out<<correctionTable[i].idNumber<<")\">";
		//<div class=\"g_\"><img id=\""<<correctionTable[i].idNumber<<"\" class=\"i_\" src=\"/?m=grab&l="<<correctionTable[i].idNumber<<"\" width=\""<<w<<"\" height=\""<<h<<"\"/></div>";
		out<<"</div></div><textarea class=\"";
		c=correctionTable[i].correlation;
		if(c>95&&c<101){out<<"b95";}
		if(c>90&&c<96){out<<"b90";}
		if(c>80&&c<91){out<<"b80";}
		if(c>70&&c<81){out<<"b70";}		
		if(c>50&&c<71){out<<"b50";}
		if(c>40&&c<51){out<<"b40";}
		if(c>30&&c<41){out<<"b30";}
		if(c>0&&c<31){out<<"b0";}
		if(c==0){out<<"c";}
		
		//out<<"\" >"<<correctionTable[i].correlation<<"| "<<correctionTable[i].name<<"</div>";
		out<<"\" id=\"g"<<correctionTable[i].idNumber<<"\" onClick=\"editText('g";
		out<<correctionTable[i].idNumber<<"')\" onBlur=\"setText("<<correctionTable[i].idNumber<<", '"<<correctionTable[i].file<<"')\" >"<<correctionTable[i].name<<"</textarea>";
		
		out<<"</td>\n";
		if(step==15){
			out<<"</tr><tr>\n";
			step=0;	
		}step++;
		
	}
	
	out<<"</tr></table>";
	out<<"<div id=\"selector\"></div>";
	out<<"<form>";
	out<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setCorrectionTable\">\n";
	out<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; out<<inputData.data["inputFolder"];out<<"\">\n";
	out<<"<textarea id=\"outputStr\" name=\"setCorrectionTable\" rows=\"7\" style=\"width:650px\" onBlur=\"alert(1)\"></textarea>\n";
	out<<"</form>";
	
	out<<"<div class=\"img\"><img src=\"correctionTable.jpg\" width=\"2200px\" height=\"4400px\"></div>";
	
	
	return out.str();	
}//_____________________________________________________________________________


string GMainEditor::drawCorrectionPage(){
	if(correctionTable.size()==0)readCorrectionTable();
    if(!correctionTable.size())return "";
    
    cout_<<"correctionTable.image="<<correctionTable[0].img[0].TIFF_string.size()<<endl;
    
	int indexPict=0;
	drawCorrectionPagePict();
	
	string fontName=aliKali->fontName,str;
	if(fontName=="")fontName="Kailasa";
	
	ostringstream out;
	int limit=1;//atoi(data.c_str());
	cout_<<"limit="<<limit<<" correctionTable.size()="<<correctionTable.size()<<endl;
	
	string path=inputData.data["root"]+"edit/headerCorrectionTable.xml";
    readText(str,path);
    out<<str;
	
	
    //	out<<".c{text-align:left;  padding-left: 41px; font-size:30px; height:32px; font-family:"<<fontName<<"}</style>\n";
	out<<".c{font-size:30px; font-family:"<<fontName<<"}</style>\n";
	//out+="data=";་out+=data;
    
	//int step=1;
	//char buff[15];
	int w,h;
	
    for(int i=0;i<correctionTable.size();i++){
		
		//if(data!=""){
		//	if(correctionTable[i].name.find(data)==-1)continue;
		//}
		if(!correctionTable[i].img->TIFF_string.size()){
			cout_<<"NO IMAGE"<<i<<" file="<<correctionTable[i].name<<endl;
			continue;
		}
		w=correctionTable[i].img->w/3;
		h=correctionTable[i].img->h/3;
		
		
		//if(w>180){h=h*180/w;w=180;}
		//if(h>100){w=w*100/h;h=100;}
        
		out<<"<img src=\"correctionTable"<<indexPict<<".jpg\" width=\"800px\" ><br>"; indexPict++;
		//out<<"<div class=\"gL_\" id=\""<<correctionTable[i].idNumber<<"\" onClick_=\"editCell(";
		//out<<correctionTable[i].idNumber<<")\">";
		out<<" &nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;<span class=\"c";
		
		correctionTable[i].name=str_replace("<br>", "", correctionTable[i].name);
		//out<<"\" >"<<correctionTable[i].correlation<<"| "<<correctionTable[i].name<<"</div>";
		out<<"\" id=\"g"<<correctionTable[i].idNumber<<"\" onClick_=\"editText('g";
		out<<correctionTable[i].idNumber<<"')\" onBlur_=\"setText("<<correctionTable[i].idNumber<<", '"<<correctionTable[i].file<<"')\" >"<<correctionTable[i].name<<"</span>";
		out<<"<br>\n";
        
		
	}
    
	//out<<"<form>";
	//out<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setCorrectionTable\">\n";
	//out<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; out<<inputData.data["inputFolder"];out<<"\">\n";
	//out<<"<textarea id=\"outputStr\" name=\"setCorrectionTable\" rows=\"7\" style=\"width:650px\" onBlur=\"alert(1)\"></textarea>\n";
	//out<<"</form>";
	
    
    return out.str();	
}//_____________________________________________________________________________


string GMainEditor::drawLetterBlock(unsigned int idNumber){
	//cout_<<"draw letter"<<endl;
	string str;
	int in=-1;
	//cout_<<"input id="<<idNumber<<" correctionTable.size()="<<correctionTable.size()<<endl;
	//находим нужную букву по idNumber
	for(int i=0;i<correctionTable.size();i++){
		//cout_<<"correctionTable["<<i<<"].idNumber="<<correctionTable[i].idNumber<<endl;
		if(correctionTable[i].idNumber==idNumber){
			in=i; break;
		}
	}
	//cout_<<"INDEX="<<in<<" TIFF_string_size="<<correctionTable[in].img->TIFF_string_size<<endl;
	if(in<0)return "";
	//correctionTable[in].img->unpackImg();
	//cout_<<"done draw";
	//str=correctionTable[in].img->TIFF_string;
	
	return correctionTable[in].img->TIFF_string;
};


void GMainEditor::drawCorrectionPagePict(){
	cout_<<"draw table Page"<<endl;
    int print=1;
	//int w,h; 
	string str;
	int imgIndex=0;
	//GBitmap *blank=GBitmap::create(w*5+40,h+120);
	int y=0;
	GBitmap *pageImg;
    
    for(int i=0;i<correctionTable.size();i++){
        
		DT("correctionTable["<<i<<"].idNumber="<<correctionTable[i].idNumber<<"TIFF_string="<<correctionTable[i].img->TIFF_string.size()<<endl);
		
        GBitmap *letterImg=loadFromTiffString(correctionTable[i].img->TIFF_string,1); 
		if(!letterImg){cout_<<"NO IMAGE";continue;}
		
		if(y==0)pageImg=GBitmap::create(letterImg->columns(),3000);
		if(y+letterImg->rows()+120>pageImg->rows()){ 
			ostringstream out_;
			out_<<"edit/_correctionPage"<<imgIndex<<".jpg";
			str=out_.str(); cout_<<"W1";
			WriteImageData(pageImg,str,0);
			pageImg->destroy();
			pageImg=GBitmap::create(letterImg->columns(),3000);
			y=0;
			
		}	
		pageImg->drawImg(letterImg,0,y+60);
		y+=letterImg->rows();
        //if(y>15000-300){
        ostringstream out_;
        out_<<"edit/correctionTable"<<imgIndex<<".jpg";
        str=out_.str(); cout_<<"W2";
        WriteImageData(letterImg,str,0);
        letterImg->destroy();
        //tableImg=GBitmap::create(w,h);
        imgIndex++;
	}
	
    
};

void GMainEditor::drawCorrectionTablePict(){
	cout_<<"draw table"<<endl;
	GBitmap *tableImg=GBitmap::create(7500,15000);
	string str;
	int w=377;
	int h=347;
	GBitmap *blank=GBitmap::create(w*5+40,h+120);
	//int in=-1;
	//cout_<<"input id="<<idNumber<<" correctionTable.size()="<<correctionTable.size()<<endl;
	//находим нужную букву по idNumber
	int step=0,y=0;
	for(int i=0;i<correctionTable.size();i++){
		//cout_<<"correctionTable["<<i<<"].idNumber="<<correctionTable[i].idNumber<<endl;
		
        GBitmap *letterImg=loadFromTiffString(correctionTable[i].img->TIFF_string,1);
		tableImg->drawImg(blank,w*step-40,y-90);
		tableImg->drawImg(letterImg,w*step+40,y+40);
		step++;
		if(step==15){
			//tableImg->drawImg(blank,w*step-40,y-90);
			y+=h;step=0;
		}
		letterImg->destroy();
		
		//if(correctionTable[i].idNumber==idNumber){
        //in=i; break;
		//}
	}
    
	WriteImageData(tableImg,"edit/correctionTable.jpg",0);
    tableImg->destroy();
	//cout_<<"INDEX="<<in<<" TIFF_string_size="<<correctionTable[in].img->TIFF_string_size<<endl;
	//if(in<0)return "";
	//correctionTable[in].img->unpackImg();
	//cout_<<"done draw";
	//str=correctionTable[in].img->TIFF_string;
	
};

string GMainEditor::drawEditLetterBlock(string &resultOCR,unsigned int idNumber){
	//char buff[15];
	//int indexMask, x0,y0,w,h;
	int in;
    
	//находим нужную букву по idNumber
	for(int i=0;i<correctionTable.size();i++){
		if(correctionTable[i].idNumber==idNumber){
			in=i; break;
		}
	}
	//int zoom=3;
	//int size=PICT_SIZE*zoom;
	//cout_<<"drawEditLetter"<<endl;
	ostringstream out;
    
	string path=inputData.data["root"]+"edit/headerEditLetterBlock.xml";
	string str;
	readText(str,path);
    out<<str;
	
	out<<"var idNumber="<<idNumber<<";";
	
	//out<<"var pictW="<<size<<";";
	//out<<"var zoom="<<zoom<<";";
	out<<"</script>\n";
	out<<"</head><body topmargin=0; leftmargin=0;>\n";
	//out<<"<style>.a{border:1px solid green; width:"<<size<<"px; height:"<<size<<"px}</style>";
	//out<<"<form enctype=\"application/x-www-form-urlencoded\" method=\"get\" action=\"javastcript:submit("<<idNumber<<")\" name=\"main\">\n";
	
	out<<"<table cellspacing=\"0\" cellpadding=\"0\" border=\"0\"><tr><td valign=\"top\">";	
	out<<"<div class=\"a\" id=\">"<<idNumber<<"\" )><img src=\"/?xml=<ocrData>grab<ocrData><index>"<<idNumber<<"</index>\" ></div>";
	out<<resultOCR;
    
	out<<"</td></tr><tr><td valign=\"top\">";
	GLetter *letter=aliKali->getLetter(in);
	out<<"<br><textarea class=\"nG\" id=\""<<letter->idNumber;
	out<<"\" onClick=\"edit("<<letter->idNumber<<")\" onBlur=\"setText("<<letter->idNumber;
	out<<")\">"<<correctionTable[in].name<<"</textarea><br>";
	//out<<"<div class=\"letterName\">"<<correctionTable[in].name<<"</div><br>";
	out<<"<textarea id=\"outputStr\" name=\"setBase\" rows=\"3\" style=\"width:420px\"></textarea>\n";
	//out<<"<font size=5>"<<aliKali[0][in]->idNumber<<"|"<<aliKali[0][in]->mask32Count()<<"</font><br>";
	out<<"<br><font size=3>Correlation value "<<correctionTable[in].correlation<<"</font><br>";
	out<<"<font size=3>Letter block width "<<correctionTable[in].img->columns()<<"</font><br>";
	out<<"<button onClick=\"submit()\">Set letter in base</button><br>\n";
	out<<"<button onClick=\"correlation("<<idNumber<<")\">OCR correction letter line</button><br>\n";
	out<<"<input type=\"text\" style=\"border:1px solid; width:100px\" id=\"searchLetter\"/>";
	out<<"<button onClick=\"openLetter()\">Open letter in base</button><br>\n";
	letter->destroy();
    
    out<<"</td></tr><table>";
	
    /*
     out<<"<div class=\"dE dM\" id=\"s";
     out<<aliKali[0][in]->idNumber<<"\" style=\"left:";
     out<<0<<"px; top:"<<size/2+aliKali[0][in]->y0*zoom<<"px; width:"<<size;
     out<<"px; height:"<<aliKali[0][in]->y1*zoom-aliKali[0][in]->y0*zoom<<"px; border: 1px solid grey;\" ></div>\n";
     
     //cout_<<"draw "<<in<<END;
     x0=PICT_SIZE/2-aliKali[0][in]->letterW/2+aliKali[0][in]->dX;
     y0=PICT_SIZE/2-aliKali[0][in]->letterH/2+aliKali[0][in]->dY;
     w=aliKali[0][in]->letterW;
     h=aliKali[0][in]->letterH;
     out<<"<div class=\"dE\" id=\"g"<<aliKali[0][in]->idNumber<<"\" style=\"left:"<<x0*zoom<<"px; top:"<<y0*zoom;
     out<<"px; width:"<<w*zoom<<"px; height:"<<h*zoom<<"px;\">\n";
     out<<"<div class=\"dM\" width:"<<w*zoom<<"px; height:"<<3<<"px;></div>\n";
     out<<"</div>";
     
     out<<"</td></tr><tr><td>";
     out<<"<input  name=\"inputFile\"  type=\"hidden\" value=\""; out<<inputData.data["inputFile"];out<<"\">\n";
     out<<"<input  name=\"ocrData\"  type=\"hidden\" value=\"setBase\">\n";
     out<<"<input  name=\"inputFolder\"  type=\"hidden\" value=\""; out<<inputData.data["inputFolder"];out<<"\">\n";
     out<<"<textarea id=\"outputStr\" name=\"setBase\" rows=\"3\" style=\"width:"<<size<<"px\"></textarea>\n";
     out<<"</td></tr><table>";
     */
    //out<<"</form>";	
	return out.str();	
}//_____________________________________________________________________________




