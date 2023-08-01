#ifndef _E_WS_HELPER_H_
#define _E_WS_HELPER_H_

#ifdef _WIN32
    #ifdef EWSHELPER_EXPORTS
        #define EWSHELPER_EXPORT __declspec(dllexport)
    #else
        #define EWSHELPER_EXPORT __declspec(dllimport)
    #endif
#else
        #define EWSHELPER_EXPORT
#endif

//**************************************************************************
class EWSHELPER_EXPORT EWSHelper
{
public:
    virtual ~EWSHelper() = 0;
    static EWSHelper *Create(const EString& esWSName, IKernelBase *pIKernelBase);
    virtual bool CustomizeEnvironment(struct soap *pSoap) = 0;
    virtual bool Setup(struct soap *pSoap, const EString& esModelNumber, const EString& esSerialNumber) = 0;
    virtual bool TearDown(struct soap *pSoap) = 0;
    virtual char *GetEndpoint(void) = 0;
    virtual char *GetModelNumber(void) = 0;
    virtual char *GetSerialNumber(void) = 0;
    virtual char *GetToken(void) = 0;
    static EString GetErrorString(struct soap *pSoap);

protected:
    EWSHelper(void);
};

//**************************************************************************
typedef EBasicAutoArray<LONG64> EWSIdArray;

#endif
