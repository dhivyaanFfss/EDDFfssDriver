//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ITextParser.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for text processing filter
//
//  Classes:      ITextParser
//
//**************************************************************************
#ifndef _ITEXTPARSER_H_
#define _ITEXTPARSER_H_

/* Group=XML Parser */

#define XML_MAX_TOKEN 128

#ifdef WIN32
#ifdef _DV_EXPORT
/* Group=XML Parser */
#define EPARSER_DECL __declspec(dllexport)
#else
/* Group=XML Parser */
#define EPARSER_DECL __declspec(dllimport)
#endif
#else
#define EPARSER_DECL
#endif

//**************************************************************************
// IxmlStreamSource - interface for source streams                        /*
//**************************************************************************
interface IxmlStreamSource 
{
public:
    typedef enum {
        xmlUTF8,
        xmlUCS2,
        xmlUCS4,
        xmlASCII
    } xmlEncoding;

public:
    virtual int      Read(TCHAR* buf, int len, KERESULT *pError) = 0;
    virtual int      Write(TCHAR* buf, int len, KERESULT *pError) = 0;
    virtual KERESULT GetStreamBuffer(EString& result) = 0;
    virtual KERESULT ResetStream() = 0;
    virtual bool     IsUtf8Encoded(void) = 0;
    virtual void     DestroyObject(void) = 0;
};

//**************************************************************************
// IxmlToken - interface for tokenizing source streams                    /*
//**************************************************************************
interface IxmlToken
{
public:
    // reading token type
    typedef enum {
        xmlStartTag,
        xmlEndTag,
        xmlComment,
        xmlText,
        xmlAttributeName,
        xmlAttributeValue,
        xmlHtmlTag,
        xmlHtmlEndTag
    } xmlTokenType;

public:
    virtual KERESULT        DestroyObject(void) = 0;

    // reading
    virtual TCHAR*          XmlGetToken(bool preserveSource, KERESULT* pError) = 0;
    virtual xmlTokenType    XmlGetTokenType(bool mixHTML) = 0;
    virtual TCHAR*          XmlGetNamespace(void) = 0;
    virtual TCHAR*          Token(void) = 0;

    // writing
    virtual KERESULT        XmlOpenSection(const TCHAR* pText) = 0;
    virtual KERESULT        XmlOpenSection(const TCHAR* pText, const TCHAR* pNamespace, const TCHAR* pNamespaceURI) = 0;
    virtual KERESULT        XmlCloseSection(const TCHAR* pText) = 0;
    virtual KERESULT        XmlAbortSection(void) = 0;
    virtual KERESULT        XmlPutAttribute(const TCHAR* pAttrib, const TCHAR *pValue) = 0;
    virtual KERESULT        XmlPutValue(const TCHAR* pType, const TCHAR *pValue) = 0;
    virtual KERESULT        XmlPutComment(const TCHAR* pComment) = 0;
    virtual KERESULT        XmlPutRaw(const TCHAR* pRawXml) = 0;
    virtual KERESULT        XmlSetPretty(bool bPretty) = 0;

    // special, for retrieving results of building XML into a string
    virtual KERESULT        XmlGetStreamBuffer(EString& result) = 0;
    virtual KERESULT        ResetStream() = 0;
};

//**************************************************************************
// Library interface for text parsing utilities                           /*
//**************************************************************************

EPARSER_DECL KERESULT CreateFileXmlToken(
                                IxmlToken** pToken,
                                TCHAR*      fileName,
                                bool        bForceCreation = false
                          );

EPARSER_DECL KERESULT CreateStringXmlToken(
                                IxmlToken** pToken,
                                TCHAR*      sourceStr
                          );

EPARSER_DECL KERESULT DestroyXmlToken(IxmlToken* pToken);

EPARSER_DECL KERESULT ParseEscTextCGI(
                                IKernelService  *pKernel,
                                TCHAR*  pSource,
                                TCHAR** pReply,
                                int*    replyLength,
                                void**  pUserData,
                                HTTP_OperationState*
                                            pReplyState
                          );


#endif // _ITEXTPARSER_H_


