//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDEvent.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Demonstrates how to use the IEDDCallback interface to 
//                generate an event.
//
//  Classes    :  
//                  EDDDriver
//                  
//


// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"


//*****************************************************************************
// Post's an event through the Agent directly to the DRM Enterprise Server.
KERESULT EDDDriver::PostEvent(TCHAR* pszName, TCHAR* pszMessage,  int severity)
{
    // Create array of events.
    EDDEvent* pEDDEvents = new EDDEvent[1];

    // Set Event information.
    pEDDEvents[0].Name = pszName;
    pEDDEvents[0].Message = pszMessage;
    pEDDEvents[0].Severity = severity;

    // Get current time stamp
    ETime::GetTimeStamp(&pEDDEvents[0].EventTime);

    // Publish Event.
    KERESULT ker = m_pIEDDCallback->OnEvents(this, 1, pEDDEvents);

    // Driver is responsible to free allocated memory.
    delete [] pEDDEvents;

    return ker;

}


