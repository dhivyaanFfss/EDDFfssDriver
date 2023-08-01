//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  TagManInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Tag Manager Interface
//
//  Classes:
//              ITagManager
//              ICounterMgr
//
//************************************************************************** */

#ifndef __TAGMAN_INTERFACE_H__
#define __TAGMAN_INTERFACE_H__

interface ITagBaseDef;
interface ITagStyleDef;
interface ITagTranslation;

#include "KernelInterface.h"
#include "TagTransInterface.h"

#define TAG_DELIMITER   _T('.')

// Forward Reference
interface IDataItem;

/* Group=Tags */

////////////////////////////////////////////////////////////////////////
// Remarks: Tag Manager interfaces.
interface ITagManager : public IKernelBase
{
   virtual ITagBaseDef*  GetTag(const TCHAR*) = 0;

   virtual int    AddTag(const ITagBaseDef*) = 0;
   virtual int    RemoveTag(const TCHAR*) = 0;
   virtual int    ModifyTag(const TCHAR*, const ITagBaseDef*) = 0;

//   virtual ITagStyleDef*  GetTagStyle(const TCHAR*) = 0;

   virtual int    AddTagStyle(const ITagStyleDef*, const ITagTranslation* = 0) = 0;
   virtual int    RemoveTagStyle(const TCHAR*) = 0;
   virtual int    ModifyTagStyle(const TCHAR*,
                              const ITagStyleDef*, const ITagTranslation* = 0) = 0;

   virtual ITagBaseDef*     CreateTag() = 0;
   virtual ITagBaseDef*     CreateTagBranch() = 0;
   virtual ITagStyleDef*    CreateTagStyleDef() = 0;

   virtual ITagTranslation* CreateTagTranslation(unsigned nTransId) = 0;
   virtual KERESULT  ReSubscribeTags(IDataServer* pDataServer) = 0;                  

   // 2.6  Enhance TagManager / Block Manager to queue and sychronously process
   // any data item.  This was added so that DataItems can be processed through
   // the Tag-Block Manager Queue.  
   virtual KERESULT  ProcessDataItem(IDataItem*, CDataValue&) = 0;

    // 3.6 Process multiple data items at once. The primary purpose is to handle
    // dummy tag updates as blocks of data to achieve consistent processing.
   virtual KERESULT  ProcessDataItems(DWORD dwCount, EHANDLE *pDataItems,
        CDataValue *pValues, KERESULT *pErrors = NULL) = 0;

   // 3.9 These methods allow to involve external source in the tag definition.
   // The tag definition provider interface is called after the tag is restored
   // but before it is opened.
   virtual KERESULT  RegisterTagDefProvider(ITagDefProvider *pProvider) = 0;
   virtual KERESULT  UnregisterTagDefProvider(ITagDefProvider *pProvider) = 0;
};


//*****************************************************************************
// Define the ICounterMgr interface.
// This is for use by custom components.  The methods relate to the Actions
// available for the CounterMgr.  
// Note:  The interface methods allow a increment, decrement, and reset value;
//        this is not available via the Action per 2.6.
// 
interface ICounterMgr : IKernelBase
{
    virtual KERESULT IncrementCounter(TCHAR* pszCounter, double incValue = 1.0) = 0;
    virtual KERESULT DecrementCounter(TCHAR* pszCounter, double decValue = 1.0) = 0;
    virtual KERESULT ResetCounter(TCHAR* pszCounter, double resetValue = 0.0) = 0;
};

#endif

