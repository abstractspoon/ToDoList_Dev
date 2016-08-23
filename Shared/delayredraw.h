#if !defined(AFX_DELAYREDRAW_H__5901E70F_441F_47B7_8D40_13F03B1D9176__INCLUDED_)
#define AFX_DELAYREDRAW_H__5901E70F_441F_47B7_8D40_13F03B1D9176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// delayredraw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDelayRedraw window

class CDelayRedraw : public CFrameWnd
{
public:
// Construction
	static BOOL Start(HWND hWnd, int nDelay = 100);

protected:
	CDelayRedraw(HWND hWnd, int nDelay = 100);

protected:
	HWND m_hwndRef;
	int m_nDelay;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelayRedraw)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDelayRedraw();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDelayRedraw)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELAYREDRAW_H__5901E70F_441F_47B7_8D40_13F03B1D9176__INCLUDED_)
