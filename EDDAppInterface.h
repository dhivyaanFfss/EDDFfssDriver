#pragma once
/***********************************************************************
 *  File: SysAppSvcIntf.h
 *  Copyright (c) 2018 FUJIFILM SonoSite, Inc.
 *  Contains Confidential Information - All Rights Reserved
 *********************************************************************/

#ifndef __EDD_APP_INTERFACE_H__
#define __EDD_APP_INTERFACE_H__

#include <windows.h>
#include <string>

#define SYSAPPADM_REPLY_FAILED  "FAILED"
#define SYSAPPADM_REPLY_OK      "OK"


class SysAppIntf
{
public:
    static SysAppIntf& Instance();
    void Initialize();
    bool WaitForSysAppCmd(std::string&);
    bool SendToSysApp(const char* message, bool flush = false);
    bool StopConnection();
    bool KillProcess();

    virtual ~SysAppIntf();

private:
    SysAppIntf();

    HANDLE m_pipe;
    bool m_bIsPipeConnected;
    bool connect();
    bool disconnect();
    bool m_bRunning;
    EMutex m_pipeMtx;

public:

};

#endif