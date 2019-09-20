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

/////////////////////////////////////////////////////////////////////////////

const int MIN_XSCALE_SPACING = 50; // pixels

static BURNDOWN_GRAPHSCALE SCALES[] = 
{
	BCS_DAY,		
	BCS_WEEK,	
	BCS_MONTH,	
	BCS_2MONTH,	
	BCS_QUARTER,	
	BCS_HALFYEAR,
	BCS_YEAR,	
};
static int NUM_SCALES = sizeof(SCALES) / sizeof(int);

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart

CBurndownChart::CBurndownChart(const CStatsItemArray& data)
	: 
	m_data(data),
	m_nScale(BCS_DAY),
	m_nChartType(BCT_INCOMPLETETASKS),
	m_calculator(data),
	m_nTrendLine(BTL_NONE)
{
	VERIFY(m_graphs.Add(new CIncompleteTasksGraph()) == BCT_INCOMPLETETASKS);
	VERIFY(m_graphs.Add(new CRemainingDaysGraph()) == BCT_REMAININGDAYS);
	VERIFY(m_graphs.Add(new CStartedEndedTasksGraph()) == BCT_STARTEDENDEDTASKS);
	VERIFY(m_graphs.Add(new CEstimatedSpentDaysGraph()) == BCT_ESTIMATEDSPENTDAYS);
	//VERIFY(m_graphs.Add(new CEstimatedSpentCostGraph()) == BCT_ESTIMATEDSPENTCOST);
	//m_graphs.Add(new ());

	//FileMisc::EnableLogging(TRUE);
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

BOOL CBurndownChart::IsValidGraph(BURNDOWN_GRAPHTYPE nType) const
{
	return ((nType >= 0) && (nType < BCT_NUMGRAPHS) && (nType < m_graphs.GetSize()));
}

CString CBurndownChart::GetGraphTitle(BURNDOWN_GRAPHTYPE nType) const
{
	if (!IsValidGraph(nType))
	{
		ASSERT(0);
		return _T("");
	}

	// else
	return m_graphs[nType]->GetTitle();
}

BOOL CBurndownChart::SetActiveGraph(BURNDOWN_GRAPHTYPE nType)
{
	if (!IsValidGraph(nType))
	{
		ASSERT(0);
		return FALSE;
	}

	if (nType != m_nChartType)
	{
		m_nChartType = nType;
		RebuildGraph(m_dtExtents);

		return TRUE;
	}

	return FALSE;
}

void CBurndownChart::ShowTrendLine(BURNDOWN_TRENDTYPE nTrend)
{
	if (m_graphs[m_nChartType]->ShowTrendLine(nTrend, m_datasets))
	{
		m_nTrendLine = nTrend;
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

BURNDOWN_GRAPHSCALE CBurndownChart::CalculateRequiredScale(int nAvailWidth, int nNumDays)
{
	// work thru the available scales until we find a suitable one
	for (int nScale = 0; nScale < NUM_SCALES; nScale++)
	{
		int nSpacing = MulDiv(SCALES[nScale], nAvailWidth, nNumDays);

		if (nSpacing > MIN_XSCALE_SPACING)
			return SCALES[nScale];
	}

	return BCS_YEAR;
}

void CBurndownChart::RebuildXScale()
{
	ClearXScaleLabels();
	SetXLabelStep(1); // Because we often have an uneven label spacing

	// Refresh scale
	int nNumDays = m_calculator.GetTotalDays();
	m_nScale = CalculateRequiredScale(m_rectData.Width(), nNumDays);

	// build ticks
	COleDateTime dtTick = m_calculator.GetStartDate();

	CDateHelper dh;
	CString sTick;
	
	for (int nDay = 0; nDay <= nNumDays; )
	{
		sTick = dh.FormatDate(dtTick);
		SetXScaleLabel(nDay, sTick);

		// next Tick date
		COleDateTime dtNextTick(dtTick);

		switch (m_nScale)
		{
		case BCS_DAY:
			dtNextTick.m_dt += 1.0;
			break;
			
		case BCS_WEEK:
			dh.OffsetDate(dtNextTick, 1, DHU_WEEKS);
			break;
			
		case BCS_MONTH:
			dh.OffsetDate(dtNextTick, 1, DHU_MONTHS);
			break;
			
		case BCS_2MONTH:
			dh.OffsetDate(dtNextTick, 2, DHU_MONTHS);
			break;
			
		case BCS_QUARTER:
			dh.OffsetDate(dtNextTick, 3, DHU_MONTHS);
			break;
			
		case BCS_HALFYEAR:
			dh.OffsetDate(dtNextTick, 6, DHU_MONTHS);
			break;
			
		case BCS_YEAR:
			dh.OffsetDate(dtNextTick, 1, DHU_YEARS);
			break;
			
		default:
			ASSERT(0);
		}

		nDay += (int)(dtNextTick.m_dt - dtTick.m_dt);

		dtTick = dtNextTick;
	}
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
	SetYText(m_graphs[m_nChartType]->GetTitle());
	
	if (!m_data.IsEmpty())
		RebuildXScale();

	{
		CScopedLogTimer log(_T("CBurndownChart::BuildGraph(%s)"), GetYText());

		m_graphs[m_nChartType]->BuildGraph(m_calculator, m_datasets);
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

	return m_graphs[m_nChartType]->GetTooltip(m_calculator, m_datasets, nHit);
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
		int nPos = m_graphs[m_nChartType]->HitTest(m_calculator, dtToday);

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
