//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  IStateMonitoring.h 
//  
//  Subsystem  :  StateMonitoring
//
//  Description:  State Monitoring interface and support definitions.
//
//  Classes:       
//              IStateMonitoring
//
//*****************************************************************************

#ifndef __ISTATE_MONITORING_H__
#define __ISTATE_MONITORING_H__

/* Group=StateMonitoring */

// Define triggers
static const TCHAR* MONITOR_STATE_CHANGED_TRIGGER = _T("OnMonitorStateChanged");
static const TCHAR* MONITOR_COMPLETE_TRIGGER = _T("OnMonitorComplete");
static const TCHAR* MONITOR_REAL_TIME_UPDATE_TRIGGER = _T("OnMonitorRealTimeUpdate");

// Define actions
static const TCHAR* RESET_MONITOR_ACTION = _T("DoResetMonitor");

static const TCHAR* MONITOR_COMPLETE_MESSAGE = _T("Monitor Complete");
static const TCHAR* MONITOR_REAL_TIME_UPDATE_MESSAGE = _T("Monitor Real Time Update");

#define MONITOR_TRIGGER_SEVERITY    1

//*****************************************************************************
interface IStateMonitoring : public IKernelBase
{
    // Error Returns:
    // KE_NOTFOUND:  ALL of the following functions return KE_NOTFOUND if the Monitor can
    //      not be found using the parameter:  esMonitor.
    // KE_TYPE_MISMATCH:  at ResetMonitor if the monitor type is time-based.
    //                    when the configuation of "stateChangeTrigger" is not "customEvent"


    // Use SetState to change the state of a given monitor.
    // Processing is done asynchronously.
    // Inputs:
    //   esMonitor:  the name of the monitor
    //               If monitor name is not found, then error return is:  KE_NOTFOUND.
    //   stateValue:  Data value with a type (Analog, Digital, String),
    //                a value and a timestamp.
    //   gatewayId indicates which mn/sn is being addressed.
    virtual KERESULT SetState(EString& esMonitor, CDataValue& stateValue, 
        int gatewayId = CONNECTOR_GATEWAY) = 0;

    // ResetMonitor allows custom program control of when a monitor
    // is reset.
    // This should only be done on "Manual" monitors NOT time-based monitors.
    // If monitor is "time-based", then error return is:  KE_TYPE_MISMATCH.
    // Processing is done asynchronously.
    // Inputs:
    //   esMonitor:  the name of the monitor.  
    //               If monitor name is not found, then error return is:  KE_NOTFOUND.
    //   gatewayId indicates which mn/sn is being addressed.
    virtual KERESULT ResetMonitor(EString& esMonitor, int gatewayId = CONNECTOR_GATEWAY) = 0;

};

#endif


