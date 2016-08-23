// KanbanStatic.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANBANSTATIC_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
#define AFX_KANBANSTATIC_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\shared\TimeHelper.h"

/////////////////////////////////////////////////////////////////////////////

// private helper
static TH_UNITS MapUnitsToTHUnits(TDC_UNITS nUnits)
{
	switch (nUnits)
	{
	case TDCU_NULL:		return THU_NULL;
	case TDCU_MINS:		return THU_MINS;
	case TDCU_HOURS:	return THU_HOURS;
	case TDCU_DAYS:		return THU_DAYS;
	case TDCU_WEEKDAYS:	return THU_WEEKDAYS;
	case TDCU_WEEKS:	return THU_WEEKS;
	case TDCU_MONTHS:	return THU_MONTHS;
	case TDCU_YEARS:	return THU_YEARS;
	}
	
	// all else
	ASSERT(0);
	return THU_NULL;
}


////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_KANBANSTATIC_H__49626430_4D7B_460F_98B7_363FCDBC388C__INCLUDED_)
