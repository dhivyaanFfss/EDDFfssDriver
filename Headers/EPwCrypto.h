//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EPwCrypto.h
//  
//  Description:  Password encryption routines
//
//**************************************************************************
#ifndef _EPWCRYPTO_H_
#define _EPWCRYPTO_H_

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

#define AX_NO_ERROR     0
#define AX_ERROR        -1

#define AX_MIN_BUF_LEN  50

#ifdef __cplusplus
extern "C"
{
#endif

/* Encrypts zero-terminated password in pszClearText and writes the result
   into pszCipherText. iBufferLength specifies the size of the output
   buffer. The function will return -1 if the output buffer is too small,
   or 0 otherwise. iBufferLength must be at least:
   
   iBufferLength = (strlen(pszClearText) + 1) * 4 + 1;
   
   The resulting ciphertext is to be used for storing passwords in
   {E|xg}EnterpriseProxy.xml.

   Note that the output is UTF-8 encoded, so it should be placed into XML
   as is, without applying UTF-8 encoding.
 */
_DV_DECLSPEC
int EncryptProxyPassword(/* in */ char *pszClearText,
                         /* out */ char *pszCipherText,
                         /* in */ int iBufferLength);

/* Encrypts zero-terminated password in pszClearText and writes the result
   into pszCipherText. iBufferLength specifies the size of the output
   buffer. The function will return -1 if the output buffer is too small,
   or 0 otherwise. iBufferLength must be at least:
   
   iBufferLength = ((strlen(pszClearText) + 7 & ~0x07) + 2) / 3 * 4 + 1;
   
   The resulting ciphertext is to be used for storing passwords in
   aremote.xml.

   Note that the output is a base64 encoded string.
 */
 _DV_DECLSPEC
int EncryptAccessPassword(/* in */ char *pszClearText,
                          /* out */ char *pszCipherText,
                          /* in */ int iBufferLength);


/*
	Function:  EncrypteDUPassword
    Returns:
        0 for success and non-zero for an error.
    Parameters:
		pszClearText is the password to be encoded.
		pszCipherText is the result of the encoding.  
            Must be a pointer to a char array of at least 50 characters.  
            Forcing the caller to define the memory removes memory 
            allocation and deallocation issues.
        iBufferLength is the length of the pszCipherText array.
*/
_DV_DECLSPEC
int EncryptDUPassword(/* in */ char *pszClearText,
                       /* out */ char *pszCipherText,
                       /* in */ int iBufferLength);


/*  
      Function:  GetDUFileChecksum 

      Returns:
         0 for success and non-zero for an error.
      Parameters:
         pszFileName the filename of the file for which to calculate the checksum.
                     The file must be in the path or be fully qualified.
         checkSumLen is the length off the pszChecksum array.
         pszChecksum contains the resultant checksum. 
                     Must be a pointer to a char array of at least 50 characters.  
                     Forcing the caller to define the memory removes memory 
                     allocation and deallocation issues.
*/

_DV_DECLSPEC
 int GetDUFileChecksum(/* in */ char* pszFileName, 
                       /* in */ int checkSumLen, 
                       /* out */ char* pszChecksum);

		


#ifdef __cplusplus
}
#endif

#endif
