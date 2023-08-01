//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  SoapdispatchInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Soap Dispatch Interface
//
//  Classes:      ISoapMsgHandler, 
//                ISoapDispatcher 
//                ISoapRedirector
//
//************************************************************************** */

#ifndef __SOAP_INTERFACE_H__
#define __SOAP_INTERFACE_H__

/* Group=Soap */

////////////////////////////////////////////////////////////////////////
// Remarks: Soap Message Handler Interface
interface ISoapMsgHandler
{
    virtual KERESULT ProcessSoapRPC(
                                    ISoapParser*    pParser,
                                    TCHAR*          pFunctionName,
                                    DWORD           fcnVersion,
                                    soapMethod*     pMethod,
                                    soapMethod*&    pReplyMethod,
                                    soapReplyState& replyState
                                    ) =0;
};

////////////////////////////////////////////////////////////////////////
// Remarks: Soap Dispatcher Interface
interface ISoapDispatcher
{
    virtual KERESULT Register(ISoapMsgHandler* pMsgHandler, TCHAR* pComponentName) = 0;
    virtual KERESULT Unregister(TCHAR* pComponentName) = 0;
    virtual KERESULT ProcessSoapRpc(
                                    ISoapParser*    pParser,
                                    TCHAR*          pComponentName, 
                                    TCHAR*          pFunctionName,
                                    DWORD           fcnVersion,
                                    soapMethod*     pMethod,
                                    soapMethod*&    pReplyMethod,
                                    soapReplyState& replyState
                                    ) = 0;
    virtual KERESULT Register(ISoapMsgHandler* pMsgHandler, TCHAR* pComponentName,
                              bool Authenticated) = 0;
};

////////////////////////////////////////////////////////////////////////
// Allow a re-direct (hook) of a ProcessSoapRpc to another handler.
// The implementor can allow multiple registered hooks, thus, the
// UnRegiseter is per ISoapMsgHandler not componentName.
// 
// Added for 3.8.3 patch for Agilent
// This interface will need to be enhanced as new scenerios arise.
interface ISoapRedirector
{
    // set bOverride to have only the hook handle the message.
    virtual KERESULT Register(ISoapMsgHandler* pISoapMsgHandler, TCHAR* pszComponentName,
        bool bOverride = false) = 0;
    virtual KERESULT Unregister(ISoapMsgHandler* pISoapMsgHandler) = 0;

};

#endif

