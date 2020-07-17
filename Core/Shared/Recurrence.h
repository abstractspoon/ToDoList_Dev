// CRecurrence.h: interface for the CRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
#define AFX_RECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;

/////////////////////////////////////////////////////////////////////////////

enum RECURRENCE_REGULARITY
{
	RECURS_ONCE,							// equiv to 'None'

	RECURS_DAILY,
	RECURS_DAY_EVERY_NDAYS					= RECURS_DAILY,		
	RECURS_DAY_EVERY_WEEKDAY,
	RECURS_DAY_EVERY_NWEEKDAYS,

	RECURS_WEEKLY,
	RECURS_WEEK_SPECIFIC_DOWS_NWEEKS		= RECURS_WEEKLY,	
	RECURS_WEEK_EVERY_NWEEKS,

	RECURS_MONTHLY,
	RECURS_MONTH_SPECIFIC_DAY_NMONTHS		= RECURS_MONTHLY,	
	RECURS_MONTH_SPECIFIC_DOW_NMONTHS,
	RECURS_MONTH_EVERY_NMONTHS,
	RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS,

	RECURS_YEARLY,
	RECURS_YEAR_SPECIFIC_DAY_MONTH			= RECURS_YEARLY,	
	RECURS_YEAR_SPECIFIC_DOW_MONTH,
	RECURS_YEAR_EVERY_NYEARS,
};

//////////////////////////////////////////////////////////////////////

struct CRecurrence
{
	//  nRegularity										m_dwSpecific1			m_dwSpecific2

	//	RECURS_DAY_EVERY_NDAYS							every 'n' days			--- (0)
	//	RECURS_DAY_EVERY_WEEKDAY						--- (0)					--- (0)
	//	RECURS_DAY_EVERY_NWEEKDAYS						every 'n' days			--- (0)

	//	RECURS_WEEK_SPECIFIC_DOWS_NWEEKS				every 'n' weeks			weekdays (DHW_...)
	//	RECURS_WEEK_EVERY_NWEEKS						every 'n' weeks			--- (0)

	//	RECURS_MONTH_EVERY_NMONTHS						every 'n' months		--- (0)
	//	RECURS_MONTH_SPECIFIC_DAY_NMONTHS				every 'n' months		day of month (1-31)
	//	RECURS_MONTH_FIRSTLASTWEEKDAY_NMONTHS			first(0), last(!0)		every 'n' months
	//	RECURS_MONTH_SPECIFIC_DOW_NMONTHS				LOWORD = which (1-5)	every 'n' months
	//													HIWORD = DOW (1-7)		

	//	RECURS_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
	//	RECURS_YEAR_EVERY_NYEARS						every 'n' years			--- (0)
	//  RECURS_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific month (1-12)
	//													HIWORD = DOW (1-7)		

	CRecurrence();
	CRecurrence(const CRecurrence& tr);

	BOOL operator==(const CRecurrence& tr) const;
	BOOL operator!=(const CRecurrence& tr) const;

	BOOL Matches(const CRecurrence& tr, BOOL bIncludeRemainingOccurrences) const;

	BOOL IsRecurring() const;
	BOOL CanRecur() const;
	BOOL GetNextOccurence(const COleDateTime& dtPrev, COleDateTime& dtNext); // updates remaining occurrences
	BOOL FitDayToScheme(COleDateTime& dtRecur) const;
	
	BOOL CalcNextOccurence(const COleDateTime& dtPrev, COleDateTime& dtNext) const;
	int CalcNextOccurences(const COleDateTime& dtPrev, const COleDateTimeRange& dtRange, CArray<double, double&>& aDates) const;

	BOOL SetRegularity(RECURRENCE_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
	RECURRENCE_REGULARITY GetRegularity(DWORD& dwSpecific1, DWORD& dwSpecific2) const;
	RECURRENCE_REGULARITY GetRegularity() const;
	
	void ClearOccurrenceCount();
	BOOL SetOccurrenceCount(int nNumOccur, int nRemainingOccur);
	int GetOccurrenceCount() const;	
	int GetRemainingOccurrenceCount() const;

protected:
	int m_nNumOccur, m_nRemainingOccur;
	RECURRENCE_REGULARITY m_nRegularity;
	DWORD m_dwSpecific1;
	DWORD m_dwSpecific2;

protected:
	BOOL ValidateDay(SYSTEMTIME& st) const;
	static BOOL IsValidRegularity(RECURRENCE_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
};


#endif // !defined(AFX_RECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
