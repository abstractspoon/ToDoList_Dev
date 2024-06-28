// BurndownGraphColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "burndownext.h"
#include "BurndownGraphColorListCtrl.h"
#include "BurndownStatic.h"
#include "BurndownChart.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\encolordialog.h"
#include "..\Shared\enstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphColorListCtrl

CBurndownGraphColorListCtrl::CBurndownGraphColorListCtrl()
{
}

CBurndownGraphColorListCtrl::~CBurndownGraphColorListCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBurndownGraphColorListCtrl message handlers

BOOL CBurndownGraphColorListCtrl::Initialize(const CBurndownChart& chart)
{
	ASSERT(GetStyle() & LVS_OWNERDRAWFIXED);

	VERIFY(chart.GetGraphColors(m_mapColors));

	AutoAdd(FALSE, FALSE);
	ShowGrid(TRUE, TRUE);

	AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(200)); // Graph titles
	EnableColumnEditing(0, FALSE);

	// Populate items
	int nSelRow = -1;

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];
		GetGrouping().InsertGroupHeader(nType, gt.nType, CEnString(gt.nLabelID));

		CGraphArray aGraphs;
		VERIFY(chart.GetGraphs(gt.nType, aGraphs, FALSE));

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			BURNDOWN_GRAPH nGraph = aGraphs[nItem];
			int nRow = AddRow(chart.GetGraphTitle(nGraph));

			SetItemData(nRow, nGraph);
			GetGrouping().SetItemGroupId(nRow, gt.nType);

			// build colour columns as we go
			int nColor = m_mapColors.GetColorCount(nGraph);

			while (nColor--)
				AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(50), ILCT_BROWSE);

			// Set selection to the currently active graph
			if (chart.GetActiveGraph() == nGraph)
				nSelRow = nRow;
		}
	}

	Sort();
	RefreshItemHeight();

	if (nSelRow != -1)
	{
		SetCurSel(nSelRow, 0);
		ScrollCellIntoView(nSelRow, 0);
	}
	
	return (GetItemCount() > 0);
}

BOOL CBurndownGraphColorListCtrl::CanEditCell(int nRow, int nCol) const
{
	if (!CInputListCtrl::CanEditCell(nRow, nCol))
		return FALSE;

	return (nCol <= m_mapColors.GetColorCount((BURNDOWN_GRAPH)GetItemData(nRow)));
}

void CBurndownGraphColorListCtrl::EditCell(int nItem, int nCol, BOOL /*bBtnClick*/)
{
	BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)GetItemData(nItem);

	CEnColorDialog dialog(m_mapColors.GetColor(nGraph, (nCol - 1)));

	if (dialog.DoModal() == IDOK)
		m_mapColors.SetColor(nGraph, (nCol - 1), dialog.GetColor());
}

void CBurndownGraphColorListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol,
											  const CRect& rText, const CString& sText,
											  COLORREF crText, UINT nDrawTextFlags)
{
	if (nCol > 0)
	{
		BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)GetItemData(nRow);

		if (m_mapColors.GetColorCount(nGraph) < nCol)
			return;

		COLORREF color = m_mapColors.GetColor(nGraph, (nCol - 1));
		ASSERT(color != CLR_NONE);

		CRect rColor(rText);
		rColor.DeflateRect(1, 3, 4, 3);

		GraphicsMisc::DrawRect(pDC, rColor, color, GraphicsMisc::Darker(color, 0.4));
	}
	else
	{
		CInputListCtrl::DrawCellText(pDC, nRow, nCol, rText, sText, crText, nDrawTextFlags);
	}
}
