// GanttDateRangeSlider.cpp : implementation file
//

#include "stdafx.h"
#include "GanttDateRangeSlider.h"
#include "GanttStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGanttDateRangeSlider

CGanttDateRangeSlider::CGanttDateRangeSlider() : m_nMonthDisplay(GTLC_DISPLAY_NONE)
{
}

CGanttDateRangeSlider::~CGanttDateRangeSlider()
{
}


BEGIN_MESSAGE_MAP(CGanttDateRangeSlider, CRangeSlider)
	//{{AFX_MSG_MAP(CGanttDateRangeSlider)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGanttDateRangeSlider message handlers

BOOL CGanttDateRangeSlider::Initialise(GTLC_MONTH_DISPLAY nDisplay)
{
	ASSERT(m_nMonthDisplay == GTLC_DISPLAY_NONE);

	return SetMonthDisplay(nDisplay);
}

BOOL CGanttDateRangeSlider::SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay)
{
	if (nDisplay == GTLC_DISPLAY_NONE)
	{
		ASSERT(0);
		return FALSE;
	}

	m_nMonthDisplay = nDisplay;
	return TRUE;
}

BOOL CGanttDateRangeSlider::SetMaxRange(const GANTTDATERANGE& dtRange)
{
	if (m_nMonthDisplay == GTLC_DISPLAY_NONE)
	{
		ASSERT(0);
		return FALSE;
	}
	
	if (!dtRange.IsValid())
	{
		ASSERT(0);
		return FALSE;
	}

	int nNumCols = GetRequiredColumnCount(dtRange, m_nMonthDisplay) + 1;
	ASSERT(nNumCols);

	SetMinMax(0.0, nNumCols);
	SetStep(1.0);

	BOOL bFirstTime = !m_dtMaxRange.IsValid();
	m_dtMaxRange = dtRange;

	if (bFirstTime)
	{
		SetRange(0.0, nNumCols);
	}
	else
	{
		// TODO
	}

	return TRUE;
}

CString CGanttDateRangeSlider::FormatSelectedRange() const
{
	CString sRange;
	GANTTDATERANGE dtRange;

	if (GetSelectedRange(dtRange))
		sRange = dtRange.Format(m_nMonthDisplay);

	return sRange;
}

BOOL CGanttDateRangeSlider::GetSelectedRange(GANTTDATERANGE& dtRange) const
{
	int nStartCol = (int)GetLeft();
	int nEndCol = (int)GetRight();

	ASSERT(nEndCol > nStartCol);

	// Subtract 1 because the end column index is really 
	// the start of the next column
	nEndCol--;
	
	int nMonthsPerCol = GetNumMonthsPerColumn(m_nMonthDisplay);

	COleDateTime dtStart(m_dtMaxRange.GetStart());
	CDateHelper::IncrementMonth(dtStart, nStartCol * nMonthsPerCol);

	COleDateTime dtEnd(m_dtMaxRange.GetStart());
	CDateHelper::IncrementMonth(dtEnd, nEndCol * nMonthsPerCol);

	dtRange.SetStart(dtStart);
	dtRange.SetEnd(dtEnd);

	TRACE(_T("CGanttDateRangeSlider::GetSelectedRange(%s)\n"), dtRange.Format(m_nMonthDisplay));

	return TRUE;
}

BOOL CGanttDateRangeSlider::SetSelectedRange(const GANTTDATERANGE& dtRange)
{
	if (!dtRange.IsValid() || !m_dtMaxRange.Contains(dtRange))
	{
		ASSERT(0);
		return FALSE;
	}

	int nNumMonthsStart = CDateHelper::CalcMonthsFromTo(m_dtMaxRange.GetStart(), dtRange.GetStart(), TRUE);
	int nNumMonthsEnd = CDateHelper::CalcMonthsFromTo(m_dtMaxRange.GetStart(), dtRange.GetEnd(), TRUE);

	int nMonthsPerCol = GetNumMonthsPerColumn(m_nMonthDisplay);

	ASSERT(nNumMonthsStart < nNumMonthsEnd);
	SetRange(nNumMonthsStart / nMonthsPerCol, nNumMonthsEnd / nMonthsPerCol);

	return TRUE;
}
