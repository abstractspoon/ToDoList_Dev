#if !defined(AFX_KANBANGROUPBYCOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
#define AFX_KANBANGROUPBYCOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanAttributeComboBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "KanbanAttributeComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox window

class CKanbanGroupByComboBox : public CKanbanAttributeComboBox
{
// Construction
public:
	CKanbanGroupByComboBox();
	virtual ~CKanbanGroupByComboBox();

	void ShowFixedColumns(BOOL /*bShow*/) { m_bShowFixedColumns = FALSE; } // never
	void ExcludeAttribute(TDC_ATTRIBUTE nAttrib);

// Attributes
protected:
	TDC_ATTRIBUTE m_nExcludeAttribID;
	//CString m_sExcludeCustomAttribID;

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanAttributeComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void BuildCombo();
	virtual TDC_ATTRIBUTE GetFallbackAttribute() const { return TDCA_NONE; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANGROUPBYCOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
