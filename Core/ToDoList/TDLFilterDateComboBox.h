#if !defined(AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
#define AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTDLFilterDateComboBox.h : header file
//

#include "tdcenum.h"

#include "..\shared\tabbedcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox window

class CTDLFilterDateComboBox : public CTabbedComboBox
{
public:
	CTDLFilterDateComboBox(int nNextNDays = 7);
	virtual ~CTDLFilterDateComboBox();

public:
	FILTER_DATE GetSelectedFilter() const;
	BOOL SelectFilter(FILTER_DATE nFilter);
	void SetNextNDays(int nDays);

protected:
	int m_nNextNDays;
	BOOL m_bRebuildingCombo;

protected:
	afx_msg BOOL OnReflectSelChange();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void BuildCombo();
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLFILTERDATECOMBOBOX_H__5B0A3DAB_18FF_456E_9BD6_1B0E60EDC898__INCLUDED_)
