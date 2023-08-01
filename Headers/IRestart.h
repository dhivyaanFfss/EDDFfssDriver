//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IRestart.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define IRestart interface
//
//
//**************************************************************************

#ifndef __IRESTART_H__
#define __IRESTART_H__

//**************************************************************************
interface IRestart
{
    virtual void SetSystemBootFlag() = 0;
    virtual void SetRestartFlag() = 0;
    virtual void SetSoftRestartFlag() = 0;
    virtual void UnsetRestartFlag() = 0;
    virtual KERESULT Shutdown() = 0;
    virtual void SetWarmRestart(bool) = 0;
    virtual bool IsWarmRestart() = 0;       // Is the session recovering data for warm restart
    virtual bool SaveForWarmRestart() = 0;  // Is the session configured for warm restart
    virtual void SetWarmRestartCleanup(bool bCleanup) = 0;
};

#endif


