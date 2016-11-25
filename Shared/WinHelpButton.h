#if !defined(AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
#define AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorbutton.h : header file
//

#include "..\shared\iconbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CWinHelpButton window

class CWinHelpButton : public CIconButton
{
// Construction
public:
	CWinHelpButton(UINT nHelpID, BOOL bAutoHandleClick = TRUE);

	BOOL Create(UINT nID, CWnd* pParent, const CRect& rPos = CRect(0, 0, 0, 0));
	void UpdatePosition();
	UINT GetHelpID() const { return m_nHelpID; }

	static BOOL SetDefaultIcon(HICON hIcon);
	static BOOL SetDefaultTooltip(LPCTSTR szTooltip);

protected:
	UINT m_nHelpID;
	BOOL m_bAutoHandleClick;
	BOOL m_bAutoPosition;

	static HICON s_hDefIcon;
	static CString s_sDefTooltip;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinHelpButton)
	//}}AFX_VIRTUAL
	virtual BOOL DoAction();

// Implementation
public:
	virtual ~CWinHelpButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWinHelpButton)
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL GetAutoPosition(const CWnd* pParent, CRect& rPos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINHELPBUTTON_H__4FA393FA_20E7_4C07_A682_5AA7A8306DF4__INCLUDED_)
