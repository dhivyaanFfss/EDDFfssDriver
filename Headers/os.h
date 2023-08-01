/*****************************************************************************

  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.

  *************************************************************************

  Filename   :  os.h
  
  Subsystem  :  Kernel/Base

  Description:  Platform-specific includes, defines, typedefs, etc. go here.
               This should be the first file included in sources.
                
  Platform supported: (define ONE of the following symbols)
        WIN32 for Windows 2000, NT, 98, and CE. CE will define UNDER_CE also.
        LINUX, SOLARIS
******************************************************************************/

#ifndef _OS_H_
#define _OS_H_

// General definitions for all OS's

/* Always compile for UNICODE, unless explicitly disabled */
#ifndef NO_UNICODE
    #ifndef UNICODE
        #define UNICODE
    #endif
#endif

/* Used for pure interfaces */
#if !(defined(WIN32) && (_MSC_VER >= 1300))
    #ifndef  interface
    #define  interface   struct
    #endif
#endif

/* typedef for Unity Kernel */
typedef void* HREMOTE;
typedef void* HKERNEL;  /*  handle to Kernel. Used when calling each API function. */

// Remarks: This is a handle used when subscribing for notification.
typedef void* HSUBSCRIPTION;    
typedef unsigned short MSGFILE;

#define INVALID_MSGFILE     ((MSGFILE) -1)
#define MAX_MESSAGE_NUMBER  65535

#ifdef __GATEWAY__
#define TO_STR_NUM(x)   (x*2)+1
#else
#define TO_STR_NUM(x)   (x*2)
#endif

#if defined(WIN32) || defined(UNDER_CE)
#include "os_win32.h"
#endif

#if defined(LINUX) || defined(__QNX__) || defined(SOLARIS)
#include "os_unix.h"
#endif

#ifndef BYTE_SWAPPED
#error "Byte order must be defined"
#endif

#if !(defined WIN32) && !(defined UNDER_CE) && !(defined LINUX) && !(defined SOLARIS)
#error "Unsupported OS"
#endif  // !(defined WIN32, UNDER_CE, LINUX, SOLARIS)

#endif  /* _OS_H_ */
