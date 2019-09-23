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

/////////////////////////////////////////////////////////////////////////////

CGraphBase::CGraphBase()
{
}

CGraphBase::~CGraphBase()
{
}

int CGraphBase::HitTest(const CStatsItemCalculator& calculator, const COleDateTime& date) const
{
	if ((date.m_dt < calculator.GetStartDate()) || (date.m_dt >= calculator.GetEndDate()))
		return -1;

	// else
	return (int)(date.m_dt - calculator.GetStartDate());
}

void CGraphBase::SetDatasetColor(CHMXDataset& dataset, COLORREF crBase)
{
	dataset.SetLineColor(GraphicsMisc::Darker(crBase, 0.05, FALSE));
	dataset.SetFillColor(GraphicsMisc::Lighter(crBase, 0.25, FALSE));
}

////////////////////////////////////////////////////////////////////////////////////////

CTimeSeriesGraph::CTimeSeriesGraph() : m_nTrend(BTL_NONE)
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

BOOL CTimeSeriesGraph::ShowTrendLine(BURNDOWN_TREND nTrend, CHMXDataset datasets[HMX_MAX_DATASET])
{
	if (nTrend == m_nTrend)
		return TRUE;

	if (CalculateTrendLines(datasets, nTrend))
		m_nTrend = nTrend;
	else
		m_nTrend = BTL_NONE;

	return (m_nTrend == nTrend);
}

BOOL CTimeSeriesGraph::CalculateTrendLine(BURNDOWN_TREND nTrend, const CHMXDataset& datasetSrc, CHMXDataset& datasetDest)
{
	BOOL bSuccess = FALSE;

	switch (nTrend)
	{
	case BTL_NONE:
		bSuccess = TRUE;
		break;

	case BTL_BEST_FIT:
		bSuccess = CalculateBestFitLine(datasetSrc, datasetDest);
		break;

	case BTL_7DAY_ROLLING_AVERAGE:
		bSuccess = CalculateMovingAverage(7, datasetSrc, datasetDest);
		break;

	case BTL_30DAY_ROLLING_AVERAGE:
		bSuccess = CalculateMovingAverage(30, datasetSrc, datasetDest);
		break;

	default:
		ASSERT(0);
		break;
	}

	if ((nTrend == BTL_NONE) || !bSuccess)
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

CString CIncompleteTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_INCOMPLETE);
}

void CIncompleteTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets[0], COLOR_GREEN);

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

	CalculateTrendLines(datasets, m_nTrend);
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

BOOL CIncompleteTasksGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const
{
	return CalculateTrendLine(nTrend, datasets[0], datasets[1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CString CRemainingDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_REMAINING);
}

void CRemainingDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets[REMAINING_ESTIMATE], COLOR_BLUE);
	SetDatasetColor(datasets[REMAINING_SPENT], COLOR_YELLOW);

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

BOOL CRemainingDaysGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const
{
	// No need to calculate trend of remaining estimate because it's already a straight line
	return CalculateTrendLine(nTrend, datasets[REMAINING_SPENT], datasets[REMAINING_SPENT + 1]);
}

//////////////////////////////////////////////////////////////////////////////////////

CString CStartedEndedTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_STARTEDENDED);
}

void CStartedEndedTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets[STARTED_TASKS], COLOR_GREEN);
	SetDatasetColor(datasets[ENDED_TASKS], COLOR_RED);

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

	CalculateTrendLines(datasets, m_nTrend);
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

BOOL CStartedEndedTasksGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const
{
	if (CalculateTrendLine(nTrend, datasets[STARTED_TASKS], datasets[STARTED_TASKS + 2]) &&
		CalculateTrendLine(nTrend, datasets[ENDED_TASKS], datasets[ENDED_TASKS + 2]))
	{
		return TRUE;
	}

	datasets[STARTED_TASKS + 2].Reset();
	datasets[ENDED_TASKS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CString CEstimatedSpentDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTDAYS);
}

void CEstimatedSpentDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets[ESTIMATED_DAYS], COLOR_PINK);
	SetDatasetColor(datasets[SPENT_DAYS], COLOR_GREEN);

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

	CalculateTrendLines(datasets, m_nTrend);
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

BOOL CEstimatedSpentDaysGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const
{
	if (CalculateTrendLine(nTrend, datasets[ESTIMATED_DAYS], datasets[ESTIMATED_DAYS + 2]) &&
		CalculateTrendLine(nTrend, datasets[SPENT_DAYS], datasets[SPENT_DAYS + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_DAYS + 2].Reset();
	datasets[SPENT_DAYS + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CString CEstimatedSpentCostGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTCOST);
}

void CEstimatedSpentCostGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets[ESTIMATED_COST], COLOR_PINK);
	SetDatasetColor(datasets[SPENT_COST], COLOR_GREEN);

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

	CalculateTrendLines(datasets, m_nTrend);
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

BOOL CEstimatedSpentCostGraph::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], BURNDOWN_TREND nTrend) const
{
	if (CalculateTrendLine(nTrend, datasets[ESTIMATED_COST], datasets[ESTIMATED_COST + 2]) &&
		CalculateTrendLine(nTrend, datasets[SPENT_COST], datasets[SPENT_COST + 2]))
	{
		return TRUE;
	}

	datasets[ESTIMATED_COST + 2].Reset();
	datasets[SPENT_COST + 2].Reset();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CAttributeFrequencyGraph::CAttributeFrequencyGraph()
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
	// Save off attrib values for building horizontal labels
	m_aAttribValues.RemoveAll();

	int nNumAttrib = aFrequencies.GetSize();

	if (nNumAttrib)
	{
		SetDatasetColor(datasets[0], COLOR_PINK);

		datasets[0].SetStyle(HMX_DATASET_STYLE_VBAR);
		datasets[0].SetSize(5);
		datasets[0].SetMin(0.0);
		datasets[0].SetDatasetSize(nNumAttrib);

		for (int nItem = 0; nItem < nNumAttrib; nItem++)
		{
			datasets[0].SetData(nItem, aFrequencies[nItem].nCount);

			if (aFrequencies[nItem].sLabel.IsEmpty())
				m_aAttribValues.Add(_T("<none>"));
			else
				m_aAttribValues.Add(aFrequencies[nItem].sLabel);
		}
	}
}

// ---------------------------------------------------------------------------

CCategoryFrequencyGraph::CCategoryFrequencyGraph()
{

}

CCategoryFrequencyGraph::~CCategoryFrequencyGraph()
{

}

CString CCategoryFrequencyGraph::GetTitle() const
{
	return _T("Category Frequency");
}

void CCategoryFrequencyGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	CArray<FREQUENCYITEM, FREQUENCYITEM&> aFrequencies;
	calculator.GetCategoryFrequencies(aFrequencies);

	CAttributeFrequencyGraph::BuildGraph(aFrequencies, datasets);
}

CString CCategoryFrequencyGraph::GetTooltip(const CStatsItemCalculator& calculator, const CHMXDataset datasets[HMX_MAX_DATASET], int nHit) const
{
	// TODO
	return _T("");
}


