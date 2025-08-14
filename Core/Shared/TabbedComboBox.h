#if !defined(AFX_TABBEDCOMBOBOX_H__17C6DAEB_521B_4C43_BC64_C01F0AD655C3__INCLUDED_)
#define AFX_TABBEDCOMBOBOX_H__17C6DAEB_521B_4C43_BC64_C01F0AD655C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tabbedcombobox.h : header file
//

#include "ownerdrawcomboboxbase.h"

/////////////////////////////////////////////////////////////////////////////
// CTabbedComboBox window

class CTabbedComboBox : public COwnerdrawComboBoxBase
{
public:
	CTabbedComboBox(int nTabStops = 0);
	virtual ~CTabbedComboBox();

protected:
	const int TABSTOPS;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText);	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDCOMBOBOX_H__17C6DAEB_521B_4C43_BC64_C01F0AD655C3__INCLUDED_)
