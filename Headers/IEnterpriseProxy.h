//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IEnterpriseProxy.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for Communication with Enterprise
//
//  Interface  :  IEnterpriseProxy
//              
//**************************************************************************
#ifndef __IENTERPRISEPROXY__H__
#define __IENTERPRISEPROXY__H__

#include "EDD_Common.h"
#include "IDeployConfig.h"
#include "SoapParserInterface.h"
#include "IPolicy.h"
#include "EWebClient.h"
#include "EPKI.h"

/* Group=Enterprise Proxy */

/* Group=XML Version */

// version of XML xmission
//
#define ENT_XML_VERSION_MAJOR 3
#define ENT_XML_VERSION_MINOR 9

/* Group=Enterprise Proxy define*/

// post timeout, in seconds
//
#define ENT_POST_TIMEOUT    300

/* Group=Actions */ 
// Action names
//
#define ENTERPRISE_START                    _T("DoEnterpriseStart")
#define ENTERPRISE_STOP                     _T("DoEnterpriseStop")

#define ENTERPRISE_ALARM                    _T("DoEnterpriseAlarm")
#define ENTERPRISE_TIMER                    _T("DoEnterpriseTimer")
#define ENTERPRISE_SNAPSHOT                 _T("DoEnterpriseSnapShot")
#define ENTERPRISE_EXPRESSION               _T("DoEnterpriseExpression")
#define ENTERPRISE_WAKEUP                   _T("DoEnterpriseWakeup")

// Schema names for the DoEnterpriseSysEvent (generic) Action

// to tell enterprise proxy of a system
#define ENTERPRISE_STARTUP_NOTIFY           _T("Startup")

// to tell enterprise proxy of a system
#define ENTERPRISE_SHUTDOWN_NOTIFY          _T("Shutdown")

/* Group=Triggers */
// Triggers - stuff we tell Notify
//
#define ENTERPRISE_QUEUE_READY              _T("OnEnterpriseQueueReady")
#define ENTERPRISE_QUEUE_EMPTY              _T("OnEnterpriseQueueEmpty")
#define ENTERPRISE_QUEUE_FULL               _T("OnEnterpriseQueueFull")
#define ENTERPRISE_QUEUE_NONEMPTY           _T("OnEnterpriseQueueNonEmpty")
#define ENTERPRISE_LISTEN_PORT_ACTIVITY     _T("OnListenPortActivity")
#define ENTERPRISE_SYSTEM_REGISTERED        _T("OnSystemRegistered")
#define ENTERPRISE_BACKUP_LINK_START        _T("OnBackupLinkStart")
#define ENTERPRISE_BACKUP_LINK_STOP         _T("OnBackupLinkStop")
#define ENTERPRISE_SERVER_ONLINE            _T("OnEnterpriseServerOnline")
#define ENTERPRISE_SERVER_OFFLINE           _T("OnEnterpriseServerOffline")

//////////////// String to identify a Dataset name
#define DATASET_NAME_IDENTIFIER             _T("DataSetNameID")
#define NULL_IP_ADDRESS							_T("0.0.0.0")

//**************************************************************************
struct RoundTripStats
{
	RoundTripStats() : m_msgCount(0), m_fAvgRTTime(0.0), m_avgSize(0) {;}

	DWORD	m_msgCount;		// message count for this period
	float	m_fAvgRTTime;	// average round-trip time
	DWORD	m_avgSize;		// average message size
};

// Forward Reference
class IWebUserAgent;

/* Group=Enterprise Proxy */

//**************************************************************************
interface IPostResultListener
{
    virtual void OnPostResult(TCHAR *pszDataTag, KERESULT rc) = 0;
};

//**************************************************************************
interface IEnterpriseProxy : public IKernelBase
{
    // TCN changed so that higher priorities are numerically higher than lower priorities:
    typedef enum {
        Low,        // no problem if this is never seen by anyone
        Normal,     // regular message
        Urgent,     // top prioriry
        Immediate,  // requires immediate attention, even initiate a connection
    } QueuePriority;

    typedef enum {
        Init,
        Locked,
        Ready,
        Pending,
        Sent,
        RetryWait,
        Error,
        BadState
    } QueueState;

    typedef enum {
        Empty,
        NonEmpty,
        Full,
        Stopped
    } QueueLevel;

    typedef enum {
        WaitForConnection = 1,
        InitiateConnection,
        AssumeConnection
    } ConnectionStrategy;

    /*
    Puts a:
    <Pi t="2001-05-14T14:12:50-04:00"/> 
    on the queue to be sent to enterprise.
    Model, serial, and owner will be used in an automatically generated wrapper:
    <eMessage id="11" rc="0" v="2.0">
        <De mn="m1" sn="s1" ow="qa">
        <Pi t="2001-05-14T14:12:50-04:00"/> 
        </De>
    </eMessage>

    :*/
    virtual KERESULT PostSnapShot(EONEVENTSTRUCT *pEvents, DWORD dwCount,
        IEnterpriseProxy::QueuePriority priority = IEnterpriseProxy::Normal,
        TCHAR *pServerConfig = NULL, TCHAR *pSourceName = NULL) = 0;

    /*
    Puts a:
    <Ev t="2001-05-14T14:12:50-04:00" nm="name" sv="sevcode" me="message"/>
    <Ev t="2001-05-14T14:12:50-04:00" nm="name" sv="sevcode" me="message"/>
    for each of n pEvents on the queue.
    :*/
    virtual KERESULT PostEvent(int n, EDDEvent* pEvents, 
        EString& model, EString& serial, EString& owner) = 0;

    /*
    Puts a:
    <Em to="reciepientList" fr="senderName" su="subject" sr="serverHost" cn="contentType">
    body
    </Em>
    on the queue to be sent to enterprise.
    Model, serial, owner are used for an automatically generated wrapper as above.
    if all are initialized to "" then a default will be supplied.  Gateway code should
    supply model,serial,owner
    :*/ 
    virtual KERESULT PostEmail(const EString& recipientList,const EString& senderName,
        const EString& subject,const EString& serverHost,const EString& senderDomain,
        const EString& contentType,
        const EString& bodytext, EString& model,EString& serial,EString& owner) = 0;

    virtual KERESULT PostAlarm(EONEVENTSTRUCT *pEvent,
        IEnterpriseProxy::QueuePriority priority = IEnterpriseProxy::Urgent,
        TCHAR *pServerConfig = NULL, TCHAR *pSourceName = NULL) = 0;

    /*
    Puts a:
    <Ad>
        <Al nm="alarm1" sv="sevcode" de="descstr" y="typestr" at="y" ak="n">
            <Di n="tagname1" y="a" q="g" t="2001-05-14T14:12:50-04:00">3.14159</Di>
        </Al>
        <Al nm="alarm2" sv="sevcode" de="descstr" y="typestr" at="y" ak="n">
            <Di n="tagname2" y="a" q="g" t="2001-05-14T14:12:50-04:00">1.57079</Di>
        </Al>
        ...
    </Ad>
    on the queue to be sent to enterprise.
    :*/
    virtual KERESULT PostAlarms(EONEVENTSTRUCT **pEvents, DWORD dwCount,
        IEnterpriseProxy::QueuePriority priority = IEnterpriseProxy::Urgent,
        TCHAR *pServerConfig = NULL, TCHAR *pSourceName = NULL) = 0;

    /*
    Puts a:
    <SOAPStatus t="2001-10-14T14:12:50-05:00" cmdId="12" userId="user" sc="2" xsc="reason phrase"/>
    on the queue to be sent to enterprise.
    :*/
    virtual KERESULT PostSOAPCommandStatus(EString& esCmdId, EString& esUserId,
        KERESULT iStatusCode, EString& esStatusReason,
        TCHAR *pServerConfig = NULL, int iDeviceId = OUT_OF_GATEWAY_IDS) = 0;

#ifdef __GATEWAY__
    /*
    <Md>
        <De mn="JEKProv2" sn="Serial1" ow="com.axeda.drm.jdbc.data-source">
            <Re r="15000" t="2001-05-14T14:12:50-04:00" y="#"/>
            <Cs>com=1,baud=9600,stop=1,parity=even</Cs>
        </De>
        <De mn="JEKProv2" sn="Serial2" ow="com.axeda.drm.jdbc.data-source">
            <Re r="15000" t="2001-05-14T14:12:50-04:00" y="#"/>
            <Cs>com=2,baud=4800,stop=2,parity=odd</Cs>
        </De>
        ...
    </Md>
    :*/
    virtual KERESULT PostGatewayReport(void) = 0;
#endif

    /*
    Puts an arbitrary XML message on the queue to be sent to the enterprise
    Model, serial, owner are used for an automatically generated wrapper as above.
    if all are initialized to "" then a default will be supplied.  Gateway code should
    supply model,serial,owner
    :*/
    virtual KERESULT QueueRequest(const EString& req,EString& model,
        EString& serial,EString& owner, TCHAR* pDataSetID = NULL,
        IEnterpriseProxy::QueuePriority priority = IEnterpriseProxy::Normal) = 0;

    // NOTE: this method is obsoleted and does not do anything
    virtual KERESULT DataStart(EString& result) = 0;

    // NOTE: this method is obsoleted and does not do anything
    virtual KERESULT DataStart(EString &result,ETimeStamp dataTime) = 0;

    // NOTE: this method is obsoleted and does not do anything
    virtual KERESULT DataEnd(EString& result) = 0;

    /*
    Appends the following to result:
    <Di n="tagname" y="a" q="g">
    3.14159
    </Di>
    type can be:
    "a" -- analog
    "d" -- digital
    "s" -- string
    "e" -- error
    quality can be:
    "g" -- good
    "b" -- bad
    "u" -- uncertain
    "e" -- error
    :*/
    virtual KERESULT DataItem(EString& result,const EString& type,
        const EString& name,const EString& value,const EString& qual,const EString& timestamp) = 0;
    virtual KERESULT DataItem(EString &result,EString name,CDataValue *dataValue,
        ETimeStamp *useStamp = NULL) = 0;

    // direct call to wakeup action
    virtual KERESULT Wakeup() = 0;
    // 3.0.2 Patch 2.  Have post use mn/sn/owner as optional gateway parameters.
    virtual KERESULT Post(TCHAR * pData, EString* pesModel = NULL,
        EString* pesSerial = NULL, EString* pesOwner = NULL) = 0;

    // Get Server Info
    virtual KERESULT GetDRMServerInfo(bool bPrimary, DRM_SERVER_INFO*) = 0;
    virtual KERESULT GetProxyServerInfo(PROXY_SERVER_INFO*) = 0;

    // Update IWebUserAgent with the security and proxy info configured
    // in the EnterpriseProxy.
    // DEPRECATED: Use SetSecurityInfoEx() instead.
    virtual KERESULT SetSecurityInfo(/* in/out */ IWebUserAgent* pWebUA) = 0;

    // returns queue level in percents (0 -- 100)
    virtual KERESULT GetQueueLevelPct(int *piPct) = 0;

    // Ping mode
    virtual KERESULT SetPingMode(bool bPingMode) = 0;

    // Ping rate is in milli-seconds
    virtual KERESULT GetPingRate(/* out */ int* pPingRate) = 0;
    virtual KERESULT SetPingRate(int pingRate) = 0;

    // Resolve EWebClient's result (EWebResult) codes
    // NOTE: this method is obsoleted and does not do anything
    // Use IWebUserAgent::GetResultString() instead
    virtual KERESULT GetWebResultMessageId(EWebResult iResult, UINT *pMsgId) = 0;

    // Resolve EWebClient's warning (EWebWarning) codes
    // NOTE: this method is obsoleted and does not do anything
    // Use IWebUserAgent::GetWarningString() instead
    virtual KERESULT GetWebWarningMessageId(EWebWarning iWarning, UINT *pMsgId) = 0;

    // Perform proxy "auto-detection" through the corresponding component
    virtual KERESULT AutoDetectProxy(EWebRequest *pRequest, EWebProxyLocation *pProxy) = 0;

    // returns DRM server status (optionally accepts server configuration name)
    virtual KERESULT GetServerStatus(bool *pbOnline, TCHAR *pServerName = NULL) = 0;

    // Enables communication with DRM server
    virtual KERESULT Enable(void) = 0;

    // Disables communication with DRM server
    virtual KERESULT Disable(void) = 0;

    // returns HTTP configuration (used by Deployment Component)
    virtual KERESULT GetHTTPConfig(HTTP_CONFIG *pConfig) = 0;

    // used to find out whether communication with the server is possible
    virtual KERESULT IsOffline(bool *pbOffline) = 0;

    // exports configuration data (registrations + Md) to a file
    virtual KERESULT ExportConfiguration(TCHAR *pFileName) = 0;

    // exports data queue content to a file
    virtual KERESULT ExportDataQueue(int iDeviceId, TCHAR *pFileName) = 0;

    // returns diagnosis for DRM server (optionally accepts server configuration name)
    virtual KERESULT GetServerDiagnosis(EString& esDiagnosis, TCHAR *pServerName = NULL) = 0;

    // Subscribes listener to post result notifications
    virtual KERESULT SubscribeToPostResult(IPostResultListener *pListener) = 0;

    // Unsubscribes listener from post result notifications
    virtual KERESULT UnsubscribeFromPostResult(IPostResultListener *pListener) = 0;

    // Retrieve the absolutte path part of the server URL
    virtual KERESULT GetDRMServerAbsPath(bool bPrimary, EString& esAbsPath) = 0;

    // Returns DRM server status and indicates whether the primary server is in use
    virtual KERESULT GetServerStatusEx(bool *pbOnline, bool *pbPrimary) = 0;

    // Sends device (online/offline) status
    virtual KERESULT PostDeviceState(EString& model, EString& serial, bool bOnline) = 0;

    // Hot fix 479
    virtual KERESULT GetCommStatus(DWORD& successCount, DWORD& retryCount) = 0;

	// patch 479
	virtual KERESULT GetStatistics(RoundTripStats& oneMinuteStats, RoundTripStats& fiveMinuteStats, 
		RoundTripStats& fifteenMinuteStats) = 0;

    // Same as SetSecurityInfo(), but SSL initialization is optional
    virtual KERESULT SetSecurityInfoEx(/* in/out */ IWebUserAgent* pWebUA, bool bInitSSL) = 0;

#ifdef __GATEWAY__
    // Sends device (online/offline) status for all managed devices
    virtual KERESULT PostManagedDeviceState(void) = 0;
#endif

    // Sets custom ping rate, which is to be used until either indicated number
    // of contacts with Enterprise has occurred, or custom ping rate is cancelled.
    // Custom ping rate may be extended for another number of contacts, when
    // Enterprise sends back a message during contact.
    virtual KERESULT SetCustomPingRate(int iPingRate, int iContactCount, bool bExtendable) = 0;

    // Cancels custom ping rate.
    virtual KERESULT CancelCustomPingRate(void) = 0;

    // Returns Enterprise PKI profile.
    virtual KERESULT GetPKIProfile(EPKI::Profile*& pPKIProfile) = 0;

    // Returns Binding Settings
    virtual KERESULT GetBindingSettings(BINDING_SETTINGS *pBindingSettings) = 0;
};



#endif //__IENTERPRISEPROXY__H__
