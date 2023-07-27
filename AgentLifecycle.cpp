#include "pch.h"

KERESULT AgentLifecycle::Initialize()
{
    if (!m_isInit)
    {
        m_isInit = true;
        g_pEDDMgr->GetInterface(IID_RESTART, (void**)&m_pIRestart);

        if (m_pIRestart == NULL)
            return KE_FAILED;
    }

    return KE_OK;
}

KERESULT AgentLifecycle::RestartAgent()
{
    m_pIRestart->SetRestartFlag();
    m_pIRestart->Shutdown();

    return KE_OK;
}
