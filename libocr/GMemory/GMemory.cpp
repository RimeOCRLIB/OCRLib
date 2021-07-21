//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-
#include "GMemory.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

namespace ocr
{
indexRecord::indexRecord()
{
	status = 0;
}

tableRecord::tableRecord()
{
	status = 0;
}

void GMemory::destroy(void)
{
	delete this;
}

GMemory::GMemory()
{
	try
	{
		init();
	}
	catch (int a)
	{
		destroy();
	}
}

// ----- initialization

void GMemory::init()
{
}
};
