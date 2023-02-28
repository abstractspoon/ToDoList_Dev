#if !defined(AFX_FILTERDLG_H__9BFA276C_9BD6_4B23_A714_BB9D059CC2EB__INCLUDED_)
#define AFX_FILTERDLG_H__9BFA276C_9BD6_4B23_A714_BB9D059CC2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDlg.h : header file
//

#include "filteredtodoctrl.h"
#include "tdlfiltercombobox.h"
#include "tdlfilteroptioncombobox.h"
#include "tdlprioritycombobox.h"
#include "tdlriskcombobox.h"
#include "tdlfilterdatecombobox.h"
#include "tdlregularitycombobox.h"
#include "TDLDialog.h"

#include "..\shared\encheckcombobox.h"
#include "..\shared\wndprompt.h"
#include "..\shared\datetimectrlex.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDlg dialog

class CTDLFilterDlg : public CTDLDialog
{
// Construction
public:
	CTDLFilterDlg(FILTER_TITLE nTitleFilter, 
				BOOL bMultiSelFilters, 
				const CStringArray& aAdvancedFilterNames, 
				const CFilteredToDoCtrl& tdc, 
				const CDWordArray& aPriorityColors,
				CWnd* pParent = NULL);

	FILTER_SHOW GetFilter(TDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLFilterDlg)
	enum { IDD = IDD_FILTER_DIALOG };
	//}}AFX_DATA
	CEnCheckComboBox			m_cbCategoryFilter;
	CEnCheckComboBox			m_cbStatusFilter;
	CEnCheckComboBox			m_cbAllocToFilter;
	CEnCheckComboBox			m_cbAllocByFilter;
	CEnCheckComboBox			m_cbVersionFilter;
	CEnCheckComboBox			m_cbTagFilter;
	CTDLFilterComboBox			m_cbTaskFilter;
	CTDLFilterDateComboBox		m_cbStartFilter;
	CTDLFilterDateComboBox		m_cbDueFilter;
	CTDLPriorityComboBox		m_cbPriorityFilter;
	CTDLRiskComboBox			m_cbRiskFilter;
	CTDLRegularityComboBox		m_cbRecurrence;
	CDateTimeCtrlEx				m_dtcUserStart, m_dtcUserDue;
	CEnEdit						m_eStartNextNDays, m_eDueNextNDays;
	CTDLFilterOptionComboBox	m_cbOptions;

	TDCFILTER m_filter;
	FILTER_TITLE m_nTitleFilter;
	TDCAUTOLISTDATA m_tldListData;
	CString m_sAdvancedFilter;
	DWORD m_dwCustomFlags;
	CStringArray m_aAdvancedFilterNames;
	CWndPromptManager m_mgrPrompts;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearfilter();
	afx_msg void OnSelchangeFiltercombo();
	//}}AFX_MSG
	afx_msg void OnSelchangeFilter();
	afx_msg void OnSelcancelFilter();
	afx_msg void OnSelchangeStartNextNDays();
	afx_msg void OnSelchangeDueNextNDays();
	afx_msg void OnSelchangeDatecombo();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR, LRESULT* pResult );	
	DECLARE_MESSAGE_MAP()
		
	void EnableDisableControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDLG_H__9BFA276C_9BD6_4B23_A714_BB9D059CC2EB__INCLUDED_)
