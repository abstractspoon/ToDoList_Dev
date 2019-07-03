// DateHelper.h: interface for the CDateHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATEHELPER_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
#define AFX_DATEHELPER_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

enum DH_DATE
{
	DHD_BEGINTHISWEEK, // DHD_ENDTHIDWEEK - 6
	DHD_TODAY,
	DHD_TOMORROW,
	DHD_ENDTHISWEEK,   
	DHD_ENDNEXTWEEK,   // DHD_ENDTHISWEEK + 7
	DHD_ENDTHISMONTH,  // beginning of next month - 1
	DHD_ENDNEXTMONTH,  // gets trickier :)
	DHD_ENDTHISYEAR,
	DHD_ENDNEXTYEAR,
	DHD_YESTERDAY,
	DHD_NOW,
	DHD_BEGINTHISMONTH, 
	DHD_BEGINTHISYEAR, 
};

enum DH_UNITS
{
	DHU_NULL	= 0,	// error code
	DHU_WEEKDAYS= 'd',
	DHU_DAYS	= 'D',
	DHU_WEEKS	= 'W',
	DHU_MONTHS	= 'M',
	DHU_YEARS	= 'Y',
};

enum
{
	DHFD_ISO		= 0x0001,
	DHFD_DOW		= 0x0002,
	DHFD_TIME		= 0x0004,
	DHFD_NOSEC		= 0x0008,
	DHFD_NOYEAR		= 0x0010,
	DHFD_NODAY		= 0x0020,
	DHFD_NOCENTURY	= 0x0040,
};

enum DH_DAYOFWEEK // Days of week bit flags
{
	DHW_NONE		= 0X00,

	DHW_SUNDAY		= 0X01,
	DHW_MONDAY		= 0X02,
	DHW_TUESDAY		= 0X04,
	DHW_WEDNESDAY	= 0X08,
	DHW_THURSDAY	= 0X10,
	DHW_FRIDAY		= 0X20,
	DHW_SATURDAY	= 0X40,

	DHW_EVERYDAY	= 0x7F
};

enum OLE_DAYOFWEEK // OLE Days of week
{
	DHO_UNDEF	= -1,

	DHO_SUNDAY	= 1,
	DHO_MONDAY,
	DHO_TUESDAY,
	DHO_WEDNESDAY,
	DHO_THURSDAY,
	DHO_FRIDAY,
	DHO_SATURDAY,
};

static OLE_DAYOFWEEK OLE_DAYSOFWEEK[7] = 
{
	DHO_SUNDAY,
	DHO_MONDAY,
	DHO_TUESDAY,
	DHO_WEDNESDAY,
	DHO_THURSDAY,
	DHO_FRIDAY,
	DHO_SATURDAY,
};

enum // Compare Flags
{
	DHC_COMPARETIME			= 0x01,
	DHC_COMPARESECONDS		= 0x02, // ignored without DHC_COMPARETIME
	DHC_NOTIMEISENDOFDAY	= 0x04, // ignored without DHC_COMPARETIME
};

//////////////////////////////////////////////////////////////////////

typedef __int64 time64_t;

//////////////////////////////////////////////////////////////////////

class COleDateTimeRange
{
public:
	COleDateTimeRange();
	COleDateTimeRange(const COleDateTimeRange& dtRange);
	COleDateTimeRange(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive = TRUE);
	COleDateTimeRange(DH_DATE nStart, DH_DATE nEnd, BOOL bInclusive = TRUE);
	COleDateTimeRange(const COleDateTime& dtStart, DH_DATE nEnd, BOOL bInclusive = TRUE);
	COleDateTimeRange(const COleDateTime& dtStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive = TRUE);
	COleDateTimeRange(DH_DATE nStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive = TRUE);

	COleDateTimeRange& operator=(const COleDateTimeRange& wi);
	BOOL operator==(const COleDateTimeRange& wi) const;

	void Reset();

	BOOL Set(const COleDateTime& dtStart, const COleDateTime& dtEnd, BOOL bInclusive = TRUE);
	BOOL Set(DH_DATE nStart, DH_DATE nEnd, BOOL bInclusive = TRUE);
	BOOL Set(const COleDateTime& dtStart, DH_DATE nEnd, BOOL bInclusive = TRUE);
	BOOL Set(const COleDateTime& dtStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive = TRUE);
	BOOL Set(DH_DATE nStart, int nEndOffset, DH_UNITS nOffsetUnits, BOOL bInclusive = TRUE);

	BOOL Contains(const COleDateTime& date, BOOL bInclusive = TRUE) const;
	BOOL Contains(const COleDateTimeRange& dtOther) const;
	BOOL IntersectsWith(const COleDateTimeRange& dtOther) const;
	BOOL GetIntersection(const COleDateTimeRange& dtOther1, const COleDateTimeRange& dtOther2);

	BOOL Add(const COleDateTimeRange& dtOther1, const COleDateTimeRange& dtOther2);
	BOOL Add(const COleDateTimeRange& dtOther, const COleDateTime& date, BOOL bInclusive = TRUE);
	BOOL Add(const COleDateTimeRange& dtOther);
	BOOL Add(const COleDateTime& date, BOOL bInclusive = TRUE);

	BOOL IsNull() const;
	BOOL IsValid() const;
	BOOL HasStart() const;
	BOOL HasEnd() const;
	COleDateTime GetStart() const;
	COleDateTime GetEnd() const;

	// returns 'end of day' if m_bInclusive is TRUE and date has no time
	COleDateTime GetEndInclusive() const; 

	int GetDayCount() const;
	int GetWeekdayCount() const;

	BOOL Offset(int nAmount, DH_UNITS nUnits);
	BOOL OffsetStart(int nAmount, DH_UNITS nUnits);
	BOOL OffsetEnd(int nAmount, DH_UNITS nUnits);

	CString Format(DWORD dwFlags = 0, TCHAR cDelim = '-') const;

	// returns 'end of day' if bInclusive is TRUE and date has no time
	static COleDateTime GetEndInclusive(const COleDateTime& date, BOOL bInclusive);

public:
	COleDateTime m_dtStart, m_dtEnd;
	BOOL m_bInclusive;
};

//////////////////////////////////////////////////////////////////////

class CDateHelper  
{
// Note: 
// 1 <= nMonth <= 12
// 1 <= nDay <= 31
// 1 <= nDOW <= 7

public:
	static BOOL IsDateSet(const COleDateTime& date);
	static void ClearDate(COleDateTime& date);
	static COleDateTime NullDate();

	static BOOL IsValidDayInMonth(int nDay, int nMonth, int nYear);
	static BOOL IsValidDayOfMonth(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth);

	static int CalcDaysFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive, BOOL bWeekdays);
	static int CalcDaysFromTo(const COleDateTime& dateFrom, DH_DATE nTo, BOOL bInclusive, BOOL bWeekdays);
	static int CalcDaysFromTo(DH_DATE nFrom, DH_DATE nTo, BOOL bInclusive, BOOL bWeekdays);

	static double GetDate(DH_DATE nDate); // 12am
	static BOOL OffsetDate(COleDateTime& date, int nAmount, DH_UNITS nUnits);

	static CString FormatDate(const COleDateTime& date, DWORD dwFlags = 0, TCHAR cDateTimeSep = ' ');
	static CString FormatCurrentDate(DWORD dwFlags = 0);
	
	// DOW = 'day of week'
	static BOOL FormatDate(const COleDateTime& date, DWORD dwFlags, CString& sDate, CString& sTime, CString& sDow);
	static BOOL FormatCurrentDate(DWORD dwFlags, CString& sDate, CString& sTime, CString& sDow);

	static BOOL DecodeDate(const CString& sDate, COleDateTime& date, BOOL bAndTime);
	static BOOL DecodeDate(const CString& sDate, double& date, BOOL bAndTime);
#if _MSC_VER < 1400
	static BOOL DecodeDate(const CString& sDate, time_t& date, BOOL bAndTime);
#endif
	static BOOL DecodeDate(const CString& sDate, time64_t& date, BOOL bAndTime);
	static BOOL DecodeISODate(const CString& sDate, COleDateTime& date, BOOL bAndTime);

	static BOOL DecodeOffset(LPCTSTR szDate, double& dAmount, DH_UNITS& nUnits, BOOL bMustHaveSign = TRUE);
	static BOOL DecodeRelativeDate(LPCTSTR szDate, COleDateTime& date, BOOL bForceWeekday, BOOL bMustHaveSign = TRUE);
	static BOOL IsValidRelativeDate(LPCTSTR szDate, BOOL bMustHaveSign = TRUE);

	static OLE_DAYOFWEEK GetDayOfWeek(const COleDateTime& date);
	static OLE_DAYOFWEEK GetDayOfWeek(const SYSTEMTIME& date);
	static OLE_DAYOFWEEK GetFirstDayOfWeek();
	static OLE_DAYOFWEEK GetLastDayOfWeek();
	static OLE_DAYOFWEEK GetNextDayOfWeek(OLE_DAYOFWEEK nDOW);
	static int GetDaysInMonth(int nMonth, int nYear); 
	static int GetDaysInMonth(const COleDateTime& date); 
	static int GetWeekofYear(const COleDateTime& date);
	static COleDateTime GetEndOfPreviousDay(const COleDateTime& date);
	static COleDateTime GetEndOfDay(const COleDateTime& date);
	static COleDateTime GetStartOfNextDay(const COleDateTime& date);
	static COleDateTime GetStartOfDay(const COleDateTime& date);

	static void GetNextMonth(int& nMonth, int& nYear, BOOL bNext = TRUE);
	static void IncrementMonth(int& nMonth, int& nYear, int nBy = 1);
	static void IncrementMonth(SYSTEMTIME& st, int nBy = 1);
	static void IncrementMonth(COleDateTime& date, int nBy = 1);
	
	static int CalcMonthsFromTo(const COleDateTime& dateFrom, const COleDateTime& dateTo, BOOL bInclusive);
	static int GetDateInMonths(int nMonth, int nYear);
	static int GetDateInMonths(const COleDateTime& date);

	static COleDateTime CalcDate(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth, int nYear);
	static int CalcDayOfMonth(OLE_DAYOFWEEK nDOW, int nWhich, int nMonth, int nYear);

	static CString GetDayOfWeekName(OLE_DAYOFWEEK nDOW, BOOL bShort = FALSE); // 1-7, sun-sat
	static CString GetMonthName(int nMonth, BOOL bShort = FALSE); // 1-12, jan-nov
	static void GetDayOfWeekNames(BOOL bShort, CStringArray& aNames); // sun-sat
	static void GetMonthNames(BOOL bShort, CStringArray& aMonths); // jan-dec
	static int CalcLongestDayOfWeekName(CDC* pDC, BOOL bShort = FALSE);

	static BOOL IsLeapYear(const COleDateTime& date = COleDateTime::GetCurrentTime());
	static BOOL IsLeapYear(int nYear);
	static BOOL IsToday(const COleDateTime& date);
	static BOOL IsSameDay(const COleDateTime& date1, const COleDateTime& date2);
	static BOOL IsEndOfDay(const COleDateTime& date);

	static void SplitDate(const COleDateTime& date, double& dDateOnly, double& dTimeOnly);
	static COleDateTime MakeDate(const COleDateTime& dtDateOnly, const COleDateTime& dtTimeOnly);

	static BOOL DateHasTime(const COleDateTime& date);
	static COleDateTime GetTimeOnly(const COleDateTime& date);
	static COleDateTime GetDateOnly(const COleDateTime& date);

	static COleDateTime GetNextAvailableDay(const COleDateTime& date, DWORD dwAvailDays);
	static BOOL ValidateDay(COleDateTime& date, DWORD dwAvailDays);

public:
	static COleDateTime GetStartOfWeek(const COleDateTime& date);
	static COleDateTime GetEndOfWeek(const COleDateTime& date);
	static COleDateTime GetStartOfMonth(const COleDateTime& date);
	static COleDateTime GetEndOfMonth(const COleDateTime& date);
	static COleDateTime GetStartOfQuarter(const COleDateTime& date);
	static COleDateTime GetEndOfQuarter(const COleDateTime& date);
	static COleDateTime GetStartOfYear(const COleDateTime& date);
	static COleDateTime GetEndOfYear(const COleDateTime& date);
	static COleDateTime GetStartOfDecade(const COleDateTime& date, BOOL bZeroBased = TRUE);
	static COleDateTime GetEndOfDecade(const COleDateTime& date, BOOL bZeroBased = TRUE);
	static COleDateTime GetStartOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased = TRUE);
	static COleDateTime GetEndOfQuarterCentury(const COleDateTime& date, BOOL bZeroBased = TRUE);

	static BOOL GetTimeT(const COleDateTime& date, time_t& timeT);
	static BOOL GetTimeT64(const COleDateTime& date, time64_t& timeT);
	static COleDateTime GetDate(time64_t date);
	static COleDateTime GetDate(double date, COleDateTime::DateTimeStatus status);

	static COleDateTime GetNearestQuarterCentury(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased = TRUE);
	static COleDateTime GetNearestDecade(const COleDateTime& date, BOOL bEnd, BOOL bZeroBased = TRUE);
	static COleDateTime GetNearestYear(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestHalfYear(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestQuarter(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestMonth(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestWeek(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestDay(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestHalfDay(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestHour(const COleDateTime& date, BOOL bEnd);
	static COleDateTime GetNearestHalfHour(const COleDateTime& date, BOOL bEnd);

	static BOOL Min(COleDateTime& date, const COleDateTime& dtOther);
	static BOOL Max(COleDateTime& date, const COleDateTime& dtOther);

	static DH_DAYOFWEEK Map(OLE_DAYOFWEEK nDOW);
	static OLE_DAYOFWEEK Map(DH_DAYOFWEEK nDOW);

	static int Compare(const COleDateTime& date1, const COleDateTime& date2, DWORD dwCompareFlags = DHC_COMPARETIME);

protected:
	static int GetISODayOfWeek(const COleDateTime& date); // 1-7 Mon-Sun
	static BOOL DecodeLocalShortDate(const CString& sDate, COleDateTime& date);
	static BOOL GetTimeT(time64_t date, time_t& timeT);
	static BOOL IsValidUnit(TCHAR nUnits);
	static BOOL DecodeOffsetEx(LPCTSTR szDate, double& dAmount, DH_UNITS& nUnits, DH_UNITS nDefUnits, BOOL bMustHaveSign);
	static COleDateTime GetNearestDayPart(const COleDateTime& date, int nNumParts, BOOL bEnd);
};

#endif // !defined(AFX_DATEHELPER_H__2A4E63F6_A106_4295_BCBA_06D03CD67AE7__INCLUDED_)
