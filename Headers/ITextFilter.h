//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ITextfilter.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Interface for text processing filter
//
//  Classes:      ITextFilter
//
//**************************************************************************
#ifndef _ITEXTFILTER_H
#define _ITEXTFILTER_H

#ifdef WIN32
#ifdef _DV_EXPORT
#define EPARSER_DECL __declspec(dllexport)
#else
#define EPARSER_DECL __declspec(dllimport)
#endif
#else
#define EPARSER_DECL
#endif

/* Group=Text Filter */

interface EPARSER_DECL ITextFilter
{
public:
    virtual ~ITextFilter(void) {};
    virtual TCHAR* GetContent(KERESULT* pError) = 0;
};
#endif // _ITEXTFILTER_H



