#if !defined(AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
#define AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanAttributeComboBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox window

class CKanbanAttributeComboBox : public CComboBox
{
// Construction
public:
	CKanbanAttributeComboBox();

	IUI_ATTRIBUTE GetSelectedAttribute() const;
	BOOL SetSelectedAttribute(IUI_ATTRIBUTE nAttrib);

	void ShowCustomAttribute(BOOL bShow = TRUE);
	void ShowFixedColumns(BOOL bShow = TRUE);

// Attributes
protected:
	BOOL m_bShowCustomAttrib;
	BOOL m_bShowFixedColumns;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKanbanAttributeComboBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKanbanAttributeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKanbanAttributeComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
