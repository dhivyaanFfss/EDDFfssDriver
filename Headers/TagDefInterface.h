//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  TagDefInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Tag Definition Interface
//
//  Classes:
//              ITagStyleDef
//              ITagBaseDef
//
//************************************************************************** 

#ifndef __TAGDEF_INTERFACE_H__
#define __TAGDEF_INTERFACE_H__

#include "DataValue.h"
#include "TagTransInterface.h"
#include "DataSrcManInterface.h"

/* Group=Tags */

////////////////////////////////////////////////////////////////////////
// Tag Style definition

// See Also: ITagManager::CreateTagStyleDef, ITagManager::AddTagStyle, 
//           ITagManager::GetTagStyle, ITagManager::ModifyTagStyle
interface ITagStyleDef: public IPersistent
{
   virtual EString    GetName() const = 0;
   virtual EString    GetDescription() const = 0;
   virtual EString    GetUnits() const = 0;
   virtual double     GetLowLimit() const = 0;
   virtual double     GetHighLimit() const = 0;
   virtual BOOL       GetDigReverse() const = 0;
   virtual unsigned   GetTransId() const = 0;
   virtual double     GetDeadband() const = 0;

   virtual ITagTranslation*
                      GetTransDef() const = 0;

   virtual void       SetTransDef(const ITagTranslation*) = 0;
   virtual int        SetLowLimit(double) = 0;
   virtual int        SetHighLimit(double) = 0;

   virtual int        SetName(const TCHAR*) = 0;
   virtual int        SetDescription(const TCHAR*) = 0;
   virtual int        SetUnits(const TCHAR*) = 0;
   virtual int        SetDigReverse(BOOL bDigReverse = TRUE) = 0;
   virtual KERESULT   SetDeadband(double deadband) = 0;

   virtual void       AddRef() = 0;
   virtual void       Release() = 0;
};


// Base Tag definition

// See Also:  ITagManager::GetTag, ITagManager::AddTag, ITagManager::ModifyTag
interface IDataServer;
class EFileIO;
interface ITagBaseDef: public IPersistent
{
    virtual bool        IsBranch() const = 0;
    virtual bool        IsParentRoot() const = 0;

    virtual EString&    GetName() = 0;
    virtual EString     GetParentName() const = 0;
    virtual EString     GetFullName() const = 0;
    virtual EString     GetDescription() const = 0;

    virtual int         SetName(const TCHAR*) = 0;
    virtual int         SetParentName(const TCHAR*) = 0;
    virtual int         SetDescription(const TCHAR*) = 0;
    virtual int         SetType(CDataValue::ValueType Type) = 0;

    virtual void        Release() = 0;

    virtual ITagBaseDef* GetParent() const = 0;
    virtual KERESULT    Open() = 0;
    virtual KERESULT    Close() = 0;
    virtual int         ModifyDefinition(const ITagBaseDef*) = 0;
    virtual KERESULT    ReOpen(IDataServer* pDataServer) = 0;
    
    virtual KERESULT    SaveDataItems(EFileIO* pDataFile) = 0;

    virtual KERESULT    GetType(CDataValue::ValueType& type) = 0;
    virtual KERESULT    GetDataSourceItemName(EString& esItemName) = 0;
    virtual KERESULT    SetDataSourceItemName(EString& esItemName) = 0;
    virtual KERESULT    GetDataSource(IDataSource*& pDataSource) = 0;
};

//**************************************************************************
// 
interface ITagDefProvider
{
    virtual int         ModifyTagDefinition(ITagBaseDef*) = 0;
};

#endif
