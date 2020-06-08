// TimeHelper.h: interface for the TimeHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_)
#define AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////

#include "WorkingWeek.h"

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////

enum TH_UNITS
{
	THU_NULL		= 0,	// error code
	THU_SECONDS		= 'S',
	THU_MINS		= 'I',
	THU_HOURS		= 'H',
	THU_WEEKDAYS	= 'K',
	THU_DAYS		= 'D',
	THU_WEEKS		= 'W',
	THU_MONTHS		= 'M',
	THU_YEARS		= 'Y',
};

//////////////////////////////////////////////////////////////////////

enum THU_HMS
{
	HMS_ALLOWZERO		= 0x01,
	HMS_DECIMALPLACES	= 0x02,
	HMS_WANTSECONDS		= 0x04,
	HMS_FORMATSPACED	= 0x08
};

//////////////////////////////////////////////////////////////////////

class CTimeHelper  
{
public:
	CTimeHelper(); // uses statically defined hours and days
	CTimeHelper(const CWorkingWeek& week);
	
	double Convert(double dTime, TH_UNITS nFromUnits, TH_UNITS nToUnits) const;
	CString FormatTimeHMS(double dTime, TH_UNITS nUnitsFrom, DWORD dwFlags = HMS_DECIMALPLACES) const;
	CString FormatTime(double dTime, TH_UNITS nUnits, int nDecPlaces) const;
	CString FormatTime(double dTime, int nDecPlaces) const;

	int Compare(double dTime1, TH_UNITS nUnits1, double dTime2, TH_UNITS nUnits2) const;

	const CWorkingWeek& WorkingWeek() const { return m_week; }

public:
	// Helpers
	static void SetUnits(TH_UNITS nUnits, LPCTSTR szUnits);
	static void SetUnits(TH_UNITS nUnits, TCHAR cUnits);
	static TCHAR GetUnits(TH_UNITS nUnits);
	static BOOL DecodeOffset(LPCTSTR szTime, double& dAmount, TH_UNITS& nUnits, BOOL bMustHaveSign = TRUE);
	static TH_UNITS DecodeUnits(TCHAR cUnits);
	static TH_UNITS DecodeUnits(LPCTSTR szValueWithUnits);

	static CString FormatClockTime(const COleDateTime& dtTime, BOOL bIncSeconds = FALSE, BOOL bISO = FALSE);
	static CString FormatClockTime(int nHour, int nMin, int nSec = 0, BOOL bIncSeconds = FALSE, BOOL bISO = FALSE);
	static double DecodeClockTime(LPCTSTR szTime, BOOL bIncSeconds = FALSE); // returns 0-24
	
protected:
	CWorkingWeek m_week;

	static CMap<TH_UNITS, TH_UNITS, TCHAR, TCHAR&> MAPUNIT2CH;

protected:
	double GetDaysToWeeksFactor(TH_UNITS nUnits) const;
	TH_UNITS GetDaysToWeeksUnits(TH_UNITS nUnits) const;

	static double GetTimeOnly(double dDate);
	static double GetTimeOnly(const COleDateTime& date);
	static BOOL Compare(TH_UNITS nFromUnits, TH_UNITS nToUnits); // 0=same, -1=nFrom < nTo else 1
	static CString FormatTimeHMS(double dTime, TH_UNITS nUnits, TH_UNITS nLeftOverUnits, 
								double dLeftOverMultiplier, BOOL bDecPlaces, TCHAR cDelim);
	static CString FormatTimeHMS(int nTime, TH_UNITS nUnits);

	static BOOL IsValidUnit(TH_UNITS nUnits);
	static BOOL RemovePM(CString& sTime);
	static BOOL RemoveAM(CString& sTime);
};

#endif // !defined(AFX_TIMEHELPER_H__BA0C1E67_FAAA_4E65_8EF3_01B011ACFBBC__INCLUDED_)
