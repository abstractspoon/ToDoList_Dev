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

	CString GetGraphTitle(BURNDOWN_CHARTTYPE nType) const;
	BOOL SetActiveGraph(BURNDOWN_CHARTTYPE nType);
	BOOL SaveToImage(CBitmap& bmImage);

	void RebuildGraph(const COleDateTimeRange& dtExtents);

protected:
	const CStatsItemArray& m_data;
	CArray<CGraphBase*, CGraphBase*> m_graphs;

	COleDateTimeRange m_dtExtents;
	CStatsItemCalculator m_calculator;

	BURNDOWN_CHARTTYPE m_nChartType;
	BURNDOWN_CHARTSCALE m_nScale;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	DECLARE_MESSAGE_MAP()

protected:
	void RebuildXScale();
	BURNDOWN_CHARTSCALE CalculateRequiredXScale() const;
	int HitTest(const CPoint& ptClient) const;

};

