#if !defined(AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
#define AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KanbanAttributeComboBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "KanbanStruct.h"

#include "..\Interfaces\iuiextension.h"

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox window

class CKanbanAttributeComboBox : public CComboBox
{
// Construction
public:
	CKanbanAttributeComboBox(BOOL bShowNone);
	virtual ~CKanbanAttributeComboBox();

	TDC_ATTRIBUTE GetSelectedAttribute() const;
	TDC_ATTRIBUTE GetSelectedAttribute(CString& sCustomAttribID) const;
	BOOL SetSelectedAttribute(TDC_ATTRIBUTE nAttribID, const CString& sCustomAttribID);

	void SetCustomAttributeDefs(const CKanbanCustomAttributeDefinitionArray& aAttribDefs);
	void ShowFixedColumns(TDC_ATTRIBUTE nAttribID);

	void DDX(CDataExchange* pDX, TDC_ATTRIBUTE& value, CString& sCustomAttribID);

protected:
	BOOL m_bShowNone;
	TDC_ATTRIBUTE m_nFixedColumnsAttribID;

	CKanbanCustomAttributeDefinitionArray m_aCustAttribDefs;

protected:
	virtual void PreSubclassWindow();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void BuildCombo();

	TDC_ATTRIBUTE GetFallbackAttribute() const { return (m_bShowNone ? TDCA_NONE : TDCA_STATUS); }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KANBANATTRIBUTECOMBOBOX_H__FDF3A712_EBBE_4050_9DAD_D224CD6C6C6E__INCLUDED_)
