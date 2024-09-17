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
	
	virtual CString GetTitle() const = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const = 0;
	virtual BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) = 0;

	BURNDOWN_GRAPHTYPE GetType() const;
	BOOL GetMinMax(double& dMin, double& dMax) const;

	const CColorArray& GetColors() const;
	BOOL SetColors(const CColorArray& aColors);
	void UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const;

	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption);
	BURNDOWN_GRAPHOPTION GetOption() const;
	BOOL IsValidOption(BURNDOWN_GRAPHOPTION nOption) const;
	BOOL HasOption(BURNDOWN_GRAPHOPTION nOption) const;

protected:
	CGraphBase(BURNDOWN_GRAPH nGraph, BURNDOWN_GRAPHOPTION nOption = BGO_INVALID);
	
	static void SetDatasetColor(CHMXDataset& dataset, COLORREF crBase);
	static void ClearData(CHMXDataset datasets[HMX_MAX_DATASET]);

	static BURNDOWN_GRAPHSCALE CalculateRequiredXScale(int nAvailWidth, int nNumDays);

protected:
	BOOL InitColorPalette(int nNumColors, int nStartColor);
	BOOL InitColorPalette(COLORREF color1, COLORREF color2 = CLR_NONE, COLORREF color3 = CLR_NONE);
	COLORREF GetColor(int nColor) const;
	void RecalcDataMinMax(const CHMXDataset datasets[HMX_MAX_DATASET], double dIgnoreVal = HMX_DATASET_VALUE_NOIGNORE);

private:
	BURNDOWN_GRAPH m_nGraph;
	BURNDOWN_GRAPHOPTION m_nOption;
	CColorArray m_aColors;
	double m_dDataMin, m_dDataMax;
};

/////////////////////////////////////////////////////////////////////////////

class CGraphsMap : public CMap<BURNDOWN_GRAPH, BURNDOWN_GRAPH, CGraphBase*, CGraphBase*&>
{
public:
	CGraphsMap();
	~CGraphsMap();

	CGraphBase* GetNext(POSITION& pos) const;
	CGraphBase* GetNext(POSITION& pos, BURNDOWN_GRAPH& nGraph) const;
	CGraphBase* GetGraph(BURNDOWN_GRAPH nGraph) const;

	BOOL HasGraph(BURNDOWN_GRAPH nGraph) const;

protected:
	BOOL AddGraph(BURNDOWN_GRAPH nGraph, CGraphBase* pGraph);
};

/////////////////////////////////////////////////////////////////////////////

class CTimeSeriesGraph : public CGraphBase
{
public:
	virtual ~CTimeSeriesGraph();

protected:
	CTimeSeriesGraph(BURNDOWN_GRAPH nGraph);

	// Virtual overrides
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);

	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;

	static BOOL CalculateTrendLine(BURNDOWN_GRAPHOPTION nTrend, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);

private:
	static BOOL CalculateBestFitLine(const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);
	static BOOL CalculateMovingAverage(int nWindowSize, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);
};

// ---------------------------------------------------------------------------

class CIncompleteTasksTimeGraph : public CTimeSeriesGraph
{
public:
	CIncompleteTasksTimeGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CRemainingDaysTimeGraph : public CTimeSeriesGraph
{
public:
	CRemainingDaysTimeGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

protected:
	enum
	{
		REMAINING_ESTIMATE,
		REMAINING_SPENT
	};

};

// ---------------------------------------------------------------------------

class CStartedEndedTasksTimeGraph : public CTimeSeriesGraph
{
public:
	CStartedEndedTasksTimeGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

protected:
	enum
	{
		STARTED_TASKS,
		ENDED_TASKS
	};
};

// ---------------------------------------------------------------------------

class CEstimatedSpentDaysTimeGraph : public CTimeSeriesGraph
{
public:
	CEstimatedSpentDaysTimeGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

protected:
	enum
	{
		ESTIMATED_DAYS,
		SPENT_DAYS,
	};
};

// ---------------------------------------------------------------------------

class CEstimatedSpentCostGraph : public CTimeSeriesGraph
{
public:
	CEstimatedSpentCostGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

protected:
	enum
	{
		ESTIMATED_COST,
		SPENT_COST,
	};
};

/////////////////////////////////////////////////////////////////////////////

class CFrequencyGraph : public CGraphBase
{
public:
	virtual ~CFrequencyGraph();

protected:
	CFrequencyGraph(BURNDOWN_GRAPH nGraph);

	// Virtual overrides
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	
	void BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, CHMXDataset datasets[HMX_MAX_DATASET]);
	BOOL UpdateGraphStyles(CHMXDataset& dataset) const;

private:
	mutable CStringArray m_aAttribValues;
};

// ---------------------------------------------------------------------------

class CCategoryFrequencyGraph : public CFrequencyGraph
{
public:
	CCategoryFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CStatusFrequencyGraph : public CFrequencyGraph
{
public:
	CStatusFrequencyGraph();
	
protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CAllocatedToFrequencyGraph : public CFrequencyGraph
{
public:
	CAllocatedToFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CAllocatedByFrequencyGraph : public CFrequencyGraph
{
public:
	CAllocatedByFrequencyGraph();
	
protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CVersionFrequencyGraph : public CFrequencyGraph
{
public:
	CVersionFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CTagFrequencyGraph : public CFrequencyGraph
{
public:
	CTagFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CPriorityFrequencyGraph : public CFrequencyGraph
{
public:
	CPriorityFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

// ---------------------------------------------------------------------------

class CRiskFrequencyGraph : public CFrequencyGraph
{
public:
	CRiskFrequencyGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
};

/////////////////////////////////////////////////////////////////////////////

class CMinMaxGraph : public CGraphBase
{
public:
	virtual ~CMinMaxGraph();

protected:
	CMinMaxGraph(BURNDOWN_GRAPH nGraph);

	// Virtual overrides
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	mutable int m_nItemOffset;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentDaysMinMaxGraph : public CMinMaxGraph
{
public:
	CEstimatedSpentDaysMinMaxGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		ESTIMATED_DAYS,
		SPENT_DAYS,
	};
};

// ---------------------------------------------------------------------------

class CDueDoneDatesMinMaxGraph : public CMinMaxGraph
{
public:
	CDueDoneDatesMinMaxGraph();

protected:
	// Virtual overrides
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	enum
	{
		DUE_DATE,
		DONE_DATE,
	};
};



/////////////////////////////////////////////////////////////////////////////
