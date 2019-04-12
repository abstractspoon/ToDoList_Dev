#if !defined(AFX_EDITWEBLINKDLG_H__DD0DB833_7D2B_4E92_8478_FD1B43B3207E__INCLUDED_)
#define AFX_EDITWEBLINKDLG_H__DD0DB833_7D2B_4E92_8478_FD1B43B3207E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditWebLinkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditWebLinkDlg dialog

class CEditWebLinkDlg : public CDialog
{
// Construction
public:
	CEditWebLinkDlg(CWnd* pParent = NULL);   // standard constructor

	CString GetWebLink() { return m_sWebLink; }

protected:
// Dialog Data
	//{{AFX_DATA(CEditWebLinkDlg)
	enum { IDD = IDD_EDITWEBLINK_DIALOG };
	CString	m_sWebLink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditWebLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditWebLinkDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWEBLINKDLG_H__DD0DB833_7D2B_4E92_8478_FD1B43B3207E__INCLUDED_)
