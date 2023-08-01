//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EOS.h
//
//  Subsystem  :  Kernel
//
//  Description:  Classes for OS platform portablility
//
//  Classes:    EMutex - Mutual Exclusion, protects a resource from multi-threads
//              ELock - Temporary object that locks a Mutex and unlocks when it goes out of scope
//              ESingleEvent - Used to signal a waiting thread of some event
//              EThread - Object that runs a thread and allows waiting on its termination
//
//**************************************************************************

#ifndef __EOS_H__
#define __EOS_H__

#ifdef WIN32
    #ifdef _DV_EXPORT
        #define _DV_DECLSPEC __declspec(dllexport)
        #define _DV_EXTERN
    #else
        #define _DV_DECLSPEC __declspec(dllimport)
        #define _DV_EXTERN   extern
    #endif
#else
    #define _DV_DECLSPEC
    #define _DV_EXTERN
#endif

class ECircularBuffer;

#ifdef UNDER_VXWORKS
    #ifdef __stdcall
        #undef __stdcall
    #endif

	#define __stdcall

	extern "C"
#endif

// UNICODE files start with this mark
#define UNICODE_BOM     0XFEFF

/* Converts a WCHAR to a char. */
_DV_DECLSPEC char* WCharToChar(char* pDest, const WCHAR* pSource);

//Converts a char to a WCHAR.
_DV_DECLSPEC WCHAR* CharToWChar(WCHAR* pDest, const char* pSource);

//Converts a WCHAR to a char minding receiver buffer size.
_DV_DECLSPEC char* NWCharToChar(char* pDest, const WCHAR* pSource, DWORD iMaxSize);

//Converts a char to a WCHAR minding receiver buffer size.
_DV_DECLSPEC WCHAR* NCharToWChar(WCHAR* pDest, const char* pSource, DWORD iMaxSize);

// Suspends the current thread for the given amount of time.
_DV_DECLSPEC void ESleep(DWORD dwMiliSeconds);

// Runs a program.
_DV_DECLSPEC bool Execute(TCHAR* appName, TCHAR* params, bool bWait);

// Runs a program with CWD set to specified directory.
_DV_DECLSPEC bool ExecuteInDir(TCHAR* appName, TCHAR* params, TCHAR* cwd, bool bWait);

// Runs a program with CWD set to the current directory.
_DV_DECLSPEC bool ExecuteInCurrentDir(TCHAR* appName, TCHAR* params, bool bWait);

// Get CPU Type.  Be as generic as possible.  For example:  x86 vs i586, i686 ...
_DV_DECLSPEC TCHAR* GetCPUType();

// Get OS.  Be generic as possible.  For example WIN32 vs WinNT, Win2000 ...
_DV_DECLSPEC TCHAR* GetOSType();

// Get CPU ID.  WIN32 and LINUX only.
_DV_DECLSPEC bool GetCPUID(DWORD (&cpuInfo)[4], DWORD infoType);
_DV_DECLSPEC bool GetFullCPUID(BYTE* pCpuid, DWORD* pSize);

_DV_DECLSPEC bool GetMACAddress(UCHAR (&macAddr)[6]);

// utility function to allow secure zero of memory for sensative variables that may contain passwords, ect
_DV_DECLSPEC void memzero(void *block, size_t size);
// Global memory available function.
// Returns 1 if sucessful, 0 if failed (OS doesn not support).
typedef struct _EMemoryStatus
{
    ULONG_PTR dwTotalPhys;
    ULONG_PTR dwAvailPhys;
    ULONG_PTR dwTotalVirtual;
    ULONG_PTR dwAvailVirtual;
} EMemoryStatus;

_DV_DECLSPEC bool EGetMemoryStatus(EMemoryStatus* pEMemoryStatus);

interface IKernelLog;       // Forward reference

// group=String Conversions

//**************************************************************************
//When using a conversion macro, specify the CONVERSION_HELPER macro at the
//beginning of your function in order to avoid compiler errors.
#define MAX_CONVERSION_STR_LEN   260
#define CONVERSION_HELPER char LOCALascii[MAX_CONVERSION_STR_LEN]={0}; WCHAR LOCALunicode[MAX_CONVERSION_STR_LEN]={0};
#ifdef UNICODE
    #define AtoT(X)  CharToWChar(LOCALunicode,(X))   // ASCII to TCHAR
    #define WtoT(X)  (X)                             // Wide to TCHAR
    #define TtoA(X)  WCharToChar(LOCALascii,(X))     // TCHAR to ASCII
    #define TtoW(X)  (X)                             // TCHAR to Wide

	#if ! defined(WIN32)&& ! defined(UNDER_CE)
		int _em_fwprintf(FILE* stream, const wchar_t* format, ...);
		int _em_swprintf(wchar_t* wcs, size_t size, const wchar_t* format, ...);
		int _em_vswprintf(wchar_t* wcs, size_t size, const wchar_t* format, va_list args);
	#endif

#else   // Unicode
    #define AtoT(X)  (X)
    #define WtoT(X)  WCharToChar(LOCALascii,(X))
    #define TtoA(X)  (X)
    #define TtoW(X)  CharToWChar(LOCALunicode,(X))
#endif  // Unicode

#if defined(WIN32) || defined(UNDER_CE)
	#include "EOS_win32.h"
#endif   // WIN32

#ifdef UNDER_VXWORKS
	#include "EOS_vxworks.h"
#endif // UNDER_VXWORKS

#ifdef _THREADX_
	#include "EOS_threadx.h"
#endif // _THREAD_X

#ifdef UNIX
	#include "EOS_unix.h"
#endif // UNIX

// group=OS Wrappers

/**************************************************************************
Object that locks a Mutex and unlocks when it goes out of scope.
Mutexes are sometimes left unlocked when a function contains
multiple return paths.

        For example,

        ELock lock(&someMutex);

            if (problem)

                return;     // Mutex will be unlocked here!

            do some work...

        return;
**************************************************************************/
class _DV_DECLSPEC ELock
{
public:
   ELock(EMutex& mutex,TCHAR *pCaller = _T("Unknown"));
   ~ELock();

private:
    EMutex& m_mutex;

#ifdef DEBUG_DEADLOCK
    TCHAR   tCaller[100];
#endif
};

// Integer overflow and wraparound checking
// functions.
//////////////////////////////////////////////////
#define OVERFLOW_CHECK 1
#ifdef OVERFLOW_CHECK
#ifdef RSIZE_MAX
#define ALLOC_SIZE_MAX RSIZE_MAX
#else
#define rsize_t size_t
#define ALLOC_SIZE_MAX ((size_t)-1)
#endif

// Used for signed integer overflow checking
template <typename T>
bool ADDITION_OVERFLOWS(T a, T b, T _min, T _max)
{
	return (((b > 0) && (a > (_max - b))) ||
			((b < 0) && (a < (_min - b))));
};

template <typename T>
bool SUBTRACT_OVERFLOWS(T a, T b, T _min, T _max)
{
	return (((b > 0 && a < (_min + b)) ||
			(b < 0 && a > (_max + b))));
};

template <typename T>
bool MULTIPLY_OVERFLOWS(T a, T b, T _min, T _max)
{
  if (a > 0) {  /* a is positive */
    if (b > 0) {  /* a and b are positive */
      if (a > (_max / b)) {
		  return true; // Overflows
      }
    } else { /* a positive, b nonpositive */
      if (b < (_min / a)) {
		  return true; // Overflows
      }
    } /* a positive, b nonpositive */
  } else { /* a is nonpositive */
    if (b > 0) { /* a is nonpositive, b is positive */
      if (a < (_min / b)) {
		  return true;	// Overflows
      }
    } else { /* a and b are nonpositive */
      if ( (a != 0) && (b < (_max / a))) {
		  return true;  // Overflows
      }
    } 
  } 
  return false;		// No Overflows
};

template <typename T>
bool DIVISION_OVERFLOWS(T a, T b, T _min, T _max)
{
	return (b == 0 || ((a == _min) && (b == -1)));
};

// Unsed for unsigned integer 
inline bool ADDITION_WRAPSAROUND(size_t a, size_t b)
{
	return (ALLOC_SIZE_MAX - a < b);
};

inline bool SUBTRACT_WRAPSAROUND(size_t a, size_t b)
{
	return (a < b);
};

inline bool MULTIPLY_WRAPSAROUND(size_t a, size_t b)
{
	return b != 0 && a > (ALLOC_SIZE_MAX/b);
};
#else
// If overflow checking is disabled, all check functions will return
// false (i.e. no overflow)
#define ADDITION_OVERFLOWS(a,b,min,max) 0 // false
#define SUBTRACT_OVERFLOWS(a,b,min,max) 0 // false
#define MULTIPLY_OVERFLOWS(a,b,min,max) 0 // false
#define DIVISION_OVERFLOWS(a,b,min,max) 0 // false
#define ADDITION_WRAPSAROUND(a,b) 0	      // false
#define SUBTRACT_WRAPSAROUND(a,b) 0		  // false
#define MULTIPLY_WRAPSAROUND(a,b) 0       // false
#endif

// Re-initialise resolver.
_DV_DECLSPEC int ResInit(void);

#endif // __EOS_H__
