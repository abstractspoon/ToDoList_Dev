#if !defined(AFX_BURNDOWNPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_BURNDOWNPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkloadPreferencesDlg.h : header file
//

#include "..\Shared\preferencesbase.h"
#include "..\Shared\colorbutton.h"

#include "..\Interfaces\ipreferences.h"

/////////////////////////////////////////////////////////////////////////////

// WPARAM = , LPARAM = 
const UINT WM_BDC_PREFSHELP = ::RegisterWindowMessage(_T("WM_BDC_PREFSHELP"));

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage dialog

class CBurndownPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CBurndownPreferencesPage(CWnd* pParent = NULL);

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CBurndownPreferencesPage)
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurndownPreferencesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBurndownPreferencesPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void EnableDisableControls();
};

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesDlg dialog

class CBurndownPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CBurndownPreferencesDlg(CWnd* pParent);

protected:
// Dialog Data
	//{{AFX_DATA(CBurndownPreferencesDlg)
	//}}AFX_DATA
	CBurndownPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBurndownPreferencesDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
