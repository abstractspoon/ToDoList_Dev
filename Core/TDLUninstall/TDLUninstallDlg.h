// TDLUninstallDlg.h : header file
//

#if !defined(AFX_TDLUNINSTALLDLG_H__BBD41ACC_AC1B_4F80_894B_E1F3554A19C8__INCLUDED_)
#define AFX_TDLUNINSTALLDLG_H__BBD41ACC_AC1B_4F80_894B_E1F3554A19C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTDLUninstallDlg dialog

class CTDLUninstallDlg : public CDialog
{
// Construction
public:
	CTDLUninstallDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTDLUninstallDlg)
	enum { IDD = IDD_TDLUNINSTALL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLUninstallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTDLUninstallDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLUNINSTALLDLG_H__BBD41ACC_AC1B_4F80_894B_E1F3554A19C8__INCLUDED_)
