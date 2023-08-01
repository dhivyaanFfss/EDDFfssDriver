//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IRunTime.h
//  
//  Subsystem  :  Connector
//
//  Description:  Define Gateway RunTime methods for Gateway
//
//  Classes:    
//				IRunTime
//				ERunTime
//
//**************************************************************************
#ifndef __IRUNTIME_H__
#define __IRUNTIME_H__

//**************************************************************************
interface IRunTime
{
    // 
    virtual KERESULT RTPreStart() = 0;
    virtual KERESULT RTStart() = 0;

    // read write configuration files.
    virtual KERESULT RTRestore(EHANDLE pData, int gatewayId) = 0;

    // future?
//    virtual KERESULT RTDisable(int gatewayId) = 0;
//    virtual KERESULT RTEnable(int gatewayId) = 0;
//    virtual KERESULT RTRemove(int gatewayId) = 0;
};

#endif
