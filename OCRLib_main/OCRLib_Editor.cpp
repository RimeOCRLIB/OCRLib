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
// C- GNU General Public License for more details.
// C-
/*  Implemmentation  to GMemoryLib dynamic library  */

#include "OCRLib.h"
#include "../libocr/config.h"
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#ifdef MACOS
   #include <malloc/malloc.h>
#endif

#include "../libocr/OCRTypes/OCRTypes.h"
#include "../libocr/GFontEditor/GFontEditor.h"
#include "../libocr/GImageEditor/GImageEditor.h"
#include "../libocr/GLogicProcessor/GLogicProcessor.h"
#include "../libocr/GMainEditor/GMainEditor.h"
#include "../libocr/GMemory/GMemory.h"
#include "../libocr/OCRString/php2stl.h"

#ifdef ESP
   #include "esp_command.h"
#endif

//#define DB_NAME "LIBRARY_CATALOG"
//#define DB_NAME "DHARMABOOK"

using namespace std;

#ifdef MAGICK
   #include <Magick++.h>
   using namespace Magick;
#endif

//#ifdef OCR_DYLIB
extern "C" {
//#endif




//#ifdef OCR_DYLIB
};
//#endif
