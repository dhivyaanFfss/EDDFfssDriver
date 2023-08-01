//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  IPolicy.h 
//  
//  Subsystem  :  Kernel
//
//  Description:  Policy Manager interface and support definitions.
//                
//                  NOTE:  APM stands for "Axeda Policy Manager" which is
//                  the Server that defines, manages and publishes the security
//                  policy.
//               
//               An Axeda Policy contains the following:
//               - A list of actions.  An "action" is equivelent to a SOAP command
//                 or message post to the Enterprise Proxy.
//               Each Action has a permission or set of permissions.  This includes
//               the permission parameters and a permission right.  A right is set to
//               always, never, or ask.
//
//  Classes:       
//              IPermissionHandler
//              EPermissionHandlerMap
//              EMsgPermissionHandlerMap
//              ENewDRMAction
//              IPolicyMgr
//
//*****************************************************************************

#ifndef __IPOLICY_H__
#define __IPOLICY_H__

/* Group=PolicyMgr */


//*****************************************************************************
// Agent Defined Users and Command IDs.  Used by Agent internal SOAP commands.
#define AXEDA_AGENT_USER    _T("Axeda Agent") 
#define AXEDA_AGENT_CMDID   _T("-1")

#define MONITOR_ACTION      _T("Monitor.")
#define MONITOR_ALARM       _T("Monitor.Alarm")
#define MONITOR_EVENT       _T("Monitor.Event")
#define MONITOR_DATA        _T("Monitor.Data")
#define MONITOR_EMAIL       _T("Monitor.Email")


//*****************************************************************************
// Policy Manager trigger name.
#define APM_OFFLINE_TRIGGER_NAME _T("OnAPMOffline")

// APM - Gateway flag for when a APM policy is defined.
#define APM_POLICY_DEFINED  _T("APMPolicyDefined")


//*****************************************************************************
// The Security policy action has 3 levels of permissions: always, ask, never
enum Permission
{
    // listed in order of priority w/ pmNever being highest priority.
    // Note:  Automatic is not a APM permission.  It is used internally to 
    // indicate a policy action for which the APM does not need to define a permission;
    // for example, EEnterpriseProxy.Response.
    pmAUTOMATIC,    //  No permission check is needed - NOT A POLICY PERMISSION

    pmALWAYS,       //  the action function is allowed
    pmASK,          //  the action function requires authorization
    pmNEVER,        //  the action function is NOT allowed
    pmASK_WINDOW,  //   special action permission: changed from Ask to AskWindow 
                    //  which means an "always" permission for the "window" period.

    pmUNDEFINED,    // the action function is NOT defined  - NOT A POLICY PERMISSION
    pmOPEN_PACKAGE, // special permission for Software Managagement's Package.Execute action
    pmUNKNOWN       // the action function is unknown, this is NOT A POLICY PERMISSION.
                    // unknonwn is used when processing an accepted ask and by packages processed by S.M.

};


//*****************************************************************************
// The scope of a new action defines the level at which the APM server sets the
// new action.
enum APMActionScope
{
    asGlobal,
    asModel,
    asDevice
};

//*****************************************************************************
// The parameter type.  Used by the APM Server UI to display parameter value as
// a string or a boolean (True/False).
enum APMParameterType
{
    atString,
    atBoolean
};

//*****************************************************************************
// See discription of parameters below.
class EActionParameter
{
public:
    EActionParameter(TCHAR* pszParameterName, bool bIsPermissionRequest = false, 
        APMParameterType type = atString)
        : m_esName(pszParameterName), m_bIsPermissionRequest(bIsPermissionRequest),
        m_type(type) { } 

    EString             m_esName;           // parameter name
    bool                m_bIsPermissionRequest;     // set true if only required when asking permission
    APMParameterType    m_type;             
};

// Define the EActionParameter collection.
typedef ETypedPtrList<EPtrList, EActionParameter*> EActionParameters;


// *****************************************************************************
// This structure is used when a component adds a new SOAP command or message to 
// the system and it needs to be registered with the APM.  
// (This is for future extensibility).
// An action definition could be a SOAP command or eMessage.  
// If eMessage, a pseudo esCmd is still required.
//
// The following is a detailed explaination of the parameters:
// ·	name: the initial name of the Action to be used by APM.  This can be edited by the 
//     APM user.  
// ·	category: the initial APM category under which to display the action name.
// ·	cmd: the Agent's internal SOAP command, usually in the format of component.function.
// ·	description:  A user friendly description of the action.
// ·	scope: the scope of action.  Values include:  global, model, device.  A gateway 
//      only SOAP command would have a "global" scope.
// ·	isEditable:  boolean flag used only when the parameter's isPermissionRequest flag
//                   if false.  True:  the parameter displayed by APM Server on the policy
//                   screen can be edited.  False:  The parameter is displayed with a
//                   default of "*", but it can not be edited by the user.
// ·	Parameter - n:  name of the parameter.  The name should not have spaces in it.
// ·	Parameter - isPermissionRequest: Set true if the APM is to display the parameter 
//         for user input when the policy permission is being defined.  
//         Set false if the parameter is used only when requesting permission from the APM 
//         for the action.  Optional parameter, the default is false.
// ·	Parameter - type:  Values include:  string, number, bool.  
//         Optional parameter, the default is string. This is normally only used when set 
//         to "bool".  This allows the APM UI to translate a 1/0 value to true/false.
// 

class ENewDRMAction
{
public:
    ENewDRMAction()
        : m_scope(asGlobal), m_bIsEditable(false){}

    EString             m_esName;
    EString             m_esCategory;
    EString             m_esCmd;          // syntax component.function 
    EString             m_esDescription;
    APMActionScope      m_scope;
    bool                m_bIsEditable;
    EActionParameters   m_parameters;     // caller is responsible for memory of collection.
};

  
//*****************************************************************************
// The Policy Manager defines a set of status codes to report to the APM Enterprise
//  These codes are sent for Almost every SOAP command
enum APMSoapStatus
{
    apmssSuccess = 0,
    apmssError,
    apmssDenied,
    apmssUndefined,

    apmssExpressionInvalid = 1000,
    apmssExpressionUnknownSymbol = 1001,
    apmssExpressionFunctionError = 1002,
    apmssFilterEvaluated = 1003,
};

//*****************************************************************************
// Used to notify listener that a new policy has been received by the APM Proxy.
// Note:  this may be called multiple times per deviceId.
interface IPolicyListener
{
    virtual KERESULT OnNewPolicy(int deviceId = CONNECTOR_GATEWAY) = 0;     
};

//*****************************************************************************
// The Policy Manager functions available to all components.
class APM_SERVER_INFO;
interface IPolicyMgr : public IKernelBase
{
    // Used for extensibility.  A new message/SOAP needs to be registered with the APM
    // The gatewayId is only used if the action scope is device or model.
    //
    // Note:  do not add a new action till after the device has been registered. 
    // (i.e. add in Start());
    virtual KERESULT RegisterAction(ENewDRMAction& rAction, int gatewayId = CONNECTOR_GATEWAY) = 0;

    // Used by SOAP manager per SOAP command to check permission before executing SOAP.
    virtual KERESULT CheckRules(ISoapParser* pIParser, soapMethod* pMethod, 
        /* out */ Permission* pPermission) = 0;

    // Used to Check that Agent Functions such as DataItems - are permissable
    virtual KERESULT CheckRules(int iGatewayID, const EString& esFunction, /* out */Permission*) = 0;

    // Used by SOAP manager per SOAP command to manages Audit logs and request permission
    // to execute a SOAP action that has its permissions set to "ask".
    virtual KERESULT ProcessPermission(ISoapParser* pIParser, soapMethod* pMethod, 
        Permission permission, KERESULT soapResult) = 0;

    // Hook to allow other components to post a message to the APM Server.
    // Message is queued and not directly posted.
    // The message is wrapped with the APM header information.
    virtual KERESULT PostMessage(EString& rMsg, int gatewayId = CONNECTOR_GATEWAY) = 0;

    // Hook to be able to register SOAP functions as not needing permission checking
    //  this might not be used if it is determined to allow a custom component to
    //  bypass the IPolicyMgr.
    virtual KERESULT AddAutomaticFunction(LPCTSTR pszFunction) = 0;

    // Allow other components to monitor when a new policy is received.
    virtual KERESULT    Subscribe(IPolicyListener* pIListener) = 0;
    virtual KERESULT    Unsubscribe(IPolicyListener* pIListener) = 0;

    // Get the component's APM Server settings
    virtual KERESULT GetAPMSettings(APM_SERVER_INFO*) = 0;

    // Post an audit message
    virtual KERESULT PostAudit(int gatewayId, APMSoapStatus status, KERESULT result, 
        const EString& esReason, const EString& esAction) = 0;
};

#endif
