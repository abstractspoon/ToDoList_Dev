#pragma once

#include "BurndownStruct.h"

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

	CString GetGraphTitle(BURNDOWN_GRAPHTYPE nType) const;
	BOOL SetActiveGraph(BURNDOWN_GRAPHTYPE nType);
	void SetTodayColour(COLORREF color);
	BOOL SaveToImage(CBitmap& bmImage);

	void EnableTrends(DWORD dwTrends);
	void RebuildGraph(const COleDateTimeRange& dtExtents);
	BURNDOWN_GRAPHSCALE GetCurrentScale() const { return m_nScale; }

	static BURNDOWN_GRAPHSCALE CalculateRequiredScale(int nAvailWidth, int nNumDays);

protected:
	const CStatsItemArray& m_data;
	CArray<CGraphBase*, CGraphBase*> m_graphs;

	COleDateTimeRange m_dtExtents;
	CStatsItemCalculator m_calculator;
	COLORREF m_crToday;
	DWORD m_dwEnabledTrends;

	BURNDOWN_GRAPHTYPE m_nChartType;
	BURNDOWN_GRAPHSCALE m_nScale;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	BOOL IsValidGraph(BURNDOWN_GRAPHTYPE nType) const;

	// virtual overrides
	CString GetTooltip(int nHit) const;
	int HitTest(const CPoint& ptClient) const;
	void DoPaint(CDC& dc, BOOL bPaintBkgnd);

};

