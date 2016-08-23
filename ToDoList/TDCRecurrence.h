// TdcRecurrence.h: interface for the TdcRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
#define AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"

//////////////////////////////////////////////////////////////////////

struct TDCRECURRENCE
{
	//  nRegularity										dwSpecific1				dwSpecific2
	
	//	TDIR_DAILY (TDIR_DAY_EVERY_NDAYS)
	//	TDIR_DAY_EVERY_NDAYS							every 'n' days			--- (0)
	//	TDIR_DAY_EVERY_WEEKDAY							--- (0)					--- (0)
	//	TDIR_DAY_EVERY_NWEEKDAYS						every 'n' days			--- (0)
	
	//	TDIR_WEEKLY	(TDIR_WEEK_SPECIFIC_DOWS_NWEEKS)				
	//	TDIR_WEEK_SPECIFIC_DOWS_NWEEKS					every 'n' weeks			weekdays (TDIW_...)
	//	TDIR_WEEK_EVERY_NWEEKS							every 'n' weeks			--- (0)
	
	//	TDIR_MONTHLY (TDIR_MONTH_SPECIFIC_DAY_NMONTHS)				
	//	TDIR_MONTH_EVERY_NMONTHS						every 'n' months		--- (0)
	//	TDIR_MONTH_SPECIFIC_DAY_NMONTHS					every 'n' months		day of month (1-31)
	//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS				every 'n' months		first(0), last(!0)
	//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS					LOWORD = which (1-5)	every 'n' months
	//													HIWORD = DOW (1-7)		
	
	//	TDIR_YEARLY	(TDIR_YEAR_SPECIFIC_DAY_MONTH)				
	//	TDIR_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
	//	TDIR_YEAR_EVERY_NYEARS							every 'n' years			--- (0)
	//  TDIR_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific month (1-12)
	//													HIWORD = DOW (1-7)		

	TDCRECURRENCE();

	BOOL operator==(const TDCRECURRENCE& tr) const;
	BOOL operator!=(const TDCRECURRENCE& tr) const;

	BOOL Matches(const TDCRECURRENCE& tr, BOOL bIncludeRemainingOccurrences) const;

	BOOL IsRecurring() const;
	BOOL CanRecur() const;
	BOOL GetNextOccurence(const COleDateTime& dtFrom, COleDateTime& dtNext);

	BOOL FitDayToScheme(COleDateTime& dtRecur) const;
	CString GetRegularityText(BOOL bIncOnce = TRUE) const;

	BOOL SetRegularity(TDI_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
	TDI_REGULARITY GetRegularity(DWORD& dwSpecific1, DWORD& dwSpecific2) const;
	TDI_REGULARITY GetRegularity() const;
	
	void ClearOccurrenceCount();
	BOOL SetOccurrenceCount(int nNumOccur, int nRemainingOccur);
	int GetOccurrenceCount() const;	
	int GetRemainingOccurrenceCount() const;

	static CString GetRegularityText(TDI_REGULARITY nRegularity, BOOL bIncOnce);

	TDI_RECURFROMOPTION nRecalcFrom; 
	TDI_RECURREUSEOPTION nReuse;

protected:
	int nNumOccur, nRemainingOccur;
	TDI_REGULARITY nRegularity;
	DWORD dwSpecific1;
	DWORD dwSpecific2;

protected:
	BOOL ValidateDay(SYSTEMTIME& st) const;
	static BOOL IsValidRegularity(TDI_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
};


#endif // !defined(AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
