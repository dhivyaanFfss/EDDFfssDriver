//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  SoapparserInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Soap Parser Interface
//
//  Classes:      ISoapParser 
//
//************************************************************************** */

#ifndef __SOAP_PARSER_INTERFACE_H__
#define __SOAP_PARSER_INTERFACE_H__

/* Group=Soap */

#include "SoapParse.h"

////////////////////////////////////////////////////////////////////////
// Remarks: Soap Parser Interface
//
// step 1: create a soap parsing object to use
//
interface ISoapParser;

interface ISoapParserFactory
{
    virtual KERESULT CreateSoapParser(ISoapParser** pParser) = 0;
    virtual KERESULT DestroySoapParser(ISoapParser* pParser) = 0;
};

#ifndef _ITEXTPARSER_H_
interface IxmlStreamSource;
#endif

////////////////////////////////////////////////////////////////////////
// Remarks: Soap Parser Interface
//
// step 2: use this interface on the actual object
//
interface ISoapParser : public IKernelBase
{
    // highest level - give and get response to soap calls
    //
    virtual KERESULT    ParseSoap(TCHAR *pSourceStr) = 0;
    virtual KERESULT    XmlParseSoap(IxmlStreamSource* pSource) = 0;
    virtual KERESULT    GetResultLength(int& length) = 0;
    virtual KERESULT    GetResult(TCHAR*& pResult, bool& isComplete) = 0;
    virtual KERESULT    GetUTF8Result(char*& pResult, bool& isComplete) = 0;
    virtual KERESULT    FreeResult(void) = 0;
    // 3.5 addition.  For use with Sax Parser.
    virtual KERESULT    GetIxmlToken(IxmlToken** ppIxmlToken) = 0;

    // creating soap method-parm-attribute structures
    //
    virtual KERESULT    SoapAddMethod(TCHAR *pName, soapMethod*& pMethod, TCHAR* pNamespace=NULL) = 0;
    virtual KERESULT    SoapFirstParm(soapMethod* pMethod, soapParm*& pParm) = 0;
    virtual KERESULT    SoapNextParm(soapParm* pParm, soapMethod* pMethod, soapParm*& pReturnParm) = 0;
    virtual KERESULT    SoapParentParm(soapParm* pParm, soapMethod* pMethod, soapParm*& pReturnParm) = 0;
    virtual KERESULT    SoapSetCurrentParm(soapParm* pParm, soapMethod* pMethod, soapParm*& pReturnParm) = 0;
    virtual KERESULT    SoapAddParm(TCHAR* pName, bool isChild, soapMethod* pMethod, soapParm*& pParm) = 0;
    virtual KERESULT    SoapAddParmValue(soapValue* pValue, soapVtype type, soapParm *pParm, soapValue*& pReturnValue) = 0;
    virtual KERESULT    SoapAddParmValue(TCHAR* pStr, soapParm *pParm, soapValue*& pReturnValue) = 0;
    virtual KERESULT    SoapAddParmValue(long val, soapParm *pParm, soapValue*& pReturnValue) = 0;
    virtual KERESULT    SoapRemoveParms(soapMethod* pMethod) = 0;
    virtual KERESULT    SoapGetAttribute(soapMethod* pParm, TCHAR* attributeName, EString& value) = 0;
    virtual KERESULT    SoapGetAttribute(soapMethod* pParm, TCHAR* attributeName, unsigned long& value) = 0;
    virtual KERESULT    SoapGetAttribute(soapParm* pParm, TCHAR* attributeName, EString& value) = 0;
    virtual KERESULT    SoapGetAttribute(soapParm* pParm, TCHAR* attributeName, unsigned long& value) = 0;
    virtual KERESULT    SoapAddAttribute(soapMethod* pMethod, TCHAR* attributeName, EString& value) = 0;
    virtual KERESULT    SoapAddAttribute(soapParm* pParm, TCHAR* attributeName, EString& value) = 0;

    // converting parm structures back into Soap format
    //
    virtual KERESULT    SoapConvertObjectParameter(soapParm* pParm, TCHAR* pName) = 0;
    virtual KERESULT    SoapConvertStringParameter(soapParm* pParm, TCHAR* pName, EString& result) = 0;
    virtual KERESULT    SoapConvertIntegerParameter(soapParm* pParm, TCHAR* pName, unsigned long& result) = 0;
    virtual KERESULT    SoapConvertBoolParameter(soapParm* pParm, TCHAR* pName, bool& result) = 0;

    // CgiInfo
    virtual KERESULT SetCgiInfo(CgiInfo& rInfo) = 0;
    virtual KERESULT GetCgiInfo(CgiInfo* pInfo) = 0;

    // invoking a parsed soap method
    //
    virtual KERESULT    SoapDispatch(soapMethod* pMethod, soapReplyState state) = 0;
    virtual KERESULT    SoapContinueInvocation(void) = 0;
    virtual KERESULT    SoapEndInvocation(void) = 0;

    virtual KERESULT Destroy() = 0;

    // 3.5 methods to do reference counting and to get Parser method object.
    virtual KERESULT    AddRef() = 0;
    virtual KERESULT    Release() = 0;
    virtual KERESULT    GetMethod(/* out*/ soapMethod** ppMethod) = 0;
    virtual KERESULT    ParseMethodName(/* in */soapMethod* pMethod, 
        /* out */ EString& resComponentName, EString& resFunctionName) = 0;


    // 3.6 extensions for security policy handling
    // By default, policy checking is enabled, this is disabled by the
    // SM package when the package permission is ok to process.  Thus,
    // each instructions individual permission is to be ignored.
    virtual KERESULT    EnablePolicyChecking(bool bEnabled = true) = 0;
    virtual KERESULT    IsPolicyEnabled(bool* pbEnabled) = 0;
    // APM needs to track if a package is opened; this means that the APM tracks each
    // instruction in the package.  If a package is not opened, the APM deals with the
    // package per is name and version, not its content.
    virtual KERESULT    SetPackageInfo(bool bIsPackage = false, bool bOpenPackage = true) = 0;
    virtual KERESULT    GetPackageInfo(bool* pIsPackage, bool* pbOpenPackage) = 0;


    virtual KERESULT    SoapGetFirstChild(soapParm* pParm, soapParm*& pReturnParm) = 0;
    virtual KERESULT    SoapGetNextElement(soapParm* pParm, soapParm*& pReturnParm) = 0;
    // Set bNested == false to only get at element level w/o nesting.
    virtual KERESULT    SoapGetValue(soapMethod* pMethod, TCHAR* pszName, EString& value, bool bNested = true ) = 0;
    virtual KERESULT    SoapGetValue(soapMethod* pMethod, TCHAR* pszName, unsigned long& value, bool bNested = true ) = 0;
    virtual KERESULT    SoapGetValue(soapParm* pParm, TCHAR* pszName, EString& value, bool bNested = true ) = 0;
    virtual KERESULT    SoapGetValue(soapParm* pParm, TCHAR* pszName, unsigned long& value, bool bNested = true ) = 0;

    virtual KERESULT    SoapRecreateXML(soapMethod* pMethod, EString& esXML) = 0;
    virtual KERESULT    SoapRecreateXML(soapParm* pParm, EString& esXML) = 0;

    // 3.8 addition.
    virtual KERESULT    SoapError(TCHAR* pError, TCHAR *pMore, soapParseState state) = 0;

    // 5.0
    virtual KERESULT    EnabledAuthentication(bool bEnabled = true) = 0;
    virtual KERESULT    IsAuthenticationEnabled(bool *pbEnabled) = 0;

    virtual KERESULT    GetServerName(EString& esServerName) = 0;
    virtual KERESULT    SetServerName(TCHAR *pszServerName) = 0;

    // 5.0 + File Transfer Enhancements: Methods supporting 64-bit integers
    virtual KERESULT    SoapAddParmValue64(LONGLONG val, soapParm *pParm, soapValue*& pReturnValue) = 0;
    virtual KERESULT    SoapGetAttribute64(soapMethod* pParm, TCHAR* attributeName, LONGLONG& value) = 0;
    virtual KERESULT    SoapGetAttribute64(soapParm* pParm, TCHAR* attributeName, LONGLONG& value) = 0;
    virtual KERESULT    SoapConvertIntegerParameter64(soapParm* pParm, TCHAR* pName, LONGLONG& result) = 0;
    virtual KERESULT    SoapGetValue64(soapMethod* pMethod, TCHAR* pszName, LONGLONG& value, bool bNested = true ) = 0;
    virtual KERESULT    SoapGetValue64(soapParm* pParm, TCHAR* pszName, LONGLONG& value, bool bNested = true ) = 0;

    // 5.3
    virtual KERESULT    SoapGetHeader(soapMethod* pMethod, soapMethod*& pHeader) = 0;

    virtual KERESULT    GetSourceName(EString& esSourceName) = 0;
    virtual KERESULT    SetSourceName(TCHAR *pszSourceName) = 0;

    virtual KERESULT    RecreateSoap(soapMethod *pHeaders, soapMethod *pMethod, EString& esXML) = 0;
};
#endif

