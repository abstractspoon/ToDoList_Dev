#if !defined(AFX_GANTTDATERANGESLIDER_H__28F0CDBA_3B22_4EB1_8169_07362290F6E1__INCLUDED_)
#define AFX_GANTTDATERANGESLIDER_H__28F0CDBA_3B22_4EB1_8169_07362290F6E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttDateRangeSlider.h : header file
//

#include "GanttEnum.h"
#include "GanttStruct.h"

#include "..\Shared\RangeSliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGanttDateRangeSlider window

class CGanttDateRangeSlider : public CRangeSliderCtrl
{
// Construction
public:
	CGanttDateRangeSlider();

	BOOL Initialise(GTLC_MONTH_DISPLAY nDisplay);
	BOOL SetMonthDisplay(GTLC_MONTH_DISPLAY nDisplay);

	BOOL GetMaxRange(GANTTDATERANGE& dtRange) const;
	BOOL SetMaxRange(const GANTTDATERANGE& dtRange);

	BOOL HasSelectedRange() const;
	BOOL GetSelectedRange(GANTTDATERANGE& dtRange) const;
	BOOL SetSelectedRange(const GANTTDATERANGE& dtRange);

	CString FormatRange(BOOL bZeroBasedDecades, TCHAR cDelim = '-') const;

protected:
	GTLC_MONTH_DISPLAY m_nMonthDisplay;
	GANTTDATERANGE m_dtMaxRange;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttDateRangeSlider)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGanttDateRangeSlider();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGanttDateRangeSlider)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL IsValidMove(double left, double right) const { return (right > left); }

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTDATERANGESLIDER_H__28F0CDBA_3B22_4EB1_8169_07362290F6E1__INCLUDED_)
