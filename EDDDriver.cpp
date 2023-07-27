//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDDriver.cpp
//  
//  Subsystem  :  
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                EDDDriver implements the requied interface:  IEDDDriver
//
//                Code Developer needs to implement StartDriver() and StopDriver().
//                Also evaluate changes to the support functions:  Acquire() and
//                Publish() where are periodically called by the EDDMgr's 
//                AcquisitionThread().
//                Note:  If using the general purpose EDDDataItem, this function
//                will need minor modifications.
//
//                The Axeda DRM Gateway will create an instance of the EDDDriver
//                for each device it manages. 
//
//  Classes    :  
//                EDDDriver

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"
#include "EDDBrowser.h"
#include "string"
#include <fstream>
#include <sstream>
#include <vector>



// Commands that will be sent from the app
constexpr const char* APP_CMD_AGENT_CONNECTION = "connectionStatus";
constexpr const char* APP_CMD_UPDATE_METADATA  = "updateMetaData";
constexpr const char* APP_CMD_CLOUD_CONNECTION_STATUS = "cloudConnectionStatus";
constexpr const char* APP_CMD_ASSOCIATION_KEY_REQ = "associationKeyReq";
constexpr const char* APP_CMD_SW_UPDATE_STATUS = "swUpdateStatus";

// Data item name that will be read from the agent
constexpr const char* ASSOCIATION_KEY_RESP_DATA_ITEM = "Association_response";
constexpr const char* ENTERPRISE_STATUS_DATA_ITEM    = "Connection_status";

static const TCHAR* OFFLINE_VALUE = _T("offline");
static const TCHAR* ONLINE_VALUE  = _T("online");
//*****************************************************************************
//                 EDDDriver  Implemenation
//*****************************************************************************

//*****************************************************************************

//*****************************************************************************
EDDDriver::EDDDriver(IEDDCallback* pCallback, TCHAR* pIdentifier,
                                   EDDDriverInfo* pEDDDriverInfo)
: m_pIEDDCallback(pCallback), m_esConfig(pIdentifier), m_hAlarm(0),
m_json({}),
m_deviceDataJson({})
{
    // Save the driver's device identification.
    if (pEDDDriverInfo)
    {
        m_esMn = pEDDDriverInfo->m_esMn;
        m_esSn = pEDDDriverInfo->m_esSn;
        m_esOwner = pEDDDriverInfo->m_esOwner;
    }

    EDDConfig config(m_items);
    config.Init();
}

//*****************************************************************************
EDDDriver::~EDDDriver()
{
    ELock itemsLock(m_itemsMtx);
    m_items.DeleteAll();
}

//*****************************************************************************
//
// Destroy() destroys this object
//
//*****************************************************************************
KERESULT EDDDriver::Destroy()
{

    g_pEDDMgr->RemoveDriver(*this);
	delete this;
	return KE_OK;
}

//*****************************************************************************
//
// StartDriver() starts the driver and performs driver-specific operations.
//
// StartDriver() is called once to put the driver into run mode. The driver should do 
// whatever specific tasks it needs to do when starting the driver such as making 
// connections to the device or setting up configurations.
//
//*****************************************************************************
KERESULT EDDDriver::StartDriver()
{
    SysAppIntf::Instance().Initialize();
    g_pEDDMgr->StartAcquistion();
	return KE_OK;
}

//*****************************************************************************
//
// StopDriver() stops the driver and performs driver-specific operations. 
//
// StopDriver is called once to put the driver into stop mode. The EDD driver should 
// perform whatever device-specific tasks it needs to do to stop the driver, such as 
// closing connections to the device.
//
//*****************************************************************************
KERESULT EDDDriver::StopDriver()
{
    // Returns KE_OK on successful; otherwise, returns KE_FAILED or other failure code(s)

    SysAppIntf::Instance().KillProcess();
    g_pEDDMgr->StopAcquistion();
    return KE_OK;
}


//*****************************************************************************
//
// WriteTagValues() writes tag values.
//
// The EDD driver should check if the operation is possible and then write the values.
//
// Returns KE_OK only if all items are written successfully. 
// The result of each tag item's operation is also set in pResult.
//
//*****************************************************************************
KERESULT EDDDriver::WriteTagValues(int iTags, EDDHANDLE* pHandles, CDataValue* pValues,
                                          KERESULT* pResults)
{
    if (iTags == 0 || pHandles == NULL || pValues == NULL)
        return KE_INVALID_PARAMS;

    KERESULT retCode = KE_OK;
    m_itemsMtx.Lock();
    for (int i = 0; i < iTags; i++)
    {
        EDDDataItem* pItem = m_items.Find(pHandles[i]);
        KERESULT ker = KE_OK;
        if (pItem)
        {
            if (pItem->IsReadOnly())
                ker = KE_ACCESS_RIGHTS;
            else
            {
                ker = pItem->SetValue(pValues[i]);

                // TODO: Test code check if offline
                // if CONNECTION_STATUS property has value "offline"
                if (pItem->GetName() == ENTERPRISE_STATUS_DATA_ITEM && pItem->GetValue().GetStringValue() == OFFLINE_VALUE)
                {
                    // Get server state as string and output to log
                    /*EString esDiagnosis;
                    KERESULT ker = g_pEDDMgr->GetServerDiagnosis(esDiagnosis);

                    ASSERT(ker == KE_OK);

                    g_pEDDMgr->CustomReport(ET_INFO, 2, esDiagnosis);*/

                    // Trigger restart
                    //g_pEDDMgr->RestartAgent();

                    return KE_OK;
                }
            }
        }
        else 
        {
            ker = KE_NOTFOUND;
        }
        if (pResults)
            pResults[i] = ker;

        if (ker != KE_OK)
            retCode = KE_FAILED;
    }
    m_itemsMtx.Unlock();

    // Update Agent with written changes.
    PublishData();

    return retCode;
}


//*****************************************************************************
//
// ReadTagValues() reads tag values.
//
// The EDD driver should check if the operation is possible and then read the values.
//
// Returns KE_OK only if all items are read successfully. 
//
//*****************************************************************************
KERESULT EDDDriver::ReadTagValues(int iTags, EDDHANDLE* pHandles, CDataValue* pValues)
{
    if (iTags == 0 || pHandles == NULL || pValues == NULL)
        return KE_INVALID_PARAMS;

    ELock itemsLock(m_itemsMtx);
    for (int i = 0; i < iTags; i++)
    {
        EDDDataItem* pItem = m_items.Find(pHandles[i]);
        if (pItem)
        {
            pValues[i] = pItem->GetValue();  // get value even if not dirty.
        }
    }
    return KE_OK;
}

//*****************************************************************************
//
// AddItem() requests that this server data item be accessible by clients.
//       - If pushback is 1, the driver should send data changes for this item when modified.
//       - If pushback is 0, the driver should wait for the client to read this tag.  
//
//
//*****************************************************************************
KERESULT EDDDriver::AddItem(EDDHANDLE handle, const EString& name, int pushData)
{
    ELock itemsLock(m_itemsMtx);
    EDDDataItem* pItem = m_items.Find((EString&) name);
    if (pItem == NULL)
        return KE_NOTFOUND;
    bool bPush = (pushData) ? true : false;
    pItem->Enable(handle, bPush);
    return KE_OK;
}

//*****************************************************************************
//
// RemoveItem() requests that the data item (specified by a handle) be removed  
// from data change notifications.
//
//*****************************************************************************
KERESULT EDDDriver::RemoveItem(EDDHANDLE handle)
{
    ELock itemsLock(m_itemsMtx);
    EDDDataItem* pItem = m_items.Find(handle);
    if (pItem == NULL)
        return KE_INVALID_NAME;

    pItem->Disable();

    return KE_OK;
}

//*****************************************************************************
//
// Acknowledge(): Acknowledge the alarm from AlarmManager
//
//  If managing device Alarms, notify the device that the alarm has been acknowledged.
// 
//*****************************************************************************
KERESULT EDDDriver::Acknowledge(ALARM_HANDLE alarmID)
{
	return KE_ALARM_NOT_FOUND;
}



//*****************************************************************************
//                      Support  Functions
//*****************************************************************************

//*****************************************************************************
// General purpose routine called periodically by EDDMgr's AcquisitionThread
// to acquired data.  In this example, the acquisition responsiblity is 
// deferred to the data item itself.
void EDDDriver::Acquire()
{
    // Set one time stamp for all acquired items.
    ETimeStamp now;
    ETime::GetTimeStamp(&now);

    std::string messageFromApp;
    if (SysAppIntf::Instance().WaitForSysAppCmd(messageFromApp))
    {
        MessageProcessor(messageFromApp);
    }
}

void EDDDriver::MessageProcessor(std::string& messageFromApp)
{
    std::vector<std::string> cmdParameters;
    parseMessageFromApp(messageFromApp, cmdParameters);

    std::string operationCmd = cmdParameters[0];
    EString operationCmdEStr = EString(operationCmd.c_str());
    EString logMsg(_T("RxD command: ") + operationCmdEStr);
    g_pEDDMgr->CustomReport(ET_INFO, 4, logMsg);

    if (operationCmd == APP_CMD_AGENT_CONNECTION)
    {
        std::string response = "Connected";
        SysAppIntf::Instance().SendToSysApp(response.c_str(), true);
    }
    else if (operationCmd == APP_CMD_CLOUD_CONNECTION_STATUS)
    {
        std::string response = "offline";
        EString enterpriseStatusDataName(ENTERPRISE_STATUS_DATA_ITEM);
        EDDDataItem* connectionStatus = m_items.Find(enterpriseStatusDataName);

        if (connectionStatus != NULL &&
            (wcscmp(connectionStatus->GetValue().GetStringValue(), ONLINE_VALUE) == 0))
        {
            response = "online";
        }
        else
        {
            // Get server state as string and output to log
            EString esDiagnosis;
            KERESULT ker = g_pEDDMgr->GetServerDiagnosis(esDiagnosis);
            std::wstring esDiagnosisWStr(esDiagnosis);
            response = std::string(esDiagnosisWStr.begin(), esDiagnosisWStr.end());
        }

        SysAppIntf::Instance().SendToSysApp(response.c_str(), true);
    }
    else if (operationCmd == APP_CMD_UPDATE_METADATA)
    {
        EDDDataItems dummyItems;
        EDDConfig config(dummyItems);

        // Read latest from file
        config.Update(m_dataItemsMap);

        // Now update the items
        UpdateDataItemsFromMap();

        std::string response = "OK";

        SysAppIntf::Instance().SendToSysApp(response.c_str(), true);
    }
}

//*****************************************************************************
// General purpose routine called periodically by EDDMgr's AcquisitionThread
// to publish information.  
// 
// The publishing of Events, Alarms, and upload of a file are here only as examples.
// 
void EDDDriver::Publish()
{
    PublishData();

    // Set false to understand how alarms/events are published and files are uploaded.
    static bool bPublishedOnce = true;  
    
    // For example reasons, only publish alarms/events and upload once.
    if (bPublishedOnce == false)
    {
        // The driver can also publish Alarms and Events.
        // To enable example, set flag to true.  For example: m_bStartAlarm = true; 
        PostEvent(_T("Demo Event"), _T("Demo event message"), 100);
        StartAlarm(_T("Demo Alarm"), _T("Demo alarm message"), 100);

        // The Driver can also upload files to the DRM Enterprise
        UploadFile(_T("English.xml"));

        bPublishedOnce = true;
    }      
     
}

//*****************************************************************************
// Publish only data changes as a block to the Agent.
void EDDDriver::PublishData()
{
    m_itemsMtx.Lock();

    // find number of dirty items.
    int cnt = 0;
    POSITION pos = m_items.GetHeadPosition();
    while (pos)
    {
        EDDDataItem* pItem = m_items.GetNext(pos);
        EString& esName = pItem->GetName();
        if (IsItemPublishable(pItem))
        {
            cnt++;
        }
    }    
    if (cnt == 0)
    {
        m_itemsMtx.Unlock();
        return;
    }

    // fill arrays for data for callback into Agent.
    EDDHANDLE* pHandles = new EDDHANDLE[cnt];
    CDataValue* pDataValues = new CDataValue[cnt];

    int ix = 0;
    pos = m_items.GetHeadPosition();
    while (pos)
    {
        EDDDataItem* pItem = m_items.GetNext(pos);
        EString& esName = pItem->GetName();
        if (IsItemPublishable(pItem))
        {
            pHandles[ix] = pItem->GetHandle();
            pDataValues[ix] = pItem->GetValue();
            pItem->ClearDirty();
            ix++;
        }
    }    
    ASSERT(ix == cnt);  // verify there was no overflow.
    m_itemsMtx.Unlock();

    // Publish only data changes as a block to the Agent.
    m_pIEDDCallback->OnDataChange(cnt, pHandles, pDataValues);

    // Driver is responsible to free allocated memory.
    delete [] pHandles;
    delete [] pDataValues;
}


//*****************************************************************************
bool EDDDriver::IsItemPublishable(EDDDataItem* pItem)
{
    bool isPublishable = pItem->IsDirty() && pItem->IsPushEnabled() && pItem->IsActive();
    return isPublishable;
}

bool EDDDriver::parseMessageFromApp(std::string cmd, std::vector<std::string>& cmdParameters)
{
    std::stringstream cmdStream(cmd);
    std::string parameter;

    while (std::getline(cmdStream, parameter, '@'))
    {
        cmdParameters.push_back(parameter);
    }

    return true;
}



KERESULT EDDDriver::UpdateDataItemsFromMap()
{
    m_itemsMtx.Lock();
    POSITION pos = m_items.GetHeadPosition();
    while (pos)
    {
        EDDDataItem* pItem = m_items.GetNext(pos);
        EString esName = pItem->GetName();
        std::wstring esNameWstr(esName.GetPtr());
        std::string esNameStr(esNameWstr.begin(), esNameWstr.end());
        if (m_dataItemsMap.find(esNameStr) != m_dataItemsMap.end())
        {
            EString value(m_dataItemsMap[esNameStr].c_str());
            pItem->SetValue(value);
        }
    }

    m_itemsMtx.Unlock();
    return KE_OK;
}