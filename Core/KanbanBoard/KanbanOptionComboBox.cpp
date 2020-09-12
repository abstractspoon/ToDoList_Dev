// FilterOptionComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanOptionComboBox.h"
#include "KanbanEnum.h"

#include "..\shared\misc.h"
#include "..\Shared\enstring.h"
#include "..\Shared\localizer.h"
#include "..\Shared\dialoghelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterOptionComboBox

CKanbanOptionComboBox::CKanbanOptionComboBox() : CCheckComboBox(), m_dwOptions(0)
{
}

CKanbanOptionComboBox::~CKanbanOptionComboBox()
{
}


BEGIN_MESSAGE_MAP(CKanbanOptionComboBox, CCheckComboBox)
	//{{AFX_MSG_MAP(CFilterOptionComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterOptionComboBox message handlers

BOOL CKanbanOptionComboBox::Initialise(DWORD dwOptions)
{
	SetSelectedOptions(dwOptions);
	return TRUE;
}

void CKanbanOptionComboBox::OnCheckChange(int /*nIndex*/)
{
	m_dwOptions = GetCheckedItemData();
}

void CKanbanOptionComboBox::SetSelectedOptions(DWORD dwOptions)
{
	ASSERT(GetSafeHwnd());

	// Build once only
	if (GetCount() == 0)
	{
		// translation done via CEnString
		CLocalizer::EnableTranslation(*this, FALSE);

		CDialogHelper::AddString(*this, IDS_OPTIONS_HIDEPARENTS, KBCF_HIDEPARENTTASKS);
		CDialogHelper::AddString(*this, IDS_OPTIONS_HIDEEMPTYCOLS, KBCF_HIDEEMPTYCOLUMNS);
		CDialogHelper::AddString(*this, IDS_OPTIONS_HIDESUBTASKS, KBCF_HIDESUBTASKS);

		EnableTooltip();
	}

	if (dwOptions != m_dwOptions)
	{
		SetCheckedByItemData(dwOptions);
		m_dwOptions = dwOptions;
	}
}

DWORD CKanbanOptionComboBox::GetSelectedOptions() const 
{
	return m_dwOptions;
}

BOOL CKanbanOptionComboBox::HasSelectedOption(DWORD dwOption) const
{
	if (!dwOption)
		return FALSE;
	
	return Misc::HasFlag(m_dwOptions, dwOption);
}

