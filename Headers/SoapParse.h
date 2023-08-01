//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  SoapParse.h
//  
//  Subsystem  :  Kernel
//
//  Description:  General Soap parsing support
//
//  Classes:      ESoapParser
//
//**************************************************************************
#ifndef _SOAPPARSE_H_
#define _SOAPPARSE_H_

#include "IWebProvider.h"

typedef enum {
	soapPsBase,
	soapPsEnvelope,
    soapPsMiscElement,
	soapPsBody,
	soapPsMethod,
	soapPsParm,
    soapPsError,
    soapPsHeader,
    soapPsHeaderElement,
    soapPsHeaderElementParm,
} soapParseState;

static TCHAR* soapStateStrings[] = {
	_T("Base"),
	_T("Envelope"),
    _T("MiscElement"),
	_T("Body"),
	_T("Method"),
	_T("Parameter"),
	_T("Error"),
	_T("Header"),
	_T("HeaderElement"),
    _T("HeaderElementParameter"),
};

typedef enum {
    soapReplyComplete,
    soapReplyFirstOfMany,
    soapReplyNextOfMany,
    soapReplyLastOfMany,
    soapReplyLast,
    soapReplySkip,
    soapReplyStop,
    soapSkipSoapEnvelope        // Use this to have IParser create XML response w/o SOAP evelope.
} soapReplyState;

#define SOAP_ENV_NAMESPACE  _T("SOAP-ENV")
#define SOAP_ENV_ENVELOPE   _T("Envelope")
#define SOAP_ENV_BODY       _T("Body")
#define SOAP_ENV_HEADER     _T("Header")


typedef TCHAR soapValue;

typedef enum {
    soapNumber,
    soapString
} soapVtype;


// the SOAP parameter list is a tree structure meant to be parsed only from the 
// the top (root).  The order is a list of named parameter value pairs.  Sub 
// parameters are handled as a sub list, who's last element points 
// SOAP parameter 
//
struct _DV_DECLSPEC soapParm  // tag_soap_parm
{
    soapParm();
    void Copy(soapParm* pSoapParm);
    void Destroy();
    void DestroyParm(soapParm* pParm);

    // properties

    TCHAR*      pName;
    soapValue*  pValue;
    soapVtype   type;
    soapParm*   pChild;
    soapParm*   pNext;
    soapParm*   pParent;
    soapParm*   pAttrs;
    soapParm*   pLastChild;
};

typedef soapParm soapAttribute;

// SOAP method
//
struct _DV_DECLSPEC soapMethod  // soapMethod
{
    soapMethod();
    void Copy(const soapMethod* pMethod);
    void Destroy();

    // properties
    TCHAR*          pName;
    TCHAR*          pNamespace;
    soapParm*       pParms;
    soapParm*       pAttrs;
    soapParm*       pCurrent;
    soapMethod*     pNext;
}; 

// The soap dispatcher uses a list of these to dispatch through
//
typedef struct tag_soap_ftab
{
    TCHAR*       pName;           // function name
    DWORD        funcID;          // id (FN_ series)
    bool         hasInterface;    // true if not local test function
    KERNEL_IID   interfaceID;     // the ID of the required interface
} aSoapFunctionTable;

#define FN_SOAP_TEST    1000
#define FN_UNSUPPORTED  (-1L)

#define SOAP_FAULT_FORMAT           \
    _T("<SOAP-ENV:Envelope \n")                                                         \
    _T("    xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"\n")            \
    _T("    SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n")  \
    _T("    <SOAP-ENV:Body>\n")                                                         \
    _T("        <SOAP-ENV:Fault>\n")                                                    \
    _T("            <faultcode>SOAP-ENV:Server</faultcode>\n")                          \
    _T("            <faultstring>%s</faultstring>\n")                                   \
    _T("        </SOAP-ENV:Fault>\n")                                                   \
    _T("    </SOAP-ENV:Body>\n")                                                        \
    _T("</SOAP-ENV:Envelope>\n")

#define SOAP_KERESULT_FORMAT           \
    _T("<SOAP-ENV:Envelope \n")                                                         \
    _T("    xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\"\n")            \
    _T("    SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n")  \
    _T("    <SOAP-ENV:Body>\n")                                                         \
    _T("        <%sResponse>\n")                                                      \
    _T("           <keresult>\n")                                                       \
    _T("               %s\n")                                                           \
    _T("           </keresult>\n")                                                      \
    _T("        </%sResponse>\n")                                                     \
    _T("    </SOAP-ENV:Body>\n")                                                        \
    _T("</SOAP-ENV:Envelope>\n")

#define SOAP_PARM_BASE64_VALUE_FORMAT    \
    _T("%s xsi:type=\"SOAP-ENC:base64\n")

#define SOAP_ENVELOPE_XML _T("SOAP-ENV:Envelope")
#define SOAP_HEADER_XML   _T("SOAP-ENV:Header")
#define SOAP_BODY_XML     _T("SOAP-ENV:Body")

#define     SOAP_MODEL_ATTR         _T("mn")
#define     SOAP_SERIAL_ATTR        _T("sn")
#define     SOAP_OWNER_ATTR         _T("ow")
#define     SOAP_USERID_ATTR        _T("userId")
#define     SOAP_CMDID_ATTR         _T("cmdId")
#define     SOAP_WAIT_ATTR          _T("wa")

#define     SOAP_AUTH_CONTEXT_ATTR  _T("authCtx")
#define     SOAP_AUTH_USER_ATTR     _T("authUser")
#define     SOAP_AUTH_PASSWORD_ATTR _T("authPasswd")

#endif // _SOAPPARSE_H_
