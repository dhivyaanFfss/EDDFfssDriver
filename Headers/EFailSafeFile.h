//**************************************************************************
//
//  Copyright © 1999-2011 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EFailSafeFile.h
//  
//  Description:  An abstraction for handling files an a fail-safe manner.
//
//  Classes:      EFailSafeFile, EFailSafeSinkFile, EFailSafeSourceFile,
//                EFailSafeStringSink, EFailSafeXmlSource, EFailSafeFileCopy
//
//**************************************************************************
#ifndef EFILE_SAFE_FILE_H
#define EFILE_SAFE_FILE_H

#include "KernelErrors.h"
#include "KernelInterface.h"
#include "ECollections.h"
#include "EFileIO.h"
#include "IWebProvider.h"
#include "ITextParser.h"
#include "ISax.h"
#include "EXMLData.h"

#ifdef WIN32
    #ifdef _DV_EXPORT
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

//**************************************************************************
class _DV_DECLSPEC EFailSafeFile
{
public:
    EFailSafeFile(const TCHAR *pszName);
    virtual ~EFailSafeFile();

    EString& GetFileName(void);
    EFileIO *GetFile(void);
    void SetFile(EFileIO *pFile);
    static const TCHAR *GetBackupFileSuffix(void);

protected:
    void SetFileName(const TCHAR *pszName);
    bool OpenFile(const TCHAR *pName, EULONG32 ulFlags);
    void CloseFile(void);

    EString m_esBaseName;
    EString m_esOrigName;
    EString m_esNewName;

    static bool m_bFileRenameAtomic;
    static const TCHAR ORIG_FILE_SUFFIX[];
    static const TCHAR NEW_FILE_SUFFIX[];
    static const TCHAR BACKUP_FILE_SUFFIX[];

private:
    EFileIO *m_pFile;
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeSinkFile : public EFailSafeFile
{
public:
    EFailSafeSinkFile(const TCHAR *pszName);

    virtual bool Init(void);
    virtual bool Finish(bool bSuccess);
    virtual bool Save(void);

protected:
    virtual bool Write(void);
    virtual bool Commit(void);
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeSourceFile : public EFailSafeFile
{
public:
    EFailSafeSourceFile(const TCHAR *pszName, bool bUseBackup = true);

    virtual bool Init(void);
    virtual bool Load(void);

    bool IsMissing(void);
    bool IsUseBackup(void);

protected:
    virtual bool Read(void);
    virtual void Reset(void);

private:
    bool HandleCommitFailure(void);

    bool m_bUseBackup;
    bool m_bMissing;
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeStringSink : public EFailSafeSinkFile
{
public:
    EFailSafeStringSink(const TCHAR *pszName, const EString& esContent,
                        EFILE_WRITE_MODE iEncoding = EFILE_WRITE_UTF8ENCODE);

protected:
    virtual bool Write(void);

private:
    EString m_esContent;
    EFILE_WRITE_MODE m_iEncoding;
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeXmlSource : public EFailSafeSourceFile
{
public:
    EFailSafeXmlSource(const TCHAR *pszName, ISax *pSax,
                       bool bUseBackup = true);

    KERESULT GetError(void);

protected:
    virtual bool Read(void);

private:
    ISax *m_pSax;
    KERESULT m_iError;
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeXmlSink : public EFailSafeSinkFile,
                                      public IByteSink
{
public:
    EFailSafeXmlSink(const TCHAR *pszName, EXMLDocument *pDocument);
    bool    Open(void);
    void    Close(void);
    bool    Write(const char *pData, int iSize);

protected:
    virtual bool Write(void);

private:
    EXMLDocument *m_pDocument;
    bool m_bSuccess;
};

//**************************************************************************
class _DV_DECLSPEC EFailSafeFileCopy : public EFailSafeSinkFile
{
public:
    EFailSafeFileCopy(const TCHAR *pszSrcName, const TCHAR *pszDstName);

protected:
    virtual bool Write(void);

private:
    EString m_esSrcName;

    static const size_t BUFFER_SIZE;
};

//**************************************************************************
inline EString& EFailSafeFile::GetFileName(void)
{
    return m_esBaseName;
}

//**************************************************************************
inline EFileIO *EFailSafeFile::GetFile(void)
{
    return m_pFile;
}

//**************************************************************************
inline void EFailSafeFile::SetFile(EFileIO *pFile)
{
    m_pFile = pFile;
}

//**************************************************************************
inline const TCHAR *EFailSafeFile::GetBackupFileSuffix(void)
{
    return BACKUP_FILE_SUFFIX;
}

//**************************************************************************
inline bool EFailSafeSourceFile::IsMissing(void)
{
    return m_bMissing;
}

//**************************************************************************
inline bool EFailSafeSourceFile::IsUseBackup(void)
{
    return m_bUseBackup;
}

//**************************************************************************
inline KERESULT EFailSafeXmlSource::GetError(void)
{
    return m_iError;
}

//**************************************************************************
inline bool EFailSafeXmlSink::Open(void)
{
    return GetFile()->IsOpen();
}

//**************************************************************************
inline void EFailSafeXmlSink::Close(void)
{
}

#endif
