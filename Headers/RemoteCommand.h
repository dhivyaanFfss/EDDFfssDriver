/////////////////////////////////////////////////////////////////////////////
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//  Command.h
//
//  This file defines the remote commands among Unity Family Applications.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __REMOTECOMMAND_H__
#define __REMOTECOMMAND_H__
#ifdef UNDER_VXWORKS
#include "os.h"
#endif
// UWatch Kernel used port 3075. The Remote Server uses port 3076
#define UC_EXIT_PORT			3011        // Axeda Connector listening port
#define UC_DEFAULT_PORT			3076        // Remote Server listening port
#define UC_IO_PORT				3077        // Remote Server StdOut and StdErr redirection port
#define UC_NAME_SIZE			64			// OEM and Device type names
#define UC_BUFFSIZE				4096		// Buffer size
#define UC_TIME_OUT				3000		// Three seconds

// Shared data in number of bytes
#define UC_SHARED_BYTES			(sizeof(TCHAR) * MAX_PATH + sizeof(DWORD) * 2)
// Shared data in number of TCHARS
#define UC_SHARED_TCHARS		(MAX_PATH + sizeof(DWORD) * 2 / sizeof(TCHAR))

// Supported remote commands
#define UC_INVALID_COMMAND		0X0000		// Invalid command
#define UC_BUILDER_CHECK		0X0001		// Builder "isAlive" check
#define UC_DOWNLOAD_FILE		0X0101		// Version 1, Download file (PC to WCE).
#define UC_UPLOAD_FILE			0X0102		// Version 1, Upload file (WCE to PC).
#define UC_RUN_WAIT				0X0103		// Version 1, Run program wait.
#define UC_RUN_RETURN			0X0104		// Version 1, Run program no wait.
#define UC_CLOSE_PROJ			0X0105		// Version 1, Close Project.
#define UC_OPEN_PROJ			0X0106		// Version 1, Open Project.
#define UC_RELOAD_PROJ			0X0107		// Version 1, Reload Project.
#define UC_EXIT_APP				0X0108		// Version 1, Exit UWatch.
#define UC_OPEN_DISPLAY			0X0109		// Version 1, Open a display
#define UC_EXIT_SERVER			0X010A		// Version 1, Exit RemoteServer
#define UC_DEVICE_INFO			0X010B		// Version 1, Return device info
#define UC_PROGRAM_COMMAND		0X010C		// Version 1, Send a command to a program
#define UC_CREATE_DIRECTORY		0X010D		// Version 1, Create directory
#define UC_DELETE_DIRECTORY		0X010E		// Version 1, Delete directory
#define UC_DELETE_DIRECTORY_REC	0X010F		// Version 1, Delete directory recursively
#define UC_LOGON_USER			0X0110		// Version 1, Log on User
#define UC_LOGOFF_USER			0X0111		// Version 1, Log off User
#define UC_QUERY_APPLICATION	0X0112		// Version 1, Query Application
#define UC_QUERY_PROJECT		0X0113		// Version 1, Query UWatch project
#define UC_QUERY_DISPLAYS		0X0114		// Version 1, Query UWatch displays
#define UC_QUERY_USERS			0X0115		// Version 1, Query UWatch users
#define UC_QUERY_DIRECTORY		0X0116		// Version 1, Query Directories
#define UC_QUERY_APP_ARGS		0X0117		// Version 1, Query App Arguments
#define UC_QUERY_FILES 			0X0118		// Version 1, Query All files
#define UC_WINDOWS_FILE 		0X0119		// Version 1, Query App Arguments
#define UC_DELETE_FILES			0X011A		// Version 1, Delete files
#define UC_COPY_FILES			0X011B		// Version 1, Copy files
#define UC_DIRECTORY_EXIST		0X011C		// Version 1, Ask if directory exists
#define UC_DEVICE_ID			0X011D		// Version 1, Get the CE device ID
#define UC_GET_VERSION			0X011E		// Version 1, Get Remoteserver version
#define UC_EXIT_CONNECTOR		0X011F		// Version 1, Shutdown Axeda Connector

#define UC_SERVER_STATUS        0X0120      // Return Enterprise Server status

#define ALIVE_MAGIC_NUMER		0X1503		// Number to XOR with UC_ALIVE_PORT

// Commands added since Agent 6.8.1
#define UC_BROWSE_EDD_ITEMS		0X10000		// Browse EDD data items
#define UC_DIRECTORIES_LISTING	0X10001		// Browse Directories with Disk Drive or root
#define UC_FILES_LISTING		0X10002		// Browse files with ASCII names. UC_QUERY_FILES returns USHORT for characters
											// UC_QUERY_FILES is wrong to assume that TCHAR is USHORT
#define UC_LIBRARIES_LISTING	0X10003		// Browse library files

// The "UPipeName" Name for Shared Memory memory area
#define UC_PIPE_NAME			_T("UPipeName")

#define UC_SHARED_SIZE			10000

// The "UWatchExistEvent" object signals UWatch is running
#define UWATCH_EXIST_OBJECT		_T("UWatchExistEvent")

// The "RemoteSrvExistEvent" object signals ERemoteSrv is running
#define REMOTESVR_EXIST_OBJECT	_T("ERemoteSrvExistEvent")

// The "UCEvent" object shared among Unity Family Applications.
#define UC_EVENT_OBJECT			_T("UCEvent")

// The "UWatchReply" event object shared among Unity Family Applications.
#define UC_UWATCH_REPLY_EVENT	_T("UWatchReply")

// The UWatchName object shared among Unity Family Applications.
#define UC_GET_UWATCH_NAME		_T("UWatchNameObject")

// The UCFileMapping object shared among Unity Family Applications.
#define UC_FILE_OBJECT			_T("UCFileMapping")

// The UWatchName FileMapping object shared among Unity Family Applications.
#define UC_UWATCHNAME_FILE_OBJECT	_T("UWatchNameFileMap")

// The Reply event from a program
#define UC_REPLY_OBJECT			_T("UReply")

// The UWatch.exe name
#define UC_UWATCH_NAME			_T("\\UWatch.exe")

// The EBatch.exe name
#define UC_EBATCH_NAME			_T("\\EBatch.exe")

// The EReset.exe name
#define UC_ERESET_NAME			_T("\\EReset.exe")

// The start.bat name
#define UC_STARTBAT_NAME		_T("\\Start.bat")

// The reset.bat name
#define UC_RESETBAT_NAME		_T("\\Reset.bat")

// STRUCT TO HOLD DEVICE INFO
typedef struct tagCEDeviceInfo
{
	WORD	wProcessorArchitecture;
	WORD	wProcessorLevel;
	WORD	wProcessorRevision;
	DWORD	dwProcessorType;
	DWORD	dwTotalMemory;
	DWORD	dwFreeMemory;
	DWORD	dwScreenX;
	DWORD	dwScreenY;
	WCHAR	OEMNames[UC_NAME_SIZE];
	WCHAR	Platform[UC_NAME_SIZE];
}CEDeviceInfo;

// ERROR CODES USED AMONG UNITY FAMILY APPLICATIONS
#define UC_ERROR_SUCCEED				0X00000000
#define UC_ERROR_UNKNOWN				0XFFFFFFFF

// Socket errors
#define UC_ERROR_SOCKET_TYPE			0X80010000
#define UC_ERROR_SOCKET_START			0X80010001
#define UC_ERROR_SOCKET_CREATE			0X80010002
#define UC_ERROR_SOCKET_CONNECT			0X80010003
#define UC_ERROR_SOCKET_SEND			0X80010004
#define UC_ERROR_SOCKET_RECEIVE			0X80010005
#define UC_ERROR_SOCKET_TIMEOUT			0X80010006

// Download file errors
#define UC_ERROR_DOWNLOAD_TYPE			0X80020000
#define UC_ERROR_DOWNLOAD_OPEN			0X80020001
#define UC_ERROR_DOWNLOAD_READ			0X80020002
#define UC_ERROR_DOWNLOAD_WRITE			0X80020003
#define UC_ERROR_DOWNLOAD_CREATE		0X80020004


// Upload file errors
#define UC_ERROR_UPLOAD_TYPE			0X80030000
#define UC_ERROR_UPLOAD_CREATE			0X80030001
#define UC_ERROR_UPLOAD_OPEN			0X80030002
#define UC_ERROR_UPLOAD_READ			0X80030003

// Run program errors
#define UC_ERROR_RUNPROGRAM_TYPE		0X80040000
#define UC_ERROR_RUNPROGRAM_CREATE		0X80040001
#define UC_ERROR_RUNPROGRAM_EXISTS		0X80040002

// Close project errors
#define UC_ERROR_CLOSEPROJECT_TYPE		0X80050000

// Open project errors
#define UC_ERROR_OPENPROJECT_TYPE		0X80060000
#define UC_ERROR_OPENPROJECT_NONEXIST	0X80060001

// Reload project errors
#define UC_ERROR_RELOADPROJECT_TYPE		0X80070000

// Exit App errors
#define UC_ERROR_EXITAPP_TYPE			0X80080000

// Open display errors
#define UC_ERROR_OPENDISPLAY_TYPE		0X80090000
#define UC_ERROR_OPENDISPLAY_NONEXIST	0X80090001

// Exit RemoteServer errors
#define UC_ERROR_EXITSERVER_TYPE		0X800A0000

// Logon user errors
#define UC_ERROR_LOGONUSER_TYPE			0X800B0000
#define UC_ERROR_LOGONUSER_NONEXIST		0X800B0001
#define UC_ERROR_LOGONUSER_WRONGPASS	0X800B0002

// Logoff user errors
#define UC_ERROR_LOGOFFUSER_TYPE		0X800C0000

// Query applicaion errors
#define UC_ERROR_QUERYAPPS_TYPE			0X800D0000

// Query project errors
#define UC_ERROR_QUERYPROJECT_TYPE		0X800E0000

// Query display errors
#define UC_ERROR_QUERYDISPLAY_TYPE		0X800F0000

// Query user errors
#define UC_ERROR_QUERYUSER_TYPE			0X80100000

// Query directory errors
#define UC_ERROR_QUERYDIRECTORY_TYPE	0X80110000

// Query application arguments errors
#define UC_ERROR_QUERYAPPARGS_TYPE		0X80120000

// Delete directory
#define UC_ERROR_DELETE_DIR_TYPE		0X80130000
#define UC_ERROR_DIR_NON_EMPTY			0X80130001

// not specific command
#define UC_ERROR_UNKNOWN_COMMAND		0X80200000
#define UC_ERROR_NO_MEMORY				0X80200001
#define UC_ERROR_DEVICE_NO_MEMORY		0X80200002
#define UC_ERROR_UWATCH_TIMEOUT			0X80200003
#define UC_ERROR_UWATCH_NONAMEMAP		0X80200004
#define UC_ERROR_UWATCH_BADNAME			0X80200005
#define	UC_ERROR_OPERATION_CANCELLED	0X80200006
#define UC_ERROR_CLOSEPROJECT_NONEXIST	0X80200007
#define	UC_ERROR_LOGOFFUSER_NOLOGON		0X80200008
#define	UC_ERROR_RELOADPROJECT_NONEXIST 0X80200009
#define	UC_ERROR_NOTSUPPORTED           0X8020000A

extern DWORD CESender(LPCTSTR, LPCTSTR, LPCTSTR, DWORD cmd, LPTSTR pResult = NULL);


#endif



