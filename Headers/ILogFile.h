//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ILogFile.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for Communication with Logger
//
//  Interface  :  ILogFile
//              
//**************************************************************************
#ifndef __ILOGFILE__H__
#define __ILOGFILE__H__

//// Don't change the values defined below. ELogFile depends on these ///////
#define LOG_FORMAT_PLAIN        0
#define LOG_FORMAT_CSV          1
#define LOG_FORMAT_HTML         2
#define LOG_DEST_LOCAL          1
#define LOG_DEST_ESFILE         2
#define LOG_DEST_EMAIL          3

/* Group=LogFile */

// This allows the Kernel Log direct access to the LogFile component to log
// event messages to file.
interface ILogFile
{
    virtual KERESULT ReportEvent(EONEVENTSTRUCT* pEvent) = 0;
    virtual KERESULT LogSnapShot(EString& rSchemaName, EONEVENTSTRUCT *pEvents, DWORD dwCount) = 0;
    virtual KERESULT LogAlarm(EString& rSchemaName, EONEVENTSTRUCT *pEvent) = 0;
    virtual KERESULT GetAgentLogFile(EString& fileName, int& format) = 0;
};


#endif //__ILOGFILE__H__
