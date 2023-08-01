//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  KernelInteface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define kernel interfaces
//
//  Interfaces:
//                  IKernelBase
//                  IKernelService
//                  IKernelLog
//                  ILogMessage  (Immutable)
//                  IDispatcher
//                  IGatewayListener
//                  IGateway
//                  IDeviceState
//
//**************************************************************************
#ifndef __KERNEL_INTERFACE_H__
#define __KERNEL_INTERFACE_H__

#include "KernelPublicIID.h"
#ifndef __KERNEL_ERRORS_H__
#include "KernelErrors.h"
#endif

interface IXdrMessageHandler;      // forward reference
interface IKernelService;
typedef ETypedPtrList<EPtrList, IKernelService*>   CServiceList;

// group=General Interfaces


////////////////////////////////////////////////////////////////////////
// Common Kernel interfaces.

//**************************************************************************
interface IKernelBase
{
   virtual bool GetInterface(KERNEL_IID InterfaceID, void** ppInterface) = 0;
};


#define     NO_GATEWAY      (-1)
#define     CONNECTOR_GATEWAY   NO_GATEWAY
#define     OUT_OF_GATEWAY_IDS  NO_GATEWAY
#define     INVALID_DEVICE_ID   (-2)


////////////// struct used by warm restart //////////////////////////////////
typedef struct
{
    TCHAR*  m_pBuffer;
}WARM_RESTART_STRUCT;

//**************************************************************************
interface IKernelService : public IKernelBase
{
    virtual KERESULT Save(EHANDLE pData) = 0;
    virtual KERESULT Restore(EHANDLE pData) = 0;
#ifdef __GATEWAY__
    virtual KERESULT Restore(EHANDLE pData, int gatewayDeviceId) = 0;
#endif
    virtual KERESULT PrepareToStart() = 0;
    virtual KERESULT PreStart() = 0;
    virtual KERESULT Start() = 0;
    virtual KERESULT PrepareToStop() = 0;
    virtual KERESULT Stop() = 0;
    virtual KERESULT AddService(IKernelService* pService, CServiceList*) = 0;

    virtual KERESULT Destroy() = 0;
    virtual KERESULT GetComponentId(DWORD& componentId) = 0;

    // warm restart support
    typedef enum{WR_SETUP, WR_SAVE_DATA, WR_CLEANUP, WR_XMLFILE, WR_RESTORE} WR_OPERATION;

    // CKernelShell will call all components to do this
    virtual KERESULT WarmRestart(WR_OPERATION op, WARM_RESTART_STRUCT* pParam) = 0;
};


typedef IKernelService* (*INIT_SERVICE_FUNCTION)(IKernelBase*, LPCTSTR);

// IKernelLog - Event Types.  Use w/ IKernelLog->Report
enum ReportClass
{
    ET_DEBUG = 1,
    ET_INFO = 3,
    ET_WARNING = 5,
    ET_IMPORTANT_INFO = 6,
    ET_ERROR = 9,
    ET_CRITICAL_ERROR = 10
};


// useful English.xml defines
#define XML_DEBUG_ID    529        
#define XML_STRING_ID   121         // general purpose "%s" message.

//**************************************************************************
// Event Reporting Functionality.  
// These messages are from a translatable XML file.
interface IKernelLog
{
public:
    virtual KERESULT Report(
                            DWORD dwEventType,  
                            UINT sourceID, 
                            UINT msgID,
                            ...
                            ) = 0;

    virtual KERESULT CustomReport(
                            MSGFILE fileID,
                            DWORD dwEventType, 
                            UINT sourceID, 
                            UINT msgID,
                            ...
                            ) = 0;
    virtual KERESULT ForceWrite(void) = 0;
    virtual KERESULT Shutdown() = 0;
};


// Interface for simply reporting messages to the display.  
// These messages are not translated.
// To obtain this interface need GetInterface(IID_LOG_MESSAGE, (void**) &pILogMessage).
interface ILogMessage
{
    // This interface is immutable.

    // Report the type of message, the DLL/component which is the source of the message,
    // and the Message itself.
    virtual KERESULT Report(DWORD dwEventType,  TCHAR* pszSourceDll, TCHAR* pszMessage) = 0;
};

//**************************************************************************
interface IMsgManager
{
public:
    // Event Reporting Functionality
    virtual TCHAR* GetString(UINT msgID, MSGFILE fileHandle = 0) = 0;

    virtual KERESULT LoadMsgFile(TCHAR* msgFileName, MSGFILE& msgFileHandle) = 0;
};

// group=Network

// API Dispatcher interface.  Used by each component to register for API 
// functional data.
class EMsgHeader;
class EXdrMessage;
class EAPI_Callback_Stub;

//**************************************************************************
interface IDispatcher
{
    virtual KERESULT    Register(IXdrMessageHandler *pMsgHandler, 
                                    KERNEL_IID componentId) = 0;
    virtual KERESULT    Unregister(KERNEL_IID componentId) = 0;

    // RPC function
    virtual bool        ProcessRpcRequest(HREMOTE pClient,EMsgHeader& header, EXdrMessage& args,EXdrMessage& ans) = 0;

    // Call Back function
    virtual bool        ProcessCallback(HREMOTE pClient,EMsgHeader& header, EXdrMessage& msg) = 0;

    virtual void        RegisterCallback(HREMOTE hRemote, EHANDLE hKey, EAPI_Callback_Stub *pFn) = 0;
    virtual KERESULT    UnregisterCallback(HREMOTE hRemote, EHANDLE hKey) = 0;
    virtual KERESULT    GetCallback(HREMOTE hRemote, EHANDLE hKey, EAPI_Callback_Stub **ppFn) = 0;
};


/* Group=Gateway */
// Forward Reference
class EDWordArray;

// GatewayStatus:
// The GatewayStatus is used to track the device from discovery through the
// project "Restore" stage. (NOTE:  in 3.6 they are tracked through the "Prestart" stage.)
// This is primarily intended for internal use by
// the Gateway component, but is also exposed for use by Custom Components.
// Also, the KernelShell, sets the Status to gsRESTORED and can attempt to
// restore a device that previously failed to restore.
// 
// This status can be read/set by standard or custom components.
// Custom components should only set:  gsNEW, gsPRESTARTED, and gsERROR.
// A custom component that wants the xGateway component to manage the device
// but have the custom component manage the project (data, events, alarms), should
// set the device status to gsPRESTARTED after adding the device.
//
// A device DOES NOT EXIST if it is in either the gsERROR or gs_WARM_RESTORE state.
//
enum GatewayStatus
{
    gsNOT_FOUND,        // not in current device list
    gsNEW,              // just added, but not yet restored.
    gsRESTORED,         // project successfully restored.  
    gsFAILED_RESTORE,   // Could not restore project successfully.
    gs_WARM_RESTORE,    // Restored during a warm restart. Temporary flag.  If found, set to gsNEW;
                        // if not found (defined or rediscovered) the device is considered
                        // invalid (doesn't exist).
    gsERROR,             // Device error or some sort.
    gsPRESTARTED,       // 3.6 All components are prestarted.  
                        // Device must have status of gsPRESTARTED for the state to be
                        // enabled and/or registered.
    gsREMOVED           // 3.9.  Allows a device to be marked as removed.
};


//**************************************************************************
// Allow other components to monitor Gateway device changes.
// The GatewayChangeType allows extensibility without an interface change.
//   
enum GatewayChangeType
{
    ctRESTORED,             // device restored, but not prestarted.
    ctPRESTARTED,           // all components prestared.
                            // A "removed device" that is reAdded will notify using ctPRESTARTED.
    ctCONFIGURATION,        // future:  topography change.
    ctREMOVED               // device "removed"  (3.9)
};

interface IGatewayListener
{
    virtual KERESULT OnDeviceChange(int deviceId, GatewayChangeType type) = 0;
};

// Forward reference
class EMnInfos;
class AUTO_DISCOVERY_INFO;

//**************************************************************************
interface IGateway : public IKernelBase
{
    // Sets pId based on unique owner - model number - serial number
    // If esOwner is empty, the default owner is used.
    virtual KERESULT    GetId(/* in */ EString& esMn, /* in */ EString& esSn,
                              /* in */ EString& esOwner, /* out */ int* pId) = 0;   
    // Get All IDs for serial numbers for a particular model number
    // If esOwner is empty, the default owner is used.
    virtual KERESULT    GetSnIds(/* in */ EString& esMn,
                                 /* in */ EString& esOwner,   
                                 /* out */ EDWordArray* pIds) = 0;
    // Get the model id from any device id from that model.  
    // The "model id" is the id of the first device added from a model group.
    // The model id is often used in name qualification for compoenent names
    // at a "model" level (e.g Timer).
    virtual KERESULT    GetModelId(/* in */ int deviceId, 
                                   /* out */ int *pMnId) = 0;

    // Get all the models.
    virtual EMapStringToPtr&    GetModels() = 0;

    // Get the model id using the model number string.
    virtual KERESULT    GetModelId(/* in */ EString& esMn, /* out */ int *pMnId) = 0;

    // Compares input 'id' to ids in model number collection to see if id
    // is the first device associated with a specific model.
    // This is often used in Model Level name qualifications.
    virtual KERESULT    IsNewMn(/* in */ int id, /* out */ bool* pbNewMn)= 0;

    // Modifies pName to be fully qualified.  name becomes name$id
    // The final name will look like:  Branch$id.SubBranch$idTag$id. 
    virtual KERESULT    CreateQualifiedName(/* in */ int id,  
                                              /* in/out */ EString* pName) = 0;
    // Modifies a fully qualified pName to be unqualified.  name$id becomes name.
    // pId contains the device id if pId is not null.
    virtual KERESULT    ParseQualifiedName(/* in/ out */ EString* pName, 
                                          /* out */ int* pId = NULL) = 0;
    virtual KERESULT    ParseQualifiedName(TCHAR* pszName, int* pId = NULL) = 0;

    // from id sets device's model number and serial number strings.
    // if id is CONNECTOR_GATEWAY, then return mn/sn from ELicense.xml
    virtual KERESULT    GetMnSn(/* in */ int id, /* out */ EString* pesMn, 
                                /* out */ EString* pesSn,
                                /* out - optional*/ EString* pesOwner = NULL) = 0;

    // For use by Custom Component and Intenally by Restore.
    // Allows definition of Gateway ID for an externally defined (not in 
    // xGateway.xml) device.  If the input model number (esMn) or owner (esOwner)
    // is empty - the License.xml file's model and/or owner will be used.  This
    // allows a custom component to only know the serial number.
    //
    // This function is only for Gateway managing a new device's ID in relationship
    // to its owner/mn/sn.
    // This routine does *NOT* modify any other componets or facilitate the
    // creation of qualified names.
    //
    // Allow AddDevice to have a SN configuration string.  Used with Auto Discovery
    // where the driver needs additional information per serial number.
    // Note, change of order of parameters from original AddDevice.
    //
    // For device's online/offline state see commented function: SetDefaultDeviceState()
    virtual KERESULT  AddDevice(/* in */ EString& esOwner,
                                /* in */ EString& esMn, 
                                /* in */ EString& esSn,
                                /* in */ EString& rSnConfig, // may be empty.
                                /* out */ int* pId) = 0;

    // from id sets device's serial number configuration string.
    // This is used by the Data Source driver to define device specific
    // configuration.
    virtual KERESULT    GetSnConfig(/* in */ int id, /* out */ EString* pesSnConfig) = 0; 

    //  A Gateway model may be disabled.  One reason is a bad project or run-time
    //  error.  A component that does output may need to check if a model is 
    //  disabled before outputing results. 
    //  This may be temporary until a mechanism is developed to "unload" tags
    //  after they have been created and/or running.
    virtual KERESULT    DisableModel(/* in */ int id, /* in */ bool bState = true) = 0;
    virtual KERESULT    IsModelDisabled(/* in */ int id, /* out */ bool* pbDisabled) = 0;

//**************************************************************************
// Add 2.6 Features.
//**************************************************************************

    // Get All Gateway IDs.  Use this instead of GetStartId(), GetNextId()
    // Clears existing memory and fills pIds with the ids of all Gateway devices
    // regardless of current status.
    virtual KERESULT    GetIds(/* out */ EDWordArray* pIds) = 0;

    // Allow tracking of each device's current status.  A device's project is ready to
    // actively run when it's status is gsPRESTARTED (gsRESTORED in 3.5).
    virtual KERESULT    GetStatus(/* in */ int id, /* out*/ GatewayStatus* pGatewayStatus) = 0;

    // Allow a custom component to set a device's status.
    virtual KERESULT    SetStatus(/* in */ int id, /* in*/ GatewayStatus gatewayStatus) = 0;

    // *** Each device maintains independent 3 states:  
    // *** Enabled/Disabled,   Registered/Not-Registered, On-line/Off-line.
    // *** If a device is disabled, the Tag Manager will no longer publish its data.
    // *** If a device is off-line, it will no-longer ping the Enterprise.

    // This allows a standard or custom component to enable/disable a device.
    // A will only return true to IsDeviceEnabled if the GatewayStatus == gsPRESTARTED 
    // (gsRestored in 3.5)
    virtual KERESULT    DisableDevice(/* in */ int id, /* in */ bool bState = true) = 0;

    // Get the Enabled/Disabled state of the device.
    // If a device is disabled, the Tag Manager will no longer publish its data.
    virtual KERESULT    IsDeviceDisabled(/* in */ int id, /* out */ bool* pbDisabled) = 0;

    // This allows a custom component to set a device managed by the Gateway component
    // off-line.
    // !!! NOTE:  AS of 3.9, this function should ONLY be called by the Kernel itself, 
    // NOT by a component or driver directly!!!
    virtual KERESULT    SetDeviceOffLine(/* in */ int id, /* in */ bool bState = true, 
        bool bLogReport = true) = 0;
    // Get the on-line / off-line state of the device.
    // If a device is off-line, it no-longer pings the Enterprise.
    virtual KERESULT    IsDeviceOffLine(/* in */ int id, /* out */ bool* pbOffLine) = 0;

    // Set device state to being managed externally.  This device's on-line/off-line state
    // will be set externally.  It will be ignored during AutoDiscovery check for 
    // missing devices.
    virtual KERESULT    DeviceManagedExternally(/* in */ int id, /* in */ bool bState = true) = 0;

    // Allow other components to monitor when a new gateway device is added.
    virtual KERESULT    Subscribe(IGatewayListener* pIListener) = 0;
    virtual KERESULT    Unsubscribe(IGatewayListener* pIListener) = 0;

    // Indicates if Gateway component is managing auto-discovery functions.
    virtual KERESULT    IsAutoDiscoveryEnabled(bool* pbAutoDiscovery) = 0;

    // Start Device via a Custom Component.  After calling AddDevice(),
    // the custom component should call StartDevice().  If the device's
    // project is to be restored, set the bRestore flag to true.
    //
    // For device's online/offline state see commented function: SetDefaultDeviceState()
    virtual KERESULT    StartDevice(int id, bool bRestore) = 0;

//**************************************************************************
// Add 3.5 Features.
//**************************************************************************

    // Creates directory tree for the device under user-provided path
    virtual KERESULT    CreateDirectoryTree(int id, EString* pesPath) = 0;

    // Get the device (relative) path:  mn\sn\ (OS delimiters used).
    // The path is appended to any existing path that is passed in.
    virtual KERESULT    GetDevicePath(/* in */ int id, /* in/out */ EString* pesPath) = 0;

    // Get the device (relative) directory; the actual location of device files.
    // for example:  DefaultProject\mn\sn\.  
    virtual KERESULT    GetDeviceDirectory(/* in */ int id, /* out */ EString* pesDeviceDir) = 0;

    // Modify the resFilename so that the path is device relative.
    // in/out:  resPath.  in:  relative path.  out:  pre-pend the device directory.
    // This does nothing for a Connector.
    virtual KERESULT    SetDeviceRelativePath(/* in */ int gatewayId,
        /* in / out */ EString* pesFileName) = 0;

    // Set true if device AutoDiscovered or externally managed.
    virtual KERESULT    IsDeviceAutoDiscovered(/* in */ int id, /* out */ bool* pbAutoDiscovered) = 0;

//**************************************************************************
// Add 3.6 Features.
//**************************************************************************
    // Get the device info with name and value map
    virtual KERESULT    GetDeviceInfo(/* in */ int id, /* in */ EString& esName, /* out */ EString& esValue) = 0;

    // Set the device info with name and value map (Gateway broadcasts
    // ctCONFIGURATION update to subscribed listeners as result of this call)
    virtual KERESULT    SetDeviceInfo(/* in */ int id, /* in */ EString& esName, /* out */ EString& esValue) = 0;
    virtual KERESULT    SetDeviceInfo(/* in */ int id, /* in */ EMapStringToString& map) = 0;

//**************************************************************************
// Add 3.7 Features.
//**************************************************************************
    // Force Discovery of all plug-ins.  Return all discovered mn (& sn) that
    // can be supported by existing projects.
    // DON'T add these mn/sn into the running gateway.
    virtual KERESULT    ReDiscover(EMnInfos* pMnInfos) = 0;

    // Force Discovery of all plug-ins (force scan loop to run).  
    // Discovered devices are added to the Gateway in the normal manner.
    virtual KERESULT    ReDiscover() = 0;

    // Does the device exist in the system.  
    // A device is not valid (doesn't exist), when it was present at shutdown 
    // (with "WarmRestart" on) and was not present (discovered or defined) when
    // the Agent restarted.
    virtual KERESULT    IsDeviceValid(/* in */ int id, /* out */ bool &bExists) = 0;

    // Set / Get Scan Period per plug in module.  
    // If pszPlugInName is NULL, then set Period 1st plug in list
    // Set scanPeriod to -1 to disable scan.
    // Scan Period is in minutes.
    virtual KERESULT    SetScanPeriod(TCHAR* pszPlugInName, DWORD scanPeriod) = 0;
    virtual KERESULT    GetScanPeriod(TCHAR* pszPlugInName, DWORD& rScanPeriod) = 0;

//**************************************************************************
// Add 3.9Features.
//**************************************************************************

    // Remove device.  This is used by EDD Driver when device is removed.
    // This is similar to off-line, but the internally the device is marked
    // as "removed".  After removing a device, the driver needs to call
    // StartDevice().  This allows the IGatewayListener to be updated via a
    // callback. Calling RemoveDevice() also sets the device off-line.
    virtual KERESULT  RemoveDevice(int id) = 0;

    // Retrieve generic auto-disovery settings for given plugin
    virtual KERESULT  GetAutoDiscoveryInfo(TCHAR* pszPlugInName, AUTO_DISCOVERY_INFO *pInfo) = 0;

//**************************************************************************
// 5.0 Features.
//**************************************************************************

    // query/set whether APM policy is defined for the device. this flag
	// used to be maintained through the Gateway's device info map, but since
	// the latter may be potentially persisted now, make this a separate flag.
    virtual KERESULT IsPolicyDefined(int id, bool& bPolicyDefined) = 0;
    virtual KERESULT SetPolicyDefined(int id, bool bPolicyDefined) = 0;

    // 5.3
    // The default state of a device is 'on-line'.  
    // SetDefaultDeviceState() allows you to set the device's default state to offline.
    // Once set, all newly added devices will default to defined device state.
    // The device state effects when dynamically adding devices using the methods
    // AddDevice() and StartDevice().  Call SetDefaultDeviceState() before calling 
    // AddDevice().
    // If you want devices defined by Deployment Utility (or Builder) to
    // default to 'offline', call SetDefaultDeviceState() in a custom
    // component's start up sequence in the function PrepareToStart(). This
    // sets the default state before the pre-defined managed devices are 
    // defined from reading configuration files.
    virtual KERESULT SetDefaultDeviceState(bool bOffline = false) = 0;
    virtual KERESULT GetDefaultDeviceState(bool& bOffline) = 0;

};

//**************************************************************************
// IDeviceStatus Interface
// Added V3.9 for managing Connector and Gateway's devices state.
//
typedef enum {
    DevStateUnknown = 0,
    DevStateOnline,
    DevStateOffline
} DEVICESTATE;

interface IDeviceState
{
    virtual KERESULT SetDeviceOffLine(int id, bool bState, bool bLogReport) = 0;
    virtual KERESULT IsDeviceOffLine(int id, bool* bpState) = 0;
};

//**************************************************************************
// IKernelShell Interface
// Added V3.5 for Software Management to retrieve agent information.
//
interface IKernelShell
{
    // Retrieve a list of module names loaded by the agent.
    virtual KERESULT GetFirstModule(EString& resModule) = 0;
    virtual KERESULT GetNextModule(EString& resModule) = 0;

    // version format Major.Minor.Build.Patch  (patch only added if non-zero)
    virtual KERESULT GetVersion(EString& resVersion) =0;

    // Used to get the Agent's project path (e.g. DefaultProject).  
    // To be used instead of EPersistent::SetProjectPath();
    virtual KERESULT SetProjectPath(TCHAR* pszPath) = 0;
    virtual KERESULT GetProjectPath(EString& resPath) = 0;

    // since 5.1:
    virtual KERESULT IsShuttingDown(bool& bShuttingDown) = 0;
    virtual KERESULT IsRunningAsService(bool& bRunningAsService) = 0;
};

#endif

