//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ISetTime.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define ISetTime interface
//
//
//**************************************************************************

#ifndef __ISETTIME_H__
#define __ISETTIME_H__

//**************************************************************************
interface ISetTime
{
    virtual KERESULT SetTime(long secondsGMTFrom1970) = 0;
    virtual KERESULT SetTimeZone(int minutesFromGMT, int DSToffset, struct tm&, struct tm&) = 0;
};

#endif


