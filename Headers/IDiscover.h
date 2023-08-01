//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IDiscover.h
//  
//  Subsystem  :  Kernel - Gateway
//
//  Description:  Define IDiscover interface to be used by all auto-discovery
//                implementations.
//
//  Classes:
//              IDiscover
//              ISNMPDiscoveryConfig
//              IDeviceStatusListener
//              IDeviceStatus
//
//**************************************************************************


#ifndef __IDISCOVER_H__
#define __IDISCOVER_H__

// Support classes
//**************************************************************************
// Serial Number info object
class ESnInfo
{
public:
    // keep this int as first data member
    int         m_Version;

    ESnInfo() {m_Version = 1;}

    EString     m_esSn;     // Serial number
    // Implementation specific configuration string.  This string is the
    // configuration parameter used when creating a EDDDriver object.
    // It is retrieved from the driver during discover and given back to 
    // the driver during device creation.
    // This information is required per device serial number.
    // In the SNMP driver implementation, this is the Node Name (IP address).
    EString     m_esConfig; 

    // Added for 3.6
    EMapStringToString  m_DeviceInfoMap;
};
// Some possible property names in the device info map.
#define DEVICE_INFO_IPADDRESS       _T("IPAddress")
#define ORIGINAL_MN                 _T("OrigMn")


class  ESnInfos : public ETypedPtrList<EPtrList, ESnInfo*>
{
public:
    
    void DeleteAll()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            ESnInfo*    pSnInfo = GetNext(pos);
            delete pSnInfo;
        }
        RemoveAll();
    }

    ESnInfo* Find(EString& esSn)
    {
        return Find(esSn.GetPtr());
    }
    
    ESnInfo* Find(TCHAR* pszSn)
    {
        if (pszSn == NULL)
            return NULL;
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            ESnInfo*    pSnInfo = GetNext(pos);
            if (pSnInfo->m_esSn == pszSn)
                return pSnInfo;
        }
        return NULL;
    }


};

//**************************************************************************
// Model Number Info object
// Each model number classification 
class EMnInfo
{
public:
    ~EMnInfo()
    {
        m_snInfos.DeleteAll();
    }

    EString     m_esModel;
    EString     m_esOwner;
    ESnInfos    m_snInfos;
};

class EMnInfos : public ETypedPtrList<EPtrList, EMnInfo*>
{
public:
    ~EMnInfos()
    {
        DeleteAll();
    }

    void DeleteAll()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EMnInfo*    pMnInfo = GetNext(pos);
            delete pMnInfo;
        }
        RemoveAll();
    }

    EMnInfo* Find(EString& esMn)
    {
        return Find(esMn.GetPtr());
    }

    EMnInfo* Find(TCHAR* pszMn)
    {
        if (pszMn == NULL)
            return NULL;

        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EMnInfo*    pMnInfo = GetNext(pos);
            if (pMnInfo->m_esModel == pszMn)
                return pMnInfo;
        }
        return NULL;

    }
};


enum DeviceStatus
{
    dsOK,               // Device is OK
    dsTIMEOUT           // Device timedout.

    // This can be expanded over time
};

// ID used to obtain the IDiscover interface.  This is a "custom" ID and will not
// be used by the Connector / Gateway as a inter-component IID.
#define  IID_DISCOVER              IID_KERNEL_CUSTOM + 1

//**************************************************************************
// Generic IDiscover interface.
// Auto-discovery requires the implementation to return the set of discovered
// devices per model number and all of the serial numbers within a given 
// classifation (category or "model number").
interface IDiscover
{
    // The caller passes in a EMnInfos list, which the callee fills.
    virtual KERESULT GetMnSnInfo(/* out */ EMnInfos& rMnInfos) = 0;

    // Using the config string returned by GetMnSnInfo(), check the status of the
    // device.  The 'timeout' is in milliseconds.
    // This is a *** SYNCHRONOUS *** function which during a timeout will stay in this 
    // routine until the "timeout" parameter elapses.
virtual KERESULT GetDeviceStatus(/* in */ TCHAR* pszConfig, /* in */ DWORD timeout,
        /* out */ DeviceStatus* pStatus) = 0;

    // Query a discoverer about its list of configured IP address, i.e. addresses,
    // which it uses when tries to discover devices
    virtual KERESULT GetDiscoveryUnicastAddrs(EStringArray *pArray) = 0;

    // Same as GetMnSnInfo, except no state/object is changed besides rMnInfos
    virtual KERESULT GetMnSnInfoReentrant(/* out */ EMnInfos& rMnInfos) = 0;
};

//**************************************************************************
interface ISNMPDiscoveryConfig
{
    virtual KERESULT GetCommunities(EStringArray& communities) = 0;
};


//**************************************************************************
interface IDeviceStatusListener
{
    // Notifiy listener when a speicific devices goes on-line or off-line
    virtual KERESULT OnConnectionChange(EString& esMn, EString& esSn, bool bOnline) = 0;
};

//**************************************************************************
// This interface should be available on any IEDDManager interface where the
// EDD Driver can manage the dynamic status of its managed devices.  
// For example, this is true of Auto-discovery using SNMP.
interface IDeviceStatus
{
    virtual KERESULT    Subscribe(IDeviceStatusListener* pIListener) = 0;
    virtual KERESULT    Unsubscribe(IDeviceStatusListener* pIListener) = 0;
    // Set rate in seconds for frequency the driver checks if an off-line device
    // is back on-line yet.
    virtual KERESULT    SetOnlineScanRate(long scanRate) = 0;
};


#endif
