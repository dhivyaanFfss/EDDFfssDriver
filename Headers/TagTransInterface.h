//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  TagTransInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define tag translation (conversion) interfaces.
//
//  Classes:
//          ITagTranslation
//
//**************************************************************************

#ifndef __TAGTRANS_INTERFACE_H__
#define __TAGTRANS_INTERFACE_H__

#include "DataValue.h"

#ifndef  interface
#define  interface   struct // {secret}
#endif

#define ID_TRANS_LINEAR    1  // {secret}


/* Group=Tags */

////////////////////////////////////////////////////////////////////////
// ITagTranslation : The base Tag Translation interface

// See Also:  ITagManager::CreateTagTranslation, ITagManager::AddTagStyle,  
//            ITagStyleDef::SetTransDef
interface ITagTranslation : public IPersistent
{
    // Remarks:  Converts in place passed raw value to engineering units
    virtual int    Raw2Engineering(CDataValue& Value) const = 0;
    // Remarks:  Converts in place passed engineering value to a raw value
    virtual int    Engineering2Raw(CDataValue& Value) const = 0;
    
    virtual ITagTranslation* Clone() const = 0;
  //  virtual ITagTranslation& operator=(const ITagTranslation&) = 0;
    virtual unsigned GetTransId() const = 0;
    
    virtual void   Release() = 0;
    
    virtual void   RangeRaw2Engineering(double& dMin, double& dMax) const = 0;

};

////////////////////////////////////////////////////////////////////////
// Linear Translation interface

// See Also:  ITagManager::CreateTagTranslation, ITagManager::AddTagStyle,  
//            ITagStyleDef::SetTransDef
interface ILinearTranslation : ITagTranslation
{
    virtual double    GetFirstRaw() = 0;
    virtual double    GetSecondRaw() = 0;
    virtual double    GetFirstEngineering() = 0;
    virtual double    GetSecondEngineering() = 0;

    // Remarks:  Set low / high range value of raw data.
    virtual void      SetRawPoints(double dRaw1, double dRaw2) = 0;
    // Remarks:  Set low / high range of engineering scaled data.
    virtual void      SetEngineeringPoints(double dEng1, double dEng2) = 0;
};


#endif

