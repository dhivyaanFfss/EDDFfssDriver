//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EPersistenceTypes.h
//  
//  Subsystem  :  Kernel/Base
//
//  Description:  Types for implementing persistence mechanism
//
//  Classes:      EPersistentInt
//                EPersistentReal
//                EPersistentString
//                EPersistentPointerList
//
//**************************************************************************

// To define a persistent type, inherit from IPersistent, and add a ERestorePersistent function
// to the static ERestoreMap.


#ifndef __EPERSISTENCETYPES_H__
#define __EPERSISTENCETYPES_H__

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

/* Group=Persistence types*/

///////////////////////////////////////////////////////////////////////////////////////
//
// Persistent Integer class
//
class _EPERSIST_DECLSPEC EPersistentInt : public IPersistent
{
public:
    EPersistentInt();
    EPersistentInt(const EPersistentInt&);
	EPersistentInt(const int& intSrc){m_nValue = intSrc;};
	virtual ~EPersistentInt();
    const EPersistentInt& operator=(const EPersistentInt& intSrc);
	const EPersistentInt& operator=(const int& intSrc);
	operator int() const;

    // IPersistent Interface
    KERESULT Save(EHANDLE pData);
    KERESULT Restore(EHANDLE pData);
	KERESULT GetType(EString & type);
	KERESULT GetNewObject(IPersistent*& object);
    KERESULT DestroyObject()
    {
        delete this;
        return KE_OK;
    }

protected:
	int m_nValue;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Persistent Real class
//
class _EPERSIST_DECLSPEC EPersistentReal : public IPersistent
{
public:
    EPersistentReal();
	EPersistentReal(const EPersistentReal&);
	EPersistentReal(const double& realSrc){m_dValue = realSrc;};
	virtual ~EPersistentReal();
    const EPersistentReal& operator=(const EPersistentReal& realSrc);
    const EPersistentReal& operator=(const double& realSrc);
    const EPersistentReal& operator=(const float& realSrc);
	operator double() const;

    // IPersistent Interface
    KERESULT     Save(EHANDLE pData);
    KERESULT     Restore(EHANDLE pData);
	KERESULT GetType(EString & type);
	KERESULT GetNewObject(IPersistent*& object);
    KERESULT DestroyObject()
    {
        delete this;
        return KE_OK;
    }

protected:
	double m_dValue;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Persistent EString class
//
class  _EPERSIST_DECLSPEC EPersistentString : public IPersistent,
                                              public EString
{
public:
    EPersistentString();
	EPersistentString(const EPersistentString&);
	EPersistentString(const EString&);
	virtual ~EPersistentString();
    const EPersistentString& operator=(const EPersistentString& strSrc);
    const EPersistentString& operator=(const EString& strSrc);
    const EPersistentString& operator=(const TCHAR* strSrc);
    //    operator EString() const;
    // IPersistent Interface
    KERESULT Save(EHANDLE pData);
    KERESULT Restore(EHANDLE pData);
	KERESULT GetType(EString & type);
	KERESULT GetNewObject(IPersistent*& object);
    KERESULT DestroyObject()
    {
        delete this;
        return KE_OK;
    }
};

///////////////////////////////////////////////////////////////////////////////////////
//
// Persistent Pointer List class
//
class  _EPERSIST_DECLSPEC EPersistentPointerList : public IPersistent,
                                                   public EPtrList
{
public:
    EPersistentPointerList(int nBlockSize =10);
	EPersistentPointerList(const EPersistentPointerList&);
	virtual ~EPersistentPointerList();

    // IPersistent Interface
    KERESULT Save(EHANDLE pData);
    KERESULT Restore(EHANDLE pData);
	KERESULT GetType(EString & type);
	KERESULT GetNewObject(IPersistent*& object);
    KERESULT DestroyObject()
    {
        delete this;
        return KE_OK;
    }

    // Traverse list and remove all objects in it
    KERESULT CleanUpList(void);

	static bool isInited;

protected:
	static void InitPersistentMap();
};

#endif //__EPERSISTENCETYPES_H__




