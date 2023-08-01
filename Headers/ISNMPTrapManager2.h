//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation Inc.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ISNMPTrapManager2.h
//  
//  Description:  SNMP trap manager interface
//
//**************************************************************************

//**************************************************************************
class ESNMPVarBind2
{
public:
    ESNMPVarBind2(void);

    EString     m_esOID;
    int         m_iType;
    CDataValue  m_value;
};

//**************************************************************************
class ESNMPVarBind2List : public ETypedPtrList<EPtrList, ESNMPVarBind2*>
{
public:
    ~ESNMPVarBind2List();
};

//**************************************************************************
class ESNMPTrapInfo2
{
public:
    ESNMPTrapInfo2(void);
    
    EString             m_esOID;
    EString             m_esSrcAddr;
    EString             m_esCommunity;
    ULONG               m_uptime;
    ESNMPVarBind2List   m_varBinds;
};

//**************************************************************************
// interface for SNMP trap handling
interface ISNMPTrapListener2
{
    virtual KERESULT OnTrap(const ESNMPTrapInfo2& trapInfo) = 0;
};

//**************************************************************************
// SNMP trap manager interface 
interface ISNMPTrapManager2
{
    virtual KERESULT Subscribe(ISNMPTrapListener2 *pHandler) = 0;
    virtual KERESULT Unsubscribe(ISNMPTrapListener2 *pHandler) = 0;
};

//**************************************************************************
inline ESNMPVarBind2::ESNMPVarBind2(void) :
    m_iType(0)
{
}

//**************************************************************************
inline ESNMPVarBind2List::~ESNMPVarBind2List()
{
    POSITION pos = GetHeadPosition();
    while (pos)
    {
        ESNMPVarBind2 *pVarBind = GetNext(pos);
        if (pVarBind)
            delete pVarBind;
    }
}

//**************************************************************************
inline ESNMPTrapInfo2::ESNMPTrapInfo2(void) :
    m_uptime(0UL)
{
}
