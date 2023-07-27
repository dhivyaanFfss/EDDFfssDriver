//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  pch.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
// 
//                Pre-Compiled header includes other common header files
//
//  Classes:
//
//**************************************************************************

// Define the following constant to use your own memory manager
// #define _IGNORE_CONNECTOR_MEMORY_MANAGER_

// Standard includes.
#include "os.h"
#include "ECollections.h"
#include "ETime.h"
#include "EArrays.h"
#include "KernelService.h"
#include "componentid.h"

// Include files when reading an XML file using the SAX parser.
#include "ISax.h"

// Include when using software management interface to upload a file.
#include "ISM.h"

#include "IEnterpriseProxy.h"
#include "IRestart.h"
#include "EnterpriseCommunication.h"
#include "AgentLifecycle.h"

// General includes for all EDD Drivers.
#include "EDD_Common.h"
#include "EddDriverApi.h"


// Make sure your EDD driver specifies any driver-specific includes.
#include "EDDDataItem.h"
#include "SimulatedDataItem.h"
#include "EDDConfig.h"
#include "EDDDriver.h"
#include "EDDBrowser.h"
#include "EDDMgr.h"

// Externals
extern EDDMgr*          g_pEDDMgr;
extern IKernelBase*     g_pIKernelBase;




