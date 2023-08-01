/**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
**************************************************************************
//
//  Filename   :  IDataBlock.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Data Block interfaces.
//
//  Classes:        
//          IDataBlockListener                                 
//          IDataBlock
//
**************************************************************************/

#ifndef __IDATA_BLOCK_H__
#define __IDATA_BLOCK_H__

//*****************************************************************************
interface IDataBlockListener
{
    virtual KERESULT    OnBlockChanged(DWORD count, EHANDLE* hClients, CDataValue*) = 0;
};

/* future concept.  If used, modify IExpression
*****************************************************************************
// Interface to be expanded in future.  Used to synchronize control between
// IDataBlock and objects that need control status.
// Conceptually, in the future, there will be a subscribe function in IDataBlock
// for this interface.  
// Per 2.5 release, this is only implemented by the Expression component and that
// information is explicitly used.
interface IDataBlockListenerSync
{
    // For Synchronous process control.
    
    // Invoke processing action. Sent from IDataBlock component when
    // all Uniform data blocks and individual values have been published.
    // Future:  could use hClient parameter to pass back hClient sent in subscribe.
    virtual KERESULT DoProcessData() = 0; 

  // Separate DataProcessing from trigger notification
    virtual KERESULT DoNotification() = 0; 
};
*/

//*****************************************************************************
interface IDataBlock
{
    virtual KERESULT Subscribe(IDataBlockListener*, DWORD count, TCHAR **ppszItemNames, 
        EHANDLE *phClientIds, /* out */ EHANDLE* phSubscription) = 0;
    virtual KERESULT Unsubscribe(EHANDLE hSubscription) = 0;
};


#endif
