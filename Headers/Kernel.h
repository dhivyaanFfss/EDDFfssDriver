//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  Kernel.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Include other interface definition files.
//
//
//**************************************************************************

#ifndef __KERNEL_H__
#define __KERNEL_H__


#ifdef  __WCEALT_H__    // in WCE MFC application operator new already defined
#define __PLACEMENT_NEW_INLINE
#endif

#include "KernelErrors.h"
#include "PersistenceInterface.h"
#include "KernelInterface.h"
#include "TagDefInterface.h"
#include "TagManInterface.h"
#include "DataSrcManInterface.h"
#include "DynamicDataInterface.h"

#include "Alarm.h"
#include "AlarmDefInterface.h"
#include "AlarmManInterface.h"
#include "DynamicAlarmInterface.h"

/////////////////////////////////////////////////////////////////////////////
//
// To include network and enum components, define the following
//
/////////////////////////////////////////////////////////////////////////////
// #define INCLUDE_NETWORK_COMPONENTS
#define INCLUDE_ENUM_COMPONENTS

#endif

