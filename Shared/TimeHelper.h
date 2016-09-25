// TimeHelper.h: interface for the TimeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_)
#define AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

enum TH_UNITS
{
	THU_NULL		= 0,	// error code
	THU_MINS		= 'I',
	THU_HOURS		= 'H',
	THU_WEEKDAYS	= 'K',
	THU_DAYS		= 'D',
	THU_WEEKS		= 'W',
	THU_MONTHS		= 'M',
	THU_YEARS		= 'Y',
};

enum THU_WORKDAYPERIOD
{
	THU_BEFORE,
	THU_MORNING,
	THU_LUNCH,
	THU_AFTERNOON,
	THU_AFTER,
};

class CTimeHelper  
{
public:
	CTimeHelper(); // uses statically defined hours and days
	CTimeHelper(double dHoursInWorkday, double dWorkdaysInWeek);
	
	double GetTime(double dTime, TH_UNITS nFromUnits, TH_UNITS nToUnits) const;
	CString FormatTimeHMS(double dTime, TH_UNITS nUnitsFrom, BOOL bDecPlaces = TRUE, BOOL bAllowZero = FALSE) const;
	CString FormatTime(double dTime, TH_UNITS nUnits, int nDecPlaces) const;
	CString FormatTime(double dTime, int nDecPlaces) const;

	void CalculatePartWorkdays(const COleDateTime& dtStart, const COleDateTime& dtEnd,
								double& dPartStartDay, double& dPartEndDay, BOOL bInDays = TRUE) const;

	THU_WORKDAYPERIOD GetWorkdayPeriod(const COleDateTime& date) const;
	double GetStartOfWorkday(BOOL bInDays = TRUE) const;
	double GetStartOfWorkdayLunch(BOOL bInDays = TRUE) const;
	double GetEndOfWorkday(BOOL bInDays = TRUE) const;
	double GetEndOfWorkdayLunch(BOOL bInDays = TRUE) const;

	double GetHoursInOneDay(BOOL bStatic = FALSE) const;
	double GetWeekdaysInOneWeek(BOOL bStatic = FALSE) const;

	int Compare(double dTime1, TH_UNITS nUnits1, double dTime2, TH_UNITS nUnits2) const;

public:
	static BOOL SetHoursInWorkday(double dHours);
	static BOOL SetWorkdaysInWeek(double dDays);
	static void SetUnits(TH_UNITS nUnits, LPCTSTR szUnits);
	static void SetUnits(TH_UNITS nUnits, TCHAR cUnits);
	static TCHAR GetUnits(TH_UNITS nUnits);
	static BOOL DecodeOffset(LPCTSTR szTime, double& dAmount, TH_UNITS& nUnits, BOOL bMustHaveSign = TRUE);
	static TH_UNITS DecodeUnits(TCHAR cUnits);
	static TH_UNITS DecodeUnits(LPCTSTR szUnits);

	static CString FormatClockTime(const COleDateTime& dtTime, BOOL bIncSeconds = FALSE, BOOL bISO = FALSE);
	static CString FormatClockTime(int nHour, int nMin, int nSec = 0, BOOL bIncSeconds = FALSE, BOOL bISO = FALSE);
	static double DecodeClockTime(LPCTSTR szTime); // returns 0-24
	
protected:
	double m_dHours2Workdays, m_dWorkdays2Weeks;

protected:
	double GetDaysToWeeksFactor(TH_UNITS nUnits) const;
	TH_UNITS GetDaysToWeeksUnits(TH_UNITS nUnits) const;

protected:
	// user definable pseudo-constants
	static double HOURS2WORKDAYS, WORKDAYS2WEEKS; 
	static CMap<TH_UNITS, TH_UNITS, TCHAR, TCHAR&> MAPUNIT2CH;
	
protected:
	static double GetTimeOnly(double dDate);
	static double GetTimeOnly(const COleDateTime& date);
	static BOOL Compare(TH_UNITS nFromUnits, TH_UNITS nToUnits); // 0=same, -1=nFrom < nTo else 1
	static CString FormatTimeHMS(double dTime, TH_UNITS nUnits, TH_UNITS nLeftOverUnits, 
								double dLeftOverMultiplier, BOOL bDecPlaces);
	static BOOL IsWeekdays(TH_UNITS nUnits);
	static BOOL IsValidUnit(int nUnits);
	static BOOL RemovePM(CString& sTime);
	static BOOL RemoveAM(CString& sTime);
};

#endif // !defined(AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_)
