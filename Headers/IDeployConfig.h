//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IDeployConfig.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Defines the IDeployConfigInterface;
//
//
//**************************************************************************

#ifndef __IDEPLOY_CONFIG_H__
#define __IDEPLOY_CONFIG_H__

#include "EUtility.h"
#include "IDiscover.h"
#include "EWebClient.h"

/////////////////////////////////////////////////////////////////////////////
//
// Various classes used by Deployment Utility
//
/////////////////////////////////////////////////////////////////////////////

// Connection type
class CONNECTION_INFO
{
public:
    CONNECTION_INFO() :
        m_bUseDialup(false),
        m_iPortNumber(1),
        m_iPortSpeed(57600),
        m_iRetryTimeout(30000),
        m_iRetryCount(3){}
    ~CONNECTION_INFO(){}
    CONNECTION_INFO& operator=(CONNECTION_INFO& info)
    {
        if (this == &info)
            return *this;
        m_bUseDialup     = info.m_bUseDialup;
        m_PhoneNumber    = info.m_PhoneNumber;
        m_Domain         = info.m_Domain;
        m_UserName       = info.m_UserName;
        m_Password       = info.m_Password;

        m_iPortNumber    = info.m_iPortNumber;
        m_iPortSpeed     = info.m_iPortSpeed;
        m_InitString     = info.m_InitString;
        m_DialPrefix     = info.m_DialPrefix;
        m_iRetryTimeout  = info.m_iRetryTimeout;
        m_iRetryCount    = info.m_iRetryCount;

        return *this;
    }


    bool    m_bUseDialup;
    // If Dialup
    EString m_PhoneNumber;
    EString m_Domain;           // Not used with Linux
    EString m_UserName;
    EString m_Password;

    // Linux-specific dialup parameters
    int     m_iPortNumber;
    int     m_iPortSpeed;
    EString m_InitString;
    EString m_DialPrefix;

    int     m_iRetryTimeout;        // in milli-seconds
    int     m_iRetryCount;
};

// HTTP configuration
class HTTP_CONFIG
{
public:
    HTTP_CONFIG() :
        m_iTimeOut(300), m_bOptimize(true), m_bPersistentConnection(true)
        {}
    ~HTTP_CONFIG() {}
    HTTP_CONFIG& operator=(HTTP_CONFIG& config)
    {
        if (this == &config)
            return *this;

        m_iTimeOut = config.m_iTimeOut;
        m_bOptimize = config.m_bOptimize;
        m_bPersistentConnection = config.m_bPersistentConnection;

        return *this;
    }

    int     m_iTimeOut;
    bool    m_bOptimize;
    bool    m_bPersistentConnection;   // only ued if bOptimize = false.
};

// SSL Info.  Used with DRM_SERVER_INFO (primary only), and 
class SSL_INFO
{
public:
    SSL_INFO() : m_bCertificateRequired(false), m_encryptionLevel(EWebCryptoNone),
                 m_bCertificateSupported(false), m_bSSLEnabled(false),
                 m_bUseCipherSuites(false) {;}
    SSL_INFO& operator=(SSL_INFO& info)
    {
        if (this == &info)
            return *this;
        m_bCertificateRequired = info.m_bCertificateRequired;
        m_bCertificateSupported = info.m_bCertificateSupported;
        m_encryptionLevel = info.m_encryptionLevel;
        m_bSSLEnabled = info.m_bSSLEnabled;
        m_esPKIProfile = info.m_esPKIProfile;
        m_esCACertFile = info.m_esCACertFile;
        m_bUseCipherSuites = info.m_bUseCipherSuites;
        m_esCipherSuites = info.m_esCipherSuites;
        return *this;
    }

    // properties
    bool            m_bCertificateRequired;
    bool            m_bCertificateSupported;
    bool            m_bSSLEnabled;
    EWebCryptoLevel m_encryptionLevel;
    EString         m_esPKIProfile;
    EString         m_esCACertFile;
    bool            m_bUseCipherSuites;
    EString         m_esCipherSuites;
};

// DRM Server      
class DRM_SERVER_INFO : public SSL_INFO
{
public:
    DRM_SERVER_INFO() {m_DRMServerPort = 0; m_EncryptionType = DEPLOY_ENCRYPTION_TYPE_NONE;
    m_bAESEnabled= false; }
    ~DRM_SERVER_INFO(){}
    DRM_SERVER_INFO& operator=(DRM_SERVER_INFO& info)
    {
        if (this == &info)
            return *this;
        (SSL_INFO&) (*this) = (SSL_INFO&) info;
        m_DRMServerAddress     = info.m_DRMServerAddress;
        m_DRMServerPort        = info.m_DRMServerPort;
        m_DRMServerDatabase    = info.m_DRMServerDatabase;
        m_EncryptionType       = info.m_EncryptionType;
        m_bAESEnabled          = info.m_bAESEnabled;
        return *this;
    }

    // Encryption type with DRM server. Don't change the values. -- HF
    enum EncryptionType
    {
        DEPLOY_ENCRYPTION_TYPE_NONE = 0, 
        DEPLOY_ENCRYPTION_TYPE_BLOWFISH = 1, 
        DEPLOY_ENCRYPTION_TYPE_SSL = 2,
        DEPLOY_ENCRYPTION_TYPE_AES = 3
    };

    EString m_DRMServerAddress;
    int     m_DRMServerPort;
    EString m_DRMServerDatabase;
    int     m_EncryptionType;  
    // m_bAESEnabled only set by EnterpriseProxy (not read).  
    // Used by DU UI for valid selection of encryption choices.
    bool    m_bAESEnabled; 
};

// Proxy Server
class PROXY_SERVER_INFO
{
public:
    PROXY_SERVER_INFO() {m_HTTPProxyPort = 0; m_SOCKSProxyPort = 0;
        m_bPersistentConnection = false;}
    ~PROXY_SERVER_INFO(){}
    PROXY_SERVER_INFO& operator=(PROXY_SERVER_INFO& info)
    {
        if (this == &info)
            return *this;

        m_HTTPProxyAddress     = info.m_HTTPProxyAddress;
        m_HTTPProxyPort        = info.m_HTTPProxyPort;
        m_HTTPProxyUserName    = info.m_HTTPProxyUserName;
        m_HTTPProxyPassword    = info.m_HTTPProxyPassword;

        m_SOCKSProxyAddress    = info.m_SOCKSProxyAddress;
        m_SOCKSProxyPort       = info.m_SOCKSProxyPort;
        m_SOCKSProxyUserName   = info.m_SOCKSProxyUserName;
        m_SOCKSProxyPassword   = info.m_SOCKSProxyPassword;

        m_PACURL               = info.m_PACURL;
        return *this;
    }

    // true if persistent connection should be used. note that this parameter
    // is not controlled by the Deployment Utility. the member is used for communication
    // through IEnterpriseProxy::GetProxyServerInfo(). should not be here.
    bool    m_bPersistentConnection;

    // HTTP Proxy Info
    EString m_HTTPProxyAddress;
    int     m_HTTPProxyPort;
    EString m_HTTPProxyUserName;
    EString m_HTTPProxyPassword;

    // SOCKS Proxy Info
    EString m_SOCKSProxyAddress;
    int     m_SOCKSProxyPort;
    EString m_SOCKSProxyUserName;
    EString m_SOCKSProxyPassword;

    // PAC URL
    EString m_PACURL;
};

// Serial number and Model number
class SM_NUMBER
{
public:
    SM_NUMBER() {}
    ~SM_NUMBER(){}
    SM_NUMBER& operator=(SM_NUMBER& info)
    {
        if (this == &info)
            return *this;
        m_Model = info.m_Model;
        m_Serial =info.m_Serial;

        return *this;
    }

    EString m_Serial;
    EString m_Model;
};

// Structure to hold binding settings 
class BINDING_SETTINGS 
{
public:
	BINDING_SETTINGS() { m_LoopbackOnly = false; }
	~BINDING_SETTINGS() {}
	BINDING_SETTINGS& operator=(BINDING_SETTINGS& settings)
	{
		if(this == &settings) 
			return *this;

		m_LoopbackOnly = settings.m_LoopbackOnly;
		return *this;
	}

	bool m_LoopbackOnly;
};

// SNMP Settings
class SNMP_SETTINGS
{
public:
    SNMP_SETTINGS()     {m_Flags = SNMP_NONE; m_Broadcast = false;}
    virtual ~SNMP_SETTINGS()
    {
        EUtility::ClearIPRange(m_Include);
        EUtility::ClearIPRange(m_Exclude);
    }
    SNMP_SETTINGS& operator=(SNMP_SETTINGS& info)
    {
        if (this == &info)
            return *this;
        m_Flags        = info.m_Flags;
        m_Broadcast    = info.m_Broadcast;

        int i;
        EUtility::ClearIPRange(m_Include);
        EUtility::ClearIPRange(m_Exclude);

        for(i = 0; i < info.m_Include.GetSize(); i++)
        {
            IPRange* pcur = info.m_Include[i];
            IPRange* pNew = new IPRange(pcur);
            m_Include.Add(pNew);
        }

        for(i = 0; i < info.m_Exclude.GetSize(); i++)
        {
            IPRange* pcur = info.m_Exclude[i];
            IPRange* pNew = new IPRange(pcur);
            m_Exclude.Add(pNew);
        }

        return *this;
    }

    // Which part is valid
    enum
    {
        SNMP_NONE           = 0X0000, 
        SNMP_BROADCAST_ON   = 0X0001, 
        SNMP_INCLUDE_ON     = 0X0002,
        SNMP_EXCLUDE_ON     = 0X0004,
        SNMP_ALL_ON         = 0X0007
    };

    DWORD           m_Flags;
    bool            m_Broadcast;
    IPRangeArray    m_Include;
    IPRangeArray    m_Exclude;
};

// Auto-discovery settings
class AUTO_DISCOVERY_INFO
{
public:
    AUTO_DISCOVERY_INFO(void) { m_bEnabled = true; m_scanRate = 0; }

    AUTO_DISCOVERY_INFO& operator=(AUTO_DISCOVERY_INFO& info)
    {
        m_bEnabled = info.m_bEnabled;
        m_scanRate = info.m_scanRate;

        return *this;
    }

    bool    m_bEnabled;
    DWORD   m_scanRate;
};

// APM Setting
class APM_SERVER_INFO : public PROXY_SERVER_INFO,
                        public SSL_INFO
{
public:
    APM_SERVER_INFO()
        :m_port(0), m_bEnabled(false), m_bLPMEnabled(false), m_bOnline(false)
    {
    }
    ~APM_SERVER_INFO()  {}
    APM_SERVER_INFO& operator=(APM_SERVER_INFO& info)
    {
        if (this == &info)
            return *this;
        (SSL_INFO&) (*this) = (SSL_INFO&) info;
        (PROXY_SERVER_INFO&) (*this) = (PROXY_SERVER_INFO&) info;
        m_port = info.m_port;
        m_url = info.m_url;
        m_bEnabled = info.m_bEnabled;
        m_bOnline = info.m_bOnline;
        return *this;
    }

    int     m_port;
    EString m_url;
    bool    m_bOnline;          // state of APM connection.
    bool    m_bEnabled;         // allows deployment utility to disable APM.
    bool    m_bLPMEnabled;
};


class RemoteAuthProviderSettings
{
public:
  RemoteAuthProviderSettings(EString name): m_bEnabled(false), m_AuthProviderName(name) {};
  ~RemoteAuthProviderSettings() {};

  bool IsEnabled() { return m_bEnabled; }
  void Enable() { m_bEnabled = true; }
  void Disable() { m_bEnabled = false; }
  EString GetName() { return m_AuthProviderName; }
  
  RemoteAuthProviderSettings(const RemoteAuthProviderSettings& rhs)
      : m_bEnabled(rhs.m_bEnabled), m_AuthProviderName(rhs.m_AuthProviderName)
  {
      copy_settings(rhs.m_Settings);
  }

  RemoteAuthProviderSettings& operator=(const RemoteAuthProviderSettings& rhs)
  {
    if (this == &rhs)
      return *this;
      
    m_bEnabled = rhs.m_bEnabled;
    m_AuthProviderName = rhs.m_AuthProviderName;

    copy_settings(rhs.m_Settings);
    return *this;
  }
  
  void Add(EString name, EString value)
  {
    m_Settings[name] = value;
  }
  
  bool GetSetting(EString name, EString& value)
  {
    return m_Settings.Lookup(name, value) ? true : false;
  }

  POSITION GetFirstSetting() const
  {
    return m_Settings.GetStartPosition();      
  }
  
  POSITION GetNextSetting(POSITION pos, EString& name, EString& value) const
  {
      m_Settings.GetNextAssoc(pos, name, value);
      return pos;
  }
  
private:

  void copy_settings(const EMapStringToString& rhs)
  {
    m_Settings.RemoveAll();
    POSITION pos = rhs.GetStartPosition();
    while (pos)
    {
      EString name, value;
      rhs.GetNextAssoc(pos, name, value);
      m_Settings[name] = value;
    }
  }

  bool m_bEnabled;
  EString m_AuthProviderName; // e.g. RADIUS, LDAP, OCSP
  EMapStringToString m_Settings;

};


class REMOTE_AUTH_INFO 
{
public:
  REMOTE_AUTH_INFO () : m_AuthSettings(), m_bMustAudit(true) {};
  ~REMOTE_AUTH_INFO () 
  {
    clear();
  };

  REMOTE_AUTH_INFO& operator=(const REMOTE_AUTH_INFO& rhs)
  {
    if (this == &rhs)
      return *this;
      
    m_bMustAudit = rhs.m_bMustAudit;

    clear();
    POSITION pos = rhs.m_AuthSettings.GetStartPosition();
    while (pos)
    {
      EString type;
      RemoteAuthProviderSettings *pAuthProviderSettings = 0;
      rhs.m_AuthSettings.GetNextAssoc(pos, type, pAuthProviderSettings);

      m_AuthSettings[type] = new RemoteAuthProviderSettings(*pAuthProviderSettings);
    }

    return *this;
  }
  
  void clear()
  {
    POSITION pos = m_AuthSettings.GetStartPosition();
    while (pos)
    {
      EString type;
      RemoteAuthProviderSettings *pAuthProviderSettings = 0;
      m_AuthSettings.GetNextAssoc(pos, type, pAuthProviderSettings);
      delete pAuthProviderSettings;
    }
  }
  
  RemoteAuthProviderSettings* GetAuthProviderSettings(EString method)
  {
    RemoteAuthProviderSettings* pSettings = 0;
    m_AuthSettings.Lookup(method, pSettings);
    if (pSettings == 0)    
    {
      pSettings = new RemoteAuthProviderSettings(method);
      m_AuthSettings[method] = pSettings;
    }
    return pSettings;
  }
  
  POSITION GetFirstProvider() const
  {
    return m_AuthSettings.GetStartPosition();
  }
  
  POSITION GetNextProvider(POSITION pos, RemoteAuthProviderSettings*& pAuth) const
  {
    EString key;
    m_AuthSettings.GetNextAssoc(pos, key, pAuth);
    return pos;
  }
  
  void EnableAuditing() 
  {
    m_bMustAudit = true;
  }

  void DisableAuditing() 
  {
    m_bMustAudit = false;
  }
  
  bool IsAuditingEnabled() 
  {
    return m_bMustAudit;
  }
  
private:
  ETypedPtrMap<EMAP_STRING_TO_PTR, EString, RemoteAuthProviderSettings*> m_AuthSettings;
  bool m_bMustAudit;
};

// Custom Settings Provider interface
interface ICustomSettingsProvider
{
    virtual KERESULT GetCustomSettings(TCHAR *pszId, IxmlToken*& pToken) = 0;
};

// IDeployConfigInterface
interface IDeployConfigInterface
{
    virtual KERESULT GetDRMServerInfo(bool bPrimary, DRM_SERVER_INFO*) = 0;
    virtual KERESULT GetConnectionType(CONNECTION_INFO*) = 0;
    virtual KERESULT GetProxyServerInfo(PROXY_SERVER_INFO*) = 0;
    virtual KERESULT GetSMNumber(SM_NUMBER*) = 0;
    virtual KERESULT GetSNMPSettings(SNMP_SETTINGS*) = 0;
    virtual KERESULT GetAPMSettings(APM_SERVER_INFO*) = 0;
    virtual KERESULT GetSNMPCommunities(EStringArray& communities) = 0;
    // Ping mode
    virtual KERESULT SetPingMode(bool bPingMode) = 0;
    // Gateway device configuration.  
    // This allows the Deployment Utility the ability to over-ride all 
    // statically (non-auto-discovered) devices.
    // Return KE_FALSE if device information is not configured by the D.U.
    virtual KERESULT GetDevices(EMnInfos& mnInfos, ETime& configTime) = 0;

    virtual KERESULT GetHTTPConfig(HTTP_CONFIG*) = 0;

    virtual KERESULT GetCustomSettings(TCHAR *pszId, IxmlToken*& pToken) = 0;

    virtual KERESULT GetAutoDiscoveryInfo(TCHAR* pszPlugInName, AUTO_DISCOVERY_INFO *pInfo) = 0;
    virtual KERESULT GetRemoteAuthInfo(REMOTE_AUTH_INFO*) = 0;

    virtual KERESULT RegisterCustomSettingsProvider(TCHAR *pszId, ICustomSettingsProvider *pProvider) = 0;
    virtual KERESULT UnregisterCustomSettingsProvider(TCHAR *pszId) = 0;
    virtual KERESULT GetBindingSettings(BINDING_SETTINGS *) = 0;
};

// IDeployConfigManager interface
interface IDeployConfigManager
{
    virtual KERESULT LoadConfig() = 0;
};

#endif // __IDEPLOY_CONFIG_H__


