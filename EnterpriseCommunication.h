#pragma once

#ifndef __ENTERPRISE_COMM_H__
#define __ENTERPRISE_COMM_H__

class EnterpriseCommunication
{
public:

    KERESULT Initialize();
    KERESULT GetServerDiagnosis(EString& esDiagnosis);

private:
    bool m_isInit = false;
    IEnterpriseProxy* m_pIEnterpriseProxy;
};

#endif
