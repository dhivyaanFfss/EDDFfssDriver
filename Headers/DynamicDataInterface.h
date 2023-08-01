//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  DynamicDataInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Dynamic data interface definitions.
//
//  Classes:                                         
//          IDataItemListener
//          IDynamicData
//
//**************************************************************************

#ifndef __DYNDATA_INTERFACE_H__
#define __DYNDATA_INTERFACE_H__

#include "DataValue.h"
#include "KernelInterface.h"

// group=Data Items

interface IDataItemProvider;
interface IDataItem;

#define DATA_ITEM_WRITE_ACTION      _T("DoDataItemWrite")
#define DATA_CHANGE_TRIGGER         _T("OnDataChanged")

////////////////////////////////////////////////////////////////////////
// Dynamic Data interfaces.

interface IDataItemListener
{
   virtual void   OnValueChanged(LONG_PTR nDataItemId, CDataValue&) = 0;

   // If True, Data Item will destroy it on unsubscribe.
   // If False, the user is responsible for destroying this object.
   virtual bool   IsDestructive() { return true; }
};


interface IDynamicData : public IKernelBase
{
   virtual KERESULT OpenDataItems(DWORD dwCount, const TCHAR **ppszItemNames, 
                                  EHANDLE *phDataItems, KERESULT *pErrors = NULL) = 0;
   virtual KERESULT CloseDataItems(DWORD dwCount, EHANDLE *phDataItems,
                                   KERESULT *pErrors = NULL) = 0;

   virtual KERESULT GetDataItemValues(DWORD dwCount, EHANDLE *phDataItems, 
                                      bool bForceRead, DWORD dwTimeout,
                                      CDataValue *pValues, KERESULT *pErrors = NULL) = 0;

   virtual KERESULT SetDataItemValues(DWORD dwCount, EHANDLE *phDataItems, CDataValue *pValues,
                                      KERESULT *pErrors = NULL) = 0;

   // Get the DataItem description. Currently, only ETag returns valid result.
   virtual KERESULT GetDataItemDescription(EHANDLE hDataItem, CDataValue& dataValue) = 0;

   virtual KERESULT Subscribe(IDataItemListener*, DWORD dwCount, EHANDLE *phDataItems,
                              EHANDLE *hClientIds, KERESULT *pErrors = NULL) = 0;

   virtual KERESULT Unsubscribe(IDataItemListener*, DWORD dwCount, EHANDLE *phDataItems,
                                KERESULT *pErrors = NULL) = 0;

   virtual void   AddProvider(IDataItemProvider*) = 0;

   virtual void   RemoveProvider(IDataItemProvider*) = 0;

   virtual KERESULT GetDataItem(const TCHAR* pszItem, /* out */ IDataItem** ppIDataItem) = 0;

   // PutItemMessage removed in version 2.5;  
   // Place in ITagManager in 2.6  See ProcessItem().
//   virtual KERESULT  PutItemMessage(IDataItem*, const CDataValue&) = 0;

   // 2.6 functions
   
   // Parse a qualified Data Item name (e.g. [Expression]Exp1) into two parts.
   // Output:  rHint = Expression, rItemName = Exp1
   // If name is not qualified, rHint is set Empty.
   virtual KERESULT ParseName(const EString& rQualifiedName, EString& rHint, 
       EString& rItemName) = 0;

};

#endif


