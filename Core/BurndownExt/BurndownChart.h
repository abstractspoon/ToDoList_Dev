#pragma once

#include "BurndownStruct.h"
#include "BurndownCalculator.h"

#include "..\Shared\HMXChartEx.h"

/////////////////////////////////////////////////////////////////////////////

class CGraphBase;

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChartEx
{
public:
	CBurndownChart(const CStatsItemArray& data);
	virtual ~CBurndownChart();

	CString GetGraphTitle(BURNDOWN_GRAPH nType) const;
	BURNDOWN_GRAPHTYPE GetActiveGraphType() const;
	BOOL SetActiveGraph(BURNDOWN_GRAPH nType);
	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);

	void ShowTrendLine(BURNDOWN_TREND nTrend);
	void RebuildGraph(const COleDateTimeRange& dtExtents);

protected:
	const CStatsItemArray& m_data;
	CArray<CGraphBase*, CGraphBase*> m_graphs;

	COleDateTimeRange m_dtExtents;
	CStatsItemCalculator m_calculator;
	COLORREF m_crToday;

	BURNDOWN_TREND m_nTrendLine;
	BURNDOWN_GRAPH m_nActiveGraph;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	BOOL IsValidGraph(BURNDOWN_GRAPH nType) const;
	void UpdateGraphTrendLine();

	// virtual overrides
	CString GetTooltip(int nHit) const;
	int HitTest(const CPoint& ptClient) const;
	void DoPaint(CDC& dc, BOOL bPaintBkgnd);

};

