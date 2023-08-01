//**************************************************************************
//
//  Copyright © 2002-2014 Axeda Systems.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDeployConfig.h
//  
//  Subsystem  :  Deployment Conifguration
//
//  Description:  Declaration for EDeployConfig class that holds the
//                entire configuration for the agent.
//
//  Classes:      
//	              EDeployConfig
//
//**************************************************************************

#ifndef __EDEPLOY_CONFIG_H__
#define __EDEPLOY_CONFIG_H__

#ifdef WIN32
    #ifdef EDEPLOYCONFIG_EXPORTS
        #define EDEPLOYCONFIG_EXPORT __declspec(dllexport)
    #else
        #define EDEPLOYCONFIG_EXPORT __declspec(dllimport)
    #endif
#else
    #define EDEPLOYCONFIG_EXPORT
#endif

#include "ISax.h"
#include "IDeployConfig.h"
#include "IDPManager.h"

//**************************************************************************
class EDEPLOYCONFIG_EXPORT CustomSettings : public ETypedPtrMap<EMAP_STRING_TO_PTR, EString, IxmlToken*>
{
public:
    ~CustomSettings();
};

//**************************************************************************
class EDEPLOYCONFIG_EXPORT AddressRange
{
public:
    AddressRange(TCHAR *pszStart, TCHAR *pszEnd) :
        m_esStart(pszStart),
        m_esEnd(pszEnd) { }

    EString m_esStart;
    EString m_esEnd;
};

//**************************************************************************
class EDEPLOYCONFIG_EXPORT AddressRangeList : public ETypedPtrList<EPtrList, AddressRange*>
{
public:
    ~AddressRangeList()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            AddressRange *pRange = GetNext(pos);
            if (pRange)
                delete pRange;
        }
    }
};

//**************************************************************************
class EDEPLOYCONFIG_EXPORT EOldSNMPConfig
{
public:
    EOldSNMPConfig(void) :
        m_bBroadcast(false) { }

    void AddCommunity(TCHAR *pszCommunity)
    {
        if (m_communities.Find(EString(pszCommunity)) == -1)
            m_communities.Add(pszCommunity);
    }
    void Translate(CustomSettings& customSettings);

    EStringArray        m_communities;
    AddressRangeList    m_includeList;
    AddressRangeList    m_excludeList;
    bool                m_bBroadcast;
};

//**************************************************************************
class EDEPLOYCONFIG_EXPORT EDUCredentials
{
public:
    EString m_esUserName;
    EString m_esPassword;
};

//**************************************************************************
// EDeployConfig for storing all Configurations
class EDEPLOYCONFIG_EXPORT EDeployConfig
{
public:
    static EDeployConfig *GetInstance(IDPManager *pDPManager);
    virtual ~EDeployConfig();

    void   ClearOutSettings();
    void   CreateConnectionType(bool bDialup);
    void   CreateHTTPConfig();
    void   CreateDRMServer(bool bPrimary);
    void   CreateAutoSNMPSettings();
    void   CreateDeviceInfo();
    void   CreateManagedDeviceInfo();
    void   CreateOldSNMPConfig();
    void   CreateBindingSettings();
    PROXY_SERVER_INFO* CreateProxyInfo();
    APM_SERVER_INFO*   CreateAPMInfo();
    REMOTE_AUTH_INFO* GetRemoteAuthInfo();
    EDUCredentials* GetDUCredentials();

    // Loads deployment configuration from XML file and verifies the checksum,
    // if the checksum file is provided.
    virtual KERESULT LoadFromFile(const TCHAR *pszConfigFile,
        const TCHAR *pszChecksumFile = NULL) = 0;

    // Saves deployment configuration to XML file and generates the checksum,
    // if the checksum file is provided.
    virtual KERESULT SaveToFile(const TCHAR *pszConfigFile,
        const TCHAR *pszChecksumFile = NULL) = 0;

    // Saves deployment configuration provided as a string to XML file and
    // generates the checksum, if the checksum file is provided.
    virtual KERESULT SaveToFile(const EString& esConfigString,
        const TCHAR *pszConfigFile, const TCHAR *pszChecksumFile = NULL) = 0;

    // Returns true, if deployment configuration was loaded from an XML file in
    // legacy format, or false otherwise.
    virtual bool IsLegacy() = 0;

    // Generates XML output based on XML element objects created by SAX parser.
    // This method is designed to be called from ISax::OnElement().
    static void ProcessCustomElement(ESaxElement *pSubTreeRoot,
        ESaxElement *pCurElement, IxmlToken *pToken);

    // This class contains pointers to valid class info
    CONNECTION_INFO*    m_pConnectionType;
    HTTP_CONFIG*        m_pHTTPConfig;
    DRM_SERVER_INFO*    m_pDRMServer;
    DRM_SERVER_INFO*    m_pDRMBackupServer;
    AUTO_DISCOVERY_INFO* m_pAutoSNMPSettings;

    PROXY_SERVER_INFO*  m_pProxyInfo;
    APM_SERVER_INFO*    m_pApmInfo;
    SM_NUMBER*          m_pDeviceInfo;
    EMnInfos*           m_pMnInfos;
    CustomSettings      m_customSettings;
    EOldSNMPConfig*     m_pOldSNMPConfig;
    REMOTE_AUTH_INFO*   m_pRemoteAuthInfo;
    EDUCredentials*     m_pDUCredentials;
    BINDING_SETTINGS*   m_pBindingSettings;

protected:
    EDeployConfig();
};

#endif

