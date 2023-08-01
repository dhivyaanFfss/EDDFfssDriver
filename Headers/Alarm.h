//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  Alarm.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Alarm header file.  General enumerations and structure 
//                definitions
//
//  Classes:  None
//
//**************************************************************************

#ifndef __ALARM_H__
#define __ALARM_H__

#include "KernelInterface.h"
#include "DataValue.h"
#include "ENotifyInterface.h"

/* Group=Alarms */

enum Condition
{
  LEVEL = 1, DEVIATION, CUSTOM, DIGITAL
};

enum SubCondition
{
  NONE_, LO, HI, LO_LO, HI_HI, DV_LO, DV_HI, DIG_LO, DIG_HI, CUSTOM_COND
};


// Remarks These bits are used by the ONEVENTSTRUCT wNewState variable.
enum StateMask      
{
  CLEARED = 0, ENABLED = 1, ACTIVE = 2, ACKED = 4
};


// Remarks  This is a handle to an alarm filter.
// See Also IAlarmFilter::SetFilter, IAlarmFilter::RemoveFilter, IAlarmFilter::GetFilter, 
// IAlarmFilter::Subscribe, IAlarmFilter::Unsubscribe.           
typedef LONG_PTR HFILTER;


// Remarks This is a unique identifier sent with an alarm notification.  
// See Also CondEventStruct, IDynAlarm::Acknowledge
struct HEvent 
{
  ULONG_PTR        nEventId;
  ETimeStamp       activeTime;
};

struct CondEventStruct
{
private:
    ~CondEventStruct();
public:    
    CondEventStruct();
    void AddRef();
    void Release();
    void SetCustomAlarm(CondEventStruct* pCustomAlarm);
    CondEventStruct* GetCustomAlarm();

    int             nState;
    int             nMask;
    EString         strDescription;
    EString         strArea;
    EString         strDataItem;
    Condition       enCondition;
    SubCondition    enSubCondition;
    long            nCategory;
    int             nSeverity;
    CDataValue      triggerData;    // Data Value when trigger event happens
    HEvent          Cookie;
    ETimeStamp       time;
#ifdef __GATEWAY__
    int             m_gatewayId;    // Gateway only:  need id to pass around.
#endif
    EString			m_strCondition;
    EString			m_strSubCondition;
    bool			m_bNoDataItem;
protected:
    DWORD           m_refCount;    
    // used only when custom handler hooks & modifies alarm
    CondEventStruct* m_pCustomAlarm;   
};

//**************************************************************************
//  CEventBuf
//**************************************************************************
class CEventBuf : public ETypedPtrArray<EPtrArray, CondEventStruct*>
{
public:
    CEventBuf()		{}
    ~CEventBuf()
	{
	    int sz = GetSize();
		for (int i = 0; i < sz; i++)
		{
			CondEventStruct* pCondEvent = GetAt(i);
			pCondEvent->Release();
		}
	    RemoveAll();
	}
};



// bit masks for m_wChangeMask
#define CHANGE_NO_STATE				0x0000
#define CHANGE_ACTIVE_STATE			0x0001
#define CHANGE_ACK_STATE			0x0002
#define CHANGE_ENABLE_STATE			0x0004
#define CHANGE_QUALITY				0x0008
#define CHANGE_SEVERITY				0x0010
#define CHANGE_SUBCONDITION			0x0020
#define CHANGE_MESSAGE				0x0040
#define CHANGE_ATTRIBUTE			0x0080
#define CHANGE_ALL_STATE			0x00FF


/////////////////////////////////////////////////////////////////////////////
//
// Dynamic alarm interfaces
//
/////////////////////////////////////////////////////////////////////////////

////// NOTE: dynamic alarm IDs must be NON zero /////////////////////////////
#define INVALID_DYNALARM_ID		0
#define DYNALARM_CATEGORY		10

typedef EHANDLE ALARM_HANDLE;

interface IdynAlarmSource
{
	virtual KERESULT 	Acknowledge(ALARM_HANDLE alarmID) = 0;
};

interface IdynAlarmProcessor
{
	virtual KERESULT	StartDynamicAlarm(EONEVENTSTRUCT* pEvent, IdynAlarmSource* pSource, ALARM_HANDLE& alarmID) = 0;
	virtual KERESULT	ChangeDynamicAlarm(ALARM_HANDLE alarmID, EONEVENTSTRUCT* pEvent) = 0;
	virtual KERESULT	AcknowledgeDynamicAlarm(ALARM_HANDLE alarmID) = 0;
	virtual KERESULT	EndAlarm(ALARM_HANDLE alarmID) = 0;
    virtual KERESULT    GetAlarmHandle(/* in */ TCHAR* pszAlarmName, /* out */ ALARM_HANDLE& alarmID) = 0;
};

#endif

