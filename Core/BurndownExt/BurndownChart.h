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

	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);
	BOOL RebuildGraph(const COleDateTimeRange& dtExtents);

	BOOL SetActiveGraph(const CGraphBase* pGraph);
	void SetShowEmptyFrequencyValues(BOOL bShowEmpty);

	void OnColoursChanged();
	void OnOptionChanged(BURNDOWN_GRAPHOPTION nOption);
	void OnDisplayISODatesChanged();

protected:
	const CStatsItemArray& m_data;

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
	virtual BOOL DrawDataset(CDC &dc, int nDatasetIndex, BYTE alpha = 255);

	// CHMXChartEx overrides
	virtual int GetNumYSubTicks(double dInterval) const;
	virtual BOOL HighlightDataPoint(int nIndex);
};

