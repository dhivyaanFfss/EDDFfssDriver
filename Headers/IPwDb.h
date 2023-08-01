//**************************************************************************
//
//  Copyright (c) 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IPwDb.h
//  
//  Description:  Password database access library API
//
//  Classes:      EPwDb
//
//**************************************************************************
#ifndef _IPWDB_H_
#define _IPWDB_H_

#ifdef WIN32
    #ifdef EPWDB_EXPORTS
       #define EPWDB_DECLSPEC __declspec(dllexport)
       #define EPWDB_EXTERN
    #else
       #define EPWDB_DECLSPEC __declspec(dllimport)
       #define EPWDB_EXTERN   extern
    #endif
#else
   #define EPWDB_DECLSPEC
   #define EPWDB_EXTERN
#endif

//**************************************************************************
// Password database errors
typedef enum
{
    EPwEOK,                 // Operation successful
    EPwEMemory,             // Out of memory
    EPwEKeyFile,            // Invalid or missing key file
    EPwEDbNotFound,         // Password database not found (add entries to create)
    EPwEDbOpen,             // Could not open password database
    EPwEDbIO,               // Password database read/write error
    EPwEDbDecrypt,          // Could not decrypt password database (wrong key?)
    EPwENotFound,           // Entry not found in password database
    EPwEExist               // Entry already exists in password database
} EPwResult;

//**************************************************************************
// Password database entry (record)
class EPWDB_DECLSPEC EPwEntry
{
public:
    EPwEntry(void);
    ~EPwEntry();

    // Compares host and user names to the ones in pEntry. Returns true when both
    // values match.
    bool    MatchesTo(EPwEntry *pEntry);

    // Copies all entry attributes to pEntry.
    void    CopyValueTo(EPwEntry *pEntry);

    char    *m_pHost;           // Host name
    char    *m_pUser;           // User name
    char    *m_pPassword;       // Password
    char    *m_pParameters;     // Parameter list (format: name1=value1;name2=value2[;...])
};

typedef ETypedPtrList<EPtrList, EPwEntry*> EPwEntryList;

//**************************************************************************
// Password database methods
// Notes:
// 1. Password database entries match when their host and user names match.
// 2. In the following methods the file name, "aeskey.txt," is assumed when
//    the key file argument is not specified.
class EPWDB_DECLSPEC EPwDb
{
public:
    // Generates new AES key file for the database file.
    static EPwResult    GenerateKey(TCHAR *pDbFile, TCHAR *pKeyFile = NULL);

    // Searches the database for the first record matching pEntry. Returns EPwENotFound
    // when record is not found. Otherwise, password and parameter list in pEntry
    // are updated.
    static EPwResult    Get(TCHAR *pDbFile, EPwEntry *pEntry, TCHAR *pKeyFile = NULL);

    // Returns all records in the database, using the array, pEntryList.
    static EPwResult    GetAll(TCHAR *pDbFile, EPwEntryList *pEntryList, TCHAR *pKeyFile = NULL);

    // Searches the database for the first record matching pEntry and modifies it,
    // replacing its password and parameter list values. Returns EPwENotFound when
    // target record is not found.
    static EPwResult    Modify(TCHAR *pDbFile, EPwEntry *pEntry, TCHAR *pKeyFile = NULL);

    // Inserts the record specified by pEntry. Returns EPwEExist when matching record
    // already exists in the database
    static EPwResult    Insert(TCHAR *pDbFile, EPwEntry *pEntry, TCHAR *pKeyFile = NULL);
    
    // Deletes the record specified by pEntry. Returns EPwENotFound when target record
    // is not found.
    static EPwResult    Delete(TCHAR *pDbFile, EPwEntry *pEntry, TCHAR *pKeyFile = NULL);

    // Updates user and host names of pEntry according to pAccount (format: host@user)
    static void         ParseAccount(char *pAccount, EPwEntry *pEntry);

    // Searches parameter by name (pName) in the parameter list specified by pParams. Returns
    // parameter value or, when parameter is not found, NULL. Caller is responsible for
    // freeing the returned value.
    static char         *GetParameterValue(char *pParams, char *pName);

    // Returns static string with textual description of the error specified by iResult.
    static char         *GetErrorString(EPwResult iResult);
};

#endif
