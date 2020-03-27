#pragma once

#include "BurndownStruct.h"
#include "BurndownCalculator.h"
#include "BurndownGraphs.h"

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
	//BOOL IsValidGraph(BURNDOWN_GRAPH nGraph) const;

	BURNDOWN_GRAPHTYPE GetActiveGraphType() const;
	BOOL SetActiveGraph(BURNDOWN_GRAPH nGraph);
	BURNDOWN_GRAPH GetActiveGraph() const { return m_nActiveGraph; }
	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);
	BOOL RebuildGraph(const COleDateTimeRange& dtExtents);

	int BuildSortedGraphList(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs) const;

	void SetGraphOption(BURNDOWN_GRAPHOPTION nOption);
	void SetGraphColors(const CGraphColorMap& mapColors);
	void GetDefaultGraphColors(CGraphColorMap& mapColors) const;

protected:
	const CStatsItemArray& m_data;
	CGraphsMap m_mapGraphs;
	CGraphColorMap m_mapGraphColors;

	COleDateTimeRange m_dtExtents;
	CStatsItemCalculator m_calculator;
	COLORREF m_crToday;

	BURNDOWN_GRAPHOPTION m_nOption;
	BURNDOWN_GRAPH m_nActiveGraph;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	void UpdateGraphOption();

	// virtual overrides
	CString GetTooltip(int nHit) const;
	int HitTest(const CPoint& ptClient) const;
	void DoPaint(CDC& dc, BOOL bPaintBkgnd);

};

