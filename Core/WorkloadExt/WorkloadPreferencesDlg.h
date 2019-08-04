#if !defined(AFX_WORKLOADPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
#define AFX_WORKLOADPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkloadPreferencesDlg.h : header file
//

#include "..\Shared\preferencesbase.h"
#include "..\Shared\checklistboxex.h"
#include "..\Shared\colorbutton.h"

#include "..\Interfaces\ipreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesPage dialog

class CWorkloadPreferencesPage : public CPreferencesPageBase
{
// Construction
public:
	CWorkloadPreferencesPage(CWnd* pParent = NULL);

	void GetColumnVisibility(CDWordArray& aColumnVis) const;

	BOOL GetOverload(int& nFromPercent, COLORREF& color) const;
	BOOL GetUnderload(int& nToPercent, COLORREF& color) const;

	BOOL GetAutoCalculateMissingAllocations() const { return m_bAutoCalcAllocations; }
	BOOL GetPreferTimeEstimateForCalcs() const { return m_bPreferTimeEstimateInCalcs; }

	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
// Dialog Data
	//{{AFX_DATA(CWorkloadPreferencesPage)
	BOOL	m_bPreferTimeEstimateInCalcs;
	BOOL	m_bAutoCalcAllocations;
	BOOL	m_bEnableOverload;
	BOOL	m_bEnableUnderload;
	//}}AFX_DATA
	int		m_nOverloadFromPercent;
	int		m_nUnderloadToPercent;
	COLORREF m_crOverload, m_crUnderload;

	CColorButton m_btnOverloadColor;
	CColorButton m_btnUnderloadColor;
	CCheckListBoxEx m_lbColumnVisibility;
	CDWordArray m_aColumnVis;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkloadPreferencesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorkloadPreferencesPage)
	afx_msg void OnEnableOverload();
	afx_msg void OnEnableUnderload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
};

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesDlg dialog

class CWorkloadPreferencesDlg : public CPreferencesDlgBase
{
// Construction
public:
	CWorkloadPreferencesDlg(CWnd* pParent);

	void GetColumnVisibility(CDWordArray& aColumnVis) const { m_page.GetColumnVisibility(aColumnVis); }
	
	BOOL GetOverload(int& nFromPercent, COLORREF& color) const { return m_page.GetOverload(nFromPercent, color); }
	BOOL GetUnderload(int& nToPercent, COLORREF& color) const { return m_page.GetUnderload(nToPercent, color); }

	BOOL GetAutoCalculateMissingAllocations() const { return m_page.GetAutoCalculateMissingAllocations(); }
	BOOL GetPreferTimeEstimateForCalcs() const { return m_page.GetPreferTimeEstimateForCalcs(); }

protected:
// Dialog Data
	//{{AFX_DATA(CWorkloadPreferencesDlg)
	//}}AFX_DATA
	CWorkloadPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWorkloadPreferencesDlg)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKLOADPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
