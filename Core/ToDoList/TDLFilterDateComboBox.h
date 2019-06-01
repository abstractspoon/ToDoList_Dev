#if !defined(AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
#define AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLFilterComboBox.h : header file
//

#include "tdcenum.h"
#include "..\shared\tabbedcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox window

class CTDLFilterDateComboBox : public CTabbedComboBox
{
// Construction
public:
	CTDLFilterDateComboBox(int nNextNDays = 7);

// Operations
public:
	FILTER_DATE GetSelectedFilter() const;
	BOOL SelectFilter(FILTER_DATE nFilter);
	void SetNextNDays(int nDays);

protected:
	int m_nNextNDays;
	BOOL m_bRebuildingCombo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLFilterDateComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDLFilterDateComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLFilterDateComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg BOOL OnReflectSelChange();

	DECLARE_MESSAGE_MAP()

protected:
	void FillCombo();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
