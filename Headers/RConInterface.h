//**************************************************************************
//
//  Copyright © 2002-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  RConInterface.h
//
//  Subsystem  :  RCon
//
//  Description:  Provides an interface to allow the transfer of data
//                across an identified socket port on a remote interface
//                to the DRM Enterprise Server
//
//  Interfaces:
//			      IRConInterface
//
//**************************************************************************

#ifndef __RCONINTERFACE_H__
#define __RCONINTERFACE_H__


#ifdef WIN32
    #ifdef ERCON_EXPORTS
       #define _ERCON_DECLSPEC __declspec(dllexport)
       #define _ERCON_EXTERN
    #else
       #define _ERCON_DECLSPEC __declspec(dllimport)
       #define _ERCON_EXTERN   extern
    #endif
#else
   #define _ERCON_DECLSPEC
   #define _ERCON_EXTERN
#endif

#define DEFAULT_DSC_PORT  17001
#define DEFAULT_DSC_SSL_PORT  17002

enum InterfaceType
{
    typeUnknown,
    typeNetwork
};

//**************************************************************************
enum enPortType
{
    PortListen,
    PortConnect
};

struct stPort
{
    stPort() {m_type = PortConnect;};
    stPort(stPort* port) {m_esPort = port->m_esPort; m_esPortEnd = port->m_esPortEnd; m_type = port->m_type;};
    EString m_esPort;
    enPortType m_type;

    // SUPPORT PORT RANGE
    EString m_esPortEnd;
    bool InRange(const int iPort)
    {
        int start = _tcstol(m_esPort.GetPtr(), NULL, 10);
        int end = _tcstol(m_esPortEnd.GetPtr(), NULL, 10);
        return ((iPort == start) || (start <= iPort && iPort <= end));
    }
};

#ifdef WIN32
_ERCON_EXTERN template class _ERCON_DECLSPEC _ETypedPtrList<EPtrList, stPort*>;
_ERCON_EXTERN template class _ERCON_DECLSPEC ETypedPtrList<EPtrList, stPort*>;
#endif // WIN32
class _ERCON_DECLSPEC EPortList : public ETypedPtrList<EPtrList, stPort*>
{
public:
    ~EPortList();
	bool InList(const EString& esPort);
    void DeleteAll(void);
};

//**************************************************************************
class _ERCON_DECLSPEC ECLIInterface
{
public:
    ECLIInterface();
    ~ECLIInterface();

    EString m_esName;
    EString m_esHostName;
    EPortList m_PortList;
    EString m_esComment;
    InterfaceType   m_IntType;
	EString	m_esLaunchType;
    bool m_bIsAuditEnable;
    bool m_bIsDynamicIP;
    bool m_bBrowserSecure;
    EString m_esBrowserAbsPath;
};

//**************************************************************************
class _ERCON_DECLSPEC ERConPackageAttributes
{
public:
    ERConPackageAttributes()
    {
        m_iPort = -1;
        m_iSSLPort = -1;
		m_iDSCPort = DEFAULT_DSC_PORT;
		m_iDSCSSLPort = DEFAULT_DSC_SSL_PORT;
    }
    ERConPackageAttributes(ERConPackageAttributes& rAttributes)
    {
        *this = rAttributes;
    }
    ERConPackageAttributes& operator=(const ERConPackageAttributes& rAttributes)
    {
        m_esInterfaceName = rAttributes.m_esInterfaceName;
        m_esSessionId = rAttributes.m_esSessionId;
        m_esTimeout = rAttributes.m_esTimeout;
        m_esMn = rAttributes.m_esMn;
        m_esSn = rAttributes.m_esSn;
        m_esGetURL = rAttributes.m_esGetURL;
        m_esPostURL = rAttributes.m_esPostURL;
        m_esServer = rAttributes.m_esServer;
        m_esPath = rAttributes.m_esPath;
		m_esMergeId = rAttributes.m_esMergeId;
		m_esUserId = rAttributes.m_esUserId;
        m_iPort = rAttributes.m_iPort;
        m_iSSLPort = rAttributes.m_iSSLPort;
		m_iDSCPort = rAttributes.m_iDSCPort;
		m_iDSCSSLPort = rAttributes.m_iDSCSSLPort;
        m_esAuthMethod = rAttributes.m_esAuthMethod;
        m_esAuthUserId =  rAttributes.m_esAuthUserId;
        m_esCmdId =  rAttributes.m_esCmdId;
        m_esEnterpriseId = rAttributes.m_esEnterpriseId;
        m_esX509Subject = rAttributes.m_esX509Subject;
        m_esX509Issuer = rAttributes.m_esX509Issuer;		
        return *this;
    }

    // properties
    EString     m_esInterfaceName;
    EString     m_esSessionId;
    EString     m_esTimeout;
    EString     m_esMn;
    EString     m_esSn;
    EString     m_esPostURL;
    EString     m_esGetURL;
    EString     m_esServer;
    EString     m_esPath;
	EString		m_esMergeId;
	EString		m_esUserId;
    int         m_iPort;
    int         m_iSSLPort;
	int			m_iDSCPort;
	int			m_iDSCSSLPort;
    EString     m_esAuthMethod;
    EString     m_esAuthUserId;
    EString     m_esCmdId;
    EString     m_esEnterpriseId;
    EString     m_esX509Subject;
    EString     m_esX509Issuer;	
};

//**************************************************************************
interface   IRConInterface
{
public:
    virtual KERESULT    StartSession(int iDeviceId,
                                     ECLIInterface* pCliInterface,
									 const ERConPackageAttributes& attrs) = 0;
    virtual KERESULT    MergeSession(const EString& esSessionId,
                                     ECLIInterface* pCliInterface,
                                     const EString& esMergeId ) = 0;
    virtual KERESULT    StopSession(LPCTSTR szSessionID) = 0;
    virtual KERESULT    StopAllSessions(void) = 0;
};


#endif  //  __RCONINTERFACE_H__
