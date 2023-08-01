//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IDeviceInfoProvider.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define IDeviceInfoProvider interface
//
//
//**************************************************************************

#ifndef __I_DEVICEINFO_PROVIDER_H__
#define __I_DEVICEINFO_PROVIDER_H__

//**************************************************************************
interface IDeviceInfoProvider
{
    // Get the device info for a particular name
    virtual KERESULT GetDeviceInfo(int gatewayID, EString& esName, EString& esValue) = 0;
};

#endif // __I_DEVICEINFO_PROVIDER_H__


