//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  DataSrcManInterface
//  
//  Subsystem  :  Kernel
//
//  Description:  Public Data Source Interfaces
//
//  Interfaces :  IGetItemType, 
//                IDataSource, 
//                IGetDataSource,
//                IDataServer,
//                IDataSourceProvider,
//                IDataSourceMgr,
//
//**************************************************************************

#ifndef __DATASRCMAN_INTERFACE_H__
#define __DATASRCMAN_INTERFACE_H__

#include "KernelInterface.h"
#include "DataItem.h"

#ifndef  interface
#define  interface   struct     // {secret}
#endif

// group=Data Source

////////////////////////////////////////////
// Data type is actually the provider of the ID.
// Each provider must register itself in Data Source Manager.
// Each call to the Data Source Manager will be redirected to Data Source Provider.

#define     DSTYPE      USHORT

//
//
//                      +------------------------------+
//                      |                              |
//                      |     Data Source Manager      |
//                      |                              |
//                      +------------------------------+
//                      |  IDataSourceManInterface     |
//                      +------------------------------+
//                   ||              ||             ||
//             *-----------*    *-----------*   *-----------*
//             | Data      |    | Data      |   | Data      |
//             | Source    |    | Source    |   | Source    |
//             | Provider  |    | Provider  |   | Provider  |
//             | Interface |    | Interface |   | Interface |
//             *-----------*    *-----------*   *-----------*
//             | Provider  |    | Provider  |   | Provider  |
//             | OPC (1)   |    | VPI (2)   |   | EDD (3)   |
//             |           |    |           |   |           |
//             | D. Server |    | D. Server |   | D. Server |
//             | D. Source |    | D. Source |   | D. Source |
//             *-----------*    *-----------*   *-----------*
//


#define    DUMMY          0
#define    OPC            1
#define    VPI            2
#define    EDD            3

//////////////////////////////////////////
struct DSPARAM
{
    DSPARAM() : enDSType(DUMMY), pszNode(NULL), pszServer(NULL), nUpdateRate(1000),
        dDeadband(0.0), bActiveState(false)
    {}


    // Server type (DUMMY / OPC / VPI / EDD /Custom ...)
    DSTYPE        enDSType;            

    // Network address of server - NULL if local
    TCHAR*        pszNode;             
    
    // Server Name
    TCHAR*        pszServer;           
    
    // Update (Sample) Rate
    unsigned long nUpdateRate;         
    
    // Percent Deadband
    double        dDeadband;           
    
    // Active State
    bool          bActiveState; 

    // Override data value's timestamp
    bool        bUseSystemTime;
};

////////////////////////////////////////////////////////////////////////
// Forward definition
interface IDataServer;

// Handle to Data Source Item
typedef void*   HDSITEM;


//**************************************************************************
//
// Connector supports the following data point types  - THIS IS FOR VPI only/
// 
//**************************************************************************

#define SVR_ITEM_TYPE_UNSIGNED_16         0
#define SVR_ITEM_TYPE_SIGNED_16           1
#define SVR_ITEM_TYPE_BCD                 2
#define SVR_ITEM_TYPE_FLOAT               3
#define SVR_ITEM_TYPE_SIGNED_32           4
#define SVR_ITEM_TYPE_UNSIGNED_32         5
#define SVR_ITEM_TYPE_DIGITAL             6
#define SVR_ITEM_TYPE_STRING              7

/////////////////////////////////////////////////////////////////////////////
//
// Group types supported
//
/////////////////////////////////////////////////////////////////////////////
#define GROUP_NONBLOCK              0           // Non-block group
#define GROUP_SNMPTABLE             1           // SNMP Table group
#define GROUP_VPIBLOCK              2           // VPI block group with

/////////////////////////////////////////////////////////////////////////////
struct EVpiItem
{
    EString         m_strItemName;     // PLC Address
    IGetItemType*   m_pIGetType;       // Tag Type (ANALOG/DIGITAL...)
    bool            m_bActive;         // Is Active
    int             m_plcAddr;         // device number in VPI
    int             m_memType;         // Memory type of gate
    int             m_gateAddr;        // each gate is 1 distance from the other
                                       // in units of memory block. See blocksize below
    unsigned        m_bitNumber;       // Bit offset for digital gates with LSB is 0
    int             m_elementSize;     // In bits
};


/////////////////////////////////////////////////////////////////////////////
typedef ETypedPtrList<EPtrList, EVpiItem*>          EVpiItemsList;
/////////////////////////////////////////////////////////////////////////////

// support for block operations
class EVpiBlock
{
public:
    EVpiBlock() : m_pBuffer(NULL), m_pValues(NULL)     {}
    ~EVpiBlock();
    int             m_blockSize;        // In units of memory block such as LONG, SHORT, BYTE , no in bytes
    char*           m_pBuffer;
    CDataValue*     m_pValues;
    EVpiItemsList   m_blockItemsList;
    EVpiItem        m_BlockItem;
};
typedef ETypedPtrList<EPtrList, EVpiBlock*>     EVpiBlocksList;


////////////////////////////////////////////////////////////////////////
// Callback that will be called on tag change
// See Also:  IDataSource::SetCallback
typedef void (*KernelCallback) (LONG_PTR, const CDataValue&);
typedef void (*KernelBlockCallback) (DWORD, EHANDLE*, CDataValue*);




////////////////////////////////////////////////////////////////////////
// IDataSource interface.

interface IDataSource : public IKernelBase
{
   virtual KERESULT    AddRef() = 0;

   virtual KERESULT    Release() = 0;

   virtual TCHAR*      GetName() = 0;
   
   virtual void        GetParam(DSPARAM&) const = 0;
   
   virtual KERESULT    SetUpdateRate(long) = 0;
   virtual long        GetUpdateRate() = 0;

   virtual KERESULT    SetDeadband(double) = 0;
   
   virtual KERESULT    SetActiveState(bool) = 0;

   virtual HDSITEM     AddItem(const TCHAR* strItemName, const TCHAR* strAccessPath,
                               IGetItemType* pIGetType, bool bInitialState,
                               double& dServerMin, double& dServerMax) = 0;
   
   virtual EVpiBlock*  CreateBlock(EString& address) = 0;
   virtual HDSITEM     AddBlockItem(EVpiBlock*, const TCHAR* strItemName, IGetItemType*) = 0;
   virtual KERESULT    ParseBlock(EVpiBlock* pBlock) = 0;
   
   virtual KERESULT    RemoveItem(HDSITEM hItem) = 0;

   virtual KERESULT    SetItemActiveState(HDSITEM hItem, bool bState = true) = 0;

   virtual KERESULT    ReadItem(HDSITEM hItem, CDataValue& Value,
                                              long nTimeout = 0) = 0;
   
   virtual KERESULT    ReadItems(int nCount, HDSITEM hItem[], CDataValue Value[],
                                              long nTimeout = 0) = 0;

   virtual KERESULT    WriteItem(HDSITEM hItem, CDataValue& Value) = 0;
   
   virtual KERESULT    WriteItems(int nCount, HDSITEM hItem[], CDataValue Value[]) = 0;

   virtual KERESULT    SetCallback(KernelCallback pFnc, 
       KernelBlockCallback pBlockFnc) = 0;
   
   virtual KERESULT    ResetCallback() = 0;

   virtual IDataServer* GetServer() = 0;

   // Does data source publish blocks of data or only individual items to
   // the data listener?
   virtual KERESULT     IsBlockDataPublished(bool *pbAreBlocksPublished) = 0;

#ifdef __GATEWAY__
   // Gateway has special add item that passes the Gateway id
   virtual HDSITEM     AddItem(int gatewayId,
                               const TCHAR* strItemName, const TCHAR* strAccessPath,
                               IGetItemType* pIGetType, bool bInitialState,
                               double& dServerMin, double& dServerMax) = 0;
#endif 
};



////////////////////////////////////////////////////////////////////////
// GetDataSource() function may be used by different Kernel components, but
// shouldn't be exposed outside the Kernel. Therefore, it needs a separate interface.
interface IGetDataSource
{
   virtual IDataSource* GetDataSource(const TCHAR*) = 0;
};

////////////////////////////////////////////////////////////////////////
class EFileIO;
interface IDataServer
{
   virtual KERESULT    AddRef() = 0;

   virtual KERESULT    Release() = 0;

   virtual DSTYPE         GetType() const   = 0;
   virtual TCHAR*         GetNode()			= 0;
   virtual TCHAR*         GetServer()       = 0;
   virtual TCHAR*         GetKey()          = 0;
   virtual bool           IsEmpty()         = 0;
   virtual int            GetSize()         = 0;
   virtual IDataSource*   GetDataSource(const TCHAR* strName) = 0;
   virtual KERESULT       ReConnectServer() = 0;
   virtual IDataSource*   AddDataSource(const TCHAR* strName,
                                        long nUpdateRate, 
                                        double dDeadband, 
                                        bool bActiveState,
                                        bool bInsert,
                                        bool bUseSystemTime) = 0;

   virtual KERESULT       RemoveDataSource(const TCHAR* strName) = 0;
   virtual KERESULT       Connect() = 0;        // called before tags are added to the server.
   virtual KERESULT       StartSampling() = 0;  // called after tags are added to the server.
   virtual KERESULT       Disconnect() = 0;
    virtual KERESULT        SaveUpdateRates(EFileIO*) = 0;
    virtual KERESULT        RestoreUpdateRates(EFileIO*) = 0;

#ifdef __GATEWAY__
   // Special Gateway method to add a driver client per serial number.
   // This is used specifically by the EDD Data Source.
   virtual KERESULT       AddDriverClient(int gatewayId) = 0;
#endif
};

////////////////////////////////////////////////////////////////////////
interface IDataSourceProvider
{
    virtual IDataServer* CreateDataServer(TCHAR *strNode, TCHAR *strServer) = 0;
    
    virtual bool         DestroyDataServer(IDataServer*) = 0;

};

////////////////////////////////////////////////////////////////////////
interface IDataSourceMgr : public IKernelBase
{
    virtual bool    RegisterDataSrcProvider(DSTYPE enDSType,IDataSourceProvider* pProvider) = 0;
    virtual bool    UnregisterDataSrcProvider(DSTYPE enDSType) = 0;

    virtual int     AddDataSource(const TCHAR* strDSName, const DSPARAM& param) = 0;
    virtual int     RemoveDataSource(const TCHAR* strDSName) = 0;
    
    virtual int     GetDataSrcParam(const TCHAR* strDSName, DSPARAM& param) = 0;
    
    virtual int     SetDataSrcUpdateRate(const TCHAR* strDSName, long nRate) = 0;
    virtual int     SetDataSrcDeadband(const TCHAR* strDSName, double dDeadband) = 0;
    virtual int     SetDataSrcActiveState (const TCHAR* strDSName, bool bState = true) = 0;
};


#endif

