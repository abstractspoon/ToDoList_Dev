#if !defined(AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_)
#define AFX_FILTEROPTIONCOMBOBOX_H__9EE08B57_87B6_43D9_8CA7_F47A30CBF66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterOptionComboBox.h : header file
//

#include "tdcstruct.h"

#include "..\shared\checkcombobox.h"

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CFilterOptionComboBox window

class CTDLFilterOptionComboBox : public CCheckComboBox
{
// Construction
public:
	CTDLFilterOptionComboBox();
	virtual ~CTDLFilterOptionComboBox();

// Operations
public:
	void Initialize(const TDCFILTER& filter, const CTDCAttributeMap& mapVisibleFilters, BOOL bMultiSelection);
	void Initialize(DWORD dwFlags, DWORD dwOptions = 0);

	DWORD GetSelectedOptions() const;
	void SetSelectedOptions(DWORD dwOptions);

// Attributes
protected:
	DWORD m_dwOptions;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterOptionComboBox)
	//}}AFX_VIRTUAL

// Implementation
// public:

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
