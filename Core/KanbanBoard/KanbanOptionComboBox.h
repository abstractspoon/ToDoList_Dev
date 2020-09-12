#if !defined(AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_)
#define AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterOptionComboBox.h : header file
//

#include "..\shared\checkcombobox.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterOptionComboBox window

class CKanbanOptionComboBox : public CCheckComboBox
{
// Construction
public:
	CKanbanOptionComboBox();
	virtual ~CKanbanOptionComboBox();

	BOOL Initialise(DWORD dwOptions);
	DWORD GetSelectedOptions() const;
	void SetSelectedOptions(DWORD dwOptions);
	BOOL HasSelectedOption(DWORD dwOption) const;

// Attributes
protected:
	DWORD m_dwOptions;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterOptionComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilterOptionComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void OnCheckChange(int nIndex);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_)
