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
	m_bHasHeadings = TRUE; // base class
}

CBurndownGraphComboBox::~CBurndownGraphComboBox()
{
}


BEGIN_MESSAGE_MAP(CBurndownGraphComboBox, COwnerdrawComboBoxBase)
	//{{AFX_MSG_MAP(CBurndownGraphComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
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

void CBurndownGraphComboBox::DDX(CDataExchange* pDX, BURNDOWN_GRAPH& nGraph)
{
	if (pDX->m_bSaveAndValidate)
	{
		int nSel = GetCurSel();
	
		if ((nSel == -1) || COwnerdrawComboBoxBase::ItemIsHeading(nSel))
		{
			ASSERT(0);
			nGraph = BCT_UNKNOWNGRAPH;
		}
		else
		{
			nGraph = (BURNDOWN_GRAPH)GetItemData(nSel);
		}
	}
	else
	{
		int nFind = CDialogHelper::FindItemByData(*this, nGraph);

		if (ItemIsHeading(nFind, nGraph))
			ASSERT(0);
		else
			SetCurSel(nFind);
	}
}

BOOL CBurndownGraphComboBox::ItemIsHeading(int /*nItem*/, DWORD dwItemData) const
{
	switch (dwItemData)
	{
	case BCT_TIMESERIES:
	case BCT_FREQUENCY:
		return TRUE;
	}
	
	return FALSE;
}

