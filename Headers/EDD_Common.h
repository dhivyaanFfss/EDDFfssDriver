//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename :  EDD_common.h
//  
//              Common include file for all EDD drivers
//
//**************************************************************************

#ifndef __EDD_COMMON_H__
#define __EDD_COMMON_H__


// HANDLE FOR EDD DRIVER
typedef void*   EDDHANDLE;

// need some alarm defines
#include "Alarm.h"


/////////////////////////////////////////////////////////////////////////////
//
// EDDEvent for device events
//
/////////////////////////////////////////////////////////////////////////////
class EDDEvent
{
public:
    EDDEvent() {}

    EString         Name;           // Internal event name
    EString         Message;        // User readable description
    int             Severity;       // 0 - 1000
    ETimeStamp      EventTime;      // Time generated

    EDDEvent&       operator=(const EDDEvent& src)
    {
        if(this != &src)
        {
            Name        = src.Name;
            Message     = src.Message;
            Severity    = src.Severity;
            EventTime   = src.EventTime;
        }
        return *this;
    }
};


/////////////////////////////////////////////////////////////////////////////
//
// IEDDCallback is callback Interface for EDD device driver
//
/////////////////////////////////////////////////////////////////////////////
interface IEDDDriver;
interface IEDDCallback
{
    // EDD Driver calls this member to send data changes back
    // The *** EDD Driver *** is responsible for freeing the memory
    virtual KERESULT    OnDataChange(int n, EDDHANDLE*, CDataValue*) = 0;

    // EDD Driver calls this member to send device generated events
    // The *** EDD Driver *** is responsible for freeing the memory
    virtual KERESULT    OnEvents(IEDDDriver*, int n, EDDEvent* pEvents) = 0;

    // EDD Driver calls this member to send device generated alarms
    // Note:  to associate an alarm with a file upload, use the IdynAlarmProcessor 
    //        interface directly.  See alarm.h
    virtual KERESULT    StartAlarm(IEDDDriver*, TCHAR* name, TCHAR* message, int severity, int state, ALARM_HANDLE& alarmID) = 0;

	// EDD Driver calls this member to change alarm severity
	virtual KERESULT	ChangeAlarmSeverity(ALARM_HANDLE alarmID, int severity) = 0;

	// EDD Driver calls this member to change alarm state: Set it ACTIVE or NONACTIVE
	virtual KERESULT	SetAlarmActive(ALARM_HANDLE alarmID, bool state) = 0;

	// EDD Driver calls this member to acknowledge alarm
	virtual KERESULT	AcknowledgeAlarm(ALARM_HANDLE alarmID) = 0;

	// EDD Drvier calls thim member to end an alarm
	// Ending an alarm is setting the alarm state as INACTIVE and ACKED
	// This will cause the alarm to removed from the alarm system
	virtual KERESULT	EndAlarm(ALARM_HANDLE alarmID) = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// IEDDDriver is the communication interface from Connector to the driver
//
// Every EDD Driver should implement a class devired from IEDDDriver Interface
//
/////////////////////////////////////////////////////////////////////////////
interface IEDDDriver
{
    // Connector calls this member once on program start
    virtual KERESULT    StartDriver() = 0;

    // Connector calls this member once on program stop
    virtual KERESULT    StopDriver() = 0;

    // Connector calls this member to write tag values
    // The Connector side is responsible of freeing up the memory
    virtual KERESULT    WriteTagValues(int n, EDDHANDLE*, CDataValue*, KERESULT* pResults) = 0;

    // Connector calls this member to read tag values
    // The Connector side is responsible of allocating and freeing up the memory
    virtual KERESULT    ReadTagValues(int n, EDDHANDLE*, CDataValue*) = 0;

    // Connector calls this member to add a tag into the data changes
    virtual KERESULT    AddItem(EDDHANDLE, const EString&, int pushback) = 0;

    // Connector calls this member to remove a tag from the data changes
    virtual KERESULT    RemoveItem(EDDHANDLE) = 0;

    // Connector calls this member to acknowledge alarms sent by this device
    virtual KERESULT    Acknowledge(ALARM_HANDLE alarmID) = 0;

	// Called to destroy the derived EDD Driver object
	virtual KERESULT    Destroy() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// Extensible device information structure.
// This is used with GetDeviceInfo() to get IP Address, user name, etc.
struct EDDDeviceInfo
{
    // Keep this int as first member
    int                 m_version;

    // Methods
    EDDDeviceInfo() : m_version(1) {}

    // name and value map
    EMapStringToString  m_NameValueMap;
};
///// supported names //////////////////////
#define DEVICE_INFO_IPADDRESS       _T("IPAddress")
#define DEVICE_INFO_USER            _T("UserName")
#define DEVICE_INFO_PASSWORD        _T("Password")

#define HOST_NAME                   _T("host")
#define SNMP_SYSTEM_OID             _T("systemOID")
#define SNMP_SYSTEM_NAME            _T("systemName")
#define COMMUNITY_NAME              _T("community")



/////////////////////////////////////////////////////////////////////////////
//
// IEDDDeviceInfo is the interface to query for device information,
//                  such as IP Address, User name, Password, etc
//
/////////////////////////////////////////////////////////////////////////////
interface IEDDDeviceInfo
{
    virtual KERESULT GetDeviceInfo(IEDDDriver* pDriverInstance, EDDDeviceInfo& DeviceInfo) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// Extensible driver information structure.
// This is used with CreateEDDDriver to pass additional information to each
// driver instance.
// This is a "versioned" structure to allow extensibility in the future.
struct EDDDriverInfo
{
    int         m_version;
    EString     m_esMn;
    EString     m_esSn;
    EString     m_esOwner;

    // ADD NEW VARIABLES TO THE BOTTOM OF THE LIST OF VARIABLE PER VERSION


    // Methods
    EDDDriverInfo() : m_version(1) {;}

};

/////////////////////////////////////////////////////////////////////////////
//
// The IEDDManagerEx implementation manages the EDD driver as a whole
//
/////////////////////////////////////////////////////////////////////////////
interface IEDDManagerEx : public IKernelBase
{
    // Called to created an EDD driver
    virtual KERESULT CreateEDDDriver(IEDDCallback*, TCHAR* pID, IEDDDriver*&,
        EDDDriverInfo* pEDDDriverInfo = NULL) = 0;

	// Called to destroy the derived Manager object
	virtual KERESULT    Destroy() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// Every EDD Driver should export an initialization function
// with the name EDD_XXX_Ex_Initialize() where XXX is the name of the driver
// library (under Windows the DLL name).
//
// ****************************************************************************
//
//  IEDDManager is still supported, but discouraged from using
//
//  If you are re-compiling your existing EDD driver,
//  you are recommended to use the IEDDManagerEx, following simple steps.

//   1.  RENAME your exported function from EDD_XXX_Intialize to
//       EDD_XXX_Ex_Intialize, where XXX is the name of your EDD driver library (DLL).
//
//   2.  RENAME IEDDManager to IEDDManagerEx.
//       This is done where you define your manager class which is derived from
//       IEDDManager.
//
//   3.  MODIFY your functions that CALL the following methods on the IEDDCallback
//       interface.
//       A.  OnDataChange
//       B.  OnEvents
//
//       It is now the responsibility of the EDD Driver to free the allocated memory.
//       Specifically, after your function calls the OnDataChange method, you
//       now must free the EDDHANDLE and CDataValue arrays.
//       And after your function calls the OnEvents method, you now must to free
//       the EDDEvent array.
//
//   4.  It is suggested that you define the following BEFORE including the file "os.h"
//
//       #define _IGNORE_CONNECTOR_MEMORY_MANAGER_
//
// ****************************************************************************
//
//  If you are compiling your EDD Driver using Microsoft Visual Studio 2005,
//  please see SDK documentation for details regarding explict compiler options
//  that must be defined.
//
// ****************************************************************************

/////////////////////////////////////////////////////////////////////////////
typedef IEDDManagerEx* (*EDD_INIT_FUNCTION_EX)(IKernelBase* pKernel, TCHAR* pIdentifier);

#endif // __EDD_COMMON_H__
