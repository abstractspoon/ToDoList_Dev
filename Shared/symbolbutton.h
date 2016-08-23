#if !defined(AFX_SYMBOLBUTTON_H__C9027C47_A3B6_40B9_A71C_1D3534AA7CF0__INCLUDED_)
#define AFX_SYMBOLBUTTON_H__C9027C47_A3B6_40B9_A71C_1D3534AA7CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// symbolbutton.h : header file
//

#include "custombutton.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolButton window

class CSymbolButton : public CCustomButton
{
// Construction
public:
	CSymbolButton(LPCTSTR szFaceName = _T("Wingdings"), int nPoint = 10);

protected:
	HFONT m_hFont;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolButton)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSymbolButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLBUTTON_H__C9027C47_A3B6_40B9_A71C_1D3534AA7CF0__INCLUDED_)
