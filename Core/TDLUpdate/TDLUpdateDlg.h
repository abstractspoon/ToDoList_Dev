// TDLUpdateDlg.h : header file
//

#if !defined(AFX_TDLUPDATEDLG_H__0644D8BF_5C7D_49EB_A493_EFF04A363E7F__INCLUDED_)
#define AFX_TDLUPDATEDLG_H__0644D8BF_5C7D_49EB_A493_EFF04A363E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTDLUpdateDlg dialog

class CTDLUpdateDlg : public CDialog
{
// Construction
public:
	CTDLUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTDLUpdateDlg)
	enum { IDD = IDD_TDLUPDATE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTDLUpdateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLUPDATEDLG_H__0644D8BF_5C7D_49EB_A493_EFF04A363E7F__INCLUDED_)
