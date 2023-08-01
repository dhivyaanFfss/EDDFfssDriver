//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  PersistenceInterface.h
//  
//  Subsystem  :  Kernel/Base
//
//  Description:  Types for implementing persistence mechanism
//
//  Classes:      IPersistent
//
//**************************************************************************
// To define a persistent type, inherit from IPersistent, and add a
// IRestorePersistent function to the static ERestoreMap.

#ifndef __PERSISTENCEINTERFACE_H__
#define __PERSISTENCEINTERFACE_H__


#define PERSISTED_DATA_ELEMENT _T("PersistedData")
#ifdef WIN32
    #ifdef _DV_EXPORT
       #define _EPERSIST_DECLSPEC __declspec(dllexport)
       #define _EPERSIST_EXTERN
    #else
       #define _EPERSIST_DECLSPEC __declspec(dllimport)
       #define _EPERSIST_EXTERN   extern
    #endif
#else
   #define _EPERSIST_DECLSPEC
   #define _EPERSIST_EXTERN
#endif

/* Group=Persistence */

//********************************************************************************
//
// IPersistent Interface
//
// Any object wanting to play in the persistence game must 
// implement this interface
//
interface IPersistent
{
    virtual KERESULT         Save(EHANDLE pData) = 0;
    virtual KERESULT         Restore(EHANDLE pData) = 0;
	virtual KERESULT         GetType(EString &) = 0;
	virtual KERESULT         GetNewObject(IPersistent*&) = 0;
	virtual KERESULT         DestroyObject() = 0;
};


//typedef IPersistent* (*ERestorePersistent)();

/*{Group=EPersistMap}*/
typedef  ETypedPtrMap<EMAP_STRING_TO_PTR, EString, IPersistent*> EPersistMap;

/*Group=Persistence defines*/
#define PERSIST_TAG 0xfeedbeef

interface IxmlToken;

/* Group=Persistence */
//********************************************************************************
//
// Persistence Utility Class
//
// This is used by objects which derive from IPersistent to implement persistence
//
class  _EPERSIST_DECLSPEC EPersistent
{
public:
    EPersistent();
    ~EPersistent();
   
    static  KERESULT Initialize(void);
    static  KERESULT Shutdown(void);
    static  KERESULT SaveSection(const TCHAR* pszModuleName, EHANDLE& hPersist);
    // bConfigFile added 3.1 to allow non Builder generated XML files.
    // When bConfigFile == false, use pszModuleName as the full path.
    static  KERESULT RestoreSection(const TCHAR* pszModuleName, EHANDLE& hPersist,
                                    bool bConfigFile = true);
    static  KERESULT CloseSection(EHANDLE hPersist);
    static  KERESULT AddFactory(IPersistent* builder);
    static  KERESULT GetFactory(TCHAR* type, IPersistent*& factory);
    static  KERESULT Comment(const TCHAR* pComment, EHANDLE pData);
    // Drivers should use IKernelShell SetProjectPath & GetProjectPath.  See KernelInteface.h.
    static  KERESULT SetProjectPath(TCHAR* pPath);
    static  KERESULT GetProjectPath(EString& rPath);
    static  EPersistent* GetEPersistent(EHANDLE& hPersist) 
                {return (EPersistent*) hPersist;}
	
	
public:
    bool            isBuildingFile(void) { return !m_Restoring; };
	void            SetVersion(int nVersion) { m_nVersion = nVersion;}
	int             GetVersion(void)          { return m_nVersion;}
	void            SetTerse(bool bTerse) { m_bTerse = bTerse;}
	bool            GetTerse(void)        { return m_bTerse;}
    IxmlToken*      GetIxmlToken(void)    { return (IxmlToken*) m_pToken; }
	
public:
    void*           m_pToken;
    DWORD           m_Tag;
    static  KERESULT GetSectionFilename(const TCHAR* pszModuleName, EString& fileName, DWORD& pos);
protected:
    // a string keyed map of restore functions for registered persistent types
	static EMapStringToPtr   m_ERestoreMap;
    static bool     m_bPathCreated;
    static EString  m_strProjectPath;
protected:
    bool            m_Restoring;
	int             m_nVersion;
	bool            m_bTerse;
};

#endif //__PERSISTENCEINTERFACE_H__




