#if !defined(AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_)
#define AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// weekdaychecklistbox.h : header file
//

#include "..\Shared\checklistboxex.h"

/////////////////////////////////////////////////////////////////////////////
// CWeekdayCheckListBox window

class CDayOfWeekCheckListBox : public CCheckListBoxEx
{
// Construction
public:
	CDayOfWeekCheckListBox();

	DWORD GetChecked() const;
	DWORD GetChecked(int& nNumChecked) const;
	void SetChecked(DWORD dwChecked);

// Attributes
protected:
	mutable DWORD m_dwChecked;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeekdayCheckListBox)
	//}}AFX_VIRTUAL
	virtual void PreSubclassWindow();

// Implementation
public:
	virtual ~CDayOfWeekCheckListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWeekdayCheckListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnInitListbox(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	static DWORD MapDOWToDay(int nDow);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEEKDAYCHECKLISTBOX_H__C4875815_07FD_4E3B_8002_99F9EC366329__INCLUDED_)
