//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  EDynamicLibrary.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Generic Object for DLL
//
//  Classes    :  EDynamicLibrary
//
//**************************************************************************
#ifndef __EDYNAMICLIBRARY__H__
#define __EDYNAMICLIBRARY__H__

#ifdef WIN32
    #ifdef _DV_EXPORT
       #define _DV_DECLSPEC __declspec(dllexport)
       #define _DV_EXTERN
    #else
       #define _DV_DECLSPEC __declspec(dllimport)
       #define _DV_EXTERN   extern
    #endif
#else
   #define _DV_DECLSPEC
   #define _DV_EXTERN
#endif

#ifdef WIN32 
    #define EDLLHANDLE  HMODULE 
#else 
#ifdef UNIX
    #define EDLLHANDLE  void*
#endif
#endif
   
#ifndef EDLLHANDLE
 #define EDLLHANDLE  int
#endif

class _DV_DECLSPEC EDynamicLibrary
{
 private:
     bool           m_bLoaded;                 // Loaded Flag
     EDLLHANDLE     m_hLibHandle;              // Library Handle
     EString        m_sName;                   // Library Name


 public:
        EDynamicLibrary(TCHAR* pName);         // Constructor
        ~EDynamicLibrary();                    // Destructor

        KERESULT Load(EString *pError = 0);    // Load Library
        KERESULT Unload();                     // Free Library

        KERESULT GetFuncPointer(TCHAR *pFncName, void** pFnc);

        bool IsLoaded() { return m_bLoaded; }
        EString&    GetName() {return m_sName;}


};

#endif //__EDYNAMICLIBRARY__H__

