// KanbanAttributeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanAttributeComboBox.h"
#include "Kanbanenum.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox

CKanbanAttributeComboBox::CKanbanAttributeComboBox() 
	: m_bShowCustomAttrib(TRUE), m_bShowFixedColumns(FALSE)
{
}

CKanbanAttributeComboBox::~CKanbanAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanAttributeComboBox, CComboBox)
	//{{AFX_MSG_MAP(CKanbanAttributeComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanAttributeComboBox message handlers

void CKanbanAttributeComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();

	BuildCombo();
}

void CKanbanAttributeComboBox::BuildCombo()
{
	I_ATTRIBUTE nSel = GetSelectedAttribute();
	ResetContent();

	CDialogHelper::AddString(*this, CEnString(IDS_STATUSATTRIB), IA_STATUS);
	CDialogHelper::AddString(*this, CEnString(IDS_ALLOCTOATTRIB), IA_ALLOCTO);
	CDialogHelper::AddString(*this, CEnString(IDS_ALLOCBYATTRIB), IA_ALLOCBY);
	CDialogHelper::AddString(*this, CEnString(IDS_CATEGORYATTRIB), IA_CATEGORY);
	CDialogHelper::AddString(*this, CEnString(IDS_PRIORITYATTRIB), IA_PRIORITY);
	CDialogHelper::AddString(*this, CEnString(IDS_RISKATTRIB), IA_RISK);
	CDialogHelper::AddString(*this, CEnString(IDS_VERSIONATTRIB), IA_VERSION);
	CDialogHelper::AddString(*this, CEnString(IDS_TAGSATTRIB), IA_TAGS);

	if (m_bShowFixedColumns)
		CDialogHelper::AddString(*this, CEnString(IDS_FIXEDCOLUMNS), IA_FIXEDCOLUMNS);

	if (m_bShowCustomAttrib)
		CDialogHelper::AddString(*this, CEnString(IDS_CUSTOMATTRIB), IA_CUSTOMATTRIB);

	SetSelectedAttribute(nSel);
}

I_ATTRIBUTE CKanbanAttributeComboBox::GetSelectedAttribute() const
{
	return (I_ATTRIBUTE)CDialogHelper::GetSelectedItemData(*this);
}

BOOL CKanbanAttributeComboBox::SetSelectedAttribute(I_ATTRIBUTE nAttrib)
{
	return (CB_ERR != CDialogHelper::SelectItemByData(*this, (DWORD)nAttrib));
}

void CKanbanAttributeComboBox::ShowCustomAttribute(BOOL bShow)
{
	if (!m_bShowCustomAttrib != !bShow) // normalise
	{
		m_bShowCustomAttrib = bShow;
		BuildCombo();
	}
}

void CKanbanAttributeComboBox::ShowFixedColumns(BOOL bShow)
{
	if (!m_bShowFixedColumns != !bShow) // normalise
	{
		m_bShowFixedColumns = bShow;
		BuildCombo();
	}
}
