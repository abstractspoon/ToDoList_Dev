// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownChart.h"
#include "BurndownGraphs.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\filemisc.h"
#include "..\shared\misc.h"
#include "..\shared\ScopedTimer.h"

////////////////////////////////////////////////////////////////////////////////

#define GET_GRAPH(e) pGraph = GetGraph(e); if (pGraph == NULL) return
#define GET_GRAPH_RET(e, ret) pGraph = GetGraph(e); if (pGraph == NULL) return ret

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
	m_calculator(data),
	m_nTrendLine(BTL_NONE)
{
	m_mapGraphs[BCT_TIMESERIES_INCOMPLETETASKS]		= new CIncompleteTasksGraph();
	m_mapGraphs[BCT_TIMESERIES_REMAININGDAYS]		= new CRemainingDaysGraph();
	m_mapGraphs[BCT_TIMESERIES_STARTEDENDEDTASKS]	= new CStartedEndedTasksGraph();
	m_mapGraphs[BCT_TIMESERIES_ESTIMATEDSPENTDAYS]	= new CEstimatedSpentDaysGraph();
	//m_mapGraphs[BCT_ESTIMATEDSPENTCOST]			= new CEstimatedSpentCostGraph();

	m_mapGraphs[BCT_FREQUENCY_CATEGORY]				= new CCategoryFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_STATUS]				= new CStatusFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_ALLOCTO]				= new CAllocatedToFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_ALLOCBY]				= new CAllocatedByFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_VERSION]				= new CVersionFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_TAGS]					= new CTagFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_PRIORITY]				= new CPriorityFrequencyGraph();
	m_mapGraphs[BCT_FREQUENCY_RISK]					= new CRiskFrequencyGraph();

	//FileMisc::EnableLogging(TRUE);

	GetDefaultGraphColors(m_mapGraphColors);
	EnableFixedLabelFontSize(); // don't scale down as available size decreases
}

CBurndownChart::~CBurndownChart()
{
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph;

		m_mapGraphs.GetNextAssoc(pos, nGraph, pGraph);
		delete pGraph;
	}
}

////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBurndownChart, CHMXChartEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart message handlers

BOOL CBurndownChart::IsValidGraph(BURNDOWN_GRAPH nGraph) const
{
	return (GetGraphType(nGraph) != BCT_UNKNOWNTYPE);
}

BURNDOWN_GRAPHTYPE CBurndownChart::GetActiveGraphType() const
{
	return GetGraphType(m_nActiveGraph);
}

CString CBurndownChart::GetGraphTitle(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(nGraph, _T(""));

	return pGraph->GetTitle();
}

BURNDOWN_GRAPHTYPE CBurndownChart::GetGraphType(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;
	GET_GRAPH_RET(nGraph, BCT_UNKNOWNTYPE);

	return pGraph->GetType();
}

CGraphBase* CBurndownChart::GetGraph(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;
	m_mapGraphs.Lookup(nGraph, pGraph);

	return pGraph;
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

	for (int nGraph = 0; nGraph < BCT_NUMGRAPHS; nGraph++)
	{
		if (GetGraphType((BURNDOWN_GRAPH)nGraph) == nType)
		{
			st.nGraph = (BURNDOWN_GRAPH)nGraph;
			st.sLabel = GetGraphTitle(st.nGraph);

			aSort.Add(st);
		}
	}

	Misc::SortArrayT<SORTITEM>(aSort, SortProc);

	int nItem = aSort.GetSize();
	aGraphs.SetSize(nItem);

	while (nItem--)
		aGraphs[nItem] = aSort[nItem].nGraph;
	
	return aGraphs.GetSize();
}

void CBurndownChart::SetGraphColors(const CGraphColorMap& mapColors)
{
	m_mapGraphColors.Copy(mapColors);

	// Update the active graph
	CGraphBase* pGraph = NULL;
	GET_GRAPH(m_nActiveGraph);

	CColorArray aColors;
	VERIFY(mapColors.Lookup(m_nActiveGraph, aColors) && aColors.GetSize());

	pGraph->SetDatasetColors(m_datasets, aColors);
	Invalidate();
}

void CBurndownChart::GetDefaultGraphColors(CGraphColorMap& mapColors) const
{
	mapColors.RemoveAll();
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph;

		m_mapGraphs.GetNextAssoc(pos, nGraph, pGraph);

		CColorArray aColors;
		VERIFY(pGraph->GetDefaultColors(aColors));

		mapColors[nGraph] = aColors;
	}
}

void CBurndownChart::ShowTrendLine(BURNDOWN_TREND nTrend)
{
	if (nTrend != m_nTrendLine)
	{
		m_nTrendLine = nTrend;

		UpdateGraphTrendLine();
	}
}

void CBurndownChart::UpdateGraphTrendLine()
{
	if (GetActiveGraphType() == BCT_TIMESERIES)
	{
		CGraphBase* pGraph = NULL;
		GET_GRAPH(m_nActiveGraph);

		CTimeSeriesGraph* pTSGraph = dynamic_cast<CTimeSeriesGraph*>(pGraph);
		ASSERT(pTSGraph);

		/*VERIFY*/(pTSGraph->ShowTrendLine(m_nTrendLine, m_datasets));
		Invalidate();
	}
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

		CColorArray aColors;
		VERIFY(m_mapGraphColors.Lookup(m_nActiveGraph, aColors) && aColors.GetSize());

		pGraph->BuildGraph(m_calculator, aColors, m_datasets);
		UpdateGraphTrendLine();
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

	if ((m_crToday != CLR_NONE) && (GetActiveGraphType() == BCT_TIMESERIES))
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
