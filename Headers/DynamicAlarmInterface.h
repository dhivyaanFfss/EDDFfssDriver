//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  DynamicAlarmInterface.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define Dynamic Alarm
//
//  Classes:
//              IAlarmListener
//              IAlarmFilter
//              IDynAlarm
//
//************************************************************************** 

#ifndef __DYNALARM_INTERFACE_H__
#define __DYNALARM_INTERFACE_H__

/* Group=Alarms */

// {secret}
 enum AlarmFilterType
   {
      BY_DATAITEMS, BY_AREAS
   };

// See Also: IAlarmFilter::GetFilter 
typedef struct FilterParam
{
	TCHAR*      pServer;
	TCHAR*		pFilterName;		/* Ignored!  For future use in hierarchy of alarms*/
    long		nCategories;		/* Number of categories*/
    long*	    pCategories;		/* Array of categories*/
    long        lowSeverity;		/* The lowest value that the alarm filters */
    long        highSeverity;		/* The highest value that the alarm filters */
    long        nAreas;				/* Ignored!  For future use in hierarchy of alarms*/
    TCHAR**     ppszAreas;			/* ignored!  For future use in hierarchy of alarms*/
    long        nItems;				/* Number of Data Items*/
    TCHAR**     ppszItems;			/* Pointer to the array of Data Items*/
}	FilterParam;


////////////////////////////////////////////////////////////////////////
// Client-side Alarm Listener : interface definition.

interface IAlarmListener
{
   virtual void OnCondEvent(LONG_PTR nClientId, CEventBuf* pEvents) = 0;
};


////////////////////////////////////////////////////////////////////////
// Alarm Filter : interface definition.

interface IAlarmFilter : public IKernelBase
{
   virtual HFILTER
                  SetFilter(const TCHAR* pszServer,
							const TCHAR* pszFilterName,
                            int nAreas, const TCHAR** ppszAreas,
                            int nItems, const TCHAR** ppszItems,
                            int nCatCnt, long* pCategories,
                            int nLowSev, int nHighSev,
                            int* pError = 0) = 0;

   virtual int    RemoveFilter(HFILTER) = 0;

   virtual int    GetFilter(HFILTER, FilterParam*) = 0;

   virtual int    GetFilterID( TCHAR* pszFilterName, HFILTER* hFilter ) = 0;

   virtual KERESULT Subscribe(HSUBSCRIPTION *, //out 
                            IAlarmListener*, HFILTER, LONG_PTR nClientId,
                            long nBufSize, long nBufTime, bool bGetHistory = false) = 0;
   virtual KERESULT Unsubscribe(HSUBSCRIPTION) = 0;
};


////////////////////////////////////////////////////////////////////////
// Dynamic Alarm : interface definition.

interface IDynAlarm : public IKernelBase
{
   // Acknowledges an alarm condition.  The pAcknowledger and pComment strings
   // are currently ignored.  The pCookies (or Cookie) are the dwCookie values from the 
   // ONEVENTSTRUCT that was passed by the EAlarmListener.OnOPCEvent method.
   virtual int    Acknowledge(int nEvents, 
                              const TCHAR* pAcknowledger, const TCHAR* pComment,
                              HEvent* pCookies, long* Errors) = 0;

   virtual int    Acknowledge(const TCHAR* pAcknowledger, const TCHAR* pComment,
                              const HEvent& Cookie) = 0;

   // {secret}
   virtual int    EnableAlarmArea(const TCHAR* strArea, bool = true) = 0;

   // {secret}
   virtual bool   IsAreaEnabled(const TCHAR* strArea) = 0;

   virtual KERESULT GetAlarmEvent(const TCHAR* strDataItem,
                                  Condition    condition,
                                  SubCondition subCondition,
                                  CondEventStruct*& pEvent) = 0;

   virtual void ReleaseAlarmEvent(CondEventStruct *pEvent) = 0;
};

#endif


