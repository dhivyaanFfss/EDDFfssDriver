//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IScriptManager.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define IScriptManager interface
//
//
//**************************************************************************

#ifndef __ISCRIPT_MANAGER_H__
#define __ISCRIPT_MANAGER_H__

//**************************************************************************
interface IScriptListener
{
    enum RunState
    {
        RunStateUnknown,
        RunStateDisabled,
        RunStateReady,
        RunStateRunning,
        RunStateTimeout,
        RunStateError,
        RunStateStopped,
        RunStateCompleted
    };

    virtual void OnRunStateChange(TCHAR *pszScriptName, RunState iState) = 0;
};

//**************************************************************************
class EScriptStatusInfo
{
public:
    IScriptListener::RunState   m_iState;
    ETimeStamp                  m_updateTime;
};

//**************************************************************************
interface IScriptManager : public IKernelBase
{
    // Get all the configured scripts for a device
    virtual KERESULT GetDeviceScripts(int gatewayID, MStringList& scripts) = 0;

    virtual KERESULT SendRegisteredScript(int gatewayID) = 0;

    // Remove the scheduled script
    virtual void RemoveScheduledScripts(int gatewayID, TCHAR* triggerName) = 0;

    // Subscribe to script run state change notifications
    virtual KERESULT Subscribe(IScriptListener *pIScriptListener) = 0;

    // Unsubscribe from script run state change notifications
    virtual KERESULT Unsubscribe(IScriptListener *pIScriptListener) = 0;

    // Start script
    virtual KERESULT StartScript(EString& model, EString& serial, EString& name) = 0;

    // Stop script
    virtual KERESULT StopScript(EString& model, EString& serial, EString& name) = 0;

    // Enable script
    virtual KERESULT EnableScript(EString& model, EString& serial, EString& name) = 0;

    // Disable script
    virtual KERESULT DisableScript(EString& model, EString& serial, EString& name) = 0;

    // Query script status
    virtual KERESULT GetScriptStatus(EString& model, EString& serial, EString& name,
        IScriptListener::RunState& iState) = 0;

    // Query script status (return extended status information)
    virtual KERESULT GetScriptStatus2(EString& model, EString& serial, EString& name,
        EScriptStatusInfo& status) = 0;

    // Simple mechanism to check if the Script Manager has processed all of its scripts.
    virtual KERESULT IsScriptQueueEmpty(bool* pIsEmpty) = 0;

};

#endif // __ISCRIPT_MANAGER_H__


