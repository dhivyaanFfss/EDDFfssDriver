//*****************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  SimulatedDataItem.h
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//  Classes    :  
//                SimulatedDataItem
//
//*****************************************************************************

#ifndef __SIMULATED_DATA_ITEM_H__
#define __SIMULATED_DATA_ITEM_H__


//*****************************************************************************
class SimulatedDataItem : public EDDDataItem
{

public:
   enum SIMTYPE {_IGNORE, _UNDEFINED = _IGNORE, RAMP, SINE, WAVE, RANDOM, DIG1,
       DIG5, TOGGLE, ARRAY};

    // Implementation of changed EDDDataItem methods
    virtual KERESULT DefineValues(EStringArray& values);
    virtual void Acquire();

    static EDDDataItem* Create(TCHAR* pszName, bool bReadOnly = false);
    static EDDDataItem* Create(EString& esName, CDataValue::ValueType valueType = CDataValue::ANALOG,
        bool bReadOnly = false, SIMTYPE simType = SINE);
    static SIMTYPE GetSimulationType (EString& esName);

protected:
    // methods
    SimulatedDataItem(EString& name, CDataValue::ValueType valueType,
        bool bReadOnly, SIMTYPE simType);
    SimulatedDataItem(EString& name, bool bReadOnly = false);
    virtual ~SimulatedDataItem();

    // Support
    void SetValueType();
    void InitTick();
    void Toggle();

    // properties
   SIMTYPE          m_type;
   DWORD            m_tick;
   EStringArray     m_values;       


};


#endif // __SIMULATED_DATA_ITEM_H__

