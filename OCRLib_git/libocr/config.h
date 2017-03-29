#ifndef CONFIG_H
#define CONFIG_H
#include <time.h>
#include<string>
#include<vector>
#include<list>
#include<map>
#include <fstream>
#include <iostream>

//#define OCR_woodblock

#define HASH_PATH "_HASH/"
#define DATADIR "/ramdisk"


#define MAIN_HOST "http://www.buddism.ru"

#define FSIZE  20000                                //максимальное количество букв в GFont

#define UNIX
#define MACOS
#define MACOS10_8
#define COCOA

#define FORK
//#define QT4_NOKIA


#ifdef UNIX
  #undef WIN32_DRAW
  #undef WIN32
  #define LOAD_PTR  LDDQU    //on Mac 
  #define MOV_PTR MOVAPS
#else
  #define LOAD_PTR  MOVDQU    //on Mac LDDQU
  #define MOV_PTR MOVDQU
#endif



#undef DRAW1  // #undef
#undef DRAW_PC

//#define DEBUGLVL

//#define DEBUGLVL_GREY   //формируются изображения объектов GMatrixOCR, GBitset
//#define DEBUGLVL_DRAW  //формируются изображения объектов GMatrixOCR с отображением хода распознавания
#define PRINT
#define REPORT


#define  DICT_REPORT_SIZE 11 ///минимальная длинна строки ответа словаря в байтах, начиная с которой строка ответа словаря применяется для расстановки слогов в текстовой строке
#define GRAMMAR_LOG 0        //печать грамматического разбора строки 

#define MMAP_IN_MEMORY         //mmap отображает содержимое больших дисковых файлов в память.
//#define READ_IN_MEMORY         //fread читает содержимое больших дисковых файлов в память.

#define HASH_DICTIONARY  1     //граматический словарь с вторичным индексом реализованным как сортированный массив значений хеша и отступа в текстовом словаре
//#define STRING_DICTIONARY 1    //граматический словарь с вторичным индексом реализованным как текстовой файл с прямым поиском подстроки в строке. Номера строк соответствуют строкам в словаре

//------------------------------------------------------------------------------------------------------------------------------------

// для выполняемой функции Matrix_Lion.h

// Способы подсчета единиц при вычислении коэффициента корреляции признаков в зависимости от операционной системы (32/64 разряда) и
// типа процессора с "POPCNT" или без ( инструкция подсчета единиц "POPCNT"была добавлена ​​в процессор "Nehalem" (SSE4.2) ). 
// Определяет базовое быстродействие процесса распознавания. Список по увеличению быстродействия 

//#define CPU_OS32_64   /// CPU универсальная ™С™ версия 32/64р, использует 32 разрядную арифметику, работает везде, самая медленная (базовая)
/// CPU (any processor) with 32/64 bit OS, realisation of C++ function, universal

//#define CPU_OS64      /// CPU ™С™ версия 64р, использует 64 разрядную арифметику, работает в 64 разрядных ОС, в 1,57 раз быстрее базовой
/// CPU (any processor) with 64 bit OS, realisation of C++ function

//#define CPU_OS64_popcnt /// CPU ™С™ версия 64р, использует 64 разрядную арифметику, работает в 64 разрядных ОС,
//  подсчитывая суммы единиц в 64х разрядных словах с помощью intrinsic-функции (интринсики) на popcnt.
//  выполняется на 3% быстрее чем  ASM x86_64, Assembler версия, с использованием SSE4.2, 64р "POPCNT",

//#define ASM_OS32_SSE2 /// ASM, Assembler версия, с использованием SSE2 128р, работает в 32 разрядных ОС, в 2,3 раза быстрее базовой
/// x86_64 with 32 bit OS, SSE2, realisation of asm function


//#define ASM_OS64_SSE3 /// ASM, Assembler версия, с использованием SSE3 128р, работает в 64 разрядных ОС, в 2,45 раза быстрее базовой
/// x86_64 with 64 bit OS, SSE3, realisation of asm function

 //#define ASM_OS64_SSE42_POPCNT /// ASM, Assembler версия, с использованием SSE4.2, 64р "POPCNT", работает в 64 разрядных ОС, в 2.76 раз быстрее. Компиляторы GCC -masm=intel (linux, macos 10.6).
/// x86_64 with 64 bit OS, SSE4.2 with POPCNT, realisation of asm function for GCC compilation with -masm=intel (XCode 4.0).

#define ASM_OS64_SSE42_POPCNT_ATT /// ASM x86_64, Assembler версия, с использованием SSE4.2, 64р "POPCNT", работает в 64 разрядных ОС,
    //в 2.03 раза быстрее чем CPU_OS32_64  (оптимизированные версии) /// CPU универсальная
    //в 1.58 раз быстрее чем CPU_OS64   (оптимизированные версии)   /// CPU ™С™ версия 64р
    //Компиляторы GCC -masm=att (XCode,QT).
/// x86_64 with 64 bit OS, SSE4.2 with POPCNT, realisation of asm function for GCC compilation with -masm=att (XCode,QT).
// при оптимизации компиляции в 1.57 раза быстрее отладочной версии

//-------------------------------------------------------------------------------------------------------------------------------------

// старые обозначения
//#define CPP_32_64    //#define CPU_OS32_64
//#define CPP_64       //#define CPU_OS64
//#define SSE_CLASSIC  //#define ASM_OS32_SSE2
//#define x86_64       //#define ASM_OS64_SSE3
//#define POPCNT_64    //#define ASM_OS64_SSE42_POPCNT

//-------------------------------------------------------------------------------------------------------------------------------------

// для отладочной функции Matrix_Lion_Search.h

//#define ASM_OS64_SSE42_POPCNT
//#define ASM_OS64_SSE42_POPCNT_Search
///  РЕЗЕРВ  ///
//#define CPP_64_DEBUG                   ///  РЕЗЕРВ
//#define ASM_OS32_MMX                   ///  РЕЗЕРВ  //  ASM  Assembler MMX 64р, находится в функции Matrix_Lion_Search.h

//-------------------------------------------------------------------------------------------------------------------------------------



//#define MMX
#define ASM_ATT
//#define ASM_INTEL
//#define NO_ASM

 
#ifdef REPORT
#define print(x)  cout<<x<<endl
#define DM(x) //cout<<x;
#define DP(x) //cout<<x;
#define DL(x) out<<x;
#define DC(x) //cout<<x;
#define DT(x) if(print)cout_<<x;
#define DT2(x) if(print>1)cout_<<x;
#define DT3(x) if(print>2)cout_<<x;
#define DT4(x) if(print>3)cout_<<x;
#define DR_(x) cout<<x;
#define DR(x) if(print)cout<<x;
#define DR2(x) if(print>1)cout<<x;
#define DS(x) x
#define DS1(x) 
#define DA(x) //if(print)c_out_<<x
#define PR(x) x



#else
#define print(x)
#define DM(x)
#define DP(x)
#define DC(x)
#define DT(x)
#define DS(x) 
#define DA(x)
#endif

#define cout_ inputData.c_out
//#define cout_ cout

#define END "<br>"<<endl
#define SH(x) ShowMessage(x)
#define OUT_S(_X_) ofstream  out; out.setf(ios::unitbuf); out.open(_X_.c_str());
#define OUT_C(_X_) ofstream  out; out.setf(ios::unitbuf); out.open(_X_);
#define TIME_START clock_t  tm_start, tm_end; double time;  tm_start=clock();
#define TIME_PRINT tm_end=clock(); tm_end-=tm_start; time=(float)tm_end/CLOCKS_PER_SEC;DC("time="<<time<<END);tm_start=clock();

#define TIME_PRINT_ tm_end=clock(); tm_end-=tm_start; time=(float)tm_end/CLOCKS_PER_SEC;cout<<"time="<<time<<endl;  tm_start=clock();
#define TIME_START_ADD tm_start=clock();
#define TIME_ADD tm_end=clock(); tm_end-=tm_start; time+=(float)tm_end/CLOCKS_PER_SEC; tm_start=clock();
#define TIME_PRINT_ADD cout<<"time="<<time<<endl;  tm_start=clock();



//<<" tm_end="<<tm_end<<" CLOCKS_PER_SEC="<<CLOCKS_PER_SEC<<endl;
#define TIME_START_ASM double time0,tm_start0,tm_end0; pTicks tcs; asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start0=tcs.tx;    
#define TIME_PRINT_ASM asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end0=tcs.tx; tm_end0-=tm_start0; time0=(double)tm_end0/3000000000;cout<<"time_asm="<<time0<<endl;  asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start0=tcs.tx;    

#define TIME_START_ADD_ASM asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start0=tcs.tx;    
#define TIME_ADD_ASM asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_end0=tcs.tx; tm_end0-=tm_start0; time0+=(double)tm_end0/3000000000; asm("rdtsc\n": "=a"(tcs.dw.tl),"=d"(tcs.dw.th));  tm_start0=tcs.tx;  

typedef union ticks
{   unsigned long long tx;
    struct dblword { long tl,th; } dw; // little endian
} pTicks;     //структура данных запроса числа тактов процессора (один такт 0.3 наносекунды, 3.3GHz)


#define TIFF_FILE 1
#define PICT_SIZE 192
#define MAXREQUEST 524288
#define MAX_OCR_LINE 100


struct commandData
{
    std::vector<std::string> fileList;
    std::vector<std::string> folderList;		
    std::map<std::string,std::string>data;
    std::vector<std::string>fileSet;
    std::vector<int>lineSize;
    std::vector<int>lineIndex;
    std::ofstream c_out;
    std::ofstream log;
    int CoreProcessing;
    int imgW,imgH,argc;
    int start;
    int startIndex;
    unsigned int idNumber;
    int x0,y0,x1,y1;
    int processIDStatus[100];
    int processID;
    int num_cores;   //количество процессоров в системе
    //Handlers for GUI
    int init;
    int OCRMode;
    int socketFD;  //socket file descriptor
    void *mainEditor;
    void *imageEditor;
    void *logicProcessor;
    void *fontEditor;
    void *matrix;
    void *Core;
    void *longMemory;
    void *Angel;
    void *prefVector;  //GVector сохраняемых данных программы
    void *pref;        //GStr<int> сохраняемых данных программы
    const char* strData;
};

extern commandData inputData;


#endif
