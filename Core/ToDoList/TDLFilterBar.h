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
#include "tdlregularitycombobox.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\encheckcombobox.h"
#include "..\shared\enedit.h"
#include "..\shared\wndprompt.h"
#include "..\shared\datetimectrlex.h"
#include "..\shared\icon.h"

#include "..\Interfaces\uithemefile.h"

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

	FILTER_SHOW GetFilter() const;
	FILTER_SHOW GetFilter(TDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const;
	FILTER_SHOW GetFilter(CString& sCustom) const;

	BOOL SelectFilter(int nFilter);
	int GetSelectedFilter() const;

	void AddAdvancedFilters(const CStringArray& aFilters);
	const CStringArray& GetAdvancedFilterNames() const;
	BOOL SetAdvancedFilterIncludesDoneTasks(const CString& sCustom, BOOL bIncDone);

	void ShowDefaultFilters(BOOL bShow);
	void RefreshFilterControls(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID = TDCA_ALL);
	void UpdateDropListData(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID);
	void SetFilterLabelAlignment(BOOL bLeft);
	void SetPriorityColors(const CDWordArray& aColors);
	int CalcHeight(int nWidth);
	BOOL FilterMatches(const TDCFILTER& filter) { return (filter == m_filter); }
	BOOL SetTitleFilterOption(FILTER_TITLE nOption);
	void ClearCheckboxHistory();

	void EnableMultiSelection(BOOL bEnable);
	void SetUITheme(const CUIThemeFile& theme);
	COLORREF CalcUIBkgndColor() const;
	BOOL CanPasteText() const; // into focused control

protected:
// Dialog Data
	//{{AFX_DATA(CFilterBar)
	//}}AFX_DATA
	CTDLFilterComboBox m_cbTaskFilter;
	CTDLFilterDateComboBox m_cbStartFilter;
	CTDLFilterDateComboBox m_cbDueFilter;
	CEnEdit m_eTitleFilter;
	CEnCheckComboBox m_cbAllocToFilter;
	CEnCheckComboBox m_cbAllocByFilter;
	CEnCheckComboBox m_cbCategoryFilter;
	CEnCheckComboBox m_cbStatusFilter;
	CEnCheckComboBox m_cbVersionFilter;
	CEnCheckComboBox m_cbTagFilter;
	CTDLPriorityComboBox m_cbPriorityFilter;
	CTDLRiskComboBox m_cbRiskFilter;
	CTDLRegularityComboBox m_cbRecurrence;
	CTDLFilterOptionComboBox m_cbOptions;
	CDateTimeCtrlEx m_dtcUserStart, m_dtcUserDue;
	CEnEdit m_eStartNextNDays, m_eDueNextNDays;
	CTDCCustomControlArray m_aCustomControls;
		
	CMap<CString, LPCTSTR, DWORD, DWORD&> m_mapCustomFlags;
	CTDCAttributeMap m_mapVisibility;
	CTDCCustomAttribDefinitionArray m_aCustomAttribDefs;

	TDCFILTER m_filter;
	FILTER_TITLE m_nTitleFilter;
	CBrush m_brUIBack;
	COLORREF m_crUIBack;
	CUIThemeFile m_theme;
	CString m_sAdvancedFilter;
	CWndPromptManager m_mgrPrompts;
	CIcon m_iconUpdateBtn;
	BOOL m_bMultiSelection;

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
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFilterBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	afx_msg void OnSelchangeFilter();
	afx_msg void OnSelchangeFilterAttribute();
	afx_msg void OnSelcancelAllocToFilter();
	afx_msg void OnSelcancelAllocByFilter();
	afx_msg void OnSelcancelCategoryFilter();
	afx_msg void OnSelcancelStatusFilter();
	afx_msg void OnSelcancelTagFilter();
	afx_msg void OnSelcancelVersionFilter();
	afx_msg void OnCloseUpOptions();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeDueDateFilter();
	afx_msg void OnSelchangeStartDateFilter();
	afx_msg void OnChangeDateFilter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult );	
	afx_msg LRESULT OnEEBtnClick(WPARAM wp, LPARAM lp);
	afx_msg void OnCustomAttributeSelchangeFilter(UINT nCtrlID);
	afx_msg void OnCustomAttributeSelcancelFilter(UINT nCtrlID);
	afx_msg void OnCustomAttributeChangeDateFilter(UINT nCtrlID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	// pseudo-handlers
	void OnCustomAttributeChangeFilter(CUSTOMATTRIBCTRLITEM& ctrl);
	void OnSelchangeDateFilter(FILTER_DATE nPrevFilter, const CTDLFilterDateComboBox& combo);

protected:
	void RemoveAdvancedFilters();
	int ReposControls(int nWidth = -1, BOOL bCalcOnly = FALSE);
	BOOL WantShowFilter(TDC_ATTRIBUTE nType) const;
	void RefreshUIBkgndBrush();
	int GetControls(CTDCControlArray& aControls) const;
	void UpdateCustomControls(const CFilteredToDoCtrl& tdc, TDC_ATTRIBUTE nAttribID);
	void NotifyParentFilterChange();
	void RebuildOptionsCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERBAR_H__FAABD9A1_72C4_4731_B7A4_48251860672C__INCLUDED_)
