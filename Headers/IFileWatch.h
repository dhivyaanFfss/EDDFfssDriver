//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IFileWatch.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for Communication with FileWatch component
//
//  Interface  :  IFileWatchMgr
//              
//**************************************************************************
#ifndef __I_FILE_WATCH_H__
#define __I_FILE_WATCH_H__

// File Watch supports the following action
#define ACTION_FILE_WATCH   _T("DoFileWatch")

// File Watch supports the following triggers
#define WATCHED_FILE_CHANGED	_T("OnFileChanged")
#define WATCHED_FILE_MISSING	_T("OnFileMissing")
#define WATCHED_FILE_UPLOADED	_T("OnFileUploaded")

#define EFW_EVENT_SEVERITY  10
#define EFW_ALARM_SEVERITY  10

//**************************************************************************
typedef enum
{
    EFileWatchActionNone,
    EFileWatchActionUpload
} EFileWatchAction;

//**************************************************************************
typedef enum
{
    EFileWatchNotifyNone,
    EFileWatchNotifyAlarm,
    EFileWatchNotifyEvent
} EFileWatchNotifyType;

//**************************************************************************
typedef enum
{
    EFileStatusTypeBasic,
    EFileStatusTypeTimeSize,
    EFileStatusTypeMD5
} EFileStatusType;


//**************************************************************************
// FileWatch Definition. struct used in AddFileWatch() call
//**************************************************************************
struct FileWatchDef
{
    FileWatchDef() : 
		m_iAction(EFileWatchActionNone),
		m_bInitialAction(false),
		m_iStatusType(EFileStatusTypeTimeSize),
		m_iChangeNotify(EFileWatchNotifyNone),
		m_iMissingNotify(EFileWatchNotifyNone),
		m_bTransient(false),
		m_iMissingSeverity(EFW_ALARM_SEVERITY),
		m_iChangeSeverity(EFW_ALARM_SEVERITY),
		m_iActionDelay(0),
		m_bSkipUploadCompression(false)
    {
    }

    EString                 m_esName;					// watched file/directory name. Must be set by caller
    EFileWatchAction        m_iAction;					// upload watched file or not
    bool                    m_bInitialAction;			// if true, take initial action
    EFileStatusType         m_iStatusType;				// how to check file change status
    EFileWatchNotifyType    m_iChangeNotify;			// event/alarm on file change
    EFileWatchNotifyType    m_iMissingNotify;			// event/alarm on file missing
    EString                 m_esHint;					// upload hint, optional
    bool                    m_bTransient;				// if true, file is transient
	int                     m_iMissingSeverity;			// event/alarm severity for missing file 
	int                     m_iChangeSeverity;			// event/alarm severity for changed file
    int                     m_iActionDelay;				// upload delay in seconds
    bool                    m_bSkipUploadCompression;	// if true, do not compress file for uploading
};

//**************************************************************************
// IFileWatchMgr interface definition, answers the Kernel Id: IID_FILE_WATCH_MGR
//**************************************************************************
interface IFileWatchMgr : public IKernelBase
{
    // Add a File Watch using the FileWatchDef
    virtual KERESULT AddFileWatch(TCHAR* pszFileWatchName, FileWatchDef* pWatchDef) = 0;

    // Add a Directory Watch using the FileWatchDef. If pszPattern is not set, will use *
    virtual KERESULT AddDirectoryWatch(TCHAR* pszFileWatchName, FileWatchDef* pWatchDef, bool bRecursive = false, TCHAR* pszPattern = NULL) = 0;

	// Remove a File or Directory Watch
    virtual KERESULT RemoveWatch(TCHAR* pszFileWatchName) = 0;
};
#endif
