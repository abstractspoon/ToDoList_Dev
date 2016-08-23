#if !defined(AFX_TDLLANGUAGEDLG_H__F1F61CB0_9292_431D_9049_1F3ED11478A1__INCLUDED_)
#define AFX_TDLLANGUAGEDLG_H__F1F61CB0_9292_431D_9049_1F3ED11478A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLLanguageDlg.h : header file
//

#include "TDLLanguageComboBox.h"
#include "TDLDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLLanguageDlg dialog

class CTDLLanguageDlg : public CTDLDialog
{
// Construction
public:
	CTDLLanguageDlg(CWnd* pParent = NULL);   // standard constructor

	CString GetLanguageFile() const;
	static CString GetDefaultLanguage();

// Dialog Data
	//{{AFX_DATA(CTDLLanguageDlg)
	enum { IDD = IDD_LANGUAGE_DIALOG };
	CTDLLanguageComboBox	m_cbLanguages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLLanguageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLLanguageDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLLANGUAGEDLG_H__F1F61CB0_9292_431D_9049_1F3ED11478A1__INCLUDED_)
