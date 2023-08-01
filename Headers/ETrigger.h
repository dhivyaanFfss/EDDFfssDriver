//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//*****************************************************************************
//
//  Filename   :  ETrigger.h
//  
//  Subsystem  :  Kernel
//
//  Description:  User Notification Subscriber
//
//  Classes    :  ETrigger
//
//*****************************************************************************
#ifndef __ETRIGGER_H__
#define __ETRIGGER_H__

#ifdef WIN32
    #ifdef _DV_EXPORT
        #define ETRIGGER_DECL __declspec(dllexport)
    #else
        #define ETRIGGER_DECL __declspec(dllimport)
    #endif
#else
    #define ETRIGGER_DECL
#endif

// This is an encapsulation of common functionality used by components to
// allow subscription to triggers, and of being informed of events through
// that subscription.  
//
// To use this class, inherit from it in your component class, register your
// triggers with it (instead of with notify), and this takes care of the rest
//

//
// To use this class, inherit from it in your component class, register your
// triggers with it (instead of with notify), and this takes care of the rest
//
//*****************************************************************************
class ETRIGGER_DECL EListener
{
public:
    EListener(void* pL, EString& rS, EHANDLE hC)  :
      m_pListener(pL), m_rSchema(rS), m_hClient(hC), m_bRemoved(false),
      m_refCount(1)
      {};

private:
    virtual ~EListener() {ASSERT(m_refCount == 0);}

    // methods
public:
    long       AddRef() 
    {
        m_refCount++;
        return m_refCount;
    }
    void Release()
    {
        m_refCount--;
        if (m_refCount <= 0)
            delete this;
    }


    // properties
public:
    void*           m_pListener;
    EString         m_rSchema;
    EHANDLE         m_hClient;
    bool            m_bRemoved;
protected:
    long           m_refCount;
};

typedef ETypedPtrList<EPtrList, EListener*>   EListenerList;
typedef ETypedPtrMap<EMAP_STRING_TO_PTR, EString, EListenerList*> ETriggerMap;

// base class for handling the subscribe->notify paradigm
//
//*****************************************************************************
class ETRIGGER_DECL ETrigger : public ITrigger
{
public:
    ETrigger();
    virtual ~ETrigger();

public:
	// ITrigger Interface
	//
    virtual KERESULT Subscribe(
                                    ITriggerListener* pITriggerListener, 
                                    EString&        rTriggerName,
                                    EString&        rSchemaName,
                                    EHANDLE         hClient,
                                    HSUBSCRIPTION*  phSubscription
                                );

    virtual KERESULT Unsubscribe(HSUBSCRIPTION hSubscription);

#ifdef __GATEWAY__
    // *** Important note:  ***
    // This function is often overridden by a derived class.
    // This function often simultaneously implements the IAction's GetGatewayType
    // as well as the ITrigger's
    virtual KERESULT GetGatewayType(EString& esTrigger, GatewayType* pGatewayType)
    {
        if (pGatewayType == NULL)
            return  KE_INVALID_PARAMS;
        *pGatewayType = DEVICE_LEVEL;
        return KE_OK;
    }
#endif

	// ETrigger functions
	//
    // Generic Subscribe
    virtual KERESULT Subscribe(
                                    void*           pITriggerListener, 
                                    EString&        rTriggerName,
                                    EString&        rSchemaName,
                                    EHANDLE         hClient,
                                    HSUBSCRIPTION*  phSubscription
                                );


    virtual KERESULT Unsubscribe(HSUBSCRIPTION hSubscription, EString& rTrigger, EString& rSchema);

    virtual KERESULT FullyUnsubscribe(void);

    virtual KERESULT InformListeners(
                                    TCHAR*          trigger,
                                    TCHAR*          schema,
                                    DWORD           nData,
                                    void*           pData
                                );

    virtual KERESULT InformOneListener(
                                    void* pListener,
                                    EHANDLE hClient,
                                    DWORD nData,
                                    void* pData
                                );

	virtual bool     SubscribersFor(
                                    TCHAR*          trigger,
                                    TCHAR*          schema
							      );

    virtual KERESULT FindListener(
                                    void*          pUserListener,
                                    EListener*&    pListener
                                 );

    virtual KERESULT RegisterTrigger(TCHAR* pszTriggerName);

protected:
    // notify component interface
    INotifyMgr*     m_pINotify;

    //ETriggerMap     m_TriggerMap;

    // map of trigger name vs.
    EMapStringToPtr m_TriggerMap;
    EMutex          m_SubEx;

    // number of listeners unsubcribed to while in an inform loop
    int             m_ListenersRemoved;
};
 
// class for handling the subscribe->notify paradigm via ENotify component
//
//*****************************************************************************
class ETRIGGER_DECL ENotifyTrigger : public ETrigger
{
public:
    virtual inline KERESULT InformOneListener(
                                    void* pListener,
                                    EHANDLE hClient,
                                    DWORD nData,
                                    void* pData
                                )
    {
        ITriggerListener* pIListener = (ITriggerListener*)pListener;

        pIListener->OnEvent(hClient, nData, (EONEVENTSTRUCT*)pData);
        return KE_OK;
    }

    virtual KERESULT RegisterTrigger(TCHAR* pszTriggerName, INotifyMgr* pINotify);
};

#endif // __ETRIGGER_H__



