//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDDataItem.h
//
//  Subsystem  :  EDD Driver
//
//  Description:
//                See EDDMgr.cpp for overview
//
//  Classes    :
//                EDDDataItem
//                EDDDataItems
//
//*****************************************************************************

#ifndef __EDD_DATA_ITEM_H__
#define __EDD_DATA_ITEM_H__

//*****************************************************************************
class EDDDataItem
{

public:
    static EDDDataItem* Create(TCHAR* pszName, CDataValue::ValueType valueType = CDataValue::ANALOG,
        bool bReadOnly = false);
    static EDDDataItem* Create(EString& esName, CDataValue::ValueType valueType = CDataValue::ANALOG,
        bool bReadOnly = false);
    virtual void Destroy();
    virtual void Enable(EDDHANDLE handle, bool bPush);
    virtual void Disable() {m_bActive = false;}
    virtual bool IsDirty() {return m_bDirty; }
    virtual void ClearDirty() {m_bDirty = false; }
    virtual void SetDirty() {m_bDirty = true; }
    virtual bool IsActive() {return m_bActive; }
    virtual bool IsPushEnabled() {return m_bPush; }
    virtual bool IsReadOnly() {return m_bReadOnly; }
    virtual CDataValue::ValueType GetType() { return m_value.GetType(); }
    virtual void SetType(CDataValue::ValueType type) { m_value.SetType(type); }
    virtual CDataValue& GetValue() {return m_value; }
    virtual EString& GetName() {return m_esName; }
    virtual KERESULT SetValue(CDataValue& value);  // over-ride for actual setting of value to hardware.
    virtual void SetTimestamp(ETimeStamp& timestamp) {m_value.SetTimestamp(timestamp); }
    virtual void SetTimestamp(ETime& rTime) {m_value.SetTimestamp(rTime.TimeStamp()); }
    virtual void SetQuality(CDataValue::Quality quality) {m_value.SetQuality(quality); }
    virtual EDDHANDLE GetHandle() { return m_handle; }
    virtual KERESULT DefineValues(EStringArray& values);
    virtual void SetValue(char* pszValue);
    virtual void SetValue(TCHAR* pszValue);
    virtual void SetValue(EString& esValue);
    virtual void SetValue(bool bValue);
    virtual void SetValue(double value);
    virtual void SetValue(int value);

    // Helper functions.
    void SetTimeAndQuality();       // sets to current time and good quality

    virtual void Acquire();

protected:
    // methods
    EDDDataItem(EString& name, CDataValue::ValueType valueType, bool bReadOnly = false);
    virtual ~EDDDataItem();


    // properties
    EString             m_esName;
    CDataValue          m_value;            // latest value.
    bool                m_bDirty;           // see if block change has yet been published.
    EDDHANDLE           m_handle;           // Saved handle passed in from the Agent
    bool                m_bPush;            // true if Driver publishes data changes.
    bool                m_bReadOnly;
    bool                m_bActive;          // true if the item is to be published.

};

//*****************************************************************************
// Collection of EDDDataItem pointers.
class EDDDataItems : public ETypedPtrList<EPtrList, EDDDataItem*>
{
public:
    ~EDDDataItems();
    void DeleteAll();
    EDDDataItem* Find(EDDHANDLE handle);
    EDDDataItem* Find(EString& esName);
};


#endif // __EDD_DATA_ITEM_H__

