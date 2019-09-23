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
#include "..\shared\ScopedTimer.h"

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart

CBurndownChart::CBurndownChart(const CStatsItemArray& data)
	: 
	m_data(data),
	m_nActiveGraph(BCT_TIMESERIES_INCOMPLETETASKS),
	m_calculator(data),
	m_nTrendLine(BTL_NONE)
{
	VERIFY(m_graphs.Add(new CIncompleteTasksGraph()) == BCT_TIMESERIES_INCOMPLETETASKS);
	VERIFY(m_graphs.Add(new CRemainingDaysGraph()) == BCT_TIMESERIES_REMAININGDAYS);
	VERIFY(m_graphs.Add(new CStartedEndedTasksGraph()) == BCT_TIMESERIES_STARTEDENDEDTASKS);
	VERIFY(m_graphs.Add(new CEstimatedSpentDaysGraph()) == BCT_TIMESERIES_ESTIMATEDSPENTDAYS);
	//VERIFY(m_graphs.Add(new CEstimatedSpentCostGraph()) == BCT_ESTIMATEDSPENTCOST);
	VERIFY(m_graphs.Add(new CCategoryFrequencyGraph()) == BCT_FREQUENCY_CATEGORY);
	//m_graphs.Add(new ());

	//FileMisc::EnableLogging(TRUE);

	SetTooltipOffset(16, 0);
}

CBurndownChart::~CBurndownChart()
{
	int nGraph = m_graphs.GetSize();

	while (nGraph--)
	{
		delete m_graphs[nGraph];
		m_graphs.RemoveAt(nGraph);
	}
}

////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBurndownChart, CHMXChartEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart message handlers

BOOL CBurndownChart::IsValidGraph(BURNDOWN_GRAPH nType) const
{
	return ((nType >= 0) && (nType < BCT_NUMGRAPHS) && (nType < m_graphs.GetSize()));
}

BURNDOWN_GRAPHTYPE CBurndownChart::GetActiveGraphType() const
{
	return m_graphs[m_nActiveGraph]->GetType();
}

CString CBurndownChart::GetGraphTitle(BURNDOWN_GRAPH nType) const
{
	if (!IsValidGraph(nType))
	{
		ASSERT(0);
		return _T("");
	}

	// else
	return m_graphs[nType]->GetTitle();
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

void CBurndownChart::ShowTrendLine(BURNDOWN_TREND nTrend)
{
	m_nTrendLine = nTrend;

	UpdateGraphTrendLine();
}

void CBurndownChart::UpdateGraphTrendLine()
{
	if (GetActiveGraphType() == BCT_TIMESERIES)
	{
		CTimeSeriesGraph* pTSGraph = dynamic_cast<CTimeSeriesGraph*>(m_graphs[m_nActiveGraph]);
		ASSERT(pTSGraph);

		VERIFY(pTSGraph->ShowTrendLine(m_nTrendLine, m_datasets));
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
	ClearXScaleLabels();

	int nLabelStep = 1;
	m_graphs[m_nActiveGraph]->RebuildXScale(m_calculator, m_rectData.Width(), m_strarrScaleXLabel, nLabelStep);

	SetXLabelStep(nLabelStep);
}

void CBurndownChart::OnSize(UINT nType, int cx, int cy) 
{
	CHMXChartEx::OnSize(nType, cx, cy);
	
	RebuildXScale();
}

void CBurndownChart::RebuildGraph(const COleDateTimeRange& dtExtents)
{
	if (!m_dtExtents.Set(dtExtents))
	{
		ASSERT(0);
		return;
	}

	m_calculator.SetDateRange(m_dtExtents);

	CWaitCursor cursor;
	CHoldRedraw hr(*this);
	
	ClearData();
	SetYText(m_graphs[m_nActiveGraph]->GetTitle());
	
	if (!m_data.IsEmpty())
		RebuildXScale();

	{
		CScopedLogTimer log(_T("CBurndownChart::BuildGraph(%s)"), GetYText());

		m_graphs[m_nActiveGraph]->BuildGraph(m_calculator, m_datasets);
		UpdateGraphTrendLine();
	}

	CalcDatas();
}

void CBurndownChart::PreSubclassWindow()
{
	SetBkGnd(GetSysColor(COLOR_WINDOW));
	SetXLabelsAreTicks(true);
	SetXLabelAngle(45);
	SetYTicks(10);

	VERIFY(InitTooltip(TRUE));
}

CString CBurndownChart::GetTooltip(int nHit) const
{
	ASSERT(nHit != -1);

	return m_graphs[m_nActiveGraph]->GetTooltip(m_calculator, m_datasets, nHit);
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

	if (m_crToday != CLR_NONE)
	{
		// Find the data point corresponding to today
		COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);
		int nPos = m_graphs[m_nActiveGraph]->HitTest(m_calculator, dtToday);

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
