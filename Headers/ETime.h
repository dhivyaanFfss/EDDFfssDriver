//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  ETime.h
//  
//  Subsystem  :  Kernel
//
//  Description:  Common Time classes
//
//  Classes:    ETime
//
//**************************************************************************
#ifndef __ETIME_H__
#define __ETIME_H__

class EString;

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

/////////// XML file for Local Time Settings ////////////////////////////////
#define LOCAL_TIME_FILE_NAME    _T("ELocalTime")
#define XML_CUR_VERSION			1


/////////////////////////////////////////////////////////////////////////////
//
// ETimeStamp class : This is the class used to keep all Connector time stamps.
//
/////////////////////////////////////////////////////////////////////////////
class _DV_DECLSPEC ETimeStamp
{
public:
	ETimeStamp() {m_Seconds = 0; m_MilliSeconds = 0;}
	time_t		m_Seconds;			// seconds since 1970-1-1 UTC
	short		m_MilliSeconds;		// million seconds 0 - 999;
};

/////////////////////////////////////////////////////////////////////////////
//
// ETime class 
//
/////////////////////////////////////////////////////////////////////////////
class EFileIO;
class _DV_DECLSPEC ETime
{
public:
    // Default constructor, set time to current time in UTC with Connector adjustment
    ETime();

    // Set time value to TimeStamp (TimeStamp is expressed in UTC with Connector adjustment)
    ETime(const ETimeStamp& TimeStamp);

    // Reset an ETime's time value;
    void SetTime(time_t timeVal);
    void SetTime(ETimeStamp& timeStamp);
    void SetTime(struct tm& tmValue, bool isLocal);

	// One time init member
	static void Initialize();

    static void SetTimeOffset(long secondsOffset);
    static void SetTimeZoneOffset(int timezoneOffset);

    // Set Connetor's time offset from the Actual time. 
    static void SetTimeOffsetFromActual(long secondsGMTFrom1970);

    // Set Connector's time zone (-300 is Eastern Time)
    static bool SetTimeZone(int minutesFromGMT, int DSToffset, struct tm&, struct tm&);

    // Get time in ISO-8601 Format
    static void GetISO8601Time(EString& rISOTime, ETimeStamp& TimeStamp, bool bEnterprise = true, bool bMiliseconds = true);

    // Parse IS0 8601 timestamp string
    static bool ParseISO8601Time(TCHAR *pszISOTime, ETimeStamp& timeStamp);
    
    static void GetMillisecondsUTC(/* out */ EString& resTime, /* in */ ETimeStamp* pTimeStamp = NULL);

	// Get seconds since 1970 for the ETimeStamp
    static double GetSecondsSince1970(const ETimeStamp* pTimeStamp = NULL);

	// Get Connector's time stamp. This member returns ETimeStamp in UTC with Connector's time adjustment 
    static void GetTimeStamp(ETimeStamp* pTimeStamp);

	//Helper fn for wce_mktime
	static time_t GetSecondsFromTm( struct tm* ptm);

    // Get the time zone connector is using
    static int GetTimeZoneMinutes()     {return m_timeZoneConnectorMins;}

    // Get the time zone of the system
    static int GetSystemZoneMinutes()   {return m_timeZoneSystemMins;}

	// compare times. return 1, 0, or, -1
    static int	CompareTimeStamp(const ETimeStamp* pone, const ETimeStamp* ptwo);

	// compare calendars. return 1, 0, or -1
	static int	CompareCalendar(struct tm* pOne, struct tm* pTwo);

    // save and restore to/from file
    static bool Save(EFileIO* pDataFile, ETimeStamp&);
    static bool Restore(EFileIO* pDataFile, ETimeStamp&);

	// compare times. return 1, 0, or, -1
	int			CompareTime(ETime &etime) const;

	// DiffTime() return differences in seconds
	double		DiffTime(const ETime& eTime);

	// Add months to ETime object
	void		AddCalendarMonths(int months);

	// Get the name of weekday and month. Local or UTC
	void	GetDayOfWeek(TCHAR* szDay, bool bLocalTime);
	void	GetMonth(TCHAR* szMonth, bool bLocalTime);

	// Get Calendar date and time. Local or UTC
    int		GetYear(bool bLocalTime);
    int		GetMonth(bool bLocalTime);				
    int		GetDay(bool bLocalTime);					
    int		GetHour(bool bLocalTime);
    int		GetMinute(bool bLocalTime);
    int		GetSecond(bool bLocalTime);
    int		GetMilliSecond(bool bLocalTime);
    int		GetDayOfWeek(bool bLocalTime);	

	bool	GetCalendar(bool bLocalTime, struct tm* pCalendar);
	time_t	GetTime()	{return m_TimeSTampUTC.m_Seconds;}
	ETimeStamp TimeStamp() {return m_TimeSTampUTC;}
	void	SetNull() {m_CalendarTimeType = CALENDAR_TIME_INVALID;
						m_TimeSTampUTC.m_Seconds = 0; m_TimeSTampUTC.m_MilliSeconds = 0;}

    bool    IsNull()
    {
        return (m_CalendarTimeType == CALENDAR_TIME_INVALID && 
            m_TimeSTampUTC.m_Seconds == 0 && m_TimeSTampUTC.m_MilliSeconds == 0);
    }

    // To be used by Timer Component only. 
    void    AddRelativeTime(long secondsOffset, short millisecondOffset = 0);
	static void RestoreLocalTime(EHANDLE hPersist);
	static void SaveLocalTime(EHANDLE hPersist);
	static void GetOSTime(ETimeStamp*);

    // returns month number (0-11) by name
    static int GetMonthByName(const EString& esMonth);

private:
	// Computes the Calendar date and time according to ETimeStamp
	void		ComputeCalendar(bool bLocalTime);
	void		SpringDSTAhead(time_t);

	// Get calendar time
	static bool	GetLocalTime(time_t seconds, struct tm* pLocal);
	static bool	GetUTCTime(time_t seconds, struct tm* pUTC);

    // Returns time zone offset for the specified time.
    static int GetSystemZoneMinutes(time_t seconds);

	// return timezone offset from UTC based on Standard Time
	static int GetSystemStandardTimeZoneMinutes();

	// This field controls the value type of m_Calendar field
	enum CALENDAR_TIME_TYPE {CALENDAR_TIME_INVALID, CALENDAR_TIME_UTC, CALENDAR_TIME_LOCAL};
	CALENDAR_TIME_TYPE	m_CalendarTimeType;	
    ETimeStamp			m_TimeSTampUTC;
	struct tm			m_Calendar;

	// For connector time offset
	static long			m_timeOffsetSecs;
	static int			m_timeZoneSystemMins;
	static int			m_timeZoneConnectorMins;
	static bool			m_bUseConnectorLocalTime;
	static int			m_DSTOffset;
	static struct tm	m_DSTStart;
	static struct tm	m_DSTEnd;
};

// Return Year in nnnn
inline int ETime::GetYear(bool bLocalTime)		{ComputeCalendar(bLocalTime); return m_Calendar.tm_year + 1900;}

// Return 1 - 12
inline int ETime::GetMonth(bool bLocalTime)		{ComputeCalendar(bLocalTime); return m_Calendar.tm_mon + 1;}

// Return 1- 31
inline int ETime::GetDay(bool bLocalTime)		{ComputeCalendar(bLocalTime); return m_Calendar.tm_mday;}

// Return 0 - 23
inline int ETime::GetHour(bool bLocalTime)		{ComputeCalendar(bLocalTime); return m_Calendar.tm_hour;}

// Return 0 - 59
inline int ETime::GetMinute(bool bLocalTime)	{ComputeCalendar(bLocalTime); return m_Calendar.tm_min;}

// Return 0 - 59
inline int ETime::GetSecond(bool bLocalTime)	{ComputeCalendar(bLocalTime); return m_Calendar.tm_sec;}

// Return 0 - 999
inline int ETime::GetMilliSecond(bool bLocalTime){ComputeCalendar(bLocalTime); return m_TimeSTampUTC.m_MilliSeconds;}

// Return 0 - 6
inline int ETime::GetDayOfWeek(bool bLocalTime) {ComputeCalendar(bLocalTime); return m_Calendar.tm_wday;}

inline int ETime::CompareTime(ETime &etime) const 
	{return CompareTimeStamp(&m_TimeSTampUTC, &etime.m_TimeSTampUTC);}

inline double ETime::DiffTime(const ETime& eTime)
	{return GetSecondsSince1970(&m_TimeSTampUTC) - GetSecondsSince1970(&eTime.m_TimeSTampUTC);}

#endif // __ETIME_H__
