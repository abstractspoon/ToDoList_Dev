// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownChart.h"
#include "BurndownGraphs.h"
#include "BurndownStatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\ScopedTimer.h"

#include "..\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////

#define GET_GRAPH(e) pGraph = m_mapGraphs.GetGraph(e); if (pGraph == NULL) return
#define GET_GRAPH_RET(e, ret) pGraph = m_mapGraphs.GetGraph(e); if (pGraph == NULL) return ret

/////////////////////////////////////////////////////////////////////////////

struct SORTITEM
{
	BURNDOWN_GRAPH nGraph;
	CString sLabel;
};

static int SortProc(const void* pV1, const void* pV2)
{
	const SORTITEM* pSI1 = (const SORTITEM*)pV1;
	const SORTITEM* pSI2 = (const SORTITEM*)pV2;

	return Misc::NaturalCompare(pSI1->sLabel, pSI2->sLabel);
}

////////////////////////////////////////////////////////////////////////////////

// Fixed number of Y ticks
const int NUM_Y_TICKS = 10;

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart

CBurndownChart::CBurndownChart(const CStatsItemArray& data)
	: 
	m_data(data),
	m_nActiveGraph(BCT_TIMESERIES_INCOMPLETETASKS),
	m_calculator(data)
{
	//FileMisc::EnableLogging(TRUE);

	EnableFixedLabelFontSize(); // don't scale down as available size decreases
}

CBurndownChart::~CBurndownChart()
{
}

////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBurndownChart, CHMXChartEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart message handlers

CString CBurndownChart::GetGraphTitle(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(nGraph, _T(""));

	return pGraph->GetTitle();
}

BOOL CBurndownChart::SetActiveGraph(BURNDOWN_GRAPH nGraph)
{
	if (!IsValidGraph(nGraph))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nGraph != m_nActiveGraph)
	{
		m_nActiveGraph = nGraph;
		RebuildGraph(m_dtExtents);

		return TRUE;
	}

	return FALSE;
}

int CBurndownChart::BuildSortedGraphList(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs) const
{
	CArray<SORTITEM, SORTITEM&> aSort;
	SORTITEM st;

	int nItem;

	for (nItem = 0; nItem < BCT_NUMGRAPHS; nItem++)
	{
		BURNDOWN_GRAPH nGraph = (BURNDOWN_GRAPH)nItem;
		ASSERT(IsValidGraph(nGraph));

		if (m_mapGraphs.HasGraph(nGraph) && (GetGraphType(nGraph) == nType))
		{
			st.nGraph = (BURNDOWN_GRAPH)nItem;
			st.sLabel = GetGraphTitle(st.nGraph);

			aSort.Add(st);
		}
	}

	Misc::SortArrayT<SORTITEM>(aSort, SortProc);

	nItem = aSort.GetSize();
	aGraphs.SetSize(nItem);

	while (nItem--)
		aGraphs[nItem] = aSort[nItem].nGraph;
	
	return aGraphs.GetSize();
}

BOOL CBurndownChart::SetActiveGraphColors(const CColorArray& aColors)
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, FALSE);

	if (!pGraph->SetColors(aColors))
		return FALSE;

	pGraph->UpdateDatasetColors(m_datasets);
	Invalidate();

	return TRUE;
}

BURNDOWN_GRAPHOPTION CBurndownChart::GetActiveGraphOption() const
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, BGO_INVALID);

	return pGraph->GetOption();
}

int CBurndownChart::GetActiveGraphColors(CColorArray& aColors) const
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, 0);

	aColors.Copy(pGraph->GetColors());
	return aColors.GetSize();
}

void CBurndownChart::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = m_mapGraphs.GetNext(pos, nGraph);

		CString sGraphKey = Misc::MakeKey(_T("GraphColors%d"), nGraph);
/*
		CString sColors = pPrefs->GetProfileString(szKey, sGraphKey);

		if (!sColors.IsEmpty()) // first time will fail
		{
			CColorArray aColors;
			Misc::Split(sColors, aColors, '|');

			pGraph->SetColors(aColors);
		}
*/

		sGraphKey = Misc::MakeKey(_T("GraphOption%d"), nGraph);
		BURNDOWN_GRAPHOPTION nOption = (BURNDOWN_GRAPHOPTION)pPrefs->GetProfileInt(szKey, sGraphKey, GetDefaultOption(nGraph));

		if (nOption != BGO_INVALID)
			pGraph->SetOption(nOption);
	}
}

void CBurndownChart::SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const
{
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = m_mapGraphs.GetNext(pos, nGraph);

		CString sGraphKey = Misc::MakeKey(_T("GraphColors%d"), nGraph);
		//pPrefs->WriteProfileString(szKey, sGraphKey, Misc::FormatArray(pGraph->GetColors(), '|'));

		sGraphKey = Misc::MakeKey(_T("GraphOption%d"), nGraph);
		pPrefs->WriteProfileInt(szKey, sGraphKey, pGraph->GetOption());
	}
}

void CBurndownChart::SetGraphColors(const CGraphColorMap& mapColors)
{
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = m_mapGraphs.GetNext(pos, nGraph);

		CColorArray aColors;
		VERIFY(mapColors.Lookup(nGraph, aColors) && aColors.GetSize());

		pGraph->SetColors(aColors);

		// Refresh active graph
		if (nGraph == m_nActiveGraph)
		{
			pGraph->UpdateDatasetColors(m_datasets);
			Invalidate();
		}
	}
}

void CBurndownChart::GetGraphColors(CGraphColorMap& mapColors) const
{
	mapColors.RemoveAll();
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = m_mapGraphs.GetNext(pos, nGraph);

		CColorArray aColors;
		aColors.Copy(pGraph->GetColors());

		mapColors[nGraph] = aColors;
	}
}

BOOL CBurndownChart::SetActiveGraphOption(BURNDOWN_GRAPHOPTION nOption)
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, FALSE);

	if (!pGraph->IsValidOption(nOption))
		return FALSE;

	if (!pGraph->HasOption(nOption))
	{
		if (pGraph->SetOption(nOption, m_calculator, m_datasets))
			Invalidate();
	}

	return TRUE;
}

BOOL CBurndownChart::SaveToImage(CBitmap& bmImage)
{
	CClientDC dc(this);
	CDC dcImage;

	if (dcImage.CreateCompatibleDC(&dc))
	{
		CRect rClient;
		GetClientRect(rClient);

		if (bmImage.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height()))
		{
			CBitmap* pOldImage = dcImage.SelectObject(&bmImage);

			SendMessage(WM_PRINTCLIENT, (WPARAM)dcImage.GetSafeHdc(), PRF_ERASEBKGND);
			Invalidate(TRUE);

			dcImage.SelectObject(pOldImage);
		}
	}

	return (bmImage.GetSafeHandle() != NULL);
}

void CBurndownChart::RebuildXScale()
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH(m_nActiveGraph);

	ClearXScaleLabels();

	int nLabelStep = 1;
	pGraph->RebuildXScale(m_calculator, m_rectData.Width(), m_strarrScaleXLabel, nLabelStep);

	SetXLabelStep(nLabelStep);
}

void CBurndownChart::OnSize(UINT nType, int cx, int cy) 
{
	CHMXChartEx::OnSize(nType, cx, cy);
	
	RebuildXScale();
}

BOOL CBurndownChart::RebuildGraph(const COleDateTimeRange& dtExtents)
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, FALSE);

	if (!m_dtExtents.Set(dtExtents))
	{
		ASSERT(0);
		return FALSE;
	}
	
	m_calculator.SetDateRange(m_dtExtents);

	CWaitCursor cursor;
	CHoldRedraw hr(*this);
	
	ClearData();
	SetYText(pGraph->GetTitle());

	{
		CScopedLogTimer log(_T("CBurndownChart::BuildGraph(%s)"), GetYText());

		pGraph->BuildGraph(m_calculator, m_datasets);
	}

	if (!m_data.IsEmpty())
		RebuildXScale();

	CalcDatas();

	return TRUE;
}

void CBurndownChart::PreSubclassWindow()
{
	SetBkGnd(GetSysColor(COLOR_WINDOW));
	SetXLabelsAreTicks(true);
	SetXLabelAngle(45);
	SetNumYTicks(NUM_Y_TICKS);

	VERIFY(InitTooltip(TRUE));
}

CString CBurndownChart::GetTooltip(int nHit) const
{
	ASSERT(nHit != -1);

	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(m_nActiveGraph, _T(""));

	return pGraph->GetTooltip(m_calculator, m_datasets, nHit);
}

int CBurndownChart::HitTest(const CPoint& ptClient) const
{
	if (!m_dtExtents.IsValid())
		return -1;

	return CHMXChartEx::HitTest(ptClient);
}

void CBurndownChart::SetTodayColour(COLORREF color)
{
	if (color != m_crToday)
	{
		m_crToday = color;

		if (GetSafeHwnd() && IsWindowVisible())
			Invalidate();
	}
}

void CBurndownChart::DoPaint(CDC& dc, BOOL bPaintBkgnd)
{
	CHMXChartEx::DoPaint(dc, bPaintBkgnd);

	if ((m_rectData.Height() > 0) && (m_crToday != CLR_NONE) && (GetGraphType(m_nActiveGraph) == BCT_TIMESERIES))
	{
		CGraphBase* pGraph = NULL;
		GET_GRAPH(m_nActiveGraph);

		// Find the data point corresponding to today
		COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);
		int nPos = m_calculator.HitTest(dtToday);

		if (nPos != -1)
		{
			CPoint ptPos;

			if (GetPointXY(0, nPos, ptPos))
			{
				GraphicsMisc::DrawVertLine(&dc, m_rectData.bottom, m_rectData.top, ptPos.x, m_crToday);
			}
		}
	}
}
