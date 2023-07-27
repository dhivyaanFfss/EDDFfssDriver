//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDBrowser.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Implements EDD driver capablity to be browsed from
//                DRM Builder.
//                The EDDExample.xml is parsed to define the data items that are
//                being managed by the driver.  The data items and their attributes
//                are then browsed by Builder using the BrowseItems method.
//
//                The EDDBrowser object is used by the exported 'C' functions called
//                by Builder.  See EDDExports.cpp
//                
//
//  Classes:  
//                EDDBrowser

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"
#include "string"
#include <fstream>

constexpr const char* META_DATA_FILE_PATH = "D:\\PTC\\deviceMetaData.json";
constexpr const char* META_DATA_KEY_IDENTIFIER = "Device_Data";
constexpr const char* META_DATA_ITEM_NAME_IDENTIFIER = "Name";
constexpr const char* META_DATA_ITEM_VALUE_IDENTIFIER = "Value";
constexpr const char* META_DATA_ITEM_TYPE_IDENTIFIER = "Type";

//*****************************************************************************
EDDBrowser::EDDBrowser() :
m_json({}),
m_deviceDataJson({})
{
}

//*****************************************************************************
EDDBrowser::~EDDBrowser()
{
    m_items.DeleteAll();
}

//*****************************************************************************
KERESULT EDDBrowser::Init()
{
    // Configure driver's data items.

    //for (int metaData = 0; metaData < FfssMetaDataId::EOL; metaData++)
    //{
    //    EDDDataItem* pItem = NULL;
    //    EString esName;
    //    EString esType;

    //    // Set defaults for writable and value type.
    //    bool bReadOnly = false;
    //    CDataValue::ValueType   valueType = CDataValue::STRING;
    //    bool valid = true;
    //    std::string str;

    //    switch (metaData)
    //    {
    //    case(PCBA_SERIAL_NUMBER):
    //    {
    //        str = "SerialNumber";
    //        esName = EString(str.c_str());
    //        break;
    //    }
    //    case(SOFTWARE_BOM_VERSION):
    //    {
    //        str = "SoftwareBomVersion";
    //        esName = EString(str.c_str());
    //        break;
    //    }
    //    case(PRODUCT_TYPE):
    //    {
    //        str = "ProductType";
    //        esName = EString(str.c_str());
    //        break;
    //    }
    //    default:
    //        valid = false;
    //        break;
    //    }

    //    if (valid)
    //    {
    //        pItem = EDDDataItem::Create(esName, valueType, bReadOnly);
    //        pItem->SetValue("Ffss_default");
    //        // make value valid by timestamp and quality
    //        ETimeStamp ts;
    //        ETime::GetTimeStamp(&ts);
    //        pItem->SetTimestamp(ts);
    //        //pItem->

    //        pItem->SetQuality(CDataValue::GOOD);

    //        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //        // Save defined data items
    //        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //        m_items.AddTail(pItem);
    //        //pItem->SetValue("example");
    //        //Add(pItem);
    //    }
    //}

    EDDConfig config(m_items);
    KERESULT ker = config.Init();

    return ker;
}

//*****************************************************************************
// return the number of data items managed by the driver, their names and attributes.
// See DriverCommon.h for attribute definitions.
//
// Note:  If the data items are correctly defined, this routine should not need to 
//        be modified.
//
DRIVER_RESULT EDDBrowser::BrowseItems(DWORD& dwCount, TCHAR**& ppszItems, 
                                             DWORD*& pdwAttributes)
{
    // default output variables in case of error.
    ppszItems = NULL;
    pdwAttributes = NULL;

    // verify that there are data items to browse.
    dwCount = m_items.GetCount();
    if (dwCount == 0)
        return DRIVER_FAILED;
    
    // initialize output variable size.
    ppszItems = new TCHAR*[dwCount];
    pdwAttributes = new DWORD[dwCount];
    if(!ppszItems || !pdwAttributes)
        return DRIVER_MEMORY;

    // For each data item, set the name and its attributes.
    DWORD ix = 0;
    POSITION pos = m_items.GetHeadPosition();
    while (pos)
    {
        EDDDataItem* pItem = m_items.GetNext(pos);
        // define item name
        int len = pItem->GetName().GetLength();
        ppszItems[ix] = new TCHAR[len + 1];
        memset(ppszItems[ix], 0, (len + 1) * sizeof(TCHAR));
        _tcsncpy(ppszItems[ix], pItem->GetName().GetPtr(), len);

        // define item attributes.
        DWORD attrib = DRIVER_FLAG_READABLE;        // assume all items are readable
        if (!pItem->IsReadOnly())
            attrib |= DRIVER_FLAG_WRITEABLE;
        switch (pItem->GetType())
        {
        case CDataValue::ANALOG:
            attrib |= DRIVER_FLAG_ANALOG;
            break;
        case CDataValue::DIGITAL:
            attrib |= DRIVER_FLAG_DIGITAL;
            break;
        case CDataValue::STRING:
            attrib |= DRIVER_FLAG_STRING;
            break;
        }
        
        pdwAttributes[ix] = attrib;
        ix++;
    }
    ASSERT(ix == dwCount);
    
    return DRIVER_OK;
}
