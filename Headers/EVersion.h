//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EVersion.h
//
//  Subsystem  :  Kernel
//
//  Description:  Version managment
//
//**************************************************************************
#ifndef _EVERSION__H_
#define _EVERSION__H_

#define __EVERSION_STRING_FUNC(major, minor, patch, build)\
    #major "." #minor "." #patch "." #build
#define EVERSION_STRING_FUNC(major, minor, patch, build)\
    __EVERSION_STRING_FUNC(major, minor, patch, build)

#define __EVERSION_WSTRING_FUNC(major, minor, patch, build)\
    L ## #major L"." L ## #minor L"." L ## #patch L"." L ## #build
#define EVERSION_WSTRING_FUNC(major, minor, patch, build)\
    __EVERSION_WSTRING_FUNC(major, minor, patch, build)

#define __EVERSION_BUILD_FUNC(i, f)\
    i ## . ## f
#define EVERSION_BUILD_FUNC(i, f)\
    __EVERSION_BUILD_FUNC(i, f)

#ifdef UNICODE
#define EVERSION_TSTRING_FUNC EVERSION_WSTRING_FUNC
#else
#define EVERSION_TSTRING_FUNC EVERSION_STRING_FUNC
#endif

/*
 Kernel Version Structure
     Major#.Minor#  Build#

 If there is a Patch version
     Major#.Minor#.Patch#  Build#.Patch Build #
     Note:  The patch build number is the fraction part of the Build number.
            It should increment every time a patch is sent to QA for testing
            even if the Patch version itself is not incremented.

*/
#define KERNEL_VERSION_MAJOR  6
#define KERNEL_VERSION_MINOR  9
#define KERNEL_VERSION_PATCH  4

/*
 Build number is a real number whose decimal representation has the main build
 number in the integral part and the "sub-build" in the fractional part.
*/
#define KERNEL_VERSION_BUILD_INT       1057
#define KERNEL_VERSION_BUILD_FRACTION  0
#define KERNEL_VERSION_BUILD\
    EVERSION_BUILD_FUNC(KERNEL_VERSION_BUILD_INT,\
                        KERNEL_VERSION_BUILD_FRACTION)

#define KERNEL_VERSION_LIST\
    KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR, KERNEL_VERSION_PATCH, KERNEL_VERSION_BUILD_INT
#define KERNEL_VERSION_STRING\
    EVERSION_STRING_FUNC(KERNEL_VERSION_MAJOR,\
                         KERNEL_VERSION_MINOR,\
                         KERNEL_VERSION_PATCH,\
                         KERNEL_VERSION_BUILD)

#define KERNEL_VERSION_TSTRING\
    EVERSION_TSTRING_FUNC(KERNEL_VERSION_MAJOR,\
                          KERNEL_VERSION_MINOR,\
                          KERNEL_VERSION_PATCH,\
                          KERNEL_VERSION_BUILD)

#ifndef __GATEWAY__
#define KERNEL_PRODUCT_NAME "Axeda Connector"
#else
#define KERNEL_PRODUCT_NAME "Axeda Gateway"
#endif

#define CUSTOM_RELEASE_NAME _T("")
/* 
 * OpenSSL - Numeric release version identifier
 * MNNFFPPS: major minor fix patch status
 * Reference: include\openssl\opensslv.h
 */
#define OPENSSL_VERSION_1_1_0       0x10100000L  // "OpenSSL 1.1.0"
#define OPENSSL_VERSION_1_1_0_pre1  0x10100001L  // "OpenSSL 1.1.0-pre1 (alpha) 10 Dec 2015"
#define OPENSSL_VERSION_1_1_1c      0x1010103fL  // "OpenSSL 1.1.1c  28 May 2019"
#define OPENSSL_VERSION_1_1_1d      0x1010104fL  // "OpenSSL 1.1.1d  10 Sep 2019"

#endif //_EVERSION__H_
