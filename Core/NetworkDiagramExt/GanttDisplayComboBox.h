#if !defined(AFX_GANTTDISPLAYCOMBOBOX_H__C029640F_BF50_42C2_B384_CD77B89D7F32__INCLUDED_)
#define AFX_GANTTDISPLAYCOMBOBOX_H__C029640F_BF50_42C2_B384_CD77B89D7F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GanttDisplayComboBox.h : header file
//

#include "GanttEnum.h"

#include "..\Shared\tabbedcombobox.h"

/////////////////////////////////////////////////////////////////////////////

class CGanttCtrl;

/////////////////////////////////////////////////////////////////////////////
// CGanttDisplayComboBox window

class CGanttDisplayComboBox : public CTabbedComboBox
{
// Construction
public:
	CGanttDisplayComboBox();
	virtual ~CGanttDisplayComboBox();

	void UpdateDisplayOptions(const CGanttCtrl& ctrl);

	BOOL SelectDisplay(GTLC_MONTH_DISPLAY nDisplay);
	GTLC_MONTH_DISPLAY GetSelectedDisplay() const;

protected:
	GTLC_MONTH_DISPLAY m_nMinEnabledDisplay;    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGanttDisplayComboBox)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CGanttDisplayComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg BOOL OnSelChange();
	afx_msg BOOL OnSelEndOK();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void FixupTabOffsets(const CStringArray& aItems, CStringArray& aOptions, CStringArray& aExamples) const;
	BOOL IsEnabledDisplay(GTLC_MONTH_DISPLAY nDisplay) const;
	GTLC_MONTH_DISPLAY GetSelectedDisplayRaw() const;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GANTTDISPLAYCOMBOBOX_H__C029640F_BF50_42C2_B384_CD77B89D7F32__INCLUDED_)
