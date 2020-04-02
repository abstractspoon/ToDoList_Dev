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
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const = 0;
	virtual BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) = 0;

	BURNDOWN_GRAPHTYPE GetType() const;

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

protected:
	BOOL InitColorPalette(int nNumColors, int nStartColor);
	BOOL InitColorPalette(COLORREF color1, COLORREF color2 = CLR_NONE, COLORREF color3 = CLR_NONE);
	COLORREF GetColor(int nColor) const;

private:
	BURNDOWN_GRAPH m_nGraph;
	BURNDOWN_GRAPHOPTION m_nOption;
	CColorArray m_aColors;
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

	virtual CString GetTitle() const = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const = 0;

	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	CTimeSeriesGraph(BURNDOWN_GRAPH nGraph);

	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;

	static BOOL CalculateTrendLine(BURNDOWN_GRAPHOPTION nTrend, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);

private:
	static BOOL CalculateBestFitLine(const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);
	static BOOL CalculateMovingAverage(int nWindowSize, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest);

	static BURNDOWN_GRAPHSCALE CalculateRequiredScale(int nAvailWidth, int nNumDays);
};

// ---------------------------------------------------------------------------

class CIncompleteTasksGraph : public CTimeSeriesGraph
{
public:
	CIncompleteTasksGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

protected:
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CRemainingDaysGraph : public CTimeSeriesGraph
{
public:
	CRemainingDaysGraph();

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
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CStartedEndedTasksGraph : public CTimeSeriesGraph
{
public:
	CStartedEndedTasksGraph();

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
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentDaysGraph : public CTimeSeriesGraph
{
public:
	CEstimatedSpentDaysGraph();

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
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentCostGraph : public CTimeSeriesGraph
{
public:
	CEstimatedSpentCostGraph();

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
	BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

/////////////////////////////////////////////////////////////////////////////

class CAttributeFrequencyGraph : public CGraphBase
{
public:
	virtual ~CAttributeFrequencyGraph();

	virtual CString GetTitle() const = 0;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;

	CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption, const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	mutable CStringArray m_aAttribValues;

protected:
	CAttributeFrequencyGraph(BURNDOWN_GRAPH nGraph);

	void BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	BOOL UpdateGraphStyles(CHMXDataset& dataset) const;
};

// ---------------------------------------------------------------------------

class CCategoryFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CCategoryFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CStatusFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CStatusFrequencyGraph();
	
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CAllocatedToFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CAllocatedToFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CAllocatedByFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CAllocatedByFrequencyGraph();
	
	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CVersionFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CVersionFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CTagFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CTagFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CPriorityFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CPriorityFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CRiskFrequencyGraph : public CAttributeFrequencyGraph
{
public:
	CRiskFrequencyGraph();

	CString GetTitle() const;
	void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
