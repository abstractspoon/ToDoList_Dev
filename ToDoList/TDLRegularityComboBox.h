#if !defined(AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_)
#define AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLRegularityComboBox.h : header file
//

#include "..\Interfaces\iTaskList.h"

/////////////////////////////////////////////////////////////////////////////
// CTDLRegularityComboBox window

class CTDLRegularityComboBox : public CComboBox
{
// Construction
public:
	CTDLRegularityComboBox();
	virtual ~CTDLRegularityComboBox();

	TDC_REGULARITY GetSelectedRegularity() const;
	int SetSelectedRegularity(TDC_REGULARITY nRegularity);

	static CString GetRegularity(TDC_REGULARITY nRegularity);

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLRegularityComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTDLRegularityComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLREGULARITYCOMBOBOX_H__FEBCAD06_B7B7_46A9_ABA2_911DF0E1AF55__INCLUDED_)
