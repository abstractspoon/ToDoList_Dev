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
	CKanbanAttributeComboBox(),
	m_nExcludeAttribID(TDCA_NONE)
{
}

CKanbanGroupByComboBox::~CKanbanGroupByComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanGroupByComboBox, CKanbanAttributeComboBox)
	//{{AFX_MSG_MAP(CKanbanGroupByComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanbanGroupByComboBox message handlers

void CKanbanGroupByComboBox::ExcludeAttribute(TDC_ATTRIBUTE nAttrib)
{
	ASSERT(nAttrib != TDCA_FIXEDCOLUMNS);
	ASSERT(!KBUtils::IsCustomAttribute(nAttrib) || m_aCustAttribDefs.GetSize());

	if (nAttrib == m_nExcludeAttribID)
		return;

	m_nExcludeAttribID = nAttrib;
	BuildCombo();
}

void CKanbanGroupByComboBox::BuildCombo()
{
	// Override base class's selection restoration because
	// our default is 'none'
	CString sSelCustID;
	TDC_ATTRIBUTE nSelAttrib = GetSelectedAttribute(sSelCustID);

	CKanbanAttributeComboBox::BuildCombo();

	// Remove excluded attribute
	int nExclude = CDialogHelper::FindItemByData(*this, m_nExcludeAttribID);
	
	if (nExclude != CB_ERR)
		DeleteString(nExclude);

	// Add extra items
	CDialogHelper::AddString(*this, CEnString(IDS_NONE), TDCA_NONE);
	CDialogHelper::AddString(*this, CEnString(IDS_DISPLAY_RECURRENCE), TDCA_RECURRENCE);

	// Restore selection
	if (!SetSelectedAttribute(nSelAttrib, sSelCustID))
		SetSelectedAttribute(TDCA_NONE, _T(""));
}

