//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  AlarmManInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Alarm manager Interface
//
//  Classes:
//          IAlarmMgr
//
//************************************************************************** 

#ifndef __ALARMMAN_INTERFACE_H__
#define __ALARMMAN_INTERFACE_H__

#include "AlarmDefInterface.h"
#include "DynamicAlarmInterface.h"

/* Group=Alarms */

////////////////////////////////////////////////////////////////////////
// Alarm Manager : interface definition.

interface IAlarmMgr : public IKernelBase
{
   virtual IAlarmDef* GetAlarm(const TCHAR* strDataItem,
                               Condition    Condition,
                               SubCondition SubCondition) = 0;

   virtual int        GetAlarmCount(bool bItemsOnly = false) = 0;

   virtual int        InsertAlarm(const IAlarmDef*, bool bReplace = false) = 0;

   virtual int        RemoveAlarm(const TCHAR* strDataItem,
                                  Condition    Condition,
                                  SubCondition SubCondition) = 0;

   virtual int        ModifyAlarmArea (const TCHAR*   strDataItem,
                                       const TCHAR*   strNewArea,
                                       bool  bNewArea = false) = 0;
	
   virtual IAlarmDef* CreateAlarmDef() = 0;

   virtual IAlarmDef* GetAlarm(const TCHAR* strDataItem,
                               const TCHAR* strCondName) = 0;
};

#endif

