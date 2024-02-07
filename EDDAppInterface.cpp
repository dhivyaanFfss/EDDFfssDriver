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
    m_bRunning(true),
    m_bLogPipeError(true)
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
    CloseHandle(m_pipe);
    return true;
}

bool SysAppIntf::KillProcess()
{
    m_bIsPipeConnected = false;
    return true;
}

void SysAppIntf::Initialize()
{
    connect();   
}

bool SysAppIntf::connect()
{
    // Blocking call until connection
    while (!m_bIsPipeConnected && m_bRunning)
    {
        STARTUPINFO info;
        ZeroMemory(&info, sizeof(STARTUPINFO));
        info.cb = sizeof info;

        m_pipe = CreateFile(
            SYSAPP_PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (m_pipe == INVALID_HANDLE_VALUE)
        {
            if (m_bLogPipeError)
            {
				DWORD error = GetLastError();
				std::string errorID = std::to_string(error);
				EString errorIdEStr = EString(errorID.c_str());
				EString errorMsg(_T("Cannot connect to pipe: ") + errorIdEStr);
                g_pEDDMgr->CustomReport(ET_INFO, 4, errorMsg);
                m_bLogPipeError = false;
            }
            Sleep(3000);
        }
        else
        {
            DWORD dwMode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;

            if (!SetNamedPipeHandleState(m_pipe, &dwMode, NULL, NULL))
            {
                m_bIsPipeConnected = true;
                // Reset loggin, so we log next error
                m_bLogPipeError = true;
            }
            else
            {
                DWORD error = GetLastError();
                std::string errorID = std::to_string(error);
                EString errorIdEStr = EString(errorID.c_str());
                EString errorMsg(_T("Cannot set named pipe state, error: ") + errorIdEStr);
                g_pEDDMgr->CustomReport(ET_INFO, 4, errorMsg);
                m_bIsPipeConnected = false;
            }
        }
    }

    return m_bIsPipeConnected;
}

bool SysAppIntf::WaitForSysAppCmd(std::string&  reply)
{
    m_pipeMtx.Lock();
    if (!connect())
    {
        m_pipeMtx.Unlock();
        return false;
    }

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
        DWORD error = GetLastError();
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

bool SysAppIntf::SendToSysApp(const char* message, bool flush)
{
    m_pipeMtx.Lock();
    if (!connect())
    {
        m_pipeMtx.Unlock();
        return false;
    }

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

    EString replyMsg = EString(message);
    EString logMsg(_T("Sent reply : ") + replyMsg);
    g_pEDDMgr->CustomReport(ET_INFO, 4, logMsg);

    m_pipeMtx.Unlock();
    return (bytesToWrite == written);
}
