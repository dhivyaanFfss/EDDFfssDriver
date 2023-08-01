//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ISyslogClient.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Define ISyslogClient interface
//
//
//**************************************************************************

#ifndef __ISYSLOG_CLIENT_H__
#define __ISYSLOG_CLIENT_H__

//**************************************************************************
// ISyslogClient sends a syslog message to some configured syslog servers
interface ISyslogClient : public IKernelBase
{
	// For Syslog message formats, refer to RFC 3164
	// severity (0 - 7)
	typedef enum{
			severityEmergency = 0,
			severityAlert,
			severityCritical,
			severityError,
			severityWarning,
			severityNotice,
			severityInformational,
			severityDebug
	}syslogSeverity;

	// facility (0 - 23)
	typedef enum{
			facilityKernel = 0,
			facilityUserLevel,
			facilityMail,
			facilitySystemDaemons,
			facilitySecurity1,
			facilityInternal,
			facilityPrinter,
			facilityNetwork,
			facilityUUCP,
			facilityClock1,
			facilitySecurity2,
			facilityFTP,
			facilityNTP,
			facilityLogAudit,
			facilityLogAlert,
			facilityClock2,
			facilityLocal_0,
			facilityLocal_1,
			facilityLocal_2,
			facilityLocal_3,
			facilityLocal_4,
			facilityLocal_5,
			facilityLocal_6,
			facilityLocal_7
	} syslogFacility;

    // Send the message to some configured Syslog Servers
	// esSource:	identifies the source component, like AxedaAgent, etc.
	// esMessage:	the message itself
    virtual KERESULT Send(EString& esSource, EString& esMessage, 
		syslogSeverity severity, syslogFacility facility = facilityLocal_0, ETime* pTime = NULL) = 0;
};

#endif // __ISYSLOG_CLIENT_H__


