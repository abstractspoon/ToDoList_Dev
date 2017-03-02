#if !defined(AFX_FILTERBAR_H__FAABD9A1_72C4_4731_B7A4_48251860672C__INCLUDED_)
#define AFX_FILTERBAR_H__FAABD9A1_72C4_4731_B7A4_48251860672C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterBar.h : header file
//

#include "tdcstruct.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdlfilteroptioncombobox.h"
#include "tdlfiltercombobox.h"
#include "tdlfilterdatecombobox.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\encheckcombobox.h"
#include "..\shared\enedit.h"
#include "..\shared\wndprompt.h"
#include "..\shared\uithemefile.h"
#include "..\shared\datetimectrlex.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterBar dialog

class CFilteredToDoCtrl;
class CDeferWndMove;

class CTDLFilterBar : public CDialog, public CDialogHelper
{
// Construction
public:
	CTDLFilterBar(CWnd* pParent = NULL);   // standard constructor
	~CTDLFilterBar();

	BOOL Create(CWnd* pParentWnd, UINT nID = 0, BOOL bVisible = TRUE);

	FILTER_SHOW GetFilter(FTDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const;
	FILTER_SHOW GetFilter(CString& sCustom) const;

	void AddCustomFilters(const CStringArray& aFilters);
	int GetCustomFilters(CStringArray& aFilters) const;
	void RemoveCustomFilters();
	void ShowDefaultFilters(BOOL bShow);

	void RefreshFilterControls(const CFilteredToDoCtrl& tdc);
	void SetFilterLabelAlignment(BOOL bLeft);
	void SetPriorityColors(const CDWordArray& aColors);
	int CalcHeight(int nWidth);
	void SetVisibleFilters(const CTDCAttributeMap& mapFilters);
	BOOL FilterMatches(const FTDCFILTER& filter) { return (filter == m_filter); }

	void EnableMultiSelection(BOOL bEnable);
	void SetUITheme(const CUIThemeFile& theme);
	COLORREF CalcUIBkgndColor() const;

protected:
// Dialog Data
	//{{AFX_DATA(CFilterBar)
	//}}AFX_DATA
	CTDLFilterComboBox			m_cbTaskFilter;
	CTDLFilterDateComboBox		m_cbStartFilter;
	CTDLFilterDateComboBox		m_cbDueFilter;
	CEnEdit						m_eTitleFilter;
	CEnCheckComboBox			m_cbAllocToFilter;
	CEnCheckComboBox			m_cbAllocByFilter;
	CEnCheckComboBox			m_cbCategoryFilter;
	CEnCheckComboBox			m_cbStatusFilter;
	CEnCheckComboBox			m_cbVersionFilter;
	CEnCheckComboBox			m_cbTagFilter;
	CTDLPriorityComboBox		m_cbPriorityFilter;
	CTDLRiskComboBox			m_cbRiskFilter;
	CTDLFilterOptionComboBox	m_cbOptions;
	CDateTimeCtrlEx				m_dtcUserStart, m_dtcUserDue;
	CEnEdit						m_eStartNextNDays, m_eDueNextNDays;

	FTDCFILTER m_filter;
	CMap<CString, LPCTSTR, DWORD, DWORD&> m_mapCustomFlags;
	CDWordArray m_aPriorityColors;
	CTDCAttributeMap m_mapVisibility;
	FTC_VIEW m_nView;
	CBrush m_brUIBack;
	COLORREF m_crUIBack;
	CUIThemeFile m_theme;
	BOOL m_bCustomFilter;
	CString m_sCustomFilter;
	BOOL m_bWantHideParents;
	CWndPromptManager m_mgrPrompts;
	BOOL m_bRefreshBkgndColor;
	HICON m_hUpdateImage;

protected:
	int DoModal() { return -1; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK() { /* do nothing */ }
	virtual void OnCancel() { /* do nothing */ }
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnSelchangeFilter();
	afx_msg void OnSelcancelFilter();
	afx_msg void OnCloseUpOptions();
	afx_msg void OnSelchangeFilter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDateFilter();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR, LRESULT* pResult );	
	afx_msg LRESULT OnEEBtnClick(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	int ReposControls(int nWidth = -1, BOOL bCalcOnly = FALSE);
	BOOL WantShowFilter(TDC_ATTRIBUTE nType);
	void SetVisibleFilters(const CTDCAttributeMap& mapFilters, BOOL bRepos);
	void RefreshUIBkgndBrush();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERBAR_H__FAABD9A1_72C4_4731_B7A4_48251860672C__INCLUDED_)
