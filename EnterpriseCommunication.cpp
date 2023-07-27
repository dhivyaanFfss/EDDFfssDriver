#include "pch.h"

KERESULT EnterpriseCommunication::Initialize()
{
    if (!m_isInit)
    {
        m_isInit = true;
        g_pEDDMgr->GetInterface(IID_ENTERPRISE_PROXY, (void**)&m_pIEnterpriseProxy);

        if (m_pIEnterpriseProxy == NULL)
            return KE_FAILED;
    }

    return KE_OK;
}

KERESULT EnterpriseCommunication::GetServerDiagnosis(EString& esDiagnosis)
{
    return m_pIEnterpriseProxy->GetServerDiagnosis(esDiagnosis);
}
