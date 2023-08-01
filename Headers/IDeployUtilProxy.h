//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  IDeployUtilProxy.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Defines the IDeployUtilProxy interface.  
//                The Deployment Component can act as a Proxy between
//                any Agent component and the Deployment Utility Application.
//                
//                This assists when a custom SOAP command is used to communicate
//                between the Deployment Utility and the Agent.  
//                IDeployUtilProxy is used to send the response from the
//                SOAP command being exectuted and the Deployment Utility.
//                
//                As a reminder, if APM is used, any SOAP command implemented must 
//                either be handled by APM or handled automatically by the APM Proxy.
//                For example, to have the SOAP command handled automatically, use
//                the following code:
//
//                    IPolicyMgr* pIPolicyMgr = NULL;
//                    GetInterface(IID_POLICY_MGR, (void**)&pIPolicyMgr);
//                    if (pIPolicyMgr)
//                        pIPolicyMgr->AddAutomaticFunction(_T("Component.Function"));
//
//                  where:  Component.Function is your SOAP command. 
//                
//
//
//*****************************************************************************

#ifndef __IDEPLOY_UTIL_PROXY_H__
#define __IDEPLOY_UTIL_PROXY_H__

#include "EXMLData.h"

enum REBOOT_STATE
{
    NO_REBOOT,
    REBOOT_AGENT,
    REBOOT_SYSTEM
};

//*****************************************************************************
// IDeployUtilProxy
interface IDeployUtilProxy : public IKernelBase
{
    // Post SOAP Response during the processing of a SOAP command from the 
    // Deployment Utility.  After the return from SOAP command, the
    // Deployment component will send the response to the D.U. Application.
    //
    // NOTE:  Other option is to use pReplyMethod and replyState in ProcessSoapRPC().
    //        In this case replyState must be set to soapSkipSoapEnvelope.
    //        See SoapDispatchInterface.h and SoapParse.h.
    //
    //        When using IDeployUtilProxy instead of the ProcessSOAPRPC() pReplyMethod,
    //        make sure to set the replyState to soapReplySkip.
    //
    // PostSOAPResponse Parameters:
    // esResponse:  The formatted XML response to the D.U. 
    // cmdResult:  When the cmdResult is set to KE_OK with esResponse.IsEmpty(), the Deployment
    //   component will return a status of OK to the Deployment Utility.
    //   When the cmdResult != KE_OK, an error status code of "scUNSPECIFIED_ERROR" is returned
    //   to the Deployment Utility.  Thus, when the cmdResult != KE_OK, the esResponse is ignored.
    //   In 3.9, the cmdResult values of KE_TIMEOUT and KE_BAD_XML_FORMAT are handled.
    //
    // rebootState:  Setting the rebootState cause the Agent to be rebooted.  This is only necessary
    //   when the SOAP command requires new configuration settings to take effect only at 
    //   start-up of the Agent.
    virtual KERESULT PostSOAPResponse(EString& esResponse, KERESULT cmdResult = KE_OK,
        REBOOT_STATE rebootState = NO_REBOOT) = 0;

    // Same as above, except response is formated in an EXMLDocument object.
    virtual KERESULT PostSOAPResponse(EXMLDocument& rXMLDoc, KERESULT cmdResult = KE_OK,
        REBOOT_STATE rebootState = NO_REBOOT) = 0;

    // Post an unsolicited response to the Deployment Utility.
    // This functionality may not be supported in the 3.7.1 release.
//    virtual KERESULT PostUnsolicitedResponse(EString& esResponse) = 0;
};

#endif // __IDEPLOY_UTIL_PROXY_H__


