//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EProgramFilter.h
//  
//  Description:  Program filter abstraction. Allows to run a process with
//                pipes attached to its stdin/stdout. This allows user to
//                read/write from/to the process' stdout/stdin. In the relay
//                mode the filter relays the process' stdin/stdout to user's
//                stdin/stdout.
//
//  Classes:      EIOBuffer, EIOChannel, EProgramFilter
//
//**************************************************************************
#ifndef EPROGRAM_FILTER_H
#define EPROGRAM_FILTER_H

#ifdef WIN32
    #ifdef _DV_EXPORT
       #define _DV_DECLSPEC __declspec(dllexport)
       #define _DV_EXTERN
    #else
       #define _DV_DECLSPEC __declspec(dllimport)
       #define _DV_EXTERN   extern
    #endif
#else
   #define _DV_DECLSPEC
   #define _DV_EXTERN
#endif

#define EIO_BUFFER_SIZE     4096
#define EIO_INVALID_HANDLE  ((HANDLE) -1)
#define EFILTER_MAX_ARG     16

//**************************************************************************
class _DV_DECLSPEC EIOBuffer
{
public:
    EIOBuffer(int iSize);
    ~EIOBuffer();

    int     PeekInbound(char **ppBuffer);
    void    SeekInbound(int iOffset);
    int     PeekOutbound(char **ppBuffer);
    void    SeekOutbound(int iOffset);
    int     Read(char *pBuffer, int iSize);
    int     Write(char *pBuffer, int iSize);

private:
    char    *m_pData;
    int     m_iSize;
    int     m_iReadPos;
    int     m_iWritePos;
    bool    m_bEmpty;
};

//**************************************************************************
class _DV_DECLSPEC EIOChannel
{
public:
    EIOChannel(void);
    ~EIOChannel();

    bool    Read(char *pBuffer, int iSize);
    bool    ReadLine(char *pBuffer, int iSize);
    bool    Write(char *pBuffer, int iSize);
    bool    Flush(void);
    bool    DoRead(void);
    bool    DoWrite(void);
    void    StartRelay(ESingleEvent *pPipeEvent);
    void    StopRelay(void);

    HANDLE  GetSource(void);
    void    SetSource(HANDLE hSource);
    HANDLE  GetSink(void);
    void    SetSink(HANDLE hSink);

private:
    EThread m_relayThread;
    static ThreadReturn __stdcall RelayThreadStub(void *pArg);
    void    RelayThread(void);

    HANDLE          m_hSource;
    HANDLE          m_hSink;
    EIOBuffer       m_buffer;
    ESingleEvent    *m_pPipeEvent;
    bool            m_bRelaying;
};

#ifndef UNDER_CE

//**************************************************************************
class _DV_DECLSPEC EProgramFilter
{
public:
    EProgramFilter(TCHAR *pCommand);
    EProgramFilter(char *pCommand);
    ~EProgramFilter();

    bool    Start(void);
    bool    Stop(void);
    bool    CheckStatus(bool bWait);
    void    StartInputRelay(ESingleEvent *pPipeEvent, HANDLE hSource);
    void    StopInputRelay(void);
    void    StartOutputRelay(ESingleEvent *pPipeEvent, HANDLE hSink);
    void    StopOutputRelay(void);
    bool    Read(char *pBuffer, int iSize);
    bool    ReadLine(char *pBuffer, int iSize);
    bool    Write(char *pBuffer, int iSize);
    bool    Flush(void);
    bool    IsRunning(void);
    int     GetExitCode(void);
    bool    IsUserConnected(void);

private:
    void    Initialize(void);

    EString             m_esCommand;
    EIOChannel          m_input;
    EIOChannel          m_output;

    HANDLE              m_hFilterIn;
    HANDLE              m_hFilterOut;

    bool                m_bRunning;
    int                 m_iResult;
#ifdef WIN32
    PROCESS_INFORMATION m_procInfo;
#else
    pid_t               m_iPid;
#endif
};

#endif // UNDER_CE

//**************************************************************************
inline HANDLE EIOChannel::GetSource(void)
{
    return m_hSource;
}

//**************************************************************************
inline void EIOChannel::SetSource(HANDLE hSource)
{
    m_hSource = hSource;
}

//**************************************************************************
inline HANDLE EIOChannel::GetSink(void)
{
    return m_hSink;
}

//**************************************************************************
inline void EIOChannel::SetSink(HANDLE hSink)
{
    m_hSink = hSink;
}

#ifndef UNDER_CE

//**************************************************************************
inline bool EProgramFilter::IsRunning(void)
{
    return m_bRunning;
}

//**************************************************************************
inline int EProgramFilter::GetExitCode(void)
{
    return m_iResult;
}

//**************************************************************************
inline bool EProgramFilter::IsUserConnected(void)
{
    return (m_input.GetSource() != EIO_INVALID_HANDLE &&
        m_output.GetSink() != EIO_INVALID_HANDLE);
}

#endif // UNDER_CE

#endif
