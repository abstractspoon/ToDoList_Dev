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
	virtual BOOL OnOptionChanged(BURNDOWN_GRAPHOPTION nOption, CHMXDataset datasets[HMX_MAX_DATASET]) const = 0;
	virtual BOOL HasAxes() const = 0;
	
	virtual void UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;

	BURNDOWN_GRAPH GetGraph() const { return m_nGraph; }
	BURNDOWN_GRAPHTYPE GetType() const { return m_nType; }
	BOOL HasType(BURNDOWN_GRAPHTYPE nType) const { return (m_nType == nType); }

	const CColorArray& GetColors() const;
	BOOL SetColors(const CColorArray& aColors);
	void SetDisplayISODates(BOOL bISO) { m_bISODates = bISO; }

	BURNDOWN_GRAPHOPTION GetOption() const;
	BOOL IsValidOption(BURNDOWN_GRAPHOPTION nOption) const;
	BOOL HasOption(BURNDOWN_GRAPHOPTION nOption) const;
	BOOL SetOption(BURNDOWN_GRAPHOPTION nOption);

	CString FormatDate(const COleDateTime& date) const;

protected:
	CGraphBase(BURNDOWN_GRAPH nGraph, BURNDOWN_GRAPHTYPE nType, BURNDOWN_GRAPHOPTION nOption = BGO_INVALID);
	
	static void SetDatasetColor(CHMXDataset& dataset, COLORREF crBase);
	static void ClearData(CHMXDataset datasets[HMX_MAX_DATASET]);

protected:
	BOOL InitColorPalette(int nNumColors, int nStartColor);
	BOOL InitColorPalette(COLORREF color1, COLORREF color2 = CLR_NONE, COLORREF color3 = CLR_NONE);
	COLORREF GetColor(int nColor) const;
	void RecalcDataMinMax(const CHMXDataset datasets[HMX_MAX_DATASET], double dIgnoreVal = HMX_DATASET_VALUE_NOIGNORE) const;

private:
	BURNDOWN_GRAPH m_nGraph;
	BURNDOWN_GRAPHTYPE m_nType;
	BURNDOWN_GRAPHOPTION m_nOption;
	CColorArray m_aColors;
	BOOL m_bISODates;

	mutable double m_dDataMin, m_dDataMax;
};

/////////////////////////////////////////////////////////////////////////////

class CGraphsMap : public CMap<BURNDOWN_GRAPH, BURNDOWN_GRAPH, CGraphBase*, CGraphBase*&>
{
public:
	CGraphsMap();
	~CGraphsMap();
	
	CString GetTitle(BURNDOWN_GRAPH nGraph) const;
	BURNDOWN_GRAPHTYPE GetType(BURNDOWN_GRAPH nGraph) const;

	CGraphBase* GetNext(POSITION& pos) const;
	CGraphBase* GetNext(POSITION& pos, BURNDOWN_GRAPH& nGraph) const;

	CGraphBase* GetGraph(BURNDOWN_GRAPH nGraph) const;
	CGraphBase* GetGraph(const CString& sCustomAttribID) const;
	BOOL HasGraph(BURNDOWN_GRAPH nGraph) const;

	int GetGraphs(BURNDOWN_GRAPHTYPE nType, CGraphArray& aGraphs, BOOL bSorted) const;
	int GetColors(CGraphColorMap& mapColors) const;
	int GetMaxColorCount() const;
	void SetAttributes(const CGraphAttributes& attrib);

	CString GetCustomAttributeID(BURNDOWN_GRAPH nGraph) const;
	CString GetCustomAttributeID(const CGraphBase* pGraph) const;

	BOOL Update(const CCustomAttributeDefinitionArray& aCustAttribDefs);

	void SetDisplayISODates(BOOL bISO);
	BOOL IsDisplayingISODates() const { return m_bISODates; }
	
protected:
	BOOL m_bISODates;

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

	// CGraphBase overrides
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	virtual BOOL OnOptionChanged(BURNDOWN_GRAPHOPTION nOption, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL HasAxes() const { return TRUE; }
	virtual void UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;

protected:
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
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CRemainingDaysTimeGraph : public CTimeSeriesGraph
{
public:
	CRemainingDaysTimeGraph();

protected:
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

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
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

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
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

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
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;

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

	// CGraphBase overrides
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	virtual void BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL UpdateGraphStyles(CHMXDataset& dataset) const;
	virtual BOOL OnOptionChanged(BURNDOWN_GRAPHOPTION nOption, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL HasAxes() const;
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;

private:
	mutable CStringArray m_aAttribValues;
};

// ---------------------------------------------------------------------------

class CCategoryFrequencyGraph : public CFrequencyGraph
{
public:
	CCategoryFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CStatusFrequencyGraph : public CFrequencyGraph
{
public:
	CStatusFrequencyGraph();
	
protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CAllocatedToFrequencyGraph : public CFrequencyGraph
{
public:
	CAllocatedToFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CAllocatedByFrequencyGraph : public CFrequencyGraph
{
public:
	CAllocatedByFrequencyGraph();
	
protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CVersionFrequencyGraph : public CFrequencyGraph
{
public:
	CVersionFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CTagFrequencyGraph : public CFrequencyGraph
{
public:
	CTagFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CPriorityFrequencyGraph : public CFrequencyGraph
{
public:
	CPriorityFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

// ---------------------------------------------------------------------------

class CRiskFrequencyGraph : public CFrequencyGraph
{
public:
	CRiskFrequencyGraph();

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

/////////////////////////////////////////////////////////////////////////////

class CMinMaxGraph : public CGraphBase
{
public:
	virtual ~CMinMaxGraph();

protected:
	CMinMaxGraph(BURNDOWN_GRAPH nGraph);

	// CGraphBase overrides
	virtual void RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const;
	virtual BOOL OnOptionChanged(BURNDOWN_GRAPHOPTION nOption, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL HasAxes() const { return TRUE; }
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;

protected:
	mutable int m_nItemOffset;
};

// ---------------------------------------------------------------------------

class CEstimatedSpentDaysMinMaxGraph : public CMinMaxGraph
{
public:
	CEstimatedSpentDaysMinMaxGraph();

protected:
	// CMinMaxGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;

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
	// CMinMaxGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;

protected:
	enum
	{
		DUE_DATE,
		DONE_DATE,
	};
};

/////////////////////////////////////////////////////////////////////////////

class CCustomAttributeGraph
{
public:
	BOOL UpdateDefinition(const CString& sLabel, const CString& sListData);
	CString GetUniqueID() const { return m_custDefinition.sUniqueID; }

protected:
	CCustomAttributeGraph(const CUSTOMATTRIBDEF& def);

protected:
	CUSTOMATTRIBDEF m_custDefinition;
};

/////////////////////////////////////////////////////////////////////////////

class CCustomAttributeTimeSeriesGraph : public CTimeSeriesGraph, public CCustomAttributeGraph
{
public:
	CCustomAttributeTimeSeriesGraph(const CUSTOMATTRIBDEF& def);

protected:
	// CTimeSeriesGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
	virtual BOOL CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual BOOL GetDataMinMax(double& dMin, double& dMax) const;
};

/////////////////////////////////////////////////////////////////////////////

class CCustomAttributeFrequencyGraph : public CFrequencyGraph, public CCustomAttributeGraph
{
public:
	CCustomAttributeFrequencyGraph(const CUSTOMATTRIBDEF& def);

protected:
	// CFrequencyGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
};

/////////////////////////////////////////////////////////////////////////////

/*
class CCustomAttributeMinMaxGraph : public CMinMaxGraph, public CCustomAttributeGraph
{
public:
	CCustomAttributeMinMaxGraph(const CUSTOMATTRIBDEF& def);

protected:
	// CMinMaxGraph overrides
	virtual CString GetTitle() const;
	virtual void BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const;
	virtual CString GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const;
};
*/

/////////////////////////////////////////////////////////////////////////////
