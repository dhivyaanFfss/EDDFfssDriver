//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  BrowserInterfaces.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define interfaces for enumerating string, browsing and managing 
//				  enumerators,
//
//  Interfaces:
//				IEEnumString
//				IEnumManager
//				IBrowse
//
//**************************************************************************

#ifndef __BROWSER_INTERFACES_H__
#define __BROWSER_INTERFACES_H__

#include "KernelInterface.h"
#include "EEnumIID.h"

// group=Enumeration

#define ENUM_LEAF               0
#define ENUM_BRANCH             1

#define ENUM_UP                 0
#define ENUM_DOWN               1
#define ENUM_TO                 2

//**************************************************************************
// Note:  IEnumString is already used by Windows; therefore, use the name IEENumString instead.
interface IEEnumString : public IKernelBase
{
    virtual ~IEEnumString() = 0;
    virtual KERESULT GetCount(/* out */ DWORD* pCount) = 0;
    virtual KERESULT Next(  /* in */  DWORD cnt,
                            /* out */ TCHAR **ppEnumStrings, 
                            /* out */ DWORD *pActualCnt ) = 0;
    virtual KERESULT Reset() = 0;
};

//**************************************************************************
inline IEEnumString::~IEEnumString()
{
}

//**************************************************************************
interface IEnumManager
{
    virtual KERESULT Register(  /* in */  IEEnumString *pIEEnumString,
        			            /* out */ EHANDLE *phEnumID ) = 0;

    // Unregister is used by API EEnumRelease...
    virtual KERESULT Unregister(/* in */  EHANDLE hEnumID) = 0;
    virtual KERESULT GetCount(/* in */  EHANDLE hEnumID,
                              /* out */ DWORD* pCount) = 0;
    virtual KERESULT EnumNext(/* in */  EHANDLE hEnumID,
                              /* in */  DWORD cnt,
                              /* out */ TCHAR** ppEnumStrings, 
                              /* out */ DWORD* pActualCnt ) = 0;
    virtual KERESULT EnumReset(/* in */  EHANDLE hEnumID) = 0;
    virtual KERESULT ChangeBrowseType(/* in */  EHANDLE hEnumID,
                                       /* in */  DWORD type) = 0;
    virtual KERESULT ChangeBrowsePosition( /* in */  EHANDLE hEnumID,
                                            /* in */ DWORD direction,
				                            /* in */ TCHAR* pszLocation ) = 0;
    virtual KERESULT GetQualifiedName(/* in */  EHANDLE hEnumID,
                                      /* in */  TCHAR* pszShortName,
				                      /* out */ TCHAR** ppszQualifiedName) = 0;
};

//**************************************************************************
interface IBrowse : public IEEnumString
{
    virtual KERESULT ChangeType(/* in */ DWORD type ) = 0;
    virtual KERESULT ChangePosition(/* in */ DWORD direction,
				                    /* in */ TCHAR *pszLocation ) = 0;
    virtual KERESULT GetQualifiedName(/* in */  TCHAR *pszShortName,
				                      /* out */ TCHAR **ppszQualifiedName) = 0;
};

//**************************************************************************
// This interface is implemented by component managers that create enumerator objects (i.e. browsers).
// IEnumerator creates the object, but the lifetime is managed by IEnumManager.

interface IEnumerator
{
    // output:  handle to enumerator that can be used with IEnumManager.
    virtual KERESULT CreateEnumerator(/*in*/ENUM_IID enumId, /* out */ EHANDLE *phEnumID) = 0;
};

#endif 
