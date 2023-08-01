//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  pch.h
//  
//  Subsystem  :  Kernel
//
//  Description:                     
//
//  Classes    :  
//
//**************************************************************************

#ifndef ___EVPICOMMON__H__
#define ___EVPICOMMON__H__

// Define the maximum length of a string tag
#define MAX_STRING_TAG_LENGTH 100

// including terminating zero 
#define VPI16_GATE_ADDRESS_LEN     9     

// Length of adress
#define GATE_ADDRESS_LEN           32

// Support for Wizcon
typedef char        WIZ_GT_ADDR[GATE_ADDRESS_LEN];

// Gate ID - Wizcon
typedef EULONG32    WIZ_GT_ID;

#define VPI_INTERFACE_VER  70

/* VPI functions */
#define VPI_COLD_START          1
#define VPI_WARM_START          2
#define VPI_PARSE_GATE          3
#define VPI_PARSE_BLOCK         4
#define VPI_READ_GATE           5
#define VPI_READ_BLOCK          6
#define VPI_WRITE_GATE          7
#define VPI_CONNECT_NODE        8
#define VPI_RCS_QUERY           9
#define VPI_WRITE_BLOCK         10
#define VPI_SHUTDOWN            11
#define VPI_CLOSE_COM           12
#define VPI_REOPEN_COM          13
#define VPI_GET_INFO            14
#define VPI_CHANGE_PARAMS       15
#define VPI_BROWSE_ADDRESS      16              //OPC


#define WPARITY_NONE   0
#define WPARITY_ODD    1
#define WPARITY_EVEN   2
#define WPARITY_MARK   3
#define WPARITY_SPACE  4

#define WSTOPBITS_1    0
#define WSTOPBITS_15   1
#define WSTOPBITS_2    2

#define DEV_NAME_LEN   12

typedef struct
{                       // for serial VPIs
  SHORT    baud;
  SHORT    wordSize;
  SHORT    stopBits;
  SHORT    parity;
} SERPARAMS;

typedef struct
{
  SHORT    station;
  SHORT    adaptor;
} NWPARAMS;


typedef union
{
  SERPARAMS   com;
  NWPARAMS    nw;  
} DEVICEPARAMS;


/* vpi callback services
*/

/* sample adjustment   type               val meaning */
#define SET_SAMPLE_RATE     0            /* milliseconds to sample on         */
#define SAMPLE_STOP         1            /* dont sample again until restarted */
#define SAMPLE_START        2            /* restart sampling                  */
#define SAMPLE_NOW          3            /* sample now                        */
#define SAMPLE_SYNCHRONIZE  4            /* millisec timeout                  */
#define XML_FILE_HANDLE     5            /* Driver needs XML file handle      */
#define DATASET_COMPLETE    32           /* tchar* proprietary dataset name   */

typedef DWORD (*VpiCallbackFunction)(void* instDat, int adjustType, ULONG_PTR adjustVal);

/* VDB - Vpi Data Block */
typedef struct
{
  void     *pInstanceData;         // Pointer to per instance vpi data
  int      function;               // Function number called
  int      vpiNumber;              // Vpi number
  int      parmSize;               // Size of additional function data
  void     *pParam;                // Pointer to additional function data
} VDB;

/* vpi types in GETINFO_VFD.type */
#define VPITYPE_NULL    1
#define VPITYPE_SPECIAL 2
#define VPITYPE_NW      3
#define VPITYPE_COM     4
#define VPITYPE_OPC     5 //OPC

/* flags in GETINFO_VFD.flag */
#define VPISUPPORT_READBLOCK   0x0001
#define VPISUPPORT_WRITEBLOCK  0x0002
#define VPISUPPORT_STRINGGATE  0x0004
#define VPISUPPORT_UNSOLICTED  0x0008
#define VPISUPPORT_WIZPLC      0x0010
#define VPISUPPORT_BROWSE      0x0020   //OPC
#define VPISUPPORT_XML_INT     0x0040   // Turn on to have Axeda Connector read XML file

/* Addtional data for VPI_GET_INFO function */
typedef struct
{
  int            ver;              // input/output : VDB version
  TCHAR*         pName;            // VPI output: Pointer to VPI name
  TCHAR*         pMessage;         // VPI output: Pointer to VPI message
  int            instanceDataSize; // VPI output: Size of data required
  int            type;             // VPI output: VPI type
  DEVICEPARAMS   p;                // VPI output: default settings
  EULONG32       flags;            // VPI output: VPISUPPORT_????
} GETINFO_VFD;


/* Addtional data for VPI_COLD_START function */
typedef struct
{
  char           devName[DEV_NAME_LEN];  // Device name to connect to
  int            comOn;                  // True is the VPI is loaded for
                                         // Communication.
                                         // Else load for Parsing or check
  int            timeout;                // Max time VPI should wait
  DEVICEPARAMS   p;

  VpiCallbackFunction   pVpiCallback;    // pointer to sampler adustment function
  void*                 ownerData;       // opaque owner data
} COLDSTART_VFD;


/* Addtional data for VPI_WARM_START function */
typedef struct
{
  int            plcAddr;
} WARMSTART_VFD;


/* Addtional data for VPI_CHANGE_PARAMS function */
typedef struct
{
  char           devName[12];        // Device name ("COM1"...)
  int            timeout;            // Max time VPI should wait
  DEVICEPARAMS   p;
} CHANGEPARAMS_VFD;


/* Addtional data for VPI_PARSE_SINGLE function */
typedef struct
{
  WIZ_GT_ADDR    *gateAddrStr;       // Pointer to gate address
  WIZ_GT_ID      gateId;             // Gate ID
  int            gateType;           // Type of gate to parse
  int            blockSize;          // Number of gates
  int            plcAddr;            // Parse result: VPI output
  int            memType;            // Parse result: VPI output
  int            gateAddr;           // Parse result: VPI output
  unsigned       bitNumber;          // Parse result: VPI output
  int            elementSize;        // Parse result: VPI output
} PARSE_VFD;


/* Addtional data for VPI_READ_SINGLE function */
typedef struct
{
  WIZ_GT_ADDR    *gateAddrStr;     // Pointer to gate address string
  void*          p1;               // Additional pointer
  int            plcAddr;          //
  int            memType;          //
  int            gateAddr;         //
  unsigned       bitNumber;        //
  int            elementSize;      //
  int            gateType;         // Type of gate to read. GATETYPE_????
  int            blockSize;        // length of block
  WIZ_GT_ID      gateId;           // Gate id to read.
  void*          pVal;             // Pointer to gate value
  ULONG*         pTimeStamp;       // Pointer to time stamp.
} READWRITE_VFD;

//OPC
/* Addtional data for VPI_BROWSE_ADDRESS function */
typedef struct
{
  WIZ_GT_ADDR    *gateAddrStr;       // Pointer to gate address
} BROWSE_VFD;

#ifndef WIN32
typedef int (*VPIFUNC)(VDB * pVDB);
#else  
typedef int (APIENTRY VPIFUNC)(VDB *);
#endif // WIN32

// Gate Format
#define WIZ_GT_FRMT_UNSIGNED16     0
#define WIZ_GT_FRMT_SIGNED16       1
#define WIZ_GT_FRMT_BCD            2
#define WIZ_GT_FRMT_FLOAT          3
#define WIZ_GT_FRMT_SIGNED32       4
#define WIZ_GT_FRMT_UNSIGNED32     5
#define WIZ_GT_FRMT_DIGITAL        6
#define WIZ_GT_FRMT_STRING         7


#pragma pack (1)

typedef union plc_data
{  struct
  {
     USHORT      DataHigh;   /* used just to put the low word in place */
     union
     {
        USHORT  us;
        SHORT   s;
        USHORT  bcd;
     } Low;
  } Data;
  float  fData;
  EULONG32  ulData;
  ELONG32   lData;
} WIZ_PLC_DATA;

#pragma pack()

/* Wizpro VPI library (Vlb functions) */

#define DTR    0x0101
#define CTS    0x0010
#define RTS    0x0202
#define DSR    0x0020


#endif
