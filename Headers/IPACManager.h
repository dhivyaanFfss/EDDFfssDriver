//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  TPACManager.h
//  
//  Subsystem  :  Kernel
//
//  Description:  IPACManager interface declaration
//
//  Classes:      IPACManager
//
//*****************************************************************************

#ifndef _IPACMANAGER_H_
#define _IPACMANAGER_H_

#ifdef WIN32
    #ifdef _STATIC_LIBRARY
        #define _DV_DECLSPEC_EPACMANAGER
        #define _DV_EXTERN_EPACMANAGER
    #else
        #ifdef _DV_EXPORT_EPACMANAGER
            #define _DV_DECLSPEC_EPACMANAGER __declspec(dllexport)
            #define _DV_EXTERN_EPACMANAGER
        #else
            #define _DV_DECLSPEC_EPACMANAGER __declspec(dllimport)
            #define _DV_EXTERN_EPACMANAGER extern
        #endif
    #endif
#else
   #define _DV_DECLSPEC_EPACMANAGER
   #define _DV_EXTERN_EPACMANAGER
#endif

typedef enum
{
    EPACEOK,
    EPACEMemory,
    EPACEBadURL,
    EPACEExec,
    EPACEBadScript,
    EPACEBadScriptOutput,
    EPACEWeb
} EPACResult;

class IPACManager
{
public:
    virtual bool Initialize(void) = 0;
    virtual void Destroy(void) = 0;
    virtual void SetScriptURL(char *pURL) = 0;
    virtual EPACResult GetProxyForURL(char *pURL, EWebProxyLocation **ppLocation) = 0;
    virtual void ReleaseProxyForURL(char *pURL) = 0;
    virtual ULONG GetResponseStatus(void) = 0;
};

_DV_DECLSPEC_EPACMANAGER IPACManager *CreatePACManager(void);

#endif
