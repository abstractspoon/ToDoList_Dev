#if !defined(AFX_BURNDOWNPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_BURNDOWNPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkloadPreferencesDlg.h : header file
//

#include "BurndownGraphColorListCtrl.h"

#include "..\Shared\preferencesbase.h"
#include "..\Shared\colorbutton.h"

/////////////////////////////////////////////////////////////////////////////

// WPARAM = , LPARAM = 
const UINT WM_BDC_PREFSHELP = ::RegisterWindowMessage(_T("WM_BDC_PREFSHELP"));

/////////////////////////////////////////////////////////////////////////////

class CBurndownChart;

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesPage dialog

class CBurndownPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CBurndownPreferencesPage(const CBurndownChart& chart, CWnd* pParent = NULL);

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

	const CGraphColorMap& GetGraphColors() const { return m_lcGraphColors.GetGraphColors(); }

protected:
// Dialog Data
	//{{AFX_DATA(CBurndownPreferencesPage)
	enum { IDD = IDD_PREFERENCES_PAGE };
	//}}AFX_DATA
	CBurndownGraphColorListCtrl	m_lcGraphColors;
	CColourButton m_btnTodayColor;

	const CBurndownChart& m_chart;

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
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CBurndownPreferencesDlg dialog

class CBurndownPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CBurndownPreferencesDlg(const CBurndownChart& chart, CWnd* pParent);

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const { m_page.SavePreferences(pPrefs, szKey); }
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey) { m_page.LoadPreferences(pPrefs, szKey); }

	const CGraphColorMap& GetGraphColors() const { return m_page.GetGraphColors(); }

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
