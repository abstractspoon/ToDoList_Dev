#if !defined(AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_)
#define AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// weekdaychecklistbox.h : header file
//

#include "checklistboxex.h"

/////////////////////////////////////////////////////////////////////////////
// CWeekdayCheckListBox window

class CDayOfWeekCheckListBox : public CCheckListBoxEx
{
// Construction
public:
	CDayOfWeekCheckListBox();
	virtual ~CDayOfWeekCheckListBox();

	DWORD GetChecked() const;
	DWORD GetChecked(int& nNumChecked) const;
	void SetChecked(DWORD dwChecked);

protected:
	mutable DWORD m_dwChecked;

protected:
	virtual void PreSubclassWindow();

protected:
	// Generated message map functions
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnInitListbox(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_)
