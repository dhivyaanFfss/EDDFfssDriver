//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  Utility.h
//
//  Subsystem  :  Kernel
//
//  Description:  define static utility functions available to all components.
//
//  Classes:    EUtility
//
//**************************************************************************

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "ETime.h"
#include "EArrays.h"

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

#define SwapInt16(x) ((((x) & 0xff00) >> 8) | (((x) & 0x00ff) <<  8))
#define SwapInt32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
                      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#define SwapTCHAR(x) (sizeof(TCHAR) == 4 ? SwapInt32(x) : SwapInt16(x))


class ECipherKey;

class _DV_DECLSPEC IPRange
{
public:
    IPRange()  {}
    IPRange(IPRange* psrc)
    {
        addr_start = psrc->addr_start;
        addr_end = psrc->addr_end;
        m_esCommunity = psrc->m_esCommunity;
    }
    bool operator==(IPRange& rIPRange)
    {
        if (addr_start == rIPRange.addr_start
            && addr_end == rIPRange.addr_end
            && m_esCommunity == rIPRange.m_esCommunity)
            return true;
        return false;
    }

    EULONG32 addr_start;
    EULONG32 addr_end;
    EString m_esCommunity;        // this will be empty at intial discovery stage.
};

//**************************************************************************
typedef struct {
    EULONG32 time_low;                 /* bits  0-31 of time field */
    USHORT  time_mid;                  /* bits 32-47 of time field */
    USHORT  time_hi_and_version;       /* bits 48-59 of time field plus 4 bit version */
    UCHAR   clock_seq_hi_and_reserved; /* bits  8-13 of clock sequence field plus 2 bit variant */
    UCHAR   clock_seq_low;             /* bits  0-7  of clock sequence field */
    UCHAR   node[6];                   /* bits  0-47 of node MAC address */
} DCE_UUID;

#ifdef WIN32
template class _DV_DECLSPEC ETypedPtrArray<EPtrArray, IPRange*>;
#endif
typedef ETypedPtrArray<EPtrArray, IPRange*> IPRangeArray;

// forward declaration
interface IMsgManager;
interface IKernelBase;
interface IGateway;
class _DV_DECLSPEC EUtility
{
public:
    static void SetTime(EString *pcsTime, ETime &rTime);
    static void SetDate(EString *pcsDate, ETime &rTime);
    static void SetDateFormatFlag();

    static bool JISEncode(const WCHAR *rIn,EByteArray &rOut, bool bAddNull);
    static bool Iso2022JpEncode(const TCHAR *rIn, EByteArray &rOut);

    static void Utf8Encode(const TCHAR *rIn,EByteArray &rOut);

    static void Utf8Decode(const char * rIn,TCHAR * rOut, int inLen = -1);
    static void PrepXML(const EString& in,EString& out);

    static void QuotedPrintableEncode(const char * rIn,char * rOut);
    static void QuotedPrintableEncode(EByteArray &rIn,EByteArray &rOut);
    static void Base64Encode(const char * rIn,char * rOut,int iLen = -1);
    static void Base64Encode(EByteArray &rIn,EByteArray &rOut);
    static void Base64Decode(const char * rIn,char * rOut);
    static void Base64Decode(EByteArray &rIn,EByteArray &rOut);
    static void UrlDecode(const char *rIn,char *rOut);

	static TCHAR* DecryptPassword(const TCHAR* pIn);
	static TCHAR* EncryptPassword(const TCHAR* pIn);

	static TCHAR* EncryptUNICODEString(const TCHAR* pIn);
	static TCHAR* DecryptUNICODEString(const TCHAR* pIn);

    // Convert each byte in array to a 2 byte hex representation  (lower case hex) and
    // place in the output string resHexEncoded.
    static void HexEncode(EByteArray& rIn, EString& resHexEncoded);

    // Opposite of HexEncode
    static EString HexDecode(EString&);

// parsing the string for /n /t /.... and change them for 0x0A...
    static void ParsString(TCHAR *);

    static bool MatchShellExpression(TCHAR *str, TCHAR *shexp);

    // Serialization members
    static bool Save(EFileIO*, char* psrc, DWORD& size);
    static bool Restore(EFileIO*, char*& pdst, DWORD& size);

    static void ClearIPRange(IPRangeArray&);

    // Defined for GetNamedValue.
#define _NV_EQUALS_     _T('=')  // NV -- name value
#define _NV_DELIMITER   _T(',')

    // Find the named value in a string.
    // Format:  Name=value, host=192.168.2.3, x=y ...
    // thus GetNamedValue of "host" sets esValue to 192.168.2.3
    // return true if found, false if not.  If not found, esValue is empty
    static bool GetNamedValue(EString& esSource, TCHAR* pszPropertyName,
        /* out */ EString& esValue);

    // For Gateway, parse name and check if device exits.
    static bool IsValidDeviceName(IGateway* pIGateway, EString& esName);

	// Output strings to stdout
	static void WriteToConsole(TCHAR* pBuffer);

    // Formats date/time according to RFC 1123
    // (current time is used when the second arg is NULL)
    static bool FormatHttpDate(EString& esTime, ETime *pTime);

    // Parses HTTP date/time according
    static bool ParseHTTPDate(const EString& esTime, ETime& theTime);

    // Opposite of HexEncode
    static bool HexDecode(EByteArray& rOut, EString& resHexEncoded);

    // Generates random name
    static void GetTempName(EString& esTempName);

    static bool LocateExecutable(const EString& name, EString& exeName);

    static bool IsMACAddressValid(UCHAR (&macAddress)[8]);
    static bool SelectUUIDMACAddress(ETypedPtrArray<EPtrArray, UCHAR*> &macArray, UCHAR (&macAddr)[6]);

    static void CreateUUID(TCHAR *pMN, TCHAR *pSN, EString& esUUID);
    static int GetUUIDVersion(const EString& esUUID);

    static void GetArguments(const EString& strParam, EStringArray& args);

    static TCHAR *GetFormattedString(const TCHAR *pszFormat, va_list *pArgs);

    // Enables or disables console echo
    static void SetConsoleEcho(bool bEnable);

private:
    static bool bFormatDayFirst;

    static USHORT GetJisX0208Char(TCHAR ch);

	static UCHAR m_MACAddr[6];
	static bool  m_bMACAddrInitialized;

};

//**************************************************************************
class _DV_DECLSPEC EProgramArguments : public EStringArray
{
public:
    EProgramArguments(int argc, TCHAR* argv[]);
    EProgramArguments(int argc, char* argv[]);
};

#endif

