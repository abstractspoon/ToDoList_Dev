// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "BurndownChart.h"
#include "BurndownGraphs.h"
#include "BurndownStatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\graphicsmisc.h"
#include "..\shared\misc.h"
#include "..\shared\ScopedTimer.h"

#include "..\Interfaces\IPreferences.h"

////////////////////////////////////////////////////////////////////////////////

#define CHECK_GRAPH() ASSERT(m_pGraph); if (!m_pGraph) return
#define CHECK_GRAPH_RET(ret) ASSERT(m_pGraph); if (!m_pGraph) return ret

/////////////////////////////////////////////////////////////////////////////

const int NUM_Y_TICKS = 10;

////////////////////////////////////////////////////////////////////////////////
// CBurndownChart

CBurndownChart::CBurndownChart(const CStatsItemArray& data)
	: 
	m_data(data),
	m_pGraph(NULL),
	m_calculator(data)
{
	EnableFixedLabelFontSize(); // don't scale down as available size decreases

	m_dRoundY = 0.0; // we do our own rounding
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

// CString CBurndownChart::GetGraphTitle(BURNDOWN_GRAPH nGraph) const
// {
// 	CGraphBase* pGraph = NULL;
// 	GET_GRAPH_RET(nGraph, _T(""));
// 
// 	return pGraph->GetTitle();
// }

// BURNDOWN_GRAPHTYPE CBurndownChart::GetGraphType(BURNDOWN_GRAPH nGraph) const
// {
// 	CGraphBase* pGraph = NULL;
// 	GET_GRAPH_RET(nGraph, BCT_UNKNOWNTYPE);
// 
// 	return pGraph->GetType();
// }

// BURNDOWN_GRAPHOPTION CBurndownChart::GetDefaultOption(BURNDOWN_GRAPH nGraph) const
// {
// 	return ::GetDefaultOption(GetGraphType(nGraph));
// }

// BOOL CBurndownChart::IsValidOption(BURNDOWN_GRAPHOPTION nOption, BURNDOWN_GRAPH nGraph) const
// {
// 	return ::IsValidOption(nOption, GetGraphType(nGraph));
// }

BOOL CBurndownChart::SetActiveGraph(const CGraphBase* pGraph)
{
	if (!pGraph)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pGraph != m_pGraph)
	{
		m_pGraph = pGraph;

		if (m_dtExtents.IsValid())
			RebuildGraph(m_dtExtents);
	}

	return TRUE;
}

// BOOL CBurndownChart::SetActiveGraphColors(const CColorArray& aColors)
// {
// 	CGraphBase* pGraph = NULL;
// 	GET_GRAPH_RET(m_nActiveGraph, FALSE);
// 
// 	if (!pGraph->SetColors(aColors))
// 		return FALSE;
// 
// 	pGraph->UpdateDatasetColors(m_datasets);
// 	Invalidate();
// 
// 	return TRUE;
// }

// BURNDOWN_GRAPHOPTION CBurndownChart::GetActiveGraphOption() const
// {
// 	CGraphBase* pGraph = NULL;
// 	GET_GRAPH_RET(m_nActiveGraph, BGO_INVALID);
// 
// 	return pGraph->GetOption();
// }

// int CBurndownChart::GetActiveGraphColors(CColorArray& aColors) const
// {
// 	CGraphBase* pGraph = NULL;
// 	GET_GRAPH_RET(m_nActiveGraph, 0);
// 
// 	aColors.Copy(pGraph->GetColors());
// 	return aColors.GetSize();
// }

/*
void CBurndownChart::LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey)
{
	POSITION pos = m_mapGraphs.GetStartPosition();

	while (pos)
	{
		BURNDOWN_GRAPH nGraph;
		CGraphBase* pGraph = m_mapGraphs.GetNext(pos, nGraph);

		CString sGraphKey = Misc::MakeKey(_T("GraphColors%d"), nGraph);
		CString sColors = pPrefs->GetProfileString(szKey, sGraphKey);

		if (!sColors.IsEmpty()) // first time will fail
		{
			CColorArray aColors;
			Misc::Split(sColors, aColors, '|');

			// Only allow the same number of colours as the graph's default palette
			if (aColors.GetSize() == pGraph->GetColors().GetSize())
				pGraph->SetColors(aColors);
		}

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
		pPrefs->WriteProfileString(szKey, sGraphKey, Misc::FormatArray(pGraph->GetColors(), '|'));

		sGraphKey = Misc::MakeKey(_T("GraphOption%d"), nGraph);
		pPrefs->WriteProfileInt(szKey, sGraphKey, pGraph->GetOption());
	}
}
*/

// void CBurndownChart::SetGraphColors(const CGraphColorMap& mapColors)
// {
// 	if (m_mapGraphs.SetGraphColors(mapColors))
// 	{
// 		CGraphBase* pGraph = NULL;
// 		GET_GRAPH(m_nActiveGraph);
// 
// 		pGraph->UpdateDatasetColors(m_datasets);
// 		Invalidate();
// 	}
// }

void CBurndownChart::OnColoursChanged()
{
	CHECK_GRAPH();

	m_pGraph->UpdateDatasetColors(m_datasets);
	Invalidate();
}

void CBurndownChart::OnOptionChanged(BURNDOWN_GRAPHOPTION nOption)
{
	CHECK_GRAPH();

	if (m_pGraph->OnOptionChanged(nOption, m_datasets))
	{
		RefreshRenderFlags(FALSE);
		Invalidate();
	}
}

void CBurndownChart::SetShowEmptyFrequencyValues(BOOL bShowEmpty)
{
	CHECK_GRAPH();

	if (m_calculator.SetShowEmptyFrequencyValues(bShowEmpty) && 
		m_pGraph->HasType(BCT_FREQUENCY) &&
		m_dtExtents.IsValid())
	{
		RebuildGraph(m_dtExtents);
	}
}

BOOL CBurndownChart::HighlightDataPoint(int nIndex)
{
	CHECK_GRAPH_RET(FALSE);
	
	if (m_pGraph->HasOption(BGO_FREQUENCY_PIE) || 
		m_pGraph->HasOption(BGO_FREQUENCY_DONUT))
	{ 
		// we handle it ourselves for now
		// TODO

		return FALSE;
	}

	return CHMXChartEx::HighlightDataPoint(nIndex);
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

	if (!m_data.IsEmpty() && m_calculator.HasValidDateRange())
	{
		CHECK_GRAPH();

		int nLabelStep = 1;
		m_pGraph->RebuildXScale(m_calculator, m_rectData.Width(), m_strarrScaleXLabel, nLabelStep);

		SetXLabelStep(nLabelStep);
	}
}

void CBurndownChart::OnSize(UINT nType, int cx, int cy) 
{
	CHMXChartEx::OnSize(nType, cx, cy);
	
	RebuildXScale();
}

BOOL CBurndownChart::RebuildGraph(const COleDateTimeRange& dtExtents)
{
	if (!dtExtents.IsValid() || !m_dtExtents.Set(dtExtents))
	{
		ASSERT(0);
		return FALSE;
	}
	
	CHECK_GRAPH_RET(FALSE);

	m_calculator.SetDateRange(m_dtExtents);

	CWaitCursor cursor;
	CHoldRedraw hr(*this);
	
	ResetDatasets();
	RefreshRenderFlags(FALSE);

	SetXText(m_pGraph->GetTitle());
	SetYText(m_pGraph->GetTitle());

	{
		CScopedLogTimer log(_T("CBurndownChart::BuildGraph(%s)"), GetYText());
		m_pGraph->BuildGraph(m_calculator, m_datasets);
	}

	RebuildXScale();
	CalcDatas();

	return TRUE;
}

CString CBurndownChart::GetYTickText(int nTick, double dValue) const
{
	// This is a virtual function which can get legitimately called
	// by CHMXChart before m_pGraph is set so we don't assert
	if (m_pGraph)
	{
		switch (m_pGraph->GetGraph())
		{
		case BCG_MINMAX_DUEDONEDATES:
			return COleDateTime(dValue).Format(VAR_DATEVALUEONLY);
		}
	}

	// All else
	return CHMXChartEx::GetYTickText(nTick, dValue);
}

int CBurndownChart::GetNumYSubTicks(double dInterval) const
{
	CHECK_GRAPH_RET(1);

	int nNumSub = CHMXChartEx::GetNumYSubTicks(dInterval);

	switch (m_pGraph->GetGraph())
	{
	case BCG_MINMAX_DUEDONEDATES:
		// Don't allow less than a day
		if ((dInterval / nNumSub) < 1.0)
			nNumSub = (int)dInterval;
		break;
	}

	return nNumSub;
}

BOOL CBurndownChart::GetMinMax(double& dMin, double& dMax, BOOL /*bDataOnly*/) const
{
	if (m_data.GetSize() == 0)
		return FALSE;

	CHECK_GRAPH_RET(FALSE);

	if (!m_pGraph->GetMinMax(dMin, dMax) || (dMin >= dMax))
		return FALSE;

	switch (m_pGraph->GetGraph())
	{
	case BCG_MINMAX_DUEDONEDATES:
		{
			double dDiff = max(10.0, (dMax - dMin));
			dMax = dMin + HMXUtils::CalcMaxYAxisValue(dDiff, NUM_Y_TICKS);
			// some comment
		}
		break;

	default: // All else
		dMin = 0.0;
		dMax = HMXUtils::CalcMaxYAxisValue(dMax, NUM_Y_TICKS);
		break;
	}

	return TRUE;
}

void CBurndownChart::RefreshRenderFlags(BOOL bRedraw)
{
	CHECK_GRAPH();

	DWORD dwFlags = ModifyRenderFlags(HMX_RENDER_TITLE, 0, FALSE); // Never draw title

	switch (m_pGraph->GetOption())
	{
	case BGO_FREQUENCY_PIE:
	case BGO_FREQUENCY_DONUT:
		// Remove then add
		dwFlags &= ~ (HMX_RENDER_YAXISTITLE | HMX_RENDER_GRID | HMX_RENDER_AXES | HMX_RENDER_BASELINE);
		dwFlags |= HMX_RENDER_XAXISTITLE;
		break;

	default:
		// Add then remove
		dwFlags |= (HMX_RENDER_YAXISTITLE | HMX_RENDER_GRID | HMX_RENDER_AXES | HMX_RENDER_BASELINE);
		dwFlags &= ~HMX_RENDER_XAXISTITLE;
		break;
	}

	SetRenderFlags(dwFlags, bRedraw);
}

void CBurndownChart::PreSubclassWindow()
{
	SetBkGnd(GetSysColor(COLOR_WINDOW));
	SetXLabelsAreTicks(TRUE);
	SetXLabelAngle(45);
	SetNumYTicks(NUM_Y_TICKS);

	VERIFY(InitTooltip(TRUE));
}

CString CBurndownChart::GetTooltip(int nHit) const
{
	ASSERT(nHit != -1);

	CHECK_GRAPH_RET(_T(""));

	return m_pGraph->GetTooltip(m_calculator, m_datasets, nHit);
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

	CHECK_GRAPH();

	if ((m_rectData.Height() > 0) && (m_crToday != CLR_NONE) && m_pGraph->HasType(BCT_TIMESERIES))
	{
		// Find the data point corresponding to today
		COleDateTime dtToday = CDateHelper::GetDate(DHD_TODAY);
		int nPos = m_calculator.HitTest(dtToday);

		if (nPos != -1)
		{
			CPoint ptPos;

			if (GetPointXY(0, nPos, ptPos))
				GraphicsMisc::DrawVertLine(&dc, m_rectData.bottom, m_rectData.top, ptPos.x, m_crToday);
		}
	}
}

BOOL CBurndownChart::DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha)
{
	if (!IsValidDatasetIndex(nDatasetIndex))
	{
		ASSERT(0);
		return FALSE;
	}

	CHECK_GRAPH_RET(FALSE);

	if (m_pGraph->HasType(BCT_MINMAX))
		return CHMXChartEx::DrawMinMaxChart(dc, m_datasets[nDatasetIndex], m_datasets[nDatasetIndex + 1], alpha);

	return CHMXChartEx::DrawDataset(dc, m_datasets[nDatasetIndex], m_pGraph->GetColors(), alpha);
}

