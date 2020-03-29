// BurndownChart.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BurndownGraphs.h"
#include "BurndownChart.h"
#include "BurndownStatic.h"

#include "..\shared\datehelper.h"
#include "..\shared\holdredraw.h"
#include "..\shared\enstring.h"
#include "..\shared\graphicsmisc.h"

////////////////////////////////////////////////////////////////////////////////

const COLORREF	COLOR_GREEN		= RGB(122, 204,   0); 
const COLORREF	COLOR_RED		= RGB(204,   0,   0); 
const COLORREF	COLOR_YELLOW	= RGB(204, 164,   0); 
const COLORREF	COLOR_BLUE		= RGB(0,     0, 244); 
const COLORREF	COLOR_PINK		= RGB(234,  28,  74); 
const COLORREF	COLOR_ORANGE	= RGB(255,  91,  21);

const int		GRAPH_LINE_THICKNESS = 1;
const int		TREND_LINE_THICKNESS = 2;
const int		MIN_XSCALE_SPACING	 = 50; // pixels

/////////////////////////////////////////////////////////////////////////////////

CGraphsMap::CGraphsMap()
{
	AddGraph(BCT_TIMESERIES_INCOMPLETETASKS,	new CIncompleteTasksGraph());
	AddGraph(BCT_TIMESERIES_REMAININGDAYS,		new CRemainingDaysGraph());
	AddGraph(BCT_TIMESERIES_STARTEDENDEDTASKS,	new CStartedEndedTasksGraph());
	AddGraph(BCT_TIMESERIES_ESTIMATEDSPENTDAYS, new CEstimatedSpentDaysGraph());
	//AddGraph(BCT_ESTIMATEDSPENTCOST,			new CEstimatedSpentCostGraph());

	AddGraph(BCT_FREQUENCY_CATEGORY,			new CCategoryFrequencyGraph());
	AddGraph(BCT_FREQUENCY_STATUS,				new CStatusFrequencyGraph());
	AddGraph(BCT_FREQUENCY_ALLOCTO,				new CAllocatedToFrequencyGraph());
	AddGraph(BCT_FREQUENCY_ALLOCBY,				new CAllocatedByFrequencyGraph());
	AddGraph(BCT_FREQUENCY_VERSION,				new CVersionFrequencyGraph());
	AddGraph(BCT_FREQUENCY_TAGS,				new CTagFrequencyGraph());
	AddGraph(BCT_FREQUENCY_PRIORITY,			new CPriorityFrequencyGraph());
	AddGraph(BCT_FREQUENCY_RISK,				new CRiskFrequencyGraph());
}

CGraphsMap::~CGraphsMap()
{
	POSITION pos = GetStartPosition();

	while (pos)
		delete GetNext(pos);

	RemoveAll();
}

BOOL CGraphsMap::AddGraph(BURNDOWN_GRAPH nGraph, CGraphBase* pGraph)
{
	if (HasGraph(nGraph))
	{
		ASSERT(0);
		delete pGraph;
		return FALSE;
	}

	SetAt(nGraph, pGraph);
	return TRUE;
}

CGraphBase* CGraphsMap::GetNext(POSITION& pos) const
{
	BURNDOWN_GRAPH nUnused;

	return GetNext(pos, nUnused);
}

CGraphBase* CGraphsMap::GetNext(POSITION& pos, BURNDOWN_GRAPH& nGraph) const
{
	CGraphBase* pGraph = NULL;
	GetNextAssoc(pos, nGraph, pGraph);

	return pGraph;
}

CGraphBase* CGraphsMap::GetGraph(BURNDOWN_GRAPH nGraph) const
{
	CGraphBase* pGraph = NULL;

	if (Lookup(nGraph, pGraph))
		ASSERT(pGraph);

	return pGraph;
}

BOOL CGraphsMap::HasGraph(BURNDOWN_GRAPH nGraph) const
{
	return (GetGraph(nGraph) != NULL);
}

/////////////////////////////////////////////////////////////////////////////

CGraphBase::CGraphBase(BURNDOWN_GRAPH nGraph) : m_nGraph(nGraph), m_nOption(BGO_NONE)
{
	ASSERT(IsValidGraph(m_nGraph));
}

CGraphBase::~CGraphBase()
{
}

void CGraphBase::UpdateDatasetColors(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	int nNumColors = m_aColors.GetSize();

	for (int nColor = 0; nColor < min(nNumColors, HMX_MAX_DATASET); nColor++)
		SetDatasetColor(datasets[nColor], m_aColors[nColor]);
}

void CGraphBase::SetDatasetColor(CHMXDataset& dataset, COLORREF crBase)
{
	dataset.SetLineColor(GraphicsMisc::Darker(crBase, 0.05, FALSE));
	dataset.SetFillColor(GraphicsMisc::Lighter(crBase, 0.25, FALSE));
}

BURNDOWN_GRAPHTYPE CGraphBase::GetType() const
{
	return GetGraphType(m_nGraph);
}

BURNDOWN_GRAPHOPTION CGraphBase::GetOption() const
{
	return m_nOption;
}

COLORREF CGraphBase::GetColor(int nColor) const
{
	int nNumColors = GetNumColors();

	if ((nColor < 0) || (nNumColors == 0))
	{
		ASSERT(0);
		return CLR_NONE;
	}

	// Rotate through colours
	nColor = (nColor % nNumColors);
	ASSERT(nColor < nNumColors);

	return m_aColors[nColor];
}

BOOL CGraphBase::SetOption(BURNDOWN_GRAPHOPTION nOption)
{
	if (!IsValidOption(nOption))
	{
		ASSERT(0);
		return FALSE;
	}

	m_nOption = nOption;
	return TRUE;
}

BOOL CGraphBase::SetColors(COLORREF color1, COLORREF color2, COLORREF color3)
{
	return (m_aColors.Set(color1, color2, color3) > 0);
}

BOOL CGraphBase::SetColors(const CColorArray& aColors)
{
	if (!aColors.GetSize() || (aColors[0] == CLR_NONE))
	{
		ASSERT(0);
		return FALSE;
	}

	m_aColors.Copy(aColors);
	return FALSE;
}

const CColorArray& CGraphBase::GetColors() const
{
	return m_aColors;
}

int CGraphBase::GetNumColors() const
{
	return m_aColors.GetSize();
}

BOOL CGraphBase::IsValidOption(BURNDOWN_GRAPHOPTION nOption) const
{
	return ::IsValidOption(nOption, m_nGraph);
}

////////////////////////////////////////////////////////////////////////////////////////

CTimeSeriesGraph::CTimeSeriesGraph(BURNDOWN_GRAPH nGraph) : CGraphBase(nGraph)
{
}

CTimeSeriesGraph::~CTimeSeriesGraph()
{
}

BOOL CTimeSeriesGraph::CalculateBestFitLine(const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	int nNumData = datasetSrc.GetDatasetSize();

	if (nNumData < 2)
		return FALSE;

	double dSumX = 0, dSumXSquared = 0, dSumY = 0, dSumXY = 0;
	int nData;

	for (nData = 0; nData < nNumData; nData++)
	{
		double dXValue = nData, dYValue;
		VERIFY(datasetSrc.GetData(nData, dYValue));

		dSumX			+= dXValue;
		dSumY			+= dYValue;
		dSumXSquared	+= (dXValue * dXValue);
		dSumXY			+= (dXValue * dYValue);
	}
	
	double dSlope = ((nNumData * dSumXY) - (dSumX * dSumY)) / ((nNumData * dSumXSquared) - (dSumX * dSumX));
	double dIntercept = ((dSumXSquared * dSumY) - (dSumX * dSumXY)) / ((dSumXSquared * nNumData) - (dSumX * dSumX));

	// Calculate the data points
	datasetDest.SetDatasetSize(nNumData);

	for (nData = 0; nData < nNumData; nData++)
	{
		double dLine = ((dSlope * nData) + dIntercept);
		dLine = max(dLine, 0.0);

		datasetDest.SetData(nData, dLine);
	}

	return TRUE;
}

BOOL CTimeSeriesGraph::CalculateMovingAverage(int nWindowSize, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	int nNumData = datasetSrc.GetDatasetSize();

	if (nNumData < nWindowSize)
		return FALSE;

	datasetDest.SetDatasetSize(nNumData);

	// The first value is the first data value
	double dValue = 0, dPrevAverage = 0, dPrevWindowStart = 0;

	datasetSrc.GetData(0, dValue);
	datasetDest.SetData(0, dValue);
	
	// The first 'nWindow' values are simple accumulated averages
	int nData;

	for (nData = 1; nData < nWindowSize; nData++)
	{
		datasetDest.GetData((nData - 1), dPrevAverage);
		datasetSrc.GetData(nData, dValue);

		dValue = (((dPrevAverage * nData) + dValue) / (nData + 1));
		datasetDest.SetData(nData, dValue);
	}

	// All the rest require the rolling 
	for (nData = nWindowSize; nData < nNumData; nData++)
	{
		datasetDest.GetData((nData - 1), dPrevAverage);
		datasetSrc.GetData(nData - nWindowSize, dPrevWindowStart);
		datasetSrc.GetData(nData, dValue);

		dValue = (((dPrevAverage * nWindowSize) - dPrevWindowStart + dValue) / nWindowSize);
		datasetDest.SetData(nData, dValue);
	}
	
	return TRUE;
}

void CTimeSeriesGraph::RebuildXScale(const CStatsItemCalculator& calculator, int nAvailWidth, CStringArray& aLabels, int& nLabelStep) const
{
	// Because we often have an uneven label spacing we need
	// to specify labels only a day by day granularity
	nLabelStep = 1;

	int nNumDays = calculator.GetTotalDays();
	aLabels.SetSize(nNumDays + 1);

	// build ticks
	COleDateTime dtTick = calculator.GetStartDate();
	CDateHelper dh;

	BURNDOWN_GRAPHSCALE nScale = CalculateRequiredScale(nAvailWidth, nNumDays);

	for (int nDay = 0; nDay <= nNumDays; )
	{
		aLabels.SetAt(nDay, dh.FormatDate(dtTick));

		// next Tick date
		COleDateTime dtNextTick(dtTick);

		switch (nScale)
		{
		case BCS_DAY:
			dtNextTick.m_dt += 1.0;
			break;

		case BCS_WEEK:
			dh.OffsetDate(dtNextTick, 1, DHU_WEEKS);
			break;

		case BCS_MONTH:
			dh.OffsetDate(dtNextTick, 1, DHU_MONTHS);
			break;

		case BCS_2MONTH:
			dh.OffsetDate(dtNextTick, 2, DHU_MONTHS);
			break;

		case BCS_QUARTER:
			dh.OffsetDate(dtNextTick, 3, DHU_MONTHS);
			break;

		case BCS_HALFYEAR:
			dh.OffsetDate(dtNextTick, 6, DHU_MONTHS);
			break;

		case BCS_YEAR:
			dh.OffsetDate(dtNextTick, 1, DHU_YEARS);
			break;

		default:
			ASSERT(0);
		}

		nDay += (int)(dtNextTick.m_dt - dtTick.m_dt);

		dtTick = dtNextTick;
	}
}

BURNDOWN_GRAPHSCALE CTimeSeriesGraph::CalculateRequiredScale(int nAvailWidth, int nNumDays)
{
	// work thru the available scales until we find a suitable one
	for (int nScale = 0; nScale < NUM_SCALES; nScale++)
	{
		int nSpacing = MulDiv(SCALES[nScale], nAvailWidth, nNumDays);

		if (nSpacing > MIN_XSCALE_SPACING)
			return SCALES[nScale];
	}

	return BCS_YEAR;
}

BOOL CTimeSeriesGraph::SetOption(BURNDOWN_GRAPHOPTION nOption, CHMXDataset datasets[HMX_MAX_DATASET])
{
	if (!CGraphBase::SetOption(nOption))
		return FALSE;

	if (!CalculateTrendLines(datasets))
		VERIFY(CGraphBase::SetOption(BGO_NONE));

	return TRUE;;
}

BOOL CTimeSeriesGraph::CalculateTrendLine(BURNDOWN_GRAPHOPTION nOption, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	BOOL bSuccess = FALSE;

	switch (nOption)
	{
	case BGO_NONE:
		bSuccess = TRUE;
		break;

	case BGO_TREND_BESTFIT:
		bSuccess = CalculateBestFitLine(datasetSrc, datasetDest);
		break;

	case BGO_TREND_7DAYAVERAGE:
		bSuccess = CalculateMovingAverage(7, datasetSrc, datasetDest);
		break;

	case BGO_TREND_30DAYAVERAGE:
		bSuccess = CalculateMovingAverage(30, datasetSrc, datasetDest);
		break;

	case BGO_TREND_90DAYAVERAGE:
		bSuccess = CalculateMovingAverage(90, datasetSrc, datasetDest);
		break;

	default:
		ASSERT(0);
		break;
	}

	if ((nOption == BGO_NONE) || !bSuccess)
		datasetDest.Reset();

	if (bSuccess)
	{
		datasetDest.SetSize(TREND_LINE_THICKNESS);
		datasetDest.SetStyle(HMX_DATASET_STYLE_LINE_DOTTED); 
		datasetDest.SetLineColor(GraphicsMisc::Darker(datasetSrc.GetLineColor(), 0.2, FALSE));
	}

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////

CIncompleteTasksGraph::CIncompleteTasksGraph()
	:
	CTimeSeriesGraph(BCT_TIMESERIES_INCOMPLETETASKS)
{
	SetColors(COLOR_GREEN);
}

CString CIncompleteTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_INCOMPLETE);
}

void CIncompleteTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetSize(GRAPH_LINE_THICKNESS);
	datasets[0].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays)
	{
		datasets[0].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		int nItemFrom = 0;
		int nNumNotDone = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				nNumNotDone = calculator.GetIncompleteTaskCount(date, nItemFrom, nItemFrom);

			datasets[0].SetData(nDay, nNumNotDone);
		}

		// Set the maximum Y value to be something 'nice'
		double dMin, dMax;

		if (HMXUtils::GetMinMax(datasets, 1, dMin, dMax, true))
		{
			ASSERT(dMin == 0.0);

			dMax = HMXUtils::CalcMaxYAxisValue(dMax, 10);
			datasets[0].SetMax(dMax);
		}
	}

	CalculateTrendLines(datasets);
}

CString CIncompleteTasksGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	CString sTooltip;
	double dNumTasks;

	if (datasets[0].GetData(nHit, dNumTasks))
	{
		double dDate = (calculator.GetStartDate() + nHit);
		sTooltip.Format(CEnString(IDS_TOOLTIP_INCOMPLETE), CDateHelper::FormatDate(dDate), (int)dNumTasks);
	}

	return sTooltip;
}

BOOL CIncompleteTasksGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	return CalculateTrendLine(GetOption(), datasets[0], datasets[1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CRemainingDaysGraph::CRemainingDaysGraph()
	:
	CTimeSeriesGraph(BCT_TIMESERIES_REMAININGDAYS)
{
	SetColors(COLOR_BLUE, COLOR_YELLOW);
}

CString CRemainingDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_REMAINING);
}

void CRemainingDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[REMAINING_ESTIMATE].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_ESTIMATE].SetSize(GRAPH_LINE_THICKNESS);
	datasets[REMAINING_ESTIMATE].SetMin(0.0);
	
	datasets[REMAINING_SPENT].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_SPENT].SetSize(GRAPH_LINE_THICKNESS);
	datasets[REMAINING_SPENT].SetMin(0.0);
	
	// build the graph
	int nNumDays = calculator.GetTotalDays();
	
	if (nNumDays > 0)
	{
		datasets[REMAINING_ESTIMATE].SetDatasetSize(nNumDays + 1);
		datasets[REMAINING_SPENT].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dTotalEst = calculator.GetDaysEstimated();

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// Time Estimate
			double dEst = ((nDay * dTotalEst) / nNumDays);

			datasets[REMAINING_ESTIMATE].SetData(nDay, dTotalEst - dEst);
		
			// Time Spent
			double dSpent = calculator.GetDaysSpent(date);
		
			datasets[REMAINING_SPENT].SetData(nDay, dTotalEst - dSpent);
		}

		// Ensure last Time Estimate value is always zero
		datasets[REMAINING_ESTIMATE].SetData(nNumDays, 0.0);
	
		// Set the maximum Y value to be something 'nice'
		if (dTotalEst > 0)
		{
			double dMax = HMXUtils::CalcMaxYAxisValue(dTotalEst, 10);

			datasets[REMAINING_ESTIMATE].SetMax(dMax);
			datasets[REMAINING_SPENT].SetMax(dMax);
		}
	}

	CalculateTrendLines(datasets);
}

CString CRemainingDaysGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dNumEst, dNumSpent;
	CString sTooltip;

	if (datasets[REMAINING_SPENT].GetData(nHit, dNumSpent) &&
		datasets[REMAINING_ESTIMATE].GetData(nHit, dNumEst))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_REMAINING), CDateHelper::FormatDate(dDate), (int)dNumEst, (int)dNumSpent);
	}

	return sTooltip;
}

BOOL CRemainingDaysGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	// No need to calculate trend of remaining estimate because it's already a straight line
	return CalculateTrendLine(GetOption(), datasets[REMAINING_SPENT], datasets[REMAINING_SPENT + 1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CStartedEndedTasksGraph::CStartedEndedTasksGraph()
	:
	CTimeSeriesGraph(BCT_TIMESERIES_STARTEDENDEDTASKS)
{
	SetColors(COLOR_RED, COLOR_GREEN);
}

CString CStartedEndedTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_STARTEDENDED);
}

void CStartedEndedTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[STARTED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[STARTED_TASKS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[STARTED_TASKS].SetMin(0.0);

	datasets[ENDED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ENDED_TASKS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[ENDED_TASKS].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[STARTED_TASKS].SetDatasetSize(nNumDays + 1);
		datasets[ENDED_TASKS].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		int nNumStarted = 0, nNumDone = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetStartedEndedTasks(date, nNumStarted, nNumDone);

			datasets[STARTED_TASKS].SetData(nDay, nNumStarted);
			datasets[ENDED_TASKS].SetData(nDay, nNumDone);
		}

		// Set the maximum Y value to be something 'nice'
		double dMax = 0.0;
		
		// Last started value will always be largest
		if (datasets[STARTED_TASKS].GetData(nNumDays, dMax))
		{
			dMax = HMXUtils::CalcMaxYAxisValue(dMax, 10);

			datasets[STARTED_TASKS].SetMax(dMax);
			datasets[ENDED_TASKS].SetMax(dMax);
		}
	}

	CalculateTrendLines(datasets);
}

CString CStartedEndedTasksGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dNumStarted, nNumDone;
	CString sTooltip;

	if (datasets[STARTED_TASKS].GetData(nHit, dNumStarted) &&
		datasets[ENDED_TASKS].GetData(nHit, nNumDone))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_STARTEDENDED), CDateHelper::FormatDate(dDate), (int)dNumStarted, (int)nNumDone);
	}

	return sTooltip;
}

BOOL CStartedEndedTasksGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[STARTED_TASKS], datasets[STARTED_TASKS + 2]) &&
		CalculateTrendLine(GetOption(), datasets[ENDED_TASKS], datasets[ENDED_TASKS + 2]))
	{
		return TRUE;
	}

	datasets[STARTED_TASKS + 2].Reset();
	datasets[ENDED_TASKS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CEstimatedSpentDaysGraph::CEstimatedSpentDaysGraph()
	:
	CTimeSeriesGraph(BCT_TIMESERIES_ESTIMATEDSPENTDAYS)
{
	SetColors(COLOR_PINK, COLOR_GREEN);
}

CString CEstimatedSpentDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTDAYS);
}

void CEstimatedSpentDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[ESTIMATED_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_DAYS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[ESTIMATED_DAYS].SetMin(0.0);

	datasets[SPENT_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_DAYS].SetSize(GRAPH_LINE_THICKNESS);
	datasets[SPENT_DAYS].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[ESTIMATED_DAYS].SetDatasetSize(nNumDays + 1);
		datasets[SPENT_DAYS].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dDaysEst = 0.0, dDaysSpent = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetDaysEstimatedSpent(date, dDaysEst, dDaysSpent);

			datasets[ESTIMATED_DAYS].SetData(nDay, dDaysEst);
			datasets[SPENT_DAYS].SetData(nDay, dDaysSpent);
		}

		// Set the maximum Y value to be something 'nice'
		double dMaxEst = 0.0, dMaxSpent = 0.0;
		
		// Last values will always be largest
		bool bHasMax = datasets[ESTIMATED_DAYS].GetData(nNumDays, dMaxEst);
		bHasMax |= datasets[SPENT_DAYS].GetData(nNumDays, dMaxSpent);
			
		if (bHasMax)
		{
			double dMax = HMXUtils::CalcMaxYAxisValue(max(dMaxEst, dMaxSpent), 10);

			datasets[ESTIMATED_DAYS].SetMax(dMax);
			datasets[SPENT_DAYS].SetMax(dMax);
		}
	}

	CalculateTrendLines(datasets);
}

CString CEstimatedSpentDaysGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dDaysEst, dDaysSpent;
	CString sTooltip;

	if (datasets[ESTIMATED_DAYS].GetData(nHit, dDaysEst) &&
		datasets[SPENT_DAYS].GetData(nHit, dDaysSpent))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_ESTIMATEDSPENT), CDateHelper::FormatDate(dDate), (int)dDaysEst, (int)dDaysSpent);
	}

	return sTooltip;
}

BOOL CEstimatedSpentDaysGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[ESTIMATED_DAYS], datasets[ESTIMATED_DAYS + 2]) &&
		CalculateTrendLine(GetOption(), datasets[SPENT_DAYS], datasets[SPENT_DAYS + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_DAYS + 2].Reset();
	datasets[SPENT_DAYS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CEstimatedSpentCostGraph::CEstimatedSpentCostGraph()
	:
	CTimeSeriesGraph(BCT_TIMESERIES_ESTIMATEDSPENTCOST)
{
	SetColors(COLOR_YELLOW, COLOR_PINK);
}

CString CEstimatedSpentCostGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTCOST);
}

void CEstimatedSpentCostGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	UpdateDatasetColors(datasets);

	datasets[ESTIMATED_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_COST].SetSize(GRAPH_LINE_THICKNESS);
	datasets[ESTIMATED_COST].SetMin(0.0);

	datasets[SPENT_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_COST].SetSize(GRAPH_LINE_THICKNESS);
	datasets[SPENT_COST].SetMin(0.0);

	// build the graph
	int nNumDays = calculator.GetTotalDays();

	if (nNumDays > 0)
	{
		datasets[ESTIMATED_COST].SetDatasetSize(nNumDays + 1);
		datasets[SPENT_COST].SetDatasetSize(nNumDays + 1);

		COleDateTime dtStart = calculator.GetStartDate(), date(dtStart);
		double dCostEst = 0.0, dCostSpent = 0;

		for (int nDay = 0; nDay <= nNumDays; nDay++, date.m_dt++)
		{
			// First value is always zero
			if (nDay > 0)
				calculator.GetCostEstimatedSpent(date, dCostEst, dCostSpent);

			datasets[ESTIMATED_COST].SetData(nDay, dCostEst);
			datasets[SPENT_COST].SetData(nDay, dCostSpent);
		}

		// Set the maximum Y value to be something 'nice'
		double dMaxEst = 0.0, dMaxSpent = 0.0;
		
		// Last values will always be largest
		bool bHasMax = datasets[ESTIMATED_COST].GetData(nNumDays, dMaxEst);
		bHasMax |= datasets[SPENT_COST].GetData(nNumDays, dMaxSpent);
			
		if (bHasMax)
		{
			double dMax = HMXUtils::CalcMaxYAxisValue(max(dMaxEst, dMaxSpent), 10);

			datasets[ESTIMATED_COST].SetMax(dMax);
			datasets[SPENT_COST].SetMax(dMax);
		}
	}

	CalculateTrendLines(datasets);
}

CString CEstimatedSpentCostGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	ASSERT(nHit != -1);

	double dDate = (calculator.GetStartDate() + nHit), dCostEst, dCostSpent;
	CString sTooltip;

	if (datasets[ESTIMATED_COST].GetData(nHit, dCostEst) &&
		datasets[SPENT_COST].GetData(nHit, dCostSpent))
	{
		sTooltip.Format(CEnString(IDS_TOOLTIP_ESTIMATEDSPENT), CDateHelper::FormatDate(dDate), (int)dCostEst, (int)dCostSpent);
	}

	return sTooltip;
}

BOOL CEstimatedSpentCostGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	if (CalculateTrendLine(GetOption(), datasets[ESTIMATED_COST], datasets[ESTIMATED_COST + 2]) &&
		CalculateTrendLine(GetOption(), datasets[SPENT_COST], datasets[SPENT_COST + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_COST + 2].Reset();
	datasets[SPENT_COST + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CAttributeFrequencyGraph::CAttributeFrequencyGraph(BURNDOWN_GRAPH nGraph) : CGraphBase(nGraph)
{

}

CAttributeFrequencyGraph::~CAttributeFrequencyGraph()
{

}

void CAttributeFrequencyGraph::RebuildXScale(const CStatsItemCalculator& /*calculator*/, int /*nAvailWidth*/, CStringArray& aLabels, int& nLabelStep) const
{
	nLabelStep = 1;
	aLabels.Copy(m_aAttribValues);
}

void CAttributeFrequencyGraph::BuildGraph(const CArray<FREQUENCYITEM, FREQUENCYITEM&>& aFrequencies, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	int nNumColors = GetNumColors();

	if (!nNumColors)
	{
		ASSERT(0);
		return;
	}

	// Save off attrib values for building horizontal labels
	m_aAttribValues.RemoveAll();

	int nNumAttrib = aFrequencies.GetSize();

	if (nNumAttrib)
	{
		// Use a different dataset for as many colours as we
		// have so that each data point has a different color
		int nMaxFreq = 0;
		int nNumDatasets = min(nNumColors, HMX_MAX_DATASET);

		for (int nAttrib = 0; nAttrib < nNumAttrib; nAttrib++)
		{
			int nDataset = (nAttrib % nNumDatasets);

			CHMXDataset& dataset = datasets[nDataset];

			// Initialise each dataset once only
			if (nAttrib < nNumDatasets)
			{
				SetDatasetColor(dataset, GetColor(nDataset));

				dataset.SetStyle(HMX_DATASET_STYLE_VBAR);
				dataset.SetSize(5); // 50% of spacing
				dataset.SetMin(0.0);
				dataset.ClearData();
				dataset.SetDatasetSize(nNumAttrib);
			}

			const FREQUENCYITEM& fi = aFrequencies[nAttrib];
			dataset.SetData(nAttrib, fi.nCount);

			if (fi.sLabel.IsEmpty())
				m_aAttribValues.Add(CEnString(IDS_NONE));
			else
				m_aAttribValues.Add(fi.sLabel);

			nMaxFreq = max(nMaxFreq, fi.nCount);
		}

		// Set the maximum Y value to be something 'nice'
		if (nMaxFreq)
		{
			double dMax = HMXUtils::CalcMaxYAxisValue(nMaxFreq, 10);

			int nDataset = nNumDatasets;

			while (nDataset--)
				datasets[nDataset].SetMax(dMax);
		}
	}
}

CString CAttributeFrequencyGraph::GetTooltip(const CStatsItemCalculator& /*calculator*/, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	CString sTooltip;
	double dCount = 0.0;
	
	if (datasets[0].GetData(nHit, dCount))
	{
		ASSERT(m_aAttribValues.GetSize() > nHit);
		sTooltip.Format(CEnString(IDS_TOOLTIP_ATTRIBFREQUENCY), m_aAttribValues[nHit], (int)dCount);
	}

	return sTooltip;
}

BOOL CAttributeFrequencyGraph::SetOption(BURNDOWN_GRAPHOPTION /*nOption*/, CHMXDataset /*datasets*/[HMX_MAX_DATASET])
{
	return FALSE;
}

// ---------------------------------------------------------------------------

CCategoryFrequencyGraph::CCategoryFrequencyGraph() 
	: 
	CAttributeFrequencyGraph(BCT_FREQUENCY_CATEGORY)
{
	SetColors(COLOR_BLUE, COLOR_RED, COLOR_YELLOW);
}

CString CCategoryFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_CATEGORY);
}

void CCategoryFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetCategoryFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CStatusFrequencyGraph::CStatusFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_STATUS)
{
	SetColors(COLOR_GREEN, COLOR_ORANGE, COLOR_BLUE);
}

CString CStatusFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_STATUS);
}

void CStatusFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetStatusFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CAllocatedToFrequencyGraph::CAllocatedToFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_ALLOCTO)
{
	SetColors(COLOR_PINK, COLOR_YELLOW, COLOR_GREEN);
}

CString CAllocatedToFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_ALLOCTO);
}

void CAllocatedToFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetAllocatedToFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CAllocatedByFrequencyGraph::CAllocatedByFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_ALLOCBY)
{
	SetColors(COLOR_RED, COLOR_GREEN, COLOR_YELLOW);
}

CString CAllocatedByFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_ALLOCBY);
}

void CAllocatedByFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetAllocatedByFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CVersionFrequencyGraph::CVersionFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_VERSION)
{
	SetColors(COLOR_YELLOW, COLOR_PINK, COLOR_BLUE);
}

CString CVersionFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_VERSION);
}

void CVersionFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetVersionFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CTagFrequencyGraph::CTagFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_TAGS)
{
	SetColors(COLOR_ORANGE, COLOR_GREEN, COLOR_PINK);
}

CString CTagFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_TAGS);
}

void CTagFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetTagFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CPriorityFrequencyGraph::CPriorityFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_PRIORITY)
{
	SetColors(COLOR_BLUE, COLOR_ORANGE, COLOR_PINK);
}

CString CPriorityFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_PRIORITY);
}

void CPriorityFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetPriorityFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

// ---------------------------------------------------------------------------

CRiskFrequencyGraph::CRiskFrequencyGraph()
	:
	CAttributeFrequencyGraph(BCT_FREQUENCY_RISK)
{
	SetColors(COLOR_GREEN, COLOR_RED, COLOR_BLUE);
}

CString CRiskFrequencyGraph::GetTitle() const
{
	return CEnString(IDS_FREQUENCY_RISK);
}

void CRiskFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetRiskFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

