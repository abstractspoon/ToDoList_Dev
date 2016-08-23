#if !defined(AFX_ABOUTDLG_H__31BB99EB_BB66_4016_82F1_8688520BE14A__INCLUDED_)
#define AFX_ABOUTDLG_H__31BB99EB_BB66_4016_82F1_8688520BE14A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//

#include "runtimedlg.h"
#include "richeditncborder.h"
#include "richeditbasectrl.h"

#include "..\3rdparty\xhtmlstatic.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

enum AB_STYLE
{
	ABS_HTMLCOPYRIGHT,
	ABS_EDITCOPYRIGHT,
	ABS_LISTCOPYRIGHT,
};

class CAboutDlg : public CRuntimeDlg
{
// Construction
public:
	CAboutDlg(UINT nAppIconID, AB_STYLE nStyle, 
				LPCTSTR szAppName, LPCTSTR szAppDescription, LPCTSTR szCopyright, LPCTSTR szLicense,
                int nAppLines = 1, int nDescLines = 3, int nCopyrightLines = 2, int nLicenseLines = 2,
				int nWidth = -1);

	int DoModal();

protected:
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	CStatic	m_stIcon;
	CString	m_sAppName;
	CString	m_sAppDescription;
	CString	m_sCopyright;
	CString	m_sLicense;
	//}}AFX_DATA
	CXHTMLStatic m_stAppName;
	CXHTMLStatic m_stAppDescription;
	CXHTMLStatic m_stCopyright;
	CXHTMLStatic m_stLicense;
	HICON m_hIcon;
	AB_STYLE m_nStyle;
	CRichEditNcBorder m_reBorder;
	CRichEditBaseCtrl m_reCopyright;
	CListCtrl m_lcCopyright;
	CImageList m_il;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__31BB99EB_BB66_4016_82F1_8688520BE14A__INCLUDED_)
