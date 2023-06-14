//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  SimulatedDataItem.cpp
//  
//  Subsystem  :  EDD Driver
//
//  Description:  
//                See EDDMgr.cpp for overview
//
//
//  Classes:
//                SimluatedDataItem
//

// For Microsoft IDE the pre-compiled header must be the first line in a CPP file.
#include "pch.h"
#include <math.h>
const double pi = 3.1415926535;


//*****************************************************************************
SimulatedDataItem::SimulatedDataItem(EString& esName, bool bReadOnly)
 : m_tick(0), m_type(SINE),
 EDDDataItem(esName, CDataValue::ANALOG, bReadOnly)
{
    m_type = GetSimulationType(esName);
    if (m_type == _IGNORE)
        m_type = SINE;      // regardless of name, simulate sine type.
    SetValueType();
    InitTick();
}

//*****************************************************************************
SimulatedDataItem::SimulatedDataItem(EString& esName, CDataValue::ValueType valueType,
                                     bool bReadOnly, SIMTYPE simType)
 : m_tick(0), m_type(simType),
 EDDDataItem(esName, valueType, bReadOnly)
{
    SetValueType();
    InitTick();
}

//*****************************************************************************
SimulatedDataItem::~SimulatedDataItem()
{
}


//*****************************************************************************
// This is used to create a Simulated Data Item programatically.
// static
EDDDataItem* SimulatedDataItem::Create(TCHAR* pszName, bool bReadOnly)
{
    EString esName(pszName);
    return new SimulatedDataItem(esName, bReadOnly);
}

//*****************************************************************************
// This is used to create a Simulated Data Item via a configuration file.
// static
EDDDataItem* SimulatedDataItem::Create(EString& esName, CDataValue::ValueType valueType,
                                        bool bReadOnly, SIMTYPE simType)
{
    return new SimulatedDataItem(esName, valueType, bReadOnly, simType);
}

//*****************************************************************************
// static
SimulatedDataItem::SIMTYPE SimulatedDataItem::GetSimulationType (EString& esName)
{
    EString esTempName(esName);
    esTempName.MakeLower();
    if( esTempName.Find( _T("ramp") ) >-1 )
    {
        return RAMP;
    }
    if( esTempName.Find( _T("sine") ) >-1 )
    {
        return SINE;
    }
    if( esTempName.Find( _T("wave") ) >-1 )
    {
        return WAVE;
    }
    if( esTempName.Find( _T("random") ) >-1 )
    {
        return RANDOM;
    }
    if( esTempName.Find( _T("dig5") ) >-1 )
    {
        return DIG5;
    }
    // Array has precedence over dig.
    if( esTempName.Find( _T("array") ) >-1 )
    {
        return ARRAY;
    }
    if( esTempName.Find( _T("dig") ) >-1 )
    {
        return DIG1;
    }
    if( esTempName.Find( _T("toggle") ) >-1 )
    {
        return TOGGLE;
    }

    return _UNDEFINED;
}

//*****************************************************************************
KERESULT SimulatedDataItem::DefineValues(EStringArray& values)
{
    if (m_type == ARRAY && values.GetSize())
    {
        // use m_values to simulate ARRAY of data.
        m_values.Append(values);
        return  KE_OK;
    }

    // Let super class handle base implemenation.
    return EDDDataItem::DefineValues(values);
}

//*****************************************************************************
//              Implementation of SimulatedDataItem
//*****************************************************************************


//*****************************************************************************
void SimulatedDataItem::Acquire()
{
    if (!m_bActive || m_type == _IGNORE)
        return;
    
    ULONG now = GetTickCount();
    double t = now/3130.0;
    t += (double) m_tick;
    
    switch( m_type )
    {
    case RAMP:
        {
            int val = (int) m_value.GetNumeric();
            val = (val+7) % 100;
            SetValue(val);
            break;
        }
    case SINE:
        {
            double val = sin(pi*t/10);
            SetValue(val);
            break;
        }
    case WAVE:
        {
            double val  = (double) ((long)(50*sin(pi*t/10)+50));
            SetValue(val);
            break;
        }
    case RANDOM:
        {
            int val = rand()%100;
            SetValue(val);
            break;
        }
    case TOGGLE:
        {
            Toggle();
            break;
        }
    case DIG1:
        {
            if( now-m_tick >= 1000 )
            {
                m_tick = now;
                Toggle();
            }
            break;
        }
    case DIG5:
        {
            if( now-m_tick >= 5000 )
            {
                m_tick = now;
                Toggle();
            }
            break;
        }
    case ARRAY:
        {
            // Read next predefined array value
            EString esValue = m_values[m_tick];
            m_tick++;
            if (m_tick > (DWORD) m_values.GetUpperBound())
                m_tick = 0;
            switch (GetType())
            {
            case CDataValue::ANALOG:
                {
                    double value = 0.0;
                    if (!esValue.IsEmpty())
                        value = _tcstod(esValue.GetPtr(), NULL);    
                    SetValue(value);
                    break;
                }
            case CDataValue::DIGITAL:
                {
                    int value = 0;
                    if (!esValue.IsEmpty())
                        value = _tcstol(esValue.GetPtr(), NULL, 10);
                    SetValue((value) ? true : false);
                    break;
                }
            case CDataValue::STRING:
                SetValue(esValue);
                break;
            }
        }
    }
    
    // Set Data quality
    m_value.SetQuality(CDataValue::GOOD);
    
}


//*****************************************************************************
//              Support
//*****************************************************************************

//*****************************************************************************
void SimulatedDataItem::SetValueType()
{
    switch (m_type)
    {
    case RAMP:
    case SINE:
    case WAVE:
    case RANDOM:
        m_value.SetType(CDataValue::ANALOG);
        break;
    case DIG5:
    case DIG1:
    case TOGGLE:
        m_value.SetType(CDataValue::DIGITAL);
        break;
    // Default:  leave as defined.
    }
}

//*****************************************************************************
// tick used for various purposes from initial value, to time function, 
// to period of sine wave.
void SimulatedDataItem::InitTick()
{
    EString esTempName(m_esName);
    for( int i=0; i<esTempName.GetLength(); i++ )
        m_tick += (int)esTempName[i];

    if (m_type == RAMP)
        SetValue((int) m_tick);     // init ramp value.

    else if (m_type == ARRAY)
        m_tick = 0;                 // used as array index.
}


//*****************************************************************************
// Toggle digital value
void SimulatedDataItem::Toggle()
{
    ASSERT(m_value.GetType() == CDataValue::DIGITAL);

    double val = 0.0;       // incase dig value not pre-defined.
    if (!m_value.IsDataInvalid())
    {
        val = (m_value.GetNumeric() == 0.0) ? 1.0 : 0.0;
    }
    SetValue(val);
}
