cout <<"www"<<endl;
string str;
//readText(str,"/sdcard/web/root/1.php");
//cout<<str;

GBitmap *img=GBitmap::create("/sdcard/web/root/1.png");

img->printToScreen();
/*

  img->binarisation64();
    
    
    GGraph graph;
    graph.init(img);
    
    graph.graphFocalLine(15);
    
    //img->bytes_data=graph.bytes_data;
    //imageEditor->WriteImageData(img,"/_Image2OCR/_2Draw_focal_lines1.jpg",0); return 0;
    
    
    graph.focalPointXTL();
    //graph.printToScreenBinary();
    cout<<" focalPoint.size():"<<graph.focalPoint.size()<<endl;
    graph.focalLineRecognition();
    cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;
    graph.findExtremPoints();
    cout<<" focalLine.size():"<<graph.focalLine.size()<<endl;
    
    graph.buildOCRStars();
    graph.buildDipole();
    //graph.printStars("/_Image2OCR/_2Draw_text.html",1);
    //graph.printStars("/_Image2OCR/_2Draw_text.html",2);
    //graph.printToScreenBinary();
cout<<"done";
*/