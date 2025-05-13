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

const int DEFAULT_NUMYTICKS = 10;

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

BOOL CBurndownChart::SetActiveGraph(const CGraphBase* pGraph, BOOL bRebuild)
{
	if (!pGraph)
	{
		ASSERT(0);
		return FALSE;
	}

	if (pGraph != m_pGraph)
	{
		m_pGraph = pGraph;

		if (bRebuild && m_dtExtents.IsValid())
			RebuildGraph(m_dtExtents);
	}

	return TRUE;
}

void CBurndownChart::OnColorsChanged()
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

void CBurndownChart::SetShowEmptyFrequencyValues(BOOL bShowEmpty, BOOL bRebuild)
{
	if (m_calculator.SetShowEmptyFrequencyValues(bShowEmpty) && bRebuild &&	m_dtExtents.IsValid())
	{
		CHECK_GRAPH();

		if (m_pGraph->HasType(BCT_FREQUENCY))
			RebuildGraph(m_dtExtents);
	}
}

void CBurndownChart::OnDisplayISODatesChanged()
{
	RebuildXScale();
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

	RecalcNumYTicks();
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
			return m_pGraph->FormatDate(dValue);
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

void CBurndownChart::RecalcNumYTicks()
{
	double dUnused1, dUnused2;
	int nNumYTicks;

	if (CalcMinMax(dUnused1, dUnused2, nNumYTicks))
		SetNumYTicks(nNumYTicks);
}

BOOL CBurndownChart::GetMinMax(double& dMin, double& dMax, BOOL /*bDataOnly*/) const
{
	int nUnused;

	return CalcMinMax(dMin, dMax, nUnused);
}

BOOL CBurndownChart::CalcMinMax(double& dMin, double& dMax, int& nNumYTicks) const
{
	if (m_data.GetSize() == 0)
		return FALSE;

	CHECK_GRAPH_RET(FALSE);

	if (!m_pGraph->GetDataMinMax(dMin, dMax) || (dMin > dMax))
		return FALSE;

	nNumYTicks = DEFAULT_NUMYTICKS;

	if ((dMin < 0.0) && (dMax == 0.0))
	{
		dMin = -HMXUtils::CalcMaxYAxisValue(-dMin, nNumYTicks);
	}
	else if ((dMin == 0.0) && (dMax > 0.0))
	{
		dMax = HMXUtils::CalcMaxYAxisValue(dMax, nNumYTicks);
	}
	else if ((dMin < 0.0) && (dMax > 0.0))
	{
		// Whichever is the greater of 'abs(min) and max', use that
		// to calculate our overall scale and then calculate the
		// 'other' end of the scale from that
		int nOtherTicks = 0;

		if (dMax > -dMin)
		{
			dMax = HMXUtils::CalcMaxYAxisValue(dMax, nNumYTicks);

			double dTick = (dMax / nNumYTicks);

			nOtherTicks = (int)((-dMin / dTick) + 1);
			dMin = (-nOtherTicks * dTick);
		}
		else // -dMin > dMax
		{
			dMin = -HMXUtils::CalcMaxYAxisValue(-dMin, nNumYTicks);

			double dTick = (-dMin / DEFAULT_NUMYTICKS); // +ve

			nOtherTicks = (int)((dMax / dTick) + 1);
			dMax = (nOtherTicks * dTick);
		}

		nNumYTicks += nOtherTicks;
	}
	else // trickier
	{
		double dDiff = max(1.0, (dMax - dMin));
		dMax = dMin + HMXUtils::CalcMaxYAxisValue(dDiff, nNumYTicks);
	}

	return TRUE;
}

void CBurndownChart::RefreshRenderFlags(BOOL bRedraw)
{
	CHECK_GRAPH();

	DWORD dwFlags = ModifyRenderFlags(HMX_RENDER_TITLE, 0, FALSE); // Never draw title

	if (m_pGraph->HasAxes())
	{
		// Add then remove
		dwFlags |= (HMX_RENDER_YAXISTITLE | HMX_RENDER_GRID | HMX_RENDER_AXES | HMX_RENDER_BASELINE);
		dwFlags &= ~HMX_RENDER_XAXISTITLE;
	}
	else
	{
		// Remove then add
		dwFlags &= ~ (HMX_RENDER_YAXISTITLE | HMX_RENDER_GRID | HMX_RENDER_AXES | HMX_RENDER_BASELINE);
		dwFlags |= HMX_RENDER_XAXISTITLE;
	}

	SetRenderFlags(dwFlags, bRedraw);
}

void CBurndownChart::PreSubclassWindow()
{
	SetBkGnd(GetSysColor(COLOR_WINDOW));
	SetXLabelsAreTicks(TRUE);
	SetXLabelAngle(45);
	SetNumYTicks(DEFAULT_NUMYTICKS);

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

BOOL CBurndownChart::XScaleHasRTLDates() const 
{ 
	switch (m_pGraph->GetType())
	{
	case BCT_TIMESERIES:
	case BCT_MINMAX:
		return CDateHelper::WantRTLDates();
	}

	return FALSE; 
}

BOOL CBurndownChart::YScaleHasRTLDates() const 
{ 
	switch (m_pGraph->GetGraph())
	{
	case BCG_MINMAX_DUEDONEDATES:
		return CDateHelper::WantRTLDates();
	}

	return FALSE; 
}
