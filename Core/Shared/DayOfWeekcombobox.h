#if !defined(AFX_WEEKDAYCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
#define AFX_WEEKDAYCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeekdayComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeekdayComboBox window

class CDayOfWeekComboBox : public CComboBox
{
// Construction
public:
	CDayOfWeekComboBox();
	virtual ~CDayOfWeekComboBox();

// Attributes
// public:

// Operations
// public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeekdayComboBox)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
// public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CWeekdayComboBox)
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WeekdayComboBox_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
