//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  TriggerListener.h
//  
//  Subsystem  :  Axeda Connector
//
//  Description:  Define ETrigger object
//
//  Classes:
//          ETriggerListener
//          ETriggerListeners
//
//**************************************************************************
#ifndef __ETRIGGER_LISTENER_H__
#define __ETRIGGER_LISTENER_H__

//**************************************************************************
struct ETriggerListener
{
    ETriggerListener(ITriggerListener* pITriggerListener, EHANDLE hClient)
        : m_pITriggerListener(pITriggerListener), m_hClient(hClient)
    {
    }
    ITriggerListener*   m_pITriggerListener;
    EHANDLE             m_hClient;
};


typedef ETypedPtrList<EPtrList, ETriggerListener*>    ETriggerListeners;

#endif
