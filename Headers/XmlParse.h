//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  XmlParse.h
//  
//  Subsystem  :  Kernel
//
//  Description:  General XML parsing support
//
//  Classes:      ExmlToken
//
//**************************************************************************

#ifndef _XMLPARSE_H
#define _XMLPARSE_H




//**************************************************************************
// ExmlStrStream - ring-buffer String wrapped in ExmlStreamSource         /*
//**************************************************************************
class EPARSER_DECL ExmlStrStream : public IxmlStreamSource
{
public:
    ExmlStrStream(TCHAR *pStr);
    virtual ~ExmlStrStream();

public:
    virtual void        DestroyObject(void);
    virtual int         Read(TCHAR* pBuf, int length, KERESULT* pError);
    virtual int         Write(TCHAR* pBuf, int length, KERESULT* pError);
    virtual TCHAR*      GetBuffer();
    virtual KERESULT    GetStreamBuffer(EString& str);
    virtual KERESULT    ResetStream();
    virtual bool        IsUtf8Encoded(void) { return false; };

private:
    EString m_Buffer;
    TCHAR*  m_pRead;
    int     m_nRemain;
};

//**************************************************************************
// ExmlFileStream - EFileIO wrapped in ExmlStreamSource                   /*
//**************************************************************************
class EPARSER_DECL ExmlFileStream : public IxmlStreamSource
{
public:
    ExmlFileStream(TCHAR *pFilename, bool bForceCreation = false) ;
    virtual ~ExmlFileStream();

public:
    virtual void        DestroyObject(void);
    virtual int         Read(TCHAR* pBuf, int length, KERESULT* pError);
    virtual int         Write(TCHAR* pBuf, int length, KERESULT* pError);
    virtual KERESULT    GetStreamBuffer(EString& result);
    virtual KERESULT    ResetStream();
    virtual bool        IsUtf8Encoded(void) { return m_FileEncoding == xmlUTF8; };
    // Is this a valid object - required because of work in constructor.
    bool                IsValid();

private:
    EFileIO* m_pFile;
    bool     m_wasReading;

    // if reading is the first action, the actual file named is read.
    // writing is done to a temp file with a ~ appended to its name, then
    // coppied to the right file in the destructor
    bool     m_bInitialRead;

    // Flags for file conversion
    xmlEncoding m_FileEncoding;
    bool        m_bSwapBytes;
};

//**************************************************************************
interface IxmlGetChar
{
public:
    virtual void  DestroyObject(void) = 0;
    virtual TCHAR GetNextChar(KERESULT*, bool&) = 0;
    virtual void  UngetChar(TCHAR uc) = 0;
    virtual KERESULT Reset() = 0;       // reset getting char from beginning
};

//**************************************************************************
class EGetXmlChar : public IxmlGetChar
{
public:
    EGetXmlChar(IxmlStreamSource* pSource, int maxLength) {
        m_readCount     = 0;
        m_pStream       = pSource;
        m_maxLength     = maxLength;
        m_bUngotten     = false;
    };
    virtual ~EGetXmlChar() {}
    virtual void  DestroyObject(void)      { delete this; };
    virtual void  UngetChar(TCHAR uc)      { m_ungotten = uc; m_bUngotten = true; }
    virtual KERESULT Reset()
    {  // reset getting char from beginning
        m_readCount = 0; 
        m_bUngotten = false; 
        return KE_OK;
    }  

protected:
    IxmlStreamSource*   m_pStream;
    int                 m_readCount;
    int                 m_maxLength;
    TCHAR               m_ungotten;
    bool                m_bUngotten;
};

//**************************************************************************
class EGetUTF8Char : public EGetXmlChar
{
public:
    EGetUTF8Char(IxmlStreamSource* pSource, int maxLength) 
        : m_pRead(NULL), EGetXmlChar(pSource, maxLength) 
    {
        m_pIObuffer       = (char*)new TCHAR[maxLength + 2];
    };
    virtual ~EGetUTF8Char()
    {             
        delete [] m_pIObuffer;
    }

public:
    virtual TCHAR GetNextChar(KERESULT*, bool&);

private:
    BYTE  GetNextByte(KERESULT* pError, bool& bEOF);
    char* m_pIObuffer;
    char* m_pRead;
};

//**************************************************************************
class EGetUCS2Char : public EGetXmlChar
{
public:
    EGetUCS2Char(IxmlStreamSource* pSource, int maxLength) 
        : m_pRead(NULL), EGetXmlChar(pSource, maxLength) 
    {
        m_pIObuffer       = new TCHAR[maxLength + 2];
    };
    ~EGetUCS2Char()
    {
        delete [] m_pIObuffer;
    }
public:
    virtual TCHAR GetNextChar(KERESULT*, bool&);

private:
    TCHAR* m_pIObuffer;
    TCHAR* m_pRead;
};

//**************************************************************************
// ExmlToken - handles parsing XML and HTML source                        /*
//**************************************************************************

//
// Tokenizer
//
class EPARSER_DECL ExmlToken : public IxmlToken
{
public:
    typedef enum {
        txtPsBase,
        txtPsElement,
        txtPsAttrLeft,
        txtPsAttrRight,
        txtPsComment,
        txtPsEntity,
        txtPsQuote,
        txtPsWhite,
        txtPsHex1,
        txtPsHex2,
        txtPsEmptyTag,
        txtPsEmptyTagBody,
        txtPsEmptyTagEnd
    } txtParseState;

public:
    ExmlToken(
                    int maxlen,
                    IxmlStreamSource *pSource
            );

    virtual ~ExmlToken();

public:
    // IxmlToken interface
    //
    virtual KERESULT        DestroyObject(void)
    {
        delete this;
        return KE_OK;
    };
    // reading
    virtual TCHAR*          XmlGetToken(bool preserveSource, KERESULT* pError);
    virtual inline TCHAR*   Token(void) { return m_pToken; };
    virtual inline TCHAR*   XmlGetNamespace(void) { return m_Namespace.GetBuffer(0); };
    virtual xmlTokenType    XmlGetTokenType(bool mixHTML);

    // writing
    virtual KERESULT        XmlOpenSection(const TCHAR* pText);
    virtual KERESULT        XmlOpenSection(const TCHAR* pText, const TCHAR* pNamespace, const TCHAR* pNamespaceURI);
    virtual KERESULT        XmlAbortSection(void);
    virtual KERESULT        XmlCloseSection(const TCHAR* pText);
    virtual KERESULT        XmlPutAttribute(const TCHAR* pAttrib, const TCHAR *pValue);
    virtual KERESULT        XmlPutValue(const TCHAR* pType, const TCHAR *pValue);
    virtual KERESULT        XmlPutComment(const TCHAR* pComment);
    virtual KERESULT        XmlPutRaw(const TCHAR* pRaw);
    virtual KERESULT        XmlSetPretty(bool bPretty) { m_pretty = bPretty; return KE_OK; };

    virtual KERESULT        XmlGetStreamBuffer(EString& result);
    virtual KERESULT        ResetStream();


    static bool             XmlTagCompare(const TCHAR* pA, const TCHAR* pB, bool caseSensitive = true);
    static bool             HtmlReservedKeyword(const TCHAR* pWord);

    // Support
    bool GetPretty() { return m_pretty;}
    void EnableQuotedTokenInBaseState(bool bEnable) { m_bQuotedTokenInBaseState = bEnable; }

protected:
    virtual void            ResetForWrite(void);
    virtual void            MakeIndentStr(int level);
    virtual KERESULT        StreamWrite(EString& source, bool isVal = false, bool notEol = false);
    virtual KERESULT        StreamWrite(TCHAR cc);
    virtual TCHAR*          StreamFormat(TCHAR* pBuf, int length, int& retLen, bool isVal = false, bool bSupressNewline = false);

    inline TCHAR UpperHexDigit(TCHAR v) {
        v >>= 4; v &= 0xF; if(v < 10) return v + _T('0'); else return v - 10 + _T('A');
    };
    inline TCHAR LowerHexDigit(TCHAR v) {
        v &= 0xF; if(v < 10) return v + _T('0'); else return v - 10 + _T('A');
    };

    bool    IsExtendedComment();

public:
    TCHAR*                  m_pToken;

protected:
    IxmlGetChar*            m_pGetChar;
    TCHAR*                  m_pInsert;
    int                     m_maxLen;
    int                     m_tokAlloc;
    int                     m_tokLen;
    IxmlStreamSource*       m_pStream;
    txtParseState           m_txtPstate;
    txtParseState           m_preQuoteState;
    txtParseState           m_preCommentState;
    txtParseState           m_preEntityState;
    txtParseState           m_preHexState;
    txtParseState           m_preWhiteState;
    TCHAR                   m_hexVal;
    bool                    m_firstTagText;
    bool                    m_bHadElement;
    bool                    m_bRawByte;
    bool                    m_bSkipLT;
    TCHAR*                  m_pSvTag;
    TCHAR*                  m_pEntity;

    EString                 m_Namespace;
    EString                 m_OutStr;
    EString                 m_IndentStr;
    bool                    m_OpenElement;
    bool                    m_pretty;
    int                     m_level;
    bool                    m_bExtendedComment;
    TCHAR                   m_pEmptyTag[128];
    bool                    m_bQuotedTokenInBaseState;
};

#endif
