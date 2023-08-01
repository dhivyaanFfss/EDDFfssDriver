//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EnumeID.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Enumerator public interfaces ID's
//
//  Classes:
//
//**************************************************************************

#ifndef __ENUM_ID_H__
#define __ENUM_ID_H__

// group=Enumeration

// See Also:  IEnumerator::CreateEnumerator
typedef  unsigned long           ENUM_IID;

#define  IID_ENUMERATE_RESERVED     0x2000   // Base ENUM_IID number 


// Public interface ID's

#define  ENUM_NODES          IID_ENUMERATE_RESERVED + 1  // ENUM_IID to enumerate Nodes
#define  ENUM_DATASOURCES    IID_ENUMERATE_RESERVED + 2  // ENUM_IID to enumerate Data sources
#define  ENUM_TAGS           IID_ENUMERATE_RESERVED + 3  // ENUM_IID to enumerate Tags
#define  ENUM_STYLES         IID_ENUMERATE_RESERVED + 4  // ENUM_IID to enumerate Tag styles
#define  ENUM_ALARMS         IID_ENUMERATE_RESERVED + 5  // ENUM_IID to enumerate Alarm definitions
#define  ENUM_FILTERS        IID_ENUMERATE_RESERVED + 6  // ENUM_IID to enumerate Alarm filters
#define  ENUM_DATA_ITEMS     IID_ENUMERATE_RESERVED + 7  
#define  ENUM_SEC_GROUPS     IID_ENUMERATE_RESERVED + 8 
#define  ENUM_SEC_USERS      IID_ENUMERATE_RESERVED + 9 
#define  ENUM_SEC_GRP_USERS  IID_ENUMERATE_RESERVED + 10
#define  ENUM_SCRIPTS        IID_ENUMERATE_RESERVED + 11
#define  ENUM_ALARM_EVENTS   IID_ENUMERATE_RESERVED + 12

#endif //__ENUM_ID_H__
