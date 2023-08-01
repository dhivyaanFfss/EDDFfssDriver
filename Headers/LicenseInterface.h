//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  LicenseInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define interfaces for Licensing
//
//  Interfaces:
//				ILicense
//
//**************************************************************************

#ifndef __LICENSE_INTERFACE_H__
#define __LICENSE_INTERFACE_H__

/* Group=Licensing */

//**************************************************************************
interface ILicense
{
    // get methods
    //virtual KERESULT GetLicense( EString& license ) = 0;
    virtual KERESULT GetModelNumber( EString& modelNumber ) = 0;
    virtual KERESULT GetSerialNumber( EString& serialNumber ) = 0;
    virtual KERESULT GetOwner( EString& ownerId ) = 0;

    // set methods
    //virtual KERESULT SetLicense( TCHAR* pLicense ) = 0;
    //virtual KERESULT SetModelNumber( TCHAR* pModelNumber ) = 0;
    //virtual KERESULT SetSerialNumber( TCHAR* pSerialNumber ) = 0;

    // product name
    //virtual KERESULT GetProductName(EString& name) = 0;

    // counter manipulation
    virtual KERESULT AdjustTagCount(  unsigned long iNumTags ) = 0;
	virtual KERESULT SetLicenseMaxTags(unsigned long maxTags) = 0;
	virtual KERESULT CheckLicenseTags() = 0;
	virtual KERESULT SetTrial( bool bIsTrial) = 0; 
	virtual bool	 IsTrial() = 0;
    //virtual KERESULT AdjustWebConnectCount( int iNumConnect ) = 0;

    // configuration string for Connector
    virtual KERESULT GetConfigString(EString& configString) = 0;
};

#endif


