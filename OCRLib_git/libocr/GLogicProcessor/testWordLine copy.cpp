#include "GLogicProcessor.h"
#include "GImageEditor.h"

/** @brief на этом этапе в результатах словаря около 70 результатов претендующих на один и тотже слог.
 //производим разбор результатов словаря по отношению к исходному тексту
 //сравниваем насколько точно каждая словарная гипотеза описывает данные текста и сравниваем гипотезы между собой.*/

void GLogicProcessor::testWordLine(GBitsetMatrix *setMatrix,
                                   vector<OCRMatch>&line,
                                   vector<OCRMatch>&dLine,
                                   vector<OCRMatch>&originalMatch){
    
    
    compressMatch(line);
    
    //устанавливаем габариты OCRMatch
    for(int m=0;m<line.size();m++){
        if(!line[m].correlation)continue;
        OCRMatch &a=line[m];
        a.s.x0=a.x0;
        a.s.x1=a.x1;
        a.s.y0=a.y0;
        a.s.y1=a.y1;
        //в массиве line для каждой гипотезы OCRMatch записаны адреса пар букв в dLine.
        //в каждой паре букв в массиве dLine в массиве line записаны адреса букв в originalMatch
        //в originalMatch в каждой составной букве в массиве letter хранятся буквы из которых составлен стек.
        //переписываем эти буквы в массив letters для каждой гипотезы line[m]
        a.letter.resize(0);
        //cout<<"a.name:"<<a.name<<endl;
        for(int i=0;i<a.line.size();i++){
            OCRMatch &g=dLine[abs(a.line[i])];
            //cout<<" g.name:"<<g.name<<endl;
            //для каждой буквы в паре переписываем составляющие ее буквы
            //если буква составная переписываем ее составляющие.
            //для одиночных букв переписываем саму букву.
            if(originalMatch[g.line[0]].letter.size()){
                for(int j=0;j<originalMatch[g.line[0]].letter.size();j++){
                    OCRMatch &c=originalMatch[g.line[0]].letter[j];
                    a.letter.push_back(c);
                }
            }else{
                OCRMatch &c=originalMatch[g.line[0]];
                a.letter.push_back(c);
            }
            if(originalMatch[g.line[1]].letter.size()){
                for(int j=0;j<originalMatch[g.line[1]].letter.size();j++){
                    OCRMatch &c=originalMatch[g.line[1]].letter[j];
                    a.letter.push_back(c);
                }
            }else{
                OCRMatch &c=originalMatch[g.line[1]];
                a.letter.push_back(c);
            }
        
        }
    }
    
    //для каждой гипотезы необходимо знать сколько фокальных точек текста она покрывает.
    //также неободимо выяснить сколько фокальных точек текста находящихся в габаритах гипотезы не учитывается данной гипотезой
    //для поиска строим графический массив в котором размещены изображения фокальных точек текста.
    //Каждая точка раскевается а область 8х8 пикселов и заполняется ID номером точки. Для отработки перекрытия точек
    //каждая точка графического массива это тип long и в случае перекрытия ID номер точки в long пишется с отступом, (аналогично записи RGB)
    //все точки текста совпавшие с гипотезой записываются в гипотезе в массив pointMatch, не совпавшие точки записываются в массив pointNoMathch.
    
#define cP(x)   p= data+(x)*4;    if(*p>0)p++; if(*p>0)p++; if(*p>0)p++; *p=i+10;
    
    uint w=setMatrix->columns();
    uint h=setMatrix->rows();
    short *data=(short*)calloc(w*h, 8);
    short *p;
    OCRPoint *points=(OCRPoint*)&setMatrix->focalPoint[0];
    for(int i=0;i<setMatrix->focalPoint.size();i++){
        uint x=(uint)points[i].x;
        uint y=(uint)points[i].y;
        
        cP(y*w+x); cP(y*w+x+1);  cP(y*w+x+2);  cP(y*w+x+3);  cP(y*w+x+4);  cP(y*w+x+5);
        cP(y*w+x-1);  cP(y*w+x-2);  cP(y*w+x-3);  cP(y*w+x-4);  cP(y*w+x-5);
        
        cP((y+1)*w+x);    cP((y+1)*w+x+1);  cP((y+1)*w+x+2);  cP((y+1)*w+x+3);  cP((y+1)*w+x+4);  cP((y+1)*w+x+5);
        cP((y+1)*w+x-1);  cP((y+1)*w+x-2);  cP((y+1)*w+x-3);  cP((y+1)*w+x-4);  cP((y+1)*w+x-5);
        cP((y-1)*w+x);    cP((y-1)*w+x+1);  cP((y-1)*w+x+2);  cP((y-1)*w+x+3);  cP((y-1)*w+x+4);  cP((y-1)*w+x+5);
        cP((y-1)*w+x-1);  cP((y-1)*w+x-2);  cP((y-1)*w+x-3);  cP((y-1)*w+x-4);  cP((y-1)*w+x-5);
        
        cP((y+2)*w+x);    cP((y+2)*w+x+1);  cP((y+2)*w+x+2);  cP((y+2)*w+x+3);  cP((y+2)*w+x+4);
        cP((y+2)*w+x-1);  cP((y+2)*w+x-2);  cP((y+2)*w+x-3);  cP((y+2)*w+x-4);
        cP((y-2)*w+x);    cP((y-2)*w+x+1);  cP((y-2)*w+x+2);  cP((y-2)*w+x+3);  cP((y-2)*w+x+4);
        cP((y-2)*w+x-1);  cP((y-2)*w+x-2);  cP((y-2)*w+x-3);  cP((y-2)*w+x-4);
        
        cP((y+3)*w+x);    cP((y+3)*w+x+1);  cP((y+3)*w+x+2);  cP((y+3)*w+x+3);  cP((y+3)*w+x+4);
        cP((y+3)*w+x-1);  cP((y+3)*w+x-2);  cP((y+3)*w+x-3);  cP((y+3)*w+x-4);
        cP((y-3)*w+x);    cP((y-3)*w+x+1);  cP((y-3)*w+x+2);  cP((y-3)*w+x+3);  cP((y-3)*w+x+4);
        cP((y-3)*w+x-1);  cP((y-3)*w+x-2);  cP((y-3)*w+x-3);  cP((y-3)*w+x-4);
        
        cP((y+4)*w+x);    cP((y+4)*w+x+1);  cP((y+4)*w+x+2);  cP((y+4)*w+x+3);
        cP((y+4)*w+x-1);  cP((y+4)*w+x-2);  cP((y+4)*w+x-3);
        cP((y-4)*w+x);    cP((y-4)*w+x+1);  cP((y-4)*w+x+2);  cP((y-4)*w+x+3);
        cP((y-4)*w+x-1);  cP((y-4)*w+x-2);  cP((y-4)*w+x-3);

        cP((y+5)*w+x);    cP((y+5)*w+x+1);
        cP((y+5)*w+x-1);
        
        cP((y-5)*w+x);    cP((y-5)*w+x+1);
        cP((y-5)*w+x-1);
    }
  
    /*
    GBitmap *img=GBitmap::create(w,h);
    uchar *p_=img->bytes_data;
    for(int i=0;i<w*h;i++){
        if(data[i*4]){
            //cout<<data[i*4]<<" ";
            *p_=0;
        }
        p_++;
    }
    ((GImageEditor*)inputData.imageEditor)->WriteImageData(img,"/_Image2OCR/1Draw.jpg",0);
   */

    for(int m=0;m<line.size();m++){
        OCRMatch &a=line[m];
        for(int n=0;n<a.letter.size();n++){
           GLetter *glyph=aliKali->getLetter(a.letter[n].letterIndex);
           OCRPoint *p=(OCRPoint*)glyph->fPoint;
            for(int i=0;i<glyph->fPointCount();i++){
                OCRMatch &t=a.letter[n];
                uint x=p[i].x+t.xCenter+MATRIX_BORDER;
                uint y=p[i].y+t.yCenter+MATRIX_BORDER;
                int id=data[(y*w+x)*4];
                if(id){
                    a.pMatch.push_back(points[id-10]);
                }
            }
        }
    }
    
    
    
    //line - ответы словаря
    //dLine - пары букв
    //originalMatch - стеки и элменты стеков

    for(int m=0;m<line.size();m++){
        if(!line[m].correlation)continue;
        for(int n=0;n<line.size();n++){
            if(!line[m].correlation)continue;
            if(m==n)continue;
            //прежде всего определяем площадь пересечения
            OCRBox s;
            intArea(line[m].s,line[n].s,s);
            //s.printBox();
            if(s.area>0){
                //два OCRMatch имеют общую площадь пересечения
                //необходимо решить какая гипотеза более верная.
                //Сначала проверяем насколько полно каждая гипотеза описывает площадь пересечения
                
                OCRMatch &a=line[m];
                OCRMatch &b=line[n];
                cout<<a.name<<" "<<b.name;
                //для a,b проверяем все составляющие их буквы
                //for(int i=0;i<a.letter.size();i++){
                //    cout<<i<<" n="<<a.letter[i].name<<" xCenter:"<<a.letter[i].xCenter<<" yCenter:"<<a.letter[i].yCenter<<endl;
                //}
                
                
                
            }
            
        }
    }

}
