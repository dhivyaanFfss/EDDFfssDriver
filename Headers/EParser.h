//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EParser.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Text Parsing (Xml/Soap/etc) Component
//
//  Classes:      EParser is an IKernelService for this component
//
//**************************************************************************
#ifndef __EPARSER_H_
#define __EPARSER_H_

#ifdef WIN32
#ifdef _DV_EXPORT
#define EPARSER_DECL __declspec(dllexport)
#else
#define EPARSER_DECL __declspec(dllimport)
#endif
#else
#define EPARSER_DECL
#endif

#define EPARSER_NAME _T("Text Parser")

// generic routines for parsing and filtering xml and tag-escaped data
//
class EPARSER_DECL EscFilter : public ITextFilter
{
public:
    EscFilter(
				IKernelService* pKernel,
				TCHAR* source,
				void *pEvent = NULL,
				bool fromFile = false,
                ETimeStamp* pTimeStamp = NULL,
                int gatewayID = OUT_OF_GATEWAY_IDS
			);
    virtual ~EscFilter();
 
public:
    typedef enum {
        eid_Bad,
        eid_Event,
        eid_Alarm,
        eid_Tag,
        eid_TagDesp,
        eid_Config,
        eid_Time,
        eid_Date
    } EscResID;

public:
    virtual TCHAR*      GetContent(KERESULT* pError);

private:
    void                Error(TCHAR* pMsg, TCHAR* pMore);
    bool                MapComponentID(TCHAR* pInCompStr, EscResID* pOutCompId);
    KERESULT            FetchItem(EscResID id, TCHAR* pItem, EString& result);    
    KERESULT            ParseString(TCHAR* pStr, EString& result);
    KERESULT            PreParseString(TCHAR* pStr);
    TCHAR*              HtmlFilter(KERESULT* pError);
    KERESULT            CreateQualifiedName(TCHAR* pClean, int bufSize);

private:
	IKernelService*		m_pKernel;
    EString             m_result;
    void*               m_pEvent;
    ETimeStamp*         m_pTimeStamp;
    IxmlToken*          m_pTok;
    EString             m_msgbuf;
    int                 m_gatewayID;
};


//**************************************************************************
// Exported Library Functions to support XML 
//**************************************************************************

// wrap the input string with Soap envelope as follows.
// <SOAP-ENV:Envelope
//    <SOAP-ENV:Body>
//          XML Command   
//    </SOAP-ENV:Body>
// </SOAP-ENV:Envelope
//
// This is helpful when sending a command to the the Agents own parser.
// 
EPARSER_DECL void WrapInSoapEnvelope(/* in/ out */ EString& resXMLCmd);


#endif // EPARSER_H_




