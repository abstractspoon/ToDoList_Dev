#if !defined(AFX_FRAMEWNDCONTAINER_H__222E6BBC_FB23_41D6_B2EF_0E4EE889F388__INCLUDED_)
#define AFX_FRAMEWNDCONTAINER_H__222E6BBC_FB23_41D6_B2EF_0E4EE889F388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// framewndcontainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameWndContainer frame

class CFrameWndContainer : public CFrameWnd
{
	DECLARE_DYNCREATE(CFrameWndContainer)
protected:
	CFrameWndContainer();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL Attach(HWND wndChild);
	void Detach() { m_hwndContained = NULL; }

protected:
	HWND m_hwndContained;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameWndContainer)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameWndContainer();

	// Generated message map functions
	//{{AFX_MSG(CFrameWndContainer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEWNDCONTAINER_H__222E6BBC_FB23_41D6_B2EF_0E4EE889F388__INCLUDED_)
