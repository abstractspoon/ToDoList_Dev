#if !defined(AFX_BURNDOWNTRENDCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_)
#define AFX_BURNDOWNTRENDCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownTrendComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBurndownTrendComboBox window

class CBurndownTrendComboBox : public CComboBox
{
// Construction
public:
	CBurndownTrendComboBox();
	virtual ~CBurndownTrendComboBox();

	BOOL Initialise();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurndownTrendComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CBurndownTrendComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNTRENDCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_)
