//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ITimer.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Timer Manager interface definitions.
//
//  Classes:                                         
//          ITimerMgr
//
//**************************************************************************

#ifndef __ITIMER_H__
#define __ITIMER_H__

/* Group=Timer */                                                      
//**************************************************************************
// Support enumerations and definitions
//**************************************************************************

// Trigger name for all Timers.  Use when subscribing for to a Timmer trigger.
#define ON_TIMER_TRIGGER_NAME _T("OnTimer")

// Timer action names.
#define  DO_TIMER_RESET     _T("DoTimerReset")
#define  DO_TIMER_START     _T("DoTimerStart")
#define  DO_TIMER_STOP      _T("DoTimerStop")


enum TimerType
{
    ABSOLUTE_TIMER,
    RELATIVE_TIMER
}; 

enum TimerPeriod
{
    PERIODIC,
    ONE_SHOT
}; 

// Date Occurance of timer
enum TimerDate
{
    DAILY_EVERYDAY,
    DAILY_WEEKDAYS,
    WEEKLY,             // see m_daysEnabled bit pattern.
    MONTHLY_PICKWEEK,
    MONTHLY_ONDAY,
    YEARLY_PICKWEEK,
    YEARLY_ONDAY,
    HOURLY,              // added for 3.5
    MINUTELY            // MINUTELY TIMERS ARE JUST FOR INTERNAL TESTING.

};

/* Group=Day define */
// Bit pattern (ORed) for day enabled when using Weekly Timer
#define ON_SUNDAY       0x01
#define ON_MONDAY       0x02
#define ON_TUESDAY      0x04
#define ON_WEDNESDAY    0x08
#define ON_THURSDAY     0x10
#define ON_FRIDAY       0x20
#define ON_SATURDAY     0x40

/* Group=Timer state define */
// Define timer state bits.
#define TIMER_IDLE          0       // ~SCHEDULED
#define TIMER_SCHEDULED     0x1
#define TIMER_ARMED         0x2

/* Group=Timer */

//**************************************************************************
// Timer Definition
// FUTURE:  Something similar will be needed when writing the Client API
//**************************************************************************
struct ETimerDef
{
    ETimerDef()
    : m_dwSeverity(0), m_type(RELATIVE_TIMER), m_period(ONE_SHOT), 
      m_bStartImmediately(false), m_absDate(WEEKLY),
      m_daysEnabled(ON_MONDAY), m_weekNumber(1)
    {
        memset(&m_configTime, 0, sizeof(struct tm));
    }

    EString         m_esName;          // schema name
    EString         m_esMessage;       // Trigger message
    EString         m_esDescription;   // overall description of schema
    DWORD           m_dwSeverity;         // Trigger severity

    // Required info for all timers
    TimerType       m_type;
    TimerPeriod     m_period;
    bool            m_bStartImmediately;    // False means start via event manager trigger
    struct tm       m_configTime;

    // Required info for Absolute timer
    TimerDate       m_absDate;
    DWORD           m_daysEnabled;      // bit pattern used w/ Weekly timer
    DWORD           m_weekNumber;       // Week number (1-5) not in SystemTime
};

//**************************************************************************
// ITimerMgr interface definition
//**************************************************************************

interface ITimerMgr : public IKernelBase
{
    // Add a timer using the ETimerDef.  Set bInternal true to keep the
    // timer from being saved in the .xml file.
    virtual KERESULT AddTimer(ETimerDef* pTimerDef, bool bInternal = false, bool bOverwrite = false) = 0;
    virtual KERESULT RemoveTimer(TCHAR* pszTimerName) = 0;

    // 2.5 patch functions.  Allows EnterpriseProxy RPC functions which set the time/timezone
    // to do so with minimal impact on running timers.
    // First call PauseAllTimers before the time change, then ReInitializeAllTimers after
    // the time has been updated.
    virtual KERESULT ReInitializeAllTimers() = 0;
    virtual KERESULT PauseAllTimers() = 0; 

    // Qualify/Unqualify dynamic timer names
    virtual KERESULT QualifyDynamicName(EString& name, bool qualify = true) = 0;

    // Additional 3.9 features
    virtual KERESULT Start(TCHAR* pszTimerName, int gatewayId = CONNECTOR_GATEWAY) = 0;
    virtual KERESULT Stop(TCHAR* pszTimerName, int gatewayId = CONNECTOR_GATEWAY) = 0;
    virtual KERESULT Reset(TCHAR* pszTimerName, int gatewayId = CONNECTOR_GATEWAY) = 0;

};

#endif


