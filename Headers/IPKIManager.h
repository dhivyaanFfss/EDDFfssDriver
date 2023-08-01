//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IPKIManager.h
//  
//  Description:  Interface for Communication with FileWatch component
//
//  Interface  :  IFileWatch
//              
//**************************************************************************
#ifndef __I_PKI_MANAGER_H__
#define __I_PKI_MANAGER_H__

#include "EPKI.h"

//**************************************************************************
interface IPKIManager
{
    // When bUseSSLKeyConfig is true, the trust and identity parts of
    // the profile are taken from secure storage (if it exists). However if the
    // trust and identity are already defined in the profile, they are NOT
    // overriden by the information from secure storage.
    virtual KERESULT    GetProfile(const TCHAR *pszName,
                                   EPKI::Profile*& pProfile,
                                   bool bUseSSLKeyConfig = false) = 0;
    virtual KERESULT    GetTrustManager(const EPKI::Profile* pProfile,
                                        EPKI::TrustManager*& pTrustManager) = 0;
    virtual KERESULT    GetSSLContext(const EPKI::Profile* pProfile,
                                      EPKI::SSLContext*& pSSLContext) = 0;
};

#endif
