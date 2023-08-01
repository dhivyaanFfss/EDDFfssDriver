//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ISax.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Sax interface definitions.
//
//  Classes:                                         
//          ISax
//          ESaxAttribute
//          ESaxAttributes
//          ESaxElement
//
//**************************************************************************

#ifndef __SAX_INTERFACE_H__
#define __SAX_INTERFACE_H__

/* Group=Sax */

#define SAX_DELIMITER       _T('\\')    // used w/ fully qualified names.
#define SAX_DELIMITER_STR   _T("\\")    

//**************************************************************************
class  _DV_DECLSPEC ESaxAttribute
{
public:
    ESaxAttribute(EString& rAttrib, EString& rValue);
    ~ESaxAttribute();

    EString         m_esName;            // Attribute name
    EString         m_esValue;           // Attribute value
};

// List of attributes w/i an Element body
typedef ETypedPtrList<EPtrList, ESaxAttribute*> ESaxAttributes;

//**************************************************************************
class  _DV_DECLSPEC ESaxElement
{
public:
    ESaxElement();
    ~ESaxElement();
    // Retreive a specific attribute Value.
    // Return NULL if not found.
    TCHAR* GetAttributeValue(TCHAR* pszAttribute);
    TCHAR* GetAttributeValue(const TCHAR* pszAttribute);
    void CreateQualifiedName(EString& resQualifiedName);
    ESaxElement* GetRoot();     // gets top level element.
    ESaxElement* FindParent(TCHAR* pszParent);    // may return NULL if parent not found.

    int AddRef(void);
    int Release(void);

    bool GetAttributeValue(TCHAR* pszName, EString& esValue);
    bool GetAttributeValue(TCHAR* pszName, int& iValue);
    bool GetAttributeValue(TCHAR* pszName, bool& bValue);

    int GetRefCnt(void) { return m_refCnt; }
    
    EString         m_esName;           // element name
    EString         m_esBody;           // element body.  May be empty
    ESaxAttributes* m_pAttributes;      // List of element attributes - may be null.
    ESaxElement*    m_pParent;          // may be null.
    ESaxElement*    m_pChild;          // may be null.
protected:
    int             m_refCnt;
    
};

// Stack of Elements - used by Sax parser for nested Elements.
typedef ETypedStack<EPtrList, ESaxElement*> ESaxElementStack;

//**************************************************************************
interface ISax
{
public:
    // Callback for Each element.
    // Do addRef on pElement if callback needs to keep the Element instance.
    // Due to linear nature of SAX parsing, the deepest nested element is
    // sent to callback first, if the top level element names are required,
    // use the ptr to the parent (Note:  the parent will only be partially 
    // completed).
    virtual KERESULT   OnElement(ESaxElement* pElement) = 0;

};

//**************************************************************************
class  _DV_DECLSPEC ESaxParser
{
public:
    ESaxParser();
    ~ESaxParser();

    // bConfigFile added 3.1 to allow non Builder generated XML files.
    // When bConfigFile == false, use pszModuleName as the full path.
    KERESULT RestoreSection(const TCHAR* pszModuleName, ISax* pISax, 
        bool bConfigFile = true);

    // Use a SAX parser with a IxmlTolken.
    // A IXmlTolken can be created using a file (use RestoreSection) or a string.
    // SEE:  CreateFileXmlToken() and CreateStringXmlToken() respectively.
    // Reset is required if IxmlToken needs to be reset before using.
    KERESULT Parse(IxmlToken* pIxmlToken, ISax* pISax, bool bReset = false);
    
    static TCHAR*   RemoveDelimiters(TCHAR* pszItem);
    static TCHAR*   RemoveQuotes(TCHAR* pszItem);
    static TCHAR*   RemoveCDataTags(TCHAR* pszItem);

protected:
    KERESULT StartNewElement();
    KERESULT SetNewAttribute(EString& rAttribName);
    void     CleanUp();

protected:
    ISax*               m_pISax;
    IxmlToken*          m_pIXmlToken;
    ESaxElement*        m_pElement;
    ESaxElementStack*   m_pStack;
    TCHAR*              m_pszToken;     // Returned string from m_pIXmlToken.
};

#endif


