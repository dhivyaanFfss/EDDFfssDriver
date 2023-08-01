//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IEDDSNMP.h
//  
//  Subsystem  :  Gateway - EDD Data Source and EDD Driver
//
//  Description:  Define IEDDSNMP interface to be used EDDDataSource for
//				  SNMP specific EDD Driver implementation methods
//
//  Classes:
//              IEDDSNMP
//
//**************************************************************************


#ifndef __IEDDSNMP_H__
#define __IEDDSNMP_H__

//**************************************************************************
// Instead of hacking the IEDDDriver interface, add a IEDDSNMP interface to
// the EDD Driver implementation for the SNMP Driver.
// The EDDDataSource can do a GetInterface() on an EDD Driver to see if it is
// the SNMP Driver.
interface IEDDDriver;
interface IEDDSNMP
{
    // This sets the SNMP driver data item to be of type "SNMP Table"; indicating
    // that the OID should be used to read an SNMP Table.
    // the bOptimization flag determines the frequency of reading the OID.
    // NOTE: This member is obsoleted by AddGroup()/SetItemGroup() below
    virtual KERESULT TableItemConfig(IEDDDriver* pIEDDDriver, EDDHANDLE handle, bool bOptimize) = 0;

    virtual EDDHANDLE AddGroup(IEDDDriver *pDriver) = 0;
    virtual KERESULT  SetItemGroup(IEDDDriver *pDriver, EDDHANDLE hItem, EDDHANDLE hGroup,
        bool bCounter = false) = 0;
};


// ID used to obtain the IEDDSNMP interface.  This is a "custom" ID and will not
// be used by the Connector / Gateway as a inter-component IID.
// 1224 is a "random" number to avoid another EDD Driver having a similar IID for 
// any additional custom interfaces.
#define  IID_EDDSNMP              IID_KERNEL_CUSTOM + 1224      

#endif


