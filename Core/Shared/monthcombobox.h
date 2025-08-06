#if !defined(AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
#define AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// monthcombobox.h : header file
//

/////////////////////////////////////////////////////////////////////////////

// class CMonthComboBox;

// void DDX_Month(CDataExchange* pDX, CMonthComboBox& combo, int& nMonth);

/////////////////////////////////////////////////////////////////////////////
// CMonthComboBox window

class CMonthComboBox : public CComboBox
{
// Construction
public:
	CMonthComboBox();
	virtual ~CMonthComboBox();

	int GetSelectedMonth() const;
	int SetSelectedMonth(int nMonth);

	void DDX(CDataExchange* pDX, int& nMonth);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonthComboBox)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
// public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMonthComboBox)
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
