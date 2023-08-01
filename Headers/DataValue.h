//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  DataValue.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Data Value
//
//  Classes: CDataValue
//           EDataValuePtrs
//
//**************************************************************************

#ifndef __DATAVALUE_H__
#define __DATAVALUE_H__

#ifndef __EARRAYS_H__
#include "EArrays.h"
#endif

// group=General
#ifdef WIN32
    #ifdef _DV_EXPORT
// {secret}
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

class EXdrMessage;

typedef ETypedPtrArray<EPtrArray, VARIANT*> EVariantArray;

#ifdef WIN32
template class _DV_DECLSPEC ETypedPtrArray<EPtrArray, VARIANT*>;
#endif

////////////////////////////////////////////////////////////////////////
// Data Value class definition

class _DV_DECLSPEC CDataValue//: public IPersistent
{
public:
    enum ValueType
    {
        ANALOG = 1, DEFAULT = ANALOG, DIGITAL, STRING
    };
    
    enum Quality    // These are the same as OPC's
    {
        GOOD=0xC0,
        BAD=0,
        UNCERTAIN=0x40,
        INVALID = 0x20   // internal Data Ctrl flag. This data pt will never be published.
    };
    
public:
    CDataValue(ValueType = DEFAULT, bool bArray = false);
    CDataValue(const CDataValue&);
    ~CDataValue();          // make virtual if subclassed.
    
    CDataValue&    operator=(const CDataValue&);
    bool           operator==(const CDataValue&) const;
    bool           operator!=(const CDataValue&) const;
    
    inline ValueType GetType() const { return m_enType; }
    void           SetType(ValueType);
	bool 		   ChangeType(ValueType dstType);

    bool           IsNumeric() const;
    bool           IsString() const;
    bool           IsInRange(double, double, int iIndex = 0) const;
    bool           IsDataInvalid(int iIndex = 0);         // check if Variant is empty
    
    double         GetNumeric(int iIndex = 0);
    int            SetNumeric(double, int iIndex = 0);
    bool           Set(const VARIANT& val, int iIndex = 0);
    VARIANT*       Get(int iIndex = 0) const;
    
    KERESULT       GetString(EString& rv, int nPrecision = 0, int iIndex = 0, 
        int maxTrailingZeros = 0);
    WCHAR*         GetStringValue(int iIndex = 0);
    int            SetString(const TCHAR*, int iIndex = 0);
#ifndef UNICODE
    int            SetString(const WCHAR*, int iIndex = 0);
#endif    
    unsigned short GetQuality() const;
    void           SetQuality(unsigned short);
    
    ETimeStamp       GetTimestamp() const;
    void           SetTimestamp(const ETimeStamp&);
    
    bool           SerializeToXdr(EXdrMessage& msg);
    bool           SerializeFmXdr(EXdrMessage& msg);

    bool            Save(EFileIO* pDataFile);
    bool            Restore(EFileIO* pDataFile);

    int             GetArraySize(void) const;
    void            SetArraySize(int iSize);
    void            RemoveAt(int iIndex);
    void            CopyAt(CDataValue& Val, int iIndex);
    void            SetArray(bool bArray);
    bool            IsArray(void) const;

protected:
    void            InitArrayElement(int iIndex);
    void            RemoveTrailingZeros(EString& resValue, int maxTrailingZeros);

protected:
    ValueType      m_enType;
    Quality        m_enQuality;
    VARIANT        m_scalarValue;
    EVariantArray  m_arrayValue;
    ETimeStamp       m_nTimestamp;
    bool           m_bArray;
};

//**************************************************************************
// EDataValuePtrs
//**************************************************************************


#ifdef WIN32
template class _DV_DECLSPEC ETypedPtrArray<EPtrArray, CDataValue*>;
#endif // WIN32

typedef ETypedPtrArray<EPtrArray, CDataValue*> CDataValuePtrArray;

class _DV_DECLSPEC EDataValuePtrs : public ETypedPtrArray<EPtrArray, CDataValue*>
{
public:
    EDataValuePtrs();
    ~EDataValuePtrs();

    void DeleteAt( int nIndex, int nCount = 1 );
    void DeleteAll();    
};

#endif // __DATAVALUE_H__
