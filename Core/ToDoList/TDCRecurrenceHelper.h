// ToDoitem.h: interface for the CToDoitem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDCRECURRENCEHELPER_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
#define AFX_TDCRECURRENCEHELPER_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////////

#include "tdcrecurrence.h"

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////////

struct TODOITEM;

///////////////////////////////////////////////////////////////////////////////////

class CTDCRecurrenceHelper
{
public:
	CTDCRecurrenceHelper(const TODOITEM& tdi);
	
	BOOL CalcNextOccurrence(COleDateTimeRange& dtOccur) const;
	BOOL CalcNextOccurrence(COleDateTime& dtNext, BOOL& bDue) const;

	int CalcNextOccurrences(const COleDateTimeRange& dtRange, CArray<COleDateTimeRange, COleDateTimeRange&>& aOccur) const;

protected:
	const TODOITEM& m_tdi;
	
protected:
	BOOL CalcNextOccurrence(const COleDateTime& dtNext, COleDateTimeRange& dtOccur) const;

	int CalcNextOccurrences(const COleDateTimeRange& dtRange, CArray<double, double&>& aDates) const;
	BOOL GetFromDate(COleDateTime& dtFrom) const;
	int CalcDaysToNextOccurrence(const COleDateTime& dtTo) const;

	BOOL CanRecur() const;
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TDCRECURRENCEHELPER_H__02C3C360_45AB_45DC_B1BF_BCBEA472F0C7__INCLUDED_)
