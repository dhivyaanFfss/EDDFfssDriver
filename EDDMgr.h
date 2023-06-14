//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDMgr.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//  Classes    :  
//                EDDMgr
//
//*****************************************************************************

#ifndef __EDD_MGR_H__
#define __EDD_MGR_H__

static const int EDD_MESSAGE_ID = 1;
static const int EDD_DEBUG_ID   = 4;
static const DWORD DEFAULT_ACQUISITION_RATE = 5000;         // 5.0 seconds.

//*****************************************************************************
//  
// An EDD driver must derive a class from IEDDManager and implement all its members.
//
//*****************************************************************************
class EDDMgr :  public IEDDManagerEx

{
public:
    // Constructor
    EDDMgr(TCHAR* pID);
    virtual ~EDDMgr();

    // IKerneBbase methods
    bool GetInterface(KERNEL_IID InterfaceID, void** ppInterface);

    // IEDDManager methods
    KERESULT CreateEDDDriver(IEDDCallback*, TCHAR* pID, IEDDDriver*&, EDDDriverInfo* pEDDDriverInfo = NULL);
	KERESULT Destroy();


    // Public support methods
    void AddRef();
    void Release();
    KERESULT Initialize();
    void RemoveDriver(EDDDriver& eddDriver);

    EDDBrowser* GetBrowser() { return &m_browser; }
    // Set in milliseconds.
    void SetAcquisitionRate(DWORD acqRate) {m_acqRate = acqRate; }
    void StartAcquistion();
    void StopAcquistion();

    void CustomReport(ReportClass reportType, UINT msgID);
    void CustomReport(ReportClass reportType, UINT msgID, EString& esMessage);


protected:
    // >>>  methods <<<<

    // Using a static thread function with the parameter being a
    // pointer to the EDDMgr, invoke the manager's thread method.
    // This technique allows the thread function to be a method of the object.
    ThreadReturn   AcquisitionThread();
    static ThreadReturn __stdcall  AcquisitionThreadStub(void* pThis)
    {
		return ((EDDMgr*)pThis)->AcquisitionThread();
	}

    ThreadReturn   AppInterfaceThread();
    static ThreadReturn __stdcall AppInterfaceThreadStub(void* pThis)
    {
        return ((EDDMgr*)pThis)->AppInterfaceThread();
    }



    // >>>  members <<<
    EString                 m_esConfig;
    DWORD                   m_nRefCount;
    MSGFILE                 m_msgFileHandle;

    // Support interfaces
    IKernelLog*             m_pIKernelLog;
    IMsgManager*            m_pIMsgManager;

    // Thread functionality.  In this example, the Thread is per the single
    // instance of the manager.  Threads can be per IEEDDriver instances as well.
    bool                    m_bRunning;
    EThread                 m_AcquisitionThread;
    EThread                 m_AppInterfaceThread;
    ESingleEvent            m_EDDEvent;
    DWORD                   m_acqRate;      // in milliseconds

    // List of created drivers.
    EDDDriverList           m_drivers;
    EMutex                  m_driversMtx;

    // Browser support
    EDDBrowser       m_browser;

};

#endif // __EDD_MGR_H__

