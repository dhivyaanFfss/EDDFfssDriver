#ifndef _EDIAG_H_
#define _EDIAG_H_

#define EDIAG_ELEMENT_GROUP     _T("Group")
#define EDIAG_ELEMENT_CONDITION _T("Condition")
#define EDIAG_ELEMENT_PARAM     _T("Param")
#define EDIAG_ATTR_ID           _T("id")
#define EDIAG_ATTR_NAME         _T("name")
#define EDIAG_ATTR_SUMMARY      _T("summary")
#define EDIAG_ATTR_DESCRIPTION  _T("description")

//**************************************************************************
class _DV_DECLSPEC EConditionParams : public EMapStringToString
{
public:
    EConditionParams(void);
};

//**************************************************************************
class _DV_DECLSPEC ECondition
{
public:
    ECondition(TCHAR *pszGroupId, TCHAR *pszId);
    ECondition(TCHAR *pszGroupId, int iId);

    EString             m_esGroupId;
    EString             m_esId;
    EConditionParams    m_params;
};

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC _ETypedPtrList<EPtrList, ECondition*>;
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrList<EPtrList, ECondition*>;
#endif

//**************************************************************************
class _DV_DECLSPEC EConditionList : public ETypedPtrList<EPtrList, ECondition*>
{
public:
    ~EConditionList();

    void Clear(void);
};

//**************************************************************************
class _DV_DECLSPEC EDiagnosis
{
public:
    ~EDiagnosis();

    void    AddCondition(ECondition *pCondition);
    void    Reset(void);

    EConditionList  m_conditions;
};

//**************************************************************************
class _DV_DECLSPEC EDiagInfo
{
public:
    void    GetText(EString& esText);

    EString m_esSummary;
    EString m_esDescription;
};

//**************************************************************************
class _DV_DECLSPEC EConditionDict : public EDiagInfo
{
public:
    EConditionParams    m_params;
};

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrMap<EMAP_STRING_TO_PTR, EString, EConditionDict*>;
#endif

//**************************************************************************
class _DV_DECLSPEC EConditionDictMap : public ETypedPtrMap<EMAP_STRING_TO_PTR, EString, EConditionDict*>
{
public:
    ~EConditionDictMap();
};

//**************************************************************************
class _DV_DECLSPEC EConditionGroupDict : public EDiagInfo
{
public:
    EConditionDictMap   m_conditions;
    EConditionParams    m_params;
};

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrMap<EMAP_STRING_TO_PTR, EString, EConditionGroupDict*>;
#endif

//**************************************************************************
class _DV_DECLSPEC EConditionGroupDictMap : public ETypedPtrMap<EMAP_STRING_TO_PTR, EString, EConditionGroupDict*>
{
public:
    ~EConditionGroupDictMap();
};

//**************************************************************************
class _DV_DECLSPEC EDiagDict : public ISax
{
public:
    bool        Restore(TCHAR *pszFile);
    void        GetText(EDiagnosis& diagnosis, EString& esText);

    KERESULT    OnElement(ESaxElement* pElement);

private:
    KERESULT    GetConditionGroupDict(ESaxElement* pElement, EConditionGroupDict*& pGroup);
    KERESULT    GetConditionDict(ESaxElement* pElement, EConditionDict*& pCondition);

public:
    EConditionGroupDictMap  m_groups;
};

//**************************************************************************
inline EConditionParams::EConditionParams(void)
{
    InitHashTable(3, TRUE);
}

//**************************************************************************
inline ECondition::ECondition(TCHAR *pszGroupId, TCHAR *pszId) :
    m_esGroupId(pszGroupId),
    m_esId(pszId)
{
}

//**************************************************************************
inline ECondition::ECondition(TCHAR *pszGroupId, int iId) :
    m_esGroupId(pszGroupId)
{
    m_esId.Format(_T("%d"), iId);
}

//**************************************************************************
inline void EDiagnosis::AddCondition(ECondition *pCondition)
{
    m_conditions.AddTail(pCondition);
}

#endif
