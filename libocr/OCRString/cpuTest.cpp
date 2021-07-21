// ---------------------------------------------------------------------------------------------
//  Cross platform Functions
// ---------------------------------------------------------------------------------------------

// Cherubim-engine. A small and modular cross platform game / graphics engine
// http://cherubim-engine.googlecode.com/svn-history/r64/trunk/Source/BaseLib/private/CBL_Cpu.cpp

// Определение типа процессора: Intel, AMD.  // , PowerPC
// Для процессоров Intel определяются параметры: MMX, SSE, SSE2, SSE3, SSE41, SSE42, Altivec,
// и так же другие специальные параметры (Popcnt, LinuxKernel).
// Ключ KeySMP для запуска функции setMatrix_Point в  Matrix_Lion.h

//----------------------------------------------------------------------------------------------------------

#include "php2stl.h"
////#include "CBL_Cpu.h"

#include <csignal>
#include <cstring>

////#include <iostream>
////#include "CBL_Debug.h"
//#include "CBL_Fpu.h"

#ifdef CB_WINDOWS
#include <windows.h>
#include <float.h>
#else
#include <unistd.h>
#endif

//static const int16 SinglePrecision = 0x000;
//static const int16 DoublePrecision = 0x200;
static const int16 ExtendedPrecision = 0x300;

// Cross compiler assembler
#define CB_ASM __asm
#define CB_ASM_pushfl pushfd
#define CB_ASM_popfl popfd

static SCpuInfo g_Cpu;

////CB_DECLARE_SUBNAMESPACE(Cpu)

// ---------------------------------------------------------------------------------------------
// функция определения характеристик процессора.
// ---------------------------------------------------------------------------------------------

void StartCpuInfo()
{
	//uint32 Xcode_Qt=0;  // Xcode_Qt=0 компилятор Xcode на "ASM" в нотации Intel
	// Xcode_Qt=1 компилятор Qt на "C"
	// Xcode_Qt=2 компилятор Qt на "ASM" в нотации AT&T

	// вызов функции определения версии ядра Linux в системе.
	GetNumLinuxKernel();
	// вызов функции определения количества процессорных ядер с помощью функции sysconf(_SC_NPROCESSORS_ONLN).
	GetNumPhysicalCpus();
	int print = 0;

	//#ifdef REMARK
	DT(endl
	   << endl
	   << "**********************************************************************************************************" << endl);
	DT("Старт функции определения характеристик процессора" << endl);

	// вызов функции определения типа процессора: Intel, AMD.
	// для процессоров Intel определяются параметры: MMX, SSE, SSE2, SSE3, SSE41, SSE42, Altivec.  Popcnt
	GetCpuInfo(); // SCpuInfo a_=GetCpuInfo();   inputData.CPUInfo=&a_;

	// вывод на экран всех параметров процессора: CpuType=1  SIMDType=7  Processors=4  Cores=8  LogicalUnits=1
	DT("CpuType=" << g_Cpu.Type << "  SIMDType=" << g_Cpu.SIMD << "  Processors=" << g_Cpu.Processors << "  Cores=" << g_Cpu.Cores << "  LogicalUnits=" << g_Cpu.LogicalUnits << "  CoresSysconf=" << g_Cpu.CoresSysconf << "  Popcnt=" << g_Cpu.Popcnt << "  LinuxKernel=" << g_Cpu.LinuxKernel << endl);

	// вывод на экран всех расшифровок типа процессора CpuType:  Intel=1, AMD=2.
	DT("CpuType: "
	   << " Intel=" << Intel << " AMD=" << AMD << endl);

	// вывод на экран расшифровки параметра  SIMDType:  SSE=3, SSE2=4, SSE3=5, SSE41=6, SSE42=7, Altivec=8.
	DT("SIMDType: "
	   << " MMX=" << MMX << " SSE=" << SSE << " SSE2=" << SSE2 << " SSE3=" << SSE3 << " SSE41=" << SSE41 << " SSE42=" << SSE42 << " Altivec=" << Altivec << endl
	   << endl);

	inputData.num_cores = g_Cpu.CoresSysconf * 0.85;

	// выбор ключа (способа подсчета единиц в слайсах) для запуска функции setMatrix_Point в  Matrix_Lion.h
	DT("запускаем функцию   setMatrix_Point  -  ");
	if (g_Cpu.SIMD == MMX)
	{ // 1. MMX
		g_Cpu.KeySMP = 1;
		DT("MMX" << endl);
	}
	if (g_Cpu.SIMD == SSE || g_Cpu.SIMD == SSE2 || g_Cpu.SIMD == SSE3 || g_Cpu.SIMD == SSE41)
	{ // 2. SSE или SSE2 или SSE3 или SSE41
		g_Cpu.KeySMP = 2;
		DT("SSE-SSE41" << endl);
	}
	if (g_Cpu.Popcnt == 1 || g_Cpu.SIMD == SSE42 || g_Cpu.SIMD == Altivec)
	{ // 3. SSE42 или Altivec на POPCNT
		g_Cpu.KeySMP = 3;
		DT("SSE42-Altivec & popcnt" << endl);
	}

#ifdef NO_ASM
	if (g_Cpu.SIMD == MMX)
	{ // 1. CPU32
		g_Cpu.KeySMP = 4;
		DT("CPU32" << endl);
	}
	if (g_Cpu.SIMD == SSE || g_Cpu.SIMD == SSE2 || g_Cpu.SIMD == SSE3 || g_Cpu.SIMD == SSE41)
	{ // 2. CPU64
		g_Cpu.KeySMP = 5;
		DT("CPU64" << endl);
	}
	if (g_Cpu.Popcnt == 1 || g_Cpu.SIMD == SSE42 || g_Cpu.SIMD == Altivec)
	{ // 3. CPU64 на popcnt intrinsic
		g_Cpu.KeySMP = 6;
		DT("CPU64 & popcnt intrinsic" << endl);
	}
#endif
	//#ifdef REMARK
	//cout_<<"ключ для запуска функции   setMatrix_Point  -  g_Cpu.KeySMP="<<g_Cpu.KeySMP<<endl<<" num_cores="<<g_Cpu.CoresSysconf<<endl;
	cout_ << "**********************************************************************************************************" << endl
		  << endl;
	//#endif
}
	// ---------------------------------------------------------------------------------------------

#include <sys/utsname.h>

// ---------------------------------------------------------------------------------------------
// Определение версии ядра Linux в системе // #include <sys/utsname.h>
// ---------------------------------------------------------------------------------------------

void GetNumLinuxKernel()
{
	struct utsname buf;
	uname(&buf);
	//cout_<<"Узнать версию ядра Linux в системе   uname="<<buf.release<<endl;
	string str = buf.release;
	g_Cpu.LinuxKernel = str;
	//return str;  // string GetNumLinuxKernel() {
}
/**/

// ---------------------------------------------------------------------------------------------
// Определение количества процессорных ядер в системе, без применения функции cpuid
// ---------------------------------------------------------------------------------------------

inline uint32 GetNumPhysicalCpus()
{
#ifdef WIN
	// Windows query
	SYSTEM_INFO Info;
	GetSystemInfo(&Info);
	g_Cpu.CoresSysconf = Info.dwNumberOfProcessors;
	///return Info.dwNumberOfProcessors;
#else
// Unix query
#ifdef MACOS
	g_Cpu.CoresSysconf = (int)sysconf(_SC_NPROCESSORS_ONLN);
	///return sysconf(_SC_NPROCESSORS_ONLN);
#endif
#ifdef LINUX
	g_Cpu.CoresSysconf = sysconf(_SC_NPROCESSORS_ONLN);
	///return sysconf(_SC_NPROCESSORS_ONLN);
#endif

#endif

	return 1; // Fallback
}

	// ---------------------------------------------------------------------------------------------

#ifdef MACOS

#include <mmintrin.h>

// ---------------------------------------------------------------------------------------------
//  x86 Data
// ---------------------------------------------------------------------------------------------

enum X86Registers
{
	RegEAX = 0,
	RegEBX,
	RegEDX,
	RegECX
}; // EDX and ECX swapped because of manufacturer string
enum FPUState
{
	FPUUnknown = 0,
	FPUSingle,
	FPUDouble
};

//static SCpuInfo g_Cpu;

// ---------------------------------------------------------------------------------------------
//  x86 utility functions
// ---------------------------------------------------------------------------------------------

// Проверка функциониования инструкции cpuid.
// проверка не работает.
static inline bool IsCpuIDSupported()
{
	long Check;
#ifdef ASM_ATT
	asm(
		"mov $0, %%rax\t\n" //начальная установка для cpuid
		"cpuid\t\n"
		"mov %%rax, %[Check]"
		: [Check] "=r"(Check));
#endif
	return (bool)Check; /// Check = true инструкция работает
};

// ---------------------------------------------------------------------------------------------

// запрос к инструкции cpuid и получение ответа по всей раскладке процессора в регистрах eax, ebx, ecx, edx

static inline void QueryCpuID(uint32 _EAX, uint32 _EBX, uint32 _ECX, uint32 _EDX, uint32 *_pRegisters)
{
	_pRegisters[RegEAX] = 0;
	_pRegisters[RegEBX] = 0;
	_pRegisters[RegECX] = 0;
	_pRegisters[RegEDX] = 0;

	if (IsCpuIDSupported())
	{
#ifdef ASM_INTEL
		uint32 OutEAX = 0;
		uint32 OutEBX = 0;
		uint32 OutECX = 0;
		uint32 OutEDX = 0;

		CB_ASM
		{
			mov eax, _EAX; // setting parameters for cpuid
			mov ebx, _EBX;
			mov ecx, _ECX;
			mov edx, _EDX;
			cpuid;			 // get infos
			mov OutEAX, eax; // move infos to array
			mov OutEBX, ebx;
			mov OutECX, ecx;
			mov OutEDX, edx;
		}
		_pRegisters[RegEAX] = OutEAX;
		_pRegisters[RegEBX] = OutEBX;
		_pRegisters[RegECX] = OutECX;
		_pRegisters[RegEDX] = OutEDX;
#else
		unsigned long OutEAX = 0;
		unsigned long OutEBX = 0;
		unsigned long OutECX = 0;
		unsigned long OutEDX = 0;

		asm(
			"cpuid\n"					  // get infos
			"mov %%rax, %[OutEAX]     \n" // move infos to array
			"mov %%rbx, %[OutEBX]     \n"
			"mov %%rcx, %[OutECX]     \n"
			"mov %%rdx, %[OutEDX]     \n"
			: [OutEAX] "=r"(OutEAX), [OutEBX] "=r"(OutEBX), [OutECX] "=r"(OutECX), [OutEDX] "=r"(OutEDX)
			: "a"(_EAX), "b"(_EBX), "c"(_ECX), "d"(_EDX) // setting parameters for cpuid
			:);
		_pRegisters[RegEAX] = (uint32)OutEAX;
		_pRegisters[RegEBX] = (uint32)OutEBX;
		_pRegisters[RegECX] = (uint32)OutECX;
		_pRegisters[RegEDX] = (uint32)OutEDX;

#endif
	}
}
// вся остальная обработка сводится к добыванию отдельных бит и групп бит из регистров и их интерпритация
// и иногда их совместная логическая обработка

// ---------------------------------------------------------------------------------------------

static inline bool IsCpuIDBitSet(uint32 _EAX, uint32 _EBX, uint32 _ECX, uint32 _EDX, uint32 _Bit, X86Registers _Register)
{
	uint32 Registers[4];
	QueryCpuID(_EAX, _EBX, _ECX, _EDX, Registers);

	return (Registers[_Register] & (1 << _Bit)) != 0;
}

// ---------------------------------------------------------------------------------------------

static inline bool IsAMDExtraFunctionSupported(uint32 _FunctionMask)
{
	uint32 RegistersExtra[4];
	QueryCpuID(0x80000000, 0, 0, 0, RegistersExtra); // check for extended functions

	return (RegistersExtra[RegEAX] >= _FunctionMask);
}

// ---------------------------------------------------------------------------------------------

inline void SetFlags(uint16 _StateFlag)
{
#ifdef ASM_INTEL
	CB_ASM
	{
		fldcw _StateFlag;
	}
#endif
}

inline uint16 GetFlags()
{
	int16 StateFlag = 0;
#ifdef ASM_INTEL
	CB_ASM
	{
		fnstcw StateFlag;
	}
#else
	//asm(
	//  "fnstcw $[StateFlag]"
	//    :[StateFlag]"=r"(StateFlag)
	//); ////######### no working
#endif
	return StateFlag;
}

static inline int32 GetFPUState()
{
	return GetFlags();
}

// ---------------------------------------------------------------------------------------------

static inline void SetFPUState(uint32 _State)
{
	SetFlags(_State);
}

// ---------------------------------------------------------------------------------------------
//  implementation
// ---------------------------------------------------------------------------------------------

const SCpuInfo &GetCpuInfo()
{
	//if (g_Cpu.Processors == 0)
	if (1)
	{
		g_Cpu.Processors = GetNumPhysicalCpus();
		g_Cpu.Cores = 1;
		g_Cpu.LogicalUnits = 1;
		g_Cpu.SIMD = NoSIMD;

		uint32 RegistersEAX0[4];
		QueryCpuID(0, 0, 0, 0, RegistersEAX0);

		char *Manufacturer = reinterpret_cast<char *>(&RegistersEAX0[RegEBX]);
		if (strncmp(Manufacturer, "GenuineIntel", 12) == 0)
			g_Cpu.Type = Intel;

		if (strncmp(Manufacturer, "AuthenticAMD", 12) == 0)
			g_Cpu.Type = AMD;

		// Get hyper-threading count
		uint32 RegistersEAX1[4];
		QueryCpuID(1, 0, 0, 0, RegistersEAX1);

		if ((RegistersEAX1[RegEDX] & (1 << 28)) != 0)
		{
			uint32 HTBitMask = 0x00FF0000;
			g_Cpu.LogicalUnits = (RegistersEAX1[RegEBX] & HTBitMask) >> 16;
		}

		// Get multi-core count
		switch (g_Cpu.Type)
		{
		case Intel:
			if (RegistersEAX0[RegEAX] >= 4)
			{
				uint32 RegistersEAX4[4];
				QueryCpuID(4, 0, 0, 0, RegistersEAX4);
				g_Cpu.Cores = (RegistersEAX4[RegEAX] >> 26) + 1; // cout_<<" g_Cpu.Cores="<<g_Cpu.Cores<<endl;   // >> 27
				// д.б. количество ядер в одной упаковке, а по факту это теоретически возможный максимум, а не реальное количество потоков
				//cout_<<"количество процессорных ядер в системе ++ num_cores="<<GetNumPhysicalCpus()<<endl;
			}
			break;

		case AMD:
			if (IsAMDExtraFunctionSupported(0x80000008))
			{
				uint32 RegistersExtra[4];
				QueryCpuID(0x80000008, 0, 0, 0, RegistersExtra); // check multi-core values

				g_Cpu.Cores = (RegistersExtra[RegECX] & 0x0000007F) + 1;
			}
			break;

		default:
			break;
		}

		// Check SIMD support         // напрямер: бит 20 поддержка (SSE4.2), бит 28 (AVX), бит 23 (POPCNT, RegECX)
		// конкретная раскладка информационных битов регистров EDX ECX после выполнения инструкции cpuid на
		// http://siyobik.info/main/reference/instruction/CPUID
		// http://www.sandpile.org/x86/cpuid.htm
		if (IsCpuIDBitSet(1, 0, 0, 0, 28, RegECX)) // Altivec Bit
			g_Cpu.SIMD = Altivec;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 28, RegECX)) // Altivec Bit
			g_Cpu.SIMD = Altivec;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 20, RegECX)) // SSE4.2 Bit
			g_Cpu.SIMD = SSE42;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 19, RegECX)) // SSE4.1 Bit
			g_Cpu.SIMD = SSE41;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 9, RegECX)) // SSE3 Bit
			g_Cpu.SIMD = SSE3;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 26, RegEDX)) // SSE2 Bit
			g_Cpu.SIMD = SSE2;
		else if (IsCpuIDBitSet(1, 0, 0, 0, 25, RegEDX)) // SSE Bit
			g_Cpu.SIMD = SSE;
		else
		{
			switch (g_Cpu.Type)
			{
			case Intel:
				if (IsCpuIDBitSet(1, 0, 0, 0, 23, RegEDX)) // MMX Bit
					g_Cpu.SIMD = MMX;
				break;

			case AMD:
				if (IsAMDExtraFunctionSupported(0x80000001))
				{
					if (IsCpuIDBitSet(0x80000001, 0, 0, 0, 31, RegEDX)) // 3DNow! Bit
						g_Cpu.SIMD = MMX;
				}
				break;

			default:
				g_Cpu.SIMD = NoSIMD;
				break;
			}
		}
		//        g_Cpu.Processors /= g_Cpu.Cores;      // g_Cpu.Processors = g_Cpu.Processors / g_Cpu.Cores;

		if (IsCpuIDBitSet(1, 0, 0, 0, 23, RegECX))
		{
			g_Cpu.Popcnt = 1;
		} // Popcnt Bit
	}

	return g_Cpu;
}
// Данная программа для моделей процессоров Intel типа i5, i7 показывает завышеные в 4 раза параметры: Processors=4  Cores=8  LogicalUnits=16
// это по видимому теоретически возможный максимум, а не то что реальное реализованно в данном процессоре. Это ограничение функции cpuid.
// ( реальные цифры для MacBookPro6,1  Intel Core i7:   Number Of Processors=1, Total Number Of Cores=2 без учета Hyper-Threading )
// Лучше определять количество процессорных ядер с помощью функции  GetNumPhysicalCpus();
// GetNumPhysicalCpus()=4 с учетом Hyper-Threading
// SSE и AVX определяютя корректно

// ---------------------------------------------------------------------------------------------
//	x86 specific functions
// ---------------------------------------------------------------------------------------------

namespace x86
{
void FlushMMXRegister()
{
	_mm_empty();
}

// ---------------------------------------------------------------------------------------------

void SetFastFPU()
{
	uint32 CurrentState = GetFPUState();

	if (CurrentState & ExtendedPrecision)
	{
		SetFPUState(CurrentState & ~ExtendedPrecision);
	}
	CurrentState = GetFPUState();
}

// ---------------------------------------------------------------------------------------------

void SetPreciseFPU()
{
	int32 CurrentState = GetFPUState();

	if ((CurrentState & ExtendedPrecision) != ExtendedPrecision)
	{
		SetFPUState(CurrentState | ExtendedPrecision);
	}
	CurrentState = GetFPUState();
}
}

#else

// ---------------------------------------------------------------------------------------------
//  Unknown Architecture
// ---------------------------------------------------------------------------------------------

const SCpuInfo &GetCpuInfo()
{
	if (g_Cpu.Processors == 0)
	{
		g_Cpu.Type = CpuTypeUnknown;
		g_Cpu.Processors = GetNumPhysicalCpus();
		g_Cpu.Cores = 1;
		g_Cpu.LogicalUnits = 1;

		g_Cpu.SIMD = NoSIMD;
		g_Cpu.Popcnt = 0;
	}

	return g_Cpu;
}
	/**/
	// ---------------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------------------

	//cout_<<"количество процессорных ядер в системе ++  num_cores="<<GetNumPhysicalCpus()<<endl;

#endif

////CB_SUBNAMESPACE_END

//******************************************************************************************************

// TEST //

// текст для кнопок //
// для любых процессоров, код на "C32".
// для процессоров Intel с SSE3, код на "ASM". // временно, пока SSE3 не переписано на intrinsic-функции (интринсики), код на "C64"
// для процессоров Intel с SSE4.2 с командой POPCNT, код на "ASM". // POPCNT, код на "intrinsic-функции (интринсики)" "C64"
// For all processors, the code in "C32".
// For Intel processors with SSE3, code for "ASM".
// For Intel processors with SSE4.2, with the team POPCNT, code for "ASM".

/*
 void EnableCRTSSE() {}
 void DisableCRTSSE() {}
 */

/*
 namespace x86
 {
 void FlushMMXRegister() {}
 void SetFastFPU() {}
 void SetPreciseFPU() {}
 void EnableCRTSSE() {}
 void DisableCRTSSE() {}
 }
 */
/*
 if( Intel==1 ){
 } // if( Intel==1 )
 
 if( AMD==2 ){ // уже выполняется
 if( g_Cpu.SIMD==MMX ) {   }                                // 1. только MMX
 } //if( AMD==2 )
 
 ///if( g_Cpu.SIMD==NoSIMD ) {   }                                                             // 1. CPU32
 */

// ---------------------------------------------------------------------------------------------
/*
 // Проверка функциониования инструкции cpuid. Check = true инструкция работает
 static inline bool IsCpuIDSupported()
 {
 bool Check = true;  // false
 
 //    __asm(
 //     ".lcomm buffer, 13\t\n"
 
 //     "movl $0, %eax\t\n"		//# define cpuid output option
 //     "cpuid\t\n"			
 
 //     "movl $buffer, %edi\t\n"	//# put values in string
 //     "movl %ebx, 0(%edi)\t\n"
 //     "movl %edx, 4(%edi)\t\n"
 //     "movl %ecx, 8(%edi)\t\n"
 //     "movl $0, %ecx\t\n"
 //     "movl %ecx, 12(%edi)"
 
 );
 
 
 CB_ASM{
 
 // pushfd           //; размещение регистра EFLAGS в стеке
 // pop eax          //; извлечение значения EFLAGS в EAX
 //mov 1, eax; 
 //cpuid;
 
 mov ebx, eax     //; сохранение значения в EBX
 xor eax, 200000h //; изменение 21-го бита
 push eax         //; размещение нового значения в стеке
 popfd            //; сохранение нового значения в EFLAGS
 pushfd           //; снова размещение EFLAGS в стеке
 pop eax          //; значение EFLAGS теперь в EAX
 xor eax, ebx     //; проверка 21-го бита
 //je no_cpuid      //; если он не изменился, то CPUID не поддерживается
 
 // CB_ASM_pushfl;		// push EFLAGS to stack
 // pop eax;			// EFLAGS -> eax
 mov ebx, eax;		// eax -> ebx (for compare)
 xor eax, 0x200000;	// modify cpuid bit
 push eax;			// push eax to stack
 CB_ASM_popfl;		// commit EFLAGS
 CB_ASM_pushfl;		// push EFLAGS to stack again
 pop eax;			// EFLAGS -> eax
 cmp eax, ebx;		// compare original and modified EFLAGS
 jz no_cpuid;
 mov Check, 1;		// if modification was kept -> supported
 no_cpuid:
 
 
 }
 
 return Check;
 };
 */

/// SCpuInfo a_=GetCpuInfo();   inputData.CPUInfo=&a_;
///     cout_<<"CpuType="<<a_.Type<<"  SIMDType="<<a_.SIMD<<"  Processors="<<a_.Processors<<"  Cores="<<a_.Cores<<"  LogicalUnits="<<a_.LogicalUnits<<"  CoresSysconf="<<a_.CoresSysconf<<"  Popcnt="<<a_.Popcnt<<"  LinuxKernel="<<a_.LinuxKernel<<endl;

// -------------------------------------------------------------------------------------------------
