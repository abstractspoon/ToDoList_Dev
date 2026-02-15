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
public:
	CTDLFilterDlg(FILTER_TITLE nTitleFilter,
				  BOOL bMultiSelFilters,
				  BOOL bShowDefaultFilters,
				  const CStringArray& aAdvancedFilterNames,
				  const CFilteredToDoCtrl& tdc,
				  const CDWordArray& aPriorityColors,
				  int nNumPriorityRiskLevels,
				  BOOL bISODateFormat,
				  CWnd* pParent = NULL);

	FILTER_SHOW GetFilter(TDCFILTER& filter, CString& sCustom, DWORD& dwCustomFlags) const;

protected:
	CEnCheckComboBox			m_cbCategoryFilter;
	CEnCheckComboBox			m_cbStatusFilter;
	CEnCheckComboBox			m_cbAllocToFilter;
	CEnCheckComboBox			m_cbAllocByFilter;
	CEnCheckComboBox			m_cbVersionFilter;
	CEnCheckComboBox			m_cbTagFilter;
	CTDLFilterComboBox			m_cbShowFilter;
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnClearfilter();
	afx_msg void OnSelchangeFiltercombo();
	afx_msg void OnSelchangeFilter();
	afx_msg void OnSelcancelFilter();
	afx_msg void OnSelchangeStartNextNDays();
	afx_msg void OnSelchangeDueNextNDays();
	afx_msg void OnSelchangeDatecombo();
	DECLARE_MESSAGE_MAP()
		
	void EnableDisableControls();
};

#endif // !defined(AFX_FILTERDLG_H__9BFA276C_9BD6_4B23_A714_BB9D059CC2EB__INCLUDED_)
