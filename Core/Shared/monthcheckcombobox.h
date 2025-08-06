#if !defined(AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
#define AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// monthcombobox.h : header file
//

#include "checkcombobox.h"

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

protected:
	// Generated message map functions
	DECLARE_MESSAGE_MAP()

	virtual void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTHCHECKCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
