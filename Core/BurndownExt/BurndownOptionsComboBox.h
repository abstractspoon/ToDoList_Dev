#if !defined(AFX_BURNDOWNOPTIONSCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_)
#define AFX_BURNDOWNOPTIONSCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BurndownOptionsComboBox.h : header file
//

#include "BurndownEnum.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownOptionsComboBox window

class CBurndownOptionsComboBox : public CComboBox
{
// Construction
public:
	CBurndownOptionsComboBox();
	virtual ~CBurndownOptionsComboBox();

	BOOL SetActiveGraph(BURNDOWN_GRAPH nGraph);
	BURNDOWN_GRAPHOPTION GetSelectedOption() const;
	BOOL SetSelectedOption(BURNDOWN_GRAPHOPTION nOption);

protected:
	BURNDOWN_GRAPHTYPE m_nGraphType;

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBurndownOptionsComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CBurndownOptionsComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BURNDOWNOPTIONSCOMBOBOX_H__97EEAC9A_CC72_4752_9920_BAEA53857B98__INCLUDED_)
