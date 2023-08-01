//**************************************************************************
//
//  Copyright (c) 1999-2009 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ISMController.h
//  
//  Description:  Interface for communication with SMController component
//
//  Interface  :  ISMController
//              
//**************************************************************************
#ifndef __I_SM_CONTROLLER_H__
#define __I_SM_CONTROLLER_H__

//**************************************************************************
class EPackageInfo
{
public:
    EPackageInfo(void)
    {
    };

    EPackageInfo(const EPackageInfo& x)
    {
        *this = x;
    };

    EPackageInfo& operator=(const EPackageInfo& packageInfo)
    {
        if (this == &packageInfo)
            return *this;

        m_esId = packageInfo.m_esId;
        m_esName = packageInfo.m_esName;
        m_esVersion = packageInfo.m_esVersion;
        m_esDescription = packageInfo.m_esDescription;

        return *this;
    }

    const EString& GetId(void) const
    {
        return m_esId;
    }

    void SetId(const EString& esId)
    {
        m_esId = esId;
    }

    const EString& GetName(void) const
    {
        return m_esName;
    }

    void SetName(const EString& esName)
    {
        m_esName = esName;
    }

    const EString& GetVersion(void) const
    {
        return m_esVersion;
    }

    void SetVersion(const EString& esVersion)
    {
        m_esVersion = esVersion;
    }

    const EString& GetDescription(void) const
    {
        return m_esDescription;
    }

    void SetDescription(const EString& esDescription)
    {
        m_esDescription = esDescription;
    }

protected:
    EString m_esId;
    EString m_esName;
    EString m_esVersion;
    EString m_esDescription;
};

//**************************************************************************
enum EPackageDeploymentMode
{
    pdmFULL,
    pdmDOWNLOAD_STAGE,
    pdmFINAL_STAGE
};

//**************************************************************************
interface ISMController
{
    virtual KERESULT    ProcessConfigRequest(ISoapParser *pIParser, soapMethod *pRequestMethod) = 0;
    virtual KERESULT    GetConfigResponse(ISoapParser *pIParser, soapMethod *pResponseMethod) = 0;
    virtual KERESULT    ProcessPackageStart(const EString& esPackageId, int iDeviceId, EString& esSOAPMessage, EPackageDeploymentMode& pdm) = 0;
    virtual KERESULT    GetPackageStatuses(EPackageStatusDataList& packageStatuses) = 0;
    virtual KERESULT    CheckNewPackages(bool bQueryServer, int iDeviceId, bool& bResult) = 0;
    virtual KERESULT    GetPackageInfo(const EString& esPackageId, EPackageInfo& packageInfo) = 0;
    virtual KERESULT    DownloadPackages(const EStringArray& packageIds, int iDeviceId) = 0;
    virtual KERESULT    ExecutePackages(const EStringArray& packageIds, int iDeviceId) = 0;
    virtual KERESULT    GetPackageStatusUpdates(int iTimeout, EPackageStatusDataList& packageStatuses) = 0;
    virtual KERESULT    RemovePackageStatuses(const EStringArray& packageIds, int iDeviceId) = 0;
};

#endif
