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

	BOOL SetActiveGraph(const CGraphBase* pGraph);
// 	BOOL SetActiveGraphOption(BURNDOWN_GRAPHOPTION nOption);
// 	BOOL SetActiveGraphColors(const CColorArray& aColors);
	void SetShowEmptyFrequencyValues(BOOL bShowEmpty);

// 	BURNDOWN_GRAPH GetActiveGraph() const { return m_nActiveGraph; }
// 	BURNDOWN_GRAPHOPTION GetActiveGraphOption() const;
// 	int GetActiveGraphColors(CColorArray& aColors) const;
	
// 	CString GetGraphTitle(BURNDOWN_GRAPH nGraph) const;
// 	BURNDOWN_GRAPHTYPE GetGraphType(BURNDOWN_GRAPH nGraph) const;
// 	BURNDOWN_GRAPHOPTION GetDefaultOption(BURNDOWN_GRAPH nGraph) const;
// 	BOOL IsValidOption(BURNDOWN_GRAPHOPTION nOption, BURNDOWN_GRAPH nGraph) const;

// 	void LoadPreferences(const IPreferences* pPrefs, LPCTSTR szKey);
// 	void SavePreferences(IPreferences* pPrefs, LPCTSTR szKey) const;

// 	void SetGraphColors(const CGraphColorMap& mapColors);
// 	const CGraphsMap& Graphs() const { return m_mapGraphs; }

	void OnColoursChanged();
	void OnOptionChanged(BURNDOWN_GRAPHOPTION nOption);

protected:
	const CStatsItemArray& m_data;

//	CGraphsMap m_mapGraphs;
	CStatsItemCalculator m_calculator;

	const CGraphBase* m_pGraph;
	COleDateTimeRange m_dtExtents;
	COLORREF m_crToday;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	void RefreshRenderFlags(BOOL bRedraw = TRUE);

	// CHMXChart overrides
	virtual CString GetTooltip(int nHit) const;
	virtual void DoPaint(CDC& dc, BOOL bPaintBkgnd = TRUE);
	virtual BOOL GetMinMax(double& dMin, double& dMax, BOOL bDataOnly) const;
	virtual CString GetYTickText(int nTick, double dValue) const;

	// CHMXChartEx overrides
	virtual int GetNumYSubTicks(double dInterval) const;

	BOOL HighlightDataPoint(int nIndex);
	BOOL DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha = 255);

};

