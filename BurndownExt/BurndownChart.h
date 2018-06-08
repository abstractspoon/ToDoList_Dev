#pragma once

#include "BurndownStruct.h"

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////
// CBurndownChart

class CBurndownChart : public CHMXChart
{
public:
	CBurndownChart(const CStatsItemArray& data);
	virtual ~CBurndownChart();

	BOOL SetChartType(BURNDOWN_CHARTTYPE nType);
	BOOL SaveToImage(CBitmap& bmImage);
	BOOL SetTimeIntervals(int nDaysInWeek, double dHoursInDay);

	void RebuildGraph(BOOL bUpdateExtents);

protected:
	const CStatsItemArray& m_data;
	
	COleDateTime m_dtEarliestDate, m_dtLatestDate;
	int m_nScale;
	BURNDOWN_CHARTTYPE m_nChartType;
	int m_nDaysInWeek;
	double m_dHoursInDay;

protected:
	void OnSize(UINT nType, int cx, int cy);
	void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void BuildSprintGraph();
	void BuildBurndownGraph();
	void RebuildXScale();
	int GetDataDuration() const;
	COleDateTime GetGraphStartDate() const;
	COleDateTime GetGraphEndDate() const;
	int CalculateRequiredXScale() const;
	void UpdateDataExtents();

};

/////////////////////////////////////////////////////////////////////////////



