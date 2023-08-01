//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IcomMgr.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define communications manager
//
//  Interfaces:
//				IComMgr
//
//**************************************************************************

#ifndef __ICOM_MGR_H__
#define __ICOM_MGR_H__

#include "IInternetProvider.h"
#include "IInternetConsumer.h"
#include "IDeployConfig.h"

//**************************************************************************
interface IComMgr
{
    // Called by internet providing providing (IInternetProvider) (EDialer) rather
    // than old style OnDial trigger when internet connection available
    virtual KERESULT InternetConnected() = 0;

    // called by provider in PrepareToStart:
    virtual KERESULT RegisterInternetProvider(IInternetProvider *provider) = 0;
    // called by consumer in PrepareToStart:
    virtual KERESULT RegisterInternetConsumer(IInternetConsumer *consumer) = 0;

    // called by kernelShell in Restore:
    virtual KERESULT SetProviderMode(bool usingProvider) = 0;
    // called by kernelShell at the end of PrepareToStart:
    virtual KERESULT RegistrationsComplete() = 0;
    // called by kernelShell at the beginning of PrepareToStop:
    virtual KERESULT AboutToStop() = 0;

    // Returns the current connection infomation
    virtual KERESULT GetConnectionInfo(CONNECTION_INFO*) = 0;

    // returns provider mode
    virtual KERESULT GetProviderMode(bool *bUsingProvider) = 0;

    // returns pointer to IInternetProvider
    virtual KERESULT GetInternetProvider(IInternetProvider **ppProvider) = 0;

    // called by Internet provider when link is down
    virtual KERESULT InternetDisconnected() = 0;
};

#endif


