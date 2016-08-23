#if !defined(AFX_BROWSERDLG_H__3AD7A73A_4A17_4625_92E9_F74C6D930C34__INCLUDED_)
#define AFX_BROWSERDLG_H__3AD7A73A_4A17_4625_92E9_F74C6D930C34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// browserdlg.h : header file
//

#include "RuntimeDlg.h"
#include "WebBrowserctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CBrowserDlg dialog

class CBrowserDlg : public CRuntimeDlg
{
// Construction
public:
	CBrowserDlg(BOOL bBrowser = TRUE, LPCTSTR szSettingsKey = NULL);

	virtual int DoModal(LPCTSTR szCaption, LPCTSTR szUrlPath, CWnd* pParentWnd = NULL);
	virtual BOOL Create(LPCTSTR szCaption, LPCTSTR szUrlPath, CWnd* pParentWnd);

	BOOL Navigate(LPCTSTR szUrlPath, LPCTSTR szCaption = NULL);
	BOOL SetDisplayInBrowser(BOOL bBrowser = TRUE);
	void SetHideOnClose(BOOL bHide = TRUE) { m_bHideOnClose = bHide; }

protected:
	CWebBrowserCtrl m_browser;
	CEdit m_edit;
	CString m_sUrl;
	BOOL m_bBrowser, m_bHideOnClose;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual void OnCancel();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBrowserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnEditSetFocus();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERDLG_H__3AD7A73A_4A17_4625_92E9_F74C6D930C34__INCLUDED_)
