// MySpellTestDlg.h : header file
//

#if !defined(AFX_MYSPELLTESTDLG_H__88432A84_3A5F_4349_AD58_03048D9BD70E__INCLUDED_)
#define AFX_MYSPELLTESTDLG_H__88432A84_3A5F_4349_AD58_03048D9BD70E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMySpellTestDlg dialog

class CMySpellTestDlg : public CDialog
{
// Construction
public:
	CMySpellTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMySpellTestDlg)
	enum { IDD = IDD_MYSPELLTEST_DIALOG };
	CRichEditCtrl	m_reText;
	CString	m_sText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpellTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMySpellTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSpellcheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPELLTESTDLG_H__88432A84_3A5F_4349_AD58_03048D9BD70E__INCLUDED_)
