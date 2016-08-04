
#ifndef DEBUG_H
#define DEBUG_H

//#define INPUT_XML       //режим запуска с параметрами командной(1) строки заданной в файле /input.xml
#define REPORT_LINE     //режим отладки построчного распознавания

#define MAIN_MODE
//#define STACK_MODE    //режим сборки стеков из результатов распознавания записанных в файл
//#define STACK_DRAW    //создаем данные необходимые для отображения процесса анализа OpenType

#endif








/*
 (1)//параметры командной строки это параметры передаваемые в HTML интерфейсе при вызове функций интерфейса
 например  xml=%3CfileList%3E/1.tif%3C/fileList%3E%3CocrData%3EbatchOCR%3C/ocrData%3E%3CocrLn%3EocrTibetan%3CocrLn%3E%3Cscale%3E100%3C/scale%3E
 подробнее см.  processInput.mm

*/