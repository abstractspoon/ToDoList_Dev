#if !defined(AFX_TDLFILTERCOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
#define AFX_TDLFILTERCOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLFilterComboBox.h : header file
//

#include "tdcenum.h"
#include "..\shared\tabbedcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterComboBox window

class CTDLFilterComboBox : public CTabbedComboBox
{
// Construction
public:
	CTDLFilterComboBox();
	virtual ~CTDLFilterComboBox();

// Attributes
public:
	FILTER_SHOW GetSelectedFilter() const;
	FILTER_SHOW GetSelectedFilter(CString& sAdvFilter) const;
	BOOL SelectFilter(FILTER_SHOW nShow, LPCTSTR szAdvFilter = NULL);
	
	void SetAdvancedFilters(const CStringArray& aFilters, LPCTSTR szAdvancedSel = NULL);
	const CStringArray& AdvancedFilterNames() const;
	BOOL HasAdvancedFilter(const CString& sAdvFilter) const;

	void ShowDefaultFilters(BOOL bShow);

// Operations
protected:
	BOOL m_bShowDefaultFilters;
	CStringArray m_aAdvancedFilterNames;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFilterComboBox)
// protected:
// 	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
// protected:
	//{{AFX_MSG(CTDLFilterComboBox)
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void BuildCombo();

	void RebuildCombo(LPCTSTR szAdvancedSel = NULL);
	int AddDefaultFilterItem(int nItem);
	int GetDefaultFilterCount() const;

	static CString FormatAdvancedFilterDisplayString(int nFilter, const CString& sFilter);
	static BOOL ExtractAdvancedFilterName(const CString& sDisplay, CString& sFilter);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILTERCOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
