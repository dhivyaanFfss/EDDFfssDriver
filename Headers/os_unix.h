//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  os_unix.h
//  
//  Subsystem  :  Kernel
//
//  Description:  declarations for unix-like platforms
//
//
//**************************************************************************

#ifndef _OS_UNIX_H
#define _OS_UNIX_H

    #define UNIX

/* Architecture Byte Ordering - Endian-ness */

/* BYTE_SWAPPED == 1 means "Intel x86" type byte ordering
 * BYTE_SWAPPED == 0 means non-Intel (typically called "Motorola") ordering
 */
    #if defined(PPC) || defined(ARM) || defined(sparc) || defined(MIPS)
        #define BYTE_SWAPPED 0
    #else
        #define BYTE_SWAPPED 1  // x86
    #endif

/*
    #if defined(MEM_DEBUG) && !defined(PAGERLIB_EXPORTS)
        void operator delete( void * p );
        void operator delete[]( void * p );
        void * operator new( unsigned int cb, char* pszFile, unsigned int line);
        void * operator new[]( unsigned int cb, char* pszFile, unsigned int line);
        #define DEBUG_NEW new(__FILE__, __LINE__)
    	    #define new DEBUG_NEW
    #endif		// MEM_DEBUG
*/
    #define _NORMAL_BLOCK 1
    #ifndef __cdecl
        #define __cdecl
    #endif

    #include <unistd.h>
    #include <fcntl.h>
    #include <netdb.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <time.h>
    #include <ctype.h>
    #include <string.h>
    #include <stdarg.h>
    #include <dlfcn.h>
    #include <dirent.h>
    #include <termios.h>
    #include <signal.h>
    #include <pthread.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <sys/ioctl.h>
    #include <sys/time.h>
    #include <sys/times.h>	
    #include <sys/timeb.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/sem.h>
    #include <sys/sysmacros.h>	
    #include <sys/param.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <arpa/inet.h>
    #include <net/if.h>
    #include <limits.h>
    #include <ifaddrs.h>
    #include <linux/if_packet.h>
    #include <resolv.h>	
    
    #ifdef SOLARIS
        #include <sys/filio.h>
        #include <sys/sockio.h>
        #include <ieeefp.h>
    #endif

    #define __USE_GNU 1    
    #define __USE_ISOC99 1

    #ifndef INVALID_SOCKET
        #define INVALID_SOCKET (-1)
    #endif

    #ifndef SOCKET_ERROR
       #define SOCKET_ERROR (-1)
    #endif

    #ifndef min
        #define min(X, Y) ((X)>(Y)? (Y) : (X))
    #endif

    #ifndef max
        #define max(X, Y) ((X)>(Y)? (X) : (Y))
    #endif

    /* Windows _s safe function call mapping */
    #ifndef memcpy_s
        #define memcpy_s(dst,dstSize,src,srcSize)   memcpy(dst, src, srcSize)
    #endif    

    #ifndef memmove_s
        #define memmove_s(dst,dstSize,src,srcSize)  memmove(dst, src, srcSize)
    #endif

        typedef int     INT;
        typedef short   SHORT;
        typedef long    LONG;
        typedef float   FLOAT;
        typedef double  DOUBLE;
        typedef int     SOCKET;
        typedef char*	LPSTR;
        typedef wchar_t WCHAR;
        typedef WCHAR* LPWSTR;
        typedef WCHAR*	BSTR;
        typedef const WCHAR* LPCWSTR;
        typedef const char*  LPCSTR;
    typedef long long		LONGLONG;
    typedef unsigned short	USHORT;
    typedef unsigned long	ULONG;   
    typedef unsigned int	UINT; 
    typedef unsigned char	BYTE;
    typedef unsigned char	UCHAR;	
    typedef unsigned long long ULONGLONG;

    // INT_PTR and UINT_PTR should be used to replace signed/unsigned int in
    // declaration of integers that store pointers.
    #if defined(_LP64) || defined(__LP64__)
        typedef long INT_PTR;
        typedef unsigned long UINT_PTR;
    #else
        typedef int INT_PTR;
        typedef unsigned int UINT_PTR;
    #endif

    // LONG_PTR and ULONG_PTR should be used to replace signed/unsigned long in
    // declaration of integers that store pointers.
    typedef long LONG_PTR;
    typedef unsigned long ULONG_PTR;

    // ELONG32 and EULONG32 should be used to replace signed/unsigned long in
    // 32-bit integer declarations. The replacement makes such declarations safe
    // on targets with LP64 model.
    #if defined(_LP64) || defined(__LP64__)
        typedef int ELONG32;
        typedef unsigned int EULONG32;
    #else
        typedef long ELONG32;
        typedef unsigned long EULONG32;
    #endif

    typedef ELONG32	  BOOL;
    typedef ELONG32	  HINSTANCE;
    typedef EULONG32  HANDLE;
    typedef EULONG32  DWORD;
    typedef DWORD     HRESULT;
    typedef ULONG_PTR EHANDLE;

    #define INFINITE 0xFFFFFFFF
    #define MAX_PATH 256
    #define WAIT_OBJECT_0 0x00000000L
    #define WAIT_TIMEOUT  0x00000102L
    #define WAIT_FAILED   0xFFFFFFFF
    #define __stdcall
    #define WINAPI
    #define PASCAL
    #define TRUE  1
    #define FALSE 0
    #define _DV_DECLSPEC
    #define _DV_EXTERN

    typedef unsigned short WORD;
    typedef BOOL VARIANT_BOOL;
    typedef DWORD SCODE;

    #define VARIANT_TRUE -1
    #define VARIANT_FALSE 0
    /*
     Generic test for success on any status value (non-negative numbers
     indicate success).

     Variant Return types */
    #define S_OK                                   ((HRESULT)0x00000000L)
    #define S_FALSE                                ((HRESULT)0x00000001L)

    typedef struct _MEMORYSTATUS {
        DWORD dwLength;
        DWORD dwMemoryLoad;
        DWORD dwTotalPhys;
        DWORD dwAvailPhys;
        DWORD dwTotalPageFile;
        DWORD dwAvailPageFile;
        DWORD dwTotalVirtual;
        DWORD dwAvailVirtual;
    } MEMORYSTATUS, *LPMEMORYSTATUS;

    #ifndef _FSIZE_T_DEFINED
        typedef unsigned long _fsize_t;
        #define _FSIZE_T_DEFINED
    #endif

    #ifndef _WFINDDATA_T_DEFINED
        struct _wfinddata_t {
            unsigned int attrib;
            time_t time_create;
            time_t time_access;
            time_t time_write;
            _fsize_t size;
            wchar_t name[260];
        };

    #define FILE_ATTRIBUTE_DIRECTORY   0x00000010
    #define FILE_ATTRIBUTE_ARCHIVE     0x00000020

    #define _FINDDATA_T_DEFINED
    #endif

//    typedef struct _SYSTEMTIME
//    {
//        WORD wYear;
//        WORD wMonth;
//        WORD wDayOfWeek;
//        WORD wDay;
//        WORD wHour;
//        WORD wMinute;
//        WORD wSecond;
//        WORD wMilliseconds;
//   } SYSTEMTIME;

//    typedef struct _FILETIME
//    {
//        DWORD dwLowDateTime;   /* low 32 bits  */
//        DWORD dwHighDateTime;  /* high 32 bits */
//    } FILETIME, *PFILETIME, *LPFILETIME;


    #define _snprintf snprintf


    /* floating point number functions */
    #define _isnan  isnan
    #define _finite finite

    #ifdef LINUX
        #define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
    #endif

    #ifdef SOLARIS
    #ifdef  __cplusplus
    extern "C"
    {
	#endif
        int setenv(const char *name, const char *value, int overwrite);
        int unsetenv(const char *name);
    #ifdef  __cplusplus
    }
	#endif
    #endif

    #ifdef  UNICODE
    /* ++++++++++++++++++++ UNICODE ++++++++++++++++++++ */

        #include <wchar.h>
        #include <wctype.h>
        #ifdef SOLARIS
            #include <widec.h>
        #endif


        #ifdef  __cplusplus
            extern "C" {
        #endif

        #ifndef _TCHAR_DEFINED
            typedef WCHAR TCHAR;
            typedef TCHAR* LPTSTR;
            typedef const TCHAR* LPCTSTR;
            #define _TCHAR_DEFINED
        #endif

        #define _TEOF       WEOF

        #define _T(x)      L ## x


        #if defined(SOLARIS) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
            wchar_t *wsdup(const wchar_t *);
            int wscasecmp(const wchar_t *, const wchar_t *);
            int wsncasecmp(const wchar_t *, const wchar_t *, size_t);
            long long wstoll(const wchar_t *, wchar_t **, int);
        #endif

        /* Formatted I/O */
        int _em_fwprintf(FILE* stream, const wchar_t* format, ...);
        int _em_swprintf(wchar_t* wcs, size_t size, const wchar_t* format, ...);
        int _em_vswprintf(wchar_t* wcs, size_t size, const wchar_t* format, va_list args);
        FILE* _em_wfopen(const wchar_t* filename, const wchar_t* filemode);

        #define _tprintf    <--disallowed, use _ftprintf(stdout,)-->
        #define _ftprintf   _em_fwprintf
        #define _stprintf   <--disallowed, use _sntprintf-->
        #define _sntprintf  _em_swprintf
        #define _vtprintf   <--disallowed, implement-->
        #define _vftprintf  <--disallowed, implement-->
        #define _vstprintf  <--disallowed, use _vsntprintf-->
        #define _vsntprintf _em_vswprintf
        #define _vsntprintf_s(buf, bufSize, count, format, args)    _em_vswprintf(buf,count,format,args)
        #define _tscanf     wscanf
        #define _ftscanf    fwscanf
        #define _stscanf    swscanf


        /* Unformatted I/O */

        #define _fgettc     fgetwc
        #define _fgettchar  _fgetwchar
        #define _fgetts     fgetws
        #define _fputtc     fputwc
        #define _fputtchar  _fputwchar
        #define _fputts     fputws
        #define _gettc      getwc
        #define _gettchar   getwchar
        #define _getts      _getws
        #define _puttc      putwc
        #define _puttchar   putwchar
        #define _putts      _putws
        #define _ungettc    ungetwc


        /* String conversion functions */

        #define _tcstod     wcstod
        #define _tcstol     wcstol
        #define _tcstoul    wcstoul

        #ifdef SOLARIS
            #define _ttoi64 watoll
        #else
            #define _ttoi64(s)  wcstoll(s, NULL, 10)
        #endif

        /* String functions */

        #define _tcscat     wcscat
        #define _tcschr     wcschr
        #define _tcscpy     wcscpy
        #define _tcscspn    wcscspn
        #define _tcslen     wcslen
        #define _tcsncat    wcsncat
        #define _tcsncpy    wcsncpy
        #define _tcspbrk    wcspbrk
        #define _tcsrchr    wcsrchr
        #define _tcsspn     wcsspn
        #define _tcsstr     wcsstr
        #define _tcstok(X,Y)   <-- disallowed, not thread safe, use ECSTOK-->
        #define ECSTOK(a,b,c) wcstok(a,b, c)

        #define _tcscpy_s(dest, size, src)  _tcscpy(dest, src)

        #ifdef SOLARIS
            #define _wcsdup     wsdup
            #define _tcsdup     wsdup
        #else
            #define _wcsdup     wcsdup
            #define _tcsdup     wcsdup
        #endif

        #define _tcsnset    _wcsnset
        #define _tcsrev     _wcsrev
        #define _tcsset     _wcsset

        #define _tcscmp     wcscmp
        #define _tcsncmp    wcsncmp
        #ifdef SOLARIS
            #define _tcsicmp    wscasecmp
            #define _tcsnicmp    wsncasecmp
        #else
            #define _tcsicmp    wcscasecmp
            #define _tcsnicmp    wcsncasecmp
        #endif

        #define _tcscoll    wcscoll
        #define _tcsicoll   _wcsicoll
        #define _tcsnccoll  _wcsncoll
        #define _tcsncoll   _wcsncoll
        #define _tcsncicoll _wcsnicoll
        #define _tcsnicoll  _wcsnicoll


        /* Time functions */

        #define _tasctime   _wasctime
        #define _tctime     _wctime
        #define _tstrdate   _wstrdate
        #define _tstrtime   _wstrtime
        #define _tutime     _wutime
        #define _tcsftime   wcsftime


        /* Directory functions */

        #define _tchdir     _wchdir
        #define _tgetcwd    _wgetcwd
        #define _tgetdcwd   _wgetdcwd
        #define _tmkdir     _wmkdir
        #define _trmdir     _wrmdir


        /* Stdio functions */

        #define _tfdopen    _wfdopen
        #define _tfsopen    _wfsopen
        #define _tfopen     _em_wfopen
           
        #define _tfreopen   _wfreopen
        #define _tperror    _wperror
        #define _tpopen     _wpopen
        #define _ttempnam   _wtempnam
        #define _ttmpnam    _wtmpnam


        /* Io functions */

        #define _taccess    _waccess
        #define _tchmod     _wchmod
        #define _tcreat     _wcreat
        #define _tfindfirst _wfindfirst
        #define _tfindfirsti64  _wfindfirsti64
        #define _tfindnext  _wfindnext
        #define _tfindnexti64   _wfindnexti64
        #define _tmktemp    _wmktemp
        #define _topen      _wopen
        #define _tremove    _wremove
        #define _trename    _wrename
        #define _tsopen     _wsopen
        #define _tunlink    _wunlink

        #define _tfinddata_t    _wfinddata_t
        #define _tfinddatai64_t _wfinddatai64_t


        /* Setlocale functions */

        #define _tsetlocale _wsetlocale

        /* Redundant "logical-character" mappings */
        TCHAR *_tcsinc( const TCHAR *current );

        #define _tcsclen    wcslen
        #define _tcsnccat   wcsncat
        #define _tcsnccpy   wcsncpy
        #define _tcsncset   _wcsnset

        #define _tcsdec     _wcsdec
        #define _tcsinc     _wcsinc
        #define _tcsnbcnt   _wcsncnt
        #define _tcsnccnt   _wcsncnt
        #define _tcsnextc   _wcsnextc
        #define _tcsninc    _wcsninc
        #define _tcsspnp    _wcsspnp

        #define _tcslwr     _wcslwr
        #define _tcsupr     _wcsupr
        #define _tcsxfrm    wcsxfrm


        /* ctype functions */

        #define _istalnum   iswalnum
        #define _istalpha   iswalpha
        #define _istascii   iswascii
        #define _istcntrl   iswcntrl
        #define _istdigit   iswdigit
        #define _istgraph   iswgraph
        #define _istlower   iswlower
        #define _istprint   iswprint
        #define _istpunct   iswpunct
        #define _istspace   iswspace
        #define _istupper   iswupper
        #define _istxdigit  iswxdigit

        #define _totupper   towupper
        #define _totlower   towlower

        #define _istlegal(_c)   (1)
        #define _istlead(_c)    (0)
        #define _istleadbyte(_c)    (0)

    #else   /* ndef UNICODE */

        /* ++++++++++++++++++++ SBCS and MBCS ++++++++++++++++++++ */

        #ifdef  __cplusplus
            extern "C" {
        #endif

        #ifndef _TCHAR_DEFINED
            typedef char TCHAR;
            typedef TCHAR* LPTSTR;
            typedef const TCHAR* LPCTSTR;
            #define _TCHAR_DEFINED
        #endif

        #define _TEOF       EOF

        #define _T(x)      x


        /* Formatted I/O */

        #define _tprintf    printf
        #define _ftprintf   fprintf
        #define _stprintf   sprintf
        #define _sntprintf  _snprintf
        #define _vtprintf   vprintf
        #define _vftprintf  vfprintf
        #define _vstprintf  vsprintf
        #define _vsntprintf _vsnprintf
        #define _tscanf     scanf
        #define _ftscanf    fscanf
        #define _stscanf    sscanf


        /* Unformatted I/O */

        #define _fgettc     fgetc
        #define _fgettchar  _fgetchar
        #define _fgetts     fgets
        #define _fputtc     fputc
        #define _fputtchar  _fputchar
        #define _fputts     fputs
        #define _gettc      getc
        #define _gettchar   getchar
        #define _getts      gets
        #define _puttc      putc
        #define _puttchar   putchar
        #define _putts      puts
        #define _ungettc    ungetc


        /* String conversion functions */

        #define _tcstod     strtod
        #define _tcstol     strtol
        #define _tcstoul    strtoul
        #define _ttoi       atoi
        #define _ttol       atol


        /* String functions */

        /* Note that _mbscat, _mbscpy and _mbsdup are functionally equivalent to 
           strcat, strcpy and strdup, respectively. */

        #define _tcscat     strcat
        #define _tcscpy     strcpy

        #define _tcsdup     strdup

        #define _tcslen     strlen
        #define _tcsxfrm    strxfrm

        #ifdef __QNX__
            WCHAR* wcsdup(WCHAR* wsToDuplicate);
            WCHAR* wcscpy(WCHAR* wsCopyTo, const WCHAR* wsSource);
            size_t wcslen(const WCHAR* ws);
            int wcscmp(const WCHAR* ws1, const WCHAR* ws2);
            char* gcvt(double dValue, int nDec, char* caBuf);
        #endif


        /* Time functions */

        #define _tasctime   asctime
        #define _tctime     ctime
        #define _tstrdate   _strdate
        #define _tstrtime   _strtime
        #define _tutime     _utime
        #define _tcsftime   strftime


        /* Directory functions */

        #define _tchdir     _chdir
        #define _tgetcwd    _getcwd
        #define _tgetdcwd   _getdcwd
        #define _tmkdir     _mkdir
        #define _trmdir     _rmdir


        /* Stdio functions */

        #ifdef  _POSIX_
            #define _tfdopen    fdopen
        #else
            #define _tfdopen    _fdopen
        #endif

        #define _tfsopen    _fsopen
        #define _tfopen     fopen
        #define _tfreopen   freopen
        #define _tperror    perror
        #define _tpopen     _popen
        #define _ttempnam   _tempnam
        #define _ttmpnam    tmpnam


        /* I/O functions */

        #define _tchmod     _chmod
        #define _tcreat     _creat
        #define _tfindfirst _findfirst
        #define _tfindfirsti64  _findfirsti64
        #define _tfindnext  _findnext
        #define _tfindnexti64   _findnexti64
        #define _tmktemp    _mktemp

        #ifdef  _POSIX_
            #define _topen      open
            #define _taccess    access
        #else
            #define _topen      _open
            #define _taccess    _access
        #endif

        #define _tremove    remove
        #define _trename    rename
        #define _tsopen     _sopen
        #define _tunlink    _unlink

        #define _tfinddata_t    _finddata_t
        #define _tfinddatai64_t _finddatai64_t


        /* ctype functions */

        #define _istascii   isascii
        #define _istcntrl   iscntrl
        #define _istxdigit  isxdigit


        /* Stat functions */

        #define _tstat      _stat
        #define _tstati64   _stati64


        /* Setlocale functions */

        #define _tsetlocale setlocale


        /* ++++++++++++++++++++ SBCS ++++++++++++++++++++ */


        #ifndef __TCHAR_DEFINED
            typedef char            _TCHAR;
            typedef signed char     _TSCHAR;
            typedef unsigned char   _TUCHAR;
            typedef char            _TXCHAR;
            typedef int             _TINT;
            #define __TCHAR_DEFINED
        #endif

        #ifndef _TCHAR_DEFINED
            typedef char            TCHAR;
            #define _TCHAR_DEFINED
        #endif


        /* String functions */

        #define _tcschr     strchr
        #define _tcscspn    strcspn
        #define _tcsncat    strncat
        #define _tcsncpy    strncpy
        #define _tcspbrk    strpbrk
        #define _tcsrchr    strrchr
        #define _tcsspn     strspn
        #define _tcsstr     strstr
        #define _tcstok(X,Y)   <-- disallowed, not thread safe, use ECSTOK-->
        #define ECSTOK(a,b,c) wcstok(a,b)

        #define _tcsnset    _strnset
        #define _tcsrev     _strrev
        #define _tcsset     _strset

        #define _tcscmp     strcmp
        #define _tcsicmp    strcasecmp

        #ifdef __QNX__
            #define _tcsicmp    _stricmp
        #endif

        #define _tcsnccmp   strncmp
        #define _tcsncmp    strncmp
        #define _tcsncicmp  _strnicmp
        #define _tcsnicmp   _strnicmp

        #define _tcscoll    strcoll
        #define _tcsicoll   _stricoll
        #define _tcsnccoll  _strncoll
        #define _tcsncoll   _strncoll
        #define _tcsncicoll _strnicoll
        #define _tcsnicoll  _strnicoll


        /* "logical-character" mappings */

        #define _tcsclen    strlen
        #define _tcsnccat   strncat
        #define _tcsnccpy   strncpy
        #define _tcsncset   _strnset


        /* MBCS-specific functions */

        #define _tcsdec     _strdec
        #define _tcsinc     _strinc
        #define _tcsnbcnt   _strncnt
        #define _tcsnccnt   _strncnt
        #define _tcsnextc   _strnextc
        #define _tcsninc    _strninc
        #define _tcsspnp    _strspnp

        /*#define _tcslwr     _strlwr */
        /*#define _tcsupr     _strupr */
        #define _tcsxfrm    strxfrm

        #define _istlead(_c)    (0)
        #define _istleadbyte(_c)    (0)


        /* ctype-functions */

        #define _istalnum   isalnum
        #define _istalpha   isalpha
        #define _istdigit   isdigit
        #define _istgraph   isgraph
        #define _istlower   islower
        #define _istprint   isprint
        #define _istpunct   ispunct
        #define _istspace   isspace
        #define _istupper   isupper

        #define _totupper   toupper
        #define _totlower   tolower

        #define _istlegal(_c)   (1)

        /* The following is optional if functional versions are available */

        /* define NULL pointer value */

        #ifndef NULL
            #ifdef __cplusplus
                #define NULL    0
            #else
                #define NULL    ((void *)0)
            #endif
        #endif

    #endif  /* UNICODE */

#if defined(DEBUG) || defined(_DEBUG)
    #define OUTPUTDEBUGSTRING(X) {_ftprintf(stderr, X);}
    #define ASSERT(X) {if(!(X)) fwprintf(stderr, _T("Assertion failed: %s (%s, %d)\n"), #X, __FILE__ , __LINE__);}
#else
    #define OUTPUTDEBUGSTRING(X)
    #define ASSERT(X) 
#endif
    #define OutputDebugString OUTPUTDEBUGSTRING

    enum VARENUM
        {   
        VT_EMPTY= 0,
        VT_NULL = 1,
        VT_I2   = 2,
        VT_I4   = 3,
        VT_R4   = 4,
        VT_R8   = 5,
        VT_CY   = 6,
        VT_DATE = 7,
        VT_BSTR = 8,
        VT_DISPATCH = 9,
        VT_ERROR    = 10,
        VT_BOOL = 11,
        VT_VARIANT  = 12,
        VT_UNKNOWN  = 13,
        VT_DECIMAL  = 14,
        VT_I1   = 16,
        VT_UI1  = 17,
        VT_UI2  = 18,
        VT_UI4  = 19,
        VT_I8   = 20,
        VT_UI8  = 21,
        VT_INT  = 22,
        VT_UINT = 23,
        VT_VOID = 24,
        VT_HRESULT  = 25,
        VT_PTR  = 26,
        VT_SAFEARRAY    = 27,
        VT_CARRAY   = 28,
        VT_USERDEFINED  = 29,
        VT_LPSTR    = 30,
        VT_LPWSTR   = 31,
        VT_RECORD   = 36,
        VT_FILETIME = 64,
        VT_BLOB = 65,
        VT_STREAM   = 66,
        VT_STORAGE  = 67,
        VT_STREAMED_OBJECT  = 68,
        VT_STORED_OBJECT    = 69,
        VT_BLOB_OBJECT  = 70,
        VT_CF   = 71,
        VT_CLSID    = 72,
        VT_BSTR_BLOB    = 0xfff,
        VT_VECTOR   = 0x1000,
        VT_ARRAY    = 0x2000,
        VT_BYREF    = 0x4000,
        VT_RESERVED = 0x8000,
        VT_ILLEGAL  = 0xffff,
        VT_ILLEGALMASKED    = 0xfff,
        VT_TYPEMASK = 0xfff
        };

    typedef unsigned short VARTYPE;
    typedef struct tagVARIANT VARIANT;
    struct  tagVARIANT
        {
                VARTYPE vt;
    /*            WORD wReserved1; */
    /*            WORD wReserved2; */
    /*            WORD wReserved3; */
                union 
                    {
                    ELONG32 lVal;
                    BYTE bVal;
                    SHORT iVal;
                    FLOAT fltVal;
                    DOUBLE dblVal;
                    VARIANT_BOOL boolVal;
    /*                _VARIANT_BOOL bool; */
                    SCODE scode;
    /*                CY cyVal; */
    /*                DATE date; */
                    BSTR bstrVal;
    /*                IUnknown __RPC_FAR *punkVal; */
    /*                IDispatch __RPC_FAR *pdispVal; */
    /*                SAFEARRAY __RPC_FAR *parray; */
    /*                BYTE __RPC_FAR *pbVal; */
    /*                SHORT __RPC_FAR *piVal; */
    /*                LONG __RPC_FAR *plVal; */
    /*                FLOAT __RPC_FAR *pfltVal; */
    /*                DOUBLE __RPC_FAR *pdblVal; */
    /*                VARIANT_BOOL __RPC_FAR *pboolVal; */
    /*                _VARIANT_BOOL __RPC_FAR *pbool; */
    /*                SCODE __RPC_FAR *pscode; */
    /*                CY __RPC_FAR *pcyVal; */
    /*                DATE __RPC_FAR *pdate; */
    /*                BSTR __RPC_FAR *pbstrVal; */
    /*                IUnknown __RPC_FAR *__RPC_FAR *ppunkVal; */
    /*                IDispatch __RPC_FAR *__RPC_FAR *ppdispVal; */
    /*                SAFEARRAY __RPC_FAR *__RPC_FAR *pparray; */
    /*                VARIANT __RPC_FAR *pvarVal; */
    /*                PVOID byref; */
    /*                CHAR cVal; */
                    USHORT uiVal;
                    EULONG32 ulVal;
                    INT intVal;
    /*                UINT uintVal; */
    /*                DECIMAL __RPC_FAR *pdecVal; */
    /*                CHAR __RPC_FAR *pcVal; */
    /*                USHORT __RPC_FAR *puiVal; */
    /*                ULONG __RPC_FAR *pulVal; */
    /*                INT __RPC_FAR *pintVal; */
    /*                UINT __RPC_FAR *puintVal; */
                    };
        };

    BSTR SysAllocString( WCHAR*  sz );
    HRESULT SysFreeString( BSTR  bstr );
    void    Sleep(unsigned long dwMilliseconds);

    void VariantInit(VARIANT * pvarg);
    HRESULT VariantClear(VARIANT * pvarg);
    HRESULT VariantCopy(VARIANT * pvargDest, VARIANT * pvargSrc);
    HRESULT VariantChangeType(VARIANT * pvargDest,
                    VARIANT * pvarSrc, USHORT wFlags, VARTYPE vt);


    #ifdef  __cplusplus
        }   /* ... extern "C" */
    #endif

#endif	// _OS_LINUX_H
