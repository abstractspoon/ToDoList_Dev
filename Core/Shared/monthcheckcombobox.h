#if !defined(AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
#define AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// monthcombobox.h : header file
//

#include "checkcombobox.h"

/////////////////////////////////////////////////////////////////////////////

// class CMonthCheckComboBox;

// void DDX_Months(CDataExchange* pDX, CMonthCheckComboBox& combo, DWORD& dwMonths);

/////////////////////////////////////////////////////////////////////////////
// CMonthCheckComboBox window

class CMonthCheckComboBox : public CCheckComboBox
{
// Construction
public:
	CMonthCheckComboBox();
	virtual ~CMonthCheckComboBox();

	DWORD GetSelectedMonths() const;
	int SetSelectedMonths(DWORD dwMonths);

	void DDX(CDataExchange* pDX, DWORD& value);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonthCheckComboBox)
// protected:
// 	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
// public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMonthCheckComboBox)
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	virtual void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
