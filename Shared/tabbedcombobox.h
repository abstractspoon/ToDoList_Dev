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
// Construction
public:
	CTabbedComboBox(int nTabStops = 20);

// Attributes
protected:
	const int TABSTOPS;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabbedComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabbedComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
								DWORD dwItemData, const CString& sItem, BOOL bList);	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDCOMBOBOX_H__17C6DAEB_521B_4C43_BC64_C01F0AD655C3__INCLUDED_)
