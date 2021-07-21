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

#ifndef _GMemoryLib_H_
#define _GMemoryLib_H_

#define RECORD_LIMIT 25


//файл дублирует OCRLib.h с меньшим количеством функций

//#ifdef OCRLib_DLIB
extern "C" {
//#endif
namespace ocr{

typedef struct pageRecord {
  const char *db;
  int index;
  int field;
  int pageCount;
  int bookCount;
  const char *text;
  char *data;
  int intData;
} pageRec;

void initCommandDataGraph();

void OCRletterProcess(void *conn, char *buff, unsigned int letterID);
void letterImg(void *conn, char *buff,unsigned int letterID);
void letterSVG(void *conn, char *name, char *drawBuf, unsigned int letterID);
void drawTwoLettersCorrelation(void *conn, char *drawBuf, unsigned int index1, unsigned int index2);
int rebuildOCRLetterFont(const char *fontPath);

};
};
//#endif

#endif








