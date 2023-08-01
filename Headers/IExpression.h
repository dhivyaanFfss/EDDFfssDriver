//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IExpression.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Expression interface definitions.
//
//  Classes:                                         
//          IExpression
//
//**************************************************************************

#ifndef __EXPRESSION_INTERFACE_H__
#define __EXPRESSION_INTERFACE_H__

/* Group=Expression */

//**************************************************************************
// ??? eventually needs to be ERemoteAPI.h
typedef struct 
{
    TCHAR*  pszName;
    TCHAR*  pszExpression;
    TCHAR*  pszMessage;
    DWORD   dwSeverity;
} EExpressionStruct;



interface IExpressionMgr : public IKernelBase
{
    virtual KERESULT    AddExpression(EExpressionStruct& rExpStruct) = 0;
    virtual KERESULT    RemoveExpression(TCHAR* pszName) = 0;

    // Invoke processing action. Sent from IDataBlock component when
    // all Uniform data blocks and individual values have been published.
    // Future:  use IDataBlockListenerSync.
    // rChangedDataItems - returns with list of data item pointers whose data
    // has been modified. 
    virtual KERESULT    DoProcessData(/* in/out */CDataItemList& rChangedDataItems) = 0; 


    // Separate DataProcessing from trigger notification.  
    // For example, must  set all Expression values before doing notification of 
    // Expression triggers.  Triggers must happen after ALL data has been published
    // by the Tag - Block manager.
    virtual KERESULT DoNotification() = 0; 

};

#endif


