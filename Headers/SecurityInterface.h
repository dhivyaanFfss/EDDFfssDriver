//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  SecurityInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Security Manager Interface
//
//  Classes:      ISecurity
//
//************************************************************************** */

#ifndef __SECURITY_INTERFACE_H__
#define __SECURITY_INTERFACE_H__

#include "KernelInterface.h"

/* Group=Security */

typedef struct EUserStruct
{
    TCHAR*       pszUserName;         /* User name */
    TCHAR*       pszDescription;      /* description*/
    TCHAR*       pszPassword;         /* Password */
} EUserStruct;

typedef struct EGroupStruct
{
    TCHAR*       pszGroupName;         /* Group name */
    TCHAR**      ppszUserNames;        /* List of user names */
    DWORD        nUsers;               /* Number of users in list */
} EGroupStruct;

////////////////////////////////////////////////////////////////////////
interface ISecurity : public IKernelBase
{
    // login
    virtual     KERESULT    ERequestLogin(EString& theUserName, EString& theChallenge) = 0;
    virtual     KERESULT    ELogin(EString& theUserName, EString& theDigest) = 0;
    virtual     KERESULT    ELogout(EString& theUserName) = 0;

    // authorization
    virtual     KERESULT    IsUserMemberofGroup(EString& theGroupName, EString& theUserName, bool* pResult) = 0;

    // user
    virtual     KERESULT    SetUser(EUserStruct* pUser) = 0;
    virtual     KERESULT    DeleteUser(EString& theUserName) = 0;
    virtual     KERESULT    GetFirstUser(EUserStruct* pUser) = 0;
    virtual     KERESULT    GetNextUser(EUserStruct* pUser) = 0;

    // groups
    virtual     KERESULT    SetGroup(EGroupStruct* pGroup) = 0;
    virtual     KERESULT    DeleteGroup(EString& theGroupName) = 0;
    virtual     KERESULT    GetFirstGroup(EGroupStruct* pGroup) = 0;
    virtual     KERESULT    GetNextGroup(EGroupStruct* pGroup) = 0;
};

#endif

