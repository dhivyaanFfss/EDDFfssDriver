//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDAlarm.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Demonstrates how to use the IEDDCallback interface to 
//                generate an alarm and set an existing alarm active or inactive.
//
//
//  Classes    :  
//                  EDDDriver
//

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"


//*****************************************************************************
// Agent Note:
// For the Agent to publish the Alarm to the DRM Enterprise, the Builder project
// must define an Alarm filter that allows "Dynamic Alarms".
KERESULT EDDDriver::StartAlarm(TCHAR* pszName, TCHAR* pszMessage,  int severity)
{
    // Publish Alarm.
    // The m_hAlarm handle is set by the called Agent function.  This must be 
    // used when calling SetAlarmActive().

    WORD alarmState = ENABLED | ACTIVE; // alarm state is a bit pattern.
    KERESULT ker = m_pIEDDCallback->StartAlarm(this, pszName, pszMessage, severity, 
        alarmState, m_hAlarm);
    return ker;
}

//*****************************************************************************
KERESULT EDDDriver::SetAlarmState(bool bActive)
{
    if (m_hAlarm == 0)
    {
        ASSERT(0);  // Must call StartAlarm() first.
        return KE_INVALID_HANDLE;
    }

    KERESULT ker = m_pIEDDCallback->SetAlarmActive(m_hAlarm, bActive);
    return ker;
}


