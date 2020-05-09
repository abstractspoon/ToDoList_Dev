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

// Attributes
public:
	FILTER_SHOW GetSelectedFilter() const;
	FILTER_SHOW GetSelectedFilter(CString& sAdvanced) const;
	BOOL SelectAdvancedFilter(FILTER_SHOW nFilter);
	BOOL SelectAdvancedFilter(const CString& sAdvanced);
	
	void AddAdvancedFilters(const CStringArray& aFilters, LPCTSTR szAdvancedSel = NULL);
	const CStringArray& GetAdvancedFilterNames() const;
	void RemoveAdvancedFilters();
	void ShowDefaultFilters(BOOL bShow);

// Operations
protected:
	BOOL m_bShowDefaultFilters;
	CStringArray m_aAdvancedFilterNames;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFilterComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	

// Implementation
public:
	virtual ~CTDLFilterComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLFilterComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void FillCombo();
	void RefillCombo(LPCTSTR szAdvancedSel = NULL);
	void RestoreSelection(FILTER_SHOW nFilter, LPCTSTR szAdvanced = NULL);
	int AddDefaultFilterItem(int nItem);

	static CString FormatAdvancedFilterDisplayString(int nFilter, const CString& sFilter, BOOL bIncAdvancedLabel = TRUE);
	static BOOL ExtractAdvancedFilterName(const CString& sDisplay, CString& sFilter);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILTERCOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
