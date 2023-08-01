//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ENotifyInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  
//
//  Classes:      ITriggerListener
//                ITrigger
//                IErrorListener
//                IAction                
//
//**************************************************************************
#ifndef __NOTIFY_INTERFACE_H__
#define __NOTIFY_INTERFACE_H__

#ifndef __KERNEL_INTERFACE_H__
#include "KernelInterface.h"
#endif

#include "ETime.h"

/* Group=Notify */

/* Group=Notify Alarm Subconditions */

#define ALM_SUBCOND_LOW         1     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_HIGH        2     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_LOLO        3     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_HIHI        4     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_DV_LO       5     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_DV_HI       6     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_DIG_LO      7     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */
#define ALM_SUBCOND_DIG_HI      8     /* See Also: EKernelAlarmDef EAddAlarm ERemoveAlarm */

/* Group=Notify Triggers */

// Trigger Event Type.  Used to determine what part of EONEVENTSTRUCT is valid.
#define SIMPLE_EVENT    0x0001	// Same as OPC_SIMPLE_EVENT
#define TRACKING_EVENT	0X0002	// Sampe as OPC_TRACKING_EVENT
#define ALARM_EVENT     0x0004	// Same as OPC_CONDITION_EVENT
#define ALARM_DYNAMIC   0x0005  // Dynamic alarm, name is not data source 

// Trigger Categories
#define NO_TRIGGER                      0                   // used for error checking
#define ALARM_LOW_TRIGGER               ALM_SUBCOND_LOW
#define ALARM_HI_TRIGGER                ALM_SUBCOND_HIGH  
#define ALARM_LOLO_TRIGGER              ALM_SUBCOND_LOLO  
#define ALARM_HIHI_TRIGGER              ALM_SUBCOND_HIHI  
#define ALARM_DV_LO_TRIGGER             ALM_SUBCOND_DV_LO 
#define ALARM_DV_HI_TRIGGER             ALM_SUBCOND_DV_HI 
#define ALARM_DIG_LO_TRIGGER            ALM_SUBCOND_DIG_LO
#define ALARM_DIG_HI_TRIGGER            ALM_SUBCOND_DIG_HI
#define EXPRESSION_TRIGGER              0x100
#define TIMER_TRIGGER                   0x101
#define DATA_ITEM_CHANGED_TRIGGER       0x107
#define SYSTEM_STARTED_TRIGGER          0x108
#define SYSTEM_STOPPING_TRIGGER         0x109
// This could conflict with above alarm triggers.  May need to change numbers
// and AND in ALARM_LOW_TRIGGER (etc).
#define ALARM_STARTED_TRIGGER           0x10A       // value in alarm  (active)
#define ALARM_ENDED_TRIGGER             0x10B       // value out of alarm (inactive),
                                                    //  not acknowledge
#define ALARM_ACKNOWLEDGED_TRIGGER      0x10C       // alarm acknowledge, and active
#define ALARM_STATE_CHANGED_TRIGGER     0x10D       // active, with subcondition change
#define ALARM_CLEARED_TRIGGER           0x10E       // not-active, acknowledge
#define ANY_ALARM_TRIGGERED             0x10F       // any of the above alarms.
#define LOG_DATA_TRIGGER                0x110       
#define LOG_ALARM_TRIGGER               0x111       
#define LOG_DATA_TRIGGER_NODATA         0x112       
#define COM_MGR_ON_INTERNET_STOP        0x113       
#define COM_MGR_ON_INTERNET_START       0x114       
#define DIALER_ON_DIALCONNECT           0x115       
#define DIALER_ON_HANGUP                0x116       
#define ENTERPRISEPROXY_ON_LISTEN_PORT  0x117       
#define UPGRADE_ON_DOWNLOAD_COMPLETE    0x118
#define UPGRADE_ON_UPLOAD_COMPLETE      0x119
#define SYSTEM_REGISTERED_EVENT         0x11A
#define APM_OFFLINE_TRIGGER             0x11B
#define BACKUP_LINK_START_TRIGGER       0x11C
#define BACKUP_LINK_STOP_TRIGGER        0x11D
#define MONITOR_STATE_CHANGED_TRIGGER_ID        0x11E
#define MONITOR_COMPLETE_TRIGGER_ID             0x11F
#define MONITOR_REAL_TIME_UPDATE_TRIGGER_ID     0x120

class EUploadFilesList;

/* Group=EONEVENTSTRUCT*/
class _DV_DECLSPEC EONEVENTSTRUCT
{
public:
    // With most constructors the caller owns the memory
    EONEVENTSTRUCT();
    EONEVENTSTRUCT(TCHAR * source, ETimeStamp eventTime, TCHAR *message,
        CDataValue* trigVal = NULL, DWORD eventType = SIMPLE_EVENT,
        DWORD eventCategory = NO_TRIGGER, DWORD severity = 0);
    EONEVENTSTRUCT(EONEVENTSTRUCT* pEvent, bool bDeepCopy = true);
    ~EONEVENTSTRUCT();

    // Serialization
    bool Save(EFileIO*);
    bool SaveVariant(EFileIO*, VARIANT*);
    bool Restore(EFileIO*);
    bool RestoreVariant(EFileIO*, VARIANT*);

protected:
    void Init();
    TCHAR* DeepCopy(TCHAR* pStr);

// properties
public:
    TCHAR*     pszSource;          // The source of event notification.
    ETimeStamp ftTime;             // Time of the event occurrence. 
    TCHAR*     pszMessage;         // Event notification message describing the event. 
    DWORD      dwEventType;        // Identifies events as Simple, Condition-Related, or Tracking. 
    DWORD      dwEventCategory;    // Standard and Vendor-specific event category codes. 
    DWORD      dwSeverity;         // Event severity (from 0 to 1000).  
    CDataValue triggerData;        // Data Value when trigger event happens
    DWORD      dwNumEventAttrs;    // The length of the vendor-specific event attribute array. 
    VARIANT*   pEventAttributes;   // Pointer to an array of vendor-specific event attributes returned for this event notification.
    DWORD      wChangeMask;        // Indicates to the client which properties of the condition have changed, to have caused the server to send the event notification. 
    WORD       wNewState;          // A word bit mask of three bits specifying the new state of the condition.
    TCHAR*     pszConditionName;   // Name of the condition related to this event notification. 
    TCHAR*     pszSubconditionName; // Name of the current sub-condition, for multi-state conditions. 
    WORD       wQuality;           // Quality associated with the condition state. 
    BOOL       bAckRequired;       // Indicates if the related condition requires acknowledgment of this event. 
    ETimeStamp ftActiveTime;       // Time that the condition became active (for single-state conditions), or the time of the transition into the current sub-condition (for multi-state conditions). 
    ULONG_PTR  dwCookie;           // Server defined cookie associated with the event notification. 
    TCHAR*     pszActorID;         // For tracking events, this is the actor ID for the event notification. 
    UINT        fileID;
    UINT        eventID;
    UINT        componentID;
    UINT        messageID;
	bool		m_bNoDataItem;
#ifdef __GATEWAY__
  int          m_gatewayId;        // Gateway only:  need id to pass around. (Alarm only).
#endif

    // Memory management
    bool       m_bDeepCopy;             // delete strings at destructor.

    // Files uploadeds can be initiated as part of the Event.
    // In 3.8 only implemented as part of an Alarm.
    EUploadFilesList* m_pUploadFilesList;
};

/*Group=Notify structs*/

// ENotifyActionInfo structure defines the action module name and schema.
// This is a separate structure because the ENotifyInfo structure can contain 
// 'n' actions to be invoked for a single trigger.
struct ENotifyActionInfo
{
    TCHAR*     pszModName; // The name of the action module which processes the 
                           // triggered event information.
    TCHAR*     pszSchema;  // The name of the action schema to run when the action 
                           // is invoked by a triggered event.
};


// ENotifyInfo structure defines a notify schema.  It associates events and actions.
// When there is 1 event associated to 'n' actions, the actionCnt is set to the number 
// of ENotifyActionInfo structures. 
struct ENotifyInfo
{
    TCHAR*              pszEventModName; // The name of the event module used as a trigger.
    TCHAR*              pszEventSchema;  // The name of the event schema to be subscribed 
                                         // to for event notification.
    ENotifyActionInfo*  pActionInfos;    // An array of action(module and schema) pairs which will 
                                         // be invoked when the event is triggered.
    DWORD               actionCnt;       // Number of ENotifyActionInfo items in pActionInfos.
    DWORD               logic;           // The logic to perform when there are multiple 
                                         // action items.  Logic could be invoke all 
                                         // actions or only invoke one action.  If the 
                                         // first invoked action fails, try the next 
                                         // in the sequence.
};


//**************************************************************************
// Attribute that can be passed as additional parameters to IAction::Invoke()
// This is a general purpose attribute containing an attribute name and an 
// attribute value.
struct EAttribute
{
    EAttribute(TCHAR* pszAttribute, CDataValue& rAttributeValue)
        : m_esName(pszAttribute), m_value(rAttributeValue)
    {
    }
    EString             m_esName;
    CDataValue          m_value;         
};

/*{Group=EAttributes}*/
typedef ETypedPtrList<EPtrList, EAttribute*> EAttributes;

/*Group=Notify interfaces*/

// ITriggerListener interface is implemented by a module that subscribes as a listener 
// for event notification.  dwCount normally is one.  It is not the intention of the 
// design to imply that all event modules should be maintaining a queue of events. 
interface ITriggerListener
{
   virtual void OnEvent(EHANDLE hClient, // [In]the handle set by the event client 
                                         // (listener) at subscribe.
                        DWORD dwCount,   // [In]number of EONEVENTSTRUCTs in pEvents.  
                                         // This is normally 1.
                        EONEVENTSTRUCT *pEvents  // [In]pointer to EONEVENTSTRUCT that 
                                                // describes the event that occurred.
                        ) = 0;

#ifdef __GATEWAY__
   // Used when subscribing to a trigger with a empty schema name.  
   // The Gateway Id is used to filter trigger callbacks.
   virtual KERESULT GetGatewayId(int* pGatewayId) = 0;
#endif

};


// Gateway enumeration:  GatewayType.
//
// The Logic of gateway qualified schema names depends of the type of the 
// trigger / action.
// if GATEWAY_LEVEL - use original schema name.  Trigger /action occurs only at 
//                    a gateway level.  For example OnSytemStart or DoEnterpriseStart
//                    occur regardless of the number of devices or models.
// if MODEL_LEVEL - qualify schema name only once per model number.  The trigger
//                  action occurs per the model of devices NOT the explict device
//                  itself.  For example, OnTimer and DoEmail are configured per
//                  device Model.
// if DEVICE_LEVEL - all trigger/actions names must be qualified for each device.
//                   For example, OnAlarm, OnExpress, DoLogDataToFile occur for
//                   each device (unique Model Number / Serial Number) 
//                   configured in the system.
// 
enum GatewayType
{
    DEVICE_LEVEL,
    MODEL_LEVEL,
    GATEWAY_LEVEL
};


// The ITrigger interface is the basis for all event interfaces.  The interface simply 
// allows for subscribing for notification when an event occurs as well as a method to 
// unsubscribe from event notification.   
interface ITrigger
{
    virtual KERESULT Subscribe(
                    ITriggerListener* pIEventListener, // [In]Pointer to the client's 
                                                       // listener interface.
                    EString& rTriggerName,             // [in] The specific trigger being subscribed to.
                                                       // Required because some ITrigger interfaces support 
                                                       // multiple triggers.
                    EString& rSchemaName,  // [In]A schema name already configured 
                                      // in the event module.  
                    EHANDLE hClient,  // [In]A value from the subscribing client that is sent 
                                      // back without modification upon event notification 
                    HSUBSCRIPTION *phSubscription // [Out]Pointer to the handle to the 
                                                  // subscription that is used when unsubscribing.
                              ) = 0;

    virtual KERESULT Unsubscribe(HSUBSCRIPTION hSubscription // [In]Handle to the subscription
                              ) = 0;

#ifdef __GATEWAY__
    virtual KERESULT GetGatewayType(
        EString& esTrigger,                 // [in] name of trigger 
        GatewayType* pGatewayType) = 0;     // [out] GatewayType           
#endif

};


// IErrorListener interface allows asynchronous callbacks with error (or success) 
// information.  This can be used by the Notification in conjunction with IAction 
// to know if the action is processed sucessfully. 
interface IErrorListener
{
   virtual void OnError(EHANDLE hClient, // [In]the client handle passed with the 
                                         // IErrorListerner pointer to the server.
                        KERESULT ker     // [In]the number indicating the success or 
                                         // error code.
                             ) = 0;
};


// IAction interface is the basis for all action interfaces.  It allows all actions 
// to be invoked in a common manner.  But not all actions will require an  
// EONEVENTSTRUCT to process the action.  One example is the Recipe module.  
// In this case, the dwCount and pEvents parameters can be ignored by the action implementation.
interface IAction
{
    virtual KERESULT Invoke(
                   EString& actionName,                   // [in] The specific action to be run.  Required
                                                          // because some IAction interfaces support multiple actions.
                   EString& schemaName,                   // [In]A schema name already configured in 
                                                          // the action module.
                   EAttributes *pAttributes,              // [in] List of action speicific attributes parameters.
                                                          // This is usually NULL
                   DWORD dwCount,                         // [In]Number of EONEVENTSTRUCTs in pEvents.  
                                                          // This is normally 1.
                   EONEVENTSTRUCT *pEvents,               // [In]ONEVENTSTRUCT that describes the 
                                                          // event that occurred.
                   IErrorListener* pIErrorListener = NULL // [In]an optional pointer to 
                                                          // an IErrorListener.  This 
                                                          // enables the IAction to 
                                                          // notify the Notification 
                                                          // module regarding the 
                                                          // success or failure of the 
                                                          // action to be processed.  
                                                          // The Notification module 
                                                          // can use this information 
                                                          // to do error recovery logic.
                                                          // It is not up to the Notification  
                                                          // module to send this error 
                                                          // information to the event 
                                                          // log, that is the responsibility 
                                                          // of the action module.
                   ) = 0;

#ifdef __GATEWAY__
    virtual KERESULT GetGatewayType(
        EString& esAction,                  // [in] name of action
        GatewayType* pGatewayType) = 0;     // [out] GatewayType           
#endif
};

#define     DO_RUN_SCRIPT           _T("DoRunScript")

interface INotifyMgr : public IKernelBase,
                       public IAction
{
    // registration

    // Register action module with notify module and get an pointer to the interface of 
    // action module
    virtual KERESULT Register(TCHAR* pszModName,   // [In]The name of the action module 
                                                   // which processes the triggered 
                                                   // event information.
                              IAction* pIEventSync // [in]Pointer to the interface of 
                                                   // action module.
                              ) = 0;

    // Register trigger module with notify module and get an pointer to the interface of 
    // trigger module.
    virtual KERESULT Register(TCHAR* pszModName,  // [In]The name of the event module 
                                                  // which used as a trigger.
                              ITrigger* pITrigger // [in]Pointer to the interface of 
                                                  // action module.
                              ) = 0;

    virtual KERESULT        SendConfiguredSchemas(int gatewayID) = 0;

    // 5.2: lookup trigger source by a specific trigger name
    virtual KERESULT GetTriggerInterface(EString& EventModName,      // [In]name of Trigger Module
                                         ITrigger** ppITrigger       // [Out]resolved ptr
                                        ) = 0;
};

#define SYSTEM_START_TRIGGER	_T("OnSystemStart")
#define SYSTEM_STOP_TRIGGER		_T("OnSystemStop")
#define REPORT_EVENT_TRIGGER	_T("OnReportEvent")

#endif




