//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDConfig.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Configures a collection of data items by parsing an 
//                xml configuration file.
//
//
//  Classes:
//                EDDConfig


// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"
#include "string"
#include <fstream>
#include <unordered_map>

constexpr const wchar_t* META_DATA_FILE_NAME = L"deviceMetaData.json";
constexpr const char* META_DATA_KEY_IDENTIFIER = "Device_Data";
constexpr const char* META_DATA_ITEM_NAME_IDENTIFIER = "Name";
constexpr const char* META_DATA_ITEM_VALUE_IDENTIFIER = "Value";
constexpr const char* META_DATA_ITEM_TYPE_IDENTIFIER = "Type";
constexpr const char* META_DATA_ITEM_WRITEABLE_IDENTIFIER = "Writeable";

//*****************************************************************************
EDDConfig::EDDConfig(EDDDataItems& items)
: m_bValidated(false), m_items(items)
{
}

//*****************************************************************************
EDDConfig::~EDDConfig()
{
}

//*****************************************************************************
KERESULT EDDConfig::Init()
{
    // Read and parse configuration json file from root directory.
    try
    {
        TCHAR currentDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, currentDir);
        std::wstring metaDataFilePath = currentDir;
        metaDataFilePath += L"\\";
        metaDataFilePath += META_DATA_FILE_NAME;

        nlohmann::json inputFile;
        std::ifstream fileReader(metaDataFilePath);
        fileReader >> inputFile;
        fileReader.close();

        nlohmann::json deviceDataJson;
        deviceDataJson = inputFile[META_DATA_KEY_IDENTIFIER];

        for (nlohmann::json& dataItem : deviceDataJson)
        {

            std::string dataItemName = dataItem[META_DATA_ITEM_NAME_IDENTIFIER].get<std::string>();
            std::string dataItemValue = dataItem[META_DATA_ITEM_VALUE_IDENTIFIER].get<std::string>();
            std::string dataItemType = dataItem[META_DATA_ITEM_TYPE_IDENTIFIER].get<std::string>();
            std::string DataItemWriteableStr = dataItem[META_DATA_ITEM_WRITEABLE_IDENTIFIER].get<std::string>();
            bool IsDataItemReadOnly = DataItemWriteableStr == "no" ? true : false;

            EDDDataItem* pItem = NULL;
            EString esName;
            EString esType;
            EString esValue;

            // Set defaults for writable and value type.
            bool bReadOnly = IsDataItemReadOnly;
            CDataValue::ValueType   valueType = CDataValue::STRING;

            if (dataItemType == "boolean")
            {
                valueType = CDataValue::DIGITAL;
            }
            else if (dataItemType == "number")
            {
                valueType = CDataValue::ANALOG;
            }

            bool valid = true;
            esName = EString(dataItemName.c_str());
            pItem = EDDDataItem::Create(esName, valueType, bReadOnly);
            esValue = EString(dataItemValue.c_str());
            pItem->SetValue(esValue);
            ETimeStamp ts;
            ETime::GetTimeStamp(&ts);
            pItem->SetTimestamp(ts);
            pItem->SetQuality(CDataValue::GOOD);

            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            // Save defined data items
            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            m_items.AddTail(pItem);

        }
    }
    catch (nlohmann::json::exception& e)
    {
        return KE_FAILED;
    }

    return KE_OK;
}

//*****************************************************************************
KERESULT EDDConfig::Update(std::unordered_map<std::string, std::string>& dataItemsMap)
{
    // Read and parse configuration json file from root directory.
    try
    {
        TCHAR currentDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, currentDir);
        std::wstring metaDataFilePath = currentDir;
        metaDataFilePath += L"\\";
        metaDataFilePath += META_DATA_FILE_NAME;

        nlohmann::json inputFile;
        std::ifstream fileReader(metaDataFilePath);
        fileReader >> inputFile;
        fileReader.close();

        nlohmann::json deviceDataJson;
        deviceDataJson = inputFile[META_DATA_KEY_IDENTIFIER];

        // First read all values
        for (nlohmann::json& dataItem : deviceDataJson)
        {

            std::string dataItemName = dataItem[META_DATA_ITEM_NAME_IDENTIFIER].get<std::string>();
            std::string dataItemValue = dataItem[META_DATA_ITEM_VALUE_IDENTIFIER].get<std::string>();
            std::string dataItemType = dataItem[META_DATA_ITEM_TYPE_IDENTIFIER].get<std::string>();
            std::string DataItemWriteableStr = dataItem[META_DATA_ITEM_WRITEABLE_IDENTIFIER].get<std::string>();
            bool IsDataItemReadOnly = DataItemWriteableStr == "no" ? true : false;

            if (IsDataItemReadOnly)
            {
                dataItemsMap[dataItemName] = dataItemValue;
            }
        }
    }
    catch (nlohmann::json::exception& e)
    {
        return KE_FAILED;
    }

    return KE_OK;
}


/*
//*****************************************************************************
//                           ISax   Implementation
//*****************************************************************************

//*****************************************************************************
// Example of file being read.
// Note:  The outer element must be the same as the name of the EDD Driver DLL.
// 
// <?xml version="1.0" standalone="yes"?>
// <EDDExample>
//   <!-- rate at which data will be acquired in seconds -->
//   <AcquisitionRate>1</AcquisitionRate>
//   <DataItems>
//     <!-- Data Item definition
//           name:  name of data item
//           type:  analog | digital | string   Optional:  default is analog
//           readOnly:   true | false   Optional:  default is false.
//           default:  default value of item.  Optional.
//     -->
//     <DataItem name="String1" type="string" readOnly="false" default="hello" />
//     <DataItem name="StringArray1" type="string" default="hello">
//       <!-- Array of values required when name contains predefined type: "array".  
// 	       Values read individually and sequentially at the AquisitionRate.  
// 		   Automatically wraps at end.
//       -->      
//       <Values>
//         <Value>This</Value>
//         <Value>is</Value>
//         <Value>an</Value>
//         <Value>array</Value>
//         <Value>of</Value>
//         <Value>strings</Value>
//       </Values>
//     </DataItem>
//     <DataItem name="sine" default="0.0" />
//     <DataItem name="wave" />
//     <DataItem name="dig5" />
//     <DataItem name="toggle" type="digital"/>
//     <DataItem name="dig1" type="digital"/>
//     <DataItem name="random" />
//     <DataItem name="ramp" />
//   </DataItems>
// </EDDExample>
//
KERESULT EDDConfig::OnElement(ESaxElement* pElement)
{
    KERESULT ker = KE_OK;

    // Make sure valid configuration file for this DLL.
    if (!m_bValidated)            
    {
        if (pElement->GetRoot()->m_esName != OUTER_ELEMENT)
            return KE_BAD_XML_FORMAT;
        m_bValidated = true;
    }

    // Read <AcquisitionRate> element
    if (pElement->m_esName == ACQUISITION_RATE_ELEMENT)
    {
        // get acquisition rate in seconds
        if (pElement->m_esBody.IsEmpty())
            return KE_BAD_XML_FORMAT;
        DWORD acqRate = _tcstoul(pElement->m_esBody.GetPtr(), NULL, 10);
        g_pEDDMgr->SetAcquisitionRate(acqRate * 1000);
    }

    // For each <DataItem> have the ConfigDataItem() function process its sub-elements.
    else if (pElement->m_esName == DATA_ITEM_ELEMENT)
    {

        EDDDataItem* pItem = NULL;
        ker = ConfigDataItem(pElement, &pItem);  // creates data item.
        if (ker != KE_OK)
        {                                                                            
            return ker;
        }

        // define item's array values.  Only valid for Item of type ARRAY.
        pItem->DefineValues(m_values);
        m_values.RemoveAll();

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // Save defined data items
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        m_items.AddTail(pItem);
    }                                                    

    // Use a member property to tempoarily store the values of an array.
    // Values stored as strings, and converted later.
    else if (pElement->m_esName == VALUE_ELEMENT)
    {
        m_values.Add(pElement->m_esBody);
    }

    return ker;
}

//*****************************************************************************
//                        Support   Functions
//*****************************************************************************

//*****************************************************************************
KERESULT EDDConfig::ConfigDataItem(ESaxElement* pElement, EDDDataItem** ppItem)
{
    EString                 esName;
    EString                 esType;

    // Set defaults for writable and value type.
    bool                    bReadOnly = false;
    CDataValue::ValueType   valueType = CDataValue::ANALOG;

    // Get data item attributes:  name, type, writable.
    if (!GetAttributeValue(pElement, NAME_ATTRIB, esName))
        return KE_BAD_XML_FORMAT;
    // optional attributes:
    GetAttributeValue(pElement, TYPE_ATTRIB, esType);
    GetAttributeValue(pElement, READ_ONLY_ATTRIB, bReadOnly);

    // Convert value type
    if (!esType.IsEmpty())
    {
        if (esType == ANALOG_STR)
            valueType = CDataValue::ANALOG;
        else if (esType == DIGITAL_STR)
            valueType = CDataValue::DIGITAL;
        else if (esType == STRING_STR)
            valueType = CDataValue::STRING;
        else                                 
            return KE_BAD_XML_FORMAT;
    }

    // Check if name is a pre-defined "simulation" type.
    SimulatedDataItem::SIMTYPE simType = SimulatedDataItem::GetSimulationType(esName);
                                     
    // Define data item
    EDDDataItem* pItem = NULL;
    if (simType != SimulatedDataItem::_UNDEFINED)
    {
        pItem = SimulatedDataItem::Create(esName, valueType, bReadOnly, simType);
    }
    else                             
    {
        pItem = EDDDataItem::Create(esName, valueType, bReadOnly); 
    }

    KERESULT ker = SetDefaultValue(pElement, pItem);
    if (ker != KE_OK)
    {
        pItem->Destroy();
        return ker;
    }
    
    *ppItem = pItem;       // set output parameter.

    return KE_OK;
}
*/

//*****************************************************************************
KERESULT EDDConfig::SetDefaultValue(ESaxElement* pElement, EDDDataItem* pItem)
{
    // Get optional default value;
    EString esDefaultValue;
    if (!GetAttributeValue(pElement, DEFAULT_ATTRIB, esDefaultValue)) 
        return KE_OK;
    
    switch(pItem->GetValue().GetType())
    {
    case CDataValue::ANALOG:
        {
            if (esDefaultValue.IsEmpty())
                return KE_BAD_XML_FORMAT;
            TCHAR*  pszEnd = NULL;
            // ignore value at this time.
            double defaultValue = _tcstod(esDefaultValue.GetPtr(), &pszEnd);    
            // check that entire string is a valid double value.
            if (pszEnd < esDefaultValue.GetPtr() + esDefaultValue.GetLength())
                return KE_BAD_XML_FORMAT;
            
            pItem->SetValue(defaultValue);
            break;
        }
    case CDataValue::STRING:
        pItem->SetValue(esDefaultValue);
        break;
    case CDataValue::DIGITAL:
        {
            bool bValue;
            GetAttributeValue(pElement, DEFAULT_ATTRIB, bValue);
            pItem->SetValue(bValue);
            break;
        }
    }
    
    // make value valid by timestamp and quality
    ETimeStamp ts;
    ETime::GetTimeStamp(&ts);
    pItem->SetTimestamp(ts);

    pItem->SetQuality(CDataValue::GOOD);

    return KE_OK;
}

//**************************************************************************
//   Global Helper Functions.
//**************************************************************************

//**************************************************************************
bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName,
                                  EString& esValue)
{
    TCHAR *pszValue = pElement->GetAttributeValue(pszName);
    if (!pszValue)
        return false;

    esValue = pszValue;

    return true;
}

//**************************************************************************
bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, int& iValue)
{
    TCHAR *pszValue = pElement->GetAttributeValue(pszName);
    if (!pszValue)
        return false;

    TCHAR *pEnd = NULL;
    iValue = _tcstol(pszValue, &pEnd, 10);
    if (pEnd == pszValue)
        return false;

    return true;
}

//**************************************************************************
bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, DWORD& iValue)
{
    TCHAR *pszValue = pElement->GetAttributeValue(pszName);
    if (!pszValue)
        return false;

    TCHAR *pEnd = NULL;
    iValue = _tcstoul(pszValue, &pEnd, 10);
    if (pEnd == pszValue)
        return false;

    return true;
}

//**************************************************************************
bool GetAttributeValue(ESaxElement* pElement, TCHAR* pszName, bool& bValue)
{
    TCHAR *pszValue = pElement->GetAttributeValue(pszName);
    if (!pszValue)
        return false;

    bValue = GetBoolValue(pszValue);
    return true;
}

//**************************************************************************
bool GetBoolValue(TCHAR* pszValue)
{
    bool bValue = false;
    if (!pszValue)
        return bValue;

    if (!_tcsicmp(pszValue, _T("true")) ||
        !_tcsicmp(pszValue, _T("yes")) ||
        !_tcsicmp(pszValue, _T("on")))
        bValue = true;
    else if (!_tcsicmp(pszValue, _T("false")) ||
        !_tcsicmp(pszValue, _T("no")) ||
        !_tcsicmp(pszValue, _T("off")))
        bValue = false;
    else
    {
        TCHAR *pEnd = NULL;
        long iValue = _tcstol(pszValue, &pEnd, 10);
        if (pEnd == pszValue)
            return false;

        bValue  = (iValue != 0);
    }

    return bValue;
}