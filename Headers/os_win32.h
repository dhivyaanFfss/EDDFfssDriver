//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  os_win32.h
//  
//  Subsystem  :  Kernel/Base
//
//  Description:  os_win32 platform supports
//
//
//**************************************************************************

#ifndef _OS_WIN32_H
#define _OS_WIN32_H


#if defined(WIN32) || defined(UNDER_CE)

    /* Architecture Byte Ordering - Endian-ness */

    /* BYTE_SWAPPED == 1 means "Intel x86" type byte ordering
     * BYTE_SWAPPED == 0 means non-Intel (typically called "Motorola") ordering
     */
    #if defined(PPC)
	    #define BYTE_SWAPPED 0
    #elif defined(ARM)
	    #define BYTE_SWAPPED 1
    #else
	    #define BYTE_SWAPPED 1  // x86
    #endif


    #if defined(UNDER_CE) && !defined(UNICODE)
        #define UNICODE
    #endif 

    #ifdef UNICODE
        #ifndef _UNICODE
            #define _UNICODE
        #endif
    #endif

    #if(_MSC_VER >= 1300)
        #if !defined(_USE_32BIT_TIME_T) && !defined(_WIN64)
            #define _USE_32BIT_TIME_T
        #endif
    #endif

    #ifndef UNDER_CE
        #if defined _DEBUG && !defined _IGNORE_CONNECTOR_MEMORY_MANAGER_
           #define _CRTDBG_MAP_ALLOC
        #endif

        #if(_MSC_VER < 1300)
            #define _WIN32_WINNT 0x0400
            // jdp define _WSPIAPI_COUNTOF before including header to work around PlatformSDK 
            // incompatibility with VC6
            #define _WSPIAPI_COUNTOF 
            #include <windows.h>
            #include <ws2tcpip.h>
        #else
            #include <winsock2.h>
            #include <ws2tcpip.h>
            #include <windows.h>
        #endif
    #else
        #include <windows.h>
        #include <winsock2.h>
        #include <ws2tcpip.h>
        #include <wincertlib.h>
    #endif

    #include <TCHAR.h>
    #include <errno.h>
    #include <time.h>
    #include <process.h>
    #include <stdio.h>
    #include <locale.h>
    #include <limits.h>

    #ifdef UNDER_CE
        #undef ASSERT 
        #define ASSERT(X)
    #else        
        #include <crtdbg.h>
        #if !defined ASSERT
            #define ASSERT(X) _ASSERTE(X)
        #endif
    #endif // UNDER_CE

    #ifdef _DEBUG
		#define OUTPUTDEBUGSTRING(X)  {OutputDebugString((X)); fflush(stderr);}
    #else
        #define OUTPUTDEBUGSTRING(X)
    #endif  /* _DEBUG */

    typedef int socklen_t;

    #ifdef _tcstok
        #undef _tcstok
    #endif
    #define _tcstok(X,Y)   <-- disallowed, not thread safe, use ECSTOK-->
    #ifdef UNICODE
        #define ECSTOK(a,b,c)   wcstok(a,b)    /* FIX! not thread safe */
    #else
        #define ECSTOK(a,b,c)   strtok(a,b)    /* FIX! not thread safe */
    #endif

    #define vsnprintf _vsnprintf

    #ifdef UNDER_CE
        #define snprintf _snprintf
        #define strdup _strdup
		#ifndef _tcscpy_s 
            #define _tcscpy_s(dst, size, src)	_tcscpy(dst, src)
        #endif

        #ifndef memcpy_s 
            #define memcpy_s(dst,dstSize,src,srcSize)   memcpy(dst, src, srcSize)
        #endif    

        #ifndef memmove_s
            #define memmove_s(dst,dstSize,src,srcSize)  memmove(dst, src, srcSize)
        #endif
    #endif

    #ifdef _WIN64
        typedef unsigned __int64 EHANDLE;
    #else
        typedef unsigned long EHANDLE;
    #endif

    // ELONG32 and EULONG32 should be used to replace signed/unsigned long in
    // 32-bit integer declarations. The replacement makes such declarations safe
    // on targets with LP64 model.
    typedef long ELONG32;
    typedef unsigned long EULONG32;

#endif		// defined (WIN32) 
#endif		// _OS_WIN32_H
