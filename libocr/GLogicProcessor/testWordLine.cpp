#include "GLogicProcessor.h"
#include "../GImageEditor/GImageEditor.h"

uint setPointCount(OCRMatch &a,OCRBox &s){
    uint p=0;
    //в каждой гипотезе записан массив интегральных сумм этой гипотезы в котором начало координат совпадает с x0 гипотезы.
    //необходимо высислить смещение по х соответствуещее области пересечения
    
    int dX0,dX1;
    dX0=s.x0-a.s.x0;
    if(dX0<0)dX0=0;
    dX1=s.x1-a.s.x0;
    if(dX1>=a.s.x1)dX1=a.s.x1;
    //рассчитываем сумму точек фокальных линий текста перекрытых гипотезой.
    p=a.pSum[dX1]-a.pSum[dX0];
    
    return p;
}

float setCorrelation(vector<OCRMatch>&originalMatch,OCRMatch &a,OCRBox &s){
    float p=0;
    uint count=0;
    //необходимо вычислить суммарную корреляцию всех букв в области пересечения
    for(int i=0;i<a.line.size();i++){
        OCRMatch &c=originalMatch[a.line[i]];
        //cout<<b.line[i]<<" "<<c.name<<" c:"<<c.correlation<<" p:"<<c.pCount<<endl;
        int x=c.xCenter+MATRIX_BORDER;
        if(x>s.x0&&x<s.x1){
            p+=c.correlation;
            count++;
        }
    }
    if(count==0)return -1;
    p/=count;
    
    return p;
}

float setCorrelationMask(vector<OCRMatch>&originalMatch,OCRMatch &a,OCRBox &s){
    uint count=0;
    float correaltion=0;
    //необходимо вычислить суммарную корреляцию всех букв в области пересечения
    
    for(int i=0;i<a.line.size();i++){
        OCRMatch &c=originalMatch[a.line[i]];
        for(int n=0;n<a.mask32Vector.size();n++){
            int xC=c.mask32Vector[n].xMax+c.mask32Vector[n].imgW/2;
            int yC=c.mask32Vector[n].yMax+c.mask32Vector[n].mH/2;
            if(xC>s.x0&&xC<s.x1&&yC>s.y0&&yC<s.y1){
                correaltion+=c.mask32Vector[n].correlation;
                count++;
            }
        
        }
       }
    if(count==0)return 1;
    correaltion/=count;
    
    return correaltion;
}


void setImagePointCount(vector<OCRMatch>&originalMatch,GBitmap *lineImg32, OCRMatch &a, int print){
    GBitmap *letterImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);
    
    for(int n=0;n<a.line.size();n++){
        OCRMatch &c=originalMatch[a.line[n]];
        c.drawPict32(letterImg,0,0,XOR_MODE);
    }
    
    letterImg->unpackImg(0);
    //if(print){
    //letterImg->savePNG("/_Image2OCR/_1Draw32_11.jpg"); //exit(0);
    //}
    
    //вычисляем массив интегральных сумм
    lineImg32->imgIntegralSum(letterImg,a.pSum, &a.s);
    if(!a.pSum.size()){
        a.pCount=0;
        a.pSum.push_back(a.pCount);
        cout<<"@@@@ error in match dimention"<<endl;
    }
    a.pCount=a.pSum[a.pSum.size()-1];
    //cout<<"m:"<<m<<" a.pCount:"<<a.pCount<<endl;
    a.pSum.push_back(a.pCount);
    letterImg->destroy();
}


void setMatchLetters(vector<OCRMatch>&line,vector<OCRMatch>&originalMatch, GBitmap *lineImg32, OCRMatch &a, OCRMatch &b, OCRBox &s){
    uint size=(uint)b.line.size();
    
    if(b.s.x0<s.x0&&b.s.x1>s.x1){
        OCRMatch newMatch;
        int i=(int)b.line.size()-1;
        while(i>=0){
            OCRMatch &c=originalMatch[b.line[i]];
            //cout<<b.line[i]<<" "<<c.name<<" c:"<<c.correlation<<" p:"<<c.pCount<<endl;
            int x=c.xCenter+MATRIX_BORDER;
            if(x>s.x1){
                if(!newMatch.line.size()){
                    newMatch.line.push_back(b.line[i]);
                    newMatch.dIndex.push_back(b.dIndex[i]);
                }else{
                    newMatch.line.insert(newMatch.line.begin(),b.line[i]);
                    newMatch.dIndex.insert(newMatch.dIndex.begin(),b.dIndex[i]);
                }
                b.line.erase(b.line.begin()+i);
                b.dIndex.erase(b.dIndex.begin()+i);
            }else{
                break;
            }
            i--;
        }
        if(newMatch.line.size()){
            newMatch.name="";
            newMatch.correlation=0;
            for(int i=0;i<newMatch.line.size();i++){
                OCRMatch &c=originalMatch[newMatch.line[i]];
                newMatch.correlation+=c.correlation;
                newMatch.name+=c.name;
                
            }
            newMatch.length=(uint)newMatch.line.size();
            newMatch.correlation/=newMatch.length;
            int ind=newMatch.line[0];
            newMatch.x0=originalMatch[ind].x0;
            ind=newMatch.line[newMatch.line.size()-1];
            newMatch.x1=originalMatch[ind].x1;
            newMatch.y0=b.y0;
            newMatch.y1=b.y1;
            newMatch.setSize(1);
            newMatch.setSize(0);

            setImagePointCount(originalMatch,lineImg32,newMatch,0);
            line.push_back(newMatch);
            
        }
    
    
    }
    
    for(int i=0;i<b.line.size();i++){
        OCRMatch &c=originalMatch[b.line[i]];
        //cout<<b.line[i]<<" "<<c.name<<" c:"<<c.correlation<<" p:"<<c.pCount<<endl;
        int x=c.xCenter+MATRIX_BORDER;
        if(x>s.x0&&x<s.x1){
            b.line.erase(b.line.begin()+i);
            b.dIndex.erase(b.dIndex.begin()+i);
            i--;
        }
    }
    if(size==b.line.size())return;
    //устанавливаем новые габариты гипотезы b
    //при необходимости разделяем гипотезу b на две гипотезы
    if(!b.line.size()){
        b.correlation=0;
        return;
    }
    b.name="";
    b.correlation=0;
    for(int i=0;i<b.line.size();i++){
        OCRMatch &c=originalMatch[b.line[i]];
        b.correlation+=c.correlation;
        b.name+=c.name;
        
    }
    b.length=(uint)b.line.size();
    b.correlation/=b.length;
    b.x0=originalMatch[b.line[0]].x0;
    b.x1=originalMatch[b.line[b.line.size()-1]].x1;
    b.setSize(1);
    b.setSize(0);
    
    setImagePointCount(originalMatch,lineImg32,b,0);
}


uint combineMatchLetters(vector<OCRMatch>&originalMatch,GBitmap *lineImg32, OCRMatch &a, OCRMatch &b, OCRBox &s){
    
    int index=0;
    int flag=1;
    int flag_=0;
    int flagStart=0;
    int i=0;
    while(i<a.line.size()&&index<b.line.size()){
        OCRMatch &c=originalMatch[a.line[i]];
        int x=c.xCenter+MATRIX_BORDER;
        if(x>s.x0&&x<s.x1&&index<b.line.size()&&!flagStart){
            flagStart=1;
        }
        if(flagStart){
            OCRMatch &c_=originalMatch[b.line[index]];
            int x_=c.xCenter+MATRIX_BORDER;
            /*
             //если не совпадает последняя буква a.line, и в b.line еще есть буквы стираем последнюю букву a.line.
            if(i==a.line.size()-1&&index<b.line.size()-1&&c_.name!=c.name){
                 a.line.pop_back();
                 if(!a.line.size()){
                     a.correlation=0;
                     return 0;
                 }
                 a.dIndex.pop_back();
                 a.x1=originalMatch[a.line[a.line.size()-1]].x1;
                 a.length--;
                 a.setSize(1);
                 a.setSize(0);
                 s.x1=a.x1+MATRIX_BORDER;
                 break;
            }
            */
            if(c_.name!=c.name||!(x_>s.x0&&x_<s.x1)){
                    flag=0;
                    break;
            }
            flag_=1;
            index++;
        }
        i++;
    }
    if(!flag_||!flag)return 0;
    
    for(int i=0;i<b.line.size();i++){
        OCRMatch &c=originalMatch[b.line[i]];
        //cout<<b.line[i]<<" "<<c.name<<" c:"<<c.correlation<<" p:"<<c.pCount<<endl;
        int x=c.xCenter+MATRIX_BORDER;
        if(x>s.x0&&x<s.x1){
            b.line.erase(b.line.begin()+i);
            b.dIndex.erase(b.dIndex.begin()+i);
            i--;
        }
    }
    if(!b.line.size()){
        b.correlation=0;
        return 1;
    }
    for(int i=0;i<b.line.size();i++){
        a.line.push_back(b.line[i]);
        a.dIndex.push_back(b.dIndex[i]);
    }
    a.name="";
    a.correlation=0;
    for(int i=0;i<a.line.size();i++){
        OCRMatch &c=originalMatch[a.line[i]];
        a.correlation+=c.correlation;
        a.name+=c.name;
        
    }
    a.correlation/=a.line.size();
    a.x0=originalMatch[a.line[0]].x0;
    a.x1=originalMatch[a.line[a.line.size()-1]].x1;
    a.setSize(1);
    a.setSize(0);
    setImagePointCount(originalMatch,lineImg32,a,0);
    if(b.length>a.length)a.length=b.length;
    b.correlation=0;
    
    if(!b.line.size()){
        b.correlation=0;
        return 1;
    }
    b.name="";
    b.correlation=0;
    for(int i=0;i<b.line.size();i++){
        OCRMatch &c=originalMatch[b.line[i]];
        b.correlation+=c.correlation;
        b.name+=c.name;
        
    }
    b.length=(uint)b.line.size();
    b.correlation/=b.length;
    b.x0=originalMatch[b.line[0]].x0;
    b.x1=originalMatch[b.line[b.line.size()-1]].x1;
    b.setSize(1);
    b.setSize(0);
    setImagePointCount(originalMatch,lineImg32,b,0);

    return 1;
}


uint collectMatchWithDirection(vector<OCRMatch>&originalMatch,GBitmap *lineImg32, OCRMatch &a, OCRMatch &b, OCRBox &s){
    //проверяем можно ли состыковать эти гипотезы
    //проверяем где находится начало общей области
    uint p=0;
    if(a.s.x0<s.x0&&a.s.x1>s.x1){
        p=combineMatchLetters(originalMatch,lineImg32,a,b,s);
        
        return p;
    }
    if(b.s.x0<s.x0&&b.s.x1>s.x1){
        p=combineMatchLetters(originalMatch,lineImg32,b,a,s);
        return p;
    }

    if(a.s.x0==s.x0&&b.s.x0==s.x0&&a.line.size()>b.line.size()){
        p=combineMatchLetters(originalMatch,lineImg32,a,b,s);
        return p;
    }
    if(a.s.x0==s.x0&&b.s.x0==s.x0&&b.line.size()>a.line.size()){
        p=combineMatchLetters(originalMatch,lineImg32,b,a,s);
        return p;
    }
    
    if(a.s.x0<s.x0&&b.s.x0==s.x0){
        p=combineMatchLetters(originalMatch,lineImg32,a,b,s);
        return p;
    }
    if(b.s.x0<s.x0&&a.s.x0==s.x0){
        p=combineMatchLetters(originalMatch,lineImg32,b,a,s);
        return p;
    }
    return p;
}


void GLogicProcessor::printDIndex(OCRMatch &a){
    if(a.dIndex.size()){
        string st=UnicodeToYagpo(a.name);
        wstring sW=UTF_to_Unicode(st);
        for(int j=0;j<sW.size();j++){
            st=Unicode_to_UTF(sW[j]);
            st=YagpoToUni(st);
            if(a.dIndex[j]==1){
                cout<<st<<"་";
            }else if(a.dIndex[j]==2){
                cout<<st<<2;
            }else{
                cout<<st<<"_";
            }
        }
        
    }
    cout<<endl;

}

/** @brief
 //производим разбор результатов словаря по отношению к исходному тексту
 //сравниваем насколько точно каждая словарная гипотеза описывает данные текста и сравниваем гипотезы между собой.*/

void GLogicProcessor::testWordLine(GBitmap *lineImg32,
                                   vector<OCRMatch>&line,
                                   vector<OCRMatch>&originalMatch){
    
    
    //drawGrapeLine(line); exit(0);

    //((GImageEditor*)inputData.imageEditor)->WriteImageData(lineImg32,"/_Image2OCR/_1Draw32.jpg",0); exit(0);
    //GBitmap *letterAImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);
    //GBitmap *letterBImg=GBitmap::create(lineImg32->columns(),lineImg32->rows(),BITMAP_32);

    //для каждой гипотезы необходимо знать сколько фокальных точек текста она покрывает.
    //также неободимо выяснить сколько точек фокальных линий текста находящихся в габаритах гипотезы не учитывается данной гипотезой
    
    
    //line - ответы словаря
    //dLine - пары букв
    //originalMatch - стеки и элменты стеков

    //сначала для каждой гипотезы устанавливаем величину pCount (количество точек фокальных линий текста которые входят в гипотезу)
    //также вычислем значения массива интегральных сумм. Алгоритм аналогичен алгоритму адаптивной бинаризации GBitmap::binarisation
    //массив интегральных сумм позволяет вычислить значение pCount для любого произвольного участка гипотезы.
    //поскольку гипотезы перекрываются по оси Х массив нтегральных сумм одномерный
    lineImg32->unpackImg(0);
    
    for(int m=0;m<line.size();m++){
        OCRMatch &a=line[m];
        if(!a.correlation)continue;
        if(a.line.size()){
            if(m==-1){
                setImagePointCount(originalMatch,lineImg32,a,1);
            }else{
                setImagePointCount(originalMatch,lineImg32,a,0);
            }
            
            
        }
        a.dIndex[a.dIndex.size()-1]=2;  // =0 0 не имеет ограничитель слога, =1 имеет и последняя буква словарной гипотезы имеет неопределенный органичитель слога
        if(a.dIndex.size()>1){
            a.dIndex[1]=2;
        }
        if(a.dIndex.size()>2){
            a.dIndex[a.dIndex.size()-2]=2;
        }
    }
    
    
    int print=0
    ;
    
        sort(line.begin(),line.end(),sortMatchL);
    
        //drawGrapeLine(line); exit(0);
        int t=-1;
    
        for(int m=0;m<line.size();m++){
            if(!line[m].correlation)continue;
            //if(line[t].correlation){cout<<" "<<m<<" tc:"<<line[t].correlation<<" n:"<<line[t].name<<" ";  printDIndex(line[t]);}
            //cout<<" "<<m<<" c:"<<line[m].correlation<<" m:"<<line[m].name<<" x0:"<<line[m].line.size()<<endl;
            //cout<<" "<<m<<" c:"<<line[t].correlation<<" n:"<<line[t].name<<" x1:"<<line[t].x1<<endl;   //379 //343
            //print=0;if(m==t){
            //    print=1;
            //}

            int length=0;
            int correlation=0;
            
            //до тех пор пока OCRMatch line[m] продолжает изменятся выполняем разбор строки по отношению к line[m]
            while (line[m].line.size()!=length&&line[m].correlation!=correlation){
                length=(int)line[m].line.size();
                correlation=line[m].correlation;
                
                for(int n=0;n<line.size();n++){
                    if(!line[n].correlation||m==n){
                        continue;
                    }
                    if(print){cout<<" "<<m<<" n:"<<n<<" c1:"<<line[m].correlation<<" c:"<<line[n].correlation<<" n:"<<line[n].name<<" x0:"<<line[n].line.size()<<" ";printDIndex(line[m]); }
                    
                    if(print&&n==2715){
                        cout<<endl;
                    }
                    //print=0; if(n!=134)continue; print=1;
                    //прежде всего определяем площадь пересечения
                    OCRBox s;
                    intArea(line[m].s,line[n].s,s);

                    if(s.area>0){
                        //два OCRMatch имеют общую площадь пересечения
                        //необходимо решить какая гипотеза более верная.
                        //Сначала проверяем насколько полно каждая гипотеза описывает площадь пересечения
                        //при этом в габариты пересечения включаем площадь над сторокой и под строкой
                        s.y0=0;
                        s.y1=lineImg32->columns();
                        
                        OCRMatch &a=line[m];
                        OCRMatch &b=line[n];
                        DR("m:"<<m<<" n:"<<n<<" "<<a.name<<" "<<b.name<<endl);
                        
                        if(collectMatchWithDirection(originalMatch,lineImg32,a,b,s)){
                            if(!a.correlation){
                                break;
                            }else{
                                continue;
                            }
                        };
                        
                        //s.printBox();
                        //a.s.printBox();
                        //b.s.printBox();

                        //определяем покрытие площади пересечения первой гипотезой.
                        //для более точного определения взаимной корреляции необходимо также подсчитывать количество точек скелета текста перекрытого каждой гипотезой букв
                        //в непрямоугольной области пересечения масок двух букв для которых вычисляется взаимная корреляция.
                        int pCountA=setPointCount(a,s);
                        int pCountB=setPointCount(b,s);
                        if(pCountA==0||pCountB==0)continue; //одна из гипотез не претендует на площадь пересечения
                        
                        DR("m:"<<m<<" n:"<<n<<" "<<a.name<<" "<<b.name<<" pCountA:"<<pCountA<<" pCountB:"<<pCountB<<" a.pCount:"<<a.pCount<<" b.pCount:"<<b.pCount<<endl);
                        
                        float correlationA=setCorrelation(originalMatch,a,s);
                        float correlationB=setCorrelation(originalMatch,b,s);
                        if(correlationA<0||correlationB<0)continue;
                        
                        //проверяем какая гипотеза имеет большую корреляцию и имееет большее покрытие области пересечения
                        float dP=(float)pCountA/pCountB;
                        float dC=(float)correlationA/correlationB;
                        float lC=(float)(100+(float)a.line.size()/b.line.size())/101;
                        float fC=lC;
                        if(a.length>5&&b.length>5)fC=(float)((100+(float)a.allMatchCount/b.allMatchCount)/101+lC)/2;
                        float sC=(float)(dP+dC+fC)/3;
                        
                        if(sC>1){
                            DR("erase b"<<endl);
                            setMatchLetters(line,originalMatch,lineImg32,a,b,s);
                        }else{
                            //необходимо стереть из гипотезы a все буквы на которые претендует гипотеза b
                            DR("trim a"<<endl);
                            setMatchLetters(line,originalMatch,lineImg32,b,a,s);
                            if(!a.correlation){
                                DR("erase a"<<endl);
                                break;
                            }
                        }
                    }
                }
            }
        }
    //drawGrapeLine(line); exit(0);
}

void GLogicProcessor::testStackLetterNew(vector<OCRMatch>&line,vector<OCRMatch>&originalMatch,GBitmap* lineImg32,GBitmap* letterAImg,GBitmap* letterBImg){
    
    //сначала для каждой гипотезы устанавливаем величину pCount (количество точек фокальных линий текста которые входят в гипотезу)
    //также вычислем значения массива интегральных сумм. Алгоритм аналогичен алгоритму адаптивной бинаризации GBitmap::binarisation
    //массив интегральных сумм позволяет вычислить значение pCount для любого произвольного участка гипотезы.
    //поскольку гипотезы перекрываются по оси Х массив нтегральных сумм одномерный
    
    //drawGrapeLine(line); exit(0);
    
    lineImg32->unpackImg(0);
    
    for(int m=0;m<line.size();m++){
        OCRMatch &a=originalMatch[line[m].line[0]];
        if(!a.correlation)continue;
        if(a.pSum.size())continue;
        if(a.line.size()){
            if(m==-1){
                setImagePointCount(originalMatch,lineImg32,a,1);
            }else{
                setImagePointCount(originalMatch,lineImg32,a,1);
            }
        }
    }
    
    //int t=41;
    
    for(int m=0;m<line.size();m++){
        //cout<<" "<<m<<" tc:"<<line[t].correlation<<" n:"<<line[t].name<<" ";  printDIndex(line[t]);
        //cout<<" "<<m<<" c:"<<line[t].correlation<<" n:"<<line[t].name<<" x0:"<<line[t].line.size()<<endl;   //379 //343
        print=0;if(m==-1){
            print=1;
        }
        if(!line[m].correlation)continue;
        if(line[m].line.size()>1)continue;
            
            for(int n=0;n<line.size();n++){
                if(!line[n].correlation||m==n){
                    continue;
                }
                if(line[n].line.size()>1)continue;
                
                OCRMatch &a=originalMatch[line[m].line[0]];
                OCRMatch &b=originalMatch[line[n].line[0]];
                
                
                //if(line[n].name=="")continue;
                if(print){cout<<" "<<m<<" c:"<<line[n].correlation<<" n:"<<line[n].name<<" x0:"<<line[n].line.size()<<" ";printDIndex(line[m]); }
                
                if(print&&n==-1){
                    cout<<endl;
                }
                //print=0; if(n!=134)continue; print=1;
                //прежде всего определяем площадь пересечения
                OCRBox s;
                intArea(a.s,b.s,s);
                
                if(s.area>0){
                    //два OCRMatch имеют общую площадь пересечения
                    //необходимо решить какая гипотеза более верная.
                    //Сначала проверяем насколько полно каждая гипотеза описывает площадь пересечения
                    //при этом в габариты пересечения включаем площадь над сторокой и под строкой
                    s.y0=0;
                    s.y1=lineImg32->columns();
                    
                    DR("m:"<<m<<" n:"<<n<<" "<<a.name<<" "<<b.name<<endl);
                    
                    //s.printBox();
                    //a.s.printBox();
                    //b.s.printBox();
                    
                    //определяем покрытие площади пересечения первой гипотезой.
                    int pCountA=setPointCount(a,s);
                    int pCountB=setPointCount(b,s);
                    if(pCountA<a.pCount/10||pCountB<b.pCount/10)continue; //одна из гипотез не претендует на площадь пересечения
                    
                    
                    DR("m:"<<m<<" n:"<<n<<" "<<a.name<<" "<<b.name<<" pCountA:"<<pCountA<<" pCountB:"<<pCountB<<" a.pCount:"<<a.pCount<<" b.pCount:"<<b.pCount<<endl);
                    
                    float correlationA=setCorrelationMask(originalMatch,a,s);
                    float correlationB=setCorrelationMask(originalMatch,b,s);

                    
                    //проверяем какая гипотеза имеет большую корреляцию и имееет большее покрытие области пересечения
                    float dP=(float)pCountA/pCountB;
                    float dC=(float)correlationA/correlationB;
                    float sC=(float)(dP+dC)/2;
                    
                    if(sC>1){
                        DR("erase b"<<endl);
                        b.correlation=0;
                        line[n].correlation=0;
                    }else{
                        DR("erase a"<<endl);
                        a.correlation=0;
                        line[m].correlation=0;
                        break;
                    }
                }
            }

    }
    //drawGrapeLine(line); exit(0);
    


}

