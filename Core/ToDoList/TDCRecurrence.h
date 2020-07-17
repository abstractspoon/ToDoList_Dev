// TdcRecurrence.h: interface for the TdcRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
#define AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tdcenum.h"

#include "..\Interfaces\ITaskList.h"

#include "..\Shared\Recurrence.h"

//////////////////////////////////////////////////////////////////////

struct TDCRECURRENCE : public CRecurrence
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
	//	TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS				first(0), last(!0)		every 'n' months
	//	TDIR_MONTH_SPECIFIC_DOW_NMONTHS					LOWORD = which (1-5)	every 'n' months
	//													HIWORD = DOW (1-7)		

	//	TDIR_YEARLY	(TDIR_YEAR_SPECIFIC_DAY_MONTH)				
	//	TDIR_YEAR_SPECIFIC_DAY_MONTH					month (1-12)			day of month (1-31)
	//	TDIR_YEAR_EVERY_NYEARS							every 'n' years			--- (0)
	//  TDIR_YEAR_SPECIFIC_DOW_MONTH					LOWORD = which (1-5)	specific month (1-12)
	//													HIWORD = DOW (1-7)		

	TDCRECURRENCE();
	TDCRECURRENCE(TDC_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);

	BOOL operator==(const TDCRECURRENCE& tr) const;
	BOOL operator!=(const TDCRECURRENCE& tr) const;

	BOOL Matches(const TDCRECURRENCE& tr, BOOL bIncludeRemainingOccurrences) const;

	BOOL SetRegularity(TDC_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2);
	TDC_REGULARITY GetRegularity(DWORD& dwSpecific1, DWORD& dwSpecific2) const;
	TDC_REGULARITY GetRegularity() const;
	
	CString GetRegularityText(BOOL bIncOnce = TRUE) const;
	static CString GetRegularityText(TDC_REGULARITY nRegularity, BOOL bIncOnce);

	TDC_RECURFROMOPTION nRecalcFrom; 
	TDC_RECURREUSEOPTION nReuse;
	BOOL bPreserveComments;

};


#endif // !defined(AFX_TDCRECURRENCE_H__B253CCF3_D214_4FAA_9E5A_3013BF7E0F80__INCLUDED_)
