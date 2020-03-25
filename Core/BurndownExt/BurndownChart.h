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

	CString GetGraphTitle(BURNDOWN_GRAPH nGraph) const;
	BURNDOWN_GRAPHTYPE GetGraphType(BURNDOWN_GRAPH nGraph) const;
	BOOL IsValidGraph(BURNDOWN_GRAPH nGraph) const;

	BURNDOWN_GRAPHTYPE GetActiveGraphType() const;
	BOOL SetActiveGraph(BURNDOWN_GRAPH nGraph);
	BURNDOWN_GRAPH GetActiveGraph() const { return m_nActiveGraph; }
	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);
	void ShowTrendLine(BURNDOWN_TREND nTrend);
	BOOL RebuildGraph(const COleDateTimeRange& dtExtents);

	int BuildSortedGraphList(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs) const;

	void SetGraphColors(const CGraphColorMap& mapColors);
	void GetDefaultGraphColors(CGraphColorMap& mapColors) const;

protected:
	const CStatsItemArray& m_data;
	CMap<BURNDOWN_GRAPH, BURNDOWN_GRAPH, CGraphBase*, CGraphBase*&> m_mapGraphs;

	COleDateTimeRange m_dtExtents;
	CStatsItemCalculator m_calculator;
	COLORREF m_crToday;
	CGraphColorMap m_mapGraphColors;

	BURNDOWN_TREND m_nTrendLine;
	BURNDOWN_GRAPH m_nActiveGraph;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	void UpdateGraphTrendLine();
	CGraphBase* GetGraph(BURNDOWN_GRAPH nGraph) const;

	// virtual overrides
	CString GetTooltip(int nHit) const;
	int HitTest(const CPoint& ptClient) const;
	void DoPaint(CDC& dc, BOOL bPaintBkgnd);
	int CalcXScaleFontSize(BOOL bTitle) const;
	int CalcYScaleFontSize(BOOL bTitle) const;

};

