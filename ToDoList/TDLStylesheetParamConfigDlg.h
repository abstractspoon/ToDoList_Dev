#if !defined(AFX_TDLSTYLESHEETPARAMCONFIGDLG_H__63273DD3_77BA_4D82_A3CD_FB2A7D4DEE2D__INCLUDED_)
#define AFX_TDLSTYLESHEETPARAMCONFIGDLG_H__63273DD3_77BA_4D82_A3CD_FB2A7D4DEE2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLStylesheetParamConfigDlg.h : header file
//

#include "..\shared\StylesheetParamConfigDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLStylesheetParamConfigDlg dialog

class CTDLStylesheetParamConfigDlg : public CStylesheetParamConfigDlg
{
// Construction
public:
	CTDLStylesheetParamConfigDlg(LPCTSTR szStylesheetPath, CWnd* pParent = NULL);   // standard constructor

	int DoModal();

	CString GetStylesheetPath() const;
	BOOL GetUsingConfiguredStylesheet() const;

	static CString GetConfigStylesheetPath(LPCTSTR szStylesheetPath);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLStylesheetParamConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLStylesheetParamConfigDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLSTYLESHEETPARAMCONFIGDLG_H__63273DD3_77BA_4D82_A3CD_FB2A7D4DEE2D__INCLUDED_)
