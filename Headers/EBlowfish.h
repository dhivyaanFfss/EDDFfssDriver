//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EBlowfish.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Secret
//
//  Classes:    
//
//**************************************************************************
#ifndef _EBLOWFISH_H__
#define _EBLOWFISH_H__

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

typedef struct
{
    EULONG32 P[16 + 2];
    EULONG32 S[4][256];
}
BLOWFISH_CTX;

//**************************************************************************
// 
// 
//
class _DV_DECLSPEC EBlowfish 
{
public:
    EBlowfish();
    ~EBlowfish();
    KERESULT EncryptBuffer( BYTE* pOut, BYTE* pSrc, int len);
    KERESULT DecryptBuffer( BYTE* pOut, BYTE* pSrc, int len);
    BYTE* GetBlowable(BYTE *pIn,int dataLen,int *dataLenOut);
    bool    InitBlowfish();
private:
    bool     ExtractKey(TCHAR *fname,char *key);
    KERESULT BlowfishEncrypt( char** pOut, char* pSrc, int len, bool encrypt, bool strict);
    KERESULT CryptDeInit();
    KERESULT CryptInit(unsigned char *key, int keyLen);
    KERESULT Decrypt( EULONG32 *xl, EULONG32 *xr);
    KERESULT Encrypt( EULONG32 *xl, EULONG32 *xr);
    EULONG32 F( EULONG32 x);
    DWORD* m_pXsrc;
    long m_xsrclen;
    BLOWFISH_CTX *ctx;
};


#endif
