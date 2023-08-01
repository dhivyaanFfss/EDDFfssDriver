//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename    :   SNMPDriverApi.h
//  
//  Description :   SNMP Driver exported API's
//
//
//**************************************************************************

#ifndef __SNMP_DRIVER_API__H_
#define __SNMP_DRIVER_API__H_

#include "DriverCommon.h"

#define TIME_OUT_SECONDS       5

#ifdef __cplusplus
extern "C"
{
#endif

// Descriptor to describe a SNMP device category
typedef struct tagSNMP_Descriptor
{
    TCHAR*      m_pszSNMPName;              // category OID in SNMP dot format
    TCHAR*      m_pszReadableName;          // category OID in reababletext foramt
    DWORD       m_hostCount;                // Number of hosts under this category
    TCHAR**     m_ppszHosts;                // actual host names
    TCHAR**     m_ppszCommunities;          // community names (should correspond to hosts array)
}SNMP_DESCRIPTOR;

typedef struct tagSNMP_BrowseItem
{
    TCHAR*      m_pszNumericOID;            // numeric form OID
    TCHAR*      m_pszResolvedOID;           // human-readable OID
    TCHAR*      m_pszValue;                 // object value string
    TCHAR*      m_pszModule;                // name of a module (MIB) where the object is defined
    TCHAR*      m_pszType;                  // object type (syntax)
    TCHAR*      m_pszEnum;                  // enumerated values of the object if any
    TCHAR*      m_pszAccess;                // access type
    TCHAR*      m_pszStatus;                // standardization status
}SNMP_BROWSE_ITEM;

typedef bool (*SNMP_BROWSE_CALLBACK)(SNMP_BROWSE_ITEM *pItem, void *pUserData);

// Call this API to initialize the driver
typedef DRIVER_RESULT (*SNMPINIT_FUNCTION)(TCHAR*);
DRIVER_RESULT DRIVER_API SNMPInit(TCHAR* pszIdentifier);

// Specifies SNMP version to be used (0 == SNMPv1; 1 == SNMPv2)
typedef DRIVER_RESULT (*SNMPSETVERSION_FUNCTION)(int);
DRIVER_RESULT DRIVER_API SNMPSetVersion(int iVersion);

// Adds include IP range for discovery
typedef DRIVER_RESULT (*SNMPADDINCLUDERANGE_FUNCTION)(TCHAR*, TCHAR*);
DRIVER_RESULT DRIVER_API SNMPAddIncludeRange(TCHAR* pszStart, TCHAR* pszEnd);

// Enables/disables broadcast for discovery
typedef DRIVER_RESULT (*SNMPENABLEBROADCAST_FUNCTION)(bool);
DRIVER_RESULT DRIVER_API SNMPEnableBroadcast(bool bBroadcast);

// Call this API to discover SNMP devices. 
// Caller is responsible of freeing memory by calling SNMPMemFree recursively on each pointer
// pIKernelBase is a void* because the 'C' interface has no definition.  
// Use NULL if undefined.
typedef DRIVER_RESULT (*SNMPDISCOVER_FUNCTION)(void*, TCHAR*, DWORD&, SNMP_DESCRIPTOR*&);
DRIVER_RESULT DRIVER_API SNMPDiscover(void* pIKernelBase, TCHAR* pszCommunity, DWORD& dwCount, 
                                      SNMP_DESCRIPTOR*& pDescriptors);

// Call this API to browse items
// Caller is responsible of freeing memory by calling SNMPMemFree recursively on each pointer
typedef DRIVER_RESULT (*SNMPBROWSE_FUNCTION)(TCHAR*,  TCHAR*, SNMP_BROWSE_CALLBACK, void*);
DRIVER_RESULT DRIVER_API SNMPBrowseItems(TCHAR* pszCommunity, TCHAR* pszHost, SNMP_BROWSE_CALLBACK pCallback, void *pUserData);

// Returns value of arbitrary OID
DRIVER_RESULT DRIVER_API SNMPGet(TCHAR* pszCommunity, TCHAR* pszHost, TCHAR* pszOID, TCHAR** pszValue, TCHAR** pszResValue);

// Returns value of System OID
typedef DRIVER_RESULT (*SNMPGETSYSOID_FUNCTION)(TCHAR*, TCHAR*, TCHAR**,TCHAR**);
DRIVER_RESULT DRIVER_API SNMPGetSysOID(TCHAR* pszCommunity, TCHAR* pszHost, TCHAR** pszValue, TCHAR** pszResValue);

// Call this API to free memory returned by any driver API
typedef DRIVER_RESULT (*SNMPMEMFREE_FUNCTION)(void*);
DRIVER_RESULT DRIVER_API SNMPMemFree(void* pMem);

// Call this API to shutdown the driver
typedef DRIVER_RESULT (*SNMPSHUTDOWN_FUNCTION)(TCHAR*);
DRIVER_RESULT DRIVER_API SNMPShutdown(TCHAR* pszIdentifier);

#ifdef __cplusplus
}
#endif


#endif // __SNMP_DRIVER_API__H_
