// TdcRecurrence.cpp: implementation of the TdcRecurrence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TdcRecurrence.h"
#include "TdcMapping.h"
#include "resource.h"

#include "..\Shared\enstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

const int OCCURS_INFINITELY = -1;	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TDCRECURRENCE::TDCRECURRENCE() 
	: 
	nRecalcFrom(TDIRO_DUEDATE), 
	nReuse(TDIRO_REUSE),
	bPreserveComments(TRUE)
{
}

TDCRECURRENCE::TDCRECURRENCE(TDC_REGULARITY nRegularity, DWORD dwSpecific1, DWORD dwSpecific2)
	: 
	nRecalcFrom(TDIRO_DUEDATE), 
	nReuse(TDIRO_REUSE),
	bPreserveComments(TRUE)
{
	VERIFY(SetRegularity(nRegularity, dwSpecific1, dwSpecific2));
}

BOOL TDCRECURRENCE::operator==(const TDCRECURRENCE& tr) const
{
	return Matches(tr, TRUE);
}

BOOL TDCRECURRENCE::operator!=(const TDCRECURRENCE& tr) const
{
	return !Matches(tr, TRUE);
}

BOOL TDCRECURRENCE::Matches(const TDCRECURRENCE& tr, BOOL bIncludeRemainingOccurrences) const
{
	BOOL bMatch = (CRecurrence::Matches(tr, bIncludeRemainingOccurrences) &&
					(tr.nRecalcFrom			== nRecalcFrom) &&
					(tr.nReuse				== nReuse) && 
					(tr.bPreserveComments	== bPreserveComments));

	return bMatch;
}

CString TDCRECURRENCE::GetRegularityText(BOOL bIncOnce) const
{
	RECURRENCE_REGULARITY nRegularity = CRecurrence::GetRegularity();

	return GetRegularityText(TDC::MapRegularityToTDCRegularity(nRegularity), bIncOnce);
}

CString TDCRECURRENCE::GetRegularityText(TDC_REGULARITY nRegularity, BOOL bIncOnce)
{
	enum { ONCE, DAILY, WEEKLY, MONTHLY, YEARLY, COUNT };
	
	static CEnString sRegularity[COUNT];
	
	switch (nRegularity)
	{
	case TDIR_DAY_EVERY_WEEKDAY:
	case TDIR_DAY_EVERY_NDAYS:   
	case TDIR_DAY_EVERY_NWEEKDAYS:   
		if (sRegularity[DAILY].IsEmpty())
			sRegularity[DAILY].LoadString(IDS_DAILY);    
		
		return sRegularity[DAILY];
		
	case TDIR_WEEK_EVERY_NWEEKS:
	case TDIR_WEEK_SPECIFIC_DOWS_NWEEKS:  
		if (sRegularity[WEEKLY].IsEmpty())
			sRegularity[WEEKLY].LoadString(IDS_WEEKLY);   
		
		return sRegularity[WEEKLY];
		
	case TDIR_MONTH_SPECIFIC_DOW_NMONTHS:
	case TDIR_MONTH_EVERY_NMONTHS:
	case TDIR_MONTH_SPECIFIC_DAY_NMONTHS: 
	case TDIR_MONTH_FIRSTLASTWEEKDAY_NMONTHS: 
		if (sRegularity[MONTHLY].IsEmpty())
			sRegularity[MONTHLY].LoadString(IDS_MONTHLY);  
		
		return sRegularity[MONTHLY];
		
	case TDIR_YEAR_SPECIFIC_DOW_MONTH:
	case TDIR_YEAR_EVERY_NYEARS:
	case TDIR_YEAR_SPECIFIC_DAY_MONTH:  
		if (sRegularity[YEARLY].IsEmpty())
			sRegularity[YEARLY].LoadString(IDS_YEARLY);   
		
		return sRegularity[YEARLY];
		
	case TDIR_ONCE:
		if (bIncOnce)
		{
			if (sRegularity[ONCE].IsEmpty())
				sRegularity[ONCE].LoadString(IDS_ONCEONLY); 
			
			return sRegularity[ONCE];
		}
		break;
		
	default: 
		// unknown or deprecated options
		ASSERT(0);
		break;
	}
	
	return _T("");
}

BOOL TDCRECURRENCE::SetRegularity(TDC_REGULARITY nReg, DWORD dwSpec1, DWORD dwSpec2)
{
	RECURRENCE_REGULARITY nRegularity = TDC::MapTDCRegularityToRegularity(nReg);

	return CRecurrence::SetRegularity(nRegularity, dwSpec1, dwSpec2);
}

TDC_REGULARITY TDCRECURRENCE::GetRegularity(DWORD& dwSpec1, DWORD& dwSpec2) const
{
	RECURRENCE_REGULARITY nRegularity = CRecurrence::GetRegularity(dwSpec1, dwSpec2);

	return TDC::MapRegularityToTDCRegularity(nRegularity);
}

TDC_REGULARITY TDCRECURRENCE::GetRegularity() const
{
	RECURRENCE_REGULARITY nRegularity = CRecurrence::GetRegularity();

	return TDC::MapRegularityToTDCRegularity(nRegularity);
}

