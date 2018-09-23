#if !defined(AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_)
#define AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniCalendarCtrl.h : header file
//

#include "..\3rdParty\FPSMiniCalendarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMiniCalendarCtrl window

class CMiniCalendarCtrl : public CFPSMiniCalendarCtrl
{
// Construction
public:
	CMiniCalendarCtrl();

	void SetShowWeekNumbers(BOOL bValue) { m_bShowWeekNumbers = bValue; }
	BOOL GetShowWeekNumbers() { return m_bShowWeekNumbers; }

protected:
	BOOL m_bShowWeekNumbers;
	int m_nWeekNumberWidth;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniCalendarCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMiniCalendarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMiniCalendarCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	virtual CSize ComputeSize();
	int ComputeWeekNumberWidth();

	virtual int DrawDaysOfWeek(CDC& dc, int iY, int iLeftX, int iRow, int iCol);
	virtual int DrawDays(CDC& dc, int iY, int iLeftX, int iRow, int iCol, int iMonth, int iYear);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_)
