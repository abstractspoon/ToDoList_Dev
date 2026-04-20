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
// CWorkloadPreferencesPage page

class CWorkloadPreferencesPage : public CPreferencesPageBase
{
public:
	CWorkloadPreferencesPage(CWnd* pParent = NULL);

	void GetColumnVisibility(CDWordArray& aColumnVis) const;

	BOOL GetOverload(int& nFromPercent, COLORREF& color) const;
	BOOL GetUnderload(int& nToPercent, COLORREF& color) const;
	COLORREF GetOverlapColor() const { return (m_bEnableOverlapColor ? m_crOverlap : CLR_NONE); }

	BOOL GetPreferTimeEstimateForCalcs() const { return m_bPreferTimeEstimateInCalcs; }
	BOOL GetAutoCalculateMissingAllocations() const { return m_bAutoCalcAllocations || m_bRecalcAllocations; }
	BOOL GetRecalculateAllocations() const { return m_bRecalcAllocations; }
	BOOL GetRecalculateAllocationsProportionally() const { return m_bRecalcProportionally; }
	BOOL GetIncludeDatelessTasksInPeriod() const { return m_bIncludeDatelessTasksInPeriod; }
	
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;
	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);

protected:
	BOOL	m_bPreferTimeEstimateInCalcs;
	BOOL	m_bAutoCalcAllocations;
	BOOL	m_bEnableOverload;
	BOOL	m_bEnableUnderload;
	BOOL	m_bRecalcAllocations;
	int		m_bRecalcProportionally;
	BOOL	m_bEnableOverlapColor;
	BOOL	m_bIncludeDatelessTasksInPeriod;
	int		m_nOverloadFromPercent;
	int		m_nUnderloadToPercent;
	COLORREF m_crOverload, m_crUnderload, m_crOverlap;

	CColourButton m_btnOverloadColor;
	CColourButton m_btnUnderloadColor;
	CColourButton m_btnOverlapColor;
	CCheckListBoxEx m_lbColumnVisibility;
	CDWordArray m_aColumnVis;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	afx_msg void OnEnableOverload();
	afx_msg void OnEnableUnderload();
	afx_msg void OnSetRecalcAllocations();
	afx_msg void OnEnableOverlapColor();

	DECLARE_MESSAGE_MAP()

protected:
	void EnableDisableControls();
};

/////////////////////////////////////////////////////////////////////////////
// CWorkloadPreferencesDlg dialog

class CWorkloadPreferencesDlg : public CPreferencesDlgBase
{
public:
	CWorkloadPreferencesDlg(CWnd* pParent);

	void GetColumnVisibility(CDWordArray& aColumnVis) const { m_page.GetColumnVisibility(aColumnVis); }
	
	BOOL GetOverload(int& nFromPercent, COLORREF& color) const { return m_page.GetOverload(nFromPercent, color); }
	BOOL GetUnderload(int& nToPercent, COLORREF& color) const { return m_page.GetUnderload(nToPercent, color); }
	COLORREF GetOverlapColor() const { return m_page.GetOverlapColor(); }

	BOOL GetAutoCalculateMissingAllocations() const { return m_page.GetAutoCalculateMissingAllocations(); }
	BOOL GetPreferTimeEstimateForCalcs() const { return m_page.GetPreferTimeEstimateForCalcs(); }
	BOOL GetRecalculateAllocations() const { return m_page.GetRecalculateAllocations(); }
	BOOL GetRecalculateAllocationsProportionally() const { return m_page.GetRecalculateAllocationsProportionally(); }
	BOOL GetIncludeDatelessTasksInPeriod() const { return m_page.GetIncludeDatelessTasksInPeriod(); }

protected:
	CWorkloadPreferencesPage m_page;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoHelp();

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WORKLOADPREFERENCESDLG_H__4BEDF571_7002_4C0D_B355_1334515CA1F9__INCLUDED_)
