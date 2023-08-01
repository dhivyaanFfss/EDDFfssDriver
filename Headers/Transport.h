//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  Transport.h
//  
//  Subsystem  :  Kernel
//
//  Description:  SOCKET wrapper class.
//
//  Classes    :  ESocket
//
//**************************************************************************
#ifndef __ETRANSPORT_H__
#define __ETRANSPORT_H__

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

/* {secret} */
#define E_TCP_SOCK  SOCK_STREAM
/* {secret} */
#define E_UDP_SOCK  SOCK_DGRAM

#define MAX_INTERFACES 16

#define LOCAL_IP_ADDRESS          _T("127.0.0.1")

//*******************************************************************
class _DV_DECLSPEC EInterfaceInfo
{
public:
    EInterfaceInfo() :
        m_bBroadcast(false),
        m_iAddress(0UL),
        m_iNetmask(0UL),
        m_iBroadcastAddress(0UL) {}

    bool    m_bBroadcast;
    EULONG32 m_iAddress;
    EULONG32 m_iNetmask;
    EULONG32 m_iBroadcastAddress;
};

#ifdef WIN32
template class _DV_DECLSPEC _ETypedPtrList<EPtrList, EInterfaceInfo*>;
template class _DV_DECLSPEC ETypedPtrList<EPtrList, EInterfaceInfo*>;
#endif
class _DV_DECLSPEC EInterfaceList : public ETypedPtrList<EPtrList, EInterfaceInfo*>
{
public:
    virtual ~EInterfaceList()
    {
        POSITION pos = GetHeadPosition();
        while (pos)
        {
            EInterfaceInfo* pInfo = GetNext(pos);
            delete pInfo;
        }
        RemoveAll();
    }
};

// group=OS Wrappers

//*******************************************************************

// Socket wrapper
class _DV_DECLSPEC ESocket
{
public:
    
    // Constructor
    ESocket(SOCKET s=INVALID_SOCKET);               

    // Destructor
    ~ESocket();                                     
  
    // Creates socket by given type
    KERESULT Create(USHORT type);  
    
    // Disconnects from remote node
    KERESULT DisconnectHard();           
    
    // Disconnects from remote node
    KERESULT Disconnect();           
    
    // Puta the socket in Listen mode
    KERESULT Listen(USHORT port); 

    // Puts the socket in listen mode on the loopback interface (not visible to LAN)
    KERESULT LoopbackListen(USHORT port);
    
    // Connects to given name
    KERESULT Connect(const sockaddr *name,int namelen);
    
    // Accept incomming connection
    KERESULT Accept(ESocket& sock, sockaddr *caller = 0,socklen_t *callerlen = 0, long timeout = -1 );
    
    // Bind socket to particular port / address
    KERESULT Bind(sockaddr_in& receiver,bool bReuse=true);
    
    // Send XDR buffer
    KERESULT Send(EXdrMessage&,long timeout = 10000);
    
    // Send charachter buffer
    KERESULT Send(const char* buf, DWORD size, int *pSent = 0,long timeout = 10000);     // Sends data via the connected socket 
    KERESULT SendQ(const char* buf, DWORD size, int *pSent = 0,long timeout = 10000);     // Sends data via the connected socket 

    // Send buffer to an IP address
    KERESULT Sendto(const char* buf, DWORD size, sockaddr_in& recpt, int *pSent = 0, long timeout = 10000);
    
    // Used by UDP sockets to send XDR buffer
    KERESULT Sendto(EXdrMessage& buf,const sockaddr *to,int tolen,long timeout = 10000);

    // Used by UDP sockets
    KERESULT Sendto(const char *buf,int len,const sockaddr *to,int tolen,long timeout = 10000);

    // Receives data from the connected socket till the buffer is filled in requered size  
    KERESULT Receive(char* buf, DWORD size, DWORD *received = 0, long timeout = 10000);   
    
    // Like Receive, but only gets what is avail, not size
    KERESULT Recv(char* buf, DWORD size, DWORD *received = 0);   
    
    // Receives data from an IP address    
    KERESULT Recvfrom(char* buf, DWORD size, sockaddr_in* source, DWORD *received = 0);   

    // Receives size data (utf8 encoded) from the connected socket, decode from UTF8 and save
    // the data in the esReceived string.
    KERESULT ReceiveUTF8(/* out */ EString& esReceived, /* in */ DWORD size, long timeout = 10000);   

    // Sets a socket option, using UNIX style arguments
    KERESULT Setsockopt(int level, int optname, const char *optval, int optlen);

    // Gets a socket option, using UNIX style arguments
    KERESULT Getsockopt(int level, int optname, const char *optval, int *optlen);

    // IOCTL for sockets, using UNIX style arguments
    KERESULT Ioctlsocket(long cmd, EULONG32 *argp);

    // Gets a socket name/address into caller
    KERESULT Getsockname(struct sockaddr *caller, socklen_t *callerlen);

    // Gets a name/address of the peer
    KERESULT Getpeername(struct sockaddr *peer, socklen_t *peerlen);

    // see if socket is readable, writable, or has errors, timeout -1 is blocking
    KERESULT Select(bool bRead, bool bWrite, bool bExc, long timeout = -1);

    // returns the local IP address of the connected socket
    char* GetSockName();

    // Checks if socket is valid
    bool     IsOk() {return (m_socket!=INVALID_SOCKET);}

    // Checks if the peer socket is closed
    bool    IsPeerClosed();

    // shutdown. (not completely disconnect)
    int ShutDown(int how);

    int GetPort() { return m_port; }

    // Operators
    void operator=(ESocket& sock)  { m_socket = sock.m_socket; }
    void operator=(SOCKET sock)   { m_socket = sock; }
    bool operator==(ESocket& sock) { return (m_socket == sock.m_socket); }
    bool operator==(SOCKET sock)  { return (m_socket == sock); }
   
    SOCKET    GetId();                               // Returns socket ID
    bool     GetCurrentIpAddress (EULONG32* address);       // Retrieves the IP address of the local host
	bool     GetCurrentIpAddress (TCHAR* szAddress);        // Retrieves the IP address of the local host (Depreciated) 
    bool     GetCurrentIpAddress (TCHAR* szAddress, unsigned int bufferSize);  // Retrieves the IP address of the local host

   
    static unsigned short HtoNS (unsigned short hostshort);
    static EULONG32       NtoHS(unsigned short data);
    static EULONG32       NtoHL(EULONG32 data);
    static EULONG32       HtoNL(EULONG32 data);
    static EULONG32       Inet_Addr (const char *cp);

    static struct hostent* GethostByName(const char* name);
    static char* Inet_ntoa(struct in_addr in );

    static int GethostName(char*name,int namelen);
    
    static DWORD GetIPFromServerName(char* pszName);

    static int Select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,const timeval *timeout);

    static int GetLastIOError(void); 
    static KERESULT GetInterfaceList(EInterfaceList *pList);

// Support Functions
protected:
    bool SystemNetInit();                 // Sockets initialization
    bool SystemNetShut();                 // Sockets shut
    void PrintErrorString(TCHAR* pszError);
    void PrintInfoString(TCHAR* pszError);

private:
    SOCKET  m_socket;                               // Socket
    int m_port;
    
    // WinSock Support
    static EMutex  m_hRefMtx;                       // Referance count mutext
    static int     m_iRefCnt;                       // Object Reference counter
    static bool    m_bInit;                       // Initialized flag
   

};



#endif //__ETRANSPORT_H__
