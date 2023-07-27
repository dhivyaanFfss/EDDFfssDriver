//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDConfig.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//  Classes    :  
//                EDDConfig
//
//*****************************************************************************

#ifndef __EDD_CONFIG_H__
#define __EDD_CONFIG_H__
#include "json.hpp"


// Global function prototypes
extern bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, EString& esValue);
extern bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, int& iValue);
extern bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, bool& bValue);
extern bool GetBoolValue(TCHAR* pszValue);

//*****************************************************************************
// XML defines
static TCHAR* OUTER_ELEMENT = _T("EDDExample");
static TCHAR* ACQUISITION_RATE_ELEMENT = _T("AcquisitionRate");
static TCHAR* DATA_ITEMS_ELEMENT = _T("DataItems");
static TCHAR* DATA_ITEM_ELEMENT = _T("DataItem");
static TCHAR* VALUES_ELEMENT = _T("Values");
static TCHAR* VALUE_ELEMENT = _T("Value");
static TCHAR* EVENTS_ELEMENT = _T("Events");
static TCHAR* EVENT_ELEMENT = _T("Event");
static TCHAR* ALARMS_ELEMENT = _T("Alarms");
static TCHAR* ALARM_ELEMENT = _T("Alarm");
static TCHAR* STATE_ELEMENT = _T("State");

// Attributes

// Data Item attributes
static TCHAR* NAME_ATTRIB = _T("name");  // also used with Event and Alarm
static TCHAR* TYPE_ATTRIB = _T("type");
static TCHAR* READ_ONLY_ATTRIB = _T("readOnly");
static TCHAR* DEFAULT_ATTRIB = _T("default");

// Event and Alarm attributes
static TCHAR* DESCRIPTION_ATTRIB = _T("description");
static TCHAR* SEVERITY_ATTRIB = _T("severity");
static TCHAR* DATA_ITEM_ATTRIB = _T("dataItem");
static TCHAR* TRIGGER_VALUE_ATTRIB = _T("triggerValue");

// Alarm attributes.
static TCHAR* ACTIVE_ATTRIB = _T("");

// attribute values
//   type values:
static TCHAR* ANALOG_STR = _T("analog");
static TCHAR* DIGITAL_STR = _T("digital");
static TCHAR* STRING_STR = _T("string");

//   writable values:
static TCHAR* TRUE_STR = _T("true");
static TCHAR* FALSE_STR = _T("true");

// forward reference
class EDDDriver;
class EDDBrowser;

//*****************************************************************************
class EDDConfig //: public ISax
{
public:
    EDDConfig(EDDDataItems& items);
    ~EDDConfig();

    // ISax Interface
    //KERESULT   OnElement(ESaxElement* pElement);


    KERESULT Init();
    KERESULT Update(std::unordered_map<std::string, std::string>& dataItemsMap);


protected:
    // Support functions
    KERESULT ConfigDataItem(ESaxElement* pElement, /* out */ EDDDataItem** ppItem);
    KERESULT SetDefaultValue(ESaxElement* pElement, EDDDataItem* pItem);

    // members
    bool                m_bValidated;

    // collection of 'value' strings as read from the configuration file per data item.
    EStringArray        m_values;

    // Saved reference to collection of items from either driver or browser.
    EDDDataItems&      m_items;
    
};

#endif // __EDD_CONFIG_H__
