//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  EDDBrowser.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//  Classes    :  
//                EDDBrowser
//
//*****************************************************************************

#ifndef __EDD_BROWSER_H__
#define __EDD_BROWSER_H__
#include "json.hpp"

//*****************************************************************************
class EDDBrowser
{
public:

    EDDBrowser();
    ~EDDBrowser();

    KERESULT Init();
    DRIVER_RESULT BrowseItems(DWORD& dwCount, TCHAR**& ppszItems, DWORD*& pdwAttributes);
    void Add(EDDDataItem* pItem) { m_items.AddTail(pItem); }

protected:
    // Support functions

    // members
    EDDDataItems   m_items;
    nlohmann::json m_json;
    nlohmann::json m_deviceDataJson;


};

#endif // __EDD_BROWSER_H__
