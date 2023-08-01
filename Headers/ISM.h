//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  ISM.h 
//  
//  Subsystem  :  Kernel
//
//  Description:  SM interface definitions.
//                SM =  Software Management
//
//  Classes:       
//              EUploadFile
//              EUploadFiles
//              EUploadFilesList
//              EPackageStatusData
//              EPackageStatusDataList                                  
//              ISM
//
//*****************************************************************************

#ifndef __SM_INTERFACE_H__
#define __SM_INTERFACE_H__

// define the IFileCompression interface
#include "TarGzip.h"

#include "ICustomHandler.h"

/* Group=SM */
#ifdef WIN32
    #ifdef _DV_EXPORT
// {secret}
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

// List of Unique clientIds used with ISM->Upload
// Make sure (relatively) that the ISMListener is unique across all listeners.
#define REG_CLIENT_ID            0x56700000

// pragma substitutions.
static TCHAR* MODEL_PRAGMA = _T("$Model$");
static TCHAR* SN_PRAGMA = _T("$SerialNumber$");

// Used both by SM and by Web Services
// FileTransfer.Upload RPC and elements / attributes
static TCHAR* DO_UPLOAD_RPC = _T("Upload");
// FileTransfer.Download RPC and elements / attributes
static TCHAR* DO_DOWNLOAD_RPC = _T("Download");


//**************************************************************************
// esName is the full path file name to be uploaded.  It may contain
// wildcards * and ?.
// Implementation in base\EUploadFile.cpp
class _DV_DECLSPEC EUploadFile
{
public:
    EUploadFile(EString& esName, bool bDelete, bool bPathDeviceRelative = false);
    EUploadFile(EUploadFile& rUploadFile);
    EUploadFile& operator=(EUploadFile& uploadFile);
    ~EUploadFile();

    // Expand "device relative" path name of upload files.
    void ExpandFilePath(IGateway& rIGateway, int gatewayId);

    // Substitute any model and serial number pragrmas in the filename.
    // For Gateway, the model and serial number are substituted.
    // The filename is expected to already be OS specific w/ path delimeters
    //   (i.e., called EFileIO::ConvertPathString()).
    // Connector:  not used.
    void PragmaSubstitution(IGateway& rIGateway, int gatewayId);

    // search for * and ? as wild cards.
    bool HasWild();

public:
    // Properties
    EString     m_esName;
    bool        m_bDelete;

    // If true, the mn/sn (device) path is pre-pended to the m_esName automatically.
    // name would become:  (for example) defaultProject\mn\sn\"m_esName"
    // This applies to Gateway only.
    bool        m_bPathDeviceRelative;  
};

#ifdef WIN32
_DV_EXTERN template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EUploadFile*>;
_DV_EXTERN template class _DV_DECLSPEC ETypedPtrList<EPtrList, EUploadFile*>;
#endif // WIN32

//**************************************************************************
// List of EUploadFile pointers
// Implementation in base\EUploadFile.cpp
class _DV_DECLSPEC EUploadFiles : public ETypedPtrList<EPtrList, EUploadFile*>
{
public:
    EUploadFiles();     // hint is optional
    EUploadFiles(EUploadFiles& rUploadFiles);
    EUploadFiles(EString& resHint);
    EUploadFiles& operator=(EUploadFiles& uploadFiles);
    ~EUploadFiles();
    void SetManageMemory(bool bState = true);       // auto-delete on desctructor.
    void DeleteAll();

    POSITION AddHead(EUploadFile* newElement)
        { return EPtrList::AddHead(newElement); }

    void ExpandFilePath(IGateway& rIGateway, int gatewayId);
    void PragmaSubstitution(IGateway& rIGateway, int gatewayId);
    void ExpandWildCardFiles();
    // Set pInhibitedPositions to get inhibited list positions.
    // If pInhibitedPositions is NULL, inhibited files will be automatically
    // removed from the list of files.
    KERESULT ProcessCustomHook(ICustomHandlerClient* pICustomHandlerClient,
        int gatewayId, HookedFileXfer::When when, EPtrArray* pInhibitedPositions = NULL);

    // properties
    EString     m_esHint;    
    bool        m_bManageMemory;
};

//**************************************************************************
// Same as EUploadFiles plus version information and extra fields:
// version 1: m_bCompress
class _DV_DECLSPEC EUploadFilesEx : public EUploadFiles
{
public:
    EUploadFilesEx(int iVersion);
    EUploadFilesEx(EUploadFiles& rUploadFiles);
    EUploadFilesEx(EUploadFilesEx& rUploadFiles);
    EUploadFilesEx(EString& resHint);
    ~EUploadFilesEx();

    void Init(void);

    int     m_iVersion;
    bool    m_bSkipCompression;
    bool    m_bNoServerRetry;
};

//**************************************************************************
// Allow collections of list of EUploadFiles.  
// Implementation in base\EUploadFile.cpp
class _DV_DECLSPEC EUploadFilesList : public EPtrList  // can't seem to derive a list from a list using typedef
{
public:
    EUploadFilesList();
    EUploadFilesList(EUploadFilesList& rUploadFilesList);
    EUploadFilesList& operator=(EUploadFilesList& uploadFilesList);
    ~EUploadFilesList();

    POSITION AddHead(EUploadFiles* pUploadFiles);
    POSITION AddTail(EUploadFiles* pUploadFiles);

    void ExpandFilePath(IGateway& rIGateway, int gatewayId);
    void PragmaSubstitution(IGateway& rIGateway, int gatewayId);
    void ExpandWildCardFiles();
    // Set pInhibitedPositions to get inhibited list positions.
    // If pInhibitedPositions is NULL, inhibited files will be automatically
    // removed from the list of files.
    void ProcessCustomHook(ICustomHandlerClient* pICustomHandlerClient,
        int gatewayId, HookedFileXfer::When when, EPtrArray* pInhibitedPositions = NULL);

};

// Enterprise Proxy Package Status defines used by APM Proxy component.
#define PS_PENDING_ASK          13
#define PS_PERMISSION_DENIED    14

//**************************************************************************
// Package Status codes.  Sent as PackageStatus POST to Enterprise Server
// These numbers MUST correspond to values on the Enterprise Server
enum PackageStatus 
{
    psQUEUED,
    psSTARTED,
    psSUCCESS,
    psERROR,            // includes extended error information
    psCANCELED,         // Received a Cancel SOAP command
    psTIMEOUT,          // A dependency timed out (returned at dependecy failure when timeout != 0).
    psROLLING_BACK = 8,     // On error ... doing rollback.
    psROLLBACK_ERROR = 9,   // includes extended error information
    psROLLBACK_SUCCESS =10,
    psDEPENDENCY_FAILED = 11,   // pre-process dependency failure.
    // status 12 is used internally by SM on the Enterprise Server.

    // The following two statuses are APM specific.
    // Both statuses are generated in the APM Proxy component and are documented here
    // as place holders.
    psPendingAsk = PS_PENDING_ASK,               // APM specific status - asking APM for permission
    psPermissionDenied = PS_PERMISSION_DENIED,   // APM specific - permission request was denied.

    psFileInhibited = 15,       // The SDK Custom Handler inhibited one or more files.
    psPAUSED = 17,              // Received a Pause SOAP command

    psNEW = 100,
    psDEPLOYMENT_PENDING = 101,
    psDOWNLOADED = 102
};

//**************************************************************************
// Package ERROR codes.  Sent as PackageStatus POST to Enterprise Server.
// This sets additional attributes when the PackageStatus is psERROR.
// These numbers MUST correspond to values on the Enterprise Server
enum PackageError
{
    peFAILED,                       // 0
    peVERSION,                      // 1
    peBAD_FORMAT,                   // 2
    peUNKNOWN_SOAP_METHOD,          // 3
    peUNSUPPORTED_FUNCTION,         // 4:  for example, data item dependency where 
                                    //     component not found.
    peDI_NAME_NOT_FOUND,            // 5:  unknown data item name in dependency.
    peREGISTRY_NAME_NOT_FOUND,      // 6:  unknown registry entry name
    peREGISTRY_FILE_READ_ERROR,     // 7:  Failed in reading or parsing registry file; dependency can not be executed.
    peINVALID_DEPENDENCY_EXPRESSION,// 8:  The operator or expected value in the 
                                    //     dependency is invalid.
    peNO_FILES_FOUND,               // 9:  upload: no explicitly defined files found,
    peSOME_FILES_NOT_FOUND,         // 10: upload: only some of the explicitly defined files were found 
                                    //     and sent to the Server and some NOT found.
    peDOWNLOAD_EXECUTION_FAILURE,   // 11: Failed to execute 1st file in download.
    peARCHIVE_ERROR,                // 12: Archive failed:  creation, extraction, list, file name etc.
    peREAD_ERROR,                   // 13: failure during file read of archive being uploaded.
    peHTTP_STATUS_ERROR,            // 14: HTTP status error (see EPackageMgr::CheckHTTPStatus())
    peCHUNK_CHECKSUM_ERROR,         // 15: Chunk Checksum (MD5) error during file download. (even after n retries).
    
    // Web Client failures - see WebResultToPackageError()
    peCONNECTION_FAILURE,           // 16: Web Client connection error
    peSOCKS_FAILURE,                // 17: Web Client SOCKS failure
    peHTTP_FAILURE,                 // 18: Web Client HTTP failure
    peSSL_FAILURE,                  // 19: Web Client SSL failure

    peABORT,                        // 20: The Agent has been stopped or the package aborted.
    peFILE_CHECKSUM_ERROR,          // 21: FILE Checksum (MD5) error during file download. (even after n retries).
    peUPLOAD_FILE_MISSING_ERROR,    // 22: A "partial" uploaded file is missing at the upload request.
    peINVALID_DIRECTORY,            // 23: The requested directory can not be created.
    peRESTART_OF_GATEWAY_DEVICE,    // 24: Can not restart a Gateway device (only the Gateway itself).
    peCUSTOM_SOAP_FUNCTION_ERROR,   // 25: General Purpose use by custom SOAP functions that have an error.
    peNO_ERROR,                     // 26:  no error, used as package stuatus default
    peMULTIFILE_UNCOMPRESSED,       // 27: Multi-file transfer without compression requested.
};

//*****************************************************************************
// 3.9 extensions for Web Services
//
// Define PackageStatusData object
//*****************************************************************************
class _DV_DECLSPEC EPackageStatusData
{
public:
    EPackageStatusData()
        : m_gatewayId(CONNECTOR_GATEWAY), m_currentInstructionIx(0), m_instructionsCnt(0),
          m_transferPercent(0), m_transferSize(0), m_status(psSTARTED), m_error(peNO_ERROR)
    {
    }
    EPackageStatusData(const EString& esPackageId, int gatewayId)
        : m_gatewayId(gatewayId), m_currentInstructionIx(0), m_instructionsCnt(0),
          m_transferPercent(0), m_transferSize(0), m_status(psNEW), m_error(peNO_ERROR),
          m_esPackageId(esPackageId)
    {
    }
    EPackageStatusData(EPackageStatusData& rPackageStatus)
    {
        *this = rPackageStatus;
    }

    EPackageStatusData& operator=(EPackageStatusData& rPackageStatus)
    {
        if (this == &rPackageStatus)
            return *this;

        m_gatewayId = rPackageStatus.m_gatewayId;
        m_esPackageName = rPackageStatus.m_esPackageName;
        m_currentInstructionIx = rPackageStatus.m_currentInstructionIx;
        m_instructionsCnt = rPackageStatus.m_currentInstructionIx;
        m_esInstruction = rPackageStatus.m_esInstruction;
        m_transferPercent = rPackageStatus.m_transferPercent;
        m_transferSize = rPackageStatus.m_transferSize;
        m_lastUpdated = rPackageStatus.m_lastUpdated;
        m_status = rPackageStatus.m_status;
        m_error = rPackageStatus.m_error;
        m_esPackageId = rPackageStatus.m_esPackageId;

        return *this;
    }

    // members
    int             m_gatewayId;
    EString         m_esPackageName;
    int             m_currentInstructionIx;     // 0 means no index
    int             m_instructionsCnt;
    EString         m_esInstruction;    // may be empty
    int             m_transferPercent;
    LONGLONG        m_transferSize;     // in bytes
    ETimeStamp      m_lastUpdated;
    PackageStatus   m_status;
    PackageError    m_error;    // initialized to fail.  Dependent on m_status == psERROR
    EString         m_esPackageId;
};


//**************************************************************************
typedef ETypedPtrList<EPtrList, EPackageStatusData*> EPackageStatusDataList;


//*****************************************************************************
// Optional listener to be informed when upload is completed.
// OnUploadComplete is passed the clientId that was input with the Upload command.
// NOTE:  The assumption is the life-time of the listener is valid until the
// CKernelService::Stop() function.
// 
interface ISMListener
{
    virtual void OnUploadComplete(EHANDLE clientId, KERESULT successCode) = 0;
};


//*****************************************************************************
// Enumeration for FileTransferControl method of ISM
enum FileTransferCtrl
{
    ftcCANCEL,
    ftcPAUSE,
    ftcRESUME
};

//*****************************************************************************
enum CompressionType
{
    ctNONE,
    ctTARGZIP
};

//*****************************************************************************
// Package status listener.
interface IPackageListener
{
    virtual void OnPackageStatusUpdate(EPackageStatusData& packageStatus) = 0;
};

//*****************************************************************************
interface ISM : public IKernelBase
{
    // Allow Custom Component or EDD to upload files directly.
    // Files must be specified in the EUploadFiles collection.
    // 
    // Model, Serial number and owner are optional for Connector, but required
    // for a Gateway upload. If the owner is a NULL pointer or empty string, 
    // the default is used.

    // 
    // The clientId and pIListener are optional.  If used, they indicate the
    // need to be notified when the upload is complete.  The clientId is passed
    // back as part of the callback notification.  
    // The clientId must be unique.  See REG_CLIENT_ID as an example above.
    // If the clientId is already in use, a  KEC_SM | KE_ALREADY_EXIST error will 
    // be returned. Use Unsubscribe to stop listening for a particular clientID or
    // ISMListener.
    //
    // An optional "hint" can be set in the EUploadFiles collection (property:  m_esHint)  
    // The hint is sent to the Enterprise Server regarding the set of files uploaded.
    // This hint indicates how the Enterprise Server should view or apply the files.

    virtual KERESULT Upload(EUploadFiles& rUploadFiles,
        /* optional parameters*/ 
            EString* pesMn = NULL, EString* pesSn = NULL, EString* pesOwner = NULL, 
            EHANDLE hClientId = 0, 
            ISMListener* pIListener = NULL ) = 0;

    // Unsubscribe - do not notify regarding the particular hClientId or for any clients
    // on the ISMListener.
    virtual KERESULT Unsubscribe(EHANDLE hClientId) = 0;
    virtual KERESULT Unsubscribe(ISMListener* pIListener) = 0;

    // Allow custom File compression.  The default implementation via ETarGzip is used
    // by the SM component if another file compression implementation is not registered.
    // 
    // If used, this is assumed to be the only file compression used for SM.  
    // The life of the IFileCompression implementation must be maintatained until the
    // Agent is stopped.
    // 
    // IFileCompression is defined in TarGzip.h.  
    // The pszExtension is ".tar.gz" for Tar gzip files.  Use ".zip" for WinZip.
    virtual KERESULT RegisterFileCompression(IFileCompression* pIFileCompression, 
        TCHAR* pszExtension) = 0;

    // Notify SM that the Registry file (Fixed name:  Dependencies.xml) has been updated.  
    // This causes SM to upload the file to the Enterprise Server.
    //
    // Model, Serial number and owner are optional for Connector, but required
    // for a Gateway upload.  
    // If the owner is a NULL pointer or empty string, the default is used.
    virtual KERESULT OnRegistryUpdated(/* optional parameters*/
        EString* pesMn = NULL, EString* pesSn = NULL, EString* pesOwner = NULL) = 0;

    // Used to indicate that the upload command was received from the DRM server but
    // is not allowed.  Used both to notify initiated of upload about the deny and
    // to update the package status information.
    virtual KERESULT OnUploadDenied(EHANDLE hClientId, int gatewayId, EString& esPackageName) = 0;

    // Simple mechanism to check if SM has processed all of its packages.
    virtual KERESULT IsPackageQueueEmpty(bool* pIsEmpty) = 0;

    // Support for Web Services.  Get collection of the status for each running package.
    virtual KERESULT GetPackageStatuses(/* in/out */ EPackageStatusDataList& packageStatuses) = 0;

    // File Transfer Control.  Controls ALL packages - both those running and those in the queue.
    // Allow user to 
    //   - Cancel all running and queued packages, 
    //   - Pause:  pauses all file transfers, but all other package commands are executed.
    //   - Resume: resume all file transfers that may have been paused.
    // Timeout.  Used only with Pause.  If a paused transfer is not resumed in X seconds,
    // the package is canceled.  New packages with file transfers are executed as normal.
    virtual KERESULT FileTransferControl(FileTransferCtrl ftc, DWORD timeout) = 0;

    // same as ISM::Upload(), but accepts EUploadFilesEx
    virtual KERESULT UploadEx(EUploadFilesEx& rUploadFiles,
        /* optional parameters*/ 
            EString* pesMn = NULL, EString* pesSn = NULL, EString* pesOwner = NULL, 
            EHANDLE hClientId = 0, 
            ISMListener* pIListener = NULL) = 0;

    // Enables the Agent's Policy Server component to indicate that the upload status
    // is pending the result of an ask for permission.
    // Used to update the package status information.  This may set the wrong status 
    // for multiple unNamed packages.
    virtual KERESULT OnUploadAskPending(int gatewayId, 
        EString& esPackageName) = 0;

    // Manage subscriptions to package status updates.
    virtual KERESULT AddPackageListener(IPackageListener *pListener) = 0;
    virtual KERESULT RemovePackageListener(IPackageListener *pListener) = 0;
};


#endif

