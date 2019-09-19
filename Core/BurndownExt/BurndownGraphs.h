#pragma once

#include "BurndownEnum.h"

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;
class CStatsItemCalculator;

/////////////////////////////////////////////////////////////////////////////

class CGraphBase
{
public:
	virtual ~CGraphBase();

	virtual CString GetTitle() const  = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;
	virtual BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]) = 0;

	int HitTest(const CStatsItemCalculator& calculator, const COleDateTime& date) const;

protected:
	DWORD m_dwTrendAnalyses;

protected:
	CGraphBase();

	static BOOL CalculateBestFitLine(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest);
	static BOOL CalculateMovingAverage(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest, int nWindowSize);

	static void SetDatasetColor(CHMXDataset datasets[HMX_MAX_DATASET], int nDataset, COLORREF crBase);

};

/////////////////////////////////////////////////////////////////////////////

class CIncompleteTasksGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]);
};

/////////////////////////////////////////////////////////////////////////////

class CRemainingDaysGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	enum
	{
		REMAINING_ESTIMATE,
		REMAINING_SPENT
	};
};

/////////////////////////////////////////////////////////////////////////////

class CStartedEndedTasksGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	enum
	{
		STARTED_TASKS,
		ENDED_TASKS
	};
};

/////////////////////////////////////////////////////////////////////////////

class CEstimatedSpentDaysGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	enum
	{
		ESTIMATED_DAYS,
		SPENT_DAYS,
	};
};

/////////////////////////////////////////////////////////////////////////////

class CEstimatedSpentCostGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL EnableTrends(DWORD dwItems, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	enum
	{
		ESTIMATED_COST,
		SPENT_COST,
	};
};

/////////////////////////////////////////////////////////////////////////////

