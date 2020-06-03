#pragma once

#include "BurndownStruct.h"
#include "BurndownCalculator.h"
#include "BurndownGraphs.h"

#include "..\Shared\HMXChartEx.h"

/////////////////////////////////////////////////////////////////////////////

class CGraphBase;

class IPreferences;

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChartEx
{
public:
	CBurndownChart(const CStatsItemArray& data);
	virtual ~CBurndownChart();

	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);
	BOOL RebuildGraph(const COleDateTimeRange& dtExtents);

	BOOL SetActiveGraph(BURNDOWN_GRAPH nGraph);
	BOOL SetActiveGraphOption(BURNDOWN_GRAPHOPTION nOption);
	BOOL SetActiveGraphColors(const CColorArray& aColors);
	void SetShowEmptyFrequencyValues(BOOL bShowEmpty);

	BURNDOWN_GRAPH GetActiveGraph() const { return m_nActiveGraph; }
	BURNDOWN_GRAPHOPTION GetActiveGraphOption() const;
	int GetActiveGraphColors(CColorArray& aColors) const;
	
	CString GetGraphTitle(BURNDOWN_GRAPH nGraph) const;
	int BuildSortedGraphList(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs) const;

	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

	void SetGraphColors(const CGraphColorMap& mapColors);
	int GetGraphColors(CGraphColorMap& mapColors) const;

protected:
	const CStatsItemArray& m_data;
	CGraphsMap m_mapGraphs;
	CStatsItemCalculator m_calculator;

	BURNDOWN_GRAPH m_nActiveGraph;
	COleDateTimeRange m_dtExtents;
	COLORREF m_crToday;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	void RefreshRenderFlags(BOOL bRedraw = TRUE);

	// virtual overrides
	CString GetTooltip(int nHit) const;
	int HitTest(const CPoint& ptClient) const;
	void DoPaint(CDC& dc, BOOL bPaintBkgnd = TRUE);

	BOOL HighlightDataPoint(int nIndex);
	bool DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha = 255);

};

