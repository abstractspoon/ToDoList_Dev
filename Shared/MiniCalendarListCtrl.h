#if !defined(AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_)
#define AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniCalendarCtrl.h : header file
//

#include "..\3rdParty\FPSMiniCalendarListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMiniCalendarCtrl window

class CMiniCalendarListCtrl : public CFPSMiniCalendarListCtrl
{
// Construction
public:
	CMiniCalendarListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniCalendarCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMiniCalendarListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMiniCalendarCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINICALENDARCTRL_H__947B7F79_1D64_405C_AD29_D0876EEBD57A__INCLUDED_)
