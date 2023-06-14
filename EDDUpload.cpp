//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDDUpload.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//                Demonstrates how to get and use the Agent's software management
//                interface to upload a file to the DRM Enterprise Server.
//
//  Classes    :  
//                  EDDDriver
//                  
//

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"


//*****************************************************************************
KERESULT EDDDriver::UploadFile(TCHAR* pszFileName, bool bDelete, TCHAR* pszHint)
{              

    // Get Agent's Software Management interface by using the IKernelBase interface
    // that was passed to the driver by the EDD_EEDDExample_Initialize method.
    ISM* pISoftwareManagement = NULL;
    g_pIKernelBase->GetInterface(IID_SM, (void**) &pISoftwareManagement);
    if (pISoftwareManagement == NULL)
        return KE_INTERFACE_NOT_FOUND;  // SoftwareManagement component must be loaded.

    EString esHint(pszHint);
    EString esName(pszFileName);

    // Create a collection to hold each upload file object.
    // There is one hint per collection of files.  A hint handler is required on the 
    // DRM Enterprise Server to do extra processing on the upload files. 
    // The hint may be empty.
    EUploadFiles uploadFiles(esHint);

    // Define the file to be uploaded.  Set bDelete true if the file is to be deleted
    // after it is successfully uploaded.
    EUploadFile  uploadFile(esName, bDelete);
    uploadFiles.AddHead(&uploadFile);
    
    // This initiates a file upload request to the DRM Enterprise Server.
    // The Enterprise Server will then create a package requesting the file be uploaded
    // and send that package to the Agent's Software Management component to be processed.
    KERESULT ker = pISoftwareManagement->Upload(uploadFiles, &m_esMn, &m_esSn, &m_esOwner);

    return ker;

}


