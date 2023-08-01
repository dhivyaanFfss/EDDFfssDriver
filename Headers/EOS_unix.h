//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  eos_linux.h
//  
//  Subsystem  :  Kernel
//
//  Description:  linux platform support
//
//
//**************************************************************************

#ifndef _EOS_LINUX_H
#define _EOS_LINUX_H

extern char** environ;

#define PATH_DELIMITER_CHAR     '/'
#define PATH_DELIMITER_STRN     _T("/")

class _DV_DECLSPEC ESharedMemory
{
public:
	ESharedMemory(TCHAR *name, ULONG size);
	~ESharedMemory();
	
	DWORD AddToMem(void* pBuf, ULONG size, ULONG timeout = INFINITE);
	DWORD GetFromMem(void* pBuf, ULONG size, ULONG timeout = INFINITE);

private:
	void	LockMem();
	void	UnLockMem();
	int				m_Name;
	int				m_hSharedMemMutex;
	HANDLE				m_hSharedMemEvent;
	int				m_hSharedMemId;
	ECircularBuffer*		m_pSircleBuffer;
	UCHAR*				m_pSharedBuffer;
	ULONG				m_Size;
};

class _DV_DECLSPEC EProcess
{
public:
    static DWORD _GetCurrentProcessId();
    static DWORD WaitForPid(DWORD pid,DWORD milliTimeout);
    static BOOL KillPid(DWORD pid);
};

//**************************************************************************
// pthreads implementation
//**************************************************************************
class EMutexWrapper
{
    friend class EMutex;
public:
    EMutexWrapper();
    
    ~EMutexWrapper();
    
    void     Lock();
    void     Unlock();
    
private:
    pthread_mutex_t* m_mutex;
};

// Group=OS Wrappers
//**************************************************************************
class _DV_DECLSPEC EMutex
{
    friend class ESingleEvent;

public:
    EMutex(TCHAR* pDesc = _T("Unknown"));

    void Lock(TCHAR* pCaller = _T("Unknown"));

    void Unlock(TCHAR* pCaller = _T("Unknown"));

    pthread_mutex_t* GetMutexRef() { return m_mtxWrapper.m_mutex; }

private:
    EMutexWrapper   m_mtxWrapper;

#ifdef DEBUG_DEADLOCK
    TCHAR           m_mtxDesc[50];
#endif

};

class ESingleEvent
{
public:
    ESingleEvent();
    ~ESingleEvent();
    bool operator!();
    
    bool     Set();
    bool     Reset();
    DWORD    Wait(DWORD dwMilliseconds = INFINITE);
    
private:
    EMutex          m_Emutex;
    pthread_cond_t  m_condition;
    bool            m_bSignaled;
};

//**************************************************************************
typedef void *  ThreadReturn;
typedef ThreadReturn (* FunctionPtr) (void*);

class EThread
{
public:
    EThread(DWORD Size = 4096,bool bCurrent = false);
    ~EThread(void);
    bool            Begin(FunctionPtr PFN, void *X, TCHAR* pChar = NULL);
    void            Wait(void);
    pthread_t       ThreadId(void) {return m_threadId;}
    bool            SetPriority(long priority);
    long            GetPriority();
    bool            IsRunning() {return m_threadId != 0;}
    bool            Kill();  
    static void     Yield();
    static pthread_t    _GetCurrentThreadID();    
    static void     ContextSwitch();
    void operator= (EThread&);

    FunctionPtr     GetFunction(void) { return m_pFunction; }
    void            *GetArg() { return m_pArg; }
    void            Reset() { m_threadId = 0; }

private:
    void            Cleanup(void);

    pthread_t       m_threadId;
    FunctionPtr     m_pFunction;
    void            *m_pArg;

    static ThreadReturn ThreadStub(void* pParam);
};


DWORD GetTickCount(void);
void* ETaskMemAlloc(ULONG cb );
void ETaskMemFree(void* pv);

// Interlocked access functions
ELONG32 InterlockedIncrement(ELONG32 *lpAddend);
ELONG32 InterlockedDecrement(ELONG32 *lpAddend);
ELONG32 InterlockedExchange(ELONG32 *lpTarget, ELONG32 Value);

#define EK_THREAD_PRIORITY_HIGHEST          5                 
#define EK_THREAD_PRIORITY_ABOVE_NORMAL     4
#define EK_THREAD_PRIORITY_NORMAL           3      
#define EK_THREAD_PRIORITY_BELOW_NORMAL     2
#define EK_THREAD_PRIORITY_LOWEST           1      

#if defined(DEBUG_AGENT_TOKEN_UUID)

// Log register values corresponding to CPUID information level.
void debugCPUID(DWORD (&cpuInfo)[4], DWORD infoType);

// Print debug information to a file.
void logToFile(unsigned int size, BYTE *data, char *description);

#endif // #if defined(DEBUG_AGENT_TOKEN_UUID)

#endif  // _EOS_LINUX_H



