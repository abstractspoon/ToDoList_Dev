// KanbanAttributeComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanGroupByComboBox.h"
#include "Kanbanenum.h"

#include "..\shared\dialoghelper.h"
#include "..\shared\enstring.h"
#include "..\shared\localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKanbanGroupByComboBox

CKanbanGroupByComboBox::CKanbanGroupByComboBox() 
	: 
	CKanbanAttributeComboBox(TRUE), // include <none>
	m_nExcludeAttribID(TDCA_NONE)
{
}

CKanbanGroupByComboBox::~CKanbanGroupByComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanGroupByComboBox, CKanbanAttributeComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanGroupByComboBox message handlers

void CKanbanGroupByComboBox::ExcludeAttribute(TDC_ATTRIBUTE nAttribID)
{
	ASSERT(nAttribID != TDCA_FIXEDCOLUMNS);
	ASSERT(!KBUtils::IsCustomAttribute(nAttribID) || m_aCustAttribDefs.GetSize());

	if (nAttribID == m_nExcludeAttribID)
		return;

	m_nExcludeAttribID = nAttribID;
	BuildCombo();
}

void CKanbanGroupByComboBox::BuildCombo()
{
	// Override base class's selection restoration because
	// of our extra item
	CString sSelCustID;
	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute(sSelCustID);

	CKanbanAttributeComboBox::BuildCombo();

	// Remove excluded attribute
	if (m_nExcludeAttribID != TDCA_NONE)
	{
		int nExclude = CDialogHelper::FindItemByDataT(*this, m_nExcludeAttribID);

		if (nExclude != CB_ERR)
			DeleteString(nExclude);
	}

	// Add extra items
	CDialogHelper::AddStringT(*this, CEnString(IDS_DISPLAY_RECURRENCE), TDCA_RECURRENCE);

	// Restore selection
	if (!SetSelectedAttribute(nSelAttrib, sSelCustID))
		SetSelectedAttribute(TDCA_NONE, _T(""));
}

