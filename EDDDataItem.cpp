//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDDataItem.cpp
//
//  Subsystem  :  EDD Driver
//
//  Description:

/*
                See EDDMgr.cpp for overview


  - EDDDataItem:  Standard EDD driver class to define and access a named data item.
    The EDDDataItem defines the name, value, and type of the data item. A collection
    of data items is maintained by each EDDDriver object.
    
*/


//                General purpose Data Item class.  Used to define data item and
//                Set / Get it properties.
//
//                Code Developer can derive a class from EDDDataItem for customer
//                specific functionality.
//
//  Classes:
//              EDDDataItem
//              EDDDataItems


// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"


//*****************************************************************************
EDDDataItem::EDDDataItem(EString& name, CDataValue::ValueType valueType, bool bReadOnly)
 : m_esName(name), m_bReadOnly(bReadOnly),
   m_handle(NULL), m_bPush(true), m_bDirty(false), m_bActive(false)
{
    m_value.SetType(valueType);
}


//*****************************************************************************
EDDDataItem::~EDDDataItem()
{
}


//*****************************************************************************
EDDDataItem* EDDDataItem::Create(TCHAR* pszName,CDataValue::ValueType valueType,
                                  bool bReadOnly)
{
    EString esName(pszName);
    return new EDDDataItem(esName, valueType, bReadOnly);
}

//*****************************************************************************
EDDDataItem* EDDDataItem::Create(EString& esName,CDataValue::ValueType valueType,
                                  bool bReadOnly)
{
    return new EDDDataItem(esName, valueType, bReadOnly);
}

//*****************************************************************************
void EDDDataItem::Destroy()
{
    delete this;
}

//*****************************************************************************
// Enables data item so it is ready to be scanned.
void EDDDataItem::Enable(EDDHANDLE handle, bool bPush)
{
    m_handle = handle;
    m_bPush = bPush;
    m_bActive = true;
}

//*****************************************************************************
KERESULT EDDDataItem::SetValue(CDataValue& value)
{
    // Set only at change.
    if (m_value.IsNumeric() 
        && m_value.GetQuality() == value.GetQuality()
        && m_value.GetNumeric() == value.GetNumeric())
        return KE_OK;
    if (m_value.IsString() && !m_value.IsDataInvalid())
    {
        EString esCurrent(m_value.GetStringValue());
        if (!value.IsDataInvalid() && esCurrent == value.GetStringValue())
            return KE_OK;
    }

    if (value.IsDataInvalid())
        return KE_INVALID_PARAMS;

    m_value = value;
    m_bDirty = true;
    return KE_OK;
}

//*****************************************************************************
void EDDDataItem::SetValue(char* pszValue)
{
    EString esValue(pszValue);
    SetValue(esValue);

}
//*****************************************************************************
void EDDDataItem::SetValue(TCHAR* pszValue)
{
    EString esValue(pszValue);
    SetValue(esValue);
}

//*****************************************************************************
void EDDDataItem::SetValue(EString& value)
{
    if (m_value.GetType() == CDataValue::STRING)
    {
        // Set only at change.
        if (!m_value.IsDataInvalid() 
            && m_value.GetQuality() == CDataValue::GOOD
            && value == m_value.GetStringValue())
            return;
        m_value.SetString(value.GetPtr());
    }
    else if (m_value.GetType() == CDataValue::ANALOG)
    {
        TCHAR*  pszEnd = NULL;
        double dValue = _tcstod(value.GetPtr(), &pszEnd);
        // check that entire string is a valid double value.
        if (pszEnd < value.GetPtr() + value.GetLength())
        {
            ASSERT(0);
        }
        SetValue(dValue);

    }
    else if (m_value.GetType() == CDataValue::DIGITAL)
    {
        SetValue(GetBoolValue(value.GetPtr()));
    }
    m_bDirty = true;
}

//*****************************************************************************
void EDDDataItem::SetValue(bool bValue)
{
    // Set only at change;
    if (!m_value.IsDataInvalid())
    {
        bool bCurrent = m_value.GetNumeric() ? true : false;
        if (bCurrent == bValue && m_value.GetQuality() == CDataValue::GOOD)
            return;
    }

    m_value.SetNumeric((bValue) ? 1.0 : 0.0);
    m_bDirty = true;
}

//*****************************************************************************
void EDDDataItem::SetValue(double value)
{
    if (!m_value.IsDataInvalid() 
        && m_value.GetQuality() == CDataValue::GOOD
        && m_value.GetNumeric() == value)
        return;         // Set only at change.

    m_value.SetNumeric(value);
    m_bDirty = true;
}

//*****************************************************************************
void EDDDataItem::SetValue(int value)
{
    if (!m_value.IsDataInvalid() 
        && m_value.GetQuality() == CDataValue::GOOD
        && m_value.GetNumeric() == (double) value)
        return;         // Set only at change.

    m_value.SetNumeric((double) value);
    m_bDirty = true;
}

//*****************************************************************************
KERESULT EDDDataItem::DefineValues(EStringArray& values)
{
    // Requires sub-class to simulate data acquisition.
    if (values.GetSize() != 0)
    {
        // 7: Ignoring array defined for data item:  %s 
        g_pEDDMgr->CustomReport(ET_WARNING, 7, GetName());

    }
    return KE_NOT_IMPLEMENTED;
}

//*****************************************************************************
// Helper function.
void EDDDataItem::SetTimeAndQuality()
{
    // make value valid by timestamp and quality
    ETimeStamp ts;
    ETime::GetTimeStamp(&ts);
    SetTimestamp(ts);

    SetQuality(CDataValue::GOOD);
}

//*****************************************************************************
void EDDDataItem::Acquire()
{
    return;
}

//*****************************************************************************
//              Implementation of EDDDataItems
//*****************************************************************************

//*****************************************************************************
EDDDataItems::~EDDDataItems()
{
    DeleteAll();
}

//*****************************************************************************
void EDDDataItems::DeleteAll()
{
    POSITION pos = GetHeadPosition();
    while(pos)
    {
        EDDDataItem* pItem = GetNext(pos);
        pItem->Destroy();
    }
    RemoveAll();
}

//*****************************************************************************
EDDDataItem* EDDDataItems::Find(EDDHANDLE handle)
{
    POSITION pos = GetHeadPosition();
    while(pos)
    {
        EDDDataItem* pItem = GetNext(pos);
        if (pItem->GetHandle() == handle)
            return pItem;
    }
    return NULL;
}

//*****************************************************************************
EDDDataItem* EDDDataItems::Find(EString& esName)
{
    POSITION pos = GetHeadPosition();
    while(pos)
    {
        EDDDataItem* pItem = GetNext(pos);
        if (pItem->GetName() == esName)
            return pItem;
    }
    return NULL;
}


