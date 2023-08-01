//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  AlarmListener.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Alarm Listener object.  This is available for general use.
//
//  Classes:
//              EAlarmListener  // Exported - an implementation of IAlarmListener
//
//**************************************************************************

#ifndef __ALARMLISTENER_H__
#define __ALARMLISTENER_H__


#ifndef __ECOLLECTIONS_H__
#include "ECollections.h"
#endif

#ifndef __DYNALARM_INTERFACE_H__
#include "DynamicAlarmInterface.h"
#endif

#include "oneventstruct.h"

// Size of the stack queue for EAlarmListener::OnCondEvent.
// {secret}
#define     FIXED_ONEVENT_QUEUE     10

/* Group=Alarms */

//**************************************************************************
//  EAlarmListener is exported and can be used by other components.  
//**************************************************************************
class _DV_DECLSPEC EAlarmListener : public IAlarmListener
{
public:
    EAlarmListener(void) 
        : IAlarmListener() {}

    // IAlarmListener methods
   virtual void OnCondEvent(LONG_PTR nClientId, CEventBuf* pEvents);

   // Remarks:  Use of the ONEVENTSTRUCT makes this function similar to that documented 
   // in the OPC Alarm and Event specification. The client derives from EAlarmListener and provides 
   // OnOPCEvent() to handle event notifications.

   virtual void OnOPCEvent(EHANDLE hClient, DWORD dwCount, ONEVENTSTRUCT *pEvents);
    
protected:
    // methods
    WCHAR* GetMessage(Condition condId, SubCondition subCondId);
};

// {secret}
typedef ETypedPtrMap<EMAP_PTR_TO_PTR, void*,  EAlarmListener*> EAlarmListeners;

#endif
