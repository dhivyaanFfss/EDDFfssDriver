//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  KernelService.h
//  
//  Subsystem  :  Kernel
//
//  Description:  KernelService helper implementation
//
//  Classes:    CKernelService
//
//**************************************************************************
#ifndef __KERNELSERVICE_H__
#define __KERNELSERVICE_H__

#include "Kernel.h"
#include "KernelInterface.h"
#include "KernelErrors.h"
#ifdef __GATEWAY__
#include "IRunTime.h"
#endif

/* Group=Interfaces */

// Service ('component') Run Status.  Note, this is a bit pattern.  The STARTED
// value includes the 'PREPARETOSTART' bit.  This can be used in the PrepareToStop()
// function to know if the component is being shutdown before it was fully started.
// This usage is component specific.
typedef enum
{
    STOPPED         = 0,
    PREPARETOSTART  = 1,
    PRESTART        = 3,
    STARTED         = 7,
    PREPARETOSTOP   = 8,
    // use the following for RunTime prestart and start to  reuse the 
    // PreStart and Start functions.
    RT_PRESTARTING  = 0x10,         
    RT_STARTING     = 0x20
} RunStatus;

#define DEVICE_ENABLED          0x1     // Not currently used.
#define DEVICE_ONLINE           0x2     // Used by Auto-discovery.  Set off-line if device missing at discovery.
#define DEVICE_EXTERNAL         0x8     // Device is externally managed by Custom Component
                                        // or defined specifically in xGateway.xml.
#define DEVICE_REMOVED          0x20    // No longer used.


//**************************************************************************
class CKernelService : 
#ifndef __GATEWAY__
                    public IKernelService
#else
                    public IKernelService,
                    public IRunTime
#endif
{
public:
    CKernelService(IKernelBase* pKernelBase)
        : m_pParent(pKernelBase),m_recursive(false),m_RunStatus(STOPPED) 
    {
        #ifdef __GATEWAY__
            m_pIGateway = NULL;
            m_gatewayDeviceId = OUT_OF_GATEWAY_IDS;
        #endif
    }
    virtual ~CKernelService() {;}
    
    // IKernelBase interface
    bool    GetInterface(KERNEL_IID InterfaceID, void** ppInterface)
    {
        bool   bResult = false;
        if (m_recursive)
            return bResult;
        m_recursive = true;
        *ppInterface = 0;
        switch (InterfaceID)
        {
        case IID_KERNEL_SERVICE:
            *ppInterface = (IKernelService*)this;
            break;
        default:
            if (m_pParent)
                bResult = m_pParent->GetInterface(InterfaceID,ppInterface);
            break;
        }
        m_recursive = false;
        return bResult;
    }

    // IKernelService interface
    // Stub out these functions so they don't have to be implemented if
    // they are not needed by the derived class.

    // Persistent interface
    //
    KERESULT Restore(EHANDLE pData) { return KE_OK; };

#ifdef __GATEWAY__

//**************************************************************************
// Gateway Restore.
// This sets two public variables (m_pIGateway && m_gatewayDeviceId) meant to 
// be used by components that derive from CKernelService.
// The m_gatewayDeviceId is helpful during the component's Restore function
// to set connector names to fully qualified gateway names.  The global
// variable works because all restore function calls are synchronous.
//**************************************************************************
KERESULT Restore(EHANDLE hPersist, int gatewayDeviceId)
{
    // Set public member m_pIGateway for each component.
    if (m_pIGateway == NULL)
    {
        GetInterface(IID_GATEWAY, (void**) &m_pIGateway);
        if (m_pIGateway == NULL)
            return KE_FAILED;
    }

    m_gatewayDeviceId = gatewayDeviceId;

    // Don't do gateway restore unless valid Id.
    if (gatewayDeviceId == OUT_OF_GATEWAY_IDS)
        return KE_OK;
        
    return Restore(hPersist);
}
#endif
    KERESULT Save(EHANDLE pData)    { return KE_OK; };

    // Functions to track the start up / shutdown state of the component.

    KERESULT PrepareToStart()   {m_RunStatus = PREPARETOSTART; return KE_OK;}
    KERESULT PreStart()         {m_RunStatus = PRESTART; return KE_OK;}
    KERESULT Start()            {m_RunStatus = STARTED; return KE_OK;}
    KERESULT PrepareToStop()    {m_RunStatus = PREPARETOSTOP; return KE_OK;}
    KERESULT Stop()             { delete this; return KE_OK; }
    KERESULT AddService(IKernelService* pService, CServiceList*){return KE_OK;}
    KERESULT Destroy()          { delete this; return KE_OK; }

    // Component ID
    // Components with an ID of 0 are always loaded regardless of license.
    KERESULT GetComponentId(DWORD& componentId) { componentId = 0; return KE_OK;}

    // Warm Restart
    KERESULT WarmRestart(WR_OPERATION op, WARM_RESTART_STRUCT* p) {return KE_OK;}

#ifdef __GATEWAY__

    // *** IRunTime interface  ***   
    //  >>> GATEWAY ONLY <<<
    // Stub out these functions so they don't have to be implemented if
    // they are not needed by the derived class.
    KERESULT RTPreStart()
    {
        return KE_NOT_IMPLEMENTED;
    }

    KERESULT RTStart()
    {
        return KE_NOT_IMPLEMENTED;
    }

    KERESULT RTRestore(EHANDLE pData, int gatewayId)
    {
        // default implementation
        return CKernelService::Restore(pData, gatewayId);
    }
#endif      // __GATEWAY__

    // CKernelService functions.

    RunStatus GetRunStatus() {return m_RunStatus;}
    void    SetRunStatus(RunStatus status) {m_RunStatus = status;} 

    // public properties meant to be directly used by derived class.
public:  
#ifdef __GATEWAY__
    IGateway*       m_pIGateway;
    // reset at each restore.  Used to set fully qualified names.
    int             m_gatewayDeviceId;      
#endif

protected:
    IKernelBase* m_pParent;

    // Pack the name in the format of 2 nulls at the end
    KERESULT        PackName(WARM_RESTART_STRUCT* pParam, const TCHAR* pName)
    {
        if(pParam == NULL)
            return KE_INVALID_PARAMS;

        size_t ichars = _tcslen(pName) + 2;
        pParam->m_pBuffer = new TCHAR[ichars];
        if(pParam->m_pBuffer == NULL)
            return KE_MEMORY;

        memset(pParam->m_pBuffer, 0, (ichars) * sizeof(TCHAR));
        _tcscpy(pParam->m_pBuffer, pName);
        return KE_OK;
    }
private:
    bool         m_recursive;
    RunStatus    m_RunStatus;
};

#endif





