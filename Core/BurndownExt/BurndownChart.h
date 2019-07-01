#pragma once

#include "BurndownStruct.h"

#include "..\Shared\ToolTipCtrlEx.h"
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
	BOOL SetTimeIntervals(int nDaysInWeek, double dHoursInDay);

	void RebuildGraph(BOOL bUpdateExtents);
	void FilterToolTipMessage(MSG* pMsg);

protected:
	const CStatsItemArray& m_data;

	CToolTipCtrlEx m_tooltip; 
	
	COleDateTimeRange m_dtExtents;
	int m_nScale;
	BURNDOWN_CHARTTYPE m_nChartType;
	int m_nDaysInWeek;
	double m_dHoursInDay;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();
	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	DECLARE_MESSAGE_MAP()

	virtual bool DrawHorzLine(CDC& dc);						
	
protected:
	void BuildSprintGraph();
	void BuildBurndownGraph();
	void RebuildXScale();
	COleDateTime GetGraphStartDate() const;
	COleDateTime GetGraphEndDate() const;
	BURNDOWN_CHARTSCALE CalculateRequiredXScale() const;
	int GetYSubTicks(double dInterval) const;
	int HitTest(const CPoint& ptClient) const;

};

/////////////////////////////////////////////////////////////////////////////



