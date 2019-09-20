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

	BOOL ShowTrendLine(BURNDOWN_TRENDTYPE nTrend, CHMXDataset datasets[HMX_MAX_DATASET]);
	int HitTest(const CStatsItemCalculator& calculator, const COleDateTime& date) const;

protected:
	BURNDOWN_TRENDTYPE m_nTrend;

protected:
	CGraphBase();

	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const = 0;
	
	static BOOL CalculateTrendLine(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend, int nDatasetSrc, int nDatasetDest);
	static void SetDatasetColor(CHMXDataset datasets[HMX_MAX_DATASET], int nDataset, COLORREF crBase);

// 	static BOOL	CopyDataset(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest);
// 	static BOOL	MoveDataset(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest);

private:
	static BOOL CalculateBestFitLine(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest);
	static BOOL CalculateMovingAverage(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest, int nWindowSize);
};

/////////////////////////////////////////////////////////////////////////////

class CIncompleteTasksGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

class CRemainingDaysGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		REMAINING_ESTIMATE,
		REMAINING_SPENT
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

class CStartedEndedTasksGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		STARTED_TASKS,
		ENDED_TASKS
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

class CEstimatedSpentDaysGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		ESTIMATED_DAYS,
		SPENT_DAYS,
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

class CEstimatedSpentCostGraph : public CGraphBase
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		ESTIMATED_COST,
		SPENT_COST,
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TRENDTYPE nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

