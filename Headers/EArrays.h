//**************************************************************************
//:>
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//:>
//**************************************************************************
//:>
//:> Filename   :  EArrays.h
//:>  
//:> Subsystem  :  Kernel/Base
//:>
//:> Description:  Array collections
//:>   Arrays are collections whose elements are directly indexed.
//:>   Access to an element is fast.
//:>
//:> Classes: 
//:>      EByteArray
//:>      EDWordArray
//:>      EStringArray
//:>      EPtrArray
//:>      ETypedPtrArray
//:>      EXdrMessage
//:>	  ECircularBuffer
//:>
//**************************************************************************

#ifndef __EARRAYS_H__
#define __EARRAYS_H__

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


#ifndef __ECOLLECTIONS_H__
#include "ECollections.h"
#endif

#include "EOS.h"

const short ETMP_MAX_SIZE = 50;         // {secret}
class ETmpString;

// group=Arrays

#ifndef _OPC
//**************************************************************************
class _DV_DECLSPEC EByteArray
{
public:

// Construction
    EByteArray();
    EByteArray(const BYTE *pData, int nSize);

// Attributes
    int GetSize() const;
    int GetUpperBound() const;
    void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
    // Clean up
    void FreeExtra();
    void RemoveAll();

    // Accesses elements
    BYTE GetAt(int nIndex) const;
    void SetAt(int nIndex, BYTE newElement);

    BYTE& ElementAt(int nIndex);

    // Directs Access to the element data (may return NULL)
    const BYTE* GetData() const;
    BYTE* GetData();

    // Potentially grows the array
    void SetAtGrow(int nIndex, BYTE newElement);

    int Add(BYTE newElement);

    int Append(const EByteArray& src);
    void Copy(const EByteArray& src);

    // Overloaded operator helpers
    BYTE operator[](int nIndex) const;
    BYTE& operator[](int nIndex);

    // Operations that move elements around
    void InsertAt(int nIndex, BYTE newElement, int nCount = 1);

    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, EByteArray* pNewArray);

    int Append(const BYTE *pData, int nSize);

// Implementation
protected:
    BYTE* m_pData;   // The actual array of data
    int m_nSize;     // The number of elements (upperBound - 1)
    int m_nMaxSize;  // The maximum allocated
    int m_nGrowBy;   // The amount to grow by


public:
    ~EByteArray();

protected:
    // Local typedefs for class templates
    typedef BYTE BASE_TYPE;
    typedef BYTE BASE_ARG_TYPE;
};

//**************************************************************************
inline int EByteArray::GetSize() const
    { return m_nSize; }
inline int EByteArray::GetUpperBound() const
    { return m_nSize-1; }
inline void EByteArray::RemoveAll()
    { SetSize(0); }
inline BYTE EByteArray::GetAt(int nIndex) const
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline void EByteArray::SetAt(int nIndex, BYTE newElement)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        m_pData[nIndex] = newElement; }

inline BYTE& EByteArray::ElementAt(int nIndex)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline const BYTE* EByteArray::GetData() const
    { return (const BYTE*)m_pData; }
inline BYTE* EByteArray::GetData()
    { return (BYTE*)m_pData; }
inline int EByteArray::Add(BYTE newElement)
    { int nIndex = m_nSize;
        SetAtGrow(nIndex, newElement);
        return nIndex; }

inline BYTE EByteArray::operator[](int nIndex) const
    { return GetAt(nIndex); }
inline BYTE& EByteArray::operator[](int nIndex)
    { return ElementAt(nIndex); }
#endif //_OPC
//**************************************************************************
// EDWordArray
class _DV_DECLSPEC EDWordArray 
{

public:

// Construction
    EDWordArray();

// Attributes
    int GetSize() const;
    int GetUpperBound() const;
    void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
    // Clean up
    void FreeExtra();
    void RemoveAll();

    // Accessing elements
    DWORD GetAt(int nIndex) const;
    void SetAt(int nIndex, DWORD newElement);

    DWORD& ElementAt(int nIndex);

    // Direct Access to the element data (may return NULL)
    const DWORD* GetData() const;
    DWORD* GetData();

    // Potentially growing the array
    void SetAtGrow(int nIndex, DWORD newElement);

    int Add(DWORD newElement);

    int Append(const EDWordArray& src);
    void Copy(const EDWordArray& src);

    // Overloaded operator helpers
    DWORD operator[](int nIndex) const;
    DWORD& operator[](int nIndex);

    // Operations that move elements around
    void InsertAt(int nIndex, DWORD newElement, int nCount = 1);

    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, EDWordArray* pNewArray);

// Implementation
protected:
    DWORD* m_pData;   // The actual array of data
    int m_nSize;     // The number of elements (upperBound - 1)
    int m_nMaxSize;  // The maximum allocated
    int m_nGrowBy;   // The amount to grow by


public:
    ~EDWordArray();

protected:
    // Local typedefs for class templates
    typedef DWORD BASE_TYPE;
    typedef DWORD BASE_ARG_TYPE;
};


//**************************************************************************
// Inlines for EDWordArray

inline int EDWordArray::GetSize() const
    { return m_nSize; }
inline int EDWordArray::GetUpperBound() const
    { return m_nSize-1; }
inline void EDWordArray::RemoveAll()
    { SetSize(0); }
inline DWORD EDWordArray::GetAt(int nIndex) const
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline void EDWordArray::SetAt(int nIndex, DWORD newElement)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        m_pData[nIndex] = newElement; }

inline DWORD& EDWordArray::ElementAt(int nIndex)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline const DWORD* EDWordArray::GetData() const
    { return (const DWORD*)m_pData; }
inline DWORD* EDWordArray::GetData()
    { return (DWORD*)m_pData; }
inline int EDWordArray::Add(DWORD newElement)
    { int nIndex = m_nSize;
        SetAtGrow(nIndex, newElement);
        return nIndex; }

inline DWORD EDWordArray::operator[](int nIndex) const
    { return GetAt(nIndex); }
inline DWORD& EDWordArray::operator[](int nIndex)
    { return ElementAt(nIndex); }



//**************************************************************************
// EStringArray

class _DV_DECLSPEC EStringArray
{

public:

// Construction
    EStringArray();

// Attributes
    int GetSize() const;
    int GetUpperBound() const;
    void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
    // Clean up
    void FreeExtra();
    void RemoveAll();

    // Accessing elements
    EString GetAt(int nIndex) const;
    void SetAt(int nIndex, LPCTSTR newElement);

    void SetAt(int nIndex, const EString& newElement);

    EString& ElementAt(int nIndex);

    // Direct Access to the element data (may return NULL)
    const EString* GetData() const;
    EString* GetData();

    // Potentially growing the array
    void SetAtGrow(int nIndex, LPCTSTR newElement);

    void SetAtGrow(int nIndex, const EString& newElement);

    int Add(LPCTSTR newElement);

    int Add(const EString& newElement);

	int Find(const EString& strTemplate);

    int Append(const EStringArray& src);
    void Copy(const EStringArray& src);

    // Overloaded operator helpers
    EString operator[](int nIndex) const;
    EString& operator[](int nIndex);

    // Operations that move elements around
    void InsertAt(int nIndex, LPCTSTR newElement, int nCount = 1);

    void InsertAt(int nIndex, const EString& newElement, int nCount = 1);

    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, EStringArray* pNewArray);

// Implementation
protected:
    EString* m_pData;   // The actual array of data
    int m_nSize;     // The number of elements (upperBound - 1)
    int m_nMaxSize;  // The maximum allocated
    int m_nGrowBy;   // The amount to grow by

    void InsertEmpty(int nIndex, int nCount);


public:
    ~EStringArray();


protected:
    // Local typedefs for class templates
    typedef EString BASE_TYPE;
    typedef LPCTSTR BASE_ARG_TYPE;
};

//**************************************************************************
// Inlines for EStringArray

inline int EStringArray::GetSize() const
    { return m_nSize; }
inline int EStringArray::GetUpperBound() const
    { return m_nSize-1; }
inline void EStringArray::RemoveAll()
    { SetSize(0); }
inline EString EStringArray::GetAt(int nIndex) const
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline void EStringArray::SetAt(int nIndex, LPCTSTR newElement)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        m_pData[nIndex] = newElement; }

inline void EStringArray::SetAt(int nIndex, const EString& newElement)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        m_pData[nIndex] = newElement; }

inline EString& EStringArray::ElementAt(int nIndex)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline const EString* EStringArray::GetData() const
    { return (const EString*)m_pData; }
inline EString* EStringArray::GetData()
    { return (EString*)m_pData; }
inline int EStringArray::Add(LPCTSTR newElement)
    { int nIndex = m_nSize;
        SetAtGrow(nIndex, newElement);
        return nIndex; }

inline int EStringArray::Add(const EString& newElement)
    { int nIndex = m_nSize;
        SetAtGrow(nIndex, newElement);
        return nIndex; }

inline EString EStringArray::operator[](int nIndex) const
    { return GetAt(nIndex); }
inline EString& EStringArray::operator[](int nIndex)
    { return ElementAt(nIndex); }


//**************************************************************************
// EPtrArray
class _DV_DECLSPEC EPtrArray 
{

public:

// Construction
    EPtrArray();

// Attributes
    int GetSize() const;
    int GetUpperBound() const;
    void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
    // Clean up
    void FreeExtra();
    void RemoveAll();

    // Accessing elements
    void* GetAt(int nIndex) const;
    void SetAt(int nIndex, void* newElement);

    void*& ElementAt(int nIndex);

    // Direct Access to the element data (may return NULL)
    const void** GetData() const;
    void** GetData();

    // Potentially growing the array
    void SetAtGrow(int nIndex, void* newElement);

    int Add(void* newElement);

    int Append(const EPtrArray& src);
    void Copy(const EPtrArray& src);

    // Overloaded operator helpers
    void* operator[](int nIndex) const;
    void*& operator[](int nIndex);

    // Operations that move elements around
    void InsertAt(int nIndex, void* newElement, int nCount = 1);

    void RemoveAt(int nIndex, int nCount = 1);
    void InsertAt(int nStartIndex, EPtrArray* pNewArray);

// Implementation
protected:
    void** m_pData;   // The actual array of data
    int m_nSize;     // The number of elements (upperBound - 1)
    int m_nMaxSize;  // The maximum allocated size
    int m_nGrowBy;   // The amount to grow by


public:
    ~EPtrArray();

protected:
    // Local typedefs for class templates
    typedef void* BASE_TYPE;
    typedef void* BASE_ARG_TYPE;
};


//**************************************************************************
// Inlines for EPtrArray

inline int EPtrArray::GetSize() const
    { return m_nSize; }
inline int EPtrArray::GetUpperBound() const
    { return m_nSize-1; }
inline void EPtrArray::RemoveAll()
    { SetSize(0); }
inline void* EPtrArray::GetAt(int nIndex) const
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline void EPtrArray::SetAt(int nIndex, void* newElement)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        m_pData[nIndex] = newElement; }

inline void*& EPtrArray::ElementAt(int nIndex)
    { ASSERT(nIndex >= 0 && nIndex < m_nSize);
        return m_pData[nIndex]; }
inline const void** EPtrArray::GetData() const
    { return (const void**)m_pData; }
inline void** EPtrArray::GetData()
    { return (void**)m_pData; }
inline int EPtrArray::Add(void* newElement)
    { int nIndex = m_nSize;
        SetAtGrow(nIndex, newElement);
        return nIndex; }

inline void* EPtrArray::operator[](int nIndex) const
    { return GetAt(nIndex); }
inline void*& EPtrArray::operator[](int nIndex)
    { return ElementAt(nIndex); }



/////////////////////////////////////////////////////////////////////////////
// ETypedPtrArray<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class ETypedPtrArray : public BASE_CLASS
{
public:
    // Accessing elements
    TYPE GetAt(int nIndex) const
        { return (TYPE)BASE_CLASS::GetAt(nIndex); }
    TYPE& ElementAt(int nIndex)
        { return (TYPE&)BASE_CLASS::ElementAt(nIndex); }
    void SetAt(int nIndex, TYPE ptr)
        { BASE_CLASS::SetAt(nIndex, ptr); }

    // Potentially growing the array
    void SetAtGrow(int nIndex, TYPE newElement)
       { BASE_CLASS::SetAtGrow(nIndex, newElement); }
    int Add(TYPE newElement)
       { return BASE_CLASS::Add(newElement); }
    int Append(const ETypedPtrArray<BASE_CLASS, TYPE>& src)
       { return BASE_CLASS::Append(src); }
    void Copy(const ETypedPtrArray<BASE_CLASS, TYPE>& src)
        { BASE_CLASS::Copy(src); }

    // Operations that move elements around
    void InsertAt(int nIndex, TYPE newElement, int nCount = 1)
        { BASE_CLASS::InsertAt(nIndex, newElement, nCount); }
    void InsertAt(int nStartIndex, ETypedPtrArray<BASE_CLASS, TYPE>* pNewArray)
       { BASE_CLASS::InsertAt(nStartIndex, pNewArray); }

    // Overloaded operator helpers
    TYPE operator[](int nIndex) const
        { return (TYPE)BASE_CLASS::operator[](nIndex); }
    TYPE& operator[](int nIndex)
        { return (TYPE&)BASE_CLASS::operator[](nIndex); }
};


// See Also: EXdrMessage::SetSixtyFourBit, EXdrMessage::GetSixtyFourBit 
struct SIXTYFOURBIT
{
    ELONG32 one;
    ELONG32 two;
};

class _DV_DECLSPEC ECircularBuffer
{
public:
	
	ECircularBuffer(ELONG32 size, void* pBuffer = NULL);
	~ECircularBuffer();

	void AddToBuff(void* addBuf, ELONG32 cbBytesWrite);
	void AddToHead(void* addBuf, ELONG32 cbBytesWrite);
	ELONG32 ReadFromBuff(void *addBuf, ELONG32 size);
	ELONG32 BuffLen();
	void LockBuffer();
	void UnLockBuffer();

protected:
	void StoreInfo();
	void GetInfo();

	ELONG32		m_SircleBufferSize;
	EMutex		m_hRefMtx;
	ELock*		m_pLock;
	unsigned char *m_pBuff;
	ELONG32		m_Tail;
	ELONG32		m_Head;
	ELONG32		m_lLength;
	bool		m_bMyBuffer;
};


#ifndef _OPC
/**************************************************************************
XDR (eXternal Data Representation RFC 1832) defines the format for storing
basic data types, including their size and byte ordering.

This class manages a message buffer containing XDR data.
The size of the message grows as data is put into it.

The typical usage is to create an EXdrMessage and put data into it.
The message is then sent or written to a file, socket, pipe, and so on.

On the receiving side, an EXdrMessage's size must be set sufficiently
large to hold a message. Then a message is read into the object.

Data is extracted using the Getxxx() functions in the same order
as the data was originally put into the message.

(Strings are not stored as ASCII strings in XDR, but as Unicode.
The caller must convert to Unicode as needed.)
*/
class _DV_DECLSPEC EXdrMessage : private EByteArray
{
public:
    EXdrMessage();

    // Erase the contents of the message, and reset the position to the beginning.
    void Clear() {memset(EByteArray::GetData(), 0, EByteArray::GetSize()); ResetCurrentPos();}
    // Set the allocation size of the message. If the final size is known,
    // this is more efficient than growing the message repeatedly.
    void SetSize(int nNewSize) {EByteArray::SetSize(nNewSize);}
    // Returns a pointer to the message data. This should only be used
    // to write or send the message, such as a socket send().
    char* GetData() const {return (char*)EByteArray::GetData();}
    // Reset the position to the beginning of the message.
    // Use this when reading a message into an EXdrMessage.
    void ResetCurrentPos() {m_position=0;}
    // Returns the current position, which is also the current message
    // size in bytes. This can be remembered to write data
    // back to a particular position in the message.
    int GetCurrentPos() {return m_position;}

    int GetSize()              { return EByteArray::GetSize(); }

    void SetPosition(int iPos) { m_position = iPos; }

    int GetVariant(VARIANT& v);
    int PutVariant(VARIANT&);
    int PutVariantAt(VARIANT&,int iPos);

    bool GetBool();
    int PutBool(bool data);
    int PutBoolAt(bool data, int position);

    int GetInt();
    int PutInt(int data);
    int PutIntAt(int data, int position);

    ELONG32 GetLong();
    int PutLong(ELONG32 data);
    int PutLongAt(ELONG32 data, int position);
    int InsertLongAt(ELONG32 data, int position);

    EHANDLE GetHandle();
    int PutHandle(EHANDLE hData);

    float GetFloat();
    int PutFloat(float data);
    int PutFloatAt(float data, int position);

    double GetDouble();
    int PutDouble(double data);
    int PutDoubleAt(double data, int position);

    // GetString() allocates the string, and the caller must free it!
    WCHAR* GetString();
    int PutString(const WCHAR* string);
    int PutStringAt(const WCHAR* string, int position);

    // Use ETmpString for storage.  No need to free data.
    void GetString(ETmpString *pTmpString);

    EXdrMessage& operator+(const EXdrMessage& msg);
    EXdrMessage& operator+=(const EXdrMessage& msg);
    EXdrMessage& operator=(const EXdrMessage& msg);

    int AppendFromPosition(EXdrMessage& msg,int iFrom,int iSize);    
    
    int InsertData(int pos,EXdrMessage& msg,int iSize, int iEndOfData);

    int RemoveData(int iStartPos,int iEndPos, int& iEndOfData);

    //  64-bit numbers (for example, FILETIME)
    SIXTYFOURBIT GetSixtyFourBit();
    int PutSixtyFourBit(SIXTYFOURBIT data);
    int PutSixtyFourBitAt(SIXTYFOURBIT data, int position);

private:
    int m_position; // Current position for Get/Put operations
};
#endif // _OPC

// group=General
class _DV_DECLSPEC ETmpString
{
// Remarks:  Useful when a temporary string is needed that generally avoids 
//           allocation of memory.  Used with EXdrMessage::GetString.
private:
    WCHAR   m_buf[ETMP_MAX_SIZE];
    WCHAR*  m_pWString;   
    TCHAR*  m_pTString;
    DWORD   m_size;
 
public:
    ETmpString(LPCWSTR pStr);
    ETmpString();
    ~ETmpString(); 

    TCHAR* GetBuffer();
    void SetString(LPCWSTR pStr);
    void SetTString();
    WCHAR* SetSize(DWORD size);
    DWORD GetSize() { return m_size; }

};

#endif
