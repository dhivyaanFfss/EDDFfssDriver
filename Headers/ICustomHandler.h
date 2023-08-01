//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  ICustomHandler.h 
//  
//  Subsystem  :  Kernel
//
//  Description:  SDK Custom Handler Manager interface and support definitions.
//
//  Classes:
//              ICustomHandlerClient       
//              ICustomHandler
//
//*****************************************************************************

#ifndef ICUSTOM_HANDLER_H__
#define ICUSTOM_HANDLER_H__

#include "EDynamicLibrary.h"
#include "EDD_Common.h"
#include "CustomHandlers.h"

typedef IKernelService* (*CUSTOM_HANDLER_INIT_SERVICE_FUNCTION)(IKernelBase*, EDynamicLibrary*);

/* Group=ICustomHandlerBase */

class HookedData
{
public:
    HookedData()
        :  m_bInhibit(false)
    {
        m_clientIds.SetSize(1, 10);
        m_dataValuePtrs.SetSize(1, 10);
    }
    void Inhibit(int ix)
    {
        m_clientIds.RemoveAt(ix);
        m_dataValuePtrs.RemoveAt(ix);
    }
    EPtrArray           m_clientIds;
    CDataValuePtrArray  m_dataValuePtrs;
    bool                m_bInhibit;         // set true by callee if any value inhibited.

};

enum CustomStatus
{
    csNO_CHANGE,
    csINHIBITED,
    csMODIFIED              // not always a valid case for each functional usage.
};

class HookedFileXfer
{
public:
    enum FileType
    {
        UPLOAD,
        DOWNLOAD
    };
    enum When
    {
        // For upload: before means before sending request to Enterprise.
        BEFORE_XFER,
        // For upload:  after means after Enterprise request to upload received.
        // For download: after means after file downloaded from Enterprise.
        AFTER_XFER
    };

    HookedFileXfer(FileType type, When when, EString& esFile)
        : m_type(type), m_when(when), m_esFile(esFile) {}

    // properties
    FileType    m_type;
    When        m_when;
    EString     m_esFile;           // File name
};

//*****************************************************************************
// Some custom handling overlaps between the Client (proxy) and the Custom Handler.
interface ICustomHandlerBase : public IKernelBase
{
    // Function called by TagManager when data that is "hooked" changes. 
    // This will invoke the CustomHandler's AX_OnDataChange function.
    virtual KERESULT HandleDataChange(int gatewayId, /* in/out */HookedData& rHookedData) = 0;
    virtual KERESULT HandleDIWrite(int gatewayId, EString& esName, 
        /* in/out */ CDataValue& rValue, /* out */ CustomStatus& rStatus) = 0;
    virtual KERESULT HandleAlarm(int gatewayId, /* in/out */ CondEventStruct& rAlarmData,
        /* out */ CustomStatus& rStatus) = 0;
    virtual KERESULT HandleEvent(int gatewayId, /* in/out */ EDDEvent& rEvent,
        /* out */ CustomStatus& rStatus) = 0;
    virtual KERESULT HandleFileXfer(int gatewayId, HookedFileXfer& fileXfer,
        /* out */ CustomStatus& rStatus) = 0;
    virtual KERESULT HandleAction(int gatewayId, EString& esAction, EString& esParameter) = 0;
    virtual KERESULT HandleRemoteSession(/* in/out */ AX_RemoteSession& session,
        /* in */ AX_BOOL bOnStart) = 0;
};


/* Group=CustomHandlerClient */

//*****************************************************************************
// This interface is implemented by the CustomHandlerClient Component that is 
// part of the Agent.
interface ICustomHandlerClient : public ICustomHandlerBase
{
    virtual KERESULT IsDataHooked(int gatewayId, /* out */ bool& bHooked) = 0;
    virtual KERESULT IsAllDataHooked(int gatewayId, /* out */ bool& bAllDataHooked) = 0;
    virtual KERESULT IsEnterpriseDIWriteHooked(int gatewayId, /* out */ bool& bHooked) = 0;
    virtual KERESULT IsAlarmHooked(int gatewayId, EString& esAlarm, bool bDynamic, /* out */ bool& bHooked) = 0;
    virtual KERESULT IsAllEventsHooked(int gatewayId, /* out */ bool& bHooked) = 0;
    virtual KERESULT IsFileHooked(int gatewayId, HookedFileXfer& fileXfer, /* out */ bool& bHooked) = 0;
    virtual KERESULT IsRemoteSessionHooked(int gatewayId, EString& esRemoteSession, /* out */ bool& bHooked) = 0;
};


// ID used with IKernelBase::GetInterface to obtain the interface:  ICustomHandlerMgr
// Note, there can be multiple ICustomHandlerMgr interfaces.  One per custom handler SDK DLL.
#define  IID_CUSTOM_HANDLER_MGR             IID_KERNEL_CUSTOM + 63

//*****************************************************************************
/* Group=CustomHandler */


// This interface is implemented by the CustomHandler library that is 
// linked to every Custom Handler DLL created by a customer.
interface ICustomHandler : public ICustomHandlerBase
{

};

#endif
