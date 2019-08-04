#pragma once

#include "BurndownStruct.h"

#include "..\Shared\HMXChartEx.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChartEx
{
public:
	CBurndownChart(const CStatsItemArray& data);
	virtual ~CBurndownChart();

	BOOL SetChartType(BURNDOWN_CHARTTYPE nType);
	BOOL SaveToImage(CBitmap& bmImage);

	void RebuildGraph(BOOL bUpdateExtents);

protected:
	const CStatsItemArray& m_data;

	COleDateTimeRange m_dtExtents;
	BURNDOWN_CHARTTYPE m_nChartType;
	int m_nScale;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	DECLARE_MESSAGE_MAP()

protected:
	void BuildSprintGraph();
	void BuildBurndownGraph();
	void RebuildXScale();
	COleDateTime GetGraphStartDate() const;
	COleDateTime GetGraphEndDate() const;
	BURNDOWN_CHARTSCALE CalculateRequiredXScale() const;
	int HitTest(const CPoint& ptClient) const;

};

/////////////////////////////////////////////////////////////////////////////



