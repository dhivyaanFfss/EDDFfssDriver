//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IInternetProvider.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Provider of internet services (like modem)
//
//  Interfaces:
//				IComMgr
//
//**************************************************************************

#ifndef __IINTERNET_PROVIDER_H__
#define __IINTERNET_PROVIDER_H__

#include "IDeployConfig.h"

//**************************************************************************
interface IInternetProvider
{
    virtual KERESULT ConnectToInternet() = 0;
    virtual KERESULT DisconnectFromInternet() = 0;
    virtual KERESULT GetDialupInfo(CONNECTION_INFO& connectionInfo) = 0;

    // Returns reference count for the internet connection
    virtual KERESULT GetConnectionRefCount(int *piRefCount) = 0;
};

#endif


