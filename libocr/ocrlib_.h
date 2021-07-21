#ifndef OCRLIB_H
#define OCRLIB_H


#include "config.h"

//_common interfaice
#include <assert.h>
#include <ctype.h>
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
#ifdef WIN32
	#include <direct.h>
	#include <io.h>
	#include <tchar.h>
	#include <windows.h>
	#include <winreg.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <unistd.h>

//OCRLib inner classes
#include "OCRTypes/OCRTypes.h"
#include "OCRString/php2stl.h"
#include "../pugixml/pugixml.hpp"
#include "GBitmap/GBitmap.h"
#include "GBitset/GBitset.h"
#include "GBitsetMatrix/GBitsetMatrix.h"
//#include "GBitsetContour.h"
#include "GBitsetOCR/GBitsetOCR.h"
#include "GMainEditor/GMainEditor.h"
#include "GImageEditor/GImageEditor.h"
#include "GFontEditor/GFontEditor.h"
#include "GLogicProcessor/GLogicProcessor.h"
#include "GMemory/GVector.h"
#include "GMemory/GMemory.h"
#include "GMemory/GMap/GMap.h"
#include "GGraph/GGraph.h"
#include "GGraphOCR/GGraphOCR.h"

#ifdef ESP
	#include "esp_command.h"
#endif


#endif










