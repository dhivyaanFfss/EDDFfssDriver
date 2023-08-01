//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IWebProvider.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface to Web Server
//
//  Classes:      IWebProvider
//
//**************************************************************************
#ifndef _IWEBPROVIDER_H_
#define _IWEBPROVIDER_H_

/* Group=Web Server defines */

// Security Level
//
typedef enum {
    HTTP_SECURE_NONE,       // no security
    HTTP_SECURE_AUTH,       // authenticate users in groups
    HTTP_SECURE_CRYPT,      // auth, plus encrypt body text in Posts and responses
    HTTP_SECURE_SOCKET,     // auth, plus socket level encryption
    HTTP_SECURE_PARANOID    // (not implemented) socket encryption of encrypted data (diff keys!!)
}
HTTP_Security;

typedef enum {
    HTTP_GET,           // get a url
    HTTP_POST,          // post data
    HTTP_POST_DONE,     // post is done with data, regardless of our state
    HTTP_HEAD,
    HTTP_GET_DONE,      // get is done with last response
    HTTP_RESPONSE,      // response to our post
    HTTP_INDEX
} HTTP_Operation;

typedef enum {
    HTTP_Pending,
    HTTP_Partial,
    HTTP_Complete,
    HTTP_ConnectionClose
} HTTP_OperationState;

#define HTTP_PORT 80

#define PCB_TAG 0xbeef


typedef KERESULT (*PostCallback)(
                                 BYTE*    pResponseBody,
                                 long     respLen,
                                 bool     isLast,
                                 KERESULT err,
                                 void*    pUserData
                                );


extern "C" {
/* Group=Web Server struct */
    typedef struct tag_post_cb {
        long   tag;                 // sanity check tag
        PostCallback
               pCallback;           // who to hand the reply to
        char*  url;                 // where to post, like: axeda.com/post/dests
        short  port;                // port to post to
        BYTE*  data;                // data to post
        long   dataLen;             // length of data to post
        BYTE*  response;            // response buffer
        long*  respLen;             // length of response buffer each way
        bool   respDone;            // true for last bufferload of response
        bool   blocking;            // true for a blocking post (no chunking of data)
        bool   urlAlloced;          // true if url was allocated by web client
        bool   dataAlloced;         // true if data was allocated by web client
        bool   respAlloced;         // true if response was allocated by web client
        void*  pUserData;           // cookie
        bool   signalEvent;         // true if pUserData is an Event to signal on
        KERESULT* errcode;          // error code from actual operation
        DWORD     exterr;           // extended error information
    } 
    PostControlBlock;// {secret}
}


/* Group=Web Server Interfaces*/

//**************************************************************************
interface IWebServer
{
public:
    virtual KERESULT GetRootPath(EString& path) const = 0;
    virtual KERESULT SetRootPath(EString& path) = 0;
    virtual KERESULT GetHTTPport(int& port) const = 0;
    virtual KERESULT SetHTTPport(int& port) = 0;
    virtual KERESULT GetMaxFiles(int& nFiles) const = 0; 
    virtual KERESULT SetMaxFiles(int& nFiles) = 0; 
    virtual KERESULT GetSecurityLevel(int& Secure) const = 0;
    virtual KERESULT SetSecurityLevel(int& Secure) = 0;
    virtual KERESULT GetGroups(EString& groups) const = 0;
    virtual KERESULT SetGroups(EString& groups) = 0;
};

#if 0
// not used any more -- HF
interface IWebClient
{
    virtual KERESULT Post(
                            TCHAR*       pAddress,   // IP Address or URL
                            int          iPort,      // Port Number
                            BYTE*        pData,      // Buffer to send
                            long         dataLen,    // Length of data to send
                            long         respLen,    // max Length of response
                            void*        pUserData,  // anything the caller wants
                            PostCallback pCallback   // Callback function
                          ) = 0;

    virtual KERESULT PostBlocking(
                            TCHAR*       pAddress,   // IP Address or URL
                            int          iPort,      // Port Number
                            BYTE*        pData,      // Send Buffer 
                            long         dataLen,    // Length of send data
                            BYTE*        pReply,     // Reply Buffer 
                            long*        replyLen    // [IN]  len of reply buffer
                                                     // [OUT] len of response data in reply buffer 
                          ) = 0;

    virtual KERESULT FlushBuffer(void) = 0;

};
#endif

/* Group=Web Server */

// Support class for CGI info from the WebServer.
class CgiInfo
{
public:
    CgiInfo(int cn, bool bIsClientRemote, char* pszHost) 
        : m_connectionNumber(cn), m_bIsClientRemote(bIsClientRemote)
    {
        if (pszHost)
        {
            CONVERSION_HELPER;
            m_esHost = AtoT(pszHost);
        }
    }
    // default constructor:
    CgiInfo()
    {
        m_connectionNumber = -1;
        m_bIsClientRemote = false;
    }
    // copy constructor:
    CgiInfo(CgiInfo &i) 
        : m_bIsClientRemote(i.m_bIsClientRemote), 
          m_connectionNumber(i.m_connectionNumber),
          m_esHost(i.m_esHost)
    {
    }
    CgiInfo & operator =(CgiInfo &i) 
    {
        this->m_bIsClientRemote = i.m_bIsClientRemote;
        this->m_connectionNumber = i.m_connectionNumber;
        this->m_esHost = i.m_esHost;
        return *this;
    }
    bool IsClientRemote()
    {
        return m_bIsClientRemote;
    }
    int GetConnectionNumber()
    {
        return m_connectionNumber;
    }
    EString& GetHost()
    {
        return m_esHost;
    }
private:
    bool        m_bIsClientRemote;
    int         m_connectionNumber;
    EString     m_esHost;       // localhost or IP address.
};

#endif // _IWEBPROVIDER_H_
