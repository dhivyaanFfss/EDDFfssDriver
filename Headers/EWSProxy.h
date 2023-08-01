#ifndef _E_WS_PROXY_H_
#define _E_WS_PROXY_H_

#include "EWSHelper.h"

//**************************************************************************
template<class T> class EWSProxy : public T
{
public:
    EWSProxy(EWSHelper *pHelper) :
        m_pHelper(pHelper)
    {
    }

    ~EWSProxy()
    {
        delete m_pHelper;
    }

    bool Setup(const EString& esModelNumber, const EString& esSerialNumber)
    {
        if (!m_pHelper->Setup(this, esModelNumber, esSerialNumber))
            return false;

        this->soap_endpoint = m_pHelper->GetEndpoint();

        m_deviceSessionInfo.Model = m_pHelper->GetModelNumber();
        m_deviceSessionInfo.Serial = m_pHelper->GetSerialNumber();
        m_deviceSessionInfo.Token = m_pHelper->GetToken();

        m_sessionInfo.SessionInfo = NULL;
        m_sessionInfo.DeviceSessionInfo = &m_deviceSessionInfo;

        this->soap_header(&m_sessionInfo);

        return true;
    }

    bool TearDown(void)
    {
        if (!m_pHelper->TearDown(this))
            return false;

        return true;
    }

private:
    EWSHelper *m_pHelper;
    ns2__SessionInfo m_userSessionInfo;
    ns2__DeviceSessionInfo m_deviceSessionInfo;
    ns2__UserOrDeviceSessionInfo m_sessionInfo;
};

//**************************************************************************
template<class T> class EWSProxyFactory
{
public:
    EWSProxyFactory(const EString& esWSName, IKernelBase *pIKernelBase) :
        m_esWSName(esWSName),
        m_pIKernelBase(pIKernelBase)
    {
    }

    EWSProxy<T> *CreateProxy(void)
    {
        EWSHelper *pHelper = EWSHelper::Create(m_esWSName, m_pIKernelBase);
        if (pHelper == NULL)
            return NULL;

        EWSProxy<T> *pProxy = new EWSProxy<T>(pHelper);
        if (!pHelper->CustomizeEnvironment(pProxy))
        {
            delete pProxy;
            delete pHelper;
            return NULL;
        }

        return pProxy;
    }

private:
    EString m_esWSName;
    IKernelBase *m_pIKernelBase;
};

#endif
