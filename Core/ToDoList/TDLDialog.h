#if !defined(AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_)
#define AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLDialog.h : header file
//

#include "..\shared\winhelpbutton.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\SizeGrip.h"
#include "..\shared\Icon.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLDialog dialog

class CTDLDialog : public CDialog, protected CDialogHelper
{
	DECLARE_DYNAMIC(CTDLDialog)

	int DoModal(HICON hIcon = NULL); // Caller owns icon

	static COLORREF GetErrorLabelTextColor();

// Construction
protected:
	CTDLDialog(UINT nIDTemplate, LPCTSTR szPrefsKey = NULL, CWnd* pParent = NULL);   // standard constructor

protected:
	CWinHelpButton m_btnHelp;
	CSizeGrip m_sbGrip;

	CIcon m_iconDlg;
	static CIcon s_iconApp;

	CSize m_sizeOrg, m_sizePrev;
	CString m_sPrefsKey;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnRepositionControls(int /*dx*/, int /*dy*/) {}
	virtual BOOL DoIdleProcessing() { return FALSE; }

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnGetFileIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL IsResizable() const;
	BOOL SetIcon(UINT nIconID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLDIALOG_H__72F38546_6EAE_43DA_A698_8F791C458BC8__INCLUDED_)
