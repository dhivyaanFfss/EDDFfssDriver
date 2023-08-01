#ifndef _E_NETWORK_H_
#define _E_NETWORK_H_

#include <wchar.h>

#ifdef _WIN32
    #ifdef ENETWORK_EXPORTS
        #define ENETWORK_EXPORT __declspec(dllexport)
    #else
        #define ENETWORK_EXPORT __declspec(dllimport)
    #endif
#else
        #define ENETWORK_EXPORT
#endif

#define ENETWORK_DECLARE_IMPL(c) \
    public: c##Impl *GetImpl(void) const { return m_pImpl; } \
    protected: c##Impl *m_pImpl;

namespace ENetwork
{
    //**************************************************************************
    class SocketImpl;
    class StreamSocketImpl;
    class ServerSocketImpl;
    class SocketAddress;
    class SocketAddressImpl;
    class Endpoint;
    class EndpointImpl;
    class SocketSelectorImpl;
    
    //**************************************************************************
#ifdef _WIN32
#ifndef _WIN64
    typedef unsigned int SocketDescriptorId;
#else
    typedef unsigned __int64 SocketDescriptorId;
#endif
#else
    typedef int SocketDescriptorId;
#endif

    //**************************************************************************
    enum AddressFamily
    {
        ADDRESS_FAMILY_UNSPEC = -1,
        ADDRESS_FAMILY_INET4,
        ADDRESS_FAMILY_INET6,
        ADDRESS_FAMILY_MAX
    };

    //**************************************************************************
    class ENETWORK_EXPORT Socket
    {
    public:
        enum Type
        {
            TYPE_STREAM,
            TYPE_DATAGRAM,
            TYPE_MAX
        };

        enum Error
        {
            ERR_OK,
            ERR_UNKNOWN,
            ERR_INVALID_ARGUMENT,
            ERR_CLOSED,
            ERR_ALREADY_BOUND,
            ERR_NOT_BOUND,
            ERR_ALREADY_CONNECTED,
            ERR_CONNECTION_IN_PROGRESS,
            ERR_NOT_CONNECTED,
            ERR_RESOURCE_UNAVAILABLE,
            ERR_NO_ACCESS,
            ERR_ADDRESS_FAMILY_NOT_SUPPORTED,
            ERR_ADDRESS_IN_USE,
            ERR_ADDRESS_UNAVAILABLE,
            ERR_CONNECTION_REFUSED,
            ERR_CONNECTION_RESET_BY_PEER,
            ERR_CONNECTION_LOST,
            ERR_TIMED_OUT,
            ERR_NETWORK_UNREACHABLE,
            ERR_HOST_UNREACHABLE,
            ERR_DATAGRAM_TOO_LONG
        };

        virtual ~Socket();
        void Duplicate(Socket& x);

        bool Bind(const Endpoint& addresses);
        bool Close(void);

        bool IsBound(void) const;
        bool IsClosed(void) const;
        
        Type GetType(void) const;
        Error GetError(void) const;
        SocketDescriptorId GetDescriptorId(AddressFamily af = ADDRESS_FAMILY_UNSPEC) const;
        SocketAddress GetLocalAddress(AddressFamily af = ADDRESS_FAMILY_UNSPEC) const;

        bool Recycle(void);
        
        static Error GetLastOsError(void);
        
        static const long INFINITE_TIMEOUT;
        static const SocketDescriptorId INVALID_DESCRIPTOR_ID;

    protected:
        Socket(SocketImpl *pImpl);
    
        ENETWORK_DECLARE_IMPL(Socket)
    };

    //**************************************************************************
    class ENETWORK_EXPORT IoSocket : public Socket
    {
    public:
        virtual bool Connect(const Endpoint& dst) = 0;

        bool Receive(const void *pBuffer, int iLength, int& iReceived,
                     long iTimeout = Socket::INFINITE_TIMEOUT);
        bool Send(const void *pBuffer, int iLength, int& iSent,
                  long iTimeout = Socket::INFINITE_TIMEOUT);

        virtual bool IsConnected(void) const = 0;

        virtual bool IsReadable(void) const = 0;
        virtual void SetReadable(bool bReadable) = 0;
        virtual bool IsWritable(void) const = 0;
        virtual void SetWritable(bool bWritable) = 0;

        virtual const SocketAddress* GetRemoteAddress(void) const = 0;

    protected:
        IoSocket(SocketImpl *pImpl);
    };

    //**************************************************************************
    class ENETWORK_EXPORT StreamSocket : public IoSocket
    {
    public:
        StreamSocket(void);

        bool Connect(const Endpoint& dst);
        bool Connect(const Endpoint& dst, long iTimeout);

        bool ReceiveAll(const void *pBuffer, int iLength,
                       long iTimeout = Socket::INFINITE_TIMEOUT);
        bool SendAll(const void *pBuffer, int iLength,
                     long iTimeout = Socket::INFINITE_TIMEOUT);

        bool IsConnected(void) const;
        bool ValidateConnection(void);

        bool IsReadable(void) const;
        void SetReadable(bool bReadable);
        bool IsWritable(void) const;
        void SetWritable(bool bWritable);

        const SocketAddress* GetRemoteAddress(void) const;

        bool SetLinger(bool bOn, int iLinger);
        bool SetTcpNoDelay(bool bOn);

    protected:
        StreamSocket(StreamSocketImpl *pImpl);

        friend class ServerSocketImpl;
    };

    //**************************************************************************
    class ENETWORK_EXPORT DatagramSocket : public IoSocket
    {
    public:
        DatagramSocket(void);

        bool Connect(const Endpoint& dst);
        
        bool ReceiveFrom(const void *pBuffer, int iLength, int& iReceived,
                         Endpoint& dst,
                         long iTimeout = Socket::INFINITE_TIMEOUT);
        bool SendTo(const void *pBuffer, int iLength, int& iSent,
                    const Endpoint& dst);
        
        bool IsConnected(void) const;

        bool IsReadable(void) const;
        void SetReadable(bool bReadable);
        bool IsWritable(void) const;
        void SetWritable(bool bWritable);

        const SocketAddress* GetRemoteAddress(void) const;
    };

    //**************************************************************************
    class ENETWORK_EXPORT ServerSocket : public Socket
    {
    public:
        ServerSocket(size_t iBacklog = DEFAULT_BACKLOG);

        bool Accept(StreamSocket*& pSocket,
                    long iTimeout = Socket::INFINITE_TIMEOUT);

        bool IsAcceptable(void);

        static const size_t DEFAULT_BACKLOG;
    };

    //**************************************************************************
    class ENETWORK_EXPORT SocketAddress
    {
    public:
        SocketAddress(const SocketAddress& x);
        ~SocketAddress();

        AddressFamily GetFamily(void) const;
        Socket::Type GetSocketType(void) const;
        const char *GetHost(void) const;
        const char *GetHostAddress(size_t& iLength) const;
        const char *GetService(void) const;

    private:
        SocketAddress(SocketAddressImpl *pImpl);

        ENETWORK_DECLARE_IMPL(SocketAddress)

        friend class EndpointImpl;
        friend class SocketAddressImpl;
        friend class SocketImpl;
        friend class ServerSocketImpl;
        friend class DatagramSocketImpl;
    };

    //**************************************************************************
    class ENETWORK_EXPORT Endpoint
    {
    public:
        enum Error
        {
            ERR_OK,
            ERR_SYSTEM,
            ERR_INVALID_HOST_NAME,
            ERR_INVALID_SERVICE_NAME,
            ERR_NAME_NOT_FOUND,
            ERR_SERVICE_NOT_SUPPORTED,
            ERR_SOCKTYPE_NOT_SUPPORTED
        };

        Endpoint(void);
        ~Endpoint();

        static Error Resolve(const char *pszHostname, const char *pszService,
                             Socket::Type socketType, Endpoint& dst);
        static Error Resolve(const char *pszHostname, unsigned short iPort,
                             Socket::Type socketType, Endpoint& dst);
        static Error Resolve(const wchar_t *pwszHostname, const wchar_t *pwszService,
                             Socket::Type socketType, Endpoint& dst);
        static Error Resolve(const wchar_t *pwszHostname, unsigned short iPort,
                             Socket::Type socketType, Endpoint& dst);
        static Error CreateWildcard(const char *pszService, Socket::Type socketType,
                                    Endpoint& dst,
                                    AddressFamily af = ADDRESS_FAMILY_UNSPEC);
        static Error CreateWildcard(unsigned short iPort, Socket::Type socketType,
                                    Endpoint& dst,
                                    AddressFamily af = ADDRESS_FAMILY_UNSPEC);
        static Error CreateLoopback(const char *pszService, Socket::Type socketType,
                                    Endpoint& dst,
                                    AddressFamily af = ADDRESS_FAMILY_UNSPEC);
        static Error CreateLoopback(unsigned short iPort, Socket::Type socketType,
                                    Endpoint& dst,
                                    AddressFamily af = ADDRESS_FAMILY_UNSPEC);

        size_t GetAddressCount(void) const;
        SocketAddress* operator[](size_t i);

        ENETWORK_DECLARE_IMPL(Endpoint)
    };

    //**************************************************************************
    class ENETWORK_EXPORT SocketSelector
    {
    public:
        enum Condition
        {
            CONDITION_READABLE = 0x1,
            CONDITION_WRITABLE = 0x2,
            CONDITION_CONNECT_COMPLETE = 0x4,
            CONDITION_ACCEPTABLE = 0x8
        };

        struct TimeValue
        {
          long iSeconds;
          long iMicroSeconds;
        };

        SocketSelector(void);
        ~SocketSelector();

        bool RegisterSocket(Socket& sock, int iConditions);
        int Select(long iTimeout);
        int Select(const TimeValue *pTimeout);

        ENETWORK_DECLARE_IMPL(SocketSelector)
    };
}

#endif
