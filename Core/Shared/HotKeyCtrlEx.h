#if !defined(AFX_HOTKEYCTRLEX_H__D798C9D0_5F7D_42A1_BA5C_0411A65D234B__INCLUDED_)
#define AFX_HOTKEYCTRLEX_H__D798C9D0_5F7D_42A1_BA5C_0411A65D234B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// hotkeyctrlex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHotKeyCtrlEx window

class CHotKeyCtrlEx : public CHotKeyCtrl
{
// Construction
public:
	CHotKeyCtrlEx(BOOL bGlobal = FALSE);

	void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);
	void SetHotKey(DWORD dwHotkey);

// Attributes
protected:
	WORD m_wInvalidComb;
	WORD m_wModifiers;
	BOOL m_bGlobal;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyCtrlEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHotKeyCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHotKeyCtrlEx)
	//}}AFX_MSG
	afx_msg LRESULT OnSetRules(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYCTRLEX_H__D798C9D0_5F7D_42A1_BA5C_0411A65D234B__INCLUDED_)
