#ifndef _EPKI_H_
#define _EPKI_H_

#include "EWebClient.h"
#include "IDPManager.h"

// Forward Reference
interface ISecureStoreMgr;

#ifdef _WIN32
    #ifdef EPKI_EXPORTS
        #define EPKI_EXPORT __declspec(dllexport)
    #else
        #define EPKI_EXPORT __declspec(dllimport)
    #endif
#else
        #define EPKI_EXPORT
#endif

namespace EPKI
{
    //**************************************************************************
    enum Encoding
    {
        ENCODING_ASN1,
        ENCODING_PEM,
        ENCODING_AUTO
    };

    //**************************************************************************
    enum KeyAlgorithm
    {
        KEY_ALGORITHM_ANY,
        KEY_ALGORITHM_RSA,
        KEY_ALGORITHM_DSA
    };

    //**************************************************************************
    class EPKI_EXPORT Shared
    {
    public:
        virtual void AddRef(void);
        virtual void Release(void);
        virtual ELONG32 GetRefCount(void);

    protected:
        Shared(void);
        virtual ~Shared();

        ELONG32 m_iRefCount;
    };

    //**************************************************************************
    class EPKI_EXPORT KeyData
    {
    public:
        KeyData(const EString& esSource,
                bool bFile,
                Encoding encoding);

        const EString&  GetSource(void) const;
        bool            IsFile(void) const;
        Encoding        GetEncoding(void) const;

    protected:
        EString     m_esSource;
        bool        m_bFile;
        Encoding    m_encoding;
    };

    //**************************************************************************
    typedef KeyData CertificateData;

#if _MSC_VER < 1300
#pragma warning (disable:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT CertificateDataList : public ::ETypedPtrList<EPtrList, CertificateData*>
    {
    public:
        ~CertificateDataList();
    };

#if _MSC_VER < 1300
#pragma warning (default:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT PrivateKeyData : public KeyData
    {
    public:
        enum Format
        {
            FORMAT_ANY,
            FORMAT_TRADITIONAL,
            FORMAT_PKCS8
        };

        PrivateKeyData(const EString& esFile);
        PrivateKeyData(const EString& esSource,
                       bool bFile,
                       Format format);

        Format          GetFormat(void) const;
        void            SetPassword(EString& esPassword);
        const EString&  GetPassword() const;

    private:
        Format          m_format;
        EString         m_esPassword;
    };

    //**************************************************************************
    class EPKI_EXPORT IdentityData
    {
    public:
        IdentityData(const CertificateData *pCertificate,
                     const PrivateKeyData *pPrivateKey);
        ~IdentityData();

        const CertificateData   *GetCertificate(void) const;
        const PrivateKeyData    *GetPrivateKey(void) const;

    private:
        const CertificateData   *m_pCertificate;
        const PrivateKeyData    *m_pPrivateKey;
    };

    //**************************************************************************
    typedef KeyData CRLData;
    typedef KeyData OCSPResponseData;

    //**************************************************************************
    class EPKI_EXPORT RevocationCheckRule
    {
    public:
        enum Type
        {
            TYPE_CRL,
            TYPE_OCSP
        };

        RevocationCheckRule(Type type,
                            bool bRequisite,
                            bool bSufficient,
                            bool bEnforced,
                            bool bFailOnError,
                            long iRetryInterval,
                            long iMaxAge);
        virtual ~RevocationCheckRule();

        Type    GetType(void) const;
        bool    IsRequisite(void) const;
        bool    IsSufficient(void) const;
        bool    IsEnforced(void) const;
        bool    IsFailOnError(void) const;
        long    GetRetryInterval(void) const;
        long    GetMaxAge(void) const;

    protected:
        Type    m_type;
        bool    m_bRequisite;
        bool    m_bSufficient;
        bool    m_bEnforced;
        bool    m_bFailOnError;
        long    m_iRetryInterval;
        long    m_iMaxAge;
    };

#if _MSC_VER < 1300
#pragma warning (disable:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT RevocationCheckRuleList : public ::ETypedPtrList<EPtrList, RevocationCheckRule*>
    {
    public:
        ~RevocationCheckRuleList();
    };

#if _MSC_VER < 1300
#pragma warning (default:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT CRLCheckRule : public RevocationCheckRule
    {
    public:
        CRLCheckRule(bool bRequisite,
                     bool bSufficient,
                     bool bEnforced,
                     bool bFailOnError,
                     long iRetryInterval,
                     long iMaxAge,
                     const EString& esDefaultDP,
                     bool bOverrideDP);

        const EString&  GetDefaultDP(void) const;
        bool            IsOverrideDP(void) const;

    private:
        EString     m_esDefaultDP;
        bool        m_bOverrideDP;
    };

    //**************************************************************************
    class EPKI_EXPORT OCSPCheckRule : public RevocationCheckRule
    {
    public:
        OCSPCheckRule(bool bRequisite,
                      bool bSufficient,
                      bool bEnforced,
                      bool bFailOnError,
                      long iRetryInterval,
                      long iMaxAge,
                      const EString& esDefaultResponder,
                      bool bOverrideAIA);

        const EString&  GetDefaultResponder(void) const;
        bool            IsOverrideAIA(void) const;

    private:
        EString     m_esDefaultResponder;
        bool        m_bOverrideAIA;
    };

    //**************************************************************************
    class Container;
    class EPKI_EXPORT Profile : public Shared
    {
    public:
        Profile(void);

        KERESULT                        Load(const EString& esConfigFile, Container *pContainer);
        bool                            IsEmpty(void) const;
        const EString&                  GetName(void) const;
        void                            SetName(const EString& esName);
        const EString&                  GetCacheDir(void) const;
        void                            SetCacheDir(const EString& esCacheDir);
        const CertificateDataList       *GetTrust(void) const;
        void                            SetTrust(const CertificateDataList* pTrust);
        bool                            CreateTrust(EString& esSSLCertCAFile, Encoding encoding = ENCODING_PEM);
        const IdentityData              *GetIdentity(void) const;
        void                            SetIdentity(const IdentityData* pIdentity);
        bool                            CreateIdentity(EString& esSSLCertFile, Encoding encoding,
                                           EString& esSSLPrivateKeyFile,
                                           EString& esSSLPrivateKeyPassword);
        const RevocationCheckRuleList   *GetRevocationCheckRules(void) const;
        void                            SetRevocationCheckRules(const RevocationCheckRuleList *pRevocationCheckingRules);
        bool                            IsCheckHostname(void) const;
        void                            SetCheckHostname(bool bCheckHostname);
        const EString&                  GetCipherSuites(void) const;
        void                            SetCipherSuites(const EString& esCipherSuites);

        static Profile                  *GetClientDefault(const EString& esName, bool bInitTrust, ISecureStoreMgr* pIRsaMgr);
        static Profile                  *GetServerDefault(const EString& esName);

        static const TCHAR              *DEFAULT_CLIENT_TRUST;
        static const TCHAR              *DEFAULT_CLIENT_IDENTITY_CERTIFICATE;
        static const TCHAR              *DEFAULT_CLIENT_IDENTITY_PRIVATE_KEY;
        static const TCHAR              *DEFAULT_SERVER_IDENTITY_CERTIFICATE;
        static const TCHAR              *DEFAULT_SERVER_IDENTITY_PRIVATE_KEY;

    private:
        ~Profile();

        EString                         m_esName;
        EString                         m_esCacheDir;
        const CertificateDataList       *m_pTrust;
        const IdentityData              *m_pIdentity;
        const RevocationCheckRuleList   *m_pRevocationCheckRules;
        bool                            m_bCheckHostname;
        EString                         m_esCipherSuites;
    };

#if _MSC_VER < 1300
#pragma warning (disable:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT ProfileMap : public ::ETypedPtrMap<EMAP_STRING_TO_PTR, EString, Profile*>
    {
    public:
        ~ProfileMap();
    };

#if _MSC_VER < 1300
#pragma warning (default:4275)
#endif

    //**************************************************************************
    class EPKI_EXPORT ContainerParams
    {
    public:
        ContainerParams(bool bDebug);
        ~ContainerParams();

        bool    IsDebug(void) const;
        void    SetDebug(bool bDebug);

    private:
        bool    m_bDebug;
    };

    //**************************************************************************
    class EPKI_EXPORT Configuration
    {
    public:
        Configuration(void);
        ~Configuration();

        KERESULT        Load(const EString& esConfigFile, Container *pContainer);
        ContainerParams *GetContainerParams(void);
        Profile         *GetProfile(const EString& esName);
        void            AddProfile(Profile *pProfile);

    private:
        ContainerParams m_containerParams;
        ProfileMap      m_profiles;
    };

    //**************************************************************************
    class TrustManagerFactory;
    class EPKI_EXPORT Container
    {
    public:
        virtual ~Container() = 0;

        virtual DWORD               GetComponentId(void) = 0;
        virtual IKernelLog          *GetKernelLog(void) = 0;
        virtual bool                IsDebug(void) = 0;
        virtual bool                SetProxySettings(IWebUserAgent *pWebUA) = 0;
        virtual TrustManagerFactory *GetTrustManagerFactory(void) = 0;
        virtual IDPManager          *GetDPManager(void) = 0;
    };

    //**************************************************************************
    class EPKI_EXPORT Manager
    {
    public:
        Container       *GetContainer(void) const;
        const Profile   *GetProfile(void) const;
        bool            IsInitialized(void);

    protected:
        Manager(Container *pContainer,
                const Profile *pProfile);
        virtual ~Manager() = 0;

        Container       *m_pContainer;
        const Profile   *m_pProfile;
        bool            m_bInitialized;
    };

    //**************************************************************************
    class EPKI_EXPORT TrustManager : public Shared,
                                     public Manager
    {
    public:
        static TrustManager *GetInstance(Container *pContainer,
                                         const Profile *pProfile);
        
        virtual bool    Init(void) = 0;
        virtual bool    VerifyClient(const CertificateDataList *pChain) = 0;
        virtual bool    VerifyServer(const CertificateDataList *pChain) = 0;

    protected:
        TrustManager(Container *pContainer,
                     const Profile *pProfile);
        virtual ~TrustManager() = 0;
    };

    //**************************************************************************
    class EPKI_EXPORT TrustManagerFactory
    {
    public:
        virtual ~TrustManagerFactory() = 0;

        static TrustManagerFactory *GetInstance(void);

        virtual TrustManager    *GetTrustManager(Container *pContainer,
                                                 const Profile *pProfile) = 0;
        virtual void            RecycleTrustManager(TrustManager *pTrustManager) = 0;

    protected:
        TrustManagerFactory(void);
    };

    //**************************************************************************
    class EPKI_EXPORT SSLContext : public Shared,
                                   public Manager
    {
    public:
        static SSLContext *GetInstance(Container *pContainer,
                                       const Profile *pProfile);

        virtual bool    Init(bool bVerifyPeer) = 0;
        virtual void    *GetContext(void) = 0;

    protected:
        SSLContext(TrustManager *pTrustManager,
                   Container *pContainer,
                   const Profile *pProfile);
        virtual ~SSLContext() = 0;

        TrustManager    *m_pTrustManager;
    };
}

#endif
