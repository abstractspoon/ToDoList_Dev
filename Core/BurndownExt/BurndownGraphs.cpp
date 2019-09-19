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

const int		LINE_THICKNESS	= 1;

/////////////////////////////////////////////////////////////////////////////

CGraphBase::CGraphBase() : m_dwTrends(0)
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

void CGraphBase::SetDatasetColor(CHMXDataset datasets[HMX_MAX_DATASET], int nDataset, COLORREF crBase)
{
	datasets[nDataset].SetLineColor(GraphicsMisc::Darker(crBase, 0.05, FALSE));
	datasets[nDataset].SetFillColor(GraphicsMisc::Lighter(crBase, 0.25, FALSE));
}

BOOL CGraphBase::CalculateBestFitLine(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest)
{
	// TODO
	return FALSE;
}

BOOL CGraphBase::CalculateMovingAverage(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest, int nWindowSize)
{
	// TODO
	return FALSE;
}

BOOL CGraphBase::CopyDataset(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest)
{
	if ((nDatasetSrc < 0) || (nDatasetSrc >= HMX_MAX_DATASET) ||
		(nDatasetDest < 0) || (nDatasetDest >= HMX_MAX_DATASET))
	{
		return FALSE;
	}

	datasets[nDatasetDest].Copy(datasets[nDatasetSrc]);
	return TRUE;
}

BOOL CGraphBase::MoveDataset(CHMXDataset datasets[HMX_MAX_DATASET], int nDatasetSrc, int nDatasetDest)
{
	if (!CopyDataset(datasets, nDatasetSrc, nDatasetDest))
		return FALSE;

	datasets[nDatasetSrc].Reset();
	return TRUE;
}

BOOL CGraphBase::CalculateTrendLines(CHMXDataset datasets[HMX_MAX_DATASET], DWORD dwTrends, int nDatasetSrc, int& nDatasetDest)
{
	// Sanity check
	if (nDatasetDest <= nDatasetSrc)
	{
		ASSERT(0);
		return FALSE;
	}

	int nStartDatasetDest = nDatasetDest;
	BOOL bSuccess = TRUE;

	for (int nTrend = 0; ((nTrend < NUM_TRENDS) && bSuccess); nTrend++)
	{
		if (dwTrends & TRENDS[nTrend].dwTrend)
		{
			switch (TRENDS[nTrend].dwTrend)
			{
			case BTL_BEST_FIT:
				bSuccess &= CalculateBestFitLine(datasets, nDatasetSrc, nDatasetDest++);
				break;

			case BTL_7DAY_MOVING_AVERAGE:
				bSuccess &= CalculateMovingAverage(datasets, nDatasetSrc, nDatasetDest++, 7);
				break;

			case BTL_30DAY_MOVING_AVERAGE:
				bSuccess &= CalculateMovingAverage(datasets, nDatasetSrc, nDatasetDest++, 30);
				break;

			default:
				ASSERT(0);
				break;
			}
		}
	}

	// If any failed, reset all
	if (!bSuccess)
		nDatasetDest = nStartDatasetDest;

	// Reset any unused datasets
	for (int nUnused = nDatasetDest; nUnused < (nStartDatasetDest + NUM_TRENDS); nUnused++)
	{
		datasets[nUnused].Reset();
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
	SetDatasetColor(datasets, 0, COLOR_GREEN);

	datasets[0].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[0].SetSize(LINE_THICKNESS);
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

BOOL CIncompleteTasksGraph::EnableTrends(DWORD dwTrends, CHMXDataset datasets[HMX_MAX_DATASET])
{
	if (dwTrends == m_dwTrends)
		return TRUE;

	int nTrendDataset = 1;

	if (CalculateTrendLines(datasets, dwTrends, 0, nTrendDataset))
	{
		m_dwTrends = dwTrends;
	}
	else
	{
		m_dwTrends = 0;
	}
		
	return (m_dwTrends == dwTrends);
}

//////////////////////////////////////////////////////////////////////////////////////

CString CRemainingDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_REMAINING);
}

void CRemainingDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets, REMAINING_ESTIMATE, COLOR_BLUE);
	SetDatasetColor(datasets, REMAINING_SPENT, COLOR_YELLOW);

	datasets[REMAINING_ESTIMATE].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_ESTIMATE].SetSize(LINE_THICKNESS);
	datasets[REMAINING_ESTIMATE].SetMin(0.0);
	
	datasets[REMAINING_SPENT].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[REMAINING_SPENT].SetSize(LINE_THICKNESS);
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

BOOL CRemainingDaysGraph::EnableTrends(DWORD dwTrends, CHMXDataset datasets[HMX_MAX_DATASET])
{
	// TODO
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////

CString CStartedEndedTasksGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_STARTEDENDED);
}

void CStartedEndedTasksGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets, STARTED_TASKS, COLOR_GREEN);
	SetDatasetColor(datasets, ENDED_TASKS, COLOR_RED);

	datasets[STARTED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[STARTED_TASKS].SetSize(LINE_THICKNESS);
	datasets[STARTED_TASKS].SetMin(0.0);

	datasets[ENDED_TASKS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ENDED_TASKS].SetSize(LINE_THICKNESS);
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

BOOL CStartedEndedTasksGraph::EnableTrends(DWORD dwTrends, CHMXDataset datasets[HMX_MAX_DATASET])
{
	// TODO
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CString CEstimatedSpentDaysGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTDAYS);
}

void CEstimatedSpentDaysGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets, ESTIMATED_DAYS, COLOR_PINK);
	SetDatasetColor(datasets, SPENT_DAYS, COLOR_GREEN);

	datasets[ESTIMATED_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_DAYS].SetSize(LINE_THICKNESS);
	datasets[ESTIMATED_DAYS].SetMin(0.0);

	datasets[SPENT_DAYS].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_DAYS].SetSize(LINE_THICKNESS);
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

BOOL CEstimatedSpentDaysGraph::EnableTrends(DWORD dwTrends, CHMXDataset datasets[HMX_MAX_DATASET])
{
	// TODO
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

CString CEstimatedSpentCostGraph::GetTitle() const
{
	return CEnString(IDS_DISPLAY_ESTIMATEDSPENTCOST);
}

void CEstimatedSpentCostGraph::BuildGraph(const CStatsItemCalculator& calculator, CHMXDataset datasets[HMX_MAX_DATASET]) const
{
	SetDatasetColor(datasets, ESTIMATED_COST, COLOR_PINK);
	SetDatasetColor(datasets, SPENT_COST, COLOR_GREEN);

	datasets[ESTIMATED_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[ESTIMATED_COST].SetSize(LINE_THICKNESS);
	datasets[ESTIMATED_COST].SetMin(0.0);

	datasets[SPENT_COST].SetStyle(HMX_DATASET_STYLE_AREALINE);
	datasets[SPENT_COST].SetSize(LINE_THICKNESS);
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

BOOL CEstimatedSpentCostGraph::EnableTrends(DWORD dwTrends, CHMXDataset datasets[HMX_MAX_DATASET])
{
	// TODO
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
