//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation Inc.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IAuthProxy.h
//
//  Description:  AuthProxy interface
//
//**************************************************************************

#define EAUTH_ATTR_USER         _T("user")
#define EAUTH_ATTR_PASSWORD     _T("password")
#define EAUTH_ATTR_METHOD       _T("method")
#define EAUTH_ATTR_AUTHPERIOD   _T("authPeriod")
#define EAUTH_ATTR_ENTERPRISE_USER _T("eUserId")
#define EAUTH_ATTR_X509_SUBJECT _T("x509subject")
#define EAUTH_ATTR_X509_ISSUER  _T("x509issuer")


//**************************************************************************
class EAuthContext
{
public:
    EAuthContext(void);
    bool        GetAttr(const EString& esName, EString& esValue);
    void        SetAttr(const EString& esName, const EString& esValue);
    soapMethod  *GetWSSecurity(void);
    void        SetWSSecurity(soapMethod *pWSSecurity);

private:
    EMapStringToString  m_attributes;
    soapMethod          *m_pWSSecurity;
};

//**************************************************************************
interface IAuthProxy
{
    virtual KERESULT Authenticate(EAuthContext& context) = 0;
    virtual KERESULT OpenSession(EAuthContext& context, const EString& esSessionId) = 0;
    virtual KERESULT CloseSession(EAuthContext& context, const EString& esSessionId) = 0;
    virtual KERESULT GetAuthPeriod(const EString& esMethod, int& iAuthPeriod) = 0;
    virtual bool     IsAuthEnabled() = 0;

};

//**************************************************************************
inline EAuthContext::EAuthContext(void) :
    m_pWSSecurity(NULL)
{
    m_attributes.InitHashTable(3);
}

//**************************************************************************
inline bool EAuthContext::GetAttr(const EString& esName, EString& esValue)
{
    return (m_attributes.Lookup(esName.GetPtr(), esValue) == TRUE);
}

//**************************************************************************
inline void EAuthContext::SetAttr(const EString& esName, const EString& esValue)
{
    m_attributes.SetAt(esName.GetPtr(), esValue.GetPtr());
}

//**************************************************************************
inline soapMethod *EAuthContext::GetWSSecurity(void)
{
    return m_pWSSecurity;
}

//**************************************************************************
inline void EAuthContext::SetWSSecurity(soapMethod *pWSSecurity)
{
    m_pWSSecurity = pWSSecurity;
}
