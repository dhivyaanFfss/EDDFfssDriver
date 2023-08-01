//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  componentid.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Defines the component ID's used in the system
//
//  Classes:
//
//*****************************************************************************

#ifndef __COMPONENT_ID_H__
#define __COMPONENT_ID_H__

/***************
 *
 * !!!!!!!!!!!!
 *
 * These ID's are used by License Generator
 * and MUST remain the same number.
 *
 * Add ONLY to the end
 *
 * Update the file \unity\Utils\LicenseGenerator\components.csv
 * to add component as licensable
 *
 * !!!!!!!!!!!!!!
 *
 ***************/

// VLAD: IMPORTANT!!!!
// If you add or edit something here you MUST
// edit message file to add component name there

// Please use empty slots (those commented out) for future use.

#define NO_COMPONENT_ID                     0
#define ALARM_COMPONENT_ID                  1
#define WEB_SERVICE_COMPONENT_ID            2       /* was:  APPLET_COMPONENT_ID  */
#define DATA_LOGGER_COMPONENT_ID            3
#define DATA_ITEM_MGR_COMPONENT_ID          4
#define DATA_SOURCE_MGR_COMPONENT_ID        5
#define AUTH_PROXY_COMPONENT_ID             6       /* was ENTERPRISE_DATA_LOGGER_COMPONENT_ID */
#define ENTERPRISE_PROXY_COMPONENT_ID       7
#define EMAIL_COMPONENT_ID                  8
#define EXPRESSION_MGR_COMPONENT_ID         9
#define MONITOR_COMPONENT_ID                10      /* was NET_TAG_MGR_COMPONENT_ID */
//#define NETWORK_COMPONENT_ID                11
#define OPC_COMPONENT_ID                    12
#define SECURITY_COMPONENT_ID               13
#define TAG_MGR_COMPONENT_ID                14
// #define UPNP_COMPONENT_ID                   15
#define VERSION_MGMT_COMPONENT_ID           16
#define VPI_COMPONENT_ID                    17
#define WEB_COMPONENT_ID                    18
#define CUSTOM_COMPONENT_ID                 19
#define RECIPE_COMPONENT_ID                 20
#define TIMER_COMPONENT_ID                  21
#define WAAG_SRV_COMPONENT_ID               22
#define DIALER_COMPONENT_ID                 23
#define GATEWAY_COMPONENT_ID                24
#define EDD_COMPONENT_ID                    25
#define KERNEL_COMPONENT_ID                 26
#define SOAP_COMPONENT_ID                   27
#define NOTIFY_COMPONENT_ID                 28
#define LOGFILE_COMPONENT_ID                29
#define SNMP_DRIVER_ID                      30
#define XBASE_COMPONENT_ID                  31
#define SCM_COMPONENT_ID                    32
#define DEPLOY_COMPONENT_ID                 33
#define SCRIPT_MGR_COMPONENT_ID             34
#define RCON_COMPONENT_ID                   35
#define APMPROXY_COMPONENT_ID               36
#define AUTOPROXY_COMPONENT_ID              37
#define FILEWATCH_COMPONENT_ID              38
#define CUSTOM_HANDLER_CLIENT_COMPONENT_ID  39
#define TRAP_HANDLER_COMPONENT_ID           40
#define XMLDA_SRV_COMPONENT_ID              41
#define MOBILE_DRM_COMPONENT_ID             42
#define TRAP_LISTENER_COMPONENT_ID          43
#define PKI_MANAGER_COMPONENT_ID            44
#define SYSLOG_CLIENT_COMPONENT_ID			45
#define RSA_COMPONENT_ID                    46
#define SM_CONTROLLER_COMPONENT_ID          47

// IDs used with Custom components.  This is useful with English.xml.

#endif //__COMPONENT_ID_H__

