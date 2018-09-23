#if !defined(AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_)
#define AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalendarCtrlEx.h : header file
//

#include "..\3rdParty\CalendarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCalendarCtrlEx window

class CCalendarCtrlEx : public CCalendarCtrl
{
// Construction
public:
	CCalendarCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalendarCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCalendarCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCalendarCtrlEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawHeader(CDC* pDC);
	virtual CString GetMonthName(const COleDateTime& date, BOOL bShort = FALSE) const;
	virtual CString GetDayOfWeekName(const COleDateTime& date, BOOL bShort = FALSE) const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALENDARCTRLEX_H__A8C8254B_FDE4_4844_9381_70A67A469B3E__INCLUDED_)
