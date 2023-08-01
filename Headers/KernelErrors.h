//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  KernelErrors.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define kernel errors
//
//  Classes:
//
//************************************************************************** 

#ifndef __KERNEL_ERRORS_H__
#define __KERNEL_ERRORS_H__

/* {secret} */
/*
*      Values are 32-bit values laid out as follows:
*    
*       3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
*       1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
*      +---+-+-+-----------------------+-------------------------------+
*      |Sev|C|R|     Facility          |               Code            |
*      +---+-+-+-----------------------+-------------------------------+
*    
*      where
*    
*          Sev - is the severity code, as follows:
*    
*              00 - Success
*              01 - Informational
*              10 - Warning
*              11 - Error
*    
*          C - is the Customer code flag, and is always 0.
*    
*          R - is a reserved bit and is always 0.
*    
*          Facility - identifies the facility.
*    
*          Code - is the facility's status code.
*    
*
*  For example: If a tag data item could not be opened, the error would be:
*  KEC_TAG + KE_OPEN.
*
*   ****************************************************************************
*   !!! The code 0xC0000000 is reserved for use by the DRM Enterprise Server !!!
*   ****************************************************************************
*/
/* end secret */

/* Group=General*/

typedef  EULONG32  KERESULT;  // A 32-bit value used to describe an error or warning.

/* group=Error Macros*/

#ifndef SUCCEEDED

/* Generic test for success on any status value. Non-negative numbers
   indicate success.
*/
#define SUCCEEDED(Status) ((ELONG32)(Status) >= 0)

/* Inverse of SUCCEEDED. */
#define FAILED(Status) ((ELONG32)(Status) < 0)

/* Generic test for an error on any status value. */
#define IS_ERROR(Status) ((EULONG32)(Status) >> 31 == SEVERITY_ERROR)

#endif

#define  KEC_MASK                (0xC000ffffL) // mask to get error only w/o component
#define  KEC_DATA_MASK           (0xC000ff00L) // mask to get error base w/o component

/* group=Component Error IDs */

#define  KEC_COMMON              (0x01000000L)  // Base library function error.
#define  KEC_KERNEL              (0x01010000L)  // Kernel error.
#define  KEC_DATASOURCE          (0x01020000L)  // Data Source component error.
#define  KEC_DATAITEMMGR         (0x01030000L)  // Data Item Manager component error.
#define  KEC_TAG                 (0x01040000L)  // Tag component error.
#define  KEC_NETTAG              (0x01050000L)  // Net Tag component error.
#define  KEC_ALARM               (0x01060000L)  // Alarm component error.
#define  KEC_APPLETSVR           (0x01070000L)  // Applet server component error.
#define  KEC_NETWORK             (0x01080000L)  // Network component error.
#define  KEC_API                 (0x01090000L)  // API error.
#define  KEC_EMAIL               (0x010A0000L)  // E-mail component error.
#define  KEC_NOTIFY              (0x010B0000L)  // Notifier component error.
#define  KEC_SECURITY            (0x010C0000L)  // Security component error.
#define  KEC_EXPRESSION          (0x010D0000L)  // Expression component error.
#define  KEC_SOAP                (0x010E0000L)  // Soap component error
#define  KEC_RECIPE              (0x010F0000L)  // Recipe component error
#define  KEC_LOGGER              (0x01100000L)  // Logger component error
#define  KEC_TIMER               (0x01110000L)  // Timer component error
#define  KEC_WAAG_SRV            (0x01120000L)  // WAAG Server component error
#define  KEC_WEB                 (0x01130000L)  // Internet component error
#define  KEC_GATEWAY             (0x01140000L)  // Gateway component error
#define  KEC_LOGFILE             (0x01150000L)  // LogFile component error
#define  KEC_SNMP                (0x01160000L)  // SNMP EDD driver error
#define  KEC_SM                  (0x01170000L)  // Software Management error
#define  KEC_PM                  (0x01180000L)  // Policy Manager errors
#define  KEC_CRYPTO              (0x01190000L)  // Crypto-related errors
#define  KEC_FILEWATCH           (0x011A0000L)  // FileWatch errors
#define  KEC_CUSTOM_CLIENT       (0x011B0000L)  // Custom Client errors
#define  KEC_MOBILE_DRM          (0x011C0000L)  // Mobile DRM errors
#define  KEC_AUTH_PROXY          (0x011D0000L)  // AuthProxy errors
#define  KEC_PKI_MANAGER         (0x011E0000L)  // PKIManager errors
#define  KEC_SYSLOG_CLIENT       (0x011F0000L)  // SyslogClient errors
#define  KEC_DP                  (0x01200000L)  // Data Protection errors

// Reserved component error ID for all SDK custom components.  
#define  KEC_CUSTOM              (0X04000000L)  

/* Errors list  */
/* group=Error IDs*/

// Reserved error base for all custom components.
// A custom component can use any of the standard error codes with its
// component. If it has a custom-specific error, the error number should
// start from the custom error base.
#define  KE_CUSTOM_ERROR_BASE    (0xC000C000L)  

#define  KE_OK                   0              // No error.  
#define  KE_FALSE                1              // No error, but didn't operate as intended.
#define  KE_INHIBITED            2              // No error, a DI write or file upload was inhibited
                                                // by the SDK Custom Handler.
#define  KE_CONTINUE             3              // No error, keep doing
                                                

// Status Codes.  Used in response to a DRM Enterprise SOAP command.  These are not errors.
//                They indicate the status of the SOAP command, especially in regard to the
//                Agent to APM Server interactions.
//                Note:  the 0x4... code is the same as a severity code of "01" in bits 31 and 30 
//                       as defined above.
#define  APM_STATUS_DENIED              (0x40000001L)   // APM policy denied the SOAP command
#define  APM_STATUS_DENIEDTIMEOUT       (0x40000002L)   // APM Server did not respond to an "ASK", request denied after timeout.
#define  APM_STATUS_UNSUPPORTED         (0x40000003L)   // No APM policy for this command.  - therefore denied.
#define  APM_STATUS_ASKING              (0x40000004L)   // Agent is currently ASKING permission to execute the SOAP command
#define  APM_STATUS_ASKDENIED           (0x40000005L)   // APM Server responded to an ASK with a "denied"
#define  APM_STATUS_ASKACCEPTED         (0x40000006L)   // APM Server responded to an ASK with an "accepted" - Agent is currently processing the SOAP command
#define  APM_STATUS_DENIEDAPMOFFLINE    (0x40000007L)   // Agent denied the a command with an APM policy of "ASK" because the APM is currently off-line.

#define  KE_AUTH_IN_PROGRESS            (0x40000101L)   // Authentication (through AuthProxy) in progress

// Error codes
//
//    !!! The code 0xC0000000 is reserved for use by the DRM Enterprise Server !!!
//
#define  KE_INTERNAL             (0xC0000001L) // Internal error. Probably the result of an unhandled situation or bug! 
#define  KE_FAILED               (0xC0000002L) // General failure; no specific information.
#define  KE_INVALID_PARAMS       (0xC0000003L) // Invalid parameters.
#define  KE_MEMORY               (0xC0000004L) // Low memory; memory allocation failed.
#define  KE_FILEOPEN             (0xC0000005L) // Open file failed. This situation may occur when loading or saving the application.
#define  KE_FILEREAD             (0xC0000006L) // File read error (not used).
#define  KE_FILEWRITE            (0xC0000007L) // File write error.
#define  KE_BADFILEFORMAT        (0xC0000008L) // Bad file format or file corrupted. This situation may occur when serialization fails.
#define  KE_NOT_IMPLEMENTED      (0xC0000009L) // (Not implemented in the current version.)
#define  KE_TIMEOUT              (0xC000000AL) // The operation timed out.
#define  KE_ADD_OBJECT           (0xC000000BL) // Failed to add a Kernel object.
#define  KE_REMOVE_OBJECT        (0xC000000CL) // Failed to remove a Kernel object.
#define  KE_ACCESS_OBJECT        (0xC000000DL) // The object is in use.
#define  KE_ACCESS_RIGHTS        (0xC000000EL) // No access rights. This is a security error.
#define  KE_READ_OBJECT          (0xC000000FL) // Read object failed.
#define  KE_WRITE_OBJECT         (0xC0000010L) // Write object failed. 
#define  KE_CONNECT              (0xC0000011L) // Connection to data source, network. 
#define  KE_DISCONNECT           (0xC0000012L) // Disconnect from data source, network.
#define  KE_OPEN                 (0xC0000013L) // Open handle failed.
#define  KE_TYPE_CONVERSION      (0xC0000014L) // Failed on type conversion (usually in data value).
#define  KE_TYPE_MISMATCH        (0xC0000015L) // Type not as expected.
#define  KE_TRANSLATION          (0xC0000016L) // Scaling or translation error.
#define  KE_ILLEGAL_CONVERSION   (0xC0000017L) // Error when trying to change object attributes when not allowed.
#define  KE_NOTFOUND             (0xC0000018L) // Object not found.
#define  KE_INVALID_HANDLE       (0xC0000019L) // Invalid handle.
#define  KE_INVALID_NAME         (0xC000001AL) // Invalid object name.
#define  KE_ALREADY_EXIST        (0xC000001BL) // Trying to add an object that already exists.
#define  KE_BAD_QUALITY          (0xC000001CL) // I/O operation got a bad quality.
#define  KE_OUTOFRANGE           (0xC000001DL) // The value provided is out of the accceptable range.
#define  KE_INVALID_COMPONENT_ID (0XC000001EL) // There is an invalid component ID in the API message.
#define  KE_INVALID_FUNCTION_ID  (0XC000001FL) // There is an invalid function ID in the API message.
#define  KE_INVALID_VERSION      (0XC0000020L) // There is an invalid version in API message.
#define  KE_NOT_INITIALIZED      (0XC0000021L) // The module wasn't initialized.
#define  KE_LISTENER_NOTFOUND	 (0xC0000022L) // Cannot find listener for subscription, e.g. data item, tag, or alarm.
#define  KE_SERVER_NOTFOUND	     (0xC0000023L) // Cannot find the server (for example, an e-mail smtp server).
#define  KE_NOT_READY            (0xC0000024L) // General Not-Ready Error
#define  KE_WOULDBLOCK           (0xC0000025L) // connect or read or write socket "would block"
#define  KE_SCHEMA_NOT_FOUND     (0xC0000026L) // Trigger or Action shema name not found.
#define  KE_ACTION_NOT_SUPPORTED (0xC0000027L) // Action name not supported by invoked IAction component.
#define  KE_TRIGGER_NOT_SUPPORTED (0xC0000028L) // Trigger name not supported by subscribed ITrigger component.
#define  KE_FILE_DIRECTORY       (0xC0000029L) // Can find model directory.
#define  KE_UNKNOWN_SOAP_METHOD  (0xC000002AL) // Invalid SOAP method in internet communication
#define  KE_BUFFER_OVERFLOW      (0xC000002BL) // buffer overflow
#define  KE_BROKEN_PIPE          (0xC000002CL) // socket disconnect
#define  KE_ERROR_IMCOMPLETE     (0xC000002DL) // partial error for multiple items operations
#define  KE_SEND_NOT_COMPLETE    (0xC000002EL) // Send function has sent < than we ask
#define  KE_INTERFACE_NOT_FOUND  (0xC000002FL) // Requested interface not found
#define  KE_BAD_SOAP_FORMAT      (0xC0000030L) // Bad or Unknow SOAP format
#define  KE_OBSOLETE             (0xC0000031L) // Function no longer supported (depricated).
#define  KE_BAD_XML_FORMAT       (0xC0000032L) // Bad or Unknow XML format
#define  KE_EINPROGRESS          (0xC0000033L) // WSAEINPROGRESS

// Node 
#define  KE_NODE_NOT_FOUND       (0XC0000100L) // The specified node is not found.
#define  KE_NODE_NOT_CONNECTED   (0XC0000101L) // The specified node is not connected.


// OPC Data Source 
#define  KE_CHANGE_STATE         (0xC0000200L) // Trying to change an object state, such as from active to inactive. 


// Data source 
#define  KE_ADD_DS_OBJECT        (0xC0000300L) // Failed to add a data source object.
#define  KE_REMOVE_DS_OBJECT     (0xC0000301L) // Failed to remove a data source object.
#define  KE_DS_NOT_FOUND         (0xC0000302L) // The data source object was not found.
#define  KE_DS_SET_CALLBACK      (0xC0000303L) // Failed on setting the callback.
#define  KE_DS_RESET_CALLBACK    (0xC0000304L) // Failed on resetting the callback.
#define  KE_DS_INVALID_DS        (0xC0000305L) // Invalid data source definition
#define  KE_MULTIPLE_ACCESS      (0xC0000306L) // Multiple clients accessing same server item
#define  KE_POLLING_MODE_ONLY    (0xC0000307L) // Accept Polling mode only
#define  KE_ONE_CLIENT_ONLY      (0xC0000308L) // Data Item be linked with one client only

// Data item 
#define  KE_DI_NOT_FOUND         (0xC0000400L) // The specified data item was not found.

// Tag 
#define  KE_BRANCH_NOT_FOUND	 (0xC0000500L) // The specified branch object was not found. 
#define  KE_TAG_NOT_FOUND		 (0xC0000501L) // The specified tag object was not found. 	
#define  KE_STYLE_NOT_FOUND		 (0xC0000502L) // The specified style object was not found.
#define  KE_INVALID_BLOCK_DATA   (0xC0000503L) // Block subscribe tags must all have the same data source.
#define  KE_BLOCK_CALLBACKS_NOT_SUPPORTED (0xC0000504L) // Tag Data source does not support block callbacks.

// Alarm 
#define  KE_ADD_AREA_OBJECT      (0xC0000600L) // Failed to add the alarm area.
#define  KE_AREA_NOT_FOUND       (0xC0000601L) // The specified alarm area was not found. 
#define  KE_ALARM_NOT_FOUND      (0xC0000602L) // The specified alarm was not found. 
#define  KE_FILTER_IN_USE   	 (0xC0000603L) // The specified alarm filter is in use by subscription (and cannot be deleted).
#define  KE_FILTER_NOT_FOUND     (0xC0000604L) // The specified filter was not found. 
#define  KE_FILTER_ALREADY_EXIST (0xC0000605L) // A filter with the specified name already exists. 
#define  KE_INVALID_ACKNOWLEDGE_TIME (0xC0000606L)  // The current alarm time is different from the alam acknowledge time.

// Enumerator 
#define  KE_ENUM_NOT_FOUND		 (0xC0000700L) // The Enumerator object was not found. 
#define  KE_ENUM_DATA_REMOVED    (0xC0000701L) // The Enumeration position is bad, due to an item being removed. 

// E-mail 
#define  KE_SOCKET_ERROR         (0xC0000801L) // Socket error.
#define  KE_MAILSERVER_RESPONSE  (0xC0000803L) // Mail server did not like parm. 

// Security
#define  KE_INVALID_USER         (0xC0000900L) // Invalid User
#define  KE_INVALID_AUTHORIZATION (0xC0000901L) // Invalid authorization

// Expression
#define  KE_INVALID_EXPRESSION      (0xC0000A00L) // Invalid Expression.
#define  KE_INVALID_EXP_ITEM_NAME   (0xC0000A01L) // Expression contains invalid item name
#define  KE_DIVISION_BY_ZERO        (0xC0000A02L) // Division by zero
#define  KE_UNKNOWN_VARIABLE        (0xC0000A03L) // Unknown expression varible name
#define  KE_EMPTY_EXPRESSION        (0xC0000A04L) // empty expression string
#define  KE_MISSING_OPERATOR        (0xC0000A05L) // Missing expressioin operator
#define  KE_SYNTAX_ERROR            (0xC0000A06L) // Expression syntax error
#define  KE_UNBALANCED_PAREN        (0xC0000A07L) // Missing expression parenthesis
#define  KE_UNKNOWN_FUNCTION        (0xC0000A08L) // Unknown function in expression
#define  KE_UNKNOWN_OPERATOR        (0xC0000A09L) // Unknown operator in expression
#define  KE_MISSING_IF              (0xC0000A0AL) // Expressin missing if
#define  KE_MISSING_THEN_ELSE       (0xC0000A0BL) // Expression missing then...else
#define  KE_EXPRESSION_TOO_LONG     (0xC0000A0CL) // Expression is too long.
#define  KE_RECURSIVE_EXPRESSION    (0xC0000A0DL) // Recursive Expression.

#define  KE_LN_NONPOSITIVE_NUMBER	(0xC0000A10L) // LN on non positive number
#define  KE_LOG_NONPOSITIVE_NUMBER  (0xC0000A11L) // LOG on non positive number
#define  KE_SQRT_NEGATIVE_NUMBER	(0xC0000A12L) // SQRT on Negative number
#define  KE_TAN_INFINITE_RESULT		(0xC0000A13L) // TAN Infinite Result
#define  KE_SEC_INFINITE_RESULT		(0xC0000A14L) // SEC Infinite Result
#define  KE_CSC_INFINITE_RESULT		(0xC0000A15L) // CSC Infinite Result
#define  KE_COT_INFINITE_RESULT		(0xC0000A16L) // COT Infinite Result
#define  KE_ASIN_VALUE_OUTOFBOUNDS	(0xC0000A17L) // ASIN input value out of Bound
#define  KE_ACOS_VALUE_OUTOFBOUNDS	(0xC0000A18L) // ACOS input value out of Bound
#define  KE_ASEC_VALUE_OUTOFBOUNDS	(0xC0000A19L) // ASEC input value out of Bound
#define  KE_ACSC_VALUE_OUTOFBOUNDS	(0xC0000A1AL) // ACSC input value out of Bound
#define  KE_ACOT_DIVISION_BY_ZERO   (0xC0000A1BL) // ACOT division by zero
#define  KE_HTAN_INFINITE_RESULT	(0xC0000A1CL) // HTAN Infinite Result
#define  KE_HSEC_INFINITE_RESULT	(0xC0000A1DL) // HSEC Infinite Result
#define  KE_HCSC_INFINITE_RESULT	(0xC0000A1EL) // HCSC Infinite Result
#define  KE_HCOT_INFINITE_RESULT	(0xC0000A1FL) // HCOT Infinite Result

// Data Logger
#define  KE_INVALID_DATA_SET        (0xC0000B00L) // Invalid Data Set.
#define  KE_INVALID_DATA_SET_ITEM_NAME (0xC0000B01L) // Invalid Data Set Data Item name
#define  KE_STREAMING_NOT_ENABLED   (0xC0000B02L) // Can not subscribe for log updates if streaming is disabled.
#define  KE_TIMER_MODULE_REQUIRED   (0xC0000B03L) // Timer module requied for TimeBased logging
#define  KE_LOGGING_NOT_ENABLED     (0xC0000B04L) // Can not subscribe for log updates if file type is none.
#define  KE_LOGGER_DB_FAILURE       (0xC0000B05L) // general EDb error
#define  KE_LOGGER_DB_CORRUPTED     (0xC0000B06L) // EDb data demarshalling error

// Version Management
#define KE_WAITING                  (0xC0000C00L) // Waiting for response to post
#define KE_UPGRADING                (0xC0000C01L) // In the process of upgrading
#define KE_UPGRADE_COMPLETE         (0xC0000C02L) // Upgrade is complete
#define KE_NO_UPGRADE               (0xC0000C03L) // No upgrade was available

// Notification (Event) Manager Component erros
#define KE_ACTION_COMPONENT_NOT_FOUND (0xC0000D01L) // Action component not found
#define KE_TRIGGER_COMPONENT_NOT_FOUND (0xC0000D02L)// TRIGGER component not found

// WAAG Server 
#define KE_INVALID_SESSION_ID       (0xC0000E01L)   // could not find Multi-request per serverSessionID.
#define KE_INVALID_FILE_FORMAT      (0xC0000E02L)   // Downloaded file has invalid format (Netsilicon)
#define KE_INVALID_FILE_SIZE        (0xC0000E03L)   // Downloaded file is too big for ROM (Netsilicon)

// Web 
#define  KE_POST                    (0xC0000F00L)   // error while posting.
#define  KE_INVALID_REQUEST         (0xC0000F01L)   // web client request not valid
#define  KE_NO_SERVER               (0xC0000F02L)   // no server for client req
#define  KE_RECEIVE_INCOMPLETE      (0xC0000F03L)   // receive incomplete
#define  KE_INVALID_PORT            (0xC0000F04L)   // Port invalid

// SNMP
#define  KE_SNMP_BASE                (0xC0001000L)
#define  KE_SNMP_ERROR(x)            (KE_SNMP_BASE + (x))

#define  KE_SNMP_TOOBIG	             (KE_SNMP_BASE + 1L)
#define  KE_SNMP_NOSUCHNAME          (KE_SNMP_BASE + 2L)
#define  KE_SNMP_BADVALUE            (KE_SNMP_BASE + 3L)
#define  KE_SNMP_READONLY            (KE_SNMP_BASE + 4L)
#define  KE_SNMP_GENERR	             (KE_SNMP_BASE + 5L)

#define  KE_SNMP_NOACCESS            (KE_SNMP_BASE + 6L)
#define  KE_SNMP_WRONGTYPE           (KE_SNMP_BASE + 7L)
#define  KE_SNMP_WRONGLENGTH         (KE_SNMP_BASE + 8L)
#define  KE_SNMP_WRONGENCODING       (KE_SNMP_BASE + 9L)
#define  KE_SNMP_WRONGVALUE          (KE_SNMP_BASE + 10L)
#define  KE_SNMP_NOCREATION          (KE_SNMP_BASE + 11L)
#define  KE_SNMP_INCONSISTENTVALUE   (KE_SNMP_BASE + 12L)
#define  KE_SNMP_RESOURCEUNAVAILABLE (KE_SNMP_BASE + 13L)
#define  KE_SNMP_COMMITFAILED        (KE_SNMP_BASE + 14L)
#define  KE_SNMP_UNDOFAILED          (KE_SNMP_BASE + 15L)
#define  KE_SNMP_AUTHORIZATIONERROR  (KE_SNMP_BASE + 16L)
#define  KE_SNMP_NOTWRITABLE         (KE_SNMP_BASE + 17L)

#define  KE_SNMP_BADOID              (KE_SNMP_BASE + 0x20L)   // bad oid

#define  KE_SNMP_BADWRITE_VAR        (KE_SNMP_BASE + 0x30L)   // bad write variable
#define  KE_SNMP_WRITE_FAILURE       (KE_SNMP_BASE + 0x31L)   // bad write 
#define  KE_SNMP_PACKET_ERROR        (KE_SNMP_BASE + 0x32L)   // packet error
#define  KE_SNMP_BADXMLFILE          (KE_SNMP_BASE + 0x33L)   // bad XML file format
#define  KE_SNMP_BADARGUMENT         (KE_SNMP_BASE + 0x34L)   // bad argument in calling
#define  KE_SNMP_BADIPADDRESS        (KE_SNMP_BASE + 0x35L)   // bad argument in calling

// WebClient
// WebClient errors are defined as EWebResult errors in EWebClient.h.
// To make a KERESULT from  EWebResult use the EWEBRESULT_TO_KERESULT macro.
#define  KE_WEB_CLIENT_BASE          (0xC0001100L)
#define  EWEBRESULT_TO_KERESULT(X) (KE_WEB_CLIENT_BASE | X)

// TarGzip errors.
#define KE_TARGZIP_BASE                     (0xC0001200L)
#define KE_TARGZIP_FILENAME_TOO_LONG        (KE_TARGZIP_BASE + 0)
#define KE_TARGZIP_FILE_FAILURE             (KE_TARGZIP_BASE + 1)   // Non-specific file failure
#define KE_TARGZIP_CREATE_FAILURE           (KE_TARGZIP_BASE + 2)   
#define KE_TARGZIP_EXTRACTION_FAILURE       (KE_TARGZIP_BASE + 3)   
#define KE_TARGZIP_LIST_FAILURE             (KE_TARGZIP_BASE + 4)   


//SCM errors
#define KE_SCM_BASE                         (0xC0001300L)
#define KE_SCM_RESTART                      (0x40001301L)       // note:  THIS IS NOT AN ERROR.
#define KE_SCM_ABORTED                      (KE_SCM_BASE + 0)
#define KE_SCM_DOWNLOAD_EXECUTION_FAILURE   (KE_SCM_BASE + 1)
#define KE_SCM_NO_UPLOAD_FILES              (KE_SCM_BASE + 2)
#define KE_SCM_PARTIAL_UPLOAD_FILES         (KE_SCM_BASE + 3)
#define KE_SCM_EMPTY_ARCHIVE                (KE_SCM_BASE + 4)   // a status more then an error
#define KE_SCM_HTTP_STATUS_ERROR            (KE_SCM_BASE + 5)
#define KE_SCM_REGISTRY_ENTRY_NOT_FOUND     (KE_SCM_BASE + 6)
#define KE_SCM_REGISTRY_FILE_ERROR          (KE_SCM_BASE + 7)
#define KE_SCM_MD5_CHUNK_ERROR              (KE_SCM_BASE + 8)
#define KE_SCM_MD5_FILE_ERROR               (KE_SCM_BASE + 9)
#define KE_SCM_UPLOAD_FILE_NOT_FOUND        (KE_SCM_BASE + 10)
#define KE_SCM_DEPENDENCY_FAILED            (KE_SCM_BASE + 11)
#define KE_SCM_INVALID_DIRECTORY            (KE_SCM_BASE + 12)  // directory can not be created.
#define KE_SCM_RESTART_OF_GATEWAY_DEVICE    (KE_SCM_BASE + 13)  // Can not restart a Gateway device (only the Gateway itself).
#define KE_CUSTOM_SOAP_FUNCTION_ERROR       (KE_SCM_BASE + 14)  // General Purpose use by custom SOAP functions that have an error.
#define KE_SCM_UPLOAD_DENIED                (KE_SCM_BASE + 15)  // upload is denied (by APM)
#define KE_SCM_MULTIFILE_UNCOMPRESSED       (KE_SCM_BASE + 16)  // multi-file transfer without compression requested.

// ScriptManager
#define KE_SCRIPT_BASE                      (0xC0001400L)
#define KE_SCRIPT_BAD_STATE                 (KE_SCRIPT_BASE + 0)    // Script's current state does not allow the required action
#define KE_SCRIPT_RUNNING                   (KE_SCRIPT_BASE + 1)    // The script is running.
#define KE_SCRIPT_NO_SCRIPT_FILE            (KE_SCRIPT_BASE + 2)    // The sequence file was not found.
#define KE_SCRIPT_NOT_FOUND                 (KE_SCRIPT_BASE + 3)    // The named script was not found.
#define KE_DUPLICATED_NAME                  (KE_SCRIPT_BASE + 4)    // Dupliacted script names are not allowed
#define KE_SCRIPT_NOT_RUNNING               (KE_SCRIPT_BASE + 5)    // The script is not running.
#define KE_SCRIPT_INVALID_SCRIPT_CMD        (KE_SCRIPT_BASE + 6)    // Script has invalid command lines.
#define KE_SCRIPT_BADXMLFILE                (KE_SCRIPT_BASE + 7)    // Script configuration file is not valid.
#define KE_SCRIPT_EMPTY_NAME                (KE_SCRIPT_BASE + 8)    // Script has empty name.
#define KE_SCRIPT_WAITING                   (KE_SCRIPT_BASE + 9)    // Script is waiting. 
#define KE_SCRIPT_DISABLED                  (KE_SCRIPT_BASE + 10)   // Script is disabled
#define KE_SCRIPT_SCHEDULED                 (KE_SCRIPT_BASE + 11)   // Scirpt is scheduled to the trigger already
#define KE_SCRIPT_NOT_SCHEDULED             (KE_SCRIPT_BASE + 12)   // Script is not scheduled to the trigger
#define KE_SCRIPT_NOT_DYNAMIC               (KE_SCRIPT_BASE + 13)   // Script is not dynamic
#define KE_SCRIPT_FINISH_WITH_ERROR         (KE_SCRIPT_BASE + 14)   // Script finished with error
#define KE_SCRIPT_INVISIBLE_EXISTS          (KE_SCRIPT_BASE + 15)   // Invisible script with the same name already exists

// Gateway (Provisioning) errors
#define KE_GATEWAY_BASE                     (0xC0001500L)
#define KE_NO_PROVISIONING_AUTO_DISCOVERY   (KE_GATEWAY_BASE + 0)  // Provisioning modify/delete not allowed w/ auto-discovered devices

// Policy Manager errors
#define KE_PM_BASE                          (0xC0001600L)
#define KE_PM_OFFLINE                       (KE_PM_BASE + 0)    // APM is offline
#define KE_PM_PERMISSION_DENIED             (KE_PM_BASE + 1)    // Can not do Command due to APM denial.
#define KE_PM_DEVICE_MUST_BE_REGISTERED     (KE_PM_BASE + 2)    // Can not do command till device is registered w/ APM

// Crypto-related errors
#define KE_CRYPTO_BASE                      (0xC0001700L)
#define KE_CRYPTO_NO_KEY_FILE               (KE_CRYPTO_BASE + 0)    // Failed to open key file
#define KE_CRYPTO_BAD_KEY_FILE              (KE_CRYPTO_BASE + 1)    // Invalid key file
#define KE_CRYPTO_BAD_KEY                   (KE_CRYPTO_BASE + 2)    // Invalid key
#define KE_CRYPTO_BAD_CIPHERTEXT            (KE_CRYPTO_BASE + 3)    // Received ciphertext is invalid
#define KE_CRYPTO_PADDING_ERROR             (KE_CRYPTO_BASE + 4)    // Received ciphertext padding is invalid
#define KE_CRYPTO_ALGORITHM_DISABLED        (KE_CRYPTO_BASE + 5)    // Crypto algorithm disabled

// FileWatch errors
#define KE_FILEWATCH_BASE                   (0xC0001800L)
#define KE_FILEWATCH_BAD_CONFIG             (KE_FILEWATCH_BASE + 0) // Invalid configuration file

// Mobile DRM errors
#define KE_MOBILE_DRM_BASE                  (0xC0001900L)
#define KE_MOBILE_DRM_CHECKSUM              (KE_MOBILE_DRM_BASE + 0) // checksum error
#define KE_MOBILE_DRM_IMPORT_PROCESSING     (KE_MOBILE_DRM_BASE + 1) // Processing error

// AuthProxy errors
#define KE_AUTH_PROXY_BASE                  (0xC0001A00L)
#define KE_AUTH_PROXY_PROVIDER_UNKNOWN      (KE_AUTH_PROXY_BASE + 0)    // authentication provider unknown
#define KE_AUTH_PROXY_PROVIDER_UNAVAIL      (KE_AUTH_PROXY_BASE + 1)    // authentication provider unavailable
#define KE_AUTH_PROXY_ACCESS_DENIED         (KE_AUTH_PROXY_BASE + 2)    // access denied
#define KE_AUTH_PROXY_NEED_MORE_CREDENTIALS (KE_AUTH_PROXY_BASE + 3)    // more credentials needed in the context


// SyslogClient component errors
#define KE_SYSLOG_CLIENT_BASE               (0xC0001B00L)
#define KE_SYSLOG_CLIENT_SOCKET_INIT        (KE_SYSLOG_CLIENT_BASE + 0)	// socket initialization failed
#define KE_SYSLOG_CLIENT_BAD_XML_FORMAT     (KE_SYSLOG_CLIENT_BASE + 1)	// bad xml config file

// Data Protection errors
#define KE_DP_BASE                          (0xC0001C00L)
#define KE_DP_NO_PROVIDERS                  (KE_DP_BASE + 0)        // No Data Protection providers available
#define KE_DP_PROVIDER_UNKNOWN              (KE_DP_BASE + 1)        // Data Protection provider unknown
#define KE_DP_MAC_VERIFY_FAILED             (KE_DP_BASE + 2)        // MAC verification failed
#define KE_DP_MAC_FILE_IO_ERROR             (KE_DP_BASE + 3)        // I/O error creating file MAC
#define KE_DP_MASTER_KEY_OPEN_FAILED        (KE_DP_BASE + 4)        // Could not open Data Protection master key
#define KE_DP_MASTER_KEY_CREATE_FAILED      (KE_DP_BASE + 5)        // Could not create Data Protection master key
#define KE_DP_MASTER_KEY_IO_ERROR           (KE_DP_BASE + 6)        // I/O error accessing Data Protection master key
#define KE_DP_MASTER_KEY_INVALID            (KE_DP_BASE + 7)        // Data Protection master key is invalid
#define KE_DP_MASTER_KEY_NOT_FOUND          (KE_DP_BASE + 8)        // Could not find master key for data unprotection/authentication
#define KE_DP_ALGORITHM_UNKNOWN             (KE_DP_BASE + 9)        // Data Protection algorithm is unknown
#define KE_DP_INVALID_FORMAT                (KE_DP_BASE + 10)       // Protected data or authentication code has invalid format

#endif

