/********************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//********************************************************************************
//
//  Filename   :  CustomHandlesr.h
//  
//  Subsystem  :  Axeda Connector/Gateway
//
//  Description:  
//			
//
//********************************************************************************/

#ifndef __CUSTOM_HANDLERS_H__
#define __CUSTOM_HANDLERS_H__

#include <time.h>

#ifdef WIN32
    #ifdef _AXEDA_CUSTOM_HANDLERS_EXPORTS
        #define _AXEDA_CUSTOM_HANDLERS_DECLSPEC __declspec(dllexport)
        #define _AXEDA_CUSTOM_HANDLERS_EXTERN
    #else
        #define _AXEDA_CUSTOM_HANDLERS_DECLSPEC __declspec(dllimport)
        #define _AXEDA_CUSTOM_HANDLERS_EXTERN   extern
    #endif
#else
    #define _AXEDA_CUSTOM_HANDLERS_DECLSPEC
    #define _AXEDA_CUSTOM_HANDLERS_EXTERN
#endif


/********************************************************************************/
/*  Custom code Exports are imported by the CustomHander library*/
/********************************************************************************/

#ifdef WIN32
    #ifdef _AXEDA_CUSTOM_EXPORTS
        #define _AXEDA_CUSTOM_DECLSPEC __declspec(dllexport)
        #define _AXEDA_CUSTOM_EXTERN
    #else
        #define _AXEDA_CUSTOM_DECLSPEC __declspec(dllimport)
        #define _AXEDA_CUSTOM_EXTERN   extern
    #endif
#else
    #define _AXEDA_CUSTOM_DECLSPEC
    #define _AXEDA_CUSTOM_EXTERN
#endif

/********************************************************************************/
/* data type definitions  */
#define AX_True          1
#define AX_False         0

typedef int AX_BOOL;
#ifdef _WIN64
typedef unsigned __int64 AX_Handle;
#else
typedef unsigned long AX_Handle;
#endif
typedef unsigned short AX_UShort;

#ifndef UNIX
typedef unsigned short AX_Char;
#else
typedef wchar_t AX_Char;
#endif

typedef struct _AX_TimeValue AX_TimeValue;
typedef struct _AX_DataValue AX_DataValue;
typedef struct _AX_DataItem AX_DataItem;
typedef struct _AX_Event AX_Event;
typedef struct _AX_Alarm AX_Alarm;
typedef struct _AX_Upload AX_Upload;
typedef struct _AX_Download AX_Download;
typedef struct _AX_RemoteSession AX_RemoteSession;
typedef struct _AX_RemoteSessionPort AX_RemoteSessionPort;

/* Error and Status code definitions  */
typedef enum
{
    AX_NO_ERROR,
    AX_GENERAL_ERROR
} AX_ErrorCodes;

/* Date/time representation structure. */
struct _AX_TimeValue
{
    time_t  iSec;               /* seconds since 00:00:00 Jan 2 1970 UTC */
    int     iMilliSec;          /* milliseconds (1/1000th sec) */
};

#define AX_DEFAULT_DEVICE_ID -1

/*  ***************************  Data Item Definitions  **********************/
/* Data item value type. */
typedef enum
{
    AX_DataAnalog = 1,            /* analog */
    AX_DataDigital,               /* digital (0/1) */
    AX_DataString                 /* string */
} AX_DataType;

/* Data item value quality. */
typedef enum
{
    AX_DataGood = 0xC0,           /* good */
    AX_DataBad = 0,               /* bad */
    AX_DataUncertain = 0x40       /* uncertain (undefined) */
} AX_DataQuality;

/* Data item value structure. */
struct _AX_DataValue
{
    AX_DataType       iType;      /* type of data */
    AX_DataQuality    iQuality;   /* quality of data */
    AX_TimeValue      timeStamp;  /* timestamp when data acquired */
    union
    {
        double      dAnalog;        /* analog value */
        AX_BOOL     bDigital;       /* digital value */
        AX_Char*    pszString;      /* string value.  */
                                    /* To modify string, malloc() new string.  The caller will free() it */
    } data;
};

/* Data item structure. */
struct _AX_DataItem
{
    int           iDeviceId;      /* device id */
    AX_Char*      pszName;        /* name */
    AX_DataValue  value;          /* value */
    AX_BOOL       bInhibit;       /* default:  AX_False.  */
                                  /* Set to AX_True to inhibit further processing of this data item */

};

/*  ***************************  Alarm Definitions  ************************/

typedef enum
{
    AX_NoType,
    AX_Level,               /* Sub conditions include:  Low, High, LowLow, HighHigh  */
    AX_Deviation,           /* Sub conditions include:  Low, High  */
    AX_Dynamic,             /* Dynamic alarms are generated via an EDD Driver or Scripting. No Sub conditions */
    AX_Digital              /* Sub conditions include:  Low, High  */
} AX_ALARM_TYPE;

typedef enum
{
    AX_NoSubType,
    AX_Low,
    AX_High,
    AX_LowLow,
    AX_HighHigh
} AX_ALARM_SUBTYPE;


struct _AX_Event
{
    int                 iDeviceId;   /* device id */

    /* The name, description and severity can be modified */
    AX_Char*            pszName;     /* name of event/alarm.  This is usually the name of the data item with */
                                     /* which the alarm is associated. */
                                     /* If modified, programmer needs to free() string and allocate a new one. */
    AX_Char*            pszDescription; /* event/alarm description (also called the event/alarm message) */
                                        /* If modified, programmer needs to free() string and allocate a new one. */
    int                 severity;    /* Number indicating the severity of the event/alarm */

    /* Set the bInhibit flag to AX_True to inhibit publishing of this event/alarm. */
    AX_BOOL             bInhibit;   /* default:  AX_False.  */
                                    
    /* The following members can NOT be modified */
    AX_TimeValue        timeStamp;  /* timestamp of event/alarm*/
};

struct _AX_Alarm
{
    AX_Event            event;       /* All alarms are a super-set of events. */

    /* The following members can NOT be modified */
    AX_ALARM_TYPE       type;       /* type of alarm */
    AX_ALARM_SUBTYPE    subType;    /* Some alarms have sub-types that for describe the alarm */
    AX_BOOL             bActive;    /* AX_TRUE if the alarm is active; AX_FALSE is the alarm is inactive */
    AX_DataItem         dataItem;   /* data value that caused alarm to be fired. */
                                    /* If then  DataItem name is NULL, then alarm was not initiated */
                                    /* by a data value change */
};


struct _AX_Upload
{
    int         iDeviceId;          /* device id */

    AX_Char*    pszFileName;        /*  name of file to be uploaded. */
                                    /* file may have absolute path or be relative to the 
                                       Agent executable directory. */

    /* bBeforeRequest is true when the Agent has not yet requested the Enterprise Server to 
       upload the file.
       bBeforeRequest is false when the Enterprise Server has already requested the file
       to be uploaded from the Agent.  
    */
    AX_BOOL     bBeforeRequest;

    /* Set the bInhibit flag to AX_True to inhibit uploading of this file. */
    AX_BOOL     bInhibit;
};

struct _AX_Download
{
    int         iDeviceId;          /* device id */
    AX_Char*    pszFileName;        /* name of file to be downloaded. */
                                    /* file may have absolute path or be relative to the 
                                       Agent executable directory. */
};

struct _AX_RemoteSessionPort
{
    AX_UShort   iPort;
    AX_BOOL     bListen;
    AX_RemoteSessionPort*   pNext;
};

struct _AX_RemoteSession
{
    int         iDeviceId;          /* device id */
    AX_Char*    pszName;            /* session name */
    AX_Char*    pszDescription;     /* session description */
    AX_Char*    pszRemoteHost;      /* remote host name (for Agent-to-RemoteHost connections only) */
    AX_Char*    pszLaunchType;      /* user mode in which the session is used, e.g. "telnet" */
    AX_RemoteSessionPort *pPortList;    /* NULL-terminated linked list of managed ports */

    /* Set the bInhibit flag to AX_True to inhibit remote session */
    AX_BOOL     bInhibit;

    AX_Char*    pszUserId;          /* Enterprise user id */
    AX_Char*    pszSessionId;       /* session id */
};

/*  ***************************  Logging  **********************/
typedef enum
{
    AX_LogDebug = 1,
    AX_LogInfo = 3,
    AX_LogWarning = 5,
    AX_LogImportantInfo = 6,
    AX_LogError = 9,
    AX_LogCriticalError = 10,
} AX_LogEvent;

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************/

/* Functions exported from the SDK library:  ECustomHandler.lib  */

/* Initializes Custom Handler. Must be called from AX_Initialize(). pParam1 and pParam2
   correspond to the arguments of AX_Initialize(). */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC void* AX_InitializeCustomHandler(void* pParam1, void* pParam2);

/* Retrieves version number of the Custom Handler interface with the Agent. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC void AX_GetVersion(int* pMajorVersion, int* pMinorVersion);

/* Retrieves version number of the Agent. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC void AX_GetKernelVersion(int* pMajorVersion, int* pMinorVersion,
                                                         int* pPatch, double* pBuild);

/* Retrieves device model number given the device id. The result string is stored in pBuf
   that has a length of iLength bytes. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_GetModelNumber(int iDevId, AX_Char *pBuf, int iLength);

/* Retrieves device serial number given the device id. The result string is stored in pBuf
   that has a length of iLength bytes. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_GetSerialNumber(int iDevId, AX_Char *pBuf, int iLength);

/* Retrieves device id into piDevId given model number and serial number strings. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_GetDeviceId(AX_Char *pModelNumber, AX_Char *pSerialNumber,
                                                       int *piDevId);

/* Retrieves values of the data items specified by pItems (an array of iCount elements).
   Note that the function allocates memory for string values. Caller should use
   AX_FreeValue() after processing is complete. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_GetDataItems(int iCount, AX_DataItem* pItems);

/* Modifies values of the data items specified by pItems (an array of iCount elements). */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_SetDataItems(int iCount, AX_DataItem* pItems);

/* Publishes an event specified by pEvent. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_CreateEvent(AX_Event* pEvent);

/* Starts an alarm specified by pAlarm. Note that type, subType and dataItem fields of
   the AX_Alarm structure are ignored. Alarm handle is returned in pHandle, which may be
   used later to stop alarm. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_StartAlarm(AX_Alarm* pAlarm, AX_Handle* pHandle);

/* Stops an alarm specified by the handle returned from AX_StartAlarm(). */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_EndAlarm(AX_Handle handle);

/* Causes the Agent to post a custom XML message associated with device specified by
   iDevId. This assumes that corresponding processing servlet exists on the server side. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_PostMessage(int iDevId, AX_Char *pszMessage);

/* Causes the Agent to upload a file associated with device specified by iDevId. bDelete
   flag indicates whether the file must be deleted when upload is complete. */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_UploadFile(int iDevId, AX_Char *pszFile, AX_BOOL bDelete);

/* Logs a custom message to the Agent log. The logged message is specified by the event
   type (iEvent), source identification string (pszSource) -- this is typically the DLL
   name, and the message text (pszMessage). */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC AX_BOOL AX_LogMessage(AX_LogEvent iEvent, AX_Char *pszSource,
                                                      AX_Char *pszMessage);

/* Frees memory that could be allocated by AX_GetDataItems(). */
_AXEDA_CUSTOM_HANDLERS_DECLSPEC void AX_FreeValue(AX_DataValue *pValue);


/*  ***************************  Function Definitions  **********************/

/*  functions required in the Custom code DLL */
_AXEDA_CUSTOM_DECLSPEC void* AX_Initialize(void* pParam1, void* pParam2);

/*  OPTIONAL Custom Functions  */
_AXEDA_CUSTOM_DECLSPEC AX_ErrorCodes AX_StartUp(void);
_AXEDA_CUSTOM_DECLSPEC AX_ErrorCodes AX_ShutDown(void);
_AXEDA_CUSTOM_DECLSPEC void AX_OnDataChange(int count, AX_DataItem* pItems);
_AXEDA_CUSTOM_DECLSPEC void AX_OnEnterpriseDIWrite(AX_DataItem* pItem);
_AXEDA_CUSTOM_DECLSPEC void AX_OnEvent(AX_Event* pEvent);
_AXEDA_CUSTOM_DECLSPEC void AX_OnAlarm(AX_Alarm* pAlarm);
_AXEDA_CUSTOM_DECLSPEC void AX_OnFileUpload(AX_Upload* pUpload);
_AXEDA_CUSTOM_DECLSPEC void AX_OnFileDownloaded(AX_Download* pDownload);
_AXEDA_CUSTOM_DECLSPEC void AX_OnRemoteSession(AX_RemoteSession* pSession, AX_BOOL bOnStart);

#ifdef __cplusplus
}
#endif


#endif   /* __CUSTOM_HANDLERS_H__ */
