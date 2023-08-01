//********************************************************************************
//
//  Copyright © 1999-2009 Axeda Corporation.  All rights reserved.
//
//********************************************************************************
//
//  Filename   :  ISecureStoreMgr.h
//  
//  Subsystem  :  Kernel
//
//  Description:  General interface for secure storage of information
//                
//
//  Classes:
//              ISecureStoreMgr       
//
//********************************************************************************

#ifndef __ISECURE_STORE_MGR_H__
#define __ISECURE_STORE_MGR_H__

#include "KernelInterface.h"

//********************************************************************************
interface ISecureStoreMgr : public IKernelBase
{
    // returns:  KE_OK for success;  KE_FAILED for any failure.
	// if esName is empty, a default name is used.
    virtual KERESULT Open(EString& esName) = 0;
    virtual KERESULT Create(EString& esName) = 0;
    virtual KERESULT Backup() = 0;
    virtual KERESULT Close() = 0;

    // Write errors return:  KE_WRITE_OBJECT
    // Read errors return:  KE_READ_OBJECT
    virtual KERESULT WriteText(EString& esLabel, EString& esValue) = 0;
    virtual KERESULT WriteBinary(EString& esLabel, BYTE* pBinary, int binaryLength) = 0;
    virtual KERESULT ReadText(EString& esLabel, /* out */ EString& esValue) = 0;
    virtual KERESULT ReadBinary(EString& esLabel, /* out */ EByteArray& binaryArray) = 0;

    // For an error return, additional error information is avaialble
    virtual KERESULT GetErrorCode(/* out */ int& errCode) = 0;
    virtual KERESULT GetErrorString(/* out */ EString& esError) = 0;

    virtual KERESULT SetProperties(char* pszConfigString) = 0;
};


#endif  // __ISECURE_STORE_MGR_H__
