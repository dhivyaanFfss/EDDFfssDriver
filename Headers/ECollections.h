//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ECollections.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Collection classes used in the kernel
//
//  Classes: 
//           EString
//           EPtrList
//           ETypedPtrList
//           ETypedQueue
//           ETypedStack
//           EMapPtrToPtr
//           EMapStringToPtr
//           EMapStringToString
//           ETypedPtrMap
//           MStringList        // memory managed string list
//
//**************************************************************************

#ifndef __ECOLLECTIONS_H__
#define __ECOLLECTIONS_H__

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

#define DEFAULT_BLOCK_SIZE 4

// group=General

//**************************************************************************
//class EString;

struct EStringData
{
    ELONG32 nRefs;          // Reference count
    int nDataLength;        // Length of data (including the terminator)
    int nAllocLength;       // Length of allocation

    TCHAR* data()           // TCHAR* to managed data
        { return (TCHAR*)(this+1); }
};


class EFileIO;
class _DV_DECLSPEC EString
{
public:
// Constructors

    // Constructs an empty EString.
    EString();
    // Copies the constructor.
    EString(const EString& stringSrc);
    // From a single character
//  EString(TCHAR ch, int nRepeat = 1);
    // From an ANSI string (converts to TCHAR).
    EString(LPCSTR lpsz);
    // From a Unicode string (converts to TCHAR).
    EString(LPCWSTR lpsz);
    // Subset of characters from an ANSI string (converts to TCHAR).
//  EString(LPCSTR lpch, int nLength);
    // Subset of characters from a Unicode string (converts to TCHAR).
//  EString(LPCWSTR lpch, int nLength);
    // From unsigned characters.
    EString(const unsigned char* psz);

// Attributes & Operations

    // Gets data length.
    int GetLength() const;
    // TRUE, if zero length.
    BOOL IsEmpty() const;
    // Clears contents to empty.
    void Empty();

    // Returns a single character at zero-based index.
    TCHAR GetAt(int nIndex) const;
    // Returns a single character at zero-based index.
    TCHAR operator[](int nIndex) const;
    // Sets a single character at zero-based index.
    void SetAt(int nIndex, TCHAR ch);
    // Returns a pointer to const string.
    operator LPCTSTR() const;

    // Returns a pointer to string.
    // Added by JEK.  This is actually a const string that 
    // MUST not be modified.
    // This is 1. faster then GetBuffer(0)
    //         2. Doesn't do a string copy for multiple references.
    TCHAR* GetPtr() const;

    // Overloaded assignment

    // Ref-counted copy from another EString.
    const EString& operator=(const EString& stringSrc);
    // Sets the string content to a single character.
//  const EString& operator=(TCHAR ch);
#ifdef UNICODE
    const EString& operator=(char ch);
#endif
    // Copies the string content from ANSI string (converts to TCHAR).
    const EString& operator=(LPCTSTR lpsz);
    // Copies the string content from Unicode string (converts to TCHAR).
//    const EString& operator=(LPCWSTR lpsz);

    // Copies the string content from unsigned chars
    const EString& operator=(const unsigned char* psz);

    // String concatenation

    // Concatenates from another EString.
    const EString& operator+=(const EString& string);
    // Concatenates a single character.
    const EString& operator+=(TCHAR ch);
#ifdef UNICODE
    // Concatenates an ANSI character after converting it to TCHAR.
    const EString& operator+=(char ch);
#endif
    // Concatenates a Unicode character after converting it to TCHAR.
    const EString& operator+=(LPCTSTR lpsz);

// group=String Operators
    friend _DV_DECLSPEC EString  operator+(const EString& string1,
            const EString& string2);
    friend _DV_DECLSPEC EString  operator+(const EString& string, TCHAR ch);
    friend _DV_DECLSPEC EString  operator+(TCHAR ch, const EString& string);
#ifdef UNICODE
    friend _DV_DECLSPEC EString  operator+(const EString& string, char ch);
    friend _DV_DECLSPEC EString  operator+(char ch, const EString& string);
#endif
    friend _DV_DECLSPEC EString  operator+(const EString& string, LPCTSTR lpsz);
    friend _DV_DECLSPEC EString  operator+(LPCTSTR lpsz, const EString& string);

//group=General
    // String comparison

    // Performs a straight character comparison.
    int Compare(LPCTSTR lpsz) const;
    // Compares strings, ignoring case.
    int CompareNoCase(LPCTSTR lpsz) const;
    // NLS-aware comparison, case-sensitive.
/*MINSTRING
    int Collate(LPCTSTR lpsz) const;
    // NLS aware comparison, case insensitive
    int CollateNoCase(LPCTSTR lpsz) const;
*/
    // Simple sub-string extraction

    // Returns nCount characters starting at zero-based nFirst.
    EString Mid(int nFirst, int nCount) const;
    // Returns all characters starting at zero-based nFirst.
    EString Mid(int nFirst) const;
    // Returns first nCount characters in string.
    EString Left(int nCount) const;
    // Returns nCount characters from end of string.
    EString Right(int nCount) const;
/*
    //  Characters from beginning that are also in the passed string
    EString SpanIncluding(LPCTSTR lpszCharSet) const;
    // Characters from beginning that are not also in the passed string
    EString SpanExcluding(LPCTSTR lpszCharSet) const;
*/
    // Upper/lower/reverse conversion

    // Performs NLS-aware conversion to uppercase.
    void MakeUpper();
    // Performs NLS-aware conversion to lowercase
    void MakeLower();
    // reverse string right-to-left
//    void MakeReverse();

    // Trimming whitespace (either side)

    // Removes whitespace starting from the right edge.
    void TrimRight();
    // Removes whitespace starting from the left side.
    void TrimLeft();

    // Trims anything (either side)

    // Removes continuous occurrences of chTarget starting from the right.
    void TrimRight(TCHAR chTarget);
    // Removes continuous occurrences of characters in passed string,
    // starting from the right.
    void TrimRight(LPCTSTR lpszTargets);
    // Removes continuous occurrences of chTarget starting from the left.
    void TrimLeft(TCHAR chTarget);
    // Removes continuous occcurrences of characters in passed string,
    // starting from the left.
    void TrimLeft(LPCTSTR lpszTargets);

    // Advanced manipulation

    // Inserts substring at zero-based index; concatenates
    // If index is past end of string
    int Insert(int nIndex, LPCTSTR pstr);
    // Replaces occurrences of substring lpszOld with lpszNew;
    // Empty lpszNew removes instances of lpszOld
    int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
    // Deletes nCount characters starting at zero-based index
    int Delete(int nIndex, int nCount = 1);
/*
    // Replaces occurrences of chOld with chNew
    int Replace(TCHAR chOld, TCHAR chNew);
    // Removes occurrences of chRemove
    int Remove(TCHAR chRemove);
    // Inserts character at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, TCHAR ch);

  // Searching...
*/

    // Finds a character starting at the left, returns -1 if not found.
    int Find(TCHAR ch) const;
    // Finds a character starting at zero-based index and moving right.
    int Find(TCHAR ch, int nStart) const;
    // Finds a character starting at the right.
    int ReverseFind(TCHAR ch) const;
    // Finds a first instance of any character in a passed string.
    int FindOneOf(LPCTSTR lpszCharSet) const;
    // Finds a first instance of a substring.
    int Find(LPCTSTR lpszSub) const;
    // Finds a first instance of a substring starting at zero-based index.
    int Find(LPCTSTR lpszSub, int nStart) const;

    // Simple formatting

    // Printf-like formatting using a passed string.
    void  Format(LPCTSTR lpszFormat, ...);

    // Access to string implementation buffer as "C" character array

    // Gets a pointer to modifiable buffer at least as long as nMinBufLength.
    LPTSTR GetBuffer(int nMinBufLength);
    // Releases a buffer, setting length to nNewLength (or to first nul, if -1).
    void ReleaseBuffer(int nNewLength = -1);
    // Gets a pointer to a modifiable buffer exactly as long as nNewLength.
    LPTSTR GetBufferSetLength(int nNewLength);
    // Releases memory allocated to, but unused by, string.
    void FreeExtra();

    // Use LockBuffer/UnlockBuffer to turn refcounting off

    // Turns refcounting back on.
    LPTSTR LockBuffer();
    // Turns off refcounting.
    void UnlockBuffer();

    // Serialize members
    bool Save(EFileIO*);
    bool Save(char*, DWORD& size);
    bool Restore(EFileIO*);
    bool Restore(char*, DWORD& size);

// Implementation
public:
    ~EString();
    int GetAllocLength() const;

    enum Encoding
    {
        ENCODING_UTF8
    };

    EString(LPCSTR lpsz, Encoding encoding);
    EString(const unsigned char* psz, Encoding encoding);
    EString(const unsigned char* psz, int nSrcLen, Encoding encoding);

protected:
    LPTSTR m_pchData;   // Pointer to ref counted string data

    // Implementation helpers
    EStringData* GetData() const;
    void Init();
    void AllocCopy(EString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
    void AllocBuffer(int nLen);
    void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
    void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
    void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
    void CopyBeforeWrite();
    void AllocBeforeWrite(int nLen);
    void Release();
    void Decode(LPCSTR lpsz, Encoding encoding);
    void Decode(LPCSTR lpsz, int nSrcLen, Encoding encoding);
    static void Release(EStringData* pData);
    static int SafeStrlen(LPCTSTR lpsz);
    static void FreeData(EStringData* pData);
};

// Compare helpers
bool  operator==(const EString& s1, const EString& s2);
bool  operator==(const EString& s1, LPCTSTR s2);
bool  operator==(LPCTSTR s1, const EString& s2);
bool  operator!=(const EString& s1, const EString& s2);
bool  operator!=(const EString& s1, LPCTSTR s2);
bool  operator!=(LPCTSTR s1, const EString& s2);
bool  operator<(const EString& s1, const EString& s2);
bool  operator<(const EString& s1, LPCTSTR s2);
bool  operator<(LPCTSTR s1, const EString& s2);
bool  operator>(const EString& s1, const EString& s2);
bool  operator>(const EString& s1, LPCTSTR s2);
bool  operator>(LPCTSTR s1, const EString& s2);
bool  operator<=(const EString& s1, const EString& s2);
bool  operator<=(const EString& s1, LPCTSTR s2);
bool  operator<=(LPCTSTR s1, const EString& s2);
bool  operator>=(const EString& s1, const EString& s2);
bool  operator>=(const EString& s1, LPCTSTR s2);
bool  operator>=(LPCTSTR s1, const EString& s2);

// group=Collections

//**************************************************************************
// {secret}
struct EPlex     // Warning variable length structure
{
    EPlex* pNext;
//#if (_AFX_PACKING >= 8)
//  DWORD dwReserved[1];    // Aligns on 8-byte boundary
//#endif
    // BYTE data[maxNum*elementSize];

    void* data() { return this+1; }

#ifdef UNDER_VXWORKS
    static EPlex* Create(EPlex*& head, UINT nMax, UINT cbElement);
#else // UNDER_VXWORKS
    static EPlex* PASCAL Create(EPlex*& head, UINT nMax, UINT cbElement);
#endif // UNDER_VXWORKS
            // Like 'calloc' but no zero fill
            // may throw memory exceptions

    void FreeDataChain();       // Frees this one and links
};

//**************************************************************************
// Abstract iteration position
// {secret}
#ifndef __AFX_H__
struct __POSITION { };
// {secret}
typedef __POSITION* POSITION;     
#endif

// group=Lists
//**************************************************************************
class _DV_DECLSPEC EPtrList
{
protected:
    struct ENode
    {
        ENode* pNext;
        ENode* pPrev;
        void* data;
    };
public:

// Construction
    EPtrList(int nBlockSize = DEFAULT_BLOCK_SIZE);

// Attributes (head and tail)
    // Gets a count of the elements.
    int GetCount() const;
    BOOL IsEmpty() const;

    // Peeks at the head or tail.
    void*& GetHead();
    void* GetHead() const;
    void*& GetTail();
    void* GetTail() const;

// Operations
    // Gets the head or tail (and removes it) - don't call on empty list!
    void* RemoveHead();
    void* RemoveTail();

    // Adds before the head or after the tail.
    POSITION AddHead(void* newElement);
    POSITION AddTail(void* newElement);


    // Adds another list of elements before the head or after the tail.
    void AddHead(EPtrList* pNewList);
    void AddTail(EPtrList* pNewList);

    // Removes all elements.
    void RemoveAll();

    // Iteration
    POSITION GetHeadPosition() const;
    POSITION GetTailPosition() const;
    void*& GetNext(POSITION& rPosition); // Returns *Position++
    void* GetNext(POSITION& rPosition) const; // Returns *Position++
    void*& GetPrev(POSITION& rPosition); // Returns *Position--
    void* GetPrev(POSITION& rPosition) const; // Returns *Position--

    // Gets/modifies an element at a given position.
    void*& GetAt(POSITION position);
    void* GetAt(POSITION position) const;
    void SetAt(POSITION pos, void* newElement);

    void RemoveAt(POSITION position);

    // Inserts before or after a given position.
    POSITION InsertBefore(POSITION position, void* newElement);
    POSITION InsertAfter(POSITION position, void* newElement);


    // Helper functions (Note: O(n) speed)
                        // Defaults to starting at the HEAD.
                        // Return NULL if not found.
    POSITION Find(void* searchValue, POSITION startAfter = NULL) const;
                        // Gets the nth (nIndex) element (may return NULL).
    POSITION FindIndex(int nIndex) const;

// Implementation
protected:
    ENode* m_pNodeHead;
    ENode* m_pNodeTail;
    int m_nCount;
    ENode* m_pNodeFree;
    struct EPlex* m_pBlocks;
    int m_nBlockSize;

    ENode* NewNode(ENode*, ENode*);
    void FreeNode(ENode*);

public:
    ~EPtrList();
};

//**************************************************************************
// ETypedPtrList<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class _ETypedPtrList : public BASE_CLASS
{
public:
// Construction
    _ETypedPtrList(int nBlockSize = DEFAULT_BLOCK_SIZE)
        : BASE_CLASS(nBlockSize) { }

    // Peeks at the head or tail.
    TYPE& GetHead()
        { return (TYPE&)BASE_CLASS::GetHead(); }
    TYPE GetHead() const
        { return (TYPE)BASE_CLASS::GetHead(); }
    TYPE& GetTail()
        { return (TYPE&)BASE_CLASS::GetTail(); }
    TYPE GetTail() const
        { return (TYPE)BASE_CLASS::GetTail(); }

    // Gets the head or tail (and removes it) - don't call an empty list!
    TYPE RemoveHead()
        { return (TYPE)BASE_CLASS::RemoveHead(); }
    TYPE RemoveTail()
        { return (TYPE)BASE_CLASS::RemoveTail(); }

    // Iteration
    TYPE& GetNext(POSITION& rPosition)
        { return (TYPE&)BASE_CLASS::GetNext(rPosition); }
    TYPE GetNext(POSITION& rPosition) const
        { return (TYPE)BASE_CLASS::GetNext(rPosition); }
    TYPE& GetPrev(POSITION& rPosition)
        { return (TYPE&)BASE_CLASS::GetPrev(rPosition); }
    TYPE GetPrev(POSITION& rPosition) const
        { return (TYPE)BASE_CLASS::GetPrev(rPosition); }

    // Gets/modifies an element at a given position.
    TYPE& GetAt(POSITION position)
        { return (TYPE&)BASE_CLASS::GetAt(position); }
    TYPE GetAt(POSITION position) const
        { return (TYPE)BASE_CLASS::GetAt(position); }
    void SetAt(POSITION pos, TYPE newElement)
        { BASE_CLASS::SetAt(pos, newElement); }
};

//**************************************************************************
template<class BASE_CLASS, class TYPE>
class ETypedPtrList : public _ETypedPtrList<BASE_CLASS, TYPE>
{
public:
// Construction
    ETypedPtrList(int nBlockSize = DEFAULT_BLOCK_SIZE)
        : _ETypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

    // Adds before the head or after the tail.
    POSITION AddHead(TYPE newElement)
        { return BASE_CLASS::AddHead(newElement); }
    POSITION AddTail(TYPE newElement)
        { return BASE_CLASS::AddTail(newElement); }

    // Adds another list of elements before the head or after the tail
    void AddHead(ETypedPtrList<BASE_CLASS, TYPE>* pNewList)
        { BASE_CLASS::AddHead(pNewList); }
    void AddTail(ETypedPtrList<BASE_CLASS, TYPE>* pNewList)
        { BASE_CLASS::AddTail(pNewList); }
};

//**************************************************************************
// Inlines for EPtrList

inline int EPtrList::GetCount() const
    { return m_nCount; }
inline BOOL EPtrList::IsEmpty() const
    { return m_nCount == 0; }
inline void*& EPtrList::GetHead()
    { ASSERT(m_pNodeHead != NULL);
        return m_pNodeHead->data; }
inline void* EPtrList::GetHead() const
    { ASSERT(m_pNodeHead != NULL);
        return m_pNodeHead->data; }
inline void*& EPtrList::GetTail()
    { ASSERT(m_pNodeTail != NULL);
        return m_pNodeTail->data; }
inline void* EPtrList::GetTail() const
    { ASSERT(m_pNodeTail != NULL);
        return m_pNodeTail->data; }
inline POSITION EPtrList::GetHeadPosition() const
    { return (POSITION) m_pNodeHead; }
inline POSITION EPtrList::GetTailPosition() const
    { return (POSITION) m_pNodeTail; }
inline void*& EPtrList::GetNext(POSITION& rPosition) // Returns *Position++
    { ENode* pNode = (ENode*) rPosition;
        rPosition = (POSITION) pNode->pNext;
        return pNode->data; }
inline void* EPtrList::GetNext(POSITION& rPosition) const // Returns *Position++
    { ENode* pNode = (ENode*) rPosition;
        rPosition = (POSITION) pNode->pNext;
        return pNode->data; }
inline void*& EPtrList::GetPrev(POSITION& rPosition) // Returns *Position--
    { ENode* pNode = (ENode*) rPosition;
        rPosition = (POSITION) pNode->pPrev;
        return pNode->data; }
inline void* EPtrList::GetPrev(POSITION& rPosition) const // Returns *Position--
    { ENode* pNode = (ENode*) rPosition;
        rPosition = (POSITION) pNode->pPrev;
        return pNode->data; }
inline void*& EPtrList::GetAt(POSITION position)
    { ENode* pNode = (ENode*) position;
        return pNode->data; }
inline void* EPtrList::GetAt(POSITION position) const
    { ENode* pNode = (ENode*) position;
        return pNode->data; }
inline void EPtrList::SetAt(POSITION pos, void* newElement)
    { ENode* pNode = (ENode*) pos;
        pNode->data = newElement; }



//**************************************************************************
// Derive EQueue from EPtrList
//
template<class BASE_CLASS, class TYPE>
class ETypedQueue : public ETypedPtrList<BASE_CLASS, TYPE>
{ 
public:
    ETypedQueue(int nBlockSize = DEFAULT_BLOCK_SIZE)
        : ETypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

    // Pushes (to tail).
    POSITION Push(TYPE newElement)
        { return BASE_CLASS::AddTail(newElement); }

    // Pushes list.
    void Push(ETypedPtrList<BASE_CLASS, TYPE>* pNewList)
        { BASE_CLASS::AddTail(pNewList); }


    // Pops (from head).
    TYPE Pop()
    { return BASE_CLASS::IsEmpty() ? NULL : (TYPE)BASE_CLASS::RemoveHead(); }

}; 

//**************************************************************************
// Derive EStack from EPtrList
//
template<class BASE_CLASS, class TYPE>
class  ETypedStack : public ETypedPtrList<BASE_CLASS, TYPE>
{ 
public:
    ETypedStack(int nBlockSize = DEFAULT_BLOCK_SIZE)
        : ETypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

    // Pushes (to head).
    POSITION Push(TYPE newElement)
        { return BASE_CLASS::AddHead(newElement); }

    // Pushes list.
    void Push(ETypedPtrList<BASE_CLASS, TYPE>* pNewList)
        { BASE_CLASS::AddHead(pNewList); }


    // Pops (from head).
    TYPE Pop()
    { return BASE_CLASS::IsEmpty() ? NULL : (TYPE)BASE_CLASS::RemoveHead(); }

}; 


// group=Maps
//**************************************************************************
class _DV_DECLSPEC EMapPtrToPtr
{
protected:
    // Association
    struct CAssoc
    {
        CAssoc* pNext;

        void* key;
        void* value;
    };

public:

// Construction
    EMapPtrToPtr(int nBlockSize = DEFAULT_BLOCK_SIZE);

// Attributes
    // Gets the number of elements.
    int GetCount() const;
    BOOL IsEmpty() const;

    // Performs a lookup.
    BOOL Lookup(void* key, void*& rValue) const;

// Operations
    // Performs a lookup and adds if not there.
    void*& operator[](void* key);

    // Adds a new (key, value) pair.
    void SetAt(void* key, void* newValue);

    // Removes existing (key, ?) pair.
    BOOL RemoveKey(void* key);
    void RemoveAll();

    // Iterates all (key, value) pairs.
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, void*& rKey, void*& rValue) const;

    // Performs advanced features for derived classes.
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (See map implementation for details.)
    // Routine used for user-provided hash keys.
    UINT HashKey(void* key) const;

// Implementation
protected:
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    int m_nCount;
    CAssoc* m_pFreeList;
    struct EPlex* m_pBlocks;
    int m_nBlockSize;

    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(void*, UINT&) const;

public:
    ~EMapPtrToPtr();
    void* GetValueAt(void* key) const;


protected:
    // Local typedefs for ETypedPtrMap class template
    typedef void* BASE_KEY;
    typedef void* BASE_ARG_KEY;
    typedef void* BASE_VALUE;
    typedef void* BASE_ARG_VALUE;
};

//**************************************************************************
class _DV_DECLSPEC EMapStringToPtr
{
protected:
    // Association
    struct CAssoc
    {
        CAssoc* pNext;
        UINT nHashValue;  // Needed for efficient iteration
        EString key;
        void* value;
    };

public:

// Construction
    EMapStringToPtr(int nBlockSize = DEFAULT_BLOCK_SIZE);

// Attributes
    // Gets the number of elements.
    int GetCount() const;
    BOOL IsEmpty() const;

    // Performs a lookup.
    BOOL Lookup(LPCTSTR key, void*& rValue) const;
    BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;

// Operations
    // Performs a lookup and adds if not there.
    void*& operator[](LPCTSTR key);

    // Adds a new (key, value) pair.
    void SetAt(LPCTSTR key, void* newValue);

    // Removes existing (key, ?) pair.
    BOOL RemoveKey(LPCTSTR key);
    void RemoveAll();

    // Iterates all (key, value) pairs.
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, EString& rKey, void*& rValue) const;

    // Advanced features for derived classes.
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (See map implementation for details.)
    // Routine used for user-provided hash keys.
    UINT HashKey(LPCTSTR key) const;

// Implementation
protected:
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    int m_nCount;
    CAssoc* m_pFreeList;
    struct EPlex* m_pBlocks;
    int m_nBlockSize;

    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(LPCTSTR, UINT&) const;

public:
    ~EMapStringToPtr();

protected:
    // Local typedefs for ETypedPtrMap class template
    typedef EString BASE_KEY;
    typedef LPCTSTR BASE_ARG_KEY;
    typedef void* BASE_VALUE;
    typedef void* BASE_ARG_VALUE;
};

//**************************************************************************
class _DV_DECLSPEC EMapStringToString
{
protected:
    // Association
    struct CAssoc
    {
        CAssoc* pNext;
        UINT nHashValue;  // Needed for efficient iteration
        EString key;
        EString value;
    };

public:

// Construction
    EMapStringToString(int nBlockSize = DEFAULT_BLOCK_SIZE);

// Attributes
    // Gets the number of elements.
    int GetCount() const;
    BOOL IsEmpty() const;

    // Performs a lookup.
    BOOL Lookup(LPCTSTR key, EString& rValue) const;
    BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;

// Operations
    // Performs a lookup and adds if not there.
    EString& operator[](LPCTSTR key);

    // Adds a new (key, value) pair.
    void SetAt(LPCTSTR key, LPCTSTR newValue);

    // Removes existing (key, ?) pair.
    BOOL RemoveKey(LPCTSTR key);
    void RemoveAll();

    // Iterates all (key, value) pairs.
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, EString& rKey, EString& rValue) const;

    // Advanced features for derived classes.
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (See map implementation for details.)
    // Routine used for user-provided hash keys.
    UINT HashKey(LPCTSTR key) const;

// Implementation
protected:
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    int m_nCount;
    CAssoc* m_pFreeList;
    struct EPlex* m_pBlocks;
    int m_nBlockSize;

    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(LPCTSTR, UINT&) const;

public:
    ~EMapStringToString();

protected:
    // Local typedefs for CTypedPtrMap class template
    typedef EString BASE_KEY;
    typedef LPCTSTR BASE_ARG_KEY;
    typedef EString BASE_VALUE;
    typedef LPCTSTR BASE_ARG_VALUE;
};

//**************************************************************************
// ETypedPtrMap<BASE_CLASS, KEY, VALUE>

// Functions needed for those compilers that don't understand typedefs inside a class declaration.
#define EMAP_PTR_TO_PTR       EMapPtrToPtr, void*, void*, void*, void*
#define EMAP_STRING_TO_PTR    EMapStringToPtr, EString, LPCTSTR, void*, void*
#define EMAP_STRING_TO_STRING EMapStringToString, EString, LPCTSTR, EString, LPCTSTR

template<class BASE_CLASS, class KEYTYPE, class ARG_KEYTYPE, class VALTYPE, class ARG_VALTYPE, class KEY, class VALUE>
class ETypedPtrMap : public BASE_CLASS
{
public:

// Construction
    ETypedPtrMap(int nBlockSize = DEFAULT_BLOCK_SIZE)
        : BASE_CLASS(nBlockSize) { }

    // Performs a lookup.
    BOOL Lookup(ARG_KEYTYPE key, VALUE& rValue) const
        { return BASE_CLASS::Lookup(key, (VALTYPE&)rValue); }

    // Performs a lookup and adds if not there.
    VALUE& operator[](ARG_KEYTYPE key)
        { return (VALUE&)BASE_CLASS::operator[](key); }

    // Adds a new key (key, value) pair.
    void SetAt(KEY key, VALUE newValue)
        { BASE_CLASS::SetAt(key, newValue); }

    // Removes existing (key, ?) pair.
    BOOL RemoveKey(KEY key)
        { return BASE_CLASS::RemoveKey(key); }

    // Iteration
    void GetNextAssoc(POSITION& rPosition, KEY& rKey, VALUE& rValue) const
        { BASE_CLASS::GetNextAssoc(rPosition, (KEYTYPE&)rKey,
            (VALTYPE&)rValue); }
};

// group=General
//**************************************************************************
// EString
// {secret}
extern LPCTSTR _DV_DECLSPEC _ePchNil;                       
#define emptyString ((EString&)*(EString*)&_ePchNil)       // {secret}

inline EStringData* EString::GetData() const
    { ASSERT(m_pchData != NULL); return ((EStringData*)m_pchData)-1; }
inline void EString::Init()
    { m_pchData = emptyString.m_pchData; }
inline EString::EString()
    { m_pchData = emptyString.m_pchData; }
inline EString::EString(const unsigned char* lpsz)
    { Init(); *this = (LPCSTR)lpsz; }
inline const EString& EString::operator=(const unsigned char* lpsz)
    { *this = (LPCSTR)lpsz; return *this; }
#ifdef UNICODE
inline const EString& EString::operator+=(char ch)
    { *this += (TCHAR)ch; return *this; }
//group=String Operators
inline EString operator+(const EString& string, char ch)
    { return string + (TCHAR)ch; }
inline EString operator+(char ch, const EString& string)
    { return (TCHAR)ch + string; }
#endif

inline int EString::GetLength() const
    { return GetData()->nDataLength; }
inline int EString::GetAllocLength() const
    { return GetData()->nAllocLength; }
inline BOOL EString::IsEmpty() const
    { return GetData()->nDataLength == 0; }
inline EString::operator LPCTSTR() const
    { return m_pchData; }
// Added by JEK.  This is actually a const string that 
// MUST not be modified.
// This is 1. faster then GetBuffer(0)
//         2. Doesn't do a string copy for multiple references.
inline TCHAR* EString::GetPtr() const
    { return (TCHAR*) m_pchData; }

inline int EString::SafeStrlen(LPCTSTR lpsz)
    { return (lpsz == NULL) ? 0 : (int) _tcslen(lpsz); }

// EString support
inline int EString::Compare(LPCTSTR lpsz) const
    { return _tcscmp(m_pchData, lpsz); }    // MBCS/Unicode aware
inline int EString::CompareNoCase(LPCTSTR lpsz) const
    { return _tcsicmp(m_pchData, lpsz); }   // MBCS/Unicode aware
inline TCHAR EString::GetAt(int nIndex) const
{
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
inline TCHAR EString::operator[](int nIndex) const
{
    // Same as GetAt
    ASSERT(nIndex >= 0);
    ASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
// group=String Operators
inline bool  operator==(const EString& s1, const EString& s2)
    { return s1.Compare(s2) == 0; }
inline bool  operator==(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) == 0; }
inline bool  operator==(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) == 0; }
inline bool  operator!=(const EString& s1, const EString& s2)
    { return s1.Compare(s2) != 0; }
inline bool  operator!=(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) != 0; }
inline bool  operator!=(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) != 0; }
inline bool  operator<(const EString& s1, const EString& s2)
    { return s1.Compare(s2) < 0; }
inline bool  operator<(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) < 0; }
inline bool  operator<(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) > 0; }
inline bool  operator>(const EString& s1, const EString& s2)
    { return s1.Compare(s2) > 0; }
inline bool  operator>(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) > 0; }
inline bool  operator>(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) < 0; }
inline bool  operator<=(const EString& s1, const EString& s2)
    { return s1.Compare(s2) <= 0; }
inline bool  operator<=(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) <= 0; }
inline bool  operator<=(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) >= 0; }
inline bool  operator>=(const EString& s1, const EString& s2)
    { return s1.Compare(s2) >= 0; }
inline bool  operator>=(const EString& s1, LPCTSTR s2)
    { return s1.Compare(s2) >= 0; }
inline bool  operator>=(LPCTSTR s1, const EString& s2)
    { return s2.Compare(s1) <= 0; }

//**************************************************************************
// Special implementations for EStrings
// It is faster to bit-wise copy a EString than to call an official
// constructor, since an empty EString can be bit-wise copied.

static inline void ConstructElement(EString* pNewData)
{
    memcpy_s(pNewData, sizeof(EString), &emptyString, sizeof(EString));
}

static inline void DestructElement(EString* pOldData)
{
    pOldData->~EString();
}

static inline void CopyElement(EString* pSrc, EString* pDest)
{
    *pSrc = *pDest;
}

// {secret}
#define BEFORE_START_POSITION ((POSITION)-1L)
//**************************************************************************
inline int EMapPtrToPtr::GetCount() const
    { return m_nCount; }
inline BOOL EMapPtrToPtr::IsEmpty() const
    { return m_nCount == 0; }
inline void EMapPtrToPtr::SetAt(void* key, void* newValue)
    { (*this)[key] = newValue; }
inline POSITION EMapPtrToPtr::GetStartPosition() const
    { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline UINT EMapPtrToPtr::GetHashTableSize() const
    { return m_nHashTableSize; }
//**************************************************************************
inline int EMapStringToPtr::GetCount() const
    { return m_nCount; }
inline BOOL EMapStringToPtr::IsEmpty() const
    { return m_nCount == 0; }
inline void EMapStringToPtr::SetAt(LPCTSTR key, void* newValue)
    { (*this)[key] = newValue; }
inline POSITION EMapStringToPtr::GetStartPosition() const
    { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline UINT EMapStringToPtr::GetHashTableSize() const
    { return m_nHashTableSize; }
//**************************************************************************
inline int EMapStringToString::GetCount() const
    { return m_nCount; }
inline BOOL EMapStringToString::IsEmpty() const
    { return m_nCount == 0; }
inline void EMapStringToString::SetAt(LPCTSTR key, LPCTSTR newValue)
    { (*this)[key] = newValue; }
inline POSITION EMapStringToString::GetStartPosition() const
    { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline UINT EMapStringToString::GetHashTableSize() const
    { return m_nHashTableSize; }

//*****************************************************************************
// MStringList
// Memory Managed String List

// This manages a list of EString pointers.  But instead of a normal EPtrList, 
// this allocates a new EString for each Added EString*.  The MStringList
// manages the life time of the allocated EString*.
// 
// Not all EPtrList functions were added here, feel free to enhance as needed.
//*****************************************************************************
class _DV_DECLSPEC MStringList  : public EPtrList
{
public:
    ~MStringList();

    int GetCount() const;
    BOOL IsEmpty() const;

    // Peeks at the head or tail.
    EString*& GetHead();
    EString* GetHead() const;
    EString*& GetTail();
    EString* GetTail() const;

    // Adds before the head or after the tail.
    POSITION AddHead(EString* pes);
    POSITION AddHead(TCHAR* pszString);
    POSITION AddTail(EString* pes);
    POSITION AddTail(TCHAR* pszString);
    // Defaults to starting at the HEAD.
    // Return NULL if not found.
    POSITION Find(EString* pesSearchValue) const;

    void RemoveAt(POSITION position);
    void* RemoveHead();
    void* RemoveTail();
    void RemoveAll();

    // Iteration
    POSITION GetHeadPosition() const;
    POSITION GetTailPosition() const;
    EString*& GetNext(POSITION& rPosition); // Returns *Position++
    EString* GetNext(POSITION& rPosition) const; // Returns *Position++
    EString*& GetPrev(POSITION& rPosition); // Returns *Position--
    EString* GetPrev(POSITION& rPosition) const; // Returns *Position--

    // Gets/modifies an element at a given position.
    EString*& GetAt(POSITION position);
    EString* GetAt(POSITION position) const;

    // Deletes old EString, allocate and SetAt() new one
    void SetAt(POSITION pos, EString* newElement);


};

#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, char*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, char*>;
#endif // WIN32

//**************************************************************************
class _DV_DECLSPEC EWTOAHelper : public ETypedPtrList<EPtrList, char*>
{
public:
    ~EWTOAHelper();

    char *Convert(TCHAR *pWCString);
};

#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, TCHAR*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, TCHAR*>;
#endif // WIN32

//**************************************************************************
class _DV_DECLSPEC EATOWHelper : public ETypedPtrList<EPtrList, TCHAR*>
{
public:
    ~EATOWHelper();

    TCHAR *Convert(char *pAString);
};

//**************************************************************************
template<class T> class EBasicAutoArray
{
public:
    EBasicAutoArray(T *ptr = 0) :
        m_ptr(ptr)
    {
    }

    EBasicAutoArray(size_t n) :
        m_ptr(new T[n])
    {
    }

    EBasicAutoArray(EBasicAutoArray<T>& x) :
        m_ptr(x.Release())
    {
    }

    ~EBasicAutoArray()
    {
        if (m_ptr)
            delete [] m_ptr;
    }

    EBasicAutoArray<T>& operator=(EBasicAutoArray<T>& x)
    {
        Reset(x.Release());
        return *this;
    }

    T operator[](int i)
    {
        return m_ptr[i];
    }

    T *GetData(void) const
    {
        return m_ptr;
    }

    void Reset(T *ptr)
    {
        if (m_ptr)
            delete [] m_ptr;
        m_ptr = ptr;
    }

    T *Release(void)
    {
        T *result = m_ptr;
        m_ptr = 0;
        return result;
    }

private:
    T *m_ptr;
};

#endif
