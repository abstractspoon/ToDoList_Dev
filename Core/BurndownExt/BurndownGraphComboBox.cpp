// BurndownGraphComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "BurndownGraphComboBox.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\Shared\DialogHelper.h"
#include "..\Shared\Localizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef CB_GETMINVISIBLE
#	define CB_GETMINVISIBLE 0x1702	
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphComboBox message handlers

BOOL CBurndownGraphComboBox::Initialise(const CGraphsMap& mapGraphs)
{
	ASSERT(GetSafeHwnd());
	ASSERT(GetCount() == 0);

	ModifyStyle(CBS_SORT, 0); // Unsorted

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];

		// Add heading
		int nHeading = CDialogHelper::AddStringT(*this, gt.nLabelID, gt.nType);
		SetHeadingItem(nHeading);

		// For each type, sort all the related graphs by name
		// before adding to combo
		CGraphArray aGraphs;
		VERIFY(mapGraphs.GetGraphs(gt.nType, aGraphs, TRUE));

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			BURNDOWN_GRAPH nGraph = aGraphs[nItem];
			CDialogHelper::AddStringT(*this, mapGraphs.GetTitle(nGraph), nGraph);
		}
	}

	CDialogHelper::RefreshMaxDropWidth(*this);

	CLocalizer::EnableTranslation(GetSafeHwnd(), FALSE);

	return TRUE;
}

void CBurndownGraphComboBox::DDX(CDataExchange* pDX, BURNDOWN_GRAPH& nGraph)
{
	if (pDX->m_bSaveAndValidate)
	{
		int nSel = GetCurSel();
	
		if (!IsSelectableItem(nSel))
		{
			ASSERT(0);
			nGraph = BCG_UNKNOWNGRAPH;
		}
		else
		{
			nGraph = (BURNDOWN_GRAPH)GetItemData(nSel);
		}
	}
	else
	{
		SetCurSel(CDialogHelper::FindItemByDataT(*this, nGraph));
	}
}

