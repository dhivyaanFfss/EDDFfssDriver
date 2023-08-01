//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EMailInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define e-mail provider interface
//
//  Classes:    
//              IEmailProvider
//
//**************************************************************************

#ifndef __EMAIL_INTERFACE_H__
#define __EMAIL_INTERFACE_H__ 1

/* Group=E-mail */

#ifdef WIN32
    #ifdef _EMAIL_EXPORT
       #define _EMAIL_DECLSPEC __declspec(dllexport)
       #define _EMAIL_EXTERN
    #else
       #define _EMAIL_DECLSPEC __declspec(dllimport)
       #define _EMAIL_EXTERN   extern
    #endif
#else
   #define _EMAIL_DECLSPEC
   #define _EMAIL_EXTERN
#endif

static const TCHAR* EEMAIL_STYLE_NAME = _T("EEmailStyle");

#include "PersistenceInterface.h"
#include "EPersistenceTypes.h"
#include "Transport.h"
#include "EDynamicLibrary.h"

#define ATTACHFILE_SEPARATOR    _T(';')

class _EMAIL_DECLSPEC EEmailStyle : public IPersistent
{
public:
	EEmailStyle();
    virtual ~EEmailStyle() {};
public:
    // IPersistent interface
    //
    virtual KERESULT Restore(EHANDLE pData);
    virtual KERESULT Save(EHANDLE pData);
    virtual KERESULT GetType(EString &type)
                        {
                            type = EEMAIL_STYLE_NAME;
                            return KE_OK;
                        };
    virtual KERESULT GetNewObject(IPersistent*& pObj)
                        {
                            pObj = new EEmailStyle;
                            return KE_OK;
                        };
    virtual KERESULT DestroyObject()
                        {
                            delete this;
                            return KE_OK;
                        };
public:
    EPersistentString mx_Name;
    EPersistentString mx_Server;
    EPersistentString mx_Recipients;
    EPersistentString mx_Domain;
    EPersistentString mx_Subject;
    EPersistentString mx_Body;
    EPersistentString mx_AttachFile;
    EPersistentInt    mx_SendMethod;
    EPersistentInt    mx_TextType;
    EString           m_strSender;
	EPersistentString mx_AuthType;
	EPersistentString mx_Username;
	EPersistentString mx_Password;
	void*			  m_pFncAuth;
    EPersistentInt    mx_EncodingMethod;
    EDynamicLibrary  *m_pLibrary;

public:
    bool isValid;
};


typedef enum {
    emailSending,           // In process of sending e-mail
    emailComplete,          // Finished sending e-mail
    emailNotfound,          // Deleted e-mail from list (old)
    emailError,             // Error sending e-mail
    emailErrorRecover       // Recoverable error
} EMAILSTATUS;

typedef enum {
    emailSMTP,              // Send email via SMTP
    emailENTERPRISE         // Send email via Enterprise Proxy
} EMAILSENDMETHOD;

typedef enum {
    emailTEXT,              // Send email as plain text
    emailHTML               // Send email as HTML text
} EMAILSENDTYPE;

typedef enum
{
    ENCODING_UFT8, 
    ENCODING_JIS
} EMAIL_ENCODE_METHOD;


typedef struct 
{
	ESocket Socket;
	EString m_Username;
	EString m_Password;
} SMTPAuthStruct;

typedef void (*EMAILSTATUSCALLBACK)(EMAILSTATUS, void *);

////////////////////////////////////////////////////////////////////////
// EmailProvider : Interface definition

interface IEmailProvider : public IKernelBase
{
public:
    //
    // SendEmail - Non-blocking; returns initial result.
    //
    virtual KERESULT SendEmail(
                                EMAILSENDMETHOD method,
                                TCHAR* serverHost,
                                TCHAR* recipientName,
                                TCHAR* senderName,
                                TCHAR* senderDomain,
                                TCHAR* subject,
                                TCHAR* bodytext,
                                TCHAR* filetoattach,
								void* funcAuth,
								TCHAR* username,
								TCHAR* password,
                                EMAILSENDTYPE txttype,
                                EMAIL_ENCODE_METHOD encodeMethod,
                                EMAILSTATUSCALLBACK pCallback = NULL,
                                TCHAR* pStyleName = NULL,
                                EHANDLE hClientId = 0,
                                int gatewayId = CONNECTOR_GATEWAY
                             ) = 0;

    virtual KERESULT SendEmail(EString pStyleName, EMAILSTATUSCALLBACK pCallback,
        EHANDLE hClientId = 0, EONEVENTSTRUCT* pEvent = NULL) = 0;

    // style management
    //

    // factory method for email style objects, to avoid concrete objects in other 
    // modules to avoid recursive dependancies
    virtual EEmailStyle* CreateEmailStyle(void) = 0;
    virtual void         DestroyEmailStyle(EEmailStyle* pStyle) = 0;

    // basic style list management
    //
    virtual KERESULT SetEmailStyle(EEmailStyle& style) = 0;
    virtual KERESULT GetEmailStyle(EEmailStyle& style) = 0;
    virtual KERESULT DeleteEmailStyle(EString& Name) = 0;

    // enumeration
    //
    virtual KERESULT GetFirstEmailStyle(EEmailStyle& style) = 0;
    virtual KERESULT GetNextEmailStyle(EEmailStyle& style) = 0;
};

#endif

