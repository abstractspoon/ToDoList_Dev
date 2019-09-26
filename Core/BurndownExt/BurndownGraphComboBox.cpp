// BurndownGraphComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "BurndownGraphComboBox.h"
#include "BurndownChart.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Misc.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

struct GRAPHTYPE
{
	BURNDOWN_GRAPHTYPE nType;
	UINT nLabelID;
};

const GRAPHTYPE GRAPHTYPES[] = 
{
	{ BCT_TIMESERIES, IDS_TIMESERIES },
	{ BCT_FREQUENCY,  IDS_FREQUENCYDIST },
};
const int NUM_GRAPHTYPES = (sizeof(GRAPHTYPES) / sizeof(GRAPHTYPES[0]));

/////////////////////////////////////////////////////////////////////////////

struct SORTITEM
{
	BURNDOWN_GRAPH nType;
	CString sLabel;
};

static int SortProc(const void* pV1, const void* pV2)
{
	const SORTITEM* pSI1 = (const SORTITEM*)pV1;
	const SORTITEM* pSI2 = (const SORTITEM*)pV2;

	return Misc::NaturalCompare(pSI1->sLabel, pSI2->sLabel);
}

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

void CBurndownGraphComboBox::Initialise(const CBurndownChart& chart)
{
	// We build the combo in a specific order
	ASSERT(!(GetStyle() & CBS_SORT));

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];

		CDialogHelper::AddString(*this, gt.nLabelID, gt.nType);

		// For each type, sort all the related graphs by name
		// before adding to combo
		CArray<SORTITEM, SORTITEM&> aGraphs;
		SORTITEM st;

		for (int nGraph = 0; nGraph < BCT_NUMGRAPHS; nGraph++)
		{
			if (chart.GetGraphType((BURNDOWN_GRAPH)nGraph) == gt.nType)
			{
				st.nType = (BURNDOWN_GRAPH)nGraph;
				st.sLabel = chart.GetGraphTitle(st.nType);

				aGraphs.Add(st);
			}
		}

		Misc::SortArrayT(aGraphs, SortProc);

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			const SORTITEM& st = aGraphs[nItem];
			CDialogHelper::AddString(*this, st.sLabel, st.nType);
		}
	}

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);
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
