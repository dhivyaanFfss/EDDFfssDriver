//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved
//
//**************************************************************************
//
//  Filename   :  EXMLData.h
//  
//  Description:  Basic XML handling facilities
//
//  Class      :  EXMLDocument, EXMLElement, EXMLAttribute
//              
//**************************************************************************
#ifndef __EXMLDATA__H__
#define __EXMLDATA__H__

#include "ISax.h"

//**************************************************************************
class _DV_DECLSPEC IByteSink
{
public:
    virtual         ~IByteSink() = 0;
    virtual bool    Open(void) = 0;
    virtual void    Close(void) = 0;
    virtual bool    Write(const char *pData, int iSize) = 0;
};

//**************************************************************************
class _DV_DECLSPEC EXMLAttribute
{
public:
    EXMLAttribute(const TCHAR *pName, const TCHAR *pValue);
    ~EXMLAttribute();

    void    Write(EByteArray& output);
    int     GetFormattedSize(void);
    void    Write(IByteSink *pSink);

private:
    EByteArray  m_name;
    EByteArray  m_value;
};

#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EXMLAttribute*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, EXMLAttribute*>;
#endif // WIN32

//**************************************************************************
class _DV_DECLSPEC EXMLAttributeList : public ETypedPtrList<EPtrList, EXMLAttribute*>
{
public:
    ~EXMLAttributeList()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EXMLAttribute *pAttribute = GetNext(pos);
            if (pAttribute)
                delete pAttribute;
        }
        RemoveAll();
    }
};

//**************************************************************************
class _DV_DECLSPEC EXMLContent
{
public:
    virtual         ~EXMLContent() = 0;
    virtual void    Write(EByteArray& output) = 0;
    virtual int     GetFormattedSize(void) = 0;
    virtual void    Write(IByteSink *pSink) = 0;
};

#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EXMLContent*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, EXMLContent*>;
#endif // WIN32

//**************************************************************************
class _DV_DECLSPEC EXMLContentList : public ETypedPtrList<EPtrList, EXMLContent*>
{
public:
    ~EXMLContentList()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EXMLContent *pContent = GetNext(pos);
            if (pContent)
                delete pContent;
        }
        RemoveAll();
    }
};

//**************************************************************************
class _DV_DECLSPEC EXMLCharData : public EXMLContent
{
public:
    EXMLCharData(const TCHAR *pCharData);
    EXMLCharData(const char *pData, int iSize);
    ~EXMLCharData();

    void    Write(EByteArray& output);
    int     GetFormattedSize(void);
    void    Write(IByteSink *pSink);

private:
    EByteArray  m_charData;
};

//**************************************************************************
class _DV_DECLSPEC EXMLElement : public EXMLContent
{
public:
    EXMLElement(const TCHAR *pName, bool bEmpty = false);
    ~EXMLElement();

    void            Write(EByteArray& output);
    int             GetFormattedSize(void);

    EXMLAttribute   *AddAttribute(const TCHAR *pName, const TCHAR *pValue);
    EXMLElement     *AddElement(const TCHAR *pName, bool bEmpty = false);
    EXMLCharData    *AddCharData(const TCHAR *pCharData);
    void            AddContent(EXMLContent *pContent);

    EXMLContentList *GetContents(void)
    {
        return &m_contents;
    }

    void            Write(IByteSink *pSink);

private:
    EByteArray          m_name;
    EXMLAttributeList   m_attributes;
    EXMLContentList     m_contents;
    bool                m_bEmpty;
};

//**************************************************************************
class _DV_DECLSPEC EXMLDocument
{
public:
    EXMLDocument(const TCHAR *pName, bool bEmpty = false);
    ~EXMLDocument();

    void        SetProlog(const TCHAR *pText);
    void        Write(EByteArray& output);
    int         GetFormattedSize(void);

    EXMLElement *GetRoot()
    {
        return &m_root;
    }

    void        Write(IByteSink *pSink);

protected:
    EByteArray  m_prolog;
    EXMLElement m_root;
};

//**************************************************************************
#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EXMLElement*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, EXMLElement*>;
template class _DV_DECLSPEC ETypedStack<class EPtrList,class EXMLElement *>;
#endif

//**************************************************************************
class _DV_DECLSPEC EXMLFilter : public ISax
{
public:
    EXMLFilter(void);
    ~EXMLFilter();

    KERESULT Filter(const EString esInput, EString& esOutput);

protected:
    virtual void FilterAttributeValue(ESaxElement *pElement,
                                      const EString& esAttrName,
                                      const EString& esAttrValueIn,
                                      EString& esAttrValueOut);

    KERESULT OnElement(ESaxElement* pElement);

    EXMLDocument *m_pDocument;
    ETypedStack<EPtrList, EXMLElement*> m_elements;
    ESaxElement *m_pParent;
};

//**************************************************************************
inline IByteSink::~IByteSink()
{
}

#endif
