// C-
// C- This software is subject to, and may be distributed under, the
// C- GNU General Public License, either Version 2 of the license,
// C- or (at your option) any later version. The license should have
// C- accompanied the software or you may obtain a copy of the license
// C- from the Free Software Foundation at http://www.fsf.org .
// C-
// C- This program is distributed in the hope that it will be useful,
// C- but WITHOUT ANY WARRANTY; without even the implied warranty of
// C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// C- GNU General Public License for .more details.
// C-

/*  Interface  to GMemoryLib dynamic library  */

#ifndef _OCRLibC_H_
#define _OCRLibC_H_

#define RECORD_LIMIT 25

extern "C" {

typedef struct pageRecord {
    char *text;
    char *data;
    char *db;
    char *ln;
    
    int index;
    int field;
    int pageCount;
    int bookCount;
    int intData;
    int command;
    int mode;
    int dataSize;
} pageRec;

void cpstr(pageRec &page,const char *str);
//функция чтения и передачи данных в интерфейс Python на основе бибиотеки CTypevvbv+++
char* getPageStream(pageRec *page_);
};

#endif
