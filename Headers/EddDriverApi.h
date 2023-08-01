//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename    :   EddDriverApi.h
//  
//  Description :   EDD Driver exported API's
//
//
//**************************************************************************

#ifndef __EDD_DRIVER_API__H_
#define __EDD_DRIVER_API__H_


#include "DriverCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Call this API to initialize the driver
typedef DRIVER_RESULT (*EDDINIT_FUNCTION)(TCHAR*);
DRIVER_RESULT DRIVER_API EDDInit(TCHAR* pszIdentifier);

// Call this API to browse tags items
// Caller is responsible of freeing memory by calling SNMPMemFree recursively on each pointer
typedef DRIVER_RESULT (*EDDBROWSE_FUNCTION)(DWORD&, TCHAR**&, DWORD*&);
DRIVER_RESULT DRIVER_API EDDBrowseItems(DWORD& dwCount, TCHAR**& ppszItems, DWORD*& pdwAttributes);

// Call this API to free memory returned by any EDD API
typedef DRIVER_RESULT (*EDDMEMFREE_FUNCTION)(void*);
DRIVER_RESULT DRIVER_API EDDMemFree(void* pMem);

// Call this API to shutdown the driver
typedef DRIVER_RESULT (*EDDSHUTDOWN_FUNCTION)(TCHAR*);
DRIVER_RESULT DRIVER_API EDDShutdown(TCHAR* pszIdentifier);

#ifdef __cplusplus
}
#endif


#endif // __EDD_DRIVER_API__H_
