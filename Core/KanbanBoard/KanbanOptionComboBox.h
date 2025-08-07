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
public:
	CKanbanOptionComboBox();
	virtual ~CKanbanOptionComboBox();

	BOOL Initialise(DWORD dwOptions);
	DWORD GetSelectedOptions() const;
	void SetSelectedOptions(DWORD dwOptions);
	BOOL HasSelectedOption(DWORD dwOption) const;

	DWORD GetOptionMask() const;

protected:
	DWORD m_dwOptions;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	void OnCheckChange(int nIndex);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_)
