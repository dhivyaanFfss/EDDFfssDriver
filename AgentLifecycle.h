#pragma once
#ifndef _AGENT_LIFECYCLE_
#define _AGENT_LIFECYCLE_


class AgentLifecycle
{
public:

    KERESULT Initialize();
    KERESULT RestartAgent();

private:
    bool m_isInit = false;
    IRestart* m_pIRestart;
};

#endif // !_AGENT_LIFECYCLE_