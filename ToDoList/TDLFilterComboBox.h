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
	FILTER_SHOW GetSelectedFilter(CString& sCustom) const;
	BOOL SelectFilter(FILTER_SHOW nFilter);
	BOOL SelectFilter(const CString& sCustom);
	
	void AddCustomFilters(const CStringArray& aFilters, LPCTSTR szCustomSel = NULL);
	int GetCustomFilters(CStringArray& aFilters) const;
	void RemoveCustomFilters();
	void ShowDefaultFilters(BOOL bShow);

// Operations
protected:
	BOOL m_bShowDefaultFilters;
	CStringArray m_aCustomFilters;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFilterComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList);	

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
	void RefillCombo(LPCTSTR szCustomSel = NULL);
	void RestoreSelection(FILTER_SHOW nFilter, LPCTSTR szCustom = NULL);
	int AddDefaultFilterItem(int nItem);

	static CString FormatCustomFilterDisplayString(int nFilter, const CString& sFilter, BOOL bIncCustomLabel = TRUE);
	static CString ExtractCustomFilterName(const CString& sDisplay);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILTERCOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
