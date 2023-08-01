//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IEDDDataSource.h
//  
//  Subsystem  :  Gateway - EDD-specific Data Source interface
//
//  Description:  IEDDDataSource interface to be used by Tag Manager for
//				  EDD-specific Data Source manipulation
//
//  Classes:
//                IEDDDataSource
//
//**************************************************************************


#ifndef __IEDDDATASOURCE_H__
#define __IEDDDATASOURCE_H__

interface IEDDDataSource
{
    virtual HDSITEM AddGroup(int gatewayId) = 0;
    virtual HDSITEM AddGroupItem(HDSITEM hGroup, const TCHAR *pszName, IGetItemType *pIGetType,
        bool bInitialState, int gatewayId) = 0;
};

#endif


