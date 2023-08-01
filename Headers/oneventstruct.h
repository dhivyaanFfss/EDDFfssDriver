/**************************************************************************
*
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
*
***************************************************************************
*
*  Filename   :  oneventstruct.h
*  
*  Subsystem  :  Kernel
*
*  Description:  Define ONEVENTSTRUCT for non-windows platforms. 
*                Use the OPC A&E include on Windows.
*
*
***************************************************************************/

#ifndef __ONEVENTSTRUCT_H__
#define __ONEVENTSTRUCT_H__

typedef struct  __tagONEVENTSTRUCT
    {
    WORD wChangeMask;          // Indicates to the client which properties of the condition have changed, to have caused the server to send the event notification. 
    WORD wNewState;            // A word bit mask of three bits specifying the new state of the condition.
    LPWSTR szSource;           // The source of event notification.
    ETimeStamp ftTime;           // Time of the event occurrence. 
    LPWSTR szMessage;          // Event notification message describing the event. 
    DWORD dwEventType;         // Identifies events as Simple, Condition-Related, or Tracking. 
    DWORD dwEventCategory;     // Standard and Vendor-specific event category codes. 
    DWORD dwSeverity;          // Event severity (from 0 to 1000).  
    LPWSTR szConditionName;    // Name of the condition related to this event notification. 
    LPWSTR szSubconditionName; // Name of the current sub-condition, for multi-state conditions. 
    WORD wQuality;             // Quality associated with the condition state. 
    BOOL bAckRequired;         // Indicates if the related condition requires acknowledgment of this event. 
    ETimeStamp ftActiveTime;     // Time that the condition became active (for single-state conditions), or the time of the transition into the current sub-condition (for multi-state conditions). 
    ULONG_PTR dwCookie;        // Server defined cookie associated with the event notification. 
    DWORD dwNumEventAttrs;     // The length of the vendor-specific event attribute array. 
    VARIANT *pEventAttributes; // Pointer to an array of vendor-specific event attributes returned for this event notification. 
    LPWSTR szActorID;          // For tracking events, this is the actor ID for the event notification. 
    }	ONEVENTSTRUCT;

#endif
