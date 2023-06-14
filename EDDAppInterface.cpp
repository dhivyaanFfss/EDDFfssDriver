#include "pch.h"
#include "EDDAppInterface.h"

#define SYSAPP_PIPE_NAME L"\\\\.\\pipe\\remoteservicesagent"
#define SVC_NACK "NACK"
#define MSG_PIPE_BUF_SIZE_SYSAPP_REMOTE_SERVICE_AGENT      18432   // in bytes
#define SEPARATOR_SYSAPP_SYSAPPSVC_SYSAPPADMIN              '@'     // exact same separator must be used in all of the three
constexpr int MSG_BUFFER = MSG_PIPE_BUF_SIZE_SYSAPP_REMOTE_SERVICE_AGENT;

SysAppIntf & SysAppIntf::Instance()
{
    static SysAppIntf instance;

    return instance;
}


/**
*******************************************************************************
* Constructor
*******************************************************************************
*/
SysAppIntf::SysAppIntf() :
    m_pipe(INVALID_HANDLE_VALUE),
    m_bIsPipeConnected(false),
    m_bRunning(true)
{
}

/**
*******************************************************************************
* Destructor
*******************************************************************************
*/
SysAppIntf::~SysAppIntf()
{
    CloseHandle(m_pipe);
}

bool SysAppIntf::StopConnection()
{
    m_bIsPipeConnected = false;
    DisconnectNamedPipe(m_pipe);
    CloseHandle(m_pipe);
    return true;
}

void SysAppIntf::Initialize()
{
   
    m_pipe = CreateNamedPipe(SYSAPP_PIPE_NAME,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
        PIPE_UNLIMITED_INSTANCES,
        MSG_BUFFER,
        MSG_BUFFER,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);
}

bool SysAppIntf::connect()
{
    // Blocking call until connection
    bool success = false;
    while (!success)
    {
        ConnectNamedPipe(m_pipe, NULL);
        DWORD error = GetLastError();
        if (success ||
            (error == ERROR_PIPE_CONNECTED))
        {
            m_bIsPipeConnected = true;
            success = true;
        }
        else if (error != ERROR_IO_PENDING &&
            error != ERROR_PIPE_LISTENING)
        {
            // Some other error occured, restart the pipe and
            // wait for connection
            StopConnection();
            Initialize();
        }
        else
        {
            Sleep(5000); // 5.0 seconds sleep before checking again
        }
    }

    return m_bIsPipeConnected;
}

bool SysAppIntf::WaitForSysAppCmd(std::string&  reply)
{
    m_pipeMtx.Lock();
    connect();

    if (m_pipe == INVALID_HANDLE_VALUE)
    {
        // error message
        StopConnection();
        m_pipeMtx.Unlock();
        return false;
    }

    bool ret = false;

    char  buffer[MSG_BUFFER] = { 0x00 };
    DWORD dwRead;
    bool   bOk = false;
    unsigned long  cbRead;

    if (m_pipe != INVALID_HANDLE_VALUE &&
        m_bRunning)
    {
        DWORD bytesRead = ReadFile(m_pipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
        if (bytesRead != 0)
        {
            /* add terminating zero */
            buffer[dwRead] = '\0';

            /* do something with data in buffer */
            printf("App cmd Rxd: %s", buffer);
            reply = buffer;
            ret = true;
        }
    }

    m_pipeMtx.Unlock();
    return ret;
}

bool SysAppIntf::SendToSysApp(const char * message, bool flush)
{
    m_pipeMtx.Lock();
    connect();

    if (m_pipe == INVALID_HANDLE_VALUE)
    {
        // error message
        StopConnection();
        m_pipeMtx.Unlock();
        return false;
    }

    char buffer[MSG_BUFFER];
    unsigned long bytesToWrite = 0;

    bytesToWrite = (unsigned long)strlen(message) + 1;
    if (bytesToWrite >= MSG_BUFFER)
    {
        // Error message
        StopConnection();
        m_pipeMtx.Unlock();
        return false;
    }

    if (flush)
    {
        FlushFileBuffers(m_pipe);
    }

    strcpy_s(buffer, message);
    unsigned long written = 0;
    if (!WriteFile(m_pipe, buffer, bytesToWrite, &written, nullptr))
    {
        // error message
    }

    m_pipeMtx.Unlock();
    return (bytesToWrite == written);
}
