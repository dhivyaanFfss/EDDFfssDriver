//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ILogger.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for Communication with Logger
//
//  Interface  :  ILogger
//              
//**************************************************************************
#ifndef __ILOGGER__H__
#define __ILOGGER__H__

#include "ENotifyInterface.h"

/* Group=Logger */

// strings for triggers and actions.

/*Group=Logger Actions*/

// actions, stuff we can be told to do
//
#define ACTION_LOG_ALARM    	_T("DoLogAlarm")
#define ACTION_LOG_EVENT    	_T("DoLogEvent")
#define ACTION_LOG_SNAPSHOT     _T("DoLogSnapShot")
#define ACTION_LOG_STREAMING    _T("DoLogStreaming")

/*Group=Logger Triggers*/

// Schemas for Actions
//

// Triggers, these are separated so subscribers can filter easier
//  NOTE how there IS a snapshot trigger, but not an Action schema, it invokes
//  a different Action with dataset as schema
//
#define TRIGGER_LOG_SNAP        _T("OnLogSnapShot")
#define TRIGGER_LOG_ALARM       _T("OnLogAlarm")
#define TRIGGER_LOG_EVENT       _T("OnLogEvent")

/* Group=Logger */

// An empty interface still allows a GetInterface on the main object of
// the component. 
interface ILogger : public IAction
{
    virtual KERESULT GetHistoryData(ETimeStamp& StartTime, ETimeStamp& EndTime,
        EDataItemIDs& DataItemIDs, EDataItemPairList& DataItemPairs, int iMaxCount = -1) = 0;
};


#endif //__ILOGGER__H__
