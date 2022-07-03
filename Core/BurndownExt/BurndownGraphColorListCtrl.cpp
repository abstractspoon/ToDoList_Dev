// BurndownGraphColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "burndownext.h"
#include "BurndownGraphColorListCtrl.h"
#include "BurndownStatic.h"
#include "BurndownChart.h"

#include "..\Shared\GraphicsMisc.h"
#include "..\Shared\encolordialog.h"

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
	ASSERT((GetStyle() & (LVS_SORTASCENDING | LVS_SORTDESCENDING)) == 0);

	VERIFY(chart.GetGraphColors(m_mapColors));

	AutoAdd(FALSE, FALSE);
	ShowGrid(TRUE, TRUE);

	AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(200)); // Graph titles
	DisableColumnEditing(0, TRUE);

	// Populate items
	int nSelRow = -1;

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];

		int nRow = AddRow(CEnString(gt.nLabelID));
		SetItemData(nRow, gt.nType);

		// For each type, sort all the related graphs by name
		// before adding to list
		CGraphArray aGraphs;
		VERIFY(chart.BuildSortedGraphList(gt.nType, aGraphs));

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			BURNDOWN_GRAPH nGraph = aGraphs[nItem];

			nRow = AddRow(chart.GetGraphTitle(nGraph));
			SetItemData(nRow, nGraph);

			// build colour columns as we go
			int nNumColors = m_mapColors.GetColorCount(nGraph);

			while (GetColumnCount() <= nNumColors)
				AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(50), ILCT_BROWSE);

			// Set selection to the currently active graph
			if (chart.GetActiveGraph() == nGraph)
				nSelRow = nRow;
		}
	}

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
	// graph title not editable
	if (nCol == 0)
		return FALSE;

	// header row not editable
	int dwItemData = GetItemData(nRow);

	if ((dwItemData == BCT_FREQUENCY) || (dwItemData == BCT_TIMESERIES))
		return FALSE;

	return (nCol <= m_mapColors.GetColorCount((BURNDOWN_GRAPH)dwItemData));
}

COLORREF CBurndownGraphColorListCtrl::GetItemBackColor(int nItem, int nCol, BOOL bSelected, BOOL bDropHighlighted, BOOL bWndFocus) const
{
	if (nCol == 0)
	{
		int dwItemData = GetItemData(nItem);

		if ((dwItemData == BCT_FREQUENCY) || (dwItemData == BCT_TIMESERIES))
			return GetSysColor(COLOR_3DLIGHT);
	}

	return CInputListCtrl::GetItemBackColor(nItem, nCol, bSelected, bDropHighlighted, bWndFocus);
}

void CBurndownGraphColorListCtrl::EditCell(int nItem, int nCol, BOOL /*bBtnClick*/)
{
	// graph title not editable
	ASSERT (nCol > 0);

	// header row not editable
	int dwItemData = GetItemData(nItem);

	ASSERT((dwItemData != BCT_FREQUENCY) && (dwItemData != BCT_TIMESERIES));

	BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)dwItemData;

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
		// header row not editable
		int dwItemData = GetItemData(nRow);

		if ((dwItemData == BCT_FREQUENCY) || (dwItemData == BCT_TIMESERIES))
			return;

		BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)dwItemData;

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
