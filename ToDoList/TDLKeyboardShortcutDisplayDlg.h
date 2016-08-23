#if !defined(AFX_KEYBOARDSHORTCUTDISPLAYDLG_H__BE407DB6_8629_4228_A2C9_BB96F87D6055__INCLUDED_)
#define AFX_KEYBOARDSHORTCUTDISPLAYDLG_H__BE407DB6_8629_4228_A2C9_BB96F87D6055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardShortcutDisplayDlg.h : header file
//

#include "TDLDialog.h"

#include "..\shared\shortcutManager.h"

/////////////////////////////////////////////////////////////////////////////
// CKeyboardShortcutDisplayDlg dialog

class CTDLKeyboardShortcutDisplayDlg : public CTDLDialog
{
// Construction
public:
	CTDLKeyboardShortcutDisplayDlg(const CStringArray& aMapping, TCHAR cDelim, CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CKeyboardShortcutDisplayDlg)
	enum { IDD = IDD_KEYBOARDSHORTCUTDISPLAYDIALOG };
	CListCtrl	m_lcShortcuts;
	//}}AFX_DATA

	const CStringArray& m_aMapping;
	TCHAR m_cDelim;
	CImageList m_il;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardShortcutDisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyboardShortcutDisplayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopyshortcuts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDSHORTCUTDISPLAYDLG_H__BE407DB6_8629_4228_A2C9_BB96F87D6055__INCLUDED_)
