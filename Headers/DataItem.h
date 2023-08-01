//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  DataItem.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Data Item
//
//  Classes:    
//              IGetItemType
//              IDataItem
//              IDataItemProvider
//              CListenerItem
//              CListenerItemsList
//              CDataItem
//              CDataItemList
//
//**************************************************************************
               
#ifndef __DATAITEM_H__
#define __DATAITEM_H__

#include "ECollections.h"
#include "EOS.h"

#include "ENotifyInterface.h"
#include "ETrigger.h"
#include "DynamicDataInterface.h"
#include "DataValue.h"

#ifdef WIN32
    #ifdef _DV_EXPORT
       #define _DV_DECLSPEC __declspec(dllexport)
       #define _DV_EXTERN                               
    #else
       #define _DV_DECLSPEC __declspec(dllimport)
       #define _DV_EXTERN   extern
    #endif
#else
   #define _DV_DECLSPEC
   #define _DV_EXTERN
#endif


// group=Data Items



////////////////////////////////////////////////////////////////////////
// IGetItemType - Support definitions 

/////////////////////////////////////////////////////////////////////////////
// special SNMP EDD Driver Tag definitions.  - 
// Returned by  IGetItemType::GetServerDataType() by Tag using SNMP Driver
/////////////////////////////////////////////////////////////////////////////
#define NO_SNMP_TABLE           0       // do nothing
#define SNMP_TABLE_TAG          1       // Tag OID is an SNMP Table
#define SNMP_TABLE_OPTIMIATION  3       // enable Optimization for SNMP_TABLE_TAG. used by driver.

////////////////////////////////////////////////////////////////////////
// Special IGetItemType::GetItemType().  Use by Tag/Block manager to 
// determine the type of handle used in the KernelCallback method.
// This handle is a actually a IGetDataType*.  
////////////////////////////////////////////////////////////////////////
enum ItemType
{
    itTAG,          // used to recast the IGetDataType* to a ETag* w/i TagManager.
    itEXPRESSION,   // special case for Expression data item.
    itDATA_ITEM     // others can be added as necessary.
};

////////////////////////////////////////////////////////////////////////
// To correctly interpret received values, Data Source needs to know the type
// of a corresponding Tag. The IGetItemType interface, implemented by Tag component,
// provides such information. A Data Source obtains a pointer to this interface as
// a parameter in the IDataSource::AddItem call.
interface IGetItemType : IKernelBase
{
   virtual CDataValue::ValueType GetDataType() = 0;
   virtual int GetServerDataType() = 0;
   virtual ItemType GetItemType() = 0;
};

////////////////////////////////////////////////////////////////////////
// IDataItem interface.
// 2.6 - change IDateItem to also inherit from IGetItemType
// CDataItem will stub out the functions of IGetItemType.
// This allows Data Items to be processed more effectively by the 
// Tag-Block Manager Queue.
interface IDataItem : public IGetItemType

{
    virtual IDataItemProvider* GetProvider() const = 0;
    
    virtual CDataValue& GetValue() = 0;
    // return true if new value is set.
    virtual bool       SetValue(const CDataValue&) = 0;
    
    virtual void       AddListener(IDataItemListener*, LONG_PTR nDataItemId) = 0;
    virtual void       RemoveListener(IDataItemListener*) = 0;
    virtual void       NotifyListeners(CDataValue& Value) = 0;
    virtual bool       ListenersExist() = 0;
    
    virtual KERESULT   GetRange(double* pLowLimit, double* pHighLimit) = 0;

    virtual void       SetActiveState(bool bState = true) = 0;
    
    // 3.7 Changes
    virtual bool        IsHooked() = 0;
    virtual void        SetHook() = 0;
    virtual void        GetDIName(EString& esDIName) = 0;
};


////////////////////////////////////////////////////////////////////////
// IDataItemProvider interface

interface IDataItemProvider : public IKernelBase
{
    virtual TCHAR*    GetTitle() = 0;
    virtual IDataItem* GetDataItem(const TCHAR*) = 0;
    
    virtual int        OpenDataItem(IDataItem*) = 0;
    virtual int        CloseDataItem(IDataItem*) = 0;
    
    virtual int        GetItemValue(IDataItem*, CDataValue&,
                        bool bForceRead = false, long nTimeout = 0) = 0;
    virtual int        SetItemValue(IDataItem*, const CDataValue&) = 0;

    // Only ETag returns valid result
    virtual KERESULT   GetItemDescription(IDataItem*, CDataValue&) = 0;

    virtual int        SubscribeItem(IDataItem*, IDataItemListener*, LONG_PTR) = 0;
    virtual int        UnsubscribeItem(IDataItem*, IDataItemListener*) = 0;
    
};


////////////////////////////////////////////////////////////////////////
// List of listeners

struct CListenerItem
{
    CListenerItem(IDataItemListener* pListener, LONG_PTR ID) : m_pListener(pListener), m_ID(ID), m_bRemoved(false) {}
    IDataItemListener* m_pListener;
    LONG_PTR           m_ID;
    bool               m_bRemoved;
};

#pragma warning(disable: 4231)

typedef ETypedPtrList<EPtrList, CListenerItem*>   CListenerItemsList;
#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC _ETypedPtrList<EPtrList, CListenerItem*>;
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrList<EPtrList, CListenerItem*>;
#endif // WIN32
////////////////////////////////////////////////////////////////////////
// class CDataItem

class _DV_DECLSPEC CDataItem : 
                public IDataItem, 
                public ETrigger
{
public:
    CDataItem(IDataItemProvider* pPvdr, CDataValue::ValueType Type = CDataValue::DEFAULT);
    CDataItem(IDataItemProvider* pPvdr, CDataValue& Val);
    virtual ~CDataItem();
    
    // IKernelBase interface
    bool           GetInterface(KERNEL_IID InterfaceID, void** ppInterface);

    // IDataItem interface
    IDataItemProvider* GetProvider() const;
    CDataValue&     GetValue();
    bool            SetValue(const CDataValue& Value);

    // IGetItemType interface implementation.  Override as necessary.
    CDataValue::ValueType GetDataType() {return m_DataValue.GetType();}
    int GetServerDataType() { return NO_SNMP_TABLE;}
    ItemType GetItemType() {return itDATA_ITEM;}

    void    AddListener(IDataItemListener*, LONG_PTR nId);
    void    RemoveListener(IDataItemListener*);
    void    NotifyListeners(CDataValue& Value);
	 
    inline bool    ListenersExist() { return SubscribersFor(_T(""), _T("")); }
    // Disabled member
    //inline bool    ListenersExist() { return !m_TriggerMap.IsEmpty(); }

    KERESULT       GetRange(double* pLowLimit, double* pHighLimit);
    void           SetActiveState(bool bState = true);

    // 3.7 Changes
    bool        IsHooked();
    void        SetHook();

    // ETrigger mixin provides sunbscription/notification
    /* all except this handled in etrigger.  And Subscribe below. */
    virtual KERESULT InformOneListener(
                                    void* pListener,
                                    EHANDLE hClient,
                                    DWORD nData,
                                    void* pData
                                );

	// ITrigger Interface
	//
    virtual KERESULT Subscribe(
                                    ITriggerListener* pITriggerListener, 
                                    EString&        rTriggerName,
                                    EString&        rSchemaName,
                                    EHANDLE         hClient,
                                    HSUBSCRIPTION*  phSubscription
                                );


    // Accessor functions
    bool           GetActiveState();

    // Support Methods
protected:
    KERESULT FireTrigger();
    void DefineEventInfo(EONEVENTSTRUCT* pEventInfo);

protected:
    IDataItemProvider*   m_pProvider;
    IDynamicData*        m_pIDynamicData;
    CDataValue           m_DataValue;
    bool                 m_bActive;
    bool                 m_bHooked;
};


typedef ETypedPtrList<EPtrList, CDataItem*>   CDataItemList;

// extended data item name
class _DV_DECLSPEC EDataItemID
{
public:
    EDataItemID()  {m_ValueSet = false;}
    ~EDataItemID() {}

	EString		m_Name;     // Full Path name of item
    EString     m_Item;     // The item name
	EString		m_Id;       // Item id
    bool        m_ValueSet; // Item has its value set
}; 

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrArray<EPtrArray, EDataItemID*>;
#endif // WIN32

// extended data item name array
class _DV_DECLSPEC EDataItemIDs : public ETypedPtrArray<EPtrArray, EDataItemID*>
{
public:
    EDataItemIDs(bool bDestroy = true) :
        m_bDestroy(bDestroy) {}
    virtual ~EDataItemIDs()
    {
        if (!m_bDestroy)
            return;

        for (int i = 0; i < GetSize(); i++)
        {
            EDataItemID* pItem = GetAt(i);
            delete pItem;
        }
        RemoveAll();
    }

private:
    bool m_bDestroy;
};

// data item pair (name - value)
class _DV_DECLSPEC EDataItemPair
{
public:
    EDataItemPair() :
        m_pID(NULL) {}
    ~EDataItemPair() {}

    EDataItemID *m_pID;
    CDataValue   m_Value;
};

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EDataItemPair*>;
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrList<EPtrList, EDataItemPair*>;
#endif // WIN32

// data item pair list
class _DV_DECLSPEC EDataItemPairList : public ETypedPtrList<EPtrList, EDataItemPair*>
{
public:
    EDataItemPairList(bool bDestroy = true) :
        m_bDestroy(bDestroy) {}
    virtual ~EDataItemPairList()
    {
        if (!m_bDestroy)
            return;

        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EDataItemPair* pPair = GetNext(pos);
            delete pPair;
        }
        RemoveAll();
    }

private:
    bool m_bDestroy;
};

#endif
