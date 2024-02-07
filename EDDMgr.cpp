//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDMgr.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  The EDDMgr implements the IEDDManager interface.  
//                It maintains the thread used to acquire and publish data.
//
//
//*****************************************************************************
//  Overview:
//    An EDD Driver publishes data, alarms, and events to the Axeda DRM Agent
//    which in turn may be configured to publishes them to the DRM Enterprise Server.
//    
//    This example, demonstrates how to publish data, alarms and events as well
//    as how to initiate a file upload.  The data managed by the driver is 
//    configured in the EDD.xml file.  
//
//    The EDD driver is configured for Agent use by the DRM Builder application.
//    This example demonstrates how to enable the EDD Driver for browsing
//    by Builder for ease of data configuration.
//
//  Key Interfaces:
//    The EDD driver design uses a combination of export 'C' functions and 
//    C++ interface definitions.
//    Builder use the following interfaces:  (see EddDriverApi.h)
//    - EDDInit:   initialization function
//    - EDDBrowseItems:   browse defined data items.
//    - EDDMemFree:  free memory allocated by the driver and passed to the Client.
//    - EDDShutdown:  Shutdown the driver.
//
//    Agent uses the following interfaces:
//    - EDD_EEDD_Initialize is the 'C' entry point to initialize this driver.
//      This is used by the Agent application.
//      It is essential that the name of this exported reflects the name of the DLL.
//      The DLL name must be replace the xxx in:  EDD_xxx_Ex_Initialize function.
//    - IEDDManager:  C++ interface used when implementing the EDD Manager instance.
//      There should only be one instance of the EDD Manager.
//    - IEDDDriver:   C++ interface used when implementing the EDD Driver instance.
//      There may be multiple instances of the EDD Driver object created.
//    
//  Agent and EDD Driver Process Flow:
//    1.  Agent loads the EDD DLL as defined by the Builder project.
//    2.  Agent calls EDD_xxx_Initialize substituting the name of the EDD DLL in for xxx.
//    3.  EDD Driver returns an instance of the EDDManager.
//    4.  Agent calls the IEDDManager method CreateEDDDriver() to create an instance 
//        of the IEDDDriver implemenation.  A DRM Agent - Gateway, creates a driver
//        instance for each managed device.
//        In this example, the driver instance reads the EDD.xml file to define the 
//        data items that are to be managed.
//    5.  The Agent invokes the AddItem method to indicate which of the EDD Driver data
//        items it is interested in.
//    6.  The Agent invokes the StartDriver method.
//        Note:  The Agent may initially invoke the ReadTagValues before invoking StartDriver.
//    7.  The Agent and driver are now up and running.
//        The EDD driver should periodically publish data changes (and alarms / events 
//        as required).  If the EDD driver is defined by the Builder project as being 
//        polled, the Agent will periodically invoke the ReadTagValues method.
//        During this time, the Agent may asynchonously invoke the WriteTagValues method.
//        The IEDDDriver implementation is responsible to maintain the current value for
//        each data item and only publish the data items upon change of value or quality.
//    8.  When the Agent is being shutdown, it will invoke the StopDriver method, then invoke
//        the Destroy method on the IEDDDriver interface.
//
//  Design
//    EDDMgr implement the IEDDManager interface.
//    EDDDriver implement the IEDDDriver interface.
// 
//    Each defined data item is derived from the EDDDataItem class.
//    Some predefined data item names can generate simulated values.  These items
//    are defined using the SimulatedDataItem class (whose super class is EDDDataItem).
//    The EDDManager implements a thread that periodically acquires and
//    publishes data. 
//    The EDDDriver contains a collection of defined data items.
//    The Acquire() and Publish() methods are implemented
//    by the EDDDriver.  The EDDDriver calls Acquire() on each
//    data item for which the Agent has requested information.
//    The EDDDriver Publish() method has examples of how to publish
//    alarms ane events and how to initiate a file upload.
//
//  Project Files
//    - EDDDataItem:  Support class to define a data item.  
//      Classes:  EDDDataItem
//
//    - EDDAlarm:  Demonstrate how to publish Alarms.  This is NOT
//      required by all EDD drivers.
//      Class:  part of EDDDriver
//
//    - EDDBrowers:  Demonstrates how to browse a collection of data items.
//       For use by Builder.
//       Class:  EDDBrowser
//
//    - EDDConfig:  Demonstrate how to Parse a configuration XML file.
//      Used to define data items managed by this driver.
//      Class:  EDDConfig
//
//    - EDDDriver:  implementation of IEDDDriver.  Required.  Contains collection
//      of data items.
//      Class:  EDDDriver.
//
//    - EDDEvent:  Demonstrates how to publish Events.    This is NOT
//      required by all EDD drivers.
//      Class:  part of EDDDriver
//
//    - EDDExports:  Exported 'C' functions used by Builder
//
//    - EDDMgr:  implementation of IEDDManager.  Required.  Manages acquisition
//      and publishing thread.  Contains a collection of EDDDrivers.
//      Class:  EDDMgr
//
//    - EDDUpload:  Demonstrates how to initiate a file upload to the DRM Enterprise
//      Server.    This is NOT required by all EDD drivers.      
//      Class:  part of EDDDriver
// 
//    - pch.cpp/h:  The pre-compiled header files.
//
//    - SimulatedDataItem:  example Class derived from EDDDataItem used by this driver
//      to simulate data items that dynamically change.
//      Class:  SimulatedDataItem
//
//      
//*****************************************************************************
//  To EDD Developer
//    Do the following to modify this example to become an actual driver:
//    1.  Change EDD_EEDDExample_Initialize to EDD_xxx_ExInitialize where
//        xxx is the name of your EDD DLL.  (See EDDExports.cpp).
//    2.  Design how to acquire data for each data item.
//        Define a new class derived from EDDDataItem and implement the
//        virtual method Acquire().  Do this either in a generic manner  
//        or define a unique class for for each data item.
//    3.  Define data items collection in EDDDriver either using a XML file 
//        or directly instantiating the new data item class.
//    4.  Define the rate of data acquisition.  See m_acqRate in EDDMgr.
//    5.  Enhance the EDDDriver class functions:  StartDriver(), StopDriver(),
//        Acquire(), and Publish().
//
//*****************************************************************************

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"
#include "EDDAppInterface.h"

// A message file is used for stings displayed to the operator that may need 
// to be internationalized.
static  TCHAR* MESSAGES_FILE = _T("EDDMessages");

//*****************************************************************************
//  There should only be ONE EDD Manager. 
//
EDDMgr::EDDMgr(TCHAR* pID)
: m_esConfig(pID), m_bRunning(false), m_nRefCount(1), m_pIKernelLog(NULL),
  m_pIMsgManager(NULL), m_msgFileHandle(0), m_acqRate(DEFAULT_ACQUISITION_RATE)
{
}

//*****************************************************************************
EDDMgr::~EDDMgr()
{

    // Shutdown the example thread.
    m_bRunning = false;             // exit flag
    m_EDDEvent.Set();               // Signal thread;  Wake it up.
    m_AcquisitionThread.Wait();     // MUST wait for thead to exit

    m_drivers.RemoveAll();

}

//*****************************************************************************
//
// GetInterface() returns the requested interface
// 
// Allows your driver to support additional interfaces that may be used by other
// Agent components
//
//*****************************************************************************
bool EDDMgr::GetInterface(KERNEL_IID InterfaceID, void** ppInterface)
{
    g_pIKernelBase->GetInterface(InterfaceID, ppInterface);
    if (ppInterface == NULL)
        return false;

    return true;
}

//*****************************************************************************
//
// CreateEDDDriver() creates an instance of the driver derived from IEDDDriver.
//
// The caller is responsible for managing memory (releasing memory, etc.).
//
//*****************************************************************************
KERESULT EDDMgr::CreateEDDDriver(IEDDCallback* pICallback, TCHAR* pID, IEDDDriver*& pDriver,
                                             EDDDriverInfo* pEDDDriverInfo)
{

    EDDDriver* pEDDDriver = new EDDDriver(pICallback, pID, pEDDDriverInfo);
    pDriver = pEDDDriver;   // Set output parameter
    if (!pEDDDriver)
        return KE_FAILED;
    
    // update list of drivers.  
    // For a Gateway, there is a driver per device.
    m_driversMtx.Lock();
    m_drivers.AddTail(pEDDDriver);
    m_driversMtx.Unlock();

    return KE_OK;
}

//*****************************************************************************
//
// Destroy() destroys this object
//
//*****************************************************************************
KERESULT EDDMgr::Destroy()
{
    Release();
	return KE_OK;
}

//**************************************************************************
// Reference counting is useful if the driver supports Auto-Discovery.
void EDDMgr::AddRef()
{ 
    m_nRefCount++; 
}

//**************************************************************************
void EDDMgr::Release()
{
    m_nRefCount--; 
    if (m_nRefCount == 0) 
    {
        g_pEDDMgr = NULL;
        delete this;                // destroy object
    }
}

//*****************************************************************************
KERESULT EDDMgr::Initialize()
{
    // Get kernel logging interface for user messages.
    // 
    if (g_pIKernelBase)
    {
        g_pIKernelBase->GetInterface(IID_STR_MGR, (void**) &m_pIMsgManager);
        g_pIKernelBase->GetInterface(IID_KERNEL_LOG, (void**)&m_pIKernelLog);
        if (!m_pIMsgManager || !m_pIKernelLog)
            return KE_FAILED;

        // A message file is used for stings displayed to the operator that may need 
        // to be internationalized.
        if(m_pIMsgManager->LoadMsgFile(MESSAGES_FILE, m_msgFileHandle) != KE_OK)
        {
            EString esDebug;
            esDebug.Format(_T("Can not load EDD Driver message file: %s"), MESSAGES_FILE);
            m_pIKernelLog->Report(ET_ERROR, EDD_COMPONENT_ID, XML_STRING_ID,
                esDebug.GetPtr());
            return KE_FAILED;
        }

        m_enterpriseComm = EnterpriseCommunication();
        m_enterpriseComm.Initialize();

        m_agentLifecycle = AgentLifecycle();
        m_agentLifecycle.Initialize();
    }
    return KE_OK;
}

//*****************************************************************************
void EDDMgr::RemoveDriver(EDDDriver& eddDriver)
{
    m_driversMtx.Lock();
    POSITION pos = m_drivers.GetHeadPosition();
    while (pos)
    {
        POSITION prevPos = pos;
        EDDDriver* pDriver = m_drivers.GetNext(pos);
        if (pDriver == &eddDriver)
        {
            m_drivers.RemoveAt(prevPos);
            break;
        }
    }
    m_driversMtx.Unlock();
}

void EDDMgr::StartAcquistion()
{
    // Start example thread.  Only manager thread needed.
    if (!m_bRunning)
    {
        m_bRunning = true; 
        m_AcquisitionThread.Begin(AcquisitionThreadStub, (void*)this,
            _T("EDDFfssDriver"));  // name useful under debugging.

        m_PublishThread.Begin(PublishInterfaceThreadStub, (void*)this,
            _T("EDDFfssDriver2"));  // name useful under debugging.

    }
}

void EDDMgr::StopAcquistion()
{
    if (m_bRunning)
    {
        m_bRunning = false;
        // Signal thread to shutdown and wait for clean exit of thread.
        m_EDDEvent.Set(); 
        m_AcquisitionThread.Wait();

        // Signal thread to shutdown and wait for clean exit of thread.
        m_PublishThreadEvent.Set();
        m_PublishThread.Wait();
    }
}

//*****************************************************************************
//  Output Custom report message to the Kernel Log.
void EDDMgr::CustomReport(ReportClass reportType, UINT msgID)
{
    if (!m_pIKernelLog)
        return;

    m_pIKernelLog->CustomReport(m_msgFileHandle, reportType, EDD_MESSAGE_ID, msgID);
}

//*****************************************************************************
//  Output Custom report
void EDDMgr::CustomReport(ReportClass reportType, UINT msgID, EString& esMessage)
{
    if (!m_pIKernelLog)
        return;
    m_pIKernelLog->CustomReport(m_msgFileHandle, reportType, EDD_MESSAGE_ID, msgID, 
        esMessage.GetPtr());
}

//*****************************************************************************
//                          EThread Example
//*****************************************************************************

//*****************************************************************************
// Thread Example.  
ThreadReturn EDDMgr::AcquisitionThread()
{
    DWORD dwMilliSecWait = m_acqRate;

    while (m_bRunning)
    {
        // At signal of event (m_EEDEvent object) or after elapsed wait time, 
        // acquire and publish data and other desired processing.
        m_EDDEvent.Wait(dwMilliSecWait);
        m_EDDEvent.Reset();         // Reset signal
        if (!m_bRunning)
            break;

        // Measure length of time spent during acquisition.
		ETime startTime;

        // Acquire and publish data.
        //m_driversMtx.Lock();
        POSITION pos = m_drivers.GetHeadPosition();
        while (pos)
        {
            EDDDriver* pDriver = m_drivers.GetNext(pos);
            pDriver->Acquire();
            //pDriver->Publish();
        }
        //m_driversMtx.Unlock();

        // Reset wait time based on length of time in acquisition.
		ETime endTime;      // resolution of milliseconds.
		DWORD elapsedTime = (DWORD) (endTime.DiffTime(startTime) * 1000);
        if (elapsedTime > m_acqRate)  
            dwMilliSecWait = 0;
        else
            dwMilliSecWait = m_acqRate - elapsedTime;
    }

    return (ThreadReturn) 1;
}

ThreadReturn EDDMgr::PublishInterfaceThread()
{
    DWORD dwMilliSecWait = m_acqRate;

    while (m_bRunning)
    {
        // At signal of event (m_EEDEvent object) or after elapsed wait time, 
        // acquire and publish data and other desired processing.
        m_PublishThreadEvent.Wait(dwMilliSecWait);
        m_PublishThreadEvent.Reset();         // Reset signal
        if (!m_bRunning)
            break;

        // Measure length of time spent during acquisition.
        ETime startTime;

        // Acquire and publish data.
        //m_driversMtx.Lock();
        POSITION pos = m_drivers.GetHeadPosition();
        while (pos)
        {
            EDDDriver* pDriver = m_drivers.GetNext(pos);
            pDriver->Publish();
        }
        //m_driversMtx.Unlock();

        // Reset wait time based on length of time in acquisition.
        ETime endTime;      // resolution of milliseconds.
        DWORD elapsedTime = (DWORD)(endTime.DiffTime(startTime) * 1000);
        if (elapsedTime > m_acqRate)
            dwMilliSecWait = 0;
        else
            dwMilliSecWait = m_acqRate - elapsedTime;
    }
    
    return (ThreadReturn) 1;
}

//*****************************************************************************
KERESULT EDDMgr::GetServerDiagnosis(EString& esDiagnosis)
{
    return m_enterpriseComm.GetServerDiagnosis(esDiagnosis);
}

//*****************************************************************************
KERESULT EDDMgr::RestartAgent()
{
    return m_agentLifecycle.RestartAgent();
}
