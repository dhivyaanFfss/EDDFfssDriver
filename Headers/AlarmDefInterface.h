//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  AlarmDefInterface.h
//  
//  Subsystem  :  Kernel - Alarm Component
//
//  Description:  define alarm definition interfaces
//
//  Classes:
//              IAlarmDef
//
//**************************************************************************


#ifndef __ALARMDEF_INTERFACE_H__
#define __ALARMDEF_INTERFACE_H__

/* Group=Alarm definition */

////////////////////////////////////////////////////////////////////////
// Alarm definition : interface definition
interface IAlarmDef
{
public:
   // Get ... methods
   virtual EString    GetArea             () const = 0;
   virtual EString    GetDataItem         () const = 0;

   virtual EString    GetDescription      () const = 0;

   virtual Condition    
                      GetCondition        () const = 0;
   virtual SubCondition 
                      GetSubCondition     () const = 0;

   virtual int        GetSeverity         () const = 0;
   virtual long       GetCategory         () const = 0;

   virtual double     GetValue            () const = 0; 
   virtual double     GetValue2           () const = 0; 
   virtual double     GetDeadband         () const = 0; 

   virtual int        GetLatency          () const = 0;

   // Set ... methods
   virtual int        SetArea             (const TCHAR*) = 0;
   virtual int        SetDataItem         (const TCHAR*) = 0;
   virtual int        SetDescription      (const TCHAR*) = 0;

   virtual int        SetCondition        (Condition) = 0;
   virtual int        SetSubCondition     (SubCondition) = 0;

   virtual int        SetSeverity         (int) = 0;
   virtual int        SetCategory         (long) = 0;

   virtual int        SetValue            (double) = 0; 
   virtual int        SetValue2           (double) = 0; 
   virtual int        SetDeadband         (double) = 0; 

   virtual int        SetLatency          (int) = 0;

   virtual void       Release             () = 0;
};

#endif
