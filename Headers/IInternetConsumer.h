//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IInternetConsumer.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Consumer of internet services (like EEnterpriseProxy or Mail)
//
//**************************************************************************

#ifndef __IINTERNET_CONSUMER_H__
#define __IINTERNET_CONSUMER_H__


//**************************************************************************
interface IInternetConsumer
{
    virtual KERESULT DoInternetStart() = 0;
    virtual KERESULT DoInternetStop() = 0;
};

#endif


