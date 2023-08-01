//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IDeviceUUIDMgr.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Device UUID manager interface
//
//**************************************************************************

#ifndef _IDEVICE_UUID_MGR_H_
#define _IDEVICE_UUID_MGR_H_


//**************************************************************************
interface IDeviceUUIDMgr
{
    // assign UUID to device (create new UUID if device is not in the map)
    virtual KERESULT AddDevice(TCHAR *pMN, TCHAR *pSN) = 0;

    // get device UUID
    virtual KERESULT GetDeviceUUID(TCHAR *pMN, TCHAR *pSN, EString& esUUID) = 0;

    // synchronize internal device-UUID map with file
    virtual KERESULT Sync(void) = 0;
};

#endif
