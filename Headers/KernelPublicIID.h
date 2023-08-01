//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  KernelPublicIID.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Kernel public interfaces ID's
//
//  Classes:
//
//**************************************************************************

#ifndef __KERNEL_PUBLIC_IID_H__
#define __KERNEL_PUBLIC_IID_H__

// group=Interface IDs
// See Also:  IKernelBase::GetInterface
typedef  EULONG32                KERNEL_IID; // 32-bit ID for Axeda Connector interfaces

#define  IID_KERNEL_RESERVED     4096       // Base ID number for interfaces
#define  IID_KERNEL_CUSTOM       0x4000     // Base ID for custom interfaces.

// Public interface ID's
                                      
// ID used with IKernelBase::GetInterface to obtain the interface:  IKernelService
#define  IID_KERNEL_SERVICE        IID_KERNEL_RESERVED + 1 
// ID used with IKernelBase::GetInterface to obtain the interface:  IDynamicData
#define  IID_DYNAMIC_DATA          IID_KERNEL_RESERVED + 2  
// ID used with IKernelBase::GetInterface to obtain the interface:  ITagManager
#define  IID_TAG_MGR               IID_KERNEL_RESERVED + 3  
// ID used with IKernelBase::GetInterface to obtain the interface:  IAlarmMgr
#define  IID_ALARM_MGR             IID_KERNEL_RESERVED + 4  
// ID used with IKernelBase::GetInterface to obtain the interface:  IAlarmFilter
#define  IID_ALARM_FILTER          IID_KERNEL_RESERVED + 5  
// ID used with IKernelBase::GetInterface to obtain the interface:  IDynAlarm
#define  IID_DYNAMIC_ALARM         IID_KERNEL_RESERVED + 6  
// ID used with IKernelBase::GetInterface to obtain the interface:  IDataSourceMgr
#define  IID_DATASOURCE_MGR        IID_KERNEL_RESERVED + 7  

// ID used with IKernelBase::GetInterface to obtain the interface:  IWizletServer
#define  IID_WIZLET_SERVER         IID_KERNEL_RESERVED + 9  
// ID used with IKernelBase::GetInterface to obtain the interface:  IKernelLog
#define  IID_KERNEL_LOG             IID_KERNEL_RESERVED + 10 
// ID used with IKernelBase::GetInterface to obtain the interface:  IDispatcher   
#define  IID_DISPATCHER            IID_KERNEL_RESERVED + 11 
// ID used with IKernelBase::GetInterface to obtain the interface:  IXdrMessageHandler
#define  IID_XDR_MESSAGE_HANDLER   IID_KERNEL_RESERVED + 12 
// ID used with IKernelBase::GetInterface to obtain the interface:  INetworkServiceProvider
#define  IID_NETWORK_SERVICE       IID_KERNEL_RESERVED + 13 
// ID used with IKernelBase::GetInterface to obtain the interface:  INetEventsDispatcher
#define  IID_NETEVENTS_DISPATCHER  IID_KERNEL_RESERVED + 14 
// ID used with IKernelBase::GetInterface to obtain the interface:  IDataItemProvider
#define  IID_DATAITEM_PROVIDER     IID_KERNEL_RESERVED + 15 
// ID used with IKernelBase::GetInterface to obtain the interface:  IEnumManager
#define  IID_ENUM_MANAGER          IID_KERNEL_RESERVED + 16 
// ID used with IKernelBase::GetInterface to obtain the interface:  IEnumerator
#define  IID_ENUMERATOR            IID_KERNEL_RESERVED + 17 
// ID used with IKernelBase::GetInterface to obtain the interface:  IOPCDataSrc
#define  IID_OPC_DATASRC           IID_KERNEL_RESERVED + 18 

// ID used with IKernelBase::GetInterface to obtain the interface:  IdynAlarmProcessor
#define  IID_DYNALARM_PROCESSOR		IID_KERNEL_RESERVED + 19

// ID used with IKernelBase::GetInterface to obtain the interface:  IBrowse 
// This is only accessable through a IEnumString object.
#define  IID_BROWSER               IID_KERNEL_RESERVED + 20

// ID used with IKernelBase::GetInterface to obtain the interface:  IEmailProvider
#define  IID_EMAIL_PROVIDER        IID_KERNEL_RESERVED + 21 
// ID used with IKernelBase::GetInterface to obtain the interface:  IWebProvider
#define  IID_WEB_SERVER          IID_KERNEL_RESERVED + 22 
// ID used with IKernelBase::GetInterface to obtain the interface:  ITextParser
#define  IID_TEXT_PARSER           IID_KERNEL_RESERVED + 23 
// ID used with IKernelBase::GetInterface to obtain the interface:  ITextParser
#define  IID_UPNP_DEVICE           IID_KERNEL_RESERVED + 24 

// ID used with IKernelBase::GetInterface to obtain the interface:  INotifyMgr
#define  IID_NOTIFY_MGR            IID_KERNEL_RESERVED + 25
// ID used with IKernelBase::GetInterface to obtain the interface:  ITrigger
#define  IID_TRIGGER               IID_KERNEL_RESERVED + 26
// ID used with IKernelBase::GetInterface to obtain the interface:  IAction
#define  IID_ACTION                IID_KERNEL_RESERVED + 27

// ID used with IKernelBase::GetInterface to obtain the interface:  ISecurity
#define  IID_SECURITY              IID_KERNEL_RESERVED + 29

// ID used with IKernelBase::GetInterface to obtain the interface:  IExpression
#define  IID_EXPRESSION            IID_KERNEL_RESERVED + 30

// ID used with IKernelBase::GetInterface to obtain the interface:  ILicense
#define  IID_LICENSE               IID_KERNEL_RESERVED + 31

// ID used with IKernelBase::GetInterface to obtain the interface:  IWebProvider
#define  IID_WEB_CLIENT            IID_KERNEL_RESERVED + 32 

// ID used with IKernelBase::GetInterface to obtain the interface:  IEnterpriseProxy
#define  IID_ENTERPRISE_PROXY      IID_KERNEL_RESERVED + 33 

// ID used with IKernelBase::GetInterface to obtain the interface:  ISoapDispatcher
#define  IID_SOAP_DISPATCH         IID_KERNEL_RESERVED + 34 

// ID used with IKernelBase::GetInterface to obtain the interface:  ISoapParser
#define  IID_SOAP_PARSER           IID_KERNEL_RESERVED + 35 

// ID used with IKernelBase::GetInterface to obtain the interface:  IRecipe
#define  IID_RECIPE_MGR            IID_KERNEL_RESERVED + 36

// ID used with IKernelBase::GetInterface to obtain the interface:  ILogger
#define  IID_LOGGER                IID_KERNEL_RESERVED + 37

// ID used with IKernelBase::GetInterface to obtain the interface:  ITimerMgr
#define  IID_TIMER                 IID_KERNEL_RESERVED + 38

// ID used with IKernelBase::GetInterface to obtain the interface:  IWaagSrvMgr
#define  IID_WAAG_SRV              IID_KERNEL_RESERVED + 39

// ID used with IKernelBase::GetInterface to obtain the interface:  IGateway
#define  IID_GATEWAY               IID_KERNEL_RESERVED + 40

// ID used with IKernelBase::GetInterface to obtain the interface:  IComMgr
#define  IID_COMMGR                IID_KERNEL_RESERVED + 41

// ID used with IKernelBase::GetInterface to obtain the interface:  IRestart
#define  IID_RESTART               IID_KERNEL_RESERVED + 42

// ID used with IKernelBase::GetInterface to obtain the interface:  IDataBlock
#define  IID_DATA_BLOCK            IID_KERNEL_RESERVED + 43

// ID used with IKernelBase::GetInterface to obtain the interface:  ISetTime
#define  IID_SET_TIME              IID_KERNEL_RESERVED + 44

// ID used with IKernelBase::GetInterface to obtain the interface:  ILogFile
#define  IID_LOG_FILE              IID_KERNEL_RESERVED + 45

// ID used with IKernelBase::GetInterface to obtain the interface:  IMsgManager
#define  IID_STR_MGR               IID_KERNEL_RESERVED + 46

// ID used with IKernelBase::GetInterface to obtain the interface:  IUpgrade
#define  IID_UPGRADE               IID_KERNEL_RESERVED + 47
// ID used with IKernelBase::GetInterface to obtain the interface:  IRunTime
#define  IID_RUN_TIME              IID_KERNEL_RESERVED + 48

// ID used with IKernelBase::GetInterface to obtain the interface:  ISNMPTrapManager
#define  IID_SNMP_TRAP_MGR         IID_KERNEL_RESERVED + 49

// ID used with IKernelBase::GetInterface to obtain the interface:  ICounterMgr
#define  IID_COUNTER_MGR           IID_KERNEL_RESERVED + 50

// ID used with IKernelBase::GetInterface to obtain the interface:  IGetItemType
// Should only be used at object level, not per component.
#define  IID_GET_ITEM_TYPE         IID_KERNEL_RESERVED + 51

// ID used with IKernelBase::GetInterface to obtain the interface:  IDataItem
// Should only be used at object level, not per component.
#define  IID_DATA_ITEM             IID_KERNEL_RESERVED + 52

// ID used with IKernelBase::GetInterface to obtain the interface:  ISM  (Software Management)
#define  IID_SM                     IID_KERNEL_RESERVED + 53

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeployConfigInterface
#define  IID_DEPLOY_CONFIG          IID_KERNEL_RESERVED + 54

// ID used with IKernelBase::GetInterface to obtain the interface:  IKernelShell
#define  IID_KERNEL_SHELL           IID_KERNEL_RESERVED + 55

// ID used with IKernelBase::GetInterface to obtain the interface:  IScriptManager
#define  IID_SCRIPT_MANAGER         IID_KERNEL_RESERVED + 56

// ID used with IKernelBase::GetInterface to obtain the interface:  IEDDDataSource
#define  IID_EDD_DATASOURCE         IID_KERNEL_RESERVED + 57

// ID used with IKernelBase::GetInterface to obtain the interface:  IPolicyMgr
#define  IID_POLICY_MGR             IID_KERNEL_RESERVED + 58

// ID used with IKernelBase::GetInterface to obtain the interface:  IEDDDeviceInfo
#define  IID_EDD_DEVICE_INFO        IID_KERNEL_RESERVED + 59

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeviceInfoProvider
#define  IID_DEVICE_INFO_PROVIDER   IID_KERNEL_RESERVED + 60

// ID used with IKernelBase::GetInterface to obtain the interface:  IAutoProxyInterface
#define  IID_AUTO_PROXY             IID_KERNEL_RESERVED + 61

// ID used with IKernelBase::GetInterface to obtain the interface:  ICustomClientMgr
#define  IID_CUSTOM_CLIENT_MGR      IID_KERNEL_RESERVED + 62

// ID used with IKernelBase::GetInterface to obtain the interface:  ILogMessage
#define  IID_LOG_MESSAGE            IID_KERNEL_RESERVED + 63

// ID used with IKernelBase::GetInterface to obtain the interface:  ISNMPDiscoveryConfig
#define  IID_SNMP_DISCOVERY_CONFIG  IID_KERNEL_RESERVED + 64

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeviceStatus
// Can NOT be used globally.  Use on IEDDManager interface.
#define  IID_DEVICE_STATUS          IID_KERNEL_RESERVED + 65

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeployUtilProxy
#define  IID_DEPLOY_UTIL_PROXY      IID_KERNEL_RESERVED + 66

// ID used with IKernelBase::GetInterface to obtain the interface:  IPACManager
#define  IID_PAC_MANAGER            IID_KERNEL_RESERVED + 67

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeviceUUIDMgr
#define  IID_DEVICE_UUID_MGR        IID_KERNEL_RESERVED + 68

// ID used with IKernelBase::GetInterface to obtain the interface:  IMobileDRMMgr
#define  IID_MOBILE_DRM_MGR         IID_KERNEL_RESERVED + 69

// ID used with IKernelBase::GetInterface to obtain the interface:  ISNMPTrapManager
#define  IID_SNMP_TRAP_MGR2         IID_KERNEL_RESERVED + 70

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeviceState
#define  IDD_DEVICE_STATE           IID_KERNEL_RESERVED + 71

// remove once component incorporated into standard product.
#define  IID_STATE_MONITORING       IID_KERNEL_RESERVED + 72

// ID used with IKernelBase::GetInterface to obtain the interface:  IRConInterface
#define  IID_RCON                   IID_KERNEL_RESERVED + 73

// ID used with IKernelBase::GetInterface to obtain the interface:  IAuthProxy
#define  IID_AUTH_PROXY             IID_KERNEL_RESERVED + 74

// ID used with IKernelBase::GetInterface to obtain the interface:  ISoapRedirector
#define  IID_SOAP_REDIRECTOR        IID_KERNEL_RESERVED + 75

// ID used with IKernelBase::GetInterface to obtain the interface:  IPKIManager
#define  IID_PKI_MANAGER            IID_KERNEL_RESERVED + 76

// ID used with IKernelBase::GetInterface to obtain the interface:  ISyslogClient
#define  IID_SYSLOG_CLIENT          IID_KERNEL_RESERVED + 77

// ID used with IKernelBase::GetInterface to obtain the interface:  ISecureStoreMgr
#define  IID_SECURE_STORE_MGR       IID_KERNEL_RESERVED + 78

// ID used with IKernelBase::GetInterface to obtain the interface:  ISMController
#define  IID_SM_CONTROLLER          IID_KERNEL_RESERVED + 79

// ID used with IKernelBase::GetInterface to obtain the interface:  IDeployConfigManager
#define  IID_DEPLOY_CONFIG_MANAGER  IID_KERNEL_RESERVED + 80

// ID used with IKernelBase::GetInterface to obtain the interface:  IDPProvider
#define  IID_DP_PROVIDER            IID_KERNEL_RESERVED + 81

// ID used with IKernelBase::GetInterface to obtain the interface:  IDPManager
#define  IID_DP_MANAGER             IID_KERNEL_RESERVED + 82

// ID used with IKernelBase::GetInterface to obtain the interface:  IFileWatchMgr
#define  IID_FILE_WATCH_MGR         IID_KERNEL_RESERVED + 83

#endif //__KERNEL_PUBLIC_IID_H__
