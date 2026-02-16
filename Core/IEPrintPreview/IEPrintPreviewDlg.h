// IEPrintPreviewDlg.h : header file
//

#if !defined(AFX_IEPRINTPREVIEWDLG_H__EFAE74B0_6238_4C25_ADDA_2F3DC40B9695__INCLUDED_)
#define AFX_IEPRINTPREVIEWDLG_H__EFAE74B0_6238_4C25_ADDA_2F3DC40B9695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Shared\EnBrowserCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CIEPrintPreviewDlg dialog

class CIEPrintPreviewDlg : public CDialog
{
// Construction
public:
	CIEPrintPreviewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIEPrintPreviewDlg)
	enum { IDD = IDD_IEPRINTPREVIEW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEPrintPreviewDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CEnBrowserCtrl m_ie;

	// Generated message map functions
	//{{AFX_MSG(CIEPrintPreviewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPrintpreview();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEPRINTPREVIEWDLG_H__EFAE74B0_6238_4C25_ADDA_2F3DC40B9695__INCLUDED_)
