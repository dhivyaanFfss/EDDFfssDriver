//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  IMobileDRMMgr.h 
//  
//  Subsystem  :  Kernel
//
//  Description:  Mobile DRM Manager interface and support definitions.
//                
//
//  Classes:
//              IMobileDRMListener       
//              IMobileDRMMgr
//
//*****************************************************************************

#ifndef __IMOBILE_DRM_MGR_H__
#define __IMOBILE_DRM_MGR_H__

/* Group=MobileDRMMgr */


#define     IMPORT_EXTENSION            _T(".xdrm")
#define     EXPORT_EXTENSION            _T(".idrm")
#define     IMPORT_TARGZIP_EXTENSION    _T(".ServiceLinkExport.tar.gz")  /* exported from the Server */
#define     EXPORT_TARGZIP_EXTENSION    _T(".ServiceLinkImport.tar.gz")  /* imported to the Server */
#define     EXPORT_ROOT_FILENAME        _T("Import")                     /* file to be imported to the Server */

//*****************************************************************************
interface IMobileDRMListener
{
    // complete only means imported file has been parsed.  Commands executed
    // in the background may not be complete (e.g. Software management packages).
    virtual KERESULT OnImportComplete(int iDeviceId) = 0;

    virtual KERESULT OnExportComplete(int iDeviceId) = 0;
};

//*****************************************************************************
// The Moblie DRM Manager functions available to all components.
interface IMobileDRMMgr : public IKernelBase
{
    // Import the files exported from the DRM Server.
    // Work done asynchronously, if need status, Subcribe().
    virtual KERESULT ProcessAllImports(void) = 0;
    virtual KERESULT ProcessImport(void) = 0;
    virtual KERESULT ProcessImport(int deviceId) = 0;

    // Causes MobileDRM component to initiate the exporting processes.
    // (while this is done via the IEnterpriseProxy, it allows a single
    // component to manage file names and directories).
    virtual KERESULT ProcessAllExports(void) = 0;
    virtual KERESULT ProcessExport(void) = 0;
    virtual KERESULT ProcessExport(int deviceId) = 0;

    // Allow other components to be notified regarding the import status.
    virtual KERESULT Subscribe(IMobileDRMListener* pIListener) = 0;
    virtual KERESULT Unsubscribe(IMobileDRMListener* pIListener) = 0;

    // Initiate archiving (tar.gzip) off all files to be exported.
    virtual KERESULT ArchiveExports(TCHAR *pszFileName = NULL) = 0;

    // Allow SM to set file upload attributes:  hint or session id.
    // esHint is set if Agent initiated upload (else empty). 
    //   note: Hint may be empty even if Agent initiated.
    // esSessionId is set if Server initiated upload (else empty)
    virtual KERESULT SetUploadAttributes(int deviceId, EString& esFilename,
        EString& esHint, EString& esSessionId) = 0;
};

#endif
