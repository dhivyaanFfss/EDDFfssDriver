//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDDriver.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//  Classes    :  
//                EDDDriver
//
//*****************************************************************************

#ifndef __EDD_DRIVER_H__
#define __EDD_DRIVER_H__
#include "json.hpp"
#include "EDDAppInterface.h"


// Name of the file used to configure data items.
static const TCHAR* CONFIG_XML_NAME = _T("EDDExample.xml");

//*****************************************************************************
//  
// An EDD Driver must derive a class from IEDDDriver and implement all its members.
//
//*****************************************************************************
class EDDDriver : public IEDDDriver
{
public:
    // Constructor and Destructor
    EDDDriver(IEDDCallback* pCallbackInt, TCHAR* pIdentifier, 
        EDDDriverInfo* pEDDDriverInfo);

    // Destructor has to be virtual to be called
    virtual ~EDDDriver();

    // IEDDDriver members
    KERESULT    StartDriver();
    KERESULT    StopDriver();
    KERESULT    WriteTagValues(int n, EDDHANDLE*, CDataValue*, KERESULT* pResults);
    KERESULT    ReadTagValues(int n, EDDHANDLE*, CDataValue*);
    KERESULT    AddItem(EDDHANDLE, const EString&, int pushback);
    KERESULT    RemoveItem(EDDHANDLE handle);
	KERESULT    Acknowledge(ALARM_HANDLE alarmID);
	KERESULT    Destroy();

    // Example functions to acquire data and publish it.
    void Acquire();
    void MessageProcessor(std::string& messageFromApp);
    void Publish();
    void PublishData();
    
protected:
    // Support Functions
    bool IsItemPublishable(EDDDataItem* pItem);

    // Example functions
    KERESULT PostEvent(TCHAR* pszName, TCHAR* pszMessage,  int severity);
    KERESULT StartAlarm(TCHAR* pszName, TCHAR* pszMessage,  int severity);
    KERESULT SetAlarmState(bool bActive);
    KERESULT UploadFile(TCHAR* pszFileName, bool bDelete = false, TCHAR* pszHint = NULL);

    void PublishName();

    // Properties

    // IEDDCallback pointer to send out data changes
    IEDDCallback*   m_pIEDDCallback;

    EString         m_esConfig;
    EString         m_esMn;         // this driver object's model
    EString         m_esSn;         // this driver object's serial number
    EString         m_esOwner;      // required but unused.

    // Collection of data items
    EDDDataItems   m_items;
    EMutex         m_itemsMtx;
    nlohmann::json m_json;
    nlohmann::json m_deviceDataJson;

    // Helper Functions
    bool parseMessageFromApp(std::string cmd, std::vector<std::string>& cmdParameters);
    
    // Support for posting alarms.
    ALARM_HANDLE    m_hAlarm;
};

//*****************************************************************************
typedef ETypedPtrList<EPtrList, EDDDriver*>  EDDDriverList;


#endif // __EDD__DRIVER_H__

