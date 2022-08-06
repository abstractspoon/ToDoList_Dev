#if !defined(AFX_GANTTSNAPCOMBOBOX_H__11DE1788_CA96_4293_B5F6_9A2297BB6A6D__INCLUDED_)
#define AFX_GANTTSNAPCOMBOBOX_H__11DE1788_CA96_4293_B5F6_9A2297BB6A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttSnapComboBox.h : header file
//

#include "GanttEnum.h"

/////////////////////////////////////////////////////////////////////////////

class CGanttCtrl;

/////////////////////////////////////////////////////////////////////////////
// CGanttSnapComboBox window

class CGanttSnapComboBox : public CComboBox
{
// Construction
public:
	CGanttSnapComboBox();

	void Rebuild(GTLC_MONTH_DISPLAY nDisplay, GTLC_SNAPMODE nSnap);

	BOOL SelectMode(GTLC_SNAPMODE nSnap);
	GTLC_SNAPMODE GetSelectedMode() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttSnapComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGanttSnapComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGanttSnapComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTSNAPCOMBOBOX_H__11DE1788_CA96_4293_B5F6_9A2297BB6A6D__INCLUDED_)
