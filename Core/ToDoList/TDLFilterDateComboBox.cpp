// CTDLFilterDateComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "TDLFilterDateComboBox.h"
#include "tdcstatic.h"

#include "..\shared\enstring.h"
#include "..\shared\localizer.h"
#include "..\shared\dialoghelper.h"
#include "..\shared\autoflag.h"
#include "..\shared\Wndprompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox

CTDLFilterDateComboBox::CTDLFilterDateComboBox(int nNextNDays) 
	: 
	CTabbedComboBox(20),
	m_nNextNDays(nNextNDays),
	m_bRebuildingCombo(FALSE)
{
}

CTDLFilterDateComboBox::~CTDLFilterDateComboBox()
{
}


BEGIN_MESSAGE_MAP(CTDLFilterDateComboBox, CTabbedComboBox)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnReflectSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDLFilterDateComboBox message handlers

void CTDLFilterDateComboBox::SetNextNDays(int nDays)
{
	if (m_nNextNDays != nDays)
	{
		m_nNextNDays = nDays;

		// Selectively replace '7' with 'N' without loss of selection
		CAutoFlag af(m_bRebuildingCombo, TRUE);
		
		FILTER_DATE nSelFilter = GetSelectedFilter();
		RebuildCombo();

		SelectFilter(nSelFilter);
	}
}

BOOL CTDLFilterDateComboBox::OnReflectSelChange()
{
	// prevent forwarding of selection changes
	// whilst rebuilding combo
	return m_bRebuildingCombo;
}

void CTDLFilterDateComboBox::BuildCombo()
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	CLocalizer::EnableTranslation(*this, FALSE);

	TCHAR nLetter = 'A';

	for (int nItem = 0; nItem < NUM_DATEFILTER; nItem++)
	{
		CEnString sFilter(DATE_FILTERS[nItem][0]);
		UINT nFilter = DATE_FILTERS[nItem][1];

		CString sText;

		switch (nFilter)
		{
		case FD_ANY:
		case FD_NONE:
		case FD_USER:
			sText = sFilter;
			break;

		case FD_NEXTNDAYS:
			if (m_nNextNDays != 7)
				sFilter.Replace(_T("7"), Misc::Format(m_nNextNDays));
			// fall thru

		default:
			sText.Format(_T("%c)\t%s"), nLetter, sFilter); 
			nLetter++;
			break;
		}

		VERIFY(CDialogHelper::AddStringT(*this, sText, nFilter) != CB_ERR);
	}

	CDialogHelper::RefreshMaxDropWidth(*this);
}

FILTER_DATE CTDLFilterDateComboBox::GetSelectedFilter() const
{
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
		return FD_ANY;

	// else
	return (FILTER_DATE)GetItemData(nSel);
}

BOOL CTDLFilterDateComboBox::SelectFilter(FILTER_DATE nFilter)
{
	CheckBuildCombo();

	return (CB_ERR != CDialogHelper::SelectItemByDataT(*this, (DWORD)nFilter));
}

void CTDLFilterDateComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
	DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	if (!(nItemState & ODS_SELECTED) && !bList && (dwItemData == FD_ANY))
	{
		// Display <any> in window prompt colour if nothing is checked
		crText = CWndPrompt::GetTextColor();
	}
	
	CTabbedComboBox::DrawItemText(dc, rect, nItem, nItemState, dwItemData, sItem, bList, crText);
}
