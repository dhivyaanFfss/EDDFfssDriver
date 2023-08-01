//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDExports.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Implement the DLL's exported functions.
//                The functions are exported using a 'C' prototype.
//                The symbol 'DRIVER_EXPORTS' must be defined.
// 
//                The Exported functions are used by DRM Agent and DRM Builder
//                  - EDD_EEDD_Initialize:  entry point for DRM Agent
//                Entry points for DRM Builder
//                  - EDDInit
//                  - EDDBrowseItems
//                  - EDDMemFree
//                  - EDDShutdown
//

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"

// Define globals.
EDDMgr*         g_pEDDMgr = NULL;
IKernelBase*    g_pIKernelBase = NULL;
int             g_driverInitialized = 0;


//*****************************************************************************
//
// Every EDD driver should have the following exported Initialization function.
// The function is in the format "EDD_XXX_Initialize"
// where XXX is the name of this driver module, minus the suffix.
//
// For example: This module name is "EDDFfssDriver.dll" and the corresponding
// Init function name is "EDD_EDDFfssDriver_Initialize()".
//
//*****************************************************************************

extern "C" DRIVER_API
IEDDManagerEx*  EDD_EDDFfssDriver_Ex_Initialize(IKernelBase* pKernel, TCHAR* pIdentifier)
{
    // Save IKernelBase.  This is used to query the Agent for its published interfaces.
    g_pIKernelBase = pKernel;

    // When called, this member should return the IEDDManager derived object pointer.
    // The caller is responsible for managing memory (releasing memory, etc.).
    if (!g_pEDDMgr)
    {
        g_pEDDMgr =  new EDDMgr(pIdentifier);
        if (g_pEDDMgr->Initialize() != KE_OK)
        {
            // Force error on client since Manager can not be successfully initialized.
            g_pEDDMgr->Release();
            g_pEDDMgr = NULL;
        }
    }
    else
        // The manager is reference counted to support optional implementation 
        // of the IDiscovery interface.  Auto-discovery is not demonstrated
        // in this example.
        g_pEDDMgr->AddRef();


    return g_pEDDMgr;
}

//*****************************************************************************
//
// EDDInit() - Initialize the driver manager.
// Used only by Axeda Builder.
//
//*****************************************************************************
extern "C" DRIVER_RESULT DRIVER_API EDDInit(TCHAR* pIdentifier)
{
    g_driverInitialized = 1;

    if (!g_pEDDMgr)
        g_pEDDMgr =  new EDDMgr(pIdentifier);
    else
        g_pEDDMgr->AddRef();

    // Initialize browsing.
    KERESULT ker = g_pEDDMgr->GetBrowser()->Init();
    if (ker != KE_OK)
    {
        g_pEDDMgr->Release();
        return DRIVER_FAILED;
    }
    return DRIVER_OK;
}


//*****************************************************************************
//
// EDDBrowseItems() browses all the items defined for this driver.
// Used only by Axeda Builder.
//
//*****************************************************************************
extern "C" DRIVER_RESULT DRIVER_API EDDBrowseItems(DWORD& dwCount, TCHAR**& ppszItems, 
                                                   DWORD*& pdwAttributes)
{
    // Must initialize the driver first
    if(g_driverInitialized == 0)
        return DRIVER_NOT_INITIALIZED;

    // The EDDBrowser object does the work...
    return g_pEDDMgr->GetBrowser()->BrowseItems(dwCount, ppszItems, pdwAttributes);

}

//*****************************************************************************
//
// EDDMemFree() frees the memory this driver returned to client
//
//*****************************************************************************
extern "C" DRIVER_RESULT DRIVER_API EDDMemFree(void* pMem)
{
    delete [] pMem;
    return KE_OK;
}

//*****************************************************************************
//
// EDDShutdown() shuts down the driver.
// Used only by Axeda Builder.
//
//*****************************************************************************
extern "C" DRIVER_RESULT DRIVER_API EDDShutdown(TCHAR* pszIdentifier)
{
    g_pEDDMgr->Release();
    return KE_OK;
}



