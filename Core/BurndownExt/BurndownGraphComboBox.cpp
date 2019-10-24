// BurndownGraphComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "BurndownGraphComboBox.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphComboBox

CBurndownGraphComboBox::CBurndownGraphComboBox()
{
}

CBurndownGraphComboBox::~CBurndownGraphComboBox()
{
}


BEGIN_MESSAGE_MAP(CBurndownGraphComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CBurndownGraphComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOK)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphComboBox message handlers

BOOL CBurndownGraphComboBox::Initialise(const CBurndownChart& chart)
{
	// Once only
	if (GetCount())
	{
		ASSERT(0);
		return FALSE;
	}

	// We build the combo in a specific order
	if (GetStyle() & CBS_SORT)
	{
		ASSERT(0);
		return FALSE;
	}

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];

		CDialogHelper::AddString(*this, gt.nLabelID, gt.nType);

		// For each type, sort all the related graphs by name
		// before adding to combo
		CGraphArray aGraphs;
		VERIFY(chart.BuildSortedGraphList(gt.nType, aGraphs));

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			BURNDOWN_GRAPH nGraph = aGraphs[nItem];
			CDialogHelper::AddString(*this, chart.GetGraphTitle(nGraph), nGraph);
		}
	}

	CDialogHelper::RefreshMaxDropWidth(*this);

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	return TRUE;
}

void CBurndownGraphComboBox::GetItemColors(int nItem, UINT nItemState, DWORD dwItemData,
											COLORREF& crText, COLORREF& crBack) const
{
	COwnerdrawComboBoxBase::GetItemColors(nItem, nItemState, dwItemData, crText, crBack);

	if (ItemDataIsContainer(dwItemData))
	{
		BOOL bItemSelected = (nItemState & ODS_SELECTED);

		if (bItemSelected)
		{
			crBack = GetSysColor(COLOR_3DDKSHADOW);
			crText = GetSysColor(COLOR_3DHIGHLIGHT);
		}
		else
		{
			crText = GetSysColor(COLOR_3DDKSHADOW);
		}
	}
}

void CBurndownGraphComboBox::DrawItemText(CDC& dc, const CRect& rect, int nItem, UINT nItemState,
											DWORD dwItemData, const CString& sItem, BOOL bList, COLORREF crText)
{
	// If it's an actual graph rather than a container then indent the item
	CRect rItem(rect);

	if (bList && !ItemDataIsContainer(dwItemData))
	{
		rItem.left += rItem.Height();
	}

	COwnerdrawComboBoxBase::DrawItemText(dc, rItem, nItem, nItemState, dwItemData, sItem, bList, crText);
}

BOOL CBurndownGraphComboBox::OnSelEndOK()
{
	// Prevent focus moving to a container item
	DWORD dwItemData = CDialogHelper::GetSelectedItemData(*this);

	if (ItemDataIsContainer(dwItemData))
		CDialogHelper::SelectItemByData(*this, (dwItemData + 1));

	return FALSE;// continue routing
}


void CBurndownGraphComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Step over container items
	int nSel = GetCurSel();

	if ((nChar == VK_UP) && ItemIsContainer(nSel - 1))
	{
		nSel -= 2;
	}
	else if ((nChar == VK_DOWN) && ItemIsContainer(nSel + 1))
	{
		nSel += 2;
	}

	if (nSel != GetCurSel())
	{
		SetCurSel(nSel);

		int nMsgID = (GetDroppedState() ? CBN_SELCHANGE : CBN_SELENDOK);
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nMsgID), (LPARAM)GetSafeHwnd());

		return; // eat
	}

	COwnerdrawComboBoxBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBurndownGraphComboBox::ItemIsContainer(int nItem) const
{
	if ((nItem < 0) || (nItem > (GetCount() - 1)))
		return FALSE;

	return ItemDataIsContainer(GetItemData(nItem));
}

BOOL CBurndownGraphComboBox::ItemDataIsContainer(DWORD dwItemData)
{
	switch (dwItemData)
	{
	case BCT_TIMESERIES:
	case BCT_FREQUENCY:
		return TRUE;
	}
	
	return FALSE;
}
