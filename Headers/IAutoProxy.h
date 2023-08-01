//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IAutoProxy.h
//
//  Subsystem  :  AutoProxy detection
//
//  Description:  Provides an interface to allow all other components to get 
//                  optimal parameters of EWebClient
//
//  Interfaces:
//			      IAutoProxyInterface
//
//**************************************************************************

#ifndef __AUTOPROXYINTERFACE_H__
#define __AUTOPROXYINTERFACE_H__

#define DEFAULT_SSL_CA_CERT_PATH     _T("Certs")
#define DEFAULT_SSL_CA_CERT_FILE     _T("SSLCACert.pem")
#define DEFAULT_SSL_CERT_FILE        _T("SSLCert.pem")
#define DEFAULT_SSL_PRIVATE_KEY_FILE _T("SSLPrivKey.pem")


//**************************************************************************
//**************************************************************************
#ifndef WEBCLIENT_PARAM_STRUCT
#define WEBCLIENT_PARAM_STRUCT
struct  ProxyServerParam
{
public:
    ProxyServerParam()
    {
        pName = NULL;
        iPort = -1;
        bAuthEnable = false;
        pUserName = NULL;
        pPasswd = NULL;
    };
    ~ProxyServerParam()
    {
        if(pName) { free(pName); pName = NULL; }
        if(pUserName) { free(pUserName); pUserName = NULL; }
        if(pPasswd) { free(pPasswd); pPasswd = NULL; }
    };

public:
    char*   pName;
    int     iPort;
    bool    bAuthEnable;
    char*   pUserName;
    char*   pPasswd;
};

//**************************************************************************
struct HTTPServerParam
{
public: 
    HTTPServerParam() 
	{ 
		pName = NULL; 
		pServerPath = NULL; 
		iPort = -1; 
		iEncryptionLevel = EWebCryptoNone;
	}
    
	~HTTPServerParam() 
	{ 
		if(pName) { free(pName); pName = NULL; } 
		if(pServerPath) { free(pServerPath); pServerPath = NULL; } 
	};

    HTTPServerParam& operator=(HTTPServerParam& s)
    {
        pName = strdup(s.pName);
		pServerPath = s.pServerPath ? strdup(s.pServerPath) : NULL;
		iPort = s.iPort;
        iEncryptionLevel = s.iEncryptionLevel;
        // Proxy
        proxyParam.iPort = s.proxyParam.iPort;
        if(s.proxyParam.pName)
            proxyParam.pName = strdup(s.proxyParam.pName);
        if(s.proxyParam.pUserName)
            proxyParam.pUserName = strdup(s.proxyParam.pUserName);
        if(s.proxyParam.pPasswd)
            proxyParam.pPasswd = strdup(s.proxyParam.pPasswd);
        proxyParam.bAuthEnable = s.proxyParam.bAuthEnable;

        // SOCKS
        socksParam.iPort = s.socksParam.iPort;
        if(s.socksParam.pName)
            socksParam.pName = strdup(s.socksParam.pName);
        if(s.socksParam.pUserName)
            socksParam.pUserName = strdup(s.socksParam.pUserName);
        if(s.socksParam.pPasswd)
            socksParam.pPasswd = strdup(s.socksParam.pPasswd);
        socksParam.bAuthEnable = s.socksParam.bAuthEnable;

        return *this;
    };

	void setServerPath(char *pPath)
	{
		pServerPath = (pPath)? strdup(pPath) : NULL;
	};

public:
    char*   pName;
	char*	pServerPath;
    int     iPort;
    int     iEncryptionLevel;
    ProxyServerParam proxyParam;
    ProxyServerParam socksParam;
};

//**************************************************************************
//**************************************************************************
struct HTTPConnectionParams
{
public:
    int iPersistent;        // 0 - disable(default), 1 - enable, 2 - strict;
    int iChunkedEncoding;   // 0 - disable(default), 1 - enable;
    int iSentinel;          // MUST be -1

public:
    HTTPConnectionParams() { iPersistent = 0; iChunkedEncoding = 0; iSentinel = -1; };
    ~HTTPConnectionParams() { ; };
};

//**************************************************************************
struct WebClientParam
{
public:
    int iHTTPVersion;                   // 0 - HTTP/1.0, 1 - HTTP/1.1(default)
    time_t timeStamp;
    HTTPConnectionParams connectionParams;
    char** ppGetHTTPHeaders;            // The laset one MUST be NULL
    char** ppPostHTTPHeaders;           // The laset one MUST be NULL

public:
    WebClientParam() 
    {
        iHTTPVersion = 1;       // we try to use HTTP/1.1 by default
        timeStamp = time(NULL);

        // PreAllocate space for headers
        ppGetHTTPHeaders = new char*[100];
        memset(ppGetHTTPHeaders, 0, 100*sizeof(char*));
        ppPostHTTPHeaders = new char*[100];
        memset(ppPostHTTPHeaders, 0, 100*sizeof(char*));
    };
    ~WebClientParam()
    {
        char** pTmp = ppGetHTTPHeaders;
        while(pTmp && *pTmp) { delete *pTmp; pTmp++; }
        if(ppGetHTTPHeaders) { delete ppGetHTTPHeaders; ppGetHTTPHeaders = NULL; }
        pTmp = ppPostHTTPHeaders;
        while(pTmp && *pTmp) { delete *pTmp; pTmp++; }
        if(ppPostHTTPHeaders) { delete ppPostHTTPHeaders; ppPostHTTPHeaders = NULL; }
    };

    void AddGetHeader(char* pHeader, char* pValue)
    {
        if(ppGetHTTPHeaders)
        {
            char** pTmp = ppGetHTTPHeaders;
            while(*pTmp)
                pTmp++;
            *pTmp = strdup(pHeader);
            pTmp++;
            *pTmp = strdup(pValue);
        }
    };
    void AddPostHeader(char* pHeader, char* pValue)
    {
        if(ppPostHTTPHeaders)
        {
            char** pTmp = ppPostHTTPHeaders;
            while(*pTmp)
                pTmp++;
            *pTmp = strdup(pHeader);
            pTmp++;
            *pTmp = strdup(pValue);
        }
    };
};

//**************************************************************************
struct ConnectionCache
{
public:
    HTTPServerParam* pConnectionParam;
    WebClientParam* pWebClientParam;

public:
    ConnectionCache();
    ~ConnectionCache();
};

#endif

interface   IAutoProxyInterface
{
public:
    virtual KERESULT    GetWebClientParam(HTTPServerParam* pInParam, WebClientParam*& pOutParam) = 0;
};


#endif  //  __RCONINTERFACE_H__
