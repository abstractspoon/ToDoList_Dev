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
public:
	CTDLLanguageDlg(LPCTSTR szSelLangFile = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTDLLanguageDlg();

	CString GetSelectedLanguageFile(BOOL bRelative = FALSE) const;
	static CString GetDefaultLanguage();

protected:
	CTDLLanguageComboBox m_cbLanguages;
	BOOL m_bFirstTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TDLLANGUAGEDLG_H__F1F61CB0_9292_431D_9049_1F3ED11478A1__INCLUDED_)
