//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EOS_win32.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Classes for WIN32 and UNDER_CE platform portablility
//
//  Classes:      EMutex, EThread, EMutexWrapper, ESingleEvent
//				  VARIANT type implementation				
//
//**************************************************************************

#ifndef _EOS_WIN32_H
#define _EOS_WIN32_H

//**************************************************************************
// Globals
//**************************************************************************
#define PATH_DELIMITER_CHAR     _T('\\')
#define PATH_DELIMITER_STRN     _T("\\")

//////////// Globals for use with NT service ////////////////////////////////
//// These are here because Remote server and ELoader need access ///////////
#ifndef UNDER_CE
#define SERVICE_KEY					_T("SYSTEM\\CurrentControlSet\\Services\\")
#else
#define SERVICE_KEY					_T("Services\\")
#endif
#define CONNECTOR_SERVICE_NAME		_T("AxedaConnector")
#define CONNECTOR_BACKUP_NAME		_T("AxedaBackup")
#define CONNECTOR_DISPLAY_NAME		_T("Axeda Connector Service")
#define CONNECTOR_EXE_NAME			_T("EKERNEL")
#define GATEWAY_SERVICE_NAME		_T("AxedaGateway")
#define GATEWAY_DISPLAY_NAME		_T("Axeda Gateway Service")

#ifdef __GATEWAY__
#define AGENT_SERVICE_DESCRIPTION   _T("Provides real-time communication and data collection for remote service")
#else
#define AGENT_SERVICE_DESCRIPTION   _T("Provides real-time communication and data collection for remote service")
#endif

#define GATEWAY_EXE_NAME			_T("XGATE")

class _DV_DECLSPEC ESharedMemory
{
public:
	ESharedMemory(TCHAR *name, ULONG size);
	~ESharedMemory();
	
	DWORD AddToMem(void* pBuf, ULONG size, ULONG timeout = INFINITE);
	DWORD GetFromMem(void* pBuf, ULONG size, ULONG timeout = INFINITE);

private:
	TCHAR				m_Name[50];
	HANDLE				m_hSharedMemMutex;
	HANDLE				m_hSharedMemEvent;
	HANDLE				m_hSharedMemFile;
	ECircularBuffer*	m_pSircleBuffer;
	UCHAR*				m_pSharedBuffer;
	ULONG				m_Size;
};
	
//**************************************************************************
class _DV_DECLSPEC EMutexWrapper
{
public:
    EMutexWrapper()       { InitializeCriticalSection(&cs); }
    ~EMutexWrapper()      { DeleteCriticalSection(&cs); }
    
    void     Lock()   { EnterCriticalSection(&cs); }
    void     Unlock() { LeaveCriticalSection(&cs); }
    
private:
    CRITICAL_SECTION  cs;
};

//**************************************************************************
class _DV_DECLSPEC ESingleEvent
{
public:
    ESingleEvent()
                        { hEvent = CreateEvent(0, FALSE, FALSE, NULL); }

    ESingleEvent(const TCHAR *pszName, bool bCreate)
                        { hEvent = bCreate ?
                            CreateEvent(0, FALSE, FALSE, pszName) :
                            OpenEvent(EVENT_ALL_ACCESS, FALSE, pszName); }

    ~ESingleEvent()     { if (hEvent) CloseHandle(hEvent);              }
    bool operator!()    { return (hEvent == 0);                         }
    
    bool     Set()      { return (SetEvent(hEvent) != 0);        }
    bool     Reset()    { return (ResetEvent(hEvent) != 0);      }
    DWORD    Wait(DWORD dwMilliseconds = INFINITE)   
    { return WaitForSingleObject(hEvent, dwMilliseconds); }
    
private:
    HANDLE   hEvent;
};

// group=Thread Defines
//**************************************************************************
// {secret}
typedef unsigned long  ThreadReturn;                    
typedef ThreadReturn (__stdcall * FunctionPtr) (void*); // {secret}

#define EK_THREAD_PRIORITY_HIGHEST          THREAD_PRIORITY_HIGHEST                 
#define EK_THREAD_PRIORITY_ABOVE_NORMAL     THREAD_PRIORITY_ABOVE_NORMAL
#define EK_THREAD_PRIORITY_NORMAL           THREAD_PRIORITY_NORMAL      
#define EK_THREAD_PRIORITY_BELOW_NORMAL     THREAD_PRIORITY_BELOW_NORMAL
#define EK_THREAD_PRIORITY_LOWEST           THREAD_PRIORITY_LOWEST      
#undef Yield

// group=OS Wrappers

class _DV_DECLSPEC EProcess
{
public:
    static DWORD _GetCurrentProcessId();
    static DWORD WaitForPid(DWORD pid,DWORD milliTimeout);
	static BOOL KillPid(DWORD pid);
};


class _DV_DECLSPEC EThread
{
public:
    EThread(DWORD Size = 4096,bool bCurrent = false);
    ~EThread(void);
    bool            Begin(FunctionPtr PFN, void *X, TCHAR* pChar = NULL);
    void            Wait(void);
    unsigned        long ThreadId(void);
    bool            SetPriority(long priority);
    long            GetPriority();
    bool            IsRunning() {return m_threadId != 0;}
    bool            Kill();
    static void     Yield() {ESleep(0);}
    static DWORD    _GetCurrentThreadID();
    
    // Felix - This function is for OS without thread support
    static void     ContextSwitch() {}
    void operator= (EThread&);

private:
    void            Cleanup(void);

    DWORD           m_threadId;
    HANDLE          m_threadHandle;
    DWORD           m_stackSize;

    static ThreadReturn __stdcall ThreadStub(void *pParam);
};

// Group=OS Functions

// These functions are used for allocating in one DLL and freeing in another.
 _DV_DECLSPEC void* ETaskMemAlloc(ULONG cb );
 _DV_DECLSPEC void ETaskMemFree(void* pv);

class _DV_DECLSPEC EMutex
{
    friend class ESingleEvent;

public:
    EMutex(TCHAR* pDesc = _T("Unknown"));

    void Lock(TCHAR* pCaller = _T("Unknown"));

    void Unlock(TCHAR* pCaller = _T("Unknown"));

private:
    EMutexWrapper   m_mtxWrapper;

#ifdef DEBUG_DEADLOCK
    TCHAR           m_mtxDesc[50];
#endif

};

#ifndef UNDER_CE

#include <aclapi.h>

//********************************************************************
class EWin32SecurityDescriptor
{
public:
    EWin32SecurityDescriptor(void);
    ~EWin32SecurityDescriptor();

    bool                AddAdminACE(DWORD dwPermissions, ACCESS_MODE mode);
    bool                AddCurrentUserACE(DWORD dwPermissions, ACCESS_MODE mode);
    SECURITY_DESCRIPTOR *GetSecurityDescriptor(void);

private:
    bool    AddACE(TRUSTEE_FORM trusteeForm,
                   TRUSTEE_TYPE trusteeType,
                   LPTSTR pTrusteeName,
                   DWORD dwPermissions,
                   ACCESS_MODE mode);

    SECURITY_DESCRIPTOR *m_pSD;
    ACL                 *m_pACL;
};

_DV_DECLSPEC bool GetServiceName(TCHAR *pszAppPath, TCHAR *pszServiceName);
_DV_DECLSPEC bool RunService(TCHAR *pszServiceName);
_DV_DECLSPEC bool CheckInteractiveSession(bool& bInteractive);
_DV_DECLSPEC bool CheckInteractiveSession(HANDLE hAccessToken, bool& bInteractive);

#endif // UNDER_CE

_DV_DECLSPEC void OutputSystemError(void);
_DV_DECLSPEC void OutputSystemError(DWORD dwError);

//********************************************************************

// The following definitions are reproduced from iptypes.h and iphlpapi.h
// for dynamically loading and running GetAdaptersAddresses.

// Definitions and structures used by getnetworkparams and getadaptersinfo apis

#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
#define DEFAULT_MINIMUM_ENTITIES        32  // arb.
#define MAX_HOSTNAME_LEN                128 // arb.
#define MAX_DOMAIN_NAME_LEN             128 // arb.
#define MAX_SCOPE_ID_LEN                256 // arb.

typedef enum {
    IpPrefixOriginOther = 0,
    IpPrefixOriginManual,
    IpPrefixOriginWellKnown,
    IpPrefixOriginDhcp,
    IpPrefixOriginRouterAdvertisement,
} IP_PREFIX_ORIGIN;

typedef enum {
    IpSuffixOriginOther = 0,
    IpSuffixOriginManual,
    IpSuffixOriginWellKnown,
    IpSuffixOriginDhcp,
    IpSuffixOriginLinkLayerAddress,
    IpSuffixOriginRandom,
} IP_SUFFIX_ORIGIN;

typedef enum {
    IpDadStateInvalid    = 0,
    IpDadStateTentative,
    IpDadStateDuplicate,
    IpDadStateDeprecated,
    IpDadStatePreferred,
} IP_DAD_STATE;

typedef struct _IP_ADAPTER_UNICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct { 
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_UNICAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;

    IP_PREFIX_ORIGIN PrefixOrigin;
    IP_SUFFIX_ORIGIN SuffixOrigin;
    IP_DAD_STATE DadState;

    ULONG ValidLifetime;
    ULONG PreferredLifetime;
    ULONG LeaseLifetime;
} IP_ADAPTER_UNICAST_ADDRESS, *PIP_ADAPTER_UNICAST_ADDRESS;

typedef struct _IP_ADAPTER_ANYCAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct { 
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_ANYCAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_ANYCAST_ADDRESS, *PIP_ADAPTER_ANYCAST_ADDRESS;

typedef struct _IP_ADAPTER_MULTICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_MULTICAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_MULTICAST_ADDRESS, *PIP_ADAPTER_MULTICAST_ADDRESS;

//
// Per-address Flags
//
#define IP_ADAPTER_ADDRESS_DNS_ELIGIBLE 0x01
#define IP_ADAPTER_ADDRESS_TRANSIENT    0x02

typedef struct _IP_ADAPTER_DNS_SERVER_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Reserved;
        };
    };
    struct _IP_ADAPTER_DNS_SERVER_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_DNS_SERVER_ADDRESS, *PIP_ADAPTER_DNS_SERVER_ADDRESS;

typedef struct _IP_ADAPTER_PREFIX {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_PREFIX *Next;
    SOCKET_ADDRESS Address;
    ULONG PrefixLength;
} IP_ADAPTER_PREFIX, *PIP_ADAPTER_PREFIX;

//
// Per-adapter Flags
//
#define IP_ADAPTER_DDNS_ENABLED               0x01
#define IP_ADAPTER_REGISTER_ADAPTER_SUFFIX    0x02
#define IP_ADAPTER_DHCP_ENABLED               0x04
#define IP_ADAPTER_RECEIVE_ONLY               0x08
#define IP_ADAPTER_NO_MULTICAST               0x10
#define IP_ADAPTER_IPV6_OTHER_STATEFUL_CONFIG 0x20


//
// OperStatus values from RFC 2863
//
typedef enum {
    IfOperStatusUp = 1,
    IfOperStatusDown,
    IfOperStatusTesting,
    IfOperStatusUnknown,
    IfOperStatusDormant,
    IfOperStatusNotPresent,
    IfOperStatusLowerLayerDown
} IF_OPER_STATUS;

typedef struct _IP_ADAPTER_ADDRESSES {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD IfIndex;
        };
    };
    struct _IP_ADAPTER_ADDRESSES *Next;
    PCHAR AdapterName;
    PIP_ADAPTER_UNICAST_ADDRESS FirstUnicastAddress;
    PIP_ADAPTER_ANYCAST_ADDRESS FirstAnycastAddress;
    PIP_ADAPTER_MULTICAST_ADDRESS FirstMulticastAddress;
    PIP_ADAPTER_DNS_SERVER_ADDRESS FirstDnsServerAddress;
    PWCHAR DnsSuffix;
    PWCHAR Description;
    PWCHAR FriendlyName;
    BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD PhysicalAddressLength;
    DWORD Flags;
    DWORD Mtu;
    DWORD IfType;
    IF_OPER_STATUS OperStatus;
    DWORD Ipv6IfIndex;
    DWORD ZoneIndices[16];
    PIP_ADAPTER_PREFIX FirstPrefix;
} IP_ADAPTER_ADDRESSES, *PIP_ADAPTER_ADDRESSES;

//
// Flags used as argument to GetAdaptersAddresses().
// "SKIP" flags are added when the default is to include the information.
// "INCLUDE" flags are added when the default is to skip the information.
//
#define GAA_FLAG_SKIP_UNICAST       0x0001
#define GAA_FLAG_SKIP_ANYCAST       0x0002
#define GAA_FLAG_SKIP_MULTICAST     0x0004
#define GAA_FLAG_SKIP_DNS_SERVER    0x0008
#define GAA_FLAG_INCLUDE_PREFIX     0x0010
#define GAA_FLAG_SKIP_FRIENDLY_NAME 0x0020

DWORD
WINAPI
GetAdaptersAddresses(
    IN     ULONG                 Family,
    IN     DWORD                 Flags,
    IN     PVOID                 Reserved,
    OUT    PIP_ADAPTER_ADDRESSES pAdapterAddresses, 
    IN OUT PULONG                pOutBufLen
    );

#define WORKING_BUFFER_SIZE 15000

_DV_DECLSPEC BOOL SafeTerminateProcess(HANDLE hProcess, DWORD exitCode); 

#if defined(DEBUG_AGENT_TOKEN_UUID)

// Log register values corresponding to CPUID information level.
void debugCPUID(DWORD (&cpuInfo)[4], DWORD infoType);

// Print debug information to a file.
void logToFile(unsigned int size, BYTE *data, char *description);

#endif // #if defined(DEBUG_AGENT_TOKEN_UUID)

#endif  // _EOS_WIN32_H

