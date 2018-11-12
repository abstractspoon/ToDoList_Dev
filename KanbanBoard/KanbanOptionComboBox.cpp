// FilterOptionComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KanbanOptionComboBox.h"
#include "KanbanEnum.h"

#include "..\shared\misc.h"
#include "..\Shared\enstring.h"
#include "..\Shared\localizer.h"

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
	// update m_dwOptions
	m_dwOptions = 0;

	for (int nItem = 0; nItem < GetCount(); nItem++)
	{
		// aggregate checked items
		if (GetCheck(nItem))
			m_dwOptions |= GetItemData(nItem);
	}
}

void CKanbanOptionComboBox::SetSelectedOptions(DWORD dwOptions)
{
	ASSERT(GetSafeHwnd());

	// Build once only
	if (GetCount() == 0)
	{
		// translation done via CEnString
		CLocalizer::EnableTranslation(*this, FALSE);

		int nItem = AddString(CEnString(IDS_OPTIONS_SHOWPARENTS));
		SetItemData(nItem, KBCF_SHOWPARENTTASKS);

		nItem = AddString(CEnString(IDS_OPTIONS_SHOWEMPTYCOLS));
		SetItemData(nItem, KBCF_SHOWEMPTYCOLUMNS);
	}

	if (dwOptions != m_dwOptions)
	{
		for (int nItem = 0; nItem < GetCount(); nItem++)
		{
			// get flag for item
			UINT nFlag = GetItemData(nItem);

			// set state
			BOOL bChecked = Misc::HasFlag(dwOptions, nFlag);
			SetCheck(nItem, (bChecked ? CCBC_CHECKED : CCBC_UNCHECKED));
		}

		m_dwOptions = dwOptions;
	}
}

DWORD CKanbanOptionComboBox::GetSelectedOptions() const 
{
	return m_dwOptions;
}

BOOL CKanbanOptionComboBox::HasSelectedOption(DWORD dwOption) const
{
	return (dwOption && ((m_dwOptions & dwOption) == dwOption)) ? TRUE : FALSE;
}

void CKanbanOptionComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	CString sItemText(sItem);

	if (sItemText.IsEmpty() && m_sText.IsEmpty())
		sItemText = CEnString(IDS_OPTIONS_NONE);

	CCheckComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItemText, bList, crText);
}
