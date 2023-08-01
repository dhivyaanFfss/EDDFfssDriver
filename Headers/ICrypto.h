//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ICrypto.h
//
//  Subsystem  :  Kernel
//
//  Description:  wrappers around some OpenSSL routines
//
//  Classes:      ICipher
//                EMD5
//
//**************************************************************************
#ifndef _ICRYPTO_H_
#define _ICRYPTO_H_

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

#define CHECKSUM_LEN        16          // length in bytes for the in/out MDXFinal array.

typedef enum
{
    ECipherTypeBlowfish,
    ECipherTypeAES
} ECipherType;

typedef enum
{
    ECipherModeDefault,
    ECipherModeECB,
    ECipherModeCBC
} ECipherMode;

typedef enum
{
    ECipherPaddingSimple,
    ECipherPaddingPKCS5
} ECipherPadding;

//*****************************************************************************
class _DV_DECLSPEC EOpenSSL
{
public:
    static bool     Initialize(void);
    static void     Shutdown(void);
    static void     RemoveErrorState(ULONG iThreadId);
    static const char *GetVersionString(void);
    static bool     IsFIPSMode(void);
    static const EString& GetDefaultCipherSuites(void);
    static EString  GetLastErrorMessage(void);
    static void    *GetDHParameters(int iKeyLength);
    static bool     ValidateCipherSuites(const EString& esCipherSuites);

private:
    static void     LockingCallback(int mode, int type, const char *file, int line);
    static ULONG    IdCallback(void);

    static EMutex  *m_pLocks;
    static bool     m_bInitialized;
    static EString  m_esDefaultCipherSuites;
};

//*****************************************************************************
// Wrapper class for MD5.
class _DV_DECLSPEC EMD5
{
public:
    EMD5();
    ~EMD5();
    void Init();
    void Update(BYTE* pData, DWORD len);
    // Returns 16 byte MD5 digest in the rByteArray
    void Final(EByteArray& rByteArray);
    // Returns MD5 digest in the user buffer (must be at least 16 byte long)
    void Final(UCHAR *pBuf);

protected:
    // Variables
    EString     m_esDigest;
    void        *m_pMD5CTX;
    bool        m_bInit;

};

//*****************************************************************************
// Wrapper class for SHA1.
class _DV_DECLSPEC ESHA1
{
public:
    ESHA1();
    ~ESHA1();
    void Init();
    void Update(BYTE* pData, DWORD len);
    // Returns 20 byte SHA1 digest in the rByteArray
    void Final(EByteArray& rByteArray);
    // Returns SHA1 digest in the user buffer (must be at least 20 byte long)
    void Final(UCHAR *pBuf);

protected:
    void        *m_pSHA1CTX;
    bool        m_bInit;
};

//*****************************************************************************
class _DV_DECLSPEC EHMAC
{
public:
    enum MDType { MD_TYPE_MD5, MD_TYPE_SHA1 };

    EHMAC(MDType mdType);
    ~EHMAC();
    void Init(BYTE *pKeyData, int iKeyLength);
    void Update(BYTE* pData, DWORD len);
    // Returns digest in the rByteArray
    void Final(EByteArray& rByteArray);
    // Returns digest in the user buffer (must long enough for used MD type)
    void Final(UCHAR *pBuf);

protected:
    MDType      m_mdType;
    void        *m_pHMACCTX;
    bool        m_bInit;
};

//*****************************************************************************
class _DV_DECLSPEC ECipherKey {
public:
    ECipherKey(const BYTE *pKey, size_t keylen);
    ~ECipherKey();

    BYTE *GetKey()        const { return m_AESKey; }
    size_t GetKeyLength() const { return m_keylen; }

private:
    BYTE   *m_AESKey;
    size_t  m_keylen;
};
//*****************************************************************************
interface ICipher
{
    virtual void        Destroy(void) = 0;
    virtual KERESULT    SetKey(BYTE *pData, int iLength, bool bEncrypt) = 0;
    virtual KERESULT    SetKey(const ECipherKey&, bool bEncrypt) = 0;
    virtual KERESULT    SetKeyFromFile(TCHAR *pFile, bool bEncrypt) = 0;
    virtual KERESULT    GenerateKeyFile(TCHAR *pFile) = 0;
    virtual void        SetPaddingChar(BYTE iPaddingChar) = 0;
    virtual void        EncryptBlock(BYTE *pIn, BYTE *pOut) = 0;
    virtual void        DecryptBlock(BYTE *pIn, BYTE *pOut) = 0;
    virtual KERESULT    Encrypt(BYTE *pIn, int iInLength, BYTE *pOut, int *piOutLength) = 0;
    virtual KERESULT    Decrypt(BYTE *pIn, int iInLength, BYTE *pOut, int *piOutLength) = 0;
    virtual int         GetBlockSize() = 0;
    virtual KERESULT    SetIV(BYTE *pData, int iLength) = 0;
    virtual bool        IsIVRequired(void) = 0;
};

_DV_DECLSPEC ICipher *ECipherCreate(ECipherType iType, ECipherMode iMode, ECipherPadding iPadding);

//*****************************************************************************
class _DV_DECLSPEC EPRNG
{
public:
    static void         GetData(void *pData, int iSize);
    static int          GetInt32(void);
    static int          GetInt32Max(void);
};

//*****************************************************************************
inline int EPRNG::GetInt32Max(void)
{
    return 2147483647;
}

#endif
