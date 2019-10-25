#pragma once

#include "BurndownEnum.h"
#include "BurndownStruct.h"

#include "..\3rdParty\HMXChart.h"

/////////////////////////////////////////////////////////////////////////////

class COleDateTimeRange;
class CStatsItemCalculator;

/////////////////////////////////////////////////////////////////////////////

class CGraphBase
{
public:
	virtual ~CGraphBase();
	
	virtual BURNDOWN_GRAPHTYPE GetType() const = 0;
	virtual CString GetTitle() const  = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const = 0;
	virtual int GetRequiredColorCount() const = 0;

	static void SetDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET], const CColorArray& aColors);

protected:
	CGraphBase();

	static void SetDatasetColor(CHMXDataset& dataset, COLORREF crBase);
};

/////////////////////////////////////////////////////////////////////////////

class CTimeSeriesGraph : public CGraphBase
{
public:
	virtual ~CTimeSeriesGraph();

	virtual CString GetTitle() const = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;

	BURNDOWN_GRAPHTYPE GetType() const { return BCT_TIMESERIES; }
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL ShowTrendLine(BURNDOWN_TREND nTrend, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	BURNDOWN_TREND m_nTrend;

protected:
	CTimeSeriesGraph();

	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const = 0;

	static BOOL CalculateTrendLine(BURNDOWN_TREND nTrend, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);

private:
	static BOOL CalculateBestFitLine(const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);
	static BOOL CalculateMovingAverage(int nWindowSize, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);

	static BURNDOWN_GRAPHSCALE CalculateRequiredScale(int nAvailWidth, int nNumDays);
};

// ---------------------------------------------------------------------------

class CIncompleteTasksGraph : public CTimeSeriesGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 1; }

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const;
};

// ---------------------------------------------------------------------------

class CRemainingDaysGraph : public CTimeSeriesGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 2; }

protected:
	enum
	{
		REMAINING_ESTIMATE,
		REMAINING_SPENT
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const;
};

// ---------------------------------------------------------------------------

class CStartedEndedTasksGraph : public CTimeSeriesGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 2; }

protected:
	enum
	{
		STARTED_TASKS,
		ENDED_TASKS
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentDaysGraph : public CTimeSeriesGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 2; }

protected:
	enum
	{
		ESTIMATED_DAYS,
		SPENT_DAYS,
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentCostGraph : public CTimeSeriesGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 2; }

protected:
	enum
	{
		ESTIMATED_COST,
		SPENT_COST,
	};

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const;
};

/////////////////////////////////////////////////////////////////////////////

class CAttributeFrequencyGraph : public CGraphBase
{
public:
	virtual ~CAttributeFrequencyGraph();

	virtual CString GetTitle() const = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;

	BURNDOWN_GRAPHTYPE GetType() const { return BCT_FREQUENCY; }
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	int GetRequiredColorCount() const { return 1; }

protected:
	mutable CStringArray m_aAttribValues;

protected:
	CAttributeFrequencyGraph();

	void BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, COLORREF crGraph, CHMXDataset& dataset) const;

};

// ---------------------------------------------------------------------------

class CCategoryFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CStatusFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CAllocatedToFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CAllocatedByFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CVersionFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CTagFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CPriorityFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------

class CRiskFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, const CColorArray& aColors, CHMXDataset datasets[HMX_MAX_DATASET]) const;

};

// ---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
