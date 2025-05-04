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

BOOL CBurndownGraphColorListCtrl::Initialize(const CGraphsMap& mapGraphs, BURNDOWN_GRAPH nActiveGraph)
{
	ASSERT(GetStyle() & LVS_OWNERDRAWFIXED);

	VERIFY(mapGraphs.GetColors(m_mapColors));

	AutoAdd(FALSE, FALSE);
	ShowGrid(TRUE, TRUE);

	AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(200)); // Graph titles
	EnableColumnEditing(0, FALSE);

	GetGrouping().EnableGroupView(*this);

	// Populate rows
	int nSelRow = -1;

	for (int nType = 0; nType < NUM_GRAPHTYPES; nType++)
	{
		const GRAPHTYPE& gt = GRAPHTYPES[nType];
		GetGrouping().InsertGroupHeader(nType, gt.nType, CEnString(gt.nLabelID));

		CGraphArray aGraphs;
		VERIFY(mapGraphs.GetGraphs(gt.nType, aGraphs, FALSE));

		for (int nItem = 0; nItem < aGraphs.GetSize(); nItem++)
		{
			BURNDOWN_GRAPH nGraph = aGraphs[nItem];
			int nRow = AddRow(mapGraphs.GetTitle(nGraph));

			SetItemData(nRow, nGraph);
			GetGrouping().SetItemGroupId(nRow, gt.nType);

			// Set selection to the currently active graph
			if (nActiveGraph == nGraph)
				nSelRow = nRow;

			// Cache custom attribute IDs for later lookups
			if (IsCustomAttributeGraph(nGraph))
				m_mapCustAttribIDs[nGraph] = mapGraphs.GetCustomAttributeID(nGraph);
		}
	}

	// Populate columns
	int nColor = mapGraphs.GetMaxColorCount();

	while (nColor--)
		AddCol(_T(""), GraphicsMisc::ScaleByDPIFactor(50), ILCT_BROWSE);

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

	CColorArray aUnused;
	return (nCol <= GetRowColors(nRow, aUnused));
}

int CBurndownGraphColorListCtrl::GetRowColors(int nRow, CColorArray& aColors) const
{
	BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)GetItemData(nRow);

	if (!IsCustomAttributeGraph(nGraph))
		return m_mapColors.GetColors(nGraph, aColors);

	// else
	CString sCustAttribID;
	VERIFY(m_mapCustAttribIDs.Lookup(nGraph, sCustAttribID));

	return m_mapColors.GetColors(sCustAttribID, aColors);
}

void CBurndownGraphColorListCtrl::EditCell(int nItem, int nCol, BOOL /*bBtnClick*/)
{
	CColorArray aColors;
	VERIFY(GetRowColors(nItem, aColors));

	CEnColorDialog dialog(aColors[nCol - 1]);

	if (dialog.DoModal() == IDOK)
	{
		aColors.SetAt((nCol - 1), dialog.GetColor());

		BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)GetItemData(nItem);

		if (IsCustomAttributeGraph(nGraph))
		{
			CString sCustAttribID;
			VERIFY(m_mapCustAttribIDs.Lookup(nGraph, sCustAttribID));

			m_mapColors.SetColors(sCustAttribID, aColors);
		}
		else
		{
			m_mapColors.SetColors(nGraph, aColors);
		}
	}
}

void CBurndownGraphColorListCtrl::DrawCellText(CDC* pDC, int nRow, int nCol,
											  const CRect& rText, const CString& sText,
											  COLORREF crText, UINT nDrawTextFlags)
{
	if (nCol > 0)
	{
		CColorArray aColors;
		VERIFY(GetRowColors(nRow, aColors));

		if (nCol > aColors.GetSize()) // first column is graph title
			return;

		COLORREF color = aColors[nCol - 1];
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
