#ifndef OCRLIB_H
#define OCRLIB_H


#include "config.h"

//_common interfaice
#include <assert.h>
#include <ctype.h>
#ifdef WIN32
#include <direct.h>
#include <io.h>
#include <tchar.h>
#include <windows.h>
#include <winreg.h>
#endif
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <wctype.h>
#include <sys/stat.h>

#include "OCRTypes/OCRTypes.h"
#include "php2stl.h"
#include "../pugixml/pugixml.hpp"
#include "GBitmap.h"
#include "GBitset.h"
#include "GBitsetMatrix.h"
#include "GBitsetContour.h"
#include "GBitsetOCR.h"
#include "php2stl.h"
#include "GMainEditor.h"
#include "GImageEditor.h"
#include "GFontEditor.h"
#include "GLogicProcessor.h"
#include "GVector.h"
#include "GMemory.h"
#include "GMap.h"
#include "../re2/re2.h"




#endif

