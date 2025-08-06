#if !defined(AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
#define AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// monthcombobox.h : header file
//

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

protected:
	virtual void PreSubclassWindow();

protected:
	// Generated message map functions
	DECLARE_MESSAGE_MAP()

	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONTHCOMBOBOX_H__4E52F369_8A97_4166_8EA2_32292F13A09A__INCLUDED_)
