//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ERwLock.h
//  
//  Description:  read-write locking classes
//
//  Classes:    ERwLock, ERwLockRef, ERwLockPool
//
//**************************************************************************

#ifndef __ERWLOCK_H__
#define __ERWLOCK_H__

//**************************************************************************
class _DV_DECLSPEC ERwLock
{
public:
    enum Operation
    {
        SharedLock,
        ExclusiveLock,
        Upgrade,
        ExclusiveUpgrade,
        Downgrade,
        Unlock
    };

    ERwLock(void);
    virtual ~ERwLock();

    bool    Request(Operation op);

protected:
    bool            IsValidRequest(Operation op, ULONG iTid);
    void            AddSharedLock(ULONG iTid);
    void            RemoveSharedLock(ULONG iTid);
    void            SetExclusiveLock(ULONG iTid);
    void            RemoveExclusiveLock(void);
    int             GetThreadSharedLockCount(ULONG iTid);
    
    EMutex          m_mutex;
    ESingleEvent    m_event;
    int             m_iSharedLockCount;
    int             m_iUpgradeCount;
    bool            m_bExclusiveLock;
    ULONG           m_iExclusiveLockOwner;
    EMapPtrToPtr    m_sharedLockMap;
};

//**************************************************************************
class _DV_DECLSPEC ERwLockRef : public ERwLock
{
public:
    ERwLockRef(void);

    void    AddRef(void);
    void    Release(void);
    int     GetRefCount(void);

private:
    int     m_iRefCount;
};

//**************************************************************************
class _DV_DECLSPEC ERwLockPool
{
public:
    ERwLockPool(void);
    ~ERwLockPool();

    bool            Request(TCHAR *pId, ERwLock::Operation op);

private:
    EMutex          m_mutex;
    EMapStringToPtr m_locks;
};

//**************************************************************************
inline ERwLockRef::ERwLockRef(void) :
    m_iRefCount(0)
{
}

//**************************************************************************
inline int ERwLockRef::GetRefCount(void)
{
    return m_iRefCount;
}

#endif

