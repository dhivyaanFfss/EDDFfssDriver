//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EWebClient.h
//  
//  Subsystem  :  Kernel
//
//  Description:  IWebUserAgent interface and EWebRequest class definitions 
//
//  Classes:       
//              IWebUserAgent
//              EWebRequest                                  
//
//*****************************************************************************

#ifndef _EWEBCLIENT_H_
#define _EWEBCLIENT_H_

#include "EWebClientConfig.h"
#include "ENetwork.h"

#ifdef WIN32
    #ifdef _STATIC_LIBRARY
        #define _DV_DECLSPEC_EWEBCLIENT
        #define _DV_EXTERN_EWEBCLIENT
    #else
        #ifdef _DV_EXPORT_EWEBCLIENT
            #define _DV_DECLSPEC_EWEBCLIENT __declspec(dllexport)
            #define _DV_EXTERN_EWEBCLIENT
        #else
            #define _DV_DECLSPEC_EWEBCLIENT __declspec(dllimport)
            #define _DV_EXTERN_EWEBCLIENT extern
        #endif
    #endif
#else
   #define _DV_DECLSPEC_EWEBCLIENT
   #define _DV_EXTERN_EWEBCLIENT
#endif

#define MIME_DEFAULT        "application/octet-stream"

#define HTTP_VERSION_10     "1.0"
#define HTTP_VERSION_11     "1.1"

#define HTTP_DEFAULT_PORT   80
#define HTTPS_DEFAULT_PORT  443

#define HTTP_METHOD_OPTIONS "OPTIONS"
#define HTTP_METHOD_GET     "GET"
#define HTTP_METHOD_HEAD    "HEAD"
#define HTTP_METHOD_POST    "POST"
#define HTTP_METHOD_PUT     "PUT"
#define HTTP_METHOD_DELETE  "DELETE"
#define HTTP_METHOD_CONNECT "CONNECT"

#define HTTP_STATUS_CONTINUE        100
#define HTTP_STATUS_OK              200
#define HTTP_STATUS_MOVED_PERM      301
#define HTTP_STATUS_FOUND           302
#define HTTP_STATUS_SEE_OTHER       303
#define HTTP_STATUS_UNAUTHORIZED    401
#define HTTP_STATUS_PROXYAUTHREQ    407
#define HTTP_STATUS_REQUEST_TIMEOUT 408
#define HTTP_SERVICE_UNAVAILABLE    503 

typedef enum
{
    EWebEOK,
    EWebEInternal,
    EWebEMemory,
    EWebENotFound,
    EWebETimeout,
    EWebEUnknownHost,
    EWebEConnLost,
    EWebESockGeneral,
    EWebESockWouldBlock,
    EWebESockConnRefused,
    EWebESockConnReset,
    EWebESockConnAborted,
    EWebESockNotConn,
    EWebESOCKSWrongVersion,
    EWebESOCKSAuthFailed,
    EWebESOCKSGeneral,                                  // 1: general SOCKS server failure
    EWebESOCKSPerm,                                     // 2: connection not allowed by ruleset
    EWebESOCKSNetUnreach,                               // 3: network unreachable
    EWebESOCKSHostUnreach,                              // 4: host unreachable
    EWebESOCKSConnRefused,                              // 5: connection refused
    EWebESOCKSTTL,                                      // 6: TTL expired
    EWebESOCKSBadCommand,                               // 7: command not supported
    EWebESOCKSBadAddress,                               // 8: address type not supported
    EWebEHTTPBadResponse,
    EWebEHTTPAuthFailed,
    EWebESSLGeneral,
    EWebESSLCertVerifyFailed,
    EWebESSLCertIssuerUnknown,
    EWebESSLCertInvalid,
    EWebESSLHandshakeFailed,
    EWebEBadURL,
    EWebESockNetUnreach,
    EWebESockHostUnreach,
    EWebESSLInit,
    EWebESSLCACertLoad,
    EWebESSLCertLoad,
    EWebESSLPrivKeyLoad,
    EWebESSLCipherSuitesInvalid
} EWebResult;

typedef enum
{
    EWebWSSLWeakerCipher
} EWebWarning;

typedef enum
{
    EWebProtoUnknown,
    EWebProtoHTTP
} EWebProtocol;

typedef enum
{
    EWebProxyProtoUnknown,
    EWebProxyProtoSOCKS,
    EWebProxyProtoHTTP,
    EWebProxyProtoDirect
} EWebProxyProtocol;

typedef enum
{
    EWebCryptoNone = 0,
    EWebCryptoLow = 1,
    EWebCryptoMedium = 2,
    EWebCryptoHigh = 3,
} EWebCryptoLevel;

#ifdef ENABLE_STATISTICS
typedef struct
{
    int m_iPostCount;
    int m_iGetCount;
    int m_iPostResponseChunkCount;
    int m_iGetResponseChunkCount;
} EWebStatistics;
#endif

#ifdef WIN32
    typedef SOCKET EWebSocketFD;
#else
    typedef int EWebSocketFD;
#endif

class EWebRequest;
class EWebSession;
class EWebProxyLocation;

class _DV_DECLSPEC_EWEBCLIENT IWebUserAgent
{
public:
    virtual EWebResult SyncExecute(EWebRequest **ppRequests,
        ULONG iCount) = 0;
    virtual EWebResult AsyncExecute(EWebRequest **ppRequests,
        ULONG iCount) = 0;
    virtual EWebResult Cancel(EWebRequest *pRequest) = 0;
    virtual EWebResult DoEvents(bool bBlocking) = 0;
    virtual bool EventsPending(void) = 0;
    virtual EWebResult Destroy(void) = 0;

    virtual EWebResult SetProxyInfo(EWebProxyProtocol iProto,
        const char *pszHost,
        USHORT iPort,
        const char *pszUser,
        const char *pszPassword) = 0;
    virtual EWebResult SetSSLInfo(EWebCryptoLevel iLevel,
        bool bServerAuth,
        const char *pszCACertFile,
        const char *pszCACertPath,
        const char *pszCertFile,
        const char *pszPrivateKeyFile) = 0;
    virtual EWebResult SetTimeout(long iSeconds,
        long iUSeconds) = 0;
    virtual EWebResult GetTimeout(long *piSeconds,
        long *piUSeconds) = 0;
    virtual EWebResult SetAsyncTimeout(long iSeconds,
        long iUSeconds) = 0;
    virtual EWebResult GetAsyncTimeout(long *piSeconds,
        long *piUSeconds) = 0;

    virtual void SetHandleCookies(bool bHandleCookies) = 0;
    virtual bool MustHandleCookies(void) = 0;
    virtual void SetHandleRedirects(bool bHandleRedirects) = 0;
    virtual bool MustHandleRedirects(void) = 0;

#ifdef ENABLE_STATISTICS
    virtual EWebStatistics *GetStatistics(void) = 0;
#endif

    virtual void SetSSLPrivateKeyPassword(const char *pszPassword) = 0;

    virtual EWebResult SetSSLContext(void *pContext) = 0;
    virtual EWebResult GetSSLContext(void **ppContext) = 0;

    virtual EWebResult SetSSLInfo2(const char *pszCipherSuites,
        bool bServerAuth,
        const char *pszCACertFile,
        const char *pszCACertPath,
        const char *pszCertFile,
        const char *pszPrivateKeyFile) = 0;

    static char *GetResultString(EWebResult iResult);
    static char *GetWarningString(EWebWarning iWarning);
    static char *GetDefaultCipherSuites(EWebCryptoLevel iLevel);
};

_DV_DECLSPEC_EWEBCLIENT IWebUserAgent *CreateWebUserAgent(void);

class EWebURL;

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebRequest
{
public:
    EWebRequest();
    virtual ~EWebRequest();

    void ClearStatus(void);

    EWebResult SetProxy(EWebProxyLocation *pProxy);
    EWebProxyLocation *GetProxy(void);

    EWebResult SetSession(EWebSession *pSession);
    EWebSession *GetSession(void);

    EWebResult SetURL(const char *pszURL);
    char *GetURL(void);

    EWebResult SetProtocol(EWebProtocol iProto);
    EWebProtocol GetProtocol(void);

    EWebResult SetVersion(const char *pszVersion);
    char *GetVersion(void);

    EWebResult SetMethod(const char *pszMethod);
    char *GetMethod(void);

    EWebResult SetSSLNeed(bool bNeedSSL);
    bool GetSSLNeed(void);

    EWebResult SetConnectionPersistence(bool bPersistentConnection);
    bool GetConnectionPersistence(void);

    EWebResult SetOriginHost(const char *pszHost);
    char *GetOriginHost(void);

    EWebResult SetOriginPort(USHORT iPort);
    USHORT GetOriginPort(void);

    EWebResult SetAbsPath(const char *pszAbsPath);
    char *GetAbsPath(void);

    EWebResult SetOriginUser(const char *pszUser);
    char *GetOriginUser(void);

    EWebResult SetOriginPassword(const char *pszPassword);
    char *GetOriginPassword(void);

    EWebResult SetRequestHeader(const char *pszName, const char *pszValue);
    char *GetRequestHeader(const char *pszName);
    void RemoveRequestHeader(const char *pszName);
    void *GetRequestHeaders(void);

    EWebResult SetResponseHeader(const char *pszName, const char *pszValue);
    char *GetResponseHeader(const char *pszName);
    void *GetResponseHeaders(void);
    void *GetFirstResponseHeader(char **ppszName, char **ppszValue);
    void *GetNextResponseHeader(void *pPosition, char **ppszName, char **ppszValue);

    EWebResult SetRequestContentType(const char *pszContentType);
    char *GetRequestContentType(void);

    EWebResult SetRequestEntityData(const char *pData);
    char *GetRequestEntityData(void);

    EWebResult SetRequestEntitySize(ULONG iSize);
    ULONG GetRequestEntitySize(void);

    EWebResult SetRequestMD5(bool bNeedMD5);
    bool GetRequestMD5(void);

    EWebResult SetStrictMode(bool bStrict);
    bool GetStrictMode(void);

    EWebResult SetUserData(const void *pUserData);
    void *GetUserData(void);

    void SetResponseStatus(ULONG iStatusCode);
    ULONG GetResponseStatus(void);

    EWebResult SetResponseMD5(bool bMD5OK);
    bool GetResponseMD5(void);

    void SetLastError(EWebResult iError);
    EWebResult GetLastError(void);

    EWebResult SetURL(EWebURL *pURL);
    EWebResult SetRelativePath(const char *pszRelativePath);

    void SetOpenTunnelAndQuit(bool bOpenTunnelAndQuit);
    bool GetOpenTunnelAndQuit(void);

    void SetLastSSLError(ULONG iSSLError);
    ULONG GetLastSSLError(void);

    char *GetLastErrorMessage(char *pszErrorMessage, size_t iLength);

    virtual void OnError(EWebResult iResult) { return; }
    virtual void OnWarning(EWebWarning iWarning) { return; }
    virtual bool OnResponse(ULONG iStatusCode) { return true; }
    virtual bool OnResponseEntity(ULONG iDataOffset,
        const char *pData,
        int iSize) { return true; }
    virtual void OnCompleted(void) { return; }
    virtual void OnLog(const char *pszMessage) { return; }
    virtual void OnTunnelOpen(ENetwork::StreamSocket& sock) { return; }

private:
    EWebProxyLocation *m_pProxy;
    EWebSession *m_pSession;
    void *m_pURL;
    EWebProtocol m_iProto;
    char *m_pszVersion;
    char *m_pszMethod;
    bool m_bNeedSSL;
    bool m_bPersistentConnection;
    char *m_pszHost;
    USHORT m_iPort;
    char *m_pszAbsPath;
    char *m_pszUser;
    char *m_pszPassword;
    void *m_pRequestHeaders;
    void *m_pResponseHeaders;
    char *m_pszRequestContentType;
    char *m_pRequestEntityData;
    ULONG m_iRequestEntitySize;
    bool m_bRequestMD5;
    bool m_bStrict;
    bool m_bOpenTunnelAndQuit;

    void *m_pUserData;
    ULONG m_iResponseStatus;
    bool m_bResponseMD5;
    EWebResult m_iError;
    ULONG m_iSSLError;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebProxyLocation
{
public:
    EWebProxyLocation(EWebProxyProtocol iProto, char *pszHost, USHORT iPort);
    ~EWebProxyLocation();

    EWebProxyProtocol GetProtocol(void);
    char *GetHost(void);
    USHORT GetPort(void);

    void ReportError(void);
    void ResetErrors(void);
    bool IsOnline(void);

private:
    EWebProxyProtocol m_iProto;
    char *m_pszHost;
    USHORT m_iPort;
    int m_iErrorCount;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebMD
{
public:
    EWebMD(void);
    virtual ~EWebMD();

    bool Init(void);
    void Update(const void *pData, size_t iLength);
    void Final(unsigned char *pMD);

protected:
    virtual const void *GetAlgorithm(void) = 0;

    void *m_pContext;
    bool m_bInitialized;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebMD4 : public EWebMD
{
public:
    EWebMD4(void);

protected:
    const void *GetAlgorithm(void);
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebMD5 : public EWebMD
{
public:
    EWebMD5(void);

protected:
    const void *GetAlgorithm(void);
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebSession
{
public:
    EWebSession();
    ~EWebSession();

    void SetResponseMD5(bool bNeedMD5);
    bool GetResponseMD5();
    void ResetResponseMD5();
    bool CheckResponseMD5(char *pszMD5);

    void OnResponse(EWebRequest *pRequest,
        ULONG iStatusCode);
    void OnResponseEntity(EWebRequest *pRequest,
        ULONG iDataOffset,
        const char *pData,
        int iSize);

private:
    bool m_bResponseMD5;
    EWebMD5 m_md5;
};

/******************************************************************************/
/* Miscellaneous utility classes                                              */
/******************************************************************************/
class EWebBuffer;
class _DV_DECLSPEC_EWEBCLIENT EWebUtil
{
public:
    static EWebResult SetString(char **ppString,
        const char *pValue,
        int iLength = -1);
    static int CIStringCompare(const char *pszString1,
        const char *pszString2);
    static int CINStringCompare(const char *pszString1,
        const char *pszString2,
        size_t iLength);
    static void BinToHex(UCHAR *pBin,
        size_t iLength,
        char *pszHex);
    static int Base64Encode(char *pInput,
        size_t iLength,
        char *pOutput);
    static int Base64Decode(char *pInput,
        size_t iLength,
        char *pOutput);
    static void URLEncode(char *pInput,
        size_t iLength,
        EWebBuffer *pOutput);
    static void GetTimeOfDay(struct timeval *pTimeVal);
    static char *GetToken(char *pSrc,
        int iMaxLength,
        char *pSep,
        int *piLength);
    static char *GetHTTPReasonPhrase(ULONG iStatus);
    // Accepts char * of URL input and redacts the relevent key value and sets it to **refacted.
    // Additionally the function allocates memory and the caller is expected to call `delete []` on the returned pointer to free the memory after usage
    static bool RedactKey(char *pInput, char **redacted);
    // utility function for use inside of eWebClient to securly zero out memory of sensative variables including passwords	
    static void memzero(void * block, size_t size);
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebLock
{
public:
    EWebLock(void);
    ~EWebLock();

    EWebResult Lock(void);
    EWebResult Unlock(void);

private:
#ifdef WIN32
    HANDLE m_Mutex;
#endif
#ifdef UNIX
    pthread_mutex_t m_Mutex;
#endif
};

/******************************************************************************/
#define EWEBBUFFER_CHUNK 4096

class _DV_DECLSPEC_EWEBCLIENT EWebBuffer
{
public:
    EWebBuffer(const char *pszContent = NULL,
        int iSize = 0);
    ~EWebBuffer();

    EWebResult Write(const char *pData,
        int iSize);
    EWebResult WriteLn(const char *pData,
        int iSize);
    EWebResult Read(char **ppData,
        int *piSize);
    EWebResult ReadLn(char **ppData,
        int *piSize);
    EWebResult SeekWritePos(int iOffset,
        bool bIsRelative);
    EWebResult SeekReadPos(int iOffset,
        bool bIsRelative);
    EWebResult GetReadBuffer(char **ppData,
        int *piSize);
    EWebResult GetWriteBuffer(char **ppData,
        int *piSize);
    EWebResult Expand(int iSize);
    EWebResult Reset(void);
    void SetNullTerminate(bool bNullTerminate);

private:
    char *m_pData;
    int m_iSize;
    int m_iWritePos;
    int m_iReadPos;
    bool m_bExternalData;
    bool m_bNullTerminate;
};

/******************************************************************************/
typedef struct _EWebListItem EWebListItem;
typedef int (*EWebUtilCompareFunc)(const void *pData1, const void *pData2);
typedef void (*EWebUtilReleaseFunc)(void *pObject);

struct _EWebListItem
{
    void *pData;
    EWebListItem *pNext;
    EWebListItem *pPrev;
};

int _DV_DECLSPEC_EWEBCLIENT PointerCompare(const void *pData1, const void *pData2);

class _DV_DECLSPEC_EWEBCLIENT EWebList
{
public:
    EWebList(EWebUtilCompareFunc pCompareFunc = (EWebUtilCompareFunc) PointerCompare,
        EWebUtilReleaseFunc pReleaseFunc = (EWebUtilReleaseFunc) NULL);
    ~EWebList();

    EWebResult Append(const void *pData);
    EWebResult Prepend(const void *pData);
    EWebResult RemoveKey(const void *pData);
    EWebResult RemoveItem(EWebListItem *pItem);
    EWebResult Free(void);

    EWebListItem *Find(const void *pData);
    EWebListItem *First(void);
    EWebListItem *Last(void);
    ULONG Count(void);

    EWebResult InsertBefore(EWebListItem *pItem, const void *pData);
    EWebResult InsertAfter(EWebListItem *pItem, const void *pData);

private:
    EWebUtilCompareFunc m_pCompareFunc;
    EWebUtilReleaseFunc m_pReleaseFunc;
    EWebListItem *m_pFirstItem;
    EWebListItem *m_pLastItem;
    ULONG m_iCount;
};

/******************************************************************************/
typedef struct _EWebHashNode EWebHashNode;
typedef int (*EWebUtilHashFunc)(const void *pKey);

struct _EWebHashNode
{
    void *pKey;
    void *pData;
    EWebHashNode *pNext;
};

int _DV_DECLSPEC_EWEBCLIENT PointerHashFunc(const void *pKey);
int _DV_DECLSPEC_EWEBCLIENT StringHashFunc(const void *pKey);

class _DV_DECLSPEC_EWEBCLIENT EWebHashTable
{
public:
    EWebHashTable(EWebUtilHashFunc pHashFunc = (EWebUtilHashFunc) PointerHashFunc,
        EWebUtilCompareFunc pCompareFunc = (EWebUtilCompareFunc) PointerCompare,
        EWebUtilReleaseFunc pReleaseFunc = (EWebUtilReleaseFunc) NULL,
        int iSize = -1);
    ~EWebHashTable();

    EWebResult Initialize(void);
    EWebResult Set(const void *pKey, const void *pData);
    EWebResult Remove(const void *pKey);
    EWebResult Free(void);

    void *Find(const void *pKey);
    ULONG Count(void);
    void Traverse(bool (*pActionFunc)(void *pData));

private:
    EWebHashNode **Lookup(const void *pKey);

    EWebUtilHashFunc m_pHashFunc;
    EWebUtilCompareFunc m_pCompareFunc;
    EWebUtilReleaseFunc m_pReleaseFunc;
    EWebHashNode **m_ppNodes;
    ULONG m_iSize;
    ULONG m_iCount;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebHeader
{
public:
    EWebHeader(bool bTransient = true) :
        pszName(NULL),
        pszValue(NULL),
        m_bTransient(bTransient) { }
    ~EWebHeader()
    {
        if (!m_bTransient)
        {
            if (pszName)
                delete [] pszName;
            if (pszValue)
                delete [] pszValue;
        }
    }

    char *pszName;
    char *pszValue;

private:
    bool m_bTransient;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebHeaders
{
public:
    EWebHeaders();
    ~EWebHeaders();

    EWebResult Set(const char *pszName,
        const char *pszValue);
    char *Get(const char *pszName);
    EWebResult Remove(const char *pszName);
    EWebResult RemoveAll(void);
    EWebResult Write(EWebBuffer *pBuffer);
    void *GetFirst(char **ppszName,
        char **ppszValue);
    void *GetNext(void *pPosition,
        char **ppszName,
        char **ppszValue);
private:
    EWebHeader *Lookup(const char *pszName);

    EWebList m_Headers;
};

#if defined(WIN32) || defined(_WINDOWS)
// <winspool.h> defines SetPort
#ifdef SetPort
#undef SetPort
#endif
#endif

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebCGIParam
{
public:
    EWebCGIParam(bool bTransient = true) :
        pszName(NULL),
        pszValue(NULL),
        m_bTransient(bTransient) { }
    ~EWebCGIParam()
    {
        if (!m_bTransient)
        {
            if (pszName)
                delete [] pszName;
            if (pszValue)
                delete [] pszValue;
        }
    }

    char *pszName;
    char *pszValue;

private:
    bool m_bTransient;
};

#if defined(WIN32) || defined(_WINDOWS)
// <winspool.h> defines SetPort
#ifdef SetPort
#undef SetPort
#endif
#endif

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebURL
{
public:
    EWebURL();
    ~EWebURL();

    EWebResult SetURL(const char *pszURL);
    char *GetURL(void);

    EWebResult SetProtocol(EWebProtocol iProto);
    EWebProtocol GetProtocol(void);

    EWebResult SetSSLNeed(bool bNeedSSL);
    bool GetSSLNeed(void);

    EWebResult SetHost(const char *pszHost);
    char *GetHost(void);

    EWebResult SetPort(USHORT iPort);
    USHORT GetPort(void);

    EWebResult SetAbsPath(const char *pszAbsPath);
    char *GetAbsPath(void);

    EWebResult SetUser(const char *pszUser);
    char *GetUser(void);

    EWebResult SetPassword(const char *pszPassword);
    char *GetPassword(void);

    EWebResult SetAbsPathBase(const char *pszAbsPathBase);
    char *GetAbsPathBase(void);

    EWebResult SetCGIParam(const char *pszName, const char *pszValue);
    EWebResult SetCGIParam(EWebCGIParam *pParam);
    char *GetCGIParam(const char *pszName);

private:
    EWebProtocol m_iProto;
    bool m_bNeedSSL;
    char *m_pszHost;
    USHORT m_iPort;
    char *m_pszAbsPath;
    char *m_pszUser;
    char *m_pszPassword;
    char *m_pszAbsPathBase;
    EWebList m_cgiParams;
    EWebBuffer m_URLBuffer;
};

#if (defined(WIN32) || defined(_WINDOWS)) && !defined(UNDER_CE)
/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebWindowsAccessToken
{
public:
    EWebWindowsAccessToken(void);
    ~EWebWindowsAccessToken();

    bool Acquire(HANDLE hProcess);
    bool IsValid(void) const;
    bool IsInteractive(void) const;
    bool IsLoggedOn(void) const;
    HANDLE GetToken(void) const;

private:
    HANDLE m_hToken;
    bool m_bInteractive;
    bool m_bLoggedOn;
};

/******************************************************************************/
class _DV_DECLSPEC_EWEBCLIENT EWebCurrentWindowsUser
{
public:
    EWebCurrentWindowsUser(void);
    ~EWebCurrentWindowsUser();

    bool Initialize(void);
    bool BeginImpersonation(void);
    void EndImpersonation(void);

private:
    bool CheckCurrentProcess(void);
    bool FindInteractiveSession(void);

    EWebWindowsAccessToken *m_pToken;
    bool m_bImpersonationNeeded;
    bool m_bImpersonated;
};
#endif

#endif
