//*****************************************************************************
//
//  Copyright © 1999-2009 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  ISSLKeyConfig.h
//
//  Subsystem  :  Connector
//
//  Description:  exported interface to define the SSL Key configuration.
//
//  Classes:
//
//*****************************************************************************

#ifndef __ISSL_KEY_CONFIG_H__
#define __ISSL_KEY_CONFIG_H__


#ifdef WIN32
    #ifdef _AXEDA_SSL_KEY_CONFIG_EXPORTS
        #define _AXEDA_SSL_KEY_CONFIG_DECLSPEC __declspec(dllexport)
        #define _AXEDA_SSL_KEY_CONFIG_EXTERN
    #else
        #define _AXEDA_SSL_KEY_CONFIG_DECLSPEC __declspec(dllimport)
        #define _AXEDA_SSL_KEY_CONFIG_EXTERN   extern
    #endif
#else
    #define _AXEDA_SSL_KEY_CONFIG_DECLSPEC
    #define _AXEDA_SSL_KEY_CONFIG_EXTERN
#endif


#ifdef __cplusplus
extern "C" {
#endif

// Status codes.  OK is >= 0.  Failed is < 0
#define SSL_KEY_CONFIG_SUCCESSFULLY_SAVED_TO_FILE			2
#define SSL_KEY_CONFIG_SUCCESSFULLY_SAVED_TO_SECURE_STORE	1
#define SSL_KEY_CONFIG_NO_ERROR								0
#define SSK_KEY_CONFIG_INVALID_PARAMETERS_ERROR				-1
#define SSK_KEY_CONFIG_SAVE_FAILED_ERROR					-2

/********************************************************************************/
/*  ***************************  Function Definitions  **********************/
/********************************************************************************/

typedef int(*SAVE_SSL_KEY_CONFIG_FUNCTION)(char*, char*, char*, char*, char*, int);
typedef int(*SAVE_SSL_KEY_CONFIG_EX_FUNCTION)(char*, char*, char*, char*, char*, int, char*);

typedef int(*GET_EXTENDED_ERROR_INFO_FUNCTION)(int*, char*, int);

// pszSslKeyConfigPath is the path for storing the SSL Key configuration XML file.
// if pszSslKeyConfigPath is NULL, the Agent root directory is used
// All other parameters must not be NULL.
// returns:
//  - SSK_KEY_CONFIG_INVALID_PARAMETERS_ERROR for a NULL pointer.
//  - SSK_KEY_CONFIG_SAVE_FAILED_ERROR if the persistence of the information was not successful.
//  - SSL_KEY_CONFIG_SUCCESSFULLY_SAVED_TO_FILE for successful storage of 
//      information to file
//  - SSL_KEY_CONFIG_SUCCESSFULLY_SAVED_TO_SECURE_STORE for successful storage of 
//      information to secure storage

_AXEDA_SSL_KEY_CONFIG_DECLSPEC int SaveSSLKeyConfig(char* pszSslKeyConfigDirectory,
                                                    char* pszCertCAFile, 
                                                    char* pszCertFile, 
                                                    char* pszPrivateKeyFile, 
                                                    char* pszPrivateKeyPassword,
                                                    int   bForceSecureStore);
_AXEDA_SSL_KEY_CONFIG_DECLSPEC int SaveSSLKeyConfigEx(char* pszSslKeyConfigDirectory,
                                                    char* pszCertCAFile, 
                                                    char* pszCertFile, 
                                                    char* pszPrivateKeyFile, 
                                                    char* pszPrivateKeyPassword,
                                                    int   bForceSecureStore,
													char* pszConfigString);

// GetExtendedErrorInfo: 
// Get additional error information regarding saving the SSL Key config 
// in a secure store.
// 
// Returns:  int error code #.  This is secure storage specific.
// returns:
//  - SSK_KEY_CONFIG_INVALID_PARAMETERS_ERROR for a NULL pointer.
//  - SSL_KEY_CONFIG_NO_ERROR for successful setting of output parameters.
// pSecureStoreErrCode:  pointer to integer set to the extended error code.
//                       The error code is secure storage specific.
// pszErrorString:  pointer to character buffer to hold the error string.
// errStringLength: length of character string buffer.
_AXEDA_SSL_KEY_CONFIG_DECLSPEC int GetExtendedErrorInfo(/* out */ int* pSecureStoreErrCode,
														/* out */ char* pszErrorString, 
														/* in */ int errStringLength);

#ifdef __cplusplus
}
#endif


#endif  // __ISSL_KEY_CONFIG_H__

