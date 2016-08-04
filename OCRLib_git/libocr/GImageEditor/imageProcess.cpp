#include "GImageEditor.h"
#include <errno.h>

void GImageEditor::imageProcess(){
    
    cout<<"START image";
    
	string strHeaderHTML,srcLine,str;
	string  path=inputData.data["tablePath"]+"/header.xml";
	replace(path.begin(),path.end(),'\\','/');
    
	if(!inputData.fileList.size()){
		readDirectoryToArray(inputData.fileList,inputData.data["inputFolder"],"img");
	}
    
    //cout<<"inputData.fileList.size()="<<inputData.fileList.size();
    
#ifdef FORK11
    int maxFork=inputData.num_cores*2;  //обработка изображений легче чем OCR
    pidID *pidIDArray;
    
    
    MemoryFile *pidData_mf;  //main file for conection with child process
    pidData_mf=MemoryFile::create(inputData.data["statPath"].c_str(), MemoryFile::if_exists_keep_if_dont_exists_create);
    pidData_mf->resize(sizeof(pidID)*maxFork);
    pidIDArray=(pidID*)pidData_mf->data(); //array which can be share between processes.
    
    for(int index=0;index<maxFork;index++)pidIDArray[index].status=0;
    //for(int index=0;index<maxFork;index++)cout_<<pidIDArray[index].status;
    
    
    int ID=0;
    unsigned int i=0;
    DIR *dir;
    int countFork=0;
    int status;
    while(i<inputData.fileList.size()){
        
        if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
            cout_<<"NEW DIR#1 "<<inputData.fileList[i]<<endl;
            readDirectoryToArray(inputData.fileList, inputData.fileList[i],"img");
            i++; continue;
        }
        //cout_<<"NEW START"<<endl;
        
        inputData.data["inputFile"]=inputData.fileList[i];
        int statusFork=forkImageProcess(pidIDArray,maxFork,ID);
        if(statusFork==0){
            wait(&status);sleep(1);  //пробуем подождать
            cout<<"fork error. Try to continue;"<<endl;
            continue;
        }
        if(statusFork==3)i++;  //пропускаем битую картинку
        countFork++;
        i++;
        if(i==inputData.fileList.size())break;
        
        cout_<<" pidIDArray[0].status= "<<pidIDArray[0].status<<" >> ";
        for(int index=0;index<maxFork;index++)cout_<<pidIDArray[index].status<<" ";
        cout_<<endl;
        
        ID=100;
        while(ID==100){
            if(countFork<maxFork-1)break;             //есть свободные слоты для новых процессов
            int forkStatusCount=0;
            for(int index=0;index<maxFork;index++){   //подсчитываем количество активных процессов
                if(pidIDArray[index].status==1)forkStatusCount++;
            }
            if(forkStatusCount==countFork){sleep(1);continue;}     //нет завершенных процессов
            wait(&status);                           //регистрируем с системе завершенный процесс
            countFork--;
            for(int index=0;index<maxFork;index++){  //маркируем слот как свободный
                //cout_<<"pidIDArray["<<index<<"].status="<<pidIDArray[index].status<<endl;
                if(pidIDArray[index].status==0){
                    
                    ID=index;
                    pidIDArray[index].status=1;
                    break;
                }
            }
            
        }
    }
    
	
#else
    DIR *dir;
    int printIndex=0;
    string path0="";
    string path1="";

    string mode="tif";
    GBitmap *printPage0;
    GBitmap *printPage1;

	for(int i=0;i<inputData.fileList.size();i++){
        cout<<"inputData.fileList["<<i<<"]="<<inputData.fileList[i]<<END;
        if( ( dir=opendir(inputData.fileList[i].c_str()))!=NULL){
            cout_<<"NEW DIR#1 "<<inputData.fileList[i]<<endl;
            readDirectoryToArray(inputData.fileList, inputData.fileList[i],"img");
            continue;
        }
        cout<<"NEW START"<<endl;
        inputData.data["inputFile"]=inputData.fileList[i];
        pechaImg=LoadImageData(inputData.data["inputFile"],0);
        proccessImage(pechaImg);
        if(inputData.data["fileSave"]=="print3_Pages"){
            //if(pechaImg->rows()<1200||pechaImg->rows()>1300)pechaImg->scaleFast((float)1216/pechaImg->rows());
            cout<<"printIndex="<<printIndex<<" pechaImg->rows()="<<pechaImg->rows()<<endl;

            
            if(printIndex==0){
                path=str_replace(".jpg", ".tif", inputData.data["inputFile"]);
                path=str_replace(".JPG", ".tif", path);
                path=str_replace(".jpg", ".tif", path);
                if(path.find(".tif")==-1)path=path+".tif";
                path0=str_replace(".tif", "_print0.tif", path);
                path1=str_replace(".tif", "_print1.tif", path);
                printPage0=GBitmap::create(pechaImg->columns(),pechaImg->rows()*3);
                printPage1=GBitmap::create(pechaImg->columns(),pechaImg->rows()*3);

                printPage0->drawImg(pechaImg,0, 0);
            }
            if(printIndex==1){
                cout<<" pechaImg->rows()*2="<<pechaImg->rows()*2<<endl;
                printPage1->drawImg(pechaImg,0, pechaImg->rows()*2);
            }
            
            if(printIndex==2){
                printPage0->drawImg(pechaImg,0, pechaImg->rows());
            }
            if(printIndex==3){
                printPage1->drawImg(pechaImg,0, pechaImg->rows());
            }
            if(printIndex==4){
                printPage0->drawImg(pechaImg,0, pechaImg->rows()*2);
            }
            if(printIndex==5){
                printPage1->drawImg(pechaImg,0, 0);
            }
            if(printIndex==5||i==inputData.fileList.size()-1){            //cout_<<" path="<<path<<endl;
                WriteImageData(printPage0,path0,0,mode);
                WriteImageData(printPage1,path1,0,mode);
                printPage0->destroy();
                printPage1->destroy();
                printIndex=-1;
            }
            printIndex++;
        }
        if(inputData.data["fileSave"]=="print3Pages"){
            //if(pechaImg->rows()<1200||pechaImg->rows()>1300)pechaImg->scaleFast((float)1216/pechaImg->rows());
            cout<<"printIndex="<<printIndex<<" pechaImg->rows()="<<pechaImg->rows()<<endl;
            int dY=21;
            int dX=21;
            
            if(printIndex==0){
                path=str_replace(".jpg", ".tif", inputData.data["inputFile"]);
                path=str_replace(".JPG", ".tif", path);
                path=str_replace(".jpg", ".tif", path);
                if(path.find(".tif")==-1)path=path+".tif";
                path0=str_replace(".tif", "_print0.tif", path);
                path1=str_replace(".tif", "_print1.tif", path);
                printPage0=GBitmap::create(pechaImg->columns(),pechaImg->rows()*2);
                printPage1=GBitmap::create(pechaImg->columns(),pechaImg->rows()*2);
                
                printPage0->drawImg(pechaImg,-dX, -dY);
            }
            if(printIndex==1){
                cout<<" pechaImg->rows()="<<pechaImg->rows()<<endl;
                printPage1->drawImg(pechaImg,-dX, pechaImg->rows()+dY);
            }
            
            if(printIndex==2){
                printPage0->drawImg(pechaImg,-dX, pechaImg->rows()+dY);
            }
            if(printIndex==3){
                printPage1->drawImg(pechaImg,-dX, -dY);
            }
           
            if(printIndex==3||i==inputData.fileList.size()-1){            //cout_<<" path="<<path<<endl;
                WriteImageData(printPage0,path0,0,mode);
                WriteImageData(printPage1,path1,0,mode);
                printPage0->destroy();
                printPage1->destroy();
                printIndex=-1;
            }
            printIndex++;
        }
        pechaImg->destroy();
    }
	
#endif
    //inputData.c_out.close();
    
    
	
}//______________________________________________
/*
int GImageEditor::forkImageProcess(pidID *pidIDArray,int maxFork,int ID){
    
    pechaImg=LoadImageData(inputData.data["inputFile"],0);
    if(pechaImg!=NULL){
        proccessImage(pechaImg);
        pechaImg->destroy();
    }else{
        cout<<"@@@!!!ERROR IN IMAGE!!!@@@"<<inputData.data["inputFile"]<<endl;
    }
   
     int pid = fork();
     if (pid < 0){
     cout<<"ERROR on fork ";
     pechaImg->destroy();
     return 0;
     }
     if (pid == 0)  {
     cout_<<"//___@inputID="<<ID<<endl;
     pechaImg=LoadImageData(inputData.data["inputFile"],0);
     if(pechaImg!=NULL){
     proccessImage(pechaImg);
     pechaImg->destroy();
     }else{
     cout<<"@@@!!!ERROR IN IMAGE!!!@@@"<<inputData.data["inputFile"]<<endl;
     }
     pidIDArray[ID].status=0;
     exit(0);
     }
     
    return 1;
    
}//______________________________________________
*/
int GImageEditor::forkImageProcess(pidID *pidIDArray,int maxFork,int ID){
    pechaImg=LoadImageData(inputData.data["inputFile"],0);
    int pid = fork();
    if (pid < 0){
        cout<<"ERROR on fork ";
        pechaImg->destroy();
        return 0;
    }    
    if (pid == 0)  {
        cout_<<"//___@inputID="<<ID<<endl;        
        if(pechaImg!=NULL){
            proccessImage(pechaImg);
        }else{
            cout<<"@@@!!!ERROR IN IMAGE!!!@@@"<<inputData.data["inputFile"]<<endl;
        }
        pidIDArray[ID].status=0;
        exit(0);
    }
    pechaImg->destroy();
    return 1;
    
}//______________________________________________


void GImageEditor::proccessImage(GBitmap *pechaImg_){
    
    if(inputData.data["action"]=="splitPages")inputData.data["SplitPage"]="splitPage";
	
	vector<stringOCR>strArray;
    if(inputData.data["SplitPage"]=="splitPage"){
		cout<<"SplitPage"<<endl;
		strArray.resize(0);
        int x=pechaImg_->columns()/6;
        int y=0;
        int w=(pechaImg_->columns()*4)/6;
        int h=pechaImg_->rows();
		GBitmap *pechaImgResize=GBitmap::createRegion(pechaImg_, x, y, w, h);
        GBitmap *pechaImgRotate=GBitmap::createRotate(pechaImgResize,-90);
        pechaImgResize->destroy();
        float angle=pechaImgRotate->detectPageAngle(8);
        pechaImgRotate->destroy();
        pechaImgRotate=GBitmap::createRotate(pechaImg,-angle);
        
		setBit=GBitset::createResize(pechaImgRotate,1,1,1);
		setBit->SplitPagesBtn=1;
		setBit->pageStringDetector(strArray,2);
        cout<<"TurnXL="<<setBit->TurnX<<" border="<<setBit->border_size()<<" w="<<setBit->columns()<<endl;
		//WriteImageData(setBit->drawData,"/_Image2OCR/1.jpg", 1); //exit(0);
		string st=inputData.data["inputFile"];
		string name=st.erase(0,st.rfind('/'));
		string ext=name;
		ext=ext.erase(0,ext.rfind('.'));
		name=name.erase(name.size()-ext.size(),ext.size());
		string folderName=inputData.data["inputFile"];
        folderName=folderName.erase(folderName.rfind('/'),folderName.size()); //cout_<<" st@@@"<<folderName<<END;
		st=folderName; st+="/"; st+=name; st+="_0"; st+=ext;
		WriteSpreadData(pechaImgRotate,setBit,-1, st); // , strArray
		st=folderName;
		st+="/"; st+=name; st+="_1"; st+=ext;  // cout_<<" st"<<st<<END;
		WriteSpreadData(pechaImgRotate,setBit,-2, st); // , strArray
		pechaImgRotate->destroy();
		setBit->destroy();
        return;
		
	}
	if(inputData.data["SplitPage"]=="splitColumns"){
        /*Для определения середины страницы вырезаем ценральную область страницы 2/3 ширины)
         затем поворачиваем на 90 градучов и исправляем наклон, определяя его по центральной области страницы
         всю страницу поворачиваем на нужный угол. В получившейся ровной странице ищем разделение на колонки.
         Лучшие результаты при наличии разделительной линии между колонками.
         */
		cout<<"splitColumns"<<END;
		strArray.resize(0);
        int x=pechaImg_->columns()/6;
        int y=0;
        int w=(pechaImg_->columns()*4)/6;
        int h=pechaImg_->rows();
		GBitmap *pechaImgResize=GBitmap::createRegion(pechaImg_, x, y, w, h);
        GBitmap *pechaImgRotate=GBitmap::createRotate(pechaImgResize,-90);
        pechaImgResize->destroy();
        float angle=pechaImgRotate->detectPageAngle(8);
        pechaImgRotate->destroy();
        pechaImgRotate=GBitmap::createRotate(pechaImg,-angle);
        //WriteImageData(pechaImgRotate,"/test.jpg", 1);
        
        setBit=GBitset::createResize(pechaImgRotate,1,1,1);
		setBit->SplitPagesBtn=1;
		setBit->pageStringDetector(strArray,1);
#ifdef DEBUGLVL_GREY
        WriteImageData(setBit->drawData,"/1.jpg", 0);
#endif
#ifdef DEBUGLVL_DRAW
        WriteImageData(setBit->drawDataRGB,"/1.jpg", 0);
#endif
        
        //  ДЛЯ ПОКАЗА КАРТИНКИ
		//if(CheckBoxTIFF->Checked==true){ext=".tif";}
		//if(AutoRemoveBorderBtn->Checked==true){
		string st=inputData.data["inputFile"];
		string name=st.erase(0,st.rfind('/'));
		string ext=name;
		ext=ext.erase(0,ext.rfind('.'));
		name=name.erase(name.size()-ext.size(),ext.size());
        st=inputData.data["inputFile"];
        st.erase(st.rfind('/'),st.size());
		st+="/";
        inputData.data["outputFolder"]=st;
        st+=name; st+="_0"; st+=ext;   cout<<" st"<<st<<END;
		WriteSpreadData(pechaImgRotate,setBit,-1, st); // , strArray
        st=inputData.data["outputFolder"];
        st+=name; st+="_1"; st+=ext;   cout<<" st"<<st<<END;
		WriteSpreadData(pechaImgRotate,setBit,-2, st); // , strArray
		setBit->destroy();
        pechaImgRotate->destroy();
        cout<<"done split";
        exit(0);
		
	}
    

    if(inputData.data["SplitPage"]=="eraseBorder"){
        int pageBorderX=atoi(inputData.data["BorderX"].c_str());
		int pageBorderY=atoi(inputData.data["BorderY"].c_str());

		cout<<"pageBorderX="<<pageBorderX<<"pageBorderY="<<pageBorderY<<END;
		pechaImg_->eraseBorder(pageBorderX,pageBorderY); // , strArray
	}

	//cout_<<"inputData.data[\"rotationBox\"]"<<inputData.data["rotationBox"]<<END;
    
    float scale=atoi(inputData.data["scale"].c_str());
    scale/=100;
    
	
	if(inputData.data["action"]=="baseNormalisation"){
        adaptiveFiltration1(pechaImg_,scale,1);
	}
    
    if(inputData.data["action"]=="adaptiveFiltration"){
        adaptiveFiltration1(pechaImg_,scale,2);
	}

    if(inputData.data["action"]=="alignContent"||inputData.data["action"]=="crop"){
        /*Для выравнивания страницы:
         0. Стираем мусор по краю страницы
         1. Уменьшаем страницу в 3 раза для оптимизации выделения строк.
         2. Определяем верхнюю и нижнюю строки
         3. Поворачиваем страницу на 90 градусов
         4. Определяем левую и правую границу текста 
         5. Рисуем центральную область по центру пустой страницы*/
		cout<<"alignContent"<<END;
		strArray.resize(0);
        pechaImg_->eraseBorder(25,25); // , strArray
        
        
		GBitmap *pechaImgResize=GBitmap::createScale(pechaImg_,0.5);
        pechaImgResize->gaussian(5);
        GBitmap *pechaImgRotate=GBitmap::createRotate(pechaImgResize,-90);
        
        setBit=GBitset::createResize(pechaImgResize,1,1,1);
        setBit->pageStringDetector(strArray,1);// Подпрограмма управления последовательностью выполнения всех подпрограмм (их 7 штук).
        //WriteImageData(pechaImgResize,"/_Image2OCR/1.jpg", 0); exit(0);
        
        if(!strArray.size())return;
        int pageY0=strArray[strArray.size()-1].y0-140;   pageY0*=2;  if(pageY0<0)pageY0=0;
        int pageY1=strArray[0].y1+140;                   pageY1*=2;  if(pageY1>pechaImg_->rows())pageY1=pechaImg_->rows();
        
#ifdef DEBUGLVL_GREY
        WriteImageData(setBit->drawData,"/_Image2OCR/1.jpg", 0);
#endif
#ifdef DEBUGLVL_DRAW
        WriteImageData(setBit->drawDataRGB,"/_Image2OCR/1.jpg", 0);
#endif
                
        
        int pageH=pageY1-pageY0;
        strArray.resize(0);
        setBit->destroy();
        setBit=GBitset::createResize(pechaImgRotate,1,1,0);
        setBit->pageStringDetector(strArray,1);  // это  надо мерить длинну строки при 90 гр повороте и короткие не учитывать  у нас гдето считактся длинна строки
        // что имеется в виду? количество квадратиков? можно и так несколько квадратиков в строке и она не учитывается.
        // но как тогда будет рамка распознаватся? в ней вообще квадратиков нет В ообше надо не спешить а подумать алгоритмы любят когда их думают
        // да и прыгает она не многое.
        //Можно выравнять по правому краю, там нет букв,  только номер     видел справа целое слово
        // для данной книги можно  по правому краю и подумать дальше. ок.
        //
        if(!strArray.size())return;
        int pageX0=strArray[strArray.size()-1].y0-50;   pageX0*=2; if(pageX0<0)pageX0=0;
        int pageX1=strArray[0].y1+50;                   pageX1*=2; if(pageX1>pechaImg_->columns())pageX0=pechaImg_->columns();
        int pageW=pechaImg_->columns();
        GBitmap *pageContent=GBitmap::createRegion(pechaImg_, 0, pageY0, pageW, pageH);
        pechaImg_->fill(255);
        if(inputData.data["action"]=="alignContent"){
            pechaImg_->drawImg(pageContent, 0, (pechaImg_->rows()-pageH)/2);
            pageContent->destroy();
        }
        if(inputData.data["action"]=="crop"){
            pechaImg_=pageContent;
        }
        
		setBit->destroy();
        pechaImgRotate->destroy();
        pechaImgResize->destroy();
	}

    
    if(inputData.data["fileSave"]=="saveTif"){    
        string mode="tif";
        string path=str_replace(".jpg", ".tif", inputData.data["inputFile"]);
        path=str_replace(".JPG", ".tif", path);
        path=str_replace(".jpg", ".tif", path);
        if(path.find(".tif")==-1)path=path+".tif";
        //cout_<<" path="<<path<<endl;
        WriteImageData(pechaImg_,path,0,mode);
    }
    if(inputData.data["fileSave"]=="saveOriginal"){
        WriteImageData(pechaImg_,inputData.data["inputFile"],0);
    }

}

void GImageEditor::imageNormalisation(GLetter* letter){
    //cout<<inputData.data["action"]; 
    //string path=inputData.data["root"]+"/edit/letterImg.jpg";
    if(inputData.data["action"]=="imageNormalisation"){
        if(!letter->maskOriginal.status||letter->maskOriginal.mW!=128){
           if(letter->maskOriginal.mW!=128)cout<<"new original"<<endl;
           letter->maskOriginal=letter->mask128;   //save original mask
           letter->maskOriginal.status=1;
           //cout<<" save original";
        }else{
           letter->mask128=letter->maskOriginal;   //restore original mask
        }    
        GBitmap *img=letter->mask128.unPackMask(); 
        int w=img->columns();
        int h=img->rows();
        GBitmap *outBitmap=GBitmap::create(PICT_SIZE,PICT_SIZE);
        int x0=letter->mask128.xMask+PICT_SIZE/2;
        int y0=letter->mask128.yMask+PICT_SIZE/2;
        outBitmap->drawImg(img, x0,y0); 
        adaptiveFiltration1(outBitmap,1,2);
        //string mode="jpg";
        //WriteImageData(outBitmap, path, 0,mode);
        img->destroy();
        img=GBitmap::createRegion(outBitmap,x0, y0, w, h);
        //WriteImageData(img, "/_1.jpg", 0);
        letter->mask128.packBitMask128(img);
        img->destroy();
        outBitmap->destroy();
        letter->reloadMask();
    }   
    if(inputData.data["action"]=="revertImage"){
        cout<<"letter->maskOriginal.status="<<(short)letter->maskOriginal.status;
        if(letter->maskOriginal.status){
           letter->mask128=letter->maskOriginal;   //restore original mask
        }
        letter->reloadMask();
    }
    if(inputData.data["action"]=="setFocalPoints"){

        vector<OCRPoint>str;
        GBitmap *img=letter->mask128.unPackMask();
        GGraph outGraph=GGraph(PICT_SIZE,PICT_SIZE);
        int x0=letter->mask128.xMask+PICT_SIZE/2;
        int y0=letter->mask128.yMask+PICT_SIZE/2;
        outGraph.drawImg(img, x0,y0);
     
        
        outGraph.StablePoint(str);
        
        //WriteImageData(outBitmap, "/_Image2OCR/_2.jpg", 0); //exit(0);
        
        //cout<<"new size="<<str->size()<<" x0="<<x0<<" y0="<<y0<<endl;
        letter->focalPoint->resize(0);
        for(int i=0;i<str.size();i++){
            str[i].x-=PICT_SIZE/2;
            str[i].y-=PICT_SIZE/2;
            letter->focalPoint->push_back(str[i]);
            cout<<"x="<<letter->fPoint[i].x<<" y="<<letter->fPoint[i].y<<" p="<<(int)letter->fPoint[i].type<<endl;
        }
        //exit(0);
        
    }
}

