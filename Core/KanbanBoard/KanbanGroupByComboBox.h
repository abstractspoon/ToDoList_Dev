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
public:
	CKanbanGroupByComboBox();
	virtual ~CKanbanGroupByComboBox();

	void ShowFixedColumns(TDC_ATTRIBUTE /*nAttribID*/) { m_nFixedColumnsAttribID = TDCA_NONE; } // never
	void ExcludeAttribute(TDC_ATTRIBUTE nAttribID);

protected:
	TDC_ATTRIBUTE m_nExcludeAttribID;

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void BuildCombo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANGROUPBYCOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
