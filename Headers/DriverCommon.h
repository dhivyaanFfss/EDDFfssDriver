//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename    :   DriverCommon.h
//  
//  Description :   Defines the constants used by all Driver API support
//
//
//**************************************************************************

#ifndef __DRIVER_COMMON__H_
#define __DRIVER_COMMON__H_


#ifdef WIN32
    #ifdef DRIVER_EXPORTS
        #define DRIVER_API __declspec(dllexport)
    #else
        #define DRIVER_API __declspec(dllimport)
    #endif
#else
   #define DRIVER_API
#endif


/////////////////////////////////////////////////////////////////////////////
//
// Driver API return code
//
/////////////////////////////////////////////////////////////////////////////
typedef unsigned long DRIVER_RESULT;

#define  DRIVER_OK              0                   // success, same value as KE_OK
#define  DRIVER_FAILED          (0xC0000002L)       // failed, same value as KE_FAILED
#define  DRIVER_NOT_INITIALIZED (0xC0000003L)       // Driver not initialized
#define  DRIVER_MEMORY          (0xC0000004L)       // memory allocation failed.



/////////////////////////////////////////////////////////////////////////////
//
// Driver tag attributes
//
/////////////////////////////////////////////////////////////////////////////
#define DRIVER_FLAG_NONE           0X00000000

#define DRIVER_FLAG_READABLE       0X00000001 
#define DRIVER_FLAG_WRITEABLE      0X00000002

#define DRIVER_FLAG_ANALOG         0X00000010 
#define DRIVER_FLAG_DIGITAL        0X00000020
#define DRIVER_FLAG_STRING         0X00000040


#endif // __DRIVER_COMMON__H_
