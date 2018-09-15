// KanbanCustomAttributeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "KanbanBoard.h"
#include "KanbanCustomAttributeComboBox.h"

#include "..\shared\Misc.h"
#include "..\shared\DialogHelper.h"
#include "..\shared\Localizer.h"

// CKanbanCustomAttributeComboBox

IMPLEMENT_DYNAMIC(CKanbanCustomAttributeComboBox, CComboBox)

CKanbanCustomAttributeComboBox::CKanbanCustomAttributeComboBox()
{

}

CKanbanCustomAttributeComboBox::~CKanbanCustomAttributeComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanCustomAttributeComboBox, CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CKanbanCustomAttributeComboBox message handlers

int CKanbanCustomAttributeComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	BuildCombo();

	return 0;
}


void CKanbanCustomAttributeComboBox::PreSubclassWindow()
{
	BuildCombo();

	CComboBox::PreSubclassWindow();
}

void CKanbanCustomAttributeComboBox::SetAttributeDefinitions(const CKanbanCustomAttributeDefinitionArray& aAttribDefs)
{
	if (Misc::MatchAllT(m_aCustAttribDefs, aAttribDefs, FALSE))
		return;

	m_aCustAttribDefs.Copy(aAttribDefs);
	
	if (GetSafeHwnd())
	{
		ResetContent();
		BuildCombo();
	}
}

CString CKanbanCustomAttributeComboBox::GetSelectedAttributeID() const
{
	int nSel = ((int)CDialogHelper::GetSelectedItemData(*this) - 1);

	if ((nSel < 0) || (nSel >= m_aCustAttribDefs.GetSize()))
		return _T("");

	// return
	return m_aCustAttribDefs[nSel].sAttribID;
}

BOOL CKanbanCustomAttributeComboBox::SetSelectedAttributeID(const CString& sAttribID)
{
	int nSel = m_aCustAttribDefs.FindDefinition(sAttribID);

	if (nSel == -1)
		return FALSE;

	return (CDialogHelper::SelectItemByData(*this, (nSel + 1)) != CB_ERR);
}

void CKanbanCustomAttributeComboBox::BuildCombo()
{
	// Only if empty
	if (!GetSafeHwnd() || GetCount())
		return;

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	CString sCurSel = GetSelectedAttributeID();

	for (int nCust = 0; nCust < m_aCustAttribDefs.GetSize(); nCust++)
	{
		const KANBANCUSTOMATTRIBDEF& kcaDef = m_aCustAttribDefs.GetData()[nCust];

		// Item id is '+1' so that '0' means unselected
		int nItem = CDialogHelper::AddString(*this, kcaDef.sAttribName, (nCust + 1));
		ASSERT(nItem != CB_ERR);
	}

	if (sCurSel.IsEmpty())
		SetCurSel(0);
	else
		SetSelectedAttributeID(sCurSel);
}
