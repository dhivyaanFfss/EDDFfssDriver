//*****************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved
//
//*****************************************************************************
//
//  Filename   :  TarGzip.h
//  
//  Subsystem  :  Axeda Connector/Gateway
//
//  Description:  Define the ETarGzip object to manage file compression using
//                Tar and Gzip.  (See the TarGzip library).  
//
//                Define IFileCompression which can be used by the SCM component
//                to allow a custom file compression implementation.
//
//  Classes:
//              IFileCompression
//              ETarGzip
//
//*****************************************************************************
#ifndef __TAR_GZIP_H__
#define __TAR_GZIP_H__

#ifdef WIN32
    #ifdef _DV_EXPORT_ETARGZIP
        #define _TARGZIP_DECLSPEC __declspec(dllexport)
        #define _TARGZIP_EXTERN
    #else
        #define _TARGZIP_DECLSPEC __declspec(dllimport)
        #define _TARGZIP_EXTERN   extern
    #endif
#else
    #define _TARGZIP_DECLSPEC
    #define _TARGZIP_EXTERN
#endif


typedef ETypedPtrList<EPtrList, EString*>   EStringList;

//*****************************************************************************
interface IFileCompression
{
    // Create Tar-GZip from input files.
    // resFileName is an in/out parameter.  If there is not an extension on 
    // the filename, Create() is expected to add it per the default of the
    // compression algorithm. For example, TarGzip will add .tar.gz
    // A WinZip compression implemenation might add .zip.
    virtual KERESULT Create(EString& resFileName, EStringList& resFiles,
        bool bSaveFullPathInfo = true) = 0;
    // Extract from Tar-GZip the files to the destination
    virtual KERESULT Extract(EString& resFileName, EString& resDestination) = 0;
    // List the files in the Tar-GZip file; output contained in resFiles.
    // A MStringList manages the life its own EString* 
    virtual KERESULT List(EString& resFileName, MStringList& resFiles) = 0;
};

//*****************************************************************************
class _TARGZIP_DECLSPEC ETarGzip : public IFileCompression
{
public:
    ETarGzip();
    ~ETarGzip();

    // Implement IFileCompression

    // Create Tar-GZip from input files.  Add .tar.gz to resFilename if
    // there is no extention.
    KERESULT Create(EString& resFileName, EStringList& resFiles, bool bSaveFullPathInfo = true);
    // Extract from Tar-GZip the files to the destination
    KERESULT Extract(EString& resFileName, EString& resDestination);
    // List the files in the Tar-GZip file; output contained in resFiles.
    KERESULT List(EString& resFileName, MStringList& resFiles);

protected:
    // Support functions
    KERESULT ConvertFileName( /* in */ char* pszFileName, bool bSaveFullPathInfo,
        /* in/out */ char* pszDrive, 
        /* in/out */ char* pszActualFileName);
};

#endif
