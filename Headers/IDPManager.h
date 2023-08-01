//**************************************************************************
//
//  Copyright (c) 1999-2011 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IDPManager.h
//  
//  Description:  Data Protection interfaces
//
//  Interface  :  IDPManager
//              
//**************************************************************************
#ifndef __I_DP_MANAGER_H__
#define __I_DP_MANAGER_H__

#ifdef WIN32
    #ifdef _DV_EXPORT
        #define _DV_DECLSPEC __declspec(dllexport)
        #define _DV_EXTERN
    #else
        #define _DV_DECLSPEC __declspec(dllimport)
        #define _DV_EXTERN extern
    #endif
#else
    #define _DV_DECLSPEC
    #define _DV_EXTERN
#endif

//**************************************************************************
interface IDPService
{
    virtual KERESULT EncryptData(const EByteArray& salt,
                                 const EByteArray& data, EByteArray& blob,
                                 DWORD iFlags) = 0;
    virtual KERESULT DecryptData(const EByteArray& salt,
                                 EByteArray& data, const EByteArray& blob,
                                 DWORD iFlags) = 0;
    virtual KERESULT CreateDataMAC(const EByteArray& salt,
                                   const EByteArray& data, EByteArray& blob,
                                   DWORD iFlags) = 0;
    virtual KERESULT VerifyDataMAC(const EByteArray& salt,
                                   const EByteArray& data, const EByteArray& blob,
                                   DWORD iFlags) = 0;
    virtual KERESULT CreateFileMAC(const EByteArray& salt,
                                   EFileIO& file, EByteArray& blob,
                                   DWORD iFlags) = 0;
    virtual KERESULT VerifyFileMAC(const EByteArray& salt,
                                   EFileIO& file, const EByteArray& blob,
                                   DWORD iFlags) = 0;
};

//**************************************************************************
interface IDPProvider : public IDPService
{
    virtual KERESULT Initialize(void) = 0;
    virtual KERESULT Shutdown(void) = 0;
    virtual KERESULT GetName(EString &esName) = 0;
};

//**************************************************************************
#define EDP_MANAGER_LEGACY 0x00000001

//**************************************************************************
interface IDPManager : public IDPService
{
    virtual KERESULT Initialize(void) = 0;
    virtual KERESULT Destroy(void) = 0;
    virtual KERESULT AddProvider(IDPProvider *pProvider) = 0;
    virtual KERESULT GetDefaultProvider(IDPProvider*& pProvider) = 0;
    virtual KERESULT SetDefaultProvider(const EString& esName) = 0;

    virtual KERESULT EncryptText(const EByteArray& salt,
                                 const EString& esText, EString& esBlob,
                                 DWORD iFlags) = 0;
    virtual KERESULT DecryptText(const EByteArray& salt,
                                 EString& esText, const EString& esBlob,
                                 DWORD iFlags) = 0;
};

//**************************************************************************
class _DV_DECLSPEC EDPManagerFactory
{
public:
    EDPManagerFactory(IKernelBase *pIKernelBase);
    IDPManager *CreateDPManager(void);

private:
    IKernelBase *m_pIKernelBase;
};

//**************************************************************************
class _DV_DECLSPEC EDPUtil
{
public:
    static void ReadUInt16(USHORT& iNumber, const BYTE *pBytes);
    static bool ReadUInt16(USHORT& iNumber, const BYTE*& pData, int& iSize);
    static void WriteUInt16(USHORT iNumber, BYTE *pBytes);
    static bool WriteUInt16(USHORT iNumber, BYTE*& pData, int& iSize);
    static void ReadUInt32(EULONG32& iNumber, const BYTE *pBytes);
    static bool ReadUInt32(EULONG32& iNumber, const BYTE*& pData, int& iSize);
    static void WriteUInt32(EULONG32 iNumber, BYTE *pBytes);
    static bool WriteUInt32(EULONG32 iNumber, BYTE*& pData, int& iSize);
    static bool ReadByteArray16(EByteArray& array, const BYTE*& pData,
                                int& iSize);
    static bool WriteByteArray16(const EByteArray& array, BYTE*& pData,
                                 int& iSize);
    static bool ReadByteArray32(EByteArray& array, const BYTE*& pData,
                                int& iSize);
    static bool WriteByteArray32(const EByteArray& array, BYTE*& pData,
                                 int& iSize);
};

#endif
